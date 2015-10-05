/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_adapter_api.h
  版 本 号   : 初稿
  作    者   : t00185981
  生成日期   : D2012_02_18

******************************************************************************/
#ifndef __HI_ADAPTER_API_H__
#define __HI_ADAPTER_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* CLI REG */
extern hi_uint32 hi_adapter_cli_init( hi_void );
extern hi_uint32 hi_adapter_cli_exit(hi_void);

/* CHIP */
/*****************************************************************************
 函 数 名  : hi_chip_get_version
 功能描述  : 获取芯片器件ID
 输入参数  : 
 输出参数  : hi_uint32 *pui_version  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_version(hi_uint32 *pui_version);
/*****************************************************************************
 函 数 名  : hi_chip_get_vendor
 功能描述  : 获取厂商ID
 输入参数  :  
 输出参数  : hi_ushort16 *pus_version 
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_vendor(hi_ushort16 *pus_vendor);

/*****************************************************************************
 函 数 名  : hi_chip_get_chip_version
 功能描述  : 获取芯片版本，vendor_id及固件版本信息
 输入参数  : hi_uint32 *pui_version           
             hi_uint32 *pui_vendor_id         
             hi_uint32 *pui_firmware_version  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_chip_version(hi_uint32 *pui_version, hi_uint32 *pui_vendor_id, hi_uint32 *pui_firmware_version);


/*****************************************************************************
 函 数 名  : hi_chip_get_upmode
 功能描述  : 获取上行模式
 输入参数  : 无
 输出参数  : hi_chip_up_mode_e *pem_mode
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_upmode(hi_chip_up_mode_e *pem_mode);
/*****************************************************************************
 函 数 名  : hi_chip_set_upmode
 功能描述  : 设置上行模式
 输入参数  : hi_chip_up_mode_e em_mode
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_upmode(hi_chip_up_mode_e em_mode);
/*****************************************************************************
 函 数 名  : hi_chip_get_access_mode
 功能描述  : 获取芯片工作模式
 输入参数  : 无
 输出参数  : hi_lsw_glb_sw_mode_e *pem_swmode
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_access_mode(hi_lsw_glb_sw_mode_e *pem_swmode);
/*****************************************************************************
 函 数 名  : hi_chip_set_access_mode
 功能描述  : 设置芯片工作模式
 输入参数  : hi_lsw_glb_sw_mode_e em_swmode
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_access_mode(hi_lsw_glb_sw_mode_e em_swmode);
/*****************************************************************************
 函 数 名  : hi_chip_get_port_cascade
 功能描述  : 获取级联模式
 输入参数  : 无
 输出参数  : hi_uchar8 *puc_enable 
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade(hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_chip_set_port_cascade
 功能描述  : 设置级联使能
 输入参数  : hi_uchar8 uc_enable
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade(hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_chip_get_port_cascade_doubletag
 功能描述  : 双层tag级联
 输入参数  : 无
 输出参数  : hi_uchar8 *puc_enable
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade_doubletag(hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_chip_set_port_cascade_doubletag
 功能描述  : 双层tag级联
 输入参数  : hi_uchar8 uc_enable
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_doubletag(hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_chip_get_port_cascade_tpid
 功能描述  : 
 输入参数  : 无
 输出参数  : hi_ushort16 *pus_tpid
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade_tpid(hi_ushort16 *pus_tpid);
/*****************************************************************************
 函 数 名  : hi_chip_set_port_cascade_tpid
 功能描述  : 
 输入参数  : hi_ushort16 us_tpid
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_tpid(hi_ushort16 us_tpid);
/*****************************************************************************
 函 数 名  : hi_chip_get_port_cascade_map
 功能描述  : 级联映射关系，默认使能
 输入参数  : 
 输出参数  : 
 返 回 值  : extern hi_uint32
*****************************************************************************/
//端口index和芯片的端口的对应关系?
extern hi_uint32 hi_chip_get_port_cascade_map(hi_uchar8 uc_portindex, hi_ushort16 *pus_vlan, hi_uchar8 *puc_cfi, hi_uchar8 *puc_pri);

