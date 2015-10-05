/* pppoatm.c - pppd plugin to implement PPPoATM protocol.
 *
 * Copyright 2000 Mitchell Blank Jr.
 * Based in part on work from Jens Axboe and Paul Mackerras.
 * Updated to ppp-2.4.1 by Bernhard Kaindl
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "pppd.h"
#include "pathnames.h"
#include "fsm.h" /* Needed for lcp.h to include cleanly */
#include "lcp.h"
#include "lib/atm.h"
#include <linux/atmdev.h>
#include <linux/atmppp.h>
#include <sys/stat.h>
#include <net/if.h>
#include <sys/ioctl.h>

//const char pppd_version[] = VERSION;

int fd_pppoa;
struct sockaddr_atmpvc pvcaddr;
static char *qosstr = NULL;

#if 0
static int pppoa_fd=0;
static int pppoatm_accept = 0;
#endif
bool llc_encaps = 0;
bool vc_encaps = 0;
static int device_got_set = 0;
static int pppoatm_max_mtu, pppoatm_max_mru;
struct channel pppoatm_channel;

int setdevname_pppoatm(const char *cp);
//static int setdevname_pppoatm(const char *cp);
static void set_line_discipline_pppoatm(int fd);
// brcm end
/* pppoa qos */
int pppoa_qos = -1;
int pppoa_pcr = -1;
int pppoa_mbs = -1;
int pppoa_scr = -1;

/* Add by w69233 for support PPPoA QoS configuration */
// Values for ATM_SET_CONFIG ulATMQoS.
#define    ATM_SC_OTHER               0
#define    ATM_SC_UBR                 1
#define    ATM_SC_UBR_PCR             2
#define    ATM_SC_CBR                 3
#define    ATM_SC_NRT_VBR             4
#define    ATM_SC_RT_VBR              5

/* Traffic description */

#define ATM_NONE	0		/* no traffic */
#define ATM_UBR		1
#define ATM_CBR		2
#define ATM_VBR		3
#define ATM_VBR_NRT	ATM_VBR
#define ATM_ABR		4
#define ATM_ANYCLASS	5		/* compatible with everything */
#define ATM_VBR_RT	6
#define ATM_UBR_PLUS	7

#define ATM_MAX_PCR	-1		/* maximum available PCR */

/* End of add */

#if 0

static option_t pppoa_options[] = {
	{ "device name", o_wild, (void *) &setdevname_pppoatm,
	  "ATM service provider IDs: VPI.VCI",
	  OPT_DEVNAM | OPT_PRIVFIX | OPT_NOARG  | OPT_A2STRVAL | OPT_STATIC,
	  devnam},
#if 0
	{ "accept", o_bool, &pppoatm_accept,
	  "set PPPoATM socket to accept incoming connections", 1 },
#endif
	{ "llc-encaps", o_bool, &llc_encaps,
	  "use LLC encapsulation for PPPoATM", 1},
	{ "vc-encaps", o_bool, &vc_encaps,
	  "use VC multiplexing for PPPoATM (default)", 1},
	{ "qos", o_string, &qosstr,
	  "set QoS for PPPoATM connection", 1},
	{ NULL }
};
#endif

/* returns:
 *  -1 if there's a problem with setting the device
 *   0 if we can't parse "cp" as a valid name of a device
 *   1 if "cp" is a reasonable thing to name a device
 * Note that we don't actually open the device at this point
 * We do need to fill in:
 *   devnam: a string representation of the device
 *   devstat: a stat structure of the device.  In this case
 *     we're not opening a device, so we just make sure
 *     to set up S_ISCHR(devstat.st_mode) != 1, so we
 *     don't get confused that we're on stdin.
 */
int setdevname_pppoatm(const char *cp)
{
	struct sockaddr_atmpvc addr;
//	extern struct stat devstat;
	if (device_got_set)
		return 0;
	//info("PPPoATM setdevname_pppoatm: '%s'", cp);
	memset(&addr, 0, sizeof addr);
	if (text2atm(cp, (struct sockaddr *) &addr, sizeof(addr),
	    T2A_PVC | T2A_NAME) < 0) {
//               if(doit)
//                   info("atm does not recognize: %s", cp);
		return 0;
           }
//	if (!doit) return 1;
	//if (!dev_set_ok()) return -1;
	memcpy(&pvcaddr, &addr, sizeof pvcaddr);
	strlcpy(devnam, cp, sizeof devnam);
//	devstat.st_mode = S_IFSOCK;
		the_channel = &pppoatm_channel;
		info("PPPoATM setdevname - remove unwanted options");
	//modem = 0;
	lcp_wantoptions[0].neg_accompression = 0;
	lcp_allowoptions[0].neg_accompression = 0;
	lcp_wantoptions[0].neg_asyncmap = 0;
	lcp_allowoptions[0].neg_asyncmap = 0;
	lcp_wantoptions[0].neg_pcompression = 0;

	info("PPPoATM setdevname_pppoatm - SUCCESS:%s", cp);
	device_got_set = 1;
	return 1;
}

#define pppoatm_overhead() (llc_encaps ? 6 : 2)

// brcm2
static void disconnect_pppoatm(int fd)
{
#if 1
   /* For 4.x xtm driver */
   struct atm_backend_ppp be;

   be.backend_num = 6;
   if (ioctl(fd, ATM_SETBACKEND, &be) < 0)
      fatal("ioctl(ATM_SETBACKEND): %m");
#else
   /* For 3.x atm driver */
	unsigned char ppp_disconn[64];
        unsigned char * outp;

	outp = ppp_disconn;
	MAKEHEADER(outp, PPP_LCP);	// 2 bytes
	PUTCHAR(5, outp);		// TERMREQ==5 			// 1 byte
	PUTCHAR(2, outp);  		// id=02			// 1 byte
    PUTSHORT(4, outp);		// HEADERLEN==4 in fsm.h	// 2 byte
	
	write(fd, ppp_disconn+2, 6);
#endif
}
static void no_device_given_pppoatm(void)
{
	fatal("No vpi.vci specified");
}
static int open_device_pppoatm(void)
{
	int fd;
	struct atm_qos qos;
	int i = 0;

	if (!device_got_set)
		no_device_given_pppoatm();
	fd = socket(AF_ATMPVC, SOCK_DGRAM, 0);
	if (fd < 0)
		fatal("failed to create socket: %m");
	// brcm
	fd_pppoa = fd;
	memset(&qos, 0, sizeof qos);
	qos.txtp.traffic_class = qos.rxtp.traffic_class = ATM_UBR;
	/* TODO: support simplified QoS setting */
	if (qosstr != NULL)
		if (text2qos(qosstr, &qos, 0))
			fatal("Can't parse QoS: \"%s\"");
	qos.txtp.max_sdu = lcp_allowoptions[0].mru + pppoatm_overhead();
	qos.rxtp.max_sdu = lcp_wantoptions[0].mru + pppoatm_overhead();
	qos.aal = ATM_AAL5;
	if (setsockopt(fd, SOL_ATM, SO_ATMQOS, &qos, sizeof(qos)) < 0)
		fatal("setsockopt(SO_ATMQOS): %m");
	/* TODO: accept on SVCs... */
	if (connect(fd, (struct sockaddr *) &pvcaddr,
	    sizeof(struct sockaddr_atmpvc)))
		fatal("connect(%s): %m", devnam);
	pppoatm_max_mtu = lcp_allowoptions[0].mru;
	pppoatm_max_mru = lcp_wantoptions[0].mru;
// brcm begin
	set_line_discipline_pppoatm(fd);
// brcm end

/* start of by d00107688 2010-09-27  当页面手动disconnect的时候会发送5个终止报文给BRASE */
#ifdef SUPPORT_ATP_PPP_REDISCON_BT_HHUB3
    if (req_rediscon) {
        usleep(300000);
        disconnect_pppoatm(fd);
        usleep(100000);
        req_rediscon = 0;
    }
#else
	if (req_rediscon) {
    	    while (i++ < 5) {
    		usleep(300000);
    		disconnect_pppoatm(fd);
	    }
	    usleep(100000);
	}
#endif
    /* End of by d00107688 2010-09-27 当页面手动disconnect的时候会发送5个终止报文给BRASE */

	return fd;
}

// brcm begin
static void close_device_pppoatm(void)
{
//    close(fd);
   if (fd_pppoa > 0) {
      struct atm_backend_ppp be;
      be.backend_num = 7;
      if (ioctl(fd_pppoa, ATM_SETBACKEND, &be) < 0)
         fatal("ioctl(ATM_SETBACKEND): %m");
      close(fd_pppoa);
      fd_pppoa= -1;
   }
}
// brcm end
static void set_line_discipline_pppoatm(int fd)
{
	struct atm_backend_ppp be;
	be.backend_num = 5;
	if (!llc_encaps)
		be.encaps = PPPOATM_ENCAPS_VC;
	else if (!vc_encaps)
		be.encaps = PPPOATM_ENCAPS_LLC;
	else
		be.encaps = PPPOATM_ENCAPS_AUTODETECT;
	if (ioctl(fd, ATM_SETBACKEND, &be) < 0){
		fatal("ioctl(ATM_SETBACKEND): %m");
       }
}

#if 0
static void reset_line_discipline_pppoatm(int fd)
{
	atm_backend_t be = ATM_BACKEND_RAW;
	/* 2.4 doesn't support this yet */
	(void) ioctl(fd, ATM_SETBACKEND, &be);
}
#endif

#if 0
static void pppoa_reautocut(int fd)
{
    unsigned char * outp;
	unsigned char ppp_disconn[64];
    
	outp = ppp_disconn;
	MAKEHEADER(outp, PPP_LCP);	
	PUTCHAR(5, outp);
	PUTCHAR(2, outp); 
    PUTSHORT(4, outp);
	
	write(fd, ppp_disconn+2, 6);
    return ;
}

extern int reautocut;
static int connect_pppoatm(void)
{
    int i;
	int fd;
	struct atp_atm_qos qos;

	if (!device_got_set)
		no_device_given_pppoatm();
	fd = socket(AF_ATMPVC, SOCK_DGRAM, 0);
	if (fd < 0)
		fatal("failed to create socket: %m");
	pppoa_fd = fd;
	memset(&qos, 0, sizeof qos);
#ifdef BROADCOM_CHIP
	qos.txtp.traffic_class = qos.rxtp.traffic_class = ATM_UBR;
	/* TODO: support simplified QoS setting */
	if (qosstr != NULL)
		if (text2qos(qosstr, (struct atm_qos*)&qos, 0))
			fatal("Can't parse QoS: \"%s\"");
	qos.txtp.max_sdu = lcp_allowoptions[0].mru + pppoatm_overhead();
	qos.rxtp.max_sdu = lcp_wantoptions[0].mru + pppoatm_overhead();
#else
	/* TODO: support simplified QoS setting */
	if (qosstr != NULL)
		if (text2qos(qosstr, &qos, 0))
			fatal("Can't parse QoS: \"%s\"");

	if (pppoa_qos != -1 && pppoa_pcr != -1 && pppoa_scr != -1 && pppoa_mbs != -1)
	{
		switch (pppoa_qos)
		{
			case ATM_SC_OTHER:
				qos.txtp.traffic_class = ATM_NONE;
				break;
			case ATM_SC_UBR:
				qos.txtp.traffic_class = ATM_UBR;
				break;
			case ATM_SC_UBR_PCR:
				qos.txtp.traffic_class = ATM_UBR_PLUS;
				break;
			case ATM_SC_CBR:
				qos.txtp.traffic_class = ATM_CBR;
				break;
			case ATM_SC_NRT_VBR:
				qos.txtp.traffic_class = ATM_VBR_NRT;
				break;
			case ATM_SC_RT_VBR:
				qos.txtp.traffic_class = ATM_VBR_RT;
				break;
			default:
				printf ("Invalid ATM QoS traffic class configuration!\n");
				break;
		}
		
		qos.txtp.pcr = pppoa_pcr;
		qos.txtp.scr = pppoa_scr;
		qos.txtp.mbs = pppoa_mbs;
		qos.rxtp = qos.txtp;
	}
	else
	{
		qos.txtp.traffic_class = qos.rxtp.traffic_class = ATM_UBR;
		qos.txtp.max_sdu = lcp_allowoptions[0].mru + pppoatm_overhead();
		qos.rxtp.max_sdu = lcp_wantoptions[0].mru + pppoatm_overhead();
	}
#endif
	qos.aal = ATM_AAL5;
	if (setsockopt(fd, SOL_ATM, SO_ATP_ATMQOS, &qos, sizeof(qos)) < 0)
		fatal("setsockopt(SO_ATMQOS): %m");
	/* TODO: accept on SVCs... */
	if (connect(fd, (struct sockaddr *) &pvcaddr,
	    sizeof(struct sockaddr_atmpvc)))
		fatal("connect(%s): %m", devnam);
	pppoatm_max_mtu = lcp_allowoptions[0].mru;
	pppoatm_max_mru = lcp_wantoptions[0].mru;
	set_line_discipline_pppoatm(fd);
    if (reautocut && req_rediscon)
    {
        for (i = 0; i <= 2; i++)
        {
            pppoa_reautocut(fd);
        }
    }
	return fd;
}
#endif


static void send_config_pppoatm(int mtu, u_int32_t asyncmap,
	int pcomp, int accomp)
//static void send_config_pppoatm(int unit, int mtu, u_int32_t asyncmap,
//	int pcomp, int accomp)
// brcm end
{
	int sock;
	struct ifreq ifr;
	if (mtu > pppoatm_max_mtu)
		error("Couldn't increase MTU to %d", mtu);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		fatal("Couldn't create IP socket: %m");
	strlcpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	ifr.ifr_mtu = mtu;
	if (ioctl(sock, SIOCSIFMTU, (caddr_t) &ifr) < 0)
		fatal("ioctl(SIOCSIFMTU): %m");
	(void) close (sock);
}

// brcm begin
static void recv_config_pppoatm(int mru, u_int32_t asyncmap,
	int pcomp, int accomp)
//static void recv_config_pppoatm(int unit, int mru, u_int32_t asyncmap,
//	int pcomp, int accomp)
// brcm end
{
	if (mru > pppoatm_max_mru)
		error("Couldn't increase MRU to %d", mru);
}

#if 0
void plugin_init(void)
{
#if _linux_
	extern int new_style_driver;	/* From sys-linux.c */
	if (!ppp_available() && !new_style_driver)
		fatal("Kernel doesn't support ppp_generic - "
		    "needed for PPPoATM");
#else
	fatal("No PPPoATM support on this OS");
#endif
	info("PPPoATM plugin_init");
	add_options(pppoa_options);
	add_devname_class(setdevname_pppoatm);
	setspeed_hook = setspeed_pppoatm;
	options_for_device_hook = options_for_pppoatm;
	open_device_hook = open_device_pppoatm;
	post_open_setup_hook = post_open_setup_pppoatm;
	pre_close_restore_hook = pre_close_restore_pppoatm;
	no_device_given_hook = no_device_given_pppoatm;
	set_line_discipline_hook = set_line_discipline_pppoatm;
	reset_line_discipline_hook = reset_line_discipline_pppoatm;
	send_config_hook = send_config_pppoatm;
	recv_config_hook = recv_config_pppoatm;
	set_xaccm_hook = set_xaccm_pppoatm;
	{
		char **a;
		for (a = bad_options; *a != NULL; a++)
			remove_option(*a);
	}
	modem = 0;
	lcp_wantoptions[0].neg_accompression = 0;
	lcp_allowoptions[0].neg_accompression = 0;
	lcp_wantoptions[0].neg_asyncmap = 0;
	lcp_allowoptions[0].neg_asyncmap = 0;
	lcp_wantoptions[0].neg_pcompression = 0;
}
#endif

struct channel pppoatm_channel = {
    options: NULL,
    process_extra_options: NULL,
    check_options: NULL,
    connect: &open_device_pppoatm,
    disconnect: &close_device_pppoatm,
    establish_ppp: &generic_establish_ppp,
    disestablish_ppp: &generic_disestablish_ppp,
    send_config: &send_config_pppoatm,
    recv_config: &recv_config_pppoatm,
    close: NULL,
    cleanup: NULL
};
