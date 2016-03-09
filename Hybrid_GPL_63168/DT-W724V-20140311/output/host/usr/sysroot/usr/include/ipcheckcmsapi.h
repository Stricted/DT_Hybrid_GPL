
/**\file ipcheckcmsapi.h
  *
  * Host related declarations for ipcheckcms and ipcheck
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/


#ifndef __IPCHECK_CMS_API_H__
#define __IPCHECK_CMS_API_H__


#include "atptypes.h"
#include "atputil.h"
#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"



#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32   ATP_IPCHECK_Init(VOS_VOID);

#ifdef SUPPORT_ATP_TR098

VOS_UINT32 ATP_IPCHECK_HostStatusChange_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID *pvNotifyHook);

VOS_UINT32 ATP_IPCHECK_HostRemove_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID *pvNotifyHook);

#endif

VOS_UINT32 ATP_IPCHECK_HostInfoUpdate_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID *pvNotifyHook);

VOS_UINT32 IpcheckHostInfoUpdateDoNotify(const ATP_HOST_INFO *pstHostInfo);


#if defined(__cplusplus)
}
#endif


#endif
