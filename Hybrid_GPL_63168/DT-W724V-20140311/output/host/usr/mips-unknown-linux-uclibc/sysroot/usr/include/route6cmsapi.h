/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file route6cmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x00192803
  *\date  2012-1-15
*/

/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : route6cmsapi.h
  作    者  : d00107688
  版    本  : 1.0
  创建日期  : 2009-11-9
  描    述  : 路由API函数
  函数列表  :

  历史记录      :
   1.日    期   : 2009-11-9
     作    者   : d00107688
     修改内容   : 完成初稿

*********************************************************************************/

#ifndef __ROUTE6_CMS_API_H__
#define __ROUTE6_CMS_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifdef ATP_DEBUG
#define ATP_ROUTE6_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define ATP_ROUTE6_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define ATP_ROUTE6_DEBUG(format,arg...)
#define ATP_ROUTE6_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif

#ifdef SUPPORT_ATP_HYBRID_BONDING
/* Start of add for DTS2013101207088 by z00210521 2013-10-12 */
#define INTERFACE_POLICE_ROUTING_6_PREF		1

#define INTERFACE_POLICE_ROUTING_6_TABLE	INTERFACE_POLICE_ROUTING_TABLE
/* End of add for DTS2013101207088 by z00210521 2013-10-12 */
#endif

//!mark table起始值
#define BIND6_FWMARKRULEDEV_TABLE_START  200   

//!IPDEV使用的策略路由表最小表号 
#define BIND6_IPRULEDEV_TABLE_START     100
//!IPDEV使用的策略路由表最大表号 
#define BIND6_IPRULEDEV_TABLE_END    120

#ifdef SUPPORT_ATP_ROUTE6_STATIC_POLICY_ROUTE
#define STATIC_POLICY_ROUTE6_TABLE_START      168
#define STATIC_POLICY_ROUTE6_TABLE_MAX_NUM    32
#define STATIC_POLICY_ROUTE6_TABLE_END		  (STATIC_POLICY_ROUTE6_TABLE_START + STATIC_POLICY_ROUTE6_TABLE_MAX_NUM)
#define STATIC_POLICY_ROUTE6_ADVNACED_RULE_PREF_START  16000

struct StaticPolicyRoute6Rule
{
    VOS_UINT32 	ulUsedFlag;                      //表示这个结构是有数据绑定过的
    //VOS_UINT32 	ulTableId;						
    VOS_CHAR                    acSrcAddr[ATP_STRING_LEN_64];
    VOS_UINT32                  uiSrcPrefixLen;
    VOS_CHAR                    acDestAddr[ATP_STRING_LEN_64];
    VOS_UINT32                  uiDstPrefixLen;
    VOS_CHAR                    acInterface[ATP_STRING_LEN_256];
};
#endif

//!源地址绑定时的源地址长度
#define BIND6_IP_LENGTH              64 
//!PVCNAME最大长度
#define BIND6_PVCNAME_LENGTH         32 
//!使用IP策略路由表的最大数目
#define BIND6_TABLE_IPRULE_MAX_NUM   32   
//!最大路由表号
#define BIND6_TABLE_MAX_NUM          256   
//!绑定标识位。标识已经绑定。
#define BIND6_TRUE                   1 
//!绑定标识位。标识未绑定。
#define BIND6_FALSE                 0

//!支持的最大LAN数目
#ifdef SUPPORT_ATP_WLAN
#define BIND6_MAXLANNUM              8        
#else
#define BIND6_MAXLANNUM              4        
#endif

/* 前缀长度 */
#define ROUTE6_WAN_PREFIXLIST_LEN (128 + 4)

/*start added by c00177661 for 策略路由mark分配*/
#define ROUTE6_BRDGE_BIND_MARK                  "0x2%d000000"  //ipv6桥绑定mark
/*end added by c00177661 for 策略路由mark分配*/

//!\brief 源ip策略路由信息结构体
/*!
 *
 */
