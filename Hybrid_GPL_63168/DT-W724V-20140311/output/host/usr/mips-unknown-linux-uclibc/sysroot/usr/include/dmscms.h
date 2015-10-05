/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dmscms.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef DMS_CONFIGURE
#define DMS_CONFIGURE
#include "usbmntcmsapi.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#ifdef ATP_DEBUG
#define ATP_DMSCMS_DEBUG(format, args...)  {printf("DMSCMS DEBUG============file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_DMSCMS_DEBUG(format, args...)
#endif

enum ATP_MSG_DMS_TYPE
{
    ATP_MSG_DMS_CMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DMSCMS),
    ATP_MSG_DMS_CMSENABLE,
};

#ifdef SUPPORT_ATP_DMS_DYNAMIC_TOPFOLDER
enum ATP_MSG_SCANNER_TYPE
{
	ATP_MSG_SCANNER_DIRECTORY_ADD = ATP_MSG_CATEGORY_DEF(ATP_MODULE_SCANNER),
	ATP_MSG_SCANNER_DIRECTORY_DELETE,
};
#endif

VOS_INT32 ATP_DmsGetCurMaxUsbDevs(VOS_CHAR *pszRetCurMaxSd, VOS_UINT32 ulLen);
VOS_INT32 CheckMountedAll(const VOS_CHAR *pszCurMaxSd);
VOS_UINT32 ATP_DMS_StartByUsbmount( VOS_VOID *pvNotifier, VOS_VOID *pvNotifyHook );
VOS_UINT32 ATP_DMS_Init(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif // LOG_CONFIGURE
