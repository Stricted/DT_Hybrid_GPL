/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : voicepub.h
  版 本 号   : 初稿
  作    者   : l110333
  生成日期   : 2009年5月8日
  最近修改   :
  功能描述   : 语音业务模块与外部模块交互接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年5月8日
    作    者   : l110333
    修改内容   : 创建文件

******************************************************************************/
#ifndef __VOICEPUB_H__
#define __VOICEPUB_H__


#include "atptypes.h"
#include "atputil.h"
#include "voicedb.h"

/********************    BEGIN: common_define   ********************/
#define VCE_STR_LEN_2               2
#define VCE_STR_LEN_4               4
#define VCE_STR_LEN_8               8
#define VCE_STR_LEN_16              16
#define VCE_STR_LEN_32              32
#define VCE_STR_LEN_64              64
#define VCE_STR_LEN_128             128
#define VCE_STR_LEN_256             256
#define VCE_STR_LEN_512             512
#define VCE_STR_LEN_1K              1024
#define VCE_STR_LEN_2K              (2*1024)
#define VCE_STR_LEN_4K              (4*1024)
#define VCE_STR_LEN_8K              (8*1024)
#define VCE_STR_LEN_16K             (16*1024)
#define VCE_STR_LEN_32K             (32*1024)
#define VCE_STR_LEN_64K             (64*1024)
#define VCE_STR_LEN_128K            (128*1024)
#define VCE_STR_LEN_256K            (256*1024)
#define VCE_STR_LEN_512K            (512*1024)
/********************    END: common_define   ********************/




/*************************  BEGIN: VOICE_LIB  *************************/
#define ATP_VOICE_PID_PATH          "/var/voice"
#define ATP_VOICE_MSGKEY_FLAG       'V'
#define ATP_VOICE_MSG_LEN           256
#define ATP_VOICE_EVENT_LEN         128

/*帐号支持能力*/
#ifdef SUPPORT_ATP_VCE_WT104_CUSTOM             
#define VCE_VOIP_MAX                30      /*最多支持的VoIP帐号个数*/
#else
#define VCE_VOIP_MAX                15      /*最多支持的VoIP帐号个数*/
#endif
#define VCE_ISDN_MAX                10      /*最多支持的ISDN帐号个数*/
#define VCE_PSTN_MAX                1       /*最多支持的PSTN帐号个数*/
#define VCE_CS_MAX                  1       /*最多支持的CS帐号个数*/
#define VCE_LINE_MAX (VCE_VOIP_MAX + VCE_ISDN_MAX + VCE_PSTN_MAX + VCE_CS_MAX) /*最多支持的帐号个数*/

/*端口支持能力*/
#define VCE_FXS_MAX                 2       /*最多支持FXS口的个数*/
#define VCE_DECT_HS_MAX             5       /*最多支持DECT子机的个数*/
#define VCE_PORT_MAX (VCE_FXS_MAX + VCE_DECT_HS_MAX)    /*最多支持端口的条目*/


/*账号索引基线值*/
#define VCE_VOIP_BASE           1                                   /*VOIP序号起始值*/
#define VCE_ISDN_BASE           (VCE_VOIP_BASE + VCE_VOIP_MAX)      /*ISDN序号起始值*/
#define VCE_PSTN_BASE           (VCE_ISDN_BASE + VCE_ISDN_MAX + 1)  /*PSTN序号起始值,后面加1是加ISDN空帐号*/
#define VCE_CS_BASE             (VCE_PSTN_BASE + VCE_PSTN_MAX)      /*CS序号起始值*/
#define VCE_FXS_INCALL_BASE     (VCE_CS_BASE + VCE_CS_MAX)          /*FXS内部呼叫账号序号起始值*/
#define VCE_DECT_INCALL_BASE    (VCE_FXS_INCALL_BASE + VCE_FXS_MAX) /*DECT内部呼叫账号序号起始值*/

/*ISDN空帐号索引,该帐号用于当没有MSN号码时ISDN主被叫可用*/
#define VCE_ISDN_DFT    (VCE_ISDN_BASE + VCE_ISDN_MAX)              /*ISDN默认顺序号(无MSN号码的情况)*/

/*呼出为Auto时Dect传递的账号索引*/
#define VCE_OUTGO_AUTO  (VCE_DECT_INCALL_BASE + VCE_DECT_HS_MAX)    /*取一个较大值作为自动选路索引*/

/*Dect LineSetting中特殊账号名称*/
#define VCE_ISDN_NAME               "ISDN"
#define VCE_PSTN_NAME               "PSTN"
#define VCE_AUTO_NAME               "auto"
#define VCE_CS_NAME                 "CS"
#define RSS_CHANNEL_NAME_WE         "Wetter"
#define RSS_CHANNEL_NAME_NA         "Nachrichten"
#define RSS_CHANNEL_NAME_WI         "Wirtschaft"
#define RSS_CHANNEL_NAME_CO         "Computer"
#define RSS_CHANNEL_NAME_SP         "Sport"
#define RSS_CHANNEL_NAME_UN         "Unterhaltung"
#define RSS_CHANNEL_NAME_BU         "Bundesliga"

#define RSS_CHANNEL_URL_WE          "http://feeds.t-online.de/rss/wetter"
#define RSS_CHANNEL_URL_NA          "http://feeds.t-online.de/rss/nachrichten"
#define RSS_CHANNEL_URL_WI          "http://feeds.t-online.de/rss/wirtschaft"
#define RSS_CHANNEL_URL_CO          "http://feeds.t-online.de/rss/computer"
#define RSS_CHANNEL_URL_SP          "http://feeds.t-online.de/rss/sport"
#define RSS_CHANNEL_URL_UN          "http://feeds.t-online.de/rss/unterhaltung"
#define RSS_CHANNEL_URL_BU          "http://feeds.t-online.de/rss/bundesliga"

#define VCE_PHONE_BOOK_MAX          250     /*最多支持电话本记录条目*/
#define VCE_MISSED_CALL_MAX         100     /*最多支持未接来电记录*/
#define VCE_INACC_CALL_MAX          100     /*最多支持已接来电记录*/
#define VCE_OUT_CALL_MAX			100     /*最多支持呼出来电记录*/
#define VCE_LINE_SRV_MAX            5       /*一个sip帐号最多支持可配置的注册服务器个数*/
#define VCE_CODEC_MAX               3       /*编码类型的个数*/
#define VCE_PIN_LEN_MAX             32      /*PIN码最大长度*/

#define SIP_MAX_NUM_LEN             64      /*用户号码最大长度*/
#define SIP_MAX_NAME_LEN          (64+4)    /*用户姓名最大长度*/
#define SIP_MAX_PSW_LEN           (64+4)    /*用户密码最大长度*/
#define SIP_MAX_URL_LEN             128     /*SIP URL最大长度 */

#define SIP_UUID_MAX_LEN            40
#define SIP_LINE_STATE_SESS_MAX     2
#define SIP_LINE_STATE_READEAY      0x01020304
#define SIP_LINE_STATE_UNREADY      0x0
#define SIP_QUERY_TIME              10
#define SIP_SESSION_QUERY_TIME      60
#define SIP_RESTE_TIME              2

#define DECT_EEPROM_VERSION_LEN     4
#define DECT_EEPROM_VERSION_STRLEN  DECT_EEPROM_VERSION_LEN * 2
#define DECT_EEPROM_SUBSDATA_LEN    200
#define DECT_EEPROM_SUBSDATA_STRLEN DECT_EEPROM_SUBSDATA_LEN * 2

#define DECT_CMO_SUBSDATA_SIGN_LEN  1
#define DECT_CMO_SUBSDATA_TOTAL_LEN DECT_CMO_SUBSDATA_SIGN_LEN + DECT_EEPROM_VERSION_STRLEN + DECT_EEPROM_SUBSDATA_STRLEN + 1
#define DECT_CMO_SUBSDATA_MAX_LEN   512

#define DECT_UPDATA_SIGN_TRUE       '1'

#define DECT_PCM_TEST_DTMF          5
#define DECT_PCM_TEST_UE            2

#define VOICE_CODECLIST_MAX         3          /*最多支持编码方式个数*/

/*BEGIN::added by l00192646 ,2012/06/04 for wt-104 extersion*/
#define VOICE_ALERT_TIMEOUT          5     /*Alert 振铃超时定时器的时间(秒)*/
/*END::added by l00192646 ,2012/06/04 for wt-104 extersion*/

/*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
#define VOICE_CALL_MAX         5          /*最多支持通话个数*/
/*END: Added by l00180792 @20120627 for wt-104 calllog*/

/*BEGIN:Added by l00180792 @20120717 for QI*/
#define VOICE_DFT_QICALC_INTERVAL           10000       /* 默认获取QI的定时器时长*/
/*BEGIN:Added by l00180792 @20120717 for QI*/

/*DTMF数据传输类型*/
typedef enum tagDTMF_TYPE_E
{
    DTMF_TYPE_InBand = 0,  
    DTMF_TYPE_RFC2833, 
    DTMF_TYPE_SIPInfo,
}DTMF_TYPE_E;

typedef enum VOICE_MSG_TYPE
{
    /*语音进程内消息类型定义*/
    VOICE_MSG_TYPE_TAPI = 8840,   	/*tapi消息*/
    VOICE_MSG_TYPE_TIMER,         	/*timer消息*/
    VOICE_MSG_TYPE_TPTD,          	/*tptd消息*/
    VOICE_MSG_TYPE_SC,				/*SC消息*/
    VOICE_MSG_TYPE_CS,				/*CS消息*/
    VOICE_MSG_TYPE_PM,				/*PM消息*/
    VOICE_MSG_TYPE_MIC,             /*MIC消息*/
#ifdef SUPPORT_DIAG_POTS_MNG
    VOICE_MSG_TYPE_DIAG,            /*DIAG消息*/
#endif
    VOICE_MSG_TYPE_LINEMNG,
    VOICE_MSG_TYPE_ISDN,

    /*语音进程外消息类型定义*/
    VOICE_MSG_TYPE_CLI = 8900,      /*CLI消息*/
    VOICE_MSG_TYPE_CMS,           	/*配置管理消息*/
    VOICE_MSG_TYPE_WEB,           	/*配置管理消息*/
    VOICE_MSG_TYPE_REBOOT,         	/*web进程消息,如修改需同步更新ATP_VOICE_MSG_REBOOT*/
    VOICE_MSG_TYPE_STOP,            /*web进程消息,如修改需同步更新ATP_VOICE_MSG_STOP*/
    /*begin: added by l110333 for log*/
    VOICE_MSG_TYPE_WAN,             /*wan进程消息*/
    /*end: added by l110333 for log*/
    VOICE_MSG_TYPE_RESTORE,         /*恢复出厂配置消息*/
    VOICE_MSG_TYPE_ATS,             /*ATS反馈或上报给Voice消息*/

    VOICE_MSG_TYPE_BUTT
} VOICE_MSG_TYPE_E;

