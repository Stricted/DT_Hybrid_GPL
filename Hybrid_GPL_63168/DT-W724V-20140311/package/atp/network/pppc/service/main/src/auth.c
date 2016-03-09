/*
 * auth.c - PPP authentication and phase control.
 *
 * Copyright (c) 1993 The Australian National University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the Australian National University.  The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

#if defined(_PATH_LASTLOG) && defined(_linux_)
#include <lastlog.h>
#endif

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef USE_PAM
#include <security/pam_appl.h>
#endif

#ifdef HAS_SHADOW
#include <shadow.h>
#ifndef PW_PPP
#define PW_PPP PW_LOGIN
#endif
#endif

#include "pppd.h"
#include "fsm.h"
#include "lcp.h"
#include "ipcp.h"
#include "upap.h"
#include "chap.h"
#ifdef CBCP_SUPPORT
#include "cbcp.h"
#endif
#include "pathnames.h"
#include "atplog.h"

#ifdef SUPPORT_ATP_DDNS
#include "ddnscmsapi.h"
#endif

#include "natcmsapi.h"

#ifdef SUPPORT_ATP_FON
#include "foncmsapi.h"
#endif

#ifdef SUPPORT_ATP_HYBRID_BONDING
#include "ltecmsapi.h"
#endif


#define PPP_CHECK_STATUS_TIMEOUT 2

#define ErrCodeFilePath "/var/InternetDiagnose"
int baud_rate;
static const char rcsid[] = RCSID;

/* Bits in scan_authfile return value */
#define NONWILD_SERVER	1
#define NONWILD_CLIENT	2

#define ISWILD(word)	(word[0] == '*' && word[1] == 0)

/* The name by which the peer authenticated itself to us. */
char peer_authname[MAXNAMELEN];

/* Records which authentication operations haven't completed yet. */
static int auth_pending[NUM_PPP];

/* Set if we have successfully called plogin() */
static int logged_in;

/* List of addresses which the peer may use. */
static struct permitted_ip *addresses[NUM_PPP];

/* Wordlist giving addresses which the peer may use
   without authenticating itself. */
static struct wordlist *noauth_addrs;

/* Extra options to apply, from the secrets file entry for the peer. */
static struct wordlist *extra_options;

/* Number of network protocols which we have opened. */
int num_np_open;

/* Number of network protocols which have come up. */
int num_np_up;

/* Set if we got the contents of passwd[] from the pap-secrets file. */
static int passwd_from_file;

/* Set if we require authentication only because we have a default route. */
static bool default_auth;

/* Hook to enable a plugin to control the idle time limit */
int (*idle_time_hook) __P((struct ppp_idle *)) = NULL;

/* Hook for a plugin to say whether we can possibly authenticate any peer */
int (*pap_check_hook) __P((void)) = NULL;

/* Hook for a plugin to check the PAP user and password */
int (*pap_auth_hook) __P((char *user, char *passwd, char **msgp,
			  struct wordlist **paddrs,
			  struct wordlist **popts)) = NULL;

/* Hook for a plugin to know about the PAP user logout */
void (*pap_logout_hook) __P((void)) = NULL;

/* Hook for a plugin to get the PAP password for authenticating us */
int (*pap_passwd_hook) __P((char *user, char *passwd)) = NULL;

/*
 * This is used to ensure that we don't start an auth-up/down
 * script while one is already running.
 */
enum script_state {
    s_down,
    s_up
};

static enum script_state auth_state = s_down;
static enum script_state auth_script_state = s_down;
static pid_t auth_script_pid = 0;

static int used_login;		/* peer authenticated against login database */

/*
 * Option variables.
 */
bool uselogin = 0;		/* Use /etc/passwd for checking PAP */
bool cryptpap = 0;		/* Passwords in pap-secrets are encrypted */
bool refuse_pap = 0;		/* Don't wanna auth. ourselves with PAP */
bool refuse_chap = 0;		/* Don't wanna auth. ourselves with CHAP */
bool usehostname = 0;		/* Use hostname for our_name */
bool auth_required = 0;		/* Always require authentication from peer */
bool allow_any_ip = 0;		/* Allow peer to use any IP address */
bool explicit_remote = 0;	/* User specified explicit remote name */
char remote_name[MAXNAMELEN];	/* Peer's name for authentication */

static char *uafname;		/* name of most recent +ua file */

/* Bits in auth_pending[] */
#define PAP_WITHPEER	1
#define PAP_PEER	2
#define CHAP_WITHPEER	4
#define CHAP_PEER	8

#if defined(SUPPORT_ATP_PPP_DYN_DIAL) && defined(SUPPORT_ATP_PPP_DT_TOCPE0056)
//W724v
//always-on的v4超时时间为sip注册间隔加5分钟(300秒)
#define ALWAYSON_MODE_V4_IDLE_PLUS   300
#define SIP_INTERVAL_DEFAULT   900

static int get_always_on_v4_idle(void);
static void check_v4_idle (void);
static char ac_parse_bcm_ip[32]="";
struct timeval last_hw_v4_xmit;
#define BCM_FCACHE_FILE   "/proc/fcache/nflist"
#define FCACHE_FILE_TO_PARSE   "/var/nflist"
#endif

#ifdef SUPPORT_ATP_PPP_IGMP_DIAL
/*START MODIFY:l00180987 2012-10-6 FOR DTS2012082503139*/
static time_t ul_fcache_ipv4_idle = 0xFFFF;

//每30秒查一次硬加速/proc/fcache/状态文件，看是否有IPv4的加速流在跑，因为ppp内核driver统计不到硬加速状态
#define CHECK_BCM_V4_HW_FLOW_INTERVAL  30
#define FCACHE_BR_FILE_TO_PARSE   "/var/brlist"
#define BCM_FCACHE_BR_FILE   "/proc/fcache/brlist"
#define FCACHE_FILE_TITLE_LINE_NUM    2
#define FCACHE_FLOWOBJECT_SIZE 11
#define PPP_IGMP_MC_FILE  "/proc/sys/net/ipv4/conf/"W724V_PPP_NAME"/mc_forwarding"

struct timeval last_hw_v4_br_xmit;

struct br_hit_list_st
{
    struct br_hit_list_st *next;
    char flow_object[FCACHE_FLOWOBJECT_SIZE];
    long hit_num;
    int delete_flag;
};

/******************************************************************************
  变量名称  : g_br_hit_list_head
  变量作用  : brlist 计数记录链表
  取值范围  : 
  调用函数  : 

  注意事项  :记录硬加速hit 非0表项
******************************************************************************/
static struct br_hit_list_st * g_br_hit_list_head = NULL;
/*END MODIFY:l00180987 2012-10-6 FOR DTS2012082503139*/
static void check_bcm_v4_hw_cache_idle(void);

#ifdef SUPPORT_ATP_MLD
/*ipv6组播统计 begin  /proc/net/ip6_mr_cache*/
static time_t ul_ip6mc_idle = 0xFFFF;
/*每30秒查一次ip6 组播统计文件proc/net/ip6_mr_cache*/
#define CHECK_V6_MC_INTERVAL  30
#define IP6_MC_FILE_TO_PARSE   "/var/ip6mr_cache"
#define IP6_MC_PROC_FILE   "/proc/net/ip6_mr_cache"
#define IP6_MC_TITLE_LINE_NUM    1
#define IP6_GROUP_ADDR_SIZE 40
#define PPP_IP6_MC_FILE  "/proc/sys/net/ipv6/conf/"W724V_PPP_NAME"/mc_forwarding"

struct timeval last_v6_mc_xmit;
struct mc6_list_st
{
    struct mc6_list_st *next;
    char group[IP6_GROUP_ADDR_SIZE];
    long pkts;
    int delete_flag;
};
/******************************************************************************
  变量名称  : g_mc6_list_head
  变量作用  : 内核的ipov6 组播计数记录链表
  取值范围  : 
  调用函数  : 
  注意事项  :记录bcm没加速的ipv6组播非0表项
******************************************************************************/
static struct mc6_list_st * g_mc6_list_head = NULL;
static void check_mc6_cache_idle(void);
/*ipv6组播统计 end*/
#endif
#endif

static int is_ppp_can_down(void);

#ifdef SUPPORT_ATP_VOICE
extern unsigned int ATP_UTIL_VoiceGetVoipStatus(void);
extern unsigned int ATP_UTIL_VoiceGetSipInterval(void);
extern unsigned int ATP_UTIL_VoiceHasUrgCall(void);
#endif

extern char *crypt __P((const char *, const char *));

/* Prototypes for procedures local to this file. */

static void network_phase __P((int));
static void check_idle __P((void *));

static void check_status __P((void *));
static void connect_time_expired __P((void *));
static int  plogin __P((char *, char *, char **));
static void plogout __P((void));
static int  null_login __P((int));
static int  get_pap_passwd __P((char *));
static int  have_pap_secret __P((int *));
static int  have_chap_secret __P((char *, char *, int, int *));
static int  ip_addr_check __P((u_int32_t, struct permitted_ip *));
static int  scan_authfile __P((FILE *, char *, char *, char *,
			       struct wordlist **, struct wordlist **,
			       char *));
static void free_wordlist __P((struct wordlist *));
static void auth_script __P((char *));
static void auth_script_done __P((void *));
static void set_allowed_addrs __P((int, struct wordlist *, struct wordlist *));
static int  some_ip_ok __P((struct wordlist *));
static int  setupapfile __P((char **));
static int  privgroup __P((char **));
static int  set_noauth_addr __P((char **));
static void check_access __P((FILE *, char *));
static int  wordlist_count __P((struct wordlist *));
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
#ifdef SUPPORT_ATP_PPPD
void pppx_check_user(void * arg);
#endif
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/

/*
 * Authentication-related options.
 */
option_t auth_options[] = {
    { "auth", o_bool, &auth_required,
      "Require authentication from peer", OPT_PRIO | 1 },
    { "noauth", o_bool, &auth_required,
      "Don't require peer to authenticate", OPT_PRIOSUB | OPT_PRIV,
      &allow_any_ip },
    { "require-pap", o_bool, &lcp_wantoptions[0].neg_upap,
      "Require PAP authentication from peer",
      OPT_PRIOSUB | 1, &auth_required },
    { "+pap", o_bool, &lcp_wantoptions[0].neg_upap,
      "Require PAP authentication from peer",
      OPT_ALIAS | OPT_PRIOSUB | 1, &auth_required },
    { "require-chap", o_bool, &lcp_wantoptions[0].neg_chap,
      "Require CHAP authentication from peer",
      OPT_PRIOSUB | 1, &auth_required },
    { "+chap", o_bool, &lcp_wantoptions[0].neg_chap,
      "Require CHAP authentication from peer",
      OPT_ALIAS | OPT_PRIOSUB | 1, &auth_required },

    { "refuse-pap", o_bool, &refuse_pap,
      "Don't agree to auth to peer with PAP", 1 },
    { "-pap", o_bool, &refuse_pap,
      "Don't allow PAP authentication with peer", OPT_ALIAS | 1 },

    { "refuse-chap", o_bool, &refuse_chap,
      "Don't agree to auth to peer with CHAP", 1 },
    { "-chap", o_bool, &refuse_chap,
      "Don't allow CHAP authentication with peer", OPT_ALIAS | 1 },

    { "name", o_string, our_name,
      "Set local name for authentication",
      OPT_PRIO | OPT_PRIV | OPT_STATIC, NULL, MAXNAMELEN },

    { "+ua", o_special, (void *)setupapfile,
      "Get PAP user and password from file",
      OPT_PRIO | OPT_A2STRVAL, &uafname },

    { "user", o_string, user,
      "Set name for auth with peer", OPT_PRIO | OPT_STATIC, NULL, MAXNAMELEN },

    { "password", o_string, passwd,
      "Password for authenticating us to the peer",
      OPT_PRIO | OPT_STATIC | OPT_HIDE, NULL, MAXSECRETLEN },

    { "usehostname", o_bool, &usehostname,
      "Must use hostname for authentication", 1 },

    { "remotename", o_string, remote_name,
      "Set remote name for authentication", OPT_PRIO | OPT_STATIC,
      &explicit_remote, MAXNAMELEN },

    { "login", o_bool, &uselogin,
      "Use system password database for PAP", 1 },

    { "papcrypt", o_bool, &cryptpap,
      "PAP passwords are encrypted", 1 },

    { "privgroup", o_special, (void *)privgroup,
      "Allow group members to use privileged options", OPT_PRIV | OPT_A2LIST },

    { "allow-ip", o_special, (void *)set_noauth_addr,
      "Set IP address(es) which can be used without authentication",
      OPT_PRIV | OPT_A2LIST },

    { NULL }
};


/*
 * setupapfile - specifies UPAP info for authenticating with peer.
 */
