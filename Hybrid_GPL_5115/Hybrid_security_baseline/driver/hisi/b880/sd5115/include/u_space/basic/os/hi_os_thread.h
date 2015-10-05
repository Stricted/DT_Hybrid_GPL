/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_thread.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_03
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_THREAD_H__
#define __HI_OS_THREAD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_OS_SYS_READ_BUF_SIZE                 (128)  

/* 线程句柄 */
#define HI_PTHREAD_T        hi_uint32

typedef hi_void*  (*hi_os_threadfun_t)(hi_void *); 

/* Fast locks (not abstract because mutexes and conditions aren't abstract). */
typedef struct {
    hi_int32  i_status;      /* "Free" or "taken" or head of waiting list */ 
    hi_int32  i_spinlock;     /* Used by compare_and_swap emulation. 
                                Also,adaptive SMP lock stores spin count here. */
}hi_os_thread_fastlock_s;

/* Data structure to describe a process' schedulability.  */
typedef struct {
    hi_int32 i_schedpriority;
}hi_os_sched_param_s;

/* Attributes for threads. (pthread_attr) */
typedef struct {
    hi_int32           i_detachstate;
    hi_int32           i_schedpolicy;
    hi_os_sched_param_s st_schedparam;
    hi_int32           i_inheritsched;
    hi_int32           i_scope;
    hi_uint32          ui_guardsize;
    hi_int32           i_stackaddrset;
    hi_void*           pv_stackaddr;
    hi_uint32          ui_stacksize;
}hi_os_thread_attr_s;

/* pthread_mutexattr_t */
typedef struct {
    hi_int32 i_mutexattr;
}hi_os_pthread_mutex_attr_s;

/* pthread_mutex_t (pthread_mutex_t)*/
typedef struct { 
    hi_int32                 i_reserved;  /* Reserved for future use */  
    hi_int32                 i_count;     /* Depth of recursive locking */ 
    hi_void*                 pv_owner;    /* Owner thread (if recursive or errcheck) */ 
    hi_int32                 i_kind;      /* Mutex kind: fast, recursive or errcheck */ 
    hi_os_thread_fastlock_s  st_lock;     /* Underlying fast lock */
}hi_os_pthread_mutex_s;

/*调整进程优先级*/
typedef enum {
    HI_OS_PRIO_PROCESS_E = 0,  /* WHO is a process ID.  */
    HI_OS_PRIO_PGRP_E = 1,     /* WHO is a process group ID.  */
    HI_OS_PRIO_USER_E = 2      /* WHO is a user ID.  */
}hi_os_which_e;

typedef enum {
    HI_OS_RUSAGE_BOTH_E = -2,      /* Both.  */
    HI_OS_RUSAGE_CHILDREN_E = -1,  /* All of its terminated child processes.  */
    HI_OS_RUSAGE_SELF_E = 0,       /* The calling process.  */
}hi_os_who_e;

extern hi_int32 hi_os_execcmd(hi_char8 *pc_command);
extern hi_int32 hi_os_execcmd_nohang(hi_char8 *pc_command);
extern hi_int32 hi_os_execv(const hi_char8 *pc_path, hi_char8* const a_pc_argv[]);
extern hi_int32 hi_os_execvp(const hi_char8 *pc_file, hi_char8 *const a_pc_argv[]);
extern hi_void hi_os_exit(hi_int32 i_status);
extern hi_int32* hi_os_pidbyname( hi_char8* pc_pidname);
extern hi_int32 hi_os_fork(hi_void);
extern hi_uint32 hi_os_getpid(hi_void);
extern hi_int32 hi_os_getpriority(hi_int32 i_which, hi_int32 i_who);
extern hi_int32 hi_os_kill(hi_int32 i_pid, hi_int32 i_sig);
extern hi_int32 hi_os_prctl(const hi_char8 * pc_name);
extern hi_int32 hi_os_pthread_setcancelstate(hi_int32 i_state, hi_int32 *i_oldstate);
extern hi_int32 hi_os_pthread_pthread_setcanceltype(hi_int32 i_type, hi_int32 *i_oldtype);
extern hi_int32 hi_os_pthread_attr_setdetachstate(hi_os_thread_attr_s *pst_attr, hi_int32 i_detachstate);
extern hi_int32 hi_os_pthread_cancel(HI_PTHREAD_T st_thread);
extern hi_int32  hi_os_pthread_create (HI_PTHREAD_T *pst_threadp,
                                      const hi_os_thread_attr_s *pst_attr,
                                      hi_os_threadfun_t pf_startroutine,
                                      hi_void *pv_arg);
extern hi_int32 hi_os_pthread_attr_destroy(hi_os_thread_attr_s *pst_attr);
extern hi_int32 hi_os_pthread_detach (HI_PTHREAD_T pst_thr);
extern hi_void hi_os_pthread_exit ( hi_void* pv_retval);
extern hi_int32 hi_os_pthread_attr_init(hi_os_thread_attr_s *pst_attr);
extern hi_int32 hi_os_pthread_join (HI_PTHREAD_T st_thr, hi_void** ppv_threadreturn);
extern HI_PTHREAD_T  hi_os_pthread_self (hi_void);
extern hi_int32 hi_os_pthread_setschedparam(HI_PTHREAD_T st_threadid, hi_int32 i_policy,
                                           hi_os_sched_param_s *pst_param);
extern hi_int32 hi_os_reboot(hi_int32 i_flag);
extern hi_uint32 hi_os_rmdelimitor( hi_char8 *pc_buf);
extern hi_int32 hi_os_setpriority(hi_int32 i_which, hi_int32 i_who, hi_int32 i_pri);
extern hi_uint32 hi_os_sleep(hi_uint32 ui_seconds);
extern hi_uint32 hi_os_msleep(hi_uint32 ui_ms);
extern hi_uint32 hi_os_usleep(hi_uint32 ui_usec);
extern hi_int32 hi_os_system(hi_char8 *pc_cmd);
extern hi_int32 hi_os_wait(const hi_int32* pi_status);
extern hi_int32 hi_os_waitpid(hi_int32 i_pid,  hi_int32 *i_status, hi_int32 i_options);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_THREAD_H__ */
