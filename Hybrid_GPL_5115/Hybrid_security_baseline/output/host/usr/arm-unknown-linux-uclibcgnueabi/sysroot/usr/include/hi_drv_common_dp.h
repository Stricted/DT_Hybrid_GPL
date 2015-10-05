/* *****************************************************************************
                  版权所有 (C), 2001-2011, 华为技术有限公司
******************************************************************************
  文 件 名   : hi_dp_dp_api.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月26日
  最近修改   :
  功能描述   : SD5113 SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月26日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */
#ifndef  __HI_DRV_COMMON_DP_H__
#define  __HI_DRV_COMMON_DP_H__

#include "hi_drv_common.h"

#define HI_DP_MPW_ASIC_EC 1

#define HI_SMD_ENABLE

//#define HI_DP_INT_NUM               25      
#define HI_DP_EPON_INT_NUM           133
#define HI_DP_GPON_INT_NUM           134
#define HI_DP_LSW_INT_NUM            135
#define HI_DP_ROGUE_ONU_INT_NUM      142 
#define HI_DP_SILENT_ONU_INT_NUM     143 
#define HI_DP_LP_INT_NUM             151 //147  FPGA连线问题

/* BEGIN: Add by G174254, 5115New, 2011-10-29 */
#define HI_DP_NNI_PORT_NUM 1
#define HI_DP_UNI_PORT_NUM 5
/* END: Add by G174254, 2011-10-29 */
#define HI_DP_PORT_NUM 7
#define HI_DP_ETH_PORT_NUM 5
#define HI_DP_SFID_NUM 128
#define HI_DP_FID_NUM 192
#define HI_DP_GEMPORT_NUM 4096
#define HI_DP_TCONT_NUM 8

/* BEGIN: Added by shengjing, 2010/10/16   PN:SD5113V200新增*/
#define HI_DP_FID_NUM_V200 324
/* END:   Added by shengjing, 2010/10/16 */

/* BEGIN: Added by 黎铸锋, 2012/2/9 */
#define HI_DP_1731_MEP_NUM 1
/* END:   Added by 黎铸锋, 2012/2/9 */

#define HI_DP_GEMPORT_INVALID 4096
#define HI_DP_TCONT_INVALID 8
#define HI_DP_LLID_INVALID 8

#define HI_DP_VLAN_MAX 4095
#define HI_DP_PRI_MAX 7
#define HI_DP_GEMPORT_MAX 4095
#define HI_DP_TCONT_MAX 8

#define HI_DP_ALLOCID_MAX 4095

#define HI_DP_PLOAM_MSG_LEN 12
#define HI_DP_OMCI_MSG_HEAD_LEN (4)
#define HI_DP_OMCI_MSG_HEAD_RSD_LEN (4)
#define HI_DP_OMCI_MSG_MAX_LEN (256)
#define HI_DP_OMCI_MSG_EXTEND_LEN (2040)

#define HI_DP_MAC_BYTE_NUM 6
#define HI_DP_SNAP_HEAD_BYTE_NUM 8
#ifdef CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_1731_HEAD_BYTE_NUM 12
#else
#define HI_DP_1731_HEAD_BYTE_NUM 8
#endif
/* BEGIN: Added by shengjing, 2010/10/16   PN:SD5113V200新增*/
#define HI_DP_DSCP_MAX 0x3F
#define HI_DP_SESSION_ID_MAX 0xFFFF

#define HI_DP_TC_MAX 0xFF
#define HI_DP_FL_MAX 0xFFFFF

#define HI_DP_IPV4_ADDR_BYTE_NUM 4
#define HI_DP_IPV6_ADDR_BYTE_NUM 16

#define HI_DP_LAN_NUM 1

//#define HI_DP_WAN_NUM    8
#define HI_DP_WAN_NUM 16   //5115 add
/* END:   Added by shengjing, 2010/10/16 */

/* BEGIN:Added by zhouqingsong */
#define MAX_PIE_CNT_NUM 200         //最大为200个表项

/* BEGIN:Added by zhouqingsong */
#define HI_DP_MPCP_MSG_LEN (60)
#if defined(CONFIG_HISI_SD5115_TYPE_T)
#define HI_DP_EPON_LLID_NUM  16
#define HI_DP_EPON_LLID_INDEX_MAX 15
#else
#define HI_DP_EPON_LLID_NUM  8
#define HI_DP_EPON_LLID_INDEX_MAX 7
#endif

#ifdef CONFIG_HISI_SD5115_AF_FPGA
 #define HI_DP_RX_SD_ADDR 0x900b6008
 #define HI_DP_RX_SD_VAL 0xc0
#endif
#if 0

/*
 #if defined(SD5113V100_PILOT_ASIC)
 #define HI_DP_RX_SD_ADDR            0x16148008
 #elif defined(SD5113V100_PILOT_FPGA)
 #define HI_DP_RX_SD_ADDR            0x900b6008
 #endif
 */
 #define HI_DP_RX_SD_ADDR 0x900b6008           //G174254 2011/9/13

 #define HI_DP_RX_SD_VAL 0xc0
#endif

/*
 #if defined(SD5113V100_PILOT_ASIC)
 #define  HI_DP_PORT_MTU_MIN       64
 #elif defined(SD5113V100_PILOT_FPGA)
 #define  HI_DP_PORT_MTU_MIN       0
 #endif
 */
#define  HI_DP_PORT_MTU_MIN 64 //G174254 2011/9/13

#define  HI_DP_PORT_MTU_MAX 2004   //G174254, 2012/1/6, DTS2012010505083

/* BEGIN: Add by G174254 5115new 2011-10-29 */
#define HI_DP_UNI_SFT_TAG_SUB_TABLE_SIZE 16
/* END: Add dy G174254 2011-10-29 */

/* BEGIN: Added by l00164498, 2012/8/4 */
#define HI_DP_TNL_NUM   8 // 隧道数目
/* END: Added by l00164498, 2012/8/4 */

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_CNT_32BIT_32BIT_E = 0,
    HI_DP_CNT_64BIT_E,
} HI_DP_CNT_MODE_E;
#endif

/* 报文丢弃的reason */
typedef enum
{
    HI_DP_DROP_REASON_NOT_DROP_E = 0,           /* 不丢                              */
    HI_DP_DROP_REASON_MAC_ERR_E,                /* MAC error                         */
    HI_DP_DROP_REASON_PON_FCS_ERR_E,            /* PON FCS error                     */
    HI_DP_DROP_REASON_JUMBO_E,                  /* 超长帧过滤                        */
    HI_DP_DROP_REASON_FRAG_E,                   /* 超短帧过滤                        */
    HI_DP_DROP_REASON_CPU_POINT_NO_EGR_E,       /* CPU指定转发但配置的egress为全0  */
    HI_DP_DROP_REASON_INVALID_SMAC_E,           /* SMAC非法丢包  */
    HI_DP_DROP_REASON_EFC_MATCH0_E,             /* EFC匹配丢包原因0  */
    HI_DP_DROP_REASON_EFC_MATCH1_E,             /* EFC匹配丢包原因1  */
    HI_DP_DROP_REASON_EFC_MATCH2_E,             /* EFC匹配丢包原因2  */
    HI_DP_DROP_REASON_EFC_MATCH3_E,             /* EFC匹配丢包原因3  */
    HI_DP_DROP_REASON_EFC_MATCH4_E,             /* EFC匹配丢包原因4  */
    HI_DP_DROP_REASON_EFC_MATCH5_E,             /* EFC匹配丢包原因5  */
    HI_DP_DROP_REASON_EFC_MATCH6_E,             /* EFC匹配丢包原因6  */
    HI_DP_DROP_REASON_EFC_MATCH7_E,             /* EFC匹配丢包原因7  */
    HI_DP_DROP_REASON_EFC_MATCH8_E,             /* EFC匹配丢包原因8  */
    HI_DP_DROP_REASON_EFC_MATCH9_E,             /* EFC匹配丢包原因9  */
    HI_DP_DROP_REASON_EFC_MATCH10_E,            /* EFC匹配丢包原因10  */
    HI_DP_DROP_REASON_EFC_MATCH11_E,            /* EFC匹配丢包原因11  */
    HI_DP_DROP_REASON_EFC_MATCH12_E,            /* EFC匹配丢包原因12  */
    HI_DP_DROP_REASON_EFC_MATCH13_E,            /* EFC匹配丢包原因13  */
    HI_DP_DROP_REASON_EFC_MATCH14_E,            /* EFC匹配丢包原因14  */
    HI_DP_DROP_REASON_EFC_MATCH15_E,            /* EFC匹配丢包原因15  */
    HI_DP_DROP_REASON_EGR_VLAN_CHECK_E,         /*23: 出口VLAN检查丢弃  */
    HI_DP_DROP_REASON_PORT_ISOLATION_E,         /*24: 端口隔离丢弃  */
    HI_DP_DROP_REASON_VLAN_TAG_E,               /*25: VLAN_Tag报文过滤  */
    HI_DP_DROP_REASON_IGR_CAR_FCTRL_E,          /*26: 入端口流控  */
    HI_DP_DROP_REASON_INVALID_USER_VLAN_E,      /*27: 用户VLAN过滤（VLAN入口检查失败）  */
    HI_DP_DROP_REASON_NAPT_IP_FRAG_E,           /*28: IP分片报文做NAPT丢弃 */
    HI_DP_DROP_REASON_IGMP_PPPOE_E,             /*29: IGMP on  PPPoE报文过滤  */
    HI_DP_DROP_REASON_ARP_E,                    /*30: ARP过滤  */
    HI_DP_DROP_REASON_RARP_E,                   /*31: RARP过滤  */
    HI_DP_DROP_REASON_PPPOE_DIS_E,              /*32: PPPoE Discovery过滤               */
    HI_DP_DROP_REASON_8021X_NOT_AUTH_E,         /*33: 802.1x认证未授权状态端口关闭      */
    HI_DP_DROP_REASON_STP_RSTP_E,               /*34: STP/RSTP丢包                      */
    HI_DP_DROP_REASON_WHITELIST_E,              /*35: 白名单过滤                        */
    HI_DP_DROP_REASON_DHCP_E,                   /*36: DHCP报文过滤                      */
    HI_DP_DROP_REASON_IGMP_IPOE_E,              /*37: IGMP on  IPoE报文过滤  */
    HI_DP_DROP_REASON_ICMP_E,                   /*38: ICMP报文过滤                      */
    HI_DP_DROP_REASON_POINT_TYPE0_E,            /*39: 指定第1种ETH_Type报文过滤         */
    HI_DP_DROP_REASON_POINT_TYPE1_E,            /*40: 指定第2种ETH_Type报文过滤         */
    HI_DP_DROP_REASON_POINT_TYPE2_E,            /*41: 指定第3种ETH_Type报文过滤         */
    HI_DP_DROP_REASON_POINT_TYPE3_E,            /*42: 指定第4种ETH_Type报文过滤         */
    HI_DP_DROP_REASON_BPDU_E,                   /*43: BPDU报文丢弃  */
    HI_DP_DROP_REASON_REMOTE_LP_TEST_PORT_E,    /*44: 远端环回测试端口配置错误丢弃*/
    HI_DP_DROP_REASON_FAIL_LEARN_E,             /*45: 自动学习失败丢弃报文              */
    HI_DP_DROP_REASON_EAPOL_E,                  /*46: EAPOL报文丢弃  */
    HI_DP_DROP_REASON_UNHIT_DMAC_E,             /*47: DMAC查表失败丢弃报文              */
    HI_DP_DROP_REASON_IGR_FILTER_E,             /*48: 源端口抑制                        */
    HI_DP_DROP_REASON_NAPT_IP_OPTION_E,           /*49: IP Option报文NAPT指示丢弃*/
    HI_DP_DROP_REASON_TOCPU_REASON_CAR_E,       /*50: TOCPU REASON CAR丢包 */

    //    HI_DP_DROP_REASON_FAIL_MAP_GEMPORT_E,       /* GEM PORT映射失败丢包              */
    HI_DP_DROP_REASON_FAIL_UP_SFC_E,            /*51: 上行简单流分类失败丢包            */
    HI_DP_DROP_REASON_FAIL_UP_MAP_E,            /*52: 上行映射失败丢包                  */
    HI_DP_DROP_REASON_SCAR_CPU_E,               /*53: TO_CPU CAR丢包                    */
    HI_DP_DROP_REASON_SCAR_LOCAL_E,             /*54: 本地转发CAR丢包                   */
    HI_DP_DROP_REASON_SCAR_BC_E,                /*55: 广播CAR丢包                       */
    HI_DP_DROP_REASON_SCAR_UUC_E,               /*56: 未知单播CAR丢包                   */
    HI_DP_DROP_REASON_FIFO_FULL_MC_COPY_E,      /*57: 组播复制FIFO满丢包                */
    HI_DP_DROP_REASON_NAPT_SUBNET_UNHIT_E,      /*58: NAPT模块中下行报文查找Subnet表项匹配失败，软件指示直接丢包  */
    HI_DP_DROP_REASON_OAM_3AH_LP_E,             /*59: 3ah OAM远端环回丢弃                */
    HI_DP_DROP_REASON_MD_LEVEL_FILTER_E,        /*60: 1ag 报文MD Level过滤丢弃           */
    HI_DP_DROP_REASON_OAM_DISABLE_E,            /*61: OAM功能没有使能丢弃 */
    HI_DP_DROP_REASON_FIFO_FULL_BC_COPY_E,      /*62: 广播复制FIFO满丢包 */
    HI_DP_DROP_REASON_PORT_TAGGED_DROP_E,       /*63: 端口tagged模式丢包 */
    HI_DP_DROP_REASON_BC_NO_EGR_E,              /*64: 广播目的端口全零丢弃 */
    HI_DP_DROP_REASON_MC_NO_EGR_E,              /*65: 组播目的端口全零丢弃 */
    HI_DP_DROP_REASON_PORT_DEFAULT_TAG_CHECK_E = 66,     /*66:端口默认TAG检查  */
    HI_DP_DROP_REASON_PORT_ASSEMBLY_ILLEGAL_E,    /* 67：级联模式非法报文（含untagged或外层TAG为非私有TAG） */
    HI_DP_DROP_REASON_UNKNOWN_EXTERN_HEADER_E,    /* 68：IPV6非知名扩展头 */
    HI_DP_DROP_REASON_IPV6_SIP_SITE_LOCAL_E,      /* 69：IPv6源IP为Site Local  */
    HI_DP_DROP_REASON_IPV6_SIP_0_E,               /* 70：IPV6 SIP等于0 */
    HI_DP_DROP_REASON_EFC_ECAR_E,                 /* 71：EFC的流CAR丢弃 */
    HI_DP_DROP_REASON_PPPOE_SESSION_LCP_E,        /* 72：PPPOE  Session阶段的LCP报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_NCP_E,        /* 73：PPPOE  Session阶段的NCP报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_CHAP_E,       /* 74：PPPOE  Session阶段的CHAP报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_PAP_E,        /* 75：PPPOE  Session阶段的PAP报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_IPV6CP_E,     /* 76：PPPOE  Session阶段的IPV6CP报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_0_E,    /* 77：PPPOE  Session阶段的用户自定义类型0报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_1_E,    /* 78：PPPOE  Session阶段的用户自定义类型1报文 */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_2_E,    /* 79：PPPOE  Session阶段的用户自定义类型2报文 */
    HI_DP_DROP_REASON_DHCPV6_E,                   /* 80：DHCPV6报文 */
    HI_DP_DROP_REASON_ICMPV6_RS_E,                /* 81：ICMPV6 RS报文 */
    HI_DP_DROP_REASON_ICMPV6_RA_E,                /* 82：ICMPV6 RA报文 */
    HI_DP_DROP_REASON_ICMPV6_NS_E,                /* 83：ICMPV6 NS报文 */
    HI_DP_DROP_REASON_ICMPV6_NA_E,                /* 84：ICMPV6 NA报文 */
    HI_DP_DROP_REASON_ICMPV6_RM_E,                /* 85：ICMPV6 RM报文 */
    HI_DP_DROP_REASON_ICMPV6_PING_REQ_E,          /* 86：ICMPV6 PingReq报文 */
    HI_DP_DROP_REASON_ICMPV6_PING_REPLY_E,        /* 87：ICMPV6 PingREPLY报文 */
    HI_DP_DROP_REASON_ICMPV6_UNREACH_E,           /* 88：ICMPv6目的不可达差错报文 */
    HI_DP_DROP_REASON_ICMPV6_GROUP_BIG_E,         /* 89：ICMPv6分组太大报文 */
    HI_DP_DROP_REASON_ICMPV6_OUTTIME_E,           /* 90：ICMPv6超时差错报文 */
    HI_DP_DROP_REASON_ICMPV6_PAMAM_ERROE_E,       /* 91：ICMPv6参数错误报文 */
    HI_DP_DROP_REASON_IPOE_MLD_QUERY_V1_V2_E,     /*92：IPOE承载的MLD Query v1/v2报文 */
    HI_DP_DROP_REASON_IPOE_MLD_REPORT_V1_E,       /* 93：IP0E承载的MLD Report v1报文 */
    HI_DP_DROP_REASON_IPOE_MLD_REPORT_V2_E,       /* 94：IPOE承载的MLD Report v2报文 */
    HI_DP_DROP_REASON_IPOE_MLD_DONE_E,            /* 95：IPOE承载的MLD DONE报文 */
    HI_DP_DROP_REASON_PPPOE_MLD_QUERY_V1_V2_E,    /* 96：PPPPOE承载的MLD Query v1/v2报文 */
    HI_DP_DROP_REASON_PPPOE_MLD_REPORT_V1_E,      /* 97：PPPOE承载的MLD Report v1报文 */
    HI_DP_DROP_REASON_PPPOE_MLD_REPORT_V2_E,      /* 98：PPPOE承载的MLD Report v2报文 */
    HI_DP_DROP_REASON_PPPOE_MLD_DONE_E,            /* 99：PPPOE承载的MLD DONE报文 */
    HI_DP_DROP_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,  /* 100：超过1个扩展头的IPV6报文 */
    HI_DP_DROP_REASON_HEADER_LAREGR_128_BYTE_E,    /* 101：报文头长度超过128B */
    HI_DP_DROP_REASON_EFC_POINT_NO_EGR_E,       /* 102：EFC指定转发无出口 */
    HI_DP_DROP_REASON_HL_TTL_LESS_1_E,             /* 103：HOP_LIMIT/TTL小于等于1捕获报文 */
    HI_DP_DROP_REASON_RT_FAIL_E,                   /* 104：默认路由禁止时路由失败 */
    HI_DP_DROP_REASON_BLACK_HOLE_RT_E,             /* 105：黑洞路由 */
    HI_DP_DROP_REASON_NO_NEXT_HOP_E,               /* 106：无下一跳 */
    HI_DP_DROP_REASON_IN_TUNNEL_MTU_FAIL_E,        /* 107：入隧道MTU检查失败 */
    HI_DP_DROP_REASON_IGR_CAR_E,                   /* 108：入口car丢弃 */
    HI_DP_DROP_REASON_EGR_CAR_E,                   /* 109：出口car丢弃 */
    HI_DP_DROP_REASON_TUNNEL_INTERFACE_INVLD_E,    /* 110：隧道接口无效 */
    HI_DP_DROP_REASON_SW_POINT_DROP_E,             /* 111：软件指示上行丢包：NAPT模块中上行报文查找Subnet表项匹配失败，软件指示直接丢包 */
    HI_DP_DROP_REASON_SNAP_LENGTH_OVER_1500_E,     /* 112：SNAP长度超过1500 */
    HI_DP_DROP_REASON_GROUP_CAR_E,                 /* 113：组car丢弃 */
    HI_DP_DROP_REASON_EGR_ALL_0_E,                 /* 114：端口全0 */
    HI_DP_DROP_REASON_SIPV6_LOOP_E,                /* 115：SIPv6等于环回地址 */
    HI_DP_DROP_REASON_SIPV6_MC_E,                  /* 116：SIPv6等于组播地址 */
    HI_DP_DROP_REASON_SIPV4_0_E,                   /* 117：SIPv4等于0 */
    HI_DP_DROP_REASON_SIPV4_LOOP_E,                /* 118：SIPv4等于环回地址 */
    HI_DP_DROP_REASON_SIPV4_MC_E,                  /* 119：SIPv4等于组播地址 */
    HI_DP_DROP_REASON_SIPV6_LINK_LOCAL_E,          /* 120：SIPv6等于Link Local地址 */
    HI_DP_DROP_REASON_IP_FILTER_E,                 /* 121：黑白名单过滤 */
    HI_DP_DROP_REASON_L3_IF_ISOLATION_E,           /* 122: 同一个端口下L3接口隔离 */
    HI_DP_DROP_REASON_TUNNEL_UNUSED_LENGTH_E,      /* 123: 入隧道报文超长或出隧道过短*/
    HI_DP_DROP_REASON_FRAG_6RD_E,                  /* 124: 6RD分片报文出隧道*/
    HI_DP_DROP_REASON_IPV4_OPTION_6RD_E,           /* 125: 6RD带IPv4 Option报文出隧道*/

    HI_DP_DROP_NUM_E,
} HI_DP_DROP_REASON_E;

typedef enum
{
    HI_DP_THRU_REASON_NOT_THRU_E = 0,                 /* 不透传                            */
    HI_DP_THRU_REASON_EFC_MATCHED0_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED1_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED2_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED3_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED4_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED5_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED6_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED7_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED8_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED9_E,                  /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED10_E,                 /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED11_E,                 /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED12_E,                 /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED13_E,                 /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED14_E,                 /* 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_EFC_MATCHED15_E,                 /*16: 自定义EFC匹配透传                 */
    HI_DP_THRU_REASON_GE0_THRU_E,                      /*17: GE0端口为透传模式                 */
    HI_DP_THRU_REASON_GE1_THRU_E,                      /*18: GE1端口为透传模式                 */
    HI_DP_THRU_REASON_FE0_THRU_E,                      /*19: FE0端口为透传模式                 */
    HI_DP_THRU_REASON_FE1_THRU_E,                      /*20: FE1端口为透传模式                 */
    HI_DP_THRU_REASON_FE2_THRU_E,                      /*21: FE2端口为透传模式                 */
    HI_DP_THRU_REASON_BPDU_E,                         /*22: BPDU报文                         */
    HI_DP_THRU_REASON_EAPOL_E,                        /*23: EAPOL报文                      */
    HI_DP_THRU_REASON_OAM_3AH,                        /*24: OAM 3AH报文                      */
    HI_DP_THRU_REASON_OAM_1AG,                        /*25: OAM 1AG报文                      */
    HI_DP_THRU_REASON_IGMP_IPOE_E,                /*26: IPoE承载 IGMP报文                          */
    HI_DP_THRU_REASON_ARP_E,                           /*27: ARP报文                           */
    HI_DP_THRU_REASON_RARP_E,                          /*28: RARP报文                          */
    HI_DP_THRU_REASON_PPPOE_DIS_E,                     /*29: PPPOE DISCOVERY报文               */
    HI_DP_THRU_REASON_IGMP_PPPOE_E,                    /*30: PPPoE承载 IGMP报文                          */
    HI_DP_THRU_REASON_DHCP_E = 32,                     /*32: DHCP报文                          */

    //    HI_DP_THRU_REASON_IPV6_E,                          /* IPV6报文                          */
    HI_DP_THRU_REASON_ICMP_E = 34,                     /*34: ICMP报文                          */
    HI_DP_THRU_REASON_POINT_TYPE0_E,                   /*35: 自定义类型透传                    */
    HI_DP_THRU_REASON_POINT_TYPE1_E,                   /*36: 自定义类型透传                    */
    HI_DP_THRU_REASON_POINT_TYPE2_E,                   /*37: 自定义类型透传                    */
    HI_DP_THRU_REASON_POINT_TYPE3_E,                   /*38: 自定义类型透传                    */

    //    HI_DP_THRU_REASON_PPPOE_SES_E,                     /* PPPOE SESSION报文                 */
    HI_DP_THRU_REASON_BASE_ON_SMAC_E = 40,                  /* 40: 基于SMAC透传                      */

    HI_DP_THRU_REASON_PPPOE_SESSION_LCP_E = 72,       /* 72：PPPOE  Session阶段的LCP报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_NCP_E,            /* 73：PPPOE  Session阶段的NCP报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_CHAP_E,           /* 74：PPPOE  Session阶段的CHAP报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_PAP_E,            /* 75：PPPOE  Session阶段的PAP报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_IPV6CP_E,         /* 76：PPPOE  Session阶段的IPV6CP报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_0_E,        /* 77：PPPOE  Session阶段的用户自定义类型0报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_1_E,        /* 78：PPPOE  Session阶段的用户自定义类型1报文 */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_2_E,        /* 79：PPPOE  Session阶段的用户自定义类型2报文 */
    HI_DP_THRU_REASON_DHCPV6_E,                       /* 80：DHCPV6报文 */
    HI_DP_THRU_REASON_ICMPV6_RS_E,                    /* 81：ICMPV6 RS报文 */
    HI_DP_THRU_REASON_ICMPV6_RA_E,                    /* 82：ICMPV6 RA报文 */
    HI_DP_THRU_REASON_ICMPV6_NS_E,                    /* 83：ICMPV6 NS报文 */
    HI_DP_THRU_REASON_ICMPV6_NA_E,                    /* 84：ICMPV6 NA报文 */
    HI_DP_THRU_REASON_ICMPV6_RM_E,                    /* 85：ICMPV6 RM报文 */
    HI_DP_THRU_REASON_ICMPV6_PING_REQ_E,              /* 86：ICMPV6 PingReq报文 */
    HI_DP_THRU_REASON_ICMPV6_PING_REPLY_E,            /* 87：ICMPV6 PingREPLY报文 */
    HI_DP_THRU_REASON_ICMPV6_UNREACH_E,               /* 88：ICMPv6目的不可达差错报文 */
    HI_DP_THRU_REASON_ICMPV6_GROUP_BIG_E,             /* 89：ICMPv6分组太大报文 */
    HI_DP_THRU_REASON_ICMPV6_OUTTIME_E,               /* 90：ICMPv6超时差错报文 */
    HI_DP_THRU_REASON_ICMPV6_PAMAM_ERROE_E,           /* 91：ICMPv6参数错误报文 */
    HI_DP_THRU_REASON_IPOE_MLD_QUERY_V1_V2_E,         /*92：IPOE承载的MLD Query v1/v2报文 */
    HI_DP_THRU_REASON_IPOE_MLD_REPORT_V1_E,           /* 93：IP0E承载的MLD Report v1报文 */
    HI_DP_THRU_REASON_IPOE_MLD_REPORT_V2_E,           /* 94：IPOE承载的MLD Report v2报文 */
    HI_DP_THRU_REASON_IPOE_MLD_DONE_E,                /* 95：IPOE承载的MLD DONE报文 */
    HI_DP_THRU_REASON_PPPOE_MLD_QUERY_V1_V2_E,        /* 96：PPPPOE承载的MLD Query v1/v2报文 */
    HI_DP_THRU_REASON_PPPOE_MLD_REPORT_V1_E,          /* 97：PPPOE承载的MLD Report v1报文 */
    HI_DP_THRU_REASON_PPPOE_MLD_REPORT_V2_E,          /* 98：PPPOE承载的MLD Report v2报文 */
    HI_DP_THRU_REASON_PPPOE_MLD_DONE_E,                /* 99：PPPOE承载的MLD DONE报文 */
    HI_DP_THRU_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,      /* 100：超过1个扩展头的IPV6报文 */
    HI_DP_THRU_REASON_HEADER_LAREGR_128_BYTE_E,        /* 101：报文头长度超过128B */

    HI_DP_THRU_NUM_E,
} HI_DP_THRU_REASON_E;

