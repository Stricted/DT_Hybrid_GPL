/******************************************************************************
  ��Ȩ����  : 2009����Ϊ�������޹�˾
  �� �� ��  : dnsapi.h
  ��    ��  : c47036
  ��    ��  : v1.0
  ��������  : 2009-2-20
  ��    ��  : DNS API

  ��ʷ��¼      :
   1.��    ��   : 2009-2-20
     ��    ��   : c47036
     �޸�����   : ��ɳ���

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
