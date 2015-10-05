#ifndef __ATP__PPPCMSGTYPES_H__
#define __ATP__PPPCMSGTYPES_H__

#define ATP_CBBID_PPPC_NAME "pppc"


/* 接口名长度 */
#define ATP_PPPC_IFC_LEN  32
/* 接口名长度 */
#define ATP_PPPC_DEV_LEN  32
/* 用户名长度 */
#define ATP_PPPC_USER_LEN  68
/* 密码长度 */
#define ATP_PPPC_PASSWD_LEN  68
/* 绑定列表长度 */
#define ATP_PPPC_BIND_LEN  64
/*AT拨号指令长度*/
#define ATP_PPPC_DIALNUMBER_LEN  32
/*APN设置长度*/
#define ATP_PPPC_SETAPN_LEN  32
/* DNS列表长度 */
#define ATP_PPPC_DNS_LEN  64
/*start of ATP 2008.01.17 c60023298 修改 for AU4D00330 文件信息长度与读取长度不一致 */
/* 最后一次连接失败信息长度 */
#define ATP_PPPC_ERROR_LEN  64
/*end of ATP 2008.01.17 c60023298 修改 for AU4D00330 文件信息长度与读取长度不一致 */
/* IP地址长度 */
#define ATP_PPPC_IPADDR_LEN  16

#define WAN_PPP_SESSION_LEN 32

#define ATP_PPPOE_SERVICENAME_LEN  256
#define ATP_PPPOE_ACNAME_LEN  256

/* MAC地址长度 */
#define ATP_PPPC_MAC_LEN  32
/* 服务类型 */
#define ATP_PPPC_SERVICELIST_LEN  32



/* 错误码 */
typedef enum
{
    ATP_PPPC_OK = 0,
    /* 定义PPPC错误码起始值 */
    ATP_ERR_PPPC_START = 0x0820,
    /* 参数校验错误 */
    ATP_ERR_PPPC_CHECK_FAILED,
    /* 配置文件错误 */
    ATP_ERR_PPPC_CONFIG_FAILED,
    /* 进程已启动 */
    ATP_ERR_PPPC_ALREADY_EXIST,
    /* 进程不存在 */
    ATP_ERR_PPPC_NO_EXIST,
}ATP_ERR_PPPC_EN;

/* 连接触发，需要与WAN模块保持一致 */
typedef enum
{
    /* 等待 */
    ATP_PPPC_TRIGGER_IDLE = 0,
    /* 断线 */
    ATP_PPPC_TRIGGER_DISCONNECTING = 1,
    /* 手动 */
    ATP_PPPC_TRIGGER_MANUAL = 2,
    /* 自动 */
    ATP_PPPC_TRIGGER_ALWAYSON = 3,
    /* 按需拨号 */
    ATP_PPPC_TRIGGER_ONDEMAND = 4,
    /* 最大值 */
    ATP_PPPC_TRIGGER_MAX
}ATP_PPPC_TRIGGER_EN;

/* PPPC连接状态 */
typedef enum
{
    /* 未初始化 */
    ATP_PPPC_STATUS_UNCONFIGURED,
    /* 正在连接 */
    ATP_PPPC_STATUS_CONNECTING,
    /* 正在认证 */
    ATP_PPPC_STATUS_AUTHENTICATING,
    /* 已连接 */
    ATP_PPPC_STATUS_CONNECTED,
    /* 未连接 */
    ATP_PPPC_STATUS_PENDING,
    /* 正在断开 */
    ATP_PPPC_STATUS_DISCONNECTING,
    /* 已断开 */
    ATP_PPPC_STATUS_DISCONNECTED 
}ATP_PPPC_STATUS_EN;

/* 协议类型 */
typedef enum
{
    /* PPPOE拨号 */
    ATP_PPPC_PROTOCOL_PPPOE,
    /* PPPOA的LLC模式 */
    ATP_PPPC_PROTOCOL_PPPOA_LLC,
    /* PPPOA的VC模式 */
    ATP_PPPC_PROTOCOL_PPPOA_VC,
    /* PPP拨号 */
    ATP_PPPC_PROTOCOL_PPP_DIALUP,
    /* PPPOE2A */
    ATP_PPPC_PROTOCOL_PPPOE2A,
    /*L2TP里面的PPP拨号*/
    ATP_PPPC_PROTOCOL_L2TP,
}ATP_PPPC_PROTOCOL_EN;

/* 认证方式 */
typedef enum
{
    /* AUTO */
    ATP_PPPC_AUTH_AUTO,
    /* PAP */
    ATP_PPPC_AUTH_PAP,
    /*CHAP */
    ATP_PPPC_AUTH_CHAP,
}ATP_PPPC_AUTH_EN;

#ifdef SUPPORT_ATP_IPV6
/* IPV6地址配置方式 */
typedef enum
{
    /* Disable */
    ATP_PPPC_IPV6_ADDR_TYPE_DISABLE,
    /* SLAAC */
    ATP_PPPC_IPV6_ADDR_TYPE_SLAAC,
    /* DHCP */
    ATP_PPPC_IPV6_ADDR_TYPE_DHCP,
    /* Static */
    ATP_PPPC_IPV6_ADDR_TYPE_STATIC,
}ATP_PPPC_IPV6_ADDR_TYPE_EN;

/* IPV6地址请求选项 */
typedef enum
{
    /* IANA + IAPD */
    ATP_PPPC_IPV6_ADDR_OPTION_BOTH,
    /* IANA */
    ATP_PPPC_IPV6_ADDR_OPTION_IANA,
    /* IAPD */
    ATP_PPPC_IPV6_ADDR_OPTION_IAPD,
    /* 自适应，暂不支持 */
    ATP_PPPC_IPV6_ADDR_OPTION_AUTO,
}ATP_PPPC_IPV6_ADDR_OPTION_EN;
#endif


/* PPPOA QOS */
typedef struct
{
    unsigned int ulATMQoS;
    unsigned int ulATMPeakCellRate;
    unsigned int ulATMMaximumBurstSize;
    unsigned int ulATMSustainableCellRate;
}ATP_PPPC_ATMQOS_ST;

/* 配置参数结构 */
typedef struct
{
    /* PPP接口名 */
    char acInterface[ATP_PPPC_IFC_LEN];
    /* PPP接口名 */
    char acDevice[ATP_PPPC_DEV_LEN];
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
    char acDevice_bak[ATP_PPPC_DEV_LEN];
#endif
    /* 拨号用户名 */
    char acUserName[ATP_PPPC_USER_LEN];
    /* 密码 */
    char acPassWord[ATP_PPPC_PASSWD_LEN];
    /* 绑定列表 */
    char acBindList[ATP_PPPC_BIND_LEN];
    /* 绑定功能标识 */
    unsigned char ucBindEnable;
    /* 代理功能标识 */
    unsigned char ucProxyEnable;
    /* 代理最大用户数 */
    unsigned char ucProxyNum;
    /* 协议类型 */
    unsigned char ucProtocol;
    /* 调试开关 */
    unsigned char ucDebugEnable;
    /* 保留 */
    unsigned char aucReserved[3];
    /* PPPOA QOS */
    ATP_PPPC_ATMQOS_ST stAtmQos;
    char acPppSessid[WAN_PPP_SESSION_LEN];
	/************ATPV1R2C03 下发进程新增写配置项****************/
    /*认证方式*/
    unsigned int ulAuth;
    /*按需拨号空闲时间*/
    unsigned int ulIdleTime;
         /*B300 新增*/
    unsigned int ulRandomDialInterval;  //PPPOE开始拨号前随机等待时间的最大值。
    unsigned int ulRetryDialInterval;  //PPPOE拨号失败后等待的时间间隔。
    unsigned int ulRetrySendInterval;  //PPPOE重发报文的时间间隔
    unsigned int ulKeepAliveInterval;  //LCP发送时间间隔
    unsigned int ulRetryTimes; //LCP连续发送失败次数修改
#ifdef SUPPORT_ATP_WANUMTS
	/*AT拨号指令*/
	char acDialNumber[ATP_PPPC_DIALNUMBER_LEN];
	/*设置APN*/
	char acSetApn[ATP_PPPC_SETAPN_LEN];
#endif
    unsigned int ulMaxMru;
#ifdef SUPPORT_ATP_MTU
    /* 最大MTU */
    unsigned int ulMaxMtu;
#endif
    int iReqTime;
     /************ATPV1R2C03 下发进程新增写配置项****************/
     /*VHG需求 pppoe service name*/
    char acPppoeServiceName[ATP_PPPOE_SERVICENAME_LEN];
    char acPppoeAcName[ATP_PPPOE_ACNAME_LEN];
    /* 以太网虚拟PPPOA的MAC地址 */
    char acPppoe2aMacAddr[ATP_PPPC_MAC_LEN];
    /* 以太网虚拟PPPOA的会话ID */
    unsigned int ulPppoe2aSid;
#ifdef SUPPORT_ATP_IPV6
    /* 是否启用IPv4功能 */
    unsigned int ulIpv4;
    /* 是否启用IPv6功能 */
    unsigned int ulIpv6;
    /* IPv6请求地址类型，如IANA或IAPD */
    unsigned int ulManagedAddress;
#endif
    unsigned int ulPvcSearchTime;
    char acServiceList[ATP_PPPC_SERVICELIST_LEN];
#ifdef SUPPORT_ATP_PPP_STATICIP
    char acRequestIP[ATP_PPPC_IPADDR_LEN];
#endif
#ifdef SUPPORT_ATP_DT_WAN_PAGE     
    char acRequestPriDns[ATP_PPPC_IPADDR_LEN]; 
    char acRequestSekDns[ATP_PPPC_IPADDR_LEN]; 
    unsigned int ulEffDemandFlag;
    unsigned int ulIspFlag;
    unsigned int ulDefaultUPFlag;
#endif
}ATP_PPPC_CONFIG_ST;

#define PPPC_IFC_LEN (64)
typedef struct tagATP_PPPC_STATUSCHG_MSG_ST
{
    VOS_UINT32 ulStatus;
    VOS_CHAR   acIntfName[PPPC_IFC_LEN];
}ATP_PPPC_STATUSCHG_MSG_ST;

enum ATP_MSGTYPES_EN
{
    ATP_MSG_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_PPPC),
    ATP_MSG_START_DHCP6C_PRO,
    ATP_MSG_ONDEMAND_READY_ROUTE,
    ATP_MSG_PPPC_STATUS_CHANGE,
    ATP_MSG_CMS_CONFIRM_VLAN_INF,
    ATP_MSG_PPPC_VLAN_FORCE_CHANGE,
/* <DTS2012082701764 w00211169 2012-9-4 begin */
    ATP_MSG_ONDEMAND_IPV6_DNS_TRIGGER,
/* DTS2012082701764 w00211169 2012-9-4 end> */
} ;

#endif // End of __ATP__MSGTYPES_H__

