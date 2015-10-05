#include "libbb.h"
#include "atpcommon.h"

extern struct hostent *ATP_GPL_Gethostbyname(const char *name, int family, char* bindaddr);

#if ENABLE_GETOPT_LONG
const char bb_transtool_long_options[] ALIGN1 =
    "download\0" No_argument        "g"
	"upload\0"   No_argument        "s"
	"verbose\0"  No_argument        "v"
	"username\0" Required_argument  "u"
	"password\0" Required_argument  "p"
	"local\0"    Required_argument  "l"
	"remote\0"   Required_argument  "r"
	"port\0"     Required_argument  "P"
	"bind\0"     Required_argument  "B"
	"addr\0"     Required_argument  "A"
    ;
#endif

#ifdef ENABLE_FEATURE_IPV6
extern int iFTPflag;
#endif
int xbind_connect(const len_and_sockaddr *lsa, const char *ip)
{
	int fd;
#ifdef SUPPORT_ATP_HYBRID_BONDING
    // for telecom list 
    unsigned int  ulMark = NON_BLACKLIST_MARK;
#endif    

    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = ATP_CONNECT_TIMEOUT_D;  // 每个命令的超时检测
    /*End of MNT 2008-10-13 14:40 for by z65940*/

    if (NULL == ip)
    {
        // No bind, the same as xconnect_stream
#ifdef SUPPORT_ATP_HYBRID_BONDING        
        fd = xsocket(lsa->sa.sa_family, SOCK_STREAM, 0);
        setsockopt(fd, SOL_SOCKET, SO_MARK, &ulMark, sizeof(ulMark));        
        xconnect(fd, &lsa->sa, lsa->len);        
#else
        fd = xconnect_stream(lsa);
#endif
        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = -1;
        /*End of MNT 2008-10-13 14:40 for by z65940*/
        return fd;
    }

    // Bind to specified local interface
    //fd = create_and_bind_stream_or_die(ip, get_nport(&(lsa->sa)));
    fd = create_and_bind_stream_or_die(ip, 0);
    xconnect(fd, &(lsa->sa), lsa->len);
    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;
    /*End of MNT 2008-10-13 14:40 for by z65940*/
	return fd;
}

void xconnect_tr143(int s, const struct sockaddr *s_addr, socklen_t addrlen, tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    if (connect(s, s_addr, addrlen) < 0) {
		if (ENABLE_FEATURE_CLEAN_UP)
			close(s);
		if (s_addr->sa_family == AF_INET)
		{
                    SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
			bb_perror_msg_and_die("%s (%s)",
				"cannot connect to remote host",
				inet_ntoa(((struct sockaddr_in *)s_addr)->sin_addr));
		}
             SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
		bb_perror_msg_and_die("cannot connect to remote host");
	}
}

int xbind_connect_tr143(const len_and_sockaddr *lsa, const char *ip, tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    int fd;
    struct sockaddr_in stBindAddr;
    int int_1 = 1;
#ifdef SUPPORT_ATP_HYBRID_BONDING	
    // for telecom list 
    unsigned int  ulMark = NON_BLACKLIST_MARK;
#endif

    g_TimeoutCnt = ATP_CONNECT_TIMEOUT_D;  // 每个命令的超时检测
   
    fd = socket(lsa->sa.sa_family, SOCK_STREAM, 0);
    if(0 > fd)
    {
        SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
        bb_perror_msg_and_die("socket");
    }

    if (NULL != ip)
    {
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &int_1, sizeof(int_1));
        memset((void *)(&stBindAddr), 0, sizeof(stBindAddr));
        stBindAddr.sin_family = lsa->sa.sa_family;
        stBindAddr.sin_addr.s_addr = inet_addr(ip);
        bind(fd, (struct sockaddr *)(&stBindAddr), sizeof(stBindAddr));
    }
#ifdef SUPPORT_ATP_HYBRID_BONDING    
    else
    {
        setsockopt(fd, SOL_SOCKET, SO_MARK, &ulMark, sizeof(ulMark));
    }
#endif

    xconnect_tr143(fd, &(lsa->sa), lsa->len, pstDiagState, bDiagFlag);
    g_TimeoutCnt = -1;
    return fd;
}

#if ENABLE_FEATURE_ATP_WGET_ZIP || ENABLE_FEATURE_ATP_FTP_ZIP || ENABLE_FEATURE_ATP_MCAST_ZIP
void atp_zip_write(gzFile fd, const void *buf, size_t count)
{
    int bytesWrite;

    if (!g_bCompress) {
        xwrite((int)fd, buf, count);
        return;
    }

    // Add to zip file
    bytesWrite = gzwrite(fd, buf, count);
    if ((size_t)bytesWrite != count) {
        bb_error_msg_and_die("zip write");
    }
}

int atp_copy_to_zipfile(int src_fd, gzFile dst_file, int filesize)
{
    int status = -1;
	off_t total = 0;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
	char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
	int buffer_size = sizeof(buffer);
#else
	char *buffer;
	int buffer_size;

	/* We want page-aligned buffer, just in case kernel is clever
	 * and can do page-aligned io more efficiently */
	buffer = mmap(NULL, CONFIG_FEATURE_COPYBUF_KB * 1024,
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON,
			/* ignored: */ -1, 0);
	buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
	if (buffer == MAP_FAILED) {
		buffer = alloca(4 * 1024);
		buffer_size = 4 * 1024;
	}
#endif

	if ((src_fd < 0) || (NULL == dst_file))
		goto out;

    if (filesize <= 0) {
        filesize = buffer_size;
        status = 1; /* copy until eof */
    }

	while (1) {
		ssize_t rd;

        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 收取每个包最长45秒
        /*End of MNT 2008-10-13 14:40 for by z65940*/
		rd = safe_read(src_fd, buffer, filesize > buffer_size ? buffer_size : filesize);
        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = -1;
        /*End of MNT 2008-10-13 14:40 for by z65940*/

		if (!rd) { /* eof - all done */
			status = 0;
			break;
		}
		if (rd < 0) {
			bb_perror_msg(bb_msg_read_error);
			break;
		}
		atp_zip_write(dst_file, (const void *)buffer, rd);
		total += rd;
		if (status < 0) { /* if we aren't copying till EOF... */
			filesize -= rd;
			if (!filesize) {
				/* 'size' bytes copied - all done */
				status = 0;
				break;
			}
		}
	}
 out:

#if CONFIG_FEATURE_COPYBUF_KB > 4
	if (buffer_size != 4 * 1024)
		munmap(buffer, buffer_size);
#endif
	return status ? -1 : total;
}
#endif

/*Start of MNT 2008-10-13 14:36 for 传输超时检测 by z65940*/
static void transfer_timeout(void)
{
    //fprintf(stderr, "g_TimeoutCnt: %d\n", g_TimeoutCnt);

    if (g_TimeoutCnt < 0)
    {
        alarm(1);
        return;
    }
    else if (g_TimeoutCnt > 0)
    {
        g_TimeoutCnt -= 1;
        alarm(1);
        return;
    }

    // Timeout
    xfunc_error_retval = ATP_TRANS_FILE_ERR;
    bb_error_msg_and_die("Transfer timeout!\n");
    exit(ATP_TRANS_TIMEOUT);
}

// Setup signals
void atp_setup_alarm(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_RESTART;

    //sa.sa_handler = (void (*)(int)) finish_detection;
    //sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = (void (*)(int))transfer_timeout;
    sigaction(SIGALRM, &sa, NULL);
    alarm(1);
}

off_t atp_copy_fd_with_timeoutEx(int src_fd, int dst_fd, off_t size, PFATPCopyFdCheck fpCheck)
{
    int status = -1;
    off_t total = 0;
    ssize_t wr;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
    char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
    enum { buffer_size = sizeof(buffer) };
#else
    char *buffer;
    int buffer_size;

    /* We want page-aligned buffer, just in case kernel is clever
        * and can do page-aligned io more efficiently */
    buffer = mmap(NULL, CONFIG_FEATURE_COPYBUF_KB * 1024,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANON,
    /* ignored: */ -1, 0);
    buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
    if (buffer == MAP_FAILED) {
        buffer = alloca(4 * 1024);
        buffer_size = 4 * 1024;
    }
#endif

    if (src_fd < 0)
        goto out;

    if (!size) {
        size = buffer_size;
        status = 1; /* copy until eof */
    }

    while (1) 
    {
        ssize_t rd;

        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
        rd = safe_read(src_fd, buffer, size > buffer_size ? buffer_size : size);
        g_TimeoutCnt = -1;

        if (!rd) { /* eof - all done */
            status = 0;
            break;
        }
        if (rd < 0) {
            bb_perror_msg(bb_msg_read_error);
            break;
        }
        /* dst_fd == -1 is a fake, else... */
        if (dst_fd >= 0) {
            if ((NULL != fpCheck) && (fpCheck(buffer, rd) < 0))
            {
                bb_perror_msg(bb_msg_invalid_date);
                break;
            }
            
            g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
            wr = full_write(dst_fd, buffer, rd);
            g_TimeoutCnt = -1;
            if (wr < rd) {
                bb_perror_msg(bb_msg_write_error);
                break;
            }
        }
        total += rd;
        if (status < 0) { /* if we aren't copying till EOF... */
            size -= rd;
            if (!size) {
                /* 'size' bytes copied - all done */
                status = 0;
                break;
            }
        }
    }
 out:

#if CONFIG_FEATURE_COPYBUF_KB > 4
    if (buffer_size != 4 * 1024)
        munmap(buffer, buffer_size);
#endif
    return status ? -1 : total;
}

off_t atp_copy_fd_with_timeout(int src_fd, int dst_fd, off_t size)
{
    return atp_copy_fd_with_timeoutEx(src_fd, dst_fd, size, NULL);
}

/*End of MNT 2008-10-13 14:36 for by z65940*/
#ifdef ENABLE_FEATURE_IPV6
int ATP_TransHostname2IP(char * hostname,char * buffer,int len,int * pIPflag)
{
    struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
    struct hostent* h = NULL;
    
    if(NULL == hostname)
    {
        return 0;
    }
    if (inet_pton(AF_INET6, hostname, &sin6.sin6_addr) > 0)    
    {
        memset(buffer,0,len);
        strcpy(buffer,hostname);
        *pIPflag = ATP_IPV6_IP;
        return 1;
    }
    else if (inet_pton(AF_INET, hostname, &sin.sin_addr) > 0)    
    {
        memset(buffer,0,len);
        strcpy(buffer,hostname);
        *pIPflag = ATP_IPV4_IP;
        return 1;
    }
    h = ATP_GPL_Gethostbyname(hostname, AF_INET6, NULL);
    if (h == NULL)
    {
        h = ATP_GPL_Gethostbyname(hostname, AF_INET, NULL);
    }
    else
    {
        memcpy(sin6.sin6_addr.s6_addr, (char*)*h->h_addr_list, 16);
        memset(buffer,0,len);
        inet_ntop(AF_INET6, &sin6.sin6_addr, buffer, INET6_ADDRSTRLEN);
        *pIPflag = ATP_IPV6_IP;
        return 1;
    }
    if (h == NULL)
    {
        return 0;
    }
    else
    {  
        sin.sin_addr.s_addr = *(int*)*h->h_addr_list;
        memset(buffer,0,len);
        strncpy(buffer ,inet_ntoa(sin.sin_addr),INET_ADDRSTRLEN);
        *pIPflag = ATP_IPV4_IP;
        return 1;
    }
}
#endif

#ifdef SUPPORT_ATP_TR143_DOWNLOAD
static int DownDiagResultSave(tr143_diag_state_t *pstDiagState)
{
    // printf("\r\n do store \r\n");
     FILE  *pfTr143Stats = NULL;
     unlink(TR143_DOWNDIAG_PID_FILE);
     pfTr143Stats = fopen (TR143_DOWNDIAG_RESULT_FILE , "w");
     if (pfTr143Stats == NULL)
     {
       return ATP_TRANS_SYS_ERR;
     }
     rewind(pfTr143Stats);
     fprintf(pfTr143Stats, "State = %s \n", pstDiagState->szState);
     fprintf(pfTr143Stats, "Rtime = %s \n", pstDiagState->szRomTime);
     fprintf(pfTr143Stats, "Btime = %s \n", pstDiagState->szBomTime);
     fprintf(pfTr143Stats, "Etime = %s \n", pstDiagState->szEomTime);
     fprintf(pfTr143Stats, "TextbytesRecv = %u \n", pstDiagState->ulTextBytesReceived);
     fprintf(pfTr143Stats, "TotalbytesRecv= %u \n", pstDiagState->ulTotalBytesReceived);
 #ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
     fprintf(pfTr143Stats, "TcpRequestTime = %s \n", pstDiagState->szTcpOpenRequestTime);
     fprintf(pfTr143Stats, "TcpResponseTime = %s \n", pstDiagState->szTcpOpenResponseTime);
 #endif
     fclose(pfTr143Stats);
     //free(pstDiagState);
     return EXIT_SUCCESS;
}

void SetDownDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag)
{
     if(TRUE == bFlag)
     {
          snprintf(pstDiagState->szState, ATP_STRING_LEN_32, "%s", pcState);
          DownDiagResultSave(pstDiagState);
          if((1 == pstDiagState->ulTableFlag) && (0 != strcmp("Completed", pcState)))
          {
            AddOrDelTablesRule(pstDiagState,"-D",  "OUTPUT", "-o");
          }
     }  
}
#endif

#ifdef SUPPORT_ATP_TR143_UPLOAD
static int UpDiagResultSave(tr143_diag_state_t *pstDiagState)
{
    // printf("\r\n do store \r\n");
     FILE  *pfTr143Stats = NULL;
     unlink(TR143_UPDIAG_PID_FILE);
     pfTr143Stats = fopen (TR143_UPDIAG_RESULT_FILE, "w");
     if (pfTr143Stats == NULL)
     {
       return ATP_TRANS_SYS_ERR;
     }
     rewind(pfTr143Stats);
     fprintf(pfTr143Stats, "State = %s \n", pstDiagState->szState);
     fprintf(pfTr143Stats, "Rtime = %s \n", pstDiagState->szRomTime);
     fprintf(pfTr143Stats, "Btime = %s \n", pstDiagState->szBomTime);
     fprintf(pfTr143Stats, "Etime = %s \n", pstDiagState->szEomTime);
     fprintf(pfTr143Stats, "TotabytesSent = %u \n", pstDiagState->ulTotalBytesSent); 
 #ifdef SUPPORT_ATP_TR143_UPLOAD_TCP      
     fprintf(pfTr143Stats, "TcpRequestTime = %s \n", pstDiagState->szTcpOpenRequestTime);
     fprintf(pfTr143Stats, "TcpResponseTime = %s \n", pstDiagState->szTcpOpenResponseTime);
 #endif
     fclose(pfTr143Stats);
     //free(pstDiagState);
     return EXIT_SUCCESS;
}

void SetUpDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag)
{
     if(TRUE == bFlag)
     {
          snprintf(pstDiagState->szState, ATP_STRING_LEN_32, "%s", pcState);
          UpDiagResultSave(pstDiagState);
          if((1 == pstDiagState->ulTableFlag) && (0 != strcmp("Completed", pcState)))
          {
            AddOrDelTablesRule(pstDiagState,"-D",  "OUTPUT", "-o");
          }
     }  
}

int diag_copy_fd_with_timeout( int dst_fd, int size)
{
    int status = -1;
    int total = 0;
    ssize_t wr = 0;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
    char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
    enum { buffer_size = sizeof(buffer)-4  };
#else
    char *buffer;
    int buffer_size;      
    /* We want page-aligned buffer, just in case kernel is clever
     * and can do page-aligned io more efficiently */
    buffer = mmap(NULL, (CONFIG_FEATURE_COPYBUF_KB * 1024),
    		PROT_READ | PROT_WRITE,
    		MAP_PRIVATE | MAP_ANON,
    		/* ignored: */ -1, 0);
    buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
    if (buffer == MAP_FAILED) {
        buffer = alloca(4 * 1024);
        buffer_size = 4 * 1024;
    }
#endif     
    if (!size)
    {
        return status;
    }
    if(size >= buffer_size)
    {
        memset(buffer, 'a', buffer_size);
        while(size >= buffer_size)
        {
            if (dst_fd >= 0) 
            {
                g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
                wr = full_write(dst_fd, buffer, buffer_size);
                g_TimeoutCnt = -1;
                if (wr < buffer_size) 
                {
                    bb_perror_msg(bb_msg_write_error);
                    return status;
                }
            }
            total += wr;
            size -= wr;
        }       
    }
    memset(buffer, '\0', buffer_size);
    memset(buffer, 'a', size);
    g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
    wr = full_write(dst_fd, buffer, size);
    g_TimeoutCnt = -1;
    if (wr < size) 
    {
        bb_perror_msg(bb_msg_write_error);
        return status;
    }
    total += wr;
       
#if CONFIG_FEATURE_COPYBUF_KB > 4
    if (buffer_size != 4 * 1024)
    {
        munmap(buffer, buffer_size);
    }
#endif
    return  total;
}
#endif

