/******************************************************************************

                  版权所有 (C), 2009-2011, 华为技术有限公司

  文 件 名   : bftpd_message.c
  版 本 号   : V1.0
  作    者   : y00183935
  生成日期   : 2012年6月26日
  最近修改   :
  功能描述   : Bftpd向ftpservercms等模块发消息
  函数列表   :
  
  修改历史   :
    日    期   : 2012年6月26日
    作    者   : y00183935
    修改内容   : 创建文件

******************************************************************************/
/*include 头文件*/
#include <stdio.h>
#include <string.h>
#include "atputil.h"
#include "bftpd_message.h"
#include "msgapi.h"


/******************************************************************************
  函数名称  : ATP_BFTPD_CHILD_Init
  功能描述  : 初始化BFTPD 子进程的MSG 消息
  输入参数  : 无
  
  调用函数  : 
  被调函数  : 提供给 BFTPD 调用
  输出参数  : 无
  返 回 值  : VOS_OK成功
  			     VOS_ERROR失败
  
  修改历史     :
  1.日    期          : 2012年6月26日
     作    者          : y00183935
     修改内容  : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_Init( )
{
    VOS_UINT32 ulRet = VOS_NOK;

    ATP_MSG_Destroy();
    ulRet = ATP_MSG_Init( VOS_NULL );

    return ulRet;
}


#ifndef SUPPORT_ATP_FTP_UPGRADE

/******************************************************************************
  函数名称  : ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg
  功能描述  : 发送处理ACL 开启和关闭数据端口的消息给ftpservercms
  输入参数  : 
         1. eOperate : 打开、关闭操作
         2. eIPType   : IP地址类型
         3. iPort        : 端口号
  
  调用函数  : 
  被调函数  : 提供给 BFTPD 调用
  输出参数  : 无
  返 回 值  : VOS_OK成功
  			     VOS_ERROR失败
  
  修改历史     :
  1.日    期          : 2012年6月26日
     作    者          : y00183935
     修改内容  : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN eOperate, ATP_FTPSERVERCMS_IPTYPE_EN eIPType, VOS_UINT32 iPort )
{
    VOS_UINT32 ulRet = VOS_NOK;

    if( iPort > 0 )
    {
#if 0 //modify by y00181549 at 20130218 for DTS2013021800116 在ftp上传下载工程中，拔插硬盘，一段时间后，会出现CMS堵塞情况   
        ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
        ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;

        VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = "";
        VOS_CHAR    *pszTmp =  VOS_NULL;

        pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
        ATP_MSG_SimpleBuilder( pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_FTPSERVERCMS_DEAL_ACL_DATAPORT, 0 );

        pszTmp = (VOS_CHAR *)( aucBuf + sizeof( ATP_MSG_HEADER_ST ) );
        snprintf( pszTmp , ATP_ACTUAL_LEN_512, "%d %d %d", eOperate, eIPType, iPort );
        pstMsg->ulDataLen = strlen( pszTmp ) + 1;

        pstResp = NULL;
        ulRet = ATP_MSG_SendAndRecv( pstMsg, &pstResp, 2*1000 );

        if( VOS_OK == ulRet && VOS_NULL != pstResp )
        {
            ulRet = (pstResp->ulMsgData);
            free((VOS_VOID *)pstResp);
        }
#else
        VOS_UINT8   aucBuf[1024] = "";

        snprintf( aucBuf , sizeof(aucBuf), "%d %d %d", eOperate, eIPType, iPort );


        ATP_MSG_SendMsgData("cms", ATP_MSG_FTPSERVERCMS_DEAL_ACL_DATAPORT, (VOS_VOID*)aucBuf, strlen( aucBuf ) + 1);
#endif
    }

    return ulRet;
}
#endif

#ifdef SUPPORT_ATP_FTP_UPGRADE

/******************************************************************************
  函数名称  : ATP_BFTPD_CHILD_SendRecv_DOUPGRADE
  功能描述  : 
  输入参数  : 
         1. eOperate : 打开、关闭操作
         2. eIPType   : IP地址类型
         3. iPort        : 端口号
  
  调用函数  : 
  被调函数  : 提供给 BFTPD 调用
  输出参数  : 无
  返 回 值  : VOS_OK成功
  			     VOS_ERROR失败
  
  修改历史     :
  1.日    期          : 2012年6月26日
     作    者          : y00183935
     修改内容  : 新生成函数
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_DOUPGRADE( VOS_CHAR * acImagePath )
{
    VOS_UINT32 ulRet = VOS_NOK;

    if( strlen(acImagePath) > 0 )
    {
        ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
        ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;

        VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = "";
        VOS_CHAR    *pszTmp =  VOS_NULL;

        pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
        ATP_MSG_SimpleBuilder( pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_FTPUPGCMS_DOUPGRADE, 0 );

        pszTmp = (VOS_CHAR *)( aucBuf + sizeof( ATP_MSG_HEADER_ST ) );
        snprintf( pszTmp , ATP_ACTUAL_LEN_512, "%s", acImagePath);
        pstMsg->ulDataLen = strlen( pszTmp ) + 1;

        pstResp = NULL;
        ulRet = ATP_MSG_SendAndRecv( pstMsg, &pstResp, 60*1000 );

        if( VOS_OK == ulRet && VOS_NULL != pstResp )
        {
            ulRet = (pstResp->ulMsgData);
            free((VOS_VOID *)pstResp);
        }
    }

    return ulRet;
}

#endif

