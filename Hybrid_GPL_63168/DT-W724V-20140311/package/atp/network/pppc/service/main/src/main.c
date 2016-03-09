/*
 * main.c - Point-to-Point Protocol main module
 *
 * Copyright (c) 1989 Carnegie Mellon University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Carnegie Mellon University.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <stdio.h>

//y42304 del 
//#include <ctype.h>


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <netdb.h>
#include <utmp.h>
#include <pwd.h>
#include <setjmp.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

//#include "msgapi.h"
//#include "msg/wancmsmsgtypes.h"
#include "pppd.h"
#include "magic.h"
#include "fsm.h"
#include "lcp.h"
#include "ipcp.h"
#ifdef INET6
#include "ipv6cp.h"
#endif
#include "upap.h"
#include "chap.h"
#include "ccp.h"
#include "pathnames.h"
#include "tdb.h"
//#include "atp_version.h"
#ifdef CBCP_SUPPORT
#include "cbcp.h"
#endif

#ifdef IPX_CHANGE
#include "ipxcp.h"
#endif /* IPX_CHANGE */
#ifdef AT_CHANGE
#include "atcp.h"
#endif
#ifdef SUPPORT_ATP_FON
#include "foncmsapi.h"
#endif
#include "pppcapi.h"

#define PPP_CURDEVNAME "/var/curpppdevname"
#define PRIVACY_BTN_PPPC_FLAG_FILE "var/pppc_privacy_btn"
extern unsigned int ATP_UTIL_GetSysTime(struct timeval* pstTimeVal);
extern unsigned int ATP_UTIL_VoiceGetSipInterval(void);

static const char rcsid[] = RCSID;

/* interface vars */
char ifname[32];		/* Interface name */
int ifunit;			/* Interface unit number */

struct channel *the_channel;

char *progname;			/* Name of this program */
char hostname[MAXNAMELEN];	/* Our hostname */
static char pidfilename[PATHLENGTH];	/* name of pid file */
static char linkpidfile[PATHLENGTH];	/* name of linkname pid file */
char ppp_devnam[PATHLENGTH];	/* name of PPP tty (maybe ttypx) */
uid_t uid;			/* Our real user-id */
struct notifier *pidchange = NULL;
struct notifier *phasechange = NULL;
struct notifier *exitnotify = NULL;
struct notifier *sigreceived = NULL;

int hungup;			/* terminal has been hung up */
int privileged;			/* we're running as real uid root */
int need_holdoff;		/* need holdoff period before restarting */
int detached;			/* have detached from terminal */
volatile int status;		/* exit status for pppd */
int unsuccess;			/* # unsuccessful connection attempts */
int do_callback;		/* != 0 if we should do callback next */
int doing_callback;		/* != 0 if we are doing callback */
TDB_CONTEXT *pppdb;		/* database for storing status etc. */
char db_key[32];

/* start of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */
int iIsExist = 0;
/* end of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */

int (*holdoff_hook) __P((void)) = NULL;
int (*new_phase_hook) __P((int)) = NULL;
void (*snoop_recv_hook) __P((unsigned char *p, int len)) = NULL;
void (*snoop_send_hook) __P((unsigned char *p, int len)) = NULL;

static int conn_running;	/* we have a [dis]connector running */
static int devfd;		/* fd of underlying device */
static int fd_ppp = -1;		/* fd for talking PPP */
static int fd_loop;		/* fd for getting demand-dial packets */
static int fd_msg = -1; /*fd for monitor stop message from cms, replace signals*/

int phase;			/* where the link is at */
int kill_link;
int open_ccp_flag;
int listen_time;
int got_sigusr2;
int got_sigterm;
int got_sighup;

//DT
//该变量用于在一次触发拨号后，如果goonflag=0，会一直自动拨号下去，需要通过goonflag = 1恢复按需拨号模式
int goonflag = 1;

#ifdef SUPPORT_ATP_PPPC6
int g_iIpv6Mode = 0;
int g_iIpv6MAddr = 0;
struct ip6_addr g_stStaticIP6Addr;
pid_t ipv6cp_dhcp_pid = 0;

/* start of by d00107688 2010-01-19 to add ipv4 switch */
int g_iIpv4Mode = 1;
/* end of by d00107688 2010-01-19 to add ipv4 switch */
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
int g_iEnableV6Dial = 0;
int g_iEnableV4Dial = 0;
#endif
#endif

/*记录是否有收到IPv6CP的报文*/
bool g_iRecvIpv6cp = 0;


/*add by z67625 2007-08-25 start*/
int lasterror = 0;  //每次连接如果设置了一次lasterror就不再设置
extern struct session *ses;
/*add by z67625 2007-08-25 end*/

static int waiting;
static sigjmp_buf sigjmp;

char **script_env;		/* Env. variable values for scripts */
int s_env_nalloc;		/* # words avail at script_env */

u_char outpacket_buf[PPP_MRU+PPP_HDRLEN]; /* buffer for outgoing packet */
u_char inpacket_buf[PPP_MRU+PPP_HDRLEN]; /* buffer for incoming packet */

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
unsigned int g_ulIspFlag = 0;
unsigned int g_ulDefualtUPFlag = 0;
unsigned int auth_error = 0;
unsigned int auth_error_count = 0;
#endif

static int n_children;		/* # child processes still running */
static int got_sigchld;		/* set if we have received a SIGCHLD */

int privopen;			/* don't lock, open device as root */
int atp_time_flag = 0;

char *no_ppp_msg = "Sorry - this system lacks PPP kernel support\n";

GIDSET_TYPE groups[NGROUPS_MAX];/* groups the user is in */
int ngroups;			/* How many groups valid in groups */

static struct timeval start_time;	/* Time when link was started. */
static struct timeval ppp_time; //PPPOE by c47036 20070112

struct pppd_stats link_stats;
int link_connect_time;
int link_stats_valid;

int trigger_demand = 0;

/*是否在收到TERM信号后执行函数，若已经在执行掉线了(lcp down和kill_link)，收到了直接返回，不做动作*/
static int bEnableTerm = 1;
/*
 * We maintain a list of child process pids and
 * functions to call when they exit.
 */
struct subprocess {
    pid_t	pid;
    char	*prog;
    void	(*done) __P((void *));
    void	*arg;
    struct subprocess *next;
};

/*Start of ATP 2014-4-24 for DTS2014031501735 by y00250667*/
extern char acDns[DNS_BIND_LEN];
extern char ouraddr[32];
extern char wanif_name[PATHLENGTH];
#define PPP_SERVICE_TYPE "VOICE_TR069_INTERNET"
/*End of ATP 2014-4-24 for DTS2014031501735 by y00250667*/

static struct subprocess *children;

/* Prototypes for procedures local to this file. */

static void setup_signals __P((void));
#if 0
static void create_pidfile __P((void));
static void create_linkpidfile __P((void));
#endif
static void cleanup __P((void));
static void get_input __P((void));
static void calltimeout __P((void));
static struct timeval *timeleft __P((struct timeval *));
static void kill_my_pg __P((int));
static void hup __P((int));
static void term __P((int));
static void chld __P((int));
static void toggle_debug __P((int));
static void triggerdemand __P((int));
static int create_monitor_socket(const char *ifname);
extern fd_set in_fds;
extern int max_in_fd;
static int proc_msg_fd(int newfd);


/*add by z67625 start*/
static void switchdebug __P((int));
/*add by z67625 end*/
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
static void check_usr __P((int));
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/

static void processv6sig __P((int));

static void open_ccp __P((int));
static void bad_signal __P((int));
static void holdoff_end __P((void *));
static int reap_kids __P((int waitfor));
static void update_db_entry __P((void));
#if 0
static void add_db_key __P((const char *));
static void delete_db_key __P((const char *));
#endif
static void cleanup_db __P((void));
static void handle_events __P((void));

extern	char	*ttyname __P((int));
extern	char	*getlogin __P((void));

#ifdef SUPPORT_ATP_PPPD
static int pppx_nego_shmid(void);
static void pppx_cleanAcnt(char *name);
#endif

static void* pppoe_setstatus_ipcp_up_timeout(void *s);

/*add by z67625 2007-08-24 start*/
int pppoe_getstatus(void);
/*add by z67625 2007-08-24 end*/
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
extern void pppx_check_user(void);
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/

#ifdef ultrix
#undef	O_NONBLOCK
#define	O_NONBLOCK	O_NDELAY
#endif

#ifdef ULTRIX
#define setlogmask(x)
#endif

/*
 * PPP Data Link Layer "protocol" table.
 * One entry per supported protocol.
 * The last entry must be NULL.
 */
struct protent *protocols[] = {
    &lcp_protent,
    &pap_protent,
    &chap_protent,
#ifdef CBCP_SUPPORT
    &cbcp_protent,
#endif
    &ipcp_protent,
#ifdef INET6
    &ipv6cp_protent,
#endif
#ifdef IPX_CHANGE
    &ipxcp_protent,
#endif
#ifdef AT_CHANGE
    &atcp_protent,
#endif
    NULL
};

static int save_pid(void)
{
    char path[128]="";
    FILE *pstPidFile = NULL;
    int lPid = 0;

    snprintf(path, sizeof(path), "%s%s/%s", WAN_PATH, wanif_name, "pid");
	pstPidFile = fopen(path, "w");
	if (NULL != pstPidFile) 
	{
	    lPid = getpid();
		fprintf(pstPidFile, "%d\n", lPid);
		fclose(pstPidFile);
	}

    return 1;
}

/*用socket收消息替代以前sigusr1的信号处理*/
static int create_monitor_socket(const char *ifname)
{
	struct sockaddr_un stLocal;
	int iLen = 0;
	int iSock = -1;
	int iSockFlags;

	if (!ifname)
	{
	    return -1;
	}
	if ((iSock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		error("could not allocate unix socket");
		return -1;
	}
	stLocal.sun_family = AF_UNIX;
	snprintf(stLocal.sun_path, sizeof(stLocal.sun_path), PPPC_EVENT_PATH, ifname);
	unlink(stLocal.sun_path);
	iLen = strlen(stLocal.sun_path) + sizeof(stLocal.sun_family);
	if (bind(iSock, (struct sockaddr *)&stLocal, iLen) == -1) {
		error("could not bind unix socket");
		close(iSock);
		return -1;
	}
	if (listen(iSock, 5) == -1) {
		error("could not listen to unix socket");
		close(iSock);
		return -1;
	}
	iSockFlags = fcntl(iSock, F_GETFL, 0);
    fcntl(iSock, F_SETFL, O_NONBLOCK | iSockFlags);
	return iSock;
}

static void check_msgfd(fd_set *pstFds)
{
    int newfd = -1;
    int iSockFlags;

    if (fd_msg < 0)
    {
        return;
    }
    if (FD_ISSET(fd_msg, pstFds))
    {
        newfd = accept(fd_msg, NULL, NULL);
        if (newfd > 0)
        {
            iSockFlags = fcntl(newfd, F_GETFL, 0);
            fcntl(newfd, F_SETFL, O_NONBLOCK | iSockFlags);
            proc_msg_fd(newfd);
        }
    }
}
/*
 * If PPP_DRV_NAME is not defined, use the default "ppp" as the device name.
 */
#if !defined(PPP_DRV_NAME)
#define PPP_DRV_NAME	"ppp"
#endif /* !defined(PPP_DRV_NAME) */

int main(int argc, char *argv[])
{
    int i, t;
    char *p;
    struct passwd *pw;
    struct protent *protp;
    char numbuf[16];
    int lastppptime = 0; //PPPOE by c47036 20070112
    int Nowppptime = 0; 
    int IPv6Enable = 1; //PPPOE by l00115777 20110419
#ifdef SUPPORT_TD_CARD
    int hspastatus = 0;
#else
    int curstatus = 0;
#endif
	unsigned int HanddialRetry = 0;
    unsigned int msleeptime = 0;
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
    char	devname_temp[MAXPATHLEN] = {0};	
#endif
    int temp = 0;
    fd_set checkFds;
    struct timeval stWait;
    int iRet = 0;
    
   /*start: 用于查询组件版本号(pppc atpv),请不要修改或删除 h00189019@20120622*/
    if ((argc == 2) && (NULL != argv[1]) && (0 == strcmp(argv[1],ATP_VERSION_CMD_KEY)))
    {
       printf("\r\n%s.\n", MODULE_VERSION);    	    		
       exit(0);
    }
    /*end */	

/*added by w00190448@20120420for DTS2012021601805:手动拨号时需要2次点击拨号按钮start*/
#if 0    
    /*B300需求,手动拨号拨3次*/
    if(g_ulIsDial3times)
    {
        HanddialRetry = 0;
    }
    else
    {
        HanddialRetry = 2;
    }
#else
    HanddialRetry = 0;
#endif
/*added by w00190448@20120420for DTS2012021601805:手动拨号时需要2次点击拨号按钮end*/

    ATP_MSG_Init(ATP_CBBID_PPPC_NAME);
    
    new_phase(PHASE_INITIALIZE);    
    
    /*
     * Ensure that fds 0, 1, 2 are open, to /dev/null if nowhere else.
     * This way we can close 0, 1, 2 in detach() without clobbering
     * a fd that we are using.
     */
    /*if ((i = open("/dev/null", O_RDWR)) >= 0) {
	while (0 <= i && i <= 2)
	    i = dup(i);
	if (i >= 0)
	    close(i);
    }*/

    script_env = NULL;

    /* Initialize syslog facilities */
    reopen_log();

    if (gethostname(hostname, MAXNAMELEN) < 0 ) {
	option_error("Couldn't get hostname: %m");
	exit(1);
    }
    hostname[MAXNAMELEN-1] = 0;

    /* make sure we don't create world or group writable files. */
    umask(umask(0777) | 022);

    uid = getuid();
    privileged = uid == 0;
    slprintf(numbuf, sizeof(numbuf), "%d", uid);
    script_setenv("ORIG_UID", numbuf, 0);

    ngroups = getgroups(NGROUPS_MAX, groups);

    /*
     * Initialize magic number generator now so that protocols may
     * use magic numbers in initialization.
     */
    magic_init();

    /*
     * Parse, in order, the system options file, the user's options file,
     * and the command line arguments.
     */
    processconfig(argv[2]);
	setup_signals();/*将信号注册函数放在靠前的位置，避免信号未注册时，就有模块调用触发按需拨号的接口时，导致ppp进程退出*/
    save_pid();
    /*添加延时，保证在页面修改账户等参数时，先让cms处理完配置参数等操作，
	避免cms和pppc同时忙，影响消息传递的时间*/
    if (0 == access(RESTART_PPPC_FLAG_FILE, F_OK))
    {
        sleep(2);
        unlink(RESTART_PPPC_FLAG_FILE);
    }

#ifdef SUPPORT_ATP_L2TP_KRNL_MODE
_DEBUG(LOG_PPPC, "argv3 is %s", argv[3]);

    if(5 == dial_protocol)
    {
        setdevname_pppol2tp(argv[3]);
    }
#endif 

     /*暂时使用下面的解析函数，完成cms后，采用上面这个*/
    //ppp_processconfig(argv);
    #ifdef SUPPORT_ATP_PPPC6
    /*获取IPV6总开关的状态,如果总开关关闭，则IPV6模式无效*/
    IPv6Enable = getIPv6Status();
    if(0 == IPv6Enable)
    {
        g_iIpv6Mode = 0;
    }
    #endif
#ifdef SUPPORT_TD_CARD
     sleep(10);
#endif
      if(idle_time_limit>0)
        demand=1;
    /*
     * Initialize each protocol.
     */
/* start of by d00107688 2009-10-19 PPPC 支持IPv6CP */     
#ifdef SUPPORT_ATP_PPPC6
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
    {
        if (PPP_IPCP == protp->protocol)
        {
            protp->enabled_flag = g_iIpv4Mode;
            if (protp->enabled_flag)
            {
                (*protp->init)(0);
            }
        }
        else if (PPP_IPV6CP == protp->protocol)
        {
            protp->enabled_flag = g_iIpv6Mode;
            if (protp->enabled_flag)
            {
                (*protp->init)(0);
            }
        }
        else
        {
            (*protp->init)(0);
        }
    }
#else
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
        (*protp->init)(0);
#endif
/* end of by d00107688 2009-10-19 PPPC 支持IPv6CP */     

    /*
     * Initialize the default channel.
     */

    progname = *argv;

    /*
     * Parse, in order, the system options file, the user's options file,
     * and the command line arguments.
     */
     
    devnam_fixed = 1;		/* can no longer change device name */
    
#ifdef SUPPORT_ATP_PPPD
    if((1 == pppx) && (-1 == shm_id))
        shm_id = pppx_nego_shmid();
#endif

    /*
     * Work out the device name, if it hasn't already been specified,
     * and parse the tty's options file.
     */
    
    if (the_channel->process_extra_options)
	(*the_channel->process_extra_options)();

    if (debug)
	setlogmask(LOG_UPTO(LOG_DEBUG));

    /*
     * Check that we are running as root.
     */
    if (geteuid() != 0) {
	option_error("must be root to run %s, since it is not setuid-root",
		     argv[0]);
	exit(EXIT_NOT_ROOT);
    }

    if (!ppp_available()) {
	option_error("%s", no_ppp_msg);
	exit(EXIT_NO_KERNEL_SUPPORT);
    }

_DEBUG(LOG_PPPC, "AAAA ");
    /*
     * Initialize system-dependent stuff.
     */
    sys_init();
    

    pppdb = tdb_open(_PATH_PPPDB, 0, 0, O_RDWR|O_CREAT, 0644);
    if (pppdb != NULL) {
	slprintf(db_key, sizeof(db_key), "pppd%d", getpid());
	update_db_entry();
    } else {
	warn("Warning: couldn't open ppp database %s", _PATH_PPPDB);
	if (multilink) {
	    warn("Warning: disabling multilink");
	    multilink = 0;
	}
    }

    /*
     * Detach ourselves from the terminal, if required,
     * and identify who is running us.
     */
    if (!nodetach && !updetach)
    {
	detach();
	}
	
    p = getlogin();
    if (p == NULL) {
	pw = getpwuid(uid);
	if (pw != NULL && pw->pw_name != NULL)
	    p = pw->pw_name;
	else
	    p = "(unknown)";
    }
    syslog(LOG_NOTICE, "pppd %s started by %s, uid %d", VERSION, p, uid);
    script_setenv("PPPLOGNAME", p, 0);

    if (devnam[0])
	script_setenv("DEVICE", devnam, 1);
    slprintf(numbuf, sizeof(numbuf), "%d", getpid());
    script_setenv("PPPD_PID", numbuf, 1);


    waiting = 0;

//    create_linkpidfile();

    /*
     * If we're doing dial-on-demand, set up the interface now.
     */
    if (demand) {
    
	/*
	 * Open the loopback channel and set it up to be the ppp interface.
	 */
	tdb_writelock(pppdb);
	fd_loop = open_ppp_loopback();
	set_ifunit(1);
	tdb_writeunlock(pppdb);
	/*
	 * Configure the interface and mark it up, etc.
	 */
    }
    /*create monitor fd for recv stop message, replace signals*/
	fd_msg = create_monitor_socket(wanif_name);
	
    do_callback = 0;
    for (;;) {
    FD_ZERO(&in_fds);
    max_in_fd = 0;
    if (fd_msg > 0)
	{
	    add_fd(fd_msg);
	}
	listen_time = 0;
	need_holdoff = 1;
	devfd = -1;
	status = EXIT_OK;
	++unsuccess;
	doing_callback = do_callback;
	do_callback = 0;
    lasterror = 0;//每次重新连接，将lasterror清0,允许设置最后一次掉线原因
	
#ifdef SUPPORT_ATP_PPPD
    if(pppx)
        pppx_setwan_state(log_name, PPPX_WAN_DOWN);
#endif

#if defined(SUPPORT_ATP_PPP_DYN_DIAL) && defined(SUPPORT_ATP_PPP_DT_TOCPE0056)
 //for W724, always-on时，初始都允许拨号
    if (0 == idle_time_limit)
    {
        g_iEnableV4Dial = 1;
        g_iEnableV6Dial = 1;
    }
#endif

    curstatus = getdialstatus();

    /* BEGIN: Added by y67514, 2008/12/29   PN:ET128*/
#ifdef SUPPORT_TD_CARD
    hspastatus = gethspastatus();
    if ( 3 == dial_protocol )
    {
        /*hspa模式*/
        /*wan状态为手动或者自动才能拨号*/
        while(HspaOk != hspastatus || (Autocon != curstatus && Handdial != curstatus && Ondemand != curstatus)  )
        {
            check_msgfd(&in_fds);
            _DEBUG(LOG_PPPC, "curstatus=%d", curstatus);
            //页面查询时间为5秒，这里改为3秒。
            sleep(3); 
            curstatus = getdialstatus();
            hspastatus = gethspastatus();
        } 
    }
    else
#endif

    {
_DEBUG(LOG_PPPC, "curstatus=%d", curstatus);
        /*wan状态为手动或者自动才能拨号*/
        while(Autocon != curstatus && Handdial != curstatus && Ondemand != curstatus)
        {
            //页面查询时间为5秒，这里改为1秒。
            //sleep(1);
            if (fd_msg > 0)
            {
                stWait.tv_sec = 1;
                stWait.tv_usec = 0;
                FD_ZERO(&checkFds);
                FD_SET(fd_msg, &checkFds);
                iRet = select(fd_msg+1, &checkFds, NULL, NULL, &stWait);
                if ((iRet < 0) && ((EINTR == errno) || (EAGAIN == errno)))
                {
                    continue;
                }
                else if (iRet > 0)
                {
                    check_msgfd(&checkFds);
                }
            }
            
            curstatus = getdialstatus();
            _DEBUG(LOG_PPPC, "curstatus=%d", curstatus);
        } 
    }
    /* END:   Added by y67514, 2008/12/29 */


#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    //if(Autocon != curstatus && Handdial != curstatus && Ondemand != curstatus)
    //{
    //    internet_status_notify(NO_DSL_AVAIL);
    //}
    while(AUTH_NODIAL == auth_error)
    {
        check_msgfd(&in_fds);
        if(debug)
        {
            printf("\n ==========auth error cause to AUTH_NODIAL,waitting reset\n");
        }
        sleep(10);
    }

/***********************************************************
    规格:
    PPP认证返回错误码为'0001'(=XXXX-XX-0001)时，不再尝试建立连接，直到：
1) 重新上电
2) 网关重起
3) 更改了拨号帐号
PPP认证错误码不是'0001'(≠XXXX-XX-0001)时，网关循环5次尝试PPPoE连接。
如果连接未能成功，则等待60秒再发起尝试循环。
*****************************************************************/
    if(AUTH_TRY == auth_error)
    {
        if(AUTH_FAILED_TRY == auth_error_count)
        {
            int i = 20;
            int status = 0;
            while(i--)
            {
                check_msgfd(&in_fds);
                sleep(3);
                notice("##sleep 3s, index=%d,wait total 60s !\n", i);
                status = getdialstatus();
                if((Wait == status)||(Discon == status))
                {
                    break;
                }
            }
        }

        auth_error_count = auth_error_count%AUTH_FAILED_TRY;

    }
    else
    {
        auth_error_count = 0;
    }
#endif

/*added by w00190448@20120420for DTS2012021601805:手动拨号时需要2次点击拨号按钮start*/
#if 0    
/*B300需求,手动拨号拨3次*/
    if((Handdial == curstatus)&&(++HanddialRetry)>=HANDDIALRETRY)
    {
#ifndef SUPPORT_TD_CARD
        setdialstatus(Wait);
#endif
        // 手动拨号失败后不等待
        need_holdoff = 0;
    }
#endif
/*added by w00190448@20120420for DTS2012021601805:手动拨号时需要2次点击拨号按钮end*/


    /*B300需求,拨号之前等待随机时间*/
    if(g_ulRandomDialInterval)
    {
        srand((unsigned int)time(0));
        msleeptime = (unsigned int)(rand()%(g_ulRandomDialInterval *1000));
        printf("\nwait %u sec before dial\n",(unsigned int)(msleeptime/1000));
        __msleep(msleeptime);
    }
    /*B300需求,拨号之前等待随机时间*/

#ifdef SUPPORT_ATP_PPPD
    if(pppx)
    {
        pppx_setwan_state(log_name, PPPX_WAN_SLEEP);
        
        while(PPPX_TRUE != pppx_wakeup())
        {
            sleep(1);
            check_msgfd(&in_fds);
        }
        pppx_setwan_state(log_name, PPPX_WAN_AUTHING);
    }
#endif

    if(demand)
    {
        /*DTS2012101608378 DT要求PADI发送间隔为3秒*/
        //sleep(1);
        set_ifunit(1);
        demand_conf();
    }
    /*always on模式也需要关闸close gate by x00202174 start*/
    /*按需拨号在启用伪路由前需关闸，在拨号成功后nat设置完成后在开闸 close gate by g00121640 start*/
    if(0 == dial_protocol)
    switchgate(1);
    /*按需拨号在启用伪路由前需关闸，在拨号成功后nat设置完成后在开闸 close gate by g00121640 end*/
    /*always on模式也需要关闸close gate by x00202174 end*/
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    DT_ResetRetryCounter(PPP_IP);
    DT_ResetRetryCounter(PPP_IPV6);
#endif
    trigger_demand = 0;//解决自验发现的历史bug.(按需拨号掉线前如果收到本地触发信号FPE，下一次等待拨号时任何信号都可以触发拨号)

#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V	
	if (demand && !doing_callback &&goonflag){
#else
	if (demand && !doing_callback ) {
#endif	 
	    /*
	     * Don't do anything until we see some activity.
	     */
	    new_phase(PHASE_DORMANT);
	    demand_unblock();
	    add_fd(fd_loop);
	    for (;;) {
        //修改配置，重置进程时，起始按需拨号要触发
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
        if (0 == access(TRIGGER_PPPC_FLAG_FILE, F_OK))
        {
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
            g_iEnableV4Dial = 1;
            g_iEnableV6Dial = 1;
#endif
            //WEB配置后触发一次
            unlink(TRIGGER_PPPC_FLAG_FILE);
            break;
        }
#endif
#ifdef SUPPORT_ATP_PPP_TRIGGERDEMAND        
		if(1 == trigger_demand)
        {
            trigger_demand = 0;
            break;
        }
#endif
#ifdef SUPPORT_ATP_HYBRID_BONDING
        //若开了lte bonding,则跳出进行拨号
        if (GetLteStatus() > 0)
        {
            break;
        }
#endif
		handle_events();
		check_msgfd(&in_fds);

        //此处用于接收FPE触发信号时的处理
        if(1 == trigger_demand)
        {
            trigger_demand = 0;
            break;
        }
	   
		if (kill_link && !persist)
		    break;
		if (get_loop_output())
		    break;
	    }
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V	
		goonflag = 0;
#endif
	    remove_fd(fd_loop);
	    if (kill_link && !persist)
		break;

	    /*
	     * Now we want to bring up the link.
	     */
#ifndef SUPPORT_ATP_PPP_DYN_DIAL
	    demand_block();
#endif
	    info("Starting link");
	}


    pppoe_setstatus(Connecting);//设置状态为正在连接
#ifdef SUPPORT_ATP_PPPC6
    pppoe_setstatus6(Connecting);
#endif
#ifdef SUPPORT_ATP_FON
    /*清除上一次拨号的v6标志文件,有这个就不用尝试ipv6心跳*/
    unlink(PPP_NO_IPV6_FOR_FON);
#endif

	printf("pppc -I %s connecting..., nas_name=%s\r\n", wanif_name, devnam);
	check_msgfd(&in_fds);

	/* Start of PPPOE by c47036 20070112 */
	//gettimeofday(&ppp_time, NULL);
	ATP_UTIL_GetSysTime(&ppp_time);   
    lastppptime = ppp_time.tv_sec * 1000 + ppp_time.tv_usec /1000;
	/* End of PPPOE by c47036 20070112 */
    
	new_phase(PHASE_SERIALCONN);

#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V

    if(PADI_fail_flag)
    {
        //counter用于在翻转时保持turn_around_count不变
        counter++;
        //接口翻转
        strcpy(devname_temp,devnam);
        strcpy(devnam,devnam_bak);
        strcpy(devnam_bak,devname_temp);
		
        if(counter%2 == 0)
        {
            turn_around_count = turn_around_count*2;
        }

        
        if(turn_around_count>4)
        {
            turn_around_count = 1;
            counter = 0;
            int i = 20;
            int status = 0;

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
        	if(0 == dial_protocol)
            {
                ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R020, "pppoe negotiation time out");
            }
#endif
            while(--i) //解决HOMEGW-8638
            {
                check_msgfd(&in_fds);
                sleep(3);
//printf("\nthe results  at %s,%s,%d,still got %d seconds ..\n",__FILE__,__FUNCTION__,__LINE__,i*3);

                status = getdialstatus();
                if((Wait == status)||(Discon == status))
                {
                    break;
                }
            }
            
			//HOMEGW-15061重发新的一轮padi时回复初始状态，避免对页面判断产生影响    
            pppoe_setlasterror("ERROR_NONE");
        }

        PADI_fail_flag = 0;       
    }

#endif
	devfd = the_channel->connect();
    //printf("\nthe results appear at %s,%s,%d,devfd is :%d\n",__FILE__,__FUNCTION__,__LINE__,devfd);
	if ((devfd < 0) 
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
	|| (PADI_FAIL_RET == devfd)
#endif	
	)
    {
        ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R011);
	    goto fail;
    }
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
//此次表明发现阶段完成，上行vlan环境也已经确定，故使用goonflag = 1恢复按需拨号模式
    if (demand)
    {
        goonflag = 1;
    }
#endif	

#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V	
//如果不是因为PADI失败需要将计数初始化
    turn_around_count = 1;
    counter = 0;
#endif	
	/* set up the serial device as a ppp interface */
	tdb_writelock(pppdb);
	fd_ppp = the_channel->establish_ppp(devfd);
     //printf("\nthe results appear at %s,%s,%d,fd_ppp is :%d\n",__FILE__,__FUNCTION__,__LINE__,fd_ppp);
	if (fd_ppp < 0) {
	    tdb_writeunlock(pppdb);
	    status = EXIT_FATAL_ERROR;
        ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R011);
	    goto disconnect;
	}

	if (!demand && ifunit >= 0)
	    set_ifunit(1);
	tdb_writeunlock(pppdb);

	/*
	 * Start opening the connection and wait for
	 * incoming events (reply, timeout, etc.).
	 */
	notice("Connect: %s <--> %s", ifname, ppp_devnam);
	//gettimeofday(&start_time, NULL);
	ATP_UTIL_GetSysTime(&start_time);
	link_stats_valid = 0;
	script_unsetenv("CONNECT_TIME");
	script_unsetenv("BYTES_SENT");
	script_unsetenv("BYTES_RCVD");
	lcp_lowerup(0);

	add_fd(fd_ppp);
	lcp_open(0);		/* Start protocol */
	status = EXIT_NEGOTIATION_FAILED;
	new_phase(PHASE_ESTABLISH);

	while (phase != PHASE_DEAD) {
	    handle_events();
	    get_input();
	    remove_fd(fd_msg);
	    add_fd(fd_msg);
	    if (kill_link)
        { 
            /*add by z67625 start*/
            pppoe_setlasterror("ERROR_FORCED_DISCONNECT");
            /*add by z67625 end*/
    		lcp_close(0, "User request");
        }
	}

#ifdef SUPPORT_ATP_PPPD
    if(pppx && (PPPX_WAN_AUTHING == pppx_getwan_state(log_name)))
        pppx_setwan_state(log_name, PPPX_WAN_AUTH_TIMEOUT);
#endif

	/*
	 * Print connect time and statistics.
	 */
	if (link_stats_valid) {
	    int t = (link_connect_time + 5) / 6;    /* 1/10ths of minutes */
	    info("Connect time %d.%d minutes.", t/10, t%10);
	    info("Sent %u bytes, received %u bytes.",
		 link_stats.bytes_out, link_stats.bytes_in);
	}
	/*
	 * Delete pid file before disestablishing ppp.  Otherwise it
	 * can happen that another pppd gets the same unit and then
	 * we delete its pid file.
	 */
	if (!demand) {
	    if (pidfilename[0] != 0
		&& unlink(pidfilename) < 0 && errno != ENOENT)
		warn("unable to delete pid file %s: %m", pidfilename);
	    pidfilename[0] = 0;
	}

	/*
	 * If we may want to bring the link up again, transfer
	 * the ppp unit back to the loopback.  Set the
	 * real serial device back to its normal mode of operation.
	 */
	remove_fd(fd_ppp);
	clean_check();
	the_channel->disestablish_ppp(devfd);
	fd_ppp = -1;
	if (!hungup)
	    lcp_lowerdown(0);
	if (!demand)
	    script_unsetenv("IFNAME");

	/*
	 * Run disconnector script, if requested.
	 * XXX we may not be able to do this if the line has hung up!
	 */
    disconnect:
    pppoe_setstatus(Disconnected);
#ifdef SUPPORT_ATP_PPPC6
    pppoe_setstatus6(Disconnected);
#endif
	new_phase(PHASE_DISCONNECT);
	the_channel->disconnect();

    fail:
    check_msgfd(&in_fds);
	if (the_channel->cleanup)
	    (*the_channel->cleanup)();

	if (!demand) {
	    if (pidfilename[0] != 0
		&& unlink(pidfilename) < 0 && errno != ENOENT)
		warn("unable to delete pid file %s: %m", pidfilename);
	    pidfilename[0] = 0;
	}

	if (!persist || (maxfail > 0 && unsuccess >= maxfail));

	if (demand)
	{
	    demand_discard();
        /* 按需拨号掉线后不等待 */
        need_holdoff = 0;
	}

    /* Start of PPPOE by c47036 20070112 */
    /* 和记需求：PPPOE拨号时间间隔60秒,其它情况保持30秒 */
	//gettimeofday(&ppp_time, NULL);
	ATP_UTIL_GetSysTime(&ppp_time);   
    //改为用毫秒计算间隔，防止49.9~50.1这种造成1s误差
    Nowppptime = ppp_time.tv_sec * 1000 + ppp_time.tv_usec /1000;
	if (((Nowppptime - lastppptime) >= 0)
	    && ((Nowppptime - lastppptime) < holdoff*1000))
	{
	    //四舍五入，超时时间始终为整数
	    temp = Nowppptime - lastppptime;
        if( 500 < temp && temp < 1000 )
        {
            temp = 1000;
        }
        else if(500 > temp)
        {
            temp = 0;            
        }
            
	    t = need_holdoff? ((holdoff*1000 - temp)/1000): 0;
	}
	else
	{
	    t = 0;
	}
	/* End of PPPOE by c47036 20070112 */

    if(debug)
    printf("\nneed_holdoff=%u t=%d\n",need_holdoff, t);

	if (holdoff_hook)
	    t = (*holdoff_hook)();

#ifdef SUPPORT_TD_CARD
       if ( 3 == dial_protocol)
       {
            /*HSPA模式时延迟30秒重拨*/
            t = 10;
            AT_DEBUG("wait for 10 seconds to redial...\n");
       }
#endif

/*B300需求,拨号失败后再次发起PPPoE会话的时间间隔*/
#if 0
    if(g_ulRetryDialInterval > 0)
    {
        t = g_ulRetryDialInterval;
        printf("\n wait %u sec before next connect\n",t);
    }
#endif
/*B300需求,拨号失败后再次发起PPPoE会话的时间间隔*/
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
//如果是PADI探测失败，不需要延迟
    if(PADI_fail_flag ==1)
    {
        t= 0;

    }
#endif	
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    if (AUTH_NODIAL == auth_error || AUTH_TRY == auth_error)
    {
        t = 0;
    }
#endif
	if (t > 0) {
	    new_phase(PHASE_HOLDOFF);
	    TIMEOUT(holdoff_end, NULL, t);
	    do {
		handle_events();
		check_msgfd(&in_fds);
		if (kill_link)
		    new_phase(PHASE_DORMANT); /* allow signal to end holdoff */
	    } while (phase == PHASE_HOLDOFF);
	    /*if (!persist)
		    sleep(30);*//*break;WAN <3.1.1PPP认证失败退出> porting by s60000658 20060505*/
	}

	
	/* begin dial by hand, 20070406, s63873 */
        if(dial_byhand)
        {
            while(!check_hand_dial())
            {
                sleep(1);
                check_msgfd(&in_fds);
            }
        }
        /* end dial by hand, 20070406 */
        remove_fd(fd_msg);
    }

    /* Wait for scripts to finish */
    /* XXX should have a timeout here */
    while (n_children > 0) {
	if (debug) {
	    struct subprocess *chp;
	    dbglog("Waiting for %d child proces.", n_children);
	    for (chp = children; chp != NULL; chp = chp->next)
		dbglog("  script %s, pid %d", chp->prog, chp->pid);
	}
	if (reap_kids(1) < 0)
	    break;
    }

    die(status);
    return 0;
}

/*
 * handle_events - wait for something to happen and respond to it.
 */
static void
handle_events()
{
    struct timeval timo;
    sigset_t mask;

    kill_link = open_ccp_flag = 0;
    if (sigsetjmp(sigjmp, 1) == 0) {
	sigprocmask(SIG_BLOCK, &mask, NULL);
	if (got_sighup || got_sigterm || got_sigusr2 || got_sigchld) {
	    sigprocmask(SIG_UNBLOCK, &mask, NULL);
	} else {
	    waiting = 1;
	    sigprocmask(SIG_UNBLOCK, &mask, NULL);
	    wait_input(timeleft(&timo));
	}
    }
    waiting = 0;
    calltimeout();
    if (got_sighup) {
	kill_link = 1;
	got_sighup = 0;
	if (status != EXIT_HANGUP)
	    status = EXIT_USER_REQUEST;
    }
    if (got_sigterm) {
	kill_link = 1;
	persist = 0;
	status = EXIT_USER_REQUEST;
	got_sigterm = 0;
    }
    if (got_sigchld) {
	reap_kids(0);	/* Don't leave dead kids lying around */
	got_sigchld = 0;
    }
    if (got_sigusr2) {
	open_ccp_flag = 1;
	got_sigusr2 = 0;
    }
}

/*
 * setup_signals - initialize signal handling.
 */
static void
setup_signals()
{
    struct sigaction sa;
    sigset_t mask;

    /*
     * Compute mask of all interesting signals and install signal handlers
     * for each.  Only one signal handler may be active at a time.  Therefore,
     * all other signals should be masked when any handler is executing.
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGUSR2);

#define SIGNAL(s, handler)	do { \
	sa.sa_handler = handler; \
	if (sigaction(s, &sa, NULL) < 0) \
	    fatal("Couldn't establish signal handler (%d): %m", s); \
    } while (0)

    sa.sa_mask = mask;
    sa.sa_flags = 0;
    SIGNAL(SIGHUP, hup);		/* Hangup */
//    SIGNAL(SIGINT, inter);		/* Interrupt */
    SIGNAL(SIGTERM, term);		/* Terminate */
    SIGNAL(SIGCHLD, chld);

    SIGNAL(SIGUSR1, toggle_debug);	/* Toggle debug flag */
    SIGNAL(SIGUSR2, open_ccp);		/* Reopen CCP */

    /*
     * Install a handler for other signals which would otherwise
     * cause pppd to exit without cleaning up.
     */
    SIGNAL(SIGABRT, processv6sig);
    //modied by z67625 
    //SIGNAL(SIGALRM, bad_signal);
    SIGNAL(SIGALRM,switchdebug);
    //modified by z67625
    SIGNAL(SIGFPE, triggerdemand);
    /*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
    SIGNAL(SIGILL, check_usr);
    /*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/
    SIGNAL(SIGPIPE, bad_signal);
    SIGNAL(SIGQUIT, bad_signal);
    SIGNAL(SIGSEGV, bad_signal);
#ifdef SIGBUS
    SIGNAL(SIGBUS, bad_signal);
#endif
#ifdef SIGEMT
    SIGNAL(SIGEMT, bad_signal);
#endif
#ifdef SIGPOLL
    SIGNAL(SIGPOLL, bad_signal);
#endif
#ifdef SIGPROF
    SIGNAL(SIGPROF, bad_signal);
#endif
#ifdef SIGSYS
    SIGNAL(SIGSYS, bad_signal);
#endif
#ifdef SIGTRAP
    SIGNAL(SIGTRAP, bad_signal);
#endif
#ifdef SIGVTALRM
    SIGNAL(SIGVTALRM, bad_signal);
#endif
#ifdef SIGXCPU
    SIGNAL(SIGXCPU, bad_signal);
#endif
#ifdef SIGXFSZ
    SIGNAL(SIGXFSZ, bad_signal);
#endif

    /*
     * Apparently we can get a SIGPIPE when we call syslog, if
     * syslogd has died and been restarted.  Ignoring it seems
     * be sufficient.
     */
    signal(SIGPIPE, SIG_IGN);
}

/*
 * set_ifunit - do things we need to do once we know which ppp
 * unit we are using.
 */
void
set_ifunit(iskey)
    int iskey;
{ 
    info("Using interface %s", wanif_name);

    snprintf(ifname, sizeof(ifname), "%s", wanif_name);
    script_setenv("IFNAME", ifname, iskey);   
}

/*
 * detach - detach us from the controlling terminal.
 */
void
detach()
{
    int pid;
    char numbuf[16];

    if (detached)
	return;
    if ((pid = fork()) < 0) {
	error("Couldn't detach (fork failed: %m)");
	die(1);			/* or just return? */
    }
    if (pid != 0) {
	/* parent */
	notify(pidchange, pid);
	exit(0);		/* parent dies */
    }
    setsid();
    chdir("/");
    close(0);
    close(1);
    close(2);
    detached = 1;
    if (log_default)
	log_to_fd = -1;
    /* update pid files if they have been written already */
//    if (pidfilename[0])
//	create_pidfile();
//    if (linkpidfile[0])
//	create_linkpidfile();
    slprintf(numbuf, sizeof(numbuf), "%d", getpid());
    script_setenv("PPPD_PID", numbuf, 1);
}

/*
 * reopen_log - (re)open our connection to syslog.
 */
void
reopen_log()
{
#ifdef ULTRIX
    openlog("pppd", LOG_PID);
#else
    openlog("pppd", LOG_PID | LOG_NDELAY, LOG_PPP);
    setlogmask(LOG_UPTO(LOG_INFO));
#endif
}


#if 0
/*
 * Create a file containing our process ID.
 */
static void
create_pidfile()
{
    FILE *pidfile;

    slprintf(pidfilename, sizeof(pidfilename), "%s%s.pid",
	     _PATH_VARRUN, ifname);
    if ((pidfile = fopen(pidfilename, "w")) != NULL) {
	fprintf(pidfile, "%d\n", getpid());
	(void) fclose(pidfile);
    } else {
	error("Failed to create pid file %s: %m", pidfilename);
	pidfilename[0] = 0;
    }
}

static void
create_linkpidfile()
{
    FILE *pidfile;

    if (linkname[0] == 0)
	return;
    script_setenv("LINKNAME", linkname, 1);
    slprintf(linkpidfile, sizeof(linkpidfile), "%sppp-%s.pid",
	     _PATH_VARRUN, linkname);
    if ((pidfile = fopen(linkpidfile, "w")) != NULL) {
	fprintf(pidfile, "%d\n", getpid());
	if (ifname[0])
	    fprintf(pidfile, "%s\n", ifname);
	(void) fclose(pidfile);
    } else {
	error("Failed to create pid file %s: %m", linkpidfile);
	linkpidfile[0] = 0;
    }
}
#endif


/*
 * holdoff_end - called via a timeout when the holdoff period ends.
 */
static void
holdoff_end(arg)
    void *arg;
{
    new_phase(PHASE_DORMANT);
}

/* List of protocol names, to make our messages a little more informative. */
struct protocol_list {
    u_short	proto;
    const char	*name;
} protocol_list[] = {
    { 0x21,	"IP" },
    { 0x23,	"OSI Network Layer" },
    { 0x25,	"Xerox NS IDP" },
    { 0x27,	"DECnet Phase IV" },
    { 0x29,	"Appletalk" },
    { 0x2b,	"Novell IPX" },
    { 0x2d,	"VJ compressed TCP/IP" },
    { 0x2f,	"VJ uncompressed TCP/IP" },
    { 0x31,	"Bridging PDU" },
    { 0x33,	"Stream Protocol ST-II" },
    { 0x35,	"Banyan Vines" },
    { 0x39,	"AppleTalk EDDP" },
    { 0x3b,	"AppleTalk SmartBuffered" },
    { 0x3d,	"Multi-Link" },
    { 0x3f,	"NETBIOS Framing" },
    { 0x41,	"Cisco Systems" },
    { 0x43,	"Ascom Timeplex" },
    { 0x45,	"Fujitsu Link Backup and Load Balancing (LBLB)" },
    { 0x47,	"DCA Remote Lan" },
    { 0x49,	"Serial Data Transport Protocol (PPP-SDTP)" },
    { 0x4b,	"SNA over 802.2" },
    { 0x4d,	"SNA" },
    { 0x4f,	"IP6 Header Compression" },
    { 0x6f,	"Stampede Bridging" },
    { 0xfb,	"single-link compression" },
    { 0xfd,	"1st choice compression" },
    { 0x0201,	"802.1d Hello Packets" },
    { 0x0203,	"IBM Source Routing BPDU" },
    { 0x0205,	"DEC LANBridge100 Spanning Tree" },
    { 0x0231,	"Luxcom" },
    { 0x0233,	"Sigma Network Systems" },
    { 0x8021,	"Internet Protocol Control Protocol" },
    { 0x8023,	"OSI Network Layer Control Protocol" },
    { 0x8025,	"Xerox NS IDP Control Protocol" },
    { 0x8027,	"DECnet Phase IV Control Protocol" },
    { 0x8029,	"Appletalk Control Protocol" },
    { 0x802b,	"Novell IPX Control Protocol" },
    { 0x8031,	"Bridging NCP" },
    { 0x8033,	"Stream Protocol Control Protocol" },
    { 0x8035,	"Banyan Vines Control Protocol" },
    { 0x803d,	"Multi-Link Control Protocol" },
    { 0x803f,	"NETBIOS Framing Control Protocol" },
    { 0x8041,	"Cisco Systems Control Protocol" },
    { 0x8043,	"Ascom Timeplex" },
    { 0x8045,	"Fujitsu LBLB Control Protocol" },
    { 0x8047,	"DCA Remote Lan Network Control Protocol (RLNCP)" },
    { 0x8049,	"Serial Data Control Protocol (PPP-SDCP)" },
    { 0x804b,	"SNA over 802.2 Control Protocol" },
    { 0x804d,	"SNA Control Protocol" },
    { 0x804f,	"IP6 Header Compression Control Protocol" },
    { 0x006f,	"Stampede Bridging Control Protocol" },
    { 0x80fb,	"Single Link Compression Control Protocol" },
    { 0x80fd,	"Compression Control Protocol" },
    { 0xc021,	"Link Control Protocol" },
    { 0xc023,	"Password Authentication Protocol" },
    { 0xc025,	"Link Quality Report" },
    { 0xc027,	"Shiva Password Authentication Protocol" },
    { 0xc029,	"CallBack Control Protocol (CBCP)" },
    { 0xc081,	"Container Control Protocol" },
    { 0xc223,	"Challenge Handshake Authentication Protocol" },
    { 0xc281,	"Proprietary Authentication Protocol" },
    { 0,	NULL },
};

/*
 * protocol_name - find a name for a PPP protocol.
 */
const char *
protocol_name(proto)
    int proto;
{
    struct protocol_list *lp;

    for (lp = protocol_list; lp->proto != 0; ++lp)
	if (proto == lp->proto)
	    return lp->name;
    return NULL;
}

static int proc_msg_fd(int newfd)
{
    char acRcvfbuf[2];
    int lRet;

    memset(acRcvfbuf, 0, sizeof(acRcvfbuf));
    while (1)
    {
        lRet = read(newfd, acRcvfbuf, sizeof(acRcvfbuf));
        if ((lRet == 0) ||
            ((lRet == -1) && (errno == ECONNRESET)))
        {
            close(newfd);
            return -1;
        }
        else if (lRet < 0)
        {
            if ((EINTR == errno) || (EAGAIN == errno))
            {
                continue;
            }
            close(newfd);
            return -1;
        }
        break;
    }
    close(newfd);
    /*检测是否Quit 退出,替换原来的信号*/
    if (0 == strcmp(acRcvfbuf, "Q"))
    {
        toggle_debug(SIGUSR1);
    }
    return 0;
}

extern int num_np_open;
/*
 * get_input - called when incoming data is available.
 */
static void
get_input()
{
    int len, i;
    u_char *p;
    u_short protocol;
    struct protent *protp;

    check_msgfd(&in_fds);
    p = inpacket_buf;	/* point to beginning of packet buffer */
    
    len = read_packet(inpacket_buf);
    
/*当select返回n>1时，需要要求read返回nr>1才合理，但b560在APN配置错误后这种情况持续出现，造成循环*/  
/*但不能让它退出，因为正常的pppc也会出现少量select_read不一致问题，这里做规避处理*/
/*问题单AU4D01336 by g00121640*/
#if 1
    if ((len < 0)&& (1 == atp_time_flag))
    {
        sleep(1);
        if(debug)
        {
            printf("\n ***************** the n return from select is >0 \n");
            printf("\n ---------------- the len return read_packet is %d, the errno is %d\n",len,errno);
        }
        return;
    }
#endif
    if (len == 0) {
	notice("Modem hangup");
	hungup = 1;
	status = EXIT_HANGUP;
	lcp_lowerdown(0);	/* serial link is no longer available */
	link_terminated(0);
	return;
    }

    /*拨上号以后，LEN的长度小于0不合理，增加判断 */
    if (debug && (len > 0) /*&& (debugflags & DBG_INPACKET)*/)
	dbglog("rcvd %P", p, len);

    if (len < PPP_HDRLEN) {
	MAINDEBUG(("io(): Received short packet."));
	return;
    }

    p += 2;				/* Skip address and control */
    GETSHORT(protocol, p);
    len -= PPP_HDRLEN;

    /*
     * Toss all non-LCP packets unless LCP is OPEN.
     */
    if (protocol != PPP_LCP && lcp_fsm[0].state != OPENED) {
	MAINDEBUG(("get_input: Received non-LCP packet when LCP not open."));
	return;
    }

    /*
     * Until we get past the authentication phase, toss all packets
     * except LCP, LQR and authentication packets.
     */
    if (phase <= PHASE_AUTHENTICATE
	&& !(protocol == PPP_LCP || protocol == PPP_LQR
	     || protocol == PPP_PAP || protocol == PPP_CHAP)) {
	MAINDEBUG(("get_input: discarding proto 0x%x in phase %d",
		   protocol, phase));
	return;
    }

    /*
     * Upcall the proper protocol input routine.
     */
    for (i = 0; (protp = protocols[i]) != NULL; ++i) {
	if (protp->protocol == protocol && protp->enabled_flag) {
	    (*protp->input)(0, p, len);
	    return;
	}
        if (protocol == (protp->protocol & ~0x8000) && protp->enabled_flag
	    && protp->datainput != NULL) {
	    (*protp->datainput)(0, p, len);
	    return;
	}
    }

    if (debug) {
	const char *pname = protocol_name(protocol);
	if (pname != NULL)
	    warn("Unsupported protocol '%s' (0x%x) received", pname, protocol);
	else
	    warn("Unsupported protocol 0x%x received", protocol);
    }

#ifdef  SUPPORT_ATP_PPP_DYN_DIAL
//for W724v , v4 v6分开拨号
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
    DT_RedialAfterNpUp(protocol);
#endif
   if (PPP_IP != protocol && PPP_IPV6 != protocol)
#endif
    lcp_sprotrej(0, p - PPP_HDRLEN, len + PPP_HDRLEN);
}

/*
 * new_phase - signal the start of a new phase of pppd's operation.
 */
void
new_phase(p)
    int p;
{
    phase = p;
    if (new_phase_hook)
	(*new_phase_hook)(p);
    notify(phasechange, p);
}

/*
 * die - clean up state and exit with the specified status.
 */
void
die(status)
    int status;
{
    cleanup();
    notify(exitnotify, status);
    syslog(LOG_INFO, "Exit.");
    exit(status);
}

/*
 * cleanup - restore anything which needs to be restored before we exit
 */
/* ARGSUSED */
static void
cleanup()
{
    sys_cleanup();

    if (fd_ppp >= 0)
	the_channel->disestablish_ppp(devfd);
    if (the_channel->cleanup)
	(*the_channel->cleanup)();

    if (pidfilename[0] != 0 && unlink(pidfilename) < 0 && errno != ENOENT)
	warn("unable to delete pid file %s: %m", pidfilename);
    pidfilename[0] = 0;
    if (linkpidfile[0] != 0 && unlink(linkpidfile) < 0 && errno != ENOENT)
	warn("unable to delete pid file %s: %m", linkpidfile);
    linkpidfile[0] = 0;

    if (pppdb != NULL)
	cleanup_db();
}

/*
 * update_link_stats - get stats at link termination.
 */
void
update_link_stats(u)
    int u;
{
    struct timeval now;
    char numbuf[32];

    if (!get_ppp_stats(u, &link_stats)
	|| ATP_UTIL_GetSysTime(&now) < 0)
	return;
    link_connect_time = now.tv_sec - start_time.tv_sec;
    link_stats_valid = 1;

    slprintf(numbuf, sizeof(numbuf), "%d", link_connect_time);
    script_setenv("CONNECT_TIME", numbuf, 0);
    slprintf(numbuf, sizeof(numbuf), "%d", link_stats.bytes_out);
    script_setenv("BYTES_SENT", numbuf, 0);
    slprintf(numbuf, sizeof(numbuf), "%d", link_stats.bytes_in);
    script_setenv("BYTES_RCVD", numbuf, 0);
}


struct	callout {
    struct timeval	c_time;		/* time at which to call routine */
    void		*c_arg;		/* argument to routine */
    void		(*c_func) __P((void *)); /* routine */
    struct		callout *c_next;
};

static struct callout *callout = NULL;	/* Callout list */
static struct timeval timenow;		/* Current time */

/*
 * timeout - Schedule a timeout.
 *
 * Note that this timeout takes the number of milliseconds, NOT hz (as in
 * the kernel).
 */
void
timeout(func, arg, secs, usecs)
    void (*func) __P((void *));
    void *arg;
    int secs, usecs;
{
    struct callout *newp, *p, **pp;

    MAINDEBUG(("Timeout %p:%p in %d.%03d seconds.", func, arg,
	       secs, usecs));

    /*
     * Allocate timeout.
     */
    if ((newp = (struct callout *) malloc(sizeof(struct callout))) == NULL)
	fatal("Out of memory in timeout()!");
    newp->c_arg = arg;
    newp->c_func = func;
    //gettimeofday(&timenow, NULL);
    ATP_UTIL_GetSysTime(&timenow);
    newp->c_time.tv_sec = timenow.tv_sec + secs;
    newp->c_time.tv_usec = timenow.tv_usec + usecs;
    if (newp->c_time.tv_usec >= 1000000) {
	newp->c_time.tv_sec += newp->c_time.tv_usec / 1000000;
	newp->c_time.tv_usec %= 1000000;
    }

    /*
     * Find correct place and link it in.
     */
    for (pp = &callout; (p = *pp); pp = &p->c_next)
	if (newp->c_time.tv_sec < p->c_time.tv_sec
	    || (newp->c_time.tv_sec == p->c_time.tv_sec
		&& newp->c_time.tv_usec < p->c_time.tv_usec))
	    break;
    newp->c_next = p;
    *pp = newp;
}


/*
 * untimeout - Unschedule a timeout.
 */
void
untimeout(func, arg)
    void (*func) __P((void *));
    void *arg;
{
    struct callout **copp, *freep;

    MAINDEBUG(("Untimeout %p:%p.", func, arg));

    /*
     * Find first matching timeout and remove it from the list.
     */
    for (copp = &callout; (freep = *copp); copp = &freep->c_next)
	if (freep->c_func == func && freep->c_arg == arg) {
	    *copp = freep->c_next;
	    free((char *) freep);
	    break;
	}
}


/*
 * calltimeout - Call any timeout routines which are now due.
 */
static void
calltimeout()
{
    struct callout *p;

    while (callout != NULL) {
	p = callout;

	if (ATP_UTIL_GetSysTime(&timenow) < 0)
	    fatal("Failed to get time of day: %m");
	if (!(p->c_time.tv_sec < timenow.tv_sec
	      || (p->c_time.tv_sec == timenow.tv_sec
		  && p->c_time.tv_usec <= timenow.tv_usec)))
	    break;		/* no, it's not time yet */

	callout = p->c_next;
	(*p->c_func)(p->c_arg);

	free((char *) p);
    }
}


/*
 * timeleft - return the length of time until the next timeout is due.
 */
static struct timeval *
timeleft(tvp)
    struct timeval *tvp;
{
    if (callout == NULL)
	return NULL;

    //gettimeofday(&timenow, NULL);
    ATP_UTIL_GetSysTime(&timenow);
    tvp->tv_sec = callout->c_time.tv_sec - timenow.tv_sec;
    tvp->tv_usec = callout->c_time.tv_usec - timenow.tv_usec;
    if (tvp->tv_usec < 0) {
	tvp->tv_usec += 1000000;
	tvp->tv_sec -= 1;
    }
    if (tvp->tv_sec < 0)
	tvp->tv_sec = tvp->tv_usec = 0;

    return tvp;
}


/*
 * kill_my_pg - send a signal to our process group, and ignore it ourselves.
 */
static void
kill_my_pg(sig)
    int sig;
{
    struct sigaction act, oldact;

    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    kill(0, sig);
    sigaction(sig, &act, &oldact);
    sigaction(sig, &oldact, NULL);
}


/*
 * hup - Catch SIGHUP signal.
 *
 * Indicates that the physical layer has been disconnected.
 * We don't rely on this indication; if the user has sent this
 * signal, we just take the link down.
 */
static void
hup(sig)
    int sig;
{
    got_sighup = 1;
    if (conn_running)
	/* Send the signal to the [dis]connector process(es) also */
	kill_my_pg(sig);
    notify(sigreceived, sig);
    if (waiting)
	siglongjmp(sigjmp, 1);
}

/*恢复开关，使收到TERM信号后能处理*/
void enable_term(void)
{
    bEnableTerm = 1;
}

void disable_term(void)
{
    bEnableTerm = 0;
}


/*
 * term - Catch SIGTERM signal and SIGINT signal (^C/del).
 *
 * Indicates that we should initiate a graceful disconnect and exit.
 */
/*ARGSUSED*/
static void
term(sig)
    int sig;
{
    if (!bEnableTerm)
    {
        if (waiting)
        {
	        siglongjmp(sigjmp, 1);
	    }
	    else
	    {
	        return;
	    }
	}
    got_sigterm = 1;
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
//确保使用goonflag = 1恢复按需拨号模式
    if (demand)
    {
        goonflag = 1;
    }
#endif	
    if (conn_running)
	/* Send the signal to the [dis]connector process(es) also */
	kill_my_pg(sig);
    notify(sigreceived, sig);
    if (waiting)
	siglongjmp(sigjmp, 1);
}


/*
 * chld - Catch SIGCHLD signal.
 * Sets a flag so we will call reap_kids in the mainline.
 */
static void
chld(sig)
    int sig;
{
    got_sigchld = 1;
    if (waiting)
	siglongjmp(sigjmp, 1);
}

static void processv6sig(sig)
    int sig;

{
    ipv6cp_down_sig(0);
}

/*
 * toggle_debug - Catch SIGUSR1 signal.
 *
 * Toggle debug flag.
 */
/*ARGSUSED*/

/*Start of ATP 2014-4-24 for DTS2014031501735 by y00250667*/
static void pppoe_delete_dns(const char* pcDns)
{
    ATP_DNS_SERVER_MSG_ST stDnsMsg;
	
    if ((NULL == pcDns) || (0 == strlen(pcDns)))
    {
        return;
    }
	
    memset(&stDnsMsg, 0, sizeof(stDnsMsg));
    snprintf(stDnsMsg.acWanAddr, sizeof(stDnsMsg.acWanAddr), "%s", ouraddr);
    snprintf(stDnsMsg.acDnsServers, sizeof(stDnsMsg.acDnsServers), "%s", pcDns);
    snprintf(stDnsMsg.acInterface, sizeof(stDnsMsg.acInterface), "%s", wanif_name);
    snprintf(stDnsMsg.acServiceType, sizeof(stDnsMsg.acServiceType), "%s", PPP_SERVICE_TYPE);
    
    ATP_MSG_SendMsgData(ATP_CBBID_DNS_NAME, ATP_MSG_DNS_DELETE_SERVER, &stDnsMsg, sizeof(stDnsMsg));
}
/*End of ATP 2014-4-24 for DTS2014031501735 by y00250667*/

static void
toggle_debug(sig)
    int sig;
{
    
    if (ATP_UTIL_VoiceGetSipInterval() != 0)
    {
        sleep(2);               // sleep 2s for the voice to receive the pppc down message
    }
    debug = !debug;
    if (debug) {
	setlogmask(LOG_UPTO(LOG_DEBUG));
    } else {
	setlogmask(LOG_UPTO(LOG_WARNING));
    }
    /*add by z67625 2007-09-01 start*/
    #ifdef SUPPORT_ATP_PPPD
    if(pppx)
    {
        /*清理帐号信息*/
        //pppx_setwan_state(log_name, PPPX_WAN_DOWN);
        ipcp_down_ext();
        pppx_cleanAcnt(log_name);
    }
    #endif
    
    /* start of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */
    iIsExist = 1;
    /* end of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */

    // TODO: down掉IPV6CP
#ifdef SUPPORT_ATP_PPPC6
   stop_dhcp6c(); 
#endif
    //add by z67625
   //session_disconnect(ses);
    if (0 == access(PRIVACY_BTN_PPPC_FLAG_FILE, F_OK))
    {
        setdialstatus(Discon); 
        while (phase != PHASE_DEAD)
        {   
            handle_events();
            get_input();
        }
        unlink(PRIVACY_BTN_PPPC_FLAG_FILE);
    }   
    the_channel->disconnect();
    
    /*start of y42304 2008-2-15: 解决问题AU4D00391:*/
    {        
        char cmd[64];
        memset(cmd, 0, sizeof(cmd));   

        snprintf(cmd, sizeof(cmd), "/var/wan/%s/pppc_tcp_mss", wanif_name);
        unlink(cmd);
        clear_tcp_mss();
    }    
    /*end of y42304 2008-2-15: 解决问题AU4D00391:*/
      
    /* start of ATP 2008.02.02 ATPV100R002C01B021 AU4D00321 ppp的设置由pppc在会话协商时自行设置 */
    /* tcp mss */
    
/* end of ATP 2008.02.02 ATPV100R002C01B021 AU4D00321 ppp的设置由pppc在会话协商时自行设置 */
    pppoe_setstatus(Disconnected);
    
	/*Start of ATP 2014-4-24 for DTS2014031501735 by y00250667*/
    /*wan down 后删除dns*/
    pppoe_delete_dns(acDns);
	/*End of ATP 2014-4-24 for DTS2014031501735 by y00250667*/
    
    exit(0);
    /*add by z67625 2007-09-01 end*/
}

/*add by z67625 start*/
static void
switchdebug(sig)
    int sig;
{
    debug = !debug;
}
/*add by z67625 end*/

static void triggerdemand(sig)
{
  if(demand)
  trigger_demand=1; 
}



/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
static void
check_usr(sig)
    int sig;
{
    switchgate(0);
#ifdef SUPPORT_ATP_PPPD
    if(pppx)
        pppx_check_user();
#endif
}
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/

/*
 * open_ccp - Catch SIGUSR2 signal.
 *
 * Try to (re)negotiate compression.
 */
/*ARGSUSED*/
static void
open_ccp(sig)
    int sig;
{
    got_sigusr2 = 1;
    if (waiting)
	siglongjmp(sigjmp, 1);
}


/*
 * bad_signal - We've caught a fatal signal.  Clean up state and exit.
 */
static void
bad_signal(sig)
    int sig;
{
    static int crashed = 0;

    if (crashed)
	_exit(127);
    crashed = 1;
    error("Fatal signal %d", sig);
    if (conn_running)
	kill_my_pg(SIGTERM);
    notify(sigreceived, sig);
    die(127);
}


/*
 * device_script - run a program to talk to the specified fds
 * (e.g. to run the connector or disconnector script).
 * stderr gets connected to the log fd or to the _PATH_CONNERRS file.
 */
int
device_script(program, in, out, dont_wait)
    char *program;
    int in, out;
    int dont_wait;
{
    int pid, fd;
    int status = -1;
    int errfd;

    ++conn_running;
    pid = fork();

    if (pid < 0) {
	--conn_running;
	error("Failed to create child process: %m");
	return -1;
    }

    if (pid != 0) {
	if (dont_wait) {
	    record_child(pid, program, NULL, NULL);
	    status = 0;
	} else {
	    while (waitpid(pid, &status, 0) < 0) {
		if (errno == EINTR)
		    continue;
		fatal("error waiting for (dis)connection process: %m");
	    }
	    --conn_running;
	}
	return (status == 0 ? 0 : -1);
    }

    /* here we are executing in the child */
    /* make sure fds 0, 1, 2 are occupied */
    while ((fd = dup(in)) >= 0) {
	if (fd > 2) {
	    close(fd);
	    break;
	}
    }

    /* dup in and out to fds > 2 */
    in = dup(in);
    out = dup(out);
    if (log_to_fd >= 0) {
	errfd = dup(log_to_fd);
    } else {
	errfd = open(_PATH_CONNERRS, O_WRONLY | O_APPEND | O_CREAT, 0600);
    }

    /* close fds 0 - 2 and any others we can think of */
    close(0);
    close(1);
    close(2);
    sys_close();
    if (the_channel->close)
	(*the_channel->close)();
    closelog();

    /* dup the in, out, err fds to 0, 1, 2 */
    dup2(in, 0);
    close(in);
    dup2(out, 1);
    close(out);
    if (errfd >= 0) {
	dup2(errfd, 2);
	close(errfd);
    }

    setuid(uid);
    if (getuid() != uid) {
	error("setuid failed");
	exit(1);
    }
    setgid(getgid());
    execl("/bin/sh", "sh", "-c", program, (char *)0);
    error("could not exec /bin/sh: %m");
    exit(99);
    /* NOTREACHED */
}


/*
 * run-program - execute a program with given arguments,
 * but don't wait for it.
 * If the program can't be executed, logs an error unless
 * must_exist is 0 and the program file doesn't exist.
 * Returns -1 if it couldn't fork, 0 if the file doesn't exist
 * or isn't an executable plain file, or the process ID of the child.
 * If done != NULL, (*done)(arg) will be called later (within
 * reap_kids) iff the return value is > 0.
 */
pid_t
run_program(prog, args, must_exist, done, arg)
    char *prog;
    char **args;
    int must_exist;
    void (*done) __P((void *));
    void *arg;
{
    int pid;
    struct stat sbuf;

    /*
     * First check if the file exists and is executable.
     * We don't use access() because that would use the
     * real user-id, which might not be root, and the script
     * might be accessible only to root.
     */
    errno = EINVAL;
    if (stat(prog, &sbuf) < 0 || !S_ISREG(sbuf.st_mode)
	|| (sbuf.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)) == 0) {
	if (must_exist || errno != ENOENT)
	    warn("Can't execute %s: %m", prog);
	return 0;
    }

    pid = fork();
    if (pid == -1) {
	error("Failed to create child process for %s: %m", prog);
	return -1;
    }
    if (pid == 0) {
	int new_fd;

	/* Leave the current location */
	(void) setsid();	/* No controlling tty. */
	(void) umask (S_IRWXG|S_IRWXO);
	(void) chdir ("/");	/* no current directory. */
	setuid(0);		/* set real UID = root */
	setgid(getegid());

	/* Ensure that nothing of our device environment is inherited. */
	sys_close();
	closelog();
	close (0);
	close (1);
	close (2);
	if (the_channel->close)
	    (*the_channel->close)();

        /* Don't pass handles to the PPP device, even by accident. */
	new_fd = open (_PATH_DEVNULL, O_RDWR);
	if (new_fd >= 0) {
	    if (new_fd != 0) {
	        dup2  (new_fd, 0); /* stdin <- /dev/null */
		close (new_fd);
	    }
	    dup2 (0, 1); /* stdout -> /dev/null */
	    dup2 (0, 2); /* stderr -> /dev/null */
	}

#ifdef BSD
	/* Force the priority back to zero if pppd is running higher. */
	if (setpriority (PRIO_PROCESS, 0, 0) < 0)
	    warn("can't reset priority to 0: %m");
#endif

	/* SysV recommends a second fork at this point. */

	/* run the program */
	execve(prog, args, script_env);
	if (must_exist || errno != ENOENT) {
	    /* have to reopen the log, there's nowhere else
	       for the message to go. */
	    reopen_log();
	    syslog(LOG_ERR, "Can't execute %s: %m", prog);
	    closelog();
	}
	_exit(-1);
    }

    if (debug)
	dbglog("Script %s started (pid %d)", prog, pid);
    record_child(pid, prog, done, arg);

    return pid;
}


/*
 * record_child - add a child process to the list for reap_kids
 * to use.
 */
void
record_child(pid, prog, done, arg)
    int pid;
    char *prog;
    void (*done) __P((void *));
    void *arg;
{
    struct subprocess *chp;

    ++n_children;

    chp = (struct subprocess *) malloc(sizeof(struct subprocess));
    if (chp == NULL) {
	warn("losing track of %s process", prog);
    } else {
	chp->pid = pid;
	chp->prog = prog;
	chp->done = done;
	chp->arg = arg;
	chp->next = children;
	children = chp;
    }
}


/*
 * reap_kids - get status from any dead child processes,
 * and log a message for abnormal terminations.
 */
static int
reap_kids(waitfor)
    int waitfor;
{
    int pid, status;
    struct subprocess *chp, **prevp;

    if (n_children == 0)
	return 0;
    while ((pid = waitpid(-1, &status, (waitfor? 0: WNOHANG))) != -1
	   && pid != 0) {
	for (prevp = &children; (chp = *prevp) != NULL; prevp = &chp->next) {
	    if (chp->pid == pid) {
		--n_children;
		*prevp = chp->next;
		break;
	    }
	}
	if (WIFSIGNALED(status)) {
	    warn("Child process %s (pid %d) terminated with signal %d",
		 (chp? chp->prog: "??"), pid, WTERMSIG(status));
	} else if (debug)
	    dbglog("Script %s finished (pid %d), status = 0x%x",
		   (chp? chp->prog: "??"), pid, status);
	if (chp && chp->done)
	    (*chp->done)(chp->arg);
	if (chp)
	    free(chp);
    }
    if (pid == -1) {
	if (errno == ECHILD)
	    return -1;
	if (errno != EINTR)
	    error("Error waiting for child process: %m");
    }
    return 0;
}

/*
 * add_notifier - add a new function to be called when something happens.
 */
void
add_notifier(notif, func, arg)
    struct notifier **notif;
    notify_func func;
    void *arg;
{
    struct notifier *np;

    np = malloc(sizeof(struct notifier));
    if (np == 0)
	novm("notifier struct");
    np->next = *notif;
    np->func = func;
    np->arg = arg;
    *notif = np;
}

/*
 * remove_notifier - remove a function from the list of things to
 * be called when something happens.
 */
void
remove_notifier(notif, func, arg)
    struct notifier **notif;
    notify_func func;
    void *arg;
{
    struct notifier *np;

    for (; (np = *notif) != 0; notif = &np->next) {
	if (np->func == func && np->arg == arg) {
	    *notif = np->next;
	    free(np);
	    break;
	}
    }
}

/*
 * notify - call a set of functions registered with add_notify.
 */
void
notify(notif, val)
    struct notifier *notif;
    int val;
{
    struct notifier *np;

    while ((np = notif) != 0) {
	notif = np->next;
	(*np->func)(np->arg, val);
    }
}

/*
 * novm - log an error message saying we ran out of memory, and die.
 */
void
novm(msg)
    char *msg;
{
    fatal("Virtual memory exhausted allocating %s\n", msg);
}

/*
 * script_setenv - set an environment variable value to be used
 * for scripts that we run (e.g. ip-up, auth-up, etc.)
 */
void
script_setenv(var, value, iskey)
    char *var, *value;
    int iskey;
{

}

/*
 * script_unsetenv - remove a variable from the environment
 * for scripts.
 */
void
script_unsetenv(var)
    char *var;
{

}

/*
 * update_db_entry - update our entry in the database.
 */
static void
update_db_entry()
{

}
#if 0
/*
 * add_db_key - add a key that we can use to look up our database entry.
 */
static void
add_db_key(str)
    const char *str;
{

}

/*
 * delete_db_key - delete a key for looking up our database entry.
 */
static void
delete_db_key(str)
    const char *str;
{

}
#endif
/*
 * cleanup_db - delete all the entries we put in the database.
 */
static void
cleanup_db()
{

}
/*add by z67625 start*/
/*获取wan状态*/
int pppoe_getstatus(void)
{
    int lRet = -1;
    FILE *fp = NULL;
    char path[128] = "";
    char cmd[128] = "";

    snprintf(path, sizeof(path), "%s%s/status",WAN_PATH,wanif_name);
    if(NULL == (fp = fopen(WAN_PATH,"r")))
        return lRet;
    fgets(cmd,128,fp);
    lRet = atoi(cmd);
    fclose(fp);
    return lRet;
}

#ifdef SUPPORT_ATP_PPPC6

#ifdef SUPPORT_ATP_TR181
void PPPC6SendStautsChangeMsg(VOS_UINT32 ulState) 
{
    PPP_DEBUG("PPPC6SendStautsChangeMsg\r\n");
    
    struct stDhcp6cStatus stDhcp6cStatusData;
    
    memset(&stDhcp6cStatusData,0, sizeof(struct stDhcp6cStatus));

    stDhcp6cStatusData.ulStatus= ulState;
    snprintf(stDhcp6cStatusData.acInterface, sizeof(stDhcp6cStatusData.acInterface), "%s", wanif_name);
    ATP_MSG_SendMsgData(ATP_CBBID_CMS_NAME, ATP_MSG_IPV6_STATUS_CHANGE, (VOS_VOID*)&stDhcp6cStatusData, sizeof(struct stDhcp6cStatus));
}
#endif

int pppoe_setstatus6(int pppstatus)
{
    int lRet = 0;

#if 0
    snprintf(path, sizeof(path), "%s%s/status6",WAN_PATH,wanif_name);
    while((NULL == (fp = fopen(path,"w"))) && i++ <2)
    {
        /* WAN已去初始化但进程还没有退出的情况会写失败，其它情况不允许写失败 */
        if(i == 1)
        {
            printf("pppc -I %s set status[%d] failed.\r\n", wanif_name, pppstatus);
            usleep(100);
        }
        else
        {
            /* 再试图打开一次 */
            printf("pppc -I %s set status[%d] failed secondly.\r\n", wanif_name, pppstatus);
            return lRet;
        }
    }
    snprintf(cmd, sizeof(cmd), "%d",pppstatus);
    fputs(cmd,fp);
    fclose(fp);

#ifdef SUPPORT_ATP_TR181
    PPPC6SendStautsChangeMsg((VOS_UINT32)pppstatus);   
#endif

#endif
    //如果未检测到失败原因，最后设置为ERROR_UNKNOWN
    if (Disconnected == pppstatus)
    {
        pppoe_setlasterror("ERROR_UNKNOWN");
    }
    
    //printf("pppc -I %s set status[%d] success.\r\n", wanif_name, pppstatus);
    /*add by z67625 解决ppp代理组播掉线后不能重组的问题,2007-11-30 start*/
    //如果为ppp代理，且设置状态为Connected的时候，不调wakeup

    if(!pppx || Connected != pppstatus)
    {
        //BSP_SYS_WakeupMonitorTask();
        //notifywanstatus();
    }
    /*add by z67625 解决ppp代理组播掉线后不能重组的问题,2007-11-30 end*/
    lRet = 1;
    return lRet;
}

#endif

#ifdef SUPPORT_ATP_TR181
static void PPPCSendStautsChangeMsg(VOS_UINT32 ulState) 
{
    static VOS_BOOL bLastIsUP = VOS_FALSE;//记录上次发送的消息是否是up
    VOS_BOOL bCurIsUP = VOS_FALSE;//当前发送的是不是up消息

    if ((ulState != Connected) && (ulState != Disconnected))
    {
        return;
    }
    PPP_DEBUG("PPPCSendStautsChangeMsg\r\n");
    ATP_DHCPC_STATUSCHG_MSG_ST stMsgData = {0, {0}};
    bCurIsUP = (ulState == Connected);//当前是否是up
    if (bLastIsUP == bCurIsUP)//当前和上次是否相同
    {
        printf("\nPPPCSendStautsChangeMsg not send\r\n");
        return;//up 之间的中间状态只允许发送一次，减少cms的负担
    }
    bLastIsUP = bCurIsUP;
    stMsgData.ulStatus = ulState;
    snprintf(stMsgData.acIntfName, sizeof(stMsgData.acIntfName), "%s", wanif_name);
    ATP_MSG_SendMsgData(ATP_CBBID_CMS_NAME, ATP_MSG_PPPC_STATUS_CHANGE, (VOS_VOID*)&stMsgData, sizeof(ATP_DHCPC_STATUSCHG_MSG_ST));
}
#else
#define PPPCSendStautsChangeMsg(x...)
#endif

/*设置wan状态*/
int pppoe_setstatus(int pppstatus)
{
    int lRet = 0;
    FILE *fp = NULL;
    char path[128] = "";
    char cmd[128] = "";
    int i = 0;

    if (Connected != pppstatus)
    {
        UNTIMEOUT(pppoe_setstatus_ipcp_up_timeout, NULL);
    }

    snprintf(path, sizeof(path), "%s%s/status",WAN_PATH,wanif_name);
    while((NULL == (fp = fopen(path,"w"))) && i++ <2)
    {
        /* WAN已去初始化但进程还没有退出的情况会写失败，其它情况不允许写失败 */
        if(i == 1)
        {
            printf("pppc -I %s set status[%d] failed.\r\n", wanif_name, pppstatus);
            usleep(100);
        }
        else
        {
            /* 再试图打开一次 */
            printf("pppc -I %s set status[%d] failed secondly.\r\n", wanif_name, pppstatus);
            return lRet;
        }
    }
    snprintf(cmd, sizeof(cmd), "%d",pppstatus);
    fputs(cmd,fp);
    fclose(fp);
    
    PPPCSendStautsChangeMsg((VOS_UINT32)pppstatus);

    //如果未检测到失败原因，最后设置为ERROR_UNKNOWN
    if (Disconnected == pppstatus)
    {
        pppoe_setlasterror("ERROR_UNKNOWN");
    }
    
    //printf("pppc -I %s set status[%d] success.\r\n", wanif_name, pppstatus);
    /*add by z67625 解决ppp代理组播掉线后不能重组的问题,2007-11-30 start*/
    //如果为ppp代理，且设置状态为Connected的时候，不调wakeup

    if(!pppx || Connected != pppstatus)
    {
        //BSP_SYS_WakeupMonitorTask();
        notifywanstatus();
    }
    /*add by z67625 解决ppp代理组播掉线后不能重组的问题,2007-11-30 end*/
    lRet = 1;
    return lRet;
    
}

#define IPCP_UP_WAIT_TIME (2)//v4 up到update之间的等待时间
static void* pppoe_setstatus_ipcp_up_timeout(void *s)
{
    ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, ATP_MSG_PPPC_STATUS_UPDATE, 0);
    return NULL;
}

/*如果v4up前收到过对端ipv6cp的报文，等待一段时间再通知v4 update事件*/
/*部分进程关注v4 update事件，之后再发dns请求 for HOMEGW-14424 by t00189036*/
void pppoe_setstatus_ipcp_up()
{
    pppoe_setstatus(Connected);
    /*如果有v6，就等待2秒再发update消息给cms*/
    /*没有v6，就立马发送update消息*/
    if (1 == g_iRecvIpv6cp)
    {
        TIMEOUT(pppoe_setstatus_ipcp_up_timeout, NULL, IPCP_UP_WAIT_TIME);
    }
    else
    {
        ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, ATP_MSG_PPPC_STATUS_UPDATE, 0);
    }
    
}


/*设置最后一次拨号失败原因*/
int pppoe_setlasterror(char *errorstr)
{
    int lRet = 0;
    FILE *fp = NULL;
    char path[128] = "";
    if(NULL == errorstr)
        return lRet;

    snprintf(path, sizeof(path), "%s%s/err",WAN_PATH,wanif_name);
    if(!lasterror) //每次连接如果没有设置lasterror就设置一次原因，否则不设置
    {
        if(NULL == (fp = fopen(path,"w")))
        {
            printf("setlasterror : open file %s failed",path);
            return lRet;
        }
        fputs(errorstr,fp);
        fclose(fp);
        lRet = 1;
        lasterror = 1;
    }
    return lRet;
}
/*add by z67625 start*/

#ifdef SUPPORT_ATP_PPPD
static void pppx_cleanAcnt(char *name)
{
    struct tagPPPXAcntSet *pAcntSet = NULL;
    struct tagPPPXWan     *pWan     = NULL;
    int i = 0;
    if(NULL == name)
        return;
    if(NULL != (pAcntSet = PPPX_GetAcntSet(shm_id)))
    {
        
        for(i = 0; i < PPPX_ACCOUNT_MAX; i++)
        {
           if(PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
               continue;
           
           if(strcmp(pAcntSet->item[i].wan.name, name) == 0)
           {
               memset(&(pAcntSet->item[i]),0,sizeof(struct tagPPPXAcnt));
               pAcntSet->item[i].state = PPPX_ACNT_UNUSED;
               printf("find Acnt !!\n");
               PPPX_PutAcntSet(pAcntSet);
               return;
           }
        }
    }
    else
         printf("PPPOE ERR: %s ,shm_id=%d, name=%s\n",__FUNCTION__, shm_id, log_name);

    if(debug)
        printf("int pppx_clean Acnt,can't find name %s 's Acnt",name);

    if(NULL != pAcntSet)
        PPPX_PutAcntSet(pAcntSet);
}
static int pppx_nego_shmid(void)
{
    FILE *fd = NULL;
    char szData[16] = "";
    char szCmd[128] = "";
    int nShmid = -1;

    if (NULL != (fd = fopen(PPPX_NEGO_SHMID_PATH, "r")) )
    {
    	fgets(szData, 8, fd);
    	fclose(fd);
    	nShmid = atoi(szData);
    	if(debug)
    	    printf("PPP WAN: get shm id <%d>, pid<%d>\n", nShmid, getpid());
    }
    else
    {
        nShmid = PPPX_CreateAcntSet(PPPX_SHM_KEY);
        snprintf(szCmd, sizeof(szCmd), "echo %d > %s", nShmid, PPPX_NEGO_SHMID_PATH);
        ATP_UTIL_ForkProcess(szCmd, -1, NULL, NULL);
        sleep(0);
        
    	if(debug)
    	    printf("PPP WAN: create shm id <%d>, pid<%d>\n", nShmid, getpid());
    }

    return nShmid;
}

#endif

/* BEGIN: Added by y67514, 2008/12/18   问题单号:ET128*/
#ifdef SUPPORT_TD_CARD
#define TIOCMGETLINE      0x741e
#define TIOCMSETLINE      0x741f
#define TIOCMSET	0x741a		/* set all modem bits */

/*<AP7D02592 wupeng 20081122 modify begin*/
/*
 *初始化3,4 interface
 *
*/
int init_usb_port_modemPort2(int usb_fd)
{
    //wu add 1120
    int set_coding    = 0x0;
    int set_control_3 = 0x3;

    //printf("++enter into set 3,4 interface function.");

    //control 
    if (ioctl(usb_fd, TIOCMSET, &set_control_3))// 3
    {
        printf("[%s,%d]3,4interface:control error,errno is %d.",__FILE__,__LINE__,errno);
    }

    //line coding
    if(ioctl(usb_fd,TIOCMSETLINE))
    {
        printf("[%s,%d]3,4interface:set line coding error,errno is %d.",__FILE__,__LINE__,errno);
    }

    //control
    if (ioctl(usb_fd, TIOCMSET, &set_control_3))// 3
    {
        printf("[%s,%d]3,4interface:control error,errno is %d.",__FILE__,__LINE__,errno);
    }

    //line coding
    if(ioctl(usb_fd,TIOCMSETLINE))
    {
        printf("[%s,%d]3,4interface:set line coding error,errno is %d.",__FILE__,__LINE__,errno);
    }

    //control
    if (ioctl(usb_fd, TIOCMSET, &set_control_3))// 3
    {
        printf("[%s,%d]3,4interface:control error,errno is %d.",__FILE__,__LINE__,errno);
    }

    return 0;
}
/*AP7D02592 wupeng 20081122 modify end>*/
#endif
/* END:   Added by y67514, 2008/12/18 */

#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
void get_pado_process(void)
{

    turn_around_count = 1;
    counter = 0;
    
    FILE* fp = fopen(PPP_CURDEVNAME, "w+");
    if (fp)
    {
        fprintf(fp, "%s", devnam);
        fclose(fp);
    }
    
    warn("devnam_original=%s, devname=%s\n", devnam_original, devnam);
	if(0 == strcmp(devnam_original,devnam))
	{
		//printf("\n ===================no need to info cms vlan interface change=======\n");
		return;
	}
	
    ATP_MSG_SendMsgData(ATP_CBBID_CMS_NAME, ATP_MSG_CMS_CONFIRM_VLAN_INF, devnam, PATHLENGTH);


    //printf("\nthe results  at %s,%s,%d,the devnam is :%s...\n",__FILE__,__FUNCTION__,__LINE__,devnam);    
}
#endif














/*------------------------------------------------------------
  函数原型 : int pppd_at_dial(int modem_fd)
  描述     : AT拨号建立连接
  输入     : modem_fd:modem口文件句柄
  输出     : 无
  返回值   : MODEM_TIMEOUT:拨号失败；MODEM_OK:拨号成功
-------------------------------------------------------------*/
int pppd_at_dial(int modem_fd)
{
    char at_dial_str[64] = "ATD";

    /* 从current profile 获取拨号字符串 */
    /* 根据读取的拨号字符串修改at_dial_str */

#ifdef SUPPORT_TD_CARD
    /* BEGIN: Added by y67514, 2009/1/6   PN:ET128*/
    if ( 0 == strlen(at_dialnumber) )
    {
        snprintf(at_dial_str+strlen(at_dial_str), sizeof(at_dial_str), "*99#");
    }
    else
#endif

    {
        snprintf(at_dial_str+strlen(at_dial_str), sizeof(at_dial_str), "%s", at_dialnumber);
    }
    /* END:   Added by y67514, 2009/1/6 */
    at_dial_str[strlen(at_dial_str)] = '\r';
   if (pppd_modem_submit_at(modem_fd, at_dial_str))
    {
        __msleep(500);
       warn("at dial : retrying ...");
        if (pppd_modem_submit_at(modem_fd, at_dial_str))
        {
           warn("at dial : failed");
            return MODEM_TIMEOUT;
        }
    }
   
    return MODEM_OK;
}
/*------------------------------------------------------------
  函数原型 : int pppd_at_apn(int modem_fd)
  描述     : 通过modem口下发AT命令设置APN
  输入     : modem_fd
  输出     : 无
  返回值   : MODEM_TIMEOUT:设置APN失败；MODEM_OK:设置APN成功
-------------------------------------------------------------*/
int pppd_at_apn(int modem_fd)
{
    char pdp_str[100] = "";

#ifdef SUPPORT_TD_CARD
    /* BEGIN: Added by y67514, 2009/1/6   PN:ET128*/
    if ( 0 == strlen(at_setapn) )
    {
        AT_PRINT("you didn't config the apn!");
        /*若无apn信息，则不用配，会使用数据卡本身的值*/
        return MODEM_OK;
    }
    /* END:   Added by y67514, 2009/1/6 */
#endif


snprintf(pdp_str,sizeof(pdp_str), "at+cgdcont=1,\"ip\",\"%s\"\r", at_setapn);
 if(pppd_modem_submit_at(modem_fd, pdp_str))
    {
        __msleep(500);
        warn("pdp setting : retrying ...");
        if (pppd_modem_submit_at(modem_fd, pdp_str))
        {
            warn("pdp setting : failed");
            return MODEM_TIMEOUT;
        }
    }   

    
    return MODEM_OK;
}



/*------------------------------------------------------------
  函数原型 : int ppp_modem_submit_at(char* at_dial_str)
  描述     : 通过modem口拨号
  输入     : at_dial_str:下发的拨号字符串, modem_fd: modem口文件句柄
  输出     : 无
  返回值   : MODEM_TIMEOUT : 表示失败; MODEM_OK : 表示成功
-------------------------------------------------------------*/
int pppd_modem_submit_at(int modem_fd, char* at_dial_str)
{
#define LOOP_TIMES 5
    int    ii = LOOP_TIMES;
    int    i_len;
    int    ret;
    
    char   ch[512], *pch;    // 用来接收数据的缓冲区，大小为512字节
    const char *match_msg = NULL;          // 上报匹配字符串
    const char *match_err_msg = NULL;
    fd_set read_set;
    struct timeval tvSelect;

#ifdef SUPPORT_TD_CARD
    if ( NULL == at_dial_str )
    {
        AT_ERRO("at_dial_str is NULL!\n");
        return MODEM_TIMEOUT;
    }
    AT_DEBUG("write AT:\n{%s}\n",at_dial_str);
#endif

    if ( NULL == at_dial_str )
    {
        AT_ERRO("at_dial_str is NULL!\n");
        return MODEM_TIMEOUT;
    }
    
    if(strstr(at_dial_str,"at+cgdcont=1,\"ip\""))
    {
        match_msg = "\r\nOK\r\n";
        match_err_msg = "ERROR";
    }
  
    else if (strstr(at_dial_str, "ATH\r"))
    {
        match_msg = "\r\nOK\r\n";
#ifdef SUPPORT_TD_CARD
        match_err_msg = "ERROR";
#else
        match_err_msg = "NO CARRIER";
#endif

    }
    else if(strstr(at_dial_str,"ATE0\r"))
    {
        match_msg="\r\nOK\r\n";
        /* BEGIN: Added by y67514, 2008/12/20   PN:ET128*/
#ifdef SUPPORT_TD_CARD
        match_err_msg = "ERROR";
#else
        match_err_msg = "NO CARRIER";
#endif
        /* END:   Added by y67514, 2008/12/20 */
    }
    /* BEGIN: Added by y67514, 2008/12/26   PN:ET128*/
    //设定签约速率
    else if(strstr(at_dial_str,"at+cgeqreq=1,2,128,2048"))
    {
         match_msg="OK";
         match_err_msg="ERROR";
    }
    else if(strstr(at_dial_str,"AT^CURC=0,0"))
    {
        match_msg="\r\nOK\r\n";
        match_err_msg = "ERROR";
    }
    /* END:   Added by y67514, 2008/12/26 */
    else 
    {
        //AT_ERRO("unknow AT:\n{%s}\n",at_dial_str);
#ifdef SUPPORT_TD_CARD
        /* BEGIN: Modified by y67514, 2008/12/29   问题单号:ET128*/
        match_msg = "CONNECT";
        /* END:   Modified by y67514, 2008/12/29 */
#else
        /*Start of modified by l00135113 for HSPA support*/
        #if 0
        match_msg = "\r\nCONNECT\r\n";
        #else
        match_msg = "CONNECT";
        #endif
        /*End of modified by l00135113 for HSPA support*/

#endif
        match_err_msg = "NO CARRIER";
    }
    i_len = strlen(at_dial_str);

    while (ii)
    {
        ret = write(modem_fd, at_dial_str, i_len);
        if (-1 == ret)
        {
            if (EINTR == errno)
            {
                continue;
            }
           error("modem : write AT dial command(%s) error", at_dial_str);
            return MODEM_TIMEOUT;
        }
        ii--;
        if (ret != i_len)
        {
            if (0 == ii) // 如果下发5次都失败
            {
               error("modem : submit AT dial command(%s) failed\n", at_dial_str);
                return MODEM_TIMEOUT;
            }
            warn("modem : have not submit whole at command for %d times", ii);
            continue;
        }
        break;
    }
 

#ifdef SUPPORT_TD_CARD
    tvSelect.tv_sec  = 15; // 最长等待15秒
#else
    tvSelect.tv_sec  = 20; // 最长等待15秒
#endif

    tvSelect.tv_usec = 0;

    pch = &ch[0];
    
/* <AP7D01306.PPP: yangjianping 2008-3-13 MODIFY BEGIN */
    while (1)
    {
        if (1 == hungup || !persist || 1 == kill_link)
        {
            return MODEM_TIMEOUT;
        }
      
        while (1)
        {
            FD_ZERO(&read_set);
            FD_SET(modem_fd, &read_set);

            ret = select(modem_fd + 1, &read_set, NULL, NULL, &tvSelect);
            if (-1 == ret)
            {

                if (EINTR == errno)
                {
                    if (1 == hungup || !persist || 1 == kill_link)
                    {
                        return MODEM_TIMEOUT;
                    }

                    continue;
                }
                error("modem : select modem error");
                return MODEM_TIMEOUT;
            }
            else if (!ret)
            {
            
/* <D00010.PPP: chenjindong 2007-4-10 MODIFY BEGIN */
                //ERROR("modem : wait for report msg timeout!");
/* D00010.PPP: chenjindong 2007-4-10 MODIFY END> */
                return MODEM_TIMEOUT;
            }
            break;
        }
  
      info("select time is : sec = %d, ms = %d, us = %d , hungup <%d>", 
            tvSelect.tv_sec, (tvSelect.tv_usec / 1000), (tvSelect.tv_usec % 1000), hungup);

        while (1)
        {
/*Start of ATP 2009-3-5 for AU4D01428 by g00121640: 无线上行拨号时，在拨号失败的条件下，cpu占用率达到97% */
            memset(ch,0,512);
            //alarm(2);
            ret = read(modem_fd, ch, 64); // 每次最多读出64字节
/*End of ATP 2009-3-5 for AU4D01428 by g00121640 */
		   // alarm(0);
// printf("\nthe results appear at %s,%s,%d,the ret of read from modem is :%d\n",__FILE__,__FUNCTION__,__LINE__,ret);
            if (-1 == ret)
            {
                if (EINTR == errno)
                {
                    if (1 == hungup || !persist || 1 == kill_link)
                    {
                        error("Read usb modem_fd error,hungup = %d persist = %d kill_link = %d\n", hungup, persist, kill_link);
                        return MODEM_TIMEOUT;
                    }
                    continue;
                }
                error("read modem_fd error\n");
                return MODEM_TIMEOUT;
            }
            break;
        }
       
        if (1 == hungup || !persist || 1 == kill_link)
        {
            return MODEM_TIMEOUT;
        }
/*Start of ATP 2009-3-5 for AU4D01428 by g00121640: 无线上行拨号时，在拨号失败的条件下，cpu占用率达到97% */
#if 0
#ifdef SUPPORT_TD_CARD
        if ( 0 == ret )
        {
            AT_ERRO("response is NULL\n");
            return MODEM_TIMEOUT;
        }
#endif
#else
        if ( 0 == ret )
        {
            AT_ERRO("response is NULL\n");
            return MODEM_TIMEOUT;
        }
#endif
/*End of ATP 2009-3-5 for AU4D01428 by g00121640 */

/*Start of ATP 2009-3-5 for AU4D01428 by g00121640: 无线上行拨号时，在拨号失败的条件下，cpu占用率达到97% */

#if 0
        pch[ret] = '\0';
#endif
/*Start of ATP 2009-3-5 for AU4D01428 by g00121640: 无线上行拨号时，在拨号失败的条件下，cpu占用率达到97% */
        pch += ret;
        if (strstr(ch, match_msg))             /* 如果上报了正确匹配字符串 */
        {
       
            info("return MODEM_OK of match_msg");
            return MODEM_OK;
        }
        else if (strstr(ch, match_err_msg))  /* 如果上报了错误匹配字符串 */
        {
            info("return MODEM_OK of match_err_msg");
            return MODEM_TIMEOUT;
        }
        else
        {
            /* BEGIN: Modified by y67514, 2009/1/5   问题单号:ET128*/
            if (pch > &ch[512-64]) 
            {
            info("return no match message at all !!");
                return MODEM_TIMEOUT;
            }
            /* END:   Modified by y67514, 2009/1/5 */
        }
    }
/* AP7D01306.PPP: yangjianping 2008-3-13 MODIFY END> */
}

/*------------------------------------------------------------
  函数原型 : int pppd_ATE(int modem_fd)
  描述     : 通过modem关闭回显功能
  输入     : modem_fd:modem口文件句柄
  输出     : 无
  返回值   : MODEM_TIMEOUT:拨号失败；MODEM_OK:拨号成功
-------------------------------------------------------------*/

int pppd_ate(int modem_fd)
{
    if (pppd_modem_submit_at(modem_fd, "ATE0\r")) 
    {
        __msleep(500);
        //AT_ERRO("ate0 setting fail first: retrying ...\n");
        if (pppd_modem_submit_at(modem_fd, "ATE0\r"))
        {
            AT_ERRO("ate0 setting fail second: Failed,Timeout\n");
            return MODEM_TIMEOUT;
        }
    }
    return MODEM_OK;
}


/*------------------------------------------------------------
  函数原型 : int pppd_ath(int modem_fd)
  描述     : AT拨号挂起连接
  输入     : modem_fd:modem口文件句柄
  输出     : 无
  返回值   : MODEM_TIMEOUT:拨号失败；MODEM_OK:拨号成功
-------------------------------------------------------------*/
int pppd_ath(int modem_fd)
{
    //__msleep(500);
    if (pppd_modem_submit_at(modem_fd, "ATH\r")) 
    {
        __msleep(500);
        //AT_ERRO("ATH set erro!\n");
        if (pppd_modem_submit_at(modem_fd, "ATH\r"))
        {
            //AT_ERRO("ATH set erro!\n");
            return MODEM_TIMEOUT;
        }
    }
    return MODEM_OK;
}

#ifdef SUPPORT_TD_CARD
/* BEGIN: Added by y67514, 2008/12/26   PN:ET128*/
/*****************************************************************************
 函 数 名  : pppd_at_curc
 功能描述  : 禁掉主动上报
 输入参数  : int modem_fd
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2008年12月26日
    作    者   : y67514
    修改内容   : 新生成函数

*****************************************************************************/
int pppd_at_curc( int modem_fd )
{
    if (pppd_modem_submit_at(modem_fd, "AT^CURC=0,0\r")) 
    {
        AT_ERRO("CURC set erro!\n");
        return MODEM_TIMEOUT;
    }
    return MODEM_OK;
}

/*------------------------------------------------------------
  函数原型 : int pppd_at_cgeqreq(int modem_fd)
  描述     : 通过modem口下发AT命令设置数据卡的签约速率
  输入     : modem_fd
  输出     : 无
  返回值   : MODEM_TIMEOUT:设置APN失败；MODEM_OK:设置APN成功
-------------------------------------------------------------*/
int pppd_at_cgeqreq(int modem_fd)
{
    char cgeqreq_str[30] = "at+cgeqreq=1,2,128,2048\r";
    info("pppd_at_cgeqreq");
    __msleep(500);
    if(pppd_modem_submit_at(modem_fd, cgeqreq_str))
    {
        __msleep(500);
        AT_ERRO("cgeqreq setting fail first: retrying ...\n");
        if (pppd_modem_submit_at(modem_fd, cgeqreq_str))
        {
            AT_ERRO("cgeqreq setting fail second: Failed,Timeout\n");
            return MODEM_TIMEOUT;
        }
    }   

    return MODEM_OK;

}
/* END:   Added by y67514, 2008/12/26 */
#endif

/*------------------------------------------------------------
  函数原型 : int __msleep(int millisecond )
  描述     : 实现以毫秒为单位的不被唤醒的等待
  输入     : millisecond(睡眠时间)
  输出     : 无
  返回值   : 0: 成功
-------------------------------------------------------------*/
int __msleep(int millisecond) 
{
    int ret;
    struct timeval tvSelect;
    
    tvSelect.tv_sec  = millisecond / 1000;
    tvSelect.tv_usec = (millisecond % 1000) * 1000;

    while (1)
    {
        ret = select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0,
                    (struct timeval*)&tvSelect);
        if (ret < 0)
        {
            if (EINTR == errno)
            {
                continue;
            }
            //_ERROR(AT_SHARE_LOG,"msleep : select error \n");
        }
        break;
    }    

    return 0;
}

