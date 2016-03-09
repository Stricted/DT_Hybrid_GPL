#ifndef __ETHCLI_H__
#define  __ETHCLI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atpconfig.h"
#include "atptypes.h"
#include "atputil.h"
#include "bhal.h"

#if defined(BCM_ETH_PWRSAVE) && defined(BCM_ETH_HWAPD_PWRSAVE)

VOS_UINT32 ATP_CLI_DisplayEthPowerSave(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 ATP_CLI_SetEthPowerSave(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif


