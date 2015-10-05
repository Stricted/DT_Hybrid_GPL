/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_l3.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2009��2��4��
  ����޸�   :
  ��������   : L3 ģ�����ݽṹͷ�ļ�
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
#endif /* __cplusplus  */

#ifndef __HI_DRV_DP_L3_H__
 #define __HI_DRV_DP_L3_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define  HI_DP_L3_HASH_ITEM_NUM 16
  #define  HI_DP_L3_HASH_BUCKET_NUM 1024
  #define  HI_DP_L3_HASH_TABLE_ITEM_NUM 16384
 #endif
 #define  HI_DP_L3_REP_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_REPLACE_TABLE_ITEM_NUM 256

 #define  HI_DP_L3_REP_OP_TIMES 4                          /* CPUÿ����һ��������Ҫ���������Ĵ� */
 #define  HI_DP_L3_REP_OP_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_REP_OP_TABLE_ITEM_NUM_V200 256

 #define  HI_DP_L3_EX_OUTNET_FILTER_NUM 8
 #define  HI_DP_L3_EX_OUTNET_FILTER_TABLE_ITEM_NUM 64

 #define  HI_DP_L3_NAPT_TABLE_ITEM_NUM (8 * 1024)

 #define  HI_DP_L3_NH_TABLE_ITEM_NUM 128
 #define  HI_DP_L3_NH_SUBTABLE_ITEM_NUM 64
 #define  HI_DP_L3_RT_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_ITRT_TABLE_ITEM_NUM 8
 #define  HI_DP_L3_OTRT_TABLE_ITEM_NUM 8
 #define  HI_DP_L3_RT_ACT_TABLE_ITEM_NUM 256
 #define  HI_DP_L3_TSCT_TABLE_ITEM_NUM 64

 #define  HI_DP_L3_TUNNEL_HEAD_LEN_MAX 64
 #define  HI_DP_L3_TUNNEL_MTU_MAX HI_BITS_LOW_11
 #define  HI_DP_L3_TUNNEL_TTL_DEC_MAX HI_BITS_LOW_8

 #define  HI_DP_L3_6RD_TUNNEL_HEAD_LEN_MAX 20

 #define  HI_DP_L3_IPV6_CNT_TABLE_ITEM_NUM 8

 #define  HI_DP_L3_IPV4_MASK_RANGE_MAX 32
 #define  HI_DP_L3_IP_MASK_RANGE_MAX 128

 #define HI_DP_L3_RT_LABEL_NUM 16
 #define HI_DP_L3_IPV6_TUNNEL_NUM 8
 #define HI_DP_L3_IPV6_STAT_NUM 16

 #define HI_DP_L3_RT_MTU_MAX HI_BITS_LOW_12
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define HI_DP_L3_NAPT_AGE_NUM 16
  #define HI_DP_L3_NAPT_LINK_CNT_PERIOD_MAX 7 // 2^7=128S
 #endif

/* NAPT������֧�ֵ�IPЭ������ */
typedef enum
{
    HI_DP_L3_TCP_PROTO_E = 0, /* */
    HI_DP_L3_UDP_PROTO_E,
} HI_DP_L3_IP_PROTO_E;

/* ipv4/v6 ����sip��� */
typedef struct
{
    HI_DP_TRANS_ACT_E em_loop_act;          /* v4/v6 */
    HI_DP_TRANS_ACT_E em_mc_act;            /* v4/v6 */
    HI_DP_TRANS_ACT_E em_site_local_act;    /* only v6 */
    HI_DP_TRANS_ACT_E em_link_local_act;    /* only v6 */
    HI_DP_TRANS_ACT_E em_all_0_act;         /* v4/v6 */
} hi_dp_l3_sip_check_s;

/* ipv6ͳ���������� */
typedef struct
{
    HI_BOOL_E    em_valid;
    HI_BOOL_E    em_vlan_en;
    uint         ui_vlan;
    HI_BOOL_E    em_port_en;         /* �����һ����Ҫ���ã�����port��Ϊ��Ч */
    HI_DP_PORT_E em_port;
} hi_dp_l3_ip_stat_attr_s;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT

typedef union
{
    struct
    {
        uint fin : 1;
        uint syn : 1;
        uint rst : 1;
        uint ack : 1;

        uint reserved0 : 28;
    } bits;

    uint value;
} hi_dp_l3_tcp_ctrl_u;

/* ��OPTION���Ŀ��� */
typedef struct
{
    HI_DP_TRANS_ACT_E em_ip_option_act;     /* ���ƴ�Option IP������NAPTʱ�Ķ�����0:��CPU��1:������*/
    HI_DP_TRANS_ACT_E em_ip_frag_act;       /* ���� IP��Ƭ������NAPTʱ�Ķ�����0:��CPU��1:������*/

    HI_DP_TRANS_ACT_E   em_tcp_act;         /* TCP���Ŀ���λ�����ı��Ķ������ƣ�00:����ת����01:����һ�ݽ�CPU��10~11;ת��CPU�� */
    hi_dp_l3_tcp_ctrl_u u_tcp_ctrl_bit;     /*TCP���Ŀ���λ�ڣ�bit[0],bit[1], bit[2]�ֱ��Ӧfin,syn,rst������*/
} hi_dp_l3_ip_option_ctrl_s;
 #endif

typedef enum
{
    HI_DP_WAN_ID_SEL_WAN_E,
    HI_DP_WAN_ID_SEL_RSP_E,
} HI_DP_WAN_ID_SEL_E;

typedef enum
{
    HI_DP_RT_MODE_IPV4_E,
    HI_DP_RT_MODE_DAUL_STACK_E,
    HI_DP_RT_MODE_IPV6_E,
    HI_DP_RT_MODE_UNKNOWN_E
} HI_DP_RT_MODE_E;
typedef struct
{
    uint ui_rt_num;
    uint ui_nh_num;
    uint ui_bwlt_num;
} hi_dp_l3_rt_spec_s;

/* napt���ò��� */
typedef struct
{
    HI_DP_TRANS_ACT_E em_napt_dn_unhit_act;  /* NAPT����,���в�ѯSUBNETû������,�������ΪתCPU���߶��� */
    HI_DP_TRANS_ACT_E em_napt_up_unhit_act;  /* NAPT����,���в�ѯSUBNETû������,�������ΪתCPU���߶��� */
    HI_BOOL_E         em_napt_age_en;
    uint              ui_napt_age_time;
    uint              ui_ipv6_point_next_header;  /* ΪIPv6��չԤ��������,֪����չͷ,KNOWN_NEXT_HEADER */
    HI_BOOL_E         em_tunnel_sip_check_en;     /* �������SIP���ʹ�� */

    HI_DP_TRANS_ACT_E em_tunnel_nh_act[HI_DP_L3_IPV6_TUNNEL_NUM];/* ����չͷ�ı������������ */
    HI_DP_TRANS_ACT_E em_mtu_over_act;            /* ��������MTU����·��·��Э�̵�MTUֵ�Ķ��� */
    HI_DP_TRANS_ACT_E em_ttl_over_act;            /* HOP_LIMIT/TTLС�ڵ���1ʱ������ */
    HI_DP_TRANS_ACT_E em_rt_unhit_act;            /*·�ɲ��ʧ�ܶ��� */

    hi_dp_l3_sip_check_s st_ipv6_sip_check;
    hi_dp_l3_sip_check_s st_ipv4_sip_check;
    HI_DP_TRANS_ACT_E    em_ipv6_unknown_header_act; /* IPV6��֪����չͷ��� */

    /* ͬһ���˿ڸ���L3�ӿ�ʹ�� */
    HI_BOOL_E em_igr_fitler_en;

    HI_DP_TRANS_ACT_E em_hov128_act;              /* cap:IPͷ�������� */

    /* ����ͷ����128byte����ͨ·�ɱ��ģ������������·��ת����
    ������п��ܲ����������ת�� */
    hi_dp_l3_ip_stat_attr_s st_ip_stat_attr[HI_DP_L3_IPV6_STAT_NUM];
    HI_DP_TRANS_ACT_E       em_ipv6_frag_act;     /* ipv6��Ƭ���Ĵ��� */
    HI_DP_TRANS_ACT_E       em_6rd_frag_act;      /* 6RD��Ƭ���Ĵ��� */
    HI_DP_TRANS_ACT_E       em_6rd_option_act;    /* 6RD option���Ĵ��� */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    hi_dp_l3_ip_option_ctrl_s st_ip_option_ctrl;
    uint                      ui_napt_link_cnt_period;  /*napt����ͳ���������ã��㷨:2��ui_napt_link_cnt_period�η���*/
 #endif

 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_DP_WAN_ID_SEL_E em_wan_id_sel;/*0��ѡ��·��DIP��WAN ID����NAPT��
                                     1��ѡ��RSP���ֵ�RSP ID����NAPT��*/
    HI_DP_RT_MODE_E    em_rt_mode; /*RT�����ƼĴ�����
                                   0:2K��IPv4·�ɣ�IPv4�ڰ��������ˣ�
                                   1:1K��IPv4·��+IPv4�ڰ��������˺�256��IPv6·�ɣ�-
                                   IPv6�ڰ���������
                                   ����:512��IPv6·�ɣ�IPv6�ڰ��������ˡ�*/
 #endif
} hi_dp_l3_attr_s;

typedef enum
{
    HI_DP_L3_NAPT_MATCH_MODE_3_TUPLES_E = 0,
    HI_DP_L3_NAPT_MATCH_MODE_4_TUPLES_E,
    HI_DP_L3_NAPT_MATCH_MODE_5_TUPLES_E,
} HI_DP_L3_NAPT_MATCH_MODE_E;

typedef enum
{
    HI_DP_L3_DIRECTION_UP_E = 0,
    HI_DP_L3_DIRECTION_DOWN_E,
} HI_DP_L3_DIRECTION_E;

typedef struct
{
    HI_DP_L3_NAPT_MATCH_MODE_E em_match_mode;

    HI_BOOL_E em_rt_lable_en;        /*·�ɱ�ǩƥ��ʹ��  IF_ID_EN*/
    uint      ui_rt_lable;           /*0-7              IF_ID*/

    HI_DP_L3_IP_PROTO_E em_proto;    /* ��Napt����������ͨ����IP����Э������ */

    HI_BOOL_E em_dip_en;
    uint      ui_dip;
    HI_BOOL_E em_dport_en;
    uint      ui_dport;
    HI_BOOL_E em_sip_en;
    uint      ui_sip;
    HI_BOOL_E em_sport_en;
    uint      ui_sport;
} hi_dp_l3_napt_table_rule_item_s;

typedef struct
{
    HI_BOOL_E em_age_en;                  /* ��̬��ʾλ */
    uint      ui_age;                     /* �������� */

    uint         ui_replace_index;
    HI_DP_PORT_E em_uni_port;             /* �û���˿���Ϣ;�����б���Ч */
    uint         ui_replace_port;         /* NAPT�滻��port������:SPORT;����:DPORT */
} hi_dp_l3_napt_table_act_item_s;

typedef struct
{
    uint ui_bucket_index;                   /* hashͰ����      */
    uint ui_idle_num;                       /* ���б������Ŀ   */
    uint ui_item_index;                     /* ����ֱ������   */
} hi_dp_l3_napt_table_result_s;

typedef struct
{
    uint                            ui_index; /* ����ֱ�ӷ���ʱ����Ч */
    HI_BOOL_E                       em_valid; /* ����ֱ�ӷ���ʱ����Ч,��ӷ��ʣ�����Ҫ�ṩ */
    hi_dp_l3_napt_table_rule_item_s st_rule;
    hi_dp_l3_napt_table_act_item_s  st_act;
    hi_dp_l3_napt_table_result_s    st_result;
} hi_dp_l3_napt_table_item_s;

/*
NOP �޲���
REP_DMAC    �滻DMACֵ
REP_SMAC    �滻SMACֵ
REP_DIP �滻DIP
REP_SIP �滻SIP
REP_PPP �޸�PPP���е�Session_id
DEL_PPP ɾ��PPPͷ
ADD_PPP ���PPPͷ
 */

