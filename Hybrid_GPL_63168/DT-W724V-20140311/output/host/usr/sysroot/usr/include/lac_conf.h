
#ifndef _LAC_CONF_H_
#define _LAC_CONF_H_

#include <arpa/inet.h>

#define SIG_WAIT_XL2TP	(SIGRTMIN + 10)
#define L2TP_MYTYPE		10
#define PPTP_MYTYPE     11

#ifndef bool
#define bool int
#endif

#define U_FAIL                  -1

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

/********************************************
 * LAC ����ģʽ
 ********************************************/
enum Dialling_Mode
{
	LAC_DIALLING,										/* ������ - ��һ�ַ��� */
	PC_DIALLING,										/* �û����� - �ڶ��ַ��� */
};

/********************************************
 * ��֤ģʽ
 ********************************************/
#define AUTHTICATION_NO         0
#define AUTHTICATION_CHAP       1
#define AUTHTICATION_PAP        2
#define AUTHTICATION_MS_CHAP    4
#define AUTHTICATION_MS_CHAP_V2 8


/********************************************
 * ����ģʽ
 ********************************************/
#define ENCRYPTION_NULL         0
#define ENCRYPTION_MPPE_40      1
#define ENCRYPTION_MPPE_128     2

#define AUTH_TYPE_ID_LEN 4

#define PPP_PID_STR_LEN  8
/********************************************
 * LAC ���ýṹ
 ********************************************/
struct LAC_Config
{
	/* name: l2tp_avp_hidden */
	int							avp_hidden;			/* avp��������		1: ����, 0: ������, Ĭ��Ϊ: 0 */
	/* name: l2tp_lns_address */
    struct in_addr  			lns_address;			/* lns �Ĺ���IP */
	/* name: l2tp_dialling_mode */
    enum Dialling_Mode			dialling_mode;			/* ����ģʽ */
	/* name: l2tp_user_max */
	int							user_max;				/* Ĭ��: 5 */
	/* name: l2tp_user_name */
	char						user_name[128];		/* LAC �����û��� */
	/* name: l2tp_password */
	char						password[128];			/* LAC �������� */
	/* name: l2tp_authtication */
	int		                    authtication;			/* ��֤ģʽ */
	/* name: l2tp_encryption */
	int                     	encryption;			/* ����ģʽ */
	/* name: l2tp_tunnel_keep_alive_time */
	int							tunnel_keep_alive_time;		/* �������ʱ��				Ĭ��: 60s */
	/* name: l2tp_tunnel_session_max */
	int							tunnel_session_max;			/* ���֧�ֵ����Ự����		Ĭ��: 5 */
    /* name: l2tp_connect_type */
    int                         connect_type;       /* LAC ���ŷ�ʽ, �ֹ����Զ� */
};
typedef struct LAC_Config l2tp_lac_conf;


/* add by stone 2008-8-25 begin */
/********************************************
 * PAC ���ýṹ
 ********************************************/
struct PAC_Config
{
	/* name: pptp_lns_address */
      struct in_addr  			pns_address;			/* pns �Ĺ���IP */
	/* name: pptp_user_name */
	char						user_name[128];		/* PAC �����û��� */
	/* name: pptp_password */
	char						password[128];			/* PAC �������� */
	/* name: pptp_authtication */
	int                 		authtication;			/* ��֤ģʽ */
	/* name: pptp_encryption */
	int                     	encryption;			/* ����ģʽ */
	/* name: pptp_tunnel_keep_alive_time */
	int							tunnel_keep_alive_time;		/* �������ʱ��				Ĭ��: 60s */
	/* name: pptp_tunnel_session_max */
	int							tunnel_session_max;			/* ���֧�ֵ����Ự����		Ĭ��: 5 */
    /* name: pptp_connect_type */
    int                         connect_type;       /* PAC ���ŷ�ʽ, �ֹ����Զ� */
};
typedef struct PAC_Config pptp_pac_conf;



#define CON_MANUAL 1   /* �ֹ�*/
#define CON_WAN_UP 2   /* �Զ� */

extern bool get_l2tp_lac_conf(l2tp_lac_conf*lac);
extern bool get_pptp_lac_conf(pptp_pac_conf*pac);

/********************************************
 * �û�����
 ********************************************/
enum User_Info_Type
{
	L2TP,
	PPPT,
};

/********************************************
 * User Info ��Ϣ�ṹ
 ********************************************/
struct User_Info
{
	char								user_name[128];		/* �û��� */
	char								password[128];			/* ���� */
	int                 				authtication;			/* ��֤ģʽ */
	enum User_Info_Type					user_type;				/* �û����� */
};
typedef struct User_Info l2tp_user_info;

extern bool get_l2tp_user_info(int which, l2tp_user_info*user);

/********************************************
 * ���״̬
 ********************************************/
enum TUNNEL_State
{
	TUNNEL_IDLE,
	TUNNEL_WAIT_CTL_REPLY,
	TUNNEL_WAIT_CTL_CONN,
	TUNNEL_ESTABLISHED,

};

enum PPTP_TUNNEL_State
{
	PPTP_TUNNEL_IDLE,
	PPTP_TUNNEL_WAIT_CTL_REPLY,
	PPTP_TUNNEL_WAIT_STOP_REPLY,
	PPTP_TUNNEL_ESTABLISHED,

};

/********************************************
 * �Ự״̬
 ********************************************/
enum SESSION_State
{
	SESSION_IDLE,
	SESSION_WAIT_TUNNEL,
	SESSION_WAIT_REPLYY,
	SESSION_WAIT_CONNECT,
	SESSION_ESTABLISHED,

};

enum PPTP_SESSION_State
{
	PPTP_SESSION_IDLE,
	PPTP_SESSION_WAIT_REPLYY,
	PPTP_SESSION_ESTABLISHED,
	PPTP_SESSION_WAIT_CS_ANS,

};

/********************************************
 * SESSION ��Ϣ�ṹ
 ********************************************/
struct SESSION_Info
{
	int						session_ID;
	enum SESSION_State		session_state;			/* �Ự״̬ */
	int						connected_time;		/* �Ự����ʱ�� */
	int						call_serial_number;	/* �������к� */
	struct in_addr			local_address;			/* ���б��ص�ַ */
	struct in_addr			remote_address;		/*  */
};

/********************************************
 * TUNNEL ��Ϣ�ṹ
 ********************************************/
struct TUNNEL_Info
{
	int						tunnel_ID;
	enum TUNNEL_State		tunnel_state;		/* ���״̬ */
	int						connection_time;	/* �������ʱ�� */
	int						session_number;	/* ����Ự���� */
	struct SESSION_Info		session_info_list[0];
};
typedef struct TUNNEL_Info l2tp_tunnel_info;


struct PPTP_SESSION_Info
{
	int						session_ID[2];
	int                     connected_time;
	enum PPTP_SESSION_State		session_state;			/* �Ự״̬ */
	struct in_addr			local_address;			/* ���б��ص�ַ */
	struct in_addr			remote_address;		/*  */
};

struct PPTP_TUNNEL_Info
{
	enum PPTP_TUNNEL_State		tunnel_state;		/* ���״̬ */
	int						    session_number;	/* ����Ự����*/
    int						    call_serial_number;
	struct PPTP_SESSION_Info	session_info_list[0];
};

struct PPTP_MSG
{
    long                        mytype;
    struct PPTP_TUNNEL_Info     tunnel_info;
};

/* ��l2tp����Ϣ�ṹ */
struct pppl2tp_event_msg {
    uint16_t    msg_type;           /* PPPL2P_MSG_LINK_UP, PPPL2TP_MSG_LINK_DOWN */  
    uint16_t    msg_len;           /* length of data that follows */
    uint8_t        msg_data[0];       /* list of TLVs, each always longword aligned */
};

struct pppl2tp_event_tlv {
    uint16_t    tlv_type;        /* macro listed in PPPL2TP_TLV... */
    uint16_t    tlv_len;
    uint8_t        tlv_value[0];    /* TLV value */
};

extern bool get_l2tp_tunnel_info(char* tunnel_info_buffer, int buffer_len);
extern bool set_l2tp_tunnel(int nUpDownInfo);
extern bool get_pptp_tunnel_info(char* tunnel_info_buffer, int buffer_len);
bool web_start_pptp(void);
bool web_stop_pptp(void);

#define L2TP_TUNNEL_UP 1
#define L2TP_TUNNEL_DOWN 2


struct l2tp_msg
{
	long					mytype;
	struct TUNNEL_Info		tunnel_info;
};

#define OPENL2TP_EVENT_SOCKET_NAME        "/var/pppl2tp-event.sock"

#define L2TP_SESSION_ID_FILE               "/var/l2tp/session.id"

/*ʹ�ýϴ��ppp����,�����������WAN��ͻ,*/
/*���300�ĵĸ���,���ر���cms ��xl2tpd���*/
#define L2TP_PPP_IFC_NAME               "ppp300"

#define L2TP_PPP_CONFIG_FILE               "/var/wan/%s/config"
#define L2TP_PPP_DIAL_FILE               "/var/wan/%s/dial"
#define L2TP_PPP_STATUS_FILE          "/var/wan/%s/status"
#define L2TP_PPP_PID_FILE               "/var/wan/%s/pid"
#define L2TP_PPP_IP_FILE               "/var/wan/%s/ipaddr"
#define L2TP_PPP_GW_FILE               "/var/wan/%s/gateway"
#define L2TP_PPP_DNS_FILE               "/var/wan/%s/dns"
#define L2TP_PPP_ERR_FILE               "/var/wan/%s/err"

/* gongqing 2008-07-24 add begin */
#define MAX_MSG_LEN 20480
#define LOG_SIZE                   1024*1024

void  _syslog(char* logfile, int level, char *format, ...);

/*xl2tpd, pppsd, rp-pppoe debug ����*/
/*#define DEBUG_L2TP*/

#ifdef DEBUG_L2TP
#define _DEBUG(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 0, message);\
}while(0)

#define _INFO(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 1, message);\
}while(0)

#define _WARN(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 2, message);\
}while(0)

#define _HIGH(logfile, format, arg...)    do{ \
   char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 3, message);\
}while(0)

#define _ERROR(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 4, message);\
}while(0)

#define _FATAL(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 5, message);\
}while(0)

#define _print(format,arg...)    do{\
    fprintf(stderr, "[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__,##arg);\
}while(0)
/* gongqing 2008-07-24 add end */
#else
#define _DEBUG(logfile, format, arg...)   
#define _INFO(logfile, format, arg...)    
#define _WARN(logfile, format, arg...)    
#define _HIGH(logfile, format, arg...)   
#define _ERROR(logfile, format, arg...)   
#define _FATAL(logfile, format, arg...)  
#define _print(format,arg...)   
#endif

#endif /* _LAC_CONF_H_ */

