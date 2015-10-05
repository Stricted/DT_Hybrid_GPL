
/****************************************************************************
 * File Name:     cwmpcms.h
 * Author:        ATP team
 * Date:           2011/12/17
 * Descripton:   LAN cms inner interface declarations
 *****************************************************************************/

#ifndef _CWMP_CMS_H
#define _CWMP_CMS_H

#include "atptypes.h"


typedef enum
{
    ATP_CWMP_NOTIFY_PRIORITY_CHANGE_TR111Url=1,
#ifdef SUPPORT_ATP_FON
    ATP_CWMP_NOTIFY_PRIORITY_CHANGE_EASYSUPPORT,
#endif
}ATP_CWMP_NOTIFY_PRIORITY_CHANGE;


#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_Cwmp_Changed_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID  *pvNotifyHook);
/* < DTS2013052505125 xiazhongyue 20130603 begin */ 
VOS_BOOL ValidUserName(VOS_CHAR *pcSrc, VOS_UINT32 ulLen);
/*   DTS2013052505125 xiazhongyue 20130603 end> */
#if defined(__cplusplus)
}
#endif


#endif
