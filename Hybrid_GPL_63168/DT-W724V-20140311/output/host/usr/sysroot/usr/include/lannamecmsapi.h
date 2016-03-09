/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file lannamecmsapi.h
  *lanname cms api
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author t00189036
  *\date  2012-2-13
*/

#ifndef __LANNAMECMS_API_H__
#define __LANNAMECMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief lanname ��ʼ������
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_LANNAME_Init(VOS_VOID);

/*!
  \brief lanname ȥ��ʼ������
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_LANNAME_UnInit(VOS_VOID);

/*!
  \brief lanname ����LAN��ά����ַ������������
  \param[in] pszDomainSearch  DHCPServer�·���PC��������PC��������DNS����ʱ�Ὣ��������Ϊ��׺
  \return ��
*/
VOS_VOID ATP_LANNAME_SetDomainSearch(VOS_CHAR* pszDomainSearch);

/*!
  \brief lanname ���±�������ǰ׺
  \param[in] pszLanNameLocalHost  ��������ǰ׺
  \return ��
*/
VOS_VOID ATP_LANNAME_SetLocalHost(VOS_CHAR* pszLanNameLocalHost);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __LANNAMECMS_API_H__ */
