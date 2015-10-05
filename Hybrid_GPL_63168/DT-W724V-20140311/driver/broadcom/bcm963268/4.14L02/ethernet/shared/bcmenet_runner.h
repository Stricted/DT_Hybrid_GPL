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
#ifndef _BCMENET_RUNNER_H_
#define _BCMENET_RUNNER_H_

#include "bcmenet.h"
#include<linux/skbuff.h>

#define ENET_CACHE_SMARTFLUSH

#define NULL_FUNC_NULL_STMT 
#define NULL_FUNC_RET_VOID {}
#define NULL_FUNC_RET_INT (0)

struct BcmEnet_devctrl {
    BcmEnetDevctrlBaseClass_s;

    int             phy_addr;           /* PHY address */
    struct sk_buff *freeSkbList;
    unsigned char *skbs_p;
    unsigned char *end_skbs_p;
};

struct enet_xmit_params {
    enet_xmit_params_base;
    int channel;
    FkBuff_t *pFkb;
    struct sk_buff *skb;
};

#if defined(_CONFIG_BCM_ENDPOINT)
#define NETDEV_WEIGHT  16 // lower weight for less voice latency
#else
#define NETDEV_WEIGHT  32
#endif

#define bcmeapi_EthGetStats(log_port, rxDropped, txDropped) NULL_FUNC_RET_VOID
#define bcmeapi_init_dev(dev) NULL_FUNC_RET_INT
#define bcmeapi_EthSetPhyRate(port, enable, bps, isWanPort) NULL_FUNC_RET_VOID
#define bcmeapi_set_mac_speed(port, speed) NULL_FUNC_RET_VOID
#define bcmeapiInterruptEnable(enable) NULL_FUNC_RET_VOID
#define bcmeapi_aelink_handler(linkstatus) NULL_FUNC_RET_VOID
#define bcmeapi_select_tx_def_queue(param) NULL_FUNC_RET_VOID
#define bcmeapi_select_tx_nodef_queue(param) NULL_FUNC_RET_VOID
#define bcmeapi_link_check(vport, priv, newstat) BCMEAPI_CTRL_TRUE
#define bcmeapi_map_interrupt(pDevCtrl) NULL_FUNC_RET_VOID
#define bcmeapi_anylink_changed()  NULL_FUNC_RET_VOID
#define bcmeapi_que_select() BCMEAPI_CTRL_TRUE 
#define bcmbcmeapiInterruptEnable(enable) NULL_FUNC_RET_VOID
#define bcmeapi_buf_reclaim(pParam) NULL_FUNC_RET_VOID
#define bcmeapi_config_tx_queue(pParam) NULL_FUNC_RET_VOID
#define bcmeapi_napi_leave(pDevCtrl) NULL_FUNC_RET_VOID
#define bcmeapi_xmit_unlock_exit_post(pXmitParams) NULL_FUNC_RET_VOID
#define bcmeapi_xmit_unlock_drop_exit_post(pXmitParams) NULL_FUNC_RET_VOID
#define bcmeapi_update_rx_queue(pDevCtrl) NULL_FUNC_RET_VOID
#define bcmeapi_prepare_rx() NULL_FUNC_NULL_STMT
#define bcmeapi_prepare_next_rx(rxpktgood) BCMEAPI_CTRL_CONTINUE
#define bcmeapi_rx_post(rxpktgood) NULL_FUNC_RET_VOID
#define bcmeapi_free_irq(pDevCtrl)  NULL_FUNC_RET_VOID
#define bcmeapi_config_queue(e)  NULL_FUNC_RET_VOID
#define bcmeapi_repare_next_queue() NULL_FUNC_RET_VOID
#define bcmeapi_ioctl_ethsw_rxscheduling(e) NULL_FUNC_RET_INT
#define bcmeapi_ioctl_ethsw_wrrparam(e) NULL_FUNC_RET_INT
#define bcmeapi_ioctl_rx_pkt_rate_config(e) NULL_FUNC_RET_INT
#define bcmeapi_ioctl_rx_pkt_rate_limit_config(e) NULL_FUNC_RET_INT
#define bcmeapi_ioctl_test_config(e) NULL_FUNC_RET_INT
#define bcmeapi_dump_queue(e, pDevCtrl) NULL_FUNC_RET_VOID
#define bcmeapi_get_num_txques(ethctl) NULL_FUNC_RET_INT
#define bcmeapi_set_num_txques(ethctl) NULL_FUNC_RET_INT
#define bcmeapi_get_num_rxques(ethctl) NULL_FUNC_RET_INT
#define bcmeapi_set_num_rxques(ethctl) NULL_FUNC_RET_INT
#define bcmeapi_is_wl_tx_chain_pkt(msg_id) NULL_FUNC_RET_INT
#define bcmeapi_wl_tx_chain(pDevCtrl, pBuf, len, rxContext1, rxContext2) NULL_FUNC_RET_INT
#define bcmapi_wl_tx_chain_post(budget) NULL_FUNC_RET_VOID
#define enet_ioctl_ethsw_dos_ctrl(ethswctl_data) NULL_FUNC_RET_INT

#ifndef CARDNAME
#define CARDNAME    "BCM68500_ENET"
#endif

/* Need to keep this number of port #defines same as robo_reg.h
 * Unless we can make the LOGICAL/PHYSICAL macros bcmenet.h independent
 * of the assumption that internal and external switch have 8 ports */
#define MAX_SWITCH_PORTS    8
#if defined(CONFIG_BCM_EXT_SWITCH)
#define MAX_TOTAL_SWITCH_PORTS (MAX_SWITCH_PORTS+MAX_SWITCH_PORTS)
#else
#define MAX_TOTAL_SWITCH_PORTS (MAX_SWITCH_PORTS)
#endif
#define NETDEV_CPU_RX_QUEUE_ID 3
#define NETDEV_CPU_RX_QUEUE_SIZE 512 /* Leave 32 packets for ploams/mpcp and 64 for omci/oam */

int bcmeapi_module_init(void);
void bcmeapi_add_dev_queue(struct net_device *dev);
int bcmeapi_init_queue(BcmEnet_devctrl *pDevCtrl);
void bcmeapi_del_dev_intr(BcmEnet_devctrl *pDevCtrl);
int bcmeapi_open_dev(BcmEnet_devctrl *pDevCtrl, struct net_device *dev);
void bcmeapi_get_chip_idrev(unsigned int *chipid, unsigned int *chiprev);
int bcmeapi_queue_select(EnetXmitParams *pParam);
void bcmeapi_buf_alloc(BcmEnet_devctrl *pDevCtrl);
void bcmeapi_get_tx_queue(EnetXmitParams *pParam);
int bcmeapi_ioctl_kernel_poll(struct net_device *dev, struct ethswctl_data *e);
void bcmeapi_update_link_status(void);
void bcmeapi_enet_poll_timer(void);
void bcmeapi_add_proc_files(struct net_device *dev, BcmEnet_devctrl *pDevCtrl);
void bcmeapi_free_queue(BcmEnet_devctrl *pDevCtrl);

#endif /* _BCMENET_RUNNER_H_ */
