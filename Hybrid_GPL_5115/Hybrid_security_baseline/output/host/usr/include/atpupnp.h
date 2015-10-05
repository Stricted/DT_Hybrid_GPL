/******************************************************************************
版权所有  : 2007-2020，华为技术有限公司
文 件 名  : upnp.h
作    者  : handy
版    本  : V1.0
创建日期  : 2008-2-3
描    述  : UPnP/TR064模块接口
函数列表  :

历史记录      :
1.日    期   : 2008-2-3
作    者   : handy
修改内容   : 完成初稿

*********************************************************************************/

#ifndef __ATP_UPNP_H__
#define __ATP_UPNP_H__

#include "atptypes.h"
#include "xmlapi.h"
#include "httpapi.h"
#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */


#define ATP_UPNP_USE_ATP_CFM    1  // use atp cfm as data model
#define ATP_UPNP_USE_SHM       0  // use gvar shm

#define ATP_CBBID_UPNP_NAME    "upnp"

/*
*  常量宏定义
*/

#define ATP_UPNP_CFG_PASSWORD           "123456"
#define ATP_UPNP_CFG_USER               "dslf-config"
#define ATP_UPNP_RESET_USER             "dslf-reset"

#define ATP_UPNP_SETCFGPWD_ACTION       "SetConfigPassword"
#define ATP_UPNP_SSDP_OPT               "\"http://schemas.upnp.org/upnp/1/0/\"; ns=01"

#define ATP_UPNP_VERSION                "ATP upnp v2.0"
#define ATP_UPNP_SERVER                 "Linux UPnP/1.0 Huawei-ATP-IGD"
#define ATP_UPNP_NT_ROOT_DEV            "upnp:rootdevice"
#define ATP_UPNP_SSDP_ALIVE             "ssdp:alive"
#define ATP_UPNP_SSDP_BYEBYE            "ssdp:byebye"
#define ATP_UPNP_SSDP_DISCOVER          "ssdp:discover"

#define TR064_PARA_NAME_PREFIX_D        "New"

#define ATP_UPNP_MSG_TIMEOUT            (30000)
#define ATP_UPNP_MAX_WAIT_TIME          (120)

#define ATP_UPNP_MAX_STR_ARR  		(32)

#define ATP_UPNP_MACADDR_LEN            (6)
#define ATP_UPNP_STR_LEN_TINY           (32)
#define ATP_UPNP_STR_LEN_SMALL          (64)
#define ATP_UPNP_STR_LEN_MID            (128)
#define ATP_UPNP_STR_LEN_BIG            (256)
#define ATP_UPNP_STR_LEN_LARGE          (512)
#define ATP_UPNP_STR_LEN_HUGE           (1024)

#define ATP_UPNP_BGET_MEM_SIZE          (65536) //45k is enough

/*
*  协议栈所支持的UPnP定义SOAP错误码
*/
#define ATP_UPNP_SUCCESS                (0)
#define ATP_UPNP_SOAP_INVALID_ACTION    (401)
#define ATP_UPNP_SOAP_INVALID_ARGS      (402)
#define ATP_UPNP_SOAP_ACTION_FAILED     (501)
#define ATP_UPNP_SOAP_ARG_VALUE_INVALID (600)
#define ATP_UPNP_SOAP_OUT_OF_MEMORY     (603)
#define ATP_UPNP_SOAP_STRARG_VALUE_TOOLONG (605)
#define ATP_UPNP_SSL_NEEDED        (504)
#define ATP_UPNP_SOAP_NOT_AUTHED        (606)
#define ATP_UPNP_SOAP_NOT_ENCRYPT       (609)
//ADD FOR DT START
#define ATP_UPNP_SOAP_VALUE_HAS_SPAC (701)
#define ATP_UPNP_SOAP_VALUE_SPAC_INVAL (702)
#define ATP_UPNP_SOAP_WANPPP_CONNCTFAILED   (704)
#define ATP_UPNP_SOAP_WANPPP_CONNINPROGRESS (705)
#define ATP_UPNP_SOAP_WANPPP_UNCONFIGRED    (706)
#define ATP_UPNP_SOAP_WANPPP_DISCONNECTING  (707)
#define ATP_UPNP_SOAP_WANPPP_INVALIDLAYER   (708)
#define ATP_UPNP_SOAP_INTERNET_DISABLED     (709)
#define ATP_UPNP_SOAP_CONNECTION_TERMITATED (711)
#define ATP_UPNP_SOAP_IDX_OUT_OF_BOUNDS (713)
#define ATP_UPNP_SOAP_NO_SUCH_ENTRY     (714)
#define ATP_UPNP_SOAP_INVA_CHANNEL     (728)
#define ATP_UPNP_SOAP_WEBUI_ACTIVD (800)
#define ATP_UPNP_SOAP_PARA_TOOSHORT (801)
#define ATP_UPNP_SOAP_PARA_TOOLONG (802)
#define ATP_UPNP_SOAP_PARA_HAVEINVACHARA (803)

#define ATP_UPNP_SOAP_TR069_ACTIVD (804)
#define ATP_UPNP_SOAP_TR064_ACTIVD (805)
#define ATP_UPNP_SOAP_WANPPP_CONNECTED (806)
#define ATP_UPNP_SOAP_WRITEACCESSDISABLED  (898)
#define ATP_UPNP_SOAP_SESSIONIDEXPIRED  (899)

#define ATP_UPNP_SOAP_INVALID_WEP_KEY   (731)
#define ATP_UPNP_SOAP_INVALID_MAC       (729)
// Portmapping
#define ATP_UPNP_SOAP_WildCardNotPermittedInSrcIP       (715)   // Not possible
#define ATP_UPNP_SOAP_WildCardNotPermittedInExtPort     (716)
#define ATP_UPNP_SOAP_ConflictInMappingEntry            (718)
#define ATP_UPNP_SOAP_SamePortValuesRequired            (724)   // Not possible
#define ATP_UPNP_SOAP_OnlyPermanentLeasesSupported      (725)
#define ATP_UPNP_SOAP_RemoteHostOnlySupportsWildcard    (726)   // Not possible
#define ATP_UPNP_SOAP_ExternalPortOnlySupportsWildcard  (727)   // Not possible
#define ATP_UPNP_SOAP_PRESHAREKEY_EMPTY       (733)/*The complete PSK key set is empty*/

#define ATP_UPNP_LOGIN_INTERVAL        (1*60) // 1 secend
#define ATP_UPNP_TRY_LOGIN_TIMES      (3)

typedef struct tagATP_UPNP_ERRSTR_ST
{
    VOS_UINT32          ulErrCode;
    const VOS_CHAR      *pcErrStr;
} ATP_UPNP_ERRSTR_ST;

//extern const ATP_UPNP_ERRSTR_ST g_pstErrStrs[];
//ADD FOR DT END
// For UPnP DM
#define ATP_UPNP_SOAP_INVALID_ARGUMENT_SYNTAX (701)
#define ATP_UPNP_SOAP_INVALID_XML       (702)
#define ATP_UPNP_SOAP_NO_SUCH_NAME      (703)
#define ATP_UPNP_SOAP_INVALID_TYPE      (704)
#define ATP_UPNP_SOAP_INVALID_VALUE     (705)
#define ATP_UPNP_SOAP_RD_ONLY_VIOLATION (706)
#define ATP_UPNP_SOAP_MULTIPLE_SET      (707)
#define ATP_UPNP_SOAP_UNAVAILABLE       (708)
#define ATP_UPNP_SOAP_INVALID_URI       (801)
#define ATP_UPNP_SOAP_UNSUPPROTED_PROTOCOL      (803)
#define ATP_UPNP_SOAP_DOWNLOAD_FAIL       (804)
#define ATP_UPNP_SOAP_DOWNLOAD_COMPLET       (8)

/*
*  UPnP/TR064协议栈接口返回的错误码
*  所有接口VOS_UINT32类型的返回码成功时，都返回VOS_OK
*/
typedef enum tagATP_UPNP_ERR_CODE
{
    ATP_ERR_UPNP_INVALID_PARA       = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_UPNP),
    ATP_ERR_UPNP_GVAR,
    ATP_ERR_UPNP_MEMORY,
    ATP_ERR_UPNP_REG_OR_ADAPT,
    ATP_ERR_UPNP_NOT_FOUND,
    ATP_ERR_UPNP_TYPE_STRING,
    ATP_ERR_UPNP_HTTP,
    ATP_ERR_UPNP_HTTP_HEAD,    
    ATP_ERR_UPNP_HTTP_SSL,
    ATP_ERR_UPNP_FILE,
    ATP_ERR_UPNP_XML,           // a
    ATP_ERR_UPNP_INTERNAL,
    ATP_ERR_UPNP_INVALID_SESSIONID,
    ATP_ERR_UPNP_ALREADY_LOCKED,
    ATP_ERR_UPNP_NOT_LOCKED,

} ATP_UPNP_ERR_CODE;

/* UPnP extended HTTP directive codes */
typedef enum tagATP_UPNP_DIRECTIVE_CODE_EN
{
    // For UPnP and TR064 only
    ATP_HTTP_DIRECTIVE_CODE_NOTIFY      = ATP_HTTP_DIRECTIVE_CODE_EXTEND,
    ATP_HTTP_DIRECTIVE_CODE_M_SEARCH,
    ATP_HTTP_DIRECTIVE_CODE_SUBSCRIBE,
    ATP_HTTP_DIRECTIVE_CODE_UNSUBSCRIBE
} ATP_UPNP_DIRECTIVE_CODE_EN;

typedef enum tagATP_UPNP_HEADER_IDX_EN
{
    ATP_HTTP_HEADER_NT                  = ATP_HTTP_HEADER_EXTEND,
    ATP_HTTP_HEADER_NTS,
    ATP_HTTP_HEADER_USN,
    ATP_HTTP_HEADER_MAN,
    ATP_HTTP_HEADER_MX,
    ATP_HTTP_HEADER_ST_TAG,
    ATP_HTTP_HEADER_EXT,
    ATP_HTTP_HEADER_TIMEOUT,
    ATP_HTTP_HEADER_CALLBACK,
    ATP_HTTP_HEADER_SID,
    ATP_HTTP_HEADER_OPT,
    ATP_HTTP_HEADER_NLS
} ATP_UPNP_HEADER_IDX_EN;

/*
*  UPnP/TR064设备、服务、动作结构定义
*/

/******************************************************************************
结构名称  : ATP_DEVICE_SERVICE_OWNER_EN
结构作用  : 指明服务或者设备属于UPnP还是TR064
调用函数  :

注意事项  :
******************************************************************************/
typedef enum tagATP_DEVICE_SERVICE_OWNER_EN
{
    ATP_DEVICE_SERVICE_OWNER_UPNP           = 0x01,
    ATP_DEVICE_SERVICE_OWNER_TR064          = 0x02,
    ATP_DEVICE_SERVICE_OWNER_ALL            = (ATP_DEVICE_SERVICE_OWNER_UPNP |ATP_DEVICE_SERVICE_OWNER_TR064),
} ATP_DEVICE_SERVICE_OWNER_EN;

typedef enum tagUPNP_START_STATUS
{
    UPNP_STATUS_UPNP_UP     = (1 << 1),
    UPNP_STATUS_UPNP_DOWN   = (~UPNP_STATUS_UPNP_UP),
    UPNP_STATUS_TR064_UP    = (1 << 2),
    UPNP_STATUS_TR064_DOWN  = (~UPNP_STATUS_TR064_UP),
}UPNP_START_STATUS;

/* UPnP服务句柄 */
struct tagATP_UPNP_SERVICE_ST;
typedef struct tagATP_UPNP_SERVICE_ST   ATP_UPNP_SERVICE_ST;

/* UPnP设备句柄 */
struct tagATP_UPNP_DEVICE_ST;
typedef struct tagATP_UPNP_DEVICE_ST    ATP_UPNP_DEVICE_ST;

/* UPnP Action句柄 */
struct tagATP_UPNP_ACTIONLIST_ST;
typedef struct tagATP_UPNP_ACTIONLIST_ST    ATP_UPNP_ACTIONLIST_ST;

/* UPnP参数改变通知结构句柄 */
struct tagATP_UPNP_EVENT_STRUCT_ST;
typedef struct tagATP_UPNP_EVENT_STRUCT_ST ATP_UPNP_EVENT_STRUCT_ST;

/* UPnP订阅句柄 */
struct tagATP_UPNP_SUBSCRIPTION_ST;
typedef struct tagATP_UPNP_SUBSCRIPTION_ST ATP_UPNP_SUBSCRIPTION_ST;

/******************************************************************************
结构名称  : ATP_UPNP_PAIR
结构作用  : 描述TR064参数及其值的对应关系
调用函数  : 
******************************************************************************/
typedef struct tgATP_UPNP_PAIR
{
    const VOS_CHAR* pszKey;
    const VOS_CHAR* pszValue;
}ATP_UPNP_PAIR;

/* 会话结束时的回调函数原型 */
typedef VOS_UINT32 (*PFUpnpOnActionEnd)(VOS_VOID *pvArg);

/******************************************************************************
结构名称  : ATP_UPNP_DEVICE_DESC_ST
结构作用  : 描述设备信息的结构
调用函数  :

注意事项  : 注册设备时，用于描述设备信息，根设备必须有该结构描述
******************************************************************************/
typedef struct tagATP_UPNP_DEVICE_DESC_ST
{
    VOS_CHAR            *pcManufacture;         // Required
    VOS_CHAR            *pcModelName;           // Required
    VOS_CHAR            *pcModelNumber;         // Recommended
    VOS_CHAR            *pcModelDescription;    // Recommended
    VOS_CHAR            *pcSerialNumber;        // Recommended
    VOS_CHAR            *pcModelURL;            // Optional
    VOS_CHAR            *pcManufactureUrl;      // Optional
    VOS_CHAR            *pcUPC;                 // Optional
} ATP_UPNP_DEVICE_DESC_ST;

/******************************************************************************
结构名称  : ATP_UPNP_ACTION_ST
结构作用  : 每个Action回调函数的结构参数
调用函数  : 无

注意事项  : 每个Action回调函数的参数，使用时，根据hAction获取输入参数，
处理完后，往hResponse中填充输出参数；
出错时，直接填充ulErrCode为相应的错误号，并且填充acErrStr
******************************************************************************/
#define ATP_UPNP_ERR_STRING_MAX_LEN     (32)

typedef struct tagATP_UPNP_ACTION_ST
{
    VOS_UINT32                      ulErrCode;
    VOS_CHAR                        acErrStr[ATP_UPNP_ERR_STRING_MAX_LEN];
    const VOS_CHAR                  *pcActionName;
    const ATP_UPNP_SERVICE_ST       *pstService;
    NODEPTR                         hAction; 
    NODEPTR                         hResponse;
    VOS_UINT32                      ulActionAttr;
#if defined(SUPPORT_ATP_UPNP_TR064) && defined(SUPPORT_ATP_TR064_SESSION)
    const VOS_CHAR                  *pcSessionID;
#endif
    struct addrinfo          *pstRemoteAddr;

} ATP_UPNP_ACTION_ST;

typedef struct tagTR064_NAME_VALUE_LIST_ST
{
    VOS_CHAR                            *pcName;
    const VOS_CHAR                      *pcValue;

    struct tagTR064_NAME_VALUE_LIST_ST  *pstNext;
} TR064_NAME_VALUE_LIST_ST;

typedef struct tagATP_UPNP_SUB_MSG_CTX_ST
{
    ATP_UPNP_SERVICE_ST* pstService;
    VOS_CHAR   acSubSID[ATP_UPNP_STR_LEN_TINY];
}ATP_UPNP_SUB_MSG_CTX_ST;

/* 每个Action处理函数的原型 */
typedef VOS_UINT32 (*PFAtpUpnpActionCb)(ATP_UPNP_ACTION_ST *pstAction);

typedef struct tagATP_UPNP_ACTION_ARRAY_ST
{
    const VOS_CHAR      *pszActionName;
    const VOS_CHAR      *pszSrvType;
    PFAtpUpnpActionCb   pfActionCallback;
    VOS_UINT32          ulActionAttr;
}ATP_UPNP_ACTION_ARRAY_ST;

typedef struct tagUPNP_END_ACTION_FUNC_LIST_ST
{
    PFUpnpOnActionEnd                       pfFunc;
    VOS_VOID                                *pvArg;

    struct tagUPNP_END_ACTION_FUNC_LIST_ST    *pstNext;
} UPNP_END_ACTION_FUNC_LIST_ST;

typedef struct tagATP_UPNP_STATVAR_ST
{
    const VOS_CHAR* pszStatVarName;
    VOS_BOOL  bSendEvt;
    const VOS_CHAR* pszDftValue;    /* 多数状态变量无需dftValue */
    const VOS_CHAR   *pszSVTR069Name;
} ATP_UPNP_STATVAR_ST;
typedef struct tagATP_UPNP_STACK_ST
{
    //VOS_VOID                    *pvTimer;
    //ATP_UTIL_TIMEREVENT_ST      *pstPeriod;

    VOS_UINT32                  ulStackWorkStatus;  // ref: UPNP_START_STATUS

    struct addrinfo              *pstLocalAddr;

    ATP_HTTP_SERVER_ST*         pstHttpServer;  // server st
    ATP_HTTP_CLIENT_ST*         pstHttpClient;  // client st
    ATP_HTTP_CLIENT_ST*         pstHttpClientIP6;  // client st
    VOS_UINT32                  ulSessionCnt;
    UPNP_END_ACTION_FUNC_LIST_ST *pstEndSessFuncList;


#if defined(SUPPORT_ATP_UPNP_TR064)
    const VOS_CHAR              *pcCfgUsername;
    const VOS_CHAR              *pcResetUsername;
    VOS_CHAR                    *pcCfgPassword;
    VOS_CHAR                    *pcResetPassword;
#endif
} ATP_UPNP_STACK_ST;

/*service type 到service ID的映射遵循标准，来自文件
upnp_igd_internetgatewaydevice1.0.pdf 2.2Device Model Table 1*/
typedef struct tagSERVICE_TYPE_TO_SERVICE_ID_ST
{
    const char      *pcServiceType;
    const char      *pcServiceID;
} SERVICE_TYPE_TO_SERVICE_ID_ST;
////////////////////////////////////////////////////////////////////////////////
//  Shm store Extension:                                                      //
//  ATP_UPNP_SHM_EXTENSION_ST stored in upnp shm, you can add something in it!//
////////////////////////////////////////////////////////////////////////////////
typedef struct tagATP_UPNP_PORTMAPPING_LIST_ST
{
    ATP_UPnP_PORTMAPPING_ST                 stPM;
    struct tagATP_UPNP_PORTMAPPING_LIST_ST* pstNext;
}ATP_UPNP_PORTMAPPING_LIST_ST;


typedef struct tagATP_UPNP_SEVICE_GVAR_ST
{
    //struct tagATP_CWMP_ENVELOPE_ST      *pstNext;
    struct tagATP_UPNP_SEVICE_GVAR_ST  *pstNext;
    VOS_CHAR                        *pcServiceId;/*pcServiceId和ulInstID可以唯一标识一service*/
    VOS_UINT32                      ulInstID;
    ATP_UPNP_SUBSCRIPTION_ST        *pstSubscriptionList;
} ATP_UPNP_SEVICE_GVAR_ST;
// the struct below is for extension, you can modify it, add something,
// this struct will be saved in share memory.
typedef struct tagATP_UPNP_SHM_EXTENSION_ST
{
    ATP_UPNP_PORTMAPPING_LIST_ST*   pstPortMapping;
    VOS_UINT32                      ulNextGenaExpireTime;
    VOS_UINT32                      ulNextSsdpTime;
#if defined (SUPPORT_ATP_IPV6)
    VOS_CHAR                        acNlsVal[40];
#endif    
    // add more if necessary
    //ATP_UPNP_SUBSCRIPTION_ST        *pstSubscriptionList;
    ATP_UPNP_SEVICE_GVAR_ST      *pstServiceList;
    struct addrinfo              *pstLocalAddr;
}ATP_UPNP_SHM_EXTENSION_ST;
typedef struct tagATP_UPNP_SEVICE_REG_ST
{
    struct tagATP_UPNP_SEVICE_REG_ST    *pstNext;      
    const VOS_CHAR                  *pcServiceType;
    const VOS_CHAR                  *pcScpdUrl;
    ATP_DEVICE_SERVICE_OWNER_EN     enServiceOwner;
    VOS_BOOL                        bMultipleInst;
    const VOS_CHAR                  *pcTR069Name;
    const ATP_UPNP_ACTION_ARRAY_ST *pstActionArray;
    const ATP_UPNP_STATVAR_ST         *pstStatVar;
}ATP_UPNP_SEVICE_REG_ST;

