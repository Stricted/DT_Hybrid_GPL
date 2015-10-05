/*
 * IPv4 support for nf_conntrack.
 *
 * 23 Mar 2004: Yasuyuki Kozakai @ USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- move L3 protocol dependent part from include/linux/netfilter_ipv4/
 *	  ip_conntarck.h
 */

#ifndef _NF_CONNTRACK_IPV4_H
#define _NF_CONNTRACK_IPV4_H

typedef unsigned int (*hi_nf_conntrack_confirm_hook)(unsigned int hooknum,
                                                   struct sk_buff *skb,
                                                   const struct net_device *in,
                                                   const struct net_device *out,
                                                   int (*okfn)(struct sk_buff *),
                                                   int conntrack_ret);
                            
extern unsigned int hi_nf_conntrack_register_confirm_hook(hi_nf_conntrack_confirm_hook hook);
extern unsigned int hi_nf_conntrack_unregister_confirm_hook(void);

extern struct nf_conntrack_l3proto nf_conntrack_l3proto_ipv4;

extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_icmp;

extern int nf_conntrack_ipv4_compat_init(void);
extern void nf_conntrack_ipv4_compat_fini(void);

extern void need_ipv4_conntrack(void);

#endif /*_NF_CONNTRACK_IPV4_H*/
