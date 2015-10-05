/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file natcmsapi.h
  *NAT 模块处理函数
  *\copyright 2007-2020，华为技术有限公司
  *\author h00170023
  *\date  2012-2-6
*/


#ifndef __ATP_NAT_API_H__
#define __ATP_NAT_API_H__

#include "atptypes.h"
#include "atputil.h"
//#include "atperrdef.h"
#include "msgapi.h"
#include "cfmapi.h"

//#define ATP_DEBUG 1

#ifdef ATP_DEBUG
#define ATP_CMD(x)  {printf("%s \r\n", (x));\
    ATP_Exec(x);}
#define NATCMS_DEBUG(format, args...)          printf("<DEBUG> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#define NATCMS_ERROR(format, args...)          printf("<ERROR> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#else
#define ATP_CMD(x)  ATP_Exec(x)
#define NATCMS_DEBUG(format, args...)
#define NATCMS_ERROR(format, args...)          printf("<ERROR> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#endif


#define ATP_FIREWALL_CONNTRACK_NUM    1000
#define ATP_NAT_CMD_LEN       256

#define ATP_SYS_PORT_MAX                    65535
#define ATP_SYS_PORT_MIN                    1

#define KRNL_STR_2_6_8        "2.6.8.1"
#define KRNL_STR_2_6_20       "2.6.20"
#define KRNL_STR_2_6_21_5     "2.6.21.5"
#define KRNL_STR_2_6_30       "2.6.30"


#define KRNL_CT_PATH_2_6_20       "/lib/kernel/net/netfilter"
#define KRNL_NAT_PATH_2_6_20      "/lib/kernel/net/ipv4/netfilter"
#define KRNL_CT_NAT_PATH_2_6_8    "/lib/kernel/net/ipv4/netfilter"

#define CT_CLEAN_PATH_2_6_8       "/proc/sys/net/ipv4/netfilter/ip_conntrack_dns"
#define CT_CLEAN_PATH_2_6_20      "/proc/sys/net/ipv4/netfilter/ip_ct_clean"

//DT要求开了port forwarding则AlwaysOn时的V4不能掉线，建空文件做个标志
//规则全部删除时，删除该文件
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#define V4_PORT_FORWARDING_FILE    "/var/port_forwarding"
#endif

typedef enum
{
    KRNL_2_6_8 = 0,
    KRNL_2_6_20 = 1,
    KRNL_2_6_21_5 = 2,
    KRNL_2_6_30 = 3,
    KRNL_MAX = 9,   
} ATP_KRNL_VERSION;

extern ATP_KRNL_VERSION g_enKrnlVersion;
extern VOS_CHAR g_acKrnlCtPath[ATP_SYS_MEDIUM_LEN];
extern VOS_CHAR g_acKrnlNatPath[ATP_SYS_MEDIUM_LEN];
extern VOS_CHAR  g_acCtCleanPath[ATP_SYS_MEDIUM_LEN];

/*!
  \brief Nat模块初始化函数
  \param[in]  *pcWanPath: 备用的WAN Path
  \param[in]  *pvHook: 备用的挂钩参数
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_Init(VOS_VOID *pcWanPath, VOS_VOID *pvHook);

/*!
  \brief NAT其他特性初始化函数DMZ,PORTMAP,PORTTRIGER
  \param[in]  *pcWanPath: 
  \param[in]  *pvHook: 
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_Other_Init(VOS_VOID *pcWanPath, VOS_VOID *pvHook);

/*!
  \brief 启用WAN的NAT功能
  \param[in]  pszPath: 需要启动NAT功能的WAN的路径
  \param[in]  *pvHook: 钩挂的备用参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_WanStart(VOS_VOID* pszPath, VOS_VOID *pvHook);

/*!
  \brief 停止WAN的NAT功能
  \param[in]  pszPath: 需要停止NAT功能的WAN的路径
  \param[in]  *pvHook: 钩挂的备用参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_WanStop(VOS_VOID* pszPath, VOS_VOID *pvHook);

/*!
  \brief 清除内存中保存的nat链表
  \param[in]  pszPath: 需要删除NAT功能的WAN的路径
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_WanClear(VOS_VOID* pszPath,VOS_VOID *pvHook);

/*START ADD: by h00170023 at 20110107 for ct clean*/
/*!
  \brief 清除连接跟踪
  \param[in]  pszPath: 需要启动NAT功能的WAN的路径
  \param[in]  *pvHook: 钩挂的备用参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_CTclean(VOS_VOID* pszPath, VOS_VOID *pvHook);
/*END ADD: by h00170023 at 20110107 for ct clean*/

