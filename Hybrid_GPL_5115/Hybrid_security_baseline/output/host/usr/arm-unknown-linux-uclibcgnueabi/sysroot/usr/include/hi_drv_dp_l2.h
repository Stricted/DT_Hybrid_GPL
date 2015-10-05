/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_flow.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   : L2 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月24日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus  */
#endif /* __cpluscplus  */


#ifndef __HI_DRV_DP_L2_H__
#define    __HI_DRV_DP_L2_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"


#define HI_DP_L2_BC_TABLE_ITEM_NUM          4608          
#define HI_DP_L2_MC_TABLE_ITEM_NUM          1024
#define HI_DP_L2_USER_TABLE_ITEM_NUM        32
#define HI_DP_L2_VLAN_TABLE_ITEM_NUM        4096
#define HI_DP_L2_ACT_TABLE_ITEM_NUM         192
#define HI_DP_L2_MAC_TABLE_ITEM_NUM         1024

// h and t used only
//#if defined (CONFIG_HISI_SD5115_TYPE_H) || defined (CONFIG_HISI_SD5115_TYPE_T)
#define HI_DP_L2_MAC_TABLE_ITEM_NUM_16K_MODE         16384
#define HI_DP_L2_MAC_TABLE_MAX_ITEM_NUM         16392  
//#endif

#define HI_DP_L2_MAC_ESP_TABLE_ITEM_NUM         8

#define HI_DP_L2_MC_NUM    256

#ifdef CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_L2_SPECIFY_MC_NUM   8
#endif

/* 
组播查表索引模式
只有MAC查表
0：VLAN+MAC查表
 */
typedef enum{
    HI_DP_L2_MATCH_MAC_VLAN_E = 0,
    HI_DP_L2_MATCH_MAC_E,
}HI_DP_L2_MATCH_MODE_E;


/* MAC学习模式 */
typedef enum
{
    HI_DP_L2_MAC_LOGIC_LEARN_FROM_PORT_E = 0,   /* 逻辑基于端口学习模式;        */
    HI_DP_L2_MAC_LOGIC_LEARN_FROM_FLOW_E,       /* 逻辑基于流学习模式;        */

    HI_DP_L2_MAC_CPU_LEARN_FROM_PORT_E,         /* CPU软件基于端口学习模式;        */
    HI_DP_L2_MAC_CPU_LEARN_FROM_FLOW_E,         /* CPU软件基于流学习模式;        */    
}HI_DP_L2_MAC_LEARN_MODE_E;

/* L3组播匹配模式 */
typedef enum{
    HI_DP_L2_IP_MC_MATCH_MODE_L2_E = 0,                 /* 兼容v120 */
    HI_DP_L2_IP_MC_MATCH_MODE_GROUP_E,                  /* (*,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_VLAN_GROUP_E,            /* (V,*,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_SOURCE_GROUP_E,          /* (*,S,G) */
    HI_DP_L2_IP_MC_MATCH_MODE_VLAN_SOURCE_GROUP_E,    /* (V,S,G) */
}HI_DP_L2_IP_MC_MATCH_MODE_E;


typedef struct
{

    HI_BOOL_E         em_ipv4_smac_rep_en;    /* IPv4组播SMAC替换模式，TX_MC_BASE_CFG_REG */
    HI_BOOL_E         em_ipv6_smac_rep_en;    /* IPv6组播SMAC替换模式，TX_MC_BASE_CFG_REG */

    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_match_mode;                /* IPv4OE非隧道组播匹配模式 */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_match_mode;                /* IPv6OE非隧道组播匹配模式 */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_ppp_match_mode;            /* Ipv6oPPPoE隧道组播匹配模式（仅针对ONT拨号） */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_ppp_match_mode;            /* Ipv4oPPPoE隧道组播匹配模式（仅针对ONT拨号） */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv4_in_tunnel_match_mode;      /* IP隧道组播匹配模式（仅对出IP隧道报文有效） */
    HI_DP_L2_IP_MC_MATCH_MODE_E    em_ipv6_in_tunnel_match_mode;      /* IP隧道(6RD)组播匹配模式（仅对出IP隧道报文有效） */
}hi_dp_l2_mc_attr_s;

