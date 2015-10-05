/* ************************************************************************** 
 * 文件功能 : 定义Intel flash 驱动所用到的一些信息,Intel flash驱动的头文件
 * 作者     : 俞佑清y42304 @ HAL&OS
 *
 * 修改历史 :
 *    1. 创建文件时间: 2005年12月30日 
 *    
 *
 * 说明: 1. 适用范围: 适合Intel/AMD flash(8位/16位/32位):
 *       2. 实现方法: 利用CFI接口来动态获取flash的大小，块数和块大小等信息的,
 *            所以假如要移植该flash驱动到其它不同容量或块结构及数据位宽的Intel
 *          芯片上，你基本只需做如下三件事:
 *          A: 重新定义宏FLASH_ADDRESS_BASE来指定新的flash芯片的基地址。
 *          B: 重新定义宏CFIDEB_BUSWIDTH来指定数据线位宽 
 *          C: 确定目标的字节序是否和交叉编译环境的CPU的字节序是否相同，不同则请
 *             定义宏CFI_BIG_ENDIALN,否则定义宏CFI_LITTLE_ENDIAN。
 *             理由: 因为在读取CFI信息时，与字节序有关。
 *          D: 调用flash_probe(ulFlashBaseAddr, 0, 1)来动态探测flash信息并打印出
 *             flash的信息，然后和你实际的flash信息比较，看结果是否相符。
 *
 * ***************************************************************************/


#ifndef __FLASHCBB_API_H
#define __FLASHCBB_API_H

#include "flash_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLASHCBB_DEBUG

/**********************************************************************
 *   如下是一些不支持CFI的一些FLASH的CHIP ID的定义
 *   CHIP ID   由Manufacture ID(MID) 低8位和Device ID(DID)低8位而拼成。
 *             MID 低8位 放在CHIP ID的高字节上
 *             DID低8位放在CHIP ID的低字节上。
 *             即CHIP ID = ((MID & 0xFF) << 8) | (DID & 0xFF)
 **********************************************************************/
 
/* AMD:  CHIP ID */
#define AM29F800BB	            0x2258
#define AM29F800BT	            0x22D6
#define AM29LV800BB	            0x225B
#define AM29LV800BT	            0x22DA
#define AM29LV160DT	            0x22C4
#define AM29LV160DB	            0x2249
#define AM29BDS323D             0x22D1
#define AM29BDS643D	            0x227E
#define AM29BDS640D                 0x0103

/*AMD类型的FLASH ID号,目前用来提供GEOMERY判断*/
#define AM29DL800T              0x224A
#define AM29DL800B              0x22CB
#define AM29LV800T              0x22DA
#define AM29LV400B              0x22BA
#define AM29LV200BT             0x223B
#define AM29LV160B              0x2249
#define AM29LV160T              0x22C4
#define AM29LV320T              0x22F6
#define MX29LV320AT             0x22A7
#define AM29LV320B              0x22F9
#define MX29LV320AB             0x22A8
#define MX29LV320BT             0xC2A8
#define MX29LV640BT             0x22C9
#define AM29LV320M              0x227E
#define AM29LV320MB             0x017E
#define AM29LV320MB_1           0x01F6
#define AM29LV320MT             0x2201
#define SST39VF200A             0x2789
#define SST39VF400A             0x2780
#define SST39VF800A             0x2781
#define SST39VF1601             0x234B
#define SST39VF3201             0x235B
#define AM29LV800B              0x225B
#define AM8MENDFLAG             0x22ED
#define MX29LV320DTTI           0x1000
/*AMD类型的FLASH ID号,目前用来提供GEOMERY判断*/

/* Atmel: CHIP ID */
#define AT49xV16x	            0x00C0
#define AT49xV16xT	            0x00C2

/* Fujitsu: CHIP ID */
#define MBM29LV160TE	        0x22C4
#define MBM29LV160BE	        0x2249
#define MBM29LV800BB	        0x225B

/* ST:    CHIP ID */
#define M29W800T	            0x00D7
#define M29W160DT	            0x22C4
#define M29W160DB	            0x2249

/* SST:   CHIP ID */
#define SST39LF800	            0x2781
#define SST39LF160	            0x2782

/* Toshiba: CHIP ID */
#define TC58FVT160	            0x00C2
#define TC58FVB160	            0x0043


/*INTEL类型的FLASH ID号,目前用来提供GEOMERY判断*/
#define ID_I28F160C3T           0x88C2
#define ID_I28F160C3B           0x88C3
#define ID_I28F320C3T           0x88C4
#define ID_I28F320C3B           0x88C5
#define ID_I28F640J3	        0x8916
/*INTEL类型的FLASH ID号,目前用来提供GEOMERY判断*/

/*********************************************************************
 * 用途: Flash Bottom/Top boot flag
 ********************************************************************/
#define CFIDEV_BOTTOMBOOT           2
#define CFIDEV_TOPBOOT              3
#define CFIDEV_UNIFORM_BP           4
#define CFIDEV_UNIFORM_TP           5


/**********************************************************************
 *   结束FLASH的设备ID的定义
 **********************************************************************/


/*********************************************************************
 * 用途: 定义FLASH芯片ID。
 *       1. 支持CFI，则定义正确的CHIP ID
 *       2. 不支持CFI，则将CHIP ID定义成0xFFFF
  *   CHIP ID  由Manufacture ID(MID) 低8位和Device ID(DID)低8位而拼成。
 *             MID 低8位 放在CHIP ID的高字节上
 *             DID低8位放在CHIP ID的低字节上。
 *             即CHIP ID = ((MID & 0xFF) << 8) | (DID & 0xFF)
 *********************************************************************/
#define FLASH_CHIP_ID           0xFFFF


/*************************************************************************************************
* 用途：定义操作系统类型: linux, pSos,VxWorks.     
* 说明：擦除和写FLASH期间，需要关中断，而开关中断函数依赖于OS，所以需要
*       依据用户在配置文件配置的操作系统类型，定义如下3个宏之一。
*************************************************************************************************/
// #define PSOS_OS
 //#define VXWORKS_OS
#define LINUX_OS
/************************************************************************
 * 用途: 定义操作FLASH的超时时间
 *       1. 支持CFI，则不需修改如下四个宏的定义值
 *       2. 不支持CFI，则需参考FLASH芯片资料，以设置正确的相应延时值  
 *
 ************************************************************************/
#define TIMEOUT_BLOCK_ERASE     11   /*2^10 ms check if block erase command has completed */
#define TIMEOUT_CHIP_ERASE      11   /*2^n ms (if supported; 00h = not supported)*/
#define TIMEOUT_SINGLE_WRITE    10  /*2^n us check if byte/word command has completed */
#define TIMEOUT_BUFFER_WRITE    10    /*2^10 us check if buffer write command has completed */
#define MAXSECTORS              1024 /*最大的sector号*/


#define FLASH_INFO_SHOW

/********************************************************************************************/
/***********/
/**********************************************************************************
             用户需要修改的宏定义部分结束
 **********************************************************************************/
/********************************************************************************
 说明: 以下所有的声明和定义在Intel flash 芯片上不需要更改 
 ********************************************************************************/

#define FLASH_MAX_CHIPSET               2   /* 最多的FLASH芯片数目 */

#define CFI_PRIMARY_ID_NULL             0   /* none is specified */
#define CFI_PRIMARY_ID_INTEL_EXT        1   /* Intel/Sharp Extended command set */
#define CFI_PRIMARY_ID_AMD_STD          2   /* AMD/Fujitsu Standard command set */ 
#define CFI_PRIMARY_ID_INTEL_STD        3   /* Intel Standard command set       */ 
#define CFI_PRIMARY_ID_AMD_EXT          4   /* AMD/Fujitsu Extended command set */
#define CFI_PRIMARY_ID_MITSUBISHI_STD   256 /* Mitsubishi Standard command set  */
#define CFI_PRIMARY_ID_MITSUBISHI_EXT   257 /* Mitsubishi Extended command set  */ 
#define CFI_PRIMARY_ID_SST              258 /* Page Write command set           */
#define CFI_PRIMARY_ID_RESERVED         65535

// Device interface code assignment
// CFI Publication 100
#define CFI_INTERFACE_X8                0   /* x8-only asynchronous interface    */
#define CFI_INTERFACE_X16               1   /* x16-only asynchronous inteface    */
#define CFI_INTERFACE_X8_16             2   /* x8 and x16 via BYTE# with asynchronous interface */
#define CFI_INTERFACE_X32               3   /* x32-only asynchronous interface                  */ 
#define CFI_INTERFACE_X16_32            5   /* x16 and x32 via WORD# with asynchronous interface*/ 
#define CFI_INTERFACE_RESERVED          65535
#define CFI_MAX_ERASE_REGIONS           8   /* flash中最大可擦除的区域数量 */

/*
 *  Start: Flash 编程命令字
 */
#define QUERY                           0x98  /* 进入Flash查询状态 */
#define READ_ID                         0x90  /* 读Flash ID的命令字 */

#define WSM_ERROR                       0x3A     

#ifdef INTEL_FLASH

#define READ_ARRAY                      0xFF  /* 启动Flash 到读的状态 */
#define SR_IS_READY                     0x80  /* Flash已准备就绪 */
#define CLEAR_STATUS                    0x50  /* 清除Flash状态的命令字 */
#define READ_STATUS                     0x70  /* 读状态寄存器 */
#define CLEAR_LOCK_BIT                  0x60  /* 单个BLOCK解锁命令字1 */
#define CONFIRM_CLEAR_LOCK_BIT          0xD0  /* 单个BLOCK解锁命令字2 */
#define WRITE_TO_BUFFER                 0xE8  /* 用写缓冲方式写FLASH的命令字*/
#define INTEL_BYTE_PROGRAM              0x10  /* 按字节编程命令字*/
#define INTEL_WORD_PROGRAM              0x40  /* 按字编程命令字*/
#define EACH_BYTE_NUMS_WRITE            32    /* 利用写Buffer方式时每次最多能写的字节数目*/
#define CONFIRM_WRITE                   0xD0  /* 确认写操作是否完成的命令字 */
#define SECTOR_ERASE                    0x20  /* 擦除Flash块的命令字 */
#define CONFIRM_ERASE                   0xD0  /* 确认擦除操作是否完成的命令字 */


#else    /* 遵循AMD 编程规格的Flash */

/*8位flash 使用*/
#define READ_ARRAY                      0xF0  /* 启动Flash 到读的状态 */
#define AMD_CMD_UNLOCK_1                0xAA  /* AMD解锁命令字1*/
#define AMD_CMD_UNLOCK_2                0x55  /* AMD解锁命令字2*/

/* 与UNLOCK BYPASS有关的命令字 */
#define AMD_UNLOCK_BYPASS               0x20
#define AMD_UNLOCK_BYPASS_RESET1        0x90
#define AMD_UNLOCK_BYPASS_RESET2        0x00


/* 与擦除Flash有关的命令字 */
#define AMD_SETUP_ERASE                 0x80
#define AMD_SECTOR_ERASE                0x30
#define AMD_CHIP_ERASE                  0x10

/* 与写Flash有关的命令字 */
#define AMD_PROGRAM_WRITE               0xA0

#define AMD_UNLOCK_ADDR1                0x555
#define AMD_UNLOCK_ADDR2                0x2AA

#endif/*end of INTEL_FLASH*/

/* End: 编程命令字的定义 */

#define INVALID_CHIP_ID                 0xFFFF


#define FLASH_WRITE_MAX_TRY_COUNT        0x7FFFF    /* 最多的写查询次数 */
#define FLASH_CLEAR_LOCK_TRY_COUNT        250000    
#define FLASH_ERASE_BLOCK_TRY_COUNT        2500000        

/* Status Register Bits */
#define BIT_0                            0x1
#define BIT_1                            0x2
#define BIT_2                            0x4
#define BIT_3                            0x8
#define BIT_4                            0x10
#define BIT_5                            0x20
#define BIT_6                            0x40
#define BIT_7                            0x80

