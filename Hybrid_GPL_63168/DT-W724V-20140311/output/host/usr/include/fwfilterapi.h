/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : firewallapi.h
  作    者  : c00126165
  版    本  : ATPV100R002C03
  创建日期  : 2008-7-1
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-11-24
     作    者   : g00121640
     修改内容   : 完成初稿

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

