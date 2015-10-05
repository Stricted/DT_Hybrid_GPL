/**\file wlancmsmsgtypes.h
  *WLANCMS需要发送的消息
  *\copyright 2007-2020，华为技术有限公司
  *\author z00175982
  *\date  2012-2-28
*/

#ifndef __ATP_WLANCMS_MSGTYPES_H__
#define __ATP_WLANCMS_MSGTYPES_H__

#define ATP_CBBID_WLANCMS_NAME "wlancms"

enum ATP_WLANCMS_MSGTYPES_EN
{
    ATP_MSG_WLANCMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_WLANCMS),
    ATP_MSG_WLANCMS_GET_WPS_PIN,
    ATP_MSG_MONITOR_WPS_CHG,
#ifdef SUPPORT_ATP_FON    
    ATP_MSG_WLANCMS_FONSSID_CTL,
#endif	
#ifdef SUPPORT_ATP_WPS
    ATP_MSG_WLANCMS_STOP_WPS, // ADD by y00181549 for UDK0000340477,在结束wps时同时终止dect对码流程 at 2014.2.14
#endif 
};

enum ATP_WLAN_SERVICE_TYPE_EN
{
    ATP_WLAN_SERVICE = ATP_MSG_CATEGORY_DEF(ATP_MODULE_WLANCMS) + 100,
};

enum ATP_WLAN_STATUS_EN
{
    ATP_WLAN_UP_STATUS,
    ATP_WLAN_DOWN_STATUS,
};

#endif
