
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/if_vlan.h>
#include <asm/atomic.h>
#include <linux/igmp.h>
#include "br_private.h"
#include "br_igmp_snooping.h"

#include "atp_interface.h"

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#endif
struct net_bridge_igmp_snooping igmp_snooping_list;

/*start by f00184228 DTS2011111007229 对于特殊地址snooping不生效*/
//static unsigned char upnp_mac[6] = {0x01, 0x00, 0x5e, 0x7f, 0xff, 0xfa};
static unsigned long upnp_ip = 0xEFFFFFFAU;

/* start of AU4D01311 by d00107688 网关开启igmp snooping功能后，动态路由：br0的rip v2功能出现异常 */
//static unsigned char ripv2_mac[6] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x09};
static unsigned long ripv2_ip = 0xE0000009U;
/* end of AU4D01311 by d00107688 网关开启igmp snooping功能后，动态路由：br0的rip v2功能出现异常 */

/* BEGIN: Added by z67728, 2010/12/1 mDNS multicast packet */
//static unsigned char mdns_mac[6] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb};
static unsigned long mdns_ip = 0xE00000FBU;
/* END:   Added by z67728, 2010/12/1 */
/*end by f00184228 DTS2011111007229 对于特殊地址snooping不生效*/

/* BEGIN: Added by z67728, 2010/7/12 For BT IGMPv2 query .*/
struct net_bridge_igmp_query_chain g_igmpquerylist;
/* END:   Added by z67728, 2010/7/12 */

/* BEGIN: Added by z67728, 2010/8/11 For IGMP statistic */
#if defined(CONFIG_BR_IGMP_STATISTIC)
extern unsigned long  g_ulIgmpStatisticEnableSwitch;
#endif
/* END:   Added by z67728, 2010/8/11 */

#if 0
#define IS_DEBUGP(format, args...) printk(format,## args)
#else
#define PRSITE_DEBUGP(format, args...)
#endif

#if 0
unsigned char extra_mac[][6] = {  
    {0x01, 0x00, 0x5e, 0x7f, 0xff, 0xfa},  /*upnp*/
    {0x01, 0x00, 0x5e, 0x00, 0x00, 0x01},  /*sys1*/ 
    {0x01, 0x00, 0x5e, 0x00, 0x00, 0x02},  /*sys2*/
    {0x01, 0x00, 0x5e, 0x00, 0x00, 0x05},  /*ospf1*/
    {0x01, 0x00, 0x5e, 0x00, 0x00, 0x06},  /*ospf2*/
    {0x01, 0x00, 0x5e, 0x00, 0x00, 0x09},  /*ripv2*/
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},  /*sys*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   /*end*/
};

int br_igmp_snooping_filter_mac(unsigned char *grp_mac)
{
    int i = 0;

    for( i = 0; extra_mac[i][0] != 0x00; i++ )
    {
        if ((!memcmp(grp_mac,extra_mac[i],ETH_ALEN))
        {
            return 0;
        }
    }

    return 1;
}

void br_igmp_snooping_del_by_grp_mac(struct net_bridge_port *dev_port, unsigned char *grp_mac)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;

	spin_lock_bh(&igmp_snooping_list->igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if ((!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN)) && (dst_entry->dev_dst == dev_port)) 
	    {
    		list_del_rcu(&dst_entry->list);
    		kfree(dst_entry);
	    }
	}
	spin_unlock_bh(&igmp_snooping_list->igmp_lock);	
}
#endif
#ifdef IGMP_SNOOPING_DEBUG
#define BR_IGMP_SNOOPING_DEBUG(format,arg...)  printk("\r\nFILE[%s] FUNC[%s] LINE[%d]"format"\r\n",__FILE__,__func__,__LINE__,##arg)
#else
#define BR_IGMP_SNOOPING_DEBUG(format,arg...)
#endif
#define BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE   (1)
#define BR_IGMP_SNOOPING_SEND_V3_REPORT_FALSE  (0)

#define BR_IGMP_SNOOPING_ROBUST_TIMER_NUM_MAX  (16)
extern unsigned int g_lIgmpReportCount;
extern unsigned int g_lIgmpReportInterval;
spinlock_t		    g_igmp_robust_lock = SPIN_LOCK_UNLOCKED;
struct net_bridge_igmp_robust_timerlist g_igmprobusttimer[BR_IGMP_SNOOPING_ROBUST_TIMER_NUM_MAX];

void br_igmp_snooping_init(void)
{
	INIT_LIST_HEAD(&igmp_snooping_list.igmp_list);
	igmp_snooping_list.igmp_lock = SPIN_LOCK_UNLOCKED;
	igmp_snooping_list.igmp_start_timer = 0;
	igmp_snooping_list.igmp_snooping_enable = 0;

    /* BEGIN: Added by z67728, 2010/7/13 For IGMPv2 query */
    init_timer(&g_igmpquerylist.querytimer.igmp_timer);
    g_igmpquerylist.igmp_lock = SPIN_LOCK_UNLOCKED;

    memset(g_igmprobusttimer,0,sizeof(g_igmprobusttimer));
    /* END:   Added by z67728, 2010/7/13 */
}

