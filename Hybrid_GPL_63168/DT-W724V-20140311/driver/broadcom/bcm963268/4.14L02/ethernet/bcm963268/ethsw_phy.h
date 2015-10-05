/*
 Copyright 2004-2012 Broadcom Corp. All Rights Reserved.

<:label-BRCM:2012:GPL/GPL:standard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/
#ifndef ETHSW_PHY_H
#define ETHSW_PHY_H
#if !defined(CONFIG_BCM968500) && !defined(CONFIG_BCM96838)  /* Temporary - isolate the inclusion of this file between runner and non-runner devices */

#include "bcm_OS_Deps.h"
#include "bcmtypes.h"
#include "bcmmii.h"
#include "ethsw.h"

#define ETHSW_PHY_GET_PHYID(port) switch_pport_phyid[port]

void ethsw_phy_rreg(int phy_id, int reg, uint16 *data);
void ethsw_phy_wreg(int phy_id, int reg, uint16 *data);

void ethsw_phy_read_reg(int phy_id, int reg, uint16 *data, int ext_bit);
void ethsw_phy_write_reg(int phy_id, int reg, uint16 *data, int ext_bit);

#endif /* !CONFIG_BCM968500 */
#endif /*ETHSW_PHY_H*/

