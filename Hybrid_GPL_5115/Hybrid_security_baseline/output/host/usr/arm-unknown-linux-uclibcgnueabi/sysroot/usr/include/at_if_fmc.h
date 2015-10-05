#ifndef __AT_IF_H__
#define __AT_IF_H__
/***********************************************************************
  ��Ȩ��Ϣ : ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���   : at_if.c
  ����     : chekeyun
  �汾     : 
  �������� : 2007-3-21
  ������� : 
  �������� : AT Server���������̹���Ľӿ�.
      
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>     <�޸�ʱ��>  <�汾>  <�޸�����>
    chengangyi   20070321    0.0.1    ��ʼ��
  ��ע: 
************************************************************************/
/*******************************************************************************
Edit History of Huawei: 
<�޸���>     <�޸�ʱ��>    <���ⵥ��>          <�޸�����>
l00162999    2012/11/19    DTS2012110602724    B890-66 V100R001C259B111(Router) �����������AssureNet������Ҫʵ��email���͹���
z00159386    2012/10/16    DTS2012102201180      TELUS�������󣬰���Voicemail�����ο��ء�SMS�ض���
l00162999    2012/12/10    DTS2012120601906      [·��]B890-66 V100R001C259B116SP01(Router)���ڹ������ƶ������л�������£����Ͷ��ŵĶ�����ʾ������ʱ����󣬸����Ի�ȡ��2013��1��1�յ�ʱ�䡣
z00159386    2013/01/09    DTS2012123101571     LTE���磬����wireless profileΪTELUS ISP�󣬵���ע������ʹ�õ�APN������һ�����õ�profile��TELUS VPN�����ڵ�APN
l00162999    2013-01-24    DTS2013012205684    [Web][B880-73]-V100R001C994B002(ROUTER): b880��������ģʽʹ�����µ�at����AT^SYSCFGEX��web server��AT server��Ҫ����Ӧ�ĸĶ����Web�����ߵĲ�ѯ���޸�
z00185914    2013/02/25    DTS2013021908159 	 ������ѡ��ʹ���ֶ�����,�����Գ���ҳ�泤ʱ���޷��������
l00162999    2013/03/13    DTS2013021907887      [����ѡ��][B880]-V100R001C994B100(AMSS): ����ע��4G�����diagnositcҳ���������Ϣδ����
z81004143    2013/05/03    DTS2013050301791     [TR069] B890-66 V100R001C994B122(ROUTER): ����TR069����
*******************************************************************************/


#include "sms_cmp_prs.h"
#include "at_lib_fmc.h"
#include "at_utils_fmc.h"
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
#include <stdio.h>
/* DTS2013012602546 z81004485 2013/1/28 end> */
/* DTS2012110602724 l00162999 20121117 end> */
/* <DTS2013021908159 ʹ���ֶ�����,�����Գ���ҳ�泤ʱ���޷�������� z00185914 20130225 BEGIN */
#include "atpconfig.h"
/* DTS2013021908159 ʹ���ֶ�����,�����Գ���ҳ�泤ʱ���޷�������� z00185914 20130225 END> */ 

/*--------------------------------------------------------
        ��ʶ��ǰAT���������
---------------------------------------------------------*/
#define AT_CMD_TYPE_CHECK        0         //��ѯ����
#define AT_CMD_TYPE_EXECUTE      1        //ִ������
#define AT_CMD_TYPE_TEST           2         //��������
#define AT_CMD_TYPE_OTHER         3        //��������

/*--------------------------------------------------------
        ���ص�AT������ͨ��ʲô��ʽ�·���
---------------------------------------------------------*/
#define SEND_WEB_DATA            0        //����AT����Ĵ����·���
#define SEND_WEB_ATCMD           1        //ת��AT�����·���

/* <DTS2013050301791 z81004143 2013/5/6 begin */
/* ����at����ִ����ط���ֵ */
#define U_SUCCESS   0
#define U_FAIL      -1
/* DTS2013050301791 z81004143 2013/5/6 end> */

/*--------------------------------------------------------
        ��ʶ��ǰAT�����ִ�н��
---------------------------------------------------------*/
/* <DTS2013042408549 z81004485 2013/4/25 begin  */
//����AT������ķ��ؽ��,������ʾ����ִ�еĽ��,
//#define AT_CMD_RESULT_ERROR     -1      //ִ��ʧ��      
//#define AT_CMD_RESULT_OK         0      //ִ�гɹ�
//AT_AMSS_RES��ʾ����ģ���AT����ķ��ؽ���ļ���,
//��������ģ��ķ��ؽ���϶�,������ֻ�������¼���.
typedef enum
{
    AT_CMD_RES_ERR = -3,//ERROR,�����͵�ERROR
    AT_CMD_RES_SUCCESS = -2,  //ok
    AT_CMD_RES_BUSY = -1,  //����cme cms error�ļ���,
    //ǰ����Ϊ·��ģ���Զ��岢ʹ�õĴ����.
    AT_CMD_RES_SIM_NOT_INSERT = 10, /* {"10",       "SIM not inserted"}, */
    AT_CMD_RES_PIN_REQ = 11,        /* ���ǹ��ĵ�PIN����. */
    AT_CMD_RES_PUK_REQ = 12,
    AT_CMD_RES_SIM_FAIL = 13,       /* {"13",       "SIM failure"}, */    
    AT_CMD_RES_SIM_BUSY = 14,       /* {"14",       "SIM busy"}, */
    AT_CMD_RES_SMS_FULL = 15,       /* DTS2012122503309 j81004134 2012/12/27 */
    AT_CMD_RES_AMSS_MAX = 65535//���ֵ.
}AT_AMSS_RES;
/* DTS2013042408549 z81004485 2013/4/25 end> */

#define  AT_IF_BASE_TIMEOUT  30

/* <DTS2013021908159 ʹ���ֶ�����,�����Գ���ҳ�泤ʱ���޷�������� z00185914 20130225 BEGIN */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
#define  AT_IF_MAX_MSGQUEUE_TIMEOUT  300	/*  ��˼ƽ̨������ʱ����Ҫ��ô��  */
#else
#define  AT_IF_MAX_MSGQUEUE_TIMEOUT  120
#endif
/* DTS2013021908159 ʹ���ֶ�����,�����Գ���ҳ�泤ʱ���޷�������� z00185914 20130225 END> */ 

#define AT_REP_TIMEOUT   1     //�����ϱ��ĵȴ�ʱ��.

#define AT_PARAM_LEN             100      //AT�����������󳤶�
#define AT_MAX_NUM_PARAM         100      //AT��������������
/*<DTS2011060304054 cell info zhaiqi.z00174311 20110425 begin*/
/*<signal info  l129990 20101127 add begin*/
#define CellInfo_CellID_ParaLength  64       //CellInfo��cellid �������̶�
/*signal info l129990 20101127 add end>*/
/*DTS2011060304054 cell info zhaiqi.z00174311 20110425 end>*/

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
/*AT+CPAS��ִ�н��*/
#define PHONE_ACT_STATUS_READY   "0"
#define PHONE_ACT_STATUS_RINGING "3"
#define PHONE_ACT_STATUS_BUSY    "4"

/* SMS AT�������������ַ�������*/
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
/* CTZUƥ���ַ� */
#define CTZU_MATCH_STR      "^CTZU:"
/* ��¼���߲��ʱ���ļ� */
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

/* <DTS2012072605581 liukai 20120806 ��Advanced Settings���޷��л�Clip Mode begin */
#define CLCC_MODE 3
#define VOIICE_SERVER 0
#define DATA_SERVER   1
#define FAX_SERVER    2
#define UNKNOW_SERVER 9
/* DTS2012072605581 liukai 20120806 ��Advanced Settings���޷��л�Clip Mode end> */

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


/*AT^SYSCFG�������ķ��ؽ��*/
typedef struct NetConfigurationResStruct
{
    int           CmdType;             //AT���������
    char          Mode[5];             //ϵͳģʽ�ο�
    char          Acqorder[5];         //����������ο�
    char          Band[64];            //Ƶ��ѡ��
    char          Roam[5];             //����֧��
    char          Srvdomain[5];        //������
    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}NetConfigurationRes;

/*AT^SYSCFGִ�в�������Ĳ���*/
typedef struct  NetConfigurationTypeStruct
{
    char          Mode[5];             //ϵͳģʽ�ο�
    char          Acqorder[5];         //����������ο�
    char          Band[64];            //Ƶ��ѡ�� 
    char          Roam[5];             //����֧��
    char          Srvdomain[5];        //������
    int           Reserve;             //������
}NetConfigurationType;

/* <DTS2013012205684 l00162999 20130122 begin */
/* B880����syscfgex�������� */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
/* AT^SYSCFGEX�������ķ��ؽ�� */
typedef struct __NetCfgExRes
{
    int           CmdType;             // AT���������
    char          Acqorder[5];         // ����������ο�
    char          Band[64];            // Ƶ��ѡ��
    char          Roam[5];             // ����֧��
    char          Srvdomain[5];        // ������
    char          LTEBand[64];          // LTEBand

    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}NetCfgExRes;

/*AT^SYSCFGEXִ�в�������Ĳ���*/
typedef struct  __NetCfgExSet
{
    char          Acqorder[5];         // ����������ο�
    char          Band[64];            // Ƶ��ѡ��
    char          Roam[5];             // ����֧��
    char          Srvdomain[5];        // ������
    char          LTEBand[64];          // LTEBand
    int           Reserve;             //������
}NetCfgExSet;

/* <DTS2013021907887 l00162999 20130314 begin */
/* AT^LOCINFO?���ؽ�� */
typedef struct __LocinfoRes
{
    int           CmdType;             // AT���������
    char          plmn[10];         
    char          lac[10];            
    char          rac[10];             
    char          cell_id[10];        

    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}LocinfoRes;

/* AT^LCELLINFO��������� */
typedef struct __LcellinfoRes
{
    int           CmdType;             // AT���������
    char          Freq[10];         
    char          Band[4];            
    char          CellId[10];             
    char          Rsrp[6];        
    char          Rsrq[6];          
    char          Rssi[6];          

    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}LcellinfoRes;

/*AT^LCELLINFOִ�в�������Ĳ���*/
typedef struct  __LcellinfoSet
{
    char          Acqorder[5];         // ����������ο�
    char          Freq[10];         
    char          Band[4];            
    char          CellId[10];             
    char          Rsrp[6];        
    char          Rsrq[6];          
    char          Rssi[6];          
    int           Reserve;             //������
}LcellinfoSet;

/* AT^CECELLID��������� */
typedef struct __CecellIdRes
{
    int           CmdType;             // AT���������
    char          Plmn[10];         
    char          Ci[10];            
    char          Pci[10];             
    char          Tac[10];        

    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}CecellIdRes;
/* DTS2013021907887 l00162999 20130314 end> */

/* <DTS2013061708346 l00162999 20130620 begin */
/* AT^CPNN��������� */
typedef struct __CPNNRes
{
    int           CmdType;             // AT���������
    char          fname[512];
    int           fname_code;
    int           fname_ci;
    char          sname[512];
    int           sname_code;
    int           sname_ci;

    AT_AMSS_RES           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}CPNNRes;
/* DTS2013061708346 l00162999 20130620 end> */
#endif
/* DTS2013012205684 l00162999 20130122 end> */

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

/* <DTS2012082300362 ChiChao 20120821 begin */
#define MAX_OPERATOR_NUM             20
 /* DTS2012082300362 ChiChao 20120821 end> */
/*AT+COPS=?����������Ӫ����Ϣ������*/
typedef struct OperatorListStruct
{
    int            OperatorNumber; //�ϱ�����Ӫ���ܸ���
    OperatorInfo   Operator[MAX_OPERATOR_NUM];   //�ϱ�����Ӫ���б����֧��20��  /* DTS2012082300362 ChiChao 20120821 */
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
#define SYS_MODE_LTE_ONLY                "38"       /*WCDMA only*/

#define SYS_ACQ_ORDER_AUTO               "0"        /*�Զ�*/
#define SYS_ACQ_ORDER_GSM_FIRST          "1"        /*��GSM��WCDMA*/
#define SYS_ACQ_ORDER_WCDMA_FIRST        "2"        /*��WCDMA��GSM*/
#define SYS_ACQ_ORDER_NO_CHANGE          "3"        /*�ޱ仯*/
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
    AT_AMSS_RES                   AtExeResult;  //ִ�в����ķ��ؽ��
    OperatorList         *TotalOper;    //�����������ص���Ӫ���б�
    RegistedOperator     *CurOperInfo;  //�����ѯ����ص���Ӫ����Ϣ
    int                   Reserve;      //������
}NetChooseOperatorResult;

/*AT+CPINִ�н��*/
typedef struct PinMgResultCodeStruct
{
    int            CmdType;       //�������AT����ķ��ؽ��
    char           Code[20];      //PIN״̬
    AT_AMSS_RES            AtExeResult;   //AT�����ִ�н��
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
    AT_AMSS_RES              AtExeResult;       //AT�����ִ�н��
    int              Reserve;           //������
}PinMgResultCodeEx;

