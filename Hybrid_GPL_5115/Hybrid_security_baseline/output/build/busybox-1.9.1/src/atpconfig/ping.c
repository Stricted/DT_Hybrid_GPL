/* vi: set sw=4 ts=4: */
/*
 * Mini ping implementation for busybox
 *
 * Copyright (C) 1999 by Randolph Chung <tausq@debian.org>
 *
 * Adapted from the ping in netkit-base 0.10:
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */
/* from ping6.c:
 * Copyright (C) 1999 by Randolph Chung <tausq@debian.org>
 *
 * This version of ping is adapted from the ping in netkit-base 0.10,
 * which is:
 *
 * Original copyright notice is retained at the end of this file.
 *
 * This version is an adaptation of ping.c from busybox.
 * The code was modified by Bart Visscher <magick@linux-fan.com>
 */

#include <net/if.h>
#include <netinet/ip_icmp.h>
#include "libbb.h"
#include "atpconfig.h"
#include "ifaddrs.h"

/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
#include "msgapi.h"
#endif
/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/

#if ENABLE_ATP_PING6
#include <netinet/icmp6.h>
/* I see RENUMBERED constants in bits/in.h - !!?
 * What a fuck is going on with libc? Is it a glibc joke? */
#ifdef IPV6_2292HOPLIMIT
#undef IPV6_HOPLIMIT
#define IPV6_HOPLIMIT IPV6_2292HOPLIMIT
#endif
#endif

enum {
	DEFDATALEN = 56,
	MAXIPLEN = 60,
	MAXICMPLEN = 76,
	MAXPACKET = 65468,
	MAX_DUP_CHK = (8 * 128),
	MAXWAIT = 10 * 1000,
	PINGINTERVAL = 1, /* 1 second */
};

#define ATP_PING_WRITE_FILE_TRUE 1
#define ATP_PING_WRITE_FILE_FLASE 0
#define PING_PID_FILE   "/var/pingPid"
#define PING_RESULT_FILE "/var/pingStats"
FILE *pfPingStats=NULL;
FILE *pingPid=NULL;
char g_acHost[64] = {0};	

unsigned long g_ulSendPacketCount = 0;
#define ATP_PING_COUNT 3

/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
#define VOS_OK                          0
#define VOS_NOK                         1
#define VOS_ERROR                       -1
#define DEV_PING_PID_FILE               "/var/DevpingPid"
#define DEV_PING_RESULT_FILE            "/var/Devpingstatus"
#define Ping_LOCK_FILE                  "/var/DevPingLockFile"
#define ATP_CBBID_PING_NAME             "ping"
#define DIAG_LINE_LENGTH                128
#define DEV_PINGCOUNT_LEN               16
#define DEV_PINGCOUNT_FLASH_LEN         128
#define ATP_PING_INTERVAL               1
int pingstopflag = 0;
int pingWandownflag = 0;
int g_iInterfaceFlag = 0;
struct sockaddr_in pinterfaceipsin;
static void pingstophandle(void);
static void pingwandownhandle(void);
static int PingLockFile(int iFd,int iMode);
static int PingOpenAndLockFile(void);
static int get_if_addr(int sock, char *name, struct sockaddr_in *sin);
#endif
/* END: add by h00190880 for CT-COM ping on 2012/12/5*/

static int g_lPingFlag = ATP_PING_WRITE_FILE_FLASE;

static void PingToLogWeb(int finish);
extern struct hostent *ATP_GPL_Gethostbyname(const char *name, int family, char* bindaddr);

/* common routines */

static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return ans;
}

#if 0

/* simple version */

static char *hostname;

static void noresp(int ign ATTRIBUTE_UNUSED)
{
	printf("No response from %s\n", hostname);
	exit(EXIT_FAILURE);
}

static void ping4(len_and_sockaddr *lsa)
{
	struct sockaddr_in pingaddr;
	struct icmp *pkt;
	int pingsock, c;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];

	pingsock = create_icmp_socket();
	pingaddr = lsa->sin;

	pkt = (struct icmp *) packet;
	memset(pkt, 0, sizeof(packet));
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));

	c = xsendto(pingsock, packet, DEFDATALEN + ICMP_MINLEN,
			   (struct sockaddr *) &pingaddr, sizeof(pingaddr));

	/* listen for replies */
	while (1) {
		struct sockaddr_in from;
		socklen_t fromlen = sizeof(from);

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		if (c >= 76) {			/* ip + icmp */
			struct iphdr *iphdr = (struct iphdr *) packet;

			pkt = (struct icmp *) (packet + (iphdr->ihl << 2));	/* skip ip hdr */
			if (pkt->icmp_type == ICMP_ECHOREPLY)
				break;
		}
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		close(pingsock);
}

#if ENABLE_ATP_PING6
static void ping6(len_and_sockaddr *lsa)
{
	struct sockaddr_in6 pingaddr;
	struct icmp6_hdr *pkt;
	int pingsock, c;
	int sockopt;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];

	pingsock = create_icmp6_socket();
	pingaddr = lsa->sin6;

	pkt = (struct icmp6_hdr *) packet;
	memset(pkt, 0, sizeof(packet));
	pkt->icmp6_type = ICMP6_ECHO_REQUEST;

	sockopt = offsetof(struct icmp6_hdr, icmp6_cksum);
	setsockopt(pingsock, SOL_RAW, IPV6_CHECKSUM, &sockopt, sizeof(sockopt));

	c = xsendto(pingsock, packet, DEFDATALEN + sizeof (struct icmp6_hdr),
			   (struct sockaddr *) &pingaddr, sizeof(pingaddr));

	/* listen for replies */
	while (1) {
		struct sockaddr_in6 from;
		socklen_t fromlen = sizeof(from);

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		if (c >= 8) {			/* icmp6_hdr */
			pkt = (struct icmp6_hdr *) packet;
			if (pkt->icmp6_type == ICMP6_ECHO_REPLY)
				break;
		}
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		close(pingsock);
}
#endif

int ping_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ping_main(int argc, char **argv)
{
	len_and_sockaddr *lsa;
#if ENABLE_ATP_PING6
	sa_family_t af = AF_UNSPEC;

	while ((++argv)[0] && argv[0][0] == '-') {
		if (argv[0][1] == '4') {
			af = AF_INET;
			continue;
		}
		if (argv[0][1] == '6') {
			af = AF_INET6;
			continue;
		}
		bb_show_usage();
	}
#else
	argv++;
#endif

	hostname = *argv;
	if (!hostname)
		bb_show_usage();

#if ENABLE_ATP_PING6
	lsa = xhost_and_af2sockaddr(hostname, 0, af);
#else
	lsa = xhost_and_af2sockaddr(hostname, 0, AF_INET);
#endif
	/* Set timer _after_ DNS resolution */
	signal(SIGALRM, noresp);
	alarm(5); /* give the host 5000ms to respond */

#if ENABLE_ATP_PING6
	if (lsa->sa.sa_family == AF_INET6)
		ping6(lsa);
	else
#endif
		ping4(lsa);
	printf("%s is alive!\n", hostname);
	return EXIT_SUCCESS;
}


#else /* FEATURE_FANCY_PING */


/* full(er) version */
#ifdef SUPPORT_ATP_DEVINFO_PING
//此处当开启DEVPING的宏时，使用该OPT_STRING
#define OPT_STRING ("qvc:W:ws:I:t:n:i:4" USE_ATP_PING6("6"))
#else
#define OPT_STRING ("qvc:W:ws:I:4" USE_ATP_PING6("6"))
#endif

enum {
/*  Modify: modify by H00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
    OPT_QUIET = 1 << 0,
    OPT_VERBOSE = 1 << 1,
    OPT_c = 1 << 2,
	OPT_W = 1 << 3,
    OPT_w = 1 << 4,
    OPT_s = 1 << 5,
    OPT_I = 1 << 6,
    OPT_t = 1 << 7,
    OPT_n = 1 << 8,
    OPT_i = 1 << 9, //add i for bind interface;
    OPT_IPV4 = 1 << 10,
    OPT_IPV6 = (1 << 11) * ENABLE_PING6,
#else
	OPT_QUIET = 1 << 0,
	OPT_VERBOSE = 1 << 1,
	OPT_c = 1 << 2,
	OPT_W = 1 << 3,
	OPT_w = 1 << 4,
	OPT_s = 1 << 5,
	OPT_I = 1 << 6,
	OPT_IPV4 = 1 << 7,
	OPT_IPV6 = (1 << 8) * ENABLE_ATP_PING6,
#endif
/*  Modify: modify by H00190880 for CT-COM ping on 2012/12/5*/
};

struct globals {
	int pingsock;
	int if_index;
	char *opt_I;
	len_and_sockaddr *source_lsa;
	unsigned datalen;
	unsigned long ntransmitted, nreceived, nrepeats, pingcount, pingMaxWait;
	uint16_t myid;
	unsigned tmin, tmax; /* in us */
	unsigned long long tsum; /* in us, sum of all times */
	const char *hostname;
	const char *dotted;
	union {
		struct sockaddr sa;
		struct sockaddr_in sin;
#if ENABLE_ATP_PING6
		struct sockaddr_in6 sin6;
#endif
	} pingaddr;
	char rcvd_tbl[MAX_DUP_CHK / 8];
	
/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
    unsigned long pinginterval;
    unsigned int  devpingindex;
#endif
/* END: add by h00190880 for CT-COM ping on 2012/12/5*/

};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define pingsock     (G.pingsock    )
#define if_index     (G.if_index    )
#define source_lsa   (G.source_lsa  )
#define opt_I        (G.opt_I       )
#define datalen      (G.datalen     )
#define ntransmitted (G.ntransmitted)
#define nreceived    (G.nreceived   )
#define nrepeats     (G.nrepeats    )
#define pingcount    (G.pingcount   )
#define pingMaxWait  (G.pingMaxWait )
#define myid         (G.myid        )
#define tmin         (G.tmin        )
#define tmax         (G.tmax        )
#define tsum         (G.tsum        )
#define hostname     (G.hostname    )
#define dotted       (G.dotted      )
#define pingaddr     (G.pingaddr    )
#define rcvd_tbl     (G.rcvd_tbl    )
/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
#define pinginterval (G.pinginterval)
#define devpingindex (G.devpingindex)
#endif
/* END: add by h00190880 for CT-COM ping on 2012/12/5*/

void BUG_ping_globals_too_big(void);
#define INIT_G() do { \
        if (sizeof(G) > COMMON_BUFSIZE) \
                BUG_ping_globals_too_big(); \
	pingsock = -1; \
	tmin = UINT_MAX; \
} while (0)


#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

/**************************************************************************/

#if ENABLE_ATP_PING6  
#ifndef IN6_IS_ADDR_INVALID
#define IN6_IS_ADDR_INVALID(a) ((((uint8_t *) (a))[0] == 0x00) || (((uint8_t *) (a))[0] == 0xFD) || (((uint8_t *) (a))[0] == 0xFF))
#endif
#endif

static int getIfIpAddr(char *name, char *netaddr, int family, int len)
{
    struct ifaddrs *ifc = NULL, *ifc1 = NULL;

    if (NULL == name)
    {
        return(-1);
    }
    if ((AF_INET6 != family) && (AF_INET != family))
    {
        return(-1);
    }
    if (0 != getifaddrs(&ifc))
    {
        return(-1);
    }
                
    ifc1 = ifc;

    for(; NULL != ifc; ifc = (*ifc).ifa_next)
    {
        //printf("%s \n", (*ifc).ifa_name);
        if (0 != strcmp((*ifc).ifa_name, name))
        {
            continue;
        }
        else
        {
            if ((NULL != (*ifc).ifa_addr) && ((AF_INET6 == ((*ifc).ifa_addr)->sa_family) || (AF_INET == ((*ifc).ifa_addr)->sa_family)))
            {
#if ENABLE_ATP_PING6
                if (AF_INET6 == ((*ifc).ifa_addr)->sa_family)
                {
                    if (IN6_IS_ADDR_LINKLOCAL(&(((struct sockaddr_in6*)((*ifc).ifa_addr))->sin6_addr)))
                    {
                        continue;
                    }

                    if(IN6_IS_ADDR_INVALID(&(((struct sockaddr_in6*)((*ifc).ifa_addr))->sin6_addr)))
                    {
                        continue;
                    }
                }
                
                if ((AF_INET6 == family) && (AF_INET6 == ((*ifc).ifa_addr)->sa_family))
                {
                    inet_ntop(AF_INET6, &(((struct sockaddr_in6*)((*ifc).ifa_addr))->sin6_addr), netaddr, 128);
                    break;
                }
                else
#endif
                if ((AF_INET == family) && (AF_INET == ((*ifc).ifa_addr)->sa_family))
                {
                    inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_addr))->sin_addr), netaddr, 128);
                    break;
                }
            } 
        }
    }

    freeifaddrs(ifc1);
    return(0);
}

static void pingstats(int junk ATTRIBUTE_UNUSED)
{
	signal(SIGINT, SIG_IGN);
	
    g_ulSendPacketCount = ntransmitted;
	printf("\n--- %s ping statistics ---\n", hostname);
	printf("%lu packets transmitted, ", ntransmitted);
	printf("%lu packets received, ", nreceived);
	if (nrepeats)
		printf("%lu duplicates, ", nrepeats);
	if (ntransmitted)
		ntransmitted = (ntransmitted - nreceived) * 100 / ntransmitted;
	printf("%lu%% packet loss\n", ntransmitted);
	if (tmin != UINT_MAX) {
		unsigned tavg = tsum / (nreceived + nrepeats);
		printf("round-trip min/avg/max = %u.%03u/%u.%03u/%u.%03u ms\n",
			tmin / 1000, tmin % 1000,
			tavg / 1000, tavg % 1000,
			tmax / 1000, tmax % 1000);
		     }
    if (g_lPingFlag)
    {
      PingToLogWeb(1);
      remove_file(PING_PID_FILE,FILEUTILS_FORCE);
    }
    g_lPingFlag = ATP_PING_WRITE_FILE_FLASE;
	exit(0);
}
#ifdef SUPPORT_ATP_DEVINFO_PING
static void sendping_tail(void (*sp)(int), const void *pkt, int size_pkt)
{
    int sz;
    unsigned long RestCount[3] = {0};
    unsigned int ulRet = VOS_NOK;
    unsigned int ulTotalCnt = 0;
    unsigned int ulIndex = 0;
    char acPingwiteBuf[DEV_PINGCOUNT_FLASH_LEN] = {0};
    int iFd = -1;
    char *acDevPingPid[DIAG_LINE_LENGTH] = {0};
    ATP_MSG_HEADER_ST stMsg;
    ATP_MSG_HEADER_ST *pstResp;

    snprintf(acDevPingPid, sizeof(acDevPingPid), "/var/DevPingPid_%d", devpingindex);
    CLR((uint16_t)ntransmitted % MAX_DUP_CHK);
    ntransmitted++;

    //构造消息体
    memset((void *)(&stMsg), 0, sizeof(stMsg));
    snprintf(stMsg.aucRecvApp, sizeof(stMsg.aucRecvApp), "%s", ATP_CBBID_CMS_NAME);
    snprintf(stMsg.aucSendApp, sizeof(stMsg.aucSendApp), "%s", ATP_CBBID_PING_NAME);
    stMsg.ulDataLen = 0;
    stMsg.ulMsgType = ATP_MSG_DEVPINGCMS_READFLASH;

    /* sizeof(pingaddr) can be larger than real sa size, but I think
     * it doesn't matter */
    sz = xsendto(pingsock, pkt, size_pkt, &pingaddr.sa, sizeof(pingaddr));
    if (sz != size_pkt)
    bb_error_msg_and_die(bb_msg_write_error);

    if (pingcount == 0 || ntransmitted < pingcount)
    {
        /* schedule next in 1s */
        if(0 == pinginterval)
        {
            pinginterval = ATP_PING_INTERVAL;
        }
        if (option_mask32 & OPT_n)
        {
            signal(SIGUSR2, pingstophandle);
            signal(SIGUSR1, pingwandownhandle);
            if (0 != pingcount)
            {
                if(NULL == acPingwiteBuf)
                {
                    return ;
                }
                iFd = PingOpenAndLockFile();
                if ( VOS_ERROR == iFd )
                {
                    return;
                }

                if(pingstopflag)
                {
                    RestCount[devpingindex] = 0;
                }
                else
                {
                    RestCount[devpingindex] = pingcount - ntransmitted - 1;
                }

                snprintf(acPingwiteBuf, sizeof(acPingwiteBuf), "pingcount_1:%d pingcount_2:%d pingcount_3:%d", RestCount[0], RestCount[1], RestCount[2]);
                //发送消息给CMS将RestCount写入flash
                ATP_MSG_SendMsgData(ATP_CBBID_CMS_NAME, ATP_MSG_DEVPINGCMS_WRITEFLASH, acPingwiteBuf, sizeof(acPingwiteBuf));

                //START :给CMS发送消息，获取FLASH中存放的剩余PING次数
                ulRet = ATP_MSG_SendAndRecv(&stMsg, &pstResp, 1*1000);
                if (VOS_OK != ulRet)
                {
                    /*解锁关闭文件*/
                    PingLockFile(iFd, F_UNLCK);
                    printf("ping read flash error!!\n");
                    return;
                }

                if (0 != strlen((char *)(pstResp + 1)))
                {
                    snprintf(&RestCount, sizeof(RestCount), "%d", (char *)(pstResp + 1));
                }

                free((void *)pstResp);
                //END :给CMS发送消息，获取FLASH中存放的剩余PING次数

                /*解锁关闭文件*/
                PingLockFile(iFd, F_UNLCK);
            }

            if(pingstopflag)
            {
                pingstopflag = 0;
                printf("file:%s;line:%d;acDevPingPid:%s\n",__FILE__,__LINE__,acDevPingPid);
                remove(acDevPingPid);
                exit(1);
            }
            else if(pingWandownflag)
            {
                pingWandownflag = 0;
                printf("file:%s;line:%d;acDevPingPid:%s\n",__FILE__,__LINE__,acDevPingPid);
                remove(acDevPingPid);
                exit(1);
            }
        }
    }
    else 
    { /* done, wait for the last ping to come back */
      /* todo, don't necessarily need to wait so long... */
        if(option_mask32 & OPT_n)
        {
            printf("ping num %d end!!\n",devpingindex);
            remove_file(acDevPingPid,FILEUTILS_FORCE);
        }
        signal(SIGALRM, pingstats);
        alarm(MAXWAIT);
    }
}
#endif

#if 0
static void sendping_tail(void (*sp)(int), const void *pkt, int size_pkt)
{
	int sz;

	CLR((uint16_t)ntransmitted % MAX_DUP_CHK);
	ntransmitted++;

	/* sizeof(pingaddr) can be larger than real sa size, but I think
	 * it doesn't matter */
	sz = xsendto(pingsock, pkt, size_pkt, &pingaddr.sa, sizeof(pingaddr));
	if (sz != size_pkt)
		bb_error_msg_and_die(bb_msg_write_error);

	signal(SIGALRM, sp);
	if (pingcount == 0 || ntransmitted < pingcount) { /* schedule next in 1s */
		alarm(PINGINTERVAL);
	} else { /* done, wait for the last ping to come back */
		/* todo, don't necessarily need to wait so long... */
		signal(SIGALRM, pingstats);
		alarm(pingMaxWait/*MAXWAIT*/);
	}
}
#endif

static void sendping4(int junk ATTRIBUTE_UNUSED)
{
    int sz;
	/* +4 reserves a place for timestamp, which may end up sitting
	 * *after* packet. Saves one if() */
	struct icmp *pkt = alloca(datalen + ICMP_MINLEN + 4);

	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_code = 0;
	pkt->icmp_cksum = 0;
	pkt->icmp_seq = htons(ntransmitted); /* don't ++ here, it can be a macro */
	pkt->icmp_id = myid;

	/* We don't do hton, because we will read it back on the same machine */
	/*if (datalen >= 4)*/
		*(uint32_t*)&pkt->icmp_dun = monotonic_us();

	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, datalen + ICMP_MINLEN);

//devping需要在函数sendping_tail中进行发送ICMP报文，用宏与平台的代码隔开
#ifdef SUPPORT_ATP_DEVINFO_PING
    sendping_tail(sendping4, pkt, datalen + ICMP_MINLEN);
#else
    CLR((uint16_t)ntransmitted % MAX_DUP_CHK);
	ntransmitted++;
#ifdef SUPPORT_ATP_TELMEX
    sz = sendto(pingsock, pkt, datalen + ICMP_MINLEN, 0, &pingaddr.sa, sizeof(pingaddr));	
    if ((sz != datalen + ICMP_MINLEN) && (-1 != sz))
#else
    sz = xsendto(pingsock, pkt, datalen + ICMP_MINLEN, &pingaddr.sa, sizeof(pingaddr));
    if (sz != datalen + ICMP_MINLEN)
#endif
		bb_error_msg_and_die(bb_msg_write_error);
#endif
}
#if ENABLE_ATP_PING6
static void sendping6(int junk ATTRIBUTE_UNUSED)
{
    int sz;
	struct icmp6_hdr *pkt = alloca(datalen + sizeof(struct icmp6_hdr) + 4);

	pkt->icmp6_type = ICMP6_ECHO_REQUEST;
	pkt->icmp6_code = 0;
	pkt->icmp6_cksum = 0;
	pkt->icmp6_seq = htons(ntransmitted); /* don't ++ here, it can be a macro */
	pkt->icmp6_id = myid;

	/*if (datalen >= 4)*/
	*(uint32_t*)(&pkt->icmp6_data8[4]) = monotonic_us();

	/* sizeof(pingaddr) can be larger than real sa size, but I think
	 * it doesn't matter */
#ifdef SUPPORT_ATP_DEVINFO_PING
    sendping_tail(sendping6, pkt, datalen + sizeof(struct icmp6_hdr));
#else
    CLR((uint16_t)ntransmitted % MAX_DUP_CHK);
	ntransmitted++;
#ifdef SUPPORT_ATP_TELMEX
    sz = sendto(pingsock, pkt, datalen + sizeof(struct icmp6_hdr), 0, &pingaddr.sa, sizeof(pingaddr));	
    if ((sz != datalen + sizeof(struct icmp6_hdr)) && (-1 != sz))
#else
	sz = xsendto(pingsock, pkt, datalen + sizeof(struct icmp6_hdr), &pingaddr.sa, sizeof(pingaddr));
	if (sz != datalen + sizeof(struct icmp6_hdr))
#endif
		bb_error_msg_and_die(bb_msg_write_error);
#endif
}
#endif

static const char *icmp_type_name(int id)
{
	switch (id) {
	case ICMP_ECHOREPLY:      return "Echo Reply";
	case ICMP_DEST_UNREACH:   return "Destination Unreachable";
	case ICMP_SOURCE_QUENCH:  return "Source Quench";
	case ICMP_REDIRECT:       return "Redirect (change route)";
	case ICMP_ECHO:           return "Echo Request";
	case ICMP_TIME_EXCEEDED:  return "Time Exceeded";
	case ICMP_PARAMETERPROB:  return "Parameter Problem";
	case ICMP_TIMESTAMP:      return "Timestamp Request";
	case ICMP_TIMESTAMPREPLY: return "Timestamp Reply";
	case ICMP_INFO_REQUEST:   return "Information Request";
	case ICMP_INFO_REPLY:     return "Information Reply";
	case ICMP_ADDRESS:        return "Address Mask Request";
	case ICMP_ADDRESSREPLY:   return "Address Mask Reply";
	default:                  return "unknown ICMP type";
	}
}
#if ENABLE_ATP_PING6
/* RFC3542 changed some definitions from RFC2292 for no good reason, whee!
 * the newer 3542 uses a MLD_ prefix where as 2292 uses ICMP6_ prefix */
#ifndef MLD_LISTENER_QUERY
# define MLD_LISTENER_QUERY ICMP6_MEMBERSHIP_QUERY
#endif
#ifndef MLD_LISTENER_REPORT
# define MLD_LISTENER_REPORT ICMP6_MEMBERSHIP_REPORT
#endif
#ifndef MLD_LISTENER_REDUCTION
# define MLD_LISTENER_REDUCTION ICMP6_MEMBERSHIP_REDUCTION
#endif
static const char *icmp6_type_name(int id)
{
	switch (id) {
	case ICMP6_DST_UNREACH:      return "Destination Unreachable";
	case ICMP6_PACKET_TOO_BIG:   return "Packet too big";
	case ICMP6_TIME_EXCEEDED:    return "Time Exceeded";
	case ICMP6_PARAM_PROB:       return "Parameter Problem";
	case ICMP6_ECHO_REPLY:       return "Echo Reply";
	case ICMP6_ECHO_REQUEST:     return "Echo Request";
	case MLD_LISTENER_QUERY:     return "Listener Query";
	case MLD_LISTENER_REPORT:    return "Listener Report";
	case MLD_LISTENER_REDUCTION: return "Listener Reduction";
	default:                     return "unknown ICMP type";
	}
}
#endif

static void unpack_tail(int sz, uint32_t *tp,
		const char *from_str,
		uint16_t recv_seq, int ttl)
{
	const char *dupmsg = " (DUP!)";
	unsigned triptime = triptime; /* for gcc */

	++nreceived;

	if (tp) {
		/* (int32_t) cast is for hypothetical 64-bit unsigned */
		/* (doesn't hurt 32-bit real-world anyway) */
		triptime = (int32_t) ((uint32_t)monotonic_us() - *tp);
		tsum += triptime;
		if (triptime < tmin)
			tmin = triptime;
		if (triptime > tmax)
			tmax = triptime;
	}

	if (TST(recv_seq % MAX_DUP_CHK)) {
		++nrepeats;
		--nreceived;
	} else {
		SET(recv_seq % MAX_DUP_CHK);
    dupmsg += 7;
		}       

    if (g_lPingFlag) 
    {
        PingToLogWeb(0);
        usleep(1);
    }

	if (option_mask32 & OPT_QUIET)
		return;
	
	printf("%d bytes from %s: seq=%u ttl=%d", sz,
		from_str, recv_seq, ttl);
	if (tp)
		printf(" time=%u.%03u ms", triptime / 1000, triptime % 1000);
	puts(dupmsg);
	fflush(stdout);
	}
static int unpack4(char *buf, int sz, struct sockaddr_in *from)
{
	struct icmp *icmppkt;
	struct iphdr *iphdr;
	int hlen;

	/* discard if too short */
	if (sz < (datalen + ICMP_MINLEN))
		return -1; 

	/* check IP header */
	iphdr = (struct iphdr *) buf;
	hlen = iphdr->ihl << 2;
	sz -= hlen;
	icmppkt = (struct icmp *) (buf + hlen);
	if (icmppkt->icmp_id != myid)
		return -1;				/* not our ping */

	if (icmppkt->icmp_type == ICMP_ECHOREPLY) {
		uint16_t recv_seq = ntohs(icmppkt->icmp_seq);
		uint32_t *tp = NULL;

		if (sz >= ICMP_MINLEN + sizeof(uint32_t))
			tp = (uint32_t *) icmppkt->icmp_data;
		unpack_tail(sz, tp,
			inet_ntoa(*(struct in_addr *) &from->sin_addr.s_addr),
			recv_seq, iphdr->ttl);
	} else if (icmppkt->icmp_type != ICMP_ECHO) {
		bb_error_msg("warning: got ICMP %d (%s)",
				icmppkt->icmp_type,
				icmp_type_name(icmppkt->icmp_type));
        return -1;
	}
       else if (icmppkt->icmp_type == ICMP_ECHO)
        {
            return -1;
        }
    return 0;
}
#if ENABLE_ATP_PING6
static int unpack6(char *packet, int sz, struct sockaddr_in6 *from, int hoplimit)
{
	struct icmp6_hdr *icmppkt;
	char buf[INET6_ADDRSTRLEN];

	/* discard if too short */
	if (sz < (datalen + sizeof(struct icmp6_hdr)))
		return -1;

	icmppkt = (struct icmp6_hdr *) packet;
	if (icmppkt->icmp6_id != myid)
		return -1;				/* not our ping */

	if (icmppkt->icmp6_type == ICMP6_ECHO_REPLY) {
		uint16_t recv_seq = ntohs(icmppkt->icmp6_seq);
		uint32_t *tp = NULL;

		if (sz >= sizeof(struct icmp6_hdr) + sizeof(uint32_t))
			tp = (uint32_t *) &icmppkt->icmp6_data8[4];
		unpack_tail(sz, tp,
			inet_ntop(AF_INET6, &pingaddr.sin6.sin6_addr,
					buf, sizeof(buf)),
			recv_seq, hoplimit);
	} else if (icmppkt->icmp6_type != ICMP6_ECHO_REQUEST) {
		bb_error_msg("warning: got ICMP %d (%s)",
				icmppkt->icmp6_type,
				icmp6_type_name(icmppkt->icmp6_type));
	}
    return 0;
}
#endif

#if 0
#define PING_DEBUG(format, args...)       {struct timeval cur_time1; ATP_UTIL_GetSysTime(&cur_time1);printf("[tv_sec:%ld tv_usec:%ld] FILE: %s FUNC: %s LINE: %d: ", cur_time1.tv_sec, cur_time1.tv_usec, __FILE__,__func__, __LINE__);printf(format, ##args);}
#else
#define PING_DEBUG(x...)
#endif
extern unsigned int ATP_UTIL_GetSysTime(struct timeval* pstTimeVal);

static void ping_compensate_time(struct timeval *last_send_time, struct timeval *timeout)
{
    struct timeval      now;
    struct timeval      diff;

    ATP_UTIL_GetSysTime(&now);

    // Get diff
    if (now.tv_usec < last_send_time->tv_usec)
    {
        diff.tv_usec = 1000 * 1000 - (last_send_time->tv_usec - now.tv_usec);
        diff.tv_sec  = (now.tv_sec - 1) - last_send_time->tv_sec;
    }
    else
    {
        diff.tv_usec = now.tv_usec - last_send_time->tv_usec;
        diff.tv_sec  = now.tv_sec - last_send_time->tv_sec;
    }

    PING_DEBUG("Time elapsed: [tv_sec:%ld tv_usec:%ld].\n", diff.tv_sec, diff.tv_usec);

    // Compensate
    if (diff.tv_usec < timeout->tv_usec)
    {
        if (timeout->tv_sec < diff.tv_sec)
        {
            timeout->tv_sec = 0;
            timeout->tv_usec = 0;
        }
        else
        {
            timeout->tv_usec -= diff.tv_usec;
            timeout->tv_sec  -= diff.tv_sec;
        }
    }
    else
    {
        timeout->tv_sec -= 1;
        if (timeout->tv_sec < diff.tv_sec)
        {
            timeout->tv_sec = 0;
            timeout->tv_usec = 0;
        }
        else
        {
            timeout->tv_usec = 1000 * 1000 - (diff.tv_usec - timeout->tv_usec);
            timeout->tv_sec  -= diff.tv_sec;
        }
    }
}

static void ping4(len_and_sockaddr *lsa)
{
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	int sockopt;
    int idx = 0;
    int err_resp = 0;
    struct timeval lastSendTime;

	pingsock = create_icmp_socket();
	pingaddr.sin = lsa->sin;
	if (source_lsa) {
		if (setsockopt(pingsock, IPPROTO_IP, IP_MULTICAST_IF,
				&source_lsa->sa, source_lsa->len))
			bb_error_msg_and_die("can't set multicast source interface");
		xbind(pingsock, &source_lsa->sa, source_lsa->len);
	}
	if (opt_I)
		setsockopt(pingsock, SOL_SOCKET, SO_BINDTODEVICE, opt_I, strlen(opt_I) + 1);

#ifdef SUPPORT_ATP_DEVINFO_PING
	if (1 == g_iInterfaceFlag)
    {
		g_iInterfaceFlag = 0;
		if (bind(pingsock, (struct sockaddr *) &pinterfaceipsin, sizeof(pinterfaceipsin)) < 0 ) 
        {
	        return;
        }
    }
#endif

	/* enable broadcast pings */
	setsockopt_broadcast(pingsock);

	/* set recv buf for broadcast pings */
	sockopt = 48 * 1024; /* explain why 48k? */
	setsockopt(pingsock, SOL_SOCKET, SO_RCVBUF, &sockopt, sizeof(sockopt));

	signal(SIGINT, pingstats);

	/* start the ping's going ... */
	//sendping4(0);

	/* listen for replies */
	while (idx < pingcount) {
		struct sockaddr_in from;
		socklen_t fromlen = (socklen_t) sizeof(from);
		int c;
        int ret;
        fd_set readFds;
        struct timeval stTimeVal;

        FD_ZERO(&readFds);
        FD_SET(pingsock, &readFds);

        stTimeVal.tv_sec  = pingMaxWait / 1000;
        stTimeVal.tv_usec = (pingMaxWait % 1000) * 1000;
        if (0 != err_resp) {
            ping_compensate_time(&lastSendTime, &stTimeVal);
        } else {
            ATP_UTIL_GetSysTime(&lastSendTime);
            PING_DEBUG("Send the %dth ping packet now [tv_sec:%ld tv_usec:%ld] ...\n", idx, lastSendTime.tv_sec, lastSendTime.tv_usec);
            sendping4(0);
            err_resp = 0;
       }

        PING_DEBUG("Wait timeout for the %dth packet [tv_sec:%ld tv_usec:%ld] ...\n", idx, stTimeVal.tv_sec, stTimeVal.tv_usec);
        ret = select((pingsock + 1), &readFds, NULL, NULL, &stTimeVal);
        if (0 == ret) {
            printf("Request timed out\n");
            fflush(stdout);
            idx++;
            err_resp = 0;
            continue;
        } else if (ret < 0) {
            if (errno != EINTR)
				bb_perror_msg("select");
            printf("Request timed out\n");
            fflush(stdout);
            continue;
        }

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
            PING_DEBUG("Recv ping response failed with %d errno %d.\n", c, errno);
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
                    printf("Request timed out\n");
                    fflush(stdout);
			continue;
		}
        err_resp = unpack4(packet, c, &from);
        PING_DEBUG("Recvd ping response with length %d unpack result %d.\n", c, err_resp);
        if (0 == err_resp) {
            idx++;
		}	
	}
}
#if ENABLE_ATP_PING6
extern int BUG_bad_offsetof_icmp6_cksum(void);
static void ping6(len_and_sockaddr *lsa)
{
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	int sockopt;
	struct msghdr msg;
	struct sockaddr_in6 from;
	struct iovec iov;
    int idx = 0;
    int err_resp = 0;
    struct timeval lastSendTime;
	char control_buf[CMSG_SPACE(36)];

	pingsock = create_icmp6_socket();
	pingaddr.sin6 = lsa->sin6;
    
        if (if_index)
                pingaddr.sin6.sin6_scope_id = if_index;
	/* untested whether "-I addr" really works for IPv6: */
	if (source_lsa)
	{
        if (if_index)
           ((struct sockaddr_in6 *)(&(source_lsa->sa)))->sin6_scope_id = if_index;
		xbind(pingsock, &source_lsa->sa, source_lsa->len);
	}
#if 0
	if (opt_I)
		setsockopt(pingsock, SOL_SOCKET, SO_BINDTODEVICE, opt_I, strlen(opt_I) + 1);
#endif

#ifdef ICMP6_FILTER
	{
		struct icmp6_filter filt;
		if (!(option_mask32 & OPT_VERBOSE)) {
			ICMP6_FILTER_SETBLOCKALL(&filt);
			ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filt);
		} else {
			ICMP6_FILTER_SETPASSALL(&filt);
		}
		if (setsockopt(pingsock, IPPROTO_ICMPV6, ICMP6_FILTER, &filt,
					   sizeof(filt)) < 0)
			bb_error_msg_and_die("setsockopt(ICMP6_FILTER)");
	}
#endif /*ICMP6_FILTER*/

	/* enable broadcast pings */
	setsockopt_broadcast(pingsock);

	/* set recv buf for broadcast pings */
	sockopt = 48 * 1024; /* explain why 48k? */
	setsockopt(pingsock, SOL_SOCKET, SO_RCVBUF, &sockopt, sizeof(sockopt));

	sockopt = offsetof(struct icmp6_hdr, icmp6_cksum);
	if (offsetof(struct icmp6_hdr, icmp6_cksum) != 2)
		BUG_bad_offsetof_icmp6_cksum();
	setsockopt(pingsock, SOL_RAW, IPV6_CHECKSUM, &sockopt, sizeof(sockopt));

	/* request ttl info to be returned in ancillary data */
	setsockopt(pingsock, SOL_IPV6, IPV6_HOPLIMIT, &const_int_1, sizeof(const_int_1));

	if (if_index)
		pingaddr.sin6.sin6_scope_id = if_index;

	signal(SIGINT, pingstats);

	/* start the ping's going ... */
	//sendping6(0);

	/* listen for replies */
	msg.msg_name = &from;
	msg.msg_namelen = sizeof(from);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control_buf;
	iov.iov_base = packet;
	iov.iov_len = sizeof(packet);

	while (idx < pingcount) {
		int c;
		struct cmsghdr *mp;
		int hoplimit = -1;
        int ret;
        fd_set readFds;
        struct timeval stTimeVal;

		msg.msg_controllen = sizeof(control_buf);

        FD_ZERO(&readFds);
        FD_SET(pingsock, &readFds);

        stTimeVal.tv_sec  = pingMaxWait / 1000;
        stTimeVal.tv_usec = (pingMaxWait % 1000) * 1000;

        if (0 != err_resp) {
            ping_compensate_time(&lastSendTime, &stTimeVal);
        } else {
            ATP_UTIL_GetSysTime(&lastSendTime);
            PING_DEBUG("Send the %dth ping packet now [tv_sec:%ld tv_usec:%ld] ...\n", idx, lastSendTime.tv_sec, lastSendTime.tv_usec);
            sendping6(0);
            err_resp = 0;
        }

        PING_DEBUG("Wait timeout for the %dth packet [tv_sec:%ld tv_usec:%ld] ...\n", idx, stTimeVal.tv_sec, stTimeVal.tv_usec);

        ret = select((pingsock + 1), &readFds, NULL, NULL, &stTimeVal);
        if (0 == ret) {
            printf("Request timed out\n");
            fflush(stdout);
            idx++;
            err_resp = 0;
            continue;
        } else if (ret < 0) {
            if (errno != EINTR)
				bb_perror_msg("select");
            printf("Request timed out\n");
            fflush(stdout);
            continue;
        }

		c = recvmsg(pingsock, &msg, 0);
		if (c < 0) {
            PING_DEBUG("Recv ping response failed with %d errno %d.\n", c, errno);
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		for (mp = CMSG_FIRSTHDR(&msg); mp; mp = CMSG_NXTHDR(&msg, mp)) {
			if (mp->cmsg_level == SOL_IPV6
			 && mp->cmsg_type == IPV6_HOPLIMIT
			 /* don't check len - we trust the kernel: */
			 /* && mp->cmsg_len >= CMSG_LEN(sizeof(int)) */
			) {
				hoplimit = *(int*)CMSG_DATA(mp);
			}
		}
		err_resp = unpack6(packet, c, &from, hoplimit);
        PING_DEBUG("Recvd ping response with length %d unpack result %d.\n", c, err_resp);
        if (0 == err_resp) {
            idx++;
        }
	}
}
#endif

static void ping(len_and_sockaddr *lsa)
{
    printf("PING %s (%s)", hostname, dotted);
    if (source_lsa) {
        printf(" from %s",
            xmalloc_sockaddr2dotted_noport(&source_lsa->sa));
    }
    printf(": %d data bytes\n", datalen);
    fflush(stdout);

#if ENABLE_ATP_PING6
	if (lsa->sa.sa_family == AF_INET6)
		ping6(lsa);
	else
#endif
		ping4(lsa);
}
len_and_sockaddr* atp_xhost_and_af2sockaddr(const char *host, int family, char *interfacename);

