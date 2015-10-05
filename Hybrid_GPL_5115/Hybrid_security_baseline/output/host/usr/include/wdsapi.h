
/* <DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 BEGIN */
/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2008, ��Ϊ�������޹�˾.
  �ļ���  : wds.h
  ��  ��  : mayang 00100263
  ��  ��  : 001
  ��������: 2008-4-22
  �������: 
  ��������: WDS����ͷ�ļ�
      
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    mayang      2008-4-22    001     init
    mayang      2008-5-7     002     1. ɾ��״̬�������������
    mayang      2008-5-15    003     1. ȫ�ֱ���Ų��C�ļ���
    gKF34687   2010-11-4     004     b683 ��ֲ
  ��ע: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<�޸���>      <ʱ��>        <���ⵥ��>             <��Ҫ����>
l00205975     2012/07/17    DTS2012072100113        ��������[B890]V100R001C994B101(Router):�����Reset����������Ч
z00185914           2013/01/28          DTS2013012808057		�����š��½�ndis�����ļ��� 
z00185914    2013/02/17    DTS2013021703454 �����š��Ż�ndis���Ŵ��� 
z00185914    2013/04/28    DTS2013042003518    ������ѡ�񡿵��岦�ųɹ�������ģʽ��auto�л�Ϊ��������ģʽʱ��ҳ����ʾ���ŶϿ���ʵ���ϲ��Ų�δ�Ͽ�
========================================================================================*/
#ifndef _WDS_H_H_
#define _WDS_H_H_

#include <stdarg.h>
#include <netinet/in.h>
#include "atpconfig.h"    /* DTS2012020400492 j81004134 2012/2/4 */

/* <DTS2013021703454 �Ż�ndis���Ŵ��� z00185914 20130217 DELETE 1 ROW */

#define WDS_SUCCESS 0
#define WDS_FAIL    -1
#define WDS_TIMEOUT -2

#define TRUE    1
#define FALSE   0

/* <DTS2013021703454 �Ż�ndis���Ŵ��� z00185914 20130217 BEGIN */
/* --------------------------------------------------------------------------
/ Common Type Defines
/ --------------------------------------------------------------------------*/
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;  

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define IP_FAMILY_IPV4  0x04
#define IP_FAMILY_IPV6  0x06
#define IP_FAMILY_IPV4V6  0x0a
/* DTS2013021703454 �Ż�ndis���Ŵ��� z00185914 20130217 END> */

/* �ӿ������� */
#define ATP_QMI_IFC_LEN  32
/* �ӿ������� */
#define ATP_QMI_DEV_LEN  32
/* �û������� */
#define ATP_QMI_USER_LEN  68
/* ���볤�� */
#define ATP_QMI_PASSWD_LEN  68
/* ���б��� */
#define ATP_QMI_BIND_LEN  64
#define WAN_QMI_SESSION_LEN 32
/*AT����ָ���*/
#define ATP_QMI_DIALNUMBER_LEN  32
/*APN���ó���*/
#define ATP_QMI_SETAPN_LEN  65   /* <DTS2010121603783.QMI: gKF34687 2010-12-18 ADD>*/
/* DNS�б��� */
#define ATP_QMI_DNS_LEN  64
/* IP��ַ���� */
#define ATP_QMI_IPADDR_LEN  16
/*VHG���� pppoe service name*/
#define ATP_QMI_SERVICENAME_LEN  256
#define ATP_QMI_ACNAME_LEN  256
/* MAC��ַ���� */
#define ATP_QMI_MAC_LEN  32
 /* profile�и�������ַ�������(��������\0) */
#define PROFILE_MAX_LEN    65
 /* ����ģʽ ���� */
