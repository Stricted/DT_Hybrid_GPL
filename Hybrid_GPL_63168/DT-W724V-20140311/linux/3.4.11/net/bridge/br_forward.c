/*
 *	Forwarding decision
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

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netpoll.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
#include <linux/export.h>
#endif
#include "br_private.h"
#if defined(CONFIG_BCM_KF_IGMP)
#include <linux/ip.h>
#include <linux/igmp.h>
#include "br_igmp.h"
#endif
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#endif
#ifdef CONFIG_ATP_DROP_ARP
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#endif
#ifdef CONFIG_ATP_DROP_NS
/* BEGIN: Added by h00169677, 2012/5/28   PN:DTS2012021404606 */
#include <net/ndisc.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
extern int	ipv6_get_lladdr(struct net_device *dev,struct in6_addr *addr,unsigned char banned_flags);
/* END:   Added by h00169677, 2012/5/28 */
#endif

/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#include "atpconfig.h"
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */

#if defined(CONFIG_BCM_KF_IP)
#if defined(CONFIG_BCM96828) && !defined(CONFIG_EPON_HGU)
#if defined(CONFIG_EPON_UNI_UNI_ENABLED)
int uni_uni_enabled = 1;
#else
int uni_uni_enabled = 0;
#endif
#endif
#endif


static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb));

/* Don't forward packets to originating port or forwarding diasabled */
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb, int state)
#else
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb)
#endif
{
#if defined(CONFIG_BCM_KF_IP)
#if defined(CONFIG_BCM_KF_IGMP)
	struct iphdr *pipmcast = NULL;
	struct igmphdr *pigmp = NULL;
#endif

#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	if (skb->dev == p->dev || state != BR_STATE_FORWARDING)
#else
	if (skb->dev == p->dev || p->state != BR_STATE_FORWARDING)
#endif
	{
		return 0;
	}

	/*
	 * Do not forward any packets received from one WAN interface 
	 * to other WAN interfaces in multiple PVC case
	 */
#if defined(CONFIG_BCM_KF_WANDEV)
	if( (skb->dev->priv_flags & p->dev->priv_flags) & IFF_WANDEV )
	{
		return 0;
	}

#define SUPPORT_LANVLAN
#ifdef SUPPORT_LANVLAN
	if ((skb->dev->priv_flags & IFF_WANDEV) == 0 &&
	    (p->dev->priv_flags   & IFF_WANDEV) == 0)
	{
		/* From LAN to LAN */
		/* Do not forward any packets to virtual interfaces on the same
		 * real interface of the originating virtual interface.
		 */
		struct net_device *sdev = skb->dev;
		struct net_device *ddev = p->dev;
         
		while (!netdev_path_is_root(sdev))
		{
			sdev = netdev_path_next_dev(sdev);
		}

		while (!netdev_path_is_root(ddev))
		{
			ddev = netdev_path_next_dev(ddev);
		}

		if (strcmp(sdev->name, ddev->name) == 0)
		{
			return 0;
		}

		if ((sdev->priv_flags & IFF_EXT_SWITCH) ^ (ddev->priv_flags & IFF_EXT_SWITCH))
		{
			return 1;
		}

/* TBD: Check for 6816 */
#if !defined(CONFIG_BCM96816)
		if (skb->pkt_type == PACKET_BROADCAST) 
		{
			if (sdev->priv_flags & IFF_HW_SWITCH & ddev->priv_flags)
			{
#if defined(CONFIG_BCM96828) && !defined(CONFIG_EPON_HGU) 
				if (!uni_uni_enabled) {
					/* Forward only US or DS broadcast */
					if (!(sdev->priv_flags & IFF_EPON_IF) && !(ddev->priv_flags & IFF_EPON_IF))
					{
						return 0;
					}
				}
#else
				return 0;
#endif
			}
		}
#if defined(CONFIG_BCM96828) && !defined(CONFIG_EPON_HGU)
		else if (skb->pkt_type != PACKET_MULTICAST && skb->pkt_type != PACKET_HOST)
		{
			return 0;
		}
#endif
#endif
	}
#endif //SUPPORT_LANVLAN
#endif /* CONFIG_BCM_KF_WANDEV */

#if defined(CONFIG_BCM96816) || !defined(SUPPORT_LANVLAN)
	/* IFF_HW_SWITCH now indicates switching of only bcast in hardware. Mcast is assumed
	   to be not switched in hardware and will be handled by bridge. */
	if (skb->pkt_type == PACKET_BROADCAST)
	{
		/* If source and destination interfaces belong to the switch, don't forward packet */
		if ((skb->dev->priv_flags & IFF_HW_SWITCH) && (p->dev->priv_flags & IFF_HW_SWITCH))
		{
			return 0;
		}
	}
#endif

#if defined(CONFIG_BCM_KF_IGMP)
	/*
	 * CPE is querying for LAN-2-LAN multicast.  These query messages 
	 * should not go on WAN interfaces.
	 * Also don't alow leaking of IGMPv2 report messages among LAN ports
	 */
	br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, NULL);
	if ( pigmp != NULL )
	{
		if((p->dev->priv_flags & IFF_WANDEV))
		{
			if (pigmp->type == IGMP_HOST_MEMBERSHIP_QUERY) 
			{
				return 0;
			}
		}
#if defined(CONFIG_BR_IGMP_SNOOP)
		else
		{
			if ((p->br->igmp_snooping) && (pigmp->type != IGMP_HOST_MEMBERSHIP_QUERY)) 
			{
				return 0;
			}
		}
#endif
	}
#endif
	return 1;
#else
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	return (((p->flags & BR_HAIRPIN_MODE) || skb->dev != p->dev) &&
		state == BR_STATE_FORWARDING);
#else
	return (((p->flags & BR_HAIRPIN_MODE) || skb->dev != p->dev) &&
		p->state == BR_STATE_FORWARDING);
#endif
#endif
}


#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
typedef struct net_device *(* br_fb_process_hook_t)(struct sk_buff *skb_p, uint32_t h_proto, struct net_device *txDev);
static br_fb_process_hook_t __rcu br_fb_process_hook;

void br_fb_bind(br_fb_process_hook_t brFbProcessHook)
{
   if ( NULL == brFbProcessHook ) {
      printk("br_fb_bind: invalid FB process hook\n");
   }
   printk("br_fb_bind: FB process hook bound to %p\n", brFbProcessHook );
   RCU_INIT_POINTER(br_fb_process_hook, brFbProcessHook);
}

static const struct net_bridge_port *br_fb_process(const struct net_bridge_port *to, struct sk_buff *skb)
{
	br_fb_process_hook_t fbProcessHook;
	struct net_device *newDev;
	int state = to->state;
	const struct net_bridge_port *txPrt = to;

	if ( NULL == txPrt ) {
		return NULL;
	}

	fbProcessHook = rcu_dereference(br_fb_process_hook);
	if ( fbProcessHook ) {
		newDev = fbProcessHook(skb, TYPE_ETH, txPrt->dev);
		if ( newDev ) {
			state = BR_STATE_FORWARDING;
			txPrt = br_port_get_rcu(newDev);
			if ( NULL == txPrt ) {
				txPrt = to;
			}
		}
	}

	if (should_deliver(txPrt, skb, state)) {
		return txPrt;
	}
	else {
		return NULL;
	}
}
EXPORT_SYMBOL(br_fb_bind);
#endif

static inline unsigned packet_length(const struct sk_buff *skb)
{
	return skb->len - (skb->protocol == htons(ETH_P_8021Q) ? VLAN_HLEN : 0);
}

int br_dev_queue_push_xmit(struct sk_buff *skb)
{
	/* ip_fragment doesn't copy the MAC header */
	if (nf_bridge_maybe_copy_header(skb) ||
	    (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))) {
		kfree_skb(skb);
	} else {
		skb_push(skb, ETH_HLEN);
		br_drop_fake_rtable(skb);
		dev_queue_xmit(skb);
	}

	return 0;
}

int br_forward_finish(struct sk_buff *skb)
{
	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}

static void __br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	skb->dev = to->dev;

	if (unlikely(netpoll_tx_running(to->dev))) {
		if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);
			br_netpoll_send_skb(to, skb);
		}
		return;
	}

	NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
		br_forward_finish);
}

#ifdef CONFIG_ATP_DROP_ARP
/*start of Add by t00163975 20110527 for 不转发WAN侧发来的与br0接口IP同网段的ARP */
/*Start:add by caoxiafei cKF24361 20100827 for downstream arp packets forward incorrect*/
static inline int should_drop_arp_pakcet(struct sk_buff *skb)
{
    struct net_device *dev = skb->dev;
    struct arphdr *arp = NULL;
    unsigned char *arp_ptr2 = NULL;
    unsigned char *arp_ptr = NULL;

    if (__constant_htons(ETH_P_8021Q) == skb->protocol)
    {
        struct vlan_hdr *vhdr = (struct vlan_hdr *)skb->data;
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

    if(NULL == arp)
    {
    	return 0;
    }

    /* 如果是从nas接口下来的ARP 包,而且目的IP或是源IP与br0接口IP相同的网段，则将其丢弃 */
    if ('n' == skb->dev->name[0]) // && 'a' == skb->dev->name[1] && 's' == skb->dev->name[2])
    {
		arp_ptr = (unsigned char *)(arp+1);
		arp_ptr += dev->addr_len;
		arp_ptr2 = arp_ptr;
		arp_ptr2 += 4 + dev->addr_len;
        
             return check_arp_for_all_br(arp_ptr,arp_ptr2,1);
	}
	return 0;
}

/*End:add by caoxiafei cKF24361 20100827 for downstream arp packets forward incorrect*/
/*end of Add by t00163975 20110527 for 不转发WAN侧发来的与br0接口IP同网段的ARP */
#endif

#ifdef CONFIG_ATP_DROP_NS
/* BEGIN: Added by h00169677, 2012/5/28   PN:DTS2012060900287 */
#define VLAN_HEAD_LEN 4
#define IPV6_HEAD_LEN 40
#define NS_TYPE_OFFSET 40
#define ICMPV6_OFFSET 6
static inline int should_drop_icmpv6ns_pakcet(struct sk_buff *skb)
{
    struct net_device *dev ;
	struct net *net;
	struct ipv6hdr *ipv6hd = NULL;
	struct nd_msg *msg = NULL;
	struct in6_addr addr_buf;
	
	memset(&addr_buf, 0, sizeof(struct in6_addr));
    if(!IS_LAN_DEV(skb->dev->name))
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
/* END:   Added by h00169677, 2012/5/28 */
#endif

static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	if (skb_warn_if_lro(skb)) {
		kfree_skb(skb);
		return;
	}
	
#ifdef CONFIG_ATP_DROP_ARP
    /*start of Add by t00163975 20110527 for 不转发WAN侧发来的与br0接口IP同网段的ARP */
    if(should_drop_arp_pakcet(skb))
    {
//            printk("File[%s],Line[%d]:packet from %s to %s drop\n",__FILE__, __LINE__, skb->dev->name, to->dev->name);
            kfree_skb(skb);
            return;
    }
    /*end of Add by t00163975 20110527 for 不转发WAN侧发来的与br0接口IP同网段的ARP */
#endif
#ifdef CONFIG_ATP_DROP_NS
    /* BEGIN: Added by h00169677, 2012/5/28   PN:DTS2012021404606 */
	if(should_drop_icmpv6ns_pakcet(skb))
	{
		kfree_skb(skb);
        return;
	}
	/* END:   Added by h00169677, 2012/5/28 */
#endif

	indev = skb->dev;
	skb->dev = to->dev;
    
/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#ifndef SUPPORT_ATP_FOR_HARDWARE_IAMGE
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */
#ifdef CONFIG_DT_COMMON
    if (IS_WAN_DEVICE(skb->dev->name))
    {
        if ((skb->protocol != htons(ETH_P_PPP_DISC))
            && (skb->protocol != htons(ETH_P_PPP_SES)))
        {
    		kfree_skb(skb);
            return;
        }
    }
#endif
/* BEGIN: Added by l00203771, 2013/3/20 DTS2013031909950 */
#endif
/* END: Added by l00203771, 2013/3/20 DTS2013031909950 */
	skb_forward_csum(skb);

	NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
		br_forward_finish);
}

