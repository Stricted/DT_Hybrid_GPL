/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : bhalplat.c
  作    者  : l63336
  版    本  : atp v1.0
  创建日期  : 2009-4-23
  描    述  :   硬件相关的初始化，目前包括
                     GPIO
                     BUTTON
                     单板信息
                     NVRAM
                     RAM
  函数列表  :
                writeNvramData
                SetPll
                SetGpio
                sesLed_mapGpio
                sesLed_ctrl
                sesBtn_read
                sesBtn_pressed
                sesBtn_poll
                sesBtn_mapIntr
                sesBtn_mapGpio
                sesBtn_isr
                ses_board_init
                ses_board_deinit
                readNvramData
                map_external_irq
                kerSysRegisterDyingGaspHandler
                kerSysNvRamSet
                kerSysNvRamGet
                kerSysGetWlanSromParams
                kerSysGetSdramSize
                kerSysGetMacAddress
                kerSysEraseNvRam
                kerSysDeregisterDyingGaspHandler
                dumpaddr
                brcm_board_cleanup
                BpUpdateWirelessSromMap
                BpGetWirelessFlags
                borad_ioc_free
                borad_ioc_alloc
                board_release
                board_read
                board_poll
                board_open
                board_ioctl
                bhalplatWpsChange
                bhalplatSoftReboot
                bhalplatsetGpio
                bhalplatPollSwitchWlanKey
                bhalplatPollResetKey
                bhalplatPollResetKey
                bhalplatInitBoard
                bhalplatHardReboot
                bhalplatGetSwitchWlanKeyStatus
                bhalplatGetResetKeyStatus
                bhalplatGetChipParam
                bhalplatGetBoardVersion
                bhalplatGetBoardPcbVersion
                bhalplatGetBoardManufacturer
                bhalplatGetBoardAllId
                bhalGetTagFromPartition
                bhalGetTagByFlashOffset
                ATP_BhalplatInit

  历史记录      :
   1.日    期   : 2009-4-23
     作    者   : l63336
     修改内容   : 完成初稿

*********************************************************************************/

#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/capability.h>

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/if.h>
#include <linux/timer.h>
#include <linux/smp.h>
#include <linux/reboot.h>
#include <linux/bcm_assert_locks.h>
#include <asm/delay.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
#include <linux/slab.h>
#endif

#include "bhal.h"
#include "bhalplat.h"
#include "atpflash.h" 
#include "boardparms.h"
#include "bcm_intr.h"
#include <bcm_map_part.h>
#include "board.h"
#include "bcmTag.h"
#include "atpled.h"
#include "atpconfig.h"
#include "msg/kcmsmonitormsgtypes.h"
#include "kerneltag.h"
#include "atpflash.h"
#include "flash_api.h"

#include "atp_system_boot.h"
#include <boardparms_voice.h>
#include <linux/mtd/mtd.h>

extern bool g_bCmsIsOK;
struct timer_list g_stRemedyResetTimer;

DEFINE_SPINLOCK(bcm_gpio_spinlock); //Luther debug for DSL

ATP_KEY_ST g_buttonlist[BHAL_KEY_END];
//struct timer_list g_buttontimer[BHAL_KEY_END];
struct timer_list g_buttontimer;

extern unsigned char g_blparms_buf[];

extern int g_flash_total_size;
extern int g_flash_sector_size;
extern bool g_bIsupgrading;
/*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
extern void ATP_LED_AllLed(unsigned int state, unsigned int color);
unsigned short g_usI2cIrq = 0;  //i2c intr
#endif
/*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
void bhalbutton_init();

#ifdef SUPPORT_ATP_WATCHDOG
extern int g_RebootStopIsr;
#endif

/*定义各种版本号*/
#define BHALPLAT_CHIPPARAMSIZE 32
#define BHALPLAT_CHIP_ID
#define COUNTS_PSEC                      2 
#define WPS_WLAN_TIME                    10  
#define REVERSE_WLAN_STATE_TIME          1

/* start of add by w00165788 for DT 5V devices power supply */
#ifdef GPIO_CTRL_POWER
static void bhalplatSetPowerOn(void);
static void bhalplatSetPowerOff(void);
#endif
/* end of add by w00165788 for DT 5V devices power supply */

#if defined (SUPPORT_ATP_WLAN)
#ifdef SUPPORT_ATP_WPS
static int WPS_FLAG = 0 ;
unsigned long board_major = 0;
static struct timer_list   switch_wlan_timer;
static wait_queue_head_t g_stBhalWaitQueue;
static int g_sWlanFlag = 0;
static unsigned int (*g_orig_fop_poll)
    (struct file *, struct poll_table_struct *) = NULL;
static int map_external_irq (int irq);

 
#define SES_EVENT_BTN_PRESSED      0x00000001
#define SES_EVENTS                 SES_EVENT_BTN_PRESSED /*OR all values if any*/
#define SES_LED_OFF     0
#define SES_LED_ON      1
#define SES_LED_BLINK   2
typedef struct
{
    unsigned long eventmask;    
} BOARD_IOC, *PBOARD_IOC;

#endif
#endif

#ifdef SUPPORT_ATP_VOICE_DT
static unsigned int g_ulUrgentcallStatus = 0;
int bhalplatSetUrgentCallStatus(unsigned int ulStatus);
#endif

#ifdef SUPPORT_ATP_WLAN_DT
static int g_iWpsProcStatusFlag = 0;
struct timer_list g_stWpsProcStatusTimer;

int bhalplatSetWpsProcStatus(void);
int bhalplatGetWpsProcStatus(void);
int bhalplatClrWpsProcStatus(void);
static void bhalplatClearWpsProcStatus(unsigned long ulData);
/* START ADD by y00181549 for DTS2013101803442 at 20131129*/
bool g_bWlanSettingBusy = FALSE;
//unsigned int g_ulPressTimes = 0;
/* END ADD by y00181549 for DTS2013101803442 at 20131129*/
#endif

static unsigned short resetBtn_irq = BP_NOT_DEFINED;
static wait_queue_head_t g_stBhalPlatWaitQueue;

static irqreturn_t reset_isr(int irq, void *dev_id);
static Bool resetBtn_pressed(void);
static void __init sesBtn_mapIntr(int context);

extern unsigned long getMemorySize(void);
extern unsigned long getCrc32(const char *pdata, unsigned long size, unsigned long crc) ;
extern int bhalGetFlashSize(void);
extern int BpGetVcopeGpio (int pio_idx);

#if defined (SUPPORT_ATP_WLAN)
#ifdef SUPPORT_ATP_WPS
static unsigned int board_poll(struct file *filp, struct poll_table_struct *wait);
static ssize_t board_read(struct file *filp,  char __user *buffer, size_t count, loff_t *ppos);
static int board_release(struct inode *inode, struct file *filp);
static int board_open( struct inode *inode, struct file *filp );
static int board_ioctl( struct inode *inode, struct file *flip,
                        unsigned int command, unsigned long arg );
static void SetGpio(int gpio, GPIO_STATE_t state);
static void __init ses_board_init();
static void SetPll (int pll_mask, int pll_value);

static struct timer_list   poll_timer;

/*START ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
static long boardunlockedIoctl(struct file *file, unsigned int cmd, unsigned long arg)
{   
    struct inode *inode = file->f_dentry->d_inode;
    long ret;
    
    ret = board_ioctl(inode, file, cmd, arg);
    return ret;
}
#endif
/*END ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/

static struct file_operations board_fops =
{
  open:       board_open,
  /*START ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
  unlocked_ioctl:      boardunlockedIoctl,
#else  
  ioctl:      board_ioctl,
#endif
  /*END ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/  
  poll:       board_poll,
  read:       board_read,
  release:    board_release,
};

static struct workqueue_struct *board_workqueue;
#endif
#endif


void (*ethsw_unmanage_fptr)(void);
EXPORT_SYMBOL(ethsw_unmanage_fptr);


#ifdef SUPPORT_HG552A
#if defined (SUPPORT_ATP_WLAN)

#define MAX_ATH_NETS 100
static int g_netdevice[MAX_ATH_NETS];
static int g_callbacks[MAX_ATH_NETS];     //static irqreturn_t jumpstart_intr(int cpl, void *dev_id, struct pt_regs *regs);
static unsigned  regIndex = 0;
void unregister_simple_config_callback(void)
{
    printk("\nunregister_simple_config_callback is called \n");
    regIndex --;	
}	

void register_simple_config_callback (void *callback, void *arg)
{
     if(regIndex > MAX_ATH_NETS)
	 	regIndex = 0;
	 
	if(NULL != arg) 
	{
	    g_netdevice [ regIndex] = arg;
	    g_callbacks[ regIndex] = callback;	
	}	
	regIndex ++;
}
EXPORT_SYMBOL(unregister_simple_config_callback);
EXPORT_SYMBOL(register_simple_config_callback);
void do_wps_pbc_notify_atheros(void)
{
    static irqreturn_t (*callbackfun_jumpstart_intr)(int cpl, void *dev_id, struct pt_regs *regs);
    void *dev_id = NULL;
    int i = 0;
	
    for(i = 0;i<regIndex;i++)
    {
        printk("board.c:notify PBC event to Atheros wsccmd...\n",regIndex);
        callbackfun_jumpstart_intr = g_callbacks[i] ;
        dev_id = g_netdevice [i];
        (* callbackfun_jumpstart_intr)(0, dev_id, NULL);    
    }
}	
#endif
#endif

void bhalplatInitBoard(void)
{

#if defined (SUPPORT_ATP_WLAN)
#ifdef SUPPORT_ATP_WPS
    register_chrdev(206, "brcmboard", &board_fops);
    board_workqueue = create_singlethread_workqueue("board");

    ses_board_init();
#endif
#endif

    return;
}

/******************************************************************************
  函数名称  : bhalplatGetChipParam
  功能描述  : 获取芯片参数信息
  输入参数  : 
              1.  enChipParamType:获取芯片参数的类型
              2.  ulString: 输出参数，保存获得的值
              3.  ulSize: 芯片参数的长度
  调用函数  : 
  被调函数  : 
  输出参数  : ulString 作为地址指向的区域
  返 回 值  : 1.VOS_ERROR 失败
              2.VOS_OK    成功

  修改历史      :
   1.日    期   : 2007-7-3
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalplatGetChipParam(ATP_BHAL_CHIP_PARAM_EN enChipParamType,unsigned long ulString,unsigned long ulSize)
{
    int iRet = BHAL_ERROR;

    if(BHALPLAT_CHIPPARAMSIZE < ulSize || enChipParamType < BHAL_CHIP_ID || enChipParamType >= BHAL_CHIP_PARAM_END)
        return iRet;
        
    switch(enChipParamType)
    {
        case BHAL_CHIP_ID:
            iRet = (int) (PERF->RevID & 0xFFFF0000) >> 16;
            break;
        case BHAL_CHIP_REVISION_ID:
            iRet = BHAL_OK;
            break;
        default:
            break;            
    }
    return iRet;                
}

/******************************************************************************
  函数名称  : bhalplatGetBoardAllId
  功能描述  : 获取Board参数信息
  输入参数  : 
              1.  pcBuf: 获取Board参数的值
              2.  ulLen: Board参数的长度
  调用函数  : 
  被调函数  : 
  输出参数  : pcBuf
  返 回 值  : 1.VOS_ERROR 失败
              2.VOS_OK    成功

  修改历史      :
   1.日    期   : 2007-7-3
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/

int bhalplatGetBoardAllId(char *pcBuf, int ulLen)
{
    return BHAL_ERROR;
}

/******************************************************************************
  函数名称  : bhalplatSoftReboot
  功能描述  : 软重启
  输入参数  : 无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-3
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
void bhalplatSoftReboot(void)
{
    always_printk(KERN_EMERG "Restarting bcm96361 system.\n");
    
    kerSysMipsSoftReset();
}
/******************************************************************************
  函数名称  : bhalplatHardReboot
  功能描述  : 硬重启
  输入参数  : 无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-3
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/

void bhalplatHardReboot(void)
{
}

extern int g_lBhalFlashWriteFlag;
extern struct timer_list g_stBhalResetTimer, g_stBhalSwitchWlanTimer, g_stBhalSwitchWPSKeyTimer;
extern int g_lBhalResetKeyState, g_lBhalWlanKeyState, g_lBhalWPSKeyState;
extern unsigned int g_ulBhalKeyScanMode;

#define RESTORE_DEFAULT_SETTING_TIME      3
#define REBOOT_SETTING_TIME               3
#define RESET_SYSTEM_TIME                 1

int kerSysGetSdramSize( void )
{
    //return BpGetSdramSize();
    return getMemorySize();
} 

int kerSysGetMacAddress( unsigned char *pucaMacAddr, unsigned long ulId )
{
    return bhalGetMacAddress(pucaMacAddr,ulId);
}

unsigned long kerSysGetMacAddressType( unsigned char *ifName )
{
    unsigned long macAddressType = MAC_ADDRESS_ANY;

    if(strstr(ifName, IF_NAME_ETH))
    {
        macAddressType = MAC_ADDRESS_ETH;
    }
    else if(strstr(ifName, IF_NAME_USB))
    {
        macAddressType = MAC_ADDRESS_USB;
    }
    else if(strstr(ifName, IF_NAME_WLAN))
    {
        macAddressType = MAC_ADDRESS_WLAN;
    }
    else if(strstr(ifName, IF_NAME_MOCA))
    {
        macAddressType = MAC_ADDRESS_MOCA;
    }
    else if(strstr(ifName, IF_NAME_ATM))
    {
        macAddressType = MAC_ADDRESS_ATM;
    }
    else if(strstr(ifName, IF_NAME_PTM))
    {
        macAddressType = MAC_ADDRESS_PTM;
    }
    else if(strstr(ifName, IF_NAME_GPON))
    {
        macAddressType = MAC_ADDRESS_GPON;
    }
    else if(strstr(ifName, IF_NAME_EPON))
    {
        macAddressType = MAC_ADDRESS_EPON;
    }

    return macAddressType;
}

#ifndef BRCM_DAYING_GASP
void kerSysDeregisterDyingGaspHandler(char *devname)
{
    /*wl_linux.c use*/
}

void kerSysRegisterDyingGaspHandler(char *devname, void *cbfn, void *context)
{
    /*wl_linux_dslcpe.c use*/
}
#endif

int BpUpdateWirelessSromMap(unsigned short chipID, unsigned short* pBase, int sizeInWords)
{
    return 0xff;

}

int BpGetWirelessFlags( unsigned short *pusValue )
{
    return 0xff;
}

extern int other_cpu_stopped;  // set by stop_this_cpu in smp.c

void kerSysMipsSoftReset(void)
{
	unsigned long cpu_id;
	int count=0;
	
	cpu_id = smp_processor_id();
	printk(KERN_INFO "kerSysMipsSoftReset: called on cpu %lu\n", cpu_id);

#ifdef SUPPORT_ATP_WATCHDOG
    g_RebootStopIsr = 1;
#endif

/*Stop other cpu*/
#if defined(CONFIG_SMP)
    
    smp_send_stop();
    do
    {
        udelay(1000);
        count++;
        if (count % 0x3fff == 0)
        {
            printk(KERN_WARNING "still waiting for other cpu to stop, "
                                "jiffies=%lu\n", jiffies);
        }
    } while (!other_cpu_stopped);
	
#endif

    local_irq_disable();  // ignore interrupts, just execute reset code now

    // Power Management on Ethernet Ports may have brought down EPHY PLL
    // and soft reset below will lock-up 6362 if the PLL is not up
    // therefore bring it up here to give it time to stabilize
    GPIO->RoboswEphyCtrl &= ~EPHY_PWR_DOWN_DLL;

    // let UART finish printing
    udelay(100);

    PERF->pll_control |= SOFT_RESET;    // soft reset mips

    for(;;) {} // spin mips and wait soft reset to take effect
	
}

void kerSysSendtoMonitorTask(int msgType, char *msgData, int msgDataLen)
{
	/*bcmenet.c use  */;
}

void kerSysSetGpio(unsigned short bpGpio, GPIO_STATE_t state)
{
	/*adsl.c use*/;
}

/******************************************************************************
  函数名称  : bhalplatsetGpio
  功能描述  : 设置GPIO
  输入参数  : gpio 需要设置的gpio 号
                           enState 设置状态
                           默认为out状态
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-20
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/

void  bhalplatsetGpio(int gpio,BSP_E_GPIO_STATE enState)
{
    GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);

    if ((enState == kGpioActive && !(gpio & BP_ACTIVE_LOW)) || 
        (enState == kGpioInactive && (gpio & BP_ACTIVE_LOW)))
    {
        GPIO->GPIOio |= GPIO_NUM_TO_MASK(gpio);
    }
    else
    {
        GPIO->GPIOio &= ~GPIO_NUM_TO_MASK(gpio);
    }

    return;
    
}
#if defined(SUPPORT_ATP_WLAN)
#ifdef SUPPORT_ATP_WPS 
static unsigned short sesBtn_irq = BP_NOT_DEFINED;
static unsigned short sesBtn_gpio = BP_NOT_DEFINED;
static unsigned short sesLed_gpio = BP_NOT_DEFINED;

signed int bhalplatWpsChange(signed int wpsmode)
{
/*start: modify by j00127542 2011-02-22 for delete wlankey&wpskey*/
#ifndef SUPPORT_ATP_DEL_WPSKEY
    signed int iRet = -1;
    if(wpsmode)
    {
        //g_lBhalWPSKeyState = 1;
        g_buttonlist[BHAL_KEY_SWITCH_WPS].keystatus = 1;
    }
    else if (!wpsmode)
    {
        
        g_buttonlist[BHAL_KEY_SWITCH_WPS].keystatus = 0;
        //g_lBhalWPSKeyState = 0;
    }
    else
    {
        return iRet;
    }
#endif
/*end: modify by j00127542 2011-02-22 for delete wlankey&wpskey*/
    return 0;
}


 static Bool sesBtn_pressed(void)
 {
 /*
     unsigned long gpio_mask = GPIO_NUM_TO_MASK(sesBtn_gpio);
     volatile unsigned long *gpio_io_reg = &GPIO->GPIOio;
     if( (sesBtn_gpio & BP_GPIO_NUM_MASK) >= 32 )
     {
     
         printk("try to  cp gpio to reg\n");
         gpio_mask = GPIO_NUM_TO_MASK_HIGH(sesBtn_gpio);
         gpio_io_reg = &GPIO->GPIOio_high;
     }   
     if (!(*gpio_io_reg & gpio_mask)){
         return POLLIN;
     }   
  */
      if(WPS_FLAG)
          return POLLIN;
	  
      return 0;
 }
 static irqreturn_t sesBtn_isr(int irq, void *dev_id, struct pt_regs *ptregs)
 {   
     if (sesBtn_pressed()){
         wake_up_interruptible(&g_stBhalWaitQueue); 
         return IRQ_RETVAL(1);
     } else {
         return IRQ_RETVAL(0);       
     }
     
     return IRQ_RETVAL(0);
 }
 static void __init sesBtn_mapGpio()
 {   
     if( BpGetWirelessSesBtnGpio(&sesBtn_gpio) == BP_SUCCESS )
     {
         printk("SES: Button GPIO 0x%x is enabled\n", sesBtn_gpio);    
     }
 }
 
 static void __init sesBtn_mapIntr(int context)
 {       
     if ( BpGetWirelessSesExtIntr(&sesBtn_irq) == BP_SUCCESS )
     {
         printk("SES: Button Interrupt 0x%x is enabled\n", sesBtn_irq);
     }
     else
 	 {
 	     return;	
 	 }        
             
     sesBtn_irq = map_external_irq (sesBtn_irq) ;
     printk("sesBtn_irq==%d\n\r",sesBtn_irq);
             
     if (BcmHalMapInterrupt((FN_HANDLER)sesBtn_isr, context, sesBtn_irq))
	 {
         printk("SES: Interrupt mapping failed\n");
     }
	 
     BcmHalInterruptEnable(sesBtn_irq);
 }
 
 //modify by yinheng for WPS 080316
 static unsigned int sesBtn_poll(struct file *file, struct poll_table_struct *wait)
 {   
 
     if (sesBtn_pressed()){
      
         return POLLIN;
     }   
     return 0;
 }
 
 static ssize_t sesBtn_read(struct file *file,  char __user *buffer, size_t count, loff_t *ppos)
 {
 //start of modify by y68191 for VDFC02_wps
     volatile unsigned int event=0;
     ssize_t ret=0;  
 
     if(!sesBtn_pressed()){
     //BcmHalInterruptEnable(sesBtn_irq);        
         return ret;
     }   
     event = SES_EVENTS;
     __copy_to_user((char*)buffer, (char*)&event, sizeof(event));    
  //   BcmHalInterruptEnable(sesBtn_irq);    
     count -= sizeof(event);
     buffer += sizeof(event);
     ret += sizeof(event);   
     return ret; 
  //end  of modify by y68191 for VDFC02_wps
 }
 
 static void __init sesLed_mapGpio()
 {   
     if( BpGetWirelessSesBtnGpio(&sesLed_gpio) == BP_SUCCESS )
     {
         printk("SES: LED GPIO 0x%x is enabled\n", sesBtn_gpio);    
     }
 }
 
 static void sesLed_ctrl(int action)
 {
 
     //char status = ((action >> 8) & 0xff); /* extract status */
     //char event = ((action >> 16) & 0xff); /* extract event */        
     //char blinktype = ((action >> 24) & 0xff); /* extract blink type for SES_LED_BLINK  */
     
     BOARD_LED_STATE led;
     
     //if(sesLed_gpio == BP_NOT_DEFINED)
     //    return;
         
     action &= 0xff; /* extract led */
 
     //printk("blinktype=%d, event=%d, status=%d\n",(int)blinktype, (int)event, (int)status);
                 
     switch (action) 
     {
         case SES_LED_ON:
             //printk("SES: led on\n");
             led = kLedStateOn;                                          
             break;
         case SES_LED_BLINK:
             //printk("SES: led blink\n");
             led = kLedStateSlowBlinkContinues;                  
             break;
         case SES_LED_OFF:
             default:
             //printk("SES: led off\n");
             led = kLedStateOff;                         
     }   
     
     kerSysLedCtrl(kLedWps, led);
 }
 
 static void __init ses_board_init()
 {
	//sesBtn_mapGpio();
	//sesBtn_mapIntr(0);
	//sesLed_mapGpio();
 }
 static void __exit ses_board_deinit()
 {
     if(sesBtn_irq)
         BcmHalInterruptDisable(sesBtn_irq);
 } 
 void __exit brcm_board_cleanup( void )
 {
     printk("brcm_board_cleanup()\n");
     
     if (board_major != -1) 
     {
         /*GPIO 3.4.3*/
         del_timer_sync(&poll_timer);
         del_timer_sync(&switch_wlan_timer);
         unregister_chrdev(board_major, "board_ioctl");
     }
 } 
 
 static BOARD_IOC* borad_ioc_alloc(void)
 {
     BOARD_IOC *board_ioc =NULL;
     board_ioc = (BOARD_IOC*) kmalloc( sizeof(BOARD_IOC) , GFP_KERNEL );
     if(board_ioc)
     {
         memset(board_ioc, 0, sizeof(BOARD_IOC));
     }
     return board_ioc;
 }
 
 static void borad_ioc_free(BOARD_IOC* board_ioc)
 {
     if(board_ioc)
     {
         kfree(board_ioc);
     }   
 }
 
 
 static int board_open( struct inode *inode, struct file *filp )
 {
     filp->private_data = borad_ioc_alloc();
 
     if (filp->private_data == NULL)
         return -ENOMEM;
             
     return( 0 );
 } 
 
 static int board_release(struct inode *inode, struct file *filp)
 {
     BOARD_IOC *board_ioc = filp->private_data;
     
     wait_event_interruptible(g_stBhalPlatWaitQueue, 1);    
     borad_ioc_free(board_ioc);
 
     return( 0 );
 } 
 
 
 static unsigned int board_poll(struct file *filp, struct poll_table_struct *wait)
 {
     unsigned int mask = 0;        
     BOARD_IOC *board_ioc = filp->private_data;      

     poll_wait(filp, &g_stBhalPlatWaitQueue, wait);
     
     if(board_ioc->eventmask & SES_EVENTS)
	 {
	     if (WPS_FLAG)
	     {
	         printk("board_poll is woke up\n\r"); 
	         mask |= sesBtn_poll(filp, wait);
	     }
     }           
 
     return mask;
 }
 
 
 static ssize_t board_read(struct file *filp,  char __user *buffer, size_t count, loff_t *ppos)
 {
     ssize_t ret=0;
     BOARD_IOC *board_ioc = filp->private_data;
     if(board_ioc->eventmask & SES_EVENTS){
        ret=sesBtn_read(filp, buffer, count, ppos);
        WPS_FLAG=0;
        return ret;
     }
     return 0;
 }
 static int board_ioctl( struct inode *inode, struct file *flip,
                         unsigned int command, unsigned long arg )
 {
     int ret = 0;
     BOARD_IOCTL_PARMS ctrlParms;
     unsigned char ucaMacAddr[NVRAM_MAC_ADDRESS_LEN];
     static int iHspaTrafficMode = MODE_NONE;
     
     switch (command) 
     {
#if 0
        case BOARD_IOCTL_GET_NR_PAGES:
            ctrlParms.result = nr_free_pages() + get_page_cache_size();
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
            break;
#endif
        case BOARD_IOCTL_DUMP_ADDR:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) 
            {
                dumpaddr( (unsigned char *) ctrlParms.string, ctrlParms.strLen );
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else
                ret = -EFAULT;
            break;

        case BOARD_IOCTL_SET_MEMORY:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) 
            {
                unsigned long  *pul = (unsigned long *)  ctrlParms.string;
                unsigned short *pus = (unsigned short *) ctrlParms.string;
                unsigned char  *puc = (unsigned char *)  ctrlParms.string;
                switch( ctrlParms.strLen )
                {
                    case 4:
                        *pul = (unsigned long) ctrlParms.offset;
                        break;
                    case 2:
                        *pus = (unsigned short) ctrlParms.offset;
                        break;
                    case 1:
                        *puc = (unsigned char) ctrlParms.offset;
                        break;
                }
                dumpaddr( (unsigned char *) ctrlParms.string, sizeof(long) );
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else
                ret = -EFAULT;
            break;
      
        case BOARD_IOCTL_LED_CTRL:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) 
            {
                kerSysLedCtrl((BOARD_LED_NAME)ctrlParms.strLen, (BOARD_LED_STATE)ctrlParms.offset);
                ret = 0;
            }
            break;


        case BOARD_IOCTL_GET_CHIP_ID:
            ctrlParms.result = (int) (PERF->RevID & 0xFFFF0000) >> 16;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
            break;

        /* 通过复位键更改wlan的状态 */
        case BOARD_IOCTL_CHECK_WLAN_STATUS:            
            ctrlParms.result = g_sWlanFlag;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
            break;

        case BOARD_IOCTL_GET_NUM_ENET: {
            ETHERNET_MAC_INFO EnetInfos[BP_MAX_ENET_MACS];
            int i, numeth = 0;
            if (BpGetEthernetMacInfo(EnetInfos, BP_MAX_ENET_MACS) == BP_SUCCESS) {
            for( i = 0; i < BP_MAX_ENET_MACS; i++) {
                if (EnetInfos[i].ucPhyType != BP_ENET_NO_PHY) {
                numeth++;
                }
            }
            ctrlParms.result = numeth;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,     sizeof(BOARD_IOCTL_PARMS));   
            ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
            }

        case BOARD_IOCTL_GET_CFE_VER:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                char *vertag =  (char *)(FLASH_BASE + CFE_VERSION_OFFSET);
                if (ctrlParms.strLen < CFE_VERSION_SIZE) {
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                    ret = -EFAULT;
                }
                else if (strncmp(vertag, "cfe-v", 5)) { // no tag info in flash
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                    ret = 0;
                }
                else {
                    ctrlParms.result = 1;
                    __copy_to_user(ctrlParms.string, vertag+CFE_VERSION_MARK_SIZE, CFE_VERSION_SIZE);
                    ctrlParms.string[CFE_VERSION_SIZE] = '\0';
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                    ret = 0;
                }
            }
            else {
                ret = -EFAULT;
            }
            break;

        case BOARD_IOCTL_GET_ENET_CFG:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                ETHERNET_MAC_INFO EnetInfos[BP_MAX_ENET_MACS];
                if (BpGetEthernetMacInfo(EnetInfos, BP_MAX_ENET_MACS) == BP_SUCCESS) {
                    if (ctrlParms.strLen == sizeof(EnetInfos)) {
                        __copy_to_user(ctrlParms.string, EnetInfos, sizeof(EnetInfos));
                        ctrlParms.result = 0;
                        __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));   
                        ret = 0;
                    } else
                        ret = -EFAULT;
                }
                else {
                    ret = -EFAULT;
                }
                break;
            }
            else {
                ret = -EFAULT;
            }
            break;            

        case BOARD_IOCTL_GET_WLAN_ANT_INUSE:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                unsigned short antInUse = 0;
                if (BpGetWirelessAntInUse(&antInUse) == BP_SUCCESS) {
                    if (ctrlParms.strLen == sizeof(antInUse)) {
                        __copy_to_user(ctrlParms.string, &antInUse, sizeof(antInUse));
                        ctrlParms.result = 0;
                        __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));   
                        ret = 0;
                    } else
                        ret = -EFAULT;
                }
            else {
               ret = -EFAULT;
            }
            break;
            }
            else {
                ret = -EFAULT;
            }
            break;            
           
        case BOARD_IOCTL_SET_TRIGGER_EVENT:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {                
                BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;                
                ctrlParms.result = -EFAULT;
                ret = -EFAULT;
                if (ctrlParms.strLen == sizeof(unsigned long)) {                                         
                    board_ioc->eventmask |= *((int*)ctrlParms.string);                    
                
                    if((board_ioc->eventmask & SES_EVENTS)) {
                            ctrlParms.result = 0;
                            ret = 0;
                    } 
                    
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));                        
                }
            break;
            }
            else {
                ret = -EFAULT;
            }
            break;                        

        case BOARD_IOCTL_GET_TRIGGER_EVENT:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;
                if (ctrlParms.strLen == sizeof(unsigned long)) {
                    __copy_to_user(ctrlParms.string, &board_ioc->eventmask, sizeof(unsigned long));
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));   
                    ret = 0;
                } else
                ret = -EFAULT;

            break;
            }
            else {
                ret = -EFAULT;
            }
            break;                
            
        case BOARD_IOCTL_UNSET_TRIGGER_EVENT:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                if (ctrlParms.strLen == sizeof(unsigned long)) {
                    BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;                    
                    board_ioc->eventmask &= (~(*((int*)ctrlParms.string)));                  
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));   
                    ret = 0;
                } else
                ret = -EFAULT;

            break;
            } 
            else {
                ret = -EFAULT;
            }
            break;      
            
        case BOARD_IOCTL_SET_SES_LED:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                if (ctrlParms.strLen == sizeof(int)) {
                    sesLed_ctrl(*(int*)ctrlParms.string);
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));   
                    ret = 0;
                } else
                ret = -EFAULT;

            break;
            }
            else {
                ret = -EFAULT;
            }
            break;                                                                  
  
        case BOARD_IOCTL_GET_VCOPE_GPIO:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                ret = ((ctrlParms.result = BpGetVcopeGpio(ctrlParms.offset)) != BP_NOT_DEFINED) ? 0 : -EFAULT;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            }
            else {
                ret = -EFAULT;  
                ctrlParms.result = BP_NOT_DEFINED;
            }

            break;

        case BOARD_IOCTL_SET_PLL: 
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                SetPll(ctrlParms.strLen, ctrlParms.offset);
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            } 
            else {
                ret = -EFAULT;  
            }
            break;

        case BOARD_IOCTL_SET_GPIO:
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
                SetGpio(ctrlParms.strLen, ctrlParms.offset);
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            } 
            else {
                ret = -EFAULT;  
            }
            break;
        default:
            ret = -EINVAL;
            ctrlParms.result = 0;
            printk("board_ioctl: invalid command %x, cmd %d .\n",command,_IOC_NR(command));
            break;

  } /* switch */
 
   return (ret);
 
}

#endif
#endif

static int map_external_irq (int irq)
{
    int map_irq ;

    switch (irq) 
    {
        case BP_EXT_INTR_0   :
            map_irq = INTERRUPT_ID_EXTERNAL_0;
        break ;
        case BP_EXT_INTR_1   :
            map_irq = INTERRUPT_ID_EXTERNAL_1;
        break ;
        case BP_EXT_INTR_2   :
            map_irq = INTERRUPT_ID_EXTERNAL_2;
        break ;
        case BP_EXT_INTR_3   :
            map_irq = INTERRUPT_ID_EXTERNAL_3;
        break ;
#ifdef INTERRUPT_ID_EXTERNAL_4
        case BP_EXT_INTR_4   :
            map_irq = INTERRUPT_ID_EXTERNAL_4;
        break ;
#endif
#ifdef INTERRUPT_ID_EXTERNAL_5
        case BP_EXT_INTR_5   :
            map_irq = INTERRUPT_ID_EXTERNAL_5;
        break ;
#endif
        default              :
        printk ("Invalid External Interrupt definition \n") ;
            map_irq = 0 ;
        break ;
    }
    return (map_irq) ;
}

static void SetPll (int pll_mask, int pll_value)
{
    PERF->pll_control &= ~pll_mask;   // clear relevant bits
    PERF->pll_control |= pll_value;   // and set desired value
}

static void SetGpio(int gpio, GPIO_STATE_t state)
{
    unsigned long gpio_mask = GPIO_NUM_TO_MASK(gpio);
    volatile unsigned long *gpio_io_reg = &GPIO->GPIOio;
    volatile unsigned long *gpio_dir_reg = &GPIO->GPIODir;
#if 0     
#if !defined (CONFIG_BCM96338)
    if( gpio >= 32 )
    {
    gpio_mask = GPIO_NUM_TO_MASK_HIGH(gpio);
    gpio_io_reg = &GPIO->GPIOio_high;
    gpio_dir_reg = &GPIO->GPIODir_high;
    }
#endif 
#endif

    *gpio_dir_reg |= gpio_mask;

    if(state == GPIO_HIGH)
        *gpio_io_reg |= gpio_mask;
    else
        *gpio_io_reg &= ~gpio_mask;
}

int kerSysReleaseMacAddress( unsigned char *pucaMacAddr )
{
    return bhalReleaseMacAddress(pucaMacAddr);
}

 void dumpaddr( unsigned char *pAddr, int nLen )
 {
     static char szHexChars[] = "0123456789abcdef";
     char szLine[80];
     char *p = szLine;
     unsigned char ch, *q;
     int i, j;
     unsigned long ul;
 
     while( nLen > 0 )
     {
         sprintf( szLine, "%8.8lx: ", (unsigned long) pAddr );
         p = szLine + strlen(szLine);
 
         for(i = 0; i < 16 && nLen > 0; i += sizeof(long), nLen -= sizeof(long))
         {
             ul = *(unsigned long *) &pAddr[i];
             q = (unsigned char *) &ul;
             for( j = 0; j < sizeof(long); j++ )
             {
                 *p++ = szHexChars[q[j] >> 4];
                 *p++ = szHexChars[q[j] & 0x0f];
                 *p++ = ' ';
             }
         }
 
         for( j = 0; j < 16 - i; j++ )
             *p++ = ' ', *p++ = ' ', *p++ = ' ';
 
         *p++ = ' ', *p++ = ' ', *p++ = ' ';
 
         for( j = 0; j < i; j++ )
         {
             ch = pAddr[j];
             *p++ = (ch > ' ' && ch < '~') ? ch : '.';
         }
 
         *p++ = '\0';
         printk( "\r\n%s\r\n", szLine );
 
         pAddr += i;
     }
     //printk( "\r\n" );
 } /* dumpaddr */


#if 0
// write the nvramData struct to nvram after CRC is calculated 
static void writeNvramData(PNVRAM_DATA pNvramData)
{
    UINT32 crc = CRC32_INIT_VALUE;
    
    pNvramData->ulCheckSum = 0;
    crc = getCrc32((char *)pNvramData, sizeof(NVRAM_DATA), crc);      
    pNvramData->ulCheckSum = crc;
    kerSysNvRamSet((char *)pNvramData, sizeof(NVRAM_DATA), 0);
}

#endif

/*Read Wlan Params data from CFE */
int kerSysGetWlanSromParams( unsigned char *wlanParams, unsigned short len)
{
    /*
        此函数被如下模块调用，此模块是非开源模块，此接口需保留
        trunk\driver\broadcom\bcm963268\4.12\wlan\bcm963268\shared\bcmsrom.c
       */
    return 0;
}

EXPORT_SYMBOL(kerSysGetWlanSromParams);

/* start of add by w00165788 for DT 5V devices power supply */
#ifdef GPIO_CTRL_POWER
static void bhalplatSetPowerOn(void)
{
	if (GPIO_LOW == GPIO_CTRL_POWER_ACTIVE_PIN_STATE)
	{
		bhalplatsetGpio(GPIO_CTRL_POWER_ACTIVE_PIN | BP_ACTIVE_LOW, kGpioActive);
	}
	else
	{
		bhalplatsetGpio(GPIO_CTRL_POWER_ACTIVE_PIN, kGpioActive);
	}
}

static void bhalplatSetPowerOff(void)
{
	if (GPIO_LOW == GPIO_CTRL_POWER_ACTIVE_PIN_STATE)
	{
		bhalplatsetGpio(GPIO_CTRL_POWER_ACTIVE_PIN | BP_ACTIVE_LOW, kGpioInactive);
	}
	else
	{
		bhalplatsetGpio(GPIO_CTRL_POWER_ACTIVE_PIN, kGpioInactive);
	}
}
#endif
/* end of add by w00165788 for DT 5V devices power supply */

/* Start of add by l00164468 for DT 5V Dect and BCM4360 control */
#define GPIO_PERIPH_CTRL_SET(x) \
    ((((x) & BP_GPIO_NUM_MASK) >= 36) ? (1 << (((x) & BP_GPIO_NUM_MASK) - 32)) : (0))

#ifdef GPIO_CTRL_BCM4360_RF
static void bhalplatCtrlBCM4360RF(GPIO_STATE_t enGpioState)
{    
    GPIO->GPIOCtrl |= GPIO_PERIPH_CTRL_SET(GPIO_CTRL_BCM4360_RF_ACTIVE_PIN);

    if (GPIO_LOW == GPIO_CTRL_BCM4360_RF_ACTIVE_PIN_STATE)
    {
        bhalplatsetGpio(GPIO_CTRL_BCM4360_RF_ACTIVE_PIN | BP_ACTIVE_LOW, enGpioState);
    }
    else
    {
        bhalplatsetGpio(GPIO_CTRL_BCM4360_RF_ACTIVE_PIN, enGpioState);
    }
}
#endif

#ifdef GPIO_CTRL_DECT_RST
static void bhalplatCtrlDectRst(GPIO_STATE_t enGpioState)
{
    GPIO->GPIOCtrl |= GPIO_PERIPH_CTRL_SET(GPIO_CTRL_DECT_RST_ACTIVE_PIN);
	
    if (GPIO_LOW == GPIO_CTRL_DECT_RST_ACTIVE_PIN_STATE)
    {
        bhalplatsetGpio(GPIO_CTRL_DECT_RST_ACTIVE_PIN | BP_ACTIVE_LOW, enGpioState);
    }
    else
    {
        bhalplatsetGpio(GPIO_CTRL_DECT_RST_ACTIVE_PIN, enGpioState);
    }
}
#endif
/* End of add by l00164468 for DT 5V Dect and BCM4360 control */

/******************************************************************************
  函数名称  : ATP_BhalplatInit
  功能描述  : 方案相关的硬件初始化.
                           bhalinit中会统一调用这个初始化
  输入参数  : 
              无
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-3-10
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/

int ATP_BhalplatInit(void)
{
    /* Flash初始化 */
    //ATP_FLASH_InitFlash();
    /* Led 初始化 */
    ATP_LED_InitLeds();
    /* Board 初始化 */
    bhalbutton_init();
    bhalplatInitBoard();

/* start of add by w00165788 for DT 5V devices power supply */
#ifdef GPIO_CTRL_POWER
    bhalplatSetPowerOn();
#endif
/* end of add by w00165788 for DT 5V devices power supply */

/* Start of add by l00164468 for DT 5V Dect and BCM4360 control */
#ifdef GPIO_CTRL_BCM4360_RF
    bhalplatCtrlBCM4360RF(kGpioActive);
#endif

#ifdef GPIO_CTRL_DECT_RST
    bhalplatCtrlDectRst(kGpioActive);
#endif
/* End of add by l00164468 for DT 5V Dect and BCM4360 control */

    /*按键初始化*/

#ifdef SUPPORT_ATP_WPS         
    init_waitqueue_head(&g_stBhalPlatWaitQueue);
#endif
	
    return 0;
}

#define FLASHFLAGBACKOFFSET             0x100000    //Flag分区偏移Flash最末尾的长度
#define FLASHFLAGSIZE                       0x10000     //FlashFlag分区的大小
#define IMAGE_FLAG_SIZE           16

void kerSysEarlyFlashInit(void)
{
	char cboardid[32];
	char cvoiceboardid[32];
    memset(cboardid,0,32);
    memset( cvoiceboardid,0,32);
	memcpy(&g_flash_total_size,g_blparms_buf + ATP_BOARD_ID_LEN*3 + ATP_TAG_FLASH_MAX_INDEX*8,4);
	memcpy(&g_flash_sector_size,g_blparms_buf + ATP_BOARD_ID_LEN*3 + ATP_TAG_FLASH_MAX_INDEX*8 + 4,4);

	printk("g_flash_total_size:%x \n",g_flash_total_size);
	printk("g_flash_sector_size:%x \n",g_flash_sector_size);

	//memset(cboardid,0,16);
	memcpy(cboardid,g_blparms_buf,ATP_BOARD_ID_LEN);
	printk("id:%s \n",cboardid);
	memcpy(cvoiceboardid,g_blparms_buf + ATP_BOARD_ID_LEN,ATP_BOARD_ID_LEN);
	printk("voice id:%s \n",cvoiceboardid);
   
	if ((BpSetBoardId(cboardid) != BP_SUCCESS))
	{
		printk("\n*** Board is not initialized properly ***\n\n");
	}
#ifdef SUPPORT_ATP_VOICE
    if ((BpSetVoiceBoardId(cvoiceboardid) != BP_SUCCESS))
    {
        printk("\n*** Voice Board id is not initialized properly ***\n\n");
    }
#endif
}

/******************************************************************************
  函数名称  : bhalplatGetBoardVersion
  功能描述  : 获取Board版本号
  输入参数  : 
              1.  pcBuf:获取Board版本号值
              2.  ulLen: Board版本号的长度
  调用函数  : 
  被调函数  : 
  输出参数  : pcBuf
  返 回 值  : 1.VOS_ERROR 失败
              2.VOS_OK    成功

  修改历史      :
   1.日    期   : 2007-7-3
     修改内容   : 完成初稿

******************************************************************************/
int bhalplatGetBoardVersion(char *pcBuf, int ulLen)
{
    if (NULL == pcBuf)
    {        
        return -1;        
    }
    //strcpy(pcBuf, BOARD_VERSION);
    return 0;
}


/******************************************************************************
  函数名称  : bhalplatGetBoardPcbVersion
  功能描述  : 获取PCB版本号
  输入参数  : 
              1.  pcBuf:获取PCB版本号值
              2.  ulLen: PCB版本号的长度
  调用函数  : 
  被调函数  : 
  输出参数  : pcBuf
  返 回 值  : 1.VOS_ERROR 失败
              2.VOS_OK    成功

  修改历史      :
   1.日    期   : 2007-7-3
     修改内容   : 完成初稿

******************************************************************************/

int bhalplatGetBoardPcbVersion(char *pcBuf, int ulLen)
{
    if (NULL == pcBuf)
    {        
        return -1;        
    }
    //strcpy(pcBuf, PCB_VERSION);
    return 0;
}

/******************************************************************************
  函数名称  : bhalplatGetBoardManufacturer
  功能描述  : 获取Board厂商信息
  输入参数  : 
              1.  pcBuf:获取Board厂商信息值
              2.  ulLen: Board厂商信息的长度
  调用函数  : 
  被调函数  : 
  输出参数  : pcBuf
  返 回 值  : 1.VOS_ERROR 失败
              2.VOS_OK    成功

  修改历史      :
   1.日    期   : 2007-7-3
     修改内容   : 完成初稿

******************************************************************************/
int bhalplatGetBoardManufacturer(char *pcBuf, int ulLen)
{
    if (NULL == pcBuf)
    {        
        return -1;        
    }
    //strcpy(pcBuf, MANUFACTURER);
    return 0;
}
#ifdef SUPPORT_ATP_NANDFLASH    
int ATP_EraseConfig(char * pacType)
{
    int i;
    struct mtd_info *mtd = NULL;
    struct erase_info erase;
    int ret = 0;
    if ((NULL == pacType)||(0 == strlen(pacType)))
    {
        return -1;
    }
    memset(&erase,0,sizeof(struct erase_info));

    for (i = 0; i < MAX_MTD_DEVICES; i++) 
    {
        mtd = get_mtd_device(NULL, i);
        if (mtd)
        {
            if (0 == strcmp(mtd->name, pacType))
            {
                break;
            }
            put_mtd_device(mtd);
        }
    }

    if (MAX_MTD_DEVICES == i)
        return -1;

    erase.addr = 0;
    erase.len = mtd->size;
    erase.mtd = mtd;
    erase.callback = NULL;
    erase.priv = NULL;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
    ret = mtd->_erase(mtd, &erase);	
#else
    ret = mtd->erase(mtd, &erase);	
#endif	

    put_mtd_device(mtd);

    return ret;
}
#endif

/*modify by y00181549 for 当cms挂掉之后无法从用户态回复出场则从内核回复的机制at 20121201*/
static void reset_remedy()
{
    int ret;
    printk("come in reset_remedy %d \r\n",g_bCmsIsOK);
    if (FALSE == g_bCmsIsOK)
    {
        printk("cms not initialized, and reset key is press, will erase config! \r\n");
#ifdef SUPPORT_ATP_NANDFLASH 
        ret = ATP_EraseConfig("config");
#ifdef SUPPORT_ATP_CONFIG_BACKUP
        ret = ATP_EraseConfig("configbak");
#endif
#endif
        bhalplatSoftReboot();
    }
    del_timer(&g_stRemedyResetTimer);
}

void reset_func()
{
    printk("reset_func\n");
    init_timer(&g_stRemedyResetTimer);
    g_stRemedyResetTimer.function = reset_remedy;
    g_stRemedyResetTimer.expires =jiffies + 15*BHAL_POLLCNT_1SEC;
    add_timer(&g_stRemedyResetTimer);
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_RESET, NULL, 0);

}
/*modify by y00181549 for 当cms挂掉之后无法从用户态回复出场则从内核回复的机制at 20121201*/

void reset2_func()
{
    printk("reset2_func\n");
    //bhalplatSoftReboot();
	int msg = 0;
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_RESET, &msg, 4);
}

#if defined (SUPPORT_ATP_WLAN)
void wps_func()
{
    //printk("wps_func\n");
    WPS_FLAG = 1;
#ifdef SUPPORT_ATP_WLAN_DT
	g_iWpsProcStatusFlag = 1;
	mod_timer(&g_stWpsProcStatusTimer, jiffies + BHAL_POLLCNT_1SEC * 120);
#endif
    wake_up_interruptible(&g_stBhalPlatWaitQueue);
#ifdef SUPPORT_ATP_VOICE_DT //DT: WPS和DECT对码共用一个按键 
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_DECT_PP, NULL, 0);
#endif
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_WPS, NULL, 0);
}

void wlan_func()
{
    printk("wlan_func\n");
    /* START MODIFY by y00181549 for DTS2013101803442 at 20131129*/
    if (TRUE == g_bWlanSettingBusy)
    {
        Touch_Buzzer_Sound();
        udelay(100000);
        Touch_Buzzer_Sound();
        always_printk("Wlan Setting is Busy\r\n");
    }
    else
    {
        Touch_Buzzer_Sound();
        syswatch_nl_send(ATP_MSG_MONITOR_EVT_WLAN, NULL, 0);
    }
    /* START MODIFY by y00181549 for DTS2013101803442 at 20131129*/
}
#endif

void dectsearch_func()
{
    printk("dectsearch_func\n");
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_DECT_SEARCH, NULL, 0);
}

//static spinlock_t atp_buttonlock;

signed long long GetGpioKeyStatus(unsigned short unGpio)
{
    unsigned long long ullGpioMask;
    volatile unsigned long long *pullGpioReg;
    signed long long sllRet = -1;
    //spin_lock(&atp_buttonlock);

    ullGpioMask = GPIO_NUM_TO_MASK(unGpio);
    pullGpioReg = &GPIO->GPIOio;
    sllRet = (unsigned long long) *pullGpioReg & ullGpioMask;
    
    //spin_unlock(&atp_buttonlock);
    return sllRet;
}

/*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
signed long long GetI2cKeyStatus(unsigned char ucI2cNum)
{
    signed long long sllRet = -1;
#ifdef SUPPORT_ATP_GPIO_I2C	
    char readKeyStatus = 0;

    if (Touch_Read_Touch_Key(&readKeyStatus))
    {
        //printk("Read I2c Board error. \r\n");
        return sllRet;
    }

    if (ucI2cNum == readKeyStatus)
    {
        sllRet = 0;
    }
#endif    
    return sllRet;
}
/*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/

static irqreturn_t button_isr(int irq, void *dev_id)
{
    mod_timer(&g_buttontimer, jiffies + BHAL_POLLCNT_1SEC/8);

    return IRQ_HANDLED;
}

static signed long long Btn_pressed(unsigned short Btn_irq)
{
	if ((Btn_irq >= INTERRUPT_ID_EXTERNAL_0) && (Btn_irq <= INTERRUPT_ID_EXTERNAL_3)) 
	{
		if (!(PERF->ExtIrqCfg & (1 << (Btn_irq - INTERRUPT_ID_EXTERNAL_0 + EI_STATUS_SHFT)))) 
		{
			return 1;
		}
	}
#ifdef INTERRUPT_ID_EXTERNAL_4
	else if ((Btn_irq >= INTERRUPT_ID_EXTERNAL_4) || (Btn_irq <= INTERRUPT_ID_EXTERNAL_5)) 
	{
		if (!(PERF->ExtIrqCfg1 & (1 << (Btn_irq - INTERRUPT_ID_EXTERNAL_4 + EI_STATUS_SHFT))))
		{
			return 1;
		}
	}
#endif

	return 0;
}

int GeIrqKeyStatus(unsigned short Btn_irq)
{
    return (!Btn_pressed(Btn_irq));
}

#define BUTTON_GPIO 0
#define BUTTON_IRQ  1
#define BUTTON_I2C  2   // ADD: l00252599 FOR I2C KEY

static void Gpio_init()
{
    static char cinit_once = 0;
    int i = 0;
    short pin = 0;
    if(0 == cinit_once)
    {
#if 0
        for(i = 0;i<BHAL_KEY_END;i++)
        {
            if(g_buttonlist[i].vaild != 1)
                continue;
            
            if(g_buttonlist[i].keymode == BUTTON_GPIO)
            {
                pin = g_buttonlist[i].buttonPin & (~BP_ACTIVE_LOW);
                if((pin>7) && (pin<16))
                {
                    printk("Set Gpio:LedCtrl\n");
                    GPIO->LEDCtrl &= ~(1<<pin);
                }
            }
        }
#endif
#if 0
		GPIO->LEDCtrl &= ~(1<<8);
		GPIO->LEDCtrl &= ~(1<<9);
		GPIO->LEDCtrl &= ~(1<<10);
		GPIO->LEDCtrl &= ~(1<<11);
		GPIO->LEDCtrl &= ~(1<<12);
		GPIO->LEDCtrl &= ~(1<<13);
		GPIO->LEDCtrl &= ~(1<<14);
		GPIO->LEDCtrl &= ~(1<<15);

		GPIO->LEDCtrl &= ~(1<<0);
		GPIO->LEDCtrl &= ~(1<<1); 

		LED->ledHWDis |= 0xFFFFFF;
#endif
        cinit_once = 1;
    }
    return;
}
/*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
void bhalI2cBuzzerSound(void)
{
#ifdef SUPPORT_ATP_GPIO_I2C
    Touch_Buzzer_Sound();
#endif	
    return;
}

void bhalplatPollbutton(unsigned long data)
{
    int uiButtonfunc = 0;    
    signed long long  sllRet = -1;
    int i = 0;

    if (g_bIsupgrading)
    {
        mod_timer(&g_buttontimer, jiffies + BHAL_POLLCNT_1SEC/COUNTS_PSEC);
        return;  //When upgrading all key not active
    }

    
    //printk("bhalplatPollbutton ");
    Gpio_init();
    for(i = 0;i<BHAL_KEY_END;i++)
    {
        sllRet = -1;
        if(g_buttonlist[i].vaild != 1)
            continue;
        
        if (BUTTON_GPIO == g_buttonlist[i].keymode)
        {
            sllRet = GetGpioKeyStatus(g_buttonlist[i].buttonPin);
        }
        /*START ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/
#ifdef SUPPORT_ATP_GPIO_I2C		
        else if ((BUTTON_I2C == g_buttonlist[i].keymode) && (BUTTON_I2C == data))
        {
            sllRet = GetI2cKeyStatus((char)g_buttonlist[i].buttonPin);
            BcmHalInterruptEnable(g_usI2cIrq);
        }
#endif		
        /*END ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/		
        else if (BUTTON_IRQ == g_buttonlist[i].keymode)
        {
            //sllRet = (GeIrqKeyStatus(g_buttonlist[i].buttonPin));
            sllRet = GeIrqKeyStatus(g_buttonlist[i].buttonPin);
            BcmHalInterruptEnable(g_buttonlist[i].buttonPin);
        }
        else
        {
            /* START ADD: l00252599 解决RESET不生效问题 2013-11-20 */
            mod_timer(&g_buttontimer,jiffies + BHAL_POLLCNT_1SEC/COUNTS_PSEC);
            /* END ADD: l00252599 解决RESET不生效问题 2013-11-20 */		
            return;
        }
            
        if (sllRet == 0) 
        {
            if (BHAL_KEYSCAN_MODE_EQUIP== g_ulBhalKeyScanMode)
            {
                g_buttonlist[i].keystatus= BHAL_KEY_STATE_PRESS;
				g_buttonlist[i].keycount = 0;
                
                if ((BUTTON_I2C == g_buttonlist[i].keymode) &&
                    (BUTTON_WPS_ACTIVE_PIN == g_buttonlist[i].buttonPin))
                {
                    bhalI2cBuzzerSound();
                }
            }
            else
            {
                //printk("bhalplatPollbutton %d press!!!\n",i);
                g_buttonlist[i].keycount++;  

                if(0 !=  g_buttonlist[i].count2
                && g_buttonlist[i].keycount == g_buttonlist[i].count2)
                {
                    /* MODIFY by y00181549 for DTS2013101803442 at 20131129*/
                    if ((BUTTON_I2C == g_buttonlist[i].keymode) && (BHAL_KEY_SWITCH_WLAN != i))
                    {
                        bhalI2cBuzzerSound();
                    }
                    
                    g_buttonlist[i].keystatus= BHAL_KEY_STATE_PRESS;
#ifdef SUPPORT_ATP_VOICE_DT
                    if (1 != g_ulUrgentcallStatus)
#endif
                    {
                        if(g_buttonlist[i].fun2 != NULL)
                            g_buttonlist[i].fun2();
                    }

                    if (BUTTON_I2C == g_buttonlist[i].keymode)
                    {
                        g_buttonlist[i].keycount = 0;
                    }
            	
                    g_buttonlist[i].buttonwork = 1;//长按生效后不响应短按
                }
            }
        }
        else if (sllRet > 0)
        {
            if(1 == g_buttonlist[i].buttonwork)
            {
            	   g_buttonlist[i].keycount = 0;
                g_buttonlist[i].buttonwork = 0;
                continue;
            }
        
            if(g_buttonlist[i].keycount >=  g_buttonlist[i].count1)
            {
                g_buttonlist[i].keystatus= BHAL_KEY_STATE_PRESS;

                if (BHAL_KEYSCAN_MODE_NORMAL == g_ulBhalKeyScanMode)
                {
#ifdef SUPPORT_ATP_VOICE_DT
                	if (1 != g_ulUrgentcallStatus)
#endif
                	{
                    	if(g_buttonlist[i].fun1 != NULL)
                        	g_buttonlist[i].fun1();
                	}
                }
                g_buttonlist[i].keycount = 0;
            }
        }
    } 

    if (sllRet >= 0)
    {
        //g_buttonlist[i].timer.expires = jiffies + BHAL_POLLCNT_1SEC/COUNTS_PSEC;
    }
    //printk("over\n");
    mod_timer(&g_buttontimer,jiffies + BHAL_POLLCNT_1SEC/COUNTS_PSEC);
}
/*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/

/*START ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/
#ifdef SUPPORT_ATP_GPIO_I2C
static irqreturn_t button_i2c_isr(int irq, void *dev_id)
{
    bhalplatPollbutton(BUTTON_I2C);
    return IRQ_HANDLED;
}
#endif
/*END ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/

void button_Create(ATP_BHAL_KEY_EN keytype,int mod,short pin,int pinstate,
int timepart1,int timepart2,int countstep1,int countstep2,void (*func1)(),void (*func2)())
{
    //printk(">>>>>>button_Create %d<<<<<<<<<<\n",keytype);
    g_buttonlist[keytype].vaild = 1;
    g_buttonlist[keytype].keymode = mod;

    //printk("button pin:0x%x mod:0x%x: pinstate:0x%x:\n",pin,mod,pinstate);

    if(g_buttonlist[keytype].keymode == BUTTON_GPIO)//处理gpio按键
    {        
        g_buttonlist[keytype].buttonPin = pin|BP_ACTIVE_LOW; 

        printk("Button gpio: 0x%x.\r\n",g_buttonlist[keytype].buttonPin);
        GPIO->GPIODir &= ~ GPIO_NUM_TO_MASK(g_buttonlist[keytype].buttonPin);  
    }
    /*START ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/
#ifdef SUPPORT_ATP_GPIO_I2C	
    else if (g_buttonlist[keytype].keymode == BUTTON_I2C) // for i2c key
    {
        g_buttonlist[keytype].buttonPin = pin;
        if (BP_SUCCESS == BpGetTouchBoardIntrNum(&g_usI2cIrq))
        {
            g_usI2cIrq = map_external_irq (g_usI2cIrq);
            BcmHalMapInterrupt((FN_HANDLER)button_i2c_isr, 0, g_usI2cIrq);
            BcmHalInterruptEnable(g_usI2cIrq); 
        }
    }
    /*END ADD: l00252599 FOR SUPPORT I2C INTERRUPT 2013-11-18*/
#endif	
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
    else//处理irq按键
    {
        g_buttonlist[keytype].buttonPin = map_external_irq (pin);
        
        printk("Button IRQ: 0x%x.\r\n",g_buttonlist[keytype].buttonPin);
    	if(BcmHalMapInterrupt((FN_HANDLER)button_isr, 0, g_buttonlist[keytype].buttonPin)) 
    	{
    		printk("\nbhalplatInitBoard: Interrupt mapping failed\n");
    	}
    	BcmHalInterruptEnable(g_buttonlist[keytype].buttonPin);
    }

    g_buttonlist[keytype].buttonwork = 0;

    if((0 != countstep2) && (countstep2 < countstep1))
    {
        g_buttonlist[keytype].vaild = 0;
        return;    
    }
    
    if(countstep1 < 0)
    {
        g_buttonlist[keytype].count1 = 0;
    }
    else
    {
        g_buttonlist[keytype].count1 = countstep1;
    }
    
    if(countstep2 < 0)
    {
        g_buttonlist[keytype].count2 = 0;
    }
    else
    {
        g_buttonlist[keytype].count2 = countstep2;
    }

    //没有设定长按和短按参数
    if((0 == countstep1) && (0 == countstep2))
    {
        g_buttonlist[keytype].vaild = 0;
        return;    
    }

    g_buttonlist[keytype].fun1 = func1;
    g_buttonlist[keytype].fun2 = func2;
    
    //printk(">>>>>>button_Create over<<<<<<<<<<\n");
}

void bhalbutton_init()
{
    int i = 0;
    for(i = BHAL_KEY_RESET;i<BHAL_KEY_END;i++)
    {
        g_buttonlist[i].vaild = 0;
        g_buttonlist[i].fun1 = NULL;
        g_buttonlist[i].fun2 = NULL;
        g_buttonlist[i].count1 = 0;
        g_buttonlist[i].count2 = 0;
        g_buttonlist[i].keycount = 0;
        g_buttonlist[i].keystatus = BHAL_KEY_STATE_NOPRESS;
        g_buttonlist[i].keymode = 0;
    }

#ifdef BUTTON_RESET
#ifdef BUTTON_POPUP_RESTORE_DEF
    button_Create(BHAL_KEY_RESET,BUTTON_RESET_MODE,BUTTON_RESET_ACTIVE_PIN,BUTTON_RESET_ACTIVE_PIN_STATE,
    0,0,BUTTON_RESET_COUNTSTEP1,BUTTON_RESET_COUNTSTEP2,
    reset_func,reset2_func);
#else
    button_Create(BHAL_KEY_RESET,BUTTON_RESET_MODE,BUTTON_RESET_ACTIVE_PIN,BUTTON_RESET_ACTIVE_PIN_STATE,
    0,0,BUTTON_RESET_COUNTSTEP1,BUTTON_RESET_COUNTSTEP2,
    reset2_func,reset_func);
#endif
#endif

#ifdef SUPPORT_ATP_WLAN
#ifdef BUTTON_WPS
    button_Create(BHAL_KEY_SWITCH_WPS,BUTTON_WPS_MODE,BUTTON_WPS_ACTIVE_PIN,BUTTON_WPS_ACTIVE_PIN_STATE,
    0,0,BUTTON_WPS_COUNTSTEP1,BUTTON_WPS_COUNTSTEP2,
    NULL,wps_func);
#endif

#ifdef BUTTON_WLAN
    /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C
    button_Create(BHAL_KEY_SWITCH_WLAN,BUTTON_WLAN_MODE,BUTTON_WLAN_ACTIVE_PIN,BUTTON_WLAN_ACTIVE_PIN_STATE,
    0,0,BUTTON_WLAN_COUNTSTEP1,BUTTON_WLAN_COUNTSTEP2,
    NULL,wlan_func);
#else
    button_Create(BHAL_KEY_SWITCH_WLAN,BUTTON_WLAN_MODE,BUTTON_WLAN_ACTIVE_PIN,BUTTON_WLAN_ACTIVE_PIN_STATE,
    0,0,BUTTON_WLAN_COUNTSTEP1,BUTTON_WLAN_COUNTSTEP2,
    wlan_func,NULL);
#endif
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#endif

#ifdef BUTTON_WLAN5G
    button_Create(BHAL_KEY_SWITCH_WLAN5G,BUTTON_WLAN5G_MODE,BUTTON_WLAN5G_ACTIVE_PIN,BUTTON_WLAN5G_ACTIVE_PIN_STATE,
    0,0,BUTTON_WLAN5G_COUNTSTEP1,BUTTON_WLAN5G_COUNTSTEP2,
    NULL,NULL);
#endif
#endif

#if 1
#ifdef BUTTON_DECT //DECT Register
    button_Create(BHAL_KEY_SWITCH_DECT_PP,BUTTON_DECT_MODE,BUTTON_DECT_ACTIVE_PIN,BUTTON_DECT_ACTIVE_PIN_STATE,
    0,0,BUTTON_DECT_COUNTSTEP1,BUTTON_DECT_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_DECTPAGE //DECT Search
    button_Create(BHAL_KEY_SWITCH_DECT_PAGE,BUTTON_DECTPAGE_MODE,BUTTON_DECTPAGE_ACTIVE_PIN,BUTTON_DECTPAGE_ACTIVE_PIN_STATE,
    0,0,BUTTON_DECTPAGE_COUNTSTEP1,BUTTON_DECTPAGE_COUNTSTEP2,
    NULL,dectsearch_func);
#endif
#endif

#ifdef BUTTON_LCD
    button_Create(BHAL_KEY_SWITCH_LCD,BUTTON_LCD_MODE,BUTTON_LCD_ACTIVE_PIN,BUTTON_LCD_ACTIVE_PIN_STATE,
    0,0,BUTTON_LCD_COUNTSTEP1,BUTTON_LCD_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_LCDPAGE
    button_Create(BHAL_SCREEN_TIMER_LCD,BUTTON_LCDPAGE_MODE,BUTTON_LCDPAGE_ACTIVE_PIN,BUTTON_LCDPAGE_ACTIVE_PIN_STATE,
    0,0,BUTTON_LCDPAGE_COUNTSTEP1,BUTTON_LCDPAGE_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_LCDESURE
    button_Create(BHAL_KEY_ENSURE_LCD,BUTTON_LCDESURE_MODE,BUTTON_LCDESURE_ACTIVE_PIN,BUTTON_LCDESURE_ACTIVE_PIN_STATE,
    0,0,BUTTON_LCDESURE_COUNTSTEP1,BUTTON_LCDESURE_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER1
    button_Create(BHAL_KEY_OTHER1,BUTTON_OTHER1_MODE,BUTTON_OTHER1_ACTIVE_PIN,BUTTON_OTHER1_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER1_COUNTSTEP1,BUTTON_OTHER1_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER2
    button_Create(BHAL_KEY_OTHER2,BUTTON_OTHER2_MODE,BUTTON_OTHER2_ACTIVE_PIN,BUTTON_OTHER2_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER2_COUNTSTEP1,BUTTON_OTHER2_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER3
    button_Create(BHAL_KEY_OTHER3,BUTTON_OTHER3_MODE,BUTTON_OTHER3_ACTIVE_PIN,BUTTON_OTHER3_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER3_COUNTSTEP1,BUTTON_OTHER3_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER4
    button_Create(BHAL_KEY_OTHER4,BUTTON_OTHER4_MODE,BUTTON_OTHER4_ACTIVE_PIN,BUTTON_OTHER4_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER4_COUNTSTEP1,BUTTON_OTHER4_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER5
    button_Create(BHAL_KEY_OTHER5,BUTTON_OTHER5_MODE,BUTTON_DECTPAGE_ACTIVE_PIN,BUTTON_OTHER5_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER5_COUNTSTEP1,BUTTON_OTHER5_COUNTSTEP2,
    NULL,NULL);
#endif

#ifdef BUTTON_OTHER6
    button_Create(BHAL_KEY_OTHER6,BUTTON_OTHER6_MODE,BUTTON_OTHER6_ACTIVE_PIN,BUTTON_OTHER6_ACTIVE_PIN_STATE,
    0,0,BUTTON_OTHER6_COUNTSTEP1,BUTTON_OTHER6_COUNTSTEP2,
    NULL,NULL);
#endif

    init_timer(&g_buttontimer);
    g_buttontimer.expires =jiffies + 10*BHAL_POLLCNT_1SEC;
    g_buttontimer.function = bhalplatPollbutton;
    //g_buttontimer.data = keytype;
    add_timer(&g_buttontimer);

#ifdef SUPPORT_ATP_WLAN_DT
	init_timer(&g_stWpsProcStatusTimer);
    g_stWpsProcStatusTimer.function = bhalplatClearWpsProcStatus;
#endif

    for(i = BHAL_KEY_RESET;i<BHAL_KEY_END;i++)
    {
        if(0 != g_buttonlist[i].vaild)
        {
            //printk("i:%d count 1:%d count2:%d key count:%d \n",i
        		//,g_buttonlist[i].count1
        		//,g_buttonlist[i].count2
        		//,g_buttonlist[i].keycount);
        }
    /*    
        g_buttonlist[i].vaild = 0;
        g_buttonlist[i].fun1 = NULL;
        g_buttonlist[i].fun2 = NULL;
        g_buttonlist[i].count1 = 0;
        g_buttonlist[i].count2 = 0;
        g_buttonlist[i].keycount = 0;
        g_buttonlist[i].keystatus = BHAL_KEY_STATE_NOPRESS;
        g_buttonlist[i].keymode = 0;
        */
    }

    return;
}

/***************************************************************************
 * Function Name: kerSysGetChipId
 * Description  : Map id read from device hardware to id of chip family
 *                consistent with  BRCM_CHIP
 * Returns      : chip id of chip family
 ***************************************************************************/
int kerSysGetChipId() { 
    int r;
    
    r = (int) ((PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT);
    /* Force BCM681x variants to be be BCM6816) */
    if( (r & 0xfff0) == 0x6810 )
        r = 0x6816;

    /* Force BCM6369 to be BCM6368) */
    if( (r & 0xfffe) == 0x6368 )
        r = 0x6368;

    /* Force BCM63168, BCM63169, and BCM63269 to be BCM63268) */
    if( ( (r & 0xffffe) == 0x63168 )
      || ( (r & 0xffffe) == 0x63268 ))
        r = 0x63268;

    return(r);
}

//Luther debug for DSL
/***************************************************************************
 * Function Name: kerSysGetDslPhyEnable
 * Description  : returns true if device should permit Phy to load
 * Returns      : true/false
 ***************************************************************************/
int kerSysGetDslPhyEnable() {
        int id;
        int r = 1;
        id = (int) ((PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT);
        if ((id == 0x63169) || (id == 0x63269)) {
	    r = 0;
        }
        return(r);
}

int bhalplatGetSwichMibTxPkts(int port, unsigned int * ulTxPkts)
{
	int base;
	volatile EthSwMIBRegs *pstEthSwMIBRegs;

	if ((port < 0) || (port > 4)) 
	{
        printk("\bhalplatGetSwichMibTxPkts: Invalid port number\n");
        return -1;
    }
	
	base = (SWITCH_BASE + 0x2000 + (port * 0x100));
	pstEthSwMIBRegs = (volatile EthSwMIBRegs *)base;

	*ulTxPkts = pstEthSwMIBRegs->TxUnicastPkts + pstEthSwMIBRegs->TxMulticastPkts + pstEthSwMIBRegs->TxBroadcastPkts;

	return 0;
}

int bhalplatGetSwichMibRxPkts(int port, unsigned int * ulRxPkts)
{
	int base;
	volatile EthSwMIBRegs *pstEthSwMIBRegs;
	
	if ((port < 0) || (port > 4)) 
	{
        printk("\bhalplatGetSwichMibRxPkts: Invalid port number\n");
        return -1;
    }
	
	base = (SWITCH_BASE + 0x2000 + (port * 0x100));
	pstEthSwMIBRegs = (volatile EthSwMIBRegs *)base;

	*ulRxPkts = pstEthSwMIBRegs->RxUnicastPkts + pstEthSwMIBRegs->RxMulticastPkts + pstEthSwMIBRegs->RxBroadcastPkts;

	return 0;
}

#ifdef GPIO_CTRL_POWER
void bhalplatSetUsbPowerDown(void)
{
	bhalplatSetPowerOff();
}
#endif

#ifdef SUPPORT_ATP_VOICE_DT
int bhalplatSetUrgentCallStatus(unsigned int ulStatus)
{
	g_ulUrgentcallStatus = ulStatus;	
	return 0;
}
#endif

#ifdef SUPPORT_ATP_WLAN_DT
static void bhalplatClearWpsProcStatus(unsigned long ulData)
{
	g_iWpsProcStatusFlag = 0;

	return;
}

int bhalplatSetWpsProcStatus(void)
{
	g_iWpsProcStatusFlag = 1;
	mod_timer(&g_stWpsProcStatusTimer, jiffies + BHAL_POLLCNT_1SEC * 120);

	return 0;
}

int bhalplatGetWpsProcStatus(void)
{
	return g_iWpsProcStatusFlag;
}

int bhalplatClrWpsProcStatus(void)
{
	g_iWpsProcStatusFlag = 0;
	
	return 0;
}
#endif

EXPORT_SYMBOL(kerSysGetSdramSize);
EXPORT_SYMBOL(kerSysGetMacAddress);
EXPORT_SYMBOL(kerSysGetMacAddressType);
EXPORT_SYMBOL(kerSysReleaseMacAddress);
#ifndef BRCM_DAYING_GASP
EXPORT_SYMBOL(kerSysRegisterDyingGaspHandler);
EXPORT_SYMBOL(kerSysDeregisterDyingGaspHandler);
#endif
EXPORT_SYMBOL(kerSysSendtoMonitorTask);
EXPORT_SYMBOL(BpUpdateWirelessSromMap);
EXPORT_SYMBOL(BpGetWirelessFlags);
EXPORT_SYMBOL(kerSysSetGpio);
EXPORT_SYMBOL(BpGetBoardId);
EXPORT_SYMBOL(BpGetVoiceParms);
EXPORT_SYMBOL(BpGetVoiceBoardId);
EXPORT_SYMBOL(kerSysGetChipId);
EXPORT_SYMBOL(kerSysGetDslPhyEnable); //Luther debug for DSL
EXPORT_SYMBOL(bcm_gpio_spinlock); //Luther debug for DSL

