/******************************************************************************
  版权所有  : 2012-2020，华为技术有限公司
  文 件 名  : atpfomapi.h
  作    者  : l00135113
  版    本  : 1.0
  创建日期  : 2012-1-11
  描    述  :
  函数列表  :

  历史记录      :
   1.日    期   : 2012-1-11
     作    者   : l00135113
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef _BHALUTILS_H_
#define _BHALUTILS_H_

#include "atptypes.h"


#if defined(__cplusplus)
extern "C" {
#endif

/*FOM:Flash Operation Master*/



/****************************Function Prototype********************************/
extern VOS_INT32 bhalIoctl(VOS_UINT32 ulBhalioctl, VOS_UINT32 ulType, VOS_INT8 *pcString,
                        VOS_UINT32 ulStrLen, VOS_UINT32 ulOffset, VOS_INT8 *pcBuf);


#if defined(__cplusplus)
}
#endif

#endif /* _BHALUTILS_H_ */
