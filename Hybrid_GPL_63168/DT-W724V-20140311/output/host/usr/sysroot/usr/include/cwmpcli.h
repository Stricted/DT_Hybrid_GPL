#ifndef __CWMPCLI_H__
#define __CWMPCLI_H__
#include "atptypes.h"
#include "cliapi.h"
#ifdef SUPPORT_ATP_CWMP
VOS_UINT32 Cli_DispalyCwmpInfoFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpAuthtypeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpWanFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#ifdef SUPPORT_ATP_TR181
#ifdef  SUPPORT_ATP_DEBUG_VERSION_CWMP
VOS_UINT32 Cli_SetCwmpParaFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif
#endif
VOS_UINT32 Cli_SetCwmpAcsUrFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpUsrFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpPwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpConReqUsrFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetCwmpConReqPwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif
#endif
