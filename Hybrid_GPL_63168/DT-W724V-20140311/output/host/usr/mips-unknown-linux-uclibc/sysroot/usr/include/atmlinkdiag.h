/******************************************************************************
  版权所有  : 2009，华为技术有限公司
  文 件 名  : atmdiag.h
  作    者  : 
  版    本  : v1.0
  创建日期  : 2009-1-15
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-1-15
     作    者   : 
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __XTM_ATM_DIAG_H__
#define __XTM_ATM_DIAG_H__

#define ATM_REQUESTED_BIT           0x01
#define ATM_REPETITIONS_BIT         0x02
#define ATM_TIMEOUT_BIT             0x04
#define ATM_INTERFACE_BIT           0x08
#define ATM_DIAGMASK_BIT            0x0F

#define XTM_BUFF_LEN_16             16
#define XTM_BUFF_LEN_32             32
#define XTM_BUFF_LEN_128            128
#define XTM_BUFF_LEN_256            256
#define XTM_BUFF_LEN_512            512
#define XTM_BUFF_LEN_264            264
#define XTM_MAX_CMD_LEN             512

#define ATM_DIAG_MAX_STATE_LEN          32

#define ATM_DIAG_LINE_LEN           100
#define ATM_DIAG_PID_FILE           "/var/run/diag.pid"
#define ATM_DIAG_RESULT_ATM_FILE    "/var/diagatm.result"

#define ATM_MAX_CMD_LEN             512

#if 1
typedef struct AtmAddr
{
    VOS_UINT32 ulPortMask;
    VOS_UINT32 ulVpi;
    VOS_UINT32 ulVci;
} ATM_ADDR, *PATM_ADDR;

typedef struct tagATM_DIAG_ST
{
    VOS_CHAR   acDiagnosticsState[XTM_BUFF_LEN_32]; /* None, Requested, Complete */
    VOS_CHAR   acInterface[XTM_BUFF_LEN_256];
    VOS_UINT32 ulRepetition;     
    VOS_UINT32 ulTimeout;                       /* in ms */
    VOS_UINT32 ulSuccessCount;    
    VOS_UINT32 ulFailCount;
    VOS_UINT32 ulAvgResponseTime;
    VOS_UINT32 ulMinResponseTime;
    VOS_UINT32 ulMaxResponseTime;
    ATM_ADDR   stPvc;
    VOS_UINT32 ulState;
} ATM_DIAG_ST;
#endif

// FUNC declare
VOS_VOID XtmDiagMain(const ATM_DIAG_ST *pstDiagData, VOS_INT32 fpid);
VOS_VOID XtmDiagStart(const ATM_DIAG_ST *pstDiagData);
VOS_INT32 XtmIsDiagRunning(VOS_VOID);

//VOS_UINT32 XtmSetDiagData(const ATM_DIAG_ST *pstDiagData);
//VOS_UINT32 XtmGetDiagData(ATM_DIAG_ST *pstDiagDat );
VOS_VOID XtmDiagStop(VOS_VOID );

#endif

