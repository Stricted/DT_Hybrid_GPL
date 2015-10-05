/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_sd511X.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_08
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_MPCP_H__
#define __HI_KERNEL_MPCP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum {
    HI_KERNEL_SD511X_CMDTYPE_MPCP_START_E          = (HI_SUBMODULE_KSOC_MPCP  | 0x01),
    HI_KERNEL_SD511X_CMDTYPE_MPCP_STOP_E           = (HI_SUBMODULE_KSOC_MPCP  | 0x02),
    HI_KERNEL_SD511X_CMDTYPE_MPCP_GET_STATUS_E     = (HI_SUBMODULE_KSOC_MPCP  | 0x03),
    HI_KERNEL_SD511X_CMDTYPE_MPCP_GET_LLD0MAC_E    = (HI_SUBMODULE_KSOC_MPCP  | 0x04),
    HI_KERNEL_SD511X_CMDTYPE_MPCP_SET_LLD0MAC_E    = (HI_SUBMODULE_KSOC_MPCP  | 0x05),
    HI_KERNEL_SD511X_CMDTYPE_MPCP_SET_SILENCE_TIME = (HI_SUBMODULE_KSOC_MPCP  | 0x06),
}hi_kernel_sd511Xmpcp_cmdtype_e;

typedef enum {
    HI_KERNEL_SD511X_CMDTYPE_READ_MEM_E            = (HI_SRCMODULE_KSOC_SD5113 | 0x01),
    HI_KERNEL_SD511X_CMDTYPE_WRITE_MEM_E           = (HI_SRCMODULE_KSOC_SD5113 | 0x02),
    HI_KERNEL_SD511X_CMDTYPE_READ_PHYMEM_E         = (HI_SRCMODULE_KSOC_SD5113 | 0x03),
    HI_KERNEL_SD511X_CMDTYPE_WRITE_PHYMEM_E        = (HI_SRCMODULE_KSOC_SD5113 | 0x04),
}hi_kernel_sd511Xmisc_cmdtype_e;

#define HI_KERNEL_MPCP_SILENCE_BY_LOID_AUTH_FAIL       0
#define HI_KERNEL_MPCP_REREG_BY_LOID_RESET             1 
#define HI_KERNEL_MPCP_SILENCE_BY_AUTH_FAIL           0

typedef struct
{
    hi_uchar8   uc_llidindex;
    hi_uchar8   uc_reason;
    hi_ushort16 us_res;
    hi_uint32   ui_timeout;
}hi_kernel_mpcp_silence_s;

extern hi_uint32 hi_kernel_mpcp_set_silence_time(hi_uchar8 uc_llidindex, hi_uint32 ui_timeout, hi_uchar8 uc_reason);
extern hi_int32 hi_kernel_mpcp_status_callback(HI_FUNCCALLBACK pf_mpcpcallback);
extern hi_int32 hi_kernel_mpcp_get_status(hi_uint32 *pui_status);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif 
