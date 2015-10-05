/*-------------------------------------------------------------------------
Led.h

-------------------------------------------------------------------------*/

/*
1: LED ON  --function,member
2: LED OFF --function,member
3: LED Hardware blink--function,member
4 LED frequence change--function, member
5 LED state--variable, member
6 LED frequence--variable,member
7 LED blink last time--variable,member
8 GPIO--variable,member
*/
#ifndef _BOARD_LED_H
#define _BOARD_LED_H

#define TRUE 1
#define FALSE 0
#define BOOL int
#define LED_DRV_MAJOR          151

#define NET_DEV_NAME_LEN       32
#define MAX_INET_DEV           16


#ifdef DEBUG
#define LED_DEBUG(format,args...)  printk("\n=== led  debug === [%s][%d]:"format"\n",__FUNCTION__,__LINE__,## args)
#define LED_OUTPUT(format,args...) printk("\n=== led output === [%s][%d]:"format"\n",__FUNCTION__,__LINE__,## args)
#else
#define LED_DEBUG(format,args...)
#define LED_OUTPUT(format,args...)
#endif

/* 单板上最多支持32个LED */
#define MAX_BOARD_LED 32

typedef enum
{
    LED_STATE_OFF,                        /* turn led off */
    LED_STATE_ON,                         /* turn led on */
    LED_STATE_BLINK,                      /* turn led blink */
    LED_STATE_BLINKONCE,             /* turn led blink once */
    LED_STATE_FAIL,                       /* turn led fail*/
    LED_STATE_END,
} LED_STATE_EN;


typedef enum
{
    GPIO_NOT,
    GPIO_0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15,
    GPIO_16,
    GPIO_17,
    GPIO_18,
    GPIO_19,
    GPIO_20,
    GPIO_21,
    GPIO_22,
    GPIO_23,
    GPIO_24,
    GPIO_25,
    GPIO_26,
    GPIO_27,
    GPIO_28,
    GPIO_29,
    GPIO_30,
    GPIO_31,
    GPIO_32,
    GPIO_33,
    GPIO_34,
    GPIO_35,
    GPIO_36,
    GPIO_37,
    GPIO_38,
    GPIO_39,
/* Don't modify the value above*/
    GPIO_41=41,
    GPIO_42=42,
    GPIO_43=43,
    GPIO_44=44,
    GPIO_USRDEF,
    GPIO_USRDEFUSB,
    GPIO_END,
} LED_GPIO_EN;

typedef enum
{
    GPIO_ACT_LOW,
    GPIO_ACT_HIGH,
    GPIO_ACT_END,
} LED_GPIO_ACT_EN;

typedef enum
{
    GPIO_OUTPUT = 0,
    GPIO_INPUT,
} LED_GPIO_DIR_EN;


typedef enum
{
    FREQ_START=0,
	FREQ_8HZ=1,
    FREQ_4HZ=2,
    FREQ_2HZ=4,
    FREQ_1HZ=8,
    FREQ_DOT5HZ=16,
    FREQ_DOT25HZ=32,
    FREQ_DOT125HZ=64,
    FREQ_END,
} LED_FREQ_EN;

typedef enum
{
    LED_NAME_NULL,
    LED_NAME_POWER,
    LED_NAME_POWER_GREEN,
    LED_NAME_POWER_RED,
    LED_NAME_ADSL_ACT,
    LED_NAME_ADSL_LINK,
    LED_NAME_WIFI,
    LED_NAME_WPS,
    LED_NAME_INET,
    LED_NAME_INET_FAIL,
    LED_NAME_RUN,
    LED_NAME_USB,
    LED_NAME_LAN1,
    LED_NAME_LAN2,
    LED_NAME_LAN3,
    LED_NAME_LAN4,
    LED_NAME_WAN,
    LED_NAME_BATTERY,
    LED_NAME_MAINTAIN,
    LED_NAME_LAN1_ACT,
    LED_NAME_LAN2_ACT,
    LED_NAME_LAN3_ACT,
    LED_NAME_LAN4_ACT,
    LED_NAME_LAN1_LINK,
    LED_NAME_LAN2_LINK,
    LED_NAME_LAN3_LINK,
    LED_NAME_LAN4_LINK,
    LED_NAME_WIFI_AP,
    LED_NAME_WIFI_STA,
    LED_NAME_WIFI_REPEAT,
    LED_NAME_LAN_4IN1,
    LED_NAME_END,
}LED_NAME;


typedef unsigned int TIMER_DIDA;
typedef void (*HANDLE_LED_CTL_FUNC)(LED_STATE_EN enLedState,LED_FREQ_EN enFreq);

typedef struct
{
    LED_NAME         enLedName;

    LED_GPIO_EN      enLedGPIO;     /*Led control GPIO, if not control by gpio, define as GPIO_NOT*/
    LED_GPIO_ACT_EN  enGpioAct;     /*Low or High to light LED*/

    LED_STATE_EN     enLedState;
    LED_STATE_EN     enCurState;

    LED_FREQ_EN      enFreq;        /*Blink frequence*/
    TIMER_DIDA       unOnLstIntvl;       /* 定义占空比，也使用频率的方式*/
    TIMER_DIDA       unTdd;         /*Timer trigger count*/

    /*If the led is not controled by gpio,
    below functions need to be register,
    otherwise are not used*/
    HANDLE_LED_CTL_FUNC  fpHwLedCtl;
}LED_INFO_ST;

#ifndef __KERNEL__
struct list_head {
	struct list_head *next, *prev;
};
#endif

/****************************************************************************
 * 提供LED根据网络接口的流量来进行闪烁的接口.
 * 注册LED时最多可以注册2个LED,即success LED  和 fail LED.
 * 不需要 fail LED时可以使用 LED_NAME_NULL来注册.
 * 可以支持监控多条网络接口流量来控制已注册的LED.
 * 
 * LED注册时, 新建 LED_NDEV_ST 结构体挂到全局链表中, 
 * 并新建LED_NDEV_PARAS_ST结构体挂载到 LED_NDEV_ST 的设备接口链表中.
 * 在 led process 函数中对所有LED对应的网络接口进行监控.
 *
 * 示意图如下:
 *
 * 全局链表头---------> LED1 struct --------> LED2 struct -------->全局链表尾
 *                     |
 *                     |-> led name
 *                     |-> led name fail
 *                     |
 *                     |-> led dev list head ---> dev1 st ---> dev2 st 
 *                                                 |
 *                                                 |-> net device name1
 *
 ***************************************************************************/
typedef struct
{
    struct list_head list;
    char acNetDev[NET_DEV_NAME_LEN];
    LED_STATE_EN  enNDevLedState;
    unsigned long last_rx;
    unsigned long trans_start;
}LED_NDEV_PARAS_ST, *PLED_NDEV_PARAS_ST;

typedef struct
{
    struct list_head list;
    LED_NDEV_PARAS_ST stLedDev;
    LED_NAME enLedName;
    LED_NAME enLedNameFail;
    LED_STATE_EN enLedCurState;
    LED_STATE_EN enLedOldState;
}LED_NDEV_ST, *PLED_NDEV_ST;

//提供给应用层API的结构接口,用来添加,删除,修改状态等.
typedef struct
{
    char acNetDev[NET_DEV_NAME_LEN];
    LED_NAME      enLedName;
    LED_NAME      enLedNameFail;
    LED_STATE_EN  enNetState;
}LED_NETDEV_ST;

#define BOARDLED_IOCTL_ADD \
    _IOWR(LED_DRV_MAJOR, 0, LED_INFO_ST)
#define BOARDLED_IOCTL_DEL \
    _IOWR(LED_DRV_MAJOR, 1, LED_INFO_ST)
#define BOARDLED_IOCTL_STATCHG \
    _IOWR(LED_DRV_MAJOR, 2, LED_INFO_ST)
#define BOARDLED_IOCTL_FREQCHG \
    _IOWR(LED_DRV_MAJOR, 3, LED_INFO_ST)
#define BOARDLED_IOCTL_NETLED_ADD \
    _IOWR(LED_DRV_MAJOR, 4, LED_NETDEV_ST)
#define BOARDLED_IOCTL_NETLED_DEL \
    _IOWR(LED_DRV_MAJOR, 5, LED_NETDEV_ST)
#define BOARDLED_IOCTL_NETLED_STATCHG \
    _IOWR(LED_DRV_MAJOR, 6, LED_NETDEV_ST)
#define BOARDLED_IOCTL_LED_ENABLE \
    _IOWR(LED_DRV_MAJOR, 7, LED_INFO_ST)
#define BOARDLED_IOCTL_LED_DISABLE \
    _IOWR(LED_DRV_MAJOR, 8, LED_INFO_ST)
#define BOARDLED_IOCTL_LED_ALL_ACTIVE \
    _IOWR(LED_DRV_MAJOR, 9, LED_INFO_ST)
/*start of modify by j00127542 20091015 for A36D08340*/
#define BOARDLED_IOCTL_LED_EQUIPMENT \
    _IOWR(LED_DRV_MAJOR, 10, LED_INFO_ST)
/*end of modify by j00127542 20091015 for A36D08340*/

int led_register(LED_INFO_ST stLedInfo);
int led_unregister(LED_NAME enName);

void led_state_change(LED_NAME enName,LED_STATE_EN enState);
void led_freq_change(LED_NAME enName,LED_FREQ_EN enFreq,LED_FREQ_EN enOnLst);
#endif


