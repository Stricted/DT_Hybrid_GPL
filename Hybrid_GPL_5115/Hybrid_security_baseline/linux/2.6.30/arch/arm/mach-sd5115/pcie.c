#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/mbus.h>
#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <asm/delay.h>
#include <linux/spinlock.h>
#include "pcie.h"
#include <mach/kexport.h>
enum pcie_sel{
	pcie_sel_none,	/*neither controllers will be selected.*/
	pcie0_x1_sel,	/*pcie0 selected.*/
	pcie1_x1_sel,	/*pcie1 selected.*/
};
enum pcie_work_mode{
	pcie_wm_rc = 0x4,	/*rc mode*/
	pcie_wm_ep = 0x0,	/*ep mode*/
	pcie_wm_lep = 0x1	/*legacy ep mode*/
};
enum pcie_controller{
	pcie_controller_none,
	pcie_controller_0,
	pcie_controller_1
};
struct pcie_info{
	u8 root_bus_nr;						/*root bus number*/
	enum pcie_controller controller;	/*belong to which controller*/
	unsigned int base_addr;             /*device config space base addr & mem-io space base addr*/
	unsigned int conf_base_addr;        /*rc config space base addr*/
	unsigned int stat0;
};
static struct pcie_info pcie_info[2];
static int pcie_controllers_nr = 0;
/*pcie0 & pcie1 sel:*/
//static unsigned int pcie0_sel = pcie_sel_none;	/*default controller is pcie0 */
/* <DTS2012110301136 l00205975 20121103 begin */
static unsigned int pcie0_sel = pcie_sel_none;//pcie0_x1_sel;	/*default controller is pcie0 */
/* DTS2012110301136 l00205975 20121103 end> */
static unsigned int pcie1_sel = pcie_sel_none;//pcie1_x1_sel;	/*pcie1 controller is not selected*/

/*work mode */
static unsigned int pcie0_work_mode = pcie_wm_rc; /*default work mode is rc.*/
static unsigned int pcie1_work_mode = pcie_wm_rc; /*default work mode is rc.*/

/*mem space size*/
static unsigned int pcie0_mem_space_size = 0xa00000;	/*default mem space size: 10M*/
static unsigned int pcie1_mem_space_size = 0xa00000;	/*default mem space size: 10M*/

/*sys controller map address*/
static void* __iomem pcie_sys_base_virt = 0;

/*locks: read lock & write lock.*/
static spinlock_t rlock = SPIN_LOCK_UNLOCKED;
static spinlock_t wlock = SPIN_LOCK_UNLOCKED;

struct pcie_iatu pcie0_iatu_table[] = {

    {
        /* 配置操作的iATU寄存器初始化 */
		.viewport	    = 0x0,          //view index
		.region_ctrl_1	= 0x4,          //ctrl 1
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x50000000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x57FFFFFF,   //limit
		.ltar		    = 0x01000000,   //target lower
		.utar		    = 0x0,          //target upper
	},

    {
        /* 配置操作的iATU寄存器初始化 */
		.viewport	    = 0x1,          //view index
		.region_ctrl_1	= 0x2,          //ctrl 1 io 64k
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x40400000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x404000ff,   //limit
		.ltar		    = 0x40400000,   //target lower
		.utar		    = 0x0,          //target upper
	},
	
    {
        /* 配置操作的iATU寄存器初始化 */
		.viewport	    = 0x2,          //view index
		.region_ctrl_1	= 0x0,          //ctrl 1 mem 0x400000
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x40000000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x403FFFFF,   //limit
		.ltar		    = 0x40000000,   //target lower
		.utar		    = 0x0,          //target upper
	},



};
struct pcie_iatu pcie1_iatu_table[] = {

    {
        /* 配置操作的iATU寄存器初始化 */
		.viewport	    = 0x0,          //view index
		.region_ctrl_1	= 0x4,          //ctrl 1
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x68000000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x68FFFFFF,   //limit
		.ltar		    = 0x01000000,   //target lower
		.utar		    = 0x0,          //target upper
	},

    {
        /* 配置操作的iATU寄存器初始化 外设配置空间 */
		.viewport	    = 0x1,          //view index
		.region_ctrl_1	= 0x2,          //ctrl 1 io 64k
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x58400000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x584000ff,   //limit
		.ltar		    = 0x58400000,   //target lower
		.utar		    = 0x0,          //target upper
	},
	
    {
        /* 配置操作的iATU寄存器初始化 io操作 外设内部寄存器*/ 
		.viewport	    = 0x2,          //view index
		.region_ctrl_1	= 0x0,          //ctrl 1 mem 0x400000
		.region_ctrl_2	= 0x80000000,   //ctrl 2
		.lbar		    = 0x58000000,   //base lower
		.ubar		    = 0x0,          //base upper
		.lar		    = 0x583FFFFF,   //limit
		.ltar		    = 0x58000000,   //target lower
		.utar		    = 0x0,          //target upper
	},



};
/*format : pcie0_mem=0xa00000;pcie1_mem=0xa00000*/
static int __init pcie0_mem_parser(char* str){
	unsigned int size;
	size = (unsigned int)simple_strtoul(str, NULL, 16);
	/*if size > 16M, set default 10M*/
	if(size > 0x1000000)
		size = 0xa00000;
	pcie0_mem_space_size = size;

	return 1;	
}
__setup("pcie0_mem=", pcie0_mem_parser);

static int __init pcie1_mem_parser(char* str){
	unsigned int size;
	size = (unsigned int)simple_strtoul(str, NULL, 16);
	/*if size > 16M, set default 10M*/
	if(size > 0x1000000)
		size = 0xa00000;
	pcie1_mem_space_size = size;

	return 1;	
}
__setup("pcie1_mem=", pcie1_mem_parser);
/*pcie sel boot args format: pcie0_sel=x1 pcie1=x1 or pcie1=x2. 
  any other value after "pcieX_sel=" prefix  will be treated as none controller will be selected.
  e.g. pcie0_sel=none will be treated as you will not selected pcie0 
 */
static int __init pcie0_sel_parser(char* str){
	if(strncasecmp(str, "x1",2) == 0)
		pcie0_sel = pcie0_x1_sel;
	else
		pcie0_sel = pcie_sel_none;

	return 1;
}
__setup("pcie0_sel=", pcie0_sel_parser);

static int __init pcie1_sel_parser(char* str){
	if(strncasecmp(str, "x1", 2) == 0)
		pcie1_sel = pcie1_x1_sel;
	else
		pcie1_sel = pcie_sel_none;

	return 1;
}
__setup("pcie1_sel=", pcie1_sel_parser);
/*work mode format: pcie0_wm=rc/ep/lep pcie1_wm=rc/ep/lep*/
static int __init pcie0_work_mode_parser(char* str){
	if(strncasecmp(str, "rc", 2) == 0)
		pcie0_work_mode = pcie_wm_rc;
	else if(strncasecmp(str, "ep", 2) == 0)
		pcie0_work_mode = pcie_wm_ep;
	else if(strncasecmp(str, "lep", 3) == 0)
		pcie0_work_mode = pcie_wm_lep;
	else 
		pcie_error("Invalid pcie_wm value for pcie0!\n");
	return 1;
}
__setup("pcie0_wm=", pcie0_work_mode_parser);

