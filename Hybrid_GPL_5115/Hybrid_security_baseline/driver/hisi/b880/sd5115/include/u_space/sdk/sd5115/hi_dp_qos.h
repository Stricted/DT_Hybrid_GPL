/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_qos.h
  版 本 号   : 初稿
  作    者   : z00209903
  生成日期   : 2012年5月17日
  最近修改   :
  功能描述   : hi dp qos SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年5月17日
     作    者   : z00209903
     修改内容: 创建文件

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_QOS_H__
 #define  __HI_DP_QOS_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_qos_scar_table_get(hi_dp_qos_scar_table_s  *pst_attr);
extern uint hi_dp_qos_scar_table_set(hi_dp_qos_scar_table_s  *pst_attr);
extern uint hi_dp_qos_ecar_table_item_get(hi_dp_qos_car_table_item_s *pst_item);
extern uint hi_dp_qos_ecar_table_item_set(hi_dp_qos_car_table_item_s *pst_item);
extern uint hi_dp_qos_tocpu_reason_table_item_set(hi_dp_qos_tocpu_reason_table_item_s *pst_item);
extern uint hi_dp_qos_tocpu_reason_table_item_get(hi_dp_qos_tocpu_reason_table_item_s *pst_item);
extern uint hi_dp_qos_tocpu_reason_car_table_item_set(hi_dp_qos_car_table_item_s  *pst_item);
extern uint hi_dp_qos_tocpu_reason_car_table_item_get(hi_dp_qos_car_table_item_s  *pst_item);
extern uint hi_dp_qos_gmap_table_item_set(hi_dp_qos_gmap_table_item_s *pst_item);
extern uint hi_dp_qos_gmap_table_item_get(hi_dp_qos_gmap_table_item_s *pst_item);
extern uint hi_dp_qos_fcar_table_item_set(hi_dp_qos_fcar_table_item_s  *pst_item);
extern uint hi_dp_qos_fcar_table_item_get(hi_dp_qos_fcar_table_item_s  *pst_item);
extern uint hi_dp_qos_gcar_table_item_set(hi_dp_qos_gcar_table_item_s *pst_item);
extern uint hi_dp_qos_gcar_table_item_get(hi_dp_qos_gcar_table_item_s  *pst_item);
extern uint hi_dp_qos_qmap_table_item_set(hi_dp_qos_qmap_table_item_s *pst_item);
extern uint hi_dp_qos_qmap_table_item_get(hi_dp_qos_qmap_table_item_s  *pst_item);
extern uint hi_dp_qos_queue_attr_set(hi_dp_qos_queue_attr_s  *pst_attr);
extern uint hi_dp_qos_queue_attr_get(hi_dp_qos_queue_attr_s  *pst_attr);
extern uint hi_dp_qos_fsch_table_item_set(hi_dp_qos_fsch_table_item_s *pst_item);
extern uint hi_dp_qos_fsch_table_item_get(hi_dp_qos_fsch_table_item_s *pst_item);
extern uint hi_dp_qos_gsch_table_item_set(hi_dp_qos_gsch_table_item_s *pst_item);
extern uint hi_dp_qos_gsch_table_item_get(hi_dp_qos_gsch_table_item_s *pst_item);
extern uint hi_dp_qos_fwred_table_item_set(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint hi_dp_qos_fwred_table_item_get(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint hi_dp_qos_gwred_table_item_set(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint hi_dp_qos_gwred_table_item_get(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint hi_dp_qos_egr_shaping_table_item_set(hi_dp_qos_egr_shaping_table_item_s  *pst_item);
extern uint hi_dp_qos_egr_shaping_table_item_get(hi_dp_qos_egr_shaping_table_item_s  *pst_item);
extern uint hi_dp_qos_attr_set(hi_dp_qos_attr_s  *pst_attr);
extern uint hi_dp_qos_attr_get(hi_dp_qos_attr_s  *pst_attr);
extern uint hi_dp_qos_queue_cnt_table_item_get(hi_dp_qos_queue_cnt_table_item_s  *pst_cnt);
extern uint hi_dp_qos_queue_cnt_table_item_clr(uint ui_qid);
extern uint hi_dp_qos_cnt_get(hi_dp_qos_cnt_s  *pst_cnt);
extern uint hi_dp_qos_cnt_clr(void);
extern uint hi_dp_qos_sta_get(hi_dp_qos_sta_s *pst_sta);

extern uint hi_dp_qos_debug(hi_dp_qos_debug_s *pst_sta);

/* 5115Add */
extern uint hi_dp_qos_fwred_map_table_item_set(hi_dp_qos_fwred_map_table_item_s *pst_item);
extern uint hi_dp_qos_fwred_map_table_item_get(hi_dp_qos_fwred_map_table_item_s *pst_item);
extern uint hi_dp_qos_gwred_map_table_item_set(hi_dp_qos_gwred_map_table_item_s *pst_item);
extern uint hi_dp_qos_gwred_map_table_item_get(hi_dp_qos_gwred_map_table_item_s *pst_item);
extern uint hi_dp_qos_epfc_map_table_item_set(hi_dp_qos_epfc_map_table_item_s * pst_item);
extern uint hi_dp_qos_epfc_map_table_item_get(hi_dp_qos_epfc_map_table_item_s * pst_item);
extern uint hi_dp_qos_fcar_map_table_item_set(hi_dp_qos_fcar_map_table_item_s *pst_item);
extern uint hi_dp_qos_fcar_map_table_item_get(hi_dp_qos_fcar_map_table_item_s *pst_item);
extern uint hi_dp_qos_gcar_map_table_item_set(hi_dp_qos_gcar_map_table_item_s *pst_item);
extern uint hi_dp_qos_gcar_map_table_item_get(hi_dp_qos_gcar_map_table_item_s *pst_item);
extern uint hi_dp_qos_queue_shaping_table_item_set(hi_dp_qos_queue_shaping_table_item_s  *pst_item);
extern uint hi_dp_qos_queue_shaping_table_item_get(hi_dp_qos_queue_shaping_table_item_s  *pst_item);
extern uint hi_dp_qos_group_shaping_table_item_set(hi_dp_qos_gs_shaping_table_item_s *pst_item);
extern uint hi_dp_qos_group_shaping_table_item_get(hi_dp_qos_gs_shaping_table_item_s *pst_item);
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint hi_dp_qos_qg_attr_get(hi_dp_qos_qg_attr_s *pst_item);
extern uint hi_dp_qos_qg_attr_set(hi_dp_qos_qg_attr_s *pst_item);
extern uint hi_dp_qos_eqg_attr_get(hi_dp_qos_eqg_attr_s *pst_item);
extern uint hi_dp_qos_eqg_attr_set(hi_dp_qos_eqg_attr_s *pst_item);
extern uint hi_dp_qos_tocpu_reason_pri_reset_get(hi_dp_qos_pri_reset_table_s *pst_item);
extern uint hi_dp_qos_tocpu_reason_pri_reset_set(hi_dp_qos_pri_reset_table_s *pst_item);
extern uint hi_dp_qos_drop_reason_get(hi_dp_qos_drop_reason_cnt_s  *pst_cnt);
extern uint hi_dp_qos_drop_reason_clr(HI_DP_PORT_E em_port);
 #endif
#ifdef CONFIG_HISI_SD5115_TYPE_T
extern uint hi_dp_qos_queue_attr_5115t_set(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
extern uint hi_dp_qos_queue_attr_5115t_get(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
#endif
#endif /* end of __HI_DP_QOS_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
