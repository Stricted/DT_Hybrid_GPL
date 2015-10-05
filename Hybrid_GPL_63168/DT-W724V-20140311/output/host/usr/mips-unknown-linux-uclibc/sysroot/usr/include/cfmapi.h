/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file cfmapi.h
  *CFM library interface \n
  *     CFM库共包含两个大部分\n
  *     数据库接口: DB操作接口用于访问数据库，每个链接cfmapi库的进程调用ATP_DBInit后都可以访问数据库。\n
  *     CFMA接口:   CFMA接口用于为配置工具提供统一的配置管理接口，主要供web、upnp及cwmp调用。
  *\author z65940
  *\date  2011-12-17
*/

#ifndef __CFM_API_H__
#define __CFM_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/******************************************************************************/
/*************/


#include "cfmadaptapi.h"
#include "httpapi.h"

//! CFM数据库多实例对象最深的层次
#define ATP_CFM_MAX_TREE_DEPTH  (6)
#define ATP_CFM_MSG_RETCODE_BASE    (1024)
#define ATP_CFM_INSTNUM_FAKE_OID    (0xFFFFFFFE)

#define ATP_CFM_CFG_ROOTNAME    "InternetGatewayDeviceConfig"
#define ATP_CFM_CFG_ENDTAG        "</InternetGatewayDeviceConfig>"

#define ATP_CFM_CFGFILE_HEAD    "<?xml version=\"1.0\" ?>"
#define ATP_CFM_CFG_EXTENTIONNAME  "ATPExtentionConfig"

#define ATP_CFM_MSG_BASE    ATP_MSG_CATEGORY_DEF(ATP_MODULE_CFM)

#ifdef  SUPPORT_ATP_TR181
#define ATP_ALIAS_LEN       64
#define CWMP_MS_INSTANCEMODE_D "ManagementServer.InstanceMode"
#endif

//! CFM debug function
/*! 通过menuconfig来选择使能debug模式
  *  或者通过make cfmcore ATP_DEBUG=1来使能
  */
#ifdef ATP_DEBUG
#define CFM_DEBUG(format, args...)  {printf("CFM DEBUG============file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define CFM_DEBUG(format, args...)
#endif

//! CFM assert function, disabled when debug
//#define ATP_CFM_SAFE_RUN 1
#if defined(ATP_CFM_SAFE_RUN) && !defined(ATP_PCLINT)
#define cfmAssert(x, format, args...)   \
    if (!(x))\
    {\
        printf("CFM ASSERT>>>>>>>>file:%s, line:%d ", __FILE__, __LINE__);\
        printf(format, ##args);\
        printf("\n");\
    }
#define cfmAssertReturn(x, retval, format, args...) \
    if (!(x))\
    {\
        printf("CFM ASSERT>>>>>>>>file:%s, line:%d ", __FILE__, __LINE__);\
        printf(format, ##args);\
        printf("\n");\
        return retval;\
    }
#else
#define cfmAssert(x...)
#define cfmAssertReturn(x...)
#endif

typedef enum tagATP_E_CFGFILE_TYPE
{
    ATP_E_CFGFILE_FULLFILE,
    ATP_E_CFGFILE_PARTFILE,
    ATP_E_CFGFILE_OUTERFILE,
}ATP_E_CFGFILE_TYPE;

//!\brief CFM error code
/*!
 *    CFM Core的错误码
 */
typedef enum tagATP_E_ERR_CFM
{
    /*CFM组件的错误码*/
    ATP_E_ERR_CFM_PARA_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CFM),
    ATP_E_ERR_CFM_NOT_INIT,/*!< 没有初始化 */
    ATP_E_ERR_CFM_OPEN_FILE,
    ATP_E_ERR_CFM_FILE_INVALID,
    ATP_E_ERR_CFM_CREATE_SHM_FAILED,
    ATP_E_ERR_CFM_LOAD_SHM_FAILED,
    ATP_E_ERR_CFM_SHM_NOT_ATTACH,
    ATP_E_ERR_CFM_INTERNAL,
    ATP_E_ERR_CFM_CFGFILE_MATCH,
    ATP_E_ERR_CFM_MEM,
    ATP_E_ERR_CFM_OID_ERR,
    ATP_E_ERR_CFM_KEYLIST_OR_OID,   //keylist或者oid无效
    ATP_E_ERR_CFM_PARANUM_ZERO,
    ATP_E_ERR_CFM_PARA_ERR_OR_INST_ZERO,  //参数无效或者实例个数为0
    ATP_E_ERR_CFM_BUILD_CFG_FILE, 
    ATP_E_ERR_CFM_SEM_CREATE,
    ATP_E_ERR_CFM_MSG_CREATE,
    ATP_E_ERR_CFM_REG_INFO,
    ATP_E_ERR_CFM_MSG_TYPE,
    ATP_E_ERR_CFM_MSG_OPCODE,
    ATP_E_ERR_CFM_CFG_TOOL_TYPE,
    ATP_E_ERR_CFM_MSG_ITEM,
    ATP_E_ERR_CFM_MSG_RESP,
    ATP_E_ERR_CFM_MSG_GETRESP,
    ATP_E_ERR_CFM_MAINPROC_DIFF,
    ATP_E_ERR_CFM_LIB_LAOD,
    ATP_E_ERR_CFM_TOO_MANY_INSTS,
    ATP_E_ERR_CFM_INVALID_INST_NUM,
    ATP_E_ERR_CFM_DOMAIN_STRING,
    ATP_E_ERR_CFM_STRING_SHORT,
    ATP_E_ERR_CFM_OBJ_DIFF,
    ATP_E_ERR_CFM_ACCESS_DENY,
    ATP_E_ERR_CFM_NOTIFY_REJECTED,
    ATP_E_ERR_CFM_NOT_FOUND_KEY,

// for tr181 alias parameter
    ATP_E_ERR_CFM_ALIAS_NOT_FOUND,
    ATP_E_ERR_CFM_NOT_ALIAS_MODE,    
    ATP_E_ERR_CFM_OBJ_HAVE_NO_ALIAS_PARA,

    
    /* 消息响应码 */
    ATP_E_ERR_CFM_RETMSG_BGN = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CFM) + ATP_CFM_MSG_RETCODE_BASE,
    ATP_E_ERR_CFM_RETMSG_DONE,    
    ATP_E_ERR_CFM_NO_VISIT_DB,
    ATP_E_ERR_CFM_NEED_REBOOT,
    ATP_E_ERR_CFM_NO_VISIT_ANY,  //不需要保存FLASH和共享内存
    ATP_E_ERR_CFM_RETMSG_END,
    
    ATP_E_ERR_CFM_MAX
}ATP_E_ERR_CFM;

//!\brief CFM KeyID define
/*!
 *
 */
enum ATP_CFM_KEYID_EN
{
    ATP_CFM_KEY_EXPORT = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_CFM), /*!< cfg file key */
    ATP_CFM_KEY_FLASH,
    ATP_CFM_KEY_PARAM,
    ATP_CFM_KEY_CFGSIGN,
    ATP_CFM_KEY_DEFAULTCFG,
};


/*********************************数据结构*************************************/

