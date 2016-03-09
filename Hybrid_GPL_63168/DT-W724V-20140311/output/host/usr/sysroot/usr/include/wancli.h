
#ifndef __WANCLI_H__
#define  __WANCLI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atpconfig.h"
#include "atptypes.h"
#include "atputil.h"
#include "bhal.h"

#ifdef SUPPORT_ATP_XDSL
VOS_INT32 EquipcmdXdslInfo(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdChiptestXdsl(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayDslinfoFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDslDownFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayDslsystemBootupinfoFunc(VOS_UINT32 ulArgc, VOS_CHAR  *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagPingDslipFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplaypingDslFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
VOS_UINT32 ATP_CLI_GetDslType(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
VOS_UINT32 ATP_CLI_DiagSetBridgeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 EquipcmdSetBridgePvc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetBridgeVlan(char *pszVlan, char *pszNum);

VOS_UINT32 ATP_CLI_DiagSetRouteipFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);

#ifdef SUPPORT_ATP_WAN
VOS_UINT32 ATP_CLI_DisplayWanFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif


