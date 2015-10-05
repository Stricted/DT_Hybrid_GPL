

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Physical DRAM offset.
 */
#ifdef CONFIG_HISI_SD5115_AF_FPGA
#define PHYS_OFFSET	UL(0x80000000)
#else
#define PHYS_OFFSET	UL(0x80e00000)
#endif

#endif /* __ASM_ARCH_MEMORY_H */
