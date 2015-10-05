#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/tcp.h>
#include <net/udp.h>

#include <linux/string.h>
#include <linux/ctype.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ipt_misc.h>
#include <linux/netfilter_ipv4/ip_nat_dnscomm.h>

static struct list_head g_hash[TCPPTSCAN_LISTHASH_SIZE];
static unsigned int g_expire_ptscan = 0;

static struct rtp_diff_block g_rtpHead;

/* start of w00104696 ADD: for QoS RTP class, the rtcp pkt also matched 20100109*/
#define RTP_HDR                 0x80
#define RTP_HDR_VER             0x80
#define RTP_PT_PCMU   0
#define RTP_PT_PCMA   8
#define RTP_PT_G722   9
#define RTP_PT_G729   18

#define RTCP_PT_SR    200
#define RTCP_PT_RR    201
#define RTCP_PT_SDES  202
#define RTCP_PT_BYE   203
#define RTCP_PT_APP   204
/* end of w00104696 ADD: for QoS RTP class, the rtcp pkt also matched 20100109*/

static __inline__ int iphash_key(const unsigned char *srcip, const unsigned char *dstip)
{
    unsigned long x;
    unsigned long y;

	x = srcip[0];
	x = (x << 2) ^ srcip[1];
	x = (x << 2) ^ srcip[2];
	x = (x << 2) ^ srcip[3];
	x ^= x >> 8;

	y = dstip[0];
	y = (y << 2) ^ dstip[1];
	y = (y << 2) ^ dstip[2];
	y = (y << 2) ^ dstip[3];
	y ^= y >> 8;

	return y & x & (TCPPTSCAN_LISTHASH_SIZE - 1);
}
static void ptscan_node_free(struct iptmisc_tcpptscan_node *pnode)
{
    if(!pnode)
        return ;
    spin_lock_bh(&pnode->lock);
    del_timer(&pnode->timer_gc);
    del_timer(&pnode->timer_refresh);
    list_del_rcu(&pnode->list);
    spin_unlock_bh(&pnode->lock);
    kfree(pnode);
}

#pragma INIT
void inline init_ptscan(void)
{
    static int inited = 0;
    int i;
    
    if(inited)
        return;
    
    for(i = 0 ; i < TCPPTSCAN_LISTHASH_SIZE; i++)
    {
        INIT_LIST_HEAD(&g_hash[i]);
    }

    inited = 1;
}
void inline init_rtp(void)
{
    static int inited = 0;
    int i;

    if(inited)
        return;
    
    for(i = 0; i < RTP_DIFF_HASH_SIZE; i++)
        INIT_LIST_HEAD(&g_rtpHead.head[i]);
    get_random_bytes(&g_rtpHead.rnd, 4);

    inited = 1;
}

#pragma TIMERTIMEOUT

static void rtp_diff_head_free(struct rtp_diff_head *phead);
static void rtp_gc_timeout(unsigned long ptr)
{
    struct rtp_diff_head *phead = (struct rtp_diff_head *)ptr;

    if(phead->refresh + RTP_DIFF_GC_TIMEOUT > jiffies)
    {
        mod_timer(&phead->timer_gc, jiffies+RTP_DIFF_GC_TIMEOUT);
    }
    else
    {
        rtp_diff_head_free(phead);
    }
}

static void refresh_timeout(unsigned long ptr)
{
    struct iptmisc_tcpptscan_node *pnode = (struct iptmisc_tcpptscan_node*)ptr;

    if((!pnode->silent) || (pnode->silent_start + TCPPTSCAN_SLIENT_MAX < jiffies))    
    {
        memset(pnode->dst_ports, 0, sizeof(pnode->dst_ports));
        pnode->recorded_ports = 0;
        pnode->silent = 0;
    }

    mod_timer(&pnode->timer_refresh, jiffies+g_expire_ptscan*TCPPTSCAN_REFRESH_INTERVAL);
}

static void gc_timeout(unsigned long ptr)
{
     struct iptmisc_tcpptscan_node *pnode = (struct iptmisc_tcpptscan_node*)ptr;
     int just_visited = 0;
     int i = 0 ;

     if(pnode->silent)
     {
         mod_timer(&pnode->timer_gc, jiffies+TCPPTSCAN_GC_INTERVAL);    
     }
     else
    {
        for(i = 0; i < TCPPTSCAN_PTHASH_SIZE2; i++)
        {
            if(pnode->dst_ports[i])
            {
                just_visited = 1;
            }
        }

        if(just_visited)
        {
             mod_timer(&pnode->timer_gc, jiffies+TCPPTSCAN_GC_INTERVAL);              
        }
        else
        {
            del_timer(&pnode->timer_gc);
            del_timer(&pnode->timer_refresh);
            list_del_rcu(&pnode->list);
            kfree(pnode);
        }
    }
     
}
#pragma RTPRTCP
static void rtp_diff_head_free(struct rtp_diff_head *phead)
{
    if(!phead)
        return;
    
    spin_lock_bh(&phead->lock);
    list_del_rcu(&phead->list);
    del_timer(&phead->timer_gc);

    if(phead->head)
    {
        if(phead->head->prev)
            phead->head->prev->next = NULL;

        phead->curr = phead->head;
        while(phead->head)
        {
            phead->curr = phead->head->next;
            kfree(phead->head);
            phead->head = phead->curr;
        }
    }
    spin_unlock_bh(&phead->lock);

    kfree(phead);
}
static inline struct rtp_diff_head* rtp_diff_create_head(struct rtp_hash_dst *pdst)
{
    struct rtp_diff_head *phead = NULL;
    
    /*rtp stream record not exist, create it*/
    if(NULL == (phead = (struct rtp_diff_head*)kmalloc(sizeof(struct rtp_diff_head), GFP_KERNEL)))
        return NULL;

    memset(phead, 0, sizeof(struct rtp_diff_head));
    /*add to link*/
    list_add_rcu(&phead->list, &g_rtpHead.head[jhash(pdst, sizeof(struct rtp_hash_dst), g_rtpHead.rnd)&(RTP_DIFF_HASH_SIZE-1)]); 
    /*init gc timer*/
    init_timer(&phead->timer_gc);
    phead->timer_gc.expires = RTP_DIFF_GC_TIMEOUT;
    phead->timer_gc.data = (unsigned long)phead;
    phead->timer_gc.function = rtp_gc_timeout;
    add_timer(&phead->timer_gc);
    /*init lock*/
    spin_lock_init(&phead->lock);
    /*init rtp packet tracing infomation*/
    phead->count = 0;; //number of nodes        
    phead->dst.addr.ip.src = pdst->addr.ip.src;    
    phead->dst.addr.ip.dst = pdst->addr.ip.dst;    
    phead->dst.src_port = pdst->src_port;    
    phead->dst.dst_port = pdst->dst_port;    
    /*init rtp packet tracing link*/
    phead->head = NULL;
    phead->curr = NULL;

    return phead;
}

static inline struct rtp_diff_head* rtp_diff_find_head(struct rtp_hash_dst *pdst)
{
    struct rtp_diff_head* phead = NULL;
    
    /*fidn rtp stream record*/
    list_for_each_entry_rcu(phead, &g_rtpHead.head[jhash(pdst, sizeof(struct rtp_hash_dst), g_rtpHead.rnd)&(RTP_DIFF_HASH_SIZE-1)], list)
    {
        if(pdst->addr.ip.src == phead->dst.addr.ip.src
        && pdst->addr.ip.dst  == phead->dst.addr.ip.dst
        && pdst->src_port == phead->dst.src_port
        && pdst->dst_port  == phead->dst.dst_port)
           return phead;
    }
    return NULL;
}
static inline void rtp_diff_calc(struct rtp_diff_head *phead, struct rtp_diff_node *prev, struct rtp_diff_node *curr, struct rtp_diff_node *next)
{
    if(!curr)
        return ;

    if(next)
    {
        //simple
        curr->value.ver_diff = next->pos.ver - curr->pos.ver;
        curr->value.pt_diff =  next->pos.pt - curr->pos.pt;
        curr->value.seq_diff =  next->pos.seq - curr->pos.seq;
        curr->value.tstamp_diff =  next->pos.tstamp- curr->pos.tstamp;
        curr->value.ssrc_diff =  next->pos.ssrc - curr->pos.ssrc;

        phead->value.ver_diff += curr->value.ver_diff;
        phead->value.pt_diff += curr->value.pt_diff;
        phead->value.seq_diff += curr->value.seq_diff;
        phead->value.tstamp_diff += curr->value.tstamp_diff;
        phead->value.ssrc_diff += curr->value.ssrc_diff;
    }
        
    if(prev)
    {
        //quadratic
        prev->value.seq_diff2 = curr->value.seq_diff - prev->value.seq_diff;
        //prev->value.tstamp_diff2 = curr->value.tstamp_diff - prev->value.tstamp_diff;
        if(curr->value.tstamp_diff > 0 &&  prev->value.tstamp_diff > 0)
            prev->value.tstamp_diff2 = 0;
        else
            prev->value.tstamp_diff2 = 1;  
        
        phead->value.seq_diff2 += prev->value.seq_diff2;
        phead->value.tstamp_diff2 += prev->value.tstamp_diff2;
    }
}
#pragma IPLAND
/* return 1 if the attack is matched, 0 otherwise */
static inline int match_ipland(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
	const struct iphdr *iph = ip_hdr(skb);
	
    if (iph->saddr == iph->daddr) 
			return 1;

    return 0;
}
    
/* return 1 if the attack is matched, 0 otherwise */
static inline int match_tcp_port_scan(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
	const struct iphdr *iph = ip_hdr(skb);
	const struct tcphdr *tcph = (void *)iph + iph->ihl*4;
    struct iptmisc_tcpptscan_node *pnode = NULL;
    unsigned int saddr, daddr;
    int node_existed = 0;
    int seg, pos, mode;
    int ret = 0;

    /* The first pkt of one tcp connection is a syn pkt */
    if(!tcph->syn)
        return 0;

/*SHUXUEGANG FOR PORT SCAN  VDF C02*/
    init_ptscan();
    
    daddr = iph->daddr;
    saddr = iph->saddr;
    list_for_each_entry_rcu(pnode, &g_hash[iphash_key((unsigned char*)&saddr, (unsigned char*)&daddr)], list)
    {
        spin_lock_bh(&pnode->lock);
        if(pnode->srcip == iph->saddr && pnode->dstip == iph->daddr)
        {
            node_existed = 1;
            break;
        }
        spin_unlock_bh(&pnode->lock);
    }

    if(!node_existed)
    {
        if(NULL == (pnode = (struct iptmisc_tcpptscan_node*)kmalloc(sizeof(struct iptmisc_tcpptscan_node), GFP_KERNEL)))
            return 0;
        else
        {
            memset(pnode, 0, sizeof(struct iptmisc_tcpptscan_node));
            daddr = pnode->dstip = iph->daddr;
            saddr = pnode->srcip = iph->saddr;
            init_timer(&pnode->timer_refresh);
            init_timer(&pnode->timer_gc);
            pnode->timer_refresh.expires = jiffies + info->expire_ptscan*TCPPTSCAN_REFRESH_INTERVAL;
            pnode->timer_refresh.data = (unsigned long)pnode;
            pnode->timer_refresh.function = refresh_timeout;
            pnode->timer_gc.expires = jiffies + TCPPTSCAN_GC_INTERVAL;
            pnode->timer_gc.data = (unsigned long)pnode;
            pnode->timer_gc.function = gc_timeout;
            add_timer(&pnode->timer_refresh);
            add_timer(&pnode->timer_gc);
            spin_lock_init(&pnode->lock);
            list_add_rcu(&pnode->list, &g_hash[iphash_key((unsigned char*)&saddr, (unsigned char*)&daddr)]); 
            spin_lock_bh(&pnode->lock);
        }
    }

    if(pnode->silent)
    {    
        ret = 1; 
        goto local_exit;
    }
    
    /* the space of index is [0,1023], each bit of the dst_port member is a index of the dest port */
	mode = tcph->dest%TCPPTSCAN_PTHASH_SIZE1;
	for(seg = 0, pos = mode; pos > TCPPTSCAN_PTHASH_WDWIDTH; pos-=TCPPTSCAN_PTHASH_WDWIDTH, seg++)
    {;}

	if(!(pnode->dst_ports[seg] & (0x0001 << pos)))
    {
    	pnode->dst_ports[seg] |= (0x0001 << pos);
    	pnode->recorded_ports++;

    	/*if a special numbers of port are quested in a special interval ,there might be a tcp port scanning*/
        if(pnode->recorded_ports > info->ports_ptscan)
        {
            pnode->silent = 1;
            pnode->silent_start = jiffies;
            ret = 1;        
        }
    }

local_exit:
    spin_unlock_bh(&pnode->lock);
    return ret;
}

static inline int match_dns_url(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
    struct dnshdr dns_hdr;
    struct udphdr udp_hdr;
    struct iphdr *iph;
    char *urls = NULL;
    char *url = NULL;
    char *url_tmp = NULL;
    int  len_dns = 0;
    unsigned char len_tmp = 0;
    unsigned int ret = 0;
    int i;
    int iphdrlen = 0;

    iph = ip_hdr(skb);

    iphdrlen = iph->ihl * 4;
     /* get dns head */
    if(0 != skb_copy_bits(skb, iphdrlen + sizeof(struct udphdr), &dns_hdr, sizeof(struct dnshdr)))
        return ret;

    /* isn't a dns query packet */
    if((unsigned int)DNSFLAG_QR(dns_hdr.flag) != DNS_PKT_TYPE_QUERY)
        return ret;

    /* get udp head */
    if(0 != skb_copy_bits(skb, iphdrlen, &udp_hdr, sizeof(struct udphdr)))
        return ret;

    len_dns = __constant_ntohs(udp_hdr.len) - sizeof(struct udphdr) - sizeof(struct dnshdr);
    /*invalid dns payload*/
    if(len_dns < 0 )
        return ret;
    
    if(NULL == (urls = kmalloc(len_dns, GFP_KERNEL)))
        return ret;
    else
        memset(urls, 0, len_dns);
    
    if(0 != skb_copy_bits(skb, iphdrlen + sizeof(struct udphdr) + sizeof(struct dnshdr), urls, len_dns))
    {
        kfree(urls);
        return ret;
    }

    for(i = 0, url = urls; i < __constant_ntohs(dns_hdr.qd_conut); i++, url = url_tmp+1)
    {
       /*
       transform from '3www6google3com0' to '.www.google.com0'
       */
        url_tmp = url;
        while(*url_tmp && (url_tmp < (urls+len_dns)))
        {
            len_tmp = *url_tmp;
            *url_tmp = '.';
            url_tmp += len_tmp + 1;
        }

        //DEBUGP("DNS url: %s\n", url);
        //if(URLFLTR_DENY == url_match(url + 1))
        if(strstr(url + 1, info->dns_url))
        {
            ret = 1;
            break;
        }            
    }

    kfree(urls);
    return ret;
}

static inline int match_rtcp(const struct rtp_hash_dst *pdst)
{
    struct rtp_diff_head *phead = NULL;
    struct rtp_hash_dst dst;
    int ret = 0;

    memcpy(&dst, pdst, sizeof(struct rtp_hash_dst));

    /* port: rtp = rtpc - 1 */
    dst.dst_port -= 1;
    dst.src_port -= 1;
    
    if(NULL == (phead = rtp_diff_find_head(&dst)))
        return 0;

    spin_lock_bh(&phead->lock);
    
    phead->refresh = jiffies;
    
    if(phead->value.ver_diff
    || phead->value.pt_diff
    || phead->value.ssrc_diff
    || phead->value.seq_diff2
    || phead->value.tstamp_diff2)
        ret = 0;
    else
        ret = 1;
    
    spin_unlock_bh(&phead->lock);
    return ret;
}
/*
hash[0]     hash[1]    hash[2]
 |
 V
head----->head-------->head
           |            |
           |            V
           |          node----->node------->node
           V
           node----->node---->node
*/
static inline int match_rtp(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
    const struct iphdr *iph = ip_hdr(skb);
    struct rtp_hash_dst dst;
    struct udphdr udp_hdr;
    struct rtphdr rtp_hdr;
    struct rtp_diff_head *phead = NULL;
    int matched = 0;
    int ulRtpPT = -1;
    int iphdrlen = iph->ihl * 4;

    /* get udp head */
    if(0 != skb_copy_bits(skb, iphdrlen, &udp_hdr, sizeof(struct udphdr)))
        return 0;

    init_rtp();
    
    dst.addr.ip.src = iph->saddr;
    dst.addr.ip.dst = iph->daddr;
    dst.src_port = udp_hdr.source;
    dst.dst_port = udp_hdr.dest;

    if(info->flag_rtcp && match_rtcp(&dst))
        return 1;

    if(NULL == (phead = rtp_diff_find_head(&dst))
     && NULL == (phead = rtp_diff_create_head(&dst)))
        return 0;  

    spin_lock_bh(&phead->lock);

    phead->refresh = jiffies;

     /* get rtp head */
    if(0 != skb_copy_bits(skb, iphdrlen + sizeof(struct udphdr) , &rtp_hdr, sizeof(struct rtphdr)))
    {
        matched = 0;
        goto local_exit;
    }
    
    ulRtpPT = (*(((char*)&rtp_hdr) + 1)) & 0x7f;
#if 1
    if ((RTP_HDR == *((char*)&rtp_hdr)) 
#if 1        
        && ((RTP_PT_PCMU == ulRtpPT) 
            || (RTP_PT_PCMA == ulRtpPT) 
            || (RTP_PT_G722 == ulRtpPT)
            || (RTP_PT_G729 == ulRtpPT))
#endif            
            )
    {
        //printk("rtp_hdr.pt %d %d\n", rtp_hdr.pt, (*(((char*)&rtp_hdr) + 1)) & 0x7f);
        matched = 1;
    }
    else
    {
        matched = 0;
    }
#else    
    /*create quadratic difference link*/
    if(phead->count < info->rtp_diff_factor)
    {
        if(NULL == (pnode = (struct rtp_diff_node*)kmalloc(sizeof(struct rtp_diff_node), GFP_KERNEL)))
        {
            matched = 0;
            goto local_exit;
        } 
        else
            memset(pnode, 0, sizeof(struct rtp_diff_node));

        /*record the difference infomation*/
        pnode->pos.ver = rtp_hdr.ver;
        pnode->pos.pt = rtp_hdr.pt;
        pnode->pos.seq = rtp_hdr.seq;
        pnode->pos.tstamp = rtp_hdr.tstamp;
        pnode->pos.ssrc = rtp_hdr.ssrc;
        
        /*add rtp info node to the analyse link*/
        if(phead->curr)
        {
           /* caculate difference */
            rtp_diff_calc(phead, phead->curr->prev, phead->curr, pnode);
            /* add to link */
            phead->curr->next = pnode;
            pnode->prev = phead->curr;
            phead->curr = phead->curr->next;
        }
        else
            phead->curr = pnode;

        if(!phead->head)
            phead->head = phead->curr;

        phead->count++;
        /* if stream infomation is enough, then close the  analyselink */
        if(phead->count == info->rtp_diff_factor)
        {
            phead->curr->next = phead->head;
            phead->head->prev = phead->curr;
        }
    }
    else/*analyse rtp stream*/
    {
        /*record the difference infomation*/
        phead->value.ver_diff -= phead->head->value.ver_diff;
        phead->value.pt_diff -= phead->head->value.pt_diff;
        phead->value.seq_diff -= phead->head->value.seq_diff;
        phead->value.tstamp_diff -= phead->head->value.tstamp_diff;
        phead->value.ssrc_diff -= phead->head->value.ssrc_diff;
        phead->value.seq_diff2 -= phead->head->value.seq_diff2;
        phead->value.tstamp_diff2 -= phead->head->value.tstamp_diff2;
        
        phead->head->pos.ver = rtp_hdr.ver;
        phead->head->pos.pt = rtp_hdr.pt;
        phead->head->pos.seq = rtp_hdr.seq;
        phead->head->pos.tstamp = rtp_hdr.tstamp;
        phead->head->pos.ssrc = rtp_hdr.ssrc;
        memset(&phead->head->value, 0, sizeof(struct rtp_diff_val));
        rtp_diff_calc(phead, phead->head->prev->prev, phead->head->prev, phead->head);
        phead->head = phead->head->next;
        if(info->flag_rtp)
        {
            matched = (phead->value.ver_diff 
                    || phead->value.pt_diff 
                    || phead->value.ssrc_diff 
                    || phead->value.seq_diff2
                    || phead->value.tstamp_diff2) ? 0 : 1;  
            goto local_exit;
        }
    }
#endif
local_exit:
    spin_unlock_bh(&phead->lock);
    return matched;
}


