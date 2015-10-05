/***********************************************************************
  ��Ȩ��Ϣ : ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���   : at_server.h
  ����     : chenjindong chenkeyun
  �汾     : 
  �������� : 2007-3-20
  ������� : 
  �������� : ͷ�ļ�
      
  ��Ҫ�����б�: ��
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    c54530       20070320   0.0.1    ��ʼ���
  ��ע: 
************************************************************************/
#ifndef __ATSERVER_H
#define __ATSERVER_H

#include "at_utils_fmc.h"
#include "at_if_fmc.h"
#include "atpconfig.h"
#ifdef SUPPORT_ATP_SMS
#include <sys/time.h>
#endif

#define AT_STATE_SIZE           32      /* size of AT state queue */
#define AT_STATE_MASK         0x1f    /* mask for AT state queue  */
#define AT_CLIENTS_SIZE       32      /* size of AT clients array */

/*Begin PN DTS2012020701548 �����������ߵ�ʱ��,Modify by z00185914 20120207*/
#define WM_START_TIME         30  // 30��
/*End PN DTS2012020701548 �����������ߵ�ʱ��,Modify by z00185914 20120207*/

#define SECOND                    1000
#define ATS_TIMER_VAL              1*SECOND
#define AT_TIMEOUT_VAL            7

#ifdef SUPPORT_TD_CARD  //֧�� TD ET128
#define AT_HANDSHAKE_VAL       10
#else
/*Added by lvxin00135113@20090907 �ӳ�Atserver�����ݿ�����ʱ���ӳ���10s*/
/*<AP7D04642 w00126555 20100513  modify begin*/
#define AT_HANDSHAKE_VAL       5
//#define AT_HANDSHAKE_VAL       10
/*AP7D04642  w00126555 20100513  modify end>*/
/*Added by lvxin00135113@20090907 �ӳ�Atserver�����ݿ�����ʱ���ӳ���10s*/
#endif

#define ATS_TIME_VIR_VAL        7

#define USR_SBIN_ATS          "/bin/ats"


#ifdef SUPPORT_TD_CARD  //֧�� TD ET128
#define USB_PCUI                 "/dev/ttyUSB4"
#define USB_DIAG                 "/dev/ttyUSB1"             //DIAG ���豸�ڵ�
#define USB_MODEM              "/dev/ttyUSB2"             //MODEM_PORT�˿��豸�ڵ�
#else
#define USB_PCUI                 "/dev/ttyUSB2"
#define USB_DIAG                 "/dev/ttyUSB1"             //DIAG ���豸�ڵ�
#define USB_MODEM              "/dev/ttyUSB0"             //MODEM_PORT�˿��豸�ڵ�
#endif
/*Start of added by lvxin00135113@20090806 for support unknow pid datacard*/
#define PROC_USER_UMTS      "/proc/proc_user_umts"
#define ATS_PROC_PID                   "/proc/proc_user_atserver.pid"
/*End of added by lvxin00135113@20090806 for support unknow pid datacard*/
#define ATS_PID                   "/var/at/atsrv.pid"
#define ATS_LOG                   "/var/at/atsrv.log"
/*AP7D01238.Optus SMS: liudan 2008-1-22 ADD BEGIN*/
#define PPP_PID                   "/var/at/pppd.pid"
/*AP7D01238.Optus SMS: liudan 2008-1-22 ADD END*/

/*<AP7D04527  w00126555 20100408 add begin*/
#define ATC_SMS_PID                   "/var/at/atcsms.pid"
/*AP7D04527  w00126555 20100408 add end>*/
/*<DTS2010060400396:���Ӷ��Ź��ܿ��� by l129990,2010-6-5 begin*/
#define SYSCTL_SMS_CONF_PATH     "/var/sysctl/SMSCtl.conf"
/*DTS2010060400396:���Ӷ��Ź��ܿ��� by l129990,2010-6-5 end>*/
#define ATS_REP_CMD                "REP_CMD"
#define AT_TEMP_SIZE               20
#define LOOPBUFSIZE       2048

#define READ_SIZE                        512

#ifdef SUPPORT_TD_CARD  //֧�� TD ET128
#define ATS_CMD_BASE_TIMEOUT    15
#else
#define ATS_CMD_BASE_TIMEOUT    30
#endif
// ��ʱ������
typedef struct 
{
    struct sigaction new_sigalrm;
    struct sigaction old_sigalrm;
    struct itimerval new_timer;
    struct itimerval old_timer;
} AT_TIMER_S;

/* AT Server ״̬ö��*/
typedef enum{
    AT_INIT_S = 0,            /*��ʼ̬*/
    AT_IDLE_S,            /*idle̬*/
    AT_PROCESS_S,        /*����̬������at����*/
    AT_HANDSHAKE_S,        /*��ʱ̬������ʱ����*/
    AT_RESET_S,            /*reset̬����Ҫ����AMSS*/
    AT_EXIT_S,            /*�˳�����̬*/
    AT_STATE_NUM
}at_maj_type;


/* AT�¼� */
typedef enum
{
    AT_NULL_E = 1,            /*���¼�*/
    AT_REQ_E,            /*�����¼�*/
    /* <DTS2012030904901,wanglong added begin,2012-3-21 */
    AT_RESETWM_E,         /* ���������¼� */
    /* DTS2012030904901,wanglong added end,2012-3-21> */
    AT_TIME_E                   /*��ʱ�¼�*/
}at_event_e;
/*<AP7D01374    chenkeyun    2008-02-25    mod    begin*/

typedef enum
{
    FIND_CR_CF = 0,            /*����\r\n*/
    FIND_CR,                      /*����\r*/
    NO_FIND_CR_CF          /*û�з���\r\n ����\r*/     
}at_found_param;
/*AP7D01374    chenkeyun    2008-02-25    mod    end  >*/

typedef enum 
{
    AT_RET_OK,
    AT_RET_ERROR,
    
}AT_RET_CODE;

typedef enum 
{
    NO_FOUND,
    FOUND
}is_find;


/*pcui��ȡ״̬ ö�� */
enum read_state_enum
{
    read_usb,       /*��״̬*/
    parse_recData,  /*����״̬*/
    find_result,    /*��ȡ��ȷֵ״̬*/
};

#define HSPA_AT_DIAL            55       //HSPA����꣬�������
#define HSPA_AT_DIAG_PORT_SET   56       //HSPA����꣬DIAG���л�����
#define HSPA_AT_ACCEPT          57       //HSPA����꣬����
#define HSPA_AT_CHUP            58       //HSPA����꣬���йҶ�
#define HSPA_AT_CALL_TIME_QUERY  62      //HSPA����꣬ͨ��ʱ����ѯ
#define HSPA_AT_AUTO_SIM_STATUS_REPORT 63  //HSPA����꣬SIM ״̬�仯ָʾ
#define HSPA_AT_RING_REPORT     64       //HSPA����꣬��������ָʾ
#define HSPA_AT_CALL_NUM_REPORT  65      //HSPA����꣬���������ʾ
#define HSPA_AT_CALL_END_REPORT  66      //HSPA����꣬ͨ������ָʾ
#define HSPA_AT_CALL_ORIG_REPORT  67      //HSPA����꣬�������ָʾ
#define HSPA_AT_CALL_CONF_REPORT  68      //HSPA����꣬������ָͨʾ
#define HSPA_AT_CONN_REPORT      71      //HSPA����ָ꣬ʾ���н�ͨ
#define HSPA_AT_CMD_MAX          100     //HSPA�����ֵ����

#define AT_MODULE 1

#define MSG_AT_QUEUE  100
#define MSG_VOICE_QUEUE  101
#define MSG_MODEM_QUEUE  102
#define MSG_TR069_QUEUE  103
#define MSG_HTTP_QUEUE  104
#define MSG_LED_QUEUE  105
#define MSG_CMD_QUEUE  106
#define AT_RCV_MSG_LENGTH  252   //������Ϣ����������
/*<mmi l129990 20101117 add begin*/
#define MMI_CMD_LEN  256
#define MMI_FLAG_FILE    "/var/mmiflag"
/*mmi l129990 20101117 add end>*/

