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

/* <DTS2013082109809 z00182709 2013/08/28 begin */
#ifdef SUPPORT_ATP_TELUS
enum ATP_DDNS_MSGTYPES_EN
{
    ATP_MSG_DDNS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DDNSCMS),
    ATP_MSG_CMS_DDNS_NEED_WRITEBACK,                                  //需要将数据写回到数据库中
} ;

#define STRING_LEN_256  256
#define STRING_LEN_64   64

typedef struct _tagDDNS_BACKUP_ST
{
    VOS_CHAR    acHostName[STRING_LEN_256];             // Host name
    VOS_INT32   lStatus;                                    // Status
    VOS_CHAR    acLastAddr[STRING_LEN_64];              // Server address
} DDNS_BACKUP_ST;
#endif /* SUPPORT_ATP_TELUS */
/* DTS2013082109809 z00182709 2013/08/28 end> */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