#define DIAL_MODE_LEN    16
/* ����ӿ�����*/
#define WAN_IFNAME_QMI  "rmnet0"
/* ����pid�ļ�*/
#define WDS_PID_FILE    "/var/wan/rmnet0/pid"
/*ipv4����profile�ļ�*/  /* DTS2012020400492 j81004134 2012/2/4 */
#define WDS_PROFILE_FILE    "/var/wan/rmnet0/config"
/* ��¼��������ͳ�����ݵ��ļ�*/
#define FMC_QMI_STATS_FILE  "/var/wan/rmnet0/umts_stats"
/*��־�ļ�*/
#define WDS_LOG_FILE    "/var/log/wds.log"
/*������ ��ʱʱ�䣬��λ:����*/
#define ALARM_TIME 1000
/*QMI TTYӦ�ò�WDS���̵Ľڵ�*/
/* added by gKF345687 on 2010.11.22 for DTS2010112200946 begin */
#define DEVICE_TTY_PATH_0 "/dev/qmitty0"
#define DEVICE_TTY_PATH_1 "/dev/qmitty1"
/* added by gKF345687 on 2010.11.22 for DTS2010112200946 end */
/*TTY��������*/
#define MAX_TTY_CHECK_TIMES 30      /* <DTS2012032804611 g68080 2012-04-12 mod > */
/*USBNET������������*/
#define MAX_USBNET_CHK_TIMES 30
// TTY�л�ȡTx ID������
#define QMI_TTY_IOC_MAGIC     'x'
#define QMI_TTY_IOCQ_TXID      _IO(QMI_TTY_IOC_MAGIC, 3)

/* <DTS2012020400492  j81004134 2012/2/4 begin */
#define IPV4_ADDR_LENGTH   (4)

/* ����ӿ����� */
#define WAN2_IFNAME_QMI  "rmnet1"
 /* ���߲����configured profile ���� */
//#define MAX_CONFIGURED_PROFILE_NUM      16
#define MAX_CONFIGURED_PROFILE_NUM      2

/* dual IP ����ʱIPv4ʹ��g_3GPP_cfg_profile_index_list[]�еĵ�һ��configured profile  */
#define CONFIGURED_PROFILE_INDEX_IPV4      0     
/* dual IP ����ʱIPv6ʹ��g_3GPP_cfg_profile_index_list[]�еĵڶ���configured profile  */
#define CONFIGURED_PROFILE_INDEX_IPV6      1         
 /* dual IP ����ʱ��Ҫ��configured profile ���� */
#define DUAL_IP_CONFIGURED_PROFILE_NUM     2
/* QMI�ӿ�����Ż�ַƫ�� */
#define WAN_QMI_BASE_OFFSET  (0x3)

#if defined (SUPPORT_ATP_QMI6)
#define IPV6_ADDR_LENGTH   (16)
#define IPV6_PREFIX_LENGTH (1)
/*ipv6����profile�ļ�*/
#define WDS_PROFILE_FILE_IPV6    "/var/wan/rmnet0/config6"
/* ��¼IPv6 WAN �豸����ͳ�����ݵ��ļ�*/
#define FMC_QMI_STATS_FILE_IPV6  "/var/wan/rmnet1/umts_stats"
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
/*----------------------WDS��������ģ��----------------------------*/
#define SIGBREAK 36       /*�Ͽ����ӵ��ź�*/
#define SIGCNNT  37       /*�������ӵ��ź�*/
#define SIGUSBDOWN  38       /*sub״̬down�ź�*/      /* <DTS2010120301841.QMI: gKF34687 2010-12-06 ADD >*/
#define SIGSENDRS  39       /*����һ��RS����*/    /* DTS2012020400492 j81004134 2012/2/4 */  
/* <DTS2012060604837 j81004134 2012/6/5 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
#define SIG_QMI_LOG_SWITCH_CHANGE 39
#endif
/* < DTS2012072100113 l00205975 2012/07/27 begin */ 
#if defined(SUPPORT_ATP_ATP_FMCWEB)
#define SIG_QMI_KEY_PRESS       40  /* �̰� */
#define SIG_QMI_KEY_LONG_PRESS  41  /* ���� */
#endif
/* DTS2012072100113 l00205975 2012/07/27 end> */ 
/* <DTS2013072505873 w00189018 2013/7/5 begin */
#define SIG_QMI_ROAM           42  /* ����֪ͨ */
/* DTS2013072505873 w00189018 2013/7/5 end> */
#define SIGFORBIDCONN           43  /*�Ͽ����ҽ�ֹ���ŵ��ź�*/
#define SIGALLOWCONN            44   /*�����ŵ��ź�*/
/* DTS2013042003518 z00185914 2013/04/28 END> */
/* DTS2012060604837 j81004134 2012/6/5 end> */
 /* added by gKF345687 on 2010.11.22 for DTS2010112201047 begin */
/*#define ATP_BIGENDIAN*/
 /* added by gKF345687 on 2010.11.22 for DTS2010112201047 end */
//#ifndef ATP_BIGENDIAN
#if 0
#define HTON16(i)   ((uint16)( (((uint16)(i) & (uint16)0x00ffU) << 8) | \
                                (((uint16)(i) & (uint16)0xff00U) >> 8) ))
#define HTON32(i)   ((uint32)( \
                                (((uint32)(i) & (uint32)0x000000ffUL) << 24) | \
                                (((uint32)(i) & (uint32)0x0000ff00UL) <<  8) | \
                                (((uint32)(i) & (uint32)0x00ff0000UL) >>  8) | \
                                (((uint32)(i) & (uint32)0xff000000UL) >> 24) ))
#define NTOH16(i)    HTON16(i)
#define NTOH32(i)    HTON32(i)
#else
#define HTON16(i) (i)
#define HTON32(i) (i)
#define NTOH16(i) (i)
#define NTOH32(i) (i)
#endif

/* ��֤��ʽ(QMI def: 1,2,3;)*/
/* <DTS2010121402419.QMI: gkf34687 2010-12-14 MODIFY BEGIN */
typedef enum
{
    ATP_QMI_AUTH_NONE,  
    ATP_QMI_AUTH_PAP,
    ATP_QMI_AUTH_CHAP,
    ATP_QMI_AUTH_AUTO
}AUTH_MODE;
/* DTS2010121402419.QMI: gkf34687 2010-12-14 MODIFY END> */

/*typedef enum
{
    CHAP_WEB = 1,
    PAP_WEB,
    PAP_CHAP_WEB
}AUTH_WEB;*/

/*�������ͣ���������var/wan/usbnet0/dial ����enum dialstatus����һ��*/
typedef enum
{
    DIAL_MODE_WAIT= 0,
    DIAL_MODE_DISCON, 
    DIAL_MODE_HAND,   /* �ֶ����� */
    DIAL_MODE_AUTO,  /* �Զ����� */
    DIAL_MODE_DEMAND,  /* ���貦�� */
    /* <DTS2012070207035 w00211169 2012-7-17 begin */
    DIAL_MODE_PENDING,  /* �ֶ�����δ���� */    
    /* DTS2012070207035 w00211169 2012-7-17 end> */
    DIAL_MODE_MAX
}DIAL_MODE_E;

 /*��������״̬����������var/wan/usbnet0/status ���� enum tagpppoestatus����һ��*/
typedef enum TAG_QMI_STATUS 
{
    QMI_STATUS_UNCONFIGURED = 0,
    QMI_STATUS_CONNECTING,
    QMI_STATUS_AUTHENTICATING,
    QMI_STATUS_CONNECTED,
    QMI_STATUS_PENDING,
    QMI_STATUS_DISCONNECTING,
    QMI_STATUS_DISCONNECTED
}QMI_STATUS_E;

/*��ǰWDS������Ϣ�ṹ*/
typedef struct
{
    char    sz_username[PROFILE_MAX_LEN];   /* �û���*/
    unsigned char   auth_type;                      /* ��֤��ʽ*/
    struct in_addr  dns1;                   /*��ҪDNS*/
    struct in_addr  dns2;                   /* ��ҪDNS*/
    struct in_addr  ip_addr;                /* IP��ַ*/
    struct in_addr  netmask;                /*����*/
    struct in_addr  gateway;                /* ���ص�ַ*/
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined (SUPPORT_ATP_QMI6)
    char ipv6_prefix;   /* ipv6��ַǰ׺ */
    char ipv6_gateway_prefix;   /* ipv6���ص�ַ ǰ׺ */
    struct in6_addr  ipv6_addr; /* ipv6��ַ */    
    struct in6_addr  ipv6_gateway; /* ipv6���ص�ַ */    
    struct in6_addr ipv6_dns1;   /* ipv6 ��Ҫdns server��ַ */
    struct in6_addr ipv6_dns2;   /* ipv6 ��Ҫdns server��ַ */
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
}wds_runtime_settings;


/* PPPOA QOS */
typedef struct
{
    unsigned int ulATMQoS;
    unsigned int ulATMPeakCellRate;
    unsigned int ulATMMaximumBurstSize;
    unsigned int ulATMSustainableCellRate;
}ATP_QMI_ATMQOS_ST;
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined (SUPPORT_ATP_QMI6)
/* IPV6��ַ���÷�ʽ */
typedef enum
{
    /* Disable */
    ATP_QMI_IPV6_ADDR_TYPE_DISABLE,
    /* SLAAC */
    ATP_QMI_IPV6_ADDR_TYPE_SLAAC,
    /* DHCP */
    ATP_QMI_IPV6_ADDR_TYPE_DHCP,
    /* Static */
    ATP_QMI_IPV6_ADDR_TYPE_STATIC,
}ATP_QMI_IPV6_ADDR_TYPE_EN;

/* IPV6��ַ����ѡ�� */
typedef enum
{
    /* IANA + IAPD */
    ATP_QMI_IPV6_ADDR_OPTION_BOTH,
    /* IANA */
    ATP_QMI_IPV6_ADDR_OPTION_IANA,
    /* IAPD */
    ATP_QMI_IPV6_ADDR_OPTION_IAPD,
    /* ����Ӧ���ݲ�֧�� */
    ATP_QMI_IPV6_ADDR_OPTION_AUTO,
}ATP_QMI_IPV6_ADDR_OPTION_EN;

/* <DTS2012022806805 00206962 2012-03-13 ADD begin> */
/* ���͸�CMS��slaac����������� */
typedef enum tag_RADVD_CMD_WORD_E
{
    RADVD_CMD_WORD_ONDEMAND_CFG = 0,               /* ���貦��ǰ������RADVD Ϊnormalģʽ */
    RADVD_CMD_WORD_COMEBACK                              /* ���貦�ź�����RADVD���̣��ָ�Ϊ���ݿ��е����� */
}RADVD_CMD_WORD_E;
/* DTS2012022806805 00206962 2012-03-13 ADD end> */
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
/* ���ò����ṹ ,��ATP_PPPC_CONFIG_ST ����һ��*/
typedef struct
{
    /* PPP�ӿ��� */
    char acInterface[ATP_QMI_IFC_LEN];
    /* PPP�ӿ��� */
    char acDevice[ATP_QMI_DEV_LEN];
    /* �����û��� */
    char acUserName[ATP_QMI_USER_LEN];
    /* ���� */
    char acPassWord[ATP_QMI_PASSWD_LEN];
    /* ���б� */
    char acBindList[ATP_QMI_BIND_LEN];
    /* �󶨹��ܱ�ʶ */
    unsigned char ucBindEnable;
    /* �����ܱ�ʶ */
    unsigned char ucProxyEnable;
    /* ��������û��� */
    unsigned char ucProxyNum;
    /* Э������ */
    unsigned char ucProtocol;
    /* ���Կ��� */
    unsigned char ucDebugEnable;
    /* ���� */
    unsigned char aucReserved[3];
    /* PPPOA QOS */
    ATP_QMI_ATMQOS_ST stAtmQos;
    char acPppSessid[WAN_QMI_SESSION_LEN];
    /*��֤��ʽ*/
    unsigned int ulAuth;
    /*���貦�ſ���ʱ��*/
    unsigned int ulIdleTime;
    /*B300 ����*/
    unsigned int ulRandomDialInterval;  /*PPPOE��ʼ����ǰ����ȴ�ʱ������ֵ��*/
    unsigned int ulRetryDialInterval;  /*PPPOE����ʧ�ܺ�ȴ���ʱ������*/
    unsigned int ulRetrySendInterval;  /*PPPOE�ط����ĵ�ʱ����*/
    unsigned int ulKeepAliveInterval;  /*LCP����ʱ����*/
    /*AT����ָ��*/
    char acDialNumber[ATP_QMI_DIALNUMBER_LEN];
    /*����APN*/
    char acSetApn[ATP_QMI_SETAPN_LEN];
    /*<DTS2010052700199 l00101002 20100531 Begin*/
    /*����ip*/
    char acSetIp[ATP_QMI_IPADDR_LEN];
    /*DTS2010052700199 l00101002 20100531 End>*/
    unsigned int ulMaxMru;
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#ifdef SUPPORT_ATP_MTU
    /* ���MTU */
    unsigned int ulMaxMtu;
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
    int iReqTime;
    /************ATPV1R2C03 �·���������д������****************/
    /*VHG���� pppoe service name*/
    char acPppoeServiceName[ATP_QMI_SERVICENAME_LEN];
    char acPppoeAcName[ATP_QMI_ACNAME_LEN];
    /* ��̫������PPPOA��MAC��ַ */
    char acPppoe2aMacAddr[ATP_QMI_MAC_LEN];
    /* ��̫������PPPOA�ĻỰID */
    unsigned int ulPppoe2aSid;
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined  (SUPPORT_ATP_QMI6)
    /* �Ƿ�����IPv4���� */
    unsigned int ulIpv4;
    /* �Ƿ�����IPv6���� */
    unsigned int ulIpv6;
    /* IPv6�����ַ���ͣ���IANA��IAPD */
    unsigned int ulManagedAddress;
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
}ATP_QMI_CONFIG_ST;

