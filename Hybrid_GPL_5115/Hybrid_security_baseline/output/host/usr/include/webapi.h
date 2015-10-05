#ifndef __ATP_WEB_API_H__
#define __ATP_WEB_API_H__

#include "atptypes.h"
#include "msgapi.h"
#include "httpapi.h"
/* <DTS2012080607642 z81004485 2012/8/18 begin  */
#include "cfmapi.h"
/* DTS2012080607642 z81004485 2012/8/18 end> */




/* Utility functions */

typedef enum tagATP_WEB_RET_CODE_EN
{
    // These errors are reserved for web stack only
    ATP_WEB_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_WEB),
    ATP_WEB_RET_MEM_ERR,
    ATP_WEB_RET_NOT_INITED,
    ATP_WEB_RET_SOCK_ERR,
    ATP_WEB_RET_NOT_SPPORTED,
    ATP_WEB_RET_NOT_FOUND,      // = 5
    ATP_WEB_RET_INVALID_DEST,
    ATP_WEB_RET_NOTMATCH_CGI,
    ATP_WEB_RET_CGI_FORMAT_ERR,
    ATP_WEB_RET_ASP_FORMAT_ERR,
    ATP_WEB_RET_INTERNAL_ERR,   // = A
    ATP_WEB_RET_VALUE_TYPE_ERR,
    ATP_WEB_RET_HTTP_PACK,
    ATP_WEB_RET_AUTH_ERR,
    ATP_WEB_RET_FILE_NOT_FOUND,     
    ATP_WEB_RET_FILE_OVERFLOW,    // = F
    ATP_WEB_RET_FILTER_STATUS,  /* filter return http status */

    // For user readable error msgs
    // Please add err at the end of this enumeration
    ATP_WEB_RET_DFT_ERR_MSG     = (ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_WEB) + 100),
    ATP_WEB_RET_INVALID_USERNAME,
    ATP_WEB_RET_INVALID_PASSWORD,
    ATP_WEB_RET_LOGIN_WAIT,
    ATP_WEB_RET_DUP_LOGIN,
    ATP_WEB_RET_TOO_MANY_USERS,
    ATP_WEB_RET_VOICE_BUSY,
    ATP_WEB_RET_FILE_TOO_LARGE,
    ATP_WEB_RET_FILE_FORMAT_ERR,
    ATP_WEB_RET_ACCESS_DENIED,
    ATP_WEB_RET_SAMBA_PATH_NOT_STORAGE,
    ATP_WEB_RET_SAMBA_USER_BUSY,
    ATP_WEB_RET_INVALID_CERT,
    ATP_WEB_RET_OLDPASSWD_NOT_MATCH,
    ATP_WEB_RET_CHANGE_SUCCESS,
    ATP_WEB_RET_FILE_EXIST,
    ATP_WEB_RET_NO_USB_DEVICE,
    ATP_WEB_RET_OSGI_EE_DISABLE,
    ATP_WEB_RET_FILE_NOT_EXIST,
    ATP_WEB_RET_EMAILSEND_ERROR,
    ATP_WEB_RET_CFM_LOCKED,
    ATP_WEB_RET_CFGFILE_TOO_LARGE,
    ATP_WEB_RET_CFGFILE_FORMAT_ERR,
    ATP_WEB_RET_CFGVOICE_BUSY,
    ATP_WEB_RET_FILE_EMPTY_ERR,
} ATP_WEB_RET_CODE_EN;
/*********************h00190880*******2012/3/28***************/
#ifdef SUPPORT_ATP_VOICE
#define ATP_WEB_RSS_MAX_NUM             (10)

#define ATP_WEB_CGI_VDB_INDEX           "id"

#define ATP_WEB_CGI_VDB_NAME            "phonebook_name"
#define ATP_WEB_CGI_VDB_VORNAME         "phonebook_vorname"
#define ATP_WEB_CGI_VDB_PRIVATENUM      "phonebook_number_p"
#define ATP_WEB_CGI_VDB_NUM1FLAGS       "num1Flags"
#define ATP_WEB_CGI_VDB_COMPANYNUM      "phonebook_number_a"
#define ATP_WEB_CGI_VDB_NUM2FLAGS       "num2Flags"
#define ATP_WEB_CGI_VDB_MOBILENUM       "phonebook_number_m"
#define ATP_WEB_CGI_VDB_NUM3FLAGS       "num3Flags"


#define ATP_WEB_CGI_VDB_IDNUM            "idNum"
#define ATP_WEB_CGI_VDB_ONLNAME          "dectonlbuch_name"
#define ATP_WEB_CGI_VDB_BNAME            "dectonlbuch_bname"
#define ATP_WEB_CGI_VDB_PASSWORD         "dectonlbuch_pwd"
#define ATP_WEB_CGI_VDB_PASSWORDWIE      "PasswordWie"

#define ATP_WEB_CGI_VDB_EMAILENABLE     "ENABLE"
#define ATP_WEB_CGI_VDB_EMAILNAME       "dectemail_name"
#define ATP_WEB_CGI_VDB_SERVERTYPE      "dectemail_servertype"
#define ATP_WEB_CGI_VDB_SERVERCONNECT   "dectemail_serverenc"
#define ATP_WEB_CGI_VDB_SERVERNAME      "dectemail_server"
#define ATP_WEB_CGI_VDB_EMAILPORT       "dectemail_port"
#define ATP_WEB_CGI_VDB_USERNAME        "dectemail_bname"
#define ATP_WEB_CGI_VDB_EMPASSWORD      "dectemail_pwd"

#define ATP_WEB_CGI_VDB_EMAILINT         "EMAILINT"
#endif
/*********************h00190880**********2012/3/28************/
#ifdef SUPPORT_ATP_DT_HTML
#define ATP_WEB_DEFAULT_IDLE_TIME               (10 * 60)    // 10 minutes
#define ATP_WEB_DEFAULT_HEART_BEAT_TIME         (30)        // 20 seconds
#else
#define ATP_WEB_DEFAULT_IDLE_TIME               (5 * 60)    // 5 minutes
#define ATP_WEB_DEFAULT_HEART_BEAT_TIME         (20)        // 20 seconds
#endif

extern VOS_UINT32 g_ulLogOutTime;
extern VOS_UINT32 g_ulHeartBeatTime;
extern VOS_CHAR g_szErrorString[256];

#define ATP_WEB_URI_MAX_PAIRS                   (80)
#define ATP_WEB_PSEUDO_COOKIE_NAME        "cookie"

// ASP related default capabilities
#define ATP_WEB_ASP_MAX_ARGC_D                  (16)
#define ATP_WEB_ASP_CMD_MAX_ARG_D               (64)
#define ATP_WEB_IPADDRESS_LEN                   (16)

#define ATP_WEB_FILETAG_NUMBER                  (4)
/*
 *  For WEB Language
 */
extern const VOS_CHAR  **g_pstLangNames;
extern VOS_UINT32  g_ulCurLang;
extern VOS_UINT32  g_ulDelayTime;
extern VOS_UINT32  g_ulLastLoginFailTime;



#ifdef SUPPORT_ATP_DT_HTML

#define WEB_LOG_SYSLOG_FILEPATH           "/var/syslog.txt"
#define WEB_LOG_MISSCALL_FILEPATH         "/var/misscall.txt"
#define WEB_LOG_INCOMINGCALL_FILEPATH     "/var/incomingcall.txt"
#define WEB_LOG_OUTGOINGCALL_FILEPATH     "/var/outgoingcall.txt"

typedef enum tagATP_WEB_LOG_INFO_EN
{
    WEB_LOG_TYPE_SYSLOG = 0,
    WEB_LOG_TYPE_MISSCALL,
    WEB_LOG_TYPE_INCOMINGCALL,
    WEB_LOG_TYPE_OUTGOINGCALL,
} ATP_WEB_LOG_INFO_EN;

typedef struct tagATP_WEB_LOGTYPE_ST
{
    VOS_UINT32          ulLogType;
    VOS_CHAR           *pcFsFileName;
    VOS_CHAR           *pcSendFileName;
    VOS_CHAR           *pcFileHeader;
    VOS_CHAR           *pcLogTypeMark;
} ATP_WEB_LOGTYPE_ST;