typedef enum
{
    HI_DP_TOCPU_REASON_L2_WLAN_E = 0,         /* 0： L2转发到非网关接口（WLAN） */
    HI_DP_TOCPU_REASON_EFC_MATCH0_E,           /*1: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH1_E,           /*2: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH2_E,           /*3: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH3_E,           /*4: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH4_E,           /*5: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH5_E,           /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH6_E,           /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH7_E,           /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH8_E,           /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH9_E,           /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH10_E,          /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH11_E,          /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH12_E,          /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH13_E,          /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH14_E,          /* 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_EFC_MATCH15_E,          /*16: 自定义EFC匹配交CPU原因            */
    HI_DP_TOCPU_REASON_RTP_CAPTURE_E,          /*17: RTP流捕获            */

    //    HI_DP_TOCPU_REASON_UNMATCH_NAPT_E ,         /* 17: 为NAPT进行EFC匹配失败             */
    //    HI_DP_TOCPU_REASON_UNTAG_E ,                /* UNTAG报文                         */
    //    HI_DP_TOCPU_REASON_VLAN_TAG_E ,             /* VLAN TAG报文                      */
    //    HI_DP_TOCPU_REASON_PRI_TAG_E ,              /* PRI UNTAG报文                     */
    //    HI_DP_TOCPU_REASON_BC_E ,                   /* 广播报文                          */
    HI_DP_TOCPU_REASON_IGMP_PPPOE_E = 22,      /*22: pppoe承载IGMP报文                          */
    HI_DP_TOCPU_REASON_IGMP_IPOE_E,            /*23: ipoe承载IGMP报文                          */
    HI_DP_TOCPU_REASON_ARP_E,                  /*24: ARP报文                           */
    HI_DP_TOCPU_REASON_RARP_E,                 /*25: RARP报文                          */
    HI_DP_TOCPU_REASON_PPPOE_DIS_E,            /*26: PPPOE DISCOVERY报文               */
    HI_DP_TOCPU_REASON_EAPOL_E,                /*27: EAPOL报文                         */
    HI_DP_TOCPU_REASON_BPDU_E,                 /*28: BPDU报文                          */
    HI_DP_TOCPU_REASON_DHCP_E,                 /*29: DHCP报文                          */
    
    HI_DP_TOCPU_REASON_DSLITE_LAGGER_THAN_MTU_E ,  /*30: 报文入DS Lite 隧道MTU 超长交CPU(CFM)    */
    HI_DP_TOCPU_REASON_ICMP_E = 31,            /*31: ICMP报文                          */
    HI_DP_TOCPU_REASON_POINT_TYPE0_E,          /*32: 自定义捕获的报文0                  */
    HI_DP_TOCPU_REASON_POINT_TYPE1_E,          /*33: 自定义捕获的报文1                  */
    HI_DP_TOCPU_REASON_POINT_TYPE2_E,          /*34: 自定义捕获的报文2                  */
    HI_DP_TOCPU_REASON_POINT_TYPE3_E,          /*35: 自定义捕获的报文3                  */

    //    HI_DP_TOCPU_REASON_PPPOE_SES_E ,            /* 36: PPPOE SESSION报文                 */
    HI_DP_TOCPU_REASON_PORT_CHANGE_E = 37,      /* 37: 静态表项的端口发生移动            */
    HI_DP_TOCPU_REASON_MAC_LEARN_E,            /* 38: 报文提交给CPU学习                 */
    HI_DP_TOCPU_REASON_MAC_LEARN_FAIL_E,       /* 39: 逻辑学习报文失败                  */
    HI_DP_TOCPU_REASON_SEARCH_FAIL_E,          /* 40: 逻辑查表失败                      */
    /*BEGIN: 问题单号DTS2012032703169，by lKF55238,2012/4/18*/
    HI_DP_TOCPU_REASON_TCP_CTRL_E,           /*41:TCP    控制报文交CPU*/
    /*END: 问题单号DTS2012032703169，by lKF55238,2012/4/18*/
    HI_DP_TOCPU_REASON_NAPT_ON_E = 42,         /* 42: NAPT加速开启时首包提交            */
    HI_DP_TOCPU_REASON_NAPT_IP_OPTION_E,       /* 43: IP Option报文做NAPT            */


    HI_DP_TOCPU_REASON_6RD_LAGGER_THAN_MTU_E , /*44: 报文入6rd 隧道MTU 超长交CPU(CFM)    */

    //HI_DP_TOCPU_REASON_UNMATCH_NAPT_OUTNET_E ,  /* 44: NAPT的OUTNET表不匹配              */
    HI_DP_TOCPU_REASON_1AG_Y1731_E = 45,       /* 45: 802.1ag / Y.1731报文              */
    HI_DP_TOCPU_REASON_3AH_E,                  /* 46: 802.3ah报文                       */
#if 0
    /* V200, 47 - 53为保留 */
    HI_DP_TOCPU_REASON_PDELAY_REQ_E,           /* 47: Pdelay_req报文捕获交CPU            */
    HI_DP_TOCPU_REASON_PDELAY_RESP_E,          /* 48: Pdelay_resp报文捕获交CPU           */
    HI_DP_TOCPU_REASON_PDELAY_RESP_FOLLOW_UP_E,/* 49: Pdelay_resp_follow_up报文捕获交CPU */
    HI_DP_TOCPU_REASON_MANAGEMENT_E,           /* 50: Management报文交CPU                */
    HI_DP_TOCPU_REASON_SIGNALING_E,            /* 51: Signaling报文交CPU                 */
    HI_DP_TOCPU_REASON_ANNOUNCE_E,             /* 52: Announce报文交CPU                  */
    HI_DP_TOCPU_REASON_OTHER_1588_E,           /* 53: 其他1588协议报文交CPU              */

    HI_DP_TOCPU_REASON_NAPT_DN_FAIL_E,         /* 54: NAPT下行查表失败交CPU              */
#endif
    HI_DP_TOCPU_REASON_CCM_LMM_0_E,             /*47: LM测试，捕获CCM/LMM报文，计数0 */
    HI_DP_TOCPU_REASON_CCM_LMM_1_E,             /*48: LM测试，捕获CCM/LMM报文，计数1 */
    HI_DP_TOCPU_REASON_CCM_LMM_2_E,             /*49: LM测试，捕获CCM/LMM报文，计数2 */
    HI_DP_TOCPU_REASON_CCM_LMM_3_E,             /*50: LM测试，捕获CCM/LMM报文，计数3 */
    HI_DP_TOCPU_REASON_CCM_LMM_4_E,             /*51: LM测试，捕获CCM/LMM报文，计数4 */
    HI_DP_TOCPU_REASON_CCM_LMM_5_E,             /*52: LM测试，捕获CCM/LMM报文，计数5 */
    HI_DP_TOCPU_REASON_CCM_LMM_6_E,             /*53: LM测试，捕获CCM/LMM报文，计数6 */
    HI_DP_TOCPU_REASON_CCM_LMM_7_E,             /*54: LM测试，捕获CCM/LMM报文，计数7 */
    HI_DP_TOCPU_REASON_IP_FRAGMENT_NAPT_E,      /* 55．	IP分片报文做NAPT交CPU */
    HI_DP_TOCPU_REASON_NAPT_DN_FAILE_E,         /* 56．	NAPT下行查表失败交CPU */
    HI_DP_TOCPU_REASON_MC_GEMPORT_IPV4_UMC_E,    /* 57: 组播GEMPORT承载IPv4未知组播报文交CPU */
    HI_DP_TOCPU_REASON_TUNNEL_UNKNOWN_MC_E,        /* 58: 隧道未知组播报文交CPU */
    HI_DP_TOCPU_REASON_MC_GEMPORT_IPV6_UMC_E,     /* 59: 组播GEMPORT承载IPv6未知组播报文交CPU */
    HI_DP_TOCPU_REASON_UC_BC_GEMPORT_IPV4_UMC_E, /* 60: 非组播GEMPORT承载IPv4未知组播报文交CPU */
    HI_DP_TOCPU_REASON_UC_BC_GEMPORT_IPV6_UMC_E, /* 61: 非组播GEMPORT承载IPv6未知组播报文交CPU */

    HI_DP_TOCPU_REASON_BC_IGR_MIRROR_E,    /* 62: 广播源端口镜像转发 */
    HI_DP_TOCPU_REASON_MC_IGR_MIRROR_E,     /* 63: 组播源端口镜像转发 */
    HI_DP_TOCPU_REASON_NAPT_EGR_IS_CPU_E,  /* 64．	NAPT加速到用户口（比如WLAN） */
    HI_DP_TOCPU_REASON_RT_TO_GW_IF_E,              /* 65．	路由到网关接口 */
    HI_DP_TOCPU_REASON_L2_FWD_TO_GW_IF_E,          /* 66．	L2转发到网关接口 */
    HI_DP_TOCPU_REASON_EFC_POINT_TO_WLAN_E,        /* 67：EFC指定转发到WLAN */
    HI_DP_TOCPU_REASON_UNKNOWN_EXTERN_HEADER_E,    /* 68：IPV6非知名扩展头 */
    HI_DP_TOCPU_REASON_IPV6_SIP_SITE_LOCAL_E,      /* 69：IPv6源IP为Site Local  */
    HI_DP_TOCPU_REASON_IPV6_SIP_0_E,               /* 70：IPV6 SIP等于0 */
    HI_DP_TOCPU_REASON_IPV6_SIP_LINK_LOCAL_E,      /* 71：IPv6源IP为Link Local  */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_LCP_E,        /* 72：PPPOE  Session阶段的LCP报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_NCP_E,        /* 73：PPPOE  Session阶段的NCP报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_CHAP_E,       /* 74：PPPOE  Session阶段的CHAP报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_PAP_E,        /* 75：PPPOE  Session阶段的PAP报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_IPV6CP_E,     /* 76：PPPOE  Session阶段的IPV6CP报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_0_E,    /* 77：PPPOE  Session阶段的用户自定义类型0报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_1_E,    /* 78：PPPOE  Session阶段的用户自定义类型1报文 */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_2_E,    /* 79：PPPOE  Session阶段的用户自定义类型2报文 */
    HI_DP_TOCPU_REASON_DHCPV6_E,                   /* 80：DHCPV6报文 */
    HI_DP_TOCPU_REASON_ICMPV6_RS_E,                /* 81：ICMPV6 RS报文 */
    HI_DP_TOCPU_REASON_ICMPV6_RA_E,                /* 82：ICMPV6 RA报文 */
    HI_DP_TOCPU_REASON_ICMPV6_NS_E,                /* 83：ICMPV6 NS报文 */
    HI_DP_TOCPU_REASON_ICMPV6_NA_E,                /* 84：ICMPV6 NA报文 */
    HI_DP_TOCPU_REASON_ICMPV6_RM_E,                /* 85：ICMPV6 RM报文 */
    HI_DP_TOCPU_REASON_ICMPV6_PING_REQ_E,          /* 86：ICMPV6 PingReq报文 */
    HI_DP_TOCPU_REASON_ICMPV6_PING_REPLY_E,        /* 87：ICMPV6 PingREPLY报文 */
    HI_DP_TOCPU_REASON_ICMPV6_UNREACH_E,           /* 88：ICMPv6目的不可达差错报文 */
    HI_DP_TOCPU_REASON_ICMPV6_GROUP_BIG_E,         /* 89：ICMPv6分组太大报文 */
    HI_DP_TOCPU_REASON_ICMPV6_OUTTIME_E,           /* 90：ICMPv6超时差错报文 */
    HI_DP_TOCPU_REASON_ICMPV6_PAMAM_ERROE_E,       /* 91：ICMPv6参数错误报文 */
    HI_DP_TOCPU_REASON_IPOE_MLD_QUERY_V1_V2_E,     /*92：IPOE承载的MLD Query v1/v2报文 */
    HI_DP_TOCPU_REASON_IPOE_MLD_REPORT_V1_E,       /* 93：IP0E承载的MLD Report v1报文 */
    HI_DP_TOCPU_REASON_IPOE_MLD_REPORT_V2_E,       /* 94：IPOE承载的MLD Report v2报文 */
    HI_DP_TOCPU_REASON_IPOE_MLD_DONE_E,            /* 95：IPOE承载的MLD DONE报文 */
    HI_DP_TOCPU_REASON_PPPOE_MLD_QUERY_V1_V2_E,    /* 96：PPPPOE承载的MLD Query v1/v2报文 */
    HI_DP_TOCPU_REASON_PPPOE_MLD_REPORT_V1_E,      /* 97：PPPOE承载的MLD Report v1报文 */
    HI_DP_TOCPU_REASON_PPPOE_MLD_REPORT_V2_E,      /* 98：PPPOE承载的MLD Report v2报文 */
    HI_DP_TOCPU_REASON_PPPOE_MLD_DONE_E,            /* 99：PPPOE承载的MLD DONE报文 */
    HI_DP_TOCPU_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,  /* 100：超过1个扩展头的IPV6报文 */
    HI_DP_TOCPU_REASON_HEADER_LAREGR_128_BYTE_E,    /* 101：报文头长度超过128B */
    HI_DP_TOCPU_REASON_RT_FWD_USR_E,                /* 102：路由转发到cpu用户口 */
    HI_DP_TOCPU_REASON_HL_TTL_LESS_1_E,             /* 103：HOP_LIMIT/TTL小于等于1捕获报文 */
    HI_DP_TOCPU_REASON_RT_FAIL_E,                   /* 104：默认路由禁止时路由失败 */
    HI_DP_TOCPU_REASON_REFUSE_HOLE_RT_E,            /* 105：拒绝路由 */
    HI_DP_TOCPU_REASON_NO_NEXT_HOP_E,               /* 106：无下一跳 */
    HI_DP_TOCPU_REASON_IN_TUNNEL_MTU_FAIL_E,        /* 107：入隧道MTU检查失败 */
    HI_DP_TOCPU_REASON_BC_BC_E,                     /* 108：广播报文广播到CPU  */
    HI_DP_TOCPU_REASON_UC_E,                        /* 109：未知单播（广播到CPU或转交CPU） */
    HI_DP_TOCPU_REASON_MC_FWD_TOCPU_E,              /* 110：已知组播报文转发到CPU用户口 */
    HI_DP_TOCPU_REASON_SIPV6_LOOP_E,                /* 111：SIPv6等于环回地址 */
    HI_DP_TOCPU_REASON_SIPV6_MC_E,                  /* 112：SIPv6等于组播地址 */
    HI_DP_TOCPU_REASON_SIPV4_0_E,                   /* 113：SIPv4等于0 */
    HI_DP_TOCPU_REASON_SIPV4_LOOP_E,                /* 114：SIPv4等于环回地址 */
    HI_DP_TOCPU_REASON_SIPV4_MC_E,                  /* 115：SIPv4等于组播地址 */
    HI_DP_TOCPU_REASON_TOCPU_AND_ROUTE_E,           /* 116：路由+拷贝CPU */
    HI_DP_TOCPU_REASON_TOCPU_AND_NAPT_E,            /* 117：NAPT+拷贝CPU */
    HI_DP_TOCPU_REASON_SNAP_LENGTH_OVER_1500_E,     /* 118：SNAP长度超过1500 */
    HI_DP_TOCPU_REASON_DS_LITE_OUT_E,               /* 119: 带隧道头的本地DS_LITE报文出隧道 */
    HI_DP_TOCPU_REASON_BC_VLAN_E,                   /* 120：广播报文vlan用户广播到CPU  */
    HI_DP_TOCPU_REASON_MC_VLAN_E,                  /* 121：已知组播报文VLAN转发到CPU用户口 */

    /* BEGIN: Added by shengjing, 2012/1/10   PN:DTS2012011002267 */

    HI_DP_TOCPU_REASON_FRAG_6RD_E,                /* 122: 6RD分片报文交cpu |||5115T IPv6 带分片扩展头交CPU(RT)*/
    HI_DP_TOCPU_REASON_DROP_REDIRECTION_E,                /* 123: 丢弃报文重定向交cpu */
    /* END:   Added by shengjing, 2012/1/10 */
    HI_DP_TOCPU_REASON_6RD_FRAG_E = 124,                  /* 124：6rd分片报文出隧道 */
    HI_DP_TOCPU_REASON_6RD_IPV4_OPTION_E,                  /* 125：6rd带IPv4 option报文出隧道 */
    HI_DP_TOCPU_REASON_EGR_MIRROR_E,                  /* 126:出端口镜像 */
    HI_DP_TOCPU_REASON_IGR_MIRROR_E,                  /* 127:入端口镜像 */

    HI_DP_TOCPU_NUM_E,
} HI_DP_TOCPU_REASON_E;

typedef enum
{
    HI_DP_IC_NULL_E = 0,
    HI_DP_IC_MPW_E,
    HI_DP_IC_H_E,
    HI_DP_IC_S_E,
    HI_DP_IC_T_E,
} HI_DP_IC_VER_E;

typedef enum
{
    HI_DP_EPON_MODE_E = 0, /* */
    HI_DP_GPON_MODE_E,
    HI_DP_UNKNOWN_PON_MODE_E,
} HI_DP_PON_MODE_E;

typedef enum
{
    HI_DP_UP_EPON_MODE_E = 0,
    HI_DP_UP_GPON_MODE_E,
    HI_DP_UP_GE_MODE_E,
    HI_DP_UP_UNKNOWN_MODE_E,    /* 默认 */
} HI_DP_UP_MODE_E;

typedef enum
{
    HI_DP_UP_TYPE_E = 0,  /* 上行QOS,默认 */
    HI_DP_DN_TYPE_E,      /* 下行QOS */
} HI_DP_QOS_TYPE_E;

/* 调整:上行字序模式 */
typedef enum
{
    HI_DP_BIG_ENDIAN_E = 0,
    HI_DP_LITTLE_ENDIAN_E,
} HI_DP_ENDIAN_E;

/* 端口 */

/*
typedef enum
{
    HI_DP_PORT_GE0_E  = 0 , // UNI_ETH0
    HI_DP_PORT_GE1_E,       // UNI_ETH1
    HI_DP_PORT_FE0_E,       // UNI_ETH2
    HI_DP_PORT_FE1_E,       // UNI_ETH3
    HI_DP_PORT_FE2_E,       // UNI_ETH4
    HI_DP_PORT_PON_E,       // NNI_PON
    HI_DP_PORT_CPU_E,       // UNI_CPU
    HI_DP_MIR_PORT_CPU_WAN_E,
    HI_DP_MIR_PORT_CPU_USER_E,

}HI_DP_PORT_E;
 */

/* 以太网端口枚举定义 */
typedef enum
{
    HI_DP_UNI_ETH0_E,
    HI_DP_UNI_ETH1_E,
    HI_DP_UNI_ETH2_E,
    HI_DP_UNI_ETH3_E,
    HI_DP_UNI_ETH4_E,
    HI_DP_NNI_PON_E,
    HI_DP_UNI_CPU_E,
    HI_DP_UNI_PRBS_E,
    HI_DP_NNI_GE_E,
} HI_DP_PORT_E;

/* 以太端口MII模式 */
typedef enum
{
    HI_DP_ETH_GMII_E = 0,       /* GMII */
    HI_DP_ETH_SGMII_E,       /* SGMII */
    HI_DP_ETH_RGMII_E,          /* RMII / RGMII */
    HI_DP_ETH_DUAL_RGMII_E,     /* 双RGMII模式 */
    HI_DP_ETH_SERDES_E,
    HI_DP_ETH_MII_E,
    HI_DP_ETH_RVMII_E,

    HI_DP_ETH_MII_MODE_NUM_E
} HI_DP_ETH_MII_MODE_E;

/* 双RGMII模式，主用RGMII口的选择 */
typedef enum
{
    HI_DP_MASTER_RGMII1_E,
    HI_DP_MASTER_RGMII2_E,

    HI_DP_MASTER_RGMII_NUM_E
} HI_DP_MASTER_RGMII_E;

//#if (defined (CONFIG_HISI_SD5115_AF_ASIC) && defined (CONFIG_HISI_SD5115_TYPE_H))
/* RGMII1的线序选择 */
typedef enum
{
    HI_DP_RGMII_LINE_SEQ0_E,
    HI_DP_RGMII_LINE_SEQ1_E,

    HI_DP_RGMII_LINE_SEQ_NUM_E
} HI_DP_RGMII_LINE_SEQ_E;
//#endif

/* 以太端口工作模式 */
typedef enum
{
    HI_DP_ETH_AUTO_NEG_E,       /* 自动协商 */

    HI_DP_ETH_HALF_10M_E,       /* 半双工10M */
    HI_DP_ETH_HALF_100M_E,      /* 半双工100M */
    HI_DP_ETH_HALF_1000M_E,     /* 半双工1000M */

    HI_DP_ETH_DUP_10M_E,        /* 全双工10M */
    HI_DP_ETH_DUP_100M_E,       /* 全双工100M */
    HI_DP_ETH_DUP_1000M_E,      /* 全双工1000M */
    HI_DP_ETH_TEST_E,      /* 测试模式*/

    HI_DP_ETH_WORK_MODE_NUM_E
} HI_DP_ETH_WORK_MODE_E;

/* 以太端口环回模式 */
typedef enum
{
    HI_DP_ETH_NO_LOOP_E = 0,   /* 正常模式 */

    HI_DP_ETH_IN_LOOP_E,        /* MAC内环回模式 */
    HI_DP_ETH_OUT_LOOP_E,       /* MAC外环回模式 */

    HI_DP_ETH_PHY_IN_LOOP_E,    /* PHY内环回模式 */
    HI_DP_ETH_PHY_OUT_LOOP_E,   /* PHY外环回模式 */

    HI_DP_ETH_LOOP_MODE_NUM_E
} HI_DP_ETH_LOOP_MODE_E;

/* 以太端口生成树模式 */
typedef enum
{
    HI_DP_ETH_NO_STP_E = 0,    /* 不支持 */
    HI_DP_ETH_STP_E,            /* 支持STP */
    HI_DP_ETH_RSTP_E,           /* 支持RSTP */
} HI_DP_ETH_STP_MODE_E;

/* 以太端口生成树状态 */
typedef enum
{
    HI_DP_ETH_STP_FORWARD_E = 0,    /* 转发 */
    HI_DP_ETH_STP_LEARN_E,          /* 学习 */
    HI_DP_ETH_STP_LISTEN_E,         /* 侦听 */
    HI_DP_ETH_STP_BLOCK_E,          /* 阻塞 */
    HI_DP_ETH_STP_CLOSE_E,          /* 关闭,丢弃 */
} HI_DP_ETH_STP_STATUS_E;

/* 以太端口的连接状态 */
typedef enum
{
    HI_DP_ETH_LINK_UP,
    HI_DP_ETH_LINK_DN
} HI_DP_ETH_LINK_STA_E;

/* 以太端口PHY支持的工作模式能力集，只读，写无效 */
typedef union
{
    struct
    {
        uint auto_neg : 1;        /* [0] */
        uint half_10M : 1;        /* [1] */
        uint half_100M : 1;       /* [2] */
        uint half_1000M : 1;      /* [3] */

        uint full_10M : 1;        /* [4] */
        uint full_100M : 1;       /* [5] */
        uint full_1000M : 1;      /* [6] */
        uint reserved : 25;
    } bits;

    uint value;
} hi_dp_eth_work_capability_u;

/* 以太端口PHY的能力集, BASE_T/BASE_TX/BASE_T4/BASE_T2指支持物理网线类型 */
typedef union
{
    struct
    {
        uint half_10BASE_T : 1;        /* [0] */
        uint full_10BASE_T : 1;        /* [1] */
        uint half_100BASE_TX : 1;      /* [2] */
        uint full_100BASE_TX : 1;      /* [3] */
        uint full_100BASE_T4 : 1;      /* [4] */
        uint half_100BASE_T2 : 1;      /* [5] */
        uint full_100BASE_T2 : 1;      /* [6] */
        uint half_1000BASE_X : 1;      /* [7] */
        uint full_1000BASE_X : 1;      /* [8] */
        uint half_1000BASE_T : 1;      /* [9] */
        uint full_1000BASE_T : 1;      /* [10] */
        uint remote_fault : 1;       /* [11] */
        uint reserved : 20;
    } bits;

    uint value;
} hi_dp_eth_phy_capability_u;

/* 以太端口PHY支持的自动协商能力集 */
typedef union
{
    struct
    {
        uint half_10M : 1;        /* [0] */
        uint half_100M : 1;       /* [1] */
        uint half_1000M : 1;      /* [2] */

        uint full_10M : 1;        /* [3] */
        uint full_100M : 1;       /* [4] */
        uint full_1000M : 1;      /* [5] */
        uint reserved : 26;
    } bits;

    uint value;
} hi_dp_eth_auto_neg_capability_u;

/* 以太端口PHY支持的EEE能力集 */
typedef union
{
    struct
    {
        uint eee_100BASE_TX : 1;        /* [0] */
        uint eee_1000BASE_T : 1;        /* [1] */

        uint reserved : 30;
    } bits;

    uint value;
} hi_dp_eth_eee_capability_u;

#if 0

/* RXDMA对各种TAG类型的报文的处理方式 */
typedef enum
{
    HI_DP_IGR_TAG_THRU_E,                  /* tagged, pri-tagged, untagged */
    HI_DP_IGR_TAG_DROP_E,                  /* tagged, pri-tagged, untagged */
    HI_DP_IGR_TAG_ADD_DEFAULT_E,           /* tagged, pri-tagged, untagged */
    HI_DP_IGR_TAG_REP_OUTER_DEFAULT_E,     /* tagged, pri-tagged */
    HI_DP_IGR_TAG_REP_OUTER_TRANS_DROP_E,  /* tagged */
    HI_DP_IGR_TAG_REP_OUTER_TRANS_TRUE_E,  /* tagged */
} HI_DP_IGR_TAG_PRCS_E;

typedef enum
{
    HI_DP_EGR_TAG_THRU_E,       /* 透传 */
    HI_DP_EGR_TAG_STRIP_E,      /* 剥离 */
    HI_DP_EGR_TAG_CHECK_E,      /* 端口VLAN检查，等于端口默认VLAN时，剥离tag，否则透传 */
    HI_DP_EGR_TAG_SWITCH_E,     /* 切换VLAN，当tag等于端口默认VLAN时，剥离tag，
                                   否则，进行VID表的查询，当查询到有对应的VID的时候，
                                   切换成指定的VID，如果没有，则透传 */
    HI_DP_EGR_TAG_HYBRID_E,     /* Hybrid模式：查看tag的属性，若为untag，则剥离tag，若为tag，则透传 */

    HI_DP_EGR_TAG_DROP_E,         /* 对于出口方向上的DROP在MCD模块实现 */
    HI_DP_EGR_TAG_DROP_DEFAULT_E,
} HI_DP_EGR_TAG_PRCS_E;
#endif

/* TPID选择 */
typedef enum
{
    HI_DP_TPID_NONE_E,
    HI_DP_TPID_IDX1_E,
    HI_DP_TPID_IDX2_E,
    HI_DP_TPID_IDX3_E,
} HI_DP_TPID_SEL_E;

/* 优先级 */
typedef enum
{
    HI_DP_S8021P_PRI_E = 0,         /* 外 */
    HI_DP_C8021P_PRI_E,             /* 内 */
    HI_DP_IPTOS_PRI_E,
    HI_DP_EFC_MARK_PRI_E,
    HI_DP_GEM_PORT_PRI_E,
    HI_DP_REP_S8021P_PRI_E,         /* 替换后的外 */
    HI_DP_REP_C8021P_PRI_E,
    HI_DP_NO_CARE_PRI_E,            /* 不关注 */
} HI_DP_PRI_MODE_E;

/* VLAN */
typedef enum
{
    HI_DP_SVLAN_E = 0,              /* 外 */
    HI_DP_CVLAN_E,                  /* 内 */
    HI_DP_EFC_MARK_VLAN_E,
    HI_DP_REP_SVLAN_E,              /* 替换后的外 */
    HI_DP_REP_CVLAN_E,
    HI_DP_NO_CARE_VLAN_E,           /* 不关注 */
} HI_DP_VLAN_MODE_E;

typedef enum
{
    HI_DP_VLAN_NO_ACTION = 0,

    /* VLAN 是内还是外, 由全局配置决定 */
    HI_DP_VLAN_TO_POINT,        /* VLAN替换为指定VLAN */
    HI_DP_VLAN_TO_ORIGINAL_S,   /* VLAN替换为原始外层VLAN */
    HI_DP_VLAN_TO_ORIGINAL_C,   /* VLAN替换为原始内层VLAN */
} HI_DP_VALN_ACT_E;

/*
无动作，
1：修改值为指定802。1p。
2：修改值为报文原外层802。1p。
3：修改值为报文原内层802。1p，如果原报文只有一层tag，则修改值为报文的原外层802。1p
4：修改值为IPTOS，如果报文非IP报文，则修改值为报文原外层802。1p
5：修改值为下行GEM映射，如果报文非下行，则无动作。

 */
typedef enum
{
    HI_DP_8021P_NO_ACTION = 0,
    HI_DP_8021P_TO_POINT,
    HI_DP_8021P_TO_ORIGINAL_S,
    HI_DP_8021P_TO_ORIGINAL_C,
    HI_DP_8021P_TO_IPTOS,
    HI_DP_8021P_TO_DN_GEM_MAP,
} HI_DP_8021P_ACT_E;

typedef enum
{
    HI_DP_TAG_NO_ACTION_E = 0,
    HI_DP_TAG_ADD_E,
    HI_DP_TAG_DEL_E,
    HI_DP_TAG_DROP_E,
    HI_DP_TAG_DEFAULT_DROP_E,
    HI_DP_TAG_REP_OUTER_DEFAULT_E,
    HI_DP_TAG_REP_OUTER_TRANS_DROP_E,
    HI_DP_TAG_REP_OUTER_TRANS_TRUE_E,
    HI_DP_TAG_CHECK_E,
    HI_DP_TAG_SWITCH_E,
    HI_DP_TAG_HYBRID_E,
    HI_DP_TAG_ACT_NUM_E,
} HI_DP_TAG_ACT_E;

/* BEGIN: Deleted by shengjing, 2010/10/16   问题单号:SD5113V200编码*/
/* 修改行为 */
typedef enum
{
    HI_DP_EDIT_NO_ACTION = 0,
    HI_DP_EDIT_ADD,
    HI_DP_EDIT_DEL,
    HI_DP_EDIT_REP,
} HI_DP_EDIT_ACT_E;
/* END: Deleted by shengjing, 2010/10/16   问题单号:SD5113V200编码*/

/* 统一一个转发行为枚举,用于FLOW, SPECIAL PKT, L2, L3等有转发配置的相关模块 */
typedef enum
{
    HI_DP_TRANS_FWD_E = 0,
    HI_DP_TRANS_DROP_E,
    HI_DP_TRANS_TOCPU_E,
    HI_DP_TRANS_TOCPU_AND_FWD_E,
    HI_DP_TRANS_TOPOINT_E,
    HI_DP_TRANS_THRU_E,

    HI_DP_TRANS_ROUTE_NAT_E, /*保留废弃*/
    HI_DP_TRANS_NAPT_E = 7,

    HI_DP_TRANS_TOCPU_AND_THRU_E,
    HI_DP_TRANS_BC_E,
    HI_DP_TRANS_ROUTE_E,             /* L3路由转发 */
    HI_DP_TRANS_TOCPU_AND_ROUTE_E,
    HI_DP_TRANS_POLICY_ROUTE_E,      /* 策略路由，待确认拼写 */

    /* BEGIN: Added by shengjing, 2011/9/10   PN:5115新增*/
    HI_DP_TRANS_BC_BY_VLAN_E,     /* 仅在vlan内广播，不考虑vlan切换 */
    HI_DP_TRANS_MC_LIST_E,             /* 组播链表转发；同5113 */
    HI_DP_TRANS_MC_PORTMAP_E,          /* 组播端口转发；5115新增 */
    /* END:   Added by shengjing, 2011/9/10 */

    HI_DP_TRANS_RESERVED_E,            /* 保留行为 */
} HI_DP_TRANS_ACT_E;

/* BEGIN: Added by shengjing, 2011/12/30   PN:接口梳理*/
/* 匹配模式 */
typedef enum
{
    HI_DP_NORMAL_MATCH_MODE_E = 0,
    HI_DP_NON_MATCH_MODE_E,

    //for efc
    HI_DP_EQUAL_MODE_E, /* == */
    HI_DP_GREATER_THAN_AND_EQUAL_MODE_E,  /* >= */
    HI_DP_LESS_THAN_AND_EQUAL_MODE_E,  /* <= */
    HI_DP_GREATER_THAN_MODE_E,  /* > */
    HI_DP_LESS_THAN_MODE_E,  /* < */
    HI_DP_NOT_EQUAL_MODE_E, /* != */
} HI_DP_MATCH_MODE_E;

/* END:   Added by shengjing, 2011/12/30 */

typedef enum
{
    HI_DP_ALARM_DISAPPEAR_E = 0,
    HI_DP_ALARM_OCCUR_E,
} HI_DP_ALARM_STATE_E;

/* BEGIN: Added by shengjing, 2010/10/21   问题单号:SD5113V200新增*/

/*端口类型*/
typedef enum
{
    HI_DP_UNI = 0,
    HI_DP_NNI,

    /*其他待扩展*/
} HI_DP_PORT_TYPE_E;

/*字段*/
typedef enum
{
    HI_DP_DMAC_E = 0,
    HI_DP_SMAC_E,

    HI_DP_DIP_E = 5,
    HI_DP_SIP_E = 6,

    /*其他待扩展*/
} HI_DP_FIELD_E;

/* IP版本 */
typedef enum
{
    HI_DP_IP_V4_E = 0,
    HI_DP_IP_V6_E,

    HI_DP_IP_VERSION,
} HI_DP_IP_VERSION_E;

/* 组播模式选择吗 */
typedef enum
{
    HI_DP_MAC_MC_E = 0,    /* 多播MAC  L2组播 */
    HI_DP_IP_MC_E,        /* 多播IP L3组播 */
} HI_DP_MC_MODE_E;

/* END:   Added by shengjing, 2010/10/21 */

/* BEGIN: Added by shengjing, 2011/9/6   PN:5115新增*/
/* tpid修改模式选择 */
typedef enum
{
    HI_DP_TPID_NO_ACTION_E = 0,
    HI_DP_TPID_0_E,
    HI_DP_TPID_1_E,
    HI_DP_TPID_2_E,
} HI_DP_TPID_REP_MODE_E;

/* 报文颜色 */
typedef enum
{
    HI_DP_COLOR_NO_E = 0,
    HI_DP_COLOR_GREEN_E,
    HI_DP_COLOR_YELLOW_E,
    HI_DP_COLOR_RED_E,
} HI_DP_COLOR_E;

/* 方向 */
typedef enum
{
    HI_DP_POSITIVE_E,   /* 正 */
    HI_DP_NEGATIVE_E    /* 负 */
} HI_DP_DIR_E;

/* ETH_TYPE */
typedef enum
{
    RESERVED_E = 0,
} HI_DP_ETH_TYPE_E;

/* L4 protocol */
typedef enum
{
    HI_DP_L4_TCP_E = 0,
    HI_DP_L4_UDP_E,
} HI_DP_L4_PROTOCOL_E;

/* GEPORT TYPE */
typedef enum
{
    HI_DP_MC_GEMPORT_E = 0,
    HI_DP_UC_AND_BC_GEMPORT_E,

    HI_DP_MC_GEMPORT_TYPE,
} HI_DP_MC_GEMPORT_TYPE_E;
/* END:   Added by shengjing, 2011/9/6 */

typedef union
{
    struct
    {
        uint eth0 : 1;
        uint eth1 : 1;
        uint eth2 : 1;
        uint eth3 : 1;
        uint eth4 : 1;
        uint pon : 1;
        uint cpu : 1;
        uint prbs : 1;
        uint tcont0 : 1;
        uint tcont1 : 1;
        uint tcont2 : 1;
        uint tcont3 : 1;
        uint tcont4 : 1;
        uint tcont5 : 1;
        uint tcont6 : 1;
        uint tcont7 : 1;
        uint llid0 : 1;
        uint llid1 : 1;
        uint llid2 : 1;
        uint llid3 : 1;
        uint llid4 : 1;
        uint llid5 : 1;
        uint llid6 : 1;
        uint llid7 : 1;

        uint reserved : 8;
    } bits;

    uint value;
} hi_dp_port_u;

typedef union
{
    struct
    {
        uint bit0 : 1;
        uint bit1 : 1;
        uint bit2 : 1;
        uint bit3 : 1;
        uint bit4 : 1;
        uint bit5 : 1;
        uint bit6 : 1;
        uint bit7 : 1;
        uint bit8 : 1;
        uint bit9 : 1;
        uint bit10 : 1;
        uint bit11 : 1;
        uint bit12 : 1;
        uint bit13 : 1;
        uint bit14 : 1;
        uint bit15 : 1;
        uint bit16 : 1;
        uint bit17 : 1;
        uint bit18 : 1;
        uint bit19 : 1;
        uint bit20 : 1;
        uint bit21 : 1;
        uint bit22 : 1;
        uint bit23 : 1;
        uint bit24 : 1;
        uint bit25 : 1;
        uint bit26 : 1;
        uint bit27 : 1;
        uint bit28 : 1;
        uint bit29 : 1;
        uint bit30 : 1;
        uint bit31 : 1;
    } bits;

    uint value;
} hi_dp_mask_u;

typedef struct
{
    uint ui_tpid;

    uint           ui_vlan;
    HI_DP_ENDIAN_E em_cfi;
    uint           ui_pri;
} hi_dp_tag_s;

typedef struct
{
    uint           ui_vlan;
    uint           ui_pri;
    HI_DP_ENDIAN_E em_cfi;
} hi_dp_tci_s;

typedef struct
{
    /* 调整:需要考虑发送无限TIMES的情况,是否定为 0即可表达 */
    /* 对发送PLOAM帧有关的参数 */
    uint ui_send_times;

    /* ADD 增加操作同步开关 */
    HI_BOOL_E em_urgen_en;
    HI_BOOL_E em_force_en;
    HI_BOOL_E em_sync_en;

    /* PLOAM帧内容 */
    uchar uc_data[HI_DP_PLOAM_MSG_LEN];
} hi_ploam_message_s;

typedef struct
{
    /* ADD 增加操作同步开关 */
    HI_BOOL_E em_sync_en;
    uint      ui_length;           /* 消息长度 */

    uchar *puc_msg;
} hi_omci_message_s;

/* MPCP报文类型 */
typedef enum
{
    HI_DP_EPON_OLT_SCB_MSG_E = 0,   /* 表示OLT广播消息 */
    HI_DP_EPON_ONU_SCB_MSG_E,       /* 表示ONU广播消息 */     
    HI_DP_EPON_SNOOP_MSG_E,         /* 侦听模式下侦听到的消息 */          
    HI_DP_EPON_LLID_MSG_E,          /* 发给LLID的消息 */              
 }HI_DP_EPON_MSG_TYPE_E;

/* BEGIN: Added by shengjing, 2011/2/24   PN:DTS2011021605342*/
/* TOS模式选择；TC or DSCP */
typedef enum
{
    HI_DP_TOS_DSCP_E = 0,
    HI_DP_TOS_TC_E,
} HI_DP_TOS_MODE_E;
/* END:   Added by shengjing, 2011/2/24 */

/*BEGIN: Added by w00180499, 2011/5/11*/
/*为xPON双模自适应功能接口定义数据结构*/
typedef enum
{
    HI_DP_PORT_EPON_E = 0,
    HI_DP_PORT_GPON_E,
    HI_DP_PORT_UNKNOWN_E,
} HI_DP_NNI_PORT_E;
#ifdef CONFIG_HISI_SD5115_MP_PILOT

/*tocpu报文优先级替换方式*/
typedef enum
{
    HI_DP_PRI_DEFAULT_E = 0,
    HI_DP_PRI_REMAP_E,
} HI_DP_PRI_TYPE_SELECT_E;
#endif


/* 5115特殊报文类型；作为报文行为表查表索引 */
typedef enum{
 
    HI_DP_SPECIAL_PKT_PPPOE_DIS_E = 0,   /* 0x0: PPPoE Discovery */
    HI_DP_SPECIAL_PKT_DHCPV6_E,          /* 0x1: DHCPV6 */
    HI_DP_SPECIAL_PKT_ICMPV6_RS_E,       /* 0x2: ICMPv6 Router Soliciation */
    HI_DP_SPECIAL_PKT_ICMPV6_RA_E,       /* 0x3: ICMPv6 Router Advertisement */
    HI_DP_SPECIAL_PKT_ICMPV6_NS_E,       /* 0x4: ICMPv6 Nerghbor Soliciation */ 
    HI_DP_SPECIAL_PKT_ICMPV6_NA_E,       /* 0x5: ICMPv6 Nerghbor Advertisement */ 
    HI_DP_SPECIAL_PKT_ICMPV6_RM_E,       /* 0x6: ICMPv6 Redirect Message */ 
    HI_DP_SPECIAL_PKT_ICMPV6_PING_REQ_E, /* 0x7: ICMPv6 Echo Equest */ 
    HI_DP_SPECIAL_PKT_ICMPV6_PING_REPLY_E, /* 0x8: ICMPv6 Echo Reply */
    HI_DP_SPECIAL_PKT_ICMPV6_UNREACH_E,    /* 0x9: ICMPv6 Destination Unreachable */
    HI_DP_SPECIAL_PKT_ICMPV6_GB_E,         /* 0xA: ICMPv6 Packet Too Long */ /* 分组太大差错报文 */
    HI_DP_SPECIAL_PKT_ICMPV6_OVER_TIME_E,  /* 0xB: ICMPv6 Time Exceeded */   /* 超时差错报文 */
    HI_DP_SPECIAL_PKT_ICMPV6_PARA_ERR_E,   /* 0xC: ICMPv6 Parameter Problem */ /* 参数错误报文 */
    HI_DP_SPECIAL_PKT_MLD_QUERY_V1_V2_IPOE_E,    /* 0xD: MLD Query over IPoE */
    HI_DP_SPECIAL_PKT_MLD_REPORT_V1_IPOE_E,      /* 0xE: MLDv1 Report over IPoE */
    HI_DP_SPECIAL_PKT_MLD_REPORT_V2_IPOE_E,      /* 0xF: MLDv2 Report over IPoE */
    HI_DP_SPECIAL_PKT_MLD_DONE_IPOE_E,           /* 0x10: MLD Done over IPoE */
    HI_DP_SPECIAL_PKT_MLD_QUERY_V1_V2_PPPOE_E,   /* 0x11: MLD Query over PPPoE */
    HI_DP_SPECIAL_PKT_MLD_REPORT_V1_PPPOE_E,     /* 0x12: MLDv1 Report over PPPoE */
    HI_DP_SPECIAL_PKT_MLD_REPORT_V2_PPPOE_E,     /* 0x13: MLDv2 Report over PPPoE */
    HI_DP_SPECIAL_PKT_MLD_DONE_PPPOE_E,           /* 0x14: MLD Done over PPPoE */    
    HI_DP_SPECIAL_PKT_ARP_E,    /* 0x15: ARP */         
    HI_DP_SPECIAL_PKT_DHCP_E,   /* 0x16: DHCP */ 
    HI_DP_SPECIAL_PKT_ICMP_E,   /* 0x17: ICMP */
    HI_DP_SPECIAL_PKT_IGMP_PPPOE_E,   /* 0x18: IGMP over PPPoE */    
    HI_DP_SPECIAL_PKT_IGMP_IPOE_E,    /* 0x19: IGMP over IPoE */    
    HI_DP_SPECIAL_PKT_RARP_E,         /* 0x1A: RARP */
    HI_DP_SPECIAL_PKT_PPPOE_LCP,      /* 0x1B: PPP LCP */
     HI_DP_SPECIAL_PKT_PPPOE_IPCP,     /* 0x1C: PPP IPCP */ 
    HI_DP_SPECIAL_PKT_PPPOE_CHAP,     /* 0x1D: PPP CHAP */ 
    HI_DP_SPECIAL_PKT_PPPOE_PAP,      /* 0x1E: PPP PAP */ 
    HI_DP_SPECIAL_PKT_PPPOE_IPV6CP,   /* 0x1F: PPP IPv6CP */ 

#ifdef  CONFIG_HISI_SD5115_TYPE_T
    HI_DP_SPECIAL_PKT_1AG_E,
#endif

    /* 以下无优先级仲裁 */
    HI_DP_SPECIAL_PKT_POINT_TYPE0_E,  /* 4种自定义ETH Type */
    HI_DP_SPECIAL_PKT_POINT_TYPE1_E,
    HI_DP_SPECIAL_PKT_POINT_TYPE2_E,
    HI_DP_SPECIAL_PKT_POINT_TYPE3_E,

    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_0_E,   /* 3种用户自定义协议类型；PPPOE_UD_ULPT12、3配置 */
    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_1_E,
    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_2_E,

    HI_DP_SPECIAL_PKT_IPV6_EXTERN_HEADER_E,               /* 带扩展头的IPv6报文 */
    HI_DP_SPECIAL_PKT_BPDU_E,               
    HI_DP_SPECIAL_PKT_EAPOL_E,  

    
    HI_DP_SPECIAL_PKT_NUM,
    
}HI_DP_SPECIAL_PKT_TYPE_E;

typedef struct
{
    HI_LEVEL_E em_rx_sd_active;    /*接收检查有效电平*/
    HI_LEVEL_E em_tx_en_active;    /*发送有效电平*/
} hi_dp_glb_optic_module_attr_s;
/*END: Added by w00180499, 2011/5/11*/

/* MPCP报文结构 */
typedef struct
{
    /* 属于哪个LLID */
    uint ui_llid_index;

    /* 强制发送 */
    HI_BOOL_E em_force_en;

    /* 接收相关参数 */
    HI_DP_EPON_MSG_TYPE_E em_msg_type;

    /* MPCP帧内容 由MPCP协议软件进行解析 */
    uchar uc_data[HI_DP_MPCP_MSG_LEN];
    
} hi_mpcp_message_s;

typedef struct
{
    uint ui_llid_index; /* LLID索引 */
    HI_BOOL_E em_force_en;     /*强制发送，使能则对指定LLID进行flush*/
    HI_DP_EPON_MSG_TYPE_E em_msg_type; /* 接收相关参数 */
    uint ui_length;     /*发送OAM消息长度*/
    uchar *puc_msg;    /*OAM协议净荷头指针,长度为ui_msg_len*/
} hi_oam_message_s;

/* MAC地址 */
typedef struct
{
    uchar mac[HI_DP_MAC_BYTE_NUM];
    uchar resreved[2];
} hi_mac_s;

typedef struct
{
    uint   mac_l;
    ushort mac_h;
    ushort reserved;
} hi_mac_int_s;

/* BEGIN: Added by shengjing, 2011/5/25   SDK优化  *//*告警上报模式*/
typedef enum
{
    HI_DP_ALARM_REAL_TIME_REPORT_E = 0,  /*实时上报*/
    HI_DP_ALARM_DELAY_REPORT_E,          /*延迟上报*/
    HI_DP_ALARM_POLL_REPORT_E,           /*轮询上报*/
    HI_DP_ALARM_ONE_TIME_REPORT_E,       /*一次上报*/
} HI_DP_ALARM_REPORT_MODE_E;

/*时钟切换状态机取值范围*/
typedef enum
{
    HI_DP_LOSS_STATE_E = 0,  /*状态: 已丢失*/
    HI_DP_RESUME_STATE_E,    /*状态: 已恢复*/
} HI_DP_CLK_SWITCH_STATE_E;

/*告警属性 */
typedef struct
{
    /* 告警使能*/
    HI_BOOL_E em_enable;

    /*上报模式*/
    HI_DP_ALARM_REPORT_MODE_E em_report_mode;

    /*参数*/
    uint ui_arg;
} hi_dp_alarm_attr_s;

typedef struct
{
    HI_BOOL_E    em_enable;
    uint         ui_gid;
    uint         ui_pri;
    uint         ui_tcont;
    uint         ui_gemport;
    HI_DP_PORT_E em_sport;
    HI_DP_PORT_E em_dport;
} hi_dp_mirror_s;

typedef struct
{
    hi_dp_mirror_s st_igr_mir;
    hi_dp_mirror_s st_egr_mir;
} hi_dp_mirror_attr_s;

/* 端口流控相关属性 */
typedef struct
{
    HI_BOOL_E em_fctl_en;      /* true:选择BM流控，并且使能, em_fctl_en 与em_car_en只有一个为true flase:关闭BM流控 */

    uint ui_high_thr;           /* 端口占用缓存资源的流控阈值上限。
                                当缓存占用数量超过该阈值时启动流控帧发送
                                 */
    uint ui_low_thr;            /* 端口占用缓存资源的流控阈值下限。
                                当缓存占用数量低于该阈值时撤销流控帧发送。
                                 */

    HI_BOOL_E em_car_en;      /* true:选择CAR流控并且使能 ,em_fctl_en 与em_car_en只有一个为true flase:关闭CAR流控  */
    HI_BOOL_E em_car_pause_en;/* 在CAR流控时是否使能PAUSE帧功能 问题单DTS2011072605802 chenzetian 20110728 */

    uint ui_ebs;             /* 最大流量尺寸，超出PBS帧丢弃，单位为64kbps，1byte/125us */
    uint ui_cbs;             /* 承诺突发尺寸，单位为64kbps，1byte/125us */
    uint ui_cir;             /* 峰值带宽     */
    uint ui_dps;             /* 流控水线 */

    /* 无法只体现两个高低水线, 因为下边这个方程有无数解 */

    /*
        --------------a 反压水线
     |  dps1
        --------------cbs
     |  dps0
        --------------b 反压撤销水线


        cbs + dps1 = a;
        cbs - dps0 = b;

        在已知a和b的情况下无法求出dps0,dps1,cbs.
     */

#if 1
    uint ui_dps0;            /* 流控尺寸增量, 反压水线 */
    uint ui_dps1;            /* 流控尺寸增量, 撤销反压水线 */
#endif
} hi_dp_eth_fctl_attr_s;

/* 端口TAG相关属性 */
#if 0
typedef struct
{
    HI_DP_IGR_TAG_PRCS_E em_igr_tagged_prcs;     /* 0 ~ 5 */
    HI_DP_IGR_TAG_PRCS_E em_igr_pri_tagged_prcs; /* 0 ~ 3 */
    HI_DP_IGR_TAG_PRCS_E em_igr_untagged_prcs;   /* 0 ~ 2 */
} hi_dp_igr_tag_prcs_s;

typedef struct
{
    HI_DP_EGR_TAG_PRCS_E em_egr_tagged_prcs;
    HI_DP_EGR_TAG_PRCS_E em_egr_pri_tagged_prcs;
    HI_DP_EGR_TAG_PRCS_E em_egr_untagged_prcs;
} hi_dp_egr_tag_prcs_s;

typedef struct
{
    hi_dp_igr_tag_prcs_s st_igr_tag_prcs;
    hi_dp_egr_tag_prcs_s st_egr_tag_prcs;

    hi_dp_tag_s st_default_tag;
} hi_dp_eth_tag_attr_s;
#endif

typedef struct
{
    HI_DP_TAG_ACT_E em_tagged_act;
    HI_DP_TAG_ACT_E em_pri_tagged_act;
    HI_DP_TAG_ACT_E em_untagged_act;
} hi_dp_tag_act_s;

typedef struct
{
    hi_dp_tag_act_s st_igr_tag_act;
    hi_dp_tag_act_s st_egr_tag_act;
    hi_dp_tag_s     st_default_tag;
} hi_dp_tag_attr_s;

typedef struct
{
    HI_DP_ETH_STP_MODE_E em_stp_mode;
    hi_dp_mirror_attr_s  st_mirror_attr;
} hi_dp_uni_attr_s;

/* RXDMA/TXDMA 端口TAG切换表项 */
typedef struct
{
    HI_BOOL_E em_vld;

    HI_BOOL_E em_pri_match_en;
    uint      ui_pri_match_value;
    HI_BOOL_E em_pri_rep_en;
    uint      ui_pri_rep_value;

    HI_DP_TPID_SEL_E em_tpid_match_sel;
    HI_DP_TPID_SEL_E em_tpid_rep_sel;

    HI_BOOL_E em_vid_match_en;
    uint      ui_vid_match_value;
    HI_BOOL_E em_vid_rep_en;
    uint      ui_vid_rep_value;
} hi_dp_tag_sft_s;

#if 0
typedef struct
{
    HI_DP_PORT_E    em_uni_port;
    uint            ui_index;               /* 0 ~ 15 */
    hi_dp_tag_sft_s st_igr_sft;
    hi_dp_tag_sft_s st_egr_sft;
} hi_dp_uni_tag_sft_item_s;

typedef struct
{
    uint            ui_index;               /* 0 ~ 15 */
    hi_dp_tag_sft_s st_igr_sft;
    hi_dp_tag_sft_s st_egr_sft;
} hi_dp_nni_tag_sft_item_s;
#endif

typedef struct
{
    HI_DP_PORT_E    em_port;
    uint            ui_index;               /* 0 ~ 15 */
    hi_dp_tag_sft_s st_igr_sft;
    hi_dp_tag_sft_s st_egr_sft;
} hi_dp_tag_sft_item_s;

/* TXDMA hybrid查询表项 */
typedef struct
{
    uint ui_vlan;

    HI_BOOL_E uni0_tag_del_en;
    HI_BOOL_E uni1_tag_del_en;
    HI_BOOL_E uni2_tag_del_en;
    HI_BOOL_E uni3_tag_del_en;
    HI_BOOL_E uni4_tag_del_en;

    //HI_BOOL_E   nni_tag_del_en;
    HI_BOOL_E cpu_tag_del_en;
} hi_dp_uni_hybrid_item_s;

/* TXDMA hybrid查询表项 */
typedef struct
{
    uint      ui_vlan;
    HI_BOOL_E nni_tag_del_en;
} hi_dp_nni_hybrid_item_s;

#endif /* end of  */
