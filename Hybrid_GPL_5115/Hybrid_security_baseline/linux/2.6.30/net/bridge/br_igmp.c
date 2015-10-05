/*
* <:copyright-BRCM:2011:DUAL/GPL:standard
* 
*    Copyright (c) 2011 Broadcom Corporation
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
:>
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <asm/atomic.h>
#include <linux/ip.h>
#include <linux/if_vlan.h>
#include "br_igmp_parse.h"
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/rtnetlink.h>
#include <linux/rtnetlink.h>
#include "br_private.h"
#include "br_igmp.h"

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
#include <linux/if_vlan.h>
#include <linux/blog.h>
#include <linux/blog_rule.h>
#include "br_mcast.h"
#endif

//#define BR_DEV_DEBUG
#ifdef BR_DEV_DEBUG
#define br_igmp_debug(args...)   color_printk(__func__, __LINE__, _RET_IP_, ## args)
#else
#define br_igmp_debug(args...)
#endif


#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)

static struct kmem_cache *br_igmp_mc_fdb_cache __read_mostly;
static struct kmem_cache *br_igmp_mc_rep_cache __read_mostly;
static u32 br_igmp_mc_fdb_salt __read_mostly;
static struct proc_dir_entry *br_igmp_entry = NULL;
static int br_igmp_lan2lan_snooping = 0;

extern int mcpd_process_skb(struct net_bridge *br, struct sk_buff *skb,
                            int protocol);

static struct in_addr ip_upnp_addr      = {0xEFFFFFFA}; /* UPnP / SSDP */
static struct in_addr ip_ntfy_srvr_addr = {0xE000FF87}; /* Notificatoin Server*/

static inline int br_igmp_mc_fdb_hash(const u32 grp)
{
	return jhash_1word(grp, br_igmp_mc_fdb_salt) & (BR_IGMP_HASH_SIZE - 1);
}

int br_igmp_control_filter(const unsigned char *dest, __be32 dest_ip)
{
    if(((dest) && is_broadcast_ether_addr(dest)) ||
       ((htonl(dest_ip) & htonl(0xFFFFFF00)) == htonl(0xE0000000)) ||
       (htonl(dest_ip) == htonl(ip_upnp_addr.s_addr)) || /* UPnp/SSDP */
       (htonl(dest_ip) == htonl(ip_ntfy_srvr_addr.s_addr)))   /* Notification srvr */
    {
        return 0;
    }
    else
    {
        return 1;
    }
} /* br_igmp_control_filter */


// 开启LAN-LAN snooping功能
void br_igmp_lan2lan_snooping_update(int val)
{
    br_igmp_lan2lan_snooping = val;
}

int br_igmp_get_lan2lan_snooping_info(void)
{
    return br_igmp_lan2lan_snooping;
}

static void br_igmp_query_timeout(unsigned long ptr)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge *br;
	struct net_bridge_mc_rep_entry *rep_entry, *rep_entry_n;
	int i;
    
	br = (struct net_bridge *) ptr;

	/* if snooping is disabled just return */
	if ( 0 == br->igmp_snooping )
		return;

	spin_lock_bh(&br->mcl_lock);
    for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
    {
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
        {
            if (time_after_eq(jiffies, dst->tstamp)) 
            {
            mcpd_nl_send_igmp_purge_entry(dst);
		    list_for_each_entry_safe(rep_entry, 
                            rep_entry_n, &dst->rep_list, list)     
                {
		        list_del(&rep_entry->list);
                    kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		    }
                hlist_del(&dst->hlist);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
		    br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)dst);
#endif
                kmem_cache_free(br_igmp_mc_fdb_cache, dst);
            }
	    }
	}
	spin_unlock_bh(&br->mcl_lock);
		
	mod_timer(&br->igmp_timer, jiffies + TIMER_CHECK_TIMEOUT*HZ);		
}

static struct net_bridge_mc_rep_entry *
                br_igmp_rep_find(const struct net_bridge_mc_fdb_entry *mc_fdb,
                                 const struct in_addr *rep)
{
	struct net_bridge_mc_rep_entry *rep_entry;
	
	list_for_each_entry(rep_entry, &mc_fdb->rep_list, list)
	{
	    if(rep_entry->rep.s_addr == rep->s_addr)
		return rep_entry;
	}

	return NULL;
}


// 本函数返回1表示存在更新项
static int br_mc_fdb_update(struct net_bridge *br, 
                            struct net_bridge_port *prt, 
                            struct in_addr *grp, 
                            struct in_addr *rep, 
                            int mode, 
                            struct in_addr *src,
                            struct net_device *from_dev)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	int ret = 0;
	int filt_mode;
	struct hlist_head *head;
	struct hlist_node *h;

	if(mode == SNOOP_IN_ADD)
		filt_mode = MCAST_INCLUDE;
	else
		filt_mode = MCAST_EXCLUDE;
    
	head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];

    // 遍历所有的dst链表，查找是否对应的节点已经存在，注意认为一个节点存在需要满足如下要求:
    // 1 同一个组播地址，2 该节点的过滤模式一样，并且针对同一个源做过滤，并且要是同一个wan,而且是同一个LAN接口
	hlist_for_each_entry(dst, h, head, hlist) {
        // 首先 必须是同一个组
		if (dst->grp.s_addr == grp->s_addr)
		{
			if((src->s_addr == dst->src_entry.src.s_addr) &&
			   (filt_mode == dst->src_entry.filt_mode) && 
			   (dst->from_dev == from_dev) &&
			   (dst->dst == prt))
			{
				/* found entry - update TS */
				dst->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT*HZ;
				if(!br_igmp_rep_find(dst, rep))
				{
					rep_entry = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_ATOMIC);
					if(rep_entry)
					{
						rep_entry->rep.s_addr = rep->s_addr;
						list_add_tail(&rep_entry->list, &dst->rep_list);
					}
				}
				ret = 1;
			}
#if defined(CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH)
			/* patch for igmp report flooding by robo */
			else if ((0 == dst->src_entry.src.s_addr) &&
			         (MCAST_EXCLUDE == dst->src_entry.filt_mode)) {
				dst->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT*HZ;
			}
#endif /* CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH*/
		}
	}

	return ret;
}

#if 0
static struct net_bridge_mc_fdb_entry *br_mc_fdb_get(struct net_bridge *br, 
                                                     struct net_bridge_port *prt, 
                                                     struct in_addr *grp, 
                                                     struct in_addr *rep, 
                                                     int mode, 
                                                     struct in_addr *src,
                                                     struct net_device *from_dev)
{
    struct net_bridge_mc_fdb_entry *dst;
	int filt_mode;
	struct hlist_head *head;
	struct hlist_node *h;
    
	if(mode == SNOOP_IN_CLEAR)
        filt_mode = MCAST_INCLUDE;
	else
        filt_mode = MCAST_EXCLUDE;
          
	spin_lock_bh(&br->mcl_lock);
   head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	hlist_for_each_entry(dst, h, head, hlist) {
	    if ((dst->grp.s_addr == grp->s_addr) && 
            (br_igmp_rep_find(dst, rep)) &&
            (filt_mode == dst->src_entry.filt_mode) && 
            (dst->src_entry.src.s_addr == src->s_addr) &&
            (dst->from_dev == from_dev) &&
            (dst->dst == prt)) 
        {
            spin_unlock_bh(&br->mcl_lock);
            return dst;
        }
    }
	spin_unlock_bh(&br->mcl_lock);
	
	return NULL;
}
#endif

int br_igmp_process_if_change(struct net_bridge *br, struct net_device *ndev)
{
	struct net_bridge_mc_fdb_entry *dst;
	int i;

	spin_lock_bh(&br->mcl_lock);
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
		{
			if ((NULL == ndev) ||
			    (dst->dst->dev == ndev) ||
			    (dst->from_dev == ndev))
			{
				mcpd_nl_send_igmp_purge_entry(dst);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
				br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)dst);
#endif
				br_igmp_mc_fdb_del_entry(br, dst);
			}
		}
	}
	spin_unlock_bh(&br->mcl_lock);

	return 0;
}

// 往组播转发表中登记一个，wan_ops为 MCPD_IF_TYPE_BRIDGED 或者 MCPD_IF_TYPE_ROUTED
int br_igmp_mc_fdb_add(struct net_device *from_dev,
                       int wan_ops,
                       struct net_bridge *br, 
                       struct net_bridge_port *prt, 
                       struct in_addr *grp, 
                       struct in_addr *rep, 
                       int mode, 
                       int tci, 
                       struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *mc_fdb = NULL;
   struct net_bridge_mc_rep_entry *rep_entry = NULL;
   struct hlist_head *head = NULL;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
    int ret = 1;
#endif
#if defined(CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH)
	struct net_bridge_mc_fdb_entry *mc_fdb_robo, *mc_fdb_robo_n;
#endif /* CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH */

	//printk("--- add mc entry ---\n");

	if(!br || !prt || !grp|| !rep || !from_dev)
		return 0;

	if(!br_igmp_control_filter(NULL, grp->s_addr))
		return 0;

	if(!netdev_path_is_leaf(from_dev))
		return 0;

	if((SNOOP_IN_ADD != mode) && (SNOOP_EX_ADD != mode))
		return 0;

	if(grp->s_addr == ip_upnp_addr.s_addr)
		return 0;
		
	mc_fdb = kmem_cache_alloc(br_igmp_mc_fdb_cache, GFP_KERNEL);
	if ( !mc_fdb )
	{
		return -ENOMEM;
	}
	rep_entry = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_KERNEL);
	if ( !rep_entry )
	{
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		return -ENOMEM;
	}

	spin_lock_bh(&br->mcl_lock);
	if (br_mc_fdb_update(br, prt, grp, rep, mode, src, from_dev))
	{
	    // 是否该节点已经存在，如果已经存在那么就更新即可
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mcl_lock);
		return 0;
	}


#if defined(CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH)
	/* patch for snooping entry when LAN client access port is moved & 
	   igmp report flooding by robo */
	head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	hlist_for_each_entry(mc_fdb_robo, h, head, hlist) {
		if ((mc_fdb_robo->grp.s_addr == grp->s_addr) &&
		    (0 == mc_fdb_robo->src_entry.src.s_addr) &&
		    (MCAST_EXCLUDE == mc_fdb_robo->src_entry.filt_mode) && 
		    (br_igmp_rep_find(mc_fdb_robo, rep)) &&
		    (mc_fdb_robo->dst != prt)) {
			list_del(&mc_fdb_robo->list);
			kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb_robo);
		}
	}
#endif /* CONFIG_BR_IGMP_SNOOP_SWITCH_PATCH */

	mc_fdb->grp.s_addr = grp->s_addr;
	memcpy(&mc_fdb->src_entry, src, sizeof(struct in_addr));
	mc_fdb->src_entry.filt_mode = (mode == SNOOP_IN_ADD) ? MCAST_INCLUDE : MCAST_EXCLUDE;
	mc_fdb->dst = prt;
	mc_fdb->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT * HZ;
	mc_fdb->lan_tci = tci;
	mc_fdb->wan_tci = 0;
	mc_fdb->num_tags = 0;
	mc_fdb->from_dev = from_dev;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	mc_fdb->type = wan_ops;
	mc_fdb->root = 1;
	mc_fdb->blog_idx = BLOG_KEY_INVALID;
#endif
	memcpy(mc_fdb->wan_name, from_dev->name, IFNAMSIZ);
	memcpy(mc_fdb->lan_name, prt->dev->name, IFNAMSIZ);
	INIT_LIST_HEAD(&mc_fdb->rep_list);
	rep_entry->rep.s_addr = rep->s_addr;
	list_add_tail(&rep_entry->list, &mc_fdb->rep_list);

	head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	hlist_add_head(&mc_fdb->hlist, head);

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	ret = br_mcast_blog_process(br, (void*)mc_fdb, BR_MCAST_PROTO_IGMP);
	if(ret < 0)
	{
		hlist_del(&mc_fdb->hlist);
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mcl_lock);
		return ret;
	}
#endif
	spin_unlock_bh(&br->mcl_lock);

	if (!br->start_timer) {
		init_timer(&br->igmp_timer);
		br->igmp_timer.expires = jiffies + TIMER_CHECK_TIMEOUT*HZ;
		br->igmp_timer.function = br_igmp_query_timeout;
		br->igmp_timer.data = (unsigned long) br;
		add_timer(&br->igmp_timer);
		br->start_timer = 1;
	}

	return 1;
}

void br_igmp_mc_fdb_cleanup(struct net_bridge *br)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge_mc_rep_entry *rep_entry, *rep_entry_n;
    int i;
    
	spin_lock_bh(&br->mcl_lock);
    for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
    {
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
        {
	    list_for_each_entry_safe(rep_entry, 
	                             rep_entry_n, &dst->rep_list, list) {
            list_del(&rep_entry->list);
                kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
	    }
            hlist_del(&dst->hlist);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG) 
	    br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)dst);
#endif
            kmem_cache_free(br_igmp_mc_fdb_cache, dst);
        }
	}
	spin_unlock_bh(&br->mcl_lock);
}

void br_igmp_mc_fdb_remove_grp(struct net_bridge *br, 
                               struct net_bridge_port *prt, 
                               struct in_addr *grp)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge_mc_rep_entry *rep_entry, *rep_entry_n;
	struct hlist_head *head = NULL;
    struct hlist_node *h, *n;

    if(!br || !prt || !grp)
        return;

	spin_lock_bh(&br->mcl_lock);
   head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	hlist_for_each_entry_safe(dst, h, n, head, hlist) {
	    if ((dst->grp.s_addr == grp->s_addr) && 
           (dst->dst == prt))
       {
		list_for_each_entry_safe(rep_entry, 
	                             rep_entry_n, &dst->rep_list, list) {
		    list_del(&rep_entry->list);
		        kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		}
            hlist_del(&dst->hlist);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG) 
		br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)dst);
#endif
            kmem_cache_free(br_igmp_mc_fdb_cache, dst);
	    }
	}
	spin_unlock_bh(&br->mcl_lock);
}

int br_igmp_mc_fdb_remove(struct net_device *from_dev,
                          struct net_bridge *br, 
                          struct net_bridge_port *prt, 
                          struct in_addr *grp, 
                          struct in_addr *rep, 
                          int mode, 
                          struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *mc_fdb;
	struct net_bridge_mc_rep_entry *rep_entry, *rep_entry_n;
	int filt_mode;
	struct hlist_head *head = NULL;
    struct hlist_node *h, *n;

	//printk("--- remove mc entry ---\n");
	
	if(!br || !prt || !grp|| !rep || !from_dev)
	    return 0;

    if(!br_igmp_control_filter(NULL, grp->s_addr))
        return 0;

	if(!netdev_path_is_leaf(from_dev))
	    return 0;

	if((SNOOP_IN_CLEAR != mode) && (SNOOP_EX_CLEAR != mode))
	   return 0;

	if(mode == SNOOP_IN_CLEAR)
        filt_mode = MCAST_INCLUDE;
	else
        filt_mode = MCAST_EXCLUDE;

    spin_lock_bh(&br->mcl_lock);
    head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	 hlist_for_each_entry_safe(mc_fdb, h, n, head, hlist)
    {
	    if ((mc_fdb->grp.s_addr == grp->s_addr) && 
            (filt_mode == mc_fdb->src_entry.filt_mode) && 
            (mc_fdb->src_entry.src.s_addr == src->s_addr) &&
            (mc_fdb->from_dev == from_dev) &&
            (mc_fdb->dst == prt)) 
       {
            list_for_each_entry_safe(rep_entry, 
	                                  rep_entry_n, &mc_fdb->rep_list, list) 
            {
		          if(rep_entry->rep.s_addr == rep->s_addr)
                {
                    list_del(&rep_entry->list);
                    kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
                }
	        }
	        if(list_empty(&mc_fdb->rep_list)) 
           {
                hlist_del(&mc_fdb->hlist);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG) 
                br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)mc_fdb);
#endif
                kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
	        }
        }
	}
    spin_unlock_bh(&br->mcl_lock);
	
	return 0;
}


// 如果是组播报文，走到这里进行转发处理
int br_igmp_mc_forward(struct net_bridge *br, 
                       struct sk_buff *skb, 
                       int forward,
                       int is_routed)
{
	struct net_bridge_mc_fdb_entry *dst;
	int status = 0;
	struct sk_buff *skb2;
	struct net_bridge_port *p, *p_n;
	struct iphdr *pip = ip_hdr(skb);
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct hlist_head *head = NULL;
	struct hlist_node *h;

	if(eth_hdr(skb)->h_proto != ETH_P_IP)
		return status;

	if ((pip->protocol == IPPROTO_IGMP))//  &&
		// (br->igmp_proxy || br->igmp_snooping))
	{
		/* for bridged WAN service, do not pass any IGMP packets
		   coming from the WAN port to mcpd. Queries can be passed 
		   through for forwarding, other types should be dropped */
		if ( skb->dev->priv_flags & IFF_WANDEV )
		{
			unsigned char igmp_type;
			if(pip->ihl == 5)
			{
				igmp_type = skb->data[20];
			}
			else
			{
				igmp_type = skb->data[24];
			}
			if ( igmp_type != IGMP_HOST_MEMBERSHIP_QUERY )
			{
				kfree_skb(skb);
				status = 1;
			}
		}
		else
		{
            // 确保报文来自于LAN侧
			if(skb->dev && (skb->dev->br_port)) 
			{ 
#if 1
                br_igmp_process(br, skb);
#else
				//mcpd_process_skb(br, skb);
#endif
			}
		}
		return status;
	}

	if (!br->igmp_snooping)
		return status;

	/* drop traffic by default when snooping is enabled
	   in blocking mode */
	if ((br->igmp_snooping == SNOOPING_BLOCKING_MODE) && 
		 br_igmp_control_filter(dest, pip->daddr))
	{
		status = 1;
	}

	spin_lock_bh(&br->mcl_lock);
	head = &br->mc_hash[br_igmp_mc_fdb_hash(pip->daddr)];
    // 遍历所有的LAN侧组播地址
	hlist_for_each_entry(dst, h, head, hlist) {
	    // 如果snooping中对应的表项一致
		if (dst->grp.s_addr == pip->daddr) {
            // 这里应该算是做的稍微好点的处理了，控制更加精细化了，如果是INCLUDE模式那么还要求
            // 对应的设备需要满足对应的源
			if((dst->src_entry.filt_mode == MCAST_INCLUDE) && 
				(pip->saddr == dst->src_entry.src.s_addr)) {
				if (!dst->dst->dirty) {
					if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
					{
						return 0;
					}
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
					blog_clone(skb, blog_ptr(skb2));
#endif
					if(forward)
						br_forward(dst->dst, skb2);
					else
						br_deliver(dst->dst, skb2);
				}
				dst->dst->dirty = 1;
				status = 1;
			}
			else if(dst->src_entry.filt_mode == MCAST_EXCLUDE) {
                // 如果是EXCLUDE模式，那么需要确保源不在EXCLUDE模式之中
				if((0 == dst->src_entry.src.s_addr) ||
					(pip->saddr != dst->src_entry.src.s_addr)) {
					if (!dst->dst->dirty) {
						if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
						{
							return 0;
						}
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
						blog_clone(skb, blog_ptr(skb2));
#endif
						if(forward)
							br_forward(dst->dst, skb2);
						else
							br_deliver(dst->dst, skb2);
					}
					dst->dst->dirty = 1;
					status = 1;
				}
				else if(pip->saddr == dst->src_entry.src.s_addr) {
					status = 1;
				}
			}
		}
	}
	spin_unlock_bh(&br->mcl_lock);

	if (status) {
		list_for_each_entry_safe(p, p_n, &br->port_list, list) {
			p->dirty = 0;
		}
	}

	if(status)
		kfree_skb(skb);

	return status;
}

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
int br_igmp_mc_fdb_update_bydev( struct net_bridge *br,
                                 struct net_device *dev )
{
	struct net_bridge_mc_fdb_entry *mc_fdb;
	int ret;
	int i;

	if(!br || !dev)
		return 0;

	if(!netdev_path_is_leaf(dev))
		return 0;

	spin_lock_bh(&br->mcl_lock);
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(mc_fdb, h, n, &br->mc_hash[i], hlist) 
		{
			if ((mc_fdb->dst->dev == dev) ||
		    	(mc_fdb->from_dev == dev))
			{
				br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)mc_fdb);
				/* do note remove the root entry */
				if (0 == mc_fdb->root)
				{
                br_igmp_mc_fdb_del_entry(br, mc_fdb);
				}
			}
		}
	}

	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(mc_fdb, h, n, &br->mc_hash[i], hlist) 
		{ 
			if ( (1 == mc_fdb->root) && 
			     ((mc_fdb->dst->dev == dev) ||
			      (mc_fdb->from_dev == dev)) )
			{
				mc_fdb->wan_tci  = 0;
				mc_fdb->num_tags = 0; 
				ret = br_mcast_blog_process(br, (void*)mc_fdb, BR_MCAST_PROTO_IGMP);
				if(ret < 0)
				{
                /* br_mcast_blog_process may return -1 if there are no blog rules
                 * which may be a valid scenario, in which case we delete the
                 * multicast entry.
                 */
                br_igmp_mc_fdb_del_entry(br, mc_fdb);
//              printk(KERN_DEBUG "%s: Failed to create the blog\n", __FUNCTION__);
				}
			}
		}
	}
	spin_unlock_bh(&br->mcl_lock);

	return 0;
}

/* This is a support function for vlan/blog processing that requires that 
   br->mcl_lock is already held */
struct net_bridge_mc_fdb_entry *br_igmp_mc_fdb_copy(
                       struct net_bridge *br, 
                       const struct net_bridge_mc_fdb_entry *igmp_fdb)
{
	struct net_bridge_mc_fdb_entry *new_igmp_fdb = NULL;
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	struct net_bridge_mc_rep_entry *rep_entry_n = NULL;
	int success = 1;
	struct hlist_head *head = NULL;

	new_igmp_fdb = kmem_cache_alloc(br_igmp_mc_fdb_cache, GFP_ATOMIC);
	if (new_igmp_fdb)
	{
		memcpy(new_igmp_fdb, igmp_fdb, sizeof(struct net_bridge_mc_fdb_entry));
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
		new_igmp_fdb->blog_idx = BLOG_KEY_INVALID;
#endif
		new_igmp_fdb->root = 0;
		INIT_LIST_HEAD(&new_igmp_fdb->rep_list);

		list_for_each_entry(rep_entry, &igmp_fdb->rep_list, list) {
			rep_entry_n = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_ATOMIC);
			if(rep_entry_n)
			{
				memcpy(rep_entry_n, 
				       rep_entry, 
				       sizeof(struct net_bridge_mc_rep_entry));
				list_add_tail(&rep_entry_n->list, &new_igmp_fdb->rep_list);
			}
			else 
			{
				success = 0;
				break;
			}
		}

		if(success)
		{
			head = &br->mc_hash[br_igmp_mc_fdb_hash(igmp_fdb->grp.s_addr)];
			hlist_add_head(&new_igmp_fdb->hlist, head);
		}
		else
		{
			list_for_each_entry_safe(rep_entry, 
			                         rep_entry_n, &new_igmp_fdb->rep_list, list) {
				list_del(&rep_entry->list);
				kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
			}
			kmem_cache_free(br_igmp_mc_fdb_cache, new_igmp_fdb);
			new_igmp_fdb = NULL;
		}
	}

	return new_igmp_fdb;
} /* br_igmp_mc_fdb_copy */
#endif

/* This function requires that br->mcl_lock is already held */
void br_igmp_mc_fdb_del_entry(struct net_bridge *br, 
                              struct net_bridge_mc_fdb_entry *igmp_fdb)
{
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	struct net_bridge_mc_rep_entry *rep_entry_n = NULL;

	list_for_each_entry_safe(rep_entry, 
	                         rep_entry_n, &igmp_fdb->rep_list, list) {
		list_del(&rep_entry->list);
		kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
	}
	hlist_del(&igmp_fdb->hlist);
	kmem_cache_free(br_igmp_mc_fdb_cache, igmp_fdb);

	return;
}

static void *snoop_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct net_device *dev;
	loff_t offs = 0;

    read_lock(&dev_base_lock);
	for_each_netdev(&init_net, dev)
    {
		if ((dev->priv_flags & IFF_EBRIDGE) &&
            (*pos == offs)) { 
            return dev;
		}
	}
	++offs;
	return NULL;
}

static void *snoop_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net_device *dev = v;

	++*pos;
	
	for(dev = next_net_device(dev); dev; dev = next_net_device(dev)) {
		if(dev->priv_flags & IFF_EBRIDGE)
			return dev;
        }
	return NULL;
}

#define MAX_IP_LEN  16
static void inet_ntoa( unsigned int saddr, char * acAddr)
{
    snprintf(acAddr,MAX_IP_LEN,"%u.%u.%u.%u",(saddr&0xff000000)>>24,(saddr&0x00ff0000)>>16,
        (saddr&0x0000ff00)>>8,(saddr&0x000000ff)>>0);
    return;
}

/*此格式影响DT的工程页面解析，请慎重修改*/
static int snoop_seq_show(struct seq_file *seq, void *v)
{
	struct net_device *dev = v;
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_mc_rep_entry *rep_entry;
	int first;
	int i;
	int tstamp;
    char GrpAddr[MAX_IP_LEN];
    char SrcAddr[MAX_IP_LEN];
    char RptAddr[MAX_IP_LEN];

	seq_printf(seq, "igmp snooping %d  proxy %d  lan2lan-snooping %d, rate-limit %dpps\n", 
	           br->igmp_snooping, 
	           br->igmp_proxy,
	           br_igmp_lan2lan_snooping,
              br->igmp_rate_limit);
	seq_printf(seq, "bridge device src-dev    #tags lan-tci    wan-tci");
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	seq_printf(seq, "    group           mode source          timeout reporter        Index\n");
#else
	seq_printf(seq, "    group           mode source          timeout reporter");
#endif

	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h;
		hlist_for_each(h, &br->mc_hash[i]) 
		{
			dst = hlist_entry(h, struct net_bridge_mc_fdb_entry, hlist);
			if(dst)
	{
		seq_printf(seq, "%-6s %-6s %-10s %02d    0x%08x 0x%08x", 
		           br->dev->name, 
		           dst->dst->dev->name, 
		           dst->from_dev->name, 
		           dst->num_tags,
		           dst->lan_tci,
		           dst->wan_tci);

		#if  0
		seq_printf(seq, " 0x%08x", dst->grp.s_addr);
          #else
              memset(GrpAddr,0,sizeof(GrpAddr));
              inet_ntoa(dst->grp.s_addr,GrpAddr);
              seq_printf(seq, " %-15s", GrpAddr);
		#endif /* #if 0 */

		if ( 0 == br->igmp_snooping )
		{
			tstamp = 0;
		}
		else
		{
			tstamp = (int)(dst->tstamp - jiffies) / HZ;
		}
        #if 0
		seq_printf(seq, " %-4s 0x%08x %-7d", 
		           (dst->src_entry.filt_mode == MCAST_EXCLUDE) ? 
		           "EX" : "IN", dst->src_entry.src.s_addr, 
		           tstamp);
        #else
        memset(SrcAddr,0,sizeof(SrcAddr));
        inet_ntoa(dst->src_entry.src.s_addr,SrcAddr);
    	seq_printf(seq, " %-4s %-15s %-7d", 
	           (dst->src_entry.filt_mode == MCAST_EXCLUDE) ? 
	           "EX" : "IN", dst->src_entry.src.s_addr, 
	           tstamp);
        #endif

		first = 1;
		list_for_each_entry(rep_entry, &dst->rep_list, list)
		{ 
			if(first)
			{
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
				#if  0
				seq_printf(seq, " 0x%08x 0x%08x\n", rep_entry->rep.s_addr, dst->blog_idx);
                #else
                    memset(RptAddr,0,sizeof(RptAddr));
                    inet_ntoa(rep_entry->rep.s_addr,RptAddr);
                    seq_printf(seq, " %-15s 0x%08x\n", RptAddr, dst->blog_idx);
				#endif /* #if 0 */
#else
				#if  0
				seq_printf(seq, " 0x%08x\n", rep_entry->rep.s_addr);
                #else
                    memset(RptAddr,0,sizeof(RptAddr));
                    inet_ntoa(rep_entry->rep.s_addr,RptAddr);
                    seq_printf(seq, " %-15s\n", RptAddr);
				#endif /* #if 0 */
#endif
				first = 0;
			}
			else
			{
				#if  0
				seq_printf(seq, "%84s 0x%08x\n", " ", rep_entry->rep.s_addr);
                #else
                    memset(RptAddr,0,sizeof(RptAddr));
                    inet_ntoa(rep_entry->rep.s_addr,RptAddr);
                    seq_printf(seq, "%97s %s\n", " ", RptAddr);
				#endif /* #if 0 */
			}
		}
	}
		}
	}

	return 0;
}

