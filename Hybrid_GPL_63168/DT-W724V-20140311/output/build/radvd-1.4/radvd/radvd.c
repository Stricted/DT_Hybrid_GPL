/*
 *   $Id: radvd.c,v 1.1 2009/08/22 07:51:01 y42304 Exp $
 *
 *   Authors:
 *    Pedro Roque		<roque@di.fc.ul.pt>
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996-2000 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <sys/file.h>
#include <config.h>
#include <includes.h>
#include <radvd.h>
#include <pathnames.h>
#include <errno.h>
#include "atp_interface.h"
struct Interface *IfaceList = NULL;

char usage_str[] =
	"[-hsv] [-d level] [-C config_file] [-m log_method] [-l log_file]\n"
	"\t[-f facility] [-p pid_file] [-u username] [-t chrootdir]";

#ifdef HAVE_GETOPT_LONG
struct option prog_opt[] = {
	{"debug", 1, 0, 'd'},
	{"config", 1, 0, 'C'},
	{"pidfile", 1, 0, 'p'},
	{"logfile", 1, 0, 'l'},
	{"logmethod", 1, 0, 'm'},
	{"facility", 1, 0, 'f'},
	{"username", 1, 0, 'u'},
	{"chrootdir", 1, 0, 't'},
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{"singleprocess", 0, 0, 's'},
	{NULL, 0, 0, 0}
};
#endif

extern FILE *yyin;

char *conf_file = NULL;
char *pname;
int sock = -1;

volatile int sighup_received = 0;
volatile int sigterm_received = 0;
volatile int sigint_received = 0;

void sighup_handler(int sig);
void sigterm_handler(int sig);
void sigint_handler(int sig);
/* BEGIN: Added by z67728, 2009/11/3 */
void sigdebug_handler(int sig);
void sigprefixchg_handler(int sig);
/* END:   Added by z67728, 2009/11/3 */
void timer_handler(void *data);
#ifdef SUPPORT_ATP_W724V_RA_CONFIG
void timer_handler2(void *data);
#endif
void config_interface(void);
void kickoff_adverts(void);
void stop_adverts(char *pcBridgeName);
void version(void);
void usage(void);
int drop_root_privileges(const char *);
int readin_config(char *);
int check_conffile_perm(const char *, const char *);
int get_advvalidlifetime(void);
/* BEGIN: Added by z67728, 2009/12/26 PN:AU4D02322 */
void disable_oldprefix(char *pcBridgeName);
void disable_oldstataddr(void);
#ifdef SUPPORT_ATP_IPV6_ULA
int  compare_prefix(char * czPrefixBefore, char *pcBridgeName);	
#endif
/* Ready 测试标志位 */
int g_iReadTestFlag = 0;

/* DHCPv6 Server 变化标志位 0 : DHCP configuration no change, 1 : DHCP configuration change  */
int g_iPrefixChgeFlag = 0;
/* END:   Added by z67728, 2009/12/26 */

char g_lBridgeName[128] = {0};

#define RADVD_CONFIGFILE_LOCK_FILE       ("/var/radvd/radvd%s.lock")

/* BEGIN: add for DTS2013100901874 网关重新拨号，网关通告旧前缀中有效 生存时间为6天，不是标书要求的ValidLifetime = 14400 (4 hours) by l00214441 2013.10.18 */ 
#define RADVD_PREFIX_DELEGATION_FILE    ("/var/radvd/radvd_prefix_delegation")
/* END: add for DTS2013100901874 网关重新拨号，网关通告旧前缀中有效 生存时间为6天，不是标书要求的ValidLifetime = 14400 (4 hours) by l00214441 2013.10.18 */ 

#ifdef SUPPORT_ATP_W724V_RA_CONFIG
/* BEGIN: add for DTS2013081504856 【W724v】Improvements for the IPv6 privacy (DTCR-2012-040) by l00214441 2013.07.19 */ 
/* Valid Lifetime 修改为6天*/
#define RELEASE_RA_VALID_LIFETIME 14400
#define RELEASE_RA_VALID_LIFETIME_PREFIX_DELEGATION 518400
/* END: add for DTS2013081504856 【W724v】Improvements for the IPv6 privacy (DTCR-2012-040) by l00214441 2013.07.19 */ 
#define BRIDGE_NAME "br0"
struct AdvOldPrefix {
	struct AdvPrefix Prefix;
	int lSendTime;
};

struct AdvOldPrefix g_stAdvPrefix;

int g_lWanDownFlag = 0;
#else
#define RELEASE_RA_VALID_LIFETIME 0
#endif

/******************************************************************************
  函数名称  : ATP_UTIL_LockFile
  功能描述  : 给制定文件pszFileName加锁
  输入参数  : 
              1.  pszFileName: 待锁文件full name
  输出参数  : 
              1.  fFile: FILE 类型文件指针  
******************************************************************************/
static int LockFile(char *pszFileName, FILE ** fFile)
{
    int iLockFd = -1;
    int iRet = 0;

    //可读可写
    iLockFd = open(pszFileName, O_RDWR|O_CREAT, 0777);
    if (iLockFd < 0)
    {
        printf("ATP_LockFile,Fail to open %s.\n", pszFileName);
        _exit(1);
    }

    do
    {
        iRet = flock(iLockFd, LOCK_EX);
        if (0 == iRet)
        {
            break;            
        }
        else 
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                close(iLockFd);
                printf("ERROR:Fail to flock %s.\n", pszFileName);
                return (-1);
            }
        }
    }while(1);

    if (NULL != fFile)
    {
        *fFile = fdopen(iLockFd, "rw");/*l41296 修改PCLINT告警*/
    }

    return iLockFd;
}

/******************************************************************************
  函数名称  : ATP_UTIL_UnlockFile
  功能描述  : 给制定文件句柄解锁
  输入参数  : 
              1.  iLockFd: 待解锁文件句柄         
******************************************************************************/
static void UnlockFile(int iLockFd)
{
    if (iLockFd < 0)
    {
        printf("UnlockFile %d fail.\n",iLockFd);    
        return;
    }

    flock(iLockFd, LOCK_UN);
    close(iLockFd);    
}

int
main(int argc, char *argv[])
{
    unsigned char msg[MSG_SIZE];
    int c, log_method;
    char *logfile, *pidfile;
    sigset_t oset, nset;
    int facility;
    char *username = NULL;
    char *chrootdir = NULL;
    int singleprocess = 0;
#ifdef HAVE_GETOPT_LONG
    int opt_idx;
#endif
    FILE * pidfp = NULL;
    int    pid   = 0;

    int lfdradvdlck = 0;
    char acFileForLock[64] = {0};

    /*start: 用于查询组件版本号(cwmp atpv),请产品不要修改或删除
    *
    *添加:黄记
    *
    *时间:2012-06-21
    *
	*/
    if ((argc == 2) && (NULL != argv[1]) && (0 == strcmp(argv[1],"atpv")))
    {
       printf("\r\n%s.\n", MODULE_VERSION);    	 	  
       exit(0);
    }
    /*end */

    pname = ((pname=strrchr(argv[0],'/')) != NULL)?pname+1:argv[0];

    srand((unsigned int)time(NULL));

    log_method = L_STDERR_SYSLOG;
    logfile = "/var/radvd/radvd.log";
    conf_file = "/var/radvd/radvdbr0.conf";
    facility = LOG_FACILITY;
    pidfile = "/var/radvd/radvd.pid";

    /* parse args */
#ifdef HAVE_GETOPT_LONG
    while ((c = getopt_long(argc, argv, "d:C:l:m:p:t:u:vhs", prog_opt, &opt_idx)) > 0)
#else
    while ((c = getopt(argc, argv, "d:C:l:m:p:t:u:vhs")) > 0)
#endif
    {
        switch (c) {
            case 'C':
                conf_file = optarg;
                break;
            case 'd':
                set_debuglevel(atoi(optarg));
                break;
            case 'f':
                facility = atoi(optarg);
                break;
            case 'l':
                logfile = optarg;
                break;
            case 'p':
                pidfile = optarg;
                break;
            case 'm':
                if (!strcmp(optarg, "syslog"))
                {
                    log_method = L_SYSLOG;
                }
                else if (!strcmp(optarg, "stderr_syslog"))
                {
                    log_method = L_STDERR_SYSLOG;
                }
                else if (!strcmp(optarg, "stderr"))
                {
                    log_method = L_STDERR;
                }
                else if (!strcmp(optarg, "logfile"))
                {
                    log_method = L_LOGFILE;
                }
                else if (!strcmp(optarg, "none"))
                {
                    log_method = L_NONE;
                }
                else
                {
                    fprintf(stderr, "%s: unknown log method: %s\n", pname, optarg);
                    exit(1);
                }
                break;
            case 't':
                chrootdir = strdup(optarg);
                break;
            case 'u':
                username = strdup(optarg);
                break;
            case 'v':
                version();
                break;
            case 's':
                singleprocess = 1;
                break;
            case 'h':
                usage();
#ifdef HAVE_GETOPT_LONG
            case ':':
                fprintf(stderr, "%s: option %s: parameter expected\n", pname,
                prog_opt[opt_idx].name);
                exit(1);
#endif
            case '?':
                exit(1);
            }
        }

        argc -= optind;
        argv += optind;

        if ( NULL != argv[0] )
        {
            snprintf(g_lBridgeName,sizeof(g_lBridgeName),"%s", argv[0]);
        }
#ifdef SUPPORT_ATP_W724V_RA_CONFIG		
        //为了让mic监听，将文件和接口写死
        conf_file = "/var/radvd/radvdbr0.conf";
        snprintf(g_lBridgeName, sizeof(g_lBridgeName), "%s", BRIDGE_NAME);
#endif    
        if (chrootdir) {
            if (!username) {
            fprintf(stderr, "Chroot as root is not safe, exiting\n");
            exit(1);
        }

        if (chroot(chrootdir) == -1) {
            perror("chroot");
            exit (1);
        }

        if (chdir("/") == -1) {
            perror("chdir");
            exit (1);
        }
        /* username will be switched later */
        }
        
        if (log_open(log_method, pname, logfile, facility) < 0)
            exit(1);

        flog(LOG_INFO, "version %s started", VERSION);

        /* get a raw socket for sending and receiving ICMPv6 messages */
        sock = open_icmpv6_socket();
        if (sock < 0)
            exit(1);

#if 0 
	/* check that 'other' cannot write the file
         * for non-root, also that self/own group can't either
         */
	if (check_conffile_perm(username, conf_file) < 0) {
		if (get_debuglevel() == 0)
			exit(1);
		else
			flog(LOG_WARNING, "Insecure file permissions, but continuing anyway");
	}
	
	/* if we know how to do it, check whether forwarding is enabled */
	if (check_ip6_forwarding()) {
		if (get_debuglevel() == 0) {
			flog(LOG_ERR, "IPv6 forwarding seems to be disabled, exiting");
			exit(1);
		}
		else
			flog(LOG_WARNING, "IPv6 forwarding seems to be disabled, but continuing anyway.");
	}
#endif

        /* parse config file */
        snprintf(acFileForLock, sizeof(acFileForLock), RADVD_CONFIGFILE_LOCK_FILE,g_lBridgeName);
        lfdradvdlck = LockFile(acFileForLock, NULL);   
        if (readin_config(conf_file) < 0)
        {
            printf("===========readin_config==error======================\n");
            UnlockFile(lfdradvdlck);        
            exit(1);
        } 
        UnlockFile(lfdradvdlck);        

        /* drop root privileges if requested. */
        if (username) {
            if (!singleprocess) {
                dlog(LOG_DEBUG, 3, "Initializing privsep");
                if (privsep_init() < 0)
                    flog(LOG_WARNING, "Failed to initialize privsep.");
            }

            if (drop_root_privileges(username) < 0)
            exit(1);
        }
#if 0
	if ((fd = open(pidfile, O_RDONLY, 0)) > 0)
	{
		ret = read(fd, pidstr, sizeof(pidstr) - 1);
		if (ret < 0)
		{
			flog(LOG_ERR, "cannot read radvd pid file, terminating: %s", strerror(errno));
			exit(1);
		}
		pidstr[ret] = '\0';
		if (!kill((pid_t)atol(pidstr), 0))
		{
			flog(LOG_ERR, "radvd already running, terminating.");
			exit(1);
		}
		close(fd);
		fd = open(pidfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	}
	else	/* FIXME: not atomic if pidfile is on an NFS mounted volume */
		fd = open(pidfile, O_CREAT|O_EXCL|O_WRONLY, 0644);

	if (fd < 0)
	{
		flog(LOG_ERR, "cannot create radvd pid file, terminating: %s", strerror(errno));
		exit(1);
	}
#endif	
	/*
	 * okay, config file is read in, socket and stuff is setup, so
	 * lets fork now...
	 */
#if 0 
	if (get_debuglevel() == 0) {

		/* Detach from controlling terminal */
		if (daemon(0, 0) < 0)
			perror("daemon");

		/* close old logfiles, including stderr */
		log_close();
		
		/* reopen logfiles, but don't log to stderr unless explicitly requested */
		if (log_method == L_STDERR_SYSLOG)
			log_method = L_SYSLOG;
		if (log_open(log_method, pname, logfile, facility) < 0)
			exit(1);

	}
#endif

    /*
    *config signal handlers, also make sure ALRM isn't blocked and raise a warning if so
    *      (some stupid scripts/pppd appears to do this...)
    */
    sigemptyset(&nset);
    sigaddset(&nset, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &nset, &oset);
    if (sigismember(&oset, SIGALRM))
        flog(LOG_WARNING, "SIGALRM has been unblocked. Your startup environment might be wrong.");

    signal(SIGHUP, sighup_handler);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);
    /* BEGIN: Added by z67728, 2009/11/3 */    
    signal(SIGUSR1, sigdebug_handler);
    signal(SIGUSR2, sigprefixchg_handler);
    /* END:   Added by z67728, 2009/11/3 */
    
#if 0
	snprintf(pidstr, sizeof(pidstr), "%ld\n", (long)getpid());
	
	write(fd, pidstr, strlen(pidstr));
	
	close(fd);
#endif

    if ( 0 == access("/var/radvd/readtest",F_OK) )
    {
        /* Ready test */
        g_iReadTestFlag = 1;
    }

    pid = getpid();
    if ((pidfp = fopen(pidfile, "w")) != NULL) {
        fprintf(pidfp, "%d", pid);
        fclose(pidfp);
    }

    config_interface();
    kickoff_adverts();

    /* enter loop */

    for (;;)
    {
        int len = 0, hoplimit = 0;
        struct sockaddr_in6 rcv_addr;
        struct in6_pktinfo *pkt_info = NULL;

        len = recv_rs_ra(sock, msg, &rcv_addr, &pkt_info, &hoplimit);
        if (len > 0) {
            process(sock, IfaceList, msg, len, 
            &rcv_addr, pkt_info, hoplimit);
        }      

        if (sigterm_received || sigint_received) {
            stop_adverts(g_lBridgeName);
            break;
        }

        if (sighup_received)
        {
            if ( 1 == g_iPrefixChgeFlag )
            {        
                disable_oldprefix(g_lBridgeName);
#ifdef SUPPORT_ATP_W724V_RA_CONFIG				
                g_lWanDownFlag = 1;
#endif				
            }            
            reload_config();

            sighup_received = 0;
            g_iPrefixChgeFlag = 0;
        }
    }

    //unlink(pidfile);
    exit(0);
}

void
timer_handler(void *data)
{
	struct Interface *iface = (struct Interface *) data;
	double next;

	dlog(LOG_DEBUG, 4, "timer_handler called for %s", iface->Name);

	send_ra_forall(sock, iface, NULL);
    if (&g_stAdvPrefix.Prefix == iface->AdvPrefixList)
    {
        g_stAdvPrefix.lSendTime++;
    }
	next = rand_between(iface->MinRtrAdvInterval, iface->MaxRtrAdvInterval); 

#ifdef SUPPORT_ATP_W724V_RA_CONFIG
    if ((iface->init_racount == MAX_INITIAL_RTR_ADVERTISEMENTS - 1 
         || MAX_INITIAL_RTR_ADVERTISEMENTS == g_stAdvPrefix.lSendTime)
        && NULL != iface->AdvPrefixList && (&g_stAdvPrefix.Prefix == iface->AdvPrefixList))
    {      
        iface->AdvPrefixList = iface->AdvPrefixList->next;               
        memset(&g_stAdvPrefix , 0, sizeof(struct AdvOldPrefix)); 
        iface->init_racount++;
    }
#endif
	if (iface->init_racount < MAX_INITIAL_RTR_ADVERTISEMENTS - 1)
	{
		iface->init_racount++;
                next = min(MAX_INITIAL_RTR_ADVERT_INTERVAL, next);
#ifdef SUPPORT_ATP_W724V_RA_CONFIG   
                if( 1 == g_lWanDownFlag )
                {
		    next = MAX_INITIAL_RTR_ADVERT_INTERVAL;
                }
#endif
	}

#ifdef SUPPORT_ATP_W724V_RA_CONFIG   
        g_lWanDownFlag = 0;
#endif

	set_timer(&iface->tm, next);
}

#ifdef SUPPORT_ATP_W724V_RA_CONFIG
void
timer_handler2(void *data)
{
	struct Interface *iface = (struct Interface *) data;

	dlog(LOG_DEBUG, 4, "timer_handler called for %s", iface->Name);

	send_na(sock, iface);
	set_timer(&iface->tm2, 8);
}
#endif

void
config_interface(void)
{
	struct Interface *iface;
	for(iface=IfaceList; iface; iface=iface->next)
	{
		if (iface->AdvLinkMTU)
        {
#ifdef SUPPORT_ATP_HYBRID
            if (!IS_BRIDGE_DEV(iface->Name)) /* DTS2014032001362 br mtu using 1500, not the pmtu by f00110348 */
#endif
            {
    			set_interface_linkmtu(iface->Name, iface->AdvLinkMTU);
            }
        } 
		if (iface->AdvCurHopLimit)
			set_interface_curhlim(iface->Name, iface->AdvCurHopLimit);
		if (iface->AdvReachableTime)
			set_interface_reachtime(iface->Name, iface->AdvReachableTime);
		if (iface->AdvRetransTimer)
			set_interface_retranstimer(iface->Name, iface->AdvRetransTimer);
	}
}

void
kickoff_adverts(void)
{
	struct Interface *iface;

	/*
	 *	send initial advertisement and set timers
	 */

	for(iface=IfaceList; iface; iface=iface->next)
	{
		if( iface->UnicastOnly )
			break;

		init_timer(&iface->tm, timer_handler, (void *) iface);
#ifdef SUPPORT_ATP_W724V_RA_CONFIG		
        init_timer(&iface->tm2, timer_handler2, (void *) iface);
        send_na(sock, iface);
        set_timer(&iface->tm2, 8);
#endif
		if (!iface->AdvSendAdvert)
			break;
                
#ifdef SUPPORT_ATP_W724V_RA_CONFIG
		//将之前释放的前缀利用全局变量记录，加入当前读配置下发的前缀链表中，保证在一个RA中下发所有内容
        if ( 1 == g_lWanDownFlag )
        {   
            if (((RELEASE_RA_VALID_LIFETIME == g_stAdvPrefix.Prefix.AdvValidLifetime) || (RELEASE_RA_VALID_LIFETIME_PREFIX_DELEGATION == g_stAdvPrefix.Prefix.AdvValidLifetime)) 
                && MAX_INITIAL_RTR_ADVERTISEMENTS > g_stAdvPrefix.lSendTime)
            {           
                g_stAdvPrefix.Prefix.next = iface->AdvPrefixList;
                iface->AdvPrefixList = &g_stAdvPrefix.Prefix;
                g_stAdvPrefix.lSendTime++;             
            }
        }
#endif

		/* send an initial advertisement */
		send_ra_forall(sock, iface, NULL);
#ifdef SUPPORT_ATP_W724V_RA_CONFIG		
        if (MAX_INITIAL_RTR_ADVERTISEMENTS == g_stAdvPrefix.lSendTime)
        {
            iface->AdvPrefixList = iface->AdvPrefixList->next;               
            memset(&g_stAdvPrefix, 0, sizeof(struct AdvOldPrefix)); 
        }
#endif
		iface->init_racount++;

		set_timer(&iface->tm,
			  min(MAX_INITIAL_RTR_ADVERT_INTERVAL,
			      iface->MaxRtrAdvInterval));
	}
}

void
stop_adverts(char *pcBridgeName)
{
    struct Interface *iface;

    /*
    *	send final RA (a SHOULD in RFC4861 section 6.2.5)
    */

    /* BEGIN: Added by z67728, 2009/12/28 PN:AU4D02322 Radvd模块关闭后,默认路由器将会失效。
    现将前缀地址的首选时间和在链标志去除,防止PC继续使用该地址通信. */    
    if ( 0 == g_iReadTestFlag )
    {        
        disable_oldprefix(pcBridgeName);
        disable_oldstataddr();
    }
    /* END:   Added by z67728, 2009/12/28 */

    for (iface=IfaceList; iface; iface=iface->next) {
        if( ! iface->UnicastOnly ) {
            if (iface->AdvSendAdvert) {
                /* send a final advertisement with zero Router Lifetime */
                iface->AdvDefaultLifetime = 0;
                send_ra_forall(sock, iface, NULL);
            }
        }
    }
}

/* BEGIN: add for DTS2013100901874 网关重新拨号，网关通告旧前缀中有效 生存时间为6天，不是标书要求的ValidLifetime = 14400 (4 hours) by l00214441 2013.10.18 */ 
int get_advvalidlifetime()
{
    if (0 == access(RADVD_PREFIX_DELEGATION_FILE, 0))
    {
        unlink(RADVD_PREFIX_DELEGATION_FILE);
        
        return RELEASE_RA_VALID_LIFETIME_PREFIX_DELEGATION;
    }

    return RELEASE_RA_VALID_LIFETIME;
}
/* END: add for DTS2013100901874 网关重新拨号，网关通告旧前缀中有效 生存时间为6天，不是标书要求的ValidLifetime = 14400 (4 hours) by l00214441 2013.10.18 */ 