/*!
  \brief PortMapping 消息处理函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_PortMapMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);

/*!
  \brief DMZ消息分发处理函数
  \param[in]  *pstMsg: 
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_DmzMsgProc(VOS_VOID *pstMsg);

/*!
  \brief 重定向telnet端口处理函数
  \param[in]  usChangePort: 
  \param[in]  usChangedPort: 
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_ChgTelnetPort(VOS_UINT16 usChangePort, VOS_UINT16 usChangedPort);

/*!
  \brief 重定向ftp端口
  \param[in]  usChangePort: 
  \param[in]  usChangedPort: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_NAT_ChgFtpPort(VOS_UINT16 usChangePort, VOS_UINT16 usChangedPort);

/*!
  \brief LAN侧br+接口信息改变后需要同步修改mac过滤中必须通过的规则，nat中masqurede规则，url过滤中缺省域名访问，ftp中的生效规则
  \param[in]  pszNewLanIp: 更改后的网关ip
  \param[in]  pszNewSubnetMask: 更改后的网关子网掩码
  \param[in]  pszOldLanIp: 更改前的网关ip 
  \param[in]  pszOldSunnetMask: 更改前的网关子网掩码
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_LanChange(VOS_CHAR* pszNewLanIp, VOS_CHAR* pszNewSubnetMask, 
                                   VOS_CHAR* pszOldLanIp, VOS_CHAR* pszOldSunnetMask);
/*!
  \brief 获取PortMa的结点个数
  \param[in]  pszPath: 相关WAN的路径
  \param[in]  pulNum: 要获取的节点个数
  \return 
  \retval VOS_OK 执行成功
*/
VOS_UINT32 ATP_NAT_PMGetNodeNums(VOS_CHAR* pszPath, VOS_UINT32* pulNum);

/*!
  \brief 打开Nat下的IGMP Proxy服务
  \param[in]  bEnable: IGMP Proxy使能
  \param[in]  pszInterface: 要设置的WAN接口
  \return 
*/
VOS_VOID ATP_NAT_ServiceIgmp(VOS_BOOL bEnable, VOS_CHAR* pszInterface);

/*!
  \brief Nat加载内核模块
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_LoadModule(VOS_VOID);

/*!
  \brief PortMapping 校验
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_PortMapCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);

/*!
  \brief PortMapping 删除一个实例
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_PortMapDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
/*!
  \brief DMZ添加修改一个实例
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_DmzAddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                         VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                         VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjIdj);

/*!
  \brief DMZ 校验
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_DMZCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);

#ifdef SUPPORT_ATP_MULTINAT
/*!
  \brief 多NAT模块校验函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_MNatCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
/*!
  \brief 多NAT模块添加修改消息处理函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_MNatAddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                                VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                                VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
/*!
  \brief 多NAT模块删除消息处理函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_NAT_MNatDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
#endif

/* start of by d00107688 for UPnP PortMapping  2009-06-11 */
#ifdef SUPPORT_ATP_UPNP

/*!
  \brief UPNP PortMapping消息处理函数
  \param[in]  pstMsg: 接受到的消息
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_VOID  ATP_NAT_PORTMAPPING_UPNP_MSG_Process(const ATP_MSG_HEADER_ST* pstMsg);

#endif
/* end of by d00107688 for UPnP PortMapping  2009-06-11 */

//w724v 使用tr181, nat只有enable,disable两个值，dt只要求CONE_NAT
#ifdef SUPPORT_ATP_TR181
VOS_UINT8 ATP_NAT_GetEnableByPath(const VOS_CHAR *pszPath);
#endif

#endif

