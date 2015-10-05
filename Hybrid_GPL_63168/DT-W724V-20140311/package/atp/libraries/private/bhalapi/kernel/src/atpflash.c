/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : atpflash.c
  作    者  : l63336
  版    本  : 1.0
  创建日期  : 2007-8-17
  描    述  : FLASH适配接口层
				ATP_FLASH_InitFlash
				ATP_FLASH_GetFlashAreaSize
				ATP_FLASH_GetFlashOffset
				ATP_FLASH_GetSize
				ATP_FLASH_ReadFlashArea
				ATP_FLASH_WriteFlashArea
				ATP_FLASH_EraseFlash
				ATP_FLASH_ReadFlash
				ATP_FLASH_WriteFlash
  历史记录      :
     日    期   : 2007-8-17
     作    者   : l63336
     修改内容   : 完成初稿

*********************************************************************************/

/* Includes. */
#include <linux/fs.h>
#include <linux/capability.h>
//#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/module.h>
//#include <asm/uaccess.h>

#define  ATPTAG_EXE
#define  BCMTAG_EXE_USE


#include "bhal.h"
#include "flashcbbapi.h"
#include "atpflash.h"
#include "atptag.h"
#include "atpconfig.h"
//#define DEBUG_FLASH

#include <linux/mtd/map.h>
#include <linux/spinlock.h>
#if defined(SUPPORT_HG622B) || defined(SUPPORT_HG658B)
#include "flash_api_bcm.h"
#endif

static spinlock_t write_flash_lock = SPIN_LOCK_UNLOCKED;
static spinlock_t read_flash_lock  = SPIN_LOCK_UNLOCKED;

int g_flash_total_size = 0;
int g_flash_sector_size = 0;


#ifdef FLASHCBB_USE_GPIO

unsigned char fp_FlashCbb_Gpio_Read_Byte(unsigned char *flash_addr);
unsigned short fp_FlashCbb_Gpio_Read_Word(unsigned char *flash_addr);
unsigned long fp_FlashCbb_Gpio_Read_Long(unsigned char *flash_addr);
void fp_FlashCbb_Gpio_Write_Byte(unsigned char *flash_addr,unsigned char ucData);
void fp_FlashCbb_Gpio_Write_Word(unsigned char *flash_addr,unsigned short usData);
void fp_FlashCbb_Gpio_Write_Long(unsigned char *flash_addr,unsigned long ulData);

extern void flash_gpio_ctl(unsigned long int GPIONum,int mode);
extern int ATP_FLASHCBB_EraseBlock(unsigned long ulAddress, unsigned long ulLen);
ATP_FLASHCBB_CALLBACK_ST g_stCnxtHook = 
{
/*Read*/
	fp_FlashCbb_Gpio_Read_Byte,
	fp_FlashCbb_Gpio_Read_Word,
	fp_FlashCbb_Gpio_Read_Long,
/*Write*/
    fp_FlashCbb_Gpio_Write_Byte,
	fp_FlashCbb_Gpio_Write_Word,
	fp_FlashCbb_Gpio_Write_Long
};

unsigned char fp_FlashCbb_Gpio_Read_Byte(unsigned char *flash_addr)
{
	unsigned char *ret_val;
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);

	if (((unsigned long)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR)
	{
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
	ret_val = flash_addr;
	spin_unlock_irq(gpio_flash_lock);

	return *ret_val;	
}

unsigned short fp_FlashCbb_Gpio_Read_Word(unsigned char *flash_addr)
{
	unsigned short *ret_val;
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);

	if (((unsigned long)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR)
	{
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
	ret_val = (unsigned short *)flash_addr;
	spin_unlock_irq(gpio_flash_lock);
	return *ret_val;	
}

unsigned long fp_FlashCbb_Gpio_Read_Long(unsigned char *flash_addr)
{
	unsigned long *ret_val;
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);

	if (((unsigned long)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR)
	{
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
	ret_val = (unsigned long *)flash_addr;
	spin_unlock_irq(gpio_flash_lock);
	return *ret_val;	
}

void fp_FlashCbb_Gpio_Write_Byte(unsigned char *flash_addr,unsigned char ucData)
{
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);	
	if ( ((unsigned int)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR )	
	{		
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}	
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
    *flash_addr = ucData;
	
	spin_unlock_irq(gpio_flash_lock);    	
	return;
}


void fp_FlashCbb_Gpio_Write_Word(unsigned char *flash_addr,unsigned short usData)
{
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);	
	if ( ((unsigned int)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR )	
	{		
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}	
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
    *flash_addr = usData;
	
	spin_unlock_irq(gpio_flash_lock);    	
	return;
}

void fp_FlashCbb_Gpio_Write_Long(unsigned char *flash_addr,unsigned long ulData)
{
	spinlock_t gpio_flash_lock = SPIN_LOCK_UNLOCKED;
	spin_lock_irq(gpio_flash_lock);	
	if ( ((unsigned int)flash_addr) & FLASHCBB_USE_GPIO_BASEADDR )	
	{		
		flash_gpio_ctl(GPIO_FLASH_16 , SET);	
	}	
	else
	{
		flash_gpio_ctl(GPIO_FLASH_16 , CLEAR);	
	}
//	DEBUG_PRINT("\r\n Gpio write Long.\r\n");
    *flash_addr = ulData;
	
	spin_unlock_irq(gpio_flash_lock);    	
	return;
}
#endif


static int g_lFlashInitialized = 0;


/***************************************************************************
// Function Name: getCrc32
// Description  : caculate the CRC 32 of the given data.
// Parameters   : pdata - array of data.
//                size - number of input data bytes.
//                crc - either CRC32_INIT_VALUE or previous return value.
// Returns      : crc.
****************************************************************************/
unsigned long getCrc32(const char *pdata, unsigned long size, unsigned long crc) 
{
    while (size-- > 0)
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];

    return crc;
}


/******************************************************************************
  函数名称  : ATP_FLASH_GetSize
  功能描述  : 获得FLASH大小
  输入参数  : 无
  调用函数  : 无
  被调函数  : bhalIoctl
  输出参数  : 无
  返 回 值  : FLASH大小，以K为单位

  修改历史      :
   1.日    期   : 2007-8-17
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/
int ATP_FLASH_GetSize(void)
{
   return g_flash_total_size;
}

int ATP_FLASH_GetBlockSize(unsigned long ulSrcAddr)
{
   return g_flash_sector_size;
}



#ifdef SUPPORT_ATP_NANDFLASH
int ATP_FLASH_WriteFlash_Jffs(void *buf, unsigned long toaddr , int len)
{
    //ATP_FLASHCBB_Jffs_Write(buf, toaddr , len);
    return 0;
}

int ATP_FLASH_EraseFlash_Jffs(unsigned long addr , int len)
{
    //ATP_FLASHCBB_Jffs_Erase(addr, len);
    return 0;
}

EXPORT_SYMBOL(getCrc32);
#endif

EXPORT_SYMBOL(ATP_FLASH_GetBlockSize);


