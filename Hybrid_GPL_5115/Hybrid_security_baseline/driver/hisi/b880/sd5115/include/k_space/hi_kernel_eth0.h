/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_eth0.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_ETH0_H__
#define __HI_KERNEL_ETH0_H__


#ifdef __cplusplus
extern "C" {
#endif

#define HI_KERNEL_ETH0_NAME             "eth0"

typedef enum
{
    HI_KERNEL_ETH0_VLANFLAG_UNTAG = 0,    /* untag vlan */
    HI_KERNEL_ETH0_VLANFLAG_STAG,         /* single vlan */
    HI_KERNEL_ETH0_VLANFLAG_CTAG,         /* double vlan */  
}hi_kernel_eth0_vlanflag_e;

typedef struct 
{
    hi_uchar8   uc_groupid;   /*tx*/
    hi_uchar8   uc_act;
    hi_uchar8   uc_pri;
    hi_uchar8   uc_port;        /*rx:ingress tx:engress*/
    
    hi_uchar8   uc_reason;
    hi_uchar8   uc_llid;        /*llid or tcontid*/
    hi_uchar8   uc_resv;
    hi_uchar8   uc_vlanflag;    /* 1: 一层HI_KERNEL_ETH0_VLANFLAG_STAG 2:二层HI_KERNEL_ETH0_VLANFLAG_CTAG*/
    
    hi_ushort16 us_svlan;
    hi_ushort16 us_cvlan;

    hi_ushort16 us_gemport;
    hi_ushort16 us_len;
    hi_kernel_net_device_s *pst_srcdev;
}hi_kernel_eth0_cb_s; 

typedef hi_int32 (*hi_kernel_eth0_dev_rxcallback)( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb );
typedef hi_int32 (*hi_kernel_eth0_dev_txcallback)( struct sk_buff *pst_skb, struct net_device  *pst_dev );
typedef hi_uint32 (*hi_kernel_eth0_get_callback)( hi_void );

extern hi_int32 hi_kernel_eth0_tx_send( hi_kernel_sk_buff_s *pst_skb, hi_kernel_net_device_s *pst_dev );
extern hi_int32 hi_kernel_eth0_tx_specialsend( hi_kernel_eth0_cb_s *pst_cb, 
                                        hi_kernel_sk_buff_s *pst_skb );

extern hi_uint32 hi_kernel_eth0_get_cpuusage( hi_void );

extern hi_void hi_kernel_eth0_dev_rxhook_reg( hi_kernel_eth0_dev_rxcallback pv_rx_callback );
extern hi_void hi_kernel_eth0_dev_rxhook_unreg( hi_void );

extern hi_void hi_kernel_eth0_dev_txhook_reg( hi_kernel_eth0_dev_txcallback pv_tx_callback );
extern hi_void hi_kernel_eth0_dev_txhook_unreg( hi_void );

#ifdef __cplusplus
}
#endif


#endif  /* __HI_KERNEL_ETH0_H__ */
