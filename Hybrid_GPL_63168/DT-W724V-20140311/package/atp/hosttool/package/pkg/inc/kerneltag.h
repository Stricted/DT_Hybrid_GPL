
#ifndef _KERNELTAG_H_
#define _KERNELTAG_H_


/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file kerneltag.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2011-12-28
*/

#define ATP_KERNEL_IMG_MAGIC_NUM  0x54781985

#define ATP_CHIP_ID_LEN		8	
#define ATP_BOARD_ID_LEN    16
#define ATP_CRC_LEN          4

#if defined(CONFIG_HTML_PARTITION) || defined(HTML_PARTITION)
#define ATP_TAG_FLASH_BOOTLOADER_INDEX          0
#define ATP_TAG_FLASH_MAIN_TAG_INDEX            1
#define ATP_TAG_FLASH_SLAVE_TAG_INDEX           2
#define ATP_TAG_FLASH_MAIN_KERNEL_INDEX         3
#define ATP_TAG_FLASH_MAIN_FS_INDEX             4
#define ATP_TAG_FLASH_SLAVE_KERNEL_INDEX        5
#define ATP_TAG_FLASH_SLAVE_FS_INDEX            6
#define ATP_TAG_FLASH_MIAN_WEB_INDEX            7
#define ATP_TAG_FLASH_SLAVE_WEB_INDEX           8
#define ATP_TAG_FLASH_MIDDLE_WARE_INDEX         9
#define ATP_TAG_FLASH_MUDULE_INDEX              10
#define ATP_TAG_FLASH_RESERVED_INDEX            11
#define ATP_TAG_FLASH_CURRENT_CONFIG_INDEX      12
#define ATP_TAG_FLASH_CUSTOMIZE_CONFIG_INDEX    13
#define ATP_TAG_FLASH_FACTORY_CONFIG_INDEX      14
#define ATP_TAG_FLASH_TEMP_PARAM_CONFIG_INDEX   15
#define ATP_TAG_FLASH_TR069_CERT_INDEX          16
#define ATP_TAG_FLASH_LOG_INDEX                 17
#define ATP_TAG_FLASH_CRASH_DUMP_INDEX          18
#define ATP_TAG_FLASH_PPP_SESSION_INDEX         19
#define ATP_TAG_FLASH_DECT_PHOTOBOOK_INDEX      20
#define ATP_TAG_FLASH_DECT_CALIBRATE_INDEX      21
#define ATP_TAG_FLASH_RESERVED_CONFIG_INDEX     22
#define ATP_TAG_FLASH_CONFIG_BACK_INDEX         23
#define ATP_TAG_FLASH_RESERVED_CONFIG_BACK_INDEX     24
#define ATP_TAG_FLASH_EQUIP_PARAM_INDEX         25
#define ATP_TAG_FLASH_WLANRF_INDEX              26
#define ATP_TAG_FLASH_FLAG_INDEX                27
#define ATP_TAG_FLASH_BAD_BLOCK_TABLE_INDEX     28
#define ATP_TAG_FLASH_TOTAL_INDEX               29
#else
#define ATP_TAG_FLASH_BOOTLOADER_INDEX 			0
#define ATP_TAG_FLASH_MAIN_TAG_INDEX 			1
#define ATP_TAG_FLASH_SLAVE_TAG_INDEX 			2
#define ATP_TAG_FLASH_MAIN_KERNEL_INDEX 		3
#define ATP_TAG_FLASH_MAIN_FS_INDEX 			4
#define ATP_TAG_FLASH_SLAVE_KERNEL_INDEX 		5
#define ATP_TAG_FLASH_SLAVE_FS_INDEX 			6
#define ATP_TAG_FLASH_MIDDLE_WARE_INDEX 		7
#define ATP_TAG_FLASH_MUDULE_INDEX 		        8
#define ATP_TAG_FLASH_RESERVED_INDEX            9
#define ATP_TAG_FLASH_CURRENT_CONFIG_INDEX      10
#define ATP_TAG_FLASH_CUSTOMIZE_CONFIG_INDEX   11
#define ATP_TAG_FLASH_FACTORY_CONFIG_INDEX      12
#define ATP_TAG_FLASH_TEMP_PARAM_CONFIG_INDEX   13
#define ATP_TAG_FLASH_TR069_CERT_INDEX          14
#define ATP_TAG_FLASH_LOG_INDEX                 15
#define ATP_TAG_FLASH_CRASH_DUMP_INDEX          16
#define ATP_TAG_FLASH_PPP_SESSION_INDEX         17
#define ATP_TAG_FLASH_DECT_PHOTOBOOK_INDEX      18
#define ATP_TAG_FLASH_DECT_CALIBRATE_INDEX      19
#define ATP_TAG_FLASH_EQUIP_PARAM_INDEX         20
#define ATP_TAG_FLASH_WLANRF_INDEX              21
#define ATP_TAG_FLASH_FLAG_INDEX                22
#define ATP_TAG_FLASH_BAD_BLOCK_TABLE_INDEX     23
#define ATP_TAG_FLASH_TOTAL_INDEX               24
#define ATP_TAG_FLASH_CONFIG_BACK_INDEX         25
#endif


