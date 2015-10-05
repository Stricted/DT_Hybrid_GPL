/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file sntpcmsapi.h 
  *sntpcms api \n
  *用于sntpcms的初始化，与数据库的交互
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-2-4
*/

#ifndef __SNTP_CMS_API_H__
#define __SNTP_CMS_API_H__


#include "msgapi.h"
#include "cfmapi.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief SNTPCMS的初始化，从数据库中读取信息，写入配置文件，启动sntp进程
  \param[in] VOS_VOID: 
  \return VOS_OK
*/
VOS_UINT32 ATP_SNTP_Init(VOS_VOID);

typedef enum
{
    ATP_SNTP_NOTIFY_PROC_CWMP=1,
    ATP_SNTP_NOTIFY_PROC_LOG,
    ATP_SNTP_NOTIFY_PROC_VOICE,   
    ATP_SNTP_NOTIFY_PROC_SYSINFO,
    ATP_SNTP_NOTIFY_PROC_NASCENTER,
    ATP_SNTP_NOTIFY_PROC_WLAN,
    ATP_SNTP_NOTIFY_PROC_PRIVACYBUTTON, 
    ATP_SNTP_NOTIFY_PROC_FIREWALLTIME,
    ATP_SNTP_NOTIFY_PROC_IPDR,
}ATP_SNTP_NOTIFY_PROC;

/* 定义SNTP 进入或离开夏令时，注册函数的优先级*/
typedef enum
{
    ATP_SNTP_DST_CHANGE_NOTIFY_PROC_DEFAULT=1
    
}ATP_SNTP_DST_CHANGE_NOTIFY_PROC;
/*Start of ATP 2013-12-23 for DTS2013121201481  by y00250667*/
typedef enum
{
    ATP_SNTP_WAN_PPP_IPV4 = 0,
    ATP_SNTP_WAN_PPP_IPV6,
    ATP_SNTP_WAN_LTE_IPV4,   
    ATP_SNTP_WAN_LTE_IPV6,    
}ATP_SNTP_WAN_TYPE;
/*End of ATP 2013-12-23 for DTS2013121201481  by y00250667*/

VOS_UINT32 ATP_SntpNotifyProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
/* 进入或离开夏令时Notify 注册函数*/
VOS_UINT32 ATP_SntpDstChangeNotifyProc_NotifyRegister(AtpNotifierProc pfProc,
                            VOS_UINT32 ulPriority, VOS_VOID *pvNotifyHook);


const VOS_CHAR *ATP_SNTP_GetStatus(VOS_VOID);

#if defined(__cplusplus)
}
#endif /* __cplusplus */


#endif
