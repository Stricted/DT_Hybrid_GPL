/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_me_l3_def.h
  版 本 号   : 初稿
  作    者   : s
  生成日期   : D2011_10_27

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
    hi_uchar8   uc_ipoption;   /* ONT的IP相关的功能设置*/
    hi_uchar8   uc_mac[6];     /* MAC地址 */
    hi_uchar8   uc_ontid[25];  /* ONT的标识符,最多25字节, 小于25字节时,需以'\0'结束 */
    hi_uint32   ui_ip;         /* ONT的IP地址*/
    hi_uint32   ui_mask;       /* ONT的掩码 */
    hi_uint32   ui_gateway;    /* ONT的默认网关 */
    hi_uint32   ui_pridns;     /* ONT的主DNS */
    hi_uint32   ui_secdns;     /* ONT的次DNS*/
    hi_uint32   ui_curip;      /* 当前的IP地址 */
    hi_uint32   ui_curmask;    /* 当前的Mask */
    hi_uint32   ui_curgateway;   /*当前的默认网关*/
    hi_uint32   ui_curpridns;    /*当前的主DNS*/
    hi_uint32   ui_cursecdns;    /*当前的次DNS*/
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
