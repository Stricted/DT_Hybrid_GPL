/**\file atpmiclisten.h
  * ATP软件平台公共函数头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author y00183935
  *\date  2012-04-09
*/

#ifndef __ATP_MIC_LISTEN_H__
#define __ATP_MIC_LISTEN_H__

/********************** Include Files ***************************************/
#include "atptypes.h"



/*function macro*/
#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************************************
  函数名称  : ATP_UTIL_MIC_ResetBackupAppFds
  功能描述  : 重置备份fd的数组
  输入参数  : 
              1.  *pulFdsArray: 数组首地址
              2.  ulFdsArrSize: 数组大小
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 返回VOS_OK表示命令执行成功，返回VOS_NOK表示命令执行失败


  修改历史     :
  1.日    期   : 2011-11-08
    作    者   : y00183935
    修改内容   : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_ResetBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize );


/******************************************************************************
  函数名称  : ATP_UTIL_MIC_AddBackupAppFds
  功能描述  : 添加fd到备份fd数组中
  输入参数  : 
              1.  *pulFdsArray: 数组首地址
              2.  ulFdsArrSize: 数组大小
              3. ulAddFd : 添加的fd
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 返回VOS_OK表示命令执行成功，返回VOS_NOK表示命令执行失败


  修改历史     :
  1.日    期   : 2011-11-08
    作    者   : y00183935
    修改内容   : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_AddBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize, const VOS_UINT32 ulAddFd );


/******************************************************************************
  函数名称  : ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection
  功能描述  : 关闭备份fd数组中请求的连接
  输入参数  : 
              1.  *pulFdsArray: 数组首地址
              2.  ulFdsArrSize: 数组大小
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 返回VOS_OK表示命令执行成功，返回VOS_NOK表示命令执行失败


  修改历史     :
  1.日    期   : 2011-11-08
    作    者   : y00183935
    修改内容   : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection( const VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize );



#if defined(__cplusplus)
}
#endif

#endif

