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
#define LOG_SIZE                   10*1024
#define MAX_MSG_LEN 512
#define ATS_LOG                   "/var/at/atsrv.log"
#define AT_SHARE_LOG                   "/var/at/at_shared.log"
/* DTS2011092600505 shidai 20110926 Modify Delete */
#define MAX_AT_RET_SIZE     262

/*<DTS2010043000117  w00126555 20100513  modify begin*/
static const int arrayGSM_7bit[128] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
};

/*转换编码表*/
static const int  arrayGSM_7DefaultTable[128] =
{
    //0       1        2       3      4       5       6       7       8       9       10      11      12      13      14      15
    0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 0x00F2, 0x00C7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,
    0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 0x03A8, 0x03A3, 0x0398, 0x039E, 0x001B/*ECS转义符*/, 0x00C6, 0x00E6, 0x00DF, 0x00C9, 
    0x0020, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x00A1, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C4, 0x00D6, 0x00D1, 0x00DC, 0x00A7, 
    0x00BF, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00E4, 0x00F6, 0x00F1, 0x00FC, 0x00E0
};
/*DTS2010043000117   w00126555 20100513  modify end>*/

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
#define at_buf_Parse(word, wordlist, next,str,key) \
    for (next = &wordlist[strspn(wordlist, str)], \
         strncpy(word, next, sizeof(word)), \
         word[strcspn(word, str)] = '\0', \
         word[sizeof(word) - 1] = '\0', \
         next = strchr(next, key); \
         strlen(word); \
         next = next ? &next[strspn(next, str)] : "", \
         strncpy(word, next, sizeof(word)), \
         word[strcspn(word, str)] = '\0', \
         word[sizeof(word) - 1] = '\0', \
         next = strchr(next, key))

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

int create_pid_file( char * filename ,int flag);
//int __start_timer(AT_TIMER_S* timer, int millisecond,  void (*timer_handler)(int));
//int __stop_timer(AT_TIMER_S *timer);
int __msleep(int millisecond);

void split_space (char *start, char key);
/* DTS2013071105737 z81004485 2013/7/11, Coverity */
int count_separator(char* str, char* sep);
unsigned long long hex_str_to_uinteger(char* hex);
int test_str_num(char* str);
void  _syslog(char* logfile, int level, char *format, ...);
/*AP7D01238.Optus SMS: liudan 2008-1-21 ADD BEGIN */
char *safe_strcpy(char *dest, const char *src);
int safe_strcmp(const char *s1, const char *s2);
/* AP7D01238.Optus SMS: liudan 2008-1-21 ADD END*/
/*AP7D01433.Optus sms: liudan 2008-3-7 MODIFY BEGIN*/
int safe_atoi(char *src);
/* AP7D01433.Optus sms: liudan 2008-3-7 MODIFY END*/
/* DTS2013071105737 z81004485 2013/7/11, Coverity */
void convert_space_to_0xFE(const char* str);
void convert_0xFE_to_space(char* str);
/*<DTS2010043000117   w00126555 20100513  begin*/
int conv_ascii_to_utf8(unsigned char *pszSrc, unsigned char *pszDst);
int get_asciiToUtf8_length(unsigned char *pcAscii, int *para1, unsigned short *para2, unsigned char *para3);
int gsm7_to_ascii(const unsigned char* pGSM_7Data, int dwInputLen, unsigned char* pASCIIData, int dwBufLen, int* dwOutputLen);
int ascii_to_gsm7(const char* pASCIIData, int dwInputLen, char* pGSM_7Data, int dwBufLen, int* dwOutputLen);
/*DTS2010043000117   w00126555 20100513  end>*/

/* <DTS2011071304912 lkf47960 2011-7-12 for T-mobile pnn MOD BEGIN*/
int gsmBytes2String(const unsigned char* pszSrc, char* pszDst, int iSrcLength);
int gsmString2Bytes(const char* pszSrc, unsigned char* pszDst, int iSrcLength);
/* DTS2011092600505 shidai 20110926 Modify Delete */

/* <DTS2011030101068.USSD: mayang 2010-03-09 MODIFY BEGIN */

