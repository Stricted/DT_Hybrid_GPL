#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/mbus.h>
#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include "pcie_godbox_1.h"


enum pcie_sel{
	/* neither controllers will be selected. */
	pcie_sel_none,
	/* pcie0 selected. pcie0 can only connected with a x1 device */
	pcie0_x1_sel
};
enum pcie_work_mode{
	pcie_wm_rc = 0x4,       /*rc mode*/
	pcie_wm_ep = 0x0,       /*ep mode*/
	pcie_wm_lep = 0x1       /*legacy ep mode*/
};
enum pcie_controller{
	pcie_controller_none,
	pcie_controller_0,
	pcie_controller_1
};
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

#define MAX_IATU_PER_CTRLLER	(6)
struct pcie_info{
	/* root bus number */
	u8		root_bus_nr;
	/* belong to which controller */
	enum		pcie_controller controller;
	/* device config space base addr */
	unsigned int	base_addr;
	/* rc config space base addr */
	unsigned int	conf_base_addr;
};

static struct pcie_info pcie_info[1];
static int pcie_controllers_nr;
/* pcie0 & pcie1 sel */
/* default controller is pcie0 */
static unsigned int pcie0_sel = CONFIG_PCIE1_SEL;

/* work mode */
/* default work mode is rc. */
static unsigned int pcie0_work_mode = pcie_wm_rc;

/* mem space size */
/* default mem space size: 64M */
static unsigned int pcie0_mem_space_size = CONFIG_PCIE1_DEVICES_MEM_SIZE;

/* sys controller map address */
static void * __iomem pcie_sys_base_virt;
static unsigned int pcie_errorvalue;

static int pcie_fault(unsigned long addr, unsigned int fsr,
		struct pt_regs *regs)
{
	pcie_errorvalue = 1;
	return 0;
}

/* locks: read lock & write lock. */
static DEFINE_SPINLOCK(rlock);
static DEFINE_SPINLOCK(wlock);

#define PCIE0_MODE_SEL  (1 << 0)
#define PCIE1_MODE_SEL  (1 << 1)
static unsigned int hisi_pcie_mode_sel;

#include "pcie_godbox_1.c"


/* format : pcie0_mem=0xa00000;pcie1_mem=0xa00000 */
static int __init pcie0_mem_parser(char* str)
{
	unsigned int size;
	if (strict_strtoul(str, 16, (long *)&size) < 0)
		return 0;
	/*if size >= 64M, set default 64M*/
	if (size >= 0x4000000)
		size = 0x4000000;
	pcie0_mem_space_size = size;
	pcie1_iatu_table[2].lbar = 0x40000000 + pcie0_mem_space_size;
	return 1;
}
__setup("pcie0_mem=", pcie0_mem_parser);

/* pcie sel boot args format: pcie0_sel=x1 pcie1=x1 or pcie1=x2.
 * any other value after "pcieX_sel=" prefix
 * will be treated as none controller will be selected.
 * e.g. pcie0_sel=none will be treated as you will not selected pcie0
 */
static int __init pcie0_sel_parser(char* str)
{
	if (strncasecmp(str, "x1", 2) == 0)
		pcie0_sel = pcie0_x1_sel;
	else
		pcie0_sel = pcie_sel_none;

	return 1;
}
__setup("pcie0_sel=", pcie0_sel_parser);

static struct resource pcie0_mem_space = {
	.name   = "PCIE0 memory space",
	.start  = PCIE1_BASE_ADDR_PHYS,
	.end    = 0,
	.flags  = IORESOURCE_MEM,
};
static struct resource pcie0_io_space = {
	.name   = "PCIE0 io space",
	.start  = 0,
	.end    = 0,
	.flags  = IORESOURCE_IO,
};

static void __init pcie_preinit(void)
{
	/* setup resource */
	pcie0_mem_space.end = PCIE1_BASE_ADDR_PHYS + pcie0_mem_space_size - 1;
	pcie0_io_space.start = pcie0_mem_space.end + 1;
	pcie0_io_space.end = PCIE1_BASE_ADDR_PHYS + PCIE_BASE_ADDR_SIZE - 1;
}

static int __init pcie_setup(int nr, struct pci_sys_data* sys)
{
	struct pcie_info *info;
	int ret;

	if (nr >= pcie_controllers_nr)
		return 0;

	info = &pcie_info[nr];
	/* record busnr for cfg ops use */
	info->root_bus_nr = sys->busnr;
	sys->mem_offset = 0;

	/* requeset resources for corresponding controller */
	if (info->controller == pcie_controller_0) {
		ret = request_resource(&ioport_resource, &pcie0_io_space);
		if (ret) {
			pcie_error("Cannot request io resource for pcie0,"\
					"pcie0_io_space.start=0x%x,end:0x%x\n",
					pcie0_io_space.start,
					pcie0_io_space.end);
			return ret;
		}
		ret = request_resource(&iomem_resource, &pcie0_mem_space);
		if (ret) {
			pcie_error("Cannot request mem resource for pcie0");
			release_resource(&pcie0_io_space);
			return ret;
		}
		sys->resource[0] = &pcie0_io_space;
		sys->resource[1] = &pcie0_mem_space;
	}
	sys->resource[2] = NULL;

	return 1;
}

static struct pcie_info * __init bus_to_info(int busnr)
{
	int i = pcie_controllers_nr - 1;
	for (; i >= 0; i--) {
		if (pcie_info[i].controller != pcie_controller_none
				&& pcie_info[i].root_bus_nr <= busnr
				&& pcie_info[i].root_bus_nr != -1)
			return &pcie_info[i];
	}
	return NULL;
}

static int __init pcie_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	struct pcie_info *info = bus_to_info(dev->bus->number);
	if (!info) {
		pcie_error("Cannot find corresponding controller "\
				"for appointed device!");
		BUG();
		return -1;
	}

	if (info->controller == pcie_controller_0) {
		switch (pin) {
		case PCIE_INTA_PIN:
			return PCIE1_IRQ_INTA;
		case PCIE_INTB_PIN:
			return PCIE1_IRQ_INTB;
		case PCIE_INTC_PIN:
			return PCIE1_IRQ_INTC;
		case PCIE_INTD_PIN:
			return PCIE1_IRQ_INTD;
		default:
			pcie_error("Unkown pin for mapping irq!");
			return -1;
		}
	}
	pcie_error("Why I'm here??");
	BUG();
	return -1;
}

#define PCIE_CFG_BUS(busnr)	((busnr & 0xff) << 20)
#define PCIE_CFG_DEV(devfn)	((devfn & 0xff)  << 12)
#define PCIE_CFG_REG(reg)	(reg & 0xffc) /*set dword align*/

static inline unsigned int to_pcie_address(struct pci_bus *bus,
		unsigned int devfn, int where)
{
	struct pcie_info *info = bus_to_info(bus->number);
	unsigned int address = 0;
	if (!info) {
		pcie_error("Cannot find corresponding controller "\
				"for appointed device!");
		BUG();
	}

	address = info->base_addr | PCIE_CFG_BUS(bus->number)
		| PCIE_CFG_DEV(devfn) | PCIE_CFG_REG(where);
	return address;
}

static inline int pcie_check_link_status(struct pcie_info *info)
{
	int val;
	val = readl(pcie_sys_base_virt +
			(info->controller == pcie_controller_0 ?
			 PERI_PCIE_STAT0 : PERI_PCIE_STAT1));
	val &= (1 << pciex_link_up);
	return val == (1 << pciex_link_up) ? 1 : 0;
}

static int pcie_read_from_device(struct pci_bus *bus, unsigned int devfn,
		int where, int size, u32 *value)
{
	struct pcie_info *info = bus_to_info(bus->number);
	unsigned int val;
	unsigned int addr;
	unsigned long flag;
	int i = 0;

	for (i = 0; i < 1000; i++) {
		if (pcie_check_link_status(info))
			break;
		udelay(1000);
	}
	if (i >= 1000) {
		pcie_debug(PCIE_DEBUG_LEVEL_MODULE, "pcie%d not link up!",
				info->controller == pcie_controller_0 ? 0 : 1);
		return -1;
	}
	spin_lock_irqsave(&rlock, flag);

	addr = to_pcie_address(bus, devfn, where);
	val = readl(addr);

	i = 0;
	while (i < 2000) {
		__asm__ __volatile__("nop\n");
		i++;
	}

	if (pcie_errorvalue == 1) {
		pcie_errorvalue = 0;
		val = 0xffffffff;
	}

	/* If got data is dword align, and val got from offset 0,
	 * I need to calculate which byte is wanted
	 */
	if (size == 1)
		*value = ((val >> ((where & 0x3) << 3)) & 0xff);
	else if (size == 2)
		*value = ((val >> ((where & 0x3) << 3)) & 0xffff);
	else if (size == 4)
		*value = val;
	else{
		pcie_error("Unkown size(%d) for read ops\n", size);
		BUG();
	}

	spin_unlock_irqrestore(&rlock, flag);
	return PCIBIOS_SUCCESSFUL;
}

static int pcie_read_from_dbi(struct pcie_info *info, unsigned int devfn,
		int where, int size, u32 *value)
{
	unsigned long flag;
	unsigned int val;

	/* for host-side config space read, ignore device func nr. */
	if (devfn > 0)
		return -EIO;

	spin_lock_irqsave(&rlock, flag);

	val = (u32)readl((void *)(info->conf_base_addr + (where & (~0x3))));

	if (1 == size)
		*value = (val >> ((where & 0x3) << 3)) & 0xff;
	else if (2 == size)
		*value = (val >> ((where & 0x3) << 3)) & 0xffff;
	else if (4 == size)
		*value = val;
	else{
		pcie_error("Unkown size for config read operation!");
		BUG();
	}

	spin_unlock_irqrestore(&rlock, flag);

	return PCIBIOS_SUCCESSFUL;
}

static int pcie_read_conf(struct pci_bus *bus, unsigned int devfn,
		int where, int size, u32 *value)
{
	struct pcie_info *info = bus_to_info(bus->number);

	if (unlikely(!info)) {
		pcie_error("Cannot find corresponding controller "\
				"for appointed device!");
		BUG();
	}
	if (bus->number == info->root_bus_nr)
		return  pcie_read_from_dbi(info, devfn, where, size, value);
	else
		return  pcie_read_from_device(bus, devfn, where, size, value);
}

static int pcie_write_to_device(struct pci_bus *bus, unsigned int devfn,
		int where, int size, u32 value)
{
	struct pcie_info *info = bus_to_info(bus->number);
	unsigned int addr;
	unsigned int org;
	unsigned long flag;

	if (!pcie_check_link_status(info)) {
		pcie_debug(PCIE_DEBUG_LEVEL_MODULE, "pcie%d not link up!",
				info->controller == pcie_controller_0 ? 0 : 1);
		return -1;
	}

	spin_lock_irqsave(&wlock, flag);

	pcie_read_from_device(bus, devfn, where, 4, &org);

	addr = to_pcie_address(bus, devfn, where);

	if (size == 1) {
		org &= (~(0xff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	} else if (size == 2) {
		org &= (~(0xffff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	} else if (size == 4) {
		org = value;
	} else {
		pcie_error("Unkown size(%d) for read ops\n", size);
		BUG();
	}
	writel(org, addr);

	spin_unlock_irqrestore(&wlock, flag);

	return PCIBIOS_SUCCESSFUL;

}

static int pcie_write_to_dbi(struct pcie_info *info, unsigned int devfn,
		int where, int size, u32 value)
{
	unsigned long flag;
	unsigned int org;

	spin_lock_irqsave(&wlock, flag);

	if (pcie_read_from_dbi(info, devfn, where, 4, &org)) {
		pcie_error("Cannot read from dbi! 0x%x:0x%x:0x%x!",
				0, devfn, where);
		spin_unlock_irqrestore(&wlock, flag);
		return -EIO;
	}
	if (size == 1) {
		org &= (~(0xff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	} else if (size == 2) {
		org &= (~(0xffff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	} else if (size == 4) {
		org = value;
	} else {
		pcie_error("Unkown size(%d) for read ops\n", size);
		BUG();
	}
	writel(org, info->conf_base_addr + (where & (~0x3)));

	spin_unlock_irqrestore(&wlock, flag);

	return PCIBIOS_SUCCESSFUL;
}

static int pcie_write_conf(struct pci_bus *bus, unsigned int devfn,
		int where, int size, u32 value)
{
	struct pcie_info *info = bus_to_info(bus->number);

	if (unlikely(!info)) {
		pcie_error("Cannot find corresponding controller "\
				"for appointed device!");
		BUG();
	}

	if (bus->number == info->root_bus_nr)
		return pcie_write_to_dbi(info, devfn, where, size, value);
	else
		return pcie_write_to_device(bus, devfn, where, size, value);
}

static struct pci_ops pcie_ops = {
	.read = pcie_read_conf,
	.write = pcie_write_conf,
};

static struct pci_bus *__init pcie_scan_bus(int nr, struct pci_sys_data* sys)
{
	struct pci_bus *bus;

	if (nr < pcie_controllers_nr) {
		bus = pci_scan_bus(sys->busnr, &pcie_ops, sys);
	} else {
		bus = NULL;
		pcie_error("Unkown controller nr :0x%x!", nr);
		BUG();
	}

	return bus;
}

static struct hw_pci hipcie __initdata = {
	.nr_controllers = 1,
	.preinit    = pcie_preinit,
	.swizzle    = pci_std_swizzle,
	.setup      = pcie_setup,
	.scan       = pcie_scan_bus,
	.map_irq    = pcie_map_irq,
};

static int __init pcie_init(void)
{
	if (pcie0_sel > pcie0_x1_sel
			|| pcie0_mem_space_size >= 0x10000000
			|| CONFIG_PCIE1_DEVICES_CONFIG_SIZE > 0x10000000
			|| CONFIG_PCIE1_DEVICES_CONFIG_SIZE < 0) {
		printk(KERN_ERR "Invalid parameter pcie0_sel(%d), "\
				"pcie0_mem_space_size(0x%x), "\
				"CONFIG_PCIE1_DEVICES_CONFIG_SIZE(0x%x)\n",
				pcie0_sel, pcie0_mem_space_size,
				CONFIG_PCIE1_DEVICES_CONFIG_SIZE);
		return -EINVAL;
	}

	if (pcie0_sel == pcie_sel_none) {
		pcie_error("Pcie0 not enabled!\n");
		return -EIO;
	}

	printk("pcie0 sel: 0x%x, pcie0 memsize:0x%x. "\
			"pcie1_iatu_table[1].lbar=0x%x\n",
			pcie0_sel, pcie0_mem_space_size,
			pcie1_iatu_table[1].lbar);

	pcie_sys_base_virt = ioremap_nocache(PCIE_SYS_BASE_PHYS, 0x1000);
	if (!pcie_sys_base_virt) {
		pcie_error("Cannot map system controller register base!");
		return -EIO;
	}

	pcie_info[0].root_bus_nr = -1;

	/*
	 * scene: PCIe host(RC)<--->SWITCH<--->PCIe device(*)
	 *                            |
	 *                            |------->NULL SLOT
	 * PCIe generate a "DataAbort" to ARM, when scaning NULL SLOT.
	 * Register "ARM VECTOR HOOK" capture and fix it.
	 */
	hook_fault_code(22, pcie_fault, 7,
			"external abort on non-linefetch");

	/* if enabled pcie0 */
	if (pcie0_sel == pcie0_x1_sel) {
		/* work at rc mode */
		if (pcie0_work_mode == pcie_wm_rc) {
			pcie_info[pcie_controllers_nr].controller =
				pcie_controller_0;

			pcie_info[pcie_controllers_nr].conf_base_addr =
				(unsigned int)ioremap_nocache(DBI_BASE_ADDR_1,
						0x1000);

			if (!pcie_info[pcie_controllers_nr].conf_base_addr) {
				pcie_error("Cannot map host's dbi space"\
						" for controller0!");
				goto err_conf_base_0;
			}

			pcie_info[pcie_controllers_nr].base_addr =
				(unsigned int)ioremap_nocache(PCIE1_MEMIO_BASE,
					CONFIG_PCIE1_DEVICES_CONFIG_SIZE);

			if (!pcie_info[pcie_controllers_nr].base_addr) {
				pcie_error("Cannot map config space base"\
						" for controller0!");
				goto err_base_0;
			}

			pcie_controllers_nr++;
			hisi_pcie_mode_sel |= PCIE0_MODE_SEL;
		}
	}

	/* none of the two controllers work at rc mode or is enabled,
	 * we do nothing
	 */
	if (!pcie_controllers_nr) {
		pcie_error("None of the two pcie controllers is enabled!");
		goto out;
	}

	if (pcie1_sys_init(hisi_pcie_mode_sel))
		goto out;
///////////÷ÿ–¬”≥…‰
            iounmap((void *)pcie_info[0].conf_base_addr);
			pcie_info[0].conf_base_addr =
				(unsigned int)ioremap_nocache(PCIE1_MEMIO_BASE,
						0x1000);
			if (!pcie_info[0].conf_base_addr) {
				pcie_error("Cannot map host's dbi space"\
						" for controller0!");
				goto err_conf_base_0;
			}

///////////
	/* reset how many controllers to enable. */
	hipcie.nr_controllers = pcie_controllers_nr;
	pci_common_init(&hipcie);
	return 0;

out:
	if (pcie0_sel == pcie0_x1_sel)
		iounmap((void *)pcie_info[0].base_addr);
err_base_0:
	if (pcie0_sel == pcie0_x1_sel)
		iounmap((void *)pcie_info[0].conf_base_addr);
err_conf_base_0:
	if (pcie_sys_base_virt)
		iounmap(pcie_sys_base_virt);

	return -EIO;
}

#include <linux/platform_device.h>
#include <linux/pm.h>

int hisi_pcie1_plat_driver_probe(struct platform_device *pdev)
{
	return pcie_init();
}
int hisi_pcie1_plat_driver_remove(struct platform_device *pdev)
{
	return 0;
}

#define PCIE_RC_DRV_NAME "hisi pcie1 root complex"
static struct platform_driver hisi_pcie_platform_driver = {
	.probe          = hisi_pcie1_plat_driver_probe,
	.remove         = hisi_pcie1_plat_driver_remove,
	.driver         = {
		.owner  = THIS_MODULE,
		.name   = PCIE_RC_DRV_NAME,
		.bus    = &platform_bus_type,
		.pm     = HISI_PCIE_PM_OPS
	},
};

static void hisi_pcie_platform_device_release(struct device *dev)
{
}
static struct resource hisi_pcie_resources[] = {
	[0] = {
		.start  = DBI_BASE_ADDR_1,
		.end    = DBI_BASE_ADDR_1 + 0x1000 - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct platform_device hisi_pcie_platform_device = {
	.name = PCIE_RC_DRV_NAME,
	.id   = 0,
	.dev = {
		.platform_data  = NULL,
		.dma_mask = (u64 *)~0,
		.coherent_dma_mask = (u64)~0,
		.release = hisi_pcie_platform_device_release,
	},
	.num_resources = ARRAY_SIZE(hisi_pcie_resources),
	.resource = hisi_pcie_resources,
};

static int hisi_pcie_init(void)
{
	int ret;
	ret = platform_device_register(&hisi_pcie_platform_device);
	if (ret)
		goto err_device;

	ret = platform_driver_register(&hisi_pcie_platform_driver);
	if (ret)
		goto err_driver;
	return 0;

err_driver:
	platform_device_unregister(&hisi_pcie_platform_device);
err_device:
	return -1;
}

static void hisi_pcie_exit(void)
{
	platform_device_unregister(&hisi_pcie_platform_device);
	platform_driver_unregister(&hisi_pcie_platform_driver);
}

module_init(hisi_pcie_init);
module_exit(hisi_pcie_exit);

MODULE_DESCRIPTION("Hisilicon PCI1-Express Root Complex driver");
MODULE_LICENSE("GPL");
