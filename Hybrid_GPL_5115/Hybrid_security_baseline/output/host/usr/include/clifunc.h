#ifndef _CLIFUN_H_
#define _CLIFUN_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "atptypes.h"
#include "atpconfig.h"

/****************************************************************
Function
可由产品实现的命令
*****************************************************************/


VOS_UINT32 Cli_DiagRebootFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#ifdef SUPPORT_ATP_VOICE_DT
VOS_UINT32 Cli_DebugRwslicFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif
VOS_INT32 parsehwaddr( const VOS_CHAR * str, VOS_CHAR *hwaddr );
VOS_INT32 parsexdigit(VOS_CHAR str);

#ifdef  __cplusplus
}
#endif

#endif

