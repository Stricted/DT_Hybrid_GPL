/*
<:copyright-BRCM:2002:GPL/GPL:standard

   Copyright (c) 2002 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  board.h                                                  */
/*   PURPOSE: Board specific information.  This module should include  */
/*            all base device addresses and board specific macros.     */
/*                                                                     */
/***********************************************************************/
#ifndef _BOARD_H
#define _BOARD_H

#include "bcm_hwdefs.h"
#include <boardparms.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BOARD_H_API_VER increases when other modules (such as PHY) depend on */
/* a new function in the board driver or in boardparms.h                */

#define BOARD_H_API_VER 5

/*****************************************************************************/
/*          board ioctl calls for flash, led and some other utilities        */
/*****************************************************************************/


/* Defines. for board driver */
#define BOARD_IOCTL_MAGIC       'B'
#define BOARD_DRV_MAJOR          206
#define NOT_SUPPROT_GET_VERSION  0xFF

#define MAC_ADDRESS_ANY         (unsigned long) -1

#define BOARD_IOCTL_FLASH_INIT \
    _IOWR(BOARD_IOCTL_MAGIC, 0, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_FLASH_WRITE \
    _IOWR(BOARD_IOCTL_MAGIC, 1, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_FLASH_READ \
    _IOWR(BOARD_IOCTL_MAGIC, 2, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_NR_PAGES \
    _IOWR(BOARD_IOCTL_MAGIC, 3, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_DUMP_ADDR \
    _IOWR(BOARD_IOCTL_MAGIC, 4, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_SET_MEMORY \
    _IOWR(BOARD_IOCTL_MAGIC, 5, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_MIPS_SOFT_RESET \
    _IOWR(BOARD_IOCTL_MAGIC, 6, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_LED_CTRL \
    _IOWR(BOARD_IOCTL_MAGIC, 7, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_ID \
    _IOWR(BOARD_IOCTL_MAGIC, 8, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_MAC_ADDRESS \
    _IOWR(BOARD_IOCTL_MAGIC, 9, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_RELEASE_MAC_ADDRESS \
    _IOWR(BOARD_IOCTL_MAGIC, 10, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_PSI_SIZE \
    _IOWR(BOARD_IOCTL_MAGIC, 11, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_SDRAM_SIZE \
    _IOWR(BOARD_IOCTL_MAGIC, 12, BOARD_IOCTL_PARMS)

/* 2008/01/28 Jiajun Weng : Modify its definition for WPS PBC trigger */

#define BOARD_IOCTL_SET_MONITOR_FD \
    _IOWR(BOARD_IOCTL_MAGIC, 17, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_WAKEUP_MONITOR_TASK \
    _IOWR(BOARD_IOCTL_MAGIC, 18, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_BOOTLINE \
    _IOWR(BOARD_IOCTL_MAGIC, 15, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_SET_BOOTLINE \
    _IOWR(BOARD_IOCTL_MAGIC, 16, BOARD_IOCTL_PARMS)
/* 2008/01/28 Jiajun Weng : Modify its definition for WPS PBC trigger */


#define BOARD_IOCTL_GET_BASE_MAC_ADDRESS \
    _IOWR(BOARD_IOCTL_MAGIC, 13, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_CHIP_ID \
    _IOWR(BOARD_IOCTL_MAGIC, 14, BOARD_IOCTL_PARMS)

/* 2008/01/28 Jiajun Weng : Modify its definition for WPS PBC trigger */
#define BOARD_IOCTL_GET_NUM_ENET \
    _IOWR(BOARD_IOCTL_MAGIC, 23, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_CFE_VER \
    _IOWR(BOARD_IOCTL_MAGIC, 24, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_ENET_CFG \
    _IOWR(BOARD_IOCTL_MAGIC, 25, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_GET_WLAN_ANT_INUSE \
    _IOWR(BOARD_IOCTL_MAGIC, 22, BOARD_IOCTL_PARMS)

/* 2008/01/28 Jiajun Weng : Modify its definition for WPS PBC trigger */
#define BOARD_IOCTL_SET_TRIGGER_EVENT \
    _IOWR(BOARD_IOCTL_MAGIC, 19, BOARD_IOCTL_PARMS)        

#define BOARD_IOCTL_GET_TRIGGER_EVENT \
    _IOWR(BOARD_IOCTL_MAGIC, 20, BOARD_IOCTL_PARMS)        

#define BOARD_IOCTL_UNSET_TRIGGER_EVENT \
    _IOWR(BOARD_IOCTL_MAGIC, 21, BOARD_IOCTL_PARMS) 

#define BOARD_IOCTL_SET_SES_LED \
    _IOWR(BOARD_IOCTL_MAGIC, 26, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_GET_VCOPE_GPIO \
    _IOWR(BOARD_IOCTL_MAGIC, 27, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_SET_CS_PAR \
    _IOWR(BOARD_IOCTL_MAGIC, 28, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_SET_PLL \
    _IOWR(BOARD_IOCTL_MAGIC, 29, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_SET_GPIO \
    _IOWR(BOARD_IOCTL_MAGIC, 30, BOARD_IOCTL_PARMS)
  
#define BOARD_IOCTL_GET_DYINGGASP \
    _IOWR(BOARD_IOCTL_MAGIC, 39, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_PUT_WEB_OP \
    _IOWR(BOARD_IOCTL_MAGIC, 31, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_PUT_CLI_OP \
    _IOWR(BOARD_IOCTL_MAGIC, 32, BOARD_IOCTL_PARMS)
    
#define BOARD_IOCTL_PUT_ITF_STATE \
    _IOWR(BOARD_IOCTL_MAGIC, 33, BOARD_IOCTL_PARMS)
    
#define  BOARD_IOCTL_EQUIPMENT_TEST \
    _IOWR(BOARD_IOCTL_MAGIC, 34, BOARD_IOCTL_PARMS)     

#define BOARD_IOCTL_CHECK_WLAN_STATUS \
    _IOWR(BOARD_IOCTL_MAGIC, 35, BOARD_IOCTL_PARMS)

#define BOARD_IOCTL_SET_VOIP_SERVICE \
    _IOWR(BOARD_IOCTL_MAGIC, 36, BOARD_IOCTL_PARMS)          

#define BOARD_IOCTL_GET_VOIP_SERVICE \
    _IOWR(BOARD_IOCTL_MAGIC, 37, BOARD_IOCTL_PARMS)          

/* HUAWEI HGW s48571 2008年1月19日 Hardware Porting add begin:*/
#define BOARD_IOCTL_SET_TRAFFIC_MODE \
        _IOWR(BOARD_IOCTL_MAGIC, 38, BOARD_IOCTL_PARMS)  

/* HUAWEI HGW s48571 2008年1月19日 Hardware Porting add end.*/

/* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add begin:*/
#define BOARD_IOCTL_SET_EQUIPTEST_MODE \
        _IOWR(BOARD_IOCTL_MAGIC, 40, BOARD_IOCTL_PARMS)  
/* HUAWEI HGW s48571 2008年2月20日 装备测试状态标志添加 add end.*/

// for the action in BOARD_IOCTL_PARMS for flash operation
typedef enum 
{
    PERSISTENT,
    NVRAM,
    BCM_IMAGE_CFE,
    BCM_IMAGE_FS,
    BCM_IMAGE_KERNEL,
    BCM_IMAGE_WHOLE,
    SCRATCH_PAD,
    FLASH_SIZE,
    SET_CS_PARAM,
    GET_FILE_TAG_FROM_FLASH,
    VARIABLE,  
} BOARD_IOCTL_ACTION;

/* start of BOARD update by y42304 20060623: 与装备测试有关的动作 */
typedef enum 
{
    SET_BASE_MAC_ADDRESS,
    GET_BASE_MAC_ADDRESS,
    SET_MAC_AMOUNT,
    GET_MAC_AMOUNT,
    GET_ALL_BOARD_ID_NAME,
    GET_PCB_VERSION,
    GET_BOARD_VERSION,
    SET_BOARD_ID,
    GET_BOARD_ID,
    SET_SERIAL_NUMBER,
    GET_SERIAL_NUMBER,
    GET_CPU_REVISION_ID,
    GET_BOARD_MANUFACTURER_NAME,
    BUTT,    
} BOARD_EQUIPMENT_TEST_ACTION;
/* end of equip test added by y42304 20060623*/
    
    
typedef struct boardIoctParms
{
    char *string;
    char *buf;
    int strLen;
    int offset;
    BOARD_IOCTL_ACTION  action;        /* flash read/write: nvram, persistent, bcm image */
    int result;
} BOARD_IOCTL_PARMS;

// LED defines 
typedef enum
{   
    kLedPower, //0
    kLedAdsl,// 1
    kLedAdsl2, // 2
    kLedInternet, // 3
    kLedLan1,// 4
    kLedLan2, // 5
    kLedLan3,// 6
    kLedLan4,// 7
    kLedLan5, // 8
    kLedWan,// 9
    kLedHpna,// 10
    kLedHspa,// 11
    kLedPpp,// 12
    kLedWlan,// 13
    kLedWlanEable,// 14
    kLedWlan5G,// 15
    kLedWlan5GEable,// 16
    kLedWps,// 17
    kLedUsb,// 18
    kLedBlPower,// 19
    kLedBlResetCfg,  // 20  
    kLedBlReStop,// 21
    kLedVoip1,// 22
    kLedVoip2,// 23
    kLedService1,// 24
    kLedService2,// 25
    kLedService3,// 26
    kLedService4,// 27
    kLedDect,// 28
    kLedPots,// 29
    kLedsesWlan,// 30
    kLedLinkLte,// 31   // START ADD: l00252599 FOR LTE LED
    kLedOnlineLte1,//32
    kLedOnlineLte2,//33
    kLedOnlineLte3,//34
    kLedOnlineLte4,//35
    kLedOnlineLte5,//36
    kLedOnlineLteAll,//37  // START ADD: l00252599 FOR SETUP LED
    kLedServiceAll,//38
    kLedfailstart,
    kLedPowerfail,
    kLedAdslfail,
    kLedAdsl2fail,
    kLedInternetfail,
    kLedLan1fail,
    kLedLan2fail,
    kLedLan3fail,
    kLedLan4fail,
    kLedLan5fail,
    kLedWanfail,
    kLedHpnafail,
    kLedHspafail,
    kLedPppfail,
    kLedWlanfail,
    kLedWlanEablefail,
    kLedWlan5Gfail,
    kLedWlan5GEablefail,
    kLedWpsfail,
    kLedUsbfail,
    kLedBlPowerfail,
    kLedBlResetCfgfail,    
    kLedBlReStopfail,
    kLedVoip1fail,
    kLedVoip2fail,
    kLedService1fail,
    kLedService2fail,
    kLedService3fail,
    kLedService4fail,
    kLedDectfail,// 26
    kLedPotsfail,// 27
    kLedsesWlanfail,// 28
    kLedEnd,       // NOTE: Insert the new led name before this one.
} BOARD_LED_NAME;

typedef enum
{
    kLedStateOff,                        /* turn led off */
    kLedStateOn,                         /* turn led on */
    kLedStateFail,                       /* turn led on red */
    kLedStateBlinkOnce,
    kLedStateSlowFlickerOnce,            /* Slow Flicker once, VDF customized and ignore the same call during blink period */
    kLedStateFastFlickerOnce,            /* Fast Flicker once, VDF customized and ignore the same call during blink period */
    kLedStateVerySlowBlinkContiunes,     /* Jiajun 12/18/2007: slow blink contines at ~500ms interval */
    kLedStateSlowBlinkContinues,         /* slow blink continues at 2HZ interval */
    kLedStateFastBlinkContinues,         /* fast blink continues at 4HZ interval */
	kLedStateCrossFastBlinkContinues,	 /* read-green fast blink continues at 4HZ interval */
	kLedStateUserWpsInProgress,          /* 2000ms on, 1000ms off */
    kLedStateUserWpsError,               /* 250ms on, 250ms off */
    kLedStateUserWpsSessionOverLap ,     /* 250ms on, 250ms off, 5 times, off for 500ms */      
    kLedStateWanOrangeandGreen,          /*add by lKF33113 2010-10-23 Orange and green 1HZ*/
    kLedStateWanOrangeandPurple,         /*add by lKF33113 2010-10-23 Orange and Purple 1HZ*/
    kLedState1HZBlinkContinues,          /* slow blink continues at 1HZ interval */
#if 0
    kLedStateUserWpsInProgress,          /* 200ms on, 100ms off */
#endif
    /*Added by lvxin00135113@20091113 for support DT key event*/   
    kLedStateBlink2HZOnce,                  /*add for support DT 6 leds sequence blink 0.5s*/
    /*Added by lvxin00135113@20091113 for support DT key event*/   
    kLedStateLighttime,
    kLedStateLightfailtime,
    kLedStateLightyellowtime,
    kLedStateServiceSlowBlinkContinues = 30,
    kLedStateBitteWartenSlowBlinkContinues,
    kLedWpsWlanStateSlowBlinkContinues,
    kLedWpsVoipStateSlowBlinkContinues,
    kLedWpsWlanVoipStateSlowBlinkContinues,
} BOARD_LED_STATE;


typedef enum
{
    kGpioInactive,
    kGpioActive
} GPIO_STATE_t;

typedef enum
{
  GPIO_LOW = 0,
  GPIO_HIGH,
} GPIO_STATE_ATP;


typedef enum
{
    GPIO_LOW32 = 0,   /* GPIO号为GPIO0-GPIO31 */
    GPIO_HIGH_BIT,    /* GPIO号为GPI32或以上 */
}GPIO_NUM;

/*start of board added by y42304 20060708: 硬件版本号*/
typedef enum 
{
    /*
     *  GPIO8 GPIO7 GPIO6
     *    0    0     0   --> HG52VAG     
     *    0    0     1   --> HG52VAGB        
     *    1    0     0   --> HG51VAG
     *    1    1     0   --> HG51VAGI
     */
    HG52VAG  = 0,   
    HG52VAGB = 1,            
    HG51VAG  = 4,
    HG51VAGI = 6,
}E_BOARD_VERSION;

typedef enum 
{
    /*
     *  GPIO8 GPIO7 GPIO6
     *    0    0     0   --> HG55magc
     *    0    0     1   --> HG55magd        
     单板名。
     HG55magc实现：BCM6358 + BCM5325 + BCM4318 + Si3210 + BCM6301
     HG55magd实现：BCM6358 + ADM6996 + BCM4318 + Si3215 + BCM6301 + Si3050(+Si3011)       
     */
    HG55MAGC  = 0,   
    HG55MAGD  = 1,           
    HG55VAGII = 0x1F,
}E_BOARD_ADSL_VERSION;
/*end of board added by y42304 20060708: 硬件版本号*/

/* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add begin:*/
typedef enum 
{
    /*
     *   GPIO7 GPIO6
     *    0     0   --> HG55MAGV
     单板名。
     HG55magc实现：BCM6358 + BCM5325 + BCM4318 + LE88221 
     后续VDF单板在这里添加
     */
    HG55MAGV  = 0,   
}E_BOARD_VDF_VERSION;

typedef enum 
{
    MODE_NONE = 0,
    MODE_2G,
    MODE_3G,
    MODE_UNDEFINED,                
}E_HSPA_TRAFFICMODE_VERSION;



/* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add end.*/



/*start of board added by y42304 20060810: 硬件制造商号 */
typedef enum 
{
    /*
     *  GPIO35 GPIO33
     *    1    1         --> Huawei
     *    1    0         --> Alpha    
     */     
    MANUFACTURER_ALPHA   = 2,    
    MANUFACTURER_HUAWEI  = 3,   
}E_BOARD_MANUFACTURER;
/*end of board added by y42304 20060810: 硬件制造商号 */

// virtual and physical map pair defined in board.c
typedef struct ledmappair
{
    BOARD_LED_NAME ledName;         // virtual led name
    BOARD_LED_STATE ledInitState;   // initial led state when the board boots.
    unsigned long long ledMask;         // physical GPIO pin mask
    unsigned short ledActiveLow;    // reset bit to turn on LED
    unsigned short ledSerial;       // indicated that LED is driven via serial output
    unsigned long long ledMaskFail;     // physical GPIO pin mask for state failure
    unsigned short ledActiveLowFail;// reset bit to turn on LED
    unsigned short ledSerialFail;   // indicated that LED is driven via serial output
    /* start of LED added by y42304 20060517: 为了解决能用GPIO32或以上点灯的问题 */    
    int ledlow32GPIO;               // 0: 表示用的GPIO0-GPIO31; 1: 表示用的是GPIO32和以上的GPIO
    /* end of LED added by y42304 20060517 */
} LED_MAP_PAIR, *PLED_MAP_PAIR;

typedef void (*HANDLE_LED_FUNC)(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState);

typedef struct ledphy{
    short ledGpio;                   // GPIO # for LED
    short ledfailGpio;               // GPIO # for Fail LED
    int blinkCountDown;            // Count for blink states
    int FlickerCount;             
    int PauseCount;
    int failshift;
    BOARD_LED_STATE ledState;        // current led state    
    BOARD_LED_STATE savedLedState;        // current led state    
    BOARD_LED_STATE nextFlickerState;
    HANDLE_LED_FUNC ledHwFunc;
    HANDLE_LED_FUNC ledHwFailFunc;
} LED_PHY_PRO, *PLED_PHY_PRO;


typedef struct ledlist{
    PLED_PHY_PRO phyled;
} LED_CTRLLIST;

#define GPIO_NOEFFECT -1


// scratch pad defines
/* SP - Persisten Scratch Pad format:
       sp header        : 32 bytes
       tokenId-1        : 8 bytes
       tokenId-1 len    : 4 bytes
       tokenId-1 data    
       ....
       tokenId-n        : 8 bytes
       tokenId-n len    : 4 bytes
       tokenId-n data    
*/

#define MAGIC_NUM_LEN       8
#define MAGIC_NUMBER        "gOGoBrCm"
#define TOKEN_NAME_LEN      16
#define SP_VERSION          1
#define SP_MAX_LEN          8 * 1024            // 8k buf before psi
#define SP_RESERVERD        20
/*start of 增加FLASH VA的分区 porting by l39225 20060504*/
#define VA_MAX_LEN        8*1024
/*end of 增加FLASH VA的分区 porting by l39225 20060504*/



typedef struct cs_config_pars_tag
{
  int   mode;
  int   base;
  int   size;
  int   wait_state;
  int   setup_time;
  int   hold_time;
} cs_config_pars_t;

typedef enum
{
    MAC_ADDRESS_OP_GET=0,
    MAC_ADDRESS_OP_RELEASE,
    MAC_ADDRESS_OP_MAX
} MAC_ADDRESS_OPERATION;

typedef enum
{
	USB_HOST_FUNC,
	USB_DEVICE_FUNC,
	USB_ALL_FUNC
} USB_FUNCTION;


typedef void (* kerSysMacAddressNotifyHook_t)(unsigned char *pucaMacAddr, MAC_ADDRESS_OPERATION op);
typedef void (* kerSysPushButtonNotifyHook_t)(void);

#define UBUS_BASE_FREQUENCY_IN_MHZ  160

#define IF_NAME_ETH    "eth"
#define IF_NAME_USB    "usb"
#define IF_NAME_WLAN   "wl"
#define IF_NAME_MOCA   "moca"
#define IF_NAME_ATM    "atm"
#define IF_NAME_PTM    "ptm"
#define IF_NAME_GPON   "gpon"
#define IF_NAME_EPON   "epon"
#define IF_NAME_VEIP    "veip"

#define MAC_ADDRESS_ANY         (unsigned long) -1

/* A unique mac id is required for a WAN interface to request for a MAC address.
 * The 32bit mac id is formated as follows:
 *     bit 28-31: MAC Address IF type (MAC_ADDRESS_*)
 *     bit 20-27: the unit number. 0 for atm0, 1 for atm1, ...
 *     bit 12-19: the connection id which starts from 1.
 *     bit 0-11:  not used. should be zero.
 */
#define MAC_ADDRESS_PLC         0x10000000
#define MAC_ADDRESS_EPONONU     0x20000000
#define MAC_ADDRESS_EPON        0x40000000
#define MAC_ADDRESS_GPON        0x80000000
#define MAC_ADDRESS_1905        0x90000000
#define MAC_ADDRESS_ETH         0xA0000000
#define MAC_ADDRESS_USB         0xB0000000
#define MAC_ADDRESS_WLAN        0xC0000000
#define MAC_ADDRESS_MOCA        0xD0000000
#define MAC_ADDRESS_ATM         0xE0000000
#define MAC_ADDRESS_PTM         0xF0000000

#define UNUSED_PARAM(a)        (a = a)

/*****************************************************************************/
/*          Function Prototypes                                              */
/*****************************************************************************/
#if !defined(__ASM_ASM_H)
void dumpaddr( unsigned char *pAddr, int nLen );

extern void kerSysEarlyFlashInit( void );
extern int kerSysGetChipId( void );
char * kerSysGetChipName( char *buf, int len);
extern int kerSysGetDslPhyEnable( void );
extern void kerSysFlashInit( void );
//extern void kerSysFlashAddrInfoGet(PFLASH_ADDR_INFO pflash_addr_info);
extern int kerSysCfeVersionGet(char *string, int stringLength);

extern int kerSysNvRamSet(const char *string, int strLen, int offset);
extern void kerSysNvRamGet(char *string, int strLen, int offset);
extern void kerSysNvRamGetBootline(char *bootline);
extern void kerSysNvRamGetBootlineLocked(char *bootline);
extern void kerSysNvRamGetBoardId(char *boardId);
extern void kerSysNvRamGetBoardIdLocked(char *boardId);
extern void kerSysNvRamGetBaseMacAddr(unsigned char *baseMacAddr);
extern unsigned long kerSysNvRamGetVersion(void);

extern int kerSysFsFileSet(const char *filename, char *buf, int len);
extern int kerSysPersistentGet(char *string, int strLen, int offset);
extern int kerSysPersistentSet(char *string, int strLen, int offset);
extern int kerSysBackupPsiGet(char *string, int strLen, int offset);
extern int kerSysBackupPsiSet(char *string, int strLen, int offset);
extern int kerSysSyslogGet(char *string, int strLen, int offset);
extern int kerSysSyslogSet(char *string, int strLen, int offset);
extern int kerSysScratchPadList(char *tokBuf, int tokLen);
extern int kerSysScratchPadGet(char *tokName, char *tokBuf, int tokLen);
extern int kerSysScratchPadSet(char *tokName, char *tokBuf, int tokLen);
extern int kerSysScratchPadClearAll(void);
extern int kerSysBcmImageSet( int flash_start_addr, char *string, int size,
    int should_yield);
extern int kerSysBcmNandImageSet( char *rootfs_part, char *string, int img_size,
    int should_yield );
extern int kerSysSetBootImageState( int state );
extern int kerSysGetBootImageState( void );
extern int kerSysSetOpticalPowerValues(unsigned short rxReading, unsigned short rxOffset, 
    unsigned short txReading);
extern int kerSysGetOpticalPowerValues(unsigned short *prxReading, unsigned short *prxOffset, 
    unsigned short *ptxReading);
extern int kerSysBlParmsGetInt( char *name, int *pvalue );
extern int kerSysBlParmsGetStr( char *name, char *pvalue, int size );
extern unsigned long kerSysGetMacAddressType( unsigned char *ifName );
extern int kerSysMacAddressNotifyBind(kerSysMacAddressNotifyHook_t hook);
extern int kerSysPushButtonNotifyBind(kerSysPushButtonNotifyHook_t hook);
extern int kerSysGetMacAddress( unsigned char *pucaAddr, unsigned long ulId );
extern int kerSysReleaseMacAddress( unsigned char *pucaAddr );
extern void kerSysGetGponSerialNumber( unsigned char *pGponSerialNumber);
extern void kerSysGetGponPassword( unsigned char *pGponPassword);
extern int kerSysGetSdramSize( void );
#if defined(CONFIG_BCM96368)
extern unsigned int kerSysGetSdramWidth( void );
#endif
extern void kerSysGetBootline(char *string, int strLen);
extern void kerSysSetBootline(char *string, int strLen);
extern void kerSysMipsSoftReset(void);
extern void kerSysLedCtrl(BOARD_LED_NAME, BOARD_LED_STATE);
extern int kerSysFlashSizeGet(void);
extern int kerSysMemoryMappedFlashSizeGet(void);
extern unsigned long kerSysReadFromFlash( void *toaddr, unsigned long fromaddr, unsigned long len );
extern int kerSysEraseFlash(unsigned long eraseaddr, unsigned long len);
extern int kerSysWriteToFlash( unsigned long toaddr, void * fromaddr, unsigned long len);
extern void kerSysRegisterDyingGaspHandler(char *devname, void *cbfn, void *context);
extern void kerSysDeregisterDyingGaspHandler(char *devname);    
extern int kerConfigCs(BOARD_IOCTL_PARMS *parms);

extern int kerSysGetPciePortEnable(int port);
extern int kerSysGetUsbHostPortEnable(int port);
extern int kerSysGetUsbDeviceEnable(void);
extern int kerSysSetPciePortPower(int on, int port);
extern int kerSysSetUsbPower(int on, USB_FUNCTION func);
extern void kerSysSetBootParm(char *name, char *value);

#if defined(WIRELESS)
void kerSysSesEventTrigger( void );
#endif

/* private functions used within board driver only */
void stopOtherCpu(void);
void resetPwrmgmtDdrMips(void);

PBP_MOCA_INFO boardGetMocaInfo(int dev);

/*
 * Access to shared GPIO registers should be done by calling these
 * functions below, which will grab a spinlock while accessing the
 * GPIO register.  However, if your code needs to access a shared
 * GPIO register but cannot call these functions, you should still
 * acquire the spinlock.
 * (Technically speaking, I should include spinlock.h and declare extern here
 * but that breaks the C++ compile of xtm.)
 */
//#include <linux/spinlock.h>
//extern spinlock_t bcm_gpio_spinlock;
extern void kerSysSetGpioState(unsigned short bpGpio, GPIO_STATE_t state);
extern void kerSysSetGpioStateLocked(unsigned short bpGpio, GPIO_STATE_t state);
extern void kerSysSetGpioDir(unsigned short bpGpio);
extern void kerSysSetGpioDirLocked(unsigned short bpGpio);
extern int kerSysSetGpioDirInput(unsigned short bpGpio);
extern int kerSysGetGpioValue(unsigned short bpGpio);


// for the voice code, which has too many kernSysSetGpio to change
#define kerSysSetGpio kerSysSetGpioState


extern unsigned long kerSysGetUbusFreq(unsigned long miscStrapBus);
extern int kerSysGetAfeId( unsigned long *afeId );
#define __kerSysGetAfeId  kerSysGetAfeId

typedef void (*MocaHostIntrCallback)(int irq, void * arg);
typedef struct
{
    int dev;
    MocaHostIntrCallback mocaCallback;
    void * userArg;
    int irq;
    int intrGpio;
} MOCA_INTR_ARG, *PMOCA_INTR_ARG;

extern void kerSysRegisterMocaHostIntrCallback(MocaHostIntrCallback callback, void * userArg, int dev);
extern void kerSysMocaHostIntrEnable(int dev);
extern void kerSysMocaHostIntrDisable(int dev);

extern unsigned int kerSysGetExtIntInfo(unsigned int irq);

#if !defined(CONFIG_BCM96816) && !defined(CONFIG_BCM96818)
/* implement in arch dependent setup.c */
extern void* kerSysGetDslPhyMemory(void);
#endif

#if defined(CONFIG_BCM_CPLD1)
int BcmCpld1Initialize(void);
void BcmCpld1CheckShutdownMode(void);
void BcmCpld1SetShutdownMode(void);
void BcmCpld1SetStandbyTimer(unsigned int duration);
#endif

#if defined (CONFIG_BCM_AVS_PWRSAVE)
extern void kerSysBcmEnableAvs(int enable);
extern int kerSysBcmAvsEnabled(void);
#endif

#if defined(CONFIG_BCM_DDR_SELF_REFRESH_PWRSAVE)
#define CONFIG_BCM_PWRMNGT_DDR_SR_API
// The structure below is to be declared in ADSL PHY MIPS LMEM, if ADSL is compiled in
typedef struct _PWRMNGT_DDR_SR_CTRL_ {
  union {
   struct {
      unsigned char   phyBusy;
      unsigned char   tp0Busy;
      unsigned char   tp1Busy;
      unsigned char   reserved;
    };
    unsigned int      word;
  };
} PWRMNGT_DDR_SR_CTRL;

void BcmPwrMngtRegisterLmemAddr(PWRMNGT_DDR_SR_CTRL *pDdrSr);
#endif

#if defined(CONFIG_BCM96838)
#define TM_BASE_ADDR_STR				"tm"
#define TM_MC_BASE_ADDR_STR				"mc"
#define TM_DEF_DDR_SIZE 				0x1400000
#define TM_MC_DEF_DDR_SIZE				0x0400000

extern void kerSysSetGpio(unsigned short bpGpio, GPIO_STATE_t state);

#endif

#endif
int BcmMemReserveGetByName(char *name, void **addr, unsigned int *size);

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H */

