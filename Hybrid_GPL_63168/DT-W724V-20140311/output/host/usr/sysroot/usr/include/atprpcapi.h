/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 ******************************************************************************/
/**\file upnprpc.c
 *
 *\copyright 2007-2020, HUAWEI
 *\author Hybrid Maintain
 *\date  2013-12-18
 */

#ifndef __ATP_RPC_API_H_INCLUDE__
#define __ATP_RPC_API_H_INCLUDE__

/*
 *  RPC Error Codes
 */
typedef enum tagATP_RPC_FAULT_CODES_EN
{
    RPC_RETCODE_OK = 0,
    RPC_ERR_METHOD_NOT_SUPPORT = 9000,
    RPC_ERR_REQ_DENIED,                 // 9001
    RPC_ERR_INTER_ERROR,                // 9002
    RPC_ERR_INVALID_ARG,                // 9003
    RPC_ERR_RES_EXCD,                   // 9004
    RPC_ERR_INVALID_PARA_NAME,          // 9005
    RPC_ERR_INVALID_PARA_TYPE,          // 9006
    RPC_ERR_INVALID_PARA_VALUE,         // 9007
    RPC_ERR_NON_WR_PARA,                // 9008
    RPC_ERR_NOTIFY_REQ_REJ,             // 9009
    RPC_ERR_FILE_TRANS_FAIL,            // 9010
    RPC_ERR_UPLOAD_FAIL,                // 9011
    RPC_ERR_FILE_TRANS_AUTH_FAIL,       // 9012
    RPC_ERR_UNSUPPORTED_PROTOCOL,       // 9013

    // Newly added error codes in Amendment 2
    RPC_ERR_FAIL_JOIN_GRP,              // 9014
    RPC_ERR_FAIL_CONTACT_SVR,           // 9015
    RPC_ERR_FAIL_ACCESS_FILE,           // 9016
    RPC_ERR_FAIL_COMPLETE_TRANS,        // 9017
    RPC_ERR_FILE_CORRUPTED,             // 9018
    RPC_ERR_AUTH_FAIL,                  // 9019

    RPC_ERR_TRANSFER_FAILED,            // 9020
    RPC_ERR_CANCELTRANS_FORBIDDEN,      // 9021
    RPC_ERR_SCHEDOWNLOAD_END = 9024,    // 9024
    RPC_ERR_WINDOWTIMENUM_NOTSUPPORT,   // 9025
    RPC_ERR_INVALID_SCHEMA = 9037,      // Extended
    RPC_ERR_REQ_AUTH_FAIL,
    RPC_ERR_REQ_NEEDED_SSL,
    RPC_ERR_REQ_WEBUI_ACTIVED = 9801,   // UI configuration session is active
    RPC_ERR_REQ_TR069_ACTIVED = 9802,   // TR-069 configuration session is active 
    RPC_ERR_REQ_TR064_ACTIVED = 9803,   // TOTR 064v2 configuration call of TR-064-I2 is active
    RPC_ERR_MAX_MASK = 0XFFFF
} ATP_RPC_FAULT_CODES_EN;

/*
 *  RPCAPI error code
 */
typedef enum tagATP_RPCAPI_ERR_CODE_EN
{
    ATP_RPCAPI_ERR_START = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_RPCSDK),
    ATP_RPCAPI_ERR_PARA,
    ATP_RPCAPI_ERR_MEM,
    ATP_RPCAPI_ERR_SOAP_XML,
    ATP_RPCAPI_ERR_RPC_NAME,
    ATP_RPCAPI_ERR_SCHEMA_RPC,
    ATP_RPCAPI_ERR_AUTH,
    ATP_RPCAPI_ERR_LOCK,
    ATP_RPCAPI_ERR_END
} ATP_RPCAPI_ERR_CODE_EN;

// RPC callback function.
typedef VOS_UINT32 (*PFATPRpcMethod)(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);

