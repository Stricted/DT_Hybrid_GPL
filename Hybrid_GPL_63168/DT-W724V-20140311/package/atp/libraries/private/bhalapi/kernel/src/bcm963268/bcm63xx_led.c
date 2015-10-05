/*
<:copyright-gpl 
 Copyright 2002 Broadcom Corp. All Rights Reserved. 
 
 This program is free software; you can distribute it and/or modify it 
 under the terms of the GNU General Public License (Version 2) as 
 published by the Free Software Foundation. 
 
 This program is distributed in the hope it will be useful, but WITHOUT 
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 for more details. 
 
 You should have received a copy of the GNU General Public License along 
 with this program; if not, write to the Free Software Foundation, Inc., 
 59 Temple Place - Suite 330, Boston MA 02111-1307, USA. 
:>
*/
/***************************************************************************
 * File Name  : bcm63xx_led.c
 *
 * Description: 
 *    This file contains bcm963xx board led control API functions. 
 *
 ***************************************************************************/
#include "bhal.h"

/* Includes. */
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/capability.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/string.h>
#include <linux/bcm_assert_locks.h>
//#include <asm/uaccess.h>

#include <bcm_map_part.h>
#include <board.h>
#include <boardparms.h>
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
#include "msg/kcmsmonitormsgtypes.h"
#endif
#include "atpconfig.h"

#define kVerySlowBlinkCount 3          // ~500ms

/* Hardware Porting add begin:*/
#define kFlickerCount       8          // ~1000ms
#define kPauseCount         8          // ~1000ms


#define k125ms              (HZ / 8)  // 125 ms
#define kFastBlinkCount     0         // 125ms, 4Hz
#define kSlowBlinkCount     1         // 250ms, 2Hz

#define k1sBlinkCount       7         // 1000ms
#define k2sBlinkCount       15        // 2000ms

#define kLedOff             0
#define kLedOn              1

#define kLedGreen           0
#define kLedRed             1

//#define DEBUG_LED 1

/*WIFI USB LED*/
struct timer_list gBlinkOnceTimer, g_LedTimer,g_LedRunTimer;
static int gBlinkOnceTimerOn = FALSE;
unsigned long usbLastRx = 0;
unsigned long wlanLastRx = 0;
int g_lBhalWpsStatus = 0;
unsigned int g_lWlan_Status;
unsigned int g_lUmts_status;
EXPORT_SYMBOL(g_lBhalWpsStatus);
EXPORT_SYMBOL(wlanLastRx);
EXPORT_SYMBOL(usbLastRx);

#ifdef SUPPORT_ATP_WLAN
extern int WlanWorkStatus;
#endif
void ATP_LED_CtrlLed(BOARD_LED_NAME enLedName, BOARD_LED_STATE enLedState);

/* CS status显示为Enable Active时, 有时候还是会以4hz频率闪烁 */
extern unsigned int g_lCs_status;
extern unsigned int g_lCsIsUp_status;

extern int g_USB_PROBE;
extern int g_isDataCard;

/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 start*/
extern bool g_bUmtsState;
/*DTS2012060901502 通过页面使能和去使能umts wan，数据卡LED快闪或者熄灭by w00190448 start*/

void ledEthernetStatInit(void);
#ifdef SUPPORT_ATP_WLAN
void ledWlanStatInit(void);
#endif
void ledWANStatInit(void);

void LedCtrl(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState);

extern int bhalplatGetSwichMibTxPkts(int port, unsigned int * ulTxPkts);
extern int bhalplatGetSwichMibRxPkts(int port, unsigned int * ulRxPkts);
extern unsigned int g_ulBhalLedMode;

void ledRunStart(void);
void ledTimerExpire_pro(void);
void ledStateRun(void);

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
static unsigned int g_SerLedRemainBlinkTimes = 0;
static unsigned int g_BwnLedRemainBlinkTimes = 0;
void SendLedBlinkTimes(unsigned int sTmpLedTimes);
void GetLedBlinkTimes(unsigned int *LedRemainBlinkTimes);
int GetLedState(BOARD_LED_NAME ledName);
#endif

#if 1
#define WAN_MAX_NUM 16

/* INTERNET */
INTERNETLED_DEV_INFO   g_DevList[WAN_MAX_NUM] = {0,0,0,0};
EXPORT_SYMBOL(g_DevList);

WLAN_DEV_INFO g_stWanDevList[WAN_MAX_NUM] = {0,0,0,0};
EXPORT_SYMBOL(g_stWanDevList);

ETHERNETLED_DEV_INFO g_stLanDevList[BHAL_LAN_MAX_NUM] = {0};
EXPORT_SYMBOL(g_stLanDevList);

unsigned int g_ulEthernetLinkStatus = 0;
EXPORT_SYMBOL(g_ulEthernetLinkStatus);

unsigned int g_ulEthernetInitialized = 0;
EXPORT_SYMBOL(g_ulEthernetInitialized);


#ifdef SUPPORT_ATP_WLAN
WLAN_DEV_INFO g_stWlanDevList[WLAN_2G_SSID_NUM];
#endif
ETHERNETLED_STAT_INFO g_stLanStatList[BHAL_LAN_MAX_NUM] = {0};
#endif


static int linking = 0;
int g_ledInitialized = 0;
extern int verflag;

/*设置LED是否可以控制*/
int g_ledCtlmode = LED_CONTROL_ENABLE;

typedef int (*BP_LED_FUNC) (unsigned short *);

typedef struct {
    BOARD_LED_NAME ledName;
    BP_LED_FUNC bpFunc;
    BP_LED_FUNC bpFuncFail;
} BP_LED_INFO, *PBP_LED_INFO;

typedef struct {
    HANDLE_LED_FUNC ledHwFunc;
    short ledGreenGpio;             // GPIO # for LED
    short ledRedGpio;               // GPIO # for Fail LED
    BOARD_LED_STATE ledState;       // current led state
    short blinkCountDown;           // Count for blink states
} LED_CTRL, *PLED_CTRL;

void blinkOnceTimerExpire(void);

static BP_LED_INFO bpLedInfo[] =
{
	{kLedPower, BpGetPowerLedGpio, NULL},
	//{kLedPowerFail, BpGetPowerFailLedGpio, NULL},
	{kLedAdsl, BpGetAdslLedGpio, NULL},
	{kLedInternet, BpGetInternetLedGpio, NULL},
	//{kLedInternetFail, BpGetInternetFailLedGpio, NULL},
	//{kLedLan, BpGetLanLedGpio, NULL},
	{kLedLan1, BpGetLan1LedGpio, NULL},
	{kLedLan2, BpGetLan2LedGpio, NULL},
	{kLedLan3, BpGetLan3LedGpio, NULL},
	{kLedLan4, BpGetLan4LedGpio, NULL},
	{kLedWlan, BpGetWirelessLedGpio, NULL},
	{kLedWps, BpGetWpsLedGpio, NULL},
	//{kLedVoip, BpGetVoip1LedGpio, NULL},
	//{kLedVoipFail, BpGetVoip1FailLedGpio, NULL},
	{kLedUsb, BpGetUsbLedGpio, NULL},
	//{kLedUsbFail, BpGetUsbFailLedGpio, NULL},
	{kLedEnd, NULL, NULL}
};

// global variables:
static struct timer_list gLedTimer;
static PLED_CTRL gLedCtrl = NULL;
static int gTimerOn = FALSE;
static unsigned int gLedRunningCounter = 0;

void (*ethsw_led_control)(unsigned long ledMask, int value) = NULL;
EXPORT_SYMBOL(ethsw_led_control);

static spinlock_t brcm_ledlock;
static spinlock_t brcm_autoledlock;

static void ledTimerExpire(void);

//***********************************************************************************
//LED operations 2
//***********************************************************************************

#define LOW_PIN 0
#define HIGH_PIN 1
#define NO_EFFECT 3


#define ALL_SERIAL_MODE 0
#define ALL_SWITCH_MODE 1
#define G_SERIAL_R_SWITCH_MODE 2
#define G_SWITCH_R_SERIAL_MODE 3
#define ALL_I2CLED_MODE 4

#define I2C_LED_ON  1
#define I2C_LED_OFF 2

PLED_PHY_PRO g_pstledphyHead = NULL;


LED_CTRLLIST g_ledctrllist[kLedfailstart] = {0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0};
static spinlock_t atp_ledlock;

static void ledToggle(PLED_PHY_PRO pLed);
static void ledTimerStart(void);

// turn led on and set the ledState
void ledOff(short led_gpio)
{
    /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C
    unsigned char i2cWriteValue[2] = {0};
#endif
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
    if(GPIO_NOEFFECT == led_gpio)
        return;
    char activestate = 3;
    if (led_gpio & BP_ACTIVE_LOW)
        activestate = LOW_PIN;
    else
        activestate = HIGH_PIN;

    if (led_gpio & BP_GPIO_SERIAL)
    {
    #if 1
        //printk("|s|");
        LED->ledMode &= ~(LED_MODE_MASK << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        if( activestate == LOW_PIN)
        {
            //printk("|L|");
            LED->ledMode |= (LED_MODE_OFF << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        }
        else
        {
            //printk("|H|");
            //printk("on %x\n",led_gpio);
            LED->ledMode |= (LED_MODE_ON << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        }
     #else
        while (GPIO->SerialLedCtrl & SER_LED_BUSY);
        if( activestate == LOW_PIN)
            GPIO->SerialLed &= ~(GPIO_NUM_TO_MASK(led_gpio));
        else
            GPIO->SerialLed |= GPIO_NUM_TO_MASK(led_gpio);
            
     #endif
    }
    /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C	
    else if (led_gpio & BP_GPIO_I2C)
    {
        //turn off 
		i2cWriteValue[0] = 0;
        i2cWriteValue[1] = (unsigned char)((led_gpio & ~BP_GPIO_I2C) + I2C_LED_OFF); 
        
        gpio_i2c_write(i2cWriteValue, sizeof(i2cWriteValue));
    }
#endif
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
    else
    {
        //printk("|W|");
        GPIO->GPIODir |= GPIO_NUM_TO_MASK(led_gpio);
        if( activestate == LOW_PIN)
        {
            //printk("|L|");
            GPIO->GPIOio |= GPIO_NUM_TO_MASK(led_gpio);
        }
        else
        {
            //printk("|H|");
            GPIO->GPIOio &= ~(GPIO_NUM_TO_MASK(led_gpio));
        }
    }
}

// turn led off and set the ledState
void ledOn(short led_gpio)
{
    char activestate = 3;
    /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C		
    unsigned char i2cWriteValue[2] = {0};
#endif
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
    if(GPIO_NOEFFECT == led_gpio)
        return;
    
    if (led_gpio & BP_ACTIVE_LOW)
        activestate = LOW_PIN;
    else
        activestate = HIGH_PIN;

    if (led_gpio & BP_GPIO_SERIAL)
	{
	#if 1
        //printk("|s|");
        LED->ledMode &= ~(LED_MODE_MASK << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        if( activestate == HIGH_PIN)
        {
            //printk("|L|");
            LED->ledMode |= (LED_MODE_OFF << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        }
        else
        {
            //printk("|H|");
            //printk("on %x\n",led_gpio);
            LED->ledMode |= (LED_MODE_ON << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        }
    #else
        while (GPIO->SerialLedCtrl & SER_LED_BUSY);
        if( activestate == LOW_PIN)
            GPIO->SerialLed &= ~(GPIO_NUM_TO_MASK(led_gpio));
        else
            GPIO->SerialLed |= GPIO_NUM_TO_MASK(led_gpio);
    #endif
    }
    /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C		
    else if (led_gpio & BP_GPIO_I2C)
    {
        //turn on
		i2cWriteValue[0] = 0;
        i2cWriteValue[1] = (unsigned char)((led_gpio & ~BP_GPIO_I2C) + I2C_LED_ON); 
        
        gpio_i2c_write(i2cWriteValue, sizeof(i2cWriteValue));
    }
#endif
    /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
    else
    {
        //printk("|W|");
        GPIO->GPIODir |= GPIO_NUM_TO_MASK(led_gpio);
        if( activestate == HIGH_PIN)
    	{
            //printk("|L|");
        	GPIO->GPIOio |= GPIO_NUM_TO_MASK(led_gpio);
    	}
        else
    	{
            //printk("|H|");
        	GPIO->GPIOio &= ~(GPIO_NUM_TO_MASK(led_gpio));
    	}
    }
}
//pLed->ledState = pLed->savedLedState = kLedStateOff;

void Led_Creat(PLED_PHY_PRO* ppstledctrl,BOARD_LED_NAME lname,int lmode,int lactivePinState,int lactivepin,int lfailPinState,int lfailpin)
{
    PLED_PHY_PRO pstledctrl;
    
    pstledctrl = (PLED_PHY_PRO)kmalloc(sizeof(LED_PHY_PRO), GFP_KERNEL);
    if( pstledctrl == NULL ) 
	{
        printk( "LED memory allocation error.\n" );
        return;
    }
    
    if(NULL == g_pstledphyHead)
        g_pstledphyHead = pstledctrl;

    g_ledctrllist[lname].phyled = pstledctrl;

    pstledctrl->ledHwFunc = NULL;
    pstledctrl->ledHwFailFunc = NULL;
    pstledctrl->ledState = kLedStateOff;       // current led state
    pstledctrl->blinkCountDown = 0;           // Count for blink states  
    pstledctrl->FlickerCount = 0;           // Count for blink states  
    pstledctrl->PauseCount = 0;           // Count for blink states  
    pstledctrl->failshift = 0;

    switch(lmode)
    {
        case ALL_SERIAL_MODE:
        if(lactivePinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledGpio = (lactivepin|BP_GPIO_SERIAL );
        }
        else//low pin
        {
            pstledctrl->ledGpio = (lactivepin|BP_GPIO_SERIAL | BP_ACTIVE_LOW);
        }

        if(lfailPinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_GPIO_SERIAL );    
        }
        else
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_GPIO_SERIAL | BP_ACTIVE_LOW);
        }
        break;
        
        case ALL_SWITCH_MODE:
        if(lactivePinState== HIGH_PIN)//hi pin    
        {
            pstledctrl->ledGpio = (lactivepin);
        }
        else//low pin
        {
            pstledctrl->ledGpio = (lactivepin|BP_ACTIVE_LOW);
        }

        if(lfailPinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledfailGpio = (lfailpin);    
        }
        else
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_ACTIVE_LOW);
        }
        break;
        
        case G_SERIAL_R_SWITCH_MODE:
        if(lactivePinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledGpio = (lactivepin|BP_GPIO_SERIAL );
        }
        else//low pin
        {
            pstledctrl->ledGpio = (lactivepin|BP_GPIO_SERIAL | BP_ACTIVE_LOW);
        }

        if(lfailPinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledfailGpio = (lfailpin);    
        }
        else
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_ACTIVE_LOW);
        }

        break;
        
        case G_SWITCH_R_SERIAL_MODE:
        if(lactivePinState== HIGH_PIN)//hi pin    
        {
            pstledctrl->ledGpio = (lactivepin);
        }
        else//low pin
        {
            pstledctrl->ledGpio = (lactivepin|BP_ACTIVE_LOW);
        }
        if(lfailPinState == HIGH_PIN)//hi pin    
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_GPIO_SERIAL );    
        }
        else
        {
            pstledctrl->ledfailGpio = (lfailpin|BP_GPIO_SERIAL | BP_ACTIVE_LOW);
        }
        break;

        /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
        case ALL_I2CLED_MODE:
        {
            pstledctrl->ledGpio = lactivepin|BP_GPIO_I2C;    
            pstledctrl->ledfailGpio = lfailpin|BP_GPIO_I2C;
            
            break;
        }
        /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
        default :
        break;
    }
    if(lfailPinState == NO_EFFECT)
        pstledctrl->ledfailGpio = GPIO_NOEFFECT;

    if(lactivePinState == NO_EFFECT)
        pstledctrl->ledGpio = GPIO_NOEFFECT;
}

/*
// LED defines 
typedef enum
{   
	kLedPower,
	kLedPowerFail,
	kLedAdsl,
	kLedInternet,
	kLedInternetFail,
	kLedLan,
	kLedLan1,
	kLedLan2,
	kLedLan3,
	kLedLan4,
	kLedWlan,
	kLedWps,
	kLedVoip,
	kLedVoipFail,
	kLedUsb,
	kLedUsbFail,
	kLedEnd,       // NOTE: Insert the new led name before this one.
} BOARD_LED_NAME;

*/
/*
// LED defines 
typedef enum
{   
	kLedPower,
	kLedAdsl,
	kLedAdsl2,
	kLedInternet,
	kLedLan1,
	kLedLan2,
	kLedLan3,
	kLedLan4,
	kLedLan5,
	kLedWan,
	kLedHpna,
	kLedHspa,
	kLedPpp,
	kLedWlan,
    kLedWlanEable,
    kLedWlan5G,
    kLedWlan5GEable,
	kLedWps,
	kLedUsb,
    kLedBlPower,
    kLedBlResetCfg,    
    kLedBlReStop,
	kLedVoip1,
	kLedVoip2,
	kLedService1,
	kLedService2,
	kLedEnd,       // NOTE: Insert the new led name before this one.
} BOARD_LED_NAME;
*/

static void LedInit(void)
{
    PLED_PHY_PRO pstledphy = NULL;
    //PLED_PHY_PRO pstledctrlnext = NULL;
    //LED_CTRL_PRO stledctrl2;
    //printk("______________LedInit2___________________\n");

#ifdef LED_POWER
    Led_Creat(&pstledphy,kLedPower,LED_POWER_MODE,LED_POWER_ACTIVE_PIN_STATE
        ,LED_POWER_ACTIVE_PIN,LED_POWER_FAIL_PIN_STATE,LED_POWER_FAIL_PIN);
    //led_test(pstledphy);
#endif
#ifdef LED_ADSL
        Led_Creat(&pstledphy,kLedAdsl,LED_ADSL_MODE,LED_ADSL_ACTIVE_PIN_STATE
            ,LED_ADSL_ACTIVE_PIN,LED_ADSL_FAIL_PIN_STATE,LED_ADSL_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_ADSL2
        Led_Creat(&pstledphy,kLedAdsl2,LED_ADSL2_MODE,LED_ADSL2_ACTIVE_PIN_STATE
            ,LED_ADSL2_ACTIVE_PIN,LED_ADSL2_FAIL_PIN_STATE,LED_ADSL2_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WLAN_ENABLE
        Led_Creat(&pstledphy,kLedWlanEable,LED_WLAN_ENABLE_MODE,LED_WLAN_ENABLE_ACTIVE_PIN_STATE
            ,LED_WLAN_ENABLE_ACTIVE_PIN,LED_WLAN_ENABLE_FAIL_PIN_STATE,LED_WLAN_ENABLE_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WLAN
        Led_Creat(&pstledphy,kLedWlan,LED_WLAN_MODE,LED_WLAN_ACTIVE_PIN_STATE
            ,LED_WLAN_ACTIVE_PIN,LED_WLAN_FAIL_PIN_STATE,LED_WLAN_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WLAN5G_ENABLE
        Led_Creat(&pstledphy,kLedWlan5GEable,LED_WLAN5G_MODE,LED_WLAN5G_ENABLE_ACTIVE_PIN_STATE
            ,LED_WLAN5G_ENABLE_ACTIVE_PIN,LED_WLAN5G_ENABLE_FAIL_PIN_STATE,LED_WLAN5G_ENABLE_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WLAN5G
        Led_Creat(&pstledphy,kLedWlan5G,LED_WLAN5G_MODE,LED_WLAN5G_ACTIVE_PIN_STATE
            ,LED_WLAN5G_ACTIVE_PIN,LED_WLAN5G_FAIL_PIN_STATE,LED_WLAN5G_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WPS
        Led_Creat(&pstledphy,kLedWps,LED_WPS_MODE,LED_WPS_ACTIVE_PIN_STATE
            ,LED_WPS_ACTIVE_PIN,LED_WPS_FAIL_PIN_STATE,LED_WPS_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_USB
        Led_Creat(&pstledphy,kLedUsb,LED_USB_MODE,LED_USB_ACTIVE_PIN_STATE
            ,LED_USB_ACTIVE_PIN,LED_USB_FAIL_PIN_STATE,LED_USB_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_LAN_1
        Led_Creat(&pstledphy,kLedLan1,LED_LAN_1_MODE,LED_LAN_1_ACTIVE_PIN_STATE
            ,LED_LAN_1_ACTIVE_PIN,LED_LAN_1_FAIL_PIN_STATE,LED_LAN_1_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_LAN_2
        Led_Creat(&pstledphy,kLedLan2,LED_LAN_2_MODE,LED_LAN_2_ACTIVE_PIN_STATE
            ,LED_LAN_2_ACTIVE_PIN,LED_LAN_2_FAIL_PIN_STATE,LED_LAN_2_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_LAN_3
        Led_Creat(&pstledphy,kLedLan3,LED_LAN_3_MODE,LED_LAN_3_ACTIVE_PIN_STATE
            ,LED_LAN_3_ACTIVE_PIN,LED_LAN_3_FAIL_PIN_STATE,LED_LAN_3_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_LAN_4
        Led_Creat(&pstledphy,kLedLan4,LED_LAN_4_MODE,LED_LAN_4_ACTIVE_PIN_STATE
            ,LED_LAN_4_ACTIVE_PIN,LED_LAN_4_FAIL_PIN_STATE,LED_LAN_4_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_LAN_5
        Led_Creat(&pstledphy,kLedLan5,LED_LAN_5_MODE,LED_LAN_5_ACTIVE_PIN_STATE
            ,LED_LAN_5_ACTIVE_PIN,LED_LAN_5_FAIL_PIN_STATE,LED_LAN_5_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_WAN
        Led_Creat(&pstledphy,kLedWan,LED_WAN_MODE,LED_WAN_ACTIVE_PIN_STATE
            ,LED_WAN_ACTIVE_PIN,LED_WAN_FAIL_PIN_STATE,LED_WAN_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_HPNA
        Led_Creat(&pstledphy,kLedHpna,LED_HPNA_MODE,LED_HPNA_ACTIVE_PIN_STATE
            ,LED_HPNA_ACTIVE_PIN,LED_HPNA_FAIL_PIN_STATE,LED_HPNA_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_HSPA
        Led_Creat(&pstledphy,kLedHspa,LED_HSPA_MODE,LED_HSPA_ACTIVE_PIN_STATE
            ,LED_HSPA_ACTIVE_PIN,LED_HSPA_FAIL_PIN_STATE,LED_HSPA_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_PPP
        Led_Creat(&pstledphy,kLedPpp,LED_PPP_MODE,LED_PPP_ACTIVE_PIN_STATE
            ,LED_PPP_ACTIVE_PIN,LED_PPP_FAIL_PIN_STATE,LED_PPP_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_INTERNET
        Led_Creat(&pstledphy,kLedInternet,LED_INTERNET_MODE,LED_INTERNET_ACTIVE_PIN_STATE
            ,LED_INTERNET_ACTIVE_PIN,LED_INTERNET_FAIL_PIN_STATE,LED_INTERNET_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_BL_POWER
        Led_Creat(&pstledphy,kLedBlPower,LED_BL_POWER_MODE,LED_BL_POWER_ACTIVE_PIN_STATE
            ,LED_BL_POWER_ACTIVE_PIN,LED_BL_POWER_FAIL_PIN_STATE,LED_BL_POWER_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_BL_RESETCFG
        Led_Creat(&pstledphy,kLedBlResetCfg,LED_BL_RESETCFG_MODE,LED_BL_RESETCFG_ACTIVE_PIN_STATE
            ,LED_BL_RESETCFG_ACTIVE_PIN,LED_BL_RESETCFG_FAIL_PIN_STATE,LED_BL_RESETCFG_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_BL_RESTOP
        Led_Creat(&pstledphy,kLedBlReStop,LED_BL_RESTOP_MODE,LED_BL_RESTOP_ACTIVE_PIN_STATE
            ,LED_BL_RESTOP_ACTIVE_PIN,LED_BL_RESTOP_FAIL_PIN_STATE,LED_BL_RESTOP_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_VOIP_1
        Led_Creat(&pstledphy,kLedVoip1,LED_VOIP_1_MODE,LED_VOIP_1_ACTIVE_PIN_STATE
            ,LED_VOIP_1_ACTIVE_PIN,LED_VOIP_1_FAIL_PIN_STATE,LED_VOIP_1_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_VOIP_2
        Led_Creat(&pstledphy,kLedVoip2,LED_VOIP_2_MODE,LED_VOIP_2_ACTIVE_PIN_STATE
            ,LED_VOIP_2_ACTIVE_PIN,LED_VOIP_2_FAIL_PIN_STATE,LED_VOIP_2_FAIL_PIN);
        //led_test(pstledphy);
#endif
#ifdef LED_SERVICE_1
        Led_Creat(&pstledphy,kLedService1,LED_SERVICE_1_MODE,LED_SERVICE_1_ACTIVE_PIN1_STATE
            ,LED_SERVICE_1_ACTIVE_PIN1, NO_EFFECT,GPIO_NOEFFECT);
        //led_test(pstledphy);
#endif
#ifdef LED_SERVICE_2
        Led_Creat(&pstledphy,kLedService2,LED_SERVICE_2_MODE,LED_SERVICE_2_ACTIVE_PIN1_STATE
            ,LED_SERVICE_2_ACTIVE_PIN1, NO_EFFECT,GPIO_NOEFFECT);
        //led_test(pstledphy);
#endif
#ifdef LED_SERVICE_3
        Led_Creat(&pstledphy,kLedService3,LED_SERVICE_3_MODE,LED_SERVICE_3_ACTIVE_PIN1_STATE
            ,LED_SERVICE_3_ACTIVE_PIN1, NO_EFFECT,GPIO_NOEFFECT);
        //led_test(pstledphy);
#endif
#ifdef LED_SERVICE_4
        Led_Creat(&pstledphy,kLedService4,LED_SERVICE_4_MODE,LED_SERVICE_4_ACTIVE_PIN1_STATE
            ,LED_SERVICE_4_ACTIVE_PIN1, NO_EFFECT,GPIO_NOEFFECT);
        //led_test(pstledphy);
#endif
#ifdef LED_DECT
            Led_Creat(&pstledphy,kLedDect,LED_DECT_MODE,LED_DECT_ACTIVE_PIN_STATE
                ,LED_DECT_ACTIVE_PIN,LED_DECT_FAIL_PIN_STATE,LED_DECT_FAIL_PIN);
            //led_test(pstledphy);
#endif
#ifdef LED_POTS
            Led_Creat(&pstledphy,kLedPots,LED_POTS_MODE,LED_POTS_ACTIVE_PIN_STATE
                ,LED_POTS_ACTIVE_PIN,LED_POTS_FAIL_PIN_STATE,LED_POTS_FAIL_PIN);
            //led_test(pstledphy);
#endif
#ifdef LED_sesWLAN
            Led_Creat(&pstledphy,kLedsesWlan,LED_sesWLAN_MODE,LED_sesWLAN_ACTIVE_PIN_STATE
                ,LED_sesWLAN_ACTIVE_PIN,LED_sesWLAN_FAIL_PIN_STATE,LED_sesWLAN_FAIL_PIN);
            //led_test(pstledphy);
#endif
            /* START ADD: l00252599 FOR SUPPORT LTE LED 2013-09-05*/
#ifdef LINK_LTE_LED
            Led_Creat(&pstledphy, kLedLinkLte, LINK_LTE_LED_MODE, LINK_LTE_LED_ACTIVE_PIN_STATE,
                 LINK_LTE_LED_ACTIVE_PIN, NO_EFFECT, GPIO_NOEFFECT);
#endif
#ifdef ONLINE_LTE_WHITE1
            Led_Creat(&pstledphy, kLedOnlineLte1, ONLINE_LTE_WHITE_1_MODE, ONLINE_LTE_WHITE_1_ACTIVE_PIN1_STATE,
                 ONLINE_LTE_WHITE_1_ACTIVE_PIN1, NO_EFFECT, GPIO_NOEFFECT);
#endif

#ifdef ONLINE_LTE_WHITE2
            Led_Creat(&pstledphy, kLedOnlineLte2, ONLINE_LTE_WHITE_2_MODE, ONLINE_LTE_WHITE_2_ACTIVE_PIN1_STATE,
                 ONLINE_LTE_WHITE_2_ACTIVE_PIN1, NO_EFFECT, GPIO_NOEFFECT);
#endif

#ifdef ONLINE_LTE_WHITE3
            Led_Creat(&pstledphy, kLedOnlineLte3, ONLINE_LTE_WHITE_3_MODE, ONLINE_LTE_WHITE_3_ACTIVE_PIN1_STATE,
                 ONLINE_LTE_WHITE_3_ACTIVE_PIN1, NO_EFFECT, GPIO_NOEFFECT);
#endif

#ifdef ONLINE_LTE_WHITE4
            Led_Creat(&pstledphy, kLedOnlineLte4, ONLINE_LTE_WHITE_4_MODE, ONLINE_LTE_WHITE_4_ACTIVE_PIN1_STATE,
                 ONLINE_LTE_WHITE_4_ACTIVE_PIN1, NO_EFFECT, GPIO_NOEFFECT);
#endif

#ifdef ONLINE_LTE_WHITE5
            Led_Creat(&pstledphy, kLedOnlineLte5, ONLINE_LTE_WHITE_5_MODE, ONLINE_LTE_WHITE_5_ACTIVE_PIN1_STATE,
                 ONLINE_LTE_WHITE_5_ACTIVE_PIN1, NO_EFFECT, GPIO_NOEFFECT);
#endif
            /*END ADD: l00252599 FOR SUPPORT LTE LED 2013-09-05*/
    //printk("_________________________________\n");
}

void hwfunc_operation(PLED_PHY_PRO ledInfoPtr,BOARD_LED_STATE ledState)
{    
    //调用硬件操作函数
    return;
}

static int sg_BoardTestLedcolor = 0;
static int sg_BoardTestLedtime = 0;    
static int sg_Color0IsRun = 0;
static int sg_Color1IsRun = 0;
int sg_Color0CouldRun = 1;
int sg_Color1CouldRun = 1;
static void blinkOneLed(BOARD_LED_NAME enLedName, int time)
{
    PLED_PHY_PRO pCurLed;
    if(enLedName < kLedfailstart)
    {
        pCurLed = g_ledctrllist[enLedName].phyled;
        if((NULL != pCurLed) && (pCurLed->ledGpio != GPIO_NOEFFECT))
        {
            ledOn(pCurLed->ledGpio);
            msleep(time);
            ledOff(pCurLed->ledGpio);
            msleep(time);
        }
    }
}

static void blinkOneRedLed(BOARD_LED_NAME enLedName, int time)
{
    PLED_PHY_PRO pCurLed;
    if( enLedName < kLedfailstart)
    {
        pCurLed = g_ledctrllist[enLedName].phyled;
        if((NULL != pCurLed) &&  (pCurLed->ledfailGpio != GPIO_NOEFFECT)) 
        {        
                ledOn(pCurLed->ledfailGpio);
                msleep(time);
                ledOff(pCurLed->ledfailGpio);
                msleep(time);
        }
    }
}
int boardSequenceAllLed(void *data)
{
    int i,j;
    PLED_PHY_PRO pCurLed;
    unsigned long ulflags;
    int color = sg_BoardTestLedcolor;
    int time = sg_BoardTestLedtime; 

    gTimerOn = FALSE;

    for (i = 0; i < kLedfailstart; i++)
    {
        if(NULL == g_ledctrllist[i].phyled)
        {
        	continue;
        }
        else
        {
        	pCurLed = g_ledctrllist[i].phyled;
        }
        /*modify by y00181549 跑马灯前不熄灭所有灯*/
        ledOff(pCurLed->ledGpio);
        ledOff(pCurLed->ledfailGpio);

        pCurLed->blinkCountDown = 0;       
        pCurLed->PauseCount = 0;
        pCurLed->FlickerCount = 0;
        pCurLed->ledState = kLedStateOff;
    }
    /*modify by y00181549 for 装备灯键测试跑马灯修改at 20120801*/
    if(BHAL_LED_MODE_EQUIP == g_ulBhalLedMode)
    {
        if (0 == color ) 
        {
            while((BHAL_LED_MODE_EQUIP == g_ulBhalLedMode) && (1 == sg_Color0CouldRun))
            {
                /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
#ifdef SUPPORT_ATP_GPIO_I2C			
                blinkOneLed(kLedPower, time);

                blinkOneLed(kLedOnlineLte1, time);                
                blinkOneLed(kLedOnlineLte2, time);
                blinkOneLed(kLedOnlineLte3, time);
                blinkOneLed(kLedOnlineLte4, time);
                blinkOneLed(kLedOnlineLte5, time);
                /*START MODIFY: l00252599 FOR DTS2014021600650 2014-02-24*/
                blinkOneLed(kLedLinkLte, time);
                blinkOneLed(kLedWan, time);
                blinkOneLed(kLedInternet, time);
                blinkOneLed(kLedVoip1, time);

                blinkOneLed(kLedService1, time);
                blinkOneLed(kLedService2, time);
                blinkOneLed(kLedService3, time);
                
                blinkOneLed(kLedWlan, time);
                blinkOneLed(kLedWps, time);
                blinkOneLed(kLedDect, time);
                /*END MODIFY: l00252599 FOR DTS2014021600650 2014-02-24*/
#else
                blinkOneLed(kLedPower, time);
                blinkOneLed(kLedWan, time);
                blinkOneLed(kLedInternet, time);
                blinkOneLed(kLedVoip1, time);
                blinkOneLed(kLedService1, time);
                blinkOneLed(kLedService2, time);
                blinkOneLed(kLedService3, time);
                blinkOneLed(kLedWlan, time);
                blinkOneLed(kLedWps, time);
                blinkOneLed(kLedDect, time);
#endif
                /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
                sg_Color0IsRun = 1;
            }
            sg_Color0IsRun = 0;
            sg_Color0CouldRun = 1;
        }
        if(1 == color)
        {
            while ((BHAL_LED_MODE_EQUIP == g_ulBhalLedMode) && (1 == sg_Color1CouldRun))
            {
                blinkOneRedLed(kLedPower, time);
                sg_Color1IsRun = 1;
            }
            sg_Color1IsRun = 0;
            sg_Color1CouldRun = 1;
        }
        //黄灯有需求请自行增加
    }
    else
    {
        if(0 == color ) 
        {
            //printk("color = green\n");
            for (i = 0; i < kLedfailstart; i++)
            {
                if(NULL == g_ledctrllist[i].phyled)
                {
                	continue;
                }
                else
                {
                	pCurLed = g_ledctrllist[i].phyled;
                }

    			if(pCurLed->ledGpio != GPIO_NOEFFECT)
                {
                    for(j = i + 1;j < kLedfailstart;j++)
    	            {
    	                if(NULL != g_ledctrllist[j].phyled)
    	                {
    	                    if(g_ledctrllist[j].phyled->ledGpio == pCurLed->ledGpio)
    	                    {
    	                    	break;
    	                    }
    	                }
    	            }

    				if(kLedfailstart != j)
    				{
                		continue;
                	}
    	            ledOn(pCurLed->ledGpio);
    	            msleep(time);
    	            ledOff(pCurLed->ledGpio);
    	            msleep(time);
    			}
            }
        }

        if(1 == color)
        {
            //printk("color = red\n");
            for (i = 0; i < kLedfailstart; i++)
            {
                if(NULL == g_ledctrllist[i].phyled)
                {
                	continue;
                }
                else
                {
                	pCurLed = g_ledctrllist[i].phyled;
                }
    			
    			if(pCurLed->ledfailGpio != GPIO_NOEFFECT)
                {                
                	for(j = i + 1;j < kLedfailstart;j++)
    	            {
    	                if(NULL != g_ledctrllist[j].phyled)
    	                {
    	                    if(g_ledctrllist[j].phyled->ledfailGpio == pCurLed->ledfailGpio)
    	                    {
    	                    	break;
    	                    }
    	                }
    	            }

    				if(kLedfailstart != j)
    				{
                		continue;
                	}
    				
    	            ledOn(pCurLed->ledfailGpio);
    	            msleep(time);
    	            ledOff(pCurLed->ledfailGpio);
    	            msleep(time);
    			}
            }
        }

        if(2 == color)
        {
            //printk("color = yellow\n");
            for (i = 0; i < kLedfailstart; i++)
            {
                
                if(NULL == g_ledctrllist[i].phyled)
                {
                	continue;
                }
                else
                {
                	pCurLed = g_ledctrllist[i].phyled;
                }
                if((pCurLed->ledfailGpio != GPIO_NOEFFECT) && (pCurLed->ledGpio != GPIO_NOEFFECT))
                {
                	ledOn(pCurLed->ledfailGpio);
                	ledOn(pCurLed->ledGpio);
                	msleep(time);
                	ledOff(pCurLed->ledfailGpio);
                	ledOff(pCurLed->ledGpio);
                	msleep(time);
                }
            }
        }
    }
    ledOn(g_ledctrllist[kLedPower].phyled->ledGpio);
    
    do_exit(0);
}

//:int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
void bhalSequenceAllLed(int *data)
{
    //printk("bhalSequenceAllLed %x ,%x\n",data[0],data[1]);
    /* Start up our control thread */
    sg_BoardTestLedcolor = data[0];
    sg_BoardTestLedtime = data[1];   
    if  ( 0 == sg_BoardTestLedtime)
    {
        if (0 == sg_BoardTestLedcolor)
        {
            sg_Color0CouldRun = 0;
        }
        else if (1 == sg_BoardTestLedcolor)
        {
            sg_Color1CouldRun = 0;
        }
        return;
    }
    /*相同颜色的灯只进入一次跑马灯模式*/
    if ((0 == sg_BoardTestLedcolor) && ( 1 == sg_Color0IsRun))
    {
        return;
    }
    else if ((1 == sg_BoardTestLedcolor) && ( 1 == sg_Color1IsRun))
    {
        return;
    }
    int ret = kernel_thread(boardSequenceAllLed, data, CLONE_KERNEL);

    if (ret < 0) 
    {
        printk("Unable to start control thread\n");	
    }
}

void LedALLset(unsigned int state ,unsigned int color)
{
    PLED_PHY_PRO ledInfoPtr;
    int i;

    if(0 == state)//on
    {
        if(0 == color)//green
        {
            //printk("\nall green\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if(GPIO_NOEFFECT == ledInfoPtr->ledGpio)
                    {
                        continue;
                    }
                }
                else
                    continue;

                if(GPIO_NOEFFECT != ledInfoPtr->ledfailGpio )
                {
                    ledOff(ledInfoPtr->ledfailGpio);
                }

                // Next, turn on the specified LED GPIO.
                ledOn(ledInfoPtr->ledGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
                
        }
        if(1 == color)//red
        {
            //printk("\nall red\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if(GPIO_NOEFFECT == ledInfoPtr->ledfailGpio)
                    {
                        continue;
                    }
                }
                else
                    continue;

                if(GPIO_NOEFFECT != ledInfoPtr->ledGpio )
                {
                    ledOff(ledInfoPtr->ledGpio);
                }

                // Next, turn on the specified LED GPIO.
                ledOn(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateFail;
                ledInfoPtr->nextFlickerState = kLedStateFail;
            }
        }
        
        if(2 == color)//yellow
        {
            //printk("\nall yellow\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                ledInfoPtr = NULL;
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if((GPIO_NOEFFECT == ledInfoPtr->ledGpio)||(GPIO_NOEFFECT == ledInfoPtr->ledfailGpio))
                    {
                        continue;
                    }
                }
                else
                    continue;

                ledOn(ledInfoPtr->ledGpio);
                ledOn(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
        }

        if(3 == color)//all
        {
            //printk("\nall yellow\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                ledInfoPtr = NULL;
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];
                }
                else
                    continue;

                ledOn(ledInfoPtr->ledGpio);
                ledOn(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
        }
    }
    else//off
    {
         if(0 == color)//red
         {  // printk("\nall green\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if(GPIO_NOEFFECT == ledInfoPtr->ledGpio)
                    {
                        continue;
                    }
                }
                else
                    continue;

                // Next, turn on the specified LED GPIO.
                ledOff(ledInfoPtr->ledGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
        }
         
        if(1 == color)//red
        {
            //printk("\nall red\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if(GPIO_NOEFFECT == ledInfoPtr->ledfailGpio)
                    {
                        continue;
                    }
                }
                else
                    continue;


                // Next, turn on the specified LED GPIO.
                ledOff(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateFail;
                ledInfoPtr->nextFlickerState = kLedStateFail;
            }
        }
        
        if(2 == color)//yellow
        {
            //printk("\nall yellow\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                ledInfoPtr = NULL;
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];

                    if((GPIO_NOEFFECT == ledInfoPtr->ledGpio)||(GPIO_NOEFFECT == ledInfoPtr->ledfailGpio))
                    {
                        continue;
                    }
                }
                else
                    continue;

                ledOff(ledInfoPtr->ledGpio);
                ledOff(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
        }

        if(3 == color)//all
        {
            //printk("\nall yellow\n");
            for(i = 0;i < kLedfailstart;i++)
            {
                ledInfoPtr = NULL;
                if(NULL != g_ledctrllist[i].phyled)
                {
                    ledInfoPtr = g_ledctrllist[i].phyled; //gpVirtLeds[(int) ledName];
                }
                else
                    continue;

                ledOff(ledInfoPtr->ledGpio);
                ledOff(ledInfoPtr->ledfailGpio);
                ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
                ledInfoPtr->nextFlickerState = kLedStateOn;
            }
        }
    }

}

void LedCtrl(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState)
{
    PLED_PHY_PRO ledInfoPtr;
    unsigned long ulflags = 0;
    int failledshiftMode = 0;
    short tmpgpio = 0;
    HANDLE_LED_FUNC tmpfunc = 0;

    //printk("LedCtrl\r\n");
    if (kLedWlan == ledName)
	{
		printk("LedCtrl ledName:%d ledState:%d\r\n",ledName,ledState);
	}
    if ((int)ledName > kLedfailstart && (int)ledName < kLedEnd)
    {
        ledName = ledName - kLedPowerfail;

        if ((int)ledName < 0)
        {
            //printk("LedCtrl err 1\r\n");
            return;
        }
        else
    	{
        	failledshiftMode = 1;
		}
    }

    // do the mapping from virtual to physical led
    if ((int)ledName < kLedfailstart)
    {
        if (NULL != g_ledctrllist[(int)ledName].phyled)
    	{
        	ledInfoPtr = g_ledctrllist[(int)ledName].phyled; //gpVirtLeds[(int) ledName];
    	}
        else
        {
            //printk("LedCtrl err 2\r\n");
            return;
        }
    }
    else
    {
        ledInfoPtr = NULL;
        return;
    }
	
    //对于fail灯的行动，直接对掉绿红灯的mask
    if (failledshiftMode != ledInfoPtr->failshift)
    {
        //printk("LedCtrl shift");
        tmpgpio = ledInfoPtr->ledGpio;
        ledInfoPtr->ledGpio = ledInfoPtr->ledfailGpio;
        ledInfoPtr->ledfailGpio = tmpgpio;

        tmpfunc = ledInfoPtr->ledHwFunc;
        ledInfoPtr->ledHwFunc = ledInfoPtr->ledHwFailFunc;
        ledInfoPtr->ledHwFailFunc = tmpfunc;

        ledInfoPtr->failshift = failledshiftMode;
    }
  
    //printk("Led1 Ctrl:");

    if (ledInfoPtr->ledHwFailFunc || ledInfoPtr->ledHwFunc )
    {
        hwfunc_operation(ledInfoPtr,ledState);
        return;
    }
    spin_lock_irqsave(&atp_ledlock, ulflags);
    //开始操作
    //printk("stage 2 ");

    // If the state is kLedStateFail and there is not a failure LED defined
    // in the board parameters, change the state to kLedStateSlowBlinkContinues.
    if( ledState == kLedStateFail && ledInfoPtr->ledfailGpio == GPIO_NOEFFECT )
        ledState = kLedStateSlowBlinkContinues;

    //printk("Change State: %i",ledState);
    //所有的点亮操作之前都要求先灭掉fail灯
    switch (ledState)
    {
        case kLedStateOn:
            // First, turn off the complimentary (failure) LED GPIO.
            if(GPIO_NOEFFECT != ledInfoPtr->ledfailGpio )
            {
                ledOff(ledInfoPtr->ledfailGpio);
            }
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
			if (kLedService2 == ledName)
			{
				ledOn(g_ledctrllist[kLedService2].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedService3].phyled->ledGpio);
			}
			else
			{
				ledOn(ledInfoPtr->ledGpio);
			}
#else
            // Next, turn on the specified LED GPIO.
            ledOn(ledInfoPtr->ledGpio);
#endif
            ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOn;
            ledInfoPtr->nextFlickerState = kLedStateOn;
			
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            break;

        case kLedStateOff: 
            // First, turn off the complimentary (failure) LED GPIO.
            if(GPIO_NOEFFECT != ledInfoPtr->ledfailGpio )
            {
                ledOff(ledInfoPtr->ledfailGpio);
            }
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
			if (kLedService2 == ledName)
			{
				ledOff(g_ledctrllist[kLedService2].phyled->ledGpio);
				ledOff(g_ledctrllist[kLedService3].phyled->ledGpio);
			}
			else
			{
				ledOff(ledInfoPtr->ledGpio);
			}
#else
            // Next, turn off the specified LED GPIO.
            ledOff(ledInfoPtr->ledGpio);
#endif
            
            ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateOff;
            ledInfoPtr->nextFlickerState = kLedStateOff;
            
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            break;

        case kLedStateFail:
            // First, turn off the complimentary (normal) LED GPIO.
            if(GPIO_NOEFFECT != ledInfoPtr->ledGpio)
            //if( ledInfoPtr->ledMask )
            {
                ledOff(ledInfoPtr->ledGpio);
            }

            // Next, turn on (red) the specified LED GPIO.
            ledOn(ledInfoPtr->ledfailGpio);
            
            ledInfoPtr->ledState = ledInfoPtr->savedLedState = kLedStateFail;
            ledInfoPtr->nextFlickerState = kLedStateFail;
            
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            break;

        case kLedStateBlinkOnce:
            // skip blinkOnce if it is already in Slow/Fast blink continues state
            if (ledInfoPtr->savedLedState == kLedStateSlowBlinkContinues ||
                ledInfoPtr->savedLedState == kLedStateFastBlinkContinues)
            {
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
            }
            else
            {
                if ( ledInfoPtr->blinkCountDown ||  // skip the call if it is 1
                     gBlinkOnceTimerOn            ) // Keven -- skip the call if BlinkOnceTimer is on
                {
                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                }
                else
                {
                    ledToggle(ledInfoPtr);//查看当前状态
                    ledInfoPtr->blinkCountDown = 1;  // it will be reset to 0 when timer expires
                    ledInfoPtr->ledState = kLedStateBlinkOnce;
                    
                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    ledTimerStart();
                }
            }
            break;

        /* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add begin:*/
        case kLedStateSlowFlickerOnce:
            // 最后本周期内如果仍然有包到达，则再点亮一个周期，误差为2s+2s]

            if( ledInfoPtr->FlickerCount != 0 || ledInfoPtr->PauseCount != 0)
            {
                    ledInfoPtr->nextFlickerState = kLedStateSlowFlickerOnce;
                    
                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
            }
            else
            {
                ledInfoPtr->blinkCountDown = kSlowBlinkCount;  // it will be reset to 0 when timer expires
                ledInfoPtr->FlickerCount = kFlickerCount;             
                ledInfoPtr->PauseCount = kPauseCount;             
                ledInfoPtr->ledState = kLedStateSlowFlickerOnce;
                ledInfoPtr->nextFlickerState = kLedStateOn;
                
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                ledTimerStart();
            }
            break;
            
        case kLedStateFastFlickerOnce:
            // 最后本周期内如果仍然有包到达，则再点亮一个周期，误差为2s+2s
            /* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add begin:*/
#if 0
            if (  g_nEquipTestMode == TRUE )
            {
                ledInfoPtr->FlickerCount = 0;
                ledInfoPtr->PauseCount = 0;
                break;
            } 
            else
#endif
            {

                /* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add end.*/
                if ( ledInfoPtr->FlickerCount != 0 || ledInfoPtr->PauseCount != 0)
                {
                        ledInfoPtr->nextFlickerState = kLedStateFastFlickerOnce;
                        spin_unlock_irqrestore(&atp_ledlock, ulflags);
                        
                }
                else
                {
                    ledInfoPtr->blinkCountDown = kFastBlinkCount;  // it will be reset to 0 when timer expires
                    ledInfoPtr->FlickerCount = kFlickerCount;             
                    ledInfoPtr->PauseCount = kPauseCount;             
                    ledInfoPtr->ledState = kLedStateFastFlickerOnce;
                    ledInfoPtr->nextFlickerState = kLedStateOn;

                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    ledTimerStart();
                }
                break;
            /* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add begin:*/
            }
            /* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add end.*/
        /* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add end.*/
                
        case kLedStateVerySlowBlinkContiunes:       /* Jiajun 12/18/2007 for 500ms blink */ 
            ledInfoPtr->blinkCountDown = kVerySlowBlinkCount;
            ledInfoPtr->ledState = kLedStateVerySlowBlinkContiunes;
            ledInfoPtr->savedLedState = kLedStateVerySlowBlinkContiunes;
            
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;

        case kLedStateSlowBlinkContinues:
            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
            ledInfoPtr->ledState = kLedStateSlowBlinkContinues;
            ledInfoPtr->savedLedState = kLedStateSlowBlinkContinues;

            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;

        case kLedStateFastBlinkContinues:
            ledInfoPtr->blinkCountDown = kFastBlinkCount;
            ledInfoPtr->ledState = kLedStateFastBlinkContinues;
            ledInfoPtr->savedLedState = kLedStateFastBlinkContinues;
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;

		case kLedStateCrossFastBlinkContinues:
			//BHAL_LED_STATE_UPG_FASTBLINK_CONTINUES
            //printk("kLedStateCrossFastBlinkContinues state\r\n");
            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
            ledInfoPtr->ledState = kLedStateCrossFastBlinkContinues;
            ledInfoPtr->savedLedState = kLedStateCrossFastBlinkContinues;
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;

         /*Added by lvxin00135113@20091113 for support DT key event*/   
        case kLedStateBlink2HZOnce:
            if ( ledInfoPtr->FlickerCount != 0 || ledInfoPtr->PauseCount != 0)
            {
                ledInfoPtr->nextFlickerState = kLedStateBlink2HZOnce;
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
            }
            else
            {
                ledInfoPtr->blinkCountDown = kVerySlowBlinkCount;  // it will be reset to 0 when timer expires
                ledInfoPtr->FlickerCount = kFlickerCount;             
                ledInfoPtr->PauseCount = kPauseCount;             
                ledInfoPtr->ledState = kLedStateBlink2HZOnce;
                ledInfoPtr->nextFlickerState = kLedStateOn;
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                
                ledTimerStart();
            }          
                
                
            break;
            /*Added by lvxin00135113@20091113 for support DT key event*/
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
		case kLedStateServiceSlowBlinkContinues:
			ledInfoPtr->blinkCountDown = kSlowBlinkCount;
            ledInfoPtr->ledState = kLedStateServiceSlowBlinkContinues;
            ledInfoPtr->savedLedState = kLedStateServiceSlowBlinkContinues;
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;
		case kLedStateBitteWartenSlowBlinkContinues:
			ledInfoPtr->blinkCountDown = kSlowBlinkCount;
            ledInfoPtr->ledState = kLedStateBitteWartenSlowBlinkContinues;
            ledInfoPtr->savedLedState = kLedStateBitteWartenSlowBlinkContinues;
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            ledTimerStart();
            break;
#endif
#if defined(SUPPORT_ATP_WLAN_DT)
        /* START OF MODIFY by y00181549 for DTS2014010607412 at 2014/1/24 要慢闪wps+wlan或者wps+voip灯时，需要同时判断另外一个灯的闪烁状态
        只有当另外一个灯也为慢闪的状态的时候，才应该将三个灯同步闪烁，否则只应该闪烁2个灯*/
		case kLedWpsWlanStateSlowBlinkContinues:
 			if ((kLedWps == ledName) && ((kLedWpsVoipStateSlowBlinkContinues == g_ledctrllist[kLedVoip1].phyled->ledState)
				|| (kLedWpsWlanVoipStateSlowBlinkContinues == g_ledctrllist[kLedVoip1].phyled->ledState)))
 			{
 				/* 
 				 * WPS 灯前一状态为WPS/VOIP 或WPS/WLAN/VOIP, 
 				 * 现在状态为WPS/WLAN, 将WPS 灯状态设置为WPS/WLAN/VOIP状态
				 */
	            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
	            ledInfoPtr->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
	            ledInfoPtr->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;
				
				g_ledctrllist[kLedWlan].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedWlan].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
				g_ledctrllist[kLedWlan].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

				g_ledctrllist[kLedVoip1].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedVoip1].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
				g_ledctrllist[kLedVoip1].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

				/* 先点亮WPS/WLAN/VOIP 灯, 保证闪烁步调一致*/
				ledOn(g_ledctrllist[kLedWps].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedWlan].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedVoip1].phyled->ledGpio);                
 			}
			else
			{
				/* 设置WPS/WLAN 灯2Hz 闪烁参数*/
	            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
	            ledInfoPtr->ledState = kLedWpsWlanStateSlowBlinkContinues;
	            ledInfoPtr->savedLedState = kLedWpsWlanStateSlowBlinkContinues;

				g_ledctrllist[kLedWlan].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedWlan].phyled->ledState = kLedWpsWlanStateSlowBlinkContinues;
				g_ledctrllist[kLedWlan].phyled->savedLedState = kLedWpsWlanStateSlowBlinkContinues;

				/* 先点亮WPS/WLAN 灯, 保证闪烁步调一致*/
				ledOn(g_ledctrllist[kLedWps].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedWlan].phyled->ledGpio);
			}
            spin_unlock_irqrestore(&atp_ledlock, ulflags);

			ledTimerStart();
            break;
        case kLedWpsVoipStateSlowBlinkContinues:
			if ((kLedWps == ledName) && ((kLedWpsWlanStateSlowBlinkContinues == g_ledctrllist[kLedWlan].phyled->ledState)
				|| (kLedWpsWlanVoipStateSlowBlinkContinues == g_ledctrllist[kLedWlan].phyled->ledState)))
 			{
 				/* 
 				 * WPS 灯前一状态为WPS/WLAN 或WPS/WLAN/VOIP, 
 				 * 现在状态为WPS/VOIP, 将WPS 灯状态设置为WPS/WLAN/VOIP状态
				 */
	            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
	            ledInfoPtr->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
	            ledInfoPtr->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;
				
				g_ledctrllist[kLedWlan].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedWlan].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
				g_ledctrllist[kLedWlan].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

				g_ledctrllist[kLedVoip1].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedVoip1].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
				g_ledctrllist[kLedVoip1].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

				/* 先点亮WPS/WLAN/VOIP 灯, 保证闪烁步调一致*/
				ledOn(g_ledctrllist[kLedWps].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedWlan].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedVoip1].phyled->ledGpio);
 			}
			else
			{
				/* 设置WPS/VOIP 灯2Hz 闪烁参数*/
	            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
	            ledInfoPtr->ledState = kLedWpsVoipStateSlowBlinkContinues;
	            ledInfoPtr->savedLedState = kLedWpsVoipStateSlowBlinkContinues;

				g_ledctrllist[kLedVoip1].phyled->blinkCountDown = kSlowBlinkCount;
				g_ledctrllist[kLedVoip1].phyled->ledState = kLedWpsVoipStateSlowBlinkContinues;
				g_ledctrllist[kLedVoip1].phyled->savedLedState = kLedWpsVoipStateSlowBlinkContinues;

				/* 先点亮WPS/VOIP 灯, 保证闪烁步调一致*/
				ledOn(g_ledctrllist[kLedWps].phyled->ledGpio);
				ledOn(g_ledctrllist[kLedVoip1].phyled->ledGpio);
			}
            spin_unlock_irqrestore(&atp_ledlock, ulflags);

			ledTimerStart();
            break;
        /* END OF MODIFY by y00181549 for DTS2014010607412 at 2014/1/24 要慢闪wps+wlan或者wps+voip灯时，需要同时判断另外一个灯的闪烁状态
        只有当另外一个灯也为慢闪的状态的时候，才应该将三个灯同步闪烁，否则只应该闪烁2个灯*/
		case kLedWpsWlanVoipStateSlowBlinkContinues:
			/* 设置WPS/WLAN/VOIP 灯2Hz 闪烁参数*/
            ledInfoPtr->blinkCountDown = kSlowBlinkCount;
            ledInfoPtr->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
            ledInfoPtr->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

			g_ledctrllist[kLedWlan].phyled->blinkCountDown = kSlowBlinkCount;
			g_ledctrllist[kLedWlan].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
			g_ledctrllist[kLedWlan].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

			g_ledctrllist[kLedVoip1].phyled->blinkCountDown = kSlowBlinkCount;
			g_ledctrllist[kLedVoip1].phyled->ledState = kLedWpsWlanVoipStateSlowBlinkContinues;
			g_ledctrllist[kLedVoip1].phyled->savedLedState = kLedWpsWlanVoipStateSlowBlinkContinues;

			/* 先点亮WPS/WLAN/VOIP 灯, 保证闪烁步调一致*/
			ledOn(g_ledctrllist[kLedWps].phyled->ledGpio);
			ledOn(g_ledctrllist[kLedWlan].phyled->ledGpio);
			ledOn(g_ledctrllist[kLedVoip1].phyled->ledGpio);
            spin_unlock_irqrestore(&atp_ledlock, ulflags);

			ledTimerStart();
            break;
#endif
        default:
            printk("Invalid led state\r\n");
            spin_unlock_irqrestore(&atp_ledlock, ulflags);
            
            break;
    }

}


//**************************************************************************************
// LED operations
//*************************************************************************************
// toggle the LED
static void ledToggle(PLED_PHY_PRO pLed)
{
    short led_gpio;
	
    //printk("Toggle");
    led_gpio = pLed->ledGpio;

    if (led_gpio == -1)
	{
    	return;
	}

    if (led_gpio & BP_GPIO_SERIAL) 
    {
        LED->ledMode ^= (LED_MODE_MASK << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
#if 0
        while (GPIO->SerialLedCtrl & SER_LED_BUSY);
        GPIO->SerialLed ^= GPIO_NUM_TO_MASK(led_gpio);
#endif
    }
    else 
    {
        GPIO->GPIODir |= GPIO_NUM_TO_MASK(led_gpio);
        GPIO->GPIOio ^= GPIO_NUM_TO_MASK(led_gpio);
    }
}  

// Keven -- LED BlinkOnce timer 
void blinkOnceTimerStart(void)
{
    if(gBlinkOnceTimerOn)
       return;

    init_timer(&gBlinkOnceTimer);
    gBlinkOnceTimer.function = (void*)blinkOnceTimerExpire;
    gBlinkOnceTimer.expires = jiffies + k125ms; 
    add_timer(&gBlinkOnceTimer);
    gBlinkOnceTimerOn = TRUE;
}

void blinkOnceTimerExpire(void)
{
    gBlinkOnceTimerOn = FALSE;
}

// led timer.  Will return if timer is already on
static void ledTimerStart(void)
{
    if (gTimerOn)
	{
    	return;
	}

    BCM_ASSERT_HAS_SPINLOCK_C(&atp_ledlock);

    init_timer(&gLedTimer);
    gLedTimer.function = (void*)ledTimerExpire_pro;
    gLedTimer.expires = jiffies + k125ms;        // timer expires in ~100ms
    add_timer (&gLedTimer);
    gTimerOn = TRUE;
} 

void ledTimerExpire_pro(void)
{
    int i;
    PLED_PHY_PRO pCurLed;
    unsigned long ulflags;
	static char ledonstate = 0;

    //printk("ledTimerExpire \n");
    gTimerOn = FALSE;
    BCM_ASSERT_NOT_HAS_SPINLOCK_V(&atp_ledlock);

    //for (i = 0, pCurLed = gLed; i < gLedCount; i++, pCurLed++)
    for (i = 0; i < kLedfailstart; i++)
    {
        if(NULL == g_ledctrllist[i].phyled)
            continue;
        else
            pCurLed = g_ledctrllist[i].phyled;
#if defined(DEBUG_LED)
        printk("led[%d]: Mask=0x%04x, State = %d, blcd=%d\n", i, pCurLed->ledMask, pCurLed->ledState, pCurLed->blinkCountDown);
#endif
        spin_lock_irqsave(&atp_ledlock, ulflags);        // LEDs can be changed from ISR
        switch (pCurLed->ledState)
        {
            case kLedStateOn:
            case kLedStateOff:
            case kLedStateFail:
                pCurLed->blinkCountDown = 0;            // reset the blink count down
                /* HUAWEI HGW s48571 2008年1月18日 Hardware porting add begin:*/
                pCurLed->PauseCount = 0;
                pCurLed->FlickerCount = 0;
                /* HUAWEI HGW s48571 2008年1月18日 Hardware porting add end.*/                
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                break;

            case kLedStateBlinkOnce:
                blinkOnceTimerStart();                            // Keven -- Start BlinkOnceTimer
                ledToggle(pCurLed);
                pCurLed->blinkCountDown = 0;                      // reset to 0
                pCurLed->ledState = pCurLed->savedLedState;
                if (pCurLed->ledState == kLedStateSlowBlinkContinues || 
                    pCurLed->ledState == kLedStateFastBlinkContinues)
                {
                    spin_unlock_irqrestore(&atp_ledlock, ulflags);

                    ledTimerStart();                  // start timer if in blinkContinues stats
                }
                else
                {
                  spin_unlock_irqrestore(&atp_ledlock, ulflags);
                }           
                break;

            /* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add begin:*/
            case kLedStateSlowFlickerOnce:
            case kLedStateFastFlickerOnce:
                if( pCurLed->FlickerCount-- > 0)
                {
                    if ( pCurLed->blinkCountDown-- == 0)
                    {
                        if (kLedStateSlowFlickerOnce == pCurLed->ledState)
                            pCurLed->blinkCountDown = kSlowBlinkCount;
                        else
                            pCurLed->blinkCountDown = kFastBlinkCount;
                        
                         ledToggle(pCurLed);
                         pCurLed->savedLedState = kLedStateOn;
                    }
                }
                else if( pCurLed->PauseCount-- > 0)
                {
                    if ( pCurLed->savedLedState == kLedStateOn )
                    {
                        ledToggle(pCurLed);
                        pCurLed->savedLedState = kLedStateOn;
                    }
                    
                    if(( 0 == pCurLed->PauseCount)&&( kLedStateFastFlickerOnce == pCurLed->nextFlickerState))
                    {
                        pCurLed->ledState = kLedStateFastFlickerOnce;
                        pCurLed->blinkCountDown = kFastBlinkCount;            
                        pCurLed->PauseCount = 8;
                        pCurLed->FlickerCount = 8;
                        pCurLed->nextFlickerState = kLedStateOn;
                    }
                }
                else
                {
                    ledOn(pCurLed->ledGpio);
                }
               
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                ledTimerStart();
                break;
            /* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add end.*/
           
            /* Jiajun 12/18/2007 for ~500ms blink */
            case kLedStateVerySlowBlinkContiunes:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kVerySlowBlinkCount;
                    ledToggle(pCurLed);                    
                }
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
				
                ledTimerStart();
                //spin_unlock_irqrestore(&atp_ledlock, ulflags);

                break;

            case kLedStateSlowBlinkContinues:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kSlowBlinkCount;
                    ledToggle(pCurLed);
                }

                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                
                ledTimerStart();
                //spin_unlock_irqrestore(&atp_ledlock, ulflags);

                break;

            case kLedStateFastBlinkContinues:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kFastBlinkCount;
                    ledToggle(pCurLed);
                }

                spin_unlock_irqrestore(&atp_ledlock, ulflags);
				
                ledTimerStart();



                break;
               
              /*Added by lvxin00135113@20091113 for support DT key event*/   
             case kLedStateBlink2HZOnce:
                if( pCurLed->FlickerCount-- > 0)
                {
                    if ( pCurLed->blinkCountDown-- == 0)
                    {
                        if (kLedStateSlowFlickerOnce == pCurLed->ledState)
                            pCurLed->blinkCountDown = kSlowBlinkCount;
                        else
                            pCurLed->blinkCountDown = kFastBlinkCount;
                        
                        pCurLed->savedLedState = kLedStateOn;
                            ledToggle(pCurLed);
                    }             
                }
                
                if(0 >= pCurLed->FlickerCount)
                {
                    //g_ulBlinkOnceSuccess = 1;/*置灯已经成功闪烁1次标识为1*/
                }
                

                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                
                ledTimerStart();
                break;

				case kLedStateCrossFastBlinkContinues:
				//printk("upg blink!\n");
				if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kSlowBlinkCount;
					if(1 == ledonstate)
					{   
						//printk("+");
						ledOn(pCurLed->ledGpio);
						ledOff(pCurLed->ledfailGpio);
						ledonstate = 0;
					}
					else
					{
						//printk("-");
						ledOff(pCurLed->ledGpio);
						ledOn(pCurLed->ledfailGpio);
						ledonstate = 1;
					}
				}

                spin_unlock_irqrestore(&atp_ledlock, ulflags);
				
				ledTimerStart();

				break;
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
            case kLedStateServiceSlowBlinkContinues:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kSlowBlinkCount;
                    ledToggle(pCurLed);
                    g_SerLedRemainBlinkTimes--;
                }

                if (g_SerLedRemainBlinkTimes <= 0)
                {
                    ledOff(pCurLed->ledGpio);
                    pCurLed->ledState = kLedStateOff;
                    syswatch_nl_send(ATP_MONITOR_EVT_SERVLED_STOP, NULL, 0);

                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    break;
                }
                else
                {						

                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    ledTimerStart();
                }
                break;
            case kLedStateBitteWartenSlowBlinkContinues:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kSlowBlinkCount;
                    ledToggle(g_ledctrllist[kLedService2].phyled);
                    ledToggle(g_ledctrllist[kLedService3].phyled);
                    g_BwnLedRemainBlinkTimes--;
                }

                if (g_BwnLedRemainBlinkTimes <= 0)
                {
                    ledOff(g_ledctrllist[kLedService2].phyled->ledGpio);
                    ledOff(g_ledctrllist[kLedService3].phyled->ledGpio);
                    g_ledctrllist[kLedService2].phyled->ledState = kLedStateOff;
                    g_ledctrllist[kLedService3].phyled->ledState = kLedStateOff;
                    syswatch_nl_send(ATP_MONITOR_EVT_SERVLED_STOP, NULL, 0);

                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    break;
                }
                else
                {						

                    spin_unlock_irqrestore(&atp_ledlock, ulflags);
                    
                    ledTimerStart();
                }
                break;
#endif
#if defined(SUPPORT_ATP_WLAN_DT)
			case kLedWpsWlanStateSlowBlinkContinues:
			case kLedWpsVoipStateSlowBlinkContinues:
			case kLedWpsWlanVoipStateSlowBlinkContinues:
                if (pCurLed->blinkCountDown-- == 0)
                {
                    pCurLed->blinkCountDown = kSlowBlinkCount;
                    ledToggle(pCurLed);
                }

                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                
                ledTimerStart();
                break;
#endif
            default:
            {
                spin_unlock_irqrestore(&atp_ledlock, ulflags);
                printk("Invalid state = %d\n", pCurLed->ledState);
            }
        }
    }
}

/*Start Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/
#define TIMER_1HZ_TIMES 8
#define TIMER_INTERVAL_125MS        (HZ / TIMER_1HZ_TIMES)              /*125ms*/
#define LED_DELAY_TIME              (((1000) / TIMER_1HZ_TIMES) * 2)    /*250ms*/
#define BUFFER_LEN 256
int g_LedTimerOn = FALSE;
int g_LedRunOn = FALSE;

int g_LedTimerPause = FALSE;
int g_LedBlinkOnceTimerOn = FALSE;
int g_LedEnable = TRUE;

void ledStatusAfterInit(void);

void __init boardLedInit(void)
{
    PBP_LED_INFO pInfo;
    ETHERNET_MAC_INFO EnetInfo;
    unsigned short i,j;
    short gpio;
    PLED_PHY_PRO pledphy = NULL;
    PLED_PHY_PRO pledphytmp = NULL;

    ledEthernetStatInit();
#ifdef SUPPORT_ATP_WLAN
    ledWlanStatInit();
#endif
    ledWANStatInit();

    spin_lock_init(&atp_ledlock);    
    
    for(i = 0;i<kLedfailstart;i++)
        g_ledctrllist[i].phyled = NULL;

    LedInit();

    // Check for multiple LED names and multiple LED GPIO pins that share the
    // same physical board LED.

    //printk("share the same physical board LED check 222\n");
#if 1
#if 1
    for(i = 0;i < kLedfailstart;i++)
    {
        pledphytmp = NULL;
        pledphy = g_ledctrllist[i].phyled;
        if(NULL != pledphy)
        {
            //printk("led %i effect ",i);
            #if 1
            for(j = i + 1;j < kLedfailstart;j++)
            {
                if(NULL != g_ledctrllist[j].phyled)
                {
                    if((g_ledctrllist[j].phyled->ledGpio == pledphy->ledGpio)&&
                        (g_ledctrllist[j].phyled->ledfailGpio == pledphy->ledfailGpio))
                    {
                        printk(">");
                        pledphytmp = g_ledctrllist[j].phyled;
                    }
                }
            }

            //指向相同的尾部节点
            if(NULL != pledphytmp)
            {
                g_ledctrllist[i].phyled = pledphytmp;
                //释放当前节点
                printk("kfree2 \n");
                kfree(pledphy);
            }
            //printk("\n");
            #endif
        }
        else
            continue;
    }
    
    unsigned short GPIOOverlays;
    /* Start with all HW LEDs disabled */
    LED->ledHWDis |= 0xFFFFFF;
	
    if( BpGetGPIOverlays(&GPIOOverlays) == BP_SUCCESS ) 
	{
        if (GPIOOverlays & BP_OVERLAY_SERIAL_LEDS) 
		{
            GPIO->GPIOMode |= (GPIO_MODE_SERIAL_LED_CLK | GPIO_MODE_SERIAL_LED_DATA);
            LED->ledInit |= LED_SERIAL_LED_EN;
        }

	    //GPIO[51:36]需要设置状态后LED才能使用,见寄存器说明41页
	    GPIO->GPIOCtrl |= (1<<6);//38
	    GPIO->GPIOCtrl |= (1<<5);//37
	    GPIO->GPIOCtrl |= (1<<4);//36
		/*
			8 inet
			9 ephy1
			10 ephy2
			11 ephy3
			12 gphy1
			13 egphy1
			14 egphy2
			15 egphy3
			0  gphy1
			1  gphy1
		*/ 

	    //GPIO[8]需要让硬件放弃对inet的控制
		GPIO->LEDCtrl &= ~(1<<8);
		GPIO->LEDCtrl &= ~(1<<9);
		GPIO->LEDCtrl &= ~(1<<10);
		GPIO->LEDCtrl &= ~(1<<11);
		GPIO->LEDCtrl &= ~(1<<12);
		GPIO->LEDCtrl &= ~(1<<13);
		GPIO->LEDCtrl &= ~(1<<14);
		GPIO->LEDCtrl &= ~(1<<15);
#ifdef SUPPORT_ATP_VOICE_DT
		GPIO->LEDCtrl &= ~(1 << 22);
#endif
		GPIO->LEDCtrl &= ~(1<<0);
		GPIO->LEDCtrl &= ~(1<<1); 

        //37位是高位时36才能控制led
        ledOn(BP_GPIO_37_AH);
#if 0
        if (GPIOOverlays & BP_OVERLAY_PHY) {
            GPIO->GPIOBaseMode |= (GPIO_BASE_VDSL_PHY_OVERRIDE_0 | GPIO_BASE_VDSL_PHY_OVERRIDE_1);
        }

        /* Enable PCIe CLKREQ signal */
        if (GPIOOverlays & BP_OVERLAY_PCIE_CLKREQ) {
            GPIO->GPIOMode |= GPIO_MODE_PCIE_CLKREQ_B;
        }
        if (GPIOOverlays & BP_OVERLAY_USB_LED) {
            LED->ledHWDis &= ~(1 << LED_USB_ACT);
        }
        
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_0) {
            GPIO->LEDCtrl |= (1 << LED_EPHY0_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY0_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_1) {
            GPIO->LEDCtrl |= (1 << LED_EPHY1_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY1_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_2) {
            GPIO->LEDCtrl |= (1 << LED_EPHY2_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY2_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_GPHY_LED_0) {
            GPIO->LEDCtrl |= (1 << LED_GPHY0_ACT);
            GPIO->LEDCtrl |= (1 << LED_GPHY0_SPD0);
            GPIO->LEDCtrl |= (1 << LED_GPHY0_SPD1);
        }
#endif
        #if 0
        if ( BpGetWanDataLedGpio(&gpio) == BP_SUCCESS ) 
        {
            if ((gpio & BP_GPIO_NUM_MASK) == LED_INET_ACT) 
            {
                /* WAN Data LED must be LED 8 */
                if (!(gpio & BP_GPIO_SERIAL)) 
                {
                    /* If LED is not serial, enable corresponding GPIO */
                    GPIO->GPIOMode |= GPIO_NUM_TO_MASK(gpio);
                }
            }
        }
        /* Enable LED controller to drive GPIO when LEDs are connected to GPIO pins */


        #endif
    }

    
#endif
//全灯熄灭

#ifndef LED_SEQUENCE_BLINK
    for(i = 0;i < kLedfailstart;i++)
    {
        pledphytmp = NULL;
        pledphy = g_ledctrllist[i].phyled;
        if(NULL != pledphy)
        {
            printk("led %i  ledGpio:%x ledfailGpio:%x\n",
                i,pledphy->ledGpio,pledphy->ledfailGpio);
            ledOff(pledphy->ledGpio);
            ledOff(pledphy->ledfailGpio);
			//g_ledctrllist[i].phyled->ledState = kLedStateVerySlowBlinkContiunes;
            //printk("\n");
        }
        else
            continue;
    }
    
    pledphy = g_ledctrllist[0].phyled;

    if(NULL != pledphy)
    {       
    	printk("power led online \n");
        ledOn(pledphy->ledGpio);
    }
#endif

#if 0
    for(i = 0;i < kLedfailstart;i++)
    {
        pledphytmp = NULL;
        pledphy = g_ledctrllist[i].phyled;
        if(NULL != pledphy)
        {
            printk("led %i effect :",i);
            ssleep(5);
            ledOn(pledphy->ledGpio);
            ssleep(5);
            ledOn(pledphy->ledfailGpio);
            printk("\n");
        }
        else
            continue;
    }
            

    ssleep(10);
    for(i = 0;i < kLedfailstart;i++)
    {
        pledphytmp = NULL;
        pledphy = g_ledctrllist[i].phyled;
        if(NULL != pledphy)
        {
            printk("led %i effect :",i);
            ledOff(pledphy->ledGpio);
            ledOff(pledphy->ledfailGpio);
            printk("\n");
        }
        else
            continue;
    }
#if 0
    printk("LEDCTRL TEST\n");
    for(i = 0;i < kLedfailstart;i++)
    {
        if(NULL != g_ledctrllist[i].phyled)
        {
        printk("led %i: \n",i);
        ssleep(5);
        LedCtrl(i,kLedStateFail);
        ssleep(5);
        LedCtrl(i,kLedStateOff);
        ssleep(5);
        LedCtrl(i,kLedStateOn);
        ssleep(5);
        LedCtrl(i,kLedStateOff);
        }
    }
#endif
#endif
#endif
#if 0
    for(l = 0;l < 23;l++)
    {
        gpiotest = (l|BP_GPIO_SERIAL|BP_ACTIVE_LOW);
        ledOff(gpiotest);
    }

    for(l = 0;l < 23;l++)
    {
        printk("led SERIAL %i\n",l);
        gpiotest = (l|BP_GPIO_SERIAL|BP_ACTIVE_LOW);
        ledOn(gpiotest);
        ssleep(5);
    }
#endif
    printk("\ntime start\n");
    ledStatusAfterInit();

    g_ledInitialized = 1;
    ledRunStart();
    ledTimerStart();
    
    /*End Modify For HG253 WIFI USB LED By lKF33113 2011-07-08*/
}

void kerSysLedCtrl(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState)
{
    if (g_ledInitialized)
    {
        if(LED_CONTROL_ENABLE == g_ledCtlmode)
        {
        	//boardLedCtrl(ledName, ledState);
        	LedCtrl(ledName, ledState);
        }        
    }
}

// This function is called for an LED that is controlled by hardware.
void kerSysLedRegisterHwHandler( BOARD_LED_NAME ledName, HANDLE_LED_FUNC ledHwFunc )
{
    //if( ledName < kLedEnd )
    //    gLedCtrl[ledName].ledHwFunc = ledHwFunc;
}

void ATP_LED_InitLeds(void)
{
    boardLedInit();
}

void ATP_LED_CtrlLed(BOARD_LED_NAME enLedName, BOARD_LED_STATE enLedState)
{
    LedCtrl(enLedName, enLedState);
}

void ATP_LED_AllLed(unsigned int state ,unsigned int color)
{
    //kerSysLedCtrl(enLedName, enLedState);
    LedALLset( state ,color);
}


void ATP_LED_ShutDownLed(void)
{
    BOARD_LED_NAME enLedName = kLedAdsl;
	
    for(enLedName = kLedAdsl; enLedName < kLedEnd; enLedName++)
    {
        kerSysLedCtrl(enLedName, kLedStateOff);
    }
}

void ledStateRun(void)
{
    int i = 0;
    static unsigned long tmpUsbLasRx = 0;
    static int iCuState = 0;
    g_LedRunOn = FALSE;
    struct net_device *pstDev = NULL;
    struct net_device_stats *pstDevStats = NULL;
    /*START ADD: l00252599 FOR 适配3.4.11内核的接口函数用于点灯 2013-10-10*/	
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))    
    struct rtnl_link_stats64 stStateTemp;
#endif
    /*END ADD: l00252599 FOR 适配3.4.11内核的接口函数用于点灯 2013-10-10*/
     
	int WanNum = 0;
#ifdef SUPPORT_ATP_WLAN
	int WlanNum = 0;
    bool  bIsWlanHaveTracffic = 0;
#endif
	bool  bIsHaveTracffic = 0;

	if(BHAL_LED_MODE_NORMAL == g_ulBhalLedMode)
	{
	    for ( i = 0; i < WAN_MAX_NUM; i++)
	    {
	    	if ('\0' != g_stWanDevList[i].acDevName[0])
	    	{
	    	  //printk("find WanDev:%s\n",g_stWanDevList[i].acDevName);
	    	  WanNum++;	
	    	}        	
	    }
	    /*Internet  LED闪烁 */
	    if ( WanNum > 0)  /*绑定有INTERNET服务的wan连接建立*/
	    {
	        for (i = 0; i < WAN_MAX_NUM; i++)
	    	{
	    		if ('\0' != g_stWanDevList[i].acDevName[0])
	    		{
	    			pstDev = dev_get_by_name(&init_net, g_stWanDevList[i].acDevName);
	    			if (NULL == pstDev)
	    			{
	    				break;   
	    			} 
                    /*START ADD: l00252599 FOR 适配3.4.11内核的接口函数用于点灯 2013-10-10*/				
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,11))
                    pstDevStats = dev_get_stats(pstDev, &stStateTemp);
#else
	                pstDevStats = dev_get_stats(pstDev);
#endif					
                    /*END ADD: l00252599 FOR 适配3.4.11内核的接口函数用于点灯 2013-10-10*/					
	                /*有数据接收*/
	                if (pstDevStats->rx_packets != g_stWanDevList[i].rx_packets)  
	    			{
	    				g_stWanDevList[i].rx_packets = pstDevStats->rx_packets;
	    				bIsHaveTracffic = 1;        
	                            dev_put(pstDev);
	    				break;
	    			}
	                /*有数据发送*/
	                else if (pstDevStats->tx_packets != g_stWanDevList[i].tx_packets)   
	    			{
	    				g_stWanDevList[i].tx_packets = pstDevStats->tx_packets;
	    				bIsHaveTracffic = 1;       
	                            dev_put(pstDev);
	    				break;
	    			}
	                else 
	                {
	                    bIsHaveTracffic = 0;
	                }    
	                dev_put(pstDev);
	    		}
	    	}
	    	  
			if (bIsHaveTracffic)
			{   
				/*有数据传输，则绿灯2HZ闪烁*/
				ATP_LED_CtrlLed(kLedInternet, kLedStateOff);
				ATP_LED_CtrlLed(kLedInternet, kLedStateFastBlinkContinues); 
			}
			else 
			{
				/*无数据传输，则绿灯常亮*/
				ATP_LED_CtrlLed(kLedInternet, kLedStateOff);
				ATP_LED_CtrlLed(kLedInternet, kLedStateOn);
			}         
	    }


	}

    ledRunStart();
}

