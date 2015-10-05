/* Connection state tracking for netfilter.  This is separated from,
   but required by, the NAT layer; it can also be used by an iptables
   extension. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2003,2004 USAGI/WIDE Project <http://www.linux-ipv6.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/jhash.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/mm.h>
#include <linux/rculist_nulls.h>
#if defined(CONFIG_MIPS_BRCM)
#include <linux/blog.h>
#endif

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>

#define NF_CONNTRACK_VERSION	"0.5.0"
/*start of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/
#ifdef CONFIG_ATP_CONNTRACK_CLEAN
#define WAN_UP_TIMER_OUT           2
struct timer_list 		wan_up_timer;        /*��ʱ��*/
static void nf_conntrack_user_cleanup(struct net *net);
#endif
/*end of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/

#ifdef  CONFIG_RESERVE_CT_SESSION
#define RESERVE_SESSION_NUM 500
#endif

int (*nfnetlink_parse_nat_setup_hook)(struct nf_conn *ct,
				      enum nf_nat_manip_type manip,
				      struct nlattr *attr) __read_mostly;
EXPORT_SYMBOL_GPL(nfnetlink_parse_nat_setup_hook);

DEFINE_SPINLOCK(nf_conntrack_lock);
EXPORT_SYMBOL_GPL(nf_conntrack_lock);

unsigned int nf_conntrack_htable_size __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_htable_size);

unsigned int nf_conntrack_max __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_max);

/* reserve conntrack for certain connection */
#ifdef CONFIG_RESERVE_CT_SESSION
unsigned int nf_reserve_ct_min __read_mostly;
EXPORT_SYMBOL_GPL(nf_reserve_ct_min);
#endif

#ifdef CONFIG_ATP_CONNTRACK_CLEAN
int nf_conntrack_clean __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_clean);
#endif
struct nf_conn nf_conntrack_untracked __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_untracked);

static struct kmem_cache *nf_conntrack_cachep __read_mostly;

static int nf_conntrack_hash_rnd_initted;
static unsigned int nf_conntrack_hash_rnd;

/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
/* bugfix for lost connection */
LIST_HEAD(safe_list);
#endif
/* <DTS2012071901143 g68080 2012-08-15 mod begin */
hi_nf_conntrack_death_hook pf_hi_nf_conntrack_death_hook;

unsigned int hi_nf_conntrack_register_death_hook(hi_nf_conntrack_death_hook hook)
{
    pf_hi_nf_conntrack_death_hook = hook;
    return 0;
}

unsigned int hi_nf_conntrack_unregister_death_hook()
{
    pf_hi_nf_conntrack_death_hook = NULL;
    return 0;
}

EXPORT_SYMBOL(hi_nf_conntrack_register_death_hook);
EXPORT_SYMBOL(hi_nf_conntrack_unregister_death_hook);
/* <DTS2012071901143 g68080 2012-08-15 mod end */

static u_int32_t __hash_conntrack(const struct nf_conntrack_tuple *tuple,
				  unsigned int size, unsigned int rnd)
{
	unsigned int n;
	u_int32_t h;

	/* The direction must be ignored, so we hash everything up to the
	 * destination ports (which is a multiple of 4) and treat the last
	 * three bytes manually.
	 */
	n = (sizeof(tuple->src) + sizeof(tuple->dst.u3)) / sizeof(u32);
	h = jhash2((u32 *)tuple, n,
		   rnd ^ (((__force __u16)tuple->dst.u.all << 16) |
			  tuple->dst.protonum));

	return ((u64)h * size) >> 32;
}

static inline u_int32_t hash_conntrack(const struct nf_conntrack_tuple *tuple)
{
	return __hash_conntrack(tuple, nf_conntrack_htable_size,
				nf_conntrack_hash_rnd);
}

bool
nf_ct_get_tuple(const struct sk_buff *skb,
		unsigned int nhoff,
		unsigned int dataoff,
		u_int16_t l3num,
		u_int8_t protonum,
		struct nf_conntrack_tuple *tuple,
		const struct nf_conntrack_l3proto *l3proto,
		const struct nf_conntrack_l4proto *l4proto)
{
	memset(tuple, 0, sizeof(*tuple));

	tuple->src.l3num = l3num;
	if (l3proto->pkt_to_tuple(skb, nhoff, tuple) == 0)
		return false;

	tuple->dst.protonum = protonum;
	tuple->dst.dir = IP_CT_DIR_ORIGINAL;

	return l4proto->pkt_to_tuple(skb, dataoff, tuple);
}
EXPORT_SYMBOL_GPL(nf_ct_get_tuple);

bool nf_ct_get_tuplepr(const struct sk_buff *skb, unsigned int nhoff,
		       u_int16_t l3num, struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_l3proto *l3proto;
	struct nf_conntrack_l4proto *l4proto;
	unsigned int protoff;
	u_int8_t protonum;
	int ret;

	rcu_read_lock();

	l3proto = __nf_ct_l3proto_find(l3num);
	ret = l3proto->get_l4proto(skb, nhoff, &protoff, &protonum);
	if (ret != NF_ACCEPT) {
		rcu_read_unlock();
		return false;
	}

	l4proto = __nf_ct_l4proto_find(l3num, protonum);

	ret = nf_ct_get_tuple(skb, nhoff, protoff, l3num, protonum, tuple,
			      l3proto, l4proto);

	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_get_tuplepr);

bool
nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
		   const struct nf_conntrack_tuple *orig,
		   const struct nf_conntrack_l3proto *l3proto,
		   const struct nf_conntrack_l4proto *l4proto)
{
	memset(inverse, 0, sizeof(*inverse));

	inverse->src.l3num = orig->src.l3num;
	if (l3proto->invert_tuple(inverse, orig) == 0)
		return false;

	inverse->dst.dir = !orig->dst.dir;

	inverse->dst.protonum = orig->dst.protonum;
	return l4proto->invert_tuple(inverse, orig);
}
EXPORT_SYMBOL_GPL(nf_ct_invert_tuple);

static void
clean_from_lists(struct nf_conn *ct)
{
	pr_debug("clean_from_lists(%p)\n", ct);
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_REPLY].hnnode);

	/* Destroy all pending expectations */
	nf_ct_remove_expectations(ct);
}
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
static void death_by_timeout(unsigned long ul_conntrack);
#endif

static void
destroy_conntrack(struct nf_conntrack *nfct)
{
	struct nf_conn *ct = (struct nf_conn *)nfct;
	struct net *net = nf_ct_net(ct);
	struct nf_conntrack_l4proto *l4proto;


#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	pr_debug("destroy_conntrack(%p) blog keys[0x%08x,0x%08x]\n",
		ct, ct->blog_key[IP_CT_DIR_ORIGINAL], ct->blog_key[IP_CT_DIR_REPLY]);


	/* Conntrack going away, notify blog client */
	if ( (ct->blog_key[IP_CT_DIR_ORIGINAL] != BLOG_KEY_NONE) || 
			(ct->blog_key[IP_CT_DIR_REPLY] != BLOG_KEY_NONE) )
	{
		/*
		 *  Blog client may perform the following blog requests:
		 *	- FLOWTRACK_KEY_SET BLOG_PARAM1_DIR_ORIG 0
		 *	- FLOWTRACK_KEY_SET BLOG_PARAM1_DIR_REPLY 0
		 *	- FLOWTRACK_EXCLUDE
		 */
		blog_notify(DESTROY_FLOWTRACK, (void*)ct,
					(uint32_t)ct->blog_key[IP_CT_DIR_ORIGINAL],
					(uint32_t)ct->blog_key[IP_CT_DIR_REPLY]);

		/* Safe: In case blog client does not set key to 0 explicilty */
		ct->blog_key[IP_CT_DIR_ORIGINAL] = BLOG_KEY_NONE;
		ct->blog_key[IP_CT_DIR_REPLY]    = BLOG_KEY_NONE;
	}
	clear_bit(IPS_BLOG_BIT, &ct->status);	/* Disable further blogging */
#else
	pr_debug("destroy_conntrack(%p)\n", ct);
#endif

	NF_CT_ASSERT(atomic_read(&nfct->use) == 0);
	NF_CT_ASSERT(!timer_pending(&ct->timeout));

	if (!test_bit(IPS_DYING_BIT, &ct->status))
		nf_conntrack_event(IPCT_DESTROY, ct);
	set_bit(IPS_DYING_BIT, &ct->status);

	/* To make sure we don't get any weird locking issues here:
	 * destroy_conntrack() MUST NOT be called with a write lock
	 * to nf_conntrack_lock!!! -HW */
	rcu_read_lock();
	l4proto = __nf_ct_l4proto_find(nf_ct_l3num(ct), nf_ct_protonum(ct));
	if (l4proto && l4proto->destroy)
		l4proto->destroy(ct);

	rcu_read_unlock();

	spin_lock_bh(&nf_conntrack_lock);
	/* Expectations will have been removed in clean_from_lists,
	 * except TFTP can create an expectation on the first packet,
	 * before connection is in the list, so we need to clean here,
	 * too. */
	nf_ct_remove_expectations(ct);

	/* We overload first tuple to link into unconfirmed list. */
	if (!nf_ct_is_confirmed(ct)) {
		BUG_ON(hlist_nulls_unhashed(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode));
		hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);
	}

	NF_CT_STAT_INC(net, delete);
	spin_unlock_bh(&nf_conntrack_lock);

	if (ct->master)
	{
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
		list_del(&ct->derived_list);
#endif
		nf_ct_put(ct->master);
	}
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
	/* Disconnect all child connections that have infinit timeout */
	if (!list_empty(&ct->derived_connections)) {
		struct nf_conn *child, *tmp;

		list_for_each_entry_safe(child, tmp, &ct->derived_connections,
			derived_list) {
			if (child->derived_timeout == 0xFFFFFFFF &&
			    del_timer(&child->timeout))
				death_by_timeout((unsigned long)child);
		}
	}
#endif

	pr_debug("destroy_conntrack: returning ct=%p to slab\n", ct);
	nf_conntrack_free(ct);
}

static void death_by_timeout(unsigned long ul_conntrack)
{
	struct nf_conn *ct = (void *)ul_conntrack;
	struct net *net = nf_ct_net(ct);
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_helper *helper;

/* <DTS2012071901143 g68080 2012-08-15 mod begin */
    if (pf_hi_nf_conntrack_death_hook != NULL)
    {
        pf_hi_nf_conntrack_death_hook(ct, HI_NF_NORMAL_DEATH_HOOK); 
    }

	if (help) {
		rcu_read_lock();
		helper = rcu_dereference(help->helper);
		if (helper && helper->destroy)
			helper->destroy(ct);
		rcu_read_unlock();
	}

	spin_lock_bh(&nf_conntrack_lock);
	/* Inside lock so preempt is disabled on module removal path.
	 * Otherwise we can get spurious warnings. */
	NF_CT_STAT_INC(net, delete_list);
	clean_from_lists(ct);
	spin_unlock_bh(&nf_conntrack_lock);
	nf_ct_put(ct);
}

static void death_by_timeout2(unsigned long ul_conntrack)
{
	struct nf_conn *ct = (void *)ul_conntrack;
	struct net *net = nf_ct_net(ct);
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_helper *helper;

    if (pf_hi_nf_conntrack_death_hook != NULL)
    {
        if (pf_hi_nf_conntrack_death_hook(ct, HI_NF_TIMEOUT_DEATH_HOOK) == 1)
        {
            ct->timeout.expires = jiffies + 10 * HZ;
            add_timer(&ct->timeout);
            return;
        }
    }
/* <DTS2012071901143 g68080 2012-08-15 mod end */

	if (help) {
		rcu_read_lock();
		helper = rcu_dereference(help->helper);
		if (helper && helper->destroy)
			helper->destroy(ct);
		rcu_read_unlock();
	}

	spin_lock_bh(&nf_conntrack_lock);
	/* Inside lock so preempt is disabled on module removal path.
	 * Otherwise we can get spurious warnings. */
	NF_CT_STAT_INC(net, delete_list);
	clean_from_lists(ct);
	spin_unlock_bh(&nf_conntrack_lock);
	nf_ct_put(ct);
}

/*
 * Warning :
 * - Caller must take a reference on returned object
 *   and recheck nf_ct_tuple_equal(tuple, &h->tuple)
 * OR
 * - Caller must lock nf_conntrack_lock before calling this function
 */
struct nf_conntrack_tuple_hash *
__nf_conntrack_find(struct net *net, const struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int hash = hash_conntrack(tuple);

	/* Disable BHs the entire time since we normally need to disable them
	 * at least once for the stats anyway.
	 */
	local_bh_disable();
begin:
	hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) {
		if (nf_ct_tuple_equal(tuple, &h->tuple)) {
			NF_CT_STAT_INC(net, found);
			local_bh_enable();
			return h;
		}
		NF_CT_STAT_INC(net, searched);
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(n) != hash)
		goto begin;
	local_bh_enable();

	return NULL;
}
EXPORT_SYMBOL_GPL(__nf_conntrack_find);

/* Find a connection corresponding to a tuple. */
struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net, const struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;

	rcu_read_lock();
begin:
	h = __nf_conntrack_find(net, tuple);
	if (h) {
		ct = nf_ct_tuplehash_to_ctrack(h);
		if (unlikely(!atomic_inc_not_zero(&ct->ct_general.use)))
			h = NULL;
		else {
			if (unlikely(!nf_ct_tuple_equal(tuple, &h->tuple))) {
				nf_ct_put(ct);
				goto begin;
			}
		}
	}
	rcu_read_unlock();

	return h;
}
EXPORT_SYMBOL_GPL(nf_conntrack_find_get);

static void __nf_conntrack_hash_insert(struct nf_conn *ct,
				       unsigned int hash,
				       unsigned int repl_hash)
{
	struct net *net = nf_ct_net(ct);

	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode,
			   &net->ct.hash[hash]);
	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_REPLY].hnnode,
			   &net->ct.hash[repl_hash]);
}

void nf_conntrack_hash_insert(struct nf_conn *ct)
{
	unsigned int hash, repl_hash;

	hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
	repl_hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple);

	__nf_conntrack_hash_insert(ct, hash, repl_hash);
}
EXPORT_SYMBOL_GPL(nf_conntrack_hash_insert);

/* Confirm a connection given skb; places it in hash table */
int
__nf_conntrack_confirm(struct sk_buff *skb)
{
	unsigned int hash, repl_hash;
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;
	struct nf_conn_help *help;
	struct hlist_nulls_node *n;
	enum ip_conntrack_info ctinfo;
	struct net *net;

	ct = nf_ct_get(skb, &ctinfo);
	net = nf_ct_net(ct);

	/* ipt_REJECT uses nf_conntrack_attach to attach related
	   ICMP/TCP RST packets in other direction.  Actual packet
	   which created connection will be IP_CT_NEW or for an
	   expected connection, IP_CT_RELATED. */
	if (CTINFO2DIR(ctinfo) != IP_CT_DIR_ORIGINAL)
		return NF_ACCEPT;

	hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
	repl_hash = hash_conntrack(&ct->tuplehash[IP_CT_DIR_REPLY].tuple);

	/* We're not in hash table, and we refuse to set up related
	   connections for unconfirmed conns.  But packet copies and
	   REJECT will give spurious warnings here. */
	/* NF_CT_ASSERT(atomic_read(&ct->ct_general.use) == 1); */

	/* No external references means noone else could have
	   confirmed us. */
	NF_CT_ASSERT(!nf_ct_is_confirmed(ct));
	pr_debug("Confirming conntrack %p\n", ct);

	spin_lock_bh(&nf_conntrack_lock);

	/* See if there's one in the list already, including reverse:
	   NAT could have grabbed it without realizing, since we're
	   not in the hash.  If there is, we lost race. */
	hlist_nulls_for_each_entry(h, n, &net->ct.hash[hash], hnnode)
		if (nf_ct_tuple_equal(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
				      &h->tuple))
			goto out;
	hlist_nulls_for_each_entry(h, n, &net->ct.hash[repl_hash], hnnode)
		if (nf_ct_tuple_equal(&ct->tuplehash[IP_CT_DIR_REPLY].tuple,
				      &h->tuple))
			goto out;

	/* Remove from unconfirmed list */
	hlist_nulls_del_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode);

	__nf_conntrack_hash_insert(ct, hash, repl_hash);
	/* Timer relative to confirmation time, not original
	   setting time, otherwise we'd get timer wrap in
	   weird delay cases. */
	ct->timeout.expires += jiffies;
	add_timer(&ct->timeout);
	atomic_inc(&ct->ct_general.use);
	set_bit(IPS_CONFIRMED_BIT, &ct->status);
	NF_CT_STAT_INC(net, insert);
	spin_unlock_bh(&nf_conntrack_lock);
	help = nfct_help(ct);
	if (help && help->helper)
		nf_conntrack_event_cache(IPCT_HELPER, ct);
#ifdef CONFIG_NF_NAT_NEEDED
	if (test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status) ||
	    test_bit(IPS_DST_NAT_DONE_BIT, &ct->status))
		nf_conntrack_event_cache(IPCT_NATINFO, ct);
#endif
	nf_conntrack_event_cache(master_ct(ct) ?
				 IPCT_RELATED : IPCT_NEW, ct);
	return NF_ACCEPT;

out:
	NF_CT_STAT_INC(net, insert_failed);
	spin_unlock_bh(&nf_conntrack_lock);
	return NF_DROP;
}
EXPORT_SYMBOL_GPL(__nf_conntrack_confirm);

/* Returns true if a connection correspondings to the tuple (required
   for NAT). */
int
nf_conntrack_tuple_taken(const struct nf_conntrack_tuple *tuple,
			 const struct nf_conn *ignored_conntrack)
{
	struct net *net = nf_ct_net(ignored_conntrack);
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int hash = hash_conntrack(tuple);

	/* Disable BHs the entire time since we need to disable them at
	 * least once for the stats anyway.
	 */
	rcu_read_lock_bh();
	hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode) {
		if (nf_ct_tuplehash_to_ctrack(h) != ignored_conntrack &&
		    nf_ct_tuple_equal(tuple, &h->tuple)) {
			NF_CT_STAT_INC(net, found);
			rcu_read_unlock_bh();
			return 1;
		}
		NF_CT_STAT_INC(net, searched);
	}
	rcu_read_unlock_bh();

	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_tuple_taken);

#define NF_CT_EVICTION_RANGE	8

/* There's a small race here where we may free a just-assured
   connection.  Too bad: we're in trouble anyway. */
static noinline int early_drop(struct net *net, unsigned int hash)
{
	/* Use oldest entry, which is roughly LRU */
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct = NULL, *tmp;
	struct hlist_nulls_node *n;
	unsigned int i, cnt = 0;
	int dropped = 0;

    unsigned int uiHash = hash;

	rcu_read_lock();
	for (i = 0; i < nf_conntrack_htable_size; i++) {
		hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash],
					 hnnode) {
			tmp = nf_ct_tuplehash_to_ctrack(h);
#ifdef CONFIG_RESERVE_CT_SESSION
            /* If conntrack is for reserve, then goto next bucket */
			if ((!test_bit(IPS_ASSURED_BIT, &tmp->status))
			&& (RESERVE_MARK != (tmp->mark&RESERVE_MASK)))
			
#else
			if (!test_bit(IPS_ASSURED_BIT, &tmp->status))
#endif		


				ct = tmp;
			cnt++;
		}

		if (ct && unlikely(!atomic_inc_not_zero(&ct->ct_general.use)))
			ct = NULL;
		if (ct || cnt >= NF_CT_EVICTION_RANGE)
			break;
		hash = (hash + 1) % nf_conntrack_htable_size;
	}

    if (!ct)
    {
        cnt = 0;
        for (i = 0; i < nf_conntrack_htable_size; i++) {
            hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[uiHash],
                         hnnode) {
                ct = nf_ct_tuplehash_to_ctrack(h);
                cnt++;
            }
        
#ifdef CONFIG_RESERVE_CT_SESSION
            /* If conntrack is for reserve, then goto next bucket */
			if (RESERVE_MARK == (tmp->mark&RESERVE_MASK))
				ct = NULL;
#endif
            if (ct && unlikely(!atomic_inc_not_zero(&ct->ct_general.use)))
                ct = NULL;
            if (ct || cnt >= NF_CT_EVICTION_RANGE)
                break;
            uiHash = (uiHash + 1) % nf_conntrack_htable_size;
        }
    }
    
	rcu_read_unlock();

	if (!ct)
		return dropped;

	if (del_timer(&ct->timeout)) {
		death_by_timeout((unsigned long)ct);
		dropped = 1;
		NF_CT_STAT_INC_ATOMIC(net, early_drop);
	}
	nf_ct_put(ct);
	return dropped;
}


/* start of ATP caiyanfeng 2008-2-1: ���BTҵ�����Ӹ�����ʱ����cms �������� */
#if CONFIG_SUPPORT_ATP	
static int force_drop(struct net *net, unsigned int hash)
{
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	struct nf_conn *ct = NULL, *tmp;
	int dropped = 0;

	rcu_read_lock();
    hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash],
                 hnnode) {
        tmp = nf_ct_tuplehash_to_ctrack(h);
            ct = tmp;
#ifdef CONFIG_RESERVE_CT_SESSION            
        /* If conntrack is for reserve, then goto next entry */
        if (ct&&(RESERVE_MARK == (ct->mark&RESERVE_MASK)))
        {   
            //printk("line<%d> function<%s>: for reserve\n",__LINE__,__FUNCTION__);        
            continue;
        }
#endif

        break;
    }    
	rcu_read_unlock();

	if (!ct)
	{
		return dropped;
	}

	if (del_timer(&ct->timeout)) 
    {
		death_by_timeout((unsigned long)ct);
		dropped = 1;
		NF_CT_STAT_INC(net, early_drop);
	}
	nf_ct_put(ct);
	return dropped;
}
#endif
/* end of ATP caiyanfeng 2008-2-1: ���BTҵ�����Ӹ�����ʱ����cms �������� */
#if defined(CONFIG_MIPS_BRCM)
static int regardless_drop(struct net *net)
{
   struct nf_conn *ct = NULL;
   int dropped = 0;

   /* Choose the first one (also the oldest one). LRU */
	rcu_read_lock();
   if (!list_empty(&safe_list)) {
      ct = container_of(safe_list.next, struct nf_conn, safe_list);
      atomic_inc(&ct->ct_general.use);
   }
	rcu_read_unlock();

   if (!ct) {   
      if (net_ratelimit())
         printk(KERN_WARNING
                  "nf_conntrack:reagrdless_drop Error: ct table full,"
                  " but safe_list is emtpy.\n");
      return dropped;
   }

   if (del_timer(&ct->timeout)) {
      death_by_timeout((unsigned long)ct);
      dropped = 1;
      NF_CT_STAT_INC_ATOMIC(net, early_drop);
   } 
   /*else{
   * this happens when the ct at safelist head is removed from the timer list 
   * but not yet freed due to ct->ct_general.use > 1. This ct will be freed when its
   * ref count is dropped to zero. At this point we dont create new connections 
   * until some old connection are freed.
   * } 
   */     

   nf_ct_put(ct);
   return dropped;
}
#endif

struct nf_conn *nf_conntrack_alloc(struct net *net,
				   const struct nf_conntrack_tuple *orig,
				   const struct nf_conntrack_tuple *repl,
				   gfp_t gfp)
{
	struct nf_conn *ct;
    
#ifdef CONFIG_RESERVE_CT_SESSION
    /*  nf_conntrack_max              --- The max number of all conntrack
        *  nf_reserve_ct_min              --- The min number of conntrack which should be reserved
        *  net.ct.count                       --- The number of conntrack which has been allocated
        *  net.ct.reserve_cnt              --- The number of conntrack which has been allocated of reserving connection.
        */

    /* current upper limit of reserve conntrack
        * NOTE: reserve_count may be bigger than nf_reserve_ct_min. 
        */
    int reserve_ct_upperlimit = (nf_reserve_ct_min > atomic_read(&net->ct.reserve_cnt)) ? nf_reserve_ct_min : atomic_read(&net->ct.reserve_cnt);
    /* number of normal conntrack which has been allocated. */
    int normal_conntrack = atomic_read(&net->ct.count) - atomic_read(&net->ct.reserve_cnt);
#endif    

	if (unlikely(!nf_conntrack_hash_rnd_initted)) {
		get_random_bytes(&nf_conntrack_hash_rnd,
				sizeof(nf_conntrack_hash_rnd));
		nf_conntrack_hash_rnd_initted = 1;
	}

	/* We don't want any race condition at early drop stage */
	atomic_inc(&net->ct.count);

#ifdef CONFIG_RESERVE_CT_SESSION    
    /* normal conntrack number bigger than the max, then kick off one from normal conntrack */
    if (nf_conntrack_max &&
        unlikely((normal_conntrack + reserve_ct_upperlimit) > nf_conntrack_max)){
#else
	if (nf_conntrack_max &&
	    unlikely(atomic_read(&net->ct.count) > nf_conntrack_max)) {
#endif	    
		unsigned int hash = hash_conntrack(orig);
		if (!early_drop(net, hash)) {
#if 0 //defined(CONFIG_MIPS_BRCM)	
			/* Sorry, we have to kick LRU out regardlessly. */
			if (!regardless_drop(net)) {
            atomic_dec(&net->ct.count);
            if (net_ratelimit())
               printk(KERN_WARNING
                  "nf_conntrack: table full, dropping"
                  " packet.\n");
            return ERR_PTR(-ENOMEM);
			} 
/* start of ATP caiyanfeng 2008-2-1: ���BTҵ�����Ӹ�����ʱ����cms �������� */
//#elif defined(CONFIG_SUPPORT_ATP)
//            {
//                static unsigned int drop_index = 0;                
//                while (!force_drop(net, ((drop_index++)%nf_conntrack_htable_size))) {
//                }
//            }
#else
/* end of ATP caiyanfeng 2008-2-1: ���BTҵ�����Ӹ�����ʱ����cms �������� */
			atomic_dec(&net->ct.count);
			if (net_ratelimit())
				printk(KERN_WARNING
				       "nf_conntrack: table full, dropping"
				       " packet.\n");
			return ERR_PTR(-ENOMEM);
#endif
		}
	}

	ct = kmem_cache_zalloc(nf_conntrack_cachep, gfp);
	if (ct == NULL) {
		pr_debug("nf_conntrack_alloc: Can't alloc conntrack.\n");
		atomic_dec(&net->ct.count);
		return ERR_PTR(-ENOMEM);
	}

	atomic_set(&ct->ct_general.use, 1);
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
	INIT_LIST_HEAD(&ct->derived_connections);
#endif

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	pr_debug("nf_conntrack_alloc: ct<%p> BLOGible\n", ct );
	set_bit(IPS_BLOG_BIT, &ct->status);  /* Enable conntrack blogging */

	/* new conntrack: reset blog keys */
	ct->blog_key[IP_CT_DIR_ORIGINAL] = BLOG_KEY_NONE;
	ct->blog_key[IP_CT_DIR_REPLY]    = BLOG_KEY_NONE;
#endif

	ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple = *orig;
	ct->tuplehash[IP_CT_DIR_REPLY].tuple = *repl;
	/* Don't set timer yet: wait for confirmation */
	setup_timer(&ct->timeout, death_by_timeout2, (unsigned long)ct);     /* <DTS2012071901143 g68080 2012-08-15 mod */
#ifdef CONFIG_NET_NS
	ct->ct_net = net;
#endif

	return ct;
}
EXPORT_SYMBOL_GPL(nf_conntrack_alloc);

void nf_conntrack_free(struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
	/* bugfix for lost connections */
	spin_lock_bh(&nf_conntrack_lock);
	if (ct->safe_list.next)
		list_del(&ct->safe_list);
	spin_unlock_bh(&nf_conntrack_lock);
#endif

#ifdef CONFIG_RESERVE_CT_SESSION  
    /* free a reserved conntrack ?*/
    if (RESERVE_MARK == (ct->mark&RESERVE_MASK))
    {
    	atomic_dec(&net->ct.reserve_cnt);    
        //printk("line<%d> function<%s>: decrease reserve_count<%d>\n",__LINE__,__FUNCTION__,net->ct.reserve_cnt);
    }
#endif

	nf_ct_ext_destroy(ct);
	atomic_dec(&net->ct.count);
	nf_ct_ext_free(ct);
	kmem_cache_free(nf_conntrack_cachep, ct);
}
EXPORT_SYMBOL_GPL(nf_conntrack_free);

/* Allocate a new conntrack: we return -ENOMEM if classification
   failed due to stress.  Otherwise it really is unclassifiable. */
static struct nf_conntrack_tuple_hash *
init_conntrack(struct net *net,
	       const struct nf_conntrack_tuple *tuple,
	       struct nf_conntrack_l3proto *l3proto,
	       struct nf_conntrack_l4proto *l4proto,
	       struct sk_buff *skb,
	       unsigned int dataoff)
{
	struct nf_conn *ct;
	struct nf_conn_help *help;
	struct nf_conntrack_tuple repl_tuple;
	struct nf_conntrack_expect *exp;

	if (!nf_ct_invert_tuple(&repl_tuple, tuple, l3proto, l4proto)) {
		pr_debug("Can't invert tuple.\n");
		return NULL;
	}

	ct = nf_conntrack_alloc(net, tuple, &repl_tuple, GFP_ATOMIC);
	if (IS_ERR(ct)) {
		pr_debug("Can't allocate conntrack.\n");
		return (struct nf_conntrack_tuple_hash *)ct;
	}

	if (!l4proto->new(ct, skb, dataoff)) {
		nf_conntrack_free(ct);
		pr_debug("init conntrack: can't track with proto module\n");
		return NULL;
	}

	nf_ct_acct_ext_add(ct, GFP_ATOMIC);

	spin_lock_bh(&nf_conntrack_lock);
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
	/* bugfix for lost connections */
	list_add_tail(&ct->safe_list, &safe_list);
#endif
	exp = nf_ct_find_expectation(net, tuple);
	if (exp) {
		pr_debug("conntrack: expectation arrives ct=%p exp=%p\n",
			 ct, exp);
		/* Welcome, Mr. Bond.  We've been expecting you... */
		__set_bit(IPS_EXPECTED_BIT, &ct->status);
		ct->master = exp->master;
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
		list_add(&ct->derived_list,
			 &exp->master->derived_connections);
		if (exp->flags & NF_CT_EXPECT_DERIVED_TIMEOUT) {
			ct->derived_timeout = exp->derived_timeout;
		}
#endif
		if (exp->helper) {
			help = nf_ct_helper_ext_add(ct, GFP_ATOMIC);
			if (help)
				rcu_assign_pointer(help->helper, exp->helper);
		}

#ifdef CONFIG_NF_CONNTRACK_MARK
		ct->mark = exp->master->mark;
#endif
#ifdef CONFIG_NF_CONNTRACK_SECMARK
		ct->secmark = exp->master->secmark;
#endif
		nf_conntrack_get(&ct->master->ct_general);
		NF_CT_STAT_INC(net, expect_new);
	} else {
		__nf_ct_try_assign_helper(ct, GFP_ATOMIC);
		NF_CT_STAT_INC(net, new);
	}

	/* Overload tuple linked list to put us in unconfirmed list. */
	hlist_nulls_add_head_rcu(&ct->tuplehash[IP_CT_DIR_ORIGINAL].hnnode,
		       &net->ct.unconfirmed);

	spin_unlock_bh(&nf_conntrack_lock);

	if (exp) {
		if (exp->expectfn)
			exp->expectfn(ct, exp);
		nf_ct_expect_put(exp);
	}

#ifdef CONFIG_NF_CONNTRACK_MARK
    /* ���Ӹ��ٵ�mark֮ǰû��ʹ�ã�����������¼���ĵ�nfmarkֵ */
    ct->mark = skb->mark;
#ifdef CONFIG_RESERVE_CT_SESSION
    if (RESERVE_MARK == (ct->mark&RESERVE_MASK))
    {
    	atomic_inc(&net->ct.reserve_cnt);
        //printk("line<%d> function<%s>: increase reserve_count<%d>\n",__LINE__,__FUNCTION__,net->ct.reserve_cnt);
    }
#endif
#endif

	return &ct->tuplehash[IP_CT_DIR_ORIGINAL];
}

/* On success, returns conntrack ptr, sets skb->nfct and ctinfo */
static inline struct nf_conn *
resolve_normal_ct(struct net *net,
		  struct sk_buff *skb,
		  unsigned int dataoff,
		  u_int16_t l3num,
		  u_int8_t protonum,
		  struct nf_conntrack_l3proto *l3proto,
		  struct nf_conntrack_l4proto *l4proto,
		  int *set_reply,
		  enum ip_conntrack_info *ctinfo)
{
	struct nf_conntrack_tuple tuple;
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;

#ifdef CONFIG_ATP_CONNTRACK_CLEAN
    if (nf_conntrack_clean)
    {
        nf_conntrack_user_cleanup(net);
    }
#endif
	if (!nf_ct_get_tuple(skb, skb_network_offset(skb),
			     dataoff, l3num, protonum, &tuple, l3proto,
			     l4proto)) {
		pr_debug("resolve_normal_ct: Can't get tuple\n");
		return NULL;
	}

	/* look for tuple match */
	h = nf_conntrack_find_get(net, &tuple);
	if (!h) {
		h = init_conntrack(net, &tuple, l3proto, l4proto, skb, dataoff);
		if (!h)
			return NULL;
		if (IS_ERR(h))
			return (void *)h;
	}
	ct = nf_ct_tuplehash_to_ctrack(h);

	/* It exists; we have (non-exclusive) reference. */
	if (NF_CT_DIRECTION(h) == IP_CT_DIR_REPLY) {
		*ctinfo = IP_CT_ESTABLISHED + IP_CT_IS_REPLY;
		/* Please set reply bit if this packet OK */
		*set_reply = 1;
	} else {
		/* Once we've had two way comms, always ESTABLISHED. */
		if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: normal packet for %p\n", ct);
			*ctinfo = IP_CT_ESTABLISHED;
		} else if (test_bit(IPS_EXPECTED_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: related packet for %p\n",
				 ct);
			*ctinfo = IP_CT_RELATED;
		} else {
			pr_debug("nf_conntrack_in: new packet for %p\n", ct);
			*ctinfo = IP_CT_NEW;
		}
		*set_reply = 0;
	}
	skb->nfct = &ct->ct_general;
	skb->nfctinfo = *ctinfo;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	{
		struct nf_conn_help * help = nfct_help(ct);

		if ( (help != (struct nf_conn_help *)NULL) &&
			 (help->helper != (struct nf_conntrack_helper *)NULL) )
		{
			pr_debug("nf_conntrack_in: skb<%p> ct<%p> helper<%s> found\n",
						skb, ct, help->helper->name);
			clear_bit(IPS_BLOG_BIT, &ct->status);
		}
		if (test_bit(IPS_BLOG_BIT, &ct->status))    /* OK to blog ? */
		{
			uint32_t ct_type=(l3num==PF_INET)?BLOG_PARAM2_IPV4:BLOG_PARAM2_IPV6;
			pr_debug("nf_conntrack_in: skb<%p> blog<%p> ct<%p>\n",
						skb, blog_ptr(skb), ct );
			blog_link(FLOWTRACK, blog_ptr(skb),
					  (void*)ct, CTINFO2DIR(skb->nfctinfo), ct_type);
		}
		else
		{
			pr_debug("nf_conntrack_in: skb<%p> ct<%p> NOT BLOGible<%p>\n",
						skb, ct, blog_ptr(skb));
			blog_skip(skb);                         /* No blogging */
		}
	}
#endif

	return ct;
}

unsigned int
nf_conntrack_in(struct net *net, u_int8_t pf, unsigned int hooknum,
		struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_conntrack_l3proto *l3proto;
	struct nf_conntrack_l4proto *l4proto;
	unsigned int dataoff;
	u_int8_t protonum;
	int set_reply = 0;
	int ret;

	/* Previously seen (loopback or untracked)?  Ignore. */
	if (skb->nfct) {
		NF_CT_STAT_INC_ATOMIC(net, ignore);
		return NF_ACCEPT;
	}

	/* rcu_read_lock()ed by nf_hook_slow */
	l3proto = __nf_ct_l3proto_find(pf);
	ret = l3proto->get_l4proto(skb, skb_network_offset(skb),
				   &dataoff, &protonum);
	if (ret <= 0) {
		pr_debug("not prepared to track yet or error occured\n");
		NF_CT_STAT_INC_ATOMIC(net, error);
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		return -ret;
	}

	l4proto = __nf_ct_l4proto_find(pf, protonum);

	/* It may be an special packet, error, unclean...
	 * inverse of the return code tells to the netfilter
	 * core what to do with the packet. */
	if (l4proto->error != NULL) {
		ret = l4proto->error(net, skb, dataoff, &ctinfo, pf, hooknum);
		if (ret <= 0) {
			NF_CT_STAT_INC_ATOMIC(net, error);
			NF_CT_STAT_INC_ATOMIC(net, invalid);
			return -ret;
		}
	}

	ct = resolve_normal_ct(net, skb, dataoff, pf, protonum,
			       l3proto, l4proto, &set_reply, &ctinfo);
	if (!ct) {
		/* Not valid part of a connection */
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		return NF_ACCEPT;
	}

	if (IS_ERR(ct)) {
		/* Too stressed to deal. */
		NF_CT_STAT_INC_ATOMIC(net, drop);
		return NF_DROP;
	}

	NF_CT_ASSERT(skb->nfct);

	ret = l4proto->packet(ct, skb, dataoff, ctinfo, pf, hooknum);
	if (ret <= 0) {
		/* Invalid: inverse of the return code tells
		 * the netfilter core what to do */
		pr_debug("nf_conntrack_in: Can't track with proto module\n");
		nf_conntrack_put(skb->nfct);
		skb->nfct = NULL;
		NF_CT_STAT_INC_ATOMIC(net, invalid);
		if (ret == -NF_DROP)
			NF_CT_STAT_INC_ATOMIC(net, drop);
		return -ret;
	}

	if (set_reply && !test_and_set_bit(IPS_SEEN_REPLY_BIT, &ct->status))
		nf_conntrack_event_cache(IPCT_STATUS, ct);
/* < DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 begin */
#if defined(CONFIG_MIPS_BRCM) || defined(CONFIG_HISI_SD5115)
/* DTS2012081101290: replaced by zhaiqi.z00174311 2012-08-30 end> */
	/* Maintain LRU list. The least recently used ctt is on the head */
	if (ctinfo == IP_CT_ESTABLISHED ||
	    ctinfo == IP_CT_ESTABLISHED + IP_CT_IS_REPLY) {
		spin_lock_bh(&nf_conntrack_lock);
		/* Update ct as latest used */
		list_move_tail(&ct->safe_list, &safe_list);
		spin_unlock_bh(&nf_conntrack_lock);
	}
#endif

	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_in);

bool nf_ct_invert_tuplepr(struct nf_conntrack_tuple *inverse,
			  const struct nf_conntrack_tuple *orig)
{
	bool ret;

	rcu_read_lock();
	ret = nf_ct_invert_tuple(inverse, orig,
				 __nf_ct_l3proto_find(orig->src.l3num),
				 __nf_ct_l4proto_find(orig->src.l3num,
						      orig->dst.protonum));
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_invert_tuplepr);

/* Alter reply tuple (maybe alter helper).  This is for NAT, and is
   implicitly racy: see __nf_conntrack_confirm */
void nf_conntrack_alter_reply(struct nf_conn *ct,
			      const struct nf_conntrack_tuple *newreply)
{
	struct nf_conn_help *help = nfct_help(ct);

	/* Should be unconfirmed, so not in hash table yet */
	NF_CT_ASSERT(!nf_ct_is_confirmed(ct));

	pr_debug("Altering reply tuple of %p to ", ct);
	nf_ct_dump_tuple(newreply);

	ct->tuplehash[IP_CT_DIR_REPLY].tuple = *newreply;
	if (ct->master || (help && !hlist_empty(&help->expectations)))
		return;

	rcu_read_lock();
	__nf_ct_try_assign_helper(ct, GFP_ATOMIC);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(nf_conntrack_alter_reply);

/* Refresh conntrack for this many jiffies and do accounting if do_acct is 1 */
void __nf_ct_refresh_acct(struct nf_conn *ct,
			  enum ip_conntrack_info ctinfo,
			  const struct sk_buff *skb,
			  unsigned long extra_jiffies,
			  int do_acct)
{
	int event = 0;

	NF_CT_ASSERT(ct->timeout.data == (unsigned long)ct);
	NF_CT_ASSERT(skb);

	spin_lock_bh(&nf_conntrack_lock);

	/* Only update if this is not a fixed timeout */
	if (test_bit(IPS_FIXED_TIMEOUT_BIT, &ct->status))
		goto acct;

	/* If not in hash table, timer will not be active yet */
	if (!nf_ct_is_confirmed(ct)) {
		ct->timeout.expires = extra_jiffies;
		event = IPCT_REFRESH;
	} else {
		unsigned long newtime = jiffies + extra_jiffies;

		/* Only update the timeout if the new timeout is at least
		   HZ jiffies from the old timeout. Need del_timer for race
		   avoidance (may already be dying). */
		if (newtime - ct->timeout.expires >= HZ
		    && del_timer(&ct->timeout)) {
			ct->timeout.expires = newtime;
			add_timer(&ct->timeout);
			event = IPCT_REFRESH;
		}
	}

acct:
	if (do_acct) {
		struct nf_conn_counter *acct;

		acct = nf_conn_acct_find(ct);
		if (acct) {
			acct[CTINFO2DIR(ctinfo)].packets++;
			acct[CTINFO2DIR(ctinfo)].bytes +=
				skb->len - skb_network_offset(skb);
		}
	}

	spin_unlock_bh(&nf_conntrack_lock);

	/* must be unlocked when calling event cache */
	if (event)
		nf_conntrack_event_cache(event, ct);
}
EXPORT_SYMBOL_GPL(__nf_ct_refresh_acct);

bool __nf_ct_kill_acct(struct nf_conn *ct,
		       enum ip_conntrack_info ctinfo,
		       const struct sk_buff *skb,
		       int do_acct)
{
	if (do_acct) {
		struct nf_conn_counter *acct;

		spin_lock_bh(&nf_conntrack_lock);
		acct = nf_conn_acct_find(ct);
		if (acct) {
			acct[CTINFO2DIR(ctinfo)].packets++;
			acct[CTINFO2DIR(ctinfo)].bytes +=
				skb->len - skb_network_offset(skb);
		}
		spin_unlock_bh(&nf_conntrack_lock);
	}

	if (del_timer(&ct->timeout)) {
		ct->timeout.function((unsigned long)ct);
		return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(__nf_ct_kill_acct);

#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_conntrack.h>
#include <linux/mutex.h>

/* Generic function for tcp/udp/sctp/dccp and alike. This needs to be
 * in ip_conntrack_core, since we don't want the protocols to autoload
 * or depend on ctnetlink */
int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *tuple)
{
	NLA_PUT_BE16(skb, CTA_PROTO_SRC_PORT, tuple->src.u.tcp.port);
	NLA_PUT_BE16(skb, CTA_PROTO_DST_PORT, tuple->dst.u.tcp.port);
	return 0;

nla_put_failure:
	return -1;
}
EXPORT_SYMBOL_GPL(nf_ct_port_tuple_to_nlattr);

const struct nla_policy nf_ct_port_nla_policy[CTA_PROTO_MAX+1] = {
	[CTA_PROTO_SRC_PORT]  = { .type = NLA_U16 },
	[CTA_PROTO_DST_PORT]  = { .type = NLA_U16 },
};
EXPORT_SYMBOL_GPL(nf_ct_port_nla_policy);

int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t)
{
	if (!tb[CTA_PROTO_SRC_PORT] || !tb[CTA_PROTO_DST_PORT])
		return -EINVAL;

	t->src.u.tcp.port = nla_get_be16(tb[CTA_PROTO_SRC_PORT]);
	t->dst.u.tcp.port = nla_get_be16(tb[CTA_PROTO_DST_PORT]);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_ct_port_nlattr_to_tuple);

int nf_ct_port_nlattr_tuple_size(void)
{
	return nla_policy_len(nf_ct_port_nla_policy, CTA_PROTO_MAX + 1);
}
EXPORT_SYMBOL_GPL(nf_ct_port_nlattr_tuple_size);
#endif

/* Used by ipt_REJECT and ip6t_REJECT. */
static void nf_conntrack_attach(struct sk_buff *nskb, struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;

	/* This ICMP is in reverse direction to the packet which caused it */
	ct = nf_ct_get(skb, &ctinfo);
	if (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL)
		ctinfo = IP_CT_RELATED + IP_CT_IS_REPLY;
	else
		ctinfo = IP_CT_RELATED;

	/* Attach to new skbuff, and increment count */
	nskb->nfct = &ct->ct_general;
	nskb->nfctinfo = ctinfo;
	nf_conntrack_get(nskb->nfct);
}

/* Bring out ya dead! */
static struct nf_conn *
get_next_corpse(struct net *net, int (*iter)(struct nf_conn *i, void *data),
		void *data, unsigned int *bucket)
{
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;
	struct hlist_nulls_node *n;

	spin_lock_bh(&nf_conntrack_lock);
	for (; *bucket < nf_conntrack_htable_size; (*bucket)++) {
		hlist_nulls_for_each_entry(h, n, &net->ct.hash[*bucket], hnnode) {
			ct = nf_ct_tuplehash_to_ctrack(h);
			if (iter(ct, data))
				goto found;
		}
	}
	hlist_nulls_for_each_entry(h, n, &net->ct.unconfirmed, hnnode) {
		ct = nf_ct_tuplehash_to_ctrack(h);
		if (iter(ct, data))
			set_bit(IPS_DYING_BIT, &ct->status);
	}
	spin_unlock_bh(&nf_conntrack_lock);
	return NULL;
found:
	atomic_inc(&ct->ct_general.use);
	spin_unlock_bh(&nf_conntrack_lock);
	return ct;
}

void nf_ct_iterate_cleanup(struct net *net,
			   int (*iter)(struct nf_conn *i, void *data),
			   void *data)
{
	struct nf_conn *ct;
	unsigned int bucket = 0;

	while ((ct = get_next_corpse(net, iter, data, &bucket)) != NULL) {
		/* Time to push up daises... */
		if (del_timer(&ct->timeout))
			death_by_timeout((unsigned long)ct);
		/* ... else the timer will get him soon. */

		nf_ct_put(ct);
	}
}
EXPORT_SYMBOL_GPL(nf_ct_iterate_cleanup);

struct __nf_ct_flush_report {
	u32 pid;
	int report;
};

static int kill_all(struct nf_conn *i, void *data)
{
	struct __nf_ct_flush_report *fr = (struct __nf_ct_flush_report *)data;

	/* get_next_corpse sets the dying bit for us */
	nf_conntrack_event_report(IPCT_DESTROY,
				  i,
				  fr->pid,
				  fr->report);

/*start added by t00191685 for HG658B DTS2012010505145 2012-1-31*/				  
#if defined(CONFIG_BCM963268) && defined(CONFIG_NF_CONNTRACK_MARK)

/* Start of added by f00120964 for conntrack clean with gateway service 2012-6-18 */
     if(((i->mark & NFMARK_TR069_CONTRACK) == NFMARK_TR069_CONTRACK)
     	||((i->mark & NFMARK_RTCP) == NFMARK_RTCP)
     	||((i->mark & NFMARK_RTP) == NFMARK_RTP)
     	||((i->mark & NFMARK_SIP) == NFMARK_SIP)
     	||((i->mark & CONNTRACK_AVOID_SERVICE) == CONNTRACK_AVOID_SERVICE))
     {
     	return 0;
     }

/* End of added by f00120964 for conntrack clean with gateway service 2012-6-18 */
#endif				  
/*end added by t00191685 for HG658B DTS2012010505145 2012-1-31*/	

/* TR069�����Ӳ����, NFMARK_TR069_CONTRACK��Ҫ���û�̬�����ֵ����һ�� */
#if defined(CONFIG_BCM96362) && defined(CONFIG_NF_CONNTRACK_MARK)
/*START modify: LIQI 20111027 HG656c use MARK 0x3010 for high priority*/
#if CONFIG_BCM_EXT_SWITCH
    return ((i->mark & 0xffff) != 0x3010);
#else
    //printk("clean ip conntrack mark[%x] port[%d].\n", i->mark, ntohs(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port));
    return ((i->mark & 0xffff) != 0x3003);
#endif
/*END modify: LIQI 20111027 HG656c use MARK 0x3010 for high priority */
#else
	return 1;
#endif
}

void nf_ct_free_hashtable(void *hash, int vmalloced, unsigned int size)
{
	if (vmalloced)
		vfree(hash);
	else
		free_pages((unsigned long)hash,
			   get_order(sizeof(struct hlist_head) * size));
}
EXPORT_SYMBOL_GPL(nf_ct_free_hashtable);

void nf_conntrack_flush(struct net *net, u32 pid, int report)
{
	struct __nf_ct_flush_report fr = {
		.pid 	= pid,
		.report = report,
	};
	nf_ct_iterate_cleanup(net, kill_all, &fr);
}
EXPORT_SYMBOL_GPL(nf_conntrack_flush);

static void nf_conntrack_cleanup_init_net(void)
{
	nf_conntrack_helper_fini();
	nf_conntrack_proto_fini();
	kmem_cache_destroy(nf_conntrack_cachep);
}

static void nf_conntrack_cleanup_net(struct net *net)
{
#if defined(CONFIG_MIPS_BRCM)
	int try_counter = 0;
	unsigned long start = jiffies;
	unsigned long end = start + HZ;
#endif
	nf_ct_event_cache_flush(net);
	nf_conntrack_ecache_fini(net);
 i_see_dead_people:
	nf_conntrack_flush(net, 0, 0);
	if (atomic_read(&net->ct.count) != 0) {
#ifdef CONFIG_MIPS_BRCM
		if (jiffies >= end) {
			printk("waiting for %d conntrack to be cleaned, "
			       "tried %d times\n",
			       atomic_read(&net->ct.count), try_counter);
			end += HZ;
		}
		try_counter++;
#endif
	schedule();
		goto i_see_dead_people;
	}
	/* wait until all references to nf_conntrack_untracked are dropped */
	while (atomic_read(&nf_conntrack_untracked.ct_general.use) > 1)
		schedule();

	nf_ct_free_hashtable(net->ct.hash, net->ct.hash_vmalloc,
			     nf_conntrack_htable_size);
	nf_conntrack_acct_fini(net);
	nf_conntrack_expect_fini(net);
	free_percpu(net->ct.stat);
#if defined(CONFIG_MIPS_BRCM)
	end = jiffies;
	if (end - start > HZ) {
		printk("nf_conntrack took %lu milliseconds to clean up\n",
		       (end - start) * 1000 / HZ);
	}
#endif
}


/* Mishearing the voices in his head, our hero wonders how he's
   supposed to kill the mall. */
void nf_conntrack_cleanup(struct net *net)
{
	if (net_eq(net, &init_net))
		rcu_assign_pointer(ip_ct_attach, NULL);

	/* This makes sure all current packets have passed through
	   netfilter framework.  Roll on, two-stage module
	   delete... */
	synchronize_net();

	nf_conntrack_cleanup_net(net);

	if (net_eq(net, &init_net)) {
		rcu_assign_pointer(nf_ct_destroy, NULL);
		nf_conntrack_cleanup_init_net();
	}
}


#ifdef CONFIG_ATP_CONNTRACK_CLEAN
static int kill_dns(struct nf_conn *i, void *data)
{
	return (i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port == htons(53));
}
static int kill_sip(struct nf_conn *i, void *data)
{
	unsigned short sipport = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port;
	return ((sipport == htons(5060)) || ((ntohs(sipport) >= 7070) && (ntohs(sipport) <= 7079)));
}
//kill h323 connection log when you registe and unregiste h323 module
static int kill_h323(struct nf_conn *i, void *data)
{    
    unsigned short h323port = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port;    
    return (h323port == htons(1719));
}

static int kill_dport(struct nf_conn *i, void *data)
{
	return (ntohs(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port) == (unsigned short)*(unsigned int*)data);
}

/*start of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/
static void wan_up_idle_timer(unsigned long ptr)
{
//    printk("wan_up_idle_timer=============>run\r\n");
    nf_conntrack_clean = 9;
    /*Start of ATP 2008-11-22 for AU4D01074 by c47036: TIMER��ʱ����Զ�ɾ�� */
    //del_timer(&wan_up_timer);
    /*End of ATP 2008-11-22 for AU4D01074 by c47036: TIMER��ʱ����Զ�ɾ�� */
}
/*end of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/

static void nf_conntrack_user_cleanup(struct net *net)
{
    switch (nf_conntrack_clean)
    {
        case 0: 
            // do nothing
            break;
        case 1: 
            // clean dns conntrack
            nf_ct_iterate_cleanup(net, kill_dns, NULL);
            break;
        case 2: 
            // clean sip conntrack
            nf_ct_iterate_cleanup(net, kill_sip, NULL);
            break;
        case 3:
            // clean h323 conntrack
            nf_ct_iterate_cleanup(net, kill_h323, NULL); 
            break;
        case 9: 
            // clean all for debug
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            /*ȷ��ȫ�����*/
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            break;
/*start of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/
        case 18:
            // clean all for debug
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            /* ȷ��ȫ����� */
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            nf_ct_iterate_cleanup(net, kill_all, NULL);
            /*Start of ATP 2008-11-22 for AU4D01074 by c47036: �ظ����TIMER֮ǰ��ɾ�� */
            del_timer(&wan_up_timer);
            /*End of ATP 2008-11-22 for AU4D01074 by c47036: �ظ����TIMER֮ǰ��ɾ�� */
            init_timer(&wan_up_timer);
	        wan_up_timer.expires = jiffies + WAN_UP_TIMER_OUT * HZ;
	        wan_up_timer.function = wan_up_idle_timer;
	        add_timer(&wan_up_timer);
            break;
