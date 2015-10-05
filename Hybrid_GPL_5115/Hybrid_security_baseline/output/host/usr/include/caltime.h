#ifndef __ATPUTILCOUNTAPI_H__
#define __ATPUTILCOUNTAPI_H__

#include "atptypes.h"

#define ATP_UTIL_PERIODICTIME_MINLEN  19
#define ATP_UTIL_PERIODICTIME_MAXLEN  20

//字符串实际的长度
#define ATP_UTIL_PERYEAR_REALLEN  4
#define ATP_UTIL_PERDAY_REALLEN   2

//年月日时间是否有效的校验宏
#define ATP_UTIL_PRDICYEAR_MIN         0
#define ATP_UTIL_PRDICMONTH_MIN        0
#define ATP_UTIL_PRDICMONTH_MAX        12
#define ATP_UTIL_PRDICDAY_MIN          0
#define ATP_UTIL_PRDICDAY_MAX          31
#define ATP_UTIL_PRDICHOUR_MIN         0
#define ATP_UTIL_PRDICHOUR_MAX         23
#define ATP_UTIL_PRDICMINU_MIN         0
#define ATP_UTIL_PRDICMINU_MAX         59
#define ATP_UTIL_PRDICSEC_MIN          0
#define ATP_UTIL_PRDICSEC_MAX          59

#define ATP_UTIL_LEAPYEAR_MONTH        2
#define ATP_UTIL_LEAPYEAR_DAY          29

#define ATP_UTIL_ONEYEAR_DAY           365
#define ATP_UTIL_SMONTH_DAY            30
#define ATP_UTIL_ONEDAY_HOUR           24
#define ATP_UTIL_ONEHOUR_MINU          60

typedef struct  tagATP_UTIL_PERIODICTIMEINT_ST
{
    VOS_UINT32 ulYear;               
    VOS_UINT32 ulMonth;
    VOS_UINT32 ulDay; 
    VOS_UINT32 ulHour; 
    VOS_UINT32 ulMinu; 
    VOS_UINT32 ulSec; 
}ATP_UTIL_PERIODICTIMEINT_ST;

typedef enum tagATP_UTIL_COMPTIME_EN
{
    ATP_UTIL_PERIODICTIME_SMALL,
    ATP_UTIL_PERIODICTIME_BIG,
    ATP_UTIL_PERIODICTIME_SAME
}ATP_UTIL_COMPTIME_EN;
#endif

