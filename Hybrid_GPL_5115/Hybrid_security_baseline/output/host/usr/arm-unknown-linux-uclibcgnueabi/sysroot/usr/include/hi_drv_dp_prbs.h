/******************************************************************************

                  版权所有 (C), 2001-2011, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_prbs.h
  版 本 号   : 初稿
  作    者   : shengjing
  生成日期   : 2011年12月16日
  最近修改   :
  功能描述   : SD5115 SDK prbs模块头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年12月16日
    作    者   : shengjing
    修改内容   : 创建文件

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus  */


#ifndef __HI_DRV_DP_PRBS_H__
#define __HI_DRV_DP_PRBS_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#define HI_DP_PRBS_CHANNEL_NUM 8  /* PRBS通道数 */
#define HI_DP_PRBS_MAX_LENGTH   9600
#define HI_DP_PRBS_MIN_LENGTH   64

/* 校验模式 */
typedef enum
{
    HI_DP_PRBS_RX_MODE_CONTINUE_E = 0,    /* 连续 */   
    HI_DP_PRBS_RX_MODE_SINGLE_E,    /* 单包 */        
}HI_DP_PRBS_RX_MODE_E;

#ifdef CONFIG_HISI_SD5115_TYPE_T
typedef struct
{
    HI_BOOL_E em_tx_seq_num_clr;   /* 发送TST帧序号清零 */
    HI_BOOL_E em_rx_seq_num_ini;   /* 接收TST帧序号初始化 */
} hi_dp_prbs_seq_num_cfg_s;

typedef struct
{
    HI_BOOL_E em_tx_crc_op_en;     /* CRC生成码取反 */
    HI_BOOL_E em_rx_crc_chk_en;    /* CRC校验使能 */
} hi_dp_prbs_crc_en_s;

typedef enum
{
    HI_DP_NULL_SIGNAL_WITHOUT_CRC32_E = 0,    /* Null signal without CRC-32 */   
    HI_DP_NULL_SIGNAL_WITH_CRC32_E,            /* Null signal with CRC-32 */        
    HI_DP_PRBS_WITHOUT_CRC32_E,                /* PRBS 2_31 - 1 without CRC-32 */        
    HI_DP_PRBS_WITH_CRC32_E,                   /* PRBS 2_31 - 1 with CRC-32 */        
}HI_DP_PRBS_PATTERN_TYPE_E;
#endif

typedef struct
{
    HI_BOOL_E    em_channel_enable[HI_DP_PRBS_CHANNEL_NUM];    /* 发包通道使能 */

    /* 发送相关 */
    uint    ui_length;  /* prbs净荷长度 */
    uint    ui_vlan[HI_DP_PRBS_CHANNEL_NUM];
    HI_DP_ENDIAN_E  em_cfi[HI_DP_PRBS_CHANNEL_NUM];//
    uint    ui_pri[HI_DP_PRBS_CHANNEL_NUM];//
    uchar   uc_dmac[HI_DP_MAC_BYTE_NUM];
    uchar   uc_smac[HI_DP_MAC_BYTE_NUM];  
    ushort  us_tpid;

    uint    ui_tx_period;    /* 发包周期 */     /* 0 ~ 0xffffffff */
    uint    ui_tx_num[HI_DP_PRBS_CHANNEL_NUM];  /* 发包个数 */  /* 1 ~ 65536 */     

    HI_BOOL_E    em_tx_enable;    /* 发包使能 */

    /* 接收校验相关 */
    HI_BOOL_E    em_rx_enable;    /* 收包使能； */
    HI_DP_PRBS_RX_MODE_E    em_rx_mode;    /* 校验模式 */
    uint         ui_rx_err_range;    /* 校验报错门限  */
    uint     ui_eth_type;
#ifdef CONFIG_HISI_SD5115_TYPE_T
    hi_dp_prbs_seq_num_cfg_s st_seq_num_cfg;    /* sequence num配置 */
    hi_dp_prbs_crc_en_s st_crc_en;               /* 校验使能 */
    HI_DP_PRBS_PATTERN_TYPE_E em_pattern_type;  /* 码型选择 */
#endif
}hi_dp_prbs_attr_s;

typedef struct
{
    HI_BOOL_E  em_non_sync;    /* 失步状态 */
    HI_BOOL_E  em_check_err[HI_DP_PRBS_CHANNEL_NUM];   //校验结果；0:正确(无错)；1:错误(有错)
}hi_dp_prbs_sta_s;

typedef struct 
{
    uint    prbs_nni_send0;
    uint    prbs_nni_send1;
    uint    prbs_nni_send2;
    uint    prbs_nni_send3;
    uint    prbs_nni_send4;
    uint    prbs_nni_send5;
    uint    prbs_nni_send6;
    uint    prbs_nni_send7;
    uint    prbs_nni_rec0;
    uint    prbs_nni_rec1;
    uint    prbs_nni_rec2;
    uint    prbs_nni_rec3;
    uint    prbs_nni_rec4;
    uint    prbs_nni_rec5;
    uint    prbs_nni_rec6;
    uint    prbs_nni_rec7;
    uint    prbs_nni_ebit0;
    uint    prbs_nni_ebit1;
    uint    prbs_nni_ebit2;
    uint    prbs_nni_ebit3;
    uint    prbs_nni_ebit4;
    uint    prbs_nni_ebit5;
    uint    prbs_nni_ebit6;
    uint    prbs_nni_ebit7;
    uint    prbs_statbit_cfg;
    uint    prbs_sync_cfg;
    uint    prbs_tx_sop_cnt;
    uint    prbs_tx_eop_cnt;
    uint    prbs_rx_sop_cnt;
    uint    prbs_rx_eop_cnt;
}hi_dp_logic_prbs_cnt_s;
typedef struct 
{
    uint    ui_tx_num[HI_DP_PRBS_CHANNEL_NUM];   /*注意是已发送的，不是配置的*/
    uint    ui_rx_num[HI_DP_PRBS_CHANNEL_NUM];  
    uint    ui_rx_err_bits[HI_DP_PRBS_CHANNEL_NUM]; 
#ifdef CONFIG_HISI_SD5115_TYPE_T
    uint    ui_err_cnt[HI_DP_PRBS_CHANNEL_NUM]; 
    uint    ui_tst_seq_err_cnt;
    uint    ui_tst_crc_err_cnt;
#endif
}hi_dp_prbs_cnt_s;
#endif /* __HI_DRV_DP_PRBS_H__  */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus  */



