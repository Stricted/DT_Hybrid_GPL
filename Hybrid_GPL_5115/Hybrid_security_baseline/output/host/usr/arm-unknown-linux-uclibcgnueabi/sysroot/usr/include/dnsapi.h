#ifndef __ATP_DNSNEW_API_H__
#define __ATP_DNSNEW_API_H__

#include "atptypes.h"
#include "msgapi.h"
#include "httpapi.h"

//调试开关
//#define ATP_DEBUG
//#undef ATP_DEBUG
#define ATP_DNS_DEFAULT_PORT    (53)
#define DNS_ADDR_LEN        (64)

/*0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                      ID                       |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                    QDCOUNT                    |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                    ANCOUNT                    |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                    NSCOUNT                    |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                    ARCOUNT                    |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-*/

/* DNS header flags */
#define ATP_DNS_FLAGS_QR        0x8000    /* response flag */    
#define ATP_DNS_FLAGS_OPCODE    0x7800    /* purpose of message */
#define ATP_DNS_FLAGS_AA        0x0400    /* authoritive answer */
#define ATP_DNS_FLAGS_TC        0x0200    /* truncated message */
#define ATP_DNS_FLAGS_RD        0x0100    /* recursion desired */
#define ATP_DNS_FLAGS_RA        0x0080    /* recursion available */
#define ATP_DNS_FLAGS_Z         0x0070    /* unused bits (MBZ as of 4.9.3a3) */
#define ATP_DNS_FLAGS_RCODE     0x000f    /* response code */

/* return code */
#define ATP_DNS_GET_STATUS_CODE(x)  ((x) & 0x000F)

#define ATP_DNS_RCODE_NOERROR       0
#define ATP_DNS_RCODE_SERVFAIL      2
#define ATP_DNS_RCODE_NO_SUCH_NAME  3
#define ATP_DNS_RCODE_REFUSED       5

/* dns type */
#define ATP_DNS_TYPE_A          1
#define ATP_DNS_TYPE_NS         2
#define ATP_DNS_TYPE_CNAME      5
#define ATP_DNS_TYPE_SOA        6
#define ATP_DNS_TYPE_PTR        12
#define ATP_DNS_TYPE_AAAA       28
#define DNS_TYPE_OPT            0x29


/* dns class */
#define ATP_DNS_CLASS_IN        1

#define ATP_DNS_NAME_COMPRESS_D     (0xC0)

#define ATP_DNS_NAME_TOTAL_LEN_D    (255)
#define ATP_DNS_NAME_LABEL_LEN_D    (63)

#define ATP_IFC_NAME_LEN_D          (16)
#define ATP_DNS_MIN_TTL             (30)

/* dns header */
typedef struct
{
    unsigned short usId;            /* query identification number */
    unsigned short usFlags;         /* flags */
    unsigned short usQdCount;       /* number of question entries */
    unsigned short usAnCount;       /* number of answer entries */
    unsigned short usNsCount;       /* number of authority entries */
    unsigned short usArCount;       /* number of resource entries */
    unsigned char pucQuery[0];
} ATP_DNS_HEADER_ST;

typedef enum tagATP_DNS_RET_CODE_EN
{
    // These errors are reserved for dns only
    ATP_DNS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_DNS),
    ATP_DNS_RET_MEM_ERR,
    ATP_DNS_RET_NOT_INITED,
    ATP_DNS_RET_SOCK_ERR,
    ATP_DNS_RET_NOT_SPPORTED,
    ATP_DNS_RET_RECV_ERR,
    ATP_DNS_RET_INTERNEL_ERR,
    ATP_DNS_RET_EXPIRED,
    ATP_DNS_RET_RESOURCE_EXCEED,
    ATP_DNS_RET_FAKE_PACKET,
    ATP_DNS_RET_NO_RELAY_NEEDED,
    ATP_DNS_RET_NEED_TO_CONTINUE,
} ATP_DNS_RET_CODE_EN;

typedef struct tagATP_DNS_SOCKET_ST ATP_DNS_SOCKET_ST;

/*
 *  DNS Packet api
 */
typedef struct tagATP_DNS_PACKET_ST
{
    VOS_UINT32          ulLen;
    ATP_DNS_HEADER_ST   pstHead[0];
} ATP_DNS_PACKET_ST;

VOS_UINT32 ATP_DNS_PacketRecvUdp(ATP_SOCKET_ST *pstSocket, ATP_DNS_PACKET_ST **ppstPacket, struct addrinfo **ppstFromAddr);
VOS_UINT32 ATP_DNS_PacketRecvTcp(ATP_SOCKET_ST *pstSocket, ATP_DNS_PACKET_ST **ppstPacket, struct addrinfo **ppstFromAddr);

ATP_DNS_PACKET_ST *ATP_DNS_PacketCopy(const ATP_DNS_PACKET_ST *pstPacket, VOS_UINT32 ulTrunkSize);

VOS_UINT32 ATP_DNS_PacketGetTTL(const ATP_DNS_PACKET_ST *pstPacket);

ATP_DNS_PACKET_ST *ATP_DNS_PacketUpdateTTL(const ATP_DNS_PACKET_ST *pstPacket, VOS_UINT32 ulGenTime,
                                                      VOS_UINT32 ulTrunkSize);

/*
 *  DNS name interface, do compressed name process
 */
VOS_UINT32 ATP_DNS_PacketGetNameLen(const ATP_DNS_PACKET_ST *pstPacket, const VOS_UINT8 *pucNamePos);

VOS_UINT32 ATP_DNS_PacketGetName(const ATP_DNS_PACKET_ST *pstPacket, const VOS_UINT8 *pucNamePos,
                    VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen, VOS_UINT32 *pulNameLen);

/*
 *  DNS resource records api
 */
typedef struct tagATP_DNS_RECORDS_ST
{
    VOS_CHAR        acName[ATP_DNS_NAME_TOTAL_LEN_D + 1];
    VOS_UINT32      ulType;
    VOS_UINT32      ulClass;
    VOS_UINT32      ulTTL;
    VOS_UINT32      ulDataLen;
    const VOS_UINT8 *pucData;
} ATP_DNS_RECORDS_ST;

VOS_UINT32 ATP_DNS_PacketGetValuesOfRR(const ATP_DNS_PACKET_ST *pstPacket,
                    const VOS_UINT8 *pucRRPos, ATP_DNS_RECORDS_ST *pstRR,
                    VOS_UINT32 *pulRRLen);

VOS_UINT32 ATP_DNS_PacketGetRRLen(const ATP_DNS_PACKET_ST *pstPacket, const VOS_UINT8 *pucRRPos);

const VOS_UINT8 *ATP_DNS_PacketGetFirstRR(const ATP_DNS_PACKET_ST *pstPacket);

const VOS_UINT8 *ATP_DNS_PacketGetNextRR(const ATP_DNS_PACKET_ST *pstPacket, const VOS_UINT8 *pucCurRR);

const VOS_UINT8 *ATP_DNS_PacketGetFirstAddtionalRecord(const ATP_DNS_PACKET_ST *pstPacket);

const VOS_UINT8 *ATP_DNS_PacketGetFirstAuthorityRecord(const ATP_DNS_PACKET_ST *pstPacket);

#ifdef SUPPORT_ATP_DNS_NOT_CACHE_NS
ATP_DNS_PACKET_ST *ATP_DNS_PacketCopyWithoutNS(const ATP_DNS_PACKET_ST *pstPacket, 
                                    VOS_UINT32 *pulTrunkSize, VOS_BOOL *pIsTrunked);

#endif

VOS_UINT32 ATP_DNS_PacketPTRName2Addr(const VOS_CHAR* pcQueryName, VOS_CHAR* pcAddr, VOS_UINT32 ulLen, VOS_BOOL *pbIPv4);
/*
 *  DNS question api
 */
typedef struct tagATP_DNS_QUESTION_ST
{    
    VOS_CHAR        acName[ATP_DNS_NAME_TOTAL_LEN_D + 1];
    VOS_UINT32      ulType;
    VOS_UINT32      ulClass;
} ATP_DNS_QUESTION_ST;

VOS_UINT32 ATP_DNS_PacketGetValuesOfQuestion(const ATP_DNS_PACKET_ST *pstPacket,
                    const VOS_UINT8 *pucQuestionPos, ATP_DNS_QUESTION_ST *pstQuestion,
                    VOS_UINT32 *pulQuestionLen);

VOS_UINT32 ATP_DNS_PacketGetQuestionLen(const ATP_DNS_PACKET_ST *pstPacket, const VOS_UINT8 *pucQuestion);

ATP_DNS_PACKET_ST *ATP_DNS_PacketBuildErrorResponse(const ATP_DNS_PACKET_ST *pstQuery,
                                                               VOS_UINT32 ulErrCode);

