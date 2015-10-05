
#ifndef __RADVDCLI_H__
#define __RADVDCLI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atpconfig.h"
#include "atptypes.h"
#include "atputil.h"
#include "bhal.h"

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 Cli_DiagSetRadvdSendRaFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdManageFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdOtherFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdIgnoreFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdHomeAgentFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdOnLinkFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdAutonomousFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdRouterAddrFlag(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdDefPreValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdLinkMtuValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdCurHopLimitValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdCurMinRaInterValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdCurMaxRaInterValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdMinDelayBetRaValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdReachableTimeValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdRetranTimeValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdDefLifeTimeValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdPrefixValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdPrefixLengthValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdPreLifeTimeValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdValidLifeTimeValue(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdDefaultCfg(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSetRadvdFlagCfg(VOS_INT32 lFlagID,VOS_INT32 lFlag,FILE *pfStream);
VOS_UINT32 Cli_DiagSetRadvdGenerateCfgFile();
VOS_UINT32 Cli_DiagSetRadvdHelp(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagReSetRadvdProc();
VOS_UINT32 Cli_DiagSetRadvdPrint();
VOS_UINT32 Cli_DiagStopRadvdProc();
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif


