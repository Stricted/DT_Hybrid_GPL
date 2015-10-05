/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : omci_me_l2_def.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2011_09_27
******************************************************************************/
#ifndef __OMCI_ME_L2_DEF_H__
#define __OMCI_ME_L2_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**************802.1p mapper service profile  HI_OMCI_ME_802_1P_E**************/
#define HI_OMCI_DSCP_2_P_LEN      24 
#pragma pack(1)

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_tp_ptr;
    hi_ushort16 us_iwtp_ptr0;
    hi_ushort16 us_iwtp_ptr1;
    hi_ushort16 us_iwtp_ptr2;
    hi_ushort16 us_iwtp_ptr3;
    hi_ushort16 us_iwtp_ptr4;
    hi_ushort16 us_iwtp_ptr5;
    hi_ushort16 us_iwtp_ptr6;
    hi_ushort16 us_iwtp_ptr7;
    hi_uchar8   uc_frameoption;       /* how the ONT should handle Untagged Ethernet frames
                                              received across the associated Ethernet interface*/
    hi_uchar8   uc_dscp2p[HI_OMCI_DSCP_2_P_LEN]; /* in conjunction with the Unmarked Frame Option attribute*/ 
    hi_uchar8   uc_defaultmark;       /* in conjunction with the Unmarked Frame Option attribute*/
    hi_uchar8   uc_tptype;            /* 0x0 if the mapper is used for Bridging-mapping. 1-PPTP ETH UNI,2-IP Host Service*/
}hi_omci_me_8021p_s;

/****************MAC bridge configuration data  HI_OMCI_ME_MAC_CFG_DATA_E**************/
#define HI_OMCI_DESIGNATED_ROOT_LEN 8   
#define HI_OMCI_DEFAULT_BRIDGE_PRI  0x8000     /* default bridge priority */    
#define HI_OMCI_DEFAULT_ROOT_PATH_COST  0   /* default root path cost */    
#define HI_OMCI_DEFAULT_ROOT_PORT_NUM  0    /* default root path cost */    
#define HI_OMCI_MAC_DEFAULT_HELLO 0x0800      /* default Hello time */    
#define HI_OMCI_MAC_DEFAULT_DELAY 0x1e00      /* default Foward delay */

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_macaddr[HI_MAC_LEN];   /* the MAC address used by the bdg*/
    hi_ushort16 us_pri;           /* the priority of the bridge*/
    hi_uchar8   uc_designatedroot[HI_OMCI_DESIGNATED_ROOT_LEN];  /*the bridge identifier for the root of the spanning tree*/
    hi_uint32   ui_rootpathcost;  /* the cost of the best path to the root as seen from the bridge*/
    hi_uchar8   uc_bdgportcount;  /* the number of existing ports controlled by this bridge*/
    hi_ushort16 us_rootportnum;   /* the port number that has the lowest cost from the bridge to the root bridge*/  
    hi_ushort16 us_hellotime;     /* the time interval (in 256ths of a second) between hello packets*/
    hi_ushort16 us_forwarddelay;  /* the time (in 256ths of a second) that the bridge on 
                                     the Ethernet card in the ONT retains a packet before forwarding it*/    
}hi_omci_me_mac_cfg_data_s;

/*************MAC bridge port configuration data  HI_OMCI_ME_MAC_PORT_CFG_DATA_E************/

#define HI_OMCI_DIRECTION_UPSTREAM            0
#define HI_OMCI_DIRECTION_DOWNSTREAM       1

/* TPType definition in MAC Bridge Port Configuration Data*/
typedef enum 
{
    HI_MAC_BRI_PORT_UNI_E        = 0x01, /*Physical path termination point Ethernet UNI   */ 
    HI_MAC_BRI_PORT_ATMTP_E      = 0x02, /*Interworking VCC termination point             */ 
    HI_MAC_BRI_PORT_8021P_MAP_E  = 0x03, /*802.1p mapper service profile                  */ 
    HI_MAC_BRI_PORT_IP_E         = 0x04, /*IP host config data                            */ 
    HI_MAC_BRI_PORT_IWTP_E       = 0x05, /*GEM interworking termination point             */ 
    HI_MAC_BRI_PORT_MIWTP_E      = 0x06, /*Multicast GEM interworking termination point   */ 
    HI_MAC_BRI_PORT_PPTP_xDSL_E  = 0x07, /*Physical path termination point xDSL UNI part 1*/ 
    HI_MAC_BRI_PORT_PPTP_vDSL_E  = 0x08, /*Physical path termination point VDSL UNI       */ 
    HI_MAC_BRI_PORT_ETHTP_E      = 0x09, /*Ethernet flow termination point                */ 
    HI_MAC_BRI_PORT_PPTP_80211_E = 0x0A, /*Physical path termination point 802.11 UNI     */ 
                                                                                        
    HI_MAC_BRI_PORT_MAX_E        = 0xFF,
}hi_omci_mac_bri_port_type_e;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_bdgid_ptr;                    /* identifies the MAC bridge controlling the port*/
    hi_uchar8   uc_portnum;                    /*This attribute is the bridge port number. It must be unique.*/
    hi_uchar8   uc_tptype;                     /* identifies the type of the TP associated with this MAC bridge port*/
    hi_ushort16 us_tp_ptr;                     /* points to the tp associated with this MAC bdg port*/
    hi_ushort16 uc_pri;                        /* the priority of the port*/
    hi_ushort16 us_pathcost;                   /* the cost contribution of the port to the path cost towards the spanning tree root bridge*/
    hi_uchar8   uc_spanind;                    /* whether or not STP LAN topology change detection is enabled at this port*/
    hi_uchar8   uc_deprecated1;                /* Deprecated */                            
    hi_uchar8   uc_deprecated2;                /* Deprecated */  
    hi_uchar8   uc_portmac[HI_MAC_LEN];   /* Port MAC Address*/
    hi_ushort16 us_out_ptr;                    /* This attribute points to a GEM traffic descriptor that limits the traffic rate leaving the MAC bridge */
    hi_ushort16 us_in_ptr;                     /* This attribute points to a GEM traffic descriptor that limits the traffic rate entering the MAC bridge */
    hi_uchar8   uc_macdepth;                   /*This attribute specifies the maximum number of MAC addresses to be learned by this MAC bridge port*/
}hi_omci_me_mac_port_cfg_s;


