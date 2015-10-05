/*
 *
 *  drivers/mtd/brcmnand/bcm7xxx-nand.c
 *
    Copyright (c) 2005-2006 Broadcom Corporation                 
    
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 2 as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    File: bcm7xxx-nand.c

    Description: 
    This is a device driver for the Broadcom NAND flash for bcm97xxx boards.
when    who what
-----   --- ----
051011  tht codings derived from OneNand generic.c implementation.

 * THIS DRIVER WAS PORTED FROM THE 2.6.18-7.2 KERNEL RELEASE
 */
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <bcm_map_part.h>
#include <board.h>
#include "brcmnand_priv.h"
#include "atpflash.h"
#include "flash_api.h"
#include "atpnandparts.h"
#include "atp_system_boot.h"
#include "kerneltag.h"
//#define PRINTK(...)
#define PRINTK printk

extern unsigned char g_blparms_buf[];
extern int g_flash_total_size;
extern int g_flash_sector_size;


#define DRIVER_NAME     "brcmnand"
#define DRIVER_INFO     "Broadcom DSL NAND controller"
int kerSysBlParmsGetInt( char *name, int *pvalue );
int kerSysBlParmsGetStr( char *name, char *pvalue, int size );
static int __devinit brcmnanddrv_probe(struct platform_device *pdev);
static int __devexit brcmnanddrv_remove(struct platform_device *pdev);
static int __devinit getTagFromSlaveRootfs(ATP_UPG_Tag_ST *pt_upgTag);
static int __devinit getTagFromMainRootfs(ATP_UPG_Tag_ST *pt_upgTag);

#if defined(CONFIG_MIPS_BRCM)
/* Only used for data storage. */


static struct mtd_partition bcm63XX_nand_parts[] = 
{
    {name: "rootfs",        offset: 0, size: 0},//mtd0
    {name: "rootfsbak",     offset: 0, size: 0},//mtd1
    {name: "config",        offset: 0, size: 0},//mtd2
    {name: "equip",        offset: 0, size: 0},    //mtd3
    {name: "upgflag",        offset: 0, size: 0},//mtd4
    {name: "blrom",        offset: 0, size: 0},//mtd5
    {name: "rootfstag",        offset: 0, size: 0},//mtd6
    {name: "reserved",        offset: 0, size: 0},//mtd7
    {name: "rootfsbaktag",        offset: 0, size: 0},//mtd8
    {name: NULL,            offset: 0, size: 0}
};


#endif

static struct platform_driver brcmnand_platform_driver =
{
    .probe      = brcmnanddrv_probe,
    .remove     = __devexit_p(brcmnanddrv_remove),
    .driver     =
     {
        .name   = DRIVER_NAME,
     },
};

static struct resource brcmnand_resources[] =
{
    [0] = {
            .name   = DRIVER_NAME,
            .start  = BPHYSADDR(BCHP_NAND_REG_START),
            .end    = BPHYSADDR(BCHP_NAND_REG_END) + 3,
            .flags  = IORESOURCE_MEM,
          },
};

static struct brcmnand_info
{
    struct mtd_info mtd;
    struct brcmnand_chip brcmnand;
    int nr_parts;
    struct mtd_partition* parts;
} *gNandInfo[NUM_NAND_CS];

int gNandCS[NAND_MAX_CS];
/* Number of NAND chips, only applicable to v1.0+ NAND controller */
int gNumNand = 0;
int gClearBBT = 0;
char gClearCET = 0;
uint32_t gNandTiming1[NAND_MAX_CS], gNandTiming2[NAND_MAX_CS];
uint32_t gAccControl[NAND_MAX_CS], gNandConfig[NAND_MAX_CS];

static unsigned long t1[NAND_MAX_CS] = {0};
static int nt1 = 0;
static unsigned long t2[NAND_MAX_CS] = {0};
static int nt2 = 0;
static unsigned long acc[NAND_MAX_CS] = {0};
static int nacc = 0;
static unsigned long nandcfg[NAND_MAX_CS] = {0};
static int ncfg = 0;
static void* gPageBuffer = NULL;

#if defined(CONFIG_MIPS_BRCM)
static void __devinit 
brcmnanddrv_setup_mtd_partitions(struct brcmnand_info* nandinfo)
{
    struct mtd_info* mtd = &nandinfo->mtd;
    unsigned char ucBootFlag[IMAGE_FLAG_SIZE];    
    char upgflag[16];
    unsigned int ulMainOrSlave;

    memset(upgflag,0,ATP_BOARD_ID_LEN);
    memcpy(upgflag,g_blparms_buf + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN,ATP_BOARD_ID_LEN);
    PRINTK("upgflag:%s \n",upgflag);

    /* 这里的len是为了不挂载Tag，它不是jffs2格式的 */
    if((0 == memcmp(upgflag, IMAGE_SLAVE_S, strlen(IMAGE_SLAVE_S)))
       || (0 == memcmp(upgflag, IMAGE_MAIN_F, strlen(IMAGE_MAIN_F))))
    {
        ulMainOrSlave = 1;
    }
    else
    {
        ulMainOrSlave = 0;
    }

    atp_setup_mtd_partitions(mtd, g_blparms_buf + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN,
            ulMainOrSlave, &nandinfo->parts, &nandinfo->nr_parts);

}
#else
static void __devinit 
brcmnanddrv_setup_mtd_partitions(struct brcmnand_info* nandinfo)
{
    int boot_from_nand = 1;

#if defined(CONFIG_BCM96368)
    if( ((GPIO->StrapBus & MISC_STRAP_BUS_BOOT_SEL_MASK) >>
        MISC_STRAP_BUS_BOOT_SEL_SHIFT) != MISC_STRAP_BUS_BOOT_NAND )
    {
        boot_from_nand = 0;
    }
#elif defined(CONFIG_BCM96362) || defined(CONFIG_BCM96328) || defined(CONFIG_BCM96816) || defined(CONFIG_BCM963268)
    if( ((MISC->miscStrapBus & MISC_STRAP_BUS_BOOT_SEL_MASK) >>
        MISC_STRAP_BUS_BOOT_SEL_SHIFT) != MISC_STRAP_BUS_BOOT_NAND )
    {
        boot_from_nand = 0;
    }
#endif

    if( boot_from_nand == 0 )
    {
        nandinfo->nr_parts = 1;
        nandinfo->parts = bcm63XX_nand_parts;

        bcm63XX_nand_parts[0].name = "data";
        bcm63XX_nand_parts[0].offset = 0;
        if( device_size(&(nandinfo->mtd)) < NAND_BBT_THRESHOLD_KB )
        {
            bcm63XX_nand_parts[0].size =
                device_size(&(nandinfo->mtd)) - (NAND_BBT_SMALL_SIZE_KB*1024);
        }
        else
        {
            bcm63XX_nand_parts[0].size =
                device_size(&(nandinfo->mtd)) - (NAND_BBT_BIG_SIZE_KB*1024);
        }
        bcm63XX_nand_parts[0].ecclayout = nandinfo->mtd.ecclayout;

        PRINTK("Part[0] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[0].name,
            bcm63XX_nand_parts[0].size, bcm63XX_nand_parts[0].offset);
    }
    else
    {
        static NVRAM_DATA nvram;
        struct mtd_info* mtd = &nandinfo->mtd;
        unsigned long rootfs_ofs;
        int rootfs, rootfs_update;

        kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs);
        kerSysNvRamGet((char *)&nvram, sizeof(nvram), 0);
        nandinfo->nr_parts = 4;
        nandinfo->parts = bcm63XX_nand_parts;

        /* Root FS.  The CFE RAM boot loader saved the rootfs offset that the
         * Linux image was loaded from.
         */
        PRINTK("rootfs_ofs=0x%8.8lx, part1ofs=0x%8.8lx, part2ofs=0x%8.8lx\n",
            rootfs_ofs, nvram.ulNandPartOfsKb[NP_ROOTFS_1],
            nvram.ulNandPartOfsKb[NP_ROOTFS_2]);
        if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
        {
            rootfs = NP_ROOTFS_1;
            rootfs_update = NP_ROOTFS_2;
        }
        else
        {
            if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_2] )
            {
                rootfs = NP_ROOTFS_2;
                rootfs_update = NP_ROOTFS_1;
            }
            else
            {
                /* Backward compatibility with old cferam. */
                extern unsigned char _text;
                unsigned long rootfs_ofs = *(unsigned long *) (&_text - 4);

                if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
                {
                    rootfs = NP_ROOTFS_1;
                    rootfs_update = NP_ROOTFS_2;
                }
                else
                {
                    rootfs = NP_ROOTFS_2;
                    rootfs_update = NP_ROOTFS_1;
                }
            }
        }

        bcm63XX_nand_parts[0].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
        bcm63XX_nand_parts[0].size = nvram.ulNandPartSizeKb[rootfs]*1024;
        bcm63XX_nand_parts[0].ecclayout = mtd->ecclayout;
        bcm63XX_nand_parts[1].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
        bcm63XX_nand_parts[1].size = nvram.ulNandPartSizeKb[rootfs_update]*1024;
        bcm63XX_nand_parts[1].ecclayout = mtd->ecclayout;

        /* Data (psi, scratch pad) */
        bcm63XX_nand_parts[2].offset = nvram.ulNandPartOfsKb[NP_DATA] * 1024;
        bcm63XX_nand_parts[2].size = nvram.ulNandPartSizeKb[NP_DATA] * 1024;
        bcm63XX_nand_parts[2].ecclayout = mtd->ecclayout;

        /* Boot and NVRAM data */
        bcm63XX_nand_parts[3].offset = nvram.ulNandPartOfsKb[NP_BOOT] * 1024;
        bcm63XX_nand_parts[3].size = nvram.ulNandPartSizeKb[NP_BOOT] * 1024;
        bcm63XX_nand_parts[3].ecclayout = mtd->ecclayout;

        PRINTK("Part[0] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[0].name,
            bcm63XX_nand_parts[0].size, bcm63XX_nand_parts[0].offset);
        PRINTK("Part[1] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[1].name,
            bcm63XX_nand_parts[1].size, bcm63XX_nand_parts[1].offset);
        PRINTK("Part[2] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[2].name,
            bcm63XX_nand_parts[2].size, bcm63XX_nand_parts[2].offset);
        PRINTK("Part[3] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[3].name,
            bcm63XX_nand_parts[3].size, bcm63XX_nand_parts[3].offset);
    }
}
#endif

static int __devinit brcmnanddrv_probe(struct platform_device *pdev)
{
    static int csi = 0; // Index into dev/nandInfo array
    int cs = 0;  // Chip Select
    int err = 0;
    struct brcmnand_info* info = NULL;
    static struct brcmnand_ctrl* ctrl = (struct brcmnand_ctrl*) 0;

    if(!gPageBuffer &&
       (gPageBuffer = kmalloc(sizeof(struct nand_buffers),GFP_KERNEL)) == NULL)
    {
        err = -ENOMEM;
    }
    else
    {
        if( (ctrl = kmalloc(sizeof(struct brcmnand_ctrl), GFP_KERNEL)) != NULL)
        {
            memset(ctrl, 0, sizeof(struct brcmnand_ctrl));
            ctrl->state = FL_READY;
            init_waitqueue_head(&ctrl->wq);
            spin_lock_init(&ctrl->chip_lock);

            if((info=kmalloc(sizeof(struct brcmnand_info),GFP_KERNEL)) != NULL)
            {
                gNandInfo[csi] = info;
                memset(info, 0, sizeof(struct brcmnand_info));
                info->brcmnand.ctrl = ctrl;
                info->brcmnand.ctrl->numchips = gNumNand = 1;
                info->brcmnand.csi = csi;

                /* For now all devices share the same buffer */
                info->brcmnand.ctrl->buffers =
                    (struct nand_buffers*) gPageBuffer;

                info->brcmnand.ctrl->numchips = gNumNand; 
                info->brcmnand.chip_shift = 0; // Only 1 chip
                info->brcmnand.priv = &info->mtd;
                info->mtd.name = dev_name(&pdev->dev);
                info->mtd.priv = &info->brcmnand;
                info->mtd.owner = THIS_MODULE;

                /* Enable the following for a flash based bad block table */
                info->brcmnand.options |= NAND_USE_FLASH_BBT;

                /* Each chip now will have its own BBT (per mtd handle) */
                if (brcmnand_scan(&info->mtd, cs, gNumNand) == 0)
                {
                    PRINTK("Master size=%08llx\n", info->mtd.size); 
                    brcmnanddrv_setup_mtd_partitions(info);
                    add_mtd_partitions(&info->mtd, info->parts, info->nr_parts);
                    dev_set_drvdata(&pdev->dev, info);
                }
                else
                    err = -ENXIO;

            }
            else
                err = -ENOMEM;

        }
        else
            err = -ENOMEM;
    }

    if( err )
    {
        if( gPageBuffer )
        {
            kfree(gPageBuffer);
            gPageBuffer = NULL;
        }

        if( ctrl )
        {
            kfree(ctrl);
            ctrl = NULL;
        }

        if( info )
        {
            kfree(info);
            info = NULL;
        }
    }

    return( err );
}

static int __devexit brcmnanddrv_remove(struct platform_device *pdev)
{
    struct brcmnand_info *info = dev_get_drvdata(&pdev->dev);

    dev_set_drvdata(&pdev->dev, NULL);

    if (info)
    {
        del_mtd_partitions(&info->mtd);

        brcmnand_release(&info->mtd);
        kfree(gPageBuffer);
        kfree(info);
    }

    return 0;
}

static int __init brcmnanddrv_init(void)
{
    int ret = 0;
    int csi;
    int ncsi;
    char cmd[32] = "\0";
    struct platform_device *pdev;

    kerSysBlParmsGetStr("NANDCMD", cmd, sizeof(cmd));

    if (cmd[0])
    {
        if (strcmp(cmd, "rescan") == 0)
            gClearBBT = 1;
        else if (strcmp(cmd, "showbbt") == 0)
            gClearBBT = 2;
        else if (strcmp(cmd, "eraseall") == 0)
            gClearBBT = 8;
        else if (strcmp(cmd, "erase") == 0)
            gClearBBT = 7;
        else if (strcmp(cmd, "clearbbt") == 0)
            gClearBBT = 9;
        else if (strcmp(cmd, "showcet") == 0)
            gClearCET = 1;
        else if (strcmp(cmd, "resetcet") == 0)
            gClearCET = 2;
        else if (strcmp(cmd, "disablecet") == 0)
            gClearCET = 3;
        else
            printk(KERN_WARNING "%s: unknown command '%s'\n",
                __FUNCTION__, cmd);
    }
    
    for (csi=0; csi<NAND_MAX_CS; csi++)
    {
        gNandTiming1[csi] = 0;
        gNandTiming2[csi] = 0;
        gAccControl[csi] = 0;
        gNandConfig[csi] = 0;
    }

    if (nacc == 1)
        PRINTK("%s: nacc=%d, gAccControl[0]=%08lx, gNandConfig[0]=%08lx\n", \
            __FUNCTION__, nacc, acc[0], nandcfg[0]);

    if (nacc>1)
        PRINTK("%s: nacc=%d, gAccControl[1]=%08lx, gNandConfig[1]=%08lx\n", \
            __FUNCTION__, nacc, acc[1], nandcfg[1]);

    for (csi=0; csi<nacc; csi++)
        gAccControl[csi] = acc[csi];

    for (csi=0; csi<ncfg; csi++)
        gNandConfig[csi] = nandcfg[csi];

    ncsi = max(nt1, nt2);
    for (csi=0; csi<ncsi; csi++)
    {
        if (nt1 && csi < nt1)
            gNandTiming1[csi] = t1[csi];

        if (nt2 && csi < nt2)
            gNandTiming2[csi] = t2[csi];
        
    }

    printk (KERN_INFO DRIVER_INFO " (BrcmNand Controller)\n");
    if( (pdev = platform_device_alloc(DRIVER_NAME, 0)) != NULL )
    {
        platform_device_add(pdev);
        platform_device_put(pdev);
        ret = platform_driver_register(&brcmnand_platform_driver);
        if (ret >= 0)
            request_resource(&iomem_resource, &brcmnand_resources[0]);
        else
            printk("brcmnanddrv_init: driver_register failed, err=%d\n", ret);
    }
    else
        ret = -ENODEV;

    return ret;
}

/*!
  \brief 获取从系统的Tag
  \param[in]  *pt_upgTag: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
static int  getTagFromSlaveRootfs(ATP_UPG_Tag_ST *pt_upgTag)
{
    ATP_UPG_Tag_ST* buf = (ATP_UPG_Tag_ST*) 0;

    int len = flash_get_sector_size(0);

    unsigned char *p;
    int i;
    ATP_UPG_Tag_ST *pkernelfsTag;

    /* 这个Slave kernel_img_magic 必定在flash中1/4到1/2的地方，逆序寻找效率高 */
    int start_blk = (flash_get_total_size()/len)/2;
    int end_blk = start_blk/2;

    if((buf = kmalloc(sizeof(ATP_UPG_Tag_ST),GFP_KERNEL))==NULL)
    {
        PRINTK("<<<<<<%s,%d,malloc error\n",__FILE__,__LINE__);
        return -1;
    }
    
    PRINTK("<<<<<<%s,%d,start_blk=%d,end_blk=%d>>>>\n",__FILE__,__LINE__,start_blk,end_blk);
    
    /* Find the directory entry. */
    for( i = start_blk ; i > end_blk ; i-- )
    {
        if( flash_read_buf(i, 0, (unsigned char *)buf, sizeof(ATP_UPG_Tag_ST)) > 0 )
        {
            p = (unsigned char *)buf;        
            pkernelfsTag = (ATP_UPG_Tag_ST *) p;
            if(((*pkernelfsTag).basic.stBasicInfo.ulMagicNumber) == ATP_KERNEL_IMG_MAGIC_NUM )
            {
                PRINTK("<<<<<<%s,%d,I have find Slave KERNEL IMAGE MAGIC NUMBER,I am in bolock %d,end_blk=%d>>>>\n",__FILE__,__LINE__,i,end_blk);
                memcpy(pt_upgTag,pkernelfsTag,sizeof(ATP_UPG_Tag_ST));
                kfree(buf);
                return 0;
            }
            else
            {
                ;
            }
        }
        else
        {
            PRINTK("<<<<<<%s,%d,Read buf failed>>>>\n",__FILE__,__LINE__);
            kfree(buf);
            return -1 ;
        }
    }
    
    PRINTK("<<<<<<%s,%d,I have not find Slave KERNEL IMAGE MAGIC NUMBER,I am in bolock %d,end_blk=%d>>>>\n",__FILE__,__LINE__,i,end_blk);
    kfree(buf);
    return -1 ;
}

/*!
  \brief 获取主系统的Tag
  \param[in]  *pt_upgTag: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
static int  getTagFromMainRootfs(ATP_UPG_Tag_ST *pt_upgTag)
{
    ATP_UPG_Tag_ST* buf = (ATP_UPG_Tag_ST*) 0;

    int len = flash_get_sector_size(0);

    unsigned char *p;
    int i;
    ATP_UPG_Tag_ST *pkernelfsTag;

    /* 这个Main kernel_img_magic 必定在flash中0到1/4的地方，正序早效率高 */
    int start_blk = 0;
    int end_blk = (flash_get_total_size()/len)/4;
    
    if((buf = kmalloc(sizeof(ATP_UPG_Tag_ST),GFP_KERNEL))==NULL)
    {
        PRINTK("<<<<<<%s,%d,malloc error\n",__FILE__,__LINE__);
        return -1;
    }
    
    PRINTK("<<<<<<%s,%d,start_blk=%d,end_blk=%d>>>>\n",__FILE__,__LINE__,start_blk,end_blk);

    /* Find the directory entry. */
    for( i = start_blk ; i < end_blk ; i++ )
    {
        if( flash_read_buf(i, 0, (unsigned char *)buf, sizeof(ATP_UPG_Tag_ST)) > 0 )
        {
            p = (char*)buf;        
            pkernelfsTag = (ATP_UPG_Tag_ST *) p;
            if(((*pkernelfsTag).basic.stBasicInfo.ulMagicNumber) == ATP_KERNEL_IMG_MAGIC_NUM )
            {
                PRINTK("<<<<<<%s,%d,I have find Main KERNEL IMAGE MAGIC NUMBER,I am in bolock %d,end_blk=%d>>>>\n",__FILE__,__LINE__,i,end_blk);
                memcpy(pt_upgTag,pkernelfsTag,sizeof(ATP_UPG_Tag_ST));
                kfree(buf);
                return 0;
            }
            else
            {
                ;
            }
        }
        else
        {
            PRINTK("<<<<<<%s,%d,Read buf failed>>>>\n",__FILE__,__LINE__);
            kfree(buf);
            return -1 ;
        }
    }
    
    PRINTK("<<<<<<%s,%d,I have not find Main KERNEL IMAGE MAGIC NUMBER>>>>\n",__FILE__,__LINE__);
    kfree(buf);
    return -1 ;
}


/***********************************************************************
 * Function Name: kerSysBlParmsGetInt
 * Description  : Returns the integer value for the requested name from
 *                the boot loader parameter buffer.
 * Returns      : 0 - success, -1 - failure
 ***********************************************************************/
 extern unsigned char g_blparms_buf[];
int kerSysBlParmsGetInt( char *name, int *pvalue )
{
    char *p2, *p1 = g_blparms_buf;
    int ret = -1;

    *pvalue = -1;

    /* The g_blparms_buf buffer contains one or more contiguous NULL termianted
     * strings that ends with an empty string.
     */
    while( *p1 )
    {
        p2 = p1;

        while( *p2 != '=' && *p2 != '\0' )
            p2++;

        if( *p2 == '=' )
        {
            *p2 = '\0';

            if( !strcmp(p1, name) )
            {
                *p2++ = '=';
                *pvalue = simple_strtol(p2, &p1, 0);
                if( *p1 == '\0' )
                    ret = 0;
                break;
            }

            *p2 = '=';
        }

        p1 += strlen(p1) + 1;
    }

    return( ret );
}

/***********************************************************************
 * Function Name: kerSysBlParmsGetStr
 * Description  : Returns the string value for the requested name from
 *                the boot loader parameter buffer.
 * Returns      : 0 - success, -1 - failure
 ***********************************************************************/
int kerSysBlParmsGetStr( char *name, char *pvalue, int size )
{
    char *p2, *p1 = g_blparms_buf;
    int ret = -1;

    /* The g_blparms_buf buffer contains one or more contiguous NULL termianted
     * strings that ends with an empty string.
     */
    while( *p1 )
    {
        p2 = p1;

        while( *p2 != '=' && *p2 != '\0' )
            p2++;

        if( *p2 == '=' )
        {
            *p2 = '\0';

            if( !strcmp(p1, name) )
            {
                *p2++ = '=';
                strncpy(pvalue, p2, size);
                ret = 0;
                break;
            }

            *p2 = '=';
        }

        p1 += strlen(p1) + 1;
    }

    return( ret );
}

static void __exit brcmnanddrv_exit(void)
{
    release_resource(&brcmnand_resources[0]);
    platform_driver_unregister(&brcmnand_platform_driver);
}


module_init(brcmnanddrv_init);
module_exit(brcmnanddrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ton Truong <ttruong@broadcom.com>");
MODULE_DESCRIPTION("Broadcom NAND flash driver");