// 配置工具类型值必须和ATP配置项生成工具中的值一致(ref: ATP V100R003C01 配置项.xls)
typedef enum tagATP_CFM_E_IF_TYPE
{
    ATP_CFM_E_IF_TYPE_BGN           = 0,
    ATP_CFM_E_IF_TYPE_CWMP_ADMIN    = 1 << 0,
    ATP_CFM_E_IF_TYPE_CLI_ADMIN     = 1 << 1,
    ATP_CFM_E_IF_TYPE_WEB_USER      = 1 << 2,
    ATP_CFM_E_IF_TYPE_WEB_ADMIN     = 1 << 3,
    ATP_CFM_E_IF_TYPE_TR064_ADMIN   = 1 << 4,
    ATP_CFM_E_IF_TYPE_CLI_USER      = 1 << 5,

    ATP_CFM_E_IF_TYPE_UPNP_DM       = 1 << 6,

    // Add more config tool types
	
	// halp allow access by tr069 and web
    ATP_CFM_E_IF_TYPE_HALP_CFG= 1 << 7,   
    ATP_CFM_E_IF_TYPE_CMS           = 1 << 14,
    ATP_CFM_E_IF_TYPE_OTHER         = 1 << 15,
    ATP_CFM_E_IF_TYPE_END           = 0xFFFF   /* 最多不能超过16种配置工具 */
} ATP_CFM_E_IF_TYPE;

typedef enum tagATP_CFM_E_OPER_TYPE // 操作码必须和消息类型一致
{   
    ATP_CFM_E_OPER_TYPE_GET = ATP_E_CFGMSG_GET,
    ATP_CFM_E_OPER_TYPE_CHK = ATP_E_CFGMSG_CHK,        
    ATP_CFM_E_OPER_TYPE_SET = ATP_E_CFGMSG_SET,
    ATP_CFM_E_OPER_TYPE_DEL = ATP_E_CFGMSG_DEL,
    ATP_CFM_E_OPER_TYPE_ADD = ATP_E_CFGMSG_ADD,
    ATP_CFM_E_OPER_TYPE_GETINST = ATP_E_CFGMSG_INSTNUM,
    
    ATP_CFM_E_OPER_TYPE_BUTT,
} ATP_CFM_E_OPER_TYPE;

typedef enum tagATP_CFM_VALUETYPE_EN
{
    ATP_CFM_VALUETYPE_UNKNOWN = 0,
    ATP_CFM_VALUETYPE_STRING,
    ATP_CFM_VALUETYPE_NUMBER,
    
    ATP_CFM_VALUETYPE_EXTEND
} ATP_CFM_VALUETYPE_EN;


typedef struct tagATP_CFM_TOOL_NAME_ST
{
    VOS_UINT32          ulCfmLevel;
    VOS_CHAR           *ulCfmToolName;
} ATP_CFM_TOOL_NAME_ST;


/**
 *  CFM main proc app
 */
typedef struct tagATP_CFM_MAINPROC_MAP
{
    const VOS_CHAR *pcMainProc;
    //VOS_UINT32 ulIndex;
    VOS_UINT32 ulIfType;
}ATP_CFM_MAINPROC_MAP;

typedef VOS_UINT32 (*AtpCfmExportProc)(VOS_UINT8 **ppucBuffer, VOS_UINT32 *pulLen);
typedef VOS_UINT32 (*AtpCfmImportProc)(VOS_UINT8 *pucBuffer, VOS_UINT32 ulLen);

typedef struct tagATP_CFM_Export_ST
{
    const VOS_CHAR              *pcName;
    AtpCfmExportProc            pfExport;
    AtpCfmImportProc            pfImport;
}ATP_CFM_Export_ST;

typedef VOS_UINT32 (*AtpCfmExportHook)(ATP_UTIL_STRFILE_ST *pstStrFile);

extern AtpCfmExportHook g_pfCfmExportHook;

//!\brief CFM全局配置文件
/*!
 * 使用配置管理进程的cfm全局结构，该结构的值由ATP_CFM_ExtConfig初始化，进程中可修改该值，
 * 结构成员值修改不影响其他进程。
 */
typedef struct tagATP_CFM_GLOBAL_CONFIG
{
    const VOS_CHAR              *pszMapFile;
    const VOS_CHAR              *pszDBSaveFile;
    const VOS_CHAR              **apcMainProcList;

    VOS_UINT16                  usCfgToolType;
    VOS_UINT16                  usExtendNameCfgTool;
    VOS_UINT32                  ulShareMemSize;  //!!!一般情况设置为128k
    //todo: cache path
} ATP_CFM_GLOBAL_CONFIG;

//!  CFM NumberOfEntries utility
/*!
 * CFM中可以自动进行TR098参数模型中NumberOfEntries的处理
*/
typedef struct tagATP_CFM_ENTRYNUMBER_ST
{
    VOS_UINT32 ulOID;    /*!< ulOID para value = ulAddOID1 obj number + ulAddOID2 obj number */
    VOS_UINT32 ulAddOID1;/*!< test */
    VOS_UINT32 ulAddOID2;/*!< 0 for ignore, not used now */
}ATP_CFM_ENTRYNUMBER_ST;


#ifdef SUPPORT_ATP_CFM_VOICE_NODE_MAP
typedef struct tagATP_CFM_VOICE_PARA_MAP_ST
{
    VOS_UINT32  ulFromOID;
    /* node InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.X_OutgoSuffixInfo.{i}.
     * mapped to X_CommonConfig.OutgoSuffixInfo.{i}. , the last 1 {i} need copy, so ulInstLevel = 1;
     */
    VOS_BOOL    bCopyLastInstNum;
    VOS_UINT32  ulToOID;
    
}ATP_CFM_VOICE_PARA_MAP_ST;
#endif



/********************************类型转换接口函数************************************/
const VOS_VOID *ATP_UTIL_Str2TypedValue(const VOS_CHAR *pszStrVal, VOS_UINT32 ulOID);

const VOS_CHAR *ATP_UTIL_TypedValue2Str(const VOS_VOID*     pvValue, 
                                        VOS_UINT32          ulOID, 
                                        const VOS_CHAR      acDigitBuf[ATP_DIGIT_STR_MAX_LEN_D],
                                        VOS_UINT32          ulDigitLen);


/*********************************全局变量*************************************/
extern ATP_THREAD_SAFE VOS_UINT32 g_ulCfmErr; //用于唯一的错误码记录

extern const VOS_UINT32 *g_aulRecycleOids;

extern VOS_BOOL g_bRestoreFactory; // 是否恢复出厂配置，若恢复出厂需对参数做补偿

/********************************调试类接口************************************/
/******************************************************************************
  函数名称  : ATP_CFM_GetVersion
  功能描述  : 获取API组件版本
  输入参数  : 
              1.  pszVer: 版本信息字符串
              2.  ulVerLen: 版本信息字符串长度
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : VOS_OK/Err Code

  修改历史      :
   1.日    期   : 2008-11-26
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
const VOS_CHAR * ATP_CFM_GetVersion(void);

/********************************CFM管理类接口*********************************/
/******************************************************************************
  函数名称  : ATP_DBMain
  功能描述  : CFM初始化入口(组件入口)
  输入参数  : 
              
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-26
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
#if 0
VOS_UINT32 ATP_DBMain();
#endif

/*!
  \brief 进行数据库的初始化
  \param[in]  enIfType: enIfType 配置工具类型
  \param[in]  bReInit:    是否需要重新初始化共享内存，若是，填VOS_TRUE
  \return 返回ATP_E_ERR_CFM类型的错误码
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_DBInit(ATP_CFM_E_IF_TYPE enIfType, VOS_BOOL bReInit);

/*!
  \brief   CFM共享内存卸载(去初始化)
  \return 返回ATP_E_ERR_CFM类型的错误码
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_DBDestroy(VOS_VOID);

/******************************************************************************
  函数名称  : ATP_DBSave
  功能描述  : 数据库保存flash
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-26
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBSave(VOS_VOID);

/*!
  \brief 把共享内存中的数据库写到Flash中
  \param[in]  pcFileToFlush:    共享内存数据库序列化后临时的xml文件路径
  \return ATP_E_ERR_CFM类型的错误码
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_DBFlush(ATP_UTIL_STRFILE_ST *pstStrFile);

VOS_UINT32 ATP_DBCheckFile(ATP_UTIL_STRFILE_ST *pstStrFile);

#define ATP_CFM_INITKEYLIST {0, 0, 0, 0, 0, 0}

/********************************************************************************
 * 函数名称  : ATP_DBExportPart
 * 功能描述  : 导出部分配置文件
 * 输入参数  : 
 *             1.  ulKeyList[ATP_CFM_MAX_TREE_DEPTH]:
 *             2.  ulOID:
 *             3.  pszExpFile:
 * 调用函数  : 
 * 被调函数  : 
 * 输出参数  : 无
 * 返 回 值  : 无
 *
 * 修改历史      :
 *  1.日    期   : 2008-12-1
 *    作    者   : p42047
 *    修改内容   : 完成初稿
******************************************************************************/
VOS_UINT32 ATP_DBExportCfgOfObj(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                   VOS_UINT32        ulOID, 
                                   ATP_UTIL_STRFILE_ST *pstStrFile);

/******************************************************************************
  函数名称  : ATP_DBExport
  功能描述  : 导出整个配置文件
  输入参数  : 
              1. path:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-1
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBExport(ATP_UTIL_STRFILE_ST *pstStrFile);

/******************************************************************************
  函数名称  : ATP_DBSetCurrUser
  功能描述  : 设置配置当前使用CFM API库的配置工具，该接口可在一个进程中多次调用。
  输入参数  : 
              1.  enIfType:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-28
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBSetCurrUser(ATP_CFM_E_IF_TYPE enIfType);

/******************************************************************************
  函数名称  : ATP_DBGetCurrUser
  功能描述  : 获取当前使用CFM API库的配置工具类型
  输入参数  : 
              
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-1
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBGetCurrUser(VOS_VOID);

/********************************DB操作类接口*********************************/
VOS_UINT32 ATP_DBGetObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulOID,
                        VOS_UINT32 **ppulObj,
                        VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBGetObjByName(const VOS_CHAR *pcObjName,
                        VOS_UINT32 **ppulObj,
                        VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBSetObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                               VOS_UINT32       ulOID,
                               const VOS_UINT32 *pulObj);

VOS_UINT32 ATP_DBSetObjByName(const VOS_CHAR *pcObjName,
                               const VOS_UINT32 *pulObj);

/******************************************************************************
  函数名称  : ATP_DBAddObj
  功能描述  : 添加对象
  输入参数  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  ulOID:
              4.  hParaList:
              5.  ulPreSetFlag:
              6.  pulInstanceId:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿
//是否确实需要预添加
******************************************************************************/
VOS_UINT32 ATP_DBAddObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulOID,
                        const VOS_UINT32 *pulObjValues,
                        VOS_UINT32 *pulInstanceId);

VOS_UINT32 ATP_DBAddObjByName(const VOS_CHAR *pcObjName,
                        const VOS_UINT32 *pulObjValues,
                        VOS_UINT32 *pulInstanceId);

/******************************************************************************
  函数名称  : ATP_DBDelObj
  功能描述  : 
  输入参数  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  ulOID:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBDelObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                               VOS_UINT32 ulOID);

VOS_UINT32 ATP_DBDelObjByName(const VOS_CHAR *pcObjName);

// 获取对象实例数
VOS_UINT32 ATP_DBGetNumOfObjs(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulOID, VOS_UINT32 *pulNum,
                              VOS_UINT32 **ppulInstArray);

VOS_UINT32 ATP_DBGetNumOfObjsByName(const VOS_CHAR *pcObjName, VOS_UINT32 *pulNum,
                              VOS_UINT32 **ppulInstArray);

VOS_UINT32 ATP_DBGetObjectBySeqNum(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulOID,
                                VOS_UINT32 ulSeqNum,
                                VOS_UINT32 **ppulObj,
                                VOS_UINT32 *puInstNo,
                                VOS_UINT32 *pulNumOfParas,
                                VOS_UINT32 ulNewKeyList[ATP_CFM_MAX_TREE_DEPTH]);

/******************************************************************************
  函数名称  : ATP_DBGetPara
  功能描述  : 
  输入参数  : 
              1.  ulKeyList:
              2.  ulOID:
              3.  *pValue:
              4.  ulValueLen:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBGetPara(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulOID,
                                VOS_VOID *pValue,
                                VOS_UINT32 ulValueLen);

/******************************************************************************
  函数名称  : ATP_DBGetPara
  功能描述  : 
  输入参数  : 
              1.  ulKeyList:
              2.  ulOID:
              3.  *pValue:
              4.  ulValueLen:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBGetParaByName(const VOS_CHAR *pcParaName, 
                                VOS_VOID *pValue,
                                VOS_UINT32 ulValueLen);

/******************************************************************************
  函数名称  : ATP_DBSetPara
  功能描述  : 
  输入参数  : 
              1.  enIntfType:
              2.  ulKeyList:
              3.  hParaList:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBSetPara(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                VOS_UINT32 ulParaCMO,
                                const VOS_VOID *pvData);

VOS_UINT32 ATP_DBSetParaByName(const VOS_CHAR *pcPara, const VOS_VOID *pvValue);

VOS_UINT32 ATP_DBDomainToKeyListIgnorePrefix(const VOS_CHAR* pszDomain, 
                                 VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32* pulOID);

VOS_UINT32 ATP_DBDomainToKeyList(const VOS_CHAR* pszDomain, 
                                        VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32* pulOID);

VOS_UINT32 ATP_DBDomainToKeyListEx(const VOS_CHAR* pszDomain, 
                                 VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 *pulOID, VOS_UINT32 *pulKeyListDepth);

#ifdef  SUPPORT_ATP_TR181

typedef    struct tagCFM_ALIAS_PARA_CMO_VALUE_ST
{
    struct tagCFM_ALIAS_PARA_CMO_VALUE_ST        *pstNext;
    VOS_UINT32        ulOID;
    VOS_UINT32        ulLeftSquareBracketsPostion;
    VOS_UINT32        ulRightSquareBracketsPostion;    
    VOS_CHAR        *pcOldValue;
    VOS_CHAR        *pcNewValue;
}CFM_ALIAS_PARA_CMO_VALUE_ST;

typedef struct tagCFM_LIST_ST
{
    struct tagCFM_LIST_ST        *pstNext;

    VOS_UINT32                        ulOID;
    VOS_UINT32                        ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
} CFM_LIST_ST;

typedef struct tagCFM_AUTO_CREATE_INSTANCE_ST
{
    struct tagCFM_AUTO_CREATE_INSTANCE_ST    *pstNext;    
    CFM_LIST_ST    *pstAutoCreatedInst;
}CFM_AUTO_CREATE_INSTANCE_ST;

typedef struct tagCFM_STRONG_REF_PARA_NAME_ST
{
    struct tagCFM_STRONG_REF_PARA_NAME_ST        *pstNext;
    VOS_UINT32    ulOID;
    VOS_CHAR    *pcParaDomainName;

}CFM_STRONG_REF_PARA_NAME_ST;

typedef struct tagDBITERATE_PARA_ST
{
    CFM_STRONG_REF_PARA_NAME_ST        *pstParaList;
} DBITERATE_PARA_ST;


/* ref parameter
*        values: A.B.[cpe-1].,A.C.[cpe-2].D.[cpe-1].
*                            |---> DomainName          |--->DomainName
*                  |--->OID,OldValue          |---->OID,OldValue
*                                                                   |---->OID,OldValue
*/

typedef struct tagCFM_DOMAIN_PARSE_ST
{
    struct tagCFM_DOMAIN_PARSE_ST    *pstNext;
    VOS_CHAR                        *pcDomainName;
    CFM_ALIAS_PARA_CMO_VALUE_ST            *pstAliasParaOV;    
}CFM_DOMAIN_PARSE_ST;


