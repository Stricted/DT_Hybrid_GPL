/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_accel.h
  版 本 号   : 初稿
  作    者   : y00206091
  生成日期   : D2012_10_18

******************************************************************************/
#ifndef __HI_KERNEL_ACCEL_H__
#define __HI_KERNEL_ACCEL_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


typedef enum
{
    HI_KERNEL_ACCEL_PROTO_IPV4OE_E,
    HI_KERNEL_ACCEL_PROTO_IPV6OE_E, 
    HI_KERNEL_ACCEL_PROTO_IPV4PPPOE_E,
    HI_KERNEL_ACCEL_PROTO_IPV6PPPOE_E, 
    HI_KERNEL_ACCEL_PROTO_NUM_E
}hi_kernel_accel_l3_proto_e; 

typedef enum  {
    HI_RET_ACCEL_BASEVALUE              = (HI_SUBMODULE_KNETWORKS_ACCEL ),
    HI_RET_ACCEL_HW_AGED                = (HI_SUBMODULE_KNETWORKS_ACCEL | 0x01  ),     // 硬件表项被老化
    HI_RET_ACCEL_HW_NOT_AGED            = (HI_SUBMODULE_KNETWORKS_ACCEL | 0x02  ),     // 硬件表项还未被老化
    HI_RET_ACCEL_NO_FREE_HW_ITEM        = (HI_SUBMODULE_KNETWORKS_ACCEL | 0x03  ),     // 没有空闲的硬件表项
}hi_kernel_network_accel_ret_e;

extern hi_uint32 hi_kernel_accel_hw_getflag(hi_void); 
extern hi_uint32 hi_kernel_accel_sw_getflag(hi_void); 
extern hi_int32  hi_kernel_accel_core_init(hi_void); 
extern hi_void   hi_kernel_accel_core_exit(hi_void); 
extern hi_uint32 hi_kernel_accel_hw_getthreshold(hi_void); 

#if 0
#define hi_kernel_accel_printmem(ui_dbglevel,puc_src,ui_len) hi_kernel_log_mem(HI_SUBMODULE_KNETWORKS_ACCEL,ui_dbglevel,puc_src,ui_len)
#define hi_kernel_accel_printmemdes(ui_dbglevel,puc_src,ui_len,fmt,arg...) hi_kernel_log_memdes(HI_SUBMODULE_KNETWORKS_ACCEL,ui_dbglevel,puc_src,ui_len,(hi_uchar8 *)fmt, ##arg)
#define hi_kernel_accel_systrace(ui_ret, arg1, arg2, arg3, arg4)  \
                        hi_kernel_systrace(HI_SUBMODULE_KNETWORKS_ACCEL,ui_ret, arg1, arg2, arg3, arg4)
                        
#define hi_kernel_accel_debug     hi_kernel_debug
#define hi_kernel_accel_print     hi_kernel_print
#else
#define hi_kernel_accel_printmem(ui_dbglevel,puc_src,ui_len)
#define hi_kernel_accel_printmemdes(ui_dbglevel,puc_src,ui_len,fmt,arg...)
#define hi_kernel_accel_systrace(ui_ret, arg1, arg2, arg3, arg4)
#define hi_kernel_accel_debug(ui_module,ui_level,fmt,arg...)
#define hi_kernel_accel_print(ui_module,ui_dbglevel,fmt,arg...)
#endif

static inline hi_kernel_net_device_s* hi_kernel_accel_dev_get_by_index(hi_uint32 ui_index)
{
    hi_kernel_net_device_s *pst_dev = HI_NULL; 

    pst_dev = hi_kernel_dev_get_by_index(&init_net, ui_index); 
    if ( HI_NULL != pst_dev )
    {        
        hi_kernel_dev_put(pst_dev); 
    }
    
    if ( ( HI_NULL == pst_dev ) || ( (pst_dev->flags & IFF_UP) != IFF_UP )
      || ( HI_NULL == pst_dev->netdev_ops ) || ( HI_NULL == pst_dev->netdev_ops->ndo_start_xmit))
    {
        hi_kernel_accel_systrace(HI_RET_FAIL, 0, 0, 0, 0);
        return HI_NULL;
    }  

    return pst_dev; 
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_ACCEL_H__ */


