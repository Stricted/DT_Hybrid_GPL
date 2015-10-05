/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_bridge.h
  版 本 号   : 初稿
  作    者   : y00206091
  生成日期   : D2012_07_11

******************************************************************************/
#ifndef __HI_KERNEL_BRIDGE_H__
#define __HI_KERNEL_BRIDGE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*********************************************************************************/
extern hi_int32 hi_kernel_accel_bridge_init(hi_void); 
extern hi_int32 hi_kernel_accel_bridge_exit(hi_void); 
extern hi_uint32 hi_kernel_accel_bridge_add_item( hi_kernel_sk_buff_s *pst_skb,
                                                  hi_kernel_net_device_s *pst_in, 
                                                  hi_kernel_net_device_s *pst_out ); 
extern hi_uint32 hi_kernel_accel_bridge_process(hi_kernel_sk_buff_s *pst_skb, hi_kernel_net_device_s *pst_dev); 
extern hi_uint32 hi_kernel_accel_bridge_delall( hi_uint32 ui_flag ); 
extern hi_uint32 hi_kernel_accel_bridge_cmdtype_dump_mac(hi_void);
extern hi_uint32 hi_kernel_accel_bridge_cmdtype_dump(hi_void); 
extern hi_void hi_kernel_accel_bridge_interface_down( hi_int32 i_ifindex );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_BRIDGE_H__ */

