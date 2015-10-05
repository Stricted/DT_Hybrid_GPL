/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_l3.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2009年2月4日
  最近修改   :
  功能描述   : L3 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年2月4日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus  */

#ifndef __HI_DRV_DP_L3_H__
 #define __HI_DRV_DP_L3_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define  HI_DP_L3_HASH_ITEM_NUM 16
  #define  HI_DP_L3_HASH_BUCKET_NUM 1024
  #define  HI_DP_L3_HASH_TABLE_ITEM_NUM 16384
 #endif
 #define  HI_DP_L3_REP_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_REPLACE_TABLE_ITEM_NUM 256

 #define  HI_DP_L3_REP_OP_TIMES 4                          /* CPU每配置一条流，都要操作表项四次 */
 #define  HI_DP_L3_REP_OP_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_REP_OP_TABLE_ITEM_NUM_V200 256

 #define  HI_DP_L3_EX_OUTNET_FILTER_NUM 8
 #define  HI_DP_L3_EX_OUTNET_FILTER_TABLE_ITEM_NUM 64

 #define  HI_DP_L3_NAPT_TABLE_ITEM_NUM (8 * 1024)

 #define  HI_DP_L3_NH_TABLE_ITEM_NUM 128
 #define  HI_DP_L3_NH_SUBTABLE_ITEM_NUM 64
 #define  HI_DP_L3_RT_TABLE_ITEM_NUM 64
 #define  HI_DP_L3_ITRT_TABLE_ITEM_NUM 8
 #define  HI_DP_L3_OTRT_TABLE_ITEM_NUM 8
 #define  HI_DP_L3_RT_ACT_TABLE_ITEM_NUM 256
 #define  HI_DP_L3_TSCT_TABLE_ITEM_NUM 64

 #define  HI_DP_L3_TUNNEL_HEAD_LEN_MAX 64
 #define  HI_DP_L3_TUNNEL_MTU_MAX HI_BITS_LOW_11
 #define  HI_DP_L3_TUNNEL_TTL_DEC_MAX HI_BITS_LOW_8

 #define  HI_DP_L3_6RD_TUNNEL_HEAD_LEN_MAX 20

 #define  HI_DP_L3_IPV6_CNT_TABLE_ITEM_NUM 8

 #define  HI_DP_L3_IPV4_MASK_RANGE_MAX 32
 #define  HI_DP_L3_IP_MASK_RANGE_MAX 128

 #define HI_DP_L3_RT_LABEL_NUM 16
 #define HI_DP_L3_IPV6_TUNNEL_NUM 8
 #define HI_DP_L3_IPV6_STAT_NUM 16

 #define HI_DP_L3_RT_MTU_MAX HI_BITS_LOW_12
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define HI_DP_L3_NAPT_AGE_NUM 16
  #define HI_DP_L3_NAPT_LINK_CNT_PERIOD_MAX 7 // 2^7=128S
 #endif

/* NAPT操作中支持的IP协议类型 */
typedef enum
{
    HI_DP_L3_TCP_PROTO_E = 0, /* */
    HI_DP_L3_UDP_PROTO_E,
} HI_DP_L3_IP_PROTO_E;

/* ipv4/v6 报文sip检查 */
typedef struct
{
    HI_DP_TRANS_ACT_E em_loop_act;          /* v4/v6 */
    HI_DP_TRANS_ACT_E em_mc_act;            /* v4/v6 */
    HI_DP_TRANS_ACT_E em_site_local_act;    /* only v6 */
    HI_DP_TRANS_ACT_E em_link_local_act;    /* only v6 */
    HI_DP_TRANS_ACT_E em_all_0_act;         /* v4/v6 */
} hi_dp_l3_sip_check_s;

/* ipv6统计索引配置 */
typedef struct
{
    HI_BOOL_E    em_valid;
    HI_BOOL_E    em_vlan_en;
    uint         ui_vlan;
    HI_BOOL_E    em_port_en;         /* 仅最后一项需要配置；其余port均为有效 */
    HI_DP_PORT_E em_port;
} hi_dp_l3_ip_stat_attr_s;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT

typedef union
{
    struct
    {
        uint fin : 1;
        uint syn : 1;
        uint rst : 1;
        uint ack : 1;

        uint reserved0 : 28;
    } bits;

    uint value;
} hi_dp_l3_tcp_ctrl_u;

/* 带OPTION报文控制 */
typedef struct
{
    HI_DP_TRANS_ACT_E em_ip_option_act;     /* 控制带Option IP报文做NAPT时的动作。0:交CPU；1:丢弃。*/
    HI_DP_TRANS_ACT_E em_ip_frag_act;       /* 控制 IP分片报文做NAPT时的动作。0:交CPU；1:丢弃。*/

    HI_DP_TRANS_ACT_E   em_tcp_act;         /* TCP报文控制位动作的报文动作控制，00:正常转发；01:复制一份交CPU；10~11;转交CPU。 */
    hi_dp_l3_tcp_ctrl_u u_tcp_ctrl_bit;     /*TCP报文控制位掩，bit[0],bit[1], bit[2]分别对应fin,syn,rst的掩码*/
} hi_dp_l3_ip_option_ctrl_s;
 #endif

typedef enum
{
    HI_DP_WAN_ID_SEL_WAN_E,
    HI_DP_WAN_ID_SEL_RSP_E,
} HI_DP_WAN_ID_SEL_E;

typedef enum
{
    HI_DP_RT_MODE_IPV4_E,
    HI_DP_RT_MODE_DAUL_STACK_E,
    HI_DP_RT_MODE_IPV6_E,
    HI_DP_RT_MODE_UNKNOWN_E
} HI_DP_RT_MODE_E;
typedef struct
{
    uint ui_rt_num;
    uint ui_nh_num;
    uint ui_bwlt_num;
} hi_dp_l3_rt_spec_s;

/* napt配置参数 */
typedef struct
{
    HI_DP_TRANS_ACT_E em_napt_dn_unhit_act;  /* NAPT处理,下行查询SUBNET没有命中,则可配置为转CPU或者丢弃 */
    HI_DP_TRANS_ACT_E em_napt_up_unhit_act;  /* NAPT处理,上行查询SUBNET没有命中,则可配置为转CPU或者丢弃 */
    HI_BOOL_E         em_napt_age_en;
    uint              ui_napt_age_time;
    uint              ui_ipv6_point_next_header;  /* 为IPv6扩展预留的配置,知名扩展头,KNOWN_NEXT_HEADER */
    HI_BOOL_E         em_tunnel_sip_check_en;     /* 隧道报文SIP检查使能 */

    HI_DP_TRANS_ACT_E em_tunnel_nh_act[HI_DP_L3_IPV6_TUNNEL_NUM];/* 带扩展头的本地隧道处理动作 */
    HI_DP_TRANS_ACT_E em_mtu_over_act;            /* 添加隧道后MTU超过路由路径协商的MTU值的动作 */
    HI_DP_TRANS_ACT_E em_ttl_over_act;            /* HOP_LIMIT/TTL小于等于1时处理动作 */
    HI_DP_TRANS_ACT_E em_rt_unhit_act;            /*路由查表失败动作 */

    hi_dp_l3_sip_check_s st_ipv6_sip_check;
    hi_dp_l3_sip_check_s st_ipv4_sip_check;
    HI_DP_TRANS_ACT_E    em_ipv6_unknown_header_act; /* IPV6非知名扩展头检查 */

    /* 同一个端口隔离L3接口使能 */
    HI_BOOL_E em_igr_fitler_en;

    HI_DP_TRANS_ACT_E em_hov128_act;              /* cap:IP头超长报文 */

    /* 报文头超过128byte的普通路由报文，可以完成正常路由转发；
    隧道等有可能不能完成正常转发 */
    hi_dp_l3_ip_stat_attr_s st_ip_stat_attr[HI_DP_L3_IPV6_STAT_NUM];
    HI_DP_TRANS_ACT_E       em_ipv6_frag_act;     /* ipv6分片报文处理 */
    HI_DP_TRANS_ACT_E       em_6rd_frag_act;      /* 6RD分片报文处理 */
    HI_DP_TRANS_ACT_E       em_6rd_option_act;    /* 6RD option报文处理 */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    hi_dp_l3_ip_option_ctrl_s st_ip_option_ctrl;
    uint                      ui_napt_link_cnt_period;  /*napt流量统计周期配置，算法:2的ui_napt_link_cnt_period次方秒*/
 #endif

 #ifdef CONFIG_HISI_SD5115_TYPE_T
    HI_DP_WAN_ID_SEL_E em_wan_id_sel;/*0：选择路由DIP的WAN ID进行NAPT；
                                     1：选择RSP区分的RSP ID进行NAPT。*/
    HI_DP_RT_MODE_E    em_rt_mode; /*RT规格控制寄存器。
                                   0:2K条IPv4路由＋IPv4黑白名单过滤；
                                   1:1K条IPv4路由+IPv4黑白名单过滤和256条IPv6路由＋-
                                   IPv6黑白名单过滤
                                   其他:512条IPv6路由＋IPv6黑白名单过滤。*/
 #endif
} hi_dp_l3_attr_s;

