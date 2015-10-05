/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_flow.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : FLOW ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��24��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

***************************************************************************** */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus  */
#endif /* __cpluscplus  */


#ifndef __HI_DRV_DP_FLOW_H__
#define   __HI_DRV_DP_FLOW_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#define HI_DP_EFC_TRAN_REASON_NUM 16
#define HI_DP_FLOW_POINT_FIELD_NUM        8

#define HI_DP_FLOW_FID_PCNT_NUM     3
#define HI_DP_FLOW_POINT_FIELD_NUM_V200  5
#define HI_DP_FLOW_TFRT_ITEM_NUM            16
#define HI_DP_FLOW_LABEL_PRI_MAX         31
#define HI_DP_FLOW_COLOR_MAX 7
#define HI_DP_FLOW_SFC_ENTRY_PRI_MAX 7

typedef enum{
    HI_DP_FLOW_G0_NO_USE_E = 0,   /* 0x0 */
    HI_DP_FLOW_G0_DMAC_H_E,       /* 0x1:32 bit */
    HI_DP_FLOW_G0_SMAC_H_E,       /* 0x2:32 bit */
    HI_DP_FLOW_G0_DIP_E,          /* 0x3:32 bit */
    HI_DP_FLOW_G0_SIP_E,          /* 0x4:32 bit */ 
    HI_DP_FLOW_G0_SVLAN_E,        /* 0x5:12 bit */
    HI_DP_FLOW_G0_ETH_TYPE_E,     /* 0x6:16 bit */
    HI_DP_FLOW_G0_IP_PROTOCAL_E,  /* 0x7: 8 bit */
    HI_DP_FLOW_G0_IP_VERSION_E,   /* 0x8: 4 bit */
    HI_DP_FLOW_G0_IP_TOS_E,       /* 0x9: 8 bit */
    HI_DP_FLOW_G0_SPORT_E,        /* 0xA:16 bit */
    HI_DP_FLOW_G0_DPORT_E,        /* 0xB:16 bit */
    HI_DP_FLOW_G0_PPPOE_PROTOCAL_E,/* 0xC:16 bit */
    HI_DP_FLOW_G0_ETH_ENCAPSULE_E, /* 0xD:16 bit */
    HI_DP_FLOW_G0_TAG_TYPE_E,      /* 0xE: 2 bit */
    HI_DP_FLOW_G0_S8021P_E,        /* 0xF: 3 bit */
    HI_DP_FLOW_G0_CVLAN_E,         /* 0x10:12 bit */
    HI_DP_FLOW_G0_GEM_PORT_E,      /* 0x11:12 bit */
    HI_DP_FLOW_G0_TAG_SUM_E,       /* 0x12: 2 bit */
    HI_DP_FLOW_G0_FRM_TYPE_E,      /* 0x13: 2 bit */
    HI_DP_FLOW_G0_POINT_FIELD0_E,  /* 0x14: 8 bit */
    HI_DP_FLOW_G0_POINT_FIELD1_E,  /* 0x15: 8 bit */
    HI_DP_FLOW_G0_POINT_FIELD2_E,  /* 0x16: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD3_E,  /* 0x17: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD4_E,  /* 0x18: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD5_E,  /* 0x19: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD6_E,  /* 0x1A: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD7_E,  /* 0x1B: 16 bit */
    HI_DP_FLOW_G0_C8021P_E,        /* 0x1C: 3 bit */
    HI_DP_FLOW_G0_PPP_ENCAP_E,     /* 0x1D: 1 bit */
    HI_DP_FLOW_G0_FC_ID_E,         /* 0x1E: 3 bit */
    HI_DP_FLOW_G0_SESSION_ID_E,    /* 0x1F:16 bit */
    HI_DP_FLOW_G0_IP_FRAG_E,       /* 0x20: 1 bit */ /* IP��Ƭ���ı�־ */
    
    HI_DP_FLOW_G0_IP_STPID_E,      /* 0x21:16 bit */
    HI_DP_FLOW_G0_IP_CTPID_E,      /* 0x22:16 bit */
    HI_DP_FLOW_G0_IP_IP_TYPE_E,    /* 0x23: 2 bit */  /* IP���ģ���IPv4oE��IPv6oE��IPv4oPPPoE��IPv6oPPPoE */
    HI_DP_FLOW_G0_SIPV6_96_TO_127_BIT_E,  /* 0x24:32 bit */     
    HI_DP_FLOW_G0_SIPV6_64_TO_95_BIT_E,   /* 0x25:32 bit */
    HI_DP_FLOW_G0_SIPV6_32_TO_63_BIT_E,   /* 0x26:32 bit */
    HI_DP_FLOW_G0_SIPV6_0_TO_31_BIT_E,    /* 0x27:32 bit */
    HI_DP_FLOW_G0_DIPV6_96_TO_127_BIT_E,  /* 0x28:32 bit */
    HI_DP_FLOW_G0_DIPV6_64_TO_95_BIT_E,   /* 0x29:32 bit */
    HI_DP_FLOW_G0_DIPV6_32_TO_63_BIT_E,   /* 0x2A:32 bit */
    HI_DP_FLOW_G0_DIPV6_0_TO_31_BIT_E,    /* 0x2B:32 bit */
    HI_DP_FLOW_G0_IPV6_TRAFFIC_CLASS_E,   /* 0x2C: 8 bit */
    HI_DP_FLOW_G0_IPV6_FLOW_LABEL_E,      /* 0x2D:20 bit */
    HI_DP_FLOW_G0_IPV6_NEXT_HEADER,       /* 0x2E: 8 bit */
    HI_DP_FLOW_G0_IPV6_EXTEND_NEXT_HEADER, /* 0x2F: 8 bit */
    HI_DP_FLOW_G0_IPV6_HOP_LIMIT_E,        /* 0x30: 8 bit */
    HI_DP_FLOW_G0_IPV4_TTL_E,              /* 0x31: 8 bit */
    HI_DP_FLOW_G0_IPV6_TUNNEL_E,           /* 0x32: 2 bit */    /* ��Ҫ����ö�٣��������DS-Lite��6RD */
    HI_DP_FLOW_G0_ICMPV6_TYPE_E,           /* 0x33: 8 bit */
    HI_DP_FLOW_G0_DMAC_TYPE_E,             /* 0x34: 48 bit */
    HI_DP_FLOW_G0_SMAC_TYPE_E,             /* 0x35: 48 bit */
    HI_DP_FLOW_G0_DIPV6_TYPE_E,           /* 0x36: 128 bit */
    HI_DP_FLOW_G0_SIPV6_TYPE_E,           /* 0x37: 128 bit */
#ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_DP_FLOW_G0_CDEI_E,                       /*0X38: 1bit*/
    HI_DP_FLOW_G0_SDEI_E,                       /*0X39: 1bit*/
#endif
    HI_DP_FLOW_G0_TYPE_MAX_E,
}HI_DP_FLOW_CHARACTER_G0_E;

