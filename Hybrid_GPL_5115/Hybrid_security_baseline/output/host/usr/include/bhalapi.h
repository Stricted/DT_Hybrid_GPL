/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : bhalapi.h
  作    者  : l69021
  版    本  : 1.0
  创建日期  : 2007-6-30
  描    述  :
  函数列表  :

  历史记录      :
   1.日    期   : 2007-6-30
     作    者   : l69021
     修改内容   : 完成初稿

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
