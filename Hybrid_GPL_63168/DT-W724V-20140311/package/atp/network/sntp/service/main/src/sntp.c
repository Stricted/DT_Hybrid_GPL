/*
 * Simple Network Time Protocol (SNTP) Client
 *
 * Query an NTP server for a time, and set it.
 * Must be run as root to actually change the system time.
 *
 * Written by Bjorn Wesen
 * Copyright (c) 1999, 2000 Axis Communications AB
 *
 * $Id: sntp.c,v 1.24 2009/04/29 08:07:55 y42304 Exp $
 *
 * HISTORY
 * -------
 
 * Revision 1.14  2000/07/05 16:03:09  bjornw
 * Round secs correctly in set_rtc
 *
 * Revision 1.13  2000/07/05 15:45:02  bjornw
 * Also set the RTC time when doing big time-changes.
 *
 * Revision 1.12  2000/07/05 13:59:56  bjornw
 * Use adjtime instead of settimeofday for small adjustments.
 *
 * Revision 1.11  2000/07/05 09:55:55  bjornw
 * * gcc-cris needs long double instead of double to get 8-byte precision
 * * Need to cast some constants to long double to get full precision in
 *   some arithmetics
 *
 * Revision 1.10  2000/07/04 10:51:07  bjornw
 * * Now uses the full sub-second resolution timing in SNTP
 * * Improved logging (turned on syslog in daemon mode)
 * * Some indentation fixes
 *
 * Revision 1.9  2000/02/15 12:14:59  finn
 * main: If something went wrong in the process of either sending or receiving
 * an NTP message, the counter `retries' was increased by one. However, this
 * counter was never reset to zero between every try, hence sntpdate could
 * only fail five times totally.
 *
 * Revision 1.8  1999/12/15 08:21:02  torbjore
 * * Now uses sntpdate.conf instead of parhand for configuration
 *
 *
 */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <syslog.h>
#include <netdb.h>
#include <signal.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <net/if.h>

#include "msgapi.h"
#include "lgplutil.h"
#include "atplog.h"
#include "sntp.h"

#undef USE_VTOP_TIMER

#ifdef USE_VTOP_TIMER
#include "os_tmr_interface.h"
#endif

#ifdef SUPPORT_ATP_SNTP_ADD_LOG
unsigned int ulSynStatForLog = 0;  //0代表开始，1代表同步失败， 2代表同步成功
#endif

extern int errno;

//#include <asm/rtc.h>  /* for the RTC_SET_TIME ioctl */

/* the NTP timestamp is the number of seconds since 0h, 1 January 1900. The
 * UNIX timeval started 70 years later (0h, 1 January 1970). The difference:
 */
const unsigned long UNIX_NTP_DIFF = 2208988800U;

const ntp_double NTP_SCALE = 4294967296.0;            /* 2^32, of course! */
const ntp_double NTP_SCALE_I = 2.3283064365e-10;      /* 2^-32 */

const long MILLION_L = 1000000;          /* For conversion to/from timeval */
const ntp_double MILLION_D = 1.0e6;          /* Must be equal to MILLION_L */
const ntp_double MILLION_D_I = 1.0e-6;       /* Must be equal to 1 / MILLION_L */

#ifdef SUPPORT_ATP_SNTP_DST
static const int        aacDay_Of_Per_Month[2][MONTH_OF_YEAR + 1] = {
        {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        {-1, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

#define START_YEAR          1900
#define REFER_YEAR          1900        /* 参考年份 */
#define REFER_WEEKDAY       1           /* 参考星期 */
#define NTP_ANS_LEAP_INDICATION     0xc0000000

static const int acDayOfYear[2] = {365, 366};

#define IsLeap(year) (((year) % 4) == 0 && (((year) % 100) != 0 || ((year) % 400) == 0))

static int JudgeJumpYear(ATP_SNTP_DST_TIME_ST *pstDstStart, ATP_SNTP_DST_TIME_ST *pstDstEnd);

static int GetRealDateFromDst(unsigned int ulYear, const ATP_SNTP_DST_TIME_ST stDstTime, SNTP_DST_REAL_ST *pstResult);

static int InDstTime(struct tm *pstTm, SNTP_DST_REAL_ST *pstDstStart, SNTP_DST_REAL_ST *pstDstEnd);
#endif

static void set_rtc(struct timeval *);

static void SntpParseServer(char* pszServers);

static unsigned char ucSyncStatus = ATP_SNTP_SYNC_STATUS_CODE_UNSYNC; 
static void WriteStatus(unsigned char ulStauts);

static void WriteOffset2Kernel(int utc_offset);

static int as_daemon = 0;  /* "daemon" or not. */

static NTP_SERVER g_stSntpServer[SNTP_SERVER_NUM];
static long g_lFailInterval = FAIL_INTERVAL;             /* 更新失败时尝试的间隔 */
static long g_lSuccessIntervalSec = SUCCESS_INTERVAL;   /* 更新成功时再次更新的间隔 */
static int g_lSuccessIntervalMicroSec = 0;
static int g_lFailIntervalMicroSec = 0;                 /* 同步失败毫秒数 */
static int g_lNtpFamily = AF_INET;       // SNTP server IP type

#ifdef  SUPPORT_ATP_TR098
static int connectNTPSrvTimeOutCnt = 0;       // when connect to the NTP server, if select 2s time out, then next time try connect time will minus 2s
#endif

static int connectNTPSrvTimeOutFlag = 0;       // when connect to the NTP server, if select 2s time out, then next time try connect time will minus 2s
static int failLogCnt = 0;

#ifdef SUPPORT_ATP_LOG_VOICE_TYPE
static int hasT102Log = 0;
#endif

//lint -save -e{24, 35, 40, 63}
#ifdef SUPPORT_ATP_SNTP_DST
ATP_SNTP_DST_ST g_stDstTz = {
    .ulEnable = 0,
};  
#endif
//lint -restore

static volatile int check_config = 0;

static void catch_signal(int sig)
{
    check_config = 1;
    signal(SIGUSR1, catch_signal);  /* retrig it */
}

long SntpTimeDiff(struct timeval *starttime, struct timeval *finishtime)
{
    long usec;
    usec = (finishtime->tv_sec - starttime->tv_sec) * 1000 * 1000;
    usec += (finishtime->tv_usec - starttime->tv_usec);
    return usec;
}


#ifdef SUPPORT_ATP_SNTP_BT
/* 非root权限运行 */
void SntpDropRootPrivileges()
{
    char* pszUser = "sntp";
    gid_t agit[2] = {ATP_GROUP_ID_FS_RW, 0};    
    struct passwd *pw = NULL;

    pw = getpwnam(pszUser);
    if (pw) 
    {
        if ((initgroups(pszUser, pw->pw_gid) != 0) 
            || (setgroups(1, agit) != 0)
            || (setgid(pw->pw_gid) != 0)
            || (seteuid(pw->pw_uid) != 0))
        {
            DEBUG_PRINTF("Change to %s uid=%d gid=%d failed.\n", 
                pszUser, pw->pw_uid, pw->pw_gid);
        }
    }
    else
    {
        DEBUG_PRINTF("No user %s\n", pszUser);
    }
}

/* 临时提升为root权限 */
void SntpRootPrivileges()
{
    if (seteuid(0) != 0)
    {
        DEBUG_PRINTF("Sntp root privileges failed.\n");
    }
}
#endif
static int SavePid(void)
{
    FILE *pstPidFile = NULL;
    int lPid = 0;

    ATP_UTIL_ForkProcess("mkdir -p /var/sntp", -1, NULL, NULL); //l00168621
    
    pstPidFile = fopen(SNTP_PID_FILE, "w");
    if (NULL != pstPidFile) 
    {
        lPid = getpid();
        printf("\r\nCurrent sntp process is %d!\r\n", lPid);
    	fprintf(pstPidFile, "%d\n", lPid);
    	fclose(pstPidFile);
    }

    return 1;
}

static void set_offset(int lOffset)
{
    FILE *pstFile = NULL;
    int lRet = 0;
    char acCmd[128] = {0};
    
	pstFile = fopen("/var/sntp/offset", "r");
	if (NULL != pstFile) 
	{
		fgets(acCmd, 128, pstFile);
		lRet = atoi(acCmd);
		fclose(pstFile);
	}

    snprintf(acCmd, sizeof(acCmd), "echo %d > /var/sntp/offset", lOffset + lRet);
    ATP_UTIL_ForkProcess(acCmd, -1, NULL, NULL); 
}

static int SntpGetLocalIp(int* localip)
{
    FILE *fp = NULL;
    char line[16] = {0};
    struct in_addr localaddr;
    
    fp = fopen("/var/sntplocaladdr", "r");
    if(fp != NULL)
    {
        fgets(line, sizeof(line), fp);
        fclose(fp);
        if(inet_aton(line, &localaddr))
        {
            *localip = localaddr.s_addr;
            SntpDebug("get local ip [%x]\n", *localip);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    return 1;
}

static void NotifySntpUpdateToOther(int lOffset)
{
#ifdef SUPPORT_ATP_VOICE
	if (VOS_OK != ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME,
										ATP_MSG_SNTP_UPDATE,
										(VOS_UINT32)(lOffset)))
	{
		printf("\r\n SNTP sends msg to cms ERROR !!! \r\n");
	}
#endif
}

static int g_lSrcActive = 0;                 
static int g_lZoneDiff = 0; 
int utc_offset = 0;
int refreshinterval = 5;              /*所有服务器同步失败到下次同步的时间间隔*/


int start_connection = 0;
int setup_connection = 0;
static int lFlg = 0;           /* 标识是否更新成功 */

#ifdef SUPPORT_ATP_SNTP_DST
static void SntpLoadDstCfgFromFile( char* param , char* value)
{
    if (!strncmp(SNTP_CONF_DST_ENABLE, param, strlen(SNTP_CONF_DST_ENABLE)))
    {
        g_stDstTz.ulEnable = (unsigned int)(atoi(value));
        if ((g_stDstTz.ulEnable != ATP_SNTP_DST_DISABLE) && (g_stDstTz.ulEnable != ATP_SNTP_DST_ENABLE))
        {
            SntpDebug("Get the Enable status of  DST error");
            g_stDstTz.ulEnable = ATP_SNTP_DST_DISABLE;
        }
        SntpDebug("Get the DstEnable:   %u", g_stDstTz.ulEnable);
    }
    /* 第几个月 */
    else if (!strncmp(SNTP_CONF_DST_START_MONTH, param, strlen(SNTP_CONF_DST_START_MONTH)))
    {
        g_stDstTz.stDstStart.iDstMonth = atoi(value);
        if ((g_stDstTz.stDstStart.iDstMonth < 1) 
            || (g_stDstTz.stDstStart.iDstMonth > MONTH_OF_YEAR))
        {
            SntpDebug("Get the month of start DST error");
            g_stDstTz.stDstStart.iDstMonth = 0;
        }
        SntpDebug("Get the DstMonth: %d", g_stDstTz.stDstStart.iDstMonth);
    }
    /* 第几个星期 */
    else if (!strncmp(SNTP_CONF_DST_START_WEEK_NUM, param, strlen(SNTP_CONF_DST_START_WEEK_NUM)))
    {
        g_stDstTz.stDstStart.iWeekNum = atoi(value);
        if ((g_stDstTz.stDstStart.iWeekNum < 1) 
            || (g_stDstTz.stDstStart.iWeekNum > MAX_WEEK_OF_MONTH))
        {
            SntpDebug("Get the DstStartDay of start DST error");
            g_stDstTz.stDstStart.iWeekNum = 0;
        }
        SntpDebug("Get the DstStartDay: %d", g_stDstTz.stDstStart.iWeekNum);
    }
    /* 星期几 */
    else if (!strncmp(SNTP_CONF_DST_START_WDAY, param, strlen(SNTP_CONF_DST_START_WDAY)))
    {
        g_stDstTz.stDstStart.iWday = atoi(value);
        if ((g_stDstTz.stDstStart.iWday < DAY_OF_WEEK_SUNDAY) 
        || (g_stDstTz.stDstStart.iWday > DAY_OF_WEEK_SATURDAY))
        {
            SntpDebug("Get the DstStartWday of start DST error");
            g_stDstTz.stDstStart.iWday = 0;
        }
        SntpDebug("Get the DstStartWday: %d", g_stDstTz.stDstStart.iWday);
    }
    /* 哪个小时分钟秒 */
    else if (!strncmp(SNTP_CONF_DST_START_TIME, param, strlen(SNTP_CONF_DST_START_TIME)))
    {
        g_stDstTz.stDstStart.ulDstTime = atol(value);
        if (g_stDstTz.stDstStart.ulDstTime >= SECOND_OF_DAY)
        {
            SntpDebug("Get the DstStartTime of start DST error");
            g_stDstTz.stDstStart.ulDstTime = 0;
        }
        SntpDebug("Get the DstStartTime: %u", g_stDstTz.stDstStart.ulDstTime);
    }
    else if (!strncmp(SNTP_CONF_DST_START_FLAG, param, strlen(SNTP_CONF_DST_START_FLAG)))
    {
        g_stDstTz.stDstStart.ulFlag = (unsigned int)atoi(value);
        if (g_stDstTz.stDstStart.ulFlag != ATP_SNTP_DST_FORMAT_M) 
        {
            SntpDebug("Get the DstStartFlag of start DST error");
            g_stDstTz.stDstStart.ulFlag = ATP_SNTP_DST_FORMAT_M;
        }
        SntpDebug("Get the DstStartFlag: %u", g_stDstTz.stDstStart.ulFlag);
    }

    /* 结束时间: 第几个月 */
    else if (!strncmp(SNTP_CONF_DST_END_MONTH, param, strlen(SNTP_CONF_DST_END_MONTH)))
    {
        g_stDstTz.stDstEnd.iDstMonth = atoi(value);
        if ((g_stDstTz.stDstEnd.iDstMonth < 1) 
        || (g_stDstTz.stDstEnd.iDstMonth > MONTH_OF_YEAR))
        {
            SntpDebug("Get the month of DstEndMonth DST error");
            g_stDstTz.stDstEnd.iDstMonth = 0;
        }
        SntpDebug("Get the DstEndMonth: %d", g_stDstTz.stDstEnd.iDstMonth);
    }
    /* 第几个星期 */
    else if (!strncmp(SNTP_CONF_DST_END_WEEK_NUM, param, strlen(SNTP_CONF_DST_END_WEEK_NUM)))
    {
        g_stDstTz.stDstEnd.iWeekNum = atoi(value);
        if ((g_stDstTz.stDstEnd.iWeekNum < 1) 
        || (g_stDstTz.stDstEnd.iWeekNum > MAX_WEEK_OF_MONTH))
        {
            SntpDebug("Get the DstEndDay of start DST error");
            g_stDstTz.stDstEnd.iWeekNum = 0;
        }
        SntpDebug("Get the DstEndDay: %d", g_stDstTz.stDstEnd.iWeekNum);
    }
    /* 星期几 */
    else if (!strncmp(SNTP_CONF_DST_END_WDAY, param, strlen(SNTP_CONF_DST_END_WDAY)))
    {
        g_stDstTz.stDstEnd.iWday = atoi(value);
        if ((g_stDstTz.stDstEnd.iWday < DAY_OF_WEEK_SUNDAY) 
        || (g_stDstTz.stDstEnd.iWday > DAY_OF_WEEK_SATURDAY))
        {
            SntpDebug("Get the DstEndWday of start DST error");
            g_stDstTz.stDstEnd.iWday = 0;
        }
        SntpDebug("Get the DstEndWday: %d", g_stDstTz.stDstEnd.iWday);
    }
    /* 哪一个时间点 */
    else if (!strncmp(SNTP_CONF_DST_END_TIME, param, strlen(SNTP_CONF_DST_END_TIME)))
    {
        g_stDstTz.stDstEnd.ulDstTime = atol(value);
        if (g_stDstTz.stDstEnd.ulDstTime >= SECOND_OF_DAY)
        {
            SntpDebug("Get the DstEndTime of start DST error");
            g_stDstTz.stDstEnd.ulDstTime = 0;
        }
        SntpDebug("Get the DstEndTime: %u", g_stDstTz.stDstEnd.ulDstTime);
    }
    else if (!strncmp(SNTP_CONF_DST_END_FLAG, param, strlen(SNTP_CONF_DST_END_FLAG)))
    {
        g_stDstTz.stDstEnd.ulFlag = (unsigned int)atoi(value);
        if (g_stDstTz.stDstEnd.ulFlag != ATP_SNTP_DST_FORMAT_M)
        {
            SntpDebug("Get the DstEndFlag of start DST error");
            g_stDstTz.stDstEnd.ulFlag = ATP_SNTP_DST_FORMAT_M;
        }
        SntpDebug("Get the DstEndFlag: %u", g_stDstTz.stDstEnd.ulFlag);
    }
    else if (!strncmp(SNTP_CONF_DST_OFFSET_TIME, param, strlen(SNTP_CONF_DST_OFFSET_TIME)))
    {
        g_stDstTz.lDstOffsetTime = atol(value);
        if ((g_stDstTz.lDstOffsetTime <= NEGATIVE_SECOND_OF_DAY) 
        || (g_stDstTz.lDstOffsetTime >= SECOND_OF_DAY))
        {
            SntpDebug("Get the DstOffsetTime of  DST error");
            g_stDstTz.lDstOffsetTime = ATP_SNTP_DEFAULT_DST_OFFSET;
        }
        SntpDebug("Get the DstOffsetTime: %ld", g_stDstTz.lDstOffsetTime);
    }
}

/*标记与NTP服务器对时校正后的本地时间是否在夏令时区段内,1=在，0=不在*/
static int IsLastInDstPeriod = 0;
static void SntpSetDST(struct timeval* new, struct timeval* adjust)
{
    struct tm *pstTm = NULL;
    SNTP_DST_REAL_ST stDstRealStart, stDstRealEnd;
    SntpDebug("Begin to do DST \n");

    if (ATP_SNTP_DST_ENABLE == g_stDstTz.ulEnable)
    {
        SntpDebug("SNTP DST enable\n");

        pstTm = gmtime(&new->tv_sec);
        if (pstTm == NULL)
        {
            SntpDebug("Sorry, gmtime error: %s\n", strerror(errno));
        }

        if (ATP_SNTP_DST_ENABLE == g_stDstTz.ulEnable)
        {
            SntpDebug("all the value config success\n");
            SntpDebug("pstTm->tm_year: %d", pstTm->tm_year);

            if (pstTm != NULL)
            {
                /* 算出夏令时具体对应着该年的几月几号 */
                GetRealDateFromDst((pstTm->tm_year + START_YEAR), 
                                                g_stDstTz.stDstStart, &stDstRealStart);
                if (0 == JudgeJumpYear(&g_stDstTz.stDstStart, &g_stDstTz.stDstEnd))
                {
                    GetRealDateFromDst((pstTm->tm_year + START_YEAR), 
                                                    g_stDstTz.stDstEnd, &stDstRealEnd);
                }
                else
                {
                    /*夏令时结束日期是下一年的*/
                    GetRealDateFromDst((pstTm->tm_year + START_YEAR + 1), 
                                                    g_stDstTz.stDstEnd, &stDstRealEnd);
                }
            }
            else
            {
                GetRealDateFromDst(2008, g_stDstTz.stDstStart, &stDstRealStart);
                GetRealDateFromDst(2008, g_stDstTz.stDstEnd, &stDstRealEnd);
            }

            if (NULL != pstTm)
            {
                if (1 == InDstTime(pstTm, &stDstRealStart, &stDstRealEnd))
                {
                    new->tv_sec += g_stDstTz.lDstOffsetTime;
                    adjust->tv_sec += g_stDstTz.lDstOffsetTime;
                    utc_offset += g_stDstTz.lDstOffsetTime;
                    IsLastInDstPeriod = 1;
                }
                else
                {
                    IsLastInDstPeriod = 0;
                    SntpDebug("No need to adjust the time");
                }
            }
        }

    }
}        

static void SntpUpdateDST()
{
    struct timeval  tmNow;
    struct tm *pstTmNow = NULL;
    int DstStatNow = 0;

    if (ATP_SNTP_DST_ENABLE == g_stDstTz.ulEnable)
    {
        gettimeofday(&tmNow, NULL);
        pstTmNow = localtime(&tmNow.tv_sec);

        if (NULL == pstTmNow)
        {
            return;
        }
        DstStatNow = pstTmNow->tm_isdst;
        /*上次对时是夏令时，此时不在夏令时区间内了，
        或者上次对时不在夏令时内，此时到了夏令时，这两种情况要更新*/
        if ((1 == IsLastInDstPeriod) && (1 != DstStatNow))
        {
            WriteOffset2Kernel(utc_offset - g_stDstTz.lDstOffsetTime);
            IsLastInDstPeriod = 0;
            
            ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, ATP_MSG_SNTP_DST_CHANGE, VOS_FALSE);
        }
        else if ((0 == IsLastInDstPeriod) && (1 == DstStatNow))
        {
            WriteOffset2Kernel(utc_offset + g_stDstTz.lDstOffsetTime);
            IsLastInDstPeriod = 1;
            
            ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, ATP_MSG_SNTP_DST_CHANGE, VOS_TRUE);
        }
    }
}


/* End by d00107688 to add DST  */
#endif    

static unsigned int  SntpSetCfg(char* time_zone, char* time_source)
{
    int missing = 0;
    
    if (strlen(g_stSntpServer[0].acServer))
    {
        SntpDebug("NTP server = [%s]\n", g_stSntpServer[0].acServer);
        SntpDebug("NTP server = [%s]\n", g_stSntpServer[1].acServer);
        SntpDebug("NTP server = [%s]\n", g_stSntpServer[2].acServer);
        SntpDebug("NTP server = [%s]\n", g_stSntpServer[3].acServer);
        SntpDebug("NTP server = [%s]\n", g_stSntpServer[4].acServer);
    }
    else
    {
        missing++;
    }

    if (time_source)
    {
        if (strcmp("NTP",time_source) == 0) 
        {
            //ntp_active = 1;
            g_lSrcActive = 1;
        }
        SntpDebug("Time Sync Source %s", time_source);
    }
    else
    {
        missing++;
    }

    if (time_zone)
    {
        int tmp_time = 0;
        //zone_diff = 0;
        g_lZoneDiff = 0;

        if (!strncmp ("NONE", time_zone, strlen ("NONE")))
        {
            SntpDebug("No Time Zone correction!\n");
        }
        else if (!strncmp ("GMT", time_zone, strlen ("GMT")))
        {
            /* first part correct */
            if (strlen(time_zone) == 3)
            {
                tmp_time = 0;
            }
            else if (time_zone[3] == '+')
            {
                tmp_time = 1;
            }
            else if (time_zone[3] == '-')
            {
                tmp_time = -1;
            }
            else
            {
                SntpDebug("Time Zone corrupt");
            }

            if (tmp_time != 0)
            {
                switch (strlen(time_zone))
                {
                    case 5: /* GMT-x */
                    if (isdigit(time_zone[4]))
                    {
                        tmp_time*=(time_zone[4]-'0')*3600;
                    }
                    break;
                    case 6: /* GMT-xx */
                    if ((isdigit(time_zone[4])) &&
                        (isdigit(time_zone[5])))
                    {
                        tmp_time *=	(time_zone[4]-'0')*36000+(time_zone[5]-'0')*3600;
                    }
                    break;
                    case 7: /* GMT-x.x */
                    if ((isdigit(time_zone[4])) &&
                        (time_zone[5] == '.') &&
                        (isdigit(time_zone[6])))
                    {
                        tmp_time*= (time_zone[4]-'0')*3600+(time_zone[6]-'0')*360;
                    }
                    break;
                    case 8: /* GMT-xx.x */
                    if ((isdigit(time_zone[4])) &&
                        (isdigit(time_zone[5])) &&
                        (time_zone[6] == '.') &&
                        (isdigit(time_zone[7])))
                    {
                        tmp_time*=
                            (time_zone[4]-'0')*36000 +
                            (time_zone[5]-'0')*3600 +
                            (time_zone[7]-'0')*360;
                    }
                    break;
                    case 9: /* GMT-xx:xx */
                    if ((isdigit(time_zone[4])) &&
                        (isdigit(time_zone[5])) &&
                        (time_zone[6] == ':') &&
                        (isdigit(time_zone[7])) &&
                        (isdigit(time_zone[8])))
                    {
                        tmp_time*=
                            (time_zone[4]-'0')*36000 +
                            (time_zone[5]-'0')*3600 +
                            (time_zone[7]-'0')*600 +
                            (time_zone[8]-'0')*60;
                    }
                    break;
                    default: /* error */
                    break;
                }
            }
            //zone_diff = tmp_time;
                  //SntpDebug("Zone Diff = %d", zone_diff);
            g_lZoneDiff = tmp_time;
            SntpDebug("Zone Diff = %d", g_lZoneDiff);                    
        }
    }
    else
    {
        missing++;
    }

    if (missing)
    {
        /* start of add by d00107688 增加状态同步 */
        ucSyncStatus = ATP_SNTP_SYNC_STATUS_CODE_UNSYNC;
        WriteStatus(ucSyncStatus);
        /* end of add by d00107688 增加状态同步 */

        SntpDebug("NTP config not complete!");
        check_config = 0;
    }
    else
    {
        check_config = 0;
    }

    return (!missing);
}

static unsigned int SntpLoadCfgFromFile()
{
    int fd;
    char fdata[2048];
    char* param = NULL;
    char* value = NULL;
    char* pszTmp = NULL;
    char acSec[32] = {0};
    char acMicroSec[8] = {0};
    int lLen;
    int cnt;
    int cnt2;
    int cnt3;
    int cnt4;
    int scnt;
    int tcnt;
	int ret = 0;

    char* time_zone = NULL;
    char* time_source = NULL;
    char* succ_syn = NULL;
    char* fail_sw = NULL;
    
    check_config = 0;

    fd = open (NTPDATE_FILE, O_RDONLY);
    if (fd >= 0)
    {
        memset(fdata,0,2048);
        tcnt=read(fd,fdata,2047);
        if (tcnt == 2047)
        {
            SntpDebug("warning! conf-file too BIG!");
        }
        close(fd);
        cnt=0;
        scnt=0;
        while (scnt < tcnt)
        {
            while (cnt < tcnt)
            {
                if ((fdata[cnt] == '\0') ||(fdata[cnt] == '\r') ||(fdata[cnt] == '\n') )
                {
                    fdata[cnt]='\0';
                    break;
                }
                cnt++;
            }
            cnt2 = scnt;
            cnt4 = scnt;
            cnt3 = 0;
            while (cnt2 < cnt)
            {
                if ((fdata[cnt2] == ' ') ||(fdata[cnt2 + 1] == '\0'))
                {
                    if (fdata[cnt2] == ' ')
                    {
                        fdata[cnt2] = '\0';
                    }
                    switch (cnt3)
                    {
                    case 0:
                        param = &fdata[cnt4];
                        break;
                    case 1:
                        value = &fdata[cnt4];
                        break;
                    default:
                        break;
                    }
                    cnt3++;
                    cnt4 = cnt2 + 1;
                }
                cnt2++;
            }

            if (cnt3 == 2)
            {
                if (!param)
                {
                    SntpDebug("error: param null");
                    goto Exit;
                }

                /* found three parts on this line */
                if (!strncmp (SNTP_CONF_TIME_ZONE, param, strlen(SNTP_CONF_TIME_ZONE)))
                {
                    if (time_zone)
                    {
                        free(time_zone);
                    }
                    time_zone = strdup(value);
                }
                else if (!strncmp (SNTP_CONF_TIME_ZONE_SRC, param, strlen(SNTP_CONF_TIME_ZONE_SRC)))
                {
                    if (time_source)
                    {
                        free(time_source);
                    }
                    time_source = strdup(value);
                }
                else if (!strncmp (SNTP_CONF_SERVER, param, strlen(SNTP_CONF_SERVER)))
                {
                    /*if (ntpserver) free(ntpserver);
                    ntpserver = strdup(value);*/
                    SntpParseServer(value);
                }
                else if (!strncmp (SNTP_CONF_FAILED_INTERVAL, param, strlen(SNTP_CONF_FAILED_INTERVAL)))
                {
                    if (fail_sw)
                    {
                        free(fail_sw);
                    }
                    fail_sw = strdup(value);
                    if (NULL != (pszTmp = strchr(fail_sw, '.')))
                    {
                        lLen = pszTmp - fail_sw;
                        if (lLen > 0)
                        {
                            if ((unsigned int)lLen >= sizeof(acSec))
                            {
                                memcpy(acSec, fail_sw, (sizeof(acSec) - 1));
                                acSec[(sizeof(acSec) - 1)] = '\0';
                            }
                            else
                            {
                                memcpy(acSec, fail_sw, lLen);
                                acSec[lLen] = '\0';
                            }
                            /* 获取小数点前的秒数 */
                            g_lFailInterval = atol(acSec);
                        }
                        else
                        {
                            g_lFailInterval = FAIL_INTERVAL;
                        }

                        /* 计算小数点后的毫秒数 */
                        fail_sw = fail_sw + lLen + 1;
                        if (fail_sw != NULL)
                        {
                            switch(strlen(fail_sw))
                            {
                                case 0:
                                    g_lFailIntervalMicroSec = 0;
                                    break;
                                case 1:
                                    g_lFailIntervalMicroSec = atoi(fail_sw) * 100;
                                    break;
                                case 2:
                                    g_lFailIntervalMicroSec = atoi(fail_sw) * 10;
                                    break;
                                case 3:
                                    g_lFailIntervalMicroSec = atoi(fail_sw);
                                    break;
                                default:
                                memcpy(acMicroSec, fail_sw, 3);
                                acMicroSec[3] = '\0';
                                g_lFailIntervalMicroSec = atoi(acMicroSec);
                            }
                            SntpDebug("\r\ng_lFailIntervalSec: [%ld], g_lFailIntervalMicroSec: [%d]\r\n", g_lFailInterval, g_lFailIntervalMicroSec);
                        }
                    }
                    else
                    {
                        g_lFailInterval = atol(fail_sw);
                    }
                    if ( (g_lFailInterval == 0) && (g_lFailIntervalMicroSec == 0))
                    {
                        g_lFailInterval = FAIL_INTERVAL;
                    }
                }             
                else if (!strncmp (SNTP_CONF_SUCCED_INTERVAL, param, strlen(SNTP_CONF_SUCCED_INTERVAL)))
                {
                    if (succ_syn)
                    {
                        free(succ_syn);
                    }
                    succ_syn = strdup(value);
                    if (NULL != (pszTmp = strchr(succ_syn, '.')))
                    {
                        lLen = pszTmp - succ_syn;
                        if (lLen > 0)
                        {
                            if ((unsigned int)lLen >= sizeof(acSec))
                            {
                                memcpy(acSec, succ_syn, (sizeof(acSec) - 1));
                                acSec[(sizeof(acSec) - 1)] = '\0';
                            }
                            else
                            {
                                memcpy(acSec, succ_syn, lLen);
                                acSec[lLen] = '\0';
                            }
                            g_lSuccessIntervalSec = atol(acSec);
                        }
                        else
                        {
                            g_lSuccessIntervalSec = SUCCESS_INTERVAL;
                        }
                        /* 过滤小数点 */
                        succ_syn = succ_syn + lLen + 1;

                        /* 不管怎么样，默认就是三位数的大小 */
                        switch (strlen(succ_syn))
                        {
                            case 0:
                            g_lSuccessIntervalMicroSec = 0;
                            break;
                            case 1:
                            g_lSuccessIntervalMicroSec = atoi(succ_syn) * 100;
                            break;
                            case 2:
                            g_lSuccessIntervalMicroSec = atoi(succ_syn) * 10;
                            break;
                            case 3:
                            g_lSuccessIntervalMicroSec = atoi(succ_syn);
                            break;
                            default:
                            memcpy(acMicroSec, succ_syn, 3);
                            acMicroSec[3] = '\0';
                            g_lSuccessIntervalMicroSec = atoi(acMicroSec);
                            break;
                        }
                        SntpDebug("\r\ng_lSuccessIntervalSec: [%ld], g_lSuccessIntervalMicroSec: [%d]\r\n", g_lSuccessIntervalSec, g_lSuccessIntervalMicroSec);
                    }
                    else
                    {
                        g_lSuccessIntervalSec = atol(succ_syn);
                    }
                    if ((g_lSuccessIntervalSec == 0) && (g_lSuccessIntervalMicroSec == 0))
                    {
                        g_lSuccessIntervalSec = SUCCESS_INTERVAL;
                    }
                }
#ifdef SUPPORT_ATP_SNTP_DST                
                else
                {
                    SntpLoadDstCfgFromFile( param ,  value);
                }
#endif                
            }
            scnt = ++cnt;
        }
        //close(fd);
    }
    else
    {
        SntpDebug("error when opening config file");
    }

	ret = SntpSetCfg( time_zone,  time_source);
	free(time_zone);
	free(time_source);

    return ret;

Exit:
    SntpDebug("Exiting...");

    ATP_MSG_Destroy();

    exit(0);    
}

static unsigned int SntpSetupConnection(char *ntpserver, int *ntpofd)
{
    struct sockaddr_in addr;
    char czTem[128];
    int localip;
    long lIpv6Flag;
    long lIpv4Flag;
    int ret;
    struct sockaddr_in6 stIpv6Addr;
    struct in_addr      in;
    struct hostent *hp = NULL;
    struct hostent *hpv6 = NULL;    
    addr.sin_port = htons(NTPPORT);
    lIpv6Flag = 0;
    lIpv4Flag = 0;
    int i = 0;
    
    memset(&in        ,0,sizeof(struct in_addr));
    memset(&stIpv6Addr,0,sizeof(struct sockaddr_in6));
    memset(czTem, 0, sizeof(czTem));
    
    if ( 0 < inet_pton(AF_INET6,ntpserver,&stIpv6Addr.sin6_addr) )
    {
        /* The address type is IPv6. */
        SntpDebug("\r\n The address type is ipv6 . Address is %s . FILE[%s] LINE[%u] \r\n",ntpserver,__FILE__,__LINE__);
        if ((VOS_TRUE == ATP_UTIL_IsWanV6Up()) || (VOS_TRUE == ATP_UTIL_IsLTEWanV6Up())) 
        {
            lIpv6Flag = 1;
        }
    }
    else if ( inet_aton(ntpserver, &in) )
    {
        /* resolve SNTP hostname */
        SntpDebug("\r\nThe function(gethostbyname) start to run!\r\n");
        hp = atp_gethostbyname(ntpserver,0);
        if (!hp)
        {
            SntpDebug("resolve %s failed.\n", ntpserver);
            localip = 0;
            if (SntpGetLocalIp(&localip))
            {
                hp = atp_gethostbyname(ntpserver, localip);
                if (!hp)
                {
                    SntpDebug("atp resolve %s failed.\n", ntpserver);
                }
            }
        }
        else if ((VOS_TRUE == ATP_UTIL_IsWanV4Up()) || (VOS_TRUE == ATP_UTIL_IsLTEWanV4Up()))
        {
            lIpv4Flag = 1;
        }
        SntpDebug("\r\nThe function(gethostbyname) end!\r\n");
    }
    else  //默认先v6,后v4
    {
#if 0
        //else if (NULL != (hp = ATP_UTIL_Gethostbyname(ntpserver,AF_INET6,NULL)) && 0 == lIpv6DnsFlag )
        if(g_lIpTypeFlag & IPV6_TYPE_ENABLE)
        {
            SntpDebug("\r\n v6 up now, g_lIpTypeFlag is %d . FILE[%s] LINE[%u] \r\n", g_lIpTypeFlag, __FILE__,__LINE__);
            if (NULL != (hp = ATP_UTIL_Gethostbyname(ntpserver,AF_INET6,NULL))) 
            {
                /* IPv6 parse DNS  */
                lIpv6Flag = 1;
                memcpy(&stIpv6Addr.sin6_addr.s6_addr,(char*)*hp->h_addr_list,16);
                inet_ntop(AF_INET6,stIpv6Addr.sin6_addr.s6_addr,czTem,sizeof(stIpv6Addr.sin6_addr.s6_addr));
                SntpDebug("\r\n Parse ipv6 dns address : %s . FILE[%s] LINE[%u] \r\n",czTem,__FILE__,__LINE__);

                //lServerNum   = k;
                //lIpv6DnsFlag = 1;
            }
            else
            {
                SntpDebug("\r\n v6 up now, ATP_UTIL_Gethostbyname failed . FILE[%s] LINE[%u] \r\n", __FILE__,__LINE__);
            }
        }
        if((g_lIpTypeFlag & IPV4_TYPE_ENABLE) && NULL == hp)
        {   
            //if ( k == lServerNum && 1 == lIpv6DnsFlag )
            //{
            /* 解决同一DNS同时支持IPv6和IPv4情况,该情况下进行交替解析和请求. */
            //    lIpv6DnsFlag = 0;
            //}

            /* resolve SNTP hostname */
            SntpDebug("\r\n v4 up now, g_lIpTypeFlag is %d. FILE[%s] LINE[%u] \r\n",g_lIpTypeFlag, __FILE__,__LINE__);
            hp = ATP_UTIL_Gethostbyname(ntpserver,AF_INET,NULL);
            SntpDebug("\r\n v4 up now, begin to ATP_UTIL_Gethostbyname. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
            if (!hp)
            {
                SntpDebug("\r\n v4 up now, ATP_UTIL_Gethostbyname failed, try again SntpGetLocalIp. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
                SntpDebug("resolve %s failed.\n", ntpserver);
                localip = 0;
                if (SntpGetLocalIp(&localip))
                {
                    SntpDebug("\r\n v4 up now, get in SntpGetLocalIp. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
                    hp = atp_gethostbyname(ntpserver, localip);
                    SntpDebug("\r\n v4 up now, begin to atp_gethostbyname . FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
                    if (!hp)
                    {
                        SntpDebug("\r\n v4 up now,  atp_gethostbyname failed. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
                        SntpDebug("atp resolve %s failed.\n", ntpserver);
                    }
                }
                else
                {
                    SntpDebug("\r\n v4 up now, get SntpGetLocalIp failed. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
                }
            }
            else
            {
                SntpDebug("\r\n v4 up now, ATP_UTIL_Gethostbyname success. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
            }
            SntpDebug("\r\nThe function(gethostbyname) end!\r\n");
        }
#endif
	
        ret = ATP_UTIL_Lookup_A_and_AAAA(ntpserver, &hp, &hpv6);	
        if ((NULL != hpv6)
        && ((VOS_TRUE == ATP_UTIL_IsWanV6Up()) || (VOS_TRUE == ATP_UTIL_IsLTEWanV6Up()) 
            || (ATP_UTIL_IsLTETunnel6Up())))
        {
            for (i = 0;(!ATP_UTIL_IsExistDefaultRoute(VOS_TRUE)) && i < SNTP_SERVER_WAITROUTE_TIMES; i++)
            {
                SntpDebug("\r\ncheck route6 try[%d]", i);
                usleep(500000);
            }
            /* IPv6 parse DNS  */
            lIpv6Flag = 1;
            memcpy(&stIpv6Addr.sin6_addr.s6_addr,(char*)*hpv6->h_addr_list,16);
            inet_ntop(AF_INET6,stIpv6Addr.sin6_addr.s6_addr,czTem,sizeof(stIpv6Addr.sin6_addr.s6_addr));
            SntpDebug("\r\n Parse ipv6 dns address : %s . FILE[%s] LINE[%u] \r\n",czTem,__FILE__,__LINE__);  
        }
        else if ((NULL != hp)
        && ((VOS_TRUE == ATP_UTIL_IsWanV4Up()) || (VOS_TRUE == ATP_UTIL_IsLTEWanV4Up()) 
            || (ATP_UTIL_IsLTETunnel4Up())))
        {
            for (i = 0;(!ATP_UTIL_IsExistDefaultRoute(VOS_FALSE)) && i < SNTP_SERVER_WAITROUTE_TIMES; i++)
            {
                SntpDebug("\r\ncheck route try[%d]", i);
                usleep(500000);
            }
            lIpv4Flag = 1;
            SntpDebug("\r\n v4 up now, ATP_UTIL_Gethostbyname success. FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);    
        }
        else
        {
            SntpDebug("\r\n%s is unknown\r\n", ntpserver);        
        }   
    }

    if (0 == lIpv4Flag && 0 == lIpv6Flag) 
    {
        SntpDebug("\r\n%s is unknown\r\n", ntpserver);
    }
    /* setup sockets and do the connection/bind for output/input */
    else if((*ntpofd = socket(lIpv6Flag ? AF_INET6 : AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket");  
    }
    else 
    {
        SntpDebug("\r\n socket,ntpofd : %d FILE[%s] LINE[%u] \r\n",*ntpofd, __FILE__,__LINE__);
        
        (VOS_VOID)ATP_UTIL_SetFilterlistControlMark(*ntpofd);
    
        if (lIpv6Flag) 
        {
            g_lNtpFamily = AF_INET6;    // sntp server is ipv6 format      
            SntpDebug("\r\n Set IPv6 parameter . FILE[%s] LINE[%u] \r\n",__FILE__,__LINE__);
            stIpv6Addr.sin6_family = AF_INET6;
            stIpv6Addr.sin6_port = htons(NTPPORT);
            /* sa6.sin6_addr is already here */
            
            if (IN6_IS_ADDR_LINKLOCAL(&(stIpv6Addr.sin6_addr)))
            {
                stIpv6Addr.sin6_scope_id = if_nametoindex("br0");
            }           
        }
        else
        {
            g_lNtpFamily = AF_INET;  // sntp server is ipv4 format
            memset(&addr, 0, sizeof(addr));
            if (hp)
            {
                memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
            }
            addr.sin_family = AF_INET;
            addr.sin_port   = htons(NTPPORT);                        
        }

        ret = 0;
        localip = 0;

        if ( lIpv6Flag )
        {
            ret = connect(*ntpofd, (struct sockaddr *) &stIpv6Addr, sizeof(stIpv6Addr));
            if ( ret < 0 )
            {                            
                SntpDebug("\r\n Connect ipv6 address,ntpofd : %d error : %s . FILE[%s] LINE[%u] \r\n",*ntpofd,strerror(errno),__FILE__,__LINE__);
            }
        }
        else
        {                        
            ret = connect(*ntpofd, (struct sockaddr *)&addr, sizeof(addr));
        }
        if (ret < 0)
        {
            close(*ntpofd);
            SntpDebug("socket connect %s failed.error no:%d\n", inet_ntoa(addr.sin_addr), errno);
            if ( !lIpv6Flag )
            {                            
                if (SntpGetLocalIp(&localip))
                {
                    if((*ntpofd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
                    {
                        perror("socket");
                    }
                    else
                    {
                        memset(&addr, 0, sizeof(addr));
                        addr.sin_family = AF_INET;
                        addr.sin_addr.s_addr = localip;                                
                        if (bind(*ntpofd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                        {
                            close(*ntpofd);
                            SntpDebug("socket bind %s failed.\n", inet_ntoa(addr.sin_addr));
                        }
                        else
                        {
                            memset(&addr, 0, sizeof(addr));
                            if (hp)
                            {
                                memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
                            }
                            addr.sin_family = AF_INET;
                            addr.sin_port   = htons(NTPPORT);
                            ret = connect(*ntpofd, (struct sockaddr *)&addr, sizeof(addr));
                            if (ret < 0)
                            {
                                close(*ntpofd);
                                SntpDebug("atp socket connect %s failed.\n", inet_ntoa(addr.sin_addr));
                            }
                        }
                    }
                }
            }
        }

        if (ret < 0)
        {
            SntpDebug("socket connect failed.\n");
        }
        else 
        {
            SntpDebug("socket connect ok.\n");
            start_connection = 1;
            return 1;
        }
    }

    return 0;
}

unsigned int SntpUpdateTime(ntp_double *T1,ntp_double *T2,ntp_double *T3,ntp_double *T4, int ntpofd)
{
    struct timeval daytime;
    struct ntpmsg ntpreq;
    struct timeval timeout;
    struct ntpmsg ntpanswer;
    fd_set myset;
    static unsigned int uIFailLogCount = 0;
    unsigned int LI;

    //for pclint
    uIFailLogCount = uIFailLogCount;
    
    memset(&ntpreq, 0, sizeof(ntpreq));
    ntpreq.flags = htonl(0x23000000); // Version 4, mode 3 (client)
    
    /* Remember the T1 transmission time, and fill it
    into the request as well.  */

    gettimeofday(&daytime, NULL);

    *T1 = (unsigned long)daytime.tv_sec + MILLION_D_I * daytime.tv_usec;

    ntpreq.transmit_stamp.sec = htonl(daytime.tv_sec + UNIX_NTP_DIFF);
    ntpreq.transmit_stamp.frac = htonl(TO_FRAC(daytime.tv_usec));

    /* send the message */

    if (send(ntpofd, &ntpreq, sizeof(ntpreq), 0) != sizeof(ntpreq))
    {
        SntpDebug("cant send ntp request");
        return 0;
    }

    SntpDebug("transmit %ld,%ld  frac = %ld", daytime.tv_sec, daytime.tv_usec,
    ntohl(ntpreq.transmit_stamp.frac));

    /* and wait for the answer, using a timeout */

    timeout.tv_sec = NTPTIMEOUT;
    timeout.tv_usec = 0;

    FD_ZERO (&myset);
    FD_SET (ntpofd, &myset);

    SntpDebug("begin to receive packet, time is %d\n", time(0));
    if (select(ntpofd + 1, &myset, NULL, NULL, &timeout) > 0)
    {
        /* remember the T4 reception time */
        gettimeofday(&daytime, NULL);

        SntpDebug("receive %ld, %ld", daytime.tv_sec, daytime.tv_usec);

        *T4 = (unsigned long)daytime.tv_sec + MILLION_D_I * daytime.tv_usec;

        /* read and see if its an NTP message */

        if (recv(ntpofd, &ntpanswer,sizeof(ntpanswer), 0) == sizeof(ntpanswer))
        {
            SntpDebug("got NTP reply, flags 0x%lu\r\n", ntohl(ntpanswer.flags));  
            uIFailLogCount = 0;  // SNTP recv NTP packet success. clear the fail count to 0
#ifdef SUPPORT_ATP_LOG_VOICE_TYPE
            hasT102Log = 0;
#endif
        }
        else
        {
            SntpDebug("return from recv, receive ntp packet error\n");
#ifdef SUPPORT_ATP_LOG_VOICE_TYPE
            if ( uIFailLogCount == 0 )    
            {
                if (g_lNtpFamily == AF_INET)
                {
                    ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SERVER_UNREACHABLE);
                }
                else
                {
                    ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTPv6_SERVER_UNREACHABLE);
                }
				
                hasT102Log = 1;
            }
            uIFailLogCount++;
#endif
            return 0;
        }
    }
    else
    {
        SntpDebug("return from select, not receive ntp packet, error ocurred or time out\n");

#ifdef  SUPPORT_ATP_TR098
        connectNTPSrvTimeOutCnt++;
#endif
        connectNTPSrvTimeOutFlag = 1;
        return 0;
    }
   
    LI = (ntohl(ntpanswer.flags) & NTP_ANS_LEAP_INDICATION);
    if (LI == NTP_ANS_LEAP_INDICATION)   // LI bit is 11, look as failed.
    {    
#ifdef SUPPORT_ATP_LOG_VOICE_TYPE
        if ( uIFailLogCount == 0 )    
        {
            if (g_lNtpFamily == AF_INET)
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_FAIL);
            }
            else
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTPv6_SYNC_FAIL);
            }
            hasT102Log = 1;
        }
        uIFailLogCount++;
#endif
    }

    /* process the data. extract the T2 and T3 times from the message */
    SntpDebug("T2 %ld,%ld  T3 = %ld,%ld", 
                    ntohl(ntpanswer.receive_stamp.sec) - UNIX_NTP_DIFF,
                    ntohl(ntpanswer.receive_stamp.frac),
                    ntohl(ntpanswer.transmit_stamp.sec) - UNIX_NTP_DIFF,
                    ntohl(ntpanswer.transmit_stamp.frac));

    *T2 = (ntohl(ntpanswer.receive_stamp.sec) - UNIX_NTP_DIFF) + 
            ((ntp_double)ntohl(ntpanswer.receive_stamp.frac)) * NTP_SCALE_I;

    *T3 = (ntohl(ntpanswer.transmit_stamp.sec) - UNIX_NTP_DIFF) + 
            ((ntp_double)ntohl(ntpanswer.transmit_stamp.frac)) * NTP_SCALE_I;

    SntpDebug("T1 %f T2 %f T3 %f T4 %f\n", *T1, *T2, *T3, *T4);
    
    return 1;   
}

static unsigned int SntpBuildTime(ntp_double T1, ntp_double T2,
                ntp_double T3, ntp_double T4, struct timeval* new,
                struct timeval* old, struct timeval* adjust)
{
    ntp_double offset = 0;

    //struct timezone zone;
    long n;
    
    /* the local clock offset is a mean value of the differences */
    offset = ((T2 - T1) + (T3 - T4)) * 0.5;

    SntpDebug("clock offset %f\n", offset);
    SntpDebug("T2-T1 %ld, T3-T4 %ld, offset %ld",
                    (long)(T2-T1),
                    (long)(T3-T4),
                    (long)offset);

    /* compensate for zone difference between us and the server */

    offset += g_lZoneDiff;

    /* Start by converting to timeval format. Note that we have to cater for
    negative, unsigned values. */

    if ((n = (long)offset) > offset)
    {
        --n;
    }
    adjust->tv_sec = n;
    adjust->tv_usec = (long)(MILLION_D * (offset - n));

    gettimeofday(old, NULL);

    new->tv_sec = old->tv_sec + adjust->tv_sec;
    new->tv_usec = (n = (long)old->tv_usec + (long)adjust->tv_usec);

    if (n < 0)
    {
        new->tv_usec += MILLION_L;
        --new->tv_sec;
    }
    else if (n >= MILLION_L)
    {
        new->tv_usec -= MILLION_L;
        ++new->tv_sec;
    }

    return 1;
}

unsigned int SntpAdjustTime(struct timeval* adjust)
{
    struct timeval previous;
    /* use adjtime to adjust the time - will smoothly "fade in"
    * the new time (only works for +/- 131072 usecs)
    */
    if (adjtime(adjust, &previous))
    {
        SntpDebug("Unable to adjust time (%d,%d): error %m",
                        adjust->tv_sec, adjust->tv_usec, errno);
    }
    else
    {
        if (previous.tv_sec || previous.tv_usec)
        {
            SntpDebug("outstanding adjustment %u sec %u usec",
                            (long)previous.tv_sec,
                            (long)previous.tv_usec);
        }
        else
        {
            SntpDebug("adjusted %d usec",
                            adjust->tv_sec < 0 ?
                            (adjust->tv_usec - MILLION_L) :
                            adjust->tv_usec);
        }
        lFlg = 1;
    }
    return 1;
}

unsigned int SntpSetTime(struct timeval* new,
                struct timeval* old, struct timeval* adjust)
{
    new->tv_sec -= utc_offset;
#ifdef USE_VTOP_TIMER                          
    gmtime_r(&(new->tv_sec), (struct tm*)&stVtopTm);

    stVtopTm.tm_year += 1900;
    stVtopTm.tm_mon += 1;
    VTOP_SetSysTime(&stVtopTm);
#else
#ifdef SUPPORT_ATP_SNTP_BT
    SntpRootPrivileges();
    settimeofday(new, NULL);  /* in the kernel */
    SntpDropRootPrivileges();
#else
    settimeofday(new, NULL);  /* in the kernel */
#endif
#endif

    set_offset(new->tv_sec - old->tv_sec);

    set_rtc(new);             /* in the RTC through /dev/rtc */

    SntpDebug("set time: old=(%ld,%.6ld) new=(%ld,%.6ld) adjust=(%ld,%.6ld)",
                    (long)old->tv_sec,(long)old->tv_usec,
                    (long)new->tv_sec,(long)new->tv_usec,
                    (long)adjust->tv_sec,(long)adjust->tv_usec);
    
    return 1;
}

static unsigned int SntpStartConnection(char *ntpserver,struct timeval *old, struct timeval *new, int ntpofd)
{
    int retries;
    ntp_double T1=0;
    ntp_double T2=0;
    ntp_double T3=0;
    ntp_double T4=0;
    struct timeval adjust;

#ifdef USE_VTOP_TIMER    
    VTOP_S_TM stVtopTm;

    memset(&stVtopTm, 0, sizeof(VTOP_S_TM));
#endif    

    start_connection = 0;
    SntpDebug("NTP: Open connection\n");

    retries = 0;

    /* 该全局变量每次刷新前都需要初始化为0,然后重新计算 */
    utc_offset = 0;

    while (retries < MAXRETRY)
    {   
        SntpDebug("sending NTP client request, retry %d\n", retries);
        if (SntpUpdateTime(&T1, &T2, &T3, &T4,ntpofd)) 
        {
            break;
        }
        retries++;
    }
    setup_connection = 1;   
    if (retries < MAXRETRY)
    {
        SntpBuildTime(T1, T2, T3, T4, new, old, &adjust);

#ifdef SUPPORT_ATP_SNTP_DST
        SntpSetDST(new, &adjust);
#endif

        utc_offset += g_lZoneDiff;
        WriteOffset2Kernel(utc_offset);

        if (as_daemon && ((!adjust.tv_sec && adjust.tv_usec < 131072) ||
             (adjust.tv_sec == -1 && adjust.tv_usec > 868928)))
        {
            SntpAdjustTime(&adjust);
        }
        else 
        {           
            SntpSetTime(new, old, &adjust);
        }

        SntpDebug("Date: %s", ctime((const time_t *)&new->tv_sec));
        refreshinterval = g_lSuccessIntervalSec;
        lFlg = 1;
        //break;
        
        return 1;
    }

    return 0;
}

static void SntpSetStatus(struct timeval new, struct timeval old)
{
    static VOS_UINT32 uIFailConut = 0;
    //for pclint
    uIFailConut = uIFailConut;
    
    if (lFlg == 0)    // SNTP synchronization failure
    {
        ucSyncStatus = ATP_SNTP_SYNC_STATUS_CODE_ERROR;
        WriteStatus(ucSyncStatus);

        // add for DT, if SNTP synchronization failed, need to inform Dect for the first time
#ifdef SUPPORT_ATP_SNTP_SYNC_FAIL_NOTIFY
        if ( uIFailConut == 0 )    
        {
            NotifySntpUpdateToOther(VOS_ERROR);
        }
        uIFailConut++;
#endif

#ifdef SUPPORT_ATP_SNTP_ADD_LOG
        if(1 != ulSynStatForLog)
        {
            ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_START);
            ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_FAIL);
        }
        ulSynStatForLog = 1;
#endif
    }
    else   // SNTP synchronization success
    {
        uIFailConut = 0;   // SNTP synchronization success. clear the fail count to 0
        failLogCnt = 0;

#ifdef SUPPORT_ATP_SNTP_ADD_LOG
        //ATP_LOG_Printf(ATP_LOG_TYPE_NTP, ATP_LOG_LEVEL_NOTICE, 1, "NTP synchronization start."); 
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_START);

        //ATP_LOG_Printf(ATP_LOG_TYPE_NTP, ATP_LOG_LEVEL_NOTICE, 1, "NTP synchronization success!");  
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_SUC);

        ulSynStatForLog = 2;
#elif defined(SUPPORT_ATP_LOG_VOICE_TYPE)
        if (!hasT102Log)
        {
            if (g_lNtpFamily == AF_INET)
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SYNC_SUC);
            }
            else
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTPv6_SYNC_SUC);
            }
        }
#endif
        
        NotifySntpUpdateToOther(new.tv_sec - old.tv_sec);
/*
#ifdef SUPPORT_ATP_CWMP_TRIGGER_DNS_FRIST
		if (VOS_OK != ATP_MSG_SendSimpleMsg(ATP_CBBID_CWMP_NAME, ATP_MSG_CWMP_SNTP_SYNC, (VOS_UINT32)(new.tv_sec - old.tv_sec)))
		{
			printf("\r\n SNTP sends msg to cwmp ERROR !!! \r\n");
		} 
#endif
*/
		if (VOS_OK != ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, ATP_MSG_SNTP_SYS_SUCCESS, (VOS_UINT32)(new.tv_sec - old.tv_sec)))
		{
			printf("\r\n SNTP sends msg to cms ERROR !!! \r\n");
		}           

        ucSyncStatus = ATP_SNTP_SYNC_STATUS_CODE_SYNC;
        WriteStatus(ucSyncStatus);

#ifdef SUPPORT_ATP_SNTP_BT
        VTP_Set_FirstTime();//add by q00122007 2010-04-30	
#endif
    }
}

static void SntpDelay()
{
    int refresh_counter = 0;
    int sleep_count = 0;
    refresh_counter = refreshinterval;

#ifdef SUPPORT_ATP_TR181
    FILE *pf;
    char tempArr[16] = {0};
    unsigned int easySupportStatus = 1;
    unsigned int defaultCredential = 1;
#endif
    
    SntpDebug("refresh counter: %d, synchronization flag is %d\n", refresh_counter, lFlg);
    
    if (lFlg == 1)
    {
        if (g_lSuccessIntervalMicroSec > 0)
        {
            SntpDebug("usleep: %d\n", g_lSuccessIntervalMicroSec);
            usleep(g_lSuccessIntervalMicroSec);
        }
    }
    else if (lFlg == 0)
    {
        if (g_lFailIntervalMicroSec > 0)
        {
            SntpDebug("usleep: %d\n", g_lFailIntervalMicroSec);
            usleep(g_lFailIntervalMicroSec);
        }
    }

    /*若所有ntp服务器都请求失败，则等5秒后再开始下一轮尝试*/
    if (0 == lFlg)
    {
#ifdef  SUPPORT_ATP_TR098
        SntpDebug("NO NTP server avaliable, connectNTPSrvTimeOutCnt is %d, sleep: %ld\n", connectNTPSrvTimeOutCnt, g_lFailInterval);
        if (g_lFailInterval - connectNTPSrvTimeOutCnt*NTPTIMEOUT > 0)
        {
            sleep(g_lFailInterval - connectNTPSrvTimeOutCnt*NTPTIMEOUT); 
        }
        connectNTPSrvTimeOutCnt = 0;
#endif

#ifdef SUPPORT_ATP_TR181

#ifdef SUPPORT_ATP_LOG_VOICE_TYPE
        if (failLogCnt == 0 && hasT102Log == 0)
        {
            if (g_lNtpFamily == AF_INET)
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTP_SERVER_UNREACHABLE);
            }
            else
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_SNTPv6_SERVER_UNREACHABLE);
            }
        }
        failLogCnt++;
#endif

        pf = fopen(NTP_EASYSUPPORT_FILE, "rb");
        if(pf != NULL)
        {
            fgets(tempArr, sizeof(tempArr), pf);
            easySupportStatus= atoi(tempArr);
            fclose(pf);
        }

        pf = fopen(NTP_DEFAULT_CREDENTIAL_FILE, "rb");
        if(pf != NULL)
        {
            fgets(tempArr, sizeof(tempArr), pf);
            defaultCredential= atoi(tempArr);
            fclose(pf);
        }

        if ( defaultCredential == 0 || easySupportStatus == 0 )    // DT requirement, if easy support not enabled, need to wait 300 seconds to time sync
        {
            SntpDebug("NO NTP server avaliable, not default credential, easysupport is off , sleep 300s\n");
            SntpDebug("NO NTP server avaliable, not default credential, easysupport is off , connectNTPSrvTimeOutFlag %d\n", connectNTPSrvTimeOutFlag);
            //sleep(300-connectNTPSrvTimeOutFlag*NTPTIMEOUT); 
            sleep(300-FAIL_INTERVAL); 
        }
#endif

    }
    else
    {
        lFlg = 0;
        if (refresh_counter > 10)
        {
            sleep_count = refresh_counter / 10 + 1;
        }
        else
        {
            sleep_count = refresh_counter / 10;
        }
        while (sleep_count-- > 0)
        {
            if ((0 == sleep_count) && (0 == refresh_counter % 10))
            {
                break;
            }
            else if (0 == sleep_count)
            {
                sleep(refresh_counter % 10);
            }
            else
            {
                sleep(10);
            }
            /*Start of ATP网络协议组 2010-4-7 判断下次对时前是否有夏令时变化 by h00163136*/
#ifdef SUPPORT_ATP_SNTP_DST
            SntpUpdateDST();
#endif
            /*End of ATP网络协议组 2010-4-7 for by h00163136*/
            if (check_config)
                break;
        }
    }
}
int main(int argc, char **argv)
{
    char *ntpserver = NULL;
    int use_config = 0;

    int k = 0;
    struct timeval  old, new;
    int ntpofd = 0;

    /*Start of Protocol 2013-12-11 22:27 for DTS2013111307868  by y00250667*/
    struct timeval stLastTime;
    struct timeval stCurrentTime;
    unsigned long ultimeElapsed = 0;
    /*End of Protocol 2013-12-11 22:27 for DTS2013111307868 by y00250667*/
    
#ifdef SUPPORT_ATP_TR181
    unsigned int uIRetryNum = 0;
#endif

    memset(&new, 0, sizeof(new));
    memset(&old, 0, sizeof(old));
    /*Start of Protocol 2013-12-11 22:28 for DTS2013111307868 by y00250667*/
    memset(&stLastTime, 0, sizeof(struct timeval));
    memset(&stCurrentTime, 0, sizeof(struct timeval));
    /*End of Protocol 2013-12-11 22:28 for DTS2013111307868 by y00250667*/
    
//for pclint
#ifdef ATP_PCLINT
    g_lNtpFamily = g_lNtpFamily;
    failLogCnt = failLogCnt;
    connectNTPSrvTimeOutFlag = connectNTPSrvTimeOutFlag;
#endif

    //lint -save -e{40,560}
    if ((argc == 2) && (NULL != argv[1]) && (0 == strcmp(argv[1],ATP_VERSION_CMD_KEY)))
    {
       printf("\r\n%s.\n", MODULE_VERSION);  
       exit(0);
    }
    //lint -restore

#ifdef SUPPORT_ATP_SNTP_BT
    SntpDropRootPrivileges();
#endif
    
    SavePid();

    WriteStatus(ucSyncStatus);
    
    if (argc < 2)
    {
        use_config = 1;
        check_config = 1;
        as_daemon++;
    }
    else
    {
        /* 常用格式 sntpserver -d xxx addr */
        if (argc == 4 && !strcmp("-d", argv[1]))
        {
            /* 刷新时间 */
            refreshinterval = atoi(argv[2]);
            /* 服务器 */
            ntpserver = argv[3];
            as_daemon++;
        }
        else
            ntpserver = argv[1];
        setup_connection++;
    }
    
    /* set up signal handler 如果调用 kill -SIGUSR1 pid */
    signal(SIGUSR1, catch_signal);
    
#ifdef USESYSLOG
    /* open logfile */
    
    if (as_daemon)
    {
        openlog(argv[0], LOG_PID, LOG_DAEMON);
        syslog(LOG_INFO, "Starting...");
    }
#endif

    /* Msg Init*/
    if (VOS_OK != ATP_MSG_Init(ATP_CBBID_SNTP_NAME))
    {
        printf("\r\nSntp msg init fail. !!!\r\n");
    }

    /*强制时间同步*/
    NotifySntpUpdateToOther(100);
    
    /* daemon loop */
    do
    {
        if ((check_config) && (use_config))
        {
            if (!SntpLoadCfgFromFile()) 
            {
                SntpDebug("Load Sntp config file filed\r\n");
                break;
            }
        }

        for (k = 0; k < SNTP_SERVER_NUM; k++)
        {        
            ntpserver = g_stSntpServer[k].acServer;
            SntpDebug("sntp server[%d][%s].\r\n", k, ntpserver);
            if (0 == strlen(ntpserver))
            {
                continue;
            }

            connectNTPSrvTimeOutFlag = 0;    // set the connectNTPSrvTimeOutFlag to 0 befroe loop
            
            setup_connection = g_lSrcActive;
            
            /*建立连接*/
            if (setup_connection) 
            {
                if (!SntpSetupConnection(ntpserver, &ntpofd)) 
                {
                    SntpDebug("SntpSetupConnection filed\r\n");                 
                }
                else
                {
                    SntpDebug("SntpSetupConnection success\r\n");                
                }
            }
            ATP_UTIL_GetSysTime(&stLastTime);
            
            /*SNTP收发包交互*/
            if (start_connection)
            {
                if (SntpStartConnection(ntpserver, &new, &old,ntpofd)) 
                {
                    SntpDebug("SntpStartConnection over\r\n");
                    close(ntpofd);
#ifdef SUPPORT_ATP_TR181
                    uIRetryNum = 0;
#endif
                    break;
                }

                /* 同步失败 */
                SntpDebug("SntpStartConnection failed\r\n"); 
                close(ntpofd);
            }
            
            ATP_UTIL_GetSysTime(&stCurrentTime);
            ultimeElapsed = SntpTimeDiff(&stLastTime, &stCurrentTime);            
            
#ifdef SUPPORT_ATP_TR181
            if (uIRetryNum < 1)
            {
                k = k -1;               
                uIRetryNum = 1;
                SntpDebug("SntpStartConnection failed, Retry once with sleep %d\r\n", FAIL_INTERVAL - connectNTPSrvTimeOutFlag*NTPTIMEOUT);
                
                usleep(FAIL_INTERVAL * 1000000 - ultimeElapsed);
                
                connectNTPSrvTimeOutFlag = 0;
            }
            else
            {
                SntpDebug("SntpStartConnection failed, Retry once with sleep %d\r\n", FAIL_INTERVAL - connectNTPSrvTimeOutFlag*NTPTIMEOUT); 
                
                usleep(FAIL_INTERVAL * 1000000 - ultimeElapsed);
                
                uIRetryNum = 0;
            }
#endif
        }

        /*根据同步标志位设置状态*/
        SntpSetStatus(new, old);
        
        /* delay "refreshinterval" seconds, but check for config update each second during
        * that wait
        */
        if (as_daemon)
        {
            SntpDelay();        
        }
    }while (as_daemon);

    return 0;
}

/* open /dev/rtc and use an ioctl to set the time in the RTC chip */

static void set_rtc(struct timeval *tv)
{
#if 0
	int fd;
	struct tm *mytm;
	unsigned int secs;

	/* Open the rtc driver */

	fd = open("/dev/rtc", O_RDONLY);

    if (fd < 0)
    {
		log("cant open /dev/rtc: %m", errno);
		return;
	}

	/* convert from seconds since 1970 to broken down time 
	 * (that the RTC needs)
	 * remember to round secs depending on usecs
	 */

	secs = tv->tv_sec + ((tv->tv_usec > 500000) ? 1 : 0);
	
	mytm = localtime((time_t *)&secs);
	
	/* set the RTC time
	 * struct rtc_time is the same as struct tm
	 */

    if (ioctl(fd, RTC_SET_TIME, mytm) < 0)
    {
		log("RTC_SET_TIME failed: %m", errno);
	}
	
	close(fd);
#endif
}

static void SntpParseServer(char* pszServers)
{
    char acBuff[ATP_SNTP_ALLSERVER_LEN];
    char* pcToken = NULL;
    char* pcPtrPtr = NULL;
    int i = 0;
    int iLen = 0;

    memset(g_stSntpServer, 0, sizeof(NTP_SERVER) * SNTP_SERVER_NUM);
    snprintf(acBuff, sizeof(acBuff), "%s", pszServers);

    pcToken = strtok_r(acBuff, ",", &pcPtrPtr);    
    while ((NULL != pcToken) && (i < SNTP_SERVER_NUM))
    {
        iLen = strlen(pcToken);
        if (iLen < ATP_SNTP_SERVER_MAXLEN)
        {
            snprintf(g_stSntpServer[i].acServer, sizeof(g_stSntpServer[i].acServer),"%s", pcToken);
            g_stSntpServer[i].acServer[iLen] = '\0';
        }
        i++;
        pcToken = strtok_r(NULL, ",", &pcPtrPtr);
    } 
}


