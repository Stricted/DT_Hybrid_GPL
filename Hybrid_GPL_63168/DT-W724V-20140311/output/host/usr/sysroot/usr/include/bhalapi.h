/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : bhalapi.h
  ��    ��  : l69021
  ��    ��  : 1.0
  ��������  : 2007-6-30
  ��    ��  :
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-6-30
     ��    ��   : l69021
     �޸�����   : ��ɳ���

*********************************************************************************/
#ifndef _BHALAPI_H_
#define _BHALAPI_H_

#include "bhalutils.h"
#include "atpfomapi.h"
#include "atpflashapi.h"


#if defined(__cplusplus)
extern "C" {
#endif


#define BHAL_NULL       ""
#define BHAL_ZERO       0
#define BHAL_ERR        -1

#ifdef ATP_DEBUG
#define ATP_FOM_DEBUG(format, args...)       {printf("\nFILE:%s,LINE:%d,FUNC:%s: ", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("\n");}
#else
#define ATP_FOM_DEBUG(x...)
#endif






#if defined(__cplusplus)
}
#endif

#endif /* _BHALAPI_H_ */
