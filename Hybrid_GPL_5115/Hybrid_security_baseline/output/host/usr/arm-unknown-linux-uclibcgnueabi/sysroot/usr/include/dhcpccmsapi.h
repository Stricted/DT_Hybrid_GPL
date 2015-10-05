/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcpccmsapi.h
  *dhcpccms对外头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-1-5
*/
#ifndef __DHCPCCMS_API_H__
#define __DHCPCCMS_API_H__
#include "msgapi.h"

#define ATP_DHCP_CLIENT_IFC_NAME_LEN        (32)
#define ATP_DHCP_CLIENT_OPTION_NAME_LEN     (64)

//!\brief DHCPCCMS的错误码
/*!
 *These errors are reserved for dhcpccms only
 */
typedef enum tagDHCPCCMS_RET_CODE_EN
{    
    DHCPCCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DHCPCCMS),/*!< 入参错误 */
    DHCPCCMS_RET_MEM_ERR,/*!< 内存错误 */
    DHCPCCMS_RET_NOT_SPPORTED,/*!< 不支持的类型 */
    DHCPCCMS_RET_WAN_ERR,/*!< 与WAN有关的错误  */
    DHCPCCMS_RET_FILE_ERR,/*!< 与文件有关的错误  */
} DHCPCCMS_RET_CODE_EN;

//!\brief DHCP Client初始化的两种方式
/*!
 *包含BRIDGE方式和IPOE方式
 */
typedef enum 
{
    ATP_DHCP_CLIENT_BRIDGE = 0,
    ATP_DHCP_CLIENT_IPOE
}ATP_DHCP_CLIENT_INIT_EN;

typedef enum
{
    ATP_DHCPC_OPTIONS_PROC_SNTP=1,
    ATP_DHCPC_OPTIONS_PROC_CWMP,
    ATP_DHCPC_OPTIONS_PROC_VOICE, 
    ATP_DHCPC_OPTIONS_PROC_ROUTE,     
}ATP_DHCPC_OPTIONS_PROC;

//!\brief DHCP Client初始化需要使用的结构体
/*!
 *包含IfcName和OptionName
 */
typedef struct tagATP_DHCP_CLIENT_INFO_ST
{
    VOS_CHAR acIfcName[ATP_DHCP_CLIENT_IFC_NAME_LEN];/*!< wan interface */
    VOS_CHAR acOptionName[ATP_DHCP_CLIENT_OPTION_NAME_LEN];/*!< option name */
}ATP_DHCP_CLIENT_INFO_ST;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*!
  \brief DHCP Client的初始化
  \param[in]  ulIndex: 用ATP_DHCP_CLIENT_INIT_EN指定初始化的方式
  \param[in]  pstDhcpcInfo: 初始化需要的结构体
  \return DHCPCCMS_RET_CODE_EN的错误码
*/
VOS_UINT32 ATP_DHCP_CLIENT_Init(VOS_UINT32 ulIndex, ATP_DHCP_CLIENT_INFO_ST* pstDhcpcInfo);
/*!
  \brief DHCP Client的去初始化
  \param[in]  ulIndex: 用ATP_DHCP_CLIENT_INIT_EN指定去初始化的方式
  \param[in]  pstDhcpcInfo: 初始化需要的结构体
  \return DHCPCCMS_RET_CODE_EN的错误码
*/
VOS_UINT32 ATP_DHCP_CLIENT_UnInit(VOS_UINT32 ulIndex,ATP_DHCP_CLIENT_INFO_ST* pstDhcpcInfo);


/*!
  \brief DhcpcOption处理函数
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_DhcpcOptionProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
                                            						
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
