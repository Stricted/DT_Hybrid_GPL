/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_oam.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2009��2��4��
  ����޸�   :
  ��������   : OAM ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��2��4��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /*  __cplusplus  */

#ifndef __HI_DRV_DP_OAM_H__
 #define __HI_DRV_DP_OAM_H__

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define SUPPORT_OAM_1731
 #endif

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"
 #ifdef SUPPORT_OAM_1731
  #include "hi_drv_dp_prbs.h"
 #endif
 #define HI_DP_OAM_LLID_NUM 8

typedef enum
{
    HI_DP_OAM_PORT_LLID0_E = 0,
    HI_DP_OAM_PORT_LLID1_E,
    HI_DP_OAM_PORT_LLID2_E,
    HI_DP_OAM_PORT_LLID3_E,
    HI_DP_OAM_PORT_LLID4_E,
    HI_DP_OAM_PORT_LLID5_E,
    HI_DP_OAM_PORT_LLID6_E,
    HI_DP_OAM_PORT_LLID7_E,
    HI_DP_OAM_PORT_GE0_E,
    HI_DP_OAM_PORT_GE1_E,
    HI_DP_OAM_PORT_FE0_E,
    HI_DP_OAM_PORT_FE1_E,
    HI_DP_OAM_PORT_FE2_E,
} HI_DP_OAM_PORT_E;

 #define HI_DP_OAM_PORT_NUM 13

typedef enum
{
    HI_DP_OAM_ERR_FR_EVENT_E = 0,  /* ��֡�¼�     */
    HI_DP_OAM_ERR_FRP_EVENT_E,     /* ��֡�����¼� */
    HI_DP_OAM_ERR_FRS_EVENT_E,     /* ��֡���¼�   */
    HI_DP_OAM_NO_EVENT_E,
} HI_DP_OAM_EVENT_SEL_E;

typedef struct
{
    HI_DP_OAM_EVENT_SEL_E em_type;      /* �¼�ͨ������     */
    HI_BOOL_E             em_notify_en; /* �¼�ͨ�湦��ʹ�� */
    uint                  ui_window;    /* ��֡���ڴ���     */
    uint                  ui_threshold; /* ��֡����         */
} hi_dp_oam_3ah_event_attr_s;

typedef struct
{
    HI_DP_OAM_EVENT_SEL_E em_event;
    HI_BOOL_E             em_lock;             /* �˿ڵ��¼�ͨ������״̬     */
    uint                  ui_err_frame_num;    /* ָ����������ͳ�Ƶ��Ĵ���֡ */
    uint                  ui_event_time_stamp; /* �������¼���ʱ��           */
} hi_dp_oam_3ah_event_info_s;