#ifndef NULL
#define NULL                            0
#endif

#define FLASH_VERBOSE                   0
#define FLASH_ERR_CNT                   512        /* Error limit before reporting */

#define FLASH_ADDR_BASE_INVALID         0xFFFFFFFF /* 非法的FLASH基地址 */


/* 内部使用的结构体 */
// Query ID structure
typedef struct hal_S_FlashCfiQueryId 
{
    unsigned char ucQuery[3];              // 0x10 - 0x12: query-unique ACSII string "QRY"
    unsigned char ucReserved1;             // 保留，为了4字节对齐
    unsigned short usPrimaryId;            // 0x13 - 0x14: primary vendor command set and control interface ID code
    unsigned short usPrimaryTableAddr;     // 0x15 - 0x16: address for primary algorithm extended query table
    unsigned short usAlternateId;          // 0x17 - 0x18: alternate vendor command set and control interface ID code
    unsigned short usAlternateTableAddr;   // 0x19 - 0x1a: address for alternate algorithm extended query table
    
    unsigned char ucVccMin;                // 0x1b: Vcc logic supply minimum voltage
    unsigned char ucVccMax;                // 0x1c: Vcc logic supply maximum voltage
    unsigned char ucVppMin;                // 0x1d: Vpp logic supply minimum voltage
    unsigned char ucVppMax;                // 0x1e: Vpp logic supply maximum voltage
    
    unsigned char ucTimeoutSingleWrite;    // 0x1f: typical timeout per single byte/word write
    unsigned char ucTimeoutBufferWrite;    // 0x20: typical timeout for minimum-size buffer write
    unsigned char ucTimeoutBlockErase;     // 0x21: typical timeout for individual block erase
    unsigned char ucTimeoutChipErase;      // 0x22: typical timeout for full chip erase
    
    unsigned char ucMaxTimeoutSingleWrite; // 0x23: maximum timeout per single byte/word write
    unsigned char ucMaxTimeoutBufferWrite; // 0x24 : maximum timeout for minimum-size buffer write
    unsigned char ucMaxTimeoutBlockErase;  // 0x25 : maximum timeout for individual block erase
    unsigned char ucMaxTimeoutChipErase;   // 0x26 : maximum timeout for full chip erase
    
    unsigned char ucDeviceSize;            // 0x27 : device size = 2^n in number of bytes    
    unsigned char ucReserved2;             // 保留，为了4字节对齐    
    unsigned short usInterfaceDesc;        // 0x28 - 0x29 : flash device interface description
    
    unsigned short usMaxMultibyteWrite;    // 0x2a - 0x2b : maximum number of bytes in multi-byte write = 2^n
    unsigned char ucEraseRegionsNum;       // 0x2c : number of erase block regions within device
    unsigned char ucReserved3;             // 保留，为了4字节对齐    
    
    /******************************************************************
    * bits 31-16 = Z:  块大小 = Z×256字节，假如 Z=0,表示块大小为128K
    * bits 15-0  = Y:  块数目 = Y+1
    ******************************************************************/    
    unsigned int uiEraseRegionInfo[CFI_MAX_ERASE_REGIONS];                                                                                    
}FLASHCBB_QUERYID_ST;


/* 每个区域的起始偏移地址，块大小和块数*/
typedef struct hal_S_FlashEraseRegions 
{
    unsigned long ulStartOff;     /* 该区域在Flash中的偏移地址 */        
    unsigned long ulBlockSize;    /* 所在区域的块大小*/
    unsigned long ulBlocksNum;    /* 该区域的块数 */
}FLASHCBB_ERASEREGIONS_ST;

typedef struct hal_S_FlashInfoManual 
{
    unsigned short usChipID;   /* Flash 芯片ID= ((MID & 0xFF) << 8) | (DID & 0xFF)*/
    unsigned long ulSize;     /* Flash 芯片的大小 */
    int iEraseRegionsNum;     /* Flash 芯片的可擦除区域数目 */

    /* Flash 芯片可擦除区域块信息结构 */
    FLASHCBB_ERASEREGIONS_ST stEraseRegions[CFI_MAX_ERASE_REGIONS];
}FLASHCBB_INFO_ST;

/* CFI information block used by this driver  */
typedef struct hal_S_FlashCfiInfo
{
    int iExist;                /* 表示是否存在Flash; 1: 表示存在; 0: 表示不存在*/
    int iChipState;            /* Flash的状态 */
    unsigned long ulBaseAddr;  /* Flash的状态 */
    unsigned long ulSize;      /* 单片Flash芯片的大小 */
    unsigned short usSectorNum; /*Sector数量*/
    
    unsigned short usSect;                  /* # of sectors -- 19 in LV, 22 in DL 目前未处理*/
    
    unsigned int ulBootFlag;
    
    struct {
    unsigned long ulSize;           /* # of bytes in this sector */
    unsigned long ulBase;           /* offset from beginning of device */
    unsigned int ulBank;            /* 1 or 2 for DL; 1 for LV */
     } stSector[MAXSECTORS];  /* per-sector info */

    FLASHCBB_QUERYID_ST stIdent; /* CFI结构信息 */
    FLASHCBB_ERASEREGIONS_ST stEraseRegions[CFI_MAX_ERASE_REGIONS];
}FLASHCBB_INFOTBL_ST, *FLASHCBB_INFOTBL_ST_PTR;


/* 目标机CPU的字节序 */
typedef enum 
{ 
    E_BIG_END = 0,
    E_LITTLE_END,
    BUT
}FLASHCBB_CPU_BYTE_ORDER_EN;

typedef enum
{
    GPIOREAD = 0,
    GPIOSET,
    GPIOCLEAR,
    GPIOEND
}FLASHCBB_GPIO_OPTYPE;

/* 表示内存中的内容已更新，防取自该内存单元中某个寄存器的内容不一致 */
#define mb()  __asm__ __volatile__ ("" : : : "memory")


#ifdef FLASHCBB_USE_GPIO

/*****************************************************************************/
/*提供给外部使用的结构体*/
/******************************************************************************
 *功能描述  :  FLASHCBB提供的钩子函数.
 *GPIO接口,不同的CPU需要使用不同的接口
 ******************************************************************************/

//unsigned int (*fp_FlashCbb_Gpio_Op)(FLASHCBB_GPIO_OPTYPE enGpioOp);

#ifndef FLASCHCBB_OK
#define FLASCHCBB_OK  0
#endif

#ifndef FLASCHCBB_ERR
#define FLASCHCBB_ERR -1
#endif

#define FLASHCBB_GPIO_SET        2
#define FLASHCBB_GPIO_NOTSET     3

/******************************************************************************
 *功能描述  :  FLASHCBB提供的钩子函数.
 *GPIO接口,不同的CPU需要使用不同的接口
 ******************************************************************************
 *函数名    :  fp_FlashCbb_Gpio_Read_*
 *描述      :  读出pucflash_addr地址中的值
 *参数      :  p**flash_addr 地址值
 
 *返 回 值  :  读出的值                     : 成功
               HAL_E_RETVAL_EFLASH_READ     : 失败
 ******************************************************************************
 *函数名    :  fp_FlashCbb_Gpio_Write_*
 *描述      :  把Data写入pucflash_addr地址
 *参数      :  pucflash_addr 地址值
               **Data需写入的值
 
 *返 回 值  :  无
               如果出错将打印错误信息
 ******************************************************************************
 * 说明     :  使用时可根据情况选择初始化特定的钩子,
               没有定义的接口要初始化为NULL
 *****************************************************************************/
