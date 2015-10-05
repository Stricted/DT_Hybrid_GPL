/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_dp.h
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

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_H__
#define  __HI_DP_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

#include "hi_drv_ioctl_dp.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define HI_DP_DEV_NAME  "/dev/hi_ioctl_dp"
#define HI_DEBUG_PRINT_POSITION()   printf("\n\r LINE = %d, FUNC = %s \n\r ", __LINE__, __func__)
#define HI_DEBUG(fmt,arg...)        if ( g_dp_debug_en ) (HI_DEBUG_PRINT_POSITION(),printf(fmt,##arg))

/***********************************************************************************************/                       
/*********************************设备打开 关闭*************************************************/

/*设备打开*/
extern uint hi_dp_dev_open(void);

/*设备关闭*/
extern void hi_dp_dev_close(void);



/***********************************************************************************************/                       
/*********************************访问权限获取**************************************************/
/*访问权限*/
extern uint hi_dp_right_get(void);
extern uint hi_dp_right_release(void);

/***********************************************************************************************/                       
/************************************************************************************************/
extern uint hi_dp_cmd_proc( uint cmd, void *data );
/***********************************************************************************************/                       
/*********************************GLOBAL CONFIG 模块********************************************/
extern uint hi_dp_glb_attr_get(hi_dp_glb_attr_s  *pst_attr);
extern uint hi_dp_glb_attr_set(hi_dp_glb_attr_s  *pst_attr);
extern uint hi_dp_glb_cnt_get(hi_dp_glb_cnt_s  *pst_cnt);
extern uint hi_dp_glb_sta_get(hi_dp_glb_sta_s  *pst_sta);
extern uint hi_dp_glb_cnt_clr(void);
extern uint hi_dp_glb_spec_get( hi_dp_glb_spec_s *pst_spec );

/***********************************************************************************************/
/*********************************   GPON 模块************************************************/
extern uint hi_dp_gpon_attr_get(hi_dp_gpon_attr_s  *pst_attr);
extern uint hi_dp_gpon_attr_set(hi_dp_gpon_attr_s  *pst_attr);
extern uint hi_dp_gpon_gemport_cnt_table_item_get(hi_dp_gpon_gemport_cnt_table_item_s *pst_item);
extern uint hi_dp_gpon_gemport_cnt_table_item_set(hi_dp_gpon_gemport_cnt_table_item_s *pst_item);
extern uint hi_dp_gpon_gemport_cnt_table_item_clr(uint   ui_index);
extern uint hi_dp_gpon_alarm_attr_get( hi_dp_gpon_alarm_attr_s* pst_attr );
extern uint hi_dp_gpon_alarm_attr_set( hi_dp_gpon_alarm_attr_s* pst_attr );

extern uint hi_dp_gpon_cnt_get( hi_dp_gpon_cnt_s* pst_cnt );
extern uint hi_dp_gpon_cnt_clr( void );
extern uint hi_dp_gpon_spcnt_get(hi_dp_gpon_spcnt_s *pst_cnt);
extern uint hi_dp_gpon_sta_get( hi_dp_gpon_sta_s *pst_sta );
extern uint  hi_dp_gpon_omci_reset(void);

/***********************************************************************************************/                       
/*********************************   GPON DN 模块************************************************/
extern uint hi_dp_gpon_dn_attr_get(hi_dp_gpon_dn_attr_s *pst_attr);
extern uint hi_dp_gpon_dn_attr_set(hi_dp_gpon_dn_attr_s *pst_attr);

extern uint hi_dp_gpon_dn_gemport_table_item_get(hi_dp_gpon_dn_gemport_table_item_s *pst_item);
extern uint hi_dp_gpon_dn_gemport_table_item_set(hi_dp_gpon_dn_gemport_table_item_s *pst_item);
extern uint hi_dp_gpon_dn_gemport_table_clr(void);

extern uint hi_dp_gpon_dn_omci_receive(hi_omci_message_s  *pst_omci);

extern uint hi_dp_gpon_dn_cnt_get(hi_dp_gpon_dn_cnt_s  *pst_cnt);
extern uint hi_dp_gpon_dn_cnt_clr( void );
extern uint hi_dp_gpon_dn_sta_get(hi_dp_gpon_dn_sta_s  *pst_sta);

extern uint hi_dp_gpon_key_switch(void);
/*参数数据类型由hi_dp_gpon_dn_bwmap_table_item_s改为hi_dp_gpon_dn_bwmap_table_s*/
extern uint hi_dp_gpon_dn_bwmap_table_item_get(hi_dp_gpon_dn_bwmap_table_s *pst_table);
extern uint hi_dp_gpon_dn_typeb_op(hi_dp_gpon_dn_typeb_op_s  *pst_op);

/*******************************************************************************************/                       
/*********************************   PON UP 模块********************************************/
extern uint hi_dp_gpon_up_attr_get(hi_dp_gpon_up_attr_s *pst_attr);
extern uint hi_dp_gpon_up_attr_set(hi_dp_gpon_up_attr_s *pst_attr);

extern uint hi_dp_gpon_up_map_table_item_get(hi_dp_gpon_up_map_table_item_s *pst_item);
extern uint hi_dp_gpon_up_map_table_item_set(hi_dp_gpon_up_map_table_item_s *pst_item);

extern uint hi_dp_gpon_up_tcont_table_item_get(hi_dp_gpon_up_tcont_table_item_s *pst_item);
extern uint hi_dp_gpon_up_tcont_table_item_set(hi_dp_gpon_up_tcont_table_item_s *pst_item);

extern uint hi_dp_gpon_up_omci_send(hi_omci_message_s  *pst_omci);
extern uint hi_dp_gpon_up_ploam_send(hi_ploam_message_s  *pst_ploam);

extern uint hi_dp_gpon_up_clean_tx_ploam(void);

extern uint hi_dp_gpon_up_cnt_get(hi_dp_gpon_up_cnt_s  *pst_cnt);
extern uint hi_dp_gpon_up_cnt_clr( void );

extern uint hi_dp_gpon_up_sta_get( hi_dp_gpon_up_sta_s *pst_sta );


/***********************************************************************************************/                       
/*********************************   EPON 公共 **************************************/
extern uint hi_dp_epon_attr_get( hi_dp_epon_attr_s *pst_attr );
extern uint hi_dp_epon_attr_set( hi_dp_epon_attr_s *pst_attr );

extern uint hi_dp_epon_alarm_attr_get( hi_dp_epon_alarm_attr_s* pst_attr );
extern uint hi_dp_epon_alarm_attr_set( hi_dp_epon_alarm_attr_s* pst_attr );

extern uint hi_dp_epon_cnt_get( hi_dp_epon_cnt_s* pst_cnt );
extern uint hi_dp_epon_cnt_clr(void);
extern uint hi_dp_epon_llid_bandwidth_get( hi_dp_epon_llid_bandwidth_s *pst_attr );
extern uint hi_dp_epon_llid_cnt_get( hi_dp_epon_llid_cnt_s* pst_cnt);  
extern uint hi_dp_epon_llid_cnt_clr( uint ui_llid );

/*********************************   EPON 下行 模块**************************************/
extern uint hi_dp_epon_dn_attr_get( hi_dp_epon_dn_attr_s* pst_attr );
extern uint hi_dp_epon_dn_attr_set( hi_dp_epon_dn_attr_s* pst_attr );
extern uint hi_dp_epon_dn_llid_table_item_get( hi_dp_epon_dn_llid_table_item_s *pst_item );
extern uint hi_dp_epon_dn_llid_table_item_set( hi_dp_epon_dn_llid_table_item_s *pst_item );
extern uint hi_dp_epon_dn_sta_get( hi_dp_epon_dn_sta_s *pst_sta );
extern uint hi_dp_epon_dn_cnt_get( hi_dp_epon_dn_cnt_s *pst_cnt );
extern uint hi_dp_epon_dn_cnt_clr( void );

/****************************************************************************************/
/*********************************   EPON 上行 模块**************************************/
extern uint hi_dp_epon_up_attr_get( hi_dp_epon_up_attr_s* pst_attr );
extern uint hi_dp_epon_up_attr_set( hi_dp_epon_up_attr_s* pst_attr );
extern uint hi_dp_epon_up_map_table_item_get( hi_dp_epon_up_map_table_item_s *pst_item );
extern uint hi_dp_epon_up_map_table_item_set( hi_dp_epon_up_map_table_item_s *pst_item );
extern uint hi_dp_epon_up_report_table_item_get( hi_dp_epon_up_report_table_item_s *pst_item  );
extern uint hi_dp_epon_up_report_table_item_set( hi_dp_epon_up_report_table_item_s *pst_item  );
extern uint hi_dp_epon_up_mpcp_send( hi_mpcp_message_s *pst_mpcp );
extern uint hi_dp_epon_up_oam_send( hi_oam_message_s *pst_oam );
extern uint hi_dp_epon_up_sta_get( hi_dp_epon_up_sta_s *pst_sta );
extern uint hi_dp_epon_up_cnt_get( hi_dp_epon_up_cnt_s *pst_cnt );
extern uint hi_dp_epon_up_cnt_clr( void );

/*************************************************************************************/ 
/*********************************   NNI PORT 模块********************************************/
extern uint hi_dp_nni_eth_port_attr_set(hi_dp_nni_eth_port_attr_s  *pst_attr);
extern uint hi_dp_nni_eth_port_attr_get(hi_dp_nni_eth_port_attr_s  *pst_attr);
extern uint hi_dp_nni_eth_port_cnt_get(hi_dp_nni_eth_port_cnt_s  *pst_cnt);
extern uint hi_dp_nni_eth_port_cnt_clr(void);
extern uint hi_dp_nni_eth_port_reset(void);
extern uint hi_dp_nni_tag_sft_table_item_get(hi_dp_tag_sft_item_s *pst_item);
extern uint hi_dp_nni_tag_sft_table_item_set(hi_dp_tag_sft_item_s *pst_item);
extern uint hi_dp_nni_hybrid_table_item_get(hi_dp_nni_hybrid_item_s *pst_item);
extern uint hi_dp_nni_hybrid_table_item_set(hi_dp_nni_hybrid_item_s *pst_item);

/***********************************************************************************************/                       
/*********************************   UNI PORT 模块********************************************/
extern uint hi_dp_uni_attr_get(hi_dp_uni_attr_s *pst_attr);
extern uint hi_dp_uni_attr_set(hi_dp_uni_attr_s *pst_attr);

extern uint hi_dp_uni_cpu_port_attr_get(hi_dp_uni_cpu_port_attr_s  *pst_attr);
extern uint hi_dp_uni_cpu_port_attr_set(hi_dp_uni_cpu_port_attr_s  *pst_attr);
extern uint hi_dp_uni_cpu_port_cnt_get(hi_dp_uni_cpu_port_cnt_s  *pst_cnt);
extern uint hi_dp_uni_cpu_port_cnt_clr(void);

extern uint hi_dp_uni_eth_port_attr_get(hi_dp_uni_eth_port_attr_s  *pst_attr);
extern uint hi_dp_uni_eth_port_attr_set(hi_dp_uni_eth_port_attr_s  *pst_attr);
extern uint hi_dp_uni_eth_port_cnt_get(hi_dp_uni_eth_port_cnt_s  *pst_cnt);
extern uint hi_dp_uni_eth_port_cnt_clr(uint ui_port_index);
extern uint hi_dp_uni_eth_port_sta_get(hi_dp_uni_eth_port_sta_s  *pst_sta);
extern uint hi_dp_uni_eth_port_reset(HI_DP_PORT_E  em_port);


extern uint hi_dp_uni_tag_sft_table_item_set(hi_dp_tag_sft_item_s *pst_item);
extern uint hi_dp_uni_tag_sft_table_item_get(hi_dp_tag_sft_item_s *pst_item);

extern uint hi_dp_uni_hybrid_table_item_set(hi_dp_uni_hybrid_item_s *pst_item);
extern uint hi_dp_uni_hybrid_table_item_get(hi_dp_uni_hybrid_item_s *pst_item);

extern uint hi_dp_uni_cascade_attr_set(hi_dp_uni_cascade_attr_s *pst_attr);
extern uint hi_dp_uni_cascade_attr_get(hi_dp_uni_cascade_attr_s *pst_attr);

extern uint hi_dp_inner_phy_reg_write(hi_inner_phy_reg_info_s *pst_phy_info);
extern uint hi_dp_inner_phy_reg_read(hi_inner_phy_reg_info_s * pst_phy_info);
extern uint hi_dp_inner_phy_exreg_write(hi_inner_phy_reg_info_s *pst_phy_info);
extern uint hi_dp_inner_phy_exreg_read(hi_inner_phy_reg_info_s * pst_phy_info);

/***********************************************************************************************/                       
/*********************************  SECURITY  模块*******************************************/

#include "hi_dp_security.h"



/***********************************************************************************************/                       
/*********************************   FLOW 模块**************************************************/

#include "hi_dp_flow.h"
/***********************************************************************************************/                       
/*********************************   SPECIAL PKT 模块*******************************************/

#include "hi_dp_special.h"

/***********************************************************************************************/                       
/*********************************   L2 模块****************************************************/
#include "hi_dp_l2.h"


/***********************************************************************************************/                       
/*********************************   L3 模块****************************************************/
#include "hi_dp_l3.h"

/***********************************************************************************************/                       
/*********************************   OAM 模块***************************************************/
#include "hi_dp_oam.h"

/***********************************************************************************************/                       
/*********************************   PRBS 模块***************************************************/
extern uint hi_dp_prbs_attr_get(hi_dp_prbs_attr_s  *pst_attr);
extern uint hi_dp_prbs_attr_set(hi_dp_prbs_attr_s  *pst_attr);
extern uint hi_dp_prbs_sta_get(hi_dp_prbs_sta_s  *pst_sta);
extern uint hi_dp_prbs_cnt_get(hi_dp_prbs_cnt_s  *pst_cnt);
extern uint hi_dp_prbs_cnt_clr(uint ui_channel);



/***********************************************************************************************/          
/*********************************   QOS  模块 *************************************************/
#include "hi_dp_qos.h"

/*****************************************************************************************/                         
/*********************************   CNT 模块********************************************/
extern uint hi_dp_cnt_am_get(hi_dp_logic_am_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_cap_get(hi_dp_logic_cap_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_cpurtx_get(hi_dp_logic_cpurtx_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_drop_reason_get(hi_dp_logic_drop_reason_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_efc_get(hi_dp_logic_efc_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_fc_get(hi_dp_logic_fc_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_glb_get(hi_dp_logic_glb_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_l2_get(hi_dp_logic_arl_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_l3_get(hi_dp_logic_napt_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_mtp_get(hi_dp_logic_mtp_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_sfc_get(hi_dp_logic_sfc_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_mcd_get(hi_dp_logic_mcd_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_oam_get(hi_dp_logic_oam_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_pa_get(hi_dp_logic_pa_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_qm_get(hi_dp_logic_qm_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_queue_get(hi_dp_logic_queue_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_rxdma_get(hi_dp_logic_rxdma_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_thru_reason_get(hi_dp_logic_thru_reason_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_tocpu_reason_get(hi_dp_logic_tocpu_reason_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_txdma_get(hi_dp_logic_txdma_cnt_s  *pst_cnt);

extern uint hi_dp_cnt_gpon_ext_get( hi_dp_logic_gpon_ext_cnt_s *pst_cnt);
extern uint hi_dp_cnt_dgtc_sys_get( hi_dp_logic_dgtc_sys_cnt_s *pst_cnt);
extern uint hi_dp_cnt_dgtc_line_get( hi_dp_logic_dgtc_line_cnt_s *pst_cnt);
extern uint hi_dp_cnt_dgem_sys_get( hi_dp_logic_dgem_sys_cnt_s *pst_cnt);
extern uint hi_dp_cnt_ugmac_sys_get( hi_dp_logic_ugmac_sys_cnt_s *pst_cnt);
extern uint hi_dp_cnt_ugmac_line_get( hi_dp_logic_ugmac_line_cnt_s *pst_cnt);
extern uint hi_dp_cnt_eps_pon_get( hi_dp_logic_eps_pon_cnt_s *pst_cnt);

extern uint hi_dp_cnt_epon_get( hi_dp_logic_epon_cnt_s *pst_cnt );
extern uint hi_dp_cnt_epon_clr( void );

extern uint hi_dp_cnt_ge_get(hi_dp_logic_ge_cnt_s  *pst_cnt);
extern uint hi_dp_cnt_fe_get(hi_dp_logic_fe_cnt_s  *pst_cnt);

extern uint hi_dp_cnt_mcm_get(hi_dp_logic_mcm_cnt_s *pst_cnt);
extern uint hi_dp_cnt_rt_get(hi_dp_logic_rt_cnt_s *pst_cnt);

extern uint hi_dp_cnt_ipv6_get(hi_dp_logic_ipv6_cnt_s *pst_cnt);
extern uint hi_dp_cnt_ipv4_get(hi_dp_logic_ipv4_cnt_s *pst_cnt);

extern uint hi_dp_cnt_ipv6_clr(uint    ui_index);
extern uint hi_dp_cnt_ipv4_clr(uint    ui_index);


extern uint hi_dp_cnt_efc_debug_clr(void);

extern uint hi_dp_cnt_pie_get(hi_dp_pie_rx_cnt_s  *pst_cnt);
extern uint hi_dp_pie_cnt_clr(void);


extern uint hi_dp_cnt_special_pkt_clr(void);
extern uint hi_dp_cnt_oam_clr(void);
extern uint hi_dp_cnt_reason_clr(void);
extern uint hi_dp_cnt_queue_clr(void);

//#ifdef HI_SDK_DEBUG
extern uint hi_dp_debug_set(hi_dp_debug_attr_s *pst_attr);
extern uint hi_dp_debug_get(hi_dp_debug_attr_s *pst_attr);
//#endif
#ifdef CONFIG_HISI_SD5115_TYPE_T	
extern uint hi_dp_qos_queue_attr_5115t_set(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
extern uint hi_dp_qos_queue_attr_5115t_get(hi_dp_qos_queue_attr_5115t_s  *pst_attr);
#endif

                        
/*********************************   DEBUG *******************************************/

/*for debug*/
extern uint  hi_dp_physical_addr_get(void *p_info);
extern uint  hi_dp_physical_addr_set(void *p_info);
extern uint  hi_dp_smd_command(hi_dp_smd_command_s *pst_cmd);
                  
#endif /* end of __HI_DP_H__*/

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of __cplusplus */
