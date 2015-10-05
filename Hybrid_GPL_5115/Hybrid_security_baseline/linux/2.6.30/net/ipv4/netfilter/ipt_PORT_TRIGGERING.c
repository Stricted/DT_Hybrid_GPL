/* Kernel module to match the port-ranges, trigger related port-ranges,
 * and alters the destination to a local IP address.
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 *
 * Description:
 *   This is kernel module for port-triggering.
 *
 *   The module follows the Netfilter framework, called extended packet 
 *   matching modules. 
 */


#include <linux/types.h>
#include <linux/ip.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/protocol.h>
#include <net/checksum.h>
#include <linux/list.h>
#include<linux/tcp.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>

#ifdef CONFIG_NF_NAT_NEEDED
#include <net/netfilter/nf_nat_rule.h>
#else
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#endif

#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* This rwlock protects the main hash table, protocol/helper/expected
 *    registrations, conntrack timers*/

DEFINE_RWLOCK(ip_conntrack_lock);


/* Return pointer to first true entry, if any, or NULL.  A macro
   required to allow inlining of cmpfn. */
#define LIST_FIND(head, cmpfn, type, args ...)		\
({							\
	const struct list_head *__i, *__j = NULL;	\
							\
	list_for_each(__i, (head))			\
		if (cmpfn((const type)__i , ## args)) {	\
			__j = __i;			\
			break;				\
		}					\
	(type)__j;					\
})



#if 0
#include<kernel.h>
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

struct ipt_trigger {
    struct list_head list;		/* Trigger list */
    struct timer_list timeout;	/* Timer for list destroying */
    u_int32_t srcip;		/* Outgoing source address */
    u_int32_t dstip;		/* Outgoing destination address */
    u_int16_t mproto;		/* Trigger protocol */
    u_int16_t rproto;		/* Related protocol */
    struct ipt_trigger_ports ports;	/* Trigger and related ports */
    u_int8_t reply;			/* Confirm a reply connection */
};

LIST_HEAD(trigger_list);

static void trigger_refresh(struct ipt_trigger *trig, unsigned long extra_jiffies)
{
    DEBUGP("%s: \n", __FUNCTION__);
    
    NF_CT_ASSERT(trig);
    
    write_lock_bh(&ip_conntrack_lock);

    /* Need del_timer for race avoidance (may already be dying). */
    if (del_timer(&trig->timeout)) {
        trig->timeout.expires = jiffies + extra_jiffies;
        add_timer(&trig->timeout);
    }

	write_unlock_bh(&ip_conntrack_lock);
}

static void __del_trigger(struct ipt_trigger *trig)
{
    DEBUGP("%s: \n", __FUNCTION__);
    
    NF_CT_ASSERT(trig);
    /* delete from 'trigger_list' */
    list_del(&trig->list);
    kfree(trig);
}

static void trigger_timeout(unsigned long ul_trig)
{
    struct ipt_trigger *trig= (void *) ul_trig;

    DEBUGP("trigger list %p timed out\n", trig);
    
    write_lock_bh(&ip_conntrack_lock);
    __del_trigger(trig);
	write_unlock_bh(&ip_conntrack_lock);
}

    static unsigned int
add_new_trigger(struct ipt_trigger *trig)
{
    struct ipt_trigger *new;

    DEBUGP("!!!!!!!!!!!! %s !!!!!!!!!!!\n", __FUNCTION__);
    
    write_lock_bh(&ip_conntrack_lock);
    new = (struct ipt_trigger *)
        kmalloc(sizeof(struct ipt_trigger), GFP_ATOMIC);

    if (!new) {
        write_unlock_bh(&ip_conntrack_lock);
        DEBUGP("%s: OOM allocating trigger list\n", __FUNCTION__);
        return -ENOMEM;
    }

    memset(new, 0, sizeof(*trig));
    INIT_LIST_HEAD(&new->list);
    memcpy(new, trig, sizeof(*trig));

    /* add to global table of trigger */
    list_add(&new->list, &trigger_list);
    /* add and start timer if required */
    init_timer(&new->timeout);
    new->timeout.data = (unsigned long)new;
    new->timeout.function = trigger_timeout;
    new->timeout.expires = jiffies + (TRIGGER_TIMEOUT * HZ);
    add_timer(&new->timeout);

	write_unlock_bh(&ip_conntrack_lock);

    return 0;
}

static inline int trigger_out_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport)
{
    DEBUGP("%s: i=%p, proto= %d, dport=%d.\n", __FUNCTION__, i, proto, dport);
    DEBUGP("%s: Got one, mproto= %d, mport[0..1]=%d, %d.\n", __FUNCTION__, 
        i->mproto, i->ports.mport[0], i->ports.mport[1]);

    #if 0
    printk("trigger out: protocol: (%d) %s\r\n", proto, proto == IPPROTO_TCP ? "TCP":"UDP");
    printk("i->mproto :            (%d) %s\r\n", i->mproto, i->mproto == IPPROTO_TCP ? "TCP":"UDP");
    #endif

    return (((i->mproto == proto) || (!i->mproto)) && (i->ports.mport[0] <= dport) 
        && (i->ports.mport[1] >= dport));
}

    static unsigned int
trigger_out(struct sk_buff **pskb,
    const struct net_device *in,
    const struct net_device *out,
        unsigned int hooknum,
    const void *targinfo )
{
    const struct ipt_trigger_info *info = targinfo;
    struct ipt_trigger trig, *found;
    const struct iphdr *iph = ip_hdr(*pskb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4; 	/* Might be TCP, UDP */

    DEBUGP("############# %s ############\n", __FUNCTION__);

    #if 0
    printk("\r\n************TRIGGER_OUT***************\r\n");
    printk(" info->mproto: (%d) %s\r\n", info->proto, (info->proto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" info->rproto: (%d)%s\r\n", info->rproto, (info->rproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk("info->mport[0...1]: %d - %d\r\n", info->ports.mport[0], info->ports.mport[1]);
    printk("info->rport[0...1]: %d - %d\r\n", info->ports.rport[0], info->ports.rport[1]);
    printk("real port:          %d\r\n",  ntohs(tcph->dest));
    printk("real protocol:      %d (%s)\r\n", iph->protocol, iph->protocol == IPPROTO_TCP ? "TCP":"UDP");
    printk("___________________________________________\r\n");
    #endif
    
    /* Check if the trigger range has already existed in 'trigger_list'. */
    /* ���ȣ���ȫ�������в����Ƿ���ڸ�skb����Ĺ��� */
    found = LIST_FIND(&trigger_list, trigger_out_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    /* ������ִ��ڸ�skb����Ĺ��� */
    if (found) {

        #if 0
        printk("find the list ................\r\n");
        #endif
        
        /* Yeah, it exists. We need to update(delay) the destroying timer. ���¼����� */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        /* In order to allow multiple hosts use the same port range, we update
           the 'saddr' after previous trigger has a reply connection. */
        /* �������reply�Ѿ�����1�ˣ���ô��ʾ�Ѿ�������������ʹ�øù����ˣ��������Ǿͽ�
          * ԴIP�ĳɱ�skb�е�ԴIP����������İ������ǻ��͵�found�д�ŵ���ǰ��IP
          */
        if (found->reply)
            found->srcip = iph->saddr;
    }
    else {
        if (((iph->protocol == info->proto)
              || (!info->proto))
            && (info->ports.mport[0] <= ntohs(tcph->dest))
            && (info->ports.mport[1] >= ntohs(tcph->dest))) {
            
            memset(&trig, 0, sizeof(trig));
            trig.srcip = iph->saddr;

#if 1
            trig.mproto = info->proto;
#else
            if (info->proto) {
                trig.mproto = info->proto;
            } else {
                trig.mproto = iph->protocol;
            }
#endif
#ifdef ATP_SUPPORT_PRTT_RPROTOCOL
            if (info->rproto) 
            {
                trig.rproto = info->rproto;
            }
            else 
            {
                trig.rproto = info->proto;
            }
#else
            trig.rproto = info->proto;
#endif

            
            memcpy(&trig.ports, &info->ports, sizeof(struct ipt_trigger_ports));
            add_new_trigger(&trig); /* Add the new 'trig' to list 'trigger_list'. */
        }
    }
    #if 0
    else {
        printk("not find the list, build one ......\r\n");
        /* Create new trigger */
        memset(&trig, 0, sizeof(trig));
        trig.srcip = iph->saddr;

        /* ��ǰ�ķ����Ǳ�ʾ������Э��Ӧ�������õ�Э�����ͣ���ȥ�Ĳ��ܣ�Ҳ������������˹���
          * ���� trigger-proto tcp ��ô��ʵ��ʾ�����������Ӧ����TCPЭ�飬������˵��ȥ����TCP
          * Э�飬���ԣ������ǳ�ȥ����UDP����TCPЭ�飬ֻҪ�Ǵӹ����ж�Ӧ�˿ڳ�ȥ�ģ���ô�Ϳ���
          * ���¸�Э��Ĵ������Ӷ��򿪶�Ӧ�˿ڡ�
          */
        #if 0
        trig.mproto = iph->protocol;
        trig.rproto = info->proto;
        #else
        if (info->proto) {
            printk("info->proto:  %d(%s)\r\n", info->proto, info->proto == IPPROTO_TCP ? "TCP":"UDP");
            trig.mproto = info->proto;
        }
        else {
            printk("line: %d\r\n", __LINE__);
            trig.mproto = iph->protocol;
        }
        #ifdef ATP_SUPPORT_PRTT_RPROTOCOL
        if (info->rproto) {
            printk("line: %d\r\n", __LINE__);
            trig.rproto = info->rproto;
        }
        else {
            printk("line: %d\r\n", __LINE__);
            trig.rproto = info->proto;
        }
        #else
        printk("line: %d\r\n", __LINE__);
        trig.rproto = info->proto;
        #endif
        
        #endif

        printk("\r\n#################################r\n");
        printk("build result: \r\n");
        printk(" trig->mproto: (%d) %s\r\n", trig.mproto, (trig.mproto == IPPROTO_TCP ? "TCP":"UDP"));
        printk(" trig->rproto: (%d)%s\r\n", trig.rproto, (trig.rproto == IPPROTO_TCP ? "TCP":"UDP"));
        printk("info->mport[0...1]: %d - %d\r\n", trig.ports.mport[0], trig.ports.mport[1]);
        printk("info->rport[0...1]: %d - %d\r\n", trig.ports.rport[0], trig.ports.rport[1]);
        printk("#####################################\r\n");

        
        memcpy(&trig.ports, &info->ports, sizeof(struct ipt_trigger_ports));
        add_new_trigger(&trig);	/* Add the new 'trig' to list 'trigger_list'. */
    }
    #endif

    return IPT_CONTINUE;	/* We don't block any packet. */
}

static inline int trigger_in_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport)
{
#if 0
    u_int16_t rproto = i->rproto;

    if (!rproto)
        rproto = proto;
#endif
    #if 0
    printk("\r\n++++++++++++trigger_in_matched+++++++++++++++++\r\n");
    printk("trigger in match:  \r\n");
    printk(" i->mproto:     (%d) %s\r\n", i->mproto, (i->mproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" i->rproto:     (%d) %s\r\n", i->rproto, (i->rproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" real protocol: (%d) %s\r\n", proto, proto == IPPROTO_TCP ? "tcp":"udp");
    printk(" real port:      %d\r\n", dport);
    printk("i->mport[0...1]: %d - %d\r\n", i->ports.mport[0], i->ports.mport[1]);
    printk("i->rport[0...1]: %d - %d\r\n", i->ports.rport[0], i->ports.rport[1]);
    printk("\r\n===============================================\r\n");
    #endif
    
    return (((i->rproto == proto) || (!i->rproto)) && (i->ports.rport[0] <= dport) 
        && (i->ports.rport[1] >= dport));
}

static unsigned int
trigger_in(struct sk_buff **pskb,
    const struct net_device *in,
    const struct net_device *out,
    unsigned int hooknum,
    const void *targinfo )
{
    struct ipt_trigger *found = NULL;
    const struct iphdr *iph = ip_hdr(*pskb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;	/* Might be TCP, UDP */

    /* Check if the trigger-ed range has already existed in 'trigger_list'. 
      * ����ȥ��һ�ѣ��������ı����Ƿ�����Ҫ��
      */
    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));
    if (found) {
        DEBUGP("############# %s ############\n", __FUNCTION__);
        /* Yeah, it exists. We need to update(delay) the destroying timer. */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        return NF_ACCEPT;	/* Accept it, or the imcoming packet could be 
                               dropped in the FORWARD chain */
    }

    return IPT_CONTINUE;	/* Our job is the interception. */
}

/* ��Ҫ����Ϊ�����İ������ȷ������ӣ�������Ҫ�������DNAT������Ͳ�֪�������İ����巢����һ���� */
    static unsigned int
trigger_dnat(struct sk_buff **pskb,
    const struct net_device *in,
    const struct net_device *out,
    unsigned int hooknum,
    const void *targinfo )
{
    struct ipt_trigger *found;
    const struct iphdr *iph = ip_hdr(*pskb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;	    /* Might be TCP, UDP */
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
	struct nf_nat_range newrange;
	const struct nf_nat_multi_range_compat *mr;

    
    NF_CT_ASSERT(hooknum == NF_INET_PRE_ROUTING);
    
	mr = targinfo;

    /* Check if the trigger-ed range has already existed in 'trigger_list'. */
    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    /* ���û���ҵ���Ӧ�Ĺ�����ô�ͼ�������һ��iptables�������skb */
    if (!found || !found->srcip)
        return IPT_CONTINUE;	/* We don't block any packet. */

    DEBUGP("############# %s ############\n", __FUNCTION__);
    found->reply = 1;	/* Confirm there has been a reply connection. */
    
    ct = nf_ct_get(*pskb, &ctinfo);

    /* ��ȻӦ���������������ӽ��� */
    NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW));

    DEBUGP("%s: got ", __FUNCTION__);
    DUMP_TUPLE(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);

	/* Transfer from original range. */
	newrange = ((struct nf_nat_range)
		{ mr->range[0].flags | IP_NAT_RANGE_MAP_IPS,
		  found->srcip, found->srcip,
		  mr->range[0].min, mr->range[0].max });


    /* Hand modified range to generic setup */
    return nf_nat_setup_info(ct, &newrange, IP_NAT_MANIP_DST);
}

static unsigned int
trigger_target(struct sk_buff *pskb,
    const struct xt_target_param * target_param)
{
    const struct ipt_trigger_info *info = target_param->targinfo;
    const struct iphdr *iph = ip_hdr(pskb);
    DEBUGP("%s: type = %s\n", __FUNCTION__, 
        (info->type == IPT_TRIGGER_DNAT) ? "dnat" :
        (info->type == IPT_TRIGGER_IN) ? "in" : "out"); 

    /* The Port-trigger only supports TCP and UDP. */
    if ((iph->protocol != IPPROTO_TCP) && (iph->protocol != IPPROTO_UDP))
        return IPT_CONTINUE;
    if (info->type == IPT_TRIGGER_OUT)
        return trigger_out(&pskb, target_param->in, target_param->out, target_param->hooknum, target_param->targinfo);
    else if (info->type == IPT_TRIGGER_IN)
        return trigger_in(&pskb, target_param->in, target_param->out, target_param->hooknum, target_param->targinfo);
    else if (info->type == IPT_TRIGGER_DNAT)
        return trigger_dnat(&pskb, target_param->in, target_param->out, target_param->hooknum, target_param->targinfo);

    return IPT_CONTINUE;
}

    static int
trigger_check(const struct xt_tgchk_param * tgchk_param)
{
    const struct ipt_trigger_info *info = tgchk_param->targinfo;
    struct list_head *cur_item, *tmp_item;

    if ((strcmp(tgchk_param->table, "mangle") == 0)) {
        DEBUGP("trigger_check: bad table `%s'.\n", tgchk_param->table);
        return 0;
    }

#if 0
    if (targinfosize != IPT_ALIGN(sizeof(*info))) {
        DEBUGP("trigger_check: size %u.\n", targinfosize);
        return 0;
    }
#endif
    if (tgchk_param->hook_mask & ~((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_FORWARD))) {
        DEBUGP("trigger_check: bad hooks %x.\n", tgchk_param->hook_mask);
        return 0;
    }
    if (info->proto) {
        if (info->proto != IPPROTO_TCP && info->proto != IPPROTO_UDP) {
            DEBUGP("trigger_check: bad proto %d.\n", info->proto);
            return 0;
        }
    }

    /* ���������relatedЭ�������ж� */
    #ifdef ATP_SUPPORT_PRTT_RPROTOCOL
    if (info->rproto) {
        if (info->rproto != IPPROTO_TCP && info->rproto != IPPROTO_UDP) {
            DEBUGP("trigger_check: bad proto %d.\n", info->rproto);
            return 0;
        }
    }
    #endif

    
    if (info->type == IPT_TRIGGER_OUT) {
        if (!info->ports.mport[0] || !info->ports.rport[0]) {
            DEBUGP("trigger_check: Try 'iptbles -j PORT_TRIGGERING -h' for help.\n");
            return 0;
        }
    }

    /* Empty the 'trigger_list' */
    list_for_each_safe(cur_item, tmp_item, &trigger_list) {
        struct ipt_trigger *trig = (void *)cur_item;

        DEBUGP("%s: list_for_each_safe(): %p.\n", __FUNCTION__, trig);
        del_timer(&trig->timeout);
        __del_trigger(trig);
    }

    return 1;
}

static struct xt_target redirect_reg = { 
    .name = "PORT_TRIGGERING",
    .family		= NFPROTO_IPV4,
    .target = trigger_target,   
    .targetsize = sizeof(struct ipt_trigger_info),
    .checkentry = trigger_check,
    .destroy = NULL,
    .me = THIS_MODULE,
};

static int __init init(void)
{
    return xt_register_target(&redirect_reg);
}

static void __exit fini(void)
{
    xt_unregister_target(&redirect_reg);
}

module_init(init);
module_exit(fini);

