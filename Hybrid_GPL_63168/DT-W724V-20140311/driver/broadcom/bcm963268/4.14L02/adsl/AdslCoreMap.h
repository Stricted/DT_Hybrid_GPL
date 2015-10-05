/* 
* <:copyright-BRCM:2002:proprietary:standard
* 
*    Copyright (c) 2002 Broadcom Corporation
*    All Rights Reserved
* 
*  This program is the proprietary software of Broadcom Corporation and/or its
*  licensors, and may only be used, duplicated, modified or distributed pursuant
*  to the terms and conditions of a separate, written license agreement executed
*  between you and Broadcom (an "Authorized License").  Except as set forth in
*  an Authorized License, Broadcom grants no license (express or implied), right
*  to use, or waiver of any kind with respect to the Software, and Broadcom
*  expressly reserves all rights in and to the Software and all intellectual
*  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
*  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
*  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
* 
*  Except as expressly set forth in the Authorized License,
* 
*  1. This program, including its structure, sequence and organization,
*     constitutes the valuable trade secrets of Broadcom, and you shall use
*     all reasonable efforts to protect the confidentiality thereof, and to
*     use this information only in connection with your use of Broadcom
*     integrated circuit products.
* 
*  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
*     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
*     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
*     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
*     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
*     PERFORMANCE OF THE SOFTWARE.
* 
*  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
*     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
*     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
*     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
*     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
*     LIMITED REMEDY.
* :>
*/
/****************************************************************************
 *
 * AdslCoreMap.h -- Definitions for ADSL core hardware
 *
 * Description:
 *	Definitions for ADSL core hardware
 *
 *
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.1 $
 *
 * $Id: AdslCoreMap.h,v 1.1 2004/04/08 21:24:49 ilyas Exp $
 *
 * $Log: AdslCoreMap.h,v $
 * Revision 1.1  2004/04/08 21:24:49  ilyas
 * Initial CVS checkin. Version A2p014
 *
 ****************************************************************************/

#if defined(__KERNEL__)
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33)
#include <generated/autoconf.h>
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21))
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif
#endif
#include "softdsl/AdslCoreDefs.h"

#if defined(CONFIG_BCM96348) || defined(CONFIG_BCM96358)
#define XDSL_ENUM_BASE		0xFFFE3000
#elif defined(CONFIG_BCM96338)
#define XDSL_ENUM_BASE		0xFFFE1000
#elif defined(CONFIG_BCM96368)
#undef XDSL_ENUM_BASE
#define XDSL_ENUM_BASE		0xB0F56000	/* DHIF */
#elif defined(CONFIG_BCM96362) || defined(CONFIG_BCM96328)
#define XDSL_ENUM_BASE		0xB0D56000	/* DHIF */
#elif defined(CONFIG_BCM963268)
#define XDSL_ENUM_BASE		0xB0756000	/* DHIF */
#elif defined(CONFIG_BCM96318)
#define XDSL_ENUM_BASE		0xB0156000	/* DHIF */
#else
#error  No definition for XDSL_ENUM_BASE
#endif

#ifndef HOST_LMEM_BASE
 #if defined(CONFIG_BCM96368)
 #define HOST_LMEM_BASE          0xB0F80000
 #elif defined(CONFIG_BCM96362) || defined(CONFIG_BCM96328)
 #define HOST_LMEM_BASE          0xB0D80000
 #elif defined(CONFIG_BCM963268)
 #define HOST_LMEM_BASE          0xB0780000
 #elif defined(CONFIG_BCM96318)
 #define HOST_LMEM_BASE          0xB0180000
 #else
 #define HOST_LMEM_BASE          0xFFF00000
 #endif
#endif

#if defined(CONFIG_BCM96358)
#define ATM_REG_BASE		0xFFFE2000
#elif defined(CONFIG_BCM96348) || defined(CONFIG_BCM96338)
#define ATM_REG_BASE		0xFFFE4000
#endif

#if defined(CONFIG_BCM96348) || defined(CONFIG_BCM96338) || defined(CONFIG_BCM96358)

#ifndef ADSL_ENUM_BASE
#define ADSL_ENUM_BASE	XDSL_ENUM_BASE	/* Prevent compile error in ATM driver under Linux3.xx releases */
#endif

#define RCVINT_BIT              0x00010000
#define XMTINT_BIT              0x01000000
#define MSGINT_BIT             0x00000020
#define MSGINT_MASK_BIT	MSGINT_BIT

/* Backplane Enumeration Space Addresses */
#define ADSL_CTL                (0x000 / 4)
#define ADSL_STATUS             (0x004 / 4)
#define ADSL_INTMASK_I          (0x024 / 4)
#define ADSL_INTMASK_F          (0x02c / 4)
#define ADSL_INTSTATUS_I        (0x020 / 4)
#define ADSL_INTSTATUS_F        (0x028 / 4)
#define ADSL_HOSTMESSAGE        (0x300 / 4)

/* DMA Regs, offset from backplane enumeration space address */

#define XMT_CTL_INTR            (0x200 / 4)
#define XMT_ADDR_INTR           (0x204 / 4)
#define XMT_PTR_INTR            (0x208 / 4)
#define XMT_STATUS_INTR         (0x20c / 4)
#define RCV_CTL_INTR            (0x210 / 4)
#define RCV_ADDR_INTR           (0x214 / 4)
#define RCV_PTR_INTR            (0x218 / 4)
#define RCV_STATUS_INTR         (0x21c / 4)
#define XMT_CTL_FAST            (0x220 / 4)
#define XMT_ADDR_FAST           (0x224 / 4)
#define XMT_PTR_FAST            (0x228 / 4)
#define XMT_STATUS_FAST         (0x22c / 4)
#define RCV_CTL_FAST            (0x230 / 4)
#define RCV_ADDR_FAST           (0x234 / 4)
#define RCV_PTR_FAST            (0x238 / 4)
#define RCV_STATUS_FAST         (0x23c / 4)

#define ADSL_CORE_RESET         (0xf98 / 4)
#define ADSL_MIPS_RESET         (0x000 / 4)

#define US_DESCR_TABLE_ADDR		0xA0004000
#define DS_DESCR_TABLE_ADDR		0xA0005000
#define DS_STATUS_SIZE			16

#define HDMA_INIT_FLAG_ADDR		0xA0006000
#define US_CURR_DSCR_ADDR		0xA0006004
#define US_LAST_DSCR_ADDR		0xA0006008
#define DS_CURR_DSCR_ADDR		0xA000600c
#define DS_LAST_DSCR_ADDR		0xA0006010

#else

/* Common for: CONFIG_BCM96368, CONFIG_BCM96362, CONFIG_BCM96328, CONFIG_BCM963268 */
#define MSGINT_BIT			0x00000100
#define MSGINT_MASK_BIT		0x00000001

#define ADSL_Core2HostMsg		(0x034 / 4)
#define ADSL_INTMASK_I		(0x03c / 4)
#define ADSL_INTSTATUS_I		ADSL_INTMASK_I
#define ADSL_HOSTMESSAGE	(0x038 / 4)

#endif /* defined(CONFIG_BCM96348) || defined(CONFIG_BCM96338) || defined(CONFIG_BCM96358) */
