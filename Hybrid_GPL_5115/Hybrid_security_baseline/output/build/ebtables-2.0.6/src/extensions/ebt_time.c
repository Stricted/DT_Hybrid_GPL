/***********************************************************
  Copyright (C), 2008, Huawei Tech. Co., Ltd.
  FileName: ebt_time.c
  Author: liliang  Version: v1.0  Date: 2008-2-25
  Description: match time for ebtables

  Copy as libipt_time.c
    Shared library add-on to iptables to add TIME matching support. 
  
  History:
      <author>  <time>   <version >   <desc>
***********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "../include/ebtables_u.h"
#include "../include/linux/netfilter_bridge/ebt_time.h"

static struct option opts[] = 
{
	{ "timestr", required_argument, 0, 't' },
	{ "weekdays", required_argument, 0, 'w'},
	{0}
};

/* Initialize the match. */
static void init(struct ebt_entry_match *match)
{
    EBT_TIME_ST* pstTimeInfo = (EBT_TIME_ST*)match->data;

	pstTimeInfo->weekdays   = 0;
	pstTimeInfo->time_begin = 0;
	pstTimeInfo->time_end   = 0;
	pstTimeInfo->nouse      = 0;
}

static int parse_time(u_int16_t* pusBegin, u_int16_t* pusEnd, const char* aucTime)
{
    char  acBuff[EBT_TIME_LEN_MAX] = {0};
    char* pszTemp = 0;
    int   ulBegin = 0;
    int   ulEnd = 0;

    if (NULL == aucTime)
    {
        printf("atp: ebt_time time string:[NULL] input error. \r\n");
        return EBT_TIME_ERR;
    }

    if (strlen(aucTime) > EBT_TIME_LEN_MAX)
    {
        printf("atp: ebt_time time string:[%s] input error. \r\n", aucTime);
        return EBT_TIME_ERR;
    }
    strcpy(acBuff, aucTime);
    pszTemp = strchr(acBuff, '-');
    if (pszTemp == NULL)
    {
        printf("atp: ebt_time time string:[%s] has no , input error. \r\n", aucTime);
        return EBT_TIME_ERR;
    }
    else
    {
        *pszTemp = '\0';
        ulBegin = atoi(acBuff);
        pszTemp++;
        ulEnd = atoi(pszTemp);
        if (0 > ulBegin || ulBegin > ulEnd || ulEnd > EBT_TIME_MAX)
        {
            printf("atp: ebt_time time string:[%s] start:[%d] end:[%d] input error. \r\n", aucTime, ulBegin, ulEnd);
            return EBT_TIME_ERR;
        }
    }
    *pusBegin = (u_int16_t)ulBegin;
    *pusEnd = (u_int16_t)ulEnd;

        /*modify by kf27876 2010.8.21 remove debug info*/
#ifdef DEBUG_XT_EBT_TIME 
	printf("atp: ebt_time input para time string:[%s] start:[%d] end:[%d]. \r\n", aucTime, ulBegin, ulEnd);
#endif
        /*end modify */
	return EBT_TIME_OK;
}

static unsigned char parse_weekdays(const char* acWeekdays)
{
    unsigned char ucDays = 0;
    int  i = 0;

    if (NULL == acWeekdays)
    {
        printf("atp: ebt_time days string:[NULL] input error. \r\n");
        return ucDays;
    }
    
    if (EBT_DAYS_PER_WEEK != strlen(acWeekdays))
    {
        printf("atp: ebt_time days string:[%s] input error. \r\n", acWeekdays);
        return ucDays;
    }
    for (i = 0; i < EBT_DAYS_PER_WEEK; i++)
    {
        ucDays = ucDays << 1;
        if (('Y' == acWeekdays[i]) || ('y' == acWeekdays[i]))
        {
            ucDays |= 1; 
        }        
    }

    /*modify by kf27876 2010.8.21 remove debug info*/
#ifdef DEBUG_XT_EBT_TIME
    printf("atp: ebt_time input para days string:[%s] to Bit:[0x%x]. \r\n", acWeekdays, ucDays);
#endif
    /* end modify */
    return ucDays;
}

static void help(void)
{
	printf(
"time options:\n"
" --timestr value --weekdays value\n"
"          timestr  value : (startHH*60 + MM)-(endHH*60 + MM)\n"
"          weekdays value: (YNNYNNN)string of day in week from Monday to Sunday.\n");
}


/* time  126-602  ==  2:06 to 10:02
   days  NYYNNYY
*/
static int parse(int c, char **argv, int argc,
                 const struct ebt_u_entry *entry,
                 unsigned int *flags, struct ebt_entry_match **match)
{
	EBT_TIME_ST* pstTimeInfo = (EBT_TIME_ST*)(*match)->data;

	switch (c) /* c is the return value of getopt_long */
	{
		/* time */
    	case 't':
    	    if (EBT_TIME_OK != parse_time(&(pstTimeInfo->time_begin), &(pstTimeInfo->time_end), optarg))
            {
                printf("atp: ebt_time parse time error. \r\n");
                return 0;
            }
            break;

        /* weekday */
        case 'w':
            pstTimeInfo->weekdays = parse_weekdays(optarg);
            break;

        default:
            return 0;
	}

	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_match *match, const char *name,
   unsigned int hookmask, unsigned int time)
{
    EBT_TIME_ST* pstTimeInfo = (EBT_TIME_ST*)match->data;

    /*modify by kf27876 2010.8.21 remove debug info*/
    pstTimeInfo = pstTimeInfo;
#ifdef DEBUG_XT_EBT_TIME
    printf("atp: time check() replace.name:[%s] start=%d:%d,stop=%d:%d,days=0x%x \r\n",
        name, pstTimeInfo->time_begin / EBT_HOURS_PER_MIN, pstTimeInfo->time_begin % EBT_HOURS_PER_MIN,
		pstTimeInfo->time_end / EBT_HOURS_PER_MIN, pstTimeInfo->time_end % EBT_HOURS_PER_MIN, pstTimeInfo->weekdays);
#endif
    /* end modify */
}

static void print(const struct ebt_u_entry *entry,
                  const struct ebt_entry_match *match)
{
	EBT_TIME_ST* pstTimeInfo = ((EBT_TIME_ST*)match->data);

        /*modify by kf27876 2010.8.21 remove debug info*/
        pstTimeInfo = pstTimeInfo;
#ifdef DEBUG_XT_EBT_TIME
	printf(" TIME from %d:%d to %d:%d on Weekdays 0x%x ",
	       pstTimeInfo->time_begin / EBT_HOURS_PER_MIN, pstTimeInfo->time_begin % EBT_HOURS_PER_MIN,
		   pstTimeInfo->time_end / EBT_HOURS_PER_MIN, pstTimeInfo->time_end % EBT_HOURS_PER_MIN, pstTimeInfo->weekdays);
#endif
        /* end modify */
}

static int compare(const struct ebt_entry_match *m1, const struct ebt_entry_match *m2)
{
    EBT_TIME_ST* pstTimeInfo1 = (EBT_TIME_ST*)m1->data;
    EBT_TIME_ST* pstTimeInfo2 = (EBT_TIME_ST*)m2->data;

    if ((pstTimeInfo1->weekdays != pstTimeInfo2->weekdays)
        || (pstTimeInfo1->time_begin != pstTimeInfo2->time_begin)
        || (pstTimeInfo1->time_end != pstTimeInfo2->time_end))
    {
        return 0;
    }
    /*modify by kf27876 2010.8.21 remove debug info*/
#ifdef DEBUG_XT_EBT_TIME
    printf("atp: ebt_time compare the same. \r\n");
#endif
    /* end modify */
    return 1;
}

static struct ebt_u_match time_match =
{
    .name = "time",
    .size = sizeof(EBT_TIME_ST),
    .help = help,   // 可忽略
    .init = init,
    .parse = parse,
    .final_check = final_check,
    .print = print,    // ebtables  打印函数
    .compare = compare,  // 删除时，比较是否是待删除的规则
    .extra_ops = opts,
};

static void _init(void) __attribute((constructor));
void _init(void)
{
	register_match(&time_match);
}
