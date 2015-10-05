/* vi: set sw=4 ts=4: */
/*
 * Mini nslookup implementation for busybox
 *
 * Copyright (C) 1999,2000 by Lineo, inc. and John Beppu
 * Copyright (C) 1999,2000,2001 by John Beppu <beppu@codepoet.org>
 *
 * Correct default name server display and explicit name server option
 * added by Ben Zeckel <bzeckel@hmc.edu> June 2001
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include <resolv.h>
#include "libbb.h"
#include <features.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/nameser.h>
#include <sys/utsname.h>
#include <sys/un.h>
#include <time.h>

/* Keep in sync with getopt32 call! */
#define OPT_INTERFACEIP    (1<<0)    /* i */
#define OPT_DNS_SERVER     (1<<1)    /* s */
#define OPT_TIMEOUT           (1<<2)    /* t */
#define OPT_REP_NUM           (1<<3)    /* n */

#define OK    ((int) 1)
#define NOK   ((int) 0)

#define MAX_RETRIES 1
#define MAX_ALIASES 5

#define DIAG_NSLOOKUP_PID_FILE "/var/nslookupPid"
#define DIAG_NSLOOKUP_RESULT_FILE  "/var/nslookupResult"

/* 1:ip + 1:full + MAX_ALIASES:aliases + 1:NULL */
#define     ALIAS_DIM       (2 + MAX_ALIASES + 1)

#undef DEBUG_NSLOOKUP
//#define DEBUG_NSLOOKUP

#ifdef DEBUG_NSLOOKUP
#define NSLOOKUPDPRINTF(X,args...) fprintf(stderr, X, ##args)
#else
#define NSLOOKUPDPRINTF(X,args...)
#endif

struct NSLookupDiagResult 
{
    unsigned ulResptime;
    unsigned ulAa;
};

static struct NSLookupDiagResult g_diagresult;
static FILE *pfNslookupStats = NULL;

/* Structs */
struct resolv_header {
    int id;
    int qr,opcode,aa,tc,rd,ra,rcode;
    int qdcount;
    int ancount;
    int nscount;
    int arcount;
};

struct resolv_question {
    char * dotted;
    int qtype;
    int qclass;
};

struct resolv_answer {
    char * dotted;
    int atype;
    int aclass;
    int ttl;
    int rdlength;
    unsigned char * rdata;
    int rdoffset;
    char* buf;
    size_t buflen;
    size_t add_count;
};

static void print_delta_ms(unsigned t1p, unsigned t2p)
{
    unsigned tt = t2p - t1p;
    g_diagresult.ulResptime = tt/1000;
}

static int handy_encode_header(struct resolv_header *h, unsigned char *dest, int maxlen)
{
    if (maxlen < HFIXEDSZ)
        return -1;

    dest[0] = (h->id & 0xff00) >> 8;
    dest[1] = (h->id & 0x00ff) >> 0;
    dest[2] = (h->qr ? 0x80 : 0) | ((h->opcode & 0x0f) << 3) |(h->aa ? 0x04 : 0) |(h->tc ? 0x02 : 0) |(h->rd ? 0x01 : 0);
    dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0f);
    dest[4] = (h->qdcount & 0xff00) >> 8;
    dest[5] = (h->qdcount & 0x00ff) >> 0;
    dest[6] = (h->ancount & 0xff00) >> 8;
    dest[7] = (h->ancount & 0x00ff) >> 0;
    dest[8] = (h->nscount & 0xff00) >> 8;
    dest[9] = (h->nscount & 0x00ff) >> 0;
    dest[10] = (h->arcount & 0xff00) >> 8;
    dest[11] = (h->arcount & 0x00ff) >> 0;

    return HFIXEDSZ;
}

static int handy_decode_header(unsigned char *data, struct resolv_header *h)
{
    h->id = (data[0] << 8) | data[1];
    h->qr = (data[2] & 0x80) ? 1 : 0;
    h->opcode = (data[2] >> 3) & 0x0f;
    h->aa = (data[2] & 0x04) ? 1 : 0;
    h->tc = (data[2] & 0x02) ? 1 : 0;
    h->rd = (data[2] & 0x01) ? 1 : 0;
    h->ra = (data[3] & 0x80) ? 1 : 0;
    h->rcode = data[3] & 0x0f;
    h->qdcount = (data[4] << 8) | data[5];
    h->ancount = (data[6] << 8) | data[7];
    h->nscount = (data[8] << 8) | data[9];
    h->arcount = (data[10] << 8) | data[11];

    return HFIXEDSZ;
}

/* Encode a dotted string into nameserver transport-level encoding.
   This routine is fairly dumb, and doesn't attempt to compress
   the data */
static int handy_encode_dotted(const char *dotted, unsigned char *dest, int maxlen)
{
    int used = 0;

    while (dotted && *dotted) {
        char *c = strchr(dotted, '.');
        int l = c ? c - dotted : strlen(dotted);

        if (l >= (maxlen - used - 1))
            return -1;

        dest[used++] = l;
        memcpy(dest + used, dotted, l);
        used += l;

        if (c)
            dotted = c + 1;
        else
            break;
    }

    if (maxlen < 1)
        return -1;

    dest[used++] = 0;

    return used;
}

/* Decode a dotted string from nameserver transport-level encoding.
   This routine understands compressed data. */
static int handy_decode_dotted(const unsigned char *data, int offset,
                    char *dest, int maxlen)
{
    int l;
    int measure = 1;
    int total = 0;
    int used = 0;

    if (!data)
        return -1;

    while ((l=data[offset++])) {
        if (measure)
            total++;
        if ((l & 0xc0) == (0xc0)) {
            if (measure)
                total++;
            /* compressed item, redirect */
            offset = ((l & 0x3f) << 8) | data[offset];
            measure = 0;
            continue;
        }

        if ((used + l + 1) >= maxlen)
            return -1;

        memcpy(dest + used, data + offset, l);
        offset += l;
        used += l;
        if (measure)
            total += l;

        if (data[offset] != 0)
            dest[used++] = '.';
        else
            dest[used++] = '\0';
    }

    /* The null byte must be counted too */
    if (measure) {
        total++;
    }

    NSLOOKUPDPRINTF("Total decode len = %d\n", total);

    return total;
}

static int handy_length_dotted(const unsigned char *data, int offset)
{
    int orig_offset = offset;
    int l;

    if (!data)
        return -1;

    while ((l = data[offset++])) {

        if ((l & 0xc0) == (0xc0)) {
            offset++;
            break;
        }

        offset += l;
    }

    return offset - orig_offset;
}

static int handy_encode_question(struct resolv_question *q,
                      unsigned char *dest, int maxlen)
{
    int i;

    i = handy_encode_dotted(q->dotted, dest, maxlen);
    if (i < 0)
        return i;

    dest += i;
    maxlen -= i;

    if (maxlen < 4)
        return -1;

    dest[0] = (q->qtype & 0xff00) >> 8;
    dest[1] = (q->qtype & 0x00ff) >> 0;
    dest[2] = (q->qclass & 0xff00) >> 8;
    dest[3] = (q->qclass & 0x00ff) >> 0;

    return i + 4;
}

static int handy_length_question(unsigned char *message, int offset)
{
    int i;

    i = handy_length_dotted(message, offset);
    if (i < 0)
        return i;

    return i + 4;
}

static int handy_decode_answer(unsigned char *message, int offset,
                    struct resolv_answer *a)
{
    char temp[256];
    int i;

    i = handy_decode_dotted(message, offset, temp, sizeof(temp));
    if (i < 0)
        return i;

    message += offset + i;

    a->dotted = strdup(temp);
    a->atype = (message[0] << 8) | message[1];
    message += 2;
    a->aclass = (message[0] << 8) | message[1];
    message += 2;
    a->ttl = (message[0] << 24) |
        (message[1] << 16) | (message[2] << 8) | (message[3] << 0);
    message += 4;
    a->rdlength = (message[0] << 8) | message[1];
    message += 2;
    a->rdata = message;
    a->rdoffset = offset + i + RRFIXEDSZ;

    NSLOOKUPDPRINTF("i=%d,rdlength=%d\n", i, a->rdlength);

    return i + RRFIXEDSZ + a->rdlength;
}

/*Start of Protocol 2014-3-28 16:12 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/
#define NON_BLACKLIST_MARK 0x1000
/*End of Protocol 2014-3-28 16:12 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/


static int handy_dns_lookup(const char *name, int type,
                 unsigned char **outpacket, struct resolv_answer *a,
                 char* wanaddr, char *pDnsIp, int lTimeout)
{
    int i, j, len, fd, pos, rc;
    struct timeval tv;
    fd_set fds;
    struct resolv_header h;
    struct resolv_question q;
    struct resolv_answer ma;
    struct sockaddr_in  bindAddr;
    int first_answer = 1;
    int retries = 0;
    unsigned char * packet = malloc(PACKETSZ);
    char *dns, *lookup = malloc(MAXDNAME);
    int local_id = -1;
    struct sockaddr_in sa;

    unsigned t1;
    unsigned t2;

    /*Start of Protocol 2014-3-28 16:12 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/
    int need_mark = 0;
    unsigned int ulMark = NON_BLACKLIST_MARK;
    /*End of Protocol 2014-3-28 16:12 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/

    fd = -1;

    if (!packet || !lookup)
        goto fail;

    NSLOOKUPDPRINTF("Looking up type %d answer for '%s'\n", type, name);
    local_id = time(NULL);

    ++local_id;
    local_id &= 0xffff;

    while (retries < MAX_RETRIES) {
        if (fd != -1)
            close(fd);

        memset(packet, 0, PACKETSZ);

        memset(&h, 0, sizeof(h));

        h.id = local_id;

        if (0xffffffff == inet_addr(pDnsIp))
        {
            dns = "127.0.0.1";            
        }
        else
        {
            dns = pDnsIp;
        }

        /*Start of Protocol 2014-4-27 16:4 for DTS2014042207110 by t00189036*/
        if (NULL == wanaddr)
        {
            /*Start of Protocol 2014-3-28 16:14 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/
            need_mark = 1;
            /*End of Protocol 2014-3-28 16:14 DTS2014032806341指定的业务可以被telekomlist控制 for by t00189036*/
        }
        /*End of Protocol 2014-4-27 16:4 for by t00189036*/

        NSLOOKUPDPRINTF("got server %s\n", dns);

        h.qdcount = 1;
        h.rd = 1;

        NSLOOKUPDPRINTF("encoding header %d\n", h.rd);

        i = handy_encode_header(&h, packet, PACKETSZ);
        if (i < 0)
            goto fail;

        strncpy(lookup,name,MAXDNAME);
        NSLOOKUPDPRINTF("lookup name: %s\n", lookup);
        q.dotted = (char *)lookup;
        q.qtype = type;
        q.qclass = 1; /* CLASS_IN C_IN*/

        j = handy_encode_question(&q, packet+i, PACKETSZ-i);
        if (j < 0)
            goto fail;

        len = i + j;

        NSLOOKUPDPRINTF("On try %d, sending query to port %d of machine %s\n",
                retries+1, NAMESERVER_PORT, dns);

        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd < 0) 
        {
            retries++;
            continue;
        }

        /*Start of Protocol 2014-3-28 16:14 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/
        if (need_mark)
        {
            (void)setsockopt(fd, SOL_SOCKET, SO_MARK, &ulMark, sizeof(ulMark));
        }
        /*End of Protocol 2014-3-28 16:14 for DTS2014032806341指定的业务可以被telekomlist控制 by t00189036*/

        /* Connect to the UDP socket so that asyncronous errors are returned */
        sa.sin_family = AF_INET;
        sa.sin_port = htons(NAMESERVER_PORT);

        memset((void *)(&bindAddr), 0, sizeof(bindAddr));
        bindAddr.sin_family = AF_INET;

        if ((NULL == wanaddr) || (inet_pton(AF_INET, wanaddr, &bindAddr.sin_addr) <= 0))
        {
            bindAddr.sin_addr.s_addr = 0;
        }
        if (bind(fd, (struct sockaddr *)(&bindAddr), sizeof(bindAddr)) < 0)
        {
            NSLOOKUPDPRINTF("Bind addr %s failed!\n", inet_ntoa(bindAddr.sin_addr));
            goto fail;
        }
        sa.sin_addr.s_addr = inet_addr(dns);

        rc = connect(fd, (struct sockaddr *) &sa, sizeof(sa));

        if (rc < 0) {
            if (errno == ENETUNREACH) {
                /* routing error, presume not transient */
                //goto fail;
                retries++;
            } else {
                /* retry */
                retries++;
            }
            continue;
        }

        NSLOOKUPDPRINTF("Transmitting packet of length %d, id=%d, qr=%d\n",
                len, h.id, h.qr);

        t1 = monotonic_us();
        send(fd, packet, len, 0);

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = lTimeout/1000;
        tv.tv_usec = (lTimeout%1000*1000);
        if (select(fd + 1, &fds, NULL, NULL, &tv) <= 0) {
            NSLOOKUPDPRINTF("Timeout\n");

            /* timed out, so retry send and receive, to next nameserver on queue */
            //goto fail;
            goto again;
        }

        t2 = monotonic_us();
        print_delta_ms(t1, t2);

        len = recv(fd, packet, 512, 0);
        if (len < HFIXEDSZ) {
            /* too short ! */
            goto again;
        }

        handy_decode_header(packet, &h);

        NSLOOKUPDPRINTF("id = %d, qr = %d\n", h.id, h.qr);

        if ((h.id != local_id) || (!h.qr)) {
            /* unsolicited */
            goto again;
        }


        NSLOOKUPDPRINTF("Got response %s\n", "(i think)!");
        NSLOOKUPDPRINTF("qrcount=%d,ancount=%d,nscount=%d,arcount=%d\n",
                h.qdcount, h.ancount, h.nscount, h.arcount);
        NSLOOKUPDPRINTF("opcode=%d,aa=%d,tc=%d,rd=%d,ra=%d,rcode=%d\n",
                h.opcode, h.aa, h.tc, h.rd, h.ra, h.rcode);

       g_diagresult.ulAa = h.aa;

        if ((h.rcode) || (h.ancount < 1)) {
            /* negative result, not present */
            goto again;
        }

        pos = HFIXEDSZ;

        for (j = 0; j < h.qdcount; j++) {
            NSLOOKUPDPRINTF("Skipping question %d at %d\n", j, pos);
            i = handy_length_question(packet, pos);
            NSLOOKUPDPRINTF("Length of question %d is %d\n", j, i);
            if (i < 0)
                goto again;
            pos += i;
        }
        NSLOOKUPDPRINTF("Decoding answer at pos %d\n", pos);

        first_answer = 1;
        for (j=0;j<h.ancount;j++,pos += i)
        {
            i = handy_decode_answer(packet, pos, &ma);

            if (i<0) {
                NSLOOKUPDPRINTF("failed decode %d\n", i);
                goto again;
            }

            if ( first_answer )
            {
                ma.buf = a->buf;
                ma.buflen = a->buflen;
                ma.add_count = a->add_count;
                memcpy(a, &ma, sizeof(ma));

                if (a->atype != 24 && (0 == a->buf || (type != 1 && type != 28)))
                {
                    break;
                }
                if (a->atype != type)
                {
                    free(a->dotted);
                    a->dotted = NULL;
                    continue;
                }
                a->add_count = h.ancount - j - 1;
                if ((a->rdlength + sizeof(struct in_addr*)) * a->add_count > a->buflen)
                {
                    break;
                }
                a->add_count = 0;
                first_answer = 0;

                if ((28 == type) &&(5 == ma.atype) && (1 == h.ancount))
                {
                    NSLOOKUPDPRINTF("Answer only CNAME V6\n");
                    goto fail;
                }                    
            }
            else
            {
                free(ma.dotted);
                if (ma.atype != type)
                {
                    continue;
                }
                if (a->rdlength != ma.rdlength)
                {
                    free(a->dotted);
                    a->dotted = NULL;
                    NSLOOKUPDPRINTF("Answer address len(%u) differs from original(%u)\n",
                            ma.rdlength, a->rdlength);
                    goto again;
                }
                memcpy(a->buf + (a->add_count * ma.rdlength), ma.rdata, ma.rdlength);
                ++a->add_count;

                if (a->add_count >= 7)
                {
                    NSLOOKUPDPRINTF("8 records at most.\n");
                    break;
                }
            }
        }

        NSLOOKUPDPRINTF("Answer name = |%s|\n", a->dotted);
        NSLOOKUPDPRINTF("Answer type = |%d|\n", a->atype);

        close(fd);

        if (outpacket)
            *outpacket = packet;
        else
            free(packet);
        free(lookup);

        return (len);               /* success! */

    again:
        retries++;
    }

 fail:
    if (fd != -1)
        close(fd);
    if (lookup)
        free(lookup);
    if (packet)
        free(packet);
    return -1;
}

static int gethostbyname_handy_r(const char * name, struct hostent * result_buf,
                    char * buf, size_t buflen, struct hostent ** result,
                    char* wanaddr, char *pDnsIp, int lTimeout)
{
    struct in_addr *in;
    struct in_addr **addr_list;
    char **alias;
    unsigned char *packet;
    struct resolv_answer a;
    int i;

    *result=NULL;
    if (!name)
        return EINVAL;

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen < sizeof(char *)*(ALIAS_DIM))
        return ERANGE;
    alias=(char **)buf;
    buf+=sizeof(char **)*(ALIAS_DIM);
    buflen-=sizeof(char **)*(ALIAS_DIM);

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    alias[0] = buf;
    alias[1] = NULL;

    /* First check if this is already an address */
    if (inet_aton(name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        result_buf->h_aliases = alias;
        *result=result_buf;
        return NETDB_SUCCESS;
    }

    for (;;) {
        a.buf = buf;
        a.buflen = buflen;
        a.add_count = 0;

        i = handy_dns_lookup(name, 1, &packet, &a, wanaddr, pDnsIp, lTimeout);

        if (i < 0) {
            NSLOOKUPDPRINTF("handy_dns_lookup\n");
            return TRY_AGAIN;
        }

        if ((a.rdlength + sizeof(struct in_addr*)) * a.add_count + 256 > buflen)
        {
            free(a.dotted);
            a.dotted = NULL;
            free(packet);
            NSLOOKUPDPRINTF("buffer too small for all addresses\n");
            return ERANGE;
        }
        else if(a.add_count > 0)
        {
            memmove(buf - sizeof(struct in_addr*)*2, buf, a.add_count * a.rdlength);
            addr_list = (struct in_addr**)(buf + a.add_count * a.rdlength);
            addr_list[0] = in;
            for (i = a.add_count-1; i>=0; --i)
                addr_list[i+1] = (struct in_addr*)(buf - sizeof(struct in_addr*)*2 + a.rdlength * i);
            addr_list[a.add_count + 1] = 0;
            buflen -= (((char*)&(addr_list[a.add_count + 2])) - buf);
            buf = (char*)&addr_list[a.add_count + 2];
        }

        if (NULL != a.dotted)
        {
            strncpy(buf, a.dotted, buflen);
            free(a.dotted);
            a.dotted = NULL;
        }

        if (a.atype == 1) { /* ADDRESS T_A=1 */
            memcpy(in, a.rdata, sizeof(*in));
            result_buf->h_name = buf;
            result_buf->h_addrtype = AF_INET;
            result_buf->h_length = sizeof(*in);
            result_buf->h_addr_list = (char **) addr_list;
            result_buf->h_aliases = alias; /* TODO: generate the full list */
            free(packet);
            break;
        } else {
            free(packet);
            return TRY_AGAIN;
        }
    }

    *result=result_buf;
    return NETDB_SUCCESS;
}

static struct hostent *NSLOOKUP_Gethostbyname(const char *name, char* bindaddr, char *pDnsIp, int lTimeout)
{
    static struct hostent h;
    static char buf[sizeof(struct in6_addr) +
                    sizeof(struct in6_addr *)*2 +
                    sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
    struct hostent *hp;

    gethostbyname_handy_r(name, &h, buf, sizeof(buf), &hp, bindaddr, pDnsIp, lTimeout);

    return hp;
}

/******************************************************************************
  函数名称：getDnsServerIpAddr
  功能描述：获取NSLookupDiagnostics.DNSServer的IP地址
  调用函数：
  输入参数：pHost: NSLookupDiagnostics.DNSServer的值
            pInterfaceIp: 如果指定了NSLookupDiagnostics.Interface，则为改interface的IP地址；如果未指定，则为NULL
  输出参数：pIpAddr: 保存解析出来的IP地址
  返 回 值：成功OK，失败NOK
  其它说明：
******************************************************************************/
static int getDnsServerIpAddr(const char *pHost, char *pIpAddr, int lLen, char *pInterfaceIp)
{
    struct hostent *hp;
    struct in_addr stTmpAddr = {0};

    hp = ATP_GPL_Gethostbyname(pHost, AF_INET, pInterfaceIp);

    if (NULL == hp)
    {
        return NOK;
    }
    if (hp->h_addrtype != AF_INET || hp->h_length != 4)
    {
        return NOK;
    }

    stTmpAddr.s_addr = *(uint32_t *)hp->h_addr;
    snprintf(pIpAddr, lLen, "%s", inet_ntoa(stTmpAddr));

    return OK;
}

static int getNSLookupHostIpAddr(const char *pHost, char *pInterfaceIp, char *pDnsIp, int lTimeout)
{
    struct hostent *hp;
    char **p;
    char **temp;

    hp = NSLOOKUP_Gethostbyname(pHost, pInterfaceIp, pDnsIp, lTimeout);

    if (NULL == hp)
    {
        return NOK;
    }
    if (hp->h_addrtype != AF_INET || hp->h_length != 4)
    {
        return NOK;
    }

    //printf("HostNameReturned: %s\n", hp->h_name);
    fprintf(pfNslookupStats, "HostNameReturned: %s\n", hp->h_name);
    //printf("IPAddresses: ");
    fprintf(pfNslookupStats, "IPAddresses: ");

    for (p = hp->h_addr_list; *p != NULL; ++p)
    {
        //printf("%s", inet_ntoa(*(struct in_addr *) (*p)));
        fprintf(pfNslookupStats, "%s", inet_ntoa(*(struct in_addr *) (*p)));
        temp = p;
        if (NULL != *(++temp))
        {
            //printf(",");
            fprintf(pfNslookupStats, ",");
        }
    }

    //printf("\n");
    fprintf(pfNslookupStats, "\n");

    return OK;
}

/*Start:z00082249 for DNSServer解析不出来也需要有一条result实例20130814*/
static void WriteDnsServerFailInfo(int lRepnum)
{
	  int i = 0;
	  
    pfNslookupStats = fopen(DIAG_NSLOOKUP_RESULT_FILE, "w");
    if (NULL == pfNslookupStats)
    {
        bb_perror_msg_and_die("Open /var/nslookupResult fail!");
    }

    fprintf(pfNslookupStats, "NSLookup Number Of Repetitions: %d\n", lRepnum);
    //将结果存入文件/var/nslookupResult
    for (i=0; i<lRepnum; i++)
    {
        memset(&g_diagresult, 0, sizeof(g_diagresult));
        fprintf(pfNslookupStats, "NSLookupDiagnostics %d\n", (i+1));
        fprintf(pfNslookupStats, "HostNameReturned:  \n");
        fprintf(pfNslookupStats, "IPAddresses:  \n");
        fprintf(pfNslookupStats, "Status: Error_DNSServerNotAvailable\n");
        fprintf(pfNslookupStats, "AnswerType: None\n");
        fprintf(pfNslookupStats, "DNSServerIP:  \n");
        fprintf(pfNslookupStats, "ResponseTime: 0\n");
    }

    fclose(pfNslookupStats);
    remove_file(DIAG_NSLOOKUP_PID_FILE, FILEUTILS_FORCE);
    return;
}
/*End:z00082249 for DNSServer解析不出来也需要有一条result实例20130814*/

int dnsdiag_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int dnsdiag_main(int argc, char **argv)
{
    char *pInterfaceIp = NULL;
    char *pDNSServer = NULL;
    char *pTimeout = NULL;
    char *pNumberOfRepetitions = NULL;
    char *pHostName = NULL;

    int lTimeout = 3000;
    int lRepnum = 1;
    char acDnsIp[16] = {0};
    int i = 0;
    FILE *pFile = NULL;

    unsigned ulOp;

    if (NULL != (pFile = fopen(DIAG_NSLOOKUP_PID_FILE, "w"))) 
    {
          fprintf(pFile,"%d\n", getpid());
          fclose(pFile);
    }

    ulOp = getopt32(argv, "i:s:t:n:", &pInterfaceIp, &pDNSServer, &pTimeout, &pNumberOfRepetitions);

    if (ulOp & OPT_TIMEOUT)
    {
        lTimeout = atoi(pTimeout);
    }

    if (ulOp & OPT_REP_NUM)
    {
        lRepnum = atoi(pNumberOfRepetitions);
    }

    /* Process hostname */
    switch (argc - optind) 
    {
        case 1:
        pHostName = argv[optind];
        break;

        default:
        bb_perror_msg_and_die("Usage: nslookup -s DNSServer [-i interfaceIp] [-t Timeout] [-n NumberOfRepetitions] HostName");
    }

    if (!(ulOp & OPT_INTERFACEIP))
    {
        pInterfaceIp = NULL;
    }

    if (ulOp & OPT_DNS_SERVER)
    {
        //如果指定了NSLookupDiagnostics.DNSServer，要查询到它的IP地址
        /*Start of Protocol 2014-4-27 16:4 for DTS2014042207110 by t00189036*/
        if (NOK == getDnsServerIpAddr(pDNSServer, acDnsIp, sizeof(acDnsIp), NULL))
        /*End of Protocol 2014-4-27 16:4 for by t00189036*/
        {
		      /*Start:z00082249 for DNSServer解析不出来也需要有一条result实例20130814*/
        	  WriteDnsServerFailInfo(lRepnum);
        	  return 0;
			  /*End:z00082249 for DNSServer解析不出来也需要有一条result实例20130814*/
        }
    }
    else
    {
        // 按照目前的实现，必须指定DNSServer
        bb_perror_msg_and_die("Usage: nslookup -s DNSServer [-i interfaceIp] [-t Timeout] [-n NumberOfRepetitions] HostName");
    }

    pfNslookupStats = fopen(DIAG_NSLOOKUP_RESULT_FILE, "w");
    if (NULL == pfNslookupStats)
    {
        bb_perror_msg_and_die("Open /var/nslookupResult fail!");
    }

    //printf("NSLookup Number Of Repetitions: %d\n", lRepnum);
    fprintf(pfNslookupStats, "NSLookup Number Of Repetitions: %d\n", lRepnum);
    //将结果存入文件/var/nslookupResult
    for (i=0; i<lRepnum; i++)
    {
        memset(&g_diagresult, 0, sizeof(g_diagresult));
        //printf("NSLookupDiagnostics %d\n", (i+1));
        fprintf(pfNslookupStats, "NSLookupDiagnostics %d\n", (i+1));
        if (OK == getNSLookupHostIpAddr(pHostName, pInterfaceIp, acDnsIp, lTimeout))
        {
            //printf("Status: Success\n");
            fprintf(pfNslookupStats, "Status: Success\n");
            //printf("AnswerType: %s\n", (g_diagresult.ulAa ? "Authoritative" : "NonAuthoritative"));
            fprintf(pfNslookupStats, "AnswerType: %s\n", (g_diagresult.ulAa ? "Authoritative" : "NonAuthoritative"));
        }
        else
        {
            //printf("HostNameReturned: NULL\n");
            fprintf(pfNslookupStats, "HostNameReturned: NULL\n");
            //printf("IPAddresses: NULL\n");
            fprintf(pfNslookupStats, "IPAddresses: NULL\n");
            //printf("Status: %s\n", ((0 == g_diagresult.ulResptime) ? "Error_Timeout" : "Error_HostNameNotResolved"));
            fprintf(pfNslookupStats, "Status: %s\n", ((0 == g_diagresult.ulResptime) ? "Error_Timeout" : "Error_HostNameNotResolved"));
            //printf("AnswerType: None\n");
            fprintf(pfNslookupStats, "AnswerType: None\n");
        }
        
        //printf("DNSServerIP: %s\n", acDnsIp);
        fprintf(pfNslookupStats, "DNSServerIP: %s\n", acDnsIp);
        //printf("ResponseTime: %u\n", g_diagresult.ulResptime);
        fprintf(pfNslookupStats, "ResponseTime: %u\n", g_diagresult.ulResptime);
    }

    fclose(pfNslookupStats);
    remove_file(DIAG_NSLOOKUP_PID_FILE, FILEUTILS_FORCE);
    
    return 0;
}
