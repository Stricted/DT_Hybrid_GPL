#ifndef __MLDCLI_H__
#define __MLDCLI_H__

#include "atptypes.h"

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 ATP_CLI_DiagSetMldcfg(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayMldRoute(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#endif
