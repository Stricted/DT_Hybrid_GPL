/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_nni.h
  版 本 号   : 初稿
  作    者   : g00174254
  生成日期   : 2011年9月21日
  最近修改   :
  功能描述   : NNI PORT 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年10月28日
    作    者   : g174254
    修改内容   : 创建文件

***************************************************************************** */

#ifndef __HI_DRV_DP_NNI_PORT_H__
#define __HI_DRV_DP_NNI_PORT_H__

#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


/* ******************************** 宏定义 ********************************** */

/* ******************************* 枚举类型 ********************************* */

/* ************************* 子结构, 在主结构中被引用 *********************** */

/* ************** 主结构: 直接对外体现的结构，作为函数的参数类型 ************ */


/* NNI端口属性 */
typedef struct
{
    uint                    ui_index;           /* 保留，后续芯片可能增加GE数目 */
    HI_BOOL_E               em_enable;

    HI_DP_ETH_MII_MODE_E    em_mii_mode;        /* MII/GMII/RMII/SEDERS等 */
    HI_DP_MASTER_RGMII_E    em_master_rgmii;    /* 主用RGMII, 1，2， 当em_mii_mode为双RGMII模式时有效 */
    HI_DP_ETH_WORK_MODE_E   em_work_mode;       /* 工作模式 */
    HI_DP_ETH_LOOP_MODE_E   em_loop_mode;       /* 环回模式 */
    
    hi_dp_eth_fctl_attr_s   st_fctl_attr;       /* 流控配置    */
    hi_dp_tag_attr_s        st_tag_attr;        /* TAG相关配置 */
    
}hi_dp_nni_eth_port_attr_s;


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
    
    ulong    ul_ch_rx_pkt;
    ulong    ul_ch_rx_byte;
    ulong    ul_ch_tx_pkt;
    ulong    ul_ch_tx_byte;

    /* ... */
    
}hi_dp_nni_eth_port_cnt_s;


typedef struct
{
    uint                        ui_fe_index;
    HI_DP_ETH_LINK_STA_E        em_link_sta;
    HI_BOOL_E                   em_eee_sta;
    
}hi_dp_nni_eth_port_sta_s;




/* ************************************************************************** */

#endif /* */

