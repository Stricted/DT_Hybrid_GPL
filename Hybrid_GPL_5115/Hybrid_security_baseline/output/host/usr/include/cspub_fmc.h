/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : CsPub.h
  版 本 号   : 初稿
  作    者   : cqLuo
  生成日期   : 2008年5月8日
  最近修改   :
  功能描述   : CS语音模块公共头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年5月8日
    作    者   : cqLuo
    修改内容   : 创建文件

******************************************************************************/
#ifndef __CS_PUB_H__
#define __CS_PUB_H__

#ifdef __cplusplus
    #if __cplusplus
extern "C" {
    #endif
#endif


#define USR_VOICE_CS            "/bin"      /*ftok目录*/
#define VOICE_CS_PID_PATH       USR_VOICE_CS
#define VOICE_CS_MSGKEY_FLAG    'v'

/*命令行执行结果文件存放路径*/
#define CS_CMDRESULT_FILE       "/var/voice/cmdResult"
#define MAX_CMDRESULT_FILE_LEN  64
#define CMDRESULT_ERROR_STR     "ERROR"

#define CS_MAX_CALLID_LEN       32          /*最大拨号号码长度*/
#define CS_MAX_CMDSTRING_LEN    128         /*命令行字符串长度*/
#define CS_CLIP_FLAG            "I"         /*打开来电显示标识*/
#define CS_CLIR_FLAG            "i"         /*关闭来电显示标识*/
#define CS_VOICE_FLAG           ";"         /*语音呼叫标识*/
#define CS_OK                   1           /**/
#define CS_NOK                  0           /**/

#define AT_RCV_MSG_LENGTH       252         /*接收消息缓冲区长度*/
/*begin: l110333 for AU4D01099(ata fail) on 2009.01.08*/
#define SIG_ATCMD_ATA           98          /*ATA信号*/
#define ATS_PID                 "/var/at/atsrv.pid"
#define MAX_ATS_PID_FILE_LEN    16
/*end: l110333 for AU4D01099(ata fail) on 2009.01.08*/

/*CS语音模块下发给AT适配模块的命令类型*/
typedef enum _E_CS_CMD_TYPE
{
	CS_CMD_TYPE_VOICE_OUTGOING = 1,			    /*语音外呼命令*/
	CS_CMD_TYPE_FAX_OUTGOING,				    /*传真外呼命令*/
	CS_CMD_TYPE_ACCEPT,						    /*接听命令*/
	CS_CMD_TYPE_END,						    /*挂断命令*/
	#ifdef CS_SUPPLY_SERVICE
	CS_CMD_TYPE_CW_ENABLE,					    /*激活呼叫等待业务*/
	CS_CMD_TYPE_CW_DISABLE,					    /*去激活呼叫等待业务*/
	CS_CMD_TYPE_CFU_ENABLE,					    /*激活无条件呼叫前转*/
	CS_CMD_TYPE_CFB_ENABLE,					    /*激活遇忙呼叫前转*/
	CS_CMD_TYPE_CFNR_ENABLE,					/*激活无应答呼叫前转*/
	CS_CMD_TYPE_CT_DISABLE,					    /*去激活呼叫转移业务*/

	CS_CMD_TYPE_END_HOLDCALL,                   /*释放被保持呼叫,或挂断CW呼叫*/
	CS_CMD_TYPE_END_ACTIVECALL,                 /*释放所有激活呼叫,并激活CH和CW呼叫*/
	CS_CMD_TYPE_END_ONE,                        /*结束一路指定的呼叫*/
	CS_CMD_TYPE_SWITCH,						    /*切换呼叫属性*/
	CS_CMD_TYPE_HOLD,						    /*除了指定呼叫外,保持所有激活呼叫*/
	CS_CMD_TYPE_3WC_START,                      /*开始三方通话*/
	#endif
	CS_CMD_TYPE_DDSETEX,					    /*DDSETEX命令*/
	CS_CMD_TYPE_CLIP,					        /*开启CLIP命令*/
	CS_CMD_TYPE_INVALID = 0xFFFFFFFF			/*无效命令*/
}E_CS_CMD_TYPE;

/*CS消息类型*/
typedef enum CS_MSG_TYPE_E_
{
    /*ATCMD消息段*/
    CS_MSG_AT_BEGIN = 0x00000000L,          /*ATCMD消息开始*/

    /*无线模块下发给业务的命令*/
    CS_MSG_AT_RESULT_REPORT,                /*AT执行结果上报*/
    CS_MSG_AT_RING_REPORT,                  /*来电响铃指示*/
    CS_MSG_AT_CLIP_REPORT,                  /*来电显示指示*/
    CS_MSG_AT_ORIG_REPORT,                  /*呼叫发起指示*/
    CS_MSG_AT_CONF_REPORT,                  /*网络连通指示*/
    CS_MSG_AT_CONN_REPORT,                  /*通话建立指示*/
    CS_MSG_AT_CEND_REPORT,                  /*通话结束指示*/
    CS_MSG_AT_SYSINFO_REPORT,               /*SysInfo指示*/
    CS_MSG_AT_SIM_STATUS_REPORT,            /*SIM卡状态变化指示*/
    //#ifdef CS_SUPPLY_SERVICE
    /*cs_20080828         chenkeyun       begin*/
    CS_MSG_AT_CCWA_REPORT,                  /*呼叫等待来电指示*/
    CS_MSG_AT_CSSI_REPORT,                  /*AT上报补充业务指示CSSI处理*/
    CS_MSG_AT_CSSU_REPORT,                  /*AT上报补充业务指示CSSU处理*/
    /*cs_20080828         chenkeyun       end*/
    //#endif

    /*业务下发给无线模块的命令*/
    CS_MSG_AT_DIAL,                         /*业务发起呼叫*/
    CS_MSG_AT_ACCEPT,                       /*业务接听*/
    CS_MSG_AT_CHUP,                         /*业务挂断呼叫*/
    CS_MSG_AT_CALL_TIME_QUERY,              /*通话时长查询*/
    CS_MSG_AT_END = 0x00000100,             /*AT消息结束*/


    /*语音应用消息段*/
    CS_MSG_VOICEAPP_BEGIN = 0x00000101,     /*语音应用消息开始*/

    /*CS给SC的消息*/
    CS_MSG_VOICEAPP_STATUS_UPDATE,          /*CS线路状态更新事件*/
    CS_MSG_VOICEAPP_INCOMING,               /*CS来电，呼入*/
    CS_MSG_VOICEAPP_CANCLE,                 /*CS取消来电*/
    CS_MSG_VOICEAPP_ENDCALL,                /*CS挂机，远端挂机*/

    /*SC给CS的消息*/
    CS_MSG_VOICEAPP_OUTGOING,               /*主叫呼出*/
    CS_MSG_VOICEAPP_ACCEPT,                 /*被叫受理呼叫*/
    //#ifdef CS_SUPPLY_SERVICE
    CS_MSG_VOICEAPP_OP_SERVICE,             /*补充业务使能消息*/
    CS_MSG_VOICEAPP_HOLD,                   /*保持正在进行通话的呼叫*/
    CS_MSG_VOICEAPP_RECOVER,                /*恢复被保持的呼叫*/
    CS_MSG_VOICEAPP_CW,                     /*呼叫等待*/
    CS_MSG_VOICEAPP_CT,                     /*呼叫转移*/
    CS_MSG_VOICEAPP_3WC_START,              /*开始三方通话*/
    CS_MSG_VOICEAPP_3WC_END,                /*结束三方通话*/
    //#endif
    CS_MSG_VOICEAPP_OFFHOOK,                /*被叫摘机接听*/
    CS_MSG_VOICEAPP_ONHOOK,                 /*本端挂机*/
    CS_MSG_VOICEAPP_END = 0x00000200,       /*语音应用消息结束*/


    /*定时器消息段*/
    CS_MSG_TIMER_BEGIN = 0x00000201,        /*定时器消息开始*/
    CS_MSG_TIMER,                           /*定时器消息*/
    CS_MSG_TIMER_END = 0x00000300,          /*定时器消息结束*/


    /*话机消息段*/
    CS_MSG_PHONE_BEGIN = 0x00000301,        /*话机消息开始*/
    CS_MSG_PHONE,                           /*话机消息*/
    CS_MSG_PHONE_END = 0x00000400,          /*话机消息结束*/


    /*USB消息段*/
    CS_MSG_USB_BEGIN = 0x00000401,          /*USB消息开始*/
    CS_MSG_USB_STATUS_UPDATE,               /*USB工作状态变化消息*/
    CS_MSG_USB_END = 0x00000500,            /*USB消息结束*/


    CS_MSG_INVALID = 0xFFFFFFFF
}CS_MSG_TYPE_E;

/*上行线路类型*/
typedef enum _E_VOICE_UP_TYPE
{
    VOICE_UP_TYPE_CS = 0,                   /*CS上行*/
    VOICE_UP_TYPE_ADSL = 1,                 /*ADSL上行*/
    VOICE_UP_TYPE_INVALID = 0xFFFFFFFF
}E_VOICE_UP_TYPE;

/*USB工作状态*/
typedef enum CS_USB_STATUS_E_
{
    CS_USB_STATUS_FAULT,                    /*故障*/
    CS_USB_STATUS_OK,                       /*正常工作*/
    CS_USB_STATUS_BUTT,
}CS_USB_STATUS_E;

/*CS线路状态*/
typedef enum CS_STATUS_E_
{
    CS_STATUS_FAULT,                        /*故障，不能外呼*/
    CS_STATUS_UNREG,                        /*未注册，只能打紧急号码*/
    CS_STATUS_REG,                          /*已注册，可以打所有电话*/
    CS_STATUS_BUTT,
}CS_STATUS_E;

/*系统服务状态*/
typedef enum CS_SERVER_STATUS_E_
{
    CS_SERVER_STATUS_NO,                    /*无服务状态*/
    CS_SERVER_STATUS_PART,                  /*有限制服务状态*/
    CS_SERVER_STATUS_OK,                    /*服务有效状态*/
    CS_SERVER_STATUS_PARTZONE,              /*有限制的区域服务状态*/
    CS_SERVER_STATUS_SLEEP,                 /*省电或深睡状态*/
    CS_SERVER_STATUS_INVALID = 0xFFFFFFFF
}CS_SERVER_STATUS_E;

/*系统服务域状态*/
typedef enum CS_ZONE_STATUS_E_
{
    CS_ZONE_STATUS_NO,                      /*无服务*/
    CS_ZONE_STATUS_OK_CS,                   /*CS服务*/
    CS_ZONE_STATUS_OK_PS,                   /*PS服务*/
    CS_ZONE_STATUS_OK_CS_PS,                /*CS和PS服务均OK*/
    CS_ZONE_STATUS_NOK_CS_PS,               /*CS和PS服务均未注册，并处于搜索状态*/
    CS_ZONE_STATUS_FAIL = 250,              /*未获取到数值，无意义*/
    CS_ZONE_STATUS_NO_SUPPORT = 255,        /*CDMA不支持*/
    CS_ZONE_STATUS_INVALID = 0xFFFFFFFF
}CS_ZONE_STATUS_E;

/*CS漫游状态*/
typedef enum CS_ROAM_STATUS_E_
{
    CS_ROAM_STATUS_YES,                     /*漫游状态*/
    CS_ROAM_STATUS_NO,                      /*非漫游状态*/
    CS_ROAM_STATUS_INVALID = 0xFFFFFFFF
}CS_ROAM_STATUS_E;

/*CS系统模式*/
typedef enum CS_SYS_MODE_E_
{
    CS_SYS_MODE_NOSERVER,                   /*无服务*/
    CS_SYS_MODE_AMPS,                       /*AMPS模式*/
    CS_SYS_MODE_CDMA,                       /*CDMA模式*/
    CS_SYS_MODE_GSM_GPRS,                   /*GSM\GPRS模式*/
    CS_SYS_MODE_HDR,                        /*HDR模式*/
    CS_SYS_MODE_WCDMA,                      /*WCDMA模式*/
    CS_SYS_MODE_GPS,                        /*GPS模式*/
    CS_SYS_MODE_INVALID = 0xFFFFFFFF
}CS_SYS_MODE_E;

/*SIM卡状态*/
typedef enum CS_SIM_STATUS_E_
{
    CS_SIM_STATUS_UNUSABLE,                 /*不可用*/
    CS_SIM_STATUS_USABLE,                   /*可用*/
    CS_SIM_STATUS_NO_CS,                    /*CS不可用*/
    CS_SIM_STATUS_NO_PS,                    /*PS不可用*/
    CS_SIM_STATUS_NO_CS_PS,                 /*CS和PS都不可用*/
    CS_SIM_STATUS_ROM = 240,                /*ROMSIM版本*/
    CS_SIM_STATUS_FAIL = 250,               /*没有获取到数值，无意义*/
    CS_SIM_STATUS_NOEXIST = 255,            /*不存在*/
    CS_SIM_STATUS_INVALID = 0xFFFFFFFF
}CS_SIM_STATUS_E;

/*SIM卡锁定状态*/
typedef enum CS_SIMLOCK_STATUS_E_
{
    CS_SIMLOCK_STATUS_YES,                  /*锁定状态*/
    CS_SIMLOCK_STATUS_NO,                   /*非锁定状态*/
    CS_SIMLOCK_STATUS_INVALID = 0xFFFFFFFF
}CS_SIMLOCK_STATUS_E;

/*CS子系统模式*/
typedef enum CS_SUBSYS_MODE_E_
{
    CS_SUBSYS_MODE_NOSERVER,                /*无服务*/
    CS_SUBSYS_MODE_GSM,                     /*GSM模式*/
    CS_SUBSYS_MODE_GPRS,                    /*GPRS模式*/
    CS_SUBSYS_MODE_EDGE,                    /*EDGE模式*/
    CS_SUBSYS_MODE_WCDMA,                   /*WCDMA模式*/
    CS_SUBSYS_MODE_HSDPA,                   /*HSDPA模式*/
    CS_SUBSYS_MODE_HSUPA,                   /*HSUPA模式*/
    CS_SUBSYS_MODE_CS,                      /*HSDPA&HSUPA模式*/
    CS_SUBSYS_MODE_INVALID = 0xFFFFFFFF
}CS_SUBSYS_MODE_E;

//#ifdef CS_SUPPLY_SERVICE
/*CS补充业务指示CSSI类型*/
typedef enum CS_CSSI_TYPE_E_
{
    CS_CSSI_TYPE_CFU_ENABLE = 0,        /*无条件呼叫转移被激活*/
    CS_CSSI_TYPE_CT_ENABLE,             /*某一个呼叫转移被激活*/
    CS_CSSI_TYPE_CT_CALL,               /*呼叫被转移*/
    CS_CSSI_TYPE_CW_CALL,               /*呼叫处于等待状态*/
    CS_CSSI_TYPE_CUG_CALL,              /*这是一个CUG呼叫*/
    CS_CSSI_TYPE_OUT_FORBID,            /*呼叫被禁止*/
    CS_CSSI_TYPE_IN_FORBID,             /*来电被禁止*/
    CS_CSSI_TYPE_FORBID_CLIR_FAIL,      /*禁止CLIR被拒绝*/
    CS_CSSI_TYPE_CF_SUC,                /*呼叫被前转 (建立MT呼叫)*/

    CS_CSSI_TYPE_BUTT
}CS_CSSI_TYPE_E;

/*CS补充业务指示CSSU类型*/
typedef enum CS_CSSU_TYPE_E_
{
    CS_CSSU_TYPE_CT_CALL = 0,           /*被转移的呼叫（建立MT呼叫）*/
    CS_CSSU_TYPE_CUG_CALL,              /*CUG 呼叫（建立MT呼叫）*/
    CS_CSSU_TYPE_HOLD,                  /*通话被保持（通话中）*/
    CS_CSSU_TYPE_RECOVER,               /*通话被恢复（通话中）*/
    CS_CSSU_TYPE_ENTER_3WC,             /*加入会议通话 (通话中)*/
    CS_CSSU_TYPE_RELEASE,               /*被保持的呼叫被释放(通话中)*/
    CS_CSSU_TYPE_CF_INFO,               /*收到前转信息*/
    CS_CSSU_TYPE_RMT_RING,              /*对方处于响铃状态，呼叫在被接通过程中(通话中)*/
    CS_CSSU_TYPE_CALLING,               /*呼叫已经被接通(通话中或建立MT呼叫)*/
    CS_CSSU_TYPE_CF_SUC,                /*呼叫被前转 (建立MT呼叫)*/
    CS_CSSU_TYPE_CF_OTHER,              /*另外的来电前转*/

    CS_CSSU_TYPE_BUTT
}CS_CSSU_TYPE_E;
//#endif

/*CS语音模块消息结构*/
typedef struct tagCS_MSG_ST
{
	signed int  lMsgType;
	char        acText[AT_RCV_MSG_LENGTH];
}CS_MSG_S;

/*CS语音模块APP事件消息结构*/
typedef struct tagCS_MSG_APP_ST
{
    unsigned int    ulEventType;                    /*事件类型*/
    unsigned int    ulCrSn;                         /*CR索引*/
    unsigned int    ulUeSn;                         /*UE索引*/
    unsigned char   aucCallNum[CS_MAX_CALLID_LEN];  /*呼叫号码*/
}CS_MSG_APP_S;

/*****************************************************************************
** 命令功能: USB状态变化
** 命令格式: 
** 响应格式: 
******************************************************************************/
typedef struct tagCS_MSG_USB_STATUS_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucStatus;       /*见CS_USB_STATUS_E*/
}CS_MSG_USB_STATUS_S;

/*****************************************************************************
** 命令功能: AT执行结果上报
** 命令格式: 
** 响应格式: 
******************************************************************************/
typedef struct tagCS_MSG_RESULT_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucType;
}CS_MSG_RESULT_S;

/*****************************************************************************
** 命令功能: 来电响铃指示RING
** 命令格式: 
** 响应格式: <CR><LF>RING<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_RING_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
}CS_MSG_RING_S;

/*****************************************************************************
** 命令功能: 来电号码显示+CLIP
** 命令格式: 
** 响应格式: <CR><LF>+CLIP:<number>,<type>,,,,<CLI validity><CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CLIP_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucTelType;      /*号码类型，其中145表示国际号码，具体取值可参见
                                      “短信发送命令 +CMGS”一节中，SC号码中的
                                      type_addr参数的具体定义。255,CDMA不支持*/
    unsigned char   ucCliFlag;      /*0 CLI 有效
                                      1 CLI 被呼叫发起方拒绝.
                                      2 由于发起网络的限制或网络问题造成CLI不可用。
                                      <type>与<CLI validity>之间保留了三个字段。*/
    char    aucTelNum[CS_MAX_CALLID_LEN]; /*来电号码，ASCII 字符，合法的字符包括:
                                            ’0’ – ‘9’, ‘*’, ‘#’*/
}CS_MSG_CLIP_S;

