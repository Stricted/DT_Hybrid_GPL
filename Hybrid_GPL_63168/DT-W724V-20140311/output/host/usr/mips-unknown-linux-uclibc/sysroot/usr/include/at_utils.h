/***********************************************************************
  版权信息 : 版权所有(C) 1988-2005, 华为技术有限公司.
  文件名   : utils_atc.h
  作者     : chenjindong,liudan,chenkeyun
  版本     : 
  创建日期 : 2007-3-20
  完成日期 : 
  功能描述 : 头文件
      
  主要函数列表: 无
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    c54530                  0.0.1    初始完成
  备注: 
************************************************************************/

#ifndef __UTILS_H
#define __UTILS_H

#define PROFILE_MAX_LEN         65      /* profile中各项最大字符串长度(包括最后的\0) */
#define PROFILE_NULL            0xff    /* profile中某项字符串为空 */
#define PROFILE_RADIO_STATIC    0       /* profile中APN/IP选项为静态 */
#define PROFILE_RADIO_DYNAMIC   1       /* profile中APN/IP选项为动态 */
#define PROFILE_MAX_LEN            65      /* profile中各项最大字符串长度(包括最后的\0) */
#define PROFILE_SUB_CHAR_0XFE        '\376'
#define PROFILE_SUB_CHAR_SAPCE       ' '
#define PROFILE_SUB_VALUE_NULL       "\377"
#define USR_SBIN                   "/bin"  /* ftok目录 */
#define LOG_SIZE                   50*1024
#define MAX_MSG_LEN 512
#define AT_SHARE_LOG                   "/var/at/at_shared.log"
#define MAX_AT_RET_SIZE     262

#define at_nvram_safe_get(name) ((at_nvram_get)(name) ? : "")
char * at_nvram_get(char *name);
int at_nvram_set(char *name, char *value);
int at_nvram_commit();
#define SAVE_PIN    "save_pin"
#define SAVE_SCID   "save_scid"

/*转换编码表*/
static const int  arrayGSM_7DefaultTable[128] =
{
    0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 
    0x00F2, 0x00C7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,
    0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 0x03A8,
    0x03A3, 0x0398, 0x039E, 0xFFFF, 0x00C6, 0x00E6, 0x00DF, 0x00C9,
    0x0020, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x00A1, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005A, 0x00C4, 0x00D6, 0x00D1, 0x00DC, 0x00A7,
    0x00BF, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007A, 0x00E4, 0x00F6, 0x00F1, 0x00FC, 0x00E0,
};

static const int  arrayGSM_7ExtTable[128] =
{
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005E, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x0020, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0x007B, 0x007D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005C,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005B, 0x007E, 0x005D, 0xFFFF,
    0x007C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20AC, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};


typedef struct{
    char  name[PROFILE_MAX_LEN];
    char  dial_str[PROFILE_MAX_LEN]; 
    char  ppp_username[PROFILE_MAX_LEN];
    char  ppp_passwd[PROFILE_MAX_LEN];
    int   apn_rd;
    char  apn[PROFILE_MAX_LEN];
    int   ip_rd;
    char  ip[PROFILE_MAX_LEN];       
}user_profile;

#if 0
move to ppp main.c
/* ppp进程上报数据 */
typedef struct{
    unsigned int ip_addr;           // IP 地址
    unsigned int ppp_state;         // PPP 连接状态
    unsigned int cur_time_len;      // 当前连接时长
    unsigned int total_time_len;    // 累计连接时长
    long long cur_ppp_ibytes;       // PPP当前接收流量
    long long cur_ppp_obytes;       // PPP当前发送流量
    long long total_ppp_ibytes;     // PPP累计接收流量
    long long total_ppp_obytes;     // PPP累计发送流量
}pppd_sub_data;

#define SIGNAL(s, handler)    do { \
    sa.sa_handler = handler; \
    if (sigaction(s, &sa, NULL) < 0) \
        FATAL("Couldn't establish signal handler (%d): %m", s); \
 } while (0);
#endif
/* <DTS2013083001384 xiazhongyue 20130830 begin */
#define at_buf_Parse(word, wordlist, next,str,key) \
    for (next = &wordlist[strspn(wordlist, str)], \
         safe_strcpy(word, next, sizeof(word)), \
         word[strcspn(word, str)] = '\0', \
         word[sizeof(word) - 1] = '\0', \
         next = strchr(next, key); \
         strlen(word); \
         next = next ? &next[strspn(next, str)] : "", \
         safe_strcpy(word, next, sizeof(word)), \
         word[strcspn(word, str)] = '\0', \
         word[sizeof(word) - 1] = '\0', \
         next = strchr(next, key))
/*  DTS2013083001384 xiazhongyue 20130830 end > */
#if 0
void _DEBUG(char* logfile,char *format, ...);
void _INFO(char* logfile,char *format, ...);
void _WARN(char* logfile,char *format, ...);
void _HIGH(char* logfile,char *format, ...);
void _ERROR(char* logfile,char *format, ...);
void _FATAL(char* logfile,char *format, ...);
int   get_curr_profile(user_profile *profile);
pid_t get_pid( char* filename );
#endif

//int __start_timer(AT_TIMER_S* timer, int millisecond,  void (*timer_handler)(int));
//int __stop_timer(AT_TIMER_S *timer);
int __msleep(int millisecond);

void split_space (char *start, char key);
/* <DTS2013083001384 xiazhongyue 20130830 begin */
#if 0
void split_key_str_array (char **start, char key);
#endif
int count_separator(char* str, char* sep);
unsigned long long hex_str_to_uinteger(char* hex);
int test_str_num(char* str);
void  _syslog(char* logfile, int level, char *format, ...);
/*AP7D01238.Optus SMS: liudan 2008-1-21 ADD BEGIN */
void safe_strcpy(char *dest, const char *src, int MaxDstSize);
int safe_strcmp(const char *s1, const char *s2);
/* AP7D01238.Optus SMS: liudan 2008-1-21 ADD END*/
/*AP7D01433.Optus sms: liudan 2008-3-7 MODIFY BEGIN*/
int safe_atoi(char *src);
/* AP7D01433.Optus sms: liudan 2008-3-7 MODIFY END*/
#if 0
void split_space1(char *start, char key);
#endif
 void convert_space_to_0xFE(const char* str);
  void convert_0xFE_to_space(char* str);
  int gsm7_to_ascii(const unsigned char *puszGSM_7Data, int InputLen, 
                                     unsigned char *puszASCIIData, int BufLen, int *OutputLen);
  int conv_ascii_to_utf8(unsigned char *pszSrc, unsigned char *pszDst);
/*  DTS2013083001384 xiazhongyue 20130830 end > */  
#if 0
#define _DEBUG(logfile, format, arg...)    do{ \
    fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define _INFO(logfile, format, arg...)    do{ \
    fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define _WARN(logfile, format, arg...)    do{ \
    fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define _HIGH(logfile, format, arg...)    do{ \
   fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define _ERROR(logfile, format, arg...)    do{ \
    fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define _FATAL(logfile, format, arg...)    do{ \
    fprintf(stderr,"[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#else
/*Start of modified by l00135113 for usbserver invalid read access*/
#if 0
#define _DEBUG(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 0, message);\
}while(0)

#define _INFO(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 1, message);\
}while(0)

#define _WARN(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 2, message);\
}while(0)

#define _HIGH(logfile, format, arg...)    do{ \
   char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 3, message);\
}while(0)

#define _ERROR(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 4, message);\
}while(0)

#define _FATAL(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    sprintf(message,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    _syslog( logfile, 5, message);\
}while(0)
#else
#define _DEBUG(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 0, message);\
}while(0)

#define _INFO(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 1, message);\
}while(0)

#define _WARN(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 2, message);\
}while(0)

#define _HIGH(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 3, message);\
}while(0)

#define _ERROR(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 4, message);\
}while(0)

#define _FATAL(logfile, format, arg...)    do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 5, message);\
}while(0)
#endif
/*End of modified by l00135113 for usbserver invalid read access*/

#define _print(format,arg...)    do{\
    fprintf(stderr, "[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__,##arg);\
}while(0)
#endif
#endif // __UTILS_H
