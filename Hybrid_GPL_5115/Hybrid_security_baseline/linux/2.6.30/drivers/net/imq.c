/*
 *             Pseudo-driver for the intermediate queue device.
 *
 *             This program is free software; you can redistribute it and/or
 *             modify it under the terms of the GNU General Public License
 *             as published by the Free Software Foundation; either version
 *             2 of the License, or (at your option) any later version.
 *
 * Authors:    Patrick McHardy, <kaber@trash.net>
 *
 *            The first version was written by Martin Devera, <devik@cdi.cz>
 *
 * Credits:    Jan Rafaj <imq2t@cedric.vabo.cz>
 *              - Update patch to 2.4.21
 *             Sebastian Strollo <sstrollo@nortelnetworks.com>
 *              - Fix "Dead-loop on netdevice imq"-issue
 *             Marcel Sebek <sebek64@post.cz>
 *              - Update to 2.6.2-rc1
 *
 *	       After some time of inactivity there is a group taking care
 *	       of IMQ again: http://www.linuximq.net
 *
 *
 *	       2004/06/30 - New version of IMQ patch to kernels <=2.6.7
 *             including the following changes:
 *
 *	       - Correction of ipv6 support "+"s issue (Hasso Tepper)
 *	       - Correction of imq_init_devs() issue that resulted in
 *	       kernel OOPS unloading IMQ as module (Norbert Buchmuller)
 *	       - Addition of functionality to choose number of IMQ devices
 *	       during kernel config (Andre Correa)
 *	       - Addition of functionality to choose how IMQ hooks on
 *	       PRE and POSTROUTING (after or before NAT) (Andre Correa)
 *	       - Cosmetic corrections (Norbert Buchmuller) (Andre Correa)
 *
 *
 *             2005/12/16 - IMQ versions between 2.6.7 and 2.6.13 were
 *             released with almost no problems. 2.6.14-x was released
 *             with some important changes: nfcache was removed; After
 *             some weeks of trouble we figured out that some IMQ fields
 *             in skb were missing in skbuff.c - skb_clone and copy_skb_header.
 *             These functions are correctly patched by this new patch version.
 *
 *             Thanks for all who helped to figure out all the problems with
 *             2.6.14.x: Patrick McHardy, Rune Kock, VeNoMouS, Max CtRiX,
 *             Kevin Shanahan, Richard Lucassen, Valery Dachev (hopefully
 *             I didn't forget anybody). I apologize again for my lack of time.
 *
 *
 *             2008/06/17 - 2.6.25 - Changed imq.c to use qdisc_run() instead 
 *             of qdisc_restart() and moved qdisc_run() to tasklet to avoid
 *             recursive locking. New initialization routines to fix 'rmmod' not
 *             working anymore. Used code from ifb.c. (Jussi Kivilinna)
 *
 *             2008/08/06 - 2.6.26 - (JK)
 *              - Replaced tasklet with 'netif_schedule()'.
 *              - Cleaned up and added comments for imq_nf_queue().
 *
 *             2009/04/12
 *              - Add skb_save_cb/skb_restore_cb helper functions for backuping
 *                control buffer. This is needed because qdisc-layer on kernels
 *                2.6.27 and newer overwrite control buffer. (Jussi Kivilinna)
 *              - Add better locking for IMQ device. Hopefully this will solve
 *                SMP issues. (Jussi Kivilinna)
 *              - Port to 2.6.27
 *              - Port to 2.6.28
 *              - Port to 2.6.29 + fix rmmod not working
 *
 *             2009/04/20 - (Jussi Kivilinna)
 *              - Use netdevice feature flags to avoid extra packet handling
 *                by core networking layer and possibly increase performance.
 *
 *             2009/09/26 - (Jussi Kivilinna)
 *              - Add imq_nf_reinject_lockless to fix deadlock with
 *                imq_nf_queue/imq_nf_reinject.
 *
 *	       Also, many thanks to pablo Sebastian Greco for making the initial
 *	       patch and to those who helped the testing.
 *
 *             More info at: http://www.linuximq.net/ (Andre Correa)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	#include <linux/netfilter_ipv6.h>
#endif
#include <linux/nbuff.h>
#include <linux/imq.h>
#include <net/pkt_sched.h>
#include <net/netfilter/nf_queue.h>

extern int dev_queue_xmit_copy(struct sk_buff *skb);

#if 0
static nf_hookfn imq_nf_hook;

static struct nf_hook_ops imq_ingress_ipv4 = {
	.hook		= imq_nf_hook,
	.owner		= THIS_MODULE,
	.pf		= PF_INET,
	.hooknum	= NF_INET_PRE_ROUTING,
#if defined(CONFIG_IMQ_BEHAVIOR_BA) || defined(CONFIG_IMQ_BEHAVIOR_BB)
	.priority	= NF_IP_PRI_MANGLE + 1
#else
	.priority	= NF_IP_PRI_NAT_DST + 1
#endif
};

static struct nf_hook_ops imq_egress_ipv4 = {
	.hook		= imq_nf_hook,
	.owner		= THIS_MODULE,
	.pf		= PF_INET,
	.hooknum	= NF_INET_POST_ROUTING,
#if defined(CONFIG_IMQ_BEHAVIOR_AA) || defined(CONFIG_IMQ_BEHAVIOR_BA)
	.priority	= NF_IP_PRI_LAST
#else
	.priority	= NF_IP_PRI_NAT_SRC - 1
#endif
};

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static struct nf_hook_ops imq_ingress_ipv6 = {
	.hook		= imq_nf_hook,
	.owner		= THIS_MODULE,
	.pf		= PF_INET6,
	.hooknum	= NF_INET_PRE_ROUTING,
#if defined(CONFIG_IMQ_BEHAVIOR_BA) || defined(CONFIG_IMQ_BEHAVIOR_BB)
	.priority	= NF_IP6_PRI_MANGLE + 1
#else
	.priority	= NF_IP6_PRI_NAT_DST + 1
#endif
};

static struct nf_hook_ops imq_egress_ipv6 = {
	.hook		= imq_nf_hook,
	.owner		= THIS_MODULE,
	.pf		= PF_INET6,
	.hooknum	= NF_INET_POST_ROUTING,
#if defined(CONFIG_IMQ_BEHAVIOR_AA) || defined(CONFIG_IMQ_BEHAVIOR_BA)
	.priority	= NF_IP6_PRI_LAST
#else
	.priority	= NF_IP6_PRI_NAT_SRC - 1
#endif
};
#endif
#endif

#if defined(CONFIG_IMQ_NUM_DEVS)
static unsigned int numdevs = CONFIG_IMQ_NUM_DEVS;
#else
static unsigned int numdevs = IMQ_MAX_DEVS;
#endif

static DEFINE_SPINLOCK(imq_nf_queue_lock);

static struct net_device *imq_devs_cache[IMQ_MAX_DEVS];


static struct net_device_stats *imq_get_stats(struct net_device *dev)
{
	return &dev->stats;
}

/* called for packets kfree'd in qdiscs at places other than enqueue */
static void imq_skb_destructor(struct sk_buff *skb)
{
	//skb_restore_cb(skb); /* kfree backup */
}
#if 0
/* locking not needed when called from imq_nf_queue */
static void imq_nf_reinject_lockless(struct nf_queue_entry *entry,
						unsigned int verdict)
{
	int status;

	if (!entry->next_outfn) {
		nf_reinject(entry, verdict);
		return;
	}

	status = entry->next_outfn(entry, entry->next_queuenum);
	if (status < 0) {
		nf_queue_entry_release_refs(entry);
		kfree_skb(entry->skb);
		kfree(entry);
	}
}

