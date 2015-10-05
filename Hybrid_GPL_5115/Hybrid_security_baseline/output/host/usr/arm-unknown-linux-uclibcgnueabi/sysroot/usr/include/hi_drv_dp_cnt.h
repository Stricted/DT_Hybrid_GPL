/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_drv_dp_cnt.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2008年9月24日
  最近修改   :
  功能描述   :  统计数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2008年9月24日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus  */

#ifndef __HI_DRV_DP_CNT_H__
 #define __HI_DRV_DP_CNT_H__

 #include "hi_drv_common.h"
 #include "hi_drv_common_dp.h"

typedef struct
{
    uint ge0_fwd_cfm_cnt;
    uint ge0_drop_cfm_cnt;

    //uint ge0_cap_cfm_cnt;
    uint ge1_fwd_cfm_cnt;
    uint ge1_drop_cfm_cnt;

    //uint ge1_cap_cfm_cnt;
    uint fe0_fwd_cfm_cnt;
    uint fe0_drop_cfm_cnt;

    //uint fe0_cap_cfm_cnt;
    uint fe1_fwd_cfm_cnt;
    uint fe1_drop_cfm_cnt;

    //uint fe1_cap_cfm_cnt;
    uint fe2_fwd_cfm_cnt;
    uint fe2_drop_cfm_cnt;

    //uint fe2_cap_cfm_cnt;
    uint pon_fwd_cfm_cnt;
    uint pon_drop_cfm_cnt;

    //uint pon_cap_cfm_cnt;
    uint cpu_fwd_cfm_cnt;
    uint cpu_drop_cfm_cnt;
    uint prbs_fwd_cfm_cnt;
    uint prbs_drop_cfm_cnt;

    uint nmc_rev_cnt;
    uint mc_rev_cnt;
    uint cfm_to_cpu_cnt;
    uint cfm_rls0_abn_cnt;
    uint ucff_full_drop_cnt;
    uint bcff_full_drop_cnt;
} hi_dp_logic_oam_cnt_s;

typedef struct
{
    //uint rxcap_stat;
    //uint rxarl_stat;
    uint txall_stat;

    //uint rxvld_stat;
    //uint txvld_stat;
    //uint tocpu17_stat;
    uint tocpu43_stat;
    uint uptocpu42_stat;

    //uint dntocpu42_stat;
    //uint tocpu44_stat;
    uint up_idx_stat;
    uint dn_idx_stat;
    uint drop49_stat;
    uint drop58_stat;
    uint drop111_stat;
    uint iptocpu55_cnt;

    //5115 add
    uint drop28_stat;
    uint dntocpu56_stat;
} hi_dp_logic_napt_cnt_s;

typedef struct
{
    uint pie_ge0_rx_cnt;
    uint pie_ge1_rx_cnt;
    uint pie_ge2_rx_cnt;
    uint pie_ge3_rx_cnt;
    uint pie_ge4_rx_cnt;
    uint pie_nni_rx_cnt;
    uint pie_cpu_rx_cnt;
    uint pie_drop_cnt;
    uint pie_ex_lng_cnt;
    uint pie_ex_sht_cnt;
    uint pie_mac_err_cnt;
    uint pie_tag_err_cnt;
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint pie_spoof_cnt;
 #endif
    uint ge0_rx_cnt;
    uint ge0_drop_cnt;
    uint ge0_ex_lng_cnt;
    uint ge0_ex_sht_cnt;
    uint ge0_mac_err_cnt;
    uint ge0_tag_err_cnt;
    uint ge1_rx_cnt;
    uint ge1_drop_cnt;
    uint ge1_ex_lng_cnt;
    uint ge1_ex_sht_cnt;
    uint ge1_mac_err_cnt;
    uint ge1_tag_err_cnt;
    uint ge2_rx_cnt;
    uint ge2_drop_cnt;
    uint ge2_ex_lng_cnt;
    uint ge2_ex_sht_cnt;
    uint ge2_mac_err_cnt;
    uint ge2_tag_err_cnt;
    uint ge3_rx_cnt;
    uint ge3_drop_cnt;
    uint ge3_ex_lng_cnt;
    uint ge3_ex_sht_cnt;
    uint ge3_mac_err_cnt;
    uint ge3_tag_err_cnt;
    uint ge4_rx_cnt;
    uint ge4_drop_cnt;
    uint ge4_ex_lng_cnt;
    uint ge4_ex_sht_cnt;
    uint ge4_mac_err_cnt;
    uint ge4_tag_err_cnt;
    uint nni_rx_cnt0;
    uint nni_rx_cnt1;
    uint nni_drop_cnt;
    uint nni_ex_lng_cnt;
    uint nni_ex_sht_cnt;
    uint nni_mac_err_cnt;
    uint nni_tag_err_cnt;
    uint port_link_err_rsv; //级联模式下，UNI_GE0级联tpid或者级联vlan匹配错误TAG寄存器有效信号
    uint port_link_err_tag;

    uint rx_ge0_car_drop_cnt;
    uint rx_ge1_car_drop_cnt;
    uint rx_ge2_car_drop_cnt;
    uint rx_ge3_car_drop_cnt;
    uint rx_ge4_car_drop_cnt;
    uint rx_nni_car_drop_cnt;
    uint rx_mcf_drop_cnt;

    uint prbs_rx_cnt;
    uint prbs_drop_cnt;
    uint prbs_ex_lng_cnt;
    uint prbs_ex_sht_cnt;
    uint prbs_mac_err_cnt;
} hi_dp_logic_rxdma_cnt_s;

typedef struct
{
    /* 寄存器 */
    uint napt_up_idx_cnt;
    uint napt_dn_idx_cnt;
    uint redir_pkt_cnt;
    uint ppp_add_err_cnt;
    uint redir_err_cnt;
    uint tag_del_err_cnt;
    uint fcb_pon0_cnt;
    uint fcb_pon1_cnt;
    uint fcb_pon2_cnt;
    uint fcb_pon3_cnt;
    uint fcb_pon4_cnt;
    uint fcb_pon5_cnt;
    uint fcb_pon6_cnt;
    uint fcb_pon7_cnt;
    uint fcb_eth0_cnt;
    uint fcb_eth1_cnt;
    uint fcb_eth2_cnt;
    uint fcb_eth3_cnt;
    uint fcb_eth4_cnt;
    uint fcb_cpu_cnt;
    uint fcb_prbs_cnt;
    uint out_eth0_cnt;
    uint out_eth1_cnt;
    uint out_eth2_cnt;
    uint out_eth3_cnt;
    uint out_eth4_cnt;
    uint out_cpu_cnt;
    uint out_prbs_cnt;

    uint out_pon0_cnt;
    uint out_pon1_cnt;
    uint out_pon2_cnt;
    uint out_pon3_cnt;
    uint out_pon4_cnt;
    uint out_pon5_cnt;
    uint out_pon6_cnt;
    uint out_pon7_cnt;

    /* 表项 */
    ulong bdt_pon0_pkt;
    ulong bdt_pon1_pkt;
    ulong bdt_pon2_pkt;
    ulong bdt_pon3_pkt;
    ulong bdt_pon4_pkt;
    ulong bdt_pon5_pkt;
    ulong bdt_pon6_pkt;
    ulong bdt_pon7_pkt;
    ulong bdt_eth0_pkt;
    ulong bdt_eth1_pkt;
    ulong bdt_eth2_pkt;
    ulong bdt_eth3_pkt;
    ulong bdt_eth4_pkt;
    ulong bdt_cpu_pkt;
    ulong bdt_prbs_pkt;

    ulong bdt_pon0_byte;
    ulong bdt_pon1_byte;
    ulong bdt_pon2_byte;
    ulong bdt_pon3_byte;
    ulong bdt_pon4_byte;
    ulong bdt_pon5_byte;
    ulong bdt_pon6_byte;
    ulong bdt_pon7_byte;
    ulong bdt_eth0_byte;
    ulong bdt_eth1_byte;
    ulong bdt_eth2_byte;
    ulong bdt_eth3_byte;
    ulong bdt_eth4_byte;
    ulong bdt_cpu_byte;
    ulong bdt_prbs_byte;
} hi_dp_logic_txdma_cnt_s;

typedef struct
{
    uint cap_rxall;          /* 从EFC模块接收的报文个数统计                 */

    //uint cap_rxeth;          /* 从EFC接收的来自ETH的报文个数统计            */
    //uint cap_rxcpu;          /* 从EFC接收的来自CPU的报文个数统计            */
    //uint cap_rxpon;          /* 从EFC接收的来自PON的报文个数统计            */
    uint cap_txall;           /* 向NAPT发送的报文个数统计                   */
    uint cap_drop;           /* CAP模块(出口)丢弃的报文个数统计             */

    //uint cap_nodrop;         /* CAP模块(出口)不丢弃的报文个数统计           */
    //uint cap_onlytocpu;      /* CAP模块(出口)提交给CPU的报文个数统计        */
    //uint cap_cpytocpu;       /* CAP模块(出口)拷贝给CPU的报文个数统计        */
    uint cap_tocpu;       /* CAP模块(出口)给CPU的报文个数统计        */
    uint cap_match;          /* CAP模块(出口)指定转发的报文个数统计         */
    uint cap_thru;           /* CAP模块(出口)透传的报文个数统计             */
    uint cap_lrn;            /* CAP模块指示学习的报文个数统计               */

    //uint cap_spri_rmk;       /* CAP模块对外层优先级remarking的报文个数统计  */
    //uint cap_cpri_rmk;       /* CAP模块对内层优先级marking的报文个数统计    */
    //uint spec_untag;         /* 捕获到Untag报文的数目                       */   //5115 del
    //uint spec_pritag;        /* 捕获到优先级tag报文的数目                   */   //5115 del
    //uint spec_arp;           /* 捕获到ARP报文的数目                         */
    //uint spec_rarp;          /* 捕获到RARP报文的数目                        */
    uint spec_bpdu;          /* 捕获到BPDU报文的数目                        */
    uint spec_eapol;         /* 捕获到EAPOL报文的数目                       */

    //uint spec_igmp;          /* 捕获到IGMP报文的数目                        */
    //uint spec_icmp;          /* 捕获到ICMP报文的数目                        */
    //uint spec_dhcp;          /* 捕获到DHCP报文的数目                        */
    //uint spec_pppoe_dis;     /* 捕获到PPPoE Discovery报文的数目             */
    //uint spec_pppoe_ses;     /* 捕获到PPPoE Session报文的数目               */
    //uint spec_ipv6;          /* 捕获到IPv6报文的数目                        */
    uint spec_8023ah;        /* 捕获到802.3ah报文的数目                     */
    uint spec_8021ag;        /* 捕获到802.1ag报文的数目                     */

    uint spec_eth0_1ag;        /* 从eth0捕获到802.1ag报文的数目                     */
    uint spec_eth1_1ag;        /*从eth1 捕获到802.1ag报文的数目                     */
    uint spec_eth2_1ag;        /* 从eth2捕获到802.1ag报文的数目                     */
    uint spec_eth3_1ag;        /* 从eth3捕获到802.1ag报文的数目                     */
    uint spec_eth4_1ag;        /* 从eth4捕获到802.1ag报文的数目                     */
    uint spec_pon_1ag;        /* 从pon捕获到802.1ag报文的数目                     */

    uint cap_smac_drop;          /* CAP模块SMAC过滤丢弃的报文个数统计             */
    uint spec_unk_hdr;        /* 未知扩展头统计计数             */
    uint spec_cpu_1ag;        /* 从cpu捕获到802.1ag报文的数目                     */

    //uint vlanmk_mis;
    //uint primk_mis;

 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint unk_hdr_cnt;        /* 未知扩展头统计                     */
 #endif

    uint cnt[HI_DP_CAP_PKT_CFG_SIZE];
} hi_dp_logic_cap_cnt_s;

typedef struct
{
    uint aflow_cnt;
    uint mflow_cnt;
    uint actcfl_cnt;
    uint fc_debug0;
} hi_dp_logic_fc_cnt_s;

typedef struct
{
    uint arl_age;               /* 老化的次数              */
    uint age_entry;             /* 被老化掉的表项计数      */
    uint sv_up_fail;            /* 上行SMAC+VLAN查表失败, 不包括丢弃报文  */
    uint sv_dn_fail;            /* 下行SMAC+VLAN查表失败, 不包括丢弃报文  */
    uint dv_up_fail;            /* 上行DMAC+VLAN查表失败   */
    uint dv_dn_fail;            /* 下行DMAC+VLAN查表失败   */
    uint learn_fail;            /* 学习失败的次数          */
    uint fe0_learn;             /* 从GE0/FE0学习的次数     */
    uint fe1_learn;             /* 从GE1/FE1学习的次数     */
    uint fe2_learn;             /* 从FE2学习的次数         */
    uint fe3_learn;             /* 从FE3学习的次数         */
    uint fe4_learn;             /* 从FE4学习的次数         */
    uint pon_learn;             /* 从PON学习的次数         */
    uint cpu_learn;             /* 从CPU学习的次数         */
    uint prbs_learn; /* 从PRBS学习的次数         */    //5115 add
    uint age_mport;             /* 老化多端口表项的次数    */
    uint smac_learn;            /* SMAC学习新表项的次数    */
    uint smac_hit;              /* SMAC命中的次数          */
    uint egress_0;              /* CAP模块egress全0统计    */
    uint dmac_hit;              /* DMAC查表命中统计        */
    uint hash_full;             /* HASH表满                */
} hi_dp_logic_arl_cnt_s;

typedef struct
{
    uint wr_full_drop;           /* 数据FIFO满丢帧统计 */
    uint data_buf_afull;         /* 数据Buffer满统计 */
    uint rec_buf_afull;          /* REC Buffer满统计 */
    uint mcf_fifo_full;          /* MCF FIFO满统计 */
    uint rec_buf_fail;           /* REC Buffer重组失败统计 */
    uint multicast_drop;         /* 组播过滤帧统计 */
    uint rec_drop;               /* 重组失败帧统计 */
    uint rx_tdm;                 /* TDM下行接收帧统计 */
    uint tx_tdm;                 /* TDM上行接收帧统计 */
    uint rec;                    /* 重组成功计数 */
    uint fcs_lenerror;           /* MCF超长超短帧过滤计数 */
} hi_dp_logic_glb_cnt_s;

typedef struct
{
    uint rx_cnt;
    uint rxeth_cnt;
    uint rxcpu_cnt;
    uint rxpon_cnt;
    uint efc_cnt;
    uint drop_cnt;

    //uint nodrop_cnt;
    uint over_128byte_cnt;
    uint unk_hdr_cnt;

    //uint tunnel_frag_cnt;

    //5115 add
    uint rx_prbs_cnt;
    uint pa_ipfrag_cnt;
    uint pkt_ds_lite_cnt;
    uint pkt_6rd_cnt;
} hi_dp_logic_pa_cnt_s;

typedef struct
{
    uint cpu_txfcb[8];
    uint cpu_txfba;
    uint cpu_rxfcb;
    uint cpu_rxfba;
    uint cputx_rx;
    uint cputx_tx;
    uint cpurx_rx;
    uint cpurx_tx[8];

    uint cpurtx_debug0;
    uint cpurtx_debug1;
    uint cpurtx_debug2;
    uint cpurtx_mtu_drop;
} hi_dp_logic_cpurtx_cnt_s;

typedef struct
{
    uint qm0_ack_err_cnt;
    uint txdma_cid_ack32_err_cnt;
    uint qm0_wieght_over_cnt;
    uint txdma_wieght_over_cnt;
    uint qm1_wieght_over_cnt;
    uint qm2_wieght_over_cnt;
    uint fc0_alloc;
    uint fc1_alloc;
    uint fc2_alloc;
    uint fc3_alloc;
    uint fc4_alloc;
    uint fc5_alloc;
    uint fc6_alloc;
    uint fc7_alloc;
} hi_dp_logic_am_cnt_s;

/* EFC模块的统计信息 */
typedef struct
{
    uint label_ent;
    uint label_subtable;
    uint label_hit;
} hi_dp_efc_debug_cnt_s;

typedef struct
{
    uint svlan_rmk;         /* SVLAN Remarking统计                */
    uint cvlan_rmk;         /* CVLAN Remarking统计                */
    uint spri_rmk;          /* SPriority Remarking统计            */
    uint cpri_rmk;          /* CPriority Remarking统计            */

    //uint efc_fid0;          /* 指定FID0报文统计                   */
    //uint efc_fid1;          /* 指定FID1报文统计                   */
    //uint efc_fid2;          /* 指定FID2报文统计                   */
    uint unhit_pkt;         /* 没有匹配的报文统计                 */
    uint execute_act;       /* 报文匹配执行ACTION的报文统计       */
    uint match_map;         /* 报文匹配执行GEMPORT映射的报文统计  */

    //uint efc_age;           /*   */
    uint efc_debug0;        /*   */
    uint efc_debug1;        /*   */
    uint efc_tocpu;         /* TOCPU报文报文统计                  */
    uint efc_drop;          /* 丢弃报文报文统计                   */
    uint efc_routing;       /* routing报文报文统计                */
    uint efc_nat;           /* nat报文报文统计                    */
    uint efc_napt;          /* napt报文报文统计                   */
    uint pppoe_add;         /* 添加PPPOE头报文报文统计            */
    uint tag_add;           /* 添加tag报文报文统计                */
    uint tag_del;           /* 删除tag报文报文统计                */
    uint multi_match;       /*  */

    uint invld_car_cnt;     /* EFC内无效CAR报文个数统计 */
    uint fcar_drop_cnt;     /* EFC内流CAR丢弃报文数统计 */

    //uint fwd_v2_cnt;        /* EFC内进行三层路由正常转发的报文数统计 */
    uint fwd_tocpu_cnt;     /* EFC内进行三层路由正常转发加拷贝CPU的报文数统计 */

    //uint route_v2_cnt;      /* EFC内进行策略路由的报文数统计 */

    hi_dp_efc_debug_cnt_s debug[8];

    uint efc_spec_fwd_cnt;
    uint efc_thru_cnt;
    uint efc_route_cpu_cnt;
    uint efc_policy_route_cnt;
    uint car0_drop_cnt;
    uint car1_drop_cnt;
    uint car2_drop_cnt;
    uint hit_pkt;                   /* 匹配的报文统计                 */
} hi_dp_logic_efc_cnt_s;

/* QM 模块的统计信息:   */
typedef struct
{
    //5115 edit
    uint qm_rx_drop;
    uint qm_rx_not_drop;
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint qm_share_uscnt[4];   //命令行，增加
 #else
    uint qm_share_uscnt;   //命令行，增加
 #endif
    uint lm_stat0;
    uint lm_stat1;
    uint lm_stat2;
    uint lm_stat3;
    uint lm_stat4;
    uint lm_stat5;
    uint lm_stat6;
    uint lm_stat7;
} hi_dp_logic_qm_cnt_s;

typedef struct
{
    uint cnt[HI_DP_DROP_NUM_E];
} hi_dp_logic_drop_reason_cnt_s;

typedef struct
{
    uint cnt[HI_DP_TOCPU_NUM_E];
} hi_dp_logic_tocpu_reason_cnt_s;

