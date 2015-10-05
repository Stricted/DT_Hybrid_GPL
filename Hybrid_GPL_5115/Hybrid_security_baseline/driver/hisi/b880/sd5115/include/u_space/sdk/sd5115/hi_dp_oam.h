/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_oam.h
  版 本 号   : 初稿
  作    者   : z00209903
  生成日期   : 2012年5月17日
  最近修改   :
  功能描述   : hi dp oam SDK接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年5月17日
     作    者   : z00209903
     修改内容: 创建文件

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_DP_OAM_H__
 #define  __HI_DP_OAM_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

extern uint hi_dp_oam_attr_get(hi_dp_oam_attr_s  *pst_attr);
extern uint hi_dp_oam_attr_set(hi_dp_oam_attr_s  *pst_attr);

extern uint hi_dp_oam_sta_get(hi_dp_oam_sta_s  *pst_sta);

extern uint hi_dp_oam_cfm_table_item_get(hi_dp_oam_cfm_table_item_s  *pst_item);
extern uint hi_dp_oam_cfm_table_item_set(hi_dp_oam_cfm_table_item_s  *pst_item);

extern uint hi_dp_oam_cnt_get(hi_dp_oam_cnt_s  *pst_cnt);
extern uint hi_dp_oam_cnt_clr(void);

extern uint hi_dp_oam_alarm_attr_get(hi_dp_oam_alarm_attr_s  *pst_attr);
extern uint hi_dp_oam_alarm_attr_set(hi_dp_oam_alarm_attr_s  *pst_attr);
 #ifdef SUPPORT_OAM_1731
extern uint hi_dp_oam_txfcl_inc(void);
extern uint hi_dp_oam_1731_attr_get(hi_dp_oam_1731_attr_s  *pst_attr);
extern uint hi_dp_oam_1731_attr_set(hi_dp_oam_1731_attr_s  *pst_attr);
 #endif

#endif /* end of __HI_DP_OAM_H__*/

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus */
#endif  /* end of __cplusplus */
