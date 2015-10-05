#ifndef __PCIE_H__
#define __PCIE_H__

#define PCIE_SYS_BASE_PHYS 		(0x10100000) //SYSCTRL
#define PCIE0_BASE_ADDR_PHYS	(0x40000000) //MEM ø’º‰
#define PCIE1_BASE_ADDR_PHYS	(0x58000000) //MEM ø’º‰
#define PCIE0_MEMIO_BASE	    (0x50000000)//≈‰÷√ø’º‰
#define PCIE1_MEMIO_BASE	    (0x68000000)//≈‰÷√ø’º‰
#define PCIE_BASE_ADDR_SIZE		(0x1000000)

#define DBI_BASE_ADDR_0			(0x10A00000) //IOø’º‰ 64k
#define DBI_BASE_ADDR_1			(0x10A02000) //IOø’º‰ 64k

#define PCIE_PHY_VALUE0         (0x05605001)
#define PCIE_PHY_VALUE1         (0x20050006)

#define PERI_CRG37			(0x00D4)
#define PERI_CRG37_PCIE0_SRST_REQ	(1U << 0)
#define PERI_CRG37_PCIE0_RST_REQ	(1U << 1)
#define PERI_CRG37_X2X_PCIE0_SRST_REQ	(1U << 2)
#define PERI_CRG37_X2X_PCIES_SRST_REQ	(1U << 6)
#define PERI_CRG37_PCIE0_CKEN		(1U << 8)
#define PERI_CRG37_PCIE_PHY_CKEN	(1U << 10)
#define PERI_CRG37_X2X_PCIES_CKEN	(1U << 11)
#define PERI_CRG37_PCIE_CLK_SEL		(1U << 16)
#define PERI_CRG37_PCIE_SLOT0_SRST_REQ	(1U << 24)
#define PERI_CRG37_PCIE_SLOT_CKEN	(1U << 26)

#define PERI_PCIE0		(0xBC)
#define PERI_PCIE1      (0xC0)
#define PERI_PCIE6		(0xd4)
#define PERI_PCIE7		(0xd8)
#define PERI_PCIE8		(0xDC)
#define PERI_PCIE9		(0xE0)

#define PERI_PCIE1_0		(0xEC)
#define PERI_PCIE1_1        (0xF0)
#define PERI_PCIE1_6		(0x104)
#define PERI_PCIE1_7		(0x108)
#define PERI_PCIE1_8		(0x10C)
#define PERI_PCIE1_9		(0x110)

#define pcie0_slv_device_type	(28)
#define pcie1_slv_device_type	(28)

#define pcie0_app_ltssm_enable	(11)
#define pcie1_app_ltssm_enable	(11)

#define PERI_PCIE_STAT0		(0x18)
#define pciex_link_up		(5)
#define PERI_PCIE_STAT1		(0x28)
#define IRQ_BASE		(32)

#define PCIE0_IRQ_INTA		(59 + IRQ_BASE)
#define PCIE0_IRQ_INTB		(60 + IRQ_BASE)
#define PCIE0_IRQ_INTC		(61 + IRQ_BASE)
#define PCIE0_IRQ_INTD		(62 + IRQ_BASE)

#define PCIE1_IRQ_INTA		(63 + IRQ_BASE)
#define PCIE1_IRQ_INTB		(64 + IRQ_BASE)
#define PCIE1_IRQ_INTC		(65 + IRQ_BASE)
#define PCIE1_IRQ_INTD		(66 + IRQ_BASE)

#define PCIE_INTA_PIN		(1)
#define PCIE_INTB_PIN		(2)
#define PCIE_INTC_PIN		(3)
#define PCIE_INTD_PIN		(4)

struct pcie_iatu{
	unsigned int viewport;          /* iATU Viewport Register        */
	unsigned int region_ctrl_1;     /* Region Control 1 Register     */
	unsigned int region_ctrl_2;     /* Region Control 2 Register     */
	unsigned int lbar;              /* Lower Base Address Register   */
	unsigned int ubar;              /* Upper Base Address Register   */
	unsigned int lar;               /* Limit Address Register        */
	unsigned int ltar;		/* Lower Target Address Register */
	unsigned int utar;              /* Upper Target Address Register */
};

#define PCIE_DEBUG_LEVEL_REG    (1)
#define PCIE_DEBUG_LEVEL_FUNC   (2)
#define PCIE_DEBUG_LEVEL_MODULE (3)

#define PCIE_DEBUG_LEVEL PCIE_DEBUG_LEVEL_MODULE

#define PCIE_DEBUG
#ifdef PCIE_DEBUG
#define pcie_debug(level,str,arg...)\
do{\
    if((level)<= PCIE_DEBUG_LEVEL){\
        printk(KERN_DEBUG "%s->%d," str "\n" ,__func__,__LINE__,##arg);\
    }\
}while(0)
#else
#define pcie_debug(level,str,arg...)
#endif

#define pcie_assert(con)\
do{\
    if(!(con)){\
        printk(KERN_ERR "%s->%d,assert fail!\n",__func__,__LINE__);\
    }\
}while(0)

#define pcie_error(str,arg...)\
do{\
    printk(KERN_ERR "%s->%d" str "\n",__func__,__LINE__,##arg);\
}while(0)

#endif
