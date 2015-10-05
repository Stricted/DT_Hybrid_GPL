/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_gpon_dn.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   : GPON DN 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月24日
    作    者   : f00125610
    修改内容   : 创建文件

  2.日    期   : 2011年9月16日
    作    者   : f00125610
    修改内容   : 修改文件

***************************************************************************** */

#ifndef __HI_DRV_DP_GPON_DN_H__
#define __HI_DRV_DP_GPON_DN_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_DP_GPON_KEY_LEN 4   /* 密钥有8组16bit */

/* gpon dn 基本属性配置 */
typedef struct
{
    /* 该属性现在修改为最大和最小两个值配置，需要跟SDK相关开发人员沟通 */
    uint       ui_key_exchange_time;            /* 下行密钥切换时间 */
    uint       ui_key_a[HI_DP_GPON_KEY_LEN];    /* A组密钥[0]对应最高16位，[7]对应最低16位 */
    uint       ui_key_b[HI_DP_GPON_KEY_LEN];    /* B组密钥[0]对应最高16位，[7]对应最低16位 */
    uint       ui_sd_thr;                       /* SD告警门限 */
    uint       ui_sf_thr;                       /* SF告警门限 */
    HI_LEVEL_E em_los_level;                    /* LOS 电平 */

    uint              ui_ber_interval;          /* 误码插入的字节间隔 */
    HI_DP_TRANS_ACT_E em_umc_act;               /* 未知组播过滤或者透传 */
    HI_BOOL_E         em_eapol_en;              /* PON口的EAPOL捕获使能 */

    void (*ploam_call_back)(hi_ploam_message_s * st_ploam);
    void (*omci_call_back)(hi_omci_message_s * st_omci);
} hi_dp_gpon_dn_attr_s;

/* TypeB优化处理 */
typedef enum
{
    HI_DP_GPON_DN_EQD_ADJ = 0,   /* EQD调整消息逻辑处理 */
    HI_DP_GPON_UP_RANGE_TIME_E,  /* range time消息逻辑处理 */
    HI_DP_GPON_DN_POP_UP_E,      /* 广播POP UP消息逻辑处理 */
} HI_DP_GPON_DN_TYPEB_OP_E;

typedef struct
{
    HI_DP_GPON_DN_TYPEB_OP_E em_op;
    HI_BOOL_E                em_enable;
} hi_dp_gpon_dn_typeb_op_s;

/* 下行GEMPORT 类型 */
typedef enum
{
    HI_DP_GPON_DN_OMCI_PORT_E = 0,   /* 用于承载OMCI的Port */
    HI_DP_GPON_DN_PRBS_PORT_E,      /* 用于承载PRBS报文的Port */
    HI_DP_GPON_DN_ETH_PORT_E,        /* 用于承载非组播以太帧的Port */
    HI_DP_GPON_DN_MC_PORT_E,        /* 用于承载组播的Port */
    HI_DP_GPON_DN_PORT_TYPE_NUM_E,
} HI_DP_GPON_DN_PORT_TYPE_E;

/* 下行GEMPORT 表项结构 */
typedef struct
{
    uint                      ui_gemport; /* gemport id, 表项索引 0 - 4095 */
    uint                      ui_pri_remarking; /* 下行优先级remarking配置0 - 7   */
    HI_DP_GPON_DN_PORT_TYPE_E em_type;    /* 端口类型                       */
    HI_BOOL_E                 em_aes_en;  /* 是否加密                       */
    HI_BOOL_E                 em_local;   /* 是否本地port                   */
} hi_dp_gpon_dn_gemport_table_item_s;

typedef struct
{
    uint ui_stop_time;             /* Stop time域值。                */
    uint ui_start_time;            /* Start time域值。               */
    uint ui_flag;                  /* FLAG域值。                     */
    uint ui_alloc_id;              /* ALLOC_ID域值。                 */
    uint ui_eof;                   /* 当前帧BWMAP结束标志。          */
    uint ui_sof;                   /* 当前帧BWMAP起始标志。          */
} hi_dp_gpon_dn_bwmap_table_item_s;

#define HI_DP_GPON_DN_BWMAP_TABLE_SIZE 256

typedef struct
{
    /* 5115修改: st_bwmap_table_item-->st_item */
    hi_dp_gpon_dn_bwmap_table_item_s st_item[HI_DP_GPON_DN_BWMAP_TABLE_SIZE];
} hi_dp_gpon_dn_bwmap_table_s;

typedef struct
{
    uint ui_ploam_proc;             /* PLOAM帧处理计数 */
    uint ui_ploam_callback;         /* 调用回调函数计数 */
    uint ui_ploam_discard;          /* PLOAM帧因回调函数为空处理错误丢弃计数 */
    uint ui_omci_proc;              /* OMCI帧处理计数 */
    uint ui_omci_callback;          /* 调用回调函数计数 */
    uint ui_omci_input;             /* OMCI入队计数 */
    uint ui_omci_output;            /* OMCI出队计数 */
    uint ui_omci_discard;           /* OMCI帧因队列满丢弃计数 */
    uint ui_omci_over_run;          /* OMCI帧一次收包超过指定次数FIFO仍然有报文计数 */
} hi_dp_gpon_dn_cnt_s;

/* 硬件当前正在使用的密钥位置 */
typedef enum
{
    HI_DP_GPON_DN_KEY_A_E = 0,
    HI_DP_GPON_DN_KEY_B_E,
} HI_DP_GPON_DN_KEY_E;

typedef struct
{
    HI_DP_GPON_DN_KEY_E em_current_key;   /* 指示当前使用的是密钥A还是密钥B */
    HI_BOOL_E           em_fec_en;        /* 下行帧FEC解码开关 */
    HI_BOOL_E           em_key_exchange_en; /* 下行密钥切换使能,注意CPU不能清0,硬件切换后会自动清0 */

    HI_DP_ALARM_STATE_E em_los;
    HI_DP_ALARM_STATE_E em_lof;
} hi_dp_gpon_dn_sta_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_GPON_DN_H__ */
