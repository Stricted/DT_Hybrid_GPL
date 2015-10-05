#if defined (CONFIG_BCM_KF_MIPS_BCM9685XX)
//#include "bl_os_wraper.h"
#include <linux/serial_core.h>
#include <asm/bootinfo.h>
#include <asm/mips-boards/prom.h>

#include "bl_lilac_rst.h"
#include "bl_lilac_brd.h"
#include "bcm_hwdefs.h"
#include "board.h"
#include <boardparms.h>

//====================================================================
unsigned char g_blparms_buf[1024];

struct NS16550 {
	unsigned long rbr;		/* 0 */
	unsigned long ier;		/* 1 */
	unsigned long fcr;		/* 2 */
	unsigned long lcr;		/* 3 */
	unsigned long mcr;		/* 4 */
	unsigned long lsr;		/* 5 */
	unsigned long msr;		/* 6 */
	unsigned long scr;		/* 7 */
} __attribute__ ((aligned(4)));

typedef volatile struct NS16550 *NS16550_t;

static NS16550_t ns_port =	(NS16550_t)0xb90a0400;
#define PROM_PRINT_BUF_SIZE 1024
#define BL_LILAC_HEX_DIGIT_ERROR	200

inline int prom_putchar(char c)
{
 	unsigned  LSR_THRE = 0x20;

	while ((ns_port->lsr & LSR_THRE) == 0);
	
	ns_port->rbr = c;
	return 1;
}

void prom_printf(char *fmt, ...)
{
	va_list		args;
	int 		i;
	char 		buf[PROM_PRINT_BUF_SIZE];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);

	for(i= 0; i < PROM_PRINT_BUF_SIZE; i++)
	{
		if(!buf[i])
			break;
		
		if (buf[i]== '\n')
			prom_putchar('\r');

		prom_putchar(buf[i]);
	}
	
	va_end(args);
}

//==========================================================================

void  __init prom_init_cmdline(void)
{
    strcpy(arcs_cmdline, CONFIG_ROOT_FLASHFS);
}

static void __init  print_board_type(void)
{
	char boardName[BP_BOARD_ID_LEN];
	
	if (!BpGetBoardId(boardName))
		prom_printf("Board: Lilac Evaluation (%s)\n",boardName);
	else
		prom_printf("Board: Unknown\n");


   // kerSysNvRamGetBoardIdLocked(promBoardIdStr); TBD
   // printk( "%s prom init\n", promBoardIdStr ); TBD
	
	prom_printf("CPU Clock:            %lu MHz\n"
				"DDR Clock:            %lu MHz\n"
				"RNR Clock:            %lu MHz\n"
				"Peripheral BUS Clock: %lu MHz\n",
				get_lilac_system_clock_rate()/1000000, get_lilac_ddr_clock_rate()/1000000,
				get_lilac_rnr_clock_rate()/1000000, get_lilac_peripheral_clock_rate()/1000000);
}


/* Retrieve a buffer of paramters passed by the boot loader.  Functions in
 * board.c can return requested parameter values to a calling Linux function.
 */
void __init retrieve_boot_loader_parameters(void)
{
    extern unsigned char _text;
    unsigned long blparms_magic = *(unsigned long *) (&_text - 8);
    unsigned long blparms_buf = *(unsigned long *) (&_text - 4);
    unsigned char *src = (unsigned char *) blparms_buf;
    unsigned char *dst = g_blparms_buf;

    if( blparms_magic != BLPARMS_MAGIC )
    {
        /* Subtract four more bytes for NAND flash images. */
        blparms_magic = *(unsigned long *) (&_text - 12);
        blparms_buf = *(unsigned long *) (&_text - 8);
        src = (unsigned char *) blparms_buf;
    }

    if( blparms_magic == BLPARMS_MAGIC )
    {
        do
        {
            *dst++ = *src++;
        } while( (src[0] != '\0' || src[1] != '\0') &&
          (unsigned long) (dst - g_blparms_buf) < sizeof(g_blparms_buf) - 2);
    }

    dst[0] = dst[1] = '\0';
}

void  __init prom_init(void)
{
	int argc = fw_arg0;
    u32 *argv = (u32 *)CKSEG0ADDR(fw_arg1);
    int i;
	char console_string[40];	
	print_board_type();
	
	kerSysEarlyFlashInit();	
	
	prom_init_cmdline();
	
	strcat(arcs_cmdline, " ");

    for (i = 1; i < argc; i++) {
        strcat(arcs_cmdline, (char *)CKSEG0ADDR(argv[i]));
        if (i < (argc - 1))
            strcat(arcs_cmdline, " ");
    }
	
	if(!strstr(arcs_cmdline, "console=ttyS"))
	{
		sprintf(console_string, " console=ttyS0,%d%c%c%c", 115200, 'n', '8', '\0');
		strcat(arcs_cmdline, console_string);
		prom_printf("Config serial console:%s\n", console_string);
	}
	
	retrieve_boot_loader_parameters();
}
//neded by /arch/mips/mm/init.c:460
void __init prom_free_prom_memory(void)
{
	return ;
}

#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */


