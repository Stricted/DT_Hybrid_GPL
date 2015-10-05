/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_oam.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2009年2月4日
  最近修改   :
  功能描述   : OAM 模块数据结构头文件
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
#endif /*  __cplusplus  */

#ifndef __HI_DRV_DP_OAM_H__
 #define __HI_DRV_DP_OAM_H__

 #ifdef CONFIG_HISI_SD5115_MP_PILOT
  #define SUPPORT_OAM_1731
 #endif

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"
 #ifdef SUPPORT_OAM_1731
  #include "hi_drv_dp_prbs.h"
 #endif
 #define HI_DP_OAM_LLID_NUM 8

typedef enum
{
    HI_DP_OAM_PORT_LLID0_E = 0,
    HI_DP_OAM_PORT_LLID1_E,
    HI_DP_OAM_PORT_LLID2_E,
    HI_DP_OAM_PORT_LLID3_E,
    HI_DP_OAM_PORT_LLID4_E,
    HI_DP_OAM_PORT_LLID5_E,
    HI_DP_OAM_PORT_LLID6_E,
    HI_DP_OAM_PORT_LLID7_E,
    HI_DP_OAM_PORT_GE0_E,
    HI_DP_OAM_PORT_GE1_E,
    HI_DP_OAM_PORT_FE0_E,
    HI_DP_OAM_PORT_FE1_E,
    HI_DP_OAM_PORT_FE2_E,
} HI_DP_OAM_PORT_E;

 #define HI_DP_OAM_PORT_NUM 13

typedef enum
{
    HI_DP_OAM_ERR_FR_EVENT_E = 0,  /* 错帧事件     */
    HI_DP_OAM_ERR_FRP_EVENT_E,     /* 错帧周期事件 */
    HI_DP_OAM_ERR_FRS_EVENT_E,     /* 错帧秒事件   */
    HI_DP_OAM_NO_EVENT_E,
} HI_DP_OAM_EVENT_SEL_E;

typedef struct
{
    HI_DP_OAM_EVENT_SEL_E em_type;      /* 事件通告类型     */
    HI_BOOL_E             em_notify_en; /* 事件通告功能使能 */
    uint                  ui_window;    /* 错帧周期窗口     */
    uint                  ui_threshold; /* 错帧门限         */
} hi_dp_oam_3ah_event_attr_s;

typedef struct
{
    HI_DP_OAM_EVENT_SEL_E em_event;
    HI_BOOL_E             em_lock;             /* 端口的事件通告锁定状态     */
    uint                  ui_err_frame_num;    /* 指定的周期内统计到的错误帧 */
    uint                  ui_event_time_stamp; /* 发生该事件的时间           */
} hi_dp_oam_3ah_event_info_s;

typedef struct
{
    HI_BOOL_E em_enable[HI_DP_ETH_PORT_NUM];            /* 端口3ah使能,8个llid默认使能,不可配 */
    HI_BOOL_E em_local_lp_en[HI_DP_OAM_PORT_NUM];       /* 近端环回使能5个ETH口和8个LLID */

    HI_BOOL_E        em_remote_loop_en;                 /* 远端环回使能 */
    HI_DP_OAM_PORT_E em_remote_loop_port;               /* 远端环回端口 */
    HI_DP_PORT_E     em_remote_loop_test_port;          /* 远端环回使测试数据端口，如果测试端口与远端环回端口配置相同
                                                        则配置无效不能进行环回测试 */
    HI_BOOL_E        em_pri_rep_en[HI_DP_ETH_PORT_NUM + 1];/* 优先级替换使能 5个ETH口和PON口 */
    uint             ui_pri_rep[HI_DP_ETH_PORT_NUM + 1];    /* 替换优先级 5个ETH口和PON口 */

    HI_DP_OAM_PORT_E           em_event_notify_llid;    /* PON端口指定支持事件通告的LLID */
    hi_dp_oam_3ah_event_attr_s st_event_attr[HI_DP_ETH_PORT_NUM + 1];   /* 端口时间属性 5个ETH口和PON口 */
 #ifdef SUPPORT_OAM_1731
    HI_BOOL_E em_car_en[HI_DP_ETH_PORT_NUM + 1];        /* 基于端口配置的oam3ah报文流量抑制使能指示。 包括5个ETH口和PON口。 */

    HI_DP_TRANS_ACT_E em_trans_act[HI_DP_ETH_PORT_NUM + 1]; /*3ah未使能时，接收oam1ag帧的处理ACTION。包括5个ETH口和PON口。
                                                            PON端口接收的oam3ah帧处理ACTION。
                                                            HI_DP_TRANS_FWD_E：只正常转发；
                                                            HI_DP_TRANS_TOCPU_E：只转交CPU；
                                                            HI_DP_TRANS_DROP_E：丢帧过滤；
                                                            HI_DP_TRANS_TOCPU_AND_FWD_E：正常转发,且复制到CPU。

                                                            从EHT端口接收oam3ah帧的处理ACTION。
                                                            HI_DP_TRANS_FWD_E：只正常转发；
                                                            HI_DP_TRANS_TOCPU_E：只转交CPU；
                                                            HI_DP_TRANS_DROP_E：丢帧过滤；
                                                            HI_DP_TRANS_TOCPU_AND_FWD_E：正常转发,且复制到CPU；
                                                            HI_DP_TRANS_THRU_E：只透传到上行口；
                                                            HI_DP_TRANS_TOCPU_AND_THRU_E：透传到上行口，且复制到CPU；
                                                            其他：保留。
                                                            注意：透传相当于指定转发到上行口
                                                             */
 #endif
} hi_dp_oam_3ah_attr_s;

// VLAN SEL TYPE: FOR SD5115T ONLY
typedef enum 
{
    HI_DP_PRI_VLAN_SEL_TYPE_FWD_E,    
    HI_DP_PRI_VLAN_SEL_TYPE_ORG_S_E,   
    HI_DP_PRI_VLAN_SEL_TYPE_MODIFIED_E,   
}HI_DP_PRI_VLAN_SEL_TYPE_E;

typedef enum 
{
    HI_DP_GEMPORT_SEL_AUTO_E,    
    HI_DP_GEMPORT_SEL_SPECIFY_E,   
}HI_DP_GEMPORT_SEL_E;

typedef struct
{
    HI_BOOL_E em_enable[HI_DP_PORT_NUM];        /* 端口CFM使能 eth,pon,cpu*/
    HI_BOOL_E em_pri_rep_en[HI_DP_PORT_NUM];    /* 优先级替换使能 5个ETH口和EPON,CPU口 */
    uint      ui_pri_rep[HI_DP_PORT_NUM];       /* 替换优先级 5个ETH口和EPON,CPU口 */

    //for T
    HI_DP_PRI_VLAN_SEL_TYPE_E em_inward_vlan_sel[HI_DP_PORT_NUM]; /* 端口VLAN选择 eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_outward_vlan_sel[HI_DP_PORT_NUM];/* 端口VLAN选择 eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_inward_pri_sel[HI_DP_PORT_NUM]; /* 端口PRI选择 eth,pon,cpu*/
    HI_DP_PRI_VLAN_SEL_TYPE_E em_outward_pri_sel [HI_DP_PORT_NUM]; /* 端口PRI选择 eth,pon,cpu*/
    HI_DP_GEMPORT_SEL_E               em_gemport_sel;
    uint                              ui_gemport;
    
 #ifdef SUPPORT_OAM_1731
    HI_BOOL_E         em_car_en[6];    /* 基于端口配置的oam1ag报文流量抑制使能指示。 包括5个ETH口和PON口。 */
    uchar             uc_share_mac[6];
    HI_BOOL_E         em_share_mep;    /*0：非共享MEP模式；
                                        1：共享MEP模式。*/
    HI_DP_TRANS_ACT_E em_trans_act[HI_DP_PORT_NUM]; /* 1ag未使能时，接收oam1ag帧的处理ACTION。
                                                    PON端口接收的oam3ah帧处理ACTION。
                                                    HI_DP_TRANS_FWD_E：只正常转发；
                                                    HI_DP_TRANS_TOCPU_E：只转交CPU；
                                                    HI_DP_TRANS_DROP_E：丢帧过滤；
                                                    HI_DP_TRANS_TOCPU_AND_FWD_E：正常转发,且复制到CPU。

                                                    从EHT，CPU端口接收oam3ah帧的处理ACTION。
                                                    HI_DP_TRANS_FWD_E：只正常转发；
                                                    HI_DP_TRANS_TOCPU_E：只转交CPU；
                                                    HI_DP_TRANS_DROP_E：丢帧过滤；
                                                    HI_DP_TRANS_TOCPU_AND_FWD_E：正常转发,且复制到CPU；
                                                    HI_DP_TRANS_THRU_E：只透传到上行口；
                                                    HI_DP_TRANS_TOCPU_AND_THRU_E：透传到上行口，且复制到CPU；
                                                    其他：保留。
                                                    注意：透传相当于指定转发到上行口
                                                     */
 #endif
} hi_dp_oam_1ag_attr_s;

typedef struct
{
    hi_dp_oam_3ah_attr_s st_3ah_attr;
    hi_dp_oam_1ag_attr_s st_1ag_attr;
} hi_dp_oam_attr_s;

typedef enum
{
    HI_DP_OAM_MEP_UP_E = 0, /* out */
    HI_DP_OAM_MEP_DN_E,     /* in */
    HI_DP_OAM_MIP_E,
} HI_DP_OAM_MP_TYPE_E;
 #if 0
typedef enum
{
    HI_DP_OAM_TEST_LM_SINGLE_END_E = 0, /* 单端LM测试 */ HI_DP_OAM_TEST_LM_SINGLE_END_PASSIVE_E
    HI_DP_OAM_TEST_LM_DUAL_END_E,       /* 双端LM测试 */
    HI_DP_OAM_TEST_DM_E, /* DM测试 */ HI_DP_OAM_TEST_DM_TWO_WAY_PASSIVE_E
    HI_DP_OAM_TEST_THR_DUAL_E,          /* 双向被动吞吐量测试 */
    HI_DP_OAM_TEST_THR_SINGLE_E,        /* 单向吞吐量测试 */
    HI_DP_OAM_TEST_TYPE_NUM_E,
    HI_DP_OAM_TEST_TYPE_NONE_E
} HI_DP_OAM_TEST_TYPE_E;

 #endif
typedef enum
{
    
    HI_DP_OAM_TEST_LM_SINGLE_END_PASSIVE_E , /* 单端LM测试 */
    HI_DP_OAM_TEST_LM_DUAL_END_E,               /* 双端LM测试 */
    HI_DP_OAM_TEST_DM_TWO_WAY_PASSIVE_E,                /* DM测试 */
    HI_DP_OAM_TEST_THR_TWO_WAY_E,               /* 双向被动吞吐量测试 */
    HI_DP_OAM_TEST_THR_ONE_WAY_E,               /* 单向吞吐量测试 */
    HI_DP_OAM_TEST_TYPE_NONE_E,
    HI_DP_OAM_TEST_TYPE_MAX_E,
} HI_DP_OAM_TEST_TYPE_E;

 #ifdef SUPPORT_OAM_1731

typedef struct
{
    /* tag */
    HI_BOOL_E      em_tag_add_en;
    HI_BOOL_E      em_tag_del_en; // not for T
    uint           ui_tpid;
    uint           ui_vlan;
    HI_DP_ENDIAN_E em_cfi;

    /* snap */
    HI_BOOL_E em_snap_add_en;
    HI_BOOL_E em_snap_del_en; // not for T
    uchar     uc_snap_head[HI_DP_SNAP_HEAD_BYTE_NUM];

    /* 1731 */
    HI_BOOL_E em_1731_add_en;
    HI_BOOL_E em_1731_del_en; // not for T
    uchar     uc_1731_head[HI_DP_1731_HEAD_BYTE_NUM];
} hi_dp_oam_1731_attr_s;

typedef  struct
{
    uint   ui_length;
    uint   ui_vlan;
    uint   ui_cfi;
    uint   ui_pri;
    ushort us_tpid;
    uchar  uc_dmac[HI_DP_MAC_BYTE_NUM];
    uchar  uc_smac[HI_DP_MAC_BYTE_NUM];

    uint ui_tx_period;
    uint ui_tx_num;
} hi_dp_mep_prbs_attr_send_s;

typedef  struct
{
    HI_DP_PRBS_RX_MODE_E em_rx_mode; /*建议只使用单包模式*/
    uint                 ui_rx_err_range;
} hi_dp_mep_prbs_attr_recv_s;

typedef  struct
{
    HI_BOOL_E                  em_tx_enable;
    hi_dp_mep_prbs_attr_send_s st_send;

    HI_BOOL_E                  em_rx_enable;
    hi_dp_mep_prbs_attr_recv_s st_recv;
} hi_dp_mep_prbs_attr_s;
 #endif

 #ifdef CONFIG_HISI_SD5115_TYPE_T

typedef struct
{
    HI_BOOL_E           em_valid;
    HI_DP_OAM_MP_TYPE_E em_mp_type;
    uint                ui_svlan;
 //   HI_BOOL_E           em_vlan_en;
    uint                ui_md_level;
    uchar                uc_pri_map_tab[8];        /*LM测试PRI MAP表*/
    uint      ui_lbr_pri;       /* 双向被动吞吐量测试时，指定应答的lbr优先级 */
    HI_DP_OAM_TEST_TYPE_E em_lm_st;
    HI_DP_OAM_TEST_TYPE_E em_dm_st;
    HI_DP_OAM_TEST_TYPE_E em_tm_st;
    HI_BOOL_E             em_user_mac_en;        /* MAC地址匹配使能,不用于协议报文,只用于数据报文计数 */
    uchar                 uc_user_mac[6];
    HI_BOOL_E             em_mep_mac_en;
    uchar                 uc_mep_mac[6];
    hi_dp_mep_prbs_attr_s st_prbs_attr;     /*只对用TST做单向吞吐量测试有效，收发均有效*/
    hi_dp_oam_1731_attr_s st_header_attr;   /*只对用TST做单向吞吐量测试有效，收发均有效*/
} hi_dp_oam_cfm_port_item_s;

 #else

typedef struct
{
    HI_BOOL_E           em_valid;
    HI_DP_OAM_MP_TYPE_E em_mp_type;
    uint                ui_svlan;
    uint                ui_md_level;

    HI_BOOL_E             em_test_en;
    HI_DP_OAM_TEST_TYPE_E em_test_st;
    uint                  ui_pri;
  #ifdef SUPPORT_OAM_1731
    HI_BOOL_E em_user_mac_en;                    /* MAC地址匹配使能,不用于协议报文,只用于数据报文计数 */
    uchar     uc_user_mac[6];

    hi_dp_mep_prbs_attr_s st_prbs_attr;     /*只对用TST做单向吞吐量测试有效，收发均有效*/
    hi_dp_oam_1731_attr_s st_header_attr;   /*只对用TST做单向吞吐量测试有效，收发均有效*/
  #endif
} hi_dp_oam_cfm_port_item_s;

 #endif/* CONFIG_HISI_SD5115_TYPE_T */

 #define HI_DP_OAM_PORT_CFMMATCH_ITEM_NUM 16

 #define HI_DP_OAM_SHARE_MEP_MODE_PORT_CFMMATCH_ITEM_NUM MEP_TABLE_ITEM_NUM
