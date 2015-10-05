/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_epon_up.h
  版 本 号   : 初稿
  作    者   : 区伟权
  生成日期   : 2009年7月13日
  最近修改   :
  功能描述   : EPON上行用户头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年7月13日
    作    者   : 区伟权
    修改内容   : 创建文件

  2.日    期   : 2011年9月16日
    作    者   : f00125610
    修改内容   : 修改文件
***************************************************************************** */

#ifndef __HI_DRV_DP_EPON_UP_H__
#define __HI_DRV_DP_EPON_UP_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* ----------------------------------------------*
 * 外部变量说明                                 *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 外部函数原型说明                             *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 内部函数原型说明                             *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 全局变量                                     *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 模块级变量                                   *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 常量定义                                     *
 *---------------------------------------------- */

/* ----------------------------------------------*
 * 宏定义                                       *
 *---------------------------------------------- */



typedef enum
{
    HI_DP_EPON_UP_QUEUE0_E = 0,
    HI_DP_EPON_UP_QUEUE1_E,
    HI_DP_EPON_UP_QUEUE2_E,
    HI_DP_EPON_UP_QUEUE3_E,
    HI_DP_EPON_UP_QUEUE4_E,
    HI_DP_EPON_UP_QUEUE5_E,
    HI_DP_EPON_UP_QUEUE6_E,
    HI_DP_EPON_UP_QUEUE7_E,
    HI_DP_EPON_UP_QUEUE_NUM,
}HI_DP_EPON_UP_QUEUE_E;


/* 光模块发光控制模式 */
typedef enum
{
    HI_DP_EPON_UP_TX_AUTO_E  = 0,   /* 逻辑自动控制 */
    HI_DP_EPON_UP_TX_ENABLE_E ,     /* 常发光 */
    HI_DP_EPON_UP_TX_DISABLE_E,     /* 不发光 */
}HI_DP_EPON_UP_TX_MODE_E;


/* 激光器发光，发送相关配置 */
typedef struct
{
    HI_LEVEL_E em_enable_level;              /* 激光器发光使能有效电平*/
    HI_LEVEL_E em_sd_level;                  /* 光模块输出的TX SD信号的极性。*/
    HI_DP_EPON_UP_TX_MODE_E em_tx_mode;      /* 发光控制模式*/    

    uint ui_ctrl_delay;                 /* Laser Control时序延迟调整配置。单位为16ns */ 
    uint ui_laser_on_delay;             /* 从PON MAC指示光模块关闭发光到光模块关闭发光状态的延迟 */
    uint ui_laser_off_delay;            /* 从PON MAC指示光模块发光到光模块输出发光状态的延迟 */
}hi_dp_epon_up_laser_attr_s;


/* MPCP注册配置 */
typedef struct
{
    HI_BOOL_E em_reg_en;         /* 注册使能 */
    uint ui_reg_llid_index;      /* 要注册的LLID索引 */
    uint ui_dis_sync_time;       /* Discovery Grant 的同步时间 */
    uint ui_dis_random_dly;      /* Discovery Grant 的随机延时 */
    uint ui_sync_time;           /* Normal Grant 的同步时间 */
}hi_dp_epon_up_register_attr_s;


/* 配置属性 */
typedef struct
{
    HI_BOOL_E em_fec_en[HI_DP_EPON_LLID_NUM];        /* FEC编码使能 */
#if defined(CONFIG_HISI_SD5115_TYPE_T)
    HI_BOOL_E em_fec_adapt_en;                       /* 5115T add: 上行FEC自适应使能 */
#endif
    uint  ui_ipg;                                    /* 上行发送帧间隔 单位为1BYTE时间，默认为12 */
    hi_dp_epon_up_laser_attr_s st_tx_attr;           /* 激光器发光，发送操作相关配置 */
    HI_BOOL_E em_dying_gasp_tx_en;                   /* 逻辑是否发送Dyinggasp */
    hi_dp_epon_up_register_attr_s st_reg_attr;       /* MPCP注册属性 */

}hi_dp_epon_up_attr_s;


/* 上行GEMPORT 映射表表项结构 */
typedef struct
{
    uint ui_sfid;           /* 流分类ID, 索引 */
    uint ui_llid;           /* LLID */
}hi_dp_epon_up_map_table_item_s;


#define HI_DP_EPON_THRESH_NUM   3

typedef struct
{
    HI_BOOL_E em_enable;
    uint      ui_report;    /* Report值只读 */
    ushort    us_thresh[HI_DP_EPON_THRESH_NUM];
    ushort    reserved;
}hi_dp_epon_up_report_queue_s;


/* Report参数表项数据结构 */
typedef struct
{
    uint ui_llid;
    uint ui_qsets_num;      /* 队列集数量  1 - 4 */
    hi_dp_epon_up_report_queue_s st_queue[HI_DP_EPON_UP_QUEUE_NUM];
}hi_dp_epon_up_report_table_item_s;


/* 状态 */
typedef struct
{

}hi_dp_epon_up_sta_s;


/* 统计信息 */
typedef struct
{
    ulong ul_tx_mpcp;
    ulong ul_tx_oam;
}hi_dp_epon_up_cnt_s;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_EPON_UP_H__ */

