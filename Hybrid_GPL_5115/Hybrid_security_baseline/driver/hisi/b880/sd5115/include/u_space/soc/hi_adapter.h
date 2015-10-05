/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_adapter.h
  版 本 号   : 初稿
  作    者   : t00185981
  生成日期   : D2012_02_09

******************************************************************************/
#ifndef __HI_ADAPTER_H__
#define __HI_ADAPTER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define HI_LSW_MAC_LEN 6
#define HI_LSW_IPV6_ADDR_BYTE_NUM 16



typedef enum
{
    HI_CHIP_UP_EPON_MODE_E = 0,
    HI_CHIP_UP_GPON_MODE_E,
    HI_CHIP_UP_GE_MODE_E,
    HI_CHIP_UP_UNKNOWN_MODE_E,
}hi_chip_up_mode_e;

typedef enum
{
    HI_LSW_UP_E = 0,
    HI_LSW_DOWN_E,
}hi_lsw_direction_e;

/*端口类型*/
typedef enum
{
    HI_LSW_UNI_PORT_E = 0,
    HI_LSW_NNI_PORT_E,
}hi_lsw_port_type_e;

/*字段*/
typedef enum
{
    HI_LSW_DMAC_E = 0,
    HI_LSW_SMAC_E,

    HI_LSW_DIP_E = 5,
    HI_LSW_SIP_E = 6,

    /*其他待扩展*/
}hi_lsw_field_e;

/* IP版本 */
typedef enum
{
    HI_LSW_IP_V4_E = 0,
    HI_LSW_IP_V6_E,

    HI_LSW_IP_VERSION,
}hi_lsw_ip_version_e;


/*端口号定义*/
typedef enum
{
    HI_LSW_PORT_UNI_ETH0_E   = 0,
    HI_LSW_PORT_UNI_ETH1_E   = 1,
    HI_LSW_PORT_UNI_ETH2_E   = 2,
    HI_LSW_PORT_UNI_ETH3_E   = 3,
    HI_LSW_PORT_UNI_ETH4_E   = 4,
    HI_LSW_PORT_NNI_PON_E    = 5,
    HI_LSW_PORT_UNI_CPU_E    = 6,
    HI_LSW_PORT_NNI_GE_E     = 8,
    HI_LSW_PORT_MAX_E,
}hi_lsw_port_e;

typedef union
{
    struct
    {
        uint eth0       : 1;
        uint eth1       : 1;
        uint eth2       : 1;
        uint eth3       : 1;
        uint eth4       : 1;
        uint pon        : 1;
        uint cpu        : 1;
        uint prbs       : 1;
        uint reserved   : 24;
    } bits;

    uint value;
} hi_lsw_port_bitmap_u;



/* 以太端口环回模式 */
typedef enum
{
    HI_LSW_PORT_ETH_NO_LOOP_E  = 0,   /* 正常模式 */
    HI_LSW_PORT_ETH_IN_LOOP_E,        /* 内环回模式 */
    HI_LSW_PORT_ETH_OUT_LOOP_E,       /* 外环回模式 */  
}hi_lsw_port_eth_loopback_e;

typedef enum
{
    HI_LSW_GLB_EXCHANG_MODE_E = 0,                  /*全交换模式*/
    HI_LSW_GLB_CONNECT_MODE_E,                      /*纯接入模式*/
    HI_LSW_GLB_CONNECT_EXCHANG_MODE_E,              /*接入交换模式*/
}hi_lsw_glb_sw_mode_e;

/* 统一一个转发行为枚举,用于FLOW, SPECIAL PKT, L2, L3等有转发配置的相关模块，与SDK的动作保存一致*/
typedef enum
{
    HI_LSW_TRANS_FWD_E = 0,
    HI_LSW_TRANS_DROP_E,
    HI_LSW_TRANS_TOCPU_E,
    HI_LSW_TRANS_TOCPU_AND_FWD_E,        
    HI_LSW_TRANS_TOPOINT_E,    
    HI_LSW_TRANS_THRU_E,

    HI_LSW_TRANS_ROUTE_NAT_E,
    HI_LSW_TRANS_NAPT_E,    

    HI_LSW_TRANS_TOCPU_AND_THRU_E,    
    HI_LSW_TRANS_BC_E,
    
    HI_LSW_TRANS_ROUTE_E,             /* L3路由转发 */    
    HI_LSW_TRANS_TOCPU_AND_ROUTE_E,    
    HI_LSW_TRANS_POLICY_ROUTE_E,      /* 策略路由，待确认拼写 */ 
    
    HI_LSW_TRANS_BC_BY_VLAN_E,     /* 仅在vlan内广播，不考虑vlan切换 */
    HI_LSW_TRANS_MC_LIST_E,             /* 组播链表转发；同5113 */
    HI_LSW_TRANS_MC_PORTMAP_E,          /* 组播端口转发；5115新增 */
}hi_lsw_trans_act_e;

/* MAC学习模式 */
typedef enum
{
    HI_LSW_MAC_LOGIC_LEARN_FROM_PORT_E = 0,   /* 逻辑基于端口学习模式;        */
    HI_LSW_MAC_LOGIC_LEARN_FROM_FLOW_E,       /* 逻辑基于流学习模式;        */
    HI_LSW_MAC_CPU_LEARN_FROM_PORT_E,         /* CPU软件基于端口学习模式;        */
    HI_LSW_MAC_CPU_LEARN_FROM_FLOW_E,         /* CPU软件基于流学习模式;        */    
}hi_lsw_mac_learn_mode_e;


/* 端口源MAC过滤模式 */
typedef enum
{
    HI_LSW_PORT_SMAC_IGNORE_E = 0,                   /* 不关心源MAC */
    HI_LSW_PORT_SMAC_ALLOW_E,                        /* 源MAC满足则允许通过 ,即白名单*/
    HI_LSW_PORT_SMAC_DROP_E,                         /* 源MAC满足则丢弃,即黑名单*/
    HI_LSW_PORT_SMAC_THRU_E,                           /* 源MAC满足则透传到上行口*/
    HI_LSW_PORT_SMAC_MAX_E,
}hi_lsw_port_blackmode_e;