static int
setupapfile(argv)
    char **argv;
{
    FILE *ufile;
    int l;
    char u[MAXNAMELEN], p[MAXSECRETLEN];
    char *fname;

    lcp_allowoptions[0].neg_upap = 1;

    /* open user info file */
    fname = strdup(*argv);
    if (fname == NULL)
	novm("+ua file name");
    seteuid(getuid());
    ufile = fopen(fname, "r");
    seteuid(0);
    if (ufile == NULL) {
	option_error("unable to open user login data file %s", fname);
	return 0;
    }
    check_access(ufile, fname);
    uafname = fname;

    /* get username */
    if (fgets(u, MAXNAMELEN - 1, ufile) == NULL
	|| fgets(p, MAXSECRETLEN - 1, ufile) == NULL){
	option_error("unable to read user login data file %s", fname);
	return 0;
    }
    fclose(ufile);

    /* get rid of newlines */
    l = strlen(u);
    if (l > 0 && u[l-1] == '\n')
	u[l-1] = 0;
    l = strlen(p);
    if (l > 0 && p[l-1] == '\n')
	p[l-1] = 0;

    if (override_value("user", option_priority, fname))
	strlcpy(user, u, sizeof(user));
    if (override_value("passwd", option_priority, fname))
	strlcpy(passwd, p, sizeof(passwd));

    return (1);
}


/*
 * privgroup - allow members of the group to have privileged access.
 */
static int
privgroup(argv)
    char **argv;
{
    struct group *g;
    int i;

    g = getgrnam(*argv);
    if (g == 0) {
	option_error("group %s is unknown", *argv);
	return 0;
    }
    for (i = 0; i < ngroups; ++i) {
	if (groups[i] == g->gr_gid) {
	    privileged = 1;
	    break;
	}
    }
    return 1;
}


/*
 * set_noauth_addr - set address(es) that can be used without authentication.
 * Equivalent to specifying an entry like `"" * "" addr' in pap-secrets.
 */
static int
set_noauth_addr(argv)
    char **argv;
{
    char *addr = *argv;
    int l = strlen(addr) + 1;
    struct wordlist *wp;

    wp = (struct wordlist *) malloc(sizeof(struct wordlist) + l);
    if (wp == NULL)
	novm("allow-ip argument");
    wp->word = (char *) (wp + 1);
    wp->next = noauth_addrs;
    BCOPY(addr, wp->word, l);
    noauth_addrs = wp;
    return 1;
}


/*
 * An Open on LCP has requested a change from Dead to Establish phase.
 * Do what's necessary to bring the physical layer up.
 */
void
link_required(unit)
    int unit;
{
}

/*
 * LCP has terminated the link; go to the Dead phase and take the
 * physical layer down.
 */
void
link_terminated(unit)
    int unit;
{
    if (phase == PHASE_DEAD)
	return;
    if (pap_logout_hook) {
	pap_logout_hook();
    } else {
	if (logged_in)
	    plogout();
    }
    new_phase(PHASE_DEAD);
    notice("Connection terminated.");
}

/*
 * LCP has gone down; it will either die or try to re-establish.
 */
void
link_down(unit)
    int unit;
{
    int i;
    struct protent *protp;

    auth_state = s_down;
    if (auth_script_state == s_up && auth_script_pid == 0) {
	update_link_stats(unit);
	auth_script_state = s_down;
	auth_script(_PATH_AUTHDOWN);
    }
    for (i = 0; (protp = protocols[i]) != NULL; ++i) {
	if (!protp->enabled_flag)
	    continue;
        if (protp->protocol != PPP_LCP && protp->lowerdown != NULL)
	    (*protp->lowerdown)(unit);
        if (protp->protocol < 0xC000 && protp->close != NULL)
        {
    	    (*protp->close)(unit, "LCP down");
        }
    }
    num_np_open = 0;
    num_np_up = 0;
    if (phase != PHASE_DEAD)
	new_phase(PHASE_TERMINATE);
}

/*
 * The link is established.
 * Proceed to the Dead, Authenticate or Network phase as appropriate.
 */
void
link_established(unit)
    int unit;
{
    int auth;
    FILE *fp = NULL;//打开认证方式的文件
    char path[128] = "";
    lcp_options *wo = &lcp_wantoptions[unit];
    lcp_options *go = &lcp_gotoptions[unit];
    lcp_options *ho = &lcp_hisoptions[unit];
    int i;
    struct protent *protp;

    /*
     * Tell higher-level protocols that LCP is up.
     */
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
        if (protp->protocol != PPP_LCP && protp->enabled_flag
	    && protp->lowerup != NULL)
	    (*protp->lowerup)(unit);

    if (auth_required && !(go->neg_chap || go->neg_upap)) {
	/*
	 * We wanted the peer to authenticate itself, and it refused:
	 * if we have some address(es) it can use without auth, fine,
	 * otherwise treat it as though it authenticated with PAP using
	 * a username * of "" and a password of "".  If that's not OK,
	 * boot it out.
	 */
	if (noauth_addrs != NULL) {
	    set_allowed_addrs(unit, NULL, NULL);
	} else if (!wo->neg_upap || uselogin || !null_login(unit)) {
	    warn("peer refused to authenticate: terminating link");
        /*add by z67625 start*/
        pppoe_setlasterror("ERROR_AUTHENTICATION_FAILURE");
        /*add by z67625 end*/
	    lcp_close(unit, "peer refused to authenticate");
	    status = EXIT_PEER_AUTH_FAILED;
	    return;
	}
    }
    /*add by z67625 设置当前认证方式 2007-09-03 start*/
    /*Start of ATP 2011-5-4 8:55 for  by h00163136*/
#if 0
    sprintf(path,"/var/wan/%s/auth",wanif_name);
#else
    snprintf(path, sizeof(path), "/var/wan/%s/auth",wanif_name);
#endif
    /*End of ATP 2011-5-4 8:55 by h00163136*/
    if(NULL != (fp = fopen(path,"w")))
    {
        if(go->neg_chap ||ho->neg_chap)
            fputs("CHAP",fp);
        if(go->neg_upap ||ho->neg_upap)
            fputs("PAP",fp);
        fclose(fp);
    }
    else
    {
        printf("open auth file failed!\n");
    }
    /*add by z67625 设置当前认证方式 2007-09-03 end*/
    new_phase(PHASE_AUTHENTICATE);
    /*add by z67625 start*/
    pppoe_setstatus(Authenticating);//设置状态为正在认证
    /*add by z67625 end*/
    used_login = 0;
    auth = 0;
    if (go->neg_chap) {
	ChapAuthPeer(unit, our_name, go->chap_mdtype);
	auth |= CHAP_PEER;
    } else if (go->neg_upap) {
	upap_authpeer(unit);
	auth |= PAP_PEER;
    }
    if (ho->neg_chap) {
	ChapAuthWithPeer(unit, user, ho->chap_mdtype);
	auth |= CHAP_WITHPEER;
    } else if (ho->neg_upap) {
	if (passwd[0] == 0) {
	    passwd_from_file = 1;
	    if (!get_pap_passwd(passwd))
		error("No secret found for PAP login");
	}
	upap_authwithpeer(unit, user, passwd);
	auth |= PAP_WITHPEER;
    }
    auth_pending[unit] = auth;

    if (!auth)
	network_phase(unit);
}

/*
 * Proceed to the network phase.
 */
static void
network_phase(unit)
    int unit;
{
    lcp_options *go = &lcp_gotoptions[unit];

    /*
     * If the peer had to authenticate, run the auth-up script now.
     */
    if (go->neg_chap || go->neg_upap) {
	auth_state = s_up;
	if (auth_script_state == s_down && auth_script_pid == 0) {
	    auth_script_state = s_up;
	    auth_script(_PATH_AUTHUP);
	}
    }

#ifdef CBCP_SUPPORT
    /*
     * If we negotiated callback, do it now.
     */
    if (go->neg_cbcp) {
	new_phase(PHASE_CALLBACK);
	(*cbcp_protent.open)(unit);
	return;
    }
#endif

    start_networks();
}

void
start_networks()
{
    int i;
    struct protent *protp;

    new_phase(PHASE_NETWORK);

#ifdef HAVE_MULTILINK
    if (multilink) {
	if (mp_join_bundle()) {
	    if (updetach && !nodetach)
		detach();
	    return;
	}
    }
#endif /* HAVE_MULTILINK */

#ifdef PPP_FILTER
    if (!demand)
	set_filters(&pass_filter, &active_filter);
#endif
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
        if (protp->protocol < 0xC000 && protp->enabled_flag
	    && protp->open != NULL) {
	    (*protp->open)(0);
	    if (protp->protocol != PPP_CCP)
	    {
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
	     if ((PPP_IPCP==protp->protocol && 1==g_iEnableV4Dial)
	         || (PPP_IPV6CP==protp->protocol && 1==g_iEnableV6Dial))
#else
        if ((PPP_IPCP == protp->protocol) || (PPP_IPV6CP == protp->protocol))
#endif
#endif
		++num_np_open;
	    }
	}

    if (num_np_open == 0)
	/* nothing to do */
    {
        /*add by z67625 start*/
        pppoe_setlasterror("ERROR_UNKNOWN");
        /*add by z67625 end*/
    	lcp_close(0, "No network protocols running");
    }
}

/*
 * The peer has failed to authenticate himself using `protocol'.
 */
void
auth_peer_fail(unit, protocol)
    int unit, protocol;
{
#ifdef SUPPORT_ATP_PPPD
    if(pppx)
        pppx_setwan_state(log_name, PPPX_WAN_AUTH_FAIL);
#endif
    /*
     * Authentication failure: take the link down
     */
     /*add by z67625 start*/
    pppoe_setlasterror("ERROR_AUTHENTICATION_FAILURE");
    /*add by z67625 end*/

    lcp_close(unit, "Authentication failed");
    status = EXIT_PEER_AUTH_FAILED;
}

/*
 * The peer has been successfully authenticated using `protocol'.
 */
void
auth_peer_success(unit, protocol, name, namelen)
    int unit, protocol;
    char *name;
    int namelen;
{
    int bit;

    switch (protocol) {
    case PPP_CHAP:
	bit = CHAP_PEER;
	break;
    case PPP_PAP:
	bit = PAP_PEER;
	break;
    default:
	warn("auth_peer_success: unknown protocol %x", protocol);
	return;
    }

    /*
     * Save the authenticated name of the peer for later.
     */
    if (namelen > sizeof(peer_authname) - 1)
	namelen = sizeof(peer_authname) - 1;
    BCOPY(name, peer_authname, namelen);
    peer_authname[namelen] = 0;
    script_setenv("PEERNAME", peer_authname, 0);

#ifdef SUPPORT_ATP_PPPD
    if(pppx)
        pppx_setwan_state(log_name, PPPX_WAN_AUTH_SUCC);
#endif
    /*add by z67625 end*/
    /*
     * If there is no more authentication still to be done,
     * proceed to the network (or callback) phase.
     */
    if ((auth_pending[unit] &= ~bit) == 0)
        network_phase(unit);
}

/*
 * We have failed to authenticate ourselves to the peer using `protocol'.
 */
void
auth_withpeer_fail(unit, protocol)
    int unit, protocol;
{
    if (passwd_from_file)
	BZERO(passwd, MAXSECRETLEN);
    /*
     * We've failed to authenticate ourselves to our peer.
     * Some servers keep sending CHAP challenges, but there
     * is no point in persisting without any way to get updated
     * authentication secrets.
     */
    /*add by z67625 2007-08-24 start*/
    pppoe_setlasterror("ERROR_AUTHENTICATION_FAILURE");
    /*add by z67625 2007-08-24 end*/
    lcp_close(unit, "Failed to authenticate ourselves to peer");
    status = EXIT_AUTH_TOPEER_FAILED;  
    persist=0;
}

/*
 * We have successfully authenticated ourselves with the peer using `protocol'.
 */
void
auth_withpeer_success(unit, protocol)
    int unit, protocol;
{
    int bit;

    switch (protocol) {
    case PPP_CHAP:
	bit = CHAP_WITHPEER;
	break;
    case PPP_PAP:
	if (passwd_from_file)
	    BZERO(passwd, MAXSECRETLEN);
	bit = PAP_WITHPEER;
	break;
    default:
	warn("auth_withpeer_success: unknown protocol %x", protocol);
	bit = 0;
    }

    /*
     * If there is no more authentication still being done,
     * proceed to the network (or callback) phase.
     */
    if ((auth_pending[unit] &= ~bit) == 0)
	network_phase(unit);
}


/*
 * np_up - a network protocol has come up.
 */
void
np_up(unit, proto)
    int unit, proto;
{
    int tlim;
#if defined(SUPPORT_ATP_PPP_DYN_DIAL) && defined(SUPPORT_ATP_PPP_DT_TOCPE0056)
    sifnpstate(unit, proto, NPSTATE_UP);
    printf("\r\nnp_up for %s\n", (PPP_IP==proto)?"IPCP" : "IPV6CP");
    if (PPP_IP == proto)
    {
        if (demand)
        {
            TIMEOUT(check_v4_idle, NULL, idle_time_limit);
        }

        else
        {
            tlim = get_always_on_v4_idle();
            printf("Setup V4 IdleTimer for Always-On mode, %d seconds\n", tlim);
            TIMEOUT(check_v4_idle, NULL, tlim);
        }

        
    }
#endif

#ifdef SUPPORT_ATP_PPP_IGMP_DIAL 
    if (PPP_IP == proto)
    {
        TIMEOUT(check_bcm_v4_hw_cache_idle, NULL, CHECK_BCM_V4_HW_FLOW_INTERVAL);
    }
#endif
#ifdef SUPPORT_ATP_MLD
    if (PPP_IPV6 == proto)
    {
        TIMEOUT(check_mc6_cache_idle, NULL, CHECK_V6_MC_INTERVAL);
    }
#endif

    
    if (num_np_up == 0) {
	/*
	 * At this point we consider that the link has come up successfully.
	 */
	status = EXIT_OK;
	unsuccess = 0;
	new_phase(PHASE_RUNNING);

	if (idle_time_hook != 0)
	    tlim = (*idle_time_hook)(NULL);
	else
	    tlim = idle_time_limit;
	if (tlim > 0)
	    TIMEOUT(check_idle, NULL, tlim);

	TIMEOUT(check_status, NULL, PPP_CHECK_STATUS_TIMEOUT);
	
#ifdef SUPPORT_ATP_PPPD
    if(pppx)
        TIMEOUT(pppx_check_user, NULL, 3);	
#endif


	/*
	 * Set a timeout to close the connection once the maximum
	 * connect time has expired.
	 */
	if (maxconnect > 0)
	    TIMEOUT(connect_time_expired, 0, maxconnect);

	/*
	 * Detach now, if the updetach option was given.
	 */
	if (updetach && !nodetach)
	    detach();
    }
    ++num_np_up;
}