typedef struct
{
    HI_DP_PORT_E              em_port; /* GE0\GE1\FE0\FE1\FE2\PON\CPU */
    hi_dp_oam_cfm_port_item_s st_item[HI_DP_OAM_PORT_CFMMATCH_ITEM_NUM];    /* FOR H,S,MPW 每个端口有连续8个配置表项,使用st_item的前8个表项.
                                                                            FOR T: NNI口每个端口有连续16个配置表项.
                                                                            其它端口,每个端口有连续8个配置表项,使用st_item的前8个表项. */
} hi_dp_oam_cfm_table_item_s;

typedef struct
{
    HI_BOOL_E em_lock;          /* 端口的事件通告锁定状态     */
    uint      ui_frame_err_num; /* 指定的周期内统计到的错误帧 */
    uint      ui_event_ts;      /* 发生该事件的时间           */
} hi_dp_oam_event_sta_s;

typedef struct
{
    /* GE0\GE1\FE0\FE1\FE2\PON */
    hi_dp_oam_event_sta_s st_event_sta[HI_DP_ETH_PORT_NUM + 1];
} hi_dp_oam_sta_s;

typedef struct
{
    uint ui_rx;             /*  非环回接收的3ah协议报文数(端口OAM 功能未打开也统计)             */
    uint ui_local_lp_rx;    /*  近端环回时接收的3ah协议报文数；       */
    uint ui_local_lp_tx;    /*  近端环回时环回回去的数据报文数；      */
    uint ui_remote_lp_rx;   /*  远端环回时接收的3ah协议报文数；       */
    uint ui_remote_test_tx; /*  远端环回时发送的测试数据报文数；      */
    uint ui_remote_test_rx; /*  远端环回时接收的测试报文数；          */
} hi_dp_oam_port_cnt_s;

/* 基于端口的各个相应的OAM 3AH统计项 */
typedef struct
{
    hi_dp_oam_port_cnt_s st_port_cnt[13];                           /* ETH + LLID */
    ulong                ul_err_frame_total[HI_DP_ETH_PORT_NUM + 1];/* 端口事件通告总错帧数 */
    uint                 ui_event_num[HI_DP_ETH_PORT_NUM + 1];      /* 端口事件通告事件数 */
} hi_dp_oam_cnt_s;

/*oam 3ah 事件中断状态，用于中断回调函数的参数ui_arg的解析*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fe0_evn_int  : 1; // [0]
        unsigned int Reserved_40  : 3; // [3..1]
        unsigned int fe1_evn_int  : 1; // [4]
        unsigned int Reserved_39  : 3; // [7..5]
        unsigned int fe2_evn_int  : 1; // [8]
        unsigned int Reserved_38  : 3; // [11..9]
        unsigned int fe3_evn_int  : 1; // [12]
        unsigned int Reserved_37  : 3; // [15..13]
        unsigned int fe4_evn_int  : 1; // [16]
        unsigned int Reserved_36  : 3; // [19..17]
        unsigned int epon_evn_int : 1; // [20]
        unsigned int Reserved_35  : 11; // [31..21]
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_EVENT_INT_STATE;

typedef enum
{
    HI_DP_OAM_EVENT_ALARM_E = 0,
    HI_DP_OAM_ALARM_NUM_E,
} HI_DP_OAM_ALARM_E;

/*OAM模块告警属性 */
typedef struct
{
    /* 告警属性*/
    hi_dp_alarm_attr_s st_alarm_attr[HI_DP_OAM_ALARM_NUM_E];
    void (*alarm_call_back)(HI_DP_OAM_ALARM_E em_alarm, HI_DP_ALARM_STATE_E em_state, uint ui_arg);
} hi_dp_oam_alarm_attr_s;

#endif /*  end of __HI_DRV_DP_OAM_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /*  end of __cplusplus  */
#endif  /*  end of __cplusplus  */
