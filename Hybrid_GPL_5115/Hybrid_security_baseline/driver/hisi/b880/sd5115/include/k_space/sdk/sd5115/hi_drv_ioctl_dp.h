/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_ioctl_dp.h
  功能描述: DP IOCTL 命令字定义
  版本描述: V1.0

  创建日期: 2009年1月23日
  创建作者: 海思511x 芯片SDK 开发组

  修改记录:
******************************************************************************/

#ifndef __HI_DRV_IOCTL_DP__
#define __HI_DRV_IOCTL_DP__

#include  "sdk_autoconf.h"
#include  "hi_drv_dp.h"

/*********************************访问权限获取********************************************/
#define HI_DP_RIGHT_GET 0x0000
#define HI_DP_RIGHT_RELEASE 0x0001

/***********************************************************************************************/

/*********************************GLB CONFIG 模块********************************************/
#define HI_DP_GLB_ATTR_GET 0x0100
#define HI_DP_GLB_ATTR_SET 0x0101

#define HI_DP_GLB_STA_GET 0X0102

#define HI_DP_GLB_CNT_GET 0X0103

#define HI_DP_GLB_CNT_CLEAR 0X0104

#define HI_DP_GLB_SPEC_GET 0x0106

/***********************************************************************************************/
/*********************************   PON下行模块********************************************/
#define HI_DP_GPON_DN_ATTR_GET 0x0200
#define HI_DP_GPON_DN_ATTR_SET 0x0201
#define HI_DP_GPON_DN_GEMPORT_TABLE_ITEM_GET 0x0202
#define HI_DP_GPON_DN_GEMPORT_TABLE_ITEM_SET 0x0203
#define HI_DP_GPON_DN_GEMPORT_TABLE_CLR 0x0204
#define HI_DP_GPON_DN_OMCI_RECEIVE 0x0205
#define HI_DP_GPON_DN_CNT_GET 0x0206
#define HI_DP_GPON_DN_CNT_CLR 0x0207
#define HI_DP_GPON_DN_STA_GET 0x0208
#define HI_DP_GPON_DN_KEY_SWITCH 0x0209
#define HI_DP_GPON_DN_BWMAP_TABLE_ITEM_GET 0x020A
#define HI_DP_GPON_DN_TYPEB_OP 0x020B
#define HI_DP_GPON_DN_SPCNT_GET 0x020C

#define HI_DP_GPON_ALARM_GET 0x020D
#define HI_DP_GPON_ALARM_SET 0x020E
#define HI_DP_GPON_CNT_GET 0x020F
#define HI_DP_GPON_CNT_CLR 0x0210

#define HI_DP_GPON_ATTR_GET 0x0211
#define HI_DP_GPON_ATTR_SET 0x0212
#define HI_DP_GPON_GEMPORT_CNT_TABLE_ITEM_GET 0x0213
#define HI_DP_GPON_GEMPORT_CNT_TABLE_ITEM_SET 0x0214
#define HI_DP_GPON_GEMPORT_CNT_TABLE_ITEM_CLR 0x0215
#define HI_DP_GPON_STA_GET 0x0216
#define HI_DP_GPON_OMCI_RESET 0x0217
#define HI_DP_GPON_IDLE_BURST_ATTR_GET 0x0218
#define HI_DP_GPON_IDLE_BURST_ATTR_SET 0x0219


/***********************************************************************************************/
/*********************************   UNI PORT 模块************************************************/
#define HI_DP_UNI_ATTR_GET 0x0300
#define HI_DP_UNI_ATTR_SET 0x0301

#define HI_DP_UNI_CPU_PORT_ATTR_GET 0x0310
#define HI_DP_UNI_CPU_PORT_ATTR_SET 0x0311
#define HI_DP_UNI_CPU_PORT_CNT_GET 0x0312
#define HI_DP_UNI_CPU_PORT_CNT_CLR 0x0313

#define HI_DP_UNI_ETH_PORT_ATTR_GET 0x0320
#define HI_DP_UNI_ETH_PORT_ATTR_SET 0x0321
#define HI_DP_UNI_ETH_PORT_CNT_GET 0x0322
#define HI_DP_UNI_ETH_PORT_CNT_CLR 0x0323
#define HI_DP_UNI_ETH_PORT_STA_GET 0x0324
#define HI_DP_UNI_ETH_PORT_RESET 0x0325

#define HI_DP_UNI_TAG_SFT_TABLE_ITEM_SET 0x0340
#define HI_DP_UNI_TAG_SFT_TABLE_ITEM_GET 0x0341
#define HI_DP_UNI_HYBRID_TABLE_ITEM_SET 0x0342
#define HI_DP_UNI_HYBRID_TABLE_ITEM_GET 0x0343

#define HI_DP_UNI_CASCADE_ATTR_SET 0x0344
#define HI_DP_UNI_CASCADE_ATTR_GET 0x0345

#define HI_DP_INNER_MDIO_REG_WRITE 0x0346
#define HI_DP_INNER_MDIO_REG_READ 0x0347
#define HI_DP_INNER_MDIO_EXREG_WRITE 0x0348
#define HI_DP_INNER_MDIO_EXREG_READ 0x0349

/**********************************  NNI PORT  ********************************/

#define HI_DP_NNI_ETH_PORT_ATTR_GET 0x0350
#define HI_DP_NNI_ETH_PORT_ATTR_SET 0x0351
#define HI_DP_NNI_ETH_PORT_CNT_GET 0x0352
#define HI_DP_NNI_ETH_PORT_CNT_CLR 0x0353
#define HI_DP_NNI_ETH_PORT_RESET 0x0354
#define HI_DP_NNI_TAG_SFT_TABLE_ITEM_GET 0x0355
#define HI_DP_NNI_TAG_SFT_TABLE_ITEM_SET 0x0356
#define HI_DP_NNI_HYBRID_TABLE_ITEM_GET 0x0357
#define HI_DP_NNI_HYBRID_TABLE_ITEM_SET 0x0358

/***********************************************************************************************/
/*********************************   FILTER 模块***************************************/
#define HI_DP_VLAN_FILTER_TABLE_ITEM_GET 0x0400
#define HI_DP_VLAN_FILTER_TABLE_ITEM_SET 0x0401

#define HI_DP_SMAC_FILTER_TABLE_ITEM_GET 0x0402
#define HI_DP_SMAC_FILTER_TABLE_ITEM_SET 0x0403

#define HI_DP_VLAN_FILTER_ATTR_GET 0x0404
#define HI_DP_VLAN_FILTER_ATTR_SET 0x0405

#define HI_DP_SMAC_FILTER_CNT_GET 0x0406
#define HI_DP_SMAC_FILTER_CNT_CLR 0x0407

#define HI_DP_IP_FILTER_TABLE_ITEM_GET 0x0408
#define HI_DP_IP_FILTER_TABLE_ITEM_SET 0x0409

#define HI_DP_SECURITY_ATTR_GET 0x040A
#define HI_DP_SECURITY_ATTR_SET 0x040B

#define HI_DP_SECURITY_CNT_GET 0x040C
#define HI_DP_SECURITY_CNT_CLR 0x040D
#define HI_DP_SECURITY_STA_GET 0x040E

/***********************************************************************************************/
/*********************************   FLOW 模块******************************************/
#define HI_DP_FLOW_ATTR_GET 0x0500
#define HI_DP_FLOW_ATTR_SET 0x0501

#define HI_DP_FLOW_FC_TABLE_ITEM_GET 0x0502
#define HI_DP_FLOW_FC_TABLE_ITEM_SET 0x0503

#define HI_DP_FLOW_EFC_TABLE_ITEM_GET 0x0504
#define HI_DP_FLOW_EFC_TABLE_ITEM_SET 0x0505

#define HI_DP_FLOW_SFC_TABLE_ITEM_GET 0x0506
#define HI_DP_FLOW_SFC_TABLE_ITEM_SET 0x0507

#define HI_DP_FLOW_CNT_GET 0x0508
#define HI_DP_FLOW_CNT_CLR 0x0509

#define HI_DP_FLOW_TCFL_REP_TABLE_ITEM_GET 0x050A
#define HI_DP_FLOW_TCFL_REP_TABLE_ITEM_SET 0x050B

#define HI_DP_FLOW_TUNNEL_TCFL_REP_TABLE_ITEM_GET 0x050C
#define HI_DP_FLOW_TUNNEL_TCFL_REP_TABLE_ITEM_SET 0x050D

#ifdef CONFIG_HISI_SD5115_MP_MPW
 #define HI_DP_FLOW_CNT_TABLE_GET 0x050E
 #define HI_DP_FLOW_CNT_TABLE_CLR 0x050F
#endif

#ifdef CONFIG_HISI_SD5115_MP_PILOT
 #define HI_DP_IGR_FLOW_CNT_TABLE_GET 0x050E
 #define HI_DP_IGR_FLOW_CNT_TABLE_CLR 0x050F

 #define HI_DP_EGR_FLOW_CNT_TABLE_GET 0x0513
 #define HI_DP_EGR_FLOW_CNT_TABLE_CLR 0x0514

 #define HI_DP_FLOW_CNT_MATCH_TABLE_GET 0x0515
 #define HI_DP_FLOW_CNT_MATCH_TABLE_SET 0x0516
#endif

#define HI_DP_FLOW_EFC_TABLE_ITEM_CLR 0x0510
#define HI_DP_FLOW_SFC_TABLE_ITEM_CLR 0x0511

#define HI_DP_FLOW_RTP_REQ 0x0512

#define HI_DP_FLOW_SFC_MAP_TABLE_ITEM_GET 0x0517
#define HI_DP_FLOW_SFC_MAP_TABLE_ITEM_SET 0x0518
#define HI_DP_FLOW_SFC_QOS_TABLE_ITEM_GET 0x0519
#define HI_DP_FLOW_SFC_QOS_TABLE_ITEM_SET 0x051A

#define HI_DP_THRU_REASON_CNT_GET 0x051B
#define HI_DP_TOCPU_REASON_CNT_GET 0x051C
#define HI_DP_THRU_REASON_CNT_CLR 0x051D
#define HI_DP_TOCPU_REASON_CNT_CLR 0x051E

/***********************************************************************************************/
/*********************************   SPECIAL PKT 模块***************************************/
#define HI_DP_SPECIAL_PKT_ATTR_GET 0x0600
#define HI_DP_SPECIAL_PKT_ATTR_SET 0x0601

#define HI_DP_SPECIAL_PKT_CNT_GET 0x0602
#define HI_DP_SPECIAL_PKT_CNT_CLR 0x0603

#define HI_DP_SPECIAL_PKT_TRANS_TABLE_ITEM_GET 0x0604
#define HI_DP_SPECIAL_PKT_TRANS_TABLE_ITEM_SET 0x0605

/***********************************************************************************************/
/*********************************   L2 模块************************************************/
#define HI_DP_L2_ATTR_GET 0x0700
#define HI_DP_L2_ATTR_SET 0x0701

#define HI_DP_L2_ACT_TABLE_ITEM_GET 0x0702
#define HI_DP_L2_ACT_TABLE_ITEM_SET 0x0703
#define HI_DP_L2_ACT_TABLE_ITEM_CLR 0x0705

#define HI_DP_L2_MAC_TABLE_ITEM_GET 0x0704

#define HI_DP_L2_MC_ADD 0x0706
#define HI_DP_L2_MC_DEL 0x0707
#define HI_DP_L2_MC_GET 0x0708

