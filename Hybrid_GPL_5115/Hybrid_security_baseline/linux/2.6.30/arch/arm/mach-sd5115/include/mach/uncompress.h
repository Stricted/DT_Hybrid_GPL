#ifndef __HISI_UNCOMPRESS_H__
#define __HISI_UNCOMPRESS_H__

#define AMBA_UART_DR	(*(volatile unsigned char *)0x101FB000)
#define AMBA_UART_LCRH	(*(volatile unsigned char *)0x101FB02C)
#define AMBA_UART_CR	(*(volatile unsigned char *)0x101FB030)
#define AMBA_UART_FR	(*(volatile unsigned char *)0x101FB018)

/*
 * This does not append a newline
 */
static inline void putc(int c)
{
	while (AMBA_UART_FR & (1 << 5))
		barrier();

	AMBA_UART_DR = c;
}

static inline void flush(void)
{
	while (AMBA_UART_FR & (1 << 3))
		barrier();
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()

#endif