typedef struct
{
    HI_BOOL_E em_enable[HI_DP_ETH_PORT_NUM];            /* �˿�3ahʹ��,8��llidĬ��ʹ��,������ */
    HI_BOOL_E em_local_lp_en[HI_DP_OAM_PORT_NUM];       /* ���˻���ʹ��5��ETH�ں�8��LLID */

    HI_BOOL_E        em_remote_loop_en;                 /* Զ�˻���ʹ�� */
    HI_DP_OAM_PORT_E em_remote_loop_port;               /* Զ�˻��ض˿� */
    HI_DP_PORT_E     em_remote_loop_test_port;          /* Զ�˻���ʹ�������ݶ˿ڣ�������Զ˿���Զ�˻��ض˿�������ͬ
                                                        ��������Ч���ܽ��л��ز��� */
    HI_BOOL_E        em_pri_rep_en[HI_DP_ETH_PORT_NUM + 1];/* ���ȼ��滻ʹ�� 5��ETH�ں�PON�� */
    uint             ui_pri_rep[HI_DP_ETH_PORT_NUM + 1];    /* �滻���ȼ� 5��ETH�ں�PON�� */

    HI_DP_OAM_PORT_E           em_event_notify_llid;    /* PON�˿�ָ��֧���¼�ͨ���LLID */
    hi_dp_oam_3ah_event_attr_s st_event_attr[HI_DP_ETH_PORT_NUM + 1];   /* �˿�ʱ������ 5��ETH�ں�PON�� */
 #ifdef SUPPORT_OAM_1731
    HI_BOOL_E em_car_en[HI_DP_ETH_PORT_NUM + 1];        /* ���ڶ˿����õ�oam3ah������������ʹ��ָʾ�� ����5��ETH�ں�PON�ڡ� */

    HI_DP_TRANS_ACT_E em_trans_act[HI_DP_ETH_PORT_NUM + 1]; /*3ahδʹ��ʱ������oam1ag֡�Ĵ���ACTION������5��ETH�ں�PON�ڡ�
                                                            PON�˿ڽ��յ�oam3ah֡����ACTION��
                                                            HI_DP_TRANS_FWD_E��ֻ����ת����
                                                            HI_DP_TRANS_TOCPU_E��ֻת��CPU��
                                                            HI_DP_TRANS_DROP_E����֡���ˣ�
                                                            HI_DP_TRANS_TOCPU_AND_FWD_E������ת��,�Ҹ��Ƶ�CPU��

                                                            ��EHT�˿ڽ���oam3ah֡�Ĵ���ACTION��
                                                            HI_DP_TRANS_FWD_E��ֻ����ת����
                                                            HI_DP_TRANS_TOCPU_E��ֻת��CPU��
                                                            HI_DP_TRANS_DROP_E����֡���ˣ�
                                                            HI_DP_TRANS_TOCPU_AND_FWD_E������ת��,�Ҹ��Ƶ�CPU��
                                                            HI_DP_TRANS_THRU_E��ֻ͸�������пڣ�
                                                            HI_DP_TRANS_TOCPU_AND_THRU_E��͸�������пڣ��Ҹ��Ƶ�CPU��
                                                            ������������
                                                            ע�⣺͸���൱��ָ��ת�������п�
                                                             */
 #endif
} hi_dp_oam_3ah_attr_s;

// VLAN SEL TYPE: FOR SD5115T ONLY
typedef enum 
{
    HI_DP_PRI_VLAN_SEL_TYPE_FWD_E,    
    HI_DP_PRI_VLAN_SEL_TYPE_ORG_S_E,   
    HI_DP_PRI_VLAN_SEL_TYPE_MODIFIED_E,   
}HI_DP_PRI_VLAN_SEL_TYPE_E;

typedef enum 
{
    HI_DP_GEMPORT_SEL_AUTO_E,    
    HI_DP_GEMPORT_SEL_SPECIFY_E,   
}HI_DP_GEMPORT_SEL_E;

typedef struct
{
    HI_BOOL_E em_enable[HI_DP_PORT_NUM];        /* �˿�CFMʹ�� eth,pon,cpu*/
    HI_BOOL_E em_pri_rep_en[HI_DP_PORT_NUM];    /* ���ȼ��滻ʹ�� 5��ETH�ں�EPON,CPU�� */
    uint      ui_pri_rep[HI_DP_PORT_NUM];       /* �滻���ȼ� 5��ETH�ں�EPON,CPU�� */

    //for T
    HI_DP_PRI_VLAN_SEL_TYPE_E em_inward_vlan_sel[HI_DP_PORT_NUM]; /* �˿�VLANѡ�� eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_outward_vlan_sel[HI_DP_PORT_NUM];/* �˿�VLANѡ�� eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_inward_pri_sel[HI_DP_PORT_NUM]; /* �˿�PRIѡ�� eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_outward_pri_sel [HI_DP_PORT_NUM]; /* �˿�PRIѡ�� eth,pon,cpu*/
    HI_DP_GEMPORT_SEL_E               em_gemport_sel;
    uint                              ui_gemport;
    
 #ifdef SUPPORT_OAM_1731
    HI_BOOL_E         em_car_en[6];    /* ���ڶ˿����õ�oam1ag������������ʹ��ָʾ�� ����5��ETH�ں�PON�ڡ� */
    uchar             uc_share_mac[6];
    HI_BOOL_E         em_share_mep;    /*0���ǹ���MEPģʽ��
                                        1������MEPģʽ��*/
    HI_DP_TRANS_ACT_E em_trans_act[HI_DP_PORT_NUM]; /* 1agδʹ��ʱ������oam1ag֡�Ĵ���ACTION��
                                                    PON�˿ڽ��յ�oam3ah֡����ACTION��
                                                    HI_DP_TRANS_FWD_E��ֻ����ת����
                                                    HI_DP_TRANS_TOCPU_E��ֻת��CPU��
                                                    HI_DP_TRANS_DROP_E����֡���ˣ�
                                                    HI_DP_TRANS_TOCPU_AND_FWD_E������ת��,�Ҹ��Ƶ�CPU��

                                                    ��EHT��CPU�˿ڽ���oam3ah֡�Ĵ���ACTION��
                                                    HI_DP_TRANS_FWD_E��ֻ����ת����
                                                    HI_DP_TRANS_TOCPU_E��ֻת��CPU��
                                                    HI_DP_TRANS_DROP_E����֡���ˣ�
                                                    HI_DP_TRANS_TOCPU_AND_FWD_E������ת��,�Ҹ��Ƶ�CPU��
                                                    HI_DP_TRANS_THRU_E��ֻ͸�������пڣ�
                                                    HI_DP_TRANS_TOCPU_AND_THRU_E��͸�������пڣ��Ҹ��Ƶ�CPU��
                                                    ������������
                                                    ע�⣺͸���൱��ָ��ת�������п�
                                                     */
 #endif
} hi_dp_oam_1ag_attr_s;

typedef struct
{
    hi_dp_oam_3ah_attr_s st_3ah_attr;
    hi_dp_oam_1ag_attr_s st_1ag_attr;
} hi_dp_oam_attr_s;

typedef enum
{
    HI_DP_OAM_MEP_UP_E = 0, /* out */
    HI_DP_OAM_MEP_DN_E,     /* in */
    HI_DP_OAM_MIP_E,
} HI_DP_OAM_MP_TYPE_E;
 #if 0
typedef enum
{
    HI_DP_OAM_TEST_LM_SINGLE_END_E = 0, /* ����LM���� */ HI_DP_OAM_TEST_LM_SINGLE_END_PASSIVE_E
    HI_DP_OAM_TEST_LM_DUAL_END_E,       /* ˫��LM���� */
    HI_DP_OAM_TEST_DM_E, /* DM���� */ HI_DP_OAM_TEST_DM_TWO_WAY_PASSIVE_E
    HI_DP_OAM_TEST_THR_DUAL_E,          /* ˫�򱻶����������� */
    HI_DP_OAM_TEST_THR_SINGLE_E,        /* �������������� */
    HI_DP_OAM_TEST_TYPE_NUM_E,
    HI_DP_OAM_TEST_TYPE_NONE_E
} HI_DP_OAM_TEST_TYPE_E;

 #endif
typedef enum
{
    
    HI_DP_OAM_TEST_LM_SINGLE_END_PASSIVE_E , /* ����LM���� */
    HI_DP_OAM_TEST_LM_DUAL_END_E,               /* ˫��LM���� */
    HI_DP_OAM_TEST_DM_TWO_WAY_PASSIVE_E,                /* DM���� */
    HI_DP_OAM_TEST_THR_TWO_WAY_E,               /* ˫�򱻶����������� */
    HI_DP_OAM_TEST_THR_ONE_WAY_E,               /* �������������� */
    HI_DP_OAM_TEST_TYPE_NONE_E,
    HI_DP_OAM_TEST_TYPE_MAX_E,
} HI_DP_OAM_TEST_TYPE_E;

 #ifdef SUPPORT_OAM_1731

typedef struct
{
    /* tag */
    HI_BOOL_E      em_tag_add_en;
    HI_BOOL_E      em_tag_del_en; // not for T
    uint           ui_tpid;
    uint           ui_vlan;
    HI_DP_ENDIAN_E em_cfi;

    /* snap */
    HI_BOOL_E em_snap_add_en;
    HI_BOOL_E em_snap_del_en; // not for T
    uchar     uc_snap_head[HI_DP_SNAP_HEAD_BYTE_NUM];

    /* 1731 */
    HI_BOOL_E em_1731_add_en;
    HI_BOOL_E em_1731_del_en; // not for T
    uchar     uc_1731_head[HI_DP_1731_HEAD_BYTE_NUM];
} hi_dp_oam_1731_attr_s;

typedef  struct
{
    uint   ui_length;
    uint   ui_vlan;
    uint   ui_cfi;
    uint   ui_pri;
    ushort us_tpid;
    uchar  uc_dmac[HI_DP_MAC_BYTE_NUM];
    uchar  uc_smac[HI_DP_MAC_BYTE_NUM];

    uint ui_tx_period;
    uint ui_tx_num;
} hi_dp_mep_prbs_attr_send_s;

typedef  struct
{
    HI_DP_PRBS_RX_MODE_E em_rx_mode; /*����ֻʹ�õ���ģʽ*/
    uint                 ui_rx_err_range;
} hi_dp_mep_prbs_attr_recv_s;

typedef  struct
{
    HI_BOOL_E                  em_tx_enable;
    hi_dp_mep_prbs_attr_send_s st_send;

    HI_BOOL_E                  em_rx_enable;
    hi_dp_mep_prbs_attr_recv_s st_recv;
} hi_dp_mep_prbs_attr_s;
 #endif

 #ifdef CONFIG_HISI_SD5115_TYPE_T

typedef struct
{
    HI_BOOL_E           em_valid;
    HI_DP_OAM_MP_TYPE_E em_mp_type;
    uint                ui_svlan;
 //   HI_BOOL_E           em_vlan_en;
    uint                ui_md_level;
    uchar                uc_pri_map_tab[8];        /*LM����PRI MAP��*/
    uint      ui_lbr_pri;       /* ˫�򱻶�����������ʱ��ָ��Ӧ���lbr���ȼ� */
    HI_DP_OAM_TEST_TYPE_E em_lm_st;
    HI_DP_OAM_TEST_TYPE_E em_dm_st;
    HI_DP_OAM_TEST_TYPE_E em_tm_st;
    HI_BOOL_E             em_user_mac_en;        /* MAC��ַƥ��ʹ��,������Э�鱨��,ֻ�������ݱ��ļ��� */
    uchar                 uc_user_mac[6];
    HI_BOOL_E             em_mep_mac_en;
    uchar                 uc_mep_mac[6];
    hi_dp_mep_prbs_attr_s st_prbs_attr;     /*ֻ����TST������������������Ч���շ�����Ч*/
    hi_dp_oam_1731_attr_s st_header_attr;   /*ֻ����TST������������������Ч���շ�����Ч*/
} hi_dp_oam_cfm_port_item_s;

 #else

typedef struct
{
    HI_BOOL_E           em_valid;
    HI_DP_OAM_MP_TYPE_E em_mp_type;
    uint                ui_svlan;
    uint                ui_md_level;

    HI_BOOL_E             em_test_en;
    HI_DP_OAM_TEST_TYPE_E em_test_st;
    uint                  ui_pri;
  #ifdef SUPPORT_OAM_1731
    HI_BOOL_E em_user_mac_en;                    /* MAC��ַƥ��ʹ��,������Э�鱨��,ֻ�������ݱ��ļ��� */
    uchar     uc_user_mac[6];

    hi_dp_mep_prbs_attr_s st_prbs_attr;     /*ֻ����TST������������������Ч���շ�����Ч*/
    hi_dp_oam_1731_attr_s st_header_attr;   /*ֻ����TST������������������Ч���շ�����Ч*/
  #endif
} hi_dp_oam_cfm_port_item_s;

 #endif/* CONFIG_HISI_SD5115_TYPE_T */

 #define HI_DP_OAM_PORT_CFMMATCH_ITEM_NUM 16

 #define HI_DP_OAM_SHARE_MEP_MODE_PORT_CFMMATCH_ITEM_NUM MEP_TABLE_ITEM_NUM
typedef struct
{
    HI_DP_PORT_E              em_port; /* GE0\GE1\FE0\FE1\FE2\PON\CPU */
    hi_dp_oam_cfm_port_item_s st_item[HI_DP_OAM_PORT_CFMMATCH_ITEM_NUM];    /* FOR H,S,MPW ÿ���˿�������8�����ñ���,ʹ��st_item��ǰ8������.
                                                                            FOR T: NNI��ÿ���˿�������16�����ñ���.
                                                                            �����˿�,ÿ���˿�������8�����ñ���,ʹ��st_item��ǰ8������. */
} hi_dp_oam_cfm_table_item_s;

typedef struct
{
    HI_BOOL_E em_lock;          /* �˿ڵ��¼�ͨ������״̬     */
    uint      ui_frame_err_num; /* ָ����������ͳ�Ƶ��Ĵ���֡ */
    uint      ui_event_ts;      /* �������¼���ʱ��           */
} hi_dp_oam_event_sta_s;

typedef struct
{
    /* GE0\GE1\FE0\FE1\FE2\PON */
    hi_dp_oam_event_sta_s st_event_sta[HI_DP_ETH_PORT_NUM + 1];
} hi_dp_oam_sta_s;

typedef struct
{
    uint ui_rx;             /*  �ǻ��ؽ��յ�3ahЭ�鱨����(�˿�OAM ����δ��Ҳͳ��)             */
    uint ui_local_lp_rx;    /*  ���˻���ʱ���յ�3ahЭ�鱨������       */
    uint ui_local_lp_tx;    /*  ���˻���ʱ���ػ�ȥ�����ݱ�������      */
    uint ui_remote_lp_rx;   /*  Զ�˻���ʱ���յ�3ahЭ�鱨������       */
    uint ui_remote_test_tx; /*  Զ�˻���ʱ���͵Ĳ������ݱ�������      */
    uint ui_remote_test_rx; /*  Զ�˻���ʱ���յĲ��Ա�������          */
} hi_dp_oam_port_cnt_s;

/* ���ڶ˿ڵĸ�����Ӧ��OAM 3AHͳ���� */
typedef struct
{
    hi_dp_oam_port_cnt_s st_port_cnt[13];                           /* ETH + LLID */
    ulong                ul_err_frame_total[HI_DP_ETH_PORT_NUM + 1];/* �˿��¼�ͨ���ܴ�֡�� */
    uint                 ui_event_num[HI_DP_ETH_PORT_NUM + 1];      /* �˿��¼�ͨ���¼��� */
} hi_dp_oam_cnt_s;

/*oam 3ah �¼��ж�״̬�������жϻص������Ĳ���ui_arg�Ľ���*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fe0_evn_int  : 1; // [0]
        unsigned int Reserved_40  : 3; // [3..1]
        unsigned int fe1_evn_int  : 1; // [4]
        unsigned int Reserved_39  : 3; // [7..5]
        unsigned int fe2_evn_int  : 1; // [8]
        unsigned int Reserved_38  : 3; // [11..9]
        unsigned int fe3_evn_int  : 1; // [12]
        unsigned int Reserved_37  : 3; // [15..13]
        unsigned int fe4_evn_int  : 1; // [16]
        unsigned int Reserved_36  : 3; // [19..17]
        unsigned int epon_evn_int : 1; // [20]
        unsigned int Reserved_35  : 11; // [31..21]
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_EVENT_INT_STATE;

typedef enum
{
    HI_DP_OAM_EVENT_ALARM_E = 0,
    HI_DP_OAM_ALARM_NUM_E,
} HI_DP_OAM_ALARM_E;

/*OAMģ��澯���� */
typedef struct
{
    /* �澯����*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_OAM_ALARM_NUM_E];
    void (*alarm_call_back)(HI_DP_OAM_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_oam_alarm_attr_s;

#endif /*  end of __HI_DRV_DP_OAM_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /*  end of __cplusplus  */
#endif  /*  end of __cplusplus  */
