/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : cwmpstkapi.h
  作    者  : handy
  版    本  : V1.0
  创建日期  : 2007-12-30
  描    述  : CWMP协议栈对外头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

*********************************************************************************/

#ifndef __CWMP_STK_API_H__
#define __CWMP_STK_API_H__

#include "atptypes.h"
#include "msgapi.h"
#include "xmlapi.h"
#include "httpapi.h"

/* Function for reading event list from flash */
typedef VOS_VOID (*PFAtpCwmpReadStkState)(VOS_CHAR *pcState,
                                             VOS_UINT32 ulLength,
                                             VOS_UINT32 *pulUpgRes);

/* Function prototype for serialize event list to flash */
typedef VOS_VOID (*PFAtpCwmpWriteStkState)(const VOS_CHAR *pcState,
                                              VOS_UINT32 ulLength);

/* Inform hook, we can change inform packet by this hook */
typedef VOS_UINT32 (* PFAtpCwmpInformHook)(NODEPTR hInformSoap);

/*
 *  Parameter Type for TR069
 */
typedef enum
{
    ATP_CWMP_VALUE_TYPE_CHAR        = 1,
    ATP_CWMP_VALUE_TYPE_LONG        = 2,
    ATP_CWMP_VALUE_TYPE_ULONG       = 3,
    ATP_CWMP_VALUE_TYPE_BOOL        = 4,
    ATP_CWMP_VALUE_TYPE_DATE        = 5,
    ATP_CWMP_VALUE_TYPE_BASE64      = 6,
    ATP_CWMP_VALUE_TYPE_ANY         = 7
} ATP_CWMP_ATTR_VALUE_TYPE_E;

/* Changing CFM parameter type to CWMP parameter type */
#define ATP_VALUETYPE_TO_CWMP(x)    ((ATP_CWMP_ATTR_VALUE_TYPE_E)(((x) & CMO_DATA_TYPE_MASK)>>23))

/*
 *  Event List for inform
 */
typedef struct tagATP_CWMP_EVT_ST
{
    struct tagATP_CWMP_EVT_ST       *pstNext;

    const VOS_CHAR                  *pcCmdKey;
    const VOS_CHAR                  *pcRpcArg;
    VOS_UINT8                       ucEvtDescIdx;
    VOS_UINT8                       ucStopped;
    VOS_UINT8                       ucWaitForConfirm;
    //lint -save -e754
    VOS_UINT8                       ucReserved;
    //lint restore
} ATP_CWMP_EVT_ST;

/*
 *  Parameter List for inform
 */

typedef struct tagATP_CWMP_VALUE_STRUCT_ST
{
    struct tagATP_CWMP_VALUE_STRUCT_ST  *pstNext;

    VOS_CHAR                        *pcName;
    VOS_CHAR                        *pcValue;
    ATP_CWMP_ATTR_VALUE_TYPE_E      enValueType;
} ATP_CWMP_VALUE_STRUCT_ST;

/*
 *  Device ID for inform
 */
typedef struct tagATP_CWMP_DEVICEID_ST
{
    VOS_CHAR            *pcManufacturer;
    VOS_CHAR            *pcOUI;
    VOS_CHAR            *pcProductClass;
    VOS_CHAR            *pcSerialNumber;
} ATP_CWMP_DEVICEID_ST;

/*
 *  Session ctrl related structures
 */
typedef enum tagATP_CWMP_SESS_CTRL_TYPE_EN
{
	ATP_CWMP_SESS_START,		// 会话开始回调函数
	ATP_CWMP_SESS_END,		    // 会话结束时回调函数(无论成功还是失败)
	ATP_CWMP_SESS_MAX
} ATP_CWMP_SESS_CTRL_TYPE_EN;

/* Session control function prototype */
typedef VOS_UINT32 (*PFCwmpSessionCtrlFunc)(
                                            VOS_UINT32  ulErrCode,
                                            VOS_VOID    *pvArg);

typedef struct tagATP_CWMP_SESS_CTRL_FUNCS
{
	PFCwmpSessionCtrlFunc               pfSessionCtrlFunc;
	VOS_UINT32                          ulSessCtrlType;
    VOS_VOID                            *pvArg;
	struct tagATP_CWMP_SESS_CTRL_FUNCS	*pstNext;
} ATP_CWMP_SESS_CTRL_FUNCS;

#ifdef SUPPORT_ATP_CWMP_MULTI_ENVELOPE
typedef struct tagATP_CWMP_ENVELOPE_ST
{
    struct tagATP_CWMP_ENVELOPE_ST      *pstNext;
    NODEPTR                             hBody;
    NODEPTR                             hResponse;
    const VOS_CHAR                      *pcSoapID;
    VOS_BOOL                            bHoldRequest;
} ATP_CWMP_ENVELOPE_ST;
#else
#define ATP_CWMP_MAX_ENVELOPES_NUM_D 1    
#endif

typedef struct tagATP_CWMP_STK_ST
{
    ATP_HTTP_SERVER_ST      *pstHttpServer;
    ATP_HTTP_CLIENT_ST      *pstHttpClient;

    VOS_UINT32              ulRetryCnt;

    VOS_UINT8               bHoldRequest;
    VOS_UINT8               bSentEmpty;
    VOS_UINT8               bInforming;
    VOS_UINT8               bIsBusy;
    VOS_UINT8               bResession;
    VOS_UINT8               bFirstInit;
    VOS_UINT8               bInformEnable;
    VOS_UINT8               ucAuthMode;

    VOS_UINT32              ulInformPeriod;
#ifdef SUPPORT_ATP_CWMP_MULTI_ENVELOPE
    VOS_UINT32              ulNumOfEnvelopes;
    VOS_UINT32              ulPeerEnvelopes;
    ATP_CWMP_ENVELOPE_ST    *pstEnvelopes;
#endif

    ATP_CWMP_EVT_ST         *pstEvtList;

    ATP_CWMP_VALUE_STRUCT_ST *pstRequiredParaList;
    ATP_CWMP_VALUE_STRUCT_ST *pstValueChgParaList;
    ATP_CWMP_VALUE_STRUCT_ST *pstExtIP;

    ATP_CWMP_SESS_CTRL_FUNCS *pstSessCtrls;

    PFAtpCwmpReadStkState   pfReadState;
    PFAtpCwmpWriteStkState  pfWriteState;
    PFAtpCwmpInformHook     pfInformHook;

    VOS_VOID                *pvMemHdl;
    const VOS_CHAR          *pcConReqUrl;

    ATP_CWMP_DEVICEID_ST    stDeviceId;
#if  (ATP_CWMP_MAX_ENVELOPES_NUM_D == 1)   
    VOS_UINT8               bSendCpeRequest;
#endif
#ifdef SUPPORT_ATP_TR181
    VOS_UINT32              ulCwmpVersion;
#endif
} ATP_CWMP_STK_ST;

/*
 *  CWMP stack error code
 */
typedef enum tagATP_CWMP_STK_ERR_CODE
{
    ATP_ERR_CWMP_STK_START              = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CWMP),
    ATP_ERR_CWMP_STK_PARA_INVALID,
    ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH,
    ATP_ERR_CWMP_STK_INVALID_FORMAT,
    ATP_ERR_CWMP_STK_TR069_NOT_INITED,
    ATP_ERR_CWMP_STK_NET_HTTP,
    ATP_ERR_CWMP_STK_NET_SEC,
    ATP_ERR_CWMP_STK_SOAP_XML,
    ATP_ERR_CWMP_STK_SOAP_ID,
    ATP_ERR_CWMP_STK_SOAP_FAULT
} ATP_CWMP_STK_ERR_CODE_EN;

/* Default Unknown Time defined by TR069 spec */
#define ATP_CWMP_DEFAULT_UNKNOWN_TIME       "0001-01-01T00:00:00Z"

/*
 *  CWMP basic stack operation
 */
ATP_CWMP_STK_ST *ATP_CWMP_StkInit(PFAtpCwmpReadStkState           pfReadState,
                                   PFAtpCwmpWriteStkState    pfWriteState);

VOS_UINT32  ATP_CWMP_StkStartSession(ATP_CWMP_STK_ST *pstCwmp);

VOS_VOID ATP_CWMP_StkSetAuthMode(ATP_CWMP_STK_ST *pstCwmp, E_HTTPAUTH_TYPE enAuthType);

VOS_UINT32 ATP_CWMP_StkGetInformPeriod(ATP_CWMP_STK_ST *pstCwmp);

VOS_UINT32 ATP_CWMP_StkGetNextRetrySeconds(ATP_CWMP_STK_ST *pstCwmp);

VOS_UINT32 ATP_CWMP_StkDoSafeProc(ATP_CWMP_STK_ST *pstCwmp, PFCwmpSessionCtrlFunc pfSafeProc, VOS_VOID *pvHook);

VOS_BOOL ATP_CWMP_StkNeedToRetry(ATP_CWMP_STK_ST *pstCwmp);

/*
 *  Session control functions
 */

VOS_UINT32  ATP_CWMP_RegSessionCtrlFunc(
                            ATP_CWMP_STK_ST                 *pstCwmp,
                            PFCwmpSessionCtrlFunc           pfFunc,
                            ATP_CWMP_SESS_CTRL_TYPE_EN      enFuncType,
                            VOS_BOOL                        bDeleteByCurSess,
                            VOS_BOOL                        bOverwrite,
                            VOS_VOID                        *pvArg);

#define ATP_CWMP_RegEndSessionFunc(pstCwmp, pfFunc, pvArg)  ATP_CWMP_RegSessionCtrlFunc(pstCwmp, (pfFunc), ATP_CWMP_SESS_END, VOS_TRUE, VOS_TRUE, (pvArg))


/* Namespace interface*/
#define ATP_CWMP_NAMESPACE_LEN_D    (32)

typedef struct tagATP_CWMP_NAME_SAPCE_ST
{
    const VOS_CHAR      *acSoapEnv;
    const VOS_CHAR      *acSoapEnc;
    const VOS_CHAR      *acSoapXsd;
    const VOS_CHAR      *acSoapXsi;
    const VOS_CHAR      *acCwmp;
} ATP_CWMP_NAME_SAPCE_ST;

extern const ATP_CWMP_NAME_SAPCE_ST g_stNameSpace;


/*
 *  RPC prototypes
 */

/* Function prototype for RPC */
typedef VOS_UINT32 (*PFATPCWMPRpcMethod)(
                        ATP_CWMP_STK_ST               *pstCwmp,
                        NODEPTR                       hRpcRequest,
                        NODEPTR                       *phRpcResponse);

typedef VOS_UINT32 (*PFATPCWMPRpcPreProc)(
                        ATP_CWMP_STK_ST               *pstCwmp,
                        VOS_UINT32                    ulRpcIdx,
                        NODEPTR                       hRpcRequest,
                        NODEPTR                       *phRpcResponse,
                        VOS_BOOL                      *pbContinue);

extern PFATPCWMPRpcPreProc  g_pfRpcPreProc;

typedef VOS_UINT32 (*PFATPCWMPConnectProc)(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_UINT32 ulErrCode, VOS_UINT32 ulTryIdx, VOS_BOOL *pbNeedRetry);
extern PFATPCWMPConnectProc  g_pfConnectMultiIp;

typedef struct tagATP_CWMP_RPC_DESC_ST
{
    VOS_CHAR                        *pcRPCName;
    PFATPCWMPRpcMethod              pfRpcMethod;
} ATP_CWMP_RPC_DESC_ST;

/*Start of 维护管理组 2010-3-17 16:37 for 问题单:AU8D04266 by x00125299*/
typedef struct tagATP_SOAP_ERROR_ST
{
    VOS_UINT32          ulErrCode;
    VOS_CHAR            *pcErrString;
} ATP_SOAP_ERROR_ST;

extern const ATP_SOAP_ERROR_ST  *m_astExternErrs;
/*End of 维护管理组 2010-3-17 16:37 for 问题单:AU8D04266 by x00125299*/

typedef enum tagATP_CWMP_CPE_RPC_IDX_EN
{
    ATP_CWMP_CPE_RPC_GETNAMES,
    ATP_CWMP_CPE_RPC_GETVALUES,
    ATP_CWMP_CPE_RPC_SETVALUES,
    ATP_CWMP_CPE_RPC_GETATTRS,
    ATP_CWMP_CPE_RPC_SETATTRS,
    ATP_CWMP_CPE_RPC_ADDOBJ,
    ATP_CWMP_CPE_RPC_DELOBJ,
    ATP_CWMP_CPE_RPC_DOWNLOAD,
    ATP_CWMP_CPE_RPC_UPLOAD,
    ATP_CWMP_CPE_RPC_REBOOT,
    ATP_CWMP_CPE_RPC_RESET,
    ATP_CWMP_CPE_RPC_SCHEDULEINFORM,
    ATP_CWMP_CPE_RPC_GETRPC,

    ATP_CWMP_CPE_RPC_NONE   = 0xFF  // Do not modify this value
} ATP_CWMP_CPE_RPC_IDX_EN;

