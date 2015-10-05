/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file routecmsapi.h
  *Route模块对外接口
  *\copyright 2007-2020，华为技术有限公司
  *\author h00170023
  *\date  2012-2-6
*/

#include "atputil.h"
#include "atp_interface.h"
#include "atpconfig.h"
#include "cfmapi.h"
#include "cmo_def.h"
#ifndef SUPPORT_ATP_TR181
#include "wancmsapi.h"
#else
#include "wancorecmsapi.h"
#endif

#define ATP_WAN_IFC_LEN 32
#define ATP_WAN_PATH_LEN 128

//#define ATP_DEBUG     1

#ifdef ATP_DEBUG
#define ATP_ROUTE_DEBUG(format, args...)          printf("<DEBUG> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#define ATP_ROUTE_ERROR(format, args...)		  printf("<ERROR> --->FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#else
#define ATP_ROUTE_DEBUG(format, args...) 
#define ATP_ROUTE_ERROR(format, args...)		 
#endif

//!源地址绑定时的源地址长度
#define BIND_IP_LENGTH              64 
//!PVCNAME最大长度
#define BIND_PVCNAME_LENGTH         32 
//!使用IP策略路由表的最大数目
#define BIND_TABLE_IPRULE_MAX_NUM   32   
//!最大路由表号
#define BIND_TABLE_MAX_NUM          256   
//!绑定标识位。标识已经绑定。
#define BIND_TRUE                   1 
//!绑定标识位。标识未绑定。
#define BIND_FALSE                  0 
//!支持的最大LAN数目
#ifdef SUPPORT_ATP_WLAN
#define BIND_MAXLANNUM              8        
#else
#define BIND_MAXLANNUM              4        
#endif

#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE
#define ATP_ROUTE_MIN_MTU			1
#define ATP_ROUTE_MAX_MTU			1540
#endif

#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE		
#define STATIC_POLICY_ROUTE_TABLE_START      180
#define STATIC_POLICY_ROUTE_TABLE_MAX_NUM    20
#define STATIC_POLICY_ROUTE_TABLE_END		  (STATIC_POLICY_ROUTE_TABLE_START + STATIC_POLICY_ROUTE_TABLE_MAX_NUM)
//#define ATP_BIND_IPRULE_UNUSED 0
//#define ATP_BIND_IPRULE_USED   1
#endif
//!IPDEV使用的策略路由表最小表号 
#define BIND_IPRULEDEV_TABLE_START  100   
//!IPDEV使用的策略路由表最大表号 
#define BIND_IPRULEDEV_TABLE_END    120   

#define BIND_SPECIALDEV_TABLE_START 150

#define BIND_FWMARKRULEDEV_TABLE_START  200   

/*为DT的FON网段172.17.2.0/24 加策略路由优先级27000,只走dsl ppp256*/
#define BIND_FON_TABLE_ID           270
#define ROUTE_SRC_IP_RULE_PREF_FON  27000


/*start added by c00177661 for 高级静态策略路由功能*/
#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE_ADVANCED
#define STATIC_POLICY_ROUTE_ADVNACED_RULE_PREF_START  26000

/* protocol */
#define ROUTE_PROTO_BOTH                           "TCP/UDP"
#define ROUTE_PROTO_TCP                            "TCP"
#define ROUTE_PROTO_UDP                            "UDP"
#define ROUTE_PROTO_ICMP                           "ICMP"
#define ROUTE_PROTO_IGMP                           "IGMP"

#define ROUTE_INT_DEFAULT                          -1

#define IS_EMPTY_STRING(s)                         (*s == '\0')

/* 区分ebtables ftos 是dscp还是tos或者ipp */
#define ROUTE_DSCP_MARK                            0x1

#define ATP_ROUTE_MIN_DSCP                         0
#define ATP_ROUTE_MAX_DSCP                         63

#define ATP_ROUTE_MIN_PORT                         1
#define ATP_ROUTE_MAX_PORT                         65535
#endif
/*end added by c00177661 for 高级静态策略路由功能*/

/*start added by c00177661 for 策略路由mark分配*/
#define ROUTE_BRDGE_BIND_MARK                  "0x1%d000000"  //ipv4桥绑定mark
#define ROUTE_STATIC_ROUTE_MARK                "0x8%d000000"  //ipv4静态路由mark
/*end added by c00177661 for 策略路由mark分配*/

/*Start of ATP 2012-5-9 for DTS2012032102857 by : 手动配置的策略路由优先级高于自动添加的，影响功能*/
#define ROUTE_SRC_IP_RULE_PREF_START  28000
#define ROUTE_BR_BIND_RULE_PREF_START 30000
#define STATIC_POLICY_ROUTE_RULE_PREF_START  32000
/*End of ATP 2012-5-9 for DTS2012032102857 by */

struct BindIpRuleMap
{
    VOS_UINT32 ulTableId;                       /*!< 使用的策略路由表号 */
    VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];  /*!< 源IP地址绑定的PVC名字，关键字。 */
    VOS_UINT32 ulUsedFlag;                      /*!< 表示这个结构是有数据绑定过的 */
    VOS_CHAR   pcSrcIp[BIND_IP_LENGTH];         /*!< 源IP地址绑定的源地址。 */
    VOS_CHAR   pcGateway[BIND_IP_LENGTH];       /*!< 下一跳地址 */
    VOS_CHAR   pcRaPrefix[ATP_WAN_PREFIXLIST_LEN];/*!< 如果该wan地址是ra生成的且prefix OnLink */
};

struct BindRoutePortMap
{
	VOS_CHAR   *pcLanName;                          /*!< LAN口名字做索引，一个LAN口只能绑定到一个PVC */
	VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];      /*!< LAN口绑定的PVC名字，如果没有绑定则为空，pppoe_x_x_x */
	VOS_CHAR   pcGateway[BIND_IP_LENGTH];           /*!< 下一跳地址 */
	VOS_CHAR   pcRaPrefix[ATP_WAN_PREFIXLIST_LEN];  /*!< 如果该wan地址是ra生成的且prefix OnLink */
	VOS_UINT32 ulFwMark;                            /*!< 每个端口分配的FWMARK值 */
	VOS_UINT32 ulTableId;                           /*!< 每个LAN路由模式下绑定用的策略路由表号 */
	VOS_UINT32 ulBindFlag;                          /*!< 绑定标识。绑定了为BIND_TRUE；没有绑定则为BIND_FALSE */
};

struct BindDftPolicyRouteInfo
{
    VOS_BOOL   bUsed;                               /*!< 该桥绑定的默认策略路由是否添加 */
	VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];      /*!< LAN口绑定的默认策略路由PVC名字，如果没有绑定则为空，pppoe_x_x_x */
	VOS_CHAR   pcGateway[BIND_IP_LENGTH];           /*!< 默认策略路由下一跳地址 */
};

typedef enum tagATP_BIND_IPRULE_FLAG
{
    ATP_BIND_IPRULE_UNUSED = 0,
    ATP_BIND_IPRULE_USED = 1
} ATP_BIND_IPRULE_FLAG_EN;

/*!缺省路由Check消息处理
  \brief 
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

VOS_UINT32 ATP_ROUTE_DefaultCheckMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);

/*!缺省路由Set消息处理
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval ATP_E_ERR_CFM_NO_VISIT_ANY 不保存FLASH和共享内存
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_DefaultSetMsgProc(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);

/*!路由实例校验处理函数
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/

VOS_UINT32 ATP_ROUTE_CheckMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            const VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);

/*!路由实例添加修改消息处理函数
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval ATP_E_ERR_CFM_NO_VISIT_ANY 不保存FLASH和共享内存
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_AddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);

/*!路由实例删除消息处理函数
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_DelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 ulObjId);

/*!
  \brief 路由初始化函数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
/*start modify by x00202174 for DTS2012080401282 2012/8/13 */
VOS_UINT32 ATP_ROUTE_Init(VOS_VOID);
/*end modify by x00202174 for DTS2012080401282 2012/8/13 */
/*!
  \brief 启动某条WAN的路由配置
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: 备用的钩子参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_Setup(const VOS_VOID *pszInterPath, VOS_VOID *pvHook); 
/*!
  \brief 停止某条WAN的路由配置
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: 备用的钩子参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_Stop(const VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief 清除某条WAN的路由配置
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: 备用的钩子参数
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_VOID ATP_ROUTE_Clear(VOS_CHAR *pszInterPath);
/*!
  \brief LAN侧参数配置改变后Route重新生效
  \param[in] VOS_VOID: 
  \return 
*/
VOS_UINT32 ATP_ROUTE_LanChanged(const VOS_VOID *pvNotifier, VOS_VOID *pvHook);

/*RIP*/
#ifdef SUPPORT_ATP_RIP
/*!
  \brief 启用RIP动态路由功能
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_StartRip(VOS_VOID);
/*!
  \brief 停止RIP动态路由功能
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
VOS_UINT32 ATP_ROUTE_StopRip(VOS_VOID);
/*!
  \brief 
  \param[in]  *pszName: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
//VOS_UINT32 ATP_ROUTE_RipDelProc(VOS_CHAR *pszName);
//VOS_UINT32 ATP_ROUTE_RipSetProc(VOS_CHAR *pszName , VOS_CHAR *pszVersion, VOS_CHAR *pszOperation);
#endif

/*!
  \brief 删除策略路由并补充策略路由
  \param[in]  pszPath: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_ROUTE_DEL_DftPoliceRouteWithSupplement(VOS_CHAR* pszPath);

/*!
  \brief 添加LAN侧绑定接口默认策略路由
  \param[in]  pszWANInfPath: 添加或删除策略路由的WAN Path
  \param[in]  *pvHook: 钩子函数的传人参数，这里传人的是添加或删除标志
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_UINT32 ATP_ROUTE_ADD_DftPoliceRoute(VOS_VOID* pszWANInfPath, VOS_VOID *pvHook);
/*!
  \brief 添加WAN接口的默认策略路由
  \param[in]  pszWANInfPath: 添加或删除策略路由的WAN Path
  \param[in]  *pvHook: 钩子函数的传人参数，这里传人的是添加或删除标志
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_ROUTE_ADD_WanDftPoliceRoute(VOS_VOID* pszWANInfPath, VOS_VOID *pvHook);
/*!
  \brief 
  \param[in]  *pszInterPath: 路由启动的WAN PATH
  \param[in]  *pvHook: 钩子函数的传人参数，这里传人的是添加或删除标志
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/

VOS_UINT32 ATP_WAN_GetGateWay_DT(const VOS_CHAR* acIfName, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface );
VOS_UINT32 ATP_WAN_GetMask_DT(const VOS_CHAR* acIfName, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);

VOS_UINT32 ATP_ROUTE_BY_WANSTART(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief 
  \param[in]  *pszInterPath: 路由停止的WAN PATH
  \param[in]  *pvHook: 钩子函数的传人参数，这里传人的是添加或删除标志
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_ROUTE_BY_WANSTOP(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief WAN删除时候清除路由相关
  \param[in]  *pszInterPath: WAN删除的PATH
  \param[in]  *pvHook: 钩子函数的传人参数
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_ROUTE_BY_WANCLEAR(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief 当一个桥绑定多个WAN时，删除某条默认路由的WAN要补充其他的WAN作为新的默认路由
  \param[in]  pszWANInfPath: 要补充添加的新的默认路由WAN的路径
  \param[in]  ulBridgeKey: 所对应的桥Key
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_ROUTE_SupplementDftRouteInMRT(VOS_CHAR* pszWANInfPath, VOS_INT32 lBridgeKey);
/*!
  \brief 为隧道单独添加路由绑定
  \param[in]  pszInterface: WAN接口
  \param[in]  ulBrKey: 绑定的LAN侧接口
  \param[in]  bIsAdd: 添加或者删除
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_BIND_RouteDevBrBind(const VOS_CHAR* pszInterface, const VOS_CHAR *pszGateway, VOS_INT32 lBrKey, VOS_BOOL bIsAdd);
/*!
  \brief 根据连接接口名，从RIP信息表中删除此连接接口名的RIP信息
  \param[in]  *pszName: 接口名
  \return 
  \retval ATP_ROUTE_STS_FAIL RIP删除失败
  \retval ATP_ROUTE_STS_OK RIP删除成功
*/
VOS_UINT32 ATP_ROUTE_RipDelProc(VOS_CHAR *pszName);
/*!
  \brief 根据连接接口名，版本，RIP操作模式设置设置RIP信息
  \param[in]  *pszName: 连接接口名
  \param[in]  *pszVersion: RIP版本
  \param[in]  *pszOperation: RIP操作模式
  \return 
  \retval ATP_ROUTE_STS_FAIL RIP设置失败
  \retval VOS_OK RIP设置成功
*/
VOS_UINT32 ATP_ROUTE_RipSetProc(VOS_CHAR *pszName , VOS_CHAR *pszVersion, VOS_CHAR *pszOperation);
/*!
  \brief 给其他模块调用通过源IP的方式绑定策略路由
  \param[in]  *pcWanPath: WAN的路径
  \param[in]  *pcPvcName: WAN接口名
  \param[in]  *pcBridgeGw: 桥的
  \param[in]  *pcSrcIp: 
  \param[in]  *pcSubMask: 
  \return 
  \retval VOS_OK 设置成功
  \retval VOS_NOK 设置失败
*/

VOS_UINT32 ATP_BIND_PolicyRouteBySrcIp(VOS_CHAR *pcWanPath, VOS_CHAR *pcPvcName, VOS_CHAR *pcBridgeGw, VOS_CHAR *pcSrcIp,VOS_CHAR *pcSubMask);
/*!
  \brief 给其他模块调用通过源IP的方式解除绑定策略路由
  \param[in]  *pcWanPath: WAN的路径
  \param[in]  *pcPvcName: WAN接口名
  \param[in]  *pcBridgeGw: 桥的
  \param[in]  *pcSrcIp: 
  \param[in]  *pcSubMask: 
  \return 
  \retval VOS_OK 设置成功
  \retval VOS_NOK 设置失败
*/
VOS_UINT32 ATP_UNBIND_PolicyRouteBySrcIp(VOS_CHAR *pcWanPath, VOS_CHAR *pcPvcName,VOS_CHAR *pcBridgeGw);

/*!
  \brief 获取主路由表默认路由信息
  \param[in]  *pszGtwy: 要获取的默认路由网关
  \param[in]  *pszWanIf: 要获取的默认路由WAN 接口
  \return 
  \retval VOS_OK 设置成功
  \retval VOS_NOK 设置失败
*/
VOS_UINT32  ATP_ROUTE_GetDftGtwy(VOS_CHAR *pszGtwy, VOS_CHAR *pszWanIf);
#ifdef SUPPORT_ATP_RIP

/*!
  \brief 添加RIP信息到数据库
  \param[in]  *pszName: WAN PATH
  \param[in]  *pszIfSide: WAN/LAN side
  \return 
  \retval VOS_OK 设置成功
  \retval VOS_NOK 设置失败
*/
VOS_UINT32 ATP_ROUTE_AddRipInfoToDB(VOS_CHAR *pszName, VOS_CHAR *pszIfSide);

/*!
  \brief 删除RIP信息到数据库
  \param[in]  *pszName: WAN PATH
  \param[in]  *pszIfSide: WAN/LAN side
  \return 
  \retval VOS_OK 设置成功
  \retval VOS_NOK 设置失败
*/
VOS_UINT32 ATP_ROUTE_DelRipInfoToDB(VOS_CHAR *pszName, VOS_CHAR *pszIfSide);
/*!
  \brief 获取第一条缺省路由的接口名
  \param[in]  *pcIfcName: 接口名
  \return 
  \retval 获取成功返回VOS_TRUE，获取失败返回VOS_FALSE
*/
VOS_BOOL ATP_UTIL_GetDefaultRouteInterfaceName(VOS_CHAR *pcIfcName) ;

#endif
#ifdef SUPPORT_ATP_TR181
VOS_UINT32 ATP_ROUTE_Renew(VOS_VOID);
#endif

