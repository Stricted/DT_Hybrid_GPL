/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file radvdcmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x192803
  *\date  2011-12-20
*/


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


#ifndef __RADVDAPI_H__
#define __RADVDAPI_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "ipv6commoncmsapi.h"

#ifdef ATP_DEBUG
#define IPV6_LAN_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define IPV6_LAN_DEBUG(format,arg...)
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif


/* Radvd 标志位枚举 */
//!\brief Radvd 标志位枚举 
/*!
 * Radvd 标志位枚举 
 */
typedef enum tag_RADVD_ENABLE_FLAG_E
{
    RADVD_FLAG_FALSE = 0,/*!< RADVD is disable */
    RADVD_FLAG_TRUE  = 1,/*!<  RADVD is enable*/
    RADVD_FLAG_EMPTY = 2,/*!< not used now */
    RADVD_FLAG_BUFF
}RADVD_ENABLE_FLAG_E;


/*!
  \brief RADVD CMS 初始化函数
  \param[in] VOS_VOID: 
  \return VOS_UINT32
  \retval 返回VOS_OK代表初始化成功
*/
extern VOS_UINT32 ATP_RADVD_Init(VOS_VOID);

/*!
  \brief 重新配置RADVD进程
  \param[in] VOS_VOID: 
  \return VOS_INT32
  \retval 重新配置是否成功
*/
extern VOS_INT32 ATP_RADVD_ReconfigRadvdPro(VOS_INT32 lBridgeKey);

/*!
  \brief 使用WAN侧数据配置RADVD参数
  \param[in]  pulSetValues: 
  \param[in]  pstDHCPv6Para: 
  \return 
  \retval 配置是否成功
*/
extern VOS_INT32 ATP_RADVD_UseParameterFromWAN(const VOS_UINT32 * pulSetValues, const ATP_DHCP6S_PARAMETER_ST * pstDHCPv6Para);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* __RADVDAPI_H__ */