/*****************************************************************************
** 命令功能: 呼叫受理指示^ORIG
** 命令格式: 
** 响应格式: <CR><LF>^ORIG<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ORIG_ST
{
    unsigned char   ucCmdFrom;      /*消息来源模块*/
    unsigned char   ucResult;       /*响应结果: 1为OK;0为NOK*/
    unsigned char   ucCallId;       /*呼叫标识CallId*/
    unsigned char   ucTelType;      /*号码类型，其中145表示国际号码，具体取值可参见
                                      “短信发送命令 +CMGS”一节中，SC号码中的
                                      type_addr参数的具体定义。255,CDMA不支持*/
}CS_MSG_ORIG_S;

/*****************************************************************************
** 命令功能: 连接建立指示^CONF
** 命令格式: 
** 响应格式: <CR><LF>^CONF<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CONF_ST
{
    unsigned char   ucCmdFrom;      /*消息来源模块*/
    unsigned char   ucResult;       /*响应结果: 1为OK;0为NOK*/
    unsigned char   ucCallId;       /*呼叫标识CallId*/
    unsigned char   ucTelType;      /*号码类型，其中145表示国际号码，具体取值可参见
                                      “短信发送命令 +CMGS”一节中，SC号码中的
                                      type_addr参数的具体定义。255,CDMA不支持*/
}CS_MSG_CONF_S;

