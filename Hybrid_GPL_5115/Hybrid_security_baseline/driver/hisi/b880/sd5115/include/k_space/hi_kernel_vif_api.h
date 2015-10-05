/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_vif_api.h
  版 本 号   : 初稿
  作    者   : luocheng 00183967
  生成日期   : D2012_03_28

******************************************************************************/
#ifndef __HI_KERNEL_VIF_API_H__
#define __HI_KERNEL_VIF_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_KERNEL_VIF_NAMELEN                       32
#define HI_KERNEL_VIF_MAX_VLAN_CNT                  4
#define HI_KERNEL_VIF_PRI_NUM                       8

/*car/pq*/
#define HI_KERNEL_VIF_CAR_DEFTEMPLATE_INDX          0
#define HI_KERNEL_VIF_PQ_DEFTEMPLATE_INDX           0

/*虚接口类型*/    
#define HI_KERNEL_VIF_TYPE_ROOTNODE             0
#define HI_KERNEL_VIF_TYPE_INTFACE              1
#define HI_KERNEL_VIF_TYPE_SUBINTFACE           2
#define HI_KERNEL_VIF_TYPE_WAN                  3

/*映射方式*/    
#define HI_KERNEL_VIF_MAPPING_MODE_PORT             0x00000001
#define HI_KERNEL_VIF_MAPPING_MODE_DMAC             0x00000002
#define HI_KERNEL_VIF_MAPPING_MODE_SMAC             0x00000004
#define HI_KERNEL_VIF_MAPPING_MODE_DIP              0x00000008
#define HI_KERNEL_VIF_MAPPING_MODE_SIP              0x00000010
#define HI_KERNEL_VIF_MAPPING_MODE_GEMPORTID        0x00000020
#define HI_KERNEL_VIF_MAPPING_MODE_LLID             0x00000040
#define HI_KERNEL_VIF_MAPPING_MODE_ETHTYPE          0x00000080
#define HI_KERNEL_VIF_MAPPING_MODE_SESSIONID        0x00000100
#define HI_KERNEL_VIF_MAPPING_MODE_TPID             0x00000200
#define HI_KERNEL_VIF_MAPPING_MODE_PRI              0x00000400
#define HI_KERNEL_VIF_MAPPING_MODE_VID              0x00000800
#define HI_KERNEL_VIF_MAPPING_MODE_IPPROTOCOL       0x00001000
#define HI_KERNEL_VIF_MAPPING_MODE_IPVERSION        0x00002000
#define HI_KERNEL_VIF_MAPPING_MODE_IPTOS            0x00004000
#define HI_KERNEL_VIF_MAPPING_MODE_DPORT            0x00008000
#define HI_KERNEL_VIF_MAPPING_MODE_SPORT            0x00010000
#define HI_KERNEL_VIF_MAPPING_MODE_REASON           0x00020000
#define HI_KERNEL_VIF_MAPPING_MODE_DMACSEG          0x00040000
#define HI_KERNEL_VIF_MAPPING_MODE_SMACSEG          0x00080000
#define HI_KERNEL_VIF_MAPPING_MODE_DIPSEG           0x00100000
#define HI_KERNEL_VIF_MAPPING_MODE_SIPSEG           0x00200000
#define HI_KERNEL_VIF_MAPPING_MODE_DPORTSEG         0x00400000
#define HI_KERNEL_VIF_MAPPING_MODE_SPORTSEG         0x00800000
#define HI_KERNEL_VIF_MAPPING_MODE_FLAG             0x01000000

/*是否广播到所有子接口*/
#define HI_KERNEL_VIF_BOARDCAST                     1

/*虚接口缺省VLAN*/
#define HI_KERNEL_VIF_VLAN_DEFAULT                      0xFFF

/* VLAN添加动作:系统缺省0
0:缺省UNTAG
1:缺省VLAN + 缺省TPID
2:缺省VLAN + 0x8100 + CVLAN + 0x8100
3:缺省VLAN + 0x8100 + CVLAN + TPID
4:缺省VLAN + 缺省TPID + CVLAN + 0x8100
5:缺省VLAN + 缺省TPID + CVLAN + TPID
*/
#define HI_KERNEL_VIF_VLAN_ACTION_UNTAG                 0x00
#define HI_KERNEL_VIF_VLAN_ACTION_TPID                  0x01
#define HI_KERNEL_VIF_VLAN_ACTION_CVLAN                 0x02
#define HI_KERNEL_VIF_VLAN_ACTION_CTPID                 0x04

