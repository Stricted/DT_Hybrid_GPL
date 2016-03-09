/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file hybridapi.h
  *hybrid sdk api,所有函数只被main模块引用，sdk内部不要引用
  *\copyright 2007-2020，华为技术有限公司
  *\author t00189036
  *\date  2013-4-19
*/
#ifndef __HYBIRD_API_H__
#define __HYBIRD_API_H__
VOS_UINT32 ATP_HYBRID_Init();
VOS_UINT32 ATP_HYBRID_WanChg(const ATP_HYBRID_WAN_CHG_INFO_ST* pstInfo);
VOS_UINT32 ATP_HYBRID_WanWillDown(ATP_HYBRID_LINK_TYPE_EN enLinkType);
VOS_UINT32 ATP_HYBRID_DhcpUp(const VOS_CHAR* pcAddr);
VOS_UINT32 ATP_HYBRID_Dhcpv6Chg(ATP_HYBRID_WAN_CHG_INFO_ST* pstInfo);
VOS_UINT32 ATP_HYBRID_DhcpDown();
VOS_VOID ATP_HYBRID_ShowInfo(VOS_UINT32 ulMsgType);
VOS_VOID ATP_HYBRID_SetFlowCtl();
VOS_VOID ATP_HYBRID_SetHelloCheck(VOS_BOOL bValue);
VOS_VOID ATP_HYBRID_SwitchTearDown(VOS_BOOL bValue);
VOS_VOID ATP_HYBRID_RunTestCase(VOS_UINT32 ulTestCaseSeq);
VOS_VOID ATP_HYBRID_GetInfo(const VOS_CHAR* pcSendApp, VOS_UINT32 ulMsgType);
VOS_VOID ATP_HYBRID_SwitchIdleHello(VOS_UINT32* pulIdleHelloEnable);
VOS_VOID ATP_HYBRID_T2ParseMsgProc(const VOS_CHAR* pcDomain);
VOS_VOID ATP_HYBRID_SetUserGroup(const VOS_CHAR* pcValue);
VOS_VOID ATP_HYBRID_SetReqTimeout(VOS_UINT32 ulValue);
VOS_VOID ATP_HYBRID_SetMssFromMSG(const ATP_HYBRID_MTU_CHG_INFO_ST* pstInfo);
VOS_VOID ATP_HYBRID_UpdateIdleHelloInterval(const VOS_UINT8 *pucValue, VOS_UINT32 ulLen);
VOS_VOID ATP_HYBRID_UpdateTrafficInterval(const VOS_UINT8 *pucValue, VOS_UINT32 ulLen);
VOS_VOID ATP_HYBRID_StartChgHelloStatus();
VOS_VOID ATP_HYBRID_ChgHelloStatus();
#endif
