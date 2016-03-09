/* ************************************************************************** 
 * �ļ����� : ����Intel flash �������õ���һЩ��Ϣ,Intel flash������ͷ�ļ�
 * ����     : ������y42304 @ HAL&OS
 *
 * �޸���ʷ :
 *    1. �����ļ�ʱ��: 2005��12��30�� 
 *    
 *
 * ˵��: 1. ���÷�Χ: �ʺ�Intel/AMD flash(8λ/16λ/32λ):
 *       2. ʵ�ַ���: ����CFI�ӿ�����̬��ȡflash�Ĵ�С�������Ϳ��С����Ϣ��,
 *            ���Լ���Ҫ��ֲ��flash������������ͬ�������ṹ������λ���Intel
 *          оƬ�ϣ������ֻ��������������:
 *          A: ���¶����FLASH_ADDRESS_BASE��ָ���µ�flashоƬ�Ļ���ַ��
 *          B: ���¶����CFIDEB_BUSWIDTH��ָ��������λ�� 
 *          C: ȷ��Ŀ����ֽ����Ƿ�ͽ�����뻷����CPU���ֽ����Ƿ���ͬ����ͬ����
 *             �����CFI_BIG_ENDIALN,�������CFI_LITTLE_ENDIAN��
 *             ����: ��Ϊ�ڶ�ȡCFI��Ϣʱ�����ֽ����йء�
 *          D: ����flash_probe(ulFlashBaseAddr, 0, 1)����̬̽��flash��Ϣ����ӡ��
 *             flash����Ϣ��Ȼ�����ʵ�ʵ�flash��Ϣ�Ƚϣ�������Ƿ������
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
 *   ������һЩ��֧��CFI��һЩFLASH��CHIP ID�Ķ���
 *   CHIP ID   ��Manufacture ID(MID) ��8λ��Device ID(DID)��8λ��ƴ�ɡ�
 *             MID ��8λ ����CHIP ID�ĸ��ֽ���
 *             DID��8λ����CHIP ID�ĵ��ֽ��ϡ�
 *             ��CHIP ID = ((MID & 0xFF) << 8) | (DID & 0xFF)
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

/*AMD���͵�FLASH ID��,Ŀǰ�����ṩGEOMERY�ж�*/
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
/*AMD���͵�FLASH ID��,Ŀǰ�����ṩGEOMERY�ж�*/

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


/*INTEL���͵�FLASH ID��,Ŀǰ�����ṩGEOMERY�ж�*/
#define ID_I28F160C3T           0x88C2
#define ID_I28F160C3B           0x88C3
#define ID_I28F320C3T           0x88C4
#define ID_I28F320C3B           0x88C5
#define ID_I28F640J3	        0x8916
/*INTEL���͵�FLASH ID��,Ŀǰ�����ṩGEOMERY�ж�*/

/*********************************************************************
 * ��;: Flash Bottom/Top boot flag
 ********************************************************************/
#define CFIDEV_BOTTOMBOOT           2
#define CFIDEV_TOPBOOT              3
#define CFIDEV_UNIFORM_BP           4
#define CFIDEV_UNIFORM_TP           5


/**********************************************************************
 *   ����FLASH���豸ID�Ķ���
 **********************************************************************/


/*********************************************************************
 * ��;: ����FLASHоƬID��
 *       1. ֧��CFI��������ȷ��CHIP ID
 *       2. ��֧��CFI����CHIP ID�����0xFFFF
  *   CHIP ID  ��Manufacture ID(MID) ��8λ��Device ID(DID)��8λ��ƴ�ɡ�
 *             MID ��8λ ����CHIP ID�ĸ��ֽ���
 *             DID��8λ����CHIP ID�ĵ��ֽ��ϡ�
 *             ��CHIP ID = ((MID & 0xFF) << 8) | (DID & 0xFF)
 *********************************************************************/
#define FLASH_CHIP_ID           0xFFFF


/*************************************************************************************************
* ��;���������ϵͳ����: linux, pSos,VxWorks.     
* ˵����������дFLASH�ڼ䣬��Ҫ���жϣ��������жϺ���������OS��������Ҫ
*       �����û��������ļ����õĲ���ϵͳ���ͣ���������3����֮һ��
*************************************************************************************************/
// #define PSOS_OS
 //#define VXWORKS_OS
#define LINUX_OS
/************************************************************************
 * ��;: �������FLASH�ĳ�ʱʱ��
 *       1. ֧��CFI�������޸������ĸ���Ķ���ֵ
 *       2. ��֧��CFI������ο�FLASHоƬ���ϣ���������ȷ����Ӧ��ʱֵ  
 *
 ************************************************************************/
#define TIMEOUT_BLOCK_ERASE     11   /*2^10 ms check if block erase command has completed */
#define TIMEOUT_CHIP_ERASE      11   /*2^n ms (if supported; 00h = not supported)*/
#define TIMEOUT_SINGLE_WRITE    10  /*2^n us check if byte/word command has completed */
#define TIMEOUT_BUFFER_WRITE    10    /*2^10 us check if buffer write command has completed */
#define MAXSECTORS              1024 /*����sector��*/


#define FLASH_INFO_SHOW

/********************************************************************************************/
/***********/
/**********************************************************************************
             �û���Ҫ�޸ĵĺ궨�岿�ֽ���
 **********************************************************************************/
/********************************************************************************
 ˵��: �������е������Ͷ�����Intel flash оƬ�ϲ���Ҫ���� 
 ********************************************************************************/

#define FLASH_MAX_CHIPSET               2   /* ����FLASHоƬ��Ŀ */

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
#define CFI_MAX_ERASE_REGIONS           8   /* flash�����ɲ������������� */

/*
 *  Start: Flash ���������
 */
#define QUERY                           0x98  /* ����Flash��ѯ״̬ */
#define READ_ID                         0x90  /* ��Flash ID�������� */

#define WSM_ERROR                       0x3A     

#ifdef INTEL_FLASH

#define READ_ARRAY                      0xFF  /* ����Flash ������״̬ */
#define SR_IS_READY                     0x80  /* Flash��׼������ */
#define CLEAR_STATUS                    0x50  /* ���Flash״̬�������� */
#define READ_STATUS                     0x70  /* ��״̬�Ĵ��� */
#define CLEAR_LOCK_BIT                  0x60  /* ����BLOCK����������1 */
#define CONFIRM_CLEAR_LOCK_BIT          0xD0  /* ����BLOCK����������2 */
#define WRITE_TO_BUFFER                 0xE8  /* ��д���巽ʽдFLASH��������*/
#define INTEL_BYTE_PROGRAM              0x10  /* ���ֽڱ��������*/
#define INTEL_WORD_PROGRAM              0x40  /* ���ֱ��������*/
#define EACH_BYTE_NUMS_WRITE            32    /* ����дBuffer��ʽʱÿ�������д���ֽ���Ŀ*/
#define CONFIRM_WRITE                   0xD0  /* ȷ��д�����Ƿ���ɵ������� */
#define SECTOR_ERASE                    0x20  /* ����Flash��������� */
#define CONFIRM_ERASE                   0xD0  /* ȷ�ϲ��������Ƿ���ɵ������� */


#else    /* ��ѭAMD ��̹���Flash */

/*8λflash ʹ��*/
#define READ_ARRAY                      0xF0  /* ����Flash ������״̬ */
#define AMD_CMD_UNLOCK_1                0xAA  /* AMD����������1*/
#define AMD_CMD_UNLOCK_2                0x55  /* AMD����������2*/

/* ��UNLOCK BYPASS�йص������� */
#define AMD_UNLOCK_BYPASS               0x20
#define AMD_UNLOCK_BYPASS_RESET1        0x90
#define AMD_UNLOCK_BYPASS_RESET2        0x00


/* �����Flash�йص������� */
#define AMD_SETUP_ERASE                 0x80
#define AMD_SECTOR_ERASE                0x30
#define AMD_CHIP_ERASE                  0x10

/* ��дFlash�йص������� */
#define AMD_PROGRAM_WRITE               0xA0

#define AMD_UNLOCK_ADDR1                0x555
#define AMD_UNLOCK_ADDR2                0x2AA

#endif/*end of INTEL_FLASH*/

/* End: ��������ֵĶ��� */

#define INVALID_CHIP_ID                 0xFFFF


#define FLASH_WRITE_MAX_TRY_COUNT        0x7FFFF    /* ����д��ѯ���� */
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

#define FLASH_ADDR_BASE_INVALID         0xFFFFFFFF /* �Ƿ���FLASH����ַ */


/* �ڲ�ʹ�õĽṹ�� */
// Query ID structure
typedef struct hal_S_FlashCfiQueryId 
{
    unsigned char ucQuery[3];              // 0x10 - 0x12: query-unique ACSII string "QRY"
    unsigned char ucReserved1;             // ������Ϊ��4�ֽڶ���
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
    unsigned char ucReserved2;             // ������Ϊ��4�ֽڶ���    
    unsigned short usInterfaceDesc;        // 0x28 - 0x29 : flash device interface description
    
    unsigned short usMaxMultibyteWrite;    // 0x2a - 0x2b : maximum number of bytes in multi-byte write = 2^n
    unsigned char ucEraseRegionsNum;       // 0x2c : number of erase block regions within device
    unsigned char ucReserved3;             // ������Ϊ��4�ֽڶ���    
    
    /******************************************************************
    * bits 31-16 = Z:  ���С = Z��256�ֽڣ����� Z=0,��ʾ���СΪ128K
    * bits 15-0  = Y:  ����Ŀ = Y+1
    ******************************************************************/    
    unsigned int uiEraseRegionInfo[CFI_MAX_ERASE_REGIONS];                                                                                    
}FLASHCBB_QUERYID_ST;


/* ÿ���������ʼƫ�Ƶ�ַ�����С�Ϳ���*/
typedef struct hal_S_FlashEraseRegions 
{
    unsigned long ulStartOff;     /* ��������Flash�е�ƫ�Ƶ�ַ */        
    unsigned long ulBlockSize;    /* ��������Ŀ��С*/
    unsigned long ulBlocksNum;    /* ������Ŀ��� */
}FLASHCBB_ERASEREGIONS_ST;

typedef struct hal_S_FlashInfoManual 
{
    unsigned short usChipID;   /* Flash оƬID= ((MID & 0xFF) << 8) | (DID & 0xFF)*/
    unsigned long ulSize;     /* Flash оƬ�Ĵ�С */
    int iEraseRegionsNum;     /* Flash оƬ�Ŀɲ���������Ŀ */

    /* Flash оƬ�ɲ����������Ϣ�ṹ */
    FLASHCBB_ERASEREGIONS_ST stEraseRegions[CFI_MAX_ERASE_REGIONS];
}FLASHCBB_INFO_ST;

/* CFI information block used by this driver  */
typedef struct hal_S_FlashCfiInfo
{
    int iExist;                /* ��ʾ�Ƿ����Flash; 1: ��ʾ����; 0: ��ʾ������*/
    int iChipState;            /* Flash��״̬ */
    unsigned long ulBaseAddr;  /* Flash��״̬ */
    unsigned long ulSize;      /* ��ƬFlashоƬ�Ĵ�С */
    unsigned short usSectorNum; /*Sector����*/
    
    unsigned short usSect;                  /* # of sectors -- 19 in LV, 22 in DL Ŀǰδ����*/
    
    unsigned int ulBootFlag;
    
    struct {
    unsigned long ulSize;           /* # of bytes in this sector */
    unsigned long ulBase;           /* offset from beginning of device */
    unsigned int ulBank;            /* 1 or 2 for DL; 1 for LV */
     } stSector[MAXSECTORS];  /* per-sector info */

    FLASHCBB_QUERYID_ST stIdent; /* CFI�ṹ��Ϣ */
    FLASHCBB_ERASEREGIONS_ST stEraseRegions[CFI_MAX_ERASE_REGIONS];
}FLASHCBB_INFOTBL_ST, *FLASHCBB_INFOTBL_ST_PTR;


/* Ŀ���CPU���ֽ��� */
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

/* ��ʾ�ڴ��е������Ѹ��£���ȡ�Ը��ڴ浥Ԫ��ĳ���Ĵ��������ݲ�һ�� */
#define mb()  __asm__ __volatile__ ("" : : : "memory")


#ifdef FLASHCBB_USE_GPIO

/*****************************************************************************/
/*�ṩ���ⲿʹ�õĽṹ��*/
/******************************************************************************
 *��������  :  FLASHCBB�ṩ�Ĺ��Ӻ���.
 *GPIO�ӿ�,��ͬ��CPU��Ҫʹ�ò�ͬ�Ľӿ�
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
 *��������  :  FLASHCBB�ṩ�Ĺ��Ӻ���.
 *GPIO�ӿ�,��ͬ��CPU��Ҫʹ�ò�ͬ�Ľӿ�
 ******************************************************************************
 *������    :  fp_FlashCbb_Gpio_Read_*
 *����      :  ����pucflash_addr��ַ�е�ֵ
 *����      :  p**flash_addr ��ֵַ
 
 *�� �� ֵ  :  ������ֵ                     : �ɹ�
               HAL_E_RETVAL_EFLASH_READ     : ʧ��
 ******************************************************************************
 *������    :  fp_FlashCbb_Gpio_Write_*
 *����      :  ��Dataд��pucflash_addr��ַ
 *����      :  pucflash_addr ��ֵַ
               **Data��д���ֵ
 
 *�� �� ֵ  :  ��
               ���������ӡ������Ϣ
 ******************************************************************************
 * ˵��     :  ʹ��ʱ�ɸ������ѡ���ʼ���ض��Ĺ���,
               û�ж���Ľӿ�Ҫ��ʼ��ΪNULL
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
 *��������  :  ��ʼ�����Ӻ���
 *�������  :  ATP_FLASHCBB_CALLBACK_ST ���ӵĽṹ��
               
 *�� �� ֵ  :  
 *             

 * ˵��     :  
 *****************************************************************************/

 void ATP_FLASHCBB_InitHook(ATP_FLASHCBB_CALLBACK_ST *stHook);


