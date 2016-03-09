/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : voicepub.h
  �� �� ��   : ����
  ��    ��   : l110333
  ��������   : 2009��5��8��
  ����޸�   :
  ��������   : ����ҵ��ģ�����ⲿģ�齻���ӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��5��8��
    ��    ��   : l110333
    �޸�����   : �����ļ�

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

/*�ʺ�֧������*/
#ifdef SUPPORT_ATP_VCE_WT104_CUSTOM             
#define VCE_VOIP_MAX                30      /*���֧�ֵ�VoIP�ʺŸ���*/
#else
#define VCE_VOIP_MAX                15      /*���֧�ֵ�VoIP�ʺŸ���*/
#endif
#define VCE_ISDN_MAX                10      /*���֧�ֵ�ISDN�ʺŸ���*/
#define VCE_PSTN_MAX                1       /*���֧�ֵ�PSTN�ʺŸ���*/
#define VCE_CS_MAX                  1       /*���֧�ֵ�CS�ʺŸ���*/
#define VCE_LINE_MAX (VCE_VOIP_MAX + VCE_ISDN_MAX + VCE_PSTN_MAX + VCE_CS_MAX) /*���֧�ֵ��ʺŸ���*/

/*�˿�֧������*/
#define VCE_FXS_MAX                 2       /*���֧��FXS�ڵĸ���*/
#define VCE_DECT_HS_MAX             5       /*���֧��DECT�ӻ��ĸ���*/
#define VCE_PORT_MAX (VCE_FXS_MAX + VCE_DECT_HS_MAX)    /*���֧�ֶ˿ڵ���Ŀ*/


/*�˺���������ֵ*/
#define VCE_VOIP_BASE           1                                   /*VOIP�����ʼֵ*/
#define VCE_ISDN_BASE           (VCE_VOIP_BASE + VCE_VOIP_MAX)      /*ISDN�����ʼֵ*/
#define VCE_PSTN_BASE           (VCE_ISDN_BASE + VCE_ISDN_MAX + 1)  /*PSTN�����ʼֵ,�����1�Ǽ�ISDN���ʺ�*/
#define VCE_CS_BASE             (VCE_PSTN_BASE + VCE_PSTN_MAX)      /*CS�����ʼֵ*/
#define VCE_FXS_INCALL_BASE     (VCE_CS_BASE + VCE_CS_MAX)          /*FXS�ڲ������˺������ʼֵ*/
#define VCE_DECT_INCALL_BASE    (VCE_FXS_INCALL_BASE + VCE_FXS_MAX) /*DECT�ڲ������˺������ʼֵ*/

/*ISDN���ʺ�����,���ʺ����ڵ�û��MSN����ʱISDN�����п���*/
#define VCE_ISDN_DFT    (VCE_ISDN_BASE + VCE_ISDN_MAX)              /*ISDNĬ��˳���(��MSN��������)*/

/*����ΪAutoʱDect���ݵ��˺�����*/
#define VCE_OUTGO_AUTO  (VCE_DECT_INCALL_BASE + VCE_DECT_HS_MAX)    /*ȡһ���ϴ�ֵ��Ϊ�Զ�ѡ·����*/

/*Dect LineSetting�������˺�����*/
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

#define VCE_PHONE_BOOK_MAX          250     /*���֧�ֵ绰����¼��Ŀ*/
#define VCE_MISSED_CALL_MAX         100     /*���֧��δ�������¼*/
#define VCE_INACC_CALL_MAX          100     /*���֧���ѽ������¼*/
#define VCE_OUT_CALL_MAX			100     /*���֧�ֺ��������¼*/
#define VCE_LINE_SRV_MAX            5       /*һ��sip�ʺ����֧�ֿ����õ�ע�����������*/
#define VCE_CODEC_MAX               3       /*�������͵ĸ���*/
#define VCE_PIN_LEN_MAX             32      /*PIN����󳤶�*/

#define SIP_MAX_NUM_LEN             64      /*�û�������󳤶�*/
#define SIP_MAX_NAME_LEN          (64+4)    /*�û�������󳤶�*/
#define SIP_MAX_PSW_LEN           (64+4)    /*�û�������󳤶�*/
#define SIP_MAX_URL_LEN             128     /*SIP URL��󳤶� */

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

#define VOICE_CODECLIST_MAX         3          /*���֧�ֱ��뷽ʽ����*/

/*BEGIN::added by l00192646 ,2012/06/04 for wt-104 extersion*/
#define VOICE_ALERT_TIMEOUT          5     /*Alert ���峬ʱ��ʱ����ʱ��(��)*/
/*END::added by l00192646 ,2012/06/04 for wt-104 extersion*/

/*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
#define VOICE_CALL_MAX         5          /*���֧��ͨ������*/
/*END: Added by l00180792 @20120627 for wt-104 calllog*/

/*BEGIN:Added by l00180792 @20120717 for QI*/
#define VOICE_DFT_QICALC_INTERVAL           10000       /* Ĭ�ϻ�ȡQI�Ķ�ʱ��ʱ��*/
/*BEGIN:Added by l00180792 @20120717 for QI*/

/*DTMF���ݴ�������*/
typedef enum tagDTMF_TYPE_E
{
    DTMF_TYPE_InBand = 0,  
    DTMF_TYPE_RFC2833, 
    DTMF_TYPE_SIPInfo,
}DTMF_TYPE_E;

typedef enum VOICE_MSG_TYPE
{
    /*������������Ϣ���Ͷ���*/
    VOICE_MSG_TYPE_TAPI = 8840,   	/*tapi��Ϣ*/
    VOICE_MSG_TYPE_TIMER,         	/*timer��Ϣ*/
    VOICE_MSG_TYPE_TPTD,          	/*tptd��Ϣ*/
    VOICE_MSG_TYPE_SC,				/*SC��Ϣ*/
    VOICE_MSG_TYPE_CS,				/*CS��Ϣ*/
    VOICE_MSG_TYPE_PM,				/*PM��Ϣ*/
    VOICE_MSG_TYPE_MIC,             /*MIC��Ϣ*/
#ifdef SUPPORT_DIAG_POTS_MNG
    VOICE_MSG_TYPE_DIAG,            /*DIAG��Ϣ*/
#endif
    VOICE_MSG_TYPE_LINEMNG,
    VOICE_MSG_TYPE_ISDN,

    /*������������Ϣ���Ͷ���*/
    VOICE_MSG_TYPE_CLI = 8900,      /*CLI��Ϣ*/
    VOICE_MSG_TYPE_CMS,           	/*���ù�����Ϣ*/
    VOICE_MSG_TYPE_WEB,           	/*���ù�����Ϣ*/
    VOICE_MSG_TYPE_REBOOT,         	/*web������Ϣ,���޸���ͬ������ATP_VOICE_MSG_REBOOT*/
    VOICE_MSG_TYPE_STOP,            /*web������Ϣ,���޸���ͬ������ATP_VOICE_MSG_STOP*/
    /*begin: added by l110333 for log*/
    VOICE_MSG_TYPE_WAN,             /*wan������Ϣ*/
    /*end: added by l110333 for log*/
    VOICE_MSG_TYPE_RESTORE,         /*�ָ�����������Ϣ*/
    VOICE_MSG_TYPE_ATS,             /*ATS�������ϱ���Voice��Ϣ*/

    VOICE_MSG_TYPE_BUTT
} VOICE_MSG_TYPE_E;

