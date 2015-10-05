/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_igmp.h
  版 本 号   : 初稿
  作    者   : kf55233
  生成日期   : D2012_02_17
  功能描述   : 组播用户态接口
******************************************************************************/
#ifndef __HI_IGMP_API_H__
#define __HI_IGMP_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



#define HI_IGMP_DEVNAME_LEN    16
/******************************************************************************
 回调函数功能描述  : 添加/删除组播节目回调函数类型
 ******************************************************************************/
typedef hi_uint32 (* HI_IGMP_GRPCALLBACK)(hi_uchar8 *puc_mac, hi_ushort16 us_vlan, hi_uchar8 *puc_ifname);

/* 接口信息 */
typedef struct
{
    hi_uchar8       uc_enable;
    hi_uchar8       uc_type;      /* 设备类型:物理以太接口/虚拟以太接口/无线接入接口  */
    hi_uchar8       uc_resv[2];
    
    hi_uchar8       uc_name[HI_IGMP_DEVNAME_LEN];
}hi_api_igmp_ifinfo_s;

typedef struct
{
    hi_ushort16              us_gourpvlan;             /* multicst VLAN */
    hi_ushort16              us_tableindex;
    hi_uint32                 ui_startip;             /* multicast start ip*/   
    hi_uint32                 ui_endip;             /* multicast end ip*/
}hi_igmp_gourpvlan_s;

extern hi_uint32 hi_igmp_add_gourpvlan_ip(hi_ushort16 us_tableindex,hi_ushort16 us_gourpvlan,hi_uint32 ui_startip,hi_uint32 ui_endip);
extern hi_uint32 hi_igmp_del_gourpvlan_ip(hi_ushort16 us_tableindex);
extern hi_uint32 hi_igmp_add_ifname(hi_uchar8 *puc_name, hi_uchar8 uc_type);
extern hi_uint32 hi_igmp_clr_statinfo( hi_void);
extern hi_uint32 hi_igmp_del_ifname(hi_uchar8 *puc_name);
extern hi_uint32 hi_igmp_dump( hi_uint32 ui_mode);
extern hi_uint32 hi_igmp_get_ageinfo(hi_void);
extern hi_uint32 hi_igmp_get_agetimeinter( hi_uint32 *pui_agetimeinter );
extern hi_uint32 hi_igmp_get_agetimes(hi_uint32 *pui_agetimes);
extern hi_uint32 hi_igmp_get_autolrnvlan( hi_uint32 *pui_mode);
extern hi_uint32 hi_igmp_get_defgrpvlan( hi_uint32 *pui_grp_defvlan );
extern hi_uint32 hi_igmp_get_defusrvlan( hi_uint32 *pui_usr_defvlan );
extern hi_uint32 hi_igmp_get_genqueryip( hi_uint32 *pui_ipaddr);
extern hi_uint32 hi_igmp_get_genquerymode(  hi_uint32 *pui_mode);
extern hi_uint32 hi_igmp_get_grpinfo(hi_void);
extern hi_uint32 hi_igmp_get_ifinfo(hi_void);
extern hi_uint32 hi_igmp_get_ifname(hi_uchar8 *puc_name, hi_uchar8 *puc_enable, hi_uchar8 *puc_type);
extern hi_uint32 hi_igmp_get_proxymode(hi_uint32 *pui_igmpproxy_mode);
extern hi_uint32 hi_igmp_get_snoopingmode( hi_uint32 *pui_igmpsnoping_mode);
extern hi_uint32 hi_igmp_get_statinfo(hi_void);
extern hi_uint32 hi_igmp_get_usrinfo(hi_void);
extern hi_uint32 hi_igmp_get_usrvlanact( hi_uint32 *pui_usrvlanact);
extern hi_uint32 hi_igmp_set_agetimeinter( hi_uint32 ui_agetimeinter );
extern hi_uint32 hi_igmp_set_agetimes( hi_uint32 ui_agetimes );
extern hi_uint32 hi_igmp_set_autolrnvlan ( hi_uint32 ui_mode );
extern hi_uint32 hi_igmp_set_defgrpvlan(hi_uint32 ui_grp_defvlan);
extern hi_uint32 hi_igmp_set_defusrvlan( hi_uint32 ui_usr_defvlan );
extern hi_uint32 hi_igmp_set_genqueryip( hi_uint32 ui_ipaddr);
extern hi_uint32 hi_igmp_set_genquerymode( hi_uint32 ui_mode);
extern hi_uint32 hi_igmp_set_proxymode( hi_uint32 ui_igmpproxy_mode);
extern hi_uint32 hi_igmp_set_snoopingmode( hi_uint32 ui_igmpsnoping_mode);
extern hi_uint32 hi_igmp_set_usrvlanact(hi_uint32 ui_usrvlanact);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