/* BEGIN: Added by w00180499, 2012/7/20 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
/* 学习本地端口定义 */
typedef union
{
    struct
    {
        uint eth0 : 1;      /* [0] */
        uint eth1 : 1;      /* [1] */
        uint eth2 : 1;      /* [2] */

        uint eth3 : 1;      /* [3] */
        uint eth4 : 1;      /* [4] */
        uint pon  : 1;      /* [5] */
        uint cpu  : 1;      /* [6] */
        uint prbs : 1;      /* [7] */
        uint reserved : 24;
    } bits;

    uint value;
} hi_dp_l2_learn_local_u;

/* ARL 全局和本地学习数目控制 */
typedef struct
{
    HI_BOOL_E              em_glb_lmt_en;               /* 全局端口自动学习新表项数目限制使能 */
    HI_BOOL_E              em_glb_lrn_cnt_clr_en;      /* 全局端口学习数目清零配置 */
    uint                   ui_glb_lmt_num;              /* 全局端口自动学习新表项数目 */

    HI_BOOL_E              em_local_lmt_en;             /* 本地端口自动学习新表项数目限制使能 */
    HI_BOOL_E              em_local_lrn_cnt_clr_en;    /* 本地端口学习数目清零配置 */
    uint                   ui_local_lmt_num;            /* 本地端口自动学习新表项数目 */
    hi_dp_l2_learn_local_u u_lrn_local;                  /* 学习本地端口定义 */
}hi_dp_l2_learn_lmt_attr_s;

typedef struct
{
    uint ui_index;
    uint ui_vlan;
    HI_BOOL_E em_vlan_en;
    uchar     uc_mac[6];
}hi_dp_l2_mc_st_attr_s;
#endif
/* END:   Added by w00180499, 2012/7/20 */

/* L2配置属性 */
typedef struct  
{    
    /* 转发抑制 */  
    /* MAC学习相关配置 */
    HI_DP_L2_MAC_LEARN_MODE_E   em_mac_learn_mode;          /* MAC学习模式 */    
    HI_DP_TRANS_ACT_E   em_mac_learn_fail_act;              /* MAC学习失败后的相应报文转发行为 */  

    //5115 add;增加prbs口
    HI_BOOL_E   em_mac_learn_en[HI_DP_PORT_NUM+1];            /* 各端口MAC学习使能 */    
    HI_BOOL_E   em_mac_learn_limit_en[HI_DP_PORT_NUM+1];      /* 各端口MAC学习数目限制使能 */
    uint        ui_mac_learn_limit_num[HI_DP_PORT_NUM+1];     /* 各端口MAC学习数目限制 */

    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    HI_BOOL_E   em_port_stat_en[HI_DP_PORT_NUM];                /*端口统计的状态使能*/              
    HI_DP_CNT_MODE_E   em_port_stat_type[HI_DP_PORT_NUM]; /*端口统计的统计类型*/

    HI_BOOL_E em_sw_uuc_thru;                     /*接入交换模式下，未知单播的行为*/
    HI_BOOL_E em_sw_umc_thru;                     /*接入交换模式下，未知组播的行为*/
    #endif
	
    /* 老化配置 */
    HI_BOOL_E    em_mac_age_en;                             /* MAC表老化使能 */
    uint ui_mac_age_time;          

    /* 查表模式 */  
    HI_DP_L2_MATCH_MODE_E   em_mc_match_mode;            /* 组播查表索引模式 */
    HI_DP_L2_MATCH_MODE_E   em_uc_match_mode;            /* 单播查表索引模式 */

    /* L2转发(查表)失败处理  */
    HI_DP_TRANS_ACT_E   em_uc_unhit_act;                    /* 单播查表失败处理 */   /* 5115增加选择 */
    HI_DP_TRANS_ACT_E   em_tunnel_mc_unhit_act;                  /* 隧道多播查表失败处理 */ 
    
    /* 广播和组播选择转发处理；仅支持:HI_DP_TRANS_BC_E，HI_DP_TRANS_BC_BY_VLAN_E */
    HI_DP_TRANS_ACT_E   em_bc_act;
    HI_DP_TRANS_ACT_E   em_mc_act;
    
    /* 基于gemport和ip version分别配置组播查表失败的处理 */
    HI_DP_TRANS_ACT_E    em_pon_mc_unhit_act[HI_DP_IP_VERSION][HI_DP_MC_GEMPORT_TYPE];   
    hi_dp_port_u    u_mc_egr[HI_DP_IP_VERSION][HI_DP_MC_GEMPORT_TYPE];      /* 多播查表失败act TOPOINT时有效 */

    /* 已知单播，未知单播，未知组播端口隔离  */
    hi_dp_port_u   u_igr_filter_en[HI_DP_PORT_NUM];     //已知报文的端口隔离
    hi_dp_port_u   u_uc_unhit_igr_filter_en[HI_DP_PORT_NUM];     //未知单报的端口隔离
    hi_dp_port_u   u_mc_unhit_igr_filter_en[HI_DP_PORT_NUM];     //未知组播的端口隔离    

    HI_BOOL_E    em_mc_filter_en;   /* 组播过滤使能  */
    uchar        uc_mc_white_mac[HI_DP_MAC_BYTE_NUM];
    uchar        uc_mc_white_mac_mask[HI_DP_MAC_BYTE_NUM];

    /* SD5113V200 TXDMA 新增 TX_ LANS_L3IF_MAC_L_REG等 */
    uchar    uc_lan_mac[HI_DP_LAN_NUM][HI_DP_MAC_BYTE_NUM];    /* ONT LAN侧MAC地址 */
    uchar    uc_wan_mac[HI_DP_WAN_NUM][HI_DP_MAC_BYTE_NUM];    /* ONT WAN侧MAC地址 */    //5115 add

    /* 组播属性 */
    hi_dp_l2_mc_attr_s    st_mc_attr;
    
#ifdef CONFIG_HISI_SD5115_TYPE_T
    /* ARL 全局和本地学习数目控制 */
    hi_dp_l2_learn_lmt_attr_s st_lrn_lmt_attr;
#endif

}hi_dp_l2_attr_s;


typedef struct
{
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_uc_pkt[HI_DP_PORT_NUM];                /* 已知单播报文计数 */
    uint ui_uuc_pkt[HI_DP_PORT_NUM];             /* 未知单播报文计数 */
    uint ui_mc_pkt[HI_DP_PORT_NUM];                /* 已知组播报文计数 */
    uint ui_umc_pkt[HI_DP_PORT_NUM];             /* 未知组播报文计数 */
    uint ui_bc_pkt[HI_DP_PORT_NUM];                           /* 广播报文计数 */
    uint ui_unknow_type_pkt[HI_DP_PORT_NUM];  
    
    ulong ul_uc_byte[HI_DP_PORT_NUM];
    ulong ul_uuc_byte[HI_DP_PORT_NUM];
    ulong ul_mc_byte[HI_DP_PORT_NUM];
    ulong ul_umc_byte[HI_DP_PORT_NUM];
    ulong ul_bc_byte[HI_DP_PORT_NUM];
    ulong ul_unknow_type_byte[HI_DP_PORT_NUM];
#ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_specify_mc_pkt[HI_DP_L2_SPECIFY_MC_NUM];         /* 指定多播组的报文计数 */
    ulong ul_specify_mc_byte[HI_DP_L2_SPECIFY_MC_NUM];
#endif
    #endif
}hi_dp_l2_cnt_s;


typedef struct
{

}hi_dp_l2_sta_s;


typedef struct 
{
    uint    ui_index;    
    uchar   uc_mac[6]; 
    ushort  us_reserved;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;
    uint    ui_fid;                 /* FID，用来查找VLAN ACTION表  */
    hi_dp_port_u    u_egr;          /* 对于组播报文,表示组播复制索引,即组播ID */
    HI_BOOL_E   em_age_en;
    
    HI_BOOL_E  em_valid;
    
}hi_dp_l2_mac_table_item_s; 


typedef struct 
{
    uchar   uc_mac[6];
    ushort  us_arg;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;       /* 5115 add;表示是否为untag报文 */

    uint    ui_fid;                 /* FID，用来查找VLAN ACTION表  */
    hi_dp_port_u    u_egr;
    HI_BOOL_E   em_age_en;
        
}hi_dp_l2_mac_s;    