void ledRunStart(void)
{
    if (g_LedRunOn)
    {
        return;
    }

    init_timer(&g_LedRunTimer);
    g_LedRunTimer.function = (void*)ledStateRun;
    //g_LedRunTimer.expires = jiffies + (TIMER_INTERVAL_125MS * 4);        // timer expires in ~100ms
    
    g_LedRunTimer.expires = jiffies + k125ms*4;        // timer expires in ~100ms
    add_timer (&g_LedRunTimer);

    g_LedRunOn = TRUE;
}


void ledEthernetStatInit(void)
{
	int i;
    
	for (i = 0; i < BHAL_LAN_MAX_NUM; i++)
      {
		g_stLanStatList[i].iPortNum    = i;
		g_stLanStatList[i].last_rx     = 0;
		g_stLanStatList[i].trans_start = 0;
		g_stLanStatList[i].next        = NULL;
	}
}

#ifdef SUPPORT_ATP_WLAN
void ledWlanStatInit(void)
{
    int i = 0;

    for (i = 0; i < WLAN_2G_SSID_NUM; i++)
    {
        memset(&g_stWlanDevList[i], 0, sizeof(g_stWlanDevList[i]));
    }
}
#endif

void ledWANStatInit(void)
{
    int i = 0;

    for (i = 0; i < WAN_MAX_NUM; i++)
    {
        memset(&g_stWanDevList[i], 0, sizeof(g_stWanDevList[i]));
    }
}

