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
#ifndef _BCMENET_H_
#define _BCMENET_H_

#ifndef FAP_4KE
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/nbuff.h>
#include "boardparms.h"
#include "bcm_OS_Deps.h"
#include <linux/bcm_log.h>
#include <bcmnet.h>
#include <bcm/bcmswapitypes.h>
#include <linux/version.h>
#endif

#if defined(PKTC)
#include <linux_osl_dslcpe_pktc.h>
#endif

#include "bcmtypes.h"
#include "bcmenet_common.h"
/* Macros need to be defined after hardware dependent headers */
#define NUM_PORTS                   1

#if defined(ENET_EPON_CONFIG)
#define MAX_EPON_IFS 8 //eight LLID
extern struct net_device* eponifid_to_dev[];
extern struct net_device* oam_dev;
extern void *oam_tx_func;
extern void *epon_data_tx_func;
#endif
#if defined(_BCMENET_LOCAL_)
#define ENET_POLL_DONE        0x80000000

typedef struct {
    unsigned int extPhyMask;
    int dump_enable;
    struct net_device_stats net_device_stats_from_hw;
    BcmEnet_devctrl *pVnetDev0_g;
}enet_global_var_t;

extern enet_global_var_t global;
extern struct net_device *vnet_dev[];
extern int vport_to_logicalport[];
extern int logicalport_to_vport[];

#define LOGICAL_PORT_TO_VPORT(p) logicalport_to_vport[(p)]
#define VPORT_TO_LOGICAL_PORT(p) vport_to_logicalport[(p)]

#ifndef FAP_4KE

#ifdef DYING_GASP_API


/* Flag indicates we're in Dying Gasp and powering down - don't clear once set */
extern int dg_in_context; 

#define ENET_TX_LOCK() if(dg_in_context==0) spin_lock_bh(&global.pVnetDev0_g->ethlock_tx)
#define ENET_TX_UNLOCK() if(dg_in_context==0) spin_unlock_bh(&global.pVnetDev0_g->ethlock_tx)
#define ENET_RX_LOCK() if(dg_in_context==0) spin_lock_bh(&global.pVnetDev0_g->ethlock_rx)
#define ENET_RX_UNLOCK() if(dg_in_context==0) spin_unlock_bh(&global.pVnetDev0_g->ethlock_rx)
#define ENET_MOCA_TX_LOCK() if(dg_in_context==0) spin_lock_bh(&global.pVnetDev0_g->ethlock_moca_tx)
#define ENET_MOCA_TX_UNLOCK() if(dg_in_context==0) spin_unlock_bh(&global.pVnetDev0_g->ethlock_moca_tx)

#else


#define ENET_TX_LOCK() spin_lock_bh(&global.pVnetDev0_g->ethlock_tx)
#define ENET_TX_UNLOCK() spin_unlock_bh(&global.pVnetDev0_g->ethlock_tx)
#define ENET_RX_LOCK() spin_lock_bh(&global.pVnetDev0_g->ethlock_rx)
#define ENET_RX_UNLOCK() spin_unlock_bh(&global.pVnetDev0_g->ethlock_rx)
#define ENET_MOCA_TX_LOCK() spin_lock_bh(&global.pVnetDev0_g->ethlock_moca_tx)
#define ENET_MOCA_TX_UNLOCK() spin_unlock_bh(&global.pVnetDev0_g->ethlock_moca_tx)

#endif /* ELSE #ifdef DYING_GASP_API */


#endif
#endif /* #if defined(_BCMENET_LOCAL_) */

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#define local_unreachable() __builtin_unreachable()
#else
#define local_unreachable() do { } while(0)
#endif

#define BULK_RX_LOCK_ACTIVE() pDevCtrl->bulk_rx_lock_active[cpuid]
#define RECORD_BULK_RX_LOCK() pDevCtrl->bulk_rx_lock_active[cpuid] = 1
#define RECORD_BULK_RX_UNLOCK() pDevCtrl->bulk_rx_lock_active[cpuid] = 0
#define DMA_THROUGHPUT_TEST_EN  0x80000

#if !defined(CONFIG_BCM968500) && !defined(CONFIG_BCM96838)
#include "bcmenet_dma.h"
#else
#include "bcmenet_runner.h"
#endif

#if defined(_BCMENET_LOCAL_)
void bcmeapi_create_vport(struct net_device *dev);
void bcmeapi_napi_post(struct BcmEnet_devctrl *pDevCtrl);
void ethsw_phyport_rreg2(int phy_id, int reg, uint16 *data, int flags);
void ethsw_phyport_wreg2(int phy_id, int reg, uint16 *data, int flags);
/*
 * IMPORTANT: The following 3 macros are only used for ISR context. The
 * recycling context is defined by enet_recycle_context_t
 */
#define CONTEXT_CHAN_MASK   0x3
#define BUILD_CONTEXT(pDevCtrl,channel) \
            (uint32)((uint32)(pDevCtrl) | ((uint32)(channel) & CONTEXT_CHAN_MASK))
#define CONTEXT_TO_PDEVCTRL(context)    (BcmEnet_devctrl*)((context) & ~CONTEXT_CHAN_MASK)
#define CONTEXT_TO_CHANNEL(context)     (int)((context) & CONTEXT_CHAN_MASK)

unsigned short bcm_type_trans(struct sk_buff *skb, struct net_device *dev);

#endif /* _BCMENET_LOCAL_ */

int bcm63xx_enet_getPortFromName(char *pIfName, int *pUnit, int *pPort);


#if defined(RXCHANNEL_PKT_RATE_LIMIT)
#endif /* defined(RXCHANNEL_PKT_RATE_LIMIT) */

#ifdef DYING_GASP_API
int enet_send_dying_gasp_pkt(void);
#endif /* DYING_GASP_API */

void bcmeapi_reset_mib_cnt(uint32_t sw_port);
struct net_device *enet_phyport_to_vport_dev(int port);

int bcm63xx_enet_isExtSwPresent(void);
int bcm63xx_enet_intSwPortToExtSw(void);
unsigned int bcm63xx_enet_extSwId(void);
void bcmeapi_enet_module_cleanup(void);
uint32 ConfigureJumboPort(uint32 regVal, int portVal, unsigned int configVal);
void bcmeapi_module_init2(void);
void link_change_handler(int port, int linkstatus, int speed, int duplex);

#endif /* _BCMENET_H_ */

