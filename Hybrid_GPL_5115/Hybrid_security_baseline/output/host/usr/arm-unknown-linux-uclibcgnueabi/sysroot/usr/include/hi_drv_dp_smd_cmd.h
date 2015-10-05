/* *****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_dp_smd.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2011��8��10��
  ����޸�   :
  ��������   : SMD ģ�����ݽṹͷ�ļ�

  �����б�   :
  �޸���ʷ   :

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
    hi_v_u32 ui_arg;                 /*����*/
} hi_dp_smd_command_s;

extern uint hi_kernel_dp_smd_command(hi_dp_smd_command_s *pst_cmd);

#endif /* end of __HI_DRV_DP_XXX_H__ */

#ifdef  __cplusplus
 #if __cplusplus
}
 #endif /* end of __cplusplus  */
#endif  /* end of __cplusplus  */
