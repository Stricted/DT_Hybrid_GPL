#ifndef __ATP_IPCHECK_MSGTYPES_H__
#define __ATP_IPCHECK_MSGTYPES_H__


#define ATP_CBBID_IPCHECK_NAME      "ipcheck"

#define ATP_IPCHECK_MAC_LEN_MAX         18
#define ATP_IPCHECK_HOST_NAME_LEN_MAX       64
#define ATP_IPCHECK_APP_LEN_MAX         32
#define ATP_HOST_ADDR_TYPE_LEN  8
#define ATP_IPCHECK_LAYER2IFC_LEN_MAX    256
#define ATP_INTERFACE_LEN 16
#define ATP_ASSOCIATED_DEVICE_PATH_LEN        256

#define ATP_HOST_STATIC_ADDRESSING  "Static"
#define ATP_HOST_DHCP_ADDRESSING     "DHCP"


typedef struct tagATPNtwkHostInfo 
{
    VOS_UINT32  ulYiaddr;
    VOS_UINT32  ulExpires;
    VOS_UINT8   acChAddr[ATP_IPCHECK_MAC_LEN_MAX];
    VOS_CHAR    acVendorID[ATP_IPCHECK_APP_LEN_MAX];
    VOS_CHAR    acClientID[ATP_IPCHECK_APP_LEN_MAX];
    VOS_CHAR    acUserID[ATP_IPCHECK_APP_LEN_MAX];
    VOS_CHAR    acHostName[ATP_IPCHECK_HOST_NAME_LEN_MAX];
    VOS_BOOL    bActive;
    VOS_CHAR    acBridgeName[ATP_INTERFACE_LEN];
    VOS_UINT32  ulStartTime;
    VOS_UINT32  ulFlags;
    
#ifdef SUPPORT_ATP_NAT_PORTMAPPING_RULES_WITH_SAMENAME
    VOS_CHAR    acDeviceName[ATP_IPCHECK_APP_LEN_MAX];
    VOS_CHAR    acDeviceType[ATP_IPCHECK_APP_LEN_MAX];
#endif

} ATP_HOST_INFO;


//TR181 specific CMO marco, the data model same as tr098, but with subtle different names
#ifdef SUPPORT_ATP_TR181

// Device.Hosts.
#define CMO_LDEV_HSS_HOST_MACADDRESS    CMO_LDEV_HSS_HOST_PHYSADDRESS  

// Device.Ethernet.Interface.{i}.
#define CMO_LDEV_LEIFC_LANETHERNETINTERFACECONFIG_OBJ    CMO_ETH_IF_INTERFACE_OBJ
#define CMO_LDEV_LEIFC_NAME    CMO_ETH_IF_NAME 

#define CMO_LDEV_WLC_WLANCONFIGURATION_OBJ    CMO_WF_SSID_SSID_OBJ
#define CMO_LDEV_WLC_NAME    CMO_WF_SSID_NAME

#endif


enum ATP_IPCHECKCMS_MSGTYPES_EN
{
    ATP_MSG_IPCHECKCMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_IPCHECKCMS),
    ATP_MSG_IPCHECKCMS_DELETE_LEASE,                       // lancms inform dhcp delete lease info
    ATP_MSG_IPCHECKCMS_HOST_ARP_REPLY,                 // lancms inform ipcheck check host info  
    
    ATP_MSG_IPCHECKCMS_HOST_NEWLY_ADD,               // ipcheck inform ipcheckcms event, new host added to LAN Host
    ATP_MSG_IPCHECKCMS_HOST_REMOVE,                     // ipcheck inform ipcheckcms event, host removed from the LAN Host
    ATP_MSG_IPCHECKCMS_HOST_UPDATE,                     // ipcheck inform ipcheckcms event, host got update to the LAN Host
    ATP_MSG_IPCHECKCMS_HOST_EIXSTED,                   // ipcheck inform ipcheckcms event, host already existed in the LAN Host
    ATP_MSG_IPCHECKCMS_HOST_WLAN_CONNECT,      //for wlan
} ;


enum ATP_IPCHECK_MSGTYPES_EN
{
    ATP_MSG_IPCHECK_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_IPCHECK),

    ATP_IPCHECK_NEIGHBORS_UPDATE,    // Neighbors info update
    ATP_IPCHECK_MSG_TEST_SHOW,
    ATP_IPCHECK_MSG_TEST_BRIDGECHG,
    ATP_IPCHECK_MSG_RELOAD_BR_ADDR,
    
    ATP_MSG_IPCHECK_END,
} ;
#endif // End of __ATP_IPCHECK_MSGTYPES_H__

