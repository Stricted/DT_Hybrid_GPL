/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file hybridmsgtypes.h
  *hybrid common define for cms&service
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2013-4-18
*/
#ifndef __ATP_HYBRID_MSGTYPES_H__
#define __ATP_HYBRID_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define SUPPORT_ATP_HYBRID_DEMO 1/*only use in demo version*/

#define ATP_CBBID_HYBRID_NAME "hybrid"
#define ATP_WAN_IF_LEN 32
#define ATP_WAN_ADDR_STR_LEN 40

#define HYBRID_LTE_INTF_NAME "rmnet0"
#define HYBRID_DSL_INTF_NAME "ppp256"
#define HYBRID_DSL_TUNNEL_NAME "gre1"
#define HYBRID_LTE_TUNNEL_NAME "gre2"
#define HYBRID_BONDING_TUNNEL_NAME "gre"
#define INTERFACE_NAME_MAX_NAME 64
#define HYBRID_T2_ADDR_PATH "/var/hybrid/T2"
#define HYBRID_PPP_DATA_CHG_PATH "/var/hybrid/pppdatachg"
#define HYBRID_CLIENT_INFO "/var/hybrid/info"
/*用于标记是否有filterlist没有执行完，没有执行完时，不接受新的filterlist*/
#define HYBRID_FILTER_LIST_EFFECT_FLAG "/var/hybrid/filter-effecting"
#define HYBRID_BONDING_DHCP_INTF_PATH "/var/hybrid/dhcpintf" 
/*Start of Protocol 2014-5-5 22:4 for HOMEGW-17271-dsl默认账号下不发dsl隧道建立请求 by t00189036*/
/*用于标记是否是默认账号，供hybrid模块使用*/
#define HYBRID_DSL_DEFAULT_USER_FLAG "/var/hybrid/default-dsl-user"
/*End of Protocol 2014-5-5 22:4 for by t00189036*/

#define HYBRID_DIR_STATIC_IP_PATH      "/var/hybrid/staticIP"  //save option121 IP
/*Start of ATP 2014-4-29 for UDK0000351739 by y00250667*/
#define HYBRID_DIR_ACSPATH "/var/ACSipaddr"
/*End of ATP 2014-4-29 for UDK0000351739 by y00250667*/

#define HYBRID_GRE_PORT 47
#define ATP_STRING_LEN_64 64
#define ATP_STRING_LEN_1024 1024

#define ATP_ETH_HLEN            14
#define ATP_IP_HDRLEN           20  /* bytes */
#define ATP_IP6_HDRLEN         40  /* bytes */
/*当前gre内部长度为8，后续支持key(session ID)后要改为12*/
#define ATP_GRE_HDRLEN        12  /* bytes */
#define ATP_PPP_HDRLEN        8   /* bytes */
#define ATP_L2TP_HDRLEN 6
#define ATP_UDP_HDRLEN 8
#define TUNNEL_PPP_MIN_MTU 1440
#define TUNNEL_LTE_MIN_MTU 1436
#define LEAST_COLON_NUM_IN_IPV6     2
#define FILTER_MASK_MAXNUM 32

/*Start of Protocol 2014-5-23 15:57 for HOMEGW-17282:添加blacklist路由 by t00189036*/
/*fierwall mark*/
#define HYBRID_TELEKOM_LIST_FWMARK  0x10000000
#define HYBRID_USER_LIST_FWMARK     0x20000000
/*Start of Protocol 2014-4-15 16:54 for DTS2014032711111:DSCP标记的telekomlist有较高的优先级没有实现 by t00189036*/
#define HYBRID_TELEKOM_DSCP_LIST_FWMARK   0x40000000
/*End of Protocol 2014-4-15 16:54 for by t00189036*/
#define HYBRID_BLACK_LIST_FWMARK 0x80000000
#define HYBRID_FWMARK_MASK          0xf0000000
/*End of Protocol 2014-5-23 15:57 for by t00189036*/

/*hybrid link type*/
typedef enum 
{
    ATP_HYBRID_LINK_TYPE_DSL,
    ATP_HYBRID_LINK_TYPE_LTE,
    
    /*add more type befor here if you need*/
    ATP_HYBRID_LINK_TYPE_MAX_NUM,/*max link type num*/
} ATP_HYBRID_LINK_TYPE_EN;

/*hybrid link type*/
typedef enum 
{
    ATP_HYBRID_WAN_CHG_TYPE_UP,
    ATP_HYBRID_WAN_CHG_TYPE_DOWN,
    ATP_HYBRID_WAN_CHG_TYPE_V6_UP,
    ATP_HYBRID_WAN_CHG_TYPE_V6_DOWN,
    ATP_HYBRID_WAN_CHG_TYPE_V6_UPDATE,
} ATP_HYBRID_WAN_CHG_TYPE_EN;

/*hybrid show info msg type*/
typedef enum
{
    ATP_HYBRID_SHOWINFO_DEFAULT,
    ATP_HYBRID_SHOWINFO_BYPASS,
    ATP_HYBRID_SHOWINFO_RTT,
    ATP_HYBRID_SHOWALL,
    ATP_HYBRID_SHOWINFO_IDLEHELLO,
} ATP_HYBRID_SHOWINFO_TYPE_EN;

typedef enum  
{
    FILTER_STACK_BOTH,
    FILTER_STACK_IPV4,
    FILTER_STACK_IPV6,
}FILTER_STACK_IP_TYPE_EN;


typedef struct
{
    VOS_CHAR acAddr[ATP_WAN_ADDR_STR_LEN];
    VOS_UINT8 ucLen;
}ATP_IPV6_PREFIX_ST;

typedef struct
{
    ATP_HYBRID_LINK_TYPE_EN     enLinkType;
    ATP_HYBRID_WAN_CHG_TYPE_EN  enChgType;
    VOS_CHAR                    acIPAddr[ATP_WAN_ADDR_STR_LEN];
    ATP_IPV6_PREFIX_ST          stPrefix;
} ATP_HYBRID_WAN_CHG_INFO_ST;

/*Start add by yaoxianzhi DTS2013121810687 20140103 【Hybrid Bonding新需求】隧道MSS能通过TR069可配置*/	
#define TUNNEL_GRE_MAX_MTU 1500

typedef struct
{
    VOS_CHAR    acIntfName[ATP_WAN_IF_LEN];
    VOS_UINT32  ulMTU;
} ATP_HYBRID_MTU_CHG_INFO_ST;
/*End add by yaoxianzhi DTS2013121810687 20140103 【Hybrid Bonding新需求】隧道MSS能通过TR069可配置*/

enum 
{   
    ATP_HYBRID_MSG_TYPE_WAN_CHG,
    ATP_HYBRID_MSG_TYPE_LTE_DHCP_CHG,
    ATP_HYBRID_MSG_TYPE_LTE_DHCPV6_CHG,
    ATP_HYBRID_MSG_TYPE_SHOW,
    ATP_HYBRID_MSG_TYPE_UPLINK_RATE,
    ATP_HYBRID_MSG_TYPE_CHG_HELLO_CHECK,  
    ATP_HYBRID_MSG_TYPE_GET_INFO,
    ATP_HYBRID_MSG_TYPE_TEST,
    ATP_MSG_HYBRID_TUNNEL_UP,
    ATP_MSG_HYBRID_TUNNEL_DOWN,
    ATP_MSG_HYBRID_SAVE_STATS,
    ATP_HYBRID_MSG_TYPE_TEARDOWN_SWITCH,
    ATP_HYBRID_MSG_TYPE_PREFIX_DELEGATE,
    ATP_HYBRID_MSG_TYPE_USER_GROUP,
    ATP_HYBRID_MSG_TYPE_SET_REQ_TIMEOUT,
    ATP_HYBRID_MSG_TUNNEL6_UP,
    ATP_HYBRID_MSG_TUNNEL6_DOWN,
    ATP_HYBRID_MSG_TYPE_WAN_WILL_DOWN,
	/*Start add by yaoxianzhi DTS2013121810687 20140103 【Hybrid Bonding新需求】隧道MSS能通过TR069可配置*/	
    ATP_HYBRID_MSG_TYPE_SET_MSS,
	/*End add by yaoxianzhi DTS2013121810687 20140103 【Hybrid Bonding新需求】隧道MSS能通过TR069可配置*/
	ATP_HYBRID_MSG_TYPE_DHCP6C_INTF_CHANGE,
	ATP_HYBRID_MSG_TYPE_RESET_RADVD,
    /* Start of 2014-4-14 for DTS2014032512399 Idle hello开发 by y00250610 */
	ATP_HYBRID_MSG_TYPE_CHAGE_IDLE_HELLO_ENABLE,
    ATP_HYBRID_MSG_TYPE_TEST_UPDATEIDLEELLO,
    ATP_HYBRID_MSG_TYPE_TEST_STARTCHGIDLEHELLO,
    ATP_HYBRID_MSG_TYPE_TEST_IDLEHELLOTRAFFIC,
    ATP_HYBRID_MSG_TYPE_TEST_CHGHELLOSTATUS,
    /* End of 2014-4-14 for DTS2014032512399 Idle hello开发 by y00250610 */
};

enum 
{   
    ATP_HYBRID_STATUS_OFFLINE,
    ATP_HYBRID_STATUS_IDLEOUT,
    ATP_HYBRID_STATUS_ACTIVING,    
    ATP_HYBRID_STATUS_ONLINE,
    ATP_HYBRID_STATUS_ERROR,
};

enum 
{   
    ATP_HYBRID_BONDINGMODE_NONTUNNEL,
    ATP_HYBRID_BONDINGMODE_DSL,
    ATP_HYBRID_BONDINGMODE_LTE,    
    ATP_HYBRID_BONDINGMODE_LTEDSL,    
};

/*fliter list type*/
typedef enum tagATP_FILTER_LIST_TYPE_EN
{
    FILTER_TYPE_UNKNOWN =          0,
    FILTER_TYPE_FQDN =             1,
    FILTER_TYPE_DSCP =             2,
    FILTER_TYPE_DST_PORT =         3,
    FILTER_TYPE_DST_IP =           4,
    FILTER_TYPE_DST_IPPORT =       5,
    FILTER_TYPE_SRC_PORT =         6,
    FILTER_TYPE_SRC_IP =           7,
    FILTER_TYPE_SRC_IPPORT =       8,
    FILTER_TYPE_MAC =              9,
    FILTER_TYPE_PROTOCOL =         10,
    FILTER_TYPE_SRC_IPRANGE =      11,
    FILTER_TYPE_DST_IPRANGE =      12,
    FILTER_TYPE_SRC_IPRANGEPORT =  13,   
    FILTER_TYPE_DST_IPRANGEPORT =  14,  
    FILTER_TYPE_MIX_IPPORT    =  15,
}ATP_FILTER_LIST_TYPE_EN;


typedef struct
{
    VOS_UINT8 ucEnable;
    VOS_UINT8 ucStatus;     
    VOS_UINT8 ucBondingMode; 
    VOS_UINT8 ucRTTSwitch;
    VOS_UINT32 ulCurrentRTT;
    VOS_UINT32 ulRTTthreshold;
    VOS_UINT32 ulBWCalculationInterval;
    VOS_UINT32 ulAvailableBW;
    VOS_UINT32 ulLastErrorInfo;
    VOS_CHAR aucBondingServerIPAddress[ATP_STRING_LEN_64];
    /* Start of 2014-4-14 for DTS2014032512399 Idle hello开发 by y00250610 */
    VOS_CHAR auLteHelloState[ATP_STRING_LEN_64]; /*Hello状态*/
    VOS_UINT32 ulIdleHelloInterval;             /*Idle Hello 间隔*/
    VOS_UINT32 ulHelloInterval;                 /*Hello 间隔*/
    VOS_UINT32 ulHelloRetryTimes;               /*Hello 重试次数*/ 
    VOS_UINT32 ulTrafficMonitorInterval;        /*流量监测间隔*/ 
    /* End of 2014-4-14 for DTS2014032512399 Idle hello开发 by y00250610 */
    VOS_UINT32 ulTunnelVerification;        /*收到的Verification数量*/
    VOS_UINT32 ulTunnelVerificationACK;     /*发送的Verification ACK数量*/
}ATP_HYBRID_DEVICE_SERVICES_BONDING_ST;

enum 
{   
    ATP_HYBRID_ERRORINFO_NONE,
    ATP_HYBRID_ERRORINFO_NOACCESSSUPPORT_SIMCARD,    
    ATP_HYBRID_ERRORINFO_NOACCESSSUPPORT_PPPDATA,
    ATP_HYBRID_ERRORINFO_IDMISMATCH,
};

//Filter list TLV格式
typedef struct tagATP_GRE_FILTER_LIST_ST
{
    VOS_UINT16 usType;  
	VOS_UINT16 usLen;
	VOS_UINT16 usEnable;
	VOS_UINT16 usDesLen;
	VOS_CHAR   acDescription[64];
	VOS_CHAR   acFilterValue[300];
}ATP_GRE_FILTER_LIST_ST;

#define FILTER_LIST_NUM_MAX 480
#define ATP_UTIL_GVAR_FILTERLIST_BUFF (256 * 1024) //256k
typedef ATP_GRE_FILTER_LIST_ST HYBRID_FILTERLIST_ARRAY[FILTER_LIST_NUM_MAX];//存储所有的Filter List

typedef enum 
{
    ATP_UTIL_GVAR_HYBRID_INIT = ATP_UTIL_GVAR_MOD_DEF(ATP_UTIL_GVAR_MOD_HYBRID),
    ATP_UTIL_GVAR_HYBRID_FILTERLIST,
}ATP_UTIL_GVAR_HYBRID_INDEX;

enum ATP_HYBRID_MSGTYPES_EN
{
    ATP_MSG_HYBRID_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_HYBRID),     
    ATP_MSG_HYBRID_FILTER_LIST,
} ;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif // End of __ATP_HYBRID_MSGTYPES_H__

