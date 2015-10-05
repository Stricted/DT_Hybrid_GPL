/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 ******************************************************************************/
/**\file atplog.h
 *
 *\copyright 2007-2020 HUAWEI
 *\author p42047
 *\date  2012-1-16
 */

#ifndef __ATP_LOG_H__
#define __ATP_LOG_H__

#include <time.h>
#include "atptypes.h"
#include "msgapi.h"
#include "atplogdef.h"
#include "atptracedef.h"
#include "atpflashapi.h"

#define ATP_CBBID_LOG_NAME      "log"

#define ATP_LOG_WELCOME_INFO    "Manufacturer:Huawei Technologies Co., Ltd."
#define ATP_LOG_HOST_NAME       "HuaweiATP"

#define ATP_LOG_MAX_CONTENT_LEN     (1000)
#define ATP_LOG_MAX_ITEM_LEN        (1024)
#define ATP_LOG_STR_LEN_MID         (128)
#define ATP_LOG_STR_LEN_LARGE       (512)
#define ATP_LOG_HOSTNAME_LEN        (32)
#define ATP_LOG_FILENAME_LEN        (64)
#define ATP_LOG_APPNAME_NUMBER      (64)
#define ATP_LOG_RTOSERVERNAME_LEN   (64)
#define ATP_LOG_LEN_64              (64)
#define ATP_LOG_LEN_32              (32)
#define ATP_LOG_MAGIC_LEN           (8)
#define ATP_LOG_APPNAME_FILTER      (ATP_LOG_APPNAME_NUMBER/2)
#define ATP_LOG_MAGIC_NUMBER        (42047)
#define ATP_LOG_MAGIC_STRING        "ATP_LOG"
#define ATP_LOG_TEMP_LOG_FILE       "/var/logtemp"
#define ATP_LOG_RTO_NAME            "rto"

#define ATP_LOG_LEVEL_NUM           8

typedef struct tm ATP_LOG_TIME_ST;

/*
 * Trace level of each module debug infomation,
 * default value is off (0),
 * other value is critical(1), error(2), warning(3), notice(4).
 * Each module can set its own trace level.
 *
**/
typedef enum tagATP_TRACE_LEVEL_EN
{
    ATP_TRACE_LEVEL_OFF         = 0,
    ATP_TRACE_LEVEL_CRITICAL    = 1,
    ATP_TRACE_LEVEL_ERROR       = 2,
    ATP_TRACE_LEVEL_WARNING     = 3,
    ATP_TRACE_LEVEL_NOTICE      = 4,

    ATP_TRACE_LEVEL_END
} ATP_TRACE_LEVEL_EN;

/*
 * LOG Error return code.
 */
typedef enum tagATP_LOG_API_RET_CODE_EN
{
    ATP_LOG_RET_PARA_ERR = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_LOG),
    ATP_LOG_RET_NOT_INITED,
    ATP_LOG_RET_TOO_LONG,
    ATP_LOG_RET_LOG_GVAR,
    ATP_LOG_RET_FLUSH_LOG,
    ATP_LOG_RET_MEM,
    ATP_LOG_RET_CONTENT_NULL,
    ATP_LOG_RET_READ_FLH_ERR,
    ATP_LOG_RET_READ_FLH_MAGIC_STRING_ERR,
} ATP_LOG_API_RET_CODE_EN;

typedef struct tagATP_LOG_MSG_ST
{
    VOS_UINT16 usLogLen;
    VOS_UINT16 usReserved;
    VOS_UINT32 ulLogTypeLevel;
    VOS_UINT32 ulLogNum;
} ATP_LOG_MSG_ST;

typedef enum tagATP_LOG_OPT
{
    ATP_LOG_OPT_SET_FILTER,
    ATP_LOG_OPT_GET_FILTER,
    ATP_LOG_OPT_QUERY,
} ATP_LOG_OPT;

typedef enum tagATP_LOG_LANG_EN
{
    ATP_LOG_LANG_ENGLISH,
    ATP_LOG_LANG_CHINESE,
    ATP_LOG_LANG_GERMAN,
    ATP_LOG_LANG_SPANISH
} ATP_LOG_LANG_EN;

