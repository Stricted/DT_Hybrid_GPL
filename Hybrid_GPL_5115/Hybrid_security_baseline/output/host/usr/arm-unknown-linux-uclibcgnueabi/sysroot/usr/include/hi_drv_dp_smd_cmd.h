/* *****************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_dp_smd.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2011年8月10日
  最近修改   :
  功能描述   : SMD 模块数据结构头文件

  函数列表   :
  修改历史   :

***************************************************************************** */

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif/* __cpluscplus  */
#endif /* __cpluscplus  */

#ifndef __HI_DP_SMD_CMD_H__

 #define    __HI_DP_SMD_CMD_H__

 #include "hi_drv_common.h"

typedef struct
{
    hi_v_u32 ui_command;
    hi_v_u32 ui_position;
    hi_v_u32 ui_arg;                 /*参数*/
} hi_dp_smd_command_s;

extern uint hi_kernel_dp_smd_command(hi_dp_smd_command_s *pst_cmd);

#endif /* end of __HI_DRV_DP_XXX_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