static inline int match_rtp2(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
    const struct iphdr *iph = ip_hdr(skb);
    struct udphdr udp_hdr;
    struct rtphdr rtp_hdr;
    int matched = 0;
    int ulRtpPT = -1;
    int ulRtcpPT = -1;
    int iphdrlen = iph->ihl * 4;

    /* get udp head */
    if (0 != skb_copy_bits(skb, iphdrlen, &udp_hdr, sizeof(struct udphdr)))
    {
        //DEBUGPC("how this happend");
        return 0;
    }

    if (0 != skb_copy_bits(skb, iphdrlen + sizeof(struct udphdr) , &rtp_hdr, sizeof(struct rtphdr)))
    {
        //DEBUGPC("get rtp header error");
        matched = 0;
        goto local_exit;
    }

    /* start of w00104696 ADD: for QoS RTP class, the rtcp pkt also matched 20100109*/    
    ulRtpPT = (*(((char*)&rtp_hdr) + 1)) & 0x7f;
    ulRtcpPT = (*(((unsigned char*)&rtp_hdr) + 1));

    //printk("come here, header: %d, type: %d\n", (*(char *)&rtp_hdr), ulRtpPT);
    
    if (
            (RTP_HDR_VER == ((*((unsigned char*)&rtp_hdr)) & 0xc0)) 
            && ((RTP_PT_PCMU == ulRtpPT) 
                || (RTP_PT_PCMA == ulRtpPT) 
                || (RTP_PT_G722 == ulRtpPT)
                || (RTP_PT_G729 == ulRtpPT))
       )
    {        
        //printk("rtp_hdr.pt %d %d\n", rtp_hdr.pt, (*(((char*)&rtp_hdr) + 1)) & 0x7f);
        matched = 1;
    }
    else if (
                (RTP_HDR_VER == ((*((unsigned char*)&rtp_hdr)) & 0xc0))
                && ((RTCP_PT_SR == ulRtcpPT)
                    || (RTCP_PT_RR == ulRtcpPT)
                    || (RTCP_PT_SDES== ulRtcpPT)
                    || (RTCP_PT_BYE == ulRtcpPT)
                    || (RTCP_PT_APP == ulRtcpPT))
            )
                
    {
        //printk("line[%d]come here, header: %d, type: %d, udp_hdr:%d\n", __LINE__, (*(unsigned char *)&rtp_hdr), ulRtpPT, udp_hdr.source);
        matched = 1;
    }
    else
    {
        matched = 0;
    }
    /* end of w00104696 ADD: for QoS RTP class, the rtcp pkt also matched 20100109*/

local_exit:
    return matched;
}

static inline int match_payload_length(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
    const struct iphdr *iph = ip_hdr(skb);
    const struct udphdr *udph;
    const struct tcphdr *tcph;
    int iphdrlen = iph->ihl * 4;
    
    if(info->flag_tcplen && (iph->protocol == IPPROTO_TCP))
    {
        tcph = (void *)iph + iphdrlen;
        if(iphdrlen + tcph->doff*4 > iph->tot_len) 
            return 0;

        if((iph->tot_len - iphdrlen - tcph->doff*4 == info->payload) ^ (info->invert.flag_tcplen))
            return 1;
        else
            return 0;
    }

    if(info->flag_udplen && (iph->protocol == IPPROTO_UDP))
    {
        udph = (void *)iph + iphdrlen;      
        if(iph->ihl*4 + udph->len > iph->tot_len)
            return 0;

        if((iph->tot_len - iphdrlen - udph->len == info->payload) ^ (info->invert.flag_udplen))
            return 1;
        else
            return 0;
    }

    return 0;
}

static inline int match_udpbomb(const struct ipt_misc_info *info, const struct sk_buff* skb)
{
    const struct iphdr *iph = ip_hdr(skb);
    int iphdrlen = iph->ihl * 4;
    const struct udphdr *udph = (void *)iph + iphdrlen;  

    return !(iph->tot_len == iphdrlen + udph->len);
}

static bool misc_match_fn(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ipt_misc_info *info = par->matchinfo;
	const struct iphdr *iph = ip_hdr(skb);

    //ip land
	if(info->flag_ipland)
        return match_ipland(info, skb);

    //tcp port scan
	if((info->flag_tcpptscan) && (iph->protocol == IPPROTO_TCP))
        return match_tcp_port_scan(info, skb);

	//dns url match
	if(info->flag_dnsurl && (iph->protocol == IPPROTO_UDP))
        return match_dns_url(info, skb);

    if ((info->flag_rtp) && (iph->protocol == IPPROTO_UDP))
    {
        return match_rtp2(info, skb);
    }
    else if((info->flag_rtp||info->flag_rtcp) && (iph->protocol == IPPROTO_UDP))
    {
        return match_rtp(info, skb);
    }

	if(info->flag_tcplen || info->flag_udplen)
	    return match_payload_length(info,  skb);

	if(info->flag_udpbomb && (iph->protocol == IPPROTO_UDP))
	    return match_udpbomb(info, skb);
	
	return 0;
}


static bool checkentry (const struct xt_mtchk_param *par)
{
	const struct ipt_misc_info *info = par->matchinfo;

	if(info->flag_tcpptscan)
    {
        if(!(info->expire_ptscan * TCPPTSCAN_REFRESH_INTERVAL > 0))
        {
            printk("misc match: tcp port scan, value of expire is not normative. 'expire*HZ/1000' should be great than zero\n");
            return 0;
        }
        else
        {
            g_expire_ptscan = info->expire_ptscan;
        }
    }

    if(info->flag_rtp || info->flag_rtcp)
    {
        if(info->rtp_diff_factor < RTP_DIFF_FACTOR_MIN)
        {
            printk("misc match: RTP diffrence factor should not be less than %d\n", RTP_DIFF_FACTOR_MIN);
            return 0;
        }        
    }
	return 1;
}

void destroy(void *matchinfo, unsigned int matchinfosize)
{
	struct iptmisc_tcpptscan_node *pnode = NULL;
	struct rtp_diff_head *prtpHead = NULL;
	const struct ipt_misc_info *info = matchinfo;
    int i = 0;

    if(info->flag_tcpptscan)
    {
        for(i = 0; i < TCPPTSCAN_PTHASH_SIZE2; i++)
        {
            while(!list_empty(&g_hash[i]))
            {
                pnode = list_entry(g_hash[i].next, struct iptmisc_tcpptscan_node, list);
                ptscan_node_free(pnode);
                pnode = NULL;
            }
        }
    }

    if(info->flag_rtp || info->flag_rtcp)
    {
        for(i = 0; i < RTP_DIFF_HASH_SIZE; i++)
        {
            while(!list_empty(&g_rtpHead.head[i]))
            {
                prtpHead = list_entry(g_rtpHead.head[i].next, struct rtp_diff_head, list);
                rtp_diff_head_free(prtpHead);
                prtpHead = NULL;
            }
        }
    }
}
#if 0
static struct xt_match misc_match = {
	//.list = { NULL, NULL },
	.name = "misc",
	.match = match,
	.checkentry = checkentry,
	.destroy = NULL,
    .matchsize = sizeof(struct ipt_misc_info),
	.me = THIS_MODULE
};
#endif

static struct xt_match misc_match = {
	.name		= "misc",
	.family		= AF_INET,
	.match		= misc_match_fn,
	.matchsize	= sizeof(struct ipt_misc_info),
	.checkentry = checkentry,
	.me		= THIS_MODULE,
};					
static int __init init(void)
{
	return xt_register_match(&misc_match);
}

static void __exit fini(void)
{
	xt_unregister_match(&misc_match);
}

module_init(init);
module_exit(fini);