/*****************************************************************************
 函 数 名  : hi_chip_set_port_cascade_map
 功能描述  : 级联映射关系，默认使能
 输入参数  : 
 输出参数  : 
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_map(hi_uchar8 uc_portindex, hi_ushort16 us_vlan, hi_uchar8 uc_cfi, hi_uchar8 uc_pri);

/*****************************************************************************
 函 数 名  : hi_chip_set_glb_vlan_mode
 功能描述  : 设置全局的vlan模式选择
 输入参数  : hi_lsw_module_e em_module        
             hi_lsw_direction_e em_direction  
             hi_lsw_vlan_mode_e em_vlan       
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
//extern hi_uint32 hi_chip_set_glb_vlan_mode(hi_lsw_module_e em_module, hi_lsw_direction_e em_direction, hi_lsw_vlan_mode_e em_vlanmode);
//extern hi_uint32 hi_chip_get_glb_vlan_mode(hi_lsw_module_e em_module, hi_lsw_direction_e em_direction, hi_lsw_vlan_mode_e *pem_vlanmode);
/*****************************************************************************
 函 数 名  : hi_chip_set_glb_pri_mode
 功能描述  : 设置全局的PRI模式选择
 输入参数  : hi_lsw_direction_e em_direction  
             hi_lsw_pri_mode_e em_pri         
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
//extern hi_uint32 hi_chip_set_glb_pri_mode(hi_lsw_direction_e em_direction, hi_lsw_pri_mode_e em_primode);
//extern hi_uint32 hi_chip_get_glb_pri_mode(hi_lsw_direction_e em_direction, hi_lsw_pri_mode_e *pem_primode);
extern hi_uint32 hi_chip_init_vlan_pri_select();

/* EMAC */
/*****************************************************************************
 函 数 名  : hi_emac_get_fec
 功能描述  : 获取epon fec状态
 输入参数  : 无
 输出参数  : hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_emac_get_fec(hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_emac_set_fec
 功能描述  : 设置epon fec状态
 输入参数  :hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_emac_set_fec(hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 uc_enable);

/* GMAC */
/*****************************************************************************
 函 数 名  : hi_gmac_get_aes
 功能描述  : aes使能/去使能状态获取
 输入参数  : 
 输出参数  : hi_ushort16 us_gemport  
             hi_uchar8 *puc_enable   
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_aes(hi_ushort16 us_gemport, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_gmac_set_aes
 功能描述  : aes使能/去使能设置
 输入参数  : hi_ushort16 us_gemport  
             hi_uchar8 uc_enable     
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_aes(hi_ushort16 us_gemport, hi_uchar8 uc_enable);
//密钥配置和切换接口在ploam里是否有提供?
//extern hi_uint32 hi_gmac_set_aes_exchange(hi_uchar8 uc_enable);
//extern hi_uint32 hi_gmac_set_aes_exchange(hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_gmac_get_fec
 功能描述  : fec状态获取
 输入参数  :           
 输出参数  : hi_lsw_direction_e em_direction  
             hi_uchar8 *puc_enable  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_fec(hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_gmac_set_fec
 功能描述  : FEC设置
 输入参数  : hi_lsw_direction_e em_direction  
             hi_uchar8 uc_enable              
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
//是否需要软件配置?
extern hi_uint32 hi_gmac_set_fec(hi_lsw_direction_e em_direction, hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_gmac_get_gemport
 功能描述  : GPON模式下该流的上行TCONT、GEMPORT获取
 输入参数  :  
 输出参数  : hi_ushort16 us_fid        
             hi_uchar8 *puc_tcontid    
             hi_ushort16 *pus_gemport 
 返 回 值  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_gmac_get_gemport(hi_ushort16 us_sfid, hi_uchar8 *puc_tcontid, hi_ushort16 *pus_gemport);
/*****************************************************************************
 函 数 名  : hi_gmac_set_gemport
 功能描述  : GPON模式下该流的上行TCONT、GEMPORT设置
 输入参数  : hi_ushort16 us_fid      
             hi_uchar8 uc_tcontid    
             hi_ushort16 us_gemport  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_gemport(hi_ushort16 us_sfid, hi_uchar8 uc_tcontid, hi_ushort16 us_gemport); 

extern  hi_uint32 hi_gmac_del_gemport(hi_ushort16 us_sfid);
/*****************************************************************************
 函 数 名  : hi_gmac_set_down_gemport_type
 功能描述  : 设置GPON下行GEMPORT的承载类型
 输入参数  : hi_ushort16 us_gemport              
             hi_lsw_gpon_dn_port_type_e em_type  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_down_gemport_type(hi_ushort16 us_gemport, hi_lsw_gpon_dn_port_type_e em_type);
/*****************************************************************************
 函 数 名  : hi_gmac_get_down_gemport_type
 功能描述  : 获取GPON下行GEMPORT的承载类型
 输入参数  : 
 输出参数  : hi_ushort16 us_gemport                
             hi_lsw_gpon_dn_port_type_e *pem_type  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_down_gemport_type(hi_ushort16 us_gemport, hi_lsw_gpon_dn_port_type_e *pem_type);
/*****************************************************************************
 函 数 名  : hi_gmac_del_down_gemport
 功能描述  : 下行删除此GEMPORT
 输入参数  : hi_ushort16 us_gemport  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_del_down_gemport(hi_ushort16 us_gemport);

/* FLOW */
/*****************************************************************************
 函 数 名  : hi_lsw_flow_add_service
 功能描述  : 添加业务切换流
 输入参数  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
             hi_ushort16 *pus_fid               
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_add_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact,
                                         hi_lsw_trans_act_e em_fwd_act, hi_ushort16 *pus_fid);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_get_service
 功能描述  : 根据业务切换流配置查找FID
 输入参数  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
             hi_ushort16 *pus_fid               
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_get_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact, 
                                         hi_lsw_trans_act_e em_fwd_act, hi_ushort16 *pus_fid);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_del_service
 功能描述  : 删除业务切换流
 输入参数  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact,
                                         hi_lsw_trans_act_e em_fwd_act);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_del_fid
 功能描述  : 根据FID删除业务切换流
 输入参数  : hi_ushort16 us_fid  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_fid(hi_ushort16 us_fid);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_add_mapping
 功能描述  : 添加映射流
 输入参数  : hi_lsw_flow_mapping_s *pst_mapinfo  
             hi_ushort16 *pus_sfid               
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_add_mapping(hi_lsw_flow_mapping_s *pst_mapinfo, hi_lsw_direction_e em_direction, hi_ushort16 *pus_sfid);

extern hi_uint32 hi_lsw_flow_del_mapping_by_sfid(hi_ushort16 us_sfid);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_del_mapping
 功能描述  : 删除映射流
 输入参数  : hi_lsw_flow_mapping_s *pst_mapinfo  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_mapping(hi_lsw_flow_mapping_s *pst_mapinfo, hi_lsw_direction_e em_direction);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_del_all
 功能描述  : 删除全部EFC和SFC表项
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_all();

/*****************************************************************************
 函 数 名  : hi_lsw_flow_change_igmp_pri
 功能描述  : 添加业务流更改组播报文的优先级
 输入参数  : hi_uchar8 uc_pri  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_change_igmp_pri(hi_uchar8 uc_pri);

/*****************************************************************************
 函 数 名  : hi_lsw_flow_diagnose
 功能描述  : 打印芯片的流配置
 输入参数  : hi_lsw_diag_flow_type_e em_type  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_diagnose(hi_lsw_diag_flow_type_e em_type);

extern hi_uint32 hi_lsw_flow_get_sfid(hi_ushort16 us_gemport, hi_ushort16 *pus_sfid);

/* CAR */
extern hi_uint32 hi_lsw_car_init_wred(hi_void);

extern hi_uint32 hi_lsw_car_set_protocol_paket_car(hi_lsw_special_pkt_e em_pkt, hi_lsw_port_e em_port, hi_uchar8 uc_enable);
extern hi_uint32 hi_lsw_car_get_protocol_paket_car(hi_lsw_special_pkt_e em_pkt, hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
extern hi_uint32 hi_lsw_car_set_protocol_paket_action(hi_lsw_special_pkt_e em_pkt,hi_lsw_port_e em_port, hi_lsw_trans_act_e em_act);
extern hi_uint32 hi_lsw_car_get_protocol_paket_action(hi_lsw_special_pkt_e em_pkt,hi_lsw_port_e em_port, hi_lsw_trans_act_e *pem_act);
extern hi_uint32 hi_lsw_car_set_simple_car(hi_lsw_simple_car_type_e em_type, hi_uchar8 uc_enable, hi_uint32 ui_cir, hi_uint32 ui_cbs);
extern hi_uint32 hi_lsw_car_get_simple_car(hi_lsw_simple_car_type_e em_type, hi_uchar8 *puc_enable, hi_uint32 *pui_cir, hi_uint32 *pui_cbs);
extern hi_uint32 hi_lsw_car_set_efc_car(hi_ushort16 us_carindex, hi_uchar8 uc_enable, hi_uint32 ui_cir, hi_uint32 ui_cbs);
extern hi_uint32 hi_lsw_car_get_efc_car(hi_ushort16 us_carindex, hi_uchar8 *puc_enable, hi_uint32 *pui_cir, hi_uint32 *pui_cbs);
extern hi_uint32 hi_lsw_car_set_flow_car(hi_ushort16 us_sfid, hi_lsw_flow_car_attr_s st_car);
extern hi_uint32 hi_lsw_car_get_flow_car(hi_ushort16 us_sfid, hi_lsw_flow_car_attr_s *pst_car);
extern hi_uint32 hi_lsw_car_get_tocpu_reason_car(hi_lsw_tocpu_reason_e em_tocpu_reason, hi_uchar8 *puc_enable, hi_uint32 *pui_cir, hi_uint32 *pui_cbs);
extern hi_uint32 hi_lsw_car_set_tocpu_reason_car(hi_lsw_tocpu_reason_e em_tocpu_reason, hi_uchar8 uc_enable, hi_uint32 ui_cir, hi_uint32 ui_cbs);
extern hi_uint32 hi_lsw_car_set_eth_car(hi_lsw_port_e em_port, hi_lsw_direction_e em_direction, hi_lsw_car_info_s *pst_carinfo);
extern hi_uint32 hi_lsw_car_set_gemport_car(hi_ushort16 us_gemport, hi_lsw_direction_e em_direction, hi_lsw_flow_car_attr_s st_attr);
extern hi_uint32 hi_lsw_car_get_gemport_car(hi_ushort16 us_gemport, hi_lsw_direction_e em_direction, hi_lsw_flow_car_attr_s *pst_attr);

/* QOS */
extern hi_uint32 hi_lsw_qos_get_queue_map(hi_lsw_qos_egr_e em_port, hi_uchar8 uc_pri, hi_uchar8 *puc_qid);
extern hi_uint32 hi_lsw_qos_set_queue_map(hi_lsw_qos_egr_e em_port, hi_uchar8 uc_pri, hi_uchar8 uc_qid);
extern hi_uint32 hi_lsw_qos_get_queue_buffer(hi_uchar8 uc_qid, hi_uint32 *pui_min, hi_uint32 *pui_max);
extern hi_uint32 hi_lsw_qos_set_queue_buffer(hi_uchar8 uc_qid, hi_uint32 ui_min, hi_uint32 ui_max);

extern hi_uint32 hi_lsw_qos_get_egr_schattr(hi_lsw_qos_egr_e em_port, hi_uchar8 uc_pri,
                            hi_lsw_qos_sch_mode_e *pem_schmode, hi_uchar8 *puc_wrr);
extern hi_uint32 hi_lsw_qos_set_egr_schattr(hi_lsw_qos_egr_e em_port, hi_uchar8 uc_pri,
                            hi_lsw_qos_sch_mode_e em_schmode,  hi_uchar8 uc_wrr);
extern hi_uint32 hi_lsw_qos_set_wred(hi_lsw_qos_wred_table_item_s st_wredattr);
extern hi_uint32 hi_lsw_qos_get_wred(hi_lsw_qos_wred_table_item_s *pst_wredattr);
/*速率为0，去使能shaping，CPU固定为pps，其他为bps*/
extern hi_uint32 hi_lsw_qos_set_egr_shaping(hi_lsw_qos_egr_e em_egr, hi_uint32 ui_rate);
extern hi_uint32 hi_lsw_qos_get_egr_shaping(hi_lsw_qos_egr_e em_egr, hi_uint32 *pui_rate);
extern hi_uint32 hi_lsw_qos_set_queue_shaping(hi_uchar8 uc_qid, hi_lsw_qos_shp_type_e em_type, hi_uint32 ui_cir, hi_uint32 ui_cbs);
extern hi_uint32 hi_lsw_qos_get_queue_shaping(hi_uchar8 uc_qid, hi_lsw_qos_shp_type_e *pem_type, hi_uint32 *pui_cir, hi_uint32 *pui_cbs);
extern hi_uint32 hi_lsw_qos_get_queuecount(hi_uchar8 *puc_count);
extern hi_uint32 hi_lsw_qos_set_in_pri(hi_uint32 ui_sfid, hi_uint32 ui_point_pri);
extern hi_uint32 hi_lsw_qos_get_in_pri(hi_uint32 ui_sfid, hi_uint32 *pui_point_pri);
extern hi_uint32 hi_lsw_qos_set_gemport_pri(hi_ushort16 us_gemport, hi_lsw_direction_e em_direction, hi_uint32 ui_pri);
extern hi_uint32 hi_lsw_qos_get_gemport_pri(hi_ushort16 us_gemport, hi_lsw_direction_e em_direction, hi_uint32 *pui_pri);

/* SEC */
extern hi_uint32 hi_lsw_sec_get_blackmode(hi_lsw_port_e em_port, hi_lsw_port_blackmode_e *pem_mode);
extern hi_uint32 hi_lsw_sec_set_blackmode(hi_lsw_port_e em_port, hi_lsw_port_blackmode_e em_mode);
extern hi_uint32 hi_lsw_sec_add_blackmac(hi_lsw_port_e em_port, hi_uchar8 *puc_mac);
extern hi_uint32 hi_lsw_sec_del_blackmac(hi_lsw_port_e em_port, hi_uchar8 *puc_mac);
extern hi_uint32 hi_lsw_sec_set_ip_filtermode(hi_lsw_port_type_e em_porttype, hi_lsw_trans_act_e em_act);
extern hi_uint32 hi_lsw_sec_get_ip_filtermode(hi_lsw_port_type_e em_porttype, hi_lsw_trans_act_e *pem_act);
extern hi_uint32 hi_lsw_sec_add_ip_filter_table(hi_lsw_sec_ip_filter_s *pst_ip_filter_table);
extern hi_uint32 hi_lsw_sec_del_ip_filter_table(hi_lsw_sec_ip_filter_s *pst_ip_filter_table);
extern hi_uint32 hi_lsw_sec_get_ip_filter_table(hi_uint32 ui_index, hi_lsw_sec_ip_filter_s *pst_ip_filter_table);
extern hi_uint32 hi_lsw_sec_diagnose(hi_lsw_diag_sec_type_e em_type);

/* VLAN */
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_add_port
 功能描述  : 将端口添加到VLAN中
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_add_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_get_port
 功能描述  : 获取vlan中的端口，以bit位形式表示，为1则表示该端口在此vlan中
             31    30    ..    6    5    4    3    2    1    0
                               cpu  pon  fe4  fe3  fe2  fe1  ge0
 输入参数  : hi_ushort16 us_vlan                
 输出参数  : hi_uint32 *pui_portmap  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_get_port(hi_ushort16 us_vlan, hi_uint32 *pui_portmap);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_del_port
 功能描述  : 将端口从VLAN中删除
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_del_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);

/*****************************************************************************
 函 数 名  : hi_lsw_vlan_bc_add_port
 功能描述  : 将端口添加到VLAN出口的广播列表中
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_add_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_bc_get_port
 功能描述  : 获取添加到vlan广播列表的端口
 输入参数  : hi_ushort16 us_vlan              
 输出参数  : hi_uint32 *pui_portmap  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_get_port(hi_ushort16 us_vlan, hi_uint32 *pui_portmap);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_bc_del_port
 功能描述  : 将端口从VLAN的广播列表中删除
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_del_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_get_hybrid_tagmode
 功能描述  : hybrid模式下，获取出端口报文外层tag剥离使能/不使能
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port           
 输出参数  : hi_uchar8 *puc_enable  
 返 回 值  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_vlan_get_hybrid_tagmode(hi_ushort16 us_vlan, hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_set_hybrid_tagmode
 功能描述  : hybrid模式下，设置出端口报文外层tag剥离使能/不使能
 输入参数  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_set_hybrid_tagmode(hi_ushort16 us_vlan, hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_get_vlanfilter
 功能描述  : 获取端口vlan过滤使能
 输入参数  : hi_lsw_port_e em_port             
 输出参数  :  hi_uchar8 *puc_enable 
 返 回 值  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_vlan_get_vlanfilter(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_set_vlanfilter
 功能描述  : 设置端口vlan过滤使能
 输入参数  : hi_lsw_port_e em_port     hi_uchar8 uc_enable    
 输出参数  : 
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_set_vlanfilter(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

/*****************************************************************************
 函 数 名  : hi_lsw_vlan_display_bcegrs
 功能描述  : 显示VLAN中所有广播出口
 输入参数  : hi_ushort16 us_vlan  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_display_bcegrs(hi_ushort16 us_vlan);
/*****************************************************************************
 函 数 名  : hi_lsw_vlan_diagnose
 功能描述  : 打印所有配置过的vlan的端口信息和untag信息
 输入参数  : hi_lsw_diag_vlan_type_e em_type  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_diagnose(hi_lsw_diag_vlan_type_e em_type);


/*****************************************************************************
 函 数 名  : hi_lsw_vlan_del_all_cfg
 功能描述  : 删除所有vlan相关配置
 输入参数  : hi_void  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_del_all_cfg();

/* PORT */
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_status
 功能描述  : 设置端口使能状态
 输入参数  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_set_status(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

/*****************************************************************************
 函 数 名  : hi_lsw_port_get_status
 功能描述  : 获取端口使能状态
 输入参数  : hi_lsw_port_e em_port  
             hi_uchar8 *puc_enable  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_get_status(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_workmode
 功能描述  : 
 输入参数  : hi_lsw_port_e em_port  
             hi_lsw_phy_mode_e em_workmode 
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_set_workmode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e em_workmode);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_workmode
 功能描述  : 
 输入参数  : hi_lsw_port_e em_port  
             hi_lsw_phy_mode_e em_workmode 
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_get_workmode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e *pem_workmode);
//extern hi_uint32 hi_lsw_port_get_allcarenable(hi_uchar8 *puc_enable);
//extern hi_uint32 hi_lsw_port_set_allcarenable(hi_uchar8 uc_enable);
//extern hi_uint32 hi_lsw_port_get_car(hi_lsw_qos_egr_e em_egr, hi_uchar8 uc_queue, hi_lsw_car_info_s *pst_carattr);
//extern hi_uint32 hi_lsw_port_set_car(hi_lsw_qos_egr_e em_egr, hi_uchar8 uc_queue, hi_lsw_car_info_s *pst_car);
//extern hi_uint32 hi_lsw_port_get_carenable(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
//extern hi_uint32 hi_lsw_port_set_carenable(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_port_get_defvlan
 功能描述  : 获取端口默认vlan
 输入参数  : hi_lsw_port_e em_port  
 输出参数  : hi_ushort16 *pus_vlan 
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_defvlan(hi_lsw_port_e em_port, hi_ushort16 *pus_vlan);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_defvlan
 功能描述  : 设置端口默认vlan
 输入参数  : hi_lsw_port_e em_port  
             hi_ushort16 us_vlan    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_defvlan(hi_lsw_port_e em_port, hi_ushort16 us_vlan);
/*****************************************************************************
 函 数 名  : hi_lsw_port_get_defpri
 功能描述  : 获取端口默认pri
 输入参数  : hi_lsw_port_e em_port            
 输出参数  : hi_ushort16 *puc_pri  
 返 回 值  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_port_get_defpri(hi_lsw_port_e em_port, hi_uchar8 *puc_pri);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_defpri
 功能描述  : 设置端口默认优先级
 输入参数  : hi_lsw_port_e em_port  
             hi_uchar8 uc_pri    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_defpri(hi_lsw_port_e em_port, hi_uchar8 uc_pri);

//gemport环回?

/*****************************************************************************
 函 数 名  : hi_lsw_port_get_loopback
 功能描述  : 获取端口环回模式
 输入参数  : hi_lsw_port_e em_port                            
 输出参数  : hi_lsw_port_eth_loopback_e *pem_mode  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_loopback(hi_lsw_port_e em_port, hi_lsw_port_eth_loopback_e *pem_mode);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_loopback
 功能描述  : 设置端口环回模式
 输入参数  : hi_lsw_port_e em_port               
             hi_lsw_port_eth_loopback_e em_mode  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_loopback(hi_lsw_port_e em_port, hi_lsw_port_eth_loopback_e em_mode);

//extern hi_uint32 hi_lsw_port_get_access_mode(hi_lsw_port_e em_port, hi_lsw_port_mode_e *pem_mode);
//extern hi_uint32 hi_lsw_port_set_access_mode(hi_lsw_port_e em_port, hi_lsw_port_mode_e em_mode);

//该为全局配置了，没有针对端口的?

/*****************************************************************************
 函 数 名  : hi_lsw_port_get_mtu
 功能描述  : 获取端口MTU值
 输入参数  : hi_lsw_port_e em_port                  
 输出参数  : hi_uint32 *pui_mtu
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_mtu(hi_lsw_port_e em_port, hi_uint32 *pui_mtu);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_mtu
 功能描述  : 设置端口MTU值
 输入参数  : hi_lsw_port_e em_port  
             hi_uint32 ui_mtu       
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_mtu(hi_lsw_port_e em_port, hi_uint32 ui_mtu);
/*****************************************************************************
 函 数 名  : hi_lsw_port_get_pause
 功能描述  : 获取端口发送pause帧使能状态
 输入参数  : hi_lsw_port_e em_port            
 输出参数  : hi_uchar8 *puc_enable  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_pause(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_pause
 功能描述  : 设置端口发送pause帧使能状态
 输入参数  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_pause(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

extern hi_uint32 hi_lsw_port_set_pausetime(hi_lsw_port_e em_port, hi_uint32 ui_time);
extern hi_uint32 hi_lsw_port_get_pausetime(hi_lsw_port_e em_port, hi_uint32 *pui_time);

/*****************************************************************************
 函 数 名  : hi_lsw_port_get_stp
 功能描述  : 获取stp模式
 输入参数  : 
 输出参数  : hi_eth_stp_mode_e *pem_stp  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_stp(hi_eth_stp_mode_e *pem_stp);


/*****************************************************************************
 函 数 名  : hi_lsw_port_set_stp
 功能描述  : 设置stp模式
 输入参数  : hi_eth_stp_mode_e em_stp  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_stp(hi_eth_stp_mode_e em_stp);

/*****************************************************************************
 函 数 名  : hi_lsw_port_get_stpstate
 功能描述  : 获取端口stp状态
 输入参数  : hi_lsw_port_e em_port                       
 输出参数  : hi_eth_stp_status_e *pem_state  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_stpstate(hi_lsw_port_e em_port, hi_eth_stp_status_e *pem_state);
//extern hi_uint32 hi_lsw_port_set_stpstate(hi_lsw_port_e em_port, hi_eth_stp_status_e em_state);
/*****************************************************************************
 函 数 名  : hi_lsw_port_get_tag_act
 功能描述  : 获取端口stp状态
 输入参数  : hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype               
 输出参数  : hi_lsw_tag_act_e *pem_tagact 
 返 回 值  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_port_get_tag_act(hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype, hi_lsw_tag_act_e *pem_tagact);
/*****************************************************************************
 函 数 名  : hi_lsw_port_set_tag_act
 功能描述  : 设置tag 动作
 输入参数  : hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype, hi_lsw_tag_act_e em_tagact       
 输出参数  :  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_tag_act(hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype, hi_lsw_tag_act_e em_tagact);

extern hi_uint32 hi_lsw_port_init_network(hi_void);

/* PHY */
extern hi_uint32 hi_lsw_phy_set_mode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e em_mode);
extern hi_uint32 hi_lsw_phy_get_mode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e *pem_mode);
extern hi_uint32 hi_lsw_phy_set_power(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
extern hi_uint32 hi_lsw_phy_get_power(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
extern hi_uint32 hi_lsw_phy_set_status(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
extern hi_uint32 hi_lsw_phy_get_status(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);

/* MAC */
//函数注释中要标明输入输出
//参数该为结构体 hi_lsw_mac_item_s
/*****************************************************************************
 函 数 名  : hi_lsw_mac_add
 功能描述  : 添加mac表项到L2MAC表
 输入参数  : hi_lsw_mac_item_s *pst_macitem  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_add(hi_lsw_mac_item_s *pst_macitem);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get
 功能描述  : 根据mac 和vlan获取L2MAC表项
 输入参数  : 
 输出参数  : hi_lsw_mac_item_s *pst_macitem    
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get(hi_lsw_mac_item_s *pst_macitem);

/*****************************************************************************
 函 数 名  : hi_lsw_mac_del
 功能描述  : 根据mac和vlan、untag删除L2MAC表项
 输入参数  : hi_uchar8 *puc_mac   
             hi_ushort16 us_vlan  
             hi_uchar8 uc_untag   
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_del(hi_uchar8 *puc_mac, hi_ushort16 us_vlan);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_l2item
 功能描述  : 根据intem的index获取L2MAC表项
 输入参数  :   
 输出参数  : hi_ushort16 us_index            
             hi_lsw_mac_item_s *pst_macitem
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_l2item(hi_ushort16 us_index, hi_lsw_mac_item_s *pst_macitem);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_ageenable
 功能描述  : 获取L2MAC表老化使能/去使能状态
 输入参数  : 
 输出参数  : hi_uchar8 *puc_enable  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_ageenable(hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_ageenable
 功能描述  : 设置L2MAC表老化使能/去使能状态
 输入参数  : hi_uchar8 uc_enable  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_ageenable(hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_agetime
 功能描述  : 设置L2MAC表老化时间
 输入参数  : hi_ushort16 us_agetime  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_agetime(hi_uint32 *pui_agetime);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_agetime
 功能描述  : 获取L2MAC表老化时间
 输入参数  :   
 输出参数  : hi_ushort16 *pus_agetime
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_agetime(hi_uint32 ui_agetime);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_learn
 功能描述  : 获取端口学习使能或去使能状态
 输入参数  :  
 输出参数  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable   
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learn(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_learn
 功能描述  : 设置端口学习使能或去使能状态
 输入参数  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learn(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_learnmode
 功能描述  : 获取mac地址学习模式
 输入参数  :   
 输出参数  :hi_lsw_mac_learn_mode_e *pem_learnmode  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learnmode(hi_lsw_mac_learn_mode_e *pem_learnmode);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_learnmode
 功能描述  : 设置mac地址学习模式
 输入参数  :  hi_lsw_mac_learn_mode_e em_learnmode
 输出参数  :  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learnmode(hi_lsw_mac_learn_mode_e em_learnmode);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_learn_fail
 功能描述  : 获取mac 地址学习失败后的动作
 输入参数  :  
 输出参数  : hi_lsw_trans_act_e *pem_act
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learn_fail(hi_lsw_trans_act_e *pem_act);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_learn_fail
 功能描述  : 设置mac 地址学习失败后的动作
 输入参数  : hi_lsw_trans_act_e em_act  
 输出参数  : 
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learn_fail(hi_lsw_trans_act_e em_act);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_limitcount
 功能描述  : 获取端口的mac地址学习限制数目
 输入参数  :  hi_lsw_port_e em_port
 输出参数  :  hi_ushort16 *pus_count
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_limitcount(hi_lsw_port_e em_port, hi_ushort16 *pus_count);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_limitcount
 功能描述  : 设置端口的mac 地址学习数目限制
 输入参数  :  hi_lsw_port_e em_port, hi_ushort16 us_count
 输出参数  :  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_limitcount(hi_lsw_port_e em_port, hi_ushort16 us_count);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_get_limitenable
 功能描述  : 获取基于端口的mac 地址学习限制使能状态
 输入参数  :  hi_lsw_port_e em_port
 输出参数  :  hi_uchar8 *puc_enable
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_limitenable(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 函 数 名  : hi_lsw_mac_set_limitenable
 功能描述  : 设置端口的mac 地址学习限制使能
 输入参数  :  hi_lsw_port_e em_port, hi_uchar8 uc_enable
 输出参数  :  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_limitenable(hi_lsw_port_e em_port, hi_uchar8 uc_enable);


/*****************************************************************************
 函 数 名  : hi_lsw_mac_diagnose
 功能描述  : mac 调试接口
 输入参数  :   
 输出参数  :   
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_diagnose(hi_lsw_diag_mac_type_e em_type);

/* PM 统计查询*/
extern hi_uint32 hi_lsw_pm_get_eth_statistic(hi_lsw_port_e em_port , hi_lsw_eth_cnt_s *pst_pm);

extern hi_uint32 hi_lsw_flow_get_gemmap_by_vlan(hi_ushort16 us_vlan, hi_ushort16 *pus_gemport, hi_ushort16 *pus_tcont, 
                                                hi_lsw_flow_mapping_s *pst_map);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_ADAPTER_API_H__ */