typedef enum tagRPC_ATTR_MASK_EN
{
    RPC_ATTR_MASK   = 0,
    RPC_ATTR_AUTH   = (1 << 0),
    RPC_ATTR_HTTPS  = (1 << 1),
    RPC_ATTR_LOCK   = (1 << 2)
} RPC_ATTR_MASK_EN;
#define RPC_NEEDS_AUTH(attr)    ((attr) & RPC_ATTR_AUTH)
#define RPC_NEEDS_HTTPS(attr)   ((attr) & RPC_ATTR_HTTPS)
#define RPC_NEEDS_LOCK(attr)    ((attr) & RPC_ATTR_LOCK)

// RPC struct.
typedef struct tagATP_RPC_PROC_ST
{
    VOS_CHAR        *pcRPCName;
    PFATPRpcMethod  pfRpcMethod;
    VOS_UINT32      rpcAttr;
} ATP_RPC_PROC_ST;

extern const ATP_RPC_PROC_ST g_astCpeRpcs[]; // RPC array.
extern const VOS_UINT8 g_ucNumOfCpeRpcs; // RPC array length.

extern PFATPRpcMethod ATP_UTIL_GetRpcProc(const VOS_CHAR *pstRpcName);
extern VOS_UINT32 ATP_UTIL_GetRpcAttr(const VOS_CHAR *pstRpcName);

extern VOS_UINT32 ATP_UTIL_RpcGetRpcMethods(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcSetParaVals(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcGetParaVals(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcGetParaNames(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcAddObj(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcDelObj(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcReboot(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcFactoryReset(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcDownload(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);
extern VOS_UINT32 ATP_UTIL_RpcUpload(NODEPTR hRpcRequest, NODEPTR *phRpcResponse, VOS_VOID *pvHook);

// SOAP name space.
typedef struct tagATP_SOAP_NAME_SAPCE_ST
{
    const VOS_CHAR *acSoapEnv;
    const VOS_CHAR *acSoapEnc;
    const VOS_CHAR *acSoapXsd;
    const VOS_CHAR *acSoapXsi;
    const VOS_CHAR *acExtend;
} ATP_SOAP_NAME_SAPCE_ST;

extern const ATP_SOAP_NAME_SAPCE_ST g_stNameSpace; // SOAP name space.

// API For rpcerr.c

extern const VOS_CHAR *ATP_RPC_GetErrStringByCode(VOS_UINT32 ulErrCode);

extern VOS_UINT32 ATP_RPC_ErrorAddPara(NODEPTR hError,
    const VOS_CHAR *pcErrPara, VOS_UINT32 ulErrCode);

extern VOS_UINT32 ATP_RPC_ErrCodeTransform(VOS_UINT32 ulErrCode);

extern VOS_UINT32 ATP_RPC_BuildErrorNode(NODEPTR *phError, VOS_UINT32 ulErrCode);

#ifdef ATP_DEBUG
#define ATP_TR064I2_DEBUG(format, args...) printf("++++++TR064I2 DEBUG++++++[file:%s, line:%d]", __FILE__, __LINE__); printf(format, ##args); printf("\n");
#else
#define ATP_TR064I2_DEBUG(format, args...)
#endif

// API for rpcsdk.c

extern VOS_UINT32 ATP_UTIL_ParseSoapEnvelope(
        NODEPTR hEnvelope,          // In
        NODEPTR *phBody,            // Out
        const VOS_CHAR **ppcSoapID, // Out
        VOS_BOOL *pbHoldRequest);   // Out


typedef VOS_UINT32 (*PFRpcSessCtrlFunc)(VOS_VOID *pvArg);
typedef struct tagRpcSessCtrlFunc
{
    PFRpcSessCtrlFunc pfSessCtrlFunc;
    VOS_VOID *pvArg;
} RPC_SESS_CTRL_FUNC;

extern RPC_SESS_CTRL_FUNC g_sessCtrlFunc;
extern VOS_UINT32 ATP_RPC_RegEndSessFunc(PFRpcSessCtrlFunc pfFunc, VOS_VOID *pvArg);

#endif