/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_os_time.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_08_03
  ����޸�   :
******************************************************************************/

#ifndef __HI_OS_TIME_H__
#define __HI_OS_TIME_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_TIME_SEC_PER_MIN     (60)
#define HI_SEC_IN_HOUR          (3600);  /* 1Сʱ������ */

/*��ʱ������*/
#define HI_OS_ITIMER_REAL       ITIMER_REAL         /* ��ϵͳ��ʵ��ʱ�������㣬���ͳ�SIGALRM�źš� */
#define HI_OS_ITIMER_VIRTUAL    ITIMER_VIRTUAL      /* �Ըý���������ִ�е�ʱ�������㣬���ͳ�SIGVTALRM�źš� */
#define HI_OS_ITIMER_PROF       ITIMER_PROF         /* �Խ���������ִ�м��ں��������ѵ�ʱ�������㣬���ͳ�SIGPROF�źš� */


/* ϵͳ�������ݽṹ */
typedef struct {
    hi_uint32 ui_sec;        /* seconds */
    hi_uint32 ui_min;        /* minutes */
    hi_uint32 ui_hour;       /* hours */
    hi_uint32 ui_mday;       /* day of the month */
    hi_uint32 ui_mon;        /* month, from 0 to 11 */
    hi_uint32 ui_year;       /* year, from 1900 */
    hi_uint32 ui_wday;       /* day of the week */
    hi_uint32 ui_yday;       /* day in the year */
    hi_uint32 ui_isdst;      /* daylight saving time */
    hi_uint32 ui_gmtoff;     /* Seconds east of UTC.  */
    hi_char8 *pc_tmzone;     /* Timezone abbreviation.  */  
}hi_os_tm_s;

/* ϵͳʱ�侫�����ݽṹ */
typedef struct {
    hi_uint32 tv_sec;           /* Seconds.  */
    hi_uint32 tv_usec;          /* Microseconds.  */
}hi_os_timeval_s;

/* ϵͳ��ʱ���������ݽṹ */
typedef struct {
    hi_os_timeval_s st_interval;
    hi_os_timeval_s st_value;
}hi_os_itimer_val_s;

/* ϵͳʱ�����ݽṹ */
typedef struct {
    hi_int32 i_minutewest;      /* minutes west of Greenwich */
    hi_int32 i_dsttime;         /* type of DST correction */
}hi_os_time_zone_s;

extern hi_char8 *hi_os_ctime(const hi_uint32 *pi_time);
extern hi_uint32 hi_os_getsystemtime(hi_void);
extern hi_void hi_os_gettime(hi_os_tm_s * pst_curdate);
extern hi_int32 hi_os_gettimeofday(hi_os_timeval_s* pst_tv);
extern hi_int32 hi_os_gettimeofday_ex(hi_os_timeval_s* pst_tv, hi_os_time_zone_s *pst_zone);
extern hi_os_tm_s * hi_os_gmtime(const hi_uint32 *pi_time);
extern hi_os_tm_s *hi_os_localtime(const hi_uint32 *pi_time);
extern hi_uint32 hi_os_mktime(hi_os_tm_s *pst_time);
extern hi_uint32 hi_os_random(hi_void);
extern hi_uint32 hi_os_settimeofday(hi_os_timeval_s *pst_time_val,hi_os_time_zone_s* pst_time_zone);
extern hi_void   hi_os_srand(hi_uint32 ui_seed);
extern hi_uint32 hi_os_strftime(hi_char8*pc_result,hi_uint32 ui_len,
                                const hi_char8 *pc_format,  const hi_os_tm_s *pst_time);
extern hi_uint32 hi_os_time(hi_uint32 *ui_time);
extern hi_uint32 hi_os_dsttotime(const hi_os_tm_s * pst_timer, hi_os_tm_s * pst_timerout);
extern hi_uint32 hi_os_timetodst(const hi_os_tm_s * pst_timer, hi_os_tm_s * pst_timerout);
extern hi_uint32 hi_os_timetogm(const hi_os_tm_s * pst_timer, hi_os_tm_s * pst_timerout);
extern hi_uint32 hi_os_timetolocal(const hi_os_tm_s * pst_timer, hi_os_tm_s * pst_timerout);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_TIME_H__ */

