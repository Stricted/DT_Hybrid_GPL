/***********************************************************************
  版权信息 : 版权所有(C) 1988-2005, 华为技术有限公司.
  文件名      : at_lib.c
  作者           : yangjianping
  版本           : 
  创建日期 : 2007-3-21
  完成日期 : 
  功能描述 : AT LIB头文件
      
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>     <修改时间>  <版本>  <修改描述>
    yangjianping   20070915    0.0.1    初始化
  备注: 
************************************************************************/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z00159386    2012/10/16    DTS2012102201180     TELUS定制需求，包含Voicemail、漫游开关、SMS重定向
l00162999    2012/12/10    DTS2012120601906      [路由]B890-66 V100R001C259B116SP01(Router)：在固网和移动上行切换的情况下，发送短信的短信提示内容中时间错误，概率性获取到2013年1月1日的时间。
*******************************************************************************/
#ifndef __AT_LIB_H__
#define __AT_LIB_H__


#define   charGreateThan        ">"
#define   BASE_REPORT_SEQ           1000
#define   SMS_CMTI_REPORT_SEQ     unsigned int(BASE_REPORT_SEQ+1)
#define    SMS_CMT_REPORT_SEQ       unsigned int(BASE_REPORT_SEQ+2)
#define   CMIT_SIZE_MAX                50
#define   MAX_ATCMD_SIZE          512     /* max size of at command */
#define   MAX_MATCH_SIZE          100     /* max size of at command match str */
#define   MAX_ATCMD_REPORT_SIZE   512     /* max size of at command report str */
#define   BASE_TIMEOUT                50
#define   AT_TABLE_MAX                  -1


#define   ATS_QUEUE_REC_KEY     860
#define   ATS_QUEUE_SEND_KEY    861

/*<DTS2011050600679 add by pkf36956 20110506 begin*/
#define MAX_NVITEMLEN     256
/*DTS2011050600679 add by pkf36956 20110506 end>*/

/* 消息队列相关变量 */
extern int rcv_mq_id  ;                                 /* ATS接收消息队列标识符 */
extern int snd_mq_id ;                                 /* ATS发送消息队列标识符 */
#if defined(SUPPORT_ATP_AT_PASS_THROUGH)
extern int rpt_mq_id;
#endif

/* request main type */
typedef enum{
    AT_REQ,                 /*普通的at cmd请求*/
    ACTIVE_REPORT,          /*激活主动上报*/
    GET_REPORT_REQ,         /*client 获取主动上报消息*/
    AT_VIR_QEQ,             /*伪命令请求*/
    CMD_REQ,                 /*命令请求*/
#if defined(SUPPORT_ATP_AT_PASS_THROUGH)
    AT_PASS_THROUGH      /* AT命令透传 */
#endif
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
    ATS_MSG_ID,
    ATC_SMS_ID,
    ATC_CS_ID,
    ATC_CSAPP_ID,
//#if defined(SUPPORT_ATP_AT_PASS_THROUGH)
    COMMMGR_MSG_ID,
//#endif
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
    /* <DTS2011071304912 lkf47960 2011-7-12 for T-mobile pnn ADD BEGIN*/
#ifdef SUPPORT_ATP_PNN
    AT_REPORT_PNN_REQ,
    AT_REPORT_OPL_REQ,
#endif
    /* DTS2011071304912: lkf47960 2011-7-12 for T-mobile pnn ADD END>*/        
/*<DTS2011060304054 cell info zhaiqi.z00174311 20110425 begin*/
    AT_REPORT_CELLINFO_REQ,
/*DTS2011060304054 cell info zhaiqi.z00174311 20110425 end>*/
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
    /* ^modetime */
    AT_REPORT_MODEMTIME_REQ, 
/* DTS2013012602546 z81004485 2013/1/28 end> */
/* DTS2012110602724 l00162999 20121117 end> */
/* Optus SMS: liudan 2008-1-15 ADD BEGIN*/
    AT_REPORT_CMGI0_REQ,
/* Optus SMS: liudan 2008-1-15 ADD END*/
/*<DTS2011060304054 zhaiqi.z00174311 20110425 delete>*/
/* <DTS2012102201180  z00159386  20121009  begin */
#ifdef SUPPORT_ATP_VOICEMAIL
    AT_REPORT_CUSD_REQ,             /*at+cusd:补充业务*/
	AT_REPORT_VOICEMAIL_REQ,        /*voicemail*/
#else    
/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */
    #ifdef SUPPORT_ATP_USSD
    AT_REPORT_CUSD_REQ,             /*at+cusd:补充业务*/
    #endif
/* DTS2011030101068.USSD: mayang 2011-03-09 MODIFY END> */

#endif /* SUPPORT_ATP_VOICEMAIL */
/* DTS2012102201180  z00159386  20121009  end> */
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
/*<mmi l129990 20101117 add begin*/
    AT_REPORT_MMIENTER,
 /*mmi l129990 20101117 add end>*/
/*<AP7D04492  w00126555 20100330 add begin*/
    AT_REPORT_PS_ANSWER,//按键拨号
/*AP7D04492  w00126555 20100330 add end>*/
    AT_REPORT_ROUTERSHUTDOWN,   /* <DTS2012101602161 g68080 2012-10-16 mod 软关机改为软件模式 */
/* <DTS2012102201180  z00159386  20121008  begin */
#ifdef SUPPORT_ATP_VOICEMAIL
    AT_REPORT_VOICEMAIL,
#endif /* SUPPORT_ATP_VOICEMAIL */
/* DTS2012102201180  z00159386  20121008  end> */
/* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 begin */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
    AT_REPORT_NDISSTAT,	
#endif
/* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 end> */
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

typedef enum
{
       MEM_ERR = -1,
    MEM_NV,
	MEM_SIM,
	MEM_SR /* DTS2012071301845 j81004134 2012/8/4 */
}sms_memory_type;

typedef enum
{
    VIRCMD_NONE = -1,
    VIRCMD_PPPD,
    VIRCMD_MAX

}at_virCmd_index;

/* 上报Client 消息内容结构 */
typedef struct
{
    at_cmd_result        ret_val;                                       /*  at cmd执行结果码  */
    char     at_match_str[MAX_MATCH_SIZE];                /*  string matched with response from usb  */
    char     at_report_str[MAX_ATCMD_REPORT_SIZE+1]; /*  response string to at command  */
    int        reserved;
}rsp_msg;

/* Client 下发消息内容结构 */
typedef struct
{
    char     at_cmd_str[MAX_ATCMD_SIZE];                   /*  string send to usb  */
    char     at_match_ok_str[MAX_MATCH_SIZE];           /*  string to match correct response from usb  */
    //char     at_match_error_str[MAX_MATCH_SIZE];       /*  string to match error response from usb  */
    int        reserved;
}req_msg;

/* AT Server/Client 消息队列消息缓冲区数据结构 */
typedef struct{
    long                  client_id;              /* 客户类型*/
    /*Added by lvxin00135113@20090907 for send siguser2 signal to atcmd*/
    unsigned int         client_pid;            /*客户pid，只有atcmd需要*/
    /*Added by lvxin00135113@20090907 for send siguser2 signal to atcmd*/
    at_req_type            req_type;            /* 请求类型 */
    at_report_type        rep_type;
    at_rsp_type            rsp_type;             /* 要求响应方式: 直接返回，文件返回 */
    unsigned int            seq;                     /* 消息序号*/
    at_mutil_type         at_mutil;             /*   */
    int                        time_out;           /* 每条AT 命令的超时时间*/
    union
    {
        req_msg      smsg;       /* 发送消息 */
        rsp_msg      rmsg;       /* 接收消息 */
    }snd_rcv_msg;
}msg_q;

#if defined(SUPPORT_ATP_AT_PASS_THROUGH)
/* AT Server 主动上报消息队列消息缓冲区数据结构 */
typedef struct{
    long    rpt_id;                           /* 主动上报类型*/
    char    at_rpt_str[MAX_ATCMD_REPORT_SIZE];       /* 主动上报at命令串 */
}rpt_msg_q;
#endif /*#ifdef SUPPORT_ATP_AT_PASS_THROUGH*/

typedef struct 
{
    int                index;
    char*                   name;
}cmd_item;


typedef struct 
{
    char   buf[1024];
    char* pcur;
    char* pread;
}report_stream_s;

/*AP7D01380     chenkeyun    2008-02-26    del*/

int read_from_msgqueue(int qid, msg_q *pmsg, int match_seq, int *retcode);

int send_to_msgqueue(int qid, msg_q *pmsg,int *retcode);

void clear_msg_from_queue(int qid, long mtype);

void remove_msg_queue(int qid);

int get_mq_num(int qid);
int check_server_ready(int *retcode);
int get_umts_status(void);

/*<DTS2011050600679 add by pkf36956 20110506 begin*/
int get_vendor_nvram(char * valuebuf,const char * nvname);
/*DTS2011050600679 add by pkf36956 20110506 end>*/

#endif

