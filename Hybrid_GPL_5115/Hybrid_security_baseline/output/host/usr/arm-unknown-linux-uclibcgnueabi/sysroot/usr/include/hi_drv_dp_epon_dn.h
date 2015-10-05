/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_epon_dn.h
  版 本 号   : 初稿
  作    者   : 区伟权
  生成日期   : 2009年7月13日
  最近修改   :
  功能描述   : EPON下行用户头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年7月13日
    作    者   : 区伟权
    修改内容   : 创建文件

  2.日    期   : 2011年9月15日
    作    者   : f00125610
    修改内容   : 5115修改
***************************************************************************** */

#ifndef __HI_DRV_DP_EPON_DN_H__
#define __HI_DRV_DP_EPON_DN_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* 配置属性 */
typedef struct
{
    HI_BOOL_E         em_eapol_en;                  /* PON口的EAPOL捕获使能 */
    HI_BOOL_E         em_fec_en;                    /* 下行FEC纠错使能 */
    HI_LEVEL_E        em_los_level;                 /* LOS 有效电平 */
    HI_DP_TRANS_ACT_E em_umc_act;                   /* 组播过滤或者透传 */
    uint              ui_gate_timeout;              /* Gate超时门限 */
    uint              ui_rtt_adj;                   /* RTT 本地时间补偿 */

    void (*mpcp_call_back)(hi_mpcp_message_s * st_mpcp);
    void (*oam_call_back)(hi_oam_message_s * st_oam);
} hi_dp_epon_dn_attr_s;

#define HI_DP_EPON_KEY_LEN 4   /* 5115新增: 密钥长度，共128bit */

/* 支持两种加密方式，修改密钥长度。*/
typedef struct
{
    uint ui_key0[HI_DP_EPON_KEY_LEN];  /* 5115修改: 密钥ID 0 */
    uint ui_key1[HI_DP_EPON_KEY_LEN];  /* 5115修改: 密钥ID 1 */
} hi_dp_epon_dn_key_s;


#define HI_DP_EPON_INVALID_LLID 0x7fff

/* 下行llid 过滤表表项，注意上行也会查询该表 */
typedef struct
{
    uint                ui_index;      /* 索引  0-7, T为0-15 */
    HI_BOOL_E           em_registered; /* 注册状态 */
    HI_BOOL_E           em_mod_en;     /* 5115新增: MOD域检查使能 */
    uint                ui_llid;       /* 注册的LLID */
    hi_mac_s            st_mac;        /* 使用的MAC地址 */
    hi_dp_epon_dn_key_s st_key;        /* 5115修改: 支持两种加密方式 */
} hi_dp_epon_dn_llid_table_item_s;

typedef struct
{
    HI_DP_ALARM_STATE_E em_ftm_los;
    HI_BOOL_E           em_synchronized;                   /* 帧同步状态 */
    uint                ui_key_index[HI_DP_EPON_LLID_NUM]; /* 各LLID距离现在最近帧的加密密钥索引 */
    HI_BOOL_E           em_cipher_st[HI_DP_EPON_LLID_NUM]; /* 各LLID距离现在最近帧的加密状态 */
    HI_BOOL_E em_fec_up_st[HI_DP_EPON_LLID_NUM];           /* 5115T add: 各LLID上行FEC加密状态 */
    HI_BOOL_E em_fec_dn_st[HI_DP_EPON_LLID_NUM];           /* 5115T add: 各LLID下行FEC加密状态 */
} hi_dp_epon_dn_sta_s;

/* 统计信息 */
typedef struct
{
    ulong ul_mpcp_call_back_cnt;
    ulong ul_mpcp_call_back_dsicard_cnt;
    ulong ul_mpcp_err_discard_cnt;
    ulong ul_rx_mpcp_cnt;

    ulong ul_oam_call_back_cnt;
    ulong ul_oam_call_back_dsicard_cnt;
    ulong ul_oam_err_discard_cnt;
    ulong ul_rx_oam_cnt;
} hi_dp_epon_dn_cnt_s;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* end of __HI_DRV_DP_EPON_DN_H__ */
