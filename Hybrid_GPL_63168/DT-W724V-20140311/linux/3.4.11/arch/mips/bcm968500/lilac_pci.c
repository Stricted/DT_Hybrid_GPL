#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
#include <linux/pci.h>
#include <asm/irq.h>

#include "bl_lilac_brd.h"
#include "bl_lilac_ic.h"
#include "bl_lilac_pci.h"
#include "bl_os_wraper.h"

static struct resource bl_mem_resource0 = {
	.name	= "BL PCI MEM0",
	.start = BL_PCI_MEM_START_0,
	.end   = BL_PCI_MEM_END_0,
	.flags = IORESOURCE_MEM
};

static struct resource bl_io_resource0 = {
	.name	= "BL PCI IO0",
	.start = BL_PCI_IO_START_0,
	.end   = BL_PCI_IO_END_0,
	.flags = IORESOURCE_IO
};

static struct resource bl_mem_resource1 = {
	.name	= "BL PCI MEM1",
	.start = BL_PCI_MEM_START_1,
	.end   = BL_PCI_MEM_END_1,
	.flags = IORESOURCE_MEM
};

static struct resource bl_io_resource1 = {
	.name	= "BL PCI IO1",
	.start = BL_PCI_IO_START_1,
	.end   = BL_PCI_IO_END_1,
	.flags = IORESOURCE_IO
};

static int bl_pci_read_config(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val);
static int bl_pci_write_config(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val);

static struct pci_ops bl_pci_ops = {
	.read = bl_pci_read_config,
	.write = bl_pci_write_config
};

static struct pci_controller bl_pci_controller0 = {
	.pci_ops	    = & bl_pci_ops,
	.mem_resource	= & bl_mem_resource0,
	.io_resource	= & bl_io_resource0
};

static struct pci_controller bl_pci_controller1 = {
	.pci_ops	    = & bl_pci_ops,
	.mem_resource	= & bl_mem_resource1,
	.io_resource	= & bl_io_resource1
};

static int bl_pci_read_config(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val)
{
	if(bl_lilac_pci_rw_config(bus->number, 0, devfn, where, size, (unsigned long *)val) == BL_LILAC_SOC_OK)
		return PCIBIOS_SUCCESSFUL;
	else
		return PCIBIOS_DEVICE_NOT_FOUND;
}

static int bl_pci_write_config(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val)
{
	if(bl_lilac_pci_rw_config(bus->number, 1, devfn, where, size, (unsigned long *)&val) == BL_LILAC_SOC_OK)
		return PCIBIOS_SUCCESSFUL;
	else
		return PCIBIOS_DEVICE_NOT_FOUND;
}

#if 0
static irqreturn_t bl_pci_int(int irq, void *par)
{
	int id = ((struct pci_controller *)par)->bus->number;
    unsigned long status;

    status = bl_lilac_pci_get_int_status(id);
	
	if(status & ~(PCI_INT_A | PCI_INT_B | PCI_INT_C | PCI_INT_D))
	{
		printk("BL PCI interrupt status 0x%lx\n", status);
		bl_lilac_pci_ack_int_mask(id, status & ~(PCI_INT_A | PCI_INT_B | PCI_INT_C | PCI_INT_D));
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}
#endif

/*
    Initialize BL PCI controller
*/
int __init  bl_pcibios_init(void)
{
    BL_LILAC_IC_INTERRUPT_PROPERTIES intProperties = {
		.priority		= 5,
		.configuration	= BL_LILAC_IC_INTERRUPT_LEVEL_HIGH,
		.reentrant		= 0,
		.fast_interrupt	= 0
	};

	if(!bl_lilac_pci_lane_enabled(0) && !bl_lilac_pci_lane_enabled(1))
		return -1;
	
	set_io_port_base(0);
	ioport_resource.end = -1;	
	
    /* initialize BL PCI controller */
	bl_lilac_pci_init();

	if(bl_lilac_pci_lane_enabled(0))
	{
		bl_lilac_pci_init_lane(0);
		bl_lilac_ic_init_int(BL_LILAC_IC_INTERRUPT_PCI_0, & intProperties);

		/* register the controller to Linux data base */
		register_pci_controller(&bl_pci_controller0);
	}

	if(bl_lilac_pci_lane_enabled(1))
	{
		bl_lilac_pci_init_lane(1);
		bl_lilac_ic_init_int(BL_LILAC_IC_INTERRUPT_PCI_1, & intProperties);

		/* register the controller to Linux data base */
		register_pci_controller(&bl_pci_controller1);
	}

    return 0;
}
/* register the initializinf routine to Linux system */
arch_initcall(bl_pcibios_init);

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
    switch(dev->pin)
    {
    case 1:
		bl_lilac_pci_set_int_mask(dev->bus->number, PCI_INT_A, 0);
        break;
    case 2:
		bl_lilac_pci_set_int_mask(dev->bus->number, PCI_INT_B, 0);
        break;
    case 3:
		bl_lilac_pci_set_int_mask(dev->bus->number, PCI_INT_C, 0);
        break;
    case 4:
		bl_lilac_pci_set_int_mask(dev->bus->number, PCI_INT_D, 0);
        break;
    }

	if(dev->bus->number == 0)
		return INT_NUM_IRQ0 + BL_LILAC_IC_INTERRUPT_PCI_0;
	else if(dev->bus->number == 1)
		return INT_NUM_IRQ0 + BL_LILAC_IC_INTERRUPT_PCI_1;
	else
		return 0;
}
#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */

