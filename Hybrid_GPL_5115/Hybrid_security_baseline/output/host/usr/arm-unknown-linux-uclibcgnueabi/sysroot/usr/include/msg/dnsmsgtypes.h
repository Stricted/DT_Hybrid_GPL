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
#define DNS_NAME_LEN        (256)//最大支持256个可见字符
#define DNS_WORD_LEN        (64)//从文件中每行文本中读取的单词长度   

#define ATP_DNS_FAKE_ADDR   "10.10.10.10"
#ifdef SUPPORT_ATP_IPV6
#define ATP_DNS_FAKE_ADDR6  "2001::10:10:10:10"
#endif
/* <DTS2012082500307 w00211169 2012-9-7 begin */
#define WIRELESS_WAN_FAKE_ADDR        "10.89.89.89"
/* <DTS2012092901051  w00211169 2012-10-15 begin */
#define WIRELESS_WAN_FAKE_IPV6_ADDR   "fc00::08:90:08:90"
/* DTS2012092901051  w00211169 2012-10-15 end> */
#define WIRELESS_WAN_INTERFACE        "rmnet0"
/* DTS2012082500307 w00211169 2012-9-7 end> */


typedef struct tagATP_DNS_SERVER_MSG_ST
{
    VOS_CHAR acDnsServers[DNS_BIND_LEN];//可能有两个地址，中间用逗号分隔
    VOS_CHAR acWanAddr[DNS_ADDR_LEN];
    VOS_CHAR acInterface[DNS_IFC_LEN];
    VOS_CHAR acServiceType[DNS_BIND_LEN];   
    VOS_CHAR acBindBr[DNS_BR_LEN];
} ATP_DNS_SERVER_MSG_ST;

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
} ;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif // End of __ATP_DNS_MSGTYPES_H__

