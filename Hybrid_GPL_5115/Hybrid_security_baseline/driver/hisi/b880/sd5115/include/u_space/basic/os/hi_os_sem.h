/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_sem.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_SEM_H__
#define __HI_OS_SEM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* semop system calls takes an array of these. */
typedef struct {
    hi_ushort16     us_sem_num;    /* semaphore index in array */
    hi_short16		us_sem_op;     /* semaphore operation */
    hi_short16		us_sem_flg;    /* operation flags */
    hi_short16		us_resv;       /* for align */
}hi_os_sembuf_s;

typedef struct 
{
    hi_os_thread_fastlock_s st_semlock;
    hi_int32 i_semvalue;
    hi_void* pc_semwaiting;
}hi_os_sem_s;

typedef union
{
    hi_int32 val;  /* value for SETVAL */
}hi_os_semun_u;

extern hi_int32 hi_os_sem_close(const hi_os_sem_s *pst_sem);
extern hi_int32 hi_os_semctl(hi_int32 i_semid, hi_int32 i_semnum, hi_int32 i_cmd, hi_os_semun_u u_arg);
extern hi_int32 hi_os_sem_destroy(hi_os_sem_s *pst_sem);
extern hi_int32 hi_os_sem_unlink(const hi_char8 * pc_name);
extern hi_int32 hi_os_semget(hi_int32 i_key,hi_int32 i_nsems, hi_int32 i_semflg);
extern hi_int32 hi_os_sem_init(hi_os_sem_s *pst_sem, hi_int32 i_shared, hi_uint32 ui_value, hi_uint32 ui_maxvalue);
extern hi_int32 hi_os_semop(hi_int32 i_semid, hi_os_sembuf_s *sops, hi_int32 i_ops);
extern hi_os_sem_s *hi_os_sem_open(const hi_char8 *pc_name, hi_int32 i_operateflag, hi_uint32 ui_mode,hi_uint32 ui_value,hi_uint32 ui_maxvalue,hi_uint32 ui_inherithandle);
extern hi_int32 hi_os_sem_post(const hi_os_sem_s *pst_sem , hi_uint32 ui_semnum);
extern hi_int32 hi_os_sem_trywait(const hi_os_sem_s *pst_sem);
extern hi_int32 hi_os_sem_wait(const hi_os_sem_s *pst_sem);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_SEM_H__ */


