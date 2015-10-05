/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dnscmsdb.h
  *TR181��dns�ڵ�������ص�ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
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
    VOS_CHAR                acServers[ATP_STRING_LEN_128];//���������v6��dns
    VOS_CHAR                acIntf[ATP_STRING_LEN_256];//��Ӧ��ipʵ��·��
    ATP_DNS_SERVERTYPE_EN   enServerType;//����
} ATP_DNS_INFO_ST;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/*!
  \brief ���DNS�ڵ��api,��Ҫͬʱ���DNS.Client.Server��DNS.Relay.Forwarding�����ڵ�
  \param[in]  pstServerInfo: ��װ��DNS server��Ϣ
  \return �ɹ�VOS_OK��ʧ�ܷ��ش�����
*/
VOS_UINT32 ATP_DNSCMS_AddServer(const ATP_DNS_INFO_ST* pstServerInfo);
/*!
  \brief ɾ��DNS�ڵ��api,��Ҫͬʱɾ��DNS.Client.Server��DNS.Relay.Forwarding�����ڵ�
  \param[in]  pcIpintf: ��Ҫɾ����DNS server��Ϣ��Ӧ��IP.Interfaceʵ��
  \param[in]  enDNSType: ��Ҫɾ����DNS server��Ϣ��Ӧ�����ͣ�һ��IP.Interfaceʵ�����ܶ�Ӧ�����ͬ���͵�DNS�ڵ�ʵ��
  \return �ɹ�VOS_OK��ʧ�ܷ��ش�����
*/
VOS_UINT32 ATP_DNSCMS_DelServer(const VOS_CHAR* pcIpintf, ATP_DNS_SERVERTYPE_EN enDNSType);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __DNSCMS_DB_H__ */

