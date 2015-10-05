/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : omci_me_ani_def.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2011_09_26

******************************************************************************/
#ifndef __OMCI_ME_ANI_DEF_H__
#define __OMCI_ME_ANI_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#pragma pack(1)

/* UNI counter of GEM Port, including 4 FE ports and 1 virtul router port */
#define HI_OMCI_UNI_CONTER                                 5

/*****************ANI-G    HI_OMCI_ME_ANI_G_E***************/
typedef struct 
{
    hi_uchar8    uc_sr;              /* TRUE-SR */
    hi_ushort16  us_tcontnum;        /* Total T-CONT Number */
    hi_ushort16  us_gemblocklen;     /* GEM Block Length, reporting block size for 
                                         GEM mode in units of Bytes, 0x30-value on autonomous instantiation */
    hi_uchar8    uc_piggyback;       /* Piggyback DBA Reporting
                                         0-mode 0 only;
                                         1-mode 0 and 1;
                                         2-mode 0 and 2; 
                                         3-mode 0,1 and 2;
                                         4-piggyback DBA reporting nor supported */
    hi_uchar8    uc_deprecated;     /* deprecated */
    hi_uchar8    uc_sf;             /* SF threshold, 0x3-0x8, default value is 0x5 */
    hi_uchar8    uc_sd;             /* SD threshold, 0x4-0x10,default value is 0x9 */
    hi_uchar8    uc_arc;
    hi_uchar8    uc_arcinter;
    hi_ushort16  us_optlevel;
    hi_uchar8    uc_loweropt;
    hi_uchar8    uc_upperopt;
    hi_ushort16  us_onursptime;
    hi_ushort16  us_txoptlevel;
    hi_uchar8    uc_lowertx;
    hi_uchar8    uc_uppertx;
}hi_omci_me_ani_g_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_ani_g_s st_anig;
}hi_omci_me_ani_g_msg_s;

/****************GAL Ethernet profile  HI_OMCI_ME_GAL_ETH_PROFILE_E***************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16  us_maxgemsize;   /*maximum GEM payload size generated in the associated GEM IWTP*/ 
}hi_omci_me_gal_eth_s;

/*******************GEM Interworking Termination Point  HI_OMCI_ME_GEM_IWTP_E******************/  
/* get Port Number */
#define HI_OMCI_GET_CES_PORT_NUM(us_instid) (hi_uchar8)((us_instid) & 0xFF)
#define HI_OMCI_ME_MULTICAST_ADDR_TBL_NUM      12

/* Interworking option definition */
typedef enum 
{
    HI_OMCI_TYPE_MEID_TDM   = 0x00, /*Unstructured TDM*/
    HI_OMCI_TYPE_MEID_MAC   = 0x01, /*MAC Bridge LAN*/
    HI_OMCI_TYPE_MEID_RSRV  = 0x02, /*Reserved for Future use*/
    HI_OMCI_TYPE_MEID_IP    = 0x03, /*IP Data Service*/
    HI_OMCI_TYPE_MEID_VIDEO = 0x04, /*Video Return Path*/
    HI_OMCI_TYPE_MEID_802   = 0x05, /*802.1p Mapper*/
}HI_GEMIWTP_OPTION_E;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_gemctp_ptr;        /* GEM port network CTP connectivity pointer*/
    hi_uchar8   uc_interworking;      /* Interworking option*/
    hi_ushort16 us_svc_ptr;           /* Service Profile Pointer */
    hi_ushort16 us_itp_ptr;      /* Interworking termination point pointer*/
    hi_uchar8   uc_pptpcnt;           /*PPTP counter*/
    hi_uchar8   uc_operstate;         /*Operational state*/
    hi_ushort16 us_gal_ptr;           /*GAL profile pointer*/
    hi_uchar8   uc_galloopback;       /*GAL loopback configuration*/
}hi_omci_me_gem_iwtp_s;

/***************GEM port network CTP   HI_OMCI_ME_GEM_CTP_E****************/
/* GEM port direction */
typedef enum 
{
    HI_PORTID_UNI_TO_ANI = 1,  /*UNI-to-ANI*/
    HI_PORTID_ANI_TO_UNI,       /*ANI-to-UNI*/
    HI_PORTID_BIDIRECTIONAL,   /*bidirectional */
}HI_PORTID_DIRECTION_E;

