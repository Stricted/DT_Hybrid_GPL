/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_gpio.h
  功能描述: gpio驱动头文件
  版本描述: V1.0
  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组
  修改记录:
******************************************************************************/
#ifndef __HI_DRV_GPIO_H__
#define __HI_DRV_GPIO_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*一组GPIO管脚*/
typedef enum
{
    HI_GPIO_BIT0_E = 0,
    HI_GPIO_BIT1_E,
    HI_GPIO_BIT2_E,
    HI_GPIO_BIT3_E,
    HI_GPIO_BIT4_E,
    HI_GPIO_BIT5_E,
    HI_GPIO_BIT6_E,
    HI_GPIO_BIT7_E,
    HI_GPIO_BIT8_E,
    HI_GPIO_BIT9_E,
    HI_GPIO_BIT10_E,
    HI_GPIO_BIT11_E,
    HI_GPIO_BIT12_E,
    HI_GPIO_BIT13_E,
    HI_GPIO_BIT14_E,
    HI_GPIO_BIT15_E,
    HI_GPIO_BIT16_E,
    HI_GPIO_BIT17_E,
    HI_GPIO_BIT18_E,
    HI_GPIO_BIT19_E,
    HI_GPIO_BIT20_E,
    HI_GPIO_BIT21_E,
    HI_GPIO_BIT22_E,
    HI_GPIO_BIT23_E,
    HI_GPIO_BIT24_E,
    HI_GPIO_BIT25_E,
    HI_GPIO_BIT26_E,
    HI_GPIO_BIT27_E,
    HI_GPIO_BIT28_E,
    HI_GPIO_BIT29_E,
    HI_GPIO_BIT30_E,
    HI_GPIO_BIT31_E,
} HI_GPIO_BIT_E;

/*GPIO组*/

/*
说明：
 在SD5115V100-MPW-ASIC中，只用到了HI_GPIO_PORT0_E -- HI_GPIO_PORT3_E共4组GPIO，
,第一组GPIO对应GPIO_0-GPIO_31，第二组GPIO对应GPIO_32-GPIO_63, 第三组GPIO对应GPIO64-GPIO_95,
 其中，从GPIO_64至GPIO_75这12个IO口目前不能作为普通输入输出口，第四组GPIO对应GPIO96-GPIO_106共11个IO口。
 */
typedef enum
{
    HI_GPIO_PORT0_E = 0,
    HI_GPIO_PORT1_E,
    HI_GPIO_PORT2_E,
    HI_GPIO_PORT3_E,
    HI_GPIO_PORT4_E,    //暂时没用
    HI_GPIO_PORT5_E,    //暂时没用
    HI_GPIO_PORT6_E,    //暂时没用
    HI_GPIO_PORT7_E,    //暂时没用
} HI_GPIO_PORT_E;

/*GPIO工作模式*/
typedef enum
{
    HI_GPIO_WORK_MODE_INPUT_E = 0,                     /*输入模式*/
    HI_GPIO_WORK_MODE_OUTPUT_E,                        /*输出模式*/
    HI_GPIO_WORK_MODE_HARDWARE_E,                      /*特殊功能模式，硬件模式管脚复用*/
} HI_GPIO_WORK_MODE_E;

/*GPIO一位的属性*/
typedef struct
{
    HI_GPIO_WORK_MODE_E em_mode;                      /*GPIO一位的工作模式*/
} hi_gpio_bit_attr_s;

/*GPIO端口属性*/
typedef struct
{
    HI_BOOL_E em_enable;                              /*GPIO端口使能属性，HI_TRUE_E: GPIO端口时钟打开、HI_FALSE_E: GPIO端口时钟关闭*/
} hi_gpio_port_attr_s;

/*统计异常记录*/
typedef struct
{
    uint ui_invalid_parameter_cnt;                    /*输入无效参数的次数*/
} hi_gpio_cnt_s;

/*GPIO位配置属性操作*/
extern uint hi_kernel_gpio_bit_attr_get(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, hi_gpio_bit_attr_s *pst_attr);
extern uint hi_kernel_gpio_bit_attr_set(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, hi_gpio_bit_attr_s *pst_attr);

/*GPIO端口属性配置,无特殊需要不要关闭GPIO端口时钟，会导致该组GPIO相关的位不能工作*/
extern uint hi_kernel_gpio_port_attr_get(HI_GPIO_PORT_E em_port, hi_gpio_port_attr_s *pst_attr);
extern uint hi_kernel_gpio_port_attr_set(HI_GPIO_PORT_E em_port, hi_gpio_port_attr_s *pst_attr);

/*GPIO端口输出 当该GPIO端口(0~7bit)全部配置为输出模式时, 通过此接口配置输出数据*/
extern uint hi_kernel_gpio_write(HI_GPIO_PORT_E em_port, uint uc_data);

/*GPIO端口输入 当该GPIO端口(0~7bit)全部配置为输入模式时, 通过此接口得到输入数据*/
extern uint hi_kernel_gpio_read(HI_GPIO_PORT_E em_port, uint *puc_data);

/*GPIO电平设置, 当该GPIO配置为输出模式时,通过此接口配置输出电平*/
extern uint hi_kernel_gpio_bit_write(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, HI_LEVEL_E em_level);

/*GPIO输入电平获取，当该GPIO配置为输入时,可通过此接口得到输入电平*/
extern uint hi_kernel_gpio_bit_read(HI_GPIO_PORT_E em_port, HI_GPIO_BIT_E em_bit, HI_LEVEL_E *pem_level);

/*取统计异常数据*/
extern uint hi_kernel_gpio_cnt_get(hi_gpio_cnt_s *pst_cnt);

#endif /* __HI_DRV_GPIO_H__ */
