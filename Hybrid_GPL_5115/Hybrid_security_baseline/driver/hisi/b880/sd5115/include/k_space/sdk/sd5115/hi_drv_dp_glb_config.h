/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_dp_local_port.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   : GLOBAL CONFIG 模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月24日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus  */
#endif /* __cpluscplus  */

#ifndef     __HI_DRV_DP_GLB_CONFIG_H__
 #define    __HI_DRV_DP_GLB_CONFIG_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"

 #define  HI_DP_TIMER_INTERVAL 1000    /* ms */

 #define  HI_DP_TPID_NUM 3
 #define  HI_DP_PORT_DET_NUM 8

typedef enum
{
    HI_DP_GLB_WORK_MODE_E = 0,
    HI_DP_GLB_TEST_MODE_E
} HI_DP_GLB_SYS_MODE_E;

/*
00：全交换模式；
01：纯接入模式；
10：接入交换模式；
11：保留。 */
typedef enum
{
    HI_DP_GLB_EXCHANG_MODE_E = 0,
    HI_DP_GLB_CONNECT_MODE_E,
    HI_DP_GLB_CONNECT_EXCHANG_MODE_E,
} HI_DP_GLB_SW_MODE_E;

// #if defined (CONFIG_HISI_SD5115_MP_PILOT)
typedef enum
{
    HI_DP_GLB_MAC_SEL_1K_E = 0,
    HI_DP_GLB_MAC_SEL_16K_E,
    HI_DP_GLB_MAC_SEL_8K_E,
    HI_DP_GLB_MAC_SEL_UNKNOWN_E,
} HI_DP_GLB_MAC_SEL_E;
// #endif

/* 全局配置参数 */
typedef struct
{
    uint                ui_mtu; /* Added by w00180499, 2011/5/30 */
    HI_DP_UP_MODE_E     em_up_mode;         /* 上行模式 */
    HI_DP_GLB_SW_MODE_E em_sw_mode;     /* LSW接入模式 */

    HI_DP_PRI_MODE_E em_up_pri_mode;
    HI_DP_PRI_MODE_E em_dn_pri_mode;

    HI_DP_VLAN_MODE_E em_up_sfc_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_sfc_vlan_mode;
    HI_DP_VLAN_MODE_E em_up_uc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_uc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_up_mc_fwd_vlan_mode;
    HI_DP_VLAN_MODE_E em_dn_mc_fwd_vlan_mode;

 #ifdef CONFIG_HISI_SD5115_TYPE_T
    ushort us_stpid[HI_DP_TPID_NUM];          /* 入口TPID识别；出口TPID添加 */
    ushort us_ctpid[HI_DP_TPID_NUM];          /* 入口TPID识别；出口TPID添加 */
 #else
    ushort us_tpid[HI_DP_TPID_NUM];          /* 入口TPID识别；出口TPID添加 */
 #endif

    HI_DP_ENDIAN_E em_default_cfi;         /* 缺省CFI: 芯片各端口报文添加缺省TAG时的CFI域配置 */

    HI_BOOL_E em_pritag_en;      /*5115 add;是否识别pri_tag报文；否则按tag报文处理*/

    // #if defined (CONFIG_HISI_SD5115_TYPE_H) ||defined (CONFIG_HISI_SD5115_TYPE_T)
    HI_DP_GLB_MAC_SEL_E em_mac_num_sel;      /* MAC地址表数目选择 */
    //#endif
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_pri_match_type;
    uint ui_port_cpu_tag_del;
 #endif

    //#ifdef HI_SDK_DEBUG
    HI_BOOL_E em_sdk_debug_print_en;
    //#endif
} hi_dp_glb_attr_s;

/* 全局状态信息 */
typedef struct
{
    uint ui_year;               /* DATE */
    uint ui_month;
    uint ui_day;

    uint ui_chip_id;            /* 32'h51130100 */
    uint ui_base_line;          /* 基线版本号 */
    uint ui_sub_version;        /* 子版本号 */

    uint ui_vendor_id;          /* 厂商ID */
} hi_dp_glb_sta_s;

typedef struct
{} hi_dp_glb_cnt_s;

/* 芯片能力集 */
typedef struct
{
    /* 芯片版本 */
    HI_DP_IC_VER_E em_version;

    /* 上行接口能力 */
    uint ui_gpon_num;  /* GPON接口数量 */
    uint ui_epon_num;  /* EPON接口数量，单GPON模式下为0 */

    /* security模块 */
    uint ui_ip_filter_num;            /* IP过滤表规格   */
    uint ui_ip_filter_ipv4_num;       /* for 5115T:IP过滤表规格 */
    uint ui_ip_filter_ipv6_num;       /* for 5115T:IP过滤表规格 */     
    /* flow模块 */
    uint ui_fc_num;                   /* FC表规格        */
    uint ui_efc_num;                  /*EFC表规格        */
    uint ui_tcfl_rep;                 /* TC/FL替换表规格 */
    uint ui_flow_point_field_num;     /* 自定义域个数    */
    uint ui_flow_g0_num;              /* G0个数          */
    uint ui_flow_g1_num;              /* G1个数          */
    uint ui_flow_key1_max;            /* key1最大值      */
    uint ui_flow_mask1_max;           /* mask1最大值     */
    uint ui_fact_num;
    uint ui_flow_statis_index;

    /* L2 */
    uint ui_ip_mc_num;          /* ip多播表规格             */
    uint ui_l2mac_fid_num;      /* mac地址表学习到的fid数目 */

    /* L3 */
    uint ui_l3_replace_num;     /* 三层替换表规格       */
    uint ui_nht_num;            /* 下一跳表规格         */
    uint ui_rt_num;             /* 路由表规格           */
    uint ui_rt_act_num;         /* 三层行为表规格       */
    uint ui_tunnel_num;         /* 隧道表规格           */
    uint ui_tunnel_rt_num;      /* 隧道路由表规格       */
    uint ui_tunnel_sip_num;     /* 隧道sip检查表规格    */
    uint ui_napt_num;           /* NAPT表规格           */
    uint ui_oif2_size;          /* OIF2表size，单位byte */
    uint ui_nht_ipv4_num;       /* for 5115T:下一跳表规格 */
    uint ui_nht_ipv6_num;       /* for 5115T:下一跳表规格 */
    uint ui_rt_ipv4_num ;       /* for 5115T:下一跳表规格 */
    uint ui_rt_ipv6_num ;       /* for 5115T:下一跳表规格 */    

    /* QOS */
    uint ui_ecar_num;                /* ECAR,扩展流量抑制规格      */
    uint ui_tocpu_reason_car_num;    /* tocpu reason car规格       */
    uint ui_tocpu_reason_map_num;    /* tocpu reason car映射表规格 */
} hi_dp_glb_spec_s;

/* BEGIN: Added by w00180499, 2011/7/8 */
typedef enum
{
    HI_LOCAL_CLOCK_E = 0,
    HI_LINE_CLOCK_E,
} HI_CURRENT_CLOCK_STATE_E;

typedef struct
{
    HI_CURRENT_CLOCK_STATE_E em_current_clock;
    uint                     ui_clk_switch_times;
} hi_dp_glb_clk_switch_info_s;
/* END:   Added by w00180499, 2011/7/8 */

#endif /* end of __WDTAPI_H */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