/* called with rcu_read_lock */
void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	to = br_fb_process(to, skb);
	if ( to ) {
#else
	if (to && should_deliver(to, skb)) {
#endif
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called with rcu_read_lock */
void br_forward(const struct net_bridge_port *to, struct sk_buff *skb, struct sk_buff *skb0)
{
#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	to = br_fb_process(to, skb);
	if ( to ) {
#else
	if (should_deliver(to, skb)) {
#endif   
		if (skb0)
			deliver_clone(to, skb, __br_forward);
		else
			__br_forward(to, skb);
		return;
	}

	if (!skb0)
		kfree_skb(skb);
}

static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;

	skb = skb_clone(skb, GFP_ATOMIC);
	if (!skb) {
		dev->stats.tx_dropped++;
		return -ENOMEM;
	}

	__packet_hook(prev, skb);
	return 0;
}

static struct net_bridge_port *maybe_deliver(
	struct net_bridge_port *prev, struct net_bridge_port *p,
	struct sk_buff *skb,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	int err;

#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
	if (!should_deliver(p, skb, p->state))
#else
	if (!should_deliver(p, skb))
#endif
		return prev;

	if (!prev)
		goto out;

	err = deliver_clone(prev, skb, __packet_hook);
	if (err)
		return ERR_PTR(err);

out:
	return p;
}

/* called under bridge lock */
static void br_flood(struct net_bridge *br, struct sk_buff *skb,
		     struct sk_buff *skb0,
		     void (*__packet_hook)(const struct net_bridge_port *p,
					   struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	Blog_t * blog_p = blog_ptr(skb);

	if (blog_p && !blog_p->rx.multicast)
		blog_skip(skb);
#endif
	prev = NULL;

	list_for_each_entry_rcu(p, &br->port_list, list) {
		prev = maybe_deliver(prev, p, skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;
	}

	if (!prev)
		goto out;

    if (skb0)
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
    {
        blog_clone(skb, blog_ptr(skb0));
#endif
        deliver_clone(prev, skb, __packet_hook);
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
    }
#endif
    else
        __packet_hook(prev, skb);
    return;

out:
	if (!skb0)
		kfree_skb(skb);
}


/* called with rcu_read_lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb)
{
	br_flood(br, skb, NULL, __br_deliver);
}

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb,
		      struct sk_buff *skb2)
{
	br_flood(br, skb, skb2, __br_forward);
}

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
/* called with rcu_read_lock */
static void br_multicast_flood(struct net_bridge_mdb_entry *mdst,
			       struct sk_buff *skb, struct sk_buff *skb0,
			       void (*__packet_hook)(
					const struct net_bridge_port *p,
					struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *prev = NULL;
	struct net_bridge_port_group *p;
	struct hlist_node *rp;

	rp = rcu_dereference(hlist_first_rcu(&br->router_list));
	p = mdst ? rcu_dereference(mdst->ports) : NULL;
	while (p || rp) {
		struct net_bridge_port *port, *lport, *rport;

		lport = p ? p->port : NULL;
		rport = rp ? hlist_entry(rp, struct net_bridge_port, rlist) :
			     NULL;

		port = (unsigned long)lport > (unsigned long)rport ?
		       lport : rport;

		prev = maybe_deliver(prev, port, skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;

		if ((unsigned long)lport >= (unsigned long)port)
			p = rcu_dereference(p->next);
		if ((unsigned long)rport >= (unsigned long)port)
			rp = rcu_dereference(hlist_next_rcu(rp));
	}

	if (!prev)
		goto out;

	if (skb0)
		deliver_clone(prev, skb, __packet_hook);
	else
		__packet_hook(prev, skb);
	return;

out:
	if (!skb0)
		kfree_skb(skb);
}

/* called with rcu_read_lock */
void br_multicast_deliver(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb)
{
	br_multicast_flood(mdst, skb, NULL, __br_deliver);
}

/* called with rcu_read_lock */
void br_multicast_forward(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb, struct sk_buff *skb2)
{
	br_multicast_flood(mdst, skb, skb2, __br_forward);
}
#endif