typedef enum
{
    HI_DP_L3_NAPT_MATCH_MODE_3_TUPLES_E = 0,
    HI_DP_L3_NAPT_MATCH_MODE_4_TUPLES_E,
    HI_DP_L3_NAPT_MATCH_MODE_5_TUPLES_E,
} HI_DP_L3_NAPT_MATCH_MODE_E;

typedef enum
{
    HI_DP_L3_DIRECTION_UP_E = 0,
    HI_DP_L3_DIRECTION_DOWN_E,
} HI_DP_L3_DIRECTION_E;

typedef struct
{
    HI_DP_L3_NAPT_MATCH_MODE_E em_match_mode;

    HI_BOOL_E em_rt_lable_en;        /*路由标签匹配使能  IF_ID_EN*/
    uint      ui_rt_lable;           /*0-7              IF_ID*/

    HI_DP_L3_IP_PROTO_E em_proto;    /* 该Napt连接中允许通过的IP报文协议类型 */

    HI_BOOL_E em_dip_en;
    uint      ui_dip;
    HI_BOOL_E em_dport_en;
    uint      ui_dport;
    HI_BOOL_E em_sip_en;
    uint      ui_sip;
    HI_BOOL_E em_sport_en;
    uint      ui_sport;
} hi_dp_l3_napt_table_rule_item_s;

typedef struct
{
    HI_BOOL_E em_age_en;                  /* 静态标示位 */
    uint      ui_age;                     /* 表项年龄 */

    uint         ui_replace_index;
    HI_DP_PORT_E em_uni_port;             /* 用户侧端口信息;仅下行表有效 */
    uint         ui_replace_port;         /* NAPT替换的port；上行:SPORT;下行:DPORT */
} hi_dp_l3_napt_table_act_item_s;

typedef struct
{
    uint ui_bucket_index;                   /* hash桶索引      */
    uint ui_idle_num;                       /* 空闲表项的数目   */
    uint ui_item_index;                     /* 表项直接索引   */
} hi_dp_l3_napt_table_result_s;

typedef struct
{
    uint                            ui_index; /* 仅在直接访问时，有效 */
    HI_BOOL_E                       em_valid; /* 仅在直接访问时，有效,间接访问，不需要提供 */
    hi_dp_l3_napt_table_rule_item_s st_rule;
    hi_dp_l3_napt_table_act_item_s  st_act;
    hi_dp_l3_napt_table_result_s    st_result;
} hi_dp_l3_napt_table_item_s;

/*
NOP 无操作
REP_DMAC    替换DMAC值
REP_SMAC    替换SMAC值
REP_DIP 替换DIP
REP_SIP 替换SIP
REP_PPP 修改PPP域中的Session_id
DEL_PPP 删除PPP头
ADD_PPP 添加PPP头
 */

/* 替换行为定义 */
typedef enum
{
    HI_DP_L3_REP_NOP_E = 0,
    HI_DP_L3_REP_DMAC_E,     /* 替换DMAC值 */
    HI_DP_L3_REP_SMAC_E,     /* 替换SMAC值 */
    HI_DP_L3_REP_DIP_E,      /* 替换DIP */
    HI_DP_L3_REP_SIP_E,      /* 替换SIP */
    HI_DP_L3_REP_PPP_E,      /* 修改PPP域中的Session_id */
    HI_DP_L3_DEL_PPP_E,      /* 删除PPP头 */
    HI_DP_L3_ADD_PPP_E,      /* 添加PPP头 */
} HI_DP_L3_REP_ACT_E;

/*
0x0：TCP报文；
0x1：UDP报文；
0x2：TCP/UDP报文；
0x3：对IP报文的协议类型不做限制；
 */

/* BEGIN: Added by shengjing, 2010/10/19   问题单号:SD5113V200新增*/
/* 出/入隧道，方向 */
typedef enum
{
    HI_DP_L3_TUNNEL_IN_E = 0,   /*加头*/
    HI_DP_L3_TUNNEL_OUT_E       /*删头*/
} HI_DP_L3_TUNNEL_DIRECTION_E;

/* 隧道TC/FL替换行为 */
typedef enum
{
    HI_DP_L3_TUNNEL_REP_NO_ACTION_E = 0,    /* 无操作;除该操作外，其他操作对于FL都是用EFC指定值修改FL */
    HI_DP_L3_TUNNEL_REP_ORIGINAL_DSCP,      /* TC使用原始报文DSCP */
    HI_DP_L3_TUNNEL_REP_EFC_DSCP_E,         /* TC使用EFC指定值；若EFC没有指定，则使用原报文DSCP */
    HI_DP_L3_TUNNEL_REP_EFC_E,              /* TC使用EFC指定值；若EFC没有指定，则不进行替换 */
} HI_DP_L3_TUNNEL_REP_E;

typedef enum
{
    HI_DP_L3_PRI_REP_NO_ACTION_E,           /* 不替换 */
    HI_DP_L3_PRI_REP_DSCP_H_E,              /* 拷贝DSCP/TC的高3bit */
    HI_DP_L3_PRI_REP_POINT_E,               /* 使用指定值替换 */
} HI_DP_L3_PRI_REP_E;

typedef enum
{
    HI_DP_TUNNEL_HEAD_LEN_40B_E = 0,
    HI_DP_TUNNEL_HEAD_LEN_48B_E,
    HI_DP_TUNNEL_HEAD_LEN_56B_E,
    HI_DP_TUNNEL_HEAD_LEN_64B_E,
    HI_DP_TUNNEL_HEAD_LEN_20B_E,    /* for 6RD only */
} HI_DP_TUNNEL_HEAD_LEN_E;

typedef enum
{
    HI_DP_TUNNEL_TYPE_DSLITE_E = 0,
    HI_DP_TUNNEL_TYPE_6RD_E,
} HI_DP_TUNNEL_TYPE_E;

typedef enum
{
    HI_DP_TUNNEL_MAP_TYPE_STATIC_E = 0,   /* 静态映射 */
    HI_DP_TUNNEL_MAP_TYPE_DYNAMIC_E,      /* 动态映射 */
} HI_DP_TUNNEL_MAP_TYPE_E;

typedef struct
{
    HI_DP_L3_REP_ACT_E em_rep_act0;      /* 替换行为0 */
    HI_DP_L3_REP_ACT_E em_rep_act1;      /* 替换行为1 */

    uint   ui_value0;                    /* 替换行为0对应的参数 */
    ushort us_value1;                    /* 替换行为1对应的参数 16 */
    uchar  uc_mac[6];                    /* 当替换行为0为MAC时候,表示替换的MAC, 此时上述两个参数无效 */
} hi_dp_l3_rep_op_item_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_L3_MAC_SEL_1K_E = 0,
    HI_DP_L3_MAC_SEL_16K_E,
    HI_DP_L3_MAC_SEL_8K_E,
    HI_DP_L3_MAC_SEL_UNKNOWN_E,
} HI_DP_L3_MAC_SEL_E;
 #endif
typedef struct
{
    uint ui_index;     /* 0 - 255,SD5113V200 */

    HI_BOOL_E em_smac_rep_en;
    uchar     uc_smac[6];
    HI_BOOL_E em_dmac_rep_en;
    uchar     uc_dmac[6];
    HI_BOOL_E em_sip_rep_en;
    uint      ui_sip;
    HI_BOOL_E em_dip_rep_en;
    uint      ui_dip;

    /*PPP 行为*/
    HI_DP_EDIT_ACT_E em_ppp_act;
    uint             ui_session_id;

    HI_BOOL_E        em_ttl_en;         /* 保持HL标志 */
    HI_DP_EDIT_ACT_E em_tag_act;        /* TAG添加/删除；优先级高于EFC */

    HI_BOOL_E          em_svlan_rep_en;  /* 外层VLAN替换使能 */
    uint               ui_svlan;
    HI_DP_L3_PRI_REP_E em_spri_rep;      /* 外层PRI替换选择 */
    uint               ui_spri;
    HI_BOOL_E          em_cvlan_rep_en;  /* 内层VLAN替换使能 */
    uint               ui_cvlan;
    HI_DP_L3_PRI_REP_E em_cpri_rep;      /* 内层PRI替换选择 */
    uint               ui_cpri;

    HI_DP_TOS_MODE_E em_tos_mode;         /* 报文TOS域选择 */
    HI_BOOL_E        em_tos_rep_en;       /* 报文TOS修改使能 */
    uint             ui_tos;              /* 报文TOS修改值 */

    HI_DP_TPID_REP_MODE_E em_stpid_rep_mode;    /* 外层tpid修改模式 */
    HI_DP_TPID_REP_MODE_E em_ctpid_rep_mode;    /* 内层tpid修改模式 */

    /* 以下配置仅对6RD隧道有效 */
    HI_DP_TUNNEL_MAP_TYPE_E em_map_type;    /* 指示映射类型 */
    uint                    ui_v4_mask_len;
    uint                    ui_6rd_prefix_len;
} hi_dp_l3_replace_table_item_s;

 #if 0
typedef struct
{
    /*uint ui_index;                    在5115T中索引无效*/

    HI_BOOL_E em_valid;

    /*下一跳IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];     /* 目的ip地址，v4只有到4byte */
    HI_DP_IP_VERSION_E em_ip_version;                        /* 报文IP版本，v4 or v6 */

    uint      ui_mtu;
    uint      ui_rt_act_index; /*  路由行为表索引，芯片OIF1*/
    HI_BOOL_E em_rsp_en;
    uint      ui_rsp_id;
} hi_dp_l3_nh_table_item_s;

typedef struct
{
    /* uint ui_index;             0-63 */

    HI_BOOL_E em_valid;

    /*目的IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* 目的ip地址，v4只有到4byte */
    HI_DP_IP_VERSION_E em_ip_version;                    /* 报文IP版本，v4 or v6 */
    uint               ui_mask_range;                    /* 掩码范围  bit范围*/
    uint               ui_mtu;
    uint               ui_rt_act_index;                  /*  路由行为表索引，芯片OIF1*/

    HI_BOOL_E em_rsp_en;
    uint      ui_rsp_id;
} hi_dp_l3_rt_table_item_s;

 #endif/* t */

typedef struct
{
    uint ui_index;                   /* 0-127 */

    HI_BOOL_E em_valid;

    /*下一跳IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM];     /* 目的ip地址，v4只有到4byte */
    HI_DP_IP_VERSION_E em_ip_version;                        /* 报文IP版本，v4 or v6 */

    HI_BOOL_E em_rsp_en;    /* for 5115T only*/
    uint      ui_rsp_id;    /* for 5115T only*/

    uint ui_mtu;
    uint ui_rt_act_index;      /*  路由行为表索引，芯片OIF1*/
} hi_dp_l3_nh_table_item_s;

typedef struct
{
    uint ui_index;             /* 0-63 */

    HI_BOOL_E em_valid;

    /*目的IP*/
    uchar              uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* 目的ip地址，v4只有到4byte */
    HI_DP_IP_VERSION_E em_ip_version;                    /* 报文IP版本，v4 or v6 */
    uint               ui_mask_range;                    /* 掩码范围  bit范围*/

    HI_BOOL_E em_rsp_en;    /* for 5115T only*/
    uint      ui_rsp_id;    /* for 5115T only*/

    uint ui_mtu;
    uint ui_rt_act_index;                  /*  路由行为表索引，芯片OIF1*/
} hi_dp_l3_rt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    /*下一跳是否有效*/
    HI_BOOL_E em_nh_valid;

    /* 如果 em_nt_valid 为无效时的行为 */
    /* drop or to_cpu */
    HI_DP_TRANS_ACT_E em_nh_invalid_act;

    /*如果 em_nt_valid为有效，即则下面的行为有效*/
    /*HI_DP_TRANS_FWD_E,HI_DP_TRANS_DROP_E,HI_DP_TRANS_TOCPU_E,*/
    /*路由行为  普通路由   黑洞路由   拒绝路由*/
    HI_DP_TRANS_ACT_E em_rt_act;

    /*如果 为普通路由,则下面的配置有效*/
    HI_DP_PORT_E em_egr;       /* 转发端口0-6 */

    HI_BOOL_E em_napt_en;      /* 做napt标志 */
    HI_BOOL_E em_rt_label_en;  /* 路由标签匹配使能*/
    uint      ui_rt_label;     /* 用于NAPT，区分WAN口 if_id*/

    HI_BOOL_E em_tunnel_en;    /* 是否本地隧道；1:根据配置进行出/入隧道操作 */
    uint      ui_tunnel_index; /* 隧道表索引 */

    /*重点考虑  屏蔽了芯片内部的部分功能*/
    HI_BOOL_E em_tc_rep_en;    /*隧道TC替换使能*/
} hi_dp_l3_rt_act_table_item_s;

typedef struct
{
    /*组合索引*/
    uint ui_index;         /* 表项ID */

    HI_BOOL_E           em_valid;
    uchar               uc_dip[HI_DP_IPV6_ADDR_BYTE_NUM]; /* 目的ip地址 */
    uint                ui_rt_act_index;                  /*L3_REP,  路由行为表索引，芯片OIF1*/
    HI_DP_TUNNEL_TYPE_E em_tunnel_type;                   /* 仅对out_tunnel有效 */
    HI_BOOL_E           em_rsp_en; /* for 5115T only*//* 仅对out_tunnel有效 */
    uint                ui_rsp_id; /* for 5115T only*//* 仅对out_tunnel有效 */
} hi_dp_l3_tunnel_rt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    uchar uc_head[HI_DP_L3_TUNNEL_HEAD_LEN_MAX];

    /* 隧道头的长度，单位byte,ui_head_len = 40 + 芯片值 * 8 */
    HI_DP_TUNNEL_HEAD_LEN_E em_head_len_add;  /* 指定添加隧道头长度 */
    HI_DP_TUNNEL_HEAD_LEN_E em_head_len_del;  /* 指定删除隧道头长度 */
    /* 芯片T_LEN_ADD、T_LEN_DEL提供同一参数配置 */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    /* MPW并不支持，打开为了兼容，PILOT支持按照隧道来配置 fufenglian */
    uint ui_ip_mtu;            /* MTU为IP报文长度，不包含链路头长度 */
 #endif
    HI_BOOL_E         em_ttl_en;              /* 出隧道报文TTL是否减ui_ttl_dec标志 */
    uint              ui_ttl_step;            /* TTL的递减步长值 */
    HI_DP_TRANS_ACT_E em_tunnel_with_nh_act;  /* 带扩展头的隧道报文出隧道动作 */
} hi_dp_l3_tunnel_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;

    uchar     uc_head[HI_DP_L3_6RD_TUNNEL_HEAD_LEN_MAX];
    HI_BOOL_E em_hop_limit_en;     /* 出隧道报文HL是否减ui_ttl_dec标志 */
    uint      ui_hop_limit_step;   /* HL的递减步长值 */
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_ip_mtu;
 #endif
} hi_dp_l3_6rd_tunnel_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_valid;                      /* RT_BASED_CFG_REG2 */

    uchar uc_ip[HI_DP_IPV6_ADDR_BYTE_NUM];   /* 源IP地址 */
} hi_dp_l3_tunnel_sip_table_item_s;

/* IPv6统计，port + vlan 做索引 */
typedef struct
{
    HI_DP_PORT_E em_port;         /* 物理端口 */
    HI_BOOL_E    em_vlan_en;      /* 接口VLAN有效标识 */
    uint         ui_vlan;         /* 接口VLAN（含路由和桥接接口） */
    ulong        ul_frame_num;    /* IPv6 帧数目 */
    ulong        ul_byte_num;     /* IPv6 帧字节数 */
    ulong        ul_uc_frame_num; /* IPv6 单播帧数 */
    ulong        ul_mc_frame_num; /* IPv6 组播帧数 */
} hi_dp_l3_ipv6_cnt_table_item_s;

typedef struct
{
    uint      ui_index;
    HI_BOOL_E em_table_hit;
} hi_dp_l3_sta_s;

typedef struct
{
    /* rt */
    ulong ul_rx_frame;    /* 接收的帧数目统计 */
    ulong ul_rx_byte;     /* 接收的帧字节数统计 */
    ulong ul_rx_uc_frame; /* 接收的单播帧数统计 */
    ulong ul_rx_mc_frame; /* 接收的组播帧数统计 */

    ulong ul_tx_frame;    /* 发送的帧数目统计 */
    ulong ul_tx_byte;     /* 发送的帧字节数统计 */
    ulong ul_tx_uc_frame; /* 发送的单播帧数统计 */
    ulong ul_tx_mc_frame; /* 发送的组播帧数统计 */
} hi_dp_wan_cnt_item_s;

typedef struct
{
    uint                 ui_index; /* 0 - 15 */
    hi_dp_wan_cnt_item_s st_v4_cnt;
    hi_dp_wan_cnt_item_s st_v6_cnt;
} hi_dp_wan_cnt_s;

typedef struct
{
    //NAPT
    uint ui_dn_drop_cnt;
    uint ui_up_drop_cnt;
    uint ui_dn_tocpu_cnt;
    uint ui_up_tocpu_cnt;
    uint ui_frag_tocpu_cnt;
    uint ui_option_drop_cnt;
    uint ui_option_tocpu_cnt;
    uint ui_up_match_cnt;
    uint ui_dn_match_cnt;
    uint ui_frag_drop_cnt;
    uint ui_ctl_bit_tocpu_cnt;

    //RT
    uint ui_rt_tocpu_cnt;
    uint ui_rt_fwd_cnt;
    uint ui_rt_drop_cnt;
    uint ui_v4_sip_err_st;
    uint ui_v6_sip_err_st;
    uint ui_nni_ip_filt_cnt;
    uint ui_uni_ip_filt_cnt;
    uint ui_ipv6_unk_hdr_cnt;

    /* BEGIN: Added by l00164498, 2012/8/17 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint  ui_in_tnl_pkt_cnt[HI_DP_TNL_NUM];       /* 进隧道包数统计 */
    uint  ui_out_tnl_pkt_cnt[HI_DP_TNL_NUM];      /* 出隧道包数统计 */
    ulong ul_in_tnl_byt_cnt[HI_DP_TNL_NUM];      /* 进隧道字节统计 */
    ulong ul_out_tnl_byt_cnt[HI_DP_TNL_NUM];     /* 出隧道字节统计 */
    uint  ui_tnl_mtu_cnt[HI_DP_TNL_NUM];          /* 隧道的MTU丢弃报文统计寄存器 */
    uint  ui_tnl_drop_cnt[HI_DP_TNL_NUM];         /* 隧道的丢弃统计寄存器 */
 #endif
    /* END: Added by l00164498, 2012/8/17 */
} hi_dp_l3_cnt_s;

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_HASH_BUCKET_NUM0_E = 0,
    HI_DP_HASH_BUCKET_NUM1_E,
} HI_DP_L3_HASH_BUCKET_NUM_E;