#endif

int ATP_FLASHCBB_Init(void);

int ATP_FLASHCBB_GetSectorSize(unsigned long ulSrcAddr);


/********�ṩ���ⲿ�Ľӿ�**********************/
 /******************************************************************************
 *��������  :  ���FLASH�Ĵ�С
 *�������  :  
               
 *�� �� ֵ  :  
 *             

 * ˵��     :  Ŀǰδʹ��
 *****************************************************************************/

unsigned long ATP_FLASHCBB_GetSize(void);

/******************************************************************************
 *��������  :  ��ȡFLASH����Ϣ
 *�������  :  stFlashInfo: �洢 Flash ��Ϣ�ṹ��
 *              
 *�� �� ֵ  :  HAL_E_RETVAL_OK:  �ɹ�
 *             ������:           ʧ�� 
 * 
 * ˵��     :  
 *****************************************************************************/
int ATP_FLASHCBB_GetFlashInfo(FLASHCBB_INFO_ST stFlashInfo[FLASH_MAX_CHIPSET]);

/******************************************************************************
 *��������  :  �������е�FLASH����Ϣ
 *�������  :  ��
 *              
 *�� �� ֵ  :  
 * ˵��     :  ��ʱδʵ��
 *****************************************************************************/
int ATP_FLASHCBB_EraseAllChip(void);

/******************************************************************************
 *��������  :  ����ָ������Ŀ�ռ�
 *�������  :  ulBaseAddr: Flash ���Ե�ַ
 *             ulOffsetAddr: ��ulBaseAddrΪ����ַ��ƫ����
 *             ulLen: �����ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  0:  �ɹ�
 *             1:  ʧ�� 
 * 
 * ˵��     :  
 *****************************************************************************/
int ATP_FLASHCBB_Eraseblock(unsigned long ulAddress, unsigned long ulLen);

/******************************************************************************
 *��������  :  ����ָ������Ŀ�ռ�
 *�������  :  ulBaseAddr: Flash ���Ե�ַ
 *             ulOffsetAddr: ��ulBaseAddrΪ����ַ��ƫ����
 *             ulLen: �����ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  0:  �ɹ�
 *             1:  ʧ�� 
 * 
 * ˵��     :  ΪCNXT���ṩ�Ľӿ�
 *****************************************************************************/
int ATP_FLASHCBB_Eraseblock2(unsigned long ulBaseAddr, unsigned long ulOffsetAddr , unsigned long ulLen);

#ifdef SUPPORT_ATP_NANDFLASH
/******************************************************************************
 *��������  :  ��ȡָ�����������
 *�������  :  ulSrcAddr: Flash ���Ե�ַ
 *             ulDstAddr: �ڴ��ַ
 *             ulLen: ��ȡ�ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  HAL_E_RETVAL_OK:  �ɹ�
 *             ������:           ʧ�� 
 * 
 * ˵��     :  
 *****************************************************************************/
int ATP_FLASHCBB_ReadFlash_CountError(unsigned long ulFlashAddr,void *pszDataBuffer, unsigned long ulLen,unsigned long *ulCount);
#endif

/******************************************************************************
 *��������  :  ��ȡָ�����������
 *�������  :  ulSrcAddr: Flash ���Ե�ַ
 *             ulDstAddr: �ڴ��ַ
 *             ulLen: ��ȡ�ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  HAL_E_RETVAL_OK:  �ɹ�
 *             ������:           ʧ�� 
 * 
 * ˵��     :  
 *****************************************************************************/
int ATP_FLASHCBB_Read(unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLen);

