/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#if defined(CONFIG_MIPS_BRCM)
#include <linux/if_vlan.h>
#include <linux/timer.h>
#include <linux/igmp.h>
#include <linux/ip.h>
#include <linux/blog.h>
#endif
#include "br_private.h"
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
#include "br_igmp.h"
#endif
#ifdef CONFIG_IGMP_SNOOPING
#include "br_igmp_snooping.h"
#endif
#ifdef CONFIG_MLD_SNOOPING
#include "br_mld_snooping.h"
#endif

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#endif

#if defined(CONFIG_BR_IGMP_STATISTIC)
#include <linux/igmp_statistic.h>
#endif

#include <linux/if_arp.h>
#include <linux/if_vlan.h>
#include <linux/inetdevice.h>

/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
#ifdef CONFIG_IPV6
#include <net/ndisc.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include "atp_interface.h"
extern int	ipv6_get_lladdr(struct net_device *dev,struct in6_addr *addr,unsigned char banned_flags);
static inline int should_drop_ns_fromwan(struct sk_buff *skb);
#endif
/* END:   Added by h00169677, 2012/6/8 */
/**
 * Features changed due to FAP power up/down
**/

/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };
#ifdef CONFIG_ATP_DROP_ARP

/* BEGIN: Added by z67728, 2010/11/18 过滤转发的非网关发送的包含网关br0地址ip的arp报文*/
static inline int IsNeedDropArp(struct sk_buff *skb)
{
    struct net_device *dev = skb->dev;
    struct arphdr *arp = NULL;
    unsigned char *arp_ptr2 = NULL;
    unsigned char *arp_ptr = NULL;

    if (__constant_htons(ETH_P_8021Q) == skb->protocol)
    {
        struct vlan_ethhdr *vhdr = (struct vlan_ethhdr *)skb->data;
        if (vhdr->h_vlan_encapsulated_proto ==  ntohs(ETH_P_ARP)) 
        {   
            arp = (struct arphdr *)(vhdr + 1);
        }
    }
    else
    {
    		__be16 ucprotocol = *(__be16 *)&(skb->data[12]);
    	if ((htons(ETH_P_ARP) == ucprotocol) || (htons(ETH_P_ARP) == skb->protocol))
    	{
        	arp = arp_hdr(skb);
    	}
    }

    if( NULL == arp )
    {
    	return 0;
    }

    //printk("\r\n OPT[%x] dev[%s]\r\n",htons(arp->ar_op),dev->name);
    /* 防止伪装arp应答和arp请求 */
	arp_ptr   = (unsigned char *)(arp+1);
	arp_ptr  += dev->addr_len;
    arp_ptr2  = arp_ptr;
    arp_ptr2 += 4 + dev->addr_len;

    return check_arp_for_all_br(arp_ptr,arp_ptr2,0);

}
/* END:   Added by z67728, 2010/11/18 */
#endif

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)&& !defined(CONFIG_BR_IGMP_SNOOP)
#define PPPOE_UDPPROTOID_OFFSET        17    
#define PPPOE_UDPADDR_OFFSET           24
static void blog_skip_ppp_multicast(struct sk_buff *skb, const unsigned char *dest)
{
	unsigned short usProtocol;
	unsigned short usProtocol1;
	__u32 *usMulticastIp;

    if(!is_multicast_ether_addr(dest) )
    {
        usProtocol = ((u16 *) skb->data)[-1];
        usProtocol1 = skb->data[PPPOE_UDPPROTOID_OFFSET];
        usMulticastIp = (__u32*)(skb->data + PPPOE_UDPADDR_OFFSET);
        if((usProtocol == __constant_htons(ETH_P_PPP_SES))  
            && (usProtocol1 == IPPROTO_UDP))
        {
            if(MULTICAST((*usMulticastIp)))
            {
                blog_skip(skb);
            }
        }
    }
}
#endif

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev, *brdev = br->dev;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	blog_link(IF_DEVICE, blog_ptr(skb), (void*)br->dev, DIR_RX, skb->len);
#endif

#if defined(CONFIG_BR_IGMP_STATISTIC)
    if ( !is_multicast_ether_addr(eth_hdr(skb)->h_dest) )
    {
        if ( IS_LAN_DEV(skb->dev->name) )
        {
            /* 上行流转发设备数据报文统计 */
            br_igmp_statistic_inc(skb,IGMP_UP_REQUEST_PACKET);
        }
    }
#endif    
	brdev->stats.rx_packets++;
	brdev->stats.rx_bytes += skb->len;

	indev = skb->dev;
    /* add lan interface (eth0.5) in iptables 20090217 */
    skb->lanindev = indev;
	skb->dev = brdev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		netif_receive_skb);
}

/* note: already called with rcu_read_lock (preempt_disabled) */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct sk_buff *skb2;
#if defined(CONFIG_MIPS_BRCM)
	struct iphdr *pip = NULL;
	__u8 igmpTypeOffset = 0;
#endif

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

#if defined(CONFIG_MIPS_BRCM)
	if ( vlan_eth_hdr(skb)->h_vlan_proto == ETH_P_IP )
	{
		pip = ip_hdr(skb);
		igmpTypeOffset = (pip->ihl << 2);
	}
	else if ( vlan_eth_hdr(skb)->h_vlan_proto == ETH_P_8021Q )
	{
		if ( vlan_eth_hdr(skb)->h_vlan_encapsulated_proto == ETH_P_IP )
		{
			pip = (struct iphdr *)(skb_network_header(skb) + sizeof(struct vlan_hdr));
			igmpTypeOffset = (pip->ihl << 2) + sizeof(struct vlan_hdr);
		}
	}

	if ((pip) && (pip->protocol == IPPROTO_IGMP))
	{
#if defined(CONFIG_BCM_GPON_MODULE)
		struct igmphdr *ih = (struct igmphdr *)&skb->data[igmpTypeOffset];

		/* drop IGMP v1 report packets */
		if (ih->type == IGMP_HOST_MEMBERSHIP_REPORT)
		{
			goto drop;
		}

		/* drop IGMP v1 query packets */
		if ((ih->type == IGMP_HOST_MEMBERSHIP_QUERY) &&
		    (ih->code == 0))
		{
			goto drop;
		}

		/* drop IGMP leave packets for group 0.0.0.0 */
		if ((ih->type == IGMP_HOST_LEAVE_MESSAGE) &&
          (0 == ih->group) )
		{
			goto drop;
		}
#endif
		/* rate limit IGMP */
		br = p->br;
		if ( br->igmp_rate_limit )
		{
			ktime_t      curTime;
			u64          diffUs;
			unsigned int usPerPacket;
			unsigned int temp32;
			unsigned int burstLimit;

			/* add tokens to the bucket - compute in microseconds */
			curTime     = ktime_get();
			usPerPacket = (1000000 / br->igmp_rate_limit);
			diffUs      = ktime_to_us(ktime_sub(curTime, br->igmp_rate_last_packet));
			diffUs     += br->igmp_rate_rem_time;

			/* allow 25% burst */
			burstLimit = br->igmp_rate_limit >> 2;
			if ( 0 == burstLimit)
			{
				burstLimit = 1;
			}

			if ( diffUs > 1000000 )
			{
				br->igmp_rate_bucket   = burstLimit;
				br->igmp_rate_rem_time = 0;
			}
			else
			{
				temp32 = (unsigned int)diffUs / usPerPacket;
				br->igmp_rate_bucket += temp32;
				if (temp32)
				{
					br->igmp_rate_rem_time = diffUs - (temp32 * usPerPacket);
				}
			}
			if (br->igmp_rate_bucket > burstLimit)
			{
				br->igmp_rate_bucket   = burstLimit;
				br->igmp_rate_rem_time = 0;
			}

			/* if bucket is empty drop the packet */
			if (0 == br->igmp_rate_bucket)
			{
				goto drop;
			}
			br->igmp_rate_bucket--;
			br->igmp_rate_last_packet.tv64 = curTime.tv64;
		}
	}
#endif

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
	br_fdb_update(br, p, eth_hdr(skb)->h_source);

	if (p->state == BR_STATE_LEARNING)
		goto drop;
	
	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

	
	/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
#ifdef CONFIG_IPV6
	if (should_drop_ns_fromwan(skb))
	{
		goto drop;
	}
#endif
	/* END:   Added by h00169677, 2012/6/8 */

   /* BEGIN: Added by h00169677, 2012/6/21   PN:DTS2012061205128 */
   if ('n' == skb->dev->name[0])
   {
        if ( IsNeedDropArp(skb) )
	    {
	        goto drop;
        }
   }
   /* END:   Added by h00169677, 2012/6/21 */
	
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	if((0x33 == dest[0]) && (0x33 == dest[1])) {
		br->statistics.multicast++;
		skb2 = skb;
		if (br_mld_mc_forward(br, skb, 1, 0))
		{
			skb = NULL;
			skb2 = NULL;
		}
	} else 
#endif
	if (is_multicast_ether_addr(dest)) {
		br->dev->stats.multicast++;
		skb2 = skb;
#if defined(CONFIG_MLD_SNOOPING)
        if ((0x33 == dest[0]) && (0x33 == dest[1]))
        {
            if ((NULL != skb) && (br_mld_snooping_forward(skb,br,dest, 1)))
            {
                skb = NULL;
            }
        }
#endif
        if ((0x33 != dest[0]) || (0x33 != dest[1]))
        {
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
    		if ((NULL != skb) && (br_igmp_mc_forward(br, skb, 1, 0)))
    		{
    			skb = NULL;
    		}
#elif defined(CONFIG_IGMP_SNOOPING)
            if ((NULL != skb) && (br_igmp_snooping_forward(skb, br, (unsigned char *)dest, 1)))
            {
                skb = NULL;
            }
#endif
        }
#if defined(CONFIG_BR_IGMP_STATISTIC)
        /* 此处只统计IGMP协议报文和总报文数 */
        if (!is_broadcast_ether_addr(dest)) {
            br_igmp_statistic_inc(skb2,IGMP_BUFF);
        }
#endif        
	} else {
		dst = __br_fdb_get(br, dest);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
		blog_link(BRIDGEFDB, blog_ptr(skb),
					(void*)__br_fdb_get(br, eth_hdr(skb)->h_source),
					BLOG_PARAM1_SRCFDB, 0);
		blog_link(BRIDGEFDB, blog_ptr(skb), (void*)dst, BLOG_PARAM1_DSTFDB, 0);
#endif
		if ((dst != NULL) && dst->is_local) 
		{
			skb2 = skb;
			/* Do not forward the packet since it's local. */
			skb = NULL;
		}
	}

	if((skb != NULL) && (skb2 == skb))
	{
		skb2 = skb_clone(skb, GFP_ATOMIC);
	}

	if (skb2) 
		br_pass_frame_up(br, skb2);

    
	if (skb) {
#ifdef CONFIG_ATP_DROP_ARP        
	    /* BEGIN: Added by z67728, 2010/9/19 PN:DTS2010091403656,ARP欺骗防御*/
	    if ( IsNeedDropArp(skb) )
	    {
	        goto drop;
            }
            /* END:   Added by z67728, 2010/9/19 */
#endif
		if (dst)
	    {
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)&& !defined(CONFIG_BR_IGMP_SNOOP)
#ifndef CONFIG_BCM96362
            blog_skip_ppp_multicast(skb, dest);
#endif
#endif
			br_forward(dst->dst, skb);
	    }
		else
			br_flood_forward(br, skb);
	}

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock (preempt_disabled) */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);

	if (p)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	__be16 *a = (__be16 *)dest;
	static const __be16 *b = (const __be16 *)br_group_address;
	static const __be16 m = cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/*
 * Called via br_handle_frame_hook.
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock (preempt_disabled)
 */
struct sk_buff *br_handle_frame(struct net_bridge_port *p, struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff *skb);

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return NULL;

	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* If STP is turned off, then forward */
		if (p->br->stp_enabled == BR_NO_STP && dest[5] == 0)
			goto forward;

		if (NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish))
			return NULL;	/* frame consumed by filter */
		else
			return skb;	/* continue processing */
	}

forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}
/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
#ifdef CONFIG_IPV6
#define VLAN_HEAD_LEN 4
#define IPV6_HEAD_LEN 40
#define NS_TYPE_OFFSET 40
#define ICMPV6_OFFSET 6
static inline int should_drop_ns_fromwan(struct sk_buff *skb)
{
    struct net_device *dev ;
	struct net *net;
	struct ipv6hdr *ipv6hd = NULL;
	struct nd_msg *msg = NULL;
	struct in6_addr addr_buf;
	
	memset(&addr_buf, 0, sizeof(struct in6_addr));
    if(!IS_WAN_DEVICE(skb->dev->name))
    {
        return 0;
    }
    
    if (__constant_htons(ETH_P_8021Q) == skb->protocol)
    {
        struct vlan_hdr *vhdr = (struct vlan_hdr *)skb->data;
        if (vhdr->h_vlan_encapsulated_proto !=  ntohs(ETH_P_IPV6))
        {
            return 0;
        }
        
        ipv6hd = (struct ipv6hdr *)(vhdr + 1);
        
        if ((ipv6_addr_any(&ipv6hd->saddr)) || 
            (IPPROTO_ICMPV6 != skb->data[VLAN_HEAD_LEN+ICMPV6_OFFSET]) ||
            (0x87 != skb->data[VLAN_HEAD_LEN+NS_TYPE_OFFSET]))
        {
            return 0;
        }
            
        msg = (struct nd_msg *)(skb->data+VLAN_HEAD_LEN+IPV6_HEAD_LEN);
    }
    else if (__constant_htons(ETH_P_IPV6) == skb->protocol)
    {
        ipv6hd = (struct ipv6hdr *)(skb->data);

        if ((ipv6_addr_any(&ipv6hd->saddr)) || 
            (IPPROTO_ICMPV6 != skb->data[ICMPV6_OFFSET]) ||
            (0x87 != skb->data[NS_TYPE_OFFSET]))
        {
            return 0;
        }
        msg = (struct nd_msg *)(skb->data+IPV6_HEAD_LEN);
    }
    else
    {
        return 0;
    }
    
    for_each_net(net) 
    {
        for_each_netdev(net, dev) 
    	{
    		if(IS_WAN_DEVICE(dev->name) || IS_BRIDGE_DEV(dev->name))
    		{
    			if (!ipv6_get_lladdr(dev, &addr_buf, IFA_F_TENTATIVE)) 
    			{
    				if ((NULL != msg) && (ipv6_addr_equal(&(msg->target),&addr_buf)))
    				{
    					return 1;
    				}
    			}
    		}
        }
    }
	return 0;
}
#endif
/* END:   Added by h00169677, 2012/6/8 */