static int __init pcie1_work_mode_parser(char* str){
	if(strncasecmp(str, "rc", 2) == 0)
		pcie1_work_mode = pcie_wm_rc;
	else if(strncasecmp(str, "ep", 2) == 0)
		pcie1_work_mode = pcie_wm_ep;
	else if(strncasecmp(str, "lep", 3) == 0)
		pcie1_work_mode = pcie_wm_lep;
	else 
		pcie_error("Invalid pcie_wm value for pcie1!\n");
	return 1;
}
__setup("pcie1_wm=", pcie1_work_mode_parser);
#define PCIE0_MODE_SEL  (1 << 0)
#define PCIE1_MODE_SEL  (1 << 1)
static int __init pcie_sys_init(unsigned int mode_sel){
	unsigned int val;
	/*setup pcie ip, need spinlock??*/
	/*setup slot parity*/

	val = readl(pcie_sys_base_virt +  PERI_PCIE_STAT0 );
	//printk("%s->%d\n  PERI_PCIE_STAT0=0x%x \n",__func__,__LINE__,val);
	val = readl(pcie_sys_base_virt +  PERI_PCIE_STAT1 );
	//printk("%s->%d\n  PERI_PCIE_STAT1=0x%x \n",__func__,__LINE__,val);
	
	//if ((mode_sel & (PCIE0_MODE_SEL | PCIE1_MODE_SEL)) == (PCIE0_MODE_SEL | PCIE1_MODE_SEL))
		//??? writel(0x1, pcie_sys_base_virt + PERI_PCIE23);
	//else
		//??? writel(0, pcie_sys_base_virt + PERI_PCIE23);
	/*setup mode*/
	/*
	   pcie1               pcie0           
	   m0  selected(X2)        not selected    
	   m1  no selected         selected(X1)    
	   m2  selected(X1)        not selected    
	  *m3  selected(X1)        selected(X1)    

	   mode0   <--> pcie_mode_sel[1:0] = 2'b00;
	   mode1   <--> pcie_mode_sel[1:0] = 2'b01;
	   mode2   <--> pcie_mode_sel[1:0] = 2'b10;
	   mode3   <--> pcie_mode_sel[1:0] = 2'b11;
	 */

	//val = readl(pcie_sys_base_virt + PERI_CTRL);
	//val &= (~(0x3 << pcie_mode_sel));
	//val |= ((pcie1_sel == pcie1_x2_sel ? 0 : mode_sel) << pcie_mode_sel);
	//writel(val, pcie_sys_base_virt + PERI_CTRL);
    

	if((mode_sel & PCIE0_MODE_SEL) == PCIE0_MODE_SEL){
		void* dbi_base = ioremap_nocache(DBI_BASE_ADDR_0, 0x1000);/*IO_ADDRESS??*/
		if(!dbi_base){
			pcie_error("Cannot map dbi base for pcie0");
			return -EIO;
		}
//himm 0x101000d8 0x0
    	/* disable controller first */ 
    	val = readl(pcie_sys_base_virt + PERI_PCIE7);
    	val = 0x0;
    	writel(val, pcie_sys_base_virt + PERI_PCIE7);
    //himm 0x101000c0 0x200000
    //himm 0x101000d4 0x8000000
    	val = readl(pcie_sys_base_virt + PERI_PCIE1);
    	val = 0x200000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1);

    	val = readl(pcie_sys_base_virt + PERI_PCIE6);
    	val = 0x8000000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE6);
        
    //himm 0x101000dc 0x5605001
    //himm 0x101000e0 0x20050006
    	/* setup phy */
    	writel(PCIE_PHY_VALUE0, pcie_sys_base_virt + PERI_PCIE8);
    	writel(PCIE_PHY_VALUE1, pcie_sys_base_virt + PERI_PCIE9);

    	if ((mode_sel & PCIE0_MODE_SEL) == PCIE0_MODE_SEL) {

    		int i;

    //himm 0x101000bc 0x40200000
    		/* set work mode */
    		val = readl(pcie_sys_base_virt + PERI_PCIE0);
    		val = 0x40200000;
    		writel(val, pcie_sys_base_virt + PERI_PCIE0);

            val = readl(dbi_base + 0x4);
    		val = (0x100007);
    		writel(val, dbi_base + 0x4);

    		/* setup iatu table */
    		for (i = 0; i < ARRAY_SIZE(pcie0_iatu_table); i++) {
    			writel(pcie0_iatu_table[i].viewport,
    					dbi_base + 0x700 + 0x200);
    			writel(pcie0_iatu_table[i].region_ctrl_1,
    					dbi_base + 0x700 + 0x204);
    			writel(pcie0_iatu_table[i].region_ctrl_2,
    					dbi_base + 0x700 + 0x208);
    			writel(pcie0_iatu_table[i].lbar,
    					dbi_base + 0x700 + 0x20c);
    			writel(pcie0_iatu_table[i].ubar,
    					dbi_base + 0x700 + 0x210);
    			writel(pcie0_iatu_table[i].lar,
    					dbi_base + 0x700 + 0x214);
    			writel(pcie0_iatu_table[i].ltar,
    					dbi_base + 0x700 + 0x218);
    			writel(pcie0_iatu_table[i].utar,
    					dbi_base + 0x700 + 0x21c);
    		}
    	}
    //himm 0x101000d8 0x800
    	/* enable controller */
    	if (mode_sel & PCIE0_MODE_SEL) {
    		val = readl(pcie_sys_base_virt + PERI_PCIE7);
    		val |= ((1 << pcie0_app_ltssm_enable) | 0x02200000);
    		writel(val, pcie_sys_base_virt + PERI_PCIE7);
    	}
    	udelay(1000);

    //himm 0x101000bc 0x40000000
    //himm 0x101000c0 0x0 
    	val = readl(pcie_sys_base_virt + PERI_PCIE0);
    	val = 0x40000000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE0);
    //发操作之前确保link up
    	udelay(1000);
        val = readl(pcie_sys_base_virt + 0x18);
        if ( (val & 0x20) && (val & 0x8000 ))
        {
            printk("PCIE0 Device Link Up val = 0x%08x.\n",val);
        	val = readl(pcie_sys_base_virt + PERI_PCIE1);
        	val = 0x0;
        	writel(val, pcie_sys_base_virt + PERI_PCIE1);    
                    
        }
        else
        {
            printk("PCIE0 Device Link Down val = 0x%08x.\n",val);
            return -1;
        }        
	}
	if((mode_sel & PCIE1_MODE_SEL) == PCIE1_MODE_SEL){
		void* dbi_base = ioremap_nocache(DBI_BASE_ADDR_1, 0x1000);/*IO_ADDRESS??*/
		if(!dbi_base){
			pcie_error("Cannot map dbi base for pcie1");
			return -EIO;
		}
//himm 0x101000d8 0x0
    	/* disable controller first */ 
    	val = readl(pcie_sys_base_virt + PERI_PCIE1_7);
    	val = 0x0;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1_7);
    //himm 0x101000c0 0x200000
    //himm 0x101000d4 0x8000000
    	val = readl(pcie_sys_base_virt + PERI_PCIE1_1);
    	val = 0x200000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1_1);

    	val = readl(pcie_sys_base_virt + PERI_PCIE1_6);
    	val = 0x8000000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1_6);
        
    //himm 0x101000dc 0x5605001
    //himm 0x101000e0 0x20050006
    	/* setup phy */
    	writel(PCIE_PHY_VALUE0, pcie_sys_base_virt + PERI_PCIE1_8);
    	writel(PCIE_PHY_VALUE1, pcie_sys_base_virt + PERI_PCIE1_9);

    	if ((mode_sel & PCIE1_MODE_SEL) == PCIE1_MODE_SEL) {

    		int i;

    //himm 0x101000bc 0x40200000
    		/* set work mode */
    		val = readl(pcie_sys_base_virt + PERI_PCIE1_0);
    		val = 0x40200000;
    		writel(val, pcie_sys_base_virt + PERI_PCIE1_0);

            val = readl(dbi_base + 0x4);
    		val = (0x100007);
    		writel(val, dbi_base + 0x4);



    		/* setup iatu table */
    		for (i = 0; i < ARRAY_SIZE(pcie1_iatu_table); i++) {
    			writel(pcie1_iatu_table[i].viewport,
    					dbi_base + 0x700 + 0x200);
    			writel(pcie1_iatu_table[i].region_ctrl_1,
    					dbi_base + 0x700 + 0x204);
    			writel(pcie1_iatu_table[i].region_ctrl_2,
    					dbi_base + 0x700 + 0x208);
    			writel(pcie1_iatu_table[i].lbar,
    					dbi_base + 0x700 + 0x20c);
    			writel(pcie1_iatu_table[i].ubar,
    					dbi_base + 0x700 + 0x210);
    			writel(pcie1_iatu_table[i].lar,
    					dbi_base + 0x700 + 0x214);
    			writel(pcie1_iatu_table[i].ltar,
    					dbi_base + 0x700 + 0x218);
    			writel(pcie1_iatu_table[i].utar,
    					dbi_base + 0x700 + 0x21c);
    		}
    	}
    //himm 0x101000d8 0x800
    	/* enable controller */
    	if (mode_sel & PCIE1_MODE_SEL) {
    		val = readl(pcie_sys_base_virt + PERI_PCIE1_7);
    		val |= ((1 << pcie1_app_ltssm_enable) | 0x02200000);
    		writel(val, pcie_sys_base_virt + PERI_PCIE1_7);
    	}
    	udelay(1000);

    //himm 0x101000bc 0x40000000
    //himm 0x101000c0 0x0 
    	val = readl(pcie_sys_base_virt + PERI_PCIE1_0);
    	val = 0x40000000;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1_0);
    //发操作之前确保link up
    	udelay(1000);
        val = readl(pcie_sys_base_virt + 0x28);
        if ( (val & 0x20) && (val & 0x8000 ))
        {
            printk("PCIE1 Device Link Up val = 0x%08x.\n",val);
        	val = readl(pcie_sys_base_virt + PERI_PCIE1_1);
        	val = 0x0;
        	writel(val, pcie_sys_base_virt + PERI_PCIE1_1);    
                    
        }
        else
        {
            printk("PCIE1 Device Link Down val = 0x%08x.\n",val);
            return -1;
        }
        
	}

	return 0;
}
static struct resource pcie0_mem_space = {
	.name   = "PCIE0 memory space",
	.start  = PCIE0_BASE_ADDR_PHYS,
	.end    = 0, 
	.flags  = IORESOURCE_MEM,
};
static struct resource pcie0_io_space = {
	.name   = "PCIE0 io space",
	.start  = 0,
	.end    = 0, 
	.flags  = IORESOURCE_IO,
};

static struct resource pcie1_mem_space = {
	.name   = "PCIE1 memory space",
	.start  = PCIE1_BASE_ADDR_PHYS,
	.end    = 0, 
	.flags  = IORESOURCE_MEM,
};
static struct resource pcie1_io_space = {
	.name   = "PCIE1 io space",
	.start  = 0,
	.end    = 0, 
	.flags  = IORESOURCE_IO,
};
static void __init pcie_preinit(void){
	/*setup resource*/
	pcie0_mem_space.end = PCIE0_BASE_ADDR_PHYS + pcie0_mem_space_size - 1;
	pcie0_io_space.start = pcie0_mem_space.end+1 ; 
	pcie0_io_space.end = PCIE0_BASE_ADDR_PHYS + PCIE_BASE_ADDR_SIZE - 1; 

	pcie1_mem_space.end = PCIE1_BASE_ADDR_PHYS + pcie1_mem_space_size - 1;
	pcie1_io_space.start = pcie1_mem_space.end +1;
	pcie1_io_space.end = PCIE1_BASE_ADDR_PHYS + PCIE_BASE_ADDR_SIZE - 1;

}

static int __init pcie_setup(int nr, struct pci_sys_data* sys){
	struct pcie_info* info;
	int ret;

	if(nr >= pcie_controllers_nr)	
		return 0;

	info = &pcie_info[nr];
	/*record busnr for cfg ops use*/
	info->root_bus_nr = sys->busnr;/*do i need to set busnr to pcie controller???,if so, how??*/
	sys->mem_offset = 0;   /*what's this? is't right?*/

	/*requeset resources for corresponding controller*/
	if(info->controller == pcie_controller_0){
		ret = request_resource(&ioport_resource, &pcie0_io_space);
		if(ret){
			pcie_error("Cannot request io resource for pcie0");
			return ret;
		}
		ret = request_resource(&iomem_resource, &pcie0_mem_space);
		if(ret){
			pcie_error("Cannot request mem resource for pcie0");
			release_resource(&pcie0_io_space);
			return ret;
		}
		sys->resource[0] = &pcie0_io_space;
		sys->resource[1] = &pcie0_mem_space;
	}

	if(info->controller == pcie_controller_1){
		ret = request_resource(&ioport_resource, &pcie1_io_space);
		if(ret){
			pcie_error("Cannot requeset io resource for pcie1");
			return ret;
		}
		ret = request_resource(&iomem_resource, &pcie1_mem_space);
		if(ret){
			pcie_error("Cannot requeset mem resource for pcie1");
			release_resource(&pcie1_io_space);
			return ret;
		}
		sys->resource[0] = &pcie1_io_space;
		sys->resource[1] = &pcie1_mem_space;
	}

	sys->resource[2] = NULL;

	return 1;
}
static struct pcie_info* __init bus_to_info(int busnr){
	int i = pcie_controllers_nr - 1;
	for( ; i >= 0; i--){
		if(pcie_info[i].controller != pcie_controller_none 
				&& pcie_info[i].root_bus_nr <= busnr
				&& pcie_info[i].root_bus_nr != -1)
			return &pcie_info[i];
	}
	return NULL;
}
static int __init pcie_map_irq(struct pci_dev* dev, u8 slot, u8 pin){
	struct pcie_info* info = bus_to_info(dev->bus->number);
	if(!info){
		pcie_error("Cannot find corresponding controller for appointed device!");
		BUG();
		return -1;
	}
	if(info->controller == pcie_controller_0){
		switch(pin){
			case PCIE_INTA_PIN: return PCIE0_IRQ_INTA;
			case PCIE_INTB_PIN: return PCIE0_IRQ_INTB;
			case PCIE_INTC_PIN: return PCIE0_IRQ_INTC;
			case PCIE_INTD_PIN: return PCIE0_IRQ_INTD;
			default :
				pcie_error("Unkown pin for mapping irq!");
				return -1;
		}
	}	
	if(info->controller == pcie_controller_1){
		switch(pin){
			case PCIE_INTA_PIN: return PCIE1_IRQ_INTA;
			case PCIE_INTB_PIN: return PCIE1_IRQ_INTB;
			case PCIE_INTC_PIN: return PCIE1_IRQ_INTC;
			case PCIE_INTD_PIN: return PCIE1_IRQ_INTD;
			default :
				pcie_error("Unkown pin for mapping irq!");
				return -1;
		}
	}
	pcie_error("Why I'm here??");
	BUG();
	return -1;
}
#define PCIE_CFG_DEV(devfn)	((devfn)  << 16)
#define PCIE_CFG_REG(reg)	((reg & 0xffc))/*set dword align*/
static inline unsigned int to_pcie_address(struct pci_bus* bus,unsigned int devfn, int where){
	struct pcie_info* info = bus_to_info(bus->number);
	unsigned int address = 0;
	if(!info){
		pcie_error("Cannot find corresponding controller for appointed device!");
		BUG();
	}

	address = info->base_addr |  PCIE_CFG_DEV(devfn) | PCIE_CFG_REG(where);
	return address;
}
static inline int pcie_check_link_status(struct pcie_info* info){
	int val;
	val = readl(pcie_sys_base_virt + (info->controller == pcie_controller_0 ? PERI_PCIE_STAT0 : PERI_PCIE_STAT1));
	//printk("%s->%d  PERI_PCIE_STAT0=0x%x \n",__func__,__LINE__,val);
	info->stat0 =val;
	val &= (1 << pciex_link_up);
	return (val == (1 << pciex_link_up)) ? 1 : 0;
}
static int pcie_read_from_device(struct pci_bus* bus, unsigned int devfn, int where, int size, u32* value){
	struct pcie_info* info = bus_to_info(bus->number);
	unsigned int val;
	unsigned int addr;
	unsigned long flag;
	int i;
	for (i = 0; i < 1000; i++){
		if(pcie_check_link_status(info)){
			//printk("%s->%d  PERI_PCIE_STAT0=0x%x \n",__func__,__LINE__,info->stat0);
			break;
		}
		udelay(1000);
	}
	if(i >= 1000){
		//pcie_debug(PCIE_DEBUG_LEVEL_MODULE, "pcie%d not link up!",info->controller == pcie_controller_0 ? 0: 1);
		printk( "pcie%d not link up!",info->controller == pcie_controller_0 ? 0: 1);
		return -1;
	}
	spin_lock_irqsave(&rlock, flag);
	/*read device is allways type1*/

	addr = to_pcie_address(bus, devfn, where);
	val = readl(addr);
		
	/*if got data is dword align, and val got from offset 0, i need to calculate which byte is wanted*/
	/*is this right? need to check*/
	if(size == 1)
		*value = ((val >> ((where & 0x3) << 3)) & 0xff);
	else if(size == 2)
		*value = ((val >> ((where & 0x3) << 3)) & 0xffff);
	else if(size == 4)
		*value = val;
	else{
		pcie_error("Unkown size(%d) for read ops\n",size);
		BUG();
	}

	spin_unlock_irqrestore(&rlock, flag);

	return PCIBIOS_SUCCESSFUL;
}
static int pcie_read_from_dbi(struct pcie_info* info, unsigned int devfn, int where, int size, u32* value){
	unsigned long flag;
	u32 v;
	/*for host-side config space read, ignore device func nr.*/
	if(devfn > 0)
		return -EIO;

	spin_lock_irqsave(&rlock, flag);
	v = (u32)readl((void*)(info->conf_base_addr + (where & (~0x3))));

	if(1 == size)
		*value = (v >> ((where & 0x3) << 3)) & 0xff;
	else if(2 == size)
		*value = (v >> ((where & 0x3) << 3)) & 0xffff;
	else if(4 == size)
		*value = v;
	else{
		pcie_error("Unkown size for config read operation!");
		BUG();
	}

	spin_unlock_irqrestore(&rlock, flag);

	return PCIBIOS_SUCCESSFUL; 
}
static int pcie_read_conf(struct pci_bus* bus, unsigned int devfn, int where, int size, u32* value){
	struct pcie_info* info = bus_to_info(bus->number);
	int ret;
	if(unlikely(!info)){
		pcie_error("Cannot find corresponding controller for appointed device!");
		BUG();
	}
	if(bus->number == info->root_bus_nr){
		ret = pcie_read_from_dbi(info, devfn, where, size, value);
		//printk("dbi ");
	}
	else{
		if(PCI_SLOT(devfn) > 0)
			return -EIO;
		ret = pcie_read_from_device(bus, devfn, where, size, value);
		if(ret!=PCIBIOS_SUCCESSFUL)
			{printk("devE pcie_read_conf");}
		//else
			//{
			//printk("dev ");
			//}
	}
	//printk("pcie_rdconf,bus:0x%x,dev:0x%x,off:0x%x,size:0x%x,val:0x%x\n",bus->number,devfn,where,size,*value);
	return ret;
}
static int pcie_write_to_device(struct pci_bus* bus, unsigned int devfn, int where, int size, u32 value){
	struct pcie_info* info = bus_to_info(bus->number);
	unsigned int addr;
	unsigned int org;
	unsigned int val;
	unsigned long flag;
	if(!pcie_check_link_status(info)){
		pcie_debug(PCIE_DEBUG_LEVEL_MODULE, "pcie%d not link up!",info->controller == pcie_controller_0 ? 0: 1);
		return -1;
	}

	spin_lock_irqsave(&wlock, flag);	
	pcie_read_from_device(bus, devfn, where, 4, &org);


	addr = to_pcie_address(bus, devfn, where);

	if(size == 1){
		org &= (~(0xff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	}else if(size == 2){
		org &= (~(0xffff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	}else if(size == 4){
		org = value;
	}else{
		pcie_error("Unkown size(%d) for read ops\n",size);
		BUG();	
	}
	writel(org, addr);

	spin_unlock_irqrestore(&wlock, flag);
	return PCIBIOS_SUCCESSFUL;

}
static int pcie_write_to_dbi(struct pcie_info* info, unsigned int devfn, int where, int size, u32 value){
	unsigned long flag;
	unsigned int org;


	spin_lock_irqsave(&wlock, flag);

	if(pcie_read_from_dbi(info, devfn, where, 4, &org)){
		pcie_error("Cannot read from dbi! 0x%x:0x%x:0x%x!",0, devfn, where);
		spin_unlock_irqrestore(&wlock, flag);
		return -EIO;
	}
	if(size == 1){
		org &= (~(0xff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	}else if(size == 2){
		org &= (~(0xffff << ((where & 0x3) << 3)));
		org |= (value << ((where & 0x3) << 3));
	}else if(size == 4){
		org = value;
	}else{
		pcie_error("Unkown size(%d) for read ops\n",size);
		BUG();	
	}
	writel(org, info->conf_base_addr + (where & (~0x3)));


	spin_unlock_irqrestore(&wlock, flag);
	return PCIBIOS_SUCCESSFUL;
}
static int pcie_write_conf(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 value){
	struct pcie_info* info = bus_to_info(bus->number);
	if(unlikely(!info)){
		pcie_error("Cannot find corresponding controller for appointed device!");
		BUG();
	}
	//printk("pcie_wrconf,bus:0x%x,dev:0x%x,off:0x%x,size:0x%x,val:0x%x  ",bus->number,devfn,where,size,value);
	if(bus->number == info->root_bus_nr){
		//printk("  dbi\n");
		return pcie_write_to_dbi(info, devfn, where, size, value);
	}
	else{
		if(PCI_SLOT(devfn) > 0)
			return -EIO;
		//printk("  dev\n");
		return pcie_write_to_device(bus, devfn, where, size, value);
	}
}
static struct pci_ops pcie_ops = {
	.read = pcie_read_conf,
	.write = pcie_write_conf,
};

static struct pci_bus *__init pcie_scan_bus(int nr, struct pci_sys_data* sys){
	struct pci_bus *bus;
	if (nr < pcie_controllers_nr) {
		bus = pci_scan_bus(sys->busnr, &pcie_ops, sys);
	} else {
		bus = NULL;
		pcie_error("Unkown controller nr :0x%x!",nr);
		BUG();
	}

	return bus;
}

static struct hw_pci sd5115_pcie = {
	.nr_controllers = 2,        /*default 2 controllers*/
	.preinit    = pcie_preinit,
	.swizzle    = pci_std_swizzle,	/*useless??*/
	.setup      = pcie_setup,
	.scan       = pcie_scan_bus,
	.map_irq    = pcie_map_irq,
};
static int __init pcie_init(void){
	unsigned int mode_sel = 0;
	unsigned int val;
	/*first, check param*/
	//if(pcie1_sel == pcie1_x2_sel && pcie0_sel != pcie_sel_none){
	//	pcie_error("While pcie1 is selected to connect with an x2 device, pcie0 cannot connect with any device");
	//	return -EINVAL;
	//}
    if(HI_CHIP_ID_S_E==hi_kernel_get_chip_id()) 
    return;

	printk("pcie0 sel: 0x%x, pcie1 sel:0x%x,pcie0 memsize:0x%x, pcie1 memsize:0x%x.\n", 
			pcie0_sel, pcie1_sel, pcie0_mem_space_size, pcie1_mem_space_size);

	pcie_sys_base_virt = ioremap_nocache(PCIE_SYS_BASE_PHYS, 0x1000);	/*maybe IO_ADDRESS is better*/
	if(!pcie_sys_base_virt){
		pcie_error("Cannot map system controller register base!");
		return -EIO;
	}

	pcie_info[0].root_bus_nr = -1;
	pcie_info[1].root_bus_nr = -1;

	/*if enabled pcie0*/
	if(pcie0_sel == pcie0_x1_sel){
		/*work at rc mode*/
		if(pcie0_work_mode == pcie_wm_rc){
			pcie_info[pcie_controllers_nr].controller = pcie_controller_0;
			pcie_info[pcie_controllers_nr].conf_base_addr = (unsigned int)ioremap_nocache(PCIE0_MEMIO_BASE, 0x1000);
			if(!pcie_info[pcie_controllers_nr].conf_base_addr){
				pcie_error("Cannot map host's dbi space for controller0!");
				goto err_conf_base_0;
			}
			pcie_info[pcie_controllers_nr].base_addr = (unsigned int)ioremap_nocache(PCIE0_BASE_ADDR_PHYS, 0x1000000); 
			if(!pcie_info[pcie_controllers_nr].base_addr){
				pcie_error("Cannot map config space base for controller0!");
				goto err_base_0;
			}

			pcie_controllers_nr++;
			mode_sel |= PCIE0_MODE_SEL;
		}
		/*work at ep mode*/
		else{
			/*FIXME:
			  This is roughly initializtion of ep mode.
			  Maybe I should to create a new func to initialize ep  mode!
TODO: Do reset and enable controllers after set corresponding work mode.
			 */
			unsigned int val = readl(pcie_sys_base_virt + PERI_PCIE0);
			val &= (~(0xf << pcie0_slv_device_type));
			writel(val, pcie_sys_base_virt + PERI_PCIE0);
		}
	}
	/*if enabled pcie1*/
	if(pcie1_sel == pcie1_x1_sel ){
		if( pcie1_work_mode == pcie_wm_rc){
			pcie_info[pcie_controllers_nr].controller = pcie_controller_1;
			pcie_info[pcie_controllers_nr].conf_base_addr = (unsigned int)ioremap_nocache(PCIE1_MEMIO_BASE, 0x1000);
			if(!pcie_info[pcie_controllers_nr].conf_base_addr){
				pcie_error("Cannot map host's dbi space for controller1!");
				goto err_conf_base_1; 
			}
			pcie_info[pcie_controllers_nr].base_addr = (unsigned int)ioremap_nocache(PCIE1_BASE_ADDR_PHYS, 0x1000000);
			if(!pcie_info[pcie_controllers_nr].base_addr){
				pcie_error("Cannot map config space base for controller1!");
				goto err_base_1;
			}

			pcie_controllers_nr++;
			mode_sel |= PCIE1_MODE_SEL;
		}
		/*ep mode*/
		else{
			/*FIXME:
			  This is roughly initializtion of ep mode.
			  Maybe I should to create a seperate func to initialize rc mode!
TODO: Do reset and enable controllers after set corresponding work mode.
			 */
			unsigned int val = readl(pcie_sys_base_virt + PERI_PCIE1_0);
			val &= (~(0xf << pcie1_slv_device_type));
			writel(val, pcie_sys_base_virt + PERI_PCIE1_0);
		}
	}
	/*none of the two controllers work at rc mode or is enabled, we do nothing*/
	if(!pcie_controllers_nr){
		pcie_error("None of the two pcie controllers is enabled!");
		goto out;
	}
	if(pcie_sys_init(mode_sel)){
		goto out; 
	}

	/*reset how many controllers to enable.*/
	sd5115_pcie.nr_controllers = pcie_controllers_nr;
	pci_common_init(&sd5115_pcie);
	return 0;

out:
err_base_1:
	if(pcie1_sel != pcie_sel_none)
		iounmap((void*)pcie_info[pcie_controllers_nr].conf_base_addr);
err_conf_base_1:
	if(pcie0_sel == pcie0_x1_sel)
		iounmap((void*)pcie_info[0].base_addr);
err_base_0:
	if(pcie0_sel == pcie0_x1_sel)
		iounmap((void*)pcie_info[0].conf_base_addr);
err_conf_base_0:
	if(pcie_sys_base_virt)
		iounmap(pcie_sys_base_virt);

	return -EIO;
}

EXPORT_SYMBOL(pcie_sys_init);
subsys_initcall(pcie_init); 


