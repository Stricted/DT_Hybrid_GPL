/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_drv_dp_vlan_filter.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��24��
  ����޸�   :
  ��������   : FLOW ģ�����ݽṹͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��24��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

***************************************************************************** */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus  */
#endif /* __cpluscplus  */


#ifndef __HI_DRV_DP_VLAN_FILTER_H__
#define    __HI_DRV_DP_VLAN_FILTER_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"





#define HI_DP_L2_VLAN_FILTER_TABLE_ITEM_NUM        4096
#define   HI_DP_SMAC_FILTER_LIST_NUM    8
#define  HI_DP_IP_FILTER_TABLE_ITEM_SIZE   64
#define  HI_DP_IP_FILTER_MASK_MAX         127
#define  HI_DP_IP_FILTER_IPV4_MASK_MAX     32
#define  HI_DP_IP_FILTER_ITEM_IPV4_ADDR_NUM      4

/* vlan ���˱��� */
typedef struct 
{
    uint    ui_vlan;
    
    HI_BOOL_E   em_eth0_en;
    HI_BOOL_E   em_eth1_en;
    HI_BOOL_E   em_eth2_en;
    HI_BOOL_E   em_eth3_en;
    HI_BOOL_E   em_eth4_en;
    HI_BOOL_E   em_pon_en;
    HI_BOOL_E   em_cpu_en;
    
}hi_dp_vlan_filter_table_item_s;



/* �˿�ԴMAC����ģʽ  */
typedef enum
{
    HI_DP_SMAC_IGNORE_E = 0,        /* ������ԴMAC  */
    HI_DP_SMAC_ALLOW_E,             /* ԴMAC����������ͨ�� ,�������� */
    HI_DP_SMAC_DROP_E,              /* ԴMAC��������,�������� */
    HI_DP_SMAC_THRU                 /* ԴMAC������͸�������п� */
}HI_DP_SMAC_FILTER_MODE_E;

typedef enum
{
    HI_DP_IP_FILTER_SEL_DIP_E = 0,   /* DIP���� */
    HI_DP_IP_FILTER_SEL_SIP_E,
}HI_DP_IP_FILTER_SEL_E;

typedef enum
{
    HI_DP_IP_FILTER_DIRECTION_UNI_E = 0,
    HI_DP_IP_FILTER_DIRECTION_NNI_E,
}HI_DP_IP_FILTER_DIRECTION_E;

typedef enum
{
    HI_DP_DROP_E = 0,          /* ������ */
    HI_DP_ALLOW_E,             /* ������ */
}HI_DP_FILTER_MODE_E;

typedef struct 
{
    HI_BOOL_E   em_enable;
    hi_mac_s    st_mac;      
}hi_dp_smac_filter_list_s;

typedef struct 
{
      HI_DP_PORT_E  em_port;

      HI_DP_SMAC_FILTER_MODE_E  em_filter_mode;
    
      hi_dp_smac_filter_list_s  st_item[HI_DP_SMAC_FILTER_LIST_NUM];
    
}hi_dp_smac_filter_table_item_s;



typedef struct
{
    HI_DP_TRANS_ACT_E      em_uni_act;    /*uni ��ƥ��IP���˱��İ�ȫ��Ϊ */   
    HI_DP_TRANS_ACT_E      em_nni_act;    /*nni ��ƥ��IP���˱��İ�ȫ��Ϊ */   

}hi_dp_security_ip_attr_s;

#ifdef CONFIG_HISI_SD5115_TYPE_T
typedef enum
{
    HI_DP_EGR_VLAN_AUTH_SVLAN = 0,        /* �޸ĺ�����vlan */
    HI_DP_EGR_VLAN_AUTH_TRANS_VLAN        /* ת��vlan */
}HI_DP_EGR_VLAN_AUTH_E;
#endif


typedef struct 
{
    HI_BOOL_E   em_filter_igr_en[HI_DP_PORT_NUM];    /* ���vlan��֤ʹ�� */
    HI_BOOL_E   em_filter_egr_en[HI_DP_PORT_NUM];  
#ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_DP_EGR_VLAN_AUTH_E   em_egr_vlan_auth_sel;     /* VLAN���ڼ��ѡ�� */
#endif
}hi_dp_security_vlan_attr_s;

typedef struct 
{
    hi_dp_security_ip_attr_s     st_ip_attr;   /* IP��ȫ����*/
    hi_dp_security_vlan_attr_s   st_vlan_attr; /* VLAN��ȫ����*/	
    
}hi_dp_security_attr_s;

typedef union
{
    uchar uc_ipv6[HI_DP_IPV6_ADDR_BYTE_NUM];
    uint  ui_ipv4[HI_DP_IP_FILTER_ITEM_IPV4_ADDR_NUM];
}hi_dp_ip_filter_addr_u;

typedef struct
{
    uint                           ui_index;
    HI_BOOL_E                      em_valid;

    HI_DP_PORT_TYPE_E              em_port_type;    /* NNI,UNI*/
    HI_DP_FIELD_E                  em_field;        /* DIP or SIP */

    HI_DP_IP_VERSION_E             em_ip_version;

    hi_dp_ip_filter_addr_u         u_ip;


    uint                           ui_mask_range;  /* ���뷶Χ  bit��Χ*/


}hi_dp_ip_filter_table_item_s;


/*���ݱ���*/
typedef struct 
{
    HI_BOOL_E   em_vlan_filter_en[HI_DP_PORT_NUM];    /* em_vlan_filter_en[0]����MPW��em_vlan_filter_en���� */
    
}hi_dp_vlan_filter_attr_s;

typedef struct
{
    uint ui_vlan_filter;        /* VLAN���˶������ļ��� */
    uint ui_blacklist_filter;   /* ������ԴMAC���˶������ļ��� */
    uint ui_whitelist_filter;   /* ��������������ͳ�� */
    uint ui_smac_thru;          /* ԴMAC͸�� */
}hi_dp_vlan_filter_cnt_s;


typedef struct
{
    uint ui_vlan_filter;        /* VLAN���˶������ļ��� */
    uint ui_blacklist_filter;   /* ������ԴMAC���˶������ļ��� */
    uint ui_whitelist_filter;   /* ��������������ͳ�� */
    uint ui_smac_thru;          /* ԴMAC͸�� */

#ifdef CONFIG_HISI_SD5115_TYPE_T
    /* ���ڶ˿ڵ�vlan������֤ͳ�� */
    uint ui_eth0_vlan_chk;
    uint ui_eth1_vlan_chk;
    uint ui_eth2_vlan_chk;
    uint ui_eth3_vlan_chk;
    uint ui_eth4_vlan_chk;
    uint ui_pon_vlan_chk;
    uint ui_cpu_vlan_chk;
#endif

}hi_dp_security_cnt_s;

typedef struct
{

}hi_dp_security_sta_s;



#endif /* end of __XXX_H__ */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


