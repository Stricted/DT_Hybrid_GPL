/*
    Copyright (C) 2002-2005  Thomas Ries <tries@gmx.net>

    This file is part of Siproxd.
    
    Siproxd is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    Siproxd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with Siproxd; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

/* $Id: siproxd.h,v 1.7 2009/06/20 01:40:57 y42304 Exp $ */
#ifndef _SIPROXD_H_
#define _SIPROXD_H_

#ifdef DMALLOC
 #include <dmalloc.h>
#endif
#include "atpconfig.h"

#define SIP_BIND        1
#define SIP_DEBUG       0
#define TOS_RCV

#if SIP_DEBUG
#define printsip(format, args...)  printf("%s, %s, %d " format, __FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define printsip(format, args...)
#endif

/* Start For Sip Bind & PPPoEProxy */
/* ATP 2008.01.26 ATPV100R002C01B021 AU4D00367 */

#define UNREG_WITHOUT_REG_HG522 1

#define VOICE_DSL_INTF_NAME     "ppp256"
#define VOICE_LTE_TUNNEL_NAME   "gre2"
#define VOICE_TUNNNEL_PREFIX    "gre"
#define VOICE_TUNNNEL_ALL       "gre+"

#define IP_RECVMARK  55
#define IP_MARK  56


#define BUFFER_SIZE	8196	/* input buffer for read from socket	*/
#define RTP_BUFFER_SIZE	1520 + 80	/* max size of an RTP frame		*/
#define URL_STRING_SIZE	128	/* max size of an URL/URI string	*/
#define STATUSCODE_SIZE 5	/* size of string representation of status */
#define DNS_CACHE_SIZE  32	/* number of entries in internal DNS cache */
#define DNS_MAX_AGE	60	/* maximum age of an cache entry (sec)	*/
#define IFADR_CACHE_SIZE 32	/* number of entries in internal IFADR cache */
#define IFADR_MAX_AGE	5	/* max. age of the IF address cache (sec) */
#define IFNAME_SIZE	16	/* max string length of a interface name */
#define HOSTNAME_SIZE	64	/* max string length of a hostname	*/
#define USERNAME_SIZE	64	/* max string length of a username (auth) */
#define PASSWORD_SIZE	64	/* max string length of a password (auth) */
#define VIA_BRANCH_SIZE 64	/* max string length for via branch param */





/* start of add by d00107688 SIP ALG 中本宏需要打开而不能够依赖于atpconfig.h 2009-06-20 */
#ifndef SUPPORT_ATP_BRIDGE_BIND
#define SUPPORT_ATP_BRIDGE_BIND
#endif
/* end of add by d00107688 SIP ALG 中本宏需要打开而不能够依赖于atpconfig.h 2009-06-20 */

#ifdef SUPPORT_ATP_BRIDGE_BIND

#define SIP_BIND_LEN    64
#define SIP_IPADDR      16

void SipReadIpAddr();
int IpisInNet(char* pszIp, char* pszNetIp, char* pszNetMask);
int parseBind(struct sockaddr_in from);

#endif
/* End For Sip Bind & PPPoEProxy */


/* start of add by d00107688 to support QoS mark 2008-9-28 增加了标记用来区分到底创建socket时是为SIP
 * 创建还是为 RTP 创建
 */
#ifdef SUPPORT_ATP_QOS
#define SOCK_CREATE_BY_SIP  1
#define SOCK_CREATE_BY_RTP  2
#define SOCK_CREATE_BY_RTCP 3

int ATP_SIP_ALG_QoS_SetMark(int ulSockfd, int nfmark);

#endif
/* end of add by d00107688 to support QoS mark 2008-9-28 增加了标记用来区分到底创建socket时是为SIP
 * 创建还是为 RTP 创建
 */

typedef enum tagSIP_FW_COMMOND_TYPE
{
    FW_SIP_CMD_TYPE_OPEN = 0,       //insert ACCEPT rule
    FW_SIP_CMD_TYPE_REMOVE = 1,    //remove ACCEPT rule
}SIP_FW_COMMAND_TYPE_EN;

/*
 * table to hold the client registrations
 */
struct urlmap_s {
   int  active;
   int  expires;
   osip_uri_t *true_url;	// true URL of UA  (inbound URL) 总是指向conntact字段
   osip_uri_t *masq_url;	// masqueraded URL (outbound URL)
   osip_uri_t *reg_url;		// registered URL  (masq URL as wished by UA)
#if UNREG_WITHOUT_REG_HG522
   char       *to_ip;
#endif
   /* DT c40163 add for STUN */
   struct sockaddr_in from;
   /* DT c40163 end  */
   
   /* DT c40163 add for QOS */
   int t_online;
   /* DT c40163 end  */

   int wan_socket_fd; /*每个sip注册单独使用一个wan socket,绑定到不同的wan源端口*/
   int wan_port;  /*wan_port,上述socket的本地源端口号*/
   char outbound_if[IFNAME_SIZE];       /* 报文从接口转发，WAN侧 */
   struct in_addr outbound_addr;       /* 报文从接口转发，WAN侧 */
   int  incall;
};
/*
 * the difference between masq_url and reg_url is, 
 * the reg URL *always* holds the url registered by the UA.
 * the masq_url may contain a different URL due to an additional
 * masquerading feature (mask_host, masked_host config options)
 */


/*
 * Array of strings - used within configuration store
 */
#define CFG_STRARR_SIZE		128	/* max 128 entries in array */
typedef struct {
   int  used;
   char *string[CFG_STRARR_SIZE];
} stringa_t;

/*
 * configuration option table
 */
struct siproxd_config {
   int debuglevel;
   int debugport;
   char *inbound_if;        /* 报文进来的接口，LAN侧 */
   char *outbound_if;       /* 报文从该端口转发，WAN侧 */
   int sip_listen_port;
   int daemonize;
   int silence_log;
   int rtp_port_low;
   int rtp_port_high;
   int rtp_timeout;
   int rtp_proxy_enable;
   char *user;
   char *chrootjail;
   char *hosts_allow_reg;
   char *hosts_allow_sip;
   char *hosts_deny_sip;
   char *proxy_auth_realm;
   char *proxy_auth_passwd;
   char *proxy_auth_pwfile;
   stringa_t mask_host;
   stringa_t masked_host;
   char *outbound_proxy_host;
   int  outbound_proxy_port;
   stringa_t outbound_proxy_domain_name;
   stringa_t outbound_proxy_domain_host;
   stringa_t outbound_proxy_domain_port;
   char *registrationfile;
   int  log_calls;
   char *pid_file;
   int  default_expires;
};

/*
 * SIP ticket
 */
#define PROTO_UNKN -1
#define PROTO_UDP  1
#define PROTO_TCP  2
   
#define REQTYP_INCOMING		1
#define REQTYP_OUTGOING		2
#define RESTYP_INCOMING		3
#define RESTYP_OUTGOING		4
typedef struct {
   osip_message_t *sipmsg;	/* SIP */
   struct sockaddr_in from;	/* received from */
   int protocol;		/* received by protocol */
   int direction;		/* direction as determined by proxy */
   /* DT c40163 add for QOS */
   int tos;
   unsigned int ulMark;
   /* DT c40163 end */
   /* BEGIN: Added by l00183184, 2013/6/27   PN:sipalg 不解析domain，直接获取dst addr */
   struct sockaddr_in to;	/* received from */
   /* END:   Added by l00183184, 2013/6/27   PN:sipalg 不解析domain，直接获取dst addr */
} sip_ticket_t;

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
extern int wanport;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

extern int ATP_UTIL_ForkProcess(const char *pcCmd, int lWaitMiliSecs,
                                            int *plExitSignal, int *plExitCode);

/*
 * Function prototypes
 */

/*				function returns STS_* status values     vvv */

/* sock.c */
int sipsock_listen(void);						/*X*/
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
int sipsock_wait(int *fd);
/* DT c40163 modify for QOS
int sipsock_read(int fd, void *buf, size_t bufsize,
                 struct sockaddr_in *from, int *protocol);
*/
int sipsock_read(int fd, void *buf, size_t bufsize,
       struct sockaddr_in *from, int *protocol, int *tos, unsigned int *ulMark) ;
/* DT c40163 end */

   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

/* DT c40163 modify for QOS
int sipsock_send(struct in_addr addr, int port,	int protocol,			
                 char *buffer, int size);
*/
//增加map_index,向wan发包时，需要为每个pc选一个map里面的wan socket句柄
int sipsock_send(struct in_addr addr, int port,	int protocol,			
                 char *buffer, int size, int t_online, int direct, int tos, int map_index);
/* DT c40163 end */

#ifdef SUPPORT_ATP_QOS
int sockbind(struct in_addr ipaddr, int localport, int errflg, int sockflag);
#else
int sockbind(struct in_addr ipaddr, int localport, int errflg);
#endif

