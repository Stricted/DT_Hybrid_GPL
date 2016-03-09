/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : bhalapi.h
  作    者  : l69021
  版    本  : 1.0
  创建日期  : 2007-6-30
  描    述  :
  函数列表  :

  历史记录      :
   1.日    期   : 2007-6-30
     作    者   : l69021
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef _ATPFLASHAPI_H_
#define _ATPFLASHAPI_H_


#include <linux/compiler.h>
#include <mtd/mtd-user.h>
#include "atptypes.h"
#include "bhal.h"
//#include "bcmTag.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*****************UPG INTERFACE  start******************/ 
//for UPG interface
#define IMAGE_MAIN_S             "MAINSS"
#define IMAGE_MAIN_F             "MAINFF"
#define IMAGE_SLAVE_F            "SLAVEFF"
#define IMAGE_SLAVE_S            "SLAVESS"

#define PROC_LINE_LEN            256
typedef enum
{
    UPLOAD_OK,
    UPLOAD_FAIL_NO_MEM,
    UPLOAD_FAIL_ILLEGAL_IMAGE,
} UPLOAD_ERR_EN;

typedef enum
{
    MAIN_RUN,
    SLAVE_RUN,
    NETBOOT_RUN,
    UPG_ERR
} UPG_GRADE_EN;

typedef enum
{
    MAIN_F,
    SLAVE_F,
}ATP_UPG_EN;

typedef enum
{
    UPG_FLAG_MAIN_FF,
    UPG_FLAG_MAIN_SS,
    UPG_FLAG_SLAVE_FF,
    UPG_FLAG_SLAVE_SS,
    UPG_FLAG_ERR
}ATP_UPG_FLAG_EN;

/*log类型*/
typedef enum 
{
    FOM_E_DEVICELOG,
    FOM_E_VOICELOG
}FOM_LOG_TYPE;

VOS_INT32 ATP_FomFlashInit();
VOS_INT32 ATP_FOM_GetLogSize(FOM_LOG_TYPE enLogType, VOS_UINT32 *pulSize);
VOS_INT32 ATP_FOM_WriteLog(FOM_LOG_TYPE enLogType, VOS_INT8 *pcSrcBuf,VOS_UINT32 pulSize);
VOS_INT32 ATP_FOM_ReadLog(FOM_LOG_TYPE enLogType, VOS_INT8 *pcSrcBuf,VOS_UINT32 pulSize);


/*for upgrade log*/
extern char g_acFirmwareVer[64];
extern VOS_BOOL g_bBootloader;

