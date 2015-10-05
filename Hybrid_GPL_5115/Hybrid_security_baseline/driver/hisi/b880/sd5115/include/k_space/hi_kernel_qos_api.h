/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : hi_kernel_qos_api.h
  �� �� ��   : ����
  ��    ��   : y00206091
  ��������   : D2012_12_10

******************************************************************************/
#ifndef __HI_KERNEL_QOS_API_H__
#define __HI_KERNEL_QOS_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define HI_KERNEL_QOS_RULE_MASK_PHYPORT   0x00000001
#define HI_KERNEL_QOS_RULE_MASK_SMAC      0x00000002
#define HI_KERNEL_QOS_RULE_MASK_DMAC      0x00000004
#define HI_KERNEL_QOS_RULE_MASK_SVID      0x00000008
#define HI_KERNEL_QOS_RULE_MASK_COS       0x00000010
#define HI_KERNEL_QOS_RULE_MASK_ETHTYPE   0x00000020
#define HI_KERNEL_QOS_RULE_MASK_DSCP      0x00000040
#define HI_KERNEL_QOS_RULE_MASK_IPPROTO   0x00000080
#define HI_KERNEL_QOS_RULE_MASK_SIP       0x00000100
#define HI_KERNEL_QOS_RULE_MASK_DIP       0x00000200
#define HI_KERNEL_QOS_RULE_MASK_SPORT     0x00000400
#define HI_KERNEL_QOS_RULE_MASK_DPORT     0x00000800


typedef enum  {
    HI_RET_QOS_BASEVALUE                = (HI_SUBMODULE_KNETWORKS_QOS   ),
    HI_RET_QOS_ERR_PORT_DISABLED        = (HI_RET_QOS_BASEVALUE | 0x01  ),     // ���˿�Qosδʹ��
    HI_RET_QOS_ERR_ITEM_INVALID         = (HI_RET_QOS_BASEVALUE | 0x02  ),     // ������Ч
    HI_RET_QOS_ERR_ITEM_MISMATCH        = (HI_RET_QOS_BASEVALUE | 0x03  ),     // ���ƥ��
    HI_RET_QOS_ERR_ITEM_EXCEPTION       = (HI_RET_QOS_BASEVALUE | 0x04  ),     // �����쳣
    HI_RET_QOS_ERR_PHYPORT_MISMATCH     = (HI_RET_QOS_BASEVALUE | 0x05  ),     // ����˿ڲ�ƥ��
    HI_RET_QOS_ERR_SMAC_MISMATCH        = (HI_RET_QOS_BASEVALUE | 0x06  ),     // Դmac��ƥ��
    HI_RET_QOS_ERR_DMAC_MISMATCH        = (HI_RET_QOS_BASEVALUE | 0x07  ),     // Ŀ��mac��ƥ��
    HI_RET_QOS_ERR_SVID_MISMATCH        = (HI_RET_QOS_BASEVALUE | 0x08  ),     // svlan id��ƥ��
    HI_RET_QOS_ERR_COS_MISMATCH         = (HI_RET_QOS_BASEVALUE | 0x09  ),     // 802.1p��ƥ��
    HI_RET_QOS_ERR_ETHTYPE_MISMATCH     = (HI_RET_QOS_BASEVALUE | 0x0A  ),     // ��̫Э��Ų�ƥ��
    HI_RET_QOS_ERR_DSCP_MISMATCH        = (HI_RET_QOS_BASEVALUE | 0x0B  ),     // dscp��ƥ��
    HI_RET_QOS_ERR_IPPROTO_MISMATCH     = (HI_RET_QOS_BASEVALUE | 0x0C  ),     // ipЭ�����Ͳ�ƥ�䲻ƥ��
    HI_RET_QOS_ERR_SIP_MISMATCH         = (HI_RET_QOS_BASEVALUE | 0x0D  ),     // Դip��ƥ��
    HI_RET_QOS_ERR_DIP_MISMATCH         = (HI_RET_QOS_BASEVALUE | 0x0E  ),     // Ŀ��ip��ƥ��
    HI_RET_QOS_ERR_SPORT_MISMATCH       = (HI_RET_QOS_BASEVALUE | 0x0F  ),     // Դ�˿ڲ�ƥ��
    HI_RET_QOS_ERR_DPORT_MISMATCH       = (HI_RET_QOS_BASEVALUE | 0x10  ),     // Ŀ�Ķ˿ڲ�ƥ��
    HI_RET_QOS_ERR_PHYPORT_INVALID      = (HI_RET_QOS_BASEVALUE | 0x11  ),     // ����˿���Ч
    HI_RET_QOS_ERR_FLOW_NO_ACCEL        = (HI_RET_QOS_BASEVALUE | 0x12  ),     // ����Ӧ�ñ�����
    HI_RET_QOS_ERR_FLOW_NOTEXIST        = (HI_RET_QOS_BASEVALUE | 0x13  ),     // ��������
    HI_RET_QOS_ERR_NO_FREE_ITEM         = (HI_RET_QOS_BASEVALUE | 0x14  ),     // �����Ѿ�������
}hi_kernel_qos_ret_e;

#define HI_KERNEL_QOS_API_INVALID_FLOW_INDEX 0xFFFFFFFF  // �Ƿ���indexֵ

#define HI_KERNEL_QOS_API_MAX_QOS_CNT        128         // ���qos������

#define HI_KERNEL_QOS_API_MAX_ACCEL_CNT      960         // ���accel������

#define HI_KERNEL_QOS_API_EFC_MAX_VLAN_CNT   2           // efc��������vlan����


typedef enum
{
    HI_KERNEL_QOS_API_FWD_ACT_BRIDGE_E,
    HI_KERNEL_QOS_API_FWD_ACT_NAPT_E,
    HI_KERNEL_QOS_API_FWD_ACT_ROUTE_E,
    HI_KERNEL_QOS_API_FWD_ACT_CNT_E,     
}hi_kernel_qos_api_fwd_act_e; 


typedef struct
{
    hi_uint32   ui_phyport; 
    hi_uint32   ui_queue_id;

    hi_uint32   ui_schedmode; 
    hi_uint32   ui_weight; 
    hi_uint32   ui_shapping_en; 
    hi_uint32   ui_cir; 
    hi_uint32   ui_cbs; 
}hi_kernel_qos_api_queue_s; 


typedef struct
{
    hi_uint32   ui_phyport; 
    hi_uint32   ui_queue_id;
    hi_uint32   ui_in; 
    hi_uint32   ui_out; 
    hi_uint32   ui_drop_total;  
    hi_uint32   ui_drop_queue_tail;  
    hi_uint32   ui_drop_buffer_low;  
    hi_uint32   ui_drop_flow_wred;  
    hi_uint32   ui_drop_group_wred;  
    hi_uint32   ui_drop_desc_low; 
}hi_kernel_qos_api_queue_stat_s; 


typedef struct
{
    hi_uint32   ui_phyport; 
    hi_uint32   ui_queue_id;
    hi_uint32   ui_wred_id; 

    hi_uint32   ui_drop_thr0; 
    hi_uint32   ui_drop_thr1; 
    hi_uint32   ui_drop_thr2; 
    hi_uint32   ui_drop_thr3; 

    hi_uint32   ui_drop_p0; 
    hi_uint32   ui_drop_p1; 
    hi_uint32   ui_drop_p2; 
    hi_uint32   ui_drop_p3; 
}hi_kernel_qos_api_wred_s; 


typedef struct
{
    hi_uint32   ui_mask;
    hi_uint32   ui_phyport; 

    hi_uchar8   uc_start_smac[HI_MAC_LEN]; 
    hi_uchar8   uc_stop_smac[HI_MAC_LEN]; 
    hi_uchar8   uc_start_dmac[HI_MAC_LEN]; 
    hi_uchar8   uc_stop_dmac[HI_MAC_LEN]; 

    hi_ushort16 us_vid;
    hi_uchar8   uc_vpri;
    hi_uchar8   uc_rsv2; 
    
    hi_ushort16 us_ethtype; 
    hi_uchar8   uc_dscp;
    hi_uchar8   uc_ipproto; 

    hi_uint32   ui_start_sip; 
    hi_uint32   ui_stop_sip; 
    hi_uint32   ui_start_dip; 
    hi_uint32   ui_stop_dip; 

    hi_ushort16 us_sport; 
    hi_ushort16 us_dport; 
}hi_kernel_qos_api_flow_rule_s; 


typedef hi_int32 (*hi_kernel_qos_api_accel_callback)(hi_uint32 ui_phyport, hi_uint32 ui_enable); 

extern hi_uint32 hi_kernel_qos_api_accel_hook_reg(hi_kernel_qos_api_accel_callback pv_callback); 
extern hi_uint32 hi_kernel_qos_api_accel_flow_add(hi_kernel_sk_buff_s           *pst_skb, 
                                                  hi_uint32                      ui_mask, 
                                                  hi_kernel_qos_api_fwd_act_e    em_fwd, 
                                                  hi_uint32                     *pui_index); 
extern hi_uint32 hi_kernel_qos_api_accel_flow_del(hi_uint32 ui_flow_idx); 
extern hi_uint32 hi_kernel_qos_api_accel_route_label_get(hi_kernel_qos_api_fwd_act_e em_act, hi_uint32 *pui_label); 

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_QOS_API_H__ */
