#ifndef __ATP_VOICECORECMS_MSGTYPES_H__
#define __ATP_VOICECORECMS_MSGTYPES_H__

#define ATP_CBBID_VOICECORE_NAME "voicecore"

enum ATP_VOICECORECMS_MSGTYPES_EN
{
    ATP_MSG_VOICECORECMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DTVOICECMS),
    ATP_MSG_VOICE_DIAG,
    ATP_MSG_VOICE_CFUPDATE,                  /*����ǰ׺���ø���*/
    ATP_MSG_VOICE_RESTORE,                   /*�ָ�Ĭ������*/
    ATP_MSG_VOICE_INIT_OK,
    ATP_MSG_VOICE_FXO_STATUS,                /*FXO��״̬ˢ��*/
    ATP_MSG_VOICE_SAVE,                      /*дFlash*/
    ATP_MSG_VOICE_DECT_FWUPDATE,             /*dect����*/
    ATP_MSG_VOICE_LED_STATUS,                /*led ��״̬*/
    ATP_MSG_WAN_WILLDOWN,                    /*wan ����down*/
    ATP_MSG_VOICE_SET_VAD,                   /*����������VAD�Ƿ�ʹ��*/
    ATP_MSG_VOICE_DECT_LINESET,              /*Dect LineSettings Update*/
	ATP_MSG_VOICE_DECT_NAME_UPDATE,          /*Dect List Change notification*/
	ATP_MSG_VOICE_DECT_PIN,                  /*Systerm pin code update by HS*/
	ATP_MSG_VOICE_DECT_WEBCFG,               /*Web Common Cfg Update Msg*/
	ATP_MSG_VOICE_DECT_CFINFO,               /*Dect CF*/
	ATP_MSG_VOICE_DECT_BRESET,               /*Base Reset*/
	ATP_MSG_VOICE_DECT_NEMOMODE,             /*Nemo Mode*/
	ATP_MSG_VOICE_DECT_SUBSDATA_SAVE,        /*�����ӻ�ע����Ϣ*/	
    ATP_MSG_VOICE_DECT_HS_WEBUNREG,          /*Web Dectע��*/  
	/*BEGIN:added by l00192646 ,2012/06/05,for wt-104*/
	ATP_MSG_VOICE_DECT_RFPICODE,     /*����Base  RFPI��*/
    ATP_MSG_DECT_UPDATE_SUCCESS,    /*�����ӻ���Ӳ������汾��*/
	ATP_MSG_DECT_VERSION_UPDATE,    /*�����ӻ��ɹ����suota������ʱ�� */
	/*END:added by l00192646 ,2012/06/05,for wt-104*/
	
	/* BEGIN: Added by zengjun 182276, 2011/5/27 :DT���Բ�����ʷ������Ϣ*/
    ATP_MSG_CMS_CALLLIST_ERASURE,
/* END:   Added by zengjun 182276, 2011/5/27 */
	/* BEGIN: Added by z00182276, 2011/8/3 HOMEGW-10905 ����CW���ܲ����� */
    ATP_MSG_VOICE_CWUPDATE,                  /*�������ø���*/
    /* END:   Added by z00182276, 2011/8/3  */
    /*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
    ATP_MSG_VOICE_SAVECALLLOG,          /*����calllog*/
    /*END: Added by l00180792 @20120627 for wt-104 calllog*/
    ATP_MSG_CMS_WLAN_ENABLE,
    ATP_MSG_CMS_WLAN_DISABLE,

	/*VOIP��������Ϣ*/
	ATP_MSG_VOICE_VOIP_NEGOTIATION_STATUS,			/**/
	ATP_MSG_VOICE_VOIP_REGISTERER_UP_STATUS,
	ATP_MSG_VOICE_VOIP_TRAFFFIC_IN_STATUS,
    ATP_MSG_VOICE_VOIP_REGISTERER_DISABLED_STATUS,
    ATP_MSG_VOICE_VOIP_CONFIGURATION_NO_STATUS,
    ATP_MSG_VOICE_VOIP_CHANGE_STATUS,   
    ATP_MSG_VOICE_DECT_STOP, //Start by z00182276 for UDK0000340477,�ڽ���wpsʱͬʱ��ֹdect�������� at 2014.2.14

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

