/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: base_type_define.h
  功能描述: 海思驱动基本数据类型定义头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组    

  修改记录: 
******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" 
{
#endif /*__cpluscplus*/
#endif /*__cpluscplus*/

#ifndef __BASE_TYPE_DEFINE_H__
#define __BASE_TYPE_DEFINE_H__

/*基本数据类型定义如下*/
#ifndef uchar 
#define uchar  unsigned char 
#endif

#ifndef ushort 
#define ushort unsigned short
#endif

#ifndef uint 
#define uint   unsigned int
#endif

#ifndef ulong
#define ulong   unsigned  long long
#endif


/*do not use char short int long */

#endif /*end of __BASE_TYPE_DEFINE_H__*/

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*end of __cplusplus*/
#endif  /*end of __cplusplus*/
