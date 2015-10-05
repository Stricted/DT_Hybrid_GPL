/*
    Copyright (C) 2002-2005  Thomas Ries <tries@gmx.net>

    This file is part of Siproxd.
    
    Siproxd is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    Siproxd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with Siproxd; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "config.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "log.h"
#include "atpconfig.h"

#ifdef SUPPORT_ATP_QOS
#include "atputil.h"
#endif
static char const ident[]="$Id: sock.c,v 1.3 2009/04/30 01:26:39 y42304 Exp $";


/* configuration storage */
extern struct siproxd_config configuration;

/* URL mapping table     */
extern struct urlmap_s urlmap[];

/* socket used for sending SIP datagrams */
int sip_udp_lan_socket=0;
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
int sip_udp_wan_socket = 0;
int g_lsockFlg = 0;

int wanport = 56005;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

#ifdef SUPPORT_ATP_BRIDGE_BIND
extern char g_acPPPoEIpAddr[SIP_IPADDR];
extern char g_acPPPoEMask[SIP_IPADDR];
extern char g_acBrIpAddr[SIP_IPADDR];
extern char g_acBrMask[SIP_IPADDR];
#endif


/*
 * binds to SIP UDP socket for listening to incoming packets
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 */
int sipsock_listen (void)
{
    struct in_addr lanipaddr;
   
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   struct in_addr wanipaddr;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

    int i = 0;
   
    memset(&lanipaddr, 0, sizeof(lanipaddr));
    /*z00164626_20120707_BEGIN: LAN侧ip须绑定br0，而不能绑定全0地址*/
    if (STS_FAILURE == get_ip_by_ifname(configuration.inbound_if, &lanipaddr))
    {
        ERROR("%s ip: %s", configuration.inbound_if, utils_inet_ntoa(lanipaddr));
        ERROR("can not get the lan ip , please check it.......");
    }
    /*z00164626_20120707_END: LAN侧ip须绑定br0，而不能绑定全0地址*/
    /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
    memset(&wanipaddr, 0, sizeof(wanipaddr));
    if (STS_FAILURE == get_ip_by_ifname(configuration.outbound_if, &wanipaddr))
    {
        ERROR("%s ip: %s", configuration.inbound_if, utils_inet_ntoa(wanipaddr));
        ERROR("can not get the wan ip , please check it.......");
    }
    else
    {
        g_lsockFlg = 1;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

   /* DT c40163 add for STUN, disable the BRIDGE BIND */
   g_lsockFlg = 0;
   /* DT c40163 end */

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
    if (1 == g_lsockFlg) //DT,这个if的代码都走不到
    {

   #ifdef SUPPORT_ATP_QOS
       
       sip_udp_lan_socket = sockbind(lanipaddr, configuration.sip_listen_port, 1, SOCK_CREATE_BY_SIP);   
       sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1, SOCK_CREATE_BY_SIP);
       if (0 == sip_udp_wan_socket)
       {
            for (i = 0; i < 5; i++)
            {
                wanport += (i * 10 + i);
                sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1, SOCK_CREATE_BY_SIP);
                if (0 == sip_udp_wan_socket)
                {
                    ERROR("bind in port: %d error", wanport);
                    continue;
                }
                else
                {
                    break;
                }
            }
       }
   #else
   sip_udp_lan_socket = sockbind(lanipaddr, configuration.sip_listen_port, 1);
   sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1);
       if (0 == sip_udp_wan_socket)
       {
            for (i = 0; i < 5; i++)
            {
                wanport += (i * 10 + i);
                sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1);
                if (0 == sip_udp_wan_socket)
                {
                    ERROR("bind in port: %d error", wanport);
                    continue;
                }
                else
                {
                    break;
                }
            }
       }
   #endif
       if ((0 == sip_udp_lan_socket) || (0 == sip_udp_wan_socket)) 
       {
            ERROR("socket bind error");
            return STS_FAILURE; /* failure */
       }
    }
    else
    {
#ifdef SUPPORT_ATP_QOS
        sip_udp_lan_socket = sockbind(lanipaddr, configuration.sip_listen_port, 1, SOCK_CREATE_BY_SIP);   
#else
        sip_udp_lan_socket = sockbind(lanipaddr, configuration.sip_listen_port, 1);   
#endif
        if (sip_udp_lan_socket == 0) return STS_FAILURE; /* failure*/
    }

   INFO("bound to port %i", configuration.sip_listen_port);
   DEBUGC(DBCLASS_NET,"bound lan socket %i",sip_udp_lan_socket);
   DEBUGC(DBCLASS_NET,"bound wan socket %i",sip_udp_wan_socket);
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   return STS_SUCCESS;
}

/*从wan socket列表中选第一个已经有数据的fd，供读取数据, -1表示没socket可读*/
static int get_ready_wan_socket(fd_set *pst_fdset)
{
    int i;
    int iRet = -1;
    
    for (i = 0; i < URLMAP_SIZE; i++)
    {
        if ((1 == urlmap[i].active) && (urlmap[i].wan_socket_fd > 0) && (FD_ISSET(urlmap[i].wan_socket_fd, pst_fdset)))
        {
            iRet = urlmap[i].wan_socket_fd;
            break;
        }
    }

    return iRet;
}


/*
 * Wait for incoming SIP message. After a 2 sec timeout
 * this function returns with sts=0
 *
 * RETURNS >0 if data received, =0 if nothing received /T/O), -1 on error
 */
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
int sipsock_wait(int *fd) {
   int sts;
   fd_set fdset;
   struct timeval timeout;
   int i;
   int iWanFd = -1;
   
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   int max_fd = 0;
   static int counter = 0;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

   timeout.tv_sec=2;
   timeout.tv_usec=0;

   FD_ZERO(&fdset);

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   if (1 == g_lsockFlg)
   {
       FD_SET (sip_udp_lan_socket, &fdset);
       FD_SET (sip_udp_wan_socket, &fdset);
       max_fd = (sip_udp_wan_socket > sip_udp_lan_socket? sip_udp_wan_socket : sip_udp_lan_socket);
   }
   else
   {
       FD_SET (sip_udp_lan_socket, &fdset);
       max_fd = sip_udp_lan_socket;
       //加入由register动态创建的WAN socket
       for (i = 0; i < URLMAP_SIZE; i++)
       {
           if((1 == urlmap[i].active) && (urlmap[i].wan_socket_fd > 0))
           {
               FD_SET (urlmap[i].wan_socket_fd, &fdset);
               max_fd = urlmap[i].wan_socket_fd > max_fd? urlmap[i].wan_socket_fd : max_fd;
           }
       }
   }

   #if 0
   sts=select (sip_udp_socket+1, &fdset, NULL, NULL, &timeout);
   #else
   sts=select (max_fd + 1, &fdset, NULL, NULL, &timeout);
   #endif
   /* WARN on failures */
   if (sts<0) {
      /* WARN on failure, except if it is an "interrupted system call"
         as it will result by SIGINT, SIGTERM */
      if (errno != 4) {
         WARN("select() returned error [%i:%s]",errno, strerror(errno));
      } else {
         DEBUGC(DBCLASS_NET,"select() returned error [%i:%s]",
                errno, strerror(errno));
      }
   }

   /* 考虑到目前siproxd的处理流程, 因为目前select之后siproxd只能read一个报文，因此如果在
    * select 中有两个数据可读，那么我们只能选出一个可读的。这里为了保证公平性因此需要在这里进行
    * 轮询
    */
    if (0 == counter)
    {
        /*先处理LAN socket里面的数据*/
        if (FD_ISSET(sip_udp_lan_socket, &fdset))
        {
            *fd = sip_udp_lan_socket;
        }
        /*
        else if (FD_ISSET(sip_udp_wan_socket, &fdset))
        {
            *fd = sip_udp_wan_socket;
        }*/
        else
        {
           iWanFd = get_ready_wan_socket(&fdset);
           if (iWanFd > 0)
           {
               *fd = iWanFd;
           }
        }
        counter = 1;
    }
    else 
    {
        /*这把先处理WAN sockets里面的数据*/
        iWanFd = get_ready_wan_socket(&fdset);
        if (iWanFd > 0)
        {
            *fd = iWanFd;
        }
        else if (FD_ISSET(sip_udp_lan_socket, &fdset))
        {
            *fd = sip_udp_lan_socket;
        }
        counter = 0;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   return sts;
}

/*
 * read a message from SIP listen socket (UDP datagram)
 *
 * RETURNS number of bytes read
 *         from is modified to return the sockaddr_in of the sender
 */
/* DT c40163 modify for QOS
int sipsock_read(int fd, void *buf, size_t bufsize,
                 struct sockaddr_in *from, int *protocol) {
*/
int sipsock_read(int fd, void *buf, size_t bufsize,
       struct sockaddr_in *from, int *protocol, int *tos, unsigned int *ulMark) {
/* DT c40163 end */

   int count;
   socklen_t fromlen;

/* DT c40163 add for QOS */
#ifdef TOS_RCV
   char tos_temp = 0;
   char ctl_buf[64];
   struct msghdr msg;
   struct iovec iov;
   struct cmsghdr *cmsg;
   memset(&msg, 0, sizeof(msg));
   msg.msg_name = from;
   msg.msg_namelen = sizeof(*from);
   msg.msg_controllen = sizeof(ctl_buf);
   msg.msg_control = ctl_buf;
   msg.msg_iovlen = 1;
   msg.msg_iov = &iov;
   iov.iov_base = buf;
   iov.iov_len = bufsize;
#endif
/* DT c40163 end */
   unsigned int Mark_temp = 0;

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
    if (fd < 0)
    {
        count = 0;
        return count;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   fromlen=sizeof(struct sockaddr_in);
   *protocol = PROTO_UDP; /* up to now, unly UDP */
   #if 0
   count=recvfrom(sip_udp_socket, buf, bufsize, 0,
                  (struct sockaddr *)from, &fromlen);
   #else
   /* DT c40163 modify for QOS
   count=recvfrom(fd, buf, bufsize, 0,
                  (struct sockaddr *)from, &fromlen);
   */
#ifndef TOS_RCV        
   count=recvfrom(fd, buf, bufsize, 0,
                  (struct sockaddr *)from, &fromlen);
#else            
   count = recvmsg(fd, &msg, MSG_WAITALL);
   if (count > 0) {
      for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
   	     if (cmsg->cmsg_type == IP_TOS) {
		    tos_temp = *(char *)CMSG_DATA(cmsg);
	     }
#ifdef SUPPORT_ATP_HYBRID
         else if (cmsg->cmsg_type == IP_MARK)
         {
            Mark_temp = *(unsigned int *)CMSG_DATA(cmsg);
            *ulMark = Mark_temp;			
         }
#endif
	     else  {
   	        printf("sipsock_read: BAD CMSG_HDR\n");
	     }
      }
   }
   *tos = tos_temp;
#endif
   /* DT c40163 end */
   #endif

   if (count<0) {
      WARN("recvfrom() returned error [%s]",strerror(errno));
      *protocol = PROTO_UNKN;
   }

   DEBUGC(DBCLASS_NET,"received UDP packet from %s, count=%i",
          utils_inet_ntoa(from->sin_addr), count);
//   DUMP_BUFFER(DBCLASS_NETTRAF, buf, count);

   return count;
}

/* DT c40163 add for QOS */
static void sipsock_setopt(int sock, int t_online, int direct, int tos)
{
   if (t_online) {
   	  if (direct == REQTYP_OUTGOING || direct == RESTYP_OUTGOING) {
         if (0 != ATP_SIP_ALG_QoS_SetMark(sock, NFMARK_SIPALG)) {
            fprintf(stderr, "can not set the nfmakr %d for SIP alg", NFMARK_SIPALG);
         }
   	  }
	  else {
	  	 int sts = 0;
         int nfmark = 0;
		 unsigned char tos_temp = tos;
         sts = setsockopt(sock, SOL_SOCKET, SO_NFMARK, (void*)&nfmark, sizeof(nfmark));
         if (sts != 0) {
            printf("sipsock_setopt: SO_NFMARK fail, sts %d %s\n", sts, strerror(errno));
         }
         sts = setsockopt(sock, IPPROTO_IP, IP_TOS, (void*)&tos_temp, sizeof(tos_temp));
         if (sts != 0) {
            printf("sipsock_setopt: IP_TOS fail, sts %d %s\n", sts, strerror(errno));
         }
	  }
   }
   else {
      if (0 != ATP_SIP_ALG_QoS_SetMark(sock, 0)) {
         fprintf(stderr, "sipsock_setopt: can not set the nfmakr %d for SIP alg", 0);
      }
   }
}
/* DT c40163 end */

/* DT c40163 modify for STUN
#ifdef SUPPORT_ATP_BRIDGE_BIND
*/
#if 0
/* DT c40163 end */

/*
 * sends an UDP datagram to the specified destination
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 *  函数修改说明:
 *      修改人:    l67187
 *      修改目的:  解决代理和绑定时没有默认路由，sip alg无法工作的问题
 *      修改方案:  在报文发送时，bind上网关发送侧的ip地址，使用网关源地址路由
 *      具体修改:  原alg  使用  初始化时创建的 sip_udp_socket，绑定了空地址
 *                 无法再次绑定，修改后，在该函数中不在使用  sip_udp_socket发送报文
 *                 而是重新创建一个socket发送，之后就关闭释放资源
 */
/* DT c40163 modify for QOS
int sipsock_send(struct in_addr addr, int port, int protocol,
                 char *buffer, int size) {
*/
int sipsock_send(struct in_addr addr, int port, int protocol,
        char *buffer, int size, int t_online, int direct, int tos) {
/* DT c40163 end */

   struct sockaddr_in dst_addr;
   struct sockaddr_in stLocalAddr;
   int sock;
   int sts = -1;
   
   /* first time: allocate a socket for sending 
   if (sip_udp_socket == 0) {
      ERROR("SIP socket not allocated");
      return STS_FAILURE;
   }
*/
   if (buffer == NULL) {
      ERROR("sipsock_send got NULL buffer");
      return STS_FAILURE;
   }

   if (protocol != PROTO_UDP) {
      ERROR("sipsock_send: only UDP supported by now");
      return STS_FAILURE;
   }

   printf("sipsock_send: t_online=%u, outgoing=%u\n", t_online, direct); 
   
    if (1 == g_lsockFlg)
    {
        dst_addr.sin_family = AF_INET;
        memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
        dst_addr.sin_port= htons(port);

        if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
        {
            
            sts = sendto(sip_udp_socket, buffer, size, 0,
                 (const struct sockaddr *)&dst_addr,
                 (socklen_t)sizeof(dst_addr));
        }
        else  /* wan 侧 */
        {
            // TODO: 这里需要优化，目前PPPoE代理的时候没有默认路由
            // 因此需要进行判断，如果没有默认路由那么就需要使用使用sip_udp_wan_socket
            // 发包，否则一直使用 sip_udp_socket 发包
            // if (没有默认路由)
            if (0 != sip_udp_wan_socket)
            {
/* DT c40163 add for QOS */
#ifdef SUPPORT_ATP_QOS
			    sipsock_setopt(sip_udp_wan_socket, t_online, direct, tos);
#endif
/* DT c40163 end */
                sts = sendto(sip_udp_wan_socket, buffer, size, 0,
                     (const struct sockaddr *)&dst_addr,
                     (socklen_t)sizeof(dst_addr));
            }
            else if (0 == sip_udp_wan_socket)
            {
/* DT c40163 add for QOS */
#ifdef SUPPORT_ATP_QOS
			    sipsock_setopt(sip_udp_socket, t_online, direct, tos);
#endif
/* DT c40163 end */
                sts = sendto(sip_udp_socket, buffer, size, 0,
                     (const struct sockaddr *)&dst_addr,
                     (socklen_t)sizeof(dst_addr));
            }
            else
            {
                ERROR("no proper socket fd to send out");
            }
        }
        
        if (sts == -1) {
           if (errno != ECONNREFUSED) {
              ERROR("sendto() [%s:%i size=%i] call failed: %s",
                    utils_inet_ntoa(addr),
                    port, size, strerror(errno));
              return STS_FAILURE;
           }
           DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
                  utils_inet_ntoa(addr), port, strerror(errno));
        }
    }
    else
    {
      printf("sipsock_send: not in bind\n"); 

   sock=socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0) {
      printsip("Sip: socket() call failed: %s",strerror(errno));
      return STS_FAILURE;
   }

   #ifdef SUPPORT_ATP_QOS
   /* DT c40163 modify for QOS
   if (0 != ATP_SIP_ALG_QoS_SetMark(sock, NFMARK_SIPALG))
   {
       fprintf(stderr, "can not set the nfmakr %d for SIP alg", NFMARK_SIPALG);
   }
   */
   sipsock_setopt(sock, t_online, direct, tos);
   /* DT c40163 end */
   #endif

   dst_addr.sin_family = AF_INET;
   memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
   dst_addr.sin_port= htons(port);

   DEBUGC(DBCLASS_NET,"send UDP packet to %s: %i", utils_inet_ntoa(addr),port);
   
   printsip("Sip:send UDP packet to %s: %i\r\n", utils_inet_ntoa(addr),port);
   DUMP_BUFFER(DBCLASS_NETTRAF, buffer, size);

    /* 本地发送源 */
    bzero(&stLocalAddr, 0);
    /* 目的ip为LAN侧ip */
    if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
    {
        get_ip_by_ifname(configuration.inbound_if, &stLocalAddr.sin_addr);
    }
    else
    {
        get_ip_by_ifname(configuration.outbound_if, &stLocalAddr.sin_addr);
    }
/*    
    if (g_lPacketDir == 0)
    {
        get_ip_by_ifname(configuration.outbound_if,&stLocalAddr.sin_addr);
//    stLocalAddr.sin_port = htons(atoi(pszLocalPort));
    }
    else
    {
        get_ip_by_ifname(configuration.inbound_if,&stLocalAddr.sin_addr);
//    stLocalAddr.sin_port = htons(atoi(pszLocalPort));
    }
*/
    stLocalAddr.sin_family = AF_INET;
    
    if (bind(sock, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)) < 0)
    {
    	perror("bind");
    }
    else
    {
        printsip("Sip: bind src ip:[%s]\r\n", inet_ntoa(stLocalAddr.sin_addr));
    }

    // if (0 != sip_udp_socket)
        #if 0
    if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
        #else
        if (0)
        #endif
    {
        sts = sendto(sock, buffer, size, 0,
             (const struct sockaddr *)&dst_addr,
             (socklen_t)sizeof(dst_addr));
   }
   else if (0 != sip_udp_socket)
   {
       sts = sendto(sip_udp_socket, buffer, size, 0,
                 (const struct sockaddr *)&dst_addr,
                 (socklen_t)sizeof(dst_addr));
   }
   else
   {
        printf("no proper socket found \r\n");
   }
   
   if (sts == -1) {
      if (errno != ECONNREFUSED) {
         ERROR("sendto() [%s:%i size=%i] call failed: %s",
               utils_inet_ntoa(addr),
               port, size, strerror(errno));
         close(sock);
         return STS_FAILURE;
      }
      DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
             utils_inet_ntoa(addr), port, strerror(errno));
   }
   close(sock);
    }
   return STS_SUCCESS;
}
#else
/*
 * sends an UDP datagram to the specified destination
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 */
/* DT c40163 modify for QOS
int sipsock_send(struct in_addr addr, int port, int protocol,
                 char *buffer, int size) {
*/
int sipsock_send(struct in_addr addr, int port, int protocol,
                 char *buffer, int size, int t_online, int direct, int tos, int index) {
/* DT c40163 end */

   struct sockaddr_in dst_addr;
   int sts;
   int iSendToFd = 0;

   /* first time: allocate a socket for sending */
   if (0 == sip_udp_lan_socket) {
      ERROR("SIP socket not allocated");
      return STS_FAILURE;
   }

   if (NULL == buffer) {
      ERROR("sipsock_send got NULL buffer");
      return STS_FAILURE;
   }

   if (protocol != PROTO_UDP) {
      ERROR("sipsock_send: only UDP supported by now");
      return STS_FAILURE;
   }

   dst_addr.sin_family = AF_INET;
   memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
   dst_addr.sin_port= htons(port);

   DEBUGPC("send UDP packet to %s: %i", utils_inet_ntoa(addr),port);
//   DUMP_BUFFER(DBCLASS_NETTRAF, buffer, size);
   //now it comes to choosing the right socket
   if ((REQTYP_INCOMING == direct) || (RESTYP_INCOMING == direct) || (0 == direct))
   {
       iSendToFd = sip_udp_lan_socket;
   }
   else
   {
       if((index < 0) || (index > URLMAP_SIZE-1))
       {
           ERROR("Error, NO WAN socket to use!");
           return STS_FAILURE;
       }
       iSendToFd = urlmap[index].wan_socket_fd;
   }
   if (iSendToFd <= 0)
   {
       ERROR("Error, NO socket to send SIP msg!");
       return STS_FAILURE;
   }
   printsip("Sip: Func(sipsock_send) iSendToFd:[%d] index[%d] \r\n", iSendToFd, index);
    
/* DT c40163 add for QOS */
#ifdef SUPPORT_ATP_QOS
   sipsock_setopt(iSendToFd, t_online, direct, tos);
#endif
/* DT c40163 end */

   sts = sendto(iSendToFd, buffer, size, 0,
                (const struct sockaddr *)&dst_addr,
                (socklen_t)sizeof(dst_addr));

   printsip("Sip: Func(sipsock_send) iSendToFd:[%d] index[%d] sts[%d] strerror(errno))[%s]\r\n", iSendToFd, index, sts, strerror(errno));

   
   if (sts == -1) {
      if (errno != ECONNREFUSED) {
         ERROR("sendto() [%s:%i size=%i] call failed: %s",
               utils_inet_ntoa(addr),
               port, size, strerror(errno));
         return STS_FAILURE;
      }
      DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
             utils_inet_ntoa(addr), port, strerror(errno));
   }

   return STS_SUCCESS;
}
#endif


