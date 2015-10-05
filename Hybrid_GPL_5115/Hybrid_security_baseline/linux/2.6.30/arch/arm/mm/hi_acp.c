#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include <mach/platform.h>
#include <mach/kexport.h>

int acp_en=1;
int acp_clk=200;

#define SC_PERCTRL1_REG     ( REG_BASE_SCTL + 0xa0  )
#define SC_PERCTRL44_REG    ( REG_BASE_SCTL + 0x14c )

#define PIE_ARADDR33    (1 << 17)
#define PIE_AWADDR33    (1 << 16)
#define PCIE0_ARADDR33  (1 << 13)
#define PCIE0_AWADDR33  (1 << 12)
#define PCIE1_ARADDR33  (1 << 9 )
#define PCIE1_AWADDR33  (1 << 8 )

#define ACP_FREQ_SEL0   (1 << 28)    
#define ACP_FREQ_SEL1   (1 << 29)

void hi_acp_init(void)
{
    unsigned long flags, ui_value;

    if(0==acp_en)
    {
        /*
         * 选择PIE、PCIE0、PCIE1读写DDR的通路为直通通路访问.
         */
        local_irq_save(flags);
        
        ui_value = readl(IO_ADDRESS(SC_PERCTRL1_REG));
        ui_value &= ~(PIE_ARADDR33 | PIE_AWADDR33 | PCIE0_ARADDR33 | \
                PCIE0_AWADDR33 | PCIE1_ARADDR33 | PCIE1_AWADDR33);
        writel(ui_value, IO_ADDRESS(SC_PERCTRL1_REG));
        printk(KERN_INFO "ACP OFF!\n");

        local_irq_restore(flags);
        return;
    }

    /*
     * 选择PIE、PCIE0、PCIE1读写DDR的通路为ACP访问.
     */
    local_irq_save(flags);
    
    ui_value = readl(IO_ADDRESS(SC_PERCTRL1_REG));
    ui_value |= PIE_ARADDR33 | PIE_AWADDR33 | PCIE0_ARADDR33 | \
            PCIE0_AWADDR33 | PCIE1_ARADDR33 | PCIE1_AWADDR33;
    writel(ui_value, IO_ADDRESS(SC_PERCTRL1_REG));
    printk(KERN_INFO "ACP ON!\n");

    /*
     * 选择AXI_ACP总线的工作频率:
     *    00：300/250MHz
     *    01：200/166MHz 
     *    10：150/125MHz
     *    11：50/42.5MHz
     */
    if(300==acp_clk)
    {
        ui_value = readl(IO_ADDRESS(SC_PERCTRL44_REG));
        ui_value &= ~(ACP_FREQ_SEL0 | ACP_FREQ_SEL1);
        writel(ui_value, IO_ADDRESS(SC_PERCTRL44_REG));
        printk("ACP bus = 300M!\n");
    }
    else if(200==acp_clk)
    {
        ui_value = readl(IO_ADDRESS(SC_PERCTRL44_REG));
        ui_value |= ACP_FREQ_SEL0;
        ui_value &= ~ACP_FREQ_SEL1;
        writel(ui_value, IO_ADDRESS(SC_PERCTRL44_REG));
        printk("ACP bus = 200M!\n");
    }
    else if(150==acp_clk)
    {
        ui_value = readl(IO_ADDRESS(SC_PERCTRL44_REG));
        ui_value &= ~ACP_FREQ_SEL0;
        ui_value |= ACP_FREQ_SEL1;
        writel(ui_value, IO_ADDRESS(SC_PERCTRL44_REG));
        printk("ACP bus = 150M!\n");
    }
    else
    {
        ui_value = readl(IO_ADDRESS(SC_PERCTRL44_REG));
        ui_value |= ACP_FREQ_SEL0 | ACP_FREQ_SEL1;
        writel(ui_value, IO_ADDRESS(SC_PERCTRL44_REG));
        printk("ACP bus = 50M!\n");
    }

    local_irq_restore(flags);

    return;
}


static int __init acp_en_option(char *str)
{
    get_option(&str, &acp_en);
    return 1;
}
static int __init acp_clk_option(char *str)
{
    get_option(&str, &acp_clk);
    if( (acp_clk!=300)
        && (acp_clk!=200)
        && (acp_clk!=150)
        && (acp_clk!=50) )
    {
        printk("acp_clk option:%s invalid, using:%d(M)\n", str, acp_clk);
    }
    return 1;
}

early_param("acp_en", acp_en_option);
early_param("acp_clk", acp_clk_option);


int hi_kernel_acp_on()
{
    return acp_en;
}
EXPORT_SYMBOL(hi_kernel_acp_on);

