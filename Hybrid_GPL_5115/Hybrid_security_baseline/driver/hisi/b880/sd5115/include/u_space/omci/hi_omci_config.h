/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_config.h
  版 本 号   : 初稿
  作    者   : t00185260
  生成日期   : D2011_09_27

******************************************************************************/
#ifndef __HI_OMCI_CONFIG_H__
#define __HI_OMCI_CONFIG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_OMCI_PROC_CONTENTLEN_STD                    32
#define HI_OMCI_PROC_CONTENTLEN_EXT                    1961  /*新协议G.988  OMCI LEN = 2K:|5bytes GEM HEAD|*/

#define HI_OMCI_PROC_GET_MSGLEN                        29
#define HI_OMCI_PROC_HEADLEN_STD                       8
#define HI_OMCI_PROC_HEADLEN_EXT                       (HI_OMCI_PROC_HEADLEN_STD + 2)

#define HI_OMCI_PROC_MSGSTCONTEXT_LEN                  40
#define HI_OMCI_PROC_MSGSTTAIL_LEN                     8
#define HI_OMCI_PROC_MSG_MAXLEN                        HI_KERNEL_OMCI_PAYLOAD_LEN
#define HI_OMCI_PROC_MSGCONTEXT_MAXLEN                 (HI_KERNEL_OMCI_PAYLOAD_LEN - HI_OMCI_PROC_HEADLEN_EXT)
#define HI_OMCI_PROC_DEVICEID_STD                      0x0A/*G.984协议设备ID*/
#define HI_OMIC_PROC_TCI_PRI_MASK                      0x8000
                                                      /*|8bytes MSG HEAD||2bytes SIZE|1961bytes MSG CONTENT||4bytes CRC|*/

/*OMCI管理实体属性操作类型*/
typedef enum
{
    HI_OMCI_ATT_R_E         = 0x1,      /*R*/
    HI_OMCI_ATT_W_E         = 0x2,      /*W*/
    HI_OMCI_ATT_S_E         = 0x4,      /*Set-by-Create*/
    HI_OMCI_ATT_R_W_E       = 0x3,      /*R and W*/
    HI_OMCI_ATT_R_S_E       = 0x5,      /*R and Set-by-Create */
    HI_OMCI_ATT_W_S_E       = 0x6,      /*W and Set-by-Create */
    HI_OMCI_ATT_R_W_S_E     = 0x7,      /*R, W and Set-by-Create*/
}hi_omci_me_att_opt_e;

/* ONT是否支持属性定义 */
typedef enum 
{
    HI_OMCI_ATT_NOT_SUPPORT_E = 0x0,  /* ONT不支持对该属性的Set操作 */
    HI_OMCI_ATT_SUPPORT_E     = 0x1,  /* ONT支持对该属性的Set操作 */
} hi_omci_me_att_spt_e;

/*omci消息通用返回错误*/
typedef enum 
{
    HI_OMCI_PRO_ERR_NO_E             =   0x0,     /*command processed successfully*/
    HI_OMCI_PRO_ERR_PROCESS_ERR_E    =   0x1,     /*command processing error*/
    HI_OMCI_PRO_ERR_NOT_SUPPORTED_E  =   0x2,     /*command not supported*/
    HI_OMCI_PRO_ERR_PARA_ERR_E       =   0x3,     /*parameter error*/
    HI_OMCI_PRO_ERR_UNKNOWN_ME_E     =   0x4,     /*unknown managed entity*/
    HI_OMCI_PRO_ERR_UNKNOWN_IN_E     =   0x5,     /*unknown managed entity instance*/
    HI_OMCI_PRO_ERR_DEV_BUSY_E       =   0x6,     /*device busy*/   
    HI_OMCI_PRO_ERR_IN_EXIST_E       =   0x7,     /*instance exists*/
    HI_OMCI_PRO_ERR_ATT_FAIL_E       =   0x9,     /*attribute(s) failed or unknown*/
}hi_omci_msg_errcode_e;

/* OMCI管理实体ID */
typedef enum 
{
    HI_OMCI_PRO_ME_ONT_DATA_E                    = 2,      /* ONT Data*/
    HI_OMCI_PRO_ME_CARD_HOLDER_E                 = 5,      /* Cardholder*/
    HI_OMCI_PRO_ME_CIRCUIT_PACK_E                = 6,      /* Cirucit Pack*/
    HI_OMCI_PRO_ME_SW_IMAGE_E                    = 7,      /* Software Image*/
    HI_OMCI_PRO_ME_PPTP_ETH_UNI_E                = 11,     /* PPTP Ethernet UNI*/
    HI_OMCI_PRO_ME_PPTP_CES_UNI_E                = 12,     /* PPTP CES UNI*/
    HI_OMCI_PRO_ME_LOGICAL_64NSUBPORT_CTP_E      =13,  /*Logical N x 64kbit/s sub-port connection termination point*/
    HI_OMCI_PRO_ME_INTERWORK_VCC_TP_E            =14,   /*Interworking VCC termination point*/
    HI_OMCI_PRO_ME_AAL5_PROFILE_E                = 16,       /*AAL 5 profile*/
    HI_OMCI_PRO_ME_AAL5_PMH_DATA_E               =18,   /*AAL 5 performance monitoring history data*/
    HI_OMCI_PRO_ME_CES_SERVICE_E                 = 21,     /* CES Service Profile*/
    HI_OMCI_PRO_ME_CES_PIMH_DATA_E               = 23,     /* CES Physical Interface Monitoring History Data*/
    HI_OMCI_PRO_ME_ETH_PM_E                      = 24,     /* Ethernet PM History Data*/
    HI_OMCI_PRO_ME_PON_PPTP_E                    = 40,     /* PON PPTP*/
    HI_OMCI_PRO_ME_MAC_SERVICE_E                 = 45,     /* MAC Bridge service Profile*/
    HI_OMCI_PRO_ME_MAC_CFG_DATA_E                = 46,     /* MAC Bridge Configuration Data*/
    HI_OMCI_PRO_ME_MAC_PORT_CFG_DATA_E           = 47,     /* MAC Bridge Port Configuration Data*/
    HI_OMCI_PRO_ME_MAC_PORT_DES_DATA_E           = 48,     /* MAC Bridge Port Designation Data*/
    HI_OMCI_PRO_ME_MAC_PORT_FILT_DATA_E          = 49,     /* MAC Bridge Port Filter Table Data*/
    HI_OMCI_PRO_ME_MAC_PORT_TAB_DATA_E           = 50,     /* MAC Bridge Port Bridge Table Data*/
    HI_OMCI_PRO_ME_MAC_PM_E                      = 51,     /* MAC Bridge PM History Data*/
    HI_OMCI_PRO_ME_MAC_PORT_PM_E                 = 52,     /* MAC Bridge Port PM History Data*/
    HI_OMCI_PRO_ME_PPTP_POTS_UNI_E               = 53,     /* PPTP POTS UNI*/
    HI_OMCI_PRO_ME_VOICE_PM_E                    = 55,     /* Voice PM History Data*/
    HI_OMCI_PRO_ME_VOICE_SERVICE_PROFILE_E       =58, /*Voice service profile*/
    HI_OMCI_PRO_ME_VP_PMH_DATA_E                 =62, /*VP performance monitoring history data*/
    HI_OMCI_PRO_ME_VLAN_TAG_E                    = 78,     /* VLAN Tagging Operation Cfg Data*/
    HI_OMCI_PRO_ME_MAC_PORT_FILT_PRE_E           = 79,     /* MAC Bdg Port Filter Pre-assign Table*/
    HI_OMCI_PRO_ME_PPTP_VIDEO_UNI_E              = 82,     /* PPTP Video UNI*/
    HI_OMCI_PRO_ME_PPTP_LCT_UNI_E                =83,  /*Physical path termination point LCT UNI*/
    HI_OMCI_PRO_ME_VLAN_FILT_E                   = 84,     /* VLAN Tagging Filter Data*/
    HI_OMCI_PRO_ME_ETH_PM2_E                     = 89,     /* Ethernet PM History Data2*/
    HI_OMCI_PRO_ME_PPTP_VIDEO_ANI_E              = 90,     /* PPTP VIDEO ANI*/
    HI_OMCI_PRO_ME_PPTP_XDSL_UNI_P1_E            =98,    /*Physical path termination point xDSL UNI part 1*/
    HI_OMCI_PRO_ME_PPTP_XDSL_UNI_P2_E            =99,    /*Physical path termination point xDSL UNI part 2*/
    HI_OMCI_PRO_ME_XDSL_LINE_INV_STAT_P1_E       =100, /*xDSL line inventory and status data part 1*/
    HI_OMCI_PRO_ME_XDSL_LINE_INV_STAT_P2_E       =101, /*xDSL line inventory and status data part 2*/
    HI_OMCI_PRO_ME_XDSL_CHN_DS_STAT_E            =102, /*xDSL channel downstream status data*/
    HI_OMCI_PRO_ME_XDSL_CHN_US_STAT_E            = 103, /*xDSL channel upstream status data*/
    HI_OMCI_PRO_ME_XDSL_CFG_PROFILE_P1_E         = 104, /*xDSL line configuration profile part 1*/
    HI_OMCI_PRO_ME_XDSL_CFG_PROFILE_P2_E         = 105, /*xDSL line configuration profile part 2*/
    HI_OMCI_PRO_ME_XDSL_CFG_PROFILE_P3_E         = 106, /*xDSL line configuration profile part 3*/
    HI_OMCI_PRO_ME_XDSL_CHN_CFG_PROFILE_E        = 107, /*XDSL channel configuration profile*/
    HI_OMCI_PRO_ME_XDSL_SUBCR_MASK_DS_PROFILE_E  =108,/*xDSL subcarrier masking downstream profile*/
    HI_OMCI_PRO_ME_XDSL_SUBCR_MASK_US_PROFILE_E  =109,/*xDSL subcarrier masking upstream profile*/
    HI_OMCI_PRO_ME_XDSL_PSD_MASK_PROFILE_E       = 110, /*XDSL PSD mask profile*/
    HI_OMCI_PRO_ME_XDSL_DS_PFI_BANDS_PROFILE_E   =111, /*xDSL downstream RFI bands profile*/
    HI_OMCI_PRO_ME_XDSL_XTUC_PMH_DATA_E          = 112, /*xDSL xTU-C performance monitoring history data*/
    HI_OMCI_PRO_ME_XDSL_XTUR_PMH_DATA_E          = 113, /*xDSL xTU-R  performance monitoring history data*/
    HI_OMCI_PRO_ME_XDSL_XTUC_CHN_PMH_DATA_E      = 114, /*xDSL xTU-C channelperformance monitoring history data*/
    HI_OMCI_PRO_ME_XDSL_XTUR_CHN_PMH_DATA_E      = 115, /*xDSL xTU-R channel performance monitoring history data*/
    HI_OMCI_PRO_ME_VIDEO_PROFILE_E               = 128,    /* Video Return Path Service Profile*/
    HI_OMCI_PRO_ME_VIDEO_STAT_E                  = 129,    /* Video Return Path Statistics*/
    HI_OMCI_PRO_ME_802_1P_E                      = 130,    /* 802.1p Mapper Service Profile*/
    HI_OMCI_PRO_ME_OLT_G_E                       =131,/*OLT-G*/
    HI_OMCI_PRO_ME_ONU_POWER_SHEDDING_E          =133, /*ONU power shedding*/
    HI_OMCI_PRO_ME_IP_HOST_CFG_DATA_E            =134,    /* IP Host Config Data*/
    HI_OMCI_PRO_ME_IP_HOST_PMH_DATA_E            =135,    /* IP Host performance monitoring history Data*/
    HI_OMCI_PRO_ME_TCP_UDP_CFG_DATA_E            =136, /*TCP/UDP config data*/
    HI_OMCI_PRO_ME_NETWORK_ADDR_E                =137, /*network address*/
    HI_OMCI_PRO_ME_VOIP_CFG_DATA_E               =138,  /*VoIP  config data*/
    HI_OMCI_PRO_ME_VOICE_CTP_E                   =139,  /*VoIP voice CTP*/
    HI_OMCI_PRO_ME_CALL_CTRL_PMH_DATA_E          =140, /*Call control performance monitoring history data*/
    HI_OMCI_PRO_ME_VOIP_LINE_STATUS_E            =141,  /*VoIP line status*/
    HI_OMCI_PRO_ME_VOIP_MEDIA_PROFILE_E          =142, /*VoIP media profile*/
    HI_OMCI_PRO_ME_RTP_RPOFILE_DATA_E            =143,/**/
    HI_OMCI_PRO_ME_RTP_PMH_DATA_E                =144,/**/
    HI_OMCI_PRO_ME_NETWORK_DIAL_PLAN_TAB_E       =145,/**/
    HI_OMCI_PRO_ME_VOIP_APP_SRV_PROFILE_E        =146,/**/
    HI_OMCI_PRO_ME_FERTURE_ACC_CODE_E            =147,/**/
    HI_OMCI_PRO_ME_ATH_SECR_METHOD_E             =148,/*Authentication security method*/
    HI_OMCI_PRO_ME_SIP_CFG_PORTAL_E              =149,/**/
    HI_OMCI_PRO_ME_SIP_AGENT_CFG_DATA_E          =150,/**/
    HI_OMCI_PRO_ME_SIP_CALL_INIT_PMH_DATA_E      =152,/**/
    HI_OMCI_PRO_ME_SIP_USER_DATA_E               =153,/**/
    HI_OMCI_PRO_ME_MGC_CFG_PORTAL_E              =154,/**/
    HI_OMCI_PRO_ME_MGC_CFG_DATA_E                =155,/**/
    HI_OMCI_PRO_ME_MGC_PMH_DATA_E                =156,/**/
    HI_OMCI_PRO_ME_LARGE_STR_E                   =157,/**/
    HI_OMCI_PRO_ME_ONT_REMOTE_DEBUG_E            =158,    /* ONT Remote Debug*/
    HI_OMCI_PRO_ME_EQM_PROTECTION_PROFILE_E      =159,/**/
    HI_OMCI_PRO_ME_EQM_EXT_PACKAGE_E             =160,/**/
    HI_OMCI_PRO_ME_MOCA_UNI_E                    =162,    /* PPTP MoCA UNI*/
    HI_OMCI_PRO_ME_MOCA_ETH_PM_E                 =163,    /* MoCA Ethernet PM History Data*/
    HI_OMCI_PRO_ME_MOCA_INTF_PM_E                =164,    /* MoCA Interface PM History Data*/
    HI_OMCI_PRO_ME_VDSL2_LINE_CFG_EXTENSION_E    =165,/**/
    HI_OMCI_PRO_ME_XDSL_LINE_INV_STAT_P3_E       =166, /*xDSL line inventory and status data part 3*/
    HI_OMCI_PRO_ME_XDSL_LINE_INV_STAT_P4_E       =167, /*xDSL line inventory and status data part 4*/
    HI_OMCI_PRO_ME_VDSL2_LINE_INV_STAT_P1_E      =168, /*VDSL2 line inventory and status data part 1*/
    HI_OMCI_PRO_ME_VDSL2_LINE_INV_STAT_P2_E      =169, /*VDSL2 line inventory and status data part 2*/
    HI_OMCI_PRO_ME_VDSL2_LINE_INV_STAT_P3_E      =170, /*VDSL2 line inventory and status data part 3*/
    HI_OMCI_PRO_ME_EXTEND_VLAN_TAG_E             =171,    /* Extended VLAN tagging operation configuration data*/
    HI_OMCI_PRO_ME_ONT_G_E                       =256,    /* ONT-G*/
    HI_OMCI_PRO_ME_ONT2_G_E                      =257,    /* ONT2-G*/
    HI_OMCI_PRO_ME_ONU_G_E                       =258,    /* ONU-G*/
    HI_OMCI_PRO_ME_ONU2_G_E                      =259,    /* ONU-2G*/
    HI_OMCI_PRO_ME_TC_ADAPTER_G_E                =261,    /* PON TC Adapter-G*/
    HI_OMCI_PRO_ME_TCONT_E                       =262,    /* T-CONT*/
    HI_OMCI_PRO_ME_ANI_G_E                       =263,    /* ANI-G*/
    HI_OMCI_PRO_ME_UNI_G_E                       =264,    /* UNI-G*/
    HI_OMCI_PRO_ME_GEM_IWTP_E                    =266,    /* GEM Interworking Termination Point*/
    HI_OMCI_PRO_ME_GEM_PORT_PM_E                 =267,    /* GEM Port PM History Data*/
    HI_OMCI_PRO_ME_GEM_CTP_E                     =268,    /* GEM Port Network CTP*/
    HI_OMCI_PRO_ME_GAL_TDM_PROFILE_E             =271,    /* GAL TDM Profile*/
    HI_OMCI_PRO_ME_GAL_ETH_PROFILE_E             =272,    /* GAL Ethernet Profile*/
    HI_OMCI_PRO_ME_THRESHOLD_DATA1_E             =273,    /* Threshold Data1*/
    HI_OMCI_PRO_ME_THRESHOLD_DATA2_E             =274,    /* Threshold Data2*/
    HI_OMCI_PRO_ME_GAL_ETH_PM_E                  =276,    /* GAL Ethernet PM History Data*/
    HI_OMCI_PRO_ME_PQ_G_E                        =277,    /* Priority Queue-G*/
    HI_OMCI_PRO_ME_TS_G_E                        =278,    /* Traffic Scheduler-G*/
    HI_OMCI_PRO_ME_PROTECTION_DATA_E             =279,    /* Protection Data*/
    HI_OMCI_PRO_ME_GEM_TRAF_DESC_E               =280,    /* GEM Traffic Descriptor*/
    HI_OMCI_PRO_ME_MULTICAST_IWTP_E              =281,    /* Multicast GEM Interworking TP*/
    HI_OMCI_PRO_ME_PSEUDOWIRE_TP_E               =282,/**/
    HI_OMCI_PRO_ME_RTP_PSEUDOWIRE_PARA_E         =283,/**/
    HI_OMCI_PRO_ME_PESUDOWIRE_MAINT_PROFILE_E    =284,/**/
    HI_OMCI_PRO_ME_PESUDOWIRE_PMH_DATA_E         =285,/**/
    HI_OMCI_PRO_ME_ETH_FLOW_TP_E                 =286,/**/
    HI_OMCI_PRO_ME_OMCI_E                        =287,/*omci*/
    HI_OMCI_PRO_ME_ME_E                          =288,/*Managed entity*/
    HI_OMCI_PRO_ME_VOIP_SW_IMAGE_OLD_E           =289,    /* HW OLD VOIP Software Image*/
    HI_OMCI_PRO_ME_DOT1X_PORT_EXT_PACKAGE_E      =290,/**/
    HI_OMCI_PRO_ME_DOT1X_CFG_PROFILE_E           =291,/**/
    HI_OMCI_PRO_ME_DOT1X_PMH_DATA_E              =292,/**/
    HI_OMCI_PRO_ME_RADIUS_PMH_DATA_E             = 293,/**/
    HI_OMCI_PRO_ME_ETH_PM3_E                     = 296,    /* Ethernet PM3 history data*/
    HI_OMCI_PRO_ME_PORT_MAP_PACKAGE_E            =297,/**/
    HI_OMCI_PRO_ME_DOT1_RATE_LIMITER_E           =298,/**/
    HI_OMCI_PRO_ME_DOT1AG_MAINT_DOMAIN_E         =299,/**/
    HI_OMCI_PRO_ME_DOT1AG_MAINT_ASSOCIATION_E    =300,/**/
    HI_OMCI_PRO_ME_DOT1AG_DEF_MD_LV_E            =301,/**/
    HI_OMCI_PRO_ME_DOT1AG_MEP_E                  =302,/**/
    HI_OMCI_PRO_ME_MULTICASE_TRANSFER_MODE_OLD_E = 303,    /* HW Old extended multicast transfer mode*/
    HI_OMCI_PRO_ME_MULTI_OPER_PROFILE_E          = 309,     /*Multicast operations profile*/
    HI_OMCI_PRO_ME_ONT_IMAGEMODE_E               = 350,    /* Flow Mapping Mode*/
    HI_OMCI_PRO_ME_ETH_PM4_E                     = 351,    /* Ethernet PM4 history data*/
    HI_OMCI_PRO_ME_ONT_FLOW_E                    = 352,    /* HW extended ONT flow*/
    HI_OMCI_PRO_ME_VOIP_SW_IMAGE_E               = 353,    /* NEW HW VOIP Software Image ME ID*/
    HI_OMCI_PRO_ME_MULTICASE_TRANSFER_MODE_E     = 367,    /* HW extended multicast transfer mode*/
    HI_OMCI_PRO_ME_E_PPTP_ETH_UNI_E              = 370,    /* HW extended Physical path termination point Ethernet UNI Extend */
    HI_OMCI_PRO_ME_HWEXTEND_VLAN_TAG_E           = 65412,    /* HW Extended VLAN tagging operation configuration data*/

    HI_OMCI_PRO_ME_ENTITY_MAX_E,                           /* Max entity id*/
}hi_omci_proc_me_type_e;

typedef enum 
{
    HI_OMCI_PRO_ACTIONS_NULL0_E              = 0, 
    HI_OMCI_PRO_ACTIONS_NULL1_E              = 1, 
    HI_OMCI_PRO_ACTIONS_NULL2_E              = 2, 
    HI_OMCI_PRO_ACTIONS_NULL3_E              = 3, 
    HI_OMCI_PRO_ACTIONS_CREATE_E             = 4,    //Create a managed entity instance with its attributes  
    HI_OMCI_PRO_ACTIONS_CREATECC_E           = 5,    //Create complete connection
    HI_OMCI_PRO_ACTIONS_DELETE_E             = 6,    //Delete a managed entity instance
    HI_OMCI_PRO_ACTIONS_DELETECC_E           = 7,    //Delete an instance of ATM VP/VC Cross-Connection ME and two associated instances of VP Network CTP-G or VC Network CTP-G ME    
    HI_OMCI_PRO_ACTIONS_SET_E                = 8,    //Set one or more attributes of a managed entity
    HI_OMCI_PRO_ACTIONS_GET_E                = 9,    //Get one or more attributes of a managed entity
    HI_OMCI_PRO_ACTIONS_GETCC_E              = 10,   //Get all attributes of an instance of ATM VP/VC Cross-Connection ME and the attributes of the associated instances of VP Network CTP-G or VC Network CTP-G
    HI_OMCI_PRO_ACTIONS_GETALLALARM_E        = 11,   //Get all alarms
    HI_OMCI_PRO_ACTIONS_GETALLALARMNEXT_E    = 12,   //Get the active alarm status of the next managed entity
    HI_OMCI_PRO_ACTIONS_MIBUPLOAD_E          = 13,   //Latch the MIB
    HI_OMCI_PRO_ACTIONS_MIBUPLOADNEXT_E      = 14,   //Get latched attributes of a managed entity instance
    HI_OMCI_PRO_ACTIONS_MIBRESET_E           = 15,   //Clear the MIB and re-initialize it to its default and reset the MIB data sync counter to 0
    HI_OMCI_PRO_NOTIFICATIONS_ALARM_E        = 16,   //Notification of an alarm
    HI_OMCI_PRO_NOTIFICATIONS_AVC_E          = 17,   //Notification of an autonomous attribute value change
    HI_OMCI_PRO_ACTIONS_TEST_E               = 18,   //Request a test on a specific managed entity
    HI_OMCI_PRO_ACTIONS_STARTDOWNLOAD_E      = 19,   //Start a software download action
    HI_OMCI_PRO_ACTIONS_DOWNLOADING_E        = 20,   //Download a section of a software image
    HI_OMCI_PRO_ACTIONS_ENDDOWNLOAD_E        = 21,   //End of a software download action
    HI_OMCI_PRO_ACTIONS_ACTIVEIMAGE_E        = 22,   //Activate the downloaded software image
    HI_OMCI_PRO_ACTIONS_COMMITIMAGE_E        = 23,   //Commit the downloaded software image
    HI_OMCI_PRO_ACTIONS_SYNCTIME_E           = 24,   //Synchronize the time between OLT and ONT
    HI_OMCI_PRO_ACTIONS_REBOOT_E             = 25,   //Reboot ONT, Subscriber Line Card or PON IF Line Card
    HI_OMCI_PRO_ACTIONS_GETNEXT_E            = 26,   //Get the latched attribute values of the managed entity within the current snapshot
    HI_OMCI_PRO_NOTIFICATIONS_TESTRESULT_E   = 27,   //Notification of test result that is initiated by "Test"
    HI_OMCI_PRO_ACTIONS_GET_CURRDATA_E       = 28,   //Get current counter value associated with one or more attributes of a managed entity
    HI_0MCI_PRO_ACTIONS_SET_TABLE_E          = 29,   //Set one or more rows of a table
    HI_OMCI_PRO_MSG_TYPE_MAX_E,                      //Max entity id
} hi_omci_proc_msg_type_e;


/* OMCI操作类型 */
typedef enum 
{
    HI_OMCI_QUERY = 0,     /* 查询操作*/
    HI_OMCI_ADD,           /* 添加操作 */
    HI_OMCI_DELETE,        /* 删除操作*/
    HI_OMCI_SET,           /* 修改操作*/
    HI_OMCI_ACTIVE,        /* 使能操作，例如端口使能等 */
    HI_OMCI_DEACTIVE,      /* 禁止操作，例如端口禁止等 */
    HI_OMCI_RESET,         /* 复位本地MIB */
	HI_OMCI_NOACTION       /* 空操作 */
}hi_omci_oper_type_e;

/* Ethernet Port state */
typedef enum tagOmciDesignPortState
{
    HI_OMCI_DESIGN_PORT_STATE_DISABLE_E  = 0x0,
    HI_OMCI_DESIGN_PORT_STATE_LISTENING_E,
    HI_OMCI_DESIGN_PORT_STATE_LEARNING_E,
    HI_OMCI_DESIGN_PORT_STATE_FORWARDING_E,
    HI_OMCI_DESIGN_PORT_STATE_BLOCKING_E,
    HI_OMCI_DESIGN_PORT_STATE_LINKDOWN_E,
    HI_OMCI_DESIGN_PORT_STATE_STP_OFF_E,
}hi_omci_design_port_state_e;

