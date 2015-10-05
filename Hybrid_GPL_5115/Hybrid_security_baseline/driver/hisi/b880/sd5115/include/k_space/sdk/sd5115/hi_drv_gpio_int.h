/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_gpio_int.h
  功能描述: gpio_int驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_GPIO_INT_H__
#define __HI_DRV_GPIO_INT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*中断引脚GPIO1的4567位*/
typedef enum
{
    HI_GPIO_INT0_E = 0,                 /*中断引脚0*/
    HI_GPIO_INT1_E,                     /*中断引脚1*/
    HI_GPIO_INT2_E,                     /*中断引脚2*/
    HI_GPIO_INT3_E,                     /*中断引脚3*/
} HI_GPIO_INT_E;

/*EX INT中断触发模式*/
typedef enum
{
    HI_GPIO_INT_LOW_LEVEL_E = 0,         /*低电平触发中断*/
    HI_GPIO_INT_HIGH_LEVEL_E,            /*高电平触发中断*/
    HI_GPIO_INT_DOWN_EDGE_E,             /*下降沿触发中断*/
    HI_GPIO_INT_UP_EDGE_E,               /*上升沿触发中断*/
} HI_GPIO_INT_MODE_E;

/*GPIO_INT中断属性*/
typedef struct
{
    HI_BOOL_E          em_enable;        /*HI_TRUE_E: 中断使能 HI_FALSE_E: 中断禁止*/
    HI_GPIO_INT_MODE_E em_int_mode;      /*触发模式*/
    void (*call_back)(void);             /*中断回调函数*/
} hi_gpio_int_attr_s;

/*统计，异常记录*/
typedef struct
{
    uint ui_invalid_parameter_cnt;     /*输入无效参数计数*/
} hi_gpio_int_cnt_s;

/*配置属性操作*/
extern uint hi_kernel_gpio_int_attr_set(HI_GPIO_INT_E em_gpio_int, hi_gpio_int_attr_s *pst_attr);
extern uint hi_kernel_gpio_int_attr_get(HI_GPIO_INT_E em_gpio_int, hi_gpio_int_attr_s *pst_attr);

/*获取统计异常信息*/
extern uint hi_kernel_gpio_int_cnt_get(hi_gpio_int_cnt_s *pst_cnt);

#endif /*__HI_DRV_GPIO_INT_H__*/
