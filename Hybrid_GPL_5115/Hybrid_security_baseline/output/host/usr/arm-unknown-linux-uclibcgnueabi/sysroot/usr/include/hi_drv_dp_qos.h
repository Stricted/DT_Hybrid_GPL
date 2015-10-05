/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_qos.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2009年2月4日
  最近修改   :
  功能描述   : QOS 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年2月4日
    作    者   : f00125610
    修改内容   : 创建文件

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

 #define  HI_DP_QOS_QUEUE_NUM 128           /* QOS 队列调度中的实体队列数量   */
 #define  HI_DP_QOS_GS_ID_NUM 32            /* QOS 队列调度中的组调度器数量   */
 #define  HI_DP_QOS_EGR_NUM HI_DP_QOS_EGR_NUM_E /* QOS 队列调度中的端口调度器数量 */
 #define  HI_DP_QOS_CPU_QUEUE_NUM 8         /* CPU口优先级数量                */
 #define  HI_DP_QOS_PRI_NUM 8               /* QOS 优先级的值共8个 0~7        */
 #define  HI_DP_QOS_GROUPID_NUM 8           /* QOS 流组ID                     */

/* 5115New Add */
 #define HI_DP_QOS_FCAR_NUM 128
 #define HI_DP_QOS_GCAR_NUM 32
 #define HI_DP_QOS_FWRED_TEMP_NUM 16
 #define HI_DP_QOS_GWRED_TEMP_NUM 4
 #define HI_DP_QOS_WRED_COLOR_MAX 16

/* BEGIN: Added by l00164498, 2012/8/9 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
  #define HI_DP_QOS_COLOR_MAP_NUM 8  /* 色敏特性中颜色映射组个数 */
 #endif
/* END: Added by l00164498, 2012/8/9 */

 #ifdef CONFIG_HISI_SD5115_MP_PILOT

/*5115 PILOT Add*/
  #define  HI_DP_QOS_EQG_NUM 4        /* QG的嵌套(Embedded QG )数量,最多支持4个EQG */
  #define  HI_DP_QOS_QG_NUM 24           /* QOS 队列组数量   */
  #define  HI_DP_QOS_QS_NUM 24           /* 队列组可以挂的队列数*/
 #endif
typedef struct
{
    HI_BOOL_E em_cpu_scar_en;         /* CPU端口简单流量抑制使能            */
    uint      ui_cpu_cir;             /* CPU端口承诺访问速率, 单位pps       */
    uint      ui_cpu_cbs;             /* CPU端口承诺突发尺寸, 单位packet    */

    HI_BOOL_E em_local_port_scar_en;  /* 本地端口简单流量抑制使能           */
    uint      ui_local_port_cir;      /* 本地端口承诺访问速率, 单位64kbit/s */
    uint      ui_local_port_cbs;      /* 本地端口承诺突发尺寸, 单位byte     */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_BOOL_E em_bc_scar_up_en;          /* 上行广播报文简单流量抑制使能           */
    uint      ui_bc_cir_up;              /* 上行广播报文承诺访问速率, 单位64kbit/s */
    uint      ui_bc_cbs_up;              /* 上行广播报文承诺突发尺寸, 单位byte     */
    HI_BOOL_E em_bc_scar_dn_en;          /* 下行广播报文简单流量抑制使能           */
    uint      ui_bc_cir_dn;              /* 下行广播报文承诺访问速率, 单位64kbit/s */
    uint      ui_bc_cbs_dn;              /* 下行广播报文承诺突发尺寸, 单位byte     */

    HI_BOOL_E em_unknown_uc_scar_up_en;  /* 上行未知单播报文简单流量抑制使能          */
    uint      ui_unknown_uc_cir_up;      /* 上行未知单播报文承诺访问速率, 单位64kbit/s*/
    uint      ui_unknown_uc_cbs_up;      /* 上行未知单播报文承诺突发尺寸, 单位byte    */

    HI_BOOL_E em_unknown_uc_scar_dn_en;  /* 下行未知单播报文简单流量抑制使能          */
    uint      ui_unknown_uc_cir_dn;      /* 下行未知单播报文承诺访问速率, 单位64kbit/s*/
    uint      ui_unknown_uc_cbs_dn;      /* 下行未知单播报文承诺突发尺寸, 单位byte    */
 #else
    HI_BOOL_E em_bc_scar_en;          /* 广播报文简单流量抑制使能           */
    uint      ui_bc_cir;              /* 广播报文承诺访问速率, 单位64kbit/s */
    uint      ui_bc_cbs;              /* 广播报文承诺突发尺寸, 单位byte     */

    HI_BOOL_E em_unknown_uc_scar_en;  /* 未知单播报文简单流量抑制使能          */
    uint      ui_unknown_uc_cir;      /* 未知单播报文承诺访问速率, 单位64kbit/s*/
    uint      ui_unknown_uc_cbs;      /* 未知单播报文承诺突发尺寸, 单位byte    */
 #endif
} hi_dp_qos_scar_table_s;

