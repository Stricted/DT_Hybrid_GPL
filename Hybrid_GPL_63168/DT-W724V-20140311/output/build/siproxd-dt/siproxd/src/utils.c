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

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>

#ifdef _SOLARIS2
# include <sys/sockio.h>
#endif

#include <sys/types.h>
#include <pwd.h>

#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "log.h"
#include "atpconfig.h"

//调用ATP_UTIL_GetOutputIntf需包含"atputil.h"
#include "atputil.h"

static char const ident[]="$Id: utils.c,v 1.3 2009/06/19 10:28:37 y42304 Exp $";

#ifdef SUPPORT_ATP_BRIDGE_BIND
extern int g_lOutIFaceIsProxy;
extern char g_acPPPoEIpAddr[SIP_IPADDR];
extern char g_acPPPoEMask[SIP_IPADDR];
extern char g_acBrIpAddr[SIP_IPADDR];
extern char g_acBrMask[SIP_IPADDR];
#endif

/* configuration storage */
extern struct siproxd_config configuration;

extern struct urlmap_s urlmap[];        /* URL mapping table     */


extern int h_errno;


/*
 * resolve a hostname and return in_addr
 * handles its own little DNS cache.
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on failure
 */
int get_ip_by_host(char *hostname, struct in_addr *addr) {
   int i, j;
   time_t t;
   struct timeval stTm;
   struct hostent *hostentry;
#if defined(HAVE_GETHOSTBYNAME_R)
   struct hostent result_buffer;
   char tmp[GETHOSTBYNAME_BUFLEN];
#endif
   int error;
   static struct {
      time_t timestamp;
      struct in_addr addr;
      char hostname[HOSTNAME_SIZE+1];
   } dns_cache[DNS_CACHE_SIZE];
   static int cache_initialized=0;

   if (hostname == NULL) {
      ERROR("get_ip_by_host: NULL hostname requested");
      return STS_FAILURE;
   }

   if (addr == NULL) {
      ERROR("get_ip_by_host: NULL in_addr passed");
      return STS_FAILURE;
   }

   /* first time: initialize DNS cache */
   if (cache_initialized == 0) {
      DEBUGC(DBCLASS_DNS, "initializing DNS cache (%i entries)", DNS_CACHE_SIZE);
      memset(dns_cache, 0, sizeof(dns_cache));
      cache_initialized=1;
   }

   //time(&t);
   ATP_UTIL_GetSysTime(&stTm);
   t = stTm.tv_sec;
   /* clean expired entries */
   for (i=0; i<DNS_CACHE_SIZE; i++) {
      if (dns_cache[i].hostname[0]=='\0') continue;
      if ( (dns_cache[i].timestamp+DNS_MAX_AGE) < t ) {
         DEBUGC(DBCLASS_DNS, "cleaning DNS cache (entry %i)", i);
         memset (&dns_cache[i], 0, sizeof(dns_cache[0]));
      }
   }

   /*
    * search requested entry in cache
    */
   for (i=0; i<DNS_CACHE_SIZE; i++) {
      if (dns_cache[i].hostname[0]=='\0') continue; /* empty */
      if (strcmp(hostname, dns_cache[i].hostname) == 0) { /* match */
         memcpy(addr, &dns_cache[i].addr, sizeof(struct in_addr));
         DEBUGC(DBCLASS_DNS, "DNS lookup - from cache: %s -> %s",
	        hostname, utils_inet_ntoa(*addr));
         return STS_SUCCESS;
      }
   }
   
   /* did not find it in cache, so I have to resolve it */

   /* need to deal with reentrant versions of gethostbyname_r()
    * as we may use threads... */
#if defined(HAVE_GETHOSTBYNAME_R)

   /* gethostbyname_r() with 3 arguments (e.g. osf/1) */
   #if defined(HAVE_FUNC_GETHOSTBYNAME_R_3)
   gethostbyname_r(hostname,		/* the FQDN */
		   &result_buffer,	/* the result buffer */ 
		   &hostentry
		   );
   if (hostentry == NULL) error = h_errno;

   /* gethostbyname_r() with 5 arguments (e.g. solaris, linux libc5) */
   #elif defined(HAVE_FUNC_GETHOSTBYNAME_R_5)
   hostentry = gethostbyname_r(hostname,        /* the FQDN */
			       &result_buffer,  /* the result buffer */
			       tmp,
			       GETHOSTBYNAME_BUFLEN,
			       &error);

   /* gethostbyname_r() with 6 arguments (e.g. linux glibc) */
   #elif defined(HAVE_FUNC_GETHOSTBYNAME_R_6)
   gethostbyname_r(hostname,        /* the FQDN */
		   &result_buffer,  /* the result buffer */
		   tmp,
		   GETHOSTBYNAME_BUFLEN,
		   &hostentry,
		   &error);
   #else
      #error "gethostbyname_r() with 3, 5 or 6 arguments supported only"
   #endif   
#elif defined(HAVE_GETHOSTBYNAME)
   hostentry=gethostbyname(hostname);
   if (hostentry == NULL) error = h_errno;
#else
   #error "need gethostbyname() or gethostbyname_r()"
#endif

   if (hostentry==NULL) {
      /*
       * Some errors just tell us that there was no IP resolvable.
       * From the manpage:
       *   HOST_NOT_FOUND
       *      The specified host is unknown.
       *   HOST_NOT_FOUND
       *      The specified host is unknown.
       *   NO_ADDRESS or NO_DATA
       *      The requested name is valid but does not have an IP
       *      address.
       */
      if ((error == HOST_NOT_FOUND) ||
          (error == NO_ADDRESS) ||
          (error == NO_DATA)) {
#ifdef HAVE_HSTRERROR
         DEBUGC(DBCLASS_DNS, "gethostbyname(%s) failed: h_errno=%i [%s]",
                hostname, h_errno, hstrerror(error));
#else
         DEBUGC(DBCLASS_DNS, "gethostbyname(%s) failed: h_errno=%i",
                hostname, error);
#endif
      } else {
#ifdef HAVE_HSTRERROR
         ERROR("gethostbyname(%s) failed: h_errno=%i [%s]",
               hostname, h_errno, hstrerror(h_errno));
#else
         ERROR("gethostbyname(%s) failed: h_errno=%i",hostname, h_errno);
#endif
      }
      return STS_FAILURE;
   }

   memcpy(addr, hostentry->h_addr, sizeof(struct in_addr));
   DEBUGC(DBCLASS_DNS, "DNS lookup - resolved: %s -> %s",
          hostname, utils_inet_ntoa(*addr));

   /*
    * find an empty slot in the cache
    */
   j=0;
   for (i=0; i<DNS_CACHE_SIZE; i++) {
      if (dns_cache[i].hostname[0]=='\0') break;
      if (dns_cache[i].timestamp < t) {
         /* remember oldest entry */
         t=dns_cache[i].timestamp;
	 j=i;
      }
   }
   /* if no empty slot found, take oldest one */
   if (i >= DNS_CACHE_SIZE) i=j;

   /*
    * store the result in the cache
    */
   DEBUGC(DBCLASS_DNS, "DNS lookup - store into cache, entry %i)", i);
   memset(&dns_cache[i], 0, sizeof(dns_cache[0]));
   strncpy(dns_cache[i].hostname, hostname, HOSTNAME_SIZE);
   //time(&dns_cache[i].timestamp);
   ATP_UTIL_GetSysTime(&stTm);
   dns_cache[i].timestamp = stTm.tv_sec;
   memcpy(&dns_cache[i].addr, addr, sizeof(struct in_addr));
   printsip("Sip: Func:get_ip_by_host(), hostname:[%s], addr:[%s]\r\n", 
       hostname, inet_ntoa(*addr));
   
   return STS_SUCCESS;
}


/*
 * Secure enviroment:
 * If running as root, put myself into a chroot jail and
 * change UID/GID to user as requested in config file
 */
void secure_enviroment (void) {
   int sts;
   struct passwd *passwd=NULL;

   DEBUGC(DBCLASS_CONFIG,"running w/uid=%i, euid=%i, gid=%i, egid=%i",
          getuid(), geteuid(), getgid(), getegid());

   if ((getuid()==0) || (geteuid()==0)) {
      /*
       * preparation - after chrooting there will be NOTHING more around
       */
      if (configuration.user) passwd=getpwnam(configuration.user);

      /*
       * change root directory into chroot jail
       */
      if (configuration.chrootjail) {
         /* !!!
          * Before chrooting I must at least once trigger the resolver
          * as it loads some dynamic libraries. Once chrootet
          * these libraries will *not* be found and gethostbyname()
          * calls will simply fail (return NULL pointer and h_errno=0).
          * Also (at least for FreeBSD) syslog() needs to be called
          * before chroot()ing - this is done in main() by an INFO().
          * Took me a while to figure THIS one out
          */
         struct in_addr dummy;
         get_ip_by_host("localhost", &dummy);
         DEBUGC(DBCLASS_CONFIG,"chrooting to %s",
                configuration.chrootjail);
         sts = chroot(configuration.chrootjail);
	 if (sts != 0) DEBUGC(DBCLASS_CONFIG,"chroot(%s) failed: %s",
	                      configuration.chrootjail, strerror(errno));
         chdir("/");
      }

     /***************************************************************************************************
        sip启用用户名为user = nobody，如果有其他业务如ftp(usb)等启用(其用户名一般也为user)，
        此时促使siproxy在读取etc/passwd时发现不为空，导致passwd也不为空，这样siproxy会修改uid和gid，
        这样权限改为非root(root:0:0  other:99:99)，导致setsockopt失败。
        如:
      sts = setsockopt(rtp_proxytable[i].rtp_tx_sock, IPPROTO_IP, IP_TOS, (void*)&ucTos, sizeof(ucTos));
    **************************************************************************************************/
 #if 0
      /*
       * change user ID and group ID 
       */
      if (passwd) {
         DEBUGC(DBCLASS_CONFIG,"changing uid/gid to %s",
                configuration.user);
         sts = setgid(passwd->pw_gid);
         DEBUGC(DBCLASS_CONFIG,"changed gid to %i - %s",
	        passwd->pw_gid, (sts==0)?"Ok":"Failed");

         sts = setegid(passwd->pw_gid);
         DEBUGC(DBCLASS_CONFIG,"changed egid to %i - %s",
	        passwd->pw_gid, (sts==0)?"Ok":"Failed");

/* don't set the real user id - as we need to elevate privs
   when setting up an RTP masquerading tunnel */
/*&&& Actually this is no longer true (7-Jul-2004/xar) */
//         sts = setuid(passwd->pw_uid);
//         DEBUGC(DBCLASS_CONFIG,"changed uid to %i - %s",
//	        passwd->pw_uid, (sts==0)?"Ok":"Failed");

         sts = seteuid(passwd->pw_uid);
         DEBUGC(DBCLASS_CONFIG,"changed euid to %i - %s",
	        passwd->pw_uid, (sts==0)?"Ok":"Failed");
      }
      #endif
   }
}


char * get_if_by_telecomlist(char *DstIP, int SrcPort, int t_online, unsigned int ulMark, char* domain)
{
    ATP_UTIL_PKT_INFO_ST stPkt = {0};

    if (t_online == 1)
    {
        return VOICE_DSL_INTF_NAME;
    }
    printf("in get_if_by_telecomlist ulMark = %x", ulMark);
    if (ulMark & 0xf0000000)
    {
        return VOICE_DSL_INTF_NAME;
    }
    if (check_TelecomList_bydomain(domain))
    {
        return VOICE_DSL_INTF_NAME;
    }

    
    memset(&stPkt, 0, sizeof(stPkt));
    snprintf(stPkt.acDstIP, sizeof(stPkt.acDstIP), "%s", DstIP);
    stPkt.ucProtocol = IPPROTO_UDP;
    stPkt.ucTos = QOSDT_TOS_BE;
    stPkt.ulMark = QOSDT_MARK_BE;
    stPkt.usDstPort = 5060;
    stPkt.usSrcPort = SrcPort;
    return ATP_UTIL_GetOutputIntf(&stPkt);
}

int check_TelecomList_bydomain(char *domain)
{
#define MAX_DOMAIN_NAME 64
    char file[MAX_DOMAIN_NAME] = {0};
    snprintf(file, sizeof(file), "/var/voice/%s", domain);
    
    if (0 == access(file, F_OK))
    {
        return 1;
    }
    else
    {
        return 0;
    }
#undef MAX_DOMAIN_NAME
}



