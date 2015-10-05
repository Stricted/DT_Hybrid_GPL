/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_runtime.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_RUNTIME_H__
#define __HI_OS_RUNTIME_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_OS_RUNTIME_MAX_LEN           512
#define HI_OS_RUNTIME_INTERVAL          1
#define HI_OS_RUNTIME_CPUTIME_NUM       4
#define HI_OS_RUNTIME_PROCNAME_LEN      32

/*进程CPU占用率信息 */
typedef struct {
    hi_char8  uc_procname[HI_OS_RUNTIME_PROCNAME_LEN];         /*进程名*/
    hi_uint32 ui_cpubusyrate;        /*该进程的CPU占用率*/
}hi_os_runtime_cpurate_s;

/*用户态运行时间, nice, 内核态运行时间, 系统空闲时间 */
typedef struct {
    hi_uint32 ui_usertime;
    hi_uint32 ui_nicetime;
    hi_uint32 ui_kerneltime;
    hi_uint32 ui_idletime;    
}hi_os_runtime_cputime_s;

/*进程占用内存信息*/
typedef struct {
    hi_char8   uc_procname[HI_OS_RUNTIME_PROCNAME_LEN];     /*进程名*/
    hi_uint32  ui_memalloc;         /*该进程已申请的内存*/
}hi_os_runtime_mem_rate_s;

/*线程状态定义*/
typedef enum {
    OS_THREAD_RUNNING,
    OS_THREAD_SLEEPING,
    OS_THREAD_UNINTERRUPT,
    OS_THREAD_ZOMBIE,
    OS_THREAD_STOPED,
    OS_THREAD_SWAPPING
}hi_os_runtime_threads_rate_s;

/*线程信息*/
typedef struct {
    hi_uint32 ui_threadid;               /*线程ID*/
    hi_os_runtime_threads_rate_s em_threadstate;     /*线程状态*/
} hi_os_runtime_thread_info_s;

extern hi_uint32 hi_os_runtime_get_cpu_busy_rate(hi_uint32 *pui_rate);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_RUNTIME_H__ */


