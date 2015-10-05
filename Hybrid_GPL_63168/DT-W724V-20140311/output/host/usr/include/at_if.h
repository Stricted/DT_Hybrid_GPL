/**\file at_if.h
  *atserver与其他进程共享接口
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-1-2
*/
#ifndef __AT_IF_H__
#define __AT_IF_H__

#include "atptypes.h"
#include "at_lib.h"
#include "sms_cmp_prs.h"

//!标识当前AT命令的类型
#define AT_CMD_TYPE_CHECK        0         //查询命令
#define AT_CMD_TYPE_EXECUTE      1        //执行命令
#define AT_CMD_TYPE_TEST           2         //测试命令
#define AT_CMD_TYPE_OTHER         3        //测试命令

//!执行AT命令返回结果
typedef enum
{
    AT_CMD_RES_ERR = -3,//ERROR,无类型的ERROR
    AT_CMD_RES_SUCCESS = -2,  //ok
    AT_CMD_RES_BUSY = -1,  //所有cme cms error的集合,
    //前三个为路由模块自定义并使用的错误号.
    AT_CMD_RES_SIM_NOT_INSERT =10,
    AT_CMD_RES_PIN_REQ= 11, //我们关心的PIN错误.
    AT_CMD_RES_PUK_REQ = 12,
    AT_CMD_RES_SIM_FAIL = 13,
    AT_CMD_RES_SMS_FULL = 14,/* <DTS2013083001384 xiazhongyue 20130830 modify> */
    AT_CMD_RES_NO_NETWORK = 15,	 /* <DTS2013103107401  z00185914 2013/11/07 BEGIN */
    AT_CMD_RES_REJECT_NETWORK = 16,  /* DTS2013103107401   z00185914 2013/11/07 END> */
    AT_CMD_RES_AMSS_MAX = 65535//最大值.
}AT_AMSS_RES_EN;

#define  AT_IF_BASE_TIMEOUT  5	/* <DTS2013083001384 xiazhongyue 20130830 modify> */

#define AT_REP_TIMEOUT   1     //主动上报的等待时间.

#define AT_PARAM_LEN             100      //AT命令参数的最大长度
#define AT_MAX_NUM_PARAM         100      //AT命令最大参数个数

#define AT_RESULT_MAX_NUM_PARAM  20       //AT命令结果中最大参数个数

#define CLCK_EXE_MODE_UNLOCK_PIN "0"      //解锁PIN码
#define CLCK_EXE_MODE_LOCK_PIN   "1"      // PIN码锁定
#define CLCK_EXE_MODE_CHECK_PIN  "2"      //查询PIN状态，<mode>=2时

#define SIM_STATE_SIM_READY           "READY"
#define SIM_STATE_SIM_PIN_REQ       "SIM PIN"
#define SIM_STATE_SIM_PUK_REQ     "SIM PUK"
#define SIM_STATE_SIM_PIN2_REQ     "SIM PIN2"
#define SIM_STATE_SIM_PUK2_REQ   "SIM PUK2"
#define AT_EQUAL_SIGN             "="
#define AT_QUESTION_SIGN         "?"
#define AT_COMMA_SIGN            ","
#define AT_MATCH_OK_STR          "OK"
#define AT_MATCH_ERR_STR         "+CME ERROR:"

#define AT_CMD_SYSCFG            "AT^SYSCFG"
#define AT_MATCH_STR_SYSCFG      "^SYSCFG:"

#define AT_CMD_COPS              "AT+COPS"
#define AT_MATCH_STR_COPS        "+COPS:"
#define AT_MATCH_STR_COPS_EX      "+COPS: ("

#define AT_CMD_CPIN                  "AT+CPIN"
#define AT_MATCH_STR_CPIN        "+CPIN:"

#define AT_CMD_CPIN_EX           "AT^CPIN"
#define AT_MATCH_STR_CPIN_EX     "^CPIN:"

#define AT_CMD_CLCK              "AT+CLCK"
#define AT_MATCH_STR_CLCK        "+CLCK:"

#define AT_CMD_CPWD              "AT+CPWD"

#define AT_CMD_CPAS              "AT+CPAS"
#define AT_MATCH_STR_CPAS        "+CPAS:"
#define AT_CMD_HSDPA             "AT^HSDPA"
#define AT_MATCH_STR_HSDPA       "^HSDPA:"
#define AT_CMD_CNMNQ             "AT+CVMNQ"
#define AT_MATCH_STR_CNMNQ       "+CVMNQ:"
#define AT_MATCH_STR_ERR         "ERROR"

#define PPP_DISCONNECTED         -1
#define PPP_CONNECTING           0
#define PPP_CONNECTED            1
#define PPP_QUERY_FAIL           -2
#define PPP_CONNECT_DIE          -3

#define AT_CMD_OPL_2G            "AT^OPL=0"
#define AT_CMD_OPL_3G            "AT^OPL=1"
#define AT_MATCH_OPL              "^OPL:"
#define AT_MATCH_OPL_ERR       "ERROR"
#define AT_CMD_PNN                  "AT^PNN=1"
#define AT_MATCH_PNN              "^PNN:"
#define AT_MATCH_PNN_ERR       "ERROR"

#define WM_VERSION_LEN          32      /* 无线模块版本号的长度 */

#define SIG_USER_MIN               40
#define SIG_WAIT_CLIENT     (SIG_USER_MIN+1)      /* signal waiting from client */
#define SIG_TIME_CLIENT     (SIG_USER_MIN+2)      /* signal waiting from client */
#define SIG_NULL_CLIENT     (SIG_USER_MIN+3)      /* signal waiting from client */

