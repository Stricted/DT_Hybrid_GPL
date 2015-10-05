/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : omci_me_equip_def.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2011_09_29

******************************************************************************/
#ifndef __OMCI_ME_EQUIP_DEF_H__
#define __OMCI_ME_EQUIP_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* ----------------cardholder   HI_OMCI_ME_CARD_HOLDER_E--------------- */

#define HI_OMCI_EQUIPMENT_ID_LEN       20 /*Equipment ID length ,protocol*/
#pragma pack(1)
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_acttype;                                      /*actual plug-in unit type*/
    hi_uchar8   uc_exptype;                                      /*expected plug-in unit type*/ 
    hi_uchar8   uc_expportcnt;                                   /*expected Port number*/
    hi_uchar8   uc_expequipid[HI_OMCI_EQUIPMENT_ID_LEN];         /*expected Equipment ID*/
    hi_uchar8   uc_actequipid[HI_OMCI_EQUIPMENT_ID_LEN];         /*actual Equipment ID*/
    hi_uchar8   uc_protect_ptr;                                  /*protection profile pointer*/
    hi_uchar8   uc_protectswitch;                                /*invoke protection switch*/
    hi_uchar8   uc_arc;                                          /*ARC*/
    hi_uchar8   uc_arcintvl;                                     /*ARC interval*/
}hi_omci_me_cardholder_s;

/* ----------------circuit pack   HI_OMCI_ME_CIRCUIT_PACK_E--------------- */

/*Cirucit Pack*/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_type;             /*Type*/
    hi_uchar8   uc_portnum;          /*Number of ports*/
    hi_uchar8   uc_sn[8];            /*Serial number*/
    hi_uchar8   uc_version[14];      /*Version*/
    hi_uint32   ui_vendorid;         /*Vendor id*/
    hi_uchar8   uc_adminstate;       /* Administrative state*/
    hi_uchar8   uc_operstate;        /*Operational state*/
    hi_uchar8   uc_bridge_ip;        /*Bridged or IP ind*/
    hi_uchar8   uc_equipid[20];      /*Equipment id*/
    hi_uchar8   uc_cardcfg;          /* Card configuration*/
    hi_uchar8   uc_tcontnum;         /* total T-CONT buffer number associated with the circuit pack*/
    hi_uchar8   uc_pqnum;            /* total PQ number associated with the circuit pack*/
    hi_uchar8   uc_tsnum;            /* total traffic scheduler number associated with the circuit pack*/
    hi_uint32   ui_powershed;        /* power shed override*/
}hi_omci_me_circuitpack_s;


/* ----------------software image   HI_OMCI_ME_SW_IMAGE_E--------------- */
typedef struct 
{
    hi_uchar8   uc_version[14];
    hi_uchar8   uc_commited;      /*Is committed*/
    hi_uchar8   uc_active;        /*Is active*/
    hi_uchar8   uc_valid;         /*Is valid*/
}hi_omci_me_software_image_s;
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_software_image_s st_image;
}hi_omci_me_software_image_msg_s;

/* ----------------onu-g     HI_OMCI_ME_ONU_G_E--------------- */
typedef struct
{
    hi_uchar8   uc_vendorid[4];       /*Vendor id*/
    hi_uchar8   uc_version[14];       /*Version*/
    hi_uchar8   uc_sn[8];             /*Serial Number*/
    hi_uchar8   uc_trafficflag;       /*Traffic management option*/
    hi_uchar8   uc_deprecated;        /*Deprecated*/
    hi_uchar8   uc_batterybackup;     /*Battery backup*/
    hi_uchar8   uc_adminstate;        /*Administrative state*/
    hi_uchar8   uc_operstate;         /*Operational state*/
	hi_uchar8   uc_onusurvival;       /*ONU survival time*/
}hi_omci_me_onu_g_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_omci_me_onu_g_s st_onug;
}hi_omci_me_onu_g_msg_s;

/* ----------------onu2-g     HI_OMCI_ME_ONU2_G_E--------------- */
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_equipid[20]; /*Equipment id*/
    hi_uchar8   uc_omccversion; /*OMCC version*/
    hi_ushort16 us_vendorcode;  /*Vendor product code*/
    hi_uchar8   uc_secflag;     /*Security capability*/
    hi_uchar8   uc_secmode;     /*Security mode*/
    hi_ushort16 us_pqnum;       /*Total priority queue number*/
    hi_uchar8   uc_trafficnum;  /*Total traffic scheduler number*/
    hi_uchar8   uc_deprecated;  /*Deprecated*/
    hi_ushort16 us_gemportnum;  /*Total GEM port-ID number*/
    hi_uint32   ui_sysuptime;   /*SysUpTime*/
    hi_ushort16 us_concap;      /*Connectivity capability*/
    hi_uchar8   uc_curmode;     /*Current connectivity mode*/  
    hi_ushort16 us_qoscfg;      /*QoS configuration flexibility*/ 
    hi_ushort16 us_pqscale;     /*Priority queue scale factor*/
}hi_omci_me_onu2_g_s;

/**************ONU data    HI_OMCI_ME_ONU_DATA_E*************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_mibsync;           /*MIB data sync*/
}hi_omci_me_onudata_s;

/*-------------ONU power shedding    HI_OMCI_ME_ONU_POWER_SHEDDING_E----------*/

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_timerreset;  /*Restore power timer reset interval*/
    hi_ushort16 us_data;        /*Data class shedding interval*/
    hi_ushort16 us_voice;       /*Voice class shedding interval*/
    hi_ushort16 us_videooverlay;/*Video overlay class shedding interval*/
    hi_ushort16 us_videoreturn; /*Video return class shedding interval*/
    hi_ushort16 us_dsl;         /*DSL class shedding interval*/
    hi_ushort16 us_atm;         /*ATM class shedding interval*/
    hi_ushort16 us_ces;         /*CES class shedding interval*/
    hi_ushort16 us_frame;       /*Frame class shedding interval*/
    hi_ushort16 us_sdhsonet;    /*Sdh-sonet class shedding interval*/
    hi_ushort16 us_status;      /*Shedding status*/
}hi_omci_me_onu_power_s; 

/*--------------Port mapping package   HI_OMCI_ME_PORT_MAP_PACKAGE_E-----------------*/
typedef struct
{
    hi_uchar8   uc_phyport;
    hi_ushort16 us_equiptype[12];
}hi_omci_combinedport_s;

#define HI_OMCI_MAX_COMBINEDPORT_NUM 1

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_maxports;
    hi_ushort16 us_portlist1;
    hi_ushort16 us_portlist2;
    hi_ushort16 us_portlist3;
    hi_ushort16 us_portlist4;
    hi_ushort16 us_portlist5;
    hi_ushort16 us_portlist6;
    hi_ushort16 us_portlist7;
    hi_ushort16 us_portlist8;
    hi_omci_combinedport_s uc_combinedport[HI_OMCI_MAX_COMBINEDPORT_NUM];
}hi_omci_me_portmappkg_s;

/********Equipment extension package   HI_OMCI_ME_EQM_EXT_PACKAGE_E*********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_envsense;      /*Environmental sense*/
    hi_ushort16 us_contactoutput; /*Contact closure output*/
}hi_omci_me_equippkg_s;

/**********Protection data    HI_OMCI_ME_PROTECTION_DATA_E**************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_working_ptr;     /*Working ANI-G pointer*/
    hi_ushort16 us_protect_ptr;     /*Protection ANI-G pointer*/
    hi_uchar8   uc_protecttype;
    hi_uchar8   uc_reverind;
    hi_ushort16 us_restoretime;
    hi_ushort16 us_switchtime;
}hi_omci_me_protectdata_s;

/************Equipment protection profile  HI_OMCI_ME_EQM_PROTECTION_PROFILE_E**************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_protectslot[2];
    hi_uchar8 uc_workingslot[8];
    hi_uchar8 uc_protectstatus[2];
    hi_uchar8 uc_reverind;
    hi_uchar8 uc_restoretime;
}hi_omci_me_equipprotect_s;

/**************ONU remote debug    HI_OMCI_ME_ONU_REMOTE_DEBUG_E***************/
#define HI_OMCI_ME_RMT_CMD_LEN 25
#define HI_OMCI_ME_RMT_DBG_REPLY_LEN 30  /*result length*/

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;                    /* Instance ID */
    hi_uchar8   uc_cmdfmt;                               /* Command Format,  0x0--ASCII, 0x1--free format. */ 
    hi_uchar8   uc_cmd[HI_OMCI_ME_RMT_CMD_LEN];          /* Command */
    hi_uchar8   uc_reply[HI_OMCI_ME_RMT_DBG_REPLY_LEN];  /* Reply */
}hi_omci_me_remote_debug_s;

