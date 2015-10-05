#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>

#include "atptypes.h"
#include "atpmiclisten.h"


/*Add by  yehuisheng 00183935@20111108 for 解决mic启动子进程后，子进程在accept以前异常退出，导致循环启动退出的问题。*/
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
VOS_UINT32 ATP_UTIL_MIC_ResetBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize )
{
    VOS_UINT32              ulIdx = 0;

    if( pulFdsArray != NULL && ulFdsArrSize > 0 )
    {
        for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
        {
            *(pulFdsArray + ulIdx) = 0;
        }

        return VOS_OK;
    }

    return VOS_NOK;
}

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
VOS_UINT32 ATP_UTIL_MIC_AddBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize, const VOS_UINT32 ulAddFd )
{
    VOS_UINT32              ulIdx = 0;

    if( pulFdsArray != NULL && ulFdsArrSize > 0 && ulAddFd > 0 )
    {
        for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
        {
            if( 0 == *(pulFdsArray + ulIdx) )
            {
                *(pulFdsArray + ulIdx) = ulAddFd;
                return VOS_OK;
            }
        }

        return VOS_NOK;
    }

    return VOS_NOK;
}

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
VOS_UINT32 ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection( const VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize )
{
    VOS_UINT32              ulMaxFd = 0;
    VOS_UINT32              ulIdx = 0;
    VOS_INT32               lAcceptFd = 0;
    VOS_INT32               lFind = 0;

    fd_set                  stFdset;
    struct timeval          stTimeVal;

    FD_ZERO(&stFdset);

    for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
    {
        if( *(pulFdsArray + ulIdx) > 0 )
        {
            FD_SET( *(pulFdsArray + ulIdx), &stFdset );
            ulMaxFd = ( ulMaxFd > *(pulFdsArray + ulIdx) ) ? ulMaxFd : *(pulFdsArray + ulIdx);

            lFind = 1;
        }
    }

    if( 1 == lFind )
    {
        stTimeVal.tv_sec  = 0;
        stTimeVal.tv_usec = 0;
        while( select( ulMaxFd + 1, &stFdset, NULL, NULL, &stTimeVal ) > 0 )
        {
            for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
            {
                if ( *(pulFdsArray + ulIdx) > 0 && FD_ISSET( *(pulFdsArray + ulIdx), &stFdset ) )
                {
                    lAcceptFd = accept( *(pulFdsArray + ulIdx), NULL, NULL );

                    if( lAcceptFd > 0 )
                    {
                        close( lAcceptFd );
                    }
                }
            }
        }
    }

    return VOS_OK;
}
/*Add by  yehuisheng 00183935@20111108 for 解决mic启动子进程后，子进程在accept以前异常退出，导致循环启动退出的问题。*/