struct stAtRcvMsg g_stAtToOther;
struct stAtRcvMsg
{
    long int lMsgType; 
    char acText[AT_RCV_MSG_LENGTH];
};


#define    HSPA_FROM_POSITION 0
#define    HSPA_RESULT_POSITION 1
#define    HSPA_OK    0


typedef enum
{
    AT_CMD_COPS_NUM_NAME = 0,
    AT_CMD_GET_SCID,
    AT_CMD_GET_IMSI,
    AT_CMD_AUTO_VALIDATE_PIN,
    AT_CMD_GET_OPL,
    AT_CMD_GET_PNN
}ATS_ONCE_CMD_FLAG_POS;

/*Start of added for support STC hspa by lvxin00135113@20090812*/
#define  ATS_HSPA_VENDOR_PROC                           "/proc/UsbModem"
#define  ATS_HSPA_VENDOR_STR                             "Vendor"
#define  ATS_HSPA_PRODUCT_VENDOR_HW             "Huawei"
#define  ATS_HSPA_PRODUCT_VENDOR_BANDLUXE   "Bandluxe"
#define  ATS_HSPA_PRODUCT_VENDOR_ZTE             "ZTE"

typedef enum
{
    AT_HSPA_VENDOR_HW = 0,
    AT_HSPA_VENDOR_BANDLUXE,
    AT_HSPA_VENDOR_ZTE,

    AT_HSPA_VENDOR_MAX
}AT_HSPA_VENDOR_EN;


/*End of added for support STC hspa by lvxin00135113@20090812*/

void at_timer_callback(int arg);
unsigned int get_ats_report_seq();

void at_client_handle(int sig);

int check_ats_ready();

void at_idle(void);
int at_handshake(int* retCode);
at_event_e at_event_read( void );
int  at_event_write(at_event_e event);
int  is_in_at_event( at_event_e event );
int  at_handshake_start(void) ;
void at_handshake_callback(int arg);
void at_setup_sig(void);
void at_server_hup(int sig);
void at_server_term(int sig);
void at_server_chld(int signum);
void at_server_bad_signal(int sig);
void at_server_sgv_signal(int sig);
void at_server_bus_signal(int sig);

void at_server_debug(int signum);

void at_create_message_queue(void);
void at_wm_reset();
int ats_cmee(int *retCode);
int parse_sysinfo(StSysinfo* sysinfo);
int parse_rssi (char* report);
int at_sm_report_handler_csq (rsp_msg* rssi_res);
int get_cpms(StCpms* cpms_res);
int get_sms_mem(rsp_msg* res_msg);
/*Start of added by lvxin00135113@20090806 for support unknow pid datacard*/

void at_server_parse_unknow_pid_probe(int signum);
void at_init_unknow_pid_parms(void);
int at_probe_open_port(const char* cmd_path, int* open_port_fd);
 /* DTS2013071105737 z81004485 2013/7/11, Coverity delete 2 lines */
/* <DTS2013061500878 j81004134 2013/6/15 begin */
int read_config_file(const char *pcFileName, char *acBuf, unsigned int ulBufLen);

typedef struct
{
    char mode[8];
    char acqorder[8];
    char band[64];
    char roam[8];
    char srvdomain[8];
}syscfg_info;

enum SYSCFG_VALUE
{
    SYSCFG_MODE = 0,
    SYSCFG_ACQORDER,
    SYSCFG_BAND,
    SYSCFG_ROAM,
    SYSCFG_SRVDOMAIN
};
/* DTS2013061500878 j81004134 2013/6/15 end> */
/* <DTS2013072407951 l00162999 20130731 begin */
#ifdef SUPPORT_ATP_B880_CUBE
int ats_query_cpnn(void);
#endif
/* DTS2013072407951 l00162999 20130731 end> */

/*End of added by lvxin00135113@20090806 for support unknow pid datacard*/
#endif 
