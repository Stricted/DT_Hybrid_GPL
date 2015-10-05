/******************************************************************************
��Ȩ����  : 2007-2020����Ϊ�������޹�˾
�� �� ��  : upnp.h
��    ��  : handy
��    ��  : V1.0
��������  : 2008-2-3
��    ��  : UPnP/TR064ģ��ӿ�
�����б�  :

��ʷ��¼      :
1.��    ��   : 2008-2-3
��    ��   : handy
�޸�����   : ��ɳ���

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
*  �����궨��
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
*  Э��ջ��֧�ֵ�UPnP����SOAP������
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
*  UPnP/TR064Э��ջ�ӿڷ��صĴ�����
*  ���нӿ�VOS_UINT32���͵ķ�����ɹ�ʱ��������VOS_OK
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
*  UPnP/TR064�豸�����񡢶����ṹ����
*/

/******************************************************************************
�ṹ����  : ATP_DEVICE_SERVICE_OWNER_EN
�ṹ����  : ָ����������豸����UPnP����TR064
���ú���  :

ע������  :
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

/* UPnP������ */
struct tagATP_UPNP_SERVICE_ST;
typedef struct tagATP_UPNP_SERVICE_ST   ATP_UPNP_SERVICE_ST;

/* UPnP�豸��� */
struct tagATP_UPNP_DEVICE_ST;
typedef struct tagATP_UPNP_DEVICE_ST    ATP_UPNP_DEVICE_ST;

/* UPnP Action��� */
struct tagATP_UPNP_ACTIONLIST_ST;
typedef struct tagATP_UPNP_ACTIONLIST_ST    ATP_UPNP_ACTIONLIST_ST;

/* UPnP�����ı�֪ͨ�ṹ��� */
struct tagATP_UPNP_EVENT_STRUCT_ST;
typedef struct tagATP_UPNP_EVENT_STRUCT_ST ATP_UPNP_EVENT_STRUCT_ST;

/* UPnP���ľ�� */
struct tagATP_UPNP_SUBSCRIPTION_ST;
typedef struct tagATP_UPNP_SUBSCRIPTION_ST ATP_UPNP_SUBSCRIPTION_ST;

/******************************************************************************
�ṹ����  : ATP_UPNP_PAIR
�ṹ����  : ����TR064��������ֵ�Ķ�Ӧ��ϵ
���ú���  : 
******************************************************************************/
typedef struct tgATP_UPNP_PAIR
{
    const VOS_CHAR* pszKey;
    const VOS_CHAR* pszValue;
}ATP_UPNP_PAIR;

/* �Ự����ʱ�Ļص�����ԭ�� */
typedef VOS_UINT32 (*PFUpnpOnActionEnd)(VOS_VOID *pvArg);

/******************************************************************************
�ṹ����  : ATP_UPNP_DEVICE_DESC_ST
�ṹ����  : �����豸��Ϣ�Ľṹ
���ú���  :

ע������  : ע���豸ʱ�����������豸��Ϣ�����豸�����иýṹ����
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
�ṹ����  : ATP_UPNP_ACTION_ST
�ṹ����  : ÿ��Action�ص������Ľṹ����
���ú���  : ��

ע������  : ÿ��Action�ص������Ĳ�����ʹ��ʱ������hAction��ȡ���������
���������hResponse��������������
����ʱ��ֱ�����ulErrCodeΪ��Ӧ�Ĵ���ţ��������acErrStr
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

/* ÿ��Action��������ԭ�� */
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
    const VOS_CHAR* pszDftValue;    /* ����״̬��������dftValue */
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

/*service type ��service ID��ӳ����ѭ��׼�������ļ�
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
    VOS_CHAR                        *pcServiceId;/*pcServiceId��ulInstID����Ψһ��ʶһservice*/
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
    /*��Բ�Ʒ������ĳʵ����tr064��upnp �û���ʹ��Ȩ��*/
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

#if 1 /* �ù�����δʵ�� */
/******************************************************************************
��������  : ATP_UPnP_RegIcon
��������  : ΪUPnP�豸ע��ͼ��
�������  :
1. pstParentDev   :   UPnP�豸�ṹָ�룬������Ϊ��
2. pcMimeType     :   ͼ���MIME���ͣ�������Ϊ��
3. pcPath         :   ͼ���·�������Ϊ����·�����ڱ�����'/'��ͷ
4. lHeight        :   ͼ��ĸ߶�
5. lWidth         :   ͼ��Ŀ��
6. lDepth         :   ͼ�����ɫ���
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-7
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPnP_RegIcon(
    const VOS_CHAR                  *pcMimeType,
    const VOS_CHAR                  *pcPath,
    VOS_INT32                       lHeight,
    VOS_INT32                       lWidth,
    VOS_INT32                       lDepth);
#endif


/* Action����ö��ֵ */
typedef enum tagATP_UPNP_ACTION_ATTR_EN
{
    ATP_UPNP_ACTION_NEED_NONTHING   = (0),
    ATP_UPNP_ACTION_NEED_AUTH       = (1 << 0),
    ATP_UPNP_ACTION_NEED_HTTPS      = (1 << 1),
    ATP_UPNP_ACTION_NOT_SPRT_TR064  = (1 << 2),
    ATP_UPNP_ACTION_NOT_SPRT_UPNP   = (1 << 3),
    ATP_UPNP_ACTION_NEED_DISABLE    = (1 << 4), /* ��Ҫ�ı�ǰ���˳�� */
    ATP_UPNP_ACTION_NOT_GET    = (1 << 5), /*bt  ����, Access to all UPnP Setters must not be allowed for upnp*/
    //ATP_UPNP_ACTION_SET    = (1 << 6), /*  bt �涨upnp ����set ,tr064 û������*/
    ATP_UPNP_ACTION_DFT_ACTION      = (1 << 30),
    ATP_UPNP_ACTION_NEED_END        = (1 << 31),
} ATP_UPNP_ACTION_ATTR_EN;


/******************************************************************************
��������  : ATP_UPNP_DestroyAllDevices
��������  : ��������ע����豸�������Action
�������  :
��
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-1-8
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_VOID ATP_UPNP_DestroyAllDevices();

VOS_UINT32 ATP_UPNP_InitDevice();


/*
*  �¼��ӿ�
*/

/******************************************************************************
��������  : ATP_UPNP_NeedToEvent
��������  : �жϵ�ǰ�����Ƿ��п��Ƶ�ע���¼�֪ͨ
�������  :
1. pstService :   ����ṹָ��
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-7
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_BOOL ATP_UPNP_NeedToEvent(const ATP_UPNP_SERVICE_ST *pstService);

/******************************************************************************
��������  : ATP_UPnP_AddEvent
��������  : ����µ��¼��ϱ�״̬����
�������  :

���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-7
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_AddEvent(ATP_UPNP_EVENT_STRUCT_ST        **ppstEvtList,
    const VOS_CHAR                  *pcEvtName,
    const VOS_CHAR                  *pcEvtValue,
    ATP_UPNP_EVENT_STRUCT_ST        **ppstNewEvt);

/******************************************************************************
��������  : ATP_UPNP_SendEvent
��������  : �����¼�֪ͨ
�������  :
1. pstService :   ����ṹָ��
2. pstEvtData :   �¼�����
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-7
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_SendEvent(
    ATP_UPNP_SERVICE_ST             *pstService,
    ATP_UPNP_EVENT_STRUCT_ST        *pstEvtData);

/******************************************************************************
��������  : freeEvent
��������  : �ͷ��¼��ṹ
�������  :
1. pstEvtList :   �¼�����
���ú���  :
��������  :
�������  : 
�� �� ֵ  : VOS_VOID

�޸���ʷ      :
1.��    ��   : 2008-03-18
��    ��   : panjinling
�޸�����   : ��ɳ���
******************************************************************************/
VOS_VOID ATP_UPNP_FreeEvent(ATP_UPNP_EVENT_STRUCT_ST* pstEvtList);


/*
*  ���ýӿ�
*/

/******************************************************************************
��������  : ATP_UPNP_SetLocalAddr
��������  : ����LAN�ڵ�IP��ַ
�������  :
1. pstLocalAddr    :   ����IP��ַ�������ֽ���
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-1-8
��    ��   : handy
�޸�����   : ��ɳ���
2. p42047 r3 modify.

******************************************************************************/
VOS_UINT32 ATP_UPNP_SetLocalAddr(const struct addrinfo *pstLocalAddr);

#if defined(SUPPORT_ATP_UPNP_TR064)
VOS_UINT32 ATP_UPNP_GetTr064SecurityInfo(VOS_CHAR** ppszCfgPwd, VOS_CHAR** ppszResetPwd);

VOS_UINT32 ATP_UPNP_SetTr064SecurityInfo(const VOS_CHAR* pszCfgPwd, const VOS_CHAR* pszResetPwd);
#endif
/******************************************************************************
��������  : ATP_UPNP_RegEndActionFunc
��������  : ע��Action������Ĵ�����
�������  :
1. pfFunc :   ע��Ļص�����ָ��
2. pvArg  :   �ص������Ĳ�����ע���߷����ڴ棬���ڻص��������ͷ�
������ʱ������Ϊ��
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-2-3
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_RegEndActionFunc(
    PFUpnpOnActionEnd           pfFunc,
    VOS_VOID                    *pvArg);

#if defined(SUPPORT_ATP_UPNP_TR064) && defined(SUPPORT_ATP_TR064_SESSION)
/******************************************************************************
��������  : ATP_UPNP_LockTr064Device
��������  : ����TR064�豸��֧��TR064ʱ����ʹ�ñ��ӿ�
�����󣬳�������ͬ��SessionID������Э��ջ����������
�������  :
1. pcSessionID:
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-2-3
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_LockTr064Device(const VOS_CHAR *pcSessionID);

/******************************************************************************
��������  : ATP_UPNP_UnLockTr064Device
��������  : ����TR064�豸��֧��TR064ʱ����ʹ�ñ��ӿ�
�������κ��豸�����Լ�������
�������  :
1. pstAction:
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-2-3
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_UnLockTr064Device(const ATP_UPNP_ACTION_ST *pstAction);

#endif

/*
*  ���üܹ���ؽӿ�
*/



/******************************************************************************
��������  : ATP_UPNP_GetDomainNameOfService
��������  : ���ݷ����ȡ��TR069����ģ���еĶ�������
�������  :
1. pstService     :   ������
2. pcNameBuf      :   ��������Ļ�����ָ��
3. ulBufLen       :   ����������
���ú���  :
��������  :
�������  :
1. pulLen         :   ������ʵ�ʳ��ȣ������ĸ�ֵʱ������Ϊ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-16
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_GetDomainNameOfService(
    const ATP_UPNP_SERVICE_ST   *pstService,
    VOS_CHAR                    *pcNameBuf,
    VOS_UINT32                  ulBufLen,
    VOS_UINT32                  *pulLen);


/*
*  �����ӿ�
*/
/******************************************************************************
��������  : ATP_UPnP_BroadcastSSDP
��������  : �㲥SSDP����
�������  :
1. bAlive         :   ����alive����byebye��Ϣ
2. bUpnpDev   :   UPnP�㲥����TR064�㲥
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-7
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_BroadcastSSDP(VOS_BOOL      bAlive,
    VOS_BOOL      bUpnpDev);
VOS_UINT32 ATP_UPNP_BroadcastSSDPEx(VOS_BOOL      bAlive,
    VOS_BOOL      bUpnpDev);

/******************************************************************************
��������  : ATP_UPNP_ForceBroadcast
��������  : ǿ��ִ���豸�㲥
�������  :
1. bAlive :   VOS_TRUE: ���߹㲥��VOS_FALSE: ���߹㲥
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2008-1-8
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_ForceBroadcast(VOS_BOOL             bAlive);

VOS_VOID ATP_UPNP_RefreshGvarServiceList(ATP_UPNP_SEVICE_GVAR_ST      **ppstServiceList);
/******************************************************************************
��������  : ATP_UPNP_RefreshDeivce
��������  : ����UPnP�豸����̬ע���˷�����߶������Ҫ���øýӿ��������豸
�������  :
1. bUPnPOrTR064   :   UPnP����TR064�豸
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-12
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_UPNP_RefreshDeivce(VOS_BOOL              bUPnPOrTR064);

/******************************************************************************
��������  : ATP_XML_GetChildNodeByName
��������  : ����XML�����һ�����ú��������ڸ��ݽڵ��������ӽڵ�
�������  :
1. hRoot      : ���ڵ㣬����Ϊ��
2. pcName     : �����ҵ��ӽڵ����ƣ�����Ϊ��
3. hOutNode   : �����Ϊ�գ����Ų��ҵ����ӽڵ�ֲ�ָ��
4. ppcValue   : �����Ϊ�գ����Ų��ҵ����ӽڵ��ֵ�������ͷ�
���ú���  :
��������  :
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
1.��    ��   : 2007-11-10
��    ��   : handy
�޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_XML_GetChildNodeByName(NODEPTR        hRoot,
    const VOS_CHAR *pcName,
    NODEPTR        *hOutNode,
    const VOS_CHAR **ppcValue);


/*****************************************************************************
�� �� ��  : ATP_UPNP_GetVersion
��������  : 
�������  : ��
�������  : ��
�� �� ֵ  : ��������汾��

�޸���ʷ      :
1.��    ��   : 2009��4��20��
��    ��   : ������42304
�޸�����   : �����ɺ���

*****************************************************************************/
VOS_CHAR * ATP_UPNP_GetVersion(VOS_VOID);

/* �ð汾�ݲ�֧�� */
typedef VOS_UINT32 (*PFUpnpOnSubscribeEnd)(ATP_UPNP_SUB_MSG_CTX_ST* pstSubCtx,
    VOS_VOID* pvArg);
/* �ð汾�ݲ�֧�� */
VOS_VOID ATP_UPNP_RegEndSubscribeFunc(PFUpnpOnSubscribeEnd pfFunc, VOS_VOID* pvArg);

/******************************************************************************
��������  : ATP_UPNP_GetServiceByDomain
��������  : ��������(tr064/tr069�������)��λ�����񲢴����¼��ṹ
�������  : 
1. pszDomain : �����ַ�����
2. pszValue : ������Ӧ��ֵ
���ú���  :
��������  :
�������  : 
1. ppstService : �ҵ��ķ���ṹ
2. ppstEvt : ���ɵ��¼��ṹ
�� �� ֵ  : VOS_OK/������

�޸���ʷ      :
1.��    ��   : 2008-04-13
��    ��   : panjinling
�޸�����   : ��ɳ���

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

/*upnpDoSetActionEx ,�������pstrParaValue�е�valueֵ���pstAction�е�value������ز���,����pstrParaValue�ǿ�
pstrParaValue �ǿ�, pstrParaValue�� pstrParaName�����ǵȳ���ָ������, ucParaNum��ʾ���ǵĳ���
pstrParaValue[i] != NULL, ����, pstrParaValue[i]ΪnameΪpstrParaName[i] ��ֵ,
pstrParaValue[i] == NULL  ��pstAction ->request�е�ֵ*/
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

