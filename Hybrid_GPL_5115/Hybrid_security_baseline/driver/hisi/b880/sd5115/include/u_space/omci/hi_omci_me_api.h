/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_me_api.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2012_08_22

******************************************************************************/
#ifndef __HI_OMCI_ME_API_H__
#define __HI_OMCI_ME_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_uint32 hi_omci_mib_saveapi(hi_void *pv_data,hi_uint32 ui_inlen,hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_tcont_set_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_tcont_get_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_8021p_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_mib_recv_msg(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_dbg_set_upgradefile(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

//pptp eth uni
extern hi_uint32 hi_omci_me_pptpethuni_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_pptpethuni_get_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_ethpm_get(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);
extern hi_uint32 hi_omci_me_ethpm2_get(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ethpm3_get(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ethpm4_get(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_ethpm_getcurrent(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ethpm2_getcurrent(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ethpm3_getcurrent(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ethpm4_getcurrent(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

//layer2
extern hi_uint32 hi_omci_me_exvlantag_create_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_exvlantag_del_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_exvlantag_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_hwextvlantag_crt_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_hwextvlantag_set_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_hwextvlantag_del_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_mac_serv_create_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_mac_serv_set_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_mac_serv_del_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_mac_design_get_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_vlan_filt_set_before (hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_vlan_filt_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_vlan_filt_create_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_vlan_filt_del_bef (hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_maccfg_create_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_maccfg_del_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_macportcfg_crt_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_macportcfg_del_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_macportcfg_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_vlantag_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_vlantag_del_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_vlantag_create_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_multi_oper_creat_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_multi_oper_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_macport_table_set(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_macportpm_get(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);
extern hi_uint32 hi_omci_me_macportpm_getcur(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);
//ANI
extern hi_uint32 hi_omci_me_multiwtp_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_multiwtp_create_aft(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_multiwtp_del_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ani_g_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ani_g_get_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_ani_g_test(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_fec_pm_getcurrent(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_fec_pm_get(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_gem_ctp_create_aft(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);
extern hi_uint32 hi_omci_me_gem_ctp_set_after(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);

//equip
extern hi_uint32 hi_omci_me_extability_set_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_onu_g_get_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_softimage_activate(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_softimage_get_bef(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_softimage_commit(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_rmtdbg_set_after(hi_void * pv_data, hi_uint32 ui_in, hi_uint32 * pui_outlen);

//layer3
extern hi_uint32 hi_omci_me_iphost_set_after(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);
extern hi_uint32 hi_omci_me_iphost_get_before(hi_void*pv_data, hi_uint32 ui_in, hi_uint32 *pui_outlen);

extern hi_uint32 hi_omci_me_set_gemport_qos(hi_ushort16 us_opertaion,hi_omci_me_gem_ctp_s *pst_gemctp);

extern hi_uint32 hi_omci_me_macport_table_create(hi_ushort16 us_instid);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_ME_API_H__ */
