
#ifndef __VLANCMS_API_H__
#define __VLANCMS_API_H__
#include "bcmtypes.h"

#include "bcm_vlan.h"

#define BCM_VLAN_IF_SUFFIX ""


#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief ɾ��������wan�ӿ�
  \param[in]  *L3DevName: wan�ӿ���
  \return 
  \retval VOS_OK �ɹ�VOS_NOKʧ��
*/
 
 VOS_UINT32 ATP_VLAN_VLANMux_DeleteWANInf( 
                                 VOS_CHAR *L3DevName
                                 ) ;
/*!
  \brief ����wan�ӿ�
  \param[in]  *L2DevName:����ӿ��� ����nas0
  \param[in]  *L3DevName: ����ӿ�������nas0_v.707.1...�ӿ����������򣺴�vlanʱ��nas0_v.{vlan id}.{wanInstId} ����vlanʱ��nas0_v_{����ӿ�index}.{wanInstId}
  \param[in]  isRouted: �Ƿ�Ϊ·��wan�����Ž�wan
  \param[in]  isMulticast: ֧�ֹ㲥
  \param[in]  mode: BCM_VLAN_MODE_RG��BCM_VLAN_MODE_ONT
  \return 
  \retval VOS_OK �ɹ�VOS_NOKʧ��
*/

 VOS_UINT32 ATP_VLAN_VLANMux_CreateWANInf( 
                                 VOS_CHAR *L2DevName, 
                                 VOS_CHAR *L3DevName, 
                                 VOS_BOOL isRouted, 
                                 VOS_BOOL isMulticast,
                                 bcmVlan_realDevMode_t mode
                                 ) ;

/*!
  \brief ���ô�vlan tag�Ľӿڽ��ܹ���
  \param[in]  *L3DevName: ����ӿ���
  \param[in]  *L2DevName: ����ӿ���
  \param[in]  isRouted: �Ƿ�Ϊ·��wan�����Ž�wan
  \param[in]  vbits: vlan id
  \param[in]  pbits: ���ȼ�����
  \return 
  \retval VOS_OK �ɹ�VOS_NOKʧ��
*/

VOS_UINT32 ATP_VLAN_Tagged_SetTxRules( 
                                VOS_CHAR *L3DevName, 
                                VOS_CHAR *L2DevName, 
                                VOS_BOOL isRouted,
                                VOS_UINT32 vbits,
                                VOS_UINT32 pbits
                                ) ;


/*!
  \brief ���ò���vlan tag�Ľӿڽ��ܹ���
  \param[in]  *L3DevName: ����ӿ���
  \param[in]  *L2DevName: ����ӿ���
  \param[in]  isRouted: �Ƿ�Ϊ·��wan�����Ž�wan
  \return 
  \retval VOS_OK �ɹ�VOS_NOKʧ��
*/

VOS_UINT32 ATP_VLAN_Untagged_SetTxRules( 
                                VOS_CHAR *L3DevName, 
                                VOS_CHAR *L2DevName, 
                                VOS_BOOL isRouted
                                ) ;

#if defined(__cplusplus)
}
#endif

#endif





