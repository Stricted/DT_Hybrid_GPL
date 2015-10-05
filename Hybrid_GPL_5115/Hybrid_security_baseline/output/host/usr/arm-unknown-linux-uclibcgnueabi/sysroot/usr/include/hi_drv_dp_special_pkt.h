/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_drv_dp_special_pkt.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2009年2月3日
  最近修改   :
  功能描述   : SPECIAL PKT模块数据结构头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年2月3日
    作    者   : f00125610
    修改内容   : 创建文件

***************************************************************************** */


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus  */


#ifndef __HI_DRV_DP_SPECIAL_PKT_H__
#define __HI_DRV_DP_SPECIAL_PKT_H__


#include "hi_drv_common.h"
#include "hi_drv_common_dp.h"

 
/* 不支持CAR抑制的特殊报文的种类数目 */
#define HI_DP_UNSUPPORT_CAR_NUM  4
/* 支持CAR抑制的特殊报文的种类数目 */
#define HI_DP_SUPPORT_CAR_NUM    12
 
#define HI_DP_POINT_TYPE_NUM  4
 #define HI_DP_PPPOE_POINT_TYPE_NUM  3

 
typedef struct  
{

    /* 4 种可指定的特殊报文定义 */
    uint    ui_point_type[4];

    /* 3种自定义PPPoE承载上层协议类型 */
    uint    ui_pppoe_point_type[HI_DP_PPPOE_POINT_TYPE_NUM];
    
}hi_dp_special_pkt_attr_s;

 typedef struct
{
    HI_DP_SPECIAL_PKT_TYPE_E    em_type;    /* 报文类型，作为查表索引 */

    HI_BOOL_E    em_security_en;     /* 是否参加安全模块的smac和入口vlan检查 */
    HI_BOOL_E    em_high_arb;        /* 高优先处理 */
    HI_BOOL_E    em_car_en[HI_DP_ETH_PORT_NUM + 1];   /* 以太口+PON口，报文是否参与car */ 
#ifdef  CONFIG_HISI_SD5115_TYPE_T
    HI_DP_TRANS_ACT_E  em_act[HI_DP_ETH_PORT_NUM + 2];   /* 以太口+PON+CPU，报文转发行为 */
#else
    HI_DP_TRANS_ACT_E  em_act[HI_DP_ETH_PORT_NUM + 1];   /* 以太口+PON，报文转发行为 */
#endif
}hi_dp_special_pkt_trans_table_item_s;
 

typedef struct  
{
    uint ui_drop;               /* 丢弃的报文个数统计                    */
    uint ui_tocpu;              /* 捕获给CPU的报文个数统计               */
    uint ui_thru;               /* 透传计数  */
   uint cnt[HI_DP_CAP_PKT_CFG_SIZE];


}hi_dp_special_pkt_cnt_s;



#endif /* __HI_DRV_DP_SPECIAL_PKT_H__  */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus  */