/* Start by d00107688 to add DST */
#ifdef SUPPORT_ATP_SNTP_DST


static int JudgeJumpYear(ATP_SNTP_DST_TIME_ST *pstDstStart, ATP_SNTP_DST_TIME_ST *pstDstEnd)
{
    if (!pstDstStart || !pstDstEnd)
    {
        return 0;
    }

    if (pstDstStart->iDstMonth > pstDstEnd->iDstMonth)
    {
        return 1;
    }
    if (pstDstStart->iDstMonth < pstDstEnd->iDstMonth)
    {
        return 0;
    }
 
    /*月份相同，比较星期数*/
    if (pstDstStart->iWeekNum > pstDstEnd->iWeekNum)
    {
        return 1;
    }
    if (pstDstStart->iWeekNum < pstDstEnd->iWeekNum)
    {
        return 0;
    }
    /*比较天*/
    if (pstDstStart->iWday > pstDstEnd->iWday)
    {
        return 1;
    }
    if (pstDstStart->iWday < pstDstEnd->iWday)
    {
        return 0;
    }
    /*前面都相同，则比较时、分、秒*/
    if (pstDstStart->ulDstTime > pstDstEnd->ulDstTime)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int GetRealDateFromDst(unsigned int ulYear, const ATP_SNTP_DST_TIME_ST stDstTime, SNTP_DST_REAL_ST *pstResult)
{
    long int lDays = 0;
    unsigned int i = 0;
    int iDayOfWeek = -1;

    if (!pstResult)
    {
        return (-1);
    }

    /* 真实日期初始化 */
    pstResult->ulRealYear = ulYear;
    pstResult->ulRealMonth = stDstTime.iDstMonth;
    pstResult->ulRealDay = 1;
    pstResult->ulOffsetTime = stDstTime.ulDstTime;

    if (ulYear > REFER_YEAR)
    {
        for (i = REFER_YEAR; i < ulYear; i++)
        {
            lDays += acDayOfYear[(int)IsLeap(i)];
        }
    }
    else
    {
        for (i = REFER_YEAR; i > ulYear; i--)
        {
            lDays -= acDayOfYear[(int)IsLeap(i)];
        }
    }

    for (i = 1; i < pstResult->ulRealMonth; i++)
    {
        lDays += aacDay_Of_Per_Month[(int)IsLeap(ulYear)][i];
    }

    /* 需要加上1号这天 */
    //lDays += 1;

    iDayOfWeek = lDays % DAY_OF_A_WEEK;
    if (iDayOfWeek < 0)
    {
        iDayOfWeek += DAY_OF_A_WEEK;
    }
    
    iDayOfWeek += REFER_WEEKDAY;
    if (iDayOfWeek >= DAY_OF_A_WEEK)
    {
        iDayOfWeek %= DAY_OF_A_WEEK;
    }

    /* 至此，获取到了该年的该月份的1号是星期几 */
    SntpDebug("year: %d month: %d  the firest day is weekday: %d\r\n", ulYear, stDstTime.iDstMonth, iDayOfWeek);

    /*夏令时指定的星期几在本月的第二个星期才开始出现*/
    if (iDayOfWeek > stDstTime.iWday)
    {
        /*若夏令时指定的是第一个星期, 意思是它的第一次出现，在本月第二个星期了*/
        pstResult->ulRealDay += DAY_OF_A_WEEK - iDayOfWeek + stDstTime.iWday + (stDstTime.iWeekNum - 1) * DAY_OF_A_WEEK;
        /*若是第5个星期，POSIX规范的意思是这个月的最后一个星期几,如果加5个星期的偏移，可能超出了这个月*/
        if (pstResult->ulRealDay > (unsigned int)aacDay_Of_Per_Month[(int)IsLeap(ulYear)][stDstTime.iDstMonth])
        {
            pstResult->ulRealDay -= DAY_OF_A_WEEK;
        }
    }
    /*夏令时指定的星期几就落在本月的第一个星期内*/
    else
    {
        pstResult->ulRealDay += stDstTime.iWday - iDayOfWeek + (stDstTime.iWeekNum - 1) * DAY_OF_A_WEEK;
        /*若是第5个星期，POSIX规范的意思是这个月的最后一个星期几,如果加5个星期的偏移，可能超出了这个月*/
        if (pstResult->ulRealDay > (unsigned int)aacDay_Of_Per_Month[(int)IsLeap(ulYear)][stDstTime.iDstMonth])
        {
            pstResult->ulRealDay -= DAY_OF_A_WEEK;
        }
    }

    SntpDebug("year: %d realmonth: %d day: %d\r\n", 
        pstResult->ulRealYear,  pstResult->ulRealMonth, pstResult->ulRealDay);

    return 0;
}


static int InDstTime(struct tm *pstTm, SNTP_DST_REAL_ST *pstDstStart, SNTP_DST_REAL_ST *pstDstEnd)
{
    struct tm stTM;
    time_t NowSeconds = 0;
    time_t DstStartSeconds = 0;
    time_t DstEndSeconds = 0;
    /*tm_isdst=1, 表示用localtime得到的本地时间tm已经是夏令时间，
        tm_isdst=0, 表示用localtime得到的本地时间tm没有夏令时偏移*/
    int dstFlagNow = pstTm->tm_isdst;
    
    /*先算出传入的待比较时间的秒数*/
    memset(&stTM, 0, sizeof(struct tm));
    stTM.tm_year = pstTm->tm_year;
    stTM.tm_mon = pstTm->tm_mon;
    stTM.tm_mday = pstTm->tm_mday;
    stTM.tm_hour = pstTm->tm_hour;
    stTM.tm_min = pstTm->tm_min;
    stTM.tm_sec = pstTm->tm_sec;
    NowSeconds = mktime(&stTM);
    
    /*算出夏令时开始的秒数*/
    /*tm 结构里的年数=真实年-1900; 月份0~11, 对应真实月份1~12月*/
    /*SNTP_DST_REAL_ST 结构里面没有hh:mm:ss，是换算后一天中的秒数*/
    memset(&stTM, 0, sizeof(struct tm));
    stTM.tm_year = pstDstStart->ulRealYear - 1900;
    stTM.tm_mon = pstDstStart->ulRealMonth - 1;
    stTM.tm_mday = pstDstStart->ulRealDay;
    DstStartSeconds = mktime(&stTM) + pstDstStart->ulOffsetTime;
    
    /*算出夏令时结束的秒数*/
    memset(&stTM, 0, sizeof(struct tm));
    stTM.tm_year = pstDstEnd->ulRealYear - 1900;
    stTM.tm_mon = pstDstEnd->ulRealMonth - 1;
    stTM.tm_mday = pstDstEnd->ulRealDay;
    DstEndSeconds = mktime(&stTM) + pstDstEnd->ulOffsetTime;

    if (1 == dstFlagNow)
    {
        if (NowSeconds >= (DstStartSeconds + g_stDstTz.lDstOffsetTime) 
            && NowSeconds < DstEndSeconds)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (NowSeconds >= DstStartSeconds 
           && NowSeconds < (DstEndSeconds -g_stDstTz.lDstOffsetTime))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

#endif
/* End by d00107688 to add DST */


static void WriteStatus(unsigned char ulStauts)
{
    FILE *fp = NULL;
    
    fp = fopen(ATP_SNTP_STATUS_FILE, "w+");
    if (NULL == fp)
    {
        perror("error to open file");
        return ;
    }

    fprintf(fp, "%d\r\n", ulStauts);
    
    fclose(fp);
}

static void WriteOffset2Kernel(int i_utc_offset)
{
    int lfd = 0;
    char acLine[32]={0};

#ifdef SUPPORT_ATP_SNTP_BT
    SntpRootPrivileges();
#endif
    lfd = open(TIME_OFFSET_KERNEL_PROC_PATH, O_WRONLY | O_TRUNC, 0644);
#if 0
    sprintf(acLine, "%d", utc_offset);
#else
    snprintf(acLine, sizeof(acLine), "%d", i_utc_offset);
#endif
    if (lfd > -1)
    {
        write(lfd, acLine, strlen(acLine));
        close(lfd);
    }
#ifdef SUPPORT_ATP_SNTP_BT
    SntpDropRootPrivileges();
#endif	
}

#ifdef SUPPORT_ATP_SNTP_BT
void VTP_Set_FirstTime()
{
    VOS_UINT32              ulType;
    ATP_MSG_HEADER_ST       *pstMsg;
    VOS_CHAR                acMsgBuf[1024] = {0};


    pstMsg = (ATP_MSG_HEADER_ST *)acMsgBuf;
    ATP_MSG_SimpleBuilder(pstMsg, ATP_CBBID_CMS_NAME, ATP_MSG_SET_FIRSTDATE, 0);
    pstMsg->ulDataLen = 1;
    if (VOS_OK != ATP_MSG_Send(pstMsg))
    {
        DEBUG_PRINTF("time send message fail\r\n");
        return ;
    }
}
#endif
