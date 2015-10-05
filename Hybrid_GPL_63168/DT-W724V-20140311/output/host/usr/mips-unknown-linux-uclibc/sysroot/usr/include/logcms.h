/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file logcms.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef LOG_CONFIGURE
#define LOG_CONFIGURE


#include "msgapi.h"
#include "cfmapi.h"

#define LOG_ERR_PARAMETER 1
#define LOG_CMD_LENGTH 128
#define LOG_ARGV_COUNT 10
#define LOG_DEBUG printf
#define LOG_CMD_CLIENTID 0
#if defined(__cplusplus)
extern "C"
{
#endif

VOS_UINT32 ATP_LOG_Init (VOS_VOID);

#if defined(__cplusplus)
}
#endif
typedef enum
{
    LOG_ERROR_BEGIN_E = 1,
    LOG_ERROR_INVALIDE_PARAM_E = LOG_ERROR_BEGIN_E,
    LOG_ERROR_INVALIDE_MODE_STRING_E,
    LOG_ERROR_END,
}LOG_ERROR_EN;

#endif // LOG_CONFIGURE
