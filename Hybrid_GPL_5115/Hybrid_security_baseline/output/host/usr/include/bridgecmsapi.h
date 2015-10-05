/**\file bridgecmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author 金鹏(j00196195)
  *\date  2011-12-19
*/

#ifndef __BRIDGE_CMS_API_H__
#define __BRIDGE_CMS_API_H__

#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"
/********************** Include Files ***************************************/
#define ATP_CBBID_BRIDGECMS_NAME "l2b"


/********************** macro  Definitions **********************************/
#define ATP_BRIDGE_DEFAULTNAME		            "br0"

/* 16个字符接口名称 * 16个接口 + 16个分界符 + '\0'*/
#define ATP_BRIDGE_INTEFACELIST_LEN             (16*16+16+1)

#define ATP_BRIDGE_IGMPSNOOPING_ENABLE_STR      "enable"
#define ATP_BRIDGE_IGMPSNOOPING_DISABLE_STR     "disable"

#define ATP_BRIDGE_IGMPSNOOPING_ENABLE_FLAG     1
#define ATP_BRIDGE_IGMPSNOOPING_DISABLE_FLAG    0

#define ATP_BRIDGE_BRGROUP_ENABLE_STR           "enable"
#define ATP_BRIDGE_BRGROUP_DISABLE_STR          "disable"

#define ATP_BRIDGE_BRGROUP_ENABLE_FLAG          1
#define ATP_BRIDGE_BRGROUP_DISABLE_FLAG         0

#define ATP_BRIDGE_BRGROUPMACLN_ENABLE_STR      "enable"
#define ATP_BRIDGE_BRGROUPMACLN_DISABLE_STR     "disable"

#define ATP_BRIDGE_BRGROUPMACLN_ENABLE_FLAG     1
#define ATP_BRIDGE_BRGROUPMACLN_DISABLE_FLAG    0

#define ATP_BRIDGE_GROUPID_MAX                  15
#define ATP_BRIDGE_GROUPID_MIN                  0

#define BRIDGE_BIND_DEFAULT_GROUPID             0 //绑定缺省组号

#define ATP_BRIDGE_AIF_INF_TYPE_WANROUTER "WANRouterConnection"
#define ATP_BRIDGE_AIF_INF_TYPE_WANINTERFACE "WANInterface"
#define ATP_BRIDGE_AIF_INF_TYPE_LANROUTER "LANRouterConnection"//暂时没考虑
#define ATP_BRIDGE_AIF_INF_TYPE_LANINTERFACE "LANInterface"

typedef enum
{
    ATP_BRIDGE_NOTIFY_DEL_FILTER_WAN_MOVE=1,
}ATP_BRIDGE_NOTIFY_DEL_FILTER;

typedef enum
{
    ATP_BRIDGE_NOTIFY_ADD_FILTER_WAN_MOVE=1,
}ATP_BRIDGE_NOTIFY_ADD_FILTER;

typedef enum
{
    ATP_BRIDGE_NOTIFY_BEFORE_DEL_BRIDGE_ROUTE6=1,
}ATP_BRIDGE_NOTIFY_BEFORE_DEL_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_DEL_BRIDGE_RIP=1,
    ATP_BRIDGE_NOTIFY_DEL_BRIDGE_RADVD,
}ATP_BRIDGE_NOTIFY_DEL_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_RIP=1,
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_RADVD,
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_ROUTE6,
}ATP_BRIDGE_NOTIFY_ADD_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_INIT_BRIDGE_ROUTE6=1,
}ATP_BRIDGE_NOTIFY_INIT_BRIDGE;

/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_BridgeDelRoutedWANFilter_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
                                            						
VOS_UINT32 ATP_BridgeAddRoutedWANFilter_NotifyRegister(AtpNotifierProc                 pfProc,
                                                                                    VOS_UINT32                      ulPriority,
                                                                                    VOS_VOID                        *pvNotifyHook);

/*!
  \brief 通过桥的key值，获取对应的landevice的ip地址和子网掩码
  \param[in]  ulByBridgeKey: 输入桥的key值
  \param[out]  *pszIpAddr: 返回ip地址
  \param[in]  ulAddrLen: 输入参数的长度限制
  \param[out]  *pszIpMask: 返回子网掩码
  \param[in]  ulMaskLen: 输入参数掩码的长度限制
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_GetIPByBridgeKey (const VOS_INT32 lByBridgeKey,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,
												VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen);

/*!
  \brief 通过接口的domain获取ip地址和子网掩码，并且返回桥的key
  \param[in]  *pszInfPath: lan接口在xml树中全路径，俗语domain
  \param[out]  *pszIpAddr: 返回ip地址
  \param[in]  ulAddrLen: 输入参数的长度限制
  \param[out]  *pszIpMask: 返回子网掩码
  \param[in]  ulMaskLen: 输入参数掩码的长度限制
  \param[out]  *pulRetBridgeKey: 返回所对应桥的bridgekey
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_GetIPAddrMaskByInterfacePath(const VOS_CHAR *pszInfPath,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,
													VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen, VOS_INT32 *plRetBridgeKey);

/*!
  \brief 根据lan实例号，获得与之对应桥的key值
  \param[in]  ulLanInst: LANDevice实例号
  \param[in]  *ulOutBridgeKey: 返回对应的桥的bridgekey
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByLANInstId(const VOS_UINT32 ulLanInst, VOS_INT32 *lOutBridgeKey );
/*!
  \brief 设置桥ip地址
  \param[in]  ulBridgeKey: 桥的key值
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_SetupIP( VOS_INT32 lBridgeKey) ;
/*!
  \brief 根据LANDevice实例号，设置与之对应的桥的ip地址
  \param[in]  ulLDVInst: LANDevice实例号
  \param[in]  VOS_CHAR*pacLanAddr: ip地址
  \param[in] VOS_CHAR*pacLanMask: 子网掩码
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_SetupIP_ByInstId( VOS_UINT32 ulLDVInst, VOS_CHAR*pacLanAddr ,VOS_CHAR*pacLanMask ) ;

/*!
  \brief:
            Clear the bridge IP-Mask when disable the bridge
  \param[in]  ulLDVInst:  LANDevice instance number
  \return 
  \retval  VOS_OK: clear success
             VOS_NOK: clear failure
*/
VOS_UINT32 ATP_BRIDGE_Down_ByInstId(VOS_UINT32 ulLDVInst);
/*!
  \brief:
            与函数ATP_BRIDGE_Update_WAN_LAN_Interface的区别就是不会操作具体接口。
不使用brctl添加和删除接口
只操作数据库
  \param[in]  ulLDVInst:  LANDevice instance number
  \return 
  \retval  VOS_OK: clear success
             VOS_NOK: clear failure
*/

VOS_UINT32 ATP_BRIDGE_Update_WAN_LAN_Interface_without_Activate(VOS_BOOL isAdd, VOS_BOOL isWAN, VOS_CHAR* pszPath);//add or delete


/*!
  \brief 更新wan和lan接口信息到layer2bridging节点中。主要更新InternetGatewayDevice.Layer2-Bridging.AvailableInterface.{i}. 
  如果不存在，则添加，如果存在则忽略。
  同时还会更新filter表，建立默认配对关系。
  在wan和lan建立或删除接口的时候，会调用这个函数。
  \param[in]  isAdd: 添加还是删除
  \param[in]  isWAN: 标示是wan还是lan
  \param[in]  pszPath: 如果是lan，这个path则是接口名，如eth0.2.如果是wan，则是wan接口的domain
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_Update_WAN_LAN_Interface(VOS_BOOL isAdd, VOS_BOOL isWAN, VOS_CHAR* pszPath);
/*!
  \brief 初始化bridge。其中负责校验landevice配置中是否存在接口name节点值，如果没有则添加，如eth0.2.
  另外，主要负责创建桥。
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/

VOS_UINT32 ATP_BRIDGE_Init (VOS_VOID);
/*!
  \brief 去初始化bridge。删除桥
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_NOK失败
  \retval VOS_OK 成功
*/


#ifdef SUPPORT_ATP_LAN_VLAN
VOS_UINT32 ATP_BRIDGE_VlanInit(VOS_VOID);
VOS_VOID ATP_BRIDGE_VlanMonitor(const VOS_CHAR *pszPort);
#endif
VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfaceName(const VOS_CHAR *pszInfName, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);
VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByInterfaceName(const VOS_CHAR *pszInfName,VOS_INT32* plRetBridgeKey);

VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfacePath(const VOS_CHAR *pszInfPath, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);
//interface Path-->AIF .interfaceKey -->Filter.BridgeKeyRef
VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByInterfacePath(const VOS_CHAR *pszInfPath,VOS_INT32* plRetBridgeKey);
/*!
  \brief 
  \param[in]  pszWanPath: 
  \param[in]  *lOutBridge: 
  \return 
*/

VOS_VOID ATP_BRIDGE_GetBridgeKeyByWanPath(const VOS_CHAR* pszWanPath, VOS_INT32 *lOutBridge);

VOS_UINT32 ATP_BRIDGE_GetAvailableWanPathByBridgeKey(const VOS_INT32 lBridgeKey, const VOS_CHAR* pszExcludeWanPath, VOS_CHAR* pszWanPath);

/*!
  \brief 开关IGMP Snooping功能
  \param[in]  bEnable: IGMP Snooping开关选项。"enable"-->使能；"disable"-->禁用
  \return 
  \retval 开关IGMP Snooping功能成功则返回VOS_OK， 
              开关IGMP Snooping功能失败则返回错误码ATP_ERR_BRIDGE_ENABLEIGMPSNOOPING_EN
*/

VOS_UINT32 ATP_BRIDGE_EnableIgmpSnooping(VOS_BOOL bEnable);

VOS_UINT32 ATP_BridgeBeforeDelBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeDelBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_BridgeAddBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeInitBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeBindChg_NotifyRegister(AtpNotifierProc                 pfProc,
                                                     VOS_UINT32                      ulPriority,
                                                     VOS_VOID                        *pvNotifyHook);

VOS_VOID BridgeBindChgDoNotify(VOS_VOID* pulInterfaceRef);

VOS_UINT32 ATP_BRIDGE_GetBridgeInstIdByBridgeKey(VOS_INT32 lInBridgeKey, VOS_UINT32 *ulOutBridgeInst );

#if defined(__cplusplus)
}
#endif



#endif