/* �滻��Ϊ���� */
typedef enum
{
    HI_DP_L3_REP_NOP_E = 0,
    HI_DP_L3_REP_DMAC_E,     /* �滻DMACֵ */
    HI_DP_L3_REP_SMAC_E,     /* �滻SMACֵ */
    HI_DP_L3_REP_DIP_E,      /* �滻DIP */
    HI_DP_L3_REP_SIP_E,      /* �滻SIP */
    HI_DP_L3_REP_PPP_E,      /* �޸�PPP���е�Session_id */
    HI_DP_L3_DEL_PPP_E,      /* ɾ��PPPͷ */
    HI_DP_L3_ADD_PPP_E,      /* ���PPPͷ */
} HI_DP_L3_REP_ACT_E;

/*
0x0��TCP���ģ�
0x1��UDP���ģ�
0x2��TCP/UDP���ģ�
0x3����IP���ĵ�Э�����Ͳ������ƣ�
 */

/* BEGIN: Added by shengjing, 2010/10/19   ���ⵥ��:SD5113V200����*/
/* ��/����������� */
typedef enum
{
    HI_DP_L3_TUNNEL_IN_E = 0,   /*��ͷ*/
    HI_DP_L3_TUNNEL_OUT_E       /*ɾͷ*/
} HI_DP_L3_TUNNEL_DIRECTION_E;

/* ���TC/FL�滻��Ϊ */
typedef enum
{
    HI_DP_L3_TUNNEL_REP_NO_ACTION_E = 0,    /* �޲���;���ò����⣬������������FL������EFCָ��ֵ�޸�FL */
    HI_DP_L3_TUNNEL_REP_ORIGINAL_DSCP,      /* TCʹ��ԭʼ����DSCP */
    HI_DP_L3_TUNNEL_REP_EFC_DSCP_E,         /* TCʹ��EFCָ��ֵ����EFCû��ָ������ʹ��ԭ����DSCP */
    HI_DP_L3_TUNNEL_REP_EFC_E,              /* TCʹ��EFCָ��ֵ����EFCû��ָ�����򲻽����滻 */
} HI_DP_L3_TUNNEL_REP_E;

typedef enum
{
    HI_DP_L3_PRI_REP_NO_ACTION_E,           /* ���滻 */
    HI_DP_L3_PRI_REP_DSCP_H_E,              /* ����DSCP/TC�ĸ�3bit */
    HI_DP_L3_PRI_REP_POINT_E,               /* ʹ��ָ��ֵ�滻 */
} HI_DP_L3_PRI_REP_E;

typedef enum
{
    HI_DP_TUNNEL_HEAD_LEN_40B_E = 0,
    HI_DP_TUNNEL_HEAD_LEN_48B_E,
    HI_DP_TUNNEL_HEAD_LEN_56B_E,
    HI_DP_TUNNEL_HEAD_LEN_64B_E,
    HI_DP_TUNNEL_HEAD_LEN_20B_E,    /* for 6RD only */
} HI_DP_TUNNEL_HEAD_LEN_E;

typedef enum
{
    HI_DP_TUNNEL_TYPE_DSLITE_E = 0,
    HI_DP_TUNNEL_TYPE_6RD_E,
} HI_DP_TUNNEL_TYPE_E;

typedef enum
{
    HI_DP_TUNNEL_MAP_TYPE_STATIC_E = 0,   /* ��̬ӳ�� */
    HI_DP_TUNNEL_MAP_TYPE_DYNAMIC_E,      /* ��̬ӳ�� */
} HI_DP_TUNNEL_MAP_TYPE_E;

typedef struct
{
    HI_DP_L3_REP_ACT_E em_rep_act0;      /* �滻��Ϊ0 */
    HI_DP_L3_REP_ACT_E em_rep_act1;      /* �滻��Ϊ1 */

    uint   ui_value0;                    /* �滻��Ϊ0��Ӧ�Ĳ��� */
    ushort us_value1;                    /* �滻��Ϊ1��Ӧ�Ĳ��� 16 */
    uchar  uc_mac[6];                    /* ���滻��Ϊ0ΪMACʱ��,��ʾ�滻��MAC, ��ʱ��������������Ч */
} hi_dp_l3_rep_op_item_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_L3_MAC_SEL_1K_E = 0,
    HI_DP_L3_MAC_SEL_16K_E,
    HI_DP_L3_MAC_SEL_8K_E,
    HI_DP_L3_MAC_SEL_UNKNOWN_E,
} HI_DP_L3_MAC_SEL_E;
 #endif
typedef struct
{
    uint ui_index;     /* 0 - 255,SD5113V200 */

    HI_BOOL_E em_smac_rep_en;
    uchar     uc_smac[6];
    HI_BOOL_E em_dmac_rep_en;
    uchar     uc_dmac[6];
    HI_BOOL_E em_sip_rep_en;
    uint      ui_sip;
    HI_BOOL_E em_dip_rep_en;
    uint      ui_dip;

    /*PPP ��Ϊ*/
    HI_DP_EDIT_ACT_E em_ppp_act;
    uint             ui_session_id;

    HI_BOOL_E        em_ttl_en;         /* ����HL��־ */
    HI_DP_EDIT_ACT_E em_tag_act;        /* TAG���/ɾ�������ȼ�����EFC */

    HI_BOOL_E          em_svlan_rep_en;  /* ���VLAN�滻ʹ�� */
    uint               ui_svlan;
    HI_DP_L3_PRI_REP_E em_spri_rep;      /* ���PRI�滻ѡ�� */
    uint               ui_spri;
    HI_BOOL_E          em_cvlan_rep_en;  /* �ڲ�VLAN�滻ʹ�� */
    uint               ui_cvlan;
    HI_DP_L3_PRI_REP_E em_cpri_rep;      /* �ڲ�PRI�滻ѡ�� */
    uint               ui_cpri;

    HI_DP_TOS_MODE_E em_tos_mode;         /* ����TOS��ѡ�� */
    HI_BOOL_E        em_tos_rep_en;       /* ����TOS�޸�ʹ�� */
    uint             ui_tos;              /* ����TOS�޸�ֵ */

    HI_DP_TPID_REP_MODE_E em_stpid_rep_mode;    /* ���tpid�޸�ģʽ */
    HI_DP_TPID_REP_MODE_E em_ctpid_rep_mode;    /* �ڲ�tpid�޸�ģʽ */

    /* �������ý���6RD�����Ч */
    HI_DP_TUNNEL_MAP_TYPE_E em_map_type;    /* ָʾӳ������ */
    uint                    ui_v4_mask_len;
    uint                    ui_6rd_prefix_len;
} hi_dp_l3_replace_table_item_s;

 #if 0
typedef struct
{
    /*uint ui_index;                    ��5115T��������Ч*/

    HI_BOOL_E em_valid;

    /*��һ��IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];     /* Ŀ��ip��ַ��v4ֻ�е�4byte */
    HI_DP_IP_VERSION_E em_ip_version;                        /* ����IP�汾��v4 or v6 */

    uint      ui_mtu;
    uint      ui_rt_act_index; /*  ·����Ϊ��������оƬOIF1*/
    HI_BOOL_E em_rsp_en;
    uint      ui_rsp_id;
} hi_dp_l3_nh_table_item_s;

typedef struct
{
    /* uint ui_index;             0-63 */

    HI_BOOL_E em_valid;

    /*Ŀ��IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* Ŀ��ip��ַ��v4ֻ�е�4byte */
    HI_DP_IP_VERSION_E em_ip_version;                    /* ����IP�汾��v4 or v6 */
    uint               ui_mask_range;                    /* ���뷶Χ  bit��Χ*/
    uint               ui_mtu;
    uint               ui_rt_act_index;                  /*  ·����Ϊ��������оƬOIF1*/

    HI_BOOL_E em_rsp_en;
    uint      ui_rsp_id;
} hi_dp_l3_rt_table_item_s;

 #endif/* t */

typedef struct
{
    uint ui_index;                   /* 0-127 */

    HI_BOOL_E em_valid;

    /*��һ��IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];     /* Ŀ��ip��ַ��v4ֻ�е�4byte */
    HI_DP_IP_VERSION_E em_ip_version;                        /* ����IP�汾��v4 or v6 */

    HI_BOOL_E em_rsp_en;    /* for 5115T only*/
    uint      ui_rsp_id;    /* for 5115T only*/

    uint ui_mtu;
    uint ui_rt_act_index;      /*  ·����Ϊ��������оƬOIF1*/
} hi_dp_l3_nh_table_item_s;

typedef struct
{
    uint ui_index;             /* 0-63 */

    HI_BOOL_E em_valid;

    /*Ŀ��IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* Ŀ��ip��ַ��v4ֻ�е�4byte */
    HI_DP_IP_VERSION_E em_ip_version;                    /* ����IP�汾��v4 or v6 */
    uint               ui_mask_range;                    /* ���뷶Χ  bit��Χ*/

    HI_BOOL_E em_rsp_en;    /* for 5115T only*/
    uint      ui_rsp_id;    /* for 5115T only*/

    uint ui_mtu;
    uint ui_rt_act_index;                  /*  ·����Ϊ��������оƬOIF1*/
} hi_dp_l3_rt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    /*��һ���Ƿ���Ч*/
    HI_BOOL_E em_nh_valid;

    /* ��� em_nt_valid Ϊ��Чʱ����Ϊ */
    /* drop or to_cpu */
    HI_DP_TRANS_ACT_E em_nh_invalid_act;

    /*��� em_nt_validΪ��Ч�������������Ϊ��Ч*/
    /*HI_DP_TRANS_FWD_E,HI_DP_TRANS_DROP_E,HI_DP_TRANS_TOCPU_E,*/
    /*·����Ϊ  ��ͨ·��   �ڶ�·��   �ܾ�·��*/
    HI_DP_TRANS_ACT_E em_rt_act;

    /*��� Ϊ��ͨ·��,�������������Ч*/
    HI_DP_PORT_E em_egr;       /* ת���˿�0-6 */

    HI_BOOL_E em_napt_en;      /* ��napt��־ */
    HI_BOOL_E em_rt_label_en;  /* ·�ɱ�ǩƥ��ʹ��*/
    uint      ui_rt_label;     /* ����NAPT������WAN�� if_id*/

    HI_BOOL_E em_tunnel_en;    /* �Ƿ񱾵������1:�������ý��г�/��������� */
    uint      ui_tunnel_index; /* ��������� */

    /*�ص㿼��  ������оƬ�ڲ��Ĳ��ֹ���*/
    HI_BOOL_E em_tc_rep_en;    /*���TC�滻ʹ��*/
} hi_dp_l3_rt_act_table_item_s;

typedef struct
{
    /*�������*/
    uint ui_index;         /* ����ID */

    HI_BOOL_E           em_valid;
    uchar               uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* Ŀ��ip��ַ */
    uint                ui_rt_act_index;                  /*L3_REP,  ·����Ϊ��������оƬOIF1*/
    HI_DP_TUNNEL_TYPE_E em_tunnel_type;                   /* ����out_tunnel��Ч */
    HI_BOOL_E           em_rsp_en; /* for 5115T only*//* ����out_tunnel��Ч */
    uint                ui_rsp_id; /* for 5115T only*//* ����out_tunnel��Ч */
} hi_dp_l3_tunnel_rt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    uchar uc_head[HI_DP_L3_TUNNEL_HEAD_LEN_MAX];

    /* ���ͷ�ĳ��ȣ���λbyte,ui_head_len = 40 + оƬֵ * 8 */
    HI_DP_TUNNEL_HEAD_LEN_E em_head_len_add;  /* ָ��������ͷ���� */
    HI_DP_TUNNEL_HEAD_LEN_E em_head_len_del;  /* ָ��ɾ�����ͷ���� */
    /* оƬT_LEN_ADD��T_LEN_DEL�ṩͬһ�������� */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    /* MPW����֧�֣���Ϊ�˼��ݣ�PILOT֧�ְ������������ fufenglian */
    uint ui_ip_mtu;            /* MTUΪIP���ĳ��ȣ���������·ͷ���� */
 #endif
    HI_BOOL_E         em_ttl_en;              /* ���������TTL�Ƿ��ui_ttl_dec��־ */
    uint              ui_ttl_step;            /* TTL�ĵݼ�����ֵ */
    HI_DP_TRANS_ACT_E em_tunnel_with_nh_act;  /* ����չͷ��������ĳ�������� */
} hi_dp_l3_tunnel_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    uchar     uc_head[HI_DP_L3_6RD_TUNNEL_HEAD_LEN_MAX];
    HI_BOOL_E em_hop_limit_en;     /* ���������HL�Ƿ��ui_ttl_dec��־ */
    uint      ui_hop_limit_step;   /* HL�ĵݼ�����ֵ */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_ip_mtu;
 #endif
} hi_dp_l3_6rd_tunnel_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;                      /* RT_BASED_CFG_REG2 */

    uchar uc_ip[HI_DP_IPV6_ADDR_BYTE_NUM];   /* ԴIP��ַ */
} hi_dp_l3_tunnel_sip_table_item_s;

