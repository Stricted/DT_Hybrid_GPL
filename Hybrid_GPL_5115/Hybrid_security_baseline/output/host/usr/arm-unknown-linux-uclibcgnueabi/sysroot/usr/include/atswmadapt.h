/*--------------------------------------------------------------------------------------------
  ��Ȩ��Ϣ : ��Ȩ����(C) 2010-2013, ��Ϊ�������޹�˾.
  �ļ���   : atswmadapt.c
  ����     : wangliang
  �汾     : 
  �������� : 2013-06-07
  ������� : 
  �������� : �̶�̨����atserverģ�������ļ�ͷ�ļ�
----------------------------------------------------------------------------------------------*/
/*******************************************************************************
Edit History of Huawei: 
<�޸���>     <�޸�ʱ��>    <���ⵥ��>          <�޸�����>

*******************************************************************************/

#ifndef __ATSWMADAPT_H
#define __ATSWMADAPT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "atptypes.h"
#include "at_lib.h"
#include "at_if.h"

VOS_INT32 AtsPcuiReady(void);
VOS_INT32 AtServerWmInit();
VOS_INT32 AtServerWmUnInit();
VOS_VOID AtsResetWm();
VOS_INT32 AtsSendCmdCheck(ATS_MSG_ST  *pstMsg,VOS_INT32 *lRetCode);
void write_pin_info(PinMgResultCodeEx *info);
VOS_INT32 AtsSmsWriteToUsb( ATS_MSG_ST *pstAtsMsg, VOS_INT32 *lRetCode);
VOS_INT32 AtsOtherReportProc(const VOS_CHAR *pszAppName, ATS_MSG_ST *pstMsg);
VOS_INT32 AtsWmInit();
VOS_INT32 AtsMsgReqProc(VOS_CHAR m_aucMsgSrc[],ATS_MSG_ST stMsg);

#if defined(__cplusplus)
}
#endif

#endif
