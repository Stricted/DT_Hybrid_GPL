
/**\file ipcheckapi.h
  *
  *  Data structure declarations for ipcheck
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/


#ifndef __IPCHECK_API_H__
#define __IPCHECK_API_H__

#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "msgapi.h"
#include "httpapi.h"

#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>
#include "atpconfig.h"


#ifdef SUPPORT_ATP_IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#ifndef IPV6_RECVNFMARK
#define IPV6_RECVNFMARK        (98)
#define IPV6_NFMARK             (99)
#endif

#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum tagATP_IPCHECK_RET_CODE_EN
{
    // These errors are reserved for dns only
    ATP_IPCHECK_RET_PARA_ERR    = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_IPCHECK),
    ATP_IPCHECK_RET_MEM_ERR,
    ATP_IPCHECK_RET_NOT_INITED,
    ATP_IPCHECK_RET_SOCK_ERR,
    ATP_IPCHECK_RET_NOT_SPPORTED,
    ATP_IPCHECK_RET_RECV_ERR,
    ATP_IPCHECK_RET_INTERNEL_ERR,
    ATP_IPCHECK_RET_EXPIRED,
    ATP_IPCHECK_RET_RESOURCE_EXCEED,
    ATP_IPCHECK_RET_FAKE_PACKET,
    ATP_IPCHECK_RET_NO_RELAY_NEEDED,
} ATP_IPCHECK_RET_CODE_EN;

/*  Standard Length Definition */
#define IPCHECK_MAC_LEN 6
#define IPCHECK_IP_LEN 4

#define IPCHECK_INDEX_MAX_NUM 253
#define IPCHECK_MAX_IP_PER_MAC_NUM  32

/*
 *  ARP related definition
 */
#define IPCHECK_PAD_LEN 18
#define IPCHECK_MAC_SETOFFE 32
#define IPCHECK_HADDR_SETOFFE 38
#define IPCHECK_PACKET_BIG 60

typedef struct _TagArpMsg {
	struct ethhdr ethhdr;	 		/* Ethernet header */
	u_short usHtype;				/* hardware type (must be ARPHRD_ETHER) */
	u_short usPtype;				/* protocol type (must be ETH_P_IP) */
	u_char  cHlen;				/* hardware address length (must be 6) */
	u_char  clen;				/* protocol address length (must be 4) */
	u_short usOperation;			/* ARP opcode */
	u_char  ucSHaddr[IPCHECK_MAC_LEN];			/* sender's hardware address */
	u_char  ucSInaddr[IPCHECK_IP_LEN];			/* sender's IP address */
	u_char  ucTHaddr[IPCHECK_MAC_LEN];			/* target's hardware address */
	u_char  ucTInaddr[IPCHECK_IP_LEN];			/* target's IP address */
	u_char  ucPad[IPCHECK_PAD_LEN];			/* pad for min. Ethernet payload (60 bytes) */
}IPCHECK_ARP_PACKET_ST;


#define  ATP_IPV6_NEIGHBORS_FILE_PATH  "/var/dhcp/dhcps/neighbors"
#define  ATP_IPV6_NEIGHBORS_FILE_PATH_NEW  "/var/dhcp/dhcps/neighbors_new"

/**     Bridge definition */

//!\brief Bridge information structure
/*!
 *  pcBrName:           Bridge name, like br0, br1
 *  aucMac:               Bridge MAC address
 *  ulIPAddress:        Bridge IPv4 address
 *  pstSockBaseV4    Bridge IPv4 socket listening ARP packets
 *  pstSockBaseV6    Bridge IPv6 socket listening ICMPv6 packets
 *  lSendSock6         Bridge IPv6 socket for sending ICMPv6 packets
 */
typedef struct tagATP_IPCHECK_BR_INFO_ST
{
    struct tagATP_IPCHECK_BR_INFO_ST        *pstNext;

    VOS_CHAR                                *pcBrName;
    VOS_UINT32                              ulLanInstId;
    VOS_UINT8                               aucMac[IPCHECK_MAC_LEN];
    VOS_UINT32                              ulIPAddress;
    ATP_SOCK_BASE_ST                        *pstSockBaseV4;
#ifdef SUPPORT_ATP_IPV6
    ATP_SOCK_BASE_ST                        *pstSockBaseV6;
    VOS_INT32                               lSendSock6;
#endif
} ATP_IPCHECK_BR_INFO_ST;

ATP_IPCHECK_BR_INFO_ST *ATP_IPCHECK_BridgeFindByName(const VOS_CHAR *pcBrName);

VOS_UINT32 ATP_IPCHECK_BridgeInit();

ATP_IPCHECK_BR_INFO_ST *ATP_IPCHECK_BridgeGetFirst(VOS_VOID);

/**     Interface port definition */

typedef struct tagATP_IPCHECK_PORT_INFO_ST
{
    struct tagATP_IPCHECK_PORT_INFO_ST      *pstNext;

    VOS_CHAR                                *pcPortName;
    const VOS_CHAR                          *pcDomainName;
    const ATP_IPCHECK_BR_INFO_ST            *pstBridge;
} ATP_IPCHECK_PORT_INFO_ST;

ATP_IPCHECK_PORT_INFO_ST *ATP_IPCHECK_PortGetFirst(VOS_VOID);

ATP_IPCHECK_PORT_INFO_ST *ATP_IPCHECK_PortFindByName(const VOS_CHAR *pcPortName);

ATP_IPCHECK_BR_INFO_ST *ATP_IPCHECK_BridgeAdd(const VOS_CHAR *pcBrName);

VOS_VOID ATP_IPCHECK_BridgeDelete(ATP_IPCHECK_BR_INFO_ST *pstBridge);

VOS_VOID ATP_IPCHECK_BridgeReloadAddress(VOS_VOID);

ATP_IPCHECK_PORT_INFO_ST *ATP_IPCHECK_PortFindAndAddIfNotExist(const ATP_IPCHECK_BR_INFO_ST *pstBridge, const VOS_CHAR *pcPortName);

/** MAC address Item definition */
/*
 *  Bridge -> Port1 -> MACAddress -> IPAddress
 */
typedef union tagIPCHECK_IPADDRESS_UN
{
    struct  in_addr     ipv4;
    struct  in6_addr    ipv6;
} IPCHECK_IPADDRESS_UN;

#define ATP_IPCHECK_DETECT_PERIOD_D     (5 * 60)       // 5minutes
#define ATP_IPCHECK_DETECT_COUNT_D      (1)             // Only try once
#define ATP_IPCHECK_DETECT_TIMEOUT_D    (2)             // 2 seconds

typedef struct _NEIGHBOR_IP_ST
{
    struct _NEIGHBOR_IP_ST *pstNext;

    VOS_UINT32          ulLastActiveTime;

    uint8_t  ip_type;  /*IP 地址类型, AF_INET 或 AF_INET6 */
    uint8_t  send_solicit; /*是否需要发邻居请求探测其是否在线,网线拔了开始探测*/
    uint8_t  solicit_count; /*在未超时前，发送探测包的计数*/
    IPCHECK_IPADDRESS_UN ip_addr;                   /*IP 地址，兼容IPv4 和IPv6*/
}NEIGHBOR_IP_INFO;

typedef struct _TagDiagipInfo
{
    struct _TagDiagipInfo   *pstNext;

    u_char    ucMac[IPCHECK_MAC_LEN];   /* 下挂PC的MAC地址  */
    int       lActive;                      /*  使用状态        */
    int       lSndArpFlag;                  /*  启用发送ARP的功能,使能后，lSndArpNum，ulExpires才会生效*/
    int       lSndArpNum;                   /*  发送ARP的次数   */
    u_int32_t ulActExpires;                 /*  网线拔掉以后，存在时间*/
    int       lDhcpsRestart;                /*  DHCPS 重启标志,0:没有重启,1:重启*/
    uint8_t   ucType;
    uint8_t   ucSynced;

    NEIGHBOR_IP_INFO                *pstIpAddrs;
    const ATP_IPCHECK_PORT_INFO_ST  *pstPort;
}IPCHECK_MAC_ST;

IPCHECK_MAC_ST *ATP_IPCHECK_MACAddressGetFirst(VOS_VOID);

IPCHECK_MAC_ST *ATP_IPCHECK_MACAddressFindByMac(const VOS_UINT8 aucMac[IPCHECK_MAC_LEN]);

IPCHECK_MAC_ST *ATP_IPCHECK_MACAddressFindByMacStr(const VOS_CHAR *pcMacAddr);

IPCHECK_MAC_ST *ATP_IPCHECK_MACAddressNew(const ATP_IPCHECK_PORT_INFO_ST *pstPortInfo, const u_char *pucMac);

VOS_VOID ATP_IPCHECK_MACAddressDeleteItem(IPCHECK_MAC_ST *pstDelItem);

VOS_VOID ATP_IPCHECK_MACAddressDeleteByPort(ATP_IPCHECK_PORT_INFO_ST *pstPort);

VOS_UINT32 ATP_IPCHECK_MACAddressGetIPAccount(const IPCHECK_MAC_ST *pstMacItem);

NEIGHBOR_IP_INFO *ATP_IPCHECK_MACAddressIsIPExist(const IPCHECK_MAC_ST *pstMacItem, VOS_INT32 lAddrType, const IPCHECK_IPADDRESS_UN *pstAddr);

NEIGHBOR_IP_INFO *ATP_IPCHECK_MACAddressAddIP(IPCHECK_MAC_ST *pstMacItem, VOS_INT32 lAddrType, const IPCHECK_IPADDRESS_UN *pstAddr);

VOS_VOID ATP_IPCHECK_MACAddressDeleteIP(IPCHECK_MAC_ST *pstMacItem, VOS_INT32 lAddrType, const IPCHECK_IPADDRESS_UN *pstAddr);

VOS_VOID ATP_IPCHECK_MACAddressDeleteByAddrType(IPCHECK_MAC_ST *pstMacItem, VOS_INT32 lAddrType);

VOS_UINT32 ATP_IPCHECK_MACAddressDectectIP(const IPCHECK_MAC_ST *pstMacItem, NEIGHBOR_IP_INFO *pstIPAddr);

VOS_VOID ATP_IPCHECK_MACAddressUpdateIPTimeout(NEIGHBOR_IP_INFO *pstIPItem);

VOS_VOID ATP_IPCHECK_MACAddressUpdateTimer();

VOS_UINT32 ATP_IPCHECK_MACAddressDectect(const IPCHECK_MAC_ST *pstMacItem);

VOS_UINT32 ATP_IPCHECK_MACAddressGetIPV4Addr(const IPCHECK_MAC_ST *pstMacItem);

#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 ATP_IPCHECK_MACAddressGetIPV6Addr(const IPCHECK_MAC_ST *pstMacItem, struct in6_addr *pstAddr);
#endif

VOS_VOID ATP_IPCHECK_MACAddressMove(const ATP_IPCHECK_PORT_INFO_ST *pstSrcPort, const ATP_IPCHECK_PORT_INFO_ST *pstDstPort);

typedef enum
{
    ATP_ACC_DEVTYPE_STB = 0, 
    ATP_ACC_DEVTYPE_COMPUTER,     
    ATP_ACC_DEVTYPE_PHONE,
    ATP_ACC_DEVTYPE_CAMERA,
    ATP_ACC_DEVTYPE_OTHER,
    ATP_ACC_DEVTYPE_BUTT
}ACC_EN;

#define DHCPS_OPTION60_STB "STB"
#define DHCPS_OPTION60_PHONE "Phone"
#define DHCPS_OPTION60_CAMERA "Camera"
#define DHCPS_OPTION60_COMPUTER "Computer"
#define DHCPS_OPTION60_SIP_PHONE "SIP-Phone"

typedef struct tag_DeviceType_ST
{
    VOS_UINT8  lType;
    const char * czDeviceName;
}DeviceType_ST;

VOS_UINT32 ATP_IPCHECK_GetInterfaceType(const VOS_CHAR *psIfc, VOS_CHAR *psIfcType, VOS_UINT32 ullen);

VOS_UINT32 ATP_IPCHECK_HostInfoGetByMac(const IPCHECK_MAC_ST *pucMac, ATP_HOST_INFO *pstHostInfo);

const VOS_CHAR *ATP_IPCHECK_GetVersion(VOS_VOID);

/** Adapt function */
extern VOS_UINT32 ATP_IPCHECK_AdaptSyncDB(VOS_BOOL);

extern VOS_UINT32 ATP_IPCHECK_RmInactiveHostFromDB(VOS_VOID);

extern VOS_UINT32 ATP_IPCHECK_GetInactiveHostTimer(VOS_VOID);

void ATP_IPCHECK_AdaptSendArpPacket(const ATP_IPCHECK_BR_INFO_ST *pstBridge, u_int32_t ulYiaddr);

#ifdef SUPPORT_ATP_IPV6
int ATP_IPCHECK_AdaptSendSolicitPacket(const ATP_IPCHECK_BR_INFO_ST *pstBridge, const struct in6_addr *dst_ip6);
#endif

#ifdef ATP_DEBUG
#define ATP_IPCHECK_DEBUG(format, args...)\
{\
    printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);\
    printf(format, ##args);\
}
#else
#define ATP_IPCHECK_DEBUG(x...)
#endif

#if defined(__cplusplus)
}
#endif


#endif