#endif


// Forward Declaration of WEB Server
typedef struct tagATP_WEB_SERVER_ST ATP_WEB_SERVER_ST;

/*
 *  For WEB Users
 */
typedef struct tagATP_WEB_USER_ST
{
    struct tagATP_WEB_USER_ST   *pstNext;

    const VOS_CHAR              *pcUserName;
    const VOS_CHAR              *pcPassword;
    VOS_UINT32                  ulLevel;
} ATP_WEB_USER_ST;

ATP_WEB_USER_ST *ATP_WEB_UserGetByName(ATP_WEB_SERVER_ST    *pstWeb,
                                                 const VOS_CHAR       *pcName);

ATP_WEB_USER_ST *ATP_WEB_UserGetByLevel(ATP_WEB_SERVER_ST    *pstWeb,
                                                VOS_UINT32           ulLevel);

VOS_VOID ATP_WEB_UserDelete(ATP_WEB_SERVER_ST *pstWeb, ATP_WEB_USER_ST *pstDel);

VOS_UINT32 ATP_WEB_UserAdd(ATP_WEB_SERVER_ST *pstWeb, const VOS_CHAR *pcUsername, const VOS_CHAR *pcPassword, VOS_UINT32 ulLevel);

VOS_UINT32 ATP_WEB_UserUpdate(ATP_WEB_SERVER_ST *pstWeb, const VOS_CHAR *pcUsername,
                                     const VOS_CHAR *pcOldPassword, const VOS_CHAR *pcNewPassword,
                                     VOS_UINT32 ulLevel, VOS_BOOL *pbChgd);

/*
 *  For Cookie Item
 */
typedef enum tagATP_WEB_COOKIE_EN
{
    ATP_WEB_COOKIE_ITEM_UNKNOWN = -1,
    ATP_WEB_COOKIE_ITEM_SESSION_ID,
    ATP_WEB_COOKIE_ITEM_LANG,

    ATP_WEB_COOKIE_ITEM_EXTEND
} ATP_WEB_COOKIE_EN;

extern const VOS_CHAR **g_apcExtendCookieItems;

const VOS_CHAR *ATP_WEB_CookieItemGetValueByName(
                            const ATP_HTTP_PACKET_ST    *pstRequest,
                            ATP_WEB_COOKIE_EN           enCookieItemIdx);

/*
 *  For Session
 */
#define ATP_WEB_SESS_LENGTH                     (11)

#ifdef SUPPORT_ATP_ANTI_CSRF_ATTACK
#define ATP_WEB_TOKEN_LENGTH                    (32)
#endif

typedef struct tagATP_WEB_SESSION_ST
{
    struct tagATP_WEB_SESSION_ST    *pstNext;
    VOS_CHAR                        ulSessionID[ATP_WEB_SESS_LENGTH + 1];
#ifdef SUPPORT_ATP_ANTI_CSRF_ATTACK
    VOS_CHAR                        pcToken[ATP_WEB_TOKEN_LENGTH + 1];
#endif
    const ATP_WEB_USER_ST   *pstUser;
    const struct addrinfo         *pstCurIp;
    struct timeval                  ulLastOpTime;
    struct timeval                  ulLastRefTime;
    VOS_VOID                        *pvHook;
} ATP_WEB_SESSION_ST;

extern ATP_WEB_SESSION_ST *g_pstCurSess;

ATP_WEB_SESSION_ST *ATP_WEB_SessionAdd(
                            ATP_WEB_SERVER_ST           *pstWeb,
                            const VOS_CHAR                   *pcUserName,
                            const struct addrinfo             *pstIp);

VOS_VOID ATP_WEB_SessionDel(
                            ATP_WEB_SERVER_ST           *pstWeb,
                            ATP_WEB_SESSION_ST          *pstDel);

VOS_VOID ATP_WEB_SessionDelByUser(
                            ATP_WEB_SERVER_ST           *pstWeb,
                            const VOS_CHAR              *pcUser);

ATP_WEB_SESSION_ST *ATP_WEB_SessionFind(
                            ATP_WEB_SERVER_ST           *pstWeb,
                            const VOS_CHAR              *pcSessionId);

VOS_UINT32 ATP_WEB_SessionGetNumByIP(
                            const ATP_WEB_SERVER_ST     *pstWeb,
                            const struct addrinfo                *pstLoginIP);

VOS_UINT32 ATP_WEB_SessionGetNumByUsername(
                            const ATP_WEB_SERVER_ST     *pstWeb,
                            const VOS_CHAR              *pcUsername);

VOS_UINT32 ATP_WEB_SessionGetNumByIPAndUsername(
                            const ATP_WEB_SERVER_ST     *pstWeb,
                            const struct addrinfo               *pstLoginIP,
                            const VOS_CHAR              *pcUsername);
VOS_UINT32 webLoginCheck(
                ATP_HTTP_CLIENT_ST *pstHttpClient,
                const VOS_CHAR *pcUsername);


VOS_UINT32 ATP_WEB_SessionGetIPByUsername(
                            const ATP_WEB_SERVER_ST     *pstWeb,
                            const VOS_CHAR              *pcUsername,
                            VOS_CHAR                    *pcLoginIP,
                            VOS_UINT32                   ulIPLength);

ATP_WEB_SESSION_ST* ATP_Web_FindSessionByRequest(
                            ATP_WEB_SERVER_ST           *pstWeb,
                            const ATP_HTTP_PACKET_ST    *pstReq);



#ifdef SUPPORT_ATP_ANTI_CSRF_ATTACK
VOS_INT32 ATP_WEB_SessionUpdateToken();
#endif

/*
 *  For VPath
 */
typedef enum tagATP_WEB_VPATH_ATTR_EN
{
    ATP_WEB_VPATH_ATTR_NOCHARE      = (1 << 0),
    ATP_WEB_VPATH_ATTR_HEARTBEAT    = (1 << 1),
    ATP_WEB_VPATH_ATTR_LANGUAGE     = (1 << 2),
    ATP_WEB_VPATH_ATTR_DIRECT_MAP   = (1 << 3),
    ATP_WEB_VPATH_ATTR_PRECHECK     = (1 << 4),
    ATP_WEB_VPATH_ATTR_NOTOKEN      = (1 << 5),
} ATP_WEB_VPATH_ATTR_EN;

typedef struct tagATP_WEB_VPATH_ST
{
    const VOS_CHAR      *pcPathName;
    const VOS_CHAR      *pcFsPath;
    VOS_UINT32          ulAttr;
    VOS_UINT32          ulAccLevel;
} ATP_WEB_VPATH_ST;

extern const ATP_WEB_VPATH_ST *g_apstVPath;
extern const VOS_CHAR         *g_pcFsPath;
extern const ATP_WEB_VPATH_ST *g_pstCurVPath;
extern const ATP_WEB_VPATH_ST  *g_pstWebpCurVPath ;

const ATP_WEB_VPATH_ST *ATP_WEB_VPathFindByUri(const VOS_CHAR *pstReqUri);

/*
 *  File type process
 */
typedef VOS_UINT32 (* PFWebFileTypeProc)(ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                           FILE                    *pstFile,
                                           VOS_INT32               lFileLen);

typedef struct tagATP_WEB_FILE_TYPE_ST
{
    const VOS_CHAR          *pcFileSuffix;
    PFWebFileTypeProc       pfProc;
} ATP_WEB_FILE_TYPE_ST;

extern const ATP_WEB_FILE_TYPE_ST *g_astFileTypes;

/*
 *  File cgi process
 */
 
typedef struct tagATP_WEB_NVPAIR_ST
{
	const VOS_CHAR                 	*pcName;
	const VOS_CHAR                 	*pcValue;
} ATP_WEB_NVPAIR_ST;

VOS_UINT32 ATP_WEB_GetNVPairsFromStr(
                                                 VOS_CHAR           *pcReqUri, 
                                                 ATP_WEB_NVPAIR_ST  *pstArray, 
                                                 VOS_UINT32         ulLength, 
                                                 VOS_UINT32         *pulALen);
VOS_VOID ATP_WEB_Decode(VOS_CHAR *pcStr);
const VOS_CHAR * ATP_WEB_NVGetValueByName( 
                                    const ATP_WEB_NVPAIR_ST *pstArray, 
                                    VOS_UINT32              ulLength, 
                                    const VOS_CHAR          *pcReqName);

typedef VOS_UINT32 (* PFWebCgiReqProc)(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 pulLength);
typedef struct tagATP_WEB_CGI_ST
{
    const VOS_CHAR  *pcCgiName;
    VOS_UINT32      ulAuthLevel;
    PFWebCgiReqProc pfProc;  
}ATP_WEB_CGI_ST;

extern VOS_BOOL g_bForceNoCache;

VOS_UINT32 ATP_WEB_CgiResponseStr(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_CHAR *str);

/*
 *  File Send Proc
 */

typedef VOS_UINT32 (*PFWebFilePreCheckProc)(ATP_HTTP_CLIENT_ST *pstClient);
typedef VOS_BOOL (*PFWebPreProc)(ATP_HTTP_CLIENT_ST *pstClient);

//search file by path
typedef FILE *(* PFWebFileSearchProc)(VOS_CHAR *pcFilePath, VOS_UINT32 ulBufLen, VOS_INT32 *plFileLen);
extern PFWebFileSearchProc g_pfFileSearchProc;


VOS_UINT32 ATP_WEB_SendRawFile(ATP_HTTP_CLIENT_ST     *pstHttpClient,
                               FILE                    *pstFile,
                               VOS_INT32               lFileLen);

VOS_UINT32 ATP_WEB_SendFile(
                                 ATP_HTTP_CLIENT_ST     *pstHttpClient,
                                 const VOS_CHAR         *pcUri);

/*Start of 维护管理组 2010-1-30 14:40 for 问题单:AU4D02394 by x00125299*/
VOS_UINT32 ATP_WEB_AdaptRelocatePage(
                                 ATP_HTTP_CLIENT_ST *pstHttpClient,
                                 const VOS_CHAR *pcPage);
/*End of 维护管理组 2010-1-30 14:40 for by x00125299*/
/*
 *  For WEB Server
 */

typedef VOS_UINT32 (*PFWebAbnormalProc)(ATP_HTTP_CLIENT_ST *pstClient, VOS_UINT32 ulErrCode);
struct tagATP_WEB_SERVER_ST
{
    ATP_WEB_USER_ST             *pstUsers;
    ATP_WEB_SESSION_ST          *pstSessions;

    PFWebAbnormalProc           pfErrProc;

    ATP_HTTP_SERVER_ST          *pstHttpServer;
};

VOS_UINT32 ATP_WEB_ResponseStatusCode(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT32 lStatusCode);

const VOS_CHAR *ATP_WEB_GetCurrentLanguage(const ATP_HTTP_CLIENT_ST *pstHttpClient);

ATP_WEB_SERVER_ST *ATP_WEB_Create(const ATP_WEB_VPATH_ST astVPath[]);

/*
 *  ASP File Proc
 */
typedef VOS_UINT32 (* PfWebAspCmdProc)(const ATP_HTTP_CLIENT_ST *pstHttpClient,
                            VOS_INT32 lArgc, VOS_CHAR * const apcArgv[]);

typedef struct tagATP_WEB_ASPCMD_ST
{
    const VOS_CHAR      *pcCmdName;
    PfWebAspCmdProc     pfProc;
} ATP_WEB_ASPCMD_ST;



VOS_UINT32 ATP_WEB_AspFileProc(ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                      FILE                    *pstFile,
                                      VOS_INT32               lFileLen);

typedef struct tagWEB_UL_FILE_STORE_ST WEB_UL_FILE_STORE_ST;

typedef VOS_UINT32 (* PfWebUploadFilePreCheckProc)(WEB_UL_FILE_STORE_ST *pstStore, const VOS_CHAR *pcBuf, VOS_UINT32 ulLen);

struct tagWEB_UL_FILE_STORE_ST
{
    const VOS_CHAR  *acFileTag;
    FILE*           pfFile;
    PfWebUploadFilePreCheckProc pfPreCheck;
    VOS_UINT32      ulCurrentLen;
    VOS_UINT32      ulMaxLen;
    VOS_UINT32      ulError;
};

VOS_UINT32 ATP_WEB_UploadFile(ATP_HTTP_CLIENT_ST  *pstHttpClient,
                              WEB_UL_FILE_STORE_ST      *pstFileStore,
                              VOS_UINT32                ulNumOfFile);
/*
 *  Embedded CGI procs
 */
#define ATP_WEB_DOMAINNAME_LENGTH       (256)
#define ATP_WEB_WAIT_TIME               (1000*30)

#define ATP_WEB_CGI_REQUESTFILE         "RequestFile"
#define ATP_WEB_CGI_GMSG_MODENAME       "ModName"
#define ATP_WEB_CGI_GMSG_SUBTYPE        "SubType"

/*
 *  Configuration related cgi procs provided by ATP
 */

typedef VOS_UINT32 (* PfWebCgiPreProc)(const ATP_HTTP_CLIENT_ST *pstHttpClient);

extern PfWebCgiPreProc      g_pfCgiPreProc;

VOS_UINT32 ATP_WEB_CgiPreProc(
                              ATP_HTTP_CLIENT_ST  *pstHttpClient, 
                              VOS_CHAR            **ppcBody, 
                              ATP_WEB_NVPAIR_ST   *pstBodyArray,
                              VOS_UINT32          ulLength, 
                              VOS_UINT32          *pulBodyPairLen);

VOS_UINT32 ATP_WEB_CgiSetCfgProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiSetDoProc(const ATP_WEB_NVPAIR_ST *pstNVPair, ATP_WEB_NVPAIR_ST  *pstBodyArray, VOS_VOID **ppstResMsg);

VOS_UINT32 ATP_WEB_CgiAddCfgProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiAddDoProc(const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLen,
                                          ATP_WEB_NVPAIR_ST *pcBodyArray, VOS_UINT32 *pulInstanceID,
                                          VOS_VOID **ppstResMsg);

VOS_UINT32 ATP_WEB_CgiDelCfgProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiDelDoProc(ATP_WEB_NVPAIR_ST *pcBodyArray);

VOS_UINT32 ATP_WEB_CgiCfgGetByAjaxProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiRemoveProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

/*
 *  GMSG related cgi procs provided by ATP
 */

VOS_UINT32 ATP_WEB_CgiGMsgGetByAjaxProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiGMsgSendCmdProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

VOS_UINT32 ATP_WEB_CgiGMsgSyncSendCmdProc(ATP_HTTP_CLIENT_ST *pstHttpClient, const ATP_WEB_NVPAIR_ST *pstNVPair, VOS_UINT32 ulLength);

/*
 *  Embedded ASP procs
 */

VOS_UINT32 ATP_WEB_ParseNVPairs(VOS_CHAR        *pcSrcStr,
                                        const VOS_CHAR  *pcSplit,
                                        VOS_UINT32      ulLen,
                                        VOS_CHAR        **ppcArgv,
                                        VOS_UINT32      *pulNumOfPairs);

VOS_UINT32 ATP_WEB_AspEscapePrintString(ATP_UTIL_STRFILE_ST *pstStrFile, const VOS_CHAR *pcStr);

/* <DTS2013040206195 z81004485 2013/4/2 PC_LINT begin */
VOS_UINT32 ATP_WEB_BuildSendFileHeader(ATP_HTTP_CLIENT_ST *pstHttpClient);
VOS_UINT32 ATP_WEB_GetArray(ATP_UTIL_STRFILE_ST *pstFile, VOS_CHAR *szDomainName, VOS_CHAR *szNodeNames);
/* DTS2013040206195 z81004485 2013/4/2 PC_LINT end> */
VOS_UINT32 ATP_WEB_AspGetCfgArray(const ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT32 lArgc, VOS_CHAR * const pcArgv[]);

VOS_UINT32 ATP_WEB_AspGetCfgEntry(const ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT32 lArgc, VOS_CHAR * const pcArgv[]);

VOS_UINT32 ATP_WEB_AspGetParaByDomainName(const ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT32 lArgc, VOS_CHAR * const pcArgv[]);

VOS_UINT32 ATP_WEB_AspGMsgGet(const ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT32 lArgc, VOS_CHAR * const pcArgv[]);

const VOS_CHAR * ATP_WEB_GetVersion(void);



#ifdef SUPPORT_ATP_DT_HTML
VOS_UINT32 WebRediect(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_CHAR *pcRediectUrl, VOS_CHAR *pcUpgStatus);
#endif

extern VOS_INT32        g_ulUTF8AutoDetect;
#ifdef SUPPORT_ATP_DMS
VOS_UINT32 ATP_WEB_AspSetUTF8AutoDetectValue(ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                      FILE                    *pstFile,
                                      VOS_INT32               lFileLen);
#endif

VOS_UINT32 ATP_WEB_AspFileProcEx(ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                      FILE                    *pstFile,
                                      VOS_INT32               lFileLen);

VOS_UINT32 ATP_WEB_DelOverTimeSess(ATP_WEB_SERVER_ST *pstWeb);

typedef struct tagATP_WEB_USER_LEVEL_ST
{
    VOS_UINT32          ulUserLevel;
    VOS_UINT32          ulCfmLevel;
} ATP_WEB_USER_LEVEL_ST;

extern ATP_WEB_USER_LEVEL_ST *g_pastLevelMap;
VOS_VOID ATP_WEB_StripSlashes(VOS_CHAR *pcReqUri);

/* <DTS2012080607642 z81004485 2012/8/18 begin  */
#ifdef ATP_DEBUG
#define ATP_WEB_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\r\n");}
#else
#define ATP_WEB_DEBUG(x...)
#endif
/* DTS2012080607642 z81004485 2012/8/18 end> */

#define ATP_WEB_TRACE       ATP_MSG_Printf

/*start of add by z00163152:20120130*/

typedef struct tagATP_WEB_CGI_LIST_ST
{
    struct tagATP_WEB_CGI_LIST_ST   *pstNext;
    const ATP_WEB_CGI_ST            *pstCgi;
} ATP_WEB_CGI_LIST_ST;



const ATP_WEB_CGI_LIST_ST * ATP_WEB_CgiFindByName(const VOS_CHAR *pcCgiName);
VOS_UINT32 ATP_WEB_RegCgiProc(const ATP_WEB_CGI_ST *pstCgi);
VOS_UINT32 ATP_WEB_RegCgiArrayProc(const ATP_WEB_CGI_ST *pstCgi, VOS_UINT32 ulArrayLen);

typedef struct tagATP_WEB_ASP_LIST_ST
{
    struct tagATP_WEB_ASP_LIST_ST   *pstNext;
    const ATP_WEB_ASPCMD_ST         *pstAsp;
} ATP_WEB_ASP_LIST_ST;

const ATP_WEB_ASP_LIST_ST * ATP_WEB_AspFindByName(const VOS_CHAR *pcAspName);
VOS_UINT32 ATP_WEB_RegAspProc(const ATP_WEB_ASPCMD_ST *pstAsp);
VOS_UINT32 ATP_WEB_RegAspArrayProc(const ATP_WEB_ASPCMD_ST *pstAsp, VOS_UINT32 ulArrayLen);

VOS_UINT32 ATP_WEB_RegPreProc(ATP_WEB_SERVER_ST *pstServer, AtpNotifierProc pfPreProc,
            VOS_UINT32 ulPriority, VOS_VOID *pvHook);


typedef enum
{
    ATP_WEB_PREPROC_SAFARI_CHECK_D =  100,
    ATP_WEB_PREPROC_PRIORITY_HURL_D,    
}ATP_WEB_NOTIFY_PRIORITY;
/*end of add by z00163152:20120130*/

VOS_CHAR* ATP_WEB_UtilReadFileToBuffer(FILE *pfFile, VOS_INT32 lFileLen);

VOS_UINT32 webGMsgGetValue(
                                ATP_UTIL_STRFILE_ST *pstSendStrFile,
                                const VOS_CHAR      *pcModName,
                                VOS_UINT32          ulSubType,
                                VOS_INT8            **ppcNodeName,
                                VOS_INT8            *pcStartTag,
                                VOS_INT8            *pcEndTag);
VOS_UINT32 webGetAndParseResponse(
                                    ATP_UTIL_STRFILE_ST *pstSendStrFile,
                                    VOS_VOID            *pstMsg,
                                    VOS_BOOL            bDomainName);

VOS_UINT32 webGetNodeValue (
                                ATP_UTIL_STRFILE_ST *pstSendStrFile,
                                const VOS_CHAR      *pcDomainName,
                                VOS_INT8 * const    apcNodeName[],
                                const VOS_INT8      *pcStartTag,
                                const VOS_INT8      *pcEndTag);

typedef VOS_UINT32 (*PfWebDoFilterProc)(ATP_WEB_SERVER_ST *pstServer, ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri);

typedef struct tagATP_WEB_FILTER_ST
{
    struct tagATP_WEB_FILTER_ST *pstNext;
    VOS_CHAR                    *pcPrefixUri;
    PfWebDoFilterProc           pfDoFilter;
}ATP_WEB_FILTER_ST;

VOS_UINT32 ATP_WEB_RegFilter(const VOS_CHAR *pcPrefixUri, PfWebDoFilterProc pfFilterProc);

VOS_UINT32 ATP_WEB_DoFilters(ATP_WEB_SERVER_ST *pstServer, ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri);

VOS_BOOL ATP_WEB_MatchPrefixPath(const VOS_CHAR *pattern, const VOS_CHAR *string);

#if defined(SUPPORT_ATP_PIN_WEB_MNG) && defined(SUPPORT_ATP_VOICE) && defined(SUPPORT_ATP_VOICE_ATP)
VOS_UINT32 ATP_WEB_VoiceBusyFilterProc(ATP_WEB_SERVER_ST *pstServer, ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri);
#endif

#ifdef SUPPORT_ATP_DT_HTML
VOS_UINT32 ATP_WEB_CheckSSLFilterPorc(ATP_WEB_SERVER_ST *pstServer, ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri);
#endif

#endif

/* <DTS2012080607642 z81004485 2012/8/18 begin  */

/* <DTS2013032702243  z00159386  20130328  begin */
VOS_UINT32 ATP_WEB_GetPara(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID, VOS_VOID *pValue,unsigned int len);
/* DTS2013032702243  z00159386  20130328  end> */
VOS_UINT32 ATP_WEB_DelObj(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulOID);
/* <DTS2013032702243  z00159386  20130328  begin */
VOS_UINT32 ATP_WEB_GetParaByDomainName(const VOS_CHAR* pszDomainName, VOS_VOID *pValue,unsigned int len);
/* DTS2013032702243  z00159386  20130328  end> */
/* <DTS2012081704854 w00189185 2012/8/26 begin  */
VOS_UINT32 Atp_Web_GetSingleObjString(ATP_UTIL_STRFILE_ST *pstSendStrFile, VOS_CHAR *pcObjDomainName, VOS_CHAR *apcNodeName[], VOS_CHAR *apcXmlName[], VOS_CHAR *pcDomainName, VOS_CHAR *pcDomainXmlName, VOS_CHAR *pcStartXmlName);
VOS_UINT32 Atp_Web_GetMultiObjString(ATP_UTIL_STRFILE_ST *pstSendStrFile, VOS_CHAR *pcObjDomainName, VOS_CHAR *apcNodeName[], VOS_CHAR *apcXmlName[], VOS_CHAR *pcDomainName, VOS_CHAR *pcDomainXmlName, VOS_CHAR *pcStartXmlName);
/* DTS2012081704854 w00189185 2012/8/26 end> */
/* DTS2012080607642 z81004485 2012/8/18 end> */


