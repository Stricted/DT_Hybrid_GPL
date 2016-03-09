#ifndef __ATP_HTTP_API_H__
#define __ATP_HTTP_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#include "strfileapi.h"
#include "msgapi.h"
#ifdef SUPPORT_ATP_SSL
#include <openssl/ssl.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
 *  HTTP Module Return Code
 */
typedef enum tagATP_HTTP_RET_CODE_EN
{
    ATP_HTTP_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_HTTPCORE),
    ATP_HTTP_RET_MEM_ERR,
    ATP_HTTP_RET_FORMAT_ERR,
    ATP_HTTP_RET_NOT_INITED,
    ATP_HTTP_RET_HEADER_ERR,
    ATP_HTTP_RET_HEADER_NOT_ENOUGH,
    ATP_HTTP_RET_UNSUPPORTED_METHOD_ERR,
    ATP_HTTP_RET_UNSUPPORTED_VERSION_ERR,
    ATP_HTTP_RET_DNS_ERR,
    ATP_HTTP_RET_SOCK_CONN,
    ATP_HTTP_RET_SOCK_RESET,
    ATP_HTTP_RET_SOCK_PEER_DISCONNECT,
    ATP_HTTP_RET_SOCK_RECV_ERR,
    ATP_HTTP_RET_SOCK_SEND_ERR,
    ATP_HTTP_RET_SOCK_TIMEOUT,
    ATP_HTTP_RET_SOCK_SSL_ERR,
    ATP_HTTP_RET_NOT_SPPORTED,
    ATP_HTTP_RET_AUTH_ERR,
    ATP_HTTP_RET_NEED_AUTH,
    ATP_HTTP_RET_TOO_MANY_CONS_ERR,
    ATP_HTTP_RET_INTERNAL_ERR,
    ATP_HTTP_RET_NO_RESPONSE,
    ATP_HTTP_RET_SSL_INVALID_CERT,
    ATP_HTTP_RET_NO_AUTHENTICATE,
    ATP_HTTP_RET_LANIP_FOR_CWMP,
    ATP_HTTP_RET_NOT_SUPPORT_EXPECT    
} ATP_HTTP_RET_CODE_EN;

/*
 *  MD5 algorithm related function prototype definition
 */
#ifdef SUPPORT_ATP_SSL      // If enabled ssl, use the library from ssl
#include <openssl/md5.h>
#else
typedef struct {
  VOS_UINT32 state[4];          /* state (ABCD) */
  VOS_UINT32 count[2];          /* number of bits, modulo 2^64 (lsb first) */
  VOS_UINT8  buffer[64];     /* input buffer */
} MD5_CTX;

void MD5_Init(MD5_CTX *ctx);
void MD5_Update(MD5_CTX *ctx, VOS_UINT8 *pucData, VOS_UINT32 ulLen);
void MD5_Final(VOS_UINT8 input[16], MD5_CTX *ctx);

#endif

/*
 *  Socket Layer
 *      This layer abastract TCP socket and SSL socket
 *      All http client and http server should use this layer to do data trans-recv
 */
/** Socket type */
typedef enum tagATP_SOCK_FLAG_EN
{
    ATP_SOCK_DFT        = 0,        // Default: TCP, no SSL, IPv4
    ATP_SOCK_UDP        = 0x01,     // Socket type, 0: TCP; 1: UDP
    ATP_SOCK_SSL        = 0x02,     // Whether SSL, 0: No SSL; 1: SSL
    ATP_SOCK_IP_VER     = 0x04,     // Version, 0: IPv4; 1: IPv6
    ATP_SOCK_UNIX       = 0x08,

    // Allowed combinations
    ATP_SOCK_TCP4_SSL   = ATP_SOCK_SSL,
    ATP_SOCK_TCP6_SSL   = ATP_SOCK_SSL | ATP_SOCK_IP_VER,
    ATP_SOCK_UDP6       = ATP_SOCK_UDP | ATP_SOCK_IP_VER,
} ATP_SOCK_FLAG_EN;

typedef struct tagATP_SOCKET_ST
{
    VOS_INT32           lSock;
    VOS_UINT32          enSockFlag;     // todo: should be ulSockFlag, ref: ATP_SOCK_FLAG_EN
#ifdef SUPPORT_ATP_SSL
    SSL_CTX             *pstCtxObj;     // Provided by others
    SSL                 *pstSslObj;     // Created when Connect or when Accept
#else
    VOS_VOID            *pstCtxObj;     // Dummy position
    VOS_VOID            *pstSslObj;     // Dummy position
#endif
    VOS_BOOL            bSSLMode;
} ATP_SOCKET_ST;

#ifdef SUPPORT_ATP_SSL

// Authorization mode
typedef enum tagATP_HTTP_SSL_MODE
{
    ATP_HTTP_SSL_CERT_AUTH_NONE,        // No authorization
    ATP_HTTP_SSL_CERT_AUTH_PEER,        // Authorization peer, need to provide root cert
    ATP_HTTP_SSL_CERT_AUTH_BY_PEER,     // To be authorized by peer, need to provide public cert and key
    ATP_HTTP_SSL_CERT_AUTH_BOTH,        // Need to do full authorization
    ATP_HTTP_SSL_CERT_AUTH_MAX
} ATP_HTTP_SSL_MODE;

/* SSL版本 */
typedef enum tagATP_HTTP_SSL_VERSION
{
    ATP_HTTP_SSL_ANY_EXCLUDE_V2,        // Any version except SSLv2
    ATP_HTTP_SSL_ANY,                   // SSLv2, SSLv3 or TLSv1
    ATP_HTTP_SSL_TLS,                   // TLSv1 only
    ATP_HTTP_SSL_V3,                    // SSLv3 only
    ATP_HTTP_SSL_MAX
} ATP_CWMP_SSL_VERSION;

SSL_CTX *ATP_UTIL_SSLCreateCtx(
                            ATP_CWMP_SSL_VERSION    enSslVersion,
                            ATP_HTTP_SSL_MODE       enSslMode,
                            const VOS_CHAR          *pcCAPath,
                            const VOS_CHAR          *pcPubCertPath,
                            const VOS_CHAR          *pcKeyPath,
                            const VOS_UINT8         *pcPassPhrase,
                            VOS_UINT32              ulPassPhraseLen,
                            VOS_BOOL                bIsServer);

SSL_CTX *ATP_UTIL_SSLCreateServerCtx(
                            ATP_CWMP_SSL_VERSION    enSslVersion,
                            ATP_HTTP_SSL_MODE       enSslMode,
                            VOS_UINT32              ulSSLID,
                            VOS_BOOL                bIsServer);
#endif

//delete for not used
#if 0
const VOS_CHAR * ATP_HTTP_GetVersion(void);
#endif

VOS_UINT32 ATP_UTIL_SocketCreateClient(
                                ATP_SOCKET_ST       *pstSock,
                                const VOS_UINT32          ulLocalAddr);

VOS_UINT32 ATP_UTIL_SocketCreateClientEx(
                                ATP_SOCKET_ST       *pstSock,
                                const struct addrinfo *pstLocalAddr);

VOS_UINT32 ATP_UTIL_SocketCreateClientEx6(
                                ATP_SOCKET_ST       *pstSock);

VOS_INT32 ATP_UTIL_SocketCreateServerEx(
                                const VOS_CHAR      *pcLocalAddr,
                                VOS_UINT32          ulPort,
                                ATP_SOCK_FLAG_EN    enSockType,
                                const VOS_CHAR       *pcIpv6ScopeIdName);

VOS_INT32 ATP_UTIL_SocketCreateServer(
                                const VOS_CHAR      *pcLocalAddr,
                                VOS_UINT32          ulPort,
                                ATP_SOCK_FLAG_EN    enSockType);

VOS_VOID ATP_UTIL_SocketCopy(
                                ATP_SOCKET_ST       *pstDst,
                                const ATP_SOCKET_ST *pstSrc);

VOS_INT32 ATP_UTIL_SocketClose(VOS_INT32      lSock);

VOS_UINT32 ATP_UTIL_SocketConnect(
                                ATP_SOCKET_ST       *pstSocket,
                                const struct addrinfo *pstAddr);

VOS_UINT32 ATP_UTIL_SocketAccept(
                                ATP_SOCKET_ST       *pstSocket,
                                ATP_SOCKET_ST       *pstNewSocket,
                                struct addrinfo         **ppstFromAddr);

VOS_UINT32 ATP_UTIL_SocketSendUDP(ATP_SOCKET_ST       *pstSocket,
                                  const VOS_CHAR      *pcBuffer,
                                  VOS_INT32           lLength,
                                  const struct addrinfo *pstAddr);

VOS_UINT32 ATP_UTIL_SocketSend(
                                ATP_SOCKET_ST       *pstSocket,
                                const VOS_CHAR      *pcBuffer,
                                VOS_INT32           lLength);

VOS_UINT32 ATP_UTIL_SocketRecvUDP(ATP_SOCKET_ST       *pstSocket,
                                  VOS_CHAR               *pcBuffer,
                                  VOS_INT32               lLength,
                                  VOS_INT32               *plRecvLen,
                                  struct addrinfo         **ppstAddr);

VOS_UINT32 ATP_UTIL_SocketRecv(
                                ATP_SOCKET_ST       *pstSocket,
                                VOS_CHAR            *pcBuffer,
                                VOS_INT32           lLength,
                                VOS_INT32           *plRecvLen);

VOS_VOID ATP_UTIL_SocketDisconnect(
                                ATP_SOCKET_ST       *pstSocket);

VOS_UINT32 ATP_UTIL_SocketSetTTL(VOS_INT32 lSock, VOS_UINT8 ucTTLValue);
VOS_UINT32 ATP_UTIL_SocketSetMcastIf(VOS_INT32 lSock, const struct addrinfo *pstIpIf);
VOS_UINT32 ATP_UTIL_SocketJoinMcast(VOS_INT32 lSock, const VOS_CHAR *pcMCastIp, const struct addrinfo *pstLocalIp);
VOS_UINT32 ATP_UTIL_SocketJoinMcastEx(
                                        VOS_INT32               lSock,
                                        const VOS_CHAR          *pcMCastIp,
                                        const struct addrinfo   *pstLocalIp, 
                                        const VOS_CHAR          *pcInterface);

typedef VOS_VOID (*PFUtilExtraSocketOp)(
                                VOS_INT32               lSockDesc,
                                const struct addrinfo *pstAddrInfo);

VOS_VOID  ATP_UTIL_RegExtraSockOp(PFUtilExtraSocketOp pfFunc);

typedef VOS_UINT32 (*PFUtilNameResolveFunc)(
                                const VOS_CHAR       *pcName,    // 协议栈待解析的域名
                                const struct addrinfo *pstLocalInterface,  // 协议栈所绑定的IP
                                VOS_UINT32             ulRemotePort,
                                struct addrinfo         **ppstResovledAddr);  // 解析后的IP地址

