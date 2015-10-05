/*
 * Author   : Ryan Shen ( s00152899 )
 * Contract : shentao@huawei.com
 * File     : core.c
 * Describe : This is core code for Linux Porting.           
 * Platform : ARM Cortex-A9 & Linux 2.6.34.10
 * Log      : 2011-09-25 : init version completed.
 */
#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/cnt32_to_63.h>
#include <linux/io.h>

#include <asm/clkdev.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/hardware/arm_timer.h>
#include <asm/hardware/gic.h>
#include <asm/hardware/vic.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>

#include <mach/time.h>
#include <mach/hardware.h>
#include <mach/early-debug.h>
#include <mach/irqs.h>
#include "clock.h"
#include <linux/bootmem.h>
#include <mach/kexport.h>
/* <DTS2012110301136 l00205975 20121103 begin */
#include <mach/hi_drv_wdt.h>
/* DTS2012110301136 l00205975 20121103 end> */
/* global variable */
void __iomem *gic_cpu_base_addr;
static unsigned long sd5115_timer_reload,timer0_clk_hz,timer1_clk_hz,timer0_clk_khz,timer1_clk_khz; 
extern void l2cache_init(void);
extern void l2cache_exit(void);
extern void l2cache_clean(void);
extern void hi_acp_init(void);
HI_CHIP_ID_E get_chip_id();

HI_CHIP_ID_E sd5115_chip_id=HI_CHIP_ID_NONE_E;

/*
 * get apb clock and set sys_timer value.
 */
static void early_init(void)
{
	unsigned long busclk;
	
	edb_trace(1);
	busclk = get_apbclk_hw();
	printk("sd5115 apb bus clk is %lu\n",busclk);

	sd5115_timer_reload = BUSCLK_TO_TIMER_RELOAD(busclk);
	timer0_clk_hz 		= BUSCLK_TO_TIMER0_CLK_HZ(busclk);
	timer0_clk_khz 		= BUSCLK_TO_TIMER0_CLK_KHZ(busclk);
	timer1_clk_hz 		= BUSCLK_TO_TIMER1_CLK_HZ(busclk);
	timer1_clk_khz 		= BUSCLK_TO_TIMER1_CLK_KHZ(busclk);
}

/*
 * init gic.
 */
void __init sd5115_gic_init_irq(void)
{
	edb_trace(1);
	gic_cpu_base_addr = (void __iomem *)CFG_GIC_CPU_BASE;
	gic_dist_init(0,(void __iomem *)CFG_GIC_DIST_BASE, HISI_GIC_IRQ_START);
	gic_cpu_init(0,(void __iomem *)CFG_GIC_CPU_BASE);
}

/*
 * cpu idle.
 */
void arch_idle(void)
{
	/*
	 * This should do all the clock switching
	 * and wait for interrupt tricks
	 */
	cpu_do_idle();
}

/* <DTS2012110301136 l00205975 20121103 begin */
/*  
 * cpu reset.
 */
/* DTS2012110301136 l00205975 20121103 end> */
void arch_reset(char mode,const char *cmd)
{
	uint ui_timer = jiffies;
	
	printk("RESET!\n");
  /* <DTS2012110301136 l00205975 20121103 begin */
	hi_kernel_wdt_set(1);
	
	hi_kernel_wdt_disable();
  /* DTS2012110301136 l00205975 20121103 end> */
	/* 兼容先前没有打开硬件狗的单板 */
	while(time_after_eq(ui_timer + 3*HZ, jiffies));

	/*Fix SD5115V100 MPW Bug, Set ECS PLL Bypass:0x10100138 21bit=1*/
	writel((readl(IO_ADDRESS(0x10100000 + 0x138)) | 0x00200000), IO_ADDRESS(0x10100000 + 0x138));
	/*Fix SD5115V100 MPW Bug, Set ECS PLL Bypass*/
	writel(0x51150100, IO_ADDRESS(0x10100000 + 0x4));
}

/* 静态地址映射，可以直接通过.virtual定义的地址访问物理地址 */
static struct map_desc sd5115_io_desc[] __initdata = {
    { /* 系统控制器 */
       .virtual      =  IO_ADDRESS(REG_BASE_SCTL),
        .pfn        = __phys_to_pfn(REG_BASE_SCTL),
        .length     = REG_IOSIZE_SCTL,
        .type       = MT_DEVICE
    },{/* 串口0 */
       .virtual     =  IO_ADDRESS(REG_BASE_UART0),
        .pfn        = __phys_to_pfn(REG_BASE_UART0),
        .length     = REG_IOSIZE_UART0,
        .type       = MT_DEVICE
    },{/* 串口1 */
       .virtual     =  IO_ADDRESS(REG_BASE_UART1),
        .pfn        = __phys_to_pfn(REG_BASE_UART1),
        .length     = REG_IOSIZE_UART1,
        .type       = MT_DEVICE
    },{/* 定时器0和1，作为系统时钟使用 */
       .virtual     =  IO_ADDRESS(REG_BASE_TIMER01),
        .pfn        = __phys_to_pfn(REG_BASE_TIMER01),
        .length     =  REG_IOSIZE_TIMER01,
        .type       = MT_DEVICE
    },{/* A9 PERI */
       .virtual     =  IO_ADDRESS(REG_BASE_A9_PERI),
        .pfn        = __phys_to_pfn(REG_BASE_A9_PERI),
        .length     =  REG_IOSIZE_A9_PERI,
        .type       = MT_DEVICE
    },{/* L2 Cache */
        .virtual     =  IO_ADDRESS(REG_BASE_L2CACHE),
        .pfn        = __phys_to_pfn(REG_BASE_L2CACHE),
        .length     =  REG_IOSIZE_L2CACHE,
        .type       = MT_DEVICE_STRONGLY_ORDERED
    },    
};

/* 静态地址映射 */
void __init sd5115_map_io(void)
{
	int i;
	
	iotable_init(sd5115_io_desc, ARRAY_SIZE(sd5115_io_desc));

    /* 初始化early debug */
    edb_init();
    
	for(i=0; i<ARRAY_SIZE(sd5115_io_desc); i++)
	{
		edb_putstr(" V: ");	edb_puthex(sd5115_io_desc[i].virtual);
		edb_putstr(" P: ");	edb_puthex(sd5115_io_desc[i].pfn);
		edb_putstr(" S: ");	edb_puthex(sd5115_io_desc[i].length);
		edb_putstr(" T: ");	edb_putul(sd5115_io_desc[i].type);
		edb_putstr("\n");
	}
	
	early_init();

	edb_trace(1);
}

static unsigned long long sd5115_cycles_2_ns(unsigned long long cyc)
{
	unsigned long cyc2ns_scale = (1000000 << 10)/timer1_clk_khz;
	return (cyc * cyc2ns_scale) >> 10;
}

static unsigned long free_timer_overflows=0;

unsigned long long sched_clock(void)
{
	unsigned long long ticks64;
	unsigned long ticks2,ticks1;
    ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
	do{
		ticks1 = ticks2;
		ticks64 = free_timer_overflows;
		ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
	}while(ticks1 > ticks2);

    /* 
    ** If INT is not cleaned, means the function is called with irq_save.
    ** The ticks has overflow but 'free_timer_overflows' is not be update.
    */
    if(readl(CFG_TIMER_VABASE + REG_TIMER1_MIS))
    {
        ticks64 += 1;
        ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
    }
    
	return sd5115_cycles_2_ns( (ticks64 << 32) | ticks2);
}

#define HIL_AMBADEV_NAME(name) hil_ambadevice_##name

#define HIL_AMBA_DEVICE(name,busid,base,platdata,id)		\
static struct amba_device HIL_AMBADEV_NAME(name) = {		\
	.dev		= {					\
		.coherent_dma_mask = ~0,			\
		.init_name= busid,				\
		.platform_data = platdata,			\
	},							\
	.res		= {					\
		.start	= REG_BASE_##base,			\
		.end	= REG_BASE_##base + 0x1000 -1,	\
		.flags	= IORESOURCE_IO,			\
	},							\
	.dma_mask	= ~0,					\
	.periphid	= id,					\
	.irq		= { INTNR_##base, NO_IRQ }		\
}

#define UART0_IRQ	{ INTNR_UART0, NO_IRQ }
#define UART1_IRQ	{ INTNR_UART1, NO_IRQ }

HIL_AMBA_DEVICE(uart0, "uart:0",  UART0,    NULL,   0x00041011);
HIL_AMBA_DEVICE(uart1, "uart:1",  UART1,    NULL,   0x00041011);

static struct amba_device *amba_devs[] __initdata = {			            
        & HIL_AMBADEV_NAME(uart0),
        & HIL_AMBADEV_NAME(uart1),
};

/*
 * These are fixed clocks.
 */
static struct clk uart_clk = {
	.rate	= 100000000,
};


static struct clk_lookup lookups[] = {
	{	/* UART0 */
		.dev_id		= "uart:0",
		.clk		= &uart_clk,
	}, {/* UART1 */
		.dev_id		= "uart:1",
		.clk		= &uart_clk,
	}
};

void __init sd5115_init(void)
{
	unsigned long i;
	
	edb_trace(1);

	for (i = 0; i < ARRAY_SIZE(lookups); i++)
		clkdev_add(&lookups[i]);

	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		edb_trace(1);
		amba_device_register(amba_devs[i], &iomem_resource);
	}

    sd5115_chip_id=get_chip_id();

#if 1 // y00206091
    hi_acp_init();
    edb_trace(1);

//	l2cache_clean();
    l2cache_init();
    edb_trace(1);

#endif

}

static void timer_set_mode(enum clock_event_mode mode,
			   struct clock_event_device *clk)
{
	unsigned long ctrl;
	switch(mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		writel(0, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	    writel(sd5115_timer_reload, CFG_TIMER_VABASE + REG_TIMER_RELOAD);
	    writel(CFG_TIMER_CONTROL, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		writel((CFG_TIMER_32BIT |CFG_TIMER_ONESHOT), CFG_TIMER_VABASE + REG_TIMER_CONTROL);
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	default:
		ctrl = readl(CFG_TIMER_VABASE + REG_TIMER_CONTROL);
		ctrl &= ~CFG_TIMER_ENABLE;
		writel(ctrl, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	}
}

static int timer_set_next_event(unsigned long evt,
				struct clock_event_device *unused)
{
	unsigned long ctrl;

	ctrl = readl(CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	ctrl &=~(CFG_TIMER_ENABLE | CFG_TIMER_INTMASK);
	writel(ctrl, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	writel(evt, CFG_TIMER_VABASE + REG_TIMER_RELOAD);
    writel(CFG_TIMER_ONE_CONTROL, CFG_TIMER_VABASE + REG_TIMER_CONTROL);

	return 0;
}

static struct clock_event_device timer0_clockevent =	 {
	.name		= "timer0",
	.shift		= 32,
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= timer_set_mode,
	.set_next_event	= timer_set_next_event,
};

/*
 * IRQ handler for the timer
 */
static irqreturn_t sd5115_timer_interrupt(int irq, void *dev_id)
{
    static unsigned char uc_print = 0;
    
    /* 内核软件长时间占用或者进入死循环,打印当前栈信息 */
    if ( ( 0 != hi_kernel_wdt_check() ) && ( 0 == uc_print ) )
    {
        uc_print++;
        dump_stack();
    }
    if ( ( readl(CFG_TIMER_VABASE+REG_TIMER_RIS) ) & 0x1 )
    {
        writel(~0, CFG_TIMER_VABASE + REG_TIMER_INTCLR);
        timer0_clockevent.event_handler(&timer0_clockevent);
    }
    return IRQ_HANDLED;
}

static irqreturn_t sd5115_freetimer_interrupt(int irq, void *dev_id)
{
	if((readl(CFG_TIMER_VABASE+REG_TIMER1_RIS))&0x1) {
		free_timer_overflows++;
		writel(~0, CFG_TIMER_VABASE + REG_TIMER1_INTCLR);
    }
    return IRQ_HANDLED;
}

static struct irqaction sd5115_timer_irq = {
	.name		= "SD5115 System Timer Tick",
	.flags		= IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
	.handler	= sd5115_timer_interrupt,
};

static struct irqaction sd5115_freetimer_irq = {
	.name		= "Free Timer",
	.flags		= IRQF_SHARED | IRQF_TIMER,
	.handler	= sd5115_freetimer_interrupt,
};

static cycle_t sd5115_get_cycles(struct clocksource *cs)
{
	return ~readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
}

static struct clocksource sd5115_clocksource = {
	.name 		= "timer1",
 	.rating		= 200,
 	.read		= sd5115_get_cycles,
	.mask		= CLOCKSOURCE_MASK(32),
 	.shift 		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static int __init sd5115_clocksource_init(void)
{

	writel(0, CFG_TIMER_VABASE + REG_TIMER1_CONTROL);
	writel(0xffffffff, CFG_TIMER_VABASE + REG_TIMER1_RELOAD);
	writel(0xffffffff, CFG_TIMER_VABASE + REG_TIMER1_VALUE);
	writel(CFG_TIMER_CONTROL, CFG_TIMER_VABASE + REG_TIMER1_CONTROL);
	
 	sd5115_clocksource.mult =
 		clocksource_khz2mult(timer1_clk_khz, sd5115_clocksource.shift);
 	clocksource_register(&sd5115_clocksource);

 	return 0;
}

static void __init sd5115_timer_init(void)
{	
	edb_trace(1);	

	/* enable of timer 0 and timer 1*/
	writel(readl(IO_ADDRESS(REG_BASE_SCTL)) | CFG_TIMER0_CLK_SOURCE, IO_ADDRESS(REG_BASE_SCTL));

	setup_irq(INTNR_TIMER_0_1, &sd5115_timer_irq);
	setup_irq(INTNR_TIMER_0_1, &sd5115_freetimer_irq);

	/* init clock soure and clock event */
	sd5115_clocksource_init();
	timer0_clockevent.mult =
		div_sc(timer0_clk_hz, NSEC_PER_SEC, timer0_clockevent.shift);
	timer0_clockevent.max_delta_ns =
		clockevent_delta2ns(0xffffffff, &timer0_clockevent);
	timer0_clockevent.min_delta_ns =
		clockevent_delta2ns(0xf, &timer0_clockevent);

	timer0_clockevent.cpumask = cpumask_of(0);
	clockevents_register_device(&timer0_clockevent);
	/*set timer clock source from our bus clock*/
	edb_trace(1);
}

struct sys_timer sd5115_timer = {
	.init = sd5115_timer_init,
};

HI_CHIP_ID_E get_chip_id()
{
    unsigned long reg;

#if (defined (CONFIG_HISI_SD5115_TYPE_T) && defined (CONFIG_HISI_SD5115_AF_FPGA))
        return HI_CHIP_ID_T_E;
#endif

    reg=readl(IO_ADDRESS(REG_BASE_SCTL+0x0800));
	printk("id:0x%x\r\n", reg);
	
    if (reg == 0x51150100)
    {
        return HI_CHIP_ID_H_E;
    }
    else if (reg == 0x51151100)
    {
        return HI_CHIP_ID_S_E;
    }
    else if (reg == 0x51152100)
    {
        return HI_CHIP_ID_T_E;
    }
    else
    {
        return HI_CHIP_ID_MPW_E;
    }
}

HI_CHIP_ID_E hi_kernel_get_chip_id()
{
    return sd5115_chip_id;
}
EXPORT_SYMBOL(hi_kernel_get_chip_id);


MACHINE_START(SD5115, "sd5115")
    .phys_io	    = 0x1010e000,                   /* 实际没有用处，配置系统中外设IO的起始地址即可 */
	.io_pg_offst	= (0xd010e000 >> 18) & 0xfffc,  /* 实际没有用处 */
	.boot_params	= PHYS_OFFSET+0x100,            /* 存放uboot参数的地址，需要和uboot中设置一致 */                                                  //必须和uboot里面设置的CFG_BOOT_PARAMS一致   
	.map_io			= sd5115_map_io,
	.init_irq		= sd5115_gic_init_irq,
	.timer			= &sd5115_timer,
	.init_machine	= sd5115_init,
MACHINE_END