static void imq_nf_reinject(struct nf_queue_entry *entry, unsigned int verdict)
{
	int status;

	if (!entry->next_outfn) {
		spin_lock_bh(&imq_nf_queue_lock);
		nf_reinject(entry, verdict);
		spin_unlock_bh(&imq_nf_queue_lock);
		return;
	}

	rcu_read_lock();
	local_bh_disable();
	status = entry->next_outfn(entry, entry->next_queuenum);
	local_bh_enable();
	if (status < 0) {
		nf_queue_entry_release_refs(entry);
		kfree_skb(entry->skb);
		kfree(entry);
	}

	rcu_read_unlock();
}
#endif

static int imq_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
    int ret = 0;
	int index;
	const struct net_device_ops *ops;

    dev->stats.tx_bytes += skb->len;
	dev->stats.tx_packets++;

    /* start of by d00107688 DTS2010101804484 DTS2010101804484 2010-10-26 */
    /* ע�⣬��� imq_dev_xmit ��������������xmit������ֱ�ӵ��õĻ������ֶβ��ᱻ�ٴθ�ֵ����˵��±�ָ��
       * ΪNULL,�Ӷ�ʹ���޷��ͷŶ�Ӧ��һЩ��socket�йصĽ��̣�ʹ���û�̬�������޷����ͳ�ȥ��
       */
    //skb->destructor = NULL;
    /* End of by d00107688 DTS2010101804484 DTS2010101804484 2010-10-26 */

	dev->trans_start = jiffies;
	index = skb->imq_flags & IMQ_F_IFMASK;

    if (QOS_DEV_IMQ0 == index)
    {
        skb->imq_flags &= ~QOS_DEV_IMQ0;
        skb->imq_flags |= QOS_DEV_IMQ1;
        ret = imq_nf_queue(skb);
        return ret;
    }
    else if (QOS_DEV_IMQ2 == index)
    {
        skb->imq_flags &= ~QOS_DEV_IMQ2;
        skb->imq_flags |= QOS_DEV_IMQ3;
        ret = imq_nf_queue(skb);
        return ret;
    }

    skb->imq_flags = 0;
    dev = __dev_get_by_index(&init_net, skb->iif);

    /*start of w00104696 Add: When there're pkts in imq, DSL down kernel crash 20101103*/
    if (!dev)
    {
        kfree_skb(skb);
        return 0;
    }
    
    ops = dev->netdev_ops;
    if (!ops)
    {
        kfree_skb(skb);
        return 0;
    }

    skb->dev = dev;
    ops->ndo_start_xmit(skb, dev);
    /*end of w00104696 Add: When there're pkts in imq, DSL down kernel crash 20101103*/
    
	return 0;
}

