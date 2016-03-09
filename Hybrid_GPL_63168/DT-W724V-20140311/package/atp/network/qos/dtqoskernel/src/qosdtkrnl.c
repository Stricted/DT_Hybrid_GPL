#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/nbuff.h>
#include <linux/ip.h>
#include <linux/in.h>
#include "qosdtkrnl.h"

struct DT_QOS_STAT_S s_dtQosStat[] = {
	{0,           0, 0, (2000 * 1 * 8 * HZ)}, 
	{(15 * 1024), 0, 0, (2000 * 1 * 8 * HZ)}, 
};

int sysctl_lan_host_ip = 0;

unsigned int   s_dtQosMarkToQue[]     = {1, 0, 2, 2, 3, 3, 2, 4};
unsigned short g_usDtQosMarkto8021P[] = {0, 0, 3, 4, 6, 6, 0, 0};
unsigned int   g_ulDtQos8021PtoMark[] = {0x0, 0x0, 0x0, 0x2, 0x3, 0x0, 0x5, 0x0};
unsigned char  s_ucDtQosMarkToTOS[]   = {0x0, 0x0, 0x60, 0x80, 0xA0, 0xC0, 0x0, 0x0};

EXPORT_SYMBOL(g_usDtQosMarkto8021P);
EXPORT_SYMBOL(g_ulDtQos8021PtoMark);

void SkbMarkForHighestQueue(struct sk_buff *skb)
{
    unsigned char iControlPkt = 0;
    unsigned char ucLcp1 = 0;
    unsigned char ucLcp2 = 0;
#ifndef CONFIG_BR_IGMP_SNOOP
    if (1 == skb->data[0])
    {
        /* igmp */
        iControlPkt = 1;
    }
#endif /* CONFIG_BR_IGMP_SNOOP */
    if (__constant_htons(ETH_P_PPP_DISC) == skb->protocol) 
	{
        iControlPkt = 1;
	}  
	else if (__constant_htons(ETH_P_PPP_SES) == skb->protocol) 
	{
        ucLcp1 = skb->data[60];
        ucLcp2 = skb->data[61];

	    if (((0xc0 == ucLcp1) && (0x21 == ucLcp2))
	    	||((0xc2 == ucLcp1) && (0x23 == ucLcp2))  //chap
	    	||((0xc0 == ucLcp1) && (0x23 == ucLcp2))  //pap
	    	||((0x80 == ucLcp1) && (0x57 == ucLcp2))
	    	||((0x80 == ucLcp1) && (0x21 == ucLcp2)))
        {	
            iControlPkt = 1;
        }

        ucLcp1 = skb->data[58];
        ucLcp2 = skb->data[59];
        if (((0xc0 == ucLcp1) && (0x21 == ucLcp2))
	    	||((0xc2 == ucLcp1) && (0x23 == ucLcp2))  //chap
	    	||((0xc0 == ucLcp1) && (0x23 == ucLcp2))  //pap
	    	||((0x80 == ucLcp1) && (0x57 == ucLcp2))
	    	||((0x80 == ucLcp1) && (0x21 == ucLcp2)))
        {
            iControlPkt = 1;
        }
        
        if ((0x06 == skb->data[42]) || (0xa5 == skb->data[43]))
        {
            if (skb->data[50] & 0x8)
            {
        		iControlPkt = 1;
            }
        }        

        ucLcp1 = skb->data[20];
        ucLcp2 = skb->data[21];

	    if ((
#ifndef CONFIG_DT_QOS            
            ((0xc0 == ucLcp1) && (0x21 == ucLcp2)) ||
#endif            
	    	((0xc2 == ucLcp1) && (0x23 == ucLcp2))  //chap
	    	||((0xc0 == ucLcp1) && (0x23 == ucLcp2))  //pap
	    	||((0x80 == ucLcp1) && (0x57 == ucLcp2))
	    	||((0x80 == ucLcp1) && (0x21 == ucLcp2)))
#ifndef CONFIG_DT_QOS
            || (0x2 == skb->data[31])
#endif
            )
        {	
        	
            iControlPkt = 1;
        }
#ifdef CONFIG_DT_QOS            
        if ((0xc0 == ucLcp1) && (0x21 == ucLcp2))
        {
            skb->mark = (skb->mark & 0xFFFFFF00) | QOSDT_MARK_VOIP_BEARER;
        }
#endif

	}
    else if (__constant_htons(ETH_P_ARP) == skb->protocol)
    {
        iControlPkt = 1;
    }

    /* dhcp udp prot 67 or 68 */
    if ((0x43 == skb->data[35]) || (0x44 == skb->data[35]))
    {
        iControlPkt = 1;
    }  
    
    /* increase priority of igmp, pppoe, ppp lcp, dhcp */
    if (iControlPkt)
    {
#ifdef CONFIG_BLOG
		blog_skip(skb);
#endif
        skb->mark |= QOS_BRCM_HIGHEST_PRI; /* The top priority is 0x7 */
    }
    
    skb->mark |= QOS_DEFAULT_MARK;
}

void DtQosMarkToTos(struct sk_buff *skb)
{
	struct iphdr *iph;	/* Our header */
    iph = ip_hdr(skb);
	if (!MULTICAST(iph->daddr)
	&& (iph->daddr & 0xffffff00) != ((__be32)(sysctl_lan_host_ip) & 0xffffff00))
	{
		iph->tos = s_ucDtQosMarkToTOS[(skb->mark & 0x7)];
	}
    return;
}

void DtQosMarkToTosWithIPCheck(struct sk_buff *skb)
{
	u_int16_t diffs[2];
	struct iphdr *iph;	/* Our header */
    iph = ip_hdr(skb);
	if ((0x0 != (skb->mark & 0x7)) && (0x0 == iph->tos)) 
    {
		/* Without TOS value, mark it from skb->mark */
		diffs[0] = htons(iph->tos) ^ 0xffff;
		iph->tos = s_ucDtQosMarkToTOS[(skb->mark & 0x7)];
		diffs[1] = htons(iph->tos);
		iph->check = csum_fold(csum_partial((char *)diffs, sizeof(diffs), iph->check^0xffff));		
	}
    return;
}

unsigned int DtQosFonAtmImqEnqueue(unsigned uMark, struct sk_buff *pNBuffSkb, pNBuff_t pNBuff, struct net_device *dev, spinlock_t *pxtmlock_tx)
{
    int ret = 0;
    int fkb_flag = 0;
    FkBuff_t * fkb_p;
    struct sk_buff * skb; 
	const struct net_device_ops *ops;

    if (qos_enable && ((uMark & CONNTRACK_AVOID_SERVICE) != CONNTRACK_AVOID_SERVICE) && ((0x0 == (uMark & 0x07)) || (0x1 == (uMark & 0x07))))
    {	
    	/*For pkt that is not accelerated, pNBuffSkb isn't NULL, so we can transmit it directly.
         For accelerated pkts, we should convert it from a FKB to a SKB and then xmit it again.
         A packet originating from the network stack wouldn't be accelerated.*/
        if (pNBuffSkb)
        {
            skb = pNBuffSkb;
        }
        else
        {
            fkb_p = (FkBuff_t *)PNBUFF_2_PBUF(pNBuff); 
            pNBuffSkb = nbuff_xlate(pNBuff);
            if (!pNBuffSkb)
            {
                return 1;
            }
            skb = pNBuffSkb;
            if ( unlikely(fkb_p != FKB_NULL))
            {
                fkb_free(fkb_p);
            }
            fkb_flag = 1;
         }
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)        
         blog_skip(skb);
#endif
         if (skb->mark & QOS_DEFAULT_MARK)
         {
            skb->imq_flags &= ~IMQ_F_ENQUEUE;
            skb->mark &= ~QOS_DEFAULT_MARK;
            skb->imq_flags |= QOS_DEV_IMQ0;
            skb->skb_iif = dev->ifindex;
            spin_unlock_bh(pxtmlock_tx);
            ret = imq_nf_queue(skb);
            spin_lock_bh(pxtmlock_tx);
            if (0 == ret)
            {
                return 1;
            }
            else
            {
                skb->imq_flags |= QOS_DEV_IMQ1;
                spin_unlock_bh(pxtmlock_tx);
                ret = imq_nf_queue(skb);
                spin_lock_bh(pxtmlock_tx);
                if (0 == ret)
                {
                    return 1;
                }                   
            }
            
            if (fkb_flag)
            {
                ops = dev->netdev_ops;
                if (!ops)
                {
                    return 1;
                }

                ops->ndo_start_xmit(pNBuffSkb, dev);
                return 1;
            }
        }
        skb->mark |= QOS_DEFAULT_MARK;
    }
    
    return 0;
}

inline unsigned int DtQosCheckRemark(unsigned int uPrio, unsigned int uLen)
{
    /* The priorities that not define a min bandwidth queue. */
    unsigned int uQue = s_dtQosMarkToQue[uPrio];
    unsigned int uDelta;
    unsigned int uSendBitHZStat;
    unsigned int uQueRemark;
    unsigned int uBitHZLen;
    unsigned int uBitHZStat;

    if ((true != (qos_enable & 0xF)) 
        || (uQue >= (sizeof(s_dtQosStat) / sizeof(s_dtQosStat[0]))) )
    {
        return uQue;
    }

    if (0 == uQue)
    {
        return uQue;
    }
    
    /* Dequeue */
    uDelta = jiffies - s_dtQosStat[uQue].uStamp;
    uSendBitHZStat = (uDelta * s_dtQosStat[uQue].uMinBW);
    if (s_dtQosStat[uQue].uQueBitHZPend > uSendBitHZStat) 
    {
        s_dtQosStat[uQue].uQueBitHZPend -= uSendBitHZStat;
    }
    else 
    {
        s_dtQosStat[uQue].uQueBitHZPend = 0;
    }
    s_dtQosStat[uQue].uStamp = jiffies;

    /* Enqueue */
    uQueRemark = uQue;
    uBitHZLen= (uLen * 8 * HZ);
    uBitHZStat= s_dtQosStat[uQue].uQueBitHZPend + uBitHZLen;
    if (uBitHZStat <= s_dtQosStat[uQue].uQueBitHZSize) 
    {
        s_dtQosStat[uQue].uQueBitHZPend += uBitHZLen;
        uQueRemark = 4; /* remark to top priority. */
        /* DEBUG
        s_uRemarkByte += uLen;
        */
    }

    return uQueRemark;
}

