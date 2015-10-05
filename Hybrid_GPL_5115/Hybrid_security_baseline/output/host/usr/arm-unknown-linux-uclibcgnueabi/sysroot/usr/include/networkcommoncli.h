#ifndef _NETWORKCMD_H_
#define _NETWORKCMD_H_

/*#ifdef __cplusplus
extern "C" {
#endif*/
#include "atptypes.h"
#include "atpconfig.h"
#ifdef SUPPORT_ATP_MLD
#include "mldcli.h"
#endif
#ifdef SUPPORT_ATP_FIREWALL
#include "firewallcli.h"
#endif
#ifdef SUPPORT_ATP_RADVD
#include "radvdcli.h"
#endif
#ifdef SUPPORT_ATP_ROUTE
#include "routecli.h"
#endif

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 ATP_CLI_ReadTestHelp(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#ifdef SUPPORT_ATP_ACC
VOS_UINT32 ATP_CLI_DispalyAccKrnlInfoFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#ifdef SUPPORT_ATP_PRSITE
VOS_UINT32 ATP_CLI_DispalyPrsiteKrnlInfoFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

/*#ifdef  __cplusplus
}
#endif*/

#endif
