/* pppoe.c - pppd plugin to implement PPPoE protocol.
 *
 * Copyright 2000 Michal Ostrowski <mostrows@styx.uwaterloo.ca>,
 *		  Jamal Hadi Salim <hadi@cyberus.ca>
 * Borrows heavily from the PPPoATM plugin by Mitchell Blank Jr.,
 * which is based in part on work from Jens Axboe and Paul Mackerras.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */

#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "pppoe.h"
#if _linux_
extern int new_style_driver;    /* From sys-linux.c */
#include "ppp_defs_linux.h"
#include "if_ppp_linux.h"
#else
#error this module meant for use with linux only at this time
#endif

/* start of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */
extern int iIsExist;
/* end of by d00107688 for AU4D02402 不启用绑定特定配置下other,internet按需拨号不生效 */

#include "pppd.h"
#include "fsm.h"
#include "lcp.h"
#include "ipcp.h"
#include "ccp.h"
#include "pathnames.h"

const char pppd_version[] = VERSION;

#define _PATH_ETHOPT         _ROOT_PATH "/etc/ppp/options."

#define PPPOE_MTU	1492
extern int kill_link;

bool	pppoe_server=0;
char	*pppoe_srv_name=NULL;
extern char pppoe_ac_name[AC_NAME_LEN];

char    *hostuniq = NULL;
int     retries = 0;

/* start SUPPORT_PPPOE2A */ 
int pppoe2a_flag = 0;
/*Start by Triductor zhengyu to define Triductor communication MAC*/
#ifdef SUPPORT_ATP_TRI_XDSL
char pppoe2a_mac[6] = {0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
#else
char pppoe2a_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif
/*End by Triductor zhengyu to define Triductor communication MAC*/
short pppoe2a_sid = 0x009a;
/* end SUPPORT_PPPOE2A */
int g_lMaxMtu = PPPOE_MTU;


static char *bad_options[] = {NULL};

int setdevname_pppoe(const char *cp);

static option_t pppoe_options[] = {
	{ "device name", o_wild, (void *) &setdevname_pppoe,
	  "Serial port device name",
	  OPT_DEVNAM | OPT_PRIVFIX | OPT_NOARG  | OPT_A2STRVAL | OPT_STATIC,
	  devnam},
	{ "pppoe_srv_name", o_string, &atp_pppoe_srv_name,
	  "PPPoE service name"},
	{ "pppoe_ac_name", o_string, &pppoe_ac_name,
	  "PPPoE access concentrator name"},
	{ "pppoe_hostuniq", o_string, &hostuniq,
	  "PPPoE client uniq hostid "},
	{ "pppoe_retransmit", o_int, &retries,
	  "PPPoE client number of retransmit tries"},
	{ "pppoe_server", o_bool, &pppoe_server,
	  "PPPoE listen for incoming requests",1},
	{ NULL }
};

int srv_init_ses(struct session *ses, char* from)
{
#if 0    
    int retval;
    retval = client_init_ses(ses, from);
    ses->init_disc = NULL;
    ses->rcv_pado  = NULL;
    ses->rcv_pads  = NULL;
    ses->rcv_padi  = srv_rcv_padi;
    ses->rcv_padr  = srv_rcv_padr;
    ses->rcv_padt  = srv_rcv_padt;

    /* retries forever */
    ses->retries   = -1;

    return retval;
#endif
    return 0;
}


struct session *ses = NULL;
static int connect_pppoe_ses(void)
{
    int err=-1;

    if( pppoe_server == 1 ){
	srv_init_ses(ses,devnam);
    }else{
	client_init_ses(ses,devnam);
    }

    
#if 0
    ses->np=1;  /* jamal debug the discovery portion */
#endif
    strlcpy(ppp_devnam, devnam, sizeof(ppp_devnam));

/* start SUPPORT_PPPOE2A */ 
    if (pppoe2a_flag)
    {
        ses->sp.sa_addr.pppoe.sid = htons(pppoe2a_sid);
        memcpy(ses->sp.sa_addr.pppoe.remote, pppoe2a_mac, ETH_ALEN);
        strcpy(ses->sp.sa_addr.pppoe.dev, devnam);
    }
    else
    {
        err= session_connect ( ses );

        if(err < 0)
        {
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V		
            if(PADI_fail_flag == 1)
            {
//如果是PADI阶段获取PADO失败，不需要走入channel_disconnect(会发送PADT)               
                return PADI_FAIL_RET;
            }
#endif			
            poe_fatal(ses,"Failed to negotiate PPPoE connection: %d %m",errno,errno);
            return 0;/*WAN <3.1.2Discover收到不可识别报文退出> porting by s60000658 20060504*/
        }
    }
/* end SUPPORT_PPPOE2A */ 

    poe_info(ses,"Connecting PPPoE socket: %E %04x %s %p",
	     ses->sp.sa_addr.pppoe.remote,
	     ses->sp.sa_addr.pppoe.sid,
	     ses->sp.sa_addr.pppoe.dev,ses);
    
    err = connect(ses->fd, (struct sockaddr*)&ses->sp,
		  sizeof(struct sockaddr_pppox));

    if( err < 0 ){
	poe_fatal(ses,"Failed to connect PPPoE socket: %d %m",errno,errno);
	return err;
    }
#if 0
    if (ses->np)
     	fatal("discovery complete\n");
#endif
    /* Once the logging is fixed, print a message here indicating
       connection parameters */

    return ses->fd;
}

static void disconnect_pppoe_ses(void)
{
    int ret;
    warn("Doing disconnect");

    session_disconnect(ses);

    ses->sp.sa_addr.pppoe.sid = 0;
    ret = connect(ses->fd, (struct sockaddr*)&ses->sp,
	    sizeof(struct sockaddr_pppox));

}

#if 0
static int setspeed_pppoe(const char *cp)
{
    return 0;
}
#endif

static void init_device_pppoe(void)
{
    struct filter *filt;
    ses=(void *)malloc(sizeof(struct session));
    if(!ses){
	fatal("No memory for new PPPoE session");
    }
    memset(ses,0,sizeof(struct session));

    if ((ses->filt=malloc(sizeof(struct filter))) == NULL) {
	poe_error (ses,"failed to malloc for Filter ");
	poe_die (-1);
    }

    filt=ses->filt;  /* makes the code more readable */
    memset(filt,0,sizeof(struct filter));

    if (strlen(pppoe_ac_name) != 0) {
	if (strlen (pppoe_ac_name) > 255) {
	    poe_error (ses," AC name too long (maximum allowed 256 chars)");
	    poe_die(-1);
	}
	ses->filt->ntag = make_filter_tag(PTT_AC_NAME,
					  strlen(pppoe_ac_name),
					  pppoe_ac_name);

	if ( ses->filt->ntag== NULL) {
	    poe_error (ses,"failed to malloc for AC name");
	    poe_die(-1);
	}

    }

//    if (atp_pppoe_srv_name!=NULL) {
    if (strlen (atp_pppoe_srv_name) > 0) {
	if (strlen (atp_pppoe_srv_name) > 255) {
	    poe_error (ses," Service name too long (maximum allowed 256 chars)");
	    poe_die(-1);
	}
	ses->filt->stag = make_filter_tag(PTT_SRV_NAME,
					  strlen(atp_pppoe_srv_name),
					  atp_pppoe_srv_name);
	if ( ses->filt->stag == NULL) {
	    poe_error (ses,"failed to malloc for service name");
	    poe_die(-1);
	}
    }

    if (hostuniq) {
	ses->filt->htag = make_filter_tag(PTT_HOST_UNIQ,
					  strlen(hostuniq),
					  hostuniq);
	if ( ses->filt->htag == NULL) {
	    poe_error (ses,"failed to malloc for Uniq Host Id ");
	    poe_die(-1);
	}
    }

    if (retries) {
	ses->retries=retries;
    }

    memcpy( ses->name, devnam, IFNAMSIZ);
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
    memcpy( ses->name_bak, devnam_bak, IFNAMSIZ);
#endif
    ses->opt_debug=1;
}

static void pppoe_extra_options()
{
    char buf[256];
    snprintf(buf, 256, _PATH_ETHOPT "%s",devnam);
    if(!options_from_file(buf, 0, 0, 1))
	exit(EXIT_OPTION_ERROR);

}



static void send_config_pppoe(int mtu,
			      u_int32_t asyncmap,
			      int pcomp,
			      int accomp)
{
    int sock;
    struct ifreq ifr;

    if (mtu > g_lMaxMtu) 
    {
    	warn("Couldn't increase MTU to %d.", mtu);
    	mtu = g_lMaxMtu;
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
	fatal("Couldn't create IP socket: %m");
    strlcpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
    ifr.ifr_mtu = mtu;
    if (ioctl(sock, SIOCSIFMTU, (caddr_t) &ifr) < 0)
	fatal("ioctl(SIOCSIFMTU): %m");
    (void) close (sock);
}


static void recv_config_pppoe(int mru,
			      u_int32_t asyncmap,
			      int pcomp,
			      int accomp)
{
    if (mru > g_lMaxMtu)
	error("Couldn't increase MRU to %d", mru);
}


struct channel pppoe_channel;
/* Check is cp is a valid ethernet device
 * return either 1 if "cp" is a reasonable thing to name a device
 * or die.
 * Note that we don't actually open the device at this point
 * We do need to fill in:
 *   devnam: a string representation of the device
 */

int (*old_setdevname_hook)(const char* cp) = NULL;
int setdevname_pppoe(const char *cp)
{
    int ret;
    char dev[IFNAMSIZ+1];
    int addr[ETH_ALEN];
    int sid;

    ret =sscanf(cp, FMTSTRING(IFNAMSIZ),addr, addr+1, addr+2,
		addr+3, addr+4, addr+5,&sid,dev);
    if( ret != 8 ){

	ret = get_sockaddr_ll(cp,NULL);
        if (ret < 0)
	    fatal("PPPoE: Cannot create PF_PACKET socket for PPPoE discovery\n");
	if (ret == 1)
	    strncpy(devnam, cp, sizeof(devnam));
    }else{
	/* long form parsed */
	ret = get_sockaddr_ll(dev,NULL);
        if (ret < 0)
	    fatal("PPPoE: Cannot create PF_PACKET socket for PPPoE discovery\n");

	strncpy(devnam, cp, sizeof(devnam));
	ret = 1;
    }
	
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
	if((ret==1)&&(0 != devnam_bak[0]))
    {
        ret =sscanf(devnam_bak, FMTSTRING(IFNAMSIZ),addr, addr+1, addr+2,
    		addr+3, addr+4, addr+5,&sid,dev);
        if( ret != 8 ){

    	ret = get_sockaddr_ll(devnam_bak,NULL);
            if (ret < 0)
    	    fatal("PPPoE: Cannot create PF_PACKET socket for PPPoE discovery\n");

        }else{
    	/* long form parsed */
    	ret = get_sockaddr_ll(dev,NULL);
            if (ret < 0)
    	    fatal("PPPoE: Cannot create PF_PACKET socket for PPPoE discovery\n");
	    	ret = 1;
        }
    }
#endif



    if( ret == 1 && the_channel != &pppoe_channel ){

	the_channel = &pppoe_channel;

	{
	    char **a;
	    for (a = bad_options; *a != NULL; a++)
		remove_option(*a);
	}

	lcp_allowoptions[0].neg_accompression = 0;
	lcp_wantoptions[0].neg_accompression = 0;

	lcp_allowoptions[0].neg_asyncmap = 0;
	lcp_wantoptions[0].neg_asyncmap = 0;

	lcp_allowoptions[0].neg_pcompression = 0;
	lcp_wantoptions[0].neg_pcompression = 0;
    
#if 0
	ccp_allowoptions[0].deflate = 0 ;
	ccp_wantoptions[0].deflate = 0 ;
#endif

	ipcp_allowoptions[0].neg_vj=0;
	ipcp_wantoptions[0].neg_vj=0;

    ipcp_allowoptions[0].default_route = 0 ;
	ipcp_wantoptions[0].default_route = 0 ;

#if 0
	ccp_allowoptions[0].bsd_compress = 0;
	ccp_wantoptions[0].bsd_compress = 0;
#endif

	init_device_pppoe();
    }
    return ret;
}



void plugin_init(void)
{
/*
  fatal("PPPoE plugin loading...");
*/

#if _linux_
    if (!ppp_available() && !new_style_driver)
	fatal("Kernel doesn't support ppp_generic needed for PPPoE");
#else
    fatal("No PPPoE support on this OS");
#endif
    add_options(pppoe_options);


    info("PPPoE Plugin Initialized");
}

struct channel pppoe_channel = {
    options: pppoe_options,
    process_extra_options: &pppoe_extra_options,
    check_options: NULL,
    connect: &connect_pppoe_ses,
    disconnect: &disconnect_pppoe_ses,
    establish_ppp: &generic_establish_ppp,
    disestablish_ppp: &generic_disestablish_ppp,
    send_config: &send_config_pppoe,
    recv_config: &recv_config_pppoe,
    close: NULL,
    cleanup: NULL
};

