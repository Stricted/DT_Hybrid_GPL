/* *****************************************************************************
                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
******************************************************************************
  �� �� ��   : hi_dp_dp_api.h
  �� �� ��   : ����
  ��    ��   : f00125610
  ��������   : 2008��9��26��
  ����޸�   :
  ��������   : SD5113 SDK�ӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��9��26��
    ��    ��   : f00125610
    �޸�����   : �����ļ�

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
#define HI_DP_LP_INT_NUM             151 //147  FPGA��������

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

/* BEGIN: Added by shengjing, 2010/10/16   PN:SD5113V200����*/
#define HI_DP_FID_NUM_V200 324
/* END:   Added by shengjing, 2010/10/16 */

/* BEGIN: Added by ������, 2012/2/9 */
#define HI_DP_1731_MEP_NUM 1
/* END:   Added by ������, 2012/2/9 */

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
/* BEGIN: Added by shengjing, 2010/10/16   PN:SD5113V200����*/
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
#define MAX_PIE_CNT_NUM 200         //���Ϊ200������

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
#define HI_DP_TNL_NUM   8 // �����Ŀ
/* END: Added by l00164498, 2012/8/4 */

#ifdef CONFIG_HISI_SD5115_MP_PILOT
typedef enum
{
    HI_DP_CNT_32BIT_32BIT_E = 0,
    HI_DP_CNT_64BIT_E,
} HI_DP_CNT_MODE_E;
#endif

/* ���Ķ�����reason */
typedef enum
{
    HI_DP_DROP_REASON_NOT_DROP_E = 0,           /* ����                              */
    HI_DP_DROP_REASON_MAC_ERR_E,                /* MAC error                         */
    HI_DP_DROP_REASON_PON_FCS_ERR_E,            /* PON FCS error                     */
    HI_DP_DROP_REASON_JUMBO_E,                  /* ����֡����                        */
    HI_DP_DROP_REASON_FRAG_E,                   /* ����֡����                        */
    HI_DP_DROP_REASON_CPU_POINT_NO_EGR_E,       /* CPUָ��ת�������õ�egressΪȫ0  */
    HI_DP_DROP_REASON_INVALID_SMAC_E,           /* SMAC�Ƿ�����  */
    HI_DP_DROP_REASON_EFC_MATCH0_E,             /* EFCƥ�䶪��ԭ��0  */
    HI_DP_DROP_REASON_EFC_MATCH1_E,             /* EFCƥ�䶪��ԭ��1  */
    HI_DP_DROP_REASON_EFC_MATCH2_E,             /* EFCƥ�䶪��ԭ��2  */
    HI_DP_DROP_REASON_EFC_MATCH3_E,             /* EFCƥ�䶪��ԭ��3  */
    HI_DP_DROP_REASON_EFC_MATCH4_E,             /* EFCƥ�䶪��ԭ��4  */
    HI_DP_DROP_REASON_EFC_MATCH5_E,             /* EFCƥ�䶪��ԭ��5  */
    HI_DP_DROP_REASON_EFC_MATCH6_E,             /* EFCƥ�䶪��ԭ��6  */
    HI_DP_DROP_REASON_EFC_MATCH7_E,             /* EFCƥ�䶪��ԭ��7  */
    HI_DP_DROP_REASON_EFC_MATCH8_E,             /* EFCƥ�䶪��ԭ��8  */
    HI_DP_DROP_REASON_EFC_MATCH9_E,             /* EFCƥ�䶪��ԭ��9  */
    HI_DP_DROP_REASON_EFC_MATCH10_E,            /* EFCƥ�䶪��ԭ��10  */
    HI_DP_DROP_REASON_EFC_MATCH11_E,            /* EFCƥ�䶪��ԭ��11  */
    HI_DP_DROP_REASON_EFC_MATCH12_E,            /* EFCƥ�䶪��ԭ��12  */
    HI_DP_DROP_REASON_EFC_MATCH13_E,            /* EFCƥ�䶪��ԭ��13  */
    HI_DP_DROP_REASON_EFC_MATCH14_E,            /* EFCƥ�䶪��ԭ��14  */
    HI_DP_DROP_REASON_EFC_MATCH15_E,            /* EFCƥ�䶪��ԭ��15  */
    HI_DP_DROP_REASON_EGR_VLAN_CHECK_E,         /*23: ����VLAN��鶪��  */
    HI_DP_DROP_REASON_PORT_ISOLATION_E,         /*24: �˿ڸ��붪��  */
    HI_DP_DROP_REASON_VLAN_TAG_E,               /*25: VLAN_Tag���Ĺ���  */
    HI_DP_DROP_REASON_IGR_CAR_FCTRL_E,          /*26: ��˿�����  */
    HI_DP_DROP_REASON_INVALID_USER_VLAN_E,      /*27: �û�VLAN���ˣ�VLAN��ڼ��ʧ�ܣ�  */
    HI_DP_DROP_REASON_NAPT_IP_FRAG_E,           /*28: IP��Ƭ������NAPT���� */
    HI_DP_DROP_REASON_IGMP_PPPOE_E,             /*29: IGMP on  PPPoE���Ĺ���  */
    HI_DP_DROP_REASON_ARP_E,                    /*30: ARP����  */
    HI_DP_DROP_REASON_RARP_E,                   /*31: RARP����  */
    HI_DP_DROP_REASON_PPPOE_DIS_E,              /*32: PPPoE Discovery����               */
    HI_DP_DROP_REASON_8021X_NOT_AUTH_E,         /*33: 802.1x��֤δ��Ȩ״̬�˿ڹر�      */
    HI_DP_DROP_REASON_STP_RSTP_E,               /*34: STP/RSTP����                      */
    HI_DP_DROP_REASON_WHITELIST_E,              /*35: ����������                        */
    HI_DP_DROP_REASON_DHCP_E,                   /*36: DHCP���Ĺ���                      */
    HI_DP_DROP_REASON_IGMP_IPOE_E,              /*37: IGMP on  IPoE���Ĺ���  */
    HI_DP_DROP_REASON_ICMP_E,                   /*38: ICMP���Ĺ���                      */
    HI_DP_DROP_REASON_POINT_TYPE0_E,            /*39: ָ����1��ETH_Type���Ĺ���         */
    HI_DP_DROP_REASON_POINT_TYPE1_E,            /*40: ָ����2��ETH_Type���Ĺ���         */
    HI_DP_DROP_REASON_POINT_TYPE2_E,            /*41: ָ����3��ETH_Type���Ĺ���         */
    HI_DP_DROP_REASON_POINT_TYPE3_E,            /*42: ָ����4��ETH_Type���Ĺ���         */
    HI_DP_DROP_REASON_BPDU_E,                   /*43: BPDU���Ķ���  */
    HI_DP_DROP_REASON_REMOTE_LP_TEST_PORT_E,    /*44: Զ�˻��ز��Զ˿����ô�����*/
    HI_DP_DROP_REASON_FAIL_LEARN_E,             /*45: �Զ�ѧϰʧ�ܶ�������              */
    HI_DP_DROP_REASON_EAPOL_E,                  /*46: EAPOL���Ķ���  */
    HI_DP_DROP_REASON_UNHIT_DMAC_E,             /*47: DMAC���ʧ�ܶ�������              */
    HI_DP_DROP_REASON_IGR_FILTER_E,             /*48: Դ�˿�����                        */
    HI_DP_DROP_REASON_NAPT_IP_OPTION_E,           /*49: IP Option����NAPTָʾ����*/
    HI_DP_DROP_REASON_TOCPU_REASON_CAR_E,       /*50: TOCPU REASON CAR���� */

    //    HI_DP_DROP_REASON_FAIL_MAP_GEMPORT_E,       /* GEM PORTӳ��ʧ�ܶ���              */
    HI_DP_DROP_REASON_FAIL_UP_SFC_E,            /*51: ���м�������ʧ�ܶ���            */
    HI_DP_DROP_REASON_FAIL_UP_MAP_E,            /*52: ����ӳ��ʧ�ܶ���                  */
    HI_DP_DROP_REASON_SCAR_CPU_E,               /*53: TO_CPU CAR����                    */
    HI_DP_DROP_REASON_SCAR_LOCAL_E,             /*54: ����ת��CAR����                   */
    HI_DP_DROP_REASON_SCAR_BC_E,                /*55: �㲥CAR����                       */
    HI_DP_DROP_REASON_SCAR_UUC_E,               /*56: δ֪����CAR����                   */
    HI_DP_DROP_REASON_FIFO_FULL_MC_COPY_E,      /*57: �鲥����FIFO������                */
    HI_DP_DROP_REASON_NAPT_SUBNET_UNHIT_E,      /*58: NAPTģ�������б��Ĳ���Subnet����ƥ��ʧ�ܣ����ָʾֱ�Ӷ���  */
    HI_DP_DROP_REASON_OAM_3AH_LP_E,             /*59: 3ah OAMԶ�˻��ض���                */
    HI_DP_DROP_REASON_MD_LEVEL_FILTER_E,        /*60: 1ag ����MD Level���˶���           */
    HI_DP_DROP_REASON_OAM_DISABLE_E,            /*61: OAM����û��ʹ�ܶ��� */
    HI_DP_DROP_REASON_FIFO_FULL_BC_COPY_E,      /*62: �㲥����FIFO������ */
    HI_DP_DROP_REASON_PORT_TAGGED_DROP_E,       /*63: �˿�taggedģʽ���� */
    HI_DP_DROP_REASON_BC_NO_EGR_E,              /*64: �㲥Ŀ�Ķ˿�ȫ�㶪�� */
    HI_DP_DROP_REASON_MC_NO_EGR_E,              /*65: �鲥Ŀ�Ķ˿�ȫ�㶪�� */
    HI_DP_DROP_REASON_PORT_DEFAULT_TAG_CHECK_E = 66,     /*66:�˿�Ĭ��TAG���  */
    HI_DP_DROP_REASON_PORT_ASSEMBLY_ILLEGAL_E,    /* 67������ģʽ�Ƿ����ģ���untagged�����TAGΪ��˽��TAG�� */
    HI_DP_DROP_REASON_UNKNOWN_EXTERN_HEADER_E,    /* 68��IPV6��֪����չͷ */
    HI_DP_DROP_REASON_IPV6_SIP_SITE_LOCAL_E,      /* 69��IPv6ԴIPΪSite Local  */
    HI_DP_DROP_REASON_IPV6_SIP_0_E,               /* 70��IPV6 SIP����0 */
    HI_DP_DROP_REASON_EFC_ECAR_E,                 /* 71��EFC����CAR���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_LCP_E,        /* 72��PPPOE  Session�׶ε�LCP���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_NCP_E,        /* 73��PPPOE  Session�׶ε�NCP���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_CHAP_E,       /* 74��PPPOE  Session�׶ε�CHAP���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_PAP_E,        /* 75��PPPOE  Session�׶ε�PAP���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_IPV6CP_E,     /* 76��PPPOE  Session�׶ε�IPV6CP���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_0_E,    /* 77��PPPOE  Session�׶ε��û��Զ�������0���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_1_E,    /* 78��PPPOE  Session�׶ε��û��Զ�������1���� */
    HI_DP_DROP_REASON_PPPOE_SESSION_POINT_2_E,    /* 79��PPPOE  Session�׶ε��û��Զ�������2���� */
    HI_DP_DROP_REASON_DHCPV6_E,                   /* 80��DHCPV6���� */
    HI_DP_DROP_REASON_ICMPV6_RS_E,                /* 81��ICMPV6 RS���� */
    HI_DP_DROP_REASON_ICMPV6_RA_E,                /* 82��ICMPV6 RA���� */
    HI_DP_DROP_REASON_ICMPV6_NS_E,                /* 83��ICMPV6 NS���� */
    HI_DP_DROP_REASON_ICMPV6_NA_E,                /* 84��ICMPV6 NA���� */
    HI_DP_DROP_REASON_ICMPV6_RM_E,                /* 85��ICMPV6 RM���� */
    HI_DP_DROP_REASON_ICMPV6_PING_REQ_E,          /* 86��ICMPV6 PingReq���� */
    HI_DP_DROP_REASON_ICMPV6_PING_REPLY_E,        /* 87��ICMPV6 PingREPLY���� */
    HI_DP_DROP_REASON_ICMPV6_UNREACH_E,           /* 88��ICMPv6Ŀ�Ĳ��ɴ����� */
    HI_DP_DROP_REASON_ICMPV6_GROUP_BIG_E,         /* 89��ICMPv6����̫���� */
    HI_DP_DROP_REASON_ICMPV6_OUTTIME_E,           /* 90��ICMPv6��ʱ����� */
    HI_DP_DROP_REASON_ICMPV6_PAMAM_ERROE_E,       /* 91��ICMPv6���������� */
    HI_DP_DROP_REASON_IPOE_MLD_QUERY_V1_V2_E,     /*92��IPOE���ص�MLD Query v1/v2���� */
    HI_DP_DROP_REASON_IPOE_MLD_REPORT_V1_E,       /* 93��IP0E���ص�MLD Report v1���� */
    HI_DP_DROP_REASON_IPOE_MLD_REPORT_V2_E,       /* 94��IPOE���ص�MLD Report v2���� */
    HI_DP_DROP_REASON_IPOE_MLD_DONE_E,            /* 95��IPOE���ص�MLD DONE���� */
    HI_DP_DROP_REASON_PPPOE_MLD_QUERY_V1_V2_E,    /* 96��PPPPOE���ص�MLD Query v1/v2���� */
    HI_DP_DROP_REASON_PPPOE_MLD_REPORT_V1_E,      /* 97��PPPOE���ص�MLD Report v1���� */
    HI_DP_DROP_REASON_PPPOE_MLD_REPORT_V2_E,      /* 98��PPPOE���ص�MLD Report v2���� */
    HI_DP_DROP_REASON_PPPOE_MLD_DONE_E,            /* 99��PPPOE���ص�MLD DONE���� */
    HI_DP_DROP_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,  /* 100������1����չͷ��IPV6���� */
    HI_DP_DROP_REASON_HEADER_LAREGR_128_BYTE_E,    /* 101������ͷ���ȳ���128B */
    HI_DP_DROP_REASON_EFC_POINT_NO_EGR_E,       /* 102��EFCָ��ת���޳��� */
    HI_DP_DROP_REASON_HL_TTL_LESS_1_E,             /* 103��HOP_LIMIT/TTLС�ڵ���1������ */
    HI_DP_DROP_REASON_RT_FAIL_E,                   /* 104��Ĭ��·�ɽ�ֹʱ·��ʧ�� */
    HI_DP_DROP_REASON_BLACK_HOLE_RT_E,             /* 105���ڶ�·�� */
    HI_DP_DROP_REASON_NO_NEXT_HOP_E,               /* 106������һ�� */
    HI_DP_DROP_REASON_IN_TUNNEL_MTU_FAIL_E,        /* 107�������MTU���ʧ�� */
    HI_DP_DROP_REASON_IGR_CAR_E,                   /* 108�����car���� */
    HI_DP_DROP_REASON_EGR_CAR_E,                   /* 109������car���� */
    HI_DP_DROP_REASON_TUNNEL_INTERFACE_INVLD_E,    /* 110������ӿ���Ч */
    HI_DP_DROP_REASON_SW_POINT_DROP_E,             /* 111�����ָʾ���ж�����NAPTģ�������б��Ĳ���Subnet����ƥ��ʧ�ܣ����ָʾֱ�Ӷ��� */
    HI_DP_DROP_REASON_SNAP_LENGTH_OVER_1500_E,     /* 112��SNAP���ȳ���1500 */
    HI_DP_DROP_REASON_GROUP_CAR_E,                 /* 113����car���� */
    HI_DP_DROP_REASON_EGR_ALL_0_E,                 /* 114���˿�ȫ0 */
    HI_DP_DROP_REASON_SIPV6_LOOP_E,                /* 115��SIPv6���ڻ��ص�ַ */
    HI_DP_DROP_REASON_SIPV6_MC_E,                  /* 116��SIPv6�����鲥��ַ */
    HI_DP_DROP_REASON_SIPV4_0_E,                   /* 117��SIPv4����0 */
    HI_DP_DROP_REASON_SIPV4_LOOP_E,                /* 118��SIPv4���ڻ��ص�ַ */
    HI_DP_DROP_REASON_SIPV4_MC_E,                  /* 119��SIPv4�����鲥��ַ */
    HI_DP_DROP_REASON_SIPV6_LINK_LOCAL_E,          /* 120��SIPv6����Link Local��ַ */
    HI_DP_DROP_REASON_IP_FILTER_E,                 /* 121���ڰ��������� */
    HI_DP_DROP_REASON_L3_IF_ISOLATION_E,           /* 122: ͬһ���˿���L3�ӿڸ��� */
    HI_DP_DROP_REASON_TUNNEL_UNUSED_LENGTH_E,      /* 123: ��������ĳ�������������*/
    HI_DP_DROP_REASON_FRAG_6RD_E,                  /* 124: 6RD��Ƭ���ĳ����*/
    HI_DP_DROP_REASON_IPV4_OPTION_6RD_E,           /* 125: 6RD��IPv4 Option���ĳ����*/

    HI_DP_DROP_NUM_E,
} HI_DP_DROP_REASON_E;

