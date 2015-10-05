/* pcie0 iatu table */
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

int pcie_sys_init(unsigned int mode_sel)
{
	unsigned int val;
		void *dbi_base = (void *)pcie_info[0].conf_base_addr;

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
        printk("PCIE Device Link Up val = 0x%08x.\n",val);
    	val = readl(pcie_sys_base_virt + PERI_PCIE1);
    	val = 0x0;
    	writel(val, pcie_sys_base_virt + PERI_PCIE1);    
		if (mode_sel & PCIE0_MODE_SEL) 
        {
			void *dbi_base;
			unsigned int val;
			dbi_base = (void *)pcie_info[0].conf_base_addr;
		}
                
    }
    else
    {
        printk("PCIE Device Link Down val = 0x%08x.\n",val);
        return -1;
    }

	return 0;
}

#ifdef CONFIG_PM
int hisi_pcie_plat_driver_suspend(struct device *dev)
{
	void *crg_base = (void *)IO_ADDRESS(REG_BASE_CRG);
	unsigned int val;
	/* disable controller first */
	val = readl(pcie_sys_base_virt + PERI_PCIE7);
	val &= (~(1 << pcie0_app_ltssm_enable));
	writel(val, pcie_sys_base_virt + PERI_PCIE7);

	/* soft reset */
	val = readl(crg_base + PERI_CRG37);;
	val |= PERI_CRG37_PCIE0_SRST_REQ;
	val |= PERI_CRG37_PCIE0_RST_REQ;
	val |= PERI_CRG37_X2X_PCIE0_SRST_REQ;
	val |= PERI_CRG37_X2X_PCIES_SRST_REQ;
	val |= PERI_CRG37_PCIE_SLOT0_SRST_REQ;
	writel(val, crg_base + PERI_CRG37);
	udelay(1000);

	/* disable clock */
	val = readl(crg_base + PERI_CRG37);
	val &= (~PERI_CRG37_PCIE0_CKEN);
	val &= (~PERI_CRG37_PCIE_PHY_CKEN);
	val &= (~PERI_CRG37_X2X_PCIES_CKEN);
	val &= (~PERI_CRG37_PCIE_SLOT_CKEN);
	writel(val, crg_base + PERI_CRG37);
	udelay(1000);
	return 0;
}

int hisi_pcie_plat_driver_resume(struct device *pdev)
{
	return pcie_sys_init(hisi_pcie_mode_sel);
}

const struct dev_pm_ops hisi_pcie_pm_ops = {
	.suspend = NULL,
	.suspend_noirq = hisi_pcie_plat_driver_suspend,
	.resume = NULL,
	.resume_noirq = hisi_pcie_plat_driver_resume
};
#define HISI_PCIE_PM_OPS (&hisi_pcie_pm_ops)
#else
#define HISI_PCIE_PM_OPS NULL
#endif
