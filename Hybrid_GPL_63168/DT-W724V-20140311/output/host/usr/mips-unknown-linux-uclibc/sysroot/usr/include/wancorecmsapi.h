#ifndef __ATP_WANCORECMS_API_H__
#define __ATP_WANCORECMS_API_H__

#include "atputil.h"
#include "atpwancore.h"


/* ǰ׺�������ֵ */
#define WAN_PREFIX_LEN_MAX 128
/* ǰ׺������Сֵ */
#define WAN_PREFIX_LEN_MIN 16
/* ǰ׺����Ĭ��ֵ */
#define WAN_PREFIX_LEN_DEFAULT 64
/* ǰ׺ʱ��Ĭ��ֵ */
#define WAN_PREFIX_TIME_DEFAULT (60*60)

/* WAN·����󳤶� */
#define ATP_WAN_PATH_LEN 128

#ifdef SUPPORT_ATP_RTU
#define ATP_WAN_IFC_LEN  16
#else
/* WAN�ӿ�����󳤶� */
#define ATP_WAN_IFC_LEN  32
#endif

/* WAN�ӿ�IP��ַ��󳤶� */
#define ATP_WAN_ADDR_LEN 16
/* PPPoE Server��ַ */
#define ATP_PPPOE_SERVER_ADDR  "192.168.239.1"

#ifdef SUPPORT_ATP_RTU
#define ATP_WAN_MAC_LEN  20
#else
/* MAC��ַ��󳤶� */
#define ATP_WAN_MAC_LEN    32
#endif

/* ��·��������󳤶� */
#define ATP_WAN_LINK_NAME_LEN  32
/* �ַ������볤�� */
#define ATP_WAN_PAD  4

/* WAN���ֳ��� */
#ifdef SUPPORT_ATP_RTU
#define ATP_WAN_NAME_LEN  (32 + ATP_WAN_PAD)
#else
#define ATP_WAN_NAME_LEN  (256 + ATP_WAN_PAD)
#endif

#ifdef SUPPORT_ATP_RTU
#define ATP_WAN_STATUS_LEN 20
#define ATP_WAN_LASTERR_LEN 28
#else
/* WAN״̬���� */
#define ATP_WAN_STATUS_LEN 32
/* WAN���Ӵ�����Ϣ���� */
#define ATP_WAN_LASTERR_LEN 32
#endif
/* WAN�������ͳ��� */
#define ATP_WAN_CONNTYPE_LEN 16
/* WAN���ӵ�ַ���ͳ��� */
#define ATP_WAN_ADDRTYPE_LEN 8
/*WAN���ӵ�������ͳ���*/
#define ATP_WAN_TUNNEL_TYPE_LEN 8
/*6rd ���ģʽ����������*/
#define ATP_WAN_6RD_MODE_LEN	8				
/* IP��ַ���� */
#define ATP_WAN_IPADDR_LEN 16
/* DNS�б��� */
#define ATP_WAN_DNS_LEN (64 + ATP_WAN_PAD)
#ifdef SUPPORT_ATP_DOT1X
#define ATP_WAN_DOT1X_TYPE_LEN  24
#define ATP_WAN_DOT1X_NAME_LEN  64
#define ATP_WAN_DOT1X_PASSWORD_LEN  64
#endif

#ifdef SUPPORT_ATP_RTU
#define ATP_WAN_PPP_USER_LEN (32 + ATP_WAN_PAD)
#define ATP_WAN_PPP_PASSWD_LEN (32 + ATP_WAN_PAD)
#define ATP_WAN_PPP_AUTH_LEN 6
#define ATP_WAN_TRANSPORT_LEN 8
#define ATP_WAN_PPP_ACNAME_LEN (32 + ATP_WAN_PAD)
#define ATP_WAN_PPP_SRVNAME_LEN (32 + ATP_WAN_PAD)
#define ATP_WAN_BIND_LEN 256
#else
/* PPP�û������� */
#define ATP_WAN_PPP_USER_LEN (64 + ATP_WAN_PAD)
/* PPP���볤�� */
#define ATP_WAN_PPP_PASSWD_LEN (64 + ATP_WAN_PAD)
/* PPP��֤Э�鳤�� */
#define ATP_WAN_PPP_AUTH_LEN 8
/* ����������PPPOE/PPPOA/PPP */
#define ATP_WAN_TRANSPORT_LEN 16
/* PPPOE���ʼ��������ֳ��� */
#define ATP_WAN_PPP_ACNAME_LEN (256 + ATP_WAN_PAD)
/* PPPOE�������ֳ��� */
#define ATP_WAN_PPP_SRVNAME_LEN (256 + ATP_WAN_PAD)
/* WAN���б��� */
#define ATP_WAN_BIND_LEN 1024
#endif
/* WAN�����б��� */
#define ATP_WAN_SERVICE_LEN 32
/* ��̬·��Э�鳤�� */
#define ATP_WAN_ROUTE_LEN 16
/* RIP����ģʽ���� */
#define WAN_RIP_LEN 8
/* ���Ӵ������� */
#define ATP_WAN_TRIGGER_LEN 16
/* APN���� */
#define ATP_WAN_APN_LEN (32 + ATP_WAN_PAD)
/* ����ָ��� */
#define ATP_WAN_DIALNUM_LEN (32 + ATP_WAN_PAD)
/* ǰ׺���� */
#define ATP_WAN_PREFIXLIST_LEN (128 + ATP_WAN_PAD)
/* IPV6��ַ���� */
#define ATP_WAN_IPV6_ADDR_LEN 64
/* IPV6 DNS���� */
#define ATP_WAN_IPV6_DNS_LEN (128 + ATP_WAN_PAD)
#ifdef SUPPORT_ATP_DOT1X
#define ATP_WAN_DOT1X_CMD_LEN 64
#define ATP_WAN_DOT1X_LINE_LEN 256
#endif
#ifdef SUPPORT_ATP_IPV6
#ifndef IPV6_MIN_MTU
#define IPV6_MIN_MTU 1280
#endif
#define IPV6_STD_MTU 1500
#define IPV6_ADDRESS_WAN_DISPLAY_NUM 3/*l00168621 display addr num on the web 20100824, can be modified.*/
#endif

/* ֪ͨCWMP PPP�û��������� */
#define WAN_CWMP_PPP_USER_NUM 4
/* LAN��Ŀ */
#ifdef SUPPORT_ATP_WLAN
#define WAN_LAN_NUM 8
#else
#define WAN_LAN_NUM 4
#endif
/* ppp�ͻ��˽ӿ�����ţ�ǰ�汣����ppp����� */
#define WAN_PPPC_BASE 0xFF

#define ATP_WAN_SAME_TYPE_NUM        2

#define ATP_WAN_STR_UNCONFIG       "Unconfigured"
#define ATP_WAN_STR_ROUTED         "IP_Routed"
#define ATP_WAN_STR_IP_BRIDGED     "IP_Bridged"
#define ATP_WAN_STR_PPP_BRIDGED    "PPPoE_Bridged"
#define ATP_WAN_STR_DHCP_SPOOFED   "DHCP_Spoofed"
#define ATP_WAN_STR_DHCP_SPOOFED_IP "DHCP_Spoofed_IP"

#define ATP_WAN_STR_DHCP           "DHCP"
#define ATP_WAN_STR_STATIC         "Static"
#define ATP_WAN_STR_SLAAC          "SLAAC"
#define ATP_WAN_STR_INTERNET       "INTERNET"
#define ATP_WAN_STR_TR069_INTERNET "TR069_INTERNET"
#define ATP_WAN_STR_TR069          "TR069"
#define ATP_WAN_STR_OTHER          "Other"
#define ATP_WAN_STR_VOICE_INTERNET "VOICE_INTERNET"
#define ATP_WAN_STR_VOICE          "VOICE"
#define ATP_WAN_STR_TR069_VOICE    "TR069_VOICE"
#define ATP_WAN_STR_VOICE_TR069_INTERNET "VOICE_TR069_INTERNET"


#define ATP_WAN_STR_IDLE           "Idle"
#define ATP_WAN_STR_ONDEMAND       "OnDemand"
#define ATP_WAN_STR_ALWAYSON       "AlwaysOn"
#define ATP_WAN_STR_MANUAL         "Manual"
#define ATP_WAN_STR_BR0            "br0"

#define ATP_WAN_STR_PAP            "PAP"
#define ATP_WAN_STR_CHAP           "CHAP"
#define ATP_WAN_STR_CHAP_PAP       "AUTO"

#define ATP_WAN_STR_CONNECTING     "Connecting"
#define ATP_WAN_STR_CONNECTED      "Connected"
#define ATP_WAN_STR_PENDING        "PendingDisconnect"
#define ATP_WAN_STR_DISCONNECTING  "Disconnecting"
#define ATP_WAN_STR_DISCONNECTED   "Disconnected"
#define ATP_WAN_STR_AUTHENTICATING "Authenticating"

#define ATP_WAN_STR_ERR_NO_CARRIER "ERROR_NO_CARRIER"
#define ATP_WAN_STR_ERR_FROCE_DISCONN "ERROR_FORCED_DISCONNECT"

#define ATP_WAN_STR_BACKUP_DISABLE "Disable"
#define ATP_WAN_STR_BACKUP_CONFIRM "Backup_Confirm"
#define ATP_WAN_STR_BACKUP_AUTO    "Backup_Auto"