/*����CLI����Ϣ����*/
typedef enum VOICE_CLI_MSG_TYPE_EN_
{
    VOICE_CLI_MSG_START = 1,   	    /*��������*/
    VOICE_CLI_MSG_STOP,	            /*��ֹ����*/
    VOICE_CLI_MSG_SIP_REG,        	/*ע��sip�˺�*/
    VOICE_CLI_MSG_SIP_UNREG,        /*ע��sip�˺�*/
    VOICE_CLI_MSG_SIP_REG_REPORTOK, /*����SIP�����˺�״̬�����������ע��ɹ�*/
    VOICE_CLI_MSG_VOICE_TAPI,
    VOICE_CLI_MSG_VOICE_LINEMNG,
    VOICE_CLI_MSG_VOICE_SC,
    VOICE_CLI_MSG_VOICE_DPLAN,
    VOICE_CLI_MSG_VOICE_PSTN,
    VOICE_CLI_MSG_VOICE_SIP,
    VOICE_CLI_MSG_VOICE_CUSTOMER,
    VOICE_CLI_MSG_VOICE_DECT,
    VOICE_CLI_MSG_QUERY_CFG,        /*��ѯ����ҵ��������Ϣ*/
    VOICE_CLI_MSG_QUERY_TAPI_CFG,   /*��ѯtapi������Ϣ*/
    VOICE_CLI_MSG_CS_STAT,          /*��ѯCSע��ͺ���״̬*/
    VOICE_CLI_MSG_QUERY_SIPID,      /*��ѯSIP�ʺ���Ϣ*/
    VOICE_CLI_MSG_QUERY_SIPSRV,     /*��ѯSIP��������Ϣ*/
    VOICE_CLI_MSG_QUERY_SIP_STAT, 	/*��ѯsip����״̬��ע��״̬*/
    VOICE_CLI_MSG_DBG_LEVEL,      	/*������־����*/
    VOICE_CLI_MSG_DBG_TELNET,      	/*telnet���������Ϣ*/
    VOICE_CLI_MSG_DBG_BR0,      	/*BR0����*/
    VOICE_CLI_MSG_QUERY_REGINFO,	/*��ѯע����Ϣ*/
    VOICE_CLI_MSG_DBG_CONSOLE,      /*console�����Ϣ*/
    VOICE_CLI_MSG_UNKNOWN,

    VOICE_CLI_MSG_BUTT
}VOICE_CLI_MSG_TYPE_E;

/* ���ù����͵�ָʾ */
typedef enum VOICE_CMS_MSG_TYPE
{
    VOICE_CMS_MSG_SIP_REG,        	/*ע��sip�ʺ�*/
    VOICE_CMS_MSG_SIP_UNREG,        /*ע��sip�˺�*/
    VOICE_CMS_MSG_WAN_STAT,       	/*WAN״̬����*/
    VOICE_CMS_MSG_UPDATE_CFG,     	/*����������*/
    VOICE_CMS_MSG_EQUIPMENT_TSET,   /*װ������*/
    VOICE_CMS_MSG_QUERY_SIP_STAT, 	/*���ù�����Ϣ,��ѯsip����״̬��ע��״̬*/
    VOICE_CMS_MSG_DBG_LEVEL,      	/*���ù�����Ϣ,������־����*/
    VOICE_CMS_MSG_DBG_TELNET,      	/*���ù�����Ϣ,�������������Ϣ*/
    VOICE_CMS_MSG_QUERY_REGINFO,	/*��ѯע����Ϣ*/
    VOICE_CMS_MSG_OUTLINE_CHANGED,  /*֪ͨ������������״̬�仯 */
    VOICE_CMS_MSG_FXO_RELAYCTL,     /*FXO�̵�������*/
    VOICE_CMS_MSG_DIAGBUTTON,       /*��ϰ�ť��Ϣ*/
    VOICE_CMS_MSG_LINERESET,        /*SIP�˺�����*/
    VOICE_CMS_MSG_STATUSQUERY,      /*����ʺ�����״̬��Ϣ*/
    VOICE_CMS_MSG_CALLSTATUS_QUERY, /*��·����״̬��ѯ*/
    VOICE_CMS_MSG_ANIMSN,           /*��ȡmsn�˺���Ϣ*/
    VOICE_CMS_MSG_WAN_WILLDOWN,     /*֪ͨ��������wan����down*/ 
    /*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
    VOICE_CMS_MSG_UPDATE_CALLLOGPARAM, /*���µ�ǰͨ��ʱ��*/
    /*END: Added by l00180792 @20120627 for wt-104 calllog*/
    VOICE_CMS_MSG_FXSSTATUSQUERY,
    /************** DECT START *****************/
    VOICE_CMS_MSG_DECT_RESTORE = 0x80, /*Dect״̬����(ע��/ע������ı仯)*/
    VOICE_CMS_MSG_DECT_NAME,        /*Dect�ӻ����Ƹ���*/
    VOICE_CMS_MSG_DECT_LINESET,     /*Dect�ӻ�LineSettings����*/ 
    VOICE_CMS_MSG_DECT_PAGE,        /*Dect�ӻ�����*/
    VOICE_CMS_MSG_DECT_REGOPEN,     /*DECT�����ӻ�ע��*/
    VOICE_CMS_MSG_DECT_REGCLOSE,    /*DECT��ֹ�ӻ�ע��*/
    VOICE_CMS_MSG_DECT_HS_REG,      /*DECT�ӻ�ע���¼�*/
    VOICE_CMS_MSG_DECT_HS_UNREG,    /*DECT�ӻ�ע���¼�*/
	/*********************h00190880************************/	
    VOICE_CMS_MSG_DECT_RFSET,       /*DECT��Ƶ����*/
    VOICE_CMS_MSG_DECT_ANT0_TXSET,  /*����0�����¼�*/
    VOICE_CMS_MSG_DECT_ANT1_TXSET,  /*����1�����¼�*/
    VOICE_CMS_MSG_DECT_ANT0_TXDISPLAY,
    VOICE_CMS_MSG_DECT_ANT1_TXDISPLAY,
    VOICE_CMS_MSG_DECT_SETRGIQM,
    VOICE_CMS_MSG_DECT_GETRGIQM,
    VOICE_CMS_MSG_DECT_GETREGHS, 	/*DECT ��ȡ�ӻ��б���Ϣ*/
	/*********************h00190880************************/	
    VOICE_CMS_MSG_DECT_UARTSET,     /*DECT UART����*/
	/*end  of add by xkf38924,2011-05-31*/
    VOICE_CMS_MSG_DECT_SETNEMO,     /*DECT NEMO ģʽ����*/
    VOICE_CMS_MSG_DECT_SETECO,      /*DECT ECOģʽ,��Ƶ��������*/
    /* START ADD: x182424 2011-10-28 FOR DTS2011111002760 */
    VOICE_CMS_MSG_DECT_SETPCM,      /*PCM����ģʽ*/
    VOICE_CMS_MSG_DECT_PCMTEST,     /* pcm��ͨ�Բ��� */
    /* END ADD: x182424 2011-10-28 FOR DTS2011111002760 */
    VOICE_CMS_MSG_DECT_SETTIME,     /*DECT ʱ��ͬ��*/
    VOICE_CMS_MSG_DECT_SETPIN,      /*DECT PIN����*/
    VOICE_CMS_MSG_DECT_GETPIN,      /*DECT PIN��ȡ*/
    VOICE_CMS_MSG_DECT_SETCALIB,    /*DECT װ����������(RVBG,RVREF,RXTUN,RFPI)*/
    VOICE_CMS_MSG_DECT_GETCALIB,    /*DECT װ��������ȡ(RVBG,RVREF,RXTUN,RFPI)*/
    VICCE_CMS_MSG_DECT_GETVER,      /*DECT �汾�Ż�ȡ*/
    VOICE_CMS_MSG_DECT_TESTMODE,    /*DECT ����ģʽ����*/
    VOICE_CMS_MSG_DECT_EEPROMSET,   /*DECT �ֹ�дEEPROM*/
    VOICE_CMS_MSG_DECT_REBOOT,      /*DECT ����*/
    VOICE_CMS_MSG_DECT_TELBOOK,     /*DECT �绰�����ø���*/
    VOICE_CMS_MSG_DECT_UPDATE,      /*DECT ����*/
    VOICE_CMS_MSG_DECT_RELOCK,      /*DECT RELOCK��Base*/
    VOICE_CMS_MSG_DECT_REINIT,      /*DECT ���³�ʼ��*/
    VOICE_CMS_MSG_DECT_LOG,         /*DECT LOG*/    
    VOICE_CMS_MSG_DECT_EEPROMGET,   /*DECT �ֹ���EEPROM*/
    VOICE_CMS_MSG_DECT_GETSUBINFO,  /*DECT ��ȡע����Ϣ*/
    VOICE_CMS_MSG_DECT_SETSUBINFO,  /*DECT ����ע����Ϣ*/
    VOICE_CMS_MSG_DECT_GETBARCODE,  /*DECT ��ȡװ����Ϣ*/
    VOICE_CMS_MSG_DECT_SETBARCODE,  /*DECT ����װ����Ϣ*/

    VOICE_CMS_MSG_DECT_GETINITSTAT, /*DECT ��ȡ��ʼ��״̬*/
    VOICE_CMS_MSG_DECT_MISSCALLCHANGE,
	VOICE_CMS_MSG_DECT_LIST_CHANGE,
	VOICE_CMS_MSG_DECT_SETMWI,      /*DECT ���Ե�����*/
	VOICE_CMS_MSG_DECT_SETCMBSLINETYPE,    /*����CMBS Line Type,������֤Ҫ��*/
	VOICE_CMS_MSG_DECT_RESETALL,
	VOICE_CMS_MSG_DECT_CMSUPDATEMISS,
	VOICE_WEB_MSG_DECT_WEBUPDATEMISS,
	VOICE_CMS_MSG_DECT_CONTACTSNOTIFY,
	VOICE_CMS_MSG_DECT_NETPHONEBOOK_RESULT,  /*������Net Phone book ���󷵻صĽ��*/
	
	VOICE_CMS_MSG_DECT_GETMAILRES,
	VOICE_CMS_MSG_DECT_NEWMAILNOTIFY,
	VOICE_CMS_MSG_DECT_NEWRSSNOTIFY,
	VOICE_CMS_MSG_DECT_READCHANNELEND,
	VOICE_CMS_MSG_DECT_CHANGELISTRSS,
	VOICE_CMS_MSG_DECT_READCHANNELERR,
	VOICE_CMS_MSG_DECT_GETMAILERR,
	/*add by w00191600  2011-10-26  for �ӻ�RSS\Email�󶨹�ϵ�����仯ʱ�·�δ����Ϣ*/
	VOICE_CMS_MSG_DECT_MODIFYCHANNELRELATE,
	VOICE_CMS_MSG_DECT_MODIFYACCOUNTRELATE,
	/*end add by w00191600  2011-10-26  for �ӻ�RSS\Email�󶨹�ϵ�����仯ʱ�·�δ����Ϣ*/
	VOICE_CMS_MSG_DECT_RECORD_CALLINFO,
	VOICE_CMS_MSG_DECT_OPERAORSNAME,
    /************** DECT END *****************/
    VOICE_CMS_MSG_DECT_CATIQMODE,
    VOICE_CMS_MSG_DECT_ENCRYPTION,
    VOICE_CMS_MSG_UNKNOWN = 0xff,

} VOICE_CMS_MSG_EN;

typedef enum VOICE_ATS_MSG_TYPE
{
	VOICE_ATS_MSG_ATA_RSP            = 1,      /*�·���ATAָ���*/
	VOICE_ATS_MSG_CHUP_RSP           = 2,      /*�·���CHUPָ���*/
    VOICE_ATS_MSG_ATD_RSP            = 3,      /*�·���ATDָ���*/
    VOICE_ATS_MSG_DDSETEX_RSP        = 4,      /*�·���DDSETEXָ���*/ 
	VOICE_ATS_MSG_CHLD_RSP           = 5,      /*�·���CHLDָ���*/
	VOICE_ATS_MSG_CLIP_RSP           = 6,      /*�·���CLIPָ���*/
	VOICE_ATS_MSG_DTMF_RSP           = 7,      /*�·���DTMFָ���*/
	VOICE_ATS_MSG_CW_RSP             = 8,      /*�·���CWָ���*/
	
	VOICE_ATS_MSG_ORIG_REPORT        = 9,      /*ORIGָʾ�ϱ�*/
	VOICE_ATS_MSG_CLIP_REPORT        = 10,     /*CLIPָʾ�ϱ�*/
	VOICE_ATS_MSG_RING_REPORT        = 11,     /*RINGָʾ�ϱ�*/  
	VOICE_ATS_MSG_CONF_REPORT        = 12,     /*CONFָʾ�ϱ�*/  
	VOICE_ATS_MSG_CONN_REPORT        = 13,     /*CONNָʾ�ϱ�*/ 
	VOICE_ATS_MSG_CEND_REPORT        = 14,     /*CENDָʾ�ϱ�*/
	
	VOICE_ATS_MSG_SYSINFO_REPORT     = 15,     /*SYSINFOָʾ�ϱ�*/
	VOICE_ATS_MSG_SIMSTATUS_REPORT   = 16,     /*SIMSTATUSָʾ�ϱ�*/
	VOICE_ATS_MSG_CSSI_REPORT        = 17,     /*CSSIָʾ�ϱ�*/
	VOICE_ATS_MSG_CSSU_REPORT        = 18,     /*CSSUָʾ�ϱ�*/ 
	VOICE_ATS_MSG_CCWA_REPORT        = 19,     /*CCWAָʾ�ϱ�*/

    VOICE_ATS_MSG_USB_STATUS_UPDATE  = 20,
	
	VOICE_ATS_MSG_UNKNOWN = 0xff,
}VOICE_ATS_MSG_EN;

/* WAN����Ϣ */
typedef enum VOICE_WAN_MSG_TYPE
{
    ATP_UTIL_PPP_UP_MANUAL = 0,   // �ֶ��������ӳɹ�
    ATP_UTIL_PPP_UP_AUTO = 1,      // �Զ��������ӳɹ�
    ATP_UTIL_PPP_UP_DEMAND = 2,   // ���貦�Ž������ӳɹ�
    ATP_UTIL_PPP_UP_VOICE = 3,     // VOIPժ�������������ӳɹ�
    ATP_UTIL_PPP_DOWN_DSL = 4,   // DSL��·ʧ�ܵ������ӶϿ�
    ATP_UTIL_PPP_DOWN_ECHO = 5,   // PPP ECHO������Ӧ�������ӶϿ�
    ATP_UTIL_PPP_DOWN_TIMEOUT = 6,  // ���貦�ų�ʱ�������������ӶϿ�
    ATP_UTIL_PPP_FAIL_SVR = 7,  // BAS����Ӧ���²���ʧ��
    ATP_UTIL_PPP_FAIL_SVR_VOICE = 8, // BAS����Ӧ����VOICEժ�������Ĳ���ʧ��
    ATP_UTIL_PPP_FAIL_AUTH = 9,  // ��֤�����²���ʧ��
    ATP_UTIL_PPP_FAIL_AUTH_VOICE = 10,  // ��֤������VOICEժ�������Ĳ���ʧ��
    ATP_UTIL_PPP_FAIL_NO_NAS = 11,  // û��nas�ӿڣ���WAN PPP�ϵ�ʱ��ժ������ʧ�ܣ� 
}VOICE_WAN_MSG_EN;

/*���е�·������ö�ٶ���*/
typedef enum tagLINE_TYPE_EN
{
    LINE_TYPE_NULL      = 0x0,
    LINE_TYPE_PSTN      = (0x1 << 0),
    LINE_TYPE_SIP       = (0x1 << 1),
    LINE_TYPE_CS        = (0x1 << 2),
    LINE_TYPE_ISDN      = (0x1 << 3),
    LINE_TYPE_INCALL    = (0x1 << 4),   
    LINE_TYPE_MIX       = (0x1 << 5),       /* ��Ϻ��� */
    LINE_TYPE_LCL       = (0x1 << 6),       /* ���ط��� */
    
    LINE_TYPE_BUT,
}LINE_TYPE_EN;

/*ģʽѡ��:VOIP/PSTNģʽ��,ISDN������Ч������*/
typedef enum LINE_MODE_E_
{
    LINE_MODE_VOIP      = LINE_TYPE_SIP,
    LINE_MODE_PSTN      = (LINE_TYPE_PSTN | LINE_TYPE_SIP),
    LINE_MODE_ISDN      = (LINE_TYPE_ISDN | LINE_TYPE_SIP),
    LINE_MODE_AUTO      = (LINE_TYPE_PSTN | LINE_TYPE_ISDN | LINE_TYPE_SIP | LINE_TYPE_CS),
    LINE_MODE_CS        = (LINE_TYPE_CS),
    LINE_MODE_BUTT
}LINE_MODE_E;

/*�˿�����ö�ٶ���*/
typedef enum tagPORT_TYPE_E_
{
    PORT_TYPE_NULL_ = 0,
    PORT_TYPE_FXS,                      /*FXS��*/
    PORT_TYPE_HS,                       /*DECT�ӻ�,HandleSet(HS)*/
    PORT_TYPE_S0,                       /*ISDN����,����ISDN*/

    PORT_TYPE_BUTT,
}PORT_TYPE_E;
/***********************h00190880*******2012/3/28**********/
typedef enum tagVOICE_STATUS_TYPE_EN
{
    VOICE_STATUS_PORT,                          /*��ȡ����ģ��Ķ˿��Ƿ�æ*/
    VOICE_STATUS_CALL,                          /*��ȡ����ģ�����·�Ƿ�æ*/
    VOICE_STATUS_URGCALL,                       /*��ȡ����ģ�����·�Ƿ��н�������*/

    VOICE_STATUS_BUT,
}VOICE_STATUS_TYPE_EN;
/***********************h00190880*******2012/3/28**********/

/*FXS���¹��豸����ö�ٶ���*/
typedef enum tagFXS_TYPE_E_
{
    FXS_TYPE_NULL = 0,
    FXS_TYPE_POTS,                      /*�绰��. Phone*/
    FXS_TYPE_AUTO_RSP,                  /*�Զ�Ӧ���. Answering machine*/
    FXS_TYPE_FAX,                       /*�����. Telefax*/
    FXS_TYPE_ALL,                       /*�ۺ��豸. Combination device*/

    FXS_TYPE_BUTT,
}FXS_TYPE_E;

/*FXO��״̬ö�ٶ���*/
typedef enum tagFXO_STATUS_E_
{
    FXO_STATUS_NULL = 0,
    FXO_STATUS_ISDN,                      /*ISDN up*/
    FXO_STATUS_PSTN,                      /*PSTN up*/

    FXO_STATUS_BUTT,
}FXO_STATUS_E;

/*��Ϣ���¼�*/
typedef struct tagATP_VOICE_MSG_ST_
{
    VOS_UINT32  ulEventType;                    /*�¼�����,����ͬ��Ϣ�µ��¼�����*/
    VOS_UINT32  ulPara;                         /*�¼�����*/
    VOS_CHAR    szBoby[ATP_VOICE_EVENT_LEN];    /*�¼�����*/
}ATP_VOICE_MSG_S;

/*������Ϣ�ṹ��*/
typedef struct tagATP_VOICE_MSG_BUF_ST_
{
    VOS_INT32  lMsgType;                        /*��Ϣ����. ��VOICE_MSG_TYPE_E*/
    VOS_CHAR   szMsgBody[ATP_VOICE_MSG_LEN];  /*��Ϣ����*/
}ATP_VOICE_MSG_BUF_S;

typedef struct tagVOICEPUB_RTP_ST
{   
    VOS_UINT32   ulLocalPortMin;                  /*RTP��ʼ�˿� */
    VOS_UINT32   ulLocalPortMax;                  /*RTP��ֹ�˿� */
    VOS_UINT32   ulDSCPMark;                      /*���ڵ�*/
    VOS_UINT32   ulTelEventPayloadType;           /*RTP telephone event��ʹ�õĸ�������*/
    VOS_UINT32   ulJitBufMaxSize;
    VOS_UINT32   ulJitBufMinSize;
    VOS_UINT32   ulJitBufActualSize;
    VOS_UINT32   ulJitBufType;
}VOICEPUB_RTP_ST;


//�˺ŵĺ������Խṹ
typedef struct tagVOICE_LINE_CALLFEATURE_INFO_ST
{
	VOS_BOOL				bCallerIDEnable;
	VOS_BOOL				bCallerIDNameEnable;
	VOS_BOOL				bCallWaitingEnable;
	VOS_BOOL  				bMWIEnable;
	VOS_CHAR 		        acCWStatus [VCE_STR_LEN_32];	
	VOS_UINT32              ulLineMessagesWaiting;
}VOICE_LINE_CALLFEATURE_INFO_ST;

/*Codeclist����Ĳ���*/
typedef struct tagVOICEPUB_CODECLIST_OBJ
{
    VOS_CHAR      acCodec[VCE_STR_LEN_32];                      /*���뷽ʽ����*/
    VOS_BOOL      bEnable;                                      /**/
    VOS_UINT32    ulPriority;                                   /*�����������͵����ȼ�*/
    VOS_CHAR      acPacketizationPeriod[4];                     /*���ʱ��10/20/30ms*/
    VOS_BOOL      bSilenceSuppression;                          /*�Ƿ�������������*/    
}VOICEPUB_CODECLIST_ST;

/*Codec����*/
typedef struct
{
    VOS_UINT32  ulType;                     /*��������ͣ���MEDIA_PROTOCAL_E*/
    VOS_BOOL    bCNGEnable;                 /*�Ƿ�����������������*/
    VOS_BOOL    bSilence;                   /*�Ƿ�������������*/
    VOS_UINT32  ulPacketPeriod;             /*���ʱ��10/20/30ms,��PACK_TIME_E*/
    VOS_BOOL    bEnable;                    /*�Ƿ����ô˱����*/
    VOS_CHAR    acTransmitCodec[VCE_STR_LEN_32];              /*��������������ʹ�õı����*/
    VOS_CHAR    acReceiveCodec[VCE_STR_LEN_32];               /*�������������ʹ�õı����*/
    VOS_UINT32  ulTransmitBitRate;                            /*��������������ı�����*/
    VOS_UINT32  ulReceiveBitRate;                             /*��������������ı�����*/    
    VOS_UINT32  ulTransmitPacketizationPeriod;                /*�������ʱ��*/
    VOS_BOOL    bTransmitSilenceSuppression;                  /*������������*/
    VOS_BOOL    bReceiveSilenceSuppression;                   /*������������*/
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
    //VOS_UINT8   ucLineMode;                   /*����ģʽ:IP/ISDN/PSTN/AUTO. ��LINE_MODE_E*/
    VOS_UINT8   Reserve[3];                     /*�ֽڶ������չ�ֶ�*/
    VOS_UINT32  ulRegExpire;                    /*ע��Expireʱ��,��λs*/
    //VOS_CHAR    acIsdnAreaCode[SIP_MAX_NUM_LEN];/*ISDN��������*/
    VOS_CHAR    szIfName[VCE_STR_LEN_32];       /*���нӿ���*/
    VOS_CHAR    acPSTNNumber[VCE_STR_LEN_32];   /*PSTN����*/
    VOS_CHAR    acUMTSNumber[VCE_STR_LEN_32 + 4];/*UMTS����*/
    VOS_BOOL    bUmts;                          /*�Ƿ�����UMTS����*/
    VOS_UINT32  ulMaxSession;                   /*����Ŀǰ֧�ֵ����Ự��*/
    VOS_UINT32  ulInterDigitTimStd;
	VOS_CHAR    acTerminDigit[VCE_STR_LEN_16];
    VOS_BOOL    bVoiceDiag;                      /* �Ƿ���װ������ */  
    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32  ulQICalcInterval;
    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32  ulDslDefaultAccount;             /* ��ʱdsl�˺��Ƿ���Ĭ�ϵ� */
}GLOBAL_CFG_FROM_WEB_S, *GLOBAL_CFG_FROM_WEB_PS;

typedef struct tagLINE_CFG_FROM_WEB_ST_
{
    VOS_BOOL    bEnable;                        /*�ʺ�ʹ��*/
    VOS_UINT32  ulLineSn;                       /*�����ʺ�����,Id*/    
    VOS_UINT32  ulWebSeq;                       /*��Ӧҳ�������*/
    VOS_CHAR    szId[SIP_MAX_NUM_LEN];          /*SIP�ʺ�*/
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*�û���*/
    VOS_CHAR    szPassword[SIP_MAX_PSW_LEN];    /*����*/
    VOS_CHAR    szCode[SIP_MAX_NUM_LEN];        /*����*/
    VOS_BOOL    bEnableCode;                    /*�����Ƿ�ʹ��*/
    VOS_BOOL    bBusyOnBusy;                    /*Busy On Busy�Ƿ���*/
    VOS_UINT8   ucProtocolType;                 /*Э������:1.UDP 2.TCP 3.TLS 4.SCTP*/
    VOS_UINT8   ucProfileType;                  /*1��ʾDT�����ṩ��;2��ʾ���������ṩ��*/
    VOS_UINT16  uwLocalPort;                    /*local port*/
    VOS_CHAR    szRegServer[SIP_MAX_URL_LEN];   /*ע�������*/
    VOS_CHAR    szOutProxy[SIP_MAX_URL_LEN];    /*outbound proxy server*/
    VOS_CHAR    szProxyServer[SIP_MAX_URL_LEN];	/*���������*/
    VOS_CHAR    szStunServer[SIP_MAX_URL_LEN];  /*stun������*/
    VOS_UINT16  uwRegPort;                      /*ע��������˿ں�*/
    VOS_UINT16  uwOutProxyPort;                 /*outbound proxy port*/
    VOS_UINT16  uwProxyPort;                    /*����������˿ں�*/
    VOS_UINT16  uwStunPort;                     /*stun�������˿ں�*/
    VOS_BOOL    bMultiLine;
    VOS_BOOL    bCLIR;
    VOS_BOOL    bSDPCodecRes;
	#ifdef SUPPORT_ATP_VCE_WT104_CUSTOM             
    VOS_BOOL    bSpecLine;                      /*����Ƿ���Profile 3�µ������˺ţ������˺�����ע��*/
    #endif
    VOS_UINT32  ulDTMFMethodG711;               /* DTMF���ݵĴ��䷽��
                                                  0:Inband   
                                                  1:RFC 4733  
                                                  2:SIP Info*/
    VOICE_LINE_CALLFEATURE_INFO_ST  stCalFeature;
}LINE_CFG_FROM_WEB_S, *LINE_CFG_FROM_WEB_PS;

typedef struct tagISDN_LINE_CFG_FROM_WEB_ST_
{
    VOS_CHAR    szNumber[SIP_MAX_NUM_LEN];      /*ISDN����*/
    VOS_BOOL    bBusyOnBusy;                    /*Busy On Busy�Ƿ���*/
    VOS_BOOL    bMultiLine;
    VOS_BOOL    bCLIR;
}ISDN_LINE_CFG_FROM_WEB_S, *ISDN_LINE_CFG_FROM_WEB_PS;

typedef struct
{
    VOS_UINT8       u8_IPUI[VCE_STR_LEN_64];    /*!< Handset identity >suota����ʱ��*/
}DECT_HS_IPUI_ST;

typedef struct tagDECT_CFG_FROM_WEB_ST_
{
    VOS_BOOL    bEnable;                        /*dectʹ��*/
    VOS_CHAR    szPin[VCE_PIN_LEN_MAX];         /*dect�ӻ�PIN��*/
	/*start of add by xkf38924,2011-05-31*/
	VOS_BOOL   bEnableEcologicalModel;          /*ʹ��dect��̬ģʽʹ��*/
	VOS_BOOL   bEnableHalfTransmitPower;        /*ʹ��dect���书�ʵ�һ��*/
	/*the end  by xkf38924,2011-05-31*/
	VOS_CHAR   szSubsData[DECT_CMO_SUBSDATA_MAX_LEN]; /*�����ӻ�ע����Ϣ*/
    VOS_UINT32 ulNewMobilId;                    /*��¼�ո�ע���ϵ��ӻ�id����cgi���ظ�ҳ��*/
    DECT_HS_IPUI_ST stHsIpui[VCE_DECT_HS_MAX];
}DECT_CFG_FROM_WEB_S, *DECT_CFG_FROM_WEB_PS;

/***********h00190880*******************/
#define VDB_DB_STATUS           "/var/vdb.status"
#define VDB_VDB_WRITE_STATUS    "/var/vdb.write"

// usPortMin :  ������Ķ˿�
// usPortMax:  ������Ķ˿ڷ�Χ���ֵ����������˿ڷ�Χ������0
// bTcp        : TCP(1, VOS_TRUE)����UDP(0, VOS_FALSE)�˿�
#if 0
typedef struct tagATP_WELKNOWN_PORT_ST
{
    VOS_UINT16           ucTcp;      // 1: TCP; 0: UDP
    VOS_UINT16           ucRange;    // �˿ڷ�Χ
    VOS_UINT16          usPort;     // �˿ں�
} ATP_WELKNOWN_PORT_ST;
#endif

/*Dect�ӻ�����*/
typedef enum tagDECT_TYPE_E_
{
    DECT_TYPE_GAP	= 0,
    DECT_TYPE_CATIQ1,
    DECT_TYPE_CATIQ2,
    DECT_TYPE_DSPG,
	DECT_TYPE_UNKNOW,
}DECT_TYPE_E;

/* ��������*/
typedef enum tagDECT_CFG_TYPE_E_
{
    DECT_CFG_NORMAL   = 0,       /* ������� */
    DECT_CFG_EQUIP,              /* װ����������DECT���� */
    DECT_CFG_PP,                 /* �ӻ����������ע�� */
    DECT_CFG_OTHER,
}DECT_CFG_TYPE_E;

typedef struct tagDECT_HS_TYPE_ST
{
    VOS_UINT32      ulHsId;     /*ȡֵ��Χ1~5*/
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

/*ǰת����*/
typedef enum tagVOICE_CF_TYPE_EN
{
    VOICE_CF_CFU          = 0,
    VOICE_CF_CFB          = 1,
    VOICE_CF_CFNR         = 2,
}VOICE_CF_TYPE_EN;

/*����ǰת��Ϣ�ṹ*/
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

/*Dect���÷������ĵ�����*/
typedef enum tagDECT_COMMONCFG_SETTYPE_E_
{
    DECT_SETTYPE_ADD   = 0,       /* ���� */
    DECT_SETTYPE_DEL,             /* ɾ�� */
    DECT_SETTYPE_SET,             /* �޸� */
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
    VOS_BOOL    bEnable;                /*�˿��Ƿ�ʹ��.Dectûע����ΪDisable;ע����ΪEnable*/
    VOS_BOOL    bEnableCW;              /*�Ƿ�ʹ�ܺ��еȴ�ҵ��*/
	/*start of add by xkf38924 ,the date :2011-05-30*/
	VOS_BOOL    bEnableVoiceBoxInfo;    /*�Ƿ�ʹ��voiceboxinfo*/
	/*end of add by xkf38924,the date:2011-05-30*/
    VOS_UINT8   ucPortID;               /*�˿�ID*/
    VOS_UINT8   ucLineID;               /*�ڲ��������кţ���ʼ�����VCE_INCALL_BASE*/
    VOS_UINT8   ucPortType;             /*�˿�����:FXS��,dect�ӻ�,ISDN���л���,��PORT_TYPE_E*/
    VOS_UINT8   ucFXSPortType;          /*FXS������:�����,�绰��,�Զ�Ӧ���,�ۺ��豸,��FXS_TYPE_E*/
    VOS_UINT8   ucDectType;             /*Dect�ӻ�����:0��ʾGAP��1��ʾCATIQ1��2��ʾCATIQ2����DECT_TYPE_E */
    #if 0
    VOS_UINT8   ucPriNumDisPlayType;    /*��ѡ�����������ʾ����,0--��ʾ;��0--����.��SIP_CLI_STATUS_E*/
    VOS_UINT8   ucSndNumDisPlayType;    /*��ѡ�����������ʾ����,0--��ʾ;��0--����.��SIP_CLI_STATUS_E*/
    #endif
    VOS_UINT8   ucInLineNum;            /*��������ʺ���Ŀ*/
    VOS_UINT8   ucOutLineNum;           /*���������ʺ���Ŀ*/
    VOS_UINT8   ucReserve;
    VOS_BOOL    bInRelateAll;           /*�����������*/
    VOS_BOOL    bOutRelateAuto;         /*�����Զ�ѡ�ʺ�*/
    VOS_UINT8   aucIncomingLine[VCE_LINE_MAX];  /*��������ʺ�. ��Ź����ʺ�����*/
    VOS_UINT8   aucOutgoingLine[VCE_LINE_MAX];  /*���������ʺ�. ��Ź����ʺ�����*/
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*�˿���*/
    VOS_BOOL    bCFUEnable;
    VOS_BOOL    bCFBEnable;
    VOS_BOOL    bCFNREnable;
    VOS_UINT32  ulCFBRingTime;
    VOS_UINT32  ulCFNRingTime;
    VOS_INT     iCFUDestPortId;                                 /*������ǰתĿ�Ķ˿ںţ���Χ0~6����ҵ��ʱΪ-1*/
    VOS_INT     iCFBDestPortId;                                 /*��æĿ�Ķ˿ںţ���Χ0~6����ҵ��ʱΪ-1*/
    VOS_INT     iCFNRDestPortId;                                /*��Ӧ��Ŀ�Ķ˿ںţ���Χ0~6����ҵ��ʱΪ-1*/    
    VOS_UINT32  ulRingTimout;                                   /*�˿��������ʱ��*/
}PHY_CFG_FROM_WEB_S, *PHY_CFG_FROM_WEB_PS;

typedef struct tagPHONE_BOOK_CFG_FROM_WEB_ST_
{
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];       /*����*/
    VOS_CHAR    szPrivateNum[SIP_MAX_NUM_LEN];  /*˽�˵绰*/
    VOS_CHAR    szFamilyNum[SIP_MAX_NUM_LEN];   /*��ͥ�绰*/
    VOS_CHAR    szMobileNum[SIP_MAX_NUM_LEN];   /*�ƶ��绰*/
}PHONE_BOOK_CFG_FROM_WEB_S, *PHONE_BOOK_CFG_FROM_WEB_PS;

