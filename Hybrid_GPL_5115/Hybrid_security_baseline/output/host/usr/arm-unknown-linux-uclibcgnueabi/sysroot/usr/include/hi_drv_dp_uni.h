/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_uni.h
  版 本 号   : 初稿
  作    者   : g00174254
  生成日期   : 2011年9月21日
  最近修改   :
  功能描述   : UNI PORT 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年9月21日
    作    者   : g174254
    修改内容   : 创建文件

***************************************************************************** */

#ifndef __HI_DRV_DP_UNI_PORT_H__
#define __HI_DRV_DP_UNI_PORT_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


/* ******************************* 宏定义 *********************************** */

#define HI_DP_ETH_CASCADE_PORT_NUM      4

/* ******************************* 枚举类型 ********************************* */
/* ETH端口LED灯闪烁控制模式 */
typedef enum
{
    HI_DP_ETH_LED_ON_E = 0,    /* LED灯常亮 */
    HI_DP_ETH_LED_OFF_E,       /* LED灯常灭 */
    HI_DP_ETH_LED_BLINK_E,     /* LED灯以一定频率闪烁 */

}HI_DP_ETH_LED_WORK_MODE_E;

typedef enum
{
    HI_DP_ETH_LED_YELLOW_E = 0,    /* 黄LED灯 */
    HI_DP_ETH_LED_GREEN_E,         /* 绿LED灯 */
    HI_DP_ETH_LED_NUM,             /* LED灯数目 */

}HI_DP_ETH_LED_TYPE_E;

typedef enum
{
    HI_DP_ETH_LOCAL_E = 0,       /* 本地端口 */
    HI_DP_ETH_REMOTE_E,          /* 对端端口 */
    HI_DP_ETH_PORT_TYPE_NUM,    /* 端口类型数目 */

}HI_DP_ETH_PORT_TYPE_E;

typedef enum
{
    HI_DP_ETH_RX_E = 0,       /* 接收方向 */
    HI_DP_ETH_TX_E,           /* 发送方向 */
    HI_DP_ETH_DIRECTION_TYPE_NUM,    /* 方向类型数目 */

}HI_DP_ETH_DIRECTION_TYPE_E;

/* ************************* 子结构, 在主结构中被引用 *********************** */


/* 以太网端口能效EEE相关属性 */
typedef struct
{
    HI_BOOL_E   em_enable;

    uint    ui_wait_time;                                                      /* 等待时间, 单位8ns, 默认100ms (0x00BEBC20) */
    uint    ui_sleep_time;                                                     /* 睡眠时间, 单位8ns, 默认20ms  (0x002625A0) */
    uint    ui_wakeup_time;                                                    /* 唤醒时间, 单位8ns, 1000M模式下, 默认16.5us(0x80F)
                                                                                    100M模式下, 默认 30 us(0xEA6)*/
    hi_dp_eth_eee_capability_u    u_phy_eee_cap[HI_DP_ETH_PORT_TYPE_NUM];      /* 本地和对端端口的EEE能力，只读 */
    hi_dp_eth_eee_capability_u    u_local_eee_cap_advertise;                  /* 本地端口EEE能力声明 */
    HI_BOOL_E                   em_eee_sta[HI_DP_ETH_DIRECTION_TYPE_NUM];
}hi_dp_eth_eee_attr_s;

/* 以太端口802.1X 相关配置 */
typedef struct
{
    HI_BOOL_E   em_8021x_auth_en;       /* 授权状态，端口打开；：未授权状态，端口关闭。 */
}hi_dp_eth_8021x_attr_s;

/* 以太端口stp 相关配置 */
typedef struct
{
    HI_DP_ETH_STP_STATUS_E  em_stp_status;      /* 端口生成树状态 */
}hi_dp_eth_stp_attr_s;

/* ETH端口LED灯闪烁控制 */
typedef struct
{
    HI_DP_ETH_LED_WORK_MODE_E   em_work_mode;    
    uint                        ui_freq;   /* 只在HI_DP_ETH_LED_BLINK_E模式下有效，单位为100ms(暂定) */
}hi_dp_eth_led_attr_s;

#if 0
/* PRBS端口以太属性 */
typedef struct
{
    
}hi_dp_prbs_eth_attr_s;


/* PRBS产生(发送)属性 */
typedef struct
{
    HI_BOOL_E           em_send_en;
    HI_PRBS_SEND_MODE   em_send_mode;
    uint                ui_send_len;
    uint                ui_send_ipg;
    
    uint                ui_seed;        // 生成种子
    
}hi_dp_prbs_send_attr_s;

/* PRBS检测(接受)属性 */
typedef struct
{
    HI_BOOL_E   em_rcv_en;
    uint        ui_sync_period;
    
}hi_dp_prbs_rcv_attr_s;
#endif


/* ************ 主结构: 直接对外体现的结构，作为函数的参数类型 ************** */


/* CPU端口属性 */
typedef struct
{
    hi_dp_tag_attr_s    st_cpu_tag_attr;    /* CPU端口TAG属性 */

    /* TPID的修改通过VLAN sft实现 */
    //uint               ui_tpid_modify;          /* TPID修改配置 */

}hi_dp_uni_cpu_port_attr_s;

/* CPU端口统计 */
typedef struct
{
    
}hi_dp_uni_cpu_port_cnt_s;

/* GE端口属性 */
typedef struct
{
    uint                              ui_index;
    HI_BOOL_E                         em_enable;

    HI_DP_ETH_MII_MODE_E              em_mii_mode;                                 /* MII/GMII/RMII/SEDERS等 */
//#if (defined (CONFIG_HISI_SD5115_AF_ASIC) && defined (CONFIG_HISI_SD5115_TYPE_H))
    HI_DP_RGMII_LINE_SEQ_E            em_line_seq;                                 /* RGMII1的线序 */
//#endif
    HI_DP_ETH_WORK_MODE_E             em_work_mode;                                /* 工作模式 */
    hi_dp_eth_work_capability_u       u_work_mode_cap;                             /* 工作模式能力 */
    hi_dp_eth_auto_neg_capability_u   u_auto_neg_cap;                              /* 自协商的能力 */
    hi_dp_eth_phy_capability_u        u_phy_cap;
    HI_DP_ETH_LOOP_MODE_E             em_loop_mode;                                /* 环回模式 */
    hi_dp_eth_led_attr_s              st_led_attr[HI_DP_ETH_LED_NUM];

    hi_dp_tag_attr_s                  st_tag_attr;                                 /* TAG相关配置 */
    
    hi_dp_eth_eee_attr_s              st_eee_attr;                                 /* EEE能效配置 */
    hi_dp_eth_fctl_attr_s             st_fctl_attr;                                 /* 流控配置 */
    hi_dp_eth_8021x_attr_s            st_8021x_attr;                                /* 802.1X  相关配置 */
    hi_dp_eth_stp_attr_s              st_stp_attr;                                  /* 端口生成树相关配置 */
    
}hi_dp_uni_eth_port_attr_s;


/* GE端口统计 */
typedef struct
{
    uint     ui_index;

    /* ALL_MAC_reg */
    ulong    ul_rx;
    ulong    ul_rx_uc;
    ulong    ul_rx_mc;
    ulong    ul_rx_bc;
    ulong    ul_rx_pause;
    ulong    ul_rx_unk_ctrl;
    ulong    ul_rx_under_min;
    ulong    ul_rx_min_63;
    ulong    ul_rx_64;
    ulong    ul_rx_65_127;
    ulong    ul_rx_128_255;
    ulong    ul_rx_256_511;
    ulong    ul_rx_512_1023;
    ulong    ul_rx_1024_1518;
    ulong    ul_rx_1519_max;
    ulong    ul_rx_over_max;
    ulong    ul_rx_undersize;
    ulong    ul_rx_fragement;
    ulong    ul_rx_over_size;
    ulong    ul_rx_jabber;
    ulong    ul_rx_fcs_err;
    ulong    ul_rx_align_err;
    ulong    ul_rx_fifo_overrun;
    ulong    ul_ch_tag_err;
    ulong    ul_rx_byte_ok;
    ulong    ul_rx_byte_err;
    
    ulong    ul_tx;
    ulong    ul_tx_uc;
    ulong    ul_tx_mc;
    ulong    ul_tx_bc;
    ulong    ul_tx_pause;
    ulong    ul_tx_min_63;
    ulong    ul_tx_64;
    ulong    ul_tx_65_127;
    ulong    ul_tx_128_255;
    ulong    ul_tx_256_511;
    ulong    ul_tx_512_1023;
    ulong    ul_tx_1024_1518;
    ulong    ul_tx_1519_max;
    ulong    ul_tx_over_max;
    ulong    ul_tx_crc_err;
    ulong    ul_tx_single_col;
    ulong    ul_tx_multi_col;
    ulong    ul_tx_excess_col;
    ulong    ul_tx_late_col;
    ulong    ul_tx_deferral_trans;
    ulong    ul_tx_crs_lost;
    ulong    ul_tx_abort;
    ulong    ul_tx_fifo_underrun;
    ulong    ul_tx_byte_ok;
    ulong    ul_tx_byte_err;
    
    /* ... */
    
}hi_dp_uni_eth_port_cnt_s;


typedef struct
{
    uint                        ui_index;
    HI_DP_ETH_LINK_STA_E        em_link_sta;
    HI_DP_ETH_WORK_MODE_E       em_work_mode;
    
}hi_dp_uni_eth_port_sta_s;

#if 0
/* PRBS */
typedef struct
{
    hi_dp_prbs_eth_attr_s       st_prbs_eth_attr;
    hi_dp_prbs_send_attr_s      st_prbs_tx_attr;
    hi_dp_prbs_send_attr_s      st_prbs_rx_attr;
    
}hi_dp_uni_prbs_port_attr_s;

typedef struct
{
    
}hi_dp_uni_prbs_port_cnt_s;

typedef struct
{
    
}hi_dp_uni_prbs_port_sta_s;
#endif

typedef struct
{
    /* 芯片固定将GE0作为级联端口，只有1个级联端口 */
    
    HI_BOOL_E     em_enable;        /* 级联模式开关 */
    uint          ui_tpid;          /* 级联模式下, 报文级联tag识别和添加值 */
    HI_BOOL_E     em_double_tag_en; /* 两层级联TAG使能 */

    /* 级联端口tci信息，用作：出口添加的pri&cfi&vid域, 入口vid到端口的映射 */
    hi_dp_tci_s   st_tag_map[HI_DP_ETH_CASCADE_PORT_NUM];

} hi_dp_uni_cascade_attr_s;

/* BEGIN: Added by w00180499, 2012/3/12 */
typedef struct
{
    unsigned int ui_phy_addr;
    unsigned int ui_reg_addr;
    unsigned short us_data;
} hi_inner_phy_reg_info_s;
/* END:   Added by w00180499, 2012/3/12 */

/* ************************************************************************** */

#endif /* */

