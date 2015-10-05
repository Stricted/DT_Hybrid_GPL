/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_omci_me_l3_def.h
  �� �� ��   : ����
  ��    ��   : s
  ��������   : D2011_10_27

******************************************************************************/
#ifndef __HI_OMCI_ME_L3_DEF_H__
#define __HI_OMCI_ME_L3_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#pragma pack(1)

/******************************************************************************
				  IP Host Config Data   
****************************************************************************/
#define HI_OMCI_ME_UNI_MAC_LEN             6 
#define HI_OMCI_ME_UNI_ONTID_LEN           25
#define HI_OMCI_ME_UNI_DOMAIN_LEN          25
#define HI_OMCI_ME_UNI_HOSTNAME_LEN        25
#define HI_OMCI_MAX_IP_HOST_NUM            1
#define HI_OMCI_ME_IPHOST_INSTID_PRIMARY   0x0000

#define HI_OMCI_ME_GET_IPHOST_OPT_WHETHER_DHCP(ucIpOpt)    (ucIpOpt & 0x01)
#define HI_OMCI_ME_GET_IPHOST_OPT_WHETHER_RESPOND_PING(ucIpOpt)    (ucIpOpt & 0x02)
#define HI_OMCI_ME_GET_IPHOST_OPT_WHETHER_RESPOND_TRACEROUTE(ucIpOpt)    (ucIpOpt & 0x04)

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_ipoption;   /* ONT��IP��صĹ�������*/
    hi_uchar8   uc_mac[6];     /* MAC��ַ */
    hi_uchar8   uc_ontid[25];  /* ONT�ı�ʶ��,���25�ֽ�, С��25�ֽ�ʱ,����'\0'���� */
    hi_uint32   ui_ip;         /* ONT��IP��ַ*/
    hi_uint32   ui_mask;       /* ONT������ */
    hi_uint32   ui_gateway;    /* ONT��Ĭ������ */
    hi_uint32   ui_pridns;     /* ONT����DNS */
    hi_uint32   ui_secdns;     /* ONT�Ĵ�DNS*/
    hi_uint32   ui_curip;      /* ��ǰ��IP��ַ */
    hi_uint32   ui_curmask;    /* ��ǰ��Mask */
    hi_uint32   ui_curgateway;   /*��ǰ��Ĭ������*/
    hi_uint32   ui_curpridns;    /*��ǰ����DNS*/
    hi_uint32   ui_cursecdns;    /*��ǰ�Ĵ�DNS*/
    hi_uchar8   uc_domain[25];   /* DOMAIN NAME*/
    hi_uchar8   uc_hostname[25]; /* Host Name*/
}hi_omci_me_iphost_cfg_s;

/************IP host performance monitoring history data****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;
    hi_ushort16 us_thresid;
    hi_uint32   ui_icmperr;
    hi_uint32   ui_dnserr;
    hi_ushort16 us_dhcp_timeout;
    hi_ushort16 us_ipconflict;
    hi_ushort16 us_outofmemory;
    hi_ushort16 us_intvlerr;
}hi_omci_me_iphost_pm_s;

/******TCP/UDP config data******/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_portid;
    hi_uchar8 uc_protocol;
    hi_uchar8 uc_tosfield; /*TOS/diffserv field*/
    hi_ushort16 us_iphost_ptr;
}hi_omci_me_tcpudp_cfg_s;

/*******TCP/UDP performance monitoring history data******/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;
    hi_ushort16 us_thresid;
    hi_ushort16 us_socketfail;
    hi_ushort16 us_listenfail;
    hi_ushort16 us_bindfail;
    hi_ushort16 us_acceptfail;
    hi_ushort16 us_selectfail;
}hi_omci_me_tcpudp_pm_s;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_ME_L3_DEF_H__ */
