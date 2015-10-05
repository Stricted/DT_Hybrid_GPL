/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file aclapi.h
  *ACL����ӿڶ���
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author h00163136
  *\date  2012-2-7
*/


#ifndef _ACL_API_H_
#define _ACL_API_H_

/*!
  \brief ����˿ڸı�ʱ���¶�Ӧ����
  \param[in]  pstServiceName: ��������Ʃ��FTP��TELNET��
  \param[in]  ulPort: �¶˿�
  \return ��
  \retval ��
*/
VOS_VOID AclPortNumChange(VOS_CHAR * pstServiceName,VOS_UINT32 ulPort);

/*!
  \brief 
  \param[in]  *pcServiceName: ������
  \return �� 
  \retval ��
*/
VOS_UINT32 AclDisable(VOS_VOID);
/*!
  \brief 
  \param[in] ��
  \return �� 
  \retval ��
*/
VOS_UINT32 AclDisable(VOS_VOID);


#ifdef SUPPORT_ATP_SECURITY_REDLINE_WAN_MANAGEMENT
VOS_UINT32 AclNotifyAddIPv4WanRules(VOS_VOID* pstTempWan, VOS_VOID* pvHook);
VOS_UINT32 AclNotifyDelIPv4WanRules(VOS_VOID* pstTempWan, VOS_VOID* pvHook);
#ifdef SUPPORT_ATP_FIREWALL_IPV6
VOS_UINT32 AclNotifyAddIPv6WanRules(VOS_VOID* pstTempWan, VOS_VOID* pvHook);
VOS_UINT32 AclNotifyDelIPv6WanRules(VOS_VOID* pstTempWan, VOS_VOID* pvHook);
#endif
#endif

#endif

