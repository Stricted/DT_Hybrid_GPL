#ifndef __IGMPCLI_H__
#define __IGMPCLI_H__

#include "atptypes.h"

#ifdef SUPPORT_ATP_USBMOUNT
VOS_UINT32 Cli_DiagSdcard1TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSdcard2TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb1TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb2TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb2SsidFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb1speedTestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb2speedTestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

#endif
