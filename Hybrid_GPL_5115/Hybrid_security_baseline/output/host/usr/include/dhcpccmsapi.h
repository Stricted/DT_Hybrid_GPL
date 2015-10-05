/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcpccmsapi.h
  *dhcpccms����ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author t00189036
  *\date  2012-1-5
*/
#ifndef __DHCPCCMS_API_H__
#define __DHCPCCMS_API_H__
#include "msgapi.h"

#define ATP_DHCP_CLIENT_IFC_NAME_LEN        (32)
#define ATP_DHCP_CLIENT_OPTION_NAME_LEN     (64)

//!\brief DHCPCCMS�Ĵ�����
/*!
 *These errors are reserved for dhcpccms only
 */
typedef enum tagDHCPCCMS_RET_CODE_EN
{    
    DHCPCCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DHCPCCMS),/*!< ��δ��� */
    DHCPCCMS_RET_MEM_ERR,/*!< �ڴ���� */
    DHCPCCMS_RET_NOT_SPPORTED,/*!< ��֧�ֵ����� */
    DHCPCCMS_RET_WAN_ERR,/*!< ��WAN�йصĴ���  */
    DHCPCCMS_RET_FILE_ERR,/*!< ���ļ��йصĴ���  */
} DHCPCCMS_RET_CODE_EN;

//!\brief DHCP Client��ʼ�������ַ�ʽ
/*!
 *����BRIDGE��ʽ��IPOE��ʽ
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

//!\brief DHCP Client��ʼ����Ҫʹ�õĽṹ��
/*!
 *����IfcName��OptionName
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
  \brief DHCP Client�ĳ�ʼ��
  \param[in]  ulIndex: ��ATP_DHCP_CLIENT_INIT_ENָ����ʼ���ķ�ʽ
  \param[in]  pstDhcpcInfo: ��ʼ����Ҫ�Ľṹ��
  \return DHCPCCMS_RET_CODE_EN�Ĵ�����
*/
VOS_UINT32 ATP_DHCP_CLIENT_Init(VOS_UINT32 ulIndex, ATP_DHCP_CLIENT_INFO_ST* pstDhcpcInfo);
/*!
  \brief DHCP Client��ȥ��ʼ��
  \param[in]  ulIndex: ��ATP_DHCP_CLIENT_INIT_ENָ��ȥ��ʼ���ķ�ʽ
  \param[in]  pstDhcpcInfo: ��ʼ����Ҫ�Ľṹ��
  \return DHCPCCMS_RET_CODE_EN�Ĵ�����
*/
VOS_UINT32 ATP_DHCP_CLIENT_UnInit(VOS_UINT32 ulIndex,ATP_DHCP_CLIENT_INFO_ST* pstDhcpcInfo);


/*!
  \brief DhcpcOption������
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_DhcpcOptionProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
                                            						
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