/* extend��ƥ���������1 */
typedef enum{
    HI_DP_FLOW_G1_NO_USE_E = 0,   /* 0x0 */
    HI_DP_FLOW_G1_DMAC_L_E,       /* 0x1:16 bit */
    HI_DP_FLOW_G1_SMAC_L_E,       /* 0x2:16 bit */
    HI_DP_FLOW_G1_DIP_E,          /* 0x3:32 bit */
    HI_DP_FLOW_G1_SIP_E,          /* 0x4:32 bit */ 
    HI_DP_FLOW_G1_SVLAN_E,        /* 0x5:12 bit */
    HI_DP_FLOW_G1_ETH_TYPE_E,     /* 0x6:16 bit */
    HI_DP_FLOW_G1_IP_PROTOCAL_E,  /* 0x7: 8 bit */
    HI_DP_FLOW_G1_IP_VERSION_E,   /* 0x8: 4 bit */
    HI_DP_FLOW_G1_IP_TOS_E,       /* 0x9: 8 bit */
    HI_DP_FLOW_G1_SPORT_E,        /* 0xA:16 bit */
    HI_DP_FLOW_G1_DPORT_E,        /* 0xB:16 bit */
    HI_DP_FLOW_G1_PPPOE_PROTOCAL_E,/* 0xC:16 bit */
    HI_DP_FLOW_G1_ETH_ENCAPSULE_E, /* 0xD:16 bit */
    HI_DP_FLOW_G1_TAG_TYPE_E,      /* 0xE: 2 bit */
    HI_DP_FLOW_G1_S8021P_E,        /* 0xF: 3 bit */
    HI_DP_FLOW_G1_CVLAN_E,         /* 0x10:12 bit */
    HI_DP_FLOW_G1_GEM_PORT_E,      /* 0x11:12 bit */
    HI_DP_FLOW_G1_TAG_SUM_E,       /* 0x12: 2 bit */
    HI_DP_FLOW_G1_FRM_TYPE_E,      /* 0x13: 2 bit */
    HI_DP_FLOW_G1_POINT_FIELD0_E,  /* 0x14: 8 bit */
    HI_DP_FLOW_G1_POINT_FIELD1_E,  /* 0x15: 8 bit */
    HI_DP_FLOW_G1_POINT_FIELD2_E,  /* 0x16: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD3_E,  /* 0x17: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD4_E,  /* 0x18: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD5_E,  /* 0x19: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD6_E,  /* 0x1A: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD7_E,  /* 0x1B: 16 bit */
    HI_DP_FLOW_G1_C8021P_E,        /* 0x1C: 3 bit */
    HI_DP_FLOW_G1_PPP_ENCAP_E,     /* 0x1D: 1 bit */
    HI_DP_FLOW_G1_FC_ID_E,         /* 0x1E: 3 bit */
    HI_DP_FLOW_G1_SESSION_ID_E,    /* 0x1F:16 bit */
    HI_DP_FLOW_G1_IP_FRAG_E,       /* 0x20: 1 bit */ /* IP��Ƭ���ı�־ */

    HI_DP_FLOW_G1_IP_STPID_E,      /* 0x21:16 bit */
    HI_DP_FLOW_G1_IP_CTPID_E,      /* 0x22:16 bit */
    HI_DP_FLOW_G1_IP_IP_TYPE_E,    /* 0x23: 2 bit */  /* IP���ģ���IPv4oE��IPv6oE��IPv4oPPPoE��IPv6oPPPoE */
    HI_DP_FLOW_G1_SIPV6_96_TO_127_BIT_E,  /* 0x24:32 bit */     
    HI_DP_FLOW_G1_SIPV6_64_TO_95_BIT_E,   /* 0x25:32 bit */
    HI_DP_FLOW_G1_SIPV6_32_TO_63_BIT_E,   /* 0x26:32 bit */
    HI_DP_FLOW_G1_SIPV6_0_TO_31_BIT_E,    /* 0x27:32 bit */
    HI_DP_FLOW_G1_DIPV6_96_TO_127_BIT_E,  /* 0x28:32 bit */
    HI_DP_FLOW_G1_DIPV6_64_TO_95_BIT_E,   /* 0x29:32 bit */
    HI_DP_FLOW_G1_DIPV6_32_TO_63_BIT_E,   /* 0x2A:32 bit */
    HI_DP_FLOW_G1_DIPV6_0_TO_31_BIT_E,    /* 0x2B:32 bit */
    HI_DP_FLOW_G1_IPV6_TRAFFIC_CLASS_E,   /* 0x2C: 8 bit */
    HI_DP_FLOW_G1_IPV6_FLOW_LABEL_E,      /* 0x2D:20 bit */
    HI_DP_FLOW_G1_IPV6_NEXT_HEADER,       /* 0x2E: 8 bit */
    HI_DP_FLOW_G1_IPV6_EXTEND_NEXT_HEADER, /* 0x2F: 8 bit */
    HI_DP_FLOW_G1_IPV6_HOP_LIMIT_E,        /* 0x30: 8 bit */
    HI_DP_FLOW_G1_IPV4_TTL_E,              /* 0x31: 8 bit */
    HI_DP_FLOW_G1_IPV6_TUNNEL_E,           /* 0x32: 2 bit */    /* ��Ҫ����ö�٣��������DS-Lite��6RD */
    HI_DP_FLOW_G1_ICMPV6_TYPE_E,           /* 0x33: 8 bit */

    HI_DP_FLOW_G1_DMAC_TYPE_E,             /* 0x34: 48 bit */
    HI_DP_FLOW_G1_SMAC_TYPE_E,             /* 0x35: 48 bit */
    HI_DP_FLOW_G1_DIPV6_TYPE_E,           /* 0x36: 128 bit */
    HI_DP_FLOW_G1_SIPV6_TYPE_E,           /* 0x37: 128 bit */

#ifdef  CONFIG_HISI_SD5115_TYPE_T
    HI_DP_FLOW_G1_CDEI_E,                       /*0X38: 1bit*/
    HI_DP_FLOW_G1_SDEI_E,                       /*0X39: 1bit*/
#endif
    HI_DP_FLOW_G1_TYPE_MAX_E,
    
}HI_DP_FLOW_CHARACTER_G1_E;


/* �̶���FLOW �����ǩ,��������ָʾ, ��Ҫ��ϸ��Ӧ�ù淶���� */
typedef enum
{
    /* lebel 0 to 5 ΪACT������ǩ */
    HI_DP_FLOW_LABEL_0_E = 0,        
    HI_DP_FLOW_LABEL_1_E,            
    HI_DP_FLOW_LABEL_2_E,
    HI_DP_FLOW_LABEL_3_E,
    HI_DP_FLOW_LABEL_4_E,              
    HI_DP_FLOW_LABEL_5_E,               

    /* lebel 6 to 7 Ϊ��ACT������ǩ */    
    HI_DP_FLOW_LABEL_6_E = 6,           
    HI_DP_FLOW_LABEL_7_E,

}HI_DP_FLOW_LABEL_E;


typedef enum
{
    HI_DP_FLOW_EFC_ACT_PRI_8021P_E = 0, /* 802.1P */
    HI_DP_FLOW_EFC_ACT_PRI_DSCP_E,      /* DSCP */ 

}HI_DP_FLOW_EFC_ACT_PRI_MODE_E;

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_PKT_TYPE_UC_E = 0,
    HI_DP_PKT_TYPE_UUC_E,
    HI_DP_PKT_TYPE_MC_FWD_E,
    HI_DP_PKT_TYPE_UMC_E,
    HI_DP_PKT_TYPE_BC_E,
    HI_DP_PKT_TYPE_FWD_E, 
}HI_DP_PKT_TYPE_E;
#endif
typedef struct 
{

    /* ƥ����0 */
    HI_DP_FLOW_CHARACTER_G0_E    em_g0;         /* ��ѡ����0          */
    uint    ui_key0;                            /* ��ӦG0��ƥ����     */ 
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint    ui_mask0;                           /* K0���mask         */
    #endif
	    
    HI_DP_MATCH_MODE_E   em_g0_match_mode;
    

    /* ƥ����1 */    
    HI_DP_FLOW_CHARACTER_G1_E    em_g1;         /* ��ѡ����1          */
    uint    ui_key1;                            /* ��ӦG1��ƥ����     */
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint    ui_mask1;                           /* K1���mask         */
    #endif
	
    HI_DP_MATCH_MODE_E   em_g1_match_mode;

    hi_dp_port_u    u_igr;

    uchar uc_mac[HI_DP_MAC_BYTE_NUM];         /* ��G0Ϊ0x34/0x35ʱ��Ч */
    uchar uc_ip[HI_DP_IPV6_ADDR_BYTE_NUM];    /* ipv6��ַ;��G0Ϊ0x36/0x37ʱ��Ч */
    

}hi_dp_flow_efc_rule_s;


typedef struct 
{
    /* tag  ��Ϊ */
    HI_DP_TAG_ACT_E em_tag_act;             /* ����TAG����           */
        
    /* vlan   ��Ϊ */
    HI_DP_VALN_ACT_E    em_cvlan_act;       /* �ڲ�VLAN�Ĳ���        */   
    uint    ui_cvlan_value;                 /* �ڲ�VLAN����ֵ  */
    HI_DP_VALN_ACT_E    em_svlan_act;       /* ���VLAN�Ĳ���            */       
    uint    ui_svlan_value;                 /* ���VLAN�Ĳ���ֵ       */ 

    /* pri ��Ϊ */
    HI_DP_8021P_ACT_E   em_cpri_act;        /* �ڲ����ȼ��Ĳ���          */       
    uint    ui_cpri_value;                  /* �ڲ����ȼ��Ĳ���ֵ        */

    HI_DP_8021P_ACT_E   em_s8021p_act;      /* ���802.1p�Ĳ���          */        
    uint    ui_s8021p_value;                /* ���802.1p�Ĳ���ֵ        */

    /* ת����Ϊ */
    HI_DP_TRANS_ACT_E   em_trans_act ;      /* ����ת����Ϊ,������NAT,NAPTת��ָʾ */

    hi_dp_port_u    u_trans_egrs;           /*ָ��ת��,��˿�*/  

    uint            ui_trans_reason;                /* ת��ԭ��,��Ҫ���ڲ���CPU�ڵ���� */
    
    
    /* ָ��L3ģ���NAPT/NAT ���ٱ����� */
    uint    ui_l3_rep_index;                /* NAT/ROUTE�滻������,NAPT����Ҫ,        */
                                            /* NAT / ROUTE ���ݴ�������ѯL3��SUBNET����ȷ���滻����
                                            ��NAPT��ֱ�Ӳ�ѯSUBNET����IP,PORTΪƥ���� */
                                            /* SD5113V200��չΪ8bit��0-255 */

    /* VLAN  mark (���)��Ϊ */
    HI_BOOL_E   em_vlan_mark_en;            /* VLAN markingʹ��           */    
    uint        ui_vlan_mark_value;             /* VLAN markingֵ             */
	

    /* PRI  mark ��Ϊ */    
    HI_BOOL_E   em_pri_mark_en;             /* PRI markingʹ��            */    
    uint    ui_pri_mark_value;              /* PRI markingֵ              */
    HI_DP_PRI_MODE_E  em_pri_mode;           /* ������ѡ��priģʽ������ֵ = ö�ٶ��� + 1 */

    /* CAR ����ָʾ Ĭ��ΪTRUE */
    HI_BOOL_E   em_car_en;                  /* ����CAR��ʶ        CAR��QOSģ��     */
    
    /* ������ARL TAG����ʹ�� */    
    HI_BOOL_E   em_arl_fid_en;              /* ARL FID����ʹ��         */  

    /*  ����V100 ·�ɡ�NAT/NAPT ʱ����ʹ�õ�pppoe_add_en��־��
          V200����ʹ�ã�Ϊ�˼��ݲ�ƷV100���룬��ʱ���ñ��������İ취��
          ��������ɾ�� */
    HI_BOOL_E   em_pppoe_add_en;

    /* ��������MAC��ַѧϰʹ�� */   
    HI_BOOL_E   em_mac_learn_en;            /* ������ѧϰMACʹ��           */

    /* TC/FL�޸� */
    /*��ͨ���Ĳ�tfrt_rep��,������Ĳ�tunnel_tfrt_rep��*/
    HI_BOOL_E   em_tcfl_rep_en;            /* TC/FL ָ���޸�ʹ�� */
    uint        ui_tfrt_index;             /* TFRT �������*/

    /* ��������car, ECAR */
    HI_BOOL_E    em_ecar_en;                 /* ECARʹ�� */
    uint         ui_ecar_id;              /* ECAR ID,ECAR������� */


    HI_DP_TPID_REP_MODE_E    em_stpid_rep_mode;    /* ���tpid�޸�ģʽ */
    HI_DP_TPID_REP_MODE_E    em_ctpid_rep_mode;    /* �ڲ�tpid�޸�ģʽ */

    HI_DP_VLAN_MODE_E   em_trans_vlan_mode;    /* ��������ת��vlanģʽ */
    HI_DP_VLAN_MODE_E   em_map_vlan_mode;    /* ��������ӳ��vlanģʽ */

    uint         ui_color;         /* ������ɫ */ 

    HI_BOOL_E    em_statis_en;        /* ������ʹ�� */
    uint         ui_statis_id;        /* ��ͳ�Ʊ����� */
 
}hi_dp_flow_efc_act_s;


typedef struct 
{
    /* ���� : ��Ӧfmap�������� */
    uint    ui_index;
    
    uint    ui_fid;                         /* 0 - 404 */

    HI_BOOL_E   em_valid;                   /* ������Ч��־ */   
    HI_BOOL_E   em_eof;                     /* �����������־�� */    

    /* ƥ����� */
    hi_dp_flow_efc_rule_s   st_rule;        /* ƥ����� */

    /* �������ָʾ */
    HI_DP_FLOW_LABEL_E      em_label;       /* �����ǩ������ȷ������������   */
    uint                    ui_label_pri;  /* �������ȼ�������ͬһLabel����Ч */

    /* TAG�ȷ��������Ϊ */    
    hi_dp_flow_efc_act_s    st_act;         /* ��Ӧ��Ϊ */

}hi_dp_flow_efc_table_item_s;




/* ����128bit */
typedef struct 
{
    /* ���� */
    uint   ui_sfid;                                 /* ����ID, ����,����ID, Ϊ����MAP��CAR������ID, 0-127 */

    HI_BOOL_E    em_map_valid;                          /* sfid����ӳ����Ч��־ */ 
    HI_BOOL_E    em_qos_valid;                          /* sfid����qos ��Ч��־ */ 

    /* ������IDƥ����� */
    HI_BOOL_E   em_fid_match_en;     
    uint        ui_fid;                                  /* ��ʾFID��ƥ���ӦEFC ID */

    /* ����VLANƥ����� */
    HI_BOOL_E                 em_vlan_match_en;     /* vlan��Ч��ʶ */
    HI_DP_MATCH_MODE_E   em_vlan_match_mode;   /* vlanƥ�䷽ʽ�����FID��Ч���ʾFID��ƥ�� */
    uint                      ui_vlan;              /* vlanֵ */

    /* ����PRIƥ����� */
    HI_BOOL_E                 em_pri_match_en;      /* pri��Ч��ʶ */    
    HI_DP_MATCH_MODE_E   em_pri_match_mode;    
    uint                      ui_pri;               /* priֵ */

    /* ���ڶ˿�ƥ����� */
    hi_dp_port_u u_igr;

    HI_BOOL_E    em_eth_type_match_en;                   /* eth_type��Ч��ʶ */
    HI_DP_MATCH_MODE_E   em_eth_type_match_mode;    /* eth_typeƥ�䷽ʽ */       
    uint         ui_eth_type;                            /* eth_type�� */    

    uint     ui_entry_pri;         /* �������ȼ������ڶ������ͬʱƥ��ʱ�����ȼ��ߵ���Ч */

    /* �����CARʹ�� */
    HI_BOOL_E   em_iprf_tp_en;

}hi_dp_flow_sfc_table_item_s;

/* ��ͳ�Ʊ� */
typedef struct
{
    uint    ui_statis_id;

    
}hi_dp_flow_statis_item_s;


/* 
0x0������ȡ������μ�EFC�����ࡣ
0x1��MACͷ
0x2��IPͷ
0x3��TCP/UDPͷ

 */
typedef enum
{
    HI_DP_FLOW_POINT_NO_FIELD = 0,
    HI_DP_FLOW_POINT_MAC_HEAD,              /* ָ��MACͷ */
    HI_DP_FLOW_POINT_IP_HEAD,
    HI_DP_FLOW_POINT_TCP_UDP_HEAD,
}HI_DP_FLOW_POINT_FIELD_E;


typedef struct 
{
    HI_DP_FLOW_POINT_FIELD_E em_field_base; 
    uint ui_field_offset;
    hi_dp_mask_u   u_mask;      

}hi_dp_flow_point_field_s;



typedef struct 
{
    
    HI_BOOL_E   em_enable;          /* ָ��ͳ��ʹ�� */
    uint      ui_fid;              /* ָ����Ҫͳ�Ƶ�FID */

}hi_dp_flow_fid_pcnt_s;




/* 
IRULE���Ϊ4���ӱ�ÿ���ӱ���2�������򣬹�֧��8��������
4��IRULE�ӱ�8����ͬʱ��ƥ�䣬�涨1���ӱ����ֻ����һ������ƥ�䣬��ô������4����ͬʱƥ�䡣�����ѡȡ���ȼ���ߵ�����ʶ��Ϊ����ƥ������
�����ӱ������ȼ�ѡ������ʶ���涨�ӱ�0�����ȼ���ߣ�����Ϊ�ӱ�1���ӱ�2���ӱ�3�����ȼ���ͣ�������ͬʱƥ��ʱ���Ը����ȼ���Ϊ����ʶ��

 */
#define HI_DP_FC_TABLE_SIZE     3

#define HI_DP_FC_MATCH_BYTE_NUM 20

typedef struct 
{
    uint    ui_fcid;               /* 0~2,��֧��3���� */

    uint    ui_imask[HI_DP_FC_MATCH_BYTE_NUM];           /* ѡ���0~63 �ֽ�, ����ѡ�� 20�� BYTE */  
    uint    ui_bitmask[HI_DP_FC_MATCH_BYTE_NUM];         /* ѡ�����20��BYTE�� ui_bitmask, 5 * 4BYTE(32bit) */  

    /* ÿ���ӱ������������������, ÿ�������ƥ�����5 * 4BYTE ��Ӧ */
    uint    ui_irule_0[HI_DP_FC_MATCH_BYTE_NUM];         /* ƥ�����1ʹ�� */    
    HI_BOOL_E       em_irule_0_en;                       /* ƥ�����1ʹ�� */   
    hi_dp_port_u u_0_igr;
    HI_DP_PORT_E    em_irule_0_igr;                      /* ���ձ��ĵ�Դ�˿�ƥ�� */
    uint    ui_irule_1[HI_DP_FC_MATCH_BYTE_NUM];         /* ƥ�����1ʹ�� */       
    HI_BOOL_E       em_irule_1_en;                       /* ƥ�����1ʹ�� */        
    hi_dp_port_u u_1_igr;
    HI_DP_PORT_E    em_irule_1_igr;                      /* ���ձ��ĵ�Դ�˿�ƥ�� */

    HI_BOOL_E       em_valid;                            /* ������Ч��־ */ 

}hi_dp_flow_fc_table_item_s;



typedef struct
{
    uint       ui_index;          /* ������0-15 */
    HI_BOOL_E  em_valid;        

    HI_BOOL_E  em_fl_rep_en;     /* ����FL�޸�ʹ�� */
    uint       ui_fl;             /*����FL�޸�ֵ */

    HI_DP_TOS_MODE_E    em_tos_mode;      /* ����TOS��ѡ�� */
    HI_BOOL_E           em_tos_rep_en;    /* ����TOS�޸�ʹ�� */
    uint                ui_tos;           /* ����TOS�޸�ֵ */
    
    
}hi_dp_flow_tcfl_rep_table_item_s;

typedef struct
{
    uint     ui_statis_id;

    ulong    ul_frame;     /*�����ͳ��ʱΪƥ���ϵ�  ������������ͳ��ʱΪ��ӳɹ��İ���*/
    #ifdef CONFIG_HISI_SD5115_MP_MPW
    uint     ui_byte;
    #endif
    #ifdef CONFIG_HISI_SD5115_MP_PILOT   
    ulong    ul_byte;       /*������ͳ����Ϊƥ���ϵ��ֽ�����������ͳ��ʱΪ��ӳɹ����ֽ���*/
    ulong    ul_drop_frame;/*������ͳ��ʱ�����İ���*/
    ulong    ul_drop_byte;  /*������ͳ��ʱ�������ֽ���*/
    #endif
	
}hi_dp_flow_cnt_table_item_s;

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef struct 
{
  
    uint  ui_statis_id;
   
    HI_BOOL_E em_valid;
    
    HI_BOOL_E em_vlan_en;
    uint  ui_vlan;

    HI_BOOL_E em_egr_en;
    HI_DP_PORT_E em_egr;

    HI_BOOL_E em_gemport_en;   
    uint ui_gemport;
    
    HI_BOOL_E em_tcont_en;
    uint ui_tcont;

    HI_BOOL_E em_pri_en;
    uint ui_pri;

    HI_BOOL_E em_fwd_type_en;
    HI_DP_PKT_TYPE_E em_fwd_type;
    
}hi_dp_flow_cnt_match_table_item_s;
#endif

typedef struct 
{
    uint ui_trans_reason_drop;      /* �ض�ת��ԭ�������� */
    uint ui_trans_reason_thru;      /* �ض�ת��ԭ��͸������ */
    uint ui_trans_reason_tocpu;     /* �ض�ת��ԭ��CPU���� */
    
}hi_dp_efc_trans_reason_cnt_s;

typedef struct 
{
    
    uint ui_efc_unhit;              /* ��EFC��������û�в����κ�ƥ�䱨�ĸ���ͳ��                     */
    uint ui_efc_hit_act;            /* EFC����ƥ���ִ��ACTION��LABEL Ϊ0��5���ı��ĸ���             */
    uint ui_efc_hit_map;            /* EFC����ƥ�䣬LABEL ֵΪGEM PORT ӳ���LABEL ֵ�ı��ĸ���ͳ��  */
    
    uint ui_efc_fid0;               /* EFCƥ��ָ��FID0���ĸ���ͳ��       */
    uint ui_efc_fid1;               /* EFCƥ��ָ��FID1���ĸ���ͳ��       */
    uint ui_efc_fid2;               /* EFCƥ��ָ��FID2���ĸ���ͳ��       */
    
    uint ui_efc_rmk_svlan;          /* EFCƥ��ָ��SVLAN Remarkingͳ��           */
    uint ui_efc_rmk_cvlan;          /* EFCƥ��ָ��CVLAN Remarkingͳ��           */
    uint ui_efc_rmk_spri;           /* EFCƥ��ָ��SPriority Remarkingͳ��       */
    uint ui_efc_rmk_cpri;           /* EFCƥ��ָ��CPriority Remarkingͳ��       */
    uint ui_efc_add_tag;            /* EFC����ƥ��ִ��ACTION Ϊ���Tag ����ͷ�ı��ĸ���ͳ��  */
    uint ui_efc_del_tag;            /* EFC����ƥ��ִ��ACTION Ϊɾ��Tag ����ͷ�ı��ĸ���ͳ��  */
    uint ui_pcnt_rmk_svlan;         /* ָ��SVLAN Remarking �ı�����Ŀͳ�ƣ���SVLAN
                                       ֵ������em_svlan_rmk_pcnt_en ָ������������
                                       ���ó�ʹ��״̬ʱ�����ҽ���SVLAN Remarking ����
                                       �ı��ĵ��޸ĺ��SVLAN ��������ui_pcnt_svlan���õ�ֵʱ����ͳ���м����� */
    uint ui_pcnt_rmk_cvlan;
    uint ui_pcnt_rmk_spri;
    uint ui_pcnt_rmk_cpri;
    
    uint ui_efc_tocpu;              /* EFC����ƥ��ִ��ACTION ΪTOCPU �ı��ĸ���ͳ��          */
    uint ui_efc_drop;               /* EFC����ƥ��ִ��ACTION Ϊ�����ı��ĸ���ͳ��            */
    uint ui_efc_indicate_fwd;       /* EFC����ƥ��ִ��ACTION Ϊָ��ת������  */
    
    uint ui_efc_route;              /* EFC����ƥ��ִ��ACTION ΪRouter/NAT �ı��ĸ���ͳ��     */
    uint ui_efc_nat;                /* EFC����ƥ��ִ��ACTION ΪRouter/NAT �ı��ĸ���ͳ��     */
    uint ui_efc_napt;               /* EFC����ƥ��ִ��ACTION ΪNAPT �ı��ĸ���ͳ��           */
    uint ui_efc_add_pppoe;          /* EFC����ƥ��ִ��ACTION Ϊ���PPPoE ����ͷ�ı��ĸ���    */
    
    uint ui_efc_multi_match;        /* EFC�������ж���������ı��ĸ���ͳ��                   */

    uint ui_sfc_unhit;              /* SFC�������ࣨCLASSIFY��ʧ�ܵı��ĸ��� */
    uint ui_sfc_fail_drop;          /* ���м�������ʧ�ܶ���  */

    /* �ض�ת��ԭ����� */
    hi_dp_efc_trans_reason_cnt_s st_tran_reason[HI_DP_EFC_TRAN_REASON_NUM];

#ifdef  CONFIG_HISI_SD5115_TYPE_T
    uint ui_efc_hit_pkt;              /* ��EFC��������ƥ�䱨�ĸ���ͳ�� */
#endif

    uint ui_rtp;                    /* ��Ƶ��ͳ�� */

}hi_dp_flow_cnt_s;

typedef enum
{
    HI_DP_FLOW_RTP_CAP_E = 0,
    HI_DP_FLOW_RTP_EFC_E,    
    
}HI_DP_FLOW_RTP_SEL_E;


typedef struct 
{
  
    /* 8���Զ�����, ��������ƥ��ѡ�� */
     hi_dp_flow_point_field_s    st_point_field[HI_DP_FLOW_POINT_FIELD_NUM];


    HI_DP_FLOW_EFC_ACT_PRI_MODE_E   em_efc_cpri_sel;  /* CPRI ѡ��       */
    HI_DP_FLOW_LABEL_E              em_map_label;     /* ����ӳ���label */
    HI_DP_FLOW_LABEL_E              em_car_label;     /* ����car��label */

    /* ������������ */
    HI_DP_FLOW_RTP_SEL_E em_rtp_sel;  /* ��Ƶ�������EFC\CAP��ʽ */
    HI_DP_L4_PROTOCOL_E em_rtp_protocol;    /* ������Э�� */
    uint ui_rtp_dip;
    uint ui_rtp_sip;
    uint ui_rtp_dport;   
    uint ui_rtp_sport;       
	
   #ifdef CONFIG_HISI_SD5115_MP_PILOT
   #ifndef CONFIG_HISI_SD5115_TYPE_T
   HI_DP_CNT_MODE_E em_igr_cnt_mode;
   #endif
   HI_DP_CNT_MODE_E em_egr_cnt_mode;
   #endif

}hi_dp_flow_attr_s;


typedef struct
{
    uint cnt[HI_DP_TOCPU_NUM_E];
} hi_dp_tocpu_reason_cnt_s;

typedef struct
{
    uint cnt[HI_DP_THRU_NUM_E];
} hi_dp_thru_reason_cnt_s;



#endif /* end of __HI_DRV_DP_FLOW_H__ */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