/*�û������ļ��ṹ*/
typedef struct tagUSER_PROFILE_ST
{
    /*char   acProfileName[PROFILE_MAX_LEN];*/
    /* <dual-stack w00211169 2012-6-19 begin */
    unsigned int  index;
    /* dual-stack w00211169 2012-6-19 end> */
    char   acDialNum[PROFILE_MAX_LEN]; 
    char   acUsername[PROFILE_MAX_LEN];
    char   acPasswd[PROFILE_MAX_LEN];
    char   acApn[PROFILE_MAX_LEN];
    char   acIpAddr[ATP_QMI_IPADDR_LEN];   
    unsigned int  uiAuth;
    char   acConnectionTrigger[DIAL_MODE_LEN];   /*dial_mode*/
    unsigned int  uiIdleDisconnectTime; /*max_idle_time*/
#if defined(SUPPORT_ATP_MTU)
    unsigned int  ulMaxMru; /* <DTS2010120401310.QMI: gKF34687 2010-11-26 ADD> */
#endif
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined  (SUPPORT_ATP_QMI6)
    /* �Ƿ�����IPv4���� */
    unsigned int ulIpv4;
    /* �Ƿ�����IPv6���� */
    unsigned int ulIpv6;
    /* IPv6�����ַ���ͣ���IANA��IAPD */
    unsigned int ulManagedAddress;
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
}USER_PROFILE_ST;

/* <dual-stack w00211169 2012-6-19 begin */
typedef struct
{    
    unsigned int handle;
    unsigned short tx_id;
    unsigned char status;
    /* <DTS2012082406657 w00211169 2012-8-25 begin */
    unsigned char ipup_done;
    /* DTS2012082406657 w00211169 2012-8-25 end> */
    USER_PROFILE_ST profile;
    wds_runtime_settings runtime_setting;
} QMI_CONNECTION_INFO_ST;
/* dual-stack w00211169 2012-6-19 end> */

/*------------------------------------------------------------
����ԭ�Σ�int ATP_QMI_Config (void)
����:                ����QMI�������ò����ļ�
����:                ��
���:                ��
����ֵ:          �ɹ�����0��ʧ�ܷ���-1
����:                ��
-------------------------------------------------------------*/
int atp_qmi_config(const ATP_QMI_CONFIG_ST* pstConfig);
/*------------------------------------------------------------
����ԭ�Σ�int ATP_QMI_Start (void)
����:               ����qmi���Ž��̣� 
����:                ��
���:                ��
����ֵ:          �ɹ�����0��ʧ�ܷ���-1
����:                ��
-------------------------------------------------------------*/
int atp_qmi_start(const char* pszInterface, int ip_family_pref);

