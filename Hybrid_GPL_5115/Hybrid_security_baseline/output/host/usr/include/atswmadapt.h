/*--------------------------------------------------------------------------------------------
  版权信息 : 版权所有(C) 2010-2013, 华为技术有限公司.
  文件名   : atswmadapt.c
  作者     : wangliang
  版本     : 
  创建日期 : 2013-06-07
  完成日期 : 
  功能描述 : 固定台无线atserver模块适配文件头文件
----------------------------------------------------------------------------------------------*/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>

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
