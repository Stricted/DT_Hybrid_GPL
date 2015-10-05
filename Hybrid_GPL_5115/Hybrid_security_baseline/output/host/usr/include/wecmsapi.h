/**\file wecmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2011-12-20
*/

#ifndef __WE_CMS_API_H__
#define __WE_CMS_API_H__

#include "cmsapi.h"
#include "cmo_def.h"

//!记录WAN ETHERNET状态
#define   ATP_WANETH_STATUS_FILE_PATH      "/var/wanethstatus"
//!链路状态
enum
{
    ATP_WANETHLINKSTATE_DOWN = 0,
    ATP_WANETHLINKSTATE_UP
};

#if defined(__cplusplus)
extern "C" {
#endif

VOS_INT32 ATP_WE_Status(VOS_VOID);

/*!
  \brief QOS检视WANETH链路状态变化
  \param[in]  pfProc: 函数名
  \param[in]  ulPriority: 优先级
  \param[in]  *pvNotifyHook: 一般为NULL
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_QosStatusChangeProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief QosLink清理函数
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_QosLinkClearProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN ETHERNET 初始化API
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_Init (VOS_VOID);

/*!
  \brief WAN ETHERNET 去初始化API
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_UnInit (VOS_VOID);

/*!
  \brief WAN ETHERNET状态监视器并进行ppp拨号
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_VOID ATP_WE_Monitor(const ATP_MSG_HEADER_ST *pstMsg);

/*!
  \brief 添加WAN连接设备处理函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_WanConnDevAddMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues, VOS_UINT32 *pulSetValues, 
    const VOS_UINT32 *pulChgParas, VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId,
    VOS_BOOL bSetObj);

/*!
  \brief 删除WAN连接设备处理函数
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_WanConnDevDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId);

/*!
  \brief 根据实例号获取该实例号WANETH名字
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pszName: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_GetName(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, VOS_CHAR* pszName, VOS_UINT32 ulLen);

/*!
  \brief 获取WANETH物理状态
  \param[in]  ulWdInstId: 
  \param[in]  pszStatus: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_GetPhysicalStatus(VOS_UINT32 ulWdInstId, VOS_CHAR* pszStatus, VOS_UINT32 ulLen);

/*!
  \brief 获取链路状态
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pulStatus: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_GetLinkStatus(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId,  VOS_UINT32* pulStatus);

/*!
  \brief 获取WAN ETH状态
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pszIfc: 
  \param[in]  ulIfcLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_GetInterface(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId,  VOS_UINT32 ulWanInstId,
    VOS_CHAR* pszIfc, VOS_UINT32 ulIfcLen);

/*!
  \brief 获取WAN ETH连接数以及最大连接数
  \param[in]  ulWdInstId: 
  \param[in]  pulMaxNum: 
  \param[in]  pulActionNum: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WE_GetConnNum(VOS_UINT32 ulWdInstId, VOS_UINT32* pulMaxNum, VOS_UINT32* pulActionNum);

#if 0
/*!
  \brief 提供给关注消息ATP_MSG_MONITOR_EVT_LAN_ETH的进程模块注册用函数
  \param[in]  ulMsgType: 
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  **ppstMsgHdl: 
  \return 
  \retval
*/
VOS_UINT32 ATP_WE_WanEthStatusChangeProc_NotifyRegister(VOS_UINT32 ulMsgType, AtpMsgProc pfProc, VOS_UINT32 ulPriority, ATP_CMS_MSG_HDL_LIST_ST **ppstMsgHdl);
#endif

#if defined(__cplusplus)
}
#endif 

#endif
