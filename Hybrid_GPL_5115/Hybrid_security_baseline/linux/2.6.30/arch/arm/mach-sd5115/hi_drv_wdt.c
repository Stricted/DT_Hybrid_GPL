/******************************************************************************
  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 *******************************************************************************
 �ļ�����: hi_drv_wdt.c
 ��������: �����ļ�
 �汾����: V1.0
 ��������: 2008��12��8��
 ��������: ��˼����������    
 �޸ļ�¼: 
 ******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <asm/io.h>

#include <mach/hi_drv_wdt.h>
#include <mach/early-debug.h>

/* <DTS2012110301136 l00205975 20121103 begin */
/*wdtӳ���������ַ*/
static volatile unsigned int *g_pui_wdt_config = NULL;
/*wdt��������*/
static hi_wdt_attr_s g_st_wdt_attr;
static struct timer_list g_st_wdt_timer;

/* �幷���� */
void hi_kernel_wdt_clear(void)
{
    *g_pui_wdt_config        = 0x55AA5A5A;
    *g_pui_wdt_config        = 0xAA55A5A5;
    g_st_wdt_attr.ui_lsttime = jiffies;
    return;
}

/* ����Ƿ�����ι�� */
unsigned int hi_kernel_wdt_check(void)
{
    unsigned int ui_inter = jiffies - g_st_wdt_attr.ui_lsttime;
    unsigned int ui_dev   = (g_st_wdt_attr.ui_timeout - g_st_wdt_attr.ui_period)*HZ;

    return ( ( 0 != g_st_wdt_attr.ui_state ) && ( ui_inter > ui_dev ) ) ? 1 : 0;
}

/* �ر�����Զ��幷 */
void hi_kernel_wdt_disable(void)
{
    g_st_wdt_attr.ui_state = 0;
    del_timer(&g_st_wdt_timer);
}

void hi_kernel_wdt_enable(void)
{
    g_st_wdt_attr.ui_state = 1;
    del_timer(&g_st_wdt_timer);
    g_st_wdt_timer.expires  = jiffies + ( g_st_wdt_attr.ui_period * HZ );
    add_timer(&g_st_wdt_timer);
}

/* ����Ӳ�����Ź��ػ�ʱ�� */
void hi_kernel_wdt_set(unsigned int ui_interval)
{
	unsigned int ui_reg_value;
	volatile unsigned int *pui_wdt_reset = NULL;

    if ( ui_interval > HI_WDT_MAX_TIMER )
    {
        printk("wdt reset time set error: out of range %d\n",HI_WDT_MAX_TIMER);        
        return;
    }
    g_st_wdt_attr.ui_timeout = ui_interval;
    
    /*�����ַӳ�䵽�����ڴ�*/
    pui_wdt_reset = ioremap(0x10100144, sizeof(unsigned int));
    if (NULL == pui_wdt_reset)
    {
        printk("ioremap error: pui_wdt_reset = HI_NULL");
        return;
    }

    ui_reg_value = *pui_wdt_reset;

    ui_reg_value &= HI_WDT_TIME_BIT_MASK;
    ui_reg_value |= (ui_interval-1) << 10;

    *pui_wdt_reset = ui_reg_value;
    iounmap(pui_wdt_reset);
    return;
}

/* �����Զ��幷ʱ���� */
void hi_kernel_wdt_set_clearintr(unsigned int ui_interval)
{
    if ( ui_interval > HI_WDT_MAX_CLRTIMER )
    {
        printk("wdt clear time set error: out of range %d\n",HI_WDT_MAX_CLRTIMER);
        return;
    }
    g_st_wdt_attr.ui_period = ui_interval;
}

/* �ں˶�ʱ�����ý����Զ��幷���� */
static void hi_kernel_wdt_timeout(unsigned long ui_data )
{
    hi_kernel_wdt_clear();
    g_st_wdt_timer.expires  = jiffies + ( g_st_wdt_attr.ui_period * HZ );
    add_timer(&g_st_wdt_timer);
    return;
}

static int __init hi_kernel_wdt_init(void)
{
    /*�����ַӳ�䵽�����ڴ�*/
    g_pui_wdt_config = ioremap(0x13f00038, sizeof(unsigned int));
    if ( NULL == g_pui_wdt_config )
    {
        printk("ioremap error: g_pui_wdt_config = NULL");
        return -1;
    }
    /*��¼Ĭ������*/
    memset( &g_st_wdt_attr, 0, sizeof(hi_wdt_attr_s) );
    g_st_wdt_attr.ui_state      = 1;
    g_st_wdt_attr.ui_period = HI_WDT_CLEAR_INTER;
    g_st_wdt_attr.ui_timeout  = HI_WDT_DEF_TIMER;
    /*ϵͳ�����幷*/
    hi_kernel_wdt_clear();
    /*Ӳ����30S��ʱ*/
    hi_kernel_wdt_set(HI_WDT_DEF_TIMER);
    /*���ÿ�붨ʱ�幷*/
    init_timer(&g_st_wdt_timer);
    g_st_wdt_timer.data     = 0x0;
    g_st_wdt_timer.function = hi_kernel_wdt_timeout;
    g_st_wdt_timer.expires  = jiffies + ( g_st_wdt_attr.ui_period * HZ );
    add_timer(&g_st_wdt_timer);

    printk("wdtchdog int the kernel init sucessful!\n");
    return 0;
}

EXPORT_SYMBOL(hi_kernel_wdt_clear);
EXPORT_SYMBOL(hi_kernel_wdt_disable);
EXPORT_SYMBOL(hi_kernel_wdt_enable);
EXPORT_SYMBOL(hi_kernel_wdt_check);
EXPORT_SYMBOL(hi_kernel_wdt_set);
EXPORT_SYMBOL(hi_kernel_wdt_set_clearintr);
/* DTS2012110301136 l00205975 20121103 end> */
pure_initcall(hi_kernel_wdt_init);

