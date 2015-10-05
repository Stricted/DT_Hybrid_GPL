/*
*    Copyright (c) 2012 Broadcom Corporation
*    All Rights Reserved
*
<:label-BRCM:2012:DUAL/GPL:standard

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

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
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
#include <linux/if_vlan.h>
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif
#include <linux/rtnetlink.h>
#include "br_private.h"
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
#include "br_igmp.h"
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#endif


void mcast_debug(Blog_t blog)
{
    printk(">>>blog=============================<<<\r\n");
    printk("rx_phyHdr    :%d\n",blog.rx.info.phyHdr);
    printk("rx_channel   :%d\n",blog.rx.info.channel);
    printk("BCM_SWC      :%d\n",blog.rx.info.bmap.BCM_SWC);        
    printk("insert_eth   :%d\n",blog.insert_eth);            
    printk("pop_pppoa    :%d\n",blog.pop_pppoa);    
    printk("tuple_phy    :%d\n",blog.key.l1_tuple.phy);    
    printk("tuple_channel:%d\n",blog.key.l1_tuple.channel);        
    printk("key_protocol :%d\n",blog.key.protocol);            
    printk("tx_phyHdr    :%d\n",blog.tx.info.phyHdr);    
    printk("tx_channel   :%d\n",blog.tx.info.channel);        
    printk("rx_saddr     :%x\n",blog.rx.tuple.saddr);            
    printk("rx_daddr     :%x\n",blog.rx.tuple.daddr);                
    printk("tx_saddr     :%x\n",blog.tx.tuple.saddr);            
    printk("tx_daddr     :%x\n",blog.tx.tuple.daddr);
    if (blog.rx.dev_p)
        printk("rx_dev       :%s\n",((struct net_device *)(blog.rx.dev_p))->name);                    
    printk("rx_multicast :%d\n",blog.rx.info.multicast);                        
    if (blog.tx.dev_p)
        printk("tx_dev       :%s\n",((struct net_device *)(blog.tx.dev_p))->name);                    
    printk("vtag_num     :%x\n",blog.vtag_num);    
    printk("vid          :%x\n",blog.vid);        
    printk(">>>blog end=======================<<<<<<\r\n");    
}


#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)


// 更具IPv4地址映射为以太网组播地址
inline void br_mcast_ipv4_to_eth(unsigned long ipv4_addr,
                                       unsigned char *mac_addr_p)
{
    unsigned char *ipv4_addr8_p = (unsigned char *)(&ipv4_addr);

    mac_addr_p[0] = 0x01;
    mac_addr_p[1] = 0x00;
    mac_addr_p[2] = 0x5E;
    mac_addr_p[3] = ipv4_addr8_p[1] & 0x7F;
    mac_addr_p[4] = ipv4_addr8_p[2];
    mac_addr_p[5] = ipv4_addr8_p[3];
}

// IPv6组播地址映射为以太网MAC组播地址
inline void br_mcast_ipv6_to_eth(unsigned char *ipv6_addr,
                                       unsigned char *mac_addr_p)
{
    mac_addr_p[0] = 0x33;
    mac_addr_p[1] = 0x33;
    mac_addr_p[2] = *(ipv6_addr + 13);
    mac_addr_p[3] = *(ipv6_addr + 14);
    mac_addr_p[4] = *(ipv6_addr + 15);
    mac_addr_p[5] = *(ipv6_addr + 16);
}


// 释放对应的组播表项，当组里面没有对应的成员的时候使用，proto字段为 BR_MCAST_PROTO_MLD 或者 BR_MCAST_PROTO_IGMP
// 本函数当在IGMP snooping删除对应的组播成员时使用
void br_mcast_blog_release(t_BR_MCAST_PROTO_TYPE proto, void *mc_fdb)
{
	Blog_t *blog_p = BLOG_NULL;
	uint32_t blog_idx = BLOG_KEY_INVALID;
	BlogTraffic_t traffic;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
	if(proto == BR_MCAST_PROTO_IGMP)
	{
		blog_idx =  ((struct net_bridge_mc_fdb_entry *)mc_fdb)->blog_idx;
		((struct net_bridge_mc_fdb_entry *)mc_fdb)->blog_idx = 0;
		traffic = BlogTraffic_IPV4_MCAST;
	}
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	if(proto == BR_MCAST_PROTO_MLD)
	{
		blog_idx =  ((struct net_br_mld_mc_fdb_entry *)mc_fdb)->blog_idx;
		((struct net_br_mld_mc_fdb_entry *)mc_fdb)->blog_idx = 0;
		traffic = BlogTraffic_IPV6_MCAST;
	}
#endif

	if(BLOG_KEY_INVALID == blog_idx)
		return;

	blog_p = blog_deactivate(blog_idx, traffic);
	if ( blog_p )
	{
		blog_rule_free_list(blog_p);
		blog_put(blog_p);
	}

	return;
}

// 本函数在当需要往组播转发表中增加一项的时候才使用，wan_ops用来表示是路由还是桥接
static void br_mcast_blog_process_wan(blogRule_t *rule_p,
                                     void *mc_fdb,
                                     t_BR_MCAST_PROTO_TYPE proto,
                                     struct net_device **wan_dev_pp,
                                     struct net_device **wan_vlan_dev_pp)
{
	blogRuleAction_t ruleAction;
	struct net_device *dev_p = NULL;
	struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif
	uint8_t                *dev_addr = NULL;
	uint32_t phyType;
	char wan_ops;

	if(!mc_fdb)
		return;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	if(BR_MCAST_PROTO_MLD == proto)
	{
		mld_fdb  = (struct net_br_mld_mc_fdb_entry *)mc_fdb;
		dev_p    = mld_fdb->from_dev;
		dev_addr = mld_fdb->dst->dev->dev_addr;
		wan_ops  = mld_fdb->type;
	}
	else
#endif
	{
		igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;
		dev_p    = igmp_fdb->from_dev;
		dev_addr = igmp_fdb->dst->dev->dev_addr;
		wan_ops  = igmp_fdb->type;
	}

	while(1)
	{
		if(netdev_path_is_root(dev_p))
		{
			*wan_dev_pp = dev_p;
			break;
		}

        // 如果是PPP接口报文
		if(dev_p->priv_flags & IFF_PPP)
		{
			rule_p->filter.hasPppoeHeader = 1;
			memset(&ruleAction, 0, sizeof(blogRuleAction_t));
			ruleAction.cmd = BLOG_RULE_CMD_POP_PPPOE_HDR;
			blog_rule_add_action(rule_p, &ruleAction);

			memset(&ruleAction, 0, sizeof(blogRuleAction_t));
			ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_DA;
			if(igmp_fdb)
				br_mcast_ipv4_to_eth(igmp_fdb->grp.s_addr, ruleAction.macAddr);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
			else
				br_mcast_ipv6_to_eth(mld_fdb->grp.s6_addr, ruleAction.macAddr);
#endif
			blog_rule_add_action(rule_p, &ruleAction);
		}
		else if(*wan_vlan_dev_pp == NULL &&
		        dev_p->priv_flags & IFF_BCM_VLAN)
		{
			*wan_vlan_dev_pp = dev_p;
		}
		dev_p = netdev_path_next_dev(dev_p);
	}

	/* For IPoA */
	phyType = netdev_path_get_hw_port_type(*wan_dev_pp);
	phyType = BLOG_GET_HW_ACT(phyType);
	//if((phyType == VC_MUX_IPOA) || (phyType == LLC_SNAP_ROUTE_IP))
	if((phyType == VC_MUX_IPOA) || (phyType == LLC_SNAP_ROUTE_IP) || !rule_p->filter.hasPppoeHeader)
	{
		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_DA;
		if(igmp_fdb)
			br_mcast_ipv4_to_eth(igmp_fdb->grp.s_addr, ruleAction.macAddr);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
		else
			br_mcast_ipv6_to_eth(mld_fdb->grp.s6_addr, ruleAction.macAddr);
#endif
		blog_rule_add_action(rule_p, &ruleAction);
	}

	if(wan_ops == MCPD_IF_TYPE_ROUTED)
	{
		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_SA;
		memcpy(ruleAction.macAddr, dev_addr, ETH_ALEN);
		blog_rule_add_action(rule_p, &ruleAction);

		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_DECR_TTL;
		blog_rule_add_action(rule_p, &ruleAction);
	}
}

static void br_mcast_blog_process_lan(void *mc_fdb,
                                     t_BR_MCAST_PROTO_TYPE proto,
                                     struct net_device **lan_dev_pp,
                                     struct net_device **lan_vlan_dev_pp)
{
    struct net_device *dev_p = NULL;
    struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
    struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif

    if(!mc_fdb)
        return;

    if(BR_MCAST_PROTO_IGMP == proto)
    {
        igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;

        dev_p = igmp_fdb->dst->dev;
    }
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
    else
    {
        mld_fdb = (struct net_br_mld_mc_fdb_entry *)mc_fdb;

        dev_p = mld_fdb->dst->dev;
    }
#endif

    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            *lan_dev_pp = dev_p;
            break;
        }

        if(*lan_vlan_dev_pp == NULL &&
           dev_p->priv_flags & IFF_BCM_VLAN)
        {
            *lan_vlan_dev_pp = dev_p;
        }

        dev_p = netdev_path_next_dev(dev_p);
    }
}

static void br_mcast_mc_fdb_update_bydev(t_BR_MCAST_PROTO_TYPE proto, 
                                         struct net_bridge *br,
                                         struct net_device *dev)
{
	if(!br || !dev)
		return;

	if(BR_MCAST_PROTO_IGMP == proto)
		br_igmp_mc_fdb_update_bydev(br, dev);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	else if(BR_MCAST_PROTO_MLD == proto)
		br_mld_mc_fdb_update_bydev(br, dev);
#endif

