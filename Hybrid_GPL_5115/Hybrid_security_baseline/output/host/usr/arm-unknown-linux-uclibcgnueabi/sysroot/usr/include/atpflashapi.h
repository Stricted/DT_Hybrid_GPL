/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : bhalapi.h
  ��    ��  : l69021
  ��    ��  : 1.0
  ��������  : 2007-6-30
  ��    ��  :
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-6-30
     ��    ��   : l69021
     �޸�����   : ��ɳ���

*********************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<�޸���>      <ʱ��>        <���ⵥ��>             <��Ҫ����>
l00205975     2012/12/11    DTS2012121101724        ��WEB��[B890]V100R001C994B116(router):WEB�汾�������
z00159386     2012/10/16    DTS2012102201180        TELUS�������󣬰���Voicemail�����ο��ء�SMS�ض���
z81004143     2013/01/12    DTS2013011106168        [Failover][B890-66 Telus] V100R001C259B118(Router)&V100R001C259B114SP01(AMSS): �̶��������ȣ����崦���ƶ����У��޸�Information���ú󣬹̶�����ҵ��ͨ
========================================================================================*/
#ifndef _ATPFLASHAPI_H_
#define _ATPFLASHAPI_H_

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

/*log����*/
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
VOS_UINT32 ATP_FLASH_UpgradeBootloader(VOS_UINT32 uloffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
VOS_UINT32 ATP_FLASH_UpgradeCfg(VOS_UINT32 uloffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
VOS_UINT32 ATP_FLASH_UpgradeTag(VOS_UINT32 ulmainoffset,VOS_UINT32 ulslaveoffset,
	VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
VOS_UINT32 ATP_FLASH_UpgradeDect(VOS_UINT32 ulmainoffset,VOS_UINT8 *pcImage, VOS_UINT32 ulLen);
UPG_GRADE_EN UPG_GetRunCurrentSystem( VOS_VOID );
/* <DTS2012121101724 l00205975 20121211 begin */   
/******************************************************************************
  ��������  : VOS_UINT32 ATP_FLASH_UpgradeWeb
  ��������  : ����WEB��Ӧ��FLASH����
  �������  :
              1. ulmainoffset  :   ������ƫ��
              2. ulslaveoffset  :  ������ƫ��
              3. pcImage  :   ��������
              4. ulLen  :   �������ݴ�С
              5. ulmainearseLen : ������������С
              5. ulslaveearseLen : ������������С
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK(�����ɹ��� VOS_NOK(����ʧ�ܣ�

  �޸���ʷ      :
   1.��    ��   : 2012-12-11
     ��    ��   : l00205975
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_FLASH_UpgradeWeb(VOS_UINT32 ulmainoffset,VOS_UINT32 ulslaveoffset,
	VOS_UINT8 *pcImage, VOS_UINT32 ulLen,VOS_UINT32 ulmainearseLen,VOS_UINT32 ulslaveearseLen);
/* DTS2012121101724 l00205975 20121211 end> */

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
     ENUM_END
}BSP_E_SW_TYPE;


/* Flash���� */
typedef enum 
{
    FOM_E_FLASH_AREA_BOOTLOADER = 0,         /* bootloader���� */
    FOM_E_FLASH_AREA_MAIN_TAG,            /* ��ϵͳTag */
    FOM_E_FLASH_AREA_SLAVE_TAG,                /* ��ϵͳTag */ 
    FOM_E_FLASH_AREA_MAIN_KERNEL,            /* �ں˷��� */
    FOM_E_FLASH_AREA_MAIN_FS,                /* �ļ�ϵͳ���� */
    FOM_E_FLASH_AREA_SLAVE_KERNEL,            /* �ں˷��� */
    FOM_E_FLASH_AREA_SLAVE_FS,                /* �ļ�ϵͳ���� */
/* <DTS2012080807214 g68080 2012-08-08 mod begin */
    FOM_E_FLASH_AREA_MAIN_WEB_FS, 
    FOM_E_FLASH_AREA_SLAVE_WEB_FS, 
    FOM_E_FLASH_AREA_FOTA_FS, 
/* <DTS2012080807214 g68080 2012-08-08 mod end */
    FOM_E_FLASH_AREA_MIDDLEWARE,
    FOM_E_FLASH_AREA_MODULE,
    
    FOM_E_FLASH_AREA_RESERVED,              /* �������÷��� */
    
    FOM_E_FLASH_AREA_CURRENT_CONFIG,     /* ��ǰ�����ļ����� */
    FOM_E_FLASH_AREA_CUSTOMIZE_CONFIG,   /* ��Ӫ�������ļ����� */    
    FOM_E_FLASH_AREA_FACTORY_CONFIG,     /* ���������ļ����� */
    FOM_E_FLASH_AREA_TEMP_PARAM,                /* �����ɱ��������: �������������ʶ�Լ�TR069�������� */
    FOM_E_FLASH_AREA_TR69_CERT,               /* TR069֤�� */
    FOM_E_FLASH_AREA_LOG,                             /* ��־�������� */    
    FOM_E_FLASH_AREA_CRASH_DUMP_LOG,                             /* ��־�������� */ 
    FOM_E_FLASH_AREA_PPP_SESSION,                 /*PPP�ỰID��MAC��ַ��Ϣ*/
    FOM_E_FLASH_AREA_DECT_PHONEBOOK,          /* DECT PHONEBOOT����*/
    FOM_E_FLASH_AREA_DECT_CALIBRATE,          /* dectУ׼����*/    
    FOM_E_FLASH_AREA_EQUIP_PARAM,            /* MAC,SN�������� */    
/* <DTS2012080807214 g68080 2012-08-08 mod begin */
    FOM_E_FLASH_AREA_WLANRF_PARAM,
    FOM_E_FLASH_AREA_UPG_FLAG,          /* UPG������ʶ*/        
    FOM_E_FLASH_AREA_BBT,
/* <DTS2012080807214 g68080 2012-08-08 mod end */
    FOM_E_FLASH_AREA_ALLFLASH,                /* ����Flash���� */  
    
    FOM_E_FLASH_AREA_END                      /* ������ */	
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


VOS_INT32 ATP_SYS_SetInfo(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);
VOS_INT32 ATP_SYS_GetInfo(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);

VOS_UINT32 ATP_SYS_GetInfoEx(BSP_E_PARAM_TYPE eParamType,
    VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);

VOS_UINT32 ATP_SYS_GetInfoSHA256(BSP_E_PARAM_TYPE eParamType, VOS_INT8 *pcParam, VOS_UINT32 ulParamLen);

//#define forcms
/*flash��������ƫ������*/

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

/* ϵͳ������־��Ϣ: */
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

/* <DTS2012102201180  z00159386  20121011  begin */
#ifdef SUPPORT_ATP_TELUS
#define ROAM_INFO_OFFSET               (SLAVE_DIFFADDR_OFFSET+SLAVE_DIFFADDR_SIZE)
#define ROAM_INFO_SIZE                 (16)
#define ROAM_ENABLE                     1
#define ROAM_DISABLE                    0
#endif /* SUPPORT_ATP_TELUS */
/* DTS2012102201180  z00159386  20121011  end> */

/******************************PIN  ����*********************************/
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

/*���ݿ�����*/
#define ComDatacard 2 //��ͨ���ݿ�
#define IccidDatacard 1 //���Բ�ѯsim��ID�����ݿ�
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
    VOS_UINT8           aucSimID[ATP_PIN_SIMID_LEN];// ��Ҫ�Լ����\0������ICCID
    VOS_UINT8           aucPIN[ATP_PIN_CODE_LEN_D];     // ��Ҫ�Լ����\0������
    VOS_UINT8           ucAutoCheck;
    VOS_UINT8           inUseFlag;
} ATP_PIN_INFO;

typedef struct tagATP_PIN_FlASH_BLOCK
{
    VOS_UINT8           aucIMEI[ATP_PIN_SIMID_LEN];  // ��Ҫ�Լ����\0������IMEI
    ATP_PIN_INFO        st_MultiPIN[ATP_DATA_CARD_PIN_NUM];
    VOS_UINT8            inDatacardKinds; //���ݿ�����:���Բ�ѯiccid�Ļ��ǲ��ܲ�ѯ
    VOS_UINT8            inDataCardUseFlag;
}ATP_PIN_FlASH_BLOCK;

typedef struct tagATP_PIN_MNG_INFO
{
    ATP_PIN_FlASH_BLOCK         st_MultiPINFlashBlock[Flash_Block_Num];
} ATP_PIN_MNG_INFO;

#else
typedef struct tagATP_PIN_MNG_INFO
{
    VOS_UINT8           aucSimId[ATP_PIN_SIMID_LEN];  // ��Ҫ�Լ����\0������
    VOS_UINT8           aucPIN[ATP_PIN_CODE_LEN_D];     // ��Ҫ�Լ����\0������
    VOS_UINT8           ucAutoCheck;
} ATP_PIN_MNG_INFO;
#endif
/* <DTS2012102201180  z00159386  20121011  begin */
#ifdef SUPPORT_ATP_TELUS
#define DEFAULT_ROAM_VAL     0 /* DTS2013011106168 z81004143 2013/1/12 */
typedef struct tagATP_ROAM_INFO
{
    VOS_UINT8      is_roam_enable;     
} ATP_ROAM_INFO;
#endif /* SUPPORT_ATP_TELUS */
/* DTS2012102201180  z00159386  20121011  end> */
/*start: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
VOS_INT32  ATP_SYS_SetConsole_Type(VOS_UINT32 uiMode);
#endif
/*end: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/

VOS_INT32 ATP_BHAL_GetPinInfo(ATP_PIN_MNG_INFO *pstPinInfo);

VOS_INT32 ATP_BHAL_SetPinInfo(ATP_PIN_MNG_INFO *pstPinInfo);

VOS_INT32 ATP_BHAL_DestroyPinInfo();
int ATP_VoiceGetPinStatus();
/* <DTS2012102201180  z00159386  20121019  begin */
#ifdef SUPPORT_ATP_TELUS
VOS_INT32 ATP_BHAL_SetRoamInfo(ATP_ROAM_INFO *pstRoamInfo);
VOS_INT32 ATP_BHAL_GetRoamInfo(ATP_ROAM_INFO *pstRoamInfo);
#endif /* SUPPORT_ATP_TELUS */
/* DTS2012102201180  z00159386  20121019  end> */
#ifdef SUPPORT_ATP_MULTI_PIN_MNG
VOS_UINT32 ATP_SYS_SavePinInfo(const VOS_CHAR *pcPin);
#endif

VOS_UINT32 ATP_SYS_ValidatePinInfoSave(const VOS_CHAR *pcPin);
#endif
/******************************PIN  ����*********************************/
VOS_INT32 ATP_CFG_ReadFile(FOM_E_FLASH_AREA eCfgArea, VOS_INT8 *pcDstBuf, VOS_UINT32	ulLen);
VOS_INT32 ATP_CFG_WriteFile(FOM_E_FLASH_AREA eCfgArea, VOS_INT8 *pcSrcBuf, VOS_UINT32	ulLen);
VOS_INT32 ATP_CFG_ClearFile(FOM_E_FLASH_AREA eCfgArea);
VOS_INT32 ATP_CFG_GetFileSize(FOM_E_FLASH_AREA eCfgArea, VOS_UINT32 *pulSize);


/******************************LED  ����*********************************/
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
//һ��time��k125ms// timer expires in ~100ms
VOS_INT32 ATP_EQUIP_TimeLEDOn_SET(int color,unsigned int time);
VOS_INT32 ATP_BHAL_SetLedState(BSP_E_LED_NAME enLedName,BSP_E_LED_STATE enLedState);
VOS_INT32 ATP_EQUIP_ScanKeyMode(ATP_BHAL_KEYSCAN_MODE_EN enScanMode);
BSP_E_KEY_STATE ATP_BHAL_GetKeyState(ATP_BHAL_KEY_EN enKeyName);
VOS_INT32 ATP_EQUIP_Key_TEST();

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
#endif

/* <DTS2013083001384 xiazhongyue 20130830 begin */
#ifdef SUPPORT_ATP_CUSTOM_FMC_PRODUCT
VOS_VOID  ATP_SYS_ResetAmss();
VOS_VOID  ATP_SYS_EnterFastBoot();
VOS_VOID  ATP_SYS_RouterShutDown();
#endif
/*  DTS2013083001384 xiazhongyue 20130830 end > */
#if defined(__cplusplus)
}
#endif
#endif /* _ATPFLASHAPI_H_ */
