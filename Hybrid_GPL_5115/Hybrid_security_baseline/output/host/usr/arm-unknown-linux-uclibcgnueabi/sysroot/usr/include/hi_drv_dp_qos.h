/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_drv_dp_qos.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2009��2��4��
  ����޸�   :
  ��������   : QOS ģ�����ݽṹͷ�ļ�
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

#ifndef __HI_DRV_DP_QOS_H__
 #define __HI_DRV_DP_QOS_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"

 #define  HI_DP_QOS_QUEUE_NUM 128           /* QOS ���е����е�ʵ���������   */
 #define  HI_DP_QOS_GS_ID_NUM 32            /* QOS ���е����е������������   */
 #define  HI_DP_QOS_EGR_NUM HI_DP_QOS_EGR_NUM_E /* QOS ���е����еĶ˿ڵ��������� */
 #define  HI_DP_QOS_CPU_QUEUE_NUM 8         /* CPU�����ȼ�����                */
 #define  HI_DP_QOS_PRI_NUM 8               /* QOS ���ȼ���ֵ��8�� 0~7        */
 #define  HI_DP_QOS_GROUPID_NUM 8           /* QOS ����ID                     */

/* 5115New Add */
 #define HI_DP_QOS_FCAR_NUM 128
 #define HI_DP_QOS_GCAR_NUM 32
 #define HI_DP_QOS_FWRED_TEMP_NUM 16
 #define HI_DP_QOS_GWRED_TEMP_NUM 4
 #define HI_DP_QOS_WRED_COLOR_MAX 16

/* BEGIN: Added by l00164498, 2012/8/9 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
  #define HI_DP_QOS_COLOR_MAP_NUM 8  /* ɫ����������ɫӳ������� */
 #endif
/* END: Added by l00164498, 2012/8/9 */

 #ifdef CONFIG_HISI_SD5115_MP_PILOT

/*5115 PILOT Add*/
  #define  HI_DP_QOS_EQG_NUM 4        /* QG��Ƕ��(Embedded QG )����,���֧��4��EQG */
  #define  HI_DP_QOS_QG_NUM 24           /* QOS ����������   */
  #define  HI_DP_QOS_QS_NUM 24           /* ��������ԹҵĶ�����*/
 #endif
typedef struct
{
    HI_BOOL_E em_cpu_scar_en;         /* CPU�˿ڼ���������ʹ��            */
    uint      ui_cpu_cir;             /* CPU�˿ڳ�ŵ��������, ��λpps       */
    uint      ui_cpu_cbs;             /* CPU�˿ڳ�ŵͻ���ߴ�, ��λpacket    */

    HI_BOOL_E em_local_port_scar_en;  /* ���ض˿ڼ���������ʹ��           */
    uint      ui_local_port_cir;      /* ���ض˿ڳ�ŵ��������, ��λ64kbit/s */
    uint      ui_local_port_cbs;      /* ���ض˿ڳ�ŵͻ���ߴ�, ��λbyte     */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_BOOL_E em_bc_scar_up_en;          /* ���й㲥���ļ���������ʹ��           */
    uint      ui_bc_cir_up;              /* ���й㲥���ĳ�ŵ��������, ��λ64kbit/s */
    uint      ui_bc_cbs_up;              /* ���й㲥���ĳ�ŵͻ���ߴ�, ��λbyte     */
    HI_BOOL_E em_bc_scar_dn_en;          /* ���й㲥���ļ���������ʹ��           */
    uint      ui_bc_cir_dn;              /* ���й㲥���ĳ�ŵ��������, ��λ64kbit/s */
    uint      ui_bc_cbs_dn;              /* ���й㲥���ĳ�ŵͻ���ߴ�, ��λbyte     */

    HI_BOOL_E em_unknown_uc_scar_up_en;  /* ����δ֪�������ļ���������ʹ��          */
    uint      ui_unknown_uc_cir_up;      /* ����δ֪�������ĳ�ŵ��������, ��λ64kbit/s*/
    uint      ui_unknown_uc_cbs_up;      /* ����δ֪�������ĳ�ŵͻ���ߴ�, ��λbyte    */

    HI_BOOL_E em_unknown_uc_scar_dn_en;  /* ����δ֪�������ļ���������ʹ��          */
    uint      ui_unknown_uc_cir_dn;      /* ����δ֪�������ĳ�ŵ��������, ��λ64kbit/s*/
    uint      ui_unknown_uc_cbs_dn;      /* ����δ֪�������ĳ�ŵͻ���ߴ�, ��λbyte    */
 #else
    HI_BOOL_E em_bc_scar_en;          /* �㲥���ļ���������ʹ��           */
    uint      ui_bc_cir;              /* �㲥���ĳ�ŵ��������, ��λ64kbit/s */
    uint      ui_bc_cbs;              /* �㲥���ĳ�ŵͻ���ߴ�, ��λbyte     */

    HI_BOOL_E em_unknown_uc_scar_en;  /* δ֪�������ļ���������ʹ��          */
    uint      ui_unknown_uc_cir;      /* δ֪�������ĳ�ŵ��������, ��λ64kbit/s*/
    uint      ui_unknown_uc_cbs;      /* δ֪�������ĳ�ŵͻ���ߴ�, ��λbyte    */
 #endif
} hi_dp_qos_scar_table_s;

typedef struct
{
    uint ui_cbs;    /* ��ŵͻ���ߴ�    */
    uint ui_cir;    /* ��ŵ��������    */
} hi_dp_qos_car_para_s;

typedef struct
{
    uint                 ui_car_id;
    HI_BOOL_E            em_valid;
    hi_dp_qos_car_para_s st_car;
} hi_dp_qos_car_table_item_s;

/* �ñ�������TO CPU CARӳ�估TO CPU���ȼ��滻 */
typedef struct
{
    HI_DP_TOCPU_REASON_E em_reason;
    HI_BOOL_E            em_pri_rep_en;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_DP_PRI_TYPE_SELECT_E em_pri_type;
 #endif
    uint      ui_pri_rep;
    HI_BOOL_E em_cpu_car_en;
    uint      ui_cpu_car_id;                   /* ���ݴ�ID������CPUԭ����CAR��ִ��CAR���� */
} hi_dp_qos_tocpu_reason_table_item_s;

/* ҵ����ӳ�� */
typedef struct
{
    uint ui_sfid;                /* ӳ��������ID      */

    uint      ui_group_id;       /* ҵ����ӳ��ID      */
    HI_BOOL_E em_point_pri_en;   /* ָ�����ȼ�ʹ��    */
    uint      ui_point_pri;      /* ָ�����ȼ������ø�ָ�����ȼ��������QOS��ش���������ӣ�WRED�� */
} hi_dp_qos_gmap_table_item_s;

typedef struct
{
    /* ��ά���� Egress + sfid */
    HI_DP_PORT_E em_egr;
    uint         ui_sfid;

    HI_BOOL_E em_vld;
    uint      ui_fid;
} hi_dp_qos_epfc_map_table_item_s;

typedef struct
{
    uint      ui_fid;
    HI_BOOL_E em_vld;
    uint      ui_fcar_index;
} hi_dp_qos_fcar_map_table_item_s;

/* ���г��� */
typedef enum
{
    HI_DP_QOS_EGR_TCONT0_E = 0,
    HI_DP_QOS_EGR_TCONT1_E,
    HI_DP_QOS_EGR_TCONT2_E,
    HI_DP_QOS_EGR_TCONT3_E,
    HI_DP_QOS_EGR_TCONT4_E,
    HI_DP_QOS_EGR_TCONT5_E,
    HI_DP_QOS_EGR_TCONT6_E,
    HI_DP_QOS_EGR_TCONT7_E,

    HI_DP_QOS_EGR_GE0_E,

    HI_DP_QOS_EGR_GE1_E,
    HI_DP_QOS_EGR_FE2_E,
    HI_DP_QOS_EGR_FE3_E,
    HI_DP_QOS_EGR_FE4_E,

    HI_DP_QOS_EGR_CPU_E,

    HI_DP_QOS_EGR_PRBS_E,

    HI_DP_QOS_EGR_NUM_E
} HI_DP_QOS_EGR_E;
//#endif
typedef struct
{
    uint            ui_gid;
    HI_DP_QOS_EGR_E em_egr;
    HI_BOOL_E       em_vld;
    uint            ui_gcar_index;
} hi_dp_qos_gcar_map_table_item_s;

typedef enum
{
    HI_DP_QOS_CAR_MODE_P_FIRST = 0, /*�����ȹ�PͰ*/
    HI_DP_QOS_CAR_MODE_C_FIRST,     /*�����ȹ�CͰ*/
} HI_DP_QOS_CAR_MODE;

typedef struct
{
    uint               ui_fcar_index; /* [0, 127]        */
    uint               ui_cir;        /* ��ŵ����        */
    uint               ui_cbs;        /* ��ŵͻ���ߴ�    */
    uint               ui_pir;        /* ��ֵ����        */
    uint               ui_pbs;        /* ��ֵͻ���ߴ�    */
    HI_BOOL_E          em_token_overflow_en;  /*CͰ���������PͰʹ�ܡ�0����ʹ�ܣ�1��ʹ�ܡ�*/
    HI_DP_QOS_CAR_MODE em_car_mode;           /*CARģʽ���� 0�������ȹ�PͰ��1�����ȹ�CͰ��*/
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_BOOL_E em_color_aware_en;                    /*CARģʽɫ�����ã�
                                                        HI_FALSE_E��ɫä��
                                                        HI_TRUE_E��ɫ����*/
 #endif    /* CONFIG_HISI_SD5115_TYPE_T */

} hi_dp_qos_fcar_table_item_s;

typedef struct
{
    /* ������� */
    uint               ui_gcar_index;
    uint               ui_cir;      /* ��ŵ����        */
    uint               ui_cbs;      /* ��ŵͻ���ߴ�    */
    uint               ui_pir;      /* ��ֵ����        */
    uint               ui_pbs;      /* ��ֵͻ���ߴ�    */
    HI_BOOL_E          em_token_overflow_en;   /*CͰ���������PͰʹ�ܡ�0����ʹ�ܣ�1��ʹ�ܡ�*/
    HI_DP_QOS_CAR_MODE em_car_mode;            /*CARģʽ���� 0�������ȹ�PͰ��1�����ȹ�CͰ��*/
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_BOOL_E em_color_aware_en;                    /*CARģʽɫ�����ã�
                                                        HI_FALSE_E��ɫä��
                                                        HI_TRUE_E��ɫ����*/
 #endif    /* CONFIG_HISI_SD5115_TYPE_T */

} hi_dp_qos_gcar_table_item_s;

/* ����ӳ��� */
typedef struct
{
    HI_DP_QOS_EGR_E em_egr;
    uint            ui_group_id;
    uint            ui_pri;

    HI_BOOL_E em_valid;
    uint      ui_qid;
} hi_dp_qos_qmap_table_item_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_OUTER_E = 0,
    HI_INNER_E
} HI_BUFFER_TYPE_E;       /*flag making using inner buffer or outer*/

/* EQG�Ĺ���ռ�� */
typedef struct
{
    uint             ui_id;     /*EQG ID :0 ~ 3*/
    HI_BUFFER_TYPE_E em_flag;   /*flag marking using inner buffer or outer, for 5115T only. Others should set to 0*/
    uint             ui_buf;    /*the buffer space for this EQG*/
    uint             ui_qg_id[HI_DP_QOS_QG_NUM]; /*the QGs belong to this EQG*/
} hi_dp_qos_eqg_attr_s;

/* QG�Ĺ���ռ�� */
typedef struct
{
    uint             ui_id; /*QG ID :0 ~ 23*/
    HI_BUFFER_TYPE_E em_flag;       /*flag marking using inner buffer or outerfor 5115T only.Others should set to 0*/
    uint             ui_buf;/*the buffer space for this QG*/
    uint             ui_q_id[HI_DP_QOS_QS_NUM]; /*the Qs belong to this qg */
} hi_dp_qos_qg_attr_s;
 #endif

/* �������Ա� */
typedef struct
{
    uint ui_qid;       /* ���е�ID                       */
    uint ui_cbuf;      /* ��ŵbuffer ����, ˽����Դ      */
    uint ui_pbuf;      /* ��ֵbuffer ����, ��������Դ  */
} hi_dp_qos_queue_attr_s;

 #ifdef CONFIG_HISI_SD5115_TYPE_T

/* �������Ա� */
typedef struct
{
    uint             ui_qid;  /* ���е�ID                       */
    uint             ui_cbuf; /* ��ŵbuffer ����, ˽����Դ      */
    uint             ui_pbuf; /* ��ֵbuffer ����, ��������Դ  */
    HI_BUFFER_TYPE_E em_flag;
} hi_dp_qos_queue_attr_5115t_s;
 #endif

typedef enum
{
    HI_DP_QOS_QUEUE_MEMBER_E,              /* ���г�Ա     */
    HI_DP_QOS_GSCH_MEMBER_E                /* ���������Ա */
} HI_DP_QOS_SCH_MEMBER_TYPE_E;

/* ��Ա����ģʽ */
typedef enum
{
    HI_DP_QOS_SCH_SP_MODE_E = 0,        /* SP����ģʽ   */
    HI_DP_QOS_SCH_WRR_MODE_E            /* WRR����ģʽ  */
} HI_DP_QOS_SCH_MODE_E;

typedef struct
{
    HI_BOOL_E                   em_valid;
    HI_DP_QOS_SCH_MEMBER_TYPE_E em_type;        /* ��Ա���� */
    uint                        ui_id;          /* ��ԱID, ���г�Ա[0, 127], ���������Ա[0, 31] */
    HI_DP_QOS_SCH_MODE_E        em_sch_mode;    /* ��Ա����ģʽ */
    uint                        ui_wrr;         /*
                                                MPW:WrrȨ��[0, 15]
                                                PILOT:
                                                1�����е���֧��128��1
                                                2�������֧��64��1 */
} hi_dp_qos_sch_member_attr_s;

typedef struct
{
    HI_DP_QOS_EGR_E             em_egr;
    hi_dp_qos_sch_member_attr_s hi_dp_qos_sch_member_s[8];
} hi_dp_qos_fsch_table_item_s;

typedef struct
{
    uint                        ui_gsch_id; /* ҵ�������ID, ����[0, 31] */
    hi_dp_qos_sch_member_attr_s hi_dp_qos_sch_member_s[8];
} hi_dp_qos_gsch_table_item_s;

/* ������WREDģ����ɫ������� */
typedef enum
{
    HI_DP_QOS_WRED_TEMP_NOT_SUPPORT_E,  /* 000����֧��WRED */
    HI_DP_QOS_WRED_TEMP_INTERNAL_E,     /* 001���ڲ����(����EFC��ǵ�����ɫ ӳ��color 0 ~ 7 ) */
    HI_DP_QOS_WRED_TEMP_DEI_E,          /* 010��DEI��� (����DEI ӳ��color 0 ~ 1 )*/
    HI_DP_QOS_WRED_TEMP_PCP_E,          /* 011��PCP��� (����TAG �� PBIT ӳ��color 0 ~ 7 )*/
    HI_DP_QOS_WRED_TEMP_DSCP_AF,        /* 100��DSCP AF��� ����AF CLASSӳ�� color 0 ~ 11*/
    HI_DP_QOS_WRED_TEMP_DSCP_TC,        /* 101��DSCP/TC��� (����DSCP��TC�ĸ�3bitӳ��color 0 ~ 7)*/
    HI_DP_QOS_WRED_TEMP_CAR,            /* 110��CAR��� (���Ի�ɫ����WRED.����Ӧ color = 0�������.)*/
} HI_DP_QOS_WRED_TEMP_COLOR_TYPE_E;

typedef struct
{
    uint      ui_qid;
    HI_BOOL_E em_vld;
    uint      ui_fw_temp_id;
} hi_dp_qos_fwred_map_table_item_s;

typedef struct
{
    uint      ui_gsid;
    HI_BOOL_E em_vld;
    uint      ui_gw_temp_id;
} hi_dp_qos_gwred_map_table_item_s;

typedef struct
{
    uint ui_temp_id;   /* WREDģ��ID: FWRED[0,15] GWRED[0,3] */

    HI_DP_QOS_WRED_TEMP_COLOR_TYPE_E em_color_type;
    uint                             ui_color; /* color_type��ͬui_color�ķ�Χ��ͬ, ��ϸ��: */
    uint                             ui_thr0;  /* WRED������ֵ0 */
    uint                             ui_thr1;  /* WRED������ֵ1 */
    uint                             ui_thr2;  /* WRED������ֵ2 */
    uint                             ui_thr3;  /* WRED������ֵ3 */

    uint ui_p0;           /* WRED��������0 */
    uint ui_p1;           /* WRED��������1 */
    uint ui_p2;           /* WRED��������2 */
    uint ui_p3;           /* WRED��������3 */
} hi_dp_qos_wred_table_item_s;

typedef enum
{
    HI_DP_QOS_SHP_BPS_E,
    HI_DP_QOS_SHP_PPS_E
} HI_DP_QOS_SHP_TYPE_E;

typedef struct
{
    HI_DP_QOS_EGR_E      em_egr;
    HI_DP_DIR_E          em_adj_dir;     /* ���������� */
    uint                 ui_adj_len;     /* 0 ~ 0x3F, Ĭ��24 */
    HI_BOOL_E            em_enable;
    HI_DP_QOS_SHP_TYPE_E em_shp_type;    /* ֻ��CPU�˿ڿ���ѡ��PPS,�����˿�ֻ����BPS */
    uint                 ui_egr_rate;
} hi_dp_qos_egr_shaping_table_item_s;

typedef struct
{
    uint ui_qid;      /* ����: ����ID */

    HI_BOOL_E            em_enable;
    HI_DP_QOS_SHP_TYPE_E em_shp_type;
    HI_DP_DIR_E          em_adj_dir;
    uint                 ui_adj_len;
    uint                 ui_cir;
    uint                 ui_cbs;
} hi_dp_qos_queue_shaping_table_item_s;

typedef struct
{
    uint ui_gsid;      /* ����: ����ID */

    HI_BOOL_E            em_enable;
    HI_DP_QOS_SHP_TYPE_E em_shp_type;
    HI_DP_DIR_E          em_adj_dir;
    uint                 ui_adj_len;
    uint                 ui_cir;
    uint                 ui_cbs;
} hi_dp_qos_gs_shaping_table_item_s;

typedef struct
{
    HI_BOOL_E em_enable;
    uint      ui_cbs;
    uint      ui_cir;
} hi_dp_qos_cpu_car_attr_s;
 #ifdef CONFIG_HISI_SD5115_MP_MPW
typedef struct
{
    HI_DP_DIR_E em_up_adj_dir;
    uint        ui_up_adj_len;

    HI_DP_DIR_E em_dn_adj_dir;
    uint        ui_dn_adj_len;
} hi_dp_qos_car_adj_s;
 #endif

/* BEGIN: Added by l00164498, 2012/8/2 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T

/*
"CPU�˿���ɫѡ��Ĵ�����
0:���802.1p��
1:���DEI;
2:IPv4��DSCP��IPv6��TC��
3:������"
 */
typedef enum
{
    HI_DP_QOS_COLOR_SEL_8021P_E = 0,      //��ɫѡ���� 0�����802.1p
    HI_DP_QOS_COLOR_SEL_DEI_E,            // 1:���DEI;
    HI_DP_QOS_COLOR_SEL_DSPC_OR_TC_E,     // 2:IPv4��DSCP��IPv6��TC��
    HI_DP_QOS_COLOR_NUM_E
} HI_DP_QOS_COLOR_SEL_E;

typedef enum
{
    HI_DP_QOS_COLOR_GREEN_E = 0,
    HI_DP_QOS_COLOR_YELLOW_E,
    HI_DP_QOS_COLOR_RED_E,
} HI_DP_QOS_COLOR_E;

typedef struct
{
    uint              ui_color_key;
    HI_DP_QOS_COLOR_E em_color;
} hi_dp_qos_color_map_s;

typedef struct
{
    HI_DP_PORT_E          em_port_id;  //0-7
    HI_DP_QOS_COLOR_SEL_E em_sel;      //0,1,2,3
    unsigned int          ui_bit_mask; // ȡֵ��ΧΪ0x0000-0xffff
    hi_dp_qos_color_map_s st_map[HI_DP_QOS_COLOR_MAP_NUM];
} hi_dp_qos_color_attr_s;
 #endif

/* END: Added by l00164498, 2012/8/2 */

/* �������� */
typedef struct
{
    HI_BOOL_E em_car_adj_en;  /*�Ƿ������̫��20�ֽڿ���*/
    /* BEGIN: Added by l00164498, 2012/8/2 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    hi_dp_qos_color_attr_s st_color_attr; //�˿�ɫ����������
 #endif
    /* END: Added by l00164498, 2012/8/2 */
} hi_dp_qos_attr_s;

/* QM ģ���ͳ����Ϣ: ���е���ͳ��  */
typedef struct
{
    uint ui_qid;                         /* ָ����Ҫ��ȡͳ�ƵĶ���ID     */

    uint ui_queue_in;                    /* ��ӳɹ�ͳ��                 */
    uint ui_queue_out;                   /* ���ӳɹ�ͳ��                 */
    uint ui_queue_tail_drop;             /* ����β����ͳ��               */
    uint ui_queue_buffer_low_drop;       /* ��Դ���㶪��ͳ��             */
    uint ui_fwred_drop;                  /* ������CAR��WRED����ͳ��      */
    uint ui_gwred_drop;                  /* ������CAR��WRED����ͳ��      */
    uint ui_descriptor_low_drop;         /* ���������㶪��ͳ��      */
} hi_dp_qos_queue_cnt_table_item_s;

typedef struct
{
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_scar_all;                      /* �μӼ��������Ƶı��ĸ���   */
    uint ui_scar_cpu_drop;                 /* SCAR TO_CPU ����             */
    uint ui_scar_local_drop;               /* SCAR����ת������             */
    uint ui_scar_bc_drop;                  /* SCAR�㲥����                 */
    uint ui_scar_unknown_uc_drop;          /* SCARδ֪��������             */
    uint ui_fcar_all;                      /* �����һ��CAR�ı��ĸ���      */
    uint ui_gcar_all;                      /* ����ڶ���CAR�ı��ĸ���      */
    uint ui_fcar_yellow;                   /* ��һ��CAR�Ļ�ɫ���ĸ���      */
    uint ui_fcar_red;                      /* ��һ��CAR�ĺ�ɫ���ĸ���      */
    uint ui_gcar_yellow;                   /* �ڶ���CAR�Ļ�ɫ���ĸ���      */
    uint ui_gcar_red;                      /* �ڶ���CAR�ĺ�ɫ���ĸ���      */
    uint ui_total_share_used[4];           /* ���й���ռ���ʹ��ͳ��       */
    uint ui_find_qid_fail;                 /* ���Ļ�ȡQIDʧ��ͳ��          */

    uint ui_cpu_queue_car_drop[8];         /* CPU����CAR��ȥͳ��           */
 #else
    uint ui_scar_all;               /* �μӼ��������Ƶı��ĸ���   */
    uint ui_tocpu_rz_car;           /* �μ�tocpu_rz_car���Ƶı��ĸ���   */
    uint ui_fcar_igr;               /* �����һ�������CAR�ı��ĸ���      */
    uint ui_fcar_egr;               /* �����һ��������CAR�ı��ĸ���      */
    uint ui_fcar_igr_yellow;        /* �����һ�������CAR��ɫ���ĸ���ͳ�ƼĴ�����*/
    uint ui_fcar_egr_yellow;        /* �����һ��������CAR��ɫ���ĸ���ͳ�ƼĴ�����*/
    uint ui_fcar_igr_green;         /* �����һ�������CAR��ɫ���ĸ���ͳ�ƼĴ�����*/
    uint ui_fcar_egr_green;         /* �����һ��������CAR��ɫ���ĸ���ͳ�ƼĴ�����*/
    uint ui_gcar_all;               /* ����ڶ���CAR�ı��ĸ���      */
    uint ui_gcar_yellow;            /* �ڶ���CAR�Ļ�ɫ���ĸ���      */
    uint ui_gcar_green;             /* �ڶ���CAR����ɫ���ĸ���      */
    uint ui_total_share_used;       /* ���й���ռ���ʹ��ͳ��       */
    uint ui_find_qid_fail;          /* ���Ļ�ȡQIDʧ��ͳ��          */
 #endif

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_eqg_ss_cnt[2][4];       /* ���й���ռ���ʹ��ͳ��       */
  #else
    uint ui_eqg_ss_cnt[4];          /* ���й���ռ���ʹ��ͳ��       */
  #endif
 #endif
} hi_dp_qos_cnt_s;

typedef struct
{
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_total_share[2];
 #else
    uint ui_total_share;
 #endif
} hi_dp_qos_sta_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef struct
{
    uint ui_id;            /* 0 - 1 */
    uint ui_pri_reset[8];  /* 0 - 7*/
} hi_dp_qos_pri_reset_table_s;
 #endif
typedef struct
{
    uint ui_p0;
    uint ui_p1;
    uint ui_p2;
    uint ui_p3;
    uint ui_p4;
    uint ui_p5;
    uint ui_p6;
    uint ui_p7;
    uint ui_p8;
    uint ui_p9;
    uint ui_p10;
    uint ui_p11;

    /*
    �ɼ������....
     */
} hi_dp_qos_debug_s;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define HI_DP_DROP_REASON_NUM 128 //lKF55238
typedef struct
{
    uint cnt[HI_DP_PORT_NUM + 1][HI_DP_DROP_NUM_E];
} hi_dp_qos_drop_reason_cnt_s;
 #endif
#endif /* end of __HI_DRV_DP_QOS_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
