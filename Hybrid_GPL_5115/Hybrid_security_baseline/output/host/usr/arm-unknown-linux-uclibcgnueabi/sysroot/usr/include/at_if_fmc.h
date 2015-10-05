#ifndef __AT_IF_H__
#define __AT_IF_H__
/***********************************************************************
  版权信息 : 版权所有(C) 1988-2005, 华为技术有限公司.
  文件名   : at_if.c
  作者     : chekeyun
  版本     : 
  创建日期 : 2007-3-21
  完成日期 : 
  功能描述 : AT Server与其它进程共享的接口.
      
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>     <修改时间>  <版本>  <修改描述>
    chengangyi   20070321    0.0.1    初始化
  备注: 
************************************************************************/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
l00162999    2012/11/19    DTS2012110602724    B890-66 V100R001C259B111(Router) 【新需求】完成AssureNet功能需要实现email发送功能
z00159386    2012/10/16    DTS2012102201180      TELUS定制需求，包含Voicemail、漫游开关、SMS重定向
l00162999    2012/12/10    DTS2012120601906      [路由]B890-66 V100R001C259B116SP01(Router)：在固网和移动上行切换的情况下，发送短信的短信提示内容中时间错误，概率性获取到2013年1月1日的时间。
z00159386    2013/01/09    DTS2012123101571     LTE网络，设置wireless profile为TELUS ISP后，单板注册网络使用的APN还是上一次设置的profile（TELUS VPN）对于的APN
l00162999    2013-01-24    DTS2013012205684    [Web][B880-73]-V100R001C994B002(ROUTER): b880设置网络模式使用了新的at命令AT^SYSCFGEX，web server和AT server需要做相应的改动配合Web对无线的查询和修改
z00185914    2013/02/25    DTS2013021908159 	 【网络选择】使用手动搜网,概率性出现页面长时间无法访问情况
l00162999    2013/03/13    DTS2013021907887      [网络选择][B880]-V100R001C994B100(AMSS): 单板注册4G网络后，diagnositc页面里相关信息未更新
z81004143    2013/05/03    DTS2013050301791     [TR069] B890-66 V100R001C994B122(ROUTER): 新增TR069功能
*******************************************************************************/


#include "sms_cmp_prs.h"
#include "at_lib_fmc.h"
#include "at_utils_fmc.h"
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
#include <stdio.h>
/* DTS2013012602546 z81004485 2013/1/28 end> */
/* DTS2012110602724 l00162999 20121117 end> */
/* <DTS2013021908159 使用手动搜网,概率性出现页面长时间无法访问情况 z00185914 20130225 BEGIN */
#include "atpconfig.h"
/* DTS2013021908159 使用手动搜网,概率性出现页面长时间无法访问情况 z00185914 20130225 END> */ 

/*--------------------------------------------------------
        标识当前AT命令的类型
---------------------------------------------------------*/
#define AT_CMD_TYPE_CHECK        0         //查询命令
#define AT_CMD_TYPE_EXECUTE      1        //执行命令
#define AT_CMD_TYPE_TEST           2         //测试命令
#define AT_CMD_TYPE_OTHER         3        //测试命令

/*--------------------------------------------------------
        返回的AT命令是通过什么方式下发的
---------------------------------------------------------*/
#define SEND_WEB_DATA            0        //调用AT命令的代号下发的
#define SEND_WEB_ATCMD           1        //转发AT命令下发的

/* <DTS2013050301791 z81004143 2013/5/6 begin */
/* 定义at命令执行相关返回值 */
#define U_SUCCESS   0
#define U_FAIL      -1
/* DTS2013050301791 z81004143 2013/5/6 end> */

/*--------------------------------------------------------
        标识当前AT命令的执行结果
---------------------------------------------------------*/
/* <DTS2013042408549 z81004485 2013/4/25 begin  */
//所有AT命令函数的返回结果,用来表示函数执行的结果,
//#define AT_CMD_RESULT_ERROR     -1      //执行失败      
//#define AT_CMD_RESULT_OK         0      //执行成功
//AT_AMSS_RES表示无线模块的AT命令的返回结果的集合,
//由于无线模块的返回结果较多,但我们只关心以下几种.
typedef enum
{
    AT_CMD_RES_ERR = -3,//ERROR,无类型的ERROR
    AT_CMD_RES_SUCCESS = -2,  //ok
    AT_CMD_RES_BUSY = -1,  //所有cme cms error的集合,
    //前三个为路由模块自定义并使用的错误号.
    AT_CMD_RES_SIM_NOT_INSERT = 10, /* {"10",       "SIM not inserted"}, */
    AT_CMD_RES_PIN_REQ = 11,        /* 我们关心的PIN错误. */
    AT_CMD_RES_PUK_REQ = 12,
    AT_CMD_RES_SIM_FAIL = 13,       /* {"13",       "SIM failure"}, */    
    AT_CMD_RES_SIM_BUSY = 14,       /* {"14",       "SIM busy"}, */
    AT_CMD_RES_SMS_FULL = 15,       /* DTS2012122503309 j81004134 2012/12/27 */
    AT_CMD_RES_AMSS_MAX = 65535//最大值.
}AT_AMSS_RES;
/* DTS2013042408549 z81004485 2013/4/25 end> */

#define  AT_IF_BASE_TIMEOUT  30

/* <DTS2013021908159 使用手动搜网,概率性出现页面长时间无法访问情况 z00185914 20130225 BEGIN */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
#define  AT_IF_MAX_MSGQUEUE_TIMEOUT  300	/*  海思平台搜网的时间需要这么长  */
#else
#define  AT_IF_MAX_MSGQUEUE_TIMEOUT  120
#endif
/* DTS2013021908159 使用手动搜网,概率性出现页面长时间无法访问情况 z00185914 20130225 END> */ 

#define AT_REP_TIMEOUT   1     //主动上报的等待时间.

#define AT_PARAM_LEN             100      //AT命令参数的最大长度
#define AT_MAX_NUM_PARAM         100      //AT命令最大参数个数
/*<DTS2011060304054 cell info zhaiqi.z00174311 20110425 begin*/
/*<signal info  l129990 20101127 add begin*/
#define CellInfo_CellID_ParaLength  64       //CellInfo的cellid 参数最大程度
/*signal info l129990 20101127 add end>*/
/*DTS2011060304054 cell info zhaiqi.z00174311 20110425 end>*/

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
/* <D00036.HTTP: chengangyi 2007-4-14 MODIFY BEGIN */
#define AT_MATCH_STR_COPS_EX      "+COPS: ("

/* D00036.HTTP: chengangyi 2007-4-14 MODIFY END> */
#define AT_CMD_CPIN                  "AT+CPIN"
#define AT_MATCH_STR_CPIN        "+CPIN:"

#define AT_CMD_CPIN_EX           "AT^CPIN"
#define AT_MATCH_STR_CPIN_EX     "^CPIN:"

#define AT_CMD_CLCK              "AT+CLCK"
#define AT_MATCH_STR_CLCK        "+CLCK:"

#define AT_CMD_CPWD              "AT+CPWD"

#define AT_CMD_CPAS              "AT+CPAS"
#define AT_MATCH_STR_CPAS        "+CPAS:"
/*AP7D00728.H3G.R99: liudan 2007-8-23 ADD BEGIN */
#define AT_CMD_HSDPA             "AT^HSDPA"
#define AT_MATCH_STR_HSDPA       "^HSDPA:"
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/
/*  <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
#define AT_CMD_CNMNQ             "AT+CVMNQ"
#define AT_MATCH_STR_CNMNQ       "+CVMNQ:"
#define AT_MATCH_STR_ERR         "ERROR"
/* AP7D010XX.Optus web server: liudan 2007-10-24 ADD END> */

#define PPP_DISCONNECTED         -1
#define PPP_CONNECTING           0
#define PPP_CONNECTED            1
#define PPP_QUERY_FAIL           -2
#define PPP_CONNECT_DIE          -3
/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
#define AT_CMD_OPL_2G            "AT^OPL=0"
#define AT_CMD_OPL_3G            "AT^OPL=1"
#define AT_MATCH_OPL              "^OPL:"
#define AT_MATCH_OPL_ERR       "ERROR"
#define AT_CMD_PNN                  "AT^PNN=1"
#define AT_MATCH_PNN              "^PNN:"
#define AT_MATCH_PNN_ERR       "ERROR"
/*AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD END>*/

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
/* <DTS2012030904901,wanglong added begin, 2012,3,20 */
#define SIG_WM_RESET        (SIG_USER_MIN+9)
#define SIG_REBOOT          (SIG_USER_MIN+10)
/* DTS2012030904901,wanglong added end, 2012,3,20> */

#if defined(SUPPORT_ATP_AT_PASS_THROUGH)
#define ATS_REPORT_SIGN_CMGR    (SIG_USER_MIN+11)
#endif

/* <DTS2012060604837 j81004134 2012/6/4 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
#define SIG_SMS_LOG_SWITCH_CHANGE (SIG_USER_MIN+12)
#define SIG_ATS_LOG_SWITCH_CHANGE (SIG_USER_MIN+13)
#define ATS_PID "/var/at/atsrv.pid"
#endif
/* DTS2012060604837 j81004134 2012/6/4 end> */
/*AT+CPAS的执行结果*/
#define PHONE_ACT_STATUS_READY   "0"
#define PHONE_ACT_STATUS_RINGING "3"
#define PHONE_ACT_STATUS_BUSY    "4"

/* SMS AT命令期望返回字符串定义*/
#define AT_MATCH_STR_CMSS   "+CMSS:"
#define AT_MATCH_STR_CMSR   "^CMSR:"
#define AT_MATCH_STR_CSCA   "+CSCA:"
#define AT_MATCH_STR_CMGI   "^CMGI:"
#define AT_MATCH_STR_CPMS   "+CPMS:"
#define AT_MATCH_STR_CMGW   "+CMGW:"
/*AP7D01522.SMS: liudan 2008-3-13 MODIFY BEGIN*/
#define AT_MATCH_STR_CMGS   "+CMGS:"
/* AP7D01522.SMS: liudan 2008-3-13 MODIFY END*/
#define CTRL_Z  '\32'

/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
/* CTZU匹配字符 */
#define CTZU_MATCH_STR      "^CTZU:"
/* 记录无线侧的时间文件 */
#define MODEMTIME_FILE_NAME  "/var/modemtime"
#define MODEMTIME_FILE_LOCK    "/var/modemtime_file.lck"
/* DTS2013012602546 z81004485 2013/1/28 end> */
/* DTS2012110602724 l00162999 20121117 end> */
/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
#define SIM_OPERATOR_NAME_MAX               256
#define OPL_LIST_MAX               128
#define OPL_ITEM_LEN_MAX       256
#define PNN_INDEX_LEN             8
/*AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD END>*/

