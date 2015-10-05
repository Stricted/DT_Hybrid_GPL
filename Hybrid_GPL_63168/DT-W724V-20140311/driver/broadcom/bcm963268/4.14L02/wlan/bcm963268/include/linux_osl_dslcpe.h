/*
 * Linux OS Independent Layer
 *
 * Copyright (C) 2008, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: linux_osl_dslcpe.h,v Exp $
 */

#ifndef _linux_osl_dslcpe_h_
#define _linux_osl_dslcpe_h_


#undef PKTPRIO
#undef PKTSETPRIO
#define	PKTPRIO(skb)			osl_pktprio((skb))
#define	PKTSETPRIO(skb, x)		osl_pktsetprio((skb), (x))
extern uint osl_pktprio(void *skb);
extern void osl_pktsetprio(void *skb, uint x);
extern int  osl_pktq_len(osl_t *osh);
extern void osl_pktpreallocinc(void *skb, int cnt);
extern void osl_pktpreallocdec(void *skb);

extern void wl_pktpreallocinc(struct sk_buff *skb, int cnt); 
extern void wl_pktpreallocdec(struct sk_buff *skb);
extern bool wl_pkt_drop_on_wmark(void *wl_ptr, struct sk_buff *skb, bool is_pktc);

#define PRIO_LOC_NFMARK 16
#define PKT_PREALLOCINC(osh, skb, c) osl_pktpreallocinc((skb), c)	

extern struct pci_dev* osh_get_pdev(osl_t *osh);

#if defined(PKTC) || defined(DSLCPE_PKTC)

/* Copy an ethernet address in reverse order */
#define	ether_rcopy(s, d) \
do { \
	((uint16 *)(d))[2] = ((uint16 *)(s))[2]; \
	((uint16 *)(d))[1] = ((uint16 *)(s))[1]; \
	((uint16 *)(d))[0] = ((uint16 *)(s))[0]; \
} while (0)

struct _macaddr {
       uint8 octet[6];
} __attribute__((packed));

/* PKT_PRIO_LVL_CNT should never exceed 16. 
 * If it does make sure the prio_bitmap in the ctf_brc_hot structure
 * and priority encoding logic between FAP and Ethernet driver for TX WLAN flows are modified
 */
#define PKT_PRIO_LVL_CNT	16 	/* PKT_PRIO_BASE_CNT * PKT_PRIO_LVL */

#define MAXBRCHOT	16
#define MAXBRCHOTIF     2
#define TOTAL_CHAIN_ENTRY_NUM   (MAXBRCHOT * MAXBRCHOTIF)

typedef struct {
    struct sk_buff *chead;
    struct sk_buff *ctail;
} chain_pair_t;
typedef struct {
    chain_pair_t chain[TOTAL_CHAIN_ENTRY_NUM];
    uint32       chainidx_bitmap;
} pktc_entry_t;

typedef struct {
    pktc_entry_t  pktc_table[PKT_PRIO_LVL_CNT]; /* chaining table for tx */
    uint16        prio_bitmap;
    struct wlc_pub  *pub;		/* pointer to public wlc state */
    osl_t         *osh;		/* pointer to os handler */
    void          *wlif;
} wl_pktc_info_t;

struct ctf_brc_hot {
    struct _macaddr    ea;     /* Dest mac addr */
    uint16             hits;    /* hit count */
    struct net_device  *tx_dev;    /* Dev to be sent */
    chain_pair_t       chain[PKT_PRIO_LVL_CNT];
    uint16             idx; /* table idx */
    uint16             prio_bitmap;
    uint32             wl_handle;
    uint8              in_use; /* Is this entry in-use */
} __attribute__((packed));

typedef struct ctf_brc_hot ctf_brc_hot_t;

/* compare two ethernet addresses - assumes the pointers can be referenced as shorts */
#define _eacmp(a, b)     ((((uint16 *)(a))[0] ^ ((uint16 *)(b))[0]) | \
                         (((uint16 *)(a))[1] ^ ((uint16 *)(b))[1]) | \
                         (((uint16 *)(a))[2] ^ ((uint16 *)(b))[2]))

#define WLAN_DEVICE_MAX 32  /* supported max wlan devices (including virtual devices) */

typedef struct pktc_devhandle {
    uint32   handle;  /* wlan handle */ 
    uint32   dev;     /* dev associated with the wlan handle */
} pktc_devhandle_t;

/* Hash table is divided into two part to avoid collision - First half (Primary) and if collision then second half (Secondary).
 * We can't afford to have a linear search during collision as this will affect performance. By dividing in two halfs, we reduce
 * the chance by 50%. 
 * Hash functions for Primary and Secondary are different - this helps in avoiding further hash collision in secondary hash */ 
#define CTF_BRC_HOT_HASH(da)    ((((uint8 *)da)[1] ^ ((uint8 *)da)[3] ^ ((uint8 *)da)[5]) % ((TOTAL_CHAIN_ENTRY_NUM)/2))
#define CTF_BRC_HOT_HASH_2(da)  (((((uint8 *)da)[0] ^ ((uint8 *)da)[2] ^ ((uint8 *)da)[4] ) % ((TOTAL_CHAIN_ENTRY_NUM)/2)) + ((TOTAL_CHAIN_ENTRY_NUM)/2))

#define CTF_HOTBRC_CMP(brc, da, rxifp) ctf_hotbrc_cmp_fn(brc, da, rxifp)
#define CTF_BRC_HOT_LOOKUP(brc, da)  ctf_brc_hot_lookup_fn(brc, da)
#define CTF_BRC_HOT_UPDATE(brc, da, dev, handle) ctf_brc_hot_update_fn(brc, da, dev, handle)
#define CTF_BRC_HOT_CLEAR(brc, da) ctf_brc_hot_clear_fn(brc, da)

static inline int ctf_hotbrc_cmp_fn(ctf_brc_hot_t *brc, uint8_t *da, struct net_device *rxifp)
{
    ctf_brc_hot_t *bh = brc + CTF_BRC_HOT_HASH(da); 
    ctf_brc_hot_t *bh2 = brc + CTF_BRC_HOT_HASH_2(da); 

    return (((_eacmp((bh)->ea.octet, (da)) == 0) && ((bh)->tx_dev != rxifp)) || ((_eacmp((bh2)->ea.octet, (da)) == 0) && ((bh2)->tx_dev != rxifp)));
}
void ctf_brc_hot_clear_fn(ctf_brc_hot_t *brc, uint8 *da);
uint32_t ctf_brc_hot_update_fn(ctf_brc_hot_t *brc, uint8 *da, struct net_device *dev, uint32 handle);
uint32 ctf_brc_hot_lookup_fn(ctf_brc_hot_t *brc, uint8_t *da);

#endif /* DSLCPE_PKTC */


#endif	/* _linux_osl_dslcpe_h_ */
