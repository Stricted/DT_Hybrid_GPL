/**\file atssdkproc.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
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

//!���ڴ��豸�ļ��ж�ȡ�ַ�
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

//!pcui��ȡ״̬ ö�� 
typedef enum
{
    //!��״̬
    ATS_READ_USB,
    //!����״̬
    ATS_PARSE_RECDATA,
    //!��ȡ��ȷֵ״̬
    ATS_FIND_RESULT, 
}ATS_READ_STATE_EN;

typedef struct
{
    VOS_INT32 lMsgType; 
    VOS_CHAR acText[AT_RCV_MSG_LENGTH];
}ATS_RCV_MSG_ST;

#define HSPA_AT_DIAL            55       //HSPA����꣬�������
#define HSPA_AT_DIAG_PORT_SET   56       //HSPA����꣬DIAG���л�����
#define HSPA_AT_ACCEPT          57       //HSPA����꣬����
#define HSPA_AT_CHUP            58       //HSPA����꣬���йҶ�
#define HSPA_AT_CALL_TIME_QUERY  62      //HSPA����꣬ͨ��ʱ����ѯ
#define HSPA_AT_AUTO_SIM_STATUS_REPORT 63  //HSPA����꣬SIM ״̬�仯ָʾ
#define HSPA_AT_RING_REPORT     64       //HSPA����꣬��������ָʾ
#define HSPA_AT_CALL_NUM_REPORT  65      //HSPA����꣬���������ʾ
#define HSPA_AT_CALL_END_REPORT  66      //HSPA����꣬ͨ������ָʾ
#define HSPA_AT_CALL_ORIG_REPORT  67      //HSPA����꣬�������ָʾ
#define HSPA_AT_CALL_CONF_REPORT  68      //HSPA����꣬������ָͨʾ
#define HSPA_AT_CONN_REPORT      71      //HSPA����ָ꣬ʾ���н�ͨ
#define HSPA_AT_CMD_MAX          100     //HSPA�����ֵ����

#define ATS_MODULE 1

typedef enum{
    ATS_READ_INIT_STATE = 0,//��ʼ��
    ATS_READ_START_STATE ,//���ڶ�USB
    ATS_READ_OVER_STATE,//����USB
}ATS_READUSB_STATE_EN;

typedef enum tagATP_ATS_RET_CODE_EN
{
    ATP_ATS_RET_INTERNAL_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_ATSERVER),
    ATP_ATS_RET_PARA_ERR,
    ATP_ATS_RET_APPNAME_ERR,
    ATP_ATS_RET_MEM_ERR,
    ATP_ATS_RET_INVALID_VALUE,
} ATP_ATS_RET_CODE_EN;

//!���ȼ�ö��
typedef enum{
    ATS_HIGHEST_PRIORITY = 0,//������ȼ�:atserver��ʼ������
    ATS_HIGHER_PRIORITY,//�θ����ȼ�:��������
    ATS_NORMAL_PRIORITY,//�������ȼ�:atcmd����
    ATS_LOWER_PRIORITY,
    ATS_LOWEST_PRIORITY
}ATS_PRIORITY_EN;

typedef struct tagATS_MSG_LIST_ST
{
    struct tagATS_MSG_LIST_ST   *pstNext;
    VOS_UINT32 ulPriority;
    VOS_CHAR    aucSendApp[ATP_MSG_MAX_NAME_LEN + 1];    //�����Դ��Ϊ�˴���atcmd�Ĳ�������
    ATS_MSG_ST stAtsMsg;
} ATS_MSG_LIST_ST;


#if defined(__cplusplus)
extern "C" {
#endif

typedef VOS_UINT32 (* PFUtilListItemMsgProc)(ATS_MSG_LIST_ST *pstItem);

typedef VOS_UINT32 (* PFUtilInputMsgListDeleteItem)(ATS_MSG_ST  *stMsg,VOS_INT32 *lRetCode);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief �������ȼ���ָ�������м���ATSERVER MSG��Ϣ
  \param[in]  ulIndex: 
  \param[in]  ulEventType: 
  \param[in]  stAtsMsg: 
  \param[in]  **ppstObjList: 
  \return 
  \retval
*/
VOS_UINT32 AtsMsgAppendProc(VOS_UINT32 ulFlag, const VOS_CHAR *pszAppName, VOS_UINT32 ulPriority, ATS_MSG_ST *stMsg, ATS_MSG_LIST_ST *pstObjList);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief ɾ���������׸���㣬������ý��
  \param[in]  **ppstStart: 
  \param[in]  pfDestroy: 
  \return 
  \retval
*/
VOS_VOID AtsMsgListDeleteItem(ATS_MSG_LIST_ST         *pstStart, PFUtilListItemMsgProc   pfDestroy);/* <DTS2013083001384 xiazhongyue 20130830 modify> */

/*!
  \brief ɾ����������н��(������Դ)
  \param[in]  *pstList: 
  \return 
  \retval
*/
VOS_VOID AtsMsgListDeleteAll(ATS_MSG_LIST_ST            *pstList);


/*!
  \brief ɾ�����������еĽ�㣬������������С�
  \param[in]  ulFlag: �Ƿ�ϲ���������־λ:Ϊ1�ϲ���Ϊ0���ϲ�
  \param[in]  **ppstInputMsglist: 
  \param[in]  **ppstOutputMsglist: 
  \param[in]  pfDestroy: ��ǰ���Ĵ�����
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
