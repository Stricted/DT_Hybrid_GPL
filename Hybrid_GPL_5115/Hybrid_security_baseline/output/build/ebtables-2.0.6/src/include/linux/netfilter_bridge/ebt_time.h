#ifndef __LINUX_BRIDGE_EBT_TIME_H
#define __LINUX_BRIDGE_EBT_TIME_H

#define EBT_DAYS_PER_WEEK    7
#define EBT_HOURS_PER_MIN    60

#define EBT_TIME_MAX         1439        /* 23*60+59 = 1439 */
#define EBT_TIME_LEN_MAX     9           /* 1439-1439 */

#define EBT_TIME_OK          0
#define EBT_TIME_ERR         -1

typedef struct tagEBT_TIME_ST 
{
    u_int8_t  weekdays;
	u_int16_t time_begin;
	u_int16_t time_end;
    u_int8_t nouse;
	//u_int16_t nouse;
}EBT_TIME_ST;

#endif
