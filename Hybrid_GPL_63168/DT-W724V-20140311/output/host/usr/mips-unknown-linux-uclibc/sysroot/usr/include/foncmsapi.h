/******************************************************************************
  版权所有  :  2012-2020，华为技术有限公司
  文 件 名  :     foncmsapi.h
  作    者  :         l00214441
  版    本  :         V1.0
  创建日期  :  2012-05-25
  描    述  :         FON 对外头文件
  函数列表  :
                             

  历史记录      :
   1.日    期   : 2012-05-25
     作    者   : l00214441
     修改内容   : 完成初稿

******************************************************************************/

#ifndef __FON_CMS_API_H__
#define __FON_CMS_API_H__


/********************** Include Files ***************************************/
#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"


/********************** Global APIs Definitions *****************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* BEGIN: add for UDK0000287751 FON: The Fon client does not calculate the TR-069 statistics parameter correctly by l00214441 2013.06.28 */  
//Stats Total
#define ATP_FON_FILE_STATS_TOTAL_SESSIONS                      "/var/fon/stats/total/sessions"
#define ATP_FON_FILE_STATS_TOTAL_USERS                         "/var/fon/stats/total/users"
#define ATP_FON_FILE_STATS_TOTAL_NAU                           "/var/fon/stats/total/nau"
#define ATP_FON_FILE_STATS_TOTAL_AF                            "/var/fon/stats/total/af"
#define ATP_FON_FILE_STATS_TOTAL_TOTALBYTESSENT                "/var/fon/stats/total/totalbytessent"
#define ATP_FON_FILE_STATS_TOTAL_TOTALBYTESRECEIVED            "/var/fon/stats/total/totalbytesreceived"

//Stats Session
#define ATP_FON_FILE_STATS_SESSION_USERSONLINE                 "/var/fon/stats/session/usersonline"
#define ATP_FON_FILE_STATS_SESSION_START                       "/var/fon/stats/session/start"
#define ATP_FON_FILE_STATS_SESSION_USERS                       "/var/fon/stats/session/users"
#define ATP_FON_FILE_STATS_SESSION_NAU                         "/var/fon/stats/session/nau"
#define ATP_FON_FILE_STATS_SESSION_AF                          "/var/fon/stats/session/af"
#define ATP_FON_FILE_STATS_SESSION_TOTALBYTESSENT              "/var/fon/stats/session/totalbytessent"
#define ATP_FON_FILE_STATS_SESSION_TOTALBYTESRECEIVED          "/var/fon/stats/session/totalbytesreceived"
/* END: add for UDK0000287751 FON: The Fon client does not calculate the TR-069 statistics parameter correctly by l00214441 2013.06.28 */  

#define ATP_FON_BRIDGE                                          "br1"


#define ATP_FON_FILE_CHILLI_CONF                         "/var/fon/chilli.conf"
#define ATP_FON_FILE_CHILLI_CONF_BACKUP          "/config/chilli.conf"
#define ATP_FON_FILE_WHITELIST                           "/var/fon/whitelist"
#define ATP_FON_FILE_WHITELIST_BACKUP             "/config/whitelist"

/* 连到FON SSID的用户数, chilli写，pppc去读*/
#define FON_SSID_USERS_NUMBER_FILE   "/var/fon/fonssid_users"

/*标志文件，当ppp是OnDemand模式下的idle掉线时建这个空文件，foncms的WAN Down消息处理收到后判断，有就不需要关FON*/
#define PPP_IDLE_DISCONNECT_FOR_FON  "/var/fon/ppp_idle_disconnect"

/*标志文件，收到了ipv6cp的reject,就告诉fon是否有v6*/
#define PPP_NO_IPV6_FOR_FON  "/var/wan/ppp256/no_ipv6"


/* 00: 2.4G Disabled   01:2.4G Enabled 10:5G Disabled 11:5G Enabled */ 
typedef enum tagATP_FON_WLAN_STATUS_EN
{
 
    ATP_FON_WLAN_24G_DISABLE = 0,
    ATP_FON_WLAN_24G_ENABLE = 1,
    ATP_FON_WLAN_5G_DISABLE = 2,
    ATP_FON_WLAN_5G_ENABLE = 3,
    ATP_FON_WLAN_DEFAULT_CHECK = 255,
}ATP_FON_WLAN_STATUS_EN;


typedef struct tagATP_FON_FILE_STAT_TOTAL_ST
{
    VOS_UINT32 ulSessions;
    VOS_UINT32 ulUsers;
    VOS_UINT32 ulAF;
}ATP_FON_FILE_STAT_TOTAL_ST;

typedef struct tagATP_FON_FILE_STAT_SESSION_ST
{
    VOS_UINT32 ulStart;
    VOS_UINT32 ulUsers;
    VOS_UINT32 ulNAU;
    VOS_UINT32 ulAF;
    VOS_UINT32 ulTotalBytesSent;
    VOS_UINT32 ulTotalBytesReceived;
}ATP_FON_FILE_STAT_SESSION_ST;

VOS_UINT32 ATP_FON_Init (VOS_VOID);
VOS_UINT32 ATP_FON_EasySupportUpdateProc(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