typedef enum
{
    HI_OMCI_ONT_MAPPING_MODE_MIN_E= 0,
    HI_OMCI_ONT_MAPPING_MODE_VLAN_E = 1,
    HI_OMCI_ONT_MAPPING_MODE_PRIORITY_E,
    HI_OMCI_ONT_MAPPING_MODE_VLAN_AND_PRIORITY_E,
    HI_OMCI_ONT_MAPPING_MODE_PORT_E,
    HI_OMCI_ONT_MAPPING_MODE_PORT_AND_VLAN_E,
    HI_OMCI_ONT_MAPPING_MODE_PORT_AND_PRIORITY_E,
    HI_OMCI_ONT_MAPPING_MODE_PORT_AND_VLAN_AND_PRIORITY_E,
    HI_OMCI_ONT_MAPPING_MODE_E1_T1_E,
    HI_OMCI_ONT_MAPPING_MODE_IPTOS_E, /*即DSCP映射方式*/
    HI_OMCI_ONT_MAPPING_MODE_AUTO_ADAPTER_E = 0xdb, /*当映射方式为0xdb时，表明当前为自适应方式*/
    HI_OMCI_ONT_MAPPING_MODE_MAX_E = 255
}hi_omci_ont_mapping_mode_e;


/***********MAC bridge performance monitoring history data HI_OMCI_ME_MAC_PM_E**********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;              /* Interval End Time */
    hi_ushort16 us_thresid;              /* Threshold DataB-PON id */
    hi_uint32   ui_discardcnt;          /* BridgeLearningEntry DiscardCount */
}hi_omci_me_macpm_s;

/**********MAC bridge port performance monitoring history data  HI_OMCI_ME_MAC_PORT_PM_E********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_interendtime;      /* Interval End Time */
    hi_ushort16 us_thresholdid;       /* Threshold DataB-PON id */
    hi_uint32   ui_forwardcnt;        /* ForwardedFrame Counter */
    hi_uint32   ui_delaydiscardcnt;   /* DelayExceededDiscardCounter */
    hi_uint32   ui_mtudiscardcnt;     /* MTUExceededDiscardCounter */
    hi_uint32   ui_rcvcnt;            /* ReceivedFrame Counter */
    hi_uint32   ui_discardcnt;        /* ReceivedAndDiscardedCounter */
}__attribute__ ((packed)) hi_omci_me_macportpm_s ;

/**************MAC bridge service profile HI_OMCI_ME_MAC_SERVICE_E*****************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_spantreeind;     /* Spanning tree ind*/
    hi_uchar8   uc_learnind;        /* Learning ind*/
    hi_uchar8   uc_portbridging;    /* Port bridging ind*/
    hi_ushort16 us_pri;             /* Priority*/
    hi_ushort16 us_maxage;          /* Max age*/
    hi_ushort16 us_hellotime;       /* Hello time*/
    hi_ushort16 us_forwarddelay;    /* Forward delay*/
    hi_uchar8   uc_unknownmac;      /* Unknown MAC address discard, treament of unknown D-MAC,TRUE-discard, False-Foward*/
    hi_uchar8   uc_learndepth;      /* the mac learning depth*/
    hi_uint32   ui_dynfilter;       /* Dynamic filtering ageing time*/
}hi_omci_me_mac_service_s;


/*****************MAC bridge port filter preassign table HI_OMCI_ME_MAC_PORT_FILT_PRE_E***************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_ipv4multi; /*IPv4 multicast filtering*/
    hi_uchar8 uc_ipv6multi; /*IPv6 multicast filtering*/
    hi_uchar8 uc_ipv4broad; /*IPv4 broadcast filtering*/
    hi_uchar8 uc_rarp;      /*RARP filtering*/
    hi_uchar8 uc_ipx;       /*IPX filtering*/
    hi_uchar8 uc_netbeui;   /*NetBEUI filtering*/
    hi_uchar8 uc_appletalk; /*AppleTalk filtering*/
    hi_uchar8 uc_bridgeinfo;/*Bridge management information filtering*/
    hi_uchar8 uc_arp;       /*ARP filtering*/
    hi_uchar8 uc_pppoe;     /*PPPoE broadcast filtering*/
}hi_omci_me_mac_port_filt_prea_s;

/*************MAC bridge port bridge design HI_OMCI_ME_MAC_PORT_DESIGN_E**************/
typedef struct
{
    hi_omci_me_msg_head_s   st_msghead;
    hi_uchar8 uc_designroot[24];
    hi_uchar8 uc_portstate;
}hi_omci_me_mac_port_design_s;

/*************MAC bridge port bridge table data HI_OMCI_ME_MAC_PORT_TAB_DATA_E**************/
typedef struct
{
    hi_ushort16 us_info;   /*Information (2 bytes)*/
    hi_uchar8   uc_macaddr[6];/*MAC address (6 bytes)*/
}omci_me_bridge_table_s;

typedef struct
{
    hi_ushort16 us_info;   /*Information (2 bytes)*/
    hi_ushort16 us_vlan;
    hi_uchar8   uc_macaddr[6];/*MAC address (6 bytes)*/
}omci_me_ext_bridge_table_s;

typedef struct
{
    hi_omci_me_msg_head_s      st_msghead;
    omci_me_bridge_table_s     st_bridge;
    hi_ushort16                us_vlanindex;
    omci_me_ext_bridge_table_s st_extbridge;
}hi_omci_me_mac_port_table_s;

/******************MAC bridge port filter table data HI_OMCI_ME_MAC_PORT_FILT_DATA_E**********************/  
#define HI_OMCI_ME_MAC_PORT_FILT_TBL_SIZE      255 
/* 一次Set动作最多可以设置的表项数量 */
#define HI_OMCI_ME_MAC_PORT_FILT_TBL_SET_MAX_ENTRY     3

