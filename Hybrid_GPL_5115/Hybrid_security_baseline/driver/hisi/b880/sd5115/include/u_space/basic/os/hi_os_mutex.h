/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_os_mutex.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_08_02
  ����޸�   :

******************************************************************************/

#ifndef __HI_OS_MUTEX_H__
#define __HI_OS_MUTEX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern hi_int32 hi_os_pthread_mutex_destroy (hi_os_pthread_mutex_s *pst_mutex);
extern hi_int32 hi_os_pthread_mutex_init (hi_os_pthread_mutex_s *pst_mutex,const hi_os_pthread_mutex_attr_s *pst_mutexattr);
extern hi_int32 hi_os_pthread_mutex_lock (hi_os_pthread_mutex_s *pst_mutex);
extern hi_int32 hi_os_pthread_mutex_trylock (hi_os_pthread_mutex_s *pst_mutex);
extern hi_int32 hi_os_pthread_mutex_unlock (hi_os_pthread_mutex_s *pst_mutex);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_MUTEX_H__ */



