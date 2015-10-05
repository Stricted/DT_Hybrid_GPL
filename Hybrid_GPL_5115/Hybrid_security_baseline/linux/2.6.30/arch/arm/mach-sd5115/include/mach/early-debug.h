/* linux/include/asm-arm/arch-hi3510_v100_m01/early-debug.h
*
* Copyright (c) 2006 Hisilicon Co., Ltd. 
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*/


#include <mach/system.h>

#ifndef __ASM_ARCH_EARLY_DEBUG_H
#define __ASM_ARCH_EARLY_DEBUG_H

#if (CONFIG_HISILICON_KCONSOLE==1)
	#define BOOTUP_UART_BASE REG_BASE_UART1
#else
	#define BOOTUP_UART_BASE REG_BASE_UART0
#endif

#if (CONFIG_HISILICON_EDB_LEVEL==255)

#define edb_init()
#define edb_cleanup()
#define edb_putc(x)
#define edb_putul(x)
#define edb_putstr(x)
#define edb_puthex(x)
#define edb_trace(x)
#define edb_put_strul(x,y)

#define hisilicon_trace(level,s,param...)

#else /* (CONFIG_HISILICON_EDB_LEVEL<255) */


#define edb_init()	do{ \
				edb_putstr("Kernel Early-Debug on Level "); \
				edb_putul(CONFIG_HISILICON_EDB_LEVEL);\
				edb_putstr(" \n"); \
			}while(0)
				
#define edb_cleanup()


#define AMBA_UART_DR	(*(volatile unsigned long *)IO_ADDRESS(BOOTUP_UART_BASE + 0x000))
#define AMBA_UART_LSR	(*(volatile unsigned long *)IO_ADDRESS(BOOTUP_UART_BASE + 0x014))

static inline void edb_putc(int c)
{
	while ( !( (AMBA_UART_LSR & (1<<5)) && (AMBA_UART_LSR & (1 << 6))));

	AMBA_UART_DR = c;
}

static void __edb_putul(unsigned long num)
{
	if(num == 0)
		return;
	__edb_putul(num/10);
	edb_putc('0'+(num%10));
}

static inline void edb_putul(unsigned long num)
{
	if(num == 0)
	{
		edb_putc('0');
		return;
	}
	__edb_putul(num);
}


static inline void edb_putstr(const char *s)
{
	while (*s) {
		while ( !( (AMBA_UART_LSR & (1<<5)) && (AMBA_UART_LSR & (1 << 6))));

		AMBA_UART_DR = *s;

		if (*s == '\n') {
			while ( !( (AMBA_UART_LSR & (1<<5)) && (AMBA_UART_LSR & (1 << 6))));

			AMBA_UART_DR = '\r';
		}
		s++;
	}
}



static inline void edb_puthex(unsigned long h)
{
	int i;
	char c;

	edb_putstr("0x");
	for(i=0; i<8; i++)
	{
		c = (h>>28)&0x0F;

		if(c>=10)
			c = (c-10)+'A';
		else c = c+'0';

		edb_putc(c);

		h = h<<4;
	}
}


#define EDB_TRACE_LEVEL CONFIG_HISILICON_EDB_LEVEL

#define edb_trace(level) do{\
				if(level >= EDB_TRACE_LEVEL){\
					edb_putstr(__FUNCTION__);\
					edb_putstr("\t");\
					edb_putul(__LINE__);\
				}\
			}while(0)
#define edb_put_strul(s,ul) do{ edb_putstr(s); edb_putul(ul); }while(0)


#define hisilicon_trace(level,s,param...) do{ \
		if(level >= EDB_TRACE_LEVEL)\
			printk(KERN_INFO "[%s, %d]\t" s "\n",__FUNCTION__,__LINE__, param);	}while(0)

#endif /* (CONFIG_HISILICON_EDB_LEVEL==255) */

#endif /* __ASM_ARCH_EARLY_DEBUG_H */

