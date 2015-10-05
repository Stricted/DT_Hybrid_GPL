/**\file at_lib.h
  *AT lib头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-1-2
*/
#ifndef __AT_LIB_H__
#define __AT_LIB_H__
#include "atptypes.h"
/* <DTS2013083001384 xiazhongyue 20130830 begin */
#include <sys/time.h>
#include <signal.h>
#ifdef SUPPORT_ATP_TRACE_CODE_DEBUG
#define ATS_DEBUG(code, format, args...)   printf("ATSERVER: file[%s] function[%s] line[%d] cd[0x%x] " format "\r\n", __FILE__, __FUNCTION__,__LINE__, code, ## args)
#define ATSExecCmd(acCmd) {printf("ATSERVER: file[%s] line[%d] %s \r\n", __FILE__, __LINE__, acCmd); ATP_Exec(acCmd);}
#else
#define ATS_DEBUG(code, format, args...)
#define ATSExecCmd ATP_Exec
#endif
/*  DTS2013083001384 xiazhongyue 20130830 end > */

#define   charGreateThan        ">"
#define   BASE_REPORT_SEQ           1000
#define   CMIT_SIZE_MAX                50
#define   MAX_ATCMD_SIZE          512     /* max size of at command */
#define   MAX_MATCH_SIZE          100     /* max size of at command match str */
#define   MAX_ATCMD_REPORT_SIZE   512     /* max size of at command report str */
#define   BASE_TIMEOUT                50
#define   AT_TABLE_MAX                  -1


#define   ATS_QUEUE_REC_KEY     860
#define   ATS_QUEUE_SEND_KEY    861

/* 消息队列相关变量 */
extern int rcv_mq_id  ;                                 /* ATS接收消息队列标识符 */
extern int snd_mq_id ;                                 /* ATS发送消息队列标识符 */

/* request main type */
typedef enum{
    AT_REQ,                 /*普通的at cmd请求*/
    ACTIVE_REPORT,          /*激活主动上报*/
    GET_REPORT_REQ,         /*client 获取主动上报消息*/
    AT_VIR_QEQ,             /*伪命令请求*/
    CMD_REQ                 /*命令请求*/
}at_req_type;

/* request sub type */
typedef enum{
    PPP_STATUS
}at_req_sub_type;


/*AT_lib.c文件中部分接口的返回值*/
typedef  enum 
{
    ATS_ERROR = -1,
    ATS_SUCCESS
}ats_result;

/*msg返回类型*/
typedef enum 
{

    DIRECT_BACK,    /*直接返回*/
    FILE_BACK,    /*文件形式返回*/
    BACK_MAX
}at_rsp_type;

typedef enum
{
    PPP_MSG_ID  =1, 
    WEB_MSG_ID ,
    SMS_MSG_ID,
    ATCMD_MSG_ID,
    //ATS_MSG_ID,
    ATC_SMS_ID,
    ATC_CS_ID,
    ATC_CSAPP_ID,
    CLIENT_ID_MAX
} MSG_CLIENT_TYPE_EX;

typedef enum 
{
    AT_REPORT_REQ_NONE = -1,
    AT_REPORT_CMTI_REQ,     
    AT_REPORT_CMT_REQ,    
    AT_REPORT_RSSI_REQ,    
    AT_REPORT_SMSFULL_REQ,
    AT_REPORT_CLIP_REQ,
    AT_REPORT_SRVST_REQ,
    AT_REPORT_SIMST_REQ,
    AT_REPORT_MODE_REQ,
    AT_REPORT_DSFLOWRPT_REQ,
    AT_REPORT_SYSINFO_REQ,          /*at^sysinfo*/
    AT_REPORT_CGMR_REQ,             /*软件版本号*/
    AT_REPORT_HWVER_REQ,             /*硬件版本号*/
    AT_REPORT_PPP_STATE_REQ,        /*pppd中的IP地址和状态*/
    AT_REPORT_COPS_REQ,
/* Optus SMS: liudan 2008-1-15 ADD BEGIN*/
    AT_REPORT_CMGI0_REQ,
/* Optus SMS: liudan 2008-1-15 ADD END*/
	/* <DTS2013083001384 xiazhongyue 20130830 begin */
	AT_REPORT_CELLINFO_REQ,
    /* ^modetime */
    AT_REPORT_MODEMTIME_REQ,
	/*  DTS2013083001384 xiazhongyue 20130830 end > */ 
    AT_REPORT_REQ_MAX
}at_report_type;

/*是否是多条纪录,多条纪录将以文件形式返回*/
typedef enum 
{
    AT_RES_ONE,    
    AT_RES_MUT
}at_mutil_type;

/*AT命令执行结果*/    
typedef enum 
{
    AT_NONE = -1,
    AT_DEVICE_DOWN,
    AT_CMD_TIMEOUT,
    AT_CMD_OK,
    AT_CME_ERROR,
    AT_CMS_ERROR,
    AT_ERROR,
    AT_NO_CARRIER,
    AT_NO_ANSWER,
    AT_CMD_NOT_SUPPORT,
    AT_TOO_MANY_PARAMS,
    AT_GREATTHAN,
    AT_CMD_RES_MAX
}at_cmd_result;

/*主动上报的at命令结果*/
typedef enum
{
    AT_REPORT_NONE = -1,
    AT_REPORT_BOOT,
    AT_REPORT_CMTI,
    AT_REPORT_CMT,
    AT_REPORT_CDSI,
    AT_REPORT_SMSFULL,
    AT_REPORT_RING,
    AT_REPORT_CONNECT,
    AT_REPORT_CLIP,
    AT_REPORT_SRVST,
    AT_REPORT_SIMST,
    AT_REPORT_MODE,
    AT_REPORT_RSSI,
    AT_REPORT_DSFLOWRPT,
    AT_REPORT_CREG,
    AT_REPORT_CEND,
    AT_REPORT_CONN,
    AT_REPORT_CONF,    
    AT_REPORT_ORIG,    
    AT_REPORT_EARST,
    AT_REPORT_CCWA,
    AT_REPORT_CSSI,    
    AT_REPORT_CSSU,
    AT_REPORT_COLP,
    AT_REPORT_CUSD,
    AT_REPORT_MAX
}at_cmd_report;

/*<AP7D01380     chenkeyun    2008-02-26    mod        begin*/
typedef enum
{
  NO_NEED_MACTH = 0,
  NEED_MACTH
}need_macth_seq;
/*AP7D01380     chenkeyun    2008-02-26    mod        end >*/


/* 此短信的类型。0=接收到的未读短信；
1=接收到的已读短信 2=存储的未发短信
3=存储的已发短信       4 =表示所有状态的短信。
*/
typedef enum
{
  SMS_DELIVER_NOT_READ = 0,
  SMS_DELIVER_READ,
  SMS_SUBMIT_NOT_SEND,
  SMS_SUBMIT_SEND,
  SMS_STATE_ALL,
}sms_state;

typedef enum
{
  DSAT_CMS_NONE               = 0,
  DSAT_CMS_ME_FAILURE         = 300,
  DSAT_CMS_SERVICE_RESERVED   = 301,
  DSAT_CMS_OP_NOT_ALLOWED     = 302,
  DSAT_CMS_OP_NOT_SUPPORTED   = 303,
  DSAT_CMS_INVALID_PDU_PARAM  = 304,
  DSAT_CMS_INVALID_TXT_PARAM  = 305,
  DSAT_CMS_SIM_NOT_INSERTED   = 310,
  DSAT_CMS_SIM_PIN_REQ        = 311,
  DSAT_CMS_PHSIM_PIN_REQ      = 312,
  DSAT_CMS_SIM_FAILURE        = 313,
  DSAT_CMS_SIM_BUSY           = 314,
  DSAT_CMS_SIM_WRONG          = 315,
  DSAT_CMS_SIM_PUK_REQ        = 316,
  DSAT_CMS_SIM_PIN2_REQ       = 317,
  DSAT_CMS_SIM_PUK2_REQ       = 318,
  DSAT_CMS_MEM_FAILURE        = 320,
  DSAT_CMS_INVALID_INDEX      = 321,
  DSAT_CMS_MEM_FULL           = 322,
  DSAT_CMS_SCA_ADDR_UNKNOWN   = 330, 
  DSAT_CMS_NO_SERVICE         = 331,
  DSAT_CMS_NETWORK_TIMEOUT    = 332,
  DSAT_CMS_CNMA_NOT_EXP       = 340,
  DSAT_CMS_UNKNOWN_ERR        = 500,
  DSAT_CMS_MAX                = 512
} dsati_cms_err_e_type;


typedef enum
{
  DSAT_CME_NO_ERROR              =  -1,  /* RESERVED */
  DSAT_CME_PHONE_FAILURE         =   0,  /* phone failure */
  DSAT_CME_NO_CONNECTION         =   1,  /* no connection to phone */
  DSAT_CME_ADAPTOR_LINK_RESERVED =   2,  /* phone adaptor link reserved */
  DSAT_CME_OP_NOT_ALLOWED        =   3,  /* operation not allowed */
  DSAT_CME_OP_NOT_SUPPORTED      =   4,  /* operation not supported */
  DSAT_CME_PH_SIM_PIN_REQUIRED   =   5,  /* PH-SIM PIN required */
  DSAT_CME_PH_FSIM_PIN_REQUIRED  =   6,  /* PH-FSIM PIN required */
  DSAT_CME_PH_FSIM_PUK_REQUIRED  =   7,  /* PH-FSIM PUK required */
  DSAT_CME_SIM_NOT_INSERTED      =  10,  /* SIM not inserted */
  DSAT_CME_SIM_PIN_REQUIRED      =  11,  /* SIM PIN required */
  DSAT_CME_SIM_PUK_REQUIRED      =  12,  /* SIM PUK required */
  DSAT_CME_SIM_FAILURE           =  13,  /* SIM failure */
  DSAT_CME_SIM_BUSY              =  14,  /* SIM busy */
  DSAT_CME_SIM_WRONG             =  15,  /* SIM wrong */
  DSAT_CME_INCORRECT_PASSWORD    =  16,  /* incorrect password */
  DSAT_CME_SIM_PIN2_REQUIRED     =  17,  /* SIM PIN2 required */
  DSAT_CME_SIM_PUK2_REQUIRED     =  18,  /* SIM PUK2 required */
  DSAT_CME_MEMORY_FULL           =  20,  /* memory full */
  DSAT_CME_INVALID_INDEX         =  21,  /* invalid index */
  DSAT_CME_NOT_FOUND             =  22,  /* not found */
  DSAT_CME_MEMORY_FAILURE        =  23,  /* memory failure */
  DSAT_CME_TEXT_STRING_TOO_LONG  =  24,  /* text string too long */
  DSAT_CME_INVALID_CHAR_IN_TEXT  =  25,  /* invalid characters in text string */
  DSAT_CME_DIAL_STRING_TOO_LONG  =  26,  /* dial string too long */
  DSAT_CME_INVALID_CHAR_IN_DIAL_STRING
    =  27,  /* invalid characters in dial string */
  DSAT_CME_NO_NETWORK_SERVICE    =  30,  /* no network service */
  DSAT_CME_NETWORK_TIMEOUT       =  31,  /* network timeout */
  DSAT_CME_NETWORK_NOT_ALLOWED   =  32,  /* network not allowed - emergency 
                                             calls only */
  DSAT_CME_NET_PERSON_PIN_REQUIRED= 40,  /* network personalization PIN 
                                             required */
  DSAT_CME_NET_PERSON_PUK_REQUIRED= 41,  /* network personalization PUK 
                                             required */
  DSAT_CME_NET_SUBSET_PERSON_PIN_REQUIRED
                                  =  42,  /* network subset personalization 
                                             PIN required */
  DSAT_CME_NET_SUBSET_PERSON_PUK_REQUIRED
                                  =  43,  /* network subset personalization PUK
                                             required */
  DSAT_CME_SP_PERSON_PIN_REQUIRED=  44,  /* service provider personalization 
                                             PIN required */
  DSAT_CME_SP_PERSON_PUK_REQUIRED=  45,  /* service provider personalization 
                                             PUK required */
  DSAT_CME_CORP_PERSON_PIN_REQUIRED
                                  =  46,  /* corporate personalization PIN 
                                             required */
  DSAT_CME_CORP_PERSON_PUK_REQUIRED
                                  =  47,  /* corporate personalization PUK
                                             required */
  DSAT_CME_UNKNOWN               = 100,  /* unknown */
  DSAT_CME_ILLEGAL_MS            = 103,  /* Illegal Mem_Store */
  DSAT_CME_ILLEGAL_ME            = 106,  /* Illegal ME */
  DSAT_CME_GPRS_serv_not_allowed = 107,  /* GPRS services not allowed */
  DSAT_CME_PLMN_NOT_ALLOWED      = 111,  /* PLMN not allowed */
  DSAT_CME_LOCATION_NOT_ALLOWED  = 112,  /* Location area not allowed */
  DSAT_CME_ROAMING_NOT_ALLOWED   = 113,  /* Roaming not allowed in this 
                                             location area */
  DSAT_CME_SERV_OP_NOT_SUPPORTED = 132,  /* service option not supported */
  DSAT_CME_REQ_SERV_OP_NOT_SUBSCRIBED
                                  = 133,  /* requested service option not 
                                             subscribed */
  DSAT_CME_SERV_OP_TEMP_OUT_OF_ORDER
                                  = 134,  /* service option temporarily 
                                             out of order */
  DSAT_CME_unspecified_GPRS_error= 148,  /* unspecified GPRS error */
  DSAT_CME_PDP_AUTH_FAILURE      = 149,  /* PDP authentication failure */
  DSAT_CME_INVALID_MOBILE_CLASS  = 150,  /* invalid mobile class */
  
  /* Qualcomm specific error codes */
  DSAT_CME_SUPS_NETWORK_REJECTED = 257,  /* network rejected supserv request */
  DSAT_CME_SUPS_RETRY_OPERATION  = 258,  /* retry operation */
  DSAT_CME_SUPS_DFL_INVALID_NUMBER  = 259,  /* invalid deflected to number */
  DSAT_CME_SUPS_DFL_SELF_NUMBER  = 260,  /* deflected to own number */
  DSAT_CME_SUPS_UNKNOWN_NUMBER   = 261,  /* unknown subscriber */
  DSAT_CME_SUPS_SERVICE_UNAVAIL  = 262,  /* service not available */
  DSAT_CME_SUPS_UNKNOWN_CLASS    = 263,  /* unknown class */
  DSAT_CME_SUPS_UNKNOWN_NETMSG   = 264,  /* unknown network message */
  

  DSAT_CME_MAX                           /* atcop internal only */
} dsat_cme_error_e_type;


