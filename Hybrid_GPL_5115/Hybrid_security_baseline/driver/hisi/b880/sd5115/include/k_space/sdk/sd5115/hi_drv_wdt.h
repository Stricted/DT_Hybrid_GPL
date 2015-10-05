/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_wdt.h
  功能描述: wdt驱动头文件
  版本描述: V1.0
  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组
  修改记录:
******************************************************************************/
#ifndef __HI_DRV_WDT_H__
#define __HI_DRV_WDT_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*wdt工作模式*/
typedef enum
{
    HI_WDT_USER_MODE_E = 0x0,                /*用户模式，用户负责清狗*/
    HI_WDT_AUTO_MODE_E,                      /*自动模式，驱动内部负责清狗*/
    HI_WDT_DISABLE_MODE_E,                   /*禁止模式，看门狗无效*/
} HI_WDT_WORK_MODE_E;

/*wdt属性*/
typedef struct
{
    HI_WDT_WORK_MODE_E em_mode;              /*默认为自动模式*/
    uint               ui_interval;          /*系统复位间隔: 1-2 * (0xffff_ffff / 系统时钟)*/
} hi_wdt_attr_s;

/*统计异常记录等*/
typedef struct
{
    uint ui_invalid_parameter_cnt;           /*无效参数计数*/
} hi_wdt_cnt_s;

/*配置属性操作*/
extern uint hi_kernel_wdt_attr_set(hi_wdt_attr_s *pst_attr);
extern uint hi_kernel_wdt_attr_get(hi_wdt_attr_s *pst_attr);

/*清狗操作*/
extern void hi_kernel_wdt_clear(void);

/*统计异常记录获取*/
extern uint hi_kernel_wdt_cnt_get(hi_wdt_cnt_s *pst_cnt);

#endif /*__HI_DRV_WDT_H__*/
