/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_kernel_igmp_api.h
  �� �� ��   : ����
  ��    ��   : h66243
  ��������   : D2011_10_06
  ����޸�   :

******************************************************************************/
#ifndef __HI_KERNEL_IGMP_API_H__
#define __HI_KERNEL_IGMP_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************************************************************
 �ص�������������  : ���/ɾ���鲥��Ŀ�ص���������
 ******************************************************************************/
typedef hi_uint32 (* HI_KERNEL_IGMP_GRPCALLBACK)(hi_uchar8 *puc_mac, hi_ushort16 us_vlan, hi_uchar8 *puc_ifname);

/*****************************************************************************
 ��������  : �鲥SNOOPING֧��ֻ�����ýӿڽ����鲥ҵ����,���ϱ����ĵĽӿ�
             û�����鲥SNOOPING�Ľӿ��б��б�����,�����ᴦ��ñ��ġ�
             ϵͳȱʡ֧��5���ӿ����ƵĽӿ�eth0/ath0/ath1/ath2/ath3��ע���Сд
             ϵͳ�Դ�Сд����
             Ϊ�����ֽӿڵı���������������̫�ӿ�FE1/FE2/FE3/FE4���߶����������̫�ӿ�
             ��������CPU���WIFI/USB����PCI/PCIE����������չ�ӿڽ�����豸,��Ҫ������Ӧ
             �Ľӿ�����,�����鲥���Ľ��޷���ȷת����
             
             uc_type��ǰ֧�����½�������:
             typedef enum
             {
                 HI_KERNEL_IGMP_IFTYPE_PHYETH   = 1,  ������̫�ӿ�
                 HI_KERNEL_IGMP_IFTYPE_PHYETH_VIRT,   ������̫�ӿ�
                 HI_KERNEL_IGMP_IFTYPE_WIFI,          ���߽���ӿ�
             }hi_kernel_igmp_iftype_e;
             
             ��ǰ֧�����/ɾ��/��ѯ����ӿ�,ÿ���ӿ�ͨ���ӿ�����Ψһƥ��,puc_name������󳤶�16�ֽ�.
             uc_enable��������,������óɹ�������Ϊ1,ɾ����Ϊ0.��ɾ���Ľӿڲ�ѯ���Ϊʧ��.
             
             ע��:ϵͳ���֧��16����ͬ���ֵĽӿ�
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_add_ifname(hi_uchar8 *puc_name, hi_uchar8 uc_type);
extern hi_uint32 hi_kernel_api_igmp_del_ifname(hi_uchar8 *puc_name);
extern hi_uint32 hi_kernel_api_igmp_get_ifname(hi_uchar8 *puc_name, hi_uchar8 *puc_enable, hi_uchar8 *puc_type);

/*****************************************************************************
 ��������  : proxyģʽ���ã�Ĭ��Ϊdisable
             �������: hi_uint32 ui_igmpproxy_mode��
             ȡֵ��Χ: enable ��1 �� disable ��0�� 
             ��ʹ��ʱ,����IGMPЭ�鱨��ֻת����CPU��
             ����ʹ��ʱ������IGMPЭ�鱨��͸����PON��ͬʱ���Ƶ�CPU��
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_proxymode(hi_uint32 ui_igmpproxy_mode);
extern hi_uint32 hi_kernel_api_igmp_get_proxymode(hi_uint32 *pui_igmpproxy_mode);

/*****************************************************************************
 ��������  : snoopingģʽ���ã�Ĭ��Ϊenable
             �������: hi_uint32 ui_enable,
             ȡֵ��Χ: enable ��1 �� disable ��0�� 
             ��ʹ��ʱ,ֻ�м����ض���Ķ˿ڲ����յ��ض�����鲥���ݱ���,��ʱ�鲥������ʹ��ȱʡ�鲥��Ŀvlan
             ����ʹ��ʱ��������flood��ÿ���˿ڣ���ʱ�鲥������ʹ��ȱʡ�û�vlan
             disable : unmatch packet flood, use defuservlan ;enable : to igmp snooping use defgrpvlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_snoopingmode(hi_uint32 ui_enable);
extern hi_uint32 hi_kernel_api_igmp_get_snoopingmode(hi_uint32 *pui_enable);

/*****************************************************************************
 ��������  : ����ȱʡ�鲥��Ŀvlanֵ��Ĭ��ֵ����4068,ע����OLT�����鲥������VLAN����һ�¡�
             �������: hi_uint32 ui_vlan 
             ȡֵ��Χ: [0,4095]
             ��ѯȱʡ�鲥��Ŀvlan���������:hi_uint32 *pui_vlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_defgrpvlan(hi_uint32 ui_vlan );
extern hi_uint32 hi_kernel_api_igmp_get_defgrpvlan(hi_uint32 *pui_vlan );

/*****************************************************************************
 ��������  : ����ȱʡ�û�vlanֵ��Ĭ��ֵ����1��
             �������: hi_uint32 ui_vlan 
             ȡֵ��Χ: [0,4095]
             ��ѯȱʡ�û�vlan���������:hi_uint32 *pui_vlan
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_defusrvlan(hi_uint32 ui_vlan );
extern hi_uint32 hi_kernel_api_igmp_get_defusrvlan(hi_uint32 *pui_vlan );



extern hi_uint32 hi_kernel_api_igmp_add_groupvlan_ip(hi_ushort16 us_tableindex,hi_ushort16 us_gourpvlan, hi_uint32 ui_startip, hi_uint32 ui_endip);
extern hi_uint32 hi_kernel_api_igmp_del_groupvlan_ip(hi_ushort16 us_tableindex);
extern hi_uint32 hi_kernel_api_igmp_get_gourpvan_ipinfo(hi_void);

/*****************************************************************************
 ��������  : �����鲥��Ŀȱʡ�ϻ�ʱ�䣬Ĭ��ֵ����300�롣
             �������: hi_uint32 ui_agetimes 
             ȡֵ��Χ: [none]
             ��ѯ�鲥��Ŀȱʡ�ϻ�ʱ�䣬�������:hi_uint32 *pui_agetimes
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_agetimes(hi_uint32 ui_agetimes );
extern hi_uint32 hi_kernel_api_igmp_get_agetimes(hi_uint32 *pui_agetimes );

/*****************************************************************************
 ��������  : �����Ƿ�����û�vlan�л�����,ȱʡ�������л�����
             �������: hi_uint32 ui_usrvlanact 
             ȡֵ��Χ: enable ��1 �� disable ��0�� 
             ��ѯ�û�vlan�л�����,�������:hi_uint32 *pui_usrvlanact
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_set_usrvlanact(hi_uint32 ui_usrvlanact);
extern hi_uint32 hi_kernel_api_igmp_get_usrvlanact(hi_uint32 *pui_usrvlanact);

/*****************************************************************************
 ��������  : ��ʾ�鲥�����û���Ϣ����Ŀ��Ϣ���ϻ�ʱ����Ϣ���豸�ӿ���Ϣ
             �������: ��
             �������:ֱ�Ӵ�ӡ��ʽ
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_get_usrinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_grpinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_ageinfo( hi_void);
extern hi_uint32 hi_kernel_api_igmp_get_ifinfo( hi_void);

/*****************************************************************************
 ��������  : ��ʾ��������鲥����ͳ����Ϣ
             �������: ��
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_get_statinfo(hi_void);
extern hi_uint32 hi_kernel_api_igmp_clr_statinfo(hi_void);

/*****************************************************************************
 ��������  : �ײ�ע�����ӽ�Ŀ�ص�����,��snoopying��ӽ�Ŀʱͬ�����øýӿ�
             ֪��Ŀ����
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_reg_grp_addcallback( HI_KERNEL_IGMP_GRPCALLBACK *pst_callback );

/*****************************************************************************
 ��������  : �ײ�ע��ɾ����Ŀ�ص�����,��snoopying��ӽ�Ŀʱͬ�����øýӿ�
*****************************************************************************/
extern hi_uint32 hi_kernel_api_igmp_reg_grp_delcallback( HI_KERNEL_IGMP_GRPCALLBACK *pst_callback );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_IGMP_API_H__ */

