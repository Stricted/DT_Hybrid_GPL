/*
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __ASM_ARCH_MX5_IMX_REGS_H__
#define __ASM_ARCH_MX5_IMX_REGS_H__

#if defined(CONFIG_MX51)
#define IRAM_BASE_ADDR		0x1FFE0000	/* internal ram */
#define IPU_CTRL_BASE_ADDR	0x40000000
#define SPBA0_BASE_ADDR         0x70000000
#define AIPS1_BASE_ADDR         0x73F00000
#define AIPS2_BASE_ADDR         0x83F00000
#define CSD0_BASE_ADDR          0x90000000
#define CSD1_BASE_ADDR          0xA0000000
#define NFC_BASE_ADDR_AXI       0xCFFF0000
#elif defined(CONFIG_MX53)
#define IPU_CTRL_BASE_ADDR      0x18000000
#define SPBA0_BASE_ADDR         0x50000000
#define AIPS1_BASE_ADDR         0x53F00000
#define AIPS2_BASE_ADDR         0x63F00000
#define CSD0_BASE_ADDR          0x70000000
#define CSD1_BASE_ADDR          0xB0000000
#define NFC_BASE_ADDR_AXI       0xF7FF0000
#define IRAM_BASE_ADDR          0xF8000000
#else
#error "CPU_TYPE not defined"
#endif

#define IRAM_SIZE		0x00020000	/* 128 KB */

/*
 * SPBA global module enabled #0
 */
#define MMC_SDHC1_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00004000)
#define MMC_SDHC2_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00008000)
#define UART3_BASE_ADDR 	(SPBA0_BASE_ADDR + 0x0000C000)
#define CSPI1_BASE_ADDR 	(SPBA0_BASE_ADDR + 0x00010000)
#define SSI2_BASE_ADDR		(SPBA0_BASE_ADDR + 0x00014000)
#define MMC_SDHC3_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00020000)
#define MMC_SDHC4_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00024000)
#define SPDIF_BASE_ADDR		(SPBA0_BASE_ADDR + 0x00028000)
#define ATA_DMA_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00030000)
#define SLIM_DMA_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00034000)
#define HSI2C_DMA_BASE_ADDR	(SPBA0_BASE_ADDR + 0x00038000)
#define SPBA_CTRL_BASE_ADDR	(SPBA0_BASE_ADDR + 0x0003C000)

/*
 * AIPS 1
 */
#define OTG_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00080000)
#define GPIO1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00084000)
#define GPIO2_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00088000)
#define GPIO3_BASE_ADDR		(AIPS1_BASE_ADDR + 0x0008C000)
#define GPIO4_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00090000)
#define KPP_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00094000)
#define WDOG1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x00098000)
#define WDOG2_BASE_ADDR		(AIPS1_BASE_ADDR + 0x0009C000)
#define GPT1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000A0000)
#define SRTC_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000A4000)
#define IOMUXC_BASE_ADDR	(AIPS1_BASE_ADDR + 0x000A8000)
#define EPIT1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000AC000)
#define EPIT2_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000B0000)
#define PWM1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000B4000)
#define PWM2_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000B8000)
#define UART1_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000BC000)
#define UART2_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000C0000)
#define SRC_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000D0000)
#define CCM_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000D4000)
#define GPC_BASE_ADDR		(AIPS1_BASE_ADDR + 0x000D8000)

#if defined(CONFIG_MX53)
#define GPIO5_BASE_ADDR         (AIPS1_BASE_ADDR + 0x000DC000)
#define GPIO6_BASE_ADDR         (AIPS1_BASE_ADDR + 0x000E0000)
#define GPIO7_BASE_ADDR         (AIPS1_BASE_ADDR + 0x000E4000)
#endif
/*
 * AIPS 2
 */
#define PLL1_BASE_ADDR		(AIPS2_BASE_ADDR + 0x00080000)
#define PLL2_BASE_ADDR		(AIPS2_BASE_ADDR + 0x00084000)
#define PLL3_BASE_ADDR		(AIPS2_BASE_ADDR + 0x00088000)
#define AHBMAX_BASE_ADDR	(AIPS2_BASE_ADDR + 0x00094000)
#define IIM_BASE_ADDR		(AIPS2_BASE_ADDR + 0x00098000)
#define CSU_BASE_ADDR		(AIPS2_BASE_ADDR + 0x0009C000)
#define ARM_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000A0000)
#define OWIRE_BASE_ADDR 	(AIPS2_BASE_ADDR + 0x000A4000)
#define FIRI_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000A8000)
#define CSPI2_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000AC000)
#define SDMA_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000B0000)
#define SCC_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000B4000)
#define ROMCP_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000B8000)
#define RTIC_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000BC000)
#define CSPI3_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000C0000)
#define I2C2_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000C4000)
#define I2C1_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000C8000)
#define SSI1_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000CC000)
#define AUDMUX_BASE_ADDR	(AIPS2_BASE_ADDR + 0x000D0000)
#define M4IF_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000D8000)
#define ESDCTL_BASE_ADDR	(AIPS2_BASE_ADDR + 0x000D9000)
#define WEIM_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000DA000)
#define NFC_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000DB000)
#define EMI_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000DBF00)
#define MIPI_HSC_BASE_ADDR	(AIPS2_BASE_ADDR + 0x000DC000)
#define ATA_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000E0000)
#define SIM_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000E4000)
#define SSI3BASE_ADDR		(AIPS2_BASE_ADDR + 0x000E8000)
#define FEC_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000EC000)
#define TVE_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000F0000)
#define VPU_BASE_ADDR		(AIPS2_BASE_ADDR + 0x000F4000)
#define SAHARA_BASE_ADDR	(AIPS2_BASE_ADDR + 0x000F8000)