/* <DTS2012072605581 liukai 20120806 在Advanced Settings中无法切换Clip Mode begin */
#define CLCC_MODE 3
#define VOIICE_SERVER 0
#define DATA_SERVER   1
#define FAX_SERVER    2
#define UNKNOW_SERVER 9
/* DTS2012072605581 liukai 20120806 在Advanced Settings中无法切换Clip Mode end> */

/* <DTS2012102603132 z00189398 2012-11-05 begin */
/* <DTS2013012400225 w00189018 20130124 begin */
#if defined (SUPPORT_ATP_TELUS) || defined (SUPPORT_ATP_NAMEFROMAMSS)
#define COPS_FIRST_CMD        "3"
#define COPS_LONG_NAME       "0"
#define COPS_SHORT_NAME     "1"
#define COPS_NUM_NAME        " 2"
#endif
/* DTS2013012400225 w00189018 20130124 end> */
/* DTS2012102603132 z00189398 2012-11-05 end> */

typedef struct 
{
    char* err_code;
    char* err_str;
}cme_err;

#define SMS_NUMBER_LENGTH             20

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
    char          Band[64];            //频带选择
    char          Roam[5];             //漫游支持
    char          Srvdomain[5];        //域设置
    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}NetConfigurationRes;

/*AT^SYSCFG执行操作传入的参数*/
typedef struct  NetConfigurationTypeStruct
{
    char          Mode[5];             //系统模式参考
    char          Acqorder[5];         //网络接入次序参考
    char          Band[64];            //频带选择， 
    char          Roam[5];             //漫游支持
    char          Srvdomain[5];        //域设置
    int           Reserve;             //保留字
}NetConfigurationType;

/* <DTS2013012205684 l00162999 20130122 begin */
/* B880采用syscfgex配置网络 */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
/* AT^SYSCFGEX读操作的返回结果 */
typedef struct __NetCfgExRes
{
    int           CmdType;             // AT命令的类型
    char          Acqorder[5];         // 网络接入次序参考
    char          Band[64];            // 频带选择
    char          Roam[5];             // 漫游支持
    char          Srvdomain[5];        // 域设置
    char          LTEBand[64];          // LTEBand

    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}NetCfgExRes;

/*AT^SYSCFGEX执行操作传入的参数*/
typedef struct  __NetCfgExSet
{
    char          Acqorder[5];         // 网络接入次序参考
    char          Band[64];            // 频带选择
    char          Roam[5];             // 漫游支持
    char          Srvdomain[5];        // 域设置
    char          LTEBand[64];          // LTEBand
    int           Reserve;             //保留字
}NetCfgExSet;

/* <DTS2013021907887 l00162999 20130314 begin */
/* AT^LOCINFO?返回结果 */
typedef struct __LocinfoRes
{
    int           CmdType;             // AT命令的类型
    char          plmn[10];         
    char          lac[10];            
    char          rac[10];             
    char          cell_id[10];        

    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}LocinfoRes;

/* AT^LCELLINFO读操作结果 */
typedef struct __LcellinfoRes
{
    int           CmdType;             // AT命令的类型
    char          Freq[10];         
    char          Band[4];            
    char          CellId[10];             
    char          Rsrp[6];        
    char          Rsrq[6];          
    char          Rssi[6];          

    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}LcellinfoRes;

/*AT^LCELLINFO执行操作传入的参数*/
typedef struct  __LcellinfoSet
{
    char          Acqorder[5];         // 网络接入次序参考
    char          Freq[10];         
    char          Band[4];            
    char          CellId[10];             
    char          Rsrp[6];        
    char          Rsrq[6];          
    char          Rssi[6];          
    int           Reserve;             //保留字
}LcellinfoSet;

/* AT^CECELLID读操作结果 */
typedef struct __CecellIdRes
{
    int           CmdType;             // AT命令的类型
    char          Plmn[10];         
    char          Ci[10];            
    char          Pci[10];             
    char          Tac[10];        

    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}CecellIdRes;
/* DTS2013021907887 l00162999 20130314 end> */

/* <DTS2013061708346 l00162999 20130620 begin */
/* AT^CPNN读操作结果 */
typedef struct __CPNNRes
{
    int           CmdType;             // AT命令的类型
    char          fname[512];
    int           fname_code;
    int           fname_ci;
    char          sname[512];
    int           sname_code;
    int           sname_ci;

    AT_AMSS_RES           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}CPNNRes;
/* DTS2013061708346 l00162999 20130620 end> */
#endif
/* DTS2013012205684 l00162999 20130122 end> */

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

/* <DTS2012082300362 ChiChao 20120821 begin */
#define MAX_OPERATOR_NUM             20
 /* DTS2012082300362 ChiChao 20120821 end> */