typedef enum
{
    HI_LSW_DIAG_L2MAC_E = 0,
}hi_lsw_diag_mac_type_e;

typedef enum
{
    HI_LSW_DIAG_SEC_BACKMAC_E = 0,
    HI_LSW_DIAG_SEC_IP_FILTER_E,
}hi_lsw_diag_sec_type_e;

typedef enum
{
    HI_LSW_DIAG_EFC_E = 0,
    HI_LSW_DIAG_SFC_E,
}hi_lsw_diag_flow_type_e;

typedef enum
{
    HI_LSW_DIAG_VLAN_INFO_E = 0,       
}hi_lsw_diag_vlan_type_e;
typedef enum
{
    HI_LSW_GLB_ETH_NOSTP_E = 0,
    HI_LSW_GLB_ETH_STP,
    HI_LSW_GLB_ETH_RSTP,
}hi_eth_stp_mode_e;


/* 以太端口生成树状态 */
typedef enum
{
    HI_ETH_STP_FORWARD_E = 0,    /* 转发 */
    HI_ETH_STP_LEARN_E,          /* 学习 */
    HI_ETH_STP_LISTEN_E,         /* 侦听 */
    HI_ETH_STP_BLOCK_E,          /* 阻塞 */    
    HI_ETH_STP_CLOSE_E,          /* 关闭,丢弃 */       
}hi_eth_stp_status_e;

typedef enum
{
    HI_LSW_PHY_MODE_AUTO = 0,                         /*自协商模式*/

    /* 半双工(HDX = HALF_DUPLEX) */
    HI_LSW_PHY_MODE_HDX_10M_E,                        /*半双工10M模式*/
    HI_LSW_PHY_MODE_HDX_100M_E,                       /*半双工100M模式*/
    HI_LSW_PHY_MODE_HDX_1000M_E,                      /*半双工1000M模式*/

    /* 全双工(HDX = FULL_DUPLEX) */
    HI_LSW_PHY_MODE_FDX_10M_E,                        /*全半双工10M模式*/
    HI_LSW_PHY_MODE_FDX_100M_E,                       /*全半双工100M模式*/
    HI_LSW_PHY_MODE_FDX_1000M_E,                      /*全半双工1000M模式*/
    
}hi_lsw_phy_mode_e;

typedef enum 
{
    HI_LSW_IGR_TAG_ACT_E = 0,                                     /* 表示入口方向 */
    HI_LSW_EGR_TAG_ACT_E,                                     /* 表示出口方向*/
}hi_lsw_inout_e;

typedef enum 
{
    HI_LSW_TAG_ACT_E    = 0,                                     /* tag      动作 */
    HI_LSW_PRI_TAG_ACT_E,                                  /* pri tag 动作*/
    HI_LSW_UATAG_ACT_E,                                     /* untag  动作*/ 

}hi_lsw_tag_type_e;

typedef enum
{
    HI_LSW_TAG_NO_ACTION_E = 0,        
    HI_LSW_TAG_ADD_E,
    HI_LSW_TAG_DEL_E,    
    HI_LSW_TAG_DROP_E,
    HI_LSW_TAG_DEFAULT_DROP_E,
    HI_LSW_TAG_REP_OUTER_DEFAULT_E,     
    HI_LSW_TAG_REP_OUTER_TRANS_DROP_E, 
    HI_LSW_TAG_REP_OUTER_TRANS_TRUE_E,
    HI_LSW_TAG_CHECK_E,  
    HI_LSW_TAG_SWITCH_E, 
    HI_LSW_TAG_HYBRID_E,
    HI_LSW_TAG_ACT_NUM_E,
}hi_lsw_tag_act_e;

/* 下行GEMPORT 类型 */
typedef enum
{
    HI_LSW_GPON_DN_MC_PORT_E  = 0,       /* 用于承载组播以太帧（包括CESoP帧）的Port */
    HI_LSW_GPON_DN_OMCI_PORT_E ,         /* 用于承载OMCI信元的Port */
    HI_LSW_GPON_DN_TDM_PORT_E  ,         /* 用于承载TDMoGEM帧（即Native TDM帧）的Port */
    HI_LSW_GPON_DN_ETH_PORT_E,           /* 用于承载其他以太帧的Port */
    HI_LSW_GPON_DN_PRBS_PORT_E ,         /* 用于承载PRBS校验报文 */    
}hi_lsw_gpon_dn_port_type_e;

typedef enum
{
    HI_LSW_FLOW_UNTAG_E = 0,
    HI_LSW_FLOW_PRI_TAG_E,
    HI_LSW_FLOW_VLAN_TAG_E,
}hi_lsw_flow_tagtype_e;

typedef enum
{
    HI_LSW_FLOW_EQUAL_MODE_E = 0,               /* == */ 
    HI_LSW_FLOW_GREATER_THAN_AND_EQUAL_MODE_E,  /* >= */  
    HI_LSW_FLOW_LESS_THAN_AND_EQUAL_MODE_E,     /* <= */   
    HI_LSW_FLOW_GREATER_THAN_MODE_E,            /* >  */      
    HI_LSW_FLOW_LESS_THAN_MODE_E,               /* <  */    
    HI_LSW_FLOW_NOT_EQUAL_MODE_E,               /* != */
}hi_lsw_flow_match_mode_e;

typedef enum
{
    HI_LSW_SCAR_TYPE_UUC_E = 0,
    HI_LSW_SCAR_TYPE_TOCPU_E,
    HI_LSW_SCAR_TYPE_BC_E,
    HI_LSW_SCAR_TYPE_LOCAL_E,
}hi_lsw_simple_car_type_e;

typedef struct
{
    hi_uint32    ui_mask;

    hi_lsw_flow_match_mode_e       em_matchmode;
    
    hi_uchar8       uc_spri;
    hi_uchar8       uc_cpri;
    hi_uchar8       uc_iptos;
    hi_uchar8       uc_tagsun;

    hi_uchar8       uc_igr;
    hi_uchar8       uc_llid;
    hi_ushort16     us_gemportid;

    hi_uchar8   uc_dmac[HI_LSW_MAC_LEN];
    hi_uchar8   uc_dmask[HI_LSW_MAC_LEN];
    hi_uchar8   uc_smac[HI_LSW_MAC_LEN];
    hi_uchar8   uc_smask[HI_LSW_MAC_LEN];

    hi_uint32   ui_dip;
    hi_uint32   ui_dipmask;
    hi_uint32   ui_sip;
    hi_uint32   ui_sipmask;

    hi_ushort16 us_dport;
    hi_ushort16 us_dpmask;  /*mask何用???*/
    hi_ushort16 us_sport;
    hi_ushort16 us_spmask;

    hi_ushort16 us_ethtype;
    hi_ushort16 us_sessionid; /*ppp会话id*/

    hi_ushort16 us_stpid;
    hi_ushort16 us_ctpid;
    
    hi_ushort16 us_svlan;
    hi_ushort16 us_cvlan;
    
    hi_uchar8   uc_reason;  /*何用???*/
    hi_uchar8   uc_ipprotocol;
    hi_uchar8   uc_ipversion;
    hi_uchar8   uc_tagtype;
}hi_lsw_flow_fmap_info_s;

typedef struct
{
    hi_uint32     ui_mask;
    hi_lsw_port_e em_outport;
    hi_uchar8     uc_pppoe;
    hi_uchar8     uc_cpri;
    hi_uchar8     uc_spri;
    hi_uchar8     uc_markpri;
    hi_ushort16   us_cvlan;
    hi_ushort16   us_svlan;
    hi_ushort16   us_markvlan;
    hi_ushort16   us_resv;
}hi_lsw_flow_fact_info_s;

typedef struct
{
    hi_uint32                   ui_mask;
    hi_lsw_flow_match_mode_e    em_matchmode;
    hi_lsw_port_bitmap_u        em_port;
    hi_ushort16                 us_vlan;
    hi_ushort16                 us_fid;
    hi_uchar8                   uc_pri;
    hi_uchar8                   uc_resv1;
    hi_ushort16                 us_ethtype;
}hi_lsw_flow_mapping_s;


typedef struct
{
    hi_lsw_port_type_e  em_porttype;
    hi_lsw_field_e      em_field;
    hi_lsw_ip_version_e em_ipversion;
    hi_uchar8           uc_mask;        //CIDR记法中的前缀长度
    hi_uchar8           uc_resv;
    hi_ushort16         us_resv;
    hi_uchar8           uc_ip[HI_LSW_IPV6_ADDR_BYTE_NUM];
}hi_lsw_sec_ip_filter_s;

typedef struct
{
    hi_ushort16 us_upvlansel;
    hi_ushort16 us_dnvlansel;
    hi_ushort16 us_upprisel;
    hi_ushort16 us_dnprisel;
}hi_lsw_mapping_value_sel_s;

typedef struct
{
    hi_uint32 ui_cbs;
    hi_uint32 ui_cir;
    hi_uint32 ui_pbs;
    hi_uint32 ui_pir;
}hi_lsw_car_info_s;

/* 队列出口 */
typedef enum {
    HI_LSW_QOS_EGR_TCONT0_E  = 0,
    HI_LSW_QOS_EGR_TCONT1_E,
    HI_LSW_QOS_EGR_TCONT2_E,
    HI_LSW_QOS_EGR_TCONT3_E,
    HI_LSW_QOS_EGR_TCONT4_E,
    HI_LSW_QOS_EGR_TCONT5_E,
    HI_LSW_QOS_EGR_TCONT6_E,
    HI_LSW_QOS_EGR_TCONT7_E, 
    
    HI_LSW_QOS_EGR_GE0_E,
    HI_LSW_QOS_EGR_GE1_E,
    HI_LSW_QOS_EGR_FE2_E,
    HI_LSW_QOS_EGR_FE3_E,
    HI_LSW_QOS_EGR_FE4_E,    
    
    HI_LSW_QOS_EGR_CPU_E,        
}hi_lsw_qos_egr_e;

/* 队列调度模式 */
typedef enum {
    HI_LSW_QOS_SCH_SP_MODE_E = 0,        /* SP调度模式 */
    HI_LSW_QOS_SCH_WRR_MODE_E,           /* WRR调度模式 */
}hi_lsw_qos_sch_mode_e;

/* 特殊报文类型 */
typedef enum{
    HI_LSW_SPECIAL_PKT_PPPOE_DIS_E = 0,   /* 0x0: PPPoE Discovery */
    HI_LSW_SPECIAL_PKT_DHCPV6_E,          /* 0x1: DHCPV6 */
    HI_LSW_SPECIAL_PKT_ICMPV6_RS_E,       /* 0x2: ICMPv6 Router Soliciation */
    HI_LSW_SPECIAL_PKT_ICMPV6_RA_E,       /* 0x3: ICMPv6 Router Advertisement */
    HI_LSW_SPECIAL_PKT_ICMPV6_NS_E,       /* 0x4: ICMPv6 Nerghbor Soliciation */ 
    HI_LSW_SPECIAL_PKT_ICMPV6_NA_E,       /* 0x5: ICMPv6 Nerghbor Advertisement */ 
    HI_LSW_SPECIAL_PKT_ICMPV6_RM_E,       /* 0x6: ICMPv6 Redirect Message */ 
    HI_LSW_SPECIAL_PKT_ICMPV6_PING_REQ_E, /* 0x7: ICMPv6 Echo Equest */ 
    HI_LSW_SPECIAL_PKT_ICMPV6_PING_REPLY_E, /* 0x8: ICMPv6 Echo Reply */
    HI_LSW_SPECIAL_PKT_ICMPV6_UNREACH_E,    /* 0x9: ICMPv6 Destination Unreachable */
    HI_LSW_SPECIAL_PKT_ICMPV6_GB_E,         /* 0xA: ICMPv6 Packet Too Long */ /* 分组太大差错报文 */
    HI_LSW_SPECIAL_PKT_ICMPV6_OVER_TIME_E,  /* 0xB: ICMPv6 Time Exceeded */   /* 超时差错报文 */
    HI_LSW_SPECIAL_PKT_ICMPV6_PARA_ERR_E,   /* 0xC: ICMPv6 Parameter Problem */ /* 参数错误报文 */
    HI_LSW_SPECIAL_PKT_MLD_QUERY_V1_V2_IPOE_E,    /* 0xD: MLD Query over IPoE */
    HI_LSW_SPECIAL_PKT_MLD_REPORT_V1_IPOE_E,      /* 0xE: MLDv1 Report over IPoE */
    HI_LSW_SPECIAL_PKT_MLD_REPORT_V2_IPOE_E,      /* 0xF: MLDv2 Report over IPoE */
    HI_LSW_SPECIAL_PKT_MLD_DONE_IPOE_E,           /* 0x10: MLD Done over IPoE */
    HI_LSW_SPECIAL_PKT_MLD_QUERY_V1_V2_PPPOE_E,   /* 0x11: MLD Query over PPPoE */
    HI_LSW_SPECIAL_PKT_MLD_REPORT_V1_PPPOE_E,     /* 0x12: MLDv1 Report over PPPoE */
    HI_LSW_SPECIAL_PKT_MLD_REPORT_V2_PPPOE_E,     /* 0x13: MLDv2 Report over PPPoE */
    HI_LSW_SPECIAL_PKT_MLD_DONE_PPPOE_E,           /* 0x14: MLD Done over PPPoE */    
    HI_LSW_SPECIAL_PKT_ARP_E,    /* 0x15: ARP */         
    HI_LSW_SPECIAL_PKT_DHCP_E,   /* 0x16: DHCP */ 
    HI_LSW_SPECIAL_PKT_ICMP_E,   /* 0x17: ICMP */
    HI_LSW_SPECIAL_PKT_IGMP_PPPOE_E,   /* 0x18: IGMP over PPPoE */    
    HI_LSW_SPECIAL_PKT_IGMP_IPOE_E,    /* 0x19: IGMP over IPoE */    
    HI_LSW_SPECIAL_PKT_RARP_E,         /* 0x1A: RARP */
    HI_LSW_SPECIAL_PKT_PPPOE_LCP,      /* 0x1B: PPP LCP */
    HI_LSW_SPECIAL_PKT_PPPOE_IPCP,     /* 0x1C: PPP IPCP */ 
    HI_LSW_SPECIAL_PKT_PPPOE_CHAP,     /* 0x1D: PPP CHAP */ 
    HI_LSW_SPECIAL_PKT_PPPOE_PAP,      /* 0x1E: PPP PAP */ 
    HI_LSW_SPECIAL_PKT_PPPOE_IPV6CP,   /* 0x1F: PPP IPv6CP */ 

    /* 以下无优先级仲裁 */
    HI_LSW_SPECIAL_PKT_POINT_TYPE0_E,  /* 4种自定义ETH Type */
    HI_LSW_SPECIAL_PKT_POINT_TYPE1_E,
    HI_LSW_SPECIAL_PKT_POINT_TYPE2_E,
    HI_LSW_SPECIAL_PKT_POINT_TYPE3_E,

    HI_LSW_SPECIAL_PKT_PPPOE_POINT_TYPE_0_E,   /* 3种用户自定义协议类型；PPPOE_UD_ULPT12、3配置 */
    HI_LSW_SPECIAL_PKT_PPPOE_POINT_TYPE_1_E,
    HI_LSW_SPECIAL_PKT_PPPOE_POINT_TYPE_2_E,

    HI_LSW_SPECIAL_PKT_IPV6_EXTERN_HEADER_E,               /* 带扩展头的IPv6报文 */
    HI_LSW_SPECIAL_PKT_BPDU_E,               
    HI_LSW_SPECIAL_PKT_EAPOL_E,               

    
    HI_LSW_SPECIAL_PKT_NUM,
}hi_lsw_special_pkt_e;

typedef enum
{
    HI_LSW_TOCPU_REASON_L2_WLAN_E = 0,         /* 0： L2转发到非网关接口（WLAN） */
    HI_LSW_TOCPU_REASON_EFC_MATCH0_E,           /*1: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH1_E,           /*2: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH2_E,           /*3: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH3_E,           /*4: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH4_E,           /*5: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH5_E,           /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH6_E,           /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH7_E,           /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH8_E,           /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH9_E,           /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH10_E,          /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH11_E,          /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH12_E,          /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH13_E,          /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH14_E,          /* 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_EFC_MATCH15_E,          /*16: 自定义EFC匹配交CPU原因            */
    HI_LSW_TOCPU_REASON_RTP_CAPTURE_E,          /*17: RTP流捕获            */

    //    HI_LSW_TOCPU_REASON_UNMATCH_NAPT_E ,         /* 17: 为NAPT进行EFC匹配失败             */
    //    HI_LSW_TOCPU_REASON_UNTAG_E ,                /* UNTAG报文                         */
    //    HI_LSW_TOCPU_REASON_VLAN_TAG_E ,             /* VLAN TAG报文                      */
    //    HI_LSW_TOCPU_REASON_PRI_TAG_E ,              /* PRI UNTAG报文                     */
    //    HI_LSW_TOCPU_REASON_BC_E ,                   /* 广播报文                          */
    HI_LSW_TOCPU_REASON_IGMP_PPPOE_E = 22,      /*22: pppoe承载IGMP报文                          */
    HI_LSW_TOCPU_REASON_IGMP_IPOE_E,            /*23: ipoe承载IGMP报文                          */
    HI_LSW_TOCPU_REASON_ARP_E,                  /*24: ARP报文                           */
    HI_LSW_TOCPU_REASON_RARP_E,                 /*25: RARP报文                          */
    HI_LSW_TOCPU_REASON_PPPOE_DIS_E,            /*26: PPPOE DISCOVERY报文               */
    HI_LSW_TOCPU_REASON_EAPOL_E,                /*27: EAPOL报文                         */
    HI_LSW_TOCPU_REASON_BPDU_E,                 /*28: BPDU报文                          */
    HI_LSW_TOCPU_REASON_DHCP_E,                 /*29: DHCP报文                          */

    //    HI_LSW_TOCPU_REASON_IPV6_E ,                 /* IPV6报文                          */
    HI_LSW_TOCPU_REASON_ICMP_E = 31,            /*31: ICMP报文                          */
    HI_LSW_TOCPU_REASON_POINT_TYPE0_E,          /*32: 自定义捕获的报文0                  */
    HI_LSW_TOCPU_REASON_POINT_TYPE1_E,          /*33: 自定义捕获的报文1                  */
    HI_LSW_TOCPU_REASON_POINT_TYPE2_E,          /*34: 自定义捕获的报文2                  */
    HI_LSW_TOCPU_REASON_POINT_TYPE3_E,          /*35: 自定义捕获的报文3                  */

    //    HI_LSW_TOCPU_REASON_PPPOE_SES_E ,            /* 36: PPPOE SESSION报文                 */
    HI_LSW_TOCPU_REASON_PORT_CHANGE_E = 37,      /* 37: 静态表项的端口发生移动            */
    HI_LSW_TOCPU_REASON_MAC_LEARN_E,            /* 38: 报文提交给CPU学习                 */
    HI_LSW_TOCPU_REASON_MAC_LEARN_FAIL_E,       /* 39: 逻辑学习报文失败                  */
    HI_LSW_TOCPU_REASON_SEARCH_FAIL_E,          /* 40: 逻辑查表失败                      */
    /*BEGIN: 问题单号DTS2012032703169，by lKF55238,2012/4/18*/
    HI_LSW_TOCPU_REASON_TCP_CTRL_E,           /*41:TCP    控制报文交CPU*/
    /*END: 问题单号DTS2012032703169，by lKF55238,2012/4/18*/
    HI_LSW_TOCPU_REASON_NAPT_ON_E = 42,         /* 42: NAPT加速开启时首包提交            */
    HI_LSW_TOCPU_REASON_NAPT_IP_OPTION_E,       /* 43: IP Option报文做NAPT            */

    //    HI_LSW_TOCPU_REASON_UNMATCH_NAPT_OUTNET_E ,  /* 44: NAPT的OUTNET表不匹配              */
    HI_LSW_TOCPU_REASON_1AG_Y1731_E = 45,       /* 45: 802.1ag / Y.1731报文              */
    HI_LSW_TOCPU_REASON_3AH_E,                  /* 46: 802.3ah报文                       */
#if 0
    /* V200, 47 - 53为保留 */
    HI_LSW_TOCPU_REASON_PDELAY_REQ_E,           /* 47: Pdelay_req报文捕获交CPU            */
    HI_LSW_TOCPU_REASON_PDELAY_RESP_E,          /* 48: Pdelay_resp报文捕获交CPU           */
    HI_LSW_TOCPU_REASON_PDELAY_RESP_FOLLOW_UP_E,/* 49: Pdelay_resp_follow_up报文捕获交CPU */
    HI_LSW_TOCPU_REASON_MANAGEMENT_E,           /* 50: Management报文交CPU                */
    HI_LSW_TOCPU_REASON_SIGNALING_E,            /* 51: Signaling报文交CPU                 */
    HI_LSW_TOCPU_REASON_ANNOUNCE_E,             /* 52: Announce报文交CPU                  */
    HI_LSW_TOCPU_REASON_OTHER_1588_E,           /* 53: 其他1588协议报文交CPU              */

    HI_LSW_TOCPU_REASON_NAPT_DN_FAIL_E,         /* 54: NAPT下行查表失败交CPU              */
#endif
    HI_LSW_TOCPU_REASON_CCM_LMM_0_E,             /*47: LM测试，捕获CCM/LMM报文，计数0 */
    HI_LSW_TOCPU_REASON_CCM_LMM_1_E,             /*48: LM测试，捕获CCM/LMM报文，计数1 */
    HI_LSW_TOCPU_REASON_CCM_LMM_2_E,             /*49: LM测试，捕获CCM/LMM报文，计数2 */
    HI_LSW_TOCPU_REASON_CCM_LMM_3_E,             /*50: LM测试，捕获CCM/LMM报文，计数3 */
    HI_LSW_TOCPU_REASON_CCM_LMM_4_E,             /*51: LM测试，捕获CCM/LMM报文，计数4 */
    HI_LSW_TOCPU_REASON_CCM_LMM_5_E,             /*52: LM测试，捕获CCM/LMM报文，计数5 */
    HI_LSW_TOCPU_REASON_CCM_LMM_6_E,             /*53: LM测试，捕获CCM/LMM报文，计数6 */
    HI_LSW_TOCPU_REASON_CCM_LMM_7_E,             /*54: LM测试，捕获CCM/LMM报文，计数7 */
    HI_LSW_TOCPU_REASON_IP_FRAGMENT_NAPT_E,      /* 55．	IP分片报文做NAPT交CPU */
    HI_LSW_TOCPU_REASON_NAPT_DN_FAILE_E,         /* 56．	NAPT下行查表失败交CPU */
    HI_LSW_TOCPU_REASON_MC_GEMPORT_IPV4_UMC_E,    /* 57: 组播GEMPORT承载IPv4未知组播报文交CPU */
    HI_LSW_TOCPU_REASON_TUNNEL_UNKNOWN_MC_E,        /* 58: 隧道未知组播报文交CPU */
    HI_LSW_TOCPU_REASON_MC_GEMPORT_IPV6_UMC_E,     /* 59: 组播GEMPORT承载IPv6未知组播报文交CPU */
    HI_LSW_TOCPU_REASON_UC_BC_GEMPORT_IPV4_UMC_E, /* 60: 非组播GEMPORT承载IPv4未知组播报文交CPU */
    HI_LSW_TOCPU_REASON_UC_BC_GEMPORT_IPV6_UMC_E, /* 61: 非组播GEMPORT承载IPv6未知组播报文交CPU */

    HI_LSW_TOCPU_REASON_BC_IGR_MIRROR_E,    /* 62: 广播源端口镜像转发 */
    HI_LSW_TOCPU_REASON_MC_IGR_MIRROR_E,     /* 63: 组播源端口镜像转发 */
    HI_LSW_TOCPU_REASON_NAPT_EGR_IS_CPU_E,  /* 64．	NAPT加速到用户口（比如WLAN） */
    HI_LSW_TOCPU_REASON_RT_TO_GW_IF_E,              /* 65．	路由到网关接口 */
    HI_LSW_TOCPU_REASON_L2_FWD_TO_GW_IF_E,          /* 66．	L2转发到网关接口 */
    HI_LSW_TOCPU_REASON_EFC_POINT_TO_WLAN_E,        /* 67：EFC指定转发到WLAN */
    HI_LSW_TOCPU_REASON_UNKNOWN_EXTERN_HEADER_E,    /* 68：IPV6非知名扩展头 */
    HI_LSW_TOCPU_REASON_IPV6_SIP_SITE_LOCAL_E,      /* 69：IPv6源IP为Site Local  */
    HI_LSW_TOCPU_REASON_IPV6_SIP_0_E,               /* 70：IPV6 SIP等于0 */
    HI_LSW_TOCPU_REASON_IPV6_SIP_LINK_LOCAL_E,      /* 71：IPv6源IP为Link Local  */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_LCP_E,        /* 72：PPPOE  Session阶段的LCP报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_NCP_E,        /* 73：PPPOE  Session阶段的NCP报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_CHAP_E,       /* 74：PPPOE  Session阶段的CHAP报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_PAP_E,        /* 75：PPPOE  Session阶段的PAP报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_IPV6CP_E,     /* 76：PPPOE  Session阶段的IPV6CP报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_POINT_0_E,    /* 77：PPPOE  Session阶段的用户自定义类型0报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_POINT_1_E,    /* 78：PPPOE  Session阶段的用户自定义类型1报文 */
    HI_LSW_TOCPU_REASON_PPPOE_SESSION_POINT_2_E,    /* 79：PPPOE  Session阶段的用户自定义类型2报文 */
    HI_LSW_TOCPU_REASON_DHCPV6_E,                   /* 80：DHCPV6报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_RS_E,                /* 81：ICMPV6 RS报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_RA_E,                /* 82：ICMPV6 RA报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_NS_E,                /* 83：ICMPV6 NS报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_NA_E,                /* 84：ICMPV6 NA报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_RM_E,                /* 85：ICMPV6 RM报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_PING_REQ_E,          /* 86：ICMPV6 PingReq报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_PING_REPLY_E,        /* 87：ICMPV6 PingREPLY报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_UNREACH_E,           /* 88：ICMPv6目的不可达差错报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_GROUP_BIG_E,         /* 89：ICMPv6分组太大报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_OUTTIME_E,           /* 90：ICMPv6超时差错报文 */
    HI_LSW_TOCPU_REASON_ICMPV6_PAMAM_ERROE_E,       /* 91：ICMPv6参数错误报文 */
    HI_LSW_TOCPU_REASON_IPOE_MLD_QUERY_V1_V2_E,     /*92：IPOE承载的MLD Query v1/v2报文 */
    HI_LSW_TOCPU_REASON_IPOE_MLD_REPORT_V1_E,       /* 93：IP0E承载的MLD Report v1报文 */
    HI_LSW_TOCPU_REASON_IPOE_MLD_REPORT_V2_E,       /* 94：IPOE承载的MLD Report v2报文 */
    HI_LSW_TOCPU_REASON_IPOE_MLD_DONE_E,            /* 95：IPOE承载的MLD DONE报文 */
    HI_LSW_TOCPU_REASON_PPPOE_MLD_QUERY_V1_V2_E,    /* 96：PPPPOE承载的MLD Query v1/v2报文 */
    HI_LSW_TOCPU_REASON_PPPOE_MLD_REPORT_V1_E,      /* 97：PPPOE承载的MLD Report v1报文 */
    HI_LSW_TOCPU_REASON_PPPOE_MLD_REPORT_V2_E,      /* 98：PPPOE承载的MLD Report v2报文 */
    HI_LSW_TOCPU_REASON_PPPOE_MLD_DONE_E,            /* 99：PPPOE承载的MLD DONE报文 */
    HI_LSW_TOCPU_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,  /* 100：超过1个扩展头的IPV6报文 */
    HI_LSW_TOCPU_REASON_HEADER_LAREGR_128_BYTE_E,    /* 101：报文头长度超过128B */
    HI_LSW_TOCPU_REASON_RT_FWD_USR_E,                /* 102：路由转发到cpu用户口 */
    HI_LSW_TOCPU_REASON_HL_TTL_LESS_1_E,             /* 103：HOP_LIMIT/TTL小于等于1捕获报文 */
    HI_LSW_TOCPU_REASON_RT_FAIL_E,                   /* 104：默认路由禁止时路由失败 */
    HI_LSW_TOCPU_REASON_REFUSE_HOLE_RT_E,            /* 105：拒绝路由 */
    HI_LSW_TOCPU_REASON_NO_NEXT_HOP_E,               /* 106：无下一跳 */
    HI_LSW_TOCPU_REASON_IN_TUNNEL_MTU_FAIL_E,        /* 107：入隧道MTU检查失败 */
    HI_LSW_TOCPU_REASON_BC_BC_E,                     /* 108：广播报文广播到CPU  */
    HI_LSW_TOCPU_REASON_UC_E,                        /* 109：未知单播（广播到CPU或转交CPU） */
    HI_LSW_TOCPU_REASON_MC_FWD_TOCPU_E,              /* 110：已知组播报文转发到CPU用户口 */
    HI_LSW_TOCPU_REASON_SIPV6_LOOP_E,                /* 111：SIPv6等于环回地址 */
    HI_LSW_TOCPU_REASON_SIPV6_MC_E,                  /* 112：SIPv6等于组播地址 */
    HI_LSW_TOCPU_REASON_SIPV4_0_E,                   /* 113：SIPv4等于0 */
    HI_LSW_TOCPU_REASON_SIPV4_LOOP_E,                /* 114：SIPv4等于环回地址 */
    HI_LSW_TOCPU_REASON_SIPV4_MC_E,                  /* 115：SIPv4等于组播地址 */
    HI_LSW_TOCPU_REASON_TOCPU_AND_ROUTE_E,           /* 116：路由+拷贝CPU */
    HI_LSW_TOCPU_REASON_TOCPU_AND_NAPT_E,            /* 117：NAPT+拷贝CPU */
    HI_LSW_TOCPU_REASON_SNAP_LENGTH_OVER_1500_E,     /* 118：SNAP长度超过1500 */
    HI_LSW_TOCPU_REASON_DS_LITE_OUT_E,               /* 119: 带隧道头的本地DS_LITE报文出隧道 */
    HI_LSW_TOCPU_REASON_BC_VLAN_E,                   /* 120：广播报文vlan用户广播到CPU  */
    HI_LSW_TOCPU_REASON_MC_VLAN_E,                  /* 121：已知组播报文VLAN转发到CPU用户口 */

    /* BEGIN: Added by shengjing, 2012/1/10   PN:DTS2012011002267 */
    HI_LSW_TOCPU_REASON_FRAG_6RD_E,                /* 122: 6RD分片报文交cpu */
    HI_LSW_TOCPU_REASON_DROP_REDIRECTION_E,                /* 123: 丢弃报文重定向交cpu */
    /* END:   Added by shengjing, 2012/1/10 */
    HI_LSW_TOCPU_REASON_6RD_FRAG_E = 124,                  /* 124：6rd分片报文出隧道 */
    HI_LSW_TOCPU_REASON_6RD_IPV4_OPTION_E,                  /* 125：6rd带IPv4 option报文出隧道 */
    HI_LSW_TOCPU_REASON_EGR_MIRROR_E,                  /* 126:出端口镜像 */
    HI_LSW_TOCPU_REASON_IGR_MIRROR_E,                  /* 127:入端口镜像 */

    HI_LSW_TOCPU_NUM_E,
} hi_lsw_tocpu_reason_e;

typedef struct
{
    hi_ulong64    ul_tx_total;        /* CPU口发包 */
    hi_ulong64    ul_rx_total;        /* CPU口收包 */
    
    hi_ulong64   ul_rx_fcs_err;                  /* 1:FCS errors                         */
    hi_ulong64   ul_tx_exc_col_err;              /* 1:Excessive collision counter        */
    hi_ulong64   ul_tx_late_col_err;             /* 1:Late collision counter             */
    hi_ulong64   ul_rx_frm_too_long_err;         /* 1:Frames too long                    */
    hi_ulong64   ul_rx_overflow_err;             /* 1:Buffer overflows on receive        */
    hi_ulong64   ul_tx_overflow_err;             /* 1:Buffer overflows on transmit  SD5113中不会出现这种情况,统计始终为0 */
    hi_ulong64   ul_tx_single_col_err;           /* 1:Single collision frame counter     */
    hi_ulong64   ul_tx_multi_col_err;            /* 1:Multiple collision frame counter   */
    hi_ulong64   ul_sqe_err;                     /* 1:SQE counter                        */
    hi_ulong64   ul_tx_defferred_err;            /* 1:Deferred transmission counter      */
    hi_ulong64   ul_tx_intervel_mac_err;         /* 1:Internal MAC transmit error counter */
    hi_ulong64   ul_tx_crs_drop_err;             /* 1:Carrier sence error counter        */
    hi_ulong64   ul_rx_align_err;                /* 1:Alignment error counter            */
    hi_ulong64   ul_rx_interval_mac_err;         /* 1:Internal MAC receive error counter */
    hi_ulong64   ul_rx_pppoe_filter_frame;       /* 2:PPPoE filtered frame counter       */
    hi_ulong64   ul_rx_drop_events;              /* 3:Drop events                        */
    hi_ulong64   ul_rx_octets;                   /* 3:Octets                             */
    hi_ulong64   ul_rx_pkts;                     /* 3:Packets                            */
    hi_ulong64   ul_rx_undersize_pkts;           /* 3:Undersize packets                  */
    hi_ulong64   ul_rx_fragments;                /* 3:Fragments                          */
    hi_ulong64   ul_rx_jabbers;                  /* 3:Jabbers                            */
    hi_ulong64   ul_rx_pkts_64_octets;           /* 3:Packets 64 octets                  */
    hi_ulong64   ul_rx_pkts_65to127_octets;      /* 3:Packets 65 to 127 octets           */
    hi_ulong64   ul_rx_pkts_128to255_octets;     /* 3:Packets 128 to 255 octets          */
    hi_ulong64   ul_rx_pkts_256to511_octets;     /* 3:Packets 256 to 511 octets          */
    hi_ulong64   ul_rx_pkts_512to1023_octets;    /* 3:Packets 512 to 1023 octets         */
    hi_ulong64   ul_rx_pkts_1024to1518_octets;   /* 3:Packets 1024 to 1518 octets        */ 
    hi_ulong64   ul_rx_pkts_1519tomtu_octets;   
    hi_ulong64   ul_tx_pkts_64_octets;           
    hi_ulong64   ul_tx_pkts_65to127_octets;      
    hi_ulong64   ul_tx_pkts_128to255_octets;     
    hi_ulong64   ul_tx_pkts_256to511_octets;     
    hi_ulong64   ul_tx_pkts_512to1023_octets;    
    hi_ulong64   ul_tx_pkts_1024to1518_octets;   
    hi_ulong64   ul_tx_pkts_1519tomtu_octets;   
    hi_ulong64   ul_rx_uc_pkts;                  /* 4:EtherReceiveUnicastPkts            */
    hi_ulong64   ul_tx_uc_pkts;                  /* 4:EtherSendUnicastPkts               */
    hi_ulong64   ul_rx_bc_pkts;                  /* 3:Broadcast packets                  */
    hi_ulong64   ul_tx_bc_pkts;                  /* 4:EtherSendBroadcastPkts             */
    hi_ulong64   ul_rx_mc_pkts;                  /* 3:Multicast packets                  */
    hi_ulong64   ul_tx_mc_pkts;                  /* 4:EtherSendMulticastPkts             */
    hi_ulong64   ul_rx_pause_frame;              /* 4:EtherReceivePauseFlowCtrlFrame     */
    hi_ulong64   ul_tx_pause_frame;              /* 4:EtherSendPauseFlowCtrlFrame        */    
    hi_ulong64   ul_rx_good_pkts_octets;         /* 4:EtherReceiveGoodPktsOctets         */
    hi_ulong64   ul_tx_good_pkts_octets;         /* 4:EtherSendGoodPktsOctets            */
    hi_ulong64   ul_rx_bad_pkts_octets;          /* 4:EtherReceiveBadPktsOctets          */
    hi_ulong64   ul_tx_bad_pkts_octets;          /* 4:12EtherSendBadPktsOctets           */
    hi_uint32      ui_thru;                        /* 端口透传计数 */
}hi_lsw_eth_cnt_s;


typedef struct
{
    hi_uchar8   uc_valid;
    hi_uchar8   uc_ageen;
    hi_ushort16 us_vlan;
    hi_uchar8   uc_mac[6];
    hi_uchar8   uc_untag;
    hi_uchar8   uc_resv;
    hi_lsw_port_e   em_egr;
}hi_lsw_mac_item_s;

typedef enum {
    HI_LSW_QOS_CAR_MODE_P_FIRST  = 0, /*报文先过P桶*/
    HI_LSW_QOS_CAR_MODE_C_FIRST,    /*报文先过C桶*/
}hi_lsw_qos_car_mode_e;


typedef enum
{
    HI_LSW_QOS_SHP_BPS_E,
    HI_LSW_QOS_SHP_PPS_E
}hi_lsw_qos_shp_type_e;


typedef struct
{
    hi_uint32   ui_cir;                 /* 承诺速率        */
    hi_uint32   ui_cbs;                 /* 承诺突发尺寸    */
    hi_uint32   ui_pir;                 /* 峰值速率        */
    hi_uint32   ui_pbs;                 /* 峰值突发尺寸    */
    hi_uchar8   uc_token_overflow_en; /*C桶令牌溢出到P桶使能。0：不使能；1：使能。*/
    hi_uchar8   uc_resv;
    hi_ushort16 us_resv;
    hi_lsw_qos_car_mode_e em_car_mode;            /*CAR模式配置 0：报文先过P桶；1报文先过C桶。*/    
}hi_lsw_flow_car_attr_s;

/* 流队列WRED模板颜色标记类型 */
typedef enum
{
    HI_WRED_TEMP_NOT_SUPPORT_E,  /* 000：不支持WRED */
    HI_LSW_QOS_WRED_TEMP_INTERNAL_E,     /* 001：内部标记(根据EFC标记的流颜色 映射color 0 ~ 7 ) */
    HI_LSW_QOS_WRED_TEMP_DEI_E,          /* 010：DEI标记 (根据DEI 映射color 0 ~ 1 )*/
    HI_LSW_QOS_WRED_TEMP_PCP_E,          /* 011：PCP标记 (根据TAG 的 PBIT 映射color 0 ~ 7 )*/
    HI_LSW_QOS_WRED_TEMP_DSCP_AF,        /* 100：DSCP AF标记 根据AF CLASS映射 color 0 ~ 11*/
    HI_LSW_QOS_WRED_TEMP_DSCP_TC,        /* 101：DSCP/TC标记 (根据DSCP或TC的高3bit映射color 0 ~ 7)*/
    HI_LSW_QOS_WRED_TEMP_CAR,            /* 110：CAR标记 (仅对黄色进行WRED.仅对应 color = 0项的配置.)*/

}hi_lsw_qos_wred_temp_color_type_e;

typedef struct
{
    hi_uint32     ui_temp_id;   /* WRED模板ID: FWRED[0,15] GWRED[0,3] */

    hi_lsw_qos_wred_temp_color_type_e    em_color_type;
    hi_uint32     ui_color;     /* color_type不同ui_color的范围不同, 详细见: */
                          /*  */
    hi_uint32  ui_thr0;         /* WRED丢弃阈值0 */
    hi_uint32  ui_thr1;         /* WRED丢弃阈值1 */
    hi_uint32  ui_thr2;         /* WRED丢弃阈值2 */
    hi_uint32  ui_thr3;         /* WRED丢弃阈值3 */
    
    hi_uint32  ui_p0;           /* WRED丢弃概率0 */    
    hi_uint32  ui_p1;           /* WRED丢弃概率1 */    
    hi_uint32  ui_p2;           /* WRED丢弃概率2 */
    hi_uint32  ui_p3;           /* WRED丢弃概率3 */

}hi_lsw_qos_wred_table_item_s;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_ADAPTER_H__ */
