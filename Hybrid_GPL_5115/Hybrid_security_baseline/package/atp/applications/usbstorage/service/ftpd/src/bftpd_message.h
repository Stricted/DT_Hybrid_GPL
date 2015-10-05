/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名       : bftpd_message.h
  版 本 号       : 初稿
  作    者           : y00183935
  生成日期   : 2012年6月25日
  最近修改   :
  功能描述   : bftpd_message.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期        : 2012年6月25日
    作    者         : y00183935
    修改内容 : 创建文件

******************************************************************************/
#ifndef __BFTPD_MESSAGE_H__
#define __BFTPD_MESSAGE_H__
#include "atptypes.h"
#include "msgapi.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


VOS_UINT32 ATP_BFTPD_CHILD_Init( );

#ifndef SUPPORT_ATP_FTP_UPGRADE
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN eOperate, ATP_FTPSERVERCMS_IPTYPE_EN eIPType, VOS_UINT32 iPort );
#endif
#ifdef SUPPORT_ATP_FTP_UPGRADE
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_DOUPGRADE( );
#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



#endif  //end #ifndef __BFTPD_MESSAGE_H__


