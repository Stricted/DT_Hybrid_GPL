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

#ifndef _BCMSW_H_
#define _BCMSW_H_

#include <bcm/bcmswapitypes.h>
#include <bcm/bcmswapistat.h>

#if !defined(CONFIG_BCM968500) && !defined(CONFIG_BCM96838)
#include "bcmsw_dma.h"
#else
#include "bcmsw_runner.h"
#endif

void fast_age_port(uint8_t port, uint8_t age_static);
int  ethsw_counter_collect(uint32_t port_map, int discard);
void bcmeapi_reset_mib_ext(void);
void fast_age_all_ext(uint8_t age_static);
void extsw_wreg_wrap(int page, int reg, uint8 *data, int len);
void extsw_rreg_wrap(int page, int reg, uint8 *data, int len);
void extsw_wreg(int page, int reg, uint8 *data, int len);
void extsw_rreg(int page, int reg, uint8 *data, int len);
void extsw_set_wanoe_portmap(uint16 wan_port_map);
void extsw_fast_age_port(uint8 port, uint8 age_static);
#if defined(CONFIG_BCM_EXT_SWITCH)
void extsw_init_config(void);
void bcmsw_set_ext_switch_pbvlan(int port, uint16_t portMap);
#else
#define extsw_init_config() {}
#define bcmsw_set_ext_switch_pbvlan(port, portMap) {}
#endif
/*Added by luoqi@20120918 00203771 for DTS2012081406305 */
int ethsw_set_multiport_address(uint8_t* addr, int ext_switch_exist);
/*Added by luoqi@20120918 00203771 for DTS2012081406305 */
int remove_arl_entry_wrapper(void *ptr);
int bcmsw_dump_mib_ext(int port, int type);
int bcmsw_set_multiport_address_ext(uint8_t* addr);
int bcmeapi_ioctl_set_multiport_address(struct ethswctl_data *e);
int bcmsw_get_hw_stats(int port, int extswitch, struct net_device_stats *stats);
void enet_arl_read_ext( uint8_t *mac, uint32_t *vid, uint32_t *val );
void enet_arl_write_ext( uint8_t *mac, uint16_t vid, uint32_t val );
int enet_arl_access_dump_ext(void);
void enet_arl_dump_ext_multiport_arl(void);
int bcmeapi_ioctl_extsw_port_jumbo_control(struct ethswctl_data *e);
int bcmsw_enable_hw_switching(void);
int bcmsw_disable_hw_switching(void);
#endif /* _BCMSW_H_ */