typedef struct tagCFM_REF_PARA_NAME_VALUE_ST
{
    struct tagCFM_REF_PARA_NAME_VALUE_ST *pstNext;
    VOS_CHAR        *pcParaDomainName;
    VOS_CHAR        *pcParaInstName;
    VOS_CHAR        *pcOldParaValues;// A.B.[cpe-1].,A.C.[wan].     length 1024
    CFM_DOMAIN_PARSE_ST            *pstDomainParse;                
}CFM_REF_PARA_NAME_VALUE_ST;

typedef struct tagCFM_REF_PARA_NAME_ST
{
    struct tagCFM_REF_PARA_NAME_ST  *pstNext;
    VOS_CHAR        *pcParaName;        // form ACS
    VOS_CHAR        *pcParaInstName;    // convert to Instance Number Mode
}CFM_REF_PARA_NAME_ST;

// 增加Device.ManagementServer.InstanceMode 取值的宏定义 add by l00163811
#define CFM_INSTANCEMODE_NUM_D                "InstanceNumber"
#define CFM_INSTANCEMODE_ALIAS_D              "InstanceAlias"

#define CWMP_MS_ALIASBASEDADDRESSING_D      "ManagementServer.AliasBasedAddressing"

VOS_UINT32 ATP_DBAliasToInstanceDomain(const VOS_CHAR* pcAliasDomainName, 
										VOS_CHAR *pcInstDomainName, 
										VOS_UINT32 ulLen);
VOS_UINT32 ATP_DBGetAliasParaValueFromPath(const VOS_CHAR *pcAliasDomainName,VOS_CHAR *pcParaAliasValue);    
VOS_BOOL   ATP_AliasIsAvailable(const VOS_CHAR *pszAliasName);
VOS_BOOL   ATP_DBGetInstIdByAliasName(const VOS_CHAR *pszDomainName, const VOS_CHAR *pszAliasName, VOS_UINT32 *pulInstNum);
VOS_UINT32 ATP_DBSetAliasParaByDomainName(const VOS_CHAR *pszDomainName, VOS_UINT32 ulInstID, const VOS_CHAR *pcAliasValue);

VOS_UINT32 ATP_DBSetAliasInAddObj(const VOS_CHAR *pszDomainName, VOS_UINT32 *pulKeyList, VOS_UINT32 ulOID, VOS_UINT32 ulInstID, VOS_UINT32 ulInstDepth);

VOS_UINT32 ATP_DBCreateInstanceIfNotExist(const VOS_CHAR *pszDomainName, CFM_AUTO_CREATE_INSTANCE_ST **pstCreatedInstList);
VOS_UINT32 ATP_DBDelObjAutoAdded(CFM_AUTO_CREATE_INSTANCE_ST *pstAutoCreatedList);
VOS_UINT32 ATP_DBReleaseAutoCreatedList(CFM_AUTO_CREATE_INSTANCE_ST *pstAutoCreatedList);

VOS_UINT32 ATP_DB_ALIAS_IterateStrongRefPara(DBITERATE_PARA_ST     *pstArg);
VOS_UINT32 ATP_DB_ALIAS_ParaAppendList(const VOS_UINT32 *pulKeyList, VOS_UINT32 ulOID, const VOS_CHAR *pcAliasNew);
VOS_UINT32 ATP_DB_ALIAS_FreeAliasParaList();
VOS_UINT32 ATP_DB_ALIAS_CmpAndUpdateRefList(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_ParseParaValue(VOS_CHAR *pcParaValue, CFM_DOMAIN_PARSE_ST **ppstList);
VOS_UINT32 ATP_DB_ALIAS_RefNameListToRefSTList(CFM_STRONG_REF_PARA_NAME_ST *pstRefParaName, CFM_REF_PARA_NAME_VALUE_ST **pstList);
VOS_UINT32 ATP_DB_ALIAS_ShowCVList();
VOS_UINT32 ATP_DB_ALIAS_ShowRefPVList(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_GetNewRefParaValuesAndSave(CFM_REF_PARA_NAME_VALUE_ST *pstRefParaValuesList);
VOS_UINT32 ATP_DB_ALIAS_GetStrongRefParaList();
VOS_UINT32 ATP_DB_ALIAS_SetRefParamValue();
VOS_UINT32 ATP_DBGetDomainNameByAliasMode(const VOS_CHAR *pcInstDomainName, VOS_CHAR *pcAliasDomainName);
VOS_BOOL   ATP_DB_ALIAS_IsAliasBasedAddressing();
VOS_BOOL   ATP_DB_ALIAS_IsAliasMode();
VOS_UINT32 ATP_DB_ALIAS_AliasParaInvalidOrExist(const VOS_CHAR *pcDomainName, const VOS_CHAR *pcValue);
VOS_VOID ATP_DB_ReleaseAutoCreateKO(ATP_UTIL_LIST_ST *pstItem);
VOS_UINT32 ATP_DB_ALIAS_GetDomainNameByAliasMode(const VOS_CHAR *pcDomainNameFromACS, 
                                                 const VOS_CHAR *pcDomainNameInst,
                                                 VOS_CHAR *pcDomainNameDest);
VOS_UINT32 ATP_DB_ALIAS_GetRefValuesByAliasMode(const VOS_CHAR *pcDomainName,
                                                VOS_UINT32 ulOID,
                                                const VOS_CHAR *pcOldValues,
                                                VOS_CHAR *pcNewValues);

VOS_VOID ATP_DB_ALIAS_SetRpcAppendRefList(const VOS_CHAR *pcParaName);
VOS_VOID ATP_DB_ALIAS_SetFlagForACSSet();
VOS_VOID ATP_DB_ALIAS_FreeRefListElement(ATP_UTIL_LIST_ST *pstItem);

#endif

VOS_UINT32 ATP_DBKeyListToDomain(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulOID,
                                        VOS_CHAR* pszDomain,
                                        VOS_UINT32 ulDomainLen);

VOS_UINT32 ATP_DBKeyListToDomainEx(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 ulOID,
                                 VOS_CHAR* pszDomain,
                                 VOS_UINT32 ulDomainLen,
                                 VOS_BOOL  bDecreaseByOne);

VOS_UINT32 ATP_DBGetInstDepthOfOID(VOS_UINT32 ulOID);

VOS_UINT32 ATP_DBObjMove(
                    const VOS_UINT32    ulSourceKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                    VOS_UINT32          ulSourceObj,
                    const VOS_UINT32    ulDestKeyList[ATP_CFM_MAX_TREE_DEPTH],
                    VOS_UINT32          *pulNewObjInstId);

VOS_UINT32 ATP_DBGetParaOIDByParaName(VOS_UINT32 ulObjOID, 
                                             const VOS_CHAR* pszParaName);


typedef struct tagATP_PARA_ATTR_VAL_ST
{
    VOS_UINT16            usAccessBits;    /* 访问列表*/
    VOS_UINT8            ucNotifyType;    /* 参数改变通知 */
    VOS_UINT8            ucReserve;        /* 保留 */
} ATP_PARA_ATTR_VAL_ST;

VOS_UINT32 ATP_CFM_ExtGetAccListItem(const VOS_CHAR *pcAccList);

VOS_UINT32 ATP_CFM_ExtGetAccListStrings(VOS_UINT32 ulAccList, const VOS_CHAR *apcNames[], VOS_UINT32 ulBufLen);
#ifdef SUPPORT_ATP_DT_CONFIGNAME
VOS_UINT32 ATP_CFM_GetExportFileName(VOS_CHAR *pcBuf, VOS_UINT32 ulBufLen);
#endif

/******************************************************************************
  函数名称  : ATP_CFM_SetParaProfile
  功能描述  : 
  输入参数  : 
              1.  ulKeyList:
              2.  hParaList:
              3.  pstParaProfile:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
typedef enum tagATP_CFM_ATTR_CHG_TYPE
{
    ATP_CFM_ATTR_CHG_ACC_LIST = 1,
    ATP_CFM_ATTR_CHG_NOTIFY = 2,
    ATP_CFM_ATTR_CHG_RESERVE = 4
} ATP_CFM_ATTR_CHG_TYPE;
VOS_UINT32 ATP_CFM_SetAttrOfCMO(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                       VOS_UINT32                ulOID,
                                       ATP_CFM_ATTR_CHG_TYPE    enChgType,
                                       const ATP_PARA_ATTR_VAL_ST    *pstParaAttr);

typedef struct tagATP_CFM_ATTR_SET_ST
{
    struct tagATP_CFM_ATTR_SET_ST *     pstNext;
    VOS_UINT32              ulOID;
    VOS_UINT32              ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    ATP_CFM_ATTR_CHG_TYPE   enChgType;
    ATP_PARA_ATTR_VAL_ST    stAttr;
} ATP_CFM_ATTR_SET_ST;

VOS_UINT32 ATP_CFM_SetAttrOfCMOEx(ATP_CFM_ATTR_SET_ST *pstSetAttrList);


/******************************************************************************
  函数名称  : ATP_CFM_GetParaProfile
  功能描述  : 
  输入参数  : 
              1.  hKeyList:
              2.  hParaList:
              3.  pstParaProfile:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFM_GetAttrOfCMO(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                      VOS_UINT32    ulOID,
                                      ATP_PARA_ATTR_VAL_ST *pstParaAttr);

VOS_UINT32 ATP_CFM_SetHideBitsOfCMO(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                                    VOS_UINT32                ulOID,
                                                    VOS_UINT32               ulHideBits);

/*************/

/*****************************配置消息操作接口*********************************/

//ppulSetValues[idx_of_cmo()]

/*
 *    CMS侧消息接口
 */
VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsg(const VOS_VOID *pvMsg, VOS_BOOL bAdd, VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulOldValues,
                                       VOS_UINT32 **ppulSetValues, VOS_UINT32 **ppulChgParas,
                                       VOS_UINT32 *pulNumOfParas, VOS_UINT32 *pulNextObjId);

VOS_UINT32 ATP_CFM_GetObjectFromOtherMsg(const VOS_VOID *pvMsg, VOS_UINT32 aulInstList[ATP_CFM_MAX_TREE_DEPTH],
                                         VOS_UINT32 *pulObjID);

VOS_UINT32 ATP_CFM_GetObjectFromGetMsg(const VOS_VOID *pvMsg, VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulGetValues,
                                       VOS_UINT32 **ppbFocusParas,
                                       VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_CFM_GetObjectFromDelMsg(const VOS_VOID *pvMsg, VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 *pulNextObjId);

VOS_UINT32 ATP_CFM_ResponseInstArrayMsg(const VOS_VOID *pvMsg, 
                                    VOS_UINT32 ulMsgType,
                                    const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                    VOS_UINT32 ulOID, 
                                    const VOS_UINT32 *pulInstArray, 
                                    VOS_UINT32 ulInstNum);

VOS_UINT32 ATP_CFM_ResponseMsgRetCode(VOS_VOID *pvMsg, VOS_UINT32 ulErrOID, VOS_UINT32 ulErrCode);

VOS_UINT32 ATP_CFM_ResponseGetMsg(VOS_VOID *pvMsg, const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                  VOS_UINT32 ulOID, const VOS_UINT32 *pulFocusParas, const VOS_UINT32 *pulGetValues);

VOS_UINT32 ATP_CFM_ResponseSetMsg(VOS_VOID *pvMsg, VOS_VOID *pvRespMsg, VOS_UINT32 ulRespCode);

VOS_UINT32 ATP_CFM_InitResponseMsg(const ATP_MSG_HEADER_ST *pstMsg, VOS_UINT32 ulErrCode, VOS_BOOL *pbSetMsg);

VOS_UINT32 ATP_CFM_AppendErrorCode(const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID,
                                                                      VOS_UINT32 ulErrCode, const VOS_CHAR *pcErrStr);
VOS_UINT32 ATP_CFM_ErrorCode(VOS_UINT32 ulDefaultErrorCode);
VOS_UINT32 ATP_CFM_GetErrorCodeString(VOS_VOID *pstErrortMsg,  VOS_CHAR * pcErrorString, VOS_UINT32 ulStringLen);

VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsgForAll(const VOS_VOID *pvMsg, VOS_BOOL bSet);

VOS_UINT32 ATP_CFM_GetObjectFromSetOrAddMsgEx(VOS_UINT32 ulObjIndex, 
                                       VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                       VOS_UINT32 *pulOID, VOS_UINT32 **ppulOldValues,
                                       VOS_UINT32 **ppulSetValues, VOS_UINT32 **ppulChgParas,
                                       VOS_UINT32 *pulNumOfParas, VOS_UINT32 *pulNextObjId, VOS_BOOL bCheck);

VOS_VOID ATP_CFM_FreeObjList(VOS_VOID);

VOS_VOID ATP_CFM_ChangeInstIdByNow(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH]);

typedef VOS_BOOL (*CfmObjCompareProc)(VOS_UINT32 ulDBObjKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                                                        const VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulDBValues, VOS_VOID *pvHook);

VOS_BOOL ATP_CFM_IsObjectExist(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulObjID,
                const VOS_UINT32 *pulSetValues, const VOS_UINT32 ulInstNum, CfmObjCompareProc pfProc, VOS_VOID *pvHook);

/*
 *    配置工具侧消息接口
 */
VOS_UINT32 ATP_CFM_CfgMsgChgType(VOS_VOID * pvMsg, VOS_UINT32 ulMsgType);

VOS_UINT32 ATP_CFM_CfgMsgHeadBuilder(VOS_VOID **ppvMsg,
                         VOS_UINT32 ulMsgType,
                         const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32 ulOID);
VOS_UINT32 ATP_CFM_CfgMsgAppendPara(VOS_VOID **ppvMsg,
                         const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32         ulOID,
                         const VOS_VOID     *pvValue);

VOS_UINT32 ATP_CFM_CfgMsgAppendParaEx(VOS_VOID **ppvMsg,
                         const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32         ulOID,
                         const VOS_VOID     *pvValue,
                         VOS_UINT32        ulValueType);

ATP_CFM_E_IF_TYPE ATP_CFM_GetCfgToolOfMsg(const VOS_VOID* pvMsg);

VOS_UINT32 ATP_CFM_GetObjNumOfMsg(const VOS_VOID* pvMsg, VOS_UINT32* pulObjNum);
VOS_UINT32 ATP_CFM_GetParaNumOfMsg(const VOS_VOID* pvMsg,
                                   VOS_UINT32 ulObjIndex,
                                   VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                   VOS_UINT32* pulParaNum);

VOS_UINT32 ATP_CFM_GetParaOfMsgBySeqNum(const VOS_VOID *pvMsg,
                                        VOS_UINT32 ulObjIndex, // 从0开始
                                        VOS_UINT32 ulSeqNum, // 从0开始
                                        VOS_UINT32 *pulOID,
                                        VOS_VOID **ppValue);

VOS_UINT32 ATP_CFM_GetParaOfMsgByOID(const VOS_VOID *pvMsg,
                                     VOS_UINT32 ulObjIndex, // 从0开始
                                     VOS_UINT32 ulOID,
                                     VOS_VOID ** ppValue);
VOS_UINT32 ATP_CFM_GetParaOfMsgByOIDEx(const VOS_VOID *pvMsg,
                                     VOS_UINT32 ulObjIndex, // 从0开始
                                     VOS_UINT32 ulOID,
                                     VOS_VOID ** ppValue,
                                     VOS_UINT32 ulValueType);

VOS_UINT32 ATP_CFM_MsgFree(VOS_VOID* pvCfmMsgHead);

// this api just for debug

#ifdef ATP_DEBUG
VOS_VOID ATP_CFM_PrintMsgEx(const VOS_VOID* pvCfmMsg, const VOS_CHAR* pszFile, VOS_UINT32 ulLine);
#define ATP_CFM_PrintMsg(pvCfmMsg) ATP_CFM_PrintMsgEx((pvCfmMsg), __FILE__, __LINE__)
#else
#define ATP_CFM_PrintMsg(x...)
#endif

/*****************************配置消息操作接口*********************************/
/*
 *    OBJ Interfaces
 */
VOS_BOOL ATP_CFM_CompareObj(VOS_UINT32 ulOID,
                            const VOS_UINT32 *pulSrcObj,
                            const VOS_UINT32 *pulDstObj);

VOS_VOID ATP_CFM_FreeObj(VOS_UINT32 ulOID,
                         VOS_UINT32 *pulObj);

VOS_VOID ATP_CFM_FreeObjByName(const VOS_CHAR *pcObjName,
                         VOS_UINT32 *pulObj);

VOS_UINT32 ATP_CFM_CopyObjEx(VOS_UINT32 ulOID, VOS_UINT32 *pulDstObj,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 *ATP_CFM_CopyObj(VOS_UINT32 ulOID,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 *ATP_CFM_CopyObjByName(const VOS_CHAR *pcObjName,
                            const VOS_UINT32 *pulSrcObj);

VOS_UINT32 ATP_CFM_SetObjStrValue(VOS_UINT32 ulParaIndex,
                                   VOS_UINT32 *pulObj,
                                   const VOS_CHAR *pcNewValue);

VOS_UINT32 ATP_CFM_SetObjUintLongValue(VOS_UINT32 ulParaIndex,
                                   VOS_UINT32 *pulObj,
                                   const VOS_UINT64 ullNewValue);

VOS_BOOL ATP_CFM_IsKeyListValid(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulCMO);

VOS_BOOL ATP_CFM_IsKeyListSame(const VOS_UINT32 ulKeyList1[ATP_CFM_MAX_TREE_DEPTH],
                                        const VOS_UINT32 ulKeyList2[ATP_CFM_MAX_TREE_DEPTH],
                                        VOS_UINT32 ulCMO);

VOS_BOOL ATP_CFM_IsAttrNeedNotify(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                      VOS_UINT32    ulOID);

VOS_UINT32 ATP_CFM_GetObjParaCmo(VOS_UINT32 ulCMO, VOS_UINT32 ulIdx, VOS_UINT32 *pulParaCmo);


#ifdef ATP_DEBUG
VOS_VOID ATP_CFM_PrintObj(VOS_UINT32 ulOID,
                          const VOS_UINT32 *pulObjValues,
                          VOS_UINT32* pulNumOfParas);
#else
#define ATP_CFM_PrintObj(x...)
#endif


/*****************************配置消息操作接口*********************************/

/******************************************************************************
  函数名称  : ATP_CFMA_GetRetInfo
  功能描述  : 
  输入参数  : 
              1.  hCfgListHandle:
              2.  penretCode:
              3.  pulErrOID:
              4.  pszRetInfo:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
#if 0
VOS_UINT32 ATP_CFMA_GetRetInfo(ATP_CFM_E_MSG_RETCODE        *penRetCode,
                                      VOS_UINT32                *pulErrOID);
#endif

/* 这后面每个CFMA接口的超时时间都放在Ctx中，只需要初始化一次，不用每次都带该参数 */

/******************************************************************************
  函数名称  : ATP_CFMA_AddObj
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  *pulInstanceID:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_AddObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                  VOS_UINT32        ulOID,
                                  const VOS_UINT32  *pulObjValues,
                                  VOS_UINT32        *pulInstanceID,
                                  VOS_VOID          **ppvResMsg);

VOS_UINT32 ATP_CFMA_AddObjByMsg(const VOS_VOID* pvCfmMsg, VOS_VOID **ppvResMsg, VOS_UINT32 * pulInstanceID);

#define IS_CFMA_ADD_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NO_VISIT_ANY == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

#if 0
VOS_UINT32 ATP_CFMA_AddObjByName(const VOS_CHAR *pcObjName, VOS_UINT32 *pulInstanceID);
#endif

/******************************************************************************
  函数名称  : ATP_CFMA_DelObj
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_DelObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID);

VOS_UINT32 ATP_CFMA_DelObjByName(const VOS_CHAR *pcDelObj);

VOS_UINT32 ATP_CFMA_DelObjByMsg(const VOS_VOID* pvCfmMsg);

#define IS_CFMA_DEL_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NO_VISIT_ANY == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

/******************************************************************************
  函数名称  : ATP_CFMA_GetPara
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_GetPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

VOS_UINT32 ATP_CFMA_GetObj(const VOS_UINT32   ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32   ulOID,
                                 VOS_UINT32   **ppulObj,
                                 VOS_UINT32   *pulNumOfParas,
                                 VOS_UINT32   ulTimeout);

#define IS_CFMA_GET_OK(x)   (VOS_OK == (x))

/******************************************************************************
  函数名称  : ATP_CFMA_CheckPara
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_CheckPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

/******************************************************************************
  函数名称  : ATP_CFMA_CheckParaAcc
  功能描述  : 
  输入参数  : 
              1.  pvSrcMsg:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2012-09-18
     作    者   : l00163811
     修改内容   : 完成初稿

******************************************************************************/

VOS_UINT32 ATP_CFMA_CheckParaAcc(const VOS_VOID *pvSrcMsg, VOS_BOOL bReadAccess, VOS_BOOL bCheckPara, VOS_UINT32 ulTimeout);

/******************************************************************************
  函数名称  : ATP_CFMA_SetPara
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_SetPara(const VOS_VOID *pvSrcMsg, VOS_VOID **ppvResMsg, VOS_UINT32 ulTimeout);

#define IS_CFMA_SET_OK(x)   ((VOS_OK == (x)) || (ATP_E_ERR_CFM_NEED_REBOOT == (x)))

/******************************************************************************
  函数名称  : ATP_CFMA_GetObjInstNum
  功能描述  : 
  输入参数  : 
              1.  CfgListHandle:
              2.  *pulInstNum:
              3.  hInsSeqNumList:
              4.  ulWaitTime:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-27
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_GetObjInstNum(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                         VOS_UINT32 ulOID, 
                                         VOS_UINT32** ppulInstArray,  //该参数在外部使用free接口直接释放
                                         VOS_UINT32* pulInstNum);

/******************************************************************************
  函数名称  : ATP_CFMA_SetLockFlag
  功能描述  : 对CFMA操作加锁、解锁
  输入参数  : 
              1.  usLockMask: 锁定位掩码
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-11-29
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFMA_SetLockFlag(VOS_BOOL bLock, VOS_BOOL bRead);

/*****************************************************************************/
VOS_UINT32 ATP_DBGetDftObject(VOS_UINT32 ulOID, 
                              VOS_UINT32 **ppulDftValues, 
                              VOS_UINT32 *pulNumOfParas);

VOS_UINT32 ATP_DBGetDftObjectByName(const VOS_CHAR *pcObjName,
                              VOS_UINT32 **ppulDftValues, 
                              VOS_UINT32 *pulNumOfParas);

/******************************************************************************
  函数名称  : ATP_DBSetDftObject
  功能描述  : Set Obj default value and save db (if bSaveDB == VOS_TRUE)
  输入参数  : 
              1.  ulKeyList[ATP_CFM_MAX_TREE_DEPTH]:
              2.  ulOID:
              3.  bSaveDB:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-18
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_DBSetDftObject(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                              VOS_UINT32 ulOID, 
                              VOS_BOOL bSaveDB);

const VOS_CHAR *ATP_DBGetNameOfOID(VOS_UINT32 ulOID);
#if 0
VOS_UINT32 ATP_CFM_AppendInstNumToKeyList(VOS_UINT32 aulInstList[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulObjId, VOS_UINT32 ulInstId);
#endif
typedef enum tagATP_CFM_INSTALLOC_TYPE_EN
{
    ATP_CFM_INSTALLOC_NOCYCLE,
    ATP_CFM_INSTALLOC_RECYCLE,
} ATP_CFM_INSTALLOC_TYPE_EN;

VOS_UINT32 ATP_DBPreGetObjInstID(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID,
                                 VOS_UINT32 *pulInstId);

typedef VOS_UINT32 (* PFCfmDBIterator)(
                            VOS_UINT32                  ulOID,
                            const VOS_CHAR              *pcDomainName,
                            const VOS_VOID              *pvValue,
                            const ATP_PARA_ATTR_VAL_ST  *pstAttr,
                            VOS_VOID                    *pvArg);
typedef enum tagATP_DB_ITERATE_TYPE_EN
{
    ATP_DB_ITERATE_VALUE    =   1,
    ATP_DB_ITERATE_ATTR     =   2,
} ATP_DB_ITERATE_TYPE_EN;

VOS_UINT32 ATP_CFMA_IterateEx(const VOS_CHAR *pcDomainName,
                            VOS_VOID *pvArg, PFCfmDBIterator pfIterator, VOS_UINT32 ulSearchDepth,
                            ATP_DB_ITERATE_TYPE_EN  enIterType);

VOS_UINT32 ATP_CFMA_Iterate(const VOS_CHAR *pcDomainName,
                            VOS_VOID *pvArg, PFCfmDBIterator pfIterator, VOS_BOOL bNextLevel,
                            ATP_DB_ITERATE_TYPE_EN  enIterType);

VOS_UINT16 ATP_CFMA_GetWrb(
	VOS_UINT32                  ulOID);



typedef VOS_UINT32 (* PFCfmTTreeIterator)(
                            VOS_UINT32                  ulOID,
                            VOS_VOID                    *pvArg);

VOS_UINT32 ATP_TTree_Iterate(VOS_UINT32 ulOID,
                            VOS_VOID *pvArg, PFCfmTTreeIterator pfIterator, VOS_UINT32 ulSearchDepth);

VOS_UINT32 ATP_TTree_BuildName(VOS_UINT32 ulOID, const VOS_CHAR *pcSeperator, VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen);
VOS_UINT32 ATP_TTree_ParseName(const VOS_CHAR *pcName, const VOS_CHAR *pcSeperator, VOS_UINT32 *pulOID);

#define ATP_TTREE_BBF_SEPERATOR_D       "{i}"
#define ATP_TTREE_UPNP_DM_SEPERATOR_D   "#"

VOS_UINT32 ATP_TTree_BuildName(VOS_UINT32 ulOID, const VOS_CHAR *pcSeperator, VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen);
VOS_UINT32 ATP_TTree_ParseName(const VOS_CHAR *pcName, const VOS_CHAR *pcSeperator, VOS_UINT32 *pulOID);

VOS_UINT32 ATP_CFM_CfgMsgAppendDiffPara(VOS_VOID **ppvMsg,
                             const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                             VOS_UINT32 ulObjId,
                             const VOS_UINT32 *pulOldObj,
                             const VOS_UINT32 *pulNewObj);

/*
  * 适配函数接口
  */
/******************************************************************************
  函数名称  : ATP_CFM_ExtConfig
  功能描述  : 适配接口，全局适配信息
  输入参数  : 
              1.  pstGlbConfig:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-1-8
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFM_ExtConfig(ATP_CFM_GLOBAL_CONFIG* pstGlbConfig);

/******************************************************************************
  函数名称  : ATP_CFM_ExtWriteCfgFile
  功能描述  : 适配接口，将配置文件写入FLASH
  输入参数  : 
              1.  pszCfgFile:
              2.  ulCfgFileType:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-1-8
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_CFM_ExtWriteCfgFile(ATP_UTIL_STRFILE_ST *pstStrFile, VOS_UINT32 ulCfgFileType);

VOS_UINT32 ATP_CFM_ExtReadCfgFile(ATP_UTIL_STRFILE_ST *pstStrFile);

VOS_BOOL ATP_CFM_ExtIsCmoNeedEncrypt(VOS_UINT32 ulCmo);

VOS_CHAR *ATP_CFM_ExtEncodeParaValue(VOS_UINT32 ulCmo, const VOS_CHAR *pcParaValue);

VOS_CHAR *ATP_CFM_ExtDecodeParaValue(VOS_UINT32 ulCmo, const VOS_CHAR *pcParaValue);

/*!
  \brief configure tools lock
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK lock ok, other return current locked tools
*/
VOS_UINT32 ATP_CFM_Lock(VOS_VOID);

VOS_UINT32 ATP_CFM_Unlock(VOS_VOID);

VOS_UINT32 ATP_CFM_Getlock(VOS_VOID);

/*
 *      Global variable
 */
typedef enum tagATP_GVAR_IDX_EN
{
    ATP_UTIL_GVAR_TR069_DATA,
    ATP_UTIL_GVAR_TR069_VAL_CHG_LIST,
    ATP_UTIL_GVAR_TR064_DATA
} ATP_GVAR_IDX_EN;

#define ATP_CFM_ATTR_ACCLIST        "AccList"
#define ATP_CFM_ATTR_NOTIFY         "Notify"
#define ATP_CFM_ATTR_RESERVE        "Resv"
#define ATP_CFM_ATTR_HIDE_BITS      "HideBits"

extern const VOS_UINT16 g_usHidePwdCfgTools;
extern VOS_BOOL g_bCheckSetPara;

/*
 *  General Message Interface
 */
typedef struct tagATP_GMSG_HEAD
{
    VOS_UINT32          ulMagicNum;      // 魔术字
    VOS_UINT32          ulAllSize;       // 消息体总大小
    VOS_UINT32          ulFreeSize;      // 消息体剩余空间
    VOS_UINT32          ulErrCode;       // 错误码
    VOS_UINT32          ulIntfType;      // 配置工具类型
    VOS_UINT32          ulGMsgSubCode;   // 配置操作码

    VOS_CHAR            acModName[16];
} ATP_GMSG_HEAD;

typedef enum tagATP_GMSG_SUBTYPE_EN
{
    ATP_GMSG_SUBTYPE_GET,
    ATP_GMSG_SUBTYPE_ASYNC_SET,
    ATP_GMSG_SUBTYPE_SYNC_SET
} ATP_GMSG_SUBTYPE_EN;

VOS_UINT32 ATP_GMSG_Builder(VOS_VOID **ppvGMsg, const VOS_CHAR  *pcModName, VOS_UINT32 ulSubType);

VOS_UINT32 ATP_GMSG_AppendNV(VOS_VOID **ppvGMsg, const VOS_CHAR *pcName, const VOS_CHAR *pcValue);

const VOS_CHAR *ATP_GMSG_GetHeadInfo(const VOS_VOID *pvGMsg, VOS_UINT32 *pulSubType);

const VOS_CHAR *ATP_GMSG_GetValueByName(const VOS_VOID *pvGMsg, const VOS_CHAR *pcName);

VOS_VOID ATP_GMSG_Free(VOS_VOID *pvGMsg);

#ifdef ATP_DEBUG
VOS_VOID ATP_GMSG_Print(VOS_VOID *pvGMsg);
#else
#define ATP_GMSG_Print(x...)
#endif

VOS_UINT32 ATP_GMSG_Send(VOS_VOID *pvGMsg);

VOS_UINT32 ATP_GMSG_SyncSend(VOS_VOID *pvGMsg, VOS_VOID **ppvRespMsg, VOS_UINT32 ulTimeout);

VOS_UINT32 ATP_GMSG_Reply(VOS_VOID *pvGMsg, VOS_UINT32 ulErrCode, VOS_VOID *pvRespGMsg);

VOS_UINT32 ATP_GMSG_ReplyRetCode(VOS_VOID *pvGMsg, VOS_UINT32 ulErrCode);

/*
 *  Value type utilities
 */
VOS_BOOL ATP_CFM_UtilChar2Ulong(const VOS_CHAR *pcValue, VOS_UINT32 *pulValue);

VOS_BOOL ATP_CFM_UtilChar2Long(const VOS_CHAR *pcValue, VOS_INT32 *plValue);

VOS_BOOL ATP_CFM_UtilConvertValueType(
                        const VOS_CHAR       *pcValue,
                        VOS_VOID             **pvValue,
                        VOS_UINT32           ulValueType);

VOS_UINT32 ATP_CFM_NotifyValueChange(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                                             VOS_UINT32 ulOid, const VOS_VOID *pvValue);

VOS_UINT32 ATP_CFM_ChangeInstId(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH], 
                                       VOS_UINT32        ulOID,
                                       VOS_UINT32       ulNewInstId);
VOS_UINT32 ATP_UTIL_EncodePath(const VOS_CHAR *pcSrc, VOS_CHAR *pcDst, VOS_UINT32 ulLen);
VOS_UINT32 ATP_UTIL_DecodePath(const VOS_CHAR *pcSrc, VOS_CHAR *pcDst, VOS_UINT32 ulLen);

typedef VOS_UINT32 (*PFCFMUtilDataProc)(const VOS_UINT8 *pucSrc, VOS_UINT32 ulSrcLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);

typedef struct tagATP_CFM_DataProc_ST
{
    PFCFMUtilDataProc pfDataProc;
}ATP_CFM_DataProc_ST;

VOS_UINT32 ATP_CFM_UtilDataProcs(
                                         const VOS_UINT8            *pucSrc, 
                                         VOS_UINT32                 ulSrcLen, 
                                         VOS_UINT8                  **ppucDst, 
                                         VOS_UINT32                 *pulDstLen,
                                         const ATP_CFM_DataProc_ST *pstDataProcs,
                                         VOS_UINT32                 ulArrayLen);

#ifdef SUPPORT_ATP_LIBZ
/*!
  \brief 压缩
  \param[in]  *pucStr: 
  \param[in]  ulStrLen: 
  \param[in]  **ppucDst: 
  \param[in]  *pulDstLen: 
  \return 
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_Compress(const VOS_UINT8 *pucStr, VOS_UINT32 ulStrLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);

/*!
  \brief 解压
  \param[in]  *pucStr: 
  \param[in]  ulStrLen: 
  \param[in]  **ppucDst: 
  \param[in]  *pulDstLen: 
  \return 
  \retval VOS_OK 成功
*/
VOS_UINT32 ATP_UTIL_Uncompress(const VOS_UINT8 *pucStr, VOS_UINT32 ulStrLen, VOS_UINT8 **ppucDst, VOS_UINT32 *pulDstLen);
#endif

typedef VOS_UINT32 (*PFCfmGetDomainValuesHook)(VOS_VOID *pvHandle, VOS_UINT32 ulOID, VOS_VOID *pvValue);

VOS_UINT32 ATP_CFM_CfgGetDomainValues(
                            VOS_CHAR * const    acDomainNames[],
                            VOS_UINT32          ulNum,
                            PFCfmGetDomainValuesHook pfHook,
                            VOS_VOID            *pvHandle);


typedef VOS_BOOL (* PFCfmEndParseHook)(VOS_UINT32 ulOID, const VOS_CHAR *pcEndBuf);
extern PFCfmEndParseHook g_pfEndParseHook;

// For DM Message
typedef struct tagATP_DMMSG_HEAD
{
    VOS_UINT32          ulMagicNum;      // 魔术字
    VOS_UINT32          ulAllSize;       // 消息体总大小
    VOS_UINT32          ulFreeSize;      // 消息体剩余空间
    VOS_UINT32          ulErrCode;       // 错误码
    VOS_UINT32          ulIntfType;      // 配置工具类型
    VOS_UINT32          ulMsgSubCode;   // 配置操作码
} ATP_DMMSG_HEAD;

typedef struct tagATP_DMMSG_ITEM
{
    VOS_UINT32          ulIdentity;
    VOS_UINT32          ulNameLen;
    VOS_UINT32          ulValueLen;
    const VOS_CHAR      *pcName;
    const VOS_CHAR      *pcValue;
} ATP_DMMSG_ITEM;

typedef enum tagATP_DMMSG_TYPE_EN
{
    ATP_DMMSG_TYPE_UNKNOWN,
    ATP_DMMSG_GET_NAMES,
    ATP_DMMSG_GET_VALUES,
    ATP_DMMSG_GET_ATTRS,
    ATP_DMMSG_SET_VALUES,
    ATP_DMMSG_SET_ATTRS,
    ATP_DMMSG_ADD_OBJ,
    ATP_DMMSG_DEL_OBJ,
    ATP_DMMSG_REBOOT,
    ATP_DMMSG_RESET,
    ATP_DMMSG_DOWNLOAD,
    ATP_DMMSG_UPLOAD,
    ATP_DMMSG_GET_DEVICE_STATUS,
    ATP_DMMSG_GET_DOWNLOAD_STATUS,
    //...
    ATP_DMMSG_CONFIG_SERVICE,
    ATP_DMMSG_GET_DEVICE_INFO,                  //获取设备的版本等信息,用于业务自动发放和升级
} ATP_DMMSG_TYPE_EN;

VOS_UINT32 ATP_DMMSG_Builder(VOS_VOID **ppvDMMsg, const VOS_CHAR *pcRecvApp, VOS_UINT32 ulSubType);
VOS_UINT32 ATP_DMMSG_AppendItem(VOS_VOID **ppvDMMsg, VOS_UINT32 ulIdentity,
                            const VOS_CHAR *pcName, const VOS_VOID *pvValue, VOS_UINT32 ulValueLength);
VOS_UINT32 ATP_DMMSG_GetErrCode(const VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_GetSubType(const VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_GetItem(const VOS_VOID *pvDMMsg, ATP_DMMSG_ITEM *pstItem, VOS_VOID **ppvPos);
VOS_VOID ATP_DMMSG_Free(VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_Send(VOS_VOID *pvDMMsg);
VOS_UINT32 ATP_DMMSG_SyncSend(VOS_VOID *pvDMMsg, VOS_VOID **ppvRespMsg, VOS_UINT32 ulTimeout);
VOS_UINT32 ATP_DMMSG_ReplyRetCode(VOS_VOID *pvDMMsg, VOS_UINT32 ulErrCode);
VOS_UINT32 ATP_DMMSG_Reply(VOS_VOID *pvDMMsg, VOS_UINT32 ulErrCode, VOS_VOID *pvRespDMMsg);
VOS_UINT32 ATP_DMMSG_ReplyEx(const VOS_CHAR *pcDstApp, VOS_UINT32 ulErrCode, VOS_VOID *pvRespDMMsg);
// check if need tr181 defined
#ifdef SUPPORT_ATP_TR181
VOS_VOID ATP_DB_ALIAS_FreeRefList();
#endif

//#ifdef ATP_DEBUG
#if 1
VOS_VOID ATP_DMMSG_Print(VOS_VOID *pvDMMsg);
#endif

#ifdef ATP_DEBUG
#define ATP_CFM_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_CFM_DEBUG(x...)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif  /* __CFM_API_H__ */