/*<DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 begin*/
#define ROUCFG_MATCH_STR "^ROUCFG"
/*DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 end>*/
//add by w00126555 0315 clip_mode
/*AT^CLIP��������ؽṹ��*/
#define CLIP_MATCH_STR "^CLIP"
#define CLIP_DTMF_MODE  "0"
#define CLIP_FSK_ITU_MODE  "10"
#define CLIP_FSK_BELL_MODE  "11"
#define CLIP_FSK_RPAS_MODE  "12"

/*AT^CLIP:����͸�ʽ*/
typedef struct ClipConfigurationResStruct
{
    int           CmdType;             //AT���������
    char         Mode[8];             //Clipģʽ
    int           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}ClipConfigurationRes;

/*AT^CLIPִ�в����ķ��ؽ��*/
typedef struct ClipConfigurationTypeStruct
{
    char         Mode[8];             //Clipģʽ
    int           Reserve;             //������
}ClipConfigurationType;

//add by w00126555 0315 antenna_mode
/*AT^ANTENNA��������ؽṹ��*/
#define ANTENNA_MATCH_STR "^ANTENNA"
#define ANTENNA_INNER_MODE  "1"
#define ANTENNA_OUTER_MODE  "0"

/*AT^ANTENNA:����͸�ʽ*/
typedef struct AntennaConfigurationResStruct
{
    int           CmdType;             //AT���������
    char         Mode[8];             //Antennaģʽ
    int           AtExeResult;         //AT����ִ�н��
    int           Reserve;             //������
}AntennaConfigurationRes;

/*AT^ANTENNAִ�в����ķ��ؽ��*/
typedef struct AntennaConfigurationTypeStruct
{
    char         Mode[8];             //Antennaģʽ
    int           Reserve;             //������
}AntennaConfigurationType;

/* <DTS2013110606509 c00159151 20131101 begin */
#ifdef SUPPORT_ATP_SIGNAL_LED_CONTROL
#define SIG_LED_OFF_STATUS   "1"     /* led�ر� */
#define SIG_LED_ON_STATUS    "0"     /* led���� */
#define OFFLED_MATCH_STR     "^offled" 

/* ���������źŵ�AT���� AT^offled����ķ��͸�ʽ */
typedef struct
{
    int cmd_type;       /* AT��������� */
    char off_value[8];  /* offled�����ֵ*/
    int at_exec_reslt;  /* AT����ִ�еĽ�� */
    int reserve;        /* ������ */
}off_led_config_t;

typedef struct 
{
    char off_led_value[8];  /* offled�����ֵ */
    int reserve;            /* ������ */
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
    AT_AMSS_RES            AtExeResult;   //AT�����ִ�н��
    char           Status[5];     //SIM��״̬�Ĳ�ѯ���
    int            Reserve;       //������
}PinEnableResult;

/*AT+CPWDִ�н��*/
typedef struct PinModifyResultCodeStruct
{
    int            CmdType;        //��Ӧ����AT�����ִ�н��
    AT_AMSS_RES            AtExeResult;    //AT�����ִ�н��
    int            Reserve;        //������
}PinModifyResultCode;

/*AT+CPAS��ִ�н��*/
typedef struct PhonseActStatusStruct
{
    int            CmdType;        //��Ӧ����AT�����ִ�н��
    AT_AMSS_RES            AtExeResult;    //AT�����ִ�н��
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
    AT_AMSS_RES            AtExeResult;   //AT�����ִ�н��
    int            Reserve;       //������
}StR99Only;
/* AP7D00728.H3G.R99: liudan 2007-8-23 ADD END*/

/* <AP7D010XX.Optus web server: liudan 2007-10-24 ADD BEGIN */
typedef struct CvmnqStruct
{
    int            CmdType;                    /*�������AT����ķ��ؽ��*/
    char           voice_mail_number[25];      //�����������,���֧�ֵ�20���ַ�.
    char           nv_number[25];              //ԭʼ�ĺ���(����NV�ж�ȡ�ĺ���)
    AT_AMSS_RES            AtExeResult;                //AT�����ִ�н��
    int            Reserve;                    //������
}StCvmnq;

/*����״̬�仯ָʾ^SRVST */
typedef struct{
    char  srv_status[3];
} StSrvstInfo;

