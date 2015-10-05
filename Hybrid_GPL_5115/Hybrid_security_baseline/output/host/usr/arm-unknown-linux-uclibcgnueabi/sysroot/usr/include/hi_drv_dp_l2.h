/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_flow.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : L2 ģ�����ݽṹͷ�ļ�
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


#ifndef __HI_DRV_DP_L2_H__
#define    __HI_DRV_DP_L2_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


#define HI_DP_L2_BC_TABLE_ITEM_NUM          4608          
#define HI_DP_L2_MC_TABLE_ITEM_NUM          1024
#define HI_DP_L2_USER_TABLE_ITEM_NUM        32
#define HI_DP_L2_VLAN_TABLE_ITEM_NUM        4096
#define HI_DP_L2_ACT_TABLE_ITEM_NUM         192
#define HI_DP_L2_MAC_TABLE_ITEM_NUM         1024

// h and t used only
//#if defined (CONFIG_HISI_SD5115_TYPE_H) || defined (CONFIG_HISI_SD5115_TYPE_T)
#define HI_DP_L2_MAC_TABLE_ITEM_NUM_16K_MODE         16384
#define HI_DP_L2_MAC_TABLE_MAX_ITEM_NUM         16392  
//#endif

#define HI_DP_L2_MAC_ESP_TABLE_ITEM_NUM         8

#define HI_DP_L2_MC_NUM    256

#ifdef CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_L2_SPECIFY_MC_NUM   8
#endif

/* 
�鲥�������ģʽ
ֻ��MAC���
0��VLAN+MAC���
 */
typedef enum{
    HI_DP_L2_MATCH_MAC_VLAN_E = 0,
    HI_DP_L2_MATCH_MAC_E,
}HI_DP_L2_MATCH_MODE_E;


/* MACѧϰģʽ */
typedef enum
{
    HI_DP_L2_MAC_LOGIC_LEARN_FROM_PORT_E = 0,   /* �߼����ڶ˿�ѧϰģʽ;        */
    HI_DP_L2_MAC_LOGIC_LEARN_FROM_FLOW_E,       /* �߼�������ѧϰģʽ;        */

    HI_DP_L2_MAC_CPU_LEARN_FROM_PORT_E,         /* CPU������ڶ˿�ѧϰģʽ;        */
    HI_DP_L2_MAC_CPU_LEARN_FROM_FLOW_E,         /* CPU���������ѧϰģʽ;        */    
}HI_DP_L2_MAC_LEARN_MODE_E;

/* L3�鲥ƥ��ģʽ */
typedef enum{
    HI_DP_L2_IP_MC_MATCH_MODE_L2_E = 0,                 /* ����v120 */
    HI_DP_L2_IP_MC_MATCH_MODE_GROUP_E,                  /* (*,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_VLAN_GROUP_E,            /* (V,*,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_SOURCE_GROUP_E,          /* (*,S,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_VLAN_SOURCE_GROUP_E,    /* (V,S,G) */
}HI_DP_L2_IP_MC_MATCH_MODE_E;


typedef struct
{

    HI_BOOL_E         em_ipv4_smac_rep_en;    /* IPv4�鲥SMAC�滻ģʽ��TX_MC_BASE_CFG_REG */
    HI_BOOL_E         em_ipv6_smac_rep_en;    /* IPv6�鲥SMAC�滻ģʽ��TX_MC_BASE_CFG_REG */

    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_match_mode;                /* IPv4OE������鲥ƥ��ģʽ */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_match_mode;                /* IPv6OE������鲥ƥ��ģʽ */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_ppp_match_mode;            /* Ipv6oPPPoE����鲥ƥ��ģʽ�������ONT���ţ� */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_ppp_match_mode;            /* Ipv4oPPPoE����鲥ƥ��ģʽ�������ONT���ţ� */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_in_tunnel_match_mode;      /* IP����鲥ƥ��ģʽ�����Գ�IP���������Ч�� */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_in_tunnel_match_mode;      /* IP���(6RD)�鲥ƥ��ģʽ�����Գ�IP���������Ч�� */
}hi_dp_l2_mc_attr_s;

/* BEGIN: Added by w00180499, 2012/7/20 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
/* ѧϰ���ض˿ڶ��� */
typedef union
{
    struct
    {
        uint eth0 : 1;      /* [0] */
        uint eth1 : 1;      /* [1] */
        uint eth2 : 1;      /* [2] */

        uint eth3 : 1;      /* [3] */
        uint eth4 : 1;      /* [4] */
        uint pon  : 1;      /* [5] */
        uint cpu  : 1;      /* [6] */
        uint prbs : 1;      /* [7] */
        uint reserved : 24;
    } bits;

    uint value;
} hi_dp_l2_learn_local_u;

/* ARL ȫ�ֺͱ���ѧϰ��Ŀ���� */
typedef struct
{
    HI_BOOL_E              em_glb_lmt_en;               /* ȫ�ֶ˿��Զ�ѧϰ�±�����Ŀ����ʹ�� */
    HI_BOOL_E              em_glb_lrn_cnt_clr_en;      /* ȫ�ֶ˿�ѧϰ��Ŀ�������� */
    uint                   ui_glb_lmt_num;              /* ȫ�ֶ˿��Զ�ѧϰ�±�����Ŀ */

    HI_BOOL_E              em_local_lmt_en;             /* ���ض˿��Զ�ѧϰ�±�����Ŀ����ʹ�� */
    HI_BOOL_E              em_local_lrn_cnt_clr_en;    /* ���ض˿�ѧϰ��Ŀ�������� */
    uint                   ui_local_lmt_num;            /* ���ض˿��Զ�ѧϰ�±�����Ŀ */
    hi_dp_l2_learn_local_u u_lrn_local;                  /* ѧϰ���ض˿ڶ��� */
}hi_dp_l2_learn_lmt_attr_s;

typedef struct
{
    uint ui_index;
    uint ui_vlan;
    HI_BOOL_E em_vlan_en;
    uchar     uc_mac[6];
}hi_dp_l2_mc_st_attr_s;
#endif
/* END:   Added by w00180499, 2012/7/20 */

/* L2�������� */
typedef struct  
{    
    /* ת������ */  
    /* MACѧϰ������� */
    HI_DP_L2_MAC_LEARN_MODE_E   em_mac_learn_mode;          /* MACѧϰģʽ */    
    HI_DP_TRANS_ACT_E   em_mac_learn_fail_act;              /* MACѧϰʧ�ܺ����Ӧ����ת����Ϊ */  

    //5115 add;����prbs��
    HI_BOOL_E   em_mac_learn_en[HI_DP_PORT_NUM+1];            /* ���˿�MACѧϰʹ�� */    
    HI_BOOL_E   em_mac_learn_limit_en[HI_DP_PORT_NUM+1];      /* ���˿�MACѧϰ��Ŀ����ʹ�� */
    uint        ui_mac_learn_limit_num[HI_DP_PORT_NUM+1];     /* ���˿�MACѧϰ��Ŀ���� */

    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_BOOL_E   em_port_stat_en[HI_DP_PORT_NUM];                /*�˿�ͳ�Ƶ�״̬ʹ��*/              
    HI_DP_CNT_MODE_E   em_port_stat_type[HI_DP_PORT_NUM]; /*�˿�ͳ�Ƶ�ͳ������*/

    HI_BOOL_E em_sw_uuc_thru;                     /*���뽻��ģʽ�£�δ֪��������Ϊ*/
    HI_BOOL_E em_sw_umc_thru;                     /*���뽻��ģʽ�£�δ֪�鲥����Ϊ*/
    #endif
	
    /* �ϻ����� */
    HI_BOOL_E    em_mac_age_en;                             /* MAC���ϻ�ʹ�� */
    uint ui_mac_age_time;          

    /* ���ģʽ */  
    HI_DP_L2_MATCH_MODE_E   em_mc_match_mode;            /* �鲥�������ģʽ */
    HI_DP_L2_MATCH_MODE_E   em_uc_match_mode;            /* �����������ģʽ */

    /* L2ת��(���)ʧ�ܴ���  */
    HI_DP_TRANS_ACT_E   em_uc_unhit_act;                    /* �������ʧ�ܴ��� */   /* 5115����ѡ�� */
    HI_DP_TRANS_ACT_E   em_tunnel_mc_unhit_act;                  /* ����ಥ���ʧ�ܴ��� */ 
    
    /* �㲥���鲥ѡ��ת��������֧��:HI_DP_TRANS_BC_E��HI_DP_TRANS_BC_BY_VLAN_E */
    HI_DP_TRANS_ACT_E   em_bc_act;
    HI_DP_TRANS_ACT_E   em_mc_act;
    
    /* ����gemport��ip version�ֱ������鲥���ʧ�ܵĴ��� */
    HI_DP_TRANS_ACT_E    em_pon_mc_unhit_act[HI_DP_IP_VERSION][HI_DP_MC_GEMPORT_TYPE];   
    hi_dp_port_u    u_mc_egr[HI_DP_IP_VERSION][HI_DP_MC_GEMPORT_TYPE];      /* �ಥ���ʧ��act TOPOINTʱ��Ч */

    /* ��֪������δ֪������δ֪�鲥�˿ڸ���  */
    hi_dp_port_u   u_igr_filter_en[HI_DP_PORT_NUM];     //��֪���ĵĶ˿ڸ���
    hi_dp_port_u   u_uc_unhit_igr_filter_en[HI_DP_PORT_NUM];     //δ֪�����Ķ˿ڸ���
    hi_dp_port_u   u_mc_unhit_igr_filter_en[HI_DP_PORT_NUM];     //δ֪�鲥�Ķ˿ڸ���    

    HI_BOOL_E    em_mc_filter_en;   /* �鲥����ʹ��  */
    uchar        uc_mc_white_mac[HI_DP_MAC_BYTE_NUM];
    uchar        uc_mc_white_mac_mask[HI_DP_MAC_BYTE_NUM];

    /* SD5113V200 TXDMA ���� TX_ LANS_L3IF_MAC_L_REG�� */
    uchar    uc_lan_mac[HI_DP_LAN_NUM][HI_DP_MAC_BYTE_NUM];    /* ONT LAN��MAC��ַ */
    uchar    uc_wan_mac[HI_DP_WAN_NUM][HI_DP_MAC_BYTE_NUM];    /* ONT WAN��MAC��ַ */    //5115 add

    /* �鲥���� */
    hi_dp_l2_mc_attr_s    st_mc_attr;
    
#ifdef CONFIG_HISI_SD5115_TYPE_T
    /* ARL ȫ�ֺͱ���ѧϰ��Ŀ���� */
    hi_dp_l2_learn_lmt_attr_s st_lrn_lmt_attr;
#endif

}hi_dp_l2_attr_s;


typedef struct
{
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_uc_pkt[HI_DP_PORT_NUM];                /* ��֪�������ļ��� */
    uint ui_uuc_pkt[HI_DP_PORT_NUM];             /* δ֪�������ļ��� */
    uint ui_mc_pkt[HI_DP_PORT_NUM];                /* ��֪�鲥���ļ��� */
    uint ui_umc_pkt[HI_DP_PORT_NUM];             /* δ֪�鲥���ļ��� */
    uint ui_bc_pkt[HI_DP_PORT_NUM];                           /* �㲥���ļ��� */
    uint ui_unknow_type_pkt[HI_DP_PORT_NUM];  
    
    ulong ul_uc_byte[HI_DP_PORT_NUM];
    ulong ul_uuc_byte[HI_DP_PORT_NUM];
    ulong ul_mc_byte[HI_DP_PORT_NUM];
    ulong ul_umc_byte[HI_DP_PORT_NUM];
    ulong ul_bc_byte[HI_DP_PORT_NUM];
    ulong ul_unknow_type_byte[HI_DP_PORT_NUM];
#ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_specify_mc_pkt[HI_DP_L2_SPECIFY_MC_NUM];         /* ָ���ಥ��ı��ļ��� */
    ulong ul_specify_mc_byte[HI_DP_L2_SPECIFY_MC_NUM];
#endif
    #endif
}hi_dp_l2_cnt_s;


typedef struct
{

}hi_dp_l2_sta_s;


typedef struct 
{
    uint    ui_index;    
    uchar   uc_mac[6]; 
    ushort  us_reserved;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;
    uint    ui_fid;                 /* FID����������VLAN ACTION��  */
    hi_dp_port_u    u_egr;          /* �����鲥����,��ʾ�鲥��������,���鲥ID */
    HI_BOOL_E   em_age_en;
    
    HI_BOOL_E  em_valid;
    
}hi_dp_l2_mac_table_item_s; 


typedef struct 
{
    uchar   uc_mac[6];
    ushort  us_arg;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;       /* 5115 add;��ʾ�Ƿ�Ϊuntag���� */

    uint    ui_fid;                 /* FID����������VLAN ACTION��  */
    hi_dp_port_u    u_egr;
    HI_BOOL_E   em_age_en;
        
}hi_dp_l2_mac_s;    



typedef struct 
{
    uint   ui_fid;

    HI_DP_VALN_ACT_E    em_cvlan_act;
    uint    ui_cvlan_value;         

    HI_DP_VALN_ACT_E    em_svlan_act;
    uint    ui_svlan_value;         

    HI_DP_8021P_ACT_E   em_c8021p_act;
    uint    ui_c8021p_value;

    HI_DP_8021P_ACT_E   em_s8021p_act;
    uint    ui_s8021p_value;

    HI_DP_TAG_ACT_E em_tag_act;

    /* 5115 add */
    HI_DP_TPID_REP_MODE_E   em_ctpid_act;
    HI_DP_TPID_REP_MODE_E   em_stpid_act;
    
    HI_BOOL_E   em_valid; 
    
}hi_dp_l2_act_table_item_s;



typedef enum{

    HI_DP_L2_VALN_REP_NO_ACTION = 0,
    HI_DP_L2_VALN_REP_CVLAN,       /* �ڲ�CVLAN�滻 */
    HI_DP_L2_VALN_REP_SVLAN,       /* ���SVLAN�滻 */

}HI_DP_L2_VALN_REP_E;


/* ��ָ����ID, MAC��VLANΨһȷ��һ���鲥�� */    
#define    HI_DP_L2_MC_USER_MAX_NUM  32


typedef struct  
{
    uint    ui_mc_id;                   /* 0 - 255;HI_DP_TRANS_MC_LIST_E��Ч */
    hi_dp_port_u u_mc_port;             /* �鲥�û�;HI_DP_TRANS_MC_PORTMAP_E��Ч */
    
    uchar   uc_mac[6];       
    ushort  us_arg;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;
    
}hi_dp_l2_mc_s;

typedef struct
{
    uint ui_index;

    HI_BOOL_E em_valid;
    uchar     uc_mac[6];
}hi_dp_l2_mc_filter_s;

typedef struct
{
    uint                  ui_index;    /* 0-255 */
    HI_BOOL_E             em_valid;

    uchar                 uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];    /*mc, group; dip */
    uchar                 uc_sip[HI_DP_IPV6_ADDR_BYTE_NUM];   /* source; sip */
    HI_DP_IP_VERSION_E    em_ip_version; 
    uint                  ui_vlan;

    uint                  ui_mc_id;     /* �鲥�������� */
    hi_dp_port_u          u_mc_port;    /* �鲥�û�;HI_DP_TRANS_MC_PORTMAP_E��Ч */

    HI_BOOL_E             em_l3_replace_en;
    uint                  ui_l3_replace_index;    /* OIF2������ */
    
}hi_dp_l2_ip_mc_s;



/*  �鲥�û���Ϣ�ṹ�� */
typedef struct  
{
    uint                ui_vlan;        /* VLAN + PORT ��ʾһ���û� */
    HI_DP_PORT_E        em_egr;
    HI_DP_L2_VALN_REP_E em_vlan_act;   
    HI_BOOL_E           em_valid;
}hi_dp_l2_mc_user_info_s;


/* ָ���鲥ID�µ��û���Ϣ���� */
typedef struct  
{
    uint    ui_mc_id;                  /* 0 - 255 */

    hi_dp_l2_mc_user_info_s  st_user;
    
}hi_dp_l2_mc_user_s;


/* ָ���鲥ID�µ������û���Ϣ���� */
typedef struct  
{
    uint     ui_mc_id;                  /* 0 - 255 */

    hi_dp_l2_mc_user_info_s st_users[HI_DP_L2_MC_USER_MAX_NUM]; /* ���32���û� */
      
}hi_dp_l2_mc_users_s;



#define    HI_DP_L2_BC_EGR_MAX_NUM  32   

typedef struct  
{
    uint            ui_fid;
    HI_DP_PORT_E    em_egr;
    HI_BOOL_E       em_valid;

}hi_dp_l2_bc_egr_info_s;

typedef struct  
{
    uint    ui_vlan;                /* 0 - 4095   ,���� */

    hi_dp_l2_bc_egr_info_s  st_egr;

}hi_dp_l2_bc_egr_s;


typedef struct  
{
    uint    ui_vlan;                /* 0 - 4095   ,���� */

    hi_dp_l2_bc_egr_info_s   st_egrs[HI_DP_L2_BC_EGR_MAX_NUM];

}hi_dp_l2_bc_egrs_s;



#endif /* end of __HI_DRV_DP_L2_H__ */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */



