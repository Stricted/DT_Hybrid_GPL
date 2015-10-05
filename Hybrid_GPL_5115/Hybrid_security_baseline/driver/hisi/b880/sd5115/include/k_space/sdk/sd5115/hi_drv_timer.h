/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: hi_drv_timer.h
  功能描述: timer驱动头文件 
  版本描述: V1.0
  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组    
  修改记录: 
******************************************************************************/
#ifndef __HI_DRV_TIMER_H__
#define __HI_DRV_TIMER_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*timer选择, 只针对timer2，timer3*/
typedef enum
{
    HI_TIMER2_E = 0,                                   /*timer2*/
    HI_TIMER3_E,                                       /*timer3*/
} HI_TIMER_E;

/*timer tick*/
typedef enum
{
    HI_TICK_1MS_E = 0x0,                               /*1ms*/
    HI_TICK_2MS_E,                                     /*2ms*/
    HI_TICK_5MS_E,                                     /*5ms*/
    HI_TICK_10MS_E,                                    /*10ms*/
    HI_TICK_20MS_E,                                    /*20ms*/
    HI_TICK_50MS_E,                                    /*50ms*/
    HI_TICK_100MS_E,                                   /*100ms*/
    HI_TICK_US_E,                                      /*微秒级别定时器，属性中ui_usec指定timer周期, 实际调用中断处理函数为ui_period * ui_usec 微秒后*/
} HI_TIMER_TICK_E;

/*每个timer独立的配置属性*/
typedef struct
{
    HI_BOOL_E em_enable;                               /*HI_TRUE_E: timer使能 HI_FALSE_E: timer禁止*/
    HI_TIMER_TICK_E em_tick;                           /*timer周期*/
    uint ui_period;                                    /*em_tick时间间隔到达的次数（也是中断发生的次数）, 最小值为1*/
    uint ui_usec;                                      /*微秒级别下定时周期，em_tick为HI_TICK_US_E下才有效，取值：1~1000000*/
    void (*call_back)(void);                           /*中断回调函数*/
} hi_timer_attr_s;

/*异常统计数据*/
typedef struct
{
    uint ui_invalid_parameter_cnt;                     /*无效参数统计*/
} hi_timer_cnt_s;

/*timer属性操作*/
extern uint hi_kernel_timer_attr_set(HI_TIMER_E em_timer, hi_timer_attr_s *pst_attr);
extern uint hi_kernel_timer_attr_get(HI_TIMER_E em_timer, hi_timer_attr_s *pst_attr);

/*当前tick操作, 这里的一个tick是发生一次中断，消耗一个em_tick的时间，此接口给用户不用注册中断处理函数*/
/*通过查询当前的tick值，达到预定的值时候用户自己处理*/
extern uint hi_kernel_timer_cur_tick_get(HI_TIMER_E em_timer, uint *pui_cur_tick);
extern uint hi_kernel_timer_cur_tick_set(HI_TIMER_E em_timer, uint ui_cur_tick);

/*异常统计数据获取*/
extern uint hi_kernel_timer_cnt_get(hi_timer_cnt_s *pst_cnt);

#endif /*__HI_DRV_TIMER_H__*/
