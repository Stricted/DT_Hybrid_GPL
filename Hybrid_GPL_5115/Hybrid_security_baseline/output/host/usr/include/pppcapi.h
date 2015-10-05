/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file pppcapi.h
  *pppccms的API函数头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author d00191326
  *\date  2011-12-28
*/

#ifndef __PPPC_API_H__
#define __PPPC_API_H__

#include "atpconfig.h"

#ifdef ATP_DEBUG
#define ATP_PPPC_LOG(code, format, args...)   printf("PPPC: file[%s] line[%d] code[0x%x] " format "\r\n", __FILE__, __LINE__, code, ## args)
#else
#define ATP_PPPC_LOG(code, format, args...)
#endif

#ifdef SUPPORT_ATP_PRIVACY_BUTTON
//配置文件和trigger备份
#define DT_PPPC_CONFIG_BAK_PATH  "/var/pppc_config_bak"
#define DT_PPPC_DIAL_BAK_PATH  "/var/pppc_dial_bak"
#endif


#if defined(__cplusplus)
extern "C" {
#endif


/*!
  \brief 配置客户端参数
  \param[in]  pstConfig: 配置参数指针
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
unsigned int ATP_PPPC_Config(ATP_PPPC_CONFIG_ST* pstConfig);

/*!
  \brief 启动PPPC客户端进程
  \param[in]  pszInterface: ppp接口名
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
unsigned int ATP_PPPC_Start(char* pszInterface);

#ifdef SUPPORT_ATP_PRIVACY_BUTTON
/*!
  \brief W724v Privacy模式时,先恢复config, dial文件启动PPPC客户端进程
  \param[in]  pszInterface: ppp接口名
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
VOS_VOID ATP_PRIVACY_PPPC_ReStart(VOS_VOID *pvArg);
/*!
  \brief 关闭PPPC客户端进程,并备份config文件和dial文件
  \param[in]  pszInterface: ppp接口名
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
VOS_VOID ATP_PRIVACY_PPPC_Stop(char* pszInterface);
#endif

/*!
  \brief 关闭PPPC客户端进程
  \param[in]  pszInterface: ppp接口名
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
unsigned int ATP_PPPC_Stop(char* pszInterface);

/*!
  \brief 设置PPPC的触发方式
  \param[in]  pszInterface: PPP接口
  \param[in]  ulTrigger: 触发方式
  \return 成功ATP_PPPC_OK，失败ATP_ERR_PPPC_EN
*/
unsigned int ATP_PPPC_SetTrigger(char* pszInterface, unsigned int ulTrigger);

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
/*!
  \brief  DSL掉线时，记录DSL的PPP WAN掉线日志
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_VOID ATP_DT_Log_DslDown(VOS_VOID);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __PPPC_API_H__ */
