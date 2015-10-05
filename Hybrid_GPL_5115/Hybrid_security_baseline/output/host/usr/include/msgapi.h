#ifndef __ATP_MSG_API_H__
#define __ATP_MSG_API_H__

#include "atptypes.h"
#include "shmemapi.h"
#include "sysutil.h"



#define ATP_MSG_CATEGORY_DEF(x)          ((x) << 20)

#define ATP_MSG_RECV_TIMEOUT_D           (1000 * 5) // 5 seconds

#include "msgtypeswrapper.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* <DTS2012060604837 j81004134 2012/6/1 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
extern VOS_UINT32 module_log_level;
#endif
/* DTS2012060604837 j81004134 2012/6/1 end> */

#define ATP_MSG_MAX_NAME_LEN    (11)

/* Message Header */
typedef struct tagATP_MSG_HEADER_ST
{
    VOS_UINT32  ulMsgType;
    VOS_UINT32  usSeqNum;

    VOS_CHAR    aucSendApp[ATP_MSG_MAX_NAME_LEN + 1];    /* 发送进程名字 */
    VOS_CHAR    aucRecvApp[ATP_MSG_MAX_NAME_LEN + 1];    /* 接收进程名字 */

    VOS_UINT32  ulMsgData;
    VOS_UINT32  ulDataLen;
}ATP_MSG_HEADER_ST;


typedef enum tagATP_MSG_RET_CODE_EN
{
    ATP_MSG_RET_SOCKET_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_MSGCORE),
    ATP_MSG_RET_PARA_ERR,
    ATP_MSG_RET_APPNAME_ERR,
    ATP_MSG_RET_NOT_INITED,
    ATP_MSG_RET_PEER_DEAD,
    ATP_MSG_RET_TIMEOUT,
    ATP_MSG_RET_MEM_ERR,
    ATP_MSG_RET_INTERNAL_ERR,
    ATP_MSG_RET_OVERFLOW,
    ATP_MSG_RET_FILE_NOT_EXIST,
    ATP_MSG_RET_NOT_FOUNDPWD,
    ATP_MSG_RET_UPROOT_FAIL
} ATP_MSG_RET_CODE_EN;


typedef VOS_VOID (*AtpMsgProc)(const ATP_MSG_HEADER_ST *pstMsg);

typedef struct tagATP_MSG_QUEUE_ST
{
    ATP_MSG_HEADER_ST           *pstMsg;
    struct tagATP_MSG_QUEUE_ST   *pstNext;
} ATP_MSG_QUEUE_ST;

typedef enum tagATP_MSG_CTX_FLAGS_EN
{
    ATP_MSG_CTX_FLAG_EXIT = (1 << 0),
    ATP_MSG_CTX_FLAG_FROM_NTWK = (1 << 1),
} ATP_MSG_CTX_FLAGS_EN;

#define ATP_MSG_NEED_EXIT() (g_stMsgCtx.bExitLoop & ATP_MSG_CTX_FLAG_EXIT)

#ifdef SUPPORT_ATP_MULTI_BOARD_COMMUNICATION
#define ATP_MSG_IS_FROM_NTWK() (g_stMsgCtx.bExitLoop & ATP_MSG_CTX_FLAG_FROM_NTWK)

#define ATP_MSG_TYPE_FORWARD 0x80000000

// Set routing to other boards msg type flag
#define ATP_MSG_TYPE_SET_NTWK_FLAG(MsgType) (MsgType | ATP_MSG_TYPE_FORWARD)
// Unset routing to other boards msg type flag
#define ATP_MSG_TYPE_UNSET_NTWK_FLAG(MsgType) (MsgType & (~ATP_MSG_TYPE_FORWARD))
// Check msg type if it is sent to other board
#define ATP_MSG_TYPE_IS_FROM_NTWK(MsgType) (MsgType & ATP_MSG_TYPE_FORWARD)

VOS_UINT32 ATP_MSG_QueueDelDuplicate(
                                ATP_MSG_QUEUE_ST        **pstMsgQueue,
                                const ATP_MSG_HEADER_ST      *pstMsg);

VOS_UINT32 ATP_MSG_Init_IP(const VOS_CHAR *pszAppName, const VOS_CHAR *pszIpAddr);

#endif

typedef struct tagATP_MSG_CONTEXT_ST
{
    VOS_CHAR            aucAppName[ATP_MSG_MAX_NAME_LEN + 1];
    VOS_INT32           lSock;
    VOS_BOOL            bExitLoop;
    AtpMsgProc          pfMsgProc;
    VOS_UINT32          ulDbgLevel;
    FILE                *pfTraceFile;
    VOS_INT32           lTraceFd;
    ATP_MSG_QUEUE_ST    *pstMsgQueue;   // Internal usage
    VOS_INT32           lAccessMode;
} ATP_MSG_CONTEXT_ST;

extern ATP_THREAD_SAFE ATP_MSG_CONTEXT_ST g_stMsgCtx;

VOS_UINT32 ATP_MSG_QueueAppend(
                                ATP_MSG_QUEUE_ST        **ppstMsgQueue,
                                ATP_MSG_HEADER_ST       *pstMsg);

VOS_VOID ATP_MSG_QueueClear(ATP_MSG_QUEUE_ST    *pstMsgQueue);

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
VOS_UINT32 ATP_MSG_Init(const VOS_CHAR *pszAppName);

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
VOS_VOID ATP_MSG_Destroy(VOS_VOID);

//VOS_UINT32 ATP_MSG_GetAppName(VOS_CHAR *pszName);

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
VOS_VOID ATP_MSG_RegisterMsgProc(AtpMsgProc pFunc);

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
VOS_UINT32 ATP_MSG_GetAndDispatch();

//VOS_UINT32 VTOP_MSG_RegisterTrace(PFUNC_MSGHandler pFunc);

//VOS_UINT32 VTOP_MSG_UnRegTrace();

/*
 *    Message Sending & Recving Interfaces
 */
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
VOS_UINT32 ATP_MSG_Send(const ATP_MSG_HEADER_ST *pstMsg);

VOS_UINT32 ATP_MSG_SendEx(VOS_INT32 lSock, const ATP_MSG_HEADER_ST *pstMsg);

/******************************************************************************
  函数名称  : ATP_MSG_SendAndRecv
  功能描述  : 发送消息，并且接收其回应消息
  输入参数  : 
              1.     pstMsg:        待发送的消息
              2.     ppstResp:    回应消息
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
VOS_UINT32 ATP_MSG_SendAndRecv(const ATP_MSG_HEADER_ST  *pstMsg,
                                        ATP_MSG_HEADER_ST        **ppstResp,
                                        VOS_UINT32               ulTimeout);

VOS_UINT32 ATP_MSG_RecvWithType(VOS_UINT32              ulMsgType,
                                        ATP_MSG_HEADER_ST         **ppstResp,
                                        VOS_UINT32                ulTimeout);

VOS_UINT32 ATP_MSG_RecvResponse(const ATP_MSG_HEADER_ST *pstMsg,
                                        ATP_MSG_HEADER_ST         **ppstResp,
                                        VOS_UINT32                ulTimeout);

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
VOS_UINT32 ATP_MSG_Recv(VOS_INT32           sSock,
                              ATP_MSG_HEADER_ST   **ppstMsg,
                              VOS_UINT32          ulTimeout);

/*
 *    Message Utilities
 */
VOS_VOID ATP_MSG_TraceProc(VOS_INT32 lNewFd);

VOS_VOID ATP_MSG_MsgFilterProc(const ATP_MSG_HEADER_ST *pstMsg);

VOS_VOID* ATP_MSG_GetMsgBlock(VOS_UINT32 ulMsgSize);
VOS_VOID* ATP_MSG_GetMsgByHead(ATP_MSG_HEADER_ST* pstMsgHead);
ATP_MSG_HEADER_ST* ATP_MSG_GetMsgHead(VOS_VOID* pvMsg);

/*
 *    Message Structure Utilities
 */
VOS_VOID ATP_MSG_Printf(const VOS_INT8 *fmt, ...);

VOS_UINT32 ATP_MSG_SendFd(VOS_INT32 lFdToSend);

VOS_UINT32 ATP_MSG_RecvFd(VOS_INT32 *plFdToRecv, VOS_UINT32 ulTimeout);


/*
 *  General List Utilities
 */
typedef struct tagATP_UTIL_LIST_ST
{
    struct tagATP_UTIL_LIST_ST      *pstNext;
} ATP_UTIL_LIST_ST;
typedef VOS_VOID (* PFUtilListItemDestroyProc)(ATP_UTIL_LIST_ST *pstItem);
typedef VOS_BOOL (* PFUtilListItemCompareProc)(const ATP_UTIL_LIST_ST *pstItem, const VOS_VOID *pvCompare);

VOS_VOID ATP_UTIL_ListDeleteAll(
                        ATP_UTIL_LIST_ST            *pstList,
                        PFUtilListItemDestroyProc   pfDestroy);

VOS_VOID ATP_UTIL_ListDeleteItem(
                        ATP_UTIL_LIST_ST            **ppstStart,
                        ATP_UTIL_LIST_ST            *pstDelItem,
                        PFUtilListItemDestroyProc   pfDestroy);

VOS_UINT32 ATP_UTIL_ListGetCount(ATP_UTIL_LIST_ST *pstList);

VOS_VOID ATP_UTIL_ListAppend(
                        ATP_UTIL_LIST_ST            **ppstList,
                        ATP_UTIL_LIST_ST            *pstItem);

ATP_UTIL_LIST_ST *ATP_UTIL_ListFindItem(const ATP_UTIL_LIST_ST *pstList, const VOS_VOID *pvData,
                                        PFUtilListItemCompareProc pfCompareProc);

/*
 * Timer utilities
 */
typedef VOS_VOID (* PfUtilTimerProc)(VOS_VOID *pvArg);

typedef struct tagATP_UTIL_TIMER_ST
{
    struct tagATP_UTIL_TIMER_ST     *pstNext;

    struct timeval                  stExpireTime; // When the event will expire
                                                  // 0 means stopped timer
    VOS_UINT32                      ulPeriod;     // The event period
                                                  // 0 means stopped timer
    VOS_UINT32                      ulRepeat;     // Event repeat count
                                                  // 0 means repeat forever

    VOS_VOID                        *pvArg;
    PfUtilTimerProc                 pfProc;
} ATP_UTIL_TIMER_ST;

ATP_UTIL_TIMER_ST *ATP_UTIL_TimerAdd(
                            VOS_UINT32      ulPeriod,   // MSeconds
                            VOS_UINT32      ulRepeatCnt,
                            PfUtilTimerProc pfProc,
                            VOS_VOID        *pvData);
/*
  * Do not call this function in a timer callback function
  */
VOS_VOID ATP_UTIL_TimerDel(ATP_UTIL_TIMER_ST *pstTimer);

VOS_VOID ATP_UTIL_TimerChgPeriod(ATP_UTIL_TIMER_ST *pstTimer, VOS_UINT32 ulPeriod);

VOS_VOID ATP_UTIL_TimerGetIntv(const struct timeval* pstNewTime, 
                               const struct timeval* pstOldTime,
                               struct timeval* pstDelta);

VOS_VOID ATP_UTIL_TimerCheck(struct timeval* pstNextWakeupIntv);

VOS_VOID ATP_UTIL_TimerDestroy();


VOS_VOID ATP_MSG_SimpleBuilder(ATP_MSG_HEADER_ST *pstMsg, const VOS_CHAR *pcDest,
                                     VOS_UINT32 ulMsgType, VOS_UINT32 ulMsgData);

VOS_UINT32 ATP_MSG_SendSimpleMsg(const VOS_CHAR *pcDest,
                                     VOS_UINT32 ulMsgType, VOS_UINT32 ulMsgData);

#define ATP_MSG_SubscribeEvent(ulEvtType)   ATP_MSG_SendSimpleMsg(NULL, ATP_MSG_SUBSCRIBE_EVT, (ulEvtType))
#define ATP_MSG_UnSubscribeEvent(ulEvtType) ATP_MSG_SendSimpleMsg(NULL, ATP_MSG_UNSUBSCRIBE_EVT, (ulEvtType))


VOS_UINT32 ATP_MSG_SendMsgData(const VOS_CHAR* pcProcName, VOS_UINT32 ulMsgType, 
                               const VOS_VOID* pvMsgBuff, VOS_UINT32 ulBuffLen);

VOS_UINT32 ATP_MSG_SendRealTimeMsg(VOS_UINT32 ulMsgData);

VOS_UINT32 ATP_MSG_GetNumOfProcesses(const VOS_CHAR *pcName);

VOS_UINT32 ATP_MSG_SafeExitProcesses(const VOS_CHAR *pcName);

ATP_MSG_HEADER_ST *ATP_MSG_Dup(const ATP_MSG_HEADER_ST *pstMsg);

/*
 * Notify utility
 */
typedef VOS_UINT32 (*AtpNotifierProc)(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);

typedef struct tagATP_UTIL_NOTIFY_ENTRY_ST
{
    struct tagATP_UTIL_NOTIFY_ENTRY_ST      *pstNext;
    AtpNotifierProc                         pfProc;
    VOS_UINT32                              ulPriority;
    VOS_VOID                                *pvNotifyHook;
} ATP_UTIL_NOTIFY_ENTRY_ST;

typedef struct tagATP_UTIL_NOTIFIER_ST
{
    struct tagATP_UTIL_NOTIFIER_ST          *pstNext;
    VOS_VOID                                *pvNotifier;
    ATP_UTIL_NOTIFY_ENTRY_ST                *pstNotifyEntries;
} ATP_UTIL_NOTIFIER_ST;

VOS_UINT32 ATP_UTIL_NotifierInit(VOS_VOID               *pvNotifier,
                                      ATP_UTIL_NOTIFIER_ST   **ppstNewNotifyEntry);

VOS_UINT32 ATP_UTIL_NotifierRegister(ATP_UTIL_NOTIFIER_ST           *pstNotifier,
                                            AtpNotifierProc                 pfProc,
                                            VOS_UINT32                      ulPriority,
                                            VOS_VOID                        *pvNotifyHook,
                                            const ATP_UTIL_NOTIFY_ENTRY_ST  **ppstNotifyEntry);

VOS_VOID ATP_UTIL_NotifierUnRegister(ATP_UTIL_NOTIFIER_ST *pstNotifier, ATP_UTIL_NOTIFY_ENTRY_ST *pstNotifyEntry);

VOS_UINT32 ATP_UTIL_NotifierDoNotify(const ATP_UTIL_NOTIFIER_ST *pstNotifier, VOS_BOOL bStopWhenError);

/*
 *  File operation utilities
 */

/*!
  \brief  往文件中格式化写入字符串内容
  \param[in]  pcFileName: 待写的文件
  \param[in]  bAppend: VOS_FALSE表示覆盖文件原有的内容；VOS_TRUE表示往文件尾追加内容
  \param[in]  *fmt: 格式化的字符串
  \param[in]  ...: 
  \return 
  \retval -1:   出错
  \retval >=0:    写入的字符串个数
*/
VOS_INT32 ATP_UTIL_FilePrintf(const VOS_CHAR *pcFileName, VOS_BOOL bAppend, VOS_CHAR *fmt, ...);

/*!
  \brief 文件读入缓存
  \param[in]  *pcFileName: 
  \param[in]  **ppcContent: 
  \param[in]  *pulLen: 
  \return 
  \retval ATP_MSG_RET_PARA_ERR 非法入参
  \retval ATP_MSG_RET_FILE_NOT_EXIST 文件不存在
  \retval ATP_MSG_RET_MEM_ERR no memry
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_ReadFileToBuf(const VOS_CHAR *pcFileName, VOS_CHAR **ppcContent, VOS_UINT32 *pulLen);

/*!
  \brief 缓存写入文件
  \param[in]  *pcContent: 
  \param[in]  ulLen: 
  \param[in]  *pcFileName: 
  \return 
  \retval ATP_MSG_RET_PARA_ERR 非法入参
  \retval ATP_MSG_RET_FILE_NOT_EXIST 文件不存在
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_WriteBufToFile(const VOS_CHAR *pcContent, VOS_UINT32 ulLen, const VOS_CHAR *pcFileName);

/*!
  \brief 文件读入缓存
  \param[in]  *pcFileName: 
  \param[in]  **ppcContent: 
  \param[in]  *pulLen: 
  \param[in]  uloffset: 
  \return 
  \retval ATP_MSG_RET_PARA_ERR 非法入参
  \retval ATP_MSG_RET_FILE_NOT_EXIST 文件不存在
  \retval ATP_MSG_RET_MEM_ERR no memry
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_ReadFileToBuf_Ex(const VOS_CHAR *pcFileName
    , VOS_CHAR **ppcContent, VOS_UINT32 *pulLen, const VOS_UINT32 uloffset);

/*!
  \brief 缓存写入文件
  \param[in]  *pcContent: 
  \param[in]  ulLen: 
  \param[in]  *pcFileName: 
  \param[in]  uloffset: 
  \return 
  \retval ATP_MSG_RET_PARA_ERR 非法入参
  \retval ATP_MSG_RET_FILE_NOT_EXIST 文件不存在
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_WriteBufToFile_Ex(const VOS_CHAR *pcContent
, VOS_UINT32 ulLen,const VOS_UINT32 uloffset, const VOS_CHAR *pcFileName);


/*!
  \brief 更新动态应用程序是否被mic 启动
  \param[in]  *pcAppName: 需更新的应用程序名
  \param[in]  bStop: 是否停止当前被mic 启动进程
  \return 返回发送消息错误码
  \retval VOS_OK 更新消息发送成功
  \retval VOS_NOK pcAppName为NULL
  \retval ATP_MSG_RET_PARA_ERR  消息参数错误
  \retval ATP_MSG_RET_NOT_INITED 未初始化
  \retval ATP_MSG_RET_OVERFLOW  消息溢出
  \retval ATP_MSG_RET_PEER_DEAD  socket被peer关闭
  \retval ATP_MSG_RET_INTERNAL_ERR  内部错误
*/
VOS_UINT32 ATP_UTIL_UpdateDynAppStatus(const VOS_CHAR *pcAppName, VOS_BOOL bStart);

VOS_UINT32 ATP_UTIL_RestartDynApp(const VOS_CHAR *pcAppName);

/*
 *  Version utility
 */
const VOS_CHAR * ATP_MSG_GetVersion(void);


/*!
  \brief 进程权限临时切换至root
  \return 
  \retval ATP_MSG_RET_UPROOT_FAIL 权限提升失败
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_UpRootPrivileges(VOS_VOID);


/*!
  \brief 恢复进程自己权限
  \param[in]  *pcAppName: 进程名称
  \return 
  \retval ATP_MSG_RET_NOT_FOUNDPWD /etc/passwd 找不到该进程名
  \retval VOS_OK 权限设置成功
*/
VOS_UINT32 ATP_UTIL_DropRootPrivileges(const VOS_CHAR *pcAppName);

/*!
  \brief 强制设置进程权限，不可恢复
  \param[in]  *pcAppName: 
  \return 
  \retval ATP_MSG_RET_NOT_FOUNDPWD /etc/passwd 找不到该进程名
*/
VOS_UINT32 ATP_UTIL_ForceSetPrivileges(const VOS_CHAR *pcAppName);

/*!
  \brief 
  \param[in]  *pcUserName: 用户名
  \param[in]  ulGroupId: 默认组
  \return 
  \retval 
*/
VOS_UINT32 ATP_UTIL_PrivilegesAddUser(const VOS_CHAR *pcUserName, VOS_UINT32 ulGroupId);

/*!
  \brief 用户是否存在
  \param[in]  *pcUserName: 
  \return 
  \retval VOS_TRUE 已存在此用户
  \retval VOS_FALSE 不存在此用户
*/
VOS_BOOL ATP_UTIL_PrivilegesIsUserExist(const VOS_CHAR *pcUserName);


/*
  * String Utilities
  */

/*!
  \brief 执行字符串strcat操作，将pcSrc中的内容连接在pcDst后面，安全编码。
  \param[in]  pcDst: 目的字符串
  \param[in]  size: 连接后的字符串长度
  \param[in]  pcSrc: 待连接字符串
  \return 连接后的字符串
  \retval pcDst
*/
const VOS_CHAR * ATP_UTIL_StrCat(VOS_CHAR * pcDst, VOS_UINT32 size, VOS_CHAR * pcSrc);
/*!
  \brief 执行字符串Trim操作，去掉两头的空白字符,
          在原字符串上修改，未开辟空间,输入为空格时，直接返回
  \param[in]  *pszStr: 输入的字符串，可以为空
  \return  Trim后的字符串指针
  \retval pszStr  Trim后的字符串指针，可以为空
*/
VOS_CHAR * ATP_UTIL_StrTrim( VOS_CHAR *pszStr );

typedef struct tagATP_UTIL_STR_IDX_MAP_ST
{
    const VOS_CHAR      *pcStrInfo;
    VOS_INT32           lIdx;
} ATP_UTIL_STR_IDX_MAP_ST;


const VOS_CHAR *ATP_UTIL_StrIdxArrayGetStrByIdx(VOS_INT32 lIdx,const ATP_UTIL_STR_IDX_MAP_ST* pstSrcStrIdx);
VOS_INT32 ATP_UTIL_StrIdxArrayGetIdxByStr(const VOS_CHAR *pcStr,const ATP_UTIL_STR_IDX_MAP_ST* pstSrcStrIdx);

const VOS_CHAR *ATP_UTIL_GetStrFromIdx(VOS_INT32 lIdx, const VOS_CHAR * const apcBaseLine[],
                                  VOS_INT32 lBaseLineLen, const VOS_CHAR * const apcExtension[]);

VOS_INT32 ATP_UTIL_GetIdxFromStr(const VOS_CHAR *pcStr, const VOS_CHAR * const apcBaseLine[],
                                      VOS_INT32 lBaseLineLen, const VOS_CHAR * const apcExtension[]);

VOS_VOID ATP_UTIL_Hex2Str(VOS_CHAR *pcdst, const VOS_CHAR *pcsrc, VOS_INT32 lStrlen);

#ifdef ATP_DEBUG
#define ATP_MSG_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_MSG_DEBUG(x...)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif

