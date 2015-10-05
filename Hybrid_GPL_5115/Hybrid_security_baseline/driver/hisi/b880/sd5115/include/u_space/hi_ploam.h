/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_ploam.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_PLOAM_H__
#define __HI_PLOAM_H__

#include "hi_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_OMCI_PAYLOAD_LEN             256
#define HI_PLOAM_PWD_LEN                10      // Password octet
#define HI_PLOAM_SN_LEN                 8       // Serial number octet

typedef struct {
    hi_uint32   ui_msgtype;
    hi_uint32   ui_len;
    hi_uchar8   a_uc_content[HI_OMCI_PAYLOAD_LEN];
}hi_omcimsg_s;

typedef enum {
    HI_PLOAM_REPORT_OMCI_MSG = 0,
    HI_PLOAM_REPORT_ONU_STATUS,
    HI_PLOAM_REPORT_ONU_ROGUESTA,
    HI_PLOAM_REPORT_ONU_MAX,
}hi_ploam_report_e;


typedef enum {
    HI_PLOAM_ROGUE_POWER_OFF = 0,
    HI_PLOAM_ROGUE_POWER_ON,
    HI_PLOAM_ROGUE_STATUS_OFF,
    HI_PLOAM_ROGUE_STATUS_ON,
}hi_ploam_roguerpt_e;

typedef struct {
    hi_uchar8 auc_sn[HI_PLOAM_SN_LEN];
    hi_uchar8 auc_pwd[HI_PLOAM_PWD_LEN];
    hi_ploam_roguerpt_e ui_roguesta;
}__attribute((packed)) hi_ploam_param_s;
    
extern hi_uint32 hi_ploam_init( HI_FUNCCALLBACK *g_a_pf_callback );
extern hi_int32 hi_ploam_onuid_get( hi_uint32 *pui_onuid );
extern hi_int32 hi_ploam_onustatus_get( hi_uint32 *pui_onustatus );
extern hi_int32 hi_ploam_pwd_get( hi_uchar8 *puc_pwd );
extern hi_int32 hi_ploam_pwd_set( hi_uchar8 *puc_pwd );
extern hi_int32 hi_ploam_sn_get( hi_uchar8 *puc_sn );
extern hi_int32 hi_ploam_sn_set( hi_uchar8 *puc_sn );
extern hi_int32 hi_ploam_start(hi_ploam_param_s *pst_param );
extern hi_int32 hi_ploam_stop( hi_void );
extern hi_int32 hi_ploam_reg_ploamcallback( hi_void *pv_callback );
extern hi_uint32 hi_omci_set_payloadlen(hi_uint32 ui_len);
extern hi_uint32 hi_ploam_set_lasermode(hi_uint32 *puc_flag);
extern hi_int32 hi_ploam_circuit_rst( hi_void );
extern hi_uint32 hi_ploam_allocid_set(hi_uint32 ui_valid, hi_uint32 ui_tcont, hi_uint32 ui_allocid);

#ifdef __cplusplus
}
#endif


#endif  /* __HI_PLOAM_H__ */
