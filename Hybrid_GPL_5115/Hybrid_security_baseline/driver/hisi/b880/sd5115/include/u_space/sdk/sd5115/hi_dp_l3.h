/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_l3.h
  �� �� ��   : ����
  ��    ��   : z00209903
  ��������   : 2012��5��17��
  ����޸�   :
  ��������   : hi dp l3 SDK�ӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��5��17��
     ��    ��   : z00209903
     �޸�����: �����ļ�

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_L3_H__
 #define  __HI_DP_L3_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_l3_attr_get(hi_dp_l3_attr_s  *pst_attr);
extern uint hi_dp_l3_attr_set(hi_dp_l3_attr_s  *pst_attr);
extern uint hi_dp_l3_replace_table_item_get(hi_dp_l3_replace_table_item_s  *pst_item);
extern uint hi_dp_l3_replace_table_item_set(hi_dp_l3_replace_table_item_s  *pst_item);

/* ֱ�ӷ��� */
extern uint hi_dp_l3_napt_table_item_get(hi_dp_l3_napt_table_item_s  *pst_item);
extern uint hi_dp_l3_napt_table_clr(void);
extern uint hi_dp_l3_napt_table_item_clr(uint ui_index);
#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint hi_dp_l3_rt_act_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint hi_dp_l3_napt_point_cnt_table_item_get(hi_dp_l3_napt_point_cnt_table_item_s *pst_item);
extern uint hi_dp_l3_napt_point_cnt_table_item_set(hi_dp_l3_napt_point_cnt_table_item_s *pst_item);
extern uint hi_dp_l3_napt_point_cnt_table_clr(void);
extern uint hi_dp_l3_napt_point_cnt_get(hi_dp_l3_napt_point_cnt_s *pst_cnt);
extern uint hi_dp_l3_napt_point_cnt_clr(void);
extern uint hi_dp_l3_alarm_attr_get(hi_dp_l3_alarm_attr_s  *pst_attr);
extern uint hi_dp_l3_alarm_attr_set(hi_dp_l3_alarm_attr_s  *pst_attr);
#endif
extern uint hi_dp_l3_nh_table_item_get(hi_dp_l3_nh_table_item_s *pst_item);
extern uint hi_dp_l3_nh_table_item_set(hi_dp_l3_nh_table_item_s *pst_item);
extern uint hi_dp_l3_nh_table_item_clr(hi_dp_l3_nh_table_item_s *pst_item);
extern uint hi_dp_l3_rt_table_item_get(hi_dp_l3_rt_table_item_s *pst_item);
extern uint hi_dp_l3_rt_table_item_set(hi_dp_l3_rt_table_item_s *pst_item);
extern uint hi_dp_l3_rt_table_item_clr(hi_dp_l3_rt_table_item_s *pst_item);

extern uint hi_dp_l3_rt_act_table_item_get(hi_dp_l3_rt_act_table_item_s *pst_item);
extern uint hi_dp_l3_rt_act_table_item_set(hi_dp_l3_rt_act_table_item_s *pst_item);
extern uint hi_dp_l3_rt_act_table_item_clr(hi_dp_l3_rt_act_table_item_s *pst_item);
extern uint hi_dp_l3_in_tunnel_rt_table_item_get(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint hi_dp_l3_in_tunnel_rt_table_item_set(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint hi_dp_l3_out_tunnel_match_table_item_get(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint hi_dp_l3_out_tunnel_match_table_item_set(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint hi_dp_l3_out_tunnel_sip_table_item_get(hi_dp_l3_tunnel_sip_table_item_s *pst_item);
extern uint hi_dp_l3_out_tunnel_sip_table_item_set(hi_dp_l3_tunnel_sip_table_item_s *pst_item);

extern uint hi_dp_l3_in_tunnel_head_table_item_get(hi_dp_l3_tunnel_table_item_s *pst_item);
extern uint hi_dp_l3_in_tunnel_head_table_item_set(hi_dp_l3_tunnel_table_item_s *pst_item);

extern uint hi_dp_l3_nh_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint hi_dp_l3_rt_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint hi_dp_l3_in_tunnel_rt_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint hi_dp_l3_out_tunnel_match_table_sta_get(hi_dp_l3_sta_s *pst_sta);

extern uint hi_dp_l3_cnt_get(hi_dp_l3_cnt_s  *pst_cnt);
extern uint hi_dp_l3_cnt_clr( void );

extern uint hi_dp_l3_napt_table_item_add(hi_dp_l3_napt_table_item_s  *pst_item);
extern uint hi_dp_l3_napt_table_item_del(hi_dp_l3_napt_table_item_s  *pst_item);
extern uint hi_dp_l3_napt_table_item_query(hi_dp_l3_napt_table_item_s  *pst_item);

extern uint hi_dp_l3_in_6rd_tunnel_head_table_item_get(hi_dp_l3_6rd_tunnel_table_item_s *pst_item);
extern uint hi_dp_l3_in_6rd_tunnel_head_table_item_set(hi_dp_l3_6rd_tunnel_table_item_s *pst_item);

/* BEGIN: Added by l00164498, 2012/8/6 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
extern uint hi_dp_l3_rsp_table_item_get(hi_dp_l3_rsp_table_item_s *pst_item);
extern uint hi_dp_l3_rsp_table_item_set(hi_dp_l3_rsp_table_item_s *pst_item);
#endif
/* END: Added by l00164498, 2012/8/6 */
extern uint hi_dp_l3_wan_cnt_get(hi_dp_wan_cnt_s  *pst_cnt);
extern uint hi_dp_l3_wan_cnt_clr( uint ui_index );

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
