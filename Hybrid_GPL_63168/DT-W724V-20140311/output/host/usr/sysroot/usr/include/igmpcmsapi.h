/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file igmpcmsapi.h
  *igmpcms����ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author t00189036
  *\date  2011-12-26
*/

#ifndef __ATP_IGMPCMS_API_H__
#define __ATP_IGMPCMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

//!\brief IGMPCMS�Ĵ�����
/*!
 *These errors are reserved for igmpcms only
 */
typedef enum tagIGMPCMS_RET_CODE_EN
{    
    IGMPCMS_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_IGMPCMS),/*!< ��δ��� */
    IGMPCMS_RET_MEM_ERR,/*!< �ڴ���� */
    IGMPCMS_RET_NOT_SPPORTED,/*!< ��֧�ֵ����� */
    IGMPCMS_RET_DB_ERR,/*!< ��д���ݿ�ʧ�� */
    IGMPCMS_RET_WAN_ERR/*!< WAN�������� */
} IGMPCMS_RET_CODE_EN;


/*!
  \brief IGMPproxy��ʼ������������IGMPproxy����
  \param[in] ��
  \return VOS_OK
*/
VOS_UINT32 ATP_IGMP_ProxyInit(VOS_VOID);
/*!
  \brief IGMPproxyȥ��ʼ��������ֹͣIGMPproxy����
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_IGMP_ProxyUnInit(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif

