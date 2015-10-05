/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_dp_local_port.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : GLOBAL CONFIG ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��24��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus  */
#endif /* __cpluscplus  */

#ifndef     __HI_DRV_DP_GLB_CONFIG_H__
 #define    __HI_DRV_DP_GLB_CONFIG_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"

 #define  HI_DP_TIMER_INTERVAL 1000    /* ms */

 #define  HI_DP_TPID_NUM 3
 #define  HI_DP_PORT_DET_NUM 8

typedef enum
{
    HI_DP_GLB_WORK_MODE_E = 0,
    HI_DP_GLB_TEST_MODE_E
} HI_DP_GLB_SYS_MODE_E;

/*
00��ȫ����ģʽ��
01��������ģʽ��
10�����뽻��ģʽ��
11�������� */
typedef enum
{
    HI_DP_GLB_EXCHANG_MODE_E = 0,
    HI_DP_GLB_CONNECT_MODE_E,
    HI_DP_GLB_CONNECT_EXCHANG_MODE_E,
} HI_DP_GLB_SW_MODE_E;

// #if defined (CONFIG_HISI_SD5115_MP_PILOT)
typedef enum
{
    HI_DP_GLB_MAC_SEL_1K_E = 0,
    HI_DP_GLB_MAC_SEL_16K_E,
    HI_DP_GLB_MAC_SEL_8K_E,
    HI_DP_GLB_MAC_SEL_UNKNOWN_E,
} HI_DP_GLB_MAC_SEL_E;
// #endif

/* ȫ�����ò��� */
typedef struct
{
    uint                ui_mtu; /* Added by w00180499, 2011/5/30 */
    HI_DP_UP_MODE_E     em_up_mode;         /* ����ģʽ */
    HI_DP_GLB_SW_MODE_E em_sw_mode;     /* LSW����ģʽ */

    HI_DP_PRI_MODE_E em_up_pri_mode;
    HI_DP_PRI_MODE_E em_dn_pri_mode;

    HI_DP_VLAN_MODE_E em_up_sfc_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_sfc_vlan_mode;
    HI_DP_VLAN_MODE_E em_up_uc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_uc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_up_mc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_mc_fwd_vlan_mode;

 #ifdef CONFIG_HISI_SD5115_TYPE_T
    ushort us_stpid[HI_DP_TPID_NUM];          /* ���TPIDʶ�𣻳���TPID��� */
    ushort us_ctpid[HI_DP_TPID_NUM];          /* ���TPIDʶ�𣻳���TPID��� */
 #else
    ushort us_tpid[HI_DP_TPID_NUM];          /* ���TPIDʶ�𣻳���TPID��� */
 #endif

    HI_DP_ENDIAN_E em_default_cfi;         /* ȱʡCFI: оƬ���˿ڱ������ȱʡTAGʱ��CFI������ */

    HI_BOOL_E em_pritag_en;      /*5115 add;�Ƿ�ʶ��pri_tag���ģ�����tag���Ĵ���*/

    // #if defined (CONFIG_HISI_SD5115_TYPE_H) ||defined (CONFIG_HISI_SD5115_TYPE_T)
    HI_DP_GLB_MAC_SEL_E em_mac_num_sel;      /* MAC��ַ����Ŀѡ�� */
    //#endif
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_pri_match_type;
    uint ui_port_cpu_tag_del;
 #endif

    //#ifdef HI_SDK_DEBUG
    HI_BOOL_E em_sdk_debug_print_en;
    //#endif
} hi_dp_glb_attr_s;

/* ȫ��״̬��Ϣ */
typedef struct
{
    uint ui_year;               /* DATE */
    uint ui_month;
    uint ui_day;

    uint ui_chip_id;            /* 32'h51130100 */
    uint ui_base_line;          /* ���߰汾�� */
    uint ui_sub_version;        /* �Ӱ汾�� */

    uint ui_vendor_id;          /* ����ID */
} hi_dp_glb_sta_s;

typedef struct
{} hi_dp_glb_cnt_s;

/* оƬ������ */
typedef struct
{
    /* оƬ�汾 */
    HI_DP_IC_VER_E em_version;

    /* ���нӿ����� */
    uint ui_gpon_num;  /* GPON�ӿ����� */
    uint ui_epon_num;  /* EPON�ӿ���������GPONģʽ��Ϊ0 */

    /* securityģ�� */
    uint ui_ip_filter_num;            /* IP���˱���   */
    uint ui_ip_filter_ipv4_num;       /* for 5115T:IP���˱��� */
    uint ui_ip_filter_ipv6_num;       /* for 5115T:IP���˱��� */     
    /* flowģ�� */
    uint ui_fc_num;                   /* FC����        */
    uint ui_efc_num;                  /*EFC����        */
    uint ui_tcfl_rep;                 /* TC/FL�滻���� */
    uint ui_flow_point_field_num;     /* �Զ��������    */
    uint ui_flow_g0_num;              /* G0����          */
    uint ui_flow_g1_num;              /* G1����          */
    uint ui_flow_key1_max;            /* key1���ֵ      */
    uint ui_flow_mask1_max;           /* mask1���ֵ     */
    uint ui_fact_num;
    uint ui_flow_statis_index;

    /* L2 */
    uint ui_ip_mc_num;          /* ip�ಥ����             */
    uint ui_l2mac_fid_num;      /* mac��ַ��ѧϰ����fid��Ŀ */

    /* L3 */
    uint ui_l3_replace_num;     /* �����滻����       */
    uint ui_nht_num;            /* ��һ������         */
    uint ui_rt_num;             /* ·�ɱ���           */
    uint ui_rt_act_num;         /* ������Ϊ����       */
    uint ui_tunnel_num;         /* �������           */
    uint ui_tunnel_rt_num;      /* ���·�ɱ���       */
    uint ui_tunnel_sip_num;     /* ���sip������    */
    uint ui_napt_num;           /* NAPT����           */
    uint ui_oif2_size;          /* OIF2��size����λbyte */
    uint ui_nht_ipv4_num;       /* for 5115T:��һ������ */
    uint ui_nht_ipv6_num;       /* for 5115T:��һ������ */
    uint ui_rt_ipv4_num ;       /* for 5115T:��һ������ */
    uint ui_rt_ipv6_num ;       /* for 5115T:��һ������ */    

    /* QOS */
    uint ui_ecar_num;                /* ECAR,��չ�������ƹ��      */
    uint ui_tocpu_reason_car_num;    /* tocpu reason car���       */
    uint ui_tocpu_reason_map_num;    /* tocpu reason carӳ����� */
} hi_dp_glb_spec_s;

/* BEGIN: Added by w00180499, 2011/7/8 */
typedef enum
{
    HI_LOCAL_CLOCK_E = 0,
    HI_LINE_CLOCK_E,
} HI_CURRENT_CLOCK_STATE_E;

typedef struct
{
    HI_CURRENT_CLOCK_STATE_E em_current_clock;
    uint                     ui_clk_switch_times;
} hi_dp_glb_clk_switch_info_s;
/* END:   Added by w00180499, 2011/7/8 */

#endif /* end of __WDTAPI_H */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
