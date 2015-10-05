
/* <DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2008, 华为技术有限公司.
  文件名  : wds.h
  作  者  : mayang 00100263
  版  本  : 001
  创建日期: 2008-4-22
  完成日期: 
  功能描述: WDS进程头文件
      
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    mayang      2008-4-22    001     init
    mayang      2008-5-7     002     1. 删除状态机队列相关声明
    mayang      2008-5-15    003     1. 全局变量挪到C文件中
    gKF34687   2010-11-4     004     b683 移植
  备注: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>      <时间>        <问题单号>             <简要描述>
l00205975     2012/07/17    DTS2012072100113        【按键】[B890]V100R001C994B101(Router):单板的Reset按键功能无效
z00185914           2013/01/28          DTS2013012808057		【拨号】新建ndis拨号文件夹 
z00185914    2013/02/17    DTS2013021703454 【拨号】优化ndis拨号代码 
z00185914    2013/04/28    DTS2013042003518    【网络选择】单板拨号成功后，网络模式从auto切换为任意网络模式时，页面提示拨号断开，实际上拨号并未断开
========================================================================================*/
#ifndef _WDS_H_H_
#define _WDS_H_H_

#include <stdarg.h>
#include <netinet/in.h>
#include "atpconfig.h"    /* DTS2012020400492 j81004134 2012/2/4 */

/* <DTS2013021703454 优化ndis拨号代码 z00185914 20130217 DELETE 1 ROW */

#define WDS_SUCCESS 0
#define WDS_FAIL    -1
#define WDS_TIMEOUT -2

#define TRUE    1
#define FALSE   0

/* <DTS2013021703454 优化ndis拨号代码 z00185914 20130217 BEGIN */
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
/* DTS2013021703454 优化ndis拨号代码 z00185914 20130217 END> */

/* 接口名长度 */
#define ATP_QMI_IFC_LEN  32
/* 接口名长度 */
#define ATP_QMI_DEV_LEN  32
/* 用户名长度 */
#define ATP_QMI_USER_LEN  68
/* 密码长度 */
#define ATP_QMI_PASSWD_LEN  68
/* 绑定列表长度 */
#define ATP_QMI_BIND_LEN  64
#define WAN_QMI_SESSION_LEN 32
/*AT拨号指令长度*/
#define ATP_QMI_DIALNUMBER_LEN  32
/*APN设置长度*/
#define ATP_QMI_SETAPN_LEN  65   /* <DTS2010121603783.QMI: gKF34687 2010-12-18 ADD>*/
/* DNS列表长度 */
#define ATP_QMI_DNS_LEN  64
/* IP地址长度 */
#define ATP_QMI_IPADDR_LEN  16
/*VHG需求 pppoe service name*/
#define ATP_QMI_SERVICENAME_LEN  256
#define ATP_QMI_ACNAME_LEN  256
/* MAC地址长度 */
#define ATP_QMI_MAC_LEN  32
 /* profile中各项最大字符串长度(包括最后的\0) */
#define PROFILE_MAX_LEN    65
 /* 拨号模式 长度 */
#define DIAL_MODE_LEN    16
/* 网络接口名称*/
#define WAN_IFNAME_QMI  "rmnet0"
/* 进程pid文件*/
#define WDS_PID_FILE    "/var/wan/rmnet0/pid"
/*ipv4进程profile文件*/  /* DTS2012020400492 j81004134 2012/2/4 */
#define WDS_PROFILE_FILE    "/var/wan/rmnet0/config"
/* 记录拨号流量统计数据的文件*/
#define FMC_QMI_STATS_FILE  "/var/wan/rmnet0/umts_stats"
/*日志文件*/
#define WDS_LOG_FILE    "/var/log/wds.log"
/*计数器 定时时间，单位:毫秒*/
#define ALARM_TIME 1000
/*QMI TTY应用层WDS进程的节点*/
/* added by gKF345687 on 2010.11.22 for DTS2010112200946 begin */
#define DEVICE_TTY_PATH_0 "/dev/qmitty0"
#define DEVICE_TTY_PATH_1 "/dev/qmitty1"
/* added by gKF345687 on 2010.11.22 for DTS2010112200946 end */
/*TTY最大检查次数*/
#define MAX_TTY_CHECK_TIMES 30      /* <DTS2012032804611 g68080 2012-04-12 mod > */
/*USBNET网卡最大检查次数*/
#define MAX_USBNET_CHK_TIMES 30
// TTY中获取Tx ID的命令
#define QMI_TTY_IOC_MAGIC     'x'
#define QMI_TTY_IOCQ_TXID      _IO(QMI_TTY_IOC_MAGIC, 3)

/* <DTS2012020400492  j81004134 2012/2/4 begin */
#define IPV4_ADDR_LENGTH   (4)

/* 网络接口名称 */
#define WAN2_IFNAME_QMI  "rmnet1"
 /* 无线侧最大configured profile 个数 */
//#define MAX_CONFIGURED_PROFILE_NUM      16
#define MAX_CONFIGURED_PROFILE_NUM      2

/* dual IP 拨号时IPv4使用g_3GPP_cfg_profile_index_list[]中的第一个configured profile  */
#define CONFIGURED_PROFILE_INDEX_IPV4      0     
/* dual IP 拨号时IPv6使用g_3GPP_cfg_profile_index_list[]中的第二个configured profile  */
#define CONFIGURED_PROFILE_INDEX_IPV6      1         
 /* dual IP 拨号时需要的configured profile 个数 */
#define DUAL_IP_CONFIGURED_PROFILE_NUM     2
/* QMI接口名编号基址偏移 */
#define WAN_QMI_BASE_OFFSET  (0x3)

#if defined (SUPPORT_ATP_QMI6)
#define IPV6_ADDR_LENGTH   (16)
#define IPV6_PREFIX_LENGTH (1)
/*ipv6进程profile文件*/
#define WDS_PROFILE_FILE_IPV6    "/var/wan/rmnet0/config6"
/* 记录IPv6 WAN 设备流量统计数据的文件*/
#define FMC_QMI_STATS_FILE_IPV6  "/var/wan/rmnet1/umts_stats"
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
/*----------------------WDS拨号连接模块----------------------------*/
#define SIGBREAK 36       /*断开连接的信号*/
#define SIGCNNT  37       /*建立连接的信号*/
#define SIGUSBDOWN  38       /*sub状态down信号*/      /* <DTS2010120301841.QMI: gKF34687 2010-12-06 ADD >*/
#define SIGSENDRS  39       /*发送一个RS报文*/    /* DTS2012020400492 j81004134 2012/2/4 */  
/* <DTS2012060604837 j81004134 2012/6/5 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
#define SIG_QMI_LOG_SWITCH_CHANGE 39
#endif
/* < DTS2012072100113 l00205975 2012/07/27 begin */ 
#if defined(SUPPORT_ATP_ATP_FMCWEB)
#define SIG_QMI_KEY_PRESS       40  /* 短按 */
#define SIG_QMI_KEY_LONG_PRESS  41  /* 长按 */
#endif
/* DTS2012072100113 l00205975 2012/07/27 end> */ 
/* <DTS2013072505873 w00189018 2013/7/5 begin */
#define SIG_QMI_ROAM           42  /* 漫游通知 */
/* DTS2013072505873 w00189018 2013/7/5 end> */
#define SIGFORBIDCONN           43  /*断开并且禁止拨号的信号*/
#define SIGALLOWCONN            44   /*允许拨号的信号*/
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

/* 认证方式(QMI def: 1,2,3;)*/
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

/*拨号类型，用于设置var/wan/usbnet0/dial ，和enum dialstatus保持一致*/
typedef enum
{
    DIAL_MODE_WAIT= 0,
    DIAL_MODE_DISCON, 
    DIAL_MODE_HAND,   /* 手动拨号 */
    DIAL_MODE_AUTO,  /* 自动拨号 */
    DIAL_MODE_DEMAND,  /* 按需拨号 */
    /* <DTS2012070207035 w00211169 2012-7-17 begin */
    DIAL_MODE_PENDING,  /* 手动拨号未激活 */    
    /* DTS2012070207035 w00211169 2012-7-17 end> */
    DIAL_MODE_MAX
}DIAL_MODE_E;

 /*拨号连接状态，用于设置var/wan/usbnet0/status ，和 enum tagpppoestatus保持一致*/
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

/*当前WDS连接信息结构*/
typedef struct
{
    char    sz_username[PROFILE_MAX_LEN];   /* 用户名*/
    unsigned char   auth_type;                      /* 认证方式*/
    struct in_addr  dns1;                   /*首要DNS*/
    struct in_addr  dns2;                   /* 次要DNS*/
    struct in_addr  ip_addr;                /* IP地址*/
    struct in_addr  netmask;                /*掩码*/
    struct in_addr  gateway;                /* 网关地址*/
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined (SUPPORT_ATP_QMI6)
    char ipv6_prefix;   /* ipv6地址前缀 */
    char ipv6_gateway_prefix;   /* ipv6网关地址 前缀 */
    struct in6_addr  ipv6_addr; /* ipv6地址 */    
    struct in6_addr  ipv6_gateway; /* ipv6网关地址 */    
    struct in6_addr ipv6_dns1;   /* ipv6 首要dns server地址 */
    struct in6_addr ipv6_dns2;   /* ipv6 次要dns server地址 */
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
/* IPV6地址配置方式 */
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

/* IPV6地址请求选项 */
typedef enum
{
    /* IANA + IAPD */
    ATP_QMI_IPV6_ADDR_OPTION_BOTH,
    /* IANA */
    ATP_QMI_IPV6_ADDR_OPTION_IANA,
    /* IAPD */
    ATP_QMI_IPV6_ADDR_OPTION_IAPD,
    /* 自适应，暂不支持 */
    ATP_QMI_IPV6_ADDR_OPTION_AUTO,
}ATP_QMI_IPV6_ADDR_OPTION_EN;

/* <DTS2012022806805 00206962 2012-03-13 ADD begin> */
/* 发送给CMS的slaac处理的命令字 */
typedef enum tag_RADVD_CMD_WORD_E
{
    RADVD_CMD_WORD_ONDEMAND_CFG = 0,               /* 按需拨号前，设置RADVD 为normal模式 */
    RADVD_CMD_WORD_COMEBACK                              /* 按需拨号后，重启RADVD进程，恢复为数据库中的配置 */
}RADVD_CMD_WORD_E;
/* DTS2012022806805 00206962 2012-03-13 ADD end> */
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
/* 配置参数结构 ,和ATP_PPPC_CONFIG_ST 保持一致*/
typedef struct
{
    /* PPP接口名 */
    char acInterface[ATP_QMI_IFC_LEN];
    /* PPP接口名 */
    char acDevice[ATP_QMI_DEV_LEN];
    /* 拨号用户名 */
    char acUserName[ATP_QMI_USER_LEN];
    /* 密码 */
    char acPassWord[ATP_QMI_PASSWD_LEN];
    /* 绑定列表 */
    char acBindList[ATP_QMI_BIND_LEN];
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
    ATP_QMI_ATMQOS_ST stAtmQos;
    char acPppSessid[WAN_QMI_SESSION_LEN];
    /*认证方式*/
    unsigned int ulAuth;
    /*按需拨号空闲时间*/
    unsigned int ulIdleTime;
    /*B300 新增*/
    unsigned int ulRandomDialInterval;  /*PPPOE开始拨号前随机等待时间的最大值。*/
    unsigned int ulRetryDialInterval;  /*PPPOE拨号失败后等待的时间间隔。*/
    unsigned int ulRetrySendInterval;  /*PPPOE重发报文的时间间隔*/
    unsigned int ulKeepAliveInterval;  /*LCP发送时间间隔*/
    /*AT拨号指令*/
    char acDialNumber[ATP_QMI_DIALNUMBER_LEN];
    /*设置APN*/
    char acSetApn[ATP_QMI_SETAPN_LEN];
    /*<DTS2010052700199 l00101002 20100531 Begin*/
    /*设置ip*/
    char acSetIp[ATP_QMI_IPADDR_LEN];
    /*DTS2010052700199 l00101002 20100531 End>*/
    unsigned int ulMaxMru;
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#ifdef SUPPORT_ATP_MTU
    /* 最大MTU */
    unsigned int ulMaxMtu;
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
    int iReqTime;
    /************ATPV1R2C03 下发进程新增写配置项****************/
    /*VHG需求 pppoe service name*/
    char acPppoeServiceName[ATP_QMI_SERVICENAME_LEN];
    char acPppoeAcName[ATP_QMI_ACNAME_LEN];
    /* 以太网虚拟PPPOA的MAC地址 */
    char acPppoe2aMacAddr[ATP_QMI_MAC_LEN];
    /* 以太网虚拟PPPOA的会话ID */
    unsigned int ulPppoe2aSid;
/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined  (SUPPORT_ATP_QMI6)
    /* 是否启用IPv4功能 */
    unsigned int ulIpv4;
    /* 是否启用IPv6功能 */
    unsigned int ulIpv6;
    /* IPv6请求地址类型，如IANA或IAPD */
    unsigned int ulManagedAddress;
#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
}ATP_QMI_CONFIG_ST;

/*用户配置文件结构*/
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
    /* 是否启用IPv4功能 */
    unsigned int ulIpv4;
    /* 是否启用IPv6功能 */
    unsigned int ulIpv6;
    /* IPv6请求地址类型，如IANA或IAPD */
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
函数原形：int ATP_QMI_Config (void)
描述:                创建QMI拨号配置参数文件
输入:                无
输出:                无
返回值:          成功返回0；失败返回-1
其他:                无
-------------------------------------------------------------*/
int atp_qmi_config(const ATP_QMI_CONFIG_ST* pstConfig);
/*------------------------------------------------------------
函数原形：int ATP_QMI_Start (void)
描述:               启动qmi拨号进程； 
输入:                无
输出:                无
返回值:          成功返回0；失败返回-1
其他:                无
-------------------------------------------------------------*/
int atp_qmi_start(const char* pszInterface, int ip_family_pref);

/*------------------------------------------------------------
函数原形：int atp_qmi_stop (void)
描述:               关闭qmi拨号进程； 
输入:                无
输出:                无
返回值:          成功返回0；失败返回-1
其他:                无
-------------------------------------------------------------*/
int atp_qmi_stop(const char* pszInterface);

/******************************************************************************
  函数名称：atp_qmi_set_trigger
  功能描述：设置QMI的触发模式
  输入参数：pszInterface 接口名
                              ulTrigger 触发模式
  输出参数：无
  返 回 值：成功WDS_SUCCESS，失败WDS_FAIL
  其它说明：
******************************************************************************/
void atp_qmi_set_trigger(char* pszInterface, unsigned int ulTrigger);  /* <DTS2010112902193.QMITTY: gkf34687 2010-11-29 MODIFY >*/

/*****************************************************************************
 函 数 名  : ATP_QMI_API_GetVersion
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 返回组件版本号

*****************************************************************************/
const char * atp_qmi_api_get_version(void);

/*------------------------------------------------------------
  函数原型:  int max_dial_time_get()
  描  述  :  获得最大拨号时间(s)
  输  入  :  无
  输  出  :  无
  返回值  :  int 
-------------------------------------------------------------*/
int max_dial_time_get(void);

/*------------------------------------------------------------
  函数原型:  int mtu_get()
  描  述  :  获得MTU值
  输  入  :  无
  输  出  :  无
  返回值  :  int 
-------------------------------------------------------------*/
int mtu_get(void);

/* <DTS2012020400492  j81004134 2012/2/4 begin */
#if defined  (SUPPORT_ATP_QMI6)
/*------------------------------------------------------------
函数原形：int ATP_QMI_Start (void)
描述:               启动qmi拨号进程； 
输入:                无
输出:                无
返回值:          成功返回0；失败返回-1
其他:                无
-------------------------------------------------------------*/
int atp_qmi6_start(const char* pszInterface);

/*------------------------------------------------------------
函数原形：int atp_qmi_stop (void)
描述:               关闭qmi拨号进程； 
输入:                无
输出:                无
返回值:          成功返回0；失败返回-1
其他:                无
-------------------------------------------------------------*/
int atp_qmi6_stop(const char* pszInterface);

/******************************************************************************
  函数名称：atp_qmi_set_trigger6
  功能描述：(在ipv6拨号方式下)设置QMI的触发模式
  输入参数：pszInterface 接口名
                              ulTrigger 触发模式
  输出参数：无
  返 回 值：无
  其它说明：
******************************************************************************/
void atp_qmi6_set_trigger(char* pszInterface, unsigned int ulTrigger);

/* <DTS2012022806805 00206962 2012-03-13 ADD begin */
/******************************************************************************
  函数名称：get_qmi6_dial_mode
  功能描述：获取qmi ipv6的拨号类型
  输入参数：无
  输出参数：无
  返 回 值：错误时返回WDS_FAIL
                          正确时返回拨号类型
  其它说明：
******************************************************************************/
int get_qmi6_dial_mode(void);

/******************************************************************************
  函数名称：get_qmi6_dial_status
  功能描述：获取qmi ipv6的拨号状态
  输入参数：无
  输出参数：无
  返 回 值：错误时返回WDS_FAIL
                          正确时返回拨号状态
  其它说明：
******************************************************************************/
int get_qmi6_dial_status(void);
/* DTS2012022806805 00206962 2012-03-13 ADD end> */

#endif
/* DTS2012020400492  j81004134 2012/2/4 end> */
#endif /*_WDS_H_H_*/
/* DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 END> */