/******************************************************************************
 *��������  :  ��ָ��Flash����д
 *�������  :  ulFlashAddr: д���FLASH���Ե�ַ
 *             pDataBuffer: pDataBuffer������Դ
 *             ulLen: д�ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  HAL_E_RETVAL_OK:  �ɹ�
 *             ������:  ʧ�� 
 * 
 * ˵��     :  
 *****************************************************************************/
 
int ATP_FLASHCBB_Write(unsigned long ulDstAddr, const void *pDataBuffer, unsigned long ulLen);
#ifdef SUPPORT_ATP_NANDFLASH
int ATP_FLASHCBB_Jffs_Write(void *buf, unsigned long toaddr , int len);
int ATP_FLASHCBB_Jffs_Erase(unsigned long addr , int len);
#endif
/******************************************************************************
 *��������  :  ��ָ��Flash����д
 *�������  :  ulBaseAddr: Flash ���Ե�ַ
 *             ulOffsetAddr: ��ulBaseAddrΪ����ַ��ƫ����
 *             ulLen: д�ĳ���(���ֽ�Ϊ��λ)               
 *              
 *�� �� ֵ  :  HAL_E_RETVAL_OK:  �ɹ�
 *             ������:  ʧ�� 
 * 
 * ˵��     :  ΪCNXTƥ����ṩ�Ľӿ�
 *****************************************************************************/

 int ATP_FLASHCBB_Write2(unsigned long ulBaseAddr, unsigned long uloffsetAddr, void *dataptr, unsigned long ulLen);
/*l63336 add for adapt CNXT interface*/

 /******************************************************************************
 *��������  :  ��Sector�Ż��Flash��Ļ���ַ
 *�������  :  Sector ���              
 *�� �� ֵ  :  
 *             NULL:              ʧ��
 *             Sector��Ļ���ַ : �ɹ�

 * ˵��     :  
 *****************************************************************************/
 unsigned long ATP_FLASHCBB_GetBasefromSector(unsigned short usSector,unsigned short usChipID);

 /******************************************************************************
 *��������  :  ��Sector�Ż��Flash��Ĵ�С
 *�������  :  Sector ���              
 *�� �� ֵ  :  
 *             NULL:              ʧ��
 *             Sector��Ļ���ַ : �ɹ�

 * ˵��     :  
 *****************************************************************************/
 unsigned long ATP_FLASHCBB_GetSizefromSector(unsigned short usSector,unsigned short usChipID);

 /******************************************************************************
 *��������  :  �ɵ�ַ���Flash��Sector���
 *�������  :  ulAddr ��ַ
               
 *�� �� ֵ  :  
 *             HAL_E_RETVAL_EFLASH_PARAMETER: ʧ��
 *             ���:                          �ɹ�

 * ˵��     :  
 *****************************************************************************/
 unsigned short ATP_FLASHCBB_GetSectorfromAddr(unsigned long ulAddr);


 /******************************************************************************
 *��������  :  ���Sector�������
 *�������  :  ��
               
 *�� �� ֵ  :  Sector ����              : ��ȷ
 *             HAL_E_RETVAL_EDEFAULT    : ����          
 *             

 * ˵��     :  ���������FLASH��������FLASH���ܵ�Sector����
 *****************************************************************************/
 unsigned int ATP_FLASHCBB_GetSectorNum(void);
 
 /******************************************************************************
 *��������  :  �ж����64K�Ƿ���8*8K
 *�������  :  
               
 *�� �� ֵ  :  
 *             

 * ˵��     :  Ŀǰδʹ��
 *****************************************************************************/
#if 0
 static void ATP_FLASHCBB_JudgeFlashIsTopDevice8K(void);
#endif

#ifdef FLASHCBB_DEBUG
/*l63336 add for adapt CNXT interface*/

/*l63336 CNXT FLASH��ز��Ժ���*/
/*���Խӿ����õĽṹ��*/
typedef struct{
    unsigned short usIsChipExt;/*оƬ�Ƿ���� 1:����; 0:������*/
    unsigned short usSectorNum;/*оƬ��Sector����*/
    unsigned short usEraseNum;/*Erase������*/
    unsigned short usEraseBlockNum;/*��һ��Erase����Ŀ�����*/
    unsigned long usEraseSize;/*��һ��Erase����Ŀ��С*/
    unsigned long usFirstSectorSize;/*��һ��Sector��Ĵ�С*/
    unsigned long usLastSectorSize;/*���һ��Sector��Ĵ�С*/
}ATP_FLASHCBB_InitAssert_ST;


