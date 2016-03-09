
/**\file LanHostCfgMgr.h
  *
  * Declarations for the ServerPool list, StaticAddress table, Option list, UserAllocatedWAN data structure. 
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/

#ifndef _LAN_HOST_CFG_MGR_H
#define _LAN_HOST_CFG_MGR_H


#include "atptypes.h"

#define DHCP_APP_LEN   32
#define WAN_CONNECTION_LEN_MAX	128
#define ALOC_WAN_TYPE   16
#define MAC_LEN			6	// binary format like  001b 219e bfe7
#define HEX_MAC_LEN        17    // hex string format like 00:1B:21:9E:BF:7E
#define SRC_INTERFACE_LEN	1024
#define LAN_COMM_ID_LEN		32	
#define RESERVED_IP_NUM_MAX	8
#define RESERVED_IP_LEN_MAX	128
#define DNS_NUM_MAX		2
#define DNS_LEN_MAX               128
#define DOMAIN_NAME_LEN		64
#define IP_ROUTER_NUM_MAX	16
#define DHCP_DECLINE_TIME   60
#define LINE_MAX_LEN        80
#define OPTION_APP_LEN      32
#define FILTER_MODE_LEN     16
#define MANUAL_CFG_IP_NUM_MAX   32
#define WAN_SPOOF_NUM_MAX   8

// static address table
typedef struct tagSTATIC_ADDR_NODE {
    struct tagSTATIC_ADDR_NODE * next;
    VOS_BOOL    enable;
    VOS_UINT8   macAddr[HEX_MAC_LEN];
    VOS_UINT32  Yiaddr;
} STATIC_ADDR_NODE;

typedef struct {
    STATIC_ADDR_NODE	*   head;
} STATIC_ADDR_TBL;

// option list
typedef struct tagOPTION {
    struct tagOPTION        * next;
    VOS_BOOL                    enable;
    VOS_UINT8                    tag;
    VOS_UINT8                    len;
    VOS_UINT8                 * value;
} OPTION_NODE;

typedef struct {
    OPTION_NODE  *           head;
} OPTION_LIST_T;

typedef enum tagLEASE_TYPE_EN
{
    ATP_LEASE_IS_OPTION125  =       (1 << 0),
    ATP_LEASE_IS_SYNC       =       (1 << 1),   // For TR181 to mark if it has been synced with DB
    ATP_LEASE_IS_DECLINED   =       (1 << 2),
} LEASE_TYPE_EN;

typedef struct tagLEASE_ADDR_NODE {
    struct  tagLEASE_ADDR_NODE * next;
    VOS_UINT8   macAddr[HEX_MAC_LEN];
    VOS_UINT32  Yiaddr;
    VOS_UINT32  expires;
    VOS_UINT32  startTime;
    char    hostName[DOMAIN_NAME_LEN];
    char    vendorId[LAN_COMM_ID_LEN];
    OPTION_NODE * optionNode;
    VOS_UINT32  ulFlags;        // isOption125Client; isDeclined; isSync
} LEASE_ADDR_NODE;

typedef struct {
    LEASE_ADDR_NODE * head;
} LEASE_ADDR_TBL;

// user allocated wan data
typedef struct {
    char                  allocatedWANType[ALOC_WAN_TYPE];
    char                  assocWANConn[WAN_CONNECTION_LEN_MAX];
    VOS_UINT32      Yiaddr;
    VOS_UINT32      passthruLease;
    VOS_UINT8        passthruMACAddr[HEX_MAC_LEN];	
    OPTION_LIST_T  optionList;
} USER_ALOC_WAN;


// user manually configured static IP
typedef struct tagDhcpsIPcheckIpInfo
{
    struct tagDhcpsIPcheckIpInfo    *pstNext;
    VOS_UINT32      ulAddr;
    VOS_UINT8       ucMac[MAC_LEN];
}MANUAL_CFG_STATIC_HOST;

typedef struct tagSERVER_POOL_NODE {
    struct tagSERVER_POOL_NODE  * next;
    VOS_UINT32            poolOrder;			// pool precedence, 1 is the highest priority
    VOS_BOOL              enable;
    VOS_CHAR              srcInterface[SRC_INTERFACE_LEN];	
    VOS_CHAR              vendorId[LAN_COMM_ID_LEN];   	// mapping to option 60
    VOS_BOOL              vendorIdExclude;
    VOS_CHAR              vendorIdMode[FILTER_MODE_LEN];
    VOS_CHAR              clientId[LAN_COMM_ID_LEN];		// mapping to option 61
    // VOS_BOOL              clientIdExclude;
    VOS_CHAR              userId[LAN_COMM_ID_LEN];		// mapping to option 77
    // VOS_BOOL              userIdExclude;
    //VOS_CHAR              clientMacAddr[MAC_LEN];		// Physical address of DHCP client
    //VOS_CHAR              clientMacAddrMask[MAC_LEN]; 	// MAC address bitmask
    // VOS_BOOL              clientAddrExclude;
    //VOS_BOOL              locallyServed;		// Assign IP by local DHCPs or relay to next hop identified by DHCPServerIPAddress 
    VOS_UINT32           startAddr;
    VOS_UINT32           endAddr;
    VOS_UINT32           reservedIpAddr[RESERVED_IP_NUM_MAX];	// at most 8 reserved IP addresses
    //VOS_CHAR             domainName[DOMAIN_NAME_LEN];
    unsigned long          leaseTime;
    USER_ALOC_WAN   userAlocWAN;
    STATIC_ADDR_TBL staticAddrTbl;
    OPTION_LIST_T       optionList;

} SERVER_POOL_NODE;

typedef struct {
    SERVER_POOL_NODE * head;
    VOS_BOOL        configurable;
    VOS_BOOL        enable;
    VOS_UINT32      startAddr;
    VOS_UINT32      endAddr;
    unsigned long     leaseTime;
    USER_ALOC_WAN   userAlocWAN;
} SERVER_POOL_LIST;   

typedef struct tagGatewayInfo {
    VOS_UINT32 serverIP;                 // our internal gateway IP addr, like 192.168.1.1
    VOS_UINT8 hwAddr[MAC_LEN];  // our internal gateway MAC addr, like 00:2F:E8:C0:78:9A
    VOS_INT32 portIndex;              //  our internal gateway use which port interface to connect to WAN
} GATEWAY_INFO;

typedef struct {
    SERVER_POOL_LIST serverPoolList;
    STATIC_ADDR_TBL staticAddrTbl;
    OPTION_LIST_T optionList;
    LEASE_ADDR_TBL  leaseAddrTbl;
} IP_ADDR_MAP;

typedef struct tagATP_DHCP_SERVER_ST{
    struct tagATP_DHCP_SERVER_ST    * next;
    const VOS_CHAR      *brID;                // for multiple bridge interfaces feature, we need to support br0( 192.168.1.1), br1(192.168.2.1) etc
    VOS_INT32               dhcpFd;             //each bridge interface with a listening socket to send/receive DHCP packet
    IP_ADDR_MAP          *IPAddrMap;     //each bridge interface has its own IP Pool resource like br0 [192.168.1.1,192.168.1.254], br1[192.168.2.1,192.168.2.254]
    GATEWAY_INFO        gatewayInfo;   // get from ioctl, used to send ARP to specified br interface
} ATP_DHCP_SERVER;

VOS_INT32 ATP_DHCPS_OptionAdd(OPTION_LIST_T *pstOptionList, VOS_BOOL bEnable, VOS_UINT8 ulTag, const VOS_UINT8 *pucBuf, VOS_UINT8 lBufLen);
VOS_INT32 ATP_DHCPS_OptionDelByTag(OPTION_LIST_T *pstOptionList, VOS_UINT8 ulTag);

VOS_INT32 ATP_DHCPS_StaticAddrAdd(STATIC_ADDR_TBL *pstStaticAddTbl, VOS_BOOL bEnable, const VOS_UINT8 *pcMacAddr, VOS_UINT32 ulIpAddr);
VOS_INT32 ATP_DHCPS_StaticAddrDelByMac(STATIC_ADDR_TBL *pstStaticAddTbl, const VOS_UINT8 *pcMacAddr);

VOS_INT32 ATP_DHCPS_ServerPoolAdd(SERVER_POOL_LIST *pstServerPoolList, const SERVER_POOL_NODE * pstServerPoolNode); 
                              
VOS_INT32 ATP_DHCPS_ServerPoolDelbyVendorID(OPTION_LIST_T *pstServerPoolList, const VOS_UINT8 *pcVendorId);

VOS_UINT32 ATP_DHCPS_LoadCfgFromDB();

VOS_UINT32 ATP_DHCPS_ReLoadCfgFromDB(VOS_VOID);

VOS_BOOL  isIPInSubPool(VOS_UINT32 Yiaddr, const IP_ADDR_MAP * IPAddrMap, SERVER_POOL_NODE ** serverPoolNode);

#define ATP_DHCPS_ListDeleteAll(list)  ATP_UTIL_ListDeleteAll((ATP_UTIL_LIST_ST *)((list)), NULL); (list) = NULL;

extern ATP_DHCP_SERVER * g_pstDhcpServer;


#endif
