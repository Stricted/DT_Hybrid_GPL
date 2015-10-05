/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_dev.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_21
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_DEV_H__
#define __HI_KERNEL_DEV_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_DEV_MSGKEY_MAX        8192

extern hi_uint32 hi_kernel_dev_init( hi_void );
extern hi_uint32 hi_kernel_dev_exit( hi_void );
extern hi_uint32 hi_kernel_dev_regmodule( hi_uint32 ui_srcmodule );
extern hi_uint32 hi_kernel_dev_unregmodule( hi_uint32 ui_srcmodule );
extern hi_uint32 hi_kernel_dev_find( hi_uint32 ui_srcmodule );
extern hi_uint32 hi_kernel_dev_dump(hi_void);
extern hi_uint32 hi_kernel_dev_msgkey( hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );
extern hi_uint32 hi_kernel_dev_unmsgkey( hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_DEV_H__ */
