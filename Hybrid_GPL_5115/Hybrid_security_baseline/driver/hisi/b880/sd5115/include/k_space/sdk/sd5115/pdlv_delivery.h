/*******************************************************************************
  文 件 名   : 
  版 本 号   : 
  作    者   :   
  生成日期   : 
  功能描述   : 报文分发模块对外接口
  修改历史   : 
*******************************************************************************/
#ifndef _PDLV_INTERFACE_H
#define _PDLV_INTERFACE_H

#include "hi_drv_common.h"

#include <linux/skbuff.h>


/*******************************************************************************
  常量定义
*******************************************************************************/
/* 通用定义 */
#define PDLV_COMMON_VALID               1           /* 表示有效               */
#define PDLV_COMMON_INVALID             0           /* 表示无效               */
#define PDLV_MACADDR_LEN                6           /* MAC地址长度            */

/* handler函数的返回值, 该值决定模块处理完数据包后的动作 */
#define PDLV_PACKET_TO_CUSTOM           1           /* 指定转发               */
#define PDLV_PACKET_TO_NORMAL           2           /* 正常转发               */
#define PDLV_PACKET_TO_KERNEL           3           /* 转发到协议栈           */
#define PDLV_PACKET_TO_DROP             4           /* 丢弃数据包             */
#define PDLV_PACKET_TO_NEXT             5           /* 跳过处理下一数据包 */


/* 物理端口编号 */
#define PDLV_ETHPORT_GE0                0           /* GE0 / FE0              */
#define PDLV_ETHPORT_GE1                1           /* GE1 / FE1              */
#define PDLV_ETHPORT_FE2                2           /* GE2                    */
#define PDLV_ETHPORT_FE3                3           /* GE3                    */
#define PDLV_ETHPORT_FE4                4           /* GE3                    */
#define PDLV_ETHPORT_PON                5           /* PON                    */
#define PDLV_ETHPORT_CPU                6           /* CPU                    */
#define PDLV_ETHPORT_MAX                7           /* 端口数目最大值         */


/*错误码*/
#define PDLV_SUCCESS 0

/*发包返回值*/
#define PDLV_REGISTER_FAULT          1
#define PDLV_TX_FAULT                2
#define PDLV_INVALID_PARAMETER       3
#define PDLV_MODE_ERROR              4

/*问题单AB3D06690，s00149918,2009/11/04*/
#define PDLV_ETH_MAX_LENGTH     2020
#define PDLV_PRIPORITY_MAX      7
#define PDLV_ETH_PORTMASK_MAX   31
#define PDLV_TCONT_MAX          7
#define PDLV_GEMPORT_MAX        4095
#define PDLV_LLID_MAX           7
#ifdef CONFIG_HISI_SD5115_TYPE_T
#define PDLV_PSPORT_FLAG_MAX   1
#define PDLV_ORG_IGR_MAX       7
#endif

#define PORT_PON 5
/* END:   Modified by shengjing, 2009/2/19 */

/* BEGIN: Modified by shengjing, 2009/3/19   增加gpon/epon模式选择*/
#define PDLV_GPON_MODE 1
#define PDLV_EPON_MODE 2

/* BEGIN: Modified by shengjing, 2009/3/6   描述:增加llid和gemport参数*/
#define PDLV_INVALID_LLID_GEMPORT 0xFFFFFFFF


/*******************************************************************************
  数据结构
*******************************************************************************/
/*------------------------------------------------------------------------------
  报文解析
------------------------------------------------------------------------------*/
/* 以下所有结构成员为主机(小)字节序 */
/* TAG */
struct pdlv_taghdr_s
{
    unsigned int    valid;                          /* 标识该结构是否有效     */
    unsigned short  tpid;                           /* TAG 标记               */
    unsigned short  priority;                       /* 优先级                 */
    unsigned short  cfi;                            /* CFI                    */
    unsigned short  vlan;                           /* VLAN                   */
};

/* MAC 地址 */
struct pdlv_machdr_s
{
    unsigned char         dmac[PDLV_MACADDR_LEN];   /* 目的MAC                */
    unsigned char         smac[PDLV_MACADDR_LEN];   /* 源MAC                  */
    struct pdlv_taghdr_s  tag_ex;                   /* 外层TAG                */
    struct pdlv_taghdr_s  tag_in;                   /* 内层TAG                */
    unsigned short        eth_proto_len;            /* 以太网协议/长度类型    */
    unsigned short        reserved;                 /* 保留未用               */
    unsigned int          hdr_len;                  /* 以太网报文头长度       */
};

struct pdlv_pkt_s
{
    unsigned int                ingress;            /* 报文来源端口           */
    unsigned int                reason;             /* 报文提交CPU的原因      */

    unsigned int                llid;               /*llid*/
    unsigned int                gemport;            /*gemport*/

    struct pdlv_machdr_s        mac_hdr;            /* MAC 头部               */
};
/* END: Modified by shengjing, 2009/3/6*/

/*------------------------------------------------------------------------------
  报文统计
------------------------------------------------------------------------------*/
/* 分发模块收发报文的总数 */
struct pdlv_stat_common_s
{
    unsigned long  rx_total;                        /* 接收报文总数           */
    unsigned long  tx_total;                        /* 嗅探到转发报文的总数   */
};

/* 报文丢弃计数 */
struct pdlv_stat_dropping_s
{
    unsigned long  dropping_flag;                   /* 报文携带丢弃标识       */
    unsigned long  length_less60;                   /* 报文小于60字节         */
    unsigned long  module_feedback;                 /* 原因为802.1ag的非法报文*/
};

/* 按报文分发原因进行计数 */
struct pdlv_stat_reason_s
{
    unsigned long  normal_to_cpu;                   /* 正常转发给CPU的报文    */
    unsigned long  efc0_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc1_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc2_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc3_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc4_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc5_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc6_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc7_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc8_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc9_matched;                    /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc10_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc11_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc12_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc13_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc14_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  efc15_matched;                   /* 自定义EFC匹配交CPU原因 */
    unsigned long  unmatched_napt;                  /* 为NAPT进行EFC匹配失败  */
    unsigned long  untag;                           /* UNTAG报文              */
    unsigned long  vlan_tag;                        /* VLAN TAG报文           */
    unsigned long  priority_tag;                    /* PRI UNTAG报文          */
    unsigned long  broadcast;                       /* 广播报文               */
    unsigned long  igmp_pppoe;                      /* 组播报文               */
    unsigned long  igmp_ipoe;                       /* 组播报文               */
    unsigned long  arp;                             /* ARP报文                */
    unsigned long  rarp;                            /* RARP报文               */
    unsigned long  pppoe_discovery;                 /* PPPOE DISCOVERY报文    */
    unsigned long  eapol;                           /* EAPOL报文              */
    unsigned long  bpdu;                            /* BPDU报文               */
    unsigned long  dhcp;                            /* DHCP报文               */
    //unsigned long  ipv6;                            /* IPV6报文               */
    unsigned long  ICMP;                            /* ICMP报文               */
    unsigned long  custom_type0;                    /* 自定义捕获的报文       */
    unsigned long  custom_type1;                    /* 自定义捕获的报文       */
    unsigned long  custom_type2;                    /* 自定义捕获的报文       */
    unsigned long  custom_type3;                    /* 自定义捕获的报文       */
    //unsigned long  pppoe_session;                   /* PPPOE SESSION报文      */
    unsigned long  port_moved;                      /* 静态表项的端口发生移动 */
    unsigned long  mac_learn;                       /* 报文提交给CPU学习      */
    unsigned long  fail_to_learn;                   /* 逻辑学习报文失败       */
    unsigned long  fail_to_search;                  /* 逻辑查表失败           */
    unsigned long  succ_to_search;                  /* 逻辑查表成功           */
    unsigned long  napt_accel_on;                   /* NAPT加速开启时首包提交 */
    //unsigned long  napt_accel_off;                  /* NAPT加速关闭时报文提交 */
    unsigned long  unmatched_napt_outnet;           /* NAPT的OUTNET表不匹配   */
    unsigned long  oam8023ah;                       /* 802.3ah报文            */
    unsigned long  oam8021ag_y1731;                 /* 802.1ag / Y.1731报文   */
    //unsigned long  pdelay_req;                      /* pdelay_req报文         */
    //unsigned long  pdelay_resp;                     /* pdelay_resp报文        */
    //unsigned long  pdelay_resp_follow_up;           /* pdelay_resp_follow_up报文 */
    //unsigned long  pdelay_management;               /* management报文         */
    //unsigned long  pdelay_signaling;                /* signaling报文          */
    //unsigned long  pdelay_announce;                 /* announce报文           */
    unsigned long  pdelay_other_1588v2;             /* 其他1588协议报文       */
    //unsigned long  down_fail_to_cpu;                /* NAPT下行查表失败交CPU  */
    unsigned long  ip_fragment;                     /* IP分片报文     */

    /* V200增加 c00133939 */
    //unsigned long tunnel_unknow_mc;                 /* 隧道未知组播报文交CPU */
    //unsigned long ipv6_unknow_mc;                   /* IPv6未知组播报文交CPU */
    //unsigned long ipv4_unknow_mc;                   /* IPv4未知组播报文交CPU */
    
    unsigned long napt_egr_is_cpu;                  /* NAPT加速到用户口（比如WLAN） */
    unsigned long rt_to_gw_if;                      /* 路由到网关接口 */
    unsigned long l2_fwd_gw_if;                     /* L2转发到网关接口 */
    unsigned long efc_specify_to_wlan;              /* EFC指定转发到WLAN */
    unsigned long ipv6_unknow_extern_header;        /* IPV6非知名扩展头 */
    unsigned long ipv6_sip_site_local;              /* IPv6源IP为Site Local */
    unsigned long ipv6_sip_0;                       /* IPV6 SIP等于0 */
    unsigned long ipv6_sip_link_local;              /* IPv6源IP为Link Local地址 */
    unsigned long pppoe_session_lcp;                /* PPPOE  Session阶段的LCP报文 */
    unsigned long pppoe_session_ncp;                /* PPPOE Session阶段的NCP报文 */
    unsigned long pppoe_session_chap;               /* PPPOE Session阶段的CHAP报文 */
    unsigned long pppoe_session_pap;                /* PPPOE Session阶段的PAP报文 */
    unsigned long pppoe_session_ipv6cp;             /* PPPOE Session阶段的IPV6CP报文 */
    unsigned long pppoe_session_point_0;            /* PPPOE Session阶段的用户自定义类型0报文 */
    unsigned long pppoe_session_point_1;            /* PPPOE Session阶段的用户自定义类型1报文 */
    unsigned long pppoe_session_point_2;            /* PPPOE Session阶段的用户自定义类型2报文 */
    unsigned long dhcpv6;                           /* DHCPV6报文 */
    unsigned long icmpv6_rs;                        /* ICMPV6 RS报文 */
    unsigned long icmpv6_ra;                        /* ICMPV6 RA报文 */
    unsigned long icmpv6_ns;                        /* ICMPV6 NS报文 */
    unsigned long icmpv6_na;                        /* CMPV6 NA报文 */
    unsigned long icmpv6_rm;                        /* ICMPV6 RM报文 */
    unsigned long icmpv6_ping_req;                  /* ICMPV6 PingReq报文 */
    unsigned long icmpv6_ping_reply;                /* ICMPV6 PingReply报文 */
    unsigned long icmpv6_unreach;                   /* ICMPv6目的不可达差错报文 */
    unsigned long icmpv6_group_big;                 /* ICMPv6分组太大报文 */
    unsigned long icmpv6_outtime;                   /* ICMPv6超时差错报文 */
    unsigned long icmpv6_param_error;               /* ICMPv6参数错误报文 */
    unsigned long ipoe_mld_query_v1_v2;             /* IPOE承载的MLD Query v1/v2报文 */
    unsigned long ipoe_mld_report_v1;               /* IPOE承载的MLD Report v1报文 */
    unsigned long ipoe_mld_report_v2;               /* IPOE承载的MLD Report v2报文 */
    unsigned long ipoe_mld_done;                    /* IPOE承载的MLD DONE报文 */
    unsigned long pppoe_mld_query_v1_v2;            /* PPPOE承载的MLD Query v1/v2报文 */
    unsigned long pppoe_mld_report_v1;              /* PPPOE承载的MLD Report v1报文 */
    unsigned long pppoe_mld_report_v2;              /* PPPOE承载的MLD Report v2报文 */
    unsigned long pppoe_mld_done;                   /* PPPOE承载的MLD DONE报文 */
    unsigned long ipv6_extern_header_num_larger_1;  /* 带扩展头的IPV6报文（MLD等协议报文除外） */
    unsigned long header_laregr_128_byte;           /* 报文头长度超过128B */
    unsigned long rt_fwd_cpu_uni;                   /* 路由转发到CPU用户口 */
    unsigned long hl_ttl_less_1;                    /* HOP_LIMIT/TTL小于等于1捕获报文 */
    unsigned long rt_fail;                          /* 路由失败（无默认路由） */
    unsigned long black_hole_rt;                    /* 拒绝路由 */
    unsigned long no_next_hop;                      /* 无下一跳 */
    unsigned long in_tunnel_mtu_fail;               /* 入隧道MTU检查失败 */
    unsigned long bc;                               /* 广播报文 */
    unsigned long uc;                               /* 未知单播报文 */
    unsigned long mc;                               /* 已知组播报文转发到CPU用户口 */
    unsigned long sipv6_loop;                       /* SIPv6等于环回地址 */
    unsigned long sipv6_mc;                         /* SIPv6等于组播地址 */
    unsigned long sipv4_0;                          /* SIPv4等于0 */
    unsigned long sipv4_loop;                       /* SIPv4等于环回地址 */
    unsigned long sipv4_mc;                         /* SIPv4等于组播地址 */

    unsigned long rtcopycpu;                         /* 路由+拷贝CPU */
    unsigned long naptcopycpu;                         /* NAPT+拷贝CPU */
    unsigned long snaplarger1500;                         /* SNAP长度超过1500 */
};

/* 统计报文分发 */
struct pdlv_stat_delivery_s
{
    unsigned long  to_voip;                         /* 分发到 VOIP 模块       */
    unsigned long  to_module;                       /* 分发到处理模块         */
    unsigned long  to_kernel;                       /* 分发到内核             */
};

/*发包统计*/
struct pdlv_stat_transmit_s
{
    unsigned long to_eth;
    unsigned long to_gpon;
    unsigned long to_epon;
    unsigned long to_forward;
};

/* 报文统计 */
struct pdlv_stat_total_s
{
    struct pdlv_stat_common_s   common;             /* 分发模块收发报文的总数 */
    struct pdlv_stat_dropping_s dropping;           /* 报文丢弃计数           */
    struct pdlv_stat_reason_s   reason;             /* 按报文分发原因进行计数 */
    struct pdlv_stat_delivery_s delivery;           /* 统计报文分发           */

    /* BEGIN: Modified by shengjing, 2009/3/11   增加发包统计*/
    struct pdlv_stat_transmit_s transmit;
};

/*发送数据包到eth*/
typedef struct
{
    unsigned char *pData;
    unsigned int  length;
    unsigned int  portmask;
    unsigned int  priority;
#ifdef CONFIG_HISI_SD5115_TYPE_T
	unsigned int psportflag;
    unsigned int orgigr;
#endif
}pdlv_sendto_eth_s;

/*发送数据包到gpon*/
typedef struct
{
    unsigned char *pData;
    unsigned int  length;
    unsigned int  gemport;
    unsigned int  tcont;
    unsigned int  priority;
#ifdef CONFIG_HISI_SD5115_TYPE_T
	unsigned int psportflag;
	unsigned int orgigr;
#endif

}pdlv_sendto_gpon_s;

/*发送数据包到epon*/
typedef struct
{
    unsigned char *pData;
    unsigned int  length;
    unsigned int  llid;
    unsigned int  priority;
#ifdef CONFIG_HISI_SD5115_TYPE_T
	unsigned int psportflag;
	unsigned int orgigr;
#endif

}pdlv_sendto_epon_s;

/*cpu正常转发数据包*/
typedef struct
{
    unsigned char *pData;
    unsigned int  length;
#ifdef CONFIG_HISI_SD5115_TYPE_T
	unsigned int psportflag;
	unsigned int orgigr;
#endif

}pdlv_sendto_forward_s;

typedef enum{
    UP_GE_FE0 = 0,
    UP_GE_FE1,
    UP_FE2,
    UP_FE3,
    UP_FE4,
    UP_PON,
    UP_CPU,
    UP_USE,
} UP_PORT_E;

/*******************************************************************************
  回调函数类型
*******************************************************************************/
/*
    参数: 
    @ 输入: 数据包缓冲区地址 
    @ 输入: 数据包长
    @ 输入: 数据包解析后的信息
    @ 输入: 额外数据(通常为 struct sk_buff *)
    @ 输出: 转发使用的 gemport (如果需要转发报文到 PON, 包括单播/组播/广播)
    @ 输出: 转发使用的 tcont   (如果需要转发报文到 PON, 包括单播/组播/广播)
*/
typedef int (*pdlv_function_handler)(unsigned char *,
                                     unsigned int,
                                     const struct pdlv_pkt_s *,
                                     void *);

/*******************************************************************************
  接口声明
*******************************************************************************/
int pdlv_register_function(unsigned int function, pdlv_function_handler handler);
int pdlv_remove_function(unsigned int function);

int pdlv_query_statistics(struct pdlv_stat_total_s *stats);
int pdlv_clear_statistics(void);
#ifdef CONFIG_HISI_SD5115_TYPE_T
int pdlv_sendto_epon(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority, unsigned int psportflag, unsigned int orgigr);
int pdlv_sendto_gpon(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority, unsigned int psportflag, unsigned int orgigr);
int pdlv_sendto_eth(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority, unsigned int psportflag, unsigned int orgigr);

int pdlv_sendto_epon_ex(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority, unsigned int timestamp_add, unsigned int psportflag, unsigned int orgigr);
int pdlv_sendto_gpon_ex(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority, unsigned int timestamp_add, unsigned int psportflag, unsigned int orgigr);
int pdlv_sendto_eth_ex(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority, unsigned int timestamp_add, unsigned int psportflag, unsigned int orgigr);
int pdlv_sendto_forward(unsigned int psportflag, unsigned int orgigr, unsigned char *data, unsigned int length);
#else
/* BEGIN: Modified by shengjing, 2009/2/19   增加发包接口*/
int pdlv_sendto_epon(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority);
int pdlv_sendto_gpon(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority);
int pdlv_sendto_eth(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority);

int pdlv_sendto_epon_ex(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_gpon_ex(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_eth_ex(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_forward(unsigned char *data, unsigned int length);
#endif



/* END:   Modified by shengjing, 2009/2/19 */


//s00149918 将原有的eth中的注册函数 移到pdlv
/*模块注册码*/
#define ETH_FUNC_RX_WAN       (0)    /*收包方向wandev注册的函数号 */
#define ETH_FUNC_RX_CAP       (1)    /*收包方向捕获注册的函数号 */
#define ETH_FUNC_RX_PDLV      (2)    /*收包方向pdlv注册的函数号 */

#define ETH_FUNC_TX_CAP       (0)     /*收包方向捕获注册的函数号 */

#define ETH_RX_HANDLER_MAX    (3)
#define ETH_TX_HANDLER_MAX    (1)

typedef int (*pdlv_module_handler)(struct sk_buff *);
extern int pdlv_rx_remove_function(unsigned int function);


extern int pdlv_rx_register_function(unsigned int function, pdlv_module_handler handler);


extern int pdlv_tx_remove_function(unsigned int function);


extern int pdlv_tx_register_function(unsigned int function, pdlv_module_handler handler);

int  pdlv_set_up_port(UP_PORT_E up_port);
UP_PORT_E pdlv_get_up_port(void);

#endif
