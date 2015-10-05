/******************************************************************************
  版权所有  : 2009，华为技术有限公司
  文 件 名  : sysinfoapi.h
  作    者  : c47036
  版    本  : v1.0
  创建日期  : 2009-2-20
  描    述  : DNS API

  历史记录      :
   1.日    期   : 2009-2-20
     作    者   : c47036
     修改内容   : 完成初稿

******************************************************************************/
#include "atptypes.h"
#include "msgapi.h"
#include "httpapi.h"


#ifndef __SYS_CMSAPI_API_H__
#define __SYS_CMSAPI_API_H__

#define WEB_USERINTF_LEN_8               (8)
#define WEB_USERINTF_LEN_16              (16)
#define WEB_USERINTF_LEN_64              (64)
#define WEB_USERINTF_LEN_32              (32)
#define WEB_USERINTF_LEN_128             (128)
#define WEB_USERINTF_LEN_256             (256)

// 05011601.00.349
#define WEB_USERINTF_VER_LEN             (15)

// 05011601.00.349SP01
#define WEB_USERINTF_VERWITHSP_LEN             (19)

#define WEB_USERINTF_DOWNLOAD_OUTTIME    (5)     // 5 second
#define WEB_USERINTF_FIRMWAREFILE_MAX    ((10) * (1024))  // 10KB
#define WEB_USERINTF_FIRMWAREFILE_NAME   "/var/firmversion.xml"


#define WEB_USERINTF_UPGRADESTATUS  "/var/upgradestatus"
#define ATP_SAVE_DOWN_TIME_PATH "/var/wgetcountdown.txt"


typedef struct tagDT_WEB_FIRMIMAGE_VERSION_ST
{
    VOS_CHAR              acProductClass[WEB_USERINTF_LEN_32];
    VOS_CHAR              acModelName[WEB_USERINTF_LEN_32];
    VOS_CHAR              acFirmwareVersion[WEB_USERINTF_LEN_32];
    VOS_CHAR              acLevel[WEB_USERINTF_LEN_16];
    VOS_CHAR              acImageURL[WEB_USERINTF_LEN_128];
    VOS_CHAR              acDigest[WEB_USERINTF_LEN_64];
} DT_WEB_FIRMIMAGE_VERSION_ST;

#if defined(__cplusplus)

#endif

#endif