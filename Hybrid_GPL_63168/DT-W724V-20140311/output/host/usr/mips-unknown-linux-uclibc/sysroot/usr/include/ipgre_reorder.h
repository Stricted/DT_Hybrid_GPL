/***************************************************************************
 * File Name    : if_smux.h
 * Description  : contains network driver implementation for Service MUX (SMUX) which enables to create multiple services on a physical interface like atm or ptm etc.,
 ***************************************************************************/
#ifndef _LINUX_GRE_REORDER_H_
#define _LINUX_GRE_REORDER_H_

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/seq_file.h>
#include <net/ip.h>
#include "atpconfig.h"

#ifdef CONFIG_ATP_HYBRID_REORDER
extern int sysctl_ipgre_queue_timeout;
extern int sysctl_ipgre_skb_timeout;
extern int sysctl_ipgre_qlen_max;
extern int sysctl_ipgre_badseq_interval;
extern int sysctl_ipgre_keepseq_enable;
extern int (*ipgre_reorder_hook)(struct sk_buff *);
extern void (*ipgre_reorder_seq_show_hook)(struct seq_file *seq);
extern struct gre_tunnel_o_seqno  g_stTunOSeq;
extern int netif_rx_cpu(struct sk_buff *skb, int cpu);

#define IPGRE_QUEUE_TIMEOUT            (sysctl_ipgre_queue_timeout * HZ / 1000)
#define IPGER_QLEN_DEF                 1000
#define IPGER_BADSEQ_INTERVAL_DEF      256
#define IPGER_QUEUE_TIMEOUT_DEF        2
#define IPGER_QUEUE_TIMEOUT_BUSY       5
#define IPGRE_QUEUE_BUSY_TRESHOLD      38400
#define IPGER_SKB_TIMEOUT_DEF          100

#define IPGER_DHCP_SERVER_PORT         67
#define IPGER_DHCP_CLIENT_PORT         68

#define IPGER_DHCPV6_SERVER_PORT       546
#define IPGER_DHCPV6_CLIENT_PORT       547

#define IPGRE_SEQ_REVERSE_NUM          100

/* Private data stored for received packets in the skb.
 */
struct ipgre_skb_cb {
    unsigned int ipgre_proto;
	unsigned int ipgre_seqno;
};

#define IPGRE_SKB_CB(skb)	((struct ipgre_skb_cb *) &skb->cb[sizeof(struct inet_skb_parm)])

struct gre_skb_queue
{
    struct sk_buff* gre_skb;
	unsigned long	trans_start;	/* Time (in jiffies) of last Tx	*/
};

struct gre_skb_buff {
	/* These two members must be first. */
	struct gre_skb_queue stgre_skb_queue[1024];
	__u32		qlen;
	unsigned int i_seqno;	/* The last seen seqno	*/
    unsigned int o_seqno;	/* The last output seqno */    
    unsigned int over_cnt;	/* The last output seqno */    
    unsigned int blow_cnt;	/* The last output seqno */    
    unsigned int over_num;	/* The last output seqno */    
    unsigned int blow_num;	/* The last output seqno */    
    unsigned int reverse_num;	/* The last output seqno */    
    unsigned int time_out_cnt;	/* The last output seqno */ 
    unsigned int o_seqno_err;	/* The last output seqno */    
    unsigned int same_seq_err_cnt;	/* The last output seqno */    

    spinlock_t	lock;
};

struct gre_tunnel_o_seqno {
    unsigned int o_seqno;	/* The last output seqno */    
	spinlock_t	lock;
};
#endif


#endif /* _LINUX_IF_SMUX_H_ */