#define ATP_UPNP_MAX_SEVICE     6
#define ATP_UPNP_MAX_SEVICE     4
typedef struct tagATP_UPNP_DEVICE_REG_ST
{
    struct tagATP_UPNP_DEVICE_REG_ST    *pstNext;
    const VOS_CHAR                  *pcFriendlyName;
    const VOS_CHAR                  *pcDeviceType;
    ATP_DEVICE_SERVICE_OWNER_EN     enDeviceOwner;
    VOS_BOOL                        bMultipleInst;
    const VOS_CHAR                  *pcTR069Name;
    /*针对产品会限制某实例在tr064或upnp 用户的使用权限*/
    ATP_UPNP_SEVICE_REG_ST          *pstSeviceReg;
    struct tagATP_UPNP_DEVICE_REG_ST *pstChildDevieReg;
}ATP_UPNP_DEVICE_REG_ST;

extern ATP_UPNP_DEVICE_REG_ST   g_stIGDDevReg;
extern ATP_UPNP_DEVICE_REG_ST   g_stWANDevReg;
extern ATP_UPNP_DEVICE_REG_ST   g_stLANDevReg;
#ifdef SUPPORT_ATP_UPNP_DM_SERVER
extern ATP_UPNP_DEVICE_REG_ST   g_stManageableDevReg;
#endif
extern ATP_UPNP_DEVICE_REG_ST   g_stWANConnectionDevReg;   




/*!
  \brief root devie reg sevice
  \param[in]  *pstUpnpSevice: 
  \return 
*/
VOS_UINT32 ATP_UPNP_IGDRegRootSevice(ATP_UPNP_SEVICE_REG_ST *pstUpnpSevice);

/*!
  \brief root device reg sevice
  \param[in]  *pstChildDevice: 
  \return 
*/
VOS_UINT32 ATP_UPNP_IGDRegRootDevice(ATP_UPNP_DEVICE_REG_ST *pstChildDevice);

/*!
  \brief device reg sevice
  \param[in]  *pstDevice: 
  \param[in]  *pstUpnpSevice: 
  \return 
*/
VOS_UINT32 ATP_UPNP_IGDRegSevice(
                                          ATP_UPNP_DEVICE_REG_ST *pstDevice, 
                                          ATP_UPNP_SEVICE_REG_ST *pstUpnpSevice);

/*!
  \brief device reg child device
  \param[in]  *pstParenDevice: 
  \param[in]  *pstChildDevice: 
  \return 
*/
VOS_UINT32 ATP_UPNP_IGDRegDevice(
                                          ATP_UPNP_DEVICE_REG_ST *pstParenDevice, 
                                          ATP_UPNP_DEVICE_REG_ST *pstChildDevice);


extern const SERVICE_TYPE_TO_SERVICE_ID_ST g_astSerTypeToSerID[];
/*************************************************************/
extern ATP_UPNP_STACK_ST        *g_pstUPnPStack;
extern const VOS_CHAR           *g_pcDescPath;
extern VOS_UINT8                g_aucMacAddr[];
extern ATP_UPNP_DEVICE_DESC_ST  g_stDevDesc;
//extern const ATP_UPNP_ACTION_ARRAY_ST g_astActionArray[];

extern VOS_BOOL  g_bIPv6State;
extern struct addrinfo * g_pstLocalAddrIPv6;
extern VOS_CHAR  g_acNLSVal[40];
extern ATP_UPNP_SHM_EXTENSION_ST* g_pstShmExt;
extern const ATP_UPNP_ERRSTR_ST  *g_pstErrStrs;

VOS_UINT32 ATP_UPNP_Init(VOS_VOID);

VOS_VOID ATP_UPNP_AdaptUpdateNextTimer(VOS_UINT32 ulNextTime);
VOS_UINT32 ATP_UPNP_CheckExpiredSubscription(VOS_VOID);

////////////////////////////////////////////////////////////////////////////////
//  End of shm store Extension                                                //
////////////////////////////////////////////////////////////////////////////////

#if 1 /* 该功能尚未实现 */
/******************************************************************************
函数名称  : ATP_UPnP_RegIcon
功能描述  : 为UPnP设备注册图标
输入参数  :
1. pstParentDev   :   UPnP设备结构指针，不允许为空
2. pcMimeType     :   图标的MIME类型，不允许为空
3. pcPath         :   图标的路径，如果为绝对路径，在必须以'/'开头
4. lHeight        :   图标的高度
5. lWidth         :   图标的宽度
6. lDepth         :   图标的颜色深度
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-7
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPnP_RegIcon(
    const VOS_CHAR                  *pcMimeType,
    const VOS_CHAR                  *pcPath,
    VOS_INT32                       lHeight,
    VOS_INT32                       lWidth,
    VOS_INT32                       lDepth);
#endif


/* Action属性枚举值 */
typedef enum tagATP_UPNP_ACTION_ATTR_EN
{
    ATP_UPNP_ACTION_NEED_NONTHING   = (0),
    ATP_UPNP_ACTION_NEED_AUTH       = (1 << 0),
    ATP_UPNP_ACTION_NEED_HTTPS      = (1 << 1),
    ATP_UPNP_ACTION_NOT_SPRT_TR064  = (1 << 2),
    ATP_UPNP_ACTION_NOT_SPRT_UPNP   = (1 << 3),
    ATP_UPNP_ACTION_NEED_DISABLE    = (1 << 4), /* 不要改变前面的顺序 */
    ATP_UPNP_ACTION_NOT_GET    = (1 << 5), /*bt  用了, Access to all UPnP Setters must not be allowed for upnp*/
    //ATP_UPNP_ACTION_SET    = (1 << 6), /*  bt 规定upnp 不能set ,tr064 没有限制*/
    ATP_UPNP_ACTION_DFT_ACTION      = (1 << 30),
    ATP_UPNP_ACTION_NEED_END        = (1 << 31),
} ATP_UPNP_ACTION_ATTR_EN;


/******************************************************************************
函数名称  : ATP_UPNP_DestroyAllDevices
功能描述  : 销毁所有注册的设备、服务和Action
输入参数  :
无
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-1-8
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_VOID ATP_UPNP_DestroyAllDevices();

VOS_UINT32 ATP_UPNP_InitDevice();


/*
*  事件接口
*/

/******************************************************************************
函数名称  : ATP_UPNP_NeedToEvent
功能描述  : 判断当前服务是否有控制点注册事件通知
输入参数  :
1. pstService :   服务结构指针
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-7
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_BOOL ATP_UPNP_NeedToEvent(const ATP_UPNP_SERVICE_ST *pstService);

/******************************************************************************
函数名称  : ATP_UPnP_AddEvent
功能描述  : 添加新的事件上报状态变量
输入参数  :

调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-7
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_AddEvent(ATP_UPNP_EVENT_STRUCT_ST        **ppstEvtList,
    const VOS_CHAR                  *pcEvtName,
    const VOS_CHAR                  *pcEvtValue,
    ATP_UPNP_EVENT_STRUCT_ST        **ppstNewEvt);

/******************************************************************************
函数名称  : ATP_UPNP_SendEvent
功能描述  : 发送事件通知
输入参数  :
1. pstService :   服务结构指针
2. pstEvtData :   事件数据
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-7
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_SendEvent(
    ATP_UPNP_SERVICE_ST             *pstService,
    ATP_UPNP_EVENT_STRUCT_ST        *pstEvtData);

/******************************************************************************
函数名称  : freeEvent
功能描述  : 释放事件结构
输入参数  :
1. pstEvtList :   事件链表
调用函数  :
被调函数  :
输出参数  : 
返 回 值  : VOS_VOID

修改历史      :
1.日    期   : 2008-03-18
作    者   : panjinling
修改内容   : 完成初稿
******************************************************************************/
VOS_VOID ATP_UPNP_FreeEvent(ATP_UPNP_EVENT_STRUCT_ST* pstEvtList);


/*
*  配置接口
*/

/******************************************************************************
函数名称  : ATP_UPNP_SetLocalAddr
功能描述  : 更新LAN口的IP地址
输入参数  :
1. pstLocalAddr    :   本地IP地址，网络字节序
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-1-8
作    者   : handy
修改内容   : 完成初稿
2. p42047 r3 modify.

******************************************************************************/
VOS_UINT32 ATP_UPNP_SetLocalAddr(const struct addrinfo *pstLocalAddr);

#if defined(SUPPORT_ATP_UPNP_TR064)
VOS_UINT32 ATP_UPNP_GetTr064SecurityInfo(VOS_CHAR** ppszCfgPwd, VOS_CHAR** ppszResetPwd);

VOS_UINT32 ATP_UPNP_SetTr064SecurityInfo(const VOS_CHAR* pszCfgPwd, const VOS_CHAR* pszResetPwd);
#endif
/******************************************************************************
函数名称  : ATP_UPNP_RegEndActionFunc
功能描述  : 注册Action结束后的处理函数
输入参数  :
1. pfFunc :   注册的回调函数指针
2. pvArg  :   回调函数的参数，注册者分配内存，并在回调函数中释放
不关心时，可以为空
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-2-3
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_RegEndActionFunc(
    PFUpnpOnActionEnd           pfFunc,
    VOS_VOID                    *pvArg);

#if defined(SUPPORT_ATP_UPNP_TR064) && defined(SUPPORT_ATP_TR064_SESSION)
/******************************************************************************
函数名称  : ATP_UPNP_LockTr064Device
功能描述  : 锁定TR064设备，支持TR064时，才使用本接口
锁定后，除非有相同的SessionID，否则，协议栈不接受配置
输入参数  :
1. pcSessionID:
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-2-3
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_LockTr064Device(const VOS_CHAR *pcSessionID);

/******************************************************************************
函数名称  : ATP_UPNP_UnLockTr064Device
功能描述  : 解锁TR064设备，支持TR064时，才使用本接口
解锁后，任何设备都可以继续配置
输入参数  :
1. pstAction:
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-2-3
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_UnLockTr064Device(const ATP_UPNP_ACTION_ST *pstAction);

#endif

/*
*  配置架构相关接口
*/



/******************************************************************************
函数名称  : ATP_UPNP_GetDomainNameOfService
功能描述  : 根据服务获取其TR069参数模型中的对象域名
输入参数  :
1. pstService     :   服务句柄
2. pcNameBuf      :   存放域名的缓冲区指针
3. ulBufLen       :   缓冲区长度
调用函数  :
被调函数  :
输出参数  :
1. pulLen         :   域名的实际长度，不关心该值时，可以为空
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-16
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_GetDomainNameOfService(
    const ATP_UPNP_SERVICE_ST   *pstService,
    VOS_CHAR                    *pcNameBuf,
    VOS_UINT32                  ulBufLen,
    VOS_UINT32                  *pulLen);


/*
*  辅助接口
*/
/******************************************************************************
函数名称  : ATP_UPnP_BroadcastSSDP
功能描述  : 广播SSDP报文
输入参数  :
1. bAlive         :   发送alive还是byebye消息
2. bUpnpDev   :   UPnP广播还是TR064广播
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-7
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_BroadcastSSDP(VOS_BOOL      bAlive,
    VOS_BOOL      bUpnpDev);
VOS_UINT32 ATP_UPNP_BroadcastSSDPEx(VOS_BOOL      bAlive,
    VOS_BOOL      bUpnpDev);

/******************************************************************************
函数名称  : ATP_UPNP_ForceBroadcast
功能描述  : 强制执行设备广播
输入参数  :
1. bAlive :   VOS_TRUE: 上线广播；VOS_FALSE: 下线广播
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2008-1-8
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_ForceBroadcast(VOS_BOOL             bAlive);

VOS_VOID ATP_UPNP_RefreshGvarServiceList(ATP_UPNP_SEVICE_GVAR_ST      **ppstServiceList);
/******************************************************************************
函数名称  : ATP_UPNP_RefreshDeivce
功能描述  : 更新UPnP设备，动态注册了服务或者对象后，需要调用该接口来更新设备
输入参数  :
1. bUPnPOrTR064   :   UPnP还是TR064设备
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-12
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_RefreshDeivce(VOS_BOOL              bUPnPOrTR064);

/******************************************************************************
函数名称  : ATP_XML_GetChildNodeByName
功能描述  : 操作XML组件的一个公用函数，用于根据节点名查找子节点
输入参数  :
1. hRoot      : 父节点，不能为空
2. pcName     : 待查找的子节点名称，不能为空
3. hOutNode   : 如果不为空，则存放查找到的子节点局部指针
4. ppcValue   : 如果不为空，则存放查找到的子节点的值，不能释放
调用函数  :
被调函数  :
输出参数  : 无
返 回 值  : 无

修改历史      :
1.日    期   : 2007-11-10
作    者   : handy
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_XML_GetChildNodeByName(NODEPTR        hRoot,
    const VOS_CHAR *pcName,
    NODEPTR        *hOutNode,
    const VOS_CHAR **ppcValue);


/*****************************************************************************
函 数 名  : ATP_UPNP_GetVersion
功能描述  : 
输入参数  : 无
输出参数  : 无
返 回 值  : 返回组件版本号

修改历史      :
1.日    期   : 2009年4月20日
作    者   : 俞佑清42304
修改内容   : 新生成函数

*****************************************************************************/
VOS_CHAR * ATP_UPNP_GetVersion(VOS_VOID);

/* 该版本暂不支持 */
typedef VOS_UINT32 (*PFUpnpOnSubscribeEnd)(ATP_UPNP_SUB_MSG_CTX_ST* pstSubCtx,
    VOS_VOID* pvArg);
/* 该版本暂不支持 */
VOS_VOID ATP_UPNP_RegEndSubscribeFunc(PFUpnpOnSubscribeEnd pfFunc, VOS_VOID* pvArg);

/******************************************************************************
函数名称  : ATP_UPNP_GetServiceByDomain
功能描述  : 根据域名(tr064/tr069点分域名)定位到服务并创建事件结构
输入参数  : 
1. pszDomain : 域名字符串。
2. pszValue : 域名对应的值
调用函数  :
被调函数  :
输出参数  : 
1. ppstService : 找到的服务结构
2. ppstEvt : 生成的事件结构
返 回 值  : VOS_OK/错误码

修改历史      :
1.日    期   : 2008-04-13
作    者   : panjinling
修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_UPNP_GetServiceByDomain(const VOS_CHAR* pszDomain,
    const VOS_CHAR* pszValue,
    ATP_UPNP_SERVICE_ST** ppstService,
    ATP_UPNP_EVENT_STRUCT_ST** ppstEvt);



//VOS_UINT32 ATP_UPNP_RegDeviceAndService(VOS_VOID);
VOS_UINT32 ATP_UPnP_DftActionSetWithDiffName(
    ATP_UPNP_ACTION_ST  *pstAction,
    const VOS_UINT32        *pulKeyList,
    const VOS_CHAR      *pcDomainName,
    const VOS_CHAR      * const apcUPnPNames[],
    const VOS_CHAR      * const apcTR069Names[],
    const VOS_CHAR      * const apcDftValues[],
    const VOS_UINT32           *ulParOID ,
    VOS_UINT32           ulNum);
VOS_UINT32 ATP_UPnP_DftActionGetWithDiffName(
    ATP_UPNP_ACTION_ST  *pstAction,
    VOS_CHAR      *pcDomainName,
    const VOS_CHAR       * const apcUPnPNames[],
    const VOS_CHAR       * const apcTR069Names[],
    const VOS_CHAR       * const apcDftValues[],
    VOS_UINT32          ulNum);
VOS_UINT32 ATP_UPnP_DftActionGetWithDiffIdx(
    const ATP_UPNP_ACTION_ST  *pstAction,
    const VOS_UINT32                 *pulObj,
    const VOS_CHAR          * const apcUPnPNames[],
    const VOS_UINT32        *apulUPnPParIdx,
    const VOS_CHAR          * const apcDftValues[],
    VOS_UINT32                  ulNum);
VOS_CHAR *SysIconvConvert (const VOS_CHAR *pszInput);
VOS_BOOL ATP_UTIL_IsStringUTF8(const VOS_CHAR* pszStr);
VOS_VOID upnpInitAllGermanChars(const VOS_CHAR* pszStr);
VOS_BOOL ATP_UPnP_IsValidHexStr(const VOS_CHAR *pcStr);
VOS_UINT32 ATP_UPnP_CheckStrLen(const VOS_CHAR *pcStr,
    VOS_UINT32 ulMinLen, VOS_UINT32 ulMaxLen);
VOS_BOOL ATP_UPnP_CheckStrInAllowedChars(const VOS_CHAR *pcStr,
    const VOS_CHAR *pcAllowedChars);
VOS_UINT32 upnpPataCheck(const VOS_CHAR *pszTmp, const VOS_CHAR *pcAllowedChars, VOS_UINT32 ulMinLen, VOS_UINT32 ulMaxLen);

char *ff_data_to_hex(char *buff, const VOS_UINT8 *src, int s);

VOS_UINT32 UpnpDoGetAction(const ATP_UPNP_ACTION_ST *pstAction,
    VOS_CHAR * const *pstrParaName,
    VOS_UINT8 ucParaNum);

VOS_UINT32 ATP_UPNP_GetUpnpServiceId( const ATP_UPNP_SERVICE_ST* pstService );

/*upnpDoSetActionEx ,如果想用pstrParaValue中的value值替代pstAction中的value设置相关参数,请让pstrParaValue非空
pstrParaValue 非空, pstrParaValue和 pstrParaName必须是等长的指针数组, ucParaNum表示他们的长度
pstrParaValue[i] != NULL, 表明, pstrParaValue[i]为name为pstrParaName[i] 的值,
pstrParaValue[i] == NULL  用pstAction ->request中的值*/
VOS_UINT32 upnpDoSetActionEx(ATP_UPNP_ACTION_ST  *pstAction,
    VOS_CHAR * const *pstrParaName,
    VOS_CHAR * const *pstrParaValue,
    VOS_UINT8 ucParaNum);