#define ATP_IPV6_ENABLE_FLAG_PATH     "/proc/sys/net/ipv6/conf/all/enable"




#ifdef SUPPORT_ATP_DOT1X
#define DHCPC_DOT1X_UNPASS 0
#define DHCPC_DOT1X_PASS   1
#endif

#ifdef SUPPORT_ATP_TR181
typedef struct tagATP_INTERFACE_ST ATP_WAN_INFO_ST;
#else
typedef struct tagATP_WAN_INFO_ST ATP_WAN_INFO_ST;
#endif

/* WAN DEV ���нӿ����� */
typedef enum 
{
    /* XDSL���� */
    ATP_WAN_TYPE_DSL = 1,
    /* ��̫������ */
    ATP_WAN_TYPE_ETH = 2,
    /* POTS���� */
    ATP_WAN_TYPE_POTS = 4,
    /* UMTS���� */
    ATP_WAN_TYPE_UMTS = 8,
    /* VDSL ����*/
    ATP_WAN_TYPE_VDSL = 16,
}ATP_WAN_TYPE_EN;

/* WAN DEV ���нӿ�״̬ */
typedef enum 
{
    /* ���� */
    ATP_WAN_LINK_STATUS_UP,
    /* ������ */
    ATP_WAN_LINK_STATUS_DOWN,
}ATP_WAN_LINK_STATUS_EN;

/* WAN DEV ��·���� */
typedef enum
{
    /* δ���� */
    ATP_WAN_LINKTYPE_UNCONFIGURED,
    /* RFC2684 ATM�ϳ����Ž���̫�� */
    ATP_WAN_LINKTYPE_EOA,
    /* RFC2684 ATM�ϳ����Ž���̫����RFC2516 ��̫������PPPЭ�飬�������ѽ��� */
    ATP_WAN_LINKTYPE_PPPOE,
    /* RFC2684 ATM��֧��·��IP */
    ATP_WAN_LINKTYPE_IPOA,
    /* RFC2364 ATM�ϳ���PPP */
    ATP_WAN_LINKTYPE_PPPOA,
    /* RFC1577 ATM��֧�ִ�ͳIP */
    ATP_WAN_LINKTYPE_CIP,
    /* ��̫������ */
    ATP_WAN_LINKTYPE_ETH,
    /* POTS���� */
    ATP_WAN_LINKTYPE_PPP_DIALUP,
    /* PTM */
    ATP_WAN_LINKTYPE_EOP,
}ATP_WAN_LINKTYPE_EN;


/* WANЭ������ */
typedef enum
{
    /* UNCONFIG */
    ATP_WAN_PROTOCOL_UNCONFIGURED,
    /* PPPOE */
    ATP_WAN_PROTOCOL_PPPOE,
    /* IPOE */
    ATP_WAN_PROTOCOL_IPOE,
    /* IP BRIDGE */
    ATP_WAN_PROTOCOL_IP_BRIDGE,
    /* PPP BRIDGE */
    ATP_WAN_PROTOCOL_PPP_BRIDGE,
    /* PPPOA */
    ATP_WAN_PROTOCOL_PPPOA,
    /* IPOA */
    ATP_WAN_PROTOCOL_IPOA,
    /* PPP DIALUP */
    ATP_WAN_PROTOCOL_PPP_DIALUP,
}ATP_WAN_PROTOCOL_EN;

/* WAN �������� */
typedef enum
{
    /* δ��ʼ�� */
    ATP_WAN_CONNTYPE_UNCONFIGURED,
    /* ·��ģʽ */
    ATP_WAN_CONNTYPE_ROUTED,
    /* �Ž�ģʽ */
    ATP_WAN_CONNTYPE_BRIDGED,
    /* ��WAN���PPP��ַͨ��DHCP�����LAN���豸 */
    ATP_WAN_CONNTYPE_DHCP_SPOOF,
#ifdef SUPPORT_ATP_DHCP_SPOOFED_IP
    ATP_WAN_CONNTYPE_DHCP_SPOOF_IP,
#endif
}ATP_WAN_CONNTYPE_EN;

/* WAN �������ͣ���λ���� */
typedef enum
{
    /* ������ͨ����ҵ�� */
    ATP_WAN_SERVICE_INTERNET = 1,
    /* TR069ר�� */
    ATP_WAN_SERVICE_TR069 = 2,
    /* VOICEר�� */
    ATP_WAN_SERVICE_VOICE = 4,
    /* ��������ҵ������ */
    ATP_WAN_SERVICE_OTHER = 8,
    /* ���ع���ҵ������ */
    ATP_WAN_SERVICE_SHARENET = 16,
}ATP_WAN_SERVICE_EN;

/* NAT���� */
typedef enum
{
    /* δ���� */
    ATP_WAN_NAT_DISABLED = 0,
    /* NAPT */
    ATP_WAN_NAT_NAPT = 1,
    /* CONE NAT */
    ATP_WAN_NAT_CONE = 2,
#ifdef SUPPORT_ATP_ALLCONENAT
    /* address restricted cone nat */
    ATP_WAN_NAT_ADDR_CONE = 3,

    /* port restricted cone nat */
    ATP_WAN_NAT_PORT_CONE = 4,
#endif

    /* NAT�������ֵ */
    ATP_WAN_NAT_MAX,
}ATP_WAN_NAT_EN;

typedef enum
{
    /* WAN���ݵ�����ҳ���ѯ*/
    ATP_WAN_GMSG_GET_BACKUP = 0,
    /* ����ATP_GMSG_SUBTYPE_ASYNC_SET�������ֶ����� */
    ATP_WAN_GMSG_SET_MANUAL_DIAL = 1,
    /* WAN���ݵ�����ҳ����� */
    ATP_WAN_GMSG_SET_BACKUP = 2,
#ifdef SUPPORT_ATP_WAN_AUTO_SENSING    
    /* WAN Auto-sensing��ǰ״̬ */
    ATP_WAN_GMSG_AUTOSENSING_STATUS = 5,
#endif
} ATP_WAN_GMSG_SUBTYPE_EN;

#ifdef SUPPORT_ATP_WAN_AUTO_SENSING
typedef enum 
{
    WAN_AUTO_SENSING_SUCCESS,            // 0: WAN Auto-sensing�ɹ�����⵽����WAN����
    WAN_AUTO_SENSING_LINKDOWN,           // 1:Auto-sensingģʽ�£�DSL��ETH���нӿڲ�����
    WAN_AUTO_SENSING_DSLWANDOWN,         // 2:Auto-sensingģʽ�£�DSL��·����,PPP����ʧ��
    WAN_AUTO_SENSING_ETHWANDOWN,         // 3:Auto-sensingģʽ�£�ETH��·����,PPP����ʧ��
    WAN_AUTO_SENSING_PPPDOWN,            // 4:Auto-sensingģʽ�£�DSL��ETH��·����,PPP����ʧ��
    WAN_FORCE_MODE_DSLDOWN,              // 5:Forced operating mode ΪDSL, DSL�ӿڲ�����
    WAN_FORCE_MODE_DSLWANDOWN,           // 6:Forced operating mode ΪDSL, DSL�ӿڿ���,PPP����ʧ��
    WAN_FORCE_MODE_ETHDOWN,              // 7:Forced operating mode ΪFibre, Fibre�ӿڲ�����
    WAN_FORCE_MODE_ETHWANDOWN,           // 8:Forced operating mode ΪFibre, Fibre�ӿڿ���,PPP����ʧ��
}ATP_WAN_AUTOSENSING_STATUS_EN;

/* WAN����Ӧͨ�ó��� */
#define ATP_WAN_AUTO_SENSING_LEN  16
/* WAN����Ӧ���ݽṹ */
typedef struct tagATP_WAN_AUTO_SENSING_ST
{
    /* ʹ������Ӧ����*/
    //VOS_UINT32 ulEnable; //���������ݲ���Ҫ
    /* δʹ������Ӧ������£������ֶ�ִ��һ������Ӧ */
    VOS_UINT32 ulForceSensing;
    /* ����Ӧ�ɹ���� */
    VOS_UINT32 ulComplete;
    /* ǿ������Ӧ�Ķ�ʱ�� */
    VOS_UINT32 ulTimer;
    /* δʹ������Ӧ������£�ǿ�Ƴ�ĳ�����У���DSL��Fibre */
    VOS_CHAR acForceMode[ATP_WAN_AUTO_SENSING_LEN];  //�ɰ汾��Ϊ DSL��Fibre����������Ҫ��Ϊ: DSL ��ETH��AUTO
    /* ��ǰ�������ͣ���DSL��Fibre��Unknown */
    VOS_CHAR acCurrentMode[ATP_WAN_AUTO_SENSING_LEN];
    /* ��һ�ε��������ͣ���DSL��Fibre��Unknown */
    VOS_CHAR acLastState[ATP_WAN_AUTO_SENSING_LEN];
    /* start of m00132836 ADD for WAN-auto LED status 20100804*/
    /* ����Ӧ��˸�ƶ�ʱ�� */
    ATP_UTIL_TIMER_ST* pLedTimer;    
    /* end of m00132836 ADD for WAN-auto LED status 20100804*/
} ATP_WAN_AUTO_SENSING_ST;

#endif

#define ATP_WAN_DOMAIN_LEN 64
#define ATP_WAN_BACKUP_MODE_LEN 32
#define ATP_WAN_BACKUP_TIMER_INTERVAL 3
#define ATP_WAN_BACKUP_INIT_DELAY 15

#ifdef SUPPORT_ATP_WAN_STATS_OVERFLOW
/* start of add by t00163975 2010-12-14 for BT-HHB3 ��չWAN���ͺͽ����ֽ�������ֵ��Χ��֧�ֳ���4g */
// ��¼���ͺͽ����ֽ�������������Ϊunsigned int��32λ������ֻ�ܼ�¼���4g��
// �����ܵ�TR098������ں�ʵ�ֵ����ƣ����ı����е����ݶ������͡�
// ���ÿ��WAN��������һ����������¼����ʱ4g����Ĵ�����һ�����ͳ�ƽ����
typedef struct
{
    /* �ѷ����ֽ��� */
    VOS_UINT32 ulBytesSentPre;
    
    /* �ѽ����ֽ��� */
    VOS_UINT32 ulBytesReceivedPre;

    /* �����ֽڼ������4g�Ĵ��� */
    VOS_UINT32 uiSendOver4gCnt;   
    
    /* �����ֽڼ������4g�Ĵ��� */
    VOS_UINT32 uiRecvOver4gCnt;   
} WAN_STAT_OVERFLOW_COUNT_ST;

#define WAN_STAT_OVERFLOW_CHECK_INTERVAL  (30)    // second
#define WAN_STAT_OVERFLOW_GAP   (1000000000)    // �������ж�ͳ����ֵ�з�תʱ��ֵ������Сֵ
/* end of add by t00163975 2010-12-14 for BT-HHB3 */
#endif

#ifdef SUPPORT_ATP_TR181
//!\brief 2	ATP_WAN_PostInit_NotifyRegisterʹ�õĺ�
/*!WANCMS��ʼ����
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_PPPOES=1,
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_ROUTE_INIT,
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_ROUTE6_INIT,  
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_NATOTHER_INIT,
}ATP_WAN_NOTIFY_PRIORITY_POSTINIT;
#endif
/* WAN�ӿ��շ�����ͳ����Ϣ*/
typedef struct
{
    /* �ѷ����ֽ��� */
    VOS_UINT32 ulBytesSent;
    /* �ѽ����ֽ��� */
    VOS_UINT32 ulBytesReceived;
    /* �ѷ��ͱ����� */
    VOS_UINT32 ulPacketsSent;
    /* �ѽ��ձ����� */
    VOS_UINT32 ulPacketsReceived;
    /* ���ʹ������� */
    VOS_UINT32 ulErrorsSent;
    /* ���մ������� */
    VOS_UINT32 ulErrorsReceived;
    /* ���͵��������� */
    VOS_UINT32 ulUnicastSent;
    /* ���յ��������� */
    VOS_UINT32 ulUnicastReceived;
    /* ���Ͷ��������� */
    VOS_UINT32 ulDiscardSent;
    /* ���ն��������� */
    VOS_UINT32 ulDiscardReceived;
    /* ���Ͷಥ������ */
    VOS_UINT32 ulMulticastSent;
    /* ���նಥ������ */
    VOS_UINT32 ulMulticastReceived;
    /* ���͹㲥������ */
    VOS_UINT32 ulBroadcastSent;
    /* ���չ㲥������ */
    VOS_UINT32 ulBroadcastReceived;
    /* ����δ֪���Ĳ��Ҷ����� */
    VOS_UINT32 ulUnknownReceived;
}ATP_WAN_STATS_ST;

typedef struct tagCurrentWanForACS
{
#if defined (SUPPORT_ATP_IPV6) && defined (SUPPORT_ATP_CWMP_IPV6)
  VOS_CHAR acCwmpIp[ATP_ACTUAL_LEN_64];
#else    
  VOS_CHAR acCwmpIp[ATP_WAN_IPADDR_LEN];
#endif
  VOS_INT32 iPid;
  VOS_CHAR   acInterface[ATP_WAN_IFC_LEN];
} CurrentWanForACS;


#ifdef SUPPORT_ATP_CWMP
extern CurrentWanForACS g_ACSWan;
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//!\brief 10	ATP_WAN_WanUpdateInfo_NotifyRegisterʹ�õĺ�
/*!
 WanUpdateInfo
 ����varĿ¼��һЩ״̬�ļ�

 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANUPDATEINFO_6RDTUNNEL=1,
    ATP_WAN_NOTIFY_PRIORITY_WANUPDATEINFO_SUPPORTRTU,
}ATP_WAN_NOTIFY_PRIORITY_WANUPDATEINFO;


//!\brief 13	ATP_WAN_RoutedWANUp_NotifyRegisterʹ�õĺ�
/*! ·��ģʽWAN up
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_LED=1,
    /*Start of Protocol 2014-5-21 15:31 for HOMEGW-17282:���blacklist·�� by t00189036*/
    /*hybridģ������ȼ�Ҫ��������ģ��*/
	ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_HYBRID,
    /*End of Protocol 2014-5-21 15:31 for by t00189036*/
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_ROUTESTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_DNSRESTART,    
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_SNTP, 	
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_VOICESTART,        // modify by y00183561 2012/12/20
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_CWMPSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_WANBACKUPCHANGED,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_QOSWANSTART,
	ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_FIREWALLWANSTART, 
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_DDNSSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_ACLWANSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_NATWANSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_CLEANTRACENAT,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_TR232START,
    /* BEGIN: Added by c65985, 2013/10/10   PN:DTS2013091007246*/
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP_RADVD,
    /* END:   Added by c65985, 2013/10/10 PN:DTS2013091007246*/
    /*HOMEGW-14424 by t00189036*/
    /*1������û�м�ʱ��Ҫ���ģ�飬�ŵ�update�У�����ipv4 wan up�Ĵ���ʱ��*/
    /*2��sntp��acs����Ҫ��v6���ȣ���Ҳ����ipv4 update�У�������v6���ϱ�*/
    /*3���޸�ǰ��ipv4 wan up������3�룬�ᵼ��cms�޷���ʱ����ipv6��Ϣ*/
    /*����ģ�龡����עipv4 wan update�¼�����Ҫ��עipv4 wan up�¼�*/
}ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUP;

//!\brief 15	ATP_WAN_PreRoutedWANDown_NotifyRegisterʹ�õĺ�
/*! Pre WAN down
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_LEDSTOP=1,
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_DNSDELETE,
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_VOICESTOPREGIT,
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_CWMPSTOP,
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_TR111STOP,    
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_TR232STOP, 
    /* BEGIN: Added by c65985, 2013/10/10   PN:DTS2013091007246*/
    ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN_RADVD,
    /* END:   Added by c65985, 2013/10/10 PN:DTS2013091007246*/   
}ATP_WAN_NOTIFY_PRIORITY_PREROUTEDWANDOWN;


//!\brief 16	ATP_WAN_RoutedWANDown_NotifyRegisterʹ�õĺ�
/*! ·��ģʽWAN down
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_HYBRID=1,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_CWMPSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_VPNWANRULESTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_VPNL2TPSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_DNSRESTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_ROUTESTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_WANBACKUPCHANGED,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_QOSWANSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_FIREWALLWANSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_ACLWANSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_NATWANSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_ALGSIPSIG,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_ALGSIPSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_DDNSSTOP,    
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_IGMPPROXYRESET,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_PTWANRULSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_DEFAULTROUTING,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_VOICESTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_GRETUNNELSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_6RDTUNNELSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_FIREWALLWANSTOP6,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_ACLWANSTOP6,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_SNTPPROC,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_CLEANTRACENAT,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_FONSTOP,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN_NAS,
}ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANDOWN;


//!\brief 25	WanPreWanUpdateInfoNotifyInitʹ�õĺ�
/*! 
pre wan udpate info
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREWANUPDATEINFO_DNSRESTART=1,
}ATP_WAN_NOTIFY_PRIORITY_PREWANUPDATEINFO;

//!\brief 25	WanPreWanUpdateInfoNotifyInitʹ�õĺ�
/*! 
pre wan udpate info
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREPPPWANUPDATEINFO_ROUTE=1,
	ATP_WAN_NOTIFY_PRIORITY_PREPPPWANUPDATEINFO_ROUTE6,
}ATP_WAN_NOTIFY_PRIORITY_PREPPPWANUPDATEINFO;


//!\brief 13	ATP_WAN_RoutedWANUpdate_NotifyRegisterʹ�õĺ�
/*! ·��ģʽWAN update
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_GRETUNNELSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_NAS,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_SIPSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_IGMPPROXYRESET,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_FONSTART,
    ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE_FTPFIREWALLSTART,
}ATP_WAN_NOTIFY_PRIORITY_ROUTEDWANUPDATE;

typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_LTEWANUP_HYBRID,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANUP_DNS,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANUP_SNTP,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANUP_GRETUNNEL,
}ATP_WAN_NOTIFY_PRIORITY_LTEWANUP;


typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_LTEWANDOWN_HYBRID,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANDOWN_DNS,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANDOWN_SNTP,
    ATP_WAN_NOTIFY_PRIORITY_LTEWANDOWN_GRETUNNEL,
}ATP_WAN_NOTIFY_PRIORITY_LTEWANDOWN;

typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_STATICWANUP_HYBRID,
}ATP_WAN_NOTIFY_PRIORITY_STATICWANUP;

typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_TUNNELDOWN_HYBRID,
}ATP_WAN_NOTIFY_PRIORITY_TUNNELDOWN;

typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_TUNNELUP_HYBRID,
}ATP_WAN_NOTIFY_PRIORITY_TUNNELUP;

typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_STATICWANDOWN_HYBRID,
}ATP_WAN_NOTIFY_PRIORITY_STATICWANDOWN;


#ifdef SUPPORT_ATP_IPV6
//!\brief 17	ATP_WAN_WanIpv6UpdateInfo_NotifyRegisterʹ�õĺ�
/*! 
 WanUpdateInfo
 ipv6����varĿ¼��һЩ״̬�ļ�

 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEINFO_DSLITETUNNEL=1,
}ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEINFO;

//!\brief 18	ATP_WAN_WanIpv6UpProcess_NotifyRegisterʹ�õĺ�
/*! 
 ipv6 WAN up 
 WanIpv6UpProcess
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_LED=1,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_HYBRID,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_ROUTESTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_DNSRESTART,    
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_RADVD,  
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_DHCP6S, 
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_FIREWALLWANSTART6,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_CWMPSTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_MLDPROXYRESET,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_IPV6COMMONGETINT,    
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_IPV6MSSADJUSTSTART,   
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_IPV6NL,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_DSLITETUNNELSTART,      
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_PRIVACYBUTTONSTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_WANBACKUPCHANGED,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_FTPFIREWALLSTART6,
}ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS;

//!\brief 19	ATP_WAN_WanIpv6DownProcess_NotifyRegisterʹ�õĺ�
/*! 
 ipv6 WAN down 
 WanIpv6DownProcess

 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_HYBRID=1,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_MLDPROXYRESET,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_CWMPSTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_ROUTESTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_FIREWALLWANSTOP6,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_IPV6MSSADJUSTSTOP,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_IPV6NL,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_DSLITETUNNELSTOP,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_RADVD,    
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_DHCP6S,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_SNTPPROC,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_VOICE,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_WANBACKUPCHANGED,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPPROCESS_FTPFIREWALLSTOP6,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS_NAS,
}ATP_WAN_NOTIFY_PRIORITY_WANIPv6DOWNPROCESS;


//!\brief 20	ATP_WAN_WanIpv6UpdateProcess_NotifyRegisterʹ�õĺ�
/*! 
 ipv6 WAN update
 WanIpv6UpdateProcess

 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_LED=1,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_HYBRID,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_SNTP,  	
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_VOICE,  
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_CWMPSTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_IPV6NL,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_DSLITETUNNELSTART,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_ABUSE,
    ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS_NAS,
}ATP_WAN_NOTIFY_PRIORITY_WANIPv6UPDATEPROCESS;

//!\brief 26	ATP_WAN_PreWanIpv6UpdateInfo_NotifyRegisterʹ�õĺ�
/*! 
pre wan6 udpate info
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREWANIPV6UPDATEINFO_LEDSTOP=1,
    ATP_WAN_NOTIFY_PRIORITY_PREWANIPV6UPDATEINFO_DNSRESTART=1,
    ATP_WAN_NOTIFY_PRIORITY_PREWANIPV6UPDATEINFO_ROUTE6,
}ATP_WAN_NOTIFY_PRIORITY_PREWANIPV6UPDATEINFO;

typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_TUNNEL6DOWN_RADVD,
}ATP_WAN_NOTIFY_PRIORITY_TUNNEL6DOWN;

typedef enum
{    
    ATP_WAN_NOTIFY_PRIORITY_TUNNEL6UP_RADVD,
}ATP_WAN_NOTIFY_PRIORITY_TUNNEL6UP;

#endif



/*!
  \brief ·��WAN UPʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_RoutedWANUp_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);



/*!
  \brief ·��WAN DOWN�����Ϣǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreRoutedWANDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief ·��WAN DOWNʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_RoutedWANDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief ·��WAN updateʱ��ע�ắ�� for HOMEGW-14424
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/

VOS_UINT32 ATP_WAN_RoutedWANUpdate_NotifyRegister(AtpNotifierProc                 pfProc,
                                                                    VOS_UINT32                      ulPriority,
                                                                    VOS_VOID                        *pvNotifyHook);


/*!
  \brief WAN��Ϣ�仯ʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanUpdateInfo_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);


/*!
  \brief WAN������Ϣǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreWanUpdateInfo_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

#ifdef SUPPORT_ATP_HYBRID
VOS_UINT32 ATP_WAN_LteWANUp_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_LteWANDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_WanLteIpv6UpProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_WanLteIpv6DownProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

#endif

VOS_UINT32 ATP_WAN_TunnelDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_TunnelUp_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
																	
/*Start of ATP 2014-2-25 for DTS2014022103725 by y00250667*/
VOS_UINT32 ATP_WAN_LTETunnul6Up_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_LTETunnul6Down_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
/*End of ATP 2014-2-25 for DTS2014022103725 by y00250667*/