/*
 * Number of GPIO pins per port
 */
#define GPIO_NUM_PIN            32

#define IIM_SREV	0x24
#define ROM_SI_REV	0x48

#define NFC_BUF_SIZE	0x1000

/* M4IF */
#define M4IF_FBPM0	0x40
#define M4IF_FIDBP	0x48

/* Assuming 24MHz input clock with doubler ON */
/*                            MFI         PDF */
#define DP_OP_850	((8 << 4) + ((1 - 1)  << 0))
#define DP_MFD_850	(48 - 1)
#define DP_MFN_850	41

#define DP_OP_800	((8 << 4) + ((1 - 1)  << 0))
#define DP_MFD_800	(3 - 1)
#define DP_MFN_800	1

#define DP_OP_700	((7 << 4) + ((1 - 1)  << 0))
#define DP_MFD_700	(24 - 1)
#define DP_MFN_700	7

#define DP_OP_665	((6 << 4) + ((1 - 1)  << 0))
#define DP_MFD_665	(96 - 1)
#define DP_MFN_665	89

#define DP_OP_532	((5 << 4) + ((1 - 1)  << 0))
#define DP_MFD_532	(24 - 1)
#define DP_MFN_532	13

#define DP_OP_400	((8 << 4) + ((2 - 1)  << 0))
#define DP_MFD_400	(3 - 1)
#define DP_MFN_400	1

#define DP_OP_216	((6 << 4) + ((3 - 1)  << 0))
#define DP_MFD_216	(4 - 1)
#define DP_MFN_216	3

#define CHIP_REV_1_0            0x10
#define CHIP_REV_1_1            0x11
#define CHIP_REV_2_0            0x20
#define CHIP_REV_2_5		0x25
#define CHIP_REV_3_0            0x30

#define BOARD_REV_1_0           0x0
#define BOARD_REV_2_0           0x1

#define IMX_IIM_BASE            (IIM_BASE_ADDR)

#if !(defined(__KERNEL_STRICT_NAMES) || defined(__ASSEMBLY__))
#include <asm/types.h>

extern void imx_get_mac_from_fuse(unsigned char *mac);

#define __REG(x)	(*((volatile u32 *)(x)))
#define __REG16(x)	(*((volatile u16 *)(x)))
#define __REG8(x)	(*((volatile u8 *)(x)))

struct clkctl {
	u32	ccr;
	u32	ccdr;
	u32	csr;
	u32	ccsr;
	u32	cacrr;
	u32	cbcdr;
	u32	cbcmr;
	u32	cscmr1;
	u32	cscmr2;
	u32	cscdr1;
	u32	cs1cdr;
	u32	cs2cdr;
	u32	cdcdr;
	u32	chsccdr;
	u32	cscdr2;
	u32	cscdr3;
	u32	cscdr4;
	u32	cwdr;
	u32	cdhipr;
	u32	cdcr;
	u32	ctor;
	u32	clpcr;
	u32	cisr;
	u32	cimr;
	u32	ccosr;
	u32	cgpr;
	u32	ccgr0;
	u32	ccgr1;
	u32	ccgr2;
	u32	ccgr3;
	u32	ccgr4;
	u32	ccgr5;
	u32	ccgr6;
	u32	cmeor;
};

/* WEIM registers */
struct weim {
	u32	csgcr1;
	u32	csgcr2;
	u32	csrcr1;
	u32	csrcr2;
	u32	cswcr1;
	u32	cswcr2;
};

/* GPIO Registers */
struct gpio_regs {
	u32	gpio_dr;
	u32	gpio_dir;
	u32	gpio_psr;
};

/* System Reset Controller (SRC) */
struct src {
	u32	scr;
	u32	sbmr;
	u32	srsr;
	u32	reserved1[2];
	u32	sisr;
	u32	simr;
};

/* CSPI registers */
struct cspi_regs {
	u32 rxdata;
	u32 txdata;
	u32 ctrl;
	u32 cfg;
	u32 intr;
	u32 dma;
	u32 stat;
	u32 period;
};

struct iim_regs {
	u32	stat;
	u32	statm;
	u32     err;
	u32	emask;
	u32	fctl;
	u32	ua;
	u32	la;
	u32	sdat;
	u32	prev;
	u32	srev;
	u32	preg_p;
	u32	scs0;
	u32	scs1;
	u32	scs2;
	u32	scs3;
	u32	res0[0x1f1];
	struct fuse_bank {
		u32	fuse_regs[0x20];
		u32	fuse_rsvd[0xe0];
	} bank[4];
};

struct fuse_bank1_regs {
	u32	fuse0_8[9];
	u32	mac_addr[6];
	u32	fuse15_31[0x11];
};

#endif /* __ASSEMBLER__*/

#endif				/* __ASM_ARCH_MX5_IMX_REGS_H__ */
