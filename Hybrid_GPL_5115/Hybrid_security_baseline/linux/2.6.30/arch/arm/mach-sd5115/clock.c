#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/mutex.h>

#include <asm/clkdev.h>
#include <linux/io.h>
#include <mach/io.h>
#include <mach/platform.h>
#include <mach/early-debug.h>
#include "clock.h"

int clk_enable(struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	return clk->rate;
}
EXPORT_SYMBOL(clk_get_rate);

//待补充实现:获取APB总线频率，供timer或者uart等APB设备使用
unsigned long get_apbclk_hw(void)
{
	return CONFIG_DEFAULT_BUSCLK; 
}

//
// sdk use function hi_kernel_ahb_clk_get, but it should be ecs exported
// 
uint hi_kernel_ahb_clk_get(uint *pui_ahb_clk)
{
#ifdef CONFIG_HISI_SD5115_AF_ASIC
	*pui_ahb_clk = 100 * 1000000;  //SD5115-MPW-ASIC中AHB总线为100MHZ
#else
	*pui_ahb_clk = 50 * 1000000;  //50MHz
#endif
    return 0;
}
EXPORT_SYMBOL(hi_kernel_ahb_clk_get);
