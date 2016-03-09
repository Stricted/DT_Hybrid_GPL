
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
    ATP_CWMP_NOTIFY_PRIORITY_WRITE_EASYSUPPORT_STATUS,

}ATP_CWMP_NOTIFY_PRIORITY_CHANGE;

/*
 save cwmp wan info 
 action:
 1. dsl up   ---> check current item if lte exist
                    make lte down and delete item then 
                    add item and save dsl cwmp wan info
 2. dsl down ---> delete item
 3. lte up   ---> check current item if dsl exist
                    ignore else 
                    add item and save
 4. lte down ---> delete item
*/
typedef enum
{
    CWMP_WAN_TYPE_DSL_IPV4=1,
    CWMP_WAN_TYPE_DSL_IPV6,        
    CWMP_WAN_TYPE_LTE_IPV4,
    CWMP_WAN_TYPE_LTE_IPV6,
}CWMP_WAN_TYPE;

typedef struct tagATP_CWMP_WAN_INFO
{
    struct tagATP_CWMP_WAN_INFO *pstNext;
    ATP_CWMP_WANIP_ST   *pstCwmpWanInfo;
    CWMP_WAN_TYPE       enCwmpWanType;    
}ATP_CWMP_WAN_INFO_ST;


#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_Cwmp_Changed_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID  *pvNotifyHook);
VOS_UINT32 CwmpWanInfoUpdate(ATP_CWMP_WANIP_ST *pstWanIP, CWMP_WAN_TYPE enWanType,  VOS_BOOL bWanUp);

#if defined(__cplusplus)
}
#endif


#endif
