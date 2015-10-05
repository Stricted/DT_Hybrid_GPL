/**\file wancmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author j00196195
  *\date  2011-12-27
*/
/*******************************************************************************
Edit History of Huawei: 
<�޸���>     <�޸�ʱ��>    <���ⵥ��>          <�޸�����>
z00159386    2012/10/16    DTS2012102201180     TELUS�������󣬰���Voicemail�����ο��ء�SMS�ض���
*******************************************************************************/
#ifndef __WAN_CMS_API_H__
#define __WAN_CMS_API_H__

#include "atptypes.h"
//#include "atperrdef.h"
#include "msgapi.h"
#include "cfmapi.h"
#include "atputil.h"
#include "atpwancore.h"
#include "wancorecmsapi.h"
#include "wancorecmsgetapi.h"
#ifdef SUPPORT_ATP_ATP_FMCWEB
#include "wuprofile.h"


/* < DTS2012102200885 z81004485 2012/10/24 begin  */
#define PPP_IF_NAME "ppp"
#define NAS_IF_NAME "nas1"
/* DTS2012102200885 z81004485 2012/10/24 end> */

#endif
//!\brief 1	ATP_WAN_PreInit_NotifyRegisterʹ�õĺ�
/*!WANCMS ��ʼ��ǰ
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_NAT_INIT=1,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_DNS_INITP,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_DNS_INIT,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_WANIPv6NL,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_FIREWALL_INIT,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_TUNNEL_INIT,
    ATP_WAN_NOTIFY_PRIORITY_PREINIT_WANBACKUP_INIT,
}ATP_WAN_NOTIFY_PRIORITY_PREINIT;

//!\brief 2	ATP_WAN_PostInit_NotifyRegisterʹ�õĺ�
/*!WANCMS��ʼ����
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_PPPOES=1,
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_ROUTE_INIT,
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_ROUTE6_INIT,  
    ATP_WAN_NOTIFY_PRIORITY_POSTINIT_NATOTHER_INIT,
}ATP_WAN_NOTIFY_PRIORITY_POSTINIT;


//!\brief 3	ATP_WAN_PreUNInit_NotifyRegisterʹ�õĺ�
/*!WANCMS ȥ��ʼ��ǰ
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREUNINIT_NULL=1,
}ATP_WAN_NOTIFY_PRIORITY_PREUNINIT;


//!\brief 4	ATP_WAN_PostUNInit_NotifyRegisterʹ�õĺ�
/*!WANCMS ȥ��ʼ����
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_POSTUNINIT_DNS_UNINIT=1,
}ATP_WAN_NOTIFY_PRIORITY_POSTUNINIT;

//!\brief 5	ATP_WAN_WanConfigProcess_NotifyRegisterʹ�õĺ�
/*!WAN�����иı䣺WanConfigProcess
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS_CWMPPPPUSER=1,
    ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS_UPNP,
    ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS_DNSRESTART,
    ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS_ALG_SIPSIG,
    ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS_FASTFWDING,
}ATP_WAN_NOTIFY_PRIORITY_CONFIGPROCESS;

//!\brief 6	ATP_WAN_PreWanInit_NotifyRegisterʹ�õĺ�
/*!wan connection ��ʼ��ǰ
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREWANINIT_NULL=1,
}ATP_WAN_NOTIFY_PRIORITY_PREWANINIT;

//!\brief 7	ATP_WAN_PostWanInit_NotifyRegisterʹ�õĺ�
/*!wan connection ��ʼ����
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_POSTWANINIT_ACCSTART=1,
    ATP_WAN_NOTIFY_PRIORITY_POSTWANINIT_WANRIPSTART,
}ATP_WAN_NOTIFY_PRIORITY_POSTWANINIT;

//!\brief 8	ATP_WAN_PreWanUNInit_NotifyRegisterʹ�õĺ�
/*!wan connection ȥ��ʼ��ǰ
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_PREWANUNINIT_ACCSTOP=1,
}ATP_WAN_NOTIFY_PRIORITY_PREWANUNINIT;

//!\brief 9	ATP_WAN_PostWanUNInit_NotifyRegisterʹ�õĺ�
/*!wan connection ȥ��ʼ����
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_POSTWANUNINIT_NULL=1,
}ATP_WAN_NOTIFY_PRIORITY_POSTWANUNINIT;


//!\brief 11	ATP_WAN_bridgeWANUp_NotifyRegisterʹ�õĺ�
/*! ��ģʽWAN up
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_BRIDGEWANUP_QOSWANSTART=1,
}ATP_WAN_NOTIFY_PRIORITY_BRIDGEWANUP;


//!\brief 12	ATP_WAN_DHCPSpoofedWANUp_NotifyRegisterʹ�õĺ�
/*! DHCPSpoofed WAN up
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANUP_START=1,
    ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANUP_ROUTESTART,
}ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANUP;

//!\brief 14	ATP_WAN_bridgeWANDown_NotifyRegisterʹ�õĺ�
/*! ��ģʽWAN down
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_BRIDGEWANDOWN_QOSWANSTOP=1,
}ATP_WAN_NOTIFY_PRIORITY_BRIDGEWANDOWN;


//!\brief 15	ATP_WAN_DHCPSpoofedWANDown_NotifyRegisterʹ�õĺ�
/*! DHCPSpoofed WAN down
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANDOWN_STOP=1,
    ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANDOWN_ROUTESTOP,
}ATP_WAN_NOTIFY_PRIORITY_DHCPSPOOFEDWANDOWN;


//!\brief 21	ATP_WAN_WanRelationClear_NotifyRegisterʹ�õĺ�
/*! 
 �����WAN�����������
 WanRelationClear

 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_ROUTECLEAR=1,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_QOSWANCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_FIREWALLWANCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_NATWANCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_ALGSIPCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_DDNSREMOVERULES,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_IGMPPROXYCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_MLDPROXYCLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_VPNWANRULECLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_PTWANRULECLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_ROUTE6CMSWANRULECLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_ETHERIPROUTECLEAR,
    ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR_TUNNELCLEAR,
}ATP_WAN_NOTIFY_PRIORITY_WANRELATIONCLEAR;


//!\brief 22	ATP_WAN_WanConnEndAddSetMsgPro_NotifyRegisterʹ�õĺ�
/*! 
 ip/ppp connection end addsetmsgpro hook

 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_IP_PPP_ADDSETMSGPRO_SIGNAL2IPCHECK=1,
}ATP_WAN_NOTIFY_PRIORITY_IP_PPP_ADDSETMSGPRO;


//!\brief 23	ATP_WAN_Single_Stopʹ�õĺ�
/*! 
 ATP_WAN_Single_Stop
 *
 */
typedef enum
{
    ATP_WAN_NOTIFY_PRIORITY_WANSINGLESTOP_ROUTESTOP=1,
}ATP_WAN_NOTIFY_PRIORITY_WANSINGLESTOP;


typedef struct tagATP_Delay_Effect_ST
{
    ATP_WAN_INFO_ST *pstWan;
    ATP_WAN_INFO_ST *pstCur;
    VOS_UINT32 *pulValues;
} ATP_Delay_Effect_ST;


#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief wancms�ĳ�ʼ��
  \param[in] VOS_VOID: 
  \return VOS_UINT32
  \retval VOS_NOK ��ʼ��ʧ��
*/
VOS_UINT32 ATP_WAN_Init(VOS_VOID);
VOS_INT32 ATP_WAN_ConnectionType(VOS_VOID);
/*!
  \brief wancms��ȥ��ʼ��
  \param[in] VOS_VOID: 
  \return VOS_UINT32
  \retval VOS_NOK ȥ��ʼ��ʧ��
*/
VOS_UINT32 ATP_WAN_UnInit(VOS_VOID);

/*!
  \brief ����WAN����
  \param[in]  acPath: WAN·��
  \return VOS_UINT32
  \retval VOS_NOK ����ʧ��
*/
VOS_UINT32 ATP_WAN_ConnectionInit(VOS_CHAR* acPath);
/*!
  \brief ֹͣWAN����
  \param[in]  acPath: WAN·��
  \return VOS_UINT32
  \retval VOS_NOK ֹͣʧ��
*/
VOS_UINT32 ATP_WAN_ConnectionUnInit(VOS_CHAR* acPath);

/*!
  \brief ��·״̬�仯
  \param[in]  ulWdInstId: ��һ��WANʵ����
  \param[in]  ulWcdInstId: �ڶ���WANʵ����
  \param[in]  ulLinkStatus: ��·״̬
  \return VOS_VOID
*/
VOS_VOID ATP_WAN_LinkChanged(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, VOS_UINT32 ulLinkStatus);

/*!
  \brief ���WAN��״̬�仯
  \param[in]  *pstMsg: ��Ϣ
  \return VOS_VOID
*/
VOS_VOID ATP_WAN_Monitor(const ATP_MSG_HEADER_ST *pstMsg);

/* <DTS2012083003620 z81004485 2012/9/11 begin  */
#ifdef SUPPORT_ATP_FIREWALL_B890
/*!
  \brief ����Ӳ���ϱ���һ��������Ϣ
  \param[in]  *pstMsg: ��Ϣ
  \return VOS_VOID
*/
VOS_VOID ATP_WAN_ManualDial(const ATP_MSG_HEADER_ST *pstMsg);
#endif  /* SUPPORT_ATP_FIREWALL_B890 */
/* DTS2012083003620 z81004485 2012/9/11 end> */


/*!
  \brief ɾ����VLAN �µ�����WAN
  \param[in]  pszLowerLayer: �²�·��
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WAN_VlanClear(VOS_CHAR* pszLowerLayer);

/*!
  \brief ������VLAN�µ�����WAN
  \param[in]  pszLowerLayer: �²�·��
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WAN_VlanStart(const VOS_CHAR* pszLowerLayer);

/*!
  \brief ֹͣ��VLAN�µ�����WAN
  \param[in]  pszLowerLayer: �²�·��
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WAN_VlanStop(const VOS_CHAR* pszLowerLayer);

/*!
  \brief ���WAN�������
  \param[in]  ulWdInstId: ��һ��WANʵ����
  \param[in]  ulWcdInstId: �ڶ���WANʵ����
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_Clear(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId);

/*!
  \brief ����WAN����
  \param[in]  ulWdInstId: ��һ��WANʵ����
  \param[in]  ulWcdInstId: �ڶ���WANʵ����
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_Start(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId);

/*!
  \brief 
  \param[in]  ulWdInstId: ��һ��WANʵ����
  \param[in]  ulWcdInstId: �ڶ���WANʵ����
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_Stop(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId);

/*!
  \brief ��ȡ��һ������INTERNET���͵�WAN����
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
ATP_WAN_INFO_ST* ATP_WAN_GetFirstInternetWan(VOS_VOID);

/*!
  \brief WAN����ص���ں���
  \param[in]  pstMsg: ��Ϣ��ָ��
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_VOID ATP_WAN_CmdCallBack(VOS_VOID* pstMsg);


#ifdef SUPPORT_ATP_VOICE
/*!
  \brief �ж��Ƿ�WAN�б��д��������ӿ�
  \param[in] void: 
  \return VOS_UINT32
  \retval ��������򷵻ظýӿ��������򷵻�NULL
*/
VOS_CHAR *ATP_WAN_GetVoiceInterfaceByService(void);

/*!
  \brief ��ȡ����ʹ�õĽӿ���API
  \param[in]  ulIfcLen: �ӿ�������ATP_WAN_IFC_LEN
  \param[in]  pszInterface: WAN�ӿ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetVoiceInterface(VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);

/*!
  \brief WAN����(VOICE�ӿ�)�ı��֪ͨVOICE
  \param[in]  ULIfcLen: �ı��Ľӿڳ���
  \param[in]  *pszInterface: �ı��Ľӿ�����
  \return VOS_UINT32
  \retval�ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_VoiceNotify(VOS_UINT32 ULIfcLen, VOS_CHAR *pszInterface);

/*!
  \brief ��ȡʵ�ʵ�WAN����״̬
  \param[in]  *pszInterface: WAN����·��
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_Wan_GetVoiceInterfaceStatus(VOS_CHAR *pszInterface);
/*!
  \brief �������øı�
  \param[in] VOS_VOID: 
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_VoiceUpdate(VOS_VOID);
/*!
  \brief �������
  \param[in]  *cOldName: ԭʼ����
  \return VOS_BOOL
  \retval VOS_FALSEУ��ʧ��VOS_TRUEУ��ɹ�
*/
VOS_BOOL ATP_WAN_VoiceCheck(VOS_CHAR *cOldName);
#endif

/*!
  \brief ��ӡWAN��Ϣ
  \param[in] VOS_VOID: 
  \return VOS_VOID
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_VOID ATP_WAN_DisplayDebug(VOS_VOID);

/*!
  \brief GMSG������
  \param[in]  *pvGMsg: ��Ϣ��ָ��
  \param[in]  ulSubType: �������
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_VOID ATP_WAN_GMsgProc(VOS_VOID *pvGMsg, VOS_UINT32 ulSubType);

/* start of AU4D01630 by d00107688 2009-05-23 ����������VOICE ���͵�WAN ��������NAT,IGMP ��ѡ���� */

/*!
  \brief У�������WAN�Ƿ��������·��
  \param[in]  *pszWanPath: WAN����·��
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_ValidWan(VOS_CHAR *pszWanPath);

#ifdef SUPPORT_ATP_IGMP_PROXY_NOT_NEED_NAT
/*  ATP_WAN_ValidWan ����չ�������ж�NAT�Ƿ�ʹ��*/
VOS_UINT32 ATP_WAN_ValidWanExt(VOS_CHAR *pszWanPath);
#endif 

/*!
  \brief У�������WAN�Ƿ��������·��
  \param[in]  *pszWanPath: WAN����·��
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_ValidWanWithoutNat(VOS_CHAR *pszWanPath);
/* end of AU4D01630 by d00107688 2009-05-23 ����������VOICE ���͵�WAN ��������NAT,IGMP ��ѡ���� */
/*!
  \brief IP WAN�ӳ���Ч
  \param[in]  ATP_Msg: 
  \return 
*/
void ATP_WAN_IP_DelayEffect(void * ATP_Msg);

/*!
  \brief PPP WAN�ӳ���Ч
  \param[in]  ATP_Msg: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
void ATP_WAN_PPP_DelayEffect(void * ATP_Msg);

/*!
  \brief ͨ��IP����ȡWAN
  \param[in]  *pszIp: WAN����IP
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetWanByIp(const VOS_CHAR* pszIp);

/*!
  \brief ͨ��IP����ȡWAN PATH
  \param[in]  *pszIp: WAN����IP
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_CHAR* ATP_WAN_GetWanPathByIp(const VOS_CHAR* pszIp);

#ifdef SUPPORT_ATP_WANUMTS
/*!
  \brief ��ȡUMTS WAN��ָ��
  \param[in]  
  \return 
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetUMTSWan(VOS_VOID);
void ATP_WAN_SetApn(ATP_WAN_INFO_ST* pstWan, const VOS_CHAR* pszIn);
void ATP_WAN_SetDialNum(ATP_WAN_INFO_ST* pstWan, const VOS_CHAR* pszIn);
#endif

#ifdef SUPPORT_ATP_IPV6
/*!
  \brief ��ȡWAN��IPv6ʹ�ܿ���API
  \param[in]  pszPath: WAN����·��      
  \return VOS_UINT32
  \retval�ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetIp6Enable(VOS_CHAR* pszPath);

/*!
  \brief ��ϵͳ����ʱ, �������ݿ�����������Ƿ�����IPv6����
  \param[in] VOS_VOID: 
  \return 
  \retval �ɹ�OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_IPV6_Init(VOS_VOID);
#endif


/*!
  \brief WAN��ʼ��ǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN��ʼ�����ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PostInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WANȥ��ʼ��ǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreUNInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WANȥ��ʼ�����ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PostUNInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN�������õ�ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanConfigProcess_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);


/*!
  \brief WAN��ʼ�����ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PostWanInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WANֹͣǰ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PreWanUNInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WANֹͣ���ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PostWanUNInit_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief ��UPʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_bridgeWANUp_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN��ʼ����ʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv4Connecting_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN����ʧ��ʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanIpv4ConnectFail_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN UPʱDHCP SPOOF��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_DHCPSpoofedWANUp_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief �Ž�WAN DOWNʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_bridgeWANDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN DOWNʱDHCP SPOOF��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_DHCPSpoofedWANDown_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
/*!
  \brief WAN��������Ϣʱ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanRelationClear_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN����޸ĺ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanConnEndAddSetMsgPro_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WANֹͣ��ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_WanSingleStop_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
																	VOS_VOID                        *pvNotifyHook);


#ifdef SUPPORT_ATP_FIREWALL_B890
VOS_UINT32 ATP_WAN_WanLinkChange_NotifyRegister(AtpNotifierProc pfProc,
                                            	VOS_UINT32 ulPriority,
                                          		VOS_VOID *pvNotifyHook);

VOS_UINT32 WanLinkChangeNotifierDoNotify(VOS_CHAR* pszWanPath);
#endif  /* SUPPORT_ATP_FIREWALL_B890 */

#ifdef SUPPORT_ATP_IPV6
/*!
  \brief IPV6 INIT���ע�ắ��
  \param[in]  pfProc: ע��Ĵ�����
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: ���Ӳ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_MSG_RET_PARA_ERR
*/
VOS_UINT32 ATP_WAN_PostIPV6Init_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);
#endif

#ifdef SUPPORT_ATP_UNIVERSAL_XTM_QOS
VOS_UINT32 ATP_WAN_PPPOAWanUpProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_WAN_PPPOAWanDnProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

VOS_UINT32 ATP_WAN_GetInterfaceByNas(VOS_CHAR* pszNas, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);
#endif

/* < DTS2012102200885 z81004485 2012/10/24 begin  */
#ifdef SUPPORT_ATP_WAN_STATS_OVERFLOW
/* ��չWAN���ͺͽ����ֽ�������ֵ��Χ��֧�ֳ���4g */
/*  ���ڼ��ÿ��WAN�����ϵ��շ������ֽ�ͳ���Ƿ��������ת */
VOS_VOID ATP_WAN_StatOverCheck(VOS_VOID* pvArg);

/* ��ָ����WAN���ӵ�OVERFLOW������λ */
VOS_VOID ATP_WAN_StatOverCntReset(ATP_WAN_INFO_ST* pstWan);

/* <DTS2013102206837  z81004485 2013/10/22 begin  */
#ifdef SUPPORT_ATP_WAN_STATS_OVERFLOW_SAVE
VOS_VOID ATP_WAN_UMTS_StatInit();
VOS_VOID ATP_WAN_UMTS_StatSave();
VOS_VOID ATP_WAN_Save_umts_stat(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_WAN_Clear_Umts_stat(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_WAN_UMTS_GetStat(WAN_STAT_OVERFLOW_COUNT_ST* pstStatInfo);

/* <DTS2013110505441 z81004485 2013/11/5 begin  */
VOS_VOID ATP_WAN_Ethernet_StatInit();
VOS_VOID ATP_WAN_Ethernet_StatUpdate(const ATP_WAN_INFO_ST* pstWan);
VOS_VOID ATP_WAN_Ethernet_StatSave();
VOS_VOID ATP_WAN_Save_Ethernet_stat(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_WAN_Clear_Ethernet_stat(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_WAN_Ethernet_GetStat(WAN_STAT_OVERFLOW_COUNT_ST* pstStatInfo);
VOS_VOID ATP_WAN_Save_Wan_stat(const ATP_MSG_HEADER_ST *pstMsg);
/* DTS2013110505441 z81004485 2013/11/5 end> */

VOS_VOID WAN_Stat_Overflow_Check_Func(VOS_VOID* pvArg);
#endif
/* DTS2013102206837  z81004485 2013/10/22 end> */

#endif

int clear_net_stats(const char *name);

/* ���������� */
VOS_VOID ATP_Wan_ConnStatReset(ATP_WAN_INFO_ST* pstWan);
/* DTS2012102200885 z81004485 2012/10/24 end> */


/*
 25	pre WAN update info hook
 	WanUpdateInfo

    ���⣬��һ��post wan update info ���Ϊ10
*/

/* <DTS2012072100279 z00189398 2012-07-21 begin */
#ifdef SUPPORT_ATP_DNS_REDIRECT
VOS_INT32 WanSetRedirection(VOS_VOID);
VOS_INT32 WanRemoveRedirection(VOS_VOID);
VOS_INT32 WanProcSetRedirection(const ATP_MSG_HEADER_ST *pstMsg);
VOS_INT32 WanProcRemoveRedirection(const ATP_MSG_HEADER_ST *pstMsg);
#endif
/* DTS2012072100279 z00189398 2012-07-21 end> */
/* <DTS2012102201180  z00159386  20121012  begin */
#ifdef SUPPORT_ATP_NOTIFICATION
VOS_INT32 NotificationSetRedirection(VOS_VOID);
VOS_INT32 NotificationRemoveRedirection(VOS_VOID);
VOS_VOID NotificationProcSetRedirection(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID NotificationProcRemoveRedirection(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID NotificationProcDelayTimer(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID NotificationProcCancelTimer(const ATP_MSG_HEADER_ST *pstMsg);
#endif /* SUPPORT_ATP_NOTIFICATION */
/* DTS2012102201180  z00159386  20121012  end> */
/*  <DTS2013022804321 wanglong 2012-2-28 begin */
#ifdef SUPPORT_ATP_PLMN_AUTO_MATCH
VOS_UINT32 plmn_match_profile(VOS_UINT32 profile_index);
/* <DTS2013062201618 z81004143 2013/6/25 begin */
/* <DTS2013070300449 z81004143 2013/7/3 begin */
void ATP_WAN_restoreUmtsConnection(void);
void ATP_WAN_retryProfileStart(void);
void ATP_WAN_retryProfileFinish(void);
void ATP_WAN_retryNextProfile(VOS_UINT32 curent_idx);
/* DTS2013070300449 z81004143 2013/7/3 end> */
/* DTS2013062201618 z81004143 2013/6/25 end> */
/* <DTS2013072304658 z81004143 2013/7/25 begin */
#ifdef SUPPORT_ATP_B880_CUBE
void ATP_WAN_getProfilePublicPara(WU_PROFILE_INFO_ST *proFileInfo);
void ATP_WAN_SetUmtsConnTriggerManual(const ATP_MSG_HEADER_ST *pstMsg);
#endif
/* DTS2013072304658 z81004143 2013/7/25 end> */
#endif
/*  DTS2013022804321 wanglong 2012-2-28 end> */
/* <DTS2012102703614 z81004485 2012/10/31 begin  */
VOS_BOOL ATP_Wan_IsOnDemandTrigger(ATP_WAN_INFO_ST* pstWan);
/* DTS2012102703614 z81004485 2012/10/31 end> */

/* <DTS2012101207542  w00211169 2012-11-16 begin */
VOS_BOOL ATP_Wan_ClearIpv6GW(ATP_WAN_INFO_ST* pstWan);
/* DTS2012101207542  w00211169 2012-11-16 end> */

#if defined(__cplusplus)
}
#endif


#endif

