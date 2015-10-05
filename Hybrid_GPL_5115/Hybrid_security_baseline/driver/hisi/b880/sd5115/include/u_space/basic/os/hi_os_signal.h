/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_signal.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_SIGNAL_H__
#define __HI_OS_SIGNAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MAX_SINGAL_NUMBER (32)

#define HI_NSIG          (1024)
#define HI_NSIG_BPW      ((8) * sizeof(hi_uint32))
#define HI_NSIG_WORDS    (HI_NSIG / HI_NSIG_BPW)
#define HI_OS_SIGINT      SIGINT
#define HI_OS_SIGTERM     SIGTERM
#define HI_OS_SIG_IGN     SIG_IGN
#define HI_OS_SIGHUP      SIGHUP
#define HI_OS_SIGQUIT     SIGQUIT
#define HI_OS_SIGILL      SIGILL
#define HI_OS_SIGABRT     SIGABRT
#define HI_OS_SIGFPE      SIGFPE
#define HI_OS_SIGPIPE     SIGPIPE
#define HI_OS_SIGALRM     SIGALRM
#define HI_OS_SIGKILL     SIGKILL
#define HI_OS_SIGBUS      SIGBUS
#define HI_OS_SIGTSTP     SIGTSTP
#define HI_OS_SIGUSR1     SIGUSR1
#define HI_OS_SIGUSR2     SIGUSR2

typedef hi_void * HI_SIGINFO_T;
typedef hi_void (*HI_PFUNCSIGPROC)(hi_int32);

typedef struct {
    hi_uint32 a_ui_sig[HI_NSIG_WORDS];
} hi_os_signal_set_s;

typedef union {
    hi_int32 i_sivalint;
    hi_void *pv_sivalptr;
} hi_os_signal_val_u;

typedef struct {
    hi_uint32 i_tvsec;       /* Seconds.  */
    hi_int32 i_tvnsec;       /* Nanoseconds.  */
}hi_os_time_spec_s;

extern hi_void* hi_os_signal(hi_int32 i_signal_info, HI_PFUNCSIGPROC pf_sig_proc);
extern hi_int32 hi_os_sigaction(hi_int32 i_sig_num, const struct sigaction* pst_act, struct sigaction* pst_old_act);
extern hi_int32 hi_os_sigaddset(hi_os_signal_set_s *pst_set, hi_int32 i_sig_num);
extern hi_int32 hi_os_sigismember(const hi_os_signal_set_s *pst_set, hi_int32 i_sig_num);
extern hi_int32 hi_os_sigemptyset(hi_os_signal_set_s* pst_set);
extern hi_int32 hi_os_sigdelset(hi_os_signal_set_s *pst_set, hi_int32 i_sig_num);
extern hi_void hi_os_signal_exception(hi_void);
extern hi_int32 hi_os_sigfillset(hi_os_signal_set_s *pst_set);
extern hi_int32 hi_os_pthread_kill(HI_PTHREAD_T thread, hi_int32 i_sign_o);
extern hi_int32 hi_os_sigpending(hi_os_signal_set_s *pst_set);
extern hi_int32 hi_os_sigprocmask(hi_int32 i_how, const hi_os_signal_set_s *pst_set, hi_os_signal_set_s *pst_old_set);
extern hi_int32 hi_os_pthread_sigmask(hi_int32 i_how, const hi_os_signal_set_s  *pst_new_mask,  hi_os_signal_set_s  *pst_old_mask);
extern hi_int32 hi_os_sigqueue(hi_int32 i_pid, hi_int32 i_sig, const hi_os_signal_val_u u_value);
extern hi_int32 hi_os_sigsuspend(const hi_os_signal_set_s *pst_mask);
extern hi_int32  hi_os_sigtimedwait(const  hi_os_signal_set_s  *pst_set,  HI_SIGINFO_T *pst_Info, const hi_os_time_spec_s *pst_timeout);
extern hi_void hi_os_signaltrap(hi_int32 i_signal);
extern hi_int32 hi_os_sigwait(const hi_os_signal_set_s *pst_set, hi_int32 *pi_sig);
extern hi_int32 hi_os_sigwaitinfo(const hi_os_signal_set_s *pst_set, HI_SIGINFO_T *pst_Info);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_SIGNAL_H__ */

