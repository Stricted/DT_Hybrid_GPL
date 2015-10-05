
/****************************************************************************
 * File Name:     ipdrcmsapi.h
 * Author:        ATP team
 * Date:           2013/9/10
 * Descripton:   IPDR cms  interface declarations
 *****************************************************************************/

#ifndef _IPDR_CMS_API_H
#define _IPDR_CMS_API_H

#include "atptypes.h"


#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_IPDR_Init(VOS_VOID);
VOS_VOID ATP_IPDR_START();
VOS_VOID ATP_IPDR_STOP();
VOS_UINT32 ATP_IPDR_GetUpWanPath(VOS_CHAR *pcWanPath, VOS_UINT32 ulLen);

#if defined(__cplusplus)
}
#endif


#endif