#define SMS_ACK_SINGAL        (SIG_USER_MIN+4)
#define SMS_NAK_SINGAL        (SIG_USER_MIN+5)
#define SMS_HTTP_SINGAL       (SIG_USER_MIN+6)
#define ATS_EXIT_SIGN            (SIG_USER_MIN+7)
#define ATS_NEW_SMS_SIGN    (SIG_USER_MIN+8)

/*AT+CPAS的执行结果*/
#define PHONE_ACT_STATUS_READY   "0"
#define PHONE_ACT_STATUS_RINGING "3"
#define PHONE_ACT_STATUS_BUSY    "4"

#define CTRL_Z  '\32'
#define SIM_OPERATOR_NAME_MAX               256
#define OPL_LIST_MAX               128
#define OPL_ITEM_LEN_MAX       256
#define PNN_INDEX_LEN             8
/* <DTS2013083001384 xiazhongyue 20130830 begin */
/* 记录无线侧的时间文件 */
#define MODEMTIME_FILE_NAME  "/var/modemtime"
#define MODEMTIME_FILE_LOCK    "/var/modemtime_file.lck"
/*  DTS2013083001384 xiazhongyue 20130830 end > */
typedef struct 
{
    VOS_CHAR* err_code;
    VOS_CHAR* err_str;
}cme_err;

typedef enum
{
    SYSMODE_NO_SERVICES =     0,
    SYSMODE_CDMA     =    2,
    SYSMODE_WCDMA  =    5
}SYSINFO_MODE;

typedef enum
{
 SYS_SRV_STATUS_LIMITED                      =          1  ,  /* Limited service */
 SYS_SRV_STATUS_SRV                              =        2   , /* Service available */
 SYS_SRV_STATUS_LIMITED_REGIONAL    = 3,    /* Limited regional service */

}SYSINFO_SRV_STATUS;

typedef enum
{
 SYS_SRV_DOMAIN_PS_ONLY  = 2,       /* Packet Switched Only Capable */
 SYS_SRV_DOMAIN_CS_PS        = 3,       /* Circuit and Packet Switched Capable */

}SYSINFO_SRV_DOMAIN;


/*AT^SYSCFG读操作的返回结果*/
typedef struct NetConfigurationResStruct
{
    int           CmdType;             //AT命令的类型
    char          Mode[5];             //系统模式参考
    char          Acqorder[5];         //网络接入次序参考
    char          Band[20];            //频带选择
    char          Roam[5];             //漫游支持
    char          Srvdomain[5];        //域设置
    AT_AMSS_RES_EN           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}NetConfigurationRes;

/*AT^SYSCFG执行操作传入的参数*/
typedef struct  NetConfigurationTypeStruct
{
    char          Mode[5];             //系统模式参考
    char          Acqorder[5];         //网络接入次序参考
    char          Band[20];            //频带选择， 
    char          Roam[5];             //漫游支持
    char          Srvdomain[5];        //域设置
    int           Reserve;             //保留字
}NetConfigurationType;

/*<AP7D00922,Optus, liukeke 2007-10-08 MODIFY BEGIN*/
/*AT+COPS=?命令返回的运营商列表中每一个运营商的信息*/
typedef struct OperatorInfoStruct
{
    char       State[5];                //网络的状态标识
    char       LongAlphanumeric[SIM_OPERATOR_NAME_MAX];    //运营商全名
    char       ShortAlphanumeric[SIM_OPERATOR_NAME_MAX];   //运营商短名（长度待确认）
    char       numeric[64];             //运营商数字代码   （长度待确认）
    char       Rat[5];                  //无线接入技术
    int        Reserve;                 //保留字
}OperatorInfo;
/*AP7D00922,Optus, liukeke 2007-10-08 MODIFY END>*/

/*AT+COPS=?保存所有运营商信息的数据*/
typedef struct OperatorListStruct
{
    int            OperatorNumber; //上报的运营商总个数
    OperatorInfo   Operator[20];   //上报的运营商列表，最多支持20个
    int            Reserve;        //保留字
}OperatorList;

/*AT+COPS注册网络时输入的数据*/
typedef struct NetChooseOperatorParamStruct    
{        
    char      Mode[5];                  //网络选择模式
    char      Format[5];                //运营商信息的格式
    char      OperatorName[64];         //运营商的名字
    char      Rat[5];                   //无线接入技术
    int       Reserve;                  //保留字
}NetChooseOperatorParam;

#define CHOOSE_OPER_MODE_AUTO            "0"        /*自动搜网*/
#define CHOOSE_OPER_MODE_MANUAL          "1"        /*手动搜网*/
#define CHOOSE_OPER_TO_LOGON             "2"        /*去注册网络*/
#define OPER_NAME_FORMAT                 "3"        /*为AT+CPOS?读命令设置的返回格式*/
#define CHOOSE_OPER_MODE_AUTO_MANUAL     "4"        /*手动与自动结合*/
#define SYS_MODE_AUTO                    "2"        /*自动选择*/
#define SYS_MODE_GSM_ONLY                "13"       /*GSM only*/
#define SYS_MODE_WCDMA_ONLY              "14"       /*WCDMA only*/
#define SYS_MODE_NO_CHANGE               "16"       /*无变化*/
#define SYS_ACQ_ORDER_AUTO               "0"        /*自动*/
#define SYS_ACQ_ORDER_GSM_FIRST          "1"        /*先GSM后WCDMA*/
#define SYS_ACQ_ORDER_WCDMA_FIRST        "2"        /*先WCDMA后GSM*/
#define SYS_ACQ_ORDER_NO_CHANGE          "3"        /*无变化*/
#define CM_BAND_PREF_GSM_DCS_1800        "80"       
#define CM_BAND_PREF_GSM_EGSM_900        "100"      
#define CM_BAND_PREF_GSM_PGSM_900        "200"      
#define CM_BAND_PREF_GSM_PCS_1900        "200000"


