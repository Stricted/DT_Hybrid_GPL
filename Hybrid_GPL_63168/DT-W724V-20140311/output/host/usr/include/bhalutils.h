/******************************************************************************
  ��Ȩ����  : 2012-2020����Ϊ�������޹�˾
  �� �� ��  : atpfomapi.h
  ��    ��  : l00135113
  ��    ��  : 1.0
  ��������  : 2012-1-11
  ��    ��  :
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2012-1-11
     ��    ��   : l00135113
     �޸�����   : ��ɳ���

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
