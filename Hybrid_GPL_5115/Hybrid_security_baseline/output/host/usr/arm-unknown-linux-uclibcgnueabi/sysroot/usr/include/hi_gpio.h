/* <DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 BEGIN */
/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

******************************************************************************
  文 件 名   : hi_monitor_gpio.h
  版 本 号   : 初稿
  作    者   : l00204224
  生成日期   : D2012_7_12

******************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>     	 	<时间>       	 <问题单号>            		 <简要描述>
z00185914           2013/01/28          DTS2013012808057		【拨号】新建ndis拨号文件夹 
========================================================================================*/

#ifndef __HI_MONITOR_GPIO_H__
#define __HI_MONITOR_GPIO_H__

#ifndef hi_void
#define   hi_void              void
#endif

#ifndef hi_char8
#define   hi_char8             char
#endif

#ifndef hi_short16
#define   hi_short16           short
#endif

#ifndef hi_int32
#define   hi_int32             int
#endif

#ifndef hi_long64
#define   hi_long64            long long
#endif

/*********************************************************/

#ifndef hi_uchar8
#define   hi_uchar8            unsigned char
#endif

#ifndef hi_ushort16
#define   hi_ushort16          unsigned short
#endif

#ifndef hi_uint32
#define   hi_uint32            unsigned int
#endif

#ifndef hi_ulong32
#define   hi_ulong32           unsigned long
#endif

#ifndef hi_ulong64
#define   hi_ulong64           unsigned long long
#endif

#define GPIO_NUM 128 //GPIO总数


//按键事件类型枚举：
typedef enum
{
    HI_GPIO_KEY_EVENT_DOWN_E  = 1,
    HI_GPIO_KEY_EVENT_SHORT_E = 2,
    HI_GPIO_KEY_EVENT_LONG_E = 4,
    HI_GPIO_KEY_EVENT_UP_E = 8
} HI_GPIO_KEY_EVENT_E;

//点灯事件类型枚举：
typedef enum
{
    HI_GPIO_LED_EVENT_ON_E = 0,
    HI_GPIO_LED_EVENT_OFF_E,
} HI_GPIO_LED_EVENT_E;

typedef struct
{
    hi_uint32 ui_on_time; //0 不启用
    hi_uint32 ui_off_time;
    hi_uint32 ui_cycle;
    hi_uchar8 uc_flag;
    hi_uint32 ui_lasttime;
    hi_uint32 ui_cyclecount;
} hi_gpio_led_s;

typedef struct
{
    HI_GPIO_KEY_EVENT_E em_event;   //按下、长按、短按、松开
    hi_uint32           ui_mask;
    hi_uint32           ui_threshlod_short;
    hi_uint32           ui_threshlod_long;
    hi_uint32           ui_lasttime;
} hi_gpio_key_s;

typedef struct
{
    hi_uchar8     uc_port;
    hi_uchar8     uc_mode;
    hi_uchar8     uc_level;
    hi_uchar8     uc_flag;
    hi_gpio_led_s st_gpioled;
    hi_gpio_key_s st_gpiokey;
    hi_void     * pv_callback;
} hi_gpio_info_s;

typedef struct
{
    hi_uint32 em_port;
    hi_uint32 em_bit;
    hi_uint32 em_level;
} hi_kernel_gpio_bit_s;

//钩子函数原型：
typedef hi_int32 (*hi_gpio_report)(  hi_uchar8 ui_port, hi_uint32 ui_time, hi_uint32 ui_type);

//1.点灯：
hi_int32 hi_gpio_led_on(hi_uchar8 ui_port);

//2.熄灯：
hi_int32 hi_gpio_led_off(hi_uchar8 ui_port);

//3.闪灯：
hi_int32 hi_gpio_led_flash( hi_uchar8 uc_port, hi_uint32 ui_on_time, hi_uint32 ui_off_time, hi_uint32 ui_cycle);

//4.设置按键门限：
hi_int32 hi_gpio_key_set_threshlod(hi_uchar8 uc_port, hi_uint32 ui_short_press, hi_uint32 ui_long_press);

//5.设置按键事件上报掩码，置为1的bit，表示允许对应事件的上报
hi_int32 hi_gpio_key_set_event(hi_uchar8 uc_port, hi_uint32 ui_mask);

//6.模拟按键按下：
hi_int32 hi_gpio_key_set_down(hi_uchar8 uc_port);

//7.模拟按键松开：
hi_int32 hi_gpio_key_set_up(hi_uchar8 uc_port);

//8.注册按键上报钩子：
hi_int32 hi_gpio_key_set_report_hook(hi_uchar8 uc_port, hi_gpio_report *pv_callback);

//9.GPIO初始化函数
hi_int32 hi_gpio_init();

//10.GPIO退出函数
hi_int32 hi_gpio_exit();

//11.灯监控
hi_uint32 hi_gpio_led_monitor( hi_void );

//12.按键监控
hi_uint32 hi_gpio_key_monitor( hi_void );

//13.添加需要控制的GPIO
hi_uint32 hi_gpio_add(hi_gpio_info_s *st_gpio_info, hi_uchar8 uc_count);

#endif

/* DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 END> */
