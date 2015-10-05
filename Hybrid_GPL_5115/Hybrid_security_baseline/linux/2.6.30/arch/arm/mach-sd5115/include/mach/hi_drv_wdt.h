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
/* <DTS2012110301136 l00205975 20121103 begin */
#define HI_WDT_TIME_BIT_MASK                    0xFFF803FF

#define HI_WDT_CLEAR_INTER                      3  // 3s
#define HI_WDT_DEF_TIMER                        30 // 30s

#define HI_WDT_MAX_CLRTIMER                     30 // 30s 最大清狗周期
#define HI_WDT_MAX_TIMER                        120

/*wdt属性*/
typedef struct
{
    unsigned int ui_state;
    unsigned int ui_lsttime;            /*上次清狗时间*/
    unsigned int ui_period;             /*清狗周期: 1 - 120秒*/
    unsigned int ui_timeout;            /*狗超时时间: 1 - 120秒*/
} hi_wdt_attr_s;

/*清狗操作*/
extern void hi_kernel_wdt_clear(void);
/*设置硬件狗时间间隔 */
extern void hi_kernel_wdt_set(unsigned int ui_interval);
/*设置清狗时间间隔 */
extern void hi_kernel_wdt_set_clearintr(unsigned int ui_interval);
/*激活开关狗*/
extern void hi_kernel_wdt_disable(void);
extern void hi_kernel_wdt_enable(void);
/*检查是否准确喂狗*/
extern unsigned int hi_kernel_wdt_check(void);
/* DTS2012110301136 l00205975 20121103 end> */
#endif /*__HI_DRV_WDT_H__*/