/* BEGIN: Added by z67728, 2009/12/26 PN:AU4D02322 */
void
disable_oldprefix(char *pcBridgeName)
{
    if ( 0 == g_iReadTestFlag )
    {        
    	struct Interface *iface;
#ifdef SUPPORT_ATP_IPV6_ULA
	struct AdvPrefix *prefix;		
	char  czPrefixBuf[RADVD_PREFIX_BUF] = {0};	
	int lRet = 0;
#endif
    	/*
    	 *	RA配置发生变化时，将变化前的前缀设置为无效.
    	 */

    for (iface=IfaceList; iface; iface=iface->next) 
	{    	    
        if (NULL != iface->AdvPrefixList && (&g_stAdvPrefix.Prefix == iface->AdvPrefixList))
        {
            iface->AdvPrefixList = iface->AdvPrefixList->next;       
        }
        
        if( ! iface->UnicastOnly ) 	    
	    {    		
		if (iface->AdvSendAdvert) 		
		{    		   
		    /* send a final advertisement with zero Router Lifetime */    		   
		    if ( NULL != iface->AdvPrefixList )    
		    {       		    
#ifdef SUPPORT_ATP_IPV6_ULA
		        /* BEGIN:  added by zKF40309,2011/3/28 ,当前缀变化时*/			
			if ( 1 == g_iPrefixChgeFlag )			
			{			    			    
			    for( prefix = iface->AdvPrefixList; prefix; prefix = prefix->next )			    
			    {			    	
			    	memset(czPrefixBuf, 0, RADVD_PREFIX_BUF);			    	
				sprintf(czPrefixBuf, "%04x%04x%04x%04x", prefix->Prefix.s6_addr16[0],prefix->Prefix.s6_addr16[1],prefix->Prefix.s6_addr16[2],prefix->Prefix.s6_addr16[3]);				
				lRet = compare_prefix(czPrefixBuf, pcBridgeName);				
				if ( 0 == lRet )				
				{
				    prefix->AdvOnLinkFlag        = 0;				    
				    prefix->AdvValidLifetime     =  get_advvalidlifetime();                            
				    prefix->AdvPreferredLifetime = 0;	

#ifdef SUPPORT_ATP_W724V_RA_CONFIG                                                           
                    memset(&g_stAdvPrefix , 0, sizeof(struct AdvOldPrefix)); 
                    g_stAdvPrefix.lSendTime = 0;
                    memcpy(&g_stAdvPrefix.Prefix, prefix, sizeof(struct AdvPrefix));
#endif			
				}				
				else				
				{
				    prefix->enabled = 0;				
				}			    
			    }			
			}			
			/* END:  added by zKF40309,2011/3/28 */			
			/* 当stop_advert() 调用该函数时*/			
			else			
			{			    
			    prefix = iface->AdvPrefixList;    			    
#endif
			    while ( 1 )    			    
			    {    			
			        /* 只能修改时间，不能修改其他参数.否则客户端认为此RA非法.                         
			        The only way to cancela previous on-link indication is to                        
			        advertise that prefix with the L-bit set and the Lifetime                         
			        set to zero.  */
#ifdef SUPPORT_ATP_IPV6_ULA
				prefix->AdvOnLinkFlag        = 0;    			        
				prefix->AdvValidLifetime     = get_advvalidlifetime();                               
				prefix->AdvPreferredLifetime = 0;			                                    
				if ( NULL == prefix->next)                                
				{                               	   
				    break;                                
				}                               
				else                               
				{                                   
				    prefix = prefix->next;                                   
				    continue;			     
				}    			
#else
                            iface->AdvPrefixList->AdvOnLinkFlag        = 0;
    				        iface->AdvPrefixList->AdvValidLifetime     = 0;
                            iface->AdvPrefixList->AdvPreferredLifetime = 0;
                            
                            if ( NULL == iface->AdvPrefixList->next)
                            {
                                break;
                            }
                            else
                            {
                                iface->AdvPrefixList = iface->AdvPrefixList->next;
                                continue;
                            }

#endif
			    }		
#ifdef SUPPORT_ATP_IPV6_ULA
			}        		
#endif
			//减少冗余RA报文，不再单独下发释放的前缀，而是记录下来，与最新配置中的前缀一起下发
			//send_ra_forall(sock, iface, NULL);    		 
		    }    		
		}    	  
	    }
	}
    }
    
    return;
}

void disable_oldstataddr()
{
    if ( 0 == g_iReadTestFlag )
    {        
    	struct Interface *iface;
#ifdef SUPPORT_ATP_IPV6_ULA
	struct AdvPrefix *prefix;
#endif
    	/*
    	 *	RA配置发生变化时，将变化前的前缀设置为无效.
    	 */

    	for (iface=IfaceList; iface; iface=iface->next) {
    		if( ! iface->UnicastOnly ) {
    			if (iface->AdvSendAdvert) {
    				/* send a final advertisement with zero Router Lifetime */
    				if ( NULL != iface->AdvPrefixList )
    				{
#ifdef SUPPORT_ATP_IPV6_ULA
    				    prefix = iface->AdvPrefixList;
#endif
    				    while ( 1 )
    				    {
                            iface->AdvManagedFlag     = 0;
    				        iface->AdvOtherConfigFlag = 0;
#ifdef SUPPORT_ATP_IPV6_ULA
                            if ( NULL == prefix->next)
                            {
                                break;
                            }
                            else
                            {
                                prefix = prefix->next;
                                continue;
                            }
#else
                            if ( NULL == iface->AdvPrefixList->next)
                            {
                                break;
                            }
                            else
                            {
                                iface->AdvPrefixList = iface->AdvPrefixList->next;
                                continue;
                            }
#endif
    				    }
                        
        				send_ra_forall(sock, iface, NULL);
    				}
    			}
    		}
    	}
    }

    return;
}
/* END:   Added by z67728, 2009/12/26 */
void reload_config(void)
{
	struct Interface *iface;

       int lfdradvdlck = 0;
       char acFileForLock[64] = {0};
       
	flog(LOG_INFO, "attempting to reread config file");

	dlog(LOG_DEBUG, 4, "reopening log");
	if (log_reopen() < 0)
		exit(1);

	/* disable timers, free interface and prefix structures */
	for(iface=IfaceList; iface; iface=iface->next)
	{
		/* check that iface->tm was set in the first place */
		if (iface->tm.next && iface->tm.prev)
		{
			dlog(LOG_DEBUG, 4, "disabling timer for %s", iface->Name);
			clear_timer(&iface->tm);
		}
#ifdef SUPPORT_ATP_W724V_RA_CONFIG		
        if (iface->tm2.next && iface->tm2.prev)
		{
			dlog(LOG_DEBUG, 4, "disabling timer for %s", iface->Name);
            clear_timer(&iface->tm2);
		}
#endif		
	}

	iface=IfaceList; 
	while(iface)
	{
		struct Interface *next_iface = iface->next;
		struct AdvPrefix *prefix;
		struct AdvRoute *route;
		struct AdvRDNSS *rdnss;

		dlog(LOG_DEBUG, 4, "freeing interface %s", iface->Name);
		
		prefix = iface->AdvPrefixList;
		while (prefix)
		{
			struct AdvPrefix *next_prefix = prefix->next;

			//不free记录释放前缀的全局变量结构体
            if ( &g_stAdvPrefix.Prefix != prefix )
            {
			    free(prefix);   
            }	
			prefix = next_prefix;
		}
		
		route = iface->AdvRouteList;
		while (route)
		{
			struct AdvRoute *next_route = route->next;

			free(route);
			route = next_route;
		}
		
		rdnss = iface->AdvRDNSSList;
		while (rdnss) 
		{
			struct AdvRDNSS *next_rdnss = rdnss->next;
			
			free(rdnss);
			rdnss = next_rdnss;
		}	 

		free(iface);
		iface = next_iface;
	}

	IfaceList = NULL;

        /* reread config file */
        snprintf(acFileForLock, sizeof(acFileForLock), RADVD_CONFIGFILE_LOCK_FILE,g_lBridgeName);
        lfdradvdlck = LockFile(acFileForLock, NULL);   
        if (readin_config(conf_file) < 0)
        {
            printf("===========readin_config==error======================\n");
            UnlockFile(lfdradvdlck);        
            exit(1);
        } 
        UnlockFile(lfdradvdlck);  


	/* XXX: fails due to lack of permissions with non-root user */
	config_interface();
	kickoff_adverts();

	flog(LOG_INFO, "resuming normal operation");
}

void
sighup_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGHUP, sighup_handler);

	dlog(LOG_DEBUG, 4, "sighup_handler called");

	sighup_received = 1;
}

void
sigterm_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGTERM, sigterm_handler);

	dlog(LOG_DEBUG, 4, "sigterm_handler called");

	sigterm_received = 1;
}

void
sigint_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGINT, sigint_handler);

	dlog(LOG_DEBUG, 4, "sigint_handler called");

	sigint_received = 1;
}

/* BEGIN: Added by z67728, 2009/11/3 */
void
sigdebug_handler(int sig)
{
    static int iLogSwitch = 0;

	signal(SIGUSR1, sigdebug_handler);
    iLogSwitch = !iLogSwitch;
    if ( iLogSwitch )
    {
        set_debuglevel(LOG_DEBUG);        
        printf("\r\n Radvd open log ! \r\n");
    }
    else
    {
        set_debuglevel(LOG_EMERG);
        printf("\r\n Radvd close log ! \r\n");
    }

    
	dlog(LOG_DEBUG, 4, "\r\n Open log ! \r\n");
    return;
}

void
sigprefixchg_handler(int sig)
{
	signal(SIGUSR2, sigprefixchg_handler);
    //printf("\r\n Prefix config change ! \r\n");
    g_iPrefixChgeFlag = 1;
    return;
}

/* END:   Added by z67728, 2009/11/3 */
int
drop_root_privileges(const char *username)
{
	struct passwd *pw = NULL;
	pw = getpwnam(username);
	if (pw) {
		if (initgroups(username, pw->pw_gid) != 0 || setgid(pw->pw_gid) != 0 || setuid(pw->pw_uid) != 0) {
			flog(LOG_ERR, "Couldn't change to '%.32s' uid=%d gid=%d", 
					username, pw->pw_uid, pw->pw_gid);
			return (-1);
		}
	}
	else {
		flog(LOG_ERR, "Couldn't find user '%.32s'", username);
		return (-1);
	}
	return 0;
}

int
check_conffile_perm(const char *username, const char *conf_file)
{
	struct stat *st = NULL;
	struct passwd *pw = NULL;
	FILE *fp = fopen(conf_file, "r");

	if (fp == NULL) {
		flog(LOG_ERR, "can't open %s: %s", conf_file, strerror(errno));
		return (-1);
	}
	fclose(fp);

	st = malloc(sizeof(struct stat));
	if (st == NULL)
		goto errorout;

	if (!username)
		username = "root";
	
	pw = getpwnam(username);

	if (stat(conf_file, st) || pw == NULL)
		goto errorout;

	if (st->st_mode & S_IWOTH) {
                flog(LOG_ERR, "Insecure file permissions (writable by others): %s", conf_file);
		goto errorout;
        }

	/* for non-root: must not be writable by self/own group */
	if (strncmp(username, "root", 5) != 0 &&
	    ((st->st_mode & S_IWGRP && pw->pw_gid == st->st_gid) ||
	     (st->st_mode & S_IWUSR && pw->pw_uid == st->st_uid))) {
                flog(LOG_ERR, "Insecure file permissions (writable by self/group): %s", conf_file);
		goto errorout;
        }

	free(st);
        return 0;

errorout:
	if (st)
		free(st);
	return(-1);
}

int
check_ip6_forwarding(void)
{
	int forw_sysctl[] = { SYSCTL_IP6_FORWARDING };
	int value;
	size_t size = sizeof(value);
	FILE *fp = NULL;

#ifdef __linux__
	fp = fopen(PROC_SYS_IP6_FORWARDING, "r");
	if (fp) {
		fscanf(fp, "%d", &value);
		fclose(fp);
	}
	else
		flog(LOG_DEBUG, "Correct IPv6 forwarding procfs entry not found, "
	                       "perhaps the procfs is disabled, "
	                        "or the kernel interface has changed?");
#endif /* __linux__ */

	if (!fp && sysctl(forw_sysctl, sizeof(forw_sysctl)/sizeof(forw_sysctl[0]),
	    &value, &size, NULL, 0) < 0) {
		flog(LOG_DEBUG, "Correct IPv6 forwarding sysctl branch not found, "
			"perhaps the kernel interface has changed?");
		return(0);	/* this is of advisory value only */
	}
	
	if (value != 1) {
		flog(LOG_DEBUG, "IPv6 forwarding setting is: %u, should be 1", value);
		return(-1);
	}
		
	return(0);
}

int
readin_config(char *fname)
{
	if ((yyin = fopen(fname, "r")) == NULL)
	{
		flog(LOG_ERR, "can't open %s: %s", fname, strerror(errno));
		return (-1);
	}

	if (yyparse() != 0)
	{
		flog(LOG_ERR, "error parsing or activating the config file: %s", fname);
        fclose(yyin);
		return (-1);
	}
	
	fclose(yyin);
	return 0;
}

void
version(void)
{
	fprintf(stderr, "Version: %s\n\n", VERSION);
	fprintf(stderr, "Compiled in settings:\n");
	fprintf(stderr, "  default config file		\"%s\"\n", PATH_RADVD_CONF);
	fprintf(stderr, "  default pidfile		\"%s\"\n", PATH_RADVD_PID);
	fprintf(stderr, "  default logfile		\"%s\"\n", PATH_RADVD_LOG);
	fprintf(stderr, "  default syslog facililty	%d\n", LOG_FACILITY);
	fprintf(stderr, "Please send bug reports or suggestions to %s.\n",
		CONTACT_EMAIL);

	exit(1);	
}

void
usage(void)
{
	fprintf(stderr, "usage: %s %s\n", pname, usage_str);
	exit(1);	
}
#ifdef SUPPORT_ATP_IPV6_ULA
/* BEGIN:   Added by zKF40309, 2011/3/28 .前缀(64 bits)比较函数*/
int 
compare_prefix(char * czPrefixBefore,char *pcBridgeName) 
{ 	
    int  lRet = 0;	
    int  i = 0;
    int  j = 0; 
    char  czPrefixAfter[RADVD_FILE_BUF] = {0};	
    char  czBuf[RADVD_PREFIX_BUF] = {0};	
    FILE *pFile;
    char acFile[RADVD_PREFIX_BUF] = {0};
    if(NULL == czPrefixBefore)	
    {		
	return -1;	
    }	

    snprintf(acFile, sizeof(acFile), "/var/radvd/radvdlocal%s.conf", pcBridgeName);
    //pFile = fopen(RADVD_LOCAL_FILE, "r");
    pFile = fopen(acFile, "r");
    if( !pFile)	
    {		
        //flog(LOG_ERR, "can't open %s: %s", RADVD_LOCAL_FILE, strerror(errno));
       flog(LOG_ERR, "can't open %s: %s", acFile, strerror(errno));
	return -1;	
    }	
    lRet = fread(czPrefixAfter, 1, RADVD_FILE_BUF, pFile);
    if ( RADVD_FILE_BUF!= lRet)	
    {
	fclose(pFile);		
	return -1;	
    }	
    fclose(pFile);	
    /*  取出本地文件中的第一个前缀，GUA 前缀*/
    for ( i = 0,j = 0; i < RADVD_FILE_BUF;i++)	
    {		
        if ( ':' != czPrefixAfter[i] )		
	{		
	    czBuf[j] = czPrefixAfter[i];			
	    j += 1;		
	}		
	if ( 16 == j )			
	    break;	
    }
    if( !strncmp(czBuf,czPrefixBefore,16 ))		
	return -1;		
    memset(czBuf, 0, RADVD_PREFIX_BUF);	
    /*  取出本地文件中的第二个前缀，ULA 前缀*/	
    for ( i = 129,j = 0; i < RADVD_FILE_BUF;i++)	
    {		
        if ( ':' != czPrefixAfter[i] )		
	{			
	    czBuf[j] = czPrefixAfter[i];			
	    j += 1;		
	}		
	if ( 16 == j )			
	    break;	
    }
    if( !strncmp(czBuf,czPrefixBefore, 16 ))		
	return -1;	
    return 0;	 
}
/* END:   Added by zKF40309, 2011/3/28 */
#endif