/*Traffic Management Option*/
typedef enum 
{
    HI_TRAFFIC_PQ = 0, /*Priority controlled and flexibly scheduled upstream traffic*/
    HI_TRAFFIC_CAR,    /*Cell rate controlled upstream traffic*/
}HI_TRAFFIC_OPTION_E;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_portid;               /*Port-ID */
    hi_ushort16 us_tcont_ptr;            /*T-CONT pointer*/
    hi_uchar8   uc_direction;                  /*Direction*/
    hi_ushort16 us_traffic_mng_ptr;      /* Traffic management pointer for upstream*/
    hi_ushort16 us_traffic_up_ptr;       /*Traffic descriptor profile pointer for upstream*/
    hi_uchar8   uc_unicnt;               /*UNI counter*/
    hi_ushort16 us_pqdn_ptr;             /*Priority queue pointer for downstream*/
    hi_uchar8   uc_encrystate;           /*Encryption state*/
    hi_ushort16 us_traffic_dn_ptr;       /*Traffic descriptor profile pointer for downstream*/
    hi_uchar8   uc_encrykey;             /*Encryption key ring*/
} hi_omci_me_gem_ctp_s;

/********************multicast gem interworking termination point  HI_OMCI_ME_MULTICAST_IWTP_E*******************/

#define HI_MULTICAST_ADDRESS_TABLE_MAX_NUM     255

typedef struct
{
    hi_ushort16  us_gemportid;
    hi_ushort16  us_secondaryindex;
    hi_uint32    ui_startip;
    hi_uint32    ui_endip;  
}hi_omci_me_multi_addrtable_s;

typedef struct
{
    hi_ushort16  us_gemportid;
    hi_ushort16  us_secondaryindex;
    hi_ushort16  us_multivlan;
    hi_ushort16  us_usergemportid;
    hi_uint32    ui_multsrcip;  
}hi_omci_me_ex_multi_addrtable_s;

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16  us_ctp_ptr;          /*GEM port network CTP connectivity pointer*/
    hi_uchar8    uc_interworking;     /* Interworking option*/
    hi_ushort16  us_svc_ptr;          /* Service profile pointer */
    hi_ushort16  us_resv1;           /* Not used 1*/
    hi_uchar8    uc_pptpcnt;
    hi_uchar8    uc_operstate;        /* 0-a ME is capable of performing its task ,1-disable*/
    hi_ushort16  us_gal_ptr;          /* 0-GAL TDM Porfile,1-GAL Ethernet profile*/
    hi_uchar8    uc_resv2;          /* Not used 2*/
    hi_omci_me_multi_addrtable_s mcaddresstable;
    hi_omci_me_ex_multi_addrtable_s ex_mcaddresstable;
}hi_omci_me_multigemiwtp_s;

/***********************tcont    HI_OMCI_ME_TCONT_E*********************************/
/* get tcont id according instance ID */
#define HI_OMCI_GET_TCONT_ID(us_instid)     (hi_uchar8)((us_instid) & 0xFF)

/*Entity structure*/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16  us_allocid;     /*Alloc-ID*/
    hi_uchar8    uc_deprecated;  /*Deprecated*/
    hi_uchar8    uc_policy;      /*Policy*/
}hi_omci_me_tcont_s;

/*******************Traffic scheduler   HI_OMCI_ME_TRAF_SCHED_E****************/
typedef struct
{
    hi_ushort16 us_tcont_ptr;   /*T-CONT pointer*/
    hi_ushort16 us_sched_ptr;   /*Traffic scheduler pointer*/
    hi_uchar8   uc_policy;      /*Policy*/
    hi_uchar8   uc_pri_wght;     /*Priority/weight*/
}hi_omci_me_traffic_sche_entry_s;

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_traffic_sche_entry_s st_trafficsche;
}hi_omci_me_traffic_sche_s;

/***********************Traffic descriptor   HI_OMCI_ME_GEM_TRAF_DESC_E*********************/
/* Flow configuration struct*/  
typedef struct 
{
    hi_ushort16 us_gemportid;     /* PortID,128~3999 */
    hi_uchar8  uc_flowtype;      /*Flow Type*/
    hi_uchar8  uc_tcontid;       /*T-CONT ID*/
    hi_ushort16 us_vlanid;
    hi_uchar8  uc_pri;   
    hi_uchar8  uc_phyport;
    hi_ushort16 us_allocid;
    hi_ushort16 us_uspq;          /*上行pq队列*/
    //hi_ushort16 us_carid;         /* gemport car or flow car*/
}hi_omci_flow_to_gemport_map_s;

typedef struct 
{
    hi_uint32   ui_cir;             /*CIR*/
    hi_uint32   ui_pir;             /*PIR*/
    hi_uint32   ui_cbs;             /*CBS*/
    hi_uint32   ui_pbs;             /*PBS*/
    hi_uchar8   uc_colormode;       /*Colour mode*/
    hi_uchar8   uc_ingcolor;        /*Ingress colour marking*/
    hi_uchar8   uc_egrcolor;        /*Egress colour marking*/
    hi_uchar8   uc_metertype;       /*Meter type*/
}hi_omci_me_traffic_desc_entry_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_traffic_desc_entry_s st_traffic;
}hi_omci_me_traffic_desc_s;

/***********************priority queue   HI_OMCI_ME_PQ_E******************************/
typedef struct 
{
    hi_uchar8   uc_queuecfg;          /*Queue configuration option*/
    hi_ushort16 us_maxqueue;          /*Maximum queue size*/
    hi_ushort16 us_allocsize;         /*Allocated queue size*/
    hi_ushort16 us_resetintvl;        /*Discard-block counter reset interval*/
    hi_ushort16 us_discard;           /*Threshold value for discarded blocks due to buffer overflow*/
    hi_uint32   ui_relatedport;       /*Related port*/
    hi_ushort16 us_traffic_ptr;       /*Traffic scheduler pointer*/
    hi_uchar8   uc_weight;            /*Weight*/
    hi_ushort16 us_operation;         /*Back pressure operation*/
    hi_uint32   ui_time;              /*Back pressure time*/
    hi_ushort16 us_occurthresh;       /*Back pressure occur queue threshold*/
    hi_ushort16 us_clearthresh;       /*Back pressure clear queue threshold*/
    hi_ushort16 us_qthresh[4];       /* Packet drop queue thresholds*/
    hi_ushort16 us_max_p;             /*Packet drop max_p*/
    hi_uchar8   uc_qdrop;             /*Queue drop w_q*/
    hi_uchar8   uc_dropmark;          /*Drop precedence colour marking*/
}hi_omci_me_pq_entry_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_pq_entry_s   st_pq;
}hi_omci_me_pq_s;

/***********GAL Ethernet performance monitoring history data  HI_OMCI_ME_GAL_ETH_PM_E***********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_interendtime;     /*Interval end time*/
    hi_ushort16 us_thresholdid; /*Threshold data 1/2 id*/
    hi_uint32   ui_discardfrm;  /*Discarded frames*/
}hi_omci_me_galeth_pm_s; 

/************FEC performance monitoring history data****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;      /*Interval end time*/
    hi_ushort16 us_thresid;      /*Threshold data 1/2 id*/
    hi_uint32   ui_correctbytes; /*Corrected bytes*/
    hi_uint32   ui_correctcode;  /*Corrected code words*/
    hi_uint32   ui_uncorrectcode;/*Uncorrectable code words*/
    hi_uint32   ui_totalcode;    /*Total code words*/
    hi_ushort16 us_fecseconds;   /*FEC seconds*/
}__attribute__ ((packed)) hi_omci_me_fec_pm_s;

/***********GEM port network CTP performance monitoring history data*************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;        /*Interval end time*/
    hi_ushort16 us_thresid;        /*Threshold data 1/2 id*/
    hi_uint32   ui_transframes;    /*Transmitted GEM frames*/
    hi_uint32   ui_rcvframes;      /*Received GEM frames*/
    hi_uchar8   uc_rcvpayload[8];  /*Received payload bytes*/
    hi_uchar8   uc_transpayload[8];/*Transmitted payload bytes*/
    hi_uint32   ui_encryerrs;      /*Encryption key errors*/
}hi_omci_me_gemctp_pm_s;
#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OMCI_ME_ANI_DEF_H__ */
