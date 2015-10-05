
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include <asm/cacheflush.h>
#include <asm/irq.h>

#include <mach/irqs.h>
#include <mach/platform.h>
#include <mach/early-debug.h>
#include <mach/cache-hil2.h>

#include <mach/kexport.h>

#define CACHE_LINE_SIZE		L2_LINE_SIZE

#define DRIVER_NAME "l2cache"

static void __iomem *l2cache_base;
static int l2cache_flag = 0;
static int l2cache_en=1;

static inline void sync_writel(unsigned long val, unsigned long reg,
			       unsigned long complete_mask)
{
	writel(val, l2cache_base + reg);
}

static inline void cache_sync(void)
{
    unsigned int reg;
    reg = readl(l2cache_base + REG_L2_SYNC);  
}

/******************************************************************************
*
* l2_invalid_auto -  auto invalid l2 cache
* 
* This routine auto invalid l2 cache 
*	
*******************************************************************************/ 
static void l2_invalid_auto(unsigned int way_num)      /* need invalid cache way num */
{
    unsigned int reg;
    
    cache_sync();
    reg = (way_num << BIT_L2_MAINT_AUTO_WAYADDRESS) | (0x1 << BIT_L2_MAINT_AUTO_START);
    sync_writel(reg, REG_L2_MAINT_AUTO,reg );
    while(!( readl(l2cache_base + REG_L2_RINT) & ( 0X1 << BIT_L2_RINT_AUTO_END ))); 
    reg = readl(l2cache_base + REG_L2_RINT);
    sync_writel(reg, REG_L2_INTCLR,reg);
    cache_sync();
}

/******************************************************************************
 *
 * l2_cache_auto-  auto clean l2 cache
 *
 * This routine auto clean l2 cache
 *
 *******************************************************************************/
static void l2_cache_auto(unsigned int way_num)      /* need clean cache way num */
{
	unsigned int reg;

	cache_sync();
	reg = (way_num << BIT_L2_MAINT_AUTO_WAYADDRESS) | (0x1 << BIT_L2_MAINT_AUTO_START | 0x2);
	sync_writel(reg, REG_L2_MAINT_AUTO,reg );
	while(!( readl(l2cache_base + REG_L2_RINT) & ( 0X1 << BIT_L2_RINT_AUTO_END )));
	reg = readl(l2cache_base + REG_L2_RINT);
	sync_writel(reg, REG_L2_INTCLR,reg);
	cache_sync();
}     

void l2cache_inv_all(void)
{
	unsigned int way_num;
	
	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}

	cache_sync();
	/*invalidate cache  all-way*/
	for(way_num = 0; way_num < L2_WAY_NUM; way_num ++)
	{
		l2_invalid_auto(way_num);
	}
	cache_sync();
}
EXPORT_SYMBOL(l2cache_inv_all);

void l2cache_clean_all(void)
{
	unsigned int way_num;

	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}

	cache_sync();
	/*invalidate cache  all-way*/
	for(way_num = 0; way_num < L2_WAY_NUM; way_num ++)
	{
		l2_cache_auto(way_num);
	}
	cache_sync();
}
EXPORT_SYMBOL(l2cache_clean_all);

void l2cache_flush_all(void)
{
	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}
	l2cache_clean_all();
	l2cache_inv_all();
}
EXPORT_SYMBOL(l2cache_flush_all);

void l2cache_inv_range(unsigned long start, unsigned long end)
{
	unsigned long addr;
 	unsigned long reg;
    	
	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}

#ifdef CONFIG_CACHE_HIL2_DEBUG
	printk("%s: start addr: 0x%x, end addr: 0x%x\n",__FUNCTION__,(unsigned int)(start),(unsigned int)(end));
#endif
	cache_sync();
	start &= ~(CACHE_LINE_SIZE - 1);

	for (addr = start; addr < end; addr += CACHE_LINE_SIZE){
	    	reg = addr | BIT_L2_INVALID_BYADDRESS;
    		sync_writel(reg, REG_L2_INVALID,reg);	
	}
	cache_sync();
}
EXPORT_SYMBOL(l2cache_inv_range);

void l2cache_clean_range(unsigned long start, unsigned long end)
{
	unsigned long addr;
	unsigned long reg;

	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}
	
#ifdef CONFIG_CACHE_HIL2_DEBUG
	printk("%s: start addr: 0x%x, end addr: 0x%x\n",__FUNCTION__,(unsigned int)(start),(unsigned int)(end));
#endif
	cache_sync();
	start &= ~(CACHE_LINE_SIZE - 1);
	for (addr = start; addr < end; addr += CACHE_LINE_SIZE)
	{
	    	reg = addr | BIT_L2_CLEAN_BYADDRESS;
    		sync_writel(reg, REG_L2_CLEAN,reg);
	}
	cache_sync();
}
EXPORT_SYMBOL(l2cache_clean_range);

void l2cache_flush_range(unsigned long start, unsigned long end)
{
	unsigned long addr;
	
	if(!l2cache_flag)
	{
		printk("%s:l2cache_flag = %d,L2 cache may not enable!\n",__FUNCTION__,l2cache_flag);
		return;
	}
	
#ifdef CONFIG_CACHE_HIL2_DEBUG
	printk("%s: start addr: 0x%x, end addr: 0x%x\n",__FUNCTION__,(unsigned int)(start),(unsigned int)(end));
#endif
	cache_sync();
	start &= ~(CACHE_LINE_SIZE - 1);
	for (addr = start; addr < end; addr += CACHE_LINE_SIZE)
	{
		sync_writel(addr | 1, REG_L2_CLEAN, 1);
		sync_writel(addr | 1, REG_L2_INVALID,1);
	}	
	cache_sync();
}
EXPORT_SYMBOL(l2cache_flush_range);

static irqreturn_t l2cache_handle(int irq, void *dev_id)
{
	unsigned int ret1,ret2,ret3;
	
	ret1 = readl(l2cache_base + REG_L2_RINT);
	ret2 = readl(l2cache_base + REG_L2_SPECIAL_CHECK0);
	ret3 = readl(l2cache_base + REG_L2_SPECIAL_CHECK1);
	writel(0,l2cache_base + REG_L2_INTCLR);//clear inner INT
	printk("L2cache interrupt,REG_L2_RINT is 0x%x, REG_L2_SPECIAL_CHECK0 is 0x%x,REG_L2_SPECIAL_CHECK1 is 0x%x!\n",ret1,ret2,ret3);
	return IRQ_HANDLED;
}

void l2cache_init(void)
{
	int ret1,ret2,ret3,aux,aux_val = 0;
	unsigned long flags;

    if( (0==l2cache_en)
        ||(HI_CHIP_ID_S_E==hi_kernel_get_chip_id()) )
    {
        l2cache_en=0;
        return;
    }

	local_irq_save(flags);

	l2cache_base = (void __iomem *)IO_ADDRESS(REG_BASE_L2CACHE); 
	
	/* disable L2cache */
	writel(0, l2cache_base + REG_L2_CTRL);

	aux_val |= (0x1 << BIT_L2_AUCTRL_EVENT_BUS_EN) | (0x1 << BIT_L2_AUCTRL_MONITOR_EN);

	aux = readl(l2cache_base + REG_L2_AUCTRL);
	aux |= aux_val;
	writel(aux, l2cache_base + REG_L2_AUCTRL);

	/* clean last error int */
	writel(0, l2cache_base + REG_L2_RINT);
	writel(0, l2cache_base + REG_L2_SPECIAL_CHECK0);
	writel(0, l2cache_base + REG_L2_SPECIAL_CHECK1);

	/* enable inner INT except BIT_L2_INTMASK_AUTO_END */
	writel(0x3fff,l2cache_base + REG_L2_INTMASK);

	l2cache_flag = 1;

	l2cache_inv_all();

	/* enable L2cache */
	writel(1, l2cache_base + REG_L2_CTRL);

	ret1 = request_irq(INTNR_L2CACHE_CHK0_INT,l2cache_handle,0,"L2 chk0",NULL);
	ret2 = request_irq(INTNR_L2CACHE_CHK1_INT,l2cache_handle,0,"L2 chk1",NULL);
	ret3 = request_irq(INTNR_L2CACHE_INT_COMB,l2cache_handle,0,"L2 com",NULL);
	if(ret1 || ret2 || ret3)
		printk("request l2 cache irq error!\n");

	local_irq_restore(flags);

	outer_cache.inv_range = l2cache_inv_range;
	outer_cache.clean_range = l2cache_clean_range;
	outer_cache.flush_range = l2cache_flush_range;

	printk(KERN_INFO "L2cache cache controller enabled\n");
}

