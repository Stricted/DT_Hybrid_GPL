/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ipintfcmsapi.h
  *ipintf ����api
  *\copyright 2007-2020����Ϊ�������޹�˾
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

//!\brief IPv4��ַ����ö��ֵ
/*!
 *�����Device.IP.Interface.{i}.IPv4Address.{i}.AddressingType��ȡֵ��Ӧ
 */
typedef enum
{
    ATP_IPV4_ADDRTYPE_DHCP,
    ATP_IPV4_ADDRTYPE_IPCP,
    ATP_IPV4_ADDRTYPE_AUTOIP,
    ATP_IPV4_ADDRTYPE_STATIC,
}ATP_IPV4_ADDRTYPE_EN;

//!\brief ��װ��IPv4��ַ��Ϣ
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

    //���յ���iana��Ϣ
    VOS_CHAR    acIPv6IanaAddr[ATP_BUFFER_128];
    VOS_UINT32 ulAddrPreferredLifetime;   
    VOS_UINT32 ulAddrValidLifetime;

    //���յ���iapd��Ϣ
    VOS_CHAR    acIPv6Prefix[ATP_BUFFER_128];
    VOS_UINT32 ulPrefixPreferredLifetime;   
    VOS_UINT32 ulPrefixValidLifetime;

    //addr��prefix �Ŀ���״̬
    ATP_IPV6_ADDRSTATUS_EN  enAddrStatus;
    ATP_IPV6_PREFIXSTATUS_EN  enPrefixStatus;   
    
    //addr��prefix ��origin״̬
    ATP_IPV6_ADDRORIGIN_EN  enAddrOrigin;
    ATP_IPV6_PREFIXORIGIN_EN  enPrefixOrigin;   
    
    /* IPV6״̬�仯��ʶ */
    VOS_UINT32 ulXIpv6Change;
    /* IPV6״̬ */
    VOS_CHAR   acXIpv6ConnStatus[ATP_WAN_STATUS_LEN];
    /*IPv6 ״̬��ΪUP��Down��ʱ��*/
    VOS_UINT32 ulStatusChgtime;
    
    /* WAN���ӵ�IPV6��ַ (����iana��ַ��raǰ׺��ַ)*/
    VOS_CHAR   acXIpv6Addr[ATP_WAN_IPV6_ADDR_LEN];
    /* WAN���ӵ�IPV6��ַǰ׺ ����*/
    VOS_UINT32 ulXIpv6AddrPrefixLength;   
    
    /*WAN�յ���OnLinkFlagΪ1��RAǰ׺, dhcpv6�����յ����,��ֻ��¼��WAN�ڵ�ַһ�µ�һ��;
    ���wan���������˵�ַ,��Ҫ������Դ����·�������Ŀ*/
    VOS_CHAR   acXIpv6RaPrefix[ATP_WAN_IPV6_ADDR_LEN];
    /*WAN�յ���OnLinkFlagΪ1��RAǰ׺�ĳ���*/
    VOS_UINT32 ulXIpv6RaPrefixLen;
    VOS_UINT32 ulRaAutoConfFlag; 
    VOS_UINT32 ulRaOnLinkFlag;       
    VOS_UINT32 ulRaPrefixPreferredLifetime;   
    VOS_UINT32 ulRaPrefixValidLifetime;
    VOS_CHAR    acRaPreferredRouterFlag[ATP_BUFFER_16];
    VOS_UINT32 ulRaRouterLifetime;    
    
    /* WAN���ӵ�IPV6Ĭ������ */
    VOS_CHAR   acXIpv6GateWay[ATP_WAN_IPV6_ADDR_LEN];
    /* WAN�����Զ�ѧϰ����IPV6Ĭ������ */
    VOS_CHAR   acXIpv6DynamicGateWay[ATP_WAN_IPV6_ADDR_LEN];
    
    /* �����Ӿ�̬���õ�IPV6 DNS��ַ */
    VOS_CHAR   acXIpv6DnsServers[ATP_WAN_IPV6_DNS_LEN];
    /* �������Զ���ȡ����IPV6 DNS��ַ */
    VOS_CHAR   acXIpv6DynamicDns[ATP_WAN_IPV6_DNS_LEN];
   
}ATP_IPINTF_IPV6INFO_ST;
#endif

#if defined(__cplusplus)
extern "C" {
#endif
/*!
  \brief ip ��Ҫupʱ��hook�㣬dhcpc��dhcp6c���ܹ�ע��hook��
*/
VOS_UINT32 ATP_IPINTF_Up_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
/*!
\brief ip ��Ҫdownʱ��hook�㣬dhcpc��dhcp6c���ܹ�ע��hook��
*/
VOS_UINT32 ATP_IPINTF_Down_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);

VOS_UINT32 ATP_IPINTF_Reset_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_PPPINTF_Reset_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);
VOS_UINT32  PPPintfResetNotifyProcDoNotify(const VOS_VOID* pszInfo);
VOS_UINT32  IpintfResetNotifyProcDoNotify(const VOS_VOID* pszInfo);
/*!
  \brief ����IPv4�ĵ�ַ����ַ����Ϊ�գ���ӦDevice.IP.Interface.{i}.IPv4Address.{i}.IPAddress����
  \���õ�ַΪ��Чֵ�ɹ���ὫIP.Interface��״̬up������Ϊ�ճɹ���ὫIP.Interface��״̬down
  \param[in]  pszPath: ip interface ʵ��·��
  \param[in]  pszValue: ip��ַ
  \return VOS_OK���ߴ�����
*/
VOS_UINT32 ATP_IPINTF_SetIPv4Addr(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief ����IPv4���룬��ӦDevice.IP.Interface.{i}.IPv4Address.{i}.SubnetMask����
  \param[in]  pszPath: ip interface ʵ��·��
  \param[in]  pszValue: ip��ַ
  \return VOS_OK���ߴ�����
*/
VOS_UINT32 ATP_IPINTF_SetIPv4Mask(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief ����IPv4��ַ���ͣ���ӦDevice.IP.Interface.{i}.IPv4Address.{i}.AddressingType����
  \param[in]  pszPath: 
  \param[in]  pszValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPINTF_SetIPv4AddrType(const VOS_CHAR* pszPath, const VOS_CHAR* pszValue);
/*!
  \brief IPv4 wan up�����������pstIpintf��pszIpintfPath����һ������
  \param[in]  *pstIpintf: ip interface ָ��
  \param[in]  *pszIpintfPath: ip interface ʵ��·��
  \param[in]  *pstAddrInfo: ��װ��ip��ز���ֵ
  \return �ɹ�����VOS_OK�����󷵻�VOS_NOK,���ߴ�����
  \retval ATP_INTF_RET_PARA_ERR ��δ���
*/
VOS_UINT32 ATP_IPINTF_Ipv4UpProcess(ATP_INTERFACE_ST *pstIpintf, VOS_CHAR *pszIpintfPath, const ATP_IPINTF_IPV4ADDRINFO_ST *pstAddrInfo);
/*!
  \brief IPv4 wan down�����������pstIpintf��pszIpintfPath����һ������
  \param[in]  *pstIpintf: ip interface ָ��
  \param[in]  *pszIpintfPath: ip interface ʵ��·��
  \return �ɹ�����VOS_OK�����󷵻�VOS_NOK,���ߴ�����
  \retval ATP_INTF_RET_PARA_ERR ��δ���
*/
VOS_UINT32 ATP_IPINTF_Ipv4DownProcess(ATP_INTERFACE_ST *pstIp, VOS_CHAR *pszIpintfPath);

/*!
  \brief IPv4 wan update������ for HOMEGW-14424
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

