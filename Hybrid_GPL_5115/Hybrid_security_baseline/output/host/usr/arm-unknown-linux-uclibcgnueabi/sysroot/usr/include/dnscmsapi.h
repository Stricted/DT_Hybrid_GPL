/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dnscmsapi.h
  *dnscms api \n
  *用于DNSCMS的初始化，设置DNS的Redirect，Resident的规则
  *\copyright 2007-2020，华为技术有限公司
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
//!\brief DNSCMS的错误码
/*!
 *These errors are reserved for dnscms only
 */
typedef enum tagDNSCMS_RET_CODE_EN
{    
    DNSCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DNSCMS),/*!< 入参错误 */
    DNSCMS_RET_MEM_ERR,/*!< 内存错误 */
    DNSCMS_RET_NOT_SPPORTED,/*!< 不支持的类型 */
    DNSCMS_RET_WAN_ERR,/*!< 与WAN有关的错误  */
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
  \brief DNSCMS初始化
  \param[in] 无
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
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
  \brief 加入重定向规则到DNS模块
  \param[in]  *pstRedirect:重定向规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/
VOS_UINT32 ATP_DNSCMS_AppendRedirect(const ATP_DNS_REDIRECT_MSG_ST *pstRedirect);

/*!
  \brief 从DNS模块中删除重定向规则
  \param[in]  *pstRedirect: 重定向规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/
VOS_UINT32 ATP_DNSCMS_DeleteRedirect(const ATP_DNS_REDIRECT_MSG_ST *pstRedirect);

/*!
  \brief 清除DNS模块的所有重定向规则
  \return ATP_MSG_RET类型的错误
*/
VOS_UINT32 ATP_DNSCMS_FlushRedirect();


/*****************************************************************************
*                               Resident Module
******************************************************************************/
/*!
  \brief 加入Resident规则到DNS模块
  \param[in]  *pstResident: Resident规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/
VOS_UINT32 ATP_DNSCMS_AppendResident(const ATP_DNS_RESIDENT_MSG_ST *pstResident);

/*!
  \brief 从DNS模块中删除Resident规则
  \param[in]  *pstResident: Resident规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/
VOS_UINT32 ATP_DNSCMS_DeleteResident(const ATP_DNS_RESIDENT_MSG_ST *pstResident);

/*!
  \brief 清除DNS模块的所有重定向规则
  \return ATP_MSG_RET类型的错误
*/
VOS_UINT32 ATP_DNSCMS_FlushResident();

/*!
  \brief 设置dns缓存时的最小TTL
  \return ATP_MSG_RET类型的错误
*/
VOS_UINT32 ATP_DNSCMS_SetMinTTL(VOS_UINT32 ulMinTTL);

/*!
  \brief 从DNS模块中添加一条过滤规则
  \param[in]  *pstFilter: 过滤规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/
VOS_UINT32 ATP_DNSCMS_AppendFilter(const ATP_DNS_FILTER_MSG_ST *pstFilter);

/*!
  \brief 从DNS模块中删除一条过滤规则
  \param[in]  *pstFilter: 过滤规则的信息
  \return ATP_MSG_RET类型的错误和DNSCMS_RET的错误码
  \retval DNSCMS_RET_PARA_ERR 入参为空
*/

VOS_UINT32 ATP_DNSCMS_DeleteFilter(const ATP_DNS_FILTER_MSG_ST *pstFilter);

/*!
  \brief 清除DNS模块的所有的过滤规则
  \return ATP_MSG_RET类型的错误
*/
VOS_UINT32 ATP_DNSCMS_FlushFilter();

#if (defined(SUPPORT_ATP_DNS_SERVERPRIORITY_W724V)||defined(SUPPORT_ATP_IPV6_PPP_DIAL_ON_DEMAND))		
/*!
  \brief W724v对按需拨号的定制
  \return ATP_MSG_RET类型的错误
*/
VOS_UINT32 ATP_DNSCMS_PPPTrigerChange2OnDemand();
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif  /* __DNSCMS_API_H__ */