/*added by wangcong00190448 for atserver rewrite*/
/*AT命令执行结果*/    
typedef enum 
{
    ATS_NONE = -1,
    ATS_DEVICE_DOWN,
    ATS_CMD_TIMEOUT,
    ATS_CMD_OK,
    ATS_CME_ERROR,
    ATS_CMS_ERROR,
    ATS_NO_CARRIER,
    ATS_NO_ANSWER,
    ATS_CMD_NOT_SUPPORT,
    ATS_TOO_MANY_PARAMS,
    ATS_GREATTHAN,
    ATS_CMD_RES_MAX
}ATS_CMD_RESULT_EN;

/* 上报Client 消息内容结构 */
typedef struct
{
    ATS_CMD_RESULT_EN        enResultVal;                                       /*  at cmd执行结果码  */
    VOS_UINT8     ucMatchStr[MAX_MATCH_SIZE];                /*  string matched with response from usb  */
    VOS_UINT8     ucReportStr[MAX_ATCMD_REPORT_SIZE+1]; /*  response string to at command  */
    VOS_INT32        lReserved;
}ATS_RCV_MSG_DATA_ST;

/* Client 下发消息内容结构 */
typedef struct
{
    VOS_UINT8     ucCmdStr[MAX_ATCMD_SIZE];                   /*  string send to usb  */
    VOS_UINT8     ucMatchOkStr[MAX_MATCH_SIZE];           /*  string to match correct response from usb  */
    VOS_INT32        lReserved;
}ATS_SND_MSG_DATA_ST;
/*消息结构体*/

/* request main type */
typedef enum{
    ATS_UNKNOW,
    ATS_REQ,                 /*普通的at cmd请求*/
    ATS_ACTIVE_REPORT,          /*激活主动上报*/
    ATS_GET_REPORT_REQ,         /*client 获取主动上报消息*/
    ATS_CMD_REQ,                 /*命令请求*/
	ATS_SMS_SENDORSAVE_REQ,    /* 短信发送或者保存请求 *//* <DTS2013083001384 xiazhongyue 20130830 modify> */
}ATS_REQUEST_TYPE_EN;

/*消息类型*/
typedef enum
{
    ATS_CMD_MSG_ID = 1,//ATCMD消息
    ATS_MSG_ID,//atserver自身初始化消息
    ATS_CSAPP_ID,//语音消息
    ATS_TIMER_MSG_ID,
    ATS_CLIENT_ID_MAX
} ATS_MSG_CLIENT_TYPE_EN;

/*上报类型*/
typedef enum 
{
    ATS_REPORT_REQ_NONE = -1,                            
    ATS_REPORT_CMTI_REQ,                                 
    ATS_REPORT_CMT_REQ,                                  
    ATS_REPORT_RSSI_REQ,                                 
    ATS_REPORT_SMSFULL_REQ,                              
    ATS_REPORT_CLIP_REQ,                                 
    ATS_REPORT_SRVST_REQ,                                
    ATS_REPORT_SIMST_REQ,                                
    ATS_REPORT_MODE_REQ,                                 
    ATS_REPORT_DSFLOWRPT_REQ,                            
    ATS_REPORT_SYSINFO_REQ,      /*at^sysinfo*/          
    ATS_REPORT_CGMR_REQ,         /*软件版本号*/          
    ATS_REPORT_HWVER_REQ,         /*硬件版本号*/         
    ATS_REPORT_PPP_STATE_REQ,    /*pppd中的IP地址和状态*/
    ATS_REPORT_COPS_REQ,                                 
    ATS_REPORT_CMGI0_REQ,                                
    ATS_REPORT_CELLINFO_REQ,                             
    ATS_REPORT_MODEMTIME_REQ,                            
    ATS_REPORT_REQ_MAX
}ATS_REPORT_TYPE_EN;

/*msg返回类型*/
typedef enum 
{

    ATS_DIRECT_BACK,    /*直接返回*/
    ATS_BACK_MAX
}ATS_RESPONSE_TYPE_EN;

typedef enum
{
    ATS_RES_ONE,
    ATS_RES_MUT
}ATS_MUTIL_TYPE;

/*主动上报的at命令结果*/
typedef enum
{
    ATS_REPORT_NONE = -1,
    ATS_REPORT_BOOT,
    ATS_REPORT_CDSI,
    ATS_REPORT_RING,
    ATS_REPORT_CONNECT,
    ATS_REPORT_CLIP,
    ATS_REPORT_SRVST,
    ATS_REPORT_SIMST,
    ATS_REPORT_MODE,
    ATS_REPORT_RSSI,
    ATS_REPORT_DSFLOWRPT,
    ATS_REPORT_CREG,
    ATS_REPORT_CEND,
    ATS_REPORT_CONN,
    ATS_REPORT_CONF,    
    ATS_REPORT_ORIG,    
    ATS_REPORT_EARST,
    ATS_REPORT_CCWA,
    ATS_REPORT_CSSI,    
    ATS_REPORT_CSSU,
    ATS_REPORT_COLP,
    ATS_REPORT_CUSD,
    /* <DTS2013083001384 xiazhongyue 20130830 begin */
    ATS_REPORT_CMTI,
    ATS_REPORT_CMT,
    ATS_REPORT_SMSFULL,/* 短信满 */
    ATS_REPORT_MMIENTER, /* MMI */
    ATS_REPORT_PS_ANSWER,/* 按键拨号 */
    ATS_REPORT_ROUTERSHUTDOWN,   /* <DTS2012101602161 g68080 2012-10-16 mod 软关机改为软件模式 */
    ATS_REPORT_VOICEMAIL,   /* 语音短信 */
	ATS_REPORT_NDISSTAT,
	/*  DTS2013083001384 xiazhongyue 20130830 end > */
    ATS_REPORT_MAX
}ATS_CMD_REPORT_EN;

/* AT Server/Client 消息数据结构 */
typedef struct{
    VOS_UINT32              ulClientId;           /* 客户类型*/
    ATS_REQUEST_TYPE_EN    enReqType;            /* 请求类型 */
    ATS_REPORT_TYPE_EN     enRepType;            /* 上报类型*/           
    ATS_RESPONSE_TYPE_EN   enRspType;            /* 要求响应方式: 直接返回，文件返回 */
    ATS_MUTIL_TYPE         enAtsMutil;
    VOS_UINT32             ulTimeOut;           /* 每条AT 命令的超时时间*/
    union
    {
        ATS_SND_MSG_DATA_ST      stSendMsgData;       /* 发送消息 */
        ATS_RCV_MSG_DATA_ST      stRecvMsgData;       /* 接收消息 */
    }atsSndRcvMsg;
}ATS_MSG_ST;

typedef enum 
{
    ATS_RET_OK,
    ATS_RET_ERROR,
    
}ATS_RET_CODE;

typedef struct 
{
    VOS_INT32                lIndex;
    VOS_CHAR*                   pcName;
}ATS_CMD_ITEM_ST;


typedef struct 
{
    char   buf[1024];
    char* pcur;
    char* pread;
}report_stream_s;

/*AP7D01380     chenkeyun    2008-02-26    del*/


VOS_INT32 ATP_ATS_GetUmtsStatus(VOS_VOID);
/* <DTS2013083001384 xiazhongyue 20130830 begin */
/* cell info 锁小区信息 */
#define CellInfo_CellID_ParaLength  64       //CellInfo的cellid 参数最大长度
#define OPEN_PCUI_PORT_FAIL 256

typedef enum
{
    MEM_ERR = -1,
    MEM_NV,
	MEM_SIM,
	MEM_SR /* DTS2012071301845 j81004134 2012/8/4 */
}sms_memory_type;

#define SMS_FULL_FILE "/tmp/smsfull"
#define MODEM_TIME_STRING_LENGTH 50
#define CLCC_MODE 3
#define VOIICE_SERVER 0

#ifdef SUPPORT_ATP_CUSTOM_FMC_PRODUCT
#define SECOND                    1000
#define AT_HANDSHAKE_VAL        5 /* 握手时间 1s */
#define ATS_TIMER_VAL           1
#define AUTO_PIN_VALIDATE_DISABLE 0//自动校验pin码关闭
#define AUTO_PIN_VALIDATE_ENABLE  1//自动校验pin码开启
#define WLSMOD_START_TIME         30  // 30秒
#define MAX_NVITEMLEN     256
#define AT_NUM_OF_AT_MSG_VAL   5 /* 定时器轮训时候最多能处理的at消息数量 */
#define DATA_SERVER   1
#define FAX_SERVER    2
#define UNKNOW_SERVER 9
#define usb_led 0
#define lan4_led 1
#define lan3_led 2
#define lan2_led 3
#define lan1_led 4
#define qmi_led 5
#define wifi_led 6
#define wifi_btn 7
#define qmi_btn 8

#define usb_led_gpio 52
#define lan4_led_gpio 51
#define lan3_led_gpio 50
#define lan2_led_gpio 49
#define lan1_led_gpio 48
#define qmi_led_gpio 102
#define wifi_led_gpio 22
#define wifi_btn_gpio 20
#define qmi_btn_gpio 43

#define  USB_MOUNT_SUCCESS  1
#define  USB_SEARCHUSB_SUCCESS  1
#define  USB_HIGHSPEED_SUCCESS  1
#define  USB_WRITE_SUCCESS  1
#define  USB_TEST_SUCCESS  1
#define  USB_WRITE_FAIL  0
#define  USB_HIGHSPEED_FAIL  0
#define  USB_TEST_FAIL  0
#define  REV_WMM_SUCCESS  1
#define  REV_WMM_FAIL  0
#define  USB_CMD_LEN  256
#define  RF_CMD_LEN   512
#define  USB_DIR    "/mnt/"
#define  USB_TEST_FILE    "usbtest.txt"
#define  PATITION_PATH    "/proc/partitions"
#define  MMI_FLAG_FILE    "/var/mmiflag"

#define PKG_STATUS_FILE "/tmp/fota_upgrade_status"

#if defined(SUPPORT_ATP_RETRY_APN)|| (SUPPORT_ATP_DNS_REDIRECT)
#define REDIRECTION_SETTED   1
#define SERVICE_VALID              2

typedef enum PPP_STATE_REASON {
    DIALUP_SUCESS = 0,
    DIALUP_PROFILE_FAILED = 1,
    DIALUP_OTHER_FAILED = 2,
    DIALUP_MAX = 3
} ppp_state_reason;
#endif

#if defined(SUPPORT_ATP_DNS_REDIRECT)
#define REDIRECT_NAS_ON     "0"
#define REDIRECT_SIM_ON      "1"
#define REDIRECT_MSG_LEN     8
#endif
#define MAX_NV_12  12
#define MAX_NV_64  64
#define MAX_NV_256  256
typedef struct{    
    char acSimscid[MAX_NV_64];
    char acSaveScid[MAX_NV_64];    
    char acSavePin[MAX_NV_256];
    char acPinEnable[MAX_NV_12];
}BHAL_NVRAM_ST;
typedef enum 
{
    NO_FOUND,
    FOUND
}is_find;


typedef enum 
{
    AT_RET_OK,
    AT_RET_ERROR,
    
}AT_RET_CODE;

/* 定时器 */
typedef struct 
{
    struct sigaction new_sigalrm;
    struct sigaction old_sigalrm;
    struct itimerval new_timer;
    struct itimerval old_timer;
} AT_TIMER_S;

#define AT_CMD_COPS_NUM_NAME 0
#define AT_CMD_GET_SCID 1
#define AT_CMD_GET_IMSI 2
#define AT_CMD_AUTO_VALIDATE_PIN  3//自动校验pin码开启
#define AT_CMD_GET_OPL 4
#define AT_CMD_GET_PNN 5
#define AT_CMD_GET_IMEI 6
#define AT_CMD_SET_COPS_FORMAT 7

/* 判断自动校验pin码是否做过一次了 */
#define GET_AT_AUTO_VALIDATE_PIN(cmdflag)  (((cmdflag)>>AT_CMD_AUTO_VALIDATE_PIN)&0x1)
/* 设置自动校验pin码做过了的标记 */
#define SET_AT_AUTO_VALIDATE_PIN(cmdflag)  ((cmdflag)=(cmdflag)|(0x1<<AT_CMD_AUTO_VALIDATE_PIN))

/* 判断自动校验pin码是否做过一次了 */
#define GET_AT_GET_IMSI(cmdflag)  (((cmdflag)>>AT_CMD_GET_IMSI)&0x1)
/* 设置自动校验pin码做过了的标记 */
#define SET_AT_GET_IMSI(cmdflag)  ((cmdflag)=(cmdflag)|(0x1<<AT_CMD_GET_IMSI))

/* 判断自动校验pin码是否做过一次了 */
#define GET_AT_GET_IMEI(cmdflag)  ((cmdflag>>AT_CMD_GET_IMEI)&0x1)
/* 设置自动校验pin码做过了的标记 */
#define SET_AT_GET_IMEI(cmdflag)  ((cmdflag)=(cmdflag)|(0x1<<AT_CMD_GET_IMEI))

/* 判断cops format是否做过一次了 */
#define GET_AT_SET_COPS_FORMAT(cmdflag)  (((cmdflag)>>AT_CMD_SET_COPS_FORMAT)&0x1)
/* 设置cops format是否设置过了的标记 */
#define SET_AT_SET_COPS_FORMAT(cmdflag)  ((cmdflag)=(cmdflag)|(0x1<<AT_CMD_SET_COPS_FORMAT))

typedef struct tagGlobPara_s
{
	/*在ATS中，包存主动上报的全局结构体 变量*/
	/*短消息满(^SMSFULL)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_smsfull_report_val;

	/* 信号格数，信号强度 */
	ATS_RCV_MSG_DATA_ST g_at_rssi_report_val;

	/*来电号码(^CLIP)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_clip_report_val;

	/*服务状态(^srvst)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_srvst_report_val;

	/*sim状态(^simst)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_simst_report_val;

	/*mode状态(^mode)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_mode_report_val;

	/*流量状态(^dsflowrpt)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_dsflowrpt_report_val;

	/*sysinfo状态(^sysinfo)主动上报。*/
	ATS_RCV_MSG_DATA_ST g_at_sysinfo_report_val;

	ATS_RCV_MSG_DATA_ST g_at_cellinfo_report_val;

	/*cops的网络数名*/
	ATS_RCV_MSG_DATA_ST g_at_cops_report_val;

	/* 无线时间 ^modemtime */
	ATS_RCV_MSG_DATA_ST g_at_modemtime_report_val;
 
	/*未读短信的个数.*/
	ATS_RCV_MSG_DATA_ST g_at_cmgi0_report_val;

    /*产品名称*/
	ATS_RCV_MSG_DATA_ST g_at_product_report_val;

	/* 是否获取到 IMSI号*/
	ATS_RCV_MSG_DATA_ST  g_at_imsi_report_val;
	
	ATS_RCV_MSG_DATA_ST  g_at_imei_report_val;

	ATS_RCV_MSG_DATA_ST  g_at_query_redirection_ppp_state_val;
	
    /*sms 初始化的短信*/
    ATS_RCV_MSG_DATA_ST  g_at_sms_report_val;

    /*软件版本*/
    ATS_RCV_MSG_DATA_ST  g_at_software_report_val;
	
	/*硬件版本*/
	ATS_RCV_MSG_DATA_ST  g_at_hardware_report_val;

	/*ppp状态*/
	ATS_RCV_MSG_DATA_ST  g_at_pppstat_report_val;
	
    VOS_INT32            g_ats_cmd_report_seq;

	VOS_INT32            g_AtsOnceCmdFlag;// = 0;用来控制无线上电后获取一次无线测的一些信息
	//!超时计数，当AT命令超时后认为命令超时失败
	VOS_INT32            g_TimeOutCounter;// = 0;
	VOS_INT32            g_HandshakeCounter;// = 0;        * 握手不成功计数 */
	VOS_INT32            g_HandshakeFailtimes;// = 0;        * 握手不成功次数 */
	VOS_INT32            g_HandshakeStatus;/* 当前是否是握手状态 */
	VOS_INT32            g_iSrvstate;// = -1; * 0表示当前服务有效 1表示当前服务无效 */
	VOS_INT32            g_m_clip_flag; // = 0;
	VOS_INT32            g_sysinfoRoamingStatus; //= -1;
	VOS_CHAR             g_perOper[64];// = {0};
	VOS_INT32            g_AtsTimeOutFlag; /* 超时定时器启动起来的标志 */
	VOS_INT32            g_query_redirection_ppp_state;/* 是否拨号失败 */	
	VOS_INT32            g_AtsTimeProcFlag; /* 超时定时器启动起来的标志 */	
}GLOB_PARA_S;

#endif


#endif
/*  DTS2013083001384 xiazhongyue 20130830 end > */