/*
 * get_ip_by_ifname:
 * fetches own IP address by its interface name
 *
 * STS_SUCCESS on returning a valid IP and interface is UP
 * STS_FAILURE if interface is DOWN or other problem
 */
int get_ip_by_ifname(char *ifname, struct in_addr *retaddr) {
   struct ifreq ifr;
   struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
   int sockfd;
   int i, j;
   int ifflags, isup;
   time_t t;
   struct timeval stTm;
   static struct {
      time_t timestamp;
      struct in_addr ifaddr;	/* IP */
      int isup;			/* interface is UP */
      char ifname[IFNAME_SIZE+1];
   } ifaddr_cache[IFADR_CACHE_SIZE];
   static int cache_initialized=0;

   if (ifname == NULL) {
      WARN("get_ip_by_ifname: got NULL ifname passed - please check config"
           "file ('if_inbound' and 'if_outbound')");
      return STS_FAILURE;
   }

   /* first time: initialize ifaddr cache */
   if (cache_initialized == 0) {
      DEBUGC(DBCLASS_DNS, "initializing ifaddr cache (%i entries)", 
             IFADR_CACHE_SIZE);
      memset(ifaddr_cache, 0, sizeof(ifaddr_cache));
      cache_initialized=1;
   }

   if (retaddr) memset(retaddr, 0, sizeof(struct in_addr));

   //time(&t);
   ATP_UTIL_GetSysTime(&stTm);
   t = stTm.tv_sec;
   /* clean expired entries */
   for (i=0; i<IFADR_CACHE_SIZE; i++) {
      if (ifaddr_cache[i].ifname[0]=='\0') continue;
      if ( (ifaddr_cache[i].timestamp+IFADR_MAX_AGE) < t ) {
         DEBUGC(DBCLASS_DNS, "cleaning ifaddr cache (entry %i)", i);
         memset (&ifaddr_cache[i], 0, sizeof(ifaddr_cache[0]));
      }
   }

   /*
    * search requested entry in cache
    */
   for (i=0; i<IFADR_CACHE_SIZE; i++) {
      if (ifaddr_cache[i].ifname[0]=='\0') continue;
      if (strcmp(ifname, ifaddr_cache[i].ifname) == 0) { /* match */
         if (retaddr) memcpy(retaddr, &ifaddr_cache[i].ifaddr,
                             sizeof(struct in_addr));
         DEBUGC(DBCLASS_DNS, "ifaddr lookup - from cache: %s -> %s %s",
	        ifname, utils_inet_ntoa(ifaddr_cache[i].ifaddr),
                (ifaddr_cache[i].isup)? "UP":"DOWN");
         return (ifaddr_cache[i].isup)? STS_SUCCESS: STS_FAILURE;
      } /* if */
   } /* for i */

   /* not found in cache, go and get it */
   memset(&ifr, 0, sizeof(ifr));

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      ERROR("Error in socket: %s\n",strerror(errno));
      return STS_FAILURE;
   }

   strcpy(ifr.ifr_name, ifname);
   sin->sin_family = AF_INET;

   /* get interface flags */
   if(ioctl(sockfd, SIOCGIFFLAGS, &ifr) != 0) {
      ERROR("Error in ioctl SIOCGIFFLAGS: %s [%s]\n",
            strerror(errno), ifname);
      close(sockfd);
      return STS_FAILURE;
   } 
   ifflags=ifr.ifr_flags;

   /* get address */
   if(ioctl(sockfd, SIOCGIFADDR, &ifr) != 0) {
      ERROR("Error in ioctl SIOCGIFADDR: %s (interface %s)\n",
      strerror(errno), ifname);
      close(sockfd);
      return STS_FAILURE;
   } 

   if (ifflags & IFF_UP) isup=1;
   else isup=0;

   DEBUGC(DBCLASS_DNS, "get_ip_by_ifname: if %s has IP:%s (flags=%x) %s",
          ifname, utils_inet_ntoa(sin->sin_addr), ifflags,
          (isup)? "UP":"DOWN");
   
   printsip("Sip:get_ip_by_ifname: if[%s] has IP:[%s] (flags=[%x]) [%s] \r\n",
          ifname, utils_inet_ntoa(sin->sin_addr), ifflags,
          (isup)? "UP":"DOWN");

#ifdef SUPPORT_ATP_BRIDGE_BIND
   if ((1 == g_lOutIFaceIsProxy) && (strcmp(ifname, "br0") == 0))
   {
       if (0 == utils_inet_aton(g_acPPPoEIpAddr, &sin->sin_addr))
       {
           printsip("Sip: PPPoEIp:[%s]\r\n", g_acPPPoEIpAddr);
           utils_inet_aton("192.168.239.1", &sin->sin_addr);
       }
       printsip("Sip: The Proxy Br0 Addr:[%s] \r\n", inet_ntoa(sin->sin_addr));
   }
#endif
   /*
    *find an empty slot in the cache
    */
   j=0;
   for (i=0; i<IFADR_CACHE_SIZE; i++) {
      if (ifaddr_cache[i].ifname[0]=='\0') break;
      if (ifaddr_cache[i].timestamp < t) {
         /* remember oldest entry */
         t=ifaddr_cache[i].timestamp;
	 j=i;
      }
   }
   /* if no empty slot found, take oldest one */
   if (i >= IFADR_CACHE_SIZE) i=j;

   /*
    * store the result in the cache
    */
   DEBUGC(DBCLASS_DNS, "ifname lookup - store into cache, entry %i)", i);
   memset(&ifaddr_cache[i], 0, sizeof(ifaddr_cache[0]));
   strncpy(ifaddr_cache[i].ifname, ifname, IFNAME_SIZE);
   ifaddr_cache[i].timestamp=t;
   memcpy(&ifaddr_cache[i].ifaddr, &sin->sin_addr, sizeof(sin->sin_addr));
   ifaddr_cache[i].isup=isup;

   if (retaddr) memcpy(retaddr, &sin->sin_addr, sizeof(sin->sin_addr));

   close(sockfd);
   return (isup)? STS_SUCCESS : STS_FAILURE;
}


/*
 * utils_inet_ntoa:
 * implements an inet_ntoa()
 *
 * Returns pointer to a static character string.
 */
char *utils_inet_ntoa(struct in_addr in) {
#if defined(HAVE_INET_NTOP)
   static char string[INET_ADDRSTRLEN];
   if ((inet_ntop(AF_INET, &in, string, INET_ADDRSTRLEN)) == NULL) {
      ERROR("inet_ntop() failed: %s\n",strerror(errno));
      string[0]='\0';
   }
   return string;
#elif defined(HAVE_INET_NTOA)
   return inet_ntoa(in);
#else
#error "need inet_ntop() or inet_ntoa()"
#endif
}


/*
 * utils_inet_aton:
 * implements an inet_aton()
 *
 * converts the string in *cp and stores it into inp
 * Returns != 0 on success
 */
