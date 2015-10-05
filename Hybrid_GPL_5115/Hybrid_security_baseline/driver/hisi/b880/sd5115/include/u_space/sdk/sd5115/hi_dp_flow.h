/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_flow.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2012��5��17��
  ����޸�   :
  ��������   : hi dp flow SDK�ӿ�ͷ�ļ�
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

#ifndef  __HI_DP_FLOW_H__
 #define  __HI_DP_FLOW_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_flow_attr_get(hi_dp_flow_attr_s  *pst_attr);
extern uint hi_dp_flow_attr_set(hi_dp_flow_attr_s  *pst_attr);

extern uint hi_dp_flow_fc_table_item_get(hi_dp_flow_fc_table_item_s  *pst_item);
extern uint hi_dp_flow_fc_table_item_set(hi_dp_flow_fc_table_item_s  *pst_item);

extern uint hi_dp_flow_efc_table_item_get(hi_dp_flow_efc_table_item_s  *pst_item);
extern uint hi_dp_flow_efc_table_item_set(hi_dp_flow_efc_table_item_s  *pst_item);
extern uint hi_dp_flow_efc_table_item_clr(hi_dp_flow_efc_table_item_s  *pst_item);

extern uint hi_dp_flow_sfc_table_item_get(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_table_item_set(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_table_item_clr(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_map_table_item_get(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_map_table_item_set(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_qos_table_item_get(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_dp_flow_sfc_qos_table_item_set(hi_dp_flow_sfc_table_item_s  *pst_item);

/*��ͨ����TF FL�滻��*/
extern uint hi_dp_flow_tcfl_rep_table_item_get(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);
extern uint hi_dp_flow_tcfl_rep_table_item_set(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);

/*�������TF FL�滻��*/
extern uint hi_dp_flow_tunnel_tcfl_rep_table_item_get(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);
extern uint hi_dp_flow_tunnel_tcfl_rep_table_item_set(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);

#ifdef CONFIG_HISI_SD5115_MP_MPW
extern uint hi_dp_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint hi_dp_flow_cnt_table_item_clr(uint ui_statis_id);
#endif

#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint hi_dp_igr_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint hi_dp_igr_flow_cnt_table_item_clr(uint ui_statis_id);

extern uint hi_dp_egr_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint hi_dp_egr_flow_cnt_table_item_clr(uint ui_statis_id);

extern uint hi_dp_flow_cnt_match_table_item_get(hi_dp_flow_cnt_match_table_item_s  *pst_item);
extern uint hi_dp_flow_cnt_match_table_item_set(hi_dp_flow_cnt_match_table_item_s  *pst_item);
#endif

extern uint hi_dp_flow_cnt_get(hi_dp_flow_cnt_s  *pst_cnt);
extern uint hi_dp_flow_cnt_clr( void );

extern uint hi_dp_flow_rtp_req( void );


extern uint hi_dp_thru_reason_cnt_get(hi_dp_thru_reason_cnt_s  *pst_cnt);

extern uint hi_dp_tocpu_reason_cnt_get(hi_dp_tocpu_reason_cnt_s  *pst_cnt);

extern uint hi_dp_thru_reason_cnt_clr( void );

extern uint hi_dp_tocpu_reason_cnt_clr( void );

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
