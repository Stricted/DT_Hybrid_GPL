/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file sntpcmsapi.h 
  *sntpcms api \n
  *����sntpcms�ĳ�ʼ���������ݿ�Ľ���
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author t00189036
  *\date  2012-2-4
*/

#ifndef __SNTP_CMS_API_H__
#define __SNTP_CMS_API_H__


#include "msgapi.h"
#include "cfmapi.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief SNTPCMS�ĳ�ʼ���������ݿ��ж�ȡ��Ϣ��д�������ļ�������sntp����
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_SNTP_Init(VOS_VOID);

typedef enum
{
    ATP_SNTP_NOTIFY_PROC_CWMP=1,
    ATP_SNTP_NOTIFY_PROC_LOG,
    ATP_SNTP_NOTIFY_PROC_VOICE,   
    ATP_SNTP_NOTIFY_PROC_SYSINFO,   
}ATP_SNTP_NOTIFY_PROC;

VOS_UINT32 ATP_SntpNotifyProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);


const VOS_CHAR *ATP_SNTP_GetStatus(VOS_VOID);

#if defined(__cplusplus)
}
#endif /* __cplusplus */


#endif
