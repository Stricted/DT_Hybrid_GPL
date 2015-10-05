
/****************************************************************************
 * File Name:     ipdrcmsinit.h
 * Author:        ATP team
 * Date:           2011/12/17
 * Descripton:   LAN cms inner interface declarations
 *****************************************************************************/

#ifndef _ipdrinit_CMS_H
#define _ipdrinit_CMS_H

#include "atptypes.h"


typedef enum
{
    ATP_CWMP_NOTIFY_PRIORITY_CHANGE_TR111Url=1,
#ifdef SUPPORT_ATP_FON
    ATP_CWMP_NOTIFY_PRIORITY_CHANGE_EASYSUPPORT,
#endif
    ATP_CWMP_NOTIFY_PRIORITY_WRITE_EASYSUPPORT_STATUS,

}ATP_CWMP_NOTIFY_PRIORITY_CHANGE;


#if defined(__cplusplus)
extern "C" {
#endif



#if defined(__cplusplus)
}
#endif


#endif
