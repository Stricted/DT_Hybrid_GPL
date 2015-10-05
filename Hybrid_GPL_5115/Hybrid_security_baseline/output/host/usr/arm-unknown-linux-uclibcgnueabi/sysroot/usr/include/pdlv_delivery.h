/*******************************************************************************
  �� �� ��   : 
  �� �� ��   : 
  ��    ��   :   
  ��������   : 
  ��������   : ���ķַ�ģ�����ӿ�
  �޸���ʷ   : 
*******************************************************************************/
#ifndef _PDLV_INTERFACE_H
#define _PDLV_INTERFACE_H

#include "hi_drv_common.h"

#include <linux/skbuff.h>


/*******************************************************************************
  ��������
*******************************************************************************/
/* ͨ�ö��� */
#define PDLV_COMMON_VALID               1           /* ��ʾ��Ч               */
#define PDLV_COMMON_INVALID             0           /* ��ʾ��Ч               */
#define PDLV_MACADDR_LEN                6           /* MAC��ַ����            */

/* handler�����ķ���ֵ, ��ֵ����ģ�鴦�������ݰ���Ķ��� */
#define PDLV_PACKET_TO_CUSTOM           1           /* ָ��ת��               */
#define PDLV_PACKET_TO_NORMAL           2           /* ����ת��               */
#define PDLV_PACKET_TO_KERNEL           3           /* ת����Э��ջ           */
#define PDLV_PACKET_TO_DROP             4           /* �������ݰ�             */
#define PDLV_PACKET_TO_NEXT             5           /* ����������һ���ݰ� */


/* ����˿ڱ�� */
#define PDLV_ETHPORT_GE0                0           /* GE0 / FE0              */
#define PDLV_ETHPORT_GE1                1           /* GE1 / FE1              */
#define PDLV_ETHPORT_FE2                2           /* GE2                    */
#define PDLV_ETHPORT_FE3                3           /* GE3                    */
#define PDLV_ETHPORT_FE4                4           /* GE3                    */
#define PDLV_ETHPORT_PON                5           /* PON                    */
#define PDLV_ETHPORT_CPU                6           /* CPU                    */
#define PDLV_ETHPORT_MAX                7           /* �˿���Ŀ���ֵ         */


/*������*/
#define PDLV_SUCCESS 0

/*��������ֵ*/
#define PDLV_REGISTER_FAULT          1
#define PDLV_TX_FAULT                2
#define PDLV_INVALID_PARAMETER       3
#define PDLV_MODE_ERROR              4

/*���ⵥAB3D06690��s00149918,2009/11/04*/
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

/* BEGIN: Modified by shengjing, 2009/3/19   ����gpon/eponģʽѡ��*/
#define PDLV_GPON_MODE 1
#define PDLV_EPON_MODE 2

/* BEGIN: Modified by shengjing, 2009/3/6   ����:����llid��gemport����*/
#define PDLV_INVALID_LLID_GEMPORT 0xFFFFFFFF


/*******************************************************************************
  ���ݽṹ
*******************************************************************************/
/*------------------------------------------------------------------------------
  ���Ľ���
------------------------------------------------------------------------------*/
/* �������нṹ��ԱΪ����(С)�ֽ��� */
/* TAG */
struct pdlv_taghdr_s
{
    unsigned int    valid;                          /* ��ʶ�ýṹ�Ƿ���Ч     */
    unsigned short  tpid;                           /* TAG ���               */
    unsigned short  priority;                       /* ���ȼ�                 */
    unsigned short  cfi;                            /* CFI                    */
    unsigned short  vlan;                           /* VLAN                   */
};

/* MAC ��ַ */
struct pdlv_machdr_s
{
    unsigned char         dmac[PDLV_MACADDR_LEN];   /* Ŀ��MAC                */
    unsigned char         smac[PDLV_MACADDR_LEN];   /* ԴMAC                  */
    struct pdlv_taghdr_s  tag_ex;                   /* ���TAG                */
    struct pdlv_taghdr_s  tag_in;                   /* �ڲ�TAG                */
    unsigned short        eth_proto_len;            /* ��̫��Э��/��������    */
    unsigned short        reserved;                 /* ����δ��               */
    unsigned int          hdr_len;                  /* ��̫������ͷ����       */
};

struct pdlv_pkt_s
{
    unsigned int                ingress;            /* ������Դ�˿�           */
    unsigned int                reason;             /* �����ύCPU��ԭ��      */

    unsigned int                llid;               /*llid*/
    unsigned int                gemport;            /*gemport*/

    struct pdlv_machdr_s        mac_hdr;            /* MAC ͷ��               */
};
/* END: Modified by shengjing, 2009/3/6*/

/*------------------------------------------------------------------------------
  ����ͳ��
------------------------------------------------------------------------------*/
/* �ַ�ģ���շ����ĵ����� */
struct pdlv_stat_common_s
{
    unsigned long  rx_total;                        /* ���ձ�������           */
    unsigned long  tx_total;                        /* ��̽��ת�����ĵ�����   */
};

/* ���Ķ������� */
struct pdlv_stat_dropping_s
{
    unsigned long  dropping_flag;                   /* ����Я��������ʶ       */
    unsigned long  length_less60;                   /* ����С��60�ֽ�         */
    unsigned long  module_feedback;                 /* ԭ��Ϊ802.1ag�ķǷ�����*/
};

/* �����ķַ�ԭ����м��� */
struct pdlv_stat_reason_s
{
    unsigned long  normal_to_cpu;                   /* ����ת����CPU�ı���    */
    unsigned long  efc0_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc1_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc2_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc3_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc4_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc5_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc6_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc7_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc8_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc9_matched;                    /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc10_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc11_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc12_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc13_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc14_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  efc15_matched;                   /* �Զ���EFCƥ�佻CPUԭ�� */
    unsigned long  unmatched_napt;                  /* ΪNAPT����EFCƥ��ʧ��  */
    unsigned long  untag;                           /* UNTAG����              */
    unsigned long  vlan_tag;                        /* VLAN TAG����           */
    unsigned long  priority_tag;                    /* PRI UNTAG����          */
    unsigned long  broadcast;                       /* �㲥����               */
    unsigned long  igmp_pppoe;                      /* �鲥����               */
    unsigned long  igmp_ipoe;                       /* �鲥����               */
    unsigned long  arp;                             /* ARP����                */
    unsigned long  rarp;                            /* RARP����               */
    unsigned long  pppoe_discovery;                 /* PPPOE DISCOVERY����    */
    unsigned long  eapol;                           /* EAPOL����              */
    unsigned long  bpdu;                            /* BPDU����               */
    unsigned long  dhcp;                            /* DHCP����               */
    //unsigned long  ipv6;                            /* IPV6����               */
    unsigned long  ICMP;                            /* ICMP����               */
    unsigned long  custom_type0;                    /* �Զ��岶��ı���       */
    unsigned long  custom_type1;                    /* �Զ��岶��ı���       */
    unsigned long  custom_type2;                    /* �Զ��岶��ı���       */
    unsigned long  custom_type3;                    /* �Զ��岶��ı���       */
    //unsigned long  pppoe_session;                   /* PPPOE SESSION����      */
    unsigned long  port_moved;                      /* ��̬����Ķ˿ڷ����ƶ� */
    unsigned long  mac_learn;                       /* �����ύ��CPUѧϰ      */
    unsigned long  fail_to_learn;                   /* �߼�ѧϰ����ʧ��       */
    unsigned long  fail_to_search;                  /* �߼����ʧ��           */
    unsigned long  succ_to_search;                  /* �߼����ɹ�           */
    unsigned long  napt_accel_on;                   /* NAPT���ٿ���ʱ�װ��ύ */
    //unsigned long  napt_accel_off;                  /* NAPT���ٹر�ʱ�����ύ */
    unsigned long  unmatched_napt_outnet;           /* NAPT��OUTNET��ƥ��   */
    unsigned long  oam8023ah;                       /* 802.3ah����            */
    unsigned long  oam8021ag_y1731;                 /* 802.1ag / Y.1731����   */
    //unsigned long  pdelay_req;                      /* pdelay_req����         */
    //unsigned long  pdelay_resp;                     /* pdelay_resp����        */
    //unsigned long  pdelay_resp_follow_up;           /* pdelay_resp_follow_up���� */
    //unsigned long  pdelay_management;               /* management����         */
    //unsigned long  pdelay_signaling;                /* signaling����          */
    //unsigned long  pdelay_announce;                 /* announce����           */
    unsigned long  pdelay_other_1588v2;             /* ����1588Э�鱨��       */
    //unsigned long  down_fail_to_cpu;                /* NAPT���в��ʧ�ܽ�CPU  */
    unsigned long  ip_fragment;                     /* IP��Ƭ����     */

    /* V200���� c00133939 */
    //unsigned long tunnel_unknow_mc;                 /* ���δ֪�鲥���Ľ�CPU */
    //unsigned long ipv6_unknow_mc;                   /* IPv6δ֪�鲥���Ľ�CPU */
    //unsigned long ipv4_unknow_mc;                   /* IPv4δ֪�鲥���Ľ�CPU */
    
    unsigned long napt_egr_is_cpu;                  /* NAPT���ٵ��û��ڣ�����WLAN�� */
    unsigned long rt_to_gw_if;                      /* ·�ɵ����ؽӿ� */
    unsigned long l2_fwd_gw_if;                     /* L2ת�������ؽӿ� */
    unsigned long efc_specify_to_wlan;              /* EFCָ��ת����WLAN */
    unsigned long ipv6_unknow_extern_header;        /* IPV6��֪����չͷ */
    unsigned long ipv6_sip_site_local;              /* IPv6ԴIPΪSite Local */
    unsigned long ipv6_sip_0;                       /* IPV6 SIP����0 */
    unsigned long ipv6_sip_link_local;              /* IPv6ԴIPΪLink Local��ַ */
    unsigned long pppoe_session_lcp;                /* PPPOE  Session�׶ε�LCP���� */
    unsigned long pppoe_session_ncp;                /* PPPOE Session�׶ε�NCP���� */
    unsigned long pppoe_session_chap;               /* PPPOE Session�׶ε�CHAP���� */
    unsigned long pppoe_session_pap;                /* PPPOE Session�׶ε�PAP���� */
    unsigned long pppoe_session_ipv6cp;             /* PPPOE Session�׶ε�IPV6CP���� */
    unsigned long pppoe_session_point_0;            /* PPPOE Session�׶ε��û��Զ�������0���� */
    unsigned long pppoe_session_point_1;            /* PPPOE Session�׶ε��û��Զ�������1���� */
    unsigned long pppoe_session_point_2;            /* PPPOE Session�׶ε��û��Զ�������2���� */
    unsigned long dhcpv6;                           /* DHCPV6���� */
    unsigned long icmpv6_rs;                        /* ICMPV6 RS���� */
    unsigned long icmpv6_ra;                        /* ICMPV6 RA���� */
    unsigned long icmpv6_ns;                        /* ICMPV6 NS���� */
    unsigned long icmpv6_na;                        /* CMPV6 NA���� */
    unsigned long icmpv6_rm;                        /* ICMPV6 RM���� */
    unsigned long icmpv6_ping_req;                  /* ICMPV6 PingReq���� */
    unsigned long icmpv6_ping_reply;                /* ICMPV6 PingReply���� */
    unsigned long icmpv6_unreach;                   /* ICMPv6Ŀ�Ĳ��ɴ����� */
    unsigned long icmpv6_group_big;                 /* ICMPv6����̫���� */
    unsigned long icmpv6_outtime;                   /* ICMPv6��ʱ����� */
    unsigned long icmpv6_param_error;               /* ICMPv6���������� */
    unsigned long ipoe_mld_query_v1_v2;             /* IPOE���ص�MLD Query v1/v2���� */
    unsigned long ipoe_mld_report_v1;               /* IPOE���ص�MLD Report v1���� */
    unsigned long ipoe_mld_report_v2;               /* IPOE���ص�MLD Report v2���� */
    unsigned long ipoe_mld_done;                    /* IPOE���ص�MLD DONE���� */
    unsigned long pppoe_mld_query_v1_v2;            /* PPPOE���ص�MLD Query v1/v2���� */
    unsigned long pppoe_mld_report_v1;              /* PPPOE���ص�MLD Report v1���� */
    unsigned long pppoe_mld_report_v2;              /* PPPOE���ص�MLD Report v2���� */
    unsigned long pppoe_mld_done;                   /* PPPOE���ص�MLD DONE���� */
    unsigned long ipv6_extern_header_num_larger_1;  /* ����չͷ��IPV6���ģ�MLD��Э�鱨�ĳ��⣩ */
    unsigned long header_laregr_128_byte;           /* ����ͷ���ȳ���128B */
    unsigned long rt_fwd_cpu_uni;                   /* ·��ת����CPU�û��� */
    unsigned long hl_ttl_less_1;                    /* HOP_LIMIT/TTLС�ڵ���1������ */
    unsigned long rt_fail;                          /* ·��ʧ�ܣ���Ĭ��·�ɣ� */
    unsigned long black_hole_rt;                    /* �ܾ�·�� */
    unsigned long no_next_hop;                      /* ����һ�� */
    unsigned long in_tunnel_mtu_fail;               /* �����MTU���ʧ�� */
    unsigned long bc;                               /* �㲥���� */
    unsigned long uc;                               /* δ֪�������� */
    unsigned long mc;                               /* ��֪�鲥����ת����CPU�û��� */
    unsigned long sipv6_loop;                       /* SIPv6���ڻ��ص�ַ */
    unsigned long sipv6_mc;                         /* SIPv6�����鲥��ַ */
    unsigned long sipv4_0;                          /* SIPv4����0 */
    unsigned long sipv4_loop;                       /* SIPv4���ڻ��ص�ַ */
    unsigned long sipv4_mc;                         /* SIPv4�����鲥��ַ */

    unsigned long rtcopycpu;                         /* ·��+����CPU */
    unsigned long naptcopycpu;                         /* NAPT+����CPU */
    unsigned long snaplarger1500;                         /* SNAP���ȳ���1500 */
};

/* ͳ�Ʊ��ķַ� */
struct pdlv_stat_delivery_s
{
    unsigned long  to_voip;                         /* �ַ��� VOIP ģ��       */
    unsigned long  to_module;                       /* �ַ�������ģ��         */
    unsigned long  to_kernel;                       /* �ַ����ں�             */
};

/*����ͳ��*/
struct pdlv_stat_transmit_s
{
    unsigned long to_eth;
    unsigned long to_gpon;
    unsigned long to_epon;
    unsigned long to_forward;
};

/* ����ͳ�� */
struct pdlv_stat_total_s
{
    struct pdlv_stat_common_s   common;             /* �ַ�ģ���շ����ĵ����� */
    struct pdlv_stat_dropping_s dropping;           /* ���Ķ�������           */
    struct pdlv_stat_reason_s   reason;             /* �����ķַ�ԭ����м��� */
    struct pdlv_stat_delivery_s delivery;           /* ͳ�Ʊ��ķַ�           */

    /* BEGIN: Modified by shengjing, 2009/3/11   ���ӷ���ͳ��*/
    struct pdlv_stat_transmit_s transmit;
};

/*�������ݰ���eth*/
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

/*�������ݰ���gpon*/
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

/*�������ݰ���epon*/
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

/*cpu����ת�����ݰ�*/
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
  �ص���������
*******************************************************************************/
/*
    ����: 
    @ ����: ���ݰ���������ַ 
    @ ����: ���ݰ���
    @ ����: ���ݰ����������Ϣ
    @ ����: ��������(ͨ��Ϊ struct sk_buff *)
    @ ���: ת��ʹ�õ� gemport (�����Ҫת�����ĵ� PON, ��������/�鲥/�㲥)
    @ ���: ת��ʹ�õ� tcont   (�����Ҫת�����ĵ� PON, ��������/�鲥/�㲥)
*/
typedef int (*pdlv_function_handler)(unsigned char *,
                                     unsigned int,
                                     const struct pdlv_pkt_s *,
                                     void *);

/*******************************************************************************
  �ӿ�����
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
/* BEGIN: Modified by shengjing, 2009/2/19   ���ӷ����ӿ�*/
int pdlv_sendto_epon(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority);
int pdlv_sendto_gpon(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority);
int pdlv_sendto_eth(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority);

int pdlv_sendto_epon_ex(unsigned char *data, unsigned int length, unsigned int llid, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_gpon_ex(unsigned char *data, unsigned int length, unsigned int tcont, unsigned int gemport, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_eth_ex(unsigned char *data, unsigned int length, unsigned int portmask, unsigned int priority, unsigned int timestamp_add);
int pdlv_sendto_forward(unsigned char *data, unsigned int length);
#endif



/* END:   Modified by shengjing, 2009/2/19 */


//s00149918 ��ԭ�е�eth�е�ע�ắ�� �Ƶ�pdlv
/*ģ��ע����*/
#define ETH_FUNC_RX_WAN       (0)    /*�հ�����wandevע��ĺ����� */
#define ETH_FUNC_RX_CAP       (1)    /*�հ����򲶻�ע��ĺ����� */
#define ETH_FUNC_RX_PDLV      (2)    /*�հ�����pdlvע��ĺ����� */

#define ETH_FUNC_TX_CAP       (0)     /*�հ����򲶻�ע��ĺ����� */

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
