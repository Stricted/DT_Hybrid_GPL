/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : omci_me_general_def.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2011_09_30

******************************************************************************/
#ifndef __OMCI_ME_GENERAL_DEF_H__
#define __OMCI_ME_GENERAL_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#pragma pack(1)
/******************UNI-G   HI_OMCI_ME_UNI_G_E*******************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_confgsta;            /*representing that alarm reprots at TC */
    hi_uchar8   uc_adminstate;          /*0-active(unlock:0),1-deactive(lock:1)*/
	hi_uchar8   uc_mngcap;              /*Management capability*/
	hi_ushort16	us_non_omci;			/*Non-OMCI management identifier*/
}hi_omci_me_uni_g_s;

/*******************OLT-G   HI_OMCI_ME_OLT_G_E*******************/
#define HI_OMCI_OLT_EQUIPID_LEN 20
#define HI_OMCI_OLT_VERSION_LEN 14

typedef struct 
{
    hi_uint32 gemseqnum;     /*equence number of the specified GEM superframe*/
    hi_uchar8 uc_tstampn[10];/*TstampN as defined in [ITU-T G.984.3]*/
}omci_olt_timeofday_s;

typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uint32   ui_oltvendorid;/*  OLT vendor id*/
    hi_uchar8   uc_equipmentid[HI_OMCI_OLT_EQUIPID_LEN];/*  Equipment id */
    hi_uchar8   uc_version[HI_OMCI_OLT_VERSION_LEN];  /*Version*/  
    omci_olt_timeofday_s st_time;
}hi_omci_me_olt_g_s;

/***************Network address  HI_OMCI_ME_NETWORK_ADDR_E****************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_secur_ptr;/*Security pointer*/
    hi_ushort16 us_addr_ptr; /*Address pointer*/
}hi_omci_me_network_addr_s;

/***********Authentication security method  HI_OMCI_ME_ATH_SECR_METHOD_E************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_validscheme;
    hi_uchar8 uc_username1[25];
    hi_uchar8 uc_password[25];
    hi_uchar8 uc_realm[25];
    hi_uchar8 uc_username2[25];
}hi_omci_me_auth_secur_method_s;

/***********Large string  HI_OMCI_ME_LARGE_STR_E**********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_largestring;
    hi_uchar8 uc_part1[25];
    hi_uchar8 uc_part2[25];
    hi_uchar8 uc_part3[25];
    hi_uchar8 uc_part4[25];
    hi_uchar8 uc_part5[25];
    hi_uchar8 uc_part6[25];
    hi_uchar8 uc_part7[25];
    hi_uchar8 uc_part8[25];
    hi_uchar8 uc_part9[25];
    hi_uchar8 uc_part10[25];
    hi_uchar8 uc_part11[25];
    hi_uchar8 uc_part12[25];
    hi_uchar8 uc_part13[25];
    hi_uchar8 uc_part14[25];
    hi_uchar8 uc_part15[25];
}hi_omci_me_large_string_s;

/**********Threshold data 1  HI_OMCI_ME_THRESHOLD_DATA1_E**********/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uint32 ui_thres1;
    hi_uint32 ui_thres2;
    hi_uint32 ui_thres3;
    hi_uint32 ui_thres4;
    hi_uint32 ui_thres5;
    hi_uint32 ui_thres6;
    hi_uint32 ui_thres7;
}hi_omci_me_threshold1_s;

/****************Threshold data 2  HI_OMCI_ME_THRESHOLD_DATA2_E*****************/
typedef struct 
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uint32 ui_thres8;
    hi_uint32 ui_thres9;
    hi_uint32 ui_thres10;
    hi_uint32 ui_thres11;
    hi_uint32 ui_thres12;
    hi_uint32 ui_thres13;
    hi_uint32 ui_thres14;
}hi_omci_me_threshold2_s;

/**************OMCI  HI_OMCI_ME_OMCI_E***************/
#define OMCI_ME_TYPE_NUM 350
#define OMCI_MSG_TYPE_NUM 29

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_metype[OMCI_ME_TYPE_NUM];
    hi_uchar8   uc_msgtype[OMCI_MSG_TYPE_NUM];
}hi_omci_me_omci_s;

/**********Managed entity*********/
#define OMCI_ATTR_ME_POINTER_NUM 16
#define OMCI_ALARM_CODE_NUM 1
#define OMCI_AVC_NUM 10
#define OMCI_INST_NUM 20

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_name[25];
    hi_ushort16 us_attrtable[OMCI_ATTR_ME_POINTER_NUM];
    hi_uchar8   uc_access;
    hi_uchar8   uc_alarmtable[OMCI_ALARM_CODE_NUM];
    hi_uchar8   uc_avctable[OMCI_AVC_NUM];
    hi_uint32   ui_action;
    hi_ushort16 us_insttable[OMCI_INST_NUM];
    hi_uchar8   uc_support;
}hi_omci_me_mng_entity_s;

/***************Attribute*****************/
#define OMCI_CODE_POINTS_NUM 1
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_name[25];
    hi_ushort16 us_size;
    hi_uchar8   uc_access;
    hi_uchar8   uc_format;
    hi_uint32   ui_lowerlimit;
    hi_uint32   ui_upperlimit;
    hi_uint32   ui_bitfield;
    hi_ushort16 us_codepoint[OMCI_CODE_POINTS_NUM];
    hi_uchar8   uc_support;
}hi_omci_me_attr_s;

/*************Octet string***********/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_length;
    hi_uchar8   uc_part1[25];
    hi_uchar8   uc_part2[25];
    hi_uchar8   uc_part3[25];
    hi_uchar8   uc_part4[25];
    hi_uchar8   uc_part5[25];
    hi_uchar8   uc_part6[25];
    hi_uchar8   uc_part7[25];
    hi_uchar8   uc_part8[25];
    hi_uchar8   uc_part9[25];
    hi_uchar8   uc_part10[25];
    hi_uchar8   uc_part11[25];
    hi_uchar8   uc_part12[25];
    hi_uchar8   uc_part13[25];
    hi_uchar8   uc_part14[25];
    hi_uchar8   uc_part15[25];
}hi_omci_me_octet_string_s;

/***************General purpose buffer***************/
#define OMCI_RESULT_STRING_LEN 100
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uint32 ui_maxsize;
    hi_uchar8 uc_bufftable[OMCI_RESULT_STRING_LEN];
}hi_omci_me_gen_purpose_buf_s;

/***************File transfer controller**************/
#define OMCI_DIRECTORY_LIST_RESULT_LEN 1
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_protocol;        /*Supported transfer protocols*/
    hi_ushort16 us_filetype;        /*File type*/
    hi_ushort16 us_fileinst;        /*File instance*/
    hi_ushort16 us_localfile_ptr;   /*Local file name pointer*/
    hi_ushort16 us_networkaddr_ptr; /*Network address pointer*/
    hi_uchar8   uc_trigger;         /*File transfer trigger*/
    hi_uchar8   uc_status;          /*File transfer status*/
    hi_ushort16 us_gemiwtp_ptr;     /*GEM IWTP pointer*/
    hi_ushort16 us_vlan;            /*VLAN*/
    hi_uint32   ui_filesize;        /*File size*/
    hi_uchar8   uc_directorylist[OMCI_DIRECTORY_LIST_RESULT_LEN];/*Directory listing table*/
}hi_omci_me_file_trans_s;

/*************Generic status portal**************/
#define OMCI_STATUS_DOC_TABLE_SIZE 1 /*Status document table*/
#define OMCI_CONFIG_DOC_TABLE_SIZE 1 /*Configuration document table*/

typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8 uc_statusdoc[OMCI_STATUS_DOC_TABLE_SIZE];
    hi_uchar8 uc_cfgdoc[OMCI_CONFIG_DOC_TABLE_SIZE];
    hi_uchar8 uc_avcrate;
}hi_omci_me_gen_status_portal_s;

/*************SNMP configuration data*************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_ushort16 us_version;     /*SNMP version*/
    hi_ushort16 us_agentaddr;   /*SNMP agent address*/
    hi_uint32   ui_serveraddr;  /*SNMP server address*/
    hi_ushort16 us_serverport;  /*SNMP server port*/
    hi_ushort16 us_security_ptr;/*Security name pointer*/
    hi_ushort16 us_comread;     /*Community for read*/
    hi_ushort16 us_comwrite;    /*Community for write*/
    hi_ushort16 us_sysname_ptr; /*Sys name pointer*/
}hi_omci_me_snmp_cfg_s;

/**************TR-069 management server*************/
typedef struct
{
    hi_omci_me_msg_head_s st_msghead;
    hi_uchar8   uc_adminstate;
    hi_ushort16 us_acsaddr;
    hi_ushort16 us_tag;
}hi_omci_me_tr069_mng_server_s;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OMCI_ME_GENERAL_DEF_H__ */
