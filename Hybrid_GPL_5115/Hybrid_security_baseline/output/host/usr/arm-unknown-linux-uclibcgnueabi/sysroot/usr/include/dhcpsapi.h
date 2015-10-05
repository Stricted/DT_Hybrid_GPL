
/**\file dhcpsapi.h
  *
  * Declarations for dhcps process
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/


#ifndef _DHCPSAPI_H
#define _DHCPSAPI_H

#include <netinet/ip.h>
#include <netinet/udp.h>

#include "options.h"
#include "stack.h"
#include "LanHostCfgMgr.h"

#include "msgapi.h"

#define DHCPS_MAC_BCAST_ADDR            "\xff\xff\xff\xff\xff\xff"

#define IS_NULL_ADDR(ea) ((((u_int8_t *)(ea))[0] | ((u_int8_t*)(ea))[1] |((u_int8_t *)(ea))[2] | \
                                            ((u_int8_t *)(ea))[3] |((u_int8_t *)(ea))[4] | ((u_int8_t *)(ea))[5]) == 0)

#define DHCPS_FILE_CWMP_PATH "/var/cwmp.conf"

#ifdef ATP_DEBUG
#define ATP_DHCP_DEBUG(format, args...)\
{\
    printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);\
    printf(format, ##args);\
}
#else
#define ATP_DHCP_DEBUG(x...)
#endif

#define DHCPS_RANDOM_XID_MAX 256

#define DHCPS_OPTION60_LEN 32

#define DHCPS_CMD_LEN_MAX 128

// for option 125
#define DHCPS_OPTION125_SN_LEN_MAX 64
#define DHCPS_OPTION125_OUI_LEN 16

#define DHCPS_OPTION125_CLIENT_OUI 1
#define DHCPS_OPITON125_CLIENT_SN 2
#define DHCPS_OPTION125_CLIENT_PROCL 3
#define DHCPS_OPTION125_SERVER_OUI 4
#define DHCPS_OPTION125_SERVER_SN 5
#define DHCPS_OPTION125_SERVER_PROCL 6


// DHCPS return code
enum ATP_DHCPS_RET_CODE_EN
{    
    DHCPS_RET_PARA_ERR  = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_CMS, ATP_MODULE_DHCPS),   // input invalid parameter 
    DHCPS_RET_MEM_ERR,       // memory error
    DHCPS_RET_SOCKET_ERR,  // socket error
    DHCPS_RET_NOT_SUPPORTED,
};

#if defined(__cplusplus)
extern "C" {
#endif

extern OPTION_NODE * g_pstOption125;

extern MANUAL_CFG_STATIC_HOST g_stDhcpsIpCheckData[MANUAL_CFG_IP_NUM_MAX];

extern DHCPS_SPOOF m_stDhcpsSpoofInfo[WAN_SPOOF_NUM_MAX];

void DhcpServerResInit(void);

int DhcpDiagramHandler(ATP_DHCP_PACKET_ST * packet, const ATP_DHCP_SERVER * dhcpServerInfo);

void DumpDhcpData(const ATP_DHCP_PACKET_ST  *packet);

void DhcpServerDeleteLease(const ATP_MSG_HEADER_ST * pstMsg);

void DhcpsHandleRequest(ATP_DHCP_PACKET_ST * rqstPacket, const ATP_DHCP_SERVER * dhcpSrvInfo);

VOS_VOID DhcpsSpoofWANUp(const ATP_MSG_HEADER_ST * packet);

VOS_VOID DhcpsSpoofWanDown(const ATP_MSG_HEADER_ST * packet);

VOS_VOID  DhcpsRcvSubscriptionUpdate(const ATP_MSG_HEADER_ST * packet);

int DhcpsGetClientPacket(ATP_DHCP_PACKET_ST *pstPacket, int lSockFd);

VOS_UINT32 SysGetUpTime();

VOS_VOID buildDhcpPacket(const LEASE_ADDR_NODE * leaseNode, ATP_DHCP_PACKET_ST * respPacket);

int DhcpsSendPacket(ATP_DHCP_PACKET_ST *pstPayLoad, int lForceBroadBcast, const ATP_DHCP_SERVER * dhcpSrvInfo);

VOS_BOOL isOption125inRqstDiagram(ATP_DHCP_PACKET_ST * rqstPacket);

int DhcpsListenSocket(unsigned int ulIp, int lPort, char *pcInf);

VOS_UINT32 DhcpsGetSpoofCfg(const VOS_CHAR * filePath);

OPTION_NODE * DhcpsCreateOption125(VOS_VOID);

VOS_VOID DhcpsSendAllIpAddr(VOS_VOID);

VOS_UINT32 DhcpsWriteLANMac2DB();

VOS_VOID ATP_DHCPS_NeighborsReload();

int ATP_DHCPS_NeighborsIsExist(u_int32_t ulUserIp, const u_char *pucMac);

VOS_UINT32 ATP_DHCPS_LoadLeaseFile(const VOS_CHAR *filePath);

VOS_VOID ATP_DHCPS_ShowPools(VOS_VOID);

VOS_VOID ATP_DHCPS_UtilConverMacStrToBinary(const VOS_CHAR *pcStrMac, VOS_UINT8 *pucBinMac);

#if defined(__cplusplus)
}
#endif


#endif