typedef struct{
/*Read*/
    unsigned char  (*fp_FlashCbb_Gpio_Read_Byte) (unsigned long pucflash_addr);
    unsigned short  (*fp_FlashCbb_Gpio_Read_Word) (unsigned long pucflash_addr);
    unsigned long  (*fp_FlashCbb_Gpio_Read_Long) (unsigned long pucflash_addr);
/*Write*/    
    void  (*fp_FlashCbb_Gpio_Write_Byte) (unsigned long pucflash_addr,unsigned char ucData);    
    void  (*fp_FlashCbb_Gpio_Write_Word) (unsigned long pucflash_addr,unsigned short usData);
    void  (*fp_FlashCbb_Gpio_Write_Long) (unsigned long pucflash_addr,unsigned long ulData);    
   
}ATP_FLASHCBB_CALLBACK_ST;

 /******************************************************************************
 *功能描述  :  初始化钩子函数
 *输入参数  :  ATP_FLASHCBB_CALLBACK_ST 钩子的结构体
               
 *返 回 值  :  
 *             

 * 说明     :  
 *****************************************************************************/

 void ATP_FLASHCBB_InitHook(ATP_FLASHCBB_CALLBACK_ST *stHook);


#endif

int ATP_FLASHCBB_Init(void);

int ATP_FLASHCBB_GetSectorSize(unsigned long ulSrcAddr);


/********提供给外部的接口**********************/
 /******************************************************************************
 *功能描述  :  获得FLASH的大小
 *输入参数  :  
               
 *返 回 值  :  
 *             

 * 说明     :  目前未使用
 *****************************************************************************/

unsigned long ATP_FLASHCBB_GetSize(void);

/******************************************************************************
 *功能描述  :  获取FLASH的信息
 *输入参数  :  stFlashInfo: 存储 Flash 信息结构体
 *              
 *返 回 值  :  HAL_E_RETVAL_OK:  成功
 *             错误码:           失败 
 * 
 * 说明     :  
 *****************************************************************************/
int ATP_FLASHCBB_GetFlashInfo(FLASHCBB_INFO_ST stFlashInfo[FLASH_MAX_CHIPSET]);

/******************************************************************************
 *功能描述  :  擦除所有的FLASH块信息
 *输入参数  :  无
 *              
 *返 回 值  :  
 * 说明     :  暂时未实现
 *****************************************************************************/
int ATP_FLASHCBB_EraseAllChip(void);

/******************************************************************************
 *功能描述  :  擦除指定区域的块空间
 *输入参数  :  ulBaseAddr: Flash 绝对地址
 *             ulOffsetAddr: 以ulBaseAddr为基地址的偏移量
 *             ulLen: 擦除的长度(以字节为单位)               
 *              
 *返 回 值  :  0:  成功
 *             1:  失败 
 * 
 * 说明     :  
 *****************************************************************************/
int ATP_FLASHCBB_Eraseblock(unsigned long ulAddress, unsigned long ulLen);

/******************************************************************************
 *功能描述  :  擦除指定区域的块空间
 *输入参数  :  ulBaseAddr: Flash 绝对地址
 *             ulOffsetAddr: 以ulBaseAddr为基地址的偏移量
 *             ulLen: 擦除的长度(以字节为单位)               
 *              
 *返 回 值  :  0:  成功
 *             1:  失败 
 * 
 * 说明     :  为CNXT而提供的接口
 *****************************************************************************/
int ATP_FLASHCBB_Eraseblock2(unsigned long ulBaseAddr, unsigned long ulOffsetAddr , unsigned long ulLen);

#ifdef SUPPORT_ATP_NANDFLASH
/******************************************************************************
 *功能描述  :  读取指定区域的内容
 *输入参数  :  ulSrcAddr: Flash 绝对地址
 *             ulDstAddr: 内存地址
 *             ulLen: 读取的长度(以字节为单位)               
 *              
 *返 回 值  :  HAL_E_RETVAL_OK:  成功
 *             错误码:           失败 
 * 
 * 说明     :  
 *****************************************************************************/
int ATP_FLASHCBB_ReadFlash_CountError(unsigned long ulFlashAddr,void *pszDataBuffer, unsigned long ulLen,unsigned long *ulCount);
#endif

/******************************************************************************
 *功能描述  :  读取指定区域的内容
 *输入参数  :  ulSrcAddr: Flash 绝对地址
 *             ulDstAddr: 内存地址
 *             ulLen: 读取的长度(以字节为单位)               
 *              
 *返 回 值  :  HAL_E_RETVAL_OK:  成功
 *             错误码:           失败 
 * 
 * 说明     :  
 *****************************************************************************/
