
/**\file dhcpscmsapi.h
  *
  * Common decalarations for other modules, like dhcps, dhcpscms, wancms etc.
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/

 
#ifndef __DHCPS_CMS_API_H__
#define __DHCPS_CMS_API_H__

#include <arpa/inet.h>

#include "atptypes.h"
#include "atputil.h"
#include "msgapi.h"
#include "cfmapi.h"
#include "atp_interface.h"

#ifdef SUPPORT_ATP_TR098 
#include "wancmsapi.h"
#endif


#define ATP_LAN_IP_DEF_ADDR                 "192.168.1.1"
#define ATP_LAN_IP_DEF_MASK                 "255.255.255.0"
#define ATP_LAN_IP_DEF_ENABLE               1
#define ATP_LAN_IP_BASE_1                       "0.0.0.1"


#define ATP_LAN_ARR_LEN                 4
#define ATP_LAN_MAX                         4
#define ATP_LAN_ENET_ID                  1   
#define ATP_LAN_ENET1_ID                (ATP_LAN_ENET_ID+1)
#define ATP_LAN_USB_ID		          (ATP_LAN_ENET_ID + ATP_LAN_MAX)
#define ATP_LAN_HPNA_ID		          (ATP_LAN_USB_ID + ATP_LAN_MAX)
#define ATP_LAN_WIRELESS_ID          (ATP_LAN_HPNA_ID + ATP_LAN_MAX)
#define ATP_LAN_SUBNET_ID              (ATP_LAN_WIRELESS_ID + ATP_LAN_MAX) 
#define ATP_LAN_ENET0_VNET_ID      (ATP_LAN_SUBNET_ID + ATP_LAN_MAX)
#define ATP_LAN_ENET1_VNET_ID      (ATP_LAN_ENET0_VNET_ID + ATP_LAN_MAX)

#define ATP_LAN_DEVICE_NUM_MAX              4
#define ATP_LAN_IP_DEF_LEN_MAX               24

typedef struct {
    VOS_CHAR *  oldIP;
    VOS_CHAR *  oldMask;
    VOS_CHAR *  newIP;
    VOS_CHAR *  newMask;
} ATP_LAN_SrvInfo;

enum
{
    ATP_LAN_ETH_LINKSTATE_DOWN = 0,
    ATP_LAN_ETH_LINKSTATE_UP
};

// DHCPSCMS return code
enum ATP_DHCPSCMS_RET_CODE_EN
{    
    DHCPSCMS_RET_PARA_ERR  = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DHCPSCMS),   // input invalid parameter 
    DHCPSCMS_RET_MEM_ERR,       // memory error
    DHCPSCMS_RET_SOCKET_ERR,  // socket error
    DHCPSCMS_RET_NOT_SPPORTED,
};

//LAN Server br0 IP/Mask change register-notify priority
typedef enum
{
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_RESTART=1,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_NATUPDATE,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_FIREWALLUPDATE,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_DNSUPDATE,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_DHCPSRESTART,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_ITUNESRESTART,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_ROUTERESTART,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_SPOOFREUPDATE,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_MCASTRESTART,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_IGMPSTART,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_LANNAMEUPDATE,
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_DMSSTART,

    /* BEGIN: Added by f00120964  2012-8-8 br0地址变化时需要通知下行qos */
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_QOSCHANNEL,
    /* BEGIN: Ended by f00120964  2012-8-8 br0地址变化时需要通知下行qos */
    
    /* BEGIN: Added by lichengliang 00183184, 2012/7/23   PN:移植c06的sipserver的相关代码，修改维护地址后可以开相应端口 */
    ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE_VOICEUPDATE
    /* END:   Added by lichengliang 00183184, 2012/7/23   PN:移植c06的sipserver的相关代码，修改维护地址后可以开相应端口 */
}ATP_LAN_NOTIFY_PRIORITY_LANIPMASKCHANGE;


//LAN Server non-br0 IP/Mask change register-notify priority
typedef enum
{
    ATP_LAN_NOTIFY_PRIORITY_LAN_NON_BR0_IPMASKCHANGE_RESTART=1,

}ATP_LAN_NOTIFY_PRIORITY_LAN_NON_BR0_IPMASKCHANGE;


//DHCP Server Enable/Disable status change register-notify priority
typedef enum
{
    ATP_LAN_NOTIFY_PRIORITY_DHCPS_ENABLE_CHANGE_UPNP_UPDATE=1,
    ATP_LAN_NOTIFY_PRIORITY_DHCPS_ENABLE_CHANGE_DMS_UPDATE,
    ATP_LAN_NOTIFY_PRIORITY_DHCPS_ENABLE_CHANGE_UDC_UPDATE,
    ATP_LAN_NOTIFY_PRIORITY_DHCPS_ENABLE_CHANGE_DHCPSRESTART,

}ATP_LAN_NOTIFY_PRIORITY_DHCPS_ENABLE_CHANGE;


#if defined(__cplusplus)
extern "C" {
#endif

/*** 1. Begin:Genenal API to manipulate the LAN node InternetGatewayDevice.LANDevice.1***/
VOS_UINT32  ATP_LAN_Init(VOS_VOID);
VOS_UINT32 ATP_LAN_Set_LANHostConfigManagement(VOS_UINT32 ulLDVInst);

VOS_UINT32 ATP_LAN_GetLANDeviceInfo(VOS_UINT32 *lanInstNum, VOS_CHAR *lanArr,VOS_UINT32 *lanArrLen);
VOS_UINT32  ATP_LAN_GetAddrMask(VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen);
#ifdef SUPPORT_ATP_SECOND_IP
VOS_UINT32 ATP_LAN_GetSecondAddrMask(VOS_BOOL *pbSndIpEnable,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen);
#endif
VOS_UINT32  ATP_LAN_GetIPAddrMaskFromDB(VOS_UINT32 ulLDVInst,VOS_CHAR *pszIpAddr,VOS_UINT32 ulAddrLen,VOS_CHAR *pszIpMask,VOS_UINT32 ulMaskLen);
VOS_UINT32  ATP_LAN_GetPathByInterfaceName(VOS_CHAR *pszLanIfcPath, VOS_UINT32 ulLen, const VOS_CHAR *pszLanIfcName);
VOS_INT32  ATP_LAN_GetIndexByName(VOS_CHAR *pszLanName) ;
#ifdef SUPPORT_ATP_TR098 
VOS_UINT32  ATP_LAN_RemoveSpoofWAN(const ATP_WAN_INFO_ST* pstWan);
#endif
VOS_UINT32  ATP_LAN_AddLANDeviceObj(VOS_UINT32 *ulRetLDVInst) ;
VOS_UINT32  ATP_LAN_AddLANIF2LDVByInterfaceName_LDVInst(const VOS_UINT32 ulLDVInst, const VOS_CHAR *pszLanIfcName);
VOS_UINT32  ATP_LAN_DelLANIFByInterfaceName(const VOS_CHAR *pszLanIfcName);
VOS_UINT32  ATP_LAN_Del_Recover_LANIF2LDV1_ByLDVInst(const VOS_UINT32 ulLDVInst);
/*** 1. End: Genenal API to manipulate the LAN node InternetGatewayDevice.LANDevice.1***/


/*** 2. Begin: For DHCPS status change (IP/Mask change, or Enable/Disable status change) register/de-regsiter-update API ***/

VOS_UINT32 ATP_LAN_SrvIPChanged_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID  *pvNotifyHook);

VOS_VOID LanSrvIPChangedDoNotify(VOS_VOID* lanSrvInfo);

VOS_UINT32 ATP_LAN_SrvNonBr0IPChanged_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, VOS_VOID *pvNotifyHook);

VOS_UINT32 ATP_DhcpSrv_EnableChanged_NotifyRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority,  VOS_VOID *pvNotifyHook);

/*** 2. End: For DHCPS status change (IP/Mask change, or Enable/Disable status change) register/de-regsiter-update API ***/


/*** 3. Begin: For DHCPS Option register/de-regsiter-update API ***/

VOS_UINT32 ATP_DHCPSCMS_OptionRegister(AtpNotifierProc  pfProc, VOS_UINT32  ulPriority, const ATP_DHCPS_REG_INFO_ST *pstRegInfo, 
                                                                                        const ATP_UTIL_NOTIFY_ENTRY_ST **pvNotifyHook);

VOS_VOID    ATP_DHCPSCMS_OptionUnRegister(const VOS_CHAR *pSubscriber);

VOS_BOOL    ATP_DHCPSCMS_OptionUpdate(const ATP_DHCPS_REG_INFO_ST *pstRegInfo);

VOS_VOID    ATP_DHCPSCMS_OptionDoNotify(const ATP_MSG_HEADER_ST *pstMsg);

const ATP_DHCPS_OPTION_VALUE_ST *ATP_LAN_GetOptionByTag(const ATP_DHCPS_NOTIFY_INFO_ST *pstNotifyInfo, VOS_UINT8 tag);
/*** 3. End: For DHCPS Option register/de-regsiter-update API ***/

#ifdef SUPPORT_ATP_TR181 
VOS_UINT32 ATP_TR181_AllowedMAC_Remove(const VOS_CHAR *pstSrcMac);
#endif

VOS_BOOL ATP_UTIL_ValidateIpSameSubNet(VOS_INT *pLan1a, VOS_INT *pLan1m, VOS_INT *pLan2a, VOS_INT *pLan2m);

#ifdef SUPPORT_ATP_RIP
VOS_UINT32 ATP_LAN_GetBridgeKeyByLanPath(VOS_CHAR * pszLanPath, VOS_INT32 *pulBrID);
VOS_UINT32 ATP_LAN_GetBrIdByLanId(const VOS_UINT32 ulLanID, VOS_INT32 *pulBrID );
#endif


#if defined(__cplusplus)
}
#endif

#endif