typedef enum
{
    HI_DP_HASH_BUCKET_NORMAL_E = 0,
    HI_DP_HASH_BUCKET_ESCAPE_E,
} HI_DP_L3_HASH_BUCKET_TYPE_E;

typedef struct
{
    uint      ui_index;
    uint      ui_bucket_index;
    HI_BOOL_E em_valid;
} hi_dp_l3_napt_point_cnt_table_item_s;

  #define HI_DP_L3_POINT_BUCHET_NUM 64

typedef struct
{
    uint ui_index;            /* 统计索引，0~63 */

    uint ui_bucket_index;
    uint ui_min_item_index;   /* 最小表项，直接索引 */

    uint ui_cnt[16];          /* 该桶内每个表项的流量统计 */

    uint ui_item_index[16];   /* 该桶内每个表项的直接索引； */
} hi_dp_l3_napt_point_cnt_s;

typedef enum
{
    HI_DP_L3_NAPT_AGE_ALARM_E = 0,
    HI_DP_L3_ALARM_NUM_E,
} HI_DP_L3_ALARM_E;

/* napt地址结构 */

typedef struct
{
    uint ui_bucket_index;    /* hash桶索引 */
    uint ui_item_index;      /* 表项绝对地址 */
} hi_dp_l3_napt_addr_s;

/* 一次napt老化中断，上报的信息 */
typedef struct
{
    uint ui_age_num;   /* 本次中断，老化的桶个数；1~16 */

    hi_dp_l3_napt_addr_s st_addr[HI_DP_L3_NAPT_AGE_NUM];
} hi_dp_l3_napt_age_info_s;

/*L3模块告警属性 */
typedef struct
{
    /* 告警属性*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_L3_ALARM_NUM_E];

    void (*alarm_call_back)(HI_DP_L3_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_l3_alarm_attr_s;
 #endif

/* BEGIN: Added by l00164498, 2012/8/6 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T

typedef union
{
    struct
    {
        uint ge0 : 1;
        uint ge1 : 1;
        uint ge2 : 1;
        uint ge3 : 1;
        uint ge4 : 1;
        uint pon : 1;
        uint cpu : 1;
        uint rsv : 25;
    } bits;
    uint value;
} hi_dp_l3_port_u;

typedef struct
{
    uint               ui_rsp_id; //匹配的rsp id,0-7
    HI_BOOL_E          em_ethtype_en; //是否关注ETH_TYPE。
    HI_BOOL_E          em_pri_en; //是否关注优先级。
    HI_BOOL_E          em_vlan_en; //是否关注转发VLAN。
    HI_BOOL_E          em_ip_en; //是否关注IP。
    HI_DP_IP_VERSION_E em_ip_ver; //SIP IP版本
    uint               ui_ethtype; //匹配的eth type
    hi_dp_l3_port_u    u_port_mask; //0：表示不匹配对应端口；1：表示匹配对应的端口。
    uint               ui_vlan;
    uint               ui_pri;

    /* IP的掩码 ui_ip_mask
    0:不掩码；
    1:SIP最低1bit掩码；
    2:SIP最低2bit掩码；
    ......
    127:SIP最低127bit掩码；
    128:SIP全掩码，即不关注SIP；
    129~256：保留。*/
    uint  ui_ip_mask;
    uchar uc_ipv4[HI_DP_IPV4_ADDR_BYTE_NUM];
    uchar uc_ipv6[HI_DP_IPV6_ADDR_BYTE_NUM];
} hi_dp_l3_rsp_table_item_s;
 #endif
/* END: Added by l00164498, 2012/8/6 */

#endif /* end of __HI_DRV_DP_L3_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
