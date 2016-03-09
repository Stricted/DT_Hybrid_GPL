/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file iptunnelapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author d00191326
  *\date  2011-12-23
*/


#ifndef __IPTUNNEL_API_H__
#define __IPTUNNEL_API_H__

#include "atputil.h"
#include "cfmapi.h"
#ifdef SUPPORT_ATP_TR181
#include "ipintfcmsapi.h"
#else
#include "wancmsapi.h"
#endif

#ifdef ATP_DEBUG
#define ATP_IPTUNNEL_DEBUG(format, args...) printf("TUNNEL: file[%s] line[%d] func[%s]: " format "\r\n", __FILE__, __LINE__,__FUNCTION__, ## args)
#define ATP_IPTUNNEL_CMD(acCmd)  {printf("TUNNEL: file[%s] line[%d] func[%s]: [%s] \r\n", __FILE__, __LINE__,__FUNCTION__, acCmd); ATP_Exec(acCmd);}
#else
#define ATP_IPTUNNEL_DEBUG(format, args...)
#define ATP_IPTUNNEL_CMD ATP_Exec
#endif

#define IPTUNNEL_CONF_MAX_NUM 4

#define IPTUNNEL_NAME_MAX_LEN 32
#define IPTUNNEL_WAN_INTERFACE_LEN 256
#define IPTUNNEL_LAN_INTERFACE_LEN 1024
#define IPTUNNEL_PATH_LEN 256
#define IPTUNNEL_CONN_STATUS_LEN 32
#define IPTUNNEL_MODE_LEN 8
#define IPTUNNEL_MECHANISM_LEN 8
#define IPTUNNEL_IPV6_ADDR_LEN 40
#define IPTUNNEL_IPV4_ADDR_LEN 20
#define IPTUNNEL_WORD_LEN 64
#define IPTUNNEL_CMD_LEN 256
#define IPTUNNEL_LINE_LEN 256
#define IPTUNNEL_ENDPOINT_LEN 260

#define IPTUNNEL_6RD_PREFIX_LEN_MIN 16
#define IPTUNNEL_6RD_PREFIX_LEN_MAX 128
#define IPTUNNEL_IPV6_DNS_LEN 132

#define TUNNEL_6RD_WAN_MIN_MTU 1300
#define TUNNEL_DSLITE_WAN_MIN_MTU 1320
#define TUNNEL_GRE_WAN_MIN_MTU 1304
#define DEFAULT_MTU 1500

#define IPTUNNEL_6IN4_STR "6in4"
#define IPTUNNEL_4IN6_STR "4in6"

#define IPTUNNEL_6RD6TO4_STR "6rd6to4"
#define IPTUNNEL_6RD_STR "6rd"
#define IPTUNNEL_6TO4_STR "6to4"
#define IPTUNNEL_DSLITE_STR "DSLite"
#define IPTUNNEL_GRE_STR "GRE"

#define IPTUNNEL_STATUS_CONNECTED "Connected"
#define IPTUNNEL_STATUS_CONNECTING "Connecting"
#define IPTUNNEL_STATUS_DISCONNECTED "Disconnected"

#define TUNNEL_MECHANISM_MAX_NUM 4
#define TUNNEL_MECHANISM_6RD 0
#define TUNNEL_MECHANISM_DSLITE 1

//!\brief IPTUNNEL���ò��������ݽṹ
/*!
 *
 */
typedef struct tagATP_IPTUNNEL_CONF_ST
{
  /*��һ�ڵ��ָ��*/
  struct tagATP_IPTUNNEL_CONF_ST* pstNext;
  /*���ʵ����ID*/
  VOS_UINT32 ulTunnelModeInstId;
  /*CMO*/
  VOS_UINT32 ulObjID;
  /*���·��*/
  VOS_CHAR acTunnelPath[IPTUNNEL_PATH_LEN];
  /*����Ƿ񼤻�*/
  VOS_UINT32 ulActivated;
  /*����ӿ���*/
  VOS_CHAR acTunnelName[IPTUNNEL_NAME_MAX_LEN];
  /*�������*/
  VOS_CHAR acAliasName[ATP_ACTUAL_LEN_128];
  /*������WAN�ӿ�*/
  VOS_CHAR acAssociatedWanIfName[IPTUNNEL_WAN_INTERFACE_LEN];
  /*����״̬*/
  VOS_CHAR acConnStatus[IPTUNNEL_CONN_STATUS_LEN];
  /*�������*/
  VOS_CHAR acMechanism[IPTUNNEL_MECHANISM_LEN];
  /*�Ƿ��Զ�����*/
  VOS_UINT32 ulDynamic;
  /*6rd��IPv4���볤��*/
  VOS_UINT32 ulIPv4MaskLen;
  /*6rdǰ׺*/
  VOS_CHAR ac6rdPrefix[IPTUNNEL_IPV6_ADDR_LEN];
  /*6rdǰ׺����*/
  VOS_UINT32 ul6rdPrefixLen;
  /*����Ķ˵㣬����Ϊ��ַ������*/
  VOS_CHAR acTunnelEndpoint[IPTUNNEL_ENDPOINT_LEN];
  /*IPv4�ĶԶ˵�ַ*/
  VOS_CHAR acIPv4Endpoint[IPTUNNEL_IPV4_ADDR_LEN];
  /*DSLite�ĶԶ�IPv6��ַ*/
  VOS_CHAR acRemoteIpv6Address[IPTUNNEL_IPV6_ADDR_LEN];
  /*IPV6 DNS��������ַ*/
  VOS_CHAR acIpv6DnsServers[IPTUNNEL_IPV6_DNS_LEN];
  /*����ӿڵ�IPv6��ַ*/
  VOS_CHAR acIpv6IfAddr[IPTUNNEL_IPV6_ADDR_LEN];
  VOS_UINT32 ulIPv4Enable;
  VOS_UINT32 ulIPv6Enable;
  /*�Ƿ�ָ����WAN·��*/
  VOS_BOOL  bConfiguredWan;
} ATP_IPTUNNEL_CONF_ST;

typedef enum
{
    ATP_6IN4_UP_NOTIFY_PRIORITY_DNSPROC=0,
    ATP_6IN4_UP_NOTIFY_PRIORITY_SNTPPROC,
}ATP_6IN4_UP_NOTIFY_PRIORITY;

typedef enum
{
    ATP_6IN4_DOWN_NOTIFY_PRIORITY_DNSPROC=0,
    ATP_6IN4_DOWN_NOTIFY_PRIORITY_SNTPPROC,
}ATP_6IN4_DOWN_NOTIFY_PRIORITY;


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*!
  \brief ���ģ��ĳ�ʼ������
  \return ��ʼ���Ƿ�ɹ�
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_Init();

/*!
  \brief ���ģ���ȥ��ʼ������
  \return ȥ��ʼ���Ƿ�ɹ�
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_UnInit();


/*!
  \brief �������������WAN UPʱ�ص�
  \param[in]  acWanPath: WAN��·��
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_Start(VOS_VOID* pszWanPath, VOS_VOID* pvHook);

/*!
  \brief ֹͣ�������WAN DOWNʱ�ص�
  \param[in]  acWanPath: WAN��·��
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_Stop(VOS_VOID* pszWanPath, VOS_VOID* pvHook);


/*!
  \brief ����WAN PATH��������Ϣ
  \param[in]  pcPath: WAN·��
  \return �����WAN��������������ָ�룬�����ڷ���VOS_NULL
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceByWanPath(const VOS_CHAR* pcPath);

/*!
  \brief wan�仯ʱɾ����ص��������
  \param[in]  pszWanPath: WAN·��
  \param[in]  pvHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_ConfClear(VOS_VOID* pszWanPath, VOS_VOID* pvHook);

/*!
  \brief Tunnel up ʱ��ע��㺯��
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_Up_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief Tunnel downʱ��ע��㺯��
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_Down_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief ������������Ƴ�ʱ��ע���
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_BrDelProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief ���������ĳ����ʱ��ע���
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_IPTunnel_BrAddProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);


/*!
  \brief ����·����ȡ���ʵ��
  \param[in]  pcPath: ���·��
  \return ATP_IPTUNNEL_CONF_ST*
  \retval VOS_NULL��ʾ������
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceByPath(const VOS_CHAR* pcPath);

/*!
  \brief ͨ��·����ȡ�������
  \param[in]  pcPath: ���·��
  \param[in]  pszAlias: �������
  \param[in]  ulAliasLen: ��������
  \return VOS_UINT32
  \retval VOS_OK��ȡ�ɹ�
*/
VOS_UINT32 ATP_IPTunnel_GetTunnelAlias(const VOS_CHAR* pcPath, VOS_CHAR* pszAlias, VOS_UINT32 ulAliasLen);

VOS_BOOL ATP_IPTunnel_IsExistActivated6in4Tunnel(const VOS_CHAR* pcPath);

VOS_BOOL ATP_IPTunnel_IsExistActivatedTunnel(VOS_VOID);

/*!
  \brief ��ȡ����ͷ��
  \return ATP_IPTUNNEL_CONF_ST
  \retval VOS_NULL����Ϊ��
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceHeader(VOS_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
