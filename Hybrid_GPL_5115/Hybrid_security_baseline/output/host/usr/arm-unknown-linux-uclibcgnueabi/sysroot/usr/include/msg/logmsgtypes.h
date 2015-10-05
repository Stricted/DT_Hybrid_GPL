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
/* <jiangounie j81004134 2012/5/10 begin */
#ifdef SUPPORT_ATP_LOG_MODULE_SWITCH
/* <DTS2012060604837 j81004134 2012/6/2 begin */
    ATP_MSG_LOG_CLI_DEBUG,
    ATP_MSG_LOG_DEBUG_INFO
/* DTS2012060604837 j81004134 2012/6/2 end> */
#else
    ATP_MSG_LOG_CLI_DEBUG
#endif
/* jiangounie j81004134 2012/5/10 end> */
} ;
#endif // End of __ATP_LOG_MSGTYPES_H__