typedef struct 
{
    uint   ui_fid;

    HI_DP_VALN_ACT_E    em_cvlan_act;
    uint    ui_cvlan_value;         

    HI_DP_VALN_ACT_E    em_svlan_act;
    uint    ui_svlan_value;         

    HI_DP_8021P_ACT_E   em_c8021p_act;
    uint    ui_c8021p_value;

    HI_DP_8021P_ACT_E   em_s8021p_act;
    uint    ui_s8021p_value;

    HI_DP_TAG_ACT_E em_tag_act;

    /* 5115 add */
    HI_DP_TPID_REP_MODE_E   em_ctpid_act;
    HI_DP_TPID_REP_MODE_E   em_stpid_act;
    
    HI_BOOL_E   em_valid; 
    
}hi_dp_l2_act_table_item_s;



typedef enum{

    HI_DP_L2_VALN_REP_NO_ACTION = 0,
    HI_DP_L2_VALN_REP_CVLAN,       /* 内层CVLAN替换 */
    HI_DP_L2_VALN_REP_SVLAN,       /* 外层SVLAN替换 */

}HI_DP_L2_VALN_REP_E;


/* 由指定的ID, MAC和VLAN唯一确定一个组播组 */    
#define    HI_DP_L2_MC_USER_MAX_NUM  32


typedef struct  
{
    uint    ui_mc_id;                   /* 0 - 255;HI_DP_TRANS_MC_LIST_E有效 */
    hi_dp_port_u u_mc_port;             /* 组播用户;HI_DP_TRANS_MC_PORTMAP_E有效 */
    
    uchar   uc_mac[6];       
    ushort  us_arg;  
    uint    ui_vlan;
    HI_BOOL_E em_untag_en;
    
}hi_dp_l2_mc_s;

typedef struct
{
    uint ui_index;

    HI_BOOL_E em_valid;
    uchar     uc_mac[6];
}hi_dp_l2_mc_filter_s;

typedef struct
{
    uint                  ui_index;    /* 0-255 */
    HI_BOOL_E             em_valid;

    uchar                 uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];    /*mc, group; dip */
    uchar                 uc_sip[HI_DP_IPV6_ADDR_BYTE_NUM];   /* source; sip */
    HI_DP_IP_VERSION_E    em_ip_version; 
    uint                  ui_vlan;

    uint                  ui_mc_id;     /* 组播复制索引 */
    hi_dp_port_u          u_mc_port;    /* 组播用户;HI_DP_TRANS_MC_PORTMAP_E有效 */

    HI_BOOL_E             em_l3_replace_en;
    uint                  ui_l3_replace_index;    /* OIF2表索引 */
    
}hi_dp_l2_ip_mc_s;



/*  组播用户信息结构体 */
typedef struct  
{
    uint                ui_vlan;        /* VLAN + PORT 表示一个用户 */
    HI_DP_PORT_E        em_egr;
    HI_DP_L2_VALN_REP_E em_vlan_act;   
    HI_BOOL_E           em_valid;
}hi_dp_l2_mc_user_info_s;


/* 指定组播ID下的用户信息操作 */
typedef struct  
{
    uint    ui_mc_id;                  /* 0 - 255 */

    hi_dp_l2_mc_user_info_s  st_user;
    
}hi_dp_l2_mc_user_s;


/* 指定组播ID下的所有用户信息操作 */
typedef struct  
{
    uint     ui_mc_id;                  /* 0 - 255 */

    hi_dp_l2_mc_user_info_s st_users[HI_DP_L2_MC_USER_MAX_NUM]; /* 最多32个用户 */
      
}hi_dp_l2_mc_users_s;



#define    HI_DP_L2_BC_EGR_MAX_NUM  32   

typedef struct  
{
    uint            ui_fid;
    HI_DP_PORT_E    em_egr;
    HI_BOOL_E       em_valid;

}hi_dp_l2_bc_egr_info_s;

typedef struct  
{
    uint    ui_vlan;                /* 0 - 4095   ,索引 */

    hi_dp_l2_bc_egr_info_s  st_egr;

}hi_dp_l2_bc_egr_s;


typedef struct  
{
    uint    ui_vlan;                /* 0 - 4095   ,索引 */

    hi_dp_l2_bc_egr_info_s   st_egrs[HI_DP_L2_BC_EGR_MAX_NUM];

}hi_dp_l2_bc_egrs_s;



#endif /* end of __HI_DRV_DP_L2_H__ */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */



