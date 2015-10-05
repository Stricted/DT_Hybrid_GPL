#include <linux/netdevice.h>
#include <linux/workqueue.h>

/******************************************************************************

                  版权所有 (C), 2011-2021, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_kernel_eth0.h
  版 本 号   : 初稿
  作    者   : y58968
  生成日期   : D2011_12_15
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_ETH0_H__
 #define __HI_KERNEL_ETH0_H__

 #define HI_KERNEL_NETWORK_SPEC_NUM 8

/*魔术字*/
 #define HI_KERNEL_NETWORK_MAGICAL 0x68776877
 #define HI_MAC_LEN 6


typedef struct __hi_drv_list_head
{
    struct __hi_list_head *next, *prev;
} hi_drv_list_head;

typedef enum
{
    HI_KERNEL_ETH0_VLANFLAG_UNTAG = 0,    /* untag vlan */
    HI_KERNEL_ETH0_VLANFLAG_STAG,         /* single vlan */
    HI_KERNEL_ETH0_VLANFLAG_CTAG,         /* double vlan */
} hi_kernel_eth0_vlanflag_e;

typedef struct
{
    unsigned int  ui_magic;     /*ui_magic = '5'<<24|'1'<<16|'1'<<8|'5' */ 
    unsigned char uc_groupid;   /*tx*/
    unsigned char uc_act;
    unsigned char uc_pri;
    unsigned char uc_port;        /*rx:ingress tx:engress*/

    unsigned char uc_reason;
    unsigned char uc_llid;        /*llid or tcontid*/
    unsigned char uc_resv;
    unsigned char uc_vlanflag;    /* 1: 一层HI_KERNEL_ETH0_VLANFLAG_STAG 2:二层HI_KERNEL_ETH0_VLANFLAG_CTAG*/

    unsigned short us_svlan;
    unsigned short us_cvlan;

    unsigned short us_gemport;
    unsigned short us_len;
} hi_kernel_eth0_cb_s;

typedef int (*hi_kernel_eth0_dev_rxcallback)( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb );

typedef struct
{
    unsigned char  uc_dmask[HI_MAC_LEN];
    unsigned char  uc_smask[HI_MAC_LEN];
    unsigned int   ui_dipmask;
    unsigned int   ui_sipmask;
    unsigned short us_dpmask;
    unsigned short us_spmask;
    unsigned int   ui_flagmask;
} hi_kernel_network_mappingmask_s;

typedef struct
{
    void   *pv_head;
    void   *pv_data;
    uint    ui_hdrlen;
    uint    ui_ifindex;
} hi_kernel_network_data_s;

typedef struct
{
    hi_drv_list_head                st_listhead;
    unsigned int                    ui_pkttype;
    hi_kernel_network_mappingmask_s st_mask;
} hi_kernel_network_spec_s;

typedef struct
{
    /*上行car动态数据*/
    uint ui_car_up_lasttime;      /*上次计算令牌时间*/
    uint ui_car_up_token;         /*当前令牌数,每发送一个报文加一,与模板数值比较*/
    uint ui_car_stat_up_green;    /*统计计数*/
    uint ui_car_stat_up_yellow;   /*统计计数*/
    uint ui_car_stat_up_red;      /*统计计数*/
    /*下行car动态数据*/
    uint ui_car_dn_lasttime;      /*上次计算令牌时间*/
    uint ui_car_dn_token;         /*当前令牌数,每发送一个报文加一,与模板数值比较*/
    uint ui_car_stat_dn_green;    /*统计计数*/
    uint ui_car_stat_dn_yellow;   /*统计计数*/
    uint ui_car_stat_dn_red;      /*统计计数*/
    /*上行pq动态数据*/
    uint ui_pq_up_lasttime;      /*上次计算令牌时间*/
    uint ui_pq_up_token_pri0;
    uint ui_pq_up_token_pri1;
    uint ui_pq_up_token_pri2;
    uint ui_pq_up_token_pri3;
    uint ui_pq_up_token_pri4;
    uint ui_pq_up_token_pri5;
    uint ui_pq_up_token_pri6;
    uint ui_pq_up_token_pri7;

    /*上行pq统计*/
    uint ui_pq_up_stat_pri0;
    uint ui_pq_up_stat_pri1;
    uint ui_pq_up_stat_pri2;
    uint ui_pq_up_stat_pri3;
    uint ui_pq_up_stat_pri4;
    uint ui_pq_up_stat_pri5;
    uint ui_pq_up_stat_pri6;
    uint ui_pq_up_stat_pri7;

    /*下行pq动态数据*/
    uint ui_pq_dn_lasttime;      /*上次计算令牌时间*/
    uint ui_pq_dn_token_pri0;
    uint ui_pq_dn_token_pri1;
    uint ui_pq_dn_token_pri2;
    uint ui_pq_dn_token_pri3;
    uint ui_pq_dn_token_pri4;
    uint ui_pq_dn_token_pri5;
    uint ui_pq_dn_token_pri6;
    uint ui_pq_dn_token_pri7;

    /*下行pq统计*/
    uint ui_pq_dn_stat_pri0;
    uint ui_pq_dn_stat_pri1;
    uint ui_pq_dn_stat_pri2;
    uint ui_pq_dn_stat_pri3;
    uint ui_pq_dn_stat_pri4;
    uint ui_pq_dn_stat_pri5;
    uint ui_pq_dn_stat_pri6;
    uint ui_pq_dn_stat_pri7;

    /*misc*/
    uint ui_napt_stat;
} hi_drv_network_traffic_stat_s;

typedef struct
{
    /*配置数据*/
    unsigned char  ui_mode;        /*0:系统根据典型CPU速率60%计算令牌数
                                   1:系统根据绝对令牌数进行调度*/
    unsigned char  uc_cpurate;     /*0..100 百分比*/
    unsigned short us_resv;
    unsigned int   ui_token;      /*需要系统真实测量最佳状态下的系统最大许可转发速率,单位 个/每秒*/
    /*动态数据*/
    unsigned int ui_drop;
    unsigned int ui_peektoken;
    unsigned int ui_curtoken;
    unsigned int ui_lasttime;
} hi_drv_network_unattack_s;

typedef struct  __hi_kernel_network_vif_s
{
    struct net_device                *pst_dev;         /*pointer to the net device*/
    struct net_device_stats st_stats;    /*the stats of the device*/
    struct net_device_stats st_devstats; /*the stats of the real device.e.g:eth0*/
    struct napi_struct napi;
    struct work_struct reset_task;
    struct __hi_kernel_network_vif_s *pst_fathervif;
    hi_drv_list_head st_rootlisthead;  /*next level node dev of sub interface*/
    hi_drv_list_head st_listhead;       /*this node*/
    hi_drv_list_head st_mappinglisthead;   /* 下挂所有配置的流映射*/
    uint             ui_magical; /*检查是否为虚拟接口创建的管理实体必须为 */

    /*映射属性*/
    uint ui_mappingtype;   /*作为管理节点/接口有效,激活是必须被配置*/

    unsigned char uc_matchmode;      /*匹配模式 HI_KERNEL_VIF_TRAFFIC_UNMATCH_FILTER*/
    unsigned char uc_mac[HI_MAC_LEN];

    unsigned char uc_vlanflag;  /*vlan处理动作*/
    unsigned char uc_priority;       /*接口调度优先级0低..7高,缺省0*/
    unsigned char uc_broadcast; /*是否广播到所有子接口*/
    unsigned char uc_resv; /*保留*/

    uint   ui_vlanmode;  /*当作为虚拟接口时,系统只能够使用TRUNK-0*/
    ushort us_defvlan;   /*缺省0为没有激活*/
    ushort us_deftpid;   /*缺省0x8100*/

    /*接口配置的基本属性*/
    uint ui_pridns;
    uint ui_secdns;
    uint ui_wins;

    void *                          pv_hashhead_mapping; /*hash表头节点,按照匹配规则索引*/
    void *                          pv_hashhead_spec; /*hash表头节点,按照匹配规则索引*/
    hi_kernel_network_mappingmask_s st_mask;

    /*接口配置的管理属性*/
    unsigned char uc_type;          /*0:管理节点/1:接口/2:子接口*/
    unsigned char uc_status;        /*接口状态0:关闭1:激活*/
    unsigned char uc_actionmode;        /*接收数据转发处理模式,缺省0:
                                        0:纯Linux软件转发
                                        1:软件加速转发 ＋ Linux转发
                                        2:硬件加速转发 ＋ Linux转发
                                        3:硬件加速转发 ＋软件加速转发 ＋ Linux转发
                                        4:虚拟接口之间桥接转发
                                         */
    unsigned char uc_actionflag;        /*接收数据转发处理动作,缺省0:
                                        0:转发给Linux;
                                        1:转发给软件加速*/

    unsigned char uc_enable8021x;   /*8021x使能0:关闭1:使能*/
    unsigned char uc_unkowncast;        /*未知单播处理方式1:转发0:丢弃*/
    unsigned char uc_unkownmultcast;    /*未知组播处理方式1:转发0:丢弃*/
    unsigned char uc_unkownbroadcast;   /*未知广播处理方式1:转发0:丢弃*/

    /*接口动态数据*/
    uint ui_activetime;    /*接口激活时刻*/

    /*流量模板*/
    unsigned char uc_car_up_indx;          /*流量调度模板索引,缺省0*/
    unsigned char uc_car_dn_indx;          /*流量调度模板索引,缺省0*/
    unsigned char uc_pq_up_indx;          /*流量调度模板索引,缺省0*/
    unsigned char uc_pq_dn_indx;          /*流量调度模板索引,缺省0*/

    /*流量统计*/
    hi_drv_network_traffic_stat_s st_trafficstat;

    /*防攻击*/
    hi_drv_network_unattack_s st_unattack;
    hi_kernel_network_spec_s  st_sepc[HI_KERNEL_NETWORK_SPEC_NUM];

    int (*pv_vifrecvcallback)(hi_kernel_eth0_cb_s * pst_cb, struct sk_buff *pst_skb,
                              struct __hi_kernel_network_vif_s *pst_vif);
    int (*pv_vifxmitcallback)(struct sk_buff *pst_skb,
                              struct __hi_kernel_network_vif_s *pst_vif);
    int (*pv_vifnaptcallback)(hi_kernel_network_data_s * puc_data, void *pst_cb, unsigned char uc_actionflag);
} hi_kernel_network_vif_s;

/*****************************************************************************/
extern struct net_device * hi_kernel_eth0_dev_create( char *ifname );
extern void                hi_kernel_eth0_dev_fillvif( hi_kernel_network_vif_s *pst_vif );
extern void                hi_kernel_eth0_dev_destroy( struct net_device *pst_dev );

extern void                hi_kernel_eth0_dev_removetag( struct sk_buff *pst_skb, ushort *pus_len);
extern int                 hi_kernel_eth0_dev_tolinux( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb,
                                                       struct net_device *pst_dev );
extern struct sk_buff *    hi_kernel_eth0_dev_skb(                                   hi_kernel_eth0_cb_s *pst_cb,
                                                                                     unsigned char *    puc_data,
                                                                                     struct net_device *pst_dev );

extern int                 hi_kernel_eth0_tx_specialsend( hi_kernel_eth0_cb_s *pst_cb, struct sk_buff *pst_skb );
extern int                 hi_kernel_eth0_tx_send( struct sk_buff *pst_skb );
extern void                hi_kernel_eth0_dev_rxhook_reg( hi_kernel_eth0_dev_rxcallback pv_rx_callback );
extern void                hi_kernel_eth0_dev_rxhook_unreg( void );
extern void                hi_kernel_eth0_dev_cb_enable(void);
extern void                hi_kernel_eth0_dev_cb_disable(void);

extern hi_kernel_eth0_dev_rxcallback g_pv_rx_pkt_callback;

/*****************************************************************************/

#endif  /* __HI_KERNEL_ETH0_H__ */