typedef struct
{
    uint cnt[HI_DP_THRU_NUM_E];
} hi_dp_logic_thru_reason_cnt_s;

/* MAP_CAR模块统计信息 */
typedef struct
{
    //5115 adde
    uint tocpu_scar_cnt;
    uint local_scar_cnt;
    uint igr_car_cnt;
    uint egr_car_cnt;
    uint gcar_cnt;
    uint tocpu_rz_car_cnt;
    uint igr_car_yel_cnt;
    uint egr_car_yel_cnt;
    uint gcar_yel_cnt;
    uint mtp_rx_cnt;
    uint mtp_tx_cnt;
    uint igr_car_gre_cnt;
    uint egr_car_gre_cnt;
    uint gcar_gre_cnt;
} hi_dp_logic_mtp_cnt_s;

typedef struct
{
    uint bcast_scar_cnt;
    uint uucast_scar_cnt;
    uint match_map_cnt;
    uint unmatch_map_cnt;
    uint match_tpf_cnt;
    uint unmatch_tpf_cnt;
    uint sfc_rx_cnt;
    uint sfc_tx_cnt;

    uint sfc_map_addr;
    uint sfc_map_vld;
    uint sfc_tpf_addr;
    uint sfc_tpf_vld;
    uint txdma_no_ack_cnt;
} hi_dp_logic_sfc_cnt_s;

typedef struct
{
    uint kn_uc_in;
    uint unkn_uc_in;
    uint kn_mc_in;
    uint unkn_mc_in;
    uint bc_in;
    uint cpu_act_mc_in;
    uint kn_uc_in_drop;
    uint unkn_uc_in_drop;
    uint kn_mc_in_drop;
    uint unkn_mc_in_drop;
    uint bc_in_drop;
    uint cpu_act_mc_in_drop;
    uint mc_out;
    uint mc_fifo_drop;
    uint bc_out;
    uint bc_fifo_drop;
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint umc_l2_drop;
    uint eth0_vlan_chk_isolt;
    uint eth1_vlan_chk_isolt;
    uint eth2_vlan_chk_isolt;
    uint eth3_vlan_chk_isolt;
    uint eth4_vlan_chk_isolt;
    uint pon_vlan_chk_isolt;
    uint cpu_vlan_chk_isolt;
 #endif
} hi_dp_logic_mcd_cnt_s;

/* QM 模块的统计信息: 队列调度统计  */
typedef struct
{
    uint ui_qid;
    uint ui_queue_in;                     /* 入队成功统计                */
    uint ui_queue_out;                    /* 出队成功统计                */
    uint ui_drop_queue_tail;              /* 队列尾丢弃统计              */
    uint ui_drop_resource_low;            /* 资源不足丢弃统计            */
    uint ui_drop_flow_wred;           /* 基于流CAR的WRED丢弃统计     */
    uint ui_drop_group_wred;          /* 基于组CAR的WRED丢弃统计     */
} hi_dp_logic_queue_cnt_s;

typedef struct
{
    uint ui_tx_omci_cnt;
    uint ui_rx_omci_cnt;
    uint ui_omci_crc_err_cnt;
    uint ui_omci_full_drop_cnt;
    uint ui_txprbs_sum_cnt;
    uint ui_rxprbs_berr_cnt;
    uint ui_rxprbs_byte_cnt;
    uint ui_rxprbs_perr_cnt;
    uint ui_rxprbs_sum_cnt;
} hi_dp_logic_gpon_ext_cnt_s;

typedef struct
{
    uint ui_bip_err_cnt;
    uint ui_rang_req_cnt;
    uint ui_sn_req_cnt;
    uint ui_ploam_msg_id0_cnt;
    uint ui_ploam_msg_id1_cnt;
    uint ui_ploam_msg_id2_cnt;
    uint ui_ploam_right_cnt;
    uint ui_ploam_wrong_cnt;
    uint ui_ploam_cnt;
    uint ui_ploam_full_drop_cnt;
    uint ui_rei_cnt;
    uint ui_plend0_err_cnt;
    uint ui_plend1_err_cnt;
    uint ui_plend_err_cnt;
    //U_PLEND_ERR_CNT        PLEND_ERR_CNT;
    //U_SUPCNT_ERR_CNT       SUPCNT_ERR_CNT;
    uint ui_supcnt_err_cnt;
    uint ui_bwmap_crc_err_cnt;
    uint ui_fec_us_swi_cnt;
} hi_dp_logic_dgtc_sys_cnt_s;

typedef struct
{
    uint ui_fec_dec_cw_cnt;
    uint ui_fec_err_cw_cnt;
    uint ui_fec_cor_cw_cnt;
    uint ui_fec_cor_byte_cnt;
    uint ui_fec_seconds_cnt;
    uint ui_fec_ds_swi_cnt;
} hi_dp_logic_dgtc_line_cnt_s;

typedef struct
{
    uint ui_ds_pon0_cnt;
    uint ui_ds_pon1_cnt;
    uint ui_ds_gem_cnt;
    uint ui_gem_eth_cnt;
    uint ui_gem_mcast_cnt;
    uint ui_gem_omci_cnt;
    uint ui_gem_prbs_cnt;
    uint ui_aes_omci_cnt;
    uint ui_aes_prbs_cnt;
    uint ui_aes_mcast_cnt;
    uint ui_aes_eth_cnt;
    uint ui_aes_blk_cnt;
    uint ui_key_ok_cnt;
    uint ui_rec_eth_cnt;
    uint ui_rec_mcast_cnt;
    uint ui_rec_omci_cnt;
    uint ui_rec_prbs_cnt;
    uint ui_short_err_cnt;
    uint ui_long_err_cnt;

    //U_LEN_ERR_CNT          LEN_ERR_CNT;
    uint ui_ds_mcast_cnt;
    uint ui_ds_ucast_cnt;
    uint ui_ds_bcast_cnt;
    uint ui_ds_eth_cnt;
    uint ui_fcs_err_cnt;
    uint ui_gem_lof_cnt;
    uint ui_ds_full_err_cnt;
    uint ui_ds_full_drop_cnt;

    //U_ETH_OVERFLOW_CNT     ETH_OVERFLOW_CNT;
    //U_ETH_PORT_REC_CNT     ETH_PORT_REC_CNT;
    uint ui_ds_port_err_cnt;
    uint ui_ds_port_drop_cnt;
    uint ui_gem_drop_cnt;
    uint ui_hec_cor_cnt;
} hi_dp_logic_dgem_sys_cnt_s;

typedef struct
{
    uint ui_plm_sum_cnt;
    uint ui_plm_msg1_cnt;
    uint ui_plm_msg0_cnt;
    uint ui_lm_nvld_cnt;
    uint ui_dbru_m0_sum_cnt;
    uint ui_dbru_m1_sum_cnt;
    uint ui_dbru_spec_cnt;
    uint ui_omci_sum_cnt;
    uint ui_gem_sum_cnt;
    uint ui_gem_cut_cnt;
    uint ui_gem_spec_cnt;
    uint ui_gem_spec_cut_cnt;
    uint ui_idle_sum_cnt;
    uint ui_idle_spec_cnt;
    uint ui_burst_sum_cnt;
    uint ui_eth_sum0_cnt;
    uint ui_eth_sum1_cnt;
    uint ui_unicast_cnt;
    uint ui_multicast_cnt;
    uint ui_broadcast_cnt;
    uint ui_us_eth_cnt;
    uint ui_tcont_err_cnt;
} hi_dp_logic_ugmac_sys_cnt_s;

typedef struct
{} hi_dp_logic_ugmac_line_cnt_s;

typedef struct
{
    uint ui_tx_buffer_depth[8];
    uint ui_gpon_qbyte[8];
    uint ui_tx_drop_pkt_cnt[8];
    uint ui_txdma_pkt_cnt[8];
    uint ui_tx_mac_pkt_cnt[8];
} hi_dp_logic_eps_pon_cnt_s;

typedef struct
{
    HI_DP_PORT_E em_port;            /* GE0/GE1 */

    uint rx_octets_ok_cnt;
    uint rx_octets_bad_cnt;
    uint rx_uc_pkts_cnt;
    uint rx_mc_pkts_cnt;
    uint rx_bc_pkts_cnt;
    uint rx_pkts_64oct_cnt;
    uint rx_pkts_65to127_oct_cnt;
    uint rx_pkts_128to255_oct_cnt;
    uint rx_pkts_256to511_oct_cnt;
    uint rx_pkts_512to1023_oct_cnt;
    uint rx_pkts_1024to1518_oct_cnt;
    uint rx_pkts_1519tomax_oct_cnt;
    uint rx_fcs_err_cnt;
    uint rx_tagged_cnt;
    uint rx_data_err_cnt;
    uint rx_align_err_cnt;
    uint rx_long_err_cnt;
    uint rx_jabber_err_cnt;
    uint rx_pause_cnt;
    uint rx_unk_ctl_cnt;
    uint rx_very_long_err_cnt;
    uint rx_runt_err_cnt;
    uint rx_short_err_cnt;
    uint tx_octets_ok_cnt;
    uint tx_octets_bad_cnt;
    uint tx_uc_pks_cnt;
    uint tx_mc_pks_cnt;
    uint tx_bc_pks_cnt;
    uint tx_pkts_64oct_cnt;
    uint tx_pkts_65to127_oct_cnt;
    uint tx_pkts_128to255_oct_cnt;
    uint tx_pkts_256to511_oct_cnt;
    uint tx_pkts_512to1023_oct_cnt;
    uint tx_pkts_1024to1518_oct_cnt;
    uint tx_pkts_1519tomax_oct_cnt;
    uint tx_total_col_cnt;
    uint tx_single_col_cnt;
    uint tx_multi_col_cnt;
    uint tx_late_col_cnt;
    uint tx_exc_col_err_cnt;
    uint tx_exc_len_drop_cnt;
    uint tx_underrun_cnt;
    uint tx_tagged_cn;
    uint tx_crc_err_cnt;
    uint tx_pause_cnt;
    uint rx_overrun_cnt;
    uint rx_lengthfield_er_cnt;
} hi_dp_logic_ge_cnt_s;

typedef struct
{
    HI_DP_PORT_E em_port;            /* FE0/FE1/FE2 */

    uint tx_pks_64oct_cnt;
    uint tx_pks_64to127_oct_cnt;
    uint tx_pks_128to255_oct_cnt;
    uint tx_pks_256to511_oct_cnt;
    uint tx_pks_512to1023_oct_cnt;
    uint tx_pks_1024to1518_oct_cnt;
    uint tx_pks_1519tomax_oct_cnt;
    uint tx_pks_cnt;
    uint tx_mc_pks_cnt;
    uint tx_bc_pks_cnt;
    uint tx_pause_cnt;
    uint tx_defferred_cnt;
    uint tx_multi_col_cnt;
    uint tx_single_col_cnt;
    uint tx_late_col_cnt;
    uint tx_exc_col_err_cnt;
    uint tx_crs_drop_cnt;
    uint tx_pkt_drop_cnt;
    uint rx_pks_64oct_cnt;
    uint rx_pks_64to127_oct_cnt;
    uint rx_pks_128to255_oct_cnt;
    uint rx_pks_256to511_oct_cnt;
    uint rx_pks_512to1023_oct_cnt;
    uint rx_pks_1024to1518_oct_cnt;
    uint rx_pks_1519tomax_oct_cnt;
    uint rx_pkt_cnt;
    uint rx_mc_pks_cnt;
    uint rx_bc_pks_cnt;
    uint rx_us_pks_cnt;
    uint rx_os_pks_cnt;
    uint rx_frag_pks_cnt;
    uint rx_jabber_pks_cnt;
    uint rx_pause_cnt;
    uint rx_unk_ctl_cnt;
    uint rx_align_err_cnt;
    uint rx_fcs_err_cnt;
    uint tx_octets_cnt;
    uint rx_octets_cnt;
    uint rx_overrun_cnt;
    uint tx_underrun_cnt;
} hi_dp_logic_fe_cnt_s;

/* 0x100 ~0x157 */
typedef struct
{
    ulong pcs_rx_all_pkt_cnt;           /* [0x0100 64]pcs接收所有帧计数器         */
    ulong pcs_rx_fec_pkt_cnt;           /* [0x0108 64]pcs接收fec帧计数器      */
    ulong pcs_rx_fec_blk_cnt;           /* [0x0110 64]pcs接收fec块计数器      */
    uint  pcs_rx_10b_err_cnt;           /* [0x0118 32]pcs接收10b字符错误计数器        */
    uint  pcs_rx_err_pkt_cnt;           /* [0x011c 32]pcs接收判断为错帧计数器         */

    uint pcs_fec_short_pkt_cnt;         /* [0x0120 32]pcs接收fec短帧计数器        */
    uint pcs_fec_long_pkt_cnt;          /* [0x0124 32]pcs接收fec长帧计数器        */
    uint pcs_fec_cor_pkt_cnt;           /* [0x0128 32]pcs接收fec纠错帧计数器      */
    uint pcs_fec_cor_blk_cnt;           /* [0x012c 32]pcs接收fec纠错块计数器      */
    uint pcs_fec_cor_byte_cnt;         /* [0x0130 32]pcs接收fec纠错字节计数器        */
    uint pcs_fec_uncor_pkt_cnt;         /* [0x0134 32]pcs接收fec不可纠错帧计数器      */
    uint pcs_fec_uncor_blk_cnt;         /* [0x0138 32]pcs接收fec不可纠错块计数器      */
    uint reserved5;                    /* [0x013c 32]保留        */

    ulong pcs_tx_all_pkt_cnt;            /* [0x0140 64]pcs发送所有帧计数器         */
    ulong pcs_tx_fec_pkt_cnt;            /* [0x0148 64]pcs发送fec帧计数器      */
    ulong pcs_tx_fec_blk_cnt;            /* [0x0150 64]pcs发送fec块计数器      */
} hi_dp_logic_epon_pcs_cnt_s;

/* 0x158 ~ 0x17f */
typedef struct
{
    uint rs_rx_prm_err_cnt;             /* [0x0158 32]rs接收前导符错误计数器      */
    uint rs_rx_crc8_err_cnt;            /* [0x015c 32]rs接收crc8校验错误计数器        */

    ulong rs_rx_all_pkt_cnt;             /* [0x0160 64]rs接收所有正确帧计数器      */
    ulong rs_rx_all_byte_cnt;            /* [0x0168 64]rs接收所有正确字节计数器        */
    ulong rs_tx_all_pkt_cnt;            /* [0x0170 64]rs发送所有帧计数器      */
    ulong rs_tx_all_byte_cnt;            /* [0x0178 64]rs发送所有字节计数器        */
} hi_dp_logic_epon_rs_cnt_s;

/* 0x180~0x19f */
typedef struct
{
    uint mpcp_rx_fifo_ovr_pkt_cnt;      /* [0x0180 32]mpcp接收fifo溢出丢帧计数器      */
    uint data_rx_fifo_ovr_pkt_cnt;      /* [0x0184 32]数据帧接收fifo溢出丢帧计数器        */
    uint data_rx_fifo_drp_pkt_cnt;      /* [0x0188 32]数据帧接收fifo丢弃错帧计数器        */
    uint data_tx_fifo_und_pkt_cnt;      /* [0x018c 32]数据帧发送fifo欠载错帧计数器        */
    uint mac_rx_err_grant_cnt;          /* [0x0190 32]mac接收错误grant计数器      */
    uint mac_rx_b2b_grant_cnt;          /* [0x0194 32]mac接收背靠背grant计数器        */
    uint reserved6;                     /* [0x0198 32]保留        */
    uint reserved7;                    /* [0x019c 32]保留        */
} hi_dp_logic_epon_err_cnt_s;

/* 0x1c0 ~0x1ff */
typedef struct
{
    ulong mac_fcs_err_less_64b_pkt_cnt;  /* [0x1a0 64]mac接收fcs校验错帧计数器（帧长<64字节）  */
    ulong mac_fcs_err_64b_mtu_pkt_cnt;   /* [0x1a8 64]mac接收fcs校验错帧计数器（64字节≤帧长≤mtu）    */
    ulong mac_fcs_err_more_mtu_pkt_cnt;  /* [0x1b0 64]mac接收fcs校验错帧计数器（帧长>mtu）         */
    ulong mac_fcs_err_byte_cnt;         /* [0x1b8 64]mac接收fcs校验错字节计数器       */
    ulong mac_rx_all_data_pkt_cnt;      /* [0x1c0 64]mac接收所有fcs正确数据帧计数器       */
    ulong mac_rx_all_data_byte_cnt;      /* [0x1c8 64]mac接收所有fcs正确数据字节计数器         */
    ulong mac_rx_all_ctrl_pkt_cnt;       /* [0x1d0 64]mac接收fcs正确所有mac控制帧计数器        */
    ulong mac_rx_err_ctrl_pkt_cnt;       /* [0x1d8 64]mac接收错误mac控制帧计数器（不包括fcs错误）      */
    ulong mac_rx_saoff_oam_cnt;
    ulong mac_tx_all_data_pkt_cnt;      /* [0x1e0 64]mac发送所有数据帧计数器      */
    ulong mac_tx_all_data_byte_cnt;      /* [0x1e8 64]mac发送所有数据字节计数器        */
    ulong mac_tx_all_mpcp_pkt_cnt;      /* [0x1f0 64]mac发送所有mpcp帧计数器      */
} hi_dp_logic_epon_mac_cnt_s;

/* 0x200 ~ 0x2ff */
typedef struct
{
    ulong reserved9;                     /* [0x218 64]保留         */
} hi_dp_logic_epon_llid_grant_cnt;

/* 0x300 ~ 0x37f */
typedef struct
{
    ulong scb_rx_data_byte_cnt;          /* [0x300 64]下行scb接收数据字节计数器        */
    ulong scb_rx_uc_pkt_cnt;             /* [0x308 64]下行scb接收da单播帧计数器        */
    ulong scb_rx_mc_pkt_cnt;             /* [0x310 64]下行scb接收da多播帧计数器        */
    ulong scb_rx_bc_pkt_cnt;             /* [0x318 64]下行scb接收da广播帧计数器        */
    ulong mac_rx_dis_gate_pkt_cnt;       /* [0x320 64]下行接收discovery gate帧计数器       */
    ulong scb_rx_less_64b_pkt_cnt;      /* [0x328 64]下行scb接收数据帧计数器（帧长<64字节）       */
    ulong scb_rx_64b_pkt_cnt;            /* [0x330 64]下行scb接收数据帧计数器（帧长＝64字节）      */
    ulong scb_rx_65b_127b_pkt_cnt;       /* [0x338 64]下行scb接收数据帧计数器（65字节≤帧长≤127字节）         */
    ulong scb_rx_128b_255b_pkt_cnt;      /* [0x340 64]下行scb接收数据帧计数器（128字节≤帧长≤255字节） */
    ulong scb_rx_256b_511b_pkt_cnt;      /* [0x348 64]下行scb接收数据帧计数器（256字节≤帧长≤511字节） */
    ulong scb_rx_512b_1023b_pkt_cnt;     /* [0x350 64]下行scb接收数据帧计数器（512字节≤帧长≤1023字节） */
    ulong scb_rx_1024b_1518b_pkt_cnt;    /* [0x358 64]下行scb接收数据帧计数器（1024字节≤帧长≤1518字节） */
    ulong scb_rx_1519b_2000b_pkt_cnt;    /* [0x360 64]下行scb接收数据帧计数器（1519字节≤帧长≤2000字节）      */
    ulong scb_rx_more_2000_pkt_cnt;
    ulong scb_rx_more_mtu_pkt_cnt;       /* [0x368 64]下行scb接收数据帧计数器（帧长>mtu）      */
    ulong scb_rx_oam_pkt_cnt;
} hi_dp_logic_epon_scb_cnt_s;

/* 0x400 ~ 0x7ff */
typedef struct
{
    ulong llid_rx_all_grant_cnt;        /* [0x200 64]llid接收正确grant计数器       */
    ulong llid_rx_frpt_grant_cnt;       /* [0x208 64]llid接收force rpt grant计数器         */
    ulong llid_rx_grant_len_sum;        /* [0x210 64]llid接收grant长度累计总和         */
    uint  RESERVED12[58];
    ulong llid_rx_data_byte_cnt;        /* [0x400 64]llid接收数据字节计数器        */
    ulong llid_rx_uc_pkt_cnt;           /* [0x408 64]llid接收da单播帧计数器        */
    ulong llid_rx_mc_pkt_cnt;           /* [0x410 64]llid接收da多播帧计数器        */
    ulong llid_rx_bc_pkt_cnt;           /* [0x418 64]llid接收da广播帧计数器        */
    ulong llid_rx_gate_pkt_cnt;         /* [0x420 64]llid接收gate帧计数器      */
    ulong llid_rx_less_64b_pkt_cnt;     /* [0x428 64]llid接收数据帧计数器（帧长<64字节）       */
    ulong llid_rx_64b_pkt_cnt;          /* [0x430 64]llid接收数据帧计数器（帧长＝64字节）      */
    ulong llid_rx_65b_127b_pkt_cnt;     /* [0x438 64]llid接收数据帧计数器（65字节≤帧长≤127字节）     */
    ulong llid_rx_128b_255b_pkt_cnt;    /* [0x440 64]llid接收数据帧计数器（128字节≤帧长≤255字节）        */
    ulong llid_rx_256b_511b_pkt_cnt;    /* [0x448 64]llid接收数据帧计数器（256字节≤帧长≤511字节）    */
    ulong llid_rx_512b_1023b_pkt_cnt;   /* [0x450 64]llid接收数据帧计数器（512字节≤帧长≤1023字节）   */
    ulong llid_rx_1024b_1518b_pkt_cnt;  /* [0x458 64]llid接收数据帧计数器（1024字节≤帧长≤1518字节）      */
    ulong llid_rx_1519b_2000b_pkt_cnt;  /* [0x460 64]llid接收数据帧计数器（1519字节≤帧长≤2000字节）         */
    ulong llid_rx_more_2000b_pkt_cnt;
    ulong llid_rx_more_mtu_pkt_cnt;     /* [0x468 64]llid接收数据帧计数器（帧长>mtu）      */
    ulong llid_rx_oam_pkt_cnt;
    ulong llid_rx_otdr_gate_pkt_cnt;
    ulong llid_rx_cipher_pkt_cnt;
    uint  RESERVED13[28];
    ulong llid_tx_data_byte_cnt;        /* [0x800 64]llid发送data字节计数器        */
    ulong llid_tx_uc_pkt_cnt;           /* [0x808 64]llid发送da单播帧计数器        */
    ulong llid_tx_mc_pkt_cnt;           /* [0x810 64]llid发送da多播帧计数器        */
    ulong llid_tx_bc_pkt_cnt;           /* [0x818 64]llid发送da广播帧计数器        */
    ulong llid_tx_rpt_pkt_cnt;          /* [0x820 64]llid发送report帧计数器        */
    ulong llid_tx_less_64b_pkt_cnt;     /* [0x828 64]llid发送数据帧计数器（帧长<64字节）       */
    ulong llid_tx_64b_pkt_cnt;          /* [0x830 64]llid发送数据帧计数器（帧长＝64字节）      */
    ulong llid_tx_65b_127b_pkt_cnt;     /* [0x838 64]llid发送数据帧计数器（65字节≤帧长≤127字节）     */
    ulong llid_tx_128b_255b_pkt_cnt;    /* [0x840 64]llid发送数据帧计数器（128字节≤帧长≤255字节）        */
    ulong llid_tx_256b_511b_pkt_cnt;    /* [0x848 64]llid发送数据帧计数器（256字节≤帧长≤511字节）        */
    ulong llid_tx_512b_1023b_pkt_cnt;   /* [0x850 64]llid发送数据帧计数器（512字节≤帧长≤1023字节）       */
    ulong llid_tx_1024b_1518b_pkt_cnt;  /* [0x858 64]llid发送数据帧计数器（1024字节≤帧长≤1518字节）      */
    ulong llid_tx_1519b_2000b_pkt_cnt;  /* [0x860 64]llid发送数据帧计数器（1519字节≤帧长≤2000字节）      */
    ulong llid_tx_more_2000b_pkt_cnt;   /* [0x868 64]llid发送数据帧计数器（帧长>mtu）      */
    ulong llid_tx_more_mtu_pkt_cnt;     /* [0x868 64]llid发送数据帧计数器（帧长>mtu）      */
    ulong llid_tx_err_data_pkt_cnt;
    ulong llid_tx_oam_pkt_cnt;
    ulong llid_tx_data_frag_pkt_cnt;
    ulong llid_tx_oam_frag_pkt_cnt;
} hi_dp_logic_epon_llid_cnt_s;

typedef struct
{
    hi_dp_logic_epon_pcs_cnt_s  st_pcs_cnt;
    hi_dp_logic_epon_rs_cnt_s   st_rs_cnt;
    uint                        mpcp_rx_fifo_ovr_pkt;
    uint                        data_rx_fifo_ovr_pkt;
    uint                        oam_rx_buffer_ovr_pkt;
    uint                        mac_rx_err_grant;
    uint                        mac_rx_b2b_grant;
    uint                        data_tx_fifo_und_pkt;
    uint                        llid_tx_flush_pkt[HI_DP_EPON_LLID_NUM];
    uint                        llid_tx_readjust_sop[HI_DP_EPON_LLID_NUM];
    hi_dp_logic_epon_mac_cnt_s  st_mac_cnt;
    hi_dp_logic_epon_scb_cnt_s  st_scb_cnt;
    hi_dp_logic_epon_llid_cnt_s st_llidn_cnt[HI_DP_EPON_LLID_NUM];
} hi_dp_logic_epon_cnt_s;

/* BEGIN: Added by shengjing, 2010/11/23   PN:SD5113V200新增*/
typedef struct
{
    uint rx_notdrop_st;           /*  RT模块接收的非丢弃报文统计寄存器 */
    uint rx_drop_st;            /* RT模块接收的丢弃报文统计寄存器 */
    uint in_tunnel_st;         /* RT模块入隧道报文统计寄存器 */
    uint out_tunnel_st;        /* RT模块出隧道报文统计寄存器 */
    uint napt_st;               /* RT模块出隧道报文统计寄存器 */
    uint rt_fail_st;          /* 路由失败报文数目统计寄存器 */
    uint in_tunnel_fail_st;   /* 入隧道失败报文数目统计寄存器 */
    uint not_out_tunnel_st;   /* 未出隧道报文数目统计寄存器 */
    uint tx_notdrop_st;       /* RT模块发送的非丢弃报文统计寄存器 */
    uint tx_drop_st;          /* RT模块发送的丢弃报文统计寄存器 */
    uint bw_filter_drop_st;   /* 黑白名单过滤丢弃报文统计寄存器 */
    /* BEGIN: Added by l00164498, 2012/8/4 */
    uint v6_frag_rt_st;       /* IPv6报文统计*/
    /* END: Added by l00164498, 2012/8/4 */
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint v4_sip_err_st;       /* IPv4非法源IP统计寄存器 */
    uint v6_sip_err_st;       /* IPv6非法源IP统计寄存器 */
    uint nni_ip_filt_cnt;     /* NNI口的IP黑白名单过滤丢弃统计寄存器 */
    uint uni_ip_filt_cnt;     /* UNI口的IP黑白名单过滤丢弃统计寄存器 */
    /* BEGIN: Added by l00164498, 2012/8/4 */
    uint tnl_mtu_cnt[HI_DP_TNL_NUM];         /* 隧道的MTU丢弃报文统计寄存器 */
    uint tnl_drop_cnt[HI_DP_TNL_NUM];        /* 隧道的丢弃统计寄存器 */
    /* END: Added by l00164498, 2012/8/4 */
 #endif
} hi_dp_logic_rt_cnt_s;

typedef struct
{
    uint tmcm_st;           /*  出隧道报文统计计数器 */
    uint ipv4_mcm_st;        /* Ipv4非隧道组播匹配报文数目统计寄存器 */
    uint ip_tmcm_st;         /* Ip隧道组播匹配报文数目统计寄存器 */
    uint ipv6_mcm_st;        /* Ipv6非隧道组播匹配报文数目统计寄存器 */
    uint pppoe_tmcm_st;      /* PPPOE隧道组播匹配报文数目统计寄存器 */
    uint unk_mc_st;          /* 未知组播报文数目统计寄存器 */

    //5115 add
    uint tnl6rd_mcm_st;
} hi_dp_logic_mcm_cnt_s;

typedef struct
{
    uint ui_index;          /* 0 - 8 */

    /* rt */
    ulong rt_rx_frame_num;    /* 接收的Ipv6帧数目统计 */
    ulong rt_rx_byte_num;     /* 接收的Ipv6帧字节数统计 */
    ulong rt_rx_uc_frame_num; /* 接收的单播Ipv6帧数统计 */
    ulong rt_rx_mc_frame_num; /* 接收的组播Ipv6帧数统计 */

    /* txdma */
    ulong txdma_rx_frame_num;    /* 接收的Ipv6帧数目统计 */
    ulong txdma_rx_byte_num;     /* 接收的Ipv6帧字节数统计 */
    ulong txdma_rx_uc_frame_num; /* 接收的单播Ipv6帧数统计 */
    ulong txdma_rx_mc_frame_num; /* 接收的组播Ipv6帧数统计 */
} hi_dp_logic_ipv6_cnt_s;

/* END:   Added by shengjing, 2010/11/23 */

typedef struct
{
    uint ui_index;          /* 0 - 8 */

    /* rt */
    ulong rt_rx_frame_num;    /* 接收的Ipv4帧数目统计 */
    ulong rt_rx_byte_num;     /* 接收的Ipv4帧字节数统计 */
    ulong rt_rx_uc_frame_num; /* 接收的单播Ipv4帧数统计 */
    ulong rt_rx_mc_frame_num; /* 接收的组播Ipv4帧数统计 */

    /* BEGIN: Added by shengjing, 2011/12/27   问题单号:DTS2011122606515  */
    /* txdma */
    ulong txdma_rx_frame_num;    /* 接收的Ipv6帧数目统计 */
    ulong txdma_rx_byte_num;     /* 接收的Ipv6帧字节数统计 */
    ulong txdma_rx_uc_frame_num; /* 接收的单播Ipv6帧数统计 */
    ulong txdma_rx_mc_frame_num; /* 接收的组播Ipv6帧数统计 */
    /* END:   Added by shengjing, 2011/12/27 */
} hi_dp_logic_ipv4_cnt_s;

/*BEGING:Added by zhouqingsong,2011/11/11*/
typedef struct
{
    ulong cnt_pie_rx[MAX_PIE_CNT_NUM];
} hi_dp_pie_rx_cnt_s;
/*END:Added by zhouqingsong,2011/11/11*/

// #ifdef HI_SDK_DEBUG

/* PCP颜色选择 */
typedef enum
{
    HI_DP_QOS_PCP_SVLAN_E = 0,
    HI_DP_QOS_PCP_CVLAN_E,
} HI_DP_QOS_PCP_MODE_E;

typedef struct
{
    HI_DP_QOS_PCP_MODE_E em_pcp_mode[HI_DP_PORT_NUM];

    uint ui_prbs_add_err_bit;     //prbs插错bit数， add by lKF55238
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint      ui_prbs_stat_bit;
    uint      ui_prbs_sync_err_bit;
    HI_BOOL_E em_non_sync;    /* 失步状态 */
 #else
    uint ui_prbs_sta;
 #endif
    uint fec_ena_bit;
    uint fec_enc_enhance_bit;
 #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint ui_prbs_seed;    //prbs种子
    uint ui_prbs_mode;  //prbs 模式
    uint ui_prbs_tx_continue; //prbs发包模式
 #endif
} hi_dp_debug_attr_s;
// #endif

#endif /* __HI_DRV_DP_CNT_H__  */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus  */
