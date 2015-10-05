#ifndef __TR143_API_H__
#define __TR143_API_H__

#include "cmsapi.h"

#if defined(__cplusplus)
extern "C" {
#endif



#ifdef SUPPORT_ATP_TR143_UDPECHO
VOS_UINT32 ATP_TR143_Init(VOS_VOID);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __TR143_API_H__ */
