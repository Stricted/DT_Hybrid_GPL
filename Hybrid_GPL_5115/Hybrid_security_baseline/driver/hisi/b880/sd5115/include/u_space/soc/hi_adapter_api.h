/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_adapter_api.h
  �� �� ��   : ����
  ��    ��   : t00185981
  ��������   : D2012_02_18

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
 �� �� ��  : hi_chip_get_version
 ��������  : ��ȡоƬ����ID
 �������  : 
 �������  : hi_uint32 *pui_version  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_version(hi_uint32 *pui_version);
/*****************************************************************************
 �� �� ��  : hi_chip_get_vendor
 ��������  : ��ȡ����ID
 �������  :  
 �������  : hi_ushort16 *pus_version 
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_vendor(hi_ushort16 *pus_vendor);

/*****************************************************************************
 �� �� ��  : hi_chip_get_chip_version
 ��������  : ��ȡоƬ�汾��vendor_id���̼��汾��Ϣ
 �������  : hi_uint32 *pui_version           
             hi_uint32 *pui_vendor_id         
             hi_uint32 *pui_firmware_version  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_chip_version(hi_uint32 *pui_version, hi_uint32 *pui_vendor_id, hi_uint32 *pui_firmware_version);


/*****************************************************************************
 �� �� ��  : hi_chip_get_upmode
 ��������  : ��ȡ����ģʽ
 �������  : ��
 �������  : hi_chip_up_mode_e *pem_mode
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_upmode(hi_chip_up_mode_e *pem_mode);
/*****************************************************************************
 �� �� ��  : hi_chip_set_upmode
 ��������  : ��������ģʽ
 �������  : hi_chip_up_mode_e em_mode
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_upmode(hi_chip_up_mode_e em_mode);
/*****************************************************************************
 �� �� ��  : hi_chip_get_access_mode
 ��������  : ��ȡоƬ����ģʽ
 �������  : ��
 �������  : hi_lsw_glb_sw_mode_e *pem_swmode
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_access_mode(hi_lsw_glb_sw_mode_e *pem_swmode);
/*****************************************************************************
 �� �� ��  : hi_chip_set_access_mode
 ��������  : ����оƬ����ģʽ
 �������  : hi_lsw_glb_sw_mode_e em_swmode
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_access_mode(hi_lsw_glb_sw_mode_e em_swmode);
/*****************************************************************************
 �� �� ��  : hi_chip_get_port_cascade
 ��������  : ��ȡ����ģʽ
 �������  : ��
 �������  : hi_uchar8 *puc_enable 
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade(hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_chip_set_port_cascade
 ��������  : ���ü���ʹ��
 �������  : hi_uchar8 uc_enable
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade(hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_chip_get_port_cascade_doubletag
 ��������  : ˫��tag����
 �������  : ��
 �������  : hi_uchar8 *puc_enable
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade_doubletag(hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_chip_set_port_cascade_doubletag
 ��������  : ˫��tag����
 �������  : hi_uchar8 uc_enable
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_doubletag(hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_chip_get_port_cascade_tpid
 ��������  : 
 �������  : ��
 �������  : hi_ushort16 *pus_tpid
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_get_port_cascade_tpid(hi_ushort16 *pus_tpid);
/*****************************************************************************
 �� �� ��  : hi_chip_set_port_cascade_tpid
 ��������  : 
 �������  : hi_ushort16 us_tpid
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_tpid(hi_ushort16 us_tpid);
/*****************************************************************************
 �� �� ��  : hi_chip_get_port_cascade_map
 ��������  : ����ӳ���ϵ��Ĭ��ʹ��
 �������  : 
 �������  : 
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
//�˿�index��оƬ�Ķ˿ڵĶ�Ӧ��ϵ?
extern hi_uint32 hi_chip_get_port_cascade_map(hi_uchar8 uc_portindex, hi_ushort16 *pus_vlan, hi_uchar8 *puc_cfi, hi_uchar8 *puc_pri);

/*****************************************************************************
 �� �� ��  : hi_chip_set_port_cascade_map
 ��������  : ����ӳ���ϵ��Ĭ��ʹ��
 �������  : 
 �������  : 
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_chip_set_port_cascade_map(hi_uchar8 uc_portindex, hi_ushort16 us_vlan, hi_uchar8 uc_cfi, hi_uchar8 uc_pri);

/*****************************************************************************
 �� �� ��  : hi_chip_set_glb_vlan_mode
 ��������  : ����ȫ�ֵ�vlanģʽѡ��
 �������  : hi_lsw_module_e em_module        
             hi_lsw_direction_e em_direction  
             hi_lsw_vlan_mode_e em_vlan       
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
//extern hi_uint32 hi_chip_set_glb_vlan_mode(hi_lsw_module_e em_module, hi_lsw_direction_e em_direction, hi_lsw_vlan_mode_e em_vlanmode);
//extern hi_uint32 hi_chip_get_glb_vlan_mode(hi_lsw_module_e em_module, hi_lsw_direction_e em_direction, hi_lsw_vlan_mode_e *pem_vlanmode);
/*****************************************************************************
 �� �� ��  : hi_chip_set_glb_pri_mode
 ��������  : ����ȫ�ֵ�PRIģʽѡ��
 �������  : hi_lsw_direction_e em_direction  
             hi_lsw_pri_mode_e em_pri         
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
//extern hi_uint32 hi_chip_set_glb_pri_mode(hi_lsw_direction_e em_direction, hi_lsw_pri_mode_e em_primode);
//extern hi_uint32 hi_chip_get_glb_pri_mode(hi_lsw_direction_e em_direction, hi_lsw_pri_mode_e *pem_primode);
extern hi_uint32 hi_chip_init_vlan_pri_select();

/* EMAC */
/*****************************************************************************
 �� �� ��  : hi_emac_get_fec
 ��������  : ��ȡepon fec״̬
 �������  : ��
 �������  : hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_emac_get_fec(hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_emac_set_fec
 ��������  : ����epon fec״̬
 �������  :hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_emac_set_fec(hi_uchar8 uc_llid, hi_lsw_direction_e em_direction, hi_uchar8 uc_enable);

/* GMAC */
/*****************************************************************************
 �� �� ��  : hi_gmac_get_aes
 ��������  : aesʹ��/ȥʹ��״̬��ȡ
 �������  : 
 �������  : hi_ushort16 us_gemport  
             hi_uchar8 *puc_enable   
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_aes(hi_ushort16 us_gemport, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_gmac_set_aes
 ��������  : aesʹ��/ȥʹ������
 �������  : hi_ushort16 us_gemport  
             hi_uchar8 uc_enable     
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_aes(hi_ushort16 us_gemport, hi_uchar8 uc_enable);
//��Կ���ú��л��ӿ���ploam���Ƿ����ṩ?
//extern hi_uint32 hi_gmac_set_aes_exchange(hi_uchar8 uc_enable);
//extern hi_uint32 hi_gmac_set_aes_exchange(hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_gmac_get_fec
 ��������  : fec״̬��ȡ
 �������  :           
 �������  : hi_lsw_direction_e em_direction  
             hi_uchar8 *puc_enable  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_fec(hi_lsw_direction_e em_direction, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_gmac_set_fec
 ��������  : FEC����
 �������  : hi_lsw_direction_e em_direction  
             hi_uchar8 uc_enable              
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
//�Ƿ���Ҫ�������?
extern hi_uint32 hi_gmac_set_fec(hi_lsw_direction_e em_direction, hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_gmac_get_gemport
 ��������  : GPONģʽ�¸���������TCONT��GEMPORT��ȡ
 �������  :  
 �������  : hi_ushort16 us_fid        
             hi_uchar8 *puc_tcontid    
             hi_ushort16 *pus_gemport 
 �� �� ֵ  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_gmac_get_gemport(hi_ushort16 us_sfid, hi_uchar8 *puc_tcontid, hi_ushort16 *pus_gemport);
/*****************************************************************************
 �� �� ��  : hi_gmac_set_gemport
 ��������  : GPONģʽ�¸���������TCONT��GEMPORT����
 �������  : hi_ushort16 us_fid      
             hi_uchar8 uc_tcontid    
             hi_ushort16 us_gemport  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_gemport(hi_ushort16 us_sfid, hi_uchar8 uc_tcontid, hi_ushort16 us_gemport); 

extern  hi_uint32 hi_gmac_del_gemport(hi_ushort16 us_sfid);
/*****************************************************************************
 �� �� ��  : hi_gmac_set_down_gemport_type
 ��������  : ����GPON����GEMPORT�ĳ�������
 �������  : hi_ushort16 us_gemport              
             hi_lsw_gpon_dn_port_type_e em_type  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_set_down_gemport_type(hi_ushort16 us_gemport, hi_lsw_gpon_dn_port_type_e em_type);
/*****************************************************************************
 �� �� ��  : hi_gmac_get_down_gemport_type
 ��������  : ��ȡGPON����GEMPORT�ĳ�������
 �������  : 
 �������  : hi_ushort16 us_gemport                
             hi_lsw_gpon_dn_port_type_e *pem_type  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_get_down_gemport_type(hi_ushort16 us_gemport, hi_lsw_gpon_dn_port_type_e *pem_type);
/*****************************************************************************
 �� �� ��  : hi_gmac_del_down_gemport
 ��������  : ����ɾ����GEMPORT
 �������  : hi_ushort16 us_gemport  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_gmac_del_down_gemport(hi_ushort16 us_gemport);

/* FLOW */
/*****************************************************************************
 �� �� ��  : hi_lsw_flow_add_service
 ��������  : ���ҵ���л���
 �������  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
             hi_ushort16 *pus_fid               
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_add_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact,
                                         hi_lsw_trans_act_e em_fwd_act, hi_ushort16 *pus_fid);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_get_service
 ��������  : ����ҵ���л������ò���FID
 �������  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
             hi_ushort16 *pus_fid               
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_get_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact, 
                                         hi_lsw_trans_act_e em_fwd_act, hi_ushort16 *pus_fid);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_del_service
 ��������  : ɾ��ҵ���л���
 �������  : hi_lsw_flow_fmap_info_s *pst_fmap  
             hi_lsw_flow_fact_info_s *pst_fact  
             hi_lsw_trans_act_e em_fwd_act      
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_service(hi_lsw_flow_fmap_info_s *pst_fmap, hi_lsw_flow_fact_info_s *pst_fact,
                                         hi_lsw_trans_act_e em_fwd_act);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_del_fid
 ��������  : ����FIDɾ��ҵ���л���
 �������  : hi_ushort16 us_fid  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_fid(hi_ushort16 us_fid);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_add_mapping
 ��������  : ���ӳ����
 �������  : hi_lsw_flow_mapping_s *pst_mapinfo  
             hi_ushort16 *pus_sfid               
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_add_mapping(hi_lsw_flow_mapping_s *pst_mapinfo, hi_lsw_direction_e em_direction, hi_ushort16 *pus_sfid);

extern hi_uint32 hi_lsw_flow_del_mapping_by_sfid(hi_ushort16 us_sfid);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_del_mapping
 ��������  : ɾ��ӳ����
 �������  : hi_lsw_flow_mapping_s *pst_mapinfo  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_mapping(hi_lsw_flow_mapping_s *pst_mapinfo, hi_lsw_direction_e em_direction);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_del_all
 ��������  : ɾ��ȫ��EFC��SFC����
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_del_all();

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_change_igmp_pri
 ��������  : ���ҵ���������鲥���ĵ����ȼ�
 �������  : hi_uchar8 uc_pri  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_flow_change_igmp_pri(hi_uchar8 uc_pri);

/*****************************************************************************
 �� �� ��  : hi_lsw_flow_diagnose
 ��������  : ��ӡоƬ��������
 �������  : hi_lsw_diag_flow_type_e em_type  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
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
/*����Ϊ0��ȥʹ��shaping��CPU�̶�Ϊpps������Ϊbps*/
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
 �� �� ��  : hi_lsw_vlan_add_port
 ��������  : ���˿���ӵ�VLAN��
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_add_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_get_port
 ��������  : ��ȡvlan�еĶ˿ڣ���bitλ��ʽ��ʾ��Ϊ1���ʾ�ö˿��ڴ�vlan��
             31    30    ..    6    5    4    3    2    1    0
                               cpu  pon  fe4  fe3  fe2  fe1  ge0
 �������  : hi_ushort16 us_vlan                
 �������  : hi_uint32 *pui_portmap  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_get_port(hi_ushort16 us_vlan, hi_uint32 *pui_portmap);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_del_port
 ��������  : ���˿ڴ�VLAN��ɾ��
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_del_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);