#ifdef SUPPORT_ATP_IPV6
/*!
  \brief IPV6 WAN��Ϣ����ʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv6UpdateInfo_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief IPV6 WAN UPʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv6UpProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief IPV6 WAN DOWNʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv6DownProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief IPV6 WAN ״̬�仯��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv6UpdateProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief IPV6 WAN������Ϣǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreWanIpv6UpdateInfo_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_WAN_Tunnel6Down_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_WAN_Tunnel6Up_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
#endif


VOS_UINT32 WanPreWanUpdateInfoNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanUpdateInfoNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanRoutedWANUpNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanPreRoutedWANDownNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanRoutedWANDownNotifierDoNotify(VOS_CHAR* pszWanPath);
#ifdef SUPPORT_ATP_HYBRID
VOS_UINT32 WanLteWANUpNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanLteWANDownNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanStaticWANUpNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanStaticWANDownNotifierDoNotify(VOS_CHAR* pszWanPath);
#endif

VOS_UINT32 WanRoutedWANUpNotifyInit(VOS_VOID);
VOS_UINT32 WanRoutedPreWANDownNotifyInit(VOS_VOID);
VOS_UINT32 WanRoutedWANDownNotifyInit(VOS_VOID);
VOS_UINT32 WanUpdateInfoNotifyInit(VOS_VOID);
VOS_UINT32 WanPreWanUpdateInfoNotifyInit(VOS_VOID);
#ifdef SUPPORT_ATP_HYBRID
VOS_UINT32 WanLteWANUpNotifyInit(VOS_VOID);
VOS_UINT32 WanLteWANDownNotifyInit(VOS_VOID);
VOS_UINT32 WanStaticWANUpNotifyInit(VOS_VOID);
VOS_UINT32 WanStaticWANDownNotifyInit(VOS_VOID);
VOS_UINT32 WanLTETunnul6UpNotifyInit(VOS_VOID);
VOS_UINT32 WanLTETunnul6DownNotifyInit(VOS_VOID);


#endif
VOS_UINT32 TunnelUpNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 TunnelDownNotifierDoNotify(VOS_CHAR* pszWanPath);

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 WanIpv6UpdateInfoNotifyInit(VOS_VOID);//  17.
VOS_UINT32 WanIpv6UpProcessNotifyInit(VOS_VOID);//  18.
VOS_UINT32 WanIpv6DownProcessNotifyInit(VOS_VOID);//  19
VOS_UINT32 WanLteIpv6UpProcessNotifyInit(VOS_VOID);//  18.
VOS_UINT32 WanLteIpv6DownProcessNotifyInit(VOS_VOID);//  19
VOS_UINT32 WanIpv6PreDownProcessNotifyInit(VOS_VOID);
VOS_UINT32 WanIpv6UpdateProcessNotifyInit(VOS_VOID);//  20  
VOS_UINT32 WanPreWanIpv6UpdateInfoNotifyInit(VOS_VOID);//26
VOS_UINT32 Tunnel6UpNotifyInit(VOS_VOID);
VOS_UINT32 Tunnel6DownNotifyInit(VOS_VOID);
VOS_UINT32 ATP_WAN_WanIpv6UpProcess_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority,VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_WAN_WanIpv6DownProcess_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_WAN_WanIpv6PreDownProcess_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32 WanPreWanIpv6UpdateInfoNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanIpv6UpdateInfoNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanIpv6UpProcessNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanIpv6DownProcessNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanIpv6PreDownProcessNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanIpv6UpdateProcessNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 Tunnel6UpNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 Tunnel6DownNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanLTETunnel6DownProcessNotifierDoNotify(VOS_CHAR* pszWanPath);
VOS_UINT32 WanLTETunnel6UpProcessNotifierDoNotify(VOS_CHAR* pszWanPath);


VOS_UINT32 WanPostIPV6InitNotifierDoNotify(VOS_VOID);
#endif

#ifdef SUPPORT_ATP_UNIVERSAL_XTM_QOS
VOS_UINT32 WANPPPOAWanUpProcNotifierDoNotify(VOS_CHAR* pszInterfaceName);
VOS_UINT32 WANPPPOAWanDnProcNotifierDoNotify(VOS_CHAR* pszInterfaceName);
#endif

#if defined(__cplusplus)
}
#endif
#endif
