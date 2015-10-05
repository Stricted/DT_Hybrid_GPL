/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_hlp.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_21
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_HLP_H__
#define __HI_KERNEL_HLP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_HLP_MISC_DYNAMIC_MINOR    0x00000013 // MISC_DYNAMIC_MINOR 动态申请
#define HI_KERNEL_HLP_MISCDEVNAME           "hi_hlp"

/* 系统支持最大命令数 */
#define HI_KERNEL_HLP_HASHSIZE              0xFFFF
#define HI_KERNEL_HLP_ITEMSIZE              16
#define HI_KERNEL_HLP_CONFLICTSIZE          1024
/* 系统支持高层协议长度 */
#define HI_KERNEL_HLP_DATABUFF              2048

typedef struct  {
    hi_uint32  ui_len;           /* inlen/outlen */
    hi_uchar8  uc_funcname[HI_FUNCNAME_MAXLEN];
    hi_uchar8  uc_text[4];
}hi_kernel_hlp_head_s;

extern hi_uint32  hi_kernel_hlp_init( hi_void );
extern hi_uint32  hi_kernel_hlp_exit( hi_void );
extern hi_uint32  hi_kernel_hlp_dump( hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_HLP_H__ */
