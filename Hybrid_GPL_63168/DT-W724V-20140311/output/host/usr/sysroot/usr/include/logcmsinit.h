/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file logcmsinit.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef __LOG_CMS_INIT_H__
#define __LOG_CMS_INIT_H__

#define ATP_CBBID_KLOG_NAME         "klog"
#define ATP_CBBID_IGMP_PROXY_NAME   "igmpproxy"
#ifndef ATP_CBBID_UPNP_NAME
#define ATP_CBBID_UPNP_NAME         "upnp"
#endif
#ifndef ATP_CBBID_VOICE_NAME
#define ATP_CBBID_VOICE_NAME         "voice"
#endif


extern VOS_UINT32 logMergeLogLevel(const VOS_CHAR* pszLevel, VOS_UINT32* pulLevelOut);

#endif //__LOG_CMS_INIT_H__