VOS_UINT32 UPG_KillProcess( VOS_VOID );
//VOS_UINT32 ATP_FLASH_Upgrade(VOS_UINT32 ulmainoffset,VOS_UINT32 ulslaveoffset,
//	VOS_UINT8 *pcImage, VOS_UINT32 ulLen,VOS_UINT32 ulearseLen);
VOS_UINT32 ATP_FLASH_UpgradeKernel(VOS_UINT32 ulmainoffset,VOS_UINT32 ulslaveoffset,
	VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
VOS_UINT32 ATP_FLASH_UpgradeBootloader(VOS_UINT32 uloffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen,VOS_UINT32 ulearseLen);
VOS_UINT32 ATP_FLASH_UpgradeCfg(VOS_UINT32 uloffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen,VOS_UINT32 ulearseLen);
VOS_UINT32 ATP_FLASH_UpgradeTag(VOS_UINT32 ulmainoffset,VOS_UINT32 ulslaveoffset,
	VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
VOS_UINT32 ATP_FLASH_UpgradeDect(VOS_UINT32 ulmainoffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
UPG_GRADE_EN UPG_GetRunCurrentSystem( VOS_VOID );
VOS_INT32 ATP_FLASH_SetUpgFlag(ATP_UPG_FLAG_EN enUpgFlag);

/*****************UPG INTERFACE  end******************/ 


#define PPP_SESSION_SIZE               (2 * 1024)
#define PPP_SESSION_OFFSET              0

#define BHAL_WARM_RESET_FLASH                PPP_SESSION_OFFSET+PPP_SESSION_SIZE
#define BHAL_WARM_RESET_SIZE                 8

#define BHAL_WLAN_TIME_CONTROL_FLASH         BHAL_WARM_RESET_FLASH+BHAL_WARM_RESET_SIZE
#define BHAL_WLAN_TIME_CONTROL_SIZE          4

#define BHAL_WLAN_FLAG_FLASH                 BHAL_WLAN_TIME_CONTROL_FLASH+BHAL_WLAN_TIME_CONTROL_SIZE
#define BHAL_WLAN_FLAG_SIZE                  4

#define BHAL_WLAN_TIME_FLASH                 BHAL_WLAN_FLAG_FLASH+BHAL_WLAN_FLAG_SIZE
#define BHAL_WLAN_TIME_SIZE                  256

/*make a area store whether file is downloading*/
#define BHAL_UPG_IN_DOWNLOAD_FLASH         BHAL_WLAN_TIME_FLASH+BHAL_WLAN_TIME_SIZE
#define BHAL_UPG_IN_DOWNLOAD_SIZE           4

#define ATP_PPPC_IFC_LEN 32
//#define WAN_PPP_SESSION_LEN 64


#define FOM_NULL       ""
#define FOM_ZERO       0
#define FOM_ERR        -1

typedef enum
{
     BOOTLOAD,
     KERNEL,
     SOFT,
     /*add software version(web)*/
     SOFT_WEB,
     /*add software version(web)*/
#ifdef SUPPORT_ATP_BOARD_TYPE_DETECT
     PRODUCT_TYPE,
#endif
     ENUM_END
}BSP_E_SW_TYPE;


/* Flash分区 */
typedef enum 
{
    FOM_E_FLASH_AREA_BOOTLOADER = 0,         /* bootloader分区 */
    FOM_E_FLASH_AREA_MAIN_TAG,            /* 主系统Tag */
    FOM_E_FLASH_AREA_SLAVE_TAG,                /* 从系统Tag */ 
    FOM_E_FLASH_AREA_MAIN_KERNEL,            /* 内核分区 */
    FOM_E_FLASH_AREA_MAIN_FS,                /* 文件系统分区 */
    FOM_E_FLASH_AREA_SLAVE_KERNEL,            /* 内核分区 */
    FOM_E_FLASH_AREA_SLAVE_FS,                /* 文件系统分区 */
    FOM_E_FLASH_AREA_MIDDLEWARE,
    FOM_E_FLASH_AREA_MODULE,
    
    FOM_E_FLASH_AREA_RESERVED,              /* 保留配置分区 */
    
    FOM_E_FLASH_AREA_CURRENT_CONFIG,     /* 当前配置文件分区 */
    FOM_E_FLASH_AREA_CUSTOMIZE_CONFIG,   /* 运营商配置文件分区 */    
    FOM_E_FLASH_AREA_FACTORY_CONFIG,     /* 出厂配置文件分区 */
    FOM_E_FLASH_AREA_TEMP_PARAM,                /* 其它可变参数分区: 用来存放升级标识以及TR069升级参数 */
    FOM_E_FLASH_AREA_TR69_CERT,               /* TR069证书 */
    FOM_E_FLASH_AREA_LOG,                             /* 日志参数分区 */    
    FOM_E_FLASH_AREA_CRASH_DUMP_LOG,                             /* 日志参数分区 */ 
    FOM_E_FLASH_AREA_PPP_SESSION,                 /*PPP会话ID及MAC地址信息*/
    FOM_E_FLASH_AREA_DECT_PHONEBOOK,          /* DECT PHONEBOOT分区*/
    FOM_E_FLASH_AREA_DECT_CALIBRATE,          /* dect校准参数*/    
    FOM_E_FLASH_AREA_CONFIG_RESERVED,          /* 配置区预留分区*/  
    FOM_E_FLASH_AREA_CONFIG_BACK,          /* dect校准参数*/    
    FOM_E_FLASH_AREA_CONFIG_BACK_RESERVED,          /* 配置区预留分区*/  
    FOM_E_FLASH_AREA_EQUIP_PARAM,            /* MAC,SN参数分区 */    
    FOM_E_FLASH_AREA_UPG_FLAG,          /* UPG升级标识*/        
    FOM_E_FLASH_AREA_ALLFLASH,                /* 整个Flash分区 */  
    
    FOM_E_FLASH_AREA_END                      /* 结束项 */	
}FOM_E_FLASH_AREA;


#ifdef ATP_DEBUG
#define ATP_BHALAPI_DEBUG(format, args...)       {printf("\nFILE:%s,LINE:%d,FUNC:%s: ", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("\n");}
#else
#define ATP_BHALAPI_DEBUG(x...)
#endif

typedef enum tagATP_FOM_RET_CODE_EN
{
    ATP_FOM_RET_READ_ERR   = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_BHALAPI),
    ATP_FOM_RET_WRITE_ERR,
    ATP_FOM_RET_ERASE_ERR,
    ATP_FOM_RET_WRITE_LEN_ERR,
    ATP_FOM_RET_ERASE_LEN_ERR,
    ATP_FOM_RET_READ_LEN_ERR,
    ATP_FOM_RET_WRITE_NULL_ERR,
    ATP_FOM_RET_ERASE_NULL_ERR,
    ATP_FOM_RET_READ_NULL_ERR,
    ATP_FOM_RET_TYPE_ERR,
    ATP_FOM_RET_NULL_ERR,
    //ATP_FOM_RET_ERASE_LEN_ERR,
    //ATP_FOM_RET_READ_LEN_ERR,
    ATP_FOM_RET_OTHER_ERR
} ATP_FOM_RET_CODE_EN;

enum ATP_BHAL_KEYID_EN
{
    ATP_BHAL_KEY_EQUIP_PARAM = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_BHALAPI), /*!< cfg file key */
};


VOS_VOID FOM_TEST();

VOS_INT32 ATP_FLASH_GetAreaSize(FOM_E_FLASH_AREA enFlashArea, VOS_UINT32 *pulSize);
VOS_INT32 ATP_FLASH_GetAreaOffset(FOM_E_FLASH_AREA enFlashArea, VOS_UINT32 *pulOffset);

VOS_INT32 ATP_FLASH_ReadArea(FOM_E_FLASH_AREA enFlashArea,VOS_INT8 *pcDstBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
VOS_INT32 ATP_FLASH_WriteArea(FOM_E_FLASH_AREA enFlashArea,VOS_INT8 *pcSrcBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
VOS_INT32 ATP_FLASH_Write(VOS_INT8 *pcSrcBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
VOS_INT32 ATP_FLASH_Erase (VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
VOS_INT32 ATP_FLASH_Read (VOS_INT8 *pcDstBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);

VOS_INT32 ATP_NET_SetBaseMacAddress(VOS_INT8 *pcBaseAddr);
VOS_INT32 ATP_NET_GetBaseMacAddress(VOS_INT8 *pcBaseAddr);
VOS_INT32 ATP_NET_GetMacAddress(VOS_INT8 *pcMacAddr, ATP_BHAL_MAC_TYPE_EN   enMacType);
VOS_INT32 ATP_NET_ReleaseMacAddress(VOS_INT8 *pcMacAddr);
VOS_INT32 ATP_NET_InitMacAddress(VOS_UINT32 max_mac_num, VOS_UINT32 max_wan_mac_num,
		VOS_UINT32 max_wlan_mac_num, VOS_UINT32 wlan_mac_align_size);
VOS_INT32 ATP_NET_InitMac_WS(ATP_BHAL_WS_MODE enMode);

VOS_INT32 ATP_SYS_SetInfo(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
VOS_INT32 ATP_SYS_GetInfo(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);

VOS_UINT32 ATP_SYS_GetInfoEx(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
	
VOS_INT32 ATP_IS_UPGRADING(VOS_BOOL bIsUpg);

VOS_UINT32 ATP_SYS_GetInfoSHA256(BSP_E_PARAM_TYPE eParamType, VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);

//#define forcms
/*flash数据区的偏移量宏*/

VOS_INT32 ATP_SYS_SetDectCalibPara(ATP_DECT_CALIB_TYPE_EN enParamType,
                                   VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
VOS_INT32 ATP_SYS_GetDectCalibPara(ATP_DECT_CALIB_TYPE_EN enParamType,
                                    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
VOS_UINT32 BSP_SYS_GetSWVersion(BSP_E_SW_TYPE enVerType,VOS_INT8 *pcDstBuf, VOS_UINT32 ulLen);
VOS_VOID BSP_SYS_SoftReboot(VOS_VOID);
VOS_VOID BSP_SYS_SoftRebootEx(VOS_VOID);
VOS_VOID BSP_SYS_SoftRebootExRestore(VOS_BOOL bIsRestore);

VOS_VOID BSP_SYS_SoftRebootDirect(VOS_VOID);

BSP_E_KEY_STATE BSP_INPUT_GetKeyState(VOS_UINT32 ulKeyCode);

VOS_INT32 ATP_SYS_GetSdramSize(VOS_UINT32 *pulSize);

/* 系统启动标志信息: */
#define UGPINFO_SIZE                   16
#define UPGINFO_START_OFFSET           0
                      
#define TR069_INFO_OFFSET              (UGPINFO_SIZE)
#define TR069_INFO_SIZE                (2*1024)

#define PIN_INFO_OFFSET                (TR069_INFO_OFFSET+TR069_INFO_SIZE)
#define PIN_INFO_SIZE                  (256)

#define BOOTFROM_INFO_OFFSET           (PIN_INFO_OFFSET+PIN_INFO_SIZE)
#define BOOTFROM_INFO_SIZE             16

#define SLAVE_DIFFADDR_OFFSET          (BOOTFROM_INFO_OFFSET+BOOTFROM_INFO_SIZE)
#define SLAVE_DIFFADDR_SIZE            (12)


/******************************PIN  处理*********************************/
#ifdef SUPPORT_ATP_PIN_MNG
#define ATP_PIN_SIMID_LEN_DATA      (20)
#define ATP_PIN_SIMID_LEN_PAD       (4)
#define ATP_PIN_SIMID_LEN           (ATP_PIN_SIMID_LEN_DATA + ATP_PIN_SIMID_LEN_PAD)
#define ATP_PIN_CODE_LEN_D          (12)
#define ATP_PIN_SIMID_LEN_D               (20)
#define SIM_AUTO_CHECK                     1
#define SIN_NAUTO_CHECK                    0

#ifdef SUPPORT_ATP_MULTI_PIN_MNG

#define ATP_DATA_CARD_PIN_NUM 3
#define Flash_Block_Num 3

/*数据卡类型*/
#define ComDatacard 2 //普通数据卡
#define IccidDatacard 1 //可以查询sim卡ID的数据卡
#define NoDatacard 0

#endif

#define PININFO_BUF_LENGTH_D       (512)
#define BHAL_AT_CMD_LENGHTH                    256
#define SIM_STA_LEG                       128


#define SIM_STA_RET                       "/var/pinres.txt"
#define BHL_COM_LENGHTH                    256

#ifdef SUPPORT_ATP_MULTI_PIN_MNG
#define SIM_ICCID_RET "/var/simICCID.txt"
#endif

#ifdef SUPPORT_ATP_MULTI_PIN_MNG
typedef struct tagATP_PIN_INFO
{
    VOS_UINT8           aucSimID[ATP_PIN_SIMID_LEN];// 需要自己添加\0结束符ICCID
    VOS_UINT8           aucPIN[ATP_PIN_CODE_LEN_D];     // 需要自己添加\0结束符
    VOS_UINT8           ucAutoCheck;
    VOS_UINT8           inUseFlag;
} ATP_PIN_INFO;

typedef struct tagATP_PIN_FlASH_BLOCK
{
    VOS_UINT8           aucIMEI[ATP_PIN_SIMID_LEN];  // 需要自己添加\0结束符IMEI
    ATP_PIN_INFO        st_MultiPIN[ATP_DATA_CARD_PIN_NUM];
    VOS_UINT8            inDatacardKinds; //数据卡类型:可以查询iccid的还是不能查询
    VOS_UINT8            inDataCardUseFlag;
}ATP_PIN_FlASH_BLOCK;

typedef struct tagATP_PIN_MNG_INFO
{
    ATP_PIN_FlASH_BLOCK         st_MultiPINFlashBlock[Flash_Block_Num];
} ATP_PIN_MNG_INFO;

#else
typedef struct tagATP_PIN_MNG_INFO
{
    VOS_UINT8           aucSimId[ATP_PIN_SIMID_LEN];  // 需要自己添加\0结束符
    VOS_UINT8           aucPIN[ATP_PIN_CODE_LEN_D];     // 需要自己添加\0结束符
    VOS_UINT8           ucAutoCheck;
} ATP_PIN_MNG_INFO;
#endif

/*start: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
VOS_INT32  ATP_SYS_SetConsole_Type(VOS_UINT32 uiMode);
#endif
/*end: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/

VOS_INT32 ATP_BHAL_GetPinInfo(ATP_PIN_MNG_INFO *pstPinInfo);

VOS_INT32 ATP_BHAL_SetPinInfo(ATP_PIN_MNG_INFO *pstPinInfo);

VOS_INT32 ATP_BHAL_DestroyPinInfo();
int ATP_VoiceGetPinStatus();

#ifdef SUPPORT_ATP_MULTI_PIN_MNG
VOS_UINT32 ATP_SYS_SavePinInfo(const VOS_CHAR *pcPin);
#endif

VOS_UINT32 ATP_SYS_ValidatePinInfoSave(const VOS_CHAR *pcPin);
#endif
/******************************PIN  处理*********************************/
VOS_INT32 ATP_CFG_ReadFile(FOM_E_FLASH_AREA eCfgArea, VOS_INT8 *pcDstBuf, VOS_UINT32	ulLen);
VOS_INT32 ATP_CFG_WriteFile(FOM_E_FLASH_AREA eCfgArea, VOS_INT8 *pcSrcBuf, VOS_UINT32	ulLen);
VOS_INT32 ATP_CFG_ClearFile(FOM_E_FLASH_AREA eCfgArea);
VOS_INT32 ATP_CFG_GetFileSize(FOM_E_FLASH_AREA eCfgArea, VOS_UINT32 *pulSize);


/******************************LED  处理*********************************/
VOS_INT32 ATP_AllLED_Blink(int Color);
VOS_INT32 ATP_BHAL_SetLedState(BSP_E_LED_NAME enLedName,BSP_E_LED_STATE enLedState);
VOS_INT32 ATP_EQUIP_AllGreenLED_On();
VOS_INT32 ATP_EQUIP_AllRedLED_On();
VOS_INT32 ATP_EQUIP_AllYellowLED_On();
VOS_INT32 ATP_EQUIP_AllLED_On();
VOS_INT32 ATP_EQUIP_AllGreenLED_Off();
VOS_INT32 ATP_EQUIP_AllRedLED_Off();
VOS_INT32 ATP_EQUIP_AllYellowLED_Off();
VOS_INT32 ATP_EQUIP_AllLED_Off();
VOS_INT32 ATP_EQUIP_AllLED_Test_On();
VOS_INT32 ATP_EQUIP_AllLED_Test_Off();
//一个time是k125ms// timer expires in ~100ms
VOS_INT32 ATP_EQUIP_TimeLEDOn_SET(int color,unsigned int time);
VOS_INT32 ATP_BHAL_SetLedState(BSP_E_LED_NAME enLedName,BSP_E_LED_STATE enLedState);
VOS_INT32 ATP_EQUIP_ScanKeyMode(ATP_BHAL_KEYSCAN_MODE_EN enScanMode);
BSP_E_KEY_STATE ATP_BHAL_GetKeyState(ATP_BHAL_KEY_EN enKeyName);
VOS_INT32 ATP_EQUIP_Key_TEST();
/*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
VOS_INT32 ATP_BHAL_GpioI2cWrite(VOS_UINT8 *pReadBuf, VOS_UINT32 uLen);
VOS_INT32 ATP_BHAL_GpioI2cRead(VOS_UINT8 *pReadBuf, VOS_UINT32 uLen);
VOS_INT32 ATP_BHAL_GpioAtmelI2cRead(VOS_UINT8 cmd, VOS_UINT8 *pReadBuf, VOS_UINT32 uLen);
VOS_INT32 ATP_BHAL_GpioAtmelI2cWrite(VOS_UINT8 cmd, VOS_UINT8 *pReadBuf, VOS_UINT32 uLen);
VOS_INT32 ATP_BHAL_GpioI2cUpg(VOS_UINT8 *pUpgBuf, VOS_UINT32 uLen);
VOS_INT32 ATP_BHAL_GetI2cState();
/* START MODIFY by y00181549 for DTS2013101803442 at 20140108*/
VOS_INT32 ATP_BHAL_GpioI2cInit();
VOS_INT32 ATP_BHAL_GpioI2cCheckRev();
/* END MODIFY by y00181549 for DTS2013101803442 at 20140108*/
#endif
/*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
VOS_INT32 ATP_EQUIP_GetChipParam(ATP_BHAL_CHIP_PARAM_EN enChipParamType,VOS_UINT32 ulSize);
VOS_INT32 ATP_SET_LED(VOS_CHAR *pDecvName,BSP_E_LED_NAME enLedName);
VOS_INT32 ATP_DEL_LED( VOS_CHAR *pDecvName ,BSP_E_LED_NAME enLedName);
VOS_INT32 ATP_DEL_ALL_LED( VOS_CHAR *pDecvName ,BSP_E_LED_NAME enLedName);
#ifdef LED_SEQUENCE_BLINK
VOS_INT32 ATP_BHAL_SquenceBlinkLed(unsigned int time);
#endif

#ifdef SUPPORT_ATP_WLAN
typedef struct tagATP_WLAN_TIME_INFO
{
    VOS_UINT8           ucTimeYear;  
    VOS_UINT8           ucTimeDay; 
    VOS_UINT8           ucKeepDis;  
} ATP_WLAN_TIME_INFO;

VOS_INT32 ATP_GetWlanTimeInfo(ATP_WLAN_TIME_INFO *pstTimeInfo);
VOS_INT32 ATP_SetWlanTimeInfo(ATP_WLAN_TIME_INFO *pstTimeInfo);
VOS_INT32 ATP_GetWlanRF(VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
VOS_INT32 ATP_SetWlanRF(VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
VOS_INT32 ATP_BHAL_Set_ServLed_TolTimes(VOS_UINT32 uTimes);
VOS_INT32 ATP_BHAL_Get_ServLed_RemTimes(unsigned int *pRemainTimes);
VOS_INT32 ATP_BHAL_GetSpecLedState(BSP_E_LED_NAME enLedName);
#endif

#ifdef SUPPORT_ATP_VOICE_DT
VOS_INT32 ATP_SYS_SetUrgentCallStatus(VOS_UINT32 ulStatus);
VOS_INT32 ATP_SYS_SetDectReset(VOS_VOID);

#endif
#ifdef SUPPORT_ATP_NANDFLASH  
VOS_INT32 ATP_SYS_Concifg_Erase();
#endif

#ifdef SUPPORT_ATP_WLAN_DT
VOS_INT32 ATP_BHAL_SetWpsProcStatus(VOS_VOID);
VOS_INT32 ATP_BHAL_GetWpsProcStatus(VOS_VOID);
VOS_INT32 ATP_BHAL_ClrWpsProcStatus(VOS_VOID);
/* START ADD by y00181549 for DTS2013101803442 at 20131129*/
VOS_INT32 ATP_BHAL_SetWlanSettingStatus(VOS_UINT32 ulStatus);
/* END ADD by y00181549 for DTS2013101803442 at 20131129*/
#endif

#ifdef SUPPORT_ATP_HYBRID
VOS_INT32 ATP_SYS_SetLteReset(VOS_VOID);
#endif

VOS_INT32 ATP_SYS_Set_Cms_Inited(VOS_VOID);

#if defined(__cplusplus)
}
#endif
#endif /* _ATPFLASHAPI_H_ */