VOS_VOID  ATP_UTIL_RegNameResolveFunc(PFUtilNameResolveFunc pfFunc);

VOS_UINT32 ATP_UTIL_GetIpAddresses(
                                const VOS_CHAR          *pcHostName,
                                const struct addrinfo    *pstLocalInterface,
                                VOS_UINT32                ulRemotePort,
                                struct addrinfo            **ppstResovledAddr);

/*
 *  Sockbase utilities
 *      This module will listen to sockets, whenever new data or connections come,
 *           event call back will be called.
 *      All sockets event will be multiplexed by this module.
 */

typedef struct tagATP_SOCK_BASE_ST ATP_SOCK_BASE_ST;
typedef VOS_VOID (* PFUtilSockEvtProc)(const ATP_SOCK_BASE_ST *pstSockBase);

struct tagATP_SOCK_BASE_ST
{
    struct tagATP_SOCK_BASE_ST  *pstNext;

    VOS_INT32                   lSock;
    VOS_INT32                   lSockFlags;
    VOS_BOOL                    bNoSelect;
    PFUtilSockEvtProc           pfProc;
    VOS_VOID                    *pvHook;
};


extern ATP_THREAD_SAFE VOS_BOOL    g_bBaseExit;
extern ATP_THREAD_SAFE VOS_UINT32  g_ulIdleTime;
typedef enum tagATP_HTTP_STOP_STATE_EN
{
	 ATP_HTTP_STOP_BEFORE,
	 ATP_HTTP_STOP_AFTER,
	 ATP_HTTP_STOP_EXTERN
} ATP_HTTP_STOP_STATE_EN;

typedef VOS_BOOL (*PFATPHTTPConnectProc)(ATP_HTTP_STOP_STATE_EN enStopState);
extern PFATPHTTPConnectProc  g_pfConnectShouldStop;


VOS_INT32 ATP_UTIL_SockBaseGetMaxFd(VOS_VOID);

VOS_UINT32 ATP_UTIL_SockBaseReg(
                                VOS_INT32           lSock,
								ATP_SOCK_FLAG_EN    lSockFlag,
								PFUtilSockEvtProc   pfProc,
								VOS_BOOL            bListen,
								VOS_VOID            *pvHook,
								ATP_SOCK_BASE_ST    **ppstRet);

VOS_UINT32 ATP_UTIL_SockBaseRegEvtProc(VOS_VOID);

VOS_VOID ATP_UTIL_SockBaseListen(const ATP_SOCK_BASE_ST *pstSockBase, VOS_BOOL bListen);

VOS_VOID ATP_UTIL_SockBaseDirectDo(ATP_SOCK_BASE_ST *pstSockBase);

VOS_VOID ATP_UTIL_SockBaseDelete(ATP_SOCK_BASE_ST *pstSockBase);

VOS_VOID ATP_UTIL_SockBaseDestroy(VOS_VOID);

VOS_UINT32 ATP_UTIL_SockBaseStart(VOS_UINT32 ulMaxIdleTime);

/*
 *  MIME types
 */
typedef struct tagATP_MIME_TYPE_ST
{
    const VOS_CHAR      *pcSuffix;
    const VOS_CHAR      *pcMimeType;
} ATP_MIME_TYPE_ST;

extern const ATP_MIME_TYPE_ST *g_pstExtMimeTypes;
#define ATP_DEFAULT_MIME_TYPE_D             "text/plain"

const VOS_CHAR *ATP_UTIL_GetMimeTypeFromSuffix(const VOS_CHAR *pcSuffix);

/*
 *  Protocol Definition
 */


/*
 *  HTTP Packet Format Definition
 */
typedef enum tagATP_NTWK_PROTOCOL_EN
{
    ATP_NTWK_PROTOCOL_UNKNOWN    = -1,  // Unknown protocol
    ATP_NTWK_PROTOCOL_HTTP    = 0,      // HTTP
    ATP_NTWK_PROTOCOL_HTTPS,            // HTTPS
    ATP_NTWK_PROTOCOL_FTP,              // FTP
    ATP_NTWK_PROTOCOL_TFTP,             // TFTP,

    ATP_NTWK_PROTOCOL_EXTEND
} ATP_NTWK_PROTOCOL_EN;

VOS_CHAR *ATP_UTIL_GetEscapedUri(const VOS_CHAR *pcSrcUri);

VOS_BOOL ATP_UTIL_ParseUrl(
                           const VOS_CHAR   *pcURL,     // In
                           VOS_INT32        *enProtocol,// Out
                           VOS_CHAR         **ppcIP,    // Out
                           VOS_UINT32       *pulPort,   // Out
                           VOS_CHAR         **ppcPath);

VOS_BOOL ATP_UTIL_ParseUrlEx(
                           const VOS_CHAR   *pcURL,     // In
                           VOS_INT32        *enProtocol,// Out
                           VOS_CHAR         **ppcIP,    // Out
                           VOS_UINT32       *pulPort,   // Out
                           VOS_CHAR         **ppcPath,
                           VOS_BOOL         bIgnorIpv6);  // Out

extern const VOS_CHAR **g_apcExtendProtocols;
extern const VOS_UINT32 *g_aulExtendPorts;

/*
 * Directive   DirectiveObj   Version
 *  GET             /post       (HTTP/)1.1
 *
 * Here the DirectiveObj should be escaped according to the HTTP spec
 * But in TR069, we use it only one time, so we omit it
 *
 *  Version        StatusCode   StatusData
 *  (HTTP/)1.1     200              OK
 */

#define ATP_HTTP_INVALID_STR        ("")

/* HTTP version */
typedef enum tagATP_HTTP_VERSION_EN
{
    ATP_HTTP_VERSION_UNKNOWN    = -1,   // Unknown version
    ATP_HTTP_VERSION_0_9        = 0,    // HTTP 0.9
    ATP_HTTP_VERSION_1_0,               // HTTP 1.0
    ATP_HTTP_VERSION_1_1,               // HTTP 1.1
} ATP_HTTP_VERSION_EN;

const VOS_CHAR *ATP_HTTP_GetVersionStrFromIdx(VOS_INT32 lVersionIdx);

VOS_INT32 ATP_HTTP_GetVersionIdxFromStr(const VOS_CHAR *pcVersionStr);

/* HTTP directive code */
typedef enum tagATP_HTTP_DIRECTIVE_CODE_EN
{
    ATP_HTTP_DIRECTIVE_CODE_UNKNOWN     = -1,
    ATP_HTTP_DIRECTIVE_CODE_GET         = 0,
    ATP_HTTP_DIRECTIVE_CODE_POST,

    ATP_HTTP_DIRECTIVE_CODE_EXTEND
} ATP_HTTP_DIRECTIVE_CODE_EN;

extern const VOS_CHAR **g_apcHttpExtendDirectives;

const VOS_CHAR *ATP_HTTP_GetDirectiveStrFromIdx(VOS_INT32 lDirectiveIdx);

VOS_INT32 ATP_HTTP_GetDirectiveIdxFromStr(const VOS_CHAR *pcDirectiveStr);

typedef enum tagATP_HTTP_HEADER_IDX_EN
{
    ATP_HTTP_HEADER_INVALID               = -1,

    ATP_HTTP_HEADER_CONTENTLENGTH         = 0,
    ATP_HTTP_HEADER_TRANSFERENCODING,
    ATP_HTTP_HEADER_WWW_AHTHENTICATE,
    ATP_HTTP_HEADER_SET_COOKIE,
    ATP_HTTP_HEADER_SET_COOKIE2,
    ATP_HTTP_HEADER_CONNECTION,
    ATP_HTTP_HEADER_AUTHORIZATION,
    ATP_HTTP_HEADER_IF_MODIFIED,
    ATP_HTTP_HEADER_LAST_MODIFIED,
    ATP_HTTP_HEADER_DATE,
    ATP_HTTP_HEADER_LOCATION,
    ATP_HTTP_HEADER_HOST,
    ATP_HTTP_HEADER_RESPONSE_COOKIE,
    ATP_HTTP_HEADER_SOAPACTION,
    ATP_HTTP_HEADER_CONTENTTYPE,
    ATP_HTTP_HEADER_USERAGENT,
    ATP_HTTP_HEADER_SERVER,
    ATP_HTTP_HEADER_CACHE_CONTROL,
    ATP_HTTP_HEADER_ACCEPT_LANG,
    ATP_HTTP_HEADER_CONTENT_LANGUAGE,
    ATP_HTTP_HEADER_EXPIRES,
    ATP_HTTP_HEADER_EXPECT,

    ATP_HTTP_HEADER_EXTEND,
    
} ATP_HTTP_HEADER_IDX_EN;
extern const VOS_CHAR **g_apcHttpExtendHeaders;

/* HTTP header values */
#define ATP_HTTP_CONNECTION_KEEPALIVE_D         "Keep-Alive"
#define ATP_HTTP_CONNECTION_CLOSE_D             "Close"
#define ATP_HTTP_CACHE_CONTROL_D                "no-cache"

const VOS_CHAR *ATP_HTTP_GetHeaderStrFromIdx(VOS_INT32 lHeaderIdx);

VOS_INT32 ATP_HTTP_GetHeaderIdxFromStr(const VOS_CHAR *pcHeaderStr);

typedef struct tagATP_HTTP_STATUS_CODE_ST
{
    VOS_INT32           lStatusCode;
    const VOS_CHAR      *pcStatusStr;
} ATP_HTTP_STATUS_CODE_ST;

/* HTTP status code */
#define ATP_HTTP_STATUS_CODE_100            100   // continue
#define ATP_HTTP_STATUS_CODE_200            200   // OK
#define ATP_HTTP_STATUS_CODE_400            400   // Bad Request
#define ATP_HTTP_STATUS_CODE_401            401   // Authorization Needed
#define ATP_HTTP_STATUS_CODE_100            100   // Continue
#define ATP_HTTP_STATUS_CODE_201            201   //File Created
#define ATP_HTTP_STATUS_CODE_204            204   // No content
#define ATP_HTTP_STATUS_CODE_404            404   // Not found
#define ATP_HTTP_STATUS_CODE_301            301   // Moved Permenently
#define ATP_HTTP_STATUS_CODE_302            302   // Found
#define ATP_HTTP_STATUS_CODE_304            304   // Not modified
#define ATP_HTTP_STATUS_CODE_307            307   // Temporary Redirect
#define ATP_HTTP_STATUS_CODE_500            500   // Internal Server Error
#define ATP_HTTP_STATUS_CODE_501            501   // 
#define ATP_HTTP_STATUS_CODE_412            412   //
#define ATP_HTTP_STATUS_CODE_403            403   // Forbidden
#define ATP_HTTP_STATUS_CODE_405            405   // Method Not Allowed
#define ATP_HTTP_STATUS_CODE_409            409   // Conflict
#define ATP_HTTP_STATUS_CODE_417            417   // Expectation Failed

