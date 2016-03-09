/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dnscmsdb.h
  *TR181上dns节点配置相关的头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-6-25
*/
#ifndef __DNSCMS_DB_H__
#define __DNSCMS_DB_H__
#include "atputil.h"
#include "msgapi.h"
#include "cfmapi.h"

typedef enum
{
    ATP_DNS_SERVERTYPE_DHCPV4,
    ATP_DNS_SERVERTYPE_DHCPV6,
    ATP_DNS_SERVERTYPE_RA,
    ATP_DNS_SERVERTYPE_IPCP,
    ATP_DNS_SERVERTYPE_STATIC,
}ATP_DNS_SERVERTYPE_EN;

typedef struct tagATP_DNS_INFO_ST
{
    VOS_CHAR                acServers[ATP_STRING_LEN_128];//最长包含两个v6的dns
    VOS_CHAR                acIntf[ATP_STRING_LEN_256];//对应的ip实例路径
    ATP_DNS_SERVERTYPE_EN   enServerType;//类型
} ATP_DNS_INFO_ST;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/*!
  \brief 添加DNS节点的api,需要同时添加DNS.Client.Server和DNS.Relay.Forwarding两个节点
  \param[in]  pstServerInfo: 封装的DNS server信息
  \return 成功VOS_OK，失败返回错误码
*/
VOS_UINT32 ATP_DNSCMS_AddServer(const ATP_DNS_INFO_ST* pstServerInfo);
/*!
  \brief 删除DNS节点的api,需要同时删除DNS.Client.Server和DNS.Relay.Forwarding两个节点
  \param[in]  pcIpintf: 需要删除的DNS server信息对应的IP.Interface实例
  \param[in]  enDNSType: 需要删除的DNS server信息对应的类型，一个IP.Interface实例可能对应多个不同类型的DNS节点实例
  \return 成功VOS_OK，失败返回错误码
*/
VOS_UINT32 ATP_DNSCMS_DelServer(const VOS_CHAR* pcIpintf, ATP_DNS_SERVERTYPE_EN enDNSType);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __DNSCMS_DB_H__ */

