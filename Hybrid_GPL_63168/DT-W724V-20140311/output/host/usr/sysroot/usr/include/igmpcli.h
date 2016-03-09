#ifndef __IGMPCLI_H__
#define __IGMPCLI_H__

#include "atptypes.h"

#if defined(SUPPORT_ATP_IGMP_LOGCLI)&&defined(SUPPORT_ATP_IGMP)
VOS_UINT32 Cli_DisplayIgmpproxyFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#endif