struct Bind6IpRuleMap
{
    VOS_UINT32 ulTableId;                       /*!< 使用的策略路由表号 */
    VOS_CHAR   pcPvcName[BIND6_PVCNAME_LENGTH];  /*!< 源IP地址绑定的PVC名字，关键字。 */
    VOS_BOOL ulUsedFlag;                      /*!< 表示这个结构是有数据绑定过的 */
    VOS_CHAR   pcSrcIp[BIND6_IP_LENGTH];         /*!< 源IP地址绑定的源地址。 */
    VOS_CHAR   pcGateway[BIND6_IP_LENGTH];       /*!< 下一跳地址 */
    VOS_CHAR   pcRaPrefix[ROUTE6_WAN_PREFIXLIST_LEN];/*!< 如果该wan地址是ra生成的且prefix OnLink */
};

//!\brief 源ip策略路由规则状态结构体
/*!
 *
 */
typedef enum tagATP_BIND6_IPRULE_FLAG
{
    ATP_BIND6_IPRULE_UNUSED = 0,
    ATP_BIND6_IPRULE_USED = 1
} ATP_BIND6_IPRULE_FLAG_EN;

/*!
  \brief 当一个桥绑定多个WAN时，删除某条默认路由的WAN要补充其他的WAN作为新的默认路由
  \param[in]  pszWANInfPath: 要补充添加的新的默认路由WAN的路径
  \param[in]  ulBridgeKey: 所对应的桥Key
  \return 
  \retval 
*/
VOS_UINT32 ATP_Route6_SupplementDftRouteInMRT(const VOS_CHAR* pszWANInfPath, const VOS_UINT32 ulBridgeKey);

/*!
  \brief 添加LAN侧绑定接口默认策略路由
  \param[in]  pszWANInfPath: 添加策略路由的WAN Path
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_UINT32 ATP_Route6_AddPolicyRoute(const VOS_CHAR* pszWANInfPath);

/*!
  \brief 删除LAN侧绑定接口默认策略路由
  \param[in]  pszWANInfPath: 删除策略路由的WAN Path
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_UINT32 ATP_Route6_DelPolicyRoute(const VOS_CHAR* pszWANInfPath);

/*!
  \brief 为隧道单独添加路由绑定
  \param[in]  pszInterface: WAN接口
  \param[in]  pszGw: 下一跳网关
  \param[in]  lBrKey: 所对应的桥Key
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_UINT32 ATP_Route6_AddPortBind(const VOS_CHAR *pszWanIfc, const VOS_CHAR *pszGateway, const VOS_INT32 lBrKey);

/*!
  \brief 为隧道单独添加路由解绑定
  \param[in]  lBrKey: 所对应的桥Key
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_UINT32 ATP_Route6_DelPortBind(const VOS_INT32 lBrKey);

/*!
  \brief LAN侧RADVD和DHCP6S前缀发生改变时刷新策略路由表
  \param[in]  ulTableId: 所对应的路由表
  \return 
  \retval VOS_TRUE 执行成功
  \retval VOS_FALSE 执行失败
*/
VOS_VOID ATP_Route6_TableRestart(const VOS_INT32 lBridgeKey);
VOS_UINT32 AddDelL2BPolicyRoute(VOS_VOID * vlBridgeKey,VOS_VOID *pvHook);
VOS_UINT32 AddDelL2BPolicyRouteEx(VOS_VOID * vlBridgeKey,VOS_VOID *pvHook);
VOS_UINT32 ATP_ROUTE6CMS_Init(VOS_VOID);
#ifdef SUPPORT_ATP_TR181
VOS_UINT32 ROUTE6CMS_Inst(VOS_VOID);
VOS_VOID ROUTE6CMS_StopAll(VOS_UINT32 ulInstId);
VOS_VOID ROUTE6CMS_SetupAll(VOS_UINT32 ulInstId);
VOS_VOID ROUTE6CMS_DelAll(VOS_UINT32 ulInstId);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

