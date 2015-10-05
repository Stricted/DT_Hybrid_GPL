/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾                    

 ******************************************************************************
  �� �� ��   : hi_kernel_napt_api.h
  �� �� ��   : ����
  ��    ��   : yubo 00186361
  ��������   : 2011-9-22
  ��������   : NAPT�ṩ��API�ӿ�
******************************************************************************/
#ifndef __HI_KERNEL_NAPT_API_H__
#define __HI_KERNEL_NAPT_API_H__

/***************************************************************************** 
 *                                INCLUDE                                    * 
 *****************************************************************************/
#include <linux/list.h>
#include <linux/netdevice.h>
#include "hi_typedef.h"
#include "hi_sysdef.h"
#include "hi_kernel_log.h"
#include "hi_logdef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************************************************************** 
 *                                DEFINE                                     * 
 *****************************************************************************/
/* ϵͳ������Ĭ��ֵ*/
#define HI_KERNEL_NAPT_DFT_SAMPLE_TIME          (10)
#define HI_KERNEL_NAPT_DFT_PACKETS_THREADHOLD   (2000)
#define HI_KERNEL_NAPT_DFT_BYTES_THREADHOLD     (2 * 1024 * 1024)
#define HI_KERNEL_NAPT_DFT_HW_ENABLE            (HI_TRUE)
#define HI_KERNEL_NAPT_DFT_HW_TIMEOUT           (180)
#define HI_KERNEL_NAPT_DFT_SW_TIMEOUT           (180)

#define HI_KERNEL_NATP_INVALID_PORT             (0xFF)


#define HI_KERNEL_NAPT_MAX_WANDEV_NAME_LEN      16



#if 0
#define HI_KERNEL_NAPT_DEBUG(fmt,arg...)  \
            hi_kernel_print(HI_SUBMODULE_KNETWORKS_ACCEL, HI_LOG_LEVEL_INFO_E, (hi_uchar8*)"[HI_KERNEL_NAPT] "fmt, ##arg)

#define HI_KERNEL_NAPT_DUMP(fmt,arg...)   \
            printk((hi_char8*)fmt, ##arg)

#define HI_KERNEL_NAPT_PRINT(fmt,arg...)  \
            hi_kernel_print(HI_SUBMODULE_KNETWORKS_ACCEL, HI_LOG_LEVEL_INFO_E, (hi_uchar8*)fmt, ##arg)

#define HI_KERNEL_NAPT_DEBUG_MEM(data, len, fmt, arg...) \
            hi_kernel_log_memdes(HI_SUBMODULE_KNETWORKS_ACCEL, HI_LOG_LEVEL_INFO_E, data, len, (hi_uchar8 *)"[HI_KERNEL_NAPT] "fmt, ##arg)
#else
#define HI_KERNEL_NAPT_DEBUG(fmt,arg...)  
#define HI_KERNEL_NAPT_DUMP(fmt,arg...)  
#define HI_KERNEL_NAPT_PRINT(fmt,arg...)  
#define HI_KERNEL_NAPT_DEBUG_MEM(data, len, fmt, arg...) 
#endif


/***************************************************************************** 
 *                                TYPEDEF                                    * 
 *****************************************************************************/
/*NAT����*/
typedef enum
{
    HI_KERNEL_NAPT_TYPE_NONE_E = 0,
    HI_KERNEL_NAPT_TYPE_SNAT_E = 1,
    HI_KERNEL_NAPT_TYPE_DNAT_E = 2,
    HI_KERNEL_NAPT_TYPE_MAX_E
} HI_KERNEL_NAPT_TYPE_E;

/*��������*/
typedef enum
{
    HI_KERNEL_NAPT_CONN_NO_E   = 0x0,
    HI_KERNEL_NAPT_CONN_SW_E   = 0x1,
    HI_KERNEL_NAPT_CONN_HW_E   = 0x2,
    HI_KERNEL_NAPT_CONN_HWSW_E = 0x3,
} HI_KERNEL_NAPT_CONN_ATTR_E;

/*�豸����1*/
typedef enum 
{
    HI_KERNEL_NAPT_DEV_NO_E    = 0x0,
    HI_KERNEL_NAPT_DEV_SW_E    = 0x1,
    HI_KERNEL_NAPT_DEV_HW_E    = 0x2,
    HI_KERNEL_NAPT_DEV_HWSW_E  = 0x3,
} HI_KERNEL_NAPT_DEV_ATTR1_E;

/*�豸����2*/
typedef enum
{
    HI_KERNEL_NAPT_DEV_ETH_E   = 0x0,
    HI_KERNEL_NAPT_DEV_PPP_E   = 0x1,
} HI_KERNEL_NAPT_DEV_ATTR2_E;

/*�豸����3*/
typedef enum
{
    HI_KERNEL_NAPT_DEV_NOLEARN = 0x0,
    HI_KERNEL_NAPT_DEV_LEARN   = 0x1,
} HI_KERNEL_NAPT_DEV_ATTR3_E;

/*�����豸����*/
typedef struct hi_kernel_napt_dev_cmdtype
{
    hi_char8  ac_ifname[HI_KERNEL_NAPT_MAX_WANDEV_NAME_LEN];
    hi_uint32 ui_flags;
} hi_kernel_napt_dev_cmdtype_s;

/*WAN�ӿ�IP��ַ*/
typedef struct hi_kernel_napt_wanip_comtype
{
    hi_uint32 ui_index;
    hi_uint32 ui_ip;
} hi_kernel_napt_wanip_cmdtype_s;

/*4����Ϣ*/
typedef struct hi_kernel_napt_l4info
{
    hi_ushort16 us_sport;                       /*ԭport*/
    hi_ushort16 us_dport;                       /*Ŀ��port*/
} hi_kernel_napt_l4info_s;

/*3����Ϣ*/
typedef struct hi_kernel_napt_l3info
{
    hi_uint32 ui_sip;                           /*ԴIP*/
    hi_uint32 ui_dip;                           /*Ŀ��IP*/
} hi_kernel_napt_l3info_s;

/*2����Ϣ*/
typedef struct hi_kernel_napt_l2repinfo
{
    hi_uchar8   auc_smac[HI_MAC_LEN];                    /*ԭMAC*/
    hi_uchar8   auc_dmac[HI_MAC_LEN];                    /*Ŀ��MAC*/
    hi_int32    i_ifindex;                      /*��ӿ�����*/
    hi_ushort16 us_sid;                         /*��ӿڶ�Ӧ��session_id*/
} hi_kernel_napt_l2info_s;

/*tuple��Ϣ*/
typedef struct hi_kernel_napt_tuple
{
    hi_kernel_napt_l4info_s st_l4info;
    hi_kernel_napt_l3info_s st_l3info;
    hi_kernel_napt_l2info_s st_l2info;
    hi_uchar8               uc_proto;           /*4��Э���TCP/UDP*/
    hi_uint32               ui_dir;
} hi_kernel_napt_tuple_s;

/*���ڱ�ʾtuple�ķ���*/
typedef enum HI_KERNEL_NAPT_DIR
{
    HI_KERNEL_NAPT_DIR_UP_E = 0,
    HI_KERNEL_NAPT_DIR_DN_E = 1,
    HI_KERNEL_NAPT_DIR_MAX_E
} HI_KERNEL_NAPT_DIR_E;

/*���ñ���*/
typedef struct hi_kernel_napt_item
{
    hi_kernel_napt_tuple_s  st_tuple[HI_KERNEL_NAPT_DIR_MAX_E]; /* �����б���2��3��4����Ϣ */
    hi_uint32               ui_nattype;                         /* NAT������ */
    hi_uint32               ui_skbmark; 
} hi_kernel_napt_item_s;


/*�����ؼ���*/
typedef struct 
{
    hi_kernel_napt_l4info_s st_l4info;
    hi_kernel_napt_l3info_s st_l3info;
    hi_uchar8               uc_proto;
} hi_kernel_napt_key_s;

/*IO����*/
typedef struct 
{
    hi_kernel_napt_item_s st_item;     /* ���ñ��� */
    hi_kernel_napt_key_s  st_key;
    hi_uint32             ui_alive;
} hi_kernel_napt_para_s;


typedef struct
{
    hi_uchar8 uc_ifname[HI_KERNEL_NAPT_MAX_WANDEV_NAME_LEN]; 
    hi_uint32 ui_phyport; 
}hi_kernel_napt_wandev_s; 


/***************************************************************************** 
 *                                FUNCTION                                   * 
 *****************************************************************************/
/******************************************************************************
 �� �� ��  : hi_kernel_napt_entry_set
 ��������  : ����NAPT����
 �������  : ui_cmdtype 
             pv_data
             ui_inlen
             pui_outlen
 �������  : ��
 �� �� ֵ  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_set(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *ui_outlen);

/******************************************************************************
 �� �� ��  : hi_kernel_napt_entry_get
 ��������  : ����tuple��Ϣ��ȡNAPT����
 �������  : pv_data
             ui_inlen
             pui_outlen
 �������  : ��
 �� �� ֵ  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_get(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen);

/******************************************************************************
 �� �� ��  : hi_kernel_napt_entry_delete
 ��������  : ����tuple��Ϣɾ��NAPT����
 �������  : pv_data
             ui_inlen
             pui_outlen
 �������  : ��
 �� �� ֵ  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_del(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen);

/******************************************************************************
 �� �� ��  : hi_kernel_napt_api_dump
 ��������  : ��ӡNAPT�������Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_api_dump(hi_void);


extern hi_void hi_kernel_accel_napt_delall(hi_uint32 ui_flag); 


extern hi_void hi_kernel_accel_napt_interface_down(hi_int32 i_ifindex); 

/******************************************************************************
 �� �� ��  : hi_kernel_napt_api_init
 ��������  : ��װAPI�ӿ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_api_init(hi_void);

/******************************************************************************
 �� �� ��  : hi_kernel_napt_api_exit
 ��������  : ж��API�ӿ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
******************************************************************************/
hi_void hi_kernel_napt_api_exit(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_KERNEL_NAPT_API_H__*/

