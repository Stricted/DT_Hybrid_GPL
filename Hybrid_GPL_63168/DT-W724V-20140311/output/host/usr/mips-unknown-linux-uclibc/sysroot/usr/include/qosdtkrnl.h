#ifndef _QOSDTKRNL_H
#define _QOSDTKRNL_H
#include <linux/raid/pq.h>
#include "imq.h"

#define QOS_BRCM_HIGHEST_PRI             0x7
#define QOS_DEFAULT_MARK                 0x200
#define QOSDT_MARK_VOIP_BEARER           0x4
/*  
 * To use this code, it must to create 4 PQ queues on the PTM/ATM interface,
 * the priority are 0, 1, 2 and 3.
 */

/* QOS: min bandwidth speed. testing */
struct DT_QOS_STAT_S {
	unsigned int uMinBW;  /* bits/second of min bandwidth */
	unsigned int uStamp;  /* Jiffies */
	unsigned int uQueBitHZPend; /* (bits*HZ) pending in queue. */
	unsigned int uQueBitHZSize; /* (bits*HZ) of total queue size */
};

extern unsigned int s_dtQosMarkToQue[];
extern unsigned short g_usDtQosMarkto8021P[];
extern unsigned int g_ulDtQos8021PtoMark[];
extern void SkbMarkForHighestQueue(struct sk_buff *skb);
extern int qos_enable;
extern inline unsigned int DtQosCheckRemark(unsigned int uPrio, unsigned int uLen);
extern unsigned int DtQosFonAtmImqEnqueue(unsigned uMark, struct sk_buff *pNBuffSkb, pNBuff_t pNBuff, struct net_device *dev, spinlock_t *pxtmlock_tx);
extern void DtQosMarkToTosWithIPCheck(struct sk_buff *skb);
extern void DtQosMarkToTos(struct sk_buff *skb);
#endif