typedef struct tagVOICE_SVC_CFG_FROM_WEB_ST_
{
    GLOBAL_CFG_FROM_WEB_S       stGlobal;                        /*����ҵ��ȫ������*/
    LINE_CFG_FROM_WEB_S         stLine[VCE_VOIP_MAX];            /*SIP�ʺ�����*/
    //ISDN_LINE_CFG_FROM_WEB_S    stIsdnLine[VCE_ISDN_MAX];      /*ISDN�ʺ�����*/
    DECT_CFG_FROM_WEB_S         stDect;                          /*DECTȫ������*/
    PHY_CFG_FROM_WEB_S          stPhy[VCE_PORT_MAX];             /*FXS�ں�dect�ӻ�����*/
    VOICEPUB_CODECLIST_ST          stCodecList[VOICE_CODECLIST_MAX];        /* ���뷽ʽ�б� */
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

/* start of modify by l00126521 2010-5-18 for QGT 8445:�ӻ�����ҳ�水ť�����޸� */
/*�ӻ�������Ϣ�ṹ��*/
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

/*�Ự��Ϣ*/
typedef struct tagSESSION_STATE_ST_
{   
    VOS_UINT32      ulStartYear;
    VOS_UINT32      ulStartMonth;
    VOS_UINT32      ulStartDay;
    VOS_UINT32      ulStartHour;
    VOS_UINT32      ulStartMinute;
    VOS_UINT32      ulStartSecond;
    VOS_UINT32      ulStartTime;                            /*��ʼʱ��*/
    VOS_UINT32      ulEndTime;                              /*����ʱ��,���Ϊ0,��ʾ��δ����*/

    VOS_INT32       ulDuration;                             /*ͨ��ʱ��*/
    VOS_CHAR        acStreamType[VCE_STR_LEN_16];
    VOS_CHAR        acFarEndIPAddress[VCE_STR_LEN_64];      /*�Զ�ip��ַ*/
    VOS_UINT32      ulFarEndUDPPort;        /*�Զ�udp�˿�*/
    VOS_UINT32      ulLocalUDPPort;        /*����udp�˿�*/
    VOS_UINT32      ulSessionID;
    VOS_UINT32      ulChangeFlag;           /**/
    TAPI_CODEC_S    stCodec;
    VOICE_CALLOG_SESSION_RTP_ST  stRtp;
    VOICE_QUALITY_ST stVoiceQI;
    struct sockaddr_storage stLclAddr;
    struct sockaddr_storage stRmtAddr;                      /*����RTP�˿�*/
}SESSION_STATE_S, *SESSION_STATE_PS;


/*RTCP��Ϣͳ��*/
typedef struct tagRTCP_INFO_ST_
{
    VOS_UINT32  ulPacketsSent;                          /*�ѷ��ͱ��ĸ���*/
    VOS_UINT32  ulPacketsReceived;                      /*�ѽ��ձ��ĸ���*/
    VOS_UINT32  ulBytesSent;                            /*�ѷ��ͱ����ֽ���*/
    VOS_UINT32  ulBytesReceived;                        /*�ѽ��ձ����ֽ���*/
    VOS_UINT32  ulPacketsLost;                          /*�Ѷ�ʧ���ĸ���*/

    /*Total number of times the receive jitter buffer has overrun for this line*/
    VOS_UINT32  ulOverruns;                             /*����������Χ��ı��ĸ���*/

    /*Total number of times the receive jitter buffer has underrun for this line*/
    VOS_UINT32  ulUnderruns;                            /*������Χ�ڵı��ĸ���*/

    /*Current receive packet loss rate in percent*/
    VOS_UINT32  ulReceivePacketLossRate;                /*���˶�����*/

    /*Current far end receive packet lost rate in percent*/
    VOS_UINT32  ulFarEndPacketLossRate;                 /*Զ�˶�����*/

    /*Current receive interarrival jitter in microseconds*/
    VOS_UINT32  ulReceiveInterarrivalJitter;            /*���˶���ʱ����*/

    /*Current Interarrival jitter in microseconds as reported from the far-end device via RTCP*/
    VOS_UINT32  ulFarEndInterarrivalJitter;             /*Զ�˶���ʱ����*/

    /*Current round trip delay in microseconds*/
    VOS_UINT32  ulRoundTripDelay;                       /*�����ӳ�[RFC3550 6.4]*/

    /*Average receive interarrival jitter in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageReceiveInterarrivalJitter;     /*����ƽ������ʱ����*/

    /*Average far-end interarrival jitter in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageFarEndInterarrivalJitter;      /*Զ��ƽ������ʱ����*/

    /*Average round trip delay in microseconds since the beginning of the current call*/
    VOS_UINT32  ulAverageRoundTripDelay;                /*ƽ�������ӳ�*/

    /*BEGIN:Added by l00180792 @20120717 for QI*/
    VOS_UINT32       ulBufferLost;                      /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.BufferLost*/
    VOS_UINT32       ulJitter;                          /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.PacketDelayVariation*/
    VOS_UINT32       ulpeakJitter;                      /* VoiceService.{i}.CallControl.CallLog.{i}.Session.{i}.RTP.BufferDelay*/
    VOS_UINT32       ulMOSLQ;                           /* MOS-listening quality (value*10) */
    VOS_UINT32       ulMOSCQ;                           /* MOS-conversational quality (value*10) */
    /*END:Added by l00180792 @20120717 for QI*/
}RTCP_INFO_ST;

/*�ʺ�״̬*/
typedef struct tagLINE_STATE_ST_
{
    VOS_CHAR        szId[SIP_MAX_NUM_LEN];                  /*Line�ʺ�*/
    VOS_CHAR        szCallStatus[VCE_STR_LEN_16];   
    VOS_UINT8       ucRegStatus;                            /*����ע��״̬*/
    /* BEGIN: Modified by l00126521 for AU8D04013, 2010/2/11 */
    VOS_UINT8       ucRegFailReason;                        /*ע��ʧ��ԭ��*/
    /* END:   Modified by l00126521 for AU8D04013, 2010/2/11 */
    
    VOS_UINT32      ulReadyFlag;                            /*��Ϣ�Ƿ�Ready�ı�־λ*/
    VOS_BOOL        bResetStatistics;                       /*�Ƿ����ͳ��*/
    VOS_UINT32      ulIncomingReceived;                     /*����绰����*/
    VOS_UINT32      ulIncomingAnswered;                     /*�����ѽӵ绰����*/
    VOS_UINT32      ulIncomingConnected;                    /*����ɹ��绰����*/
    VOS_UINT32      ulIncomingFailed;                       /*����ʧ�ܵ绰����*/
    VOS_UINT32      ulOutgoingAttempted;                    /*�����绰����*/
    VOS_UINT32      ulOutgoingAnswered;                     /*�����ѽӵ绰����*/
    VOS_UINT32      ulOutgoingConnected;                    /*�����ɹ��绰����*/
    VOS_UINT32      ulOutgoingFailed;                       /*����ʧ�ܵ绰����*/
    VOS_UINT32      ulCallsDropped;                         /*ͨ���������жϸ���*/
    VOS_UINT32      ulTotalCallTime;
    VOS_UINT32      ulServerDownTime;
    /*BEGIN;added by l00192646,2012/06/27,for wt-104 line call waiting status*/
    VOS_CHAR        szCallWaitingStatus[VCE_STR_LEN_32];    /*��·���Ƿ��к���*/
    /*END;added by l00192646,2012/06/27,for wt-104 line call waiting status*/
    VOS_UINT32      ulLineMessagesWaiting;                  /*��·�����Եĸ���*/

    SESSION_STATE_S stSession[SIP_LINE_STATE_SESS_MAX];     /*�ʺ�����ĺ�����Ϣ*/
    
    RTCP_INFO_ST    stRtcp;                                 /*RTCPͳ����Ϣ*/
}LINE_STATE_S, *LINE_STATE_PS;

/*PhyInterface state*/
typedef struct tagPHY_STATE_ST_
{
    VOS_BOOL    bBusy;                                  /*�˿��Ƿ�����ʹ��*/
    VOS_BOOL    bCFType;                                /*ǰת����*/
    VOS_CHAR    szCFNum[SIP_MAX_NUM_LEN];               /*ǰת����*/
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
    VOS_CHAR    szId[SIP_MAX_NUM_LEN];                  /*VoIP�ʺ�*/
}SIP_INFO_S, *SIP_INFO_PS;

typedef struct tagGLOBAL_STATE_ST_
{
    VOS_BOOL    bPortBusy;                              /*�Ƿ��ж˿�����ʹ��*/
    VOS_BOOL    bHasUrgCall;                            /*�Ƿ���ڽ�������*/
    VOS_BOOL    bHasUseableLine;                        /*�Ƿ���ڿ����ʺ�*/
    /*BEGIN:Added by l00180792 @20120828 for DTS2012081703300*/
    VOS_BOOL    bSNTPUpdate;                            /*�Ƿ�ɹ���ȡ��SNTPʱ��*/
    /*BEGIN:Added by l00180792 @20120828 for DTS2012081703300*/
    VOS_UINT32  ulFxoStatus;                            /*FXO��״̬:ISDN UP;PSTN UP;fail*/
    VOS_UINT32  ulDectStatus;                            /*FXO��״̬:ISDN UP;PSTN UP;fail*/
    VOS_UINT32  ulRegExpire;                            /*ע��ˢ��ʱ��*/
}GLOBAL_STATE_S, *GLOBAL_STATE_PS;

typedef struct tagISDN_AUTO_ST_
{
    VOS_CHAR    szNumber[SIP_MAX_NUM_LEN];              /*ISDN����*/
    
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

//��������log�ṹ
typedef struct tagVOICE_CALLOG_ST
{
    VOS_UINT32              ulCrSn;                             /*��·���ж�Ӧ��CrSn*/
    VOS_BOOL                bUsed;                              /*��log����ʹ��*/
    VOS_UINT32              ulCalllogInstId;                    /*��Ӧʵ����*/
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
    GLOBAL_STATE_S  stGlobal;                           /*ȫ��״̬��Ϣ*/
    SIP_INFO_S      stSip[VCE_VOIP_MAX];                /*VoIP�ʺŹؼ�������Ϣ*/
    LINE_STATE_S    stLine[VCE_VOIP_MAX];               /*�ʺ�״̬��Ϣ*/
    PHY_STATE_S     stPort[VCE_PORT_MAX];               /*�˿�״̬��Ϣ*/
    FXS_STATE_S     stFxs[VCE_FXS_MAX];                  /*S�����Ƿ�ӻ���*/
    ISDN_AUTO_S     stAutoIsdn[VCE_ISDN_MAX];           /*�Զ���ȡISDN����*/
    VOICE_CALLOG_ST stCalllog[VOICE_CALL_MAX];
    VOS_BOOL        bAnt0IsOn;                           /* ��¼����0��״̬*/
    VOS_BOOL        bAnt1IsOn;                           /* ��¼����1��״̬*/
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
/* ��Ϣ����дȨ�� */
#define VHG_VOICE_MSG_W             01
#define VHG_VOICE_CMD_MSG_TYPE      8849
#define VHG_VOICE_CMS_MSGKEY        'V'
#define VHG_VOICE_PID_PATH          "/var/vhgvpid"

/*��Ϣ������*/
typedef enum tagVHG_VOICE_MSG_TYPE_E
{
    VHG_MSG_TYPE_CMS,                       /*��Ʒ����ҵ��������Ϣ*/
    VHG_MSG_TYPE_TIMER,                     /*timer��Ϣ*/
    VHG_MSG_TYPE_BUTT
}VHG_VOICE_MSG_TYPE_E;

/* �����л����ù����͵�ָʾ */
typedef enum tagVHG_VOICE_CMS_MSG_TYPE_E
{
	VHG_VOICE_CMS_MSG_UPDATA_CONFIG,
    VHG_VOICE_CMS_MSG_UNKNOWN
}VHG_VOICE_CMS_MSG_TYPE_E;

/* ���ò������� (���ù������) */
typedef enum tagVHG_VOICE_CMS_MSG_CMO_E
{
	VHG_VOICE_CMS_MSG_VOICEPROFILE,
    VHG_VOICE_CMS_MSG_RTP,
    VHG_VOICE_CMS_MSG_VOICEPROCESSING,
    VHG_VOICE_CMS_MSG_CODEC,
    VHG_VOICE_CMS_MSG_CODECLIST
}VHG_VOICE_CMS_MSG_CMO_E;

/*ȫ�ֲ���*/
typedef struct
{
    /*voice profile����Ĳ���*/
    VOS_UINT32  ulDTMFMethod;               /* DTMF���ݵĴ��䷽��DTMF_TYPE_E
                                             0:Inband
                                             1:RFC 2833
                                             2:SIP Info*/
    VOS_UINT32  ulRegion;	                /*����REGION_CODING_E*/
    VOS_UINT32  ulFaxOption;                /*��������FAX_TYPE_E
                                             0:G.711A FAX Passthrough
                                             1:G.711U FAX Passthrough
                                             2:T.38 FAX*/
    VOS_UINT32  ulSipPriority;              /*SIP RTP���ķ��͵����ȼ�1:��2:��3:��SIP_PRIORITY_E*/
    VOS_UINT32  ulTOSPriority;              /*TOS���ȼ�SIP_PRIORITY_E*/

    /*RTP����Ĳ���*/
    VOS_UINT32  ulLclRTPPortMin;            /*��СRTP�˿�*/
    VOS_UINT32  ulLclRTPPortMax;            /*���RTP�˿�*/
    VOS_BOOL    bJitterBuffer;              /*�Ƿ����ö�������*/
    VOS_UINT32  ulJBLength;                 /*JitterBuffer����*/
    VOS_BOOL    bPackLostCompensate;        /*�Ƿ����ö�������*/
    VOS_UINT32  ulJBType;                   /*JitterBuffer����JB_TYPE_E*/
    VOS_UINT32  ulTelEventPayloadType;      /*RTP telephone event��ʹ�õĸ�������*/
    VOS_UINT32  ulJitBufMaxSize;
    VOS_UINT32  ulJitBufMinSize;
    VOS_UINT32  ulJitBufActualSize;
    VOS_UINT32  ulJitBufType;

    /*VoiceProcessing����Ĳ���*/
    VOS_INT32	lTransmitGain;              /*��Ͳ����*/
    VOS_INT32	lReceiveGain;               /*��Ͳ����*/

}TAPI_GLOBAL_S;



/*�ʺŲ���*/
typedef struct
{
    VOS_BOOL        bECEnable;              /*�Ƿ����ûز�����*/

    VOS_UINT32      ulWebSeq;               /*���ڱ���˺���ҳ���˳��*/
    
    VOS_UINT32      ulDTMFMethod;           /* DTMF���ݵĴ��䷽��
                                                  0:Inband   
                                                  1:RFC 4733  
                                                  2:SIP Info*/ 
    TAPI_CODEC_S    stCodec[VCE_CODEC_MAX]; /*������б�*/
}TAPI_LINE_S;

/*��������*/
typedef struct
{
    TAPI_GLOBAL_S   stGlobal;               /*ȫ�ֲ���*/
    TAPI_LINE_S     stLine[VCE_VOIP_MAX];   /*VoIP�ʺŲ���*/
}TAPI_CMS_S, *TAPI_CMS_PS;

/* CMS message */
typedef struct VHG_VOICE_CMS_MSG
{
    VHG_VOICE_CMS_MSG_TYPE_E  lEventType;   /*���ò�������,��VHG_VOICE_CMS_MSG_TYPE_E*/
    VOS_UINT32                ulCMDHead;    /*���ò�������*/
    VHG_VOICE_CMS_MSG_CMO_E   lCMDBody;     /*���ò�������*/
}VHG_VCMS_MSG_ST;

typedef struct VHG_VCMS_MSG
{
    VHG_VOICE_MSG_TYPE_E eMsgType;          /*��Ϣ���ͣ��������������Ƿ���������Ϣ,��VHG_VOICE_MSG_TYPE_E*/
    VHG_VCMS_MSG_ST      CMSMsg;            /*�����������Ϣʱ������������Ϣ������*/
} VHG_VOICE_MSG_ST;

typedef struct VHG_VCMS_MSG_BUFF
{
    VOS_INT32         mtype;                /* ��Ϣ���� ,�����ж��Ƿ��ǲ�Ʒ����ҵ����Ϣ*/
    VHG_VOICE_MSG_ST  stVoiceMsg;           /* ��Ϣ���� */
}VHG_VOICE_MSG_BUFF_ST;

#define	DECT_NAME_MAX_LEN			64
typedef struct tagDECT_NAME_UPDATE
{
	VOS_UINT32	ulDectIndex;
	VOS_CHAR	acDectName[DECT_NAME_MAX_LEN];
}DECT_NAME_UPDATE;
extern TAPI_CMS_PS g_pstVoiceTapiCfg;
/*************************  END: TAPI_CMS  *************************/



/*Start of ά�������� 2011-9-26 11:25 for RSS by x00125299*/
typedef struct tagATP_VOICE_RSSRESPONSE_ST
{
    VOS_UINT32  ulIdx;
	VOS_CHAR    acSessinList[ATP_ACTUAL_LEN_256];
} ATP_VOICE_RSSRESPONSE_ST;
/*End of ά�������� 2011-9-26 11:25 for RSS by x00125299*/

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

/*add by w00191600  2011-11-5 for ���IMAP�ʼ��Ѷ�*/
typedef struct tagATP_EmailSign_MSG_ST
{
    long long 	llAccountIdx;
    long long	llMailUid;//imap�ʼ���UID
}ATP_EmailSign_MSG_ST;
/*end add by w00191600  2011-11-5 for ���IMAP�ʼ��Ѷ�*/

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