/* <DTS2011103000139 jiaxinghua 20111030 move out USSD for PNN Begin */
int decode_7bit(const unsigned char *puszSrc, unsigned char *puszDst, int iSrcLength);
int get_srcstr_length(char *pchar, int *para1, unsigned short *para2, unsigned char *para3);
int conv_to_utf8(char *pdst_str, int counte_bytes, unsigned short byte_one, unsigned char utf_one);
/* DTS2011103000139 jiaxinghua 20111030 move out USSD for PNN End> */
/* DTS2013061708346 l00162999 20130620 DELETE */
/* <DTS2011092600505 shidai 20110926 Modify Begin */
int conv_7bit_to_ascii(char *pszName, char *szAscII);
int conv_UCS2_to_utf8(char *pszName, char *pnn_name);
int conv_8bit_to_ascii(char *pszName, char *szAscII);
int conv_8bitHexStr_to_utf8(char *pszSrc, char *pszDst);
int conv_7bitHexStr_to_utf8(char *pszSrc, char *pszDst);
/* <DTS2011103000139 jiaxinghua 20111030 move out USSD for PNN> */
/* DTS2011092600505 shidai 20110926 Modify End */

int encode_7bit_ussd(const unsigned char* pszSrc, unsigned char* pszDst, int iSrcLength);
/* DTS2013071105737 z81004485 2013/7/11, Coverity */
int count_special_char_ussd(char* msg_str);
int process_special_UssdMsg_char(char* processed_ussd_content, unsigned int processedLen, char* ussd_content, unsigned int Len);
/* DTS2013061708346 l00162999 20130620 DELETE */
/* DTS2011030101068.USSD: mayang 2011-03-09 MODIFY END> */
/* DTS2011071304912: lkf47960 2011-7-12 for T-mobile pnn MOD END>*/    
void convertATStrToUnicode(char *str, char *unicode, int str_len); /* <DTS2013090701129 j81004134 2013/9/14 */    

#ifdef ATP_PCLINT 
/* <DTS2013061803625 z81004485 2013/6/18 begin  */
#define _DEBUG(logfile, format, arg...) 
#define _INFO(logfile, format, arg...)  
#define _WARN(logfile, format, arg...)  
#define _HIGH(logfile, format, arg...)  
#define _ERROR(logfile, format, arg...)  
#define _FATAL(logfile, format, arg...)  
#define _print(format,arg...)   
#else
#ifdef ATP_DEBUG
#define _DEBUG(logfile, format, arg...) /*lint -save -e717*/  do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 0, message);\
}while(0)/*lint -restore*/

#define _INFO(logfile, format, arg...)  /*lint -save -e717*/  do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 1, message);\
}while(0)/*lint -restore*/

#define _WARN(logfile, format, arg...)  /*lint -save -e717*/  do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 2, message);\
}while(0)/*lint -restore*/

#define _HIGH(logfile, format, arg...)   /*lint -save -e717*/ do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 3, message);\
}while(0)/*lint -restore*/

#define _ERROR(logfile, format, arg...)  /*lint -save -e717*/  do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 4, message);\
}while(0)/*lint -restore*/

#define _FATAL(logfile, format, arg...)  /*lint -save -e717*/  do{ \
    char message[MAX_MSG_LEN+2];\
    snprintf(message,MAX_MSG_LEN+1,"[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
    message[MAX_MSG_LEN+1] = '\0'; \
    _syslog( logfile, 5, message);\
}while(0)/*lint -restore*/
#define _print(format,arg...)  /*lint -save -e717*/  do{\
    fprintf(stderr, "[%16s: %4d]:" format "\n", __FUNCTION__, __LINE__,##arg);\
}while(0)/*lint -restore*/
#else
#define _DEBUG(logfile, format, arg...)
#define _INFO(logfile, format, arg...) logprintf_info(format, ##arg)
#define _WARN(logfile, format, arg...)
#define _HIGH(logfile, format, arg...) logprintf_info(format, ##arg)
#define _ERROR(logfile, format, arg...) logprintf_error(format, ##arg)
#define _FATAL(logfile, format, arg...)
#define _print(format,arg...)
#endif
#endif/*end of SUPPORT_ATP_PCLINT*/
/* DTS2013061803625 z81004485 2013/6/18 end> */
#endif // __UTILS_H
