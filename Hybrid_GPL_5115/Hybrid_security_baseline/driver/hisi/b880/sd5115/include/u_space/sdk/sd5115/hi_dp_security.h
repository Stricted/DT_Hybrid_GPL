/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_security.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2012��5��17��
  ����޸�   :
  ��������   : hi dp security SDK�ӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��5��17��
     ��    ��   : x00214067
     �޸�����: �����ļ�

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_SECURITY_H__
 #define  __HI_DP_SECURITY_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_security_attr_get(hi_dp_security_attr_s *pst_attr);
extern uint hi_dp_security_attr_set(hi_dp_security_attr_s *pst_attr);

/*���ݱ���*/
extern uint hi_dp_vlan_filter_attr_set(hi_dp_vlan_filter_attr_s *pst_attr);
extern uint hi_dp_vlan_filter_attr_get(hi_dp_vlan_filter_attr_s *pst_attr);

extern uint hi_dp_vlan_filter_table_item_get(hi_dp_vlan_filter_table_item_s  *pst_item);
extern uint hi_dp_vlan_filter_table_item_set(hi_dp_vlan_filter_table_item_s  *pst_item);

extern uint hi_dp_smac_filter_table_item_get(hi_dp_smac_filter_table_item_s  *pst_item);
extern uint hi_dp_smac_filter_table_item_set(hi_dp_smac_filter_table_item_s  *pst_item);

extern uint hi_dp_ip_filter_table_item_get(hi_dp_ip_filter_table_item_s  *pst_item);
extern uint hi_dp_ip_filter_table_item_set(hi_dp_ip_filter_table_item_s  *pst_item);

/*���ݱ���*/
extern uint hi_dp_vlan_filter_cnt_get(hi_dp_vlan_filter_cnt_s  *pst_cnt);
extern uint hi_dp_vlan_filter_cnt_clr( void );

extern uint hi_dp_security_cnt_get(hi_dp_security_cnt_s  *pst_cnt);
extern uint hi_dp_security_cnt_clr( void );
extern uint hi_dp_security_sta_get(hi_dp_security_sta_s  *pst_sta);

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
