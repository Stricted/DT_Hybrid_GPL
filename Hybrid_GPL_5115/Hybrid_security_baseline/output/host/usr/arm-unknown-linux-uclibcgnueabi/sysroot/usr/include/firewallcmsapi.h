/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file firewallcmsapi.h
  *firewallcms api \n
  *����ǽģ�����ӿ�\n
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author h00163136
  *\date  2012-2-4
*/


#ifndef __FIREWALL_API_H__
#define __FIREWALL_API_H__

#include "atputil.h"

//!\brief ����ǽ����������ͣ����ӣ����룬ɾ��
/*!
 *ATP_FW_COMMAND_TYPE_EN��FW_COMMAND_TYPE_EN�����ṹ��Ҫ����һ��
 */
typedef enum tagAtpFwCommandType
{
    ATP_FW_CMD_TYPE_ADD = 0,              /*!< FW_CMD_TYPE_ADD */
    ATP_FW_CMD_TYPE_INSERT = 1,        /*!< FW_CMD_TYPE_INSERT */
    ATP_FW_CMD_TYPE_REMOVE = 2       /*!< FW_CMD_TYPE_REMOVE */
}ATP_FW_COMMAND_TYPE_EN;   /*!< FW_COMMAND_TYPE_EN */


#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief ����ǽ��ʼ������
  \param[in] ��: 
  \return VOS_OK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_Init(VOS_VOID);

/*!
  \brief ����ǽȥ��ʼ������
  \param[in] ��: 
  \return VOS_OK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_UnInit(VOS_VOID);

#if 0
/*!
  \brief ����ǽ���õ�ǰ�ȼ�
  \param[in]  ulCmo: 
  \param[in]  *pValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_FIREWALL_SetCurrLevel(VOS_UINT32 ulCmo, VOS_INT8 *pValue);

/*!
  \brief  ����ǽ��ȡ��ǰ�ȼ�
  �������  : 
  \param[in]  ulCmo: 
  \param[in]  *pulValue: 
  \param[in]  ulLength: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_FIREWALL_GetCurrLevel(VOS_UINT32 ulCmo, VOS_UINT32 *pulValue,VOS_UINT32 ulLength);
#endif

/*!
  \brief WAN UPʱ�����з���ǽ������
  \param[in]  pszWanPath: WAN·����
  \param[in]  *pvHook: û�õ�����Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStart (VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief WAN Downʱ�����й�������
  \param[in]  pszWanPath: WAN·����
  \param[in]  *pvHook: û�õ�����Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStop(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

#ifdef SUPPORT_ATP_FIREWALL_IPV6
/*!
  \brief IPv6ʹ�ܵ�WAN�ӿ�Upʱ�������ipv6����ǽ����
  \param[in]  pszWanPath: IPv6 WAN·����
  \param[in]  *pvHook: û�õ�����Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStart6 (VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief IPv6ʹ�ܵ�WAN�ӿ�Down��ʱ�������ipv6����ǽ����
  \param[in]  pszWanPath: IPv6 WAN·����
  \param[in]  *pvHook: û�õ�����Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStop6(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
#endif

#ifdef SUPPORT_ATP_IPV6
/* <DTS2013050807659 wanglong 2013-5-9 begin */
/******************************************************************************
  ��������  : ATP_FIREWALL_Ipv6WanAllowMcastNs
  ��������  : 
  �������  : 
              1.  pszWanPath:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2013-4-15
     ��    ��   : l00184769
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_Ipv6WanAllowMcastNs(VOS_CHAR* pszWanPath, VOS_BOOL bAllow);
/* DTS2013050807659 wanglong 2013-5-9 end> */
/*!
  \brief IPv6 WAN UPʱ������tcp mss������
  \param[in]  pszWanPath: IPv6 WAN·����
  \param[in]  *pvHook: ��Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_IPV6_MSS_AdjustStart(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief IPv6 WAN Downʱ������tcp mss������
  \param[in]  *pszWanPath:  IPv6 WAN·����
  \param[in]  *pvHook: ��Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_IPV6_MSS_AdjustStop(VOS_VOID *pszWanPath, VOS_VOID *pvHook);
#endif

/*!
  \brief WAN ɾ��ʱ��ɾ������WAN�϶�Ӧ�ķ���ǽ����
  \param[in]  pszWanPath: WAN·����
  \param[in]  *pvHook: ��Ϊ����WAN״̬���Ӻ���ͳһ��ʽ
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanClear(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief LANά����ַ�ı�󣬸���mac�����б�������Ĺ���
  \param[in]  *pvNotifier: Ϊ����WAN״̬���Ӻ���ͳһ��ʽ,ʹ��ʱת��ΪIP��ʽATP_LAN_SrvInfo
  \param[in]  *pvHook: 
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_LanChange(VOS_VOID *pvNotifier, VOS_VOID *pvHook);

/*!
  \brief ����ǽ��IP Extension�Ľӿں���
  \param[in]  pcIpaddr: IP��ַ
  \param[in]  ulIpExtCmd: ATP_FW_COMMAND_TYPE_EN���Ͳ�������add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceIpExt(VOS_CHAR* pcIpaddr, VOS_UINT32 ulIpExtCmd);

/*!
  \brief ����ǽ��SIP ALG�Ľӿں���
  \param[in]  pszWanRemote: WAN�ӿ���
  \param[in]  ulPort: SIP����˿�
  \param[in]  ulRtpPortLow: RTP��ʼ�˿�
  \param[in]  ulRtpPortHigh: RTP�����˿�
  \param[in]  ulSipAlgCmd: FW_COMMAND_TYPE_EN���ͣ�add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceSipAlg(VOS_CHAR* pszWanRemote, VOS_UINT32 ulPort, 
                                      VOS_UINT32 ulRtpPortLow, VOS_UINT32 ulRtpPortHigh,
                                      VOS_UINT32 ulSipAlgCmd);

/*!
  \brief ����ǽ��FTP�Ľӿں���
  \param[in]  ulOldPort: �ɶ˿�
  \param[in]  ulNewPort: �¶˿�
  \param[in]  ulFtpCmd: FW_COMMAND_TYPE_EN���ͣ�add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceFtp(VOS_UINT32 ulOldPort,VOS_UINT32 ulNewPort,VOS_UINT32 ulFtpCmd);

#ifdef SUPPORT_ATP_VOICE
/*!
  \brief ����ǽ���������ģ��Ľӿں���
  \param[in]  lLocalPortMin: ���ض˿���ʼ�˿�
  \param[in]  lLocalPortMax: ���ض˿ڽ����˿�
  \param[in]  ulVoiceCmd: ATP_FW_CMD_TYPE_ADD  ����Ӧ�ӿ�\n
                                             ATP_FW_CMD_TYPE_REMOVE  �ر���Ӧ�ӿ�\n
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceVoice(VOS_INT32 lLocalPortMin, VOS_INT32 lLocalPortMax, VOS_UINT32 ulVoiceCmd);
#endif

/*!
  \brief ��ȡ����ǽ�Ƿ�ʹ��
  \param[in] ��
  \return VOS_FALSE/VOS_TRUE
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_BOOL ATP_FIREWALL_GetEnable(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif

