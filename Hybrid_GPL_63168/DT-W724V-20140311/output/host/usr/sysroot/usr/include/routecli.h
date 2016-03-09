#ifndef __ROUTECLI_H__
#define __ROUTECLI_H__

#include "atptypes.h"
#include "cliapi.h"

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 Cli_DiagSetAddDefaultRoute(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetDelDefaultRoute(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayRoute(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayTable(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_ReadTestSetRouteCmdHelp(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#endif
