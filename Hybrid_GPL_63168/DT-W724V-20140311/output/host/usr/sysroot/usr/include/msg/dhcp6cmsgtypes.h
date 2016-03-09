/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6ccmsmsgtypes.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author x00192803
  *\date  2012-1-31
*/

#ifndef __ATP_DHCP6CCMS_MSGTYPES_H__
#define __ATP_DHCP6CCMS_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_CBBID_DHCP6C_NAME                  "dhcp6c"

#define DCHP6C_UPDATEINFO 9

/* WAN���ӵ�ַ���ͳ��� */
#define DCHP6C_ADDRTYPE_LEN 8

/* WAN·����󳤶� */
#define DCHP6C_PATH_LEN 128

/* WAN�ӿ�����󳤶� */
#define DCHP6C_IFC_LEN  32

#define DHCP6C_REQADDR_FLAG_FILE                   "/var/wan/%s/requestaddr"
#define DHCP6C_REQPREFIX_FLAG_FILE                   "/var/wan/%s/requestprefix"
#define DHCP6C_RAPIDCOMMIT_FLAG_FILE                   "/var/wan/%s/rapidcommit"
#define DHCP6C_SUGGEST_T1_FILE                   "/var/wan/%s/suggestt1"
#define DHCP6C_SUGGEST_T2_FILE                   "/var/wan/%s/suggestt2"

#define DHCP6C_ACCEPT_RA_FILE                "/var/wan/%s/acceptra"

#define ACCEPT_RA_FILE_PATH                 "/proc/sys/net/ipv6/conf/%s/accept_ra"

#define ACCEPT_RA_AUTOCONF_FILE_PATH        "/proc/sys/net/ipv6/conf/%s/autoconf"


/* start of by d00107688 2010-01-26 ����֧���Ƿ��������Ĭ��·�� */
#define ACCEPT_RA_DLF_ROUTE_FILE_PATH       "/proc/sys/net/ipv6/conf/%s/accept_ra_defrtr"
/* end of by d00107688 2010-01-26 ����֧���Ƿ��������Ĭ��·�� */

/* DHCP ǰ׺��Ϣ */
#define WAN_IAPD_PREFIX_FILE                   "/var/wan/%s/iapdprefix6"

/*prefix ValidLifeTime ��Ϣ*/
#define WAN_IAPD_VALID_TIME_FILE                   "/var/wan/%s/iapdvalidtime6"

/*prefix PreferredLifeTime ��Ϣ*/
#define WAN_IAPD_PREFER_TIME_FILE                   "/var/wan/%s/iapdprefertime6"

/*iana addr  ��Ϣ*/
#define WAN_IANA_ADDR_FILE                   "/var/wan/%s/ianaaddr6"

/*iana addr ValidLifeTime ��Ϣ*/
#define WAN_IANA_VALID_TIME_FILE                   "/var/wan/%s/ianavalidtime6"

/*iana addr PreferredLifeTime ��Ϣ*/
#define WAN_IANA_PREFER_TIME_FILE                   "/var/wan/%s/ianaprefertime6"

/*ra prefix��Ϣ*/
#define WAN_RA_PREFIX_FILE                     "/var/wan/%s/raprefix6"

/*ra prefix ValidLifeTime ��Ϣ*/
#define WAN_RA_PREFIX_VALID_TIME_FILE                   "/var/wan/%s/raprefixvalidtime6"

/*ra prefix PreferredLifeTime ��Ϣ*/
#define WAN_RA_PREFIX_PREFER_TIME_FILE                   "/var/wan/%s/raprefixprefertime6"

/*RA��� Managed Flag*/
#define WAN_RA_M_FLAG_FILE                   "/var/wan/%s/M_flag"
/*RA��� Other Flag*/
#define WAN_RA_O_FLAG_FILE                   "/var/wan/%s/O_flag"
/*RA��� router prefer flag*/
#define WAN_RA_ROUTER_PREFER_FLAG_FILE                   "/var/wan/%s/rarouterprefer_flag"
/*RA��� router life time*/
#define WAN_RA_ROUTER_LIFETIME_FILE                   "/var/wan/%s/rarouterlifetime"

/*ipv6 ״̬��Ϣ*/
#define WAN_STATUS6_FILE                    "/var/wan/%s/status6"

/* Ĭ������ */
#define WAN_GW6_FILE                        "/var/wan/%s/gateway6"

/* IP��ַ�б� */
#define WAN_IPADDR6_FILE                    "/var/wan/%s/ipaddr6"

/* DHCP DNS �ļ� */
#define WAN_DHCPC6_DNS                      "/var/wan/%s/dhcpdns6"

/* DHCP6C DUID�ļ� */
#define WAN_DHCP6C_DUID                      "/var/wan/dhcp6cduid"
/*Begin:Added by luokunling 2012/1/24:��¼br0 Gua��ַ,ʹ�����ǹ���ҳ����*/
#define WAN_INTERFACE_LLA                      "/var/wan/%s/lla"
/*End:Ended by luokunling 2012/1/24*/
#define DHCP6C_PIDFILE                      "/var/wan/%s/dhcp6c.pid"

#define GRE_PREFIX "gre"

//!���ò�������
#define DHCP6C_PARA_LEN  128

//!option ����
#define DHCP6C_OPTION_LEN  256

#ifdef SUPPORT_ATP_TR181
typedef struct tagDHCP6C_INFO_ST
{
    struct tagDHCP6C_INFO_ST   *pstNext;

    unsigned int  ulInstId;				/*!<���ʵ����. */
    
    unsigned int bEnable;                 /*!<Enables or disables this Client entry. */

    char acInterfacePath[DCHP6C_PATH_LEN] ;                            /* �ӿ�·���� */

    char acInterface[DCHP6C_IFC_LEN];                            /* �ӿ��� */

    char acStatus[DCHP6C_IFC_LEN] ;                            /*  */

    char acDUID[DHCP6C_PARA_LEN];    /*!< cliend duid */

    unsigned int bRequestAddresses;                 /*!< Enables or disables inclusion of the Identity Association (IA) for Non-Temporary Address */

    unsigned int bRequestPrefixes;                       /*!< Enables or disables inclusion of the Identity Association (IA) for Prefix Delegation */

    unsigned int bRapidCommit;                       /*!< Enables or disables inclusion of the Rapid Commit option  */

    unsigned int bRenew;                       /*!<When set to true, the Client will renew its DHCPv6-supplied information   */ 

    int   lSuggestedT1;                 /*!<T1 value, in seconds, that the client SHOULD use when sending IA optionsn   */ 

    int   lSuggestedT2;                  /*!<T2 value, in seconds, that the client SHOULD use when sending IA optionsn   */ 

    char acSupportedOptions[DHCP6C_PARA_LEN];    /*!< The options that the client is able to process in server responses   */ 

    char acRequestedOptions[DHCP6C_PARA_LEN];    /*!<An ordered list of the top-level options (i.e. not encapsulated options) that the client will explicitly request from the server.   */ 
  
} DHCP6C_INFO_ST;

struct stDhcp6cServerInfo
{
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< �ӿ���  */
    VOS_CHAR          acSourceAddress[DHCP6C_PARA_LEN];        /*!< server ip  */
    VOS_CHAR          acDUID[DHCP6C_PARA_LEN];    /*!< server duid */
    VOS_CHAR          acInformationRefreshTime[DHCP6C_PARA_LEN];    /*!< OPTION 32*/
};

struct stDhcp6cOpt
{
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< �ӿ���  */
    VOS_UINT32        ulOptTag;        /*!< option  code */
    VOS_UINT8         acOptValue[DHCP6C_OPTION_LEN];        /*!< option  value  */
    VOS_CHAR          acServer[DHCP6C_PARA_LEN];    /*!< option server */
};
#endif

//!\brief dhcp6c��Ϣ�ṹ��
/*!
 * Я���ӿ����Լ������в�������Ϣ��dhcp6c����
 */
struct stDhcp6cData
{
    VOS_UINT32        ulPid;                 /*!< dhcp6c pid */
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< �ӿ���  */
    VOS_CHAR          inputoptions[DHCP6C_PARA_LEN];    /*!< ���ò�����Ϣ */
#ifdef SUPPORT_ATP_TR181
    DHCP6C_INFO_ST stDhcp6cInfo;
#endif	
};

struct stDhcp6cStatus
{
    VOS_UINT32          ulChange;                 /*!< dhcp6c change */
    VOS_UINT32          ulStatus;                 /*!< dhcp6c Status */
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< �ӿ���  */
};

enum ATP_DHCP6C_MSGTYPES_EN
{
    ATP_MSG_DHCP6C_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DHCP6CCMS),    //����DHCP6C����      
    ATP_MSG_DHCP6C_STOP,         //ֹͣDHCP6C����
    ATP_MSG_DHCP6C_INITOVER,
#ifdef SUPPORT_ATP_TR181
    ATP_MSG_DHCP6C_GET_SERVER_INFO,   
    ATP_MSG_DHCP6C_GET_OPTION_INFO,
    ATP_MSG_IPV6_STATUS_CHANGE,
    ATP_MSG_DHCP6C_SEND_OPTION_INFO,    
#endif
} ;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
