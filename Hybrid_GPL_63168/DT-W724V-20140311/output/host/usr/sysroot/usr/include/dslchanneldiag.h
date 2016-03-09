#ifndef __XDSL_DIAG_H__
#define __XDSL_DIAG_H__

#include "atputil.h"
#include "cfmapi.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define ADSL_DIAG_LINE_LENGTH             100
#define ADSL_DIAG_PID_FILE                "/var/run/adsldiag.pid"
#define ADSL_DIAG_RESULT_FILE             "/var/run/adsldiag.result"
#define ADSL_DIAG_STATUS_FILE             "/var/run/deltstatus"
#define ADSL_DIAG_MAX_STATE_LENGTH        32
#define ADSL_DIAG_MAX_DATA_LENGTH         256 /* pair of 256 interger, commas and dot */
#define ADSL_DIAG_LOOPBACK_TIMEOUT_PERIOD 150   /* maximum 2.5 minutes */
#define ADSL_DIAG_LOOPBACK_ERROR          -1    /* test complete but failed */
#define ADSL_DIAG_LOOPBACK_SUCCESS        0     /* test complete and successful */
#define ADSL_DIAG_LOOPBACK_TIMEOUT        1     /* test not complete, time out */
#define ADSL_DIAG_STRING_LEN              16
#define ADSL_DIAG_CMD_LEN                 256


#ifndef __ADSLCMDADAPTER_H__

typedef struct tagDSL_DIAG_ADSL_ST
{
    VOS_CHAR acDiagnosticsState[ADSL_DIAG_MAX_DATA_LENGTH];     /* None, Requested, Complete */
    VOS_CHAR acInterface[ADSL_DIAG_MAX_DATA_LENGTH];
    VOS_INT ulACTPSDds;     /* 实际下行行功率谱密度 */
    VOS_INT ulACTPSDus;     /* 实际上行功率谱密度 */
    VOS_INT ulACTATPds;     /* 实际下行累计发送器功率 */
    VOS_INT ulACTATPus;     /* 实际上行累计发送器功率 */
    VOS_INT ulHLINSCds;     /* 下行线性表述 */
    VOS_CHAR acHLINpsds[ADSL_DIAG_MAX_DATA_LENGTH];   /* 每一副载波频率的下行线性信道特征*/    
    VOS_CHAR acQLNpsds[ADSL_DIAG_MAX_DATA_LENGTH];    /* 每一副载波频率的下行静止线噪音*/
    VOS_CHAR acSNRpsds[ADSL_DIAG_MAX_DATA_LENGTH];    /* 每一副载波频率的下行信噪比*/
    VOS_CHAR acBITSpsds[ADSL_DIAG_MAX_DATA_LENGTH];   /* 每一副载波频率的下行比特分配*/  
} DSL_DIAG_ADSL_ST;



VOS_INT32 XdslIsDiagTestRunning(VOS_VOID);
VOS_VOID XdslDiagLoopbackMain(VOS_VOID);
//static VOS_VOID XdslWaitDiagEnd(VOS_INT iParam);
//static VOS_VOID AdslDiagStartLoopback(VOS_VOID);
VOS_INT32 XdslGetDiagDataInt(DSL_DIAG_ADSL_ST *pstDiagData) ;
VOS_INT32 XdslGetDiagStr(const VOS_CHAR *pszStatStr, VOS_CHAR *ppszResult);
VOS_INT32 XdslSetDiagData(const VOS_CHAR *pszDiagState);
VOS_INT32 XdslDiagLoopbackData(VOS_INT32 iTestStatus);


#endif

#if defined(__cplusplus)
}
#endif

#endif 