typedef struct 
{
    hi_uchar8   uc_entrynum;     /* an index in this attribute list */
    hi_uchar8   uc_filter;       /* Filter byte */
    hi_uchar8   uc_macadd[HI_MAC_LEN];   /* MAC address */
}hi_omci_me_filt_entry_s;

typedef struct 
{
    hi_omci_me_msg_head_s    st_msghead;
    hi_omci_me_filt_entry_s  st_filtertbl;
}hi_omci_me_mac_port_filt_s;

/*********************Multicast operations profile HI_OMCI_ME_MULTI_OPER_PROFILE_E********************/  
#define HI_OMCI_IGMP_CTRL_TAB            1024 

typedef enum 
{
    HI_IGMP_SNOOP_ONLY                    = 0,
    HI_IGMP_SNOOP_WITH_PROXY       = 1, 
    HI_IGMP_PROXY                              = 2,
    HI_IGMP_ENABLE_CTRL                   = 5,
}HI_IGMP_PROXY_MODE_E;

typedef struct
{
    hi_ushort16 us_tableindex;/*Table index*/
	hi_ushort16 us_gemport;   /*GEM port-ID*/
	hi_ushort16 us_vlan;      /*VLAN ID*/
	hi_uint32   ui_sip;       /*Source IP address*/
	hi_uint32   ui_dip_start; /*Destination IP address of the start of the multicast range*/
	hi_uint32   ui_dip_end;   /*Destination IP address of the end of the multicast range*/
	hi_uint32   ui_imputedbandwidth;/*Imputed group bandwidth. Expressed in bytes per second*/
	hi_ushort16 us_resv;      /*Reserved*/
}hi_omci_ctrl_table_s;

typedef struct
{
    hi_ushort16 us_vlan;     /*VLAN ID*/
    hi_uint32   ui_sip;      /*Source IP address*/
    hi_uint32   ui_multi_dip;/*Multicast destination IP address*/
}hi_omci_lost_grp_s;
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_version ;               /* the version of IGMP to be supported */
    hi_uchar8   uc_function;               /* IGMP function*/
    hi_uchar8   uc_imleave;                /* whether or not enables the immediate leave function */
    hi_ushort16 us_uptci;                  /* Upstream IGMP TCI */
    hi_uchar8   uc_uptagctrl;              /* Upstream IGMP tag control */
    hi_uint32   ui_rate;                   /* maximum rate of upstream IGMP traffic */
    hi_omci_ctrl_table_s   st_dynctrl;     /* dynamic multicast group address ranges */
    hi_omci_ctrl_table_s   st_stactrl;     /* static multicast group address ranges */
    hi_omci_lost_grp_s     st_lostgrp;     /* a list of groups from the dynamic access control list table */
    hi_uchar8   uc_robustness;             /* possible packet loss in the network */
    hi_uint32   ui_ipaddress;              /* IP address to be used by a proxy querier */
    hi_uint32   ui_queryintvl;             /* interval between general queries in seconds */ 
    hi_uint32   ui_maxrsptime;             /* max response time added by the proxy into general query messages directed to UNIs */ 
    hi_uint32   ui_lastqueryintvl;         /* max response time inserted into group-specific queries sent to UNIs in response to group leave messages */
    hi_uchar8   uc_unauthbehavi;           /*Unauthorized join request behaviour*/
}hi_omci_me_multi_oper_profile_s;

/************************VLAN Tagging Filter Data HI_OMCI_ME_VLAN_FILT_E***************************/   
/*vlan Tagging Filter Table */
#define HI_OMCI_VLAN_FILTER_TAB            12 

/* get the vlan ID and PRI from TCI */
#define HI_OMCI_GET_VLAN_FROM_TCI(us_TCI)   ((hi_ushort16)(us_TCI) & 0xFFF)
#define HI_OMCI_GET_PRI_FROM_TCI(us_TCI)   ((hi_ushort16)(us_TCI >> 13 ) & 0x7)

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_vlantab[HI_OMCI_VLAN_FILTER_TAB];
    hi_uchar8   uc_forward_oper;      
    hi_uchar8   uc_entitynum;       /*the number of entries in the VLAN Filter 
                                      Table that are valid*/
}hi_omci_me_vlan_filt_s;

/*********VLAN Tagging Operation Configuration Data  HI_OMCI_ME_VLAN_TAG_E************/  
/* OMCI default TCI, vlan is 1, priority is 1 */
#define HI_OMCI_DEFAULT_TCI                0x0001
    
/* OMCI default VLAN, vlan is 1 */
#define HI_OMCI_DEFAULT_VLAN               0x1

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_usopermode;
    hi_ushort16 us_ustci;
    hi_uchar8   uc_dsopermode; 
    hi_uchar8   uc_assoctype;
    hi_ushort16 us_assocme_ptr;
}hi_omci_me_vlantag_op_s;

/* The default Upstream VLAN tagging operation mode*/
#define HI_OMCI_UPSTREAM_VLAN_TRANSPRANT_MODE       0x00
#define HI_OMCI_UPSTREAM_VLAN_RETAG_MODE                 0x01
#define HI_OMCI_UPSTREAM_VLAN_ADD_TAG_MODE             0x02
#define HI_OMCI_DOWNSTREAM_VLAN_TRANSPRANT_MODE 0x00
#define HI_OMCI_DOWNSTREAM_VLAN_REMOVETAG_MODE   0x01

typedef enum 
{
    HI_OMCI_VLAN_TAG_OPER_TYPE_DEFAULT_E = 0    ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_IP_HOST_E        ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_8021P_E          ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_MAC_BR_PORT_E    ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_PPTP_xDSL_UNI_E  ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_GEM_IWTP_E       ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_MULTI_GEM_IWTP_E ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_PPTP_MoCA_UNI_E  ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_PPTP_80211_UNI_E ,   
    HI_OMCI_VLAN_TAG_OPER_TYPE_ETH_FLOW_TP_E    ,       
    HI_OMCI_VLAN_TAG_OPER_TYPE_PPTP_ETH_UNI_E   ,       

    HI_OMCI_VLAN_TAG_OPER_TYPE_MAX_E    = 255   ,
}EN_OMCI_ME_VLAN_TAG;

/****************Extended VLAN tagging operation configuration data  HI_OMCI_ME_EXTEND_VLAN_TAG_E**************/
/*Filter Ethertype*/
#define HI_OMCI_ETHERTYPE_NOFILTER            0     /*Do not filter on Ethertype.*/
#define HI_OMCI_ETHERTYPE_FILTER_IPoE         1     /*Ethertype = 0x0800 (filter IPoE frames)*/
#define HI_OMCI_ETHERTYPE_FILTER_PPPoE        2     /*Ethertype = 0x8863 or 0x8864 (filter PPPoE frames)*/
#define HI_OMCI_ETHERTYPE_FILTER_ARP          3     /*Ethertype = 0x0806 (filter ARP frames)*/
#define HI_OMCI_ETHERTYPE_FILTER_USERDEFINED  15    /*Ethertype = 0x0806 (filter ARP frames)*/


/* Treatment tags to remove field */
#define HI_OMCI_TREAT_REMOVE_0_TAG             0
#define HI_OMCI_TREAT_REMOVE_1_TAG             1
#define HI_OMCI_TREAT_REMOVE_2_TAG             2
#define HI_OMCI_TREAT_REMOVE_RESERVED          3

#define HI_OMCI_FILETER_PRIORITY_NO            8    /* not filter */
#define HI_OMCI_FILTER_OUTER_NO_TAG            15   /*没有OUTER TAG */
#define HI_OMCI_FILTER_INNER_NO_TAG            15   /*没有INNER TAG*/
#define HI_OMCI_FILTER_PRIORITY_DEFAULT_TAG    14 	/* Default Priority Tag rule */

#define HI_OMCI_TREATMENT_OUTER_TAG_NO_ADD     15  	/*不添加OUTER TAG*/
#define HI_OMCI_TREATMENT_INNER_TAG_NO_ADD     15 	/*不添加INNER TAG */
#define HI_OMCI_TREATMENT_INNER_TAG_ADD_COPY   8 	/* add inner tag */
#define HI_OMCI_FILETER_NO                     8    /* not filter */

/* 无效的vlan */
#define HI_OMCI_INVALID_VLAN                   4096

#define HI_OMCI_EXTEND_VLAN_TAG_ENTRY_SIZE     16
#define HI_OMCI_EXTEND_HWVLAN_TAG_ENTRY_SIZE   18
#define HI_OMCI_EXTEND_VLAN_TAG_TABLE          16
#define HI_OMCI_DOWNSTREAM_MODE_TRANSPARENT    1
#define HI_OMCI_EXVLAN_8_BYTE                  8
#define HI_OMCI_HWEXVLAN_LAST_2_BYTE           2

typedef enum                                         
{                                                    
    HI_OMCI_ASSOCIATE_TYPE_MAC_BRG_PORT_CFG_DATA = 0, //MAC bridge port configuration data            
    HI_OMCI_ASSOCIATE_TYPE_8021P_MAPPER_SRV_PROFILE,  //802.1p mapper service profile                 
    HI_OMCI_ASSOCIATE_TYPE_PPTP_ETH_UNI,              //Physical path termination point Ethernet UNI  
    HI_OMCI_ASSOCIATE_TYPE_IP_HOST_CFG_DATA,          //IP host config data                           
    HI_OMCI_ASSOCIATE_TYPE_PPTP_XDSL_UNI,             //Physical path termination point xDSL UNI      
    HI_OMCI_ASSOCIATE_TYPE_GEM_IWTP,                  //GEM interworking termination point            
    HI_OMCI_ASSOCIATE_TYPE_MULTI_GEM_IWTP,            //Multicast GEM interworking termination point  
    HI_OMCI_ASSOCIATE_TYPE_PPTP_MOCA_UNI,             //Physical path termination point MoCA UNI      
    HI_OMCI_ASSOCIATE_TYPE_PPTP_80211_UNI,            //Physical path termination point 802.11 UNI    
    HI_OMCI_ASSOCIATE_TYPE_ETH_FLOW_TERMIN_POINT = 9  //Ethernet flow termination point               
}HI_OMCI_ME_ASSOCIATE_TYPE_E;     

