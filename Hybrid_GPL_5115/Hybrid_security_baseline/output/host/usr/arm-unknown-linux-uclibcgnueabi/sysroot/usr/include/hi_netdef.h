/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_netdef.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_07_29
  最近修改   :

******************************************************************************/
#ifndef __HI_NETDEF_H__
#define __HI_NETDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*网络序转换,系统只支持little-endian*/
#ifndef H_NTOLL
#define H_NTOLL(data)    ((hi_ulong64)( ((hi_ulong64)(data)  >> 56) \
                                        | ((hi_ulong64)(data) << 56)\
                                        | (((hi_ulong64)(data) >> 40)  & 0x000000000000ff00 ) \
                                        | (((hi_ulong64)(data) << 40 ) & 0x00ff000000000000 )\
                                        | (((hi_ulong64)(data) >> 24)  & 0x0000000000ff0000 )\
                                        | (((hi_ulong64)(data) << 24)  & 0x0000ff0000000000 )\
                                        | (((hi_ulong64)(data) >> 8  )  & 0x00000000ff000000 )\
                                        | (((hi_ulong64)(data) << 8  )  & 0x000000ff00000000 )))
#endif

#ifndef H_NTOHL
#define H_NTOHL(data)    ((hi_uint32)( ((hi_uint32)(data)  >> 24) \
                                        | ((hi_uint32)(data)  << 24)  \
                                        | (((hi_uint32)(data) >> 8) & 0x0000ff00 )   \
                                        | (((hi_uint32)(data) << 8) & 0x00ff0000 )))
#endif
#ifndef H_HTONL
#define H_HTONL(data)    ((hi_uint32)( ((hi_uint32)(data)  >> 24) \
                                        | ((hi_uint32)(data)  << 24)  \
                                        | (((hi_uint32)(data) >> 8) & 0x0000ff00 )   \
                                        | (((hi_uint32)(data) << 8) & 0x00ff0000 )))
#endif
#ifndef H_NTOHS
#define H_NTOHS(data)    ((hi_ushort16)(((hi_ushort16)(data) >> 8 ) \
                                        | ((hi_ushort16)(data) << 8 )))
#endif

#ifndef H_HTONS
#define H_HTONS(data)    ((hi_ushort16) (((hi_ushort16)(data) >> 8 ) \
                                        | ((hi_ushort16)(data) << 8 )))
#endif

#define H_USHORT(byte0, byte1)  ( ( HI_BITMASK_LOW08 & (hi_uint32)(byte0))    \
                                | ( (HI_BITMASK_LOW08 & (hi_uint32)(byte1)) << HI_NUM_8) )
#define H_UINT(byte0, byte1, byte2, byte3)  ( (HI_BITMASK_LOW08 & (hi_uint32)(byte0))    \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte1)) << HI_NUM_8) \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte2)) << (HI_NUM_8 * 2)) \
                                | ((HI_BITMASK_LOW08 & (hi_uint32)(byte3)) << (HI_NUM_8 * 3)) )


#define HI_INADDR_ANY       (0x00000000)
#define HI_INADDR_NONE      0xFFFFFFFFUL
#define HI_INVALID_SOCKET   0xFFFFFFFFUL

#define HI_MAC_LEN          6   /*MAC地址长度*/
#define HI_ETH_HLEN         14      /* Total octets in header.   */
#define HI_VLAN_HLEN        4
#define HI_VLAN_MASK        0xEFFF
#define HI_VLAN_DEF_TPID    0x8100
#define HI_VLAN_DEF_TPID1   0x88a8
#define HI_IPVERSION        4

#define HI_ETH_P_LOOP       0x0060      /* Ethernet Loopback packet */
#define HI_ETH_P_PUP        0x0200      /* Xerox PUP packet     */
#define HI_ETH_P_PUPAT      0x0201      /* Xerox PUP Addr Trans packet  */
#define HI_ETH_P_IP         0x0800      /* Internet Protocol packet */
#define HI_ETH_P_X25        0x0805      /* CCITT X.25           */
#define HI_ETH_P_ARP        0x0806      /* Address Resolution packet    */
#define HI_ETH_P_BPQ        0x08FF      /* G8BPQ AX.25 Ethernet Packet  [ NOT AN OFFICIALLY REGISTERED ID ] */
#define HI_ETH_P_IEEEPUP    0x0a00      /* Xerox IEEE802.3 PUP packet */
#define HI_ETH_P_IEEEPUPAT  0x0a01      /* Xerox IEEE802.3 PUP Addr Trans packet */
#define HI_ETH_P_DEC        0x6000      /* DEC Assigned proto           */
#define HI_ETH_P_DNA_DL     0x6001      /* DEC DNA Dump/Load            */
#define HI_ETH_P_DNA_RC     0x6002      /* DEC DNA Remote Console       */
#define HI_ETH_P_DNA_RT     0x6003      /* DEC DNA Routing              */
#define HI_ETH_P_LAT        0x6004      /* DEC LAT                      */
#define HI_ETH_P_DIAG       0x6005      /* DEC Diagnostics              */
#define HI_ETH_P_CUST       0x6006      /* DEC Customer use             */
#define HI_ETH_P_SCA        0x6007      /* DEC Systems Comms Arch       */
#define HI_ETH_P_RARP       0x8035      /* Reverse Addr Res packet  */
#define HI_ETH_P_ATALK      0x809B      /* Appletalk DDP        */
#define HI_ETH_P_AARP       0x80F3      /* Appletalk AARP       */
#define HI_ETH_P_8021Q      0x8100      /* 802.1Q VLAN Extended Header  */
#define HI_ETH_P_IPX        0x8137      /* IPX over DIX         */
#define HI_ETH_P_IPV6       0x86DD      /* IPv6 over bluebook       */
#define HI_ETH_P_SLOW       0x8809      /* Slow Protocol. See 802.3ad 43B */
#define HI_ETH_P_WCCP       0x883E      /* Web-cache coordination protocol */
#define HI_ETH_P_PPP_DISC   0x8863      /* PPPoE discovery messages     */
#define HI_ETH_P_PPP_SES    0x8864      /* PPPoE session messages   */
#define HI_ETH_P_MPLS_UC    0x8847      /* MPLS Unicast traffic     */
#define HI_ETH_P_MPLS_MC    0x8848      /* MPLS Multicast traffic   */
#define HI_ETH_P_ATMMPOA    0x884c      /* MultiProtocol Over ATM   */
#define HI_ETH_P_ATMFATE    0x8884      /* Frame-based ATM Transport*/
#define HI_ETH_P_PPP_PRO_IP 0x0021

typedef enum {
  HI_IPPROTO_IP_E = 0,      /* Dummy protocol for TCP       */
  HI_IPPROTO_ICMP_E = 1,        /* Internet Control Message Protocol    */
  HI_IPPROTO_IGMP_E = 2,        /* Internet Group Management Protocol   */
  HI_IPPROTO_IPIP_E = 4,        /* IPIP tunnels (older KA9Q tunnels use 94) */
  HI_IPPROTO_TCP_E = 6,     /* Transmission Control Protocol    */
  HI_IPPROTO_EGP_E = 8,     /* Exterior Gateway Protocol        */
  HI_IPPROTO_PUP_E = 12,        /* PUP protocol             */
  HI_IPPROTO_UDP_E = 17,        /* User Datagram Protocol       */
  HI_IPPROTO_IDP_E = 22,        /* XNS IDP protocol         */
  HI_IPPROTO_DCCP_E = 33,       /* Datagram Congestion Control Protocol */
  HI_IPPROTO_RSVP_E = 46,       /* RSVP protocol            */
  HI_IPPROTO_GRE_E = 47,        /* Cisco GRE tunnels (rfc 1701,1702)    */

  HI_IPPROTO_IPV6_E = 41,/* IPv6-in-IPv4 tunnelling     */

  HI_IPPROTO_ESP_E = 50,            /* Encapsulation Security Payload protocol */
  HI_IPPROTO_AH_E = 51,             /* Authentication Header protocol       */
  HI_IPPROTO_BEETPH_E = 94,        /* IP option pseudo header for BEET */
  HI_IPPROTO_PIM_E    = 103,        /* Protocol Independent Multicast   */

  HI_IPPROTO_COMP_E   = 108,                /* Compression Header protocol */
  HI_IPPROTO_SCTP_E   = 132,        /* Stream Control Transport Protocol    */
  HI_IPPROTO_UDPLITE_E = 136,   /* UDP-Lite (RFC 3828)          */

  HI_IPPROTO_RAW_E   = 255,     /* Raw IP packets           */
  HI_IPPROTO_MAX_E,
}hi_ipproto_e;


#define HI_NETWORK_IS_VALID_MAC_ADDR(addr)                                               \
        (( !( 0x01 & (addr)[0] ) ) &&                                                    \
         ( ( (addr)[0] | (addr)[1] | (addr)[2] | (addr)[3] | (addr)[4] | (addr)[5]) ) )



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_NETDEF_H__ */
