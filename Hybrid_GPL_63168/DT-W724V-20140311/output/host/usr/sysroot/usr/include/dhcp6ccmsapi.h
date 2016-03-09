/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6ccmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
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

//!��ӡ��
#ifdef ATP_DEBUG
#define ATP_DHCP6C_DEBUG(format, args...) {printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_DHCP6C_DEBUG(x...)
#endif
#define ATP_DHCP6C_ERROR(format, args...) {printf("FILE: %s  LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}

/*!
  \brief ����DHCP6C(����ǵ�һ��wan��������������dhcp6c�����������wan������Ϣ�������µ�dhcp6c�ṹ��)
  \param[in]  pstWan: 
  \return VOS_UINT32
  \retval �����Ƿ�ɹ�
*/
VOS_UINT32  ATP_DHCP6C_Start(const ATP_WAN_INFO_ST* pstWan);

/*!
  \brief ֹͣDHCP6C(������һ��wan��kill dhcp6c���̣������������wan������Ϣ��ɾ����ǰ��dhcp6c�ṹ��)
  \param[in]  pstWan: 
  \return VOS_UINT32
  \retval ֹͣ�Ƿ�ɹ�
*/
VOS_UINT32  ATP_DHCP6C_Stop(const ATP_WAN_INFO_ST* pstWan);

#ifdef SUPPORT_ATP_TR181
/*!
  \brief ͨ���ӿ��ҵ���ǰclient ��ȡ��ipv6��Ϣ���
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
