/**\file atssdkproc.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-1-7
*/
#ifndef __ATS_SDK_PROC_H__
#define __ATS_SDK_PROC_H__
#include "atptypes.h"
#include "httpapi.h"
#include "at_if.h"
#include "cspub.h"

typedef enum
{
    ATS_FIND_CR_CF = 0,
    ATS_FIND_CR,
    ATS_NO_FIND_CR_CF 
}ATS_FOUND_PARAM_EN;

//!用于从设备文件中读取字符
#define LOOPBUFSIZE       2048
#define READ_SIZE                        512
typedef struct  {
    VOS_CHAR acStrmBuf[LOOPBUFSIZE];
    VOS_CHAR* pcCur;
    VOS_CHAR* pcRead;
}AtsTtyReportStream_st;

typedef enum 
{
    ATS_NO_FOUND = 0,
    ATS_FOUND
}ATS_IS_FIND_EN;

#define CMDNOTSUPPORT "COMMAND NOT SUPPORT"/* <DTS2013083001384 xiazhongyue 20130830 modify> */

//!pcui读取状态 枚举 
typedef enum
{
    //!读状态
    ATS_READ_USB,
    //!解释状态
    ATS_PARSE_RECDATA,
    //!获取正确值状态
    ATS_FIND_RESULT, 
}ATS_READ_STATE_EN;

typedef struct
{
    VOS_INT32 lMsgType; 
    VOS_CHAR acText[AT_RCV_MSG_LENGTH];
}ATS_RCV_MSG_ST;

#define HSPA_AT_DIAL            55       //HSPA命令宏，发起呼叫
#define HSPA_AT_DIAG_PORT_SET   56       //HSPA命令宏，DIAG口切换设置
#define HSPA_AT_ACCEPT          57       //HSPA命令宏，接听
#define HSPA_AT_CHUP            58       //HSPA命令宏，呼叫挂断
#define HSPA_AT_CALL_TIME_QUERY  62      //HSPA命令宏，通话时长查询
#define HSPA_AT_AUTO_SIM_STATUS_REPORT 63  //HSPA命令宏，SIM 状态变化指示
#define HSPA_AT_RING_REPORT     64       //HSPA命令宏，来电响铃指示
#define HSPA_AT_CALL_NUM_REPORT  65      //HSPA命令宏，来电号码显示
#define HSPA_AT_CALL_END_REPORT  66      //HSPA命令宏，通话结束指示
#define HSPA_AT_CALL_ORIG_REPORT  67      //HSPA命令宏，发起呼叫指示
#define HSPA_AT_CALL_CONF_REPORT  68      //HSPA命令宏，网络连通指示
#define HSPA_AT_CONN_REPORT      71      //HSPA命令宏，指示主叫接通
#define HSPA_AT_CMD_MAX          100     //HSPA命令宏值上限

#define ATS_MODULE 1

typedef enum{
    ATS_READ_INIT_STATE = 0,//初始化
    ATS_READ_START_STATE ,//正在读USB
    ATS_READ_OVER_STATE,//读完USB
}ATS_READUSB_STATE_EN;

typedef enum tagATP_ATS_RET_CODE_EN
{
    ATP_ATS_RET_INTERNAL_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_ATSERVER),
    ATP_ATS_RET_PARA_ERR,
    ATP_ATS_RET_APPNAME_ERR,
    ATP_ATS_RET_MEM_ERR,
    ATP_ATS_RET_INVALID_VALUE,
} ATP_ATS_RET_CODE_EN;

//!优先级枚举
typedef enum{
    ATS_HIGHEST_PRIORITY = 0,//最高优先级:atserver初始化请求
    ATS_HIGHER_PRIORITY,//次高优先级:语音请求
    ATS_NORMAL_PRIORITY,//正常优先级:atcmd请求
    ATS_LOWER_PRIORITY,
    ATS_LOWEST_PRIORITY
}ATS_PRIORITY_EN;

typedef struct tagATS_MSG_LIST_ST
{
    struct tagATS_MSG_LIST_ST   *pstNext;
    VOS_UINT32 ulPriority;
    VOS_CHAR    aucSendApp[ATP_MSG_MAX_NAME_LEN + 1];    //结点来源，为了处理atcmd的并发命令
    ATS_MSG_ST stAtsMsg;
} ATS_MSG_LIST_ST;


#if defined(__cplusplus)
extern "C" {
#endif

typedef VOS_UINT32 (* PFUtilListItemMsgProc)(ATS_MSG_LIST_ST *pstItem);

typedef VOS_UINT32 (* PFUtilInputMsgListDeleteItem)(ATS_MSG_ST  *stMsg,VOS_INT32 *lRetCode);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief 根据优先级在指定链表中加入ATSERVER MSG消息
  \param[in]  ulIndex: 
  \param[in]  ulEventType: 
  \param[in]  stAtsMsg: 
  \param[in]  **ppstObjList: 
  \return 
  \retval
*/
VOS_UINT32 AtsMsgAppendProc(VOS_UINT32 ulFlag, const VOS_CHAR *pszAppName, VOS_UINT32 ulPriority, ATS_MSG_ST *stMsg, ATS_MSG_LIST_ST *pstObjList);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief 删除链表中首个结点，并处理该结点
  \param[in]  **ppstStart: 
  \param[in]  pfDestroy: 
  \return 
  \retval
*/
VOS_VOID AtsMsgListDeleteItem(ATS_MSG_LIST_ST         *pstStart, PFUtilListItemMsgProc   pfDestroy);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief 删除链表的所有结点(回收资源)
  \param[in]  *pstList: 
  \return 
  \retval
*/
VOS_VOID AtsMsgListDeleteAll(ATS_MSG_LIST_ST            *pstList);


/*!
  \brief 删除输入链表中的结点，放入输出链表中。
  \param[in]  ulFlag: 是否合并输出链表标志位:为1合并；为0不合并
  \param[in]  **ppstInputMsglist: 
  \param[in]  **ppstOutputMsglist: 
  \param[in]  pfDestroy: 当前结点的处理函数
  \return 
  \retval
*/
VOS_VOID AtsMsgListDeleteItemAppendList(
                        ATS_MSG_LIST_ST            *pstInputMsglist,
                        ATS_MSG_LIST_ST            *pstOutputMsglist,
                        PFUtilListItemMsgProc   pfDestroy);/* <DTS2013083001384 xiazhongyue 20130830 modify> */


VOS_INT32 AtsInitGlobalParameters(VOS_VOID);

VOS_VOID AtsInitReadStreamBuf(VOS_VOID);

VOS_INT32 AtsParseSysinfo(StSysinfo* sysinfo);
VOS_VOID ATP_ATS_RecvMsgEvtProc(const ATP_SOCK_BASE_ST *pstSockBase);

VOS_VOID NotifyWanUmts(VOS_VOID);

VOS_INT32 ATP_PinManagement (VOS_INT32 lCmdType,
                                  VOS_CHAR *pcPin, 
                                  VOS_CHAR *pcNewPin, 
                                  PinMgResultCodeEx *pstResult);

VOS_INT32 AtsReadFromUsb(ATS_MSG_ST* pstMsg, VOS_INT32 * lRetcode);

#ifdef SUPPORT_ATP_VOICE
VOS_VOID NotifyVoice(VOS_VOID);
#ifdef SUPPORT_ATP_VOICE_DT
VOS_INT32 AtsSendMsgToCsVoice(VOS_UINT32 ulMsgType, VOS_CHAR *pcBoby, VOS_UINT32 ulBodyLen);
#endif
#endif

/* <DTS2013083001384 xiazhongyue 20130830 begin */
void WriteFile(const char *file, const char *fmt, ...);
void writeFileEx(const char *file, const char *fmt, ...);
VOS_INT32 AT_LockFile(const VOS_CHAR *pszFileName, FILE ** fFile);
VOS_VOID AT_UnlockFile(VOS_INT32 iLockFd);
VOS_INT32 CreatePidFile(const VOS_CHAR * filename ,VOS_INT32 flag);
VOS_INT32 AtSmsReportHandler(VOS_CHAR *rcv_buf,VOS_INT32  *retcode,VOS_INT32  report_index);
VOS_INT32 AtCmtReportHandler(VOS_CHAR *rcv_buf,VOS_INT32  *retcode);
VOS_INT32 AtSmsFullReportHandler(VOS_CHAR *rcv_buf,VOS_INT32  *retcode,VOS_INT32  report_index);
VOS_INT32 IsFileExists(VOS_CHAR * filename);
VOS_INT32 DeleteFile(VOS_CHAR * filename);
VOS_INT32 AtsRecToStream();
VOS_INT32 AtsPraserExcResult(ATS_MSG_ST *pstMsg);
VOS_INT32 AtsGetActiveReport (ATS_MSG_ST* pstMsg, VOS_INT32 * lRetcode);
VOS_INT32 AtsPaserResponse(ATS_MSG_ST *pstMsg);
VOS_INT32 ATP_AtserverSendMsg(const VOS_CHAR *pcDest, VOS_UINT32 ulEventType, ATS_RCV_MSG_DATA_ST *stRecMsg);
/*  DTS2013083001384 xiazhongyue 20130830 end > */

#if defined(__cplusplus)
}
#endif

#endif
