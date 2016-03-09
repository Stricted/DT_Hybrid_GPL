#ifndef __LOGCLI_H__
#define __LOGCLI_H__

#include "atptypes.h"
#include "cliapi.h"

#ifdef SUPPORT_ATP_LOG

VOS_UINT32 Cli_SetLogRecordItemFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetLogFlushFunc(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_SetLogQueryFunc(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_SetLogClearFunc(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_SetLogKlogFunc(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_SetLogCommand(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_SetLogCleanFunc(VOS_UINT32 ulArgc, VOS_CHAR* acLastPara[]);
VOS_UINT32 Cli_DectLogStartFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DectLogStopFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DectLogReadFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DectLogClearFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

#endif

#endif // __LOGCLI_H__
