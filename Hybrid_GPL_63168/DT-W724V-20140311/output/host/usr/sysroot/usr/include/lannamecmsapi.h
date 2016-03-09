/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file lannamecmsapi.h
  *lanname cms api
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-2-13
*/

#ifndef __LANNAMECMS_API_H__
#define __LANNAMECMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief lanname 初始化函数
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_LANNAME_Init(VOS_VOID);

/*!
  \brief lanname 去初始化函数
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_LANNAME_UnInit(VOS_VOID);

/*!
  \brief lanname 更新LAN的维护地址、域名、主机
  \param[in] pszDomainSearch  DHCPServer下发给PC的域名，PC发起主机DNS请求时会将该域名作为后缀
  \return 无
*/
VOS_VOID ATP_LANNAME_SetDomainSearch(VOS_CHAR* pszDomainSearch);

/*!
  \brief lanname 更新本机域名前缀
  \param[in] pszLanNameLocalHost  本机域名前缀
  \return 无
*/
VOS_VOID ATP_LANNAME_SetLocalHost(VOS_CHAR* pszLanNameLocalHost);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __LANNAMECMS_API_H__ */
