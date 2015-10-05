/*
    Copyright 2000-2010 Broadcom Corporation

    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the 揋PL?, available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:

        As a special exception, the copyright holders of this software give
        you permission to link this software with independent modules, and to
        copy and distribute the resulting executable under terms of your
        choice, provided that you also meet, for each linked independent
        module, the terms and conditions of the license of that module. 
        An independent module is a module which is not derived from this
        software.  The special exception does not apply to any modifications
        of the software.

    Notwithstanding the above, under no circumstances may you combine this
    software in any way with any other Broadcom software provided under a
    license other than the GPL, without Broadcom's express prior written
    consent.
*/                       

/***************************************************************************
 * File Name  : flash_api.c
 *
 * Description: This file contains the implementation of the wrapper functions
 *              for the flash device interface.
 ***************************************************************************/

/** Includes. */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/gen_probe.h>


#include "bcmtypes.h"
#include "bcm_hwdefs.h"
#include "flash_api_bcm.h"
#include <linux/version.h>

/*change by l63336 for 2.6.30*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif
#include "atpconfig.h"
/*change by l63336 for 2.6.30*/

#include "bcm_map_part.h"

#include "hal_retvalue.h"

/** Externs. **/
#if !defined(INC_CFI_FLASH_DRIVER)
#define INC_CFI_FLASH_DRIVER        0  
#endif

#if !defined(INC_SPI_FLASH_DRIVER)
#define INC_SPI_FLASH_DRIVER        1
#endif

#if !defined(INC_NAND_FLASH_DRIVER)
#define INC_NAND_FLASH_DRIVER       0
#endif

#if (INC_CFI_FLASH_DRIVER==1)
extern int cfi_flash_init(flash_device_info_t **flash_info);
#else
#define cfi_flash_init(x)           FLASH_API_ERROR
#endif

#if (INC_SPI_FLASH_DRIVER==1)
extern int spi_flash_init(flash_device_info_t **flash_info);
#else
#define spi_flash_init(x)           FLASH_API_ERROR
#endif

#if (INC_NAND_FLASH_DRIVER==1)
extern int nand_flash_init(flash_device_info_t **flash_info);
#else
#define nand_flash_init(x)           FLASH_API_ERROR
#endif

static struct semaphore semflash;

/** Variables. **/
static flash_device_info_t *g_flash_info = NULL;

#ifndef BHAL_FLASH_BASE
#define BHAL_FLASH_BASE                      0xB8000000
#endif



/***************************************************************************
 * Function Name: display_flash_info
 * Description  : Displays information about the flash part.
 * Returns      : None.
 ***************************************************************************/
static void display_flash_info(int ret, flash_device_info_t *flash_info)
{
    switch (flash_info->flash_type) {
    case FLASH_IFC_PARALLEL:
        printk( "Parallel flash device");
        break;

    case FLASH_IFC_SPI:
        printk( "Serial flash device");
        break;

    case FLASH_IFC_HS_SPI:
        printk( "HS Serial flash device");
        break;

    case FLASH_IFC_NAND:
        printk( "NAND flash device");
        break;
    }

    if( ret == FLASH_API_OK ) {
        printk(": name %s, id 0x%4.4x",
            flash_info->flash_device_name, flash_info->flash_device_id);
        if(g_flash_info)
        {
            printk(" size %dKB", (*g_flash_info->fn_flash_get_total_size)() / 1024);       
        }
        else
        {
            printk(" size %dKB", 0);
        }
       
        printk("\n");
    }
    else {
        printk( " id %4.4x is not supported.\n", flash_info->flash_device_id );
    }
} /* display_flash_info */

/***************************************************************************
 * Function Name: flash_init
 * Description  : Initialize flash part.
 * Returns      : FLASH_API_OK or FLASH_API_ERROR
 ***************************************************************************/
int flash_init_info(void)
{
    int type = FLASH_IFC_UNKNOWN;
    int ret = FLASH_API_ERROR;

    /* If available, use bootstrap to decide which flash to use */
#if defined(_BCM96816_) || defined(CONFIG_BCM96816) || defined(_BCM96368_) || defined(CONFIG_BCM96368)
    unsigned int bootsel;
#if defined(_BCM96816_) || defined(CONFIG_BCM96816)
    bootsel = MISC->miscStrapBus;
#elif defined(_BCM96368_) || defined(CONFIG_BCM96368)
    bootsel = GPIO->StrapBus;
#endif
    switch ((bootsel & MISC_STRAP_BUS_BOOT_SEL_MASK)>>MISC_STRAP_BUS_BOOT_SEL_SHIFT) {
    case MISC_STRAP_BUS_BOOT_PARALLEL:
        type = FLASH_IFC_PARALLEL;
        break;

    case MISC_STRAP_BUS_BOOT_SERIAL:
        type = FLASH_IFC_SPI;
        break;

    case MISC_STRAP_BUS_BOOT_NAND:
        type = FLASH_IFC_NAND;
        break;

    }
#elif defined(_BCM96362_) || defined(CONFIG_BCM96362) || defined(_BCM96328_) || defined(CONFIG_BCM96328) || defined(_BCM963268_) || defined(CONFIG_BCM963268) 
    if( ((MISC->miscStrapBus & MISC_STRAP_BUS_BOOT_SEL_MASK) >>
        MISC_STRAP_BUS_BOOT_SEL_SHIFT) ==  MISC_STRAP_BUS_BOOT_SERIAL )
        type = FLASH_IFC_SPI;
    else
        type = FLASH_IFC_NAND;
#endif

#if defined(_BCM96318_) || defined(CONFIG_BCM96318)
    type = FLASH_IFC_SPI;
#endif

    switch (type) {
    case FLASH_IFC_PARALLEL:
        ret = cfi_flash_init( &g_flash_info );
        break;

    case FLASH_IFC_SPI:
        ret = spi_flash_init( &g_flash_info );
        break;

    case FLASH_IFC_NAND:
        ret = nand_flash_init( &g_flash_info );
        break;

    case FLASH_IFC_UNKNOWN:
        /* Try to detect flash chips, give priority to parallel flash */
        /* Our reference design has both, and we usually use parallel. */
        ret = cfi_flash_init( &g_flash_info );
        if (ret != FLASH_API_OK) {
            ret = spi_flash_init( &g_flash_info );
        }
        break;
    }

    if (g_flash_info != NULL) {
        display_flash_info(ret, g_flash_info);
    }
    else {
        printk( "BCM Flash API. Flash device is not found.\n" );
    }


    return( ret );
} /* flash_init */

/***************************************************************************
 * Function Name: flash_sector_erase_int
 * Description  : Erase the specfied flash sector.
 * Returns      : FLASH_API_OK or FLASH_API_ERROR
 ***************************************************************************/
int flash_sector_erase_int(unsigned short sector)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_sector_erase_int) (sector)
        : FLASH_API_ERROR );
} /* flash_sector_erase_int */

/***************************************************************************
 * Function Name: flash_read_buf
 * Description  : Reads from flash memory.
 * Returns      : number of bytes read or FLASH_API_ERROR
 ***************************************************************************/
int flash_read_buf(unsigned short sector, int offset, unsigned char *buffer,
    int numbytes)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_read_buf) (sector, offset, buffer, numbytes)
        : FLASH_API_ERROR );
} /* flash_read_buf */

/***************************************************************************
 * Function Name: flash_write_buf
 * Description  : Writes to flash memory.
 * Returns      : number of bytes written or FLASH_API_ERROR
 ***************************************************************************/
int flash_write_buf(unsigned short sector, int offset, unsigned char *buffer,
    int numbytes)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_write_buf) (sector, offset, buffer, numbytes)
        : FLASH_API_ERROR );
} /* flash_write_buf */

/***************************************************************************
 * Function Name: flash_get_numsectors
 * Description  : Returns the number of sectors in the flash device.
 * Returns      : Number of sectors in the flash device.
 ***************************************************************************/
int flash_get_numsectors(void)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_get_numsectors) ()
        : FLASH_API_ERROR );
} /* flash_get_numsectors */

/***************************************************************************
 * Function Name: flash_get_sector_size
 * Description  : Returns the number of bytes in the specfied flash sector.
 * Returns      : Number of bytes in the specfied flash sector.
 ***************************************************************************/
int flash_get_sector_size(unsigned short sector)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_get_sector_size) (sector)
        : FLASH_API_ERROR );
} /* flash_get_sector_size */

/***************************************************************************
 * Function Name: flash_get_memptr
 * Description  : Returns the base MIPS memory address for the specfied flash
 *                sector.
 * Returns      : Base MIPS memory address for the specfied flash sector.
 ***************************************************************************/
unsigned char *flash_get_memptr(unsigned short sector)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_get_memptr) (sector)
        : NULL );
} /* flash_get_memptr */

/***************************************************************************
 * Function Name: flash_get_blk
 * Description  : Returns the flash sector for the specfied MIPS address.
 * Returns      : Flash sector for the specfied MIPS address.
 ***************************************************************************/
int flash_get_blk(int addr)
{
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_get_blk) (addr)
        : FLASH_API_ERROR );
} /* flash_get_blk */

/***************************************************************************
 * Function Name: flash_get_total_size
 * Description  : Returns the number of bytes in the flash device.
 * Returns      : Number of bytes in the flash device.
 ***************************************************************************/
int flash_get_total_size(void)
{
#if (INC_NAND_FLASH_DRIVER==1)
    return( (g_flash_info)
        ? (*g_flash_info->fn_flash_get_total_size) ()
        : FLASH_API_ERROR );
#else
    if(g_flash_info)
    {
        if((*g_flash_info->fn_flash_get_total_size)() >= 0x1000000)
        {
            return 0x1000000;
        }
        else
        {
            return (*g_flash_info->fn_flash_get_total_size)();
        }
    }
    else
    {
        return FLASH_API_ERROR;
    }


#endif
} /* flash_get_total_size */

/***************************************************************************
 * Function Name: flash_get_flash_type
 * Description  : Returns type of the flash memory.
 * Returns      : Type of the flash memory.
 ***************************************************************************/
int flash_get_flash_type(void)
{
    return( (g_flash_info)
        ? (g_flash_info->flash_type)
        : FLASH_API_ERROR );
} /* flash_get_flash_type */

void spiflash_chip_init( void )
{
    sema_init(&semflash, 1);
    flash_init_info();
}


#if 0//def SUPPORT_ATP_NANDFLASH
/******************************************************************************
  函数名称  : ATP_FLASH_ReadFlash
  功能描述  : 按相对基地址偏移读FLASH
  输入参数  : 1. ulFlashAddr     相对于基地址的偏移
              2. ulLen           要读的长度
              
  调用函数  : 无
  被调函数  : bhalIoctl
  输出参数  : 1. pDataBuffer     读去的内容
  返 回 值  : FLASH_API_ERROR 失败
              FLASH_API_OK    成功
  修改历史      :
   1.日    期   : 2007-8-17
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/

int ATP_FLASHCBB_ReadFlash_CountError(unsigned long ulFlashAddr,void *pszDataBuffer, unsigned long ulLen,unsigned long *pulCount)
{
    int iReturn = FLASH_API_ERROR;
    int iBlkStart = 0;
    int iSectSize = 0;
    int iReadSize = 0;
/* 从1  开始*/    
    int iErrCnt = 1;
    
    unsigned long ulSrcAddr = ulFlashAddr + BHAL_FLASH_BASE;
    
    down(&semflash);    
    iBlkStart = flash_get_blk((int)ulSrcAddr);

    
    if(iBlkStart < 0)
    {            
        up(&semflash);
        return FLASH_API_ERROR;
    }            
    do
    {
        iSectSize = flash_get_sector_size(iBlkStart);
        if(ulLen >= iSectSize)
        {
            iReadSize = iSectSize;
        }
        else
        {
            iReadSize = ulLen;
        }
        
        iReturn = flash_read_buf( iBlkStart, 0, (unsigned char *)pszDataBuffer, iReadSize );
        if(iReturn != iReadSize)
        {
            iBlkStart++;
            iErrCnt++;
        }
        else
        {
            iBlkStart++;
            DEBUG_PRINTK("\n=======iBlkStart:%d=======\n",iBlkStart);
            ulLen -= iReadSize; 
            pszDataBuffer += iReadSize;
        }
        
    }while (ulLen > 0);
    
    up(&semflash);
    
    
    if(0 == ulLen)
    {
        *pulCount = iErrCnt;
        return FLASH_API_OK;
    }
    else
    {
        return FLASH_API_ERROR;
    }
}
#endif


int spiflash_size_get(void)
{
    return flash_get_total_size();
}


int mtd_spiflash_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
    int iRet = FLASH_API_ERROR;
    int sect = flash_get_blk((int)from);
    unsigned char *start = flash_get_memptr(sect);
    
    down(&semflash);
    iRet = flash_read_buf(sect, (int)from - (int) start, buf, len);
    up(&semflash);

	if(retlen)
		(*retlen) = len;

    if(FLASH_API_OK == iRet)
    {
        return 0;
    }
    else
    {
        return -EINVAL;
    }
}

int mtd_spiflash_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
    int iRet = FLASH_API_ERROR;
    int sect = flash_get_blk((int)to);
    unsigned char *start = flash_get_memptr(sect);
    
    down(&semflash);
    iRet = flash_write_buf(sect, (int)to - (int) start, buf, len);
    up(&semflash);

	if(retlen)
		(*retlen) = len;

    if(len == iRet)
    {
        return 0;
    }
    else
    {
        return -EINVAL;
    }

}


int mtd_spiflash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
    int i = 0;
    int iBlkStart = 0;
    int iSectSize = 0;
    unsigned long ulAddress;
	unsigned long ulLen;

	ulAddress = instr->addr;
	ulLen = instr->len;
	
    if (ulLen == 0)
    {
        return -EINVAL;;
    }
    
    iBlkStart = flash_get_blk(ulAddress);
    if( iBlkStart < 0 )
        return (FLASH_API_ERROR);

    down(&semflash);
    
    do 
    {
        iSectSize = flash_get_sector_size(iBlkStart);

        flash_sector_erase_int(iBlkStart);     // erase blk before flash

        if (iSectSize > ulLen) 
        {
            if (ulLen & 1) 
                ulLen++;
            iSectSize = ulLen;
        }
        
        printk("@");
        iBlkStart++;
        ulLen -= iSectSize; 
        
    } while (ulLen > 0);
    
    up(&semflash);   

    if(0 == ulLen)
    {
		instr->state = MTD_ERASE_DONE;
		if (instr->callback)
			instr->callback(instr);

		return 0;
    }
    else
    {
	    instr->state = MTD_ERASE_FAILED;
		if (instr->callback)
			instr->callback(instr);
        return -EINVAL;
    }

}

static struct mtd_info *spiflash_probe(struct map_info *map)
{
	struct mtd_info *mtd;
    int flashsize = 0;
	int sectorsize = 0;
    
	mtd = kzalloc(sizeof(*mtd), GFP_KERNEL);
	if (!mtd) {
		printk(KERN_WARNING "Failed to allocate memory for MTD device\n");
		return NULL;
	}

	spiflash_chip_init();
	
    flashsize = spiflash_size_get();
	sectorsize = flash_get_sector_size(0);
	
	mtd->priv = map;
	mtd->type = MTD_NORFLASH;

	mtd->erase   = mtd_spiflash_erase;
	mtd->write   = mtd_spiflash_write;
	mtd->read    = mtd_spiflash_read;
	mtd->flags   = MTD_CAP_NORFLASH;
	mtd->name    = "norflash";
	mtd->writesize = FLASH_PAGE_256;
	mtd->owner   = THIS_MODULE;

	mtd->size      = flashsize;
	mtd->erasesize = sectorsize;

    printk("spiflash_probe: flash size:%d, sectorsize:%d\n", flashsize, sectorsize);
	
	return mtd;
}

static void spiflash_destroy(struct mtd_info *mtd)
{

}

static struct mtd_chip_driver spiflash_chipdrv = {
	.probe	 = spiflash_probe,
	.destroy = spiflash_destroy,
	.name	 = "spiflash_probe",
	.module	 = THIS_MODULE
};

static int __init spiflash_probe_init(void)
{
	register_mtd_chip_driver(&spiflash_chipdrv);
	return 0;
}

static void __exit spiflash_probe_exit(void)
{
	unregister_mtd_chip_driver(&spiflash_chipdrv);
}

module_init(spiflash_probe_init);
module_exit(spiflash_probe_exit);



