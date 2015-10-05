/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file algapi.h
  *algcms 内部头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author f00120964
  *\date  2011-12-20
*/

#ifndef __ATP_ALG_API_H__
#define __ATP_ALG_API_H__

#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"

/* <DTS2013040203403 j81004134 2013/4/2 begin */
#define SAMBA_PORT_137              137
#define SAMBA_PORT_138              138
#define SAMBA_PORT_139              139
#define SAMBA_PORT_445              445
#define CUPS_PORT_631               631
#define FTPSERVER_PORT_21           21
#define VOIP_PORT_28090             28090
#define VOIP_PORT_START             7070
#define VOIP_PORT_END               7101
#define CWMP_PORT_7547              7547
#define UPNP_PORT_37215             37215
#define UPNP_PORT_1900              1900
#define UPNP_PORT_37443             37443
#define WEB_PORT_80                 80
#define WEB_PORT_443                443
#define DLNA_PORT_56001             56001
#define DLNA_PORT_56002             56002
#define XL2TPD_PORT_1701            1701
#define RACOON_PORT_500             500
#define DHCPV6_CLIENT_PORT_546      546
#define DHCPV6_SERVER_PORT_547      547
#define DNS_PORT_53                   53
#define DHCPS_PORT_67               67
#define DIAGD_PORT_1280             1280
#define SYSLOG_PORT_514             514
#define FTPSERVER_PORT_990          990
#define WLAN_PORT_38000             38000
#define WLAN_PORT_42000             42000
/* DTS2013040203403 j81004134 2013/4/2 end> */
#define ALG_MAX_PORT_NUM              65535
#define ALG_SIP_DEFAULT_PORT          5060
#define ALG_SIP_RTPPORT_LOW           7070
#define ALG_SIP_RTPPORT_HIGH          7079
#define ALG_SIP_CONF_PATH             "/var/alg/siproxd.conf"
#define ALG_SIP_PIDFILE               "/var/alg/sippid"
#define ALG_SIP_DIR                   "/var/alg"
#define ALG_SIP_START_FAIL            0
#define ALG_SIP_START_SUCCESS         1
#define ALG_SIP_DEFAULT_WANPATH       ""
//#define IFC_BRIDGE_NAME            "br0"

typedef struct tagALG_CFM_INFO_ST
{
    VOS_CHAR    acSipWanPath[ATP_STRING_LEN_128];
    VOS_UINT32  ulSipPort;
    VOS_BOOL    bSipEnable;
    VOS_BOOL    bH323Enable;
    VOS_BOOL    bRTSPEnable;
    VOS_BOOL    bPPTPEnable;
}ALG_CFM_INFO_ST;



#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_ALG_SetCfgInfo(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                              VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                              VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);

VOS_UINT32 ATP_ALG_Check(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                         VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                         VOS_UINT32 ulNumOfPara);

VOS_UINT32 ATP_ALG_Init(VOS_VOID);
VOS_UINT32 ATP_ALG_UnInit(VOS_VOID);
VOS_UINT32 ATP_ALG_SipStart(VOS_VOID* pszWanPath,VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_ALG_SipStop(VOS_VOID* pszWanPath,VOS_VOID *pvNotifyHook);
VOS_UINT32 ATP_ALG_SipClear(VOS_VOID* pszWanPath,VOS_VOID *pvNotifyHook);
VOS_BOOL ATP_ALG_SipSig(VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook);
VOS_BOOL AlgRedirectPPPProxySipPkt(VOS_UINT32 ulPort, VOS_BOOL bSipAlgCmd);

#if defined(__cplusplus)
}
#endif

#endif