/*****************************************************************************
** 命令功能: 通话建立指示^CONN
** 命令格式: 
** 响应格式: <CR><LF>^CONN<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CONN_ST
{
    unsigned char   ucCmdFrom;      /*消息来源模块*/
    unsigned char   ucResult;       /*响应结果: 1为OK;0为NOK*/
    unsigned char   ucCallId;       /*呼叫标识CallId*/
    unsigned char   ucTelType;      /*号码类型，其中145表示国际号码，具体取值可参见
                                      “短信发送命令 +CMGS”一节中，SC号码中的
                                      type_addr参数的具体定义。255,CDMA不支持*/
}CS_MSG_CONN_S;

/*****************************************************************************
** 命令功能: 通话结束指示^CEND
** 命令格式: 
** 响应格式: <CR><LF>^CEND:<call_x>,< duration >,<end_status>[,<cc_cause>]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CEND_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucCallId;       /*呼叫ID，EC取值0－6*/
    unsigned char   ucEndCause;     /*呼叫结束原因，EC取值255*/
    unsigned int    ulDuration;     /*通话时长，以s为单位*/
    unsigned char   ucReserve[3];   /*保留字节，扩展和对齐用*/
    unsigned char   auCallCtlCause; /*呼叫控制信息 EC取值255
                             注：如果为网侧引发的呼叫结束才会有字段<cc_cause>的上报
                             如果本地发出的呼叫，还没有得到网侧的响应，
                             呼叫就结束，此时就不会有<cc_cause>上报。*/
}CS_MSG_CEND_S; 

/*****************************************************************************
** 命令功能: 系统的信息查询
** 命令格式: AT^SYSINFO
** 响应格式: <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,< roam_status >,
             < sys_mode >,< sim_state >[,<lock_state>,<sys_submode>]<CR><LF><CR>
             <LF>OK<CR><LF> <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,
             < roam_status >,< sys_mode >,< sim_state >[,<lock_state>,
             <sys_submode>]<CR><LF><CR><LF>OK<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_SYSINFO_ST
{
	unsigned char   ucCmdFrom;
	unsigned char   ucResult;
	unsigned char   ucSrvStatus;    /*系统服务状态.见CS_SERVER_STATUS_E,取值如下
                                      0:  无服务
                                      1:  有限制服务
                                      2:  服务有效 
                                      3:  有限制的区域服务
                                      4:  省电和深睡状态            */
	unsigned char   ucSrvDomain;    /*系统服务域.见CS_ZONE_STATUS_E,取值如下
                                      0：    无服务
                                      1：    仅CS服务
                                      2：    仅PS服务
                                      3：    PS+CS服务
                                      4：    CS、PS均未注册，并处于搜索状态
                                      255: CDMA不支持。
        					          250: 表示未获取到数值，无意义 */
	unsigned char   ucRoamStatus;   /*漫游状态.见CS_ROAM_STATUS_E,取值如下
                                      0：非漫游状态
                                      1：漫游状态                   */
	unsigned char   ucSysMode;      /*系统模式.见CS_SYS_MODE_E,取值如下
        							  0：无服务
        							  1：AMPS模式（暂不使用）
        							  2：CDMA模式（暂不使用）
        							  3：GSM/GPRS 模式
        							  4：HDR模式
        							  5：WCDMA模式
        							  6：GPS模式                    */	
	unsigned char   ucSimState;     /*SIM卡状态.见CS_SIM_STATUS_E,取值如下
    	                              0：  USIM卡状态无效
    	                              1:   USIM卡状态有效  
    	                              2：  USIM在CS下无效（EW）
    	                              3：  USIM在PS下无效(EW)
    	                              4：  USIM在PS+CS下均无效(EW)
    	                              240: ROMSIM版本
    	                              255: USIM卡不存在   
    					              250: 表示未获取到数值，无意义 */
	unsigned char   ucLockState;    /*SIM卡的LOCK状态.见CS_SIMLOCK_STATUS_E,取值如下
                                      0：SIM卡未被SIMLOCK功能锁定
                                      1：SIM卡被SIMLOCK功能锁定     */
	unsigned char   ucSysSubMode;   /*见CS_SUBSYS_MODE_E
	                                  0：无服务
    					              1：GSM模式
    					              2：GPRS模式
    					              3：EDGE模式
    					              4：WCDMA模式
    					              5：HSDPA模式
    					              6：HSUPA模式
        							  7：HSDPA&HSUPA模式            */
	unsigned char   ucExtern1;
	unsigned char   ucExtern2;
	unsigned char   ucExtern3;
}CS_MSG_SYSINFO_S;

/*****************************************************************************
** 命令功能: SIM 状态变化指示
** 命令格式: 
** 响应格式: <CR><LF>^SIMST:<sim_state>[,<lock_state>]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_SIM_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   ucSimState;    /*USIM卡状态，取值如下：
                                     0:   USIM卡状态无效
                                     1:   USIM卡状态有效  
                                     2:   USIM在CS下无效
                                     3:   USIM在PS下无效
                                     4:   USIM在PS+CS下均无效
                                     255: USIM卡不存在，包括USIM卡确实不存在和因为
                                      被SIMLOCK锁住而不存在两种情况。此时，应通
                                      过ucLockState的取值判定USIM卡的真实状态*/
    unsigned char   ucLockState;    /*SIM卡的LOCK状态，取值如下：
                                    0: SIM卡未被SIMLOCK功能锁定
                                    1: SIM卡被SIMLOCK功能锁定*/
}CS_MSG_SIM_S;

/* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
/*****************************************************************************
** 命令功能: 拨号状态变化指示
** 命令格式: 
** 响应格式: <CR><LF>^NDISSTAT:<stat>[,<err_code>[,<wx_state>[,<PDP_type>]]]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_NDISSTAT_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   ucNdisState;    /*NDIS状态，取值如下：
                                     0：连接断开；
									 1：已连接；
									 2：连接中（仅在单板自动连接时上报，暂不支持）；
									 3：断开中（仅在单板自动连接时上报，暂不支持）。*/
	unsigned char   wx_state;		/*整型值，WiMax 数据卡子状态，暂不支持。*/	
    unsigned int   err_code;       /*整型值，错误码，仅在拨号失败时上报该参数。*/
	unsigned char	PDP_type[10];  /*"IPV4" "IPV6"*/
}CS_MSG_NDISSTAT_S;

/*****************************************************************************
** 命令功能: IPV4拨号状态DHCP命令结果
** 命令格式: 
** 响应格式: <CR><LF>^DHCP:<clip>,<netmask>,<gate>,<dhcp>,<pDNS>,<sDNS>,<max_rx_data>,<max_tx_data><CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_DHCP_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   clip[10];    
    unsigned char   netmask[10];  
	unsigned char   gate[10];
	unsigned char   dhcp[10];
	unsigned char   pDNS[10];
	unsigned char   sDNS[10];
	unsigned int   max_rx_data;
	unsigned int   max_tx_data;
}CS_MSG_DHCP_S;

/*****************************************************************************
** 命令功能: IPV6拨号状态DHCP命令结果
** 命令格式: 
** 响应格式: <CR><LF>^DHCPV6: <clip_v6>,<netmask_v6>,<gate_v6>,<dhcp_v6>,<pDNS_v6>,<sDNS_v6>,<max_rx_data>,<max_tx_data><CR><LF><CR><LF>OK<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_DHCPV6_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   clip_v6[128];    
    unsigned char   netmask_v6[128];  
	unsigned char   gate_v6[128];
	unsigned char   dhcp_v6[128];
	unsigned char   pDNS_v6[128];
	unsigned char   sDNS_v6[128];
	unsigned int   max_rx_data;
	unsigned int   max_tx_data;
}CS_MSG_DHCPV6_S;
#endif
/* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */

/*****************************************************************************
** 命令功能: 通话时长查询
** 命令格式: AT^CDUR=<idx>
** 响应格式: <CR<LF>^cdur:<idx>,<duration><CR<LF><CR<LF>OK<CR<LF>
******************************************************************************/
typedef struct tagCS_MSG_CDUR_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucExtern1;
    unsigned char   ucExtern2;
    unsigned int    ulDuration;         /*通话时长，以s为单位*/
}CS_MSG_CDUR_S;

/*****************************************************************************
** 命令功能: 业务发起呼叫
** 命令格式: ATD[digits][I/i][;]
** 响应格式: <CR><LF>OK<CR><LF>执行错误<CR><LF>NO CARRIER<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ATD_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
}CS_MSG_ATD_S;

/*****************************************************************************
** 命令功能: 业务接听来电
** 命令格式: ATA
** 响应格式: <CR><LF>OK<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ATA_ST
{
    unsigned char   ucCmdFrom; 
    unsigned char   ucResult;
}CS_MSG_ATA_S;

/*cs_20080828       chenkeyun       end*/
/*****************************************************************************
** 命令功能: 呼叫等待激活时的来电指示+CCWA
** 命令格式: ATA
** 响应格式: +CCWA:<number>,<type>,<class>,[<alpha>][,<CLIvalidity>[,<subaddr>,<satype> [,<priority> ]]]
取值说明
<n> （禁止/使能result code的主动上报）：
0	禁止
1	使能
<mode>：
0	去激活
1	激活
2	状态查询
<classx>：业务类别，缺省值为7，代表语音、数据和传真业务的组合。目前只支持语音业务，取值为1。
1	语音业务
2	数据业务
4	传真
8	短消息
16	同步电路数据
32	异步电路数据
64	专用分组接入
128	专用PAD接入
<status>：
0	未激活
1	激活
<number>：处于等待状态的来电号码
<type>：号码类型，其中145表示国际号码，具体取值可参见"呼叫前转 +CCFC"一节中，号码类型<type>参数的具体定义。
<alpha>：代表<number>的字符型数字文字。（保留不用 TBD）
<CLI validity>：
0	CLI 有效
1	CLI 被呼叫发起方保留.
2	由于发起网络的限制造成CLI不可用。
当CLI不可用（<CLI validity>=2），<number> 应该为空 （""） ，并且 <type>取值无意义。
<subaddr>字符型的号码子地址，格式由<satype>决定。（保留不用 TBD）
<satype>8bit编码的号码子地址格式。（保留不用 TBD）
<priority>数字型的参数，用来表示来电的优先级。（保留不用 TBD）
注：TBD参考3.3节中TBD说明。


******************************************************************************/
#ifndef ATP_PCLINT
typedef struct tagCS_MSG_CCWA_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   call_number[CS_MAX_CALLID_LEN]; 
    unsigned char   call_type;
    unsigned char   call_class;
}CS_MSG_CCWA_S;
#else
typedef struct tagCS_MSG_CCWA_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
                 char   call_number[CS_MAX_CALLID_LEN]; 
    unsigned char   call_type;
    unsigned char   call_class;
}CS_MSG_CCWA_S;
#endif
/*****************************************************************************
如果+CSSN命令设置使能+CSSI（<n>=1），则在建立MO呼叫中，接收到网络发来的补充业务指示时，上报TE指示
** 响应格式: +CSSI: <code1>[,<index>[,<number>,<type>
[,<subaddr>,<satype>]]]
取值说明
<code1>： 可能取值为0-8，与HOLD/RETRIEVE指示相关的取值为2和3。
0	无条件呼叫转移被激活
1	某一个呼叫转移被激活
2  呼叫被转移
3	  呼叫处于等待状态
4	 这是一个CUG呼叫
5 呼叫被禁止
6	来电被禁止 
7	禁止CLIR被拒绝
8	呼叫被前转
<index> 0-9, 10表示没有标号(保留不用，TBD)
<number>: 字符型电话号码，格式由<type>决定(保留不用，TBD)
<type>: 号码类型，具体取值可参见"呼叫前转 +CCFC"一节中，号码类型<type>参数的具体定义。(保留不用，TBD)
<subaddr>:字符串型分机号码，具体格式由<satype>决定(保留不用，TBD)
<satype>:分机号码的类型，8bit编码的整型格式(保留不用，TBD)
注：TBD说明,参考3.7.3 TBD说明

******************************************************************************/
#ifndef ATP_PCLINT
/*modify for pclint*/
typedef struct tagCS_MSG_CSSI_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   call_code1; 
}CS_MSG_CSSI_S;
#else
typedef struct tagCS_MSG_CSSI_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
                 char  * call_code1; 
}CS_MSG_CSSI_S;
#endif

/*****************************************************************************
如果+CSSN命令设置使能+CSSU（<m>=1），则在通话过程中和建立MT呼叫中，接收到网络发来的补充业务指示时，上报TE指示
取值说明
<code2>： 可能取值为0-10，与HOLD/RETRIEVE指示相关的取值为2和3。
0	被转移的呼叫（建立MT呼叫）
1	CUG 呼叫（建立MT呼叫）
2  通话被保持（通话中）
3	通话被恢复（通话中）
4	加入会议通话 (通话中)
5	被保持的呼叫被释放(通话中)
6	收到前转信息 
7	对方处于响铃状态，呼叫在被接通过程中(通话中)
8	呼叫已经被接通(通话中或建立MT呼叫)
9	呼叫被前转 (建立MT呼叫)
10	另外的来电前转
<index> 0-9, 10表示没有标号(保留不用，TBD)
<number>: 字符型电话号码，格式由<type>决定(保留不用，TBD)
<type>: 号码类型，具体取值可参见"呼叫前转 +CCFC"一节中，号码类型<type>参数的具体定义。(保留不用，TBD)
<subaddr>:字符串型分机号码，具体格式由<satype>决定(保留不用，TBD)
<satype>:分机号码的类型，8bit编码的整型格式(保留不用，TBD)
注（TBD说明）：从高通的代码看，这些参数都没有在代码中提供，原因是高通的代码是参考老的协议实现的，而且这些参数是可选的，所以建议：维持高通的代码。
******************************************************************************/
#ifndef ATP_PCLINT
typedef struct tagCS_MSG_CSSU_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   call_code2; 
}CS_MSG_CSSU_S;
/*cs_20080828       chenkeyun       end*/
#else
typedef struct tagCS_MSG_CSSU_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
                 char *  call_code2; 
}CS_MSG_CSSU_S;
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif  /*__CS_PUB_H__*/


