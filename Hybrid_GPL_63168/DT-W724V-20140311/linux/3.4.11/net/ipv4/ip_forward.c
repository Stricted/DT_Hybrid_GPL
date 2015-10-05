/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		The IP forwarding functionality.
 *
 * Authors:	see ip.c
 *
 * Fixes:
 *		Many		:	Split from ip.c , see ip_input.c for
 *					history.
 *		Dave Gregorich	:	NULL ip_rt_put fix for multicast
 *					routing.
 *		Jos Vos		:	Add call_out_firewall before sending,
 *					use output device for accounting.
 *		Jos Vos		:	Call forward firewall after routing
 *					(always use output device).
 *		Mike McLagan	:	Routing by source
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/netfilter_ipv4.h>
#include <net/checksum.h>
#include <linux/route.h>
#include <net/route.h>
#include <net/xfrm.h>
#ifdef CONFIG_ATP_HYBRID
#include <net/netfilter/nf_conntrack.h>
#endif

#ifdef CONFIG_DT_QOS
#include "qosdtkrnl.h"
#endif
/* do not send exceed icmp when recv the dhcp from HAAP DTS2013101509929 */ 
#ifdef CONFIG_ATP_HYBRID
#define DHCPC_UDP_PORT        68    
#endif

static int ip_forward_finish(struct sk_buff *skb)
{
	struct ip_options * opt	= &(IPCB(skb)->opt);
#ifdef CONFIG_ATP_HYBRID
    struct nf_conn *nfct;
#endif

	IP_INC_STATS_BH(dev_net(skb_dst(skb)->dev), IPSTATS_MIB_OUTFORWDATAGRAMS);

	if (unlikely(opt->optlen))
		ip_forward_options(skb);

    
#ifdef CONFIG_ATP_HYBRID
        /*Start of Protocol 2013-12-26 10:37 for bypass/tunnel֮���л�ʱ���������������ϵ���· by t00189036*/
        nfct = (struct nf_conn *)skb->nfct;
        /*ֻ���������Ӹ��٣�û�д���ƽ���л���mark��������lan��ı���*/
        if (nfct && !(nfct->mark & ATP_CONNTRACK_FLOW_MARK_MASK) && (skb->mark & PPP_TRIGER_MARK))
        {
            /*���ڷ���PPP��������ı��ķֱ���ϲ�ͬ��Mark*/
            if (IS_PPP_WAN_DEVICE(skb_dst(skb)->dev->name))
            {
                nfct->mark |= ATP_CONNTRACK_BYPASS_MARK;
            }
            else if (IS_GRE_DEV(skb_dst(skb)->dev->name))
            {
                nfct->mark |= ATP_CONNTRACK_TUNNEL_MARK;
            }
        }
        /*End of Protocol 2013-12-26 10:37 for bypass/tunnel֮���л�ʱ���������������ϵ���· by t00189036*/
#endif

	return dst_output(skb);
}

int ip_forward(struct sk_buff *skb)
{
	struct iphdr *iph;	/* Our header */
	struct rtable *rt;	/* Route we use */
#ifdef CONFIG_ATP_HYBRID     
    struct udphdr *uh;	/* do not send exceed icmp when recv the dhcp from HAAP DTS2013101509929 */ 
#endif
	struct ip_options * opt	= &(IPCB(skb)->opt);

	if (skb_warn_if_lro(skb))
		goto drop;

	if (!xfrm4_policy_check(NULL, XFRM_POLICY_FWD, skb))
		goto drop;

	if (IPCB(skb)->opt.router_alert && ip_call_ra_chain(skb))
		return NET_RX_SUCCESS;

	if (skb->pkt_type != PACKET_HOST)
		goto drop;

	skb_forward_csum(skb);

	/*
	 *	According to the RFC, we must first decrease the TTL field. If
	 *	that reaches zero, we must reply an ICMP control message telling
	 *	that the packet's lifetime expired.
	 */
	if (ip_hdr(skb)->ttl <= 1)
		goto too_many_hops;

	if (!xfrm4_route_forward(skb))
		goto drop;

	rt = skb_rtable(skb);

	if (opt->is_strictroute && opt->nexthop != rt->rt_gateway)
		goto sr_failed;

	if (unlikely(skb->len > dst_mtu(&rt->dst) && !skb_is_gso(skb) &&
		     (ip_hdr(skb)->frag_off & htons(IP_DF))) && !skb->local_df) {
		IP_INC_STATS(dev_net(rt->dst.dev), IPSTATS_MIB_FRAGFAILS);
		icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED,
			  htonl(dst_mtu(&rt->dst)));
		goto drop;
	}

	/* We are about to mangle packet. Copy it! */
	if (skb_cow(skb, LL_RESERVED_SPACE(rt->dst.dev)+rt->dst.header_len))
		goto drop;
	iph = ip_hdr(skb);

	/* Decrease ttl after skb cow done */
	ip_decrease_ttl(iph);

#ifdef CONFIG_DT_QOS
	DtQosMarkToTosWithIPCheck(skb);
#endif

	/*
	 *	We now generate an ICMP HOST REDIRECT giving the route
	 *	we calculated.
	 */
	if (rt->rt_flags&RTCF_DOREDIRECT && !opt->srr && !skb_sec_path(skb))
		ip_rt_send_redirect(skb);

	skb->priority = rt_tos2priority(iph->tos);

#if defined(CONFIG_BCM_KF_WANDEV)
	/* Never forward a packet from a WAN intf to the other WAN intf */
	if( (skb->dev) && (rt->dst.dev) && 
		((skb->dev->priv_flags & rt->dst.dev->priv_flags) & IFF_WANDEV) )
		goto drop;
#endif

	return NF_HOOK(NFPROTO_IPV4, NF_INET_FORWARD, skb, skb->dev,
		       rt->dst.dev, ip_forward_finish);

sr_failed:
	/*
	 *	Strict routing permits no gatewaying
	 */
	 icmp_send(skb, ICMP_DEST_UNREACH, ICMP_SR_FAILED, 0);
	 goto drop;

too_many_hops:
#ifdef CONFIG_ATP_HYBRID 
    /* start do not send exceed icmp when recv the dhcp from HAAP DTS2013101509929 */ 
    if (IPPROTO_UDP == ip_hdr(skb)->protocol)
    {
        __skb_pull(skb, ip_hdrlen(skb));
	    skb_reset_transport_header(skb);   
        uh = udp_hdr(skb);
        if (DHCPC_UDP_PORT == ntohs(uh->dest))
        {
            goto drop;
        }
    }
    /* end do not send exceed icmp when recv the dhcp from HAAP DTS2013101509929 */ 
#endif    
	/* Tell the sender its packet died... */
	IP_INC_STATS_BH(dev_net(skb_dst(skb)->dev), IPSTATS_MIB_INHDRERRORS);
	icmp_send(skb, ICMP_TIME_EXCEEDED, ICMP_EXC_TTL, 0);
drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}
