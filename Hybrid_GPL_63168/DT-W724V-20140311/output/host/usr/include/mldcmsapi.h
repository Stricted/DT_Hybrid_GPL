/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file mldcmsapi.h
  *mldcms对外头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2012-2-10
*/

#ifndef __ATP_MLDCMS_API_H__
#define __ATP_MLDCMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief MLDproxy初始化函数，启动MLDproxy进程
  \param[in] 无
  \return VOS_OK
*/
VOS_UINT32 ATP_MLD_Init(VOS_VOID);

#if defined(__cplusplus)
}
#endif

#endif