/*end of ���ⵥ:AU4D00925�����Ӹ�����Ϣ��ա�by 00126165 2009-10-25*/           
        default: 
            // clean by dest port
            nf_ct_iterate_cleanup(net, kill_dport, &nf_conntrack_clean);
            break;            
    }
    nf_conntrack_clean = 0;
}
#endif




void *nf_ct_alloc_hashtable(unsigned int *sizep, int *vmalloced, int nulls)
{
	struct hlist_nulls_head *hash;
	unsigned int nr_slots, i;
	size_t sz;

	*vmalloced = 0;

	BUILD_BUG_ON(sizeof(struct hlist_nulls_head) != sizeof(struct hlist_head));
	nr_slots = *sizep = roundup(*sizep, PAGE_SIZE / sizeof(struct hlist_nulls_head));
	sz = nr_slots * sizeof(struct hlist_nulls_head);
	hash = (void *)__get_free_pages(GFP_KERNEL | __GFP_NOWARN | __GFP_ZERO,
					get_order(sz));
	if (!hash) {
		*vmalloced = 1;
		printk(KERN_WARNING "nf_conntrack: falling back to vmalloc.\n");
		hash = __vmalloc(sz, GFP_KERNEL | __GFP_ZERO, PAGE_KERNEL);
	}

	if (hash && nulls)
		for (i = 0; i < nr_slots; i++)
			INIT_HLIST_NULLS_HEAD(&hash[i], i);

	return hash;
}
EXPORT_SYMBOL_GPL(nf_ct_alloc_hashtable);

int nf_conntrack_set_hashsize(const char *val, struct kernel_param *kp)
{
	int i, bucket, vmalloced, old_vmalloced;
	unsigned int hashsize, old_size;
	int rnd;
	struct hlist_nulls_head *hash, *old_hash;
	struct nf_conntrack_tuple_hash *h;

	/* On boot, we can set this without any fancy locking. */
	if (!nf_conntrack_htable_size)
		return param_set_uint(val, kp);

	hashsize = simple_strtoul(val, NULL, 0);
	if (!hashsize)
		return -EINVAL;

	hash = nf_ct_alloc_hashtable(&hashsize, &vmalloced, 1);
	if (!hash)
		return -ENOMEM;

	/* We have to rehahs for the new table anyway, so we also can
	 * use a newrandom seed */
	get_random_bytes(&rnd, sizeof(rnd));

	/* Lookups in the old hash might happen in parallel, which means we
	 * might get false negatives during connection lookup. New connections
	 * created because of a false negative won't make it into the hash
	 * though since that required taking the lock.
	 */
	spin_lock_bh(&nf_conntrack_lock);
	for (i = 0; i < nf_conntrack_htable_size; i++) {
		while (!hlist_nulls_empty(&init_net.ct.hash[i])) {
			h = hlist_nulls_entry(init_net.ct.hash[i].first,
					struct nf_conntrack_tuple_hash, hnnode);
			hlist_nulls_del_rcu(&h->hnnode);
			bucket = __hash_conntrack(&h->tuple, hashsize, rnd);
			hlist_nulls_add_head_rcu(&h->hnnode, &hash[bucket]);
		}
	}
	old_size = nf_conntrack_htable_size;
	old_vmalloced = init_net.ct.hash_vmalloc;
	old_hash = init_net.ct.hash;

	nf_conntrack_htable_size = hashsize;
	init_net.ct.hash_vmalloc = vmalloced;
	init_net.ct.hash = hash;
	nf_conntrack_hash_rnd = rnd;
	spin_unlock_bh(&nf_conntrack_lock);

	nf_ct_free_hashtable(old_hash, old_vmalloced, old_size);
	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_set_hashsize);

module_param_call(hashsize, nf_conntrack_set_hashsize, param_get_uint,
		  &nf_conntrack_htable_size, 0600);

static int nf_conntrack_init_init_net(void)
{
	int max_factor = 8;
	int ret;

	/* Idea from tcp.c: use 1/16384 of memory.  On i386: 32MB
	 * machine has 512 buckets. >= 1GB machines have 16384 buckets. */
	if (!nf_conntrack_htable_size) {
		nf_conntrack_htable_size
			= (((num_physpages << PAGE_SHIFT) / 16384)
			   / sizeof(struct hlist_head));
		if (num_physpages > (1024 * 1024 * 1024 / PAGE_SIZE))
			nf_conntrack_htable_size = 16384;
		if (nf_conntrack_htable_size < 32)
			nf_conntrack_htable_size = 32;

		/* Use a max. factor of four by default to get the same max as
		 * with the old struct list_heads. When a table size is given
		 * we use the old value of 8 to avoid reducing the max.
		 * entries. */
		max_factor = 4;
	}
	nf_conntrack_max = max_factor * nf_conntrack_htable_size;

	printk("nf_conntrack version %s (%u buckets, %d max)\n",
	       NF_CONNTRACK_VERSION, nf_conntrack_htable_size,
	       nf_conntrack_max);

	nf_conntrack_cachep = kmem_cache_create("nf_conntrack",
						sizeof(struct nf_conn),
						0, SLAB_DESTROY_BY_RCU, NULL);
	if (!nf_conntrack_cachep) {
		printk(KERN_ERR "Unable to create nf_conn slab cache\n");
		ret = -ENOMEM;
		goto err_cache;
	}

	ret = nf_conntrack_proto_init();
	if (ret < 0)
		goto err_proto;

	ret = nf_conntrack_helper_init();
	if (ret < 0)
		goto err_helper;

	return 0;

err_helper:
	nf_conntrack_proto_fini();
err_proto:
	kmem_cache_destroy(nf_conntrack_cachep);
err_cache:
	return ret;
}

static int nf_conntrack_init_net(struct net *net)
{
	int ret;

	atomic_set(&net->ct.count, 0);
#ifdef CONFIG_RESERVE_CT_SESSION  
	atomic_set(&net->ct.reserve_cnt, 0);
    //printk("line<%d> function<%s>: set reserve_count<%d>\n",__LINE__,__FUNCTION__,net->ct.reserve_cnt);
    nf_reserve_ct_min = RESERVE_SESSION_NUM;
#endif
	INIT_HLIST_NULLS_HEAD(&net->ct.unconfirmed, 0);
	net->ct.stat = alloc_percpu(struct ip_conntrack_stat);
	if (!net->ct.stat) {
		ret = -ENOMEM;
		goto err_stat;
	}
	ret = nf_conntrack_ecache_init(net);
	if (ret < 0)
		goto err_ecache;
	net->ct.hash = nf_ct_alloc_hashtable(&nf_conntrack_htable_size,
					     &net->ct.hash_vmalloc, 1);
	if (!net->ct.hash) {
		ret = -ENOMEM;
		printk(KERN_ERR "Unable to create nf_conntrack_hash\n");
		goto err_hash;
	}
	ret = nf_conntrack_expect_init(net);
	if (ret < 0)
		goto err_expect;
	ret = nf_conntrack_acct_init(net);
	if (ret < 0)
		goto err_acct;

	/* Set up fake conntrack:
	    - to never be deleted, not in any hashes */
#ifdef CONFIG_NET_NS
	nf_conntrack_untracked.ct_net = &init_net;
#endif
	atomic_set(&nf_conntrack_untracked.ct_general.use, 1);
	/*  - and look it like as a confirmed connection */
	set_bit(IPS_CONFIRMED_BIT, &nf_conntrack_untracked.status);

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	blog_refresh_fn = (blog_refresh_t)__nf_ct_refresh_acct;
#endif

	return 0;

err_acct:
	nf_conntrack_expect_fini(net);
err_expect:
	nf_ct_free_hashtable(net->ct.hash, net->ct.hash_vmalloc,
			     nf_conntrack_htable_size);
err_hash:
	nf_conntrack_ecache_fini(net);
err_ecache:
	free_percpu(net->ct.stat);
err_stat:
	return ret;
}

int nf_conntrack_init(struct net *net)
{
	int ret;

	if (net_eq(net, &init_net)) {
		ret = nf_conntrack_init_init_net();
		if (ret < 0)
			goto out_init_net;
	}
	ret = nf_conntrack_init_net(net);
	if (ret < 0)
		goto out_net;

	if (net_eq(net, &init_net)) {
		/* For use by REJECT target */
		rcu_assign_pointer(ip_ct_attach, nf_conntrack_attach);
		rcu_assign_pointer(nf_ct_destroy, destroy_conntrack);
	}
	return 0;

out_net:
	if (net_eq(net, &init_net))
		nf_conntrack_cleanup_init_net();
out_init_net:
	return ret;
}
