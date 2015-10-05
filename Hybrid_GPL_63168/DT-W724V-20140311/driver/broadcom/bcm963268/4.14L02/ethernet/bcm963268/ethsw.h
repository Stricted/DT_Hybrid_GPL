/*
 Copyright 2004-2010 Broadcom Corp. All Rights Reserved.

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
#ifndef _ETHSW_H_
#define _ETHSW_H_

#include "bcmenet_common.h"
#include "bcmmii.h"
#if !defined(CONFIG_BCM968500) && !defined(CONFIG_BCM96838)
#include "ethsw_dma.h"
#else
#include "ethsw_runner.h"
#endif


/****************************************************************************
    Prototypes
****************************************************************************/

int ethsw_setup_led(void);
void ethsw_setup_hw_apd(unsigned int enable);
int ethsw_phy_pll_up(int ephy_and_gphy);
uint32 ethsw_ephy_auto_power_down_wakeup(void);
uint32 ethsw_ephy_auto_power_down_sleep(void);
int ethsw_reset_ports(struct net_device *dev);
int ethsw_set_mac(int port, PHY_STAT ps);
int ethsw_phy_intr_ctrl(int port, int on);
PHY_STAT ethsw_phy_stat(int port);
void ethsw_switch_power_off(void* context);
void ethsw_init_config(void);
int ethsw_setup_phys(void);
void ethsw_phy_advertise_caps(void);
void ethsw_phy_apply_init_bp(void);
int ethsw_add_proc_files(struct net_device *dev);
int ethsw_del_proc_files(void);
int ethsw_enable_sar_port(void);
int ethsw_disable_sar_port(void);
int ethsw_save_port_state(void);
int ethsw_restore_port_state(void);
int ethsw_port_to_phyid(int port);
void ethsw_port_based_vlan(int port_map, int wan_port_bit, int txSoftSwitchingMap);
int ethsw_enable_hw_switching(void);
int ethsw_disable_hw_switching(void);
int ethsw_set_hw_switching(uint32 state);
int ethsw_get_hw_switching_state(void);

void ethsw_phyport_wreg(int port, int reg, uint16 *data);
void ethsw_phyport_rreg(int port, int reg, uint16 *data);

void ethsw_configure_ports(int port_map, int *pphy_id);
void ethsw_phy_advertise_all(uint32 phy_id);
void ethsw_isolate_phy(int phyId, int isolate);
void ethsw_eee_init(void);
void ethsw_eee_port_enable(int port, int enable, int linkstate);
void ethsw_eee_process_delayed_enable_requests(void);
void ethsw_phy_config(void);
void ethsw_init_table(BcmEnet_devctrl *pDevCtrl);
int  bcmeapi_ioctl_ethsw_arl_access(struct ethswctl_data *e);

#if defined(CONFIG_BCM96828) && !defined(CONFIG_EPON_HGU)
void saveEthPortToRxIudmaConfig(uint8 port, uint8 iudma);
int restoreEthPortToRxIudmaConfig(uint8 port);
int enet_learning_ctrl(uint32_t portMask, uint8_t enable);
int bcm_fun_enet_drv_handler(void *ptr);
int epon_uni_to_uni_ctrl(unsigned int portMap, int val);
#endif

#endif /* _ETHSW_H_ */
