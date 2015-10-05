/**\file bridgecmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author ����(j00196195)
  *\date  2011-12-19
*/

#ifndef __BRIDGE_CMS_API_H__
#define __BRIDGE_CMS_API_H__

#include "msgapi.h"
#include "cfmapi.h"
#include "cmsapi.h"
/********************** Include Files ***************************************/
#define ATP_CBBID_BRIDGECMS_NAME "l2b"


/********************** macro  Definitions **********************************/
#define ATP_BRIDGE_DEFAULTNAME		            "br0"

/* 16���ַ��ӿ����� * 16���ӿ� + 16���ֽ�� + '\0'*/
#define ATP_BRIDGE_INTEFACELIST_LEN             (16*16+16+1)

#define ATP_BRIDGE_IGMPSNOOPING_ENABLE_STR      "enable"
#define ATP_BRIDGE_IGMPSNOOPING_DISABLE_STR     "disable"

#define ATP_BRIDGE_IGMPSNOOPING_ENABLE_FLAG     1
#define ATP_BRIDGE_IGMPSNOOPING_DISABLE_FLAG    0

#define ATP_BRIDGE_BRGROUP_ENABLE_STR           "enable"
#define ATP_BRIDGE_BRGROUP_DISABLE_STR          "disable"

#define ATP_BRIDGE_BRGROUP_ENABLE_FLAG          1
#define ATP_BRIDGE_BRGROUP_DISABLE_FLAG         0

#define ATP_BRIDGE_BRGROUPMACLN_ENABLE_STR      "enable"
#define ATP_BRIDGE_BRGROUPMACLN_DISABLE_STR     "disable"

#define ATP_BRIDGE_BRGROUPMACLN_ENABLE_FLAG     1
#define ATP_BRIDGE_BRGROUPMACLN_DISABLE_FLAG    0

#define ATP_BRIDGE_GROUPID_MAX                  15
#define ATP_BRIDGE_GROUPID_MIN                  0

#define BRIDGE_BIND_DEFAULT_GROUPID             0 //��ȱʡ���

#define ATP_BRIDGE_AIF_INF_TYPE_WANROUTER "WANRouterConnection"
#define ATP_BRIDGE_AIF_INF_TYPE_WANINTERFACE "WANInterface"
#define ATP_BRIDGE_AIF_INF_TYPE_LANROUTER "LANRouterConnection"//��ʱû����
#define ATP_BRIDGE_AIF_INF_TYPE_LANINTERFACE "LANInterface"

typedef enum
{
    ATP_BRIDGE_NOTIFY_DEL_FILTER_WAN_MOVE=1,
}ATP_BRIDGE_NOTIFY_DEL_FILTER;

typedef enum
{
    ATP_BRIDGE_NOTIFY_ADD_FILTER_WAN_MOVE=1,
}ATP_BRIDGE_NOTIFY_ADD_FILTER;

typedef enum
{
    ATP_BRIDGE_NOTIFY_BEFORE_DEL_BRIDGE_ROUTE6=1,
}ATP_BRIDGE_NOTIFY_BEFORE_DEL_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_DEL_BRIDGE_RIP=1,
    ATP_BRIDGE_NOTIFY_DEL_BRIDGE_RADVD,
}ATP_BRIDGE_NOTIFY_DEL_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_RIP=1,
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_RADVD,
    ATP_BRIDGE_NOTIFY_ADD_BRIDGE_ROUTE6,
}ATP_BRIDGE_NOTIFY_ADD_BRIDGE;

typedef enum
{
    ATP_BRIDGE_NOTIFY_INIT_BRIDGE_ROUTE6=1,
}ATP_BRIDGE_NOTIFY_INIT_BRIDGE;

/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C" {
#endif

VOS_UINT32 ATP_BridgeDelRoutedWANFilter_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
                                            						
VOS_UINT32 ATP_BridgeAddRoutedWANFilter_NotifyRegister(AtpNotifierProc                 pfProc,
                                                                                    VOS_UINT32                      ulPriority,
                                                                                    VOS_VOID                        *pvNotifyHook);

/*!
  \brief ͨ���ŵ�keyֵ����ȡ��Ӧ��landevice��ip��ַ����������
  \param[in]  ulByBridgeKey: �����ŵ�keyֵ
  \param[out]  *pszIpAddr: ����ip��ַ
  \param[in]  ulAddrLen: ��������ĳ�������
  \param[out]  *pszIpMask: ������������
  \param[in]  ulMaskLen: �����������ĳ�������
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_GetIPByBridgeKey (const VOS_INT32 lByBridgeKey,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,
												VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen);

/*!
  \brief ͨ���ӿڵ�domain��ȡip��ַ���������룬���ҷ����ŵ�key
  \param[in]  *pszInfPath: lan�ӿ���xml����ȫ·��������domain
  \param[out]  *pszIpAddr: ����ip��ַ
  \param[in]  ulAddrLen: ��������ĳ�������
  \param[out]  *pszIpMask: ������������
  \param[in]  ulMaskLen: �����������ĳ�������
  \param[out]  *pulRetBridgeKey: ��������Ӧ�ŵ�bridgekey
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_GetIPAddrMaskByInterfacePath(const VOS_CHAR *pszInfPath,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,
													VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen, VOS_INT32 *plRetBridgeKey);

/*!
  \brief ����lanʵ���ţ������֮��Ӧ�ŵ�keyֵ
  \param[in]  ulLanInst: LANDeviceʵ����
  \param[in]  *ulOutBridgeKey: ���ض�Ӧ���ŵ�bridgekey
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByLANInstId(const VOS_UINT32 ulLanInst, VOS_INT32 *lOutBridgeKey );
/*!
  \brief ������ip��ַ
  \param[in]  ulBridgeKey: �ŵ�keyֵ
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_SetupIP( VOS_INT32 lBridgeKey) ;
/*!
  \brief ����LANDeviceʵ���ţ�������֮��Ӧ���ŵ�ip��ַ
  \param[in]  ulLDVInst: LANDeviceʵ����
  \param[in]  VOS_CHAR*pacLanAddr: ip��ַ
  \param[in] VOS_CHAR*pacLanMask: ��������
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_SetupIP_ByInstId( VOS_UINT32 ulLDVInst, VOS_CHAR*pacLanAddr ,VOS_CHAR*pacLanMask ) ;

/*!
  \brief:
            Clear the bridge IP-Mask when disable the bridge
  \param[in]  ulLDVInst:  LANDevice instance number
  \return 
  \retval  VOS_OK: clear success
             VOS_NOK: clear failure
*/
VOS_UINT32 ATP_BRIDGE_Down_ByInstId(VOS_UINT32 ulLDVInst);
/*!
  \brief:
            �뺯��ATP_BRIDGE_Update_WAN_LAN_Interface��������ǲ����������ӿڡ�
��ʹ��brctl��Ӻ�ɾ���ӿ�
ֻ�������ݿ�
  \param[in]  ulLDVInst:  LANDevice instance number
  \return 
  \retval  VOS_OK: clear success
             VOS_NOK: clear failure
*/

VOS_UINT32 ATP_BRIDGE_Update_WAN_LAN_Interface_without_Activate(VOS_BOOL isAdd, VOS_BOOL isWAN, VOS_CHAR* pszPath);//add or delete


/*!
  \brief ����wan��lan�ӿ���Ϣ��layer2bridging�ڵ��С���Ҫ����InternetGatewayDevice.Layer2-Bridging.AvailableInterface.{i}. 
  ��������ڣ�����ӣ������������ԡ�
  ͬʱ�������filter������Ĭ����Թ�ϵ��
  ��wan��lan������ɾ���ӿڵ�ʱ�򣬻�������������
  \param[in]  isAdd: ��ӻ���ɾ��
  \param[in]  isWAN: ��ʾ��wan����lan
  \param[in]  pszPath: �����lan�����path���ǽӿ�������eth0.2.�����wan������wan�ӿڵ�domain
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_Update_WAN_LAN_Interface(VOS_BOOL isAdd, VOS_BOOL isWAN, VOS_CHAR* pszPath);
/*!
  \brief ��ʼ��bridge�����и���У��landevice�������Ƿ���ڽӿ�name�ڵ�ֵ�����û������ӣ���eth0.2.
  ���⣬��Ҫ���𴴽��š�
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/

VOS_UINT32 ATP_BRIDGE_Init (VOS_VOID);
/*!
  \brief ȥ��ʼ��bridge��ɾ����
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_NOKʧ��
  \retval VOS_OK �ɹ�
*/


#ifdef SUPPORT_ATP_LAN_VLAN
VOS_UINT32 ATP_BRIDGE_VlanInit(VOS_VOID);
VOS_VOID ATP_BRIDGE_VlanMonitor(const VOS_CHAR *pszPort);
#endif
VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfaceName(const VOS_CHAR *pszInfName, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);
VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByInterfaceName(const VOS_CHAR *pszInfName,VOS_INT32* plRetBridgeKey);

VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfacePath(const VOS_CHAR *pszInfPath, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);
//interface Path-->AIF .interfaceKey -->Filter.BridgeKeyRef
VOS_UINT32 ATP_BRIDGE_GetBridgeKeyByInterfacePath(const VOS_CHAR *pszInfPath,VOS_INT32* plRetBridgeKey);
/*!
  \brief 
  \param[in]  pszWanPath: 
  \param[in]  *lOutBridge: 
  \return 
*/

VOS_VOID ATP_BRIDGE_GetBridgeKeyByWanPath(const VOS_CHAR* pszWanPath, VOS_INT32 *lOutBridge);

VOS_UINT32 ATP_BRIDGE_GetAvailableWanPathByBridgeKey(const VOS_INT32 lBridgeKey, const VOS_CHAR* pszExcludeWanPath, VOS_CHAR* pszWanPath);

/*!
  \brief ����IGMP Snooping����
  \param[in]  bEnable: IGMP Snooping����ѡ�"enable"-->ʹ�ܣ�"disable"-->����
  \return 
  \retval ����IGMP Snooping���ܳɹ��򷵻�VOS_OK�� 
              ����IGMP Snooping����ʧ���򷵻ش�����ATP_ERR_BRIDGE_ENABLEIGMPSNOOPING_EN
*/

VOS_UINT32 ATP_BRIDGE_EnableIgmpSnooping(VOS_BOOL bEnable);

VOS_UINT32 ATP_BridgeBeforeDelBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeDelBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
VOS_UINT32 ATP_BridgeAddBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeInitBridge_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_BridgeBindChg_NotifyRegister(AtpNotifierProc                 pfProc,
                                                     VOS_UINT32                      ulPriority,
                                                     VOS_VOID                        *pvNotifyHook);

VOS_VOID BridgeBindChgDoNotify(VOS_VOID* pulInterfaceRef);

VOS_UINT32 ATP_BRIDGE_GetBridgeInstIdByBridgeKey(VOS_INT32 lInBridgeKey, VOS_UINT32 *ulOutBridgeInst );

#if defined(__cplusplus)
}
#endif



#endif
