#ifndef __ATP_SNTP_MSGTYPES_H__
#define __ATP_SNTP_MSGTYPES_H__

#define ATP_CBBID_SNTP_NAME "sntp"

#define NTPDATE_FILE "/var/sntp.conf"
#define NTP_WANTYPE_FILE "/var/sntpwantype"


//TR181 specific CMO marco, the data model same as tr098, but with subtle different names
#ifdef SUPPORT_ATP_TR181
// Device.Time
#define CMO_TIME_LOCALTIMEZONENAME      CMO_TIME_LOCALTIMEZONE
#endif

#ifdef  SUPPORT_ATP_TR181
#define SNTP_UPDATE_INTERVAL    (6*60*60)
#endif


/* 文件配置关键字列表 */
#define SNTP_CONF_TIME_ZONE                     "TimeZone"
#define SNTP_CONF_TIME_ZONE_SRC                 "TimeSyncSource"
#define SNTP_CONF_SERVER                        "NTPServer"
#define SNTP_CONF_FAILED_INTERVAL               "NTPFailUpdate"
#define SNTP_CONF_SUCCED_INTERVAL               "NTPSuccessUpdate"

#ifdef SUPPORT_ATP_SNTP_DST
#define SNTP_CONF_DST_ENABLE                    "DstEnable"
#define SNTP_CONF_DST_START_MONTH               "DstStartMonth"
#define SNTP_CONF_DST_START_WEEK_NUM            "DstStartWeekNum"
#define SNTP_CONF_DST_START_WDAY                "DstStartWday"
#define SNTP_CONF_DST_START_TIME                "DstStartTime"
#define SNTP_CONF_DST_START_FLAG                "DstStartFlag"

#define SNTP_CONF_DST_END_MONTH                 "DstEndMonth"
#define SNTP_CONF_DST_END_WEEK_NUM              "DstEndWeekNum"
#define SNTP_CONF_DST_END_WDAY                  "DstEndWday"
#define SNTP_CONF_DST_END_TIME                  "DstEndTime"
#define SNTP_CONF_DST_END_FLAG                  "DstEndFlag"

#define SNTP_CONF_DST_OFFSET_TIME               "DstOffsetTime"
#endif

#define SNTP_SERVER_NUM  5          /* 最多支持5个服务器 */

/*sntp服务器名最大字符串长度*/
#define ATP_SNTP_SERVER_MAXLEN          64

/*所有服务器名加起来字符串最大长度*/
#define ATP_SNTP_ALLSERVER_LEN          (SNTP_SERVER_NUM * ATP_SNTP_SERVER_MAXLEN)

/* 夏令时是否使能 */
#define ATP_SNTP_DST_ENABLE             1
#define ATP_SNTP_DST_DISABLE            0

/* Start by d00107688 to add DST */
#define MONTH_START_OF_YEAR                     1
#define MONTH_OF_YEAR                           12
#define DAY_OF_A_WEEK                           7
#define MAX_WEEK_OF_MONTH                       5
#define DAY_OF_WEEK_SUNDAY                      0
#define DAY_OF_WEEK_SATURDAY                    6
#define HOUR_OF_DAY                             24                                  /* 一天的小时数 */
#define SECOND_OF_MIN                           60                                  /* 每分钟的秒数 */
#define SECOND_OF_HOUR                          (60 * (SECOND_OF_MIN))              /* 每小时的秒数 */
#define SECOND_OF_DAY                           ((HOUR_OF_DAY) * (SECOND_OF_HOUR))  /* 一天的秒数 */
#define NEGATIVE_SECOND_OF_DAY                  (0 - (SECOND_OF_DAY))
/* End by d00107688 to add DST */

#define IPV4_TYPE_ENABLE 1
#define IPV6_TYPE_ENABLE 2

/* start of by d00107688 增加状态报告 */
#define ATP_SNTP_STATUS_FILE                        "/var/sntp/status"
/* SNTP同步状态码 */
#define ATP_SNTP_SYNC_STATUS_CODE_DISABLE           0
#define ATP_SNTP_SYNC_STATUS_CODE_UNSYNC            1
#define ATP_SNTP_SYNC_STATUS_CODE_SYNC              2
#define ATP_SNTP_SYNC_STATUS_CODE_ERROR             3
#define ATP_SNTP_SYNC_STATUS_MAX                    4
/* end of by d00107688 增加状态报告 */

/*保存sntp进程pid号文件*/
#define ATP_SNTP_PID_FILE                           "/var/sntp/pidfile"
#define ATP_SNTP_LINE_MAX                           32


/* 目前支持设置两种格式的时区类型，一种是直接的 GMT+XX:XX 另外一种是设置国家码 */
#define SNTP_TIME_FORMAT_GMT                        1
//#define SNTP_TIME_FORMAT_COUNTRY                    2

enum ATP_SNTP_MSGTYPES_EN
{
    ATP_MSG_SNTP_SERVER = ATP_MSG_CATEGORY_DEF(ATP_MODULE_SNTP),
    ATP_MSG_SNTP_UPDATE,
    ATP_MSG_SNTP_SYS_SUCCESS,//同步成功
} ;


#ifdef SUPPORT_ATP_SNTP_DST

/* 夏令时格式 不支持J 和 N 格式 */
#define ATP_SNTP_DST_FORMAT_M                       0          /* 对应的格式为M，也就是 某个月的某个星期的星期几 */ 
//#define ATP_SNTP_DST_FORMAT_J                       1          /* 对应的格式为J,也就是从第多少天开始，包括闰年 */
//#define ATP_SNTP_DST_FORMAT_N                       2          /* 对应的格式为N，也就是从第多少天开始，不包括闰年 */

#define ATP_SNTP_DEFAULT_DST_OFFSET                 3600       /* 默认夏令时调整时间 */

/******************************************************************************
  结构名称  : ATP_SNTP_DST_ST
  结构作用  : 保存欲设置的夏令时配置信息
  调用函数  : 

  注意事项  : 目前仅支持夏令时格式为: 
******************************************************************************/
typedef struct
{
    int iDstMonth;          /* 夏令时开始月份(1-12), 
                                        注意:Linux的gmtime, localtime函数得到的struct tm 结构中tm_mon取值0-11
                                        作月份比较时要注意转换*/    
    int iWeekNum;           /* 第几个星期, 1--5 */
    int iWday;              /* 夏令时时间周天，为0-6，表示周天-周六 
                                         * 0表示星期天，1表示星期一，.... 6表示周六
                                         * 星期几
                                         */
    unsigned int         ulDstTime;         /* 夏令时从该天的哪一秒开始 0-86400(一天) */
    unsigned int ulFlag;           /* 格式标志，夏令时是 M 格式还是J格式还是N格式 */
}ATP_SNTP_DST_TIME_ST;

typedef struct
{
    unsigned int ulEnable;
    long    lDstOffsetTime;
    ATP_SNTP_DST_TIME_ST stDstStart;
    ATP_SNTP_DST_TIME_ST stDstEnd;
}
ATP_SNTP_DST_ST;

#endif




#endif // End of __ATP_SNTP_MSGTYPES_H__

