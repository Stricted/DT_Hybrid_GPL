
/*
 Copyright 2002-2010 Broadcom Corp. All Rights Reserved.

 <:label-BRCM:2011:DUAL/GPL:standard    
 
 Unless you and Broadcom execute a separate written software license
 agreement governing use of this software, this software is licensed
 to you under the terms of the GNU General Public License version 2
 (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
 with the following added to such license:
 
    As a special exception, the copyright holders of this software give
    you permission to link this software with independent modules, and
    to copy and distribute the resulting executable under terms of your
    choice, provided that you also meet, for each linked independent
    module, the terms and conditions of the license of that module.
    An independent module is a module which is not derived from this
    software.  The special exception does not apply to any modifications
    of the software.
 
 Not withstanding the above, under no circumstances may you combine
 this software in any way with any other Broadcom software provided
 under a license other than the GPL, without Broadcom's express prior
 written consent.
 
 :>
*/
#ifndef _BCMENET_RUNNER_INLINE_H_
#define _BCMENET_RUNNER_INLINE_H_

#include "rdpa_api.h"

#define WL_NUM_OF_SSID_PER_UNIT 8 

extern int wan_port_id; 
extern rdpa_system_init_cfg_t init_cfg;


static inline int bcmeapi_alloc_skb(BcmEnet_devctrl *pDevCtrl, struct sk_buff **skb)
{

#if 1
    if (pDevCtrl->freeSkbList) {
        *skb = pDevCtrl->freeSkbList;
        pDevCtrl->freeSkbList = pDevCtrl->freeSkbList->next_free;
    }
    else
#endif
    {
        *skb = kmem_cache_alloc(enetSkbCache, GFP_ATOMIC);

        if (!(*skb)) {
            return BCMEAPI_CTRL_FALSE;
        }
    }
	return BCMEAPI_CTRL_TRUE;
}



static inline int bcmeapi_free_skb(BcmEnet_devctrl *pDevCtrl, 
        struct sk_buff *skb, int free_flag)
{
    unsigned int is_bulk_rx_lock_active;
    uint32 cpuid;

    if( !(free_flag & SKB_RECYCLE ))
    {
        return BCMEAPI_CTRL_FALSE;
    }

    /*
     * Disable preemption so that my cpuid will not change in this func.
     * Not possible for the state of bulk_rx_lock_active to change
     * underneath this function on the same cpu.
     */
    preempt_disable();
    cpuid =  smp_processor_id();
    is_bulk_rx_lock_active = pDevCtrl->bulk_rx_lock_active[cpuid];

    if (0 == is_bulk_rx_lock_active)
        ENET_RX_LOCK();

    if ((unsigned char *)skb < pDevCtrl->skbs_p || (unsigned char *)skb >= pDevCtrl->end_skbs_p)
    {
        kmem_cache_free(enetSkbCache, skb);
    }
    else
    {
        skb->next_free = pDevCtrl->freeSkbList;
        pDevCtrl->freeSkbList = skb;      
    }

    if (0 == is_bulk_rx_lock_active)
        ENET_RX_UNLOCK();

    preempt_enable();
    return BCMEAPI_CTRL_TRUE;
}

extern void bdmf_sysb_databuf_recycle(void *pBuf, unsigned context);

/* Callback: fkb and data recycling */
static inline void __bcm63xx_enet_recycle_fkb(struct fkbuff * pFkb,
                                              uint32 context)
{
    /* No cache flush */
    bdmf_sysb_databuf_recycle(pFkb, context);
}

static inline void bcmeapi_kfree_buf_irq(BcmEnet_devctrl *pDevCtrl, struct fkbuff * pFkb, unsigned char *pBuf) 
{
    nbuff_flush(pFkb, pFkb->data, pFkb->len);/*TODO change this to invalidate */
    __bcm63xx_enet_recycle_fkb(pFkb, pFkb->recycle_context);
}
static inline void bcmeapi_blog_drop(BcmEnet_devctrl *pDevCtrl, struct fkbuff  *pFkb, unsigned char *pBuf)
{
    bcmeapi_kfree_buf_irq(pDevCtrl, pFkb, pBuf);
}

static inline void bcm63xx_enet_recycle_skb_or_data(struct sk_buff *skb,
                                             uint32 context, uint32 free_flag)
{
    BcmEnet_devctrl *pDevCtrl = (BcmEnet_devctrl *)netdev_priv(vnet_dev[0]);

    if (bcmeapi_free_skb(pDevCtrl, skb, free_flag) != BCMEAPI_CTRL_TRUE)
    { // free data
        uint8 *pData = skb->head + BCM_PKT_HEADROOM;
        uint8 *pEnd;
#if defined(ENET_CACHE_SMARTFLUSH)
        uint8 *dirty_p = skb_shinfo(skb)->dirty_p;
        uint8 *shinfoBegin = (uint8 *)skb_shinfo(skb);
        uint8 *shinfoEnd;
        if (skb_shinfo(skb)->nr_frags == 0) {
            // no frags was used on this skb, so can shorten amount of data
            // flushed on the skb_shared_info structure
            shinfoEnd = shinfoBegin + offsetof(struct skb_shared_info, frags);
        }
        else {
            shinfoEnd = shinfoBegin + sizeof(struct skb_shared_info);
        }
        //cache_flush_region(shinfoBegin, shinfoEnd);
        cache_invalidate_region(shinfoBegin, shinfoEnd);

        // If driver returned this buffer to us with a valid dirty_p,
        // then we can shorten the flush length.
        if (IS_SKBSHINFO_DIRTYP_ACK(dirty_p)) {
            CLR_SKBSHINFO_DIRTYP_ACK(dirty_p);
            if ((dirty_p < skb->head) || (dirty_p > shinfoBegin)) {
                printk("invalid dirty_p detected: %p valid=[%p %p]\n",
                        dirty_p, skb->head, shinfoBegin);
                pEnd = shinfoBegin;
            } else {
                pEnd = (dirty_p < pData) ? pData : dirty_p;
            }
        } else {
            pEnd = shinfoBegin;
        }
#else
        pEnd = pData + BCM_MAX_PKT_LEN;
#endif
        cache_invalidate_region(pData, pEnd);
        bdmf_sysb_databuf_recycle(PDATA_TO_PFKBUFF(pData, BCM_PKT_HEADROOM), context);
    }
}

/* Common recycle callback for fkb, skb or data */
inline void bcm63xx_enet_recycle(pNBuff_t pNBuff, uint32 context, uint32 flags)
{
    if ( IS_FKBUFF_PTR(pNBuff) ) {
        __bcm63xx_enet_recycle_fkb(PNBUFF_2_FKBUFF(pNBuff), context);
    } else { /* IS_SKBUFF_PTR(pNBuff) */
        bcm63xx_enet_recycle_skb_or_data(PNBUFF_2_SKBUFF(pNBuff),context,flags);
    }
}

#if 1
static inline void bcmeapi_set_fkb_recycle_hook(FkBuff_t * pFkb)
{

        pFkb->recycle_hook = (RecycleFuncP)bcm63xx_enet_recycle;
    //    pFkb->recycle_context = 0;
}
#endif

static inline int bcmeapi_skb_headerinit(int len, BcmEnet_devctrl *pDevCtrl, struct sk_buff *skb, 
						   FkBuff_t * pFkb, unsigned char *pBuf)
{

    skb_headerinit(BCM_PKT_HEADROOM,
#if defined(ENET_CACHE_SMARTFLUSH)
            SKB_DATA_ALIGN(len+BCM_SKB_TAILROOM),
#else
            BCM_MAX_PKT_LEN,
#endif
            skb, pBuf, (RecycleFuncP)bcm63xx_enet_recycle_skb_or_data,
            pFkb->recycle_context, pFkb->blog_p);

    skb_trim(skb, len);
    return BCMEAPI_CTRL_TRUE;
}

static inline int bcmeapi_rx_pkt(BcmEnet_devctrl *pDevCtrl, unsigned char **pBuf, FkBuff_t **pFkb, 
                   int *len, int *gemid, int *phy_port_id, int *is_wifi_port, struct net_device **dev, uint32 *rxpktgood,
                   int *msg_id_p, uint32 *context_p, uint32 *context1_p)
{
   int rc;
   uint32 cpuid = smp_processor_id();
    rdpa_cpu_rx_info_t info = {};

    /*TODO:remove wifi code from enet driver move it to WFD
     * check with yoni/Ilya  
     */ 
#if 1
    *is_wifi_port = 0;
#endif

    rc = rdpa_cpu_packet_get(rdpa_cpu_host, NETDEV_CPU_RX_QUEUE_ID,
        (bdmf_sysb *)pFkb, &info);
    if (rc)
    {
        RECORD_BULK_RX_UNLOCK();
        ENET_RX_UNLOCK();
        if (rc == BDMF_ERR_NO_MORE)
        {
            /* CPU Rx queue is empty. */
            *rxpktgood |= ENET_POLL_DONE;
            return BCMEAPI_CTRL_BREAK;
        }
        /* Some error */
        printk(KERN_NOTICE "Error in rdpa_cpu_packet_get() (%d)\n", rc);
        /* Consider error packets (assuming RDD succeeded dequeuing
         * them) as read packets. */
      (*rxpktgood)++;
        return BCMEAPI_CTRL_CONTINUE;
    }
    if (global.dump_enable)
    {
        printk("reason: %d src_bridge_port: %d gemflow: %d\n", info.reason, info.src_port, info.reason_data);
    }
    
    
    *pBuf = (*pFkb)->data;
    *len = (*pFkb)->len;
    *gemid = info.reason_data;

    switch (info.src_port)
    {
        case rdpa_if_wan0:
        case rdpa_if_wan1:
            *phy_port_id = wan_port_id;
            break;

#if 1
        case rdpa_if_ssid0 ... rdpa_if_ssid15:
            *is_wifi_port = 1;
            break; /* 'phy_port_id' should remain -1. */
#endif

        default:
#if defined(CONFIG_BCM_EXT_SWITCH)  /* Only external switch ports in-use */
           {
              /* Ext Switch Only */
              ((BcmEnet_hdr2*)(*pBuf))->brcm_type = BRCM_TYPE2;
              *phy_port_id = BCM_PORT_FROM_TYPE2_TAG(((BcmEnet_hdr2*)(*pBuf))->brcm_tag);
              *phy_port_id = PHYSICAL_PORT_TO_LOGICAL_PORT(*phy_port_id, 1); /* Logical port for External switch port */
           }
#else  /* Only Internal switch */
           *phy_port_id = info.src_port - rdpa_if_lan0;
#endif
           break;
   }

#if 1
    if (*is_wifi_port)
    {
        char devname[IFNAMSIZ];

        unsigned int unit;

        unit = info.src_port - rdpa_if_ssid0;
        if (unit < WL_NUM_OF_SSID_PER_UNIT)
        {
            if (unit == 0)
                sprintf(devname, "wl0");
            else
                sprintf(devname, "wl0.%u", unit);
        }
        else
        {
            if (unit == WL_NUM_OF_SSID_PER_UNIT)
                strcpy(devname, "wl1");
            else
                sprintf(devname, "wl1.%u", unit- WL_NUM_OF_SSID_PER_UNIT);
        }
        *dev = __dev_get_by_name(&init_net, devname);
        return BCMEAPI_CTRL_TRUE | BCMEAPI_CTRL_FLAG_TRUE;
    }
#endif
    return BCMEAPI_CTRL_TRUE;
}

typedef unsigned char (*EponLinkTxFunc) (unsigned char *data,unsigned int size,
    const rdpa_cpu_tx_info_t *info);

