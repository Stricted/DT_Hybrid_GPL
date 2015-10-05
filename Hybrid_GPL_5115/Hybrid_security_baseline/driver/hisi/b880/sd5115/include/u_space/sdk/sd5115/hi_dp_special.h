/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_special.h
  版 本 号   : 初稿
  作    者   : f00125610
  生成日期   : 2012年5月17日
  最近修改   :
  功能描述   : hi dp special SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年5月17日
     作    者   : x00214067
     修改内容: 创建文件

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_SPECIAL_H__
#define  __HI_DP_SPECIAL_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_special_pkt_attr_get(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint hi_dp_special_pkt_attr_set(hi_dp_special_pkt_attr_s  *pst_attr);
extern uint hi_dp_special_pkt_cnt_get(hi_dp_special_pkt_cnt_s  *pst_cnt);
extern uint hi_dp_special_pkt_cnt_clr( void );

extern uint hi_dp_special_pkt_trans_table_item_set(hi_dp_special_pkt_trans_table_item_s *pst_item);
extern uint hi_dp_special_pkt_trans_table_item_get(hi_dp_special_pkt_trans_table_item_s *pst_item);

#endif /* end of __HI_DP_L3_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
