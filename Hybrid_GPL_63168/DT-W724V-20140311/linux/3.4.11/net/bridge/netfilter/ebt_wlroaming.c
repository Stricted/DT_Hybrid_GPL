/*
*    Copyright (c) 2003-2012 Broadcom Corporation
*    All Rights Reserved
*
<:label-BRCM:2012:GPL/GPL:standard

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

/*
 *  ebt_wlroaming
 *
 */
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_wlroaming_t.h>
#include <linux/if_vlan.h>
#include <linux/module.h>
#include <linux/ip.h>
#if defined(CONFIG_IPV6)
#include <linux/ipv6.h>
#endif
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/inetdevice.h>
#include <linux/ethtool.h>
#include <net/arp.h>
#include "../br_private.h"
#define ROAMING_DEBUG

#ifdef ROAMING_DEBUG
	#define  dbg_printk(fmt,...)  printk(fmt,__VA_ARGS__)
#else
	#define dbg_printk(fmt,...)
#endif

int  (*ptr_fc_getmacs_match4)(uint32_t addr4,void **srcmac,void **dstmac)=NULL;
int  (*ptr_fc_flush)(void)=NULL;

EXPORT_SYMBOL(ptr_fc_getmacs_match4);
EXPORT_SYMBOL(ptr_fc_flush);
extern void fdb_delete_entry(void *entry);
extern int fdb_delete_by_addr(struct net_bridge_port *p, const u8 *addr);


static int dev_is_wl(struct net_device *dev)  {

	struct ethtool_drvinfo info;
	const struct ethtool_ops *ops = dev->ethtool_ops;

	memset(&info, 0, sizeof(info));
	info.cmd = ETHTOOL_GDRVINFO;
	if (ops && ops->get_drvinfo) {
		ops->get_drvinfo(dev, &info);
	} else if (dev->dev.parent && dev->dev.parent->driver) {
		strlcpy(info.bus_info, dev_name(dev->dev.parent),
				sizeof(info.bus_info));
		strlcpy(info.driver, dev->dev.parent->driver->name,
				sizeof(info.driver));
	} else {
		return -EOPNOTSUPP;
	}   
	if(strncmp(info.driver,"wl",2)==0) return 1; 
	else return 0;

}

static inline struct net_device *bridge_parent_roaming(const struct net_device *dev)
{
	struct net_bridge_port *port = br_port_get_rcu(dev);

	return port ? port->br->dev : NULL;
}

/* START OF MODIFY by y00181549 for lock pretect at 2014/1/24*/
static unsigned int ebt_wlroaming_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct ebt_wlroaming_t_info *info = par->targinfo;
	const struct arphdr *ah;
	struct arphdr _arph;
	__be32 *sap, *dap;
	__be32 saddr, daddr;
	const unsigned char *mp;
	unsigned char _mac[ETH_ALEN];
    struct net_bridge_fdb_entry *srcmac = NULL;
	struct net_bridge_fdb_entry *dstmac = NULL;
	struct net_device *brdev = NULL;
	__be32 gwIP = 0,searchIP = 0;
	int ret = 0;
	struct net_bridge_port *port = NULL;
	unsigned char	srcaddr[6] = {0};
	unsigned char	dstaddr[6] = {0};
    
    port = br_port_get_rcu(skb->dev);
	ah = skb_header_pointer(skb, 0, sizeof(_arph), &_arph);
	if (ah == NULL)
		return EBT_CONTINUE;

	if (ah->ar_op == htons(ARPOP_REQUEST) && dev_is_wl(skb->rxdev)==1) {
		if (ah->ar_pln != sizeof(__be32) || ah->ar_pro != htons(ETH_P_IP))
			return EBT_CONTINUE;
		sap = skb_header_pointer(skb, sizeof(struct arphdr) +
				ah->ar_hln, sizeof(saddr),
				&saddr);
		if (sap == NULL)
			return EBT_CONTINUE;
		dap = skb_header_pointer(skb, sizeof(struct arphdr) +
				2*ah->ar_hln+sizeof(saddr),
				sizeof(daddr), &daddr);
		if (dap == NULL)
			return EBT_CONTINUE;

		mp = skb_header_pointer(skb, sizeof(struct arphdr), sizeof(_mac), &_mac);
		if (mp == NULL)
			return EBT_CONTINUE;

		brdev = bridge_parent_roaming(skb->dev);
		if(brdev) {
			gwIP = inet_select_addr(brdev,0,RT_SCOPE_LINK);
			if(*sap == 0) {
    			dbg_printk("Roaming:%s:%d on br:%s(%04x), dev:%s received  sap:%08x,dap:%08x ,src mac:%08x%04x\r\n",__FUNCTION__,__LINE__,brdev->name,gwIP,skb->dev->name,*sap,*dap,*((__be32 *)mp),*((__be16 *)(mp+4)));
				dbg_printk("Roaming:%s:%d  Received PROBE ARP \r\n",__FUNCTION__,__LINE__ );
				searchIP=*dap;
			} else if(*dap == gwIP){
				//dbg_printk("Roaming:%s:%d  ARP destination to bridge IP for gateway ARP \r\n",__FUNCTION__,__LINE__ );
				searchIP = *sap;
			}
			if(searchIP) {
				//searchIP=0xc0a80114;
                spin_lock_bh(&port->br->hash_lock);
				/* now searching in flowcache if this IP has flowcache entry setup */
				ret = ptr_fc_getmacs_match4(searchIP,(void **)&srcmac,(void **)&dstmac);
				while (ret == 0 && (srcmac || dstmac))
				{
					dbg_printk("Roaming:%s:%d  has matching found for %d.%d.%d.%d \r\n",__FUNCTION__,__LINE__ ,((unsigned char *)&searchIP)[0],
							((unsigned char *)&searchIP)[1],((unsigned char *)&searchIP)[2],((unsigned char *)&searchIP)[3]);
					if(srcmac) 
						dbg_printk("Roaming:%s:%d  srcmac:%08x%04x \r\n",__FUNCTION__,__LINE__,*((unsigned int*)srcmac->addr.addr),*((unsigned int*)(srcmac->addr.addr+4)));
					if(dstmac)
						dbg_printk("Roaming:%s:%d  dstmac:%08x%04x \r\n",__FUNCTION__,__LINE__,*((unsigned int*)dstmac->addr.addr),*((unsigned int*)(dstmac->addr.addr+4)));
					if(!(( srcmac && !compare_ether_addr((u8 *)mp,(u8 *)(srcmac->addr.addr)))
                         || ( dstmac && !compare_ether_addr((u8 *)mp,(u8 *)(dstmac->addr.addr)))))
					{
						
						if (info->target == EBT_ACCEPT) 
                        {
							if(srcmac)
							{
								dbg_printk("Roaming:%s:%d flush entry src mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(srcmac->addr))),*((__be16 *)(((char *)&(srcmac->addr))+4)));
                                memcpy(srcaddr,srcmac->addr.addr,sizeof(srcaddr));
                                if (!srcmac->is_local)
                                {
	                                fdb_delete_by_addr(port, srcaddr);
                                }
                        	    // Modify for DTS2014041005485 by y00181549 at 2014/4/17
                                else if (!dstmac)
                                {
        							ptr_fc_flush();
                                    always_printk("Roaming:%s:%d ptr_fc_flush src mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(srcmac->addr))),*((__be16 *)(((char *)&(srcmac->addr))+4)));
                                }
                                else
                                {
                                    dbg_printk("Roaming:%s:%d flush entry src mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(srcmac->addr))),*((__be16 *)(((char *)&(srcmac->addr))+4)));
                                }
								
							}
							if(dstmac)
							{
								dbg_printk("Roaming:%s:%d flush entry dest mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(dstmac->addr))),*((__be16 *)(((char *)&(dstmac->addr))+4)));
                                memcpy(dstaddr,dstmac->addr.addr,sizeof(dstaddr));
                                if (!dstmac->is_local)
                                {
	                                fdb_delete_by_addr(port, dstaddr);
                                }
                        	    // Modify for DTS2014041005485 by y00181549 at 2014/4/17
                                else if (!srcmac)
                                {
        							ptr_fc_flush();
                                    always_printk("Roaming:%s:%d ptr_fc_flush dest mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(dstmac->addr))),*((__be16 *)(((char *)&(dstmac->addr))+4)));
                                }
                                else
                                {
                                    dbg_printk("Roaming:%s:%d flush entry dest mac:%08x%04x\r\n",__FUNCTION__,__LINE__,*((__be32 *)(&(dstmac->addr))),*((__be16 *)(((char *)&(dstmac->addr))+4)));
                                }
								
							}
						} 
                        else if(ptr_fc_flush)
						{
                        /*这段代码走不到*/
							ptr_fc_flush();
							dbg_printk("Roaming:%s:%d flush all flow cache \r\n",__FUNCTION__,__LINE__ );
						}
                        spin_unlock_bh(&port->br->hash_lock);
                        
						/* Send gratuitous ARP out the bridge  */
						if (srcmac) 
						{	
							arp_send(ARPOP_REQUEST, ETH_P_ARP,searchIP,brdev ,searchIP, srcaddr ,mp, brdev->broadcast);
							arp_send(ARPOP_REPLY, ETH_P_ARP,searchIP,brdev ,searchIP, srcaddr ,mp, brdev->broadcast);
						}
						if (dstmac) 
						{	
							arp_send(ARPOP_REQUEST, ETH_P_ARP,searchIP,brdev ,searchIP, dstaddr ,mp, brdev->broadcast);
							arp_send(ARPOP_REPLY, ETH_P_ARP,searchIP,brdev ,searchIP, dstaddr ,mp, brdev->broadcast);
						}

                        srcmac = NULL;
                        dstmac = NULL;
                        memset(srcaddr,0,sizeof(srcaddr));
                        memset(dstaddr,0,sizeof(dstaddr));
                        
                        spin_lock_bh(&port->br->hash_lock);
						ret = ptr_fc_getmacs_match4(searchIP, (void **)&srcmac, (void **)&dstmac);
					}
					else
				    {
						ret = 1;
				    }
				}
				spin_unlock_bh(&port->br->hash_lock);
			}
		}
	}
	return EBT_CONTINUE;
	
}
/* END OF MODIFY by y00181549 for lock pretect at 2014/1/24*/

static bool ebt_wlroaming_tg_check(const struct xt_tgchk_param *par)

{
	const struct ebt_wlroaming_t_info *info = par->targinfo;
	const struct ebt_entry *e = par->entryinfo;


	if ((e->ethproto != __constant_htons(ETH_P_ARP)))
		return -EINVAL;
				
	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;
	
	if (INVALID_TARGET)
		return -EINVAL;
	
	return 0;
	
}

static struct xt_target ebt_wlroaming_tg_reg = {
	.name		= EBT_WLROAMING_TARGET,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.target		= ebt_wlroaming_tg,
	.checkentry	= ebt_wlroaming_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_wlroaming_t_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_wlroaming_init(void)
{
	int ret;
	ret = xt_register_target(&ebt_wlroaming_tg_reg);

	if(ret == 0)
		printk(KERN_INFO "ebt_wlroam registered\n");

	return ret;
}

static void __exit ebt_wlroaming_fini(void)
{
	xt_unregister_target(&ebt_wlroaming_tg_reg);
}

module_init(ebt_wlroaming_init);
module_exit(ebt_wlroaming_fini);
MODULE_LICENSE("GPL");
