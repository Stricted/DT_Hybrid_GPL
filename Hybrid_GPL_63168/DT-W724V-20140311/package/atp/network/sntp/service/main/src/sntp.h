#ifndef __SNTP_H__
#define __SNTP_H__

#define TIME_OFFSET_KERNEL_PROC_PATH "/proc/sys/kernel/sysctl_localtime_offset"
#define NTPPORT 123   // NTP UDP port number
#define NTPTIMEOUT 2  // retry timeout in seconds
#define MAXRETRY 1    // number of times to retry

#ifdef SUPPORT_ATP_TR098
#define SUCCESS_INTERVAL (24 * 60 * 60)     /* 同步成功默认24小时之后再次同步 */
#define FAIL_INTERVAL 5
#elif SUPPORT_ATP_TR181
#define SUCCESS_INTERVAL    (6*60*60)
#define FAIL_INTERVAL 15
#endif


/* very portable; CRIS has 4-byte doubles and 8-byte long doubles
 * and we need 8-byte precision...
 */
typedef double ntp_double;


struct ntp_stamp
{
	unsigned long sec, frac;
};

/* NTP message format v4, from RFC 2030

                     1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |LI | VN  |Mode |    Stratum    |     Poll      |   Precision   |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                          Root Delay                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       Root Dispersion                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     Reference Identifier                      |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                   Reference Timestamp (64)                    |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                   Originate Timestamp (64)                    |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                    Receive Timestamp (64)                     |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                    Transmit Timestamp (64)                    |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                 Key Identifier (optional) (32)                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                                                               |
      |                 Message Digest (optional) (128)               |
      |                                                               |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

  Size of all required fields is 48 bytes. Including optional fields, 68.

*/

struct ntpmsg
{
	unsigned long flags;
	unsigned long root_delay, root_dispersion, ref_id;
	struct ntp_stamp ref_stamp;      // time our clock was last set
	struct ntp_stamp orig_stamp;     // time we sent the request to the server      (T1)
	struct ntp_stamp receive_stamp;  // time the server received the request        (T2)
	struct ntp_stamp transmit_stamp; // time the server sent back the request to us (T3)
};

#define TO_FRAC(x) ((unsigned long)((((ntp_double)x) * 1.0e-6) * NTP_SCALE))
#define TO_USEC(x) ((unsigned long)((((ntp_double)x) * NTP_SCALE_I) * 1.0e6))


#define SNTP_PID_FILE         "/var/sntp/pidfile"

/*调试宏*/
//#define SNTP_DEBUG
#ifdef ATP_DEBUG
#define SntpDebug(str, args...) do { printf("\r\n(%s  %d)    ", __FUNCTION__, __LINE__);  printf("\033[1;33;40m"); printf(str, ## args); \
       printf("\033[0;37;40m\r\n");} while(0)
#else
#define SntpDebug(str, args...) 
#endif

typedef struct
{
    int lActive;
    char acServer[ATP_SNTP_SERVER_MAXLEN];
}NTP_SERVER;


/* sntp 夏令时格式算出来的具体对应到的几月几号 */
typedef struct
{
    unsigned int     ulRealYear;
    unsigned int     ulRealMonth;
    unsigned int     ulRealDay;
    unsigned int     ulOffsetTime;
}SNTP_DST_REAL_ST;

#endif