/*
 * np_down - a network protocol has gone down.
 */
void
np_down(unit, proto)
    int unit, proto;
{
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#ifdef SUPPORT_ATP_DT_TOCPE0056
    sifnpstate(unit, proto, NPSTATE_DOWN);
#endif
    printf("np_down for %s\n", (PPP_IP==proto)?"IPCP" : "IPV6CP");
    if (PPP_IP == proto)
    {
#ifdef SUPPORT_ATP_DT_TOCPE0056
        UNTIMEOUT(check_v4_idle, NULL);
#endif

        UNTIMEOUT(check_bcm_v4_hw_cache_idle, NULL);
        /*ipcp_down里面已经发了down消息*/
    }
    else if (PPP_IPV6 == proto)
    {
        UNTIMEOUT(check_mc6_cache_idle, NULL);
        pppoe_setstatus6(Disconnected);
    }
#endif

    if (--num_np_up == 0) {
	UNTIMEOUT(check_idle, NULL);
	UNTIMEOUT(check_status, NULL);
	new_phase(PHASE_NETWORK);
    }
}

/*
 * np_finished - a network protocol has finished using the link.
 */
void
np_finished(unit, proto)
    int unit, proto;
{
    if (--num_np_open <= 0) {
	/* no further use for the link: shut up shop. */
    /*add by z67625 start*/
    pppoe_setlasterror("ERROR_UNKNOWN");
    /*add by z67625 end*/

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    /*for W724v reset dt ncp counter*/
        DT_ResetRetryCounter(proto);
#endif
	lcp_close(0, "No network protocols running");
    }
}

  
/*****************************************************************************
函 数 名: DT_UTIL_PppConnDown
功能描述: ppp断线通知语音停止注册，2秒钟后ppp断开
         
输入参数: 无
输出参数: 无
返 回 值: 如果需要等待返回TURE，否则返回FALSE
*****************************************************************************/
int DT_UTIL_PppConnDown(unsigned int msgType)
{
#ifdef SUPPORT_ATP_VCESIP
    ATP_MSG_SendSimpleMsg(ATP_CBBID_CMS_NAME, msgType, 0);
#endif
    return 1;
}

void  DT_NotifyConnectionDown(unsigned int msgType)
{
    if(0 != dial_protocol )
    {
        return ;
    }
    
    if (ATP_UTIL_VoiceGetSipInterval() != 0)
    {
        if(1 == DT_UTIL_PppConnDown(msgType))
        {
            sleep(2);
        }
    }
}

extern int goonflag;
/*
 * check_idle - check whether the link has been idle for long
 * enough that we can shut it down.
 */
static void
check_idle(arg)
    void *arg;
{
    struct ppp_idle idle;
#ifdef SUPPORT_ATP_PPP_DEMAND_RCVPACKET
    time_t itime;
#endif
    int tlim;
#ifdef SUPPORT_ATP_PPP_IGMP_DIAL
    int the_idle;
#endif
#ifdef SUPPORT_ATP_FON
    FILE *fp = NULL;
#endif

    if (!get_idle_time(0, &idle))
	return;
    if (idle_time_hook != 0) {
	tlim = idle_time_hook(&idle);
    } else {
#ifdef SUPPORT_ATP_PPP_IGMP_DIAL
    the_idle = MIN(idle.xmit_idle, ul_fcache_ipv4_idle);
/*检查ipv6组播是否有流*/
#ifdef SUPPORT_ATP_MLD
    the_idle = MIN(the_idle, ul_ip6mc_idle);
#endif
    tlim = idle_time_limit - the_idle;
#else
	tlim = idle_time_limit - idle.xmit_idle;
#endif
	//按需拨号不计算接收的流量
	#ifdef SUPPORT_ATP_PPP_DEMAND_RCVPACKET
	itime = MIN(idle.xmit_idle, idle.recv_idle);
	tlim = idle_time_limit - itime;
	#endif
    }
    //有voip通话,紧急呼叫，cwmp在配置,则不能掉线
    if ((tlim <= 0) && is_ppp_can_down())
    {
        DT_NotifyConnectionDown(ATP_MSG_WAN_V4_V6_WILLDOWN);

        /* link is idle: shut it down. */
        notice("Terminating connection due to lack of activity.");
        /*add by z67625 start*/
        pppoe_setlasterror("ERROR_IDLE_DISCONNECT");
#ifdef SUPPORT_ATP_FON
        /*建一个空的标志文件给foncms读,foncms判断完后删除*/
        fp = fopen(PPP_IDLE_DISCONNECT_FOR_FON, "w+");
        if (fp)
        {
            fclose(fp);
            fp = NULL;
        }
#endif
        /*add by z67625 end*/
        lcp_close(0, "Link inactive");
        need_holdoff = 0;
        status = EXIT_IDLE_TIMEOUT;
        req_rediscon = 0;
        memset(sesinfo, 0, sizeof(sesinfo));

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
        sifnpstate(0, PPP_IP, NPSTATE_DOWN);
        sifnpstate(0, PPP_IPV6, NPSTATE_DOWN);
#endif
        if (demand)
        {
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
            g_iEnableV4Dial = 0;
            g_iEnableV6Dial = 0;
#endif
            DT_ResetRetryCounter(PPP_IP);
            DT_ResetRetryCounter(PPP_IPV6);
            goonflag = 1;
        }
#endif

    } else {
         if(tlim <= 0)
        {
            tlim = idle_time_limit;
        }
        TIMEOUT(check_idle, NULL, tlim);
    }
}

#if defined(SUPPORT_ATP_PPP_DYN_DIAL) && defined(SUPPORT_ATP_PPP_DT_TOCPE0056)
static void check_v4_idle(void)
{
    struct ppp_v4_idle idle;
    int tlim;
    char acCmd[64] = {0};
#ifdef BCM_PKTFLOW_MODULE
    time_t the_idle = 0;
#endif

    if (!get_v4_idle_time(0, &idle))
    {
        return;
    }
#ifdef BCM_PKTFLOW_MODULE
    the_idle = MIN(idle.xmit_v4_idle, ul_fcache_ipv4_idle);    
#endif

    if (idle_time_hook != 0) {
        tlim = idle_time_hook(&idle);
    } 
    else
    {
#ifdef BCM_PKTFLOW_MODULE
        if (demand)
        {
            tlim = idle_time_limit - the_idle;
        }
        else
        {
            tlim = get_always_on_v4_idle() - the_idle;
        }
#else
        if (demand)
        {
            tlim = idle_time_limit - idle.xmit_v4_idle;
        }
        else
        {
            tlim = get_always_on_v4_idle() - idle.xmit_v4_idle;
        }
#endif
    }
    /*DT要求在出厂状态时，pppoe在空闲时间到后就应该断开，而不用理会provisioncode,因此问题单AU8D04111不是问题 */
    /*w723v 问题单AU8D04111 by g00121640  2010-3-23 start*/
    if ((tlim <= 0) && (is_ppp_can_down()))
    {
        DT_NotifyConnectionDown(ATP_MSG_WAN_V4_WILLDOWN);
        /* v4 is idle: shut it down. */
        notice("Terminating ipv4 connection due to lack of activity.");
        pppoe_setlasterror("ERROR_IDLE_DISCONNECT");
        fsm_close(&ipcp_fsm[0], "ipv4 idle timer expire");
        //置为等待触发拨号
        ipcp_fsm[0].dt_ncp_stage = STAGE_WAITING_TRIGGER ;
        g_iEnableV4Dial = 0;
        sifnpstate(0, PPP_IP, NPSTATE_DOWN);

        snprintf(acCmd, sizeof(acCmd), "route del default"); //for W724v 避免有多余的默认路由
        ATP_UTIL_ForkProcess(acCmd, -1, NULL, NULL);
        snprintf(acCmd, sizeof(acCmd), "route add default dev %s", W724V_PPP_NAME);
        ATP_UTIL_ForkProcess(acCmd, -1, NULL, NULL);

    } else {
        if(tlim <= 0)
        {
            if (!demand)
            {
                tlim = get_always_on_v4_idle();
            }
            else
            {
                tlim = idle_time_limit;
            }
        }
        TIMEOUT(check_v4_idle, NULL, tlim);
    }
}
#endif

int getdialstatus(void)
{
    FILE * fd = NULL;
    char path[128]="";
    char data[8];
    int lRet = 0;
    /*Start of ATP 2011-5-4 8:55 for  by h00163136*/
#if 0
    sprintf(path, "%s%s/dial", WAN_PATH, wanif_name);
#else
    snprintf(path, sizeof(path), "%s%s/dial", WAN_PATH, wanif_name);
#endif
    /*End of ATP 2011-5-4 8:55 by h00163136*/
    if ((fd=fopen(path, "r"))) {
	fgets(data, 8, fd); //获取拨号状态，4个,手动,自动,断线,等待
	fclose(fd);
    lRet =atoi(data);
	if(Ondemand == lRet)
	{
     demand=1;
	}
	}
	return lRet;
}

int setdialstatus(int stat)
{
    FILE * fd = NULL;
    char path[128]="";
    char data[8];
    int lRet = 0;
    /*Start of ATP 2011-5-4 8:55 for  by h00163136*/
#if 0
    sprintf(path, "%s%s/dial", WAN_PATH, wanif_name);
    sprintf(data,"%d",stat);
#else
    snprintf(path, sizeof(path), "%s%s/dial", WAN_PATH, wanif_name);
    snprintf(data, sizeof(data), "%d",stat);
#endif
    /*End of ATP 2011-5-4 8:55 by h00163136*/
    if ((fd=fopen(path, "w"))) 
    {
        fputs(data,fd);//设置拨号状态
        fclose(fd);
        lRet = 1;
    }
    return lRet;
}

static void check_status(arg)
    void *arg;
{
    /*add by z67625 start*/
    if (Discon == getdialstatus()) {
    /* START: Add by y00183561 for DTS2012092205585 手动disconnect要先发注销 2012/9/24 */
    DT_NotifyConnectionDown(ATP_MSG_WAN_V4_V6_WILLDOWN);
    /* END: Add by y00183561 for DTS2012092205585 2012/9/24 */  
    
/* start of ATP 2008.02.02 ATPV100R002C01B021 AU4D00321 ppp的设置由pppc在会话协商时自行设置 */
    clear_tcp_mss();
/* end of ATP 2008.02.02 ATPV100R002C01B021 AU4D00321 ppp的设置由pppc在会话协商时自行设置 */
    /* link is idle: shut it down. */
	notice("Terminating connection due to link down.");
    pppoe_setlasterror("ERROR_USER_DISCONNECT");
    /*add by z67625 end*/
    lcp_close(0, "Link down");
	need_holdoff = 0;
	status = EXIT_IDLE_TIMEOUT;
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
        //for W724
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
        sifnpstate(0, PPP_IP, NPSTATE_DOWN);
        sifnpstate(0, PPP_IPV6, NPSTATE_DOWN);
#endif
        if (demand)
        {
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
            g_iEnableV4Dial = 0;
            g_iEnableV6Dial = 0;
#endif
            DT_ResetRetryCounter(PPP_IP);
            DT_ResetRetryCounter(PPP_IPV6);
            goonflag = 1;
        }
#endif   
    } else {
	UNTIMEOUT(check_status, NULL);
	TIMEOUT(check_status, NULL, PPP_CHECK_STATUS_TIMEOUT);
    }
}


/*
 * connect_time_expired - log a message and close the connection.
 */
static void
connect_time_expired(arg)
    void *arg;
{
    info("Connect time expired");
    pppoe_setlasterror("ERROR_ISP_TIME_OUT");
    lcp_close(0, "Connect time expired");	/* Close connection */
    status = EXIT_CONNECT_TIME;
}

/*
 * auth_check_options - called to check authentication options.
 */
void
auth_check_options()
{
    lcp_options *wo = &lcp_wantoptions[0];
    int can_auth;
    int lacks_ip;

    /* Default our_name to hostname, and user to our_name */
    if (our_name[0] == 0 || usehostname)
	strlcpy(our_name, hostname, sizeof(our_name));
    if (user[0] == 0)
	strlcpy(user, our_name, sizeof(user));

    /*
     * If we have a default route, require the peer to authenticate
     * unless the noauth option was given or the real user is root.
     */
    if (!auth_required && !allow_any_ip && have_route_to(0) && !privileged) {
	auth_required = 1;
	default_auth = 1;
    }

    /* If authentication is required, ask peer for CHAP or PAP. */
    if (auth_required) {
	allow_any_ip = 0;
	if (!wo->neg_chap && !wo->neg_upap) {
	    wo->neg_chap = 1;
	    wo->neg_upap = 1;
	}
    } else {
	wo->neg_chap = 0;
	wo->neg_upap = 0;
    }

    /*
     * Check whether we have appropriate secrets to use
     * to authenticate the peer.
     */
    lacks_ip = 0;
    can_auth = wo->neg_upap && (uselogin || have_pap_secret(&lacks_ip));
    if (!can_auth && wo->neg_chap) {
	can_auth = have_chap_secret((explicit_remote? remote_name: NULL),
				    our_name, 1, &lacks_ip);
    }

    if (auth_required && !can_auth && noauth_addrs == NULL) {
	if (default_auth) {
	    option_error(
"By default the remote system is required to authenticate itself");
	    option_error(
"(because this system has a default route to the internet)");
	} else if (explicit_remote)
	    option_error(
"The remote system (%s) is required to authenticate itself",
			 remote_name);
	else
	    option_error(
"The remote system is required to authenticate itself");
	option_error(
"but I couldn't find any suitable secret (password) for it to use to do so.");
	if (lacks_ip)
	    option_error(
"(None of the available passwords would let it use an IP address.)");

	exit(1);
    }
}

/*
 * auth_reset - called when LCP is starting negotiations to recheck
 * authentication options, i.e. whether we have appropriate secrets
 * to use for authenticating ourselves and/or the peer.
 */
void
auth_reset(unit)
    int unit;
{
    lcp_options *go = &lcp_gotoptions[unit];
    lcp_options *ao = &lcp_allowoptions[0];

    ao->neg_upap = !refuse_pap && (passwd[0] != 0 || get_pap_passwd(NULL));
    ao->neg_chap = !refuse_chap
	&& (passwd[0] != 0
	    || have_chap_secret(user, (explicit_remote? remote_name: NULL),
				0, NULL));

    if (go->neg_upap && !uselogin && !have_pap_secret(NULL))
	go->neg_upap = 0;
    if (go->neg_chap) {
	if (!have_chap_secret((explicit_remote? remote_name: NULL),
			      our_name, 1, NULL))
	    go->neg_chap = 0;
    }
}


/*
 * check_passwd - Check the user name and passwd against the PAP secrets
 * file.  If requested, also check against the system password database,
 * and login the user if OK.
 *
 * returns:
 *	UPAP_AUTHNAK: Authentication failed.
 *	UPAP_AUTHACK: Authentication succeeded.
 * In either case, msg points to an appropriate message.
 */
int
check_passwd(unit, auser, userlen, apasswd, passwdlen, msg)
    int unit;
    char *auser;
    int userlen;
    char *apasswd;
    int passwdlen;
    char **msg;
{
    int ret;
    char *filename;
    FILE *f;
    struct wordlist *addrs = NULL, *opts = NULL;
    char passwd[256], user[256];
    char secret[MAXWORDLEN];
    static int attempts = 0;

    /*
     * Make copies of apasswd and auser, then null-terminate them.
     * If there are unprintable characters in the password, make
     * them visible.
     */
    slprintf(passwd, sizeof(passwd), "%.*v", passwdlen, apasswd);
    slprintf(user, sizeof(user), "%.*v", userlen, auser);
    *msg = "";

    /*
     * Check if a plugin wants to handle this.
     */
    if (pap_auth_hook) {
	ret = (*pap_auth_hook)(user, passwd, msg, &addrs, &opts);
	if (ret >= 0) {
	    if (ret)
		set_allowed_addrs(unit, addrs, opts);
	    BZERO(passwd, sizeof(passwd));
	    if (addrs != 0)
		free_wordlist(addrs);
	    return ret? UPAP_AUTHACK: UPAP_AUTHNAK;
	}
    }

    /*
     * Open the file of pap secrets and scan for a suitable secret
     * for authenticating this user.
     */
    filename = _PATH_UPAPFILE;
    addrs = opts = NULL;
    ret = UPAP_AUTHNAK;
    f = fopen(filename, "r");
    if (f == NULL) {
	error("Can't open PAP password file %s: %m", filename);

    } else {
	check_access(f, filename);
	if (scan_authfile(f, user, our_name, secret, &addrs, &opts, filename) < 0) {
	    warn("no PAP secret found for %s", user);
	} else {
	    /*
	     * If the secret is "@login", it means to check
	     * the password against the login database.
	     */
	    int login_secret = strcmp(secret, "@login") == 0;
	    ret = UPAP_AUTHACK;
	    if (uselogin || login_secret) {
		/* login option or secret is @login */
		ret = plogin(user, passwd, msg);
		if (ret == UPAP_AUTHNAK)
		    warn("PAP login failure for %s", user);
		else
		    used_login = 1;
	    }
	    if (secret[0] != 0 && !login_secret) {
		/* password given in pap-secrets - must match */
		if ((cryptpap || strcmp(passwd, secret) != 0)
		    && strcmp(crypt(passwd, secret), secret) != 0) {
		    ret = UPAP_AUTHNAK;
		    warn("PAP authentication failure for %s", user);
		}
	    }
	}
	fclose(f);
    }

    if (ret == UPAP_AUTHNAK) {
        if (**msg == 0)
	    *msg = "Login incorrect";
	/*
	 * XXX can we ever get here more than once??
	 * Frustrate passwd stealer programs.
	 * Allow 10 tries, but start backing off after 3 (stolen from login).
	 * On 10'th, drop the connection.
	 */
	if (attempts++ >= 10) {
	    warn("%d LOGIN FAILURES ON %s, %s", attempts, devnam, user);
        /*add by z67625 start*/
        pppoe_setlasterror("ERROR_AUTHENTICATION_FAILURE");
        /*add by z67625 end*/
	    lcp_close(unit, "login failed");
	}
	if (attempts > 3)
	    sleep((u_int) (attempts - 3) * 5);
	if (opts != NULL)
	    free_wordlist(opts);

    } else {
	attempts = 0;			/* Reset count */
	if (**msg == 0)
	    *msg = "Login ok";
	set_allowed_addrs(unit, addrs, opts);
    }

    if (addrs != NULL)
	free_wordlist(addrs);
    BZERO(passwd, sizeof(passwd));
    BZERO(secret, sizeof(secret));

    return ret;
}

/*
 * This function is needed for PAM.
 */

#ifdef USE_PAM
/* Static variables used to communicate between the conversation function
 * and the server_login function 
 */
static char *PAM_username;
static char *PAM_password;
static int PAM_error = 0;
static pam_handle_t *pamh = NULL;

/* PAM conversation function
 * Here we assume (for now, at least) that echo on means login name, and
 * echo off means password.
 */

static int PAM_conv (int num_msg, const struct pam_message **msg,
                    struct pam_response **resp, void *appdata_ptr)
{
    int replies = 0;
    struct pam_response *reply = NULL;

#define COPY_STRING(s) (s) ? strdup(s) : NULL

    reply = malloc(sizeof(struct pam_response) * num_msg);
    if (!reply) return PAM_CONV_ERR;

    for (replies = 0; replies < num_msg; replies++) {
        switch (msg[replies]->msg_style) {
            case PAM_PROMPT_ECHO_ON:
                reply[replies].resp_retcode = PAM_SUCCESS;
                reply[replies].resp = COPY_STRING(PAM_username);
                /* PAM frees resp */
                break;
            case PAM_PROMPT_ECHO_OFF:
                reply[replies].resp_retcode = PAM_SUCCESS;
                reply[replies].resp = COPY_STRING(PAM_password);
                /* PAM frees resp */
                break;
            case PAM_TEXT_INFO:
                /* fall through */
            case PAM_ERROR_MSG:
                /* ignore it, but pam still wants a NULL response... */
                reply[replies].resp_retcode = PAM_SUCCESS;
                reply[replies].resp = NULL;
                break;
            default:       
                /* Must be an error of some sort... */
                free (reply);
                PAM_error = 1;
                return PAM_CONV_ERR;
        }
    }
    *resp = reply;     
    return PAM_SUCCESS;
}

static struct pam_conv PAM_conversation = {
    &PAM_conv,
    NULL
};
#endif  /* USE_PAM */

/*
 * plogin - Check the user name and password against the system
 * password database, and login the user if OK.
 *
 * returns:
 *	UPAP_AUTHNAK: Login failed.
 *	UPAP_AUTHACK: Login succeeded.
 * In either case, msg points to an appropriate message.
 */

static int
plogin(user, passwd, msg)
    char *user;
    char *passwd;
    char **msg;
{
    char *tty;

#ifdef USE_PAM
    int pam_error;

    pam_error = pam_start ("ppp", user, &PAM_conversation, &pamh);
    if (pam_error != PAM_SUCCESS) {
        *msg = (char *) pam_strerror (pamh, pam_error);
	reopen_log();
	return UPAP_AUTHNAK;
    }
    /*
     * Define the fields for the credential validation
     */
     
    PAM_username = user;
    PAM_password = passwd;
    PAM_error = 0;
    pam_set_item (pamh, PAM_TTY, devnam); /* this might be useful to some modules */

    /*
     * Validate the user
     */
    pam_error = pam_authenticate (pamh, PAM_SILENT);
    if (pam_error == PAM_SUCCESS && !PAM_error) {    
        pam_error = pam_acct_mgmt (pamh, PAM_SILENT);
        if (pam_error == PAM_SUCCESS)
	    pam_open_session (pamh, PAM_SILENT);
    }

    *msg = (char *) pam_strerror (pamh, pam_error);

    /*
     * Clean up the mess
     */
    reopen_log();	/* apparently the PAM stuff does closelog() */
    PAM_username = NULL;
    PAM_password = NULL;
    if (pam_error != PAM_SUCCESS)
        return UPAP_AUTHNAK;
#else /* #ifdef USE_PAM */

/*
 * Use the non-PAM methods directly
 */

#ifdef HAS_SHADOW
    struct spwd *spwd;
    struct spwd *getspnam();
#endif
    struct passwd *pw = getpwnam(user);

    endpwent();
    if (pw == NULL)
	return (UPAP_AUTHNAK);

#ifdef HAS_SHADOW
    spwd = getspnam(user);
    endspent();
    if (spwd) {
	/* check the age of the password entry */
	long now = time(NULL) / 86400L;

	if ((spwd->sp_expire > 0 && now >= spwd->sp_expire)
	    || ((spwd->sp_max >= 0 && spwd->sp_max < 10000)
		&& spwd->sp_lstchg >= 0
		&& now >= spwd->sp_lstchg + spwd->sp_max)) {
	    warn("Password for %s has expired", user);
	    return (UPAP_AUTHNAK);
	}
	pw->pw_passwd = spwd->sp_pwdp;
    }
#endif

    /*
     * If no passwd, don't let them login.
     */
    if (pw->pw_passwd == NULL || strlen(pw->pw_passwd) < 2
	|| strcmp(crypt(passwd, pw->pw_passwd), pw->pw_passwd) != 0)
	return (UPAP_AUTHNAK);

#endif /* #ifdef USE_PAM */

    /*
     * Write a wtmp entry for this user.
     */

    tty = devnam;
    if (strncmp(tty, "/dev/", 5) == 0)
	tty += 5;
    logwtmp(tty, user, remote_name);		/* Add wtmp login entry */

#if defined(_PATH_LASTLOG) && !defined(USE_PAM)
    if (pw != (struct passwd *)NULL) {
	    struct lastlog ll;
	    int fd;

	    if ((fd = open(_PATH_LASTLOG, O_RDWR, 0)) >= 0) {
		(void)lseek(fd, (off_t)(pw->pw_uid * sizeof(ll)), SEEK_SET);
		memset((void *)&ll, 0, sizeof(ll));
		(void)time(&ll.ll_time);
		(void)strncpy(ll.ll_line, tty, sizeof(ll.ll_line));
		(void)write(fd, (char *)&ll, sizeof(ll));
		(void)close(fd);
	    }
    }
#endif /* _PATH_LASTLOG and not USE_PAM */

    info("user %s logged in", user);
    logged_in = 1;

    return (UPAP_AUTHACK);
}

/*
 * plogout - Logout the user.
 */
static void
plogout()
{
#ifdef USE_PAM
    int pam_error;

    if (pamh != NULL) {
	pam_error = pam_close_session (pamh, PAM_SILENT);
	pam_end (pamh, pam_error);
	pamh = NULL;
    }
    /* Apparently the pam stuff does closelog(). */
    reopen_log();
#else /* ! USE_PAM */   
    char *tty;

    tty = devnam;
    if (strncmp(tty, "/dev/", 5) == 0)
	tty += 5;
    logwtmp(tty, "", "");		/* Wipe out utmp logout entry */
#endif /* ! USE_PAM */
    logged_in = 0;
}


/*
 * null_login - Check if a username of "" and a password of "" are
 * acceptable, and iff so, set the list of acceptable IP addresses
 * and return 1.
 */
static int
null_login(unit)
    int unit;
{
    char *filename;
    FILE *f;
    int i, ret;
    struct wordlist *addrs, *opts;
    char secret[MAXWORDLEN];

    /*
     * Open the file of pap secrets and scan for a suitable secret.
     */
    filename = _PATH_UPAPFILE;
    addrs = NULL;
    f = fopen(filename, "r");
    if (f == NULL)
	return 0;
    check_access(f, filename);

    i = scan_authfile(f, "", our_name, secret, &addrs, &opts, filename);
    ret = i >= 0 && secret[0] == 0;
    BZERO(secret, sizeof(secret));

    if (ret)
	set_allowed_addrs(unit, addrs, opts);
    else if (opts != 0)
	free_wordlist(opts);
    if (addrs != 0)
	free_wordlist(addrs);

    fclose(f);
    return ret;
}


/*
 * get_pap_passwd - get a password for authenticating ourselves with
 * our peer using PAP.  Returns 1 on success, 0 if no suitable password
 * could be found.
 * Assumes passwd points to MAXSECRETLEN bytes of space (if non-null).
 */
static int
get_pap_passwd(passwd)
    char *passwd;
{
    char *filename;
    FILE *f;
    int ret;
    char secret[MAXWORDLEN];

    /*
     * Check whether a plugin wants to supply this.
     */
    if (pap_passwd_hook) {
	ret = (*pap_passwd_hook)(user, passwd);
	if (ret >= 0)
	    return ret;
    }

    filename = _PATH_UPAPFILE;
    f = fopen(filename, "r");
    if (f == NULL)
	return 0;
    check_access(f, filename);
    ret = scan_authfile(f, user,
			(remote_name[0]? remote_name: NULL),
			secret, NULL, NULL, filename);
    fclose(f);
    if (ret < 0)
	return 0;
    if (passwd != NULL)
	strlcpy(passwd, secret, MAXSECRETLEN);
    BZERO(secret, sizeof(secret));
    return 1;
}


/*
 * have_pap_secret - check whether we have a PAP file with any
 * secrets that we could possibly use for authenticating the peer.
 */
static int
have_pap_secret(lacks_ipp)
    int *lacks_ipp;
{
    FILE *f;
    int ret;
    char *filename;
    struct wordlist *addrs;

    /* let the plugin decide, if there is one */
    if (pap_check_hook) {
	ret = (*pap_check_hook)();
	if (ret >= 0)
	    return ret;
    }

    filename = _PATH_UPAPFILE;
    f = fopen(filename, "r");
    if (f == NULL)
	return 0;

    ret = scan_authfile(f, (explicit_remote? remote_name: NULL), our_name,
			NULL, &addrs, NULL, filename);
    fclose(f);
    if (ret >= 0 && !some_ip_ok(addrs)) {
	if (lacks_ipp != 0)
	    *lacks_ipp = 1;
	ret = -1;
    }
    if (addrs != 0)
	free_wordlist(addrs);

    return ret >= 0;
}


/*
 * have_chap_secret - check whether we have a CHAP file with a
 * secret that we could possibly use for authenticating `client'
 * on `server'.  Either can be the null string, meaning we don't
 * know the identity yet.
 */
static int
have_chap_secret(client, server, need_ip, lacks_ipp)
    char *client;
    char *server;
    int need_ip;
    int *lacks_ipp;
{
    FILE *f;
    int ret;
    char *filename;
    struct wordlist *addrs;

    filename = _PATH_CHAPFILE;
    f = fopen(filename, "r");
    if (f == NULL)
	return 0;

    if (client != NULL && client[0] == 0)
	client = NULL;
    else if (server != NULL && server[0] == 0)
	server = NULL;

    ret = scan_authfile(f, client, server, NULL, &addrs, NULL, filename);
    fclose(f);
    if (ret >= 0 && need_ip && !some_ip_ok(addrs)) {
	if (lacks_ipp != 0)
	    *lacks_ipp = 1;
	ret = -1;
    }
    if (addrs != 0)
	free_wordlist(addrs);

    return ret >= 0;
}


/*
 * get_secret - open the CHAP secret file and return the secret
 * for authenticating the given client on the given server.
 * (We could be either client or server).
 */
int
get_secret(unit, client, server, secret, secret_len, am_server)
    int unit;
    char *client;
    char *server;
    char *secret;
    int *secret_len;
    int am_server;
{
    FILE *f;
    int ret, len;
    char *filename;
    struct wordlist *addrs, *opts;
    char secbuf[MAXWORDLEN];

    if (!am_server && passwd[0] != 0) {
	strlcpy(secbuf, passwd, sizeof(secbuf));
    } else {
	filename = _PATH_CHAPFILE;
	addrs = NULL;
	secbuf[0] = 0;

	f = fopen(filename, "r");
	if (f == NULL) {
	    error("Can't open chap secret file %s: %m", filename);
	    return 0;
	}
	check_access(f, filename);

	ret = scan_authfile(f, client, server, secbuf, &addrs, &opts, filename);
	fclose(f);
	if (ret < 0)
	    return 0;

	if (am_server)
	    set_allowed_addrs(unit, addrs, opts);
	else if (opts != 0)
	    free_wordlist(opts);
	if (addrs != 0)
	    free_wordlist(addrs);
    }

    len = strlen(secbuf);
    if (len > MAXSECRETLEN) {
	error("Secret for %s on %s is too long", client, server);
	len = MAXSECRETLEN;
    }
    BCOPY(secbuf, secret, len);
    BZERO(secbuf, sizeof(secbuf));
    *secret_len = len;

    return 1;
}

/*
 * set_allowed_addrs() - set the list of allowed addresses.
 * Also looks for `--' indicating options to apply for this peer
 * and leaves the following words in extra_options.
 */
static void
set_allowed_addrs(unit, addrs, opts)
    int unit;
    struct wordlist *addrs;
    struct wordlist *opts;
{
    int n;
    struct wordlist *ap, **plink;
    struct permitted_ip *ip;
    char *ptr_word, *ptr_mask;
    struct hostent *hp;
    struct netent *np;
    u_int32_t a, mask, ah, offset;
    struct ipcp_options *wo = &ipcp_wantoptions[unit];
    u_int32_t suggested_ip = 0;

    if (addresses[unit] != NULL)
	free(addresses[unit]);
    addresses[unit] = NULL;
    if (extra_options != NULL)
	free_wordlist(extra_options);
    extra_options = opts;

    /*
     * Count the number of IP addresses given.
     */
    n = wordlist_count(addrs) + wordlist_count(noauth_addrs);
    if (n == 0)
	return;
    ip = (struct permitted_ip *) malloc((n + 1) * sizeof(struct permitted_ip));
    if (ip == 0)
	return;

    /* temporarily append the noauth_addrs list to addrs */
    for (plink = &addrs; *plink != NULL; plink = &(*plink)->next)
	;
    *plink = noauth_addrs;

    n = 0;
    for (ap = addrs; ap != NULL; ap = ap->next) {
	/* "-" means no addresses authorized, "*" means any address allowed */
	ptr_word = ap->word;
	if (strcmp(ptr_word, "-") == 0)
	    break;
	if (strcmp(ptr_word, "*") == 0) {
	    ip[n].permit = 1;
	    ip[n].base = ip[n].mask = 0;
	    ++n;
	    break;
	}

	ip[n].permit = 1;
	if (*ptr_word == '!') {
	    ip[n].permit = 0;
	    ++ptr_word;
	}

	mask = ~ (u_int32_t) 0;
	offset = 0;
	ptr_mask = strchr (ptr_word, '/');
	if (ptr_mask != NULL) {
	    int bit_count;
	    char *endp;

	    bit_count = (int) strtol (ptr_mask+1, &endp, 10);
	    if (bit_count <= 0 || bit_count > 32) {
		warn("invalid address length %v in auth. address list",
		     ptr_mask+1);
		continue;
	    }
	    bit_count = 32 - bit_count;	/* # bits in host part */
	    if (*endp == '+') {
		offset = ifunit + 1;
		++endp;
	    }
	    if (*endp != 0) {
		warn("invalid address length syntax: %v", ptr_mask+1);
		continue;
	    }
	    *ptr_mask = '\0';
	    mask <<= bit_count;
	}

	hp = gethostbyname(ptr_word);
	if (hp != NULL && hp->h_addrtype == AF_INET) {
	    a = *(u_int32_t *)hp->h_addr;
	} else {
	    np = getnetbyname (ptr_word);
	    if (np != NULL && np->n_addrtype == AF_INET) {
		a = htonl (*(u_int32_t *)np->n_net);
		if (ptr_mask == NULL) {
		    /* calculate appropriate mask for net */
		    ah = ntohl(a);
		    if (IN_CLASSA(ah))
			mask = IN_CLASSA_NET;
		    else if (IN_CLASSB(ah))
			mask = IN_CLASSB_NET;
		    else if (IN_CLASSC(ah))
			mask = IN_CLASSC_NET;
		}
	    } else {
		a = inet_addr (ptr_word);
	    }
	}

	if (ptr_mask != NULL)
	    *ptr_mask = '/';

	if (a == (u_int32_t)-1L) {
	    warn("unknown host %s in auth. address list", ap->word);
	    continue;
	}
	if (offset != 0) {
	    if (offset >= ~mask) {
		warn("interface unit %d too large for subnet %v",
		     ifunit, ptr_word);
		continue;
	    }
	    a = htonl((ntohl(a) & mask) + offset);
	    mask = ~(u_int32_t)0;
	}
	ip[n].mask = htonl(mask);
	ip[n].base = a & ip[n].mask;
	++n;
	if (~mask == 0 && suggested_ip == 0)
	    suggested_ip = a;
    }
    *plink = NULL;

    ip[n].permit = 0;		/* make the last entry forbid all addresses */
    ip[n].base = 0;		/* to terminate the list */
    ip[n].mask = 0;

    addresses[unit] = ip;

    /*
     * If the address given for the peer isn't authorized, or if
     * the user hasn't given one, AND there is an authorized address
     * which is a single host, then use that if we find one.
     */
    if (suggested_ip != 0
	&& (wo->hisaddr == 0 || !auth_ip_addr(unit, wo->hisaddr))) {
	wo->hisaddr = suggested_ip;
	/*
	 * Do we insist on this address?  No, if there are other
	 * addresses authorized than the suggested one.
	 */
	if (n > 1)
	    wo->accept_remote = 1;
    }
}

/*
 * auth_ip_addr - check whether the peer is authorized to use
 * a given IP address.  Returns 1 if authorized, 0 otherwise.
 */
int
auth_ip_addr(unit, addr)
    int unit;
    u_int32_t addr;
{
    int ok;

    /* don't allow loopback or multicast address */
    if (bad_ip_adrs(addr))
	return 0;

    if (addresses[unit] != NULL) {
	ok = ip_addr_check(addr, addresses[unit]);
	if (ok >= 0)
	    return ok;
    }
    if (auth_required)
	return 0;		/* no addresses authorized */
    return allow_any_ip || privileged || !have_route_to(addr);
}

static int
ip_addr_check(addr, addrs)
    u_int32_t addr;
    struct permitted_ip *addrs;
{
    for (; ; ++addrs)
	if ((addr & addrs->mask) == addrs->base)
	    return addrs->permit;
}

/*
 * bad_ip_adrs - return 1 if the IP address is one we don't want
 * to use, such as an address in the loopback net or a multicast address.
 * addr is in network byte order.
 */
int
bad_ip_adrs(addr)
    u_int32_t addr;
{
    addr = ntohl(addr);
    return (addr >> IN_CLASSA_NSHIFT) == IN_LOOPBACKNET
	|| IN_MULTICAST(addr) || IN_BADCLASS(addr);
}

/*
 * some_ip_ok - check a wordlist to see if it authorizes any
 * IP address(es).
 */
static int
some_ip_ok(addrs)
    struct wordlist *addrs;
{
    for (; addrs != 0; addrs = addrs->next) {
	if (addrs->word[0] == '-')
	    break;
	if (addrs->word[0] != '!')
	    return 1;		/* some IP address is allowed */
    }
    return 0;
}

/*
 * check_access - complain if a secret file has too-liberal permissions.
 */
static void
check_access(f, filename)
    FILE *f;
    char *filename;
{
    struct stat sbuf;

    if (fstat(fileno(f), &sbuf) < 0) {
	warn("cannot stat secret file %s: %m", filename);
    } else if ((sbuf.st_mode & (S_IRWXG | S_IRWXO)) != 0) {
	warn("Warning - secret file %s has world and/or group access",
	     filename);
    }
}


/*
 * scan_authfile - Scan an authorization file for a secret suitable
 * for authenticating `client' on `server'.  The return value is -1
 * if no secret is found, otherwise >= 0.  The return value has
 * NONWILD_CLIENT set if the secret didn't have "*" for the client, and
 * NONWILD_SERVER set if the secret didn't have "*" for the server.
 * Any following words on the line up to a "--" (i.e. address authorization
 * info) are placed in a wordlist and returned in *addrs.  Any
 * following words (extra options) are placed in a wordlist and
 * returned in *opts.
 * We assume secret is NULL or points to MAXWORDLEN bytes of space.
 */
static int
scan_authfile(f, client, server, secret, addrs, opts, filename)
    FILE *f;
    char *client;
    char *server;
    char *secret;
    struct wordlist **addrs;
    struct wordlist **opts;
    char *filename;
{
    int newline, xxx;
    int got_flag, best_flag;
    FILE *sf;
    struct wordlist *ap, *addr_list, *alist, **app;
    char word[MAXWORDLEN];
    char atfile[MAXWORDLEN];
    char lsecret[MAXWORDLEN];

    if (addrs != NULL)
	*addrs = NULL;
    if (opts != NULL)
	*opts = NULL;
    addr_list = NULL;
    if (!getword(f, word, &newline, filename))
	return -1;		/* file is empty??? */
    newline = 1;
    best_flag = -1;
    for (;;) {
	/*
	 * Skip until we find a word at the start of a line.
	 */
	while (!newline && getword(f, word, &newline, filename))
	    ;
	if (!newline)
	    break;		/* got to end of file */

	/*
	 * Got a client - check if it's a match or a wildcard.
	 */
	got_flag = 0;
	if (client != NULL && strcmp(word, client) != 0 && !ISWILD(word)) {
	    newline = 0;
	    continue;
	}
	if (!ISWILD(word))
	    got_flag = NONWILD_CLIENT;

	/*
	 * Now get a server and check if it matches.
	 */
	if (!getword(f, word, &newline, filename))
	    break;
	if (newline)
	    continue;
	if (!ISWILD(word)) {
	    if (server != NULL && strcmp(word, server) != 0)
		continue;
	    got_flag |= NONWILD_SERVER;
	}

	/*
	 * Got some sort of a match - see if it's better than what
	 * we have already.
	 */
	if (got_flag <= best_flag)
	    continue;

	/*
	 * Get the secret.
	 */
	if (!getword(f, word, &newline, filename))
	    break;
	if (newline)
	    continue;

	if (secret != NULL) {
	    /*
	     * Special syntax: @/pathname means read secret from file.
	     */
	    if (word[0] == '@' && word[1] == '/') {
		strlcpy(atfile, word+1, sizeof(atfile));
		if ((sf = fopen(atfile, "r")) == NULL) {
		    warn("can't open indirect secret file %s", atfile);
		    continue;
		}
		check_access(sf, atfile);
		if (!getword(sf, word, &xxx, atfile)) {
		    warn("no secret in indirect secret file %s", atfile);
		    fclose(sf);
		    continue;
		}
		fclose(sf);
	    }
	    strlcpy(lsecret, word, sizeof(lsecret));
	}

	/*
	 * Now read address authorization info and make a wordlist.
	 */
	app = &alist;
	for (;;) {
	    if (!getword(f, word, &newline, filename) || newline)
		break;
	    ap = (struct wordlist *)
		    malloc(sizeof(struct wordlist) + strlen(word) + 1);
	    if (ap == NULL)
		novm("authorized addresses");
	    ap->word = (char *) (ap + 1);
	    strcpy(ap->word, word);
	    *app = ap;
	    app = &ap->next;
	}
	*app = NULL;

	/*
	 * This is the best so far; remember it.
	 */
	best_flag = got_flag;
	if (addr_list)
	    free_wordlist(addr_list);
	addr_list = alist;
	if (secret != NULL)
	    strlcpy(secret, lsecret, MAXWORDLEN);

	if (!newline)
	    break;
    }

    /* scan for a -- word indicating the start of options */
    for (app = &addr_list; (ap = *app) != NULL; app = &ap->next)
	if (strcmp(ap->word, "--") == 0)
	    break;
    /* ap = start of options */
    if (ap != NULL) {
	ap = ap->next;		/* first option */
	free(*app);			/* free the "--" word */
	*app = NULL;		/* terminate addr list */
    }
    if (opts != NULL)
	*opts = ap;
    else if (ap != NULL)
	free_wordlist(ap);
    if (addrs != NULL)
	*addrs = addr_list;
    else if (addr_list != NULL)
	free_wordlist(addr_list);

    return best_flag;
}

/*
 * wordlist_count - return the number of items in a wordlist
 */
static int
wordlist_count(wp)
    struct wordlist *wp;
{
    int n;

    for (n = 0; wp != NULL; wp = wp->next)
	++n;
    return n;
}

/*
 * free_wordlist - release memory allocated for a wordlist.
 */
static void
free_wordlist(wp)
    struct wordlist *wp;
{
    struct wordlist *next;

    while (wp != NULL) {
	next = wp->next;
	free(wp);
	wp = next;
    }
}

/*
 * auth_script_done - called when the auth-up or auth-down script
 * has finished.
 */
static void
auth_script_done(arg)
    void *arg;
{
    auth_script_pid = 0;
    switch (auth_script_state) {
    case s_up:
	if (auth_state == s_down) {
	    auth_script_state = s_down;
	    auth_script(_PATH_AUTHDOWN);
	}
	break;
    case s_down:
	if (auth_state == s_up) {
	    auth_script_state = s_up;
	    auth_script(_PATH_AUTHUP);
	}
	break;
    }
}

/*
 * auth_script - execute a script with arguments
 * interface-name peer-name real-user tty speed
 */
static void
auth_script(script)
    char *script;
{
    char strspeed[32];
    struct passwd *pw;
    char struid[32];
    char *user_name;
    char *argv[8];

    if ((pw = getpwuid(getuid())) != NULL && pw->pw_name != NULL)
	user_name = pw->pw_name;
    else {
	slprintf(struid, sizeof(struid), "%d", getuid());
	user_name = struid;
    }
    slprintf(strspeed, sizeof(strspeed), "%d", baud_rate);

    argv[0] = script;
    argv[1] = ifname;
    argv[2] = peer_authname;
    argv[3] = user_name;
    argv[4] = devnam;
    argv[5] = strspeed;
    argv[6] = NULL;

    auth_script_pid = run_program(script, argv, 0, auth_script_done, NULL);
}
#ifdef SUPPORT_ATP_PPPD
/* 
   check the state for wakeup.
   PPPX_WAN_WAKEUP  PPPX_TRUE
   other            PPPX_FALSE
*/
int pppx_wakeup(void)
{
    struct tagPPPXAcntSet *pAcntSet = NULL;
    struct tagPPPXWan     *pWan     = NULL;
    int nRet = PPPX_FALSE;

    if(NULL == (pAcntSet = PPPX_GetAcntSet(shm_id)))
        return PPPX_FALSE;
    
    if(NULL == (pWan = PPPX_FindWan(pAcntSet, log_name)))
        goto local_return;

    if(PPPX_WAN_WAKEUP == pWan->state)
    {
        /*Start of ATP 2011-5-3 15:5 for  by h00163136*/
    #if 0
        strcpy(user, pWan->acnt->user);
        strcpy(passwd, pWan->acnt->pw);
    #else
        snprintf(user, sizeof(user), "%s", pWan->acnt->user);
        snprintf(passwd, sizeof(passwd), "%s", pWan->acnt->pw);
    #endif
        /*End of ATP 2011-5-3 15:5 by h00163136*/
        nRet = PPPX_TRUE;
    }

local_return:
    PPPX_PutAcntSet(pAcntSet);
    return nRet;
}

void pppx_setwan_state(char *name, int state)
{
    struct tagPPPXAcntSet *pAcntSet = NULL;
    struct tagPPPXWan     *pWan     = NULL;

    if(NULL == name)
        return;

    if(NULL != (pAcntSet = PPPX_GetAcntSet(shm_id))
     && NULL != (pWan = PPPX_FindWan(pAcntSet, name)))
    {
        pWan->state = state;
        if(PPPX_WAN_SLEEP == state)
        {
            pWan->acnt->user[0] = 0;
            pWan->acnt->pw[0] = 0;
            pWan->acnt->state = PPPX_ACNT_IDLE;
        }
        else
        {
            pWan->acnt->state = PPPX_ACNT_BUSY;            
        }
    }
    else
         printf("PPPOE ERR: %s ,shm_id=%d, name=%s, state=%d\n",__FUNCTION__, shm_id, log_name, state);

    if(debug)
        printf("PPP WAN: set wan<%s> state<%d> account.state<%d>\n", pWan->name, pWan->state, pWan->acnt->state);

    if(NULL != pAcntSet)
        PPPX_PutAcntSet(pAcntSet);
}

int pppx_getwan_state(char *name)
{
    struct tagPPPXAcntSet *pAcntSet = NULL;
    struct tagPPPXWan     *pWan     = NULL;
    int nRet = PPPX_WAN_INIT;

    if(NULL == name)
        return nRet;

    if(NULL != (pAcntSet = PPPX_GetAcntSet(shm_id))
     && NULL != (pWan = PPPX_FindWan(pAcntSet, name)))
    {
        nRet = pWan->state;
    }
    else
         printf("PPPOE ERR: %s ,shm_id=%d, name=%s, state=%d\n",__FUNCTION__, shm_id, log_name);

    if(NULL != pAcntSet)
        PPPX_PutAcntSet(pAcntSet);

    return nRet;
}

/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 start*/
void pppx_check_user(void * arg)
/*add by z67625 修改pppx检测网线拔插机制,2007-12-5 end*/
{

    struct tagPPPXAcntSet *pAcntSet = NULL;
    struct tagPPPXWan     *pWan     = NULL;
    int isIdle = PPPX_FALSE;

    if(NULL != (pAcntSet = PPPX_GetAcntSet(shm_id))
     && NULL != (pWan = PPPX_FindWan(pAcntSet, log_name))
     && 0 == pWan->acnt->lan_num)
    {
         isIdle = PPPX_TRUE;
         pWan->acnt->user[0] = 0;
         pWan->acnt->pw[0] = 0;
    }

    if(NULL != pAcntSet)
        PPPX_PutAcntSet(pAcntSet);
    
    if (PPPX_TRUE == isIdle) {
    	/* link is idle: shut it down. */
    	notice("Terminating connection due to no user on line.");
        /*add by z67625 start*/
        pppoe_setlasterror("ERROR_USER_DISCONN");
        /*add by z67625 end*/
    	lcp_close(0, "Link down");
    	need_holdoff = 0;
    	status = EXIT_IDLE_TIMEOUT;
        UNTIMEOUT(pppx_check_user, NULL);
    } else {
    	UNTIMEOUT(pppx_check_user, NULL);
    	TIMEOUT(pppx_check_user, NULL, 3);
    }
}
#endif
/* begin dial by hand, 20070406, s63873 */
#define PPPOE_DIAL_BYHAND_FILE "dial_byhand"
int check_hand_dial(void)
{
    char path[128] = {0};

    /*Start of ATP 2011-5-4 8:56 for  by h00163136*/
#if 0
    sprintf(path, "/var/%s_%s", wanif_name, PPPOE_DIAL_BYHAND_FILE);
#else
    snprintf(path, sizeof(path), "/var/%s_%s", wanif_name, PPPOE_DIAL_BYHAND_FILE);
#endif
    /*End of ATP 2011-5-4 8:56 by h00163136*/
    if (access(path, F_OK) == 0)
    {
        unlink(path);
        return 1;
    }
    else
        return 0;
}
#undef PPPOE_DIAL_BYHAND_FILE
/* end dial by hand, 20070406 */

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
void ppp_log_nak_code(const char *pszPacket, unsigned int ullen)
{
    char                acCmd[8] = {0};
    unsigned int        ulErr = 0;
    char                acMsg[256] = {0};
    unsigned int        ulMsg_Len = 0;
    unsigned char       *pcTemp = NULL;

    char acAutherr[256] = {0};
    char path[128] = {0};
    FILE *fp = NULL;

    if (NULL == pszPacket)
    {
        printf("====== handle error code input packet is null\n");
        if (0 == g_ulIspFlag)
        {
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001);
        }
        return;
    }
    if (0 != dial_protocol)
    {
        return;
    }

    if (ullen < 4)
    {
        printf("nak code len <4 \n");
        if (0 == g_ulIspFlag)
        {
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001);
        }
        return;
    }

    ulMsg_Len = ullen - 4;
    pcTemp = pszPacket + 4;

    if(ullen>=256)
    {
        memcpy(acAutherr,pszPacket,(256-1));
        acAutherr[256-1] = 0;
    }
    else
    {
        memcpy(acAutherr,pszPacket,ullen);
        acAutherr[ullen]= 0;
    }

    //供TR064 LastAuthErrorInfo参数使用
    sprintf(path,"%s%s/lastautherr",WAN_PATH,wanif_name);

    fp = fopen(path, "w");
    if (NULL == fp)
    {
        perror("open file error");
        return ;
    }
    fprintf(fp, "%s\n", acAutherr);

    fclose(fp);
    
    acCmd[0] = pszPacket[0];
    acCmd[1] = pszPacket[1];
    acCmd[2] = pszPacket[2];
    acCmd[3] = pszPacket[3];

    if (0 != g_ulIspFlag)
    {
        return;
    }

    if (('0' != acCmd[0]) || ('0' != acCmd[1])
        || (acCmd[2] < '0') || (acCmd[2] > '9')
        || (acCmd[3] < '0') || (acCmd[3] > '9'))
    {
        //如果是未知的错误码，则把所有NAK信息作为MSG输出
        if (ullen >= 256)
        {
            memcpy(acMsg, pszPacket, (sizeof(acMsg) - 1));
            acMsg[sizeof(acMsg) - 1] = 0;
        }
        else
        {
            memcpy(acMsg, pszPacket, ullen);
            acMsg[ullen] = 0;
        }
        ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0000, acMsg);
		fp = fopen(ErrCodeFilePath,"w+");
	    if(NULL != fp) {
		    fprintf(fp,"err_code_r56_none");
		    fclose(fp);
	    }
		/*start:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
        //ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LOG_FAIL_WITHOUT_INFO);
		//*End:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
    }
    else
    {
        //如果是已知的错误码，则根据错误码输出日志值，并将错误后的字符串作为日志的错误信息。
        ulErr = atoi(acCmd);
        if (ulMsg_Len >= sizeof(acMsg))
        {
            memcpy(acMsg, pcTemp, (sizeof(acMsg) - 1));
            acMsg[sizeof(acMsg) - 1] = 0;
        }
        else
        {
            memcpy(acMsg, pcTemp, ulMsg_Len);
            acMsg[ulMsg_Len] = 0;
        }
        //HOMEGW-15160, 4位错误码记到文件，供网页正确显示
        snprintf(path, sizeof(path), "%s%s/NakCode", WAN_PATH, wanif_name);
        fp = fopen(path, "w");
        if (fp)
        {
            fprintf(fp, "%c%c%c%c\n", acCmd[0], acCmd[1], acCmd[2], acCmd[3]);
            fclose(fp);
        }

        switch (ulErr)
        {
            case ERROR_CODE_0001:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0001, acMsg);
            break;
            case ERROR_CODE_0002:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0002, acMsg);
            break;
            case ERROR_CODE_0003:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0003, acMsg);
            break;
            case ERROR_CODE_0004:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0004, acMsg);
            break;        
            case ERROR_CODE_0005:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0005, acMsg);
            break;
            case ERROR_CODE_0006:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0006, acMsg);
            break;   
            case ERROR_CODE_0007:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0007, acMsg);
            break;
            case ERROR_CODE_0008:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0008, acMsg);
            break;
            case ERROR_CODE_0009:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0009, acMsg);
            break;
            /*Start of w723v 2012-5-7 9:55 for  by z00190439*/
            //增加错误码ERROR_CODE_0010 和ERROR_CODE_0011的处理,根据tocpe0041文档，这两种错误不需写入log
            case ERROR_CODE_0010:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0000, acMsg);
			/* 添 加 错 误 码对应的错误信息 */			
			fp = fopen(ErrCodeFilePath,"w+");
			if(NULL != fp) {
				fprintf(fp,"err_code_r56_0010");
				fclose(fp);
			}
            break;
            case ERROR_CODE_0011:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0000, acMsg);
			/* 添 加 错 误 码对应的错误信息 */
			fp = fopen(ErrCodeFilePath,"w+");
			if(NULL != fp) {
				fprintf(fp,"err_code_r56_0011");
				fclose(fp);
			}
            break;
            /*End of w723v 2012-5-7 9:55 for by z00190439*/
            case ERROR_CODE_0012:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0012, acMsg);
            break;    
            case ERROR_CODE_0013:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0013, acMsg);
            break;
            case ERROR_CODE_0014:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0014, acMsg);
            break;
            case ERROR_CODE_0015:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0015, acMsg);
            break;
            case ERROR_CODE_0016:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0016, acMsg);
            break;
            case ERROR_CODE_0017:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0017, acMsg);
            break;
            case ERROR_CODE_0018:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0018, acMsg);
            break;
            case ERROR_CODE_0019:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0019, acMsg);
            break;
            case ERROR_CODE_0020:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0020, acMsg);
            break;
            case ERROR_CODE_0021:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0021, acMsg);
            break;
            case ERROR_CODE_0022:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0022, acMsg);
            break;
            case ERROR_CODE_0023:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0023, acMsg);
            break;
            case ERROR_CODE_0024:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0024, acMsg);
            break;
            case ERROR_CODE_0025:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0025, acMsg);
            break;
            case ERROR_CODE_0026:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0026, acMsg);
            break;
            case ERROR_CODE_0027:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0027, acMsg);
            break;
            case ERROR_CODE_0028:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0028, acMsg);
            break;
            case ERROR_CODE_0029:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0029, acMsg);
            break;
            case ERROR_CODE_0030:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0030, acMsg);
            break;
            case ERROR_CODE_0031:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0031, acMsg);
            break;
            case ERROR_CODE_0032:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0032, acMsg);
            break;
            case ERROR_CODE_0033:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0033, acMsg);
            break;
            case ERROR_CODE_0034:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0034, acMsg);
            break;
            case ERROR_CODE_0035:
            ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0035, acMsg);
            break;

            default:
                ulErr = 0;
                ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R001_56_0000, acMsg);
				fp = fopen(ErrCodeFilePath,"w+");
			    if(NULL != fp) {
				    fprintf(fp,"err_code_r56_none");
				    fclose(fp);
			    }
                break;

        }

        /*start:z00182249 for UDK0000287513：P003记录条件为当网关发的ipv6cp报文被服务器reject或NAK时 0130909*/
        //ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LOG_FAIL_WITH_INFO, acMsg);
		/*End:z00182249 for UDK0000287513：P003记录条件为当网关发的ipv6cp报文被服务器reject或NAK时20130909*/
    }
}

#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
static int get_always_on_v4_idle(void)
{
    int ulCurSipInterval = 0;

#ifdef SUPPORT_ATP_VOICE
    ulCurSipInterval = ATP_UTIL_VoiceGetSipInterval();
    if (0 == ulCurSipInterval)
    {
        ulCurSipInterval = SIP_INTERVAL_DEFAULT;
    }
#endif

    return (ulCurSipInterval + ALWAYSON_MODE_V4_IDLE_PLUS);
}

//把ip地址格式如20.20.20.122转换成020.020.020.122，以方便到bcm硬件加速/proc/文件中读取核对,判断硬加速是否在跑
void FormatBcmParseIP(const u_int32_t *ulAddr)
{
    u_int8_t *ucP = ulAddr;
    snprintf(ac_parse_bcm_ip, sizeof(ac_parse_bcm_ip), "%03d.%03d.%03d.%03d", ucP[0], ucP[1], ucP[2], ucP[3]);
    //printf("###### get ac_parse_bcm_ip=%s\n", ac_parse_bcm_ip);
}
#endif

#ifdef SUPPORT_ATP_PPP_IGMP_DIAL
/*START ADD:l00180987 2012-10-6 FOR DTS2012082503139*/
/******************************************************************************
  函数名称  : brlist_hitlist_reset_delete_flag
  功能描述  : 将所有节点置为待删除状态，后续只有与当前brlist匹配的才保留
  输入参数  : 
              无
  调用函数  : check_bcm_v4_hw_cache_idle
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2012-10-6
     作    者   : l00180987
     修改内容   : 完成初稿

******************************************************************************/
static void brlist_hitlist_reset_delete_flag()
{
    struct br_hit_list_st * pst_hit_curr_node = NULL;

    pst_hit_curr_node = g_br_hit_list_head;
    while(pst_hit_curr_node)
    {
        pst_hit_curr_node->delete_flag = 1;
        pst_hit_curr_node = pst_hit_curr_node->next;
    }
}


/******************************************************************************
  函数名称  : brlist_hitlist_delete
  功能描述  : 删除所有有delete标记的条目，过包为0或条目已不存在会被删除
  输入参数  : 
              无
  调用函数  : check_bcm_v4_hw_cache_idle
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2012-10-6
     作    者   : l00180987
     修改内容   : 完成初稿

******************************************************************************/

static void brlist_hitlist_delete()
{
    struct br_hit_list_st * pst_hit_curr_node = NULL;
    struct br_hit_list_st * pst_hit_pre_node = NULL;
    struct br_hit_list_st * pst_hit_del_node = NULL;

    pst_hit_curr_node = g_br_hit_list_head;
    pst_hit_pre_node = pst_hit_curr_node;
    
    while(pst_hit_curr_node)
    {
        if (pst_hit_curr_node->delete_flag)
        {
            /* delete head */
            if (pst_hit_curr_node == g_br_hit_list_head)
            {
                g_br_hit_list_head = pst_hit_curr_node->next;
                pst_hit_pre_node = pst_hit_curr_node->next;
                pst_hit_del_node = pst_hit_curr_node;
                pst_hit_curr_node = pst_hit_curr_node->next;
            }
            else/* delete not head */
            {
                pst_hit_pre_node->next = pst_hit_curr_node->next;
                pst_hit_del_node = pst_hit_curr_node;
                pst_hit_curr_node = pst_hit_curr_node->next;
            }

            free(pst_hit_del_node);
            pst_hit_del_node = NULL;
            continue;
        }

        pst_hit_pre_node = pst_hit_curr_node;
        pst_hit_curr_node = pst_hit_curr_node->next;
    }
}


/******************************************************************************
  函数名称  : brlist_hitlist_update
  功能描述  : 以flow object为key查找保存的链表，如果条目已存在且
             过包数 有变化则更新，如条目不存在则创建加入链表
  输入参数  : 
              1.  flow_object:flowobject匹配关键字
              2.  hit_num:flow object对应的新过包数
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 1:表项已存在且过包增加，或表项还不存在但有过包
              0: other

  修改历史      :
   1.日    期   : 2012-10-6
     作    者   : l00180987
     修改内容   : 完成初稿

******************************************************************************/

static int brlist_hitlist_update(char * flow_object, long hit_num)
{
    struct br_hit_list_st * pst_hit_node = NULL;
    struct br_hit_list_st * pst_hit_curr_node = NULL;

    pst_hit_curr_node = g_br_hit_list_head;
    while(pst_hit_curr_node)
    {
        if (0 == strcmp(pst_hit_curr_node->flow_object, flow_object))
        {
            /*brlist有过包*/
            if (hit_num && (pst_hit_curr_node->hit_num != hit_num))
            {
                pst_hit_curr_node->hit_num = hit_num;
                pst_hit_curr_node->delete_flag = 0;
                return 1;
            }

            /*当前flow无过包但有计数*/
            if (hit_num)
            {
                pst_hit_curr_node->delete_flag = 0;
            }
            
            return 0;
        }
        
        pst_hit_curr_node = pst_hit_curr_node->next;
    }

    /*list中未找到，hit_num非0则新增*/
    if (0 == hit_num)
    {
        return 0;
    }

    pst_hit_node = (struct br_hit_list_st *)malloc(sizeof(struct br_hit_list_st));
    if (NULL == pst_hit_node)
    {
        return 0;
    }

    memset(pst_hit_node, 0, sizeof(struct br_hit_list_st));
    snprintf(pst_hit_node->flow_object, sizeof(pst_hit_node->flow_object), "%s", flow_object);
    pst_hit_node->hit_num = hit_num;
    pst_hit_node->delete_flag = 0;
    pst_hit_node->next = NULL;

    if (NULL == g_br_hit_list_head)
    {
        g_br_hit_list_head = pst_hit_node;
        return 1;
    }

    pst_hit_node->next = g_br_hit_list_head;
    g_br_hit_list_head = pst_hit_node;
    return 1;
    
}


/******************************************************************************
  函数名称  : get_bcm_brlist_hwhit_num
  功能描述  : 根据输入行信息获取过包数
  输入参数  : 
              1.  *pacLine:输入行信息，来自brlist
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : brlist对应一条记录的当前过包数

  修改历史      :
   1.日    期   : 2012-10-6
     作    者   : l00180987
     修改内容   : 完成初稿

cat /proc/fcache/br_list
软加速时表项格式:
<flow id>                   <sw hitnum>
0x8c4e0300@00008    0:   484     179906: 249221632 0xffff          0  0x00000000  0x00000000 XPHY

硬加速时表项格式:  
<flow id>                                                <hw hitnum>
0x8c4e0300@00008    0:     0          0:         0 0x1012      79876  0x00000000  0x00000000 XPHY 

******************************************************************************/

static long get_bcm_brlist_hwhit_num(char *pacLine)
{
    long hwhit_num = 0;
    char hit_str[16];
    char *pstr = pacLine;

    memset(hit_str, 0, sizeof(hit_str));
    
    /*acLine的第59个字符开始的11个字符是hw hit number*/
    pstr += 58;
    snprintf(hit_str, 11, "%s", pstr);
    hwhit_num = atol(hit_str);
    /*hw hit没有,再看sw hit项目的计数*/
    if (0 == hwhit_num)
    {
        pstr = pacLine;
        /*acLine的第30个字符开始的11个字符是sw hit number*/
        pstr += 29;
        snprintf(hit_str, 11, "%s", pstr);
        hwhit_num = atol(hit_str);
    }

    return hwhit_num;
}
/*END ADD:l00180987 2012-10-6 FOR DTS2012082503139*/

static void check_bcm_v4_hw_cache_idle(void)
{
    char acCmd[128] = {0};
    char acLine[256] = {0};
    FILE *pstFile = NULL;
    struct timeval now;
	
    time_t ul_fcache_br_idle;
    int line;
    long br_hit;
    char br_fo[FCACHE_FLOWOBJECT_SIZE];
    int brlist_updated;

    ul_fcache_ipv4_idle = 0xFFFF;
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
    snprintf(acCmd, sizeof(acCmd), "cat %s > %s", BCM_FCACHE_FILE, FCACHE_FILE_TO_PARSE);
    system(acCmd);
    pstFile = fopen(FCACHE_FILE_TO_PARSE, "r");
    if (NULL == pstFile)
    {
        return;
    }
    while (fgets(acLine, sizeof(acLine), pstFile))
    {
        //printf("get nfList Line=%s\n", acLine);
        if (strstr(acLine, ac_parse_bcm_ip))
        {
            ATP_UTIL_GetSysTime(&last_hw_v4_xmit);
            break;
        }
    }
    fclose(pstFile);
    ATP_UTIL_GetSysTime(&now);
    ul_fcache_ipv4_idle = now.tv_sec - last_hw_v4_xmit.tv_sec;
    //printf("HW accelerating, fcache_ipv4_idle=%d\n", ul_fcache_ipv4_idle);
#endif

    /*START ADD:l00180987 2012-10-6 FOR DTS2012082503139*/
    /*add check brlist*/
    /*proxy 接口是ppp接口才检查*/
    pstFile = fopen(PPP_IGMP_MC_FILE, "r");
    if (NULL == pstFile)
    {
        goto timeout;
    }

    memset(acLine, 0, sizeof(acLine));
    if (NULL == fgets(acLine, sizeof(acLine), pstFile))
    {
        fclose(pstFile);
        goto timeout;
    }

    fclose(pstFile);

    /*ppp不是proxy接口*/
    if (0 == atoi(acLine))
    {
        goto timeout;
    }

    /*ppp 是proxy接口则检查brlist*/
    snprintf(acCmd, sizeof(acCmd), "cat %s > %s", BCM_FCACHE_BR_FILE, FCACHE_BR_FILE_TO_PARSE);
    system(acCmd);

    pstFile = fopen(FCACHE_BR_FILE_TO_PARSE, "r");
    if (NULL == pstFile)
    {
        goto timeout;
    }

    brlist_hitlist_reset_delete_flag();
    line = 0;
    brlist_updated = 0;
    while (fgets(acLine, sizeof(acLine), pstFile))
    {
        line++;
        
        /*skip title line*/
        if (FCACHE_FILE_TITLE_LINE_NUM >= line)
        {
            continue;
        }

        memset(br_fo, 0, sizeof(br_fo));
        snprintf(br_fo, FCACHE_FLOWOBJECT_SIZE, "%s", acLine);
            
        br_hit = get_bcm_brlist_hwhit_num(acLine);
        if (brlist_hitlist_update(br_fo, br_hit))
        {            
            brlist_updated = 1;
        }
    }
    fclose(pstFile);

    /*delete unfind or zero entry*/
    brlist_hitlist_delete();

    if (brlist_updated)
    {
        ATP_UTIL_GetSysTime(&last_hw_v4_br_xmit);
    }
    
    ATP_UTIL_GetSysTime(&now);
    ul_fcache_br_idle = now.tv_sec - last_hw_v4_br_xmit.tv_sec;
    
#ifdef SUPPORT_ATP_PPP_DT_TOCPE0056
    if (ul_fcache_br_idle < ul_fcache_ipv4_idle)
    {
        ul_fcache_ipv4_idle = ul_fcache_br_idle;
        
    }
#else
    ul_fcache_ipv4_idle = ul_fcache_br_idle;
#endif
    /*END ADD:l00180987 2012-10-6 FOR DTS2012082503139*/

timeout:
    TIMEOUT(check_bcm_v4_hw_cache_idle, NULL, CHECK_BCM_V4_HW_FLOW_INTERVAL);
}
#endif

#ifdef SUPPORT_ATP_MLD
/******************************************************************************
  函数名称  : mc6_list_reset_delete_flag
  功能描述  : 将所有节点置为待删除状态，后续只有与当前brlist匹配的才保留
  输入参数  : 
              无
  调用函数  : check_mc6_cache_idle
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无
******************************************************************************/
static void mc6_list_reset_delete_flag()
{
    struct mc6_list_st * pst_mc6_curr_node = NULL;

    pst_mc6_curr_node = g_mc6_list_head;
    while (pst_mc6_curr_node)
    {
        pst_mc6_curr_node->delete_flag = 1;
        pst_mc6_curr_node = pst_mc6_curr_node->next;
    }
}

/******************************************************************************
  函数名称  : mc6_list_delete
  功能描述  : 删除所有有delete标记的条目，过包为0或条目已不存在会被删除
  输入参数  : 
              无
  调用函数  : check_mc6_cache_idle
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无
******************************************************************************/
static void mc6_list_delete()
{
    struct mc6_list_st * pst_curr_node = NULL;
    struct mc6_list_st * pst_pre_node = NULL;
    struct mc6_list_st * pst_del_node = NULL;

    pst_curr_node = g_mc6_list_head;
    pst_pre_node = pst_curr_node;
    
    while(pst_curr_node)
    {
        if (pst_curr_node->delete_flag)
        {
            /* delete head */
            if (pst_curr_node == g_mc6_list_head)
            {
                g_mc6_list_head = pst_curr_node->next;
                pst_pre_node = pst_curr_node->next;
                pst_del_node = pst_curr_node;
                pst_curr_node = pst_curr_node->next;
            }
            else/* delete not head */
            {
                pst_pre_node->next = pst_curr_node->next;
                pst_del_node = pst_curr_node;
                pst_curr_node = pst_curr_node->next;
            }
            free(pst_del_node);
            pst_del_node = NULL;
            continue;
        }
        pst_pre_node = pst_curr_node;
        pst_curr_node = pst_curr_node->next;
    }
}


/******************************************************************************
  函数名称  : mc6_list_update
  功能描述  : 以mc6的group即v6组地址为key查找保存的链表，如果条目已存在且
             过包数 有变化则更新，如条目不存在则创建加入链表
  输入参数  : 
              1.  group:v6组播组地址
              2.  pkts:该组上的新过包数
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 1:表项已存在且过包增加，或表项还不存在但有过包
              0: other
******************************************************************************/
static int mc6_list_update(char *group, long pkts)
{
    struct mc6_list_st * pst_hit_node = NULL;
    struct mc6_list_st * pst_hit_curr_node = NULL;

    pst_hit_curr_node = g_mc6_list_head;
    while (pst_hit_curr_node)
    {
        if (0 == strcmp(pst_hit_curr_node->group, group))
        {
            /*mc6 list有过包*/
            if (pkts && (pst_hit_curr_node->pkts != pkts))
            {
                pst_hit_curr_node->pkts = pkts;
                pst_hit_curr_node->delete_flag = 0;
                return 1;
            }

            /*当前flow无过包但有计数*/
            if (pkts)
            {
                pst_hit_curr_node->delete_flag = 0;
            }
            
            return 0;
        }
        
        pst_hit_curr_node = pst_hit_curr_node->next;
    }

    /*list中未找到，hit_num非0则新增*/
    if (0 == pkts)
    {
        return 0;
    }

    pst_hit_node = (struct mc6_list_st *)malloc(sizeof(struct mc6_list_st));
    if (NULL == pst_hit_node)
    {
        return 0;
    }

    memset(pst_hit_node, 0, sizeof(struct mc6_list_st));
    snprintf(pst_hit_node->group, sizeof(pst_hit_node->group), "%s", group);
    pst_hit_node->pkts = pkts;
    pst_hit_node->delete_flag = 0;
    pst_hit_node->next = NULL;

    if (NULL == g_mc6_list_head)
    {
        g_mc6_list_head = pst_hit_node;
        return 1;
    }
    pst_hit_node->next = g_mc6_list_head;
    g_mc6_list_head = pst_hit_node;
    return 1;
}


/******************************************************************************
  函数名称  : get_mc6_hit_num
  功能描述  : 根据输入行信息获取过包数
  输入参数  : 
              1.  *pacLine:输入行信息，来自ip6_mr_cache
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : brlist对应一条记录的当前过包数
******************************************************************************/
static long get_mc6_hit_num(char *pacLine)
{
    long hit_num = 0;
    char hit_str[16];
    char *pstr = pacLine;

    memset(hit_str, 0, sizeof(hit_str));
    
    /*acLine的第83个字符开始的10个字符是pkts计数*/
    pstr += 82;
    snprintf(hit_str, 11, "%s", pstr);

    hit_num = atol(hit_str);

    return hit_num;
}

static void check_mc6_cache_idle(void)
{
    char acCmd[128] = {0};
    char acLine[256] = {0};
    FILE *pstFile = NULL;
    struct timeval now;
	
    time_t ul_fcache_br_idle;
    int line;
    long mc6_pkts;
    char v6_group[IP6_GROUP_ADDR_SIZE];
    int mc6_updated;

    ul_ip6mc_idle = 0xFFFF;

    /*proxy 接口是ppp接口才检查*/
    pstFile = fopen(PPP_IP6_MC_FILE, "r");
    if (NULL == pstFile)
    {
        goto nextcheck;
    }

    memset(acLine, 0, sizeof(acLine));
    if (NULL == fgets(acLine, sizeof(acLine), pstFile))
    {
        fclose(pstFile);
        goto nextcheck;
    }

    fclose(pstFile);

    /*ppp不是proxy接口*/
    if (0 == atoi(acLine))
    {
        goto nextcheck;
    }

    /*ppp 是proxy接口则检查brlist*/
    snprintf(acCmd, sizeof(acCmd), "cat %s > %s", IP6_MC_PROC_FILE, IP6_MC_FILE_TO_PARSE);
    system(acCmd);

    pstFile = fopen(IP6_MC_FILE_TO_PARSE, "r");
    if (NULL == pstFile)
    {
        goto nextcheck;
    }

    mc6_list_reset_delete_flag();
    line = 0;
    mc6_updated = 0;
    while (fgets(acLine, sizeof(acLine), pstFile))
    {
        line++;
        
        /*skip title line*/
        if (IP6_MC_TITLE_LINE_NUM >= line)
        {
            continue;
        }

        memset(v6_group, 0, sizeof(v6_group));
        snprintf(v6_group, sizeof(v6_group), "%s", acLine);
            
        mc6_pkts = get_mc6_hit_num(acLine);
        if (mc6_list_update(v6_group, mc6_pkts))
        {
            mc6_updated = 1;
        }
    }
    fclose(pstFile);

    /*delete unfind or zero entry*/
    mc6_list_delete();

    if (mc6_updated)
    {
        ATP_UTIL_GetSysTime(&last_v6_mc_xmit);
    }
    
    ATP_UTIL_GetSysTime(&now);
    ul_ip6mc_idle = now.tv_sec - last_v6_mc_xmit.tv_sec;
nextcheck:
    TIMEOUT(check_mc6_cache_idle, NULL, CHECK_V6_MC_INTERVAL);
}
#endif  /*end of ipv6 mld*/

#endif

//有sip通话，紧急通话，cwmp在进行，不能掉线
static int is_ppp_can_down(void)
{
#ifdef SUPPORT_ATP_FON
    int iCount = 0 ;
    FILE *pFile = NULL;
    char acLine[8] = {0};
#endif

#ifdef SUPPORT_ATP_VOICE
    if (ATP_UTIL_VoiceGetVoipStatus())
    {
        notice("PPP can't down! VoIP calling...\n");
        return 0;
    }
    if (ATP_UTIL_VoiceHasUrgCall())
    {
        notice("PPP can't down! UrgCall calling...\n");
        return 0;
    }
#endif

#ifdef SUPPORT_ATP_CWMP
#define ATP_CWMP_SESSION_FILE       "/var/cwmpssion"
    //文件存在，表示有cwmp会话正在进行，不能掉线
    if (0 == access(ATP_CWMP_SESSION_FILE, R_OK))
    {
        notice("PPP can't down! CWMP ongoing...\n");
        return 0;
    }
#endif

#ifdef SUPPORT_ATP_UPG
    //文件存在，表示有upg会话正在进行，不能掉线
    if (0 == access("/var/upgupgradeflag", R_OK))
    {
        notice("PPP can't down! UPG ongoing...\n");
        return 0;
    }
#endif

#ifdef SUPPORT_ATP_FON
    pFile = fopen(FON_SSID_USERS_NUMBER_FILE,  "r");
    if(pFile)
    {
        fgets(acLine, sizeof(acLine),  pFile);
        iCount = atoi(acLine);
        if (iCount > 0)
        {
            fclose(pFile);
			notice("PPP can't down! FON users connected...\n");
            return 0;
        }
        fclose(pFile);
    }
#endif

#ifdef SUPPORT_ATP_HYBRID_BONDING
    /*LTE >0,在使用，则不允许按需拨号掉线*/
    if (GetLteStatus() > 0)
    {
        return 0;
    }
#endif

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    //DT 规格 Always-On时，开了ddns，则v4不能掉线
#ifdef SUPPORT_ATP_DDNS
    if ((0 == access(DDNS_STATUS_FILE, R_OK)) && !demand)
    {
        notice("PPP can't down! DynDNS enabled...\n");
        return 0;
    }
#endif
    //DT 规格 Always-On时，开了port forwarding，则v4不能掉线
#ifdef SUPPORT_ATP_NATCMS_DT
    if ((0 == access(V4_PORT_FORWARDING_FILE, R_OK)) && !demand)
    {
        notice("PPP can't down! V4 port forwarding enabled...\n");
        return 0;
    }
#endif
#endif
    return 1;
}

#ifdef SUPPORT_ATP_HYBRID_BONDING
/*获取LTE使用状态*/
int GetLteStatus(void)
{
    int iState = 0 ;
    FILE *pFile = NULL;
    char acLine[8] = {0};

    pFile = fopen(LTE_STATUS_FILE,  "r");
    if (pFile)
    {
        fgets(acLine, sizeof(acLine),  pFile);
        iState = atoi(acLine);
        fclose(pFile);
    }
    return iState;
}
#endif