/*
 * generic routine to allocate and bind a socket to a specified
 * local address and port (UDP)
 * errflg !=0 log errors, ==0 don't
 *
 * RETURNS socket number on success, zero on failure
 */
#ifdef SUPPORT_ATP_QOS
int sockbind(struct in_addr ipaddr, int localport, int errflg, int sockflag) 
#else
int sockbind(struct in_addr ipaddr, int localport, int errflg) 
#endif
{
   struct sockaddr_in my_addr;
   int sts;
   int sock;
   int flags;
   unsigned char on = 0;

   memset(&my_addr, 0, sizeof(my_addr));

   my_addr.sin_family = AF_INET;
   memcpy(&my_addr.sin_addr.s_addr, &ipaddr, sizeof(struct in_addr));
   my_addr.sin_port = htons(localport);

   sock=socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0) {
      ERROR("socket() call failed: %s",strerror(errno));
      return 0;
   }
#ifdef  SUPPORT_ATP_QOS
    if (SOCK_CREATE_BY_SIP == sockflag)
    {
        if (0 != ATP_SIP_ALG_QoS_SetMark(sock, NFMARK_SIPALG))
        {
            fprintf(stderr, "can not set the nfmakr %d for SIP alg", NFMARK_SIPALG);
        }
    }
    else if (SOCK_CREATE_BY_RTP == sockflag)
    {
        if (0 != ATP_SIP_ALG_QoS_SetMark(sock, NFMARK_RTP))
        {
            fprintf(stderr, "can not set the nfmakr %d for SIP alg", NFMARK_RTP);
        }
    }
    else if (SOCK_CREATE_BY_RTCP == sockflag)
    {
        if (0 != ATP_SIP_ALG_QoS_SetMark(sock, NFMARK_RTCP))
        {
            fprintf(stderr, "can not set the nfmakr %d for SIP alg", NFMARK_RTCP);
        }
    }
    else
    {
        /* DT c40163 modify for QOS
        fprintf(stderr, "can not known which flow this is");
        */
        if (0 != ATP_SIP_ALG_QoS_SetMark(sock, 0))
        {
            fprintf(stderr, "can not set the nfmakr %d for SIP alg", 0);
        }
		/* DT c40163 end */
    }
#endif

#ifdef TOS_RCV
   on = 1;
   sts = setsockopt(sock, IPPROTO_IP, IP_RECVTOS, (void*)&on, sizeof(on));
   if (sts != 0)
   {
       printf("sockbind setsockopt IP_RECVTOS fail, sts %d %s\n", sts, strerror(errno));
   }
#endif

#ifdef SUPPORT_ATP_HYBRID
   on = 1;
   sts = setsockopt(sock, IPPROTO_IP, IP_RECVMARK, (void*)&on, sizeof(on));
   if (sts != 0)
   {
       printf("sockbind setsockopt IP_RECVMARK fail, sts %d %s\n", sts, strerror(errno));
   }
#endif


#if 0
    /* 重用地址，将来便于扩展，以后可能当用户选中接口为auto的时候可以考虑将所有接口全部传入从而一个一个发送尝试 */
    {
        on = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int)) < 0)
        {
            ERROR("set socket reuse addr error : %s", strerror(errno));
        }
    }
#endif

   sts=bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr));
   if (sts != 0) {
      if (errflg) ERROR("bind failed: %s",strerror(errno));
      close(sock);
      return 0;
   }

   /*
    * It has been seen on linux 2.2.x systems that for some
    * reason (bug?) inside the RTP relay, select()
    * claims that a certain file descriptor has data available to
    * read, a subsequent call to read() or recv() then does block!!
    * So lets make the FD's we are going to use non-blocking, so
    * we will at least survive and not run into a deadlock.
    *
    * There is a way to (more or less) reproduce this effect:
    * Make a local UA to local UA call and then very quickly do
    * HOLD/unHOLD, several times.
    */
   flags = fcntl(sock, F_GETFL);
   if (flags < 0) {
      ERROR("fcntl(F_SETFL) failed: %s",strerror(errno));
      close(sock);
      return 0;
   }
   if (fcntl(sock, F_SETFL, (long) flags | O_NONBLOCK) < 0) {
      ERROR("fcntl(F_SETFL) failed: %s",strerror(errno));
      close(sock);
      return 0;
   }
   printsip("Sip: Func(sockbind) ipaddr:[%s],localport:[%d], errflg:[%d] \r\n", 
       inet_ntoa(ipaddr), localport, errflg);
   return sock;
}

//用户发注册报文时，为每个PC单独分配一个wan socket
int get_wan_socket(char *ifname, int *plWanPort)
{
    struct in_addr wanipaddr;
    static int iWanPort = 0;
    int iRetFd = 0;
    int i;

    if (!wanport)
    {
        ERROR("NULL param wanport!");
        return 0;
    }
    //从默认配置的56005开始尝试端口号
    if ((0 == iWanPort) || (iWanPort >= (configuration.sip_listen_port + 2*URLMAP_SIZE)))
    {
        iWanPort = configuration.sip_listen_port; 
    }
    
    memset(&wanipaddr, 0, sizeof(wanipaddr));
    if (STS_FAILURE == get_ip_by_ifname(ifname, &wanipaddr))
    {
        ERROR("%s ip: %s", configuration.inbound_if, utils_inet_ntoa(wanipaddr));
        ERROR("can not get the wan ip , please check it.......");
        return 0;
    }
    iRetFd = sockbind(wanipaddr, iWanPort, 1, SOCK_CREATE_BY_SIP);
    //绑定不成功，重试最多URLMAP_SIZE次
    if (0 == iRetFd)
    {
        for (i = 0; i < URLMAP_SIZE; i++)
        {
            iWanPort++;
            iRetFd = sockbind(wanipaddr, iWanPort, 1, SOCK_CREATE_BY_SIP);
            if (0 == iRetFd)
            {
                ERROR("SIPROXD bind wan port[%d] error", iWanPort);
                continue;
            }
            else
            {
                DEBUG("SIPROXD bind wan port[%d] success", iWanPort);
                break;
            }
        }
    }
    DEBUG("SIPROXD get wan port=[%d], sockfd=[%d]", iWanPort, iRetFd);
    *plWanPort = iWanPort;
    iWanPort++;
    return iRetFd;
}