#define HI_DP_L2_MC_USER_ADD 0x0709
#define HI_DP_L2_MC_USER_DEL 0x070A

#define HI_DP_L2_MC_USERS_DEL 0x070B
#define HI_DP_L2_MC_USERS_GET 0x070C

#define HI_DP_L2_CNT_GET 0x0711

#define HI_DP_L2_MAC_ADD 0x0712
#define HI_DP_L2_MAC_DEL 0x0713
#define HI_DP_L2_MAC_GET 0x0714

#define HI_DP_L2_CNT_CLR 0x0715

#define HI_DP_L2_VLAN_BC_ADD 0x0716
#define HI_DP_L2_VLAN_BC_DEL 0x0717
#define HI_DP_L2_VLAN_BC_GET 0x0718
#define HI_DP_L2_VLAN_DEL 0x0719
#define HI_DP_L2_IP_MC_TABLE_ITEM_GET 0x0720
#define HI_DP_L2_IP_MC_TABLE_ITEM_SET 0x0721

#define HI_DP_L2_MC_FILTER_ADD 0x0722
#define HI_DP_L2_MC_FILTER_DEL 0x0723
#define HI_DP_L2_MC_FILTER_GET 0x0724
#define HI_DP_L2_MC_FILTER_ITEM_GET 0x0725
#define HI_DP_L2_MAC_TABLE_RESET 0x0726

/* BEGIN: Added by w00180499, 2012/7/20 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_L2_MC_ST_ATTR_GET 0x0727
#define HI_DP_L2_MC_ST_ATTR_SET 0x0728
#endif
/* END:   Added by w00180499, 2012/7/20 */

/***********************************************************************************************/
/*********************************   L3 模块************************************************/

#define HI_DP_L3_ATTR_GET 0x0800
#define HI_DP_L3_ATTR_SET 0x0801

#define HI_DP_L3_EX_OUTNET_FILTER_TABLE_ITEM_GET 0x0804
#define HI_DP_L3_EX_OUTNET_FILTER_TABLE_ITEM_SET 0x0805

#define HI_DP_L3_NAPT_TABLE_ITEM_GET 0x0806
#define HI_DP_L3_NAPT_TABLE_CLR 0x0807

#define HI_DP_L3_CNT_GET 0x0808
#define HI_DP_L3_CNT_CLR 0x0809

#define HI_DP_L3_NAPT_UP_TABLE_ITEM_ADD 0x080A
#define HI_DP_L3_NAPT_UP_TABLE_ITEM_DEL 0x080B
#define HI_DP_L3_NAPT_UP_TABLE_ITEM_QUERY 0x080C

#define HI_DP_L3_REPLACE_TABLE_ITEM_GET 0x080D
#define HI_DP_L3_REPLACE_TABLE_ITEM_SET 0x080E

#define HI_DP_L3_NH_TABLE_ITEM_GET 0x080F
#define HI_DP_L3_NH_TABLE_ITEM_SET 0x0810

#define HI_DP_L3_RT_TABLE_ITEM_GET 0x0811
#define HI_DP_L3_RT_TABLE_ITEM_SET 0x0812

#define HI_DP_L3_RT_ACT_TABLE_ITEM_GET 0x0813
#define HI_DP_L3_RT_ACT_TABLE_ITEM_SET 0x0814

#define HI_DP_L3_OIF1_STA_GET 0X081A
#define HI_DP_L3_STA_GET 0X081B
#define HI_DP_L3_NH_STA_GET 0X081C
#define HI_DP_L3_RT_STA_GET 0X081D
#define HI_DP_L3_ITRT_STA_GET 0X081E
#define HI_DP_L3_OTRT_STA_GET 0X081F

#define HI_DP_L3_OUT_TUNNEL_SIP_TABLE_ITEM_GET 0x0820
#define HI_DP_L3_OUT_TUNNEL_SIP_TABLE_ITEM_SET 0x0821

#define HI_DP_L3_IN_TUNNEL_RT_TABLE_ITEM_GET 0x0822
#define HI_DP_L3_IN_TUNNEL_RT_TABLE_ITEM_SET 0x0823

#define HI_DP_L3_OUT_TUNNEL_MATCH_TABLE_ITEM_GET 0x0824
#define HI_DP_L3_OUT_TUNNEL_MATCH_TABLE_ITEM_SET 0x0825

#define HI_DP_L3_IN_TUNNEL_HEAD_TABLE_ITEM_GET 0x0826
#define HI_DP_L3_IN_TUNNEL_HEAD_TABLE_ITEM_SET 0x0827

#define HI_DP_L3_NAPT_TABLE_ITEM_ADD 0x0828
#define HI_DP_L3_NAPT_TABLE_ITEM_DEL 0x0829
#define HI_DP_L3_NAPT_TABLE_ITEM_QUERY 0x082A

#define HI_DP_L3_IN_6RD_TUNNEL_HEAD_TABLE_ITEM_GET 0x082B
#define HI_DP_L3_IN_6RD_TUNNEL_HEAD_TABLE_ITEM_SET 0x082C

#define HI_DP_L3_NH_TABLE_ITEM_CLR 0x082D
#define HI_DP_L3_RT_TABLE_ITEM_CLR 0x082E
#define HI_DP_L3_RT_ACT_TABLE_ITEM_CLR 0x082F

#define HI_DP_L3_NAPT_POINT_CNT_TABLE_ITEM_GET 0x0830
#define HI_DP_L3_NAPT_POINT_CNT_TABLE_ITEM_SET 0x0831
#define HI_DP_L3_NAPT_POINT_CNT_TABLE_ITEM_CLR 0x0832
#define HI_DP_L3_NAPT_POINT_CNT_GET 0x0833
#define HI_DP_L3_NAPT_POINT_CNT_CLR 0x0834
 #define HI_DP_L3_NAPT_TABLE_CLR_ITEM 0x0835
#define HI_DP_L3_ALARM_GET 0x0836
#define HI_DP_L3_ALARM_SET 0x0837

/* BEGIN: Added by l00164498, 2012/8/6 */
#ifdef CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_L3_RSP_TABLE_ITEM_GET 0x0838
#define HI_DP_L3_RSP_TABLE_ITEM_SET 0x0839
#endif

#define HI_DP_L3_WAN_CNT_GET 0x083a
#define HI_DP_L3_WAN_CNT_CLR 0x083b
/* END: Added by l00164498, 2012/8/6 */
/***********************************************************************************************/
/*********************************   OAM 模块************************************************/

#define HI_DP_OAM_ATTR_GET 0x0900
#define HI_DP_OAM_ATTR_SET 0x0901
#define HI_DP_OAM_STA_GET 0x0902

#define HI_DP_OAM_CFM_TABLE_ITEM_GET 0x0903
#define HI_DP_OAM_CFM_TABLE_ITEM_SET 0x0904

#define HI_DP_OAM_CNT_GET 0x0905
#define HI_DP_OAM_CNT_CLR 0x0906

#define HI_DP_OAM_ALARM_GET 0x0907
#define HI_DP_OAM_ALARM_SET 0x0908
#ifdef SUPPORT_OAM_1731
#define HI_DP_OAM_TXFCL_INC 0x0909
#define HI_DP_OAM_1731_ATTR_GET 0x090a
#define HI_DP_OAM_1731_ATTR_SET 0x090b

#endif

/***********************************************************************************************/
/*********************************   QOS  模块 **************************************************/
#define HI_DP_QOS_SCAR_GET 0x0A00
#define HI_DP_QOS_SCAR_SET 0x0A01
#define HI_DP_QOS_ECAR_GET 0x0A02
#define HI_DP_QOS_ECAR_SET 0x0A03
#define HI_DP_QOS_TOCPU_REASON_SET 0x0A04
#define HI_DP_QOS_TOCPU_REASON_GET 0x0A05
#define HI_DP_QOS_TOCPU_REASON_CAR_SET 0x0A06
#define HI_DP_QOS_TOCPU_REASON_CAR_GET 0x0A07
#define HI_DP_QOS_GMAP_SET 0x0A08
#define HI_DP_QOS_GMAP_GET 0x0A09
#define HI_DP_QOS_FCAR_SET 0x0A0A
#define HI_DP_QOS_FCAR_GET 0x0A0B
#define HI_DP_QOS_GCAR_SET 0x0A0C
#define HI_DP_QOS_GCAR_GET 0x0A0D
#define HI_DP_QOS_QMAP_SET 0x0A0E
#define HI_DP_QOS_QMAP_GET 0x0A0F
#define HI_DP_QOS_QUEUE_SET 0x0A10
#define HI_DP_QOS_QUEUE_GET 0x0A11
#define HI_DP_QOS_FSCH_SET 0x0A12
#define HI_DP_QOS_FSCH_GET 0x0A13
#define HI_DP_QOS_GSCH_SET 0x0A14
#define HI_DP_QOS_GSCH_GET 0x0A15
#define HI_DP_QOS_FWRED_SET 0x0A16
#define HI_DP_QOS_FWRED_GET 0x0A17
#define HI_DP_QOS_GWRED_SET 0x0A18
#define HI_DP_QOS_GWRED_GET 0x0A19
#define HI_DP_QOS_SHAPING_SET 0x0A1A
#define HI_DP_QOS_SHAPING_GET 0x0A1B
#define HI_DP_QOS_ATTR_SET 0x0A1C
#define HI_DP_QOS_ATTR_GET 0x0A1D
#define HI_DP_QOS_QUEUE_CNT_GET 0x0A1E
#define HI_DP_QOS_QUEUE_CNT_CLR 0x0A1F
#define HI_DP_QOS_CNT_GET 0x0A20
#define HI_DP_QOS_CNT_CLR 0x0A21
#define HI_DP_QOS_STA_GET 0x0A22
#define HI_DP_QOS_DEBUG 0x0A23

#define HI_DP_QOS_EPFC_MAP_TABLE_SET 0x0A30
#define HI_DP_QOS_EPFC_MAP_TABLE_GET 0x0A31
#define HI_DP_QOS_FCAR_MAP_TABLE_SET 0x0A32
#define HI_DP_QOS_FCAR_MAP_TABLE_GET 0x0A33
#define HI_DP_QOS_GCAR_MAP_TABLE_SET 0x0A34
#define HI_DP_QOS_GCAR_MAP_TABLE_GET 0x0A35
#define HI_DP_QOS_QUEUE_SHAP_TABLE_SET 0x0A36
#define HI_DP_QOS_QUEUE_SHAP_TABLE_GET 0x0A37
#define HI_DP_QOS_GS_SHAP_TABLE_SET 0x0A38
#define HI_DP_QOS_GS_SHAP_TABLE_GET 0x0A39
#define HI_DP_QOS_FWRED_MAP_TABLE_SET 0x0A3A
#define HI_DP_QOS_FWRED_MAP_TABLE_GET 0x0A3B
#define HI_DP_QOS_GWRED_MAP_TABLE_SET 0x0A3C
#define HI_DP_QOS_GWRED_MAP_TABLE_GET 0x0A3D

#ifdef CONFIG_HISI_SD5115_MP_PILOT
 #define HI_DP_QOS_EQG_ATTR_SET 0x0A3E
 #define HI_DP_QOS_EQG_ATTR_GET 0x0A3F
 #define HI_DP_QOS_QG_ATTR_SET 0x0A40
 #define HI_DP_QOS_QG_ATTR_GET 0x0A41
 #define HI_DP_QOS_PRI_RESET_SET 0x0A42
 #define HI_DP_QOS_PRI_RESET_GET 0x0A43
 #define HI_DP_QOS_DROP_REASON_GET 0x0A44
 #define HI_DP_QOS_DROP_REASON_CLR 0x0A45

 #ifdef CONFIG_HISI_SD5115_TYPE_T
  #define HI_DP_QOS_QUEUE_SET_5115T 0x0A46
  #define HI_DP_QOS_QUEUE_GET_5115T 0x0A47
 #endif
#endif

/***********************************************************************************************/
/*********************************   PON UP 模块********************************************/

#define HI_DP_GPON_UP_ATTR_GET 0x0B00
#define HI_DP_GPON_UP_ATTR_SET 0x0B01

#define HI_DP_GPON_UP_OMCI_SEND 0x0B02
#define HI_DP_GPON_UP_PLOAM_SEND 0x0B03

#define HI_DP_GPON_UP_MAP_TABLE_ITEM_GET 0x0B04
#define HI_DP_GPON_UP_MAP_TABLE_ITEM_SET 0x0B05

#define HI_DP_GPON_UP_TCONT_TABLE_ITEM_GET 0x0B06
#define HI_DP_GPON_UP_TCONT_TABLE_ITEM_SET 0x0B07

#define HI_DP_GPON_UP_CNT_GET 0x0B08
#define HI_DP_GPON_UP_CLEAN_TX_PLOAM 0x0B0C
#define HI_DP_GPON_UP_STA_GET 0x0B0D
#define HI_DP_GPON_UP_CNT_CLR 0x0B0E

#define HI_DP_GPON_UP_PLOAM_READY 0x0B0F

/***********************************************************************************************/
/*********************************   CNT 模块********************************************/
#define HI_DP_CNT_GLB_GET 0x0D00
#define HI_DP_CNT_CPURTX_GET 0x0D01
#define HI_DP_CNT_RXDMA_GET 0x0D02
#define HI_DP_CNT_TXDMA_GET 0x0D03
#define HI_DP_CNT_PA_GET 0x0D04
#define HI_DP_CNT_FC_GET 0x0D05
#define HI_DP_CNT_EFC_GET 0x0D06
#define HI_DP_CNT_CAP_GET 0x0D07
#define HI_DP_CNT_L2_GET 0x0D08
#define HI_DP_CNT_L3_GET 0x0D09
#define HI_DP_CNT_MTP_GET 0x0D0A
#define HI_DP_CNT_MCD_GET 0x0D0B
#define HI_DP_CNT_OAM_GET 0x0D0C
#define HI_DP_CNT_QM_GET 0x0D0D
#define HI_DP_CNT_QUEUE_GET 0x0D0E
#define HI_DP_CNT_DROP_REASON_GET 0x0D0F
#define HI_DP_CNT_THRU_REASON_GET 0x0D10
#define HI_DP_CNT_TOCPU_REASON_GET 0x0D11
#define HI_DP_CNT_AM_GET 0x0D13

#define HI_DP_CNT_OAM_CLEAR 0x0D14
#define HI_DP_CNT_QUEUE_CLEAR 0x0D15
#define HI_DP_CNT_REASON_CLEAR 0x0D16
#define HI_DP_CNT_SPECIAL_PKT_CLEAR 0x0D17

#define HI_DP_CNT_GPON_EXT_GET 0x0D18
#define HI_DP_CNT_DGTC_SYS_GET 0x0D19
#define HI_DP_CNT_DGTC_LINE_GET 0x0D1A
#define HI_DP_CNT_DGEM_SYS_GET 0x0D1B
#define HI_DP_CNT_UGMAC_SYS_GET 0x0D1C
#define HI_DP_CNT_UGMAC_LINE_GET 0x0D1D
#define HI_DP_CNT_EPS_PON_GET 0x0D27
#define HI_DP_CNT_GPON_CLR 0x0D2A
#define HI_DP_CNT_GE_GET 0x0D1E
#define HI_DP_CNT_FE_GET 0x0D1F

