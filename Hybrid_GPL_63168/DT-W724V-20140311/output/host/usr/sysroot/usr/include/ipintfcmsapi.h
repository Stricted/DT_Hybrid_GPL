/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ipintfcmsapi.h
  *ipintf 对外api
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-5-24
*/
#ifndef __IPINTFCMS_API_H__
#define __IPINTFCMS_API_H__

#include "atpinterfaceapi.h"
#include "atpwancore.h"
#include "wancorecmsapi.h"
#include "wancorecmsgetapi.h"

#ifdef ATP_DEBUG
#define ATP_IPINTF_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_IPINTF_DEBUG(x...)
#endif

#define ATP_IPINTF_ERROR(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}

#define ATP_IPINTF_IPV4ADDR_INST_MAX_NUM  1
#define ATP_IPINTF_ARR_LEN                4

//!\brief up priority
/*!
 *
 */
typedef enum
{
	/* <DTS2013081901325 xiazhongyue 20130819 begin */
    ATP_IPINTF_NOTIFY_UP_LTE = 0,
	/* DTS2013081901325 xiazhongyue 20130819 end>   */
    ATP_IPINTF_NOTIFY_UP_DHCPC=1,
    ATP_IPINTF_NOTIFY_UP_PPPC,
    ATP_IPINTF_NOTIFY_UP_DHCP6c,
}ATP_IPINTF_NOTIFY_UP_EN;

//!\brief down priority
/*!
 *
 */
typedef enum
{
	/* <DTS2013081901325 xiazhongyue 20130819 begin */
    ATP_IPINTF_NOTIFY_DOWN_LTE=0,
	/* DTS2013081901325 xiazhongyue 20130819 end>   */
    ATP_IPINTF_NOTIFY_DOWN_DHCPC=1,
    ATP_IPINTF_NOTIFY_DOWN_PPPC,
    ATP_IPINTF_NOTIFY_DOWN_DHCP6c,   
}ATP_IPINTF_NOTIFY_DOWN_EN;

//!\brief IPv4地址类型枚举值
/*!
 *与参数Device.IP.Interface.{i}.IPv4Address.{i}.AddressingType的取值对应
 */
typedef enum
{
    ATP_IPV4_ADDRTYPE_DHCP,
    ATP_IPV4_ADDRTYPE_IPCP,
    ATP_IPV4_ADDRTYPE_AUTOIP,
    ATP_IPV4_ADDRTYPE_STATIC,
}ATP_IPV4_ADDRTYPE_EN;

//!\brief 封装的IPv4地址信息
typedef struct tagATP_IPINTF_IPV4ADDRINFO_ST
{
    VOS_CHAR    acIPAddr[ATP_WAN_ADDR_LEN];
    VOS_CHAR    acIPMask[ATP_WAN_ADDR_LEN];   
    VOS_CHAR    acName[ATP_STRING_LEN_64];
    ATP_IPV4_ADDRTYPE_EN  enAddrType;
}ATP_IPINTF_IPV4ADDRINFO_ST;

#ifdef SUPPORT_ATP_IPV6
typedef enum
{
    ATP_IPV6_ADDRSTATUS_NULL ,
    ATP_IPV6_ADDRSTATUS_Preferred,
    ATP_IPV6_ADDRSTATUS_Deprecated,
    ATP_IPV6_ADDRSTATUS_Invalid,

}ATP_IPV6_ADDRSTATUS_EN;

typedef enum
{
    ATP_IPV6_PREFIXSTATUS_NULL ,
    ATP_IPV6_PREFIXSTATUS_Preferred,
    ATP_IPV6_PREFIXSTATUS_Deprecated,
    ATP_IPV6_PREFIXSTATUS_Invalid,
    ATP_IPV6_PREFIXSTATUS_Inaccessible,
    ATP_IPV6_PREFIXSTATUS_Unknown,

}ATP_IPV6_PREFIXSTATUS_EN;

typedef enum
{
    ATP_IPV6_ADDRORIGIN_NULL ,
    ATP_IPV6_ADDRORIGIN_AutoConfigured,
    ATP_IPV6_ADDRORIGIN_DHCPv6,
    ATP_IPV6_ADDRORIGIN_WellKnown,
    ATP_IPV6_ADDRORIGIN_Static,

}ATP_IPV6_ADDRORIGIN_EN;

typedef enum
{
    ATP_IPV6_PREFIXORIGIN_NULL ,
    ATP_IPV6_PREFIXORIGIN_AutoConfigured,
    ATP_IPV6_PREFIXORIGIN_DHCPv6,
    ATP_IPV6_PREFIXORIGIN_WellKnown,
    ATP_IPV6_PREFIXORIGIN_Static,

}ATP_IPV6_PREFIXORIGIN_EN;

typedef struct tagATP_IPINTF_IPV6INFO_ST
{
    struct tagATP_IPINTF_IPV6INFO_ST   *pstNext;

    VOS_CHAR    acIpintfName[ATP_STRING_LEN_64];

    //接收到的iana信息
    VOS_CHAR    acIPv6IanaAddr[ATP_BUFFER_128];
    VOS_UINT32 ulAddrPreferredLifetime;   
    VOS_UINT32 ulAddrValidLifetime;

    //接收到的iapd信息
    VOS_CHAR    acIPv6Prefix[ATP_BUFFER_128];
    VOS_UINT32 ulPrefixPreferredLifetime;   
    VOS_UINT32 ulPrefixValidLifetime;

    //addr和prefix 的可用状态
    ATP_IPV6_ADDRSTATUS_EN  enAddrStatus;
    ATP_IPV6_PREFIXSTATUS_EN  enPrefixStatus;   
    
    //addr和prefix 的origin状态
    ATP_IPV6_ADDRORIGIN_EN  enAddrOrigin;
    ATP_IPV6_PREFIXORIGIN_EN  enPrefixOrigin;   
    
    /* IPV6状态变化标识 */
    VOS_UINT32 ulXIpv6Change;
    /* IPV6状态 */
    VOS_CHAR   acXIpv6ConnStatus[ATP_WAN_STATUS_LEN];
    /*IPv6 状态变为UP或Down的时间*/
    VOS_UINT32 ulStatusChgtime;
    
    /* WAN连接的IPV6地址 (包括iana地址和ra前缀地址)*/
    VOS_CHAR   acXIpv6Addr[ATP_WAN_IPV6_ADDR_LEN];
    /* WAN连接的IPV6地址前缀 长度*/
    VOS_UINT32 ulXIpv6AddrPrefixLength;   
    
    /*WAN收到的OnLinkFlag为1的RA前缀, dhcpv6可能收到多个,但只记录和WAN口地址一致的一个;
    如果wan用它生成了地址,则要用它在源策略路由里加条目*/
    VOS_CHAR   acXIpv6RaPrefix[ATP_WAN_IPV6_ADDR_LEN];
    /*WAN收到的OnLinkFlag为1的RA前缀的长度*/
    VOS_UINT32 ulXIpv6RaPrefixLen;
    VOS_UINT32 ulRaAutoConfFlag; 
    VOS_UINT32 ulRaOnLinkFlag;       
    VOS_UINT32 ulRaPrefixPreferredLifetime;   
    VOS_UINT32 ulRaPrefixValidLifetime;
    VOS_CHAR    acRaPreferredRouterFlag[ATP_BUFFER_16];
    VOS_UINT32 ulRaRouterLifetime;    
    
    /* WAN连接的IPV6默认网关 */
    VOS_CHAR   acXIpv6GateWay[ATP_WAN_IPV6_ADDR_LEN];
    /* WAN连接自动学习到的IPV6默认网关 */
    VOS_CHAR   acXIpv6DynamicGateWay[ATP_WAN_IPV6_ADDR_LEN];
    
    /* 该连接静态设置的IPV6 DNS地址 */
    VOS_CHAR   acXIpv6DnsServers[ATP_WAN_IPV6_DNS_LEN];
    /* 该连接自动获取到的IPV6 DNS地址 */
    VOS_CHAR   acXIpv6DynamicDns[ATP_WAN_IPV6_DNS_LEN];
   
}ATP_IPINTF_IPV6INFO_ST;
#endif

#if defined(__cplusplus)
extern "C" {
#endif
/*!
  \brief ip 需要up时的hook点，dhcpc、dhcp6c可能关注该hook点
*/
VOS_UINT32 ATP_IPINTF_Up_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
/*!
\brief ip 需要down时的hook点，dhcpc、dhcp6c可能关注该hook点
*/
VOS_UINT32 ATP_IPINTF_Down_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);

VOS_UINT32 ATP_IPINTF_Reset_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_PPPINTF_Reset_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32  PPPintfResetNotifyProcDoNotify(const VOS_VOID* pszInfo);
VOS_UINT32  IpintfResetNotifyProcDoNotify(const VOS_VOID* pszInfo);
/*!
  \brief 设置IPv4的地址，地址可以为空，对应Device.IP.Interface.{i}.IPv4Address.{i}.IPAddress参数
  \设置地址为有效值成功后会将IP.Interface的状态up，设置为空成功后会将IP.Interface的状态down
  \param[in]  pszPath: ip interface 实例路径
  \param[in]  pszValue: ip地址
  \return VOS_OK或者错误码
*/
VOS_UINT32 ATP_IPINTF_SetIPv4Addr(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief 设置IPv4掩码，对应Device.IP.Interface.{i}.IPv4Address.{i}.SubnetMask参数
  \param[in]  pszPath: ip interface 实例路径
  \param[in]  pszValue: ip地址
  \return VOS_OK或者错误码
*/
VOS_UINT32 ATP_IPINTF_SetIPv4Mask(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief 设置IPv4地址类型，对应Device.IP.Interface.{i}.IPv4Address.{i}.AddressingType参数
  \param[in]  pszPath: 
  \param[in]  pszValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPINTF_SetIPv4AddrType(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief IPv4 wan up处理函数，入参pstIpintf和pszIpintfPath传入一个即可
  \param[in]  *pstIpintf: ip interface 指针
  \param[in]  *pszIpintfPath: ip interface 实例路径
  \param[in]  *pstAddrInfo: 封装的ip相关参数值
  \return 成功返回VOS_OK，错误返回VOS_NOK,或者错误码
  \retval ATP_INTF_RET_PARA_ERR 入参错误
*/
VOS_UINT32 ATP_IPINTF_Ipv4UpProcess(ATP_INTERFACE_ST *pstIpintf, VOS_CHAR *pszIpintfPath, const ATP_IPINTF_IPV4ADDRINFO_ST *pstAddrInfo);
/*!
  \brief IPv4 wan down处理函数，入参pstIpintf和pszIpintfPath传入一个即可
  \param[in]  *pstIpintf: ip interface 指针
  \param[in]  *pszIpintfPath: ip interface 实例路径
  \return 成功返回VOS_OK，错误返回VOS_NOK,或者错误码
  \retval ATP_INTF_RET_PARA_ERR 入参错误
*/
VOS_UINT32 ATP_IPINTF_Ipv4DownProcess(ATP_INTERFACE_ST *pstIp, VOS_CHAR *pszIpintfPath);

/*!
  \brief IPv4 wan update处理函数 for HOMEGW-14424
*/
VOS_VOID ATP_IPINTF_Ipv4UpdateProcess();

/*Start of Protocol 2013-12-11 22:29 for DTS2013120503096 by y00250667*/
VOS_BOOL ATP_WAN_IsPPPWan(const VOS_CHAR* pszPath);
VOS_BOOL ATP_WAN_IsLTEWan(const VOS_CHAR* pszPath);
VOS_INT32 ATP_WAN_ISLTETunnelUp();
#define WAN_GRE2_STATUS6_FILE       "/var/wan/gre2/status6"
/*End of Protocol 2013-12-11 22:29 for DTS2013120503096 by y00250667*/

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 ATP_IPINTF_Ipv6UpProcess(const VOS_CHAR *pszIpintfPath, const ATP_IPINTF_IPV6INFO_ST *pstIPv6Info);
VOS_UINT32 ATP_IPINTF_Ipv6UpdateProcess(const VOS_CHAR *pszIpintfPath, const ATP_IPINTF_IPV6INFO_ST *pstIPv6Info);
VOS_UINT32 ATP_IPINTF_Ipv6DownProcess(const VOS_CHAR *pszIpintfPath, const ATP_IPINTF_IPV6INFO_ST *pstIPv6Info);
#endif

#if defined(__cplusplus)
}
#endif

#endif

