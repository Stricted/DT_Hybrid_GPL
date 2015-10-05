/*
 *	BcmDog :	A Hardware Watchdog for BCM96361
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <bcm_map_part.h>
#include <bcm_hwdefs.h>
#include <linux/interrupt.h>
#include <bcm_intr.h>

#define PFX "BcmDog: "

static unsigned long bcmdog_alive;
static char expect_close;
/*Start of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */
#if 0
static spinlock_t bcmdog_lock;
static unsigned int bcmdog_interrupt_num = 0;
#else
extern spinlock_t bcmdog_lock;
extern unsigned int bcmdog_interrupt_num;

/*Start of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
#if 0
extern unsigned int bcmdog_start_flag;
#else
#include "bcm_OS_Deps.h"
//extern struct timer_list   g_stBhalWatchdogTimer;

#ifdef CONFIG_CRASH_DUMPFILE
extern int ATP_Krnl_Crash_RegWriteFlash(const char * pszBuffer);
extern char g_acWriteCrashBuf[2048];
#endif

#endif
/*End of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */

#endif
/*End of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */

extern struct timer_list   g_stBhalWatchdogTimer;

/*Start of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
#if 0
#define WATCHDOG_TIMEOUT	30		/* Default is 30 seconds */
#else
#define WATCHDOG_TIMEOUT	40		/* Default is 40 seconds */
#endif
/*End of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */

static int bcmdog_margin = WATCHDOG_TIMEOUT;	/* in seconds */
module_param(bcmdog_margin, int, 0);
MODULE_PARM_DESC(bcmdog_margin,
	"BCM Watchdog timeout margin in seconds. (0 < bcmdog_margin < 33, default="
					__MODULE_STRING(WATCHDOG_TIMEOUT) ")");
					

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout,
		"BCM Watchdog cannot be stopped once started (default="
				__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");


static void bcmdog_start(void)
{
	spin_lock(&bcmdog_lock);
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;
	spin_unlock(&bcmdog_lock);
}

static void bcmdog_setWDRstLen(void)
{
	spin_lock(&bcmdog_lock);
    TIMER->WDResetCount = 0x2ffffff;
	spin_unlock(&bcmdog_lock);
}

static int bcmdog_set_heartbeat(int t)
{
	if ((t * FPERIPH < 0x00000001) || (t * FPERIPH > 0xFFFFFFFF))
		return -EINVAL;

	bcmdog_margin = t;
	return 0;
}

static void bcmdog_setWatchDogDefCount(void)
{
	spin_lock(&bcmdog_lock);
    TIMER->WatchDogDefCount = bcmdog_margin * FPERIPH;
	bcmdog_interrupt_num = 0;
    spin_unlock(&bcmdog_lock);
}

static void bcmdog_stop(void)
{
	spin_lock(&bcmdog_lock);
    TIMER->WatchDogCtl = 0xEE00;
    TIMER->WatchDogCtl = 0x00EE;
	spin_unlock(&bcmdog_lock);
}

static void bcmdog_keepalive(void)
{
    bcmdog_setWatchDogDefCount();
    bcmdog_stop();    
    bcmdog_start();
}

irqreturn_t bcm_watchdog_interrupt(int irq, void *dev_id)
{
    uint32   count;
	
    if ((TIMER->TimerInts & WATCHDOG) == WATCHDOG)
    {
		bcmdog_interrupt_num++;
		/*Start of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
        //printk("\nThe watchdog interrupt num = %d\n",bcmdog_interrupt_num);
        #if 0
		if (bcmdog_interrupt_num == 13)
		{
		#else
		if (bcmdog_interrupt_num == 10)
        {
            #ifdef CONFIG_CRASH_DUMPFILE
            memset(g_acWriteCrashBuf, 0, sizeof(g_acWriteCrashBuf));
            sprintf(g_acWriteCrashBuf,"\n\nIt is a watchdog reboot\n\n");
            ATP_Krnl_Crash_RegWriteFlash(g_acWriteCrashBuf);
		    #endif

		#endif
        /*End of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */

        /*Add by y00105954 2010-10-8 规避解决不插网卡无法重启的问题*/
        #define EPHY_PWR_DOWN_DLL       (1<<25)
            GPIO->RoboswEphyCtrl &= ~EPHY_PWR_DOWN_DLL;
        printk("It is a watchdog reboot\n");  					
        /*Add by y00105954 2010-10-8 规避解决不插网卡无法重启的问题*/
        }
        count = TIMER->WatchDogCtl;
        TIMER->WatchDogCtl = 0xEE00;
        TIMER->WatchDogCtl = 0x00EE;
        TIMER->WatchDogDefCount = count;
        TIMER->WatchDogCtl = 0xFF00;
        TIMER->WatchDogCtl = 0x00FF;
        
        return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

/*
 *	Allow only one person to hold it open
 */
static int bcmdog_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &bcmdog_alive))
		return -EBUSY;

	if (nowayout)
		__module_get(THIS_MODULE);

	/* Activate timer */
	bcmdog_setWDRstLen();
    bcmdog_keepalive();

	bcmdog_alive = 1;

	return nonseekable_open(inode, file);
}

static int bcmdog_release(struct inode *inode, struct file *file)
{
	/*
	 *	Shut off the timer.
	 * 	Lock it in if it's a module and we set nowayout
	 */
	if (expect_close == 42) {
		bcmdog_stop();
		module_put(THIS_MODULE);
	} else {
		printk(KERN_CRIT PFX "Unexpected close, not stopping BCM watchdog!\n");
		bcmdog_keepalive();
	}
	clear_bit(0, &bcmdog_alive);
	expect_close = 0;
	return 0;
}

static ssize_t bcmdog_write(struct file *file, const char *data,
						size_t len, loff_t *ppos)
{
	/*
	 *	Refresh the timer.
	 */
	if(len) {
		if (!nowayout) {
			size_t i;

			/* In case it was set long ago */
			expect_close = 0;

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					expect_close = 42;
			}
		}
		bcmdog_keepalive();
	}
	return len;
}

static long bcmdog_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_margin;
	static const struct watchdog_info ident = {
		.options =		WDIOF_SETTIMEOUT |
					WDIOF_KEEPALIVEPING |
					WDIOF_MAGICCLOSE,
		.firmware_version =	0,
		.identity =		"BCM Watchdog",
	};
	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &ident, sizeof(ident)) ? -EFAULT : 0;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);
	case WDIOC_KEEPALIVE:
		bcmdog_keepalive();
		return 0;
	case WDIOC_SETTIMEOUT:
		if (get_user(new_margin, p))
			return -EFAULT;
		if (bcmdog_set_heartbeat(new_margin))
			return -EINVAL;
		bcmdog_keepalive();
		/* Fall */
	case WDIOC_GETTIMEOUT:
		return put_user(bcmdog_margin, p);
	default:
		return -ENOTTY;
	}
}

/*
 *	Notifier for system down
 */

static int bcmdog_notify_sys(struct notifier_block *this,
					unsigned long code, void *unused)
{
	if (code == SYS_DOWN || code == SYS_HALT)
		bcmdog_stop();		/* Turn the WDT off */

	return NOTIFY_DONE;
}

/*
 *	Kernel Interfaces
 */

static const struct file_operations bcmdog_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= bcmdog_write,
	.unlocked_ioctl	= bcmdog_ioctl,
	.open		= bcmdog_open,
	.release	= bcmdog_release,
};

static struct miscdevice bcmdog_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &bcmdog_fops,
};

static struct notifier_block bcmdog_notifier = {
	.notifier_call	= bcmdog_notify_sys,
};

static char banner[] __initdata = KERN_INFO "BCM Hardware Watchdog Timer for BCM96361\n";

static int __init watchdog_init(void)
{
	int ret;

    if (request_irq(INTERRUPT_ID_TIMER, bcm_watchdog_interrupt, IRQF_DISABLED|IRQF_SHARED, "BCM WATCHDOG", (void *)&(TIMER->WatchDogCtl)))
    {        
        return -EBUSY;
    }
    
	/* Check that the bcmdog_margin value is within it's range ; if not reset to the default */
	if (bcmdog_set_heartbeat(bcmdog_margin)) {
		bcmdog_set_heartbeat(WATCHDOG_TIMEOUT);
		printk(KERN_INFO PFX "bcmdog_margin value must be 0<bcmdog_margin<33, using %d\n",
			WATCHDOG_TIMEOUT);
	}

	ret = register_reboot_notifier(&bcmdog_notifier);
	if (ret) {
		printk (KERN_ERR PFX "cannot register reboot notifier (err=%d)\n",
			ret);
		return ret;
	}

	ret = misc_register(&bcmdog_miscdev);
	if (ret) {
		printk (KERN_ERR PFX "cannot register miscdev on minor=%d (err=%d)\n",
			WATCHDOG_MINOR, ret);
		unregister_reboot_notifier(&bcmdog_notifier);
		return ret;
	}

	printk(banner);

	/*Start of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
	#if 0
    /*Start of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */
    bcmdog_start_flag = 1;
    /*End of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */
	#else
	bcmdog_setWDRstLen();
    bcmdog_keepalive();
	add_timer(&g_stBhalWatchdogTimer);
    #endif
    /*End of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */

	return 0;
}

static void __exit watchdog_exit(void)
{
	misc_deregister(&bcmdog_miscdev);
	unregister_reboot_notifier(&bcmdog_notifier);
	free_irq(INTERRUPT_ID_TIMER, 0x02);
	
	/*Start of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
	#if 0
    /*Start of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */
    bcmdog_start_flag = 0;
    /*End of Change by z00175982 for exceptional reboot 2010-12-18 DTS2010101601212 */
    #endif
    /*End of Change by z00175982 for supplementary exceptional reboot 2010-12-24 DTS2010101601212 */
}

module_init(watchdog_init);
module_exit(watchdog_exit);

MODULE_AUTHOR("Tiny <tiny@huawei.com>");
MODULE_DESCRIPTION("Hardware Watchdog Device for BCM96361");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

