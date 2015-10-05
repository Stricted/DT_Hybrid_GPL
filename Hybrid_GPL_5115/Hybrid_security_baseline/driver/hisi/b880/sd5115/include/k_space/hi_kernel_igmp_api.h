/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_igmp_api.h
  版 本 号   : 初稿
  作    者   : h66243
  生成日期   : D2011_10_06
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_IGMP_API_H__
#define __HI_KERNEL_IGMP_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************************************************************
 回调函数功能描述  : 添加/删除组播节目回调函数类型
 ******************************************************************************/
typedef hi_uint32 (* HI_KERNEL_IGMP_GRPCALLBACK)(hi_uchar8 *puc_mac, hi_ushort16 us_vlan, hi_uchar8 *puc_ifname);

/*****************************************************************************
 功能描述  : 组播SNOOPING支持只对配置接口进行组播业务处理,即上报报文的接口
             没有在组播SNOOPING的接口列表中被配置,将不会处理该报文。
             系统缺省支持5个接口名称的接口eth0/ath0/ath1/ath2/ath3请注意大小写
             系统对大小写敏感
             为了区分接口的报文是来自物理以太接口FE1/FE2/FE3/FE4或者对其的虚拟以太接口
             还是来自CPU侧的WIFI/USB其它PCI/PCIE或者其它扩展接口接入的设备,需要配置相应
             的接口类型,否则组播报文将无法正确转发。
             
             uc_type当前支持以下接入类型:
             typedef enum
             {
                 HI_KERNEL_IGMP_IFTYPE_PHYETH   = 1,  物理以太接口
                 HI_KERNEL_IGMP_IFTYPE_PHYETH_VIRT,   虚拟以太接口
                 HI_KERNEL_IGMP_IFTYPE_WIFI,          无线接入接口
             }hi_kernel_igmp_iftype_e;
             
             当前支持添加/删除/查询三类接口,每个接口通过接口名称唯一匹配,puc_name名称最大长度16字节.
             uc_enable无须输入,添加配置成功后设置为1,删除后为0.被删除的接口查询结果为失败.
             
             注意:系统最大支持16个不同名字的接口
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_add_ifname(hi_uchar8 *puc_name, hi_uchar8 uc_type);
extern hi_uint32 hi_kernel_api_igmp_del_ifname(hi_uchar8 *puc_name);
extern hi_uint32 hi_kernel_api_igmp_get_ifname(hi_uchar8 *puc_name, hi_uchar8 *puc_enable, hi_uchar8 *puc_type);

/*****************************************************************************
 功能描述  : proxy模式设置，默认为disable
             输入参数: hi_uint32 ui_igmpproxy_mode，
             取值范围: enable ＝1 ， disable ＝0， 
             当使能时,设置IGMP协议报文只转发到CPU；
             当不使能时，设置IGMP协议报文透传到PON，同时复制到CPU。
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_proxymode(hi_uint32 ui_igmpproxy_mode);
extern hi_uint32 hi_kernel_api_igmp_get_proxymode(hi_uint32 *pui_igmpproxy_mode);

/*****************************************************************************
 功能描述  : snooping模式设置，默认为enable
             输入参数: hi_uint32 ui_enable,
             取值范围: enable ＝1 ， disable ＝0， 
             当使能时,只有加入特定组的端口才能收到特定组的组播数据报文,此时组播数据流使用缺省组播节目vlan
             当不使能时，播数据flood到每个端口，此时组播数据流使用缺省用户vlan
             disable : unmatch packet flood, use defuservlan ;enable : to igmp snooping use defgrpvlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_snoopingmode(hi_uint32 ui_enable);
extern hi_uint32 hi_kernel_api_igmp_get_snoopingmode(hi_uint32 *pui_enable);

/*****************************************************************************
 功能描述  : 配置缺省组播节目vlan值，默认值等于4068,注意与OLT下行组播数据流VLAN保持一致。
             输入参数: hi_uint32 ui_vlan 
             取值范围: [0,4095]
             查询缺省组播节目vlan，输入参数:hi_uint32 *pui_vlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_defgrpvlan(hi_uint32 ui_vlan );
extern hi_uint32 hi_kernel_api_igmp_get_defgrpvlan(hi_uint32 *pui_vlan );

/*****************************************************************************
 功能描述  : 配置缺省用户vlan值，默认值等于1。
             输入参数: hi_uint32 ui_vlan 
             取值范围: [0,4095]
             查询缺省用户vlan，输入参数:hi_uint32 *pui_vlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_defusrvlan(hi_uint32 ui_vlan );
extern hi_uint32 hi_kernel_api_igmp_get_defusrvlan(hi_uint32 *pui_vlan );



extern hi_uint32 hi_kernel_api_igmp_add_groupvlan_ip(hi_ushort16 us_tableindex,hi_ushort16 us_gourpvlan, hi_uint32 ui_startip, hi_uint32 ui_endip);
extern hi_uint32 hi_kernel_api_igmp_del_groupvlan_ip(hi_ushort16 us_tableindex);
extern hi_uint32 hi_kernel_api_igmp_get_gourpvan_ipinfo(hi_void);

/*****************************************************************************
 功能描述  : 配置组播节目缺省老化时间，默认值等于300秒。
             输入参数: hi_uint32 ui_agetimes 
             取值范围: [none]
             查询组播节目缺省老化时间，输入参数:hi_uint32 *pui_agetimes
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_agetimes(hi_uint32 ui_agetimes );
extern hi_uint32 hi_kernel_api_igmp_get_agetimes(hi_uint32 *pui_agetimes );

/*****************************************************************************
 功能描述  : 配置是否进行用户vlan切换操作,缺省不进行切换操作
             输入参数: hi_uint32 ui_usrvlanact 
             取值范围: enable ＝1 ， disable ＝0， 
             查询用户vlan切换操作,输入参数:hi_uint32 *pui_usrvlanact
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_usrvlanact(hi_uint32 ui_usrvlanact);
extern hi_uint32 hi_kernel_api_igmp_get_usrvlanact(hi_uint32 *pui_usrvlanact);

/*****************************************************************************
 功能描述  : 显示组播所有用户信息，节目信息，老化时间信息，设备接口信息
             输入参数: 无
             输出参数:直接打印方式
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_get_usrinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_grpinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_ageinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_ifinfo( hi_void);

/*****************************************************************************
 功能描述  : 显示或者清除组播所有统计信息
             输入参数: 无
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_get_statinfo(hi_void);
extern hi_uint32 hi_kernel_api_igmp_clr_statinfo(hi_void);

/*****************************************************************************
 功能描述  : 底层注册增加节目回调函数,当snoopying添加节目时同步调用该接口
             知节目更新
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_reg_grp_addcallback( HI_KERNEL_IGMP_GRPCALLBACK *pst_callback );

/*****************************************************************************
 功能描述  : 底层注册删除节目回调函数,当snoopying添加节目时同步调用该接口
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_reg_grp_delcallback( HI_KERNEL_IGMP_GRPCALLBACK *pst_callback );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_IGMP_API_H__ */