typedef struct
{
    uint ui_cbs;    /* 承诺突发尺寸    */
    uint ui_cir;    /* 承诺访问速率    */
} hi_dp_qos_car_para_s;

typedef struct
{
    uint                 ui_car_id;
    HI_BOOL_E            em_valid;
    hi_dp_qos_car_para_s st_car;
} hi_dp_qos_car_table_item_s;

/* 该表整合了TO CPU CAR映射及TO CPU优先级替换 */
typedef struct
{
    HI_DP_TOCPU_REASON_E em_reason;
    HI_BOOL_E            em_pri_rep_en;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_DP_PRI_TYPE_SELECT_E em_pri_type;
 #endif
    uint      ui_pri_rep;
    HI_BOOL_E em_cpu_car_en;
    uint      ui_cpu_car_id;                   /* 依据此ID索引交CPU原因报文CAR表，执行CAR操作 */
} hi_dp_qos_tocpu_reason_table_item_s;

/* 业务组映射 */
typedef struct
{
    uint ui_sfid;                /* 映射流分类ID      */

    uint      ui_group_id;       /* 业务组映射ID      */
    HI_BOOL_E em_point_pri_en;   /* 指定优先级使能    */
    uint      ui_point_pri;      /* 指定优先级，利用该指定优先级参与后续QOS相关处理，包括入队，WRED等 */
} hi_dp_qos_gmap_table_item_s;

typedef struct
{
    /* 二维索引 Egress + sfid */
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

/* 队列出口 */
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
    HI_DP_QOS_CAR_MODE_P_FIRST = 0, /*报文先过P桶*/
    HI_DP_QOS_CAR_MODE_C_FIRST,     /*报文先过C桶*/
} HI_DP_QOS_CAR_MODE;

typedef struct
{
    uint               ui_fcar_index; /* [0, 127]        */
    uint               ui_cir;        /* 承诺速率        */
    uint               ui_cbs;        /* 承诺突发尺寸    */
    uint               ui_pir;        /* 峰值速率        */
    uint               ui_pbs;        /* 峰值突发尺寸    */
    HI_BOOL_E          em_token_overflow_en;  /*C桶令牌溢出到P桶使能。0：不使能；1：使能。*/
    HI_DP_QOS_CAR_MODE em_car_mode;           /*CAR模式配置 0：报文先过P桶；1报文先过C桶。*/
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_BOOL_E em_color_aware_en;                    /*CAR模式色敏配置：
                                                        HI_FALSE_E：色盲；
                                                        HI_TRUE_E：色敏。*/
 #endif    /* CONFIG_HISI_SD5115_TYPE_T */

} hi_dp_qos_fcar_table_item_s;

typedef struct
{
    /* 组合索引 */
    uint               ui_gcar_index;
    uint               ui_cir;      /* 承诺速率        */
    uint               ui_cbs;      /* 承诺突发尺寸    */
    uint               ui_pir;      /* 峰值速率        */
    uint               ui_pbs;      /* 峰值突发尺寸    */
    HI_BOOL_E          em_token_overflow_en;   /*C桶令牌溢出到P桶使能。0：不使能；1：使能。*/
    HI_DP_QOS_CAR_MODE em_car_mode;            /*CAR模式配置 0：报文先过P桶；1报文先过C桶。*/
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_BOOL_E em_color_aware_en;                    /*CAR模式色敏配置：
                                                        HI_FALSE_E：色盲；
                                                        HI_TRUE_E：色敏。*/
 #endif    /* CONFIG_HISI_SD5115_TYPE_T */

} hi_dp_qos_gcar_table_item_s;

/* 队列映射表 */
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

/* EQG的共享空间表 */
typedef struct
{
    uint             ui_id;     /*EQG ID :0 ~ 3*/
    HI_BUFFER_TYPE_E em_flag;   /*flag marking using inner buffer or outer, for 5115T only. Others should set to 0*/
    uint             ui_buf;    /*the buffer space for this EQG*/
    uint             ui_qg_id[HI_DP_QOS_QG_NUM]; /*the QGs belong to this EQG*/
} hi_dp_qos_eqg_attr_s;

/* QG的共享空间表 */
typedef struct
{
    uint             ui_id; /*QG ID :0 ~ 23*/
    HI_BUFFER_TYPE_E em_flag;       /*flag marking using inner buffer or outerfor 5115T only.Others should set to 0*/
    uint             ui_buf;/*the buffer space for this QG*/
    uint             ui_q_id[HI_DP_QOS_QS_NUM]; /*the Qs belong to this qg */
} hi_dp_qos_qg_attr_s;
 #endif

/* 队列属性表 */
typedef struct
{
    uint ui_qid;       /* 队列的ID                       */
    uint ui_cbuf;      /* 承诺buffer 数量, 私有资源      */
    uint ui_pbuf;      /* 峰值buffer 数量, 最大可用资源  */
} hi_dp_qos_queue_attr_s;

 #ifdef CONFIG_HISI_SD5115_TYPE_T

/* 队列属性表 */
typedef struct
{
    uint             ui_qid;  /* 队列的ID                       */
    uint             ui_cbuf; /* 承诺buffer 数量, 私有资源      */
    uint             ui_pbuf; /* 峰值buffer 数量, 最大可用资源  */
    HI_BUFFER_TYPE_E em_flag;
} hi_dp_qos_queue_attr_5115t_s;
 #endif

typedef enum
{
    HI_DP_QOS_QUEUE_MEMBER_E,              /* 队列成员     */
    HI_DP_QOS_GSCH_MEMBER_E                /* 组调度器成员 */
} HI_DP_QOS_SCH_MEMBER_TYPE_E;

/* 成员调度模式 */
typedef enum
{
    HI_DP_QOS_SCH_SP_MODE_E = 0,        /* SP调度模式   */
    HI_DP_QOS_SCH_WRR_MODE_E            /* WRR调度模式  */
} HI_DP_QOS_SCH_MODE_E;

typedef struct
{
    HI_BOOL_E                   em_valid;
    HI_DP_QOS_SCH_MEMBER_TYPE_E em_type;        /* 成员类型 */
    uint                        ui_id;          /* 成员ID, 队列成员[0, 127], 组调度器成员[0, 31] */
    HI_DP_QOS_SCH_MODE_E        em_sch_mode;    /* 成员调度模式 */
    uint                        ui_wrr;         /*
                                                MPW:Wrr权重[0, 15]
                                                PILOT:
                                                1）队列调度支持128：1
                                                2）组调度支持64：1 */
} hi_dp_qos_sch_member_attr_s;

typedef struct
{
    HI_DP_QOS_EGR_E             em_egr;
    hi_dp_qos_sch_member_attr_s hi_dp_qos_sch_member_s[8];
} hi_dp_qos_fsch_table_item_s;

typedef struct
{
    uint                        ui_gsch_id; /* 业务组调度ID, 索引[0, 31] */
    hi_dp_qos_sch_member_attr_s hi_dp_qos_sch_member_s[8];
} hi_dp_qos_gsch_table_item_s;

/* 流队列WRED模板颜色标记类型 */
typedef enum
{
    HI_DP_QOS_WRED_TEMP_NOT_SUPPORT_E,  /* 000：不支持WRED */
    HI_DP_QOS_WRED_TEMP_INTERNAL_E,     /* 001：内部标记(根据EFC标记的流颜色 映射color 0 ~ 7 ) */
    HI_DP_QOS_WRED_TEMP_DEI_E,          /* 010：DEI标记 (根据DEI 映射color 0 ~ 1 )*/
    HI_DP_QOS_WRED_TEMP_PCP_E,          /* 011：PCP标记 (根据TAG 的 PBIT 映射color 0 ~ 7 )*/
    HI_DP_QOS_WRED_TEMP_DSCP_AF,        /* 100：DSCP AF标记 根据AF CLASS映射 color 0 ~ 11*/
    HI_DP_QOS_WRED_TEMP_DSCP_TC,        /* 101：DSCP/TC标记 (根据DSCP或TC的高3bit映射color 0 ~ 7)*/
    HI_DP_QOS_WRED_TEMP_CAR,            /* 110：CAR标记 (仅对黄色进行WRED.仅对应 color = 0项的配置.)*/
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
    uint ui_temp_id;   /* WRED模板ID: FWRED[0,15] GWRED[0,3] */

    HI_DP_QOS_WRED_TEMP_COLOR_TYPE_E em_color_type;
    uint                             ui_color; /* color_type不同ui_color的范围不同, 详细见: */
    uint                             ui_thr0;  /* WRED丢弃阈值0 */
    uint                             ui_thr1;  /* WRED丢弃阈值1 */
    uint                             ui_thr2;  /* WRED丢弃阈值2 */
    uint                             ui_thr3;  /* WRED丢弃阈值3 */

    uint ui_p0;           /* WRED丢弃概率0 */
    uint ui_p1;           /* WRED丢弃概率1 */
    uint ui_p2;           /* WRED丢弃概率2 */
    uint ui_p3;           /* WRED丢弃概率3 */
} hi_dp_qos_wred_table_item_s;

typedef enum
{
    HI_DP_QOS_SHP_BPS_E,
    HI_DP_QOS_SHP_PPS_E
} HI_DP_QOS_SHP_TYPE_E;

typedef struct
{
    HI_DP_QOS_EGR_E      em_egr;
    HI_DP_DIR_E          em_adj_dir;     /* 命名需讨论 */
    uint                 ui_adj_len;     /* 0 ~ 0x3F, 默认24 */
    HI_BOOL_E            em_enable;
    HI_DP_QOS_SHP_TYPE_E em_shp_type;    /* 只有CPU端口可以选择PPS,其他端口只能是BPS */
    uint                 ui_egr_rate;
} hi_dp_qos_egr_shaping_table_item_s;

typedef struct
{
    uint ui_qid;      /* 索引: 队列ID */

    HI_BOOL_E            em_enable;
    HI_DP_QOS_SHP_TYPE_E em_shp_type;
    HI_DP_DIR_E          em_adj_dir;
    uint                 ui_adj_len;
    uint                 ui_cir;
    uint                 ui_cbs;
} hi_dp_qos_queue_shaping_table_item_s;

typedef struct
{
    uint ui_gsid;      /* 索引: 队列ID */

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
"CPU端口颜色选择寄存器。
0:外层802.1p；
1:外层DEI;
2:IPv4的DSCP或IPv6的TC域；
3:保留。"
 */
typedef enum
{
    HI_DP_QOS_COLOR_SEL_8021P_E = 0,      //颜色选择域 0：外层802.1p
    HI_DP_QOS_COLOR_SEL_DEI_E,            // 1:外层DEI;
    HI_DP_QOS_COLOR_SEL_DSPC_OR_TC_E,     // 2:IPv4的DSCP或IPv6的TC域；
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
    unsigned int          ui_bit_mask; // 取值范围为0x0000-0xffff
    hi_dp_qos_color_map_s st_map[HI_DP_QOS_COLOR_MAP_NUM];
} hi_dp_qos_color_attr_s;
 #endif

/* END: Added by l00164498, 2012/8/2 */

/* 配置属性 */
typedef struct
{
    HI_BOOL_E em_car_adj_en;  /*是否计算以太网20字节开销*/
    /* BEGIN: Added by l00164498, 2012/8/2 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    hi_dp_qos_color_attr_s st_color_attr; //端口色敏特性配置
 #endif
    /* END: Added by l00164498, 2012/8/2 */
} hi_dp_qos_attr_s;

/* QM 模块的统计信息: 队列调度统计  */
typedef struct
{
    uint ui_qid;                         /* 指定需要获取统计的队列ID     */

    uint ui_queue_in;                    /* 入队成功统计                 */
    uint ui_queue_out;                   /* 出队成功统计                 */
    uint ui_queue_tail_drop;             /* 队列尾丢弃统计               */
    uint ui_queue_buffer_low_drop;       /* 资源不足丢弃统计             */
    uint ui_fwred_drop;                  /* 基于流CAR的WRED丢弃统计      */
    uint ui_gwred_drop;                  /* 基于组CAR的WRED丢弃统计      */
    uint ui_descriptor_low_drop;         /* 描述符不足丢弃统计      */
} hi_dp_qos_queue_cnt_table_item_s;

typedef struct
{
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_scar_all;                      /* 参加简单流量抑制的报文个数   */
    uint ui_scar_cpu_drop;                 /* SCAR TO_CPU 丢包             */
    uint ui_scar_local_drop;               /* SCAR本地转发丢包             */
    uint ui_scar_bc_drop;                  /* SCAR广播丢包                 */
    uint ui_scar_unknown_uc_drop;          /* SCAR未知单播丢包             */
    uint ui_fcar_all;                      /* 参与第一级CAR的报文个数      */
    uint ui_gcar_all;                      /* 参与第二级CAR的报文个数      */
    uint ui_fcar_yellow;                   /* 第一级CAR的黄色报文个数      */
    uint ui_fcar_red;                      /* 第一级CAR的红色报文个数      */
    uint ui_gcar_yellow;                   /* 第二级CAR的黄色报文个数      */
    uint ui_gcar_red;                      /* 第二级CAR的红色报文个数      */
    uint ui_total_share_used[4];           /* 队列共享空间已使用统计       */
    uint ui_find_qid_fail;                 /* 报文获取QID失败统计          */

    uint ui_cpu_queue_car_drop[8];         /* CPU队列CAR丢去统计           */
 #else
    uint ui_scar_all;               /* 参加简单流量抑制的报文个数   */
    uint ui_tocpu_rz_car;           /* 参加tocpu_rz_car抑制的报文个数   */
    uint ui_fcar_igr;               /* 参与第一级入口流CAR的报文个数      */
    uint ui_fcar_egr;               /* 参与第一级出口流CAR的报文个数      */
    uint ui_fcar_igr_yellow;        /* 参与第一级入口流CAR黄色报文个数统计寄存器。*/
    uint ui_fcar_egr_yellow;        /* 参与第一级出口流CAR黄色报文个数统计寄存器。*/
    uint ui_fcar_igr_green;         /* 参与第一级入口流CAR绿色报文个数统计寄存器。*/
    uint ui_fcar_egr_green;         /* 参与第一级出口流CAR绿色报文个数统计寄存器。*/
    uint ui_gcar_all;               /* 参与第二级CAR的报文个数      */
    uint ui_gcar_yellow;            /* 第二级CAR的黄色报文个数      */
    uint ui_gcar_green;             /* 第二级CAR的绿色报文个数      */
    uint ui_total_share_used;       /* 队列共享空间已使用统计       */
    uint ui_find_qid_fail;          /* 报文获取QID失败统计          */
 #endif

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_eqg_ss_cnt[2][4];       /* 队列共享空间已使用统计       */
  #else
    uint ui_eqg_ss_cnt[4];          /* 队列共享空间已使用统计       */
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
    可继续添加....
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
