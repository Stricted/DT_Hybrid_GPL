/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_tapi.h
  版 本 号   : 初稿
  作    者   : h66243
  生成日期   : D2012_4_16

******************************************************************************/
#ifndef __HI_OMCI_TAPI_H__
#define __HI_OMCI_TAPI_H__
#include "hi_adapter.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* iphost cfg */
extern hi_uint32 hi_omci_tapi_set_dhcpcfg(hi_uchar8 uc_dhcpoption);
extern hi_uint32 hi_omci_tapi_set_ipaddr(hi_uint32 ui_ipaddr,hi_uint32 ui_mask,hi_uint32 ui_gateway,
	                                     hi_uint32 ui_pridns,hi_uint32 ui_secdns);
extern hi_uint32 hi_omci_tapi_get_ipaddr(hi_uint32 *pui_ipaddr,hi_uint32 *pui_mask,hi_uint32 *pui_gateway,
	                                     hi_uint32 *pui_pridns,hi_uint32 *pui_secdns);
/* onu-g */
extern hi_uint32 hi_omci_tapi_get_version(hi_omci_me_onu_g_s* st_onug);

/* ani-g */
extern hi_uint32 hi_omci_tapi_get_anig(hi_omci_me_ani_g_msg_s* pst_anig);
extern hi_uint32 hi_omci_tapi_set_anig(hi_omci_me_ani_g_msg_s* pst_anig);
extern hi_uint32 hi_omci_tapi_anig_test(hi_short16 * ps_voltage, hi_short16 * ps_rx, hi_short16 * ps_tx, hi_short16 * ps_bias, hi_short16 * ps_tep);
extern hi_uint32 hi_omci_tapi_anig_testresult(hi_void *pv_data);

/*software image*/
extern hi_uint32 hi_omci_tapi_softimage_get(hi_ushort16 us_instid,hi_omci_me_software_image_msg_s* pst_image);
extern hi_uint32 hi_omci_tapi_softimage_activate(hi_ushort16 us_instid,hi_omci_me_software_image_msg_s* st_image);
extern hi_uint32 hi_omci_tapi_softimage_commit(hi_ushort16 us_instid,hi_omci_me_software_image_msg_s* st_image);

/*pm*/
extern hi_uint32 hi_omci_tapi_get_eth_statistic(hi_lsw_port_e em_port, hi_lsw_eth_cnt_s * pst_pm);

extern hi_uint32 hi_omci_tapi_rmtdbg_execmd(const hi_char8 *pc_in,  hi_char8 *pc_out);

extern hi_uint32 hi_omci_tapi_macportpm_get(hi_omci_me_macportpm_s *pst_data);
extern hi_uint32 hi_omci_tapi_ethpm_get(hi_omci_me_msg_ethpm_s *pst_data);
extern hi_uint32 hi_omci_tapi_ethpm2_get(hi_omci_me_ethpm2_s *pst_data);
extern hi_uint32 hi_omci_tapi_ethpm3_get(hi_omci_me_msg_ethpm3_s *pst_data);
extern hi_uint32 hi_omci_tapi_ethpm4_get(hi_omci_me_msg_ethpm4_s *pst_data);
extern hi_uint32 hi_omci_tapi_fecpm_get(hi_omci_me_fec_pm_s *pst_data);

extern hi_uint32 hi_omci_tapi_clear_stat( hi_void );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_TAPI_H__ */