int ATP_FLASHCBB_MemCmp(void *pvSrc,void *pvDst,unsigned long ulsize);
/**********************************************************************************
*  Function��   ATP_FLASHCBB_GetSectorfromAddr_Assert
*  Description������ATP_FLASHCBB_GetSectorfromAddr�����Ƿ�ɹ���
*  Calls��      ��
*  Input��      usSector��  �û�Ԥ�ڵ�Sector��ֵ
*               ulTestAddr: ��ַ��ֵ
*               usChip:       ��Ҫ��֤��FLASHоƬ��0,1
*  Return��     BHAL_E_FLASH_OK��       ��ʾ�ɹ�
*               HAL_E_RETVAL_EDEFAULT�� ��ʾʧ��
*                                       ����ӡ��FLASHCBB ERROR:��ʼ�Ĵ�����Ϣ
***********************************************************************************/
int ATP_FLASHCBB_GetSectorfromAddr_Assert(unsigned short usSector,unsigned long ulTestAddr,unsigned short usChipID);

/**********************************************************************************
*  Function��   ATP_FLASHCBB_GetBasefromSector_Assert
*  Description������ATP_FLASHCBB_GetBasefromSector�����Ƿ�ɹ���
*  Calls��      ��
*  Input:       usSectorNum��  ���������û��������FLASH��ʼ����Ϣ��ֵ 
*               ulInitAddr:        �û�Ԥ�ڵĵ�ַ��ֵ
*               usChip:       ��Ҫ��֤��FLASHоƬ��0,1
*  Return��     BHAL_E_FLASH_OK��       ��ʾ�ɹ�
*               HAL_E_RETVAL_EDEFAULT�� ��ʾʧ��
*                                       ����ӡ��FLASHCBB ERROR:��ʼ�Ĵ�����Ϣ
***********************************************************************************/
int ATP_FLASHCBB_GetBasefromSector_Assert(unsigned short usSectorNum,unsigned long ulInitAddr,unsigned short usChipID);

/**********************************************************************************
 * Function��   int ATP_FLASHCBB_Init_Assert
 * Description�����Գ�ʼ�������Ƿ�ɹ���
 * Calls��      ��
 * Input��      pstFlashCbbValue�����������û�Ԥ�ڵ�FLASH��ʼ�����ֵ 
 *              usChip:                 ��Ҫ��֤��FLASHоƬ��0,1
 * Return��     BHAL_E_FLASH_OK��       ��ʾ�ɹ�
 *              HAL_E_RETVAL_EDEFAULT�� ��ʾʧ��
 *                                      ����ӡ��FLASHCBB ERROR:��ʼ�Ĵ�����Ϣ.
**********************************************************************************/
int ATP_FLASHCBB_Init_Assert(ATP_FLASHCBB_InitAssert_ST *stFlashCbbValue,unsigned short usChip);

/**********************************************************************************
* Function��    int ATP_FLASHCBB_EWRblockSector_Assert
* Description�� ����FLASH��ERASE,WRITE,READ�����Ƿ���ȷ��
* Calls��       ��
* Input��       usSectorNum��  ��Ҫ����Sector��ֵ 
*               usChip:       ��Ҫ��֤��FLASHоƬ��0,1
* Return��      BHAL_E_FLASH_OK��       ��ʾ�ɹ�
*               HAL_E_RETVAL_EDEFAULT�� ��ʾʧ��
*                                       ����ӡ��FLASHCBB ERROR:��ʼ�Ĵ�����Ϣ
**********************************************************************************/

int ATP_FLASHCBB_EWRblockSector_Assert(unsigned short usSectnum,unsigned short usChipID);

/**********************************************************************************
* Function��    int ATP_FLASHCBB_EWRblockAddr_Assert
* Description�� ����FLASH��ERASE,WRITE,READ�����Ƿ���ȷ��
* Calls��       ��
* Input��       ulBaseAddr��  ��Ҫ���Ի���ַ 
*               usChip:       ��Ҫ��֤��FLASHоƬ��0,1
* Return��      BHAL_E_FLASH_OK��       ��ʾ�ɹ�
*               HAL_E_RETVAL_EDEFAULT�� ��ʾʧ��
*                                       ����ӡ��FLASHCBB ERROR:��ʼ�Ĵ�����Ϣ
**********************************************************************************/
int ATP_FLASHCBB_EWRblockAddr_Assert(unsigned long ulBaseAddr,unsigned short usChipID);
#endif
#ifdef __cplusplus
}
#endif
#endif
