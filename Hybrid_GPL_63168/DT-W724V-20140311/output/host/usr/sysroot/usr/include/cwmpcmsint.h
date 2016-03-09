/******************************************************************************
  版权所有  : 2009，华为技术有限公司
  文 件 名  : dnsapi.h
  作    者  : c47036
  版    本  : v1.0
  创建日期  : 2009-2-20
  描    述  : DNS API

  历史记录      :
   1.日    期   : 2009-2-20
     作    者   : c47036
     修改内容   : 完成初稿

******************************************************************************/

#ifndef __CWMP_CMSINT_API_H__
#define __CWMP_CMSINT_API_H__

#include "atpconfig.h"
#include "atputil.h"



#ifdef SUPPORT_ATP_REQ_PORT
#define CONNREQPORT_MAX  65535
#endif

/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C"
{
#endif
VOS_VOID CwmpTakeEffectProc(VOS_VOID *pvHook);
VOS_UINT32 EasySupportUpdateStatus(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);

#ifdef SUPPORT_ATP_CWMP_DOWNLOAD_QUEUE
VOS_UINT32 ATP_CWMP_InitSchTimeInfo();
//VOS_UINT32 ATP_CWMP_UnInitSchTimeInfo();
#endif

#if defined(__cplusplus)
}
#endif

#endif