typedef enum tagATP_CWMP_ACS_RPC_IDX_EN
{
    ATP_CWMP_ACS_RPC_INFORM,
    ATP_CWMP_ACS_RPC_TRANS_COMPLETE,
#ifdef SUPPORT_ATP_CWMP_CONTROL_OSGI    
    ATP_CWMP_ACS_RPC_DU_STATE_CHANGE_COMPLETE,
#endif
    ATP_CWMP_ACS_RPC_NONE   = ATP_CWMP_CPE_RPC_NONE // Do not modify this value
} ATP_CWMP_ACS_RPC_IDX_EN;

extern const ATP_CWMP_RPC_DESC_ST g_astCpeRpcs[];
extern const ATP_CWMP_RPC_DESC_ST g_astAcsRpcs[];
extern const VOS_UINT8 g_ucNumOfCpeRpcs;
extern const VOS_UINT8 g_ucNumOfAcsRpcs;

/*
 *  Inform functions
 */

VOS_UINT32 ATP_CWMP_SetDeviceID(ATP_CWMP_STK_ST *pstCwmp, const ATP_CWMP_DEVICEID_ST *pstDeviceID);

/*
 *  Inform parameter list functions
 */
VOS_UINT32 ATP_CWMP_AddParameter(
                                ATP_CWMP_STK_ST             *pstCwmp,
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E  enValueType);

VOS_UINT32 ATP_CWMP_AddParaValueChg(
                                ATP_CWMP_STK_ST             *pstCwmp,
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E  enValueType);

VOS_UINT32 ATP_CWMP_SetConnectionIP(ATP_CWMP_STK_ST    *pstCwmp,
                                             const VOS_CHAR     *pcParaName,
                                             const VOS_CHAR     *pcParaValue);

VOS_VOID ATP_CWMP_DelParameter(ATP_CWMP_STK_ST *pstCwmp, const VOS_CHAR *pcParaName);

/*
 *  Inform event list functions
 */

/* This enumeration must be aligned with g_astEvts */
typedef enum tagATP_CWMP_EVT_IDX_EN
{
    ATP_CWMP_EVT_BOOTSTRAP,
    ATP_CWMP_EVT_BOOT,
    ATP_CWMP_EVT_PERIODIC,
    ATP_CWMP_EVT_SCHEDULED,
    ATP_CWMP_EVT_VALUECHANGE,
    ATP_CWMP_EVT_CONNREQ,
    ATP_CWMP_EVT_TRANSCOMPLETE,
    ATP_CWMP_EVT_DIAGCOMPLETE,
#ifdef SUPPORT_ATP_CWMP_CONTROL_OSGI    
    ATP_CWMP_EVT_DUSTATECHANGECOMPLETE,
#endif
    ATP_CWMP_EVT_MREBOOT,
    ATP_CWMP_EVT_MSCHEDULE,
    ATP_CWMP_EVT_MDOWNLOAD,
    ATP_CWMP_EVT_MUPLOAD,
#ifdef SUPPORT_ATP_CWMP_CONTROL_OSGI    
    ATP_CWMP_EVT_MDUCHANGESTATE,
#endif
#ifdef SUPPORT_ATP_CWMP_SCHDWL
    ATP_CWMP_EVT_MSCHDOWNL,
#endif
    ATP_CWMP_EVT_NONE   = 0xFF  // Do not modify this value
} ATP_CWMP_EVT_IDX_EN;

#define ATP_CWMP_EVT_MULTIPLE           (0x80)

typedef enum tagATP_CWMP_EVT_RETRY_POLICY_EN
{
    ATP_CWMP_EVT_RETRY_EXCLUSIVE,       // 必须重试，且不和其它事件共存，
                                        // 如O BOOTSTRAP事件
    ATP_CWMP_EVT_RETRY_FOREVER,         // 即使掉电重启也需要重试的事件
    ATP_CWMP_EVT_RETRY_ALIVE,           // 掉电后不需要重试的事件
    ATP_CWMP_EVT_RETRY_NO,              // 不需要重试的事件
    ATP_CWMP_EVT_RETRY_ARBITRARY        // 没有定义的事件
} ATP_CWMP_EVT_RETRY_POLICY_EN;

#define ATP_IS_EVT_MULTIPLE(x)  ((0 != ((x) & ATP_CWMP_EVT_MULTIPLE)) ? VOS_TRUE : VOS_FALSE)
#define ATP_GET_EVT_POLICY(x)   ((x) & (~ATP_CWMP_EVT_MULTIPLE))

typedef struct tagATP_CWMP_EVT_DESC_ST
{
    const VOS_CHAR              *pcEvtName;     // 事件名称，注册时必须给出

    VOS_UINT8                   ucRetryPolicy;  // 重试策略，注册时必须填充,最高位表示是否可以累积
    VOS_UINT8                   ucConfirmRpc;   // 要确认事件被ACS接收到，CPE所必须接收到的回应RPC方法
    VOS_UINT8                   ucRequestRpc;   // 在Inform中上报完本事件后，需要发送给ACS的RPC方法
    VOS_UINT8                   ucRelatedEvt;   // 关联的事件，如M Reboot和1 Boot的关联
} ATP_CWMP_EVT_DESC_ST;

extern const ATP_CWMP_EVT_DESC_ST g_astEvts[];
extern const VOS_UINT8 g_ucNumOfEvts;

#define ATP_CWMP_EVT_NOT_SERIALIZE  (0x80000000)
typedef enum tagATP_CWMP_EVT_ADD_TYPE_EN
{
    ATP_CWMP_EVT_ACTIVE_INFORM      = 1,
    ATP_CWMP_EVT_PASSIVE_INFORM,
    ATP_CWMP_EVT_NOT_CONFIRMED,
    ATP_CWMP_EVT_STOPPED,

    ATP_CWMP_EVT_MAX
} ATP_CWMP_EVT_ADD_TYPE_EN;

#define ATP_IS_EVT_NEED_SERIALIZE(x)    ((0 != ((x) & ATP_CWMP_EVT_NOT_SERIALIZE)) ? VOS_FALSE : VOS_TRUE)
#define ATP_GET_EVT_ADD_TYPE(x) ((x) & (~ATP_CWMP_EVT_NOT_SERIALIZE))

VOS_UINT32 ATP_CWMP_AddEvent(ATP_CWMP_STK_ST        *pstCwmpHdl,
                                ATP_CWMP_EVT_IDX_EN         enEvtIdx,
                                const VOS_CHAR              *pcCmdKey,
                                const VOS_CHAR              *pcReqRpc,
                                ATP_CWMP_EVT_ADD_TYPE_EN    enEvtType,
                                VOS_VOID                    **ppvEvtHandle);

VOS_VOID ATP_CWMP_DeleteEvent(
                                ATP_CWMP_STK_ST             *pstCwmp,
                                ATP_CWMP_EVT_IDX_EN         enEvtIdxs,
                                const VOS_CHAR              *pstCommandKey);
VOS_VOID ATP_CWMP_DeleteEventEx(
                                ATP_CWMP_STK_ST             *pstCwmp,
                                ATP_CWMP_EVT_ST             *pstEvt);

VOS_UINT32 ATP_CWMP_ActivateEvent(ATP_CWMP_STK_ST *pstCwmp, VOS_VOID *pvEvtHandle, VOS_BOOL bActiveInform);

VOS_UINT32 ATP_CWMP_EventSetRpcAndCmdKey(ATP_CWMP_STK_ST *pstCwmp, VOS_VOID *pvEvtHandle,
                                                      const VOS_CHAR *pcRpc, const VOS_CHAR *pcCmdKey);

/*
 *  CWMP RPC Error Codes
 */
