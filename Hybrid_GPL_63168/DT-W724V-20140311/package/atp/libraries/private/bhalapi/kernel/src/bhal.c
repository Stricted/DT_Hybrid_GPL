/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : bhal.c
  作    者  : l69021
  版    本  : 1.0
  创建日期  : 2007-6-30
  描    述  : This file contains Linux character device driver entry
*              for the board related ioctl calls: flash, get free kernel
*              page and dump kernel memory, etc.
  函数列表  :

  历史记录      :
   1.日    期   : 2007-6-30
     作    者   : l69021
     修改内容   : 完成初稿

*****************************************************************************/
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/capability.h>

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/pagemap.h>
//#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/if.h>
#include <linux/timer.h>
#include "atpflash.h"
#include "bhalplat.h"
#include "bhal.h"
#include "atpconfig.h"
#if defined(CONFIG_MIPS_BRCM)
#include "board.h"
#endif
#include <linux/vmalloc.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
#include <linux/slab.h>
#endif
#include "boardparms.h"
#include "linux/delay.h"

/* START ADD by y00181549 for DTS2013101803442 at 20131129*/
 #include "msg/kcmsmonitormsgtypes.h"
 /* END ADD by y00181549 for DTS2013101803442 at 20131129*/

#ifdef BRCM_DAYING_GASP
#include <bcm_intr.h>

typedef void (*cb_dgasp_t)(void *arg);

typedef struct _CB_DGASP__LIST
{
    struct list_head list;
    char name[IFNAMSIZ];
    cb_dgasp_t cb_dgasp_fn;
    void *context;
}CB_DGASP_LIST , *PCB_DGASP_LIST;

static CB_DGASP_LIST *g_cb_dgasp_list_head = NULL;
int g_SysDyingGaspIsr = 1;
#endif

extern ATP_KEY_ST g_buttonlist[BHAL_KEY_END];
extern struct timer_list g_buttontimer;
extern WLAN_DEV_INFO g_stWanDevList[WAN_MAX_NUM];

#ifdef SUPPORT_ATP_WLAN_DT
extern struct timer_list g_stWpsProcStatusTimer;
/* START ADD by y00181549 for DTS2013101803442 at 20131129*/
extern bool g_bWlanSettingBusy;
/* END ADD by y00181549 for DTS2013101803442 at 20131129*/
#endif

#ifdef SUPPORT_ATP_WLAN
extern WLAN_DEV_INFO g_stWlanDevList[WLAN_2G_SSID_NUM];
#endif

#ifdef SUPPORT_ATP_WATCHDOG
static DEFINE_SPINLOCK(bcmdog_lock);
int BhalHardWatchDogInit(VOS_VOID);
static irqreturn_t ext_timer_isr(int irq, unsigned int param);
struct timer_list   g_stBhalWatchdogTimer;      /*watchdog Timer*/
static void bhalWatchdogKeepAlive(unsigned long ulData);
unsigned int bcmdog_interrupt_num = 0;
irqreturn_t bcm_watchdog_interrupt(int irq, void *dev_id);
int g_RebootStopIsr = 0;
#endif

/* Timer */
unsigned int g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_NORMAL;
unsigned int g_ulBhalLedMode = BHAL_LED_MODE_NORMAL;
EXPORT_SYMBOL(g_ulBhalLedMode);
bool g_bIsupgrading = false;
EXPORT_SYMBOL(g_bIsupgrading);


unsigned long g_ulBhalMajor = 0; /* 主设备号*/

int g_lBhalFlashWriteFlag = BHAL_FLASH_IDLE;

static int g_lBhalVoipServiceState = BHAL_VOIP_OFF;

static wait_queue_head_t g_stBhalWaitQueue;

//BHAL_VARIABLE_SYS_PARAM_ST g_stBhalSysInfo;
BHAL_MAC_ADDR_INFO_ST MacAddrs[BHAL_MAC_NUMBERS_MAX];
char szBaseMacAddr[BHAL_OBLI_MAC_ADDRESS_LEN];
char macinit = 0;
EXPORT_SYMBOL(macinit);
EXPORT_SYMBOL(szBaseMacAddr);

    
unsigned long g_ulDSLActiveTime = 0; 
unsigned long g_ulDSLUpTime = 0; 

/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 start*/
bool g_bUmtsState = FALSE;
bool g_boldUmtsState = FALSE;

/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 start*/
/*start: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
int g_TagConsole = 0;
EXPORT_SYMBOL(g_TagConsole);
#endif
/*end: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/
bool g_bCmsIsOK = FALSE;

static int bhalOpen( struct inode *inode, struct file *filp );
static int bhalIoctl( struct inode *inode, struct file *flip, unsigned int command, unsigned long arg );
static int bhalRelease(struct inode *inode, struct file *filp);
static void bhalSetGpio(int gpio, BSP_E_GPIO_STATE enState);
static int bhalGetBoardParam(BSP_E_PARAM_TYPE enBoardParamType, char *pcBoardParam, int ulParamLen);
static void bhalPollResetKey(unsigned long ulData);

static void bhalSetGpio(int gpio, BSP_E_GPIO_STATE state);
static void bhalCtrlLed(BSP_E_LED_NAME enLedName, BSP_E_LED_STATE enLedState);
static int bhalSetBaseMacAddress( unsigned char *pucaMacAddr);
static void bhalGetBootloaderVersion(BHAL_IOCTL_PARMS_ST *stCtrlParms);
static int bhalSetBoardParam(BSP_E_PARAM_TYPE enBoardParamType, char *pcBoardParam, int ulParamLen);
static void *bhalRetriedKmalloc(unsigned long ulSize);
static void bhalRetriedKfree(void *pBuf);
static int bhalGetChipParam(ATP_BHAL_CHIP_PARAM_EN enChipParamType,unsigned long ulString,unsigned long ulSize);
static void bhalHardReboot(void);
#ifdef LED_SEQUENCE_BLINK
static void bhalSequenceBlinkLed(unsigned int iTime);
static void bhalPwrOnSequenceBlinkLed(unsigned int iTime);
#endif

#ifdef BRCM_DAYING_GASP
static irqreturn_t bhalDyingGaspIsr(int irq, void * dev_id);
static void __init bhalInitDyingGaspHandler( void );
static void __exit bhalDeinitDyingGaspHandler( void );
void kerSysRegisterDyingGaspHandler(char *devname, void *cbfn, void *context);
void kerSysDeregisterDyingGaspHandler(char *devname);
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
static void bhalSendLedTimes(unsigned int tmpLedTimes);
static void bhalGetLedTimes(unsigned int *LedRemainBlinkTimes);
#endif

#ifdef SUPPORT_ATP_DECT_VOICE
//static int bhalGetDectCalibParam(ATP_DECT_CALIB_TYPE_EN enDectCalibType, char *pcDectParam, int ulParamLen);
//static int bhalSetDectCalibParam(ATP_DECT_CALIB_TYPE_EN enDectCalibType, char *pcDectParam, int ulParamLen);
#endif

void bhalSoftReboot(void);

#ifdef SUPPORT_ATP_TCCONSOLE
void bhalSoftReset(void);
#endif
void __exit bhalCleanup( void );
int bhalGetBaseMacAddress( unsigned char *pucaMacAddr);

static int bhalWpsChange(int WPSMODE);
int bhalGetSdramSize(void);

extern int ATP_BhalplatInit(void);
extern void bhalplatSoftReboot(void);
extern void bhalplatHardReboot(void);
extern signed int bhalplatWpsChange(signed int wpsmode);
extern int bhalplatGetChipParam(ATP_BHAL_CHIP_PARAM_EN enChipParamType,unsigned long ulString,unsigned long ulSize);
extern int bhalplatGetProductType(char *pcBuf, int ulLen);
extern int bhalplatGetBoardAllId(char *pcBuf, int ulLen);
extern int bhalplatGetBoardManufacturer(char *pcBuf, int ulLen);
extern void  bhalplatsetGpio(int gpio, BSP_E_GPIO_STATE enState);

extern void ATP_LED_CtrlLed(BOARD_LED_NAME enLedName, BOARD_LED_STATE enLedState);
extern void ATP_LED_AllLed(unsigned int state, unsigned int color);
extern void bhalSequenceAllLed(int *data);

#ifdef LED_SEQUENCE_BLINK
extern void boardSequenceBlinkLed(unsigned int iTime);
extern void boardPwrOnSequenceBlinkLed(unsigned int iTime);
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
extern void SendLedBlinkTimes(unsigned int sTmpLedTimes);
extern void GetLedBlinkTimes(unsigned int *LedRemainBlinkTimes);
extern int GetLedState(BOARD_LED_NAME ledName);
#endif

#ifdef SUPPORT_ATP_VOICE_DT
extern int bhalplatSetUrgentCallStatus(unsigned int ulStatus);
extern int BpGetDectRestGpio( unsigned short *pusValue );

#endif

#ifdef SUPPORT_ATP_HYBRID
extern int BpGetLteRestGpio( unsigned short *pusValue );
#endif

#ifdef SUPPORT_ATP_WLAN_DT
extern int bhalplatSetWpsProcStatus(void);
extern int bhalplatGetWpsProcStatus(void);
extern int bhalplatClrWpsProcStatus(void);
#endif

#ifdef GPIO_CTRL_POWER
extern void bhalplatSetUsbPowerDown(void);
#endif

extern unsigned int  ATP_LED_ChangeStatus(BSP_E_LED_NAME enLedName, unsigned int ulStatus);

#if defined(CONFIG_QUALCOMM_CHIP)
extern void led_on(BOARD_SPEC_LED_NAME name);
extern void led_off(BOARD_SPEC_LED_NAME name);
extern void led_blink_time_ex(BOARD_SPEC_LED_NAME name, unsigned long last_second,
		unsigned long on_ms, unsigned long off_ms);
#endif

unsigned long getCrc32(const char *pdata, unsigned long size, unsigned long crc) ;

typedef struct
{
    unsigned long eventmask;
} BHAL_IOC_ST, *PBHAL_IOC_ST;
/*START ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
static long bhalunlockedIoctl(struct file *file, unsigned int cmd, unsigned long arg)
{   
    struct inode *inode = file->f_dentry->d_inode;
    long ret;
    
    ret = bhalIoctl(inode, file, cmd, arg);   
    return ret;
}
#endif
/*END ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/
static struct file_operations bhal_fops =
{
  open:       bhalOpen,
  /*START ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/  
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
  unlocked_ioctl:      bhalunlockedIoctl,
#else
  ioctl:      bhalIoctl,
#endif  
  /*END ADD: l81004871 FOR 适配3.4.11 内核的IOCTL 2013-08-08*/
  release:    bhalRelease,
};

/* 定义成MODULE */
#if defined(MODULE)
int init_module(void)
{
    return (bhalInit());
}

void cleanup_module(void)
{
    if (MOD_IN_USE)
        printk("bhal: cleanup_module failed because module is in use\n");
    else
        bhalCleanup();
}
#endif
/* 定义成MODULE */

/******************************************************************************
  函数名称  : bhal_init
  功能描述  :
  输入参数  :
              1.  void:
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static int __init bhalInit( void )
{
    int iRet = -1;

    iRet = register_chrdev(BHAL_DRV_MAJOR, "bhal", &bhal_fops );
    if (iRet < 0)
    {
        printk( "bhalInit(major %d): fail to reg dev.\n",BHAL_DRV_MAJOR);
    }
    else
    {
        printk("bhal: bhalInit entry\n");
        g_ulBhalMajor = BHAL_DRV_MAJOR;

        /*硬件相关的初始化为了统一将FLASH和LED的初始化都放在ATP_BhalplatInit*/
        ATP_BhalplatInit();

#ifdef BRCM_DAYING_GASP
        bhalInitDyingGaspHandler();
#endif
	
        init_waitqueue_head(&g_stBhalWaitQueue);

        /* 处理Mac地址 */
        //memset(&szBaseMacAddr, 0, sizeof(szBaseMacAddr));
        memset(&MacAddrs, 0, sizeof(MacAddrs));
    }
    
#ifdef SUPPORT_ATP_WATCHDOG
    BhalHardWatchDogInit();
#endif

#ifdef LED_SEQUENCE_BLINK
    bhalPwrOnSequenceBlinkLed(LED_SEQUENCE_BLINK_TIME);
#endif
		
    return iRet;
}

/******************************************************************************
  函数名称  : bhal_cleanup
  功能描述  :
  输入参数  :
              1.  void:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
void __exit bhalCleanup( void )
{
    if (g_ulBhalMajor != 0)
    {
        del_timer_sync(&g_buttontimer);
#ifdef SUPPORT_ATP_WLAN_DT
		del_timer(&g_stWpsProcStatusTimer);
#endif
        unregister_chrdev(g_ulBhalMajor, "board_ioctl");
    }
}

/******************************************************************************
  函数名称  : bhalIocalloc
  功能描述  :
  输入参数  :
              1. void:
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static BHAL_IOC_ST* bhalIocalloc(void)
{
    BHAL_IOC_ST *stBhalioc = NULL;
    stBhalioc = (BHAL_IOC_ST*) kmalloc( sizeof(BHAL_IOC_ST) , GFP_KERNEL );
    if (stBhalioc)
    {
        memset((void *)stBhalioc, 0, sizeof(BHAL_IOC_ST));
    }
    return stBhalioc;
}

/******************************************************************************
  函数名称  : bhalIocfree
  功能描述  :
  输入参数  :
              1.  bhal_ioc:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static void bhalIocfree(BHAL_IOC_ST* bhal_ioc)
{
    if (bhal_ioc)
    {
        kfree(bhal_ioc);
    }
}

/******************************************************************************
  函数名称  : bhalOpen
  功能描述  :
  输入参数  :
              1.  *inode:
              2.  *filp:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static int bhalOpen( struct inode *inode, struct file *filp )
{
    filp->private_data = bhalIocalloc();
    /*we do not use inode*/
    if (filp->private_data == NULL)
        return -ENOMEM;

    return( 0 );
}

/******************************************************************************
  函数名称  : BhalRelease
  功能描述  :
  输入参数  :
              1.  *inode:
              2.  *filp:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static int bhalRelease(struct inode *inode, struct file *filp)
{
    BHAL_IOC_ST *stBhalioc = filp->private_data;

    wait_event_interruptible(g_stBhalWaitQueue, 1);
    bhalIocfree(stBhalioc);
    return( 0 );
}

/******************************************************************************
  函数名称  : bhalIoctl
  功能描述  :
  输入参数  :
              1.  *inode:
              2.  *flip:
              3.  command:
              4.  arg:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static int bhalIoctl( struct inode *inode, struct file *flip,
                        unsigned int command, unsigned long arg )
{
    int i = 0;

    int iRet = -1;
    unsigned long currenttime = jiffies / HZ;   //add by s00124929 2009-12-7
    BHAL_IOCTL_PARMS_ST stCtrlParms;
#ifdef SUPPORT_ATP_INTERNET_LED_SINGTEL    
    bool bExist = FALSE; //ADD by w00182511 2011/11/28 FOR DTS2011112606784
#endif
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
	unsigned int LedRemainBlinkTimes = 0;
#endif

	BOARD_LED_ACTION_INFO led_action_info = {0};

    memset((void *)&stCtrlParms, 0, sizeof(BHAL_IOCTL_PARMS_ST));

    switch(command)
    {
        case BHAL_IOCTL_INIT_FLASH:
        {
            iRet = 0;
            break;
        }

        case BHAL_IOCTL_GET_FLASH_SIZE:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (iRet == 0)
            {
                switch(stCtrlParms.ulType)
                {   
                    /************用来获取flashblock大小******************/
                    case BSP_E_FLASH_AREA_END:
                        iRet = ATP_FLASH_GetBlockSize(stCtrlParms.ulOffset);
                        break;
                    break;
                
                    case BSP_E_FLASH_AREA_ALLFLASH:
                    {
                        iRet = ATP_FLASH_GetSize();
                        break;
                    }

                    default:
                    {
                        iRet = BHAL_ERROR;
                        break;
                    }
                }
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_GET_CHIP_PARAM:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                iRet = bhalGetChipParam((ATP_BHAL_CHIP_PARAM_EN)stCtrlParms.ulType, \
                                    0, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
    
        case BHAL_GET_DSLACTIVETIME:
        {
	        if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(BHAL_IOCTL_PARMS_ST)) == 0)
            {
	            __copy_to_user(stCtrlParms.pcString, &g_ulDSLActiveTime, sizeof(g_ulDSLActiveTime));
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }            
            break;
        }
        
        case BHAL_GET_DSLUPTIME:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(BHAL_IOCTL_PARMS_ST)) == 0)
            {
                __copy_to_user(stCtrlParms.pcString, &g_ulDSLUpTime, sizeof(g_ulDSLUpTime));
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;            
        }
        
        case BHAL_IOCTL_GET_CURRENT_TIME:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(BHAL_IOCTL_PARMS_ST)) == 0)
            {
                __copy_to_user(stCtrlParms.pcString, &currenttime, sizeof(currenttime));
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;            
        }

        case BHAL_IOCTL_CTRL_LED:
        {
			int data[6];
			static unsigned int keyscanmode = BHAL_KEYSCAN_MODE_NORMAL;
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                switch((BSP_E_LED_OPERATION)stCtrlParms.ulType)
                {
                    case BHAL_LED_SET:
                    bhalCtrlLed((BSP_E_LED_NAME)stCtrlParms.ulStrLen,(BSP_E_LED_STATE)stCtrlParms.ulOffset);
                    break;

                    case BHAL_LED_ALL_ON:
                    g_ulBhalLedMode = BHAL_LED_MODE_EQUIP;
                    keyscanmode = g_ulBhalKeyScanMode;
                    g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_EQUIP;
                    ATP_LED_AllLed(0,stCtrlParms.ulOffset);
                    break;

                    case BHAL_LED_ALL_OFF:
                    g_ulBhalLedMode = BHAL_LED_MODE_EQUIP;  
                    g_ulBhalKeyScanMode = keyscanmode;

					for (i = 0; i < WAN_MAX_NUM; i++)
					{
						memset(&g_stWanDevList[i], 0, sizeof(WLAN_DEV_INFO));
					}
					iRet = BHAL_OK;

#ifdef SUPPORT_ATP_WLAN

					for (i = 0; i < WLAN_2G_SSID_NUM; i++)
					{
						memset(&g_stWlanDevList[i], 0, sizeof(WLAN_DEV_INFO));
					}
					iRet = BHAL_OK;
#endif
                    ATP_LED_AllLed(1,stCtrlParms.ulOffset);
                    break;

                    case BHAL_LED_TIME_SET:
					g_ulBhalLedMode = BHAL_LED_MODE_EQUIP;  
                    g_ulBhalKeyScanMode = keyscanmode;
                    //ATP_LED_AllLed(1,3);
                    //ATP_LED_TimeSet((BSP_E_LED_NAME)stCtrlParms.ulStrLen,kLedStateLighttime,(int)stCtrlParms.ulOffset);
                    data[0] = stCtrlParms.ulStrLen;
                    data[1] = stCtrlParms.ulOffset;
                    bhalSequenceAllLed(&data[0]);
                    break;
					
#ifdef LED_SEQUENCE_BLINK
                    case BHAL_LED_SEQUENCE_BLINK:
                    data[0] = stCtrlParms.ulOffset;
                    bhalSequenceBlinkLed(data[0]);
                    break;
#endif

#if defined(SUPPORT_ATP_WLAN)
#if defined(SUPPORT_ATP_WPS)
#ifdef MTK_CHIP
					case BHAL_LED_WPS_SET:
					memcpy(data,stCtrlParms.pcString,24);
					mtk_wps_gpio_led_set(&data[0]);
					break;
#endif
#endif
#endif					

                    case BHAL_LED_TEST_ON:
                    g_ulBhalLedMode = BHAL_LED_MODE_EQUIP;
                    keyscanmode = g_ulBhalKeyScanMode;
                    g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_EQUIP;
                    break;
                    
                    case BHAL_LED_TEST_OFF:
                    g_ulBhalLedMode = BHAL_LED_MODE_NORMAL;
                    g_ulBhalKeyScanMode = keyscanmode;
                    g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_NORMAL;
                    break;

                    case BHAL_LED_TEST_STATE_SET:
                    ATP_LED_CtrlLed((BOARD_LED_NAME)stCtrlParms.ulStrLen,(BOARD_LED_STATE)stCtrlParms.ulOffset);
                    break;

                    case BHAL_LED_ALL_BLINK:
                    g_ulBhalLedMode = BHAL_LED_MODE_EQUIP;  
                    g_ulBhalKeyScanMode = keyscanmode;
                    ATP_LED_AllLed(2,stCtrlParms.ulOffset);
                    break;

		case BHAL_LED_ACTION:
#if defined(CONFIG_QUALCOMM_CHIP)
			if(NULL != stCtrlParms.pcBuf) {
				if(0 == copy_from_user(&led_action_info, stCtrlParms.pcBuf, sizeof(led_action_info))) {
					/*
					printk("%s Line %d: name = %d, action = %d, last = %d, on = %d, off = %d \n",
							__FUNCTION__, __LINE__,
							led_action_info.name, led_action_info.action,
							led_action_info.last, led_action_info.on, led_action_info.off);
					*/
					if(led_action_info.action == LED_ACTION_BLINK) {
						led_blink_time_ex(led_action_info.name, led_action_info.last,
								led_action_info.on, led_action_info.off);
					}
					else if(led_action_info.action == LED_ACTION_ON) {
						led_on(led_action_info.name);
					}
					else {
						 // LED_ACTION_OFF
						 led_off(led_action_info.name);
					}
				}
				else {
					printk("%s Line %d: erro \n", __FILE__, __LINE__);
				}
			}
#endif
			break;
                    
                    default:
                    break;
                }
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }

        case BHAL_IOCTL_SET_GPIO_BIT:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                bhalSetGpio(stCtrlParms.ulStrLen, (BSP_E_GPIO_STATE)stCtrlParms.ulOffset);
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        /*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
        case BHAL_IOCTL_GPIO_I2C_UPG:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = gpio_i2c_upg(stCtrlParms.pcBuf, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_GPIO_I2C_READ:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = gpio_i2c_read(stCtrlParms.pcBuf, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_GPIO_I2C_WRITE:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = gpio_i2c_write(stCtrlParms.pcBuf, stCtrlParms.ulStrLen);                
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_GPIO_ATMEL_I2C_READ:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = gpio_i2c_read_atmel(stCtrlParms.ulType, stCtrlParms.pcBuf, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_GPIO_ATMEL_I2C_WRITE:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = gpio_i2c_write_atmel(stCtrlParms.ulType, stCtrlParms.pcBuf, stCtrlParms.ulStrLen);                
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
#endif
        /*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/	
        case BHAL_IOCTL_GET_VOIP_STATE:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                iRet = g_lBhalVoipServiceState;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }

        case BHAL_IOCTL_CHECK_KEY_STATE:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                switch (stCtrlParms.ulType)
                {
                    case BHAL_KEY_RESET:
                    {
                        iRet = g_buttonlist[BHAL_KEY_RESET].keystatus;
                        g_buttonlist[BHAL_KEY_RESET].keystatus = BHAL_KEY_STATE_NOPRESS;
                        break;
                    }

                    case BHAL_KEY_SWITCH_WLAN:
                    {
                        iRet = g_buttonlist[BHAL_KEY_SWITCH_WLAN].keystatus;
                        g_buttonlist[BHAL_KEY_SWITCH_WLAN].keystatus = BHAL_KEY_STATE_NOPRESS;
                        break;
                    }

#if defined (SUPPORT_ATP_WPS)
                    case BHAL_KEY_SWITCH_WPS:
                    {
                        iRet = g_buttonlist[BHAL_KEY_SWITCH_WPS].keystatus;
                        g_buttonlist[BHAL_KEY_SWITCH_WPS].keystatus = BHAL_KEY_STATE_NOPRESS;
                        break;
                    }
#endif

#if defined (BUTTON_DECTPAGE)
                    case BHAL_KEY_SWITCH_DECT_PAGE:
                    {
                        iRet = g_buttonlist[BHAL_KEY_SWITCH_DECT_PAGE].keystatus;
                        g_buttonlist[BHAL_KEY_SWITCH_DECT_PAGE].keystatus = BHAL_KEY_STATE_NOPRESS;

                        break;
                    }
#endif		
                    default:
                    {
                        iRet = BHAL_KEY_STATE_NOKEY;
                        break;
                    }
                }
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }

        case BHAL_IOCTL_SOFT_REBOOT:
        {
            bhalSoftReboot();
            break;
        }

#ifdef SUPPORT_ATP_TCCONSOLE
        case BHAL_IOCTL_SOFT_RESET:
        {
            bhalSoftReset();          
            break;
        }
#endif       
        case BHAL_IOCTL_HARD_REBOOT:
        {
            bhalHardReboot();
            break;
        }

#if defined(SUPPORT_ATP_WLAN)
#if defined(SUPPORT_ATP_WPS)
        case BHAL_KEY_SWITCH_WLAN_WPS:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                bhalWpsChange(stCtrlParms.ulType);
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
#endif
#endif
        case BHAL_IOCTL_GET_BOOTLOADER_VERSION:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                bhalGetBootloaderVersion(&stCtrlParms);
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }

        case BHAL_IOCTL_GET_SDRAM_SIZE:
        {
            iRet = bhalGetSdramSize();
            break;
        }
        case BHAL_IOCTL_SETUP_KEYTEST_MODE:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                int i;
                if (BHAL_KEYSCAN_MODE_EQUIP == stCtrlParms.ulOffset)
                {
                    g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_EQUIP;
                }
                else
                {
                 	g_ulBhalKeyScanMode = BHAL_KEYSCAN_MODE_NORMAL;
                }
                
                for(i = BHAL_KEY_RESET;i<BHAL_KEY_END;i++)
                    g_buttonlist[i].keystatus = BHAL_KEY_STATE_NOPRESS;

                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
    
#ifdef SUPPORT_ATP_DECT_VOICE
        case BHAL_IOCTL_GET_DECT_CALIB_PARA:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                //iRet = bhalGetDectCalibParam((ATP_DECT_CALIB_TYPE_EN)stCtrlParms.ulType,
                //                            stCtrlParms.pcString, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
        case BHAL_IOCTL_SET_DECT_CALIB_PARA:
        {
            if (copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)) == 0)
            {
                //iRet = bhalSetDectCalibParam((ATP_DECT_CALIB_TYPE_EN)stCtrlParms.ulType,
                 //                           stCtrlParms.pcString, stCtrlParms.ulStrLen);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
       case BHAL_IOCTL_DECT_RESET:
       { 
           unsigned short gpio;
           iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
           if (0 == iRet)
           {
               if (BpGetDectRestGpio(&gpio) == BHAL_OK)
               {
                   GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);
                   GPIO->GPIOio &= ~ GPIO_NUM_TO_MASK(gpio);
                   udelay(100);
                   GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);
                   GPIO->GPIOio |= GPIO_NUM_TO_MASK(gpio);
               }
               iRet = BHAL_OK;
           }
           else
           {
               iRet = BHAL_ERROR;
           }
           break;
       }
#endif

#ifdef SUPPORT_ATP_HYBRID
        case BHAL_IOCTL_LTE_RESET:
        { 
            unsigned short gpio;
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (0 == iRet)
            {
                if (BpGetLteRestGpio(&gpio) == BHAL_OK)
                {
                    GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);
                    GPIO->GPIOio |= GPIO_NUM_TO_MASK(gpio);
                    udelay(100000);
                    GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);
                    GPIO->GPIOio &= ~ GPIO_NUM_TO_MASK(gpio);
                }
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
#endif

		case BHAL_IOCTL_SET_LED:
		{
			 iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
			 //printk("BHAL_IOCTL_SET_LED:%s ,%d\n",stCtrlParms.pcString,stCtrlParms.ulType);
			 if(stCtrlParms.ulType == BHAL_LED_Internet)
			 {
				 if (( 0 == iRet ) && (strlen(stCtrlParms.pcString)<BHAL_WAN_INTERFACE_LEN_MAX))
				 {
					 for(i = 0; i < WAN_MAX_NUM; i++)
					 {
						 if(0 == strlen(g_stWanDevList[i].acDevName))
						 {
							 snprintf(g_stWanDevList[i].acDevName, BHAL_WAN_INTERFACE_LEN_MAX, "%s", stCtrlParms.pcString);
						 	 //printk("\ng_stWanDevList[i].acDevName: %s set ok\n",
							 //	g_stWanDevList[i].acDevName);
                             if(stCtrlParms.ulStrLen == 1)
                             {
                                //printk("\ng_stWanDevList[i].acDevName: %s No LedDetect\n",
							 	//g_stWanDevList[i].acDevName);
                                g_stWanDevList[i].ledDeactive = 1;
                             }
                             
							 break;
						 }						  
					 }			   
					 iRet = BHAL_OK;
				 }
				 else
				 {
					 iRet = BHAL_ERROR;
				 }
			 }

/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 start*/
			 if(stCtrlParms.ulType == BHAL_LED_Hspa)
			 {
			 	if(stCtrlParms.pcString == NULL)
				{
					g_bUmtsState = TRUE;//3g处理
			 	}
				else
				{
					g_boldUmtsState = TRUE;//3g处理
				}
			 }
/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 end*/

#ifdef SUPPORT_ATP_WLAN
			 if(stCtrlParms.ulType == BHAL_LED_Wlan)
			 {
				 if (( 0 == iRet ) && (strlen(stCtrlParms.pcString)<BHAL_WAN_INTERFACE_LEN_MAX))
				 {
				 	 char findConnect = 0;
				 	 for (i = 0; i < WLAN_2G_SSID_NUM; i++)
					 {
						 if (0 == strncmp(g_stWlanDevList[i].acDevName, stCtrlParms.pcString, BHAL_WAN_INTERFACE_LEN_MAX))
						 {
						 	 //printk("\ng_stWlanDevList[i].acDevName: %s add ok\n",
							 //	g_stWlanDevList[i].acDevName);
							 //memset(&g_stWlanDevList[i], 0, sizeof(WLAN_DEV_INFO));
							 g_stWlanDevList[i].connectNum++;
							 findConnect = 1;
							 break;
						 }
					 }

					 if(0 == findConnect)
					 {
						 for(i = 0; i < WLAN_2G_SSID_NUM; i++)
						 {
							 if(0 == strlen(g_stWlanDevList[i].acDevName))
							 {
								 snprintf(g_stWlanDevList[i].acDevName, BHAL_WAN_INTERFACE_LEN_MAX, "%s", stCtrlParms.pcString);
							 	 //printk("\ng_stWlanDevList[i].acDevName: %s set ok\n",
								 //	g_stWlanDevList[i].acDevName);
								 g_stWlanDevList[i].connectNum = 1;
								 break;
							 }						  
						 }
					 }
					 iRet = BHAL_OK;
				 }
				 else
				 {
					 iRet = BHAL_ERROR;
				 }
			 }
#endif
			 break;  
		}
		
		case BHAL_IOCTL_DEL_LED:
		{
			 iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
			 //printk("BHAL_IOCTL_DEL_LED:%s ,%d\n",stCtrlParms.pcString,stCtrlParms.ulType);
			 if(stCtrlParms.ulType == BHAL_LED_Internet)
			 {
				 if (( 0 == iRet ) && (strlen(stCtrlParms.pcString) < BHAL_WAN_INTERFACE_LEN_MAX))
				 {							 
					 for (i = 0; i < WAN_MAX_NUM; i++)
					 {
						 if (0 == strncmp(g_stWanDevList[i].acDevName, stCtrlParms.pcString, BHAL_WAN_INTERFACE_LEN_MAX))
						 {
						 	 //printk("\ng_stWanDevList[i].acDevName: %s delete ok\n",
							 //	g_stWanDevList[i].acDevName);
							 memset(&g_stWanDevList[i], 0, sizeof(WLAN_DEV_INFO));
							 //break;
						 }
					 }
					 iRet = BHAL_OK;
				 }
				 else
				 {
					 iRet = BHAL_ERROR;
				 }
			 }

/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭start*/			 
			 if(stCtrlParms.ulType == BHAL_LED_Hspa)
			 {
			 	if(stCtrlParms.pcString == NULL)
				{
					g_bUmtsState = FALSE;//3g处理
			 	}
				else
				{
					g_boldUmtsState = FALSE;//3g处理
				}
			 }
/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭end*/
			 
#ifdef SUPPORT_ATP_WLAN
			 if(stCtrlParms.ulType == BHAL_LED_Wlan)
			 {
				 if (( 0 == iRet ) && (strlen(stCtrlParms.pcString) < BHAL_WAN_INTERFACE_LEN_MAX))
				 {							 
					 for (i = 0; i < WLAN_2G_SSID_NUM; i++)
					 {
						 if (0 == strncmp(g_stWlanDevList[i].acDevName, stCtrlParms.pcString, BHAL_WAN_INTERFACE_LEN_MAX))
						 {
						 	 //printk("\ng_stWlanDevList[i].acDevName: %s delete ok\n",
							 //	g_stWlanDevList[i].acDevName);
							 g_stWlanDevList[i].connectNum = g_stWlanDevList[i].connectNum - 1;
							 if(0 == g_stWlanDevList[i].connectNum)
							 {
							 	memset(&g_stWlanDevList[i], 0, sizeof(WLAN_DEV_INFO));
							 }
							 break;
						 }
					 }
					 iRet = BHAL_OK;
				 }
				 else
				 {
					 iRet = BHAL_ERROR;
				 }
			 }
#endif
			 break;  
		}
		
		case BHAL_IOCTL_DEL_ALL_LED:
		{
			 iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
			 //printk("\nBHAL_IOCTL_DEL_ALL_LED,%d\n",stCtrlParms.ulType);
			 if(stCtrlParms.ulType == BHAL_LED_Internet)
			 {
				 for (i = 0; i < WAN_MAX_NUM; i++)
				 {
					 memset(&g_stWanDevList[i], 0, sizeof(WLAN_DEV_INFO));
				 }
				 iRet = BHAL_OK;
			 }
#ifdef SUPPORT_ATP_WLAN
			 if(stCtrlParms.ulType == BHAL_LED_Wlan)
			 {		 
				 for (i = 0; i < WLAN_2G_SSID_NUM; i++)
				 {
					 memset(&g_stWlanDevList[i], 0, sizeof(WLAN_DEV_INFO));
				 }
				 iRet = BHAL_OK;
			 }
#endif
			 break;  
		}
		
#ifdef BRCM_DAYING_GASP
        case BHAL_IOCTL_CTRL_DYING_GASP:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                g_SysDyingGaspIsr = stCtrlParms.ulOffset;
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            
            break;
        }
#endif        
        /*start: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/       
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
            case BHAL_IOCTL_SET_CONSOLE_TYPE:
            {
                iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
                if (( 0 == iRet ) && (( 0 == stCtrlParms.ulType) || (1 == stCtrlParms.ulType)))
                {
                    g_TagConsole = stCtrlParms.ulType;
                    iRet = BHAL_OK;
                }
                else
                {
                    iRet = BHAL_ERROR;
                }
                printk("tagConsole==[%d]\n",g_TagConsole);
                break;
            }
#endif
        /*end: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
		case BHAL_IOCTL_SET_LED_TOTAL_TIMES:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
            	bhalSendLedTimes(stCtrlParms.ulOffset);
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            
            break;
        }
		case BHAL_IOCTL_GET_LED_REMAIN_TIMES:
        {
			bhalGetLedTimes(&LedRemainBlinkTimes);
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                __copy_to_user(stCtrlParms.pcBuf, (char *)&LedRemainBlinkTimes, sizeof(LedRemainBlinkTimes));
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
		case BHAL_IOCTL_GET_LED_STATE:
        {
            if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
            {
                iRet = GetLedState((BOARD_LED_NAME)stCtrlParms.ulOffset);                
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }
#endif
#ifdef SUPPORT_ATP_VOICE_DT
		case BHAL_IOCTL_SET_URGENTCALL_STATUS:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (( 0 == iRet ) && (( 0 == stCtrlParms.ulOffset) || (1 == stCtrlParms.ulOffset)))
            {
            	iRet = bhalplatSetUrgentCallStatus(stCtrlParms.ulOffset);
            }
            else
            {
                iRet = BHAL_ERROR;
            }
			
            break;
        }
#endif	
        case  BHAL_IOCTL_SET_KERNELMAC:
        {
            macinit = 1;
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            /*START ADD: l81008471 FOR 解决如果没有设置MAC, 导致初始MAC为FF问题 2013-08-02*/
            if ((((stCtrlParms.pcString[0]  & 0x0FF) == 0xFF)
               && ((stCtrlParms.pcString[1] & 0x0FF) == 0xFF)
               && ((stCtrlParms.pcString[2] & 0x0FF) == 0xFF)
               && ((stCtrlParms.pcString[3] & 0x0FF) == 0xFF)
               && ((stCtrlParms.pcString[4] & 0x0FF) == 0xFF)
               && ((stCtrlParms.pcString[5] & 0x0FF) == 0xFF))
               || ((stCtrlParms.pcString[0] == 0x00)
               && (stCtrlParms.pcString[1] == 0x00)
               && (stCtrlParms.pcString[2] == 0x00)
               && (stCtrlParms.pcString[3] == 0x00)
               && (stCtrlParms.pcString[4] == 0x00)
               && (stCtrlParms.pcString[5] == 0x00)))
            {
                printk("\nWarning!!! mac addr has not been set, please set it.\n");
                stCtrlParms.pcString[0] = 0x00;
                stCtrlParms.pcString[1] = 0x10;
                stCtrlParms.pcString[2] = 0x20;
                stCtrlParms.pcString[3] = 0x30;
                stCtrlParms.pcString[4] = 0x40;
                stCtrlParms.pcString[5] = 0x50;
            }
            /*END ADD: l81008471 FOR 解决如果没有设置MAC, 导致初始MAC为FF问题 2013-08-02*/
            memcpy(szBaseMacAddr,stCtrlParms.pcString,BHAL_OBLI_MAC_ADDRESS_LEN);
            printk("szBaseMacAddr:");
            for(i = 0;i<BHAL_OBLI_MAC_ADDRESS_LEN;i++)
            {  
                printk("%02x ",0x0ff&szBaseMacAddr[i]);
            }
            printk("\n");
            //printk("HZ:%x\n",HZ);
            break;
        }
#ifdef SUPPORT_ATP_BOARD_TYPE_DETECT
		case BHAL_IOCTL_GET_PRODUCT_TYPE:
		{
			if (0 == copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms)))
			{
				unsigned char buf[BHAL_BOARD_TYPE_STRING_LEN] = {0};
				if(BHAL_OK == bhalplatGetProductType(buf, sizeof(buf))) {
					copy_to_user(stCtrlParms.pcString, buf, strlen(buf) + 1);
					iRet = BHAL_OK;
				}
				else {
					iRet = BHAL_ERROR;
				}
            }
            else
            {
                iRet = BHAL_ERROR;
            }
		    break;
        }
#endif
#ifdef SUPPORT_ATP_WLAN_DT
		case BHAL_IOCTL_SET_WPS_BUTTON_STATUS:
        {
            iRet = bhalplatSetWpsProcStatus();
            break;
        }
		case BHAL_IOCTL_GET_WPS_BUTTON_STATUS:
        {
            iRet = bhalplatGetWpsProcStatus();
            break;
        }
		case BHAL_IOCTL_CLR_WPS_BUTTON_STATUS:
        {
            iRet = bhalplatClrWpsProcStatus();
            break;
        }
        /* START MODIFY by y00181549 for DTS2013101803442 at 20140108*/
        case BHAL_IOCTL_SET_WLAN_SETTING_STATUS:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (( 0 == iRet ) && (( 0 == stCtrlParms.ulOffset) || (1 == stCtrlParms.ulOffset)))
            {
                iRet = BHAL_OK;
                g_bWlanSettingBusy = stCtrlParms.ulOffset;
            }
            else
            {
                iRet = BHAL_ERROR;
            }

            break;
        }
        /* END MODIFY by y00181549 for DTS2013101803442 at 20140108*/
#endif	
	case BHAL_IOCTL_CMS_INITED:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (( 0 == iRet ) && (( 0 == stCtrlParms.ulOffset) || (1 == stCtrlParms.ulOffset)))
            {
                g_bCmsIsOK = TRUE;
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
			
            break;
        }
		/* START MODIFY by y00181549 for DTS2013101803442 串口小板在cms初始化完成之后再进行初始化，防止消息丢失at 20140108*/
#ifdef SUPPORT_ATP_GPIO_I2C  
/*Init cms*/
	case BHAL_IOCTL_I2C_INIT:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (( 0 == iRet ) && (1 == stCtrlParms.ulOffset))
            {
                if (0 != gpio_i2c_init())
                {
                    printk("init gpio i2c error\r\n");
                }
                iRet = BHAL_OK;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
			
            break;
        }
#endif
		/* END MODIFY by y00181549 for DTS2013101803442 串口小板在cms初始化完成之后再进行初始化，防止消息丢失at 20140108*/
        case BHAL_IOCTL_UPG_FLAG:
        {
            iRet = copy_from_user((void*)&stCtrlParms, (void*)arg, sizeof(stCtrlParms));
            if (0 == iRet)
            {
                g_bIsupgrading = (bool)stCtrlParms.ulType;
            }
            else
            {
                iRet = BHAL_ERROR;
            }
            break;
        }

        default:
        {
            iRet = BHAL_ERROR;
            printk("board_ioctl: invalid command %x, cmd %d .\n",command,_IOC_NR(command));
            break;
        }

    }/* switch */

    stCtrlParms.lResult = iRet;
    __copy_to_user((BHAL_IOCTL_PARMS_ST*)arg, &stCtrlParms, sizeof(BHAL_IOCTL_PARMS_ST));

    return iRet;
}

#if defined (SUPPORT_ATP_WLAN)
#if defined (SUPPORT_ATP_WPS)
static int bhalWpsChange(int WPSMODE)
{
    signed int iRet = -1;
    if (WPSMODE == 0 || WPSMODE == 1)
    {
        return bhalplatWpsChange(WPSMODE);
    }
    else
    {
        return iRet;
    }
}
#endif
#endif

/******************************************************************************
 * 功能描述  :  判断目标CPU的字节序
 * 参数      :  无                                                             *
 * 返 回 值  :  BHAL_BIG_ENDIAN:     大端                                            *
 *              BHAL_LITTLE_ENDIAN:  小端                                            *
 * 说明     :
 *****************************************************************************/
int bhalJudgeIsBigEnd(void)
{
    unsigned short usVal = 0x0001;
    return (*(unsigned char*)(&usVal));
}

unsigned long bhalCpuToEndian32(unsigned long ulVal)
{
    if (BHAL_LITTLE_ENDIAN == bhalJudgeIsBigEnd())
    {
         ulVal =  ((ulVal>>24) | (ulVal<<24) | ((ulVal>>8)&0x0000FF00) | ((ulVal<<8)&0x00FF0000));
    }
    return ulVal;
}


/******************************************************************************
  函数名称  : bhalGetBaseMacAddress
  功能描述  :
  输入参数  :
              1.  *pucaMacAddr:
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalGetBaseMacAddress( unsigned char *pucaMacAddr)
{
    //char* pcBuf = NULL;
    int iReturn = FLASH_API_ERROR;
    //BHAL_VARIABLE_SYS_PARAM_ST* pstSysParam = NULL;
    //BHAL_VARIABLE_EX_PARAM_ST*  pstExParam = NULL;
    //int iLen = 0;
    int i;

    if(0 == macinit)
    {
        if(pucaMacAddr)
        {     
            for(i = 0;i<8;i++)
            {
              pucaMacAddr[i] = i & 0x0FF;
            }
            iReturn = FLASH_API_OK;

            printk("\n$Read mac is %02X%02X%02X%02X%02X%02X \r\n",
            pucaMacAddr[0] & 0x0FF,pucaMacAddr[1] & 0x0FF,pucaMacAddr[2] & 0x0FF,
            pucaMacAddr[3] & 0x0FF,pucaMacAddr[4] & 0x0FF,pucaMacAddr[5] & 0x0FF);
        
        }
        else   
        {
            printk("\n$Read mac error \r\n");
        }
    }
    else
    {
        return FLASH_API_OK;  
    }
    return iReturn;    
}

#ifdef DT_MAC_SUPPORT
/******************************************************************************
  函数名称  : bhalGetMacAddress
  功能描述  :
  输入参数  :
              1.  *pucaMacAddr:
              2.  ulId:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalGetMacAddress( unsigned char *pucaMacAddr, unsigned long ulId)
{
    const unsigned long constMacAddrIncIndex = 3;
    int nRet = 0;
    static int lWanMacNum = 0;
    static int lWlanMacNum = 0;
    int lTmpMacIndx = 0;
    BHAL_MAC_ADDR_INFO_ST *pstMai = NULL;
    BHAL_MAC_ADDR_INFO_ST *pstMaiFreeNoId = NULL;
    BHAL_MAC_ADDR_INFO_ST *pstMaiFreeId = NULL;
    unsigned long i = 0, ulIdxNoId = 0, ulIdxId = 0, baseMacAddr = 0;

    bhalGetBaseMacAddress(szBaseMacAddr);
    //bhalCpuToEndian32(g_stBhalSysInfo.ucaBaseMacAddr);
    memcpy((unsigned char *) &baseMacAddr,
        &szBaseMacAddr[constMacAddrIncIndex],
        BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex);
		
    /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
    baseMacAddr = bhalCpuToEndian32(baseMacAddr);
    baseMacAddr >>= 8;

    if (ulId >= ATP_BHAL_ETH_MAC && ulId < BHAL_MAC_NUMBERS_MAX) 
    {
        pstMai = MacAddrs;
        pstMai = pstMai+lTmpMacIndx;	
        baseMacAddr = (baseMacAddr + lTmpMacIndx) << 8;

        /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
        baseMacAddr = bhalCpuToEndian32(baseMacAddr);

        memcpy( pucaMacAddr, szBaseMacAddr,
            constMacAddrIncIndex);
        memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
            &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
        printk("\r\n$Read mac is %02X%02X%02X%02X%02X%02X\r\n",
            pucaMacAddr[0] & 0x0FF,pucaMacAddr[1] & 0x0FF,pucaMacAddr[2] & 0x0FF,
            pucaMacAddr[3] & 0x0FF,pucaMacAddr[4] & 0x0FF,pucaMacAddr[5] & 0x0FF);
        pstMai->chInUse = 1;
        pstMaiFreeNoId = pstMaiFreeId = NULL;
        return 0;		
    } 

    for( i = 0, pstMai = MacAddrs; i < BHAL_MAC_NUMBERS_MAX; i++, pstMai++ )
    {
        if( ulId == pstMai->ulId || ulId == MAC_ADDRESS_ANY )
        {
            /* This MAC address has been used by the caller in the past. */
            baseMacAddr = (baseMacAddr + i) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy( pucaMacAddr, szBaseMacAddr,
                constMacAddrIncIndex);
            memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
                &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMai->chInUse = 1;
            pstMaiFreeNoId = pstMaiFreeId = NULL;
            break;
        }
        else  if( pstMai->chInUse == 0 )
        {
            if( pstMai->ulId == 0 && pstMaiFreeNoId == NULL )
            {
                 /* 有以前从来没用过的mac地址 */
                 pstMaiFreeNoId = pstMai;
                 ulIdxNoId = i;
            }
            else  if( pstMai->ulId != 0 && pstMaiFreeId == NULL)
            {
                int j = 0;
                BHAL_MAC_ADDR_INFO_ST *pstLocalMai = pstMai+1;
                /*判断是否还有从来没有被分配的mac地址 */
                for (j = i+1; j < BHAL_MAC_NUMBERS_MAX; j++)
                {
                    if((pstLocalMai->chInUse == 0) && (pstLocalMai->ulId == 0))
                    {
                        continue;
                    }
                }

                 pstMaiFreeId = pstMai;
                 ulIdxId = i;
            }
        }
    }

    if( pstMaiFreeNoId || pstMaiFreeId )
    {
        /* An available MAC address was found. */
        memcpy(pucaMacAddr, szBaseMacAddr, BHAL_MAC_ADDRESS_LEN);
        if( pstMaiFreeNoId )
        {
            baseMacAddr = (baseMacAddr + ulIdxNoId) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy(pucaMacAddr, szBaseMacAddr, constMacAddrIncIndex);
            memcpy(pucaMacAddr + constMacAddrIncIndex, (unsigned char*)&baseMacAddr,
                BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMaiFreeNoId->ulId = ulId;
            pstMaiFreeNoId->chInUse = 1;
        }
        else
        {
            baseMacAddr = (baseMacAddr + ulIdxId) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy( pucaMacAddr, szBaseMacAddr,constMacAddrIncIndex);
            memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
                &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMaiFreeId->ulId = ulId;
            pstMaiFreeId->chInUse = 1;
        }
    }
    else
    {
        if( i == BHAL_MAC_NUMBERS_MAX )
        {
            nRet = -EADDRNOTAVAIL;
        }
    }
    
    return( nRet );
}
#else

/******************************************************************************
  函数名称  : bhalGetMacAddress
  功能描述  :
  输入参数  :
              1.  *pucaMacAddr:
              2.  ulId:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalGetMacAddress( unsigned char *pucaMacAddr, unsigned long ulId )
{
    const unsigned long constMacAddrIncIndex = 3;
    int nRet = 0;
    static int lWanMacNum = 0;
    static int lWlanMacNum = 0;
    int lTmpMacIndx = 0;
    BHAL_MAC_ADDR_INFO_ST *pstMai = NULL;
    BHAL_MAC_ADDR_INFO_ST *pstMaiFreeNoId = NULL;
    BHAL_MAC_ADDR_INFO_ST *pstMaiFreeId = NULL;
    unsigned long i = 0, ulIdxNoId = 0, ulIdxId = 0, baseMacAddr = 0;
#ifdef SUPPORT_ATP_HG533
    unsigned long wlanMacInd = BHAL_MAC_NUMBERS_MAX - 1, wlanMac, lastMac;
#endif

#if defined(CONFIG_MIPS_BRCM)
    if (2003566592 == ulId)
    {
        return nRet;
    }
#endif

    bhalGetBaseMacAddress(szBaseMacAddr);
//    bhalCpuToEndian32(g_stBhalSysInfo.ucaBaseMacAddr);
    memcpy((unsigned char *) &baseMacAddr,
            &szBaseMacAddr[constMacAddrIncIndex],
            BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex);

    /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
    baseMacAddr = bhalCpuToEndian32(baseMacAddr);

    baseMacAddr >>= 8;
    
/***************************************************
通用规格:
以太网口分配第1个MAC(基MAC)
WAN接口分配第2-9个MAC，按顺序分配
WLAN接口第10-13个MAC，按顺序分配

****************************************************/
    if (ulId >= ATP_BHAL_ETH_MAC && ulId < ATP_BHAL_END_MAC) 
    { 		
        if(ATP_BHAL_WAN_MAC == ulId)
        {
            if(0 == lWanMacNum)
            {
                lWanMacNum = BHAL_WAN_MAC_MIN_INDX;
                lTmpMacIndx = lWanMacNum;
                lWanMacNum ++;                
            }            
            if(lWanMacNum > BHAL_WAN_MAC_MAX_INDX)
            {
                return -1;
            }
            else
            {
                lTmpMacIndx = lWanMacNum;
                lWanMacNum ++;
            }
        }

        if(ATP_BHAL_WLAN_MAC == ulId)
        {
            if(0 == lWlanMacNum)
            {
                lWlanMacNum = BHAL_WLAN_MAC_MIN_INDX;
                lTmpMacIndx = lWlanMacNum;
                lWlanMacNum ++;                
            }
            else if(lWlanMacNum > BHAL_WLAN_MAC_MAX_INDX)
            {
                return -1;
            }
            else
            {
                lTmpMacIndx = lWlanMacNum;
                lWlanMacNum ++;
            }            
        }

        
        pstMai = MacAddrs;
        pstMai = pstMai+lTmpMacIndx;	
        baseMacAddr = (baseMacAddr + lTmpMacIndx) << 8;

        /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
        baseMacAddr = bhalCpuToEndian32(baseMacAddr);

        memcpy( pucaMacAddr, szBaseMacAddr,
            constMacAddrIncIndex);
        memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
            &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
      printk("\r\n$Read mac is %02X%02X%02X%02X%02X%02X \r\n",
      pucaMacAddr[0] & 0x0FF,pucaMacAddr[1] & 0x0FF,pucaMacAddr[2] & 0x0FF,
      pucaMacAddr[3] & 0x0FF,pucaMacAddr[4] & 0x0FF,pucaMacAddr[5] & 0x0FF);
        pstMai->chInUse = 1;
        pstMaiFreeNoId = pstMaiFreeId = NULL;
        return 0;		
    } 

    for( i = 0, pstMai = MacAddrs; i < BHAL_MAC_NUMBERS_MAX; i++, pstMai++ )
    {
        if( ulId == pstMai->ulId || ulId == MAC_ADDRESS_ANY )
        {
            /* This MAC address has been used by the caller in the past. */
            baseMacAddr = (baseMacAddr + i) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy( pucaMacAddr, szBaseMacAddr,
                constMacAddrIncIndex);
            memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
                &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMai->chInUse = 1;
            pstMaiFreeNoId = pstMaiFreeId = NULL;
            break;
        }
        else  if( pstMai->chInUse == 0 )
        {
            if( pstMai->ulId == 0 && pstMaiFreeNoId == NULL )
            {
                 /* 有以前从来没用过的mac地址 */
                 pstMaiFreeNoId = pstMai;
                 ulIdxNoId = i;
            }
            else  if( pstMai->ulId != 0 && pstMaiFreeId == NULL)
            {
                int j = 0;
                BHAL_MAC_ADDR_INFO_ST *pstLocalMai = pstMai+1;
                /*判断是否还有从来没有被分配的mac地址 */
                for (j = i+1; j < BHAL_MAC_NUMBERS_MAX; j++)
                {
                    if((pstLocalMai->chInUse == 0) && (pstLocalMai->ulId == 0))
                    {
                        continue;
                    }
                }

                 pstMaiFreeId = pstMai;
                 ulIdxId = i;
            }
        }
    }

    if( pstMaiFreeNoId || pstMaiFreeId )
    {
        /* An available MAC address was found. */
        memcpy(pucaMacAddr, szBaseMacAddr, BHAL_MAC_ADDRESS_LEN);
        if( pstMaiFreeNoId )
        {
            baseMacAddr = (baseMacAddr + ulIdxNoId) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy(pucaMacAddr, szBaseMacAddr, constMacAddrIncIndex);
            memcpy(pucaMacAddr + constMacAddrIncIndex, (unsigned char*)&baseMacAddr,
                BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMaiFreeNoId->ulId = ulId;
            pstMaiFreeNoId->chInUse = 1;
        }
        else
        {
            baseMacAddr = (baseMacAddr + ulIdxId) << 8;

            /* 自动处理由于CPU字节序不同导致分配mac地址不正确的问题 */
            baseMacAddr = bhalCpuToEndian32(baseMacAddr);

            memcpy( pucaMacAddr, szBaseMacAddr,constMacAddrIncIndex);
            memcpy( pucaMacAddr + constMacAddrIncIndex, (unsigned char *)
                &baseMacAddr, BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex );
            pstMaiFreeId->ulId = ulId;
            pstMaiFreeId->chInUse = 1;
        }
    }
    else
    {
        if( i == BHAL_MAC_NUMBERS_MAX )
        {
            nRet = -EADDRNOTAVAIL;
        }
    }

    return( nRet );
}
#endif

/******************************************************************************
  函数名称  : bhalReleaseMacAddress
  功能描述  :
  输入参数  :
              1.  *pucaMacAddr:
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalReleaseMacAddress( unsigned char *pucaMacAddr )
{
    const unsigned long constMacAddrIncIndex = 3;
    int nRet = -EINVAL;
    unsigned long ulIdx = 0;
    unsigned long baseMacAddr = 0;
    unsigned long relMacAddr = 0;

    /* baseMacAddr = last 3 bytes of the base MAC address treated as a 24 bit integer */
    memcpy((unsigned char *) &baseMacAddr,
            &szBaseMacAddr[constMacAddrIncIndex],
            BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex);

    baseMacAddr = bhalCpuToEndian32(baseMacAddr);
    baseMacAddr >>= 8;

    /* Get last 3 bytes of MAC address to release. */
    memcpy((unsigned char *) &relMacAddr, &pucaMacAddr[constMacAddrIncIndex],
            BHAL_MAC_ADDRESS_LEN - constMacAddrIncIndex);

    relMacAddr = bhalCpuToEndian32(relMacAddr);
    relMacAddr >>= 8;

    ulIdx = relMacAddr - baseMacAddr;

    if( ulIdx < BHAL_MAC_NUMBERS_MAX )
    {
        BHAL_MAC_ADDR_INFO_ST *pstMai = &MacAddrs[ulIdx];
        if( pstMai->chInUse == 1 )
        {
            pstMai->chInUse = 0;		
            pstMai->ulId = 0;
            nRet = 0;
        }
    }
    return( nRet );
}

#if 0
/* start of w00104696 ADD for A36D08358: ppp send PADT 20091016 */
#ifdef SUPPORT_ATP_PPP_AUTODISCONN
/******************************************************************************
  函数名称  : bhalGetPppSession
  功能描述  : 根据偏移地址从flash中获取信息
  输入参数  : 
  调用函数  : 
  被调函数  : bhalIoctl
  输出参数  : 
            *pcSession:对应 PPP Session 信息
  返 回 值  : 无

  修改历史      :
     日    期   : 2009-4-27
     作    者   : w00104696
     修改内容   : 完成初稿

******************************************************************************/
int bhalGetPppSession(void *pcSession)
{
    BHAL_PPP_SESSION_ST stPppInfo[WAN_MAX_NUM];
    int i = 0;
    
    memset(stPppInfo, 0, sizeof(stPppInfo));
    
    ATP_FLASH_ReadFlashArea(BHAL_FLASH_AREA_PPP_SESSION, (char*)&stPppInfo, sizeof(stPppInfo), BHAL_PPP_SESSION_OFFSET);

    for (i = 0; i < WAN_MAX_NUM; i++)
    {
        if (((stPppInfo[i].acInterface[0] < '!' || stPppInfo[i].acInterface[0] > '~') && (stPppInfo[i].acInterface[0] != '\0'))
            || ((stPppInfo[i].acPppSessid[0] < '!' || stPppInfo[i].acPppSessid[0] > '~') && (stPppInfo[i].acPppSessid[0] != '\0')))
        {
            memset(&stPppInfo[i], 0, sizeof(BHAL_PPP_SESSION_ST));
        }
        //printk("==>[bhal.c], [%d], i=%d, Interface=%s,SessionId=%s\n", __LINE__, i,
        //         stPppInfo[i].acInterface, stPppInfo[i].acPppSessid);
    }
  
    memcpy(pcSession, &stPppInfo, sizeof(stPppInfo));
    return 0;
}

/******************************************************************************
  函数名称  : bhalGetPppSession
  功能描述  : 根据偏移地址将信息写入flash中
  输入参数  : 
              *pcSession:需要写flash的参数信息
              ulLen:对应参数长度值
  调用函数  : 
  被调函数  : bhalIoctl
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
     日    期   : 2009-4-27
     作    者   : w00104696
     修改内容   : 完成初稿

******************************************************************************/
int bhalSavePppSession(unsigned char *pcSession, unsigned int uLen)
{
    ATP_FLASH_WriteFlashArea(BHAL_FLASH_AREA_PPP_SESSION, pcSession, uLen, BHAL_PPP_SESSION_OFFSET); 
    return 0;
}
#endif
/* end of w00104696 ADD for A36D08358: ppp send PADT 20091016 */
#endif
/******************************************************************************
  函数名称  : bhalGetChipParam
  功能描述  :
  输入参数  :
              1.  enChipParamType:
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-3
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static int bhalGetChipParam(ATP_BHAL_CHIP_PARAM_EN enChipParamType,unsigned long ulString,unsigned long ulSize)
{
    return (bhalplatGetChipParam(enChipParamType,ulString,ulSize));
}

/******************************************************************************
  函数名称  : BhalSoftReboot
  功能描述  : 系统软复位
  输入参数  :
              1. void:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-6-30
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
void bhalSoftReboot(void)
{
    bhalplatSoftReboot();
    return;
}

#ifdef SUPPORT_ATP_TCCONSOLE
/******************************************************************************
  函数名称  : bhalSoftReset
  功能描述  : 系统软复位
  输入参数  : 
              1. void:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   :
     作    者   :

******************************************************************************/
void bhalSoftReset(void)
{
    bhalplatSoftReset();
}
#endif

/******************************************************************************
  函数名称  : BhalHardReboot
  功能描述  : 系统硬复位(需要硬件的支持)
  输入参数  :
              1. void:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-6-30
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
void bhalHardReboot(void)
{
    bhalplatHardReboot();
    return;
}

/******************************************************************************
  函数名称  : BhalGetBootloaderVersion
  功能描述  :
  输入参数  :
              1. void:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static void bhalGetBootloaderVersion(BHAL_IOCTL_PARMS_ST *stCtrlParms)
{

    if((stCtrlParms->ulStrLen) < BHAL_BOOTLOADVER_SIZE)
    {
        return;
    }
    memcpy(stCtrlParms->pcString,BHAL_BOOTLOAD_VERSION,BHAL_BOOTLOADVER_SIZE);
    return;
}

/******************************************************************************
  函数名称  : bhalGetSdramSize
  功能描述  :
  输入参数  :
              1. void:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-2
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
int bhalGetSdramSize(void)
{
    return BHAL_SDMEMSIZE;/*32M*/
}


/******************************************************************************
  函数名称  : bhalCtrlLed
  功能描述  :
  输入参数  :
              1.  ledName:
              2.  ledState:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-4
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
#if 0
static void bhalCtrlLed(BSP_E_LED_NAME enLedName, BSP_E_LED_STATE enLedState)
{
	ATP_LED_CtrlLed(enLedName, enLedState);
    return;
}
#endif

static void bhalCtrlLed(BSP_E_LED_NAME enLedName, BSP_E_LED_STATE enLedState)
{
    if (BHAL_LED_MODE_NORMAL == g_ulBhalLedMode)
    {
	    ATP_LED_ChangeStatus(enLedName, (unsigned int)enLedState);
    }
    return;
}

/******************************************************************************
  函数名称  : bhalSetGpio
  功能描述  :
  输入参数  :
              1.  gpio: GPIO号
              2.  state:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-7-4
     作    者   : l69021
     修改内容   : 完成初稿

******************************************************************************/
static void bhalSetGpio(int gpio, BSP_E_GPIO_STATE enState)
{
  bhalplatsetGpio(gpio,enState);
  return;

}

#ifdef SUPPORT_ATP_WATCHDOG
int BhalHardWatchDogInit(VOS_VOID)
{
    unsigned long flags;
    /* Connect and enable interrupt */
	
    if (BcmHalMapInterrupt((FN_HANDLER)ext_timer_isr, INTERRUPT_ID_TIMER, INTERRUPT_ID_TIMER))
    { 
        return -1;
    }
    
    BcmHalInterruptEnable(INTERRUPT_ID_TIMER);

    spin_lock_irqsave(&bcmdog_lock, flags); 
    TIMER->WatchDogCtl = 0xEE00;
    TIMER->WatchDogCtl = 0x00EE;
    TIMER->WatchDogDefCount = SUPPORT_ATP_WATCHDOG_TIMER * FPERIPH;
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;
    spin_unlock_irqrestore(&bcmdog_lock, flags); 
	
    return 0;
}

static irqreturn_t ext_timer_isr(int irq, unsigned int param)
{
    unsigned long flags;
    uint32 int_status;
	
    spin_lock_irqsave(&bcmdog_lock, flags); 

    /* retrieve timer int */
    int_status = TIMER->TimerInts;
    
    BcmHalInterruptEnable(INTERRUPT_ID_TIMER);

    /* check watchdog timer interrupt */
    if ((!g_RebootStopIsr) && (int_status & WATCHDOG))
    {
        TIMER->WatchDogCtl = 0xEE00;
        TIMER->WatchDogCtl = 0x00EE;
        TIMER->WatchDogCtl = 0xFF00;
        TIMER->WatchDogCtl = 0x00FF;
    }

    spin_unlock_irqrestore(&bcmdog_lock, flags); 
    return IRQ_HANDLED;
}

irqreturn_t bcm_watchdog_interrupt(int irq, void *dev_id)
{
    uint32   count;
	
    if ((TIMER->TimerInts & WATCHDOG) == WATCHDOG)
    {        
		if (bcmdog_interrupt_num++ == 10)
        {
#define EPHY_PWR_DOWN_DLL       (1<<25)
            GPIO->RoboswEphyCtrl &= ~EPHY_PWR_DOWN_DLL;
            printk("It is a watchdog reboot\n");  					
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

void bhalplatWatchdogKeepAlive(void)
{
    unsigned long flags;
    /* bcmdog_setWatchDogDefCount */
	spin_lock_irqsave(&bcmdog_lock, flags);
	
    bcmdog_interrupt_num = 0;
        
    /* bcmdog_stop */
    TIMER->WatchDogCtl = 0xEE00;
    TIMER->WatchDogCtl = 0x00EE;
    TIMER->WatchDogDefCount = SUPPORT_ATP_WATCHDOG_TIMER * FPERIPH;
    /* bcmdog_start */
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;
    
    spin_unlock_irqrestore(&bcmdog_lock, flags);
    return;
}

/******************************************************************************
  函数名称  : bhalWatchdogKeepAlive
  功能描述  :
  输入参数  :
              1.  ulData:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2010-12-18
     作    者   : z00175982
     修改内容   : 完成初稿

******************************************************************************/
static void bhalWatchdogKeepAlive(unsigned long ulData)
{
    /*we do not use uldata*/
    bhalplatWatchdogKeepAlive();
    g_stBhalWatchdogTimer.expires = jiffies + 13*BHAL_POLLCNT_1SEC;     
    add_timer(&g_stBhalWatchdogTimer);
    return;
}
#endif

/******************************************************************************
  函数名称  : bhalRetriedKmalloc
  功能描述  : 申请内存
  输入参数  : ulSize: 需要申请内存的字节长度
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-9-11
     作    者   : 俞佑清42304
     修改内容   : 完成初稿

******************************************************************************/
static void *bhalRetriedKmalloc(unsigned long ulSize)
{
    void *pBuf;
    pBuf = vmalloc(ulSize);

    if(pBuf)
        memset(pBuf, 0, ulSize);

    return pBuf;
}

/******************************************************************************
  函数名称  : bhalRetriedKfree
  功能描述  : 释放内存
  输入参数  : ulSize: 需要申请内存的字节长度
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-9-11
     作    者   : 俞佑清42304
     修改内容   : 完成初稿

******************************************************************************/
static void bhalRetriedKfree(void *pBuf)
{
    if(pBuf)
    {
        vfree(pBuf);
    }
    return;
}

#ifdef LED_SEQUENCE_BLINK
void bhalSequenceBlinkLed(unsigned int iTime)
{
    /* Start up control thread */
    int ret = kernel_thread(boardSequenceBlinkLed, iTime, CLONE_KERNEL);

    if (ret < 0) 
    {
        printk("Unable to start control thread\n");	
    }
}

void bhalPwrOnSequenceBlinkLed(unsigned int iTime)
{
	boardPwrOnSequenceBlinkLed(iTime);
}
#endif

#ifdef BRCM_DAYING_GASP
static irqreturn_t bhalDyingGaspIsr(int irq, void * dev_id)
{
    struct list_head *pos;
    CB_DGASP_LIST *tmp = NULL, *dslOrGpon = NULL;

    printk("bhalDyingGaspIsr: Enter dyinggasp ISR.\r\n");
    
    //y42304 2010017: 解决部分单板上电时会由于有一次dyinggasp中断导致系统异常重启
    if (0 == g_SysDyingGaspIsr)
    {        
        printk("bhalDyingGaspIsr: DyingGasp not enable!\r\n");    
        return IRQ_HANDLED;
    }

    UART->Data = 'D';
    UART->Data = '%';
    UART->Data = 'G';
	
#ifdef GPIO_CTRL_POWER
	bhalplatSetUsbPowerDown();
#endif

    /* first to turn off everything other than dsl or gpon */
    list_for_each (pos, &g_cb_dgasp_list_head->list) 
    {
        tmp = list_entry(pos, CB_DGASP_LIST, list);
        if(strncmp(tmp->name, "dsl", 3) && strncmp(tmp->name, "gpon", 4)) 
        {
            (tmp->cb_dgasp_fn)(tmp->context);
        }
        else 
        {
            dslOrGpon = tmp;
        }
    }

    /* now send dgasp */
    if (dslOrGpon)
    {
         printk("Send dyinggasp msg.\r\n");
        (dslOrGpon->cb_dgasp_fn)(dslOrGpon->context);
    }

    /* reset and shutdown system */

    /* Set WD to fire in 1 sec in case power is restored before reset occurs */
    TIMER->WatchDogDefCount = 1000000 * (FPERIPH/1000000);
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;

#if defined(CONFIG_BCM96358)
    PERF->blkEnables &= ~(EMAC_CLK_EN | USBS_CLK_EN | SAR_CLK_EN);
#endif

    // If power is going down, nothing should continue!
    while (1);
	
    return(IRQ_HANDLED);
}

static void __init bhalInitDyingGaspHandler( void )
{
    CB_DGASP_LIST *new_node;

    if( g_cb_dgasp_list_head != NULL) {
        printk("bhalInitDyingGaspHandler: list head is not null!\n");
        return;
    }
    new_node= (CB_DGASP_LIST *)kmalloc(sizeof(CB_DGASP_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_DGASP_LIST));
    INIT_LIST_HEAD(&new_node->list);
    g_cb_dgasp_list_head = new_node;

    BcmHalMapInterrupt((FN_HANDLER)bhalDyingGaspIsr, 0, INTERRUPT_ID_DG);
    BcmHalInterruptEnable( INTERRUPT_ID_DG );
} /* bhalInitDyingGaspHandler */

static void __exit bhalDeinitDyingGaspHandler( void )
{
    struct list_head *pos;
    CB_DGASP_LIST *tmp;

    if(g_cb_dgasp_list_head == NULL)
        return;

    list_for_each(pos, &g_cb_dgasp_list_head->list) {
        tmp = list_entry(pos, CB_DGASP_LIST, list);
        list_del(pos);
        kfree(tmp);
    }

    kfree(g_cb_dgasp_list_head);
    g_cb_dgasp_list_head = NULL;

} /* bhalDeinitDyingGaspHandler */

void kerSysRegisterDyingGaspHandler(char *devname, void *cbfn, void *context)
{
    CB_DGASP_LIST *new_node;

    if( g_cb_dgasp_list_head == NULL) 
    {
        printk("kerSysRegisterDyingGaspHandler: list head is null\n");
        return;
    }

    if( devname == NULL || cbfn == NULL ) 
    {
        printk("kerSysRegisterDyingGaspHandler: register info not enough (%s,%x,%x)\n", devname, (unsigned int)cbfn, (unsigned int)context);
        return;
    }

    /*start of y42304 2010-2-5: wlan和语音不注册dyinggasp*/
    if ((0 == strcmp(devname,"wl0")) 
        || (0 == strcmp(devname,"wl1")) 
        || (0 == strcmp(devname,"eth0"))
        || (0 == strcmp(devname,"endpoint"))) 
    {
        //return;
    }
    /*end of y42304 2010-2-5: wlan和语音不注册dyinggasp*/

    new_node= (CB_DGASP_LIST *)kmalloc(sizeof(CB_DGASP_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_DGASP_LIST));
    INIT_LIST_HEAD(&new_node->list);
    snprintf(new_node->name, IFNAMSIZ, "%s", devname);
    new_node->cb_dgasp_fn = (cb_dgasp_t)cbfn;
    new_node->context = context;
    list_add(&new_node->list, &g_cb_dgasp_list_head->list);

    printk("kerSysRegisterDyingGaspHandler: %s registered\r\n", devname);
} /* kerSysRegisterDyingGaspHandler */

void kerSysDeregisterDyingGaspHandler(char *devname)
{
    struct list_head *pos;
    CB_DGASP_LIST *tmp;

    if(g_cb_dgasp_list_head == NULL)
    {
        printk("kerSysDeregisterDyingGaspHandler: list head is null!\n");
        return;
    }

    if(devname == NULL)
    {
        printk("kerSysDeregisterDyingGaspHandler: devname is null!\n");
        return;
    }

    printk("kerSysDeregisterDyingGaspHandler: %s is deregistering\n", devname);

    list_for_each(pos, &g_cb_dgasp_list_head->list)
    {
        tmp = list_entry(pos, CB_DGASP_LIST, list);
        if (!strcmp(tmp->name, devname))
        {
            list_del(pos);
            kfree(tmp);
            printk("kerSysDeregisterDyingGaspHandler: %s is deregistered\n", devname);
            return;
        }
    }
    
    printk("kerSysDeregisterDyingGaspHandler: %s not (de)registered\n", devname);
} /* kerSysDeregisterDyingGaspHandler */
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
static void bhalSendLedTimes(unsigned int tmpLedTimes)
{
	SendLedBlinkTimes(tmpLedTimes);
}

static void bhalGetLedTimes(unsigned int *pLedRemainBlinkTimes)
{
	GetLedBlinkTimes(pLedRemainBlinkTimes);
}
#endif

#ifdef MTK_DYINGGASP
extern int bhalDyingGaspIsrForMTK(int irq, void * dev_id);

void __init bhalInitDyingGaspHandlerForMTK( void )
{
    int result = 1;
    //DYINGGASP_INT 18
    //		GPIO_INT,		//10	IPL11		
    printk("dyinggasp init\n");
    if(VPint(0xbfb00084) & (1<<16))
    {
        VPint(0xbfb00084) |=(1<<16);
    }
    result = request_irq(19,bhalDyingGaspIsrForMTK,SA_INTERRUPT,"MTK_Dyinggasp" ,NULL);
    if(result)
    {
        printk("request_irq failed\n");
    }
    
} /* bhalInitDyingGaspHandler */
#endif

#if 0

/******************************************************************************
  函数名称  : bhalReadFlash
  功能描述  : 释放内存
  输入参数  : pszDataBuffer: 存放读取数据的buffer
              ulFlashAddr: FLASH绝对地址
              ulLen:       读取的字节长度
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-9-11
     作    者   : 俞佑清42304
     修改内容   : 完成初稿

******************************************************************************/
int bhalReadFlash(void *pszDataBuffer, unsigned long ulFlashAddr, unsigned long ulLen)
{
    if ( NULL != pszDataBuffer)
    {
        return ATP_FLASH_ReadFlash(ulFlashAddr-BHAL_FLASH_BASE, pszDataBuffer, ulLen);
    }
    return FLASH_API_ERROR;
}

/******************************************************************************
  函数名称  : bhalWriteFlash
  功能描述  :
  输入参数  :
              1. pszDataBuffer:
              2. ulFlashAddr:
              3. ulLen:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-30
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/

int bhalWriteFlash(void *pszDataBuffer, unsigned long ulFlashAddr, unsigned long ulLen)
{
    if ( NULL != pszDataBuffer)
    {
       return ATP_FLASH_WriteFlash(ulFlashAddr-BHAL_FLASH_BASE, pszDataBuffer, ulLen);
    }
    return FLASH_API_ERROR;
}

/******************************************************************************
  函数名称  : bhalEraseFlash
  功能描述  :
  输入参数  :
              1. pszDataBuffer:
              2. ulFlashAddr:
              3. ulLen:
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-12-30
     作    者   : l63336
     修改内容   : 完成初稿

******************************************************************************/

int bhalEraseFlash(unsigned long ulFlashAddr, unsigned long ulLen)
{
   return ATP_FLASH_EraseFlash(ulFlashAddr-BHAL_FLASH_BASE, ulLen);
}

int bhalGetFlashSize(void)
{
    return ATP_FLASH_GetSize();
}

EXPORT_SYMBOL(bhalReleaseMacAddress);
EXPORT_SYMBOL(bhalReadFlash);
EXPORT_SYMBOL(bhalEraseFlash);
EXPORT_SYMBOL(bhalGetFlashSize);
EXPORT_SYMBOL(bhalWriteFlash);

#endif

/***************************************************************************
 * MACRO to call driver initialization and cleanup functions.
 ***************************************************************************/
core_initcall( bhalInit ); //让bhal模块先调用
module_exit( bhalRelease );

EXPORT_SYMBOL(bhalGetBaseMacAddress);
EXPORT_SYMBOL(bhalGetMacAddress);


EXPORT_SYMBOL(g_ulDSLActiveTime);
EXPORT_SYMBOL(g_ulDSLUpTime);

EXPORT_SYMBOL(bhalSoftReboot);

#ifdef SUPPORT_ATP_WATCHDOG
EXPORT_SYMBOL(g_stBhalWatchdogTimer);
#endif

volatile int isVoiceIdle = 1;
EXPORT_SYMBOL(isVoiceIdle);

#ifdef BRCM_DAYING_GASP
EXPORT_SYMBOL(kerSysRegisterDyingGaspHandler);
EXPORT_SYMBOL(kerSysDeregisterDyingGaspHandler);
#endif