int  utils_inet_aton(const char *cp, struct in_addr *inp) {
#if defined(HAVE_INET_PTON)
   return inet_pton (AF_INET, cp, inp);
#elif defined(HAVE_INET_ATON)
   return inet_aton(cp, inp);
#else
#error "need inet_pton() or inet_aton()"
#endif
}

/* 本函数返回的是 从 目的地址为 ip 的，通过本地哪个接口可以到达的接口，该本地接口存放在 local_ip 中 */
void get_local_ip(char *ip, struct in_addr *local_ip, int idx) {
   /* Returns the local IP address used to reach ip. */
   int sock;
   struct sockaddr_in local;
   struct sockaddr_in remote;
#ifdef SUPPORT_ATP_BRIDGE_BIND
   struct sockaddr_in stLocalAddr;
#endif
   int namelen;
   struct in_addr temip;
   int i;

   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
      perror("socket");
   }

   for (i = 0; i < strlen(ip); i++)
   {
       if (((ip[i] < '0') || (ip[i] > '9')) && (ip[i] != '.'))
       {
           break;
       }
   }

   memset((char *) &remote, 0, sizeof(remote));
   remote.sin_family = AF_INET;
   if (i < strlen(ip))
   {
       /* 添加域名解析 */
       if (STS_FAILURE == get_ip_by_host(ip, &temip))
       {
           printf("resolve failed.\n");
       }
       remote.sin_addr.s_addr = temip.s_addr;
   }
   else
   {
       remote.sin_addr.s_addr = inet_addr(ip);
   }
#ifdef SUPPORT_ATP_BRIDGE_BIND
   /* 本地发送源 */
    bzero(&stLocalAddr, 0);

    /* 目的ip为LAN侧ip */
    if ((1 == IpisInNet(ip, g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(ip, g_acPPPoEIpAddr, g_acPPPoEMask)))
    {
        get_ip_by_ifname(configuration.inbound_if, &stLocalAddr.sin_addr);
        
    }
    else
    {
        //get_ip_by_ifname(urlmap[idx].outbound_if, &stLocalAddr.sin_addr);
        memcpy(&stLocalAddr.sin_addr, &urlmap[idx].outbound_addr, sizeof(stLocalAddr.sin_addr));
    }
/*    
    if (g_lPacketDir == 0)
    {
        get_ip_by_ifname(configuration.outbound_if, &stLocalAddr.sin_addr);
//    stLocalAddr.sin_port = htons(atoi(pszLocalPort));
    }
    else
    {
        get_ip_by_ifname(configuration.inbound_if, &stLocalAddr.sin_addr);
    }
*/    
    stLocalAddr.sin_family = AF_INET;
    
    if (bind(sock, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)) < 0)
    {
        DEBUGPC("bind error can not get for %s.........", ip);
    	perror("bind");
    }
    else
    {
        printsip("Sip:get_local_ip() bind src ip:[%s]\r\n", inet_ntoa(stLocalAddr.sin_addr));
    }
#endif
   /* 此处连接自动寻找路由，然后通过getsockname可以获得下一跳路由地址 */
   if (connect(sock, (struct sockaddr *) &remote, sizeof(remote))) {
      perror("connect");
   }
   else
    {
        printsip("Sip:get_local_ip() connect Remoteip:[%s]\r\n", inet_ntoa(remote.sin_addr));
    }

   namelen = sizeof(local);
   if (getsockname(sock, (struct sockaddr *) &local,
		   (socklen_t *) &namelen) < 0) {
      perror("getsockname");
   }

   if (close(sock)) {
      perror("close");
   }   
   memcpy(local_ip, &local.sin_addr, sizeof(struct in_addr));

   
   DEBUGPC("---------------- notice: Sip: ip:[%s], local_ip[%s] \r\n", ip, inet_ntoa(*local_ip));
}
#if defined(SUPPORT_ATP_SIPPXY_RECORDDSTIP)
#undef SIPALG_DSTIP
#define SIPALG_DSTIP 71

/* 
 * search dst ip base on src ip. , 0 means fail. else sucess
 */
int getPrxoyDstIp(const osip_message_t *sipMsg, struct in_addr *dstIp)
{
    int lSock = -1;
    int call_id_len = 0;
    int lRet = STS_FAILURE;
    struct SipRouteInfoItem stRouteInfo;
    unsigned int ulRet = sizeof(struct SipRouteInfoItem);

    
    char *tmp;
    int i;
    
    if (NULL == dstIp)
    {
        return lRet;
    }
    
    memset(&stRouteInfo, 0, sizeof(struct SipRouteInfoItem));
    
    i = osip_call_id_to_str (sipMsg->call_id, &tmp);
    if (i != 0)
    {
        return lRet;
    }
    
    call_id_len = strlen(tmp);
    stRouteInfo.call_id = (char *)malloc((call_id_len+1)*sizeof(char));
    if (0 == stRouteInfo.call_id)
    {
        DEBUGPC("---------------- in getPrxoyDstIp malloc stRouteInfo.call_id fail \r\n");
        osip_free(tmp);
        return lRet;
    }
    memset(stRouteInfo.call_id, 0, call_id_len+1);
    snprintf(stRouteInfo.call_id, call_id_len+1, "%s", tmp);
    osip_free (tmp);
    
    if (0 <= (lSock = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        if (0 == getsockopt(lSock, IPPROTO_IP, SIPALG_DSTIP, (void*)&stRouteInfo, (void*)&ulRet))
        {
            lRet = STS_SUCCESS;
            dstIp->s_addr= stRouteInfo.daddr.s_addr;
        }
        close(lSock);
    }

    if (0 != stRouteInfo.call_id)
    {
        free(stRouteInfo.call_id);
    }
    return lRet;
}

/* 
 * whether the string is host or ip address
 */
int ValidateIpAddress(const char *pcIpAddr) 
{
    unsigned int  i = 0;
    int lRet = 0;
    char    *pcToken = NULL;
    char    *pcLast = NULL; 
    char    *pcEnd = NULL;
    char    acBuf[SYS_IP_LENGTH];
    int     lNum = 0;
    char   *pcTemp;

    if ((pcIpAddr == NULL) || (strlen(pcIpAddr) >= SYS_IP_LENGTH))
    {
        return lRet;
    }

    memset(acBuf, 0, sizeof(acBuf));

    snprintf(acBuf, SYS_IP_LENGTH, "%s", pcIpAddr);

    pcTemp = acBuf;
    while(*pcTemp != '\0')
    {
        if (*pcTemp == '.')
        {
            i++;
        }
        pcTemp++;
    }
    if (i != SYS_IP_BYTE_FOUR)
    {
        return lRet;
    }
    
    pcToken = strtok_r(acBuf, ".", &pcLast);
    if (pcToken == NULL)
    {
        return lRet;
    }
    if (!((*pcToken >= '0')&&(*pcToken <= '9')))
    {
        return lRet;
    }
    lNum = strtol(pcToken, &pcEnd, SYS_BASE_DEC);    

    if ( (*pcEnd == '\0') && (lNum <= SYS_IP_MAX_NUMBER) ) 
    {
        for ( i = 0; i < (SYS_IP_BYTE - 1); i++ ) 
        {
            pcToken = strtok_r(NULL, ".", &pcLast);
            if ( pcToken == NULL )
            {
                break;
            }
            if (!((*pcToken >= '0')&&(*pcToken <= '9')))
            {
                break;
            }
            lNum = strtol(pcToken, &pcEnd, SYS_BASE_DEC);
            if ( (*pcEnd != '\0') || (lNum > SYS_IP_MAX_NUMBER) )
            {
                break;
            }
        }
       
        if ( i == (SYS_IP_BYTE - 1) )
        {
            lRet = 1;
        }
    }

    return lRet;
}
#endif