/*来自CLI的消息类型*/
typedef enum VOICE_CLI_MSG_TYPE_EN_
{
    VOICE_CLI_MSG_START = 1,   	    /*启动语音*/
    VOICE_CLI_MSG_STOP,	            /*终止语音*/
    VOICE_CLI_MSG_SIP_REG,        	/*注册sip账号*/
    VOICE_CLI_MSG_SIP_UNREG,        /*注销sip账号*/
    VOICE_CLI_MSG_SIP_REG_REPORTOK, /*不管SIP上行账号状态，都将其设成注册成功*/
    VOICE_CLI_MSG_VOICE_TAPI,
    VOICE_CLI_MSG_VOICE_LINEMNG,
    VOICE_CLI_MSG_VOICE_SC,
    VOICE_CLI_MSG_VOICE_DPLAN,
    VOICE_CLI_MSG_VOICE_PSTN,
    VOICE_CLI_MSG_VOICE_SIP,
    VOICE_CLI_MSG_VOICE_CUSTOMER,
    VOICE_CLI_MSG_VOICE_DECT,
    VOICE_CLI_MSG_QUERY_CFG,        /*查询语音业务配置信息*/
    VOICE_CLI_MSG_QUERY_TAPI_CFG,   /*查询tapi配置信息*/
    VOICE_CLI_MSG_CS_STAT,          /*查询CS注册和呼叫状态*/
    VOICE_CLI_MSG_QUERY_SIPID,      /*查询SIP帐号信息*/
    VOICE_CLI_MSG_QUERY_SIPSRV,     /*查询SIP服务器信息*/
    VOICE_CLI_MSG_QUERY_SIP_STAT, 	/*查询sip呼叫状态和注册状态*/
    VOICE_CLI_MSG_DBG_LEVEL,      	/*设置日志级别*/
    VOICE_CLI_MSG_DBG_TELNET,      	/*telnet输出调试信息*/
    VOICE_CLI_MSG_DBG_BR0,      	/*BR0上行*/
    VOICE_CLI_MSG_QUERY_REGINFO,	/*查询注册信息*/
    VOICE_CLI_MSG_DBG_CONSOLE,      /*console输出信息*/
    VOICE_CLI_MSG_UNKNOWN,

    VOICE_CLI_MSG_BUTT
}VOICE_CLI_MSG_TYPE_E;

/* 配置管理发送的指示 */
typedef enum VOICE_CMS_MSG_TYPE
{
    VOICE_CMS_MSG_SIP_REG,        	/*注册sip帐号*/
    VOICE_CMS_MSG_SIP_UNREG,        /*注销sip账号*/
    VOICE_CMS_MSG_WAN_STAT,       	/*WAN状态报告*/
    VOICE_CMS_MSG_UPDATE_CFG,     	/*更新了配置*/
    VOICE_CMS_MSG_EQUIPMENT_TSET,   /*装备测试*/
    VOICE_CMS_MSG_QUERY_SIP_STAT, 	/*配置管理消息,查询sip呼叫状态和注册状态*/
    VOICE_CMS_MSG_DBG_LEVEL,      	/*配置管理消息,设置日志级别*/
    VOICE_CMS_MSG_DBG_TELNET,      	/*配置管理消息,串口输出调试信息*/
    VOICE_CMS_MSG_QUERY_REGINFO,	/*查询注册信息*/
    VOICE_CMS_MSG_OUTLINE_CHANGED,  /*通知语音进程外线状态变化 */
    VOICE_CMS_MSG_FXO_RELAYCTL,     /*FXO继电器控制*/
    VOICE_CMS_MSG_DIAGBUTTON,       /*诊断按钮消息*/
    VOICE_CMS_MSG_LINERESET,        /*SIP账号重启*/
    VOICE_CMS_MSG_STATUSQUERY,      /*清空帐号配置状态信息*/
    VOICE_CMS_MSG_CALLSTATUS_QUERY, /*线路呼叫状态查询*/
    VOICE_CMS_MSG_ANIMSN,           /*获取msn账号消息*/
    VOICE_CMS_MSG_WAN_WILLDOWN,     /*通知语音进程wan即将down*/ 
    /*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
    VOICE_CMS_MSG_UPDATE_CALLLOGPARAM, /*更新当前通话时间*/
    /*END: Added by l00180792 @20120627 for wt-104 calllog*/
    VOICE_CMS_MSG_FXSSTATUSQUERY,
    /************** DECT START *****************/
    VOICE_CMS_MSG_DECT_RESTORE = 0x80, /*Dect状态更新(注册/注销引起的变化)*/
    VOICE_CMS_MSG_DECT_NAME,        /*Dect子机名称更新*/
    VOICE_CMS_MSG_DECT_LINESET,     /*Dect子机LineSettings更新*/ 
    VOICE_CMS_MSG_DECT_PAGE,        /*Dect子机查找*/
    VOICE_CMS_MSG_DECT_REGOPEN,     /*DECT允许子机注册*/
    VOICE_CMS_MSG_DECT_REGCLOSE,    /*DECT禁止子机注册*/
    VOICE_CMS_MSG_DECT_HS_REG,      /*DECT子机注册事件*/
    VOICE_CMS_MSG_DECT_HS_UNREG,    /*DECT子机注销事件*/
	/*********************h00190880************************/	
    VOICE_CMS_MSG_DECT_RFSET,       /*DECT射频开关*/
    VOICE_CMS_MSG_DECT_ANT0_TXSET,  /*天线0设置事件*/
    VOICE_CMS_MSG_DECT_ANT1_TXSET,  /*天线1设置事件*/
    VOICE_CMS_MSG_DECT_ANT0_TXDISPLAY,
    VOICE_CMS_MSG_DECT_ANT1_TXDISPLAY,
    VOICE_CMS_MSG_DECT_SETRGIQM,
    VOICE_CMS_MSG_DECT_GETRGIQM,
    VOICE_CMS_MSG_DECT_GETREGHS, 	/*DECT 获取子机列表信息*/
	/*********************h00190880************************/	
    VOICE_CMS_MSG_DECT_UARTSET,     /*DECT UART开关*/
	/*end  of add by xkf38924,2011-05-31*/
    VOICE_CMS_MSG_DECT_SETNEMO,     /*DECT NEMO 模式设置*/
    VOICE_CMS_MSG_DECT_SETECO,      /*DECT ECO模式,射频调节设置*/
    /* START ADD: x182424 2011-10-28 FOR DTS2011111002760 */
    VOICE_CMS_MSG_DECT_SETPCM,      /*PCM测试模式*/
    VOICE_CMS_MSG_DECT_PCMTEST,     /* pcm连通性测试 */
    /* END ADD: x182424 2011-10-28 FOR DTS2011111002760 */
    VOICE_CMS_MSG_DECT_SETTIME,     /*DECT 时间同步*/
    VOICE_CMS_MSG_DECT_SETPIN,      /*DECT PIN设置*/
    VOICE_CMS_MSG_DECT_GETPIN,      /*DECT PIN获取*/
    VOICE_CMS_MSG_DECT_SETCALIB,    /*DECT 装备参数设置(RVBG,RVREF,RXTUN,RFPI)*/
    VOICE_CMS_MSG_DECT_GETCALIB,    /*DECT 装备参数获取(RVBG,RVREF,RXTUN,RFPI)*/
    VICCE_CMS_MSG_DECT_GETVER,      /*DECT 版本号获取*/
    VOICE_CMS_MSG_DECT_TESTMODE,    /*DECT 测试模式设置*/
    VOICE_CMS_MSG_DECT_EEPROMSET,   /*DECT 手工写EEPROM*/
    VOICE_CMS_MSG_DECT_REBOOT,      /*DECT 重启*/
    VOICE_CMS_MSG_DECT_TELBOOK,     /*DECT 电话本配置更新*/
    VOICE_CMS_MSG_DECT_UPDATE,      /*DECT 升级*/
    VOICE_CMS_MSG_DECT_RELOCK,      /*DECT RELOCK到Base*/
    VOICE_CMS_MSG_DECT_REINIT,      /*DECT 重新初始化*/
    VOICE_CMS_MSG_DECT_LOG,         /*DECT LOG*/    
    VOICE_CMS_MSG_DECT_EEPROMGET,   /*DECT 手工读EEPROM*/
    VOICE_CMS_MSG_DECT_GETSUBINFO,  /*DECT 获取注册信息*/
    VOICE_CMS_MSG_DECT_SETSUBINFO,  /*DECT 设置注册信息*/
    VOICE_CMS_MSG_DECT_GETBARCODE,  /*DECT 获取装备信息*/
    VOICE_CMS_MSG_DECT_SETBARCODE,  /*DECT 设置装备信息*/

    VOICE_CMS_MSG_DECT_GETINITSTAT, /*DECT 获取初始化状态*/
    VOICE_CMS_MSG_DECT_MISSCALLCHANGE,
	VOICE_CMS_MSG_DECT_LIST_CHANGE,
	VOICE_CMS_MSG_DECT_SETMWI,      /*DECT 留言灯设置*/
	VOICE_CMS_MSG_DECT_SETCMBSLINETYPE,    /*设置CMBS Line Type,满足认证要求*/
	VOICE_CMS_MSG_DECT_RESETALL,
	VOICE_CMS_MSG_DECT_CMSUPDATEMISS,
	VOICE_WEB_MSG_DECT_WEBUPDATEMISS,
	VOICE_CMS_MSG_DECT_CONTACTSNOTIFY,
	VOICE_CMS_MSG_DECT_NETPHONEBOOK_RESULT,  /*网络侧对Net Phone book 请求返回的结果*/
	
	VOICE_CMS_MSG_DECT_GETMAILRES,
	VOICE_CMS_MSG_DECT_NEWMAILNOTIFY,
	VOICE_CMS_MSG_DECT_NEWRSSNOTIFY,
	VOICE_CMS_MSG_DECT_READCHANNELEND,
	VOICE_CMS_MSG_DECT_CHANGELISTRSS,
	VOICE_CMS_MSG_DECT_READCHANNELERR,
	VOICE_CMS_MSG_DECT_GETMAILERR,
	/*add by w00191600  2011-10-26  for 子机RSS\Email绑定关系发生变化时下发未读信息*/
	VOICE_CMS_MSG_DECT_MODIFYCHANNELRELATE,
	VOICE_CMS_MSG_DECT_MODIFYACCOUNTRELATE,
	/*end add by w00191600  2011-10-26  for 子机RSS\Email绑定关系发生变化时下发未读信息*/
	VOICE_CMS_MSG_DECT_RECORD_CALLINFO,
	VOICE_CMS_MSG_DECT_OPERAORSNAME,
    /************** DECT END *****************/
    VOICE_CMS_MSG_DECT_CATIQMODE,
    VOICE_CMS_MSG_DECT_ENCRYPTION,
    VOICE_CMS_MSG_UNKNOWN = 0xff,

} VOICE_CMS_MSG_EN;

typedef enum VOICE_ATS_MSG_TYPE
{
	VOICE_ATS_MSG_ATA_RSP            = 1,      /*下发的ATA指令反馈*/
	VOICE_ATS_MSG_CHUP_RSP           = 2,      /*下发的CHUP指令反馈*/
    VOICE_ATS_MSG_ATD_RSP            = 3,      /*下发的ATD指令反馈*/
    VOICE_ATS_MSG_DDSETEX_RSP        = 4,      /*下发的DDSETEX指令反馈*/ 
	VOICE_ATS_MSG_CHLD_RSP           = 5,      /*下发的CHLD指令反馈*/
	VOICE_ATS_MSG_CLIP_RSP           = 6,      /*下发的CLIP指令反馈*/
	VOICE_ATS_MSG_DTMF_RSP           = 7,      /*下发的DTMF指令反馈*/
	VOICE_ATS_MSG_CW_RSP             = 8,      /*下发的CW指令反馈*/
	
	VOICE_ATS_MSG_ORIG_REPORT        = 9,      /*ORIG指示上报*/
	VOICE_ATS_MSG_CLIP_REPORT        = 10,     /*CLIP指示上报*/
	VOICE_ATS_MSG_RING_REPORT        = 11,     /*RING指示上报*/  
	VOICE_ATS_MSG_CONF_REPORT        = 12,     /*CONF指示上报*/  
	VOICE_ATS_MSG_CONN_REPORT        = 13,     /*CONN指示上报*/ 
	VOICE_ATS_MSG_CEND_REPORT        = 14,     /*CEND指示上报*/
	
	VOICE_ATS_MSG_SYSINFO_REPORT     = 15,     /*SYSINFO指示上报*/
	VOICE_ATS_MSG_SIMSTATUS_REPORT   = 16,     /*SIMSTATUS指示上报*/
	VOICE_ATS_MSG_CSSI_REPORT        = 17,     /*CSSI指示上报*/
	VOICE_ATS_MSG_CSSU_REPORT        = 18,     /*CSSU指示上报*/ 
	VOICE_ATS_MSG_CCWA_REPORT        = 19,     /*CCWA指示上报*/

    VOICE_ATS_MSG_USB_STATUS_UPDATE  = 20,
	
	VOICE_ATS_MSG_UNKNOWN = 0xff,
}VOICE_ATS_MSG_EN;

/* WAN子消息 */
typedef enum VOICE_WAN_MSG_TYPE
{
    ATP_UTIL_PPP_UP_MANUAL = 0,   // 手动建立连接成功
    ATP_UTIL_PPP_UP_AUTO = 1,      // 自动建立连接成功
    ATP_UTIL_PPP_UP_DEMAND = 2,   // 按需拨号建立连接成功
    ATP_UTIL_PPP_UP_VOICE = 3,     // VOIP摘机触发建立连接成功
    ATP_UTIL_PPP_DOWN_DSL = 4,   // DSL链路失败导致连接断开
    ATP_UTIL_PPP_DOWN_ECHO = 5,   // PPP ECHO请求无应答导致连接断开
    ATP_UTIL_PPP_DOWN_TIMEOUT = 6,  // 按需拨号超时无流量导致连接断开
    ATP_UTIL_PPP_FAIL_SVR = 7,  // BAS无响应导致拨号失败
    ATP_UTIL_PPP_FAIL_SVR_VOICE = 8, // BAS无响应导致VOICE摘机触发的拨号失败
    ATP_UTIL_PPP_FAIL_AUTH = 9,  // 认证错误导致拨号失败
    ATP_UTIL_PPP_FAIL_AUTH_VOICE = 10,  // 认证错误导致VOICE摘机触发的拨号失败
    ATP_UTIL_PPP_FAIL_NO_NAS = 11,  // 没有nas接口（当WAN PPP断掉时，摘机拨号失败） 
}VOICE_WAN_MSG_EN;

/*呼叫的路由类型枚举定义*/
typedef enum tagLINE_TYPE_EN
{
    LINE_TYPE_NULL      = 0x0,
    LINE_TYPE_PSTN      = (0x1 << 0),
    LINE_TYPE_SIP       = (0x1 << 1),
    LINE_TYPE_CS        = (0x1 << 2),
    LINE_TYPE_ISDN      = (0x1 << 3),
    LINE_TYPE_INCALL    = (0x1 << 4),   
    LINE_TYPE_MIX       = (0x1 << 5),       /* 混合呼叫 */
    LINE_TYPE_LCL       = (0x1 << 6),       /* 本地服务 */
    
    LINE_TYPE_BUT,
}LINE_TYPE_EN;

/*模式选择:VOIP/PSTN模式下,ISDN号码无效不可配*/
typedef enum LINE_MODE_E_
{
    LINE_MODE_VOIP      = LINE_TYPE_SIP,
    LINE_MODE_PSTN      = (LINE_TYPE_PSTN | LINE_TYPE_SIP),
    LINE_MODE_ISDN      = (LINE_TYPE_ISDN | LINE_TYPE_SIP),
    LINE_MODE_AUTO      = (LINE_TYPE_PSTN | LINE_TYPE_ISDN | LINE_TYPE_SIP | LINE_TYPE_CS),
    LINE_MODE_CS        = (LINE_TYPE_CS),
    LINE_MODE_BUTT
}LINE_MODE_E;

/*端口类型枚举定义*/
typedef enum tagPORT_TYPE_E_
{
    PORT_TYPE_NULL_ = 0,
    PORT_TYPE_FXS,                      /*FXS口*/
    PORT_TYPE_HS,                       /*DECT子机,HandleSet(HS)*/
    PORT_TYPE_S0,                       /*ISDN话机,下行ISDN*/

    PORT_TYPE_BUTT,
}PORT_TYPE_E;
/***********************h00190880*******2012/3/28**********/
typedef enum tagVOICE_STATUS_TYPE_EN
{
    VOICE_STATUS_PORT,                          /*获取语音模块的端口是否忙*/
    VOICE_STATUS_CALL,                          /*获取语音模块的线路是否忙*/
    VOICE_STATUS_URGCALL,                       /*获取语音模块的线路是否有紧急呼叫*/

    VOICE_STATUS_BUT,
}VOICE_STATUS_TYPE_EN;
/***********************h00190880*******2012/3/28**********/

/*FXS口下挂设备类型枚举定义*/
typedef enum tagFXS_TYPE_E_
{
    FXS_TYPE_NULL = 0,
    FXS_TYPE_POTS,                      /*电话机. Phone*/
    FXS_TYPE_AUTO_RSP,                  /*自动应答机. Answering machine*/
    FXS_TYPE_FAX,                       /*传真机. Telefax*/
    FXS_TYPE_ALL,                       /*综合设备. Combination device*/

    FXS_TYPE_BUTT,
}FXS_TYPE_E;

/*FXO口状态枚举定义*/
typedef enum tagFXO_STATUS_E_
{
    FXO_STATUS_NULL = 0,
    FXO_STATUS_ISDN,                      /*ISDN up*/
    FXO_STATUS_PSTN,                      /*PSTN up*/

    FXO_STATUS_BUTT,
}FXO_STATUS_E;

/*消息子事件*/
typedef struct tagATP_VOICE_MSG_ST_
{
    VOS_UINT32  ulEventType;                    /*事件类型,见不同消息下的事件类型*/
    VOS_UINT32  ulPara;                         /*事件参数*/
    VOS_CHAR    szBoby[ATP_VOICE_EVENT_LEN];    /*事件数据*/
}ATP_VOICE_MSG_S;

/*语音消息结构体*/
typedef struct tagATP_VOICE_MSG_BUF_ST_
{
    VOS_INT32  lMsgType;                        /*消息类型. 见VOICE_MSG_TYPE_E*/
    VOS_CHAR   szMsgBody[ATP_VOICE_MSG_LEN];  /*消息内容*/
}ATP_VOICE_MSG_BUF_S;

typedef struct tagVOICEPUB_RTP_ST
{   
    VOS_UINT32   ulLocalPortMin;                  /*RTP起始端口 */
    VOS_UINT32   ulLocalPortMax;                  /*RTP终止端口 */
    VOS_UINT32   ulDSCPMark;                      /*规格节点*/
    VOS_UINT32   ulTelEventPayloadType;           /*RTP telephone event所使用的负载类型*/
    VOS_UINT32   ulJitBufMaxSize;
    VOS_UINT32   ulJitBufMinSize;
    VOS_UINT32   ulJitBufActualSize;
    VOS_UINT32   ulJitBufType;
}VOICEPUB_RTP_ST;