#if defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD)

int GetCurDateTime(char *pcBuf, int ulBufLen)
{
    struct tm   *curdate;
    struct timeval  stTimeVal;
   int       ret =0;
    
    if(NULL == pcBuf)
    {
        return EXIT_SUCCESS;
    }
    if ((ret = gettimeofday(&stTimeVal, NULL)) < 0)
    {
        return EXIT_SUCCESS;
    }
    curdate = gmtime((time_t *)(&(stTimeVal.tv_sec)));
    /*End of ATP网络协议组 2010-4-1 10:38 by h00163136*/
    //strftime(pcBuf, ulBufLen, "%a, %d %b %Y %H:%M:%S", curdate);
    strftime(pcBuf, ulBufLen, "%Y-%m-%dT%H:%M:%S", curdate);
    snprintf(pcBuf,ulBufLen, "%s.%d",pcBuf, (int)stTimeVal.tv_usec); 

    return EXIT_SUCCESS;
}

void SetDiagState(tr143_diag_state_t *pstDiagState, const char * pcState, bool bFlag)
{                
      if(TRANS_TOOL_OPT_DOWNLOAD == pstDiagState->ulTransDir)
      {    
            SetDownDiagState(pstDiagState, pcState, bFlag);
      }               
      else
      {
           SetUpDiagState(pstDiagState, pcState, bFlag);   
      }
}

void ATP_UTIL_GetIfcStatInfo(char *pszIfcName, ATP_UTIL_INTERFACE_STAT_ST *pstState)
{
    int iCount = 0;
    char acCol[17][32];
    char *pszChar = NULL;
    FILE *fs      = NULL;

    char acLine[512], acBuf[512];

    memset(acCol, 0, sizeof(acCol));
    memset(acLine, 0, sizeof(acLine));
    memset(acBuf,  0, sizeof(acBuf));

    fs = fopen("/proc/net/dev", "r");

    if (NULL == fs)
    {
        return;
    }

    if ((NULL == pszIfcName) || (NULL == pstState))
    {
        fclose(fs);
        return;
    }
#ifdef ATP_PCLINT
    pszIfcName = pszIfcName;
#endif

    while (NULL != fgets(acLine, sizeof(acLine), fs) )
    {
        /* 忽略文件开始2行 */
        if ((iCount++) < 2 )
        {
            continue;
        }

        /* normally line will have the following example value
           "eth0: 19140785 181329 0 0 0 0 0 0 372073009 454948 0 0 0 0 0 0"
           but when the number is too big then line will have the following example value
           "eth0:19140785 181329 0 0 0 0 0 0 372073009 454948 0 0 0 0 0 0"
           so to make the parsing correctly, the following codes are added
           to insert space between ':' and number */
        pszChar = strchr(acLine, ':');
        if (NULL != pszChar)
        {
            pszChar++;
        }
        if ((NULL != pszChar) && isdigit(*pszChar) )
        {
            //strcpy(acBuf, pszChar);
            snprintf(acBuf, sizeof(acBuf), "%s", pszChar);
            *pszChar = ' ';
           strcpy(++pszChar, acBuf);
        }

        /* 如果接口被找到，则存储统计值 */
        if (NULL != strstr(acLine, pszIfcName) )
        {
            sscanf(acLine, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                   acCol[0], acCol[1], acCol[2], acCol[3], acCol[4], acCol[5],
                   acCol[6], acCol[7], acCol[8], acCol[9], acCol[10], acCol[11],
                   acCol[12],acCol[13], acCol[14], acCol[15], acCol[16]);
            pstState->ulBytesReceived        = strtoul(acCol[1],0,10);
            pstState->ulPacketsReceived      = strtoul(acCol[2],0,10);          
            pstState->ulPacketsReceivedErr   = strtoul(acCol[3],0,10);
            pstState->ulPacketsReceivedDrops = strtoul(acCol[4],0,10);
            pstState->ulBytesSent            = strtoul(acCol[9],0,10);
            pstState->ulPacketsSent          = strtoul(acCol[10],0,10);
            pstState->ulPacketsSentErr       = strtoul(acCol[11],0,10);
            pstState->ulPacketsSentDrops     = strtoul(acCol[12],0,10);

            break;
        }
    }
    fclose(fs);
}

#define ATP_SYS_GIANT_LEN                   1024

#ifdef SUPPORT_ATP_QUEUE
void  AddOrDelTablesRule(tr143_diag_state_t  *pstDiagState, const char *pcAction, const char *pcChain, const char *pcInOROut)
{ 
    struct in_addr        stAddr_4; 
    char        szCmd[ATP_SYS_GIANT_LEN] = {0};

    if (inet_pton(AF_INET, pstDiagState->szRemoteIP, &stAddr_4) > 0 )
    {
        if (0 != pstDiagState->ulEthPriority)
        {
            snprintf(szCmd, sizeof(szCmd), "ebtables -t filter %s %s -p ipv4  --ip-proto 6 %s %s  --ip-dst %s --ip-dport %u --vlan-prio %u",
                pcAction, pcChain, pcInOROut, pstDiagState->szIfName, pstDiagState->szRemoteIP, pstDiagState->ulPort, pstDiagState->ulEthPriority);
            ATP_UTIL_ForkProcess(szCmd, -1, NULL, NULL);
        }
    }
    else
    {
        return;
    }
    if ( 0 != pstDiagState->ulDscp)
    {
        snprintf(szCmd, sizeof(szCmd), "iptables -t mangle %s %s %s %s  -d %s  -p tcp --dport %u -j DSCP --set-dscp 0x%x",
           pcAction, pcChain, pcInOROut, pstDiagState->szIfName, pstDiagState->szRemoteIP, pstDiagState->ulPort, pstDiagState->ulDscp);
        ATP_UTIL_ForkProcess(szCmd, -1, NULL, NULL);
    }
}
#endif
#endif


/*
  * For file upload
  */
#define ATP_IMAGE_HEAD_LEN      (1024)      /* 接收1k包头 */
#define ATP_IMAGE_HEAD_PATH     "/var/image.head.busybox"

static char             m_pcImageHead[ATP_IMAGE_HEAD_LEN];
static unsigned  int    m_ulImageHeadLen = 0;
static char             *m_pcCfgTool = NULL;

void ATPUpgradePrecheckInit(char *pcCfgTool)
{
    m_pcCfgTool = pcCfgTool;
    m_ulImageHeadLen = 0;
}    

void ATPUpgradePrecheckDestroy(void)
{    
    unlink(ATP_IMAGE_HEAD_PATH);
}

int ATPUpgradePrecheckProc(const char *pszBuf, int ulLen)
{
    unsigned  int       ulCopyLen    = 0;
    FILE                *pfFile      = VOS_NULL;
    unsigned  int       lExitCode    = 0;
    char                acCmd[128]   = {0};

    if (NULL == m_pcCfgTool)
    {
        return 0;
    }

    if (m_ulImageHeadLen > ATP_IMAGE_HEAD_LEN)
    {
        return 0;
    }
    
    ulCopyLen = ATP_IMAGE_HEAD_LEN - m_ulImageHeadLen;
    if (ulLen < ulCopyLen)
    {
        ulCopyLen = ulLen;
    }

    memcpy(m_pcImageHead + m_ulImageHeadLen, pszBuf, ulCopyLen);
    m_ulImageHeadLen += ulCopyLen;

    // 已接收1k包头, 写文件起upg进程校验
    if (ATP_IMAGE_HEAD_LEN == m_ulImageHeadLen)
    {
        pfFile = fopen(ATP_IMAGE_HEAD_PATH, "wb");
        if (VOS_NULL == pfFile)
        {
            return -1;
        }

        fwrite(m_pcImageHead, 1, ATP_IMAGE_HEAD_LEN, pfFile);

        fclose(pfFile);
        m_ulImageHeadLen ++;

        snprintf(acCmd, sizeof(acCmd), "upg -v -f " ATP_IMAGE_HEAD_PATH " -c %s", m_pcCfgTool);
        ATP_UTIL_ForkProcess(acCmd, -1, NULL, &lExitCode);
   
        if (VOS_OK != lExitCode)
        {
            printf("head check error.\n");
            return -1; 
        }

        printf("head check ok .\n");
    }

    return 0;
}
