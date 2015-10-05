/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : CsPub.h
  �� �� ��   : ����
  ��    ��   : cqLuo
  ��������   : 2008��5��8��
  ����޸�   :
  ��������   : CS����ģ�鹫��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��5��8��
    ��    ��   : cqLuo
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CS_PUB_H__
#define __CS_PUB_H__

#ifdef __cplusplus
    #if __cplusplus
extern "C" {
    #endif
#endif


#define USR_VOICE_CS            "/bin"      /*ftokĿ¼*/
#define VOICE_CS_PID_PATH       USR_VOICE_CS
#define VOICE_CS_MSGKEY_FLAG    'v'

/*������ִ�н���ļ����·��*/
#define CS_CMDRESULT_FILE       "/var/voice/cmdResult"
#define MAX_CMDRESULT_FILE_LEN  64
#define CMDRESULT_ERROR_STR     "ERROR"

#define CS_MAX_CALLID_LEN       32          /*��󲦺ź��볤��*/
#define CS_MAX_CMDSTRING_LEN    128         /*�������ַ�������*/
#define CS_CLIP_FLAG            "I"         /*��������ʾ��ʶ*/
#define CS_CLIR_FLAG            "i"         /*�ر�������ʾ��ʶ*/
#define CS_VOICE_FLAG           ";"         /*�������б�ʶ*/
#define CS_OK                   1           /**/
#define CS_NOK                  0           /**/

#define AT_RCV_MSG_LENGTH       252         /*������Ϣ����������*/
/*begin: l110333 for AU4D01099(ata fail) on 2009.01.08*/
#define SIG_ATCMD_ATA           98          /*ATA�ź�*/
#define ATS_PID                 "/var/at/atsrv.pid"
#define MAX_ATS_PID_FILE_LEN    16
/*end: l110333 for AU4D01099(ata fail) on 2009.01.08*/

/*CS����ģ���·���AT����ģ�����������*/
typedef enum _E_CS_CMD_TYPE
{
	CS_CMD_TYPE_VOICE_OUTGOING = 1,			    /*�����������*/
	CS_CMD_TYPE_FAX_OUTGOING,				    /*�����������*/
	CS_CMD_TYPE_ACCEPT,						    /*��������*/
	CS_CMD_TYPE_END,						    /*�Ҷ�����*/
	#ifdef CS_SUPPLY_SERVICE
	CS_CMD_TYPE_CW_ENABLE,					    /*������еȴ�ҵ��*/
	CS_CMD_TYPE_CW_DISABLE,					    /*ȥ������еȴ�ҵ��*/
	CS_CMD_TYPE_CFU_ENABLE,					    /*��������������ǰת*/
	CS_CMD_TYPE_CFB_ENABLE,					    /*������æ����ǰת*/
	CS_CMD_TYPE_CFNR_ENABLE,					/*������Ӧ�����ǰת*/
	CS_CMD_TYPE_CT_DISABLE,					    /*ȥ�������ת��ҵ��*/

	CS_CMD_TYPE_END_HOLDCALL,                   /*�ͷű����ֺ���,��Ҷ�CW����*/
	CS_CMD_TYPE_END_ACTIVECALL,                 /*�ͷ����м������,������CH��CW����*/
	CS_CMD_TYPE_END_ONE,                        /*����һ·ָ���ĺ���*/
	CS_CMD_TYPE_SWITCH,						    /*�л���������*/
	CS_CMD_TYPE_HOLD,						    /*����ָ��������,�������м������*/
	CS_CMD_TYPE_3WC_START,                      /*��ʼ����ͨ��*/
	#endif
	CS_CMD_TYPE_DDSETEX,					    /*DDSETEX����*/
	CS_CMD_TYPE_CLIP,					        /*����CLIP����*/
	CS_CMD_TYPE_INVALID = 0xFFFFFFFF			/*��Ч����*/
}E_CS_CMD_TYPE;

/*CS��Ϣ����*/
typedef enum CS_MSG_TYPE_E_
{
    /*ATCMD��Ϣ��*/
    CS_MSG_AT_BEGIN = 0x00000000L,          /*ATCMD��Ϣ��ʼ*/

    /*����ģ���·���ҵ�������*/
    CS_MSG_AT_RESULT_REPORT,                /*ATִ�н���ϱ�*/
    CS_MSG_AT_RING_REPORT,                  /*��������ָʾ*/
    CS_MSG_AT_CLIP_REPORT,                  /*������ʾָʾ*/
    CS_MSG_AT_ORIG_REPORT,                  /*���з���ָʾ*/
    CS_MSG_AT_CONF_REPORT,                  /*������ָͨʾ*/
    CS_MSG_AT_CONN_REPORT,                  /*ͨ������ָʾ*/
    CS_MSG_AT_CEND_REPORT,                  /*ͨ������ָʾ*/
    CS_MSG_AT_SYSINFO_REPORT,               /*SysInfoָʾ*/
    CS_MSG_AT_SIM_STATUS_REPORT,            /*SIM��״̬�仯ָʾ*/
    //#ifdef CS_SUPPLY_SERVICE
    /*cs_20080828         chenkeyun       begin*/
    CS_MSG_AT_CCWA_REPORT,                  /*���еȴ�����ָʾ*/
    CS_MSG_AT_CSSI_REPORT,                  /*AT�ϱ�����ҵ��ָʾCSSI����*/
    CS_MSG_AT_CSSU_REPORT,                  /*AT�ϱ�����ҵ��ָʾCSSU����*/
    /*cs_20080828         chenkeyun       end*/
    //#endif

    /*ҵ���·�������ģ�������*/
    CS_MSG_AT_DIAL,                         /*ҵ�������*/
    CS_MSG_AT_ACCEPT,                       /*ҵ�����*/
    CS_MSG_AT_CHUP,                         /*ҵ��ҶϺ���*/
    CS_MSG_AT_CALL_TIME_QUERY,              /*ͨ��ʱ����ѯ*/
    CS_MSG_AT_END = 0x00000100,             /*AT��Ϣ����*/


    /*����Ӧ����Ϣ��*/
    CS_MSG_VOICEAPP_BEGIN = 0x00000101,     /*����Ӧ����Ϣ��ʼ*/

    /*CS��SC����Ϣ*/
    CS_MSG_VOICEAPP_STATUS_UPDATE,          /*CS��·״̬�����¼�*/
    CS_MSG_VOICEAPP_INCOMING,               /*CS���磬����*/
    CS_MSG_VOICEAPP_CANCLE,                 /*CSȡ������*/
    CS_MSG_VOICEAPP_ENDCALL,                /*CS�һ���Զ�˹һ�*/

    /*SC��CS����Ϣ*/
    CS_MSG_VOICEAPP_OUTGOING,               /*���к���*/
    CS_MSG_VOICEAPP_ACCEPT,                 /*�����������*/
    //#ifdef CS_SUPPLY_SERVICE
    CS_MSG_VOICEAPP_OP_SERVICE,             /*����ҵ��ʹ����Ϣ*/
    CS_MSG_VOICEAPP_HOLD,                   /*�������ڽ���ͨ���ĺ���*/
    CS_MSG_VOICEAPP_RECOVER,                /*�ָ������ֵĺ���*/
    CS_MSG_VOICEAPP_CW,                     /*���еȴ�*/
    CS_MSG_VOICEAPP_CT,                     /*����ת��*/
    CS_MSG_VOICEAPP_3WC_START,              /*��ʼ����ͨ��*/
    CS_MSG_VOICEAPP_3WC_END,                /*��������ͨ��*/
    //#endif
    CS_MSG_VOICEAPP_OFFHOOK,                /*����ժ������*/
    CS_MSG_VOICEAPP_ONHOOK,                 /*���˹һ�*/
    CS_MSG_VOICEAPP_END = 0x00000200,       /*����Ӧ����Ϣ����*/


    /*��ʱ����Ϣ��*/
    CS_MSG_TIMER_BEGIN = 0x00000201,        /*��ʱ����Ϣ��ʼ*/
    CS_MSG_TIMER,                           /*��ʱ����Ϣ*/
    CS_MSG_TIMER_END = 0x00000300,          /*��ʱ����Ϣ����*/


    /*������Ϣ��*/
    CS_MSG_PHONE_BEGIN = 0x00000301,        /*������Ϣ��ʼ*/
    CS_MSG_PHONE,                           /*������Ϣ*/
    CS_MSG_PHONE_END = 0x00000400,          /*������Ϣ����*/


    /*USB��Ϣ��*/
    CS_MSG_USB_BEGIN = 0x00000401,          /*USB��Ϣ��ʼ*/
    CS_MSG_USB_STATUS_UPDATE,               /*USB����״̬�仯��Ϣ*/
    CS_MSG_USB_END = 0x00000500,            /*USB��Ϣ����*/


    CS_MSG_INVALID = 0xFFFFFFFF
}CS_MSG_TYPE_E;

/*������·����*/
typedef enum _E_VOICE_UP_TYPE
{
    VOICE_UP_TYPE_CS = 0,                   /*CS����*/
    VOICE_UP_TYPE_ADSL = 1,                 /*ADSL����*/
    VOICE_UP_TYPE_INVALID = 0xFFFFFFFF
}E_VOICE_UP_TYPE;

/*USB����״̬*/
typedef enum CS_USB_STATUS_E_
{
    CS_USB_STATUS_FAULT,                    /*����*/
    CS_USB_STATUS_OK,                       /*��������*/
    CS_USB_STATUS_BUTT,
}CS_USB_STATUS_E;

/*CS��·״̬*/
typedef enum CS_STATUS_E_
{
    CS_STATUS_FAULT,                        /*���ϣ��������*/
    CS_STATUS_UNREG,                        /*δע�ᣬֻ�ܴ��������*/
    CS_STATUS_REG,                          /*��ע�ᣬ���Դ����е绰*/
    CS_STATUS_BUTT,
}CS_STATUS_E;

/*ϵͳ����״̬*/
typedef enum CS_SERVER_STATUS_E_
{
    CS_SERVER_STATUS_NO,                    /*�޷���״̬*/
    CS_SERVER_STATUS_PART,                  /*�����Ʒ���״̬*/
    CS_SERVER_STATUS_OK,                    /*������Ч״̬*/
    CS_SERVER_STATUS_PARTZONE,              /*�����Ƶ��������״̬*/
    CS_SERVER_STATUS_SLEEP,                 /*ʡ�����˯״̬*/
    CS_SERVER_STATUS_INVALID = 0xFFFFFFFF
}CS_SERVER_STATUS_E;

/*ϵͳ������״̬*/
typedef enum CS_ZONE_STATUS_E_
{
    CS_ZONE_STATUS_NO,                      /*�޷���*/
    CS_ZONE_STATUS_OK_CS,                   /*CS����*/
    CS_ZONE_STATUS_OK_PS,                   /*PS����*/
    CS_ZONE_STATUS_OK_CS_PS,                /*CS��PS�����OK*/
    CS_ZONE_STATUS_NOK_CS_PS,               /*CS��PS�����δע�ᣬ����������״̬*/
    CS_ZONE_STATUS_FAIL = 250,              /*δ��ȡ����ֵ��������*/
    CS_ZONE_STATUS_NO_SUPPORT = 255,        /*CDMA��֧��*/
    CS_ZONE_STATUS_INVALID = 0xFFFFFFFF
}CS_ZONE_STATUS_E;

/*CS����״̬*/
typedef enum CS_ROAM_STATUS_E_
{
    CS_ROAM_STATUS_YES,                     /*����״̬*/
    CS_ROAM_STATUS_NO,                      /*������״̬*/
    CS_ROAM_STATUS_INVALID = 0xFFFFFFFF
}CS_ROAM_STATUS_E;

/*CSϵͳģʽ*/
typedef enum CS_SYS_MODE_E_
{
    CS_SYS_MODE_NOSERVER,                   /*�޷���*/
    CS_SYS_MODE_AMPS,                       /*AMPSģʽ*/
    CS_SYS_MODE_CDMA,                       /*CDMAģʽ*/
    CS_SYS_MODE_GSM_GPRS,                   /*GSM\GPRSģʽ*/
    CS_SYS_MODE_HDR,                        /*HDRģʽ*/
    CS_SYS_MODE_WCDMA,                      /*WCDMAģʽ*/
    CS_SYS_MODE_GPS,                        /*GPSģʽ*/
    CS_SYS_MODE_INVALID = 0xFFFFFFFF
}CS_SYS_MODE_E;

/*SIM��״̬*/
typedef enum CS_SIM_STATUS_E_
{
    CS_SIM_STATUS_UNUSABLE,                 /*������*/
    CS_SIM_STATUS_USABLE,                   /*����*/
    CS_SIM_STATUS_NO_CS,                    /*CS������*/
    CS_SIM_STATUS_NO_PS,                    /*PS������*/
    CS_SIM_STATUS_NO_CS_PS,                 /*CS��PS��������*/
    CS_SIM_STATUS_ROM = 240,                /*ROMSIM�汾*/
    CS_SIM_STATUS_FAIL = 250,               /*û�л�ȡ����ֵ��������*/
    CS_SIM_STATUS_NOEXIST = 255,            /*������*/
    CS_SIM_STATUS_INVALID = 0xFFFFFFFF
}CS_SIM_STATUS_E;

/*SIM������״̬*/
typedef enum CS_SIMLOCK_STATUS_E_
{
    CS_SIMLOCK_STATUS_YES,                  /*����״̬*/
    CS_SIMLOCK_STATUS_NO,                   /*������״̬*/
    CS_SIMLOCK_STATUS_INVALID = 0xFFFFFFFF
}CS_SIMLOCK_STATUS_E;

/*CS��ϵͳģʽ*/
typedef enum CS_SUBSYS_MODE_E_
{
    CS_SUBSYS_MODE_NOSERVER,                /*�޷���*/
    CS_SUBSYS_MODE_GSM,                     /*GSMģʽ*/
    CS_SUBSYS_MODE_GPRS,                    /*GPRSģʽ*/
    CS_SUBSYS_MODE_EDGE,                    /*EDGEģʽ*/
    CS_SUBSYS_MODE_WCDMA,                   /*WCDMAģʽ*/
    CS_SUBSYS_MODE_HSDPA,                   /*HSDPAģʽ*/
    CS_SUBSYS_MODE_HSUPA,                   /*HSUPAģʽ*/
    CS_SUBSYS_MODE_CS,                      /*HSDPA&HSUPAģʽ*/
    CS_SUBSYS_MODE_INVALID = 0xFFFFFFFF
}CS_SUBSYS_MODE_E;

//#ifdef CS_SUPPLY_SERVICE
/*CS����ҵ��ָʾCSSI����*/
typedef enum CS_CSSI_TYPE_E_
{
    CS_CSSI_TYPE_CFU_ENABLE = 0,        /*����������ת�Ʊ�����*/
    CS_CSSI_TYPE_CT_ENABLE,             /*ĳһ������ת�Ʊ�����*/
    CS_CSSI_TYPE_CT_CALL,               /*���б�ת��*/
    CS_CSSI_TYPE_CW_CALL,               /*���д��ڵȴ�״̬*/
    CS_CSSI_TYPE_CUG_CALL,              /*����һ��CUG����*/
    CS_CSSI_TYPE_OUT_FORBID,            /*���б���ֹ*/
    CS_CSSI_TYPE_IN_FORBID,             /*���类��ֹ*/
    CS_CSSI_TYPE_FORBID_CLIR_FAIL,      /*��ֹCLIR���ܾ�*/
    CS_CSSI_TYPE_CF_SUC,                /*���б�ǰת (����MT����)*/

    CS_CSSI_TYPE_BUTT
}CS_CSSI_TYPE_E;

/*CS����ҵ��ָʾCSSU����*/
typedef enum CS_CSSU_TYPE_E_
{
    CS_CSSU_TYPE_CT_CALL = 0,           /*��ת�Ƶĺ��У�����MT���У�*/
    CS_CSSU_TYPE_CUG_CALL,              /*CUG ���У�����MT���У�*/
    CS_CSSU_TYPE_HOLD,                  /*ͨ�������֣�ͨ���У�*/
    CS_CSSU_TYPE_RECOVER,               /*ͨ�����ָ���ͨ���У�*/
    CS_CSSU_TYPE_ENTER_3WC,             /*�������ͨ�� (ͨ����)*/
    CS_CSSU_TYPE_RELEASE,               /*�����ֵĺ��б��ͷ�(ͨ����)*/
    CS_CSSU_TYPE_CF_INFO,               /*�յ�ǰת��Ϣ*/
    CS_CSSU_TYPE_RMT_RING,              /*�Է���������״̬�������ڱ���ͨ������(ͨ����)*/
    CS_CSSU_TYPE_CALLING,               /*�����Ѿ�����ͨ(ͨ���л���MT����)*/
    CS_CSSU_TYPE_CF_SUC,                /*���б�ǰת (����MT����)*/
    CS_CSSU_TYPE_CF_OTHER,              /*���������ǰת*/

    CS_CSSU_TYPE_BUTT
}CS_CSSU_TYPE_E;
//#endif

/*CS����ģ����Ϣ�ṹ*/
typedef struct tagCS_MSG_ST
{
	signed int  lMsgType;
	char        acText[AT_RCV_MSG_LENGTH];
}CS_MSG_S;

/*CS����ģ��APP�¼���Ϣ�ṹ*/
typedef struct tagCS_MSG_APP_ST
{
    unsigned int    ulEventType;                    /*�¼�����*/
    unsigned int    ulCrSn;                         /*CR����*/
    unsigned int    ulUeSn;                         /*UE����*/
    unsigned char   aucCallNum[CS_MAX_CALLID_LEN];  /*���к���*/
}CS_MSG_APP_S;

/*****************************************************************************
** �����: USB״̬�仯
** �����ʽ: 
** ��Ӧ��ʽ: 
******************************************************************************/
typedef struct tagCS_MSG_USB_STATUS_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucStatus;       /*��CS_USB_STATUS_E*/
}CS_MSG_USB_STATUS_S;

