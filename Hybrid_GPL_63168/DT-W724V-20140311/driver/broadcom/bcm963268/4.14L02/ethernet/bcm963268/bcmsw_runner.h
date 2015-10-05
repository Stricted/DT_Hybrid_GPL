/*
    Copyright 2004-2010 Broadcom Corporation

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

#ifndef _BCMSW_RUNNER_H_
#define _BCMSW_RUNNER_H_

#include <bcm/bcmswapitypes.h>
#include <bcm/bcmswapistat.h>
#include "bcmenet_common.h"

/* Internal switch access functions */
#define ethsw_rreg(page, reg, data, len) {}
#define ethsw_wreg(page, reg, data, len) {}
#define ethsw_read_reg(addr, data, len) {}
#define ethsw_write_reg(addr, data, len) {}

#define bcmeapi_ioctl_ethsw_control(e) 0
#define bcmeapi_ioctl_ethsw_prio_control(e) 0
#define bcmeapi_ioctl_ethsw_vlan(e) 0
#define bcmeapi_ioctl_ethsw_pmdioaccess(dev, e) 0
#define bcmeapi_ioctl_ethsw_cosq_txchannel_mapping(e) 0
#define bcmeapi_ioctl_ethsw_cosq_sched(e) 0
#define bcmeapi_ioctl_ethsw_spiaccess(bus_num, spi_id, chip_id, e) 0
#define bcmeapi_ioctl_ethsw_cosq_config(e) 0
#define bcmeapi_ioctl_ethsw_clear_port_stats(e) 0
#define bcmeapi_ioctl_ethsw_port_loopback(e, phy_id) 0
#define bcmeapi_ioctl_ethsw_port_jumbo_control(e) 0
#define bcmeapi_ioctl_ethsw_cos_priority_method_config(e) 0
#define bcmeapi_ioctl_ethsw_port_default_tag_config(e) 0
#define bcmeapi_ioctl_ethsw_phy_mode(e, phy_id) 0
#define bcmeapi_ioctl_ethsw_clear_stats(portmap) 0
#define bcmeapi_ioctl_ethsw_cosq_txq_sel(e) 0
#define bcmeapi_ioctl_ethsw_counter_get(e) 0
#define bcmeapi_ioctl_ethsw_cosq_rxchannel_mapping(e) 0
#define bcmeapi_ioctl_ethsw_info(dev, e) 0
#define bcmeapi_ioctl_ethsw_regaccess(e) 0
#define bcmeapi_ioctl_ethsw_port_irc_set(e) 0
#define bcmeapi_ioctl_ethsw_pbvlan(e) 0
#define bcmeapi_ioctl_ethsw_port_erc_set(e) 0
#define bcmeapi_ioctl_ethsw_port_irc_get(e) 0
#define bcmeapi_ioctl_ethsw_port_erc_get(e) 0

#define bcmeapi_ethsw_init_hw(unit, portMap, wanPortMap) {}
#define bcmeapi_ioctl_debug_conf(e)
#define bcmeapi_ethsw_dump_page(page) {}
#define bcmeapi_ethsw_set_stp_mode(unit, port, stpState) {}
#define bcmeapi_ioctl_ethsw_cosq_port_mapping(e) 0
#define enet_arl_read( mac, vid, val ) {}
#define enet_arl_write(mac, vid, val) {}
#define enet_arl_access_dump() {}  /* This should return status actually ?? */
#define enet_arl_dump_multiport_arl() {}
#define fast_age_all(age_static) {}

#define bcmsw_spi_rreg(bus_num, spi_ss, chip_id, page, reg, data, len) {}
#define bcmsw_spi_wreg(bus_num, spi_ss, chip_id, page, reg, data, len) {}
#define extsw_eee_init() {}
#define extsw_apd_set_compatibility_mode() {}
#define remove_arl_entry(mac) 0  /* This should return status actually ?? */
#define bcmeapi_reset_mib() {}
#define bcmeapi_set_multiport_address(addr) {}
int bcmeapi_init_ext_sw_if(extsw_info_t *extSwInfo);
int bcmeapi_ioctl_ethsw_clear_port_emac(struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_get_port_emac(struct ethswctl_data *e);
#ifdef CONFIG_BCM96838
int bcmeapi_ioctl_ethsw_port_pause_capability(struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_traffic_control(struct ethswctl_data *e, int phy_id);
int bcmeapi_ioctl_ethsw_phy_autoneg_info(struct ethswctl_data *e, int phy_id);
int bcmeapi_ioctl_ethsw_link_status(struct ethswctl_data *e, int phy_id);
int bcmeapi_ioctl_ethsw_port_transparent_set (struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_vlanlist_set (struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_vlan_isolation_set (struct ethswctl_data *e);
#else  // CONFIG_BCM96838
#define bcmeapi_ioctl_ethsw_port_pause_capability(e) 0
#define bcmeapi_ioctl_ethsw_port_traffic_control(e) 0
#endif  // CONFIG_BCM96838
int ethsw_get_hw_stats(int port, struct net_device_stats *stats);
int bcmeapi_ioctl_ethsw_sal_dal_set (struct ethswctl_data *e);
int ethsw_set_mac_hw(int port, PHY_STAT ps);
int bcmeapi_ioctl_ethsw_mtu_set (struct ethswctl_data *e);
#endif /* _BCMSW_RUNNER_H_ */