ATP_DNS_PACKET_ST *ATP_DNS_PacketBuildResponse(const ATP_DNS_PACKET_ST *pstQuery,
                                                    const ATP_DNS_RECORDS_ST *pstRR, VOS_UINT32 ulGenTime);

#ifdef ATP_DEBUG
VOS_VOID ATP_DNS_PacketPrintHeader(const ATP_DNS_PACKET_ST *pstPacket);
VOS_VOID ATP_DNS_PacketPrintQuestion(const ATP_DNS_QUESTION_ST *pstQuestion);
VOS_VOID ATP_DNS_PacketPrintRecord(const ATP_DNS_RECORDS_ST *pstRecord);
VOS_VOID ATP_DNS_PacketPrintAddress(const struct addrinfo *pstAddr);
#else
#define ATP_DNS_PacketPrintHeader(x...)
#define ATP_DNS_PacketPrintQuestion(x...)
#define ATP_DNS_PacketPrintRecord(x...)
#define ATP_DNS_PacketPrintAddress(x...)
#endif

/* dns tunnel */
typedef enum
{
    DNS_TUNNEL_NONE = 0,
    DNS_TUNNEL_6TO4 = 1,
    DNS_TUNNEL_DSLITE = 2,    
}DNS_TUNNEL_EN;

// Forward declaration
typedef struct tagATP_DNS_SERVER_ST ATP_DNS_SERVER_ST;

typedef VOS_UINT32 (* PFDNSDataProc)(ATP_DNS_SERVER_ST *pstServer);

/*
 *  Server Socket List API
 */
typedef enum tagATP_DNS_SOCK_TYPE_EN
{
    ATP_DNS_SOCK_UDP_LISTEN,    // for recv lan Query and reply answer
    ATP_DNS_SOCK_UDP_RELAY,    //for forward query to wan, and recv wan answer
    ATP_DNS_SOCK_TCP_LISTEN,  //for lan initial listen
    ATP_DNS_SOCK_TCP_QUERY,     //accepted, for recv lan query and reply it
    ATP_DNS_SOCK_TCP_RELAY_CONNECT,   //for active open to wan server, it is async connecting
    ATP_DNS_SOCK_TCP_RELAY_SEND,   //for active open to wan server, connected, recving response
} ATP_DNS_SOCK_TYPE_EN;

struct tagATP_DNS_SOCKET_ST
{
    struct tagATP_DNS_SOCKET_ST     *pstNext;

    VOS_UINT32                      ulSocketType;   // Of type ATP_DNS_SOCK_TYPE_EN
    ATP_SOCKET_ST                   stServerSock;
    ATP_SOCK_BASE_ST                *pstSockBase;
    VOS_VOID                        *pvHook;
    ATP_UTIL_TIMER_ST               *pstTimer;
};

VOS_VOID ATP_DNS_ServerSocketDestroy(ATP_UTIL_LIST_ST *pstItem);

#define ATP_DNS_ServerSocketDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), ATP_DNS_ServerSocketDestroy)

/*
 * DNS WAN Info List API
 */
typedef struct tagATP_DNS_WAN_INFO_ST
{
    struct tagATP_DNS_WAN_INFO_ST *pstNext;

    const VOS_CHAR  *pcBindInterface;
    const VOS_CHAR  *pcServiceType;
    const VOS_CHAR  *pcBindBr;
    struct addrinfo *pstWanAddr;
    struct addrinfo *pstServerAddrList;
} ATP_DNS_WAN_INFO_ST;

VOS_UINT32 ATP_DNS_WanInfoAppend(ATP_DNS_SERVER_ST *pstServer,  const VOS_CHAR *pcDnsAddress, const VOS_CHAR *pcWanIP,
                                 const VOS_CHAR *pcIntfName, const VOS_CHAR *pcServiceType, const VOS_CHAR* pcBindBr, ATP_DNS_WAN_INFO_ST **ppstInfo);

ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetByAddr(const ATP_DNS_SERVER_ST *pstServer,
                        const struct addrinfo *pstAddr);
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetNextBindWan(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_WAN_INFO_ST *pstPrevWan, VOS_INT32 ulFamily);
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetNextBindWanIpv6(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_WAN_INFO_ST *pstPrevWan);


struct addrinfo *ATP_DNS_WanInfoFindNthServerAddress(const ATP_DNS_WAN_INFO_ST *pstWanInfo, VOS_UINT32 ulIdx);

ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetByLANPort(const ATP_DNS_SERVER_ST *pstServer, const VOS_CHAR *pcIntf, ATP_DNS_WAN_INFO_ST *pstWan, VOS_INT32 ulFamily);
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetByLANPortIpv6(const ATP_DNS_SERVER_ST *pstServer, const VOS_CHAR *pcIntf, ATP_DNS_WAN_INFO_ST *pstWan);


ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetNextInternetWan(const ATP_DNS_SERVER_ST *pstServer, ATP_DNS_WAN_INFO_ST *pstWan);
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetNextInternetIPv6Wan(const ATP_DNS_SERVER_ST *pstServer, ATP_DNS_WAN_INFO_ST *pstWan);


VOS_VOID ATP_DNS_WanInfoDestroy(ATP_UTIL_LIST_ST *pstItem);

ATP_DNS_SOCKET_ST *ATP_DNS_ServerFindByWanInfo(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_WAN_INFO_ST *pstWanInfo);

VOS_VOID ATP_DNS_ServerDeleteRelaySockets(ATP_DNS_SERVER_ST *pstServer);

#define ATP_DNS_WanInfoDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), ATP_DNS_WanInfoDestroy)
#define ATP_DNS_WanInfoDeleteAll(list) ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), ATP_DNS_WanInfoDestroy); (list) = NULL;

ATP_DNS_WAN_INFO_ST* ATP_DNS_WanInfoStrip(ATP_DNS_WAN_INFO_ST **ppstWanList, const ATP_DNS_WAN_INFO_ST *pstWan);                
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoFind(ATP_DNS_WAN_INFO_ST *pstWanList, const ATP_DNS_WAN_INFO_ST *pstWan);
ATP_DNS_WAN_INFO_ST *ATP_DNS_WanInfoGetByInterfaceAndType(const ATP_DNS_SERVER_ST *pstServer,
                        const VOS_CHAR *pcIntf, VOS_UINT32 ulAddrType);

/*
 *  DNS Query List API
 */
typedef struct tagATP_DNS_QUERY_ST
{
    struct tagATP_DNS_QUERY_ST     *pstNext;
    VOS_UINT64          ullGenTime;              // The generation time of this record
    VOS_UINT64          ullTimeout;               // Meaningful only for cache
    ATP_DNS_PACKET_ST   *pstPacket;
    struct addrinfo     *pstFromAddr;        // need to be freed for UDP socket;
    ATP_DNS_WAN_INFO_ST *pstFromWan;        //是否是与wan绑定的服务，如ACS，或者voice等程序发的查询包
    VOS_BOOL            bIsFromLoopback;
    ATP_DNS_SOCKET_ST   *pstServerSocket;    // need to be destroied for TCP socket
    ATP_DNS_SOCKET_ST   *pstRelaySocket;
    VOS_CHAR            acLANPort[ATP_IFC_NAME_LEN_D];

    VOS_UINT8           ucTryCnt;           // Relay重试的次数
    VOS_UINT8           ucServersNum;       //对于udp的query，表示重试时成功重试的服务器数目，用于判断是否接收到所有的回复。
    //对于tcp的query，表示当前转发的tcp连接对应的是当前wan的第几个服务器，用于tcp的主备切换，因为tcp不易获得当前socket的对端地址。
    //用getpeername函数获取对端地址的方法也可以。
    VOS_UINT16          usFlags;
    ATP_DNS_WAN_INFO_ST *pstFirstRelayWan;
    
    ATP_DNS_PACKET_ST   *pstFirstServerReply;      // response of the first server 
    ATP_DNS_WAN_INFO_ST *pstFirstServerReplyWan;// the wan interface which the FirstServerReply came from
} ATP_DNS_QUERY_ST;

#define ATP_DNS_QUERY_FLAG_NEED_SWAP_D      (0x0001)

#define ATP_DNS_QUERY_TIMEOUT_UDP_D         (2000)
#define ATP_DNS_QUERY_TIMEOUT_TCP_D         (3000)

#define ATP_DNS_MAX_QUERY_NUM_D             (30)

ATP_DNS_QUERY_ST *ATP_DNS_QueryGetNewest(ATP_DNS_QUERY_ST *pstItem);

VOS_UINT32 ATP_DNS_QueryAppend(ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_PACKET_ST *pstPacket,
                                        const struct addrinfo *pstFromAddr, ATP_DNS_SOCKET_ST *pstServerSocket,
                                        VOS_UINT64 ulTimeout, ATP_DNS_QUERY_ST **ppstQuery);

VOS_VOID ATP_DNS_QueryUpdateTimer(ATP_DNS_SERVER_ST *pstServer);

VOS_VOID ATP_DNS_QueryDestroy(ATP_UTIL_LIST_ST *pstItem);

ATP_DNS_QUERY_ST *ATP_DNS_QueryFindByRelaySocket(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_SOCKET_ST *pstRelaySocket);
ATP_DNS_QUERY_ST *ATP_DNS_QueryFindByServerSocket(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_SOCKET_ST *pstServerSocket);

#define ATP_DNS_QueryDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), ATP_DNS_QueryDestroy)
#define ATP_DNS_QueryDeleteAll(list) ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), ATP_DNS_QueryDestroy); (list) = NULL;

ATP_DNS_QUERY_ST *ATP_DNS_QueryFind(ATP_DNS_QUERY_ST *pstQueryList, const ATP_DNS_QUESTION_ST *pstQuestion);
ATP_DNS_QUERY_ST *ATP_DNS_QueryFindNext(const ATP_DNS_QUERY_ST *pstQuery, const ATP_DNS_RECORDS_ST *pstRecord);
VOS_UINT32 ATP_DNS_QueryContain(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_QUERY_ST *pstQuery);
VOS_UINT32 ATP_DNS_WanInfoBindSocket(ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_WAN_INFO_ST *pstWanInfo, ATP_DNS_QUERY_ST *pstQuery);


/*
 *  DNS Cache List API
 */
typedef struct tagATP_DNS_CACHE_ST
{
    struct tagATP_DNS_CACHE_ST     *pstNext;
    VOS_UINT32          ulGenTime;              // The generation time of this record
    VOS_UINT32          ulTimeout;               // Meaningful only for cache
    ATP_DNS_PACKET_ST   *pstPacket;
    ATP_DNS_WAN_INFO_ST *pstWan;
} ATP_DNS_CACHE_ST;

#define ATP_DNS_MAX_CACHE_NUM_D             (30)

ATP_DNS_CACHE_ST *ATP_DNS_CacheGetNewest(ATP_DNS_CACHE_ST *pstItem);

VOS_VOID ATP_DNS_CacheUpdateTimer(ATP_DNS_SERVER_ST *pstServer);

VOS_UINT32 ATP_DNS_CacheAppend(ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_PACKET_ST *pstPacket, ATP_DNS_WAN_INFO_ST *pstWan);

ATP_DNS_CACHE_ST *ATP_DNS_CacheFind(ATP_DNS_CACHE_ST *pstCacheList, const ATP_DNS_QUESTION_ST *pstQuestion, const ATP_DNS_WAN_INFO_ST *pstWan);
VOS_VOID ATP_DNS_CacheDestroy(ATP_UTIL_LIST_ST *pstItem);

#define ATP_DNS_CacheDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), ATP_DNS_CacheDestroy)
#define ATP_DNS_CacheDeleteAll(list) ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), ATP_DNS_CacheDestroy); (list) = NULL;
VOS_UINT32 ATP_DNS_CacheDeleteByWanInfo(ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_WAN_INFO_ST *pstWanInfo);
VOS_UINT32 ATP_DNS_CacheContain(const ATP_DNS_SERVER_ST *pstServer, const ATP_DNS_CACHE_ST *pstCache);


//DNS CACHE 超时监听链表
typedef struct tagATP_DNS_TTL_LISTEN_ST
{
    struct tagATP_DNS_TTL_LISTEN_ST     *pstNext;
    VOS_CHAR    aucSendApp[ATP_MSG_MAX_NAME_LEN + 1];    /* 发送进程名字 */
    VOS_CHAR    *pacName;
    VOS_UINT32  ulType; 
    VOS_UINT32  ulClass;
} ATP_DNS_TTL_LISTEN_ST;

#define ATP_DNS_MAX_LISTEN_NUM_D            (30)

VOS_UINT32 ATP_DNS_TtlListenAppend(ATP_DNS_SERVER_ST *pstServer, const VOS_CHAR *aucSendApp, const ATP_DNS_QUESTION_ST *pstQuestion);

ATP_DNS_TTL_LISTEN_ST *ATP_DNS_TtlListenFind(ATP_DNS_TTL_LISTEN_ST *pstTtlListenList, const VOS_CHAR *aucSendApp, const ATP_DNS_QUESTION_ST *pstQuestion);

VOS_VOID ATP_DNS_TtlListenDestroy(ATP_UTIL_LIST_ST *pstItem);

#define ATP_DNS_TtlListenDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), ATP_DNS_TtlListenDestroy)
#define ATP_DNS_TtlListenDeleteAll(list) ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), ATP_DNS_TtlListenDestroy); (list) = NULL;

VOS_VOID ATP_DNS_TTL_Print_Listen_List(ATP_DNS_TTL_LISTEN_ST *pstTtlListenItem);
VOS_VOID ATP_DNS_Print_Cache_List(ATP_DNS_CACHE_ST *pstCacheItem);
VOS_VOID ATP_DNS_Print_Wan_List(ATP_DNS_WAN_INFO_ST *pstWanList);

VOS_UINT32 DnsGetLanPortByIp(const struct addrinfo *pstAddr, char *acLan, int ulLen);
VOS_UINT32 DnsGetBridgeByIp(const struct addrinfo *pstAddr, char *acBr, int ulLen);

//DNS 缓存过滤的列表
typedef struct tagATP_DNS_FILTER_ST
{
    struct tagATP_DNS_FILTER_ST         *pstNext;
    ATP_DNS_FILTER_MSG_ST               stFilter;
} ATP_DNS_FILTER_ST;
#define ATP_DNS_MAX_FILTER_NUM_D            (30)

//地址过滤判断函数
//pcName:对指定的域名判断是否需要过滤，为NULL时不对域名进行判断
//pcAddr:对指定的地址判断是否需要过滤，为NULL时不对地址进行判断
//ulType:对指定的类型判断是否需要过滤，为0是不对类型进行判断
//需要过滤时，返回VOS_OK，不需要时，返回VOS_NOK
//这3个参数必须有1个不为NULL或不为0
//这个3个参数可以随机组合，
//如当pcName不为NULL且pcAddr不为NULL时，表示对域名和地址都匹配时才需要过滤
VOS_UINT32 ATP_DNS_Filter(VOS_CHAR *pcName, VOS_CHAR *pcAddr, VOS_UINT32 ulType, ATP_DNS_FILTER_ST *pstFilterList);
VOS_UINT32 ATP_DNS_FilterAppend(ATP_DNS_SERVER_ST *pstServer, ATP_DNS_FILTER_MSG_ST *pstFilterMsg);
ATP_DNS_FILTER_ST *ATP_DNS_FilterFind(ATP_DNS_FILTER_ST *pstFilterList, const ATP_DNS_FILTER_MSG_ST *pstFilterMsg);
#define ATP_DNS_FilterDelete(list, item) ATP_UTIL_ListDeleteItem((ATP_UTIL_LIST_ST **)(&(list)), (ATP_UTIL_LIST_ST *)(item), NULL)
#define ATP_DNS_FilterDeleteAll(list) ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), NULL); (list) = NULL;


//全局dns信息
struct tagATP_DNS_SERVER_ST
{
    // All WAN info
    ATP_DNS_WAN_INFO_ST    *pstWanList;         // All WANs are listed here

    // All dns sockets
    ATP_DNS_SOCKET_ST      *pstServerSockList;   //include udp sock,  tcp listen sock, and subsequent accepted fds to be append to itself, share same pfProc func

    // Query List
    ATP_DNS_QUERY_ST       *pstQueryList;      // From who(sock fd), trans id, query domain name...
    VOS_UINT32             ulMaxQuerries;
    ATP_UTIL_TIMER_ST      *pstQueryTimer;
    ATP_DNS_QUERY_ST       *pstNewestQuery;

    // Cache List
    ATP_DNS_CACHE_ST       *pstCacheList;    // ATP_DNS_CACHE_ITEM_ST
    VOS_UINT32             ulMaxCaches;
    ATP_UTIL_TIMER_ST      *pstCacheTimer;
    ATP_DNS_CACHE_ST       *pstNewestCache;
    
    //TTL listen list
    ATP_DNS_TTL_LISTEN_ST   *pstTtlListenList;
    VOS_UINT32              ulMaxListers;

    VOS_UINT32              ulMinTTL; //最小ttl可配置

    //过滤器，哪些报文不缓存
    ATP_DNS_FILTER_ST       *pstFilterList;
    VOS_UINT32               ulMaxFilters;
};

VOS_UINT32 ATP_DNS_ServerSendPacket(ATP_DNS_SOCKET_ST *pstRelaySocket, ATP_DNS_PACKET_ST *pstDnsPacket, const struct addrinfo *pstDestAddr);

ATP_DNS_SERVER_ST *ATP_DNS_Create(VOS_VOID);


VOS_UINT32 ATP_DNS_AppendSocket(
                    ATP_DNS_SERVER_ST               *pstServer,
                    const ATP_SOCKET_ST             *pstSocket,
                    VOS_UINT32                      ulSockType,
                    VOS_VOID                        *pvHook,
                    ATP_UTIL_TIMER_ST               *pstTimer,
                    ATP_DNS_QUERY_ST                *pstQuery,
                    ATP_DNS_SOCKET_ST               **ppstSocket);

extern ATP_DNS_SERVER_ST *g_pstServer;

VOS_UINT32 DnsDoTcpRelay(ATP_DNS_QUERY_ST *pstQuery, const VOS_CHAR *pcLANPort);
VOS_UINT32 DnsUdpRetry(ATP_DNS_QUERY_ST *pstQuery, VOS_BOOL bIsTimeoutOrErrorReply);

/*
 *  DNS adapt functions
 */
VOS_UINT32 ATP_DNS_AdaptDoRelay(const ATP_DNS_SOCKET_ST *pstServerSocket, ATP_DNS_QUERY_ST *pstQuery);

VOS_VOID ATP_DNS_AdaptQueryTimeout(VOS_VOID *pvHook);

VOS_VOID ATP_DNS_AdaptCacheTimeout(VOS_VOID *pvHook);

VOS_UINT64 ATP_DNS_AdaptGetQueryTimeout(const ATP_DNS_QUERY_ST *pstQuery, VOS_UINT32 ulSocketType);

ATP_DNS_WAN_INFO_ST *ATP_DNS_AdaptGetFirstWanByLanIp(const ATP_DNS_SERVER_ST *pstServer, const struct addrinfo *pstLanAddr);

VOS_UINT32 ATP_DNS_AdaptCheckPacket(const ATP_DNS_PACKET_ST *pstPacket);
#ifdef SUPPORT_ATP_DNS_SWAPSERVER
VOS_VOID ATP_DNS_AdaptSwapServer(ATP_DNS_WAN_INFO_ST *pstWan);
#endif
ATP_DNS_CACHE_ST *ATP_DNS_AdaptFindCahe(const ATP_DNS_SERVER_ST *pstServer, const struct addrinfo *pstFromAddr,
                                        const ATP_DNS_QUESTION_ST *pstQuestion);
VOS_UINT16 ATP_DNS_AdaptGetRandomNum(const VOS_UINT16 ulMin, const VOS_UINT16 ulMax, const VOS_UINT16 ulDev);

#ifdef SUPPORT_ATP_DNS_BIT20
VOS_VOID DnsBit20ChangeName(ATP_DNS_QUERY_ST *pstQuery, VOS_CHAR* pcBakName, VOS_UINT32 ulLen);
VOS_VOID DnsBit20RevertQueryName(ATP_DNS_PACKET_ST* pstPacket, VOS_CHAR* pcBakName);
VOS_VOID DnsBit20RevertReplyName(ATP_DNS_PACKET_ST* pstRely, const ATP_DNS_QUERY_ST *pstQuery);
#endif

#ifdef SUPPORT_ATP_DNS_SERVERPRIORITY_W724V
VOS_VOID DnsAdaptDTErrorReplyLog(const ATP_DNS_WAN_INFO_ST *pstWan, const struct addrinfo *pstAddr, VOS_UINT32 ulRcode);
#endif

VOS_VOID ATP_DNS_AdaptGetInfo();

const VOS_CHAR * ATP_DNS_GetVersion(void);

#ifdef ATP_DEBUG
#define ATP_DNS_DEBUG(format, args...)       {struct timeval cur_time1; ATP_UTIL_GetSysTime(&cur_time1);printf("[tv_sec:%ld tv_usec:%ld] FILE: %s FUNC: %s LINE: %d: ", cur_time1.tv_sec, cur_time1.tv_usec, __FILE__,__func__, __LINE__);printf(format, ##args);}
#else
#define ATP_DNS_DEBUG(x...)
#endif

#define ATP_DNS_TRACE       ATP_MSG_Printf

#endif

