/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : firewallapi.h
  ��    ��  : c00126165
  ��    ��  : ATPV100R002C03
  ��������  : 2008-7-1
  ��    ��  : 
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2009-11-24
     ��    ��   : g00121640
     �޸�����   : ��ɳ���

*********************************************************************************/


#ifndef __FWFILTER_API_H__
#define __FWFILTER_API_H__



#include "atputil.h"


#define ATP_FWFILTER_DEV_NUM_MAX    256
#define ATP_FWFILTER_TIME_NUM_MAX    16


#if defined(__cplusplus)
extern "C" {
#endif


VOS_UINT32 ATP_FILTER_Init(VOS_VOID);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif

