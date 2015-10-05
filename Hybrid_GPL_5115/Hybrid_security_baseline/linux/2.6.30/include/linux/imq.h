#ifndef _IMQ_H
#define _IMQ_H

#define IMQ_MAX_DEVS     16
/*Start of modified by f00120964 for qos function 2012-1-6 */
#define QOS_DEFAULT_MARK  0x100000
/*End of modified by f00120964 for qos function 2012-1-6 */
#define QOS_DEV_IMQ0      0x0
#define QOS_DEV_IMQ1      0x1
#define QOS_DEV_IMQ2      0x2
#define QOS_DEV_IMQ3      0x3
/*Start of modified by f00120964 for qos function 2012-1-6 */
/*Start add by l00184769: keep the kernel and cms of qos consistent20110629*/
#define QOS_HIGHEST_PRI                  0x100
/*end add by l00184769: keep the kernel and cms of qos consistent20110629*/
/*End of modified by f00120964 for qos function 2012-1-6 */
#define IS_ETN_WAN(name)  (('n' == name[0]) && ('0' == name[3]))

/* IFMASK (16 device indexes, 0 to 15) and flag(s) fit in 5 bits */
#define IMQ_F_BITS	5

#define IMQ_F_IFMASK	0x0f
#define IMQ_F_ENQUEUE	0x10
extern int qos_enable;
extern int imq_nf_queue(struct sk_buff *skb);
//#define IMQ_MAX_DEVS	(IMQ_F_IFMASK + 1)

#endif /* _IMQ_H */