int imq_nf_queue(struct sk_buff *skb)
{
	struct net_device *dev;
	struct sk_buff *skb_orig, *skb_shared;
	struct Qdisc *q;
	struct netdev_queue *txq;
	int users, index;
	int retval = 0;

	index = skb->imq_flags & IMQ_F_IFMASK;
	if (unlikely(index > numdevs - 1)) {
		if (net_ratelimit())
			printk(KERN_WARNING
			       "IMQ: invalid device specified, highest is %u\n",
			       numdevs - 1);
		retval = -EINVAL;
		goto out;
	}

	/* check for imq device by index from cache */
	dev = imq_devs_cache[index];
	if (unlikely(!dev)) {
		char buf[8];

		/* get device by name and cache result */
		snprintf(buf, sizeof(buf), "imq%d", index);
		dev = dev_get_by_name(&init_net, buf);
		if (!dev) {
			/* not found ?!*/
			BUG();
			retval = -ENODEV;
			goto out;
		}

		imq_devs_cache[index] = dev;
		dev_put(dev);
	}

	if (unlikely(!(dev->flags & IFF_UP))) {
		retval = 1;
		goto out;
	}

	dev->last_rx = jiffies;
	dev->stats.rx_bytes += skb->len;
	dev->stats.rx_packets++;

    skb->dev = dev;
    dev_queue_xmit_copy(skb);
out:
	return retval;
}

#if 0
static struct nf_queue_handler nfqh = {
	.name  = "imq",
	.outfn = imq_nf_queue,
};
#endif

static unsigned int imq_nf_hook(unsigned int hook, struct sk_buff *pskb,
				const struct net_device *indev,
				const struct net_device *outdev,
				int (*okfn)(struct sk_buff *))
{
	if (pskb->imq_flags & IMQ_F_ENQUEUE)
		return NF_QUEUE;

	return NF_ACCEPT;
}

static int imq_close(struct net_device *dev)
{
	netif_stop_queue(dev);
	return 0;
}

static int imq_open(struct net_device *dev)
{
	netif_start_queue(dev);
	return 0;
}

static const struct net_device_ops imq_netdev_ops = {
	.ndo_open		= imq_open,
	.ndo_stop		= imq_close,
	.ndo_start_xmit		= imq_dev_xmit,
	.ndo_get_stats		= imq_get_stats,
};

static void imq_setup(struct net_device *dev)
{
	dev->netdev_ops		= &imq_netdev_ops;
	dev->type               = ARPHRD_VOID;
	dev->mtu                = 16000;
	dev->tx_queue_len       = 11000;
	dev->flags              = IFF_NOARP;
	dev->features           = NETIF_F_SG | NETIF_F_FRAGLIST |
				  NETIF_F_GSO | NETIF_F_HW_CSUM |
				  NETIF_F_HIGHDMA;
}

static int imq_validate(struct nlattr *tb[], struct nlattr *data[])
{
	int ret = 0;

	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN) {
			ret = -EINVAL;
			goto end;
		}
		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS]))) {
			ret = -EADDRNOTAVAIL;
			goto end;
		}
	}
	return 0;
end:
	printk(KERN_WARNING "IMQ: imq_validate failed (%d)\n", ret);
	return ret;
}

static struct rtnl_link_ops imq_link_ops __read_mostly = {
	.kind		= "imq",
	.priv_size	= 0,
	.setup		= imq_setup,
	.validate	= imq_validate,
};

#if 0
static int __init imq_init_hooks(void)
{
	int err;

	nf_register_queue_imq_handler(&nfqh);

	err = nf_register_hook(&imq_ingress_ipv4);
	if (err)
		goto err1;

	err = nf_register_hook(&imq_egress_ipv4);
	if (err)
		goto err2;

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	err = nf_register_hook(&imq_ingress_ipv6);
	if (err)
		goto err3;

	err = nf_register_hook(&imq_egress_ipv6);
	if (err)
		goto err4;
#endif

	return 0;

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
err4:
	nf_unregister_hook(&imq_ingress_ipv6);
err3:
	nf_unregister_hook(&imq_egress_ipv4);
#endif
err2:
	nf_unregister_hook(&imq_ingress_ipv4);
err1:
	nf_unregister_queue_imq_handler();
	return err;
}
#endif

static int __init imq_init_one(int index)
{
	struct net_device *dev;
	int ret;

	dev = alloc_netdev(0, "imq%d", imq_setup);
	if (!dev)
		return -ENOMEM;

	ret = dev_alloc_name(dev, dev->name);
	if (ret < 0)
		goto fail;

	dev->rtnl_link_ops = &imq_link_ops;
	ret = register_netdevice(dev);
	if (ret < 0)
		goto fail;

	return 0;
fail:
	free_netdev(dev);
	return ret;
}

static int __init imq_init_devs(void)
{
	int err, i;

	if (numdevs < 1 || numdevs > IMQ_MAX_DEVS) {
		printk(KERN_ERR "IMQ: numdevs has to be betweed 1 and %u\n",
		       IMQ_MAX_DEVS);
		return -EINVAL;
	}

	rtnl_lock();
	err = __rtnl_link_register(&imq_link_ops);

	for (i = 0; i < numdevs && !err; i++)
		err = imq_init_one(i);

	if (err) {
		__rtnl_link_unregister(&imq_link_ops);
		memset(imq_devs_cache, 0, sizeof(imq_devs_cache));
	}
	rtnl_unlock();

	return err;
}

static int __init imq_init_module(void)
{
	int err;

#if defined(CONFIG_IMQ_NUM_DEVS)
	BUILD_BUG_ON(CONFIG_IMQ_NUM_DEVS > 16);
	BUILD_BUG_ON(CONFIG_IMQ_NUM_DEVS < 2);
	BUILD_BUG_ON(CONFIG_IMQ_NUM_DEVS - 1 > IMQ_F_IFMASK);
#endif

	err = imq_init_devs();
	if (err) {
		printk(KERN_ERR "IMQ: Error trying imq_init_devs(net)\n");
		return err;
	}
#if 0
	err = imq_init_hooks();
	if (err) {
		printk(KERN_ERR "IMQ: Error trying imq_init_hooks()\n");
		rtnl_link_unregister(&imq_link_ops);
		memset(imq_devs_cache, 0, sizeof(imq_devs_cache));
		return err;
	}
#endif
	printk(KERN_INFO "IMQ driver loaded successfully.\n");

#if defined(CONFIG_IMQ_BEHAVIOR_BA) || defined(CONFIG_IMQ_BEHAVIOR_BB)
	printk(KERN_INFO "\tHooking IMQ before NAT on PREROUTING.\n");
#else
	printk(KERN_INFO "\tHooking IMQ after NAT on PREROUTING.\n");
#endif
#if defined(CONFIG_IMQ_BEHAVIOR_AB) || defined(CONFIG_IMQ_BEHAVIOR_BB)
	printk(KERN_INFO "\tHooking IMQ before NAT on POSTROUTING.\n");
#else
	printk(KERN_INFO "\tHooking IMQ after NAT on POSTROUTING.\n");
#endif

	return 0;
}
#if 0
static void __exit imq_unhook(void)
{
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	nf_unregister_hook(&imq_ingress_ipv6);
	nf_unregister_hook(&imq_egress_ipv6);
#endif
	nf_unregister_hook(&imq_ingress_ipv4);
	nf_unregister_hook(&imq_egress_ipv4);

	nf_unregister_queue_imq_handler();
}
#endif
static void __exit imq_cleanup_devs(void)
{
	rtnl_link_unregister(&imq_link_ops);
	memset(imq_devs_cache, 0, sizeof(imq_devs_cache));
}

static void __exit imq_exit_module(void)
{
#if 0
    imq_unhook();
#endif
	imq_cleanup_devs();
	printk(KERN_INFO "IMQ driver unloaded successfully.\n");
}

module_init(imq_init_module);
module_exit(imq_exit_module);

module_param(numdevs, int, 0);
MODULE_PARM_DESC(numdevs, "number of IMQ devices (how many imq* devices will "
			"be created)");
MODULE_AUTHOR("http://www.linuximq.net");
MODULE_DESCRIPTION("Pseudo-driver for the intermediate queue device. See "
			"http://www.linuximq.net/ for more information.");
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK("imq");
EXPORT_SYMBOL(imq_nf_queue);
