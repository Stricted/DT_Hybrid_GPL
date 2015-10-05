
#ifndef __VLANCMS_API_H__
#define __VLANCMS_API_H__
#include "bcmtypes.h"

#include "bcm_vlan.h"

#define BCM_VLAN_IF_SUFFIX ""


#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief 删除创建的wan接口
  \param[in]  *L3DevName: wan接口名
  \return 
  \retval VOS_OK 成功VOS_NOK失败
*/
 
 VOS_UINT32 ATP_VLAN_VLANMux_DeleteWANInf( 
                                 VOS_CHAR *L3DevName
                                 ) ;
/*!
  \brief 创建wan接口
  \param[in]  *L2DevName:二层接口名 ，如nas0
  \param[in]  *L3DevName: 三层接口名，如nas0_v.707.1...接口名命名规则：带vlan时：nas0_v.{vlan id}.{wanInstId} 不带vlan时，nas0_v_{二层接口index}.{wanInstId}
  \param[in]  isRouted: 是否为路由wan还是桥接wan
  \param[in]  isMulticast: 支持广播
  \param[in]  mode: BCM_VLAN_MODE_RG或BCM_VLAN_MODE_ONT
  \return 
  \retval VOS_OK 成功VOS_NOK失败
*/

 VOS_UINT32 ATP_VLAN_VLANMux_CreateWANInf( 
                                 VOS_CHAR *L2DevName, 
                                 VOS_CHAR *L3DevName, 
                                 VOS_BOOL isRouted, 
                                 VOS_BOOL isMulticast,
                                 bcmVlan_realDevMode_t mode
                                 ) ;

/*!
  \brief 设置带vlan tag的接口接受规则
  \param[in]  *L3DevName: 三层接口名
  \param[in]  *L2DevName: 二层接口名
  \param[in]  isRouted: 是否为路由wan还是桥接wan
  \param[in]  vbits: vlan id
  \param[in]  pbits: 优先级设置
  \return 
  \retval VOS_OK 成功VOS_NOK失败
*/

VOS_UINT32 ATP_VLAN_Tagged_SetTxRules( 
                                VOS_CHAR *L3DevName, 
                                VOS_CHAR *L2DevName, 
                                VOS_BOOL isRouted,
                                VOS_UINT32 vbits,
                                VOS_UINT32 pbits
                                ) ;


/*!
  \brief 设置不带vlan tag的接口接受规则
  \param[in]  *L3DevName: 三层接口名
  \param[in]  *L2DevName: 二层接口名
  \param[in]  isRouted: 是否为路由wan还是桥接wan
  \return 
  \retval VOS_OK 成功VOS_NOK失败
*/

VOS_UINT32 ATP_VLAN_Untagged_SetTxRules( 
                                VOS_CHAR *L3DevName, 
                                VOS_CHAR *L2DevName, 
                                VOS_BOOL isRouted
                                ) ;

#if defined(__cplusplus)
}
#endif

#endif





