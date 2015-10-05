/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_ploam.h
  版 本 号   : 初稿
  作    者   : t00185260
  生成日期   : 2012_02_27

******************************************************************************/
#ifndef __HI_KERNEL_PLOAM_H__
#define __HI_KERNEL_PLOAM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_OMCI_PAYLOAD_LEN                  256
#define HI_PLOAM_SN_LEN                               8       // Serial number octet
#define HI_PLOAM_PWD_LEN                             10      // Password octet

typedef enum {
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_START_E         = (HI_SUBMODULE_KSOC_PLOAM | 0x01),       
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_STOP_E          = (HI_SUBMODULE_KSOC_PLOAM | 0x02),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_SETSN_E         = (HI_SUBMODULE_KSOC_PLOAM | 0x03),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_GETSN_E         = (HI_SUBMODULE_KSOC_PLOAM | 0x04),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_SETPWD_E        = (HI_SUBMODULE_KSOC_PLOAM | 0x05),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_GETPWD_E        = (HI_SUBMODULE_KSOC_PLOAM | 0x06),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_GETONUID_E      = (HI_SUBMODULE_KSOC_PLOAM | 0x07),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_GETSTATUS_E     = (HI_SUBMODULE_KSOC_PLOAM | 0x08),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_DUMPSTAT_E      = (HI_SUBMODULE_KSOC_PLOAM | 0x09),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_DUMPEVENT_E     = (HI_SUBMODULE_KSOC_PLOAM | 0x0A),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_ROUGERONU_E     = (HI_SUBMODULE_KSOC_PLOAM | 0x0B),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_SETTXPOWER_E    = (HI_SUBMODULE_KSOC_PLOAM | 0x0C),
    HI_KERNEL_SD511X_CMDTYPE_PLOAM_RSTCIRCUIT_E    = (HI_SUBMODULE_KSOC_PLOAM | 0x0D),
}hi_kernel_sd511Xploam_cmdtype_e;

typedef enum
{
    HI_KERNEL_PLOAM_REPORT_OMCI_MSG     = ( HI_SUBMODULE_KSOC_PLOAM | 0xF1 ),
    HI_KERNEL_PLOAM_REPORT_ONU_STATUS   = ( HI_SUBMODULE_KSOC_PLOAM | 0xF2 ),
    HI_KERNEL_PLOAM_REPORT_ONU_ROGUESTA = ( HI_SUBMODULE_KSOC_PLOAM | 0xF3 ),
}hi_kernel_ploam_report_e;

typedef enum {
    HI_PLOAM_ROGUEONU_OFF_E = 0,
    HI_PLOAM_ROGUEONU_ON_E,
    HI_PLOAM_LASER_OFF_E,               
}hi_ploam_rougesonutate_e;

typedef enum {
    HI_KERNEL_PLOAM_ROGUE_POWER_OFF = 0,
    HI_KERNEL_PLOAM_ROGUE_POWER_ON,
    HI_KERNEL_PLOAM_ROGUE_STATUS_OFF,
    HI_KERNEL_PLOAM_ROGUE_STATUS_ON,
}hi_kernel_ploam_roguerpt_e;

typedef struct {
    hi_uint32   ui_msgtype;
    hi_uint32   ui_len;
    hi_uchar8   a_uc_content[HI_KERNEL_OMCI_PAYLOAD_LEN]; 
}hi_kernel_omcimsg_s;

typedef struct {
    hi_uchar8 auc_sn[HI_PLOAM_SN_LEN];      // ONU&T Serial Number = VID + VSSN
    hi_uchar8 auc_pwd[HI_PLOAM_PWD_LEN];    // ONU&T password
    hi_kernel_ploam_roguerpt_e ui_roguesta;
}__attribute__((packed)) hi_kernel_ploam_param_s;

extern hi_void hi_kernel_ploam_set_lasermode(hi_uchar8 uc_state);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_PLOAM_H__ */
