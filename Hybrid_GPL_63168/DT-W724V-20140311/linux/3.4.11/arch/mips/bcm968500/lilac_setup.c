#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
/*
 *  	lilac_setup.c
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/serial_8250.h>
#include <asm/time.h>
#include <asm/reboot.h>
#include <asm/mips-boards/prom.h>
#include <asm/bootinfo.h>
#include <../kernel/time/tick-internal.h>

#include "bl_os_wraper.h"
#include "access_macros.h"
#include "bl_lilac_rst.h"
#include "bl_lilac_ic.h"
#include "bl_lilac_timer.h"
#include "bl_lilac_mips_blocks.h"
#include "bl_lilac_eth_common.h"
#include <board.h>


extern void prom_printf(char *fmt, ...);

#define BL_LILAC_MAX_MEM_PART_NAME	32
#define PAT64   0xfeed6464
#define PAT128  0x12801280
#define PAT256  0x02560256

unsigned long bl_lilac_get_phys_mem_size(void)
{
	static long size = 0;
			
	if(size == 0) 
	{
		volatile __u32 *p255 = (unsigned int *)0xaffffffc;
		volatile __u32 *p127 = (unsigned int *)0xa7fffffc;
		volatile __u32 *p63  = (unsigned int *)0xa3fffffc;
		
		__u32 p63_save  = *p63;
		__u32 p127_save = *p127;
		__u32 p255_save = *p255;
		
		*p63  = PAT64;
		*p127 = PAT128;
		*p255 = PAT256;
		
		if(*p63 != PAT64)
		{
			size = 0x4000000;//64M
			*p63  = p63_save;
		} 
		else if(*p127 != PAT128)
		{
			size = 0x8000000;//128M
			*p63  = p63_save;
			*p127 = p127_save;	
		}
		else 
		{
			size = 0x10000000;//256M
			*p63  = p63_save;
			*p127 = p127_save;
			*p255 = p255_save;
		}
		prom_printf("PHYSICAL MEMORY SIZE DETECTED: %dMB\n", (int)(size >> 20));
	}
	return size;
}

struct boot_mem_name 
{
	char name[BL_LILAC_MAX_MEM_PART_NAME];
} map_name[BOOT_MEM_MAP_MAX];


unsigned long get_usable_mem_size(void)
{
	int i;
	for(i = 0; i < boot_mem_map.nr_map; i++)
	{
		if( boot_mem_map.map[i].type == BOOT_MEM_RAM )
			return boot_mem_map.map[i].size;
	}
	return 0;
}

void bl_lilac_mem_rsrv_init(void)
{
	unsigned long		start, size;
	int 				x = 0;
	unsigned long 		usr_usable;	
	unsigned long 		usr_reserved;
	
	unsigned long 		end 			= bl_lilac_get_phys_mem_size();
	int 				i = 0;
	NVRAM_DATA NvramData;

	if (boot_mem_map.nr_map != 0)
	{
		usr_usable		= get_usable_mem_size();
		usr_reserved	= end - usr_usable;
		
		if( usr_usable == 0 )
		{
			prom_printf("Error: Detected usable memory of 0 bytes\n");
			BUG_ON(1);
		}
		
		if( end < usr_usable )
		{
			prom_printf("Error: Detected usable memory greater than physical memory\n");
			BUG_ON(1);
		}

		if(usr_reserved == 0)
		{
			boot_mem_map.nr_map = 0;
		}
		else
		{
			boot_mem_map.map[0].addr = end - usr_reserved;
			boot_mem_map.map[0].size = usr_reserved;
			boot_mem_map.map[0].type = BOOT_MEM_RESERVED;
			strcpy(map_name[0].name, "mem_user_reserved");
			boot_mem_map.nr_map = 1;
			end = end - usr_reserved;
		}
	}

	if(end > 0x8000000)
	{
		boot_mem_map.map[boot_mem_map.nr_map].size = end-0x8000000;
		end = 0x8000000;
		boot_mem_map.map[boot_mem_map.nr_map].addr = end;
		boot_mem_map.map[boot_mem_map.nr_map].type = BOOT_MEM_RAM;
		boot_mem_map.nr_map++;
	}

	kerSysNvRamGet((char *)&NvramData, sizeof(NVRAM_DATA), 0);

	strcpy(map_name[boot_mem_map.nr_map].name, TM_BASE_ADDR_STR);	
	i = NvramData.allocs.alloc_rdp.tmsize;
	if (i == 0xff) {
		/* Erased NVRAM should be treated as zero */
		i = 0;
	}
	boot_mem_map.map[boot_mem_map.nr_map].size = i * 0x100000;
	
	if(boot_mem_map.map[boot_mem_map.nr_map].size < BL_LILAC_TM_DEF_DDR_SIZE)
		boot_mem_map.map[boot_mem_map.nr_map].size = BL_LILAC_TM_DEF_DDR_SIZE;
		
	end = end - boot_mem_map.map[boot_mem_map.nr_map].size;
	boot_mem_map.map[boot_mem_map.nr_map].addr = end;
	boot_mem_map.map[boot_mem_map.nr_map].type = BOOT_MEM_RESERVED;
	boot_mem_map.nr_map++;

	strcpy(map_name[boot_mem_map.nr_map].name, TM_MC_BASE_ADDR_STR);	
	i = NvramData.allocs.alloc_rdp.mcsize;
	if (i == 0xff) {
		/* Erased NVRAM should be treated as zero */
		i = 0;
	}
	boot_mem_map.map[boot_mem_map.nr_map].size = i * 0x100000;
	
	if(boot_mem_map.map[boot_mem_map.nr_map].size < BL_LILAC_TM_MC_DEF_DDR_SIZE)
		boot_mem_map.map[boot_mem_map.nr_map].size = BL_LILAC_TM_MC_DEF_DDR_SIZE;
		
	end = end - boot_mem_map.map[boot_mem_map.nr_map].size;
	boot_mem_map.map[boot_mem_map.nr_map].addr = end;
	boot_mem_map.map[boot_mem_map.nr_map].type = BOOT_MEM_RESERVED;
	boot_mem_map.nr_map++;

#ifdef CONFIG_BCM_ENDPOINT_MODULE
	strcpy(map_name[boot_mem_map.nr_map].name, DSP_BASE_ADDR_STR);
	boot_mem_map.map[boot_mem_map.nr_map].size = BL_LILAC_DSP_DEF_DDR_SIZE;
	end = end - boot_mem_map.map[boot_mem_map.nr_map].size;
	boot_mem_map.map[boot_mem_map.nr_map].addr = end;
	boot_mem_map.map[boot_mem_map.nr_map].type = BOOT_MEM_RESERVED;
	boot_mem_map.nr_map++;
#endif

	/* add the linux usable region */
	start	= 0;
	size 	= end;
	x 		= boot_mem_map.nr_map;
	
	boot_mem_map.map[x].addr = start;
	boot_mem_map.map[x].size = size;
	boot_mem_map.map[x].type = BOOT_MEM_RAM;
	boot_mem_map.nr_map++;
}

BL_LILAC_SOC_STATUS bl_lilac_plat_mem_rsrv_get_by_name(char *name, void **addr, size_t *size)
{
	int i;
	*addr = NULL;
	*size = 0;

	/* the data in index i of boot_mem_map refers to the name in map_name with the same index i */
	for (i=0; i<boot_mem_map.nr_map; i++)
	{
		if ( strcmp(name, map_name[i].name) == 0 )
		{
			*addr = (void*)(((unsigned int)boot_mem_map.map[i].addr) | 0xA0000000);
			*size = boot_mem_map.map[i].size;
			return BL_LILAC_SOC_OK;
		}
	}
	return BL_LILAC_SOC_ERROR;
}

static void __init serial_init(void)
{
	struct uart_port s;

	bl_lilac_cr_device_reset(BL_LILAC_CR_UART_C, BL_LILAC_CR_OFF);
	/* enable UART */
	bl_lilac_cr_device_reset(BL_LILAC_CR_UART_C, BL_LILAC_CR_ON);

	memset(&s, 0, sizeof(s));
	s.membase = (void __iomem *)(CE_MIPS_BLOCKS_VPB_UART_ADDRESS | SOC_BASE_ADDRESS); 
	s.mapbase = CE_MIPS_BLOCKS_VPB_UART_ADDRESS | SOC_BASE_ADDRESS;

	s.irq =  INT_NUM_IRQ0 + BL_LILAC_IC_INTERRUPT_UART;
	s.uartclk = get_lilac_peripheral_clock_rate();
	s.flags = UPF_BOOT_AUTOCONF |  UPF_SKIP_TEST;
	s.iotype = UPIO_MEM32;
	s.regshift = 2;
	s.line = 0;

	if(early_serial_setup(&s) != 0) 
	{
		prom_printf(KERN_ERR "Serial setup failed!\n");
	}
}

void __init plat_mem_setup(void)
{
	prom_printf("plat_mem_setup\n");

	bl_lilac_mem_rsrv_init();

	serial_init();
	bl_lilac_cr_device_reset(BL_LILAC_CR_TIMERS_C, BL_LILAC_CR_ON);
	
	mips_hpt_frequency	=  get_lilac_system_clock_rate()/2;
	_machine_restart	=  lilac_restart;
	
	printk("Lilac SOC setup\n");
}

/* how many counter cycles in a jiffy */
static unsigned long cycles_per_jiffy __read_mostly;

/* expirelo is the count value for next CPU timer interrupt */
static unsigned int expirelo;

static void c0_timer_ack(void)
{
	unsigned int count;

	/* Ack this timer interrupt and set the next one.  */
	expirelo += cycles_per_jiffy;
	write_c0_compare(expirelo);

	/* Check to see if we have missed any timer interrupts.  */
	while (((count = read_c0_count()) - expirelo) < 0x7fffffff) 
	{
		/* missed_timer_count++; */
		expirelo = count + cycles_per_jiffy;
		write_c0_compare(expirelo);
	}
}

static cycle_t c0_hpt_read(struct clocksource *cs)
{
	return read_c0_count();
}

struct clocksource clocksource_mips = {
	.name		= "MIPS",
	.mask		= 0xffffffff,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	.rating		= 300,
	.read		= c0_hpt_read
};

irqreturn_t  lilac_timer_handler(int irq_num,  void *dev_id)
{
	raw_spin_lock(&xtime_lock);
	
	c0_timer_ack();
	do_timer(1);
	
	raw_spin_unlock(&xtime_lock);
	
	update_process_times(user_mode(get_irq_regs()));
	
	return IRQ_HANDLED;
}

static struct irqaction lilac_timer_action = {
	.handler = lilac_timer_handler,
	.flags = IRQF_DISABLED | IRQF_PERCPU | IRQF_TIMER,
	.name = "lilac_timer",
};

void __init lilac_late_time_init(void)
{
	bl_rttmr_init(DEFAULT_RTTIMER);
}

void __init plat_time_init(void)
{
	u64 temp;
	u32 shift;

	write_c0_count(0);
	write_c0_compare(0);
	
	setup_irq(0x67, &lilac_timer_action);
	
	timekeeping_init();

	cycles_per_jiffy = (mips_hpt_frequency + HZ / 2) / HZ;
	expirelo = read_c0_count() + cycles_per_jiffy;
	write_c0_compare(expirelo);
	write_c0_status(read_c0_status() | STATUSF_IP7);

	/* Find a shift value */
	for (shift = 32; shift > 0; shift--)
	{
		temp = (u64) NSEC_PER_SEC << shift;
		do_div(temp, mips_hpt_frequency);
		if((temp >> 32) == 0)
			break;
	}
	clocksource_mips.shift	= shift;
	clocksource_mips.mult	= (u32)temp;
	
	clocksource_register(&clocksource_mips);
	
	late_time_init = lilac_late_time_init;
}

// neede by function `show_cpuinfo' /arch/mips/kernel/proc.c
const char *get_system_type(void)
{
	return "BCM968500 Lilac SOC";
}
#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */

