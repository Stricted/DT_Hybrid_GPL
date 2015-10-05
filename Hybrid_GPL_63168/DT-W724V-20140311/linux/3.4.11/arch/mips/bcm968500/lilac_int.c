#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
/*
 *  	lilac_int.c
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/mipsregs.h>
#include <asm/irq.h>
#include <asm/irq_cpu.h>
#include <asm/io.h>

#include "bl_os_wraper.h"
#include "bl_lilac_ic.h"

asmlinkage void plat_irq_dispatch(void)
{
	int isr;
	unsigned long register pending = read_c0_status() & read_c0_cause();

	if(pending & CAUSEF_IP7)    /* timer interrupt */
	{
		do_IRQ(0x67);
	}

	if(pending & CAUSEF_IP2)
	{
		isr = bl_lilac_ic_interrupt_number(0);
		if(isr < 32)
			do_IRQ(isr + INT_NUM_IRQ0);
	}

	if(pending & CAUSEF_IP3)
	{
          	isr = bl_lilac_ic_interrupt_number(1);
		if(isr < 32)
			do_IRQ(isr + (INT_NUM_IRQ0+32));
	}

}

static void lilac_disable_irq(struct irq_data *data)
{
	bl_lilac_ic_mask_irq( data->irq - INT_NUM_IRQ0);
}

static void lilac_enable_irq(struct irq_data *data)
{
	bl_lilac_ic_unmask_irq(data->irq - INT_NUM_IRQ0);
}

static void lilac_ack_irq(struct irq_data *data)
{
	bl_lilac_ic_isr_ack(data->irq - INT_NUM_IRQ0);
}

static struct irq_chip lilac_irq_type = {
	.name	= "Lilac_IRQ",
	.irq_mask	= (void*)lilac_disable_irq,
	.irq_unmask	= (void*)lilac_enable_irq,
	.irq_ack	= (void*)lilac_ack_irq,
};

void __init arch_init_irq(void)
{
	int i;
   	BL_LILAC_IC_INTERRUPT_PROPERTIES intUart =
   	{
    	.priority		= 2,
    	.configuration	= BL_LILAC_IC_INTERRUPT_LEVEL_HIGH,
    	.reentrant		= 0,
    	.fast_interrupt	= 0
   	};

	/*  Mask all interrupts. */
	clear_c0_status(ST0_IM);

	mips_cpu_irq_init();

	bl_lilac_ic_init(MIPSC);

	for(i = INT_NUM_IRQ0;  i < INT_NUM_IRQ0 + BL_LILAC_IC_INTERRUPT_LAST;  i++)
	{
		bl_lilac_ic_mask_irq(i);
		irq_set_chip_and_handler(i, &lilac_irq_type, handle_level_irq);
	}

	bl_lilac_ic_set_ghost(BL_LILACIC_INTERRUPT_GHOST);

	write_c0_status(read_c0_status() | STATUSF_IP3 | STATUSF_IP2);

	/* configure uart interrupt */
	bl_lilac_ic_init_int(BL_LILAC_IC_INTERRUPT_UART, & intUart);
}
#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */

