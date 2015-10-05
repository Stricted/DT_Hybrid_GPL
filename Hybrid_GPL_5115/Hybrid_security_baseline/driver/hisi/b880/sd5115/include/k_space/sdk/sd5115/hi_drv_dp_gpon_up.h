/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_gpon_up.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   : GPON UP 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月24日
    作    者   : f00125610
    修改内容   : 创建文件

  2.日    期   : 2011年9月16日
    作    者   : f00125610
    修改内容   : 修改文件
***************************************************************************** */

#ifndef __HI_DRV_DP_GPON_UP_H__
#define __HI_DRV_DP_GPON_UP_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_DP_GPON_UP_GEMPORT_CNT_NUM 128
#define HI_GPON_ALLOCID_INVALID 0xffff
#define HI_GPON_EXCLUSIVE_TCONT 0x80

/* 光模块发光控制模式 */
typedef enum
{
    HI_DP_GPON_UP_TX_AUTO_E = 0,   /* 逻辑自动控制 */
    HI_DP_GPON_UP_TX_ENABLE_E,     /* 常发光 */
    HI_DP_GPON_UP_TX_DISABLE_E,     /* 不发光 */
} HI_DP_GPON_UP_TX_MODE_E;

/* Dying Gasp 发送控制，软件、硬件 */
typedef enum
{
    HI_DP_SOFT_E = 0,
    HI_DP_HARDWARE_E,
} HI_DP_DYINGGASP_SEND_MODE_E;

typedef enum
{
    HI_DP_GPON_UP_DBRU_32B_E = 0,   /* 32字节 */
    HI_DP_GPON_UP_DBRU_48B_E,       /* 48字节 */
    HI_DP_GPON_UP_DBRU_64B_E,       /* 64字节 */
    HI_DP_GPON_UP_DBRU_96B_E,       /* 96字节 */
    HI_DP_GPON_UP_DBRU_128B_E,      /* 128字节 */
    HI_DP_GPON_UP_DBRU_192B_E,      /* 192字节 */
    HI_DP_GPON_UP_DBRU_256B_E,      /* 256字节 */
    HI_DP_GPON_UP_DBRU_384B_E,      /* 384字节 */
    HI_DP_GPON_UP_DBRU_UINT_NUM_E,
} HI_DP_GPON_UP_DBRU_UNIT_E;

typedef struct
{
    uint ui_preamble_type1_bits;        /* TYPE1类型前导码长度 */
    uint ui_preamble_type2_bits;        /* TYPE2类型前导码长度 */
    uint ui_preamble_type3_bytes;       /* TYPE3类型前导码长度 */
    uint ui_preamble_type3_pattern;     /* TYPE3类型前导码样式 */
    uint ui_delimiter;                  /* 定界符样式 */
} hi_dp_gpon_up_plou_s;

typedef struct
{
    uint ui_pre_eqd;       /* 初始化延时 */
    uint ui_eqd;           /* ONU测距后配置的等效延时字节整数 */
    uint ui_eqd_dec;
    uint ui_eqd_inc;
    HI_BOOL_E em_eqd_precision;
} hi_dp_gpon_up_eqd_s;

/* OMCI 相关属性 */
typedef struct
{
    HI_BOOL_E em_cut_en;           /* OMCI切片使能 */
    uint      ui_tcont;            /* OMCI发送的TCONT索引 */
    uint      ui_gemport;          /* CPU发送OMCI信元的Port ID */
} hi_dp_gpon_up_omci_attr_s;

/* 发送相关属性 */
typedef struct
{
    HI_LEVEL_E              em_enable_level; /* 激光器发光使能电平 */
    HI_LEVEL_E              em_sd_level;/* 光模块发光电平 */
    HI_DP_GPON_UP_TX_MODE_E em_tx_mode; /* 发光控制 */
    uint ui_tx_vld_ahead;               /* 光模块发送使能信号提前时间的配置，单位byte */

    uint ui_laser_on_delay;             /* 从PON MAC指示光模块关闭发光到光模块关闭发光状态的延迟 */
    uint ui_laser_off_delay;            /* 从PON MAC指示光模块发光到光模块输出发光状态的延迟 */
} hi_dp_gpon_up_laser_attr_s;

/* gpon up 基本属性配置 */
typedef struct
{
    hi_dp_gpon_up_plou_s      st_plou;       /* Preamble/Delimiter */
    uint                      ui_onu_id;     /* ONU-ID */
    HI_DP_GPON_UP_DBRU_UNIT_E em_dbru_unit;  /* DBRU上报粒度 */

    /* 延时属性配置 */
    uint                ui_fixed_delay;  /* ONU处理固定延时 */
    uint                ui_random_delay; /* 随机延时配置寄存器，随机延时的数值由软件生成 */
    uint                ui_tx_delay;     /* 上行发送数据有效信号的延时周期数 */
    hi_dp_gpon_up_eqd_s st_eqd;

    HI_BOOL_E em_dying_gasp_tx_en;   /* 逻辑是否发送Dyinggasp */
    hi_dp_gpon_up_laser_attr_s st_tx_attr;  /* 激光器发光，发送操作相关配置 */
    hi_dp_gpon_up_omci_attr_s  st_omci;
} hi_dp_gpon_up_attr_s;

/* 上行GEMPORT 映射表表项结构 */
typedef struct
{
    uint ui_sfid;             /* 流分类ID, 索引 */
    uint ui_gemport;          /* 映射GEMPORT ID */
    uint ui_tcont;            /* 映射TCONT ID   */
} hi_dp_gpon_up_map_table_item_s;

/* 上行TCOUNT 表项结构 */
typedef struct
{
    uint ui_tcont;               /* TCOUNT  ID 0~8, 0-7表示普通tcont，8只能承载OMCI */
    uint      ui_alloc_id;       /* allocid */
    HI_BOOL_E em_valid;
} hi_dp_gpon_up_tcont_table_item_s;

typedef struct
{
    uint ui_ploam_tx;       /* 驱动发送PLOAM帧计数 */
    uint ui_omci_tx;        /* 驱动发送OMCI帧计数 */
} hi_dp_gpon_up_cnt_s;


typedef struct 
{
}hi_dp_gpon_up_sta_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_GPON_UP_H__ */
