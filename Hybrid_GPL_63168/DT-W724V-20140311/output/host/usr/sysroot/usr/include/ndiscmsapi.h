/*****************************************************************************
 *               Copyright (C) 2013, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file Ndiscmsapi.h
  *dhcpccms对外头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2013-8-6
*/
#ifndef __NDISCCMS_API_H__
#define __NDISCCMS_API_H__
#include "msgapi.h"
#include "ipintfcmsapi.h"
#ifdef ATP_DEBUG
#define ATP_LTE_NDIS_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_LTE_NDIS_DEBUG(x...)
#endif
typedef struct 
{
    VOS_BOOL bIpv4CurState; //ipv4 当前状态
    VOS_BOOL bIpv6CurState;// ipv6 当前状态
}ATP_LTE_NDIS_INFO_ST;
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

VOS_UINT32 ATP_Ndis_Init();
VOS_UINT32 ATP_Ndis_UnInit();
ATP_IPINTF_IPV6INFO_ST *ATP_Ndis_FindIpv6InfoByInterface(const char * pcInterface);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

