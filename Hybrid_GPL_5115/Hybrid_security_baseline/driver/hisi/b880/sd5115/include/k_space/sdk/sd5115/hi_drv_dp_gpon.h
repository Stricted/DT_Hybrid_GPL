/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_gpon.h
  版 本 号   : 初稿
  作    者   : shengjing
  生成日期   : 2011年5月25日
  最近修改   :
  功能描述   : gpon模块头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年5月25日
    作    者   : shengjing
    修改内容   : 创建文件

  2.日    期   : 2011年9月16日
    作    者   : f00125610
    修改内容   : 修改文件

******************************************************************************/
#ifndef __HI_DRV_DP_GPON_H__
#define __HI_DRV_DP_GPON_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef enum
{
    HI_DP_GPON_LOS_OCCUR_ALARM_E = 0,
    HI_DP_GPON_LOS_DIS_ALARM_E,
    HI_DP_GPON_LOF_OCCUR_ALARM_E,
    HI_DP_GPON_LOF_DIS_ALARM_E,
    HI_DP_GPON_DYINGGASP_ALARM_E,
    HI_DP_GPON_REI_COUNTER_ALARM_E,
    HI_DP_GPON_SF_ALARM_E,
    HI_DP_GPON_SD_ALARM_E,
    HI_DP_GPON_LCDG_ALARM_E,
#if defined (CONFIG_HISI_SD5115_MP_PILOT)
    HI_DP_GPON_BW_IDLE_ALARM_E,
#endif
    HI_DP_GPON_ROGUE_ONU_ALARM_E,
    HI_DP_GPON_SILENT_ONU_ALARM_E,
    HI_DP_GPON_LWI_ALARM_E,
    HI_DP_GPON_FWI_ALARM_E,
    HI_DP_GPON_EMPTY_ALARM_E,
    HI_DP_GPON_ALARM_NUM_E,
} HI_DP_GPON_ALARM_E;

#define HI_DP_GPON_LWI_EGR_BIT  HI_BIT_1
#define HI_DP_GPON_LWI_IGR_BIT  HI_BIT_0

typedef struct
{
    HI_BOOL_E   em_enable;            /* GPON工作使能 */
    hi_dp_tag_s st_default_tag;       /* 缺省tag */
    HI_BOOL_E   em_eth_sync_en;
    HI_BOOL_E   em_igr_detect_en;     /* 入端口流量监控使能 */
    HI_BOOL_E   em_egr_detect_en;     /* 出端口流量监控使能 */
} hi_dp_gpon_attr_s;

/*gpon模块告警属性 */
typedef struct
{
    /* 告警属性*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_GPON_ALARM_NUM_E];

    /* 模块alam 告警回调函数*/
    void (*alarm_call_back)(HI_DP_GPON_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_gpon_alarm_attr_s;

/* 5115修改: 统计项修改 */
typedef struct
{
    uint      ui_index;  /* 0~127,芯片仅支持128个GEMPORT的统计 */
    uint      ui_gemport;/* 指定查找的gemport_id      */
    HI_BOOL_E em_valid;

    ulong ul_up_frame;   /* 指定gemport，上行以太帧数 */
    ulong ul_up_gem;     /* 指定gemport，上行GEM帧数  */
    ulong ul_up_byte;    /* 指定gemport，上行字节数   */

    ulong ul_dn_frame;   /* 指定gemport，下行以太帧数 */
    ulong ul_dn_gem;     /* 指定gemport，下行GEM帧数  */
    ulong ul_dn_byte;    /* 指定gemport，下行字节数   */
} hi_dp_gpon_gemport_cnt_table_item_s;

typedef struct
{
    /* 告警 */
    uint ui_alarm_times[HI_DP_GPON_ALARM_NUM_E];    /* 各告警产生次数 */
    uint ui_alarm_call_back_times;                  /* 告警回调次数 */
    uint ui_alarm_discard_times;

    ulong ul_tx_ploam;         /* PLOAMU消息发送次数统计计数器。*/
    ulong ul_tx_ploam_nvld;    /* NO_MESSAGE消息的PLOAMu发送次数计数器。*/
    ulong ul_tx_dbru_m0;           /* DBRU模式0的发送总次数统计计数器。*/
    ulong ul_tx_dbru_m1;           /* DBRU模式1的发送总次数统计计数器。*/
    ulong ul_tx_dbru_spec;     /* 指定TCONT DBRU的发送总次数统计计数器。*/
    ulong ul_tx_omci;          /* OMCI发送总包数计数器。*/
    ulong ul_tx_gem;              /* 上行GEM发送统计个数计数器。*/
    ulong ul_tx_gem_cut;           /* 上行GEM切片统计计数器。*/
    ulong ul_tx_idle;          /* 上行发送IDLE帧总字节数计数器。*/
    ulong ul_tx_burst;         /* 上行burst发送个数统计计数器。*/
    ulong ul_tx_eth_byte;       /* PON口上行以太总字节数个数统计计数器。*/
    ulong ul_tx_uc;            /* PON口上行以太单播总个数统计计数器。*/
    ulong ul_tx_mc;            /* PON口上行以太组播总个数统计计数器。*/
    ulong ul_tx_bc;           /* PON口上行以太广播总个数统计计数器。*/
    ulong ul_tx_eth;           /* PON口上行以太总个数统计计数器。*/
    ulong ul_bip_err;          /* BIP错误统计计数器。*/
    ulong ul_range_req;       /* RANGE请求计数器。*/
    ulong ul_sn_req;              /* SN请求计数器。*/
    ulong ul_ploam_right;      /* 正确的ploam消息统计。*/
    ulong ul_ploam_wrong;      /* CRC错误的ploam消息统计。*/
    ulong ul_ploam;           /* 所有从ploam缓存内读出给CPU的ploam消息统计，包括CRC错误的ploam消息。*/
    ulong ul_ploam_full_drop;   /* 满丢弃或低功耗状态接收到的本地ploam消息数目统计。*/
    ulong ul_rei;                 /* 统计间隔内的BIP错误统计。*/
    ulong ul_bwmap_crc_err;    /* 本地BWMAP CRC校验错误统计计数器。*/
    ulong ul_fec_us_switch;    /* 上行FEC切换统计计数器。*/
    ulong ul_fec_decode_cw;    /* FEC码字解码计数器。*/
    ulong ul_fec_err_cw;          /* FEC码字误码计数器。*/
    ulong ul_fec_cor_cw;          /* FEC码字纠错计数器。*/
    ulong ul_fec_uncor_cw;     /* FEC码字未纠错计数器。*/
    ulong ul_fec_cor_byte;    /* FEC纠错BYTE数计数器。*/
    ulong ul_fec_seconds;     /* FEC Seconds是错误统计，每1秒钟查询一次，如果存在Uncorrectable code words，则统计值增加1。*/
    ulong ul_fec_ds_switch;   /* 下行FEC切换统计计数器。*/
    ulong ul_rx_pon_byte;     /* 本地PON总字节数据统计计数器。*/
    ulong ul_rx_gem;           /* 接收本地GEM次数统计计数器。*/
    ulong ul_gem_eth;        /* 下行非组播ETH GEM个数计数器，注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_gem_mc;             /* 下行组播类型GEM个数计数器，注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_gem_omci;       /* OMCI GEM统计计数器。*/
    ulong ul_gem_prbs;       /* PRBS GEM统计寄存器。*/
    ulong ul_aes_omci;       /* 下行需要解密的OMCI GEM计数器。*/
    ulong ul_aes_prbs;       /* 下行需要解密的PRBS GEM计数器。*/
    ulong ul_aes_mc;            /* 下行需要解密的组播GEM计数器，注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_aes_eth;       /* 下行需要解密的非组播ETH GEM计数器，注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_aes_blk;       /* 加密数据块统计计数器。*/
    ulong ul_key_switch;     /* 下行密钥切换成功次数统计计数器。*/
    ulong ul_rec_eth;        /* 下行需重组的非组播ETH GEM帧计数器（PTI为0），注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_rec_mc;            /* 下行需重组的组播类型GEM帧计数器（PTI为0），注：组播和非组播判断基准为PORTID配置类型。*/
    ulong ul_rec_omci;      /*OMCI重组（PTI=0）GEM统计计数器。*/
    ulong ul_rec_prbs;      /*PRBS 重组（PTI=0）GEM统计寄存器。*/
    ulong ul_short_err;     /*以太超短过滤统计计数器。*/
    ulong ul_long_err;      /*以太超长过滤统计计数器。*/
    ulong ul_rx_mc;        /*下行组播报文计数器（组播通过DA第一个字节的最低bit识别）。*/
    ulong ul_rx_uc;        /*下行单播报文计数器（单播通过DA第一个字节的最低bit识别）。*/
    ulong ul_rx_bc;        /*下行广播报文计数器（单播通过DA第一个字节识别）。*/
    ulong ul_rx_eth;           /*下行以太报文统计计数器。*/
    ulong ul_fcs_err;       /*FCS校验出错统计计数器。*/
    ulong ul_gem_lof;       /*GEM层状态机失步统计计数器。*/
    ulong ul_gem_drop;      /*本地GEM丢弃统计计数器，下行处理能力不足，原因：GEM帧PLI连续多个过小，如1字节。。*/
    ulong ul_hec_cor;       /*HEC 1bit纠错统计计数器。*/
    ulong ul_rx_omci;       /*OMCI接收包数统计计数器。*/
    ulong ul_omci_crc_err;   /*OMCI接收数数据CRC校验错误统计计数器。*/
    ulong ul_omci_full_drop; /*OMCI接收数据满丢弃统计计数器。*/
#if defined(CONFIG_HISI_SD5115_TYPE_T)
    ulong ul_tx_less_64;      /* 5115T add: 上行ETH报文长度小于64B的报文个数统计计数器。*/
    ulong ul_tx_64;           /* 5115T add: 上行ETH报文长度等于64B的报文个数统计计数器。*/
    ulong ul_tx_65to127;      /* 5115T add: 上行ETH报文长度[65B,127B]的报文个数统计计数器。*/
    ulong ul_tx_128to255;     /* 5115T add: 上行ETH报文长度[128B,255B]的报文个数统计计数器。*/
    ulong ul_tx_256to511;     /* 5115T add: 上行ETH报文长度[256B,511B]的报文个数统计计数器。*/
    ulong ul_tx_512to1023;    /* 5115T add: 上行ETH报文长度[512B,1023B]的报文个数统计计数器。*/
    ulong ul_tx_1024to1518;   /* 5115T add: 上行ETH报文长度[1024B,1518B]的报文个数统计计数器。*/
    ulong ul_tx_1519tomtu;    /* 5115T add: 上行ETH报文长度[1519B,MTU]的报文个数统计计数器。*/
    ulong ul_tx_more_mtu;     /* 5115T add: 上行ETH报文长度大于MTU的报文个数统计计数器。*/
    ulong ul_bwmap_right;     /* 5115T add: 本地BWMAP合法的统计计数器。*/
    ulong ul_bwmap_wrong;     /* 5115T add: 本地BWMAP非法的统计计数器。*/
    ulong ul_ploam_bc;        /* 5115T add: 广播ploam消息计数器。*/
    ulong ul_ploam_uc;        /* 5115T add: 单播ploam消息计数器。*/
    ulong ul_rx_less_64;      /* 5115T add: 下行ETH报文长度小于64B的报文个数统计计数器。*/
    ulong ul_rx_64;           /* 5115T add: 下行ETH报文长度等于64B的报文个数统计计数器。*/
    ulong ul_rx_65to127;      /* 5115T add: 下行ETH报文长度[65B,127B]的报文个数统计计数器。*/
    ulong ul_rx_128to255;     /* 5115T add: 下行ETH报文长度[128B,255B]的报文个数统计计数器。*/
    ulong ul_rx_256to511;     /* 5115T add: 下行ETH报文长度[256B,511B]的报文个数统计计数器。*/
    ulong ul_rx_512to1023;    /* 5115T add: 下行ETH报文长度[512B,1023B]的报文个数统计计数器。*/
    ulong ul_rx_1024to1518;   /* 5115T add: 下行ETH报文长度[1024B,1518B]的报文个数统计计数器。*/
    ulong ul_rx_1519tomtu;    /* 5115T add: 下行ETH报文长度[1519B,MTU]的报文个数统计计数器。*/
    ulong ul_rx_more_mtu;     /* 5115T add: 下行ETH报文长度大于MTU的报文个数统计计数器。*/
#endif
} hi_dp_gpon_cnt_s;

typedef struct
{
    uint ui_spcnt_curr;   /* 当前帧的超帧计数器值 */
} hi_dp_gpon_spcnt_s;

typedef struct
{
    HI_DP_ALARM_STATE_E em_los;             /* 当前LOS状态 */
    HI_DP_ALARM_STATE_E em_lof;             /* 当前LOF状态 */
    HI_BOOL_E em_igr_empty;                 /* 入端口流量状态 */
    HI_BOOL_E em_egr_empty;                 /* 出端口流量状态 */    
} hi_dp_gpon_sta_s;


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_DP_GPON_H__ */
