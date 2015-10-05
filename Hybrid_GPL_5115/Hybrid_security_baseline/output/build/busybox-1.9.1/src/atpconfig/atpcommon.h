#ifndef __ATP_COMMON_H__
#define __ATP_COMMON_H__

#include "platform.h"
#include "atputil.h"

#if ENABLE_FEATURE_ATP_WGET_ZIP || ENABLE_FEATURE_ATP_FTP_ZIP || ENABLE_FEATURE_ATP_TFTP_ZIP || ENABLE_FEATURE_ATP_MCAST_ZIP
typedef void * gzFile;

extern gzFile gzopen(const char *path, const char *mode);
extern int gzwrite(gzFile file, const void *buf, unsigned int len);
extern int gzclose(gzFile file);

#endif

/* ftpget ftpput tftp wget传输工具的返回码 */
#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

/* ftpget ftpput tftp wget传输工具的命令行选项 */
#define TRANS_TOOL_OPT_DOWNLOAD	    1
#define TRANS_TOOL_OPT_UPLOAD	    2
#define TRANS_TOOL_OPT_VERBOSE	    4
#define TRANS_TOOL_OPT_USER	        8
#define TRANS_TOOL_OPT_PASSWORD	    16
#define TRANS_TOOL_OPT_LOCALPATH	32
#define TRANS_TOOL_OPT_REMOTEPATH	64
#define TRANS_TOOL_OPT_REMOTEPORT	128
#define TRANS_TOOL_OPT_BINDIP       256
#define TRANS_TOOL_OPT_REMOTEIP     512
// 1024,2048,4096
#define TRANS_TOOL_OPT_ZIP	        8192

#ifdef ENABLE_FEATURE_IPV6
#define ATP_IPV4_IP     0
#define ATP_IPV6_IP     1
#define ATP_LEN_64      64
#endif

/*Diagbostic record files*/
#ifdef SUPPORT_ATP_TR143_DOWNLOAD
#define TR143_DOWNDIAG_RESULT_FILE "/var/DownloadDiagState"
#define TR143_DOWNDIAG_PID_FILE    "/var/DownloadDiagPid"
#endif
#ifdef SUPPORT_ATP_TR143_UPLOAD
#define TR143_UPDIAG_RESULT_FILE "/var/UploadDiagState"
#define TR143_UPDIAG_PID_FILE    "/var/UploadDiagPid"
#endif

#if ENABLE_GETOPT_LONG
/* ftp wget和tftp传输工具的命令行用法说明 */
extern const char bb_transtool_long_options[];
#endif

typedef struct  tagtr143_diag_state_t{
/* None, Requested, Completed, Error_InitConnectionFailed, Error_NoResponse,Error_TransferFailed,Error_PasswordRequestFailed, Error_LoginFailed,Error_NoTransferMode,Error_NoPASV,Error_NoCWD,Error_NoSTOR,Error_NoTransferComplete */
  char szState[ATP_STRING_LEN_32];      
  //int ulTextFileLength;
  char  szRomTime[ATP_STRING_LEN_64];             /* in ms */
  char  szBomTime[ATP_STRING_LEN_64];
  char  szEomTime[ATP_STRING_LEN_64];
  unsigned int    ulTextBytesReceived;
  unsigned int    ulTotalBytesReceived;
  unsigned int    ulTotalBytesSent;
  char   szTcpOpenRequestTime[ATP_STRING_LEN_64]; 
  char   szTcpOpenResponseTime[ATP_STRING_LEN_64];
  char   szIfName[32];
  char   szRemoteIP[ATP_LEN_64];
  unsigned int      ulTransDir;
  unsigned int      ulDscp;
  unsigned int      ulEthPriority;
  unsigned int      ulPort;
  unsigned int      ulTableFlag;
} tr143_diag_state_t;

// 使用全局内存，更加节省内存
struct atp_trans_globals {
	int g_lTransEnd;            // 传输结束位置
	int g_lMaxTrans;            // 下载文件最大大小
    const char *g_pcLocalIP;    // 本地绑定IP
    const char *g_pcTransBegin; // 传输开始地址
    const char *g_pcTransEnd;   // 传输结束地址

    int         bCompress;

    unsigned int g_ulTotalLen;
    int mcast_seq_num;
    int mcast_len;
    unsigned char *mcast_buf;

    int         timout_cnt;
    /*BEGIN:added by l00192646,2012/07/21,for suota*/
    #ifdef SUPPORT_ATP_VOICE_DT
    const char  *pcPassword;
    #endif
    /*END:added by l00192646,2012/07/21,for suota*/
};

#define HANDY (*(struct atp_trans_globals*)&bb_common_bufsiz1)

#define g_pcLocalIP     (HANDY.g_pcLocalIP    )
#define g_pcTransBegin  (HANDY.g_pcTransBegin )
#define g_pcTransEnd    (HANDY.g_pcTransEnd   )
#define g_lTransEnd     (HANDY.g_lTransEnd    )
#define g_lMaxTrans     (HANDY.g_lMaxTrans    )

#define g_ulTotalLen    (HANDY.g_ulTotalLen   )
#define g_lMcastSeqNum  (HANDY.mcast_seq_num  )
#define g_lMcastLen     (HANDY.mcast_len      )
#define g_pucMcastBuf   (HANDY.mcast_buf      )
#define g_bCompress     (HANDY.bCompress      )

#define g_TimeoutCnt    (HANDY.timout_cnt     )

/*BEGIN:added by l00192646,2012/07/21,for suota*/
#ifdef SUPPORT_ATP_VOICE_DT
#define g_CertPwd       (HANDY.pcPassword     )
#endif
/*END:added by l00192646,2012/07/21,for suota*/


int xbind_connect(const len_and_sockaddr *lsa, const char *ip);
void xconnect_tr143(int s, const struct sockaddr *s_addr, socklen_t addrlen, tr143_diag_state_t *pstDiagState, bool bDiagFlag);
int xbind_connect_tr143(const len_and_sockaddr *lsa, const char *ip, tr143_diag_state_t *pstDiagState, bool bDiagFlag);

#if ENABLE_FEATURE_ATP_WGET_ZIP || ENABLE_FEATURE_ATP_FTP_ZIP || ENABLE_FEATURE_ATP_MCAST_ZIP
void atp_zip_write(gzFile fd, const void *buf, size_t count);
#define atp_write atp_zip_write

int atp_copy_to_zipfile(int src_fd, gzFile dst_file, int filesize);
#else
#define atp_write xwrite
#endif

/*Start of MNT 2008-10-13 14:36 for 传输超时检测 by z65940*/
#define ATP_CONNECT_TIMEOUT_D   (20)    // 20秒连接超时
#define ATP_PACK_TIMEOUT_D      (30)    // 30秒没有收到报文则超时

void atp_setup_alarm(void);
off_t atp_copy_fd_with_timeout(int src_fd, int dst_fd, off_t size);
typedef int (*PFATPCopyFdCheck)(const char *buffer, int len);
void ATPUpgradePrecheckInit(char *pcCfgTool);
void ATPUpgradePrecheckDestroy(void);
int ATPUpgradePrecheckProc(const char *pszBuf, int ulLen);
off_t atp_copy_fd_with_timeoutEx(int src_fd, int dst_fd, off_t size, PFATPCopyFdCheck fpCheck);

/*End of MNT 2008-10-13 14:36 for by z65940*/
#ifdef ENABLE_FEATURE_IPV6
int ATP_TransHostname2IP(char * hostname,char* buffer,int len,int * pIPflag);
#endif

#ifdef SUPPORT_ATP_TR143_DOWNLOAD
void SetDownDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag);
//#define SetDownDiagState(x) SetUpDiagStateEx((x))
#else
#define SetDownDiagState(x,y,z)
#endif

#ifdef SUPPORT_ATP_TR143_UPLOAD
void SetUpDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag);
int diag_copy_fd_with_timeout( int dst_fd, int size);
//#define SetUpDiagState(x) SetUpDiagStateEx((x))
#else
#define SetUpDiagState(x,y,z)
#endif

#if defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD)
 int GetCurDateTime(char *pcBuf, int ulBufLen);
void ATP_UTIL_GetIfcStatInfo(char *pszIfcName, ATP_UTIL_INTERFACE_STAT_ST *pstState);
#ifdef SUPPORT_ATP_QUEUE
void  AddOrDelTablesRule(tr143_diag_state_t  *pstDiagState, const char *pcAction, const char *pcChain, const char *pcInOROut);
#else
#define AddOrDelTablesRule(x,y,z,w)
#endif
void SetDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag);
//#define SetDiagState(x) SetDiagStateEx((x), (y),(z))
#else
#define SetDiagState(x,y,z)
#endif
#endif
