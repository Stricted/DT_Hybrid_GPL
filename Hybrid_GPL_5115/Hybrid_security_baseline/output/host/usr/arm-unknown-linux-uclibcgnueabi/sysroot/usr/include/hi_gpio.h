/* <DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 BEGIN */
/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_monitor_gpio.h
  �� �� ��   : ����
  ��    ��   : l00204224
  ��������   : D2012_7_12

******************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<�޸���>     	 	<ʱ��>       	 <���ⵥ��>            		 <��Ҫ����>
z00185914           2013/01/28          DTS2013012808057		�����š��½�ndis�����ļ��� 
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

#define GPIO_NUM 128 //GPIO����


//�����¼�����ö�٣�
typedef enum
{
    HI_GPIO_KEY_EVENT_DOWN_E  = 1,
    HI_GPIO_KEY_EVENT_SHORT_E = 2,
    HI_GPIO_KEY_EVENT_LONG_E = 4,
    HI_GPIO_KEY_EVENT_UP_E = 8
} HI_GPIO_KEY_EVENT_E;

//����¼�����ö�٣�
typedef enum
{
    HI_GPIO_LED_EVENT_ON_E = 0,
    HI_GPIO_LED_EVENT_OFF_E,
} HI_GPIO_LED_EVENT_E;

typedef struct
{
    hi_uint32 ui_on_time; //0 ������
    hi_uint32 ui_off_time;
    hi_uint32 ui_cycle;
    hi_uchar8 uc_flag;
    hi_uint32 ui_lasttime;
    hi_uint32 ui_cyclecount;
} hi_gpio_led_s;

typedef struct
{
    HI_GPIO_KEY_EVENT_E em_event;   //���¡��������̰����ɿ�
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

//���Ӻ���ԭ�ͣ�
typedef hi_int32 (*hi_gpio_report)(  hi_uchar8 ui_port, hi_uint32 ui_time, hi_uint32 ui_type);

//1.��ƣ�
hi_int32 hi_gpio_led_on(hi_uchar8 ui_port);

//2.Ϩ�ƣ�
hi_int32 hi_gpio_led_off(hi_uchar8 ui_port);

//3.���ƣ�
hi_int32 hi_gpio_led_flash( hi_uchar8 uc_port, hi_uint32 ui_on_time, hi_uint32 ui_off_time, hi_uint32 ui_cycle);

//4.���ð������ޣ�
hi_int32 hi_gpio_key_set_threshlod(hi_uchar8 uc_port, hi_uint32 ui_short_press, hi_uint32 ui_long_press);

//5.���ð����¼��ϱ����룬��Ϊ1��bit����ʾ�����Ӧ�¼����ϱ�
hi_int32 hi_gpio_key_set_event(hi_uchar8 uc_port, hi_uint32 ui_mask);

//6.ģ�ⰴ�����£�
hi_int32 hi_gpio_key_set_down(hi_uchar8 uc_port);

//7.ģ�ⰴ���ɿ���
hi_int32 hi_gpio_key_set_up(hi_uchar8 uc_port);

//8.ע�ᰴ���ϱ����ӣ�
hi_int32 hi_gpio_key_set_report_hook(hi_uchar8 uc_port, hi_gpio_report *pv_callback);

//9.GPIO��ʼ������
hi_int32 hi_gpio_init();

//10.GPIO�˳�����
hi_int32 hi_gpio_exit();

//11.�Ƽ��
hi_uint32 hi_gpio_led_monitor( hi_void );

//12.�������
hi_uint32 hi_gpio_key_monitor( hi_void );

//13.�����Ҫ���Ƶ�GPIO
hi_uint32 hi_gpio_add(hi_gpio_info_s *st_gpio_info, hi_uchar8 uc_count);

#endif

/* DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 END> */
