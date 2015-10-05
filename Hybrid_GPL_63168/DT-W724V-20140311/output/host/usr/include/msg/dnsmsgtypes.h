#ifndef __ATP_DNS_MSGTYPES_H__
#define __ATP_DNS_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_CBBID_DNS_NAME "dns"


#define DNS_WILDCARD "*"

#define DNS_BR_LEN          (8)
#define DNS_ADDR_LEN        (64)
#define DNS_BIND_LEN        (128)
#define DNS_CMD_LEN         (512)
#define DNS_IFC_LEN         (32)
#define DNS_SEQ_LEN          (11)
#define DNS_NAME_LEN        (256)//最大支持256个可见字符
#define DNS_WORD_LEN        (64)//从文件中每行文本中读取的单词长度   

/*Start of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/
#define DNS_STRING_LEN_1024 1024
/*End of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/

#define ATP_DNS_FAKE_ADDR   "10.10.10.10"
#ifdef SUPPORT_ATP_IPV6
#define ATP_DNS_FAKE_ADDR6  "2001::10:10:10:10"
#endif

#ifdef SUPPORT_ATP_FON
#define PORT_RANDOM_FON_MAX         (65000)
#define PORT_RANDOM_FON_MIN         (55000)
#define DNS_FON_MARK 0x1
#define DNS_FON_BIND_IF "br0"
#endif

typedef struct tagATP_DNS_SERVER_MSG_ST
{
    VOS_CHAR acDnsServers[DNS_BIND_LEN];//可能有两个地址，中间用逗号分隔
    VOS_CHAR acWanAddr[DNS_ADDR_LEN];
    VOS_CHAR acInterface[DNS_IFC_LEN];
    VOS_CHAR acServiceType[DNS_BIND_LEN];   
    VOS_CHAR acBindBr[DNS_BR_LEN];
} ATP_DNS_SERVER_MSG_ST;

typedef struct tagATP_DNS_STATIC_WAN_INFO_ST
{
    VOS_CHAR        acServerAddr[DNS_BIND_LEN];     
    VOS_CHAR        acBindInterface[DNS_IFC_LEN];    
    VOS_CHAR        acSeq[DNS_SEQ_LEN]; 
    VOS_BOOL        bEnable;
    VOS_BOOL        bManul;
    VOS_BOOL        bIpType; 
    VOS_BOOL        bWebType;
} ATP_DNS_STATIC_WAN_INFO_ST;

typedef struct tagATP_DNS_STATIC_WAN_INFO_LIST_ST
{
    struct tagATP_DNS_STATIC_WAN_INFO_LIST_ST *pstNext;
    ATP_DNS_STATIC_WAN_INFO_ST stStaticWanInfo;
} ATP_DNS_STATIC_WAN_INFO_LIST_ST;


typedef struct tagATP_DNS_REDIRECT_MSG_ST
{
    VOS_CHAR    acAddr[DNS_ADDR_LEN];
    VOS_UINT32  ulTtl;
    VOS_CHAR    acLan[DNS_IFC_LEN];   
} ATP_DNS_REDIRECT_MSG_ST;

typedef struct tagATP_DNS_RESIDENT_MSG_ST
{
    VOS_CHAR    acName[DNS_NAME_LEN];
    VOS_CHAR    acAddr[DNS_ADDR_LEN];
    VOS_UINT32  ulTtl;
} ATP_DNS_RESIDENT_MSG_ST;

typedef struct tagATP_DNS_FILTER_MSG_ST
{
    VOS_CHAR    acName[DNS_NAME_LEN];
    VOS_CHAR    acAddr[DNS_ADDR_LEN];
    VOS_UINT32  ulType;
} ATP_DNS_FILTER_MSG_ST;

/*Start of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/
typedef enum tagATP_DNS_MODULE_TYPE_EN
{
    ATP_DNS_MODULE_TYPE_ROUTE,
    ATP_DNS_MODULE_TYPE_FILTERIST,
    ATP_DNS_MODULE_TYPE_ABUSE,
} ATP_DNS_MODULE_TYPE_EN;

typedef struct tagATP_DNS_DOMAIN_ITEM_ST
{  
    ATP_DNS_MODULE_TYPE_EN          enModuleType;/*注册模块类型，不同类型对应不同的处理函数*/
    VOS_CHAR                        acDomain[DNS_NAME_LEN];/*注册的域名*/
    VOS_CHAR                        acReserve[DNS_WORD_LEN];/*保留字段，供各模块扩展传输其他信息*/
    /*Filterlist使用acReserve的前2个字节表示端口，
                               第3、4个字节表示是Telekomlist还是Userlist
                               第5、6、7、8表示是Telekomlist或Userlist下的第几个节点*/
    /*Route目前没有使用acReserve*/
} ATP_DNS_DOMAIN_ITEM_ST;

typedef struct tagATP_DNS_DOMAIN_FILTERLIST_ST
{
    VOS_UINT16 usPort;/*配置的端口*/
    VOS_UINT16 usType;/*是Telekomlist还是Userlist, 0表示Userlist，1表示Telekomlist*/
    VOS_UINT32 ulSeq;/*是Telekomlist或Userlist下的第几个节点*/
} ATP_DNS_DOMAIN_FILTERLIST_ST;
/*End of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/

enum ATP_DNS_MSGTYPES_EN
{
    ATP_MSG_DNS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DNS),//dns启动后，告知dnscms, 用于获取配置信息
    
    ATP_MSG_DNS_DISPLAY_CACHE,
    ATP_MSG_DNS_CLEAR_CACHE,
    
    ATP_MSG_DNS_APPEND_SERVER,
    ATP_MSG_DNS_DELETE_SERVER,
    ATP_MSG_DNS_FLUSH_SERVER,
    ATP_MSG_DNS_DISPLAY_SERVER,

    ATP_MSG_DNS_APPEND_RESIDENT,
    ATP_MSG_DNS_DELETE_RESIDENT,
    ATP_MSG_DNS_FLUSH_RESIDENT,
    ATP_MSG_DNS_DISPLAY_RESIDENT,

    ATP_MSG_DNS_APPEND_REDIRECT,
    ATP_MSG_DNS_DELETE_REDIRECT,
    ATP_MSG_DNS_FLUSH_REDIRECT,
    ATP_MSG_DNS_DISPLAY_REDIRECT,    

    ATP_MSG_DNS_TTL_EXPIRE_REGISTER,
    ATP_MSG_DNS_TTL_EXPIRE_UNREGISTER,
    ATP_MSG_DNS_TTL_EXPIRE_NOTIFY,

    ATP_MSG_DNS_SET_MINTTL,

    ATP_MSG_DNS_APPEND_FILTER,
    ATP_MSG_DNS_DELETE_FILTER,
    ATP_MSG_DNS_FLUSH_FILTER,
    ATP_MSG_DNS_DISPLAY_FILTER,
    
    ATP_MSG_DNS_PROBE_SUCCESS,

    ATP_MSG_DNS_GET_INFO,//W724v 工程页面获取dns信息，包括server和cache

    ATP_MSG_DNS_ONDEMAND_V4,//是否需要触发IPv4按需拨号
    ATP_MSG_DNS_ONDEMAND_V6,//是否需要触发IPv6按需拨号

    ATP_MSG_DNS_SET_TIMEOUT,//设置超时时间
    ATP_MSG_DNS_SET_DOMAIN,//dhcpc下发的dns后缀
    ATP_MSG_DNS_SET_CREDENTIAL,//设置账号是否为默认账号

    ATP_MSG_DNS_GET_DEBUG_INFO,
    ATP_MSG_DNS_SET_DYN_SERVER,

    ATP_MSG_DNS_GET_MEMORY_INFO,
    
    ATP_MSG_DNS_STATIC_SET_INFO,//hybrid 工程页面设置静态dns 信息
	/*Start of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/
    ATP_MSG_DNS_STATIC_DEL_INFO,
    ATP_MSG_DNS_ADD_DOMAIN,
    ATP_MSG_DNS_DEL_DOMAIN,
	/*End of Protocol 2014-5-24 for UDK0000351459 Filter list TTL修改  by y00250667*/
} ;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif // End of __ATP_DNS_MSGTYPES_H__