//账号的呼叫特性结构
typedef struct tagVOICE_LINE_CALLFEATURE_INFO_ST
{
	VOS_BOOL				bCallerIDEnable;
	VOS_BOOL				bCallerIDNameEnable;
	VOS_BOOL				bCallWaitingEnable;
	VOS_BOOL  				bMWIEnable;
	VOS_CHAR 		        acCWStatus [VCE_STR_LEN_32];	
	VOS_UINT32              ulLineMessagesWaiting;
}VOICE_LINE_CALLFEATURE_INFO_ST;

/*Codeclist对象的参数*/
typedef struct tagVOICEPUB_CODECLIST_OBJ
{
    VOS_CHAR      acCodec[VCE_STR_LEN_32];                      /*编码方式名称*/
    VOS_BOOL      bEnable;                                      /**/
    VOS_UINT32    ulPriority;                                   /*语音编码类型的优先级*/
    VOS_CHAR      acPacketizationPeriod[4];                     /*打包时长10/20/30ms*/
    VOS_BOOL      bSilenceSuppression;                          /*是否启用语音活动检测*/    
}VOICEPUB_CODECLIST_ST;

/*Codec参数*/
typedef struct
{
    VOS_UINT32  ulType;                     /*编解码类型，见MEDIA_PROTOCAL_E*/
    VOS_BOOL    bCNGEnable;                 /*是否启用舒适噪音生成*/
    VOS_BOOL    bSilence;                   /*是否启用语音活动检测*/
    VOS_UINT32  ulPacketPeriod;             /*打包时长10/20/30ms,见PACK_TIME_E*/
    VOS_BOOL    bEnable;                    /*是否启用此编解码*/
    VOS_CHAR    acTransmitCodec[VCE_STR_LEN_32];              /*呼出的语音流所使用的编解码*/
    VOS_CHAR    acReceiveCodec[VCE_STR_LEN_32];               /*呼入的语音流所使用的编解码*/
    VOS_UINT32  ulTransmitBitRate;                            /*呼出语音流传输的比特率*/
    VOS_UINT32  ulReceiveBitRate;                             /*呼入语音流传输的比特率*/    
    VOS_UINT32  ulTransmitPacketizationPeriod;                /*呼出打包时长*/
    VOS_BOOL    bTransmitSilenceSuppression;                  /*呼出语音活动检测*/
    VOS_BOOL    bReceiveSilenceSuppression;                   /*呼入语音活动检测*/
}TAPI_CODEC_S;

typedef struct tagVOICE_CALLOG_SESSION_RTP_ST
{
    VOS_UINT32              ulPackDelayVaria;
    VOS_UINT32              ulBuferDelay;
    VOS_UINT32              ulBurstNum;
  	VOS_UINT32             	ulBufferLost;   
	VOS_UINT32             	ulPacketsRecve;                      					
	VOS_UINT32             	ulPacketsLost;                          				
	VOS_UINT32             	ulOverruns;                          				
	VOS_UINT32            	ulUnderruns; 
	VOS_UINT32            	ulAveRoundTripDelay;   
    /*BEGIN: Added by l00180792 @20120719 for wt-104 calllog*/
    VOS_UINT32            	ulDEEcho; 
    VOS_CHAR                szDERQI[VCE_STR_LEN_256];
    /*END: Added by l00180792 @20120719 for wt-104 calllog*/
}VOICE_CALLOG_SESSION_RTP_ST;

typedef struct tagGLOBAL_CFG_FROM_WEB_ST_
{
    //VOS_UINT8   ucLineMode;                   /*网关模式:IP/ISDN/PSTN/AUTO. 见LINE_MODE_E*/
    VOS_UINT8   Reserve[3];                     /*字节对齐和扩展字段*/
    VOS_UINT32  ulRegExpire;                    /*注册Expire时间,单位s*/
    //VOS_CHAR    acIsdnAreaCode[SIP_MAX_NUM_LEN];/*ISDN号码区号*/
    VOS_CHAR    szIfName[VCE_STR_LEN_32];       /*上行接口名*/
    VOS_CHAR    acPSTNNumber[VCE_STR_LEN_32];   /*PSTN号码*/
    VOS_CHAR    acUMTSNumber[VCE_STR_LEN_32 + 4];/*UMTS号码*/
    VOS_BOOL    bUmts;                          /*是否启用UMTS语音*/
    VOS_UINT32  ulMaxSession;                   /*网关目前支持的最大会话数*/
    VOS_UINT32  ulInterDigitTimStd;
	VOS_CHAR    acTerminDigit[VCE_STR_LEN_16];
    VOS_BOOL    bVoiceDiag;                      /* 是否在装备测试 */  
    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32  ulQICalcInterval;
    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32  ulDslDefaultAccount;             /* 此时dsl账号是否是默认的 */
}GLOBAL_CFG_FROM_WEB_S, *GLOBAL_CFG_FROM_WEB_PS;

typedef struct tagLINE_CFG_FROM_WEB_ST_
{
    VOS_BOOL    bEnable;                        /*帐号使能*/
    VOS_UINT32  ulLineSn;                       /*本地帐号索引,Id*/    
    VOS_UINT32  ulWebSeq;                       /*对应页面的排序*/
    VOS_CHAR    szId[SIP_MAX_NUM_LEN];          /*SIP帐号*/
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*用户名*/
    VOS_CHAR    szPassword[SIP_MAX_PSW_LEN];    /*密码*/
    VOS_CHAR    szCode[SIP_MAX_NUM_LEN];        /*区号*/
    VOS_BOOL    bEnableCode;                    /*区号是否使能*/
    VOS_BOOL    bBusyOnBusy;                    /*Busy On Busy是否开启*/
    VOS_UINT8   ucProtocolType;                 /*协议类型:1.UDP 2.TCP 3.TLS 4.SCTP*/
    VOS_UINT8   ucProfileType;                  /*1表示DT服务提供商;2表示其它服务提供商*/
    VOS_UINT16  uwLocalPort;                    /*local port*/
    VOS_CHAR    szRegServer[SIP_MAX_URL_LEN];   /*注册服务器*/
    VOS_CHAR    szOutProxy[SIP_MAX_URL_LEN];    /*outbound proxy server*/
    VOS_CHAR    szProxyServer[SIP_MAX_URL_LEN];	/*代理服务器*/
    VOS_CHAR    szStunServer[SIP_MAX_URL_LEN];  /*stun服务器*/
    VOS_UINT16  uwRegPort;                      /*注册服务器端口号*/
    VOS_UINT16  uwOutProxyPort;                 /*outbound proxy port*/
    VOS_UINT16  uwProxyPort;                    /*代理服务器端口号*/
    VOS_UINT16  uwStunPort;                     /*stun服务器端口号*/
    VOS_BOOL    bMultiLine;
    VOS_BOOL    bCLIR;
    VOS_BOOL    bSDPCodecRes;
	#ifdef SUPPORT_ATP_VCE_WT104_CUSTOM             
    VOS_BOOL    bSpecLine;                      /*标记是否是Profile 3下的特殊账号，此类账号无需注册*/
    #endif
    VOS_UINT32  ulDTMFMethodG711;               /* DTMF数据的传输方法
                                                  0:Inband   
                                                  1:RFC 4733  
                                                  2:SIP Info*/
    VOICE_LINE_CALLFEATURE_INFO_ST  stCalFeature;
}LINE_CFG_FROM_WEB_S, *LINE_CFG_FROM_WEB_PS;

typedef struct tagISDN_LINE_CFG_FROM_WEB_ST_
{
    VOS_CHAR    szNumber[SIP_MAX_NUM_LEN];      /*ISDN号码*/
    VOS_BOOL    bBusyOnBusy;                    /*Busy On Busy是否开启*/
    VOS_BOOL    bMultiLine;
    VOS_BOOL    bCLIR;
}ISDN_LINE_CFG_FROM_WEB_S, *ISDN_LINE_CFG_FROM_WEB_PS;

typedef struct
{
    VOS_UINT8       u8_IPUI[VCE_STR_LEN_64];    /*!< Handset identity >suota升级时用*/
}DECT_HS_IPUI_ST;

typedef struct tagDECT_CFG_FROM_WEB_ST_
{
    VOS_BOOL    bEnable;                        /*dect使能*/
    VOS_CHAR    szPin[VCE_PIN_LEN_MAX];         /*dect子机PIN码*/
	/*start of add by xkf38924,2011-05-31*/
	VOS_BOOL   bEnableEcologicalModel;          /*使能dect生态模式使能*/
	VOS_BOOL   bEnableHalfTransmitPower;        /*使能dect发射功率的一半*/
	/*the end  by xkf38924,2011-05-31*/
	VOS_CHAR   szSubsData[DECT_CMO_SUBSDATA_MAX_LEN]; /*备份子机注册信息*/
    VOS_UINT32 ulNewMobilId;                    /*记录刚刚注册上的子机id，供cgi返回给页面*/
    DECT_HS_IPUI_ST stHsIpui[VCE_DECT_HS_MAX];
}DECT_CFG_FROM_WEB_S, *DECT_CFG_FROM_WEB_PS;

/***********h00190880*******************/
#define VDB_DB_STATUS           "/var/vdb.status"
#define VDB_VDB_WRITE_STATUS    "/var/vdb.write"

// usPortMin :  被检验的端口
// usPortMax:  被检验的端口范围最大值，如果不检查端口范围，则填0
// bTcp        : TCP(1, VOS_TRUE)还是UDP(0, VOS_FALSE)端口
#if 0
typedef struct tagATP_WELKNOWN_PORT_ST
{
    VOS_UINT16           ucTcp;      // 1: TCP; 0: UDP
    VOS_UINT16           ucRange;    // 端口范围
    VOS_UINT16          usPort;     // 端口号
} ATP_WELKNOWN_PORT_ST;
#endif

/*Dect子机类型*/
typedef enum tagDECT_TYPE_E_
{
    DECT_TYPE_GAP	= 0,
    DECT_TYPE_CATIQ1,
    DECT_TYPE_CATIQ2,
    DECT_TYPE_DSPG,
	DECT_TYPE_UNKNOW,
}DECT_TYPE_E;

/* 配置类型*/
typedef enum tagDECT_CFG_TYPE_E_
{
    DECT_CFG_NORMAL   = 0,       /* 正常情况 */
    DECT_CFG_EQUIP,              /* 装备命令配置DECT对码 */
    DECT_CFG_PP,                 /* 子机动作引起的注销 */
    DECT_CFG_OTHER,
}DECT_CFG_TYPE_E;

typedef struct tagDECT_HS_TYPE_ST
{
    VOS_UINT32      ulHsId;     /*取值范围1~5*/
    DECT_TYPE_E     enHsType;
    DECT_CFG_TYPE_E enConfigType;
    VOS_CHAR    acIPUI[VCE_STR_LEN_64];                       /*InternationalPortableUserIdentity*/
    VOS_CHAR    acIPEI[VCE_STR_LEN_64];                       /*InternationalPortableEquipementIdentity*/
    VOS_CHAR    acRFPIAttachTo[VCE_STR_LEN_32];
    VOS_CHAR    acSubscripTime[VCE_STR_LEN_32];
}DECT_HS_TYPE_ST;

/*LineSettings*/
typedef struct tagDECT_LINE_SET_ST
{
    long long           llEntryId;
    VOS_UINT32          ulChangeList[32];
    ATP_TAB_VOICE_LINESETTING_ST          stInfo;
}DECT_LINE_SET_ST;

/*前转类型*/
typedef enum tagVOICE_CF_TYPE_EN
{
    VOICE_CF_CFU          = 0,
    VOICE_CF_CFB          = 1,
    VOICE_CF_CFNR         = 2,
}VOICE_CF_TYPE_EN;

/*更新前转消息结构*/
typedef struct tagATP_VOICE_CFINFO_ST
{
    VOS_INT    iSrcPortId;
    VOS_INT    iDstPortId;
    VOICE_CF_TYPE_EN enCFType;
} ATP_VOICE_CFINFO_ST;

typedef struct tagATP_VOICE_CWINFO_ST
{
    VOS_UINT32  ucPortMask;
	VOS_BOOL   bActive;
} ATP_VOICE_CWINFO_ST;

/*Dect配置发生更改的类型*/
typedef enum tagDECT_COMMONCFG_SETTYPE_E_
{
    DECT_SETTYPE_ADD   = 0,       /* 增加 */
    DECT_SETTYPE_DEL,             /* 删除 */
    DECT_SETTYPE_SET,             /* 修改 */
    DECT_SETTYPE_OTHER,
}DECT_COMMONCFG_SETTYPE_E;

typedef struct tagATP_VOICE_DECTINFO_ST
{
    int          lEntryId;
    int          lListId;
    DECT_COMMONCFG_SETTYPE_E enSetType;    
}ATP_VOICE_DECTINFO_ST;

/***********h00190880*******************/
typedef struct tagPHY_CFG_FROM_WEB_ST_
{
    VOS_BOOL    bEnable;                /*端口是否使能.Dect没注册上为Disable;注册上为Enable*/
    VOS_BOOL    bEnableCW;              /*是否使能呼叫等待业务*/
	/*start of add by xkf38924 ,the date :2011-05-30*/
	VOS_BOOL    bEnableVoiceBoxInfo;    /*是否使能voiceboxinfo*/
	/*end of add by xkf38924,the date:2011-05-30*/
    VOS_UINT8   ucPortID;               /*端口ID*/
    VOS_UINT8   ucLineID;               /*内部号码序列号，起始号码见VCE_INCALL_BASE*/
    VOS_UINT8   ucPortType;             /*端口类型:FXS口,dect子机,ISDN下行话机,见PORT_TYPE_E*/
    VOS_UINT8   ucFXSPortType;          /*FXS口类型:传真机,电话机,自动应答机,综合设备,见FXS_TYPE_E*/
    VOS_UINT8   ucDectType;             /*Dect子机类型:0表示GAP，1表示CATIQ1，2表示CATIQ2，见DECT_TYPE_E */
    #if 0
    VOS_UINT8   ucPriNumDisPlayType;    /*优选号码的来电显示类型,0--显示;非0--限制.见SIP_CLI_STATUS_E*/
    VOS_UINT8   ucSndNumDisPlayType;    /*备选号码的来电显示类型,0--显示;非0--限制.见SIP_CLI_STATUS_E*/
    #endif
    VOS_UINT8   ucInLineNum;            /*呼入关联帐号数目*/
    VOS_UINT8   ucOutLineNum;           /*呼出关联帐号数目*/
    VOS_UINT8   ucReserve;
    VOS_BOOL    bInRelateAll;           /*呼入关联所有*/
    VOS_BOOL    bOutRelateAuto;         /*呼出自动选帐号*/
    VOS_UINT8   aucIncomingLine[VCE_LINE_MAX];  /*呼入关联帐号. 存放关联帐号索引*/
    VOS_UINT8   aucOutgoingLine[VCE_LINE_MAX];  /*呼出关联帐号. 存放关联帐号索引*/
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*端口名*/
    VOS_BOOL    bCFUEnable;
    VOS_BOOL    bCFBEnable;
    VOS_BOOL    bCFNREnable;
    VOS_UINT32  ulCFBRingTime;
    VOS_UINT32  ulCFNRingTime;
    VOS_INT     iCFUDestPortId;                                 /*无条件前转目的端口号，范围0~6，无业务时为-1*/
    VOS_INT     iCFBDestPortId;                                 /*遇忙目的端口号，范围0~6，无业务时为-1*/
    VOS_INT     iCFNRDestPortId;                                /*无应答目的端口号，范围0~6，无业务时为-1*/    
    VOS_UINT32  ulRingTimout;                                   /*端口响铃持续时间*/
}PHY_CFG_FROM_WEB_S, *PHY_CFG_FROM_WEB_PS;

typedef struct tagPHONE_BOOK_CFG_FROM_WEB_ST_
{
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*姓名*/
    VOS_CHAR    szPrivateNum[SIP_MAX_NUM_LEN];  /*私人电话*/
    VOS_CHAR    szFamilyNum[SIP_MAX_NUM_LEN];   /*家庭电话*/
    VOS_CHAR    szMobileNum[SIP_MAX_NUM_LEN];   /*移动电话*/
}PHONE_BOOK_CFG_FROM_WEB_S, *PHONE_BOOK_CFG_FROM_WEB_PS;

typedef struct tagVOICE_SVC_CFG_FROM_WEB_ST_
{
    GLOBAL_CFG_FROM_WEB_S       stGlobal;                        /*语音业务全局配置*/
    LINE_CFG_FROM_WEB_S         stLine[VCE_VOIP_MAX];            /*SIP帐号配置*/
    //ISDN_LINE_CFG_FROM_WEB_S    stIsdnLine[VCE_ISDN_MAX];      /*ISDN帐号配置*/
    DECT_CFG_FROM_WEB_S         stDect;                          /*DECT全局配置*/
    PHY_CFG_FROM_WEB_S          stPhy[VCE_PORT_MAX];             /*FXS口和dect子机配置*/
    VOICEPUB_CODECLIST_ST          stCodecList[VOICE_CODECLIST_MAX];        /* 编码方式列表 */
    //PHONE_BOOK_CFG_FROM_WEB_S   stPhoneBook[VCE_PHONE_BOOK_MAX];
}VOICE_SVC_CFG_S, *VOICE_SVC_CFG_PS;

typedef struct tagDECT_EEPROM_ST
{
    VOS_UINT32  ulOffset;
    VOS_UINT32  ulLen;
    VOS_CHAR    acValue[128];
}DECT_EEPROM_ST;

typedef struct tagDECT_MWI_ST
{
    VOS_UINT32  ulLineID;
    VOS_UINT32  ulMsgNum;
}DECT_MWI_ST;

typedef struct tagDECT_CMBS_LINE_TYPE_ST
{
    VOS_UINT32  ulLineID;
    VOS_UINT32  ulType;
}DECT_CMBS_LINE_TYPE_ST;

typedef struct tagDECT_EEPSUB_ST
{
    VOS_CHAR    acRFPI[12];
    VOS_CHAR    ucHsNr[4];
    VOS_CHAR    acSubInfo[84];
}DECT_EEPSUB_ST;

typedef struct tagDECT_EQUIP_ST
{
    VOS_CHAR    acValue[100];
}DECT_EQUIP_ST;

/* start of modify by l00126521 2010-5-18 for QGT 8445:子机呼叫页面按钮控制修改 */
/*子机呼叫消息结构体*/
typedef struct tagDECT_LOCATE_ST
{
    VOS_UINT32 ulHsMask;
    VOS_UINT32 ulStatus;
    VOS_CHAR   acReserved[VCE_STR_LEN_16];
}DECT_LOCATE_ST;
/* end of modify by l00126521 2010-5-18 for QGT 8445*/

typedef struct tagDECT_CMBS_LINE_CF_ST
{
    VOS_UINT32  ulLineID;
    VOS_UINT32  ulType;                   /*0:CFU;1:CFB;2:CFNR*/
    char acActiveCode[VCE_STR_LEN_32];    /*Activation code number*/
    char acDeactiveCode[VCE_STR_LEN_32];  /*Deactivation code number*/
}DECT_CMBS_LINE_CF_ST;

/*BEGIN:added by l00192646,2012/06/15,for wt-104 DECT*/

#define CMBS_MAX_VERSION_LENGTH 20

typedef struct tagDECT_INFO_ST
{
     VOS_UINT32 ulHandsetFlag;
     VOS_CHAR acHandsetHardwareVer[CMBS_MAX_VERSION_LENGTH]; 
     VOS_CHAR acHandsetSoftwareVer[CMBS_MAX_VERSION_LENGTH]; 
     VOS_CHAR acLastUPTime[VCE_STR_LEN_64]; 
}DECT_INFO_ST;
/*END:added by l00192646,2012/06/15,for wt-104 DECT*/
extern VOICE_SVC_CFG_PS g_pstVoiceServiceCfg;
/*************************  END: VOICE_LIB  *************************/




/*************************  BEGIN: VOICE_STATE  *************************/
#define FILE_PATH_PORT_STATUS       "/var/voice/portstatus"
#define FILE_PATH_VOIP_STATUS       "/var/voice/voipstatus"
#define FILE_PATH_URGCALL_STATUS    "/var/voice/urgcallstatus"
#define FILE_PATH_DECT_UPGIMAGE     "/var/voice/dectimage"
#define FILE_PATH_DECT_CMBS_LOG     "/var/voice/dectLog"
#define FILE_PATH_DECT_CMBS_TRACE   "/var/voice/dectTrace"
#define FILE_PATH_DECT_TMPVALUE     "/var/voice/decttmpvalue"
#define FILE_PATH_DECT_UPDATE     "/var/voice/dectupdate"
#define FILE_PATH_DECT_SUBINFO      "/var/voice/dectsubinfo"
#define FILE_PATH_DECT_SUBINFOSTAT  "/var/voice/dectsubinfostat"
#define FILE_PATH_DECT_BASE_VER     "/var/voice/dectbaseversion"
#define FILE_PATH_DIAG_FXS1_TO_FXS2 "/var/voice/fxs1tofxs2"
#define FILE_PATH_DIAG_FXS2_TO_FXS1 "/var/voice/fxs2tofxs1"
#define FILE_PATH_DECT_PCM_RESULT   "/var/voice/pcmresult"
#define FILE_PATH_VOIP_INTERVAL     "/var/voice/voipinterval"
#define FILE_PATH_DECT_BARCODE      "/var/voice/dectbarcode"


#define VOICE_STATUS_LEN_MAX        (16)

/* rfpi code (10) + hs Nr (2) + subscribe info (80) */

#define VOICE_DECT_RFPI_LEN         (10)
#define VOICE_DECT_HSNR_LEN         (2)
#define VOICE_DECT_SUBINFO_LEN      (80)

#define VOICE_DECT_SUBINFO_LEN_ALL  (400)
#define VOICE_DECT_SUBINFO_TOTAL_LEN  (VOICE_DECT_RFPI_LEN + VOICE_DECT_SUBINFO_LEN)
#define VOICE_DECT_BARCODE_LEN      (16)

#define LINE_CALL_STATUS_STR_IDLE               "Idle"
#define LINE_CALL_STATUS_STR_CALLING            "Calling"
#define LINE_CALL_STATUS_STR_RINGING            "Ringing"
#define LINE_CALL_STATUS_STR_CONNECTING         "Connecting"
#define LINE_CALL_STATUS_STR_INCALL             "InCall"
#define LINE_CALL_STATUS_STR_HOLD               "Hold"
#define LINE_CALL_STATUS_STR_DISCONNECTING      "Disconnecting"

/*BEGIN:Added by l00180792 @20120717 for QI*/
typedef struct tagVOICE_QUALITY_ST_
{
    VOS_CHAR       acQualityIndicator[VCE_STR_LEN_64];
    VOS_CHAR       acWorstQIValues[VCE_STR_LEN_64];
    VOS_CHAR       acWorstQITimestamps[VCE_STR_LEN_64];
}VOICE_QUALITY_ST;
/*END:Added by l00180792 @20120717 for QI*/

/*会话信息*/
typedef struct tagSESSION_STATE_ST_
{   
    VOS_UINT32      ulStartYear;
    VOS_UINT32      ulStartMonth;
    VOS_UINT32      ulStartDay;
    VOS_UINT32      ulStartHour;
    VOS_UINT32      ulStartMinute;
    VOS_UINT32      ulStartSecond;
    VOS_UINT32      ulStartTime;                            /*开始时刻*/
    VOS_UINT32      ulEndTime;                              /*结束时刻,如果为0,表示还未结束*/

    VOS_INT32       ulDuration;                             /*通话时长*/
    VOS_CHAR        acStreamType[VCE_STR_LEN_16];
    VOS_CHAR        acFarEndIPAddress[VCE_STR_LEN_64];      /*对端ip地址*/
    VOS_UINT32      ulFarEndUDPPort;        /*对端udp端口*/
    VOS_UINT32      ulLocalUDPPort;        /*本地udp端口*/
    VOS_UINT32      ulSessionID;
    VOS_UINT32      ulChangeFlag;           /**/
    TAPI_CODEC_S    stCodec;
    VOICE_CALLOG_SESSION_RTP_ST  stRtp;
    VOICE_QUALITY_ST stVoiceQI;
    struct sockaddr_storage stLclAddr;
    struct sockaddr_storage stRmtAddr;                      /*本端RTP端口*/
}SESSION_STATE_S, *SESSION_STATE_PS;


/*RTCP信息统计*/
typedef struct tagRTCP_INFO_ST_
{
    VOS_UINT32  ulPacketsSent;                          /*已发送报文个数*/
    VOS_UINT32  ulPacketsReceived;                      /*已接收报文个数*/
    VOS_UINT32  ulBytesSent;                            /*已发送报文字节数*/
    VOS_UINT32  ulBytesReceived;                        /*已接收报文字节数*/
    VOS_UINT32  ulPacketsLost;                          /*已丢失报文个数*/

    /*Total number of times the receive jitter buffer has overrun for this line*/
    VOS_UINT32  ulOverruns;                             /*超出抖动范围外的报文个数*/

    /*Total number of times the receive jitter buffer has underrun for this line*/
    VOS_UINT32  ulUnderruns;                            /*抖动范围内的报文个数*/

    /*Current receive packet loss rate in percent*/
    VOS_UINT32  ulReceivePacketLossRate;                /*本端丢包率*/

    /*Current far end receive packet lost rate in percent*/
    VOS_UINT32  ulFarEndPacketLossRate;                 /*远端丢包率*/

    /*Current receive interarrival jitter in microseconds*/
    VOS_UINT32  ulReceiveInterarrivalJitter;            /*本端抖动时间间隔*/

    /*Current Interarrival jitter in microseconds as reported from the far-end device via RTCP*/
    VOS_UINT32  ulFarEndInterarrivalJitter;             /*远端抖动时间间隔*/

    /*Current round trip delay in microseconds*/
    VOS_UINT32  ulRoundTripDelay;                       /*往返延迟[RFC3550 6.4]*/

    /*Average receive interarrival jitter in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageReceiveInterarrivalJitter;     /*本端平均抖动时间间隔*/

    /*Average far-end interarrival jitter in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageFarEndInterarrivalJitter;      /*远端平均抖动时间间隔*/

    /*Average round trip delay in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageRoundTripDelay;                /*平均往返延迟*/

    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32       ulBufferLost;                      /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.BufferLost*/
    VOS_UINT32       ulJitter;                          /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.PacketDelayVariation*/
    VOS_UINT32       ulpeakJitter;                      /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.BufferDelay*/
    VOS_UINT32       ulMOSLQ;                           /* MOS-listening quality (value*10) */
    VOS_UINT32       ulMOSCQ;                           /* MOS-conversational quality (value*10) */
    /*END:Added by l00180792 @20120717 for QI*/
}RTCP_INFO_ST;

/*帐号状态*/
typedef struct tagLINE_STATE_ST_
{
    VOS_CHAR        szId[SIP_MAX_NUM_LEN];                  /*Line帐号*/
    VOS_CHAR        szCallStatus[VCE_STR_LEN_16];   
    VOS_UINT8       ucRegStatus;                            /*号码注册状态*/
    /* BEGIN: Modified by l00126521 for AU8D04013, 2010/2/11 */
    VOS_UINT8       ucRegFailReason;                        /*注册失败原因*/
    /* END:   Modified by l00126521 for AU8D04013, 2010/2/11 */
    
    VOS_UINT32      ulReadyFlag;                            /*信息是否Ready的标志位*/
    VOS_BOOL        bResetStatistics;                       /*是否清空统计*/
    VOS_UINT32      ulIncomingReceived;                     /*呼入电话个数*/
    VOS_UINT32      ulIncomingAnswered;                     /*呼入已接电话个数*/
    VOS_UINT32      ulIncomingConnected;                    /*呼入成功电话个数*/
    VOS_UINT32      ulIncomingFailed;                       /*呼入失败电话个数*/
    VOS_UINT32      ulOutgoingAttempted;                    /*呼出电话个数*/
    VOS_UINT32      ulOutgoingAnswered;                     /*呼出已接电话个数*/
    VOS_UINT32      ulOutgoingConnected;                    /*呼出成功电话个数*/
    VOS_UINT32      ulOutgoingFailed;                       /*呼出失败电话个数*/
    VOS_UINT32      ulCallsDropped;                         /*通话不正常中断个数*/
    VOS_UINT32      ulTotalCallTime;
    VOS_UINT32      ulServerDownTime;
    /*BEGIN;added by l00192646,2012/06/27,for wt-104 line call waiting status*/
    VOS_CHAR        szCallWaitingStatus[VCE_STR_LEN_32];    /*线路上是否有呼等*/
    /*END;added by l00192646,2012/06/27,for wt-104 line call waiting status*/
    VOS_UINT32      ulLineMessagesWaiting;                  /*线路上留言的个数*/

    SESSION_STATE_S stSession[SIP_LINE_STATE_SESS_MAX];     /*帐号上面的呼叫信息*/
    
    RTCP_INFO_ST    stRtcp;                                 /*RTCP统计信息*/
}LINE_STATE_S, *LINE_STATE_PS;

/*PhyInterface state*/
typedef struct tagPHY_STATE_ST_
{
    VOS_BOOL    bBusy;                                  /*端口是否正在使用*/
    VOS_BOOL    bCFType;                                /*前转类型*/
    VOS_CHAR    szCFNum[SIP_MAX_NUM_LEN];               /*前转号码*/
}PHY_STATE_S, *PHY_STATE_PS;

/*Fxs state*/
typedef struct tagFXS_STATE_ST_
{
    VOS_BOOL    ulReadyFlag;                                  
    VOS_BOOL    bActive; 
    VOS_BOOL    bflag;
}FXS_STATE_S, *FXS_STATE_PS;

typedef struct tagSIP_INFO_ST_
{
    VOS_CHAR    szId[SIP_MAX_NUM_LEN];                  /*VoIP帐号*/
}SIP_INFO_S, *SIP_INFO_PS;

