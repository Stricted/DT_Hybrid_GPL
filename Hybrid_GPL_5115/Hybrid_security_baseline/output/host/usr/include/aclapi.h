/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file aclapi.h
  *ACL对外接口定义
  *\copyright 2007-2020，华为技术有限公司
  *\author h00163136
  *\date  2012-2-7
*/


#ifndef _ACL_API_H_
#define _ACL_API_H_

/*!
  \brief 服务端口改变时更新对应规则
  \param[in]  pstServiceName: 服务名，譬如FTP、TELNET等
  \param[in]  ulPort: 新端口
  \return 无
  \retval 无
*/
VOS_VOID AclPortNumChange(VOS_CHAR * pstServiceName,VOS_UINT32 ulPort);

/*!
  \brief 
  \param[in]  *pcServiceName: 服务名
  \return 无 
  \retval 无
*/
VOS_UINT32 AclDisable(VOS_VOID);
/*!
  \brief 
  \param[in] 无
  \return 无 
  \retval 无
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

