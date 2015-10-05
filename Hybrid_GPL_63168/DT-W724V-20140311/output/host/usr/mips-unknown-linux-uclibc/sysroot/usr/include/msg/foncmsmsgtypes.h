/**\file foncmsmsgtypes.h
  *FONCMS��Ҫ���͵���Ϣ
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author l00214441
  *\date  2012-2-28
*/

#ifndef __ATP_FONCMS_MSGTYPES_H__
#define __ATP_FONCMS_MSGTYPES_H__

typedef enum tagATP_FONCMS_MSGTYPES_EN
{
    ATP_MSG_FONCMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_FONCMS),
    ATP_MSG_FONCMS_WLAN_UPDATE,
    ATP_MSG_FONCMS_WEB_LOGOUT,
    ATP_MSG_FONCMS_RELOAD_CONFIG,
    ATP_MSG_FONCMS_RADIUS_FINISH,
}ATP_FONCMS_MSGTYPES_EN;

#endif