typedef struct tagGLOBAL_STATE_ST_
{
    VOS_BOOL    bPortBusy;                              /*是否有端口正在使用*/
    VOS_BOOL    bHasUrgCall;                            /*是否存在紧急呼叫*/
    VOS_BOOL    bHasUseableLine;                        /*是否存在可用帐号*/
    /*BEGIN:Added by l00180792 @20120828 for DTS2012081703300*/
    VOS_BOOL    bSNTPUpdate;                            /*是否成功获取到SNTP时间*/
    /*BEGIN:Added by l00180792 @20120828 for DTS2012081703300*/
    VOS_UINT32  ulFxoStatus;                            /*FXO口状态:ISDN UP;PSTN UP;fail*/
    VOS_UINT32  ulDectStatus;                            /*FXO口状态:ISDN UP;PSTN UP;fail*/
    VOS_UINT32  ulRegExpire;                            /*注册刷新时间*/
}GLOBAL_STATE_S, *GLOBAL_STATE_PS;

typedef struct tagISDN_AUTO_ST_
{
    VOS_CHAR    szNumber[SIP_MAX_NUM_LEN];              /*ISDN号码*/
    
}ISDN_AUTO_S, *ISDN_AUTO_PS;

//SignalPerformance
typedef struct tagVOICE_CALOG_SIGNALPERFORMANCE_ST
{
	VOS_UINT32				ulCalSetupDelay;
	int				        iOutMediaEstabDelay;
	int				        iIncomMediaEstabDelay;
}VOICE_CALOG_SIGNALPERFORMANCE_ST;

#define VOICE_CALLLOG_CALLID                    (1 << 0)
#define VOICE_CALLLOG_CALLIINGNUM               (1 << 1)
#define VOICE_CALLLOG_CALLEDNUM                 (1 << 2)
#define VOICE_CALLLOG_REMOTEPART                (1 << 3)
#define VOICE_CALLLOG_USEDLINE                  (1 << 4)
#define VOICE_CALLLOG_USEDEXTENSIONS            (1 << 5)
#define VOICE_CALLLOG_DIRECTION                 (1 << 6)
#define VOICE_CALLLOG_STARTTIME                 (1 << 7)
#define VOICE_CALLLOG_DURATION                  (1 << 8)
#define VOICE_CALLLOG_CURCALLSTATUS             (1 << 9)
#define VOICE_CALLLOG_CALLTERMINATCAUSE         (1 << 10)
#define VOICE_CALLLOG_TERMAUSEDREF              (1 << 11)
#define VOICE_CALLLOG_INTERVALJITTER            (1 << 12)
//#define VOICE_CALLLOG_FARENDJITTER              (1 << 13)
//#define VOICE_CALLLOG_AVERECEVJITTER            (1 << 14)
//#define VOICE_CALLLOG_AVEFARENDJITTER           (1 << 15)
#define VOICE_CALLLOG_ROUNDTRIPDELAY            (1 << 16)
#define VOICE_CALLLOG_AVEROUNDTRIPDELAY         (1 << 17)
#define VOICE_CALLLOG_SESSIONNUM                (1 << 18)
#define VOICE_CALLLOG_ROUNDTRIPDELAY            (1 << 19)
#define VOICE_CALLLOG_CALSETUPDELAY             (1 << 20)
#define VOICE_CALLLOG_OUTESTABDELAY             (1 << 21)
#define VOICE_CALLLOG_INCOMESTABDELAY           (1 << 22)
#define VOICE_CALLLOG_SESSION_STREAMTYPE        (1 << 23)
#define VOICE_CALLLOG_SESSION_START             (1 << 24)
#define VOICE_CALLLOG_SESSION_DURATION          (1 << 25)
#define VOICE_CALLLOG_SESSION_FARENDIPADDR      (1 << 26)
#define VOICE_CALLLOG_SESSION_FARENDUDPPORT     (1 << 27)
#define VOICE_CALLLOG_SESSION_LOCALUDPPORT      (1 << 28)
#define VOICE_CALLLOG_SESSION_SESSIONID         (1 << 29)
#define VOICE_CALLLOG_SESSION_CODEC             (1 << 30)
#define VOICE_CALLLOG_SESSION_RTP               (1 << 31)

//语音呼叫log结构
typedef struct tagVOICE_CALLOG_ST
{
    VOS_UINT32              ulCrSn;                             /*该路呼叫对应的CrSn*/
    VOS_BOOL                bUsed;                              /*该log正在使用*/
    VOS_UINT32              ulCalllogInstId;                    /*对应实例号*/
    VOS_CHAR      		    acCallID[VCE_STR_LEN_256]; 			/*Entry id*/
    VOS_CHAR 				acCallingNumber[VCE_STR_LEN_256];	/*Calling Number*/
    VOS_CHAR 				acCalledNumber[VCE_STR_LEN_256];	/*Called Number*/
    VOS_CHAR 				acRmotePart[VCE_STR_LEN_32];
    VOS_CHAR 				acUsedLine[VCE_STR_LEN_64];
	VOS_CHAR 				acUsedExtensions[VCE_STR_LEN_64];	/*List of UsedExtension*/
	VOS_CHAR 				acDirection[VCE_STR_LEN_32]; 		/*Incoming or Outgoing*/
	VOS_CHAR 				acStartTime[VCE_STR_LEN_32]; 		/*log start time*/
    VOS_UINT32             	ulDuration;                         /*duration time*/
	VOS_CHAR      			acCurCallStatus[VCE_STR_LEN_32];            /*current call status*/
	VOS_CHAR         		acCallTernaCause[VCE_STR_LEN_32];        /*CallTerminationCause*/
	VOS_UINT32             	ulTermaUsedRef;                          				/*number of Terminal used*/
	VOS_UINT32             	ulRecevIntervalJitter;                          				
	VOS_UINT32            	ulFarEndIntervalJitter;     
	VOS_UINT32             	ulAveRecevIntervalJitter; 
	VOS_UINT32            	ulAveFarEndIntervalJitter; 
	VOS_UINT32            	ulRoundTripDelay; 
    VOS_UINT32            	ulAveRoundTripDelay;
	VOS_UINT32            	ulSessionNumber; 
    VOS_UINT32              ulChangeFlag;
    VOICE_CALOG_SIGNALPERFORMANCE_ST  stSignalPerform;
	SESSION_STATE_S                   stCallogSession[SIP_LINE_STATE_SESS_MAX];
}VOICE_CALLOG_ST;

typedef struct tagVOICE_CALLOGINFO_ST_
{
    VOS_UINT32       ulCallLogIndex;
    VOS_BOOL         bSavedToDB;
}VOICE_CALLOGINFO_ST;

typedef struct tagVOICE_STATE_ST_
{
    GLOBAL_STATE_S  stGlobal;                           /*全局状态信息*/
    SIP_INFO_S      stSip[VCE_VOIP_MAX];                /*VoIP帐号关键配置信息*/
    LINE_STATE_S    stLine[VCE_VOIP_MAX];               /*帐号状态信息*/
    PHY_STATE_S     stPort[VCE_PORT_MAX];               /*端口状态信息*/
    FXS_STATE_S     stFxs[VCE_FXS_MAX];                  /*S口上是否接话机*/
    ISDN_AUTO_S     stAutoIsdn[VCE_ISDN_MAX];           /*自动获取ISDN号码*/
    VOICE_CALLOG_ST stCalllog[VOICE_CALL_MAX];
    VOS_BOOL        bAnt0IsOn;                           /* 记录天线0打开状态*/
    VOS_BOOL        bAnt1IsOn;                           /* 记录天线1打开状态*/
}VOICE_STATE_S, *VOICE_STATE_PS;

extern VOICE_STATE_PS g_pstVoiceState;

extern VOS_INT32 ATP_UTIL_GetVoiceQue(VOS_INT32 *hVoiceQue);
extern VOS_INT32 ATP_UTIL_SendMsgToVoice(VOS_UINT32 ulMsgType, VOS_UINT32 ulEventType,
                                         VOS_UINT32 ulPara, VOS_CHAR *pcBoby);
extern VOS_BOOL ATP_UTIL_VoiceGetPortStatus(VOS_VOID);
extern VOS_BOOL ATP_UTIL_VoiceGetVoipStatus(VOS_VOID);
extern VOS_BOOL ATP_UTIL_VoiceHasUrgCall(VOS_VOID);
extern VOS_VOID ATP_UTIL_SendPinMsgToVoice(VOS_VOID);

VOS_UINT32 ATP_UTIL_Mktime(const unsigned int year0, const unsigned int mon0,   
                              const unsigned int day, const unsigned int hour,   
                              const unsigned int min, const unsigned int sec);  

/*************************  END: VOICE_STATE  *************************/




/*************************  BEGIN: TAPI_CMS  *************************/
/* 消息队列写权限 */
#define VHG_VOICE_MSG_W             01
#define VHG_VOICE_CMD_MSG_TYPE      8849
#define VHG_VOICE_CMS_MSGKEY        'V'
#define VHG_VOICE_PID_PATH          "/var/vhgvpid"

/*消息的类型*/
typedef enum tagVHG_VOICE_MSG_TYPE_E
{
    VHG_MSG_TYPE_CMS,                       /*产品语音业务配置消息*/
    VHG_MSG_TYPE_TIMER,                     /*timer消息*/
    VHG_MSG_TYPE_BUTT
}VHG_VOICE_MSG_TYPE_E;

/* 命令行或配置管理发送的指示 */
typedef enum tagVHG_VOICE_CMS_MSG_TYPE_E
{
	VHG_VOICE_CMS_MSG_UPDATA_CONFIG,
    VHG_VOICE_CMS_MSG_UNKNOWN
}VHG_VOICE_CMS_MSG_TYPE_E;

/* 配置操作数据 (配置管理对象) */
typedef enum tagVHG_VOICE_CMS_MSG_CMO_E
{
	VHG_VOICE_CMS_MSG_VOICEPROFILE,
    VHG_VOICE_CMS_MSG_RTP,
    VHG_VOICE_CMS_MSG_VOICEPROCESSING,
    VHG_VOICE_CMS_MSG_CODEC,
    VHG_VOICE_CMS_MSG_CODECLIST
}VHG_VOICE_CMS_MSG_CMO_E;

/*全局参数*/
typedef struct
{
    /*voice profile对象的参数*/
    VOS_UINT32  ulDTMFMethod;               /* DTMF数据的传输方法DTMF_TYPE_E
                                             0:Inband
                                             1:RFC 2833
                                             2:SIP Info*/
    VOS_UINT32  ulRegion;	                /*国别REGION_CODING_E*/
    VOS_UINT32  ulFaxOption;                /*传真类型FAX_TYPE_E
                                             0:G.711A FAX Passthrough
                                             1:G.711U FAX Passthrough
                                             2:T.38 FAX*/
    VOS_UINT32  ulSipPriority;              /*SIP RTP报文发送的优先级1:低2:中3:高SIP_PRIORITY_E*/
    VOS_UINT32  ulTOSPriority;              /*TOS优先级SIP_PRIORITY_E*/

    /*RTP对象的参数*/
    VOS_UINT32  ulLclRTPPortMin;            /*最小RTP端口*/
    VOS_UINT32  ulLclRTPPortMax;            /*最大RTP端口*/
    VOS_BOOL    bJitterBuffer;              /*是否启用抖动缓冲*/
    VOS_UINT32  ulJBLength;                 /*JitterBuffer长度*/
    VOS_BOOL    bPackLostCompensate;        /*是否启用丢包补偿*/
    VOS_UINT32  ulJBType;                   /*JitterBuffer类型JB_TYPE_E*/
    VOS_UINT32  ulTelEventPayloadType;      /*RTP telephone event所使用的负载类型*/
    VOS_UINT32  ulJitBufMaxSize;
    VOS_UINT32  ulJitBufMinSize;
    VOS_UINT32  ulJitBufActualSize;
    VOS_UINT32  ulJitBufType;

    /*VoiceProcessing对象的参数*/
    VOS_INT32	lTransmitGain;              /*话筒音量*/
    VOS_INT32	lReceiveGain;               /*听筒音量*/

}TAPI_GLOBAL_S;



/*帐号参数*/
typedef struct
{
    VOS_BOOL        bECEnable;              /*是否启用回波抵消*/

    VOS_UINT32      ulWebSeq;               /*用于标记账号在页面的顺序*/
    
    VOS_UINT32      ulDTMFMethod;           /* DTMF数据的传输方法
                                                  0:Inband   
                                                  1:RFC 4733  
                                                  2:SIP Info*/ 
    TAPI_CODEC_S    stCodec[VCE_CODEC_MAX]; /*编解码列表*/
}TAPI_LINE_S;

/*语音参数*/
typedef struct
{
    TAPI_GLOBAL_S   stGlobal;               /*全局参数*/
    TAPI_LINE_S     stLine[VCE_VOIP_MAX];   /*VoIP帐号参数*/
}TAPI_CMS_S, *TAPI_CMS_PS;

/* CMS message */
typedef struct VHG_VOICE_CMS_MSG
{
    VHG_VOICE_CMS_MSG_TYPE_E  lEventType;   /*配置操作类型,见VHG_VOICE_CMS_MSG_TYPE_E*/
    VOS_UINT32                ulCMDHead;    /*配置操作内容*/
    VHG_VOICE_CMS_MSG_CMO_E   lCMDBody;     /*配置操作数据*/
}VHG_VCMS_MSG_ST;

typedef struct VHG_VCMS_MSG
{
    VHG_VOICE_MSG_TYPE_E eMsgType;          /*消息类型，可以用来区分是否是配置消息,见VHG_VOICE_MSG_TYPE_E*/
    VHG_VCMS_MSG_ST      CMSMsg;            /*如果是配置消息时，则是配置消息的内容*/
} VHG_VOICE_MSG_ST;

typedef struct VHG_VCMS_MSG_BUFF
{
    VOS_INT32         mtype;                /* 消息区分 ,用来判断是否是产品语音业务消息*/
    VHG_VOICE_MSG_ST  stVoiceMsg;           /* 消息内容 */
}VHG_VOICE_MSG_BUFF_ST;

#define	DECT_NAME_MAX_LEN			64
typedef struct tagDECT_NAME_UPDATE
{
	VOS_UINT32	ulDectIndex;
	VOS_CHAR	acDectName[DECT_NAME_MAX_LEN];
}DECT_NAME_UPDATE;
extern TAPI_CMS_PS g_pstVoiceTapiCfg;
/*************************  END: TAPI_CMS  *************************/



/*Start of 维护管理组 2011-9-26 11:25 for RSS by x00125299*/
typedef struct tagATP_VOICE_RSSRESPONSE_ST
{
    VOS_UINT32  ulIdx;
	VOS_CHAR    acSessinList[ATP_ACTUAL_LEN_256];
} ATP_VOICE_RSSRESPONSE_ST;
/*End of 维护管理组 2011-9-26 11:25 for RSS by x00125299*/

/*RSS*/
typedef struct tagATP_RSSRead_MSG_ST
{
    VOS_UINT32  ulStartId;
	VOS_UINT32  ulCount;
	VOS_UINT32  ulSessionId;
}ATP_RSSRead_MSG_ST;

/*EMAIl*/
typedef struct tagATP_EmailRead_MSG_ST
{
    long long   llAccountIdx;
	VOS_UINT32  ulSessionId;
}ATP_EmailRead_MSG_ST;

typedef struct tagATP_EmailDel_MSG_ST
{
    long long 	llAccountIdx;
    int		lMailNumber;
}ATP_EmailDel_MSG_ST;

/*add by w00191600  2011-11-5 for 标记IMAP邮件已读*/
typedef struct tagATP_EmailSign_MSG_ST
{
    long long 	llAccountIdx;
    long long	llMailUid;//imap邮件的UID
}ATP_EmailSign_MSG_ST;
/*end add by w00191600  2011-11-5 for 标记IMAP邮件已读*/

/*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
/*Direction of the call*/
#define CALLLOG_DIRECTION_INCOMING  "Incoming"
#define CALLLOG_DIRECTION_OUTGOING  "Outgoing"

/*Status of the call*/
#define CALLLOG_CURCALLSTATUS_IDLE  			"Idle"
#define CALLLOG_CURCALLSTATUS_DIALING  		"Dialing"
#define CALLLOG_CURCALLSTATUS_DELIVERED  	"Delivered"
#define CALLLOG_CURCALLSTATUS_CONNECTED  	"Connected"
#define CALLLOG_CURCALLSTATUS_ALERTING  		"Alerting"
#define CALLLOG_CURCALLSTATUS_DISCONNECTED  "Disconnected"

/*Call termination cause*/
#define CALLLOG_CALLTERMICAUSE_NOEXTENSIONSMAPPED  			"NoExtensionsMapped"
#define CALLLOG_CALLTERMICAUSE_NOEXTENSIONSAVAILABLE  			"NoExtensionsAvailable"
#define CALLLOG_CALLTERMICAUSE_ANONYMOUSCALLREJECTION  		"AnonymousCallRejection"
#define CALLLOG_CALLTERMICAUSE_CALLWAITINGREJECTED  			"CallWaitingRejected"
#define CALLLOG_CALLTERMICAUSE_CALLFORWARDINGUNCONDITIONAL  	"CallForwardingUnconditional"
#define CALLLOG_CALLTERMICAUSE_CALLFORWARDINGBUSY  			"CallForwardingBusy"
#define CALLLOG_CALLTERMICAUSE_CALLFORWARDINGNOREPLY  		"CallForwardingNoReply"
#define CALLLOG_CALLTERMICAUSE_LOCALDISCONNECT  				"LocalDisconnect"
#define CALLLOG_CALLTERMICAUSE_LOCALFORBIDDEN  					"LocalForbidden"
#define CALLLOG_CALLTERMICAUSE_LOCALTIMEOUT  					"LocalTimeout"
#define CALLLOG_CALLTERMICAUSE_LOCALMEDIAERROR  				"LocalMediaError"
#define CALLLOG_CALLTERMICAUSE_LOCALPRIORITY  					"LocalPriority"
#define CALLLOG_CALLTERMICAUSE_LOCALREJECT  						"LocalReject"
#define CALLLOG_CALLTERMICAUSE_LOCALTRANSFER  					"LocalTransfer"
#define CALLLOG_CALLTERMICAUSE_LOCALINTERNALERROR  			"LocalInternalError"
#define CALLLOG_CALLTERMICAUSE_REMOTEDISCONNECT  				"RemoteDisconnect"
#define CALLLOG_CALLTERMICAUSE_REMOTEBADREQUEST  				"RemoteBadRequest"
#define CALLLOG_CALLTERMICAUSE_REMOTEFORBIDDEN  				"RemoteForbidden"
#define CALLLOG_CALLTERMICAUSE_REMOTENOTFOUND  				"RemoteNotFound"
#define CALLLOG_CALLTERMICAUSE_REMOTEREJECT  					"RemoteReject"
#define CALLLOG_CALLTERMICAUSE_REMOTENOTALLOWED  				"RemoteNotAllowed"
#define CALLLOG_CALLTERMICAUSE_REMOTENOTACCEPTABLE  			"RemoteNotAcceptable"
#define CALLLOG_CALLTERMICAUSE_REMOTETIMEOUT  					"RemoteTimeout"
#define CALLLOG_CALLTERMICAUSE_REMOTEUNAVAILABLE  				"RemoteUnavailable"
#define CALLLOG_CALLTERMICAUSE_REMOTEBUSY  						"RemoteBusy"
#define CALLLOG_CALLTERMICAUSE_REMOTENOTSUPPORTED  			"RemoteNotSupported"
#define CALLLOG_CALLTERMICAUSE_REMOTENETWORKFAILURE  			"RemoteNetworkFailure"

/*Stream Type*/
#define CALLLOG_STREAMTYPE_AUDIO  			"Audio"
#define CALLLOG_STREAMTYPE_VIDEO  			"Video"
#define CALLLOG_STREAMTYPE_FAX  				"Fax"
#define CALLLOG_STREAMTYPE_MODEM  			"Modem"

/*Codec Type*/
#define CALLLOG_CODECTYPE_G711A  			"G.711ALaw"
#define CALLLOG_CODECTYPE_G711U  			"G.711MuLaw"
#define CALLLOG_CODECTYPE_G722  			"G.722"

/*Codec transmit bitrate*/
#define CALLLOG_CODECBITRATE_64K  			64000
/*END: Added by l00180792 @20120627 for wt-104 calllog*/

#endif /* __VOICEPUB_H__ */

