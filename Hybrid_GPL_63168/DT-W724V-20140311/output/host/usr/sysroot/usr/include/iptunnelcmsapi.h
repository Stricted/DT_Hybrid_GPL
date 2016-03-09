/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file iptunnelapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author d00191326
  *\date  2011-12-23
*/


#ifndef __IPTUNNEL_API_H__
#define __IPTUNNEL_API_H__

#include "atputil.h"
#include "cfmapi.h"
#ifdef SUPPORT_ATP_TR181
#include "ipintfcmsapi.h"
#else
#include "wancmsapi.h"
#endif

#ifdef ATP_DEBUG
#define ATP_IPTUNNEL_DEBUG(format, args...) printf("TUNNEL: file[%s] line[%d] func[%s]: " format "\r\n", __FILE__, __LINE__,__FUNCTION__, ## args)
#define ATP_IPTUNNEL_CMD(acCmd)  {printf("TUNNEL: file[%s] line[%d] func[%s]: [%s] \r\n", __FILE__, __LINE__,__FUNCTION__, acCmd); ATP_Exec(acCmd);}
#else
#define ATP_IPTUNNEL_DEBUG(format, args...)
#define ATP_IPTUNNEL_CMD ATP_Exec
#endif

#define IPTUNNEL_CONF_MAX_NUM 4

#define IPTUNNEL_NAME_MAX_LEN 32
#define IPTUNNEL_WAN_INTERFACE_LEN 256
#define IPTUNNEL_LAN_INTERFACE_LEN 1024
#define IPTUNNEL_PATH_LEN 256
#define IPTUNNEL_CONN_STATUS_LEN 32
#define IPTUNNEL_MODE_LEN 8
#define IPTUNNEL_MECHANISM_LEN 8
#define IPTUNNEL_IPV6_ADDR_LEN 40
#define IPTUNNEL_IPV4_ADDR_LEN 20
#define IPTUNNEL_WORD_LEN 64
#define IPTUNNEL_CMD_LEN 256
#define IPTUNNEL_LINE_LEN 256
#define IPTUNNEL_ENDPOINT_LEN 260

#define IPTUNNEL_6RD_PREFIX_LEN_MIN 16
#define IPTUNNEL_6RD_PREFIX_LEN_MAX 128
#define IPTUNNEL_IPV6_DNS_LEN 132

#define TUNNEL_6RD_WAN_MIN_MTU 1300
#define TUNNEL_DSLITE_WAN_MIN_MTU 1320
#define TUNNEL_GRE_WAN_MIN_MTU 1304
#define DEFAULT_MTU 1500

#define IPTUNNEL_6IN4_STR "6in4"
#define IPTUNNEL_4IN6_STR "4in6"

#define IPTUNNEL_6RD6TO4_STR "6rd6to4"
#define IPTUNNEL_6RD_STR "6rd"
#define IPTUNNEL_6TO4_STR "6to4"
#define IPTUNNEL_DSLITE_STR "DSLite"
#define IPTUNNEL_GRE_STR "GRE"

#define IPTUNNEL_STATUS_CONNECTED "Connected"
#define IPTUNNEL_STATUS_CONNECTING "Connecting"
#define IPTUNNEL_STATUS_DISCONNECTED "Disconnected"

#define TUNNEL_MECHANISM_MAX_NUM 4
#define TUNNEL_MECHANISM_6RD 0
#define TUNNEL_MECHANISM_DSLITE 1

//!\brief IPTUNNEL配置参数的数据结构
/*!
 *
 */
typedef struct tagATP_IPTUNNEL_CONF_ST
{
  /*下一节点的指针*/
  struct tagATP_IPTUNNEL_CONF_ST* pstNext;
  /*隧道实例的ID*/
  VOS_UINT32 ulTunnelModeInstId;
  /*CMO*/
  VOS_UINT32 ulObjID;
  /*隧道路径*/
  VOS_CHAR acTunnelPath[IPTUNNEL_PATH_LEN];
  /*隧道是否激活*/
  VOS_UINT32 ulActivated;
  /*隧道接口名*/
  VOS_CHAR acTunnelName[IPTUNNEL_NAME_MAX_LEN];
  /*隧道别名*/
  VOS_CHAR acAliasName[ATP_ACTUAL_LEN_128];
  /*关联的WAN接口*/
  VOS_CHAR acAssociatedWanIfName[IPTUNNEL_WAN_INTERFACE_LEN];
  /*连接状态*/
  VOS_CHAR acConnStatus[IPTUNNEL_CONN_STATUS_LEN];
  /*隧道类型*/
  VOS_CHAR acMechanism[IPTUNNEL_MECHANISM_LEN];
  /*是否自动配置*/
  VOS_UINT32 ulDynamic;
  /*6rd的IPv4掩码长度*/
  VOS_UINT32 ulIPv4MaskLen;
  /*6rd前缀*/
  VOS_CHAR ac6rdPrefix[IPTUNNEL_IPV6_ADDR_LEN];
  /*6rd前缀长度*/
  VOS_UINT32 ul6rdPrefixLen;
  /*隧道的端点，可以为地址或域名*/
  VOS_CHAR acTunnelEndpoint[IPTUNNEL_ENDPOINT_LEN];
  /*IPv4的对端地址*/
  VOS_CHAR acIPv4Endpoint[IPTUNNEL_IPV4_ADDR_LEN];
  /*DSLite的对端IPv6地址*/
  VOS_CHAR acRemoteIpv6Address[IPTUNNEL_IPV6_ADDR_LEN];
  /*IPV6 DNS服务器地址*/
  VOS_CHAR acIpv6DnsServers[IPTUNNEL_IPV6_DNS_LEN];
  /*隧道接口的IPv6地址*/
  VOS_CHAR acIpv6IfAddr[IPTUNNEL_IPV6_ADDR_LEN];
  VOS_UINT32 ulIPv4Enable;
  VOS_UINT32 ulIPv6Enable;
  /*是否指定了WAN路径*/
  VOS_BOOL  bConfiguredWan;
} ATP_IPTUNNEL_CONF_ST;

typedef enum
{
    ATP_6IN4_UP_NOTIFY_PRIORITY_DNSPROC=0,
    ATP_6IN4_UP_NOTIFY_PRIORITY_SNTPPROC,
}ATP_6IN4_UP_NOTIFY_PRIORITY;

typedef enum
{
    ATP_6IN4_DOWN_NOTIFY_PRIORITY_DNSPROC=0,
    ATP_6IN4_DOWN_NOTIFY_PRIORITY_SNTPPROC,
}ATP_6IN4_DOWN_NOTIFY_PRIORITY;


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*!
  \brief 隧道模块的初始化函数
  \return 初始化是否成功
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_Init();

/*!
  \brief 隧道模块的去初始化函数
  \return 去初始化是否成功
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_UnInit();


/*!
  \brief 启动隧道服务，由WAN UP时回调
  \param[in]  acWanPath: WAN的路径
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_Start(VOS_VOID* pszWanPath, VOS_VOID* pvHook);

/*!
  \brief 停止隧道，由WAN DOWN时回调
  \param[in]  acWanPath: WAN的路径
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_Stop(VOS_VOID* pszWanPath, VOS_VOID* pvHook);


/*!
  \brief 根据WAN PATH获得隧道信息
  \param[in]  pcPath: WAN路径
  \return 如果该WAN存在隧道返回隧道指针，不存在返回VOS_NULL
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceByWanPath(const VOS_CHAR* pcPath);

/*!
  \brief wan变化时删除相关的隧道配置
  \param[in]  pszWanPath: WAN路径
  \param[in]  pvHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_ConfClear(VOS_VOID* pszWanPath, VOS_VOID* pvHook);

/*!
  \brief Tunnel up 时的注册点函数
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_Up_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief Tunnel down时的注册点函数
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_Down_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief 将隧道从桥上移除时的注册点
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_BrDelProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief 将隧道绑定在某桥上时的注册点
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return VOS_UINT32
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_IPTunnel_BrAddProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);


/*!
  \brief 根据路径获取隧道实例
  \param[in]  pcPath: 隧道路径
  \return ATP_IPTUNNEL_CONF_ST*
  \retval VOS_NULL表示不存在
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceByPath(const VOS_CHAR* pcPath);

/*!
  \brief 通过路径获取隧道别名
  \param[in]  pcPath: 隧道路径
  \param[in]  pszAlias: 隧道别名
  \param[in]  ulAliasLen: 别名长度
  \return VOS_UINT32
  \retval VOS_OK获取成功
*/
VOS_UINT32 ATP_IPTunnel_GetTunnelAlias(const VOS_CHAR* pcPath, VOS_CHAR* pszAlias, VOS_UINT32 ulAliasLen);

VOS_BOOL ATP_IPTunnel_IsExistActivated6in4Tunnel(const VOS_CHAR* pcPath);

VOS_BOOL ATP_IPTunnel_IsExistActivatedTunnel(VOS_VOID);

/*!
  \brief 获取链表头部
  \return ATP_IPTUNNEL_CONF_ST
  \retval VOS_NULL链表为空
*/
ATP_IPTUNNEL_CONF_ST* ATP_IPTunnel_GetInstanceHeader(VOS_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
