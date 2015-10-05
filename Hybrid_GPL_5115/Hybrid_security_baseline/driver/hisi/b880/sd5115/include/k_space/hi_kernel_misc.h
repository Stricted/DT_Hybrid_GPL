/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_misc.h
  版 本 号   : 初稿
  作    者   : luocheng 00183967
  生成日期   : D2012_03_28

******************************************************************************/
#ifndef __HI_KERNEL_MISC_H__
#define __HI_KERNEL_MISC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct
{
    hi_uint32 ui_addr;
    hi_uint32 ui_len;
    hi_uint32 ui_data;
}hi_kernel_misc_util_mem_s;

typedef struct{
    hi_uint32   ui_phy_addr;
    hi_uint32   ui_reg_addr;
    hi_ushort16 us_data;
    hi_ushort16 us_resv;
}hi_kernel_mdio_data_s;

typedef struct
{
    hi_uint32 ui_enable;
    hi_uint32 ui_baud_rate;
} hi_kernel_mdio_attr_s;

extern hi_int32 hi_kernel_misc_util_init(hi_void);
extern hi_int32 hi_kernel_misc_util_exit(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_MISC_H__ */