/* IPv6ͳ�ƣ�port + vlan ������ */
typedef struct
{
    HI_DP_PORT_E em_port;         /* ����˿� */
    HI_BOOL_E    em_vlan_en;      /* �ӿ�VLAN��Ч��ʶ */
    uint         ui_vlan;         /* �ӿ�VLAN����·�ɺ��Žӽӿڣ� */
    ulong        ul_frame_num;    /* IPv6 ֡��Ŀ */
    ulong        ul_byte_num;     /* IPv6 ֡�ֽ��� */
    ulong        ul_uc_frame_num; /* IPv6 ����֡�� */
    ulong        ul_mc_frame_num; /* IPv6 �鲥֡�� */
} hi_dp_l3_ipv6_cnt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_table_hit;
} hi_dp_l3_sta_s;

typedef struct
{
    /* rt */
    ulong ul_rx_frame;    /* ���յ�֡��Ŀͳ�� */
    ulong ul_rx_byte;     /* ���յ�֡�ֽ���ͳ�� */
    ulong ul_rx_uc_frame; /* ���յĵ���֡��ͳ�� */
    ulong ul_rx_mc_frame; /* ���յ��鲥֡��ͳ�� */

    ulong ul_tx_frame;    /* ���͵�֡��Ŀͳ�� */
    ulong ul_tx_byte;     /* ���͵�֡�ֽ���ͳ�� */
    ulong ul_tx_uc_frame; /* ���͵ĵ���֡��ͳ�� */
    ulong ul_tx_mc_frame; /* ���͵��鲥֡��ͳ�� */
} hi_dp_wan_cnt_item_s;

typedef struct
{
    uint                 ui_index; /* 0 - 15 */
    hi_dp_wan_cnt_item_s st_v4_cnt;
    hi_dp_wan_cnt_item_s st_v6_cnt;
} hi_dp_wan_cnt_s;

typedef struct
{
    //NAPT
    uint ui_dn_drop_cnt;
    uint ui_up_drop_cnt;
    uint ui_dn_tocpu_cnt;
    uint ui_up_tocpu_cnt;
    uint ui_frag_tocpu_cnt;
    uint ui_option_drop_cnt;
    uint ui_option_tocpu_cnt;
    uint ui_up_match_cnt;
    uint ui_dn_match_cnt;
    uint ui_frag_drop_cnt;
    uint ui_ctl_bit_tocpu_cnt;

    //RT
    uint ui_rt_tocpu_cnt;
    uint ui_rt_fwd_cnt;
    uint ui_rt_drop_cnt;
    uint ui_v4_sip_err_st;
    uint ui_v6_sip_err_st;
    uint ui_nni_ip_filt_cnt;
    uint ui_uni_ip_filt_cnt;
    uint ui_ipv6_unk_hdr_cnt;

    /* BEGIN: Added by l00164498, 2012/8/17 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint  ui_in_tnl_pkt_cnt[HI_DP_TNL_NUM];       /* ���������ͳ�� */
    uint  ui_out_tnl_pkt_cnt[HI_DP_TNL_NUM];      /* ���������ͳ�� */
    ulong ul_in_tnl_byt_cnt[HI_DP_TNL_NUM];      /* ������ֽ�ͳ�� */
    ulong ul_out_tnl_byt_cnt[HI_DP_TNL_NUM];     /* ������ֽ�ͳ�� */
    uint  ui_tnl_mtu_cnt[HI_DP_TNL_NUM];          /* �����MTU��������ͳ�ƼĴ��� */
    uint  ui_tnl_drop_cnt[HI_DP_TNL_NUM];         /* ����Ķ���ͳ�ƼĴ��� */
 #endif
    /* END: Added by l00164498, 2012/8/17 */
} hi_dp_l3_cnt_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_HASH_BUCKET_NUM0_E = 0,
    HI_DP_HASH_BUCKET_NUM1_E,
} HI_DP_L3_HASH_BUCKET_NUM_E;

typedef enum
{
    HI_DP_HASH_BUCKET_NORMAL_E = 0,
    HI_DP_HASH_BUCKET_ESCAPE_E,
} HI_DP_L3_HASH_BUCKET_TYPE_E;

typedef struct
{
    uint      ui_index;
    uint      ui_bucket_index;
    HI_BOOL_E em_valid;
} hi_dp_l3_napt_point_cnt_table_item_s;

  #define HI_DP_L3_POINT_BUCHET_NUM 64

typedef struct
{
    uint ui_index;            /* ͳ��������0~63 */

    uint ui_bucket_index;
    uint ui_min_item_index;   /* ��С���ֱ������ */

    uint ui_cnt[16];          /* ��Ͱ��ÿ�����������ͳ�� */

    uint ui_item_index[16];   /* ��Ͱ��ÿ�������ֱ�������� */
} hi_dp_l3_napt_point_cnt_s;

typedef enum
{
    HI_DP_L3_NAPT_AGE_ALARM_E = 0,
    HI_DP_L3_ALARM_NUM_E,
} HI_DP_L3_ALARM_E;

/* napt��ַ�ṹ */

typedef struct
{
    uint ui_bucket_index;    /* hashͰ���� */
    uint ui_item_index;      /* ������Ե�ַ */
} hi_dp_l3_napt_addr_s;

/* һ��napt�ϻ��жϣ��ϱ�����Ϣ */
typedef struct
{
    uint ui_age_num;   /* �����жϣ��ϻ���Ͱ������1~16 */

    hi_dp_l3_napt_addr_s st_addr[HI_DP_L3_NAPT_AGE_NUM];
} hi_dp_l3_napt_age_info_s;

/*L3ģ��澯���� */
typedef struct
{
    /* �澯����*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_L3_ALARM_NUM_E];

    void (*alarm_call_back)(HI_DP_L3_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_l3_alarm_attr_s;
 #endif

/* BEGIN: Added by l00164498, 2012/8/6 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T

typedef union
{
    struct
    {
        uint ge0 : 1;
        uint ge1 : 1;
        uint ge2 : 1;
        uint ge3 : 1;
        uint ge4 : 1;
        uint pon : 1;
        uint cpu : 1;
        uint rsv : 25;
    } bits;
    uint value;
} hi_dp_l3_port_u;

typedef struct
{
    uint               ui_rsp_id; //ƥ���rsp id,0-7
    HI_BOOL_E          em_ethtype_en; //�Ƿ��עETH_TYPE��
    HI_BOOL_E          em_pri_en; //�Ƿ��ע���ȼ���
    HI_BOOL_E          em_vlan_en; //�Ƿ��עת��VLAN��
    HI_BOOL_E          em_ip_en; //�Ƿ��עIP��
    HI_DP_IP_VERSION_E em_ip_ver; //SIP IP�汾
    uint               ui_ethtype; //ƥ���eth type
    hi_dp_l3_port_u    u_port_mask; //0����ʾ��ƥ���Ӧ�˿ڣ�1����ʾƥ���Ӧ�Ķ˿ڡ�
    uint               ui_vlan;
    uint               ui_pri;

    /* IP������ ui_ip_mask
    0:�����룻
    1:SIP���1bit���룻
    2:SIP���2bit���룻
    ......
    127:SIP���127bit���룻
    128:SIPȫ���룬������עSIP��
    129~256��������*/
    uint  ui_ip_mask;
    uchar uc_ipv4[HI_DP_IPV4_ADDR_BYTE_NUM];
    uchar uc_ipv6[HI_DP_IPV6_ADDR_BYTE_NUM];
} hi_dp_l3_rsp_table_item_s;
 #endif
/* END: Added by l00164498, 2012/8/6 */

#endif /* end of __HI_DRV_DP_L3_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