/*< AU1D00199,968,liukeke,2007-11-16 ADD BEGIN*/
#define CM_BAND_PREF_WCDMA_1900             "800000"
#define CM_BAND_PREF_WCDMA_850             "4000000"
/*AU1D00199,968,liukeke,2007-11-16 ADD END >*/

/*AP7D00728.H3G.R99: liudan 2007-8-23 ADD BEGIN */
#define R99_NOT_SUPPROT_HSDPA        "0"
#define R99_SUPPROT_HSDPA            "1"
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/
/* <AP7D00411.WEB SERVER: liudan 2007-7-4 ADD BEGIN */
#define CM_BAND_PREF_GSM_850             "80000"
/* AP7D00411.WEB SERVER: liudan 2007-7-4 ADD END> */

#define CM_BAND_PREF_WCDMA_I_IMT_2000    "400000"   
#define CM_BAND_PREF_ANY                 "3FFFFFFF" 
#define CM_BAND_PREF_NO_CHANGE           "40000000" 
#define CM_BAND_PREF_MIXED_BAND          "400380"   
#define OPER_RAT_TECNO_GSM_GPRS          "0"        /* GSM/GPRS制式*/
#define OPER_RAT_TECNO_WCDMA             "2"        /* WCDMA制式*/
#define OPER_INFO_STATUS_UNKNOWN         "0"        /*未知的*/
#define OPER_INFO_STATUS_USEABLE         "1"        /*可用的*/
#define OPER_INFO_STATUS_LOGED_ON        "2"        /*当前注册的*/
#define OPER_INFO_STATUS_FORBIDDEN       "3"        /*禁止*/
/*AT+COPS?保存当前运营商信息的数据*/
#define PHONE_ACT_STATUS_READY            "0"
#define PHONE_ACT_STATUS_RINGING          "3"
#define PHONE_ACT_STATUS_BUSY             "4"
/* <AP7D01399,liudan,2008-03-3 ADD BEGIN */
#define OPER_RAT_TECNO_WCDMA_GSM             "30"
/* <AP7D01399,liudan,2008-03-3 ADD END*/
#define OPERATOR_NAME_LEN           64         /* 运营商短名最大长度 */
#define NETWORK_LEN                       4          /* 2G/3G 最大长度 */

#define MAX_OPERATOR_LINE_LEN          1024
#define OPERATOR_LIST                           "/html/api/ntwk/operator_list.res"/* <DTS2013083001384 xiazhongyue 20130830 modify> */
    
/*<AP7D00922,Optus, liukeke 2007-10-08 MODIFY BEGIN*/
typedef struct
{
    char      Mode[5];                  //网络选择模式
    char      Format[5];                //运营商信息的格式
    char      OperatorName[64];         //运营商的数字名
    char      Operator_Short_Name[SIM_OPERATOR_NAME_MAX];         //运营商的短名名
    char      Operator_Full_Name[SIM_OPERATOR_NAME_MAX];           //运营商的长名名
    char      Rat[5];                   //无线接入技术
    int        Reserve;                  //保留字
}RegistedOperator;
/*AP7D00922,Optus, liukeke 2007-10-08 MODIFY END>*/

/*AT+COPS执行的结果*/
typedef struct NetChooseOperatorResultStruct       
{
    int                   CmdType;      //指示是哪种命令的返回结果
    AT_AMSS_RES_EN                   AtExeResult;  //执行操作的返回结果
    OperatorList         *TotalOper;    //保存测试命令返回的运营商列表
    RegistedOperator     *CurOperInfo;  //保存查询命令返回的运营商信息
    int                   Reserve;      //保留字
}NetChooseOperatorResult;

/*AT+CPIN执行结果*/
typedef struct PinMgResultCodeStruct
{
    int            CmdType;       //针对哪种AT命令的返回结果
    char           Code[20];      //PIN状态
    AT_AMSS_RES_EN AtExeResult;   //AT命令的执行结果
    int            Reserve;       //保留字
}PinMgResultCode;

/*PIN码剩余输入次数*/
typedef struct PinResTimesStruct
{
    char             ResTimes[5];       //<times>字段，剩余的输入次数
    char             ResPukTimes[5];    //<puk_times>字段，剩余的PUK输入次数
    char             ResPinTimes[5];    //<pin_times>字段，剩余的PIN输入次数
    char             ResPuk2Times[5];   //<puk2_times>字段，剩余的PUK2输入次数
    char             ResPin2Times[5];   //<pin2_times>字段，剩余的PIN2输入次数
    int              Reserve;           //保留字
}PinResTimes;


/*AT^CPIN执行结果*/
typedef struct PinMgResultCodeExStruct
{
    int              CmdType;           //针对哪种AT命令的返回结果
    char             Code[20];           //PIN状态
    PinResTimes     ResTimes;          //剩余输入次数
    AT_AMSS_RES_EN              AtExeResult;       //AT命令的执行结果
    int              Reserve;           //保留字
}PinMgResultCodeEx;

/*AT+CLCK传入参数*/
typedef struct PinEnStructStruct
{
    char           Fac[5];         //该命令的操作对象 
    char           Mode[5];        //命令的模式（解锁、锁、查询）
    char           Passwd[11];     //PIN码字符串
    char           classx[5];      //默认为7，当前不支持
    int            Reserve;        //保留字段
}PinEnStruct;

/*AT+CLCK执行结果*/
#define SIM_STATUS_NEGATIVE    "0"  //未激活
#define SIM_STATUS_ACTIVE     "1" //激活
typedef struct PinEnableResultStruct
{
    int            CmdType;       //针对哪种AT命令的返回结果
    AT_AMSS_RES_EN            AtExeResult;   //AT命令的执行结果
    char           Status[5];     //SIM卡状态的查询结果
    int            Reserve;       //保留字
}PinEnableResult;

/*AT+CPWD执行结果*/
typedef struct PinModifyResultCodeStruct
{
    int            CmdType;        //对应哪种AT命令的执行结果
    AT_AMSS_RES_EN            AtExeResult;    //AT命令的执行结果
    int            Reserve;        //保留字
}PinModifyResultCode;

/*AT+CPAS的执行结果*/
typedef struct PhonseActStatusStruct
{
    int            CmdType;        //对应哪种AT命令的执行结果
    AT_AMSS_RES_EN AtExeResult;    //AT命令的执行结果
    char           PActStatus[5];  //当前是否处于通话状态
    int            Reserve;        //保留字
}PhoneActStatus;
/* <D00018.WEB SERVER: liudan 2007-4-9 DEL BEGIN */
/* D00018.WEB SERVER: liudan 2007-4-9 DEL END> */

/*AP7D00728.H3G.R99: liudan 2007-8-23 ADD BEGIN */
typedef struct R99OnlyStruct
{
    int            CmdType;       //针对哪种AT命令的返回结果
    char           Code[5];      //AT^HSDPA状态
    AT_AMSS_RES_EN            AtExeResult;   //AT命令的执行结果
    int            Reserve;       //保留字
}StR99Only;
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/

/* <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
typedef struct CvmnqStruct
{
    int            CmdType;                    /*针对哪种AT命令的返回结果*/
    char           voice_mail_number[25];      //语音信箱号码,最大支持到20个字符.
    char           nv_number[25];              //原始的号码(即从NV中读取的号码)
    AT_AMSS_RES_EN            AtExeResult;                //AT命令的执行结果
    int            Reserve;                    //保留字
}StCvmnq;

/*服务状态变化指示^SRVST */
typedef struct{
    char  srv_status[3];
} StSrvstInfo;

/*系统模式变化事件指示 ^MODE */
typedef struct{
    char  sys_mode[3];
} StSysModeInfo;
/* RSSI 变化指示 ^RSSI */

typedef struct{
    char  rssi [5];
} StRssiInfo;

/* DS流量上报^DSFLOWRPT */
typedef struct{
    unsigned int  curr_ds_time;// 当前这次DS的连接时间，单位为秒
    unsigned int  tx_rate; //表示当前的发送速率，单位为字节每秒
    unsigned int  rx_rate; //表示当前的接收速率，单位为字节每秒。
    unsigned long long  curr_tx_flow;// 表示当前这次DS的发送流量，单位为字节。
    unsigned long long  curr_rx_flow;// 表示当前这次DS的接收流量，单位为字节。
    unsigned int  qos_tx_rate;// 与网络侧协商后确定的PDP连接发送速率，单位为字节每秒。
    unsigned int  qos_rx_rate;// 与网络侧协商后确定的PDP连接接收速率，单位为字节每秒。
} StDsflowrptInfo;

/* DS流量查询^DSFLOWQRY*/
typedef struct{
    unsigned int  last_ds_time;// 当前这次DS的连接时间，单位为秒。
    unsigned long long  last_tx_flow;// 表示当前这次DS的发送流量，单位为字节。
    unsigned long long  last_rx_flow;// 表示当前这次DS的接收流量，单位为字节。
    unsigned int total_ds_time;// 累计DS的连接时间，单位为秒。
    unsigned long long  total_tx_flow;//  累计DS的发送流量，单位为字节。
    unsigned long long  total_rx_flow;//  累计DS的接收流量，单位为字节。
    AT_AMSS_RES_EN AtExeResult;   //AT命令的执行结果
} StDsflowQureyInfo;

/* 软件 版本号查询+CGMR */
typedef struct{
    char acSwVer[64];
    AT_AMSS_RES_EN AtExeResult;   //AT命令的执行结果
} StSwVer;

typedef struct {
    int rcsq_level;
    int rscp;
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCsqlvl;

typedef struct               /* 系统信息 */
{
    VOS_INT32 srv_status;
    VOS_INT32 srv_domain;
    VOS_INT32 roam_status;
    VOS_INT32 sys_mode;
    VOS_INT32 sim_state;
    VOS_INT32 lock_state;
    VOS_INT32 submode;
    AT_AMSS_RES_EN AtExeResult;   //AT命令的执行结果
} StSysinfo;

/* ppp IP地址和连接状态 */
typedef struct
{
    unsigned int ip_addr;           // IP 地址
    int ppp_state;         // PPP 连接状态
    unsigned int cur_time_len;      // 当前连接时长
    unsigned int total_time_len;    // 累计连接时长
    unsigned long long cur_ppp_ibytes;       // PPP当前接收流量
    unsigned long long cur_ppp_obytes;       // PPP当前发送流量
    unsigned long long total_ppp_ibytes;     // PPP累计接收流量
    unsigned long long total_ppp_obytes;     // PPP累计发送流量
}StPppInfo;


typedef struct{
 char mem1[5];
 char used1[5];
 char total1[5];
 char mem2[5];
 char used2[5];
 char total2[5];
 char mem3[5];
 char used3[5];
 char total3[5];
 AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果

}StCpms;

typedef struct{
    at_cmd_result  amss_res;
    dsati_cms_err_e_type  cms_res;
    dsat_cme_error_e_type cme_res;
}StAtRes;

typedef struct{
     AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
}StCmmt;


typedef struct ScidStruct
{
    char           Code[21];      //AT^HSDPA状态
    AT_AMSS_RES_EN            AtExeResult;   //AT命令的执行结果
}StScid;


/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
/*从SIM卡取得的运营商信息*/
typedef struct OplStruct
{
    char   numeric[64];             //运营商数字名
    char   lac_start[8];             //本地区域码的起始值
    char   lac_end[8];             //本地区域码的结束值
    char   index_pnn[PNN_INDEX_LEN];            //指向PNN的第index_pnn条记录
}OplRes;

typedef struct OplListStruct
{
    OplRes     opl_list_2g[OPL_LIST_MAX];
    OplRes     opl_list_3g[OPL_LIST_MAX];
    int           list_num_2g;
    int           list_num_3g;
}OplResList;

typedef struct PnnStruct
{
    char     full_name[SIM_OPERATOR_NAME_MAX];
    char     short_name[SIM_OPERATOR_NAME_MAX];
    char     reserve[SIM_OPERATOR_NAME_MAX];
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
}PnnRes;

void GetAtSrvId(void);

int AT_SysConfig (int CmdType, NetConfigurationType *Param, NetConfigurationRes *Result);
int AT_ChoseOperators(int CmdType, 
                              NetChooseOperatorParam *Param, 
                              NetChooseOperatorResult *Result);
int AT_PinManagement (int CmdType, 
                               char*Pin, 
                               char *NewPin, 
                               PinMgResultCode *Result);
int AT_PinEnableDisableCmd (int CmdType, 
                                      PinEnStruct *Param, 
                                      PinEnableResult *Result);
int AT_PinModify (int CmdType, 
                       char *Fac, 
                       char *OldPwd, 
                       char *NewPwd, 
                       PinModifyResultCode *Result);
int AT_PhoneActSt(int CmdType, PhoneActStatus *Result);
int AT_SysInfo( StSysinfo *Result);

/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
int AT_Get_Opl (int CmdType, OplResList *Result);
int AT_Get_PNN( PnnRes *pnn_name, char *pnn_index, char *flag );

/*AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD END>*/

/*AP7D00728.H3G.R99: liudan 2007-8-23 ADD BEGIN */
int AT_Hsdpa (int CmdType, 
                          char* Param,  
                          StR99Only* Result);
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/

/* <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
int AT_cvmnq (int CmdType, 
              StCvmnq* Param,  
              StCvmnq* Result);
/* AP7D010XX.Optus web server: liudan 2007-10-24 ADD END> */

int AT_Cpms(int cmd_type, StCpms* cpms_para, StCpms* cpms_result);

/*查询主动上报.*/
int AT_Rssi(StRssiInfo *rssi_res);
int AT_Dsflowrpt(StDsflowrptInfo *flow_res);
/*解析cops信息*/
int AT_check_oper_name(NetChooseOperatorResult *Result);
int parse_operator_data(char *psz_operator_data, 
                                             char *psz_operator_short_name, 
                                             char *psz_operator_full_name );
int get_operator_name( char *psz_operator_numeric_name, 
                                           char *psz_operator_short_name, 
                                           char *psz_operator_full_name );

/* Optus SMS: liudan 2007-12-1 ADD END> */

int conv_name_to_utf8(char *pszName, char *pnn_name);
int conv_name_to_ascii(char *pszName, char *pnn_name);
/*int decode_7bit(const unsigned char *puszSrc, unsigned char *puszDst, int iSrcLength);//
int gsm7_to_ascii(const unsigned char *puszGSM_7Data, int InputLen, 
                                     unsigned char *puszASCIIData, int BufLen, int *OutputLen); */

int conv_to_utf8(char *pdst_str, int counte_bytes, 
                                unsigned short byte_one, unsigned char utf_one);
int get_srcstr_length(char *pchar, int *para1, unsigned short *para2, unsigned char *para3);
const char * ATP_ATSHARED_GetVersion(void);

//int get_hex_from_str(char *s); //
//int char_to_hex(char *ch);//
void split_space_opl(char *start, char key);
 AT_AMSS_RES_EN Error_str_to_id(char *at_report_str);
 int set_network_config( char* rat );
int AT_SavePIN(char *pszPinCode);

int atcmdSendCmd(int iCmdType, int iPara1, char* pcPara2);

/*Added by lvxin00135113@20110627 to modify CS atcmd send */
extern VOS_INT32   SendCSATCMDToAtSrvProc_EX(VOS_CHAR *pstAtCmdStr,VOS_CHAR *pstAtCmdMatchOKStr);
/*Added by lvxin00135113@20110627 to modify CS atcmd send */

#ifdef SUPPORT_ATP_ATSERVER

VOS_INT32 ATP_SendMsgToAtserver(const VOS_CHAR *pcSrc, VOS_UINT32 ulEventType, const ATS_MSG_ST *pstSrcAtsMsg, ATS_RCV_MSG_DATA_ST *stRecMsg);

#ifdef SUPPORT_ATP_VOICE
VOS_INT32 ATP_SendCommandToAterver(VOS_CHAR *pstAtCmdStr,VOS_CHAR *pstAtCmdMatchOKStr);

/* <DTS2013103107401  z00185914 2013/11/07 BEGIN */
int AT_PinManagementEx (int CmdType,
                                  const char*Pin, 
                                  const char *NewPin, 
                                  PinMgResultCodeEx *Result);
/* DTS2013103107401   z00185914 2013/11/07 END> */
#endif
#endif 

void ReplaceSpecialChar(char *at_value_str);

#define MAX_OPERATOR_NUM             20
#define ANTENNA_MATCH_STR "^ANTENNA"
#define ANTENNA_INNER_MODE  "1"
#define ANTENNA_OUTER_MODE  "0"

/*AT^CLIP参数及相关结构体*/
#define CLIP_MATCH_STR "^CLIP"
#define CLIP_DTMF_MODE  "0"
#define CLIP_FSK_ITU_MODE  "10"
#define CLIP_FSK_BELL_MODE  "11"
#define CLIP_FSK_RPAS_MODE  "12"

#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
#define SIG_SMS_LOG_SWITCH_CHANGE (SIG_USER_MIN+12)
#define SIG_ATS_LOG_SWITCH_CHANGE (SIG_USER_MIN+13)
#define ATS_PID "/var/at/atsrv.pid"
#endif

#define SMS_SR_BASE_NUMBER 500
#define SMS_NUMBER_LENGTH  21

#define SN_MAX_LENGTH  20

typedef struct
{
    VOS_INT32 vmactive;
    VOS_INT32 vmindex;
} VmindInfo;
#if 0
/* 从共享文件中读取出的预短信 ,并且是解析前的短信结构体.*/
enum EMSGState
{
    DELIVER_NOT_READ = 0,
    DELIVER_READ,
    SUBMIT_NOT_SEND,
    SUBMIT_SEND
};

#define SMS_PDU_SIZE 512
#define PER_SMS_RESERVED_SIZE 16
typedef struct PerSmsStruct{
    int  index;                   /*短信的序号值。*/
    enum EMSGState state;         /* 此短信的类型。0=接收到的未读短信；1=接收到的已读短信；2=存储的未发短信 3=存储的已发短信*/
    unsigned short length;        /* PDU数据的字节数,最大为255*/
    unsigned char reserved[PER_SMS_RESERVED_SIZE];         /* 保留字，现在没有使用。*/
    unsigned char pdu[SMS_PDU_SIZE];    /*pdu包*/
}StPerSms;

typedef enum 
{
	CNMI_DS0,
	CNMI_DS1,
	CNMI_DS2,
}cnmi_ds;
#endif

typedef struct NetCfgExResStruct
{
    VOS_INT32         CmdType;             // AT命令的类型
    VOS_CHAR          Acqorder[5];         // 网络接入次序参考
    VOS_CHAR          Band[64];            // 频带选择
    VOS_CHAR          Roam[5];             // 漫游支持
    VOS_CHAR          Srvdomain[5];        // 域设置
    VOS_CHAR          LTEBand[64];          // LTEBand

    AT_AMSS_RES_EN    AtExeResult;         //AT命令执行结果
    VOS_INT32         Reserve;             //保留字
}NetCfgExResType;

/*AT^SYSCFGEX执行操作传入的参数*/
typedef struct NetCfgExSetStruct
{
    VOS_CHAR          Acqorder[5];         /* 网络接入次序参考 */
    VOS_CHAR          Band[64];            /* 频带选择 */
    VOS_CHAR          Roam[5];             /* 漫游支持 */
    VOS_CHAR          Srvdomain[5];        /* 域设置 */
    VOS_CHAR          LTEBand[64];          /* LTEBand */
    VOS_INT32         Reserve;             /*保留字*/
}NetCfgExSetType;


/* AT^LOCINFO?返回结果 */
typedef struct LocinfoResStruct
{
    VOS_INT32         CmdType;             // AT命令的类型
    VOS_CHAR          plmn[10];         
    VOS_CHAR          lac[10];            
    VOS_CHAR          rac[10];             
    VOS_CHAR          cell_id[10];        
    AT_AMSS_RES_EN    AtExeResult;         //AT命令执行结果
    VOS_INT32         Reserve;             //保留字
}LocinfoResType;

/* AT^LCELLINFO读操作结果 */
typedef struct LcellinfoResStruct
{
    VOS_INT32         CmdType;             // AT命令的类型
    VOS_CHAR          Freq[10];         
    VOS_CHAR          Band[4];            
    VOS_CHAR          CellId[10];             
    VOS_CHAR          Rsrp[6];        
    VOS_CHAR          Rsrq[6];          
    VOS_CHAR          Rssi[6];          

    AT_AMSS_RES_EN    AtExeResult;         //AT命令执行结果
    VOS_INT32         Reserve;             //保留字
}LcellinfoResType;

/*AT^LCELLINFO执行操作传入的参数*/
typedef struct  LcellinfoSetStruct
{
    VOS_CHAR          Acqorder[5];         // 网络接入次序参考
    VOS_CHAR          Freq[10];         
    VOS_CHAR          Band[4];            
    VOS_CHAR          CellId[10];             
    VOS_CHAR          Rsrp[6];        
    VOS_CHAR          Rsrq[6];          
    VOS_CHAR          Rssi[6];          
    VOS_INT32         Reserve;             //保留字
}LcellinfoSetType;

/* AT^CECELLID读操作结果 */
typedef struct CecellIdResStruct
{
    VOS_INT32         CmdType;             // AT命令的类型
    VOS_CHAR          Plmn[10];         
    VOS_CHAR          Ci[10];            
    VOS_CHAR          Pci[10];             
    VOS_CHAR          Tac[10];        

    AT_AMSS_RES_EN     AtExeResult;         //AT命令执行结果
    VOS_INT32          Reserve;             //保留字
}CecellIdResType;

typedef struct CellInfoResStruct
{
    VOS_INT32  sys_mode;
    VOS_CHAR cell_id[CellInfo_CellID_ParaLength];
    VOS_INT32  mcc;
    VOS_INT32  mnc;
    VOS_CHAR lac[12];
    VOS_CHAR rssi[32];
    VOS_CHAR ecio[32];
    VOS_CHAR rscp[32];
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    //ATS_CMD_RESULT_EN
}CellInfoResType;

typedef struct               /* 系统信息 */
{
    VOS_INT32 version;
    VOS_INT32 pin_correct;
    VOS_INT32 iccid_correct;
    VOS_INT32 ecc_check;
    VOS_INT32 lock_style;
    VOS_INT32 total_num;
    VOS_INT32 num_been_locked;
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
} StLockuim2ue;

/* 命令发送格式*/
typedef struct ConfigurationResStruct
{
    VOS_INT32  CmdType;             //AT命令的类型
    VOS_CHAR   Mode[8];             //模式
    VOS_INT32  AtExeResult;         //AT命令执行结果
    VOS_INT32  Reserve;             //保留字
}ConfigurationRes;

/* 执行操作的返回结果*/
typedef struct ConfigurationTypeStruct
{
    VOS_CHAR  Mode[8];             //模式
    VOS_INT32 Reserve;             //保留字
}ConfigurationType;

#define PBM_NUM_SIZE 42
#define PBM_NAME_SIZE 90
typedef struct
{
    VOS_CHAR name[PBM_NAME_SIZE];
    VOS_CHAR num[PBM_NUM_SIZE];
    VOS_INT32  type;
	AT_AMSS_RES_EN  AtExeResult;
} StCnumInfo;

typedef struct
{
    VOS_INT32          fun;
    VOS_INT32          rst;
    AT_AMSS_RES_EN  AtExeResult;
} StCFUN;

typedef struct
{
    VOS_INT32          stat;
    VOS_INT32          AcT;
    VOS_CHAR         rac[16];
    VOS_CHAR         lac[16];
    VOS_CHAR         ci[16];
    AT_AMSS_RES_EN  AtExeResult;
} StCGREG;

typedef struct
{
    VOS_INT32          mm_state;
    VOS_INT32          mm_idle_substate;
    VOS_INT32          gmm_state;
    VOS_INT32          gmm_substate;
    VOS_CHAR         currentBand[33];
    AT_AMSS_RES_EN  AtExeResult;
} StMMGMM;

typedef struct{
 VOS_CHAR mode[5];
 VOS_CHAR acqorder[5];
 VOS_CHAR band[15];
 VOS_CHAR roam[5];
 VOS_CHAR srvdomain[5];
 VOS_CHAR clip[5];
 VOS_CHAR antenna[5];
 AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
}StRoutcfgs;

/* 删除短信的命令传入的参数 */
typedef struct {
    VOS_INT32  index;
    VOS_INT32  delflag;
    VOS_INT32  Reserve;       //保留字
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
}StCmgdPara;

#define COMM_STR_LEN_32 32
/*设置和查询短信中心号码*/
typedef struct {
    char sca[COMM_STR_LEN_32];
    char tosca[COMM_STR_LEN_32];
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCsca;

/* CMSS命令的输入参数 */
typedef struct {
    int index;
    char da[SMS_NUMBER_LENGTH+3];     /* 短信接收者的号码"13801001372" 需要加引号*/
    int toda;      /* 号码类型 */
    int  Reserve;       //保留字
}StCmssPara;

/* 从存储器发送短信后的返回结果。 */
typedef struct {
    int mr;
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCmssRes;

/* 保存短信的返回结果 */
typedef struct {
    int  index;
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCmgwRes;

/* at命令at^sn返回结果。 */
typedef struct {
    char sn[SN_MAX_LENGTH];
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StSnRes;

/* 从存储器发送短信后的返回结果。 */
typedef struct {
    int status;
    AT_AMSS_RES_EN  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StClckRes;

typedef enum 
{
	CNMI_DS0,
	CNMI_DS1,
	CNMI_DS2,
}cnmi_ds;

VOS_INT32 At_if_BuildCmd
(
	VOS_INT32            *pulClientId,
	ATS_REQUEST_TYPE_EN  *pReqType,
	ATS_RESPONSE_TYPE_EN *pRspType,
	ATS_MUTIL_TYPE       *pAtsMutil,
	VOS_INT32           *pulTimeOut,
	VOS_CHAR             *ucCmdStr,
	VOS_CHAR             *ucMatchStr,
	ATS_MSG_ST           *pstMsgBuf
);
VOS_INT32 execATCMD_Ex(ATS_MSG_ST *pstMsgBuf, ATS_RCV_MSG_DATA_ST *pstRcvMsgBuf);
AT_AMSS_RES_EN Error_str_to_id(VOS_CHAR *at_report_str);
VOS_INT32 AT_Clcc();
VOS_INT32 AT_AntennaConfig
(
	VOS_INT32          CmdType, 
	ConfigurationType *Param, 
	ConfigurationRes  *Result
);
VOS_INT32 AT_ClipConfig
(
	VOS_INT32          CmdType, 
	ConfigurationType *Param, 
	ConfigurationRes  *Result
);
VOS_INT32 AT_Lockuim2ue(StLockuim2ue *Result);
/* <DTS2013103107401  z00185914 2013/11/07 BEGIN */
VOS_INT32 AT_PinManagementEx
(
	VOS_INT32 CmdType,
	const VOS_CHAR *Pin, 
	const VOS_CHAR *NewPin, 
	PinMgResultCodeEx *Result
);
/* DTS2013103107401   z00185914 2013/11/07 END> */
VOS_INT32 AT_PinEnableDisableCmd 
(
	VOS_INT32 CmdType, 
	PinEnStruct *Param, 
	PinEnableResult *Result
);
VOS_INT32 AT_SysConfig 
(
	VOS_INT32 CmdType, 
	NetConfigurationType *Param, 
	NetConfigurationRes *Result
);
VOS_INT32 AT_ChoseOperators
(
	VOS_INT32 CmdType, 
	NetChooseOperatorParam *Param, 
	NetChooseOperatorResult *Result
);
VOS_INT32 AT_cnum(StCnumInfo *Result);
VOS_INT32 AT_ICCID( StScid *result );
VOS_INT32 AT_CFUN(StCFUN *Result);
VOS_INT32 AT_CGREG(StCGREG *Result);
VOS_INT32 AT_MMGMM(StMMGMM *Result);
VOS_INT32 AT_CellInfo(CellInfoResType *Result);
VOS_INT32 AT_SysInfoForXml(StSysinfo *Result,char *at_report_str);
pid_t get_pid( VOS_CHAR * filename );
VOS_INT32 AT_Cpms(VOS_INT32 cmd_type, StCpms* cpms_para, StCpms* cpms_res);
VOS_INT32 AT_Cmgd (VOS_INT32 cmd_type, StCmgdPara* cmgd_para);
VOS_INT32 At_ReadSmsMsg(sms_memory_type memType,  VOS_INT32 smsIndex, VOS_INT32 *plRet);
VOS_INT32 AT_CheckSmsReady(VOS_INT32 *status);
VOS_INT32 AT_CpmsCmd
(
	VOS_INT32       CmdType,
	sms_memory_type memType,
	VOS_INT32      *pResult
);
VOS_INT32 AT_Cnmi(cnmi_ds ds_status,VOS_INT32 *pResult);
VOS_INT32 AT_Cmgf(VOS_INT32 *pResult);
VOS_INT32 AT_Cmsr(VOS_INT32 *pResult);
VOS_INT32 AT_Cmgi(sms_state stats,VOS_INT32 *pResult,ATS_RCV_MSG_DATA_ST *pstTmpMsgBuf);
VOS_INT32 AT_CpmsCmd
(
	VOS_INT32       CmdType,
	sms_memory_type memType,
	VOS_INT32      *pResult
);
VOS_INT32 AT_Cmmt (VOS_INT32 index_tmp, VOS_INT32 state,StCmmt *result);
VOS_INT32 AT_Csca( VOS_INT32 CmdType, StCsca* csca_para, StCsca* csca_res);
VOS_INT32 AT_Cmgw (StCmgsInfo* sms_pdu, StCmgwRes* cmgw_result);
VOS_INT32 AT_Cmss (StCmssPara* cmss_para, StCmssRes* cmss_result);
VOS_INT32 AT_ModemTime(char *time_str);
VOS_INT32 AT_Clck(VOS_INT32 CmdType,VOS_CHAR *InputStr,StClckRes *result,VOS_INT32 ulSpecialMode);
VOS_INT32 AT_New_Cpin(VOS_INT32 CmdType,VOS_CHAR *InputStr, PinMgResultCodeEx *Result);
VOS_INT32 AT_Cpwd(VOS_INT32 CmdType,VOS_CHAR *InputStr,PinMgResultCodeEx *Result);
VOS_INT32 AT_Sn(StSnRes *result);
VOS_INT32 AT_ICCID_New( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
VOS_INT32 AT_SysConfigEx (int CmdType, NetCfgExSetType *Param, NetCfgExResType *Result);
VOS_INT32 AT_Locinfo (VOS_INT32 CmdType, LocinfoResType *Result);
VOS_INT32 AT_Lcellinfo (VOS_INT32 CmdType, LcellinfoSetType *Param, LcellinfoResType *Result);
VOS_INT32 AT_CecellId (VOS_INT32 CmdType, CecellIdResType *Result);
VOS_INT32 AT_Cmgi_unread_sms (VOS_INT32* sms_list,VOS_UINT32 list_size, VOS_INT32* list_length);
VOS_INT32 AtsSetMmiRouterSuccess(VOS_INT32 *lRetCode);
VOS_INT32 AtsSetMmiRouterFail(VOS_INT32 *lRetCode);
VOS_INT32 AT_MmiRouter(VOS_BOOL mmiflag);
VOS_INT32 AT_BSn(StSnRes *result);
VOS_INT32 AT_RSTAMSS(VOS_VOID);
VOS_INT32 AT_SCID( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
VOS_INT32 AT_CIMI( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
VOS_INT32 AT_GSN( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
int AT_Csqlvl (StCsqlvl *Result);
int AT_DsflowQurey(StDsflowQureyInfo *Result);
/* <DTS2013093003078 xiazhongyue 20131008 begin */
VOS_INT32 AT_PhyNum( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
VOS_INT32 AT_CardMode( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
/* DTS2013093003078 xiazhongyue 20131008 end  > */
int AT_CFUNManagementEx (VOS_BOOL bMode, StCFUN *Result);
VOS_INT32 AT_GroupID( VOS_CHAR *pOutPutStr,VOS_INT32 *result);
VOS_INT32 AT_Cqst(VOS_INT32 input, VOS_INT32 *result);
#endif
/*  DTS2013083001384 xiazhongyue 20130830 end > */