/* Extended VLAN action */
typedef enum
{
    HI_OMCI_EXVLAN_ACTION_UNTAG_ADD1    =1      ,    //1 Untagged frames:Insert 1 full tag (X):
    HI_OMCI_EXVLAN_ACTION_UNTAG_DEFAULT         ,    //2 Default case, do nothing
    HI_OMCI_EXVLAN_ACTION_UNTAG_ADD2            ,    //3 Insert 2 tags (X,Y):
    HI_OMCI_EXVLAN_ACTION_1TAG_ADD1             ,    //4 Single tagged frames: Insert 1 full tag (X):
    HI_OMCI_EXVLAN_ACTION_1TAG_ADD1_COPY_PRI    ,    //5 Insert 1 tag (X),copy priority:
    HI_OMCI_EXVLAN_ACTION_1TAG_ADD2             ,    //6 Insert 2 tags (X,Y):
    HI_OMCI_EXVLAN_ACTION_1TAG_MOD_VALN_PRI     ,    //7 Modify tag:
    HI_OMCI_EXVLAN_ACTION_1TAG_MOD_VLAN         ,    //8 Modify tag,keep original priority:
    HI_OMCI_EXVLAN_ACTION_1TAG_MOD_ADD_TAG      ,    //9 Modify and insert tag:
    HI_OMCI_EXVLAN_ACTION_1TAG_DEL_TAG          ,    //10 Remove tag:
    HI_OMCI_EXVLAN_ACTION_1TAG_DEFAULT          ,    //11 Default case, do nothing
    HI_OMCI_EXVLAN_ACTION_1TAG_ADD_2            ,    //12 Insert two tags:
    HI_OMCI_EXVLAN_ACTION_2TAG_ADD1             ,    //13 Double tagged frames:Insert 1 tag (X):
    HI_OMCI_EXVLAN_ACTION_2TAG_ADD1_COPY_PRI    ,    //14 Insert 1 tag (X),copy external priority:
    HI_OMCI_EXVLAN_ACTION_2TAG_ADD2             ,    //15 Insert 2 tags (X,Y):
    HI_OMCI_EXVLAN_ACTION_2TAG_ADD2_COPY_PRI    ,    //16 Insert 2 tags (X,Y),copy external and internal priority:
    HI_OMCI_EXVLAN_ACTION_2TAG_MOD_OUT_VLAN_PRI ,    //17 Modify external tag:
    HI_OMCI_EXVLAN_ACTION_2TAG_MOD_OUT_VLAN     ,    //18 Modify external tag, keep original priority:
    HI_OMCI_EXVLAN_ACTION_2TAG_MOD_2_VLAN_PIR   ,    //19 Modify both tags:
    HI_OMCI_EXVLAN_ACTION_2TAG_MOD_2_VLAN       ,    //20 Modify both tags, keep original priorities:
    HI_OMCI_EXVLAN_ACTION_2TAG_SWAP_2_TAG       ,    //21 Swap both tags:
    HI_OMCI_EXVLAN_ACTION_2TAG_DEL_OUT_TAG      ,    //22 Remove outer tag:
    HI_OMCI_EXVLAN_ACTION_2TAG_DEL_2_TAG        ,    //23 Remove both tags:
    HI_OMCI_EXVLAN_ACTION_2TAG_DEAULT           ,    //24 Default case, do nothing

    HI_OMCI_EXVLAN_ACTION_INVALID
}hi_omci_me_exvlan_action_type_e;

/*Filter field*/                                                                                                                                                   
#define HI_OMCI_EXVLAN_GET_FILTER_OUT_PRI(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_outer_pri   )
#define HI_OMCI_EXVLAN_GET_FILTER_OUT_VID(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_outer_vid   )
#define HI_OMCI_EXVLAN_GET_FILTER_OUT_TPID(pucUpVlanTbl) (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_outer_tpid  )
#define HI_OMCI_EXVLAN_GET_FILTER_INN_PRI(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_in_pri   )
#define HI_OMCI_EXVLAN_GET_FILTER_INN_VID(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_in_vid   )
#define HI_OMCI_EXVLAN_GET_FILTER_INN_TPID(pucUpVlanTbl) (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_in_tpid  )
#define HI_OMCI_EXVLAN_GET_FILTER_ETHTYPE(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_filt_ethertype  )
                                                                                                                                                                   
#define HI_OMCI_EXVLAN_GET_TREAT_TAG_REMOVE(pucUpVlanTbl)      (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_tag_remove     )
#define HI_OMCI_EXVLAN_GET_TREAT_OUT_PRI(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_outer_pri )
#define HI_OMCI_EXVLAN_GET_TREAT_OUT_VID(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_outer_vid )
#define HI_OMCI_EXVLAN_GET_TREAT_OUT_TPID(pucUpVlanTbl) (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_outer_tpid)
#define HI_OMCI_EXVLAN_GET_TREAT_INN_PRI(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_inner_pri )
#define HI_OMCI_EXVLAN_GET_TREAT_INN_VID(pucUpVlanTbl)  (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_inner_vid )
#define HI_OMCI_EXVLAN_GET_TREAT_INN_TPID(pucUpVlanTbl) (((omci_extvlantag_s*)pucUpVlanTbl)->ui_treat_inner_tpid)   

typedef struct
{
    hi_uint32    ui_reserved1:12     	;  //Padding
    hi_uint32    ui_filt_outer_tpid:3   ;  //Filter outer TPID/DE
    hi_uint32    ui_filt_outer_vid:13   ;  //Filter outer VID
    hi_uint32    ui_filt_outer_pri:4    ;  //Filter outer priority

    hi_uint32    ui_filt_ethertype:4    ;  //Filter Ethertype
    hi_uint32    ui_reserved2:8     	;  //Padding
    hi_uint32    ui_filt_in_tpid:3     	;  //Filter inner TPID/DE
    hi_uint32    ui_filt_in_vid:13     	;  //Filter inner VID    
    hi_uint32    ui_filt_in_pri:4      	;  //Filter inner priority

    hi_uint32    ui_treat_outer_tpid:3  ;  //Treatment outer TPID/DE 
    hi_uint32    ui_treat_outer_vid:13  ;  //Treatment outer VID
    hi_uint32    ui_treat_outer_pri:4   ;  //Treatment outer priority
    hi_uint32    ui_reserved3:10    	;  //Padding
    hi_uint32    ui_treat_tag_remove:2  ;  //Treatment tags to remove

    hi_uint32    ui_treat_inner_tpid:3  ;  //Filter inner TPID/DE
    hi_uint32    ui_treat_inner_vid:13  ;  //Filter inner VID
    hi_uint32    ui_treat_inner_pri:4   ;  //Treatment inner priority
    hi_uint32    ui_reserved4:12    	;  //Padding      
}omci_extvlantag_s;