VOS_UINT32 upnpDoSetAction(ATP_UPNP_ACTION_ST  *pstAction,
    VOS_CHAR * const *pstrParaName,
    VOS_UINT8 ucParaNum);
VOS_UINT32 upnpBuildResponseByNameAndValue(
    NODEPTR          hRpcResponse,
    const VOS_CHAR   *pcParaName,
    const VOS_CHAR   *pcParaValue);

VOS_UINT32 upnpGetParaValueInSoap(ATP_UPNP_ACTION_ST *pstAction,
    const VOS_CHAR     *pcParaName,
    VOS_CHAR           **pcParaValue);
VOS_UINT32 unpnGetValueByDomainName(const VOS_CHAR *pcDomainName,
    VOS_CHAR   **pcValue);
VOS_UINT32 upnpServerOnData(ATP_HTTP_CLIENT_ST      *pstHttpClient,
    VOS_VOID                *pvHook);

extern VOS_VOID*           g_pvUpnpShmHandle;

#if defined(SUPPORT_ATP_MEM_CHECK) 
#define ATP_UPNP_GvarMalloc(ulLen)      ATP_UTIL_BgetMallocEx(g_pvUpnpShmHandle, ulLen, __FILE__, __LINE__)
#define ATP_UPNP_GvarFree(pvMem)        ATP_UTIL_BgetFreeEx(g_pvUpnpShmHandle, pvMem)
#define ATP_UPNP_GvarStrDup(pszString)  ATP_UTIL_BgetStrdupEx(g_pvUpnpShmHandle, pszString, pszFile, ulLine)
#else
VOS_VOID* ATP_UPNP_GvarMalloc(VOS_UINT32 ulLen);
VOS_VOID ATP_UPNP_GvarFree(VOS_VOID* pvMem);
VOS_VOID* ATP_UPNP_GvarStrDup(const VOS_CHAR* pszString);
#endif


extern VOS_UINT32 UpnpSsdpSocketProc(ATP_HTTP_CLIENT_ST *pstHttpClient);
extern VOS_VOID upnpServerOnError(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_UINT32 ulErrCode);

typedef VOS_UINT32 (*PfUpnpActionPreProc)(ATP_HTTP_CLIENT_ST* pstHttpClient,
                            ATP_UPNP_ACTION_ST* pstAction);

extern PfUpnpActionPreProc pfUpnpActionPreProc;

VOS_VOID ATP_ACTION_DeleteLegacyPortMapping(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 UpnpGetParaStatVar(const ATP_UPNP_SERVICE_ST       *pstService, 
                                        const VOS_CHAR *pstrParaName, 
                                        ATP_UPNP_STATVAR_ST **ppstStatVar);

const VOS_CHAR * ATP_UPNPAPI_GetVersion(void);

typedef VOS_UINT32 (*PFATPUPnPActionEndProc)(ATP_UPNP_ACTION_ST *pstAction, ATP_HTTP_CLIENT_ST* pstHttpClient);
extern PFATPUPnPActionEndProc  g_pfActionEndProc;

// DEBUG MACRO
#ifdef ATP_DEBUG
#define ATP_UPNP_DEBUG(format, args...)  {printf("UPNP DEBUG===file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_UPNP_DEBUG(format, args...)
#endif

#ifdef __cplusplus
}
#endif /* __cpluscplus */


#endif