/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_bc_add_port
 ��������  : ���˿���ӵ�VLAN���ڵĹ㲥�б���
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_add_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_bc_get_port
 ��������  : ��ȡ��ӵ�vlan�㲥�б�Ķ˿�
 �������  : hi_ushort16 us_vlan              
 �������  : hi_uint32 *pui_portmap  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_get_port(hi_ushort16 us_vlan, hi_uint32 *pui_portmap);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_bc_del_port
 ��������  : ���˿ڴ�VLAN�Ĺ㲥�б���ɾ��
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_bc_del_port(hi_ushort16 us_vlan, hi_lsw_port_e em_port);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_get_hybrid_tagmode
 ��������  : hybridģʽ�£���ȡ���˿ڱ������tag����ʹ��/��ʹ��
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port           
 �������  : hi_uchar8 *puc_enable  
 �� �� ֵ  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_vlan_get_hybrid_tagmode(hi_ushort16 us_vlan, hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_set_hybrid_tagmode
 ��������  : hybridģʽ�£����ó��˿ڱ������tag����ʹ��/��ʹ��
 �������  : hi_ushort16 us_vlan    
             hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_set_hybrid_tagmode(hi_ushort16 us_vlan, hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_get_vlanfilter
 ��������  : ��ȡ�˿�vlan����ʹ��
 �������  : hi_lsw_port_e em_port             
 �������  :  hi_uchar8 *puc_enable 
 �� �� ֵ  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_vlan_get_vlanfilter(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_set_vlanfilter
 ��������  : ���ö˿�vlan����ʹ��
 �������  : hi_lsw_port_e em_port     hi_uchar8 uc_enable    
 �������  : 
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_set_vlanfilter(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_display_bcegrs
 ��������  : ��ʾVLAN�����й㲥����
 �������  : hi_ushort16 us_vlan  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_display_bcegrs(hi_ushort16 us_vlan);
/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_diagnose
 ��������  : ��ӡ�������ù���vlan�Ķ˿���Ϣ��untag��Ϣ
 �������  : hi_lsw_diag_vlan_type_e em_type  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_diagnose(hi_lsw_diag_vlan_type_e em_type);


/*****************************************************************************
 �� �� ��  : hi_lsw_vlan_del_all_cfg
 ��������  : ɾ������vlan�������
 �������  : hi_void  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_vlan_del_all_cfg();

/* PORT */
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_status
 ��������  : ���ö˿�ʹ��״̬
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_set_status(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_status
 ��������  : ��ȡ�˿�ʹ��״̬
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 *puc_enable  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_get_status(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_workmode
 ��������  : 
 �������  : hi_lsw_port_e em_port  
             hi_lsw_phy_mode_e em_workmode 
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_set_workmode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e em_workmode);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_workmode
 ��������  : 
 �������  : hi_lsw_port_e em_port  
             hi_lsw_phy_mode_e em_workmode 
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
hi_uint32 hi_lsw_port_get_workmode(hi_lsw_port_e em_port, hi_lsw_phy_mode_e *pem_workmode);
//extern hi_uint32 hi_lsw_port_get_allcarenable(hi_uchar8 *puc_enable);
//extern hi_uint32 hi_lsw_port_set_allcarenable(hi_uchar8 uc_enable);
//extern hi_uint32 hi_lsw_port_get_car(hi_lsw_qos_egr_e em_egr, hi_uchar8 uc_queue, hi_lsw_car_info_s *pst_carattr);
//extern hi_uint32 hi_lsw_port_set_car(hi_lsw_qos_egr_e em_egr, hi_uchar8 uc_queue, hi_lsw_car_info_s *pst_car);
//extern hi_uint32 hi_lsw_port_get_carenable(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
//extern hi_uint32 hi_lsw_port_set_carenable(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_defvlan
 ��������  : ��ȡ�˿�Ĭ��vlan
 �������  : hi_lsw_port_e em_port  
 �������  : hi_ushort16 *pus_vlan 
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_defvlan(hi_lsw_port_e em_port, hi_ushort16 *pus_vlan);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_defvlan
 ��������  : ���ö˿�Ĭ��vlan
 �������  : hi_lsw_port_e em_port  
             hi_ushort16 us_vlan    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_defvlan(hi_lsw_port_e em_port, hi_ushort16 us_vlan);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_defpri
 ��������  : ��ȡ�˿�Ĭ��pri
 �������  : hi_lsw_port_e em_port            
 �������  : hi_ushort16 *puc_pri  
 �� �� ֵ  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_port_get_defpri(hi_lsw_port_e em_port, hi_uchar8 *puc_pri);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_defpri
 ��������  : ���ö˿�Ĭ�����ȼ�
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 uc_pri    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_defpri(hi_lsw_port_e em_port, hi_uchar8 uc_pri);

//gemport����?

/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_loopback
 ��������  : ��ȡ�˿ڻ���ģʽ
 �������  : hi_lsw_port_e em_port                            
 �������  : hi_lsw_port_eth_loopback_e *pem_mode  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_loopback(hi_lsw_port_e em_port, hi_lsw_port_eth_loopback_e *pem_mode);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_loopback
 ��������  : ���ö˿ڻ���ģʽ
 �������  : hi_lsw_port_e em_port               
             hi_lsw_port_eth_loopback_e em_mode  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_loopback(hi_lsw_port_e em_port, hi_lsw_port_eth_loopback_e em_mode);

//extern hi_uint32 hi_lsw_port_get_access_mode(hi_lsw_port_e em_port, hi_lsw_port_mode_e *pem_mode);
//extern hi_uint32 hi_lsw_port_set_access_mode(hi_lsw_port_e em_port, hi_lsw_port_mode_e em_mode);

//��Ϊȫ�������ˣ�û����Զ˿ڵ�?

/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_mtu
 ��������  : ��ȡ�˿�MTUֵ
 �������  : hi_lsw_port_e em_port                  
 �������  : hi_uint32 *pui_mtu
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_mtu(hi_lsw_port_e em_port, hi_uint32 *pui_mtu);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_mtu
 ��������  : ���ö˿�MTUֵ
 �������  : hi_lsw_port_e em_port  
             hi_uint32 ui_mtu       
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_mtu(hi_lsw_port_e em_port, hi_uint32 ui_mtu);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_pause
 ��������  : ��ȡ�˿ڷ���pause֡ʹ��״̬
 �������  : hi_lsw_port_e em_port            
 �������  : hi_uchar8 *puc_enable  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_pause(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_pause
 ��������  : ���ö˿ڷ���pause֡ʹ��״̬
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_pause(hi_lsw_port_e em_port, hi_uchar8 uc_enable);

extern hi_uint32 hi_lsw_port_set_pausetime(hi_lsw_port_e em_port, hi_uint32 ui_time);
extern hi_uint32 hi_lsw_port_get_pausetime(hi_lsw_port_e em_port, hi_uint32 *pui_time);

/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_stp
 ��������  : ��ȡstpģʽ
 �������  : 
 �������  : hi_eth_stp_mode_e *pem_stp  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_stp(hi_eth_stp_mode_e *pem_stp);


/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_stp
 ��������  : ����stpģʽ
 �������  : hi_eth_stp_mode_e em_stp  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_set_stp(hi_eth_stp_mode_e em_stp);

/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_stpstate
 ��������  : ��ȡ�˿�stp״̬
 �������  : hi_lsw_port_e em_port                       
 �������  : hi_eth_stp_status_e *pem_state  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_port_get_stpstate(hi_lsw_port_e em_port, hi_eth_stp_status_e *pem_state);
//extern hi_uint32 hi_lsw_port_set_stpstate(hi_lsw_port_e em_port, hi_eth_stp_status_e em_state);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_get_tag_act
 ��������  : ��ȡ�˿�stp״̬
 �������  : hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype               
 �������  : hi_lsw_tag_act_e *pem_tagact 
 �� �� ֵ  : hi_uint32
*****************************************************************************/

extern hi_uint32 hi_lsw_port_get_tag_act(hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype, hi_lsw_tag_act_e *pem_tagact);
/*****************************************************************************
 �� �� ��  : hi_lsw_port_set_tag_act
 ��������  : ����tag ����
 �������  : hi_lsw_port_e em_port, hi_lsw_inout_e em_dir, hi_lsw_tag_type_e em_tagtype, hi_lsw_tag_act_e em_tagact       
 �������  :  
 �� �� ֵ  : hi_uint32
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
//����ע����Ҫ�����������
//������Ϊ�ṹ�� hi_lsw_mac_item_s
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_add
 ��������  : ���mac���L2MAC��
 �������  : hi_lsw_mac_item_s *pst_macitem  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_add(hi_lsw_mac_item_s *pst_macitem);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get
 ��������  : ����mac ��vlan��ȡL2MAC����
 �������  : 
 �������  : hi_lsw_mac_item_s *pst_macitem    
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get(hi_lsw_mac_item_s *pst_macitem);

/*****************************************************************************
 �� �� ��  : hi_lsw_mac_del
 ��������  : ����mac��vlan��untagɾ��L2MAC����
 �������  : hi_uchar8 *puc_mac   
             hi_ushort16 us_vlan  
             hi_uchar8 uc_untag   
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_del(hi_uchar8 *puc_mac, hi_ushort16 us_vlan);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_l2item
 ��������  : ����intem��index��ȡL2MAC����
 �������  :   
 �������  : hi_ushort16 us_index            
             hi_lsw_mac_item_s *pst_macitem
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_l2item(hi_ushort16 us_index, hi_lsw_mac_item_s *pst_macitem);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_ageenable
 ��������  : ��ȡL2MAC���ϻ�ʹ��/ȥʹ��״̬
 �������  : 
 �������  : hi_uchar8 *puc_enable  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_ageenable(hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_ageenable
 ��������  : ����L2MAC���ϻ�ʹ��/ȥʹ��״̬
 �������  : hi_uchar8 uc_enable  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_ageenable(hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_agetime
 ��������  : ����L2MAC���ϻ�ʱ��
 �������  : hi_ushort16 us_agetime  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_agetime(hi_uint32 *pui_agetime);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_agetime
 ��������  : ��ȡL2MAC���ϻ�ʱ��
 �������  :   
 �������  : hi_ushort16 *pus_agetime
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_agetime(hi_uint32 ui_agetime);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_learn
 ��������  : ��ȡ�˿�ѧϰʹ�ܻ�ȥʹ��״̬
 �������  :  
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable   
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learn(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_learn
 ��������  : ���ö˿�ѧϰʹ�ܻ�ȥʹ��״̬
 �������  : hi_lsw_port_e em_port  
             hi_uchar8 uc_enable    
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learn(hi_lsw_port_e em_port, hi_uchar8 uc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_learnmode
 ��������  : ��ȡmac��ַѧϰģʽ
 �������  :   
 �������  :hi_lsw_mac_learn_mode_e *pem_learnmode  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learnmode(hi_lsw_mac_learn_mode_e *pem_learnmode);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_learnmode
 ��������  : ����mac��ַѧϰģʽ
 �������  :  hi_lsw_mac_learn_mode_e em_learnmode
 �������  :  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learnmode(hi_lsw_mac_learn_mode_e em_learnmode);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_learn_fail
 ��������  : ��ȡmac ��ַѧϰʧ�ܺ�Ķ���
 �������  :  
 �������  : hi_lsw_trans_act_e *pem_act
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_learn_fail(hi_lsw_trans_act_e *pem_act);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_learn_fail
 ��������  : ����mac ��ַѧϰʧ�ܺ�Ķ���
 �������  : hi_lsw_trans_act_e em_act  
 �������  : 
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_learn_fail(hi_lsw_trans_act_e em_act);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_limitcount
 ��������  : ��ȡ�˿ڵ�mac��ַѧϰ������Ŀ
 �������  :  hi_lsw_port_e em_port
 �������  :  hi_ushort16 *pus_count
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_limitcount(hi_lsw_port_e em_port, hi_ushort16 *pus_count);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_limitcount
 ��������  : ���ö˿ڵ�mac ��ַѧϰ��Ŀ����
 �������  :  hi_lsw_port_e em_port, hi_ushort16 us_count
 �������  :  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_limitcount(hi_lsw_port_e em_port, hi_ushort16 us_count);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_get_limitenable
 ��������  : ��ȡ���ڶ˿ڵ�mac ��ַѧϰ����ʹ��״̬
 �������  :  hi_lsw_port_e em_port
 �������  :  hi_uchar8 *puc_enable
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_get_limitenable(hi_lsw_port_e em_port, hi_uchar8 *puc_enable);
/*****************************************************************************
 �� �� ��  : hi_lsw_mac_set_limitenable
 ��������  : ���ö˿ڵ�mac ��ַѧϰ����ʹ��
 �������  :  hi_lsw_port_e em_port, hi_uchar8 uc_enable
 �������  :  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_set_limitenable(hi_lsw_port_e em_port, hi_uchar8 uc_enable);


/*****************************************************************************
 �� �� ��  : hi_lsw_mac_diagnose
 ��������  : mac ���Խӿ�
 �������  :   
 �������  :   
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_lsw_mac_diagnose(hi_lsw_diag_mac_type_e em_type);

/* PM ͳ�Ʋ�ѯ*/
extern hi_uint32 hi_lsw_pm_get_eth_statistic(hi_lsw_port_e em_port , hi_lsw_eth_cnt_s *pst_pm);

extern hi_uint32 hi_lsw_flow_get_gemmap_by_vlan(hi_ushort16 us_vlan, hi_ushort16 *pus_gemport, hi_ushort16 *pus_tcont, 
                                                hi_lsw_flow_mapping_s *pst_map);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_ADAPTER_API_H__ */
