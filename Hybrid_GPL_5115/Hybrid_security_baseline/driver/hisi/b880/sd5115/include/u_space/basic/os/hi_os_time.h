/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_time.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_03
  最近修改   :
******************************************************************************/

#ifndef __HI_OS_TIME_H__
#define __HI_OS_TIME_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_TIME_SEC_PER_MIN     (60)
#define HI_SEC_IN_HOUR          (3600);  /* 1小时的秒数 */

/*定时器类型*/
#define HI_OS_ITIMER_REAL       ITIMER_REAL         /* 以系统真实的时间来计算，它送出SIGALRM信号。 */
#define HI_OS_ITIMER_VIRTUAL    ITIMER_VIRTUAL      /* 以该进程真正有执行的时间来计算，它送出SIGVTALRM信号。 */
#define HI_OS_ITIMER_PROF       ITIMER_PROF         /* 以进程真正有执行及在核心中所费的时间来计算，它送出SIGPROF信号。 */


/* 系统日期数据结构 */
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

/* 系统时间精度数据结构 */
typedef struct {
    hi_uint32 tv_sec;           /* Seconds.  */
    hi_uint32 tv_usec;          /* Microseconds.  */
}hi_os_timeval_s;

/* 系统定时器参数数据结构 */
typedef struct {
    hi_os_timeval_s st_interval;
    hi_os_timeval_s st_value;
}hi_os_itimer_val_s;

/* 系统时区数据结构 */
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

