#ifndef _IP_NAT_DNSCOMM_H_
#define  _IP_NAT_DNSCOMM_H_

#include <linux/netfilter_ipv4.h>

#define DNS_PKT_TYPE_QUERY      0
#define DNS_PKT_TYPE_RESPONSE  1

#define DNSFLAG_QR(flag)        (((flag)>>15)&0x1)
#define DNSFLAG_OPCODE(flag)    (((flag)>>11)&0xF)
#define DNSFLAG_AA(flag)        (((flag)>>10)&0x1)
#define DNSFLAG_TC(flag)        (((flag)>>9)&0x1)
#define DNSFLAG_RD(flag)        (((flag)>>8)&0x1)
#define DNSFLAG_RA(flag)        (((flag)>>7)&0x1)
#define DNSFLAG_Z(flag)         (((flag)>>4)&0x7)
#define DNSFLAG_RCODE(flag)     ((flag)&0xF)

struct dnshdr
{
    unsigned short id;
    unsigned short flag;
    unsigned short qd_conut;
    unsigned short an_count;
    unsigned short ns_count;
    unsigned short ar_count;
};

extern unsigned int dns_do_match(struct sk_buff *skb);
#endif// _IP_NAT_DNSCOMM_H_