typedef enum tagATP_CWMP_CPE_FAULT_CODES_EN
{
    ATP_CWMP_RPC_CPE_OK                                 = 0,
    ATP_CWMP_RPC_CPE_METHOD_NOT_SPT                     = 9000,
    ATP_CWMP_RPC_CPE_REQ_DENIED                         = 9001,
    ATP_CWMP_RPC_CPE_INTER_ERROR                        = 9002,
    ATP_CWMP_RPC_CPE_INVALID_ARG                        = 9003,
    ATP_CWMP_RPC_CPE_RES_EXCD                           = 9004,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_NAME                 = 9005,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_TYPE                 = 9006,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_VALUE                = 9007,
    ATP_CWMP_RPC_CPE_ATMPT_SET_NON_WRITE_PARAM          = 9008,
    ATP_CWMP_RPC_CPE_NOTFY_REQ_REJ                      = 9009,
    ATP_CWMP_RPC_CPE_DOWNLOAD_FAIL                      = 9010,
    ATP_CWMP_RPC_CPE_UPLOAD_FAIL                        = 9011,
    ATP_CWMP_RPC_CPE_FILE_TRANF_AUTH_FAIL               = 9012,
    ATP_CWMP_RPC_CPE_UNSPTED_PROT_FOR_FILE_TRAN         = 9013,

    // Newly added error codes in Amendment 2
    ATP_CWMP_RPC_CPE_FAIL_JOIN_GRP                      = 9014,
    ATP_CWMP_RPC_CPE_FAIL_CONTACT_SERVER                = 9015,
    ATP_CWMP_RPC_CPE_FAIL_ACCESS_FILE                   = 9016,
    ATP_CWMP_RPC_CPE_FAIL_COMPLETE_TRANS                = 9017,
    ATP_CWMP_RPC_CPE_FILE_CORRUPTED                     = 9018,
    ATP_CWMP_RPC_CPE_FAIL_AUTHENTICATION                = 9019,

    ATP_CWMP_RPC_CPE_TRANSFER_FAILED                 = 9020,
    ATP_CWMP_RPC_CPE_CANCELTRANS_FORBIDDEN               = 9021,
    ATP_CWMP_RPC_CPE_SCHEDOWNLOAD_END                   = 9024,
    ATP_CWMP_RPC_CPE_WINDOWTIMENUM_NOTSUPPORT           = 9025,
    ATP_CWMP_RPC_CPE_INVALID_SCHEMA                     = 9037,     // Extended
    ATP_CWMP_RPC_CPE_MAX_MASK                           = 0XFFFF
} ATP_CWMP_CPE_FAULT_CODES_EN;

const VOS_CHAR *ATP_CWMP_GetErrStringByCode(VOS_UINT32 ulErrCode);

/******************************************************************************
  函数名称  : ATP_CWMP_BuildErrorNode
  功能描述  : 构造SOAP错误DOM树，格式如下:
      <soap:Fault>
          <faultcode>Server</faultcode>
          <faultstring>CWMP fault</faultstring>
          <detail>
              <cwmp:Fault>
                  <FaultCode>8003</FaultCode>
                  <FaultString>Invalid arguments</FaultString>
              </cwmp:Fault>
          </detail>
      </soap:Fault>
  输入参数  :
              1. ulErrCode  :   错误码
  调用函数  :
  被调函数  :
  输出?问?  :
              1. phError    :   存放构造的DOM树的指针
  返 回 值  :
              VOS_OK
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_BuildErrorNode(
                                   NODEPTR                        *phError,
                                   VOS_UINT32                     ulErrCode);

/******************************************************************************
  函数名称  : ATP_CWMP_ErrorAddPara
  功能描述  : 往构造好的SOAP错误DOM树中添加错误参数
            <cwmp:Fault>
                      <FaultCode>9003</FaultCode>
                      <FaultString>Invalid arguments</FaultString>
                      <SetParameterValuesFault>
                          <ParameterName>
                              InternetGatewayDevice.Time.LocalTimeZone
                          </ParameterName>
                          <FaultCode>9012</FaultCode>
                          <FaultString>...</FaultString>
                      </SetParameterValuesFault>
             _________<SetParameterValuesFault>
            |             <ParameterName>
          添|  pcErrPara:     InternetGatewayDevice.Time.LocalTimeZoneName
          加|             </ParameterName>
          部|  ulErrCode: <FaultCode>9012</FaultCode>
          分|             <FaultString>...</FaultString>
            |_________</SetParameterValuesFault>
                  </cwmp:Fault>
          </detail>
          </soap:Fault>

  输入参数  :
              1. hError     :   已经构造好的错误DOM树
              2. pcErrPara  :   错误参数，不能为空，不要超过256字节
              3. ulErrCode  :   错误码
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_ErrorAddPara(NODEPTR             hError,
                            const VOS_CHAR           *pcErrPara,
                            VOS_UINT32               ulErrCode);

typedef enum tagCWMP_NEXT_LEVEL_TYPE_EN
{
    CWMP_NEXT_LEVEL_TYPE_NONE,
    CWMP_NEXT_LEVEL_TYPE_STATUS_0,
    CWMP_NEXT_LEVEL_TYPE_STATUS_1,
    CWMP_NEXT_LEVEL_TYPE_PARALIST
} CWMP_NEXT_LEVEL_TYPE_EN;

/******************************************************************************
  函数名称  : ATP_CWMP_CreateRpcTopNode
  功能描述  : 构造外层DOM树，其格式如下:
CWMP_NEXT_LEVEL_TYPE_STATUS_0/CWMP_NEXT_LEVEL_TYPE_STATUS_1:
    <cwmp:SetParameterValuesResponse>       // phTopNode
          <Status>0</Status>                // phNextLevelNode
    </cwmp:SetParameterValuesResponse>

CWMP_NEXT_LEVEL_TYPE_NONE:
    <cwmp:SetParameterAttributesResponse>   // phTopNode
    </cwmp:SetParameterAttributesResponse>

CWMP_NEXT_LEVEL_TYPE_PARALIST:
    <cwmp:GetParameterValuesResponse>       // phTopNode
        <ParameterList>                     // phNextLevelNode
        </ParameterList>
    </cwmp:GetParameterValuesResponse>

  输入参数  :
              1. pcRpcName      :   RPC名称，不能为空，不要超过256字节
              2. bResponse      :   是否为Response
              3. ulNextLevelType:   构造类型，见描述
  调用函数  :
  被调函数  :
  输出参数  :
              1. phTopNode      :   存放外层DOM树指针的指针，不能为空
              2. phNextLevelNode:   存放内层DOM树指针的指针，可以为空
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_CreateRpcTopNode(
                        const VOS_CHAR                  *pcRpcName,
                        VOS_BOOL                        bResponse,
                        VOS_UINT32                      ulNextLevelType,
                        NODEPTR                         *phTopNode,
                        NODEPTR                         *phNextLevelNode);

/******************************************************************************
  函数名称  : ATP_CWMP_SetArrayTypeAttr
  功能描述  : 设置ParameterList属性
    <cwmp:GetParameterValuesResponse>
                      |pcTypeNs|                             |-ulArrayNum-|
        <ParameterList soapenc:arrayType="cwmp:ParameterValueStruct[1]">
                       |------添加部分---------|                  |---|
            <ParameterValueStruct>             |----pcArrayType---|
                <Name>InternetGatewayDevice.NumberOfWANDevices</Name>
                <Value></Value>
            </ParameterValueStruct>
        </ParameterList>
    </cwmp:GetParameterValuesResponse>

  输入参数  :
              1. hParaList      :   已经构造好的ParameterList树DOM指针
              2. pcTypeNs       :   ParameterList中每个参数类型的名字空间，
                                    不要超过256字节
              3. pcArrayType    :   ParameterList中每个参数的类型
              4. ulArrayNum     :   ParameterList中参数的个数
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetArrayTypeAttr(
                        NODEPTR                         hParaList,
                        const VOS_CHAR                  *pcTypeNs,
                        const VOS_CHAR                  *pcArrayType,
                        VOS_UINT32                      ulArrayNum);

/******************************************************************************
  函数名称  : ATP_CWMP_AppendParaInfoNode
  功能描述  : 往ParameterList中添加参数
    <cwmp:GetParameterNamesResponse>
        <ParameterList SOAP-ENC:arrayType="cwmp:ParameterInfoStruct[1]">
        _
     添 |   <ParameterInfoStruct>
     加 |       <Name>InternetGatewayDevice.WANDevice</Name>    // pcParaName
     部 |       <Writable>0</Writable>                          // bWritable
     分 |   </ParameterInfoStruct>
        -
        </ParameterList>
    </cwmp:GetParameterNamesResponse>

  输入参数  :
              1. hParaList  :   已经构造好的ParameterList树DOM指针
              2. pcParaName :   待添加参数的名称，不能为空，不要超过256字节
              3. bWritable  :   待添加参数是否可写
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_AppendParaInfoNode(
                                        NODEPTR         hParaList,
                                        const VOS_CHAR  *pcParaName,
                                        VOS_BOOL        bWritable);

/******************************************************************************
  函数名称  : ATP_CWMP_AppendParaValueStructNode
  功能描述  : 添加ParameterValueStruct结构
  <cwmp:GetParameterValuesResponse>
      <ParameterList soapenc:arrayType="cwmp:ParameterValueStruct[1]">
      _
   添 | <ParameterValueStruct>
   加 |   <Name>InternetGatewayDevice.NumberOfWANDevices</Name> // pcParaName
   部 |   <Value>1</Value>                                      // pcParaValue
   分 | </ParameterValueStruct>
      -
      </ParameterList>
  </cwmp:GetParameterValuesResponse>

  输入参数  :
              1. hParaList      :   已经构造好的ParameterList树DOM指针
              2. pcParaName     :   待添加的参数名称，不能为空，不要超过256字节
              3. pcParaValue    :   待添加的参数值
              4. enValueType    :   参数值的类型
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_AppendParaValueStructNode(
                                NODEPTR                     hParaList,
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E   enValueType);

/******************************************************************************
  函数名称  : ATP_CWMP_BuildTransCompleteNodes
  功能描述  : 创建TransferComplete DOM树
    <cwmp:TransferComplete>
        <CommandKey></CommandKey>           // pcCmdKey
        <FaultStruct>
            <FaultCode></FaultCode>         // ulErrCode
            <FaultString></FaultString>
        </FaultStruct>
        <StartTime></StartTime>             // pcStartTime
        <CompleteTime></CompleteTime>       // pcEndTime
    </cwmp:TransferComplete>

  输入参数  :
              1. pcCmdKey       :   RPC的CommandKey
              2. pcStartTime    :   传输开始时间，可以为空
                                    为空时，使用默认时间"0001-01-01T00:00:00Z"
              3. pcEndTime      :   传输结束时间，可以为空
                                    为空时，使用默认时间"0001-01-01T00:00:00Z"
              4. ulErrCode      :   传输错误码
  调用函数  :
  被调函数  :
  输出参数  :
              1. ppcRpcStr       :   构造好的RPC字符串
  返 回 值  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML组件错误码

  修改历史      :
   1.日    期   : 2007-12-30
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CWMP_BuildTransCompleteRpc(
                                    const VOS_CHAR  *pcCmdKey,
                                    const VOS_CHAR  *pcStartTime,
                                    const VOS_CHAR  *pcEndTime,
                                    VOS_UINT32      ulErrCode,
                                    VOS_CHAR        **ppcRpcStr);

/*
 *  CWMP 协议栈内置RPC方法
 */

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not support)
 *      9002 Internal Error
 */
VOS_UINT32 ATP_CWMP_RpcGetRpcMethods(
                                    ATP_CWMP_STK_ST *pstCwmp,
                                    NODEPTR         hRpcRequest,
                                    NODEPTR         *phRpcResponse);

/*
 *  CWMP Adapt interfaces
 */
VOS_UINT32 ATP_CWMP_AdaptInitPara(ATP_CWMP_STK_ST *pstCwmp);

#define ATP_CWMP_PARAMETERKEY_MAX_LEN               (32)
VOS_VOID ATP_CWMP_AdaptSetParameterKey(ATP_CWMP_STK_ST *pstCwmp, const VOS_CHAR *pcParaKey);

const VOS_CHAR *ATP_CWMP_AdaptGetParameterKey(ATP_CWMP_STK_ST *pstCwmp);

VOS_UINT32 ATP_CWMP_AdaptReboot(VOS_UINT32 ulErrCode, VOS_VOID *pvArg);

VOS_UINT32 ATP_CWMP_AdaptFactoryReset(VOS_UINT32 ulErrCode, VOS_VOID *pvArg);

const struct addrinfo *ATP_CWMP_AdaptGetTransIp(const ATP_CWMP_STK_ST *pstCwmp);

typedef VOS_UINT32 (*PFCwmpSetAuthTypeFunc)(VOS_UINT32          ulAuthMode); 
VOS_VOID  ATP_UTIL_RegCwmpSetAuthTypeFunc(PFCwmpSetAuthTypeFunc pfFunc);

typedef VOS_UINT32 (*PFATPCWMPFirstStartUp)(ATP_CWMP_STK_ST *pstCwmp, VOS_VOID *pvHook); 
extern PFATPCWMPFirstStartUp  g_pfCwmpFirstStartUp;

const VOS_CHAR * ATP_CWMP_GetVersion(void);


//#define ATP_DEBUG 1
#ifdef ATP_DEBUG
#define ATP_CWMP_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_CWMP_DEBUG(x...)
#endif

#define ATP_CWMP_TRACE      ATP_MSG_Printf

#endif