/*AP7D01278.Optus SMS: liudan 2008-1-26 ADD BEGIN */
/*���������������ϱ�.*/
typedef struct{
    char  mem_type[5];
    AT_AMSS_RES            AtExeResult;                //AT�����ִ�н��

} StSmmemfull;
/* AP7D01278.Optus SMS: liudan 2008-1-26 ADD END*/
/* SIM ״̬�仯ָʾ ^SIMST */
typedef struct{
    char  sim_state[3];
    char  lock_state[3];
} StSimstInfo;

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

typedef struct               /* ϵͳ��Ϣ */
{
    int srv_status;
    int srv_domain;
    int roam_status;
    int sys_mode;
    int sim_state;
    int lock_state;
    int submode;
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
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
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
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

typedef struct
{
    char soft[WM_VERSION_LEN];        /* ���Ȳ�����31���ַ��� */
    char hard[WM_VERSION_LEN];        /* ���Ȳ�����31���ַ��� */
}StAmssVersion;

/* �Ӵ洢�����Ͷ��ź�ķ��ؽ���� */
typedef struct {
    int mr;
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCmssRes;

/* CMSS������������ */
typedef struct {
    int index;
    char da[SMS_NUMBER_LENGTH+3];     /* ���Ž����ߵĺ���"13801001372" ��Ҫ������*/
    int toda;      /* �������� */
    int  Reserve;       //������
}StCmssPara;

/* ������ŵķ��ؽ�� */
typedef struct {
    int  index;
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCmgwRes;

/* ɾ�����ŵķ��ؽ�� OK/+CMS ERROR:err */


/* ɾ�����ŵ������Ĳ��� */
typedef struct {
    int  index;
    int  delflag;
    int  Reserve;       //������
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
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
 AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��

}StCpms;

/*<DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 begin*/
typedef struct{
 char mode[5];
 char acqorder[5];
 char band[15];
 char roam[5];
 char srvdomain[5];
 char clip[5];
 char antenna[5];
 AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
}StRoutcfgs;
/*DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 end>*/

typedef struct{
    at_cmd_result  amss_res;
    dsati_cms_err_e_type  cms_res;
    dsat_cme_error_e_type cme_res;
}StAtRes;

typedef struct{
     AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
}StCmmt;

/*�����Ƿ��Ѷ�ȡ���*/
typedef struct {
    int ReadTag;      //�����Ƿ��Ѷ�ȡ���
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int  Reserve;     //������
}StCmsrRes;

/* <DTS2011052400954.AT: mayang 2010-05-26 MODIFY BEGIN */
#define COMM_STR_LEN_32 32
/*���úͲ�ѯ�������ĺ���*/
typedef struct {
    char sca[COMM_STR_LEN_32];
    char tosca[COMM_STR_LEN_32];
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
}StCsca;
/* DTS2011052400954.AT: mayang 2010-05-26 MODIFY END>*/

typedef struct ScidStruct
{
    char           Code[21];      //AT^HSDPA״̬
    AT_AMSS_RES            AtExeResult;   //AT�����ִ�н��
}StScid;

/*���úͲ�ѯ�������ĺ���*/
typedef struct {
    int call_ID;
    char cDTMFEvent;
    int on_length;
    int off_length;
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int  Reserve;       //������
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
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
}PnnRes;

/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */
/* ����ҵ�� +CUSD */
#ifdef SUPPORT_ATP_USSD
/* <DTS2011092600505 shidai 20110926 Modify Begin */
#define AT_IF_INFO          "/var/at/at_if.log"
#define STR_CUSD_RESULT_LEN  (512)   /* ���ߴ������ĵ���󳤶�Ϊ 512 */
#define CUSD_TYPE_LEN       (4)

/*  USSD ���뷽ʽ */
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
/* <DTS2012072605581 liukai 20120806 ��Advanced Settings���޷��л�Clip Mode begin */
int AT_Clcc();
/* DTS2012072605581 liukai 20120806 ��Advanced Settings���޷��л�Clip Mode end> */
int AT_SysInfoForXml(StSysinfo *Result,char *at_report_str);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */


/* <DTS2012101508427 z81004485 2012/10/12 begin  */
int AT_CFUN(StCFUN *Result);
int AT_MMGMM(StMMGMM *Result);
int AT_CGREG(StCGREG *Result);
int AT_CellInfo(CellInfoResType *Result);
/* DTS2012101508427 z81004485 2012/10/12 end> */


/*<DTS2010051200134 l00101002 20100517 Add begin*/
typedef struct               /* ϵͳ��Ϣ */
{
    int version;
    int pin_correct;
    int iccid_correct;
    int ecc_check;
    int lock_style;
    int total_num;
    int num_been_locked;
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
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
/* ������� */
int AT_Cmgw (StCmgsInfo* sms_pdu, StCmgwRes* cmgw_result);
/* ��sm�з��Ͷ��� */
int AT_Cmss (StCmssPara* cmss_para, StCmssRes* cmss_result);/*+CMSS*/
/* ɾ�� */
int AT_Cmgd (int cmd_type, StCmgdPara* cmgd_para);

/* ATSʹ�ô˺�������ɳ�ʼ�� */
int AT_Cmsr (int cmd_type, int ReadTag, StCmsrRes* cmsr_res);
/*  */
int AT_Csca( int cmd_type, StCsca* csca_para, StCsca* csca_res);

/* ��ȡ����״̬�µĶ���index���б� 
ATSʹ�ô˺��������������ȡ
HTTPDʹ�ô˺�������ѯ�Ƿ���δ������*/
int AT_Cmgi (int state, int* sms_list,int list_size, int* list_length);
/* Optus SMS: liudan 2008-1-15 ADD BEGIN*/
int AT_Cmgi_unread_sms (int* sms_list,unsigned int list_size, int* list_length);
/* Optus SMS: liudan 2008-1-15 ADD END*/
/* �ֶ��޸Ķ���״̬ */
int AT_Cmmt (int index, int state,StCmmt *result);
/* ��ѯsm����֧�ֶ�����, ���ж�����,*/
int AT_Cpms(int cmd_type, StCpms* cpms_para, StCpms* cpms_result);
/*AP7D01278.Optus SMS: liudan 2008-1-26 ADD BEGIN */
/*<DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 begin*/
int AT_Roucfg(int cmd_type, char* roucfg_para, StRoutcfgs* roucfg_res);
/*DTS2010051300332:·�ɲ��ܵ���clip���� by l129990,2010-7-2 end>*/
/*��ѯ�����ϱ�.*/
int AT_smmemfull(StSmmemfull *sm_full_res);
int AT_Rssi(StRssiInfo *rssi_res);
int AT_Hwver(StAmssVersion* ver_res);
int AT_Dsflowrpt(StDsflowrptInfo *flow_res);
int AT_Cgmr(StAmssVersion* ver_res);
/* AP7D01278.Optus SMS: liudan 2008-1-26 ADD END*/
/*����cops��Ϣ*/
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

//add by w00126555 0315 clip_mode��������
int AT_ClipConfig(int CmdType, ClipConfigurationType *Param, ClipConfigurationRes *Result);
int AT_AntennaConfig(int CmdType, 	AntennaConfigurationType *Param, AntennaConfigurationRes *Result);
/*<AP7D04527  w00126555 20100408 add begin*/
int AT_cnum(StCnumInfo *Result);
/*AP7D04527  w00126555 20100408 add end>*/

/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */
#ifdef SUPPORT_ATP_USSD
/*����AT+CUSD����*/
int AT_Cusd_Msg_Send (int CmdType, char *rspType, char *cusdStr, char *codeType);
/*��ѯ����ҵ�������ϱ����*/
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
/* <DTS2012081504862 liukai 20120816 ·�ɶ��������ʱ��ģʽ begin */
typedef struct
{
    int           mode;      
    int           timezone;
    AT_AMSS_RES   AtExeResult;   //AT�����ִ�н��
}CtzuConfigurationType;

/* DTS2013061803625 z81004485 Coverity 2013/6/18 */
/* DTS2012081504862 liukai 20120816 ·�ɶ��������ʱ��ģʽ end> */

int AT_CheckSmsReady(int *status); /* DTS2012082301238 j81004134 2012/8/23 */
/* <DTS2012110602724 l00162999 20121117 begin */
/* <DTS2013101501273 z81004485 2013/10/15 delete 1 line> */
typedef struct
{
    int     simOptType;
    char    simlockpwd[9];
    int     OptState;
    int     remaintimes;  
    AT_AMSS_RES  AtExeResult;   //AT�����ִ�н��
    int     Reserve;       //������
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
0	�ر�״̬��
1	����״̬�ƣ���ʱ״̬Ϊ�ر�֮ǰ��״̬
2	���ųɹ�
3	����ʧ�ܣ�ע�⣺����δ���Ϻ�
4	�ֶ�����ģʽ�£�·��������ʱ��֪ͨ��������
5	�Ͽ�����
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