typedef struct
{
    hi_uint32    ui_reserved1:12        ;  //Padding
    hi_uint32    ui_filt_outer_tpid:3   ;  //Filter outer TPID/DE
    hi_uint32    ui_filt_outer_vid:13   ;  //Filter outer VID
    hi_uint32    ui_filt_outer_pri:4    ;  //Filter outer priority

    hi_uint32    ui_filt_ethertype:4    ;  //Filter Ethertype
    hi_uint32    ui_reserved2:8         ;  //Padding
    hi_uint32    ui_filt_in_tpid:3      ;  //Filter inner TPID/DE
    hi_uint32    ui_filt_in_vid:13      ;  //Filter inner VID    
    hi_uint32    ui_filt_in_pri:4       ;  //Filter inner priority

    hi_uint32    ui_treat_outer_tpid:3  ;  //Treatment outer TPID/DE 
    hi_uint32    ui_treat_outer_vid:13  ;  //Treatment outer VID
    hi_uint32    ui_treat_outer_pri:4   ;  //Treatment outer priority
    hi_uint32    ui_reserved3:10        ;  //Padding
    hi_uint32    ui_treat_tag_remove:2  ;  //Treatment tags to remove

    hi_uint32    ui_treat_inner_tpid:3  ;  //Filter inner TPID/DE
    hi_uint32    ui_treat_inner_vid:13  ;  //Filter inner VID
    hi_uint32    ui_treat_inner_pri:4   ;  //Treatment inner priority
    hi_uint32    ui_reserved4:12        ;  //Padding

    hi_ushort16  us_ethtype             ;  //User-defined Ethtype vlaue
}omci_hwextvlantag_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8    uc_type;
    hi_ushort16  us_rcvvlanmax;
    hi_ushort16  us_intpid;
    hi_ushort16  us_outtpid;
    hi_uchar8    uc_dnmode;
    omci_extvlantag_s  st_vlantagtable;
    hi_ushort16  us_assocme_ptr;
    hi_uchar8    uc_dscp2pbit[24];       /*DSCP to P bit mapping*/
}hi_omci_me_extvlantag_op_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16  us_rcvvlanmax;
    omci_hwextvlantag_s  st_hwvlantagtable;
}hi_omci_me_hwextvlantag_op_s;

/********************Multicast subscriber config info***************************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8    uc_metype;             /*indicates the type of the ME implicitly linked*/
    hi_ushort16  us_profile_ptr;        /*points to an instance of the multicast operations profil*/
    hi_ushort16  us_maxgrp;             /*maximum number of dynamic multicast groups*/
    hi_uint32    ui_maxbw;              /*Max multicast bandwidth*/
    hi_uchar8    uc_bwenfor;            /*  Bandwidth enforcement*/
}hi_omci_me_multisubs_cfg_s;

/**********Dot1X port extension package HI_OMCI_ME_DOT1X_PORT_EXT_PACKAGE_E************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_dot1x;         /*Dot1x enable*/
    hi_uchar8   uc_actreg;        /*Action register*/
    hi_uchar8   uc_authenpae;     /*Authenticator PAE state*/
    hi_uchar8   uc_backendauthen; /*Backend authentication state*/
    hi_uchar8   uc_adminctrl;     /*Admin controlled directions*/
    hi_uchar8   uc_operctrl;      /*Operational controlled directions*/
    hi_uchar8   uc_authenctrl;    /*Authenticator controlled port status*/
    hi_ushort16 us_quiet;         /*Quiet period*/
    hi_ushort16 us_servertimeout; /*Server timeout period*/
    hi_ushort16 us_reauthen;      /*Re-authentication period*/
    hi_uchar8   uc_reauthenenable; /*Re-authentication enabled*/
    hi_uchar8   uc_keyenable;     /*Key transmission enabled*/
}hi_omci_me_dot1xport_s;

/*******Dot1X configuration profile HI_OMCI_ME_DOT1X_CFG_PROFILE_E*******/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_circuitid;        /*Circuit ID prefix*/
    hi_uchar8   uc_fallback;         /*Fallback policy*/
    hi_ushort16 us_authserver1;      /*Auth server 1*/
    hi_uchar8   uc_sharedsecret1[25];/*Shared secret auth1*/
    hi_ushort16 us_authserver2;      /*Auth server 2*/
    hi_uchar8   uc_sharedsecret2[25];/*Shared secret auth2*/
    hi_ushort16 us_authserver3;      /*Auth server 3*/
    hi_uchar8   uc_sharedsecret3[25];/*Shared secret auth3*/
    hi_uint32   ui_oltproxy;         /*OLT proxy address*/
}hi_omci_me_dot1xprofile_s;

/**********Dot1X performance monitoring history data HI_OMCI_ME_DOT1X_PMH_DATA_E**********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_endtime;        /*Interval end time*/
    hi_ushort16 us_thresid;      /*Threshold data 1/2 id*/
    hi_uint32 ui_eapolrcv;       /*EAPOL frames received*/
    hi_uint32 ui_eapoltrans;     /*EAPOL frames transmitted*/
    hi_uint32 ui_eapolstartrcv;  /*EAPOL start frames received*/
    hi_uint32 ui_eapollogoffrcv; /*EAPOL logoff frames received*/
    hi_uint32 ui_invalideapolrcv;/*Invalid EAPOL frames received*/
    hi_uint32 ui_eapresprcv;     /*EAP resp/id frames received*/
    hi_uint32 ui_eaprsprcv;      /*EAP response frames received*/
    hi_uint32 ui_eapinittrans;   /*EAP initial request frames transmitted*/
    hi_uint32 ui_eapreqtrans;    /*EAP request frames transmitted*/
    hi_uint32 ui_eaperrrcv;      /*EAP length error frames received*/
    hi_uint32 ui_eapsucc;        /*EAP success frames generated autonomously*/
    hi_uint32 ui_eapfail;        /*EAP failure frames generated autonomously*/
   
}hi_omci_me_dot1xpm_s;

/******************Radius performance monitoring history data HI_OMCI_ME_RADIUS_PMH_DATA_E*******************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_endtime;
    hi_ushort16 us_thresid;
    hi_uint32 ui_accreqstrans; /*Access-request packets transmitted*/
    hi_uint32 ui_accreqretrans;/*Access-request retransmission count*/
    hi_uint32 ui_accchallrcv;  /*Access-challenge packets received*/
    hi_uint32 ui_accacceptrcv; /*Access-accept packets received*/
    hi_uint32 ui_accrejectrcv; /*Access-reject packets received*/
    hi_uint32 ui_invalidrcv;   /*Invalid radius packets received*/
}hi_omci_me_radiuspm_s;

