/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_l2.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2012年5月17日
  最近修改   :
  功能描述   : hi dp l2 SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年5月17日
     作    者   : x00214067
     修改内容: 创建文件

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_L2_H__
 #define  __HI_DP_L2_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_l2_attr_get(hi_dp_l2_attr_s    *pst_attr);
extern uint hi_dp_l2_attr_set(hi_dp_l2_attr_s    *pst_attr);
extern uint hi_dp_l2_act_table_item_get(hi_dp_l2_act_table_item_s    *pst_item);
extern uint hi_dp_l2_act_table_item_set(hi_dp_l2_act_table_item_s    *pst_item);
extern uint hi_dp_l2_act_table_item_clr(hi_dp_l2_act_table_item_s    *pst_item);
extern uint hi_dp_l2_mac_table_item_get(hi_dp_l2_mac_table_item_s    *pst_item);
extern uint hi_dp_l2_mac_add(hi_dp_l2_mac_s    *pst_mac);
extern uint hi_dp_l2_mac_del(hi_dp_l2_mac_s    *pst_mac);
extern uint hi_dp_l2_mac_get(hi_dp_l2_mac_s    *pst_mac);

extern uint hi_dp_l2_mc_add(hi_dp_l2_mc_s    *pst_mc);
extern uint hi_dp_l2_mc_del(hi_dp_l2_mc_s    *pst_mc);
extern uint hi_dp_l2_mc_get(hi_dp_l2_mc_s    *pst_mc);
extern uint hi_dp_l2_mc_users_del(uint  ui_mc_id);
extern uint hi_dp_l2_mc_users_get(hi_dp_l2_mc_users_s    *pst_users);
extern uint hi_dp_l2_mc_user_add(hi_dp_l2_mc_user_s  *pst_user);
extern uint hi_dp_l2_mc_user_del(hi_dp_l2_mc_user_s  *pst_user);

extern uint hi_dp_l2_ip_mc_table_item_get(hi_dp_l2_ip_mc_s    *pst_item);
extern uint hi_dp_l2_ip_mc_table_item_set(hi_dp_l2_ip_mc_s    *pst_item);

extern uint hi_dp_l2_cnt_get(hi_dp_l2_cnt_s  *pst_cnt);

#ifdef CONFIG_HISI_SD5115_MP_MPW
extern uint hi_dp_l2_cnt_clr( void );
#endif

#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint hi_dp_l2_cnt_clr(uint ui_port_id);
#endif

extern uint hi_dp_l2_vlan_bc_add (hi_dp_l2_bc_egr_s *pst_egr);
extern uint hi_dp_l2_vlan_bc_del (hi_dp_l2_bc_egr_s *pst_egr);
extern uint hi_dp_l2_vlan_del (uint ui_vlan);
extern uint hi_dp_l2_vlan_bc_get (hi_dp_l2_bc_egrs_s *pst_egrs);

extern uint hi_dp_l2_mc_filter_add(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint hi_dp_l2_mc_filter_del(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint hi_dp_l2_mc_filter_get(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint hi_dp_l2_mc_filter_table_item_get(hi_dp_l2_mc_filter_s    *pst_item);
extern uint hi_dp_l2_mac_table_reset(void );

/* BEGIN: Added by w00180499, 2012/7/20 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
extern uint hi_dp_l2_mc_st_attr_get(hi_dp_l2_mc_st_attr_s    *pst_attr);
extern uint hi_dp_l2_mc_st_attr_set(hi_dp_l2_mc_st_attr_s    *pst_attr);
#endif
/* END:   Added by w00180499, 2012/7/20 */

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