/*****************************************************************************
** �����: ATִ�н���ϱ�
** �����ʽ: 
** ��Ӧ��ʽ: 
******************************************************************************/
typedef struct tagCS_MSG_RESULT_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucType;
}CS_MSG_RESULT_S;

/*****************************************************************************
** �����: ��������ָʾRING
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>RING<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_RING_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
}CS_MSG_RING_S;

/*****************************************************************************
** �����: ���������ʾ+CLIP
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>+CLIP:<number>,<type>,,,,<CLI validity><CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CLIP_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucTelType;      /*�������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ�
                                      �����ŷ������� +CMGS��һ���У�SC�����е�
                                      type_addr�����ľ��嶨�塣255,CDMA��֧��*/
    unsigned char   ucCliFlag;      /*0 CLI ��Ч
                                      1 CLI �����з��𷽾ܾ�.
                                      2 ���ڷ�����������ƻ������������CLI�����á�
                                      <type>��<CLI validity>֮�䱣���������ֶΡ�*/
    char    aucTelNum[CS_MAX_CALLID_LEN]; /*������룬ASCII �ַ����Ϸ����ַ�����:
                                            ��0�� �C ��9��, ��*��, ��#��*/
}CS_MSG_CLIP_S;

/*****************************************************************************
** �����: ��������ָʾ^ORIG
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^ORIG<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ORIG_ST
{
    unsigned char   ucCmdFrom;      /*��Ϣ��Դģ��*/
    unsigned char   ucResult;       /*��Ӧ���: 1ΪOK;0ΪNOK*/
    unsigned char   ucCallId;       /*���б�ʶCallId*/
    unsigned char   ucTelType;      /*�������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ�
                                      �����ŷ������� +CMGS��һ���У�SC�����е�
                                      type_addr�����ľ��嶨�塣255,CDMA��֧��*/
}CS_MSG_ORIG_S;

/*****************************************************************************
** �����: ���ӽ���ָʾ^CONF
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^CONF<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CONF_ST
{
    unsigned char   ucCmdFrom;      /*��Ϣ��Դģ��*/
    unsigned char   ucResult;       /*��Ӧ���: 1ΪOK;0ΪNOK*/
    unsigned char   ucCallId;       /*���б�ʶCallId*/
    unsigned char   ucTelType;      /*�������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ�
                                      �����ŷ������� +CMGS��һ���У�SC�����е�
                                      type_addr�����ľ��嶨�塣255,CDMA��֧��*/
}CS_MSG_CONF_S;

/*****************************************************************************
** �����: ͨ������ָʾ^CONN
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^CONN<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CONN_ST
{
    unsigned char   ucCmdFrom;      /*��Ϣ��Դģ��*/
    unsigned char   ucResult;       /*��Ӧ���: 1ΪOK;0ΪNOK*/
    unsigned char   ucCallId;       /*���б�ʶCallId*/
    unsigned char   ucTelType;      /*�������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ�
                                      �����ŷ������� +CMGS��һ���У�SC�����е�
                                      type_addr�����ľ��嶨�塣255,CDMA��֧��*/
}CS_MSG_CONN_S;

/*****************************************************************************
** �����: ͨ������ָʾ^CEND
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^CEND:<call_x>,< duration >,<end_status>[,<cc_cause>]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_CEND_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucCallId;       /*����ID��ECȡֵ0��6*/
    unsigned char   ucEndCause;     /*���н���ԭ��ECȡֵ255*/
    unsigned int    ulDuration;     /*ͨ��ʱ������sΪ��λ*/
    unsigned char   ucReserve[3];   /*�����ֽڣ���չ�Ͷ�����*/
    unsigned char   auCallCtlCause; /*���п�����Ϣ ECȡֵ255
                             ע�����Ϊ���������ĺ��н����Ż����ֶ�<cc_cause>���ϱ�
                             ������ط����ĺ��У���û�еõ��������Ӧ��
                             ���оͽ�������ʱ�Ͳ�����<cc_cause>�ϱ���*/
}CS_MSG_CEND_S; 

/*****************************************************************************
** �����: ϵͳ����Ϣ��ѯ
** �����ʽ: AT^SYSINFO
** ��Ӧ��ʽ: <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,< roam_status >,
             < sys_mode >,< sim_state >[,<lock_state>,<sys_submode>]<CR><LF><CR>
             <LF>OK<CR><LF> <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,
             < roam_status >,< sys_mode >,< sim_state >[,<lock_state>,
             <sys_submode>]<CR><LF><CR><LF>OK<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_SYSINFO_ST
{
	unsigned char   ucCmdFrom;
	unsigned char   ucResult;
	unsigned char   ucSrvStatus;    /*ϵͳ����״̬.��CS_SERVER_STATUS_E,ȡֵ����
                                      0:  �޷���
                                      1:  �����Ʒ���
                                      2:  ������Ч 
                                      3:  �����Ƶ��������
                                      4:  ʡ�����˯״̬            */
	unsigned char   ucSrvDomain;    /*ϵͳ������.��CS_ZONE_STATUS_E,ȡֵ����
                                      0��    �޷���
                                      1��    ��CS����
                                      2��    ��PS����
                                      3��    PS+CS����
                                      4��    CS��PS��δע�ᣬ����������״̬
                                      255: CDMA��֧�֡�
        					          250: ��ʾδ��ȡ����ֵ�������� */
	unsigned char   ucRoamStatus;   /*����״̬.��CS_ROAM_STATUS_E,ȡֵ����
                                      0��������״̬
                                      1������״̬                   */
	unsigned char   ucSysMode;      /*ϵͳģʽ.��CS_SYS_MODE_E,ȡֵ����
        							  0���޷���
        							  1��AMPSģʽ���ݲ�ʹ�ã�
        							  2��CDMAģʽ���ݲ�ʹ�ã�
        							  3��GSM/GPRS ģʽ
        							  4��HDRģʽ
        							  5��WCDMAģʽ
        							  6��GPSģʽ                    */	
	unsigned char   ucSimState;     /*SIM��״̬.��CS_SIM_STATUS_E,ȡֵ����
    	                              0��  USIM��״̬��Ч
    	                              1:   USIM��״̬��Ч  
    	                              2��  USIM��CS����Ч��EW��
    	                              3��  USIM��PS����Ч(EW)
    	                              4��  USIM��PS+CS�¾���Ч(EW)
    	                              240: ROMSIM�汾
    	                              255: USIM��������   
    					              250: ��ʾδ��ȡ����ֵ�������� */
	unsigned char   ucLockState;    /*SIM����LOCK״̬.��CS_SIMLOCK_STATUS_E,ȡֵ����
                                      0��SIM��δ��SIMLOCK��������
                                      1��SIM����SIMLOCK��������     */
	unsigned char   ucSysSubMode;   /*��CS_SUBSYS_MODE_E
	                                  0���޷���
    					              1��GSMģʽ
    					              2��GPRSģʽ
    					              3��EDGEģʽ
    					              4��WCDMAģʽ
    					              5��HSDPAģʽ
    					              6��HSUPAģʽ
        							  7��HSDPA&HSUPAģʽ            */
	unsigned char   ucExtern1;
	unsigned char   ucExtern2;
	unsigned char   ucExtern3;
}CS_MSG_SYSINFO_S;

/*****************************************************************************
** �����: SIM ״̬�仯ָʾ
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^SIMST:<sim_state>[,<lock_state>]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_SIM_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   ucSimState;    /*USIM��״̬��ȡֵ���£�
                                     0:   USIM��״̬��Ч
                                     1:   USIM��״̬��Ч  
                                     2:   USIM��CS����Ч
                                     3:   USIM��PS����Ч
                                     4:   USIM��PS+CS�¾���Ч
                                     255: USIM�������ڣ�����USIM��ȷʵ�����ں���Ϊ
                                      ��SIMLOCK��ס�������������������ʱ��Ӧͨ
                                      ��ucLockState��ȡֵ�ж�USIM������ʵ״̬*/
    unsigned char   ucLockState;    /*SIM����LOCK״̬��ȡֵ���£�
                                    0: SIM��δ��SIMLOCK��������
                                    1: SIM����SIMLOCK��������*/
}CS_MSG_SIM_S;

/* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
/*****************************************************************************
** �����: ����״̬�仯ָʾ
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^NDISSTAT:<stat>[,<err_code>[,<wx_state>[,<PDP_type>]]]<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_NDISSTAT_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult; 
    unsigned char   ucNdisState;    /*NDIS״̬��ȡֵ���£�
                                     0�����ӶϿ���
									 1�������ӣ�
									 2�������У����ڵ����Զ�����ʱ�ϱ����ݲ�֧�֣���
									 3���Ͽ��У����ڵ����Զ�����ʱ�ϱ����ݲ�֧�֣���*/
	unsigned char   wx_state;		/*����ֵ��WiMax ���ݿ���״̬���ݲ�֧�֡�*/	
    unsigned int   err_code;       /*����ֵ�������룬���ڲ���ʧ��ʱ�ϱ��ò�����*/
	unsigned char	PDP_type[10];  /*"IPV4" "IPV6"*/
}CS_MSG_NDISSTAT_S;

/*****************************************************************************
** �����: IPV4����״̬DHCP������
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^DHCP:<clip>,<netmask>,<gate>,<dhcp>,<pDNS>,<sDNS>,<max_rx_data>,<max_tx_data><CR><LF>
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
** �����: IPV6����״̬DHCP������
** �����ʽ: 
** ��Ӧ��ʽ: <CR><LF>^DHCPV6: <clip_v6>,<netmask_v6>,<gate_v6>,<dhcp_v6>,<pDNS_v6>,<sDNS_v6>,<max_rx_data>,<max_tx_data><CR><LF><CR><LF>OK<CR><LF>
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
** �����: ͨ��ʱ����ѯ
** �����ʽ: AT^CDUR=<idx>
** ��Ӧ��ʽ: <CR<LF>^cdur:<idx>,<duration><CR<LF><CR<LF>OK<CR<LF>
******************************************************************************/
typedef struct tagCS_MSG_CDUR_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
    unsigned char   ucExtern1;
    unsigned char   ucExtern2;
    unsigned int    ulDuration;         /*ͨ��ʱ������sΪ��λ*/
}CS_MSG_CDUR_S;

/*****************************************************************************
** �����: ҵ�������
** �����ʽ: ATD[digits][I/i][;]
** ��Ӧ��ʽ: <CR><LF>OK<CR><LF>ִ�д���<CR><LF>NO CARRIER<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ATD_ST
{
    unsigned char   ucCmdFrom;
    unsigned char   ucResult;
}CS_MSG_ATD_S;

/*****************************************************************************
** �����: ҵ���������
** �����ʽ: ATA
** ��Ӧ��ʽ: <CR><LF>OK<CR><LF>
******************************************************************************/
typedef struct tagCS_MSG_ATA_ST
{
    unsigned char   ucCmdFrom; 
    unsigned char   ucResult;
}CS_MSG_ATA_S;

/*cs_20080828       chenkeyun       end*/
/*****************************************************************************
** �����: ���еȴ�����ʱ������ָʾ+CCWA
** �����ʽ: ATA
** ��Ӧ��ʽ: +CCWA:<number>,<type>,<class>,[<alpha>][,<CLIvalidity>[,<subaddr>,<satype> [,<priority> ]]]
ȡֵ˵��
<n> ����ֹ/ʹ��result code�������ϱ�����
0	��ֹ
1	ʹ��
<mode>��
0	ȥ����
1	����
2	״̬��ѯ
<classx>��ҵ�����ȱʡֵΪ7���������������ݺʹ���ҵ�����ϡ�Ŀǰֻ֧������ҵ��ȡֵΪ1��
1	����ҵ��
2	����ҵ��
4	����
8	����Ϣ
16	ͬ����·����
32	�첽��·����
64	ר�÷������
128	ר��PAD����
<status>��
0	δ����
1	����
<number>�����ڵȴ�״̬���������
<type>���������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ�"����ǰת +CCFC"һ���У���������<type>�����ľ��嶨�塣
<alpha>������<number>���ַ����������֡����������� TBD��
<CLI validity>��
0	CLI ��Ч
1	CLI �����з��𷽱���.
2	���ڷ���������������CLI�����á�
��CLI�����ã�<CLI validity>=2����<number> Ӧ��Ϊ�� ��""�� ������ <type>ȡֵ�����塣
<subaddr>�ַ��͵ĺ����ӵ�ַ����ʽ��<satype>���������������� TBD��
<satype>8bit����ĺ����ӵ�ַ��ʽ������������ TBD��
<priority>�����͵Ĳ�����������ʾ��������ȼ������������� TBD��
ע��TBD�ο�3.3����TBD˵����


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
���+CSSN��������ʹ��+CSSI��<n>=1�������ڽ���MO�����У����յ����緢���Ĳ���ҵ��ָʾʱ���ϱ�TEָʾ
** ��Ӧ��ʽ: +CSSI: <code1>[,<index>[,<number>,<type>
[,<subaddr>,<satype>]]]
ȡֵ˵��
<code1>�� ����ȡֵΪ0-8����HOLD/RETRIEVEָʾ��ص�ȡֵΪ2��3��
0	����������ת�Ʊ�����
1	ĳһ������ת�Ʊ�����
2  ���б�ת��
3	  ���д��ڵȴ�״̬
4	 ����һ��CUG����
5 ���б���ֹ
6	���类��ֹ 
7	��ֹCLIR���ܾ�
8	���б�ǰת
<index> 0-9, 10��ʾû�б��(�������ã�TBD)
<number>: �ַ��͵绰���룬��ʽ��<type>����(�������ã�TBD)
<type>: �������ͣ�����ȡֵ�ɲμ�"����ǰת +CCFC"һ���У���������<type>�����ľ��嶨�塣(�������ã�TBD)
<subaddr>:�ַ����ͷֻ����룬�����ʽ��<satype>����(�������ã�TBD)
<satype>:�ֻ���������ͣ�8bit��������͸�ʽ(�������ã�TBD)
ע��TBD˵��,�ο�3.7.3 TBD˵��

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
���+CSSN��������ʹ��+CSSU��<m>=1��������ͨ�������кͽ���MT�����У����յ����緢���Ĳ���ҵ��ָʾʱ���ϱ�TEָʾ
ȡֵ˵��
<code2>�� ����ȡֵΪ0-10����HOLD/RETRIEVEָʾ��ص�ȡֵΪ2��3��
0	��ת�Ƶĺ��У�����MT���У�
1	CUG ���У�����MT���У�
2  ͨ�������֣�ͨ���У�
3	ͨ�����ָ���ͨ���У�
4	�������ͨ�� (ͨ����)
5	�����ֵĺ��б��ͷ�(ͨ����)
6	�յ�ǰת��Ϣ 
7	�Է���������״̬�������ڱ���ͨ������(ͨ����)
8	�����Ѿ�����ͨ(ͨ���л���MT����)
9	���б�ǰת (����MT����)
10	���������ǰת
<index> 0-9, 10��ʾû�б��(�������ã�TBD)
<number>: �ַ��͵绰���룬��ʽ��<type>����(�������ã�TBD)
<type>: �������ͣ�����ȡֵ�ɲμ�"����ǰת +CCFC"һ���У���������<type>�����ľ��嶨�塣(�������ã�TBD)
<subaddr>:�ַ����ͷֻ����룬�����ʽ��<satype>����(�������ã�TBD)
<satype>:�ֻ���������ͣ�8bit��������͸�ʽ(�������ã�TBD)
ע��TBD˵�������Ӹ�ͨ�Ĵ��뿴����Щ������û���ڴ������ṩ��ԭ���Ǹ�ͨ�Ĵ����ǲο��ϵ�Э��ʵ�ֵģ�������Щ�����ǿ�ѡ�ģ����Խ��飺ά�ָ�ͨ�Ĵ��롣
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


