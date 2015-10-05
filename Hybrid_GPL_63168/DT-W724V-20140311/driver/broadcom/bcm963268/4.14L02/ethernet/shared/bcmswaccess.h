/*
<:copyright-BRCM:2002:DUAL/GPL:standard

   Copyright (c) 2002 Broadcom Corporation
   All Rights Reserved

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
#ifndef _BCMSWACCESS_H_
#define _BCMSWACCESS_H_

#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
#include "ethsw_runner.h"   /* This should actually go in bcmswaccess_runner.h */
#else
#include "bcmswaccess_dma.h"
#endif /* !CONFIG_BCM968500 */
void bcmsw_pmdio_rreg(int page, int reg, uint8 *data, int len);
void bcmsw_pmdio_wreg(int page, int reg, uint8 *data, int len);
void get_ext_switch_access_info(int usConfigType, int *bus_type, int *spi_id);
int enet_ioctl_ethsw_regaccess(struct ethswctl_data *e);
int enet_ioctl_ethsw_pmdioaccess(struct net_device *dev, struct ethswctl_data *e);
int enet_ioctl_ethsw_info(struct net_device *dev, struct ethswctl_data *e);
int enet_ioctl_phy_cfg_get(struct net_device *dev, struct ethswctl_data *e);

#define bcmeapi_ioctl_phy_cfg_get enet_ioctl_phy_cfg_get

#endif /* _BCMSWACCESS_H_ */