void l2cache_clean(void)
{
	unsigned long flags;
	
    if(0==l2cache_en)
    {
        return;
    }

	local_irq_save(flags);

	/* disable L2cache */
	l2cache_base = (void __iomem *)IO_ADDRESS(REG_BASE_L2CACHE); 
	writel(0, l2cache_base + REG_L2_CTRL);

	/* clean  L2cache */
	l2cache_flush_all();
	l2cache_flag = 0;

	local_irq_restore(flags);
	
	return;
}

void l2cache_exit(void)
{
	int ret=0;
	unsigned long flags;
	
    if(0==l2cache_en)
    {
        return;
    }

	/* disable L2cache */
	local_irq_save(flags);

	writel(0, l2cache_base + REG_L2_CTRL);
	l2cache_flush_all();
	l2cache_flag = 0;

	free_irq(INTNR_L2CACHE_CHK0_INT,NULL);
	free_irq(INTNR_L2CACHE_CHK1_INT,NULL);
	free_irq(INTNR_L2CACHE_INT_COMB,NULL);

	local_irq_restore(flags);
	
	printk(KERN_EMERG "L2cache is uninstalled\n");
}

int l2cache_suspend(void)
{
	int ret=0;
	unsigned long flags;
	
    if(0==l2cache_en)
    {
        return ret;
    }

	/* disable L2cache */
	local_irq_save(flags);

	writel(0, l2cache_base + REG_L2_CTRL);
	l2cache_flush_all();
	l2cache_flag = 0;

	free_irq(INTNR_L2CACHE_CHK0_INT,NULL);
	free_irq(INTNR_L2CACHE_CHK1_INT,NULL);
	free_irq(INTNR_L2CACHE_INT_COMB,NULL);

	local_irq_restore(flags);
	
	return ret;
}

int l2cache_resume(void)
{
	unsigned long aux,aux_val = 0;
	unsigned long flags,ret,ret1,ret2;

    if(0==l2cache_en)
    {
        return ret;
    }

	local_irq_save(flags);

	/* disable L2cache */
	writel(0, l2cache_base + REG_L2_CTRL);

	/* monitor */
	aux_val |= (0x1 << BIT_L2_AUCTRL_EVENT_BUS_EN) | (0x1 << BIT_L2_AUCTRL_MONITOR_EN);
	
	aux = readl(l2cache_base + REG_L2_AUCTRL);
	aux |= aux_val;
	writel(aux, l2cache_base + REG_L2_AUCTRL);

	/* clean last error int */
	writel(0, l2cache_base + REG_L2_RINT);
	writel(0, l2cache_base + REG_L2_SPECIAL_CHECK0);
	writel(0, l2cache_base + REG_L2_SPECIAL_CHECK1);

	/* enable inner INT except BIT_L2_INTMASK_AUTO_END */
	writel(0x3fff,l2cache_base + REG_L2_INTMASK);

	l2cache_flag = 1;
	l2cache_inv_all();

	/* enable L2cache */
	writel(1, l2cache_base + REG_L2_CTRL);

	ret  = request_irq(INTNR_L2CACHE_CHK0_INT,l2cache_handle,0,"L2 chk0",NULL);
	ret1 = request_irq(INTNR_L2CACHE_CHK1_INT,l2cache_handle,0,"L2 chk1",NULL);
	ret2 =request_irq(INTNR_L2CACHE_INT_COMB,l2cache_handle,0,"L2 com",NULL);
	if (ret || ret1 || ret2)
		printk("request l2 cache irq error!\n");

	local_irq_restore(flags);

	return 0;
}

static int __init l2cache_option(char *str)
{
    get_option(&str, &l2cache_en);
    return 1;
}
early_param("l2cache", l2cache_option);