/***************ONU-E*****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uint32 ui_vendorid;
    hi_uchar8 uc_version[14];
    hi_uchar8 uc_sn[8];
}hi_omci_me_onu_e_s;

/***********ONU dynamic power management control**********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_reductcap; /*Power reduction management capability*/
    hi_uchar8   uc_reductmode;
    hi_ushort16 us_itransinit;
    hi_ushort16 us_itxinit;
    hi_uint32   ui_maxsleep; /*Maximum sleep interval*/
    hi_uint32   ui_minaware; /*Minimum aware interval*/
    hi_ushort16 us_minactive; /*Minimum active held interval*/
}hi_omci_me_powermng_s;

/************ONU image mode    HI_OMCI_ME_ONU_IMAGEMODE_E*************/
typedef struct 
{
    hi_uchar8 uc_uppqmap;                 
    hi_uchar8 uc_uppqnum;               
    hi_uchar8 uc_dnpqmap;            
    hi_uchar8 uc_dnpqnum;          
}omci_cos_queue_map_capa_s;

typedef struct 
{
    hi_uchar8 us_upcos0:3;
    hi_uchar8 us_upcos1:3;
    hi_uchar8 us_upcos2:3;
    hi_uchar8 us_upcos3:3;
    hi_uchar8 us_upcos4:3;
    hi_uchar8 us_upcos5:3;
    hi_uchar8 us_upcos6:3;
    hi_uchar8 us_upcos7:3; 

    hi_uchar8 us_dncos0:3;
    hi_uchar8 us_dncos1:3;
    hi_uchar8 us_dncos2:3;
    hi_uchar8 us_dncos3:3;
    hi_uchar8 us_dncos4:3;
    hi_uchar8 us_dncos5:3;
    hi_uchar8 us_dncos6:3;
    hi_uchar8 us_dncos7:3;
}omci_cos_queue_map_attr_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_flowmapmode;       /*Flow Mapping Mode*/
    hi_uchar8   uc_trafficoption;  
    hi_uchar8   uc_flowcarmode;           
    hi_uchar8   uc_onutransmode;    
    hi_ushort16 us_curmacnum;            
    hi_uint32   ul_macagetime;          
                                   
    hi_uchar8   uc_timesync;         
    hi_uint32   ul_olttimehigh;          
    hi_uint32   ul_olttimelow;           
    hi_ushort16 us_txopticallevel;     
    hi_ushort16 uc_opticalcurrent;      
    hi_char8    c_temperature;          
    hi_ushort16 us_voltage;              
    omci_cos_queue_map_capa_s st_d1q2pqcapa;  
    omci_cos_queue_map_attr_s st_d1q2pqmap;
    hi_uchar8 uc_batterymode;                     
}hi_omci_me_onu_imagemode_s;

/***********HW extended ONT ability************/
/* 用于ont image实体的属性3的枚举，指示ont是按照gem port traffic descriptor实体下发的值来做car，
   还是按照flow来做car，此情景主要用于一个gemport可对应多条流的情况;
   默认应该是1，按照gemport trfc dscrptr做car，与协议一致 */
typedef enum
{
    HI_OMCI_CAR_MODE_GEMPORT_CAR               = 1,
    HI_OMCI_CAR_MODE_FLOW_CAR                   = 2,
    HI_OMCI_CAR_MODE_BUTT,
}HI_OMCI_CAR_MODE_E;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_flowmappingmode;     /* Flow Mapping Mode*/
    hi_uchar8   uc_trafficmanage;        /* Traffic management option*/
    hi_uchar8   uc_flowcar;         /* Flow CAR*/
    hi_uchar8   uc_transparent;             /* Ont transparent*/
    hi_ushort16 us_macnum;          /* Current Mac Learning Number*/
    hi_uint32   ui_macagetime;      /* MAC Age Time */
    hi_uchar8   uc_timesync ;       /* Time sync from olt identify/Big entity ability */
    hi_uint32   ui_timehigh ;       /* OLT time high 32bits */
    hi_uint32   ui_timelow ;         /* OLT time low 32bits */
    hi_ushort16 us_signallevel;       /* Tx Optical signal level*/
    hi_ushort16 us_laserbias;       /* Laser Bias Current*/
    hi_uchar8   uc_temperature;     /* Temperature*/
    hi_ushort16 us_voltage;         /* voltage*/
    hi_uchar8   uc_queuecapability[4]; /* 802.1p_to_queue capability */
    hi_uchar8   uc_queuemap[6];     /* 802.1p_to_queue map */
    hi_uchar8   uc_batterymode;     /* batterymode */
}hi_omci_me_extended_ont_ability_s;

#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OMCI_ME_EQUIP_DEF_H__ */
