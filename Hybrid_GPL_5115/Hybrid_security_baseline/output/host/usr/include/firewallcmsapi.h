/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file firewallcmsapi.h
  *firewallcms api \n
  *防火墙模块对外接口\n
  *\copyright 2007-2020，华为技术有限公司
  *\author h00163136
  *\date  2012-2-4
*/


#ifndef __FIREWALL_API_H__
#define __FIREWALL_API_H__

#include "atputil.h"

//!\brief 防火墙规则操作类型，增加，插入，删除
/*!
 *ATP_FW_COMMAND_TYPE_EN与FW_COMMAND_TYPE_EN两个结构需要保存一致
 */
typedef enum tagAtpFwCommandType
{
    ATP_FW_CMD_TYPE_ADD = 0,              /*!< FW_CMD_TYPE_ADD */
    ATP_FW_CMD_TYPE_INSERT = 1,        /*!< FW_CMD_TYPE_INSERT */
    ATP_FW_CMD_TYPE_REMOVE = 2       /*!< FW_CMD_TYPE_REMOVE */
}ATP_FW_COMMAND_TYPE_EN;   /*!< FW_COMMAND_TYPE_EN */


#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief 防火墙初始化函数
  \param[in] 无: 
  \return VOS_OK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_Init(VOS_VOID);

/*!
  \brief 防火墙去初始化函数
  \param[in] 无: 
  \return VOS_OK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_UnInit(VOS_VOID);

#if 0
/*!
  \brief 防火墙设置当前等级
  \param[in]  ulCmo: 
  \param[in]  *pValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_FIREWALL_SetCurrLevel(VOS_UINT32 ulCmo, VOS_INT8 *pValue);

/*!
  \brief  防火墙获取当前等级
  输入参数  : 
  \param[in]  ulCmo: 
  \param[in]  *pulValue: 
  \param[in]  ulLength: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_FIREWALL_GetCurrLevel(VOS_UINT32 ulCmo, VOS_UINT32 *pulValue,VOS_UINT32 ulLength);
#endif

/*!
  \brief WAN UP时，进行防火墙规则处理
  \param[in]  pszWanPath: WAN路径名
  \param[in]  *pvHook: 没用到，仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStart (VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief WAN Down时，进行规则处理函数
  \param[in]  pszWanPath: WAN路径名
  \param[in]  *pvHook: 没用到，仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStop(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

#ifdef SUPPORT_ATP_FIREWALL_IPV6
/*!
  \brief IPv6使能的WAN接口Up时启动相关ipv6防火墙规则
  \param[in]  pszWanPath: IPv6 WAN路径名
  \param[in]  *pvHook: 没用到，仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStart6 (VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief IPv6使能的WAN接口Down掉时启动相关ipv6防火墙规则
  \param[in]  pszWanPath: IPv6 WAN路径名
  \param[in]  *pvHook: 没用到，仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanStop6(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
#endif

#ifdef SUPPORT_ATP_IPV6
/* <DTS2013050807659 wanglong 2013-5-9 begin */
/******************************************************************************
  函数名称  : ATP_FIREWALL_Ipv6WanAllowMcastNs
  功能描述  : 
  输入参数  : 
              1.  pszWanPath:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2013-4-15
     作    者   : l00184769
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_Ipv6WanAllowMcastNs(VOS_CHAR* pszWanPath, VOS_BOOL bAllow);
/* DTS2013050807659 wanglong 2013-5-9 end> */
/*!
  \brief IPv6 WAN UP时，进行tcp mss处理函数
  \param[in]  pszWanPath: IPv6 WAN路径名
  \param[in]  *pvHook: 仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_IPV6_MSS_AdjustStart(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief IPv6 WAN Down时，进行tcp mss处理函数
  \param[in]  *pszWanPath:  IPv6 WAN路径名
  \param[in]  *pvHook: 仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_IPV6_MSS_AdjustStop(VOS_VOID *pszWanPath, VOS_VOID *pvHook);
#endif

/*!
  \brief WAN 删除时，删掉这条WAN上对应的防火墙规则
  \param[in]  pszWanPath: WAN路径名
  \param[in]  *pvHook: 仅为符合WAN状态钩子函数统一格式
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_WanClear(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/*!
  \brief LAN维护地址改变后，更新mac过滤中必须允许的规则
  \param[in]  *pvNotifier: 为符合WAN状态钩子函数统一格式,使用时转换为IP格式ATP_LAN_SrvInfo
  \param[in]  *pvHook: 
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_LanChange(VOS_VOID *pvNotifier, VOS_VOID *pvHook);

/*!
  \brief 防火墙对IP Extension的接口函数
  \param[in]  pcIpaddr: IP地址
  \param[in]  ulIpExtCmd: ATP_FW_COMMAND_TYPE_EN类型操作类型add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceIpExt(VOS_CHAR* pcIpaddr, VOS_UINT32 ulIpExtCmd);

/*!
  \brief 防火墙对SIP ALG的接口函数
  \param[in]  pszWanRemote: WAN接口名
  \param[in]  ulPort: SIP信令端口
  \param[in]  ulRtpPortLow: RTP起始端口
  \param[in]  ulRtpPortHigh: RTP结束端口
  \param[in]  ulSipAlgCmd: FW_COMMAND_TYPE_EN类型，add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceSipAlg(VOS_CHAR* pszWanRemote, VOS_UINT32 ulPort, 
                                      VOS_UINT32 ulRtpPortLow, VOS_UINT32 ulRtpPortHigh,
                                      VOS_UINT32 ulSipAlgCmd);

/*!
  \brief 防火墙对FTP的接口函数
  \param[in]  ulOldPort: 旧端口
  \param[in]  ulNewPort: 新端口
  \param[in]  ulFtpCmd: FW_COMMAND_TYPE_EN类型，add/insert/remove
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceFtp(VOS_UINT32 ulOldPort,VOS_UINT32 ulNewPort,VOS_UINT32 ulFtpCmd);

#ifdef SUPPORT_ATP_VOICE
/*!
  \brief 防火墙服务对语音模块的接口函数
  \param[in]  lLocalPortMin: 本地端口起始端口
  \param[in]  lLocalPortMax: 本地端口结束端口
  \param[in]  ulVoiceCmd: ATP_FW_CMD_TYPE_ADD  打开相应接口\n
                                             ATP_FW_CMD_TYPE_REMOVE  关闭相应接口\n
  \return VOS_OK/VOS_NOK
  \retval 
*/
VOS_UINT32 ATP_FIREWALL_ServiceVoice(VOS_INT32 lLocalPortMin, VOS_INT32 lLocalPortMax, VOS_UINT32 ulVoiceCmd);
#endif

/*!
  \brief 获取防火墙是否使能
  \param[in] 无
  \return VOS_FALSE/VOS_TRUE
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_BOOL ATP_FIREWALL_GetEnable(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif

