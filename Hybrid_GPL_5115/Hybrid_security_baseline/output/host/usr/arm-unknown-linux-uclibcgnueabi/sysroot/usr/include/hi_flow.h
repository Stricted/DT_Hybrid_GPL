/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_flow.h
  版 本 号   : 初稿
  作    者   : t00185981
  生成日期   : D2011_10_26

******************************************************************************/
#ifndef __HI_FLOW_H__
#define __HI_FLOW_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* for efc map */
#define HI_FLOW_MAPPING_MODE_PORT               0x00000001
#define HI_FLOW_MAPPING_MODE_DMAC               0x00000002
#define HI_FLOW_MAPPING_MODE_SMAC               0x00000004
#define HI_FLOW_MAPPING_MODE_DIP                0x00000008
#define HI_FLOW_MAPPING_MODE_SIP                0x00000010
#define HI_FLOW_MAPPING_MODE_GEMPORTID          0x00000020
#define HI_FLOW_MAPPING_MODE_LLID               0x00000040
#define HI_FLOW_MAPPING_MODE_ETHTYPE            0x00000080
#define HI_FLOW_MAPPING_MODE_SESSIONID          0x00000100
#define HI_FLOW_MAPPING_MODE_STPID              0x00000200
#define HI_FLOW_MAPPING_MODE_SPRI               0x00000400
#define HI_FLOW_MAPPING_MODE_SVID               0x00000800
#define HI_FLOW_MAPPING_MODE_CTPID              0x00001000
#define HI_FLOW_MAPPING_MODE_CPRI               0x00002000
#define HI_FLOW_MAPPING_MODE_CVID               0x00004000
#define HI_FLOW_MAPPING_MODE_IPPROTOCOL         0x00008000
#define HI_FLOW_MAPPING_MODE_IPVERSION          0x00010000
#define HI_FLOW_MAPPING_MODE_IPTOS              0x00020000
#define HI_FLOW_MAPPING_MODE_DPORT              0x00040000
#define HI_FLOW_MAPPING_MODE_SPORT              0x00080000
#define HI_FLOW_MAPPING_MODE_TAGTYPE            0x00100000
#define HI_FLOW_MAPPING_MODE_TAGSUM             0x00200000
#define HI_FLOW_MAPPING_MODE_DIPV6              0x00400000
#define HI_FLOW_MAPPING_MODE_SIPV6              0x00800000

#define HI_FLOW_MAPPING_MODE_REASON             0x00400000
#define HI_FLOW_MAPPING_MODE_DIPSEG             0x00800000
#define HI_FLOW_MAPPING_MODE_SIPSEG             0x01000000
#define HI_FLOW_MAPPING_MODE_DPORTSEG           0x02000000
#define HI_FLOW_MAPPING_MODE_SPORTSEG           0x04000000
#define HI_FLOW_MAPPING_MODE_FLAG               0x08000000

/* for efc act */
#define HI_FLOW_ACT_MODE_ADD_TAG                0x00000001
#define HI_FLOW_ACT_MODE_DEL_TAG                0x00000002
#define HI_FLOW_ACT_MODE_SPRI_TO_DEFINE         0x00000004              /*修改外层PRI为指定PRI*/
#define HI_FLOW_ACT_MODE_SPRI_TO_SSPRI          0x00000008              /*修改外层PRI为报文原外层PRI*/
#define HI_FLOW_ACT_MODE_SPRI_TO_SCPRI          0x00000010              /*修改外层PRI为报文原内层PRI*/
#define HI_FLOW_ACT_MODE_SPRI_TO_IPTOS          0x00000020              /*修改外层PRI为TPTOS*/
#define HI_FLOW_ACT_MODE_SPRI_TO_GEMPORT        0x00000040              /*修改外层PRI为下行GEMPORT映射的优先级*/
#define HI_FLOW_ACT_MODE_CPRI_TO_DEFINE         0x00000080              /*修改内层PRI为指定PRI*/
#define HI_FLOW_ACT_MODE_CPRI_TO_SSPRI          0x00000100              /*修改内层PRI为报文原外层PRI*/
#define HI_FLOW_ACT_MODE_CPRI_TO_SCPRI          0x00000200              /*修改内层PRI为报文原内层PRI*/
#define HI_FLOW_ACT_MODE_CPRI_TO_IPTOS          0x00000400              /*修改内层PRI为TPTOS*/
#define HI_FLOW_ACT_MODE_CPRI_TO_GEMPORT        0x00000800              /*修改内层PRI为下行GEMPORT映射的优先级*/
#define HI_FLOW_ACT_MODE_SVLAN_TO_DEFINE        0x00001000              /*修改外层VLAN为指定VLAN*/
#define HI_FLOW_ACT_MODE_SVLAN_TO_SSVLAN        0x00002000              /*修改外层VLAN为报文原外层VLAN*/
#define HI_FLOW_ACT_MODE_SVLAN_TO_SCVLAN        0x00004000              /*修改外层VLAN为报文原内层VLAN*/
#define HI_FLOW_ACT_MODE_CVLAN_TO_DEFINE        0x00008000              /*修改内层VLAN为指定VLAN*/
#define HI_FLOW_ACT_MODE_CVLAN_TO_SSVLAN        0x00010000              /*修改内层VLAN为报文原外层VLAN*/
#define HI_FLOW_ACT_MODE_CVLAN_TO_SCVLAN        0x00020000              /*修改内层VLAN为报文原内层VLAN*/
#define HI_FLOW_ACT_MARKING_PRI                 0x00040000              /*指定报文PRI marking值*/
#define HI_FLOW_ACT_MARKING_VLAN                0x00080000              /*指定报文VLAN marking值*/



/* for sfc map */
#define HI_FLOW_MAPPING_IGR                     0x00000001
#define HI_FLOW_MAPPING_VLAN                    0x00000002
#define HI_FLOW_MAPPING_PRI                     0x00000004
#define HI_FLOW_MAPPING_FID                     0x00000008
#define HI_FLOW_MAPPING_ETHTYPE                 0x00000010

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_FLOW_H__ */
