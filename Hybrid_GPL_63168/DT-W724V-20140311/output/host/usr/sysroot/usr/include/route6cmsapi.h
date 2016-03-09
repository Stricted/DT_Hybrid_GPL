/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file route6cmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author x00192803
  *\date  2012-1-15
*/

/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : route6cmsapi.h
  ��    ��  : d00107688
  ��    ��  : 1.0
  ��������  : 2009-11-9
  ��    ��  : ·��API����
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2009-11-9
     ��    ��   : d00107688
     �޸�����   : ��ɳ���

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

//!mark table��ʼֵ
#define BIND6_FWMARKRULEDEV_TABLE_START  200   

//!IPDEVʹ�õĲ���·�ɱ���С��� 
#define BIND6_IPRULEDEV_TABLE_START     100
//!IPDEVʹ�õĲ���·�ɱ������ 
#define BIND6_IPRULEDEV_TABLE_END    120

#ifdef SUPPORT_ATP_ROUTE6_STATIC_POLICY_ROUTE
#define STATIC_POLICY_ROUTE6_TABLE_START      168
#define STATIC_POLICY_ROUTE6_TABLE_MAX_NUM    32
#define STATIC_POLICY_ROUTE6_TABLE_END		  (STATIC_POLICY_ROUTE6_TABLE_START + STATIC_POLICY_ROUTE6_TABLE_MAX_NUM)
#define STATIC_POLICY_ROUTE6_ADVNACED_RULE_PREF_START  16000

struct StaticPolicyRoute6Rule
{
    VOS_UINT32 	ulUsedFlag;                      //��ʾ����ṹ�������ݰ󶨹���
    //VOS_UINT32 	ulTableId;						
    VOS_CHAR                    acSrcAddr[ATP_STRING_LEN_64];
    VOS_UINT32                  uiSrcPrefixLen;
    VOS_CHAR                    acDestAddr[ATP_STRING_LEN_64];
    VOS_UINT32                  uiDstPrefixLen;
    VOS_CHAR                    acInterface[ATP_STRING_LEN_256];
};
#endif

//!Դ��ַ��ʱ��Դ��ַ����
#define BIND6_IP_LENGTH              64 
//!PVCNAME��󳤶�
#define BIND6_PVCNAME_LENGTH         32 
//!ʹ��IP����·�ɱ�������Ŀ
#define BIND6_TABLE_IPRULE_MAX_NUM   32   
//!���·�ɱ��
#define BIND6_TABLE_MAX_NUM          256   
//!�󶨱�ʶλ����ʶ�Ѿ��󶨡�
#define BIND6_TRUE                   1 
//!�󶨱�ʶλ����ʶδ�󶨡�
#define BIND6_FALSE                 0

//!֧�ֵ����LAN��Ŀ
#ifdef SUPPORT_ATP_WLAN
#define BIND6_MAXLANNUM              8        
#else
#define BIND6_MAXLANNUM              4        
#endif

/* ǰ׺���� */
#define ROUTE6_WAN_PREFIXLIST_LEN (128 + 4)

/*start added by c00177661 for ����·��mark����*/
#define ROUTE6_BRDGE_BIND_MARK                  "0x2%d000000"  //ipv6�Ű�mark
/*end added by c00177661 for ����·��mark����*/

//!\brief Դip����·����Ϣ�ṹ��
/*!
 *
 */
struct Bind6IpRuleMap
{
    VOS_UINT32 ulTableId;                       /*!< ʹ�õĲ���·�ɱ�� */
    VOS_CHAR   pcPvcName[BIND6_PVCNAME_LENGTH];  /*!< ԴIP��ַ�󶨵�PVC���֣��ؼ��֡� */
    VOS_BOOL ulUsedFlag;                      /*!< ��ʾ����ṹ�������ݰ󶨹��� */
    VOS_CHAR   pcSrcIp[BIND6_IP_LENGTH];         /*!< ԴIP��ַ�󶨵�Դ��ַ�� */
    VOS_CHAR   pcGateway[BIND6_IP_LENGTH];       /*!< ��һ����ַ */
    VOS_CHAR   pcRaPrefix[ROUTE6_WAN_PREFIXLIST_LEN];/*!< �����wan��ַ��ra���ɵ���prefix OnLink */
};

//!\brief Դip����·�ɹ���״̬�ṹ��
/*!
 *
 */
typedef enum tagATP_BIND6_IPRULE_FLAG
{
    ATP_BIND6_IPRULE_UNUSED = 0,
    ATP_BIND6_IPRULE_USED = 1
} ATP_BIND6_IPRULE_FLAG_EN;

/*!
  \brief ��һ���Ű󶨶��WANʱ��ɾ��ĳ��Ĭ��·�ɵ�WANҪ����������WAN��Ϊ�µ�Ĭ��·��
  \param[in]  pszWANInfPath: Ҫ������ӵ��µ�Ĭ��·��WAN��·��
  \param[in]  ulBridgeKey: ����Ӧ����Key
  \return 
  \retval 
*/
VOS_UINT32 ATP_Route6_SupplementDftRouteInMRT(const VOS_CHAR* pszWANInfPath, const VOS_UINT32 ulBridgeKey);

/*!
  \brief ���LAN��󶨽ӿ�Ĭ�ϲ���·��
  \param[in]  pszWANInfPath: ��Ӳ���·�ɵ�WAN Path
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
*/
VOS_UINT32 ATP_Route6_AddPolicyRoute(const VOS_CHAR* pszWANInfPath);

/*!
  \brief ɾ��LAN��󶨽ӿ�Ĭ�ϲ���·��
  \param[in]  pszWANInfPath: ɾ������·�ɵ�WAN Path
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
*/
VOS_UINT32 ATP_Route6_DelPolicyRoute(const VOS_CHAR* pszWANInfPath);

/*!
  \brief Ϊ����������·�ɰ�
  \param[in]  pszInterface: WAN�ӿ�
  \param[in]  pszGw: ��һ������
  \param[in]  lBrKey: ����Ӧ����Key
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
*/
VOS_UINT32 ATP_Route6_AddPortBind(const VOS_CHAR *pszWanIfc, const VOS_CHAR *pszGateway, const VOS_INT32 lBrKey);

/*!
  \brief Ϊ����������·�ɽ��
  \param[in]  lBrKey: ����Ӧ����Key
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
*/
VOS_UINT32 ATP_Route6_DelPortBind(const VOS_INT32 lBrKey);

/*!
  \brief LAN��RADVD��DHCP6Sǰ׺�����ı�ʱˢ�²���·�ɱ�
  \param[in]  ulTableId: ����Ӧ��·�ɱ�
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
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