/*对应每个实体的api函数传入的消息结构*/
/*消息结构公共头*/
typedef struct
{
   hi_ushort16  us_meid;      /*实体ID*/
   hi_ushort16  us_instid;     /*实例ID*/
   hi_ushort16  us_attmask; /*属性掩码*/
   hi_ushort16  us_msgsize; /*消息大小*/
   hi_uchar8    uc_attnum;  /*属性个数*/
   hi_uchar8    uc_resv[3];
}hi_omci_me_msg_head_s;

/*统一接口消息*/
typedef struct
{
   hi_omci_me_msg_head_s    st_msghead;
   hi_uchar8                uc_content[HI_OMCI_PROC_CONTENTLEN_EXT+2];
}hi_omci_msg_s;


#define  HI_OMCI_ATTR1   1
#define  HI_OMCI_ATTR2   2
#define  HI_OMCI_ATTR3   3
#define  HI_OMCI_ATTR4   4
#define  HI_OMCI_ATTR5   5
#define  HI_OMCI_ATTR6   6
#define  HI_OMCI_ATTR7   7
#define  HI_OMCI_ATTR8   8
#define  HI_OMCI_ATTR9   9
#define  HI_OMCI_ATTR10  10
#define  HI_OMCI_ATTR11  11
#define  HI_OMCI_ATTR12  12
#define  HI_OMCI_ATTR13  13
#define  HI_OMCI_ATTR14  14
#define  HI_OMCI_ATTR15  15
#define  HI_OMCI_ATTR16  16

#define  HI_OMCI_ZERO	0
#define  HI_OMCI_FALSE	0
#define  HI_OMCI_TRUE	1
#define HI_OMCI_CHECK_ATTR(us_mask,en_attr) (((us_mask & (0x8000 >> (en_attr-1))) == 0) ? HI_FALSE : HI_TRUE)

typedef struct 
{
    hi_uint32       ui_cmdtype;
    hi_ushort16     us_meid;
    hi_ushort16     us_instid;
    hi_uchar8       uc_alarmid;
    hi_uchar8       uc_status;
}hi_omci_proc_alarm_msg_item_s;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OMCI_CONFIG___ */