#define HI_DP_CNT_EPON_GET 0x0D20
#define HI_DP_CNT_EPON_CLR 0x0D21

#define HI_DP_CNT_MCM_GET 0x0D22
#define HI_DP_CNT_RT_GET 0x0D23
#define HI_DP_CNT_IPV6_GET 0x0D24
#define HI_DP_CNT_IPV6_CLR 0x0D25
#define HI_DP_CNT_EFC_DEBUG_CLR 0x0D26

#define HI_DP_CNT_IPV4_GET 0x0D29
#define HI_DP_CNT_IPV4_CLR 0x0D28

#define HI_DP_CNT_SFC_GET 0x0D2A

#define HI_DP_CNT_PIE_GET 0x0D2B
#define HI_DP_CNT_PIE_CLR 0x0D2C

//#ifdef HI_SDK_DEBUG
#define HI_DP_DEBUG_ATTR_SET 0x0D2E
#define HI_DP_DEBUG_ATTR_GET 0x0D2F
//#endif

/***********************************************************************************************/
/*********************************   EPON公共模块 ********************************************/
#define HI_DP_EPON_ATTR_GET 0x0E00
#define HI_DP_EPON_ATTR_SET 0x0E01
#define HI_DP_EPON_ALARM_GET 0x0E02
#define HI_DP_EPON_ALARM_SET 0x0E03
#define HI_DP_EPON_CNT_GET 0x0E04
#define HI_DP_EPON_CNT_CLR 0x0E05
#define HI_DP_EPON_BANDWIDTH_GET 0x0E06
#define HI_DP_EPON_LLID_CNT_GET 0x0E07
#define HI_DP_EPON_LLID_CNT_CLR 0x0E08

/*********************************   EPON下行模块********************************************/
#define HI_DP_EPON_DN_ATTR_GET 0x0E50
#define HI_DP_EPON_DN_ATTR_SET 0x0E51
#define HI_DP_EPON_DN_LLID_TABLE_ITEM_GET 0x0E52
#define HI_DP_EPON_DN_LLID_TABLE_ITEM_SET 0x0E53
#define HI_DP_EPON_DN_CNT_GET 0x0E54
#define HI_DP_EPON_DN_STA_GET 0x0E55
#define HI_DP_EPON_DN_CNT_CLR 0x0E56

/***********************************************************************************************/
/*********************************   EPON上行模块********************************************/
#define HI_DP_EPON_UP_ATTR_GET 0x0F00
#define HI_DP_EPON_UP_ATTR_SET 0x0F01
#define HI_DP_EPON_UP_MAP_TABLE_ITEM_GET 0x0F02
#define HI_DP_EPON_UP_MAP_TABLE_ITEM_SET 0x0F03
#define HI_DP_EPON_UP_MPCP_SEND 0x0F04
#define HI_DP_EPON_UP_OAM_SEND 0x0F05
#define HI_DP_EPON_UP_CNT_GET 0x0F06
#define HI_DP_EPON_UP_STA_GET 0x0F07
#define HI_DP_EPON_UP_RPT_TABLE_ITEM_GET 0x0F08
#define HI_DP_EPON_UP_RPT_TABLE_ITEM_SET 0x0F09
#define HI_DP_EPON_UP_CNT_CLR 0x0F0A

/***********************************************************************************************/
/*********************************   PRBS模块********************************************/
#define HI_DP_PRBS_ATTR_GET 0x1000
#define HI_DP_PRBS_ATTR_SET 0x1001
#define HI_DP_PRBS_STA_GET 0x1002
#define HI_DP_PRBS_CNT_GET 0x1003
#define HI_DP_PRBS_CNT_CLR 0x1004

/***********************************************************************************************/

/*for debug*/
#define HI_DP_PHYSICAL_ADDR_GET 0xf000
#define HI_DP_PHYSICAL_ADDR_SET 0xf001
#define HI_DP_SMD_COMMAND 0xf002

#endif  /*end of */
