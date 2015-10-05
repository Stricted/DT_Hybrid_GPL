#include <linux/errno.h>
#include <linux/module.h>
#include <linux/reboot.h>

#include "atpconfig.h"
#include "msg/kcmsmonitormsgtypes.h"
#include "hi_drv_ext_int.h"

MODULE_LICENSE("GPL");
MODULE_ALIAS("hi_ext_int_custom");

extern void hi_ext_int2_e_callback_func(void);

hi_ext_int_attr_s st_hi_ext_int2_e_attr = 
{
    HI_TRUE_E,
    HI_LOW_LEVEL_E,
    hi_ext_int2_e_callback_func
};

void hi_ext_int2_e_callback_func(void)
{
    st_hi_ext_int2_e_attr.em_enable = HI_FALSE_E;
    hi_kernel_ext_int_attr_set(HI_EXT_INT2_E, &st_hi_ext_int2_e_attr);

    printk("\nkernel int2 ext\n");
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_REBOOT, NULL, 0);

	return;
}

static int __init hi_ext_int_custom_init(void)
{
    return (int)hi_kernel_ext_int_attr_set(HI_EXT_INT2_E, &st_hi_ext_int2_e_attr);
}

static void __exit hi_ext_int_custom_fini(void)
{
    st_hi_ext_int2_e_attr.em_enable = HI_FALSE_E;
    hi_kernel_ext_int_attr_set(HI_EXT_INT2_E, &st_hi_ext_int2_e_attr);

    return;
}

module_init(hi_ext_int_custom_init);
module_exit(hi_ext_int_custom_fini);