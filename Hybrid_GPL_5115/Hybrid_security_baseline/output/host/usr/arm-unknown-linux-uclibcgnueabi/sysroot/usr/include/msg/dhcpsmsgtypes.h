#ifndef __ATP_DHCPS_MSGTYPES_H__
#define __ATP_DHCPS_MSGTYPES_H__


#define ATP_CBBID_DHCPS_NAME "dhcps"
#define ATP_DHCP_SERVER_LEASE_FILE  "/var/dhcp/dhcps/leases"
#define DHCPS_SPOOF_FILE_PATH           "/var/dhcp/dhcps/spoof_passthrough"


#define DHCPS_ALOC_WAN_TYPE_LEN      16
#define DHCPS_WAN_CONN_LEN_MAX       128

//TR181 specific CMO marco, the data model same as tr098, but with subtle different names
#ifdef SUPPORT_ATP_TR181

// Dhcpv4.Server
#define CMO_LDEV_LHCM_DHCPSERVERENABLE  CMO_LDEV_LHCM_DHCPS_ENABLE
#define CMO_LDEV_LHCM_DHCPCONDITIONALPOOLNUMBEROFENTRIES  CMO_LDEV_LHCM_DHCPS_POOLNUMBEROFENTRIES  

// Dhcpv4.Server.pool.{i}.
#define CMO_LDEV_LHCM_IPPOOL_DHCPCONDITIONALSERVINGPOOL_OBJ  CMO_LDEV_LHCM_IPPOOL_POOL_OBJ 
#define CMO_LDEV_LHCM_IPPOOL_POOLORDER  CMO_LDEV_LHCM_IPPOOL_ORDER
#define CMO_LDEV_LHCM_IPPOOL_DHCPLEASETIME  CMO_LDEV_LHCM_IPPOOL_LEASETIME 
#define CMO_LDEV_LHCM_IPPOOL_DHCPOPTIONNUMBEROFENTRIES  CMO_LDEV_LHCM_IPPOOL_OPTIONNUMBEROFENTRIES

// Dhcpv4.Server.pool.{i}.option.{i}.
#define CMO_LDEV_LHCM_IPPOOL_DHCP_OPTION_DHCPOPTION_OBJ CMO_LDEV_LHCM_IPPOOL_DHCP_OPTION_OPTION_OBJ 

//Device.IP.Interface.{i}.IPv4Address.{i}
#define CMO_LDEV_LHCM_IPIF_IPINTERFACEIPADDRESS         CMO_IP_IF_IPV4ADDR_IPADDRESS 
#define CMO_LDEV_LHCM_IPIF_IPINTERFACESUBNETMASK      CMO_IP_IF_IPV4ADDR_SUBNETMASK 

#endif


enum ATP_DHCPS_MSGTYPES_EN
{
    ATP_MSG_DHCPS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DHCPS),
    ATP_MSG_DHCPSCMS_SERVER_INFO,                          // DHCP send server info to IPcheck, including mac, pool [start, end], dhcps pid
    ATP_MSG_DHCPSCMS_SYNC_DEVICE_INFO,               // sync device info
    ATP_MSG_DHCPSCMS_SPOOF_WAN_DOWN,               // lancms inform dhcp SPOOF WAN status down
    ATP_MSG_DHCPSCMS_SPOOF_WAN_UP,                    // lancms inform dhcp SPOOF WAN status up
    ATP_MSG_DHCPSCMS_LAN_STATUS_DOWN,             // lancms inform ipcheck eth lan port status change
    ATP_MSG_DHCPSCMS_LAN_STATUS_UP,                  // lancms inform ipcheck eth lan port status up change
    ATP_MSG_DHCPSCMS_WLAN_STATUS_DOWN,         // lancms inform ipcheck wlan port status up change
    ATP_MSG_DHCPSCMS_LHCM_UPDATE,                     // update the LHCM fields which not need to restart dhcps
    ATP_MSG_DHCPSCMS_SERVING_POOL_UPDATE,    // update the serving pool fields which not need to restart dhcps
    ATP_MSG_DHCPSCMS_STATIC_ADDR_ADD,            // add static address node
    ATP_MSG_DHCPSCMS_STATIC_ADDR_DELETE,      // delete static address node
    ATP_MSG_DHCPSCMS_OPTION_ADD,                     // add option node
    ATP_MSG_DHCPSCMS_OPTION_DELETE,               // delete option node
    ATP_MSG_DHCPSCMS_REG_OPTION_UPDATE,      // dhcpscms inform dhcps for subscribed option add-delete update
    ATP_MSG_DHCPSCMS_NOTIFY_OPTION,              // dhcps inform dhcpscms for option notification
    ATP_MSG_DHCPS_UPDATE_LEASE,             // Tell ipcheck that lease file has been updated
    ATP_MSG_DHCPS_ADD_IPMAC,
    ATP_MSG_DHCPS_DEL_IPMAC,
    ATP_MSG_DHCPS_TEST_SHOW,                // Show debug info
} ;

// WAN SPOOF data structure
typedef struct __tagDhcpSpoofInfo
{
    VOS_CHAR    passThruWanConn[DHCPS_WAN_CONN_LEN_MAX];        
    VOS_UINT32  passThruIPAddr;                  
    VOS_UINT32  passThruRouterAddr;              
    VOS_UINT32  passThruMaskAddr;                  
    VOS_UINT32  passThruDnsSrvOne;          
    VOS_UINT32  passThruDnsSrvTwo;           
} DHCPS_SPOOF;


/*** Start: For Layer2Bridging Option subscription declaration  ***/
enum
{
    ATP_DHCPS_SYNC_OPTION_REG = 0,
    ATP_DHCPS_ASYNC_OPTION_REG
};

#define REG_OPTION_NUM_MAX 3
#define REG_OPTION_LEN_MAX  256
#define MAC_ADDR_LEN 6
#define SUBCSRIBER_NAME_LEN 12
#define SUBSCRIBER_NUM_MAX  8

typedef struct tagATP_DHCPS_REG_INFO_ST
{
    VOS_CHAR       subscriber[SUBCSRIBER_NAME_LEN];   // module name who register the subscription, used to identify the subscriber
    VOS_UINT8      usRegType;               // Syn or async, if sync, dhcps need wait for dhcpscms reply
    VOS_UINT8      msgType;                 // which request msg it comes, like DISCOVER, REQUEST, INFORM, RELEASE
    VOS_UINT8      usNumOfOptionID;
    VOS_UINT8      aulOptionIDArray[REG_OPTION_NUM_MAX];
} ATP_DHCPS_REG_INFO_ST;

typedef struct tagATP_DHCPS_OPTION_VALUE_ST
{
    VOS_UINT8      ulTag;
    VOS_UINT8      ulLen;
    VOS_UINT8      aucOptionValue[REG_OPTION_LEN_MAX];
}ATP_DHCPS_OPTION_VALUE_ST;

// All option values for the current discover package
typedef struct tagATP_DHCPS_NOTIFY_INFO_ST
{
    VOS_UINT8        aucMacAddr[MAC_ADDR_LEN];
    VOS_UINT32      ulNumOfOptionID;
    ATP_DHCPS_OPTION_VALUE_ST astOptions[0];
} ATP_DHCPS_NOTIFY_INFO_ST;
/*** End: For Layer2Bridging Option subscription declaration  ***/


#endif // End of __ATP_DHCPS_MSGTYPES_H__