/*------------------------------------------------------------
  函数原型 : int ppp_at_tty(int modem_fd)
  描述     : 建立tty连接
  输入     : modem_fd:modem口文件句柄
  输出     : 无
  返回值   : 0:拨号失败；1:拨号成功
-------------------------------------------------------------*/
int ppp_at_tty(int ttyfd)
{
#ifdef SUPPORT_TD_CARD
    /* BEGIN: Added by y67514, 2008/12/26   PN:ET128*/
    /*禁掉modem口的主动上报*/

    if(pppd_at_curc(ttyfd))
    {
        return(0);
    }
    /* END:   Added by y67514, 2008/12/26 */
#endif

        if(pppd_ate(ttyfd))
        {
        return(0);
		}
#ifdef SUPPORT_TD_CARD
    pppd_ath(ttyfd);
#endif
        if (pppd_at_apn(ttyfd))
        {
            info("Setting APN failed!");
            return(0);
        }
        if (pppd_at_dial(ttyfd)) 
        {

            info("AT dial failed");
            if (pppd_ath(ttyfd)) 
            {

                if (!persist) 
                {
                    info("Serial link disconnected.");
                    return 0;
                }
                
                if (pppd_ath(ttyfd)) 
                {
                    warn("disconnect script failed");
                } 
                else 
                {
                    info("Serial link disconnected.");
                }
                
            } 
            else 
            {
                info("Serial link disconnected.");
            }
            return 0;
        }
		return 1;
}

/*------------------------------------------------------------
  函数原型 : void auth_option_initialize()
  描述     : 选择认证方式
  输入     : 无
  输出     : 无
  返回值   : 无
-------------------------------------------------------------*/
void auth_option_initialize()
{
switch(auth_option)
    {
        case 0:
            lcp_wantoptions[0].neg_upap = 0;
            lcp_wantoptions[0].neg_chap = 0;
            lcp_allowoptions[0].neg_upap = 1;
            lcp_allowoptions[0].neg_chap = 1;
            break;
        case 1:
            lcp_wantoptions[0].neg_upap = 0;
            lcp_wantoptions[0].neg_chap = 0;
            lcp_allowoptions[0].neg_upap = 1;
            lcp_allowoptions[0].neg_chap = 0;
            break;
        case 2:
            lcp_wantoptions[0].neg_upap = 0;
            lcp_wantoptions[0].neg_chap = 0;
            lcp_allowoptions[0].neg_upap = 0;
            lcp_allowoptions[0].neg_chap = 1;
            break;
        default:
            lcp_wantoptions[0].neg_upap = 0;
            lcp_wantoptions[0].neg_chap = 0;
            lcp_allowoptions[0].neg_upap = 1;
            lcp_allowoptions[0].neg_chap = 1;
            break;
    }

    
    return;
}

static void
log_write_new(logfile, message)
    char* logfile;
    char *message;
{
    static int log_pos = 0;

    FILE  *fp;

    log_pos += strlen(message);

    if (log_pos > LOG_SIZE) 
    {
        fp =  fopen(logfile, "w");
        log_pos = 0;
    }
    else
    {
        fp =  fopen(logfile, "a+");
    }

    if(fp)
    {
        fputs(message, fp);
        fclose ( fp );
    }
}


static char *log_level[] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "HIGH",
    "ERR",
    "FATAL",
};

void  _syslog(char* logfile, int level, char *format, ...)
{
    if (level < 0 || level > 5)
    {
        return ;
    }
    va_list args;
    char message[MAX_MSG_LEN + 2];
    int flen, mlen;
    if (level >= 0)//atoi(nvram_safe_get("sys_log_level"))
    {
        flen = snprintf(message, MAX_MSG_LEN, "[%s] ", log_level[level]);
        if (flen > 0)
        {
            va_start(args, format);
            mlen = vsnprintf(&message[flen], MAX_MSG_LEN - flen, format, args);
            if (mlen > 0 && message[flen+mlen-1] != '\n')
            {
                message[flen+mlen] = '\n';
                message[flen+mlen+1] = 0;
            }
            va_end (args);
            log_write_new(logfile, message);
        }
    }
    return ;
}

void notifywanstatus()
{
// network/wan dir not used in hybrid , ATP_MSG_WANCORE_WANCONN_CHANGE this message not registered        
//    ATP_MSG_SendMsgData(ATP_CBBID_CMS_NAME, ATP_MSG_WANCORE_WANCONN_CHANGE, NULL, 0);
}

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
/*for W724v，在V4或V6拨号up后，还能随时进行另一协议拨号*/
void DT_RedialAfterNpUp(int protocol)
{
    fsm *f;

    //只处理DT IPCP/IPV6CP协商
    if (protocol != PPP_IP && protocol != PPP_IPV6)
    {
        return;
    }
    //always-on 时ipv6直接反复重试，不需报文触发
    if (!demand && PPP_IPV6 == protocol)
    {
        return; 
    }
    f = (PPP_IP == protocol)?&ipcp_fsm[0] : &ipv6cp_fsm[0]; 
    if (!f->ncp_enable || (STAGE_WAITING_TRIGGER != f->dt_ncp_stage))
    {
        printf("ncp_enable false or not dt_ncp_stage not waiting\n");
        return;
    }
    //v6只触发v6, v4同时触发v4和v6
    if (PPP_IP == protocol)
    {
        g_iEnableV4Dial = 1;
        g_iEnableV6Dial = 1;
    }
    else
    {
        g_iEnableV6Dial = 1;
    }
    printf("call fsm_open(), recv protocol %s while wait for trigger\n", (PPP_IP == protocol)?"IPv4" : "IPv6");
    f->state = CLOSED;
    fsm_open(f);
    ++num_np_open;
    printf("%s %d, num_np_open+1,  now=%d\n", __func__, __LINE__, num_np_open);

    //on-demand时，v4报文还需触发v6拨号(若此时ipv6cp还没开始协商)
    if (demand && (PPP_IP == protocol) && (STAGE_WAITING_TRIGGER == ipv6cp_fsm[0].dt_ncp_stage))
    {
        printf("on-demand mode, V4 trigger V6 dialing\n");
        ipv6cp_fsm[0].state = CLOSED;
        fsm_open(&ipv6cp_fsm[0]);
        ++num_np_open;
        printf("%s %d, num_np_open+1,  now=%d\n", __func__, __LINE__, num_np_open);
    }
    
}
#endif

//重置计数器
void DT_ResetRetryCounter(int proto)
{
    fsm *f;
    
     if (PPP_IP == proto)
     {
        f = &ipcp_fsm[0];   
     }
     else if (PPP_IPV6 == proto)
     {
        f = &ipv6cp_fsm[0];
     }
     else
     {
        return;
     }
     f ->dt_ncp_stage = STAGE_WAITING_TRIGGER;
     f->burst_counter = 0;
     f->penalty_counter =0;
     f->retry_interval  =1;
}
#endif

