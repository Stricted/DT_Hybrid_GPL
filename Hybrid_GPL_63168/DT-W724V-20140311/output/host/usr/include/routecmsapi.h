/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file routecmsapi.h
  *Routeģ�����ӿ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author h00170023
  *\date  2012-2-6
*/

#include "atputil.h"
#include "atp_interface.h"
#include "atpconfig.h"
#include "cfmapi.h"
#include "cmo_def.h"
#ifndef SUPPORT_ATP_TR181
#include "wancmsapi.h"
#else
#include "wancorecmsapi.h"
#endif

#define ATP_WAN_IFC_LEN 32
#define ATP_WAN_PATH_LEN 128

//#define ATP_DEBUG     1

#ifdef ATP_DEBUG
#define ATP_ROUTE_DEBUG(format, args...)          printf("<DEBUG> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#define ATP_ROUTE_ERROR(format, args...)		  printf("<ERROR> --->FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#else
#define ATP_ROUTE_DEBUG(format, args...) 
#define ATP_ROUTE_ERROR(format, args...)		 
#endif

//!Դ��ַ��ʱ��Դ��ַ����
#define BIND_IP_LENGTH              64 
//!PVCNAME��󳤶�
#define BIND_PVCNAME_LENGTH         32 
//!ʹ��IP����·�ɱ�������Ŀ
#define BIND_TABLE_IPRULE_MAX_NUM   32   
//!���·�ɱ��
#define BIND_TABLE_MAX_NUM          256   
//!�󶨱�ʶλ����ʶ�Ѿ��󶨡�
#define BIND_TRUE                   1 
//!�󶨱�ʶλ����ʶδ�󶨡�
#define BIND_FALSE                  0 
//!֧�ֵ����LAN��Ŀ
#ifdef SUPPORT_ATP_WLAN
#define BIND_MAXLANNUM              8        
#else
#define BIND_MAXLANNUM              4        
#endif

#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE
#define ATP_ROUTE_MIN_MTU			1
#define ATP_ROUTE_MAX_MTU			1540
#endif

#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE		
#define STATIC_POLICY_ROUTE_TABLE_START      180
#define STATIC_POLICY_ROUTE_TABLE_MAX_NUM    20
#define STATIC_POLICY_ROUTE_TABLE_END		  (STATIC_POLICY_ROUTE_TABLE_START + STATIC_POLICY_ROUTE_TABLE_MAX_NUM)
//#define ATP_BIND_IPRULE_UNUSED 0
//#define ATP_BIND_IPRULE_USED   1
#endif
//!IPDEVʹ�õĲ���·�ɱ���С��� 
#define BIND_IPRULEDEV_TABLE_START  100   
//!IPDEVʹ�õĲ���·�ɱ������ 
#define BIND_IPRULEDEV_TABLE_END    120   

#define BIND_SPECIALDEV_TABLE_START 150

#define BIND_FWMARKRULEDEV_TABLE_START  200   

/*ΪDT��FON����172.17.2.0/24 �Ӳ���·�����ȼ�27000,ֻ��dsl ppp256*/
#define BIND_FON_TABLE_ID           270
#define ROUTE_SRC_IP_RULE_PREF_FON  27000


/*start added by c00177661 for �߼���̬����·�ɹ���*/
#ifdef SUPPORT_ATP_STATIC_POLICY_ROUTE_ADVANCED
#define STATIC_POLICY_ROUTE_ADVNACED_RULE_PREF_START  26000

/* protocol */
#define ROUTE_PROTO_BOTH                           "TCP/UDP"
#define ROUTE_PROTO_TCP                            "TCP"
#define ROUTE_PROTO_UDP                            "UDP"
#define ROUTE_PROTO_ICMP                           "ICMP"
#define ROUTE_PROTO_IGMP                           "IGMP"

#define ROUTE_INT_DEFAULT                          -1

#define IS_EMPTY_STRING(s)                         (*s == '\0')

/* ����ebtables ftos ��dscp����tos����ipp */
#define ROUTE_DSCP_MARK                            0x1

#define ATP_ROUTE_MIN_DSCP                         0
#define ATP_ROUTE_MAX_DSCP                         63

#define ATP_ROUTE_MIN_PORT                         1
#define ATP_ROUTE_MAX_PORT                         65535
#endif
/*end added by c00177661 for �߼���̬����·�ɹ���*/

/*start added by c00177661 for ����·��mark����*/
#define ROUTE_BRDGE_BIND_MARK                  "0x1%d000000"  //ipv4�Ű�mark
#define ROUTE_STATIC_ROUTE_MARK                "0x8%d000000"  //ipv4��̬·��mark
/*end added by c00177661 for ����·��mark����*/

/*Start of ATP 2012-5-9 for DTS2012032102857 by : �ֶ����õĲ���·�����ȼ������Զ���ӵģ�Ӱ�칦��*/
#define ROUTE_SRC_IP_RULE_PREF_START  28000
#define ROUTE_BR_BIND_RULE_PREF_START 30000
#define STATIC_POLICY_ROUTE_RULE_PREF_START  32000
/*End of ATP 2012-5-9 for DTS2012032102857 by */

struct BindIpRuleMap
{
    VOS_UINT32 ulTableId;                       /*!< ʹ�õĲ���·�ɱ�� */
    VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];  /*!< ԴIP��ַ�󶨵�PVC���֣��ؼ��֡� */
    VOS_UINT32 ulUsedFlag;                      /*!< ��ʾ����ṹ�������ݰ󶨹��� */
    VOS_CHAR   pcSrcIp[BIND_IP_LENGTH];         /*!< ԴIP��ַ�󶨵�Դ��ַ�� */
    VOS_CHAR   pcGateway[BIND_IP_LENGTH];       /*!< ��һ����ַ */
    VOS_CHAR   pcRaPrefix[ATP_WAN_PREFIXLIST_LEN];/*!< �����wan��ַ��ra���ɵ���prefix OnLink */
};

struct BindRoutePortMap
{
	VOS_CHAR   *pcLanName;                          /*!< LAN��������������һ��LAN��ֻ�ܰ󶨵�һ��PVC */
	VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];      /*!< LAN�ڰ󶨵�PVC���֣����û�а���Ϊ�գ�pppoe_x_x_x */
	VOS_CHAR   pcGateway[BIND_IP_LENGTH];           /*!< ��һ����ַ */
	VOS_CHAR   pcRaPrefix[ATP_WAN_PREFIXLIST_LEN];  /*!< �����wan��ַ��ra���ɵ���prefix OnLink */
	VOS_UINT32 ulFwMark;                            /*!< ÿ���˿ڷ����FWMARKֵ */
	VOS_UINT32 ulTableId;                           /*!< ÿ��LAN·��ģʽ�°��õĲ���·�ɱ�� */
	VOS_UINT32 ulBindFlag;                          /*!< �󶨱�ʶ������ΪBIND_TRUE��û�а���ΪBIND_FALSE */
};

struct BindDftPolicyRouteInfo
{
    VOS_BOOL   bUsed;                               /*!< ���Ű󶨵�Ĭ�ϲ���·���Ƿ���� */
	VOS_CHAR   pcPvcName[BIND_PVCNAME_LENGTH];      /*!< LAN�ڰ󶨵�Ĭ�ϲ���·��PVC���֣����û�а���Ϊ�գ�pppoe_x_x_x */
	VOS_CHAR   pcGateway[BIND_IP_LENGTH];           /*!< Ĭ�ϲ���·����һ����ַ */
};

typedef enum tagATP_BIND_IPRULE_FLAG
{
    ATP_BIND_IPRULE_UNUSED = 0,
    ATP_BIND_IPRULE_USED = 1
} ATP_BIND_IPRULE_FLAG_EN;

/*!ȱʡ·��Check��Ϣ����
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/

VOS_UINT32 ATP_ROUTE_DefaultCheckMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);

/*!ȱʡ·��Set��Ϣ����
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval ATP_E_ERR_CFM_NO_VISIT_ANY ������FLASH�͹����ڴ�
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_DefaultSetMsgProc(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);

/*!·��ʵ��У�鴦����
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/

VOS_UINT32 ATP_ROUTE_CheckMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            const VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);

/*!·��ʵ������޸���Ϣ������
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval ATP_E_ERR_CFM_NO_VISIT_ANY ������FLASH�͹����ڴ�
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_AddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);

/*!·��ʵ��ɾ����Ϣ������
  \brief 
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_DelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                 VOS_UINT32 ulObjId);

/*!
  \brief ·�ɳ�ʼ������
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
/*start modify by x00202174 for DTS2012080401282 2012/8/13 */
VOS_UINT32 ATP_ROUTE_Init(VOS_VOID);
/*end modify by x00202174 for DTS2012080401282 2012/8/13 */
/*!
  \brief ����ĳ��WAN��·������
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: ���õĹ��Ӳ���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_Setup(const VOS_VOID *pszInterPath, VOS_VOID *pvHook); 
/*!
  \brief ֹͣĳ��WAN��·������
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: ���õĹ��Ӳ���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_Stop(const VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief ���ĳ��WAN��·������
  \param[in]  *pszInterPath: WAN Path
  \param[in]  *pvHook: ���õĹ��Ӳ���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_VOID ATP_ROUTE_Clear(VOS_CHAR *pszInterPath);
/*!
  \brief LAN��������øı��Route������Ч
  \param[in] VOS_VOID: 
  \return 
*/
VOS_UINT32 ATP_ROUTE_LanChanged(const VOS_VOID *pvNotifier, VOS_VOID *pvHook);

/*RIP*/
#ifdef SUPPORT_ATP_RIP
/*!
  \brief ����RIP��̬·�ɹ���
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_StartRip(VOS_VOID);
/*!
  \brief ֹͣRIP��̬·�ɹ���
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_StopRip(VOS_VOID);
/*!
  \brief 
  \param[in]  *pszName: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
//VOS_UINT32 ATP_ROUTE_RipDelProc(VOS_CHAR *pszName);
//VOS_UINT32 ATP_ROUTE_RipSetProc(VOS_CHAR *pszName , VOS_CHAR *pszVersion, VOS_CHAR *pszOperation);
#endif

/*!
  \brief ɾ������·�ɲ��������·��
  \param[in]  pszPath: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_ROUTE_DEL_DftPoliceRouteWithSupplement(VOS_CHAR* pszPath);

/*!
  \brief ���LAN��󶨽ӿ�Ĭ�ϲ���·��
  \param[in]  pszWANInfPath: ��ӻ�ɾ������·�ɵ�WAN Path
  \param[in]  *pvHook: ���Ӻ����Ĵ��˲��������ﴫ�˵�����ӻ�ɾ����־
  \return 
  \retval VOS_TRUE ִ�гɹ�
  \retval VOS_FALSE ִ��ʧ��
*/
VOS_UINT32 ATP_ROUTE_ADD_DftPoliceRoute(VOS_VOID* pszWANInfPath, VOS_VOID *pvHook);
/*!
  \brief ���WAN�ӿڵ�Ĭ�ϲ���·��
  \param[in]  pszWANInfPath: ��ӻ�ɾ������·�ɵ�WAN Path
  \param[in]  *pvHook: ���Ӻ����Ĵ��˲��������ﴫ�˵�����ӻ�ɾ����־
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_ROUTE_ADD_WanDftPoliceRoute(VOS_VOID* pszWANInfPath, VOS_VOID *pvHook);
/*!
  \brief 
  \param[in]  *pszInterPath: ·��������WAN PATH
  \param[in]  *pvHook: ���Ӻ����Ĵ��˲��������ﴫ�˵�����ӻ�ɾ����־
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/

VOS_UINT32 ATP_WAN_GetGateWay_DT(const VOS_CHAR* acIfName, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface );
VOS_UINT32 ATP_WAN_GetMask_DT(const VOS_CHAR* acIfName, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);

VOS_UINT32 ATP_ROUTE_BY_WANSTART(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief 
  \param[in]  *pszInterPath: ·��ֹͣ��WAN PATH
  \param[in]  *pvHook: ���Ӻ����Ĵ��˲��������ﴫ�˵�����ӻ�ɾ����־
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_ROUTE_BY_WANSTOP(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief WANɾ��ʱ�����·�����
  \param[in]  *pszInterPath: WANɾ����PATH
  \param[in]  *pvHook: ���Ӻ����Ĵ��˲���
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_ROUTE_BY_WANCLEAR(VOS_VOID *pszInterPath, VOS_VOID *pvHook);
/*!
  \brief ��һ���Ű󶨶��WANʱ��ɾ��ĳ��Ĭ��·�ɵ�WANҪ����������WAN��Ϊ�µ�Ĭ��·��
  \param[in]  pszWANInfPath: Ҫ������ӵ��µ�Ĭ��·��WAN��·��
  \param[in]  ulBridgeKey: ����Ӧ����Key
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_ROUTE_SupplementDftRouteInMRT(VOS_CHAR* pszWANInfPath, VOS_INT32 lBridgeKey);
/*!
  \brief Ϊ����������·�ɰ�
  \param[in]  pszInterface: WAN�ӿ�
  \param[in]  ulBrKey: �󶨵�LAN��ӿ�
  \param[in]  bIsAdd: ��ӻ���ɾ��
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_BIND_RouteDevBrBind(const VOS_CHAR* pszInterface, const VOS_CHAR *pszGateway, VOS_INT32 lBrKey, VOS_BOOL bIsAdd);
/*!
  \brief �������ӽӿ�������RIP��Ϣ����ɾ�������ӽӿ�����RIP��Ϣ
  \param[in]  *pszName: �ӿ���
  \return 
  \retval ATP_ROUTE_STS_FAIL RIPɾ��ʧ��
  \retval ATP_ROUTE_STS_OK RIPɾ���ɹ�
*/
VOS_UINT32 ATP_ROUTE_RipDelProc(VOS_CHAR *pszName);
/*!
  \brief �������ӽӿ������汾��RIP����ģʽ��������RIP��Ϣ
  \param[in]  *pszName: ���ӽӿ���
  \param[in]  *pszVersion: RIP�汾
  \param[in]  *pszOperation: RIP����ģʽ
  \return 
  \retval ATP_ROUTE_STS_FAIL RIP����ʧ��
  \retval VOS_OK RIP���óɹ�
*/
VOS_UINT32 ATP_ROUTE_RipSetProc(VOS_CHAR *pszName , VOS_CHAR *pszVersion, VOS_CHAR *pszOperation);
/*!
  \brief ������ģ�����ͨ��ԴIP�ķ�ʽ�󶨲���·��
  \param[in]  *pcWanPath: WAN��·��
  \param[in]  *pcPvcName: WAN�ӿ���
  \param[in]  *pcBridgeGw: �ŵ�
  \param[in]  *pcSrcIp: 
  \param[in]  *pcSubMask: 
  \return 
  \retval VOS_OK ���óɹ�
  \retval VOS_NOK ����ʧ��
*/

VOS_UINT32 ATP_BIND_PolicyRouteBySrcIp(VOS_CHAR *pcWanPath, VOS_CHAR *pcPvcName, VOS_CHAR *pcBridgeGw, VOS_CHAR *pcSrcIp,VOS_CHAR *pcSubMask);
/*!
  \brief ������ģ�����ͨ��ԴIP�ķ�ʽ����󶨲���·��
  \param[in]  *pcWanPath: WAN��·��
  \param[in]  *pcPvcName: WAN�ӿ���
  \param[in]  *pcBridgeGw: �ŵ�
  \param[in]  *pcSrcIp: 
  \param[in]  *pcSubMask: 
  \return 
  \retval VOS_OK ���óɹ�
  \retval VOS_NOK ����ʧ��
*/
VOS_UINT32 ATP_UNBIND_PolicyRouteBySrcIp(VOS_CHAR *pcWanPath, VOS_CHAR *pcPvcName,VOS_CHAR *pcBridgeGw);

/*!
  \brief ��ȡ��·�ɱ�Ĭ��·����Ϣ
  \param[in]  *pszGtwy: Ҫ��ȡ��Ĭ��·������
  \param[in]  *pszWanIf: Ҫ��ȡ��Ĭ��·��WAN �ӿ�
  \return 
  \retval VOS_OK ���óɹ�
  \retval VOS_NOK ����ʧ��
*/
VOS_UINT32  ATP_ROUTE_GetDftGtwy(VOS_CHAR *pszGtwy, VOS_CHAR *pszWanIf);
#ifdef SUPPORT_ATP_RIP

/*!
  \brief ���RIP��Ϣ�����ݿ�
  \param[in]  *pszName: WAN PATH
  \param[in]  *pszIfSide: WAN/LAN side
  \return 
  \retval VOS_OK ���óɹ�
  \retval VOS_NOK ����ʧ��
*/
VOS_UINT32 ATP_ROUTE_AddRipInfoToDB(VOS_CHAR *pszName, VOS_CHAR *pszIfSide);

/*!
  \brief ɾ��RIP��Ϣ�����ݿ�
  \param[in]  *pszName: WAN PATH
  \param[in]  *pszIfSide: WAN/LAN side
  \return 
  \retval VOS_OK ���óɹ�
  \retval VOS_NOK ����ʧ��
*/
VOS_UINT32 ATP_ROUTE_DelRipInfoToDB(VOS_CHAR *pszName, VOS_CHAR *pszIfSide);
/*!
  \brief ��ȡ��һ��ȱʡ·�ɵĽӿ���
  \param[in]  *pcIfcName: �ӿ���
  \return 
  \retval ��ȡ�ɹ�����VOS_TRUE����ȡʧ�ܷ���VOS_FALSE
*/
VOS_BOOL ATP_UTIL_GetDefaultRouteInterfaceName(VOS_CHAR *pcIfcName) ;

#endif
#ifdef SUPPORT_ATP_TR181
VOS_UINT32 ATP_ROUTE_Renew(VOS_VOID);
#endif

