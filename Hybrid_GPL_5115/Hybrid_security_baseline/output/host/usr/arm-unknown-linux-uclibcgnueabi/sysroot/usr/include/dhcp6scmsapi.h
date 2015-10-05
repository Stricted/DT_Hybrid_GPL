/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6scmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x192803
  *\date  2011-12-20
*/


#ifndef __DHCP6SAPI_H__
#define __DHCP6SAPI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "ipv6commoncmsapi.h"

//TR181 specific CMO marco, the data model same as tr098, but with subtle different names
#ifdef SUPPORT_ATP_TR181
// max number of serving pools
#define ATP_DHCPv6_SERVING_POOL_MAX     8
#define ATP_DHCPv6_VENDORID_LEN_MAX     32
#define ATP_DHCPv6_OPTION_MAX     32

// Dhcpv6.Server
#define CMO_LDEV_LHCM_DHCPV6_X_DHCPV6_OBJ   CMO_DHCPV6_SER_PL_POOL_OBJ 
#define CMO_LDEV_LHCM_DHCPV6_DHCPSERVERENABLE  CMO_DHCPV6_SER_ENABLE
#define CMO_LDEV_LHCM_DHCPV6_ASSOCIATEDCONNECTION  CMO_DHCPV6_SER_PL_ASSOCIATEDCONNECTION
#define CMO_LDEV_LHCM_DHCPV6_USEALLOCATEDWAN  CMO_DHCPV6_SER_PL_USEALLOCATEDWAN
#endif


#ifdef ATP_DEBUG
#define IPV6_LAN_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define IPV6_LAN_DEBUG(format,arg...)
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif

//!合法ipv6地址长度
#define ATP_DHCP6S_BUFFER_LENGTH     (128)

//#define ATP_DHCP6S_DNS_SERVER_NUM    (2)

//!DHCP6S状态标志
#define ATP_DHCP6S_FLAG_ENABLE       (1)
#define ATP_DHCP6S_FLAG_DISABLE      (0)

//!DHCP6S配置模式
#define ATP_DHCP6S_MODE_NORMAL       "Normal"

//!配置文件缓存大小
#define DHCP6S_CFG_FILE_BUFF        (128)
#define DHCP6S_CFG_WAN_NAME_BUFF    (256)
#define DHCP6S_CFG_TEM_BUFF         (512)

//!保存前缀数据的临时缓冲区大小
#define DHCP6S_PREFIX_BUFFER        (24)

//!定义固定前缀长度
#define DHCP6S_PREFIX_MAX_LENGTH    (64)
#define DHCP6S_PREFIX_MIN_LENGTH    (16)

#define DHCP6S_TINY_LEN 16


/*!
  \brief DHCP6s初始化函数
  \param[in] VOS_VOID: 
  \return VOS_INT32
  \retval 初始化是否成功
*/
VOS_UINT32 ATP_DHCP6S_Init(VOS_VOID);

/*!
  \brief 使用WAN测参数配置DHCPv6 Server服务器
  \param[in]  pstDHCPv6Para: 
  \return VOS_INT32
  \retval 
*/
VOS_INT32 ATP_DHCPV6_SERVER_LanHostCfgUseParameterFromWAN(const ATP_DHCP6S_PARAMETER_ST * pstDHCPv6Para);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DHCP6SCTL_H__ */


