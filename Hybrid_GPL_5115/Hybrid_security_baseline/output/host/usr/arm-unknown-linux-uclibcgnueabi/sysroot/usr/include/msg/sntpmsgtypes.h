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


/* �ļ����ùؼ����б� */
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

#define SNTP_SERVER_NUM  5          /* ���֧��5�������� */

/*sntp������������ַ�������*/
#define ATP_SNTP_SERVER_MAXLEN          64

/*���з��������������ַ�����󳤶�*/
#define ATP_SNTP_ALLSERVER_LEN          (SNTP_SERVER_NUM * ATP_SNTP_SERVER_MAXLEN)

/* ����ʱ�Ƿ�ʹ�� */
#define ATP_SNTP_DST_ENABLE             1
#define ATP_SNTP_DST_DISABLE            0

/* Start by d00107688 to add DST */
#define MONTH_START_OF_YEAR                     1
#define MONTH_OF_YEAR                           12
#define DAY_OF_A_WEEK                           7
#define MAX_WEEK_OF_MONTH                       5
#define DAY_OF_WEEK_SUNDAY                      0
#define DAY_OF_WEEK_SATURDAY                    6
#define HOUR_OF_DAY                             24                                  /* һ���Сʱ�� */
#define SECOND_OF_MIN                           60                                  /* ÿ���ӵ����� */
#define SECOND_OF_HOUR                          (60 * (SECOND_OF_MIN))              /* ÿСʱ������ */
#define SECOND_OF_DAY                           ((HOUR_OF_DAY) * (SECOND_OF_HOUR))  /* һ������� */
#define NEGATIVE_SECOND_OF_DAY                  (0 - (SECOND_OF_DAY))
/* End by d00107688 to add DST */

#define IPV4_TYPE_ENABLE 1
#define IPV6_TYPE_ENABLE 2

/* start of by d00107688 ����״̬���� */
#define ATP_SNTP_STATUS_FILE                        "/var/sntp/status"
/* SNTPͬ��״̬�� */
#define ATP_SNTP_SYNC_STATUS_CODE_DISABLE           0
#define ATP_SNTP_SYNC_STATUS_CODE_UNSYNC            1
#define ATP_SNTP_SYNC_STATUS_CODE_SYNC              2
#define ATP_SNTP_SYNC_STATUS_CODE_ERROR             3
#define ATP_SNTP_SYNC_STATUS_MAX                    4
/* end of by d00107688 ����״̬���� */

/*����sntp����pid���ļ�*/
#define ATP_SNTP_PID_FILE                           "/var/sntp/pidfile"
#define ATP_SNTP_LINE_MAX                           32


/* Ŀǰ֧���������ָ�ʽ��ʱ�����ͣ�һ����ֱ�ӵ� GMT+XX:XX ����һ�������ù����� */
#define SNTP_TIME_FORMAT_GMT                        1
//#define SNTP_TIME_FORMAT_COUNTRY                    2

enum ATP_SNTP_MSGTYPES_EN
{
    ATP_MSG_SNTP_SERVER = ATP_MSG_CATEGORY_DEF(ATP_MODULE_SNTP),
    ATP_MSG_SNTP_UPDATE,
    ATP_MSG_SNTP_SYS_SUCCESS,//ͬ���ɹ�
} ;


#ifdef SUPPORT_ATP_SNTP_DST

/* ����ʱ��ʽ ��֧��J �� N ��ʽ */
#define ATP_SNTP_DST_FORMAT_M                       0          /* ��Ӧ�ĸ�ʽΪM��Ҳ���� ĳ���µ�ĳ�����ڵ����ڼ� */ 
//#define ATP_SNTP_DST_FORMAT_J                       1          /* ��Ӧ�ĸ�ʽΪJ,Ҳ���Ǵӵڶ����쿪ʼ���������� */
//#define ATP_SNTP_DST_FORMAT_N                       2          /* ��Ӧ�ĸ�ʽΪN��Ҳ���Ǵӵڶ����쿪ʼ������������ */

#define ATP_SNTP_DEFAULT_DST_OFFSET                 3600       /* Ĭ������ʱ����ʱ�� */

/******************************************************************************
  �ṹ����  : ATP_SNTP_DST_ST
  �ṹ����  : ���������õ�����ʱ������Ϣ
  ���ú���  : 

  ע������  : Ŀǰ��֧������ʱ��ʽΪ: 
******************************************************************************/
typedef struct
{
    int iDstMonth;          /* ����ʱ��ʼ�·�(1-12), 
                                        ע��:Linux��gmtime, localtime�����õ���struct tm �ṹ��tm_monȡֵ0-11
                                        ���·ݱȽ�ʱҪע��ת��*/    
    int iWeekNum;           /* �ڼ�������, 1--5 */
    int iWday;              /* ����ʱʱ�����죬Ϊ0-6����ʾ����-���� 
                                         * 0��ʾ�����죬1��ʾ����һ��.... 6��ʾ����
                                         * ���ڼ�
                                         */
    unsigned int         ulDstTime;         /* ����ʱ�Ӹ������һ�뿪ʼ 0-86400(һ��) */
    unsigned int ulFlag;           /* ��ʽ��־������ʱ�� M ��ʽ����J��ʽ����N��ʽ */
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

