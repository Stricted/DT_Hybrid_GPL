/******************************************************************************
  版权所有  : 2007-2008，华为技术有限公司
  文 件 名  : wuprofile.h
  作    者  : l00101002
  版    本  : v1.0
  创建日期  : 2009-1-15
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-1-15
     作    者   : l00101002
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_SIMCARD_API_H__
#define __ATP_SIMCARD_API_H__

#include "atputil.h"

#define SIM_READY             "Valid"
#define PIN_REQUIRED      "Invalid"
#define PUK_REQUIRED     "PUKRequired"
#define SIM_INIT                  "NotInitialized"
#define SIM_NO_CARD      "NoCard"
#define PIN_FAILED             "Error"

#define PIN_Enabled          "Pin Enabled"
#define PIN_DESABLED      "Pin Disabled"

#define LTE_SIM_STR_LEN 32
#define LTE_SIM_CMD_LEN 128
#define LTE_SIM_TMP_RSLT_FILE "/var/lte_tmp_result"
#define LTE_SIM_RSLT_FILE_OPEN_TIMES 5
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif
