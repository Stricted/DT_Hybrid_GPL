/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ddnscmsmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author X00192803
  *\date  2012-3-20
*/

#ifndef __ATP_DDNS_MSGTYPES_H__
#define __ATP_DDNS_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_CBBID_DDNS_NAME                  "ddns"

#ifdef SUPPORT_ATP_DDNS_GET_STATUS
enum ATP_DDNS_MSGTYPES_EN
{
    ATP_MSG_DDNS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DDNSCMS),    //启动DHCP6s进程      
    ATP_MSG_CMS_DDNS_STATUS,
} ;

/*start by m00132836 for ddns request of connect to server status */

#define DDNS_STATUS_LENGTH      256

typedef struct _tagDDNSStatus
{
    char  acStatus[DDNS_STATUS_LENGTH]; //connected;  error (with reason) or connecting
    char  acHostname[DDNS_STATUS_LENGTH]; // Hostname
    //struct _tagDDNSStatus *pstNext;
}DDNS_STATUS_ST;
/*end by m00132836 for ddns request of connect to server status */
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
