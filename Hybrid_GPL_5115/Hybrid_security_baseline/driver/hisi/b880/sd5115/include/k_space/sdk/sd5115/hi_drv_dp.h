/* *****************************************************************************
                  版权所有 (C), 2001-2011, 华为技术有限公司
******************************************************************************
  文 件 名   : hi_dp_dp_api.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月26日
  最近修改   :
  功能描述   : SD5113 SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月26日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */
#ifndef  __HI_DRV_DP_H__
#define  __HI_DRV_DP_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#include "hi_drv_dp_glb_config.h"
#include "hi_drv_dp_uni.h"
#include "hi_drv_dp_flow.h"
#include "hi_drv_dp_security.h"
#include "hi_drv_dp_special_pkt.h"
#include "hi_drv_dp_l2.h"
#include "hi_drv_dp_l3.h"
#include "hi_drv_dp_qos.h"
#include "hi_drv_dp_prbs.h"
#include "hi_drv_dp_oam.h"
#include "hi_drv_dp_gpon_dn.h"
#include "hi_drv_dp_gpon_up.h"
#include "hi_drv_dp_gpon.h"
#include "hi_drv_dp_epon_dn.h"
#include "hi_drv_dp_epon_up.h"
#include "hi_drv_dp_epon.h"
#include "hi_drv_dp_cnt.h"
#include "hi_drv_dp_smd_cmd.h"
#include "hi_drv_dp_nni_eth.h"

/* ********************************设备打开 关闭******************************************* */

/* ********************************访问权限获取******************************************* */

extern uint  hi_kernel_dp_right_get(void);
extern uint  hi_kernel_dp_right_release(void);

/* ********************************************************************************************* */
/* ********************************GLB CONFIG 模块******************************************* */
extern uint  hi_kernel_dp_glb_attr_get(hi_dp_glb_attr_s  *pst_attr);
extern uint  hi_kernel_dp_glb_attr_set(hi_dp_glb_attr_s  *pst_attr);
extern uint  hi_kernel_dp_glb_cnt_get(hi_dp_glb_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_glb_sta_get(hi_dp_glb_sta_s  *pst_sta);
extern uint  hi_kernel_dp_glb_cnt_clr(void);
extern uint  hi_kernel_dp_glb_spec_get( hi_dp_glb_spec_s *pst_spec );

//extern uint hi_kernel_dp_glb_xpon_sense( HI_DP_NNI_PORT_E *pem_nni );

/* BEGIN: Added by w00180499, 2011/5/26 */
/* ********************************************************************************************* */
/* ********************************   GPON 模块******************************************* */
extern uint  hi_kernel_dp_gpon_attr_get(hi_dp_gpon_attr_s *pst_attr);
extern uint  hi_kernel_dp_gpon_attr_set(hi_dp_gpon_attr_s *pst_attr);
extern uint  hi_kernel_dp_gpon_gemport_cnt_table_item_get(hi_dp_gpon_gemport_cnt_table_item_s *pst_cnt);
extern uint  hi_kernel_dp_gpon_gemport_cnt_table_item_set(hi_dp_gpon_gemport_cnt_table_item_s *pst_cnt);
extern uint  hi_kernel_dp_gpon_gemport_cnt_table_item_clr(uint ui_index);
extern uint  hi_kernel_dp_gpon_alarm_attr_get( hi_dp_gpon_alarm_attr_s* pst_attr );
extern uint  hi_kernel_dp_gpon_alarm_attr_set( hi_dp_gpon_alarm_attr_s* pst_attr );
extern uint  hi_kernel_dp_gpon_cnt_get( hi_dp_gpon_cnt_s* pst_cnt );
extern uint  hi_kernel_dp_gpon_cnt_clr( void );
extern void  hi_kernel_dp_gpon_int_clr(HI_DP_GPON_ALARM_E em_alarm);
extern uint  hi_kernel_dp_gpon_spcnt_get( hi_dp_gpon_spcnt_s*  pst_cnt);
extern uint  hi_kernel_dp_gpon_sta_get(hi_dp_gpon_sta_s  *pst_sta);
extern void  hi_kernel_dp_gpon_omci_reset(void);


/* ********************************************************************************************* */
/* ********************************   GPON DN 模块******************************************* */
extern uint  hi_kernel_dp_gpon_dn_attr_get(hi_dp_gpon_dn_attr_s *pst_attr);
extern uint  hi_kernel_dp_gpon_dn_attr_set(hi_dp_gpon_dn_attr_s *pst_attr);

extern uint  hi_kernel_dp_gpon_dn_gemport_table_item_get(hi_dp_gpon_dn_gemport_table_item_s *pst_item);
extern uint  hi_kernel_dp_gpon_dn_gemport_table_item_set(hi_dp_gpon_dn_gemport_table_item_s *pst_item);

extern void  hi_kernel_dp_gpon_dn_gemport_table_clr(void);

extern uint  hi_kernel_dp_gpon_dn_omci_receive(hi_omci_message_s  *pst_omci);

extern uint  hi_kernel_dp_gpon_dn_cnt_get(hi_dp_gpon_dn_cnt_s  *pst_cnt);
extern void  hi_kernel_dp_gpon_dn_cnt_clr(void);

extern uint  hi_kernel_dp_gpon_dn_sta_get(hi_dp_gpon_dn_sta_s  *pst_sta);

extern void  hi_kernel_dp_gpon_key_switch(void);

/*参数数据类型由hi_dp_gpon_dn_bwmap_table_item_s改为hi_dp_gpon_dn_bwmap_table_s,  by w00180499, 2011/4/2*/
extern uint  hi_kernel_dp_gpon_dn_bwmap_table_item_get(hi_dp_gpon_dn_bwmap_table_s *pst_table);

extern uint  hi_kernel_dp_gpon_dn_typeb_op(hi_dp_gpon_dn_typeb_op_s  *pst_op);

/* ********************************   GPON UP 模块******************************************* */
extern uint  hi_kernel_dp_gpon_up_attr_get(hi_dp_gpon_up_attr_s *pst_attr);
extern uint  hi_kernel_dp_gpon_up_attr_set(const hi_dp_gpon_up_attr_s *pst_attr);

extern uint  hi_kernel_dp_gpon_up_map_table_item_get(hi_dp_gpon_up_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_gpon_up_map_table_item_set(const hi_dp_gpon_up_map_table_item_s *pst_item);

extern uint  hi_kernel_dp_gpon_up_tcont_table_item_get(hi_dp_gpon_up_tcont_table_item_s *pst_item);
extern uint  hi_kernel_dp_gpon_up_tcont_table_item_set(const hi_dp_gpon_up_tcont_table_item_s *pst_item);

#if 0   //mpw asic not support;pilot fpga or later;fufenglian 2012/2/29

/*以alloc为操作对象,tcont为内容*/
extern uint  hi_kernel_dp_gpon_up_allocid_add(hi_dp_gpon_up_alloc_tcont_s  *pst_alloc);
extern uint  hi_kernel_dp_gpon_up_allocid_del(hi_dp_gpon_up_alloc_tcont_s  *pst_alloc);
extern uint  hi_kernel_dp_gpon_up_allocid_get(hi_dp_gpon_up_alloc_tcont_s  *pst_alloc);

/*以tcont为操作对象,alloc为内容*/
extern uint  hi_kernel_dp_gpon_up_tcont_add(hi_dp_gpon_up_alloc_tcont_s  *pst_tcont);
extern uint  hi_kernel_dp_gpon_up_tcont_del(hi_dp_gpon_up_alloc_tcont_s  *pst_tcont);
extern uint  hi_kernel_dp_gpon_up_tcont_get(hi_dp_gpon_up_alloc_tcont_s  *pst_tcont);
#endif
extern uint  hi_kernel_dp_gpon_up_omci_send(hi_omci_message_s  *pst_omci);
extern uint  hi_kernel_dp_gpon_up_ploam_send(hi_ploam_message_s  *pst_ploam);

/* BEGIN: Added by w00180499, 2011/5/31 */
extern uint  hi_kernel_dp_gpon_up_ploam_ready(void);
/* END: Added by w00180499, 2011/5/31 */
extern uint  hi_kernel_dp_gpon_up_cnt_get(hi_dp_gpon_up_cnt_s  *pst_cnt);
extern void  hi_kernel_dp_gpon_up_cnt_clr(void);

extern void  hi_kernel_dp_gpon_up_clean_tx_ploam(void);
extern void  hi_kernel_dp_gpon_up_eqd_update(void);
extern uint  hi_kernel_dp_gpon_up_sta_get(const hi_dp_gpon_up_sta_s *pst_sta );

/* ********************************************************************************************* */
/* ********************************   EPON 公共模块******************************************* */
extern uint  hi_kernel_dp_epon_attr_get( hi_dp_epon_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_attr_set( hi_dp_epon_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_alarm_attr_get( hi_dp_epon_alarm_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_alarm_attr_set( hi_dp_epon_alarm_attr_s* pst_attr );

extern uint  hi_kernel_dp_epon_cnt_get( hi_dp_epon_cnt_s* pst_cnt );
extern uint  hi_kernel_dp_epon_cnt_clr( void );
extern uint  hi_kernel_dp_epon_llid_cnt_get( hi_dp_epon_llid_cnt_s* pst_cnt );
extern uint  hi_kernel_dp_epon_llid_cnt_clr( uint ui_llid );
extern uint  hi_kernel_dp_epon_llid_bandwidth_get(hi_dp_epon_llid_bandwidth_s *pst_attr);

/* ********************************   EPON DN 模块******************************************* */
extern uint  hi_kernel_dp_epon_dn_attr_get( hi_dp_epon_dn_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_dn_attr_set( hi_dp_epon_dn_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_dn_llid_table_item_get( hi_dp_epon_dn_llid_table_item_s* pst_item );
extern uint  hi_kernel_dp_epon_dn_llid_table_item_set( hi_dp_epon_dn_llid_table_item_s* pst_item );
extern uint  hi_kernel_dp_epon_dn_sta_get( hi_dp_epon_dn_sta_s  *pst_sta );
extern uint  hi_kernel_dp_epon_dn_cnt_get( hi_dp_epon_dn_cnt_s  *pst_cnt );
extern void  hi_kernel_dp_epon_dn_cnt_clr( void );

/* ********************************   EPON UP 模块******************************************* */
extern uint  hi_kernel_dp_epon_up_attr_get( hi_dp_epon_up_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_up_attr_set( hi_dp_epon_up_attr_s* pst_attr );
extern uint  hi_kernel_dp_epon_up_map_table_item_get( hi_dp_epon_up_map_table_item_s *pst_item );
extern uint  hi_kernel_dp_epon_up_map_table_item_set( hi_dp_epon_up_map_table_item_s *pst_item );
extern uint  hi_kernel_dp_epon_up_report_table_item_get( hi_dp_epon_up_report_table_item_s *pst_item );
extern uint  hi_kernel_dp_epon_up_report_table_item_set( hi_dp_epon_up_report_table_item_s *pst_item  );
extern uint  hi_kernel_dp_epon_up_mpcp_send( hi_mpcp_message_s *pst_mpcp );
extern uint  hi_kernel_dp_epon_up_oam_send( hi_oam_message_s *pst_oam );
extern uint  hi_kernel_dp_epon_up_sta_get( hi_dp_epon_up_sta_s *pst_sta );
extern uint  hi_kernel_dp_epon_up_cnt_get( hi_dp_epon_up_cnt_s *pst_cnt );
extern void  hi_kernel_dp_epon_up_cnt_clr( void );

/* ********************************************************************************************* */
/* ********************************   NNI ETH 模块*********************************************** */
extern uint  hi_kernel_dp_nni_eth_port_attr_set(hi_dp_nni_eth_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_nni_eth_port_attr_get(hi_dp_nni_eth_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_nni_eth_port_cnt_get(hi_dp_nni_eth_port_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_nni_eth_port_cnt_clr(void);
extern uint  hi_kernel_dp_nni_eth_port_reset(void);
extern uint  hi_kernel_dp_nni_tag_sft_table_item_get(hi_dp_tag_sft_item_s *pst_item);
extern uint  hi_kernel_dp_nni_tag_sft_table_item_set(hi_dp_tag_sft_item_s *pst_item);
extern uint  hi_kernel_dp_nni_hybrid_table_item_get(hi_dp_nni_hybrid_item_s *pst_item);
extern uint  hi_kernel_dp_nni_hybrid_table_item_set(hi_dp_nni_hybrid_item_s *pst_item);

/* ********************************************************************************************* */
/* ********************************   UNI PORT 模块*********************************************** */
extern uint  hi_kernel_dp_uni_attr_get(hi_dp_uni_attr_s * pst_attr);
extern uint  hi_kernel_dp_uni_attr_set(hi_dp_uni_attr_s * pst_attr);

extern uint  hi_kernel_dp_uni_cpu_port_attr_get(hi_dp_uni_cpu_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_cpu_port_attr_set(hi_dp_uni_cpu_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_cpu_port_cnt_get(hi_dp_uni_cpu_port_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_uni_cpu_port_cnt_clr(void);

extern uint  hi_kernel_dp_uni_eth_port_attr_get(hi_dp_uni_eth_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_eth_port_attr_set(hi_dp_uni_eth_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_eth_port_cnt_get(hi_dp_uni_eth_port_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_uni_eth_port_cnt_clr(uint ui_port_index);
extern uint  hi_kernel_dp_uni_eth_port_sta_get(hi_dp_uni_eth_port_sta_s  *pst_sta);

extern uint  hi_kernel_dp_uni_eth_port_reset(HI_DP_PORT_E em_port);

#if 0  //delete by shengjing ; use  hi_kernel_dp_prbs_port_attr_get etc. 2012/2/29
extern uint  hi_kernel_dp_uni_prbs_port_attr_get(hi_dp_uni_prbs_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_prbs_port_attr_set(hi_dp_uni_prbs_port_attr_s  *pst_attr);
extern uint  hi_kernel_dp_uni_prbs_port_cnt_get(hi_dp_uni_prbs_port_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_uni_prbs_port_cnt_clr(void);
extern uint  hi_kernel_dp_uni_prbs_port_sta_get(hi_dp_uni_prbs_port_sta_s  *pst_sta);
extern uint  hi_kernel_dp_uni_prbs_port_start(void);
#endif

extern uint  hi_kernel_dp_uni_tag_sft_table_item_set(hi_dp_tag_sft_item_s *pst_item);
extern uint  hi_kernel_dp_uni_tag_sft_table_item_get(hi_dp_tag_sft_item_s *pst_item);

extern uint  hi_kernel_dp_uni_hybrid_table_item_set(hi_dp_uni_hybrid_item_s *pst_item);
extern uint  hi_kernel_dp_uni_hybrid_table_item_get(hi_dp_uni_hybrid_item_s *pst_item);

extern uint  hi_kernel_dp_uni_cascade_attr_set(hi_dp_uni_cascade_attr_s * pst_attr);
extern uint  hi_kernel_dp_uni_cascade_attr_get(hi_dp_uni_cascade_attr_s * pst_attr);

/* ********************************************************************************************* */
/* ********************************  SECURITY  模块*********************************************** */
/* BEGIN: Added by shengjing, 2010/10/19   PN:SD5113V200新增*/
extern uint  hi_kernel_dp_security_attr_get(hi_dp_security_attr_s *pst_attr);
extern uint  hi_kernel_dp_security_attr_set(hi_dp_security_attr_s *pst_attr);
/* END:   Added by shengjing, 2010/10/19 */

/*兼容保留*/
extern uint  hi_kernel_dp_vlan_filter_attr_get(hi_dp_vlan_filter_attr_s *pst_attr);
extern uint  hi_kernel_dp_vlan_filter_attr_set(hi_dp_vlan_filter_attr_s *pst_attr);

extern uint  hi_kernel_dp_vlan_filter_table_item_get(hi_dp_vlan_filter_table_item_s  *pst_item);
extern uint  hi_kernel_dp_vlan_filter_table_item_set(hi_dp_vlan_filter_table_item_s  *pst_item);

extern uint  hi_kernel_dp_smac_filter_table_item_get(hi_dp_smac_filter_table_item_s  *pst_item);
extern uint  hi_kernel_dp_smac_filter_table_item_set(hi_dp_smac_filter_table_item_s  *pst_item);

/* BEGIN: Added by shengjing, 2010/10/19   PN:SD5113V200新增*/
extern uint  hi_kernel_dp_ip_filter_table_item_get(hi_dp_ip_filter_table_item_s  *pst_item);
extern uint  hi_kernel_dp_ip_filter_table_item_set(hi_dp_ip_filter_table_item_s  *pst_item);
/* END:   Added by shengjing, 2010/10/19 */

/*兼容保留*/
extern uint  hi_kernel_dp_vlan_filter_cnt_get(hi_dp_vlan_filter_cnt_s  *pst_cnt);
extern void hi_kernel_dp_vlan_filter_cnt_clr( void );


/* BEGIN: Added by shengjing, 2010/10/19   PN:SD5113V200新增*/
extern uint  hi_kernel_dp_security_cnt_get(hi_dp_security_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_security_cnt_clr( void );
extern uint  hi_kernel_dp_security_sta_get(hi_dp_security_sta_s  *pst_sta);
/* END:   Added by shengjing, 2010/10/19 */

/* ********************************************************************************************* */
/* ********************************   FLOW 模块***************************************** */
extern uint  hi_kernel_dp_flow_attr_get(hi_dp_flow_attr_s  *pst_attr);
extern uint  hi_kernel_dp_flow_attr_set(hi_dp_flow_attr_s  *pst_attr);

extern uint  hi_kernel_dp_flow_efc_table_item_get(hi_dp_flow_efc_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_efc_table_item_set(hi_dp_flow_efc_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_efc_table_item_clr(hi_dp_flow_efc_table_item_s  *pst_item);

extern uint  hi_kernel_dp_flow_fc_table_item_get(hi_dp_flow_fc_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_fc_table_item_set(hi_dp_flow_fc_table_item_s  *pst_item);

extern uint  hi_kernel_dp_flow_sfc_table_item_get(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_sfc_table_item_set(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_sfc_table_item_clr(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_kernel_dp_flow_sfc_map_table_item_get(hi_dp_flow_sfc_table_item_s  *pst_item);
extern uint hi_kernel_dp_flow_sfc_map_table_item_set(hi_dp_flow_sfc_table_item_s * pst_item);
extern uint hi_kernel_dp_flow_sfc_qos_table_item_get(hi_dp_flow_sfc_table_item_s * pst_item);
extern uint hi_kernel_dp_flow_sfc_qos_table_item_set(hi_dp_flow_sfc_table_item_s * pst_item);
extern uint  hi_kernel_dp_flow_tcfl_rep_table_item_get(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_tcfl_rep_table_item_set(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);

/*隧道报文TF FL替换表*/
extern uint  hi_kernel_dp_flow_tunnel_tcfl_rep_table_item_get(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_tunnel_tcfl_rep_table_item_set(hi_dp_flow_tcfl_rep_table_item_s  *pst_item);

#ifdef CONFIG_HISI_SD5115_MP_MPW
extern uint  hi_kernel_dp_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint hi_kernel_dp_flow_cnt_table_item_clr(hi_dp_flow_cnt_table_item_s *pui_statis_id);
#endif

#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint  hi_kernel_dp_igr_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint  hi_kernel_dp_igr_flow_cnt_table_item_clr(hi_dp_flow_cnt_table_item_s *pui_statis_id);

extern uint  hi_kernel_dp_egr_flow_cnt_table_item_get(hi_dp_flow_cnt_table_item_s  *pst_item);
extern uint  hi_kernel_dp_egr_flow_cnt_table_item_clr(hi_dp_flow_cnt_table_item_s *pui_statis_id);

extern uint  hi_kernel_dp_flow_cnt_match_table_item_get(hi_dp_flow_cnt_match_table_item_s  *pst_item);
extern uint  hi_kernel_dp_flow_cnt_match_table_item_set(hi_dp_flow_cnt_match_table_item_s  *pst_item);
#endif

extern uint  hi_kernel_dp_flow_cnt_get(hi_dp_flow_cnt_s  *pst_cnt);
extern void  hi_kernel_dp_flow_cnt_clr(void);

extern uint  hi_kernel_dp_flow_rtp_req( void );


extern uint hi_kernel_dp_thru_reason_cnt_get(hi_dp_thru_reason_cnt_s  *pst_cnt);
extern uint hi_kernel_dp_tocpu_reason_cnt_get(hi_dp_tocpu_reason_cnt_s  *pst_cnt);
extern void hi_kernel_dp_thru_reason_cnt_clr( void );
extern void hi_kernel_dp_tocpu_reason_cnt_clr( void );


/* ********************************************************************************************* */
/* ********************************   SPECIAL PKT 模块************************************** */
extern uint  hi_kernel_dp_special_pkt_attr_get(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint  hi_kernel_dp_special_pkt_attr_set(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint hi_kernel_dp_special_pkt_cnt_get(hi_dp_special_pkt_cnt_s  *pst_cnt);
extern void hi_kernel_dp_special_pkt_cnt_clr( void );

extern uint  hi_kernel_dp_special_pkt_trans_table_item_set(hi_dp_special_pkt_trans_table_item_s *pst_item);
extern uint  hi_kernel_dp_special_pkt_trans_table_item_get(hi_dp_special_pkt_trans_table_item_s *pst_item);

/* ********************************************************************************************* */
/* ********************************   L2 模块*********************************************** */
extern uint  hi_kernel_dp_l2_attr_get(hi_dp_l2_attr_s    *pst_attr);
extern uint  hi_kernel_dp_l2_attr_set(hi_dp_l2_attr_s    *pst_attr);
extern uint  hi_kernel_dp_l2_act_table_item_get(hi_dp_l2_act_table_item_s    *pst_item);
extern uint  hi_kernel_dp_l2_act_table_item_set(hi_dp_l2_act_table_item_s    *pst_item);
extern uint  hi_kernel_dp_l2_act_table_item_clr(hi_dp_l2_act_table_item_s    *pst_item);

extern uint  hi_kernel_dp_l2_mac_table_item_get(hi_dp_l2_mac_table_item_s    *pst_item);
extern uint  hi_kernel_dp_l2_mac_add(hi_dp_l2_mac_s    *pst_mac);
extern uint  hi_kernel_dp_l2_mac_del(hi_dp_l2_mac_s    *pst_mac);
extern uint  hi_kernel_dp_l2_mac_get(hi_dp_l2_mac_s    *pst_mac);

extern uint  hi_kernel_dp_l2_mc_add(hi_dp_l2_mc_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_del(hi_dp_l2_mc_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_get(hi_dp_l2_mc_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_users_del(uint ui_mc_id);
extern uint  hi_kernel_dp_l2_mc_users_get(hi_dp_l2_mc_users_s    *pst_users);
extern uint  hi_kernel_dp_l2_mc_user_add(hi_dp_l2_mc_user_s  *pst_user);
extern uint  hi_kernel_dp_l2_mc_user_del(hi_dp_l2_mc_user_s  *pst_user);

extern uint  hi_kernel_dp_l2_ip_mc_table_item_get(hi_dp_l2_ip_mc_s    *pst_mc);
extern uint  hi_kernel_dp_l2_ip_mc_table_item_set(hi_dp_l2_ip_mc_s    *pst_mc);


extern uint  hi_kernel_dp_l2_cnt_get(hi_dp_l2_cnt_s  *pst_cnt);
#ifdef CONFIG_HISI_SD5115_MP_MPW
extern void hi_kernel_dp_l2_cnt_clr( void );
#else
extern uint  hi_kernel_dp_l2_cnt_clr(uint ui_port_id );
#endif

extern uint  hi_kernel_dp_l2_vlan_bc_add (hi_dp_l2_bc_egr_s *pst_egr);
extern uint  hi_kernel_dp_l2_vlan_bc_del (hi_dp_l2_bc_egr_s *pst_egr);
extern uint  hi_kernel_dp_l2_vlan_del (uint ui_vlan);
extern uint  hi_kernel_dp_l2_vlan_bc_get (hi_dp_l2_bc_egrs_s *pst_egrs);

extern uint  hi_kernel_dp_l2_mc_filter_add(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_filter_del(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_filter_get(hi_dp_l2_mc_filter_s    *pst_mc);
extern uint  hi_kernel_dp_l2_mc_filter_table_item_get(hi_dp_l2_mc_filter_s    *pst_item);
extern uint  hi_kernel_dp_l2_mac_table_reset(void );

/* BEGIN: Added by w00180499, 2012/7/20 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
extern uint hi_kernel_dp_l2_mc_st_attr_get(hi_dp_l2_mc_st_attr_s    *pst_attr);
extern uint hi_kernel_dp_l2_mc_st_attr_set(hi_dp_l2_mc_st_attr_s    *pst_attr);
#endif
/* END:   Added by w00180499, 2012/7/20 */

/* ********************************************************************************************* */
/* ********************************   L3 模块*********************************************** */
extern uint  hi_kernel_dp_l3_attr_get(hi_dp_l3_attr_s  *pst_attr);
extern uint  hi_kernel_dp_l3_attr_set(hi_dp_l3_attr_s  *pst_attr);

extern uint  hi_kernel_dp_l3_replace_table_item_get(hi_dp_l3_replace_table_item_s  *pst_item);
extern uint  hi_kernel_dp_l3_replace_table_item_set(hi_dp_l3_replace_table_item_s  *pst_item);
extern uint  hi_kernel_dp_l3_napt_table_item_get(hi_dp_l3_napt_table_item_s  *pst_item);

#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint  hi_kernel_dp_l3_alarm_attr_get(hi_dp_l3_alarm_attr_s *pst_item);
extern uint  hi_kernel_dp_l3_alarm_attr_set(hi_dp_l3_alarm_attr_s *pst_item);
#endif
extern uint  hi_kernel_dp_l3_napt_table_item_clr(uint ui_index);
extern uint  hi_kernel_dp_l3_napt_table_clr(void);

extern uint  hi_kernel_dp_l3_nh_table_item_get(hi_dp_l3_nh_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_nh_table_item_set(hi_dp_l3_nh_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_nh_table_item_clr(hi_dp_l3_nh_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_rt_table_item_get(hi_dp_l3_rt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_rt_table_item_set(hi_dp_l3_rt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_rt_table_item_clr(hi_dp_l3_rt_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_rt_act_table_item_get(hi_dp_l3_rt_act_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_rt_act_table_item_set(hi_dp_l3_rt_act_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_rt_act_table_item_clr(hi_dp_l3_rt_act_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_nh_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint  hi_kernel_dp_l3_rt_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint  hi_kernel_dp_l3_in_tunnel_rt_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint  hi_kernel_dp_l3_out_tunnel_match_table_sta_get(hi_dp_l3_sta_s *pst_sta);
extern uint  hi_kernel_dp_l3_rt_act_table_sta_get(hi_dp_l3_sta_s *pst_sta);

/* BEGIN: Add by G174254, 2011-8-11, 根据庞振意见修改整改隧道部分接口 */
extern uint  hi_kernel_dp_l3_in_tunnel_rt_table_item_get(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_in_tunnel_rt_table_item_set(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_out_tunnel_match_table_item_get(hi_dp_l3_tunnel_rt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_out_tunnel_match_table_item_set(hi_dp_l3_tunnel_rt_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_out_tunnel_sip_table_item_get(hi_dp_l3_tunnel_sip_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_out_tunnel_sip_table_item_set(hi_dp_l3_tunnel_sip_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_in_tunnel_head_table_item_get(hi_dp_l3_tunnel_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_in_tunnel_head_table_item_set(hi_dp_l3_tunnel_table_item_s *pst_item);

extern uint  hi_kernel_dp_l3_cnt_get(hi_dp_l3_cnt_s  *pst_cnt);
extern void  hi_kernel_dp_l3_cnt_clr( void );

extern uint  hi_kernel_dp_l3_napt_table_item_add(hi_dp_l3_napt_table_item_s  *pst_item);
extern uint  hi_kernel_dp_l3_napt_table_item_del(hi_dp_l3_napt_table_item_s  *pst_item);
extern uint  hi_kernel_dp_l3_napt_table_item_query(hi_dp_l3_napt_table_item_s  *pst_item);

extern uint  hi_kernel_dp_l3_in_6rd_tunnel_head_table_item_get(hi_dp_l3_6rd_tunnel_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_in_6rd_tunnel_head_table_item_set(hi_dp_l3_6rd_tunnel_table_item_s *pst_item);

#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint  hi_kernel_dp_l3_napt_point_cnt_table_item_get(hi_dp_l3_napt_point_cnt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_napt_point_cnt_table_item_set(hi_dp_l3_napt_point_cnt_table_item_s *pst_item);
extern uint  hi_kernel_dp_l3_napt_point_cnt_table_clr(void);

extern uint  hi_kernel_dp_l3_napt_point_cnt_get(hi_dp_l3_napt_point_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_l3_napt_point_cnt_clr(void);
#endif
extern uint hi_kernel_dp_l3_wan_cnt_get(hi_dp_wan_cnt_s  *pst_cnt);
extern void hi_kernel_dp_l3_wan_cnt_clr( uint ui_index );
/* BEGIN: Added by l00164498, 2012/8/6 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
extern uint hi_kernel_dp_l3_rsp_table_item_get(hi_dp_l3_rsp_table_item_s *pst_item);
extern uint hi_kernel_dp_l3_rsp_table_item_set(hi_dp_l3_rsp_table_item_s *pst_item);
#endif
/* END: Added by l00164498, 2012/8/6 */
/* ********************************************************************************************* */
/* ********************************   OAM 模块*********************************************** */
extern uint  hi_kernel_dp_oam_attr_get(hi_dp_oam_attr_s  *pst_attr);
extern uint  hi_kernel_dp_oam_attr_set(hi_dp_oam_attr_s  *pst_attr);
extern uint  hi_kernel_dp_oam_sta_get(hi_dp_oam_sta_s  *pst_sta);
extern uint  hi_kernel_dp_oam_cfm_table_item_set(hi_dp_oam_cfm_table_item_s  *pst_item);
extern uint  hi_kernel_dp_oam_cfm_table_item_get(hi_dp_oam_cfm_table_item_s  *pst_item);
extern uint  hi_kernel_dp_oam_cnt_get(hi_dp_oam_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_oam_cnt_clr(void);
extern uint  hi_kernel_dp_oam_alarm_attr_get(hi_dp_oam_alarm_attr_s *pst_item);
extern uint  hi_kernel_dp_oam_alarm_attr_set(hi_dp_oam_alarm_attr_s *pst_item);
#ifdef SUPPORT_OAM_1731
extern uint  hi_kernel_dp_oam_txfcl_inc(void);
extern uint hi_kernel_dp_oam_1731_attr_set(hi_dp_oam_1731_attr_s  *pst_attr);
extern uint hi_kernel_dp_oam_1731_attr_get(hi_dp_oam_1731_attr_s  *pst_attr);

#endif

/* BEGIN: Added by shengjing, 2011/12/16   新增prbs模块 */
/***********************************************************************************************/
/*********************************   prbs 模块***************************************************/
extern uint  hi_kernel_dp_prbs_attr_get(hi_dp_prbs_attr_s  *pst_attr);
extern uint  hi_kernel_dp_prbs_attr_set(hi_dp_prbs_attr_s  *pst_attr);
extern uint  hi_kernel_dp_prbs_sta_get(hi_dp_prbs_sta_s  *pst_sta);
extern uint  hi_kernel_dp_prbs_cnt_get(hi_dp_prbs_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_prbs_cnt_clr(uint ui_channel);
/* END:   Added by shengjing, 2011/12/16 */

/* ********************************************************************************************* */
/* ********************************   QOS  模块 ************************************************* */
extern uint  hi_kernel_dp_qos_scar_table_set(hi_dp_qos_scar_table_s  *pst_attr);
extern uint  hi_kernel_dp_qos_scar_table_get(hi_dp_qos_scar_table_s  *pst_attr);
extern uint  hi_kernel_dp_qos_ecar_table_item_set(hi_dp_qos_car_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_ecar_table_item_get(hi_dp_qos_car_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_tocpu_reason_table_item_set(hi_dp_qos_tocpu_reason_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_tocpu_reason_table_item_get(hi_dp_qos_tocpu_reason_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_tocpu_reason_car_table_item_set(hi_dp_qos_car_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_tocpu_reason_car_table_item_get(hi_dp_qos_car_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_gmap_table_item_set(hi_dp_qos_gmap_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gmap_table_item_get(hi_dp_qos_gmap_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fcar_table_item_set(hi_dp_qos_fcar_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_fcar_table_item_get(hi_dp_qos_fcar_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_gcar_table_item_set(hi_dp_qos_gcar_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_gcar_table_item_get(hi_dp_qos_gcar_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_qmap_table_item_set(hi_dp_qos_qmap_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_qmap_table_item_get(hi_dp_qos_qmap_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_queue_attr_set(hi_dp_qos_queue_attr_s  *pst_attr);
extern uint  hi_kernel_dp_qos_queue_attr_get(hi_dp_qos_queue_attr_s *pst_attr);
extern uint  hi_kernel_dp_qos_fsch_table_item_set(hi_dp_qos_fsch_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fsch_table_item_get(hi_dp_qos_fsch_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gsch_table_item_set(hi_dp_qos_gsch_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gsch_table_item_get(hi_dp_qos_gsch_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fwred_map_table_item_set(hi_dp_qos_fwred_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fwred_map_table_item_get(hi_dp_qos_fwred_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gwred_map_table_item_set(hi_dp_qos_gwred_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gwred_map_table_item_get(hi_dp_qos_gwred_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fwred_table_item_set(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_fwred_table_item_get(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_gwred_table_item_set(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_gwred_table_item_get(hi_dp_qos_wred_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_egr_shaping_table_item_set(hi_dp_qos_egr_shaping_table_item_s * pst_item);
extern uint  hi_kernel_dp_qos_egr_shaping_table_item_get(hi_dp_qos_egr_shaping_table_item_s * pst_item);
extern uint  hi_kernel_dp_qos_epfc_map_table_item_get(hi_dp_qos_epfc_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_epfc_map_table_item_set(hi_dp_qos_epfc_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fcar_map_table_item_set(hi_dp_qos_fcar_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_fcar_map_table_item_get(hi_dp_qos_fcar_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gcar_map_table_item_set(hi_dp_qos_gcar_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_gcar_map_table_item_get(hi_dp_qos_gcar_map_table_item_s *pst_item);
extern uint  hi_kernel_dp_qos_queue_shaping_table_item_set(hi_dp_qos_queue_shaping_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_queue_shaping_table_item_get(hi_dp_qos_queue_shaping_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_group_shaping_table_item_set(hi_dp_qos_gs_shaping_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_group_shaping_table_item_get(hi_dp_qos_gs_shaping_table_item_s  *pst_item);
extern uint  hi_kernel_dp_qos_attr_set(hi_dp_qos_attr_s  *pst_attr);
extern uint  hi_kernel_dp_qos_attr_get(hi_dp_qos_attr_s  *pst_attr);
extern uint  hi_kernel_dp_qos_queue_cnt_table_item_get(hi_dp_qos_queue_cnt_table_item_s  *pst_cnt);
extern uint  hi_kernel_dp_qos_queue_cnt_table_item_clr(uint ui_qid);
extern uint  hi_kernel_dp_qos_cnt_get(hi_dp_qos_cnt_s  *pst_cnt);
extern void  hi_kernel_dp_qos_cnt_clr(void);
extern uint  hi_kernel_dp_qos_sta_get(hi_dp_qos_sta_s *pst_sta);
extern uint  hi_kernel_dp_qos_debug(hi_dp_qos_debug_s *pst_sta);
#ifdef CONFIG_HISI_SD5115_MP_PILOT
extern uint  hi_kernel_dp_qos_attr_set(hi_dp_qos_attr_s  *pst_attr);
extern uint  hi_kernel_dp_qos_attr_get(hi_dp_qos_attr_s  *pst_attr);

extern uint  hi_kernel_dp_qos_qg_attr_get(hi_dp_qos_qg_attr_s *pst_attr);
extern uint  hi_kernel_dp_qos_qg_attr_set(hi_dp_qos_qg_attr_s *pst_attr);
extern uint  hi_kernel_dp_qos_eqg_attr_get(hi_dp_qos_eqg_attr_s *pst_attr);
extern uint  hi_kernel_dp_qos_eqg_attr_set(hi_dp_qos_eqg_attr_s *pst_attr);
extern uint  hi_kernel_dp_qos_tocpu_reason_pri_reset_set(hi_dp_qos_pri_reset_table_s  *pst_item);
extern uint  hi_kernel_dp_qos_tocpu_reason_pri_reset_get(hi_dp_qos_pri_reset_table_s  *pst_item);
extern uint  hi_kernel_dp_qos_drop_reason_get(hi_dp_qos_drop_reason_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_qos_drop_reason_clr(HI_DP_PORT_E em_port);
#ifdef CONFIG_HISI_SD5115_TYPE_T	
extern uint hi_kernel_dp_qos_queue_attr_5115t_set(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
extern uint hi_kernel_dp_qos_queue_attr_5115t_get(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
#endif   
#endif

/* *************************************************************************************** */
/* *********************************   CNT 模块******************************************* */
extern uint  hi_kernel_dp_cnt_am_get(hi_dp_logic_am_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_cap_get(hi_dp_logic_cap_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_cpurtx_get(hi_dp_logic_cpurtx_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_drop_reason_get(hi_dp_logic_drop_reason_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_efc_get(hi_dp_logic_efc_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_fc_get(hi_dp_logic_fc_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_glb_get(hi_dp_logic_glb_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_l2_get(hi_dp_logic_arl_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_l3_get(hi_dp_logic_napt_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_mtp_get(hi_dp_logic_mtp_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_sfc_get(hi_dp_logic_sfc_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_mcd_get(hi_dp_logic_mcd_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_oam_get(hi_dp_logic_oam_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_pa_get(hi_dp_logic_pa_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_qm_get(hi_dp_logic_qm_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_queue_get(hi_dp_logic_queue_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_rxdma_get(hi_dp_logic_rxdma_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_thru_reason_get(hi_dp_logic_thru_reason_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_tocpu_reason_get(hi_dp_logic_tocpu_reason_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_txdma_get(hi_dp_logic_txdma_cnt_s  *pst_cnt);


extern uint  hi_kernel_dp_cnt_ge_get(hi_dp_logic_ge_cnt_s  *pst_cnt);
extern uint  hi_kernel_dp_cnt_fe_get(hi_dp_logic_fe_cnt_s  *pst_cnt);

extern void  hi_kernel_dp_cnt_reason_clr(void);
extern void  hi_kernel_dp_cnt_special_pkt_clr(void);
extern void  hi_kernel_dp_cnt_queue_clr(void);
extern void  hi_kernel_dp_cnt_oam_clr(void);

/* BEGIN: Added by shengjing, 2010/12/1   PN:SD5113V200新增*/
extern uint  hi_kernel_dp_cnt_mcm_get(hi_dp_logic_mcm_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_rt_get(hi_dp_logic_rt_cnt_s *pst_cnt);

extern uint  hi_kernel_dp_cnt_ipv6_get(hi_dp_logic_ipv6_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_ipv4_get(hi_dp_logic_ipv4_cnt_s *pst_cnt);

extern uint  hi_kernel_dp_cnt_ipv6_clr(uint ui_index);
extern uint  hi_kernel_dp_cnt_ipv4_clr(uint ui_index);

/* BEGIN: Added by shengjing, 2010/12/30   问题单号:  */
/* 仅删除efc debug统计 */
extern uint  hi_kernel_dp_cnt_efc_debug_clr(void);

/* END:   Added by shengjing, 2010/12/1 */

extern uint  hi_kernel_dp_cnt_gpon_ext_get( hi_dp_logic_gpon_ext_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_dgtc_sys_get( hi_dp_logic_dgtc_sys_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_dgtc_line_get( hi_dp_logic_dgtc_line_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_dgem_sys_get( hi_dp_logic_dgem_sys_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_ugmac_sys_get( hi_dp_logic_ugmac_sys_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_ugmac_line_get( hi_dp_logic_ugmac_line_cnt_s *pst_cnt);
extern uint  hi_kernel_dp_cnt_eps_pon_get( hi_dp_logic_eps_pon_cnt_s *pst_cnt);

extern void hi_kernel_dp_cnt_gpon_clr( void );

extern uint  hi_kernel_dp_cnt_epon_get( hi_dp_logic_epon_cnt_s *pst_cnt );
extern void  hi_kernel_dp_cnt_epon_clr( void );

/*BEGIN：Added by zhouqingsong,2011/11/11*/
extern uint  hi_kernel_dp_cnt_pie_get(hi_dp_pie_rx_cnt_s * pst_cnt);
extern uint  hi_kernel_dp_cnt_pie_clr(void);
/*END：Added by zhouqingsong,2011/11/11*/


//#ifdef HI_SDK_DEBUG
extern uint  hi_kernel_dp_debug_set(hi_dp_debug_attr_s *pst_attr);
extern uint  hi_kernel_dp_debug_get(hi_dp_debug_attr_s *pst_attr);
//#endif

/* ********************************************************************************************* */

/* for debug */
extern uint  hi_kernel_dp_physic_addr_get(void *get_info);
extern uint  hi_kernel_dp_physic_addr_set(void *set_info);

//extern uint  hi_kernel_dp_smd_command(hi_dp_smd_command_s *pst_cmd);

#endif /* end of  */