/*AT+COPS=?保存所有运营商信息的数据*/
typedef struct OperatorListStruct
{
    int            OperatorNumber; //上报的运营商总个数
    OperatorInfo   Operator[MAX_OPERATOR_NUM];   //上报的运营商列表，最多支持20个  /* DTS2012082300362 ChiChao 20120821 */
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
#define SYS_MODE_LTE_ONLY                "38"       /*WCDMA only*/

#define SYS_ACQ_ORDER_AUTO               "0"        /*自动*/
#define SYS_ACQ_ORDER_GSM_FIRST          "1"        /*先GSM后WCDMA*/
#define SYS_ACQ_ORDER_WCDMA_FIRST        "2"        /*先WCDMA后GSM*/
#define SYS_ACQ_ORDER_NO_CHANGE          "3"        /*无变化*/
#define CM_BAND_PREF_GSM_DCS_1800        "80"       
#define CM_BAND_PREF_GSM_EGSM_900        "100"      
#define CM_BAND_PREF_GSM_PGSM_900        "200"      
#define CM_BAND_PREF_GSM_PCS_1900        "200000"
/* <BB5D00312.ATS: jiazhida 2008-9-3 ADD BEGIN */
#define CM_BAND_PREF_WCDMA_900           "2000000000000"
/* BB5D00312.ATS: jiazhida 2008-9-3 ADD END> */

/*< AU1D00199,968,liukeke,2007-11-16 ADD BEGIN*/
#define CM_BAND_PREF_WCDMA_1900             "800000"
#define CM_BAND_PREF_WCDMA_850             "4000000"
/*AU1D00199,968,liukeke,2007-11-16 ADD END >*/

#define CM_BAND_PREF_AWS_1700               "2000000" /* DTS2011061402654:B683 CS Voice, m00100263 20110614 Modify */
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
/*modify by lijiechun 20100119*/
#if 0
#define OPERATOR_LIST                           "/3w/res/operator_list.res"
#else
#define OPERATOR_LIST                         "/etc/operator_list.res"
#endif
/*modify by lijiechun 20100119*/

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
    AT_AMSS_RES                   AtExeResult;  //执行操作的返回结果
    OperatorList         *TotalOper;    //保存测试命令返回的运营商列表
    RegistedOperator     *CurOperInfo;  //保存查询命令返回的运营商信息
    int                   Reserve;      //保留字
}NetChooseOperatorResult;

/*AT+CPIN执行结果*/
typedef struct PinMgResultCodeStruct
{
    int            CmdType;       //针对哪种AT命令的返回结果
    char           Code[20];      //PIN状态
    AT_AMSS_RES            AtExeResult;   //AT命令的执行结果
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
    AT_AMSS_RES              AtExeResult;       //AT命令的执行结果
    int              Reserve;           //保留字
}PinMgResultCodeEx;

/*<DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 begin*/
#define ROUCFG_MATCH_STR "^ROUCFG"
/*DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 end>*/
//add by w00126555 0315 clip_mode
/*AT^CLIP参数及相关结构体*/
#define CLIP_MATCH_STR "^CLIP"
#define CLIP_DTMF_MODE  "0"
#define CLIP_FSK_ITU_MODE  "10"
#define CLIP_FSK_BELL_MODE  "11"
#define CLIP_FSK_RPAS_MODE  "12"

/*AT^CLIP:命令发送格式*/
typedef struct ClipConfigurationResStruct
{
    int           CmdType;             //AT命令的类型
    char         Mode[8];             //Clip模式
    int           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}ClipConfigurationRes;

/*AT^CLIP执行操作的返回结果*/
typedef struct ClipConfigurationTypeStruct
{
    char         Mode[8];             //Clip模式
    int           Reserve;             //保留字
}ClipConfigurationType;

//add by w00126555 0315 antenna_mode
/*AT^ANTENNA参数及相关结构体*/
#define ANTENNA_MATCH_STR "^ANTENNA"
#define ANTENNA_INNER_MODE  "1"
#define ANTENNA_OUTER_MODE  "0"

/*AT^ANTENNA:命令发送格式*/
typedef struct AntennaConfigurationResStruct
{
    int           CmdType;             //AT命令的类型
    char         Mode[8];             //Antenna模式
    int           AtExeResult;         //AT命令执行结果
    int           Reserve;             //保留字
}AntennaConfigurationRes;

/*AT^ANTENNA执行操作的返回结果*/
typedef struct AntennaConfigurationTypeStruct
{
    char         Mode[8];             //Antenna模式
    int           Reserve;             //保留字
}AntennaConfigurationType;

/* <DTS2013110606509 c00159151 20131101 begin */
#ifdef SUPPORT_ATP_SIGNAL_LED_CONTROL
#define SIG_LED_OFF_STATUS   "1"     /* led关闭 */
#define SIG_LED_ON_STATUS    "0"     /* led开启 */
#define OFFLED_MATCH_STR     "^offled" 

/* 控制无线信号灯AT命令 AT^offled命令的发送格式 */
typedef struct
{
    int cmd_type;       /* AT命令的类型 */
    char off_value[8];  /* offled命令的值*/
    int at_exec_reslt;  /* AT命令执行的结果 */
    int reserve;        /* 保留字 */
}off_led_config_t;

typedef struct 
{
    char off_led_value[8];  /* offled命令的值 */
    int reserve;            /* 保留字 */
}off_led_value_t;
#endif /* SUPPORT_ATP_SIGNAL_LED_CONTROL END */
/* DTS2013110606509 c00159151 20131101 end> */

/*<AP7D04527  w00126555 20100408 add begin*/
/*AT+CNUM*/
#define PBM_NUM_SIZE 42
#define PBM_NAME_SIZE 90
typedef struct
{
    char name[PBM_NAME_SIZE];
    char num[PBM_NUM_SIZE];
    int  type;
} StCnumInfo;
/*AP7D04527  w00126555 20100408 add end>*/

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
    AT_AMSS_RES            AtExeResult;   //AT命令的执行结果
    char           Status[5];     //SIM卡状态的查询结果
    int            Reserve;       //保留字
}PinEnableResult;

/*AT+CPWD执行结果*/
typedef struct PinModifyResultCodeStruct
{
    int            CmdType;        //对应哪种AT命令的执行结果
    AT_AMSS_RES            AtExeResult;    //AT命令的执行结果
    int            Reserve;        //保留字
}PinModifyResultCode;

/*AT+CPAS的执行结果*/
typedef struct PhonseActStatusStruct
{
    int            CmdType;        //对应哪种AT命令的执行结果
    AT_AMSS_RES            AtExeResult;    //AT命令的执行结果
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
    AT_AMSS_RES            AtExeResult;   //AT命令的执行结果
    int            Reserve;       //保留字
}StR99Only;
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/

/* <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
typedef struct CvmnqStruct
{
    int            CmdType;                    /*针对哪种AT命令的返回结果*/
    char           voice_mail_number[25];      //语音信箱号码,最大支持到20个字符.
    char           nv_number[25];              //原始的号码(即从NV中读取的号码)
    AT_AMSS_RES            AtExeResult;                //AT命令的执行结果
    int            Reserve;                    //保留字
}StCvmnq;

/*服务状态变化指示^SRVST */
typedef struct{
    char  srv_status[3];
} StSrvstInfo;

/*AP7D01278.Optus SMS: liudan 2008-1-26 ADD BEGIN */
/*短信已满的主动上报.*/
typedef struct{
    char  mem_type[5];
    AT_AMSS_RES            AtExeResult;                //AT命令的执行结果

} StSmmemfull;
/* AP7D01278.Optus SMS: liudan 2008-1-26 ADD END*/
/* SIM 状态变化指示 ^SIMST */
typedef struct{
    char  sim_state[3];
    char  lock_state[3];
} StSimstInfo;

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

typedef struct               /* 系统信息 */
{
    int srv_status;
    int srv_domain;
    int roam_status;
    int sys_mode;
    int sim_state;
    int lock_state;
    int submode;
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
} StSysinfo;

/*<DTS2011060304054 zhaiqi.z00174311 20110425 delete>*/

/* <DTS2012101508427 z81004485 2012/10/12 begin  */
/*<DTS2011060304054 cell info zhaiqi.z00174311 20110425 begin*/
typedef struct CellInfoResStruct
{
    int  sys_mode;
    char cell_id[CellInfo_CellID_ParaLength];
    int  mcc;
    int  mnc;
    char lac[12];
    char rssi[32];
    char ecio[32];
    char rscp[32];
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
}CellInfoResType;
/*DTS2011060304054 cell info zhaiqi.z00174311 20110425 end>*/



typedef struct
{
    int          stat;
    int          AcT;
    char         rac[16];
    char         lac[16];
    char         ci[16];
    AT_AMSS_RES  AtExeResult;
} StCGREG;


typedef struct
{
    int          mm_state;
    int          mm_idle_substate;
    int          gmm_state;
    int          gmm_substate;
    char         currentBand[33];
    AT_AMSS_RES  AtExeResult;
} StMMGMM;


typedef struct
{
    int          fun;
    int          rst;
    AT_AMSS_RES  AtExeResult;
} StCFUN;
/* DTS2012101508427 z81004485 2012/10/12 end> */



/*Begin PN DTS2012021004614 add frequence band modify by z00185914 20120208*/
typedef struct CellInfoExResStruct
{
    int  sys_mode;
    char cell_id[CellInfo_CellID_ParaLength];
    int  mcc;
    int  mnc;
    char lac[12];
    char rssi[32];
    char ecio[32];
    char rscp[32];
	char cur_band[32];
}CellInfoExResType;
/*End PN DTS2012021004614 add frequence band modify by z00185914 20120208*/
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

typedef struct
{
    char soft[WM_VERSION_LEN];        /* 长度不大于31的字符串 */
    char hard[WM_VERSION_LEN];        /* 长度不大于31的字符串 */
}StAmssVersion;

/* 从存储器发送短信后的返回结果。 */
typedef struct {
    int mr;
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCmssRes;

/* CMSS命令的输入参数 */
typedef struct {
    int index;
    char da[SMS_NUMBER_LENGTH+3];     /* 短信接收者的号码"13801001372" 需要加引号*/
    int toda;      /* 号码类型 */
    int  Reserve;       //保留字
}StCmssPara;

/* 保存短信的返回结果 */
typedef struct {
    int  index;
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCmgwRes;

/* 删除短信的返回结果 OK/+CMS ERROR:err */


/* 删除短信的命令传入的参数 */
typedef struct {
    int  index;
    int  delflag;
    int  Reserve;       //保留字
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
}StCmgdPara;

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
 AT_AMSS_RES  AtExeResult;   //AT命令的执行结果

}StCpms;

/*<DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 begin*/
typedef struct{
 char mode[5];
 char acqorder[5];
 char band[15];
 char roam[5];
 char srvdomain[5];
 char clip[5];
 char antenna[5];
 AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
}StRoutcfgs;
/*DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 end>*/

typedef struct{
    at_cmd_result  amss_res;
    dsati_cms_err_e_type  cms_res;
    dsat_cme_error_e_type cme_res;
}StAtRes;

typedef struct{
     AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
}StCmmt;

/*短信是否已读取结果*/
typedef struct {
    int ReadTag;      //短信是否已读取标记
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int  Reserve;     //保留字
}StCmsrRes;

/* <DTS2011052400954.AT: mayang 2010-05-26 MODIFY BEGIN */
#define COMM_STR_LEN_32 32
/*设置和查询短信中心号码*/
typedef struct {
    char sca[COMM_STR_LEN_32];
    char tosca[COMM_STR_LEN_32];
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StCsca;
/* DTS2011052400954.AT: mayang 2010-05-26 MODIFY END>*/

typedef struct ScidStruct
{
    char           Code[21];      //AT^HSDPA状态
    AT_AMSS_RES            AtExeResult;   //AT命令的执行结果
}StScid;

/*设置和查询短信中心号码*/
typedef struct {
    int call_ID;
    char cDTMFEvent;
    int on_length;
    int off_length;
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int  Reserve;       //保留字
}StDTMF;

/*
typedef enum
{
    U_SUCCESS = -2
    U_FAIL = -1,
    U_BUSY,
    U_phone_failure,
    U_Operation_not_allowed,
    U_Operation_not_supported,
    U_SIM_not_inserted,
    U_SIMPIN,
    U_SIMPUK,
    U_SIMFAIL,
    U_SIM_busy,
    U_SIM_wrong,
    U_incorrect_password,
    U_SIM_PIN2_required,
    U_SIM_PUK2_required,
    U_memory_full,
    U_invalid_index,
    U_not_found,
    U_memory_failure,
    U_text_string_too_long,
    U_dial_string_too_long,
    U_invalid_characters_in_dial_string,
    U_no_network_service,    
    U_network_timeout,
    U_network_not_allowed_emergency_calls_only,
    U_unknown,
    U_service_option_temporarily_out_of_order,
    U_network_rejected_supserv_request,
    U_retry_operation,
    U_invalid_deflected_to_number,
    U_deflected_to_own_number,
    U_unknown_subscriber,
    U_service_not_available,
    U_unknown_class,
    U_unknown_network_message,
    U_call_index_error,
    U_call_state_error,
    U_sys_state_error,
    U_parameters_error,
    U_spn_file_wrong,
    U_spn_file_accessed_denied,
    U_spn_file_not_exist,
    U_another_SPN_query_operation_still_not_finished,
    U_MAX_FINAL_RESULT
}U_RET_ID;
*/
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
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
}PnnRes;

/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */
/* 补充业务 +CUSD */
#ifdef SUPPORT_ATP_USSD
/* <DTS2011092600505 shidai 20110926 Modify Begin */
#define AT_IF_INFO          "/var/at/at_if.log"
#define STR_CUSD_RESULT_LEN  (512)   /* 无线传过来的的最大长度为 512 */
#define CUSD_TYPE_LEN       (4)

/*  USSD 编码方式 */
#define USSD_DCS_7BIT    (0)
#define USSD_DCS_8BIT    (4)
#define USSD_DCS_UCS2    (8)

typedef struct
{
    char rspType[CUSD_TYPE_LEN];
    char cusdStr[STR_CUSD_RESULT_LEN];
    char codeType[CUSD_TYPE_LEN];
} StCusdInfo;
/* DTS2011092600505 shidai 20110926 Modify End> */

/* DTS2011092600505 shidai 20110926 Delete */

#endif
/* DTS2011030101068.USSD: mayang 2011-03-09 MODIFY END> */
/* <DTS2012102201180  z00159386  20121009  begin */
#ifdef SUPPORT_ATP_VOICEMAIL
typedef struct
{
    int vmactive;
    int vmindex;
} VmindInfo;
#endif /* SUPPORT_ATP_VOICEMAIL */
/* DTS2012102201180  z00159386  20121009  end> */

extern MSG_CLIENT_TYPE_EX MSG_CLIENT_TYPE;

#define MODEM_TIME_STRING_LENGTH 50 /* DTS2012072703919 j81004134 2012/8/16 */
void GetAtSrvId(void);
int ReadDataFromAtSrvProc(msg_q *msg_buf);
int GetDataFromAtServer(msg_q *msg_buf);

int AT_SysConfig (int CmdType, NetConfigurationType *Param, NetConfigurationRes *Result);
int AT_ChoseOperators(int CmdType, 
                              NetChooseOperatorParam *Param, 
                              NetChooseOperatorResult *Result);
int AT_PinManagement (int CmdType, 
                               char*Pin, 
                               char *NewPin, 
                               PinMgResultCode *Result);
int AT_PinManagementEx (int CmdType,
                                  char*Pin, 
                                  char *NewPin, 
                                  PinMgResultCodeEx *Result);
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
/* <DTS2012072605581 liukai 20120806 在Advanced Settings中无法切换Clip Mode begin */
int AT_Clcc();
/* DTS2012072605581 liukai 20120806 在Advanced Settings中无法切换Clip Mode end> */
int AT_SysInfoForXml(StSysinfo *Result,char *at_report_str);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */


/* <DTS2012101508427 z81004485 2012/10/12 begin  */
int AT_CFUN(StCFUN *Result);
int AT_MMGMM(StMMGMM *Result);
int AT_CGREG(StCGREG *Result);
int AT_CellInfo(CellInfoResType *Result);
/* DTS2012101508427 z81004485 2012/10/12 end> */


/*<DTS2010051200134 l00101002 20100517 Add begin*/
typedef struct               /* 系统信息 */
{
    int version;
    int pin_correct;
    int iccid_correct;
    int ecc_check;
    int lock_style;
    int total_num;
    int num_been_locked;
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
} StLockuim2ue;
int AT_Lockuim2ue(StLockuim2ue *Result);
/*DTS2010051200134 l00101002 20100517 Add end>*/ 
/* <DTS2011080101802: gKF34687 2011-8-1 delete 8 lines> */
/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
/* DTS2013061803625 z81004485 Coverity 2013/6/18 */
#if 0
int char_to_hex(char *ch);
int get_hex_from_str(char *s);
int get_srcstr_length(char *pchar, int *para1, unsigned short *para2, unsigned char *para3);
int conv_to_utf8(char *pdst_str, int counte_bytes, 
                                unsigned short byte_one, unsigned char utf_one);
int gsm7_to_ascii(const unsigned char *puszGSM_7Data, int InputLen, 
                                     unsigned char *puszASCIIData, int BufLen, int *OutputLen);
int decode_7bit(const unsigned char *puszSrc, unsigned char *puszDst, int iSrcLength);
int conv_name_to_ascii(char *pszName, char *pnn_name);
int conv_name_to_utf8(char *pszName, char *pnn_name);
int parse_pnn_name( char *rcv_buf, PnnRes *pnn_name );
#endif
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

/* <AP7D010YY.Optus SMS: liudan 2007-12-1 ADD BEGIN */
/* 保存短信 */
int AT_Cmgw (StCmgsInfo* sms_pdu, StCmgwRes* cmgw_result);
/* 从sm中发送短信 */
int AT_Cmss (StCmssPara* cmss_para, StCmssRes* cmss_result);/*+CMSS*/
/* 删除 */
int AT_Cmgd (int cmd_type, StCmgdPara* cmgd_para);

/* ATS使用此函数来完成初始化 */
int AT_Cmsr (int cmd_type, int ReadTag, StCmsrRes* cmsr_res);
/*  */
int AT_Csca( int cmd_type, StCsca* csca_para, StCsca* csca_res);

/* 读取各种状态下的短信index的列表。 
ATS使用此函数后，完成逐条读取
HTTPD使用此函数，查询是否有未读短信*/
int AT_Cmgi (int state, int* sms_list,int list_size, int* list_length);
/* Optus SMS: liudan 2008-1-15 ADD BEGIN*/
int AT_Cmgi_unread_sms (int* sms_list,unsigned int list_size, int* list_length);
/* Optus SMS: liudan 2008-1-15 ADD END*/
/* 手动修改短信状态 */
int AT_Cmmt (int index, int state,StCmmt *result);
/* 查询sm卡可支持多少条, 已有多少条,*/
int AT_Cpms(int cmd_type, StCpms* cpms_para, StCpms* cpms_result);
/*AP7D01278.Optus SMS: liudan 2008-1-26 ADD BEGIN */
/*<DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 begin*/
int AT_Roucfg(int cmd_type, char* roucfg_para, StRoutcfgs* roucfg_res);
/*DTS2010051300332:路由不能导入clip配置 by l129990,2010-7-2 end>*/
/*查询主动上报.*/
int AT_smmemfull(StSmmemfull *sm_full_res);
int AT_Rssi(StRssiInfo *rssi_res);
int AT_Hwver(StAmssVersion* ver_res);
int AT_Dsflowrpt(StDsflowrptInfo *flow_res);
int AT_Cgmr(StAmssVersion* ver_res);
/* AP7D01278.Optus SMS: liudan 2008-1-26 ADD END*/
/*解析cops信息*/
int AT_check_oper_name(NetChooseOperatorResult *Result);
int parse_operator_data(const char *psz_operator_data, char *psz_operator_short_name, char *psz_operator_full_name );     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
int get_operator_name(const char *psz_operator_numeric_name, char *psz_operator_short_name, char *psz_operator_full_name );     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */

/* Optus SMS: liudan 2007-12-1 ADD END> */
/*AP7D01522.SMS: liudan 2008-3-24 ADD BEGIN */
int AT_Cmgs (StCmgsInfo* sms_pdu, StCmssRes* cmgs_result);
/* AP7D01522.SMS: liudan 2008-3-24 ADD END*/

/* <DTS2011103000139 jiaxinghua 20111030 pnn parse delete> */
/*int decode_7bit(const unsigned char *puszSrc, unsigned char *puszDst, int iSrcLength);//
int gsm7_to_ascii(const unsigned char *puszGSM_7Data, int InputLen, 
                                     unsigned char *puszASCIIData, int BufLen, int *OutputLen); */
/* <DTS2011103000139 jiaxinghua 20111030 pnn parse delete Begin */
#if 0
int conv_to_utf8(char *pdst_str, int counte_bytes, 
                                unsigned short byte_one, unsigned char utf_one);

int get_srcstr_length(char *pchar, int *para1, unsigned short *para2, unsigned char *para3);
#endif
/* DTS2011103000139 jiaxinghua 20111030 pnn parse delete End> */
const char * ATP_ATSHARED_GetVersion(void);

//int get_hex_from_str(char *s); //
//int char_to_hex(char *ch);//
void split_space_opl(char *start, char key);
 AT_AMSS_RES Error_str_to_id(char *at_report_str);
 int set_network_config( char* rat );
int AT_DTMF(StDTMF *stDtmf);
int AT_SCID(StScid *result);
int AT_SavePIN(const char *pszPinCode);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
int atsSendAtcmd(msg_q *msg_buf);
int atcmdSendCmd(int iCmdType, int iPara1, char* pcPara2);

//add by w00126555 0315 clip_mode函数声明
int AT_ClipConfig(int CmdType, ClipConfigurationType *Param, ClipConfigurationRes *Result);
int AT_AntennaConfig(int CmdType, 	AntennaConfigurationType *Param, AntennaConfigurationRes *Result);
/*<AP7D04527  w00126555 20100408 add begin*/
int AT_cnum(StCnumInfo *Result);
/*AP7D04527  w00126555 20100408 add end>*/

/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */
#ifdef SUPPORT_ATP_USSD
/*发送AT+CUSD命令*/
int AT_Cusd_Msg_Send (int CmdType, char *rspType, char *cusdStr, char *codeType);
/*查询补充业务主动上报结果*/
int AT_Cusd(StCusdInfo *cusd_result);
/* <DTS2011092600505 shidai 20110926 Modify Begin */
int USSD_decoder(const char * ussd_strtype);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */

/* DTS2011092600505 shidai 20110926 Modify End> */
#endif
/* DTS2011030101068.USSD: mayang 2011-03-09 MODIFY END> */
/* <DTS2012102201180  z00159386  20121009  begin */
#ifdef SUPPORT_ATP_VOICEMAIL
int AT_VMIND(VmindInfo *vmind_result);
int AT_VMCHK(VmindInfo *vmind_result);
#endif /* SUPPORT_ATP_VOICEMAIL */
/* DTS2012102201180  z00159386  20121009  end> */
 
int AT_Cnmi(int report_enable, int *pat_cmd_result); /* DTS2012071205115 j81004134 2012/7/13 */     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
int AT_ModemTime(char *time_str); /* DTS2012072703919 j81004134 2012/8/15 */
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
int ats_query_modemtime(const StSysinfo *pstSysinfo, int *retCode);
int AT_LockFile(const char *pszFileName, FILE ** fFile);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
void AT_UnlockFile(int iLockFd);
/* DTS2013012602546 z81004485 2013/1/28 end> */
/* DTS2012110602724 l00162999 20121117 end> */
/* <DTS2012081504862 liukai 20120816 路由端添加设置时区模式 begin */
typedef struct
{
    int           mode;      
    int           timezone;
    AT_AMSS_RES   AtExeResult;   //AT命令的执行结果
}CtzuConfigurationType;

/* DTS2013061803625 z81004485 Coverity 2013/6/18 */
/* DTS2012081504862 liukai 20120816 路由端添加设置时区模式 end> */

int AT_CheckSmsReady(int *status); /* DTS2012082301238 j81004134 2012/8/23 */
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013101501273 z81004485 2013/10/15 delete 1 line> */
typedef struct
{
    int     simOptType;
    char    simlockpwd[9];
    int     OptState;
    int     remaintimes;  
    AT_AMSS_RES  AtExeResult;   //AT命令的执行结果
    int     Reserve;       //保留字
} simlockinfo;

int AT_SimLockOp(int CmdType, simlockinfo *Param);
/* <DTS2013020603354  z00159386  20130205  begin */
/* <DTS2013101501273 z81004485 2013/10/15 delete 1 line> */
/* DTS2012110602724 l00162999 20121117 end> */
#ifdef SUPPORT_ATP_APNATTACHED
/* <DTS2012123101571  z00159386  20130109  begin */
int AT_CGDCONT(char *apn, int *at_result);
int AT_SWITCHPROFILE(int *at_result);
/* DTS2012123101571  z00159386  20130109  end> */
#endif /* SUPPORT_ATP_APNATTACHED */
/* DTS2013020603354  z00159386  20130205  end> */

/* <DTS2013021907887  l00162999 20130311 begin */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
int AT_SysConfigEx (int CmdType, NetCfgExSet *Param, NetCfgExRes *Result);
int AT_Locinfo (int CmdType, LocinfoRes *Result);
int AT_Lcellinfo (int CmdType, LcellinfoSet *Param, LcellinfoRes *Result);
int AT_CecellId (int CmdType, CecellIdRes *Result);
#endif
/* DTS2013021907887  l00162999 20130311 end> */
int AT_ICCID(StScid *result); /* DTS2013050301791 z81004143 2013/3/22 */


unsigned int filter_special_char_for_web(char *buf); /* DTS2013042405892 j81004134 2013/5/2 */
/* <DTS2013090701129 j81004134 2013/9/14 begin */
#ifdef SUPPORT_ATP_FMC_DECT
int AT_RTCUPDATE(char *op_name, unsigned int op_name_len);
#endif
/* DTS2013090701129 j81004134 2013/9/14 end> */

/* <DTS2013110606509 c00159151 20131105 begin */
#ifdef SUPPORT_ATP_SIGNAL_LED_CONTROL
int AT_Offled(int CmdType, off_led_value_t *value_ptr, off_led_config_t *config_ptr);
#endif /* SUPPORT_ATP_SIGNAL_LED_CONTROL END */
/* DTS2013110606509 c00159151 20131105 end> */
#endif

/* <DTS2013030604697 j81004134 2013/3/7 begin */
#define NV_OPT_FOREVER                    1
#define NV_OPT_FORONCE                    0
int AT_ERROPT(int restart_opt, int nv_opt);
/* DTS2013030604697 j81004134 2013/3/7 end> */
/* <DTS2013061708346 l00162999 20130620 begin */
#if defined(CONFIG_B881_PRODUCT) || defined(CONFIG_B880_PRODUCT)
#define CPNN_FILE_NAME "/var/cpnn"  /* DTS2013072407951 l00162999 20130731 */
/*
0	关闭状态灯
1	开启状态灯，此时状态为关闭之前的状态
2	拨号成功
3	拨号失败，注意：不是未拨上号
4	手动拨号模式下，路由有流量时，通知无线闪灯
5	断开拨号
*/
#define LED_MODE_STATUSLED_OFF   0
#define LED_MODE_STATUSLED_ON    1
#define LED_MODE_DIAL_CONNECT    2
#define LED_MODE_DIAL_FAIL       3
#define LED_MODE_DIAL_MANUALTIP  4
#define LED_MODE_DIAL_DISCONNECT 5
int AT_AmssLed(unsigned int mode);
int AT_Cpnn(CPNNRes *cpnn_res, int buf_len, int card_type);
#endif
/* DTS2013061708346 l00162999 20130620 end> */
/*<CODE.AT_CLIENT: chengangyi 2007-3-29 ADD END>*/
