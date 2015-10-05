/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file logmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef __ATP_LOG_MSGTYPES_H__
#define __ATP_LOG_MSGTYPES_H__

#define ATP_CBBID_LOG_NAME                  "log"

enum ATP_LOG_MSGTYPES_EN
{
    ATP_MSG_LOG_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_LOG),
    ATP_MSG_LOG_ITEM,
    ATP_MSG_LOG_KERNEL_ITEM,
    ATP_MSG_LOG_FILTER,
    ATP_MSG_LOG_RTO,
    ATP_MSG_LOG_TRIGGER,
    ATP_MSG_LOG_FLUSH,
    ATP_MSG_LOG_CLEAR,
    ATP_MSG_LOG_ENABLE,
    ATP_MSG_LOG_CLI_DEBUG
} ;
#endif // End of __ATP_LOG_MSGTYPES_H__

