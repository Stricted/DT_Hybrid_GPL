/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file telnetdcms.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef __TELNETD_CMS_H__
#define __TELNETD_CMS_H__
#include "atptypes.h"

typedef enum
{
    ATP_TELNETD_NOTIFY_PRIORITY_CHANGE_SSHDENABLE=1,
}ATP_TELNETD_NOTIFY_PRIORITY_CHANGE;

#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_Telnetd_Changed_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID  *pvNotifyHook);

#if defined(__cplusplus)
}
#endif

#endif //__TELNETD_CMS_H__