/*****************************************************************************
 函 数 名  : br_igmp_snooping_robust_timeout
 功能描述  : 桥接鲁棒定时器超时处理函数。
 输入参数  : unsigned long ptr  
 输出参数  : 无
 返 回 值  : static void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2010年12月16日
    作    者   : z67728
    修改内容   : 新生成函数

*****************************************************************************/
static void br_igmp_snooping_robust_timeout(unsigned long ptr)
{
    struct net_bridge_igmp_robust_timerlist *pstrobust = 
        (struct net_bridge_igmp_robust_timerlist *)ptr;
    int             delay = 0;
    struct sk_buff *skb2  = NULL;

    if ( NULL == pstrobust )
    {
        BR_IGMP_SNOOPING_DEBUG("%s","pstrobust is NULL.");
        return;
    }

    if ( NULL == pstrobust->skb )
    {
        BR_IGMP_SNOOPING_DEBUG("%s","pstrobust->skb is NULL.");
        pstrobust->use = 0;
        del_timer(&pstrobust->timer);
        return;
    }

    if ( NULL == pstrobust->br )
    {
        BR_IGMP_SNOOPING_DEBUG("%s","pstrobust->br is NULL.");
        pstrobust->use = 0;
        kfree_skb(pstrobust->skb);
        del_timer(&pstrobust->timer);
        return;
    }
    
    if ( 1 < pstrobust->num )
    {
        skb2 = skb_clone((struct sk_buff *)pstrobust->skb, GFP_ATOMIC);
    }
    else
    {
        skb2 = (struct sk_buff *)pstrobust->skb;
    }

    br_forward_igmpv3report(pstrobust->br,skb2);
    pstrobust->num--;
    BR_IGMP_SNOOPING_DEBUG("Send packet,num[%u].",pstrobust->num);

    if ( 0 < pstrobust->num )
    {
        delay = random32() % pstrobust->interval;
        mod_timer(&pstrobust->timer, jiffies + delay + 2);
    }
    else
    {
        pstrobust->use = 0;
        del_timer(&pstrobust->timer);
    }
    return;
}

/*****************************************************************************
 函 数 名  : br_igmp_snooping_robustfunc
 功能描述  : BT桥接鲁棒性发包函数。
 输入参数  : struct net_bridge *br  
             struct sk_buff *skb    
 输出参数  : 无
 返 回 值  : static void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2010年12月16日
    作    者   : z67728
    修改内容   : 新生成函数

*****************************************************************************/
static void br_igmp_snooping_robustfunc(struct net_bridge *br,struct sk_buff *skb)
{
    int loop = 0;

    if ( NULL == br || NULL == skb )
    {
        BR_IGMP_SNOOPING_DEBUG("%s","br or skb is NULL.");
        return;
    }

    spin_lock_bh(&g_igmp_robust_lock);
    for ( loop = 0 ; loop < BR_IGMP_SNOOPING_ROBUST_TIMER_NUM_MAX ; loop++ )
    {
        if ( 0 == g_igmprobusttimer[loop].use )
        {
            g_igmprobusttimer[loop].skb      = skb_clone(skb, GFP_ATOMIC);
            if ( NULL == g_igmprobusttimer[loop].skb )
            {
                break;
            }

            g_igmprobusttimer[loop].use      = 1;
            g_igmprobusttimer[loop].num      = g_lIgmpReportCount - 1;
            g_igmprobusttimer[loop].interval = g_lIgmpReportInterval;
            g_igmprobusttimer[loop].br       = br;

            init_timer(&g_igmprobusttimer[loop].timer);
            g_igmprobusttimer[loop].timer.expires  = jiffies + (random32() % g_lIgmpReportInterval);
    	    g_igmprobusttimer[loop].timer.function = br_igmp_snooping_robust_timeout;
    	    g_igmprobusttimer[loop].timer.data     = (unsigned long)(&g_igmprobusttimer[loop]);
    	    add_timer(&g_igmprobusttimer[loop].timer);
            BR_IGMP_SNOOPING_DEBUG("Use timer[%d] num[%u] interval[%u].",
                loop,g_igmprobusttimer[loop].num,g_igmprobusttimer[loop].interval);
            break;
        }
    }

    spin_unlock_bh(&g_igmp_robust_lock);
    return;
}

static void br_igmp_snooping_query_timeout(unsigned long ptr)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;
	struct net_bridge_igmp_snooping *igmp_list;

	igmp_list = (struct net_bridge_igmp_snooping *) ptr;

	spin_lock_bh(&igmp_list->igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_list->igmp_list, list) 
    {
	    if ((jiffies > dst_entry->time) && ((jiffies - dst_entry->time) < (IGMP_QUERY_TIMEOUT*HZ)))
	    {
    		list_del_rcu(&dst_entry->list);
    		kfree(dst_entry);
	    }
	}
	spin_unlock_bh(&igmp_list->igmp_lock);
		
	mod_timer(&igmp_list->igmp_timer, jiffies + IGMP_TIMER_CHECK_TIMEOUT*HZ);		
}

static int br_igmp_snooping_update(struct net_bridge_port *dev_port, unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;
	int ret = 0;
    
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if (!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN))
	    {
    		if ((!memcmp(&dst_entry->host_mac, host_mac, ETH_ALEN)) && (dst_entry->dev_dst == dev_port))
    		{    		
                dst_entry->time = jiffies + IGMP_QUERY_TIMEOUT*HZ;
    		    ret = 1;
    		}
	    }
	}
	
	return ret;
}

static struct net_bridge_igmp_snooping_entry *br_igmp_snooping_get(struct net_bridge_port *dev_port, unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;

	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if ((!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN)) && (!memcmp(&dst_entry->host_mac, host_mac, ETH_ALEN))) 
	    {
		    if (dst_entry->dev_dst == dev_port)
		    {
    		    return dst_entry;
		    }
	    }
	}
	
	return NULL;
}

static int br_igmp_snooping_add(struct net_bridge_port *dev_port, unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_igmp_snooping_entry *igmp_entry;
		    
	if (br_igmp_snooping_update(dev_port, grp_mac, host_mac))
	{
    	return 0;
	}

	igmp_entry = kmalloc(sizeof(struct net_bridge_igmp_snooping_entry), GFP_ATOMIC);
	if (!igmp_entry)
	{
	    return ENOMEM;
	}	
	memcpy(igmp_entry->host_mac, host_mac, ETH_ALEN);	
	memcpy(igmp_entry->grp_mac, grp_mac, ETH_ALEN);
	igmp_entry->time = jiffies + IGMP_QUERY_TIMEOUT*HZ;
    igmp_entry->dev_dst = dev_port;
    
	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	list_add_rcu(&igmp_entry->list, &igmp_snooping_list.igmp_list);
	spin_unlock_bh(&igmp_snooping_list.igmp_lock);

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
    blog_notify(MCAST_CONTROL_EVT, (void*)igmp_entry,
					BLOG_PARAM1_MCAST_ADD, BLOG_PARAM2_MCAST_IPV4);
#endif
	if (!igmp_snooping_list.igmp_start_timer) 
	{
    	init_timer(&igmp_snooping_list.igmp_timer);
	    igmp_snooping_list.igmp_timer.expires = jiffies + IGMP_TIMER_CHECK_TIMEOUT*HZ;
	    igmp_snooping_list.igmp_timer.function = br_igmp_snooping_query_timeout;
	    igmp_snooping_list.igmp_timer.data = (unsigned long)(&igmp_snooping_list);
	    add_timer(&igmp_snooping_list.igmp_timer);
	    igmp_snooping_list.igmp_start_timer = 1;
	}

	return 1;
}

static int br_igmp_snooping_del(struct net_bridge_port *dev_port, unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;
    
	if ((dst_entry = br_igmp_snooping_get(dev_port, grp_mac, host_mac))) 
	{
    	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	    list_del_rcu(&dst_entry->list);
    #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
        blog_notify(MCAST_CONTROL_EVT, (void*)dst_entry,
					BLOG_PARAM1_MCAST_DEL, BLOG_PARAM2_MCAST_IPV4);
    #endif              
	    kfree(dst_entry);
    	spin_unlock_bh(&igmp_snooping_list.igmp_lock);

	    return 1;
	}
	
	return 0;
}

/* BEGIN: Added by h00169677, 2012/6/18   PN:DTS2012061404250 */
int br_igmp_snooping_del_by_nbp(struct net_bridge_port *dev_port)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;

	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
		if (dst_entry ->dev_dst == dev_port) 
		{
			    list_del_rcu(&dst_entry->list);
			    kfree(dst_entry);
		    #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
			    blog_notify(MCAST_CONTROL_EVT, (void*)dst_entry,
					BLOG_PARAM1_MCAST_DEL, BLOG_PARAM2_MCAST_IPV4);
		    #endif	    

		}
	}
    spin_unlock_bh(&igmp_snooping_list.igmp_lock);
    
	return 0;
}
/* END:   Added by h00169677, 2012/6/18 */

static int br_igmp_snooping_empty(unsigned char *grp_mac)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;
    
	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if ((!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN))) 
	    {
			spin_unlock_bh(&igmp_snooping_list.igmp_lock);
	    	return 0;
	    }
	}
	spin_unlock_bh(&igmp_snooping_list.igmp_lock);
	return 1;
}

void br_igmp_snooping_show(void)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;

	printk("\nigmp snooping: <%s>\n",(igmp_snooping_list.igmp_snooping_enable?"Enable":"Disable"));

	printk("\nbridge	device	group			source			timeout\n");

	/* BEGIN: Added by h00169677, 2012/6/18   PN:DTS2012061404250 */
	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	/* END:   Added by h00169677, 2012/6/18 */
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if (NULL == dst_entry->dev_dst)
	    {
	        continue;
	    }
	    printk("%s   %s  	",dst_entry->dev_dst->br->dev->name, dst_entry->dev_dst->dev->name);
        printk("%02x:%02x:%02x:%02x:%02x:%02x",dst_entry->grp_mac[0],
                                               dst_entry->grp_mac[1],
                                               dst_entry->grp_mac[2],
                                               dst_entry->grp_mac[3],
                                               dst_entry->grp_mac[4],
                                               dst_entry->grp_mac[5]);
	    printk("	");
        printk("%02x:%02x:%02x:%02x:%02x:%02x",dst_entry->host_mac[0],
                                               dst_entry->host_mac[1],
                                               dst_entry->host_mac[2],
                                               dst_entry->host_mac[3],
                                               dst_entry->host_mac[4],
                                               dst_entry->host_mac[5]);
	    printk("	%d\n", (int) (dst_entry->time - jiffies)/HZ);
	}
    /* BEGIN: Added by h00169677, 2012/6/18   PN:DTS2012061404250 */
    spin_unlock_bh(&igmp_snooping_list.igmp_lock);
    /* END:   Added by h00169677, 2012/6/18 */
}


void br_igmp_snooping_clear(void)
{
	struct net_bridge_igmp_snooping_entry *dst_entry;
   
	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    list_del_rcu(&dst_entry->list);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
    	blog_notify(MCAST_CONTROL_EVT, (void*)dst_entry,
    					BLOG_PARAM1_MCAST_DEL, BLOG_PARAM2_MCAST_IPV4);
#endif	
	    kfree(dst_entry);
	}
	del_timer(&(igmp_snooping_list.igmp_timer));
	spin_unlock_bh(&igmp_snooping_list.igmp_lock);
}

/* DT c4163 add for AU8D04182, , add by z67728 for BT IGMP function. This allow interface which don't join currrent multicast group reciving IGMP report packet. */
unsigned int br_igmp_snooping_no_v1v2report(const struct net_bridge_port *dev_port, struct sk_buff *skb)
{
	unsigned int no_report = 1;
	unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_igmp_snooping_entry *dst_entry;
    /* BEGIN: Added by z67728, 2010/7/13 For BT IGMPv2 query */
    struct net_bridge_igmp_query_group *pstCurNode = NULL;
    /* END:   Added by z67728, 2010/7/13 */

	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list) 
	{
	    if (!memcmp(&dst_entry->grp_mac, dest, ETH_ALEN) && (dst_entry->dev_dst == dev_port)) 
        {
			no_report = 0;
			break;
	    }
        /* BEGIN: Added by z67728, 2010/7/13 For BT IGMPv2 query */        
        else if ( 1 == g_igmpquerylist.querytimer.igmp_query_enable 
            && NULL != (pstCurNode = br_igmp_snooping_find_query_node(dest,dev_port->dev->name))
            && ( IS_WAN_DEVICE(dev_port->dev->name)) )
        {
            br_igmp_snooping_del_query_node(dest);
            break;
        }
        /* END:   Added by z67728, 2010/7/13 */
        /* BEGIN: Added by z67728, 2010/7/13 For BT : the port which don't join group can receive igmp report packet. */        
        else if (!br_igmp_snooping_empty(dest) && ( IS_WAN_DEVICE(dev_port->dev->name)))
        {
            /* 已经加入组的report报文不需要从wan侧转发 */
            no_report = 0;
			break;
        }
        /* END:   Added by z67728, 2010/7/13 */
	}

	return no_report;
}
/* DT c4163 end */

int br_igmp_snooping_forward(struct sk_buff *skb, struct net_bridge *br,unsigned char *dest,int forward)
{
    int k;
    int status = 0;
    unsigned char mc_mac[6];
    unsigned char *data = skb->data;
    unsigned short protocol ;
    unsigned short usIpHeadLen = 0;
    igmp_ipaddr ipaddr;
    struct sk_buff *skb2;
    struct net_bridge_igmp_snooping_entry *dst_entry;    
    struct igmpv3_report *pstRepV3;
    struct igmpv3_grec   *pstGrec;
    unsigned int forward_map = 0;
#ifdef CONFIG_ATP_BRIDGE_BIND
    struct bridgebind_group *pgroup = NULL;
#endif
    unsigned int iCnt = 0;
    /* BEGIN: Added by z67728, 2010/10/29 For IGMPv3 query and report. */
    struct net_bridge_igmp_query_group *pstCurNode         = NULL;
    unsigned int                        ulSendv3ReportFlag = BR_IGMP_SNOOPING_SEND_V3_REPORT_FALSE;
    unsigned int                        ulRobustFlag       = BR_IGMP_SNOOPING_SEND_V3_REPORT_FALSE;
    /* END:   Added by z67728, 2010/10/29 */

    /*start by f00184228 DTS2011111007229 snooping不生效*/
    struct iphdr *iph;
    /*end by f00184228 DTS2011111007229 snooping不生效*/

    protocol = ((u16 *) data)[-1];

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
#if  defined(CONFIG_BR_IGMP_STATISTIC)
    if ( 1 == g_ulIgmpStatisticEnableSwitch ){
        blog_skip(skb);
    }
#else
#ifndef CONFIG_BCM96362
    blog_skip(skb);
#endif
#endif
#endif

    if (!igmp_snooping_list.igmp_snooping_enable || protocol != __constant_htons(ETH_P_IP))
    {
        return 0;
    }

    /*start by f00184228 DTS2011111007229 对于特殊地址snooping不生效*/
    iph = ip_hdr(skb);
    if ((1 == igmp_snooping_list.igmp_snooping_enable)
        && iph && ((__constant_htonl(upnp_ip) == iph->daddr)
        ||(__constant_htonl(ripv2_ip) == iph->daddr)
        ||(__constant_htonl(mdns_ip) == iph->daddr)))
    {
        BR_IGMP_SNOOPING_DEBUG("return with %u",iph->daddr);
        return 0;
    }
    /*end by f00184228 DTS2011111007229 对于特殊地址snooping不生效*/
    
    if (data[9] == IPPROTO_IGMP) 
    {
        /* AU4D00944 AU4D00955: 增加skb->dev->br_port指针合法性判断，否则br_nf_forward_ip会出错 */
        if (NULL == skb->dev->br_port)
        {
            return 0;
        }
        usIpHeadLen = (skb->data[0] & 0x0f) * 4;
        if ((data[usIpHeadLen] == IGMPV2_HOST_MEMBERSHIP_REPORT 
            || data[usIpHeadLen] == IGMP_HOST_MEMBERSHIP_REPORT)
             && protocol == __constant_htons(ETH_P_IP))
        {   
			if (!br_igmp_snooping_empty(dest))
            {
                status = 1;
            }
            else
            {
                BR_IGMP_SNOOPING_DEBUG("%s","IGMPv2 Join group.");
                br_igmp_snooping_robustfunc(br,skb);
            }

            br_igmp_snooping_add(skb->dev->br_port, dest, eth_hdr(skb)->h_source);
			
            /* DT c4163 add for AU8D04182, , add by z67728 for BT IGMP function. This allow interface which don't join currrent multicast group reciving IGMP report packet. */
			if (forward) {
                skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2 != 0) {
					br_flood_forward_igmpv1v2report(br, skb2);
				}
			}
			/* DT c4163 end */
        }
        else if (data[usIpHeadLen] == IGMPV3_HOST_MEMBERSHIP_REPORT && 
            protocol == __constant_htons(ETH_P_IP))
        {
            pstRepV3 = (struct igmpv3_report *)&data[usIpHeadLen];
            pstGrec = &pstRepV3->grec[0];
            for (k = 0; k < pstRepV3->ngrec; k++)
            {
                ipaddr.ulIpAddr = pstGrec->grec_mca;
                mc_mac[0] = 0x01;
                mc_mac[1] = 0x00;
                mc_mac[2] = 0x5e;
                mc_mac[3] = 0x7f & ipaddr.acIpAddr[1];
                mc_mac[4] = ipaddr.acIpAddr[2];
                mc_mac[5] = ipaddr.acIpAddr[3];
                if ((IGMPV3_MODE_IS_EXCLUDE == pstGrec->grec_type) || 
                    (IGMPV3_CHANGE_TO_EXCLUDE == pstGrec->grec_type) || 
                    (pstGrec->grec_nsrcs != 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
                    (pstGrec->grec_nsrcs != 0 && IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type) ||
                    (IGMPV3_ALLOW_NEW_SOURCES == pstGrec->grec_type)) 
                {
#if 1
                    BR_IGMP_SNOOPING_DEBUG("Current v3 mac[%02x:%02x:%02x:%02x:%02x:%02x]",mc_mac[0],
                                                                                           mc_mac[1],
                                                                                           mc_mac[2],
                                                                                           mc_mac[3],
                                                                                           mc_mac[4],
                                                                                           mc_mac[5]);
                    if (!br_igmp_snooping_empty(mc_mac))
        		    {
            		    /* BEGIN: Added by z67728, 2010/10/29 For IGMPv3 query and report. */
        		        if ( 1 == g_igmpquerylist.querytimer.igmp_query_enable 
                            && NULL != (pstCurNode = br_igmp_snooping_find_query_node(mc_mac,NULL)))
                        {
                            BR_IGMP_SNOOPING_DEBUG("Find mac[%02x:%02x:%02x:%02x:%02x:%02x]",mc_mac[0],
                                                                                             mc_mac[1],
                                                                                             mc_mac[2],
                                                                                             mc_mac[3],
                                                                                             mc_mac[4],
                                                                                             mc_mac[5]);
                            br_igmp_snooping_del_query_node(mc_mac);
                            ulSendv3ReportFlag = BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE;
                        }
            		    /* END:   Added by z67728, 2010/10/29 */
                        iCnt += 1;
        		    }
                    else
                    {
                        ulRobustFlag = BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE;
                    }
                    
        		    br_igmp_snooping_add(skb->dev->br_port, mc_mac, eth_hdr(skb)->h_source);
#else
                    br_igmp_snooping_add(skb->dev->br_port, mc_mac, eth_hdr(skb)->h_source);
#endif
        		}
        		else if ((IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type && pstGrec->grec_nsrcs == 0) || 
                         (pstGrec->grec_nsrcs == 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
                         (IGMPV3_BLOCK_OLD_SOURCES == pstGrec->grec_type)) 
                {
                    if (!br_igmp_snooping_empty(mc_mac))
                    {
                        ulRobustFlag = BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE;
                    }
                    
        		    br_igmp_snooping_del(skb->dev->br_port, mc_mac, eth_hdr(skb)->h_source);
                    if (!br_igmp_snooping_empty(mc_mac))
                    {
                        iCnt += 1;
                    }
                }
                pstGrec = (struct igmpv3_grec *)((char *)pstGrec + sizeof(struct igmpv3_grec) + pstGrec->grec_nsrcs * sizeof(struct in_addr)); 
            }
            
            if (iCnt == pstRepV3->ngrec)
            {
                // TODO: 尽管这种报文不应该被snooping转发但是这种报文还是需要送给proxy处理。
                /* BEGIN: Modified by h00169677, 2012/7/3   PN:DTS2012053007052 */
                if (forward == 0)
                {
                    status = 1;
                }
                /* END:   Modified by h00169677, 2012/7/3 */
            }
            else
            {
                if ( BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE == ulRobustFlag )
                {
                    BR_IGMP_SNOOPING_DEBUG("%s","IGMPv3 Process packet.");
                    br_igmp_snooping_robustfunc(br,skb);
                }
            }

            /* BEGIN: Added by z67728, 2010/10/29 For IGMPv3 query and report. */
            if ( BR_IGMP_SNOOPING_SEND_V3_REPORT_TRUE == ulSendv3ReportFlag && 0 != forward )
            {
                BR_IGMP_SNOOPING_DEBUG("%s","Send igmpv3 report packet to wan.");
                /* Send IGMPv3 report packet through wan bridge port. */
				if ( NULL != (skb2 = skb_clone(skb, GFP_ATOMIC)))
                {
                    BR_IGMP_SNOOPING_DEBUG("%s","Clone packet success.");
					br_forward_igmpv3report(br, skb2);
				}
            }
            /* END:   Added by z67728, 2010/10/29 */
        }
        else if (data[usIpHeadLen] == IGMP_HOST_LEAVE_MESSAGE) 
        {
            mc_mac[0] = 0x01;
            mc_mac[1] = 0x00;
            mc_mac[2] = 0x5e;
            mc_mac[3] = 0x7f & data[usIpHeadLen + 5];
            mc_mac[4] = data[usIpHeadLen + 6];
            mc_mac[5] = data[usIpHeadLen + 7];

            br_igmp_snooping_del(skb->dev->br_port, mc_mac, eth_hdr(skb)->h_source);
            if (br_igmp_snooping_empty(mc_mac))
            {
                BR_IGMP_SNOOPING_DEBUG("%s","IGMPv2 Leave packet.");
                br_igmp_snooping_robustfunc(br,skb);

                skb2 = skb_clone(skb, GFP_ATOMIC);
                /* start igmp snooping NULL pointer A36D08515 by f00110348 */
                if (NULL == skb2)
                {
                    return 0;
                }
                /* end igmp snooping NULL pointer A36D08515 by f00110348 */
	        #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
                blog_clone(skb, blog_ptr(skb2));
            #endif
                /* start of AU4D01376 by d00107688 2009-02-16  存在一条PPPoE的WAN,同时开启IGMP Snooping与IGMP Proxy,且将该wan与IGMP Proxy绑定,再将IGMP Proxy去使能,网关会打印cpu 0重启 */
                if (forward)
                {
                    br_flood_forward(br, skb2);
                }
                else
                {
                    br_flood_deliver(br, skb2);
                }
                /* end of AU4D01376 by d00107688 2009-02-16  存在一条PPPoE的WAN,同时开启IGMP Snooping与IGMP Proxy,且将该wan与IGMP Proxy绑定,再将IGMP Proxy去使能,网关会打印cpu 0重启 */
            }		    
            status = 1;
        }
        else if (data[usIpHeadLen] == IGMP_HOST_MEMBERSHIP_QUERY
              && protocol == __constant_htons(ETH_P_IP)
              && IS_WAN_DEVICE(skb->dev->name))
        {
            struct net_bridge_igmp_snooping_entry *dst_entry  = NULL;
            struct net_bridge_igmp_query_group *   query_node = NULL;

        	spin_lock_bh(&igmp_snooping_list.igmp_lock);
        	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
        	{
        	    /*使用skb->dev->name的原因是保证从哪个wan收到query报文则从哪条wan转发report.*/
        	    query_node = br_igmp_snooping_find_query_node(dst_entry->grp_mac,skb->dev->name);
                if ( NULL == query_node )
                {
                    BR_IGMP_SNOOPING_DEBUG("Add %s mac[%02x:%02x:%02x:%02x:%02x:%02x] to query list",skb->dev->name,
                                                                                                     dst_entry->grp_mac[0],
                                                                                                     dst_entry->grp_mac[1],
                                                                                                     dst_entry->grp_mac[2],
                                                                                                     dst_entry->grp_mac[3],
                                                                                                     dst_entry->grp_mac[4],
                                                                                                     dst_entry->grp_mac[5]);
                    br_igmp_snooping_add_query_node(dst_entry->grp_mac,skb->dev->name);
                }
        	}
        	spin_unlock_bh(&igmp_snooping_list.igmp_lock);

            spin_lock_bh(&g_igmpquerylist.igmp_lock);
            g_igmpquerylist.querytimer.igmp_timer.expires  = jiffies + data[usIpHeadLen + 1]*HZ/IGMP_TIMER_CHECK_TIMEOUT;
    	    g_igmpquerylist.querytimer.igmp_timer.function = br_igmp_snooping_query_report_timeout;
    	    g_igmpquerylist.querytimer.igmp_timer.data     = (unsigned long)(&igmp_snooping_list);

            /* WAN侧收到qurey报文,需要特定组可以上报V2 report报文. */
            if ( 0 == g_igmpquerylist.querytimer.igmp_start_timer )
            {
        	    add_timer(&g_igmpquerylist.querytimer.igmp_timer);
        	    g_igmpquerylist.querytimer.igmp_start_timer  = 1;
                g_igmpquerylist.querytimer.igmp_query_enable = 1;
            }
            else
            {
                /* 需要添加定时器未完成操作时代码 */
                mod_timer(&g_igmpquerylist.querytimer.igmp_timer,g_igmpquerylist.querytimer.igmp_timer.expires);
            }

            spin_unlock_bh(&g_igmpquerylist.igmp_lock);
        }

        return status;
    }

    /* start of by d00107688 2010-11-26 如果两个PC加入同一个组播组会导致系统挂死的规避，这段代码需要去掉 */
#if 0
    iCnt = 0;
    list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
    {
        if (!memcmp(&dst_entry->grp_mac, dest, ETH_ALEN))
        {
            iCnt++;
        }
    }
    if (iCnt > 1)
    {
        blog_skip(skb);
    }
#endif
    /* End of by d00107688 2010-11-26 如果两个PC加入同一个组播组会导致系统挂死的规避，这段代码需要去掉 */
    
    forward_map = 0;
    list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
    {
        if (!memcmp(&dst_entry->grp_mac, dest, ETH_ALEN)) 
        {
            if (NULL == dst_entry->dev_dst
            || (forward_map & (1<<dst_entry->dev_dst->port_no)))
            {
                continue;
            }
#ifdef CONFIG_ATP_BRIDGE_BIND
            if(br->group_enable)
            {
                pgroup = br_get_group(br, brgetgroup(br, skb));

                if(((NULL != pgroup)
                && (ISIN_GROUP(pgroup->ports, dst_entry->dev_dst->port_no)))
                || (!forward)) //路由WAN的组播流不判断绑定组
                {
                    skb2 = skb_clone(skb, GFP_ATOMIC);
                    /* start igmp snooping NULL pointer A36D08515 by f00110348 */
                    if (NULL == skb2)
                    {
                        return 0;
                    }                    
                    /* end igmp snooping NULL pointer A36D08515 by f00110348 */
                #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
                    blog_clone(skb, blog_ptr(skb2));
                #endif
                    if (forward)
                    {
                        br_forward(dst_entry->dev_dst, skb2);
                    }
                    else
                    {
                        br_deliver(dst_entry->dev_dst, skb2);
                    }
                }

                if( NULL != pgroup ) 
                {
                    br_release_group(pgroup);
                }
            }
            else
            {
#endif
            /* BEGIN: Modified by h00169677, 2012/7/17   PN:DTS2012071603967 */
            /* BEGIN: Modified by h00169677, 2012/7/3   PN:DTS2012053007052  */
                if (forward) //桥接snooping
                {
                    if (0 == memcmp(dst_entry->dev_dst->br->dev->name,skb->dev->br_port->br->dev->name,strlen(skb->dev->br_port->br->dev->name)))
                    {
        				forward_map |= (1<<dst_entry->dev_dst->port_no);
                        skb2 = skb_clone(skb, GFP_ATOMIC);
                        /* start igmp snooping NULL pointer A36D08515 by f00110348 */
                        if (NULL == skb2)
                        {
                            return 0;
                        }
                        /* end igmp snooping NULL pointer A36D08515 by f00110348 */
                    #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
                        blog_clone(skb, blog_ptr(skb2));
                    #endif
                        br_forward(dst_entry->dev_dst, skb2);
                        
                    }
                }
                else //路由snooping
                {
                    if (0 == memcmp(dst_entry->dev_dst->br->dev->name,skb->dev->name,strlen(skb->dev->name)))
                    {
        				forward_map |= (1<<dst_entry->dev_dst->port_no);
                        skb2 = skb_clone(skb, GFP_ATOMIC);
                        /* start igmp snooping NULL pointer A36D08515 by f00110348 */
                        if (NULL == skb2)
                        {
                            return 0;
                        }
                        /* end igmp snooping NULL pointer A36D08515 by f00110348 */
                    #if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
                        blog_clone(skb, blog_ptr(skb2));
                    #endif
                        
                        br_deliver(dst_entry->dev_dst, skb2);
                    }
                }
            /* END:   Modified by h00169677, 2012/7/3 */
            /* END:   Modified by h00169677, 2012/7/17 */
#ifdef CONFIG_ATP_BRIDGE_BIND                
            }
#endif            
            status = 1;
        }	    
    }
    forward_map = 0;

#if 1
    memset(mc_mac, 0xff, 6);
    if((1 == igmp_snooping_list.igmp_snooping_enable) 
          && memcmp(mc_mac, eth_hdr(skb)->h_dest, ETH_ALEN)
          && (skb->dev && (!IS_LAN_DEV(skb->dev->name))))
    {
        status = 1;
    }
#endif

    if ((!forward) && (status))
    {
        kfree_skb(skb);
    }

    return status;
}

void br_igmp_snooping_set_enable(int enable)
{
    igmp_snooping_list.igmp_snooping_enable = enable;
}

int br_igmp_snooping_get_enable(void)
{
    return igmp_snooping_list.igmp_snooping_enable;
}

/* BEGIN: Added by z67728, 2010/7/12 FOR BT IGMP : 当snooping记录加入组信息后,
   主机发送的IGMPv2报告报文将不会通过桥端口,这样将会导致组播流超时中断.当收到
   来自桥接wan的查询报文后,snooping允许对应组的IGMPv2报告报文通过.*/
void br_igmp_snooping_query_report_timeout(unsigned long ptr)
{
    g_igmpquerylist.querytimer.igmp_start_timer  = 0;
    g_igmpquerylist.querytimer.igmp_query_enable = 0;
    del_timer(&g_igmpquerylist.querytimer.igmp_timer);
    br_igmp_snooping_free_query_chain();
    return;
}

void br_igmp_snooping_free_query_chain(void)
{
    struct net_bridge_igmp_query_group *pstCurNode = NULL;

    if ( NULL == g_igmpquerylist.queruchain )
    {
        return;
    }

    while ( NULL != g_igmpquerylist.queruchain )
    {
        pstCurNode = g_igmpquerylist.queruchain;
        g_igmpquerylist.queruchain = g_igmpquerylist.queruchain->next;
        kfree(pstCurNode);
    }
    
    return;
}

int br_igmp_snooping_del_query_node(unsigned char * dest)
{
    struct net_bridge_igmp_query_group *pstCurNode = NULL;
    struct net_bridge_igmp_query_group *pstPreNode = NULL;

    if ( NULL == dest )
    {
        return -1;
    }

    if ( NULL == g_igmpquerylist.queruchain )
    {
        return 0;
    }
    
    spin_lock_bh(&g_igmpquerylist.igmp_lock);
    
    pstCurNode = g_igmpquerylist.queruchain;
    while ( pstCurNode )
    {
        if ( 0 == memcmp(dest,pstCurNode->grp_mac,ETH_ALEN) )
        {
            if ( NULL == pstPreNode )
            {
                g_igmpquerylist.queruchain = pstCurNode->next;
            }
            else
            {
                pstPreNode->next = pstCurNode->next;
            }
            
            kfree(pstCurNode);
            pstCurNode = NULL;
            break;
        }

        pstPreNode = pstCurNode;
        pstCurNode = pstCurNode->next;
    }

    spin_unlock_bh(&g_igmpquerylist.igmp_lock);
    
    return 0;
}

struct net_bridge_igmp_query_group *br_igmp_snooping_find_query_node(unsigned char * dest,unsigned char *interface)
{
    struct net_bridge_igmp_query_group *pstCurNode = NULL;
    
    if ( NULL == g_igmpquerylist.queruchain || NULL == dest )
    {
        return NULL;
    }

    spin_lock_bh(&g_igmpquerylist.igmp_lock);

    pstCurNode = g_igmpquerylist.queruchain;
    while ( pstCurNode )
    {
        /* 为兼容IGMPv3查询,可以设置interface为空 */
        if ( NULL != interface 
          && 0 == memcmp(dest,pstCurNode->grp_mac,ETH_ALEN) 
          && 0 == strcmp(interface,pstCurNode->interface) )
        {
            spin_unlock_bh(&g_igmpquerylist.igmp_lock);
            return pstCurNode;
        }
        else if (NULL == interface && 0 == memcmp(dest,pstCurNode->grp_mac,ETH_ALEN))
        {
            spin_unlock_bh(&g_igmpquerylist.igmp_lock);
            return pstCurNode;
        }
        
        pstCurNode = pstCurNode->next;
    }

    spin_unlock_bh(&g_igmpquerylist.igmp_lock);
    return NULL;
}

int br_igmp_snooping_add_query_node(unsigned char * dest,unsigned char *interface)
{
    struct net_bridge_igmp_query_group *pstNewNode = NULL;
    
    if ( NULL == dest )
    {
        return -1;
    }

    pstNewNode = (struct net_bridge_igmp_query_group *)kmalloc(sizeof(struct net_bridge_igmp_query_group),GFP_KERNEL);
    if ( NULL == pstNewNode )
    {
        return -1;
    }

    memset(pstNewNode,0,sizeof(struct net_bridge_igmp_query_group));

    memcpy(pstNewNode->grp_mac,dest,ETH_ALEN);
    strcpy(pstNewNode->interface,interface);

    spin_lock_bh(&g_igmpquerylist.igmp_lock);

    if ( NULL == g_igmpquerylist.queruchain )
    {
        g_igmpquerylist.queruchain = pstNewNode;
    }
    else
    {
        pstNewNode->next            = g_igmpquerylist.queruchain;
        g_igmpquerylist.queruchain  = pstNewNode;
    }

    spin_unlock_bh(&g_igmpquerylist.igmp_lock);
        
    return 0;
}
/* END:   Added by z67728, 2010/7/12 */

#if defined(CONFIG_BR_IGMP_STATISTIC)
/* BEGIN: Added by z67728, 2010/7/28 For BT IGMP statistic */
int br_igmp_snooping_find_intrface_group_num(const char*pcInterface)
{
    int    lCount = 0;
    struct net_bridge_igmp_snooping_entry *dst_entry = NULL;

    if ( NULL == pcInterface )
    {
        return 0;
    }
    
	spin_lock_bh(&igmp_snooping_list.igmp_lock);
	list_for_each_entry_rcu(dst_entry, &igmp_snooping_list.igmp_list, list)
	{
	    if ( 0 == strcmp(pcInterface,dst_entry->dev_dst->dev->name)) 
	    {
		    lCount++;
	    }
	}
	spin_unlock_bh(&igmp_snooping_list.igmp_lock);

    return lCount;
}
/* END:   Added by z67728, 2010/7/28 */
#endif

