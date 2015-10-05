/**\file atsinit.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-1-6
*/
#ifndef __ATSINIT_H
#define __ATSINIT_H

//!数据卡初始化状态
#define ATS_DATACARD_INIT_START 0
#define ATS_DATACARD_INIT_OVER 1

//!打开pcui的尝试次数
#define WM_START_TIME         3

#define ATS_INIT_TIME        5  /* <DTS2013083001384 xiazhongyue 20130830 modify> */

#define HSPA_OK    0

#define ATS_REP_CMD                "REP_CMD"

//!确定产品信息
#define  ATS_HSPA_VENDOR_PROC                           "/proc/UsbModem"
#define  ATS_HSPA_VENDOR_STR                             "Vendor"
#define  ATS_HSPA_PRODUCT_VENDOR_HW             "Huawei"
#define  ATS_HSPA_PRODUCT_VENDOR_BANDLUXE   "Bandluxe"
#define  ATS_HSPA_PRODUCT_VENDOR_ZTE             "ZTE"
/* <DTS2013083001384 xiazhongyue 20130830 begin */

#define ATS_HSPA_HSPA_KIND_TD			"TD-SCDMA"
#define ATS_HSPA_HSPA_KIND_WCDMA	"WCDMA"
#define ATS_HSPA_HSPA_KIND_CDMA		"CDMA"
#define ATS_HSPA_HSPA_KIND_WCDMA_CDMA	"WCDMA&CDMA"

#define UNKNOW_KIND 0x00000001;
#define HUAWEI_FACTORY 0x00000002;
#define ZTE_FACTORY 0x00000004;
#define BANDLUXE_FACTORY 0x00000008;
#define TD_KIND 0x00000010;
/*  DTS2013083001384 xiazhongyue 20130830 end > */


#define   MAX_MATCH_SIZE          100     /* max size of at command match str */
#define   MAX_ATCMD_REPORT_SIZE   512     /* max size of at command report str */
#define   MAX_ATCMD_SIZE          512     /* max size of at command */

#define   ATS_READ_SIZE     512  

//!消息超时
#define ATS_CMD_BASE_TIMEOUT    30

typedef enum{    
    ATS_HSPA_VENDOR_HW = 0,
    ATS_HSPA_VENDOR_BANDLUXE,
    ATS_HSPA_VENDOR_ZTE,
    AT_HSPA_VENDOR_MAX
   }ATS_HSPA_VENDOR_EN;

#define ATS_INTFACENAME_LEN         32

#define USBSERVER_SIMDOWN     0
#define USBSERVER_SIMUP          1

#define USBSERVER_DATACARDDOWN      0
#define USBSERVER_DATACARDUP            1

#define AT_MODULE 1

/* <DTS2013083001384 xiazhongyue 20130830 begin */
#define HEX_NUM_LENGHT 20
#define LONG_UP	16

#define DATA_KILOMEGA (1<<30)
#define DATA_MILLION	(1<<20)
#define DATA_KILO		(1024)

/*  DTS2013083001384 xiazhongyue 20130830 end > */
typedef enum{    
    ATS_CMD_COPS_NUM_NAME = 0,
    ATS_CMD_GET_SCID,
    ATS_CMD_GET_IMSI,
    ATS_CMD_AUTO_VALIDATE_PIN,
    ATS_CMD_GET_OPL,
    ATS_CMD_GET_PNN
}ATS_ONCE_CMD_FLAG_POS;
/* <DTS2013083001384 xiazhongyue 20130830 begin */

typedef struct stCardDataFlowInfo
{
	unsigned long int ulCurrDsTime;
	unsigned long int ulTxRate;
	unsigned long int ulRxRate;
	unsigned long int ulQosTxRate;
	unsigned long int ulQosRxRate;
	unsigned long int ulTxDataH;
	unsigned long int ulTxDataL;
	unsigned long int ulRxDataH;
	unsigned long int ulRxDataL;
}ST_CARD_DATA_FLOW_INFO;


#define     UNKOWN         0
#define     WCDMA           1
#define     LTE_WCDMA                 2

/*华为数据卡相关信息(适配用)*/
typedef struct ATP_HW_DATACARD_INFO
{
    const VOS_CHAR acModel[16];                 //数据卡型号
    VOS_INT32 iMode;                  //数据卡支持的制式
}ATP_HW_DATACARD_INFO_ST;
/*  DTS2013083001384 xiazhongyue 20130830 end > */


#if defined(__cplusplus)
extern "C" {
#endif

VOS_INT32 AtsSetActivePin(VOS_CHAR *psz_save_pin);
VOS_INT32 AtsSendNormalCmdMsg(ATS_MSG_ST  *stMsg,VOS_INT32 *lRetCode);

VOS_INT32 AtsSmReportHandlerCsq (ATS_RCV_MSG_DATA_ST* rssi_res);
VOS_INT32 AtsCommonSetAtCmd(VOS_CHAR* at_cmd, const VOS_CHAR* at_match, ATS_RCV_MSG_DATA_ST* cmd_result);

#if 0
VOS_VOID AtsClosePCUI(VOS_VOID);
#endif

VOS_INT32 AtsOpenPCUI(VOS_VOID);

//!for covertity
#if 0
VOS_VOID AtsFreeResouce(VOS_VOID);
#endif

VOS_VOID AtsSetClip(VOS_VOID);

VOS_INT32 AtsCmee(VOS_INT32 *retCode);
VOS_INT32 AtsSetAte(VOS_INT32 *lRetCode);

VOS_VOID AtsQuerySysinfo(VOS_VOID);

VOS_INT32 AtsInitWm(VOS_INT32 *lRetCode);

VOS_INT32   AtsResInit(VOS_VOID);

VOS_UINT32 AtsWriteMsgToUsb(ATS_MSG_LIST_ST *pstItem);

VOS_VOID AtsMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_INT32 AtsSimCardNeedValidate(VOS_BOOL* b_is_check_pin);
VOS_VOID AtsTimerProc(VOS_VOID *argv);

VOS_VOID AtsDatacardProc(VOS_VOID);

#ifdef SUPPORT_ATP_3G_FIRST
VOS_INT32 AtsSet3GModle(VOS_INT32 *lRetCode);
#endif

#ifdef SUPPORT_ATP_PIN_MNG    
#ifdef SUPPORT_ATP_MULTI_PIN_MNG
VOS_INT32 AtsActiveMultipinHandle(VOS_INT32 *lRetCode, ATS_MSG_ST *stMsg);

VOS_INT32 AtsActiveSinglepinHandle(VOS_INT32 *lRetCode);

VOS_INT32 AtsPinHandle(VOS_INT32 *lRetCode);
#else

VOS_INT32 AtsActivePinHandle(VOS_INT32 *lRetCode);

#endif
#endif

VOS_INT32 AtsCheckDialmodeByModel(VOS_INT32 *lRetCode);

#ifdef SUPPORT_ATP_LTE_FIRST
VOS_INT32 AtsSetLteModle(VOS_INT32 *lRetCode);
#endif

VOS_VOID AtsResetDatastatics(VOS_VOID);
VOS_VOID AtsSetDatastatics(VOS_CHAR* pChar);
VOS_INT32 AtsParsePinEx(ATS_RCV_MSG_DATA_ST* cmd_report, PinMgResultCodeEx* Result, VOS_INT32 CmdType);
VOS_UINT32 AtsVoiceAndAtcmdReqProc(const VOS_CHAR *pszAppName, ATS_MSG_ST *stMsg);

VOS_VOID AtsClosePCUI(VOS_VOID);
/*  DTS2013083001384 xiazhongyue 20130830 end > */
#if defined(__cplusplus)
}
#endif

#endif
