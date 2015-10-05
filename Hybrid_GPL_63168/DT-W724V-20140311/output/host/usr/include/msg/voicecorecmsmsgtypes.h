#ifndef __ATP_VOICECORECMS_MSGTYPES_H__
#define __ATP_VOICECORECMS_MSGTYPES_H__

#define ATP_CBBID_VOICECORE_NAME "voicecore"

enum ATP_VOICECORECMS_MSGTYPES_EN
{
    ATP_MSG_VOICECORECMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DTVOICECMS),
    ATP_MSG_VOICE_DIAG,
    ATP_MSG_VOICE_CFUPDATE,                  /*呼叫前缀配置更新*/
    ATP_MSG_VOICE_RESTORE,                   /*恢复默认配置*/
    ATP_MSG_VOICE_INIT_OK,
    ATP_MSG_VOICE_FXO_STATUS,                /*FXO口状态刷新*/
    ATP_MSG_VOICE_SAVE,                      /*写Flash*/
    ATP_MSG_VOICE_DECT_FWUPDATE,             /*dect升级*/
    ATP_MSG_VOICE_LED_STATUS,                /*led 灯状态*/
    ATP_MSG_WAN_WILLDOWN,                    /*wan 即将down*/
    ATP_MSG_VOICE_SET_VAD,                   /*命令行设置VAD是否使能*/
    ATP_MSG_VOICE_DECT_LINESET,              /*Dect LineSettings Update*/
	ATP_MSG_VOICE_DECT_NAME_UPDATE,          /*Dect List Change notification*/
	ATP_MSG_VOICE_DECT_PIN,                  /*Systerm pin code update by HS*/
	ATP_MSG_VOICE_DECT_WEBCFG,               /*Web Common Cfg Update Msg*/
	ATP_MSG_VOICE_DECT_CFINFO,               /*Dect CF*/
	ATP_MSG_VOICE_DECT_BRESET,               /*Base Reset*/
	ATP_MSG_VOICE_DECT_NEMOMODE,             /*Nemo Mode*/
	ATP_MSG_VOICE_DECT_SUBSDATA_SAVE,        /*保存子机注册信息*/	
    ATP_MSG_VOICE_DECT_HS_WEBUNREG,          /*Web Dect注销*/  
	/*BEGIN:added by l00192646 ,2012/06/05,for wt-104*/
	ATP_MSG_VOICE_DECT_RFPICODE,     /*保存Base  RFPI码*/
    ATP_MSG_DECT_UPDATE_SUCCESS,    /*保存子机的硬件软件版本号*/
	ATP_MSG_DECT_VERSION_UPDATE,    /*保存子机成功完成suota升级的时间 */
	/*END:added by l00192646 ,2012/06/05,for wt-104*/
	
	/* BEGIN: Added by zengjun 182276, 2011/5/27 :DT特性拨号历史擦除消息*/
    ATP_MSG_CMS_CALLLIST_ERASURE,
/* END:   Added by zengjun 182276, 2011/5/27 */
	/* BEGIN: Added by z00182276, 2011/8/3 HOMEGW-10905 加入CW功能操作码 */
    ATP_MSG_VOICE_CWUPDATE,                  /*呼等配置更新*/
    /* END:   Added by z00182276, 2011/8/3  */
    /*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
    ATP_MSG_VOICE_SAVECALLLOG,          /*保存calllog*/
    /*END: Added by l00180792 @20120627 for wt-104 calllog*/
    ATP_MSG_CMS_WLAN_ENABLE,
    ATP_MSG_CMS_WLAN_DISABLE,

	/*VOIP点灯相关消息*/
	ATP_MSG_VOICE_VOIP_NEGOTIATION_STATUS,			/**/
	ATP_MSG_VOICE_VOIP_REGISTERER_UP_STATUS,
	ATP_MSG_VOICE_VOIP_TRAFFFIC_IN_STATUS,
    ATP_MSG_VOICE_VOIP_REGISTERER_DISABLED_STATUS,
    ATP_MSG_VOICE_VOIP_CONFIGURATION_NO_STATUS,
    ATP_MSG_VOICE_VOIP_CHANGE_STATUS,   
    ATP_MSG_VOICE_DECT_STOP, //Start by z00182276 for UDK0000340477,在结束wps时同时终止dect对码流程 at 2014.2.14

};

enum ATP_VOICE_VOIP_SERVICE_TYPE_EN
{
    ATP_VOICE_VOIP_SERVICE = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DTVOICECMS) + 100,
};

enum ATP_VOICE_VOIP_STATUS_EN
{
	ATP_VOICE_VOIP_NEGOTIATION_STATUS,			/**/
	ATP_VOICE_VOIP_REGISTERER_UP_STATUS,
	ATP_VOICE_VOIP_TRAFFIC_IN_STATUS,
    ATP_VOICE_VOIP_REGISTERER_DISABLED_STATUS,
    ATP_VOICE_VOIP_CONFIGURATION_NO_STATUS,
};


#endif // End of __ATP_VOICECORECMS_MSGTYPES_H__

