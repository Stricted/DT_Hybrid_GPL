/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_special.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2012��5��17��
  ����޸�   :
  ��������   : hi dp special SDK�ӿ�ͷ�ļ�
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

#ifndef  __HI_DP_SPECIAL_H__
#define  __HI_DP_SPECIAL_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_special_pkt_attr_get(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint hi_dp_special_pkt_attr_set(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint hi_dp_special_pkt_cnt_get(hi_dp_special_pkt_cnt_s  *pst_cnt);
extern uint hi_dp_special_pkt_cnt_clr( void );

extern uint hi_dp_special_pkt_trans_table_item_set(hi_dp_special_pkt_trans_table_item_s *pst_item);
extern uint hi_dp_special_pkt_trans_table_item_get(hi_dp_special_pkt_trans_table_item_s *pst_item);

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
