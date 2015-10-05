/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dnscmsapi.h
  *dnscms api \n
  *����DNSCMS�ĳ�ʼ��������DNS��Redirect��Resident�Ĺ���
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author t00189036
  *\date  2011-12-20
*/
#ifndef __DNSCMS_API_H__
#define __DNSCMS_API_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "msgapi.h"
#ifdef SUPPORT_ATP_TR181
#include "dnscmsdb.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define DNS_FILE_RESOVL   "/var/dns/resolv.conf"
//!\brief DNSCMS�Ĵ�����
/*!
 *These errors are reserved for dnscms only
 */
typedef enum tagDNSCMS_RET_CODE_EN
{    
    DNSCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DNSCMS),/*!< ��δ��� */
    DNSCMS_RET_MEM_ERR,/*!< �ڴ���� */
    DNSCMS_RET_NOT_SPPORTED,/*!< ��֧�ֵ����� */
    DNSCMS_RET_WAN_ERR,/*!< ��WAN�йصĴ���  */
} DNSCMS_RET_CODE_EN;

#ifdef ATP_DEBUG
#define ATP_DNSCMS_DEBUG(format, args...)       {printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);printf(format, ##args);}
#else
#define ATP_DNSCMS_DEBUG(x...)
#endif

/*****************************************************************************
*                               Init Module
******************************************************************************/
/*!
  \brief DNSCMS��ʼ��
  \param[in] ��
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
#ifdef SUPPORT_ATP_TR181
VOS_UINT32 ATP_DNSCMS_Init();
#else
VOS_UINT32 ATP_DNSCMS_Init(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);
#endif


/*****************************************************************************
*                               Redirect Module
******************************************************************************/
/*!
  \brief �����ض������DNSģ��
  \param[in]  *pstRedirect:�ض���������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
VOS_UINT32 ATP_DNSCMS_AppendRedirect(const ATP_DNS_REDIRECT_MSG_ST *pstRedirect);

/*!
  \brief ��DNSģ����ɾ���ض������
  \param[in]  *pstRedirect: �ض���������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
VOS_UINT32 ATP_DNSCMS_DeleteRedirect(const ATP_DNS_REDIRECT_MSG_ST *pstRedirect);

/*!
  \brief ���DNSģ��������ض������
  \return ATP_MSG_RET���͵Ĵ���
*/
VOS_UINT32 ATP_DNSCMS_FlushRedirect();


/*****************************************************************************
*                               Resident Module
******************************************************************************/
/*!
  \brief ����Resident����DNSģ��
  \param[in]  *pstResident: Resident�������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
VOS_UINT32 ATP_DNSCMS_AppendResident(const ATP_DNS_RESIDENT_MSG_ST *pstResident);

/*!
  \brief ��DNSģ����ɾ��Resident����
  \param[in]  *pstResident: Resident�������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
VOS_UINT32 ATP_DNSCMS_DeleteResident(const ATP_DNS_RESIDENT_MSG_ST *pstResident);

/*!
  \brief ���DNSģ��������ض������
  \return ATP_MSG_RET���͵Ĵ���
*/
VOS_UINT32 ATP_DNSCMS_FlushResident();

/*!
  \brief ����dns����ʱ����СTTL
  \return ATP_MSG_RET���͵Ĵ���
*/
VOS_UINT32 ATP_DNSCMS_SetMinTTL(VOS_UINT32 ulMinTTL);

/*!
  \brief ��DNSģ�������һ�����˹���
  \param[in]  *pstFilter: ���˹������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/
VOS_UINT32 ATP_DNSCMS_AppendFilter(const ATP_DNS_FILTER_MSG_ST *pstFilter);

/*!
  \brief ��DNSģ����ɾ��һ�����˹���
  \param[in]  *pstFilter: ���˹������Ϣ
  \return ATP_MSG_RET���͵Ĵ����DNSCMS_RET�Ĵ�����
  \retval DNSCMS_RET_PARA_ERR ���Ϊ��
*/

VOS_UINT32 ATP_DNSCMS_DeleteFilter(const ATP_DNS_FILTER_MSG_ST *pstFilter);

/*!
  \brief ���DNSģ������еĹ��˹���
  \return ATP_MSG_RET���͵Ĵ���
*/
VOS_UINT32 ATP_DNSCMS_FlushFilter();

#if (defined(SUPPORT_ATP_DNS_SERVERPRIORITY_W724V)||defined(SUPPORT_ATP_IPV6_PPP_DIAL_ON_DEMAND))		
/*!
  \brief W724v�԰��貦�ŵĶ���
  \return ATP_MSG_RET���͵Ĵ���
*/
VOS_UINT32 ATP_DNSCMS_PPPTrigerChange2OnDemand();
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif  /* __DNSCMS_API_H__ */