typedef enum
{
    HI_DP_THRU_REASON_NOT_THRU_E = 0,                 /* ��͸��                            */
    HI_DP_THRU_REASON_EFC_MATCHED0_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED1_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED2_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED3_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED4_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED5_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED6_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED7_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED8_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED9_E,                  /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED10_E,                 /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED11_E,                 /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED12_E,                 /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED13_E,                 /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED14_E,                 /* �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_EFC_MATCHED15_E,                 /*16: �Զ���EFCƥ��͸��                 */
    HI_DP_THRU_REASON_GE0_THRU_E,                      /*17: GE0�˿�Ϊ͸��ģʽ                 */
    HI_DP_THRU_REASON_GE1_THRU_E,                      /*18: GE1�˿�Ϊ͸��ģʽ                 */
    HI_DP_THRU_REASON_FE0_THRU_E,                      /*19: FE0�˿�Ϊ͸��ģʽ                 */
    HI_DP_THRU_REASON_FE1_THRU_E,                      /*20: FE1�˿�Ϊ͸��ģʽ                 */
    HI_DP_THRU_REASON_FE2_THRU_E,                      /*21: FE2�˿�Ϊ͸��ģʽ                 */
    HI_DP_THRU_REASON_BPDU_E,                         /*22: BPDU����                         */
    HI_DP_THRU_REASON_EAPOL_E,                        /*23: EAPOL����                      */
    HI_DP_THRU_REASON_OAM_3AH,                        /*24: OAM 3AH����                      */
    HI_DP_THRU_REASON_OAM_1AG,                        /*25: OAM 1AG����                      */
    HI_DP_THRU_REASON_IGMP_IPOE_E,                /*26: IPoE���� IGMP����                          */
    HI_DP_THRU_REASON_ARP_E,                           /*27: ARP����                           */
    HI_DP_THRU_REASON_RARP_E,                          /*28: RARP����                          */
    HI_DP_THRU_REASON_PPPOE_DIS_E,                     /*29: PPPOE DISCOVERY����               */
    HI_DP_THRU_REASON_IGMP_PPPOE_E,                    /*30: PPPoE���� IGMP����                          */
    HI_DP_THRU_REASON_DHCP_E = 32,                     /*32: DHCP����                          */

    //    HI_DP_THRU_REASON_IPV6_E,                          /* IPV6����                          */
    HI_DP_THRU_REASON_ICMP_E = 34,                     /*34: ICMP����                          */
    HI_DP_THRU_REASON_POINT_TYPE0_E,                   /*35: �Զ�������͸��                    */
    HI_DP_THRU_REASON_POINT_TYPE1_E,                   /*36: �Զ�������͸��                    */
    HI_DP_THRU_REASON_POINT_TYPE2_E,                   /*37: �Զ�������͸��                    */
    HI_DP_THRU_REASON_POINT_TYPE3_E,                   /*38: �Զ�������͸��                    */

    //    HI_DP_THRU_REASON_PPPOE_SES_E,                     /* PPPOE SESSION����                 */
    HI_DP_THRU_REASON_BASE_ON_SMAC_E = 40,                  /* 40: ����SMAC͸��                      */

    HI_DP_THRU_REASON_PPPOE_SESSION_LCP_E = 72,       /* 72��PPPOE  Session�׶ε�LCP���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_NCP_E,            /* 73��PPPOE  Session�׶ε�NCP���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_CHAP_E,           /* 74��PPPOE  Session�׶ε�CHAP���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_PAP_E,            /* 75��PPPOE  Session�׶ε�PAP���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_IPV6CP_E,         /* 76��PPPOE  Session�׶ε�IPV6CP���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_0_E,        /* 77��PPPOE  Session�׶ε��û��Զ�������0���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_1_E,        /* 78��PPPOE  Session�׶ε��û��Զ�������1���� */
    HI_DP_THRU_REASON_PPPOE_SESSION_POINT_2_E,        /* 79��PPPOE  Session�׶ε��û��Զ�������2���� */
    HI_DP_THRU_REASON_DHCPV6_E,                       /* 80��DHCPV6���� */
    HI_DP_THRU_REASON_ICMPV6_RS_E,                    /* 81��ICMPV6 RS���� */
    HI_DP_THRU_REASON_ICMPV6_RA_E,                    /* 82��ICMPV6 RA���� */
    HI_DP_THRU_REASON_ICMPV6_NS_E,                    /* 83��ICMPV6 NS���� */
    HI_DP_THRU_REASON_ICMPV6_NA_E,                    /* 84��ICMPV6 NA���� */
    HI_DP_THRU_REASON_ICMPV6_RM_E,                    /* 85��ICMPV6 RM���� */
    HI_DP_THRU_REASON_ICMPV6_PING_REQ_E,              /* 86��ICMPV6 PingReq���� */
    HI_DP_THRU_REASON_ICMPV6_PING_REPLY_E,            /* 87��ICMPV6 PingREPLY���� */
    HI_DP_THRU_REASON_ICMPV6_UNREACH_E,               /* 88��ICMPv6Ŀ�Ĳ��ɴ����� */
    HI_DP_THRU_REASON_ICMPV6_GROUP_BIG_E,             /* 89��ICMPv6����̫���� */
    HI_DP_THRU_REASON_ICMPV6_OUTTIME_E,               /* 90��ICMPv6��ʱ����� */
    HI_DP_THRU_REASON_ICMPV6_PAMAM_ERROE_E,           /* 91��ICMPv6���������� */
    HI_DP_THRU_REASON_IPOE_MLD_QUERY_V1_V2_E,         /*92��IPOE���ص�MLD Query v1/v2���� */
    HI_DP_THRU_REASON_IPOE_MLD_REPORT_V1_E,           /* 93��IP0E���ص�MLD Report v1���� */
    HI_DP_THRU_REASON_IPOE_MLD_REPORT_V2_E,           /* 94��IPOE���ص�MLD Report v2���� */
    HI_DP_THRU_REASON_IPOE_MLD_DONE_E,                /* 95��IPOE���ص�MLD DONE���� */
    HI_DP_THRU_REASON_PPPOE_MLD_QUERY_V1_V2_E,        /* 96��PPPPOE���ص�MLD Query v1/v2���� */
    HI_DP_THRU_REASON_PPPOE_MLD_REPORT_V1_E,          /* 97��PPPOE���ص�MLD Report v1���� */
    HI_DP_THRU_REASON_PPPOE_MLD_REPORT_V2_E,          /* 98��PPPOE���ص�MLD Report v2���� */
    HI_DP_THRU_REASON_PPPOE_MLD_DONE_E,                /* 99��PPPOE���ص�MLD DONE���� */
    HI_DP_THRU_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,      /* 100������1����չͷ��IPV6���� */
    HI_DP_THRU_REASON_HEADER_LAREGR_128_BYTE_E,        /* 101������ͷ���ȳ���128B */

    HI_DP_THRU_NUM_E,
} HI_DP_THRU_REASON_E;

typedef enum
{
    HI_DP_TOCPU_REASON_L2_WLAN_E = 0,         /* 0�� L2ת���������ؽӿڣ�WLAN�� */
    HI_DP_TOCPU_REASON_EFC_MATCH0_E,           /*1: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH1_E,           /*2: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH2_E,           /*3: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH3_E,           /*4: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH4_E,           /*5: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH5_E,           /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH6_E,           /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH7_E,           /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH8_E,           /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH9_E,           /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH10_E,          /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH11_E,          /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH12_E,          /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH13_E,          /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH14_E,          /* �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_EFC_MATCH15_E,          /*16: �Զ���EFCƥ�佻CPUԭ��            */
    HI_DP_TOCPU_REASON_RTP_CAPTURE_E,          /*17: RTP������            */

    //    HI_DP_TOCPU_REASON_UNMATCH_NAPT_E ,         /* 17: ΪNAPT����EFCƥ��ʧ��             */
    //    HI_DP_TOCPU_REASON_UNTAG_E ,                /* UNTAG����                         */
    //    HI_DP_TOCPU_REASON_VLAN_TAG_E ,             /* VLAN TAG����                      */
    //    HI_DP_TOCPU_REASON_PRI_TAG_E ,              /* PRI UNTAG����                     */
    //    HI_DP_TOCPU_REASON_BC_E ,                   /* �㲥����                          */
    HI_DP_TOCPU_REASON_IGMP_PPPOE_E = 22,      /*22: pppoe����IGMP����                          */
    HI_DP_TOCPU_REASON_IGMP_IPOE_E,            /*23: ipoe����IGMP����                          */
    HI_DP_TOCPU_REASON_ARP_E,                  /*24: ARP����                           */
    HI_DP_TOCPU_REASON_RARP_E,                 /*25: RARP����                          */
    HI_DP_TOCPU_REASON_PPPOE_DIS_E,            /*26: PPPOE DISCOVERY����               */
    HI_DP_TOCPU_REASON_EAPOL_E,                /*27: EAPOL����                         */
    HI_DP_TOCPU_REASON_BPDU_E,                 /*28: BPDU����                          */
    HI_DP_TOCPU_REASON_DHCP_E,                 /*29: DHCP����                          */
    
    HI_DP_TOCPU_REASON_DSLITE_LAGGER_THAN_MTU_E ,  /*30: ������DS Lite ���MTU ������CPU(CFM)    */
    HI_DP_TOCPU_REASON_ICMP_E = 31,            /*31: ICMP����                          */
    HI_DP_TOCPU_REASON_POINT_TYPE0_E,          /*32: �Զ��岶��ı���0                  */
    HI_DP_TOCPU_REASON_POINT_TYPE1_E,          /*33: �Զ��岶��ı���1                  */
    HI_DP_TOCPU_REASON_POINT_TYPE2_E,          /*34: �Զ��岶��ı���2                  */
    HI_DP_TOCPU_REASON_POINT_TYPE3_E,          /*35: �Զ��岶��ı���3                  */

    //    HI_DP_TOCPU_REASON_PPPOE_SES_E ,            /* 36: PPPOE SESSION����                 */
    HI_DP_TOCPU_REASON_PORT_CHANGE_E = 37,      /* 37: ��̬����Ķ˿ڷ����ƶ�            */
    HI_DP_TOCPU_REASON_MAC_LEARN_E,            /* 38: �����ύ��CPUѧϰ                 */
    HI_DP_TOCPU_REASON_MAC_LEARN_FAIL_E,       /* 39: �߼�ѧϰ����ʧ��                  */
    HI_DP_TOCPU_REASON_SEARCH_FAIL_E,          /* 40: �߼����ʧ��                      */
    /*BEGIN: ���ⵥ��DTS2012032703169��by lKF55238,2012/4/18*/
    HI_DP_TOCPU_REASON_TCP_CTRL_E,           /*41:TCP    ���Ʊ��Ľ�CPU*/
    /*END: ���ⵥ��DTS2012032703169��by lKF55238,2012/4/18*/
    HI_DP_TOCPU_REASON_NAPT_ON_E = 42,         /* 42: NAPT���ٿ���ʱ�װ��ύ            */
    HI_DP_TOCPU_REASON_NAPT_IP_OPTION_E,       /* 43: IP Option������NAPT            */


    HI_DP_TOCPU_REASON_6RD_LAGGER_THAN_MTU_E , /*44: ������6rd ���MTU ������CPU(CFM)    */

    //HI_DP_TOCPU_REASON_UNMATCH_NAPT_OUTNET_E ,  /* 44: NAPT��OUTNET��ƥ��              */
    HI_DP_TOCPU_REASON_1AG_Y1731_E = 45,       /* 45: 802.1ag / Y.1731����              */
    HI_DP_TOCPU_REASON_3AH_E,                  /* 46: 802.3ah����                       */
#if 0
    /* V200, 47 - 53Ϊ���� */
    HI_DP_TOCPU_REASON_PDELAY_REQ_E,           /* 47: Pdelay_req���Ĳ���CPU            */
    HI_DP_TOCPU_REASON_PDELAY_RESP_E,          /* 48: Pdelay_resp���Ĳ���CPU           */
    HI_DP_TOCPU_REASON_PDELAY_RESP_FOLLOW_UP_E,/* 49: Pdelay_resp_follow_up���Ĳ���CPU */
    HI_DP_TOCPU_REASON_MANAGEMENT_E,           /* 50: Management���Ľ�CPU                */
    HI_DP_TOCPU_REASON_SIGNALING_E,            /* 51: Signaling���Ľ�CPU                 */
    HI_DP_TOCPU_REASON_ANNOUNCE_E,             /* 52: Announce���Ľ�CPU                  */
    HI_DP_TOCPU_REASON_OTHER_1588_E,           /* 53: ����1588Э�鱨�Ľ�CPU              */

    HI_DP_TOCPU_REASON_NAPT_DN_FAIL_E,         /* 54: NAPT���в��ʧ�ܽ�CPU              */
#endif
    HI_DP_TOCPU_REASON_CCM_LMM_0_E,             /*47: LM���ԣ�����CCM/LMM���ģ�����0 */
    HI_DP_TOCPU_REASON_CCM_LMM_1_E,             /*48: LM���ԣ�����CCM/LMM���ģ�����1 */
    HI_DP_TOCPU_REASON_CCM_LMM_2_E,             /*49: LM���ԣ�����CCM/LMM���ģ�����2 */
    HI_DP_TOCPU_REASON_CCM_LMM_3_E,             /*50: LM���ԣ�����CCM/LMM���ģ�����3 */
    HI_DP_TOCPU_REASON_CCM_LMM_4_E,             /*51: LM���ԣ�����CCM/LMM���ģ�����4 */
    HI_DP_TOCPU_REASON_CCM_LMM_5_E,             /*52: LM���ԣ�����CCM/LMM���ģ�����5 */
    HI_DP_TOCPU_REASON_CCM_LMM_6_E,             /*53: LM���ԣ�����CCM/LMM���ģ�����6 */
    HI_DP_TOCPU_REASON_CCM_LMM_7_E,             /*54: LM���ԣ�����CCM/LMM���ģ�����7 */
    HI_DP_TOCPU_REASON_IP_FRAGMENT_NAPT_E,      /* 55��	IP��Ƭ������NAPT��CPU */
    HI_DP_TOCPU_REASON_NAPT_DN_FAILE_E,         /* 56��	NAPT���в��ʧ�ܽ�CPU */
    HI_DP_TOCPU_REASON_MC_GEMPORT_IPV4_UMC_E,    /* 57: �鲥GEMPORT����IPv4δ֪�鲥���Ľ�CPU */
    HI_DP_TOCPU_REASON_TUNNEL_UNKNOWN_MC_E,        /* 58: ���δ֪�鲥���Ľ�CPU */
    HI_DP_TOCPU_REASON_MC_GEMPORT_IPV6_UMC_E,     /* 59: �鲥GEMPORT����IPv6δ֪�鲥���Ľ�CPU */
    HI_DP_TOCPU_REASON_UC_BC_GEMPORT_IPV4_UMC_E, /* 60: ���鲥GEMPORT����IPv4δ֪�鲥���Ľ�CPU */
    HI_DP_TOCPU_REASON_UC_BC_GEMPORT_IPV6_UMC_E, /* 61: ���鲥GEMPORT����IPv6δ֪�鲥���Ľ�CPU */

    HI_DP_TOCPU_REASON_BC_IGR_MIRROR_E,    /* 62: �㲥Դ�˿ھ���ת�� */
    HI_DP_TOCPU_REASON_MC_IGR_MIRROR_E,     /* 63: �鲥Դ�˿ھ���ת�� */
    HI_DP_TOCPU_REASON_NAPT_EGR_IS_CPU_E,  /* 64��	NAPT���ٵ��û��ڣ�����WLAN�� */
    HI_DP_TOCPU_REASON_RT_TO_GW_IF_E,              /* 65��	·�ɵ����ؽӿ� */
    HI_DP_TOCPU_REASON_L2_FWD_TO_GW_IF_E,          /* 66��	L2ת�������ؽӿ� */
    HI_DP_TOCPU_REASON_EFC_POINT_TO_WLAN_E,        /* 67��EFCָ��ת����WLAN */
    HI_DP_TOCPU_REASON_UNKNOWN_EXTERN_HEADER_E,    /* 68��IPV6��֪����չͷ */
    HI_DP_TOCPU_REASON_IPV6_SIP_SITE_LOCAL_E,      /* 69��IPv6ԴIPΪSite Local  */
    HI_DP_TOCPU_REASON_IPV6_SIP_0_E,               /* 70��IPV6 SIP����0 */
    HI_DP_TOCPU_REASON_IPV6_SIP_LINK_LOCAL_E,      /* 71��IPv6ԴIPΪLink Local  */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_LCP_E,        /* 72��PPPOE  Session�׶ε�LCP���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_NCP_E,        /* 73��PPPOE  Session�׶ε�NCP���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_CHAP_E,       /* 74��PPPOE  Session�׶ε�CHAP���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_PAP_E,        /* 75��PPPOE  Session�׶ε�PAP���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_IPV6CP_E,     /* 76��PPPOE  Session�׶ε�IPV6CP���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_0_E,    /* 77��PPPOE  Session�׶ε��û��Զ�������0���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_1_E,    /* 78��PPPOE  Session�׶ε��û��Զ�������1���� */
    HI_DP_TOCPU_REASON_PPPOE_SESSION_POINT_2_E,    /* 79��PPPOE  Session�׶ε��û��Զ�������2���� */
    HI_DP_TOCPU_REASON_DHCPV6_E,                   /* 80��DHCPV6���� */
    HI_DP_TOCPU_REASON_ICMPV6_RS_E,                /* 81��ICMPV6 RS���� */
    HI_DP_TOCPU_REASON_ICMPV6_RA_E,                /* 82��ICMPV6 RA���� */
    HI_DP_TOCPU_REASON_ICMPV6_NS_E,                /* 83��ICMPV6 NS���� */
    HI_DP_TOCPU_REASON_ICMPV6_NA_E,                /* 84��ICMPV6 NA���� */
    HI_DP_TOCPU_REASON_ICMPV6_RM_E,                /* 85��ICMPV6 RM���� */
    HI_DP_TOCPU_REASON_ICMPV6_PING_REQ_E,          /* 86��ICMPV6 PingReq���� */
    HI_DP_TOCPU_REASON_ICMPV6_PING_REPLY_E,        /* 87��ICMPV6 PingREPLY���� */
    HI_DP_TOCPU_REASON_ICMPV6_UNREACH_E,           /* 88��ICMPv6Ŀ�Ĳ��ɴ����� */
    HI_DP_TOCPU_REASON_ICMPV6_GROUP_BIG_E,         /* 89��ICMPv6����̫���� */
    HI_DP_TOCPU_REASON_ICMPV6_OUTTIME_E,           /* 90��ICMPv6��ʱ����� */
    HI_DP_TOCPU_REASON_ICMPV6_PAMAM_ERROE_E,       /* 91��ICMPv6���������� */
    HI_DP_TOCPU_REASON_IPOE_MLD_QUERY_V1_V2_E,     /*92��IPOE���ص�MLD Query v1/v2���� */
    HI_DP_TOCPU_REASON_IPOE_MLD_REPORT_V1_E,       /* 93��IP0E���ص�MLD Report v1���� */
    HI_DP_TOCPU_REASON_IPOE_MLD_REPORT_V2_E,       /* 94��IPOE���ص�MLD Report v2���� */
    HI_DP_TOCPU_REASON_IPOE_MLD_DONE_E,            /* 95��IPOE���ص�MLD DONE���� */
    HI_DP_TOCPU_REASON_PPPOE_MLD_QUERY_V1_V2_E,    /* 96��PPPPOE���ص�MLD Query v1/v2���� */
    HI_DP_TOCPU_REASON_PPPOE_MLD_REPORT_V1_E,      /* 97��PPPOE���ص�MLD Report v1���� */
    HI_DP_TOCPU_REASON_PPPOE_MLD_REPORT_V2_E,      /* 98��PPPOE���ص�MLD Report v2���� */
    HI_DP_TOCPU_REASON_PPPOE_MLD_DONE_E,            /* 99��PPPOE���ص�MLD DONE���� */
    HI_DP_TOCPU_REASON_IPV6_EXTERN_HEADER_NUM_LARGER_1_E,  /* 100������1����չͷ��IPV6���� */
    HI_DP_TOCPU_REASON_HEADER_LAREGR_128_BYTE_E,    /* 101������ͷ���ȳ���128B */
    HI_DP_TOCPU_REASON_RT_FWD_USR_E,                /* 102��·��ת����cpu�û��� */
    HI_DP_TOCPU_REASON_HL_TTL_LESS_1_E,             /* 103��HOP_LIMIT/TTLС�ڵ���1������ */
    HI_DP_TOCPU_REASON_RT_FAIL_E,                   /* 104��Ĭ��·�ɽ�ֹʱ·��ʧ�� */
    HI_DP_TOCPU_REASON_REFUSE_HOLE_RT_E,            /* 105���ܾ�·�� */
    HI_DP_TOCPU_REASON_NO_NEXT_HOP_E,               /* 106������һ�� */
    HI_DP_TOCPU_REASON_IN_TUNNEL_MTU_FAIL_E,        /* 107�������MTU���ʧ�� */
    HI_DP_TOCPU_REASON_BC_BC_E,                     /* 108���㲥���Ĺ㲥��CPU  */
    HI_DP_TOCPU_REASON_UC_E,                        /* 109��δ֪�������㲥��CPU��ת��CPU�� */
    HI_DP_TOCPU_REASON_MC_FWD_TOCPU_E,              /* 110����֪�鲥����ת����CPU�û��� */
    HI_DP_TOCPU_REASON_SIPV6_LOOP_E,                /* 111��SIPv6���ڻ��ص�ַ */
    HI_DP_TOCPU_REASON_SIPV6_MC_E,                  /* 112��SIPv6�����鲥��ַ */
    HI_DP_TOCPU_REASON_SIPV4_0_E,                   /* 113��SIPv4����0 */
    HI_DP_TOCPU_REASON_SIPV4_LOOP_E,                /* 114��SIPv4���ڻ��ص�ַ */
    HI_DP_TOCPU_REASON_SIPV4_MC_E,                  /* 115��SIPv4�����鲥��ַ */
    HI_DP_TOCPU_REASON_TOCPU_AND_ROUTE_E,           /* 116��·��+����CPU */
    HI_DP_TOCPU_REASON_TOCPU_AND_NAPT_E,            /* 117��NAPT+����CPU */
    HI_DP_TOCPU_REASON_SNAP_LENGTH_OVER_1500_E,     /* 118��SNAP���ȳ���1500 */
    HI_DP_TOCPU_REASON_DS_LITE_OUT_E,               /* 119: �����ͷ�ı���DS_LITE���ĳ���� */
    HI_DP_TOCPU_REASON_BC_VLAN_E,                   /* 120���㲥����vlan�û��㲥��CPU  */
    HI_DP_TOCPU_REASON_MC_VLAN_E,                  /* 121����֪�鲥����VLANת����CPU�û��� */

    /* BEGIN: Added by shengjing, 2012/1/10   PN:DTS2012011002267 */

    HI_DP_TOCPU_REASON_FRAG_6RD_E,                /* 122: 6RD��Ƭ���Ľ�cpu |||5115T IPv6 ����Ƭ��չͷ��CPU(RT)*/
    HI_DP_TOCPU_REASON_DROP_REDIRECTION_E,                /* 123: ���������ض���cpu */
    /* END:   Added by shengjing, 2012/1/10 */
    HI_DP_TOCPU_REASON_6RD_FRAG_E = 124,                  /* 124��6rd��Ƭ���ĳ���� */
    HI_DP_TOCPU_REASON_6RD_IPV4_OPTION_E,                  /* 125��6rd��IPv4 option���ĳ���� */
    HI_DP_TOCPU_REASON_EGR_MIRROR_E,                  /* 126:���˿ھ��� */
    HI_DP_TOCPU_REASON_IGR_MIRROR_E,                  /* 127:��˿ھ��� */

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
    HI_DP_UP_UNKNOWN_MODE_E,    /* Ĭ�� */
} HI_DP_UP_MODE_E;

