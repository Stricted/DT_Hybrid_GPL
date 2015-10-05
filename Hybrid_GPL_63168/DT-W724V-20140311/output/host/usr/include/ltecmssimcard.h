/******************************************************************************
  ��Ȩ����  : 2007-2008����Ϊ�������޹�˾
  �� �� ��  : wuprofile.h
  ��    ��  : l00101002
  ��    ��  : v1.0
  ��������  : 2009-1-15
  ��    ��  : 
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2009-1-15
     ��    ��   : l00101002
     �޸�����   : ��ɳ���

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
