/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *
 * Authors:
 *	Aneesh V <aneesh@ti.com>
 *
 * Derived from OMAP3 work by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _OMAP4_H_
#define _OMAP4_H_

#if !(defined(__KERNEL_STRICT_NAMES) || defined(__ASSEMBLY__))
#include <asm/types.h>
#endif /* !(__KERNEL_STRICT_NAMES || __ASSEMBLY__) */

/*
 * L4 Peripherals - L4 Wakeup and L4 Core now
 */
#define OMAP44XX_L4_CORE_BASE	0x4A000000
#define OMAP44XX_L4_WKUP_BASE	0x4A300000
#define OMAP44XX_L4_PER_BASE	0x48000000

#define OMAP44XX_DRAM_ADDR_SPACE_START	0x80000000
#define OMAP44XX_DRAM_ADDR_SPACE_END	0xD0000000


/* CONTROL */
#define CTRL_BASE		(OMAP44XX_L4_CORE_BASE + 0x2000)
#define CONTROL_PADCONF_CORE	(OMAP44XX_L4_CORE_BASE + 0x100000)
#define CONTROL_PADCONF_WKUP	(OMAP44XX_L4_CORE_BASE + 0x31E000)

/* UART */
#define UART1_BASE		(OMAP44XX_L4_PER_BASE + 0x6a000)
#define UART2_BASE		(OMAP44XX_L4_PER_BASE + 0x6c000)
#define UART3_BASE		(OMAP44XX_L4_PER_BASE + 0x20000)

/* General Purpose Timers */
#define GPT1_BASE		(OMAP44XX_L4_WKUP_BASE + 0x18000)
#define GPT2_BASE		(OMAP44XX_L4_PER_BASE  + 0x32000)
#define GPT3_BASE		(OMAP44XX_L4_PER_BASE  + 0x34000)

/* Watchdog Timer2 - MPU watchdog */
#define WDT2_BASE		(OMAP44XX_L4_WKUP_BASE + 0x14000)

/* 32KTIMER */
#define SYNC_32KTIMER_BASE	(OMAP44XX_L4_WKUP_BASE + 0x4000)

/* GPMC */
#define OMAP44XX_GPMC_BASE	0x50000000

/* DMM */
#define OMAP44XX_DMM_BASE		0x4E000000
#define DMM_LISA_MAP_BASE		(OMAP44XX_DMM_BASE + 0x40)
#define DMM_LISA_MAP_SYS_SIZE_MASK	(7 << 20)
#define DMM_LISA_MAP_SYS_SIZE_SHIFT	20
#define DMM_LISA_MAP_SYS_ADDR_MASK	(0xFF << 24)
/*
 * Hardware Register Details
 */

/* Watchdog Timer */
#define WD_UNLOCK1		0xAAAA
#define WD_UNLOCK2		0x5555

/* GP Timer */
#define TCLR_ST			(0x1 << 0)
#define TCLR_AR			(0x1 << 1)
#define TCLR_PRE		(0x1 << 5)

/*
 * PRCM
 */

/* PRM */
#define PRM_BASE		0x4A306000
#define PRM_DEVICE_BASE		(PRM_BASE + 0x1B00)

#define PRM_RSTCTRL		PRM_DEVICE_BASE
#define PRM_RSTCTRL_RESET	0x01

#ifndef __ASSEMBLY__

struct s32ktimer {
	unsigned char res[0x10];
	unsigned int s32k_cr;	/* 0x10 */
};

#endif /* __ASSEMBLY__ */

/*
 * Non-secure SRAM Addresses
 * Non-secure RAM starts at 0x40300000 for GP devices. But we keep SRAM_BASE
 * at 0x40304000(EMU base) so that our code works for both EMU and GP
 */
#define NON_SECURE_SRAM_START	0x40304000
#define NON_SECURE_SRAM_END	0x4030E000	/* Not inclusive */
/* base address for indirect vectors (internal boot mode) */
#define SRAM_ROM_VECT_BASE	0x4030D000
/* Temporary SRAM stack used while low level init is done */
#define LOW_LEVEL_SRAM_STACK	NON_SECURE_SRAM_END

/*
 * OMAP4 real hardware:
 * TODO: Change this to the IDCODE in the hw regsiter
 */
#define CPU_OMAP4430_ES10	1
#define CPU_OMAP4430_ES20	2

#endif
