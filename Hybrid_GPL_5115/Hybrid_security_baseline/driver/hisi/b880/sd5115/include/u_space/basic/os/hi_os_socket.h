/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_socket.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_04
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_SOCKEET_H__
#define __HI_OS_SOCKEET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef hi_int32    HI_OS_SOCKET_T;
typedef hi_uint32   HI_OS_INADDR_T;       /* 网络地址类型 */
typedef hi_int32    HI_OS_SSIZE_T;        /* socket字符长度类型*/
typedef hi_uint32   HI_OS_SOCKLEN_T;      /* socket长度类型 */

#define  HI_FD_SETSIZE  (1024)

/* 通用的SOCKET地址结构 */
typedef struct {
  hi_ushort16 us_safamily;
  hi_char8    a_c_sadata[14];
}hi_os_socket_addr_s;

/* IP地址结构  */
typedef struct {
    hi_uint32 ui_addr;
}hi_os_in_addr_s;

/* 使用AF_INET domain的SOCKET地址 */
typedef struct {
        hi_ushort16 us_sinfamily;     // 2
        hi_ushort16 us_sinport;       // 2
        hi_os_in_addr_s st_sinaddr;    // 8
        hi_char8    a_c_sinzero[8];    // 16
} hi_os_socket_addr_in_s;

/* Description of data base entry for a single host.  */
typedef struct {
    hi_char8 *pc_hname;                 /* Official name of host.  */
    hi_char8 **ppc_haliases;            /* Alias list.  */
    hi_int32 h_addrtype;                /* Host address type.  */
    hi_int32 h_length;                  /* Length of address.  */
    hi_char8 **ppc_haddrlist;           /* List of addresses from name server.  */
}hi_os_hostent_s;

#define HI_OS_FD_SET_SIZE       (HI_FD_SETSIZE/8)
/* 文件描述词 */
typedef  struct
{
    hi_int32 fds_bits[HI_OS_FD_SET_SIZE];
}hi_os_fd_set_s;

/* Types of sockets.  */
typedef enum
{
    HI_SOCK_STREAM = (1),   /* Sequenced, reliable, connection-based   byte streams.  */
    HI_SOCK_DGRAM = (2),    /* Connectionless, unreliable datagrams  of fixed maximum length.  */
    HI_SOCK_RAW = (3),      /* Raw protocol interface.  */
    HI_SOCK_RDM = (4),      /* Reliably-delivered messages.  */
    /* Sequenced, reliable, connection-based,
       datagrams of fixed maximum length.  */
    HI_SOCK_SEQPACKET = (5),
    /* Linux specific way of getting packets
       at the dev level.  For writing rarp and
       other similar things on the user level. */
    HI_SOCK_PACKET = (10)
}hi_os_socket_type_e;


#define HI_AF_INET      (2)
#define HI_IPPROTO_TCP  (6)
#define HI_IPPROTO_UDP  (17)    /* User Datagram Protocol.  */
#define HI_AF_INET6     (10)    /* IP version 6.  */
#define HI_TCP_NODELAY  0x1

/* For setsockoptions(2) */
#define HI_SOL_SOCKET    (1)

#define HI_SO_DEBUG     (1)
#define HI_SO_REUSEADDR (2)
#define HI_SO_TYPE      (3)
#define HI_SO_ERROR     (4)
#define HI_SO_DONTROUTE (5)
#define HI_SO_BROADCAST (6)
#define HI_SO_SNDBUF    (7)
#define HI_SO_RCVBUF    (8)
#define HI_SO_KEEPALIVE (9)
#define HI_SO_OOBINLINE (10)
#define HI_SO_NO_CHECK  (11)
#define HI_SO_PRIORITY  (12)
#define HI_SO_LINGER    (13)
#define HI_SO_BSDCOMPAT (14)
/* To add :#define SO_REUSEPORT 15 */
#define HI_SO_PASSCRED  (16)
#define HI_SO_PEERCRED  (17)
#define HI_SO_RCVLOWAT  (18)
#define HI_SO_SNDLOWAT  (19)
#define HI_SO_RCVTIMEO  (20)
#define HI_SO_SNDTIMEO  (21)

/* Security levels - as per NRL IPv6 - don't actually do anything */
#define HI_SO_SECURITY_AUTHENTICATION       (22)
#define HI_SO_SECURITY_ENCRYPTION_TRANSPORT (23)
#define HI_SO_SECURITY_ENCRYPTION_NETWORK   (24)
#define HI_SO_BINDTODEVICE  (25)

/* Socket filtering */
#define HI_SO_ATTACH_FILTER (26)
#define HI_SO_DETACH_FILTER (27)

#define HI_SO_PEERNAME      (28)
#define HI_SO_TIMESTAMP     (29)
#define HI_SCM_TIMESTAMP    HI_SO_TIMESTAMP

#define HI_FIONBIO          (0x5421)
#define HI_FIONREAD         (0x541B)
#define HI_SIOCATMARK       (0x8905)
#define HI_SIOCGIFNAME    (0x8910)        /* get iface name        */
#define HI_SIOCSIFLINK    (0x8911)        /* set iface channel        */
#define HI_SIOCGIFCONF    (0x8912)        /* get iface list        */
#define HI_SIOCGIFFLAGS   (0x8913)        /* get flags            */
#define HI_SIOCSIFFLAGS   (0x8914)        /* set flags            */
#define HI_SIOCGIFADDR    (0x8915)        /* get PA address        */
#define HI_SIOCSIFADDR    (0x8916)        /* set PA address        */
#define HI_SIOCGIFDSTADDR (0x8917)        /* get remote PA address    */
#define HI_SIOCSIFDSTADDR (0x8918)        /* set remote PA address    */
#define HI_SIOCGIFBRDADDR (0x8919)        /* get broadcast PA address    */
#define HI_SIOCSIFBRDADDR (0x891a)        /* set broadcast PA address    */
#define HI_SIOCGIFNETMASK (0x891b)        /* get network PA mask        */
#define HI_SIOCSIFNETMASK (0x891c)        /* set network PA mask        */
#define HI_SIOCGIFMETRIC  (0x891d)        /* get metric            */
#define HI_SIOCSIFMETRIC  (0x891e)        /* set metric            */
#define HI_SIOCGIFMEM     (0x891f)        /* get memory address (BSD)    */
#define HI_SIOCSIFMEM     (0x8920)        /* set memory address (BSD)    */
#define HI_SIOCGIFMTU     (0x8921)        /* get MTU size            */
#define HI_SIOCSIFMTU     (0x8922)        /* set MTU size            */
#define HI_SIOCSIFNAME    (0x8923)        /* set interface name */
#define HI_SIOCSIFHIADDR  (0x8924)        /* set hardware address     */
#define HI_SIOCGIFENCAP   (0x8925)        /* get/set encapsulations       */
#define HI_SIOCSIFENCAP   (0x8926)
#define HI_SIOCGIFHIADDR  (0x8927)        /* Get hardware address        */
#define HI_SIOCGIFSLAVE   (0x8929)        /* Driver slaving support    */
#define HI_SIOCSIFSLAVE   (0x8930)
#define HI_SIOCADDMULTI   (0x8931)        /* Multicast address lists    */
#define HI_SIOCDELMULTI   (0x8932)
#define HI_SIOCGIFINDEX   (0x8933)        /* name -> if_index mapping    */
#define HI_SIOGIFINDEX    SIOCGIFINDEX    /* misprint compatibility :-)    */
#define HI_SIOCSIFPFLAGS  (0x8934)        /* set/get extended flags set    */
#define HI_SIOCGIFPFLAGS  (0x8935)
#define HI_SIOCDIFADDR    (0x8936)        /* delete PA address        */
#define HI_SIOCSIFHIBROADCAST   (0x8937)  /* set hardware broadcast addr    */
#define HI_SIOCGIFCOUNT   (0x8938)        /* get number of devices */
#define HI_IFHIADDRLEN   (6)
#define HI_IFNAMSIZ      (16)

typedef struct {
    hi_uint32   ui_memstart;
    hi_uint32   ui_memend;
    hi_ushort16 uc_baseaddr;
    hi_uchar8   uc_irq;
    hi_uchar8   uc_dma;
    hi_uchar8   uc_port;
    hi_uchar8   uc_resv[3];
}hi_os_ifmap_s;

typedef struct hi_ifreq
{
    union
    {
        char a_c_ifrnname[HI_IFNAMSIZ];        /* if name, e.g. "en0" */
    } ifr_ifrn;

    union
    {
        hi_os_socket_addr_s ifru_addr;
        hi_os_socket_addr_s ifru_dstaddr;
        hi_os_socket_addr_s ifru_broadaddr;
        hi_os_socket_addr_s ifru_netmask;
        hi_os_socket_addr_s ifru_hiaddr;
        hi_short16 ifru_flags;
        hi_int32 ifru_ivalue;
        hi_int32 ifru_mtu;
        hi_os_ifmap_s ifru_map;
        hi_char8 ifru_slave[HI_IFNAMSIZ];    /* Just fits the size */
        hi_char8 ifru_newname[HI_IFNAMSIZ];
        hi_char8 * ifru_data;
    } ifr_ifru;
}hi_os_ifreq_s;

#define ifr_name      ifr_ifrn.a_c_ifrnname  /* interface name     */
#define ifr_hiaddr    ifr_ifru.ifru_hiaddr   /* MAC address         */
#define ifr_addr      ifr_ifru.ifru_addr     /* address        */
#define ifr_dstaddr   ifr_ifru.ifru_dstaddr  /* other end of p-p lnk    */
#define ifr_broadaddr ifr_ifru.ifru_broadaddr/* broadcast address    */
#define ifr_netmask   ifr_ifru.ifru_netmask  /* interface net mask    */
#define ifr_flags     ifr_ifru.ifru_flags    /* flags        */
#define ifr_metric    ifr_ifru.ifru_ivalue   /* metric        */
#define ifr_mtu       ifr_ifru.ifru_mtu      /* mtu            */
#define ifr_map       ifr_ifru.ifru_map      /* device map        */
#define ifr_slave     ifr_ifru.ifru_slave    /* slave device        */
#define ifr_data      ifr_ifru.ifru_data     /* for use by interface    */
#define ifr_ifindex   ifr_ifru.ifru_ivalue   /* interface index    */
#define ifr_bandwidth ifr_ifru.ifru_ivalue   /* link bandwidth    */
#define ifr_qlen      ifr_ifru.ifru_ivalue   /* Queue length     */
#define ifr_newname   ifr_ifru.ifru_newname  /* New name        */
#define ifr_settings  ifr_ifru.ifru_settings /* Device/proto settings*/

#define HI_IP_TOS       1
#define HI_IP_TTL       2
#define HI_IP_HDRINCL   3
#define HI_IP_OPTIONS   4
#define HI_IP_ROUTER_ALERT  5
#define HI_IP_RECVOPTS  6
#define HI_IP_RETOPTS   7
#define HI_IP_PKTINFO   8
#define HI_IP_PKTOPTIONS    9
#define HI_IP_MTU_DISCOVER  10
#define HI_IP_RECVERR   11
#define HI_IP_RECVTTL   12
#define HI_IP_RECVTOS   13
#define HI_IP_MTU       14
#define HI_IP_FREEBIND  15
#define HI_IP_IPSEC_POLICY  16
#define HI_IP_XFRM_POLICY   17
#define HI_IP_PASSSEC   18

#define HI_OS_SYS_BASE_DEC                  10
#define HI_OS_SYS_IP_LENGTH                 16
#define HI_OS_SYS_IP_BYTE                   4
#define HI_OS_SYS_IP_MAX_NUMBER             255

extern hi_int32 hi_os_accept( HI_OS_SOCKET_T       ui_socket,
                                hi_os_socket_addr_s *pst_addr,
                                hi_uint32       *p_addrlen );
extern hi_int32 hi_os_bind( HI_OS_SOCKET_T       ui_socket,
                             const hi_os_socket_addr_s *pst_addr,
                             hi_int32        i_addrlen );
extern hi_uint32 hi_os_checkipaddr(hi_uchar8 *puc_ipaddr);
extern hi_int32 hi_os_connect(hi_uint32  ui_sockfd,  const hi_os_socket_addr_s *pst_servaddr, HI_OS_SOCKLEN_T ui_addrlen);
extern hi_int32 hi_os_fd_isset(hi_int32 i_fd, hi_os_fd_set_s *pst_fdset);
extern hi_void hi_os_fd_zero(hi_os_fd_set_s *pst_fdset);
extern hi_void hi_os_fd_clr(hi_int32 i_fd, hi_os_fd_set_s *pst_fdset);
extern hi_void hi_os_fd_set(hi_int32 i_fd, hi_os_fd_set_s *pst_fdset);
extern hi_os_hostent_s * hi_os_gethostbyname(const hi_char8 *pc_name);
extern hi_int32 hi_os_gethostname(hi_char8 *pc_name, hi_uint32 ui_len);
extern hi_int32 hi_os_getsockname(hi_int32 ui_socket, hi_os_socket_addr_s *pst_name, HI_OS_SOCKLEN_T *pst_namelen);
extern hi_int32  hi_os_getsockopt(HI_OS_SOCKET_T ui_socket, hi_int32 i_level, hi_int32 i_optname, hi_void *p_optval, HI_OS_SOCKLEN_T   *pst_optlen);
extern hi_int32 hi_os_ioctl( HI_OS_SOCKET_T ui_socket,hi_int32  i_cmd,hi_void *pv_arg );
extern hi_int32 hi_os_listen( HI_OS_SOCKET_T ui_socket,hi_int32 i_backlog );
extern HI_OS_SSIZE_T hi_os_recv( HI_OS_SOCKET_T ui_socket, hi_uchar8 *puc_buf, hi_int32 i_len, hi_int32 i_flags );
extern HI_OS_SSIZE_T hi_os_recvfrom( HI_OS_SOCKET_T ui_socket,
                                 hi_uchar8 *puc_buf,
                                 hi_int32        i_len,
                                 hi_int32        i_flags,
                                 hi_os_socket_addr_s *pst_form,
                                 hi_uint32       *pui_formlen );
extern hi_int32 hi_os_select( hi_int32       i_width, hi_os_fd_set_s *pst_readfds,
                              hi_os_fd_set_s *pst_writefds,hi_os_fd_set_s *pst_exceptfds,
                              const hi_os_timeval_s *pst_timeout );
extern HI_OS_SSIZE_T hi_os_send( HI_OS_SOCKET_T ui_socket,const hi_uchar8 *puc_buf,hi_int32 i_len,hi_int32 i_flags );
extern HI_OS_SSIZE_T hi_os_sendto( HI_OS_SOCKET_T        ui_socket,
                                const hi_uchar8        *puc_buf,
                                hi_int32        i_len,
                                hi_int32        i_flags,
                                const hi_os_socket_addr_s *pst_to,
                                hi_int32        i_tolen );
extern hi_int32  hi_os_setsockopt(HI_OS_SOCKET_T ui_socket,  hi_int32  i_level,  hi_int32  i_optname, 
       				const hi_void  *p_optval,   HI_OS_SOCKLEN_T ui_optlen);
extern HI_OS_SOCKET_T hi_os_socket( hi_int32 i_af, hi_int32 i_type,hi_int32 i_protocol );

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_SOCKEET_H__ */



