#ifndef __CWMP_APPAPI_H__
#define __CWMP_APPAPI_H__

#include "cwmpstkapi.h"

typedef enum tagATP_CWMP_ERR_CODE
{
    ATP_ERR_CWMP_START              = (ATP_ERR_CWMP_STK_START + 0x800),
    ATP_ERR_CWMP_PARA_INVALID,
    ATP_ERR_CWMP_MEM_NOT_ENOUGH,
    ATP_ERR_CWMP_SYSTEM_CALL,
    ATP_ERR_CWMP_SCHEMA_RPC,
    ATP_ERR_CWMP_CFM_LOCKED,
    ATP_ERR_CWMP_PRDTIME_VALID,
    ATP_ERR_CWMP_END
} ATP_CWMP_ERR_CODE_EN;

//typedef enum tagATP_CWMP
VOS_UINT32 ATP_CWMP_GetInformParameters(ATP_CWMP_STK_ST *pstCwmp);
VOS_UINT32 ATP_CWMP_GetDeviceID(ATP_CWMP_STK_ST *pstCwmp);

VOS_UINT32 ATP_CWMP_RpcErrCodeTransform(VOS_UINT32 ulErrCode);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9005 Invalid Name
 */
VOS_UINT32 ATP_CWMP_RpcGetParaNames(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args (Not support)
 *      9004 Resource Exceeds (Not possible)
 *      9005 Invalid Name
 */
VOS_UINT32 ATP_CWMP_RpcGetParaValues(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9004 Resource Exceeds (Not possible)
 *      9005 Invalid Name
 *      9006 Invalid Type
 *      9007 Invalid Value
 *      9008 Set non-writable parameter
 */
VOS_UINT32 ATP_CWMP_RpcSetParaValues(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9004 Resource Exceeds (Not possible)
 *      9005 Invalid Name
 */
VOS_UINT32 ATP_CWMP_RpcGetParaAttributes(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9004 Resource Exceeds (Not possible)
 *      9005 Invalid Name
 *      9009 Notification request rejected
 */
VOS_UINT32 ATP_CWMP_RpcSetParaAttributes(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9004 Resource Exceeds
 *      9005 Invalid Name
 */
VOS_UINT32 ATP_CWMP_RpcAddObject(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9005 Invalid Name
 */
VOS_UINT32 ATP_CWMP_RpcDeleteObject(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9000 Method not supported (Not possible)
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9005 Invalid Name
 *      9010 Download failure (File too large)
 *      9012 Authentication Failure
 *      9013 Unsupported protocol
 */
VOS_UINT32 ATP_CWMP_RpcDownload(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9000 Method not supported (Not possible)
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 *      9005 Invalid Name
 *      9011 Upload failure
 *      9012 Authentication Failure
 *      9013 Unsupported protocol
 */
VOS_UINT32 ATP_CWMP_RpcUpload(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);


/*
 *  Allowed CWMP Error Code:
 *      9000 Method not supported (Not possible)
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args
 */
VOS_UINT32 ATP_CWMP_RpcReboot(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  Allowed CWMP Error Code:
 *      9001 Request Denied (Not possible)
 *      9002 Internal Error
 *      9003 Invalid Args (Not possible)
 */
VOS_UINT32 ATP_CWMP_RpcFactoryReset(
                            ATP_CWMP_STK_ST *pstCwmp,
                            NODEPTR         hRpcRequest,
                            NODEPTR         *phRpcResponse);

/*
 *  File transfer direction
 */
typedef enum tagATP_CWMP_TRANS_DIR_EN
{
	 ATP_CWMP_TRANS_DIR_UPLOAD,
	 ATP_CWMP_TRANS_DIR_DOWNLOAD,
} ATP_CWMP_TRANS_DIR_EN;

#ifdef SUPPORT_ATP_CWMP_CONTROL_OSGI
typedef enum tagATP_CWMP_TRANS_DU_OPT_EN
{
	 ATP_CWMP_TRANS_DU_INSTALL,
	 ATP_CWMP_TRANS_DU_UPDATE,
	 ATP_CWMP_TRANS_DU_UNINSTALL,
} ATP_CWMP_TRANS_DU_OPT_EN;

#define ATP_CWMP_URL_MAX_LEN_D              (1024)
#define ATP_CWMP_UUID_MAX_LEN_D             (36)
#define ATP_CWMP_VERSION_MAX_LEN_D          (32)
#define ATP_CWMP_COMMON_MAX_LEN_D           (256)
#define ATP_CWMP_DU_OPTION_MAX_D            (20)
#endif
#define ATP_CWMP_CMDKEY_MAX_LEN_D           (32)
#define ATP_CWMP_TIME_MAX_LEN_D             (32)

/*
 *  File transfer msg
 */
typedef struct tagATP_CWMP_TRANS_MSG_ST
{
    VOS_UINT32          ulErrCode;
    VOS_UINT32          ulEvtHdl;
    VOS_UINT32          ulNeedReboot;
    VOS_CHAR            acCmdKey[ATP_CWMP_CMDKEY_MAX_LEN_D + 4];
    VOS_CHAR            acStartTime[ATP_CWMP_TIME_MAX_LEN_D];
    VOS_CHAR            acEndTime[ATP_CWMP_TIME_MAX_LEN_D];
} ATP_CWMP_TRANS_MSG_ST;

typedef struct tagATP_CWMP_REBOOT_STRUCT_ST
{
    ATP_CWMP_STK_ST  *pstCwmp;
    VOS_UINT32             ulEvtHdl;
    VOS_CHAR               acCmdKey[ATP_CWMP_CMDKEY_MAX_LEN_D + 4];
} ATP_CWMP_REBOOT_STRUCT_ST;

#ifdef SUPPORT_ATP_CWMP_MOUNT_DM

VOS_BOOL ATP_CWMP_EndParseHook(VOS_UINT32 ulOID, const VOS_CHAR *pcEndBuf);
VOS_UINT32 UpnpDmConvertError(VOS_UINT32 ulUnpnError);
// For attaching externel data model
typedef VOS_UINT32 (* PFCwmpGetIdentityProc)(VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH]);

typedef VOS_UINT32 (* PFCwmpGetParaNamesProc)(VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], const VOS_CHAR *pcName,
                                                 VOS_BOOL bNextLevel, NODEPTR hRpcResponse, VOS_UINT32 *pulCnt);

typedef VOS_UINT32 (* PFCwmpGetParaValuesProc)(VOS_VOID *pvDmMsg, NODEPTR hRpcResponse, VOS_UINT32 *pulCnt);
typedef VOS_UINT32 (* PFCwmpGetParaAttributesProc)(VOS_VOID *pvDmMsg, NODEPTR hRpcResponse, VOS_UINT32 *pulCnt);
typedef VOS_UINT32 (* PFCwmpSetParaValuesProc)(VOS_VOID *pvDmMsg, VOS_BOOL *pbStatus);
typedef VOS_UINT32 (* PFCwmpSetParaAttributesProc)(VOS_VOID *pvDmMsg);
typedef VOS_UINT32 (* PFCwmpAddObjectProc)(VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], const VOS_CHAR *pcName, VOS_UINT32 *pulInstanceID, VOS_BOOL *pbStatus);
typedef VOS_UINT32 (* PFCwmpDeleteObjectProc)(VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], const VOS_CHAR *pcName, VOS_BOOL *pbStatus);

typedef struct tagATP_CWMP_ATTACHED_DM_ST
{
    VOS_UINT32                      ulAttachCMO;
    const VOS_CHAR                  *pcMountAppName;

    PFCwmpGetIdentityProc           pfGetId;

    PFCwmpGetParaNamesProc          pfGetNames;
    PFCwmpGetParaValuesProc         pfGetValues;
    PFCwmpGetParaAttributesProc     pfGetAttrs;
    PFCwmpSetParaValuesProc         pfSetValues;
    PFCwmpSetParaAttributesProc     pfSetAttrs;
    PFCwmpAddObjectProc             pfAddObj;
    PFCwmpDeleteObjectProc          pfDelObj;
} ATP_CWMP_ATTACHED_DM_ST;

extern const ATP_CWMP_ATTACHED_DM_ST g_astAttachedDms[];
extern const VOS_UINT8 g_ucNumOfAttachedDms;

extern const ATP_CWMP_ATTACHED_DM_ST *g_pstCurDm;
extern VOS_CHAR g_acAttachedParaName[];

#endif  // End of SUPPORT_ATP_CWMP_MOUNT_DM

extern VOS_BOOL g_bSingleSystem;

const VOS_CHAR * ATP_CWMPAPP_GetVersion(void);
#endif