static void snoop_seq_stop(struct seq_file *seq, void *v)
{
    read_unlock(&dev_base_lock);
}

static struct seq_operations snoop_seq_ops = {
	.start = snoop_seq_start,
	.next  = snoop_seq_next,
	.stop  = snoop_seq_stop,
	.show  = snoop_seq_show,
};

static int snoop_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &snoop_seq_ops);
}

static struct file_operations br_igmp_snoop_proc_fops = {
	.owner = THIS_MODULE,
	.open  = snoop_seq_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

void br_igmp_mc_rep_free(struct net_bridge_mc_rep_entry *rep)
{
    kmem_cache_free(br_igmp_mc_rep_cache, rep);

    return;
}

void br_igmp_mc_fdb_free(struct net_bridge_mc_fdb_entry *mc_fdb)
{
    kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);

    return;
}

int __init br_igmp_snooping_init(void)
{
	br_igmp_entry = proc_create("igmp_snooping", 0, init_net.proc_net,
			   &br_igmp_snoop_proc_fops);

	if(!br_igmp_entry) {
		printk("error while creating igmp_snooping proc\n");
        return -ENOMEM;
	}

	br_igmp_mc_fdb_cache = kmem_cache_create("bridge_igmp_mc_fdb_cache",
                            sizeof(struct net_bridge_mc_fdb_entry),
                            0,
                            SLAB_HWCACHE_ALIGN, NULL);
    if (!br_igmp_mc_fdb_cache)
		return -ENOMEM;

    br_igmp_mc_rep_cache = kmem_cache_create("bridge_igmp_mc_rep_cache",
                            sizeof(struct net_bridge_mc_rep_entry),
                            0,
                            SLAB_HWCACHE_ALIGN, NULL);
    if (!br_igmp_mc_rep_cache)
    {
        kmem_cache_destroy(br_igmp_mc_fdb_cache);
		return -ENOMEM;
    }

	get_random_bytes(&br_igmp_mc_fdb_salt, sizeof(br_igmp_mc_fdb_salt));

    return 0;
}

void br_igmp_snooping_fini(void)
{
	kmem_cache_destroy(br_igmp_mc_fdb_cache);
	kmem_cache_destroy(br_igmp_mc_rep_cache);

    return;
}
#endif