//从56005开始，给每个pc分配一个单独的wan端口
int get_wan_socket(char *ifname, int *plWanPort);

/* register.c */
void register_init(void);
void register_shut(void);
int  register_client(sip_ticket_t *ticket, int force_lcl_masq);		/*X*/
void register_agemap(void);
int  register_response(sip_ticket_t *ticket, int flag);			/*X*/

/* proxy.c */
int proxy_request (sip_ticket_t *ticket);				/*X*/
int proxy_response (sip_ticket_t *ticket, int fd);				/*X*/


/* start of AU4D01936 by d00107688 2009-06-19 sip电话通过pppoe proxy接入网关获取地址，sip alg正常但通话过程中，网关开端口出错，导致RTP流不能互通 */
#if 0
int proxy_rewrite_invitation_body(osip_message_t *m, int direction,
				  struct in_addr *local_ip);            /*X*/
#else
/* DT c40163 modify
int proxy_rewrite_invitation_body(int idex, osip_message_t *m, int direction,
				  struct in_addr *local_ip); 
*/
int proxy_rewrite_invitation_body(int idex, osip_message_t *mymsg, int direction,
                  struct in_addr *local_ip, struct in_addr *from_ip, int wan_port);
/* DT c40163 end */
#endif
/* end of AU4D01936 by d00107688 2009-06-19 sip电话通过pppoe proxy接入网关获取地址，sip alg正常但通话过程中，网关开端口出错，导致RTP流不能互通 */

int proxy_rewrite_request_uri(osip_message_t *mymsg, int idx);		/*X*/

/* route_preprocessing.c */
int route_preprocess(sip_ticket_t *ticket);				/*X*/
int route_add_recordroute(sip_ticket_t *ticket);			/*X*/
int route_purge_recordroute(sip_ticket_t *ticket);			/*X*/
int route_postprocess(sip_ticket_t *ticket);				/*X*/
int route_determine_nexthop(sip_ticket_t *ticket,
                            struct in_addr *dest, int *port);		/*X*/

//Modify by z00182249:一线反馈sip报文路由域丢失导致服务器不认可，注册成功 2013/12/24
int route_get_nexthop(sip_ticket_t *ticket,
                            struct in_addr *dest, int *port);

/* utils.c */
int  get_ip_by_host(char *hostname, struct in_addr *addr);		/*X*/
void secure_enviroment (void);
int  get_ip_by_ifname(char *ifname, struct in_addr *retaddr);		/*X*/
char *utils_inet_ntoa(struct in_addr in);
int  utils_inet_aton(const char *cp, struct in_addr *inp);
void get_local_ip(char *ip, struct in_addr *local_ip, int idx);

/* sip_utils.c */
osip_message_t * msg_make_template_reply (sip_ticket_t *ticket, int code);
int  check_vialoop (sip_ticket_t *ticket);				/*X*/
int  is_via_local (osip_via_t *via, struct in_addr *local_ip, int local_port);		/*X*/
int  compare_url(osip_uri_t *url1, osip_uri_t *url2);			/*X*/
int  compare_callid(osip_call_id_t *cid1, osip_call_id_t *cid2);	/*X*/
int  is_sipuri_local (sip_ticket_t *ticket);				/*X*/
int  check_rewrite_rq_uri (osip_message_t *sip);			/*X*/
int  sip_gen_response(sip_ticket_t *ticket, int code);			/*X*/
#define IF_OUTBOUND 0
#define IF_INBOUND  1
int sip_add_myvia (sip_ticket_t *ticket, int interface,
		   struct in_addr *local_ip, int local_port, int idx);		                /*X*/
int  sip_del_myvia (sip_ticket_t *ticket, struct in_addr *local_ip, int local_port);/*X*/
int  sip_rewrite_contact (sip_ticket_t *ticket, int direction,
			  struct in_addr *local_ip, int index);	                /*X*/
int  sip_calculate_branch_id (sip_ticket_t *ticket, char *id);		/*X*/
int  sip_find_outbound_proxy(sip_ticket_t *ticket, struct in_addr *addr,
                             int *port);				/*X*/
/* DT c40163 add for QOS */
//int is_t_online(osip_uri_t *req_url, int ind_urlmap);
int is_t_online(osip_message_t *msg, int ind_urlmap);
/* DT c40163 end */

/* readconf.c */
int read_config(char *name, int search);				/*X*/
int make_default_config(void);						/*X*/

/* rtpproxy.c */
int  rtpproxy_init( void );						/*X*/

/* DT c40163 modify for QOS
int  rtp_start_fwd (osip_call_id_t *callid, char *client_id, 
                    int direction, int media_stream_no,
		    struct in_addr outbound_ipaddr, int *outboundport,
                    struct in_addr lcl_client_ipaddr, int lcl_clientport);
*/
int  rtp_start_fwd (osip_call_id_t *callid, char *client_id, 
                    int direction, int media_stream_no,
		            struct in_addr outbound_ipaddr, int *outboundport,
                    struct in_addr lcl_client_ipaddr, int lcl_clientport,
                    int t_online, int wan_port);
/* DT c40163 end */

int  rtp_stop_fwd (osip_call_id_t *callid, int direction, int wan_port);		/*X*/
void rtpproxy_kill( void );						/*X*/

/* accessctl.c */
int  accesslist_check(struct sockaddr_in from);

/* security.c */
int  security_check_raw(char *sip_buffer, int size);			/*X*/
int  security_check_sip(sip_ticket_t *ticket);				/*X*/

/* auth.c */
int  authenticate_proxy(sip_ticket_t *ticket);				/*X*/
int  auth_include_authrq(sip_ticket_t *ticket);				/*X*/
void CvtHex(char *hash, char *hashstring);

/* fwapi.c */
int fwapi_start_rtp(int rtp_direction,
                    struct in_addr local_ipaddr, int local_port,
                    struct in_addr remote_ipaddr, int remote_port);
int fwapi_stop_rtp(int rtp_direction,
                   struct in_addr local_ipaddr, int local_port,
                   struct in_addr remote_ipaddr, int remote_port);

void proxy_control_wan_port(SIP_FW_COMMAND_TYPE_EN EN_ACTION, int iWanPort, int idex);

#if defined(SUPPORT_ATP_SIPPXY_RECORDDSTIP)
#define SYS_IP_LENGTH                   16
#define SYS_IP_BYTE_FOUR                3
#define SYS_BASE_DEC                    10
#define SYS_IP_MAX_NUMBER               255
#define SYS_IP_BYTE                     4
#define MAX_CALLID_LEN                  256
struct SipRouteInfoItem
{
    struct in_addr daddr;
    uint16_t  dport;
    uint32_t  proto;
    char *call_id;
    int call_id_len;
};

int getPrxoyDstIp(const osip_message_t *sipMsg, struct in_addr *dstIp);
int ValidateIpAddress(const char *pcIpAddr);
#endif
/*
 * some constant definitions
 */
#define SIP_PORT	5060	/* default port to listen */
#define DEFAULT_MAXFWD	70	/* default Max-Forward count */

/* DT c40163 modify. Default expires set to 900 seconds */
#if 0
#define DEFAULT_EXPIRES	3600	/* default Expires timeout */
#endif
#define DEFAULT_EXPIRES	900 	/* default Expires timeout */
/* DT c40163 end */

#define URLMAP_SIZE	32	/* number of URL mapping table entries	*/
#define RTPPROXY_SIZE	64	/* number of rtp proxy entries		*/


				/* scratch buffer for gethostbyname_r() */
#if defined(PR_NETDB_BUF_SIZE)
   #define GETHOSTBYNAME_BUFLEN PR_NETDB_BUF_SIZE 
#else
   #define GETHOSTBYNAME_BUFLEN 1024
#endif

/* constants for security testing */
#define SEC_MINLEN	16	/* minimum received length */
#define SEC_MAXLINELEN	1024	/* maximum acceptable length of one line
				   in the SIP telegram (security check)
                                   Careful: Proxy-Authorization lines may
                                   get quite long */

/* symbols for access control */
#define ACCESSCTL_SIP	1	/* for access control - SIP allowed	*/
#define ACCESSCTL_REG	2	/* --"--              - registr. allowed */

/* symbolic return stati */
#define STS_SUCCESS	0	/* SUCCESS				*/
#define STS_TRUE	0	/* TRUE					*/
#define STS_FAILURE	1	/* FAILURE				*/
#define STS_FALSE	1	/* FALSE				*/
#define STS_NEED_AUTH	1001	/* need authentication			*/
#define ALG_TRUE	1	/* TRUE					*/
#define ALG_FALSE	0	/* FALSE     			*/

/* symbolic direction of data */
#define DIR_INCOMING    1
#define DIR_OUTGOING    2

/* various */
#ifndef satoi
#define satoi atoi  /* used in libosips MSG_TEST_CODE macro ... */
#endif

#endif