typedef enum
{
    HI_DP_UP_TYPE_E = 0,  /* ����QOS,Ĭ�� */
    HI_DP_DN_TYPE_E,      /* ����QOS */
} HI_DP_QOS_TYPE_E;

/* ����:��������ģʽ */
typedef enum
{
    HI_DP_BIG_ENDIAN_E = 0,
    HI_DP_LITTLE_ENDIAN_E,
} HI_DP_ENDIAN_E;

/* �˿� */

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

/* ��̫���˿�ö�ٶ��� */
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

/* ��̫�˿�MIIģʽ */
typedef enum
{
    HI_DP_ETH_GMII_E = 0,       /* GMII */
    HI_DP_ETH_SGMII_E,       /* SGMII */
    HI_DP_ETH_RGMII_E,          /* RMII / RGMII */
    HI_DP_ETH_DUAL_RGMII_E,     /* ˫RGMIIģʽ */
    HI_DP_ETH_SERDES_E,
    HI_DP_ETH_MII_E,
    HI_DP_ETH_RVMII_E,

    HI_DP_ETH_MII_MODE_NUM_E
} HI_DP_ETH_MII_MODE_E;

/* ˫RGMIIģʽ������RGMII�ڵ�ѡ�� */
typedef enum
{
    HI_DP_MASTER_RGMII1_E,
    HI_DP_MASTER_RGMII2_E,

    HI_DP_MASTER_RGMII_NUM_E
} HI_DP_MASTER_RGMII_E;

//#if (defined (CONFIG_HISI_SD5115_AF_ASIC) && defined (CONFIG_HISI_SD5115_TYPE_H))
/* RGMII1������ѡ�� */
typedef enum
{
    HI_DP_RGMII_LINE_SEQ0_E,
    HI_DP_RGMII_LINE_SEQ1_E,

    HI_DP_RGMII_LINE_SEQ_NUM_E
} HI_DP_RGMII_LINE_SEQ_E;
//#endif

/* ��̫�˿ڹ���ģʽ */
typedef enum
{
    HI_DP_ETH_AUTO_NEG_E,       /* �Զ�Э�� */

    HI_DP_ETH_HALF_10M_E,       /* ��˫��10M */
    HI_DP_ETH_HALF_100M_E,      /* ��˫��100M */
    HI_DP_ETH_HALF_1000M_E,     /* ��˫��1000M */

    HI_DP_ETH_DUP_10M_E,        /* ȫ˫��10M */
    HI_DP_ETH_DUP_100M_E,       /* ȫ˫��100M */
    HI_DP_ETH_DUP_1000M_E,      /* ȫ˫��1000M */
    HI_DP_ETH_TEST_E,      /* ����ģʽ*/

    HI_DP_ETH_WORK_MODE_NUM_E
} HI_DP_ETH_WORK_MODE_E;

/* ��̫�˿ڻ���ģʽ */
typedef enum
{
    HI_DP_ETH_NO_LOOP_E = 0,   /* ����ģʽ */

    HI_DP_ETH_IN_LOOP_E,        /* MAC�ڻ���ģʽ */
    HI_DP_ETH_OUT_LOOP_E,       /* MAC�⻷��ģʽ */

    HI_DP_ETH_PHY_IN_LOOP_E,    /* PHY�ڻ���ģʽ */
    HI_DP_ETH_PHY_OUT_LOOP_E,   /* PHY�⻷��ģʽ */

    HI_DP_ETH_LOOP_MODE_NUM_E
} HI_DP_ETH_LOOP_MODE_E;

/* ��̫�˿�������ģʽ */
typedef enum
{
    HI_DP_ETH_NO_STP_E = 0,    /* ��֧�� */
    HI_DP_ETH_STP_E,            /* ֧��STP */
    HI_DP_ETH_RSTP_E,           /* ֧��RSTP */
} HI_DP_ETH_STP_MODE_E;

/* ��̫�˿�������״̬ */
typedef enum
{
    HI_DP_ETH_STP_FORWARD_E = 0,    /* ת�� */
    HI_DP_ETH_STP_LEARN_E,          /* ѧϰ */
    HI_DP_ETH_STP_LISTEN_E,         /* ���� */
    HI_DP_ETH_STP_BLOCK_E,          /* ���� */
    HI_DP_ETH_STP_CLOSE_E,          /* �ر�,���� */
} HI_DP_ETH_STP_STATUS_E;

/* ��̫�˿ڵ�����״̬ */
typedef enum
{
    HI_DP_ETH_LINK_UP,
    HI_DP_ETH_LINK_DN
} HI_DP_ETH_LINK_STA_E;

/* ��̫�˿�PHY֧�ֵĹ���ģʽ��������ֻ����д��Ч */
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

/* ��̫�˿�PHY��������, BASE_T/BASE_TX/BASE_T4/BASE_T2ָ֧�������������� */
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

/* ��̫�˿�PHY֧�ֵ��Զ�Э�������� */
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

/* ��̫�˿�PHY֧�ֵ�EEE������ */
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

/* RXDMA�Ը���TAG���͵ı��ĵĴ���ʽ */
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
    HI_DP_EGR_TAG_THRU_E,       /* ͸�� */
    HI_DP_EGR_TAG_STRIP_E,      /* ���� */
    HI_DP_EGR_TAG_CHECK_E,      /* �˿�VLAN��飬���ڶ˿�Ĭ��VLANʱ������tag������͸�� */
    HI_DP_EGR_TAG_SWITCH_E,     /* �л�VLAN����tag���ڶ˿�Ĭ��VLANʱ������tag��
                                   ���򣬽���VID��Ĳ�ѯ������ѯ���ж�Ӧ��VID��ʱ��
                                   �л���ָ����VID�����û�У���͸�� */
    HI_DP_EGR_TAG_HYBRID_E,     /* Hybridģʽ���鿴tag�����ԣ���Ϊuntag�������tag����Ϊtag����͸�� */

    HI_DP_EGR_TAG_DROP_E,         /* ���ڳ��ڷ����ϵ�DROP��MCDģ��ʵ�� */
    HI_DP_EGR_TAG_DROP_DEFAULT_E,
} HI_DP_EGR_TAG_PRCS_E;
#endif

/* TPIDѡ�� */
typedef enum
{
    HI_DP_TPID_NONE_E,
    HI_DP_TPID_IDX1_E,
    HI_DP_TPID_IDX2_E,
    HI_DP_TPID_IDX3_E,
} HI_DP_TPID_SEL_E;

/* ���ȼ� */
typedef enum
{
    HI_DP_S8021P_PRI_E = 0,         /* �� */
    HI_DP_C8021P_PRI_E,             /* �� */
    HI_DP_IPTOS_PRI_E,
    HI_DP_EFC_MARK_PRI_E,
    HI_DP_GEM_PORT_PRI_E,
    HI_DP_REP_S8021P_PRI_E,         /* �滻����� */
    HI_DP_REP_C8021P_PRI_E,
    HI_DP_NO_CARE_PRI_E,            /* ����ע */
} HI_DP_PRI_MODE_E;

/* VLAN */
typedef enum
{
    HI_DP_SVLAN_E = 0,              /* �� */
    HI_DP_CVLAN_E,                  /* �� */
    HI_DP_EFC_MARK_VLAN_E,
    HI_DP_REP_SVLAN_E,              /* �滻����� */
    HI_DP_REP_CVLAN_E,
    HI_DP_NO_CARE_VLAN_E,           /* ����ע */
} HI_DP_VLAN_MODE_E;

typedef enum
{
    HI_DP_VLAN_NO_ACTION = 0,

    /* VLAN ���ڻ�����, ��ȫ�����þ��� */
    HI_DP_VLAN_TO_POINT,        /* VLAN�滻Ϊָ��VLAN */
    HI_DP_VLAN_TO_ORIGINAL_S,   /* VLAN�滻Ϊԭʼ���VLAN */
    HI_DP_VLAN_TO_ORIGINAL_C,   /* VLAN�滻Ϊԭʼ�ڲ�VLAN */
} HI_DP_VALN_ACT_E;

/*
�޶�����
1���޸�ֵΪָ��802��1p��
2���޸�ֵΪ����ԭ���802��1p��
3���޸�ֵΪ����ԭ�ڲ�802��1p�����ԭ����ֻ��һ��tag�����޸�ֵΪ���ĵ�ԭ���802��1p
4���޸�ֵΪIPTOS��������ķ�IP���ģ����޸�ֵΪ����ԭ���802��1p
5���޸�ֵΪ����GEMӳ�䣬������ķ����У����޶�����

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

/* BEGIN: Deleted by shengjing, 2010/10/16   ���ⵥ��:SD5113V200����*/
/* �޸���Ϊ */
typedef enum
{
    HI_DP_EDIT_NO_ACTION = 0,
    HI_DP_EDIT_ADD,
    HI_DP_EDIT_DEL,
    HI_DP_EDIT_REP,
} HI_DP_EDIT_ACT_E;
/* END: Deleted by shengjing, 2010/10/16   ���ⵥ��:SD5113V200����*/

/* ͳһһ��ת����Ϊö��,����FLOW, SPECIAL PKT, L2, L3����ת�����õ����ģ�� */
typedef enum
{
    HI_DP_TRANS_FWD_E = 0,
    HI_DP_TRANS_DROP_E,
    HI_DP_TRANS_TOCPU_E,
    HI_DP_TRANS_TOCPU_AND_FWD_E,
    HI_DP_TRANS_TOPOINT_E,
    HI_DP_TRANS_THRU_E,

    HI_DP_TRANS_ROUTE_NAT_E, /*��������*/
    HI_DP_TRANS_NAPT_E = 7,

    HI_DP_TRANS_TOCPU_AND_THRU_E,
    HI_DP_TRANS_BC_E,
    HI_DP_TRANS_ROUTE_E,             /* L3·��ת�� */
    HI_DP_TRANS_TOCPU_AND_ROUTE_E,
    HI_DP_TRANS_POLICY_ROUTE_E,      /* ����·�ɣ���ȷ��ƴд */

    /* BEGIN: Added by shengjing, 2011/9/10   PN:5115����*/
    HI_DP_TRANS_BC_BY_VLAN_E,     /* ����vlan�ڹ㲥��������vlan�л� */
    HI_DP_TRANS_MC_LIST_E,             /* �鲥����ת����ͬ5113 */
    HI_DP_TRANS_MC_PORTMAP_E,          /* �鲥�˿�ת����5115���� */
    /* END:   Added by shengjing, 2011/9/10 */

    HI_DP_TRANS_RESERVED_E,            /* ������Ϊ */
} HI_DP_TRANS_ACT_E;

/* BEGIN: Added by shengjing, 2011/12/30   PN:�ӿ�����*/
/* ƥ��ģʽ */
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

/* BEGIN: Added by shengjing, 2010/10/21   ���ⵥ��:SD5113V200����*/

/*�˿�����*/
typedef enum
{
    HI_DP_UNI = 0,
    HI_DP_NNI,

    /*��������չ*/
} HI_DP_PORT_TYPE_E;

/*�ֶ�*/
typedef enum
{
    HI_DP_DMAC_E = 0,
    HI_DP_SMAC_E,

    HI_DP_DIP_E = 5,
    HI_DP_SIP_E = 6,

    /*��������չ*/
} HI_DP_FIELD_E;

/* IP�汾 */
typedef enum
{
    HI_DP_IP_V4_E = 0,
    HI_DP_IP_V6_E,

    HI_DP_IP_VERSION,
} HI_DP_IP_VERSION_E;

/* �鲥ģʽѡ���� */
typedef enum
{
    HI_DP_MAC_MC_E = 0,    /* �ಥMAC  L2�鲥 */
    HI_DP_IP_MC_E,        /* �ಥIP L3�鲥 */
} HI_DP_MC_MODE_E;

/* END:   Added by shengjing, 2010/10/21 */

/* BEGIN: Added by shengjing, 2011/9/6   PN:5115����*/
/* tpid�޸�ģʽѡ�� */
typedef enum
{
    HI_DP_TPID_NO_ACTION_E = 0,
    HI_DP_TPID_0_E,
    HI_DP_TPID_1_E,
    HI_DP_TPID_2_E,
} HI_DP_TPID_REP_MODE_E;

/* ������ɫ */
typedef enum
{
    HI_DP_COLOR_NO_E = 0,
    HI_DP_COLOR_GREEN_E,
    HI_DP_COLOR_YELLOW_E,
    HI_DP_COLOR_RED_E,
} HI_DP_COLOR_E;

/* ���� */
typedef enum
{
    HI_DP_POSITIVE_E,   /* �� */
    HI_DP_NEGATIVE_E    /* �� */
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
    /* ����:��Ҫ���Ƿ�������TIMES�����,�Ƿ�Ϊ 0���ɱ�� */
    /* �Է���PLOAM֡�йصĲ��� */
    uint ui_send_times;

    /* ADD ���Ӳ���ͬ������ */
    HI_BOOL_E em_urgen_en;
    HI_BOOL_E em_force_en;
    HI_BOOL_E em_sync_en;

    /* PLOAM֡���� */
    uchar uc_data[HI_DP_PLOAM_MSG_LEN];
} hi_ploam_message_s;

typedef struct
{
    /* ADD ���Ӳ���ͬ������ */
    HI_BOOL_E em_sync_en;
    uint      ui_length;           /* ��Ϣ���� */

    uchar *puc_msg;
} hi_omci_message_s;

/* MPCP�������� */
typedef enum
{
    HI_DP_EPON_OLT_SCB_MSG_E = 0,   /* ��ʾOLT�㲥��Ϣ */
    HI_DP_EPON_ONU_SCB_MSG_E,       /* ��ʾONU�㲥��Ϣ */     
    HI_DP_EPON_SNOOP_MSG_E,         /* ����ģʽ������������Ϣ */          
    HI_DP_EPON_LLID_MSG_E,          /* ����LLID����Ϣ */              
 }HI_DP_EPON_MSG_TYPE_E;

/* BEGIN: Added by shengjing, 2011/2/24   PN:DTS2011021605342*/
/* TOSģʽѡ��TC or DSCP */
typedef enum
{
    HI_DP_TOS_DSCP_E = 0,
    HI_DP_TOS_TC_E,
} HI_DP_TOS_MODE_E;
/* END:   Added by shengjing, 2011/2/24 */

/*BEGIN: Added by w00180499, 2011/5/11*/
/*ΪxPON˫ģ����Ӧ���ܽӿڶ������ݽṹ*/
typedef enum
{
    HI_DP_PORT_EPON_E = 0,
    HI_DP_PORT_GPON_E,
    HI_DP_PORT_UNKNOWN_E,
} HI_DP_NNI_PORT_E;
#ifdef CONFIG_HISI_SD5115_MP_PILOT

/*tocpu�������ȼ��滻��ʽ*/
typedef enum
{
    HI_DP_PRI_DEFAULT_E = 0,
    HI_DP_PRI_REMAP_E,
} HI_DP_PRI_TYPE_SELECT_E;
#endif


/* 5115���ⱨ�����ͣ���Ϊ������Ϊ�������� */
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
    HI_DP_SPECIAL_PKT_ICMPV6_GB_E,         /* 0xA: ICMPv6 Packet Too Long */ /* ����̫������ */
    HI_DP_SPECIAL_PKT_ICMPV6_OVER_TIME_E,  /* 0xB: ICMPv6 Time Exceeded */   /* ��ʱ����� */
    HI_DP_SPECIAL_PKT_ICMPV6_PARA_ERR_E,   /* 0xC: ICMPv6 Parameter Problem */ /* ���������� */
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

    /* ���������ȼ��ٲ� */
    HI_DP_SPECIAL_PKT_POINT_TYPE0_E,  /* 4���Զ���ETH Type */
    HI_DP_SPECIAL_PKT_POINT_TYPE1_E,
    HI_DP_SPECIAL_PKT_POINT_TYPE2_E,
    HI_DP_SPECIAL_PKT_POINT_TYPE3_E,

    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_0_E,   /* 3���û��Զ���Э�����ͣ�PPPOE_UD_ULPT12��3���� */
    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_1_E,
    HI_DP_SPECIAL_PKT_PPPOE_POINT_TYPE_2_E,

    HI_DP_SPECIAL_PKT_IPV6_EXTERN_HEADER_E,               /* ����չͷ��IPv6���� */
    HI_DP_SPECIAL_PKT_BPDU_E,               
    HI_DP_SPECIAL_PKT_EAPOL_E,  

    
    HI_DP_SPECIAL_PKT_NUM,
    
}HI_DP_SPECIAL_PKT_TYPE_E;

typedef struct
{
    HI_LEVEL_E em_rx_sd_active;    /*���ռ����Ч��ƽ*/
    HI_LEVEL_E em_tx_en_active;    /*������Ч��ƽ*/
} hi_dp_glb_optic_module_attr_s;
/*END: Added by w00180499, 2011/5/11*/

/* MPCP���Ľṹ */
typedef struct
{
    /* �����ĸ�LLID */
    uint ui_llid_index;

    /* ǿ�Ʒ��� */
    HI_BOOL_E em_force_en;

    /* ������ز��� */
    HI_DP_EPON_MSG_TYPE_E em_msg_type;

    /* MPCP֡���� ��MPCPЭ��������н��� */
    uchar uc_data[HI_DP_MPCP_MSG_LEN];
    
} hi_mpcp_message_s;

typedef struct
{
    uint ui_llid_index; /* LLID���� */
    HI_BOOL_E em_force_en;     /*ǿ�Ʒ��ͣ�ʹ�����ָ��LLID����flush*/
    HI_DP_EPON_MSG_TYPE_E em_msg_type; /* ������ز��� */
    uint ui_length;     /*����OAM��Ϣ����*/
    uchar *puc_msg;    /*OAMЭ�龻��ͷָ��,����Ϊui_msg_len*/
} hi_oam_message_s;

/* MAC��ַ */
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

/* BEGIN: Added by shengjing, 2011/5/25   SDK�Ż�  *//*�澯�ϱ�ģʽ*/
typedef enum
{
    HI_DP_ALARM_REAL_TIME_REPORT_E = 0,  /*ʵʱ�ϱ�*/
    HI_DP_ALARM_DELAY_REPORT_E,          /*�ӳ��ϱ�*/
    HI_DP_ALARM_POLL_REPORT_E,           /*��ѯ�ϱ�*/
    HI_DP_ALARM_ONE_TIME_REPORT_E,       /*һ���ϱ�*/
} HI_DP_ALARM_REPORT_MODE_E;

/*ʱ���л�״̬��ȡֵ��Χ*/
typedef enum
{
    HI_DP_LOSS_STATE_E = 0,  /*״̬: �Ѷ�ʧ*/
    HI_DP_RESUME_STATE_E,    /*״̬: �ѻָ�*/
} HI_DP_CLK_SWITCH_STATE_E;

/*�澯���� */
typedef struct
{
    /* �澯ʹ��*/
    HI_BOOL_E em_enable;

    /*�ϱ�ģʽ*/
    HI_DP_ALARM_REPORT_MODE_E em_report_mode;

    /*����*/
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

/* �˿������������ */
typedef struct
{
    HI_BOOL_E em_fctl_en;      /* true:ѡ��BM���أ�����ʹ��, em_fctl_en ��em_car_enֻ��һ��Ϊtrue flase:�ر�BM���� */

    uint ui_high_thr;           /* �˿�ռ�û�����Դ��������ֵ���ޡ�
                                ������ռ��������������ֵʱ��������֡����
                                 */
    uint ui_low_thr;            /* �˿�ռ�û�����Դ��������ֵ���ޡ�
                                ������ռ���������ڸ���ֵʱ��������֡���͡�
                                 */

    HI_BOOL_E em_car_en;      /* true:ѡ��CAR���ز���ʹ�� ,em_fctl_en ��em_car_enֻ��һ��Ϊtrue flase:�ر�CAR����  */
    HI_BOOL_E em_car_pause_en;/* ��CAR����ʱ�Ƿ�ʹ��PAUSE֡���� ���ⵥDTS2011072605802 chenzetian 20110728 */

    uint ui_ebs;             /* ��������ߴ磬����PBS֡��������λΪ64kbps��1byte/125us */
    uint ui_cbs;             /* ��ŵͻ���ߴ磬��λΪ64kbps��1byte/125us */
    uint ui_cir;             /* ��ֵ����     */
    uint ui_dps;             /* ����ˮ�� */

    /* �޷�ֻ���������ߵ�ˮ��, ��Ϊ�±���������������� */

    /*
        --------------a ��ѹˮ��
     |  dps1
        --------------cbs
     |  dps0
        --------------b ��ѹ����ˮ��


        cbs + dps1 = a;
        cbs - dps0 = b;

        ����֪a��b��������޷����dps0,dps1,cbs.
     */

#if 1
    uint ui_dps0;            /* ���سߴ�����, ��ѹˮ�� */
    uint ui_dps1;            /* ���سߴ�����, ������ѹˮ�� */
#endif
} hi_dp_eth_fctl_attr_s;

/* �˿�TAG������� */
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

/* RXDMA/TXDMA �˿�TAG�л����� */
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

/* TXDMA hybrid��ѯ���� */
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

/* TXDMA hybrid��ѯ���� */
typedef struct
{
    uint      ui_vlan;
    HI_BOOL_E nni_tag_del_en;
} hi_dp_nni_hybrid_item_s;

#endif /* end of  */
