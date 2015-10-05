/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_os_runtime.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_08_02
  ����޸�   :

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

/*����CPUռ������Ϣ */
typedef struct {
    hi_char8  uc_procname[HI_OS_RUNTIME_PROCNAME_LEN];         /*������*/
    hi_uint32 ui_cpubusyrate;        /*�ý��̵�CPUռ����*/
}hi_os_runtime_cpurate_s;

/*�û�̬����ʱ��, nice, �ں�̬����ʱ��, ϵͳ����ʱ�� */
typedef struct {
    hi_uint32 ui_usertime;
    hi_uint32 ui_nicetime;
    hi_uint32 ui_kerneltime;
    hi_uint32 ui_idletime;    
}hi_os_runtime_cputime_s;

/*����ռ���ڴ���Ϣ*/
typedef struct {
    hi_char8   uc_procname[HI_OS_RUNTIME_PROCNAME_LEN];     /*������*/
    hi_uint32  ui_memalloc;         /*�ý�����������ڴ�*/
}hi_os_runtime_mem_rate_s;

/*�߳�״̬����*/
typedef enum {
    OS_THREAD_RUNNING,
    OS_THREAD_SLEEPING,
    OS_THREAD_UNINTERRUPT,
    OS_THREAD_ZOMBIE,
    OS_THREAD_STOPED,
    OS_THREAD_SWAPPING
}hi_os_runtime_threads_rate_s;

/*�߳���Ϣ*/
typedef struct {
    hi_uint32 ui_threadid;               /*�߳�ID*/
    hi_os_runtime_threads_rate_s em_threadstate;     /*�߳�״̬*/
} hi_os_runtime_thread_info_s;

extern hi_uint32 hi_os_runtime_get_cpu_busy_rate(hi_uint32 *pui_rate);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_RUNTIME_H__ */


