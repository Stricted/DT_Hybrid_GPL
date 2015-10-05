/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_flow.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   : FLOW 模块数据结构头文件
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


#ifndef __HI_DRV_DP_FLOW_H__
#define   __HI_DRV_DP_FLOW_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

#define HI_DP_EFC_TRAN_REASON_NUM 16
#define HI_DP_FLOW_POINT_FIELD_NUM        8

#define HI_DP_FLOW_FID_PCNT_NUM     3
#define HI_DP_FLOW_POINT_FIELD_NUM_V200  5
#define HI_DP_FLOW_TFRT_ITEM_NUM            16
#define HI_DP_FLOW_LABEL_PRI_MAX         31
#define HI_DP_FLOW_COLOR_MAX 7
#define HI_DP_FLOW_SFC_ENTRY_PRI_MAX 7

typedef enum{
    HI_DP_FLOW_G0_NO_USE_E = 0,   /* 0x0 */
    HI_DP_FLOW_G0_DMAC_H_E,       /* 0x1:32 bit */
    HI_DP_FLOW_G0_SMAC_H_E,       /* 0x2:32 bit */
    HI_DP_FLOW_G0_DIP_E,          /* 0x3:32 bit */
    HI_DP_FLOW_G0_SIP_E,          /* 0x4:32 bit */ 
    HI_DP_FLOW_G0_SVLAN_E,        /* 0x5:12 bit */
    HI_DP_FLOW_G0_ETH_TYPE_E,     /* 0x6:16 bit */
    HI_DP_FLOW_G0_IP_PROTOCAL_E,  /* 0x7: 8 bit */
    HI_DP_FLOW_G0_IP_VERSION_E,   /* 0x8: 4 bit */
    HI_DP_FLOW_G0_IP_TOS_E,       /* 0x9: 8 bit */
    HI_DP_FLOW_G0_SPORT_E,        /* 0xA:16 bit */
    HI_DP_FLOW_G0_DPORT_E,        /* 0xB:16 bit */
    HI_DP_FLOW_G0_PPPOE_PROTOCAL_E,/* 0xC:16 bit */
    HI_DP_FLOW_G0_ETH_ENCAPSULE_E, /* 0xD:16 bit */
    HI_DP_FLOW_G0_TAG_TYPE_E,      /* 0xE: 2 bit */
    HI_DP_FLOW_G0_S8021P_E,        /* 0xF: 3 bit */
    HI_DP_FLOW_G0_CVLAN_E,         /* 0x10:12 bit */
    HI_DP_FLOW_G0_GEM_PORT_E,      /* 0x11:12 bit */
    HI_DP_FLOW_G0_TAG_SUM_E,       /* 0x12: 2 bit */
    HI_DP_FLOW_G0_FRM_TYPE_E,      /* 0x13: 2 bit */
    HI_DP_FLOW_G0_POINT_FIELD0_E,  /* 0x14: 8 bit */
    HI_DP_FLOW_G0_POINT_FIELD1_E,  /* 0x15: 8 bit */
    HI_DP_FLOW_G0_POINT_FIELD2_E,  /* 0x16: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD3_E,  /* 0x17: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD4_E,  /* 0x18: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD5_E,  /* 0x19: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD6_E,  /* 0x1A: 16 bit */
    HI_DP_FLOW_G0_POINT_FIELD7_E,  /* 0x1B: 16 bit */
    HI_DP_FLOW_G0_C8021P_E,        /* 0x1C: 3 bit */
    HI_DP_FLOW_G0_PPP_ENCAP_E,     /* 0x1D: 1 bit */
    HI_DP_FLOW_G0_FC_ID_E,         /* 0x1E: 3 bit */
    HI_DP_FLOW_G0_SESSION_ID_E,    /* 0x1F:16 bit */
    HI_DP_FLOW_G0_IP_FRAG_E,       /* 0x20: 1 bit */ /* IP分片报文标志 */
    
    HI_DP_FLOW_G0_IP_STPID_E,      /* 0x21:16 bit */
    HI_DP_FLOW_G0_IP_CTPID_E,      /* 0x22:16 bit */
    HI_DP_FLOW_G0_IP_IP_TYPE_E,    /* 0x23: 2 bit */  /* IP报文，含IPv4oE、IPv6oE、IPv4oPPPoE和IPv6oPPPoE */
    HI_DP_FLOW_G0_SIPV6_96_TO_127_BIT_E,  /* 0x24:32 bit */     
    HI_DP_FLOW_G0_SIPV6_64_TO_95_BIT_E,   /* 0x25:32 bit */
    HI_DP_FLOW_G0_SIPV6_32_TO_63_BIT_E,   /* 0x26:32 bit */
    HI_DP_FLOW_G0_SIPV6_0_TO_31_BIT_E,    /* 0x27:32 bit */
    HI_DP_FLOW_G0_DIPV6_96_TO_127_BIT_E,  /* 0x28:32 bit */
    HI_DP_FLOW_G0_DIPV6_64_TO_95_BIT_E,   /* 0x29:32 bit */
    HI_DP_FLOW_G0_DIPV6_32_TO_63_BIT_E,   /* 0x2A:32 bit */
    HI_DP_FLOW_G0_DIPV6_0_TO_31_BIT_E,    /* 0x2B:32 bit */
    HI_DP_FLOW_G0_IPV6_TRAFFIC_CLASS_E,   /* 0x2C: 8 bit */
    HI_DP_FLOW_G0_IPV6_FLOW_LABEL_E,      /* 0x2D:20 bit */
    HI_DP_FLOW_G0_IPV6_NEXT_HEADER,       /* 0x2E: 8 bit */
    HI_DP_FLOW_G0_IPV6_EXTEND_NEXT_HEADER, /* 0x2F: 8 bit */
    HI_DP_FLOW_G0_IPV6_HOP_LIMIT_E,        /* 0x30: 8 bit */
    HI_DP_FLOW_G0_IPV4_TTL_E,              /* 0x31: 8 bit */
    HI_DP_FLOW_G0_IPV6_TUNNEL_E,           /* 0x32: 2 bit */    /* 需要增加枚举；非隧道，DS-Lite，6RD */
    HI_DP_FLOW_G0_ICMPV6_TYPE_E,           /* 0x33: 8 bit */
    HI_DP_FLOW_G0_DMAC_TYPE_E,             /* 0x34: 48 bit */
    HI_DP_FLOW_G0_SMAC_TYPE_E,             /* 0x35: 48 bit */
    HI_DP_FLOW_G0_DIPV6_TYPE_E,           /* 0x36: 128 bit */
    HI_DP_FLOW_G0_SIPV6_TYPE_E,           /* 0x37: 128 bit */
#ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_DP_FLOW_G0_CDEI_E,                       /*0X38: 1bit*/
    HI_DP_FLOW_G0_SDEI_E,                       /*0X39: 1bit*/
#endif
    HI_DP_FLOW_G0_TYPE_MAX_E,
}HI_DP_FLOW_CHARACTER_G0_E;

/* extend流匹配的特征域1 */
typedef enum{
    HI_DP_FLOW_G1_NO_USE_E = 0,   /* 0x0 */
    HI_DP_FLOW_G1_DMAC_L_E,       /* 0x1:16 bit */
    HI_DP_FLOW_G1_SMAC_L_E,       /* 0x2:16 bit */
    HI_DP_FLOW_G1_DIP_E,          /* 0x3:32 bit */
    HI_DP_FLOW_G1_SIP_E,          /* 0x4:32 bit */ 
    HI_DP_FLOW_G1_SVLAN_E,        /* 0x5:12 bit */
    HI_DP_FLOW_G1_ETH_TYPE_E,     /* 0x6:16 bit */
    HI_DP_FLOW_G1_IP_PROTOCAL_E,  /* 0x7: 8 bit */
    HI_DP_FLOW_G1_IP_VERSION_E,   /* 0x8: 4 bit */
    HI_DP_FLOW_G1_IP_TOS_E,       /* 0x9: 8 bit */
    HI_DP_FLOW_G1_SPORT_E,        /* 0xA:16 bit */
    HI_DP_FLOW_G1_DPORT_E,        /* 0xB:16 bit */
    HI_DP_FLOW_G1_PPPOE_PROTOCAL_E,/* 0xC:16 bit */
    HI_DP_FLOW_G1_ETH_ENCAPSULE_E, /* 0xD:16 bit */
    HI_DP_FLOW_G1_TAG_TYPE_E,      /* 0xE: 2 bit */
    HI_DP_FLOW_G1_S8021P_E,        /* 0xF: 3 bit */
    HI_DP_FLOW_G1_CVLAN_E,         /* 0x10:12 bit */
    HI_DP_FLOW_G1_GEM_PORT_E,      /* 0x11:12 bit */
    HI_DP_FLOW_G1_TAG_SUM_E,       /* 0x12: 2 bit */
    HI_DP_FLOW_G1_FRM_TYPE_E,      /* 0x13: 2 bit */
    HI_DP_FLOW_G1_POINT_FIELD0_E,  /* 0x14: 8 bit */
    HI_DP_FLOW_G1_POINT_FIELD1_E,  /* 0x15: 8 bit */
    HI_DP_FLOW_G1_POINT_FIELD2_E,  /* 0x16: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD3_E,  /* 0x17: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD4_E,  /* 0x18: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD5_E,  /* 0x19: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD6_E,  /* 0x1A: 16 bit */
    HI_DP_FLOW_G1_POINT_FIELD7_E,  /* 0x1B: 16 bit */
    HI_DP_FLOW_G1_C8021P_E,        /* 0x1C: 3 bit */
    HI_DP_FLOW_G1_PPP_ENCAP_E,     /* 0x1D: 1 bit */
    HI_DP_FLOW_G1_FC_ID_E,         /* 0x1E: 3 bit */
    HI_DP_FLOW_G1_SESSION_ID_E,    /* 0x1F:16 bit */
    HI_DP_FLOW_G1_IP_FRAG_E,       /* 0x20: 1 bit */ /* IP分片报文标志 */

    HI_DP_FLOW_G1_IP_STPID_E,      /* 0x21:16 bit */
    HI_DP_FLOW_G1_IP_CTPID_E,      /* 0x22:16 bit */
    HI_DP_FLOW_G1_IP_IP_TYPE_E,    /* 0x23: 2 bit */  /* IP报文，含IPv4oE、IPv6oE、IPv4oPPPoE和IPv6oPPPoE */
    HI_DP_FLOW_G1_SIPV6_96_TO_127_BIT_E,  /* 0x24:32 bit */     
    HI_DP_FLOW_G1_SIPV6_64_TO_95_BIT_E,   /* 0x25:32 bit */
    HI_DP_FLOW_G1_SIPV6_32_TO_63_BIT_E,   /* 0x26:32 bit */
    HI_DP_FLOW_G1_SIPV6_0_TO_31_BIT_E,    /* 0x27:32 bit */
    HI_DP_FLOW_G1_DIPV6_96_TO_127_BIT_E,  /* 0x28:32 bit */
    HI_DP_FLOW_G1_DIPV6_64_TO_95_BIT_E,   /* 0x29:32 bit */
    HI_DP_FLOW_G1_DIPV6_32_TO_63_BIT_E,   /* 0x2A:32 bit */
    HI_DP_FLOW_G1_DIPV6_0_TO_31_BIT_E,    /* 0x2B:32 bit */
    HI_DP_FLOW_G1_IPV6_TRAFFIC_CLASS_E,   /* 0x2C: 8 bit */
    HI_DP_FLOW_G1_IPV6_FLOW_LABEL_E,      /* 0x2D:20 bit */
    HI_DP_FLOW_G1_IPV6_NEXT_HEADER,       /* 0x2E: 8 bit */
    HI_DP_FLOW_G1_IPV6_EXTEND_NEXT_HEADER, /* 0x2F: 8 bit */
    HI_DP_FLOW_G1_IPV6_HOP_LIMIT_E,        /* 0x30: 8 bit */
    HI_DP_FLOW_G1_IPV4_TTL_E,              /* 0x31: 8 bit */
    HI_DP_FLOW_G1_IPV6_TUNNEL_E,           /* 0x32: 2 bit */    /* 需要增加枚举；非隧道，DS-Lite，6RD */
    HI_DP_FLOW_G1_ICMPV6_TYPE_E,           /* 0x33: 8 bit */

    HI_DP_FLOW_G1_DMAC_TYPE_E,             /* 0x34: 48 bit */
    HI_DP_FLOW_G1_SMAC_TYPE_E,             /* 0x35: 48 bit */
    HI_DP_FLOW_G1_DIPV6_TYPE_E,           /* 0x36: 128 bit */
    HI_DP_FLOW_G1_SIPV6_TYPE_E,           /* 0x37: 128 bit */

#ifdef  CONFIG_HISI_SD5115_TYPE_T
    HI_DP_FLOW_G1_CDEI_E,                       /*0X38: 1bit*/
    HI_DP_FLOW_G1_SDEI_E,                       /*0X39: 1bit*/
#endif
    HI_DP_FLOW_G1_TYPE_MAX_E,
    
}HI_DP_FLOW_CHARACTER_G1_E;


/* 固定的FLOW 分类标签,即流操作指示, 需要详细的应用规范规则 */
typedef enum
{
    /* lebel 0 to 5 为ACT操作标签 */
    HI_DP_FLOW_LABEL_0_E = 0,        
    HI_DP_FLOW_LABEL_1_E,            
    HI_DP_FLOW_LABEL_2_E,
    HI_DP_FLOW_LABEL_3_E,
    HI_DP_FLOW_LABEL_4_E,              
    HI_DP_FLOW_LABEL_5_E,               

    /* lebel 6 to 7 为非ACT操作标签 */    
    HI_DP_FLOW_LABEL_6_E = 6,           
    HI_DP_FLOW_LABEL_7_E,

}HI_DP_FLOW_LABEL_E;


typedef enum
{
    HI_DP_FLOW_EFC_ACT_PRI_8021P_E = 0, /* 802.1P */
    HI_DP_FLOW_EFC_ACT_PRI_DSCP_E,      /* DSCP */ 

}HI_DP_FLOW_EFC_ACT_PRI_MODE_E;

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_PKT_TYPE_UC_E = 0,
    HI_DP_PKT_TYPE_UUC_E,
    HI_DP_PKT_TYPE_MC_FWD_E,
    HI_DP_PKT_TYPE_UMC_E,
    HI_DP_PKT_TYPE_BC_E,
    HI_DP_PKT_TYPE_FWD_E, 
}HI_DP_PKT_TYPE_E;
#endif
typedef struct 
{

    /* 匹配域0 */
    HI_DP_FLOW_CHARACTER_G0_E    em_g0;         /* 域选择组0          */
    uint    ui_key0;                            /* 对应G0的匹配域     */ 
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint    ui_mask0;                           /* K0域的mask         */
    #endif
	    
    HI_DP_MATCH_MODE_E   em_g0_match_mode;
    

    /* 匹配域1 */    
    HI_DP_FLOW_CHARACTER_G1_E    em_g1;         /* 域选择组1          */
    uint    ui_key1;                            /* 对应G1的匹配域     */
    #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint    ui_mask1;                           /* K1域的mask         */
    #endif
	
    HI_DP_MATCH_MODE_E   em_g1_match_mode;

    hi_dp_port_u    u_igr;

    uchar uc_mac[HI_DP_MAC_BYTE_NUM];         /* 仅G0为0x34/0x35时有效 */
    uchar uc_ip[HI_DP_IPV6_ADDR_BYTE_NUM];    /* ipv6地址;仅G0为0x36/0x37时有效 */
    

}hi_dp_flow_efc_rule_s;


typedef struct 
{
    /* tag  行为 */
    HI_DP_TAG_ACT_E em_tag_act;             /* 报文TAG处理           */
        
    /* vlan   行为 */
    HI_DP_VALN_ACT_E    em_cvlan_act;       /* 内层VLAN的操作        */   
    uint    ui_cvlan_value;                 /* 内层VLAN操作值  */
    HI_DP_VALN_ACT_E    em_svlan_act;       /* 外层VLAN的操作            */       
    uint    ui_svlan_value;                 /* 外层VLAN的操作值       */ 

    /* pri 行为 */
    HI_DP_8021P_ACT_E   em_cpri_act;        /* 内层优先级的操作          */       
    uint    ui_cpri_value;                  /* 内层优先级的操作值        */

    HI_DP_8021P_ACT_E   em_s8021p_act;      /* 外层802.1p的操作          */        
    uint    ui_s8021p_value;                /* 外层802.1p的操作值        */

    /* 转发行为 */
    HI_DP_TRANS_ACT_E   em_trans_act ;      /* 报文转发行为,包括了NAT,NAPT转发指示 */

    hi_dp_port_u    u_trans_egrs;           /*指定转发,多端口*/  

    uint            ui_trans_reason;                /* 转发原因,主要用于捕获到CPU口的情况 */
    
    
    /* 指向L3模块的NAPT/NAT 加速表索引 */
    uint    ui_l3_rep_index;                /* NAT/ROUTE替换表索引,NAPT不需要,        */
                                            /* NAT / ROUTE 根据此索引查询L3的SUBNET表以确定替换操作
                                            而NAPT则直接查询SUBNET表以IP,PORT为匹配项 */
                                            /* SD5113V200扩展为8bit；0-255 */

    /* VLAN  mark (标记)行为 */
    HI_BOOL_E   em_vlan_mark_en;            /* VLAN marking使能           */    
    uint        ui_vlan_mark_value;             /* VLAN marking值             */
	

    /* PRI  mark 行为 */    
    HI_BOOL_E   em_pri_mark_en;             /* PRI marking使能            */    
    uint    ui_pri_mark_value;              /* PRI marking值              */
    HI_DP_PRI_MODE_E  em_pri_mode;           /* 基于流选择pri模式；配置值 = 枚举定义 + 1 */

    /* CAR 抑制指示 默认为TRUE */
    HI_BOOL_E   em_car_en;                  /* 流做CAR标识        CAR在QOS模块     */
    
    /* 流查找ARL TAG操作使能 */    
    HI_BOOL_E   em_arl_fid_en;              /* ARL FID动作使能         */  

    /*  仅在V100 路由、NAT/NAPT 时才能使用到pppoe_add_en标志，
          V200不再使用，为了兼容产品V100代码，暂时采用保留废弃的办法，
          后续会逐步删除 */
    HI_BOOL_E   em_pppoe_add_en;

    /* 基于流的MAC地址学习使能 */   
    HI_BOOL_E   em_mac_learn_en;            /* 基于流学习MAC使能           */

    /* TC/FL修改 */
    /*普通报文查tfrt_rep表,隧道报文查tunnel_tfrt_rep表*/
    HI_BOOL_E   em_tcfl_rep_en;            /* TC/FL 指定修改使能 */
    uint        ui_tfrt_index;             /* TFRT 查表索引*/

    /* 基于流做car, ECAR */
    HI_BOOL_E    em_ecar_en;                 /* ECAR使能 */
    uint         ui_ecar_id;              /* ECAR ID,ECAR查表索引 */


    HI_DP_TPID_REP_MODE_E    em_stpid_rep_mode;    /* 外层tpid修改模式 */
    HI_DP_TPID_REP_MODE_E    em_ctpid_rep_mode;    /* 内层tpid修改模式 */

    HI_DP_VLAN_MODE_E   em_trans_vlan_mode;    /* 基于流的转发vlan模式 */
    HI_DP_VLAN_MODE_E   em_map_vlan_mode;    /* 基于流的映射vlan模式 */

    uint         ui_color;         /* 流的颜色 */ 

    HI_BOOL_E    em_statis_en;        /* 流计数使能 */
    uint         ui_statis_id;        /* 流统计表索引 */
 
}hi_dp_flow_efc_act_s;


typedef struct 
{
    /* 索引 : 对应fmap表表项序号 */
    uint    ui_index;
    
    uint    ui_fid;                         /* 0 - 404 */

    HI_BOOL_E   em_valid;                   /* 表项有效标志 */   
    HI_BOOL_E   em_eof;                     /* 组合流结束标志符 */    

    /* 匹配规则 */
    hi_dp_flow_efc_rule_s   st_rule;        /* 匹配规则 */

    /* 分类操作指示 */
    HI_DP_FLOW_LABEL_E      em_label;       /* 分类标签，用于确定流操作类型   */
    uint                    ui_label_pri;  /* 表项优先级，仅在同一Label内有效 */

    /* TAG等分类操作行为 */    
    hi_dp_flow_efc_act_s    st_act;         /* 对应行为 */

}hi_dp_flow_efc_table_item_s;




/* 长表128bit */
typedef struct 
{
    /* 索引 */
    uint   ui_sfid;                                 /* 简单流ID, 索引,分类ID, 为后续MAP即CAR的输入ID, 0-127 */

    HI_BOOL_E    em_map_valid;                          /* sfid用于映射有效标志 */ 
    HI_BOOL_E    em_qos_valid;                          /* sfid用于qos 有效标志 */ 

    /* 基于流ID匹配规则 */
    HI_BOOL_E   em_fid_match_en;     
    uint        ui_fid;                                  /* 表示FID的匹配对应EFC ID */

    /* 基于VLAN匹配规则 */
    HI_BOOL_E                 em_vlan_match_en;     /* vlan有效标识 */
    HI_DP_MATCH_MODE_E   em_vlan_match_mode;   /* vlan匹配方式，如果FID有效则表示FID的匹配 */
    uint                      ui_vlan;              /* vlan值 */

    /* 基于PRI匹配规则 */
    HI_BOOL_E                 em_pri_match_en;      /* pri有效标识 */    
    HI_DP_MATCH_MODE_E   em_pri_match_mode;    
    uint                      ui_pri;               /* pri值 */

    /* 基于端口匹配规则 */
    hi_dp_port_u u_igr;

    HI_BOOL_E    em_eth_type_match_en;                   /* eth_type有效标识 */
    HI_DP_MATCH_MODE_E   em_eth_type_match_mode;    /* eth_type匹配方式 */       
    uint         ui_eth_type;                            /* eth_type号 */    

    uint     ui_entry_pri;         /* 表项优先级；用于多个表项同时匹配时，优先级高的有效 */

    /* 入口流CAR使能 */
    HI_BOOL_E   em_iprf_tp_en;

}hi_dp_flow_sfc_table_item_s;

/* 流统计表 */
typedef struct
{
    uint    ui_statis_id;

    
}hi_dp_flow_statis_item_s;


/* 
0x0：不提取特殊域参加EFC流分类。
0x1：MAC头
0x2：IP头
0x3：TCP/UDP头

 */
typedef enum
{
    HI_DP_FLOW_POINT_NO_FIELD = 0,
    HI_DP_FLOW_POINT_MAC_HEAD,              /* 指定MAC头 */
    HI_DP_FLOW_POINT_IP_HEAD,
    HI_DP_FLOW_POINT_TCP_UDP_HEAD,
}HI_DP_FLOW_POINT_FIELD_E;


typedef struct 
{
    HI_DP_FLOW_POINT_FIELD_E em_field_base; 
    uint ui_field_offset;
    hi_dp_mask_u   u_mask;      

}hi_dp_flow_point_field_s;



typedef struct 
{
    
    HI_BOOL_E   em_enable;          /* 指定统计使能 */
    uint      ui_fid;              /* 指定需要统计的FID */

}hi_dp_flow_fid_pcnt_s;




/* 
IRULE表分为4个子表，每个子表有2条流规则，共支持8个流规则
4个IRULE子表，8条流同时做匹配，规定1个子表最多只能由一条规则匹配，那么最多会有4条流同时匹配。则从中选取优先级最高的流标识作为最后的匹配结果。
根据子表间的优先级选择流标识，规定子表0的优先级最高，依次为子表1和子表2，子表3的优先级最低，多条流同时匹配时，以高优先级的为流标识。

 */
#define HI_DP_FC_TABLE_SIZE     3

#define HI_DP_FC_MATCH_BYTE_NUM 20

typedef struct 
{
    uint    ui_fcid;               /* 0~2,共支持3条流 */

    uint    ui_imask[HI_DP_FC_MATCH_BYTE_NUM];           /* 选择第0~63 字节, 最多可选择 20个 BYTE */  
    uint    ui_bitmask[HI_DP_FC_MATCH_BYTE_NUM];         /* 选择出的20个BYTE的 ui_bitmask, 5 * 4BYTE(32bit) */  

    /* 每个子表可以配置两条流规则, 每条规则的匹配域跟5 * 4BYTE 对应 */
    uint    ui_irule_0[HI_DP_FC_MATCH_BYTE_NUM];         /* 匹配规则1使能 */    
    HI_BOOL_E       em_irule_0_en;                       /* 匹配规则1使能 */   
    hi_dp_port_u u_0_igr;
    HI_DP_PORT_E    em_irule_0_igr;                      /* 接收报文的源端口匹配 */
    uint    ui_irule_1[HI_DP_FC_MATCH_BYTE_NUM];         /* 匹配规则1使能 */       
    HI_BOOL_E       em_irule_1_en;                       /* 匹配规则1使能 */        
    hi_dp_port_u u_1_igr;
    HI_DP_PORT_E    em_irule_1_igr;                      /* 接收报文的源端口匹配 */

    HI_BOOL_E       em_valid;                            /* 表项有效标志 */ 

}hi_dp_flow_fc_table_item_s;



typedef struct
{
    uint       ui_index;          /* 索引，0-15 */
    HI_BOOL_E  em_valid;        

    HI_BOOL_E  em_fl_rep_en;     /* 报文FL修改使能 */
    uint       ui_fl;             /*报文FL修改值 */

    HI_DP_TOS_MODE_E    em_tos_mode;      /* 报文TOS域选择 */
    HI_BOOL_E           em_tos_rep_en;    /* 报文TOS修改使能 */
    uint                ui_tos;           /* 报文TOS修改值 */
    
    
}hi_dp_flow_tcfl_rep_table_item_s;

typedef struct
{
    uint     ui_statis_id;

    ulong    ul_frame;     /*入口流统计时为匹配上的  包数，出口流统计时为入队成功的包数*/
    #ifdef CONFIG_HISI_SD5115_MP_MPW
    uint     ui_byte;
    #endif
    #ifdef CONFIG_HISI_SD5115_MP_PILOT   
    ulong    ul_byte;       /*出口流统计是为匹配上的字节数，出口流统计时为入队成功的字节数*/
    ulong    ul_drop_frame;/*出口流统计时丢弃的包数*/
    ulong    ul_drop_byte;  /*出口流统计时丢弃的字节数*/
    #endif
	
}hi_dp_flow_cnt_table_item_s;

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef struct 
{
  
    uint  ui_statis_id;
   
    HI_BOOL_E em_valid;
    
    HI_BOOL_E em_vlan_en;
    uint  ui_vlan;

    HI_BOOL_E em_egr_en;
    HI_DP_PORT_E em_egr;

    HI_BOOL_E em_gemport_en;   
    uint ui_gemport;
    
    HI_BOOL_E em_tcont_en;
    uint ui_tcont;

    HI_BOOL_E em_pri_en;
    uint ui_pri;

    HI_BOOL_E em_fwd_type_en;
    HI_DP_PKT_TYPE_E em_fwd_type;
    
}hi_dp_flow_cnt_match_table_item_s;
#endif

typedef struct 
{
    uint ui_trans_reason_drop;      /* 特定转发原因丢弃计数 */
    uint ui_trans_reason_thru;      /* 特定转发原因透传计数 */
    uint ui_trans_reason_tocpu;     /* 特定转发原因交CPU计数 */
    
}hi_dp_efc_trans_reason_cnt_s;

typedef struct 
{
    
    uint ui_efc_unhit;              /* 在EFC流分类中没有产生任何匹配报文个数统计                     */
    uint ui_efc_hit_act;            /* EFC报文匹配后执行ACTION（LABEL 为0～5）的报文个数             */
    uint ui_efc_hit_map;            /* EFC报文匹配，LABEL 值为GEM PORT 映射的LABEL 值的报文个数统计  */
    
    uint ui_efc_fid0;               /* EFC匹配指定FID0报文个数统计       */
    uint ui_efc_fid1;               /* EFC匹配指定FID1报文个数统计       */
    uint ui_efc_fid2;               /* EFC匹配指定FID2报文个数统计       */
    
    uint ui_efc_rmk_svlan;          /* EFC匹配指定SVLAN Remarking统计           */
    uint ui_efc_rmk_cvlan;          /* EFC匹配指定CVLAN Remarking统计           */
    uint ui_efc_rmk_spri;           /* EFC匹配指定SPriority Remarking统计       */
    uint ui_efc_rmk_cpri;           /* EFC匹配指定CPriority Remarking统计       */
    uint ui_efc_add_tag;            /* EFC报文匹配执行ACTION 为添加Tag 报文头的报文个数统计  */
    uint ui_efc_del_tag;            /* EFC报文匹配执行ACTION 为删除Tag 报文头的报文个数统计  */
    uint ui_pcnt_rmk_svlan;         /* 指定SVLAN Remarking 的报文数目统计，其SVLAN
                                       值由属性em_svlan_rmk_pcnt_en 指定，当该属性
                                       配置成使能状态时，并且进行SVLAN Remarking 操作
                                       的报文的修改后的SVLAN 等于属性ui_pcnt_svlan配置的值时，此统计有计数。 */
    uint ui_pcnt_rmk_cvlan;
    uint ui_pcnt_rmk_spri;
    uint ui_pcnt_rmk_cpri;
    
    uint ui_efc_tocpu;              /* EFC报文匹配执行ACTION 为TOCPU 的报文个数统计          */
    uint ui_efc_drop;               /* EFC报文匹配执行ACTION 为丢弃的报文个数统计            */
    uint ui_efc_indicate_fwd;       /* EFC报文匹配执行ACTION 为指定转发计数  */
    
    uint ui_efc_route;              /* EFC报文匹配执行ACTION 为Router/NAT 的报文个数统计     */
    uint ui_efc_nat;                /* EFC报文匹配执行ACTION 为Router/NAT 的报文个数统计     */
    uint ui_efc_napt;               /* EFC报文匹配执行ACTION 为NAPT 的报文个数统计           */
    uint ui_efc_add_pppoe;          /* EFC报文匹配执行ACTION 为添加PPPoE 报文头的报文个数    */
    
    uint ui_efc_multi_match;        /* EFC报文命中多条流规则的报文个数统计                   */

    uint ui_sfc_unhit;              /* SFC简单流分类（CLASSIFY）失败的报文个数 */
    uint ui_sfc_fail_drop;          /* 上行简单流分类失败丢包  */

    /* 特定转发原因计数 */
    hi_dp_efc_trans_reason_cnt_s st_tran_reason[HI_DP_EFC_TRAN_REASON_NUM];

#ifdef  CONFIG_HISI_SD5115_TYPE_T
    uint ui_efc_hit_pkt;              /* 在EFC流分类中匹配报文个数统计 */
#endif

    uint ui_rtp;                    /* 视频流统计 */

}hi_dp_flow_cnt_s;

typedef enum
{
    HI_DP_FLOW_RTP_CAP_E = 0,
    HI_DP_FLOW_RTP_EFC_E,    
    
}HI_DP_FLOW_RTP_SEL_E;


typedef struct 
{
  
    /* 8个自定义域, 用来参与匹配选择 */
     hi_dp_flow_point_field_s    st_point_field[HI_DP_FLOW_POINT_FIELD_NUM];


    HI_DP_FLOW_EFC_ACT_PRI_MODE_E   em_efc_cpri_sel;  /* CPRI 选择       */
    HI_DP_FLOW_LABEL_E              em_map_label;     /* 用作映射的label */
    HI_DP_FLOW_LABEL_E              em_car_label;     /* 用作car的label */

    /* 流捕获功能配置 */
    HI_DP_FLOW_RTP_SEL_E em_rtp_sel;  /* 视频捕获采用EFC\CAP方式 */
    HI_DP_L4_PROTOCOL_E em_rtp_protocol;    /* 流捕获协议 */
    uint ui_rtp_dip;
    uint ui_rtp_sip;
    uint ui_rtp_dport;   
    uint ui_rtp_sport;       
	
   #ifdef CONFIG_HISI_SD5115_MP_PILOT
   #ifndef CONFIG_HISI_SD5115_TYPE_T
   HI_DP_CNT_MODE_E em_igr_cnt_mode;
   #endif
   HI_DP_CNT_MODE_E em_egr_cnt_mode;
   #endif

}hi_dp_flow_attr_s;


typedef struct
{
    uint cnt[HI_DP_TOCPU_NUM_E];
} hi_dp_tocpu_reason_cnt_s;

typedef struct
{
    uint cnt[HI_DP_THRU_NUM_E];
} hi_dp_thru_reason_cnt_s;



#endif /* end of __HI_DRV_DP_FLOW_H__ */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