void ledStatusAfterInit(void)
{
    return;
}

#ifdef LED_SEQUENCE_BLINK
#define MAX_LED_ORDER_STR 100
int boardParseLedOrder(char *cLedOrder, int *iLedMap)
{
    int  iLedNums = 0;
    char *cTmpLed = NULL;
	
    if (NULL == cLedOrder)
    {
        return -1;
    }
    
    while(NULL != (cTmpLed = strsep(&cLedOrder, ",")))
    {
        iLedMap[iLedNums] = simple_strtoul(cTmpLed, (char **)NULL, 10);  
        iLedNums++;
    }

    if (LED_SEQUENCE_BLINK_NUMS != iLedNums)
    {
        return -1;
    }

    return 1;
}

void boardSequenceBlinkLed(unsigned int iTime)
{
    int  i;
    int  iLedMap[LED_SEQUENCE_BLINK_NUMS] = {-1};
    char cLedOrder[MAX_LED_ORDER_STR] = LED_SEQUENCE_BLINK_ORDER;

    if (-1 == boardParseLedOrder(cLedOrder, iLedMap))
    {
         printk("boardSequenceBlinkLed: Parse LED Order Error!\r\n");
         return;
    }
    
    /* 先灭掉9个灯 */
    for (i = 0; i < LED_SEQUENCE_BLINK_NUMS; i++)
    {
    	if (kLedService3 == iLedMap[i])
        {
            continue;
        }
		/* Start 关闭LTE信号灯 DTS2013111909938*/
		if (kLedOnlineLteAll == iLedMap[i])
		{
            LedCtrl(kLedOnlineLte1, kLedStateOff);
            LedCtrl(kLedOnlineLte2, kLedStateOff);
            LedCtrl(kLedOnlineLte3, kLedStateOff);
            LedCtrl(kLedOnlineLte4, kLedStateOff);
            LedCtrl(kLedOnlineLte5, kLedStateOff);
		}
		/* 关闭Warning灯 DTS2013111909938*/
		else if (kLedServiceAll == iLedMap[i])
        {
            LedCtrl(kLedService1, kLedStateOff);                
            LedCtrl(kLedService2, kLedStateOff);                
        }
		else
		{
        	LedCtrl(iLedMap[i], kLedStateOff);
        }
		/* End 关闭LTE信号灯 DTS2013111909938*/		
    }
		
    /* 依次闪烁一次 */
    for (i = 0; i < LED_SEQUENCE_BLINK_NUMS; i++)
    {
        if ((kLedPower <= iLedMap[i]) && (kLedEnd > iLedMap[i]))
        {
			if (kLedPower == iLedMap[i])
            {
            	msleep(iTime);
                continue;
            }
			
            if (kLedService3 == iLedMap[i])
            {
                continue;
            }

			/*Start LTE信号灯闪烁 DTS2013111909938*/
			if (kLedOnlineLteAll == iLedMap[i])
			{
	            LedCtrl(kLedOnlineLte1, kLedStateOn);
	            LedCtrl(kLedOnlineLte2, kLedStateOn);
	            LedCtrl(kLedOnlineLte3, kLedStateOn);
	            LedCtrl(kLedOnlineLte4, kLedStateOn);
	            LedCtrl(kLedOnlineLte5, kLedStateOn);				
                msleep(iTime);
	            LedCtrl(kLedOnlineLte1, kLedStateOff);
	            LedCtrl(kLedOnlineLte2, kLedStateOff);
	            LedCtrl(kLedOnlineLte3, kLedStateOff);
	            LedCtrl(kLedOnlineLte4, kLedStateOff);
	            LedCtrl(kLedOnlineLte5, kLedStateOff);
                msleep(iTime);
				continue;
			}
			/* Warning灯闪烁 DTS2013111909938*/
			else if (kLedServiceAll == iLedMap[i])
            {
                LedCtrl(kLedService1, kLedStateOn);                
                LedCtrl(kLedService2, kLedStateOn);    
                msleep(iTime);
                LedCtrl(kLedService1, kLedStateOff);                
                LedCtrl(kLedService2, kLedStateOff);                
                msleep(iTime);
            }
			else
			{
	            LedCtrl(iLedMap[i], kLedStateOn);
	            msleep(iTime);
	            LedCtrl(iLedMap[i], kLedStateOff);
	            msleep(iTime);    
			}
		/* End 关闭LTE信号灯 DTS2013111909938*/				
        }
    }

    /* 最后点亮电源灯 */
    LedCtrl(kLedPower, kLedStateOn);

	do_exit(0);
}

void boardPwrOnSequenceBlinkLed(unsigned int iTime)
{
    int  i;
    int  iLedMap[LED_SEQUENCE_BLINK_NUMS] = {-1};
    char cLedOrder[MAX_LED_ORDER_STR] = LED_SEQUENCE_BLINK_ORDER;

    if (-1 == boardParseLedOrder(cLedOrder, iLedMap))
    {
         printk("boardPwrOnSequenceBlinkLed: Parse LED Order Error!\r\n");
         return;
    }
    
    /* 先灭掉9个灯 */
    for (i = 0; i < LED_SEQUENCE_BLINK_NUMS; i++)
    {		
    	if (kLedService3 == iLedMap[i])
        {
            continue;
        }
		
        LedCtrl(iLedMap[i], kLedStateOff);
    }
		
    /* 依次闪烁一次 */
    for (i = 0; i < LED_SEQUENCE_BLINK_NUMS; i++)
    {
        if ((kLedPower <= iLedMap[i]) && (kLedEnd > iLedMap[i]))
        {
			if (kLedPower == iLedMap[i])
            {
            	msleep(iTime);
                continue;
            }
			
            if (kLedService3 == iLedMap[i])
            {
                continue;
            }
            /*START ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
            if (kLedOnlineLteAll == iLedMap[i])
            {
                LedCtrl(kLedOnlineLte1, kLedStateOn);
                LedCtrl(kLedOnlineLte2, kLedStateOn);
                LedCtrl(kLedOnlineLte3, kLedStateOn);
                LedCtrl(kLedOnlineLte4, kLedStateOn);
                LedCtrl(kLedOnlineLte5, kLedStateOn);
                msleep(iTime);
                LedCtrl(kLedOnlineLte1, kLedStateOff);
                LedCtrl(kLedOnlineLte2, kLedStateOff);
                LedCtrl(kLedOnlineLte3, kLedStateOff);
                LedCtrl(kLedOnlineLte4, kLedStateOff);
                LedCtrl(kLedOnlineLte5, kLedStateOff);
                msleep(iTime);
            }
            else if (kLedServiceAll == iLedMap[i])
            {
                LedCtrl(kLedService1, kLedStateOn);                
                LedCtrl(kLedService2, kLedStateOn);    
                msleep(iTime);
                LedCtrl(kLedService1, kLedStateOff);                
                LedCtrl(kLedService2, kLedStateOff);                
                msleep(iTime);
            }
            else
            {
                LedCtrl(iLedMap[i], kLedStateOn);
                msleep(iTime);
                LedCtrl(iLedMap[i], kLedStateOff);
                msleep(iTime);    
            }
            /*END ADD: l00252599 FOR ADAPT I2C LED AND KEY 2013-09-12*/
        }
    }

    /* 最后点亮电源灯 */
    LedCtrl(kLedPower, kLedStateOn);
}
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
void SendLedBlinkTimes(unsigned int sTmpLedTimes)
{
    g_SerLedRemainBlinkTimes = sTmpLedTimes;
	g_BwnLedRemainBlinkTimes = sTmpLedTimes;
}

void GetLedBlinkTimes(unsigned int *pLedRemainBlinkTimes)
{
    *pLedRemainBlinkTimes = g_SerLedRemainBlinkTimes;
}

int GetLedState(BOARD_LED_NAME ledName)
{
	PLED_PHY_PRO ledInfoPtr;

    if (!g_ledInitialized)
    {
        return -1;
    }

	// do the mapping from virtual to physical led
	if (NULL != g_ledctrllist[(int)ledName].phyled)
	{
        ledInfoPtr = g_ledctrllist[(int)ledName].phyled;
	}
    else
    {
        ledInfoPtr = NULL;
    }

    if (NULL != ledInfoPtr)
    {
        return ledInfoPtr->ledState;
    }
    else
    {
        /* 如果当前未获取到LED的状态，统一返回off */
        return kLedStateOff;
    }
}
#endif

EXPORT_SYMBOL(kerSysLedCtrl);
EXPORT_SYMBOL(BpGetEthernetMacInfo);
EXPORT_SYMBOL(BpGetRj11InnerOuterPairGpios);
EXPORT_SYMBOL(BpGetDslPhyAfeIds);