extern const ATP_HTTP_STATUS_CODE_ST *g_pstHttpExtendStatusCodes;

const VOS_CHAR *ATP_HTTP_GetStatusStrFromIdx(VOS_INT32 lStatusIdx);

VOS_BOOL ATP_HTTP_IsDataValidPacket(const VOS_CHAR *pcBuf, VOS_INT32 lLen);

typedef struct tagATP_HTTP_RESPONSEDATA_ST
{
    struct tagATP_HTTP_RESPONSEDATA_ST  *pstNext;

    VOS_CHAR                        *pcKey;
    VOS_CHAR                        *pcValue;
}ATP_HTTP_RESPONSEDATA_ST;


typedef struct tagATP_HTTP_HEADER_ST
{
    struct tagATP_HTTP_HEADER_ST    *pstNext;

    VOS_INT32                       lHeaderIdx;
    VOS_CHAR                        *pcHeaderValue;
} ATP_HTTP_HEADER_ST;

typedef struct tagATP_HTTP_PACKET_ST
{
    VOS_INT16               sVersion;
    VOS_INT16               sDirectiveCode;
    VOS_INT16               sStatusCode;
    VOS_INT16               sChunkMode;
    VOS_CHAR                *pcRequestUri;

    ATP_HTTP_HEADER_ST      *pstHeaderList;
    ATP_HTTP_RESPONSEDATA_ST    *pstDataList;   // response data
    VOS_VOID                *pvData;           // For extension
} ATP_HTTP_PACKET_ST;

/*!
  \brief clear http package response data
  \param[in]  *pstPacket: 
  \return 
*/
VOS_VOID ATP_HTTP_PacketClearResponseData(ATP_HTTP_PACKET_ST *pstPacket);
/*!
  \brief set http package response data
  \param[in]  *pstPacket: 
  \param[in]  *pcKey: key
  \param[in]  *pcValue: value
  \return 
*/
VOS_UINT32 ATP_HTTP_PackageSetResponseData(
                                                ATP_HTTP_PACKET_ST *pstPacket, 
                                                const VOS_CHAR *pcKey,
                                                const VOS_CHAR *pcValue);   

typedef VOS_UINT32 (*PfHttpPackageResponseDataVisitor)(
                                                VOS_VOID *pvHandle, 
                                                const VOS_CHAR *pcKey,
                                                const VOS_CHAR *pcValue);

/*!
  \brief http package response data visit
  \param[in]  *pstPacket: 
  \param[in]  pfVisitor: 
  \param[in]  *pvValue: 
  \return 
*/
VOS_UINT32 ATP_HTTP_PackageVisitResponseData(
                                   ATP_HTTP_PACKET_ST              *pstPacket, 
                                   PfHttpPackageResponseDataVisitor pfVisitor,
                                   VOS_VOID *pvValue);

VOS_VOID ATP_HTTP_PacketDestroy(ATP_HTTP_PACKET_ST *pstPacket);

ATP_HTTP_HEADER_ST *ATP_HTTP_PacketStripHeader(
                                    ATP_HTTP_PACKET_ST      *pstPacket,
                                    VOS_INT32               lHeaderIdx);

VOS_VOID ATP_HTTP_PacketClearHeaders(ATP_HTTP_PACKET_ST *pstPacket);

VOS_VOID ATP_HTTP_PacketClearHeaderByIdx(
                                    ATP_HTTP_PACKET_ST      *pstPacket,
                                    VOS_UINT32              ulIdx);

#define ATP_HTTP_PacketClearCookies(packet) \
                    ATP_HTTP_PacketClearHeaderByIdx((packet), \
                            ATP_HTTP_HEADER_RESPONSE_COOKIE)

ATP_HTTP_HEADER_ST *ATP_HTTP_PacketGetHeaderBySeq(
                                    const ATP_HTTP_PACKET_ST *pstPacket,
                                    VOS_INT32                lHeaderIdx,
                                    const ATP_HTTP_HEADER_ST *pstHeader);

VOS_VOID ATP_HTTP_PacketMoveCookies(
                                    ATP_HTTP_PACKET_ST      *pstPacketDst,
                                    ATP_HTTP_PACKET_ST      *pstPacketSrc);

const VOS_CHAR *ATP_HTTP_PacketGetHeaderValue(
                                    const ATP_HTTP_PACKET_ST *pstPacket,
                                    VOS_INT32               lHeaderIdx);

VOS_UINT32 ATP_HTTP_PacketSetHeader(
                                    ATP_HTTP_PACKET_ST      *pstPacket,
                                    VOS_BOOL                bOverwrite,
                                    VOS_INT32               lHeaderIdx,
                                    const VOS_CHAR          *pcHeaderValue);

VOS_BOOL ATP_HTTP_ReadChunkLen(
                                VOS_CHAR        *pcBuffer,
                                VOS_INT32       lBufferLen,
                                VOS_INT32       *plChunkHeaderLen,
                                VOS_INT32       *plChunkLen);

VOS_UINT32 ATP_HTTP_PacketBuildHeaderEx(
                        ATP_UTIL_STRFILE_ST         *pstStrBuffer,
                        const ATP_HTTP_PACKET_ST    *pstPacket,
                        VOS_INT32                   lContentLen,
                        VOS_INT32                   *plLen,
                        VOS_UINT32                  ulType);

VOS_UINT32 ATP_HTTP_PacketBuildHeader(
                        ATP_UTIL_STRFILE_ST         *pstStrBuffer,
                        const ATP_HTTP_PACKET_ST    *pstPacket,
                        VOS_INT32                   lContentLen,
                        VOS_INT32                   *plLen);

VOS_UINT32 ATP_HTTP_PacketParseOnlyHeader(
                                    ATP_HTTP_PACKET_ST      *pstPacket,
                                    VOS_CHAR                *buffer,
                                    VOS_INT32               *plHeaderLen);

VOS_UINT32 ATP_HTTP_PacketParseHeader(
                                    ATP_HTTP_PACKET_ST      *pstPacket,
                                    VOS_CHAR                *buffer,
                                    VOS_INT32               length,
                                    VOS_BOOL                *pbIsChunkMode,
                                    VOS_INT32               *plHeaderLen);

/*
 *  HTTP Authorization Module
 */


#if 1//#ifdef SUPPORT_ATP_HTTP_AUTH

typedef struct tagATP_HTTP_AUTH_INFO_ST
{
    VOS_CHAR                *pcUsername;
    VOS_CHAR                *pcPassword;
} ATP_HTTP_AUTH_INFO_ST;


#define ATP_HTTP_AUTH_DEFAULT_REALM         "HuaweiHomeGateway"
#define ATP_HTTP_AUTH_MAX_RETRY_CNT         (3)

typedef enum tagE_E_HTTPAUTH_ERR_CODE
{
    E_HTTPAUTH_DIGEST_ERR_realm,
    E_HTTPAUTH_DIGEST_ERR_nonce,
    E_HTTPAUTH_DIGEST_ERR_qop,
    E_HTTPAUTH_DIGEST_ERR_username,
    E_HTTPAUTH_DIGEST_ERR_uri,
    E_HTTPAUTH_DIGEST_ERR_cnonce,
    E_HTTPAUTH_DIGEST_ERR_nc,
    E_HTTPAUTH_DIGEST_ERR_response,
    E_HTTPAUTH_DIGEST_ERR_algorithm,
    E_HTTPAUTH_DIGEST_ERR_opaque,

    // For extention
    E_HTTPAUTH_ERR_PARA,
    E_HTTPAUTH_ERR_SYNTAX,
    E_HTTPAUTH_ERR_MEMORY,

    E_HTTPAUTH_ERR_SYS,

    E_HTTPAUTH_ERR_CREDENTIAL,

    E_HTTPAUTH_ERR_NONE
} E_HTTPAUTH_ERR_CODE;

typedef enum tagHTTPDigest_ConfigBits
{
    HTTPDigest_Config_Qop_Auth                      = 0x0001,
    HTTPDigest_Config_Qop_Auth_Int                  = 0x0002,
    HTTPDigest_Config_Qop_Auth_None                 = 0x0004,
    HTTPDigest_Config_Qop_Mask                      = 0x00FF,

    HTTPDigest_Config_Algorithm_MD5                 = 0x0100,
    HTTPDigest_Config_Algorithm_MD5_sess            = 0x0200,
    HTTPDigest_Config_Algorithm_Mask                = 0xFF00
} HTTPDigest_ConfigBits;

typedef enum tagE_HTTPAUTH_TYPE
{
    E_HTTPAUTH_TYPE_NONE,
    E_HTTPAUTH_TYPE_BASIC,
    E_HTTPAUTH_TYPE_DIGEST,
    E_HTTPAUTH_TYPE_ADAPT,
    E_HTTPAUTH_TYPE_MAX
} E_HTTPAUTH_TYPE;

typedef enum tagHTTPDigest_FieldIndex
{
    HTTPDigest_Field_realm,
    HTTPDigest_Field_nonce,
    HTTPDigest_Field_qop,
    HTTPDigest_Field_username,
    HTTPDigest_Field_uri,
    HTTPDigest_Field_cnonce,
    HTTPDigest_Field_nc,
    HTTPDigest_Field_response,
    HTTPDigest_Field_algorithm,
    HTTPDigest_Field_opaque,
    HTTPDigest_Field_End

} HTTPDigest_FieldIndex;

E_HTTPAUTH_ERR_CODE HTTPAuth_BuildAuthorizationHeader(
                                const VOS_CHAR   *pcAuthenticate,
                                const VOS_CHAR   *pcUsername,
                                const VOS_CHAR   *pcPassword,
                                const VOS_CHAR   *pcMethod,
                                const VOS_CHAR   *pcUri,
                                VOS_CHAR         **ppcAuthorization,
                                E_HTTPAUTH_TYPE  *eAuthType);

E_HTTPAUTH_ERR_CODE HTTPAuth_BuildChallengeHeader(
                                E_HTTPAUTH_TYPE         eAuthType,
                                HTTPDigest_ConfigBits   eAbility,
                                VOS_CHAR                **ppcNonce,
                                VOS_CHAR                **ppcAuthenticate);

E_HTTPAUTH_ERR_CODE HTTPAuth_CheckAuthorization(
                                E_HTTPAUTH_TYPE         eAuthType,
                                const VOS_CHAR          *pcUsername,
                                const VOS_CHAR          *pcPassword,
                                const VOS_CHAR          *pcMethod,
                                const VOS_CHAR          *pcUri,
                                const VOS_CHAR          *pcNonce,
                                const VOS_CHAR          *pcAuthorization);

#endif

/*
 *  HttpClient Module
 */

typedef struct tagATP_HTTP_CLIENT_ST ATP_HTTP_CLIENT_ST;

typedef VOS_UINT32 (*PfHttpClientDataProc)(
                                      ATP_HTTP_CLIENT_ST   *pstHttpClient);

typedef VOS_VOID (*PfHttpClientErrProc)(
                                      ATP_HTTP_CLIENT_ST   *pstHttpClient,
                                      VOS_UINT32           ulErrCode);

typedef VOS_UINT32 (*PfHttpClientRequestPreProc)(
                                      ATP_HTTP_CLIENT_ST   *pstHttpClient,
                                      ATP_HTTP_PACKET_ST   *pstHttpRequestPacket);

struct tagATP_HTTP_CLIENT_ST
{
    /*
         *  Socket Handler
         */
    ATP_SOCKET_ST           stSocket;
    PfHttpClientDataProc    pfDataProc;
    PfHttpClientErrProc     pfErrProc;
    VOS_VOID                *pvHook;

    /*
     *   Address Related Fields
     */
    struct addrinfo          *pstLocalAddr;
    struct addrinfo          *pstRemoteAddr;

    VOS_UINT32              ulRemotePort;
    const VOS_CHAR        *pcHost;

    /*
     *   Recv Buffer
     */
    VOS_CHAR                *pcBuf;
    VOS_INT32               lBufferLen;

    /*
     *  Send Buffer
     */
    ATP_HTTP_PACKET_ST      *pstRecvPacket;     /* 接收到报文的报头 */
    VOS_BOOL                       bBodyRecvd;
    ATP_UTIL_STRFILE_ST     *pstSendStrFile;    /* 发送给服务器的报体 */
    ATP_HTTP_PACKET_ST      stHttpRequest;      /* 发送给服务器的报头 */

    /*
     *  Sockbase Pointer
     */
    ATP_SOCK_BASE_ST        *pstSockBase;

    /*
     *  Timeout timer
     */
    ATP_UTIL_TIMER_ST       *pstTimer;
    VOS_UINT32              ulDftTimeout;

    /*
     *  HTTP authorization related parameters
     */
#if 1//#ifdef SUPPORT_ATP_HTTP_AUTH
    ATP_HTTP_AUTH_INFO_ST   *pstAuthInfo;       /* 使用结构体便于节省内存 */
    VOS_UINT16              usAuthCnt;
    VOS_UINT16              usAuthMode;
    VOS_CHAR                *pcNonce;
#endif
    /*
     *  HTTP redirect url
     */
    VOS_CHAR                *pcRedirectUrl;
};

#ifdef SUPPORT_ATP_SSL
typedef VOS_BOOL (*PfHttpsNegotiationCheck)(ATP_SOCKET_ST *pstSocket);
extern PfHttpsNegotiationCheck g_pfHttpsCheck;
#endif

#if 1//#ifdef SUPPORT_ATP_HTTP_AUTH
VOS_UINT32 ATP_HTTP_ClientCheckAuthorization(
                                    ATP_HTTP_CLIENT_ST          *pstClient,
                                    const ATP_HTTP_PACKET_ST    *pstHttpPacket,
                                    const ATP_HTTP_AUTH_INFO_ST *pstAuthInfo,
                                    VOS_BOOL                    *pbIsChanllenge);
#endif

ATP_HTTP_CLIENT_ST *ATP_HTTP_ClientCreate(
                    const ATP_SOCKET_ST     *pstSock,
                    PfHttpClientDataProc    pfDataProc,
                    PfHttpClientErrProc     pfErrProc,
                    VOS_VOID                *pvHook,
                    VOS_BOOL                bListen);

VOS_VOID ATP_HTTP_ClientDestroy(ATP_HTTP_CLIENT_ST *pstHttpClient);

VOS_UINT32 ATP_UTIL_SocketConnectAsync(
                                ATP_SOCKET_ST       *pstSocket,
                                const struct addrinfo *pstAddr);
VOS_UINT32 ATP_HTTP_ClientConnect(ATP_HTTP_CLIENT_ST *pstHttpClient);
VOS_UINT32 ATP_HTTP_ClientSetSocket(ATP_HTTP_CLIENT_ST       *pstHttpClient,
                                              const ATP_SOCKET_ST       *pstSock);
VOS_VOID ATP_HTTP_ClientDisconnect(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_UINT32 ulErrCode);

VOS_UINT32 ATP_HTTP_ClientSendEx(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                        VOS_UINT32         ulType);

VOS_UINT32 ATP_HTTP_ClientSend(ATP_HTTP_CLIENT_ST    *pstHttpClient);

ATP_UTIL_STRFILE_ST *ATP_HTTP_ClientGetSendBuf(ATP_HTTP_CLIENT_ST *pstHttpClient);

VOS_UINT32 ATP_HTTP_ClientRecvAllBody(
                                    ATP_HTTP_CLIENT_ST          *pstHttpClient,
                                    const ATP_HTTP_PACKET_ST    *pstRecvPacket,
                                    VOS_CHAR                    **ppcRecvBuf,
                                    VOS_INT32                   *plBufLen);

VOS_UINT32 ATP_HTTP_ClientRecvPartOfBody(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        VOS_CHAR                *pcRecvBuf,
                                        VOS_INT32               lBufLen);

VOS_UINT32 ATP_HTTP_ClientRecvLine(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        VOS_CHAR                *pcRecvBuf,
                                        VOS_INT32               lBufLen,
                                        VOS_INT32               *plRecvLen);

VOS_UINT32 ATP_HTTP_ClientRecvBlockOfData(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        VOS_CHAR                *pcRecvBuf,
                                        VOS_INT32               lBufLen,
                                        VOS_INT32               *plBytesRecvd);

VOS_UINT32 ATP_HTTP_ClientUnRecvData(
                                        ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                        const VOS_CHAR          *pcRecvBuf,
                                        VOS_INT32               lBufLen);

VOS_UINT32 ATP_HTTP_ClientRecvChunkHeader(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                                    VOS_INT32          *plChunkLen);

VOS_UINT32 ATP_HTTP_ClientRecvHeader(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                             ATP_HTTP_PACKET_ST *pstRecvPacket);

#if 1//#ifdef SUPPORT_ATP_HTTP_AUTH
VOS_UINT32 ATP_HTTP_ClientSetAuthInfo(
                                ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                const VOS_CHAR          *pcUserName,
                                const VOS_CHAR          *pcPassword);
#endif

VOS_UINT32 ATP_HTTP_ClientSetUrl(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                      const VOS_CHAR     *pcUrl);

VOS_UINT32 ATP_HTTP_ClientSetUrlEx(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                      const VOS_CHAR     *pcUrl, VOS_BOOL bIgnorIpv6);

/*
 *  HttpServer Module
 */
typedef VOS_UINT32 (*PFHttpServerOnData)(
                                ATP_HTTP_CLIENT_ST      *pstHttpClient,
                                VOS_VOID                *pvHook);

typedef struct tagATP_HTTP_SERVER_ST
{
    PFHttpServerOnData      pfDataProc;
    VOS_VOID                *pvHook;

    /*
     *  Sockets to be listened
     */
    VOS_VOID                *pvSockList;
    VOS_VOID                *pvHttpClientList;
    VOS_UINT32              ulConnectionCnt;

    VOS_UINT32              ulMaxClients;

    /*
     *  HTTP authorization related parameters
     */
#if 1//#ifdef SUPPORT_ATP_HTTP_AUTH
    ATP_HTTP_AUTH_INFO_ST   *pstAuthInfo;       /* 使用结构体便于节省内存 */
    VOS_UINT16              usAuthMode;
#endif

    VOS_UINT32              ulDftTimeout;
} ATP_HTTP_SERVER_ST;

ATP_HTTP_SERVER_ST *ATP_HTTP_ServerCreate(PFHttpServerOnData pfProc, VOS_VOID *pvHook);


VOS_UINT32 ATP_HTTP_ServerAppendSocket(
                        ATP_HTTP_SERVER_ST              *pstServer,
                        const ATP_SOCKET_ST             *pstSocket);

VOS_VOID ATP_HTTP_ServerStop(
                        ATP_HTTP_SERVER_ST              *pstServer);

VOS_UINT32 ATP_HTTP_ServerSetAuthInfo(ATP_HTTP_SERVER_ST *pstServer,
                                            const VOS_CHAR      *pcUserName,
                                            const VOS_CHAR      *pcPassword);
typedef VOS_UINT32 (*PfHttpServerAuthProc)(
                                      ATP_HTTP_CLIENT_ST   *pstHttpClient,
                                      ATP_HTTP_PACKET_ST   *pstRecvPacket,
                                      VOS_BOOL             *pbIsChanllenge);
extern PfHttpServerAuthProc g_pfServerAuthProc;
struct addrinfo *ATP_UTIL_CopyAddrInfo(const struct addrinfo *pstSrc, VOS_BOOL bCopyList);

VOS_BOOL ATP_UTIL_IsAddrInfoSame(const struct addrinfo *pstAddrL, const struct addrinfo *pstAddrR);

struct addrinfo *ATP_UTIL_ParseIPAddress(
                                ATP_SOCK_FLAG_EN      enSockType,
                                const VOS_CHAR           *pcAddr,
                                VOS_UINT32                 ulPort);

struct addrinfo *ATP_UTIL_MakeAddrInfo(
                                VOS_UINT32               ulSockFlag,
                                const struct sockaddr   *pstSockAddr,
                                socklen_t                   socklen);

VOS_BOOL ATP_UTIL_IsAddrLoopback(const struct addrinfo *pstAddr);

VOS_UINT32 HttpClientDoAuthorization(ATP_HTTP_CLIENT_ST        *pstHttpClient,
                                            const ATP_HTTP_PACKET_ST  *pstPacket);

VOS_UINT32 ATP_HTTP_ClientRedirect(ATP_HTTP_CLIENT_ST *pstHttpClient,
                                                                     const ATP_HTTP_PACKET_ST  *pstPacket);
/* Start Add by l00163811 2013-8-28 */
VOS_UINT32 ATP_UTIL_GetIntfByIPAddr(const VOS_CHAR *pcIpAddr, 
												VOS_CHAR *pcIntfName, 
												VOS_UINT32 ulIntfLen);


VOS_UINT32 ATP_UTIL_GetIntfBySockAddr(const struct addrinfo *pstSockAddr, 
												VOS_CHAR *pcIntfName, 
												VOS_UINT32 ulIntfLen);
/* End Add by l00163811 2013-8-28 */


typedef VOS_VOID (*PfHttpSockBaseLoopHooker)(VOS_VOID);
extern ATP_THREAD_SAFE PfHttpSockBaseLoopHooker g_pfHttpSockBaseLoopProc;
extern ATP_THREAD_SAFE PfHttpSockBaseLoopHooker g_pfHttpSockBaseLoopProcForCms;
#ifdef ATP_DEBUG
#define ATP_HTTP_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_HTTP_DEBUG(x...)
#endif

#define ATP_HTTP_TRACE ATP_MSG_Printf

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif

