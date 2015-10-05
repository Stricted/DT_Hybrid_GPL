/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_vif_func.h
  版 本 号   : 初稿
  作    者   : yangfan 58968
  生成日期   : D2012_10_19

******************************************************************************/

#ifndef __I_KERNEL_VIFCMD_H__
#define __I_KERNEL_VIFCMD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef hi_int32 (*hi_kernel_xmit_callback)(hi_kernel_sk_buff_s *pst_skb,hi_kernel_net_device_s *pst_dev);
extern hi_int32 hi_kernel_vif_recv( hi_kernel_eth0_cb_s *pst_cb, hi_kernel_sk_buff_s *pst_skb );
extern hi_int32 hi_kernel_vif_xmit( hi_kernel_sk_buff_s *pst_skb, hi_kernel_net_device_s *pst_dev );
extern hi_uint32 hi_kernel_vif_set_extdev(hi_kernel_net_device_s  *pst_dev, 
                                          hi_kernel_net_device_s  *pst_vifdev_ext, hi_kernel_xmit_callback  pf_callback );
extern hi_kernel_xmit_callback hi_kernel_vif_get_extdev( hi_kernel_net_device_s  *pst_vifdev );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __I_KERNEL_VIFCMD_H__ */

