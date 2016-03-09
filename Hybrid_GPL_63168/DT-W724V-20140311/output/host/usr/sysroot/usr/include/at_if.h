/**\file at_if.h
  *atserver���������̹���ӿ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author w00190448
  *\date  2012-1-2
*/
#ifndef __AT_IF_H__
#define __AT_IF_H__

#include "atptypes.h"
#include "at_lib.h"
#include "sms_cmp_prs.h"

//!��ʶ��ǰAT���������
#define AT_CMD_TYPE_CHECK        0         //��ѯ����
#define AT_CMD_TYPE_EXECUTE      1        //ִ������
#define AT_CMD_TYPE_TEST           2         //��������
#define AT_CMD_TYPE_OTHER         3        //��������

//!ִ��AT����ؽ��
typedef enum
{
    AT_CMD_RES_ERR = -3,//ERROR,�����͵�ERROR
    AT_CMD_RES_SUCCESS = -2,  //ok
    AT_CMD_RES_BUSY = -1,  //����cme cms error�ļ���,
    //ǰ����Ϊ·��ģ���Զ��岢ʹ�õĴ����.
    AT_CMD_RES_SIM_NOT_INSERT =10,
    AT_CMD_RES_PIN_REQ= 11, //���ǹ��ĵ�PIN����.
    AT_CMD_RES_PUK_REQ = 12,
    AT_CMD_RES_SIM_FAIL = 13,
    AT_CMD_RES_SMS_FULL = 14,/* <DTS2013083001384 xiazhongyue 20130830 modify> */
    AT_CMD_RES_NO_NETWORK = 15,	 /* <DTS2013103107401  z00185914 2013/11/07 BEGIN */
    AT_CMD_RES_REJECT_NETWORK = 16,  /* DTS2013103107401   z00185914 2013/11/07 END> */
    AT_CMD_RES_AMSS_MAX = 65535//���ֵ.
}AT_AMSS_RES_EN;

#define  AT_IF_BASE_TIMEOUT  5	/* <DTS2013083001384 xiazhongyue 20130830 modify> */

#define AT_REP_TIMEOUT   1     //�����ϱ��ĵȴ�ʱ��.

#define AT_PARAM_LEN             100      //AT�����������󳤶�
#define AT_MAX_NUM_PARAM         100      //AT��������������

#define AT_RESULT_MAX_NUM_PARAM  20       //AT������������������

#define CLCK_EXE_MODE_UNLOCK_PIN "0"      //����PIN��
#define CLCK_EXE_MODE_LOCK_PIN   "1"      // PIN������
#define CLCK_EXE_MODE_CHECK_PIN  "2"      //��ѯPIN״̬��<mode>=2ʱ

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

#define WM_VERSION_LEN          32      /* ����ģ��汾�ŵĳ��� */

#define SIG_USER_MIN               40
#define SIG_WAIT_CLIENT     (SIG_USER_MIN+1)      /* signal waiting from client */
#define SIG_TIME_CLIENT     (SIG_USER_MIN+2)      /* signal waiting from client */
#define SIG_NULL_CLIENT     (SIG_USER_MIN+3)      /* signal waiting from client */

#define SMS_ACK_SINGAL        (SIG_USER_MIN+4)
#define SMS_NAK_SINGAL        (SIG_USER_MIN+5)
#define SMS_HTTP_SINGAL       (SIG_USER_MIN+6)
#define ATS_EXIT_SIGN            (SIG_USER_MIN+7)
#define ATS_NEW_SMS_SIGN    (SIG_USER_MIN+8)

/*AT+CPAS��ִ�н��*/
#define PHONE_ACT_STATUS_READY   "0"
#define PHONE_ACT_STATUS_RINGING "3"
#define PHONE_ACT_STATUS_BUSY    "4"

#define CTRL_Z  '\32'
#define SIM_OPERATOR_NAME_MAX               256
#define OPL_LIST_MAX               128
#define OPL_ITEM_LEN_MAX       256
#define PNN_INDEX_LEN             8
/* <DTS2013083001384 xiazhongyue 20130830 begin */
/* ��¼���߲��ʱ���ļ� */
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


/*AT^SYSCFG�������ķ��ؽ��*/
typedef struct NetConfigurationResStruct
{
    int           CmdType;             //AT���������
    char          Mode[5];             //ϵͳģʽ�ο�
    char          Acqorder[5];         //����������ο�
    char          Band[20];            //Ƶ��ѡ��
    char          Roam[5];             //����֧��
    char          Srvdomain[5];        //������
    AT_AMSS_RES_EN           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}NetConfigurationRes;

/*AT^SYSCFGִ�в�������Ĳ���*/
typedef struct  NetConfigurationTypeStruct
{
    char          Mode[5];             //ϵͳģʽ�ο�
    char          Acqorder[5];         //����������ο�
    char          Band[20];            //Ƶ��ѡ�� 
    char          Roam[5];             //����֧��
    char          Srvdomain[5];        //������
    int           Reserve;             //������
}NetConfigurationType;

/*<AP7D00922,Optus, liukeke 2007-10-08 MODIFY BEGIN*/
/*AT+COPS=?����ص���Ӫ���б���ÿһ����Ӫ�̵���Ϣ*/
typedef struct OperatorInfoStruct
{
    char       State[5];                //�����״̬��ʶ
    char       LongAlphanumeric[SIM_OPERATOR_NAME_MAX];    //��Ӫ��ȫ��
    char       ShortAlphanumeric[SIM_OPERATOR_NAME_MAX];   //��Ӫ�̶��������ȴ�ȷ�ϣ�
    char       numeric[64];             //��Ӫ�����ִ���   �����ȴ�ȷ�ϣ�
    char       Rat[5];                  //���߽��뼼��
    int        Reserve;                 //������
}OperatorInfo;
/*AP7D00922,Optus, liukeke 2007-10-08 MODIFY END>*/

/*AT+COPS=?����������Ӫ����Ϣ������*/
typedef struct OperatorListStruct
{
    int            OperatorNumber; //�ϱ�����Ӫ���ܸ���
    OperatorInfo   Operator[20];   //�ϱ�����Ӫ���б����֧��20��
    int            Reserve;        //������
}OperatorList;

/*AT+COPSע������ʱ���������*/
typedef struct NetChooseOperatorParamStruct    
{        
    char      Mode[5];                  //����ѡ��ģʽ
    char      Format[5];                //��Ӫ����Ϣ�ĸ�ʽ
    char      OperatorName[64];         //��Ӫ�̵�����
    char      Rat[5];                   //���߽��뼼��
    int       Reserve;                  //������
}NetChooseOperatorParam;

#define CHOOSE_OPER_MODE_AUTO            "0"        /*�Զ�����*/
#define CHOOSE_OPER_MODE_MANUAL          "1"        /*�ֶ�����*/
#define CHOOSE_OPER_TO_LOGON             "2"        /*ȥע������*/
#define OPER_NAME_FORMAT                 "3"        /*ΪAT+CPOS?���������õķ��ظ�ʽ*/
#define CHOOSE_OPER_MODE_AUTO_MANUAL     "4"        /*�ֶ����Զ����*/
#define SYS_MODE_AUTO                    "2"        /*�Զ�ѡ��*/
#define SYS_MODE_GSM_ONLY                "13"       /*GSM only*/
#define SYS_MODE_WCDMA_ONLY              "14"       /*WCDMA only*/
#define SYS_MODE_NO_CHANGE               "16"       /*�ޱ仯*/
#define SYS_ACQ_ORDER_AUTO               "0"        /*�Զ�*/
#define SYS_ACQ_ORDER_GSM_FIRST          "1"        /*��GSM��WCDMA*/
#define SYS_ACQ_ORDER_WCDMA_FIRST        "2"        /*��WCDMA��GSM*/
#define SYS_ACQ_ORDER_NO_CHANGE          "3"        /*�ޱ仯*/
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
#define OPER_RAT_TECNO_GSM_GPRS          "0"        /* GSM/GPRS��ʽ*/
#define OPER_RAT_TECNO_WCDMA             "2"        /* WCDMA��ʽ*/
#define OPER_INFO_STATUS_UNKNOWN         "0"        /*δ֪��*/
#define OPER_INFO_STATUS_USEABLE         "1"        /*���õ�*/
#define OPER_INFO_STATUS_LOGED_ON        "2"        /*��ǰע���*/
#define OPER_INFO_STATUS_FORBIDDEN       "3"        /*��ֹ*/
/*AT+COPS?���浱ǰ��Ӫ����Ϣ������*/
#define PHONE_ACT_STATUS_READY            "0"
#define PHONE_ACT_STATUS_RINGING          "3"
#define PHONE_ACT_STATUS_BUSY             "4"
/* <AP7D01399,liudan,2008-03-3 ADD BEGIN */
#define OPER_RAT_TECNO_WCDMA_GSM             "30"
/* <AP7D01399,liudan,2008-03-3 ADD END*/
#define OPERATOR_NAME_LEN           64         /* ��Ӫ�̶�����󳤶� */
#define NETWORK_LEN                       4          /* 2G/3G ��󳤶� */

#define MAX_OPERATOR_LINE_LEN          1024
#define OPERATOR_LIST                           "/html/api/ntwk/operator_list.res"/* <DTS2013083001384 xiazhongyue 20130830 modify> */
    
/*<AP7D00922,Optus, liukeke 2007-10-08 MODIFY BEGIN*/
typedef struct
{
    char      Mode[5];                  //����ѡ��ģʽ
    char      Format[5];                //��Ӫ����Ϣ�ĸ�ʽ
    char      OperatorName[64];         //��Ӫ�̵�������
    char      Operator_Short_Name[SIM_OPERATOR_NAME_MAX];         //��Ӫ�̵Ķ�����
    char      Operator_Full_Name[SIM_OPERATOR_NAME_MAX];           //��Ӫ�̵ĳ�����
    char      Rat[5];                   //���߽��뼼��
    int        Reserve;                  //������
}RegistedOperator;
/*AP7D00922,Optus, liukeke 2007-10-08 MODIFY END>*/

/*AT+COPSִ�еĽ��*/
typedef struct NetChooseOperatorResultStruct       
{
    int                   CmdType;      //ָʾ����������ķ��ؽ��
    AT_AMSS_RES_EN                   AtExeResult;  //ִ�в����ķ��ؽ��
    OperatorList         *TotalOper;    //�����������ص���Ӫ���б�
    RegistedOperator     *CurOperInfo;  //�����ѯ����ص���Ӫ����Ϣ
    int                   Reserve;      //������
}NetChooseOperatorResult;

/*AT+CPINִ�н��*/
typedef struct PinMgResultCodeStruct
{
    int            CmdType;       //�������AT����ķ��ؽ��
    char           Code[20];      //PIN״̬
    AT_AMSS_RES_EN AtExeResult;   //AT�����ִ�н��
    int            Reserve;       //������
}PinMgResultCode;

/*PIN��ʣ���������*/
typedef struct PinResTimesStruct
{
    char             ResTimes[5];       //<times>�ֶΣ�ʣ����������
    char             ResPukTimes[5];    //<puk_times>�ֶΣ�ʣ���PUK�������
    char             ResPinTimes[5];    //<pin_times>�ֶΣ�ʣ���PIN�������
    char             ResPuk2Times[5];   //<puk2_times>�ֶΣ�ʣ���PUK2�������
    char             ResPin2Times[5];   //<pin2_times>�ֶΣ�ʣ���PIN2�������
    int              Reserve;           //������
}PinResTimes;


/*AT^CPINִ�н��*/
typedef struct PinMgResultCodeExStruct
{
    int              CmdType;           //�������AT����ķ��ؽ��
    char             Code[20];           //PIN״̬
    PinResTimes     ResTimes;          //ʣ���������
    AT_AMSS_RES_EN              AtExeResult;       //AT�����ִ�н��
    int              Reserve;           //������
}PinMgResultCodeEx;

/*AT+CLCK�������*/
typedef struct PinEnStructStruct
{
    char           Fac[5];         //������Ĳ������� 
    char           Mode[5];        //�����ģʽ��������������ѯ��
    char           Passwd[11];     //PIN���ַ���
    char           classx[5];      //Ĭ��Ϊ7����ǰ��֧��
    int            Reserve;        //�����ֶ�
}PinEnStruct;

/*AT+CLCKִ�н��*/
#define SIM_STATUS_NEGATIVE    "0"  //δ����
#define SIM_STATUS_ACTIVE     "1" //����
typedef struct PinEnableResultStruct
{
    int            CmdType;       //�������AT����ķ��ؽ��
    AT_AMSS_RES_EN            AtExeResult;   //AT�����ִ�н��
    char           Status[5];     //SIM��״̬�Ĳ�ѯ���
    int            Reserve;       //������
}PinEnableResult;

/*AT+CPWDִ�н��*/
typedef struct PinModifyResultCodeStruct
{
    int            CmdType;        //��Ӧ����AT�����ִ�н��
    AT_AMSS_RES_EN            AtExeResult;    //AT�����ִ�н��
    int            Reserve;        //������
}PinModifyResultCode;

/*AT+CPAS��ִ�н��*/
typedef struct PhonseActStatusStruct
{
    int            CmdType;        //��Ӧ����AT�����ִ�н��
    AT_AMSS_RES_EN AtExeResult;    //AT�����ִ�н��
    char           PActStatus[5];  //��ǰ�Ƿ���ͨ��״̬
    int            Reserve;        //������
}PhoneActStatus;
/* <D00018.WEB SERVER: liudan 2007-4-9 DEL BEGIN */
/* D00018.WEB SERVER: liudan 2007-4-9 DEL END> */

/*AP7D00728.H3G.R99: liudan 2007-8-23 ADD BEGIN */
typedef struct R99OnlyStruct
{
    int            CmdType;       //�������AT����ķ��ؽ��
    char           Code[5];      //AT^HSDPA״̬
    AT_AMSS_RES_EN            AtExeResult;   //AT�����ִ�н��
    int            Reserve;       //������
}StR99Only;
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/

/* <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
typedef struct CvmnqStruct
{
    int            CmdType;                    /*�������AT����ķ��ؽ��*/
    char           voice_mail_number[25];      //�����������,���֧�ֵ�20���ַ�.
    char           nv_number[25];              //ԭʼ�ĺ���(����NV�ж�ȡ�ĺ���)
    AT_AMSS_RES_EN            AtExeResult;                //AT�����ִ�н��
    int            Reserve;                    //������
}StCvmnq;

/*����״̬�仯ָʾ^SRVST */
typedef struct{
    char  srv_status[3];
} StSrvstInfo;

/*ϵͳģʽ�仯�¼�ָʾ ^MODE */
typedef struct{
    char  sys_mode[3];
} StSysModeInfo;
/* RSSI �仯ָʾ ^RSSI */

typedef struct{
    char  rssi [5];
} StRssiInfo;

/* DS�����ϱ�^DSFLOWRPT */
typedef struct{
    unsigned int  curr_ds_time;// ��ǰ���DS������ʱ�䣬��λΪ��
    unsigned int  tx_rate; //��ʾ��ǰ�ķ������ʣ���λΪ�ֽ�ÿ��
    unsigned int  rx_rate; //��ʾ��ǰ�Ľ������ʣ���λΪ�ֽ�ÿ�롣
    unsigned long long  curr_tx_flow;// ��ʾ��ǰ���DS�ķ�����������λΪ�ֽڡ�
    unsigned long long  curr_rx_flow;// ��ʾ��ǰ���DS�Ľ�����������λΪ�ֽڡ�
    unsigned int  qos_tx_rate;// �������Э�̺�ȷ����PDP���ӷ������ʣ���λΪ�ֽ�ÿ�롣
    unsigned int  qos_rx_rate;// �������Э�̺�ȷ����PDP���ӽ������ʣ���λΪ�ֽ�ÿ�롣
} StDsflowrptInfo;

/* DS������ѯ^DSFLOWQRY*/
typedef struct{
    unsigned int  last_ds_time;// ��ǰ���DS������ʱ�䣬��λΪ�롣
    unsigned long long  last_tx_flow;// ��ʾ��ǰ���DS�ķ�����������λΪ�ֽڡ�
    unsigned long long  last_rx_flow;// ��ʾ��ǰ���DS�Ľ�����������λΪ�ֽڡ�
    unsigned int total_ds_time;// �ۼ�DS������ʱ�䣬��λΪ�롣
    unsigned long long  total_tx_flow;//  �ۼ�DS�ķ�����������λΪ�ֽڡ�
    unsigned long long  total_rx_flow;//  �ۼ�DS�Ľ�����������λΪ�ֽڡ�
    AT_AMSS_RES_EN AtExeResult;   //AT�����ִ�н��
} StDsflowQureyInfo;

/* ��� �汾�Ų�ѯ+CGMR */
typedef struct{
    char acSwVer[64];
    AT_AMSS_RES_EN AtExeResult;   //AT�����ִ�н��
} StSwVer;

typedef struct {
    int rcsq_level;
    int rscp;
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCsqlvl;

typedef struct               /* ϵͳ��Ϣ */
{
    VOS_INT32 srv_status;
    VOS_INT32 srv_domain;
    VOS_INT32 roam_status;
    VOS_INT32 sys_mode;
    VOS_INT32 sim_state;
    VOS_INT32 lock_state;
    VOS_INT32 submode;
    AT_AMSS_RES_EN AtExeResult;   //AT�����ִ�н��
} StSysinfo;

/* ppp IP��ַ������״̬ */
typedef struct
{
    unsigned int ip_addr;           // IP ��ַ
    int ppp_state;         // PPP ����״̬
    unsigned int cur_time_len;      // ��ǰ����ʱ��
    unsigned int total_time_len;    // �ۼ�����ʱ��
    unsigned long long cur_ppp_ibytes;       // PPP��ǰ��������
    unsigned long long cur_ppp_obytes;       // PPP��ǰ��������
    unsigned long long total_ppp_ibytes;     // PPP�ۼƽ�������
    unsigned long long total_ppp_obytes;     // PPP�ۼƷ�������
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
 AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��

}StCpms;

typedef struct{
    at_cmd_result  amss_res;
    dsati_cms_err_e_type  cms_res;
    dsat_cme_error_e_type cme_res;
}StAtRes;

typedef struct{
     AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
}StCmmt;


typedef struct ScidStruct
{
    char           Code[21];      //AT^HSDPA״̬
    AT_AMSS_RES_EN            AtExeResult;   //AT�����ִ�н��
}StScid;


/*<AP7D00767,Austria-PNN, liukeke 2007-08-16 ADD BEGIN*/
/*��SIM��ȡ�õ���Ӫ����Ϣ*/
typedef struct OplStruct
{
    char   numeric[64];             //��Ӫ��������
    char   lac_start[8];             //�������������ʼֵ
    char   lac_end[8];             //����������Ľ���ֵ
    char   index_pnn[PNN_INDEX_LEN];            //ָ��PNN�ĵ�index_pnn����¼
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
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
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

/*��ѯ�����ϱ�.*/
int AT_Rssi(StRssiInfo *rssi_res);
int AT_Dsflowrpt(StDsflowrptInfo *flow_res);
/*����cops��Ϣ*/
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

/*AT^CLIP��������ؽṹ��*/
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
/* �ӹ����ļ��ж�ȡ����Ԥ���� ,�����ǽ���ǰ�Ķ��Žṹ��.*/
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
    int  index;                   /*���ŵ����ֵ��*/
    enum EMSGState state;         /* �˶��ŵ����͡�0=���յ���δ�����ţ�1=���յ����Ѷ����ţ�2=�洢��δ������ 3=�洢���ѷ�����*/
    unsigned short length;        /* PDU���ݵ��ֽ���,���Ϊ255*/
    unsigned char reserved[PER_SMS_RESERVED_SIZE];         /* �����֣�����û��ʹ�á�*/
    unsigned char pdu[SMS_PDU_SIZE];    /*pdu��*/
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
    VOS_INT32         CmdType;             // AT���������
    VOS_CHAR          Acqorder[5];         // ����������ο�
    VOS_CHAR          Band[64];            // Ƶ��ѡ��
    VOS_CHAR          Roam[5];             // ����֧��
    VOS_CHAR          Srvdomain[5];        // ������
    VOS_CHAR          LTEBand[64];          // LTEBand

    AT_AMSS_RES_EN    AtExeResult;         //AT����ִ�н��
    VOS_INT32         Reserve;             //������
}NetCfgExResType;

/*AT^SYSCFGEXִ�в�������Ĳ���*/
typedef struct NetCfgExSetStruct
{
    VOS_CHAR          Acqorder[5];         /* ����������ο� */
    VOS_CHAR          Band[64];            /* Ƶ��ѡ�� */
    VOS_CHAR          Roam[5];             /* ����֧�� */
    VOS_CHAR          Srvdomain[5];        /* ������ */
    VOS_CHAR          LTEBand[64];          /* LTEBand */
    VOS_INT32         Reserve;             /*������*/
}NetCfgExSetType;


/* AT^LOCINFO?���ؽ�� */
typedef struct LocinfoResStruct
{
    VOS_INT32         CmdType;             // AT���������
    VOS_CHAR          plmn[10];         
    VOS_CHAR          lac[10];            
    VOS_CHAR          rac[10];             
    VOS_CHAR          cell_id[10];        
    AT_AMSS_RES_EN    AtExeResult;         //AT����ִ�н��
    VOS_INT32         Reserve;             //������
}LocinfoResType;

/* AT^LCELLINFO��������� */
typedef struct LcellinfoResStruct
{
    VOS_INT32         CmdType;             // AT���������
    VOS_CHAR          Freq[10];         
    VOS_CHAR          Band[4];            
    VOS_CHAR          CellId[10];             
    VOS_CHAR          Rsrp[6];        
    VOS_CHAR          Rsrq[6];          
    VOS_CHAR          Rssi[6];          

    AT_AMSS_RES_EN    AtExeResult;         //AT����ִ�н��
    VOS_INT32         Reserve;             //������
}LcellinfoResType;

/*AT^LCELLINFOִ�в�������Ĳ���*/
typedef struct  LcellinfoSetStruct
{
    VOS_CHAR          Acqorder[5];         // ����������ο�
    VOS_CHAR          Freq[10];         
    VOS_CHAR          Band[4];            
    VOS_CHAR          CellId[10];             
    VOS_CHAR          Rsrp[6];        
    VOS_CHAR          Rsrq[6];          
    VOS_CHAR          Rssi[6];          
    VOS_INT32         Reserve;             //������
}LcellinfoSetType;

/* AT^CECELLID��������� */
typedef struct CecellIdResStruct
{
    VOS_INT32         CmdType;             // AT���������
    VOS_CHAR          Plmn[10];         
    VOS_CHAR          Ci[10];            
    VOS_CHAR          Pci[10];             
    VOS_CHAR          Tac[10];        

    AT_AMSS_RES_EN     AtExeResult;         //AT����ִ�н��
    VOS_INT32          Reserve;             //������
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
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    //ATS_CMD_RESULT_EN
}CellInfoResType;

typedef struct               /* ϵͳ��Ϣ */
{
    VOS_INT32 version;
    VOS_INT32 pin_correct;
    VOS_INT32 iccid_correct;
    VOS_INT32 ecc_check;
    VOS_INT32 lock_style;
    VOS_INT32 total_num;
    VOS_INT32 num_been_locked;
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
} StLockuim2ue;

/* ����͸�ʽ*/
typedef struct ConfigurationResStruct
{
    VOS_INT32  CmdType;             //AT���������
    VOS_CHAR   Mode[8];             //ģʽ
    VOS_INT32  AtExeResult;         //AT����ִ�н��
    VOS_INT32  Reserve;             //������
}ConfigurationRes;

/* ִ�в����ķ��ؽ��*/
typedef struct ConfigurationTypeStruct
{
    VOS_CHAR  Mode[8];             //ģʽ
    VOS_INT32 Reserve;             //������
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
 AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
}StRoutcfgs;

/* ɾ�����ŵ������Ĳ��� */
typedef struct {
    VOS_INT32  index;
    VOS_INT32  delflag;
    VOS_INT32  Reserve;       //������
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
}StCmgdPara;

#define COMM_STR_LEN_32 32
/*���úͲ�ѯ�������ĺ���*/
typedef struct {
    char sca[COMM_STR_LEN_32];
    char tosca[COMM_STR_LEN_32];
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCsca;

/* CMSS������������ */
typedef struct {
    int index;
    char da[SMS_NUMBER_LENGTH+3];     /* ���Ž����ߵĺ���"13801001372" ��Ҫ������*/
    int toda;      /* �������� */
    int  Reserve;       //������
}StCmssPara;

/* �Ӵ洢�����Ͷ��ź�ķ��ؽ���� */
typedef struct {
    int mr;
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCmssRes;

/* ������ŵķ��ؽ�� */
typedef struct {
    int  index;
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCmgwRes;

/* at����at^sn���ؽ���� */
typedef struct {
    char sn[SN_MAX_LENGTH];
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StSnRes;

/* �Ӵ洢�����Ͷ��ź�ķ��ؽ���� */
typedef struct {
    int status;
    AT_AMSS_RES_EN  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
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