/***********Dot1 rate limiter  HI_OMCI_ME_DOT1_RATE_LIMITER_E***************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_parentme_ptr;//Parent ME pointer
    hi_uchar8   uc_tptype;  //TP type
    hi_ushort16 us_upucrate_ptr;//Upstream unicast flood rate pointer
    hi_ushort16 us_upbcrate_ptr;//Upstream broadcast rate pointer
    hi_ushort16 us_upmcrate_ptr;//Upstream multicast payload rate pointer
}hi_omci_me_dot1rate_s;

/************Dot1ag maintenance domain HI_OMCI_ME_DOT1AG_MAINT_DOMAIN_E**************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_mdlevel;     
    hi_uchar8 uc_mdnamefmt;  
    hi_uchar8 uc_mdname1[25];  
    hi_uchar8 uc_mdname2[25]; 
    hi_uchar8 uc_mhfcreat; 
    hi_uchar8 uc_senderid;
}hi_omci_me_dot1agdomain_s;

/************Dot1ag maintenance association  HI_OMCI_ME_DOT1AG_MAINT_ASSOCIATION_E*************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_md_ptr;
    hi_uchar8   uc_manamefmt;
    hi_uchar8   uc_maname1[25];
    hi_uchar8   uc_maname2[25];
    hi_uchar8   uc_ccminterval;
    hi_ushort16 us_vlan[12];
    hi_uchar8   uc_mhfcreat;
    hi_uchar8   uc_senderid;
}hi_omci_me_dot1agasso_s;

/*************Dot1ag default MD level  HI_OMCI_ME_DOT1AG_DEF_MD_LV_E**************/
typedef struct
{
    hi_ushort16 us_primaryvlan;/*Primary VLAN ID*/
    hi_uchar8   uc_tablectrl;  /*Table control*/
    hi_uchar8   uc_status;     /*Status*/
    hi_uchar8   uc_level;      /*Level*/
    hi_uchar8   uc_mhfcreat;   /*MHF creation*/
    hi_uchar8   uc_senderid;   /*Sender ID permission*/
    hi_ushort16 us_vlanlist[11];/*Associated VLANs list*/
}omci_defaultmdlevel_s;

#define MD_LEVEL_NUM 10

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_layer2type;    /*Layer 2 type*/
    hi_uchar8 uc_catlevel;      /*Catchall level*/
    hi_uchar8 uc_catmhfcreation;/*Catchall MHF creation*/
    hi_uchar8 uc_catsenderid;   /*Catchall sender ID permission*/
    omci_defaultmdlevel_s st_mdlevel[MD_LEVEL_NUM];/*Default MD level table*/
}hi_omci_me_dot1agmd_level_s;

/****************Dot1ag MEP  HI_OMCI_ME_DOT1AG_MEP_E***************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_entity_ptr; /*Layer 2 entity pointer*/
    hi_uchar8   uc_type;          /*Layer 2 type*/
    hi_ushort16 us_ma_ptr;     /*MA pointer*/
    hi_ushort16 us_mepid;         /*MEP ID*/
    hi_uchar8   uc_mepctrl;       /*MEP control*/
    hi_ushort16 us_primvlan;      /*Primary VLAN*/
    hi_uchar8   uc_adminstate;    /*Administrative state*/
    hi_uchar8   uc_priority;      /*CCM and LTM priority*/
    hi_uchar8   uc_egrid[8];      /*Egress identifier*/
    hi_ushort16 us_peermepid[12]; /*Peer MEP IDs*/
    hi_uchar8   uc_aiscontrol;    /*ETH AIS control*/
    hi_uchar8   uc_faultalarm;    /*Fault alarm threshold*/
    hi_ushort16 us_declartime;    /*Alarm declaration soak time*/
    hi_ushort16 us_cleartime;     /*Alarm clear soak time*/
}hi_omci_me_dot1agmep_s;

/****************Dot1ag MEP status******************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_macaddr[6];         //MEP MAC address
    hi_uchar8 uc_faultstate;         //Fault notification generator state
    hi_uchar8 uc_highpridefect;      //Highest priority defect observed
    hi_uchar8 uc_curdefect;          //Current defects
    hi_uchar8 uc_lasterr[128];       //Last received errored CCM table
    hi_uchar8 uc_lastxcon[128];      //Last received xcon CCM table
    hi_uint32 ui_outofseqcnt;        //Out of sequence CCMs count
    hi_uint32 ui_ccmtrans;           //CCMs transmitted count
    hi_uint32 ui_unexpltr;           //Unexpected LTRs count
    hi_uint32 ui_lbrtrans;           //LBRs transmitted count
    hi_uint32 ui_nextloopback;       //Next loopback transaction identifier
    hi_uint32 ui_nexttrace;          //Next link trace transaction identifier
}hi_omci_me_dot1agmep_status_s;

/**************Dot1ag MEP CCM database**************/
#define SENDER_ID_LEN 8
typedef struct
{
    hi_ushort16 us_rmepid;
    hi_uchar8   uc_rmepstate;
    hi_uint32   ui_failedtime;
    hi_uchar8   uc_macaddr[6];
    hi_uchar8   uc_rdi;
    hi_uchar8   uc_portstatus;
    hi_uchar8   uc_interstatus;
    hi_uchar8   uc_senderid[SENDER_ID_LEN];
}rmep_database_table_s;

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    rmep_database_table_s st_db1;  //RMEP 1 database table
    rmep_database_table_s st_db2;  //RMEP 2 database table
    rmep_database_table_s st_db3;  //RMEP 3 database table
    rmep_database_table_s st_db4;  //RMEP 4 database table
    rmep_database_table_s st_db5;  //RMEP 5 database table
    rmep_database_table_s st_db6;  //RMEP 6 database table
    rmep_database_table_s st_db7;  //RMEP 7 database table
    rmep_database_table_s st_db8;  //RMEP 8 database table
    rmep_database_table_s st_db9;  //RMEP 9 database table
    rmep_database_table_s st_db10; //RMEP 10 database table
    rmep_database_table_s st_db11; //RMEP 11 database table
    rmep_database_table_s st_db12; //RMEP 12 database table
}hi_omci_me_dot1agccm_s;

/*************Dot1ag CFM stack**************/
typedef struct
{
    hi_ushort16 us_portid;
    hi_uchar8   uc_level;
    hi_uchar8   uc_direction;
    hi_ushort16 us_vlanid;
    hi_ushort16 us_md_ptr;
    hi_ushort16 us_ma_ptr;
    hi_ushort16 us_mepid;
    hi_uchar8   uc_macaddr[6];
}mp_status_table_s;

typedef struct
{
    hi_ushort16 us_vlanid;
    hi_ushort16 us_portid;
    hi_uchar8   uc_detectederr;
}cfg_err_list_table_s;


typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_layer2type;
    mp_status_table_s  st_mpstatus[1];   //MP status table
    cfg_err_list_table_s st_configerr[1];//Configuration error list table
}hi_omci_me_dot1agcfm_s;

/***********Dot1ag chassis-management info**************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_chassidlen;          //Chassis ID length
    hi_uchar8 uc_chassidsubtype;      //Chassis ID subtype
    hi_uchar8 uc_chassidpart1[25];    //Chassis ID part 1
    hi_uchar8 uc_chassidpart2[25];    //Chassis ID part 2
    hi_uchar8 uc_domainlen;           //Management address domain length
    hi_uchar8 uc_domain1[25];         //Management address domain 1
    hi_uchar8 uc_domain2[25];         //Management address domain 2
    hi_uchar8 uc_mngaddrlen;          //Management address length
    hi_uchar8 uc_mngaddr1[25];        //Management address 1
    hi_uchar8 uc_mngaddr2[25];        //Management address 2
}hi_omci_me_dot1agchassis_s;

/***********Multicast subscriber monitor************/
typedef struct
{
    hi_ushort16 us_vlanid;   //VLAN ID
    hi_uint32 ui_sip;        //Source IP address
    hi_uint32 ui_dip;        //Multicast destination IP address
    hi_uint32 ui_bestbandwidth;//Best efforts actual bandwidth estimate, bytes per second
    hi_uint32 ui_cip;        //Client (set-top box) IP address
    hi_uint32 ui_jointime;   //Time since the most recent join of this client to the IP channel,
    hi_ushort16 us_resv;     //Reserved
}active_grp_list_s;

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_metype;//ME type
    hi_uint32 ui_curbandwidth;//Current multicast bandwidth
    hi_uint32 ui_msgcnt;//Join messages counter
    hi_uint32 ui_bwexceedcnt;//Bandwidth exceeded counter
    active_grp_list_s st_actgrplist;//Active group list table
}hi_omci_me_multisubs_monitor_s;

/**************Ethernet frame performance monitoring history data upstream*****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_endtime;
    hi_ushort16 us_thresid;
    hi_uint32 ui_dropevents;
    hi_uint32 ui_octets;
    hi_uint32 ui_packets;
    hi_uint32 ui_bc;
    hi_uint32 ui_mc;
    hi_uint32 ui_crcerrored;
    hi_uint32 ui_undersize;
    hi_uint32 ui_oversize;
    hi_uint32 ui_pkt64;
    hi_uint32 ui_pkt65to127;
    hi_uint32 ui_pkt128to255;
    hi_uint32 ui_pkt256to511;
    hi_uint32 ui_pkt512to1023;
    hi_uint32 ui_pkt1024to1518;
}hi_omci_me_ethpm_up_s;

/**************Ethernet frame performance monitoring history data downstream*****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_endtime;
    hi_ushort16 us_thresid;
    hi_uint32 ui_dropevents;
    hi_uint32 ui_octets;
    hi_uint32 ui_packets;
    hi_uint32 ui_bc;
    hi_uint32 ui_mc;
    hi_uint32 ui_crcerrored;
    hi_uint32 ui_undersize;
    hi_uint32 ui_oversize;
    hi_uint32 ui_pkt64;
    hi_uint32 ui_pkt65to127;
    hi_uint32 ui_pkt128to255;
    hi_uint32 ui_pkt256to511;
    hi_uint32 ui_pkt512to1023;
    hi_uint32 ui_pkt1024to1518;
}hi_omci_me_ethpm_dn_s;

/*************Ethernet frame extended PM**************/
typedef struct
{
    hi_ushort16 us_thresid;
    hi_ushort16 us_class;
    hi_ushort16 us_inst;
    hi_ushort16 us_accumula_disable;
    hi_ushort16 us_tca_disable;
    hi_ushort16 us_ctlfields;
    hi_ushort16 us_tci;
    hi_ushort16 us_reserved;
}omci_ctrlblock_s;

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_endtime;
    omci_ctrlblock_s st_ctrlblock;
    hi_uint32 ui_dropevents;
    hi_uint32 ui_octets;
    hi_uint32 ui_frames;
    hi_uint32 ui_bc;
    hi_uint32 ui_mc;
    hi_uint32 ui_crcerrored;
    hi_uint32 ui_undersize;
    hi_uint32 ui_oversize;
    hi_uint32 ui_frm64;
    hi_uint32 ui_frm65to127;
    hi_uint32 ui_frm128to255;
    hi_uint32 ui_frm256to511;
    hi_uint32 ui_frm512to1023;
    hi_uint32 ui_frm1024to1518;
}hi_omci_me_eth_ext_pm_s;

typedef struct
{
    hi_ushort16 us_filterinvlan;
    hi_ushort16 us_filteroutvlan;
    hi_uchar8   uc_filterinpri;
    hi_uchar8   uc_filteroutpri;
    hi_ushort16 us_treatinvlan;
    hi_ushort16 us_treatoutvlan;
    hi_uchar8   uc_treatinpri;
    hi_uchar8   uc_treatoutpri;
}hi_omci_vlan_opt_rule_s;


#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __OMCI_ME_L2_DEF_H__ */
