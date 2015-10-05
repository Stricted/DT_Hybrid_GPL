/************************************************************ 

*  版权所有:   2007-2020, 华为技术有限公司

*  文件名:     ats_hwplatform.h (cnxt)

*  作者:       

*  版本:       1.0

*  完成日期:   6/6/2007

*  描述:       Adapted form ats_hardware_api.c
               

*  主要函数列表:


*  修改历史记录列表:
*    1.日期: 
*      作者: 
*      修改: 完成初稿
**************************************************************/
#ifndef _BHALPLAT_H_
#define _BHALPLAT_H_
#include <bcm_map_part.h>
#include "atpconfig.h"


#ifdef SUPPORT_B560
#include "boardparms.h"
#endif

#define GPIO_5_NUM_RESET                5
#define GPIO_6_NUM_WLCTL                6

#define BHAL_KILO_SIZE                  1024   /* 1K */

/*#define FLASH_BASE                     (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00))  */
#define FLASH_BASE                     0xB8000000 

#define BHAL_FLASH_BASE                 FLASH_BASE
#define BHAL_BOOT_OFFSET                0
#define BHAL_BLOCK_SIZE                 0x20000     /*128k*/

#define kVerySlowBlinkCount 3          // ~500ms
#define kFlickerCount       8          // ~1000ms
#define kPauseCount         8          // ~1000ms

#define RESTORE_DEFOULT_TIME    6*8    /*6s*/
#define RESET_KEY_TIME          7      /*1s*/


/*****************************************************************************/
/*  Select the PLL value to get the desired CPU clock frequency.             */
/*****************************************************************************/
#define FPERIPH            50000000

/*****************************************************************************/
/* Board memory type offset                                                  */
/*****************************************************************************/
#define SDRAM_TYPE_ADDRESS_OFFSET       16
#define THREAD_NUM_ADDRESS_OFFSET       20
#define NVRAM_DATA_OFFSET               0x0580
#define NVRAM_DATA_ID                   0x0f1e2d3c
#define BOARD_SDRAM_TYPE                *(unsigned long *)(FLASH_BASE + SDRAM_TYPE_ADDRESS_OFFSET)
#define BOARD_SDRAM_TYPE_ADDRESS        (0xA0000000 + PHYS_FLASH_BASE + SDRAM_TYPE_ADDRESS_OFFSET)
#define BOARD_THREAD_NUM                *(unsigned long *)(FLASH_BASE + THREAD_NUM_ADDRESS_OFFSET)
#define BOARD_THREAD_NUM_ADDRESS        (0xA0000000 + PHYS_FLASH_BASE + THREAD_NUM_ADDRESS_OFFSET)
#define BOARD_IMAGE_DOWNLOAD_ADDRESS    \
    ((cfe_sdramsize > 0x00800000) ? 0x80800000 : 0x80000000) 
#define BOARD_IMAGE_DOWNLOAD_SIZE       \
    ((cfe_sdramsize > 0x00800000) ? 0x00800000 : 0x00400000) 

#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6
#define NVRAM_MAC_COUNT_MAX             32
#define NVRAM_MAC_COUNT_DEFAULT         0 
#define NVRAM_SERIAL_NUMBER_LEN         128


/*
** From where configuration information comes.
*/
typedef enum
{
    ConfigInfoUnknown,    /* Storage location unknown */
    ConfigInfoEeprom,   /* Stored in serial EEPROM */
    ConfigInfoFlash     /* Stored in FLASH memory */
} ConfigInfoLocation;


/*****************************************************************************/
/*          Function Prototypes                                              */
/*****************************************************************************/
void bhalplatInitboard(void);
int ATP_BhalplatInit(void);
void bhalplatSoftReboot(void);
void bhalplatHardReboot(void);

#endif /* _BHALPLAT_H_ */