#define ATP_TAG_FLASH_MAX_INDEX				   32

#define ATP_TAG_BASIC_SIZE                     61
#define ATP_TAG_FLASH_SIZE                     64

#define ATP_KILO_SIZE                          1024

//!\brief image flash address & len
/*!
 *
 */
typedef struct tagATP_UPG_TAG_FLASH_PAIR_ST
{
    unsigned int ulAddress;
    unsigned int ulLen;
}ATP_UPG_TAG_FLASH_PAIR_ST;

//!\brief flash layout
/*!
 * 256 
 */
typedef struct tagATP_UPG_TAG_FlashLayout_ST
{
    ATP_UPG_TAG_FLASH_PAIR_ST astFlashInfo[ATP_TAG_FLASH_MAX_INDEX];
}ATP_UPG_TAG_FlashLayout_ST;

typedef struct tagATP_UPG_SIGNATUREINFO_ST
{    
    unsigned char                   ucHasSign;      /*!< 是否签名 */    
    unsigned char                   ucRSABits;      /*!< RSA位 */     
    unsigned char                   ucHashAlgo;     /*!< hash算法 */
    unsigned char                   ucReserved;     /*!< 保留 */
} ATP_UPG_SIGNATUREINFO_ST;

//!\brief basic info
/*!
 *  252
 */
typedef struct tagATP_UPG_TAG_BASIC_ST
{
    unsigned int                ulMagicNumber;
    unsigned char               chipId[ATP_CHIP_ID_LEN];
    unsigned char               boardId[ATP_BOARD_ID_LEN];     
    unsigned char               voiceboardId[ATP_BOARD_ID_LEN];
    unsigned int                ulBigEndian;          
    ATP_UPG_SIGNATUREINFO_ST    stSignature;
    unsigned int                ulSignLen;
    unsigned int                ulSystemType;
    unsigned int                ulFsLen;
    unsigned int                ulKernelLen;
    /*  新增信息在此处添加 */
}ATP_UPG_TAG_BASIC_ST;

//!\brief image tag define
/*!
 *
 */
typedef struct tagATP_UPG_Tag_ST
{
    union{
        ATP_UPG_TAG_BASIC_ST stBasicInfo;
        unsigned int aulBasicInfo[ATP_TAG_BASIC_SIZE];
    }basic;

    union{
        ATP_UPG_TAG_FlashLayout_ST stFlashLayoutInfo;
        unsigned int aulFlashInfo[ATP_TAG_FLASH_SIZE];
    }flashLayout;

    unsigned char   imageValidationFsCrc[ATP_CRC_LEN];		//fs的crc校验值，ATP_CRC_LEN = 4
    unsigned char   imageValidationKernelCrc[ATP_CRC_LEN];	//Kernel的crc校验值
    unsigned char   aucHeadCRC[ATP_CRC_LEN];
}ATP_UPG_Tag_ST,*PATP_UPG_Tag_ST;



#endif
