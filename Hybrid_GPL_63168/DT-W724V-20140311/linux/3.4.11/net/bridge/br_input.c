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

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include <linux/export.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_IP)
#include <linux/if_vlan.h>
#include <linux/timer.h>
#include <linux/igmp.h>
#include <linux/ip.h>
#include <linux/ktime.h>
#endif
#ifdef CONFIG_ATP_MLD_SNOOPING
#include "br_mld_snooping.h"
#endif
#ifdef CONFIG_ATP_DROP_ARP
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#endif
#ifdef CONFIG_ATP_DROP_NS
/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
#include <net/ndisc.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include "atp_interface.h"
extern int	ipv6_get_lladdr(struct net_device *dev,struct in6_addr *addr,unsigned char banned_flags);
static inline int should_drop_ns_fromwan(struct sk_buff *skb);
/* END:   Added by h00169677, 2012/6/8 */
#endif

/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#include "atpconfig.h"
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#endif
#if defined(CONFIG_BCM_KF_IGMP)
#include "br_igmp.h"
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#endif
#if defined(CONFIG_BCM_KF_WL)
#if defined(PKTC)
#include <linux_osl_dslcpe_pktc.h>

uint32_t (*wl_pktc_req_hook)(int req_id, uint32_t param0, uint32_t param1, uint32_t param2) = NULL;
EXPORT_SYMBOL(wl_pktc_req_hook);
#endif /* PKTC */
#endif

/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

/* Hook for brouter */
br_should_route_hook_t __rcu *br_should_route_hook __read_mostly;
EXPORT_SYMBOL(br_should_route_hook);

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

static int br_pass_frame_up(struct sk_buff *skb)
{
	struct net_device *indev, *brdev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(brdev);
	struct br_cpu_netstats *brstats = this_cpu_ptr(br->stats);

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	blog_link(IF_DEVICE, blog_ptr(skb), (void*)br->dev, DIR_RX, skb->len);

	/* Gather general RX statistics */
	brdev->stats.rx_packets++;
	brdev->stats.rx_bytes += skb->len;
#endif   

	u64_stats_update_begin(&brstats->syncp);
	brstats->rx_packets++;
	brstats->rx_bytes += skb->len;
	u64_stats_update_end(&brstats->syncp);

	indev = skb->dev;
	skb->dev = brdev;

	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		       netif_receive_skb);
}

/* note: already called with rcu_read_lock */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct net_bridge_mdb_entry *mdst;
	struct sk_buff *skb2;
#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || defined(CONFIG_BCM_KF_IGMP_RATE_LIMIT)
	struct iphdr *pipmcast = NULL;
	struct igmphdr *pigmp = NULL;
#endif
#if (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))
	struct ipv6hdr *pipv6mcast = NULL;
	struct icmp6hdr *picmpv6 = NULL;
#endif

#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
	__u16 vid = VLAN_N_VID;
#endif
	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || defined(CONFIG_BCM_KF_IGMP_RATE_LIMIT)
	br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, NULL);
#endif

#if defined(CONFIG_BCM_KF_IGMP_RATE_LIMIT)
	if ( pigmp != NULL)
	{
#if defined(CONFIG_BCM_GPON_MODULE)
		/* drop IGMP v1 report packets */
		if (pigmp->type == IGMP_HOST_MEMBERSHIP_REPORT)
		{
			goto drop;
		}

		/* drop IGMP v1 query packets */
		if ((pigmp->type == IGMP_HOST_MEMBERSHIP_QUERY) &&
		    (pigmp->code == 0))
		{
			goto drop;
		}

		/* drop IGMP leave packets for group 0.0.0.0 */
		if ((pigmp->type == IGMP_HOST_LEAVE_MESSAGE) &&
		    (0 == pigmp->group) )
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

#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
#if defined(CONFIG_BCM_KF_VLAN) && (defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE))
	if (skb->vlan_count)
 		vid = (skb->vlan_header[0] >> 16) & VLAN_VID_MASK;
	else 
#endif /* CONFIG_BCM_VLAN) */
	/* 
	*  dev.c/__netif_receive_skb(): if proto == ETH_P_8021Q
	*  call vlan_untag() to remove tag and save vid in skb->vlan_tci
	*/
	if (vlan_tx_tag_present(skb))
		vid = skb->vlan_tci & VLAN_VID_MASK;
	else if ( vlan_eth_hdr(skb)->h_vlan_proto == ETH_P_8021Q )
		vid = vlan_eth_hdr(skb)->h_vlan_TCI & VLAN_VID_MASK;
#endif

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
	br_fdb_update(br, p, eth_hdr(skb)->h_source, vid);
#else
	br_fdb_update(br, p, eth_hdr(skb)->h_source);
#endif

	/*START MODIFY:l00180987 2012-11-20 FOR snooping开关失效*/
	/*ATP 使用自己的snooping处理*/
#ifndef CONFIG_ATP_COMMON
	if (!is_broadcast_ether_addr(dest) && is_multicast_ether_addr(dest) &&
	    br_multicast_rcv(br, p, skb))
		goto drop;
#endif
	/*END MODIFY:l00180987 2012-11-20 FOR snooping开关失效*/
#if defined(CONFIG_BCM_KF_WL)
	if ((p->state == BR_STATE_LEARNING) && (skb->protocol != 0x886c /*ETHER_TYPE_BRCM*/))
#else
	if (p->state == BR_STATE_LEARNING)
#endif
		goto drop;

	BR_INPUT_SKB_CB(skb)->brdev = br->dev;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

	
#ifdef CONFIG_ATP_DROP_NS
	/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
	if (should_drop_ns_fromwan(skb))
	{
		goto drop;
	}
	/* END:   Added by h00169677, 2012/6/8 */
#endif
#ifdef CONFIG_ATP_DROP_ARP
   /* BEGIN: Added by h00169677, 2012/6/21   PN:DTS2012061205128 */
   if ('n' == skb->dev->name[0])
   {
        if ( IsNeedDropArp(skb) )
	    {
	        goto drop;
        }
   }
   /* END:   Added by h00169677, 2012/6/21 */
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	br_mld_get_ip_icmp_hdrs(skb, &pipv6mcast, &picmpv6, NULL);
	if(pipv6mcast != NULL ) {
		if (br_mld_mc_forward(br, skb, 1, 0))
		{
			/* packet processed by mld snooping - no further processing required */
			goto out;
		}
	}
	else
#endif
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
	if (pipmcast != NULL) {
		if (br_igmp_mc_forward(br, skb, 1, 0)) 
		{
			/* packet processed by igmp snooping - no further processing required */
			goto out;
		}
	}
#endif

	if (is_broadcast_ether_addr(dest))
#if defined(CONFIG_BCM_KF_EXTSTATS)
	{
		br->dev->stats.rx_broadcast_packets++;
#endif
		skb2 = skb;   
#if defined(CONFIG_BCM_KF_EXTSTATS)
	}
#endif
	else if (is_multicast_ether_addr(dest)) {
#ifdef CONFIG_ATP_COMMON
		// 先默认可以允许网关接收并转发给其他接口
		skb2 = skb;
#ifdef CONFIG_ATP_MLD_SNOOPING 
		if ((0x33 == dest[0]) && (0x33 == dest[1]))
        {
			if ((NULL != skb) && (br_mld_snooping_forward(skb,br,dest, 1)))
			{
				// 如果被snooping处理了，那么这种情况下表示的是报文不需要被转发了 
				skb = NULL;
			}
		}
#endif

#else
		mdst = br_mdb_get(br, skb);
		if (mdst || BR_INPUT_SKB_CB_MROUTERS_ONLY(skb)) {
			if ((mdst && mdst->mglist) ||
			    br_multicast_is_router(br))
				skb2 = skb;
			br_multicast_forward(mdst, skb, skb2);
			skb = NULL;
			if (!skb2)
				goto out;
		} else
			skb2 = skb;
#endif
		br->dev->stats.multicast++;
#if defined(CONFIG_BCM_KF_EXTSTATS)
		br->dev->stats.rx_multicast_bytes += skb2->len;
#endif
#if !(defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG))
	} else if ((dst = __br_fdb_get(br, dest)) && dst->is_local) {
		skb2 = skb;
		/* Do not forward the packet since it's local. */
		skb = NULL;
	}
#else
	} 
	else {
		dst = __br_fdb_get(br, dest);
		blog_link(BRIDGEFDB, blog_ptr(skb),
					(void*)__br_fdb_get(br, eth_hdr(skb)->h_source),
					BLOG_PARAM1_SRCFDB, 0);
		blog_link(BRIDGEFDB, blog_ptr(skb), (void*)dst, BLOG_PARAM1_DSTFDB, 0);

#if defined(PKTC)
		/* wlan pktc */
		if ((dst != NULL) && (!dst->is_local))
		{
#if defined(CONFIG_BCM_KF_WL)
			u8 from_wl_to_switch=0, from_switch_to_wl=0;
			struct net_bridge_fdb_entry *src = __br_fdb_get(br, eth_hdr(skb)->h_source);
			struct net_device *root_dst_dev_p = dst->dst->dev;

			if (unlikely(wl_pktc_req_hook == NULL))
				goto next;
			
			if (unlikely(src == NULL))
				goto next;

			if ((src->dst->dev->path.hw_port_type == BLOG_WLANPHY) &&
			    (dst->dst->dev->path.hw_port_type == BLOG_ENETPHY))
			{
				from_wl_to_switch = 1;
				while(!netdev_path_is_root(root_dst_dev_p) && (root_dst_dev_p->priv_flags & IFF_BCM_VLAN))
				{
					root_dst_dev_p = netdev_path_next_dev(root_dst_dev_p);
				}
			}
			else if ((src->dst->dev->path.hw_port_type == BLOG_ENETPHY) &&
			    (dst->dst->dev->path.hw_port_type == BLOG_WLANPHY) &&
				wl_pktc_req_hook(GET_PKTC_TX_MODE, 0, 0, 0))
				from_switch_to_wl = 1;

#if defined(CONFIG_BCM_KF_WANDEV)
			if ((from_wl_to_switch || from_switch_to_wl) && !(dst->dst->dev->priv_flags & IFF_WANDEV) && netdev_path_is_root(root_dst_dev_p)) { 
			/* Also check for non-WAN cases.
			   For the Rx direction, VLAN cases are allowed as long as the packets are untagged.
			   Tagged packets are not forwarded through the chaining path by WLAN driver. Tagged packets go through the flowcache path.
			   see wlc_sendup_chain() function for reference.
			   For the Tx direction, there are no VLAN interfaces created on wl device when LAN_VLAN flag is enabled in the build.
			   The netdev_path_is_root() check makes sure that we are always transmitting to a root device */
				uint32_t chainIdx = wl_pktc_req_hook(UPDATE_BRC_HOT, (uint32_t)&(dst->addr.addr[0]), (uint32_t)root_dst_dev_p, 0);
				if (chainIdx != INVALID_CHAIN_IDX)
				{
					//Update chainIdx in blog
					// chainEntry->tx_dev will always be NOT NULL as we just added that above
					if (skb->blog_p != NULL) 
					{
						skb->blog_p->wlTxChainIdx = chainIdx;
						//   printk("Added ChainTableEntry Idx %d Dev %s blogSrcAddr 0x%x blogDstAddr 0x%x DstMac %x:%x:%x:%x:%x:%x\n", 
						//        chainEntry->idx, dst->dst->dev->name, skb->blog_p->rx.tuple.saddr, skb->blog_p->rx.tuple.daddr, dst->addr.addr[0],
						//      dst->addr.addr[1], dst->addr.addr[2], dst->addr.addr[3], dst->addr.addr[4], dst->addr.addr[5]);
					}
				}
			}
#endif /* CONFIG_BCM_KF_WANDEV */
#endif
		}
next:
#endif /* PKTC */
		if ((dst != NULL) && dst->is_local) 
		{
			skb2 = skb;
			/* Do not forward the packet since it's local. */
			skb = NULL;
		}
	}
#endif
	if (skb) {
		if (dst) {
			dst->used = jiffies;
			br_forward(dst->dst, skb, skb2);
		} else
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
			if ( BR_STATE_BLOCKING == p->state ) {
				/* prevent flooding unknown unicast from blocked port */
				goto drop;
			}
			else
#endif
			br_flood_forward(br, skb, skb2);
	}

	if (skb2)
		return br_pass_frame_up(skb2);

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);

#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
	br_fdb_update(p->br, p, eth_hdr(skb)->h_source, VLAN_N_VID);
#else
	br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
#endif
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
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock
 */
rx_handler_result_t br_handle_frame(struct sk_buff **pskb)
{
	struct net_bridge_port *p;
	struct sk_buff *skb = *pskb;
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	br_should_route_hook_t *rhook;

	if (unlikely(skb->pkt_type == PACKET_LOOPBACK))
		return RX_HANDLER_PASS;

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#ifndef SUPPORT_ATP_FOR_HARDWARE_IAMGE
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */
#ifdef CONFIG_DT_COMMON
    if (IS_WAN_DEVICE(skb->dev->name))
    {
        if ((skb->protocol != htons(ETH_P_PPP_DISC))
            && (skb->protocol != htons(ETH_P_PPP_SES)))
        {
            goto drop;
        }
    }
#endif
/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#endif
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */
	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return RX_HANDLER_CONSUMED;

	p = br_port_get_rcu(skb->dev);

	if (unlikely(is_link_local(dest))) {
		/*
		 * See IEEE 802.1D Table 7-10 Reserved addresses
		 *
		 * Assignment		 		Value
		 * Bridge Group Address		01-80-C2-00-00-00
		 * (MAC Control) 802.3		01-80-C2-00-00-01
		 * (Link Aggregation) 802.3	01-80-C2-00-00-02
		 * 802.1X PAE address		01-80-C2-00-00-03
		 *
		 * 802.1AB LLDP 		01-80-C2-00-00-0E
		 *
		 * Others reserved for future standardization
		 */
		switch (dest[5]) {
		case 0x00:	/* Bridge Group Address */
			/* If STP is turned off,
			   then must forward to keep loop detection */
			if (p->br->stp_enabled == BR_NO_STP)
				goto forward;
			break;

		case 0x01:	/* IEEE MAC (Pause) */
			goto drop;

		default:
			/* Allow selective forwarding for most other protocols */
			if (p->br->group_fwd_mask & (1u << dest[5]))
				goto forward;
		}

		/* Deliver packet to local host only */
		if (NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish)) {
			return RX_HANDLER_CONSUMED; /* consumed by filter */
		} else {
			*pskb = skb;
			return RX_HANDLER_PASS;	/* continue processing */
		}
	}

forward:
#if defined(CONFIG_BCM_KF_IEEE1905) && defined(CONFIG_BCM_IEEE1905)
	/* allow broute to forward packets to the stack in any STP state */
	rhook = rcu_dereference(br_should_route_hook);
	if (rhook) {
		if ((*rhook)(skb)) {
			*pskb = skb;
			if ( (skb->protocol == 0x893a) || (skb->protocol == 0x8912) || (skb->protocol == 0x88e1) )
			{
				br_handle_local_finish(skb);
			}
			return RX_HANDLER_PASS;
		}
		dest = eth_hdr(skb)->h_dest;
	}
#endif

#if defined(CONFIG_BCM_KF_WL)
	if ((skb->protocol == 0x886c /*ETHER_TYPE_BRCM*/) && (p->state != BR_STATE_FORWARDING)) {
		/* force to forward brcm_type event packet */
		NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		return RX_HANDLER_CONSUMED;
	}
#endif

	switch (p->state) {
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	case BR_STATE_BLOCKING:
		/* if this is unicast let it through even if the port is blocked 
		   it will be dropped later if a destination is not found to
                   prevent flooding unicast from a blocked port */
		if ( is_multicast_ether_addr(dest) ) {
			goto drop;
		}
#endif
	case BR_STATE_FORWARDING:
#if !defined(CONFIG_BCM_KF_IEEE1905) || !defined(CONFIG_BCM_IEEE1905)
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook) {
			if ((*rhook)(skb)) {
				*pskb = skb;
				return RX_HANDLER_PASS;
			}
			dest = eth_hdr(skb)->h_dest;
		}
#endif
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return RX_HANDLER_CONSUMED;
}

#ifdef CONFIG_ATP_DROP_NS
/* BEGIN: Added by h00169677, 2012/6/8   PN:DTS2012052606695 */
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
/* END:   Added by h00169677, 2012/6/8 */
#endif
