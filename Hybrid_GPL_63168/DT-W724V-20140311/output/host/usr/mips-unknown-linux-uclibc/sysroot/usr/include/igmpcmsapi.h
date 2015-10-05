/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file igmpcmsapi.h
  *igmpcms对外头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2011-12-26
*/

#ifndef __ATP_IGMPCMS_API_H__
#define __ATP_IGMPCMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

//!\brief IGMPCMS的错误码
/*!
 *These errors are reserved for igmpcms only
 */
typedef enum tagIGMPCMS_RET_CODE_EN
{    
    IGMPCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_IGMPCMS),/*!< 入参错误 */
    IGMPCMS_RET_MEM_ERR,/*!< 内存错误 */
    IGMPCMS_RET_NOT_SPPORTED,/*!< 不支持的类型 */
    IGMPCMS_RET_DB_ERR,/*!< 读写数据库失败 */
    IGMPCMS_RET_WAN_ERR/*!< WAN参数错误 */
} IGMPCMS_RET_CODE_EN;


/*!
  \brief IGMPproxy初始化函数，启动IGMPproxy进程
  \param[in] 无
  \return VOS_OK
*/
VOS_UINT32 ATP_IGMP_ProxyInit(VOS_VOID);
/*!
  \brief IGMPproxy去初始化函数，停止IGMPproxy进程
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_IGMP_ProxyUnInit(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif

