/*
 * A simple flash mapping code for BCM963xx board flash memory
 * It is simple because it only treats all the flash memory as ROM
 * It is used with chips/map_rom.c
 *
 *  Song Wang (songw@broadcom.com)
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
//#include <linux/config.h>
#include <linux/interrupt.h>
#include <board.h>
#include <bcmTag.h>
#include <bcm_map_part.h>
#include <atpconfig.h>
//#include "atpflashlayout.h"
#include <linux/vmalloc.h>

extern PFILE_TAG bhalGetTagFromPartition(int);

extern int bhalReadFlash(void *pszDataBuffer, unsigned long ulFlashAddr, unsigned long ulLen);
extern void ATP_FLASH_InitFlash(void);
extern int bhalGetFlashSize(void);
extern int ATP_FLASH_WriteFlash_Jffs(void *buf, unsigned long toaddr , int len);
extern int ATP_FLASH_EraseFlash_Jffs(unsigned long addr , int len);
extern int ATP_FLASH_ReadFlash(unsigned long ulFlashAddr,void *pszDataBuffer, unsigned long ulLen);

#ifdef CONFIG_ATP_SUPPORT_DOUBLE_SYSTEM
static int  getBootsystem(void);
extern PFILE_TAG bhalReadSlaveTag(void);
#endif

#define  BCM_MTD_VERSION	"2.1"	/* NOR FLASH ONLY */

const char * null_mtd_c = "NULLMTD";

/*
 * MTD Driver Entry Points using kerSys interface to flash_api
 *
 * Assumption:
 * - Single region with all sectors of the same size per MTD device registered.
 * - BankSize = 2
 *
 * Runtime spcification of device size, offset using Flash Partition Info.
 */

#define ALLOC_TYPE_KMALLOC   0
#define ALLOC_TYPE_VMALLOC   1

 void *retriedKmalloc(size_t size)
{
    void *pBuf;
    unsigned char *bufp8 ;

    size += 4 ; /* 4 bytes are used to store the housekeeping information used for freeing */

    // Memory allocation changed from kmalloc() to vmalloc() as the latter is not susceptible to memory fragmentation under low memory conditions
    // We have modified Linux VM to search all pages by default, it is no longer necessary to retry here
    if (!in_interrupt() ) {
        pBuf = vmalloc(size);
        if (pBuf) {
            memset(pBuf, 0, size);
            bufp8 = (unsigned char *) pBuf ;
            *bufp8 = ALLOC_TYPE_VMALLOC ;
            pBuf = bufp8 + 4 ;
        }
    }
    else { // kmalloc is still needed if in interrupt
        pBuf = kmalloc(size, GFP_ATOMIC);
        if (pBuf) {
            memset(pBuf, 0, size);
            bufp8 = (unsigned char *) pBuf ;
            *bufp8 = ALLOC_TYPE_KMALLOC ;
            pBuf = bufp8 + 4 ;
        }
    }

    return pBuf;
}

static int bcm63xx_erase_invalid(struct mtd_info *mtd, struct erase_info *instr)
{
    char * mtdname_p = (char*)null_mtd_c;
    if ( mtd ) mtdname_p = mtd->name;
    printk("ERROR: bcm63xx_erase_invalid( mtd[%s])\n", mtdname_p );

    /* Proceed as if done */
    instr->state = MTD_ERASE_DONE;
    mtd_erase_callback( instr );

    return (0);
}

static int bcm63xx_point_invalid(struct mtd_info *mtd, loff_t from,
			size_t len, size_t *retlen, u_char **mtdbuf)
{
    return (-EINVAL);
}

static void bcm63xx_unpoint_invalid(struct mtd_info *mtd, u_char * addr,
			loff_t from, size_t len)
{
}

static int bcm63xx_read(struct mtd_info *mtd, loff_t from, size_t len,
			size_t *retlen, u_char *buf)
{
    unsigned long flash_base;
    *retlen = 0;
    if ( mtd == (struct mtd_info *)NULL )
    {
        printk("ERROR: bcm63xx_read( mtd[%s])\n", null_mtd_c);
        return (-EINVAL);
    }
    flash_base = (unsigned long)mtd->priv;

    *retlen = bhalReadFlash(buf, flash_base + from, len); 
    if ( 0 == *retlen)
    {
        *retlen = len;
    }
    return 0;
}

static int bcm63xx_write_invalid(struct mtd_info *mtd, loff_t to, size_t len,
			size_t *retlen, const u_char *buf)
{
    char * mtdname_p = (char*)null_mtd_c;
    *retlen = 0;
    if ( mtd ) mtdname_p = mtd->name;
    printk("ERROR: bcm63xx_write_invalid( mtd[%s])\n", mtdname_p );

    return (-EINVAL);
}

static void bcm63xx_noop(struct mtd_info *mtd)
{
	/* NO OPERATION */
}

static map_word brcm_physmap_read16(struct map_info *map, unsigned long ofs)
{
	map_word val;

    ATP_FLASH_ReadFlash( map->map_priv_1 + ofs, &val.x[0], sizeof(short) );
	return val;
}

static void brcm_physmap_copy_from(struct map_info *map, void *to, unsigned long from, ssize_t len)
{
    ATP_FLASH_ReadFlash( map->map_priv_1 + from, to, len );
}
static void brcm_physmap_copy_to(struct map_info * map, unsigned long to, void * from, ssize_t len)
{
	//printk("brcm_physmap_copy_to 0x%lx : %d \n", to, len);
	ATP_FLASH_WriteFlash_Jffs(from, map->map_priv_1+to,len);	
}

static int contrib_writev (struct mtd_info *mtd, const struct kvec *vecs,
				unsigned long count, loff_t to, size_t *retlen)
{
    struct map_info *map = mtd->priv;
	int ret = 0;
	char *pBuff, *pBuff_temp;
	unsigned long ofs, i;
	size_t len = 0;
    
	for (i = 0; i < count; i++)
		len += vecs[i].iov_len;

	if(len == 0)
	{
		*retlen = len;
		
		return 0;
	}

	
	pBuff = pBuff_temp = NULL;
	
	pBuff_temp = pBuff = retriedKmalloc(len);
	if(pBuff)
	{		
		for (i = 0; i < count; i++)
		{
			memcpy(pBuff_temp , vecs[i].iov_base,vecs[i].iov_len);
			pBuff_temp += vecs[i].iov_len;
		}
		brcm_physmap_copy_to(map ,to , pBuff, len);

		*retlen = len;
	}else
	{
		return -EIO;
	}
	return 0;
}


static int contrib_erase (struct mtd_info *mtd, struct erase_info *instr)
{
	unsigned long ofs, len;
	int ret;
	struct map_info *map = mtd->priv;

	ofs = instr->addr;
	len = instr->len;

	//printk("contrib_physmap_erase 0x%lx : %d \n", ofs, len);
	ret = ATP_FLASH_EraseFlash_Jffs(map->map_priv_1+ofs, len);

	if(ret)
		return ret;

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);
	return 0;
}



/*---------------------------------------------------------------------------
 *	List of Broadcom MTD Devices per supported Flash File Systems
 *
 * - Non WRITEABLE MTD device for the Root FileSystem/kernel
 *   [e.g. the RootFS MTD may host SquashFS]
 * - WRITEABLE MTD device for an Auxillary FileSystem, if configured
 *   [e.g. the AuxFS MTD may host JFFS2]
 *---------------------------------------------------------------------------*/

static struct mtd_info mtdRootFS =
{
	.name		= "BCM63XX RootFS",
	.index		= -1,			/* not registered */
	.type		= MTD_NORFLASH,//MTD_NORFLASH,
	.flags		= 0,/* No capability: i.e. CLEAR/SET BITS, ERASEABLE */
	.size		= 0,
	.erasesize	= 0,				/* NO ERASE */
	.numeraseregions= 0,
	.eraseregions	= (struct mtd_erase_region_info*) NULL,
	.writesize  =1,
//	.bank_size	= 2,
	.read		= bcm63xx_read,
	.erase		= bcm63xx_erase_invalid,	/* READONLY */
	.write		= bcm63xx_write_invalid,	/* READONLY */
	.point		= bcm63xx_point_invalid,	/* No XIP */
	.unpoint	= bcm63xx_unpoint_invalid,	/* No XIP */
	.sync		= bcm63xx_noop,
	// NAND Flash Devices not supported: ecc, oob, kvec read/write
	.priv		= (void*) NULL,
	.owner		= THIS_MODULE
};

#if defined(CONFIG_ATP_SUPPORT_MODULE)
static struct map_info brcm_physmap_map_module = 
{
	.name		= "BCM63XX mtdAuxFS For module",
	.bankwidth	= 2,
	.read		= brcm_physmap_read16,
	.copy_from	= brcm_physmap_copy_from,
	.copy_to 	= brcm_physmap_copy_to
};
static struct mtd_info *ptmtdAuxFSModule;
#endif

#if defined(CONFIG_ATP_SUPPORT_MIDDLE_WARE)
static struct map_info brcm_physmap_map_middle = 
{
	.name		= "BCM63XX mtdAuxFS For Middle",
	.bankwidth	= 2,
	.read		= brcm_physmap_read16,
	.copy_from	= brcm_physmap_copy_from,
	.copy_to 	= brcm_physmap_copy_to
};

static struct mtd_info *ptmtdAuxFSMiddle;
#endif

static int __init init_brcm_physmap(void)
{
        unsigned int rootfs_addr, kernel_addr;
        PFILE_TAG pTag = (PFILE_TAG)NULL;
        
        ATP_FLASH_InitFlash();

	    printk("bcm963xx_mtd driver v%s\n", BCM_MTD_VERSION);
	/*
	 * Data fill the runtime configuration of MTD RootFS Flash Device
	 */
	//if (! (pTag = kerSysImageTagGet()) )
#ifdef  CONFIG_ATP_SUPPORT_DOUBLE_SYSTEM
    if (1 == getBootsystem())
    {    
        printk("\n==boot from slave kernel==\n");   

        /*Start modified by lvxin 00135113@20090505 for support double system and middleware*/
        if (! (pTag = bhalReadSlaveTag()))
        {
            printk("Failed to read image tag from slave system.\n");
            return -EIO;
        }
      
        //printk("\n===start to init mtd=333========\n");
        rootfs_addr = (unsigned int) 
        simple_strtoul(pTag->rootfsAddress, NULL, 10)+(BHAL_SLAVE_KERNEL_START_OFFSET-BHAL_MAIN_KERNEL_START_OFFSET)
                + BOOT_OFFSET;
        kernel_addr = (unsigned int)
        simple_strtoul(pTag->kernelAddress, NULL, 10)+(BHAL_SLAVE_KERNEL_START_OFFSET-BHAL_MAIN_KERNEL_START_OFFSET)
                + BOOT_OFFSET;
        
        if ((mtdRootFS.size = (unsigned int)(kernel_addr - rootfs_addr)) <= 0)
        {
            printk("Invalid RootFs size\n");
            return -EIO;
        }
        printk("\n rootfs_addr=0x%x kernel_addr=0x%x BOOT_OFFSET:0x%x \n",rootfs_addr,kernel_addr,BOOT_OFFSET);
        /*End modified by lvxin 00135113@20090505 for support double system and middleware*/
    }
    else
     {
            printk("\n==boot from main kernel==\n");
            if (! (pTag = bhalGetTagFromPartition(0)) )
            {
                printk("Failed to read image tag from flash\n");
                return -EIO;
            }

            rootfs_addr = (unsigned int) 
            simple_strtoul(pTag->rootfsAddress, NULL, 10)
                            + BOOT_OFFSET;
            kernel_addr = (unsigned int)
            simple_strtoul(pTag->kernelAddress, NULL, 10)
                            + BOOT_OFFSET;

            if ((mtdRootFS.size = (unsigned int)(kernel_addr - rootfs_addr)) <= 0)
            {
                printk("Invalid RootFs size\n");
                return -EIO;
            }
		
            printk("\n rootfs_addr=0x%x kernel_addr=0x%x  BOOT_OFFSET:0x%x  \n",rootfs_addr,kernel_addr,BOOT_OFFSET);

     }
#else
 	     printk("\n==boot from main kernel==\n");
            if (! (pTag = bhalGetTagFromPartition(0)) )
            {
                printk("Failed to read image tag from flash\n");
                return -EIO;
            }

            rootfs_addr = (unsigned int) 
            simple_strtoul(pTag->rootfsAddress, NULL, 10)
                            + BOOT_OFFSET;
            kernel_addr = (unsigned int)
            simple_strtoul(pTag->kernelAddress, NULL, 10)
                            + BOOT_OFFSET;

            if ((mtdRootFS.size = (kernel_addr - rootfs_addr)) <= 0)
            {
                printk("Invalid RootFs size\n");
                return -EIO;
            }
		else
		{
			printk("RootFs size= 0x%llx\n",mtdRootFS.size);
		}
		
            printk("\n rootfs_addr=0x%x kernel_addr=0x%x  BOOT_OFFSET:0x%x  \n",rootfs_addr,kernel_addr,BOOT_OFFSET);
		
#endif
	/*
	 * CAUTION:
	 * rootfs_addr is NOT ALIGNED WITH a sector boundary.
	 * As, RootFS MTD is not writeable and not explicit erase capability
	 * this is not an issue.
	 * Support for writeable RootFS mtd would need to take into account
	 * the offset of rootfs_addr from the sector base.
	 */
     mtdRootFS.priv = (void*)rootfs_addr;
    
     if ( add_mtd_device( &mtdRootFS ) )	/* Register Device RootFS */
     {
         printk("Failed to register device mtd[%s]\n", mtdRootFS.name);
         return -EIO;
     }
       
     printk("Registered device mtd[%s] dev[%d] Flash[0x%08x]\n",
             mtdRootFS.name, mtdRootFS.index,
             (int)mtdRootFS.priv);
 
 #if defined(CONFIG_ATP_SUPPORT_MODULE)
     if (1 == getBootsystem()) //boot from slave
     {
        //brcm_physmap_map_module.size = (unsigned int)(BHAL_SLAVE_MODULE_SIZE);
        //brcm_physmap_map_module.map_priv_1 = (unsigned int)(BHAL_SLAVE_MODULE_START_OFFSET) + (unsigned int)(FLASH_BASE);
        brcm_physmap_map_module.size = (unsigned int)(BHAL_MAIN_MODULE_SIZE);
        brcm_physmap_map_module.map_priv_1 = (unsigned int)(BHAL_MAIN_MODULE_START_OFFSET) + (unsigned int)(FLASH_BASE);               
     }
     else //boot from main
     {
        brcm_physmap_map_module.size = (unsigned int)(BHAL_MAIN_MODULE_SIZE);
        brcm_physmap_map_module.map_priv_1 = (unsigned int)(BHAL_MAIN_MODULE_START_OFFSET) + (unsigned int)(FLASH_BASE);        
     }

     ptmtdAuxFSModule = do_map_probe("map_rom", &brcm_physmap_map_module);

     if (ptmtdAuxFSModule) 
     {
 		ptmtdAuxFSModule->owner = THIS_MODULE;
 		ptmtdAuxFSModule->erasesize = FLASH_LENGTH_BOOT_ROM;
 		ptmtdAuxFSModule->erase = contrib_erase;
 		ptmtdAuxFSModule->writev = contrib_writev;
        ptmtdAuxFSModule->flags |= MTD_WRITEABLE;
 		add_mtd_device(ptmtdAuxFSModule);
     }
     printk("Registered device mtd[%s] dev[%d] Flash[0x%08x,%u]\n",
                     ptmtdAuxFSModule->name, ptmtdAuxFSModule->index,
		        (int)brcm_physmap_map_module.map_priv_1, brcm_physmap_map_module.size);
#endif

#if defined(CONFIG_ATP_SUPPORT_MIDDLE_WARE) 
	        brcm_physmap_map_middle.size = (unsigned int)(BHAL_MIDDLEWARE_FS_SIZE);
	        brcm_physmap_map_middle.map_priv_1 = (unsigned int)(BHAL_MIDDLEWARE_FS_START_OFFSET) + (unsigned int)(FLASH_BASE);

	        ptmtdAuxFSMiddle = do_map_probe("map_rom", &brcm_physmap_map_middle);

	        if (ptmtdAuxFSMiddle) 
	        {
        		ptmtdAuxFSMiddle->owner = THIS_MODULE;
        		ptmtdAuxFSMiddle->erasesize = FLASH_LENGTH_BOOT_ROM;
        		ptmtdAuxFSMiddle->erase = contrib_erase;
        		ptmtdAuxFSMiddle->writev = contrib_writev;
           		ptmtdAuxFSMiddle->flags |= MTD_WRITEABLE;
        		add_mtd_device(ptmtdAuxFSMiddle);
	        }
            printk("Registered device mtd[%s] dev[%d] Flash[0x%08x,%u]\n",
                            ptmtdAuxFSMiddle->name, ptmtdAuxFSMiddle->index,
			        (int)brcm_physmap_map_middle.map_priv_1, brcm_physmap_map_middle.size);
#endif

	return 0;
}

static void __exit cleanup_brcm_physmap(void)
{
	if (mtdRootFS.index >= 0)
	{
		mtdRootFS.index = -1;

		del_mtd_device( &mtdRootFS );

        	mtdRootFS.size = 0;
		mtdRootFS.priv = (void*)NULL;
	}

#if defined(CONFIG_ATP_SUPPORT_MODULE)
	if (ptmtdAuxFSModule->index >= 0)
	{
		ptmtdAuxFSModule->index = -1;

		del_mtd_device( ptmtdAuxFSModule );

		ptmtdAuxFSModule->size = 0;
		ptmtdAuxFSModule->priv = (void*)NULL;
		ptmtdAuxFSModule->erasesize = 0;
		ptmtdAuxFSModule->numeraseregions = 0;
		ptmtdAuxFSModule->eraseregions->offset = 0;
		ptmtdAuxFSModule->eraseregions->erasesize = 0;
		ptmtdAuxFSModule->eraseregions->numblocks = 0;
	}
#endif	

#if defined(CONFIG_ATP_SUPPORT_MIDDLE_WARE)
	if (ptmtdAuxFSMiddle->index >= 0)
	{
		ptmtdAuxFSMiddle->index = -1;

		del_mtd_device( ptmtdAuxFSMiddle );

		ptmtdAuxFSMiddle->size = 0;
		ptmtdAuxFSMiddle->priv = (void*)NULL;
		ptmtdAuxFSMiddle->erasesize = 0;
		ptmtdAuxFSMiddle->numeraseregions = 0;
		ptmtdAuxFSMiddle->eraseregions->offset = 0;
		ptmtdAuxFSMiddle->eraseregions->erasesize = 0;
		ptmtdAuxFSMiddle->eraseregions->numblocks = 0;
	}
#endif
}

#ifdef CONFIG_ATP_SUPPORT_DOUBLE_SYSTEM

static int getBootsystem()
{
    int	    flashTotalSize =  0;
    unsigned char        pszStartInfo[BOOT_TAG_SIZE];
    signed int            lRet;
    
    memset (pszStartInfo,0,sizeof(pszStartInfo));
    flashTotalSize= bhalGetFlashSize();
#if !defined(CONFIG_ATP_SUPPORT_BIG_SYSTEM) && !defined(CONFIG_ATP_SUPPORT_SMALL_SYSTEM)
    //假如FLASH小于16M，就不进行双系统升级
    if (flashTotalSize < DOUBLE_FLASHSIZE)
    {
        printk("\n Flashsize is less than 16M,no suport double system.\n");
        return 0 ;
    }
#endif

    lRet = bhalReadFlash((void *)pszStartInfo,
                             FLASH_BASE + flashTotalSize - BOOT_TAG_SIZE,
                             BOOT_TAG_SIZE);     

    /*IMAGE_SLAVE_S/IMAGE_MAIN_F需要从SLAVE启动*/
    if ((NULL != strstr(pszStartInfo, IMAGE_SLAVE_S)) || (NULL != strstr(pszStartInfo, IMAGE_MAIN_F)) )
    {
    	 return 1;
    }   

   return 0;     
}
#endif

module_init(init_brcm_physmap);
module_exit(cleanup_brcm_physmap);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Song Wang songw@broadcom.com");
MODULE_DESCRIPTION("Configurable MTD Driver for Flash File System");
