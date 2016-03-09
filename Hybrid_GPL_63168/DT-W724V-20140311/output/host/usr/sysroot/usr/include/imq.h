#ifndef _IMQ_H
#define _IMQ_H
#define IMQ_MAX_DEVS      16
#define QOS_DEV_IMQ0      0x0
#define QOS_DEV_IMQ1      0x1
#define QOS_DEV_IMQ2      0x2
#define QOS_DEV_IMQ3      0x3

/* IFMASK (16 device indexes, 0 to 15) and flag(s) fit in 5 bits */
#define IMQ_F_BITS	5

#define IMQ_F_IFMASK	0x0f
#define IMQ_F_ENQUEUE	0x10
extern int imq_nf_queue(struct sk_buff *skb);

#endif /* _IMQ_H */

