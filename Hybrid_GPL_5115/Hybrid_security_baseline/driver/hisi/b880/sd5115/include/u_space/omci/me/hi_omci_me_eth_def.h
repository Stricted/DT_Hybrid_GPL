/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : omci_me_eth_def.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2011_09_29

******************************************************************************/
#ifndef __OMCI_ME_ETH_DEF_H__
#define __OMCI_ME_ETH_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum 
{
    HI_OMCI_ETHIF_CFG_AUTOSENSING_E               = 0x00,/*Auto-Sensing*/
    HI_OMCI_ETHIF_CFG_10_BASE_T_E                 = 0x01,/*10BaseT*/
    HI_OMCI_ETHIF_CFG_100_BASE_T_E                = 0x02,/*100BaseT*/  
    HI_OMCI_ETHIF_CFG_GIGA_ETH_E                  = 0x03,/*Gigabit Ethernet*/        
    HI_OMCI_ETHIF_CFG_10_BASE_T_AUTOSENSING_E     = 0x10,/*10BaseT Auto-sensing*/
    HI_OMCI_ETHIF_CFG_10_BASE_T_HALF_DUPLEX_E     = 0x11,/*10BaseT (half duplex)*/  
    HI_OMCI_ETHIF_CFG_100_BASE_T_HALF_DUPLEX_E    = 0x12,/*100BaseT (half duplex)*/
    HI_OMCI_ETHIF_CFG_GIGA_ETH_HALF_DUPLEX_E      = 0x13,/*Gigabit Ethernet (half duplex)*/
    HI_OMCI_ETHIF_CFG_GIGA_ETH_AUTOSENSING_E      = 0x20 /*Gigabit Ethernet Auto-sensing*/
}hi_omci_ethif_cfg_opt_e;

/*Eth Loop Mode*/
typedef enum 
{
    HI_OMCI_ETH_NO_LOOPBACK                = 0x00,  /*No loopback*/
    HI_OMCI_ETH_LOOPBACK                      = 0x03, /*Loopback of downstream traffic after PHY transceiver*/
}hi_omci_me_EthLoopMode_e;

#define HI_OMCI_ADMINSTATE_UNLOCK 0x00
#define HI_OMCI_ADMINSTATE_LOCK    0x01

#define HI_OMCI_DISABLE   0x00
#define HI_OMCI_ENABLE    0x01

/************************PPTP Ethernet UNI   HI_OMCI_ME_PPTP_ETH_UNI_E*******************************/ 
/* 以太端口Link状态 */
#define HI_OMCI_ETH_LINK_UP                        1
#define HI_OMCI_ETH_LINK_DOWN                      0
/*PPTP ETH实体的起始实例ID编号*/
#define HI_OMCI_ME_ETH_MIN_INST_ID    0x0101
#define HI_OMCI_GET_PORTID_FROM_INSTID(us_InstId)  (((us_InstId) & 0x00FF))

#pragma pack(1)

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_exptype;           /* 0-autosensing*/
    hi_uchar8   uc_sensetype;         /* if uc_exptype is not equal 0,uc_exptype equals uc_sensetype*/
    hi_uchar8   uc_autoconfg;         /* 0-auto,1-10BaseT,2-100BaseT,3-GE*/
    hi_uchar8   uc_loopback;          /* 0-no loopback,3-loopback of downstream 
                                         traffic after PHY transceiver*/
    hi_uchar8   uc_adminstate;        /* 0-active(unlock:0),1-deactive(lock:1)*/
    hi_uchar8   uc_operstate;         /* 0-a ME is capable of performing its task,1-disable*/
    hi_uchar8   uc_configind;         /* TRUE-full duplex,FALSE-half duplex*/
    hi_ushort16 us_maxframe;          /* maximum allowed frame size*/
    hi_uchar8   uc_dteind;
    hi_ushort16 us_pausetime;
    hi_uchar8   uc_bridgeind;
    hi_uchar8   uc_arc;
    hi_uchar8   uc_arcintvl;
    hi_uchar8   uc_pppoefilter;
    hi_uchar8   uc_powerctrl;
}hi_omci_me_pptpethuni_s;

/**************Ethernet performance monitoring history data  HI_OMCI_ME_ETH_PM_E****************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;    
    hi_ushort16 us_thresid;
    
    hi_uint32  ui_fcserrors;       
    hi_uint32  ui_excesscnt;    /*Excessive collision counter*/
    hi_uint32  ui_latecnt;      /*Late collision counter*/
    hi_uint32  ui_frmlong;      /*Frames too long*/
    
    hi_uint32  ui_overflowrcv;  /*Buffer overflows on receive*/
    hi_uint32  ui_overflowtrans;/*Buffer overflows on transmit*/
    hi_uint32  ui_singlcoll;    /*Single collision frame counter*/
    hi_uint32  ui_mulcoll;      /*Multiple collisions frame counter*/

    hi_uint32  ui_sqe;          /*SQE counter*/
    hi_uint32  ui_defertrans;   /*Deferred transmission counter*/
    hi_uint32  ui_mactranserr;  /*Internal MAC transmit error counter*/
    hi_uint32  ui_senseerr;     /*Carrier sense error counter*/

    hi_uint32  ui_alignerr;     /*Alignment error counter*/
    hi_uint32  ui_macrcverr;    /*Internal MAC receive error counter*/
}__attribute__ ((packed)) hi_omci_me_msg_ethpm_s;

/********Ethernet performance monitoring history data 2  HI_OMCI_ME_ETH_PM2_E********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime; /* Interval end time */
    hi_ushort16 us_thresid; /* Threshold data 1/2 id */
    hi_uint32   ui_filtercnt;/*   PPPoE filtered frame counter*/
}__attribute__ ((packed)) hi_omci_me_ethpm2_s;

/********Ethernet performance monitoring history data 3  HI_OMCI_ME_ETH_PM3_E********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;    
    hi_ushort16 us_thresid;
    
    hi_uint32  ui_dropsevt;
    hi_uint32  ui_octets;
    hi_uint32  ui_packets;
    hi_uint32  ui_bcpkt;
    
    hi_uint32  ui_mcpkt;
    hi_uint32  ui_undersizepkt;
    hi_uint32  ui_fragments;
    hi_uint32  ui_jabber;

    hi_uint32  ui_pkt64;
    hi_uint32  ui_pkt65to127;
    hi_uint32  ui_pkt128to255;
    hi_uint32  ui_pkt256to511;
    hi_uint32  ui_pkt512to1023;
    hi_uint32  ui_pkt1024to1518;
}__attribute__ ((packed)) hi_omci_me_msg_ethpm3_s;

/********Ethernet performance monitoring history data 4  HI_OMCI_ME_ETH_PM4_E********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8  uc_endtime;    
    hi_ushort16 us_thresid;
    
    hi_uint32  ui_rcvucpkt;
    hi_uint32  ui_senducpkt;
    hi_uint32  ui_sendbcpkt;
    hi_uint32  ui_sendmcpkt;    
    hi_uint32  ui_rcvpauseflowctlfrm;
    hi_uint32  ui_sendpauseflowctlfrm;
    
    hi_ulong64  ui_rcvgoodpkt;
    hi_ulong64  ui_sendgoodpkt;
    hi_ulong64  ui_rcvbadpkt;
    hi_ulong64  ui_sendbadpkt;
    
    hi_uchar8   uc_bytelength;    
}__attribute__ ((packed)) hi_omci_me_msg_ethpm4_s;

/**************virtual eth interface point***************/
typedef struct
{
   hi_omci_me_msg_head_s st_msghead;
   hi_uchar8   uc_admstate;
   hi_uchar8   uc_optstate;
   hi_uchar8   uc_domainname[25];
   hi_ushort16 us_tcpudp_ptr;
   hi_ushort16 us_ianaasgport;
}hi_omci_me_veip_s;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OMCI_ME_ETH_DEF_H__ */
