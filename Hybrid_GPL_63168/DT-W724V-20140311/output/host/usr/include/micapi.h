#ifndef __ATP_MIC_API_H__
#define __ATP_MIC_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#include "atptypes.h"
#include "msgapi.h"

/* Utility functions */

typedef enum tagATP_MIC_RET_CODE_EN
{
    ATP_MIC_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_MIC),
    ATP_MIC_RET_MEM_ERR,
    ATP_MIC_RET_NOT_INITED,
    ATP_MIC_RET_SOCK_ERR,
    ATP_MIC_RET_NOT_SPPORTED,
    ATP_MIC_RET_INVALID_DEST,
    ATP_MIC_RET_INTERNAL_ERR,
    ATP_MIC_RET_TIMEOUT
} ATP_MIC_RET_CODE_EN;

const VOS_CHAR *ATP_MIC_GetVersion(void);

/*******************************************************
//                 _________                          _____________
//                |              |                         |                     |
//                | MsgClt    |                        | InetdMngr      |
//                |________|                         |____________|
//                   \         \                                       |
//                    \       __\________            ______|_______
//                     \      |                |           |                      |
//                      \     |  MsgSvr   |           |    SockSvr       |
//                        \   |_________|           |_____________|
//                    ____\________________________|______
//                   |                                                             |
//                   |            SockBase                                   |
//                   |___________________________________|
//
********************************************************/

/** SockBase Interfaces */
#if 1//#ifdef SUPPORT_ATP_HTTP
#define ATP_MIC_SockBaseReg  ATP_UTIL_SockBaseReg

#define ATP_MIC_SockBaseListen ATP_UTIL_SockBaseListen

#define ATP_MIC_SockBaseDelete ATP_UTIL_SockBaseDelete

#define ATP_MIC_SockBaseDestroy ATP_UTIL_SockBaseDestroy

#define ATP_MIC_SockBaseGetMaxFd ATP_UTIL_SockBaseGetMaxFd

#define ATP_MIC_SockBaseStart() ATP_UTIL_SockBaseStart(0)
#else
typedef enum tagATP_SOCK_FLAG_EN
{
    ATP_SOCK_DFT        = 0,        // Default: TCP, no SSL, IPv4
    ATP_SOCK_UDP        = 0x01,     // Socket type, 0: TCP; 1: UDP
    ATP_SOCK_SSL        = 0x02,     // Whether SSL, 0: No SSL; 1: SSL
    ATP_SOCK_IP_VER     = 0x04,     // Version, 0: IPv4; 1: IPv6
    ATP_SOCK_UNIX       = 0x08,

    // Allowed combinations
    ATP_SOCK_TCP4_SSL   = ATP_SOCK_SSL,
    ATP_SOCK_TCP6_SSL   = ATP_SOCK_SSL | ATP_SOCK_IP_VER,
    ATP_SOCK_UDP6       = ATP_SOCK_UDP | ATP_SOCK_IP_VER,
} ATP_SOCK_FLAG_EN;

VOS_INT32 ATP_UTIL_SocketCreateServer(
                                const VOS_CHAR      *pcLocalAddr,
                                VOS_UINT32          ulPort,
                                ATP_SOCK_FLAG_EN    enSockType);

typedef struct tagATP_SOCK_BASE_ST ATP_SOCK_BASE_ST;
typedef VOS_VOID (* PFMicSockEvtProc)(const ATP_SOCK_BASE_ST *pstSockBase);
struct tagATP_SOCK_BASE_ST
{
    VOS_INT32                   lSock;
    VOS_INT32                   lSockFlags;
    PFMicSockEvtProc            pfProc;
    VOS_VOID                    *pvHook;
    struct tagATP_SOCK_BASE_ST  *pstNext;
};

extern VOS_BOOL    g_bBaseExit;

VOS_UINT32 ATP_MIC_SockBaseRegEvtProc();

VOS_UINT32 ATP_MIC_SockBaseReg(
                                VOS_INT32           lSock,
								ATP_SOCK_FLAG_EN    lSockFlag,
								PFMicSockEvtProc    pfProc,
								VOS_BOOL            bListen,
								VOS_VOID            *pvHook,
								ATP_SOCK_BASE_ST    **ppstRet);

VOS_VOID ATP_MIC_SockBaseListen(const ATP_SOCK_BASE_ST *pstSockBase, VOS_BOOL bListen);

VOS_VOID ATP_MIC_SockBaseDelete(ATP_SOCK_BASE_ST *pstSockBase);

VOS_VOID ATP_MIC_SockBaseDestroy();

VOS_INT32 ATP_MIC_SockBaseGetMaxFd();

VOS_UINT32 ATP_MIC_SockBaseStart();
#endif

#ifdef SUPPORT_ATP_MIC_INETD
/** SockSvr Interfaces
  *     Only inetd uses SockSvr, so we add the macro with INETD
  */
typedef struct tagATP_SOCK_SVR_ST ATP_SOCK_SVR_ST;
typedef VOS_VOID (* PFMicSockSvrProc)(ATP_SOCK_SVR_ST           *pstSockSvr,
							            const ATP_SOCK_BASE_ST    *pstSockBase);

#define ATP_MAX_SOCKCLT_PER_SVR		(8)
struct tagATP_SOCK_SVR_ST
{
    PFMicSockSvrProc                pfProc;
    ATP_SOCK_BASE_ST                *pstSockBases[ATP_MAX_SOCKCLT_PER_SVR];
    VOS_UINT16                      ausPorts[ATP_MAX_SOCKCLT_PER_SVR];
    VOS_VOID                        *pvHook;
};

ATP_SOCK_SVR_ST *ATP_MIC_SockSvrCreate(
                            PFMicSockSvrProc    pfProc,
                            VOS_VOID            *pvHook);

VOS_VOID ATP_MIC_SockSvrDestroy(ATP_SOCK_SVR_ST *pstSockSvr);

VOS_UINT32 ATP_MIC_SockSvrAppendSock(
                            VOS_INT32           lSock,
                            ATP_SOCK_FLAG_EN    enSockType,
                            VOS_UINT32          ulPort,
                            ATP_SOCK_SVR_ST     *pstSvr);

VOS_UINT32 ATP_MIC_SockSvrListen(const ATP_SOCK_SVR_ST *pstSvr, VOS_BOOL bListen);

VOS_VOID ATP_MIC_SockSvrDeleteOne(ATP_SOCK_SVR_ST *pstSockSvr, ATP_SOCK_BASE_ST* pstSockBase);

VOS_VOID ATP_MIC_SockSvrDeleteAll(ATP_SOCK_SVR_ST *pstSockSvr);

/** Inetd Mgnr Interfaces  */
typedef ATP_SOCK_SVR_ST *(* PFMicInetdInitProc)(VOS_VOID);
typedef struct tagATP_INETD_PROC_ST ATP_INETD_PROC_ST;
typedef VOS_UINT32 (* PFMicInetdResumeListen)(const ATP_INETD_PROC_ST *pstInetdItem);

typedef VOS_BOOL (* PFMicCreateSocketProc)(const VOS_CHAR *pcLanIP,
                                ATP_INETD_PROC_ST *pstInetdProc,
                                VOS_INT32 alNewFds[ATP_MAX_SOCKCLT_PER_SVR]);

typedef enum tagATP_INETD_APP_TYPE_EN
{
    ATP_INETD_APP_SOCK_LISTEN,          // mic create socket and listen
    ATP_INETD_APP_SOCK_LISTEN_UPDATE,   // mic create socket and listen and send fd to app when updated
    ATP_INETD_APP_MONITOR_ENABLE,       // mic monitor app only
    ATP_INETD_APP_MONITOR_DISABLE       // mic monitor app only
} ATP_INETD_APP_TYPE_EN;

struct tagATP_INETD_PROC_ST
{
    struct tagATP_INETD_PROC_ST *pstNext;

    /* Inetd app information, filled by user */
    const VOS_CHAR              *pcAppName;     /* Application Name */
    const VOS_CHAR              *pcArgs;

    PFMicCreateSocketProc       pfSockProc;
    ATP_INETD_APP_TYPE_EN       enAppType;

    ATP_MSG_QUEUE_ST            *pstMsgQueue;   /* Msg queue for this proc during it's idle */
    ATP_SOCK_SVR_ST             *pstSockSvr;    /* SockSvr struct, internal usage */
    VOS_INT32                   lPid;
    VOS_BOOL                    bLaunch;
};

VOS_UINT32 ATP_MIC_InetdMngrInit();

VOS_UINT32 ATP_MIC_RegInetdApp(const VOS_CHAR *pcAppName, const VOS_CHAR *pcArgs,
                                PFMicCreateSocketProc  pfSockProc,
                                ATP_INETD_APP_TYPE_EN  enAppType);

VOS_VOID ATP_MIC_InetdMngrDestroy();

VOS_UINT32 ATP_MIC_InetdLaunchApp(
                                    ATP_INETD_PROC_ST           *pstInted);

#if 0
VOS_VOID ATP_MIC_InetdAppChgAddr(
                                    const ATP_INETD_PROC_ST     *pstInetd,
                                    const VOS_CHAR              *pcNewAddr,
                                    VOS_BOOL                    bListen);
#endif

VOS_VOID ATP_MIC_InetdAppListen(const ATP_INETD_PROC_ST *pstInetd, VOS_BOOL bListen);

ATP_INETD_PROC_ST *ATP_MIC_InetdFindAppByName(const VOS_CHAR *pcAppName);

ATP_INETD_PROC_ST *ATP_MIC_InetdFindAppByPid(VOS_INT32 lPid);

#endif

/** MsgSvr Interfaces */
VOS_UINT32 ATP_MIC_MsgSvrCreate(const VOS_CHAR *pcSvrPath);
#ifdef SUPPORT_ATP_MULTI_BOARD_COMMUNICATION
// different proccess for different app
typedef VOS_UINT32 (*PFMicHeartBeatProcFunc)(VOS_VOID       *pvArg);

typedef struct tagATP_MSG_HEARTBEAT_ST
{
    struct tagATP_MSG_HEARTBEAT_ST *pstNext;
    VOS_CHAR acAppName[64];
    VOS_UINT32 ulStartTime;
    VOS_UINT32 ulEndTime;
    VOS_UINT32 ulFailureCounter;  
    // the Msg packet sequeue numer
    VOS_UINT32 ulSeqNum;
    VOS_BOOL   bEnabled;
    PFMicHeartBeatProcFunc pfProcFunc;    
    ATP_UTIL_TIMER_ST *pstHeartBeatTimer;    
    ATP_UTIL_TIMER_ST *pstHBOverTimeTimer;
}ATP_MSG_HEARTBEAT_ST;

VOS_VOID ATP_MIC_MsgCltHBAppend(VOS_CONST_CHAR *pcAppName);


VOS_VOID ATP_MIC_HBRegCenter();

// reg for on App with app name and process function
VOS_UINT32 ATP_MIC_HBReg(VOS_CONST_CHAR *pcAppName, PFMicHeartBeatProcFunc pfProc);


VOS_UINT32 ATP_MIC_MsgSvrCreate_IP(const VOS_CHAR *pcLocalAddr, VOS_UINT32 ulPort);

VOS_UINT32 ATP_MIC_MsgCltHBRecvProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID MicMsgCltEvtCallback_IP(const ATP_SOCK_BASE_ST *pstSockBase);

VOS_VOID ATP_MIC_HBExmicDel(VOS_VOID);

#endif
/** MsgClt Interfaces */
typedef struct tagATP_MSG_CLT_ST
{
    const VOS_CHAR                  *pcCltName;     /* Message Client Name */
    ATP_SOCK_BASE_ST                *pstSockBase;   /* SockBase struct */

    VOS_VOID                        *pvHook;        /* Hook to parent */
    struct tagATP_MSG_CLT_ST        *pstNext;
} ATP_MSG_CLT_ST;

VOS_UINT32 ATP_MIC_MsgCltAppend(
                                    VOS_INT32               lSock,
                                    const ATP_MSG_HEADER_ST *pstRegMsg);

VOS_VOID MicMsgCltEvtCallback(const ATP_SOCK_BASE_ST *pstSockBase);


VOS_UINT32 ATP_MIC_MsgCltGetNumOfApp(const VOS_CHAR *pcMsgCltName);

ATP_MSG_CLT_ST *ATP_MIC_MsgCltFindByName(const VOS_CHAR *pcMsgCltName);

VOS_BOOL ATP_MIC_MsgCltSwitchDest(const VOS_CHAR *pcAppName);

VOS_VOID ATP_MIC_DefaultMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

/*
 *  Utilities
 */
VOS_VOID ATP_MIC_NotifyEvent(const ATP_MSG_HEADER_ST *pstMsg);

#ifdef ATP_DEBUG
#define ATP_MIC_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#define ATP_MIC_CMM_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}

#else
#define ATP_MIC_CMM_DEBUG(format, args...)
#define ATP_MIC_DEBUG(x...)
#endif

#define ATP_MIC_TRACE ATP_MSG_Printf

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif


