/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_mib.h
  版 本 号   : 初稿
  作    者   : t00185260
  生成日期   : D2011_09_22

******************************************************************************/
#ifndef __HI_OMCI_MIB_H__
#define __HI_OMCI_MIB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_OMCI_SYNCNUM_MAX                     255

#define HI_OMCI_SQL_MAX_LEN                     1024    /*sql语句最大长度*/
#define HI_ATT_MASK_ONE                         0x8000  /*属性掩码第一位*/
#define HI_OMCI_ATT_INFO_MAX                    32      
#define HI_OMCI_MIB_MAXATTR_NUM                 16

#define HI_OMCI_STD_ATTR_MAXSIZE                26      /*该宏定义严禁修改, 每次上传ME实体消息的最大长度*/
#define HI_OMCI_EXT_ATTR_MAXSIZE                250

#define HI_OMCI_MIB_UPLOAD_SET_ENTITY(pv_data,meid)     ((*(hi_ushort16 *)(pv_data + 0 )) = H_HTONS(meid))
#define HI_OMCI_MIB_UPLOAD_SET_INST(pv_data,instid)     ((*(hi_ushort16 *)(pv_data + 2 )) = H_HTONS(instid))
#define HI_OMCI_MIB_UPLOAD_SET_MASK(pv_data,mask)       ((*(hi_ushort16 *)(pv_data + 4 )) = H_HTONS(mask))
#define HI_OMCI_MIB_UPLOAD_OFFSET                6

typedef struct 
{
    hi_ushort16     us_meid;
    hi_uchar8       uc_alarmid;
    hi_uint32       ui_cmdtype;
    hi_uchar8*      uc_description;
}hi_omci_mib_alarm_item_s;

typedef struct 
{
    hi_ushort16     us_meid;
    hi_ushort16     us_instid;
    hi_uchar8       uc_alarm[28];
}hi_omci_mib_alarm_s;

typedef struct
{
    hi_list_head  st_listhead;
    hi_uint32     ui_size;
    hi_uchar8     uc_text[4];
}hi_omci_mibupload_seg_s;

typedef struct
{
    hi_uint32 ui_totalnum;
    hi_uint32 ui_blocksize;     //HI_OMCI_STD_ATTR_MAXSIZE,HI_OMCI_EXT_ATTR_MAXSIZE
    hi_list_head *pst_lastlist;
    hi_list_head  st_listhead;
}hi_omci_mibupload_s;

typedef struct 
{
    hi_uchar8   uc_size; 
    hi_uchar8   uc_opt;
    hi_uchar8   uc_avc;
    hi_uchar8   uc_type; 
   
    hi_ushort16 ui_version;
    hi_ushort16 us_tabnum; 

    hi_uchar8  *puc_data;
    hi_uchar8   uc_desc[HI_OMCI_ATT_INFO_MAX];
}hi_omci_mib_attr_s;

typedef struct 
{
    hi_ushort16 us_meid;
    hi_ushort16 us_instid;
    
    hi_uchar8   uc_num;
    hi_uchar8   uc_opt;
    hi_uchar8   uc_resv[2];
    
    hi_ushort16  us_mask;
    hi_ushort16  us_totalsize;

    hi_omci_mib_attr_s st_attr[HI_OMCI_MIB_MAXATTR_NUM];    
}hi_omci_mib_attrlist_s;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_OMCI_MIB_H__ */
