/*
    hw_hspa.h - struct for support huawei datacard devices

    * Initial work by:    
     *   (c) 20090508 David Lv (l00135113@huawei.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __LINUX_HW_HSPA_H
#define __LINUX_HW_HSPA_H

#include <linux/kref.h>
#include <linux/mutex.h>

#define BANDLUXE_C270_VENDOR_ID	0x1A8D
#define HUAWEI_HSPA_CARD	"Huawei"
#define BANDLUXE_HSPA_CARD	"Bandluxe"
/*hw_datacard struct,to match the pid and proc_users_umts string*/
 struct hw_datacard_id 
{
    __u16 pid;

    const char * portString;
};

/*the interfaces' order: modem+diag+pcui*/
#define MODEM_DIAG_PCUI         "modem:ttyUSB0\nvoice:ttyUSB1\npcui:ttyUSB2\n"

/*the interfaces' order: modem+pcui*/
#define MODEM_PCUI                  "modem:ttyUSB0\npcui:ttyUSB1\n"

/*the interfaces' order: MDM+NDIS+DIAG+PCUI*/
#define MODEM_NDIS_DIAG_PCUI  "modem:ttyUSB0\nndis:ttyUSB1\nvoice:ttyUSB2\npcui:ttyUSB3\n"

/*the interfaces' order:MDM+HID+DIAG+PCUI*/
#define MODEM_HID_DIAG_PCUI     "modem:ttyUSB0\nhid:ttyUSB1\nvoice:ttyUSB2\npcui:ttyUSB3\n"

/*the interfaces' order:MDM+NDIS+PCUI*/
#define MODEM_NDIS_PCUI         "modem:ttyUSB0\nndis:ttyUSB1\npcui:ttyUSB2\n"

/*the interface's order:MDM+PCUI*/

/*为什么有个相同的定义?注释by w00190448*/
//#define MODEM_PCUI         "modem:ttyUSB0\npcui:ttyUSB2\n"
/*the interfaces' order: modem+diag+pcui*/
#define MODEM_DIAG_PCUI_NDIS         "modem:ttyUSB0\nvoice:ttyUSB1\npcui:ttyUSB2\nndis:ttyUSB3\n"
/*the interfaces' order: modem+ndis+diag+pcui*/
#define MODEM_QMI_DIAG_PCUI         "modem:ttyUSB0\nndis:qmitty0\ndiag:ttyUSB1\npcui:ttyUSB2\n"
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#define DIAG_TEST_PCUI_NMEA_NDIS         "diagd:ttyUSB0\ntest:ttyUSB1\npcui:ttyUSB2\nnema:ttyUSB3\n	"
/* DTS2012031902908 g68080 2012-05-28 mod end> */

#endif	/* ifdef __LINUX_HW_HSPA_H */
