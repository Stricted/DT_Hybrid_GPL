/*
  USB Driver for GSM modems

  Copyright (C) 2005  Matthias Urlichs <smurf@smurf.noris.de>

  This driver is free software; you can redistribute it and/or modify
  it under the terms of Version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  Portions copied from the Keyspan driver by Hugh Blemings <hugh@blemings.org>

  History: see the git log.

  Work sponsored by: Sigos GmbH, Germany <info@sigos.de>

  This driver exists because the "normal" serial driver doesn't work too well
  with GSM modems. Issues:
  - data loss -- one single Receive URB is not nearly enough
  - nonstandard flow (Option devices) control
  - controlling the baud rate doesn't make sense

  This driver is named "option" because the most common device it's
  used for is a PC-Card (with an internal OHCI-USB interface, behind
  which the GSM interface sits), made by Option Inc.

  Some of the "one port" devices actually exhibit multiple USB instances
  on the USB bus. This is not a bug, these ports are used for different
  device features.
*/
/******************************************************************************   
l00205975    2012/07/17  DTS2012071700779    【usb】：ttyUSB接口反复打开出现IO错误 
q00186737    2012/12/19  DTS2012121906958     [880]路由透传升级功能
q00186737    2013/02/07  DTS2013020703621     [B880]histudio透传log
******************************************************************************/
#define DRIVER_VERSION "v0.7.2"
#define DRIVER_AUTHOR "Matthias Urlichs <smurf@smurf.noris.de>"
#define DRIVER_DESC "USB Driver for GSM modems"

#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#ifdef CONFIG_ATP_USB_ADAPTOR
#include <linux/proc_fs.h>
#endif
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#ifdef CONFIG_ATP_USB_ADAPTOR
#include "hw_hspa.h"

#endif
/*Add start, DTS2010081100920, l67944, 20100824*/
#if defined (CONFIG_ATP_BOARDLED)
#include "boardled.h"
extern int g_USB_PROBE;
#endif
/*Add end, l67944, 20100824*/	
/*Start Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/
#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM96318)
#include "board.h"
extern int g_isDataCard;
extern int g_USB_PROBE;
extern unsigned long usbLastRx;
#endif
/*End Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/

/* Function prototypes */
static int  option_open(struct tty_struct *tty, struct usb_serial_port *port,
							struct file *filp);
static void option_close(struct tty_struct *tty, struct usb_serial_port *port,
							struct file *filp);
static int  option_startup(struct usb_serial *serial);
static void option_shutdown(struct usb_serial *serial);
static int  option_write_room(struct tty_struct *tty);

static void option_instat_callback(struct urb *urb);

static int option_write(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count);
static int  option_chars_in_buffer(struct tty_struct *tty);
static void option_set_termios(struct tty_struct *tty,
			struct usb_serial_port *port, struct ktermios *old);
static int  option_tiocmget(struct tty_struct *tty, struct file *file);
static int  option_tiocmset(struct tty_struct *tty, struct file *file,
				unsigned int set, unsigned int clear);
static int  option_send_setup(struct tty_struct *tty, struct usb_serial_port *port);
static int  option_suspend(struct usb_serial *serial, pm_message_t message);
static int  option_resume(struct usb_serial *serial);
#ifdef CONFIG_ATP_USB_ADAPTOR
static int usb_option_clear_halt(struct usb_device *dev, int pipe);
#endif

/* Vendor and product IDs */
#define OPTION_VENDOR_ID			0x0AF0
#define OPTION_PRODUCT_COLT			0x5000
#define OPTION_PRODUCT_RICOLA			0x6000
#define OPTION_PRODUCT_RICOLA_LIGHT		0x6100
#define OPTION_PRODUCT_RICOLA_QUAD		0x6200
#define OPTION_PRODUCT_RICOLA_QUAD_LIGHT	0x6300
#define OPTION_PRODUCT_RICOLA_NDIS		0x6050
#define OPTION_PRODUCT_RICOLA_NDIS_LIGHT	0x6150
#define OPTION_PRODUCT_RICOLA_NDIS_QUAD		0x6250
#define OPTION_PRODUCT_RICOLA_NDIS_QUAD_LIGHT	0x6350
#define OPTION_PRODUCT_COBRA			0x6500
#define OPTION_PRODUCT_COBRA_BUS		0x6501
#define OPTION_PRODUCT_VIPER			0x6600
#define OPTION_PRODUCT_VIPER_BUS		0x6601
#define OPTION_PRODUCT_GT_MAX_READY		0x6701
#define OPTION_PRODUCT_FUJI_MODEM_LIGHT		0x6721
#define OPTION_PRODUCT_FUJI_MODEM_GT		0x6741
#define OPTION_PRODUCT_FUJI_MODEM_EX		0x6761
#define OPTION_PRODUCT_KOI_MODEM		0x6800
#define OPTION_PRODUCT_SCORPION_MODEM		0x6901
#define OPTION_PRODUCT_ETNA_MODEM		0x7001
#define OPTION_PRODUCT_ETNA_MODEM_LITE		0x7021
#define OPTION_PRODUCT_ETNA_MODEM_GT		0x7041
#define OPTION_PRODUCT_ETNA_MODEM_EX		0x7061
#define OPTION_PRODUCT_ETNA_KOI_MODEM		0x7100
#define OPTION_PRODUCT_GTM380_MODEM		0x7201
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#define MDM9215_VENDOR_ID			0x05C6
#define HUAWEI_PRODUCT_B890             0x9025
/* DTS2012031902908 g68080 2012-05-28 mod end> */
#define HUAWEI_VENDOR_ID			0x12D1
#ifdef CONFIG_ATP_USB_ADAPTOR
#define AUDIOVOX_VENDOR_ID              0x0F3D
#define NOVATELWIRELESS_VENDOR_ID       0x1410
#define ANYDATA_VENDOR_ID               0x16d5
#define AUDIOVOX_PRODUCT_AIRCARD        0x0112
//#define NOVATELWIRELESS_PRODUCT_U740    0x1400
#define ANYDATA_PRODUCT_ID              0x6501
#define OPTION_PRODUCT_OLD              0x5000
#define OPTION_PRODUCT_FUSION           0x6000
#define OPTION_PRODUCT_FUSION2          0x6300
#define OPTION_PRODUCT_COBRA            0x6500
#define OPTION_PRODUCT_COBRA2           0x6600
#define OPTION_PRODUCT_GTMAX36          0x6701
#endif
#define HUAWEI_PRODUCT_E600			0x1001
#define HUAWEI_PRODUCT_E220			0x1003
#define HUAWEI_PRODUCT_E220BIS			0x1004
#define HUAWEI_PRODUCT_E1401			0x1401
#define HUAWEI_PRODUCT_E1402			0x1402
#define HUAWEI_PRODUCT_E1403			0x1403
#define HUAWEI_PRODUCT_E1404			0x1404
#define HUAWEI_PRODUCT_E1405			0x1405
#define HUAWEI_PRODUCT_E1406			0x1406
#define HUAWEI_PRODUCT_E1407			0x1407
#define HUAWEI_PRODUCT_E1408			0x1408
#define HUAWEI_PRODUCT_E1409			0x1409
#define HUAWEI_PRODUCT_E140A			0x140A
#define HUAWEI_PRODUCT_E140B			0x140B
#define HUAWEI_PRODUCT_E140C			0x140C
#define HUAWEI_PRODUCT_E140D			0x140D
#define HUAWEI_PRODUCT_E140E			0x140E
#define HUAWEI_PRODUCT_E140F			0x140F
#define HUAWEI_PRODUCT_E1410			0x1410
#define HUAWEI_PRODUCT_E1411			0x1411
#define HUAWEI_PRODUCT_E1412			0x1412
#define HUAWEI_PRODUCT_E1413			0x1413
#define HUAWEI_PRODUCT_E1414			0x1414
#define HUAWEI_PRODUCT_E1415			0x1415
#define HUAWEI_PRODUCT_E1416			0x1416
#define HUAWEI_PRODUCT_E1417			0x1417
#define HUAWEI_PRODUCT_E1418			0x1418
#define HUAWEI_PRODUCT_E1419			0x1419
#define HUAWEI_PRODUCT_E141A			0x141A
#define HUAWEI_PRODUCT_E141B			0x141B
#define HUAWEI_PRODUCT_E141C			0x141C
#define HUAWEI_PRODUCT_E141D			0x141D
#define HUAWEI_PRODUCT_E141E			0x141E
#define HUAWEI_PRODUCT_E141F			0x141F
#define HUAWEI_PRODUCT_E1420			0x1420
#define HUAWEI_PRODUCT_E1421			0x1421
#define HUAWEI_PRODUCT_E1422			0x1422
#define HUAWEI_PRODUCT_E1423			0x1423
#define HUAWEI_PRODUCT_E1424			0x1424
#define HUAWEI_PRODUCT_E1425			0x1425
#define HUAWEI_PRODUCT_E1426			0x1426
#define HUAWEI_PRODUCT_E1427			0x1427
#define HUAWEI_PRODUCT_E1428			0x1428
#define HUAWEI_PRODUCT_E1429			0x1429
#define HUAWEI_PRODUCT_E142A			0x142A
#define HUAWEI_PRODUCT_E142B			0x142B
#define HUAWEI_PRODUCT_E142C			0x142C
#define HUAWEI_PRODUCT_E142D			0x142D
#define HUAWEI_PRODUCT_E142E			0x142E
#define HUAWEI_PRODUCT_E142F			0x142F
#define HUAWEI_PRODUCT_E1430			0x1430
#define HUAWEI_PRODUCT_E1431			0x1431
#define HUAWEI_PRODUCT_E1432			0x1432
#define HUAWEI_PRODUCT_E1433			0x1433
#define HUAWEI_PRODUCT_E1434			0x1434
#define HUAWEI_PRODUCT_E1435			0x1435
#define HUAWEI_PRODUCT_E1436			0x1436
#define HUAWEI_PRODUCT_E1437			0x1437
#define HUAWEI_PRODUCT_E1438			0x1438
#define HUAWEI_PRODUCT_E1439			0x1439
#define HUAWEI_PRODUCT_E143A			0x143A
#define HUAWEI_PRODUCT_E143B			0x143B
#define HUAWEI_PRODUCT_E143C			0x143C
#define HUAWEI_PRODUCT_E143D			0x143D
#define HUAWEI_PRODUCT_E143E			0x143E
#define HUAWEI_PRODUCT_E143F			0x143F

#define QUANTA_VENDOR_ID			0x0408
#define QUANTA_PRODUCT_Q101			0xEA02
#define QUANTA_PRODUCT_Q111			0xEA03
#define QUANTA_PRODUCT_GLX			0xEA04
#define QUANTA_PRODUCT_GKE			0xEA05
#define QUANTA_PRODUCT_GLE			0xEA06

#ifdef CONFIG_ATP_USB_ADAPTOR
#define HUAWEI_PRODUCT_B561             0x1004
#endif
#define NOVATELWIRELESS_VENDOR_ID		0x1410

/* YISO PRODUCTS */

#define YISO_VENDOR_ID				0x0EAB
#define YISO_PRODUCT_U893			0xC893

/* MERLIN EVDO PRODUCTS */
#define NOVATELWIRELESS_PRODUCT_V640		0x1100
#define NOVATELWIRELESS_PRODUCT_V620		0x1110
#define NOVATELWIRELESS_PRODUCT_V740		0x1120
#define NOVATELWIRELESS_PRODUCT_V720		0x1130

/* MERLIN HSDPA/HSPA PRODUCTS */
#define NOVATELWIRELESS_PRODUCT_U730		0x1400
#define NOVATELWIRELESS_PRODUCT_U740		0x1410
#define NOVATELWIRELESS_PRODUCT_U870		0x1420
#define NOVATELWIRELESS_PRODUCT_XU870		0x1430
#define NOVATELWIRELESS_PRODUCT_X950D		0x1450

/* EXPEDITE PRODUCTS */
#define NOVATELWIRELESS_PRODUCT_EV620		0x2100
#define NOVATELWIRELESS_PRODUCT_ES720		0x2110
#define NOVATELWIRELESS_PRODUCT_E725		0x2120
#define NOVATELWIRELESS_PRODUCT_ES620		0x2130
#define NOVATELWIRELESS_PRODUCT_EU730		0x2400
#define NOVATELWIRELESS_PRODUCT_EU740		0x2410
#define NOVATELWIRELESS_PRODUCT_EU870D		0x2420

/* OVATION PRODUCTS */
#define NOVATELWIRELESS_PRODUCT_MC727		0x4100
#define NOVATELWIRELESS_PRODUCT_MC950D		0x4400
#define NOVATELWIRELESS_PRODUCT_U727		0x5010

/* FUTURE NOVATEL PRODUCTS */
#define NOVATELWIRELESS_PRODUCT_EVDO_FULLSPEED	0X6000
#define NOVATELWIRELESS_PRODUCT_EVDO_HIGHSPEED	0X6001
#define NOVATELWIRELESS_PRODUCT_HSPA_FULLSPEED	0X7000
#define NOVATELWIRELESS_PRODUCT_HSPA_HIGHSPEED	0X7001
#define NOVATELWIRELESS_PRODUCT_EVDO_EMBEDDED_FULLSPEED	0X8000
#define NOVATELWIRELESS_PRODUCT_EVDO_EMBEDDED_HIGHSPEED	0X8001
#define NOVATELWIRELESS_PRODUCT_HSPA_EMBEDDED_FULLSPEED	0X9000
#define NOVATELWIRELESS_PRODUCT_HSPA_EMBEDDED_HIGHSPEED	0X9001
#define NOVATELWIRELESS_PRODUCT_GLOBAL		0XA001

/* AMOI PRODUCTS */
#define AMOI_VENDOR_ID				0x1614
#define AMOI_PRODUCT_H01			0x0800
#define AMOI_PRODUCT_H01A			0x7002
#define AMOI_PRODUCT_H02			0x0802

#define DELL_VENDOR_ID				0x413C

/* Dell modems */
#define DELL_PRODUCT_5700_MINICARD		0x8114
#define DELL_PRODUCT_5500_MINICARD		0x8115
#define DELL_PRODUCT_5505_MINICARD		0x8116
#define DELL_PRODUCT_5700_EXPRESSCARD		0x8117
#define DELL_PRODUCT_5510_EXPRESSCARD		0x8118

#define DELL_PRODUCT_5700_MINICARD_SPRINT	0x8128
#define DELL_PRODUCT_5700_MINICARD_TELUS	0x8129

#define DELL_PRODUCT_5720_MINICARD_VZW		0x8133
#define DELL_PRODUCT_5720_MINICARD_SPRINT	0x8134
#define DELL_PRODUCT_5720_MINICARD_TELUS	0x8135
#define DELL_PRODUCT_5520_MINICARD_CINGULAR	0x8136
#define DELL_PRODUCT_5520_MINICARD_GENERIC_L	0x8137
#define DELL_PRODUCT_5520_MINICARD_GENERIC_I	0x8138

#define DELL_PRODUCT_5730_MINICARD_SPRINT	0x8180
#define DELL_PRODUCT_5730_MINICARD_TELUS	0x8181
#define DELL_PRODUCT_5730_MINICARD_VZW		0x8182

#define KYOCERA_VENDOR_ID			0x0c88
#define KYOCERA_PRODUCT_KPC650			0x17da
#define KYOCERA_PRODUCT_KPC680			0x180a

#define ANYDATA_VENDOR_ID			0x16d5
#define ANYDATA_PRODUCT_ADU_620UW		0x6202
#define ANYDATA_PRODUCT_ADU_E100A		0x6501
#define ANYDATA_PRODUCT_ADU_500A		0x6502

#define AXESSTEL_VENDOR_ID			0x1726
#define AXESSTEL_PRODUCT_MV110H			0x1000

#define ONDA_VENDOR_ID				0x19d2
#define ONDA_PRODUCT_MSA501HS			0x0001
#define ONDA_PRODUCT_ET502HS			0x0002
#define ONDA_PRODUCT_MT503HS			0x2000

#define BANDRICH_VENDOR_ID			0x1A8D
#define BANDRICH_PRODUCT_C100_1			0x1002
#define BANDRICH_PRODUCT_C100_2			0x1003
#define BANDRICH_PRODUCT_1004			0x1004
#define BANDRICH_PRODUCT_1005			0x1005
#define BANDRICH_PRODUCT_1006			0x1006
#define BANDRICH_PRODUCT_1007			0x1007
#define BANDRICH_PRODUCT_1008			0x1008
#define BANDRICH_PRODUCT_1009			0x1009
#define BANDRICH_PRODUCT_100A			0x100a

#define BANDRICH_PRODUCT_100B			0x100b
#define BANDRICH_PRODUCT_100C			0x100c
#define BANDRICH_PRODUCT_100D			0x100d
#define BANDRICH_PRODUCT_100E			0x100e

#define BANDRICH_PRODUCT_100F			0x100f
#define BANDRICH_PRODUCT_1010			0x1010
#define BANDRICH_PRODUCT_1011			0x1011
#define BANDRICH_PRODUCT_1012			0x1012

#define AMOI_VENDOR_ID			0x1614
#define AMOI_PRODUCT_9508			0x0800

#define QUALCOMM_VENDOR_ID			0x05C6

#define MAXON_VENDOR_ID				0x16d8

#define TELIT_VENDOR_ID				0x1bc7
#define TELIT_PRODUCT_UC864E			0x1003

/* ZTE PRODUCTS */
#define ZTE_VENDOR_ID				0x19d2
#define ZTE_PRODUCT_MF622			0x0001
#define ZTE_PRODUCT_MF628			0x0015
#define ZTE_PRODUCT_MF626			0x0031
#define ZTE_PRODUCT_CDMA_TECH			0xfffe

#define BENQ_VENDOR_ID				0x04a5
#define BENQ_PRODUCT_H10			0x4068

#define DLINK_VENDOR_ID				0x1186
#define DLINK_PRODUCT_DWM_652			0x3e04

#ifdef CONFIG_USB_HISI_SD5115
#define DSPG_VENDOR_ID			0xdada
#define DSPG_PRODUCT_ID			0xb001
#endif	//#ifdef CONFIG_USB_HISI_SD5115

#ifdef CONFIG_ATP_USB_ADAPTOR
/* HUAWEI HGW s48571  Hardware Porting add begin:*/
extern int g_nHspaTrafficMode ;
/* HUAWEI HGW s48571  Hardware Porting add end.*/
/*add by tanghaiguo 2010.7.9 */
extern atomic_t g_iDataCardIn;
/*add by tanghaiguo 2010.7.9 */
#endif
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
static struct usb_device_id option_ids[] = {
#ifdef CONFIG_ATP_USB_ADAPTOR
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_OLD) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_FUSION) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_FUSION2) },
#endif	
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_COLT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_LIGHT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_QUAD) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_QUAD_LIGHT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_NDIS) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_NDIS_LIGHT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_NDIS_QUAD) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_RICOLA_NDIS_QUAD_LIGHT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_COBRA) },
#ifdef CONFIG_ATP_USB_ADAPTOR	
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_COBRA2) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_GTMAX36) },
#endif	
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_COBRA_BUS) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_VIPER) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_VIPER_BUS) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_GT_MAX_READY) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_FUJI_MODEM_LIGHT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_FUJI_MODEM_GT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_FUJI_MODEM_EX) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_KOI_MODEM) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_SCORPION_MODEM) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_ETNA_MODEM) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_ETNA_MODEM_LITE) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_ETNA_MODEM_GT) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_ETNA_MODEM_EX) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_ETNA_KOI_MODEM) },
	{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_GTM380_MODEM) },
	{ USB_DEVICE(QUANTA_VENDOR_ID, QUANTA_PRODUCT_Q101) },
	{ USB_DEVICE(QUANTA_VENDOR_ID, QUANTA_PRODUCT_Q111) },
	{ USB_DEVICE(QUANTA_VENDOR_ID, QUANTA_PRODUCT_GLX) },
	{ USB_DEVICE(QUANTA_VENDOR_ID, QUANTA_PRODUCT_GKE) },
	{ USB_DEVICE(QUANTA_VENDOR_ID, QUANTA_PRODUCT_GLE) },
#ifdef CONFIG_ATP_USB_ADAPTOR
	/* BEGIN: Added by c106292, 2009/7/29	PN:沙特数据卡C270*/
	{ USB_DEVICE(BANDLUXE_C270_VENDOR_ID, 0x1009) }, 
	/* END:   Added by c106292, 2009/7/29 */
	/* BEGIN: Added by c106292, 2009/7/29	PN: E1786 11.108.02.00.00不能识别问题*/
	{ USB_DEVICE(HUAWEI_VENDOR_ID, 0x140c) }, /* E1786 11.108.02.00.00 */
#ifdef CONFIG_ATP_NDIS_QMI
	{ USB_DEVICE(HUAWEI_VENDOR_ID, 0x14ac) }, /*E372*/
	{ USB_DEVICE(HUAWEI_VENDOR_ID, 0x1C05) }, /*E173*/
#endif
	{ USB_DEVICE(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_B561) },
	{ USB_DEVICE(AUDIOVOX_VENDOR_ID, AUDIOVOX_PRODUCT_AIRCARD) },
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID,NOVATELWIRELESS_PRODUCT_U740) },
#endif
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E600, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E220, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E220BIS, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1401, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1402, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1403, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1404, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1405, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1406, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1407, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1408, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1409, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140A, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140B, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140C, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140D, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140E, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140F, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1410, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1411, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1412, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1413, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1414, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1415, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1416, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1417, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1418, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1419, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141A, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141B, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141C, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141D, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141E, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E141F, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1420, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1421, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1422, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1423, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1424, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1425, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1426, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1427, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1428, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1429, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142A, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142B, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142C, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142D, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142E, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E142F, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1430, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1431, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1432, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1433, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1434, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1435, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1436, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1437, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1438, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E1439, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143A, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143B, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143C, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143D, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143E, 0xff, 0xff, 0xff) },
	{ USB_DEVICE_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E143F, 0xff, 0xff, 0xff) },
	{ USB_DEVICE(AMOI_VENDOR_ID, AMOI_PRODUCT_9508) },
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_V640) }, /* Novatel Merlin V640/XV620 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_V620) }, /* Novatel Merlin V620/S620 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_V740) }, /* Novatel Merlin EX720/V740/X720 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_V720) }, /* Novatel Merlin V720/S720/PC720 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_U730) }, /* Novatel U730/U740 (VF version) */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_U740) }, /* Novatel U740 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_U870) }, /* Novatel U870 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_XU870) }, /* Novatel Merlin XU870 HSDPA/3G */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_X950D) }, /* Novatel X950D */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EV620) }, /* Novatel EV620/ES620 CDMA/EV-DO */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_ES720) }, /* Novatel ES620/ES720/U720/USB720 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_E725) }, /* Novatel E725/E726 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_ES620) }, /* Novatel Merlin ES620 SM Bus */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EU730) }, /* Novatel EU730 and Vodafone EU740 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EU740) }, /* Novatel non-Vodafone EU740 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EU870D) }, /* Novatel EU850D/EU860D/EU870D */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_MC950D) }, /* Novatel MC930D/MC950D */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_MC727) }, /* Novatel MC727/U727/USB727 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_U727) }, /* Novatel MC727/U727/USB727 */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EVDO_FULLSPEED) }, /* Novatel EVDO product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_HSPA_FULLSPEED) }, /* Novatel HSPA product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EVDO_EMBEDDED_FULLSPEED) }, /* Novatel EVDO Embedded product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_HSPA_EMBEDDED_FULLSPEED) }, /* Novatel HSPA Embedded product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EVDO_HIGHSPEED) }, /* Novatel EVDO product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_HSPA_HIGHSPEED) }, /* Novatel HSPA product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_EVDO_EMBEDDED_HIGHSPEED) }, /* Novatel EVDO Embedded product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_HSPA_EMBEDDED_HIGHSPEED) }, /* Novatel HSPA Embedded product */
	{ USB_DEVICE(NOVATELWIRELESS_VENDOR_ID, NOVATELWIRELESS_PRODUCT_GLOBAL) }, /* Novatel Global product */

	{ USB_DEVICE(AMOI_VENDOR_ID, AMOI_PRODUCT_H01) },
	{ USB_DEVICE(AMOI_VENDOR_ID, AMOI_PRODUCT_H01A) },
	{ USB_DEVICE(AMOI_VENDOR_ID, AMOI_PRODUCT_H02) },

	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5700_MINICARD) },		/* Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card == Novatel Expedite EV620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5500_MINICARD) },		/* Dell Wireless 5500 Mobile Broadband HSDPA Mini-Card == Novatel Expedite EU740 HSDPA/3G */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5505_MINICARD) },		/* Dell Wireless 5505 Mobile Broadband HSDPA Mini-Card == Novatel Expedite EU740 HSDPA/3G */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5700_EXPRESSCARD) },		/* Dell Wireless 5700 Mobile Broadband CDMA/EVDO ExpressCard == Novatel Merlin XV620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5510_EXPRESSCARD) },		/* Dell Wireless 5510 Mobile Broadband HSDPA ExpressCard == Novatel Merlin XU870 HSDPA/3G */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5700_MINICARD_SPRINT) },	/* Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card == Novatel Expedite E720 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5700_MINICARD_TELUS) },	/* Dell Wireless 5700 Mobile Broadband CDMA/EVDO Mini-Card == Novatel Expedite ET620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5720_MINICARD_VZW) }, 	/* Dell Wireless 5720 == Novatel EV620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5720_MINICARD_SPRINT) }, 	/* Dell Wireless 5720 == Novatel EV620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5720_MINICARD_TELUS) }, 	/* Dell Wireless 5720 == Novatel EV620 CDMA/EV-DO */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5520_MINICARD_CINGULAR) },	/* Dell Wireless HSDPA 5520 == Novatel Expedite EU860D */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5520_MINICARD_GENERIC_L) },	/* Dell Wireless HSDPA 5520 */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5520_MINICARD_GENERIC_I) },	/* Dell Wireless 5520 Voda I Mobile Broadband (3G HSDPA) Minicard */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5730_MINICARD_SPRINT) },	/* Dell Wireless 5730 Mobile Broadband EVDO/HSPA Mini-Card */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5730_MINICARD_TELUS) },	/* Dell Wireless 5730 Mobile Broadband EVDO/HSPA Mini-Card */
	{ USB_DEVICE(DELL_VENDOR_ID, DELL_PRODUCT_5730_MINICARD_VZW) }, 	/* Dell Wireless 5730 Mobile Broadband EVDO/HSPA Mini-Card */
	{ USB_DEVICE(ANYDATA_VENDOR_ID, ANYDATA_PRODUCT_ADU_E100A) },	/* ADU-E100, ADU-310 */
	{ USB_DEVICE(ANYDATA_VENDOR_ID, ANYDATA_PRODUCT_ADU_500A) },
	{ USB_DEVICE(ANYDATA_VENDOR_ID, ANYDATA_PRODUCT_ADU_620UW) },
	{ USB_DEVICE(AXESSTEL_VENDOR_ID, AXESSTEL_PRODUCT_MV110H) },
	{ USB_DEVICE(ONDA_VENDOR_ID, ONDA_PRODUCT_MSA501HS) },
	{ USB_DEVICE(ONDA_VENDOR_ID, ONDA_PRODUCT_ET502HS) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0003) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0004) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0005) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0006) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0007) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0008) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0009) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000a) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000b) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000c) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000d) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000e) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x000f) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0010) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0011) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0012) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0013) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0014) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0015) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0016) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0017) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0018) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0019) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0020) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0021) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0022) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0023) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0024) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0025) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0026) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0027) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0028) },
	{ USB_DEVICE(ONDA_VENDOR_ID, 0x0029) },
	{ USB_DEVICE(ONDA_VENDOR_ID, ONDA_PRODUCT_MT503HS) },
	{ USB_DEVICE(YISO_VENDOR_ID, YISO_PRODUCT_U893) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_C100_1) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_C100_2) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1004) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1005) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1006) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1007) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1008) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1009) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100A) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100B) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100C) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100D) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100E) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_100F) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1010) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1011) },
	{ USB_DEVICE(BANDRICH_VENDOR_ID, BANDRICH_PRODUCT_1012) },
	{ USB_DEVICE(KYOCERA_VENDOR_ID, KYOCERA_PRODUCT_KPC650) },
	{ USB_DEVICE(KYOCERA_VENDOR_ID, KYOCERA_PRODUCT_KPC680) },
	{ USB_DEVICE(QUALCOMM_VENDOR_ID, 0x6000)}, /* ZTE AC8700 */
	{ USB_DEVICE(QUALCOMM_VENDOR_ID, 0x6613)}, /* Onda H600/ZTE MF330 */
	{ USB_DEVICE(MAXON_VENDOR_ID, 0x6280) }, /* BP3-USB & BP3-EXT HSDPA */
	{ USB_DEVICE(TELIT_VENDOR_ID, TELIT_PRODUCT_UC864E) },
	{ USB_DEVICE(ZTE_VENDOR_ID, ZTE_PRODUCT_MF622) },
	{ USB_DEVICE(ZTE_VENDOR_ID, ZTE_PRODUCT_MF626) },
	{ USB_DEVICE(ZTE_VENDOR_ID, ZTE_PRODUCT_MF628) },
	{ USB_DEVICE(ZTE_VENDOR_ID, ZTE_PRODUCT_CDMA_TECH) },
	{ USB_DEVICE(BENQ_VENDOR_ID, BENQ_PRODUCT_H10) },
	{ USB_DEVICE(DLINK_VENDOR_ID, DLINK_PRODUCT_DWM_652) },
	{ USB_DEVICE(0x1da5, 0x4515) }, /* BenQ H20 */
#ifdef CONFIG_USB_HISI_SD5115
	{ USB_DEVICE_AND_INTERFACE_INFO(DSPG_VENDOR_ID, DSPG_PRODUCT_ID, 0x0a, 0x00, 0x00) },
#endif	//#ifdef CONFIG_USB_HISI_SD5115
#ifdef CONFIG_ATP_NDIS_QMI
	{ USB_INTERFACE_INFO_HUAWEIOPTION(HUAWEI_VENDOR_ID, 0x02,0x02,0xFF) },//2010-12-13:W45260 兼容支持Jungo/ 高通两种方案数据卡
	{ USB_INTERFACE_INFO_HUAWEIOPTION(HUAWEI_VENDOR_ID, 0xFF,0xFF,0xFF) },//2010-12-13:W45260 兼容支持Jungo/ 高通两种方案数据卡
#endif
	{ } /* Terminating entry */
};
#else
/* <DTS2012121906958 q00186737 for download 2012/12/19 begin*/
#ifdef CONFIG_B880_PRODUCT  /*B880_PRODUCT*/
static struct usb_device_id option_ids[] = {
       { USB_DEVICE_MATCH_HUAWEI(0,HUAWEI_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(1,HUAWEI_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(2,HUAWEI_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(3,HUAWEI_VENDOR_ID)},
       /* <DTS2013020703621 q00186737 2013/02/07 begin*/
       { USB_DEVICE_MATCH_HUAWEI(4,HUAWEI_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(5,HUAWEI_VENDOR_ID)},
       /* DTS2013020703621 q00186737 2013/02/07 end> */
       {}
 };

#else /*B890 PRODUCT*/
static struct usb_device_id option_ids[] = {
       { USB_DEVICE(MDM9215_VENDOR_ID, HUAWEI_PRODUCT_B890) },
       { USB_DEVICE_MATCH_HUAWEI(0,MDM9215_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(1,MDM9215_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(2,MDM9215_VENDOR_ID)},
       { USB_DEVICE_MATCH_HUAWEI(3,MDM9215_VENDOR_ID)},
       {}
};
#endif
/* DTS2012121906958 q00186737 for download 2012/12/19 end> */
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
MODULE_DEVICE_TABLE(usb, option_ids);

static struct usb_driver option_driver = {
	.name       = "option",
	.probe      = usb_serial_probe,
	.disconnect = usb_serial_disconnect,
	.suspend    = usb_serial_suspend,
	.resume     = usb_serial_resume,
	.id_table   = option_ids,
/* <DTS2012031902908 g68080 2012-05-28 mod begin */
#ifndef CONFIG_USB_QMITTY_MODULE
    .no_dynamic_id = 	1,
#endif
/* DTS2012031902908 g68080 2012-05-28 mod end> */
};

/* The card has three separate interfaces, which the serial driver
 * recognizes separately, thus num_port=1.
 */

static struct usb_serial_driver option_1port_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"option1",
	},
	.description       = "GSM modem (1-port)",
	.usb_driver        = &option_driver,
	.id_table          = option_ids,
	.num_ports         = 1,
	.open              = option_open,
	.close             = option_close,
	.write             = option_write,
	.write_room        = option_write_room,
	.chars_in_buffer   = option_chars_in_buffer,
	.set_termios       = option_set_termios,
	.tiocmget          = option_tiocmget,
	.tiocmset          = option_tiocmset,
	.attach            = option_startup,
	.shutdown          = option_shutdown,
	.read_int_callback = option_instat_callback,
	.suspend           = option_suspend,
	.resume            = option_resume,
};

static int debug;

/* per port private data */

#define N_IN_URB 4
#define N_OUT_URB 4
#define IN_BUFLEN 4096
#define OUT_BUFLEN 4096

struct option_port_private {
	/* Input endpoints and buffer for this port */
	struct urb *in_urbs[N_IN_URB];
	u8 *in_buffer[N_IN_URB];
	/* Output endpoints and buffer for this port */
	struct urb *out_urbs[N_OUT_URB];
	u8 *out_buffer[N_OUT_URB];
	unsigned long out_busy;		/* Bit vector of URBs in use */

	/* Settings for the port */
	int rts_state;	/* Handshaking pins (outputs) */
	int dtr_state;
	int cts_state;	/* Handshaking pins (inputs) */
	int dsr_state;
	int dcd_state;
	int ri_state;

	unsigned long tx_start_time[N_OUT_URB];
};

#ifdef CONFIG_ATP_USB_ADAPTOR
extern int iUsbTtyState;
extern char ttyUSB_pid[];
extern char acHspaFactory[];
extern struct proc_dir_entry proc_root;


/* when some one writes to proc file this function will be invoked */
static int option_write_procmem (struct file *file,
                             const char *buffer,
                             unsigned long count,
                             void *data)
{
   strcpy(ttyUSB_pid, buffer);
   return count;
}


int option_read_procmem(char *buf, char **start, off_t offset,
                   int count, int *eof, void *data)
{
	int len = 0;
	if( iUsbTtyState>0 )
	{
	    len += sprintf(buf, "Usb-Serial Port Num:%d\r\nVendor:%s\r\n", iUsbTtyState,acHspaFactory);
	}
	else
	{
	    	len += sprintf(buf, "Usb-Serial Port Num:%d\r\n", iUsbTtyState);
	}
       // len += sprintf(buf, "ttyUSB status: %d\r\n", iUsbTtyState);

	*eof = 1;
	return len;
}

static void option_create_proc(void)
{

    struct proc_dir_entry* proc_pid = NULL;
#if 0    
	create_proc_read_entry("UsbModem", 0 /* default mode */,
			NULL /* parent dir */, option_read_procmem,
			NULL /* client data */);
#endif
    memset(ttyUSB_pid, 0, 64);

    proc_pid = create_proc_entry("UsbModem", S_IFREG | S_IRUSR , &proc_root ); 
    proc_pid->write_proc = option_write_procmem;
    proc_pid->read_proc = option_read_procmem;
    
}

static void option_remove_proc(void)
{
	/* no problem if it was not registered */
	remove_proc_entry("UsbModem", NULL /* parent dir */);
}

#endif
/* Functions used by new usb-serial code. */
static int __init option_init(void)
{
	int retval;
	retval = usb_serial_register(&option_1port_device);
	if (retval)
		goto failed_1port_device_register;
	retval = usb_register(&option_driver);
	if (retval)
		goto failed_driver_register;
#ifdef CONFIG_ATP_USB_ADAPTOR		
	option_create_proc();
#endif
	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
	       DRIVER_DESC "\n");

	return 0;

failed_driver_register:
	usb_serial_deregister(&option_1port_device);
failed_1port_device_register:
	return retval;
}

static void __exit option_exit(void)
{
#ifdef CONFIG_ATP_USB_ADAPTOR
	option_remove_proc();
#endif	
	usb_deregister(&option_driver);
	usb_serial_deregister(&option_1port_device);
}

module_init(option_init);
module_exit(option_exit);

static void option_set_termios(struct tty_struct *tty,
		struct usb_serial_port *port, struct ktermios *old_termios)
{
	dbg("%s", __func__);
	/* Doesn't support option setting */
	tty_termios_copy_hw(tty->termios, old_termios);
	option_send_setup(tty, port);
}

static int option_tiocmget(struct tty_struct *tty, struct file *file)
{
	struct usb_serial_port *port = tty->driver_data;
	unsigned int value;
	struct option_port_private *portdata;

	portdata = usb_get_serial_port_data(port);

	value = ((portdata->rts_state) ? TIOCM_RTS : 0) |
		((portdata->dtr_state) ? TIOCM_DTR : 0) |
		((portdata->cts_state) ? TIOCM_CTS : 0) |
		((portdata->dsr_state) ? TIOCM_DSR : 0) |
		((portdata->dcd_state) ? TIOCM_CAR : 0) |
		((portdata->ri_state) ? TIOCM_RNG : 0);

	return value;
}

static int option_tiocmset(struct tty_struct *tty, struct file *file,
			unsigned int set, unsigned int clear)
{
	struct usb_serial_port *port = tty->driver_data;
	struct option_port_private *portdata;

	portdata = usb_get_serial_port_data(port);

	/* FIXME: what locks portdata fields ? */
	if (set & TIOCM_RTS)
		portdata->rts_state = 1;
	if (set & TIOCM_DTR)
		portdata->dtr_state = 1;

	if (clear & TIOCM_RTS)
		portdata->rts_state = 0;
	if (clear & TIOCM_DTR)
		portdata->dtr_state = 0;
	return option_send_setup(tty, port);
}

/* Write */
static int option_write(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count)
{
	struct option_port_private *portdata;
	int i;
	int left, todo;
	struct urb *this_urb = NULL; /* spurious */
	int err;
#ifdef CONFIG_ATP_USB_ADAPTOR	
	int ret;
#endif	
	portdata = usb_get_serial_port_data(port);

	dbg("%s: write (%d chars)", __func__, count);

	i = 0;
	left = count;
	for (i = 0; left > 0 && i < N_OUT_URB; i++) {
		todo = left;
		if (todo > OUT_BUFLEN)
			todo = OUT_BUFLEN;

		this_urb = portdata->out_urbs[i];
		if (test_and_set_bit(i, &portdata->out_busy)) {
			if (time_before(jiffies,
					portdata->tx_start_time[i] + 10 * HZ))
				continue;
			usb_unlink_urb(this_urb);
			continue;
		}
		dbg("%s: endpoint %d buf %d", __func__,
			usb_pipeendpoint(this_urb->pipe), i);

		/* send the data */
		memcpy(this_urb->transfer_buffer, buf, todo);
		this_urb->transfer_buffer_length = todo;

		this_urb->dev = port->serial->dev;
		err = usb_submit_urb(this_urb, GFP_ATOMIC);
		if (err) {
#ifdef CONFIG_ATP_USB_ADAPTOR
            switch (err)
    		{
                /*ep stall*/
                case -EPIPE:
                    printk("\ni am in %s,%d\n", __FILE__, __LINE__);
                    ret = usb_option_clear_halt(this_urb->dev, this_urb->pipe);
                    usb_submit_urb(this_urb, GFP_ATOMIC);
                    break;
                default:

                    break;
    		}
#else
			dbg("usb_submit_urb %p (write bulk) failed "
				"(%d)", this_urb, err);
			clear_bit(i, &portdata->out_busy);
#endif
			continue;
		}
		portdata->tx_start_time[i] = jiffies;
		buf += todo;
		left -= todo;
	}

/*Add start, DTS2010081100920, l67944, 20100824*/
#ifdef CONFIG_ATP_BOARDLED 
    if(g_USB_PROBE)
    {
        led_state_change(LED_NAME_USB,LED_STATE_BLINKONCE);
    }
#endif
/*Add end, l67944, 20100824*/	
/*Start Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/
#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM96318)
	usbLastRx = jiffies;
#endif
/*End Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/	
	count -= left;
	dbg("%s: wrote (did %d)", __func__, count);
	return count;
}

static void option_indat_callback(struct urb *urb)
{
	int err;
	int endpoint;
	struct usb_serial_port *port;
	struct tty_struct *tty;
	unsigned char *data = urb->transfer_buffer;
	int status = urb->status;

	dbg("%s: %p", __func__, urb);

	endpoint = usb_pipeendpoint(urb->pipe);
	port =  urb->context;

	if (status) {
		dbg("%s: nonzero status: %d on endpoint %02x.",
		    __func__, status, endpoint);
	} else {
		tty = tty_port_tty_get(&port->port);
		if (urb->actual_length) {
			tty_buffer_request_room(tty, urb->actual_length);
			tty_insert_flip_string(tty, data, urb->actual_length);
			tty_flip_buffer_push(tty);
		} else 
			dbg("%s: empty read urb received", __func__);
		tty_kref_put(tty);

		/* Resubmit urb so we continue receiving */
		if (port->port.count && status != -ESHUTDOWN) {
			err = usb_submit_urb(urb, GFP_ATOMIC);
			if (err)
				printk(KERN_ERR "%s: resubmit read urb failed. "
					"(%d)", __func__, err);
		}
	}
	return;
}

static void option_outdat_callback(struct urb *urb)
{
	struct usb_serial_port *port;
	struct option_port_private *portdata;
	int i;

	dbg("%s", __func__);

	port =  urb->context;

	usb_serial_port_softint(port);

	portdata = usb_get_serial_port_data(port);
	for (i = 0; i < N_OUT_URB; ++i) {
		if (portdata->out_urbs[i] == urb) {
			smp_mb__before_clear_bit();
			clear_bit(i, &portdata->out_busy);
			break;
		}
	}
}

static void option_instat_callback(struct urb *urb)
{
	int err;
	int status = urb->status;
	struct usb_serial_port *port =  urb->context;
	struct option_port_private *portdata = usb_get_serial_port_data(port);
	struct usb_serial *serial = port->serial;

	dbg("%s", __func__);
	dbg("%s: urb %p port %p has data %p", __func__, urb, port, portdata);

	if (status == 0) {
		struct usb_ctrlrequest *req_pkt =
				(struct usb_ctrlrequest *)urb->transfer_buffer;

		if (!req_pkt) {
			dbg("%s: NULL req_pkt\n", __func__);
			return;
		}
		if ((req_pkt->bRequestType == 0xA1) &&
				(req_pkt->bRequest == 0x20)) {
			int old_dcd_state;
			unsigned char signals = *((unsigned char *)
					urb->transfer_buffer +
					sizeof(struct usb_ctrlrequest));

			dbg("%s: signal x%x", __func__, signals);

			old_dcd_state = portdata->dcd_state;
			portdata->cts_state = 1;
			portdata->dcd_state = ((signals & 0x01) ? 1 : 0);
			portdata->dsr_state = ((signals & 0x02) ? 1 : 0);
			portdata->ri_state = ((signals & 0x08) ? 1 : 0);

			if (old_dcd_state && !portdata->dcd_state) {
				struct tty_struct *tty =
						tty_port_tty_get(&port->port);
				if (tty && !C_CLOCAL(tty))
					tty_hangup(tty);
				tty_kref_put(tty);
			}
		} else {
			dbg("%s: type %x req %x", __func__,
				req_pkt->bRequestType, req_pkt->bRequest);
		}
	} else
	{
#ifdef CONFIG_ATP_USB_ADAPTOR
		if (urb->status == -EPIPE)
		{
			usb_option_clear_halt(urb->dev, urb->pipe);
		}

#else
		err("%s: error %d", __func__, status);
#endif
    }
	/* Resubmit urb so we continue receiving IRQ data */
	if (status != -ESHUTDOWN && status != -ENOENT) {
		urb->dev = serial->dev;
		err = usb_submit_urb(urb, GFP_ATOMIC);
#ifdef CONFIG_ATP_USB_ADAPTOR
		switch (err)
		{
            /*urb正确传输*/
            case 0:
                break;
            /*ep stall*/
            case -EPIPE:
                usb_option_clear_halt(urb->dev, urb->pipe);
                usb_submit_urb(urb, GFP_ATOMIC);
                break;
            default:
			    dbg("%s: submit irq_in urb failed %d",
				    __FUNCTION__, err);
                break;
		}
#else
		if (err)
			dbg("%s: resubmit intr urb failed. (%d)",
				__func__, err);
#endif
	}
}

static int option_write_room(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct option_port_private *portdata;
	int i;
	int data_len = 0;
	struct urb *this_urb;

	portdata = usb_get_serial_port_data(port);

	for (i = 0; i < N_OUT_URB; i++) {
		this_urb = portdata->out_urbs[i];
		if (this_urb && !test_bit(i, &portdata->out_busy))
			data_len += OUT_BUFLEN;
	}

	dbg("%s: %d", __func__, data_len);
	return data_len;
}

static int option_chars_in_buffer(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct option_port_private *portdata;
	int i;
	int data_len = 0;
	struct urb *this_urb;

	portdata = usb_get_serial_port_data(port);

	for (i = 0; i < N_OUT_URB; i++) {
		this_urb = portdata->out_urbs[i];
		/* FIXME: This locking is insufficient as this_urb may
		   go unused during the test */
		if (this_urb && test_bit(i, &portdata->out_busy))
			data_len += this_urb->transfer_buffer_length;
	}
	dbg("%s: %d", __func__, data_len);
	return data_len;
}

static int option_open(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
{
	struct option_port_private *portdata;
	struct usb_serial *serial = port->serial;
	int i, err;
	struct urb *urb;

	portdata = usb_get_serial_port_data(port);

	dbg("%s", __func__);

	/* Set some sane defaults */
	portdata->rts_state = 1;
	portdata->dtr_state = 1;

	/* Reset low level data toggle and start reading from endpoints */
	for (i = 0; i < N_IN_URB; i++) {
		urb = portdata->in_urbs[i];
		if (!urb)
			continue;
		if (urb->dev != serial->dev) {
			dbg("%s: dev %p != %p", __func__,
				urb->dev, serial->dev);
			continue;
		}

		/*
		 * make sure endpoint data toggle is synchronized with the
		 * device
		 */
#ifdef CONFIG_ATP_USB_ADAPTOR
		usb_option_clear_halt(urb->dev, urb->pipe);
#else
		usb_clear_halt(urb->dev, urb->pipe);
#endif
		err = usb_submit_urb(urb, GFP_KERNEL);
		if (err) {
			dbg("%s: submit urb %d failed (%d) %d",
				__func__, i, err,
				urb->transfer_buffer_length);
		}
	}

	/* Reset low level data toggle on out endpoints */
	for (i = 0; i < N_OUT_URB; i++) {
		urb = portdata->out_urbs[i];
		if (!urb)
			continue;
		urb->dev = serial->dev;
		/* usb_settoggle(urb->dev, usb_pipeendpoint(urb->pipe),
				usb_pipeout(urb->pipe), 0); */
#ifdef CONFIG_ATP_USB_ADAPTOR
        usb_option_clear_halt(urb->dev, urb->pipe);
#endif
	}

#if 1 //#ifdef CONFIG_ATP_SUPPORT_B560        /* <DTS2012061104394 g68080 2012-06-11 mod */
	{
		struct tty_struct *ptty;
		ptty = tty_port_tty_get(&port->port);
		if (ptty)
		{
		    ptty->low_latency = 1;            /* <DTS2012061104394 g68080 2012-06-11 mod */
			ptty->raw = 1;
			ptty->real_raw = 1;
      /* < DTS2012071700779 l00205975, begin 2012/07/17 */ 
      /* 获取tty后会增加tty的索引计数， 临时修改完后需要put回去，保持计数不变 */
      tty_kref_put(ptty); 
      /* DTS2012071700779 l00205975, end 2012/07/17> */	
		}
	}
#else
#ifdef CONFIG_ATP_USB_ADAPTOR
	usb_option_clear_halt(urb->dev, urb->pipe);
#endif    
#endif

	option_send_setup(tty, port);

	return 0;
}

static void option_close(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
{
	int i;
	struct usb_serial *serial = port->serial;
	struct option_port_private *portdata;

	dbg("%s", __func__);
#ifdef CONFIG_ATP_USB_ADAPTOR	
    msleep(2000);
    if ( serial )
    {
    }
    else
    {
        printk("!!!!!!!!VOX_DEBUG:%s:%s:%d::VHG==\n",__FILE__,__FUNCTION__,__LINE__);
        return;
    }
    if (atomic_read(&serial->kref.refcount) > 0)
    {
    }
    else
    {
        printk("!!!!!!!!VOX_DEBUG:%s:%s:%d::VHG==\n",__FILE__,__FUNCTION__,__LINE__);
        return;
    }
    
    if ( port )
    {
        portdata = usb_get_serial_port_data(port);
        if (!portdata)
        {
            printk("%s %d, portdata %p\n", __FUNCTION__, __LINE__, portdata);
            return;
        }
    }
    else
    {
        printk("!!!!!VOX_DEBUG:%s:%s:%d::VHG==\n",__FILE__,__FUNCTION__,__LINE__);
        return;
    }
#else	
	portdata = usb_get_serial_port_data(port);
#endif    
	portdata->rts_state = 0;
	portdata->dtr_state = 0;

	if (serial->dev) {
		mutex_lock(&serial->disc_mutex);
		if (!serial->disconnected)
			option_send_setup(tty, port);
		mutex_unlock(&serial->disc_mutex);

		/* Stop reading/writing urbs */
		for (i = 0; i < N_IN_URB; i++)
			usb_kill_urb(portdata->in_urbs[i]);
		for (i = 0; i < N_OUT_URB; i++)
			usb_kill_urb(portdata->out_urbs[i]);
	}
  /* < DTS2012071700779 l00205975, begin 2012/07/17 */ 
  /* 该处代码会提前释放掉tty， 导致usb_serial.c serial_close时释放逻辑分支不能执行 
  根据谁申请（在serial_open中申请)谁释放原则， 注释掉该处代码，保证serial_close正常
  释放逻辑 */
	//tty_port_tty_set(&port->port, NULL);
  /* DTS2012071700779 l00205975, end 2012/07/17> */	
}

/* Helper functions used by option_setup_urbs */
static struct urb *option_setup_urb(struct usb_serial *serial, int endpoint,
		int dir, void *ctx, char *buf, int len,
		void (*callback)(struct urb *))
{
	struct urb *urb;

	if (endpoint == -1)
		return NULL;		/* endpoint not needed */

	urb = usb_alloc_urb(0, GFP_KERNEL);		/* No ISO */
	if (urb == NULL) {
		dbg("%s: alloc for endpoint %d failed.", __func__, endpoint);
		return NULL;
	}

		/* Fill URB using supplied data. */
	usb_fill_bulk_urb(urb, serial->dev,
		      usb_sndbulkpipe(serial->dev, endpoint) | dir,
		      buf, len, callback, ctx);

	return urb;
}

/* Setup urbs */
static void option_setup_urbs(struct usb_serial *serial)
{
	int i, j;
	struct usb_serial_port *port;
	struct option_port_private *portdata;

	dbg("%s", __func__);

	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);

		/* Do indat endpoints first */
		for (j = 0; j < N_IN_URB; ++j) {
			portdata->in_urbs[j] = option_setup_urb(serial,
					port->bulk_in_endpointAddress,
					USB_DIR_IN, port,
					portdata->in_buffer[j],
					IN_BUFLEN, option_indat_callback);
		}

		/* outdat endpoints */
		for (j = 0; j < N_OUT_URB; ++j) {
			portdata->out_urbs[j] = option_setup_urb(serial,
					port->bulk_out_endpointAddress,
					USB_DIR_OUT, port,
					portdata->out_buffer[j],
					OUT_BUFLEN, option_outdat_callback);
		}
	}
}


/** send RTS/DTR state to the port.
 *
 * This is exactly the same as SET_CONTROL_LINE_STATE from the PSTN
 * CDC.
*/
static int option_send_setup(struct tty_struct *tty,
						struct usb_serial_port *port)
{
	struct usb_serial *serial = port->serial;
	struct option_port_private *portdata;
	int ifNum = serial->interface->cur_altsetting->desc.bInterfaceNumber;
	dbg("%s", __func__);

	portdata = usb_get_serial_port_data(port);

	if (tty) {
		int val = 0;
		if (portdata->dtr_state)
			val |= 0x01;
		if (portdata->rts_state)
			val |= 0x02;

		return usb_control_msg(serial->dev,
			usb_rcvctrlpipe(serial->dev, 0),
			0x22, 0x21, val, ifNum, NULL, 0, USB_CTRL_SET_TIMEOUT);
	}
	return 0;
}

static int option_startup(struct usb_serial *serial)
{
	int i, j, err;
	struct usb_serial_port *port;
	struct option_port_private *portdata;
	u8 *buffer;
#ifdef CONFIG_ATP_USB_ADAPTOR	
	int pid = 0;
    struct task_struct* ttyUsb = NULL;
#endif
	dbg("%s", __func__);

#ifdef CONFIG_ATP_USB_ADAPTOR
    if (serial->dev->descriptor.idVendor == 0x12d1)
    {
        atomic_set(&g_iDataCardIn, 1);
    }
#endif	
	/* Now setup per port private data */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		portdata = kzalloc(sizeof(*portdata), GFP_KERNEL);
		if (!portdata) {
			dbg("%s: kmalloc for option_port_private (%d) failed!.",
					__func__, i);
			return 1;
		}
#ifdef CONFIG_ATP_USB_ADAPTOR		
		memset(portdata, 0, sizeof(*portdata));
#endif
		for (j = 0; j < N_IN_URB; j++) {
			buffer = (u8 *)__get_free_page(GFP_KERNEL);
			if (!buffer)
				goto bail_out_error;
			portdata->in_buffer[j] = buffer;
		}

		for (j = 0; j < N_OUT_URB; j++) {
			buffer = kmalloc(OUT_BUFLEN, GFP_KERNEL);
			if (!buffer)
				goto bail_out_error2;
			portdata->out_buffer[j] = buffer;
		}

		usb_set_serial_port_data(port, portdata);

		if (!port->interrupt_in_urb)
			continue;
		err = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
#ifdef CONFIG_ATP_USB_ADAPTOR
		switch (err)
		{
            /*urb正确传输*/
            case 0:
                break;
            /*ep stall*/
            case -EPIPE:
                usb_option_clear_halt(port->interrupt_in_urb->dev, port->interrupt_in_urb->pipe);
                break;
            default:
			    dbg("%s: submit irq_in urb failed %d",
				    __FUNCTION__, err);
                break;
		}

#else
		if (err)
			dbg("%s: submit irq_in urb failed %d",
				__func__, err);
#endif
	}
	option_setup_urbs(serial);
#ifdef CONFIG_ATP_USB_ADAPTOR    
    ++iUsbTtyState;

    if (iUsbTtyState == 2)
    {
        pid = simple_strtol(ttyUSB_pid, NULL, 10); 

        if ( pid != 0 )
        {
            ttyUsb = find_task_by_vpid(pid);
            if(ttyUsb)
            {
                send_sig(SIGUSR1, ttyUsb, 0);
            }
        }
    }
#endif

 
	return 0;

bail_out_error2:
	for (j = 0; j < N_OUT_URB; j++)
		kfree(portdata->out_buffer[j]);
bail_out_error:
	for (j = 0; j < N_IN_URB; j++)
		if (portdata->in_buffer[j])
			free_page((unsigned long)portdata->in_buffer[j]);
	kfree(portdata);
	return 1;
}

static void stop_read_write_urbs(struct usb_serial *serial)
{
	int i, j;
	struct usb_serial_port *port;
	struct option_port_private *portdata;

	/* Stop reading/writing urbs */
	for (i = 0; i < serial->num_ports; ++i) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);
		for (j = 0; j < N_IN_URB; j++)
			usb_kill_urb(portdata->in_urbs[j]);
		for (j = 0; j < N_OUT_URB; j++)
			usb_kill_urb(portdata->out_urbs[j]);
	}
}

static void option_shutdown(struct usb_serial *serial)
{
	int i, j;
	struct usb_serial_port *port;
	struct option_port_private *portdata;

	dbg("%s", __func__);

	stop_read_write_urbs(serial);

	/* Now free them */
	for (i = 0; i < serial->num_ports; ++i) {
		port = serial->port[i];
        /* 如果在count不等于0的时候free，应用层close时会导致失败 */
#ifdef CONFIG_ATP_USB_ADAPTOR		
        if (port->port.count != 0)
        {
            continue;
        }
#endif		
		portdata = usb_get_serial_port_data(port);

		for (j = 0; j < N_IN_URB; j++) {
			if (portdata->in_urbs[j]) {
				usb_free_urb(portdata->in_urbs[j]);
				free_page((unsigned long)
					portdata->in_buffer[j]);
				portdata->in_urbs[j] = NULL;
			}
		}
		for (j = 0; j < N_OUT_URB; j++) {
			if (portdata->out_urbs[j]) {
				usb_free_urb(portdata->out_urbs[j]);
				kfree(portdata->out_buffer[j]);
				portdata->out_urbs[j] = NULL;
			}
		}
	}

	/* Now free per port private data */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
        /* 如果在count不等于0的时候free，应用层close时会导致失败 */
#ifdef CONFIG_ATP_USB_ADAPTOR		
        if (port->port.count != 0)
        {
            continue;
        }        
		kfree(usb_get_serial_port_data(port));
    	usb_set_serial_port_data(port, NULL);
        portdata = usb_get_serial_port_data(port);
        serial->port[i] = NULL;
#else		
		kfree(usb_get_serial_port_data(port));
#endif		
	}
}

static int option_suspend(struct usb_serial *serial, pm_message_t message)
{
	dbg("%s entered", __func__);
	stop_read_write_urbs(serial);

	return 0;
}

static int option_resume(struct usb_serial *serial)
{
	int err, i, j;
	struct usb_serial_port *port;
	struct urb *urb;
	struct option_port_private *portdata;

	dbg("%s entered", __func__);
	/* get the interrupt URBs resubmitted unconditionally */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		if (!port->interrupt_in_urb) {
			dbg("%s: No interrupt URB for port %d\n", __func__, i);
			continue;
		}
		port->interrupt_in_urb->dev = serial->dev;
		err = usb_submit_urb(port->interrupt_in_urb, GFP_NOIO);
		dbg("Submitted interrupt URB for port %d (result %d)", i, err);
		if (err < 0) {
			err("%s: Error %d for interrupt URB of port%d",
				 __func__, err, i);
			return err;
		}
	}

	for (i = 0; i < serial->num_ports; i++) {
		/* walk all ports */
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);
		mutex_lock(&port->mutex);

		/* skip closed ports */
		if (!port->port.count) {
			mutex_unlock(&port->mutex);
			continue;
		}

		for (j = 0; j < N_IN_URB; j++) {
			urb = portdata->in_urbs[j];
			err = usb_submit_urb(urb, GFP_NOIO);
			if (err < 0) {
				mutex_unlock(&port->mutex);
				err("%s: Error %d for bulk URB %d",
					 __func__, err, i);
				return err;
			}
		}
		mutex_unlock(&port->mutex);
	}
	return 0;
}

#ifdef CONFIG_ATP_USB_ADAPTOR
static int usb_option_clear_halt(struct usb_device *dev, int pipe)
{
    int result;
	int endp = usb_pipeendpoint(pipe);
	
	if (usb_pipein (pipe))
		endp |= USB_DIR_IN;

	/* we don't care if it wasn't halted first. in fact some devices
	 * (like some ibmcam model 1 units) seem to expect hosts to make
	 * this request for iso endpoints, which can't halt!
	 */
    
	result = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
		USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT,
		USB_ENDPOINT_HALT, endp, NULL, 0,
		HZ * USB_CTRL_SET_TIMEOUT);

        //printk("i am in %s,%d,return value is %d",__FILE__, __LINE__, result);
	/* don't un-halt or force to DATA0 except on success */
	if (result < 0)
		return result;

	/* NOTE:  seems like Microsoft and Apple don't bother verifying
	 * the clear "took", so some devices could lock up if you check...
	 * such as the Hagiwara FlashGate DUAL.  So we won't bother.
	 *
	 * NOTE:  make sure the logic here doesn't diverge much from
	 * the copy in usb-storage, for as long as we need two copies.
	 */

	/* toggle was reset by the clear, then ep was reactivated */
	usb_settoggle(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);
	//usb_endpoint_running(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

	return 0;
}
#endif
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug messages");