static inline int bcmeapi_pkt_xmt_dispatch (EnetXmitParams *pParam)
{
    int rc;
    rdpa_cpu_tx_info_t info = {};
    
    if (pParam->port_id == GPON_PORT_ID || ((init_cfg.wan_type == rdpa_wan_gbe) && (init_cfg.gbe_wan_emac == pParam->port_id)))
    {
        info.method = rdpa_cpu_tx_port;
        info.port = rdpa_if_wan0;
        info.x.wan.queue_id = pParam->egress_queue;
#if defined(ENET_GPON_CONFIG)
        info.x.wan.flow = pParam->gemid;
#endif
        rc = rdpa_cpu_send_sysb((bdmf_sysb)pParam->pNBuff, &info);
        if (rc < 0)
        {
#if defined(ENET_GPON_CONFIG)
            rdpa_gem_flow_us_cfg_t us_cfg = {};
            bdmf_object_handle gem = NULL; 

            rdpa_gem_get(pParam->gemid, &gem);
            if (gem)
            { 
                rdpa_gem_us_cfg_get(gem,&us_cfg); 
                bdmf_put(gem);
                if (!us_cfg.tcont)
                { 
                    printk("can't send sysb - no Tcont US cfg for gem (%d) \n", pParam->gemid);
                    return BCMEAPI_CTRL_SKIP;
                }
            }
#endif
            printk(KERN_NOTICE "rdpa_cpu_send_sysb() for GPON port "
                "returned %d (wan_flow: %d queue_id: %u)\n", rc, info.x.wan.flow,
                info.x.wan.queue_id);
            /* skb is already released by rdpa_cpu_send_sysb() */
            return BCMEAPI_CTRL_SKIP;
        }
    }
#if defined(ENET_EPON_CONFIG)
    else if (pParam->port_id == EPON_PORT_ID)
    {
	info.method = rdpa_cpu_tx_port;
	info.port = rdpa_if_wan0;
	info.x.wan.queue_id = pParam->egress_queue ;
	info.x.wan.flow = 0;
        /*TODO fix this , should not assume skb here and fkb does not have dev*/
        if (IS_FKBUFF_PTR(pParam->pNBuff) || pParam->skb->dev != oam_dev)
        {
            /* epon data traffic */
            if (epon_data_tx_func != NULL)
            {            
                if (FALSE == (*(EponLinkTxFunc)epon_data_tx_func)(pParam->pNBuff,
                    bdmf_sysb_length((bdmf_sysb)pParam->pNBuff), &info))
                {
                    //printk(KERN_NOTICE "epon_data_tx_func() failed\n");
                    /* skb is already released by rdpa_cpu_send_sysb() */
                    return BCMEAPI_CTRL_SKIP;
                }
            }
            else
            {
                printk(KERN_NOTICE "epon_data_tx_func not defined\n");
                return BCMEAPI_CTRL_BREAK;
            }
        } 
        else
        {
            /* epon oam traffic */
            if (oam_tx_func != NULL)
            {   
                if (FALSE == (*(EponLinkTxFunc)oam_tx_func)((unsigned char *)pParam->pNBuff,
                    bdmf_sysb_length((bdmf_sysb)pParam->pNBuff), &info))
                {
                    //printk(KERN_NOTICE "oam_tx_func() failed\n"); 
                    /* skb is already released by rdpa_cpu_send_sysb() */
                    return BCMEAPI_CTRL_SKIP;
                }
            }
            else
            {
                printk(KERN_NOTICE "oam_tx_func  not defined\n");
                return BCMEAPI_CTRL_BREAK;
            }
        }    
    }
#endif	          		
    else
    {
        info.method = rdpa_cpu_tx_port;
#if defined(CONFIG_BCM_EXT_SWITCH)
        info.port = rdpa_physical_port_to_rdpa_if(LOGICAL_PORT_TO_PHYSICAL_PORT(pParam->port_id));
#else
        info.port = LOGICAL_PORT_TO_PHYSICAL_PORT(pParam->port_id) + rdpa_if_lan0;
#endif
        /* ToDo: should be correct queue mapping here */
        info.x.lan.queue_id = 0;
        rc = rdpa_cpu_send_sysb((bdmf_sysb)pParam->pNBuff, &info);
        if (rc < 0)
        {
            printk(KERN_NOTICE "rdpa_cpu_send_sysb() for LAN port %d "
                "returned %d\n", pParam->port_id, rc);
            /* skb is already released by rdpa_cpu_send_sysb() */
            return BCMEAPI_CTRL_SKIP;
        }
    }
    return BCMEAPI_CTRL_CONTINUE;
}

#endif /* _BCMENET_RUNNER_INLINE_H_ */