/*------------------------------------------------------------
����ԭ�Σ�int atp_qmi_stop (void)
����:               �ر�qmi���Ž��̣� 
����:                ��
���:                ��
����ֵ:          �ɹ�����0��ʧ�ܷ���-1
����:                ��
-------------------------------------------------------------*/
int atp_qmi_stop(const char* pszInterface);

/******************************************************************************
  �������ƣ�atp_qmi_set_trigger
  ��������������QMI�Ĵ���ģʽ
  ���������pszInterface �ӿ���
                              ulTrigger ����ģʽ
  �����������
  �� �� ֵ���ɹ�WDS_SUCCESS��ʧ��WDS_FAIL
  ����˵����
******************************************************************************/
void atp_qmi_set_trigger(char* pszInterface, unsigned int ulTrigger);  /* <DTS2010112902193.QMITTY: gkf34687 2010-11-29 MODIFY >*/

/*****************************************************************************
 �� �� ��  : ATP_QMI_API_GetVersion
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��������汾��

*****************************************************************************/
const char * atp_qmi_api_get_version(void);

/*------------------------------------------------------------
  ����ԭ��:  int max_dial_time_get()
  ��  ��  :  �����󲦺�ʱ��(s)
  ��  ��  :  ��
  ��  ��  :  ��
  ����ֵ  :  int 
-------------------------------------------------------------*/
int max_dial_time_get(void);

/*------------------------------------------------------------
  ����ԭ��:  int mtu_get()
  ��  ��  :  ���MTUֵ
  ��  ��  :  ��
  ��  ��  :  ��
  ����ֵ  :  int 
-------------------------------------------------------------*/
int mtu_get(void);

/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined  (SUPPORT_ATP_QMI6)
/*------------------------------------------------------------
����ԭ�Σ�int ATP_QMI_Start (void)
����:               ����qmi���Ž��̣� 
����:                ��
���:                ��
����ֵ:          �ɹ�����0��ʧ�ܷ���-1
����:                ��
-------------------------------------------------------------*/
int atp_qmi6_start(const char* pszInterface);

/*------------------------------------------------------------
����ԭ�Σ�int atp_qmi_stop (void)
����:               �ر�qmi���Ž��̣� 
����:                ��
���:                ��
����ֵ:          �ɹ�����0��ʧ�ܷ���-1
����:                ��
-------------------------------------------------------------*/
int atp_qmi6_stop(const char* pszInterface);

/******************************************************************************
  �������ƣ�atp_qmi_set_trigger6
  ����������(��ipv6���ŷ�ʽ��)����QMI�Ĵ���ģʽ
  ���������pszInterface �ӿ���
                              ulTrigger ����ģʽ
  �����������
  �� �� ֵ����
  ����˵����
******************************************************************************/
void atp_qmi6_set_trigger(char* pszInterface, unsigned int ulTrigger);

/* <DTS2012022806805 00206962 2012-03-13 ADD begin */
/******************************************************************************
  �������ƣ�get_qmi6_dial_mode
  ������������ȡqmi ipv6�Ĳ�������
  �����������
  �����������
  �� �� ֵ������ʱ����WDS_FAIL
                          ��ȷʱ���ز�������
  ����˵����
******************************************************************************/
int get_qmi6_dial_mode(void);

/******************************************************************************
  �������ƣ�get_qmi6_dial_status
  ������������ȡqmi ipv6�Ĳ���״̬
  �����������
  �����������
  �� �� ֵ������ʱ����WDS_FAIL
                          ��ȷʱ���ز���״̬
  ����˵����
******************************************************************************/
int get_qmi6_dial_status(void);
/* DTS2012022806805 00206962 2012-03-13 ADD end> */

#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
#endif /*_WDS_H_H_*/
/* DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 END> */

