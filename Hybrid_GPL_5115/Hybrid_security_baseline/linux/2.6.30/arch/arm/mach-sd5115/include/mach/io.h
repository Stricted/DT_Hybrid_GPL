#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffffffff

#define __io(a)		__typesafe_io(a)
#define __mem_pci(a)	(a)

#define IO_ADDRESS(x) (0xF0000000 | (((x)&0xf0000000) >> 4) | ((x)&0x0FFFFFFF))

#endif
