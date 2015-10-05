/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_kernel_drv_dp_epon.h
  Version       : Initial Draft
  Author        : z53794
  Created       : 2011/5/27
  Last Modified :
  Description   : hi_kernel_drv_dp_epon.c header file
  Function List :
  History       :
  1.Date        : 2011/5/27
    Author      : z53794
    Modification: Created file

  2.Date        : 2011/9/15
    Author      : f00125610
    Modification: Modified file

******************************************************************************/
#ifndef __HI_DRV_DP_EPON_H__
#define __HI_DRV_DP_EPON_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

/*----------------------------------------------*
* external variables                           *
*----------------------------------------------*/

/*----------------------------------------------*
* external routine prototypes                  *
*----------------------------------------------*/

/*----------------------------------------------*
* internal routine prototypes                  *
*----------------------------------------------*/

/*----------------------------------------------*
* project-wide global variables                *
*----------------------------------------------*/

/*----------------------------------------------*
* module-wide global variables                 *
*----------------------------------------------*/

/*----------------------------------------------*
* constants                                    *
*----------------------------------------------*/

/*----------------------------------------------*
* macros                                       *
*----------------------------------------------*/

/*----------------------------------------------*
* routines' implementations                    *
*----------------------------------------------*/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* 密文加密类型 */
typedef enum
{
    HI_DP_EPON_TRIPLE_CHURNING_E = 0,  /* 三重搅动 */
    HI_DP_EPON_AES_E,                  /* AES */
} HI_DP_EPON_DECIPHER_TYPE_E;

/* EPON 配置属性 */
typedef struct
{
    HI_BOOL_E                  em_enable;       /* 使能开关，默认打开 */
    hi_dp_tag_s                st_default_tag;  /* 缺省TAG */
    HI_DP_EPON_DECIPHER_TYPE_E em_decipher_type;/* 加密类型 */
    HI_BOOL_E em_single_llid_en;                   /* 单llid模式 */
    HI_BOOL_E em_eth_sync_en;                   /* 同步以太使能 */
} hi_dp_epon_attr_s;

typedef enum
{
    HI_DP_EPON_ROGUE_ONU_ALARM_E = 0,       /* 流氓ONU */
    HI_DP_EPON_SILENT_ONU_ALARM_E,          /* 不发光ONU */
    HI_DP_EPON_LOS_ALARM_E,                 /* 光模块RX 进入LOS 状态告警 */
    HI_DP_EPON_SD_ALARM_E,                  /* 光模块RX 退出LOS 状态告警 */
    HI_DP_EPON_DYINGGASP_ALARM_E,
    HI_DP_EPON_GATE_TIMEOUT_ALARM_E,        /* LLID0-7,通过参数体现 */
    HI_DP_EPON_ALARM_NUM_E,
} HI_DP_EPON_ALARM_E;

typedef struct
{
    /* 告警属性 */
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_EPON_ALARM_NUM_E];

    /* 模块alam 告警回调函数 */
    void (*alarm_call_back)(HI_DP_EPON_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_epon_alarm_attr_s;

typedef struct
{
    /* 告警 */
    uint  ui_alarm_times[HI_DP_EPON_ALARM_NUM_E];   /* 各告警产生次数 */
    uint  ui_alarm_call_back_times;                 /* 告警回调次数 */
    uint  ui_alarm_discard_times;
    ulong ul_mpcp_rx_fifo_over;        /* MPCP接收FIFO溢出丢帧计数器。 */
    ulong ul_data_rx_fifo_over;        /* 数据帧接收FIFO溢出丢帧计数器。 */
    ulong ul_oam_rx_buffer_over;       /* OAM帧接收缓存溢出统计 */
    ulong ul_mac_rx_err_grant;         /* MAC接收错误Grant计数器。 */
    ulong ul_mac_rx_b2b_grant;         /* MAC接收背靠背Grant计数器。 */
    ulong ul_data_tx_fifo_und;         /* 数据帧发送FIFO欠载错帧计数器。 */

    ulong ul_mac_fcs_err_less_64;      /* MAC接收FCS校验错帧（帧长<64bytes）计数器 。*/
    ulong ul_mac_fcs_err_64_mtu;       /* MAC接收FCS校验错帧（64bytes≤帧长≤MTU）计数器 。*/
    ulong ul_mac_fcs_err_more_mtu;     /* MAC接收FCS校验错帧（帧长>MTU）计数器 。*/
    ulong ul_mac_fcs_err_byte;         /* MAC接收FCS校验错字节计数器 。*/
    ulong ul_mac_rx_all;               /* MAC接收所有FCS正确数据帧计数器 。*/
    ulong ul_mac_rx_byte;              /* MAC接收所有FCS正确数据字节计数器 。*/
    ulong ul_mac_rx_all_ctrl;          /* MAC接收FCS正确所有MAC控制帧计数器 。*/
    ulong ul_mac_rx_err_ctrl;          /* MAC接收错误MAC控制帧（不包括FCS错误）计数器 。*/
    ulong ul_mac_rx_saoff_oam;         /* MAC接收到SA(OFF)OAM帧计数器 。*/
    ulong ul_mac_tx_all;               /* MAC发送所有数据帧计数器 。*/
    ulong ul_mac_tx_byte;              /* MAC发送所有数据字节计数器 。*/
    ulong ul_mac_tx_mpcp;              /* MAC发送所有MPCP帧计数器 。*/

    ulong ul_scb_rx_byte;              /* 下行SCB接收数据字节计数器 。*/
    ulong ul_scb_rx_uc;                /* 下行SCB接收DA单播帧计数器 。*/
    ulong ul_scb_rx_mc;                /* 下行SCB接收DA多播帧计数器 。*/
    ulong ul_scb_rx_bc;                /* 下行SCB接收DA广播帧计数器 。*/
    ulong ul_mac_rx_dis_gate;          /* 下行接收Discovery Gate帧计数器 。*/
    ulong ul_scb_rx_less_64;           /* 下行SCB接收数据帧或OAM帧计数器（帧长<64bytes） 。*/
    ulong ul_scb_rx_64;                /* 下行SCB接收数据帧或OAM帧计数器（帧长=64bytes） 。*/
    ulong ul_scb_rx_65to127;           /* 下行SCB接收数据帧或OAM帧计数器（65bytes≤帧长≤127bytes） 。*/
    ulong ul_scb_rx_128to255;          /* 下行SCB接收数据帧或OAM帧计数器（128bytes≤帧长≤255bytes） 。*/
    ulong ul_scb_rx_256to511;          /* 下行SCB接收数据帧或OAM帧计数器（256bytes≤帧长≤511bytes） 。*/
    ulong ul_scb_rx_512to1023;         /* 下行SCB接收数据帧或OAM帧计数器（512bytes≤帧长≤1023bytes） 。*/
    ulong ul_scb_rx_1024to1518;        /* 下行SCB接收数据帧或OAM帧计数器（1024bytes≤帧长≤1518bytes） 。*/
    ulong ul_scb_rx_1519to2000;        /* 下行SCB接收数据帧或OAM帧计数器（1519bytes≤帧长≤2000bytes） 。*/
    ulong ul_scb_rx_more_2000;         /* 下行SCB接收数据帧或OAM帧计数器（帧长>2000bytes） 。*/
    ulong ul_scb_rx_more_mtu;          /* 下行SCB接收数据帧或OAM帧计数器（帧长>MTU） 。*/
    ulong ul_scb_rx_oam;               /* 下行SCB接收OAM帧计数器 。*/

    ulong ul_fec_tx_total_code_words;
    ulong ul_fec_rx_corrected_bytes;
    ulong ul_fec_rx_corrected_code_words;
    ulong ul_fec_rx_uncorrectable_code_words;
    ulong ul_fec_rx_total_code_words;
    ulong ul_fec_rx_seconds;

} hi_dp_epon_cnt_s;

typedef struct
{
    uint ui_llid;
    uint ui_sec;
} hi_dp_epon_llid_bandwidth_s;

typedef struct
{
    ulong ul_byte;          /* 发送字节计数 */
    ulong ul_uc;            /* 发送单播帧计数 */
    ulong ul_mc;            /* 发送组播帧计数 */
    ulong ul_bc;            /* 发送组播帧计数 */
    ulong ul_report;        /* 发送的report帧计数 */
    ulong ul_less_64;       /* 发送小于64字节帧计数 */
    ulong ul_64;            /* 发送等于64字节帧计数 */
    ulong ul_65to127;       /* 发送65到127字节帧计数 */
    ulong ul_128to255;      /* 发送128到255字节帧计数 */
    ulong ul_256to511;      /* 发送256到511字节帧计数 */
    ulong ul_512to1023;     /* 发送512到1023字节帧计数 */
    ulong ul_1024to1518;    /* 发送1024到1518字节帧计数 */
    ulong ul_1519to2000;    /* 发送1519到2000字节帧计数 */
    ulong ul_more_2000;     /* 发送大于2000字节帧计数 */
    ulong ul_more_mtu;      /* 发送大于MTU帧计数 */
    ulong ul_err_data;      /* 发送错误数据帧计数 */
    ulong ul_oam;           /* 发送OAM帧计数 */
    ulong ul_data_frag;     /* 发送数据分片计数 */
    ulong ul_oam_frag;      /* 发送OAM分片计数 */
} hi_dp_epon_llid_tx_cnt_s;

typedef struct
{
    ulong ul_grant;         /* 接收正确的Grant计数 */
    ulong ul_frpt_grant;    /* 接收正确的Force Report Grant计数 */
    ulong ul_grant_len_sum; /* 接收Grant长度累计总和计数 */
    ulong ul_byte;          /* 接收字节计数 */
    ulong ul_uc;            /* 接收单播帧计数 */
    ulong ul_mc;            /* 接收组播帧计数 */
    ulong ul_bc;            /* 接收组播帧计数 */
    ulong ul_gate;          /* 接收的Gate帧计数 */
    ulong ul_less_64;       /* 接收小于64字节帧计数 */
    ulong ul_64;            /* 接收等于64字节帧计数 */
    ulong ul_65to127;       /* 接收65到127字节帧计数 */
    ulong ul_128to255;      /* 接收128到255字节帧计数 */
    ulong ul_256to511;      /* 接收256到511字节帧计数 */
    ulong ul_512to1023;     /* 接收512到1023字节帧计数 */
    ulong ul_1024to1518;    /* 接收1024到1518字节帧计数 */
    ulong ul_1519to2000;    /* 接收1519到2000字节帧计数 */
    ulong ul_more_2000;     /* 接收大于2000字节帧计数 */
    ulong ul_more_mtu;      /* 接收大于MTU帧计数 */
    ulong ul_oam;           /* 接收OAM帧计数 */
    ulong ul_otdr_gate;     /* 接收OTDR GATE帧计数 */
    ulong ul_cipher;        /* 接收CIPHER帧计数 */
} hi_dp_epon_llid_rx_cnt_s;

typedef struct
{
    uint                     ui_llid;
    hi_dp_epon_llid_rx_cnt_s st_rx;
    hi_dp_epon_llid_tx_cnt_s st_tx;
} hi_dp_epon_llid_cnt_s;
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_EPON_H__ */
