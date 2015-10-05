/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_crc.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_07_28
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_CRC_H__
#define __HI_KERNEL_CRC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_uint32 hi_kernel_os_crc_get( hi_uint32 ui_crc, hi_uchar8 *puc_packdata,
                        hi_uint32 ui_packlen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_CRC_H__ */