len_and_sockaddr* atp_xhost_and_af2sockaddr(const char *host, int family, char *interfacename )
{
    len_and_sockaddr *lsa;
    struct hostent* h = NULL;
    char netaddr[128] = {0};

    lsa = xzalloc(sizeof(len_and_sockaddr));

    if (NULL == lsa)
    {
        return NULL;
    }
#if ENABLE_ATP_PING6
    if (inet_pton(AF_INET6, host, &lsa->sin6.sin6_addr) > 0)    
    {
        lsa->sin6.sin6_family = AF_INET6;
        lsa->len = sizeof(struct sockaddr_in6);
    }
    else 
#endif
        if (inet_pton(AF_INET, host, &lsa->sin.sin_addr) > 0)    
    {
        lsa->sin.sin_family = AF_INET;
        lsa->len = sizeof(struct sockaddr_in);
    }
    else
    {
        if ((AF_INET == family) || (AF_INET6 == family))
        {
            getIfIpAddr(interfacename, netaddr, family, 128);
        }
        else
        {
            getIfIpAddr(interfacename, netaddr, AF_INET6, 128);
            if ('\0' == netaddr[0])
            {
                getIfIpAddr(interfacename, netaddr, AF_INET, 128);
            }
        }
        
        if ((NULL == interfacename) || ('\0' == netaddr[0]))
        {
            if (AF_INET == family)
            {
                h = ATP_GPL_Gethostbyname(host, AF_INET, NULL);
            }
            else if (AF_INET6 == family)
            {
                h = ATP_GPL_Gethostbyname(host, AF_INET6, NULL);
            }
            else
            {
                h = ATP_GPL_Gethostbyname(host, AF_INET6, NULL);
                if (h == NULL) {
                    h = ATP_GPL_Gethostbyname(host, AF_INET, NULL);
                }
            }
        }
        else
        {
            if (AF_INET == family)
            {
                h = ATP_GPL_Gethostbyname(host, AF_INET, netaddr);
            }
            else if (AF_INET6 == family)
            {
                h = ATP_GPL_Gethostbyname(host, AF_INET6, netaddr);
            }
            else
            {
                getIfIpAddr(interfacename, netaddr, AF_INET6, 128);
                h = ATP_GPL_Gethostbyname(host, AF_INET6, netaddr);
                if (h == NULL) {
                    getIfIpAddr(interfacename, netaddr, AF_INET, 128);
                    h = ATP_GPL_Gethostbyname(host, AF_INET, netaddr);
                }
            }
        }
        if (h == NULL)
        {
            return NULL;
        }
#if ENABLE_ATP_PING6
        if (h->h_addrtype == AF_INET6)
        {
            memcpy(lsa->sin6.sin6_addr.s6_addr, (char*)*h->h_addr_list, 16);
            lsa->sin6.sin6_family = AF_INET6;
            lsa->len = sizeof(struct sockaddr_in6);
        }
        else
#endif            
        {
            lsa->sin.sin_addr.s_addr = *(int*)*h->h_addr_list;
            lsa->sin.sin_family = AF_INET;
            lsa->len = sizeof(struct sockaddr_in);
        }
    }

    return lsa;
}


int ping_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ping_main(int argc, char **argv)
{
	len_and_sockaddr *lsa;
    
	USE_ATP_PING6(sa_family_t af = AF_UNSPEC;)
	/* start of add for ping times 50 by m00132836 2011-04-25 */
	/* Start of modify for DTS2010061900370,add -i for IPPing diagnosist d00167548 2010-08-23*/	
	char *opt_c, *opt_s, *opt_W;
#ifdef SUPPORT_ATP_DEVINFO_PING
    /* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
    char *opt_t;
    char *opt_n;
    char *opt_i;
	char acinterface[64] = {0};
	memset(acinterface,0,sizeof(acinterface));
	int sock = 0;
    /* END: add by h00190880 for CT-COM ping on 2012/12/5*/
#endif

	/* end of add for ping times 50 by m00132836 2011-04-25 */
    char netaddr[128] = {0};
#ifdef SUPPORT_ATP_DEVINFO_PING
    // Msg init
    if (VOS_OK != ATP_MSG_Init(ATP_CBBID_PING_NAME))
    {
        printf("PING msg init failed: .\n");
        return -1;
    }
#endif

	INIT_G();

	datalen = DEFDATALEN;

	/*默认ping的次数为 3*/
	pingcount = ATP_PING_COUNT;
    pingMaxWait = MAXWAIT;

#ifdef SUPPORT_ATP_DEVINFO_PING
    /* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
    pinginterval = ATP_PING_INTERVAL;
    /* END: add by h00190880 for CT-COM ping on 2012/12/5*/
#endif

	/* exactly one argument needed, -w and -q don't mix */
	opt_complementary = "=1:q--v:v--q";
#ifdef SUPPORT_ATP_DEVINFO_PING
    //devping需要的参数更多，因此需要从新写一套
    getopt32(argv, OPT_STRING, &opt_c, &opt_W, &opt_s, &opt_I, &opt_t, &opt_n, &opt_i);
#else
	/* start of add for ping times 50 by m00132836 2011-04-25 */
	/* Start of modify for DTS2010061900370,add -i for IPPing diagnosist d00167548 2010-08-23*/
	getopt32(argv, OPT_STRING, &opt_c, &opt_W, &opt_s, &opt_I);
	/* End of modify for DTS2010061900370,add -i for IPPing diagnosist d00167548 2010-08-23*/	
	/* end of add for ping times 50 by m00132836 2011-04-25 */
#endif

	if (option_mask32 & OPT_c)
		pingcount = xatoul(opt_c);// -c

    if (option_mask32 & OPT_W){
		pingMaxWait = xatoul(opt_W); // -W
    }

#ifndef SUPPORT_ATP_DEVINFO_PING
    if (option_mask32 & OPT_s)
        datalen = xatou16(opt_s); // -s
#endif

	myid = (uint16_t)getpid();

    /*增加ping -w数据的处理*/
    if (option_mask32 & OPT_w)
    {
        if ((pingPid = fopen (PING_PID_FILE, "w")) != NULL) 
        {
              fprintf(pingPid,"%d\n", myid);
              (void)fclose(pingPid);
        }
        /*Start of ATP 2013-07-11 by z00192937: telmex 页面诊断需使用，请勿删除*/
        printf("%s \n", argv[optind]);
        /*End of ATP 2013-07-11 by z00192937*/
        g_lPingFlag = ATP_PING_WRITE_FILE_TRUE;
    }

    hostname = argv[optind];

#ifdef SUPPORT_ATP_DEVINFO_PING
    /* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
    if (option_mask32 & OPT_t)
    {
        pinginterval = xatoul(opt_t);// -t
    }

    if (option_mask32 & OPT_n)
    {
        devpingindex = xatoul(opt_n);// -n        
    }

    if(option_mask32 & OPT_n)
    {
        FILE *pDevpingPid = NULL;
        char *acDevPingPid[DIAG_LINE_LENGTH] = {0};
        snprintf(acDevPingPid, sizeof(acDevPingPid), "/var/DevPingPid_%d", devpingindex);
        if ((pDevpingPid = fopen (acDevPingPid, "w")) != NULL) 
        {
              fprintf(pDevpingPid,"%d\n", myid);
              (void)fclose(pDevpingPid);
        }
    }

    if (option_mask32 & OPT_i)
    {
        g_iInterfaceFlag = 0;
        strcpy(acinterface, opt_i);

        sock = socket( AF_INET, SOCK_STREAM, 0 );
        if (get_if_addr(sock, acinterface, &pinterfaceipsin) != 0)
        {
            printf("此接口尚未获取到IP地址");
            return 0;
        }
        g_iInterfaceFlag = 1;
    }
    /* END: add by h00190880 for CT-COM ping on 2012/12/5*/
#endif

#if ENABLE_ATP_PING6
	if (option_mask32 & OPT_IPV4)
		af = AF_INET;
	if (option_mask32 & OPT_IPV6)
		af = AF_INET6;
    if ((AF_INET == af) || (AF_INET6 == af))
    {
        if (option_mask32 & OPT_I) {
            lsa = atp_xhost_and_af2sockaddr(hostname, af, opt_I);
        }
        else{
            lsa = atp_xhost_and_af2sockaddr(hostname, af, NULL);
        }
    }
    else
    {
        if (option_mask32 & OPT_I) {
            lsa = atp_xhost_and_af2sockaddr(hostname, af, opt_I);
        }
        else{
            lsa = atp_xhost_and_af2sockaddr(hostname, 0, NULL);
        }
    }
    if (lsa)
    {
        if (lsa->sa.sa_family == AF_INET6)
        {
            inet_ntop(AF_INET6, &lsa->sin6.sin6_addr, g_acHost, INET6_ADDRSTRLEN);
        }
        else
        {
            inet_ntop(AF_INET, &lsa->sin.sin_addr, g_acHost, INET_ADDRSTRLEN);
        }
    }
#else
	lsa = xhost_and_af2sockaddr(hostname, 0, AF_INET);
    if (lsa)
    {
        strncpy(g_acHost, inet_ntoa(lsa->sin.sin_addr),16);
    }
#endif

    if (option_mask32 & OPT_I) { // -I
        if(lsa)
        {
            if (0 != getIfIpAddr(opt_I, netaddr, lsa->sa.sa_family, 128))
            {
                printf("Interface not get ip address yet");
                return 0;
            }
            source_lsa = xdotted2sockaddr(netaddr, 0);
        }
        #if 0
        if_index = if_nametoindex(opt_I);
        if (!if_index) {
            /* TODO: I'm not sure it takes IPv6 unless in [XX:XX..] format */
            source_lsa = xdotted2sockaddr(opt_I, 0);
            opt_I = NULL; /* don't try to bind to device later */
        }

  #if ENABLE_ATP_PING6
        else
        {
            if (lsa)
            {
                if ((AF_INET6 == lsa->sa.sa_family) && (NULL != opt_I))
                {
                    if (0 != getIfIpAddr(opt_I, netaddr, lsa->sa.sa_family, 128))
                    {
                        printf("Interface not get ipv6 address yet");
                        return 0;
                    }
                    source_lsa = xdotted2sockaddr(netaddr, 0);
                }
            }
        }
    #endif
    #endif
    
    }

    if (lsa)
    {
    	if (source_lsa && source_lsa->sa.sa_family != lsa->sa.sa_family)
    		/* leaking it here... */
    		source_lsa = NULL;

    	dotted = xmalloc_sockaddr2dotted_noport(&lsa->sa);
    	ping(lsa);
    	pingstats(0);
    }
    else
    {
        printf("ping: bad address '%s' \n", hostname);
        fflush(stdout);
        bb_error_msg("bad address '%s'", hostname);
    }
    
    if (g_lPingFlag) 
    {
        remove_file(PING_PID_FILE,FILEUTILS_FORCE);
    }
    g_lPingFlag = ATP_PING_WRITE_FILE_FLASE;
    
	return EXIT_SUCCESS;
}

/* ADD: add by h00190880 for CT-COM ping on 2012/12/5*/
#ifdef SUPPORT_ATP_DEVINFO_PING
static int get_if_addr(int sock, char *name, struct sockaddr_in *sin)
{
	struct ifreq ifr;
	if (0 == name)
	{
		return -1;
	}
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, name);

	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
    // perror("ioctl(SIOCGIFADDR)");
	    memset(sin, 0, sizeof(struct sockaddr_in));
	    return -1;
	}

	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl(SIOCGIFADDR)");
	    memset(sin, 0, sizeof(struct sockaddr_in));
	    return -1;
	}

	if(ifr.ifr_addr.sa_family == AF_INET)
	{
	    memcpy(sin, &(ifr.ifr_addr), sizeof(struct sockaddr_in));
	    return 0;
	}
	else
	{
		memset(sin, 0, sizeof(struct sockaddr_in));
		return -1;
	}
	return -1;
}

static void pingstophandle(void)
{
   pingstopflag = 1; 
}

static void pingwandownhandle(void)
{
    pingWandownflag = 1;
}

static int PingLockFile(int iFd,int iMode)
{
    struct flock stLock;

    if (VOS_ERROR == iFd)
    {
        return VOS_ERROR;
    }

    stLock.l_type = (short)iMode;
    stLock.l_whence = SEEK_SET;
    stLock.l_start = 0;
    stLock.l_len = 0;
    stLock.l_pid = myid;

    if (0 != fcntl(iFd,F_SETLKW,&stLock))
    {
        return VOS_ERROR;
    }

    return VOS_OK;
}

static int PingOpenAndLockFile()
{
    int iFd = VOS_ERROR;

    iFd = open(Ping_LOCK_FILE, O_RDWR | O_CREAT);

    if (VOS_ERROR == iFd)
    {
        return VOS_ERROR;
    }
    if ( VOS_OK != PingLockFile(iFd,F_WRLCK) )
    {
        return VOS_ERROR;
    }

    return iFd;
}
#endif
/* END: add by h00190880 for CT-COM ping on 2012/12/5*/

static void PingToLogWeb(int finish)
{

  pfPingStats = fopen (PING_RESULT_FILE, "w");
  if (pfPingStats == NULL)
  {
    return;
  }
  rewind(pfPingStats);
  if ((finish) && (ntransmitted == 0))
  {
    fprintf(pfPingStats,"%d\n",finish);
  }
  else
  {
    fprintf(pfPingStats,"%d\n",finish);
  }

  /* IP, sent, receive, lost, min, max, average */
  if( nreceived )
  {
    unsigned tavg = tsum / (nreceived + nrepeats);
    fprintf(pfPingStats,"Host = %s\n", g_acHost);
    fprintf(pfPingStats,"Sent = %ld\n", g_ulSendPacketCount );
    fprintf(pfPingStats,"Receive = %ld\n", nreceived );
    fprintf(pfPingStats,"Lost = %ld\n",    ntransmitted);
    fprintf(pfPingStats,"Min = %u.%03u ms\n", (tmin / 1000), (tmin % 1000));
    fprintf(pfPingStats,"Max = %u.%03u ms\n",  (tmax / 1000), (tmax % 1000));
    fprintf(pfPingStats,"Average = %u.%03u ms\n",(tavg / 1000),(tavg % 1000));
  }
  else
  {
    fprintf(pfPingStats,"Host = %s\n", g_acHost );
    fprintf(pfPingStats,"Sent = %ld\n", g_ulSendPacketCount );
    fprintf(pfPingStats,"Receive = %ld\n", nreceived );
    fprintf(pfPingStats,"Lost = %ld\n",    ntransmitted);
    fprintf(pfPingStats,"Min = %u.%03u ms\n", (tmin / 1000), (tmin % 1000));
    fprintf(pfPingStats,"Max = %u.%03u ms\n", (tmin / 1000), (tmin % 1000));
    fprintf(pfPingStats,"Average = %u.%03u ms\n",(tmin / 1000), (tmin % 1000));
  }
  fclose(pfPingStats);
}

#endif /* FEATURE_FANCY_PING */


#if ENABLE_ATP_PING6
int ping6_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ping6_main(int argc, char **argv)
{
	argv[0] = (char*)"-6";
	return ping_main(argc + 1, argv - 1);
}
#endif

/* from ping6.c:
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. <BSD Advertising Clause omitted per the July 22, 1999 licensing change
 *		ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change>
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
