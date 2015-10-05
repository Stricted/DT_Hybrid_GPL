/*
    hw_card_pids.h - pids for huawei WCDMA datacard

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

/*基础形态*/
//{0x1001,"modem:ttyUSB0\nvoice:ttyUSB1\npcui:ttyUSB2\n"},

//{0x1003,"modem:ttyUSB0\npcui:ttyUSB1\n"},
{0x1001,MODEM_DIAG_PCUI},
{0X1003,MODEM_PCUI},
{0X1004,MODEM_DIAG_PCUI},
{0X141E,MODEM_PCUI},
{0X1417,MODEM_NDIS_DIAG_PCUI},
{0X1406,MODEM_PCUI},
{0X1418,MODEM_DIAG_PCUI},
{0X1420,MODEM_PCUI},
{0X140A,MODEM_DIAG_PCUI},
{0X140B,MODEM_DIAG_PCUI},
{0X1422,MODEM_DIAG_PCUI},
{0X1427,MODEM_NDIS_DIAG_PCUI},
{0X1428,MODEM_HID_DIAG_PCUI},
{0X1429,MODEM_NDIS_PCUI},
{0X142A,MODEM_NDIS_PCUI},
{0X1448,MODEM_PCUI},
{0X1402,MODEM_NDIS_PCUI},
{0X14AC,MODEM_NDIS_DIAG_PCUI},

/*扩展形态*/
{0X141A,MODEM_DIAG_PCUI},
{0x141B,MODEM_PCUI},
{0x1406,MODEM_PCUI},
{0x1411,MODEM_DIAG_PCUI},
{0x1412,MODEM_DIAG_PCUI},
{0x1413,MODEM_NDIS_DIAG_PCUI},
{0x1414,MODEM_DIAG_PCUI},
{0x1416,MODEM_DIAG_PCUI},
{0x1419,MODEM_DIAG_PCUI},
#ifdef CONFIG_ATP_NDIS_QMI
{0x140C,MODEM_QMI_DIAG_PCUI},
#else
{0x140C,MODEM_NDIS_DIAG_PCUI},
#endif
{0x1429,MODEM_NDIS_PCUI},
{0X141B,MODEM_PCUI},
{0x141A,MODEM_DIAG_PCUI},
{0x1404,MODEM_DIAG_PCUI},
{0x1407,MODEM_PCUI},
{0x142B,MODEM_PCUI},
{0x1009,MODEM_DIAG_PCUI},
{0x9002,MODEM_PCUI},
{0x1506, MODEM_NDIS_PCUI }, //NDIS
{0X1C05,MODEM_DIAG_PCUI},//E352s
/*the end string,just for a terminate flag*/
{0x9401,MODEM_DIAG_PCUI_NDIS}, //NDIS
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
{0x9025,DIAG_TEST_PCUI_NMEA_NDIS},
/* DTS2012031902908 g68080 2012-05-28 mod end> */
{0x0000,NULL},