int ATP_FLASHCBB_Read(unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLen);

/******************************************************************************
 *功能描述  :  往指定Flash区域写
 *输入参数  :  ulFlashAddr: 写入的FLASH绝对地址
 *             pDataBuffer: pDataBuffer数据来源
 *             ulLen: 写的长度(以字节为单位)               
 *              
 *返 回 值  :  HAL_E_RETVAL_OK:  成功
 *             错误码:  失败 
 * 
 * 说明     :  
 *****************************************************************************/
 
int ATP_FLASHCBB_Write(unsigned long ulDstAddr, const void *pDataBuffer, unsigned long ulLen);
#ifdef SUPPORT_ATP_NANDFLASH
int ATP_FLASHCBB_Jffs_Write(void *buf, unsigned long toaddr , int len);
int ATP_FLASHCBB_Jffs_Erase(unsigned long addr , int len);
#endif
/******************************************************************************
 *功能描述  :  往指定Flash区域写
 *输入参数  :  ulBaseAddr: Flash 绝对地址
 *             ulOffsetAddr: 以ulBaseAddr为基地址的偏移量
 *             ulLen: 写的长度(以字节为单位)               
 *              
 *返 回 值  :  HAL_E_RETVAL_OK:  成功
 *             错误码:  失败 
 * 
 * 说明     :  为CNXT匹配而提供的接口
 *****************************************************************************/

 int ATP_FLASHCBB_Write2(unsigned long ulBaseAddr, unsigned long uloffsetAddr, void *dataptr, unsigned long ulLen);
/*l63336 add for adapt CNXT interface*/

 /******************************************************************************
 *功能描述  :  由Sector号获得Flash块的基地址
 *输入参数  :  Sector 块号              
 *返 回 值  :  
 *             NULL:              失败
 *             Sector块的基地址 : 成功

 * 说明     :  
 *****************************************************************************/
 unsigned long ATP_FLASHCBB_GetBasefromSector(unsigned short usSector,unsigned short usChipID);

 /******************************************************************************
 *功能描述  :  由Sector号获得Flash块的大小
 *输入参数  :  Sector 块号              
 *返 回 值  :  
 *             NULL:              失败
 *             Sector块的基地址 : 成功

 * 说明     :  
 *****************************************************************************/
 unsigned long ATP_FLASHCBB_GetSizefromSector(unsigned short usSector,unsigned short usChipID);

 /******************************************************************************
 *功能描述  :  由地址获得Flash的Sector块号
 *输入参数  :  ulAddr 地址
               
 *返 回 值  :  
 *             HAL_E_RETVAL_EFLASH_PARAMETER: 失败
 *             块号:                          成功

 * 说明     :  
 *****************************************************************************/
 unsigned short ATP_FLASHCBB_GetSectorfromAddr(unsigned long ulAddr);


 /******************************************************************************
 *功能描述  :  获得Sector块的数量
 *输入参数  :  无
               
 *返 回 值  :  Sector 数量              : 正确
 *             HAL_E_RETVAL_EDEFAULT    : 错误          
 *             

 * 说明     :  如果有两块FLASH返回两块FLASH的总的Sector数量
 *****************************************************************************/
 unsigned int ATP_FLASHCBB_GetSectorNum(void);
 
 /******************************************************************************
 *功能描述  :  判断最后64K是否是8*8K
 *输入参数  :  
               
 *返 回 值  :  
 *             

 * 说明     :  目前未使用
 *****************************************************************************/
#if 0
 static void ATP_FLASHCBB_JudgeFlashIsTopDevice8K(void);
#endif

#ifdef FLASHCBB_DEBUG
/*l63336 add for adapt CNXT interface*/

/*l63336 CNXT FLASH相关测试函数*/
/*测试接口试用的结构体*/
typedef struct{
    unsigned short usIsChipExt;/*芯片是否存在 1:存在; 0:不存在*/
    unsigned short usSectorNum;/*芯片的Sector数量*/
    unsigned short usEraseNum;/*Erase的数量*/
    unsigned short usEraseBlockNum;/*第一个Erase区域的块数量*/
    unsigned long usEraseSize;/*第一个Erase区域的块大小*/
    unsigned long usFirstSectorSize;/*第一个Sector块的大小*/
    unsigned long usLastSectorSize;/*最后一个Sector快的大小*/
}ATP_FLASHCBB_InitAssert_ST;


