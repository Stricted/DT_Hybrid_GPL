/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司                    

 ******************************************************************************
  文 件 名   : hi_kernel_napt_api.h
  版 本 号   : 初稿
  作    者   : yubo 00186361
  生成日期   : 2011-9-22
  功能描述   : NAPT提供的API接口
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
/* 系统启动的默认值*/
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
/*NAT类型*/
typedef enum
{
    HI_KERNEL_NAPT_TYPE_NONE_E = 0,
    HI_KERNEL_NAPT_TYPE_SNAT_E = 1,
    HI_KERNEL_NAPT_TYPE_DNAT_E = 2,
    HI_KERNEL_NAPT_TYPE_MAX_E
} HI_KERNEL_NAPT_TYPE_E;

/*连接属性*/
typedef enum
{
    HI_KERNEL_NAPT_CONN_NO_E   = 0x0,
    HI_KERNEL_NAPT_CONN_SW_E   = 0x1,
    HI_KERNEL_NAPT_CONN_HW_E   = 0x2,
    HI_KERNEL_NAPT_CONN_HWSW_E = 0x3,
} HI_KERNEL_NAPT_CONN_ATTR_E;

/*设备属性1*/
typedef enum 
{
    HI_KERNEL_NAPT_DEV_NO_E    = 0x0,
    HI_KERNEL_NAPT_DEV_SW_E    = 0x1,
    HI_KERNEL_NAPT_DEV_HW_E    = 0x2,
    HI_KERNEL_NAPT_DEV_HWSW_E  = 0x3,
} HI_KERNEL_NAPT_DEV_ATTR1_E;

/*设备属性2*/
typedef enum
{
    HI_KERNEL_NAPT_DEV_ETH_E   = 0x0,
    HI_KERNEL_NAPT_DEV_PPP_E   = 0x1,
} HI_KERNEL_NAPT_DEV_ATTR2_E;

/*设备属性3*/
typedef enum
{
    HI_KERNEL_NAPT_DEV_NOLEARN = 0x0,
    HI_KERNEL_NAPT_DEV_LEARN   = 0x1,
} HI_KERNEL_NAPT_DEV_ATTR3_E;

/*网络设备属性*/
typedef struct hi_kernel_napt_dev_cmdtype
{
    hi_char8  ac_ifname[HI_KERNEL_NAPT_MAX_WANDEV_NAME_LEN];
    hi_uint32 ui_flags;
} hi_kernel_napt_dev_cmdtype_s;

/*WAN接口IP地址*/
typedef struct hi_kernel_napt_wanip_comtype
{
    hi_uint32 ui_index;
    hi_uint32 ui_ip;
} hi_kernel_napt_wanip_cmdtype_s;

/*4层信息*/
typedef struct hi_kernel_napt_l4info
{
    hi_ushort16 us_sport;                       /*原port*/
    hi_ushort16 us_dport;                       /*目的port*/
} hi_kernel_napt_l4info_s;

/*3层信息*/
typedef struct hi_kernel_napt_l3info
{
    hi_uint32 ui_sip;                           /*源IP*/
    hi_uint32 ui_dip;                           /*目的IP*/
} hi_kernel_napt_l3info_s;

/*2层信息*/
typedef struct hi_kernel_napt_l2repinfo
{
    hi_uchar8   auc_smac[HI_MAC_LEN];                    /*原MAC*/
    hi_uchar8   auc_dmac[HI_MAC_LEN];                    /*目的MAC*/
    hi_int32    i_ifindex;                      /*入接口索引*/
    hi_ushort16 us_sid;                         /*入接口对应的session_id*/
} hi_kernel_napt_l2info_s;

/*tuple信息*/
typedef struct hi_kernel_napt_tuple
{
    hi_kernel_napt_l4info_s st_l4info;
    hi_kernel_napt_l3info_s st_l3info;
    hi_kernel_napt_l2info_s st_l2info;
    hi_uchar8               uc_proto;           /*4层协议号TCP/UDP*/
    hi_uint32               ui_dir;
} hi_kernel_napt_tuple_s;

/*用于表示tuple的方向*/
typedef enum HI_KERNEL_NAPT_DIR
{
    HI_KERNEL_NAPT_DIR_UP_E = 0,
    HI_KERNEL_NAPT_DIR_DN_E = 1,
    HI_KERNEL_NAPT_DIR_MAX_E
} HI_KERNEL_NAPT_DIR_E;

/*配置表项*/
typedef struct hi_kernel_napt_item
{
    hi_kernel_napt_tuple_s  st_tuple[HI_KERNEL_NAPT_DIR_MAX_E]; /* 上下行报文2、3、4层信息 */
    hi_uint32               ui_nattype;                         /* NAT的类型 */
    hi_uint32               ui_skbmark; 
} hi_kernel_napt_item_s;


/*索引关键字*/
typedef struct 
{
    hi_kernel_napt_l4info_s st_l4info;
    hi_kernel_napt_l3info_s st_l3info;
    hi_uchar8               uc_proto;
} hi_kernel_napt_key_s;

/*IO参数*/
typedef struct 
{
    hi_kernel_napt_item_s st_item;     /* 配置表项 */
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
 函 数 名  : hi_kernel_napt_entry_set
 功能描述  : 设置NAPT表项
 输入参数  : ui_cmdtype 
             pv_data
             ui_inlen
             pui_outlen
 输出参数  : 无
 返 回 值  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_set(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *ui_outlen);

/******************************************************************************
 函 数 名  : hi_kernel_napt_entry_get
 功能描述  : 根据tuple信息获取NAPT表项
 输入参数  : pv_data
             ui_inlen
             pui_outlen
 输出参数  : 无
 返 回 值  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_get(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen);

/******************************************************************************
 函 数 名  : hi_kernel_napt_entry_delete
 功能描述  : 根据tuple信息删除NAPT表项
 输入参数  : pv_data
             ui_inlen
             pui_outlen
 输出参数  : 无
 返 回 值  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_entry_del(hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen);

/******************************************************************************
 函 数 名  : hi_kernel_napt_api_dump
 功能描述  : 打印NAPT表项的信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_api_dump(hi_void);


extern hi_void hi_kernel_accel_napt_delall(hi_uint32 ui_flag); 


extern hi_void hi_kernel_accel_napt_interface_down(hi_int32 i_ifindex); 

/******************************************************************************
 函 数 名  : hi_kernel_napt_api_init
 功能描述  : 安装API接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : HI_RET_SUCC/HI_RET_FAIL
******************************************************************************/
hi_uint32 hi_kernel_napt_api_init(hi_void);

/******************************************************************************
 函 数 名  : hi_kernel_napt_api_exit
 功能描述  : 卸载API接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
******************************************************************************/
hi_void hi_kernel_napt_api_exit(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_KERNEL_NAPT_API_H__*/