/* 与sdk中的HI_DP_NNI_GE_E */
#define HI_KERNEL_VIF_PHYPORT_EXT_START             9
#define HI_KERNEL_VIF_PHYPORT_EXT_NUM               4
#define HI_KERNEL_VIF_PHYPORT_NUM                   (HI_KERNEL_VIF_PHYPORT_EXT_START+HI_KERNEL_VIF_PHYPORT_EXT_NUM)

typedef struct
{
    hi_uchar8 uc_fatherifname[HI_KERNEL_VIF_NAMELEN];
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32 ui_type;  /*接口类型HI_KERNEL_VIF_VIF_TYPE_ROOTNODE*/
}hi_kernel_vif_cmdtype_vif_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32 ui_phyport;  
}hi_kernel_vif_cmdtype_phyvif_s;

typedef struct
{
    hi_uint32   ui_cir;    /*承诺速率*/
    hi_uint32   ui_pir;    /*峰值速率*/          
    hi_ushort16 us_bind;   /*引用计数*/
    hi_ushort16 us_resv;
}hi_kernel_vif_car_s;

typedef struct
{
    hi_ushort16 us_tpid; 
    union
    {
        hi_ushort16 pri:3, 
                    tci:1, 
                    vid:12; 
        hi_ushort16 value;    
    }us_vlan; 
}hi_kernel_vif_vlan_s; 

typedef struct
{
    hi_kernel_vif_vlan_s st_vlan[HI_KERNEL_VIF_MAX_VLAN_CNT]; 
    hi_uint32            ui_cnt; 
}hi_kernel_vif_dev_vlan_info_s; 

typedef struct
{
    hi_uint32 ui_indx;
    hi_kernel_vif_car_s st_data;
}hi_kernel_vif_cmdtype_car_s;

typedef struct
{
    hi_uchar8   uc_schmode;  /*0:sp 1:wrr 2: sp + wrr*/
    hi_uchar8   uc_resv[3];

    /*单位时间内系统允许发包/收包个数*/
    hi_uint32   ui_wgt[HI_KERNEL_VIF_PRI_NUM];
}hi_kernel_vif_pq_s;

typedef struct
{
    hi_uint32 ui_indx;
    hi_kernel_vif_pq_s st_data;
}hi_kernel_vif_cmdtype_pq_s;

typedef struct
{
    /*上行car动态数据*/
    hi_uint32 ui_car_up_lasttime;      /*上次计算令牌时间*/
    hi_uint32 ui_car_up_token;         /*当前令牌数,每发送一个报文加一,与模板数值比较*/
    hi_uint32 ui_car_stat_up_green;    /*统计计数*/
    hi_uint32 ui_car_stat_up_yellow;   /*统计计数*/
    hi_uint32 ui_car_stat_up_red;      /*统计计数*/
    /*下行car动态数据*/
    hi_uint32 ui_car_dn_lasttime;      /*上次计算令牌时间*/
    hi_uint32 ui_car_dn_token;         /*当前令牌数,每发送一个报文加一,与模板数值比较*/
    hi_uint32 ui_car_stat_dn_green;    /*统计计数*/
    hi_uint32 ui_car_stat_dn_yellow;   /*统计计数*/
    hi_uint32 ui_car_stat_dn_red;      /*统计计数*/
    /*misc*/
    hi_uint32 ui_napt_stat;    
}hi_kernel_vif_traffic_stat_s;

typedef struct
{
    hi_uchar8   uc_phyport;
    hi_uchar8   uc_llid;
    hi_ushort16 us_gemportid;

    hi_uchar8   uc_dmac[HI_MAC_LEN];
    hi_uchar8   uc_smac[HI_MAC_LEN];

    hi_uint32   ui_dip;
    hi_uint32   ui_sip;

    hi_ushort16 us_dport;
    hi_ushort16 us_sport;

    hi_ushort16 us_ethtype;
    hi_ushort16 us_sessionid; /*ppp会话id*/

    hi_kernel_vif_vlan_s st_vlan;
    
    hi_uchar8   uc_reason;
    hi_uchar8   uc_ipprotocol;
    hi_uchar8   uc_ipversion;
    hi_uchar8   uc_iptos;

    hi_uint32   ui_flag;
}hi_kernel_vif_mapping_s;

typedef struct
{
    hi_kernel_vif_mapping_s st_mapping; 
    hi_kernel_vif_vlan_s            st_vlan[HI_KERNEL_VIF_MAX_VLAN_CNT]; 
    hi_uint32                       ui_vlan_index;     
}hi_kernel_vif_mapping_info_s;

typedef struct 
{
    hi_uchar8   uc_dmask[HI_MAC_LEN];
    hi_uchar8   uc_smask[HI_MAC_LEN];
    hi_uint32   ui_dipmask;
    hi_uint32   ui_sipmask;
    hi_ushort16 us_dpmask;
    hi_ushort16 us_spmask;
    hi_uint32   ui_flagmask;
}hi_kernel_vif_mappingmask_s;

typedef struct
{
    hi_uchar8 uc_fatherifname[HI_KERNEL_VIF_NAMELEN];
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32 ui_indx; /*接口下流索引*/
    hi_kernel_vif_mapping_s st_data;
}hi_kernel_vif_cmdtype_mapping_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uchar8 uc_car_up_indx;          /*流量调度模板索引,缺省0*/
    hi_uchar8 uc_car_dn_indx;          /*流量调度模板索引,缺省0*/
    hi_uchar8 uc_resv;
    hi_uchar8 uc_pq_indx;              /*流量调度模板索引,缺省0*/
}hi_kernel_vif_cmdtype_traffic_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_kernel_vif_traffic_stat_s st_data;
}hi_kernel_vif_cmdtype_traffic_stat_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_kernel_vif_mappingmask_s st_mask;
}hi_kernel_vif_cmdtype_vifmask_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uchar8 uc_mac[ HI_MAC_LEN ];
    hi_uchar8 uc_resv[2];    
}hi_kernel_vif_cmdtype_mac_s;

typedef struct
{
    hi_uchar8   uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uchar8   uc_vlanflag;  /*vlan处理动作 HI_KERNEL_VIF_VLAN_ACTION_UNTAG*/  
    hi_uchar8   uc_resv[3];
    hi_ushort16 us_defvlan;   /*缺省0为没有激活*/
    hi_ushort16 us_deftpid;   /*缺省0x8100*/
}hi_kernel_vif_cmdtype_vlan_s;

typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_VIF_NAMELEN];

    hi_uint32 ui_broadcast;         /*是否广播到所有子接口*/
    hi_uint32 ui_mappingtype;
    hi_uint32 ui_iftype;        /* 接口类型 */
}hi_kernel_vif_cmdtype_misc_s;

typedef struct
{
    hi_uchar8   uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32   ui_phyport; 
}hi_kernel_vif_cmdtype_phyport_s; 


typedef struct
{
    hi_uchar8   uc_fatherifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32   uc_ifname[HI_KERNEL_VIF_NAMELEN]; 
}hi_kernel_vif_cmdtype_fatherif_s; 


typedef struct
{
    hi_uchar8   uc_ifname[HI_KERNEL_VIF_NAMELEN];
    hi_uint32   ui_dumptype;
}hi_kernel_vif_cmdtype_dump_s;

/*******************************************************/
typedef hi_int32 (*hi_kernel_vif_callback)(hi_kernel_sk_buff_s *pst_skb, hi_void *pv_data );
extern hi_void hi_kernel_vif_softengine_hook_reg( hi_kernel_vif_callback pv_rxcallback);
extern hi_void hi_kernel_vif_softengine_hook_unreg( hi_void );

extern hi_uint32 hi_kernel_vif_get_phyport(hi_kernel_net_device_s *pst_dev, hi_uint32 *pui_phyport);
extern hi_uint32 hi_kernel_vif_get_vlan( hi_kernel_net_device_s *pst_dev, hi_kernel_vif_dev_vlan_info_s *pst_vlan );

extern hi_uint32 hi_kernel_vif_check_device(hi_kernel_net_device_s *pst_dev);  
extern hi_uint32 hi_kernel_vif_check_local_device(hi_kernel_net_device_s *pst_dev);  
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_VIF_API_H__ */