int ATP_FLASHCBB_MemCmp(void *pvSrc,void *pvDst,unsigned long ulsize);
/**********************************************************************************
*  Function：   ATP_FLASHCBB_GetSectorfromAddr_Assert
*  Description：测试ATP_FLASHCBB_GetSectorfromAddr函数是否成功。
*  Calls：      无
*  Input：      usSector：  用户预期的Sector的值
*               ulTestAddr: 地址的值
*               usChip:       需要验证的FLASH芯片号0,1
*  Return：     BHAL_E_FLASH_OK：       表示成功
*               HAL_E_RETVAL_EDEFAULT： 表示失败
*                                       并打印以FLASHCBB ERROR:开始的错误信息
***********************************************************************************/
int ATP_FLASHCBB_GetSectorfromAddr_Assert(unsigned short usSector,unsigned long ulTestAddr,unsigned short usChipID);

/**********************************************************************************
*  Function：   ATP_FLASHCBB_GetBasefromSector_Assert
*  Description：测试ATP_FLASHCBB_GetBasefromSector函数是否成功。
*  Calls：      无
*  Input:       usSectorNum：  传进来的用户计算出来FLASH初始化信息的值 
*               ulInitAddr:        用户预期的地址的值
*               usChip:       需要验证的FLASH芯片号0,1
*  Return：     BHAL_E_FLASH_OK：       表示成功
*               HAL_E_RETVAL_EDEFAULT： 表示失败
*                                       并打印以FLASHCBB ERROR:开始的错误信息
***********************************************************************************/
int ATP_FLASHCBB_GetBasefromSector_Assert(unsigned short usSectorNum,unsigned long ulInitAddr,unsigned short usChipID);

/**********************************************************************************
 * Function：   int ATP_FLASHCBB_Init_Assert
 * Description：测试初始化函数是否成功。
 * Calls：      无
 * Input：      pstFlashCbbValue：传进来的用户预期的FLASH初始化后的值 
 *              usChip:                 需要验证的FLASH芯片号0,1
 * Return：     BHAL_E_FLASH_OK：       表示成功
 *              HAL_E_RETVAL_EDEFAULT： 表示失败
 *                                      并打印以FLASHCBB ERROR:开始的错误信息.
**********************************************************************************/
int ATP_FLASHCBB_Init_Assert(ATP_FLASHCBB_InitAssert_ST *stFlashCbbValue,unsigned short usChip);

/**********************************************************************************
* Function：    int ATP_FLASHCBB_EWRblockSector_Assert
* Description： 测试FLASH的ERASE,WRITE,READ函数是否正确。
* Calls：       无
* Input：       usSectorNum：  需要测试Sector块值 
*               usChip:       需要验证的FLASH芯片号0,1
* Return：      BHAL_E_FLASH_OK：       表示成功
*               HAL_E_RETVAL_EDEFAULT： 表示失败
*                                       并打印以FLASHCBB ERROR:开始的错误信息
**********************************************************************************/

int ATP_FLASHCBB_EWRblockSector_Assert(unsigned short usSectnum,unsigned short usChipID);

/**********************************************************************************
* Function：    int ATP_FLASHCBB_EWRblockAddr_Assert
* Description： 测试FLASH的ERASE,WRITE,READ函数是否正确。
* Calls：       无
* Input：       ulBaseAddr：  需要测试基地址 
*               usChip:       需要验证的FLASH芯片号0,1
* Return：      BHAL_E_FLASH_OK：       表示成功
*               HAL_E_RETVAL_EDEFAULT： 表示失败
*                                       并打印以FLASHCBB ERROR:开始的错误信息
**********************************************************************************/
int ATP_FLASHCBB_EWRblockAddr_Assert(unsigned long ulBaseAddr,unsigned short usChipID);
#endif
#ifdef __cplusplus
}
#endif
#endif
