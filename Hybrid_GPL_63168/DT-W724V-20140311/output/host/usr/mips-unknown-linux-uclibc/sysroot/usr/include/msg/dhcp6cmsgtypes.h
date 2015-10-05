/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6ccmsmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
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

/* WAN连接地址类型长度 */
#define DCHP6C_ADDRTYPE_LEN 8

/* WAN路径最大长度 */
#define DCHP6C_PATH_LEN 128

/* WAN接口名最大长度 */
#define DCHP6C_IFC_LEN  32

#define DHCP6C_REQADDR_FLAG_FILE                   "/var/wan/%s/requestaddr"
#define DHCP6C_REQPREFIX_FLAG_FILE                   "/var/wan/%s/requestprefix"
#define DHCP6C_RAPIDCOMMIT_FLAG_FILE                   "/var/wan/%s/rapidcommit"
#define DHCP6C_SUGGEST_T1_FILE                   "/var/wan/%s/suggestt1"
#define DHCP6C_SUGGEST_T2_FILE                   "/var/wan/%s/suggestt2"

#define DHCP6C_ACCEPT_RA_FILE                "/var/wan/%s/acceptra"

#define ACCEPT_RA_FILE_PATH                 "/proc/sys/net/ipv6/conf/%s/accept_ra"

#define ACCEPT_RA_AUTOCONF_FILE_PATH        "/proc/sys/net/ipv6/conf/%s/autoconf"


/* start of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */
#define ACCEPT_RA_DLF_ROUTE_FILE_PATH       "/proc/sys/net/ipv6/conf/%s/accept_ra_defrtr"
/* end of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */

/* DHCP 前缀信息 */
#define WAN_IAPD_PREFIX_FILE                   "/var/wan/%s/iapdprefix6"

/*prefix ValidLifeTime 信息*/
#define WAN_IAPD_VALID_TIME_FILE                   "/var/wan/%s/iapdvalidtime6"

/*prefix PreferredLifeTime 信息*/
#define WAN_IAPD_PREFER_TIME_FILE                   "/var/wan/%s/iapdprefertime6"

/*iana addr  信息*/
#define WAN_IANA_ADDR_FILE                   "/var/wan/%s/ianaaddr6"

/*iana addr ValidLifeTime 信息*/
#define WAN_IANA_VALID_TIME_FILE                   "/var/wan/%s/ianavalidtime6"

/*iana addr PreferredLifeTime 信息*/
#define WAN_IANA_PREFER_TIME_FILE                   "/var/wan/%s/ianaprefertime6"

/*ra prefix信息*/
#define WAN_RA_PREFIX_FILE                     "/var/wan/%s/raprefix6"

/*ra prefix ValidLifeTime 信息*/
#define WAN_RA_PREFIX_VALID_TIME_FILE                   "/var/wan/%s/raprefixvalidtime6"

/*ra prefix PreferredLifeTime 信息*/
#define WAN_RA_PREFIX_PREFER_TIME_FILE                   "/var/wan/%s/raprefixprefertime6"

/*RA里的 Managed Flag*/
#define WAN_RA_M_FLAG_FILE                   "/var/wan/%s/M_flag"
/*RA里的 Other Flag*/
#define WAN_RA_O_FLAG_FILE                   "/var/wan/%s/O_flag"
/*RA里的 router prefer flag*/
#define WAN_RA_ROUTER_PREFER_FLAG_FILE                   "/var/wan/%s/rarouterprefer_flag"
/*RA里的 router life time*/
#define WAN_RA_ROUTER_LIFETIME_FILE                   "/var/wan/%s/rarouterlifetime"

/*ipv6 状态信息*/
#define WAN_STATUS6_FILE                    "/var/wan/%s/status6"

/* 默认网关 */
#define WAN_GW6_FILE                        "/var/wan/%s/gateway6"

/* IP地址列表 */
#define WAN_IPADDR6_FILE                    "/var/wan/%s/ipaddr6"

/* DHCP DNS 文件 */
#define WAN_DHCPC6_DNS                      "/var/wan/%s/dhcpdns6"

/* DHCP6C DUID文件 */
#define WAN_DHCP6C_DUID                      "/var/wan/dhcp6cduid"
/*Begin:Added by luokunling 2012/1/24:记录br0 Gua地址,使其现是工程页面上*/
#define WAN_INTERFACE_LLA                      "/var/wan/%s/lla"
/*End:Ended by luokunling 2012/1/24*/
#define DHCP6C_PIDFILE                      "/var/wan/%s/dhcp6c.pid"

#define GRE_PREFIX "gre"

//!配置参数长度
#define DHCP6C_PARA_LEN  128

//!option 长度
#define DHCP6C_OPTION_LEN  256

#ifdef SUPPORT_ATP_TR181
typedef struct tagDHCP6C_INFO_ST
{
    struct tagDHCP6C_INFO_ST   *pstNext;

    unsigned int  ulInstId;				/*!<结点实例号. */
    
    unsigned int bEnable;                 /*!<Enables or disables this Client entry. */

    char acInterfacePath[DCHP6C_PATH_LEN] ;                            /* 接口路径名 */

    char acInterface[DCHP6C_IFC_LEN];                            /* 接口名 */

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
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< 接口名  */
    VOS_CHAR          acSourceAddress[DHCP6C_PARA_LEN];        /*!< server ip  */
    VOS_CHAR          acDUID[DHCP6C_PARA_LEN];    /*!< server duid */
    VOS_CHAR          acInformationRefreshTime[DHCP6C_PARA_LEN];    /*!< OPTION 32*/
};

struct stDhcp6cOpt
{
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< 接口名  */
    VOS_UINT32        ulOptTag;        /*!< option  code */
    VOS_UINT8         acOptValue[DHCP6C_OPTION_LEN];        /*!< option  value  */
    VOS_CHAR          acServer[DHCP6C_PARA_LEN];    /*!< option server */
};
#endif

//!\brief dhcp6c消息结构体
/*!
 * 携带接口名以及命令行参数等信息给dhcp6c进程
 */
struct stDhcp6cData
{
    VOS_UINT32        ulPid;                 /*!< dhcp6c pid */
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< 接口名  */
    VOS_CHAR          inputoptions[DHCP6C_PARA_LEN];    /*!< 配置参数信息 */
#ifdef SUPPORT_ATP_TR181
    DHCP6C_INFO_ST stDhcp6cInfo;
#endif	
};

struct stDhcp6cStatus
{
    VOS_UINT32          ulChange;                 /*!< dhcp6c change */
    VOS_UINT32          ulStatus;                 /*!< dhcp6c Status */
    VOS_CHAR          acInterface[DCHP6C_IFC_LEN];        /*!< 接口名  */
};

enum ATP_DHCP6C_MSGTYPES_EN
{
    ATP_MSG_DHCP6C_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DHCP6CCMS),    //启动DHCP6C进程      
    ATP_MSG_DHCP6C_STOP,         //停止DHCP6C进程
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
