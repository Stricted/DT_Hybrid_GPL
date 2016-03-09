/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6ccmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x192803
  *\date  2012-1-4
*/

#ifndef __DHCP6C_API_H__
#define __DHCP6C_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

//!打印宏
#ifdef ATP_DEBUG
#define ATP_DHCP6C_DEBUG(format, args...) {printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_DHCP6C_DEBUG(x...)
#endif
#define ATP_DHCP6C_ERROR(format, args...) {printf("FILE: %s  LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}

/*!
  \brief 启动DHCP6C(如果是第一条wan，则靠命令行启动dhcp6c，如果是已有wan，则靠消息来创建新的dhcp6c结构体)
  \param[in]  pstWan: 
  \return VOS_UINT32
  \retval 启动是否成功
*/
VOS_UINT32  ATP_DHCP6C_Start(const ATP_WAN_INFO_ST* pstWan);

/*!
  \brief 停止DHCP6C(如果最后一条wan，kill dhcp6c进程，如果还有其他wan，则靠消息来删除当前的dhcp6c结构体)
  \param[in]  pstWan: 
  \return VOS_UINT32
  \retval 停止是否成功
*/
VOS_UINT32  ATP_DHCP6C_Stop(const ATP_WAN_INFO_ST* pstWan);

#ifdef SUPPORT_ATP_TR181
/*!
  \brief 通过接口找到当前client 获取的ipv6信息结点
  \param[in]  pcInterface: 
  \return 
  \retval 
*/
ATP_IPINTF_IPV6INFO_ST *ATP_DHCP6C_FindIpv6InfoByInterface(const char * pcInterface);

VOS_UINT32 ATP_Dhcp6cCms_Init(const VOS_CHAR *pcInterface);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
