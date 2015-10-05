/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_timer.h
  版 本 号   : 初稿
  作    者   : hisilicon l00183967
  生成日期   : D2011_07_23
  最近修改   :

******************************************************************************/
#ifndef __HI_KERNEL_TIMER_H__
#define __HI_KERNEL_TIMER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct
{
    struct timer_list st_timer;          // linux kernel timer struct
    hi_uint32         ui_nexttimeout;    // in unit of hi_kernel_jiffies(ticks)
}hi_kernel_timer_s;

// stop timer, del_timer always execute successful
#define hi_kernel_timer_stop(pst_timer)    (void)hi_kernel_del_timer_sync(&(pst_timer)->st_timer)

// start timer
#define hi_kernel_timer_start(pst_timer, pv_func, ui_data)  do  \
{   \
    if ( (pst_timer)->ui_nexttimeout > 0)     \
    {   \
        (pst_timer)->st_timer.expires = hi_kernel_jiffies + (pst_timer)->ui_nexttimeout;  \
        (pst_timer)->st_timer.function = (hi_void*)pv_func; \
        (pst_timer)->st_timer.data = (ui_data);        \
        hi_kernel_add_timer(&(pst_timer)->st_timer);            \
    }   \
} while(0)

#define hi_kernel_timer_init(pst_timer, ui_timeout)    do  \
{   \
    (pst_timer)->ui_nexttimeout = (ui_timeout);   \
    hi_kernel_init_timer(&(pst_timer)->st_timer);       \
} while(0)


// stop & restart timer
#define hi_kernel_timer_restart(pst_timer, pv_func, ui_data, ui_timeout, ui_mintime)   do  \
{   \
    (void)hi_kernel_del_timer_sync(&(pst_timer)->st_timer);   \
    if ((ui_timeout) > 0)    \
    {   \
        (pst_timer)->ui_nexttimeout = HI_MAX(ui_mintime, (ui_timeout)); \
        (pst_timer)->st_timer.expires = hi_kernel_jiffies + (pst_timer)->ui_nexttimeout;  \
        (pst_timer)->st_timer.function = (hi_void*)(pv_func); \
        (pst_timer)->st_timer.data = (ui_data);        \
        hi_kernel_add_timer(&(pst_timer)->st_timer);            \
    }   \
    else    \
    {   \
        (pst_timer)->ui_nexttimeout = 0;  \
    }   \
} while(0)
#define hi_kernel_timer_modify(pst_timer, pv_func, ui_timeout, ui_data)  do  \
    {   \
        if ((ui_timeout) > 0)    \
        {       \
            (pst_timer)->ui_nexttimeout    = (ui_timeout);               \
            (pst_timer)->st_timer.expires  = hi_kernel_jiffies + (pst_timer)->ui_nexttimeout; \
            (pst_timer)->st_timer.function = (hi_void*)(pv_func); \
            (pst_timer)->st_timer.data     = (ui_data);        \
            hi_kernel_mod_timer(&(pst_timer)->st_timer,  (pst_timer)->st_timer.expires ); \
        }       \
        else    \
        {       \
            (pst_timer)->ui_nexttimeout = 0;  \
        }   \
    } while(0)

#define hi_kernel_timer_isrunning(pst_timer)   hi_kernel_timer_pending(&(pst_timer)->st_timer)    
#define hi_kernel_timer_setnexttimeout(pst_timer, ui_timeout, ui_mintime)  do  \
{   \
    if ((ui_timeout) > 0)    \
    {       \
        (pst_timer)->ui_nexttimeout = HI_MAX(ui_mintime, (ui_timeout)); \
    }       \
    else    \
    {       \
        (pst_timer)->ui_nexttimeout = 0;  \
    }   \
} while(0)
#define hi_kernel_timer_getnexttimeout(pst_timer)  ((pst_timer)->ui_nexttimeout)
#define hi_kernel_timer_getnextexpires(pst_timer)  ((pst_timer)->st_timer.expires)
#define hi_kernel_timer_aftereq(expires)           (hi_kernel_time_after_eq(hi_kernel_jiffies, expires))
#define hi_kernel_msecs_to_hi_kernel_jiffies(msecs)         (hi_kernel_msecs_to_jiffies(msecs))


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_TIMER_H__ */