	return;
}

void br_mcast_vlan_notify_for_blog_update(struct net_device *ndev,
                                          blogRuleVlanNotifyDirection_t direction,
                                          uint32_t nbrOfTags)
{
	struct net_bridge *br = NULL;
	struct net_device *dev = NULL;

	if((ndev->priv_flags & IFF_WANDEV) && (direction == DIR_TX))
		return;

	read_lock(&dev_base_lock);
	for(dev = first_net_device(&init_net); 
	    dev; 
	    dev = next_net_device(dev)) 
	{
		br = netdev_priv(dev);
		if((dev->priv_flags & IFF_EBRIDGE) && (br))
		{
			/* snooping entries could be present even if snooping is
			   disabled, update existing entries */
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
			spin_lock_bh(&br->lock);
			br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_IGMP, br, ndev);
			spin_unlock_bh(&br->lock);
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
			spin_lock_bh(&br->lock);
			br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_MLD, br, ndev);
			spin_unlock_bh(&br->lock);
#endif /* CONFIG_BR_MLD_SNOOP */
		}
	}
	read_unlock(&dev_base_lock);

	return;
}

void br_mcast_handle_netdevice_events(struct net_device *ndev, unsigned long event)
{
    struct net_bridge *br = NULL;
    struct net_device *dev = NULL;
	int i;

    switch (event) {
        case NETDEV_GOING_DOWN:
        case NETDEV_CHANGE:
            for(dev = first_net_device(&init_net); 
                dev; 
                dev = next_net_device(dev)) {
                br = netdev_priv(dev);
                if((dev->priv_flags & IFF_EBRIDGE) && (br)) {
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
                    struct net_br_mld_mc_fdb_entry *mld_dst;
                    struct net_br_mld_mc_rep_entry *mldrep, *mldrep_n;
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
                    /* snooping entries could be present even if snooping is
                       disabled, update existing entries */
                    struct net_bridge_mc_fdb_entry *dst;
                    struct net_bridge_mc_rep_entry *rep_entry, *rep_entry_n;
                    spin_lock_bh(&br->lock);
                    spin_lock_bh(&br->mcl_lock);
                    for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
                    {
                        struct hlist_node *h, *n;
                        hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
                        {
                    if((!memcmp(ndev->name, dst->wan_name, IFNAMSIZ)) ||
                        (!memcmp(ndev->name, dst->lan_name, IFNAMSIZ)) ||
                        (!memcmp(ndev->name, dev->name, IFNAMSIZ))) {
                            if ( br->igmp_snooping )
                            {
                               mcpd_nl_send_igmp_purge_entry(dst);
                            }
                            list_for_each_entry_safe(rep_entry, 
                                     rep_entry_n, &dst->rep_list, list) {
                                list_del(&rep_entry->list);
                                        br_igmp_mc_rep_free(rep_entry);
                            }
                                hlist_del(&dst->hlist);
                            br_mcast_blog_release(BR_MCAST_PROTO_IGMP, dst);
                                br_igmp_mc_fdb_free(dst);
                            }
                        }
                    }
                    spin_unlock_bh(&br->mcl_lock);
                    spin_unlock_bh(&br->lock);
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
                    /* snooping entries could be present even if snooping is
                       disabled, update existing entries */
                    spin_lock_bh(&br->lock);
                    spin_lock_bh(&br->mld_mcl_lock);
                    for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
                    {
                        struct hlist_node *h, *n;
                        hlist_for_each_entry_safe(mld_dst, h, n, &br->mld_mc_hash[i], hlist) 
                        {
                    if((!memcmp(ndev->name, mld_dst->wan_name, IFNAMSIZ)) ||
                        (!memcmp(ndev->name, mld_dst->lan_name, IFNAMSIZ)) ||
                        (!memcmp(ndev->name, dev->name, IFNAMSIZ))) {
                                list_for_each_entry_safe(mldrep, 
                                                        mldrep_n, &mld_dst->rep_list, list) {
                                    list_del(&mldrep->list);
                                    br_mld_mc_rep_free(mldrep);
                            }
                                hlist_del(&mld_dst->hlist);
                            br_mcast_blog_release(BR_MCAST_PROTO_MLD, mld_dst);
                                br_mld_mc_fdb_free(mld_dst);
                            }
                        }
                    }
                    spin_unlock_bh(&br->mld_mcl_lock);
                    spin_unlock_bh(&br->lock);
#endif
                }
            }
            break;
    }

    return;
}

static void *br_mcast_mc_fdb_copy(t_BR_MCAST_PROTO_TYPE proto,
								struct net_bridge *br, 
                                const void *mc_fdb)
{
    if(!mc_fdb)
        return NULL;

    if(BR_MCAST_PROTO_IGMP == proto)
        return br_igmp_mc_fdb_copy(br, (struct net_bridge_mc_fdb_entry *)mc_fdb);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
    else if(BR_MCAST_PROTO_MLD == proto)
        return br_mld_mc_fdb_copy(br, (struct net_br_mld_mc_fdb_entry *)mc_fdb);
#endif

	return NULL;
}

static void br_mcast_mc_fdb_del_entry(t_BR_MCAST_PROTO_TYPE proto, 
                               struct net_bridge *br, 
                               void *mc_fdb)
{

    if(!mc_fdb)
        return;

    if(BR_MCAST_PROTO_IGMP == proto)
        br_igmp_mc_fdb_del_entry(br, (struct net_bridge_mc_fdb_entry *)mc_fdb);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
    else if(BR_MCAST_PROTO_MLD == proto)
        br_mld_mc_fdb_del_entry(br, (struct net_br_mld_mc_fdb_entry *)mc_fdb);
#endif

    return;
} /* br_mcast_mc_fdb_del_entry */

static void br_mcast_blog_link_devices(Blog_t *blog_p, struct net_device *rxDev, 
                                       struct net_device *txDev, int wanType )
{
    struct net_device *dev_p;

    dev_p = rxDev;
    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            break;
        }

        blog_link(IF_DEVICE, blog_p, dev_p, DIR_RX, blog_p->tx.pktlen);
        dev_p = netdev_path_next_dev(dev_p);
    }

    if ( (wanType == MCPD_IF_TYPE_ROUTED) && (txDev->br_port) )
    {
       /* routed packets will come through br_dev_xmit, link bridge
          device with blog */
       blog_link(IF_DEVICE, blog_p, txDev->br_port->br->dev, 
                 DIR_TX, blog_p->tx.pktlen);
    }

    dev_p = txDev;
    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            break;
        }

        blog_link(IF_DEVICE, blog_p, dev_p, DIR_TX, blog_p->tx.pktlen);
        dev_p = netdev_path_next_dev(dev_p);
    }
}


static int br_mcast_vlan_process(struct net_bridge     *br,
                                 void                  *mc_fdb,
                                 t_BR_MCAST_PROTO_TYPE  proto,
                                 Blog_t                *blog_p)
{
    Blog_t           *new_blog_p;
    void             *new_mc_fdb = NULL;
    blogRule_t       *rule_p = NULL;
    int               firstRule = 1;
    uint32_t          vid = 0;
    blogRuleFilter_t *rule_filter = NULL;
    BlogTraffic_t     traffic;
    int               activates = 0;
    void             *rxDev;
    void             *txDev;
    int               wanType;

    if(!mc_fdb || !blog_p || !br)
        return 0;

    if(!((BR_MCAST_PROTO_IGMP == proto) || (BR_MCAST_PROTO_MLD == proto)))
        return 0;

    firstRule = 1;
    rule_p = (blogRule_t *)blog_p->blogRule_p;
    while( rule_p )
    {
        blogRuleFilter_t *filter_p;

        filter_p = &rule_p->filter;
        /* if there is a rule that specifies a protocol filter that does not match
           blog key protocol skip it */
        if(blog_rule_filterInUse(filter_p->ipv4.mask.ip_proto))
        {
            if(filter_p->ipv4.mask.ip_proto & BLOG_RULE_IP_PROTO_MASK)
            {
                uint8_t proto;

                proto = filter_p->ipv4.value.ip_proto >> BLOG_RULE_IP_PROTO_SHIFT;
                if (proto != blog_p->key.protocol)
                {
                    /* skip this rule */
                    blog_p->blogRule_p = rule_p->next_p;
                    blog_rule_free(rule_p);
                    rule_p = blog_p->blogRule_p;
                    continue;
                }
            }
        }

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
        if(blog_rule_filterInUse(filter_p->ipv6.mask.nxtHdr))
        {
            if(filter_p->ipv6.mask.nxtHdr & BLOG_RULE_IP6_NXT_HDR_MASK)
            {
                uint8_t nxtHdr;

                nxtHdr = filter_p->ipv6.value.nxtHdr >> BLOG_RULE_IP6_NXT_HDR_SHIFT;
                if (nxtHdr != blog_p->key.protocol)
                {
                    /* skip this rule */
                    blog_p->blogRule_p = rule_p->next_p;
                    blog_rule_free(rule_p);
                    rule_p = blog_p->blogRule_p;
                    continue;
                }
            }
        }
#endif

        /* create new fdb entry unless this is the first rule. For the
           first rule use the fdb entry that was passed in */
        if ( 1 == firstRule )
        {
            firstRule  = 0;
            new_mc_fdb = mc_fdb;
        }
        else
        {
            new_mc_fdb = br_mcast_mc_fdb_copy(proto, br , mc_fdb);
            if(!new_mc_fdb)
            {
                printk(KERN_WARNING "%s new_mc_fdb allocation failed\n",__FUNCTION__);
                break;
            }
        }

        /* get a new blog and copy original blog */
        new_blog_p = blog_get();
        if(new_blog_p == BLOG_NULL) {
            br_mcast_mc_fdb_del_entry(proto, br, new_mc_fdb);
            break;
        }
        blog_copy(new_blog_p, blog_p);

        /* pop the rule off the original blog now that a new fdb and blog have been
           allocated. This is to ensure that all rules are freed in case of error */
        blog_p->blogRule_p = rule_p->next_p;
        rule_p->next_p = NULL;
        new_blog_p->blogRule_p = rule_p;

        rule_filter = &(((blogRule_t *)new_blog_p->blogRule_p)->filter);
        new_blog_p->vtag_num = rule_filter->nbrOfVlanTags;
        vid = ((rule_filter->vlan[0].value.h_vlan_TCI &
                rule_filter->vlan[0].mask.h_vlan_TCI) & 0xFFF);
        new_blog_p->vid = vid ? vid : 0xFFFF; 
        vid = ((rule_filter->vlan[1].value.h_vlan_TCI &
                rule_filter->vlan[1].mask.h_vlan_TCI) & 0xFFF);
        new_blog_p->vid |= vid ? (vid << 16) : 0xFFFF0000;

        blog_link(MCAST_FDB, new_blog_p, (void *)new_mc_fdb, 0, 0);

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
        if(BR_MCAST_PROTO_MLD == proto)
        {
            traffic = BlogTraffic_IPV6_MCAST;
            ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->wan_tci = new_blog_p->vid;
            ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->num_tags = new_blog_p->vtag_num;
            rxDev   = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->from_dev;
            txDev   = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->dst->dev;
            wanType = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->type;
        }
        else//IGMP
#endif
        {
            traffic = BlogTraffic_IPV4_MCAST;
            ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->wan_tci = new_blog_p->vid;
            ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->num_tags = new_blog_p->vtag_num;
            rxDev   = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->from_dev;
            txDev   = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->dst->dev;
            wanType = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->type;
        }

        br_mcast_blog_link_devices(new_blog_p, rxDev, txDev, wanType);

        if ( blog_activate(new_blog_p, traffic) == BLOG_KEY_INVALID )
        {
            blog_rule_free_list(new_blog_p);
            blog_put(new_blog_p);
            if ( new_mc_fdb != mc_fdb )
            {
               br_mcast_mc_fdb_del_entry(proto, br, new_mc_fdb);
            }
        }
        else
        {
            activates++;
        }

        /* advance to the next rule */
        rule_p = blog_p->blogRule_p;
    }

    /* Free blog. The blog will only have rules if there was an error */
    blog_rule_free_list(blog_p);
    blog_put(blog_p);

    return activates;
} /* br_mcast_vlan_process */



// br表示哪个桥，mc_fdb表示要操作的组播记录，proto表示是IGMP还是MLD，wan_ops表示对应的是路由还是桥接口
int br_mcast_blog_process(struct net_bridge *br,
                            void            *mc_fdb,
                            t_BR_MCAST_PROTO_TYPE proto)
{
	Blog_t *blog_p = BLOG_NULL;
	blogRule_t *rule_p = NULL;
	struct net_device *wan_vlan_dev_p = NULL;
	struct net_device *lan_vlan_dev_p = NULL;
	struct net_device *wan_dev_p = NULL;
	struct net_device *lan_dev_p = NULL;
	struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif
	uint8_t *dev_addr = NULL;
	struct net_device *from_dev = NULL;
	uint32_t phyType;
   int numActivates;

	if(!mc_fdb)
		return -1;

	if(BR_MCAST_PROTO_IGMP == proto)
	{
		igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;
		from_dev = igmp_fdb->from_dev;
		dev_addr = igmp_fdb->dst->dev->dev_addr;
	}
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	else
	{
		mld_fdb = (struct net_br_mld_mc_fdb_entry *)mc_fdb;
		from_dev = mld_fdb->from_dev;
		dev_addr = mld_fdb->dst->dev->dev_addr;
	}
#endif

	/* allocate blog */
	blog_p = blog_get();
	if(blog_p == BLOG_NULL) {
		printk(KERN_WARNING "%s blog_p allocation failed\n",__FUNCTION__);
		return -1;
	}

	/* allocate blog rule */
	rule_p = blog_rule_alloc();
	if(rule_p == NULL)
	{
		printk(KERN_WARNING "%s blog_rule allocation failed\n",__FUNCTION__);
		blog_put(blog_p);
		return -1;
	}

	blog_rule_init(rule_p);
	blog_p->blogRule_p = (void *)rule_p;

	/* for LAN2LAN don't do anything */
	if(br->dev != from_dev) 
	{
		/* find WAN devices */
		br_mcast_blog_process_wan(rule_p, mc_fdb, proto,
		                          &wan_dev_p, &wan_vlan_dev_p);
	}

	/* find LAN devices */
	br_mcast_blog_process_lan(mc_fdb, proto, &lan_dev_p, &lan_vlan_dev_p);

	/* for LAN2LAN don't do anything */
	if(br->dev == from_dev) 
	{
		blog_p->rx.info.phyHdr = 0;
		blog_p->rx.info.channel = 0xFF; /* for lan2lan mcast */
		blog_p->rx.info.bmap.BCM_SWC = 1;
	}
	else
	{
		phyType = netdev_path_get_hw_port_type(wan_dev_p);
        printk("FILE:%s, LINE:%d, phyType:%d\n",__FILE__,__LINE__,phyType);            
		blog_p->rx.info.phyHdr = (phyType & BLOG_PHYHDR_MASK);
		phyType = BLOG_GET_HW_ACT(phyType);

		if(blog_p->rx.info.phyHdrType == BLOG_GPONPHY)
		{
			unsigned int hw_subport_mcast;
			unsigned int mask;
			int i;

			hw_subport_mcast = netdev_path_get_hw_subport_mcast(wan_dev_p);

			if(hw_subport_mcast)
			{
				for(i=0; i<(sizeof(unsigned int) * 8); ++i)
				{
					mask = (unsigned int)(1 << i);
					if(hw_subport_mcast & mask)
					{
						/* currently we only support one GEM port (source subport) */
						blog_p->rx.info.channel = i;
						if(hw_subport_mcast & ~mask)
						{
							printk("Warning: Multiple GPON multicast GEM sources "
							       "<0x%08X>, selected <%u>\n",
							       hw_subport_mcast, blog_p->rx.info.channel);
						}
						break;
					}
				}
			}
			else
			{
				/* Not a GPON Multicast WAN device */
				blog_rule_free_list(blog_p);
				blog_put(blog_p);
				return -1;
			}
		}
		else /* Ethernet or DSL WAN device */
		{
			blog_p->rx.info.channel = netdev_path_get_hw_port(wan_dev_p);
		}

		if( (blog_p->rx.info.phyHdrType == BLOG_XTMPHY) &&
		    ((phyType == VC_MUX_PPPOA) ||
		     (phyType == VC_MUX_IPOA) ||
		     (phyType == LLC_SNAP_ROUTE_IP) ||
		     (phyType == LLC_ENCAPS_PPP)) )
		{
			blog_p->insert_eth = 1;
		}

		if( (blog_p->rx.info.phyHdrType == BLOG_XTMPHY) &&
		    ((phyType == VC_MUX_PPPOA) ||
		     (phyType == LLC_ENCAPS_PPP)) )
		{
			blog_p->pop_pppoa = 1;
		}

#if defined(CONFIG_BCM96816)
		blog_p->rx.info.bmap.BCM_SWC = 1;
#else
		if(blog_p->rx.info.phyHdrType == BLOG_ENETPHY)
		{
			blog_p->rx.info.bmap.BCM_SWC = 1;
		}
		else
		{
			blog_p->rx.info.bmap.BCM_XPHY = 1;
		}
#endif
	}

	blog_p->tx.info.bmap.BCM_SWC = 1;

	blog_p->key.l1_tuple.phy = blog_p->rx.info.phyHdr;
	blog_p->key.l1_tuple.channel = blog_p->rx.info.channel;
	blog_p->key.protocol = BLOG_IPPROTO_UDP;

	phyType = netdev_path_get_hw_port_type(lan_dev_p);
	blog_p->tx.info.phyHdr = (phyType & BLOG_PHYHDR_MASK);
	blog_p->tx.info.channel = netdev_path_get_hw_port(lan_dev_p);

	if(BR_MCAST_PROTO_IGMP == proto)
	{
		blog_p->rx.tuple.saddr = igmp_fdb->src_entry.src.s_addr;
		blog_p->rx.tuple.daddr = igmp_fdb->grp.s_addr;
		blog_p->tx.tuple.saddr = igmp_fdb->src_entry.src.s_addr;
		blog_p->tx.tuple.daddr = igmp_fdb->grp.s_addr;
	}
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	else
	{
		BCM_IN6_ASSIGN_ADDR(&blog_p->tupleV6.saddr, &mld_fdb->src_entry.src);
		BCM_IN6_ASSIGN_ADDR(&blog_p->tupleV6.daddr, &mld_fdb->grp);
	}
#endif

	blog_p->rx.dev_p = wan_dev_p;
	blog_p->rx.info.multicast = 1;

	if(BR_MCAST_PROTO_IGMP == proto)
		blog_p->rx.info.bmap.L3_IPv4 = 1;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	else
		blog_p->rx.info.bmap.L3_IPv6 = 1;
#endif

	blog_p->tx.dev_p = lan_dev_p;

	/* add vlan blog rules, if any vlan interfaces were found */
    mcast_debug(*blog_p);   
	if(blogRuleVlanHook && (wan_vlan_dev_p || lan_vlan_dev_p)) {
		if(blogRuleVlanHook(blog_p, wan_vlan_dev_p, lan_vlan_dev_p) < 0) {
			printk(KERN_WARNING "Error while processing VLAN blog rules\n");
			blog_rule_free_list(blog_p);
			blog_put(blog_p);
			return -1;
		}
	}

	/* blog must have at least one rule */
	if (NULL == blog_p->blogRule_p)
	{
		/* blogRule_p == NULL may be valid if there are no 
		   VLAN rules and the default behavior for either interface is DROP */
		//printk(KERN_WARNING "Error while processing VLAN blog rules\n");
		blog_put(blog_p);
		return -1;
	}

	numActivates = br_mcast_vlan_process(br, mc_fdb, proto, blog_p);
	if ( 0 == numActivates )
	{
		return - 1;
	}

	return 0;
} /* br_mcast_blog_process */
#endif
