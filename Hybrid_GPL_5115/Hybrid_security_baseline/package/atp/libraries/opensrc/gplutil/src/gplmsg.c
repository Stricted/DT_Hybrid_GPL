#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>

#include "atptypes.h"
#include "msgapi.h"
#include "atplog.h"

#ifndef SUPPORT_DESKTOP
//#include "commondrv.h"
#endif



ATP_THREAD_SAFE ATP_MSG_CONTEXT_ST g_stMsgCtx = {"", -1, 0, NULL, 0, NULL, -1, NULL,0};

/******************************************************************************
  函数名称  : ATP_MSG_Init
  功能描述  : 初始化消息组件
  输入参数  : 
              1.  pszAppName:    进程名称，可以为空。
                              为空时，消息服务器随机分配进程名称。
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
static VOS_UINT32 MsgRegister()
{
    VOS_CHAR acBuf[sizeof(ATP_MSG_HEADER_ST)];
    ATP_MSG_HEADER_ST    *pstSend;
    ATP_MSG_HEADER_ST    *pstRecv;
    VOS_UINT32 ulRet;

    memset((VOS_VOID *)acBuf, 0, sizeof(acBuf));

    pstSend = (ATP_MSG_HEADER_ST *)acBuf;
    pstSend->ulMsgType = ATP_MSG_REGISTER;
    pstSend->usSeqNum = 0;
    pstSend->ulDataLen = 0;
    snprintf(pstSend->aucSendApp, (ATP_MSG_MAX_NAME_LEN + 1), "%s", g_stMsgCtx.aucAppName);

    pstRecv = NULL;
    ulRet = ATP_MSG_SendAndRecv(pstSend, &pstRecv, 0);
    if (VOS_OK != ulRet)
    {
        return ulRet;
    }
    if (NULL == pstRecv)
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    // Backup app name
    memcpy((VOS_VOID *)g_stMsgCtx.aucAppName, (VOS_VOID *)pstRecv->aucRecvApp,
           ATP_MSG_MAX_NAME_LEN);
    g_stMsgCtx.aucAppName[ATP_MSG_MAX_NAME_LEN] = '\0';
    free((VOS_VOID *)pstRecv);

    if ('\0' == g_stMsgCtx.aucAppName[0])
    {
        return ATP_MSG_RET_APPNAME_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 ATP_MSG_Init(const VOS_CHAR *pszAppName)
{
    VOS_UINT32 ulRet;
    VOS_INT32 lIra;
    struct sockaddr_un  stAddr;

    signal(SIGPIPE, SIG_IGN);

    // Already inited
    if (g_stMsgCtx.lSock >= 0)
    {
        return VOS_OK;
    }

    if (NULL != pszAppName)
    {
        snprintf(g_stMsgCtx.aucAppName, (ATP_MSG_MAX_NAME_LEN + 1), "%s", pszAppName);
    }
    else
    {
        g_stMsgCtx.aucAppName[0] = '\0';
    }

    // Create socket
    g_stMsgCtx.lSock = socket(AF_UNIX, SOCK_STREAM, 0);
    //g_stMsgCtx.lSock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_stMsgCtx.lSock < 0)
    {
        return ATP_MSG_RET_SOCKET_ERR;
    }

    //Set some flags
    lIra = 1;
    setsockopt(g_stMsgCtx.lSock, SOL_SOCKET, SO_REUSEADDR, (VOS_CHAR *)&lIra, sizeof(lIra));
    lIra = 1;
    fcntl(g_stMsgCtx.lSock, F_SETFD, FD_CLOEXEC);

    // Connect
    memset((VOS_CHAR *)(&stAddr), 0, sizeof(stAddr));
    stAddr.sun_family = AF_UNIX;
    //stAddr.sin_family = AF_INET;
    strncpy(stAddr.sun_path, ATP_MSG_SVR_PATH, sizeof(stAddr.sun_path));
    //stAddr.sin_addr.s_addr = inet_addr(ATP_MSG_LOCAL_HOST);
    //stAddr.sin_port = htons(ATP_MSG_LOCAL_PORT);
    if (connect(g_stMsgCtx.lSock,
                    (struct sockaddr *)&stAddr,
                    sizeof(stAddr)) < 0)
    {
        close(g_stMsgCtx.lSock);
        g_stMsgCtx.lSock = -1;
        ATP_MSG_DEBUG("Connect to msgsvr failed %d.\n", errno);
        return ATP_MSG_RET_SOCKET_ERR;
    }

    ulRet = MsgRegister();
    if (VOS_OK != ulRet)
    {
        ATP_MSG_DEBUG("Register to msgsvr failed %x.\n", ulRet);
        ATP_MSG_Destroy();
    }

    return ulRet;
}

/******************************************************************************
  函数名称  : ATP_MSG_Destroy
  功能描述  : 去初始化消息组件
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_VOID ATP_MSG_Destroy()
{
    if (g_stMsgCtx.lSock > 0)
    {
        close(g_stMsgCtx.lSock);
    }
    memset((VOS_VOID *)(&g_stMsgCtx), 0, sizeof(g_stMsgCtx));
    g_stMsgCtx.lSock = -1;
}

/*
 *    Message Utility Interfaces
 */
VOS_VOID ATP_MSG_TraceProc(VOS_INT32 lNewFd)
{
    // Close old trace
    if (NULL != g_stMsgCtx.pfTraceFile)
    {
        fclose(g_stMsgCtx.pfTraceFile);
    }
    g_stMsgCtx.pfTraceFile = NULL;
    if (g_stMsgCtx.lTraceFd >= 0)
    {
        close(g_stMsgCtx.lTraceFd);
        g_stMsgCtx.lTraceFd = -1;
    }
    if (0 == g_stMsgCtx.ulDbgLevel)     // Close debug info
    {
        if (lNewFd >= 0)
        {
            close(lNewFd);
        }
        return;
    }
    if (lNewFd >= 0)
    {
        // Open new trace file
        g_stMsgCtx.pfTraceFile = fdopen(lNewFd, "w");
        if (NULL == g_stMsgCtx.pfTraceFile)
        {
            close(lNewFd);
            return;
        }
        g_stMsgCtx.lTraceFd = lNewFd;
    }
}

#if 0
static VOS_VOID MsgTraceRouteFd(const ATP_MSG_HEADER_ST *pstMsg, VOS_INT32 lNewFd)
{
    ATP_MSG_HEADER_ST       stMsg;

    memset((VOS_VOID *)(&stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgType = ATP_MSG_TRACE_CTRL;
    MicMsgRouting(&stMsg);
    g_stMsgCtx.lSock = ;
    if (VOS_OK != ATP_MSG_Send(&stMsg))
    {
        close(lNewFd);
        return;
    }
    ATP_MSG_SendFd(lNewFd);
    close(lNewFd);
}
#endif

#if 0
extern VOS_BOOL     g_bLogEnable;
VOS_VOID ATP_MSG_MsgFilterProc(const ATP_MSG_HEADER_ST *pstMsg)
{
    VOS_INT32       lNewFd;

    // Do default msg proc
    if (NULL != pstMsg)
    {
        switch (pstMsg->ulMsgType)
        {
        case ATP_MSG_LOG_CTRL:
            if (pstMsg->ulMsgData > 0)
            {
                g_bLogEnable = VOS_TRUE;
            }
            else
            {
                g_bLogEnable = VOS_FALSE;
            }
        return;

        case ATP_MSG_TRACE_CTRL:
            g_stMsgCtx.ulDbgLevel = pstMsg->ulMsgData;
            // Recv fd imediately
            if (VOS_OK != ATP_MSG_RecvFd(&lNewFd, 1000))
            {
                return;
            }

            ATP_MSG_TraceProc(lNewFd);
        return;

        default:
        break;
        }
    }

    if (NULL != g_stMsgCtx.pfMsgProc)
    {
        g_stMsgCtx.pfMsgProc(pstMsg);
    }
}

/*
 *    Message Dispatch Interfaces
 */
/******************************************************************************
  函数名称  : ATP_MSG_RegisterMsgProc
  功能描述  : 注册消息处理函数
  输入参数  : 
              1.  pFunc:    消息处理函数
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_VOID ATP_MSG_RegisterMsgProc(AtpMsgProc pFunc)
{
    g_stMsgCtx.pfMsgProc = pFunc;
}

/******************************************************************************
  函数名称  : VTOP_MSG_GetAndDispatch
  功能描述  : 启动消息分发循环
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_MSG_GetAndDispatch()
{
    fd_set              readFds;
    VOS_INT32           lRet;
    ATP_MSG_HEADER_ST   *pstMsg;
//#ifdef SUPPORT_ATP_MSGTIMER
#if 1
    struct timeval      stTimeVal;
#endif

    if ((NULL == g_stMsgCtx.pfMsgProc) || (g_stMsgCtx.lSock < 0))
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    while (VOS_FALSE == g_stMsgCtx.bExitLoop)
    {
        // Wait for data
        FD_ZERO(&readFds);
        FD_SET(g_stMsgCtx.lSock, &readFds);

    //#ifdef SUPPORT_ATP_MSGTIMER
    #if 1
        ATP_UTIL_TimerCheck(&stTimeVal);
        lRet = select((g_stMsgCtx.lSock + 1), &readFds, NULL, NULL, &stTimeVal);
    #else
        lRet = select((g_stMsgCtx.lSock + 1), &readFds, NULL, NULL, NULL);
    #endif
        if (lRet <= 0)
        {
            continue;
        }
        if (0 == FD_ISSET(g_stMsgCtx.lSock, &readFds))
        {
            continue;
        }

        // Recv msg
        pstMsg = NULL;
        lRet = (VOS_INT32)ATP_MSG_Recv(g_stMsgCtx.lSock, &pstMsg, 0);
        if (VOS_OK != lRet)
        {
            if (ATP_MSG_RET_PEER_DEAD == lRet)
            {
                return ATP_MSG_RET_PEER_DEAD;
            }
            continue;
        }

        // Do msg proc
        ATP_MSG_MsgFilterProc(pstMsg);

        // Free msg
        free((VOS_VOID *)pstMsg);
    }

    return VOS_OK;
}
#endif

/*
 *    Message Sending & Recving Interfaces
 */
VOS_UINT32 ATP_MSG_SendEx(VOS_INT32 lSock, const ATP_MSG_HEADER_ST *pstMsg)
{
    VOS_UINT32      ulTotalLen;
    VOS_UINT32      ulCnt;
    VOS_INT32       lRet;
    struct timeval  tv;
    fd_set          writeFds;

    if ((NULL == pstMsg) || (lSock < 0))
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    // Need to check if it will overflow first
    //lint -save -e716
    while (1)
    {
        FD_ZERO(&writeFds);
        FD_SET(lSock, &writeFds);
        tv.tv_sec  = 10;
        tv.tv_usec = 0;
        lRet = select((lSock + 1), NULL, &writeFds, NULL, &tv);
        if ((lRet != 1) || (!(FD_ISSET(lSock, &writeFds))))
        {
            if ((lRet < 0) && ((EINTR == errno) || (EAGAIN == errno)))
            {
                //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!Wait to send msg interrupted.\n");
                continue;
            }
            printf("Send msg err %d:%d\n", lRet, errno);
            return ATP_MSG_RET_OVERFLOW;
        }
        break;
    }
    //lint restore

    // Send msg body
    ulCnt = 0;
    ulTotalLen = sizeof(ATP_MSG_HEADER_ST) + pstMsg->ulDataLen;
    while (ulCnt < ulTotalLen)
    {
        lRet = write(lSock, (((VOS_UINT8 *)pstMsg) + ulCnt), (ulTotalLen - ulCnt));
        if (lRet < 0)
        {
            // The socket is closed by peer
            if (errno == EPIPE)
            {
                return ATP_MSG_RET_PEER_DEAD;
            }
            else if ((EINTR == errno) || (EAGAIN == errno))
            {
                //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!Send msg interrupted.\n");
                continue;
            }
            return ATP_MSG_RET_INTERNAL_ERR;
        }
        ulCnt += (VOS_UINT32)lRet;
    }

    return VOS_OK;
}

/******************************************************************************
  函数名称  : ATP_MSG_Send
  功能描述  : 发送一个消息
  输入参数  : 
              1.  pstMsg: 消息指针
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_MSG_Send(const ATP_MSG_HEADER_ST *pstMsg)
{
    if (NULL == pstMsg)
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    // Must be inited 
    if (g_stMsgCtx.lSock < 0)
    {
        return ATP_MSG_RET_NOT_INITED;
    }

    snprintf(((ATP_MSG_HEADER_ST *)pstMsg)->aucSendApp, (ATP_MSG_MAX_NAME_LEN + 1), "%s", g_stMsgCtx.aucAppName);

    return ATP_MSG_SendEx(g_stMsgCtx.lSock, pstMsg);
}

static VOS_BOOL MsgWaitForData(VOS_INT32 fd, VOS_UINT32 ulTimeout)
{
    struct timeval tv;
    fd_set readFds;
    VOS_INT32 lRet;

    while (1)
    {
        FD_ZERO(&readFds);
        FD_SET(fd, &readFds);

        tv.tv_sec = (long)(ulTimeout / 1000);
        tv.tv_usec = (long)((ulTimeout % 1000) * 1000);
        lRet = select(fd+1, &readFds, NULL, NULL, &tv);
        if ((lRet == 1) && (FD_ISSET(fd, &readFds)))
        {
            return VOS_TRUE;
        }
        if (0 == lRet)  // It is really timeout
        {
            break;
        }
        // If error, just continue
        //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!Wait for data interrupted.\n");
        continue;
    }

    return VOS_FALSE;
}

VOS_UINT32 ATP_MSG_QueueAppend(
                                ATP_MSG_QUEUE_ST        **ppstMsgQueue,
                                ATP_MSG_HEADER_ST       *pstMsg)
{
    ATP_MSG_QUEUE_ST *pstItem;
    ATP_MSG_QUEUE_ST **ppstTmp;

    if ((NULL == ppstMsgQueue) || (NULL == pstMsg))
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    pstItem = (ATP_MSG_QUEUE_ST *)malloc(sizeof(ATP_MSG_QUEUE_ST));
    if (NULL == pstItem)
    {
        return ATP_MSG_RET_MEM_ERR;
    }
    pstItem->pstMsg = pstMsg;
    pstItem->pstNext = NULL;

    ppstTmp = ppstMsgQueue;
    while (NULL != (*ppstTmp))
    {
        ppstTmp = &((*ppstTmp)->pstNext);
    }
    (*ppstTmp) = pstItem;
    return VOS_OK;
}

VOS_VOID ATP_MSG_QueueClear(ATP_MSG_QUEUE_ST    *pstMsgQueue)
{
    ATP_MSG_QUEUE_ST    *pstItem;
    ATP_MSG_QUEUE_ST    *pstTmp;

    pstItem = pstMsgQueue;
    while (NULL != pstItem)
    {
        pstTmp = pstItem->pstNext;

        (VOS_VOID)ATP_MSG_SendEx(g_stMsgCtx.lSock, pstItem->pstMsg);

        free((VOS_VOID *)(pstItem->pstMsg));
        free((VOS_VOID *)pstItem);
        pstItem = pstTmp;
    }
}

/******************************************************************************
  函数名称  : ATP_MSG_SendAndRecv
  功能描述  : 发送消息，并且接收其回应消息
  输入参数  : 
              1.     pstMsg:        待发送的消息
              2.     ppstResp:    回应消息
              3.     pulTimeout:    超时，为空表示永久阻塞
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_MSG_SendAndRecv(const ATP_MSG_HEADER_ST   *pstMsg,
                                        ATP_MSG_HEADER_ST         **ppstResp,
                                        VOS_UINT32                ulTimeout)
{
    VOS_UINT32          ulRet;
    VOS_BOOL            bContinue;
    ATP_MSG_HEADER_ST   *pstRet;

    // Check input parameter
    if ((NULL == pstMsg) || (NULL == ppstResp))
    {
        return ATP_MSG_RET_PARA_ERR;
    }
    (*ppstResp) = NULL;

    // Send request first
    ulRet = ATP_MSG_Send(pstMsg);
    if (VOS_OK != ulRet)
    {
        return ulRet;
    }

    // Loop to receive response
    bContinue = VOS_TRUE;
    while (VOS_TRUE == bContinue)
    {
        // Recv msg first
        ulRet = ATP_MSG_Recv(g_stMsgCtx.lSock, &pstRet, ulTimeout);
        if (VOS_OK != ulRet)
        {
            break;
        }

        // Check msg type
        if (pstMsg->ulMsgType != pstRet->ulMsgType)
        {
            ATP_MSG_DEBUG("Backup not matched msg %x.\n", pstRet->ulMsgType);
            if (VOS_OK != ATP_MSG_QueueAppend(&(g_stMsgCtx.pstMsgQueue), pstRet))
            {
                free((VOS_VOID *)pstRet);
            }
            continue;
        }

        (*ppstResp) = pstRet;
        break;
    }

    // Restore all backup msg
    ATP_MSG_QueueClear(g_stMsgCtx.pstMsgQueue);
    g_stMsgCtx.pstMsgQueue = NULL;

    return ulRet;
}

/******************************************************************************
  函数名称  : ATP_MSG_Recv
  功能描述  : 从socket接收消息
  输入参数  : 
              1.     sSock:        socket描述符
              2.     ppstMsg:        输出消息指针
              3.     ulTimeout:    超时时间，为0表示永久阻塞，单位ms
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_MSG_Recv(VOS_INT32             sSock,
                              ATP_MSG_HEADER_ST     **ppstMsg,
                              VOS_UINT32            ulTimeout)
{
    ATP_MSG_HEADER_ST    *pstMsg;
    VOS_INT32           lRet;
    VOS_UINT32          ulBytesRemain;
    VOS_CHAR            *pcBody;

    if ((sSock < 0) || (NULL == ppstMsg))
    {
        return ATP_MSG_RET_PARA_ERR;
    }

    // Wait if needed
    (*ppstMsg) = NULL;
    if (0 != ulTimeout)
    {
        if (VOS_FALSE == MsgWaitForData(sSock, ulTimeout))
        {
            return ATP_MSG_RET_TIMEOUT;
        }
    }

    // Alloc msg header
    pstMsg = (ATP_MSG_HEADER_ST *)malloc(sizeof(ATP_MSG_HEADER_ST));
    if (NULL == pstMsg)
    {
        return ATP_MSG_RET_MEM_ERR;
    }

    // Recv msg header
    while (1)
    {
        lRet = read(sSock, (VOS_VOID *)pstMsg, sizeof(ATP_MSG_HEADER_ST));
        if ((lRet == 0) ||
            ((lRet == -1) && (errno == 131)))  /* new 2.6.21 kernel seems to give us this before rc==0 */
        {
            free((VOS_VOID *)pstMsg);
            return ATP_MSG_RET_PEER_DEAD;
        }
        else if (lRet < 0)
        {
            // Interrupted, just continue
            if ((EINTR == errno) || (EAGAIN == errno))
            {
                //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!Recv msg header interrupted.\n");
                continue;
            }
            free((VOS_VOID *)pstMsg);
            return ATP_MSG_RET_INTERNAL_ERR;
        }
        else if ((VOS_UINT32)lRet != sizeof(ATP_MSG_HEADER_ST))
        {
            free((VOS_VOID *)pstMsg);
            return ATP_MSG_RET_INTERNAL_ERR;
        }
        // Message header is OK
        break;
    }

    // If no msg body, return immediately
    if (0 == pstMsg->ulDataLen)
    {
        (*ppstMsg) = pstMsg;
        return VOS_OK;
    }

    // Realloc enough memory to recv data
    pstMsg = realloc(pstMsg, (sizeof(ATP_MSG_HEADER_ST) + pstMsg->ulDataLen));
    if (NULL == pstMsg)
    {
        return ATP_MSG_RET_MEM_ERR;
    }

    // Continue to recv the remaining body
    pcBody = (VOS_CHAR *)(pstMsg + 1);
    ulBytesRemain = pstMsg->ulDataLen;
    while (ulBytesRemain > 0)
    {
        if (0 != ulTimeout)
        {
            if (VOS_FALSE == MsgWaitForData(sSock, ulTimeout))
            {
                free((VOS_VOID *)pstMsg);
                return ATP_MSG_RET_TIMEOUT;
            }
        }

        lRet = read(sSock, pcBody, ulBytesRemain);
        if ((lRet == 0) ||
            ((lRet == -1) && (errno == 131)))
        {
            free((VOS_VOID *)pstMsg);
            return ATP_MSG_RET_PEER_DEAD;
        }
        else if (lRet < 0)
        {
            // Interrupted, just continue
            if ((EINTR == errno) || (EAGAIN == errno))
            {
                //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!Recv msg body interrupted.\n");
                continue;
            }
            free((VOS_VOID *)pstMsg);
            return ATP_MSG_RET_INTERNAL_ERR;
        }

        pcBody += lRet;
        ulBytesRemain -= (VOS_UINT32)lRet;
    }

    (*ppstMsg) = pstMsg;
    return VOS_OK;
}

VOS_UINT32 ATP_MSG_GetNumOfProcesses(const VOS_CHAR *pcName)
{
    VOS_UINT32          ulRetVal;
    ATP_MSG_HEADER_ST   *pstMsg;
    ATP_MSG_HEADER_ST   *pstResp;
    VOS_CHAR            acMsg[sizeof(ATP_MSG_HEADER_ST) + ATP_MSG_MAX_NAME_LEN + 1];

    if (NULL == pcName)
    {
        return 0;
    }

    memset((void *)acMsg, 0, sizeof(acMsg));
    pstMsg = (ATP_MSG_HEADER_ST *)acMsg;
    pstMsg->ulMsgType = ATP_MSG_GET_NUM_OF_PROC;
    pstMsg->ulDataLen = (ATP_MSG_MAX_NAME_LEN + 1);
    snprintf((char *)(acMsg + sizeof(ATP_MSG_HEADER_ST)), (ATP_MSG_MAX_NAME_LEN + 1), "%s", pcName);
    ulRetVal = ATP_MSG_SendAndRecv(pstMsg, &pstResp, 5000);
    if (VOS_OK != ulRetVal)
    {
        return 0;
    }
    if (NULL == pstResp)
    {
        return 0;
    }
    ulRetVal = pstResp->ulMsgData;
    free((VOS_VOID *)pstResp);
    return ulRetVal;
}

/*
 *    Message Utilities
 */

/******************************************************************************
  函数名称  : 
  功能描述  : 
  输入参数  : 
              1.  
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-5
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
//lint -save -e429
VOS_VOID* ATP_MSG_GetMsgBlock(VOS_UINT32 ulMsgSize)
{
    ATP_MSG_HEADER_ST* pstMsgHead;

    pstMsgHead = malloc(sizeof(ATP_MSG_HEADER_ST) + ulMsgSize);
    if (!pstMsgHead)
    {
        return VOS_NULL_PTR;
    }
    memset(pstMsgHead, 0, sizeof(ATP_MSG_HEADER_ST));
    pstMsgHead->ulDataLen = ulMsgSize;
    return (pstMsgHead + 1);
}
//lint restore

// 初始化外部分配的消息块
VOS_VOID* ATP_MSG_GetMsgBlockEx(VOS_VOID* pvBuff, VOS_UINT32 ulBuffLen)
{
    ATP_MSG_HEADER_ST* pstMsgHead;

    if (ulBuffLen < sizeof(ATP_MSG_HEADER_ST))
    {
        return VOS_NULL_PTR;
    }
    pstMsgHead = pvBuff;
    memset(pstMsgHead, 0, sizeof(ATP_MSG_HEADER_ST));
    pstMsgHead->ulDataLen = ulBuffLen - sizeof(ATP_MSG_HEADER_ST);
    return (pstMsgHead + 1);
}

VOS_UINT32 ATP_MSG_SetMsgProfile(VOS_VOID* pvMsg, const VOS_CHAR acRecvApp[ATP_MSG_MAX_NAME_LEN + 1], 
                            VOS_UINT32 ulMsgType, VOS_UINT32 ulMsgLen)
{
    ATP_MSG_HEADER_ST* pstMsgHead;

    pstMsgHead = (ATP_MSG_HEADER_ST*)pvMsg - 1;
    strncpy(pstMsgHead->aucRecvApp, acRecvApp, ATP_MSG_MAX_NAME_LEN);
    pstMsgHead->aucRecvApp[ATP_MSG_MAX_NAME_LEN] = '\0';
    pstMsgHead->ulMsgType = ulMsgType;
    if (ulMsgLen <= pstMsgHead->ulDataLen)
    {
        pstMsgHead->ulDataLen = ulMsgLen;
    }
    else
    {
        return ATP_MSG_RET_PARA_ERR;
    }
    
    return VOS_OK;
}

ATP_MSG_HEADER_ST* ATP_MSG_GetMsgHead(VOS_VOID* pvMsg)
{
    return (((ATP_MSG_HEADER_ST*)pvMsg) - 1);
}

VOS_VOID* ATP_MSG_GetMsgByHead(ATP_MSG_HEADER_ST* pstMsgHead)
{
    if (!pstMsgHead || pstMsgHead->ulDataLen == 0)
    {
        return VOS_NULL_PTR;
    }

    return (pstMsgHead + 1);
}

/*
 *  Trace utility
 */
VOS_VOID ATP_MSG_Printf(const VOS_INT8 *fmt, ...)
{
    va_list             vaArgList;

    if (NULL == g_stMsgCtx.pfTraceFile)
    {
        return;
    }

    va_start(vaArgList, fmt);
    //lint -save -e64
    vfprintf(g_stMsgCtx.pfTraceFile, fmt, vaArgList);
    //lint restore
    va_end(vaArgList);
    fflush(g_stMsgCtx.pfTraceFile);
}

#if defined(SOLARIS)
#define _XOPEN_SOURCE	500	/* Single UNIX Specification, Version 2  for Solaris 9 */
#define CMSG_LEN(x)	_CMSG_DATA_ALIGN(sizeof(struct cmsghdr)+(x))
#elif !defined(BSD)
#define _XOPEN_SOURCE	600	/* Single UNIX Specification, Version 3 */
#endif

/* size of control buffer to send/recv one file descriptor */
#define	CONTROLLEN	CMSG_LEN(sizeof(int))

static struct cmsghdr	*cmptr = NULL;	/* malloc'ed first time */

VOS_UINT32 ATP_MSG_SendFd(VOS_INT32 lFdToSend)
{
    struct iovec        iov[1];
    struct msghdr       msg;
    VOS_CHAR            buf[2];	/* send_fd()/recv_fd() 2-byte protocol */

    if (g_stMsgCtx.lSock < 0)
    {
        return ATP_MSG_RET_NOT_INITED;
    }

    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    if ((NULL == cmptr) && (NULL == (cmptr = malloc(CONTROLLEN))))
    {
        return ATP_MSG_RET_MEM_ERR;
    }
    cmptr->cmsg_level  = SOL_SOCKET;
    cmptr->cmsg_type   = SCM_RIGHTS;
    cmptr->cmsg_len    = CONTROLLEN;
    msg.msg_control    = cmptr;
    msg.msg_controllen = CONTROLLEN;
    //lint -save -e826
    memcpy((VOS_CHAR*)(CMSG_DATA(cmptr)) ,(VOS_CHAR*) lFdToSend, sizeof(lFdToSend));		/* the fd to pass */
    //lint restore
    buf[1] = 0;		/* zero status means OK */
    buf[0] = 0;			/* null byte flag to recv_fd() */
    if (sendmsg(g_stMsgCtx.lSock, &msg, 0) != 2)
    {
        return ATP_MSG_RET_INTERNAL_ERR;
    }

    return VOS_OK;
}

static struct cmsghdr	*cmptr_r = NULL;	/* malloc'ed first time */

VOS_UINT32 ATP_MSG_RecvFd(VOS_INT32 *plFdToRecv, VOS_UINT32 ulTimeout)
{
    VOS_INT32       nr;
    VOS_CHAR        buf[4096];
    struct iovec    iov[1];
    struct msghdr   msg;
    VOS_INT32 * pltmp;

    if (NULL == plFdToRecv)
    {
        return ATP_MSG_RET_PARA_ERR;
    }
    if (g_stMsgCtx.lSock < 0)
    {
        return ATP_MSG_RET_NOT_INITED;
    }

    // Wait for msg to come
    if (ulTimeout > 0)
    {
        if (VOS_FALSE == MsgWaitForData(g_stMsgCtx.lSock, ulTimeout))
        {
            return ATP_MSG_RET_TIMEOUT;
        }
    }

    // Prepare to recv
    iov[0].iov_base = buf;
    iov[0].iov_len  = sizeof(buf);
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    if ((NULL == cmptr_r) && (NULL == (cmptr_r = malloc(CONTROLLEN))))
    {
        return ATP_MSG_RET_MEM_ERR;
    }
    msg.msg_control    = cmptr_r;
    msg.msg_controllen = CONTROLLEN;
    if ((nr = recvmsg(g_stMsgCtx.lSock, &msg, 0)) < 0)
    {
        return ATP_MSG_RET_INTERNAL_ERR;
    }
    else if (nr == 0)
    {
        return ATP_MSG_RET_PEER_DEAD;
    }

    if ((2 != nr) || (CONTROLLEN != msg.msg_controllen) || (0 != buf[0]) || (0 != buf[1]))
    {
        return ATP_MSG_RET_INTERNAL_ERR;
    }
    //lint -save -e826
    pltmp =(VOS_INT32 *) (CMSG_DATA(cmptr_r));
    (*plFdToRecv) = *pltmp;
    //lint restore

    return VOS_OK;
}

VOS_VOID ATP_MSG_SimpleBuilder(ATP_MSG_HEADER_ST *pstMsg, const VOS_CHAR *pcDest,
                                     VOS_UINT32 ulMsgType, VOS_UINT32 ulMsgData)
{
    if (NULL == pstMsg)
    {
        return;
    }

    memset((VOS_VOID *)pstMsg, 0, sizeof(ATP_MSG_HEADER_ST));
    pstMsg->ulMsgType = ulMsgType;
    pstMsg->ulMsgData = ulMsgData;
    if (NULL == pcDest)
    {
        return;
    }
    snprintf(pstMsg->aucRecvApp, (ATP_MSG_MAX_NAME_LEN + 1), "%s", pcDest);
}

VOS_UINT32 ATP_MSG_SendSimpleMsg(const VOS_CHAR *pcDest,
                                     VOS_UINT32 ulMsgType, VOS_UINT32 ulMsgData)
{
    ATP_MSG_HEADER_ST stMsg;

    ATP_MSG_SimpleBuilder(&stMsg, pcDest, ulMsgType, ulMsgData);
    return ATP_MSG_Send(&stMsg);
}

VOS_UINT32 ATP_MSG_SendMsgData(const VOS_CHAR* pcProcName, VOS_UINT32 ulMsgType, 
                               const VOS_VOID* pvMsgBuff, VOS_UINT32 ulBuffLen)
{
    ATP_MSG_HEADER_ST* pstMsgHead;
    VOS_UINT32 ulRet;

    if (NULL == pvMsgBuff)
    {
        return ATP_MSG_SendSimpleMsg(pcProcName, ulMsgType, 0);
    }

    pstMsgHead = (ATP_MSG_HEADER_ST*)malloc(sizeof(ATP_MSG_HEADER_ST) + ulBuffLen);
    if (!pstMsgHead)
    {
        return ATP_MSG_RET_MEM_ERR;
    }
    memset((VOS_VOID*)pstMsgHead, 0, sizeof(ATP_MSG_HEADER_ST));
    if (NULL != pcProcName)
    {
        snprintf(pstMsgHead->aucRecvApp, (ATP_MSG_MAX_NAME_LEN + 1), "%s", pcProcName);
    }
    pstMsgHead->ulDataLen = ulBuffLen;
    pstMsgHead->ulMsgType = ulMsgType;
    memcpy((VOS_VOID*)(pstMsgHead + 1), pvMsgBuff, ulBuffLen);
    ulRet = ATP_MSG_Send(pstMsgHead);
    free(pstMsgHead);
    return ulRet;
}

#ifdef SUPPORT_ATP_LOG
VOS_BOOL logLogTypeCheck(VOS_UINT32 ulLogType)
{
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i;
    
    for (i = 0; i < ATP_LOG_TYPE_COUNT; i++)
    {
        if ((ulLogType & (1 << i)) != 0)
        {
            ulCount++;
        }
        if (ulCount > 1)
        {
            return VOS_FALSE;
        }
    }
    if (ulCount != 1)
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}

VOS_BOOL logLogLevelCheck(VOS_UINT32 ulLogLevel)
{
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i;
    
    for (i = 0; i < ATP_LOG_LEVEL_COUNT; i++)
    {
        if ((ulLogLevel & (1 << i)) != 0)
        {
            ulCount++;
        }
        if (ulCount > 1)
        {
            return VOS_FALSE;
        }
    }
    if (ulCount != 1)
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}
#endif

VOS_BOOL g_bLogEnable = VOS_TRUE;

VOS_UINT32 ATP_LOG_Printf(VOS_UINT32 ulLogType, VOS_UINT32 ulLogLevel,
                              VOS_UINT32 ulLogNum, const VOS_INT8 *fmt, ...)
{
    va_list             vaArgList;
#ifdef SUPPORT_ATP_LOG
    ATP_MSG_HEADER_ST   *pstMsgHeader;
    ATP_LOG_MSG_ST      *pstLogHeader;
    VOS_CHAR            *pcLogStr;
    VOS_UINT32          ulLen;
    VOS_CHAR            acLogMsg[sizeof(ATP_MSG_HEADER_ST) +
                            sizeof(ATP_LOG_MSG_ST) + ATP_LOG_MAX_CONTENT_LEN + 1];

    if ((ulLogNum == 0xFFFFFFFF) || (NULL == fmt))
    {
        return ATP_LOG_RET_PARA_ERR;
    }

    if (VOS_FALSE == logLogTypeCheck(ulLogType) ||
        VOS_FALSE == logLogLevelCheck(ulLogLevel))
    {
        return ATP_LOG_RET_PARA_ERR;
    }

    if (VOS_FALSE == g_bLogEnable)
    {
        return VOS_OK;
    }
    if (g_stMsgCtx.lSock <= 0)
    {
        return ATP_LOG_RET_NOT_INITED;
    }

    memset((VOS_VOID *)acLogMsg, 0, sizeof(acLogMsg));
    pstMsgHeader = (ATP_MSG_HEADER_ST *)acLogMsg;
    pstLogHeader = (ATP_LOG_MSG_ST *)(acLogMsg + sizeof(ATP_MSG_HEADER_ST));
    pcLogStr = ((VOS_CHAR*)pstLogHeader) + sizeof(ATP_LOG_MSG_ST);

    va_start(vaArgList, fmt);
    ulLen = (VOS_UINT32)vsnprintf(pcLogStr, ATP_LOG_MAX_CONTENT_LEN, fmt, vaArgList);
    va_end(vaArgList);
    if (ulLen > ATP_LOG_MAX_CONTENT_LEN)
    {
        return ATP_LOG_RET_TOO_LONG;
    }

    // Fill log msg data
    pstLogHeader->ulLogTypeLevel    |= (ulLogType << ATP_LOG_TYPE_OFFSET);
    pstLogHeader->ulLogTypeLevel    |= ulLogLevel;
    pstLogHeader->ulLogNum          = ulLogNum;
    pstLogHeader->usLogLen          = (VOS_UINT16)ulLen;

    // Fill msg header
    snprintf(pstMsgHeader->aucRecvApp, ATP_MSG_MAX_NAME_LEN + 1, "%s", ATP_CBBID_LOG_NAME);
    pstMsgHeader->ulMsgType     = ATP_MSG_LOG_ITEM;
    pstMsgHeader->ulDataLen     = (VOS_UINT32)(pstLogHeader->usLogLen +
                                    sizeof(ATP_LOG_MSG_ST));

    return ATP_MSG_Send(pstMsgHeader);
#else
    va_start(vaArgList, fmt);
    vfprintf(stdout, fmt, vaArgList);
    va_end(vaArgList);

    return VOS_OK;
#endif
}

VOS_UINT32 ATP_LOG_RecordItem(VOS_UINT32 ulLogItemIdx, ...)
{
    va_list                   arg_ptr;
    VOS_UINT32                ulRet;
    VOS_INT8*                 pszArg;
    VOS_UINT32                ulBufLen;
    VOS_UINT32                ulParaNum = 0;

    VOS_INT8*                 pszSendBuf;
    ATP_MSG_HEADER_ST*        pszMsgAddr;
    ATP_LOG_LOGITEM_PARAS_ST* pszParaStAddr;
    VOS_INT8*                 pszParaStrAddr;

    ulBufLen = sizeof(ATP_LOG_LOGITEM_PARAS_ST);

    va_start(arg_ptr, ulLogItemIdx);
    while (VOS_NULL_PTR != (pszArg = va_arg(arg_ptr, VOS_INT8*)))
    {
        ulBufLen += strlen(pszArg)+1; // also add '\0'
    }
    va_end(arg_ptr);

    pszSendBuf = (VOS_INT8*)malloc(sizeof(ATP_MSG_HEADER_ST) + sizeof(ATP_LOG_LOGITEM_PARAS_ST)+ ulBufLen);
    if (VOS_NULL_PTR == pszSendBuf)
    {
        return ATP_LOG_RET_MEM;
    }
    pszMsgAddr = (ATP_MSG_HEADER_ST*)pszSendBuf;
    pszParaStAddr = (ATP_LOG_LOGITEM_PARAS_ST*)(pszSendBuf + sizeof(ATP_MSG_HEADER_ST));
    pszParaStrAddr = (VOS_INT8*)pszParaStAddr + sizeof(ATP_LOG_LOGITEM_PARAS_ST);

    va_start(arg_ptr, ulLogItemIdx);
    while (VOS_NULL_PTR != (pszArg = va_arg(arg_ptr, VOS_INT8*)))
    {
        memcpy(pszParaStrAddr, (VOS_UINT8*)pszArg, strlen(pszArg)+1);
        pszParaStrAddr += strlen(pszArg)+1;
        ulParaNum++;
    }
    va_end(arg_ptr);

    pszParaStAddr->ulFeatureIdOffset = ulLogItemIdx;
    pszParaStAddr->usParaNum = ulParaNum;
    pszParaStAddr->ulContentLen = ulBufLen - sizeof(ATP_LOG_LOGITEM_PARAS_ST);

    snprintf(pszMsgAddr->aucRecvApp, ATP_MSG_MAX_NAME_LEN + 1, "%s", ATP_CBBID_LOG_NAME);
    pszMsgAddr->ulMsgType = ATP_MSG_LOG_ITEM;
    pszMsgAddr->ulDataLen = sizeof(ATP_LOG_LOGITEM_PARAS_ST)+ ulBufLen;

    ulRet = ATP_MSG_Send(pszMsgAddr);
    free(pszSendBuf);
    return ulRet;
}

VOS_UINT32 ATP_LOG_KLogPrintf(VOS_UINT32 ulLogType, VOS_UINT32 ulLogLevel,
                              VOS_UINT32 ulLogNum, const VOS_INT8* pstLogString)
{
    ATP_MSG_HEADER_ST* pszMsgAddr;
    ATP_LOG_ITEM_ST*   pLogItem;
    VOS_UINT32         ulRet;
    if ((ulLogNum == 0xFFFFFFFF) || (NULL == pstLogString)
            || VOS_FALSE == logLogTypeCheck(ulLogType)
            || VOS_FALSE == logLogLevelCheck(ulLogLevel))
    {
        return ATP_LOG_RET_PARA_ERR;
    }
    pszMsgAddr = (ATP_MSG_HEADER_ST*)malloc(sizeof(ATP_MSG_HEADER_ST)+sizeof(ATP_LOG_ITEM_ST)+strlen(pstLogString)+1);
    if (VOS_NULL_PTR == pszMsgAddr)
    {
        ATP_LOG_DEBUG("ATP_LOG_KLogPrintf pszMsgAddr malloc error. \r\n");
        return ATP_LOG_RET_MEM;
    }
    memset(pszMsgAddr, 0, sizeof(ATP_MSG_HEADER_ST)+sizeof(ATP_LOG_ITEM_ST)+strlen(pstLogString)+1);
    pLogItem= (ATP_LOG_ITEM_ST*)(pszMsgAddr+1);
    pLogItem->ulLogTypeLevel |= (ulLogType << ATP_LOG_TYPE_OFFSET);
    pLogItem->ulLogTypeLevel |= ulLogLevel;
    pLogItem->ulLogNum = ulLogNum;
    pLogItem->stLogParasSt.ulFeatureIdOffset = (VOS_UINT32)((VOS_UINT32)ATP_LOG_KERNEL_FEATURE_ID << LOG_FEATUREID_OFFSET);
    pLogItem->stLogParasSt.usParaNum = 1;
    pLogItem->stLogParasSt.ulContentLen = (VOS_UINT16)(strlen(pstLogString)+1); // also add '\0'
    memcpy(pLogItem->stLogParasSt.pszContent, pstLogString, strlen(pstLogString)+1); // also add '\0'
    snprintf(pszMsgAddr->aucRecvApp, ATP_MSG_MAX_NAME_LEN + 1, "%s", ATP_CBBID_LOG_NAME);
    pszMsgAddr->ulMsgType = ATP_MSG_LOG_KERNEL_ITEM;
    pszMsgAddr->ulDataLen = sizeof(ATP_LOG_ITEM_ST)+strlen(pstLogString)+1;
    ulRet = ATP_MSG_Send(pszMsgAddr);
    free(pszMsgAddr);
    return ulRet;
}
/* <DTS2012060604837 j81004134 2012/6/1 begin */
/* <jiangounie j81004134 2012/5/10 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
VOS_UINT32 ATP_LOG_DEBUG_OUTPUT(VOS_UINT32 ulLogType, VOS_UINT32 ulLogLevel, const char *message)
{
    ATP_LOG_ITEM_ST *log_item_pointer;
    ATP_MSG_HEADER_ST   *pstMsgHeader;
    VOS_CHAR            *pcLogStr;
    VOS_UINT32          ulLen;
    VOS_CHAR            acLogMsg[sizeof(ATP_MSG_HEADER_ST) +
                            sizeof(ATP_LOG_ITEM_ST) + ATP_LOG_MAX_CONTENT_LEN + 1];

    if (NULL == message)
    {
        return ATP_LOG_RET_PARA_ERR;
    }

    if (g_stMsgCtx.lSock <= 0)
    {
        return ATP_LOG_RET_NOT_INITED;
    }

    memset((VOS_VOID *)acLogMsg, 0, sizeof(acLogMsg));
    pstMsgHeader = (ATP_MSG_HEADER_ST *)acLogMsg;
    log_item_pointer = (ATP_LOG_ITEM_ST *)(acLogMsg + sizeof(ATP_MSG_HEADER_ST));
    pcLogStr = ((VOS_CHAR*)log_item_pointer) + sizeof(ATP_LOG_ITEM_ST);

    ulLen = strlen(message);
    memcpy(pcLogStr, message, ulLen+1);

    // Fill log msg data
    log_item_pointer->ulLogTypeLevel |= (ulLogType << ATP_LOG_TYPE_OFFSET);
    log_item_pointer->ulLogTypeLevel |= ulLogLevel;
    log_item_pointer->ulLogNum = 1;
    log_item_pointer->stLogParasSt.ulFeatureIdOffset = 0;    /* 暂定填0 */
    log_item_pointer->stLogParasSt.usParaNum = 1;
    log_item_pointer->stLogParasSt.ulContentLen = (VOS_UINT16)ulLen;

    // Fill msg header
    snprintf(pstMsgHeader->aucRecvApp, ATP_MSG_MAX_NAME_LEN + 1, "%s", ATP_CBBID_LOG_NAME);
    pstMsgHeader->ulMsgType     = ATP_MSG_LOG_DEBUG_INFO;
    pstMsgHeader->ulDataLen     = (VOS_UINT32)(ulLen + sizeof(ATP_LOG_ITEM_ST));

    return ATP_MSG_Send(pstMsgHeader);
}
#endif
/* jiangounie j81004134 2012/5/10 end> */
/* DTS2012060604837 j81004134 2012/6/1 end> */
/******************************************************************************
  函数名称  : BSP_SYS_WakeupMonitorTask
  功能描述  : 唤醒监控进程
  输入参数  : 无
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-6-29
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
void BSP_SYS_WakeupMonitorTask(void)
{
    ATP_MSG_Init(NULL);

    ATP_MSG_SendMsgData("cms", 1, NULL, 0);
}