typedef struct tagATP_LOG_FILTER_MSG
{
    VOS_UINT32 ulLogTypeLevel;
    VOS_UINT32 ulLogNum;
    VOS_UINT32 ulLogMaxNum;
    VOS_UINT32 ulLogMaxBufLen;
    VOS_UINT32 ulOpt;  // ref: ATP_LOG_OPT
    VOS_UINT32 ulLogLanguage; // ref: ATP_LOG_LANG_EN;
    VOS_UINT32 ulLogType;
    VOS_CHAR   acFilePath[ATP_LOG_FILENAME_LEN];
} ATP_LOG_FILTER_MSG;

typedef struct tagATP_LOG_RTO_MSG
{
    VOS_CHAR   acPriServer[ATP_LOG_RTOSERVERNAME_LEN + 4];
    VOS_CHAR   acSecServer[ATP_LOG_RTOSERVERNAME_LEN + 4];

    VOS_UINT16 usPriPort;  // 0 for close
    VOS_UINT16 usSecPort;  // 0 for close

    VOS_CHAR   acHostName[ATP_LOG_HOSTNAME_LEN];

    VOS_INT32  lFd;
    VOS_UINT32 ulOpr;      // ref: ATP_LOG_RTO_OPT_EN
} ATP_LOG_RTO_MSG;

typedef struct tagATP_LOG_TRIGGER_INTV_MSG
{
    VOS_UINT32  ulTriggerIntv;  // second, 0: stop trigger
} ATP_LOG_TRIGGER_INTV_MSG;

typedef struct tagATP_LOG_FLUSH_MSG
{
    VOS_UINT32 ulReserved;      // not used now
} ATP_LOG_FLUSH_MSG;

typedef struct tagATP_LOG_CLEAR_BUFF_MSG
{
    VOS_UINT32 ulReserved;      // not used now
} ATP_LOG_CLEAR_BUFF_MSG;

typedef struct tagATP_LOG_FLASH_HEAD_ST
{
    VOS_CHAR    acMagic[ATP_LOG_MAGIC_LEN];
    VOS_UINT32  ulFlashBuffLen;
} ATP_LOG_FLASH_HEAD_ST;

typedef struct tagATP_LOG_FORMAT_ST
{
    const VOS_CHAR* pszAppName;
    VOS_UINT32      ulLogType;
    VOS_UINT32      ulLogLevel;
    VOS_UINT32      ulLogNum;
    const ATP_LOG_TIME_ST*  pstTime;
    const VOS_CHAR* pszHostName;
    const VOS_CHAR* pszContent;
    VOS_UINT32      ulContentLen;
    const VOS_CHAR* pszLogItem;
    const VOS_CHAR* pszOwner;
} ATP_LOG_FORMAT_ST;

#define ATP_LOG_NUMBER_DEF(x)  ((1 << 31) + ((x) << 20))

typedef enum tagATP_LOG_NUMBER_EN
{
    /* cfm */
    ATP_LOG_LEVEL_CFM_START     = ATP_LOG_NUMBER_DEF(ATP_MODULE_CFM),

    /* cms */
    ATP_LOG_LEVEL_CMS_START     = ATP_LOG_NUMBER_DEF(ATP_MODULE_CMS),

    /* mic */
    ATP_LOG_LEVEL_MIC_START     = ATP_LOG_NUMBER_DEF(ATP_MODULE_MIC),

} ATP_LOG_NUMBER_EN;

#define ATP_LOG_ALL_TYPE        (0x00FFFFFF)
#define ATP_LOG_ALL_LEVEL       (0x000000FF)
#define ATP_LOG_ALL_LOGNUM      (0xFFFFFFFF)

#define ATP_LOG_TYPE_COUNT      (24)
#define ATP_LOG_LEVEL_COUNT     (8)

typedef enum tagATP_LOG_RTO_OPT_EN
{
    ATP_LOG_RTO_SERVER,
    ATP_LOG_RTO_OPEN,
    ATP_LOG_RTO_CLOSE,
    ATP_LOG_RTO_CLOSEALL
} ATP_LOG_RTO_OPT_EN;

// These enum values come from AMS, which means feature id.
// These values are auto generated, which are unchangeable.
typedef enum tagATP_LOG_FEATURE_NAME_EN
{
    ATP_LOG_FEATURE_IGMP        =   0x40,
    ATP_LOG_FEATURE_WEB         =   0x50,
    ATP_LOG_FEATURE_DDNS        =   0x90,
    ATP_LOG_FEATURE_SNTP        =   0xa0,
    ATP_LOG_FEATURE_PPPC        =   0xb0,

    ATP_LOG_FEATURE_LOG         =   0x1b0,
    ATP_LOG_FEATURE_UPNP        =   0x310,
    ATP_LOG_FEATURE_QOS         =   0x4d0,
    ATP_LOG_FEATURE_WAN         =   0x8f0,

    ATP_LOG_FEATURE_WLAN        =   0x140,
    ATP_LOG_FEATURE_USBMOUNT    =   0x150,
    ATP_LOG_FEATURE_CWMP        =   0x180,
    ATP_LOG_FEATURE_UPG         =   0x1a0,
    ATP_LOG_FEATURE_CFM         =   0x1e0,

    ATP_LOG_FEATURE_NAT         =   0x4f0,

    ATP_LOG_FEATURE_CLI         =   0x840,
    ATP_LOG_FEATURE_BHAL        =   0x850,
    ATP_LOG_FEATURE_FIREWALL    =   0xac0,
    ATP_LOG_FEATURE_XDSL        =   0xbd0,
    ATP_LOG_FEATURE_POWERMNGT   =   0xbe0,
    ATP_LOG_FEATURE_VOICE       =   0xd50,

    ATP_LOG_FEATURE_HTTP,

    ATP_LOG_FEATURE_UNKNOWN
} ATP_LOG_FEATURE_NAME_EN;

#define ATP_LOG_KERNEL_FEATURE_ID   0xfff

#define LOG_FEATUREID_OFFSET (20)

#define ATP_LOG_TYPE_OFFSET     (8)
#define ATP_LOG_GET_LOGTYPE(x)  (1 << ((x) + ATP_LOG_TYPE_OFFSET))
#define ATP_LOG_GET_LOGLEVEL(x) (1 << (x))

typedef enum tagATP_LOG_APP_TYPE_EN
{
    ATP_LOG_APP_TYPE_DEVICE = 0,
    ATP_LOG_APP_TYPE_VOICE = 1,
    ATP_LOG_APP_TYPE_END
} ATP_LOG_APP_TYPE_EN;

typedef struct tagATP_LOG_APP_TYPE_TO_FLASH_AREA
{
    ATP_LOG_APP_TYPE_EN appType;
#ifndef SUPPORT_DESKTOP
    FOM_LOG_TYPE        logFlashArea;
#else
    VOS_UINT32          logFlashArea;
#endif
} ATP_LOG_APP_TYPE_TO_FLASH_AREA;

typedef struct tagATP_LOG_TYPE_LIMIT_ST
{
    VOS_UINT32          ulLogType;
    VOS_UINT32          ulLimitCnt;
    VOS_UINT32          ulCnt;
} ATP_LOG_TYPE_LIMIT_ST;

// The structure sent to log process
typedef struct tagATP_LOG_LOGITEM_PARAS_ST
{
    VOS_UINT32  ulFeatureIdOffset;
    VOS_UINT16  usParaNum;
    VOS_UINT16  ulContentLen;
    VOS_INT8    pszContent[0];
} ATP_LOG_LOGITEM_PARAS_ST;

#define LOGITEM_PARAS_ST_LENGTH(p) (sizeof(ATP_LOG_LOGITEM_PARAS_ST)+(p)->ulContentLen)

// The structure written in flash
typedef struct tagATP_LOG_ITEM_ST
{
    VOS_UINT32                  ulLogTypeLevel;     // higher 24 bits for log type, logwer 8 bits for log level.
    VOS_UINT32                  ulLogNum;
    time_t                      ulLogTime;
    VOS_UINT16                  usMagic;            // usMagic = LOGITEM_PARAS_ST_LENGTH(&stLogParasSt) + ATP_LOG_MAGIC_NUMBER
    ATP_LOG_LOGITEM_PARAS_ST    stLogParasSt;
} ATP_LOG_ITEM_ST;

#define LOG_ITEM_ST_LENGTH(p) (sizeof(ATP_LOG_ITEM_ST)+(p)->stLogParasSt.ulContentLen)

// The structure written in /var/debuglog
typedef struct tagATP_TRACE_ITEM_ST
{
    VOS_UINT32                  ulTraceLevel;   // trace level. (Critical, error, warning, notice)
    ATP_LOG_LOGITEM_PARAS_ST    stLogParasSt;
} ATP_TRACE_ITEM_ST;

typedef struct tagATP_LOG_ITEM_ST_LIST
{
    struct tagATP_LOG_ITEM_ST_LIST  *pstNext;
    ATP_LOG_ITEM_ST                 stLogItem;
} ATP_LOG_ITEM_ST_LIST;

#define LOG_ITEM_ST_LIST_LENGTH(p) (sizeof(struct tagATP_LOG_ITEM_ST_LIST*)+LOG_ITEM_ST_LENGTH(&p->stLogItem))

// The structure in atplogstring.c
typedef struct tagATP_LOG_LOGITEM_INFO_ST
{
    VOS_UINT32      ulFeatureIdOffset;  // higher 12 bits for Feature id, lower 20 bits for Offset;
    VOS_UINT32      ulTypeLevel;        // higher 24 bits for log type, lower 8 bits for log level.
    VOS_UINT32      ulLogNum;
} ATP_LOG_LOGITEM_INFO_ST;

// The structure in atptracestr.h
typedef struct tagATP_TRACE_INFO_ST
{
    VOS_UINT32      ulFeatureIdOffset;  // higher 12 bits for Feature id, lower 20 bits for Offset;
    const VOS_CHAR  *pcTraceStr;        // trace string.
} ATP_TRACE_INFO_ST;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define IS_DEBUG_LOG(logid) ((logid) & 0x800000)

/*!
  \brief 日志记录API
  \param[in]  ulLogItemIdx: 日志ID
  \param[in]  ...: 日志可变参数列表，ulLogItemIdx对应的日志所需要的参数
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_RecordItem(VOS_UINT32 ulLogItemIdx, ...);
#define ATP_LOG_LogItemPrint(ulLogIdx, args...) ATP_LOG_RecordItem(ulLogIdx, ##args, VOS_NULL_PTR)

/*!
  \brief
  \param[in]  ulLogType: 日志类型
  \param[in]  ulLogLevel: 日志界别
  \param[in]  ulLogNum: 日志号
  \param[in]  *pszLogString: 日志字符串
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_KLogPrintf(VOS_UINT32 ulLogType, VOS_UINT32 ulLogLevel, VOS_UINT32 ulLogNum, const VOS_INT8 *pszLogString);
#define ATP_LOG_TrivialLogPrint(type, level, num, str) ATP_LOG_KLogPrintf((type), (level), (num), (str))


/*!
  \brief 日志查询API
  \param[in]  ulLogTypeMask: 日志类型掩码
  \param[in]  ulLogLevelMask: 日志级别掩码
  \param[in]  ulLogNum: 日志记录号
  \param[in]  language: 查询日志的语言
  \param[in]  ulMaxRecordNum:  查询日志限定的最大记录数
  \param[in]  pszFilePath:  以文件形式输出日志，指定的文件路径名
  \param[in]  pulRecordNum: 查询到的日志的记录数目
  \return VOS_OK/Error Code.
  \retval
*/

extern VOS_UINT32 ATP_LOG_QueryExt(VOS_UINT32 ulLogTypeMask, VOS_UINT32 ulLogLevelMask,
        VOS_UINT32 ulLogNum, VOS_UINT32 language, VOS_UINT32 ulMaxRecordNum,
        const VOS_CHAR* pszFilePath, VOS_UINT32* pulRecordNum, VOS_UINT32 ulLogType);

extern VOS_UINT32 ATP_LOG_Query(VOS_UINT32 ulLogTypeMask, VOS_UINT32 ulLogLevelMask,
        VOS_UINT32 ulLogNum, VOS_UINT32 language, VOS_UINT32 ulMaxRecordNum,
        const VOS_CHAR* pszFilePath, VOS_UINT32* pulRecordNum);
/*!
  \brief 日志删除API, 清除缓冲区中的全部日志
  \param[in] void:
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_ClearBuff(void);

extern VOS_UINT32 ATP_LOG_ClearLogByType(VOS_UINT32 ulLogType);

/*!
  \brief 将内存中的日志写入FLASH中
  \param[in] void:
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_Flush(void);

/*!
  \brief 设置日志的过滤策略
  \param[in]  ulLogTypeMask:日志类型掩码
  \param[in]  ulLogLevelMask:日志级别掩码
  \param[in]  ulLogNum:日志号
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_SetFilter(VOS_UINT32 ulLogTypeMask, VOS_UINT32 ulLogLevelMask,
        VOS_UINT32 ulLogNum);

/*!
  \brief 查询日志的过滤策略
  \param[in]  pulLogTypeMask:日志类型掩码
  \param[in]  pulLogLevelMask:日志级别掩码
  \param[in]  pulLogNum:日志号
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_GetFilter(VOS_UINT32* pulLogTypeMask, VOS_UINT32* pulLogLevelMask,
        VOS_UINT32* pulLogNum);

/*!
  \brief 日志实时输出到日志服务器
  \param[in]  pszHostName:  主机名
  \param[in]  acPrimServer: 主服务器ip或域名
  \param[in]  usPrimServerPort: 主服务器端口号
  \param[in]  acSecServer: 备服务器ip或域名
  \param[in]  usSecServerPort: 备服务器端口号
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_RtoToServer(const VOS_CHAR* pszHostName,
        const VOS_CHAR* acPrimServer,
        VOS_UINT16      usPrimServerPort,
        const VOS_CHAR* acSecServer,
        VOS_UINT16      usSecServerPort);

/*!
  \brief 开启/关闭LOG服务
  \param[in]  bEnable:
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_Enable(VOS_BOOL bEnable);

/*!
  \brief 获取LOG版本号
  \param[in] void:
  \return VOS_CHAR*
  \retval
*/
extern const VOS_CHAR* ATP_LOG_GetVersion(void);

extern const ATP_LOG_LOGITEM_INFO_ST G_ATP_LOGITEM_INFO_ARR[];
extern const VOS_CHAR* G_ATP_LOGITEM_STRING_ARR[];

/*!
  \brief 根据日志ID，获取日志的结构信息
  \param[in]  ulLogItemIdx: 日志ID
  \return 日志ID对应的结构ATP_LOG_LOGITEM_INFO_ST*，没查找到则返回NULL
  \retval
*/
extern const ATP_LOG_LOGITEM_INFO_ST* ATP_UTIL_LogItemFindByIdx(VOS_UINT32 ulLogItemIdx);

/*!
  \brief 根据日志的结构，和语种类别，获取到对应的日志字符串
  \param[in]  *pstLogItem: 日志信息结构
  \param[in]  lang: 日志的语种
  \return  日志字符串(const VOS_CHAR*)，没查找到则返回NULL
  \retval
*/
extern const VOS_CHAR* ATP_UTIL_LogItemGetFmtStr(const ATP_LOG_LOGITEM_INFO_ST *pstLogItem, ATP_LOG_LANG_EN lang);

/*!
  \brief 根据日志的id，获取日志对应的参数个数
  \param[in]  logid: 标识的唯一的日志。
  \return  参数个数
  \retval
*/
extern VOS_INT32 ATP_UTIL_LogItemGetParaNum(VOS_UINT32 logid);

/*!
  \brief 根据日志的参数结构，和语种类别，格式化成所对应的目标日志字符串
  \param[out]  buf: 目标日志字符串缓冲区
  \param[in]  ulBufLen:  存放目标日志字符串的缓冲区大小
  \param[in]  pszLogParaSt: 日志参数结构，包含了各参数的字符串
  \param[in]  lang: 语种类别
  \return 目标日志字符串的长度。-1 表示失败
  \retval
*/
extern VOS_INT32 ATP_UTIL_LogGetLogContent(VOS_CHAR* buf, VOS_UINT32 ulBufLen, const ATP_LOG_LOGITEM_PARAS_ST* pszLogParaSt, ATP_LOG_LANG_EN lang);

/*!
  \brief 根据日志的信息结构，和语种类别，获取日志的类型字符串
  \param[in]  pstLogItem: 日志信息结构
  \param[in]  lang: 语种类别
  \return 日志类型的字符串(const VOS_CHAR*)
  \retval
*/
extern const VOS_CHAR* ATP_UTIL_LogItemGetTypeString(const ATP_LOG_LOGITEM_INFO_ST* pstLogItem, ATP_LOG_LANG_EN lang);

/*!
  \brief 根据日志的信息结构，和语种类别，获取日志的级别字符串
  \param[in]  pstLogItem: 日志信息结构
  \param[in]  lang: 语种类别
  \return 日志级别的字符串(const VOS_CHAR*)
  \retval
*/
extern const VOS_CHAR* ATP_UTIL_LogItemGetLevelString(const ATP_LOG_LOGITEM_INFO_ST* pstLogItem, ATP_LOG_LANG_EN lang);

/*!
  \brief
  \param[in]
  \param[out]  p: All level string.
  \return
  \retval
*/
extern VOS_VOID ATP_UTIL_GetAllLogLevel(VOS_CHAR** p);

/*!
  \brief
  \param[in]  ulLogMaxBufferNumber: 日志缓冲区最大记录日志条数
  \return VOS_OK/Error Code.
  \retval
*/
extern VOS_UINT32 ATP_LOG_ProcStart(VOS_UINT32 ulLogMaxBufferNumber);

extern VOS_UINT32 ATP_LOG_TriggerIntv(VOS_UINT32 ulIntvSec);
extern VOS_UINT32 ATP_LOG_Switch(const VOS_CHAR acAppName[ATP_MSG_MAX_NAME_LEN + 1], VOS_BOOL bEnable);
extern VOS_UINT32 ATP_LOG_Printf(VOS_UINT32 ulLogType, VOS_UINT32 ulLogLevel, VOS_UINT32 ulLogNum, const VOS_INT8 *fmt, ...);

extern VOS_VOID ATP_LOG_MsgProc(const ATP_MSG_HEADER_ST* pstMsg);
extern VOS_VOID ATP_LOG_ItemMsgProcEx(VOS_VOID* pvMsg, const time_t* pstTime);
extern VOS_VOID ATP_LOG_ItemMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_RecordItemMsgProc(VOS_VOID* pvMsg, const time_t* pstTime);
extern VOS_VOID ATP_LOG_RecordKLogMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_FilterMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_RtoMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_TriggerMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_FlushMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_ClearByTypeMsgProc(VOS_UINT32 ulType);
extern VOS_VOID ATP_LOG_ClearMsgProc(VOS_VOID* pvMsg);
extern VOS_VOID ATP_LOG_EnableMsgProc(VOS_VOID* pvMsg);

extern ATP_LOG_TYPE_LIMIT_ST g_pstLogTypeLimits[];
extern ATP_LOG_APP_TYPE_TO_FLASH_AREA g_AppType_FlashArea[];

//***********************
//* LOG API for Adaptor *
//***********************
/*!
  \brief 获取发送日志的组件名
  \param[in]  ulLogIdx: 日志ID
  \return 日志的组件名或空字符串
  \retval
*/
extern const VOS_CHAR* logGetProcName(VOS_UINT32 ulLogIdx);

extern VOS_VOID ATP_LOG_PreRecord(const ATP_LOG_FILTER_MSG *pstFilter, FILE* pfFile);
extern VOS_VOID ATP_LOG_AftRecord(const ATP_LOG_FILTER_MSG *pstFilter, FILE* pfFile);

extern VOS_UINT32 ATP_LOG_RegRtoServeParse(const VOS_CHAR *acPriServer, const VOS_CHAR *acSecServer, VOS_UINT32 *pulPriIp, VOS_UINT32 *pulSecIp);

/*!
  \brief 获取日志存储的flash分区类型
  \param[in]  pszLogItemSt: 日志信息结构
  \return
  \retval ATP_LOG_APP_TYPE_VOICE/ATP_LOG_APP_TYPE_DEVICE/ATP_LOG_APP_TYPE_END
*/
extern ATP_LOG_APP_TYPE_EN getLogItemAppType(const ATP_LOG_ITEM_ST* pszLogItemSt);

/*!
  \brief 格式化日志
  \param[in]  pstLogFormat: 格式化的日志结构
  \param[in]  pszLogItem: 存储在flash中的日志结构
  \param[in]  lang: 语种类别
  \return VOS_OK/VOS_NOK
  \retval
*/
extern VOS_UINT32 ATP_LOG_RegFormat(const ATP_LOG_FORMAT_ST* pstLogFormat, const ATP_LOG_ITEM_ST* pszLogItem, ATP_LOG_LANG_EN lang);

/*!
  \brief 从flash日志分区中读取日志
  \param[in]  ppszBuff: 读取flash的内存空间
  \param[in]  pulLen: 读取flash的内存长度
  \param[in]  logFlashArea: flash存储日志类型，设备日志或语音日志
  \return VOS_OK/VOS_NOK
  \retval
*/
extern VOS_UINT32 ATP_LOG_ReadFlash(VOS_CHAR** ppszBuff, VOS_UINT32* pulLen, FOM_LOG_TYPE logFlashArea);

/*!
  \brief 写日志到flash分区
  \param[in]  pszBuffer: 写入flash的内存地址
  \param[in]  ulLen: 写入flash的长度
  \param[in]  logFlashArea: flash存储日志类型，设备日志或语音日志
  \return VOS_OK/VOS_NOK
  \retval
*/
extern VOS_UINT32 ATP_LOG_WriteFlash(const VOS_CHAR* pszBuffer, VOS_UINT32 ulLen, FOM_LOG_TYPE logFlashArea);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifdef ATP_DEBUG
#define ATP_LOG_DEBUG(format, args...)  {printf("[ ============== FILE : %s, LINE : %d ================= ]", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_LOG_DEBUG(format, args...)
#endif

#define LOG_ELEMENT(logId, level, type) {logId, level << ATP_LOG_TYPE_OFFSET | type, 1}

#define CIRCLE_DLIST_INIT(list) \
do { \
    list->pstPrev = list; \
    list->pstNext = list; \
} while(0)

#define CIRCLE_DLIST_INSERT(newNode, prev, next) \
do { \
    newNode->pstNext = next; \
    prev->pstNext = newNode; \
    newNode->pstPrev = prev; \
    next->pstPrev = newNode; \
} while(0)

#define CIRCLE_DLIST_APPEND(cdlist, newNode) \
do { \
    if (NULL == cdlist){ \
        CIRCLE_DLIST_INIT(newNode); \
        cdlist = newNode; \
    } \
    else { \
        CIRCLE_DLIST_INSERT(newNode, cdlist->pstPrev, cdlist); \
    } \
} while(0)

#define CIRCLE_DLIST_INSERT_HEAD(cdlist, newNode) \
do { \
    CIRCLE_DLIST_APPEND(cdlist, newNode); \
    cdlist = newNode; \
} while(0)

VOS_UINT32 ATP_LOG_RegLogLimits(VOS_UINT32 ulLogId, VOS_UINT32 ulTimeInterval);

typedef struct tagAtpLogQueryLimits
{
    VOS_UINT32 ulMaxRecordNum;
    VOS_UINT32 ulMaxBufLength;
} ATP_LOG_QUERY_LIMITS_ST;

#endif
