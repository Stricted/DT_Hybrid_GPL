/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ddnscmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author X00192803
  *\date  2012-3-20
*/

#ifndef __ATP_DDNS_API_H__
#define __ATP_DDNS_API_H__

#define DDNS_MAX_ENTRY          32 

#define DDNS_PROVIDER_LEN       256
#define DDNS_USERNAME_LEN       256
#define DDNS_USERPASS_LEN       256
#define DDNS_PROTO_LEN          16
#define DDNS_SERVERADDR_LEN     64
#define DDNS_DOMAINNAME_LEN     256
#define DDNS_HOSTNAME_LEN       256
#define DDNS_WANPATH_LEN        256

#define DDNS_DEFAULT_SERPORT    80

#define DDNS_NULL_CHAR          '\0'

#define DDNS_CFG_FILE           "/var/ddnsc.cfg"
#define DDNS_CACHE_FILE       "/var/ddnsc.cache"
#define DDNS_STATUS_FILE     "/var/ddnsc.status"     // 1 means ddnsc started, 0 means stop
#define DDNS_APP_NAME          "ddnsc"


#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_DDNS_Init(VOS_VOID);
VOS_UINT32 ATP_DDNS_UnInit(VOS_VOID);
VOS_UINT32 ATP_DDNS_StartService(const VOS_CHAR *pszWanPath);
VOS_UINT32 ATP_DDNS_StopService(const VOS_CHAR *pszWanPath);
VOS_UINT32 ATP_DDNS_RemoveRules(const VOS_CHAR* pszWanPath);


#if defined(__cplusplus)
}
#endif

#endif

