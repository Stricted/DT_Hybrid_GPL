/* vim: set ai tabstop=4 expandtab: */
/*
 * Layer Two Tunnelling Protocol Daemon
 * Copyright (C) 1998 Adtran, Inc.
 * Copyright (C) 2002 Jeff McAdams
 *
 * Mark Spencer
 *
 * This software is distributed under the terms
 * of the GPL, which you should have received
 * along with this source.
 *
 * Network routines for UDP handling
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "l2tp.h"
#include "ipsecmast.h"
#include "chillout.h"

char hostname[256];
struct sockaddr_in server, from;        /* Server and transmitter structs */
int server_socket;              /* Server socket */
#ifdef USE_KERNEL
int kernel_support;             /* Kernel Support there or not? */
#endif

chillout_t chillout = NULL;           /* pm */
extern int waiting_first_cid;           /* pm */
extern int lac_hangup(int cid);         /* pm */
extern int g_lChilloutSend;

/* chillout callbacks prototipes */
static int callback_tunnel(chillout_t co, void *data, int argc, char **argv);
static int callback_session(chillout_t co, void *data, int argc, char **argv);
static int callback_show(chillout_t co, void *data, int argc, char **argv);
static int callback_default(chillout_t co, void *data, int argc, char **argv);

static struct callback global_callbacks[] = {
    { "xl2tpd", "tunnel", 0, callback_tunnel },
    { "xl2tpd", "session", 0, callback_session },
    { "xl2tpd", "show", 0, callback_show },
    { "xl2tpd", "default", 0, callback_default },
    { "", "", 0, NULL }
};


struct half_words {
	_u16 s0;
	_u16 s1;
	_u16 s2;
	_u16 s3;
} __attribute__ ((packed));

/* defined in xl2tpd.c */
extern struct tunnel *l2tp_call (char *host, int port, struct lac *lac, struct lns *lns);

int clear_network (void)
{
    /* close read/write socket */  
    l2tp_log (LOG_CRIT, "%s: close sockets\n",__FUNCTION__);
    if (chillout)
    {
        chillout_finish(chillout);
        chillout = NULL;
    }
    return 0;
}

/* start LNS错误导致无法建立隧道 DTS2012103105714 */
int init_server_socket(void)
{
    long arg;
    unsigned int length = sizeof (server);
    gethostname (hostname, sizeof (hostname));
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = gconfig.listenaddr; 
    server.sin_port = htons (gconfig.port);
    if ((server_socket = socket (PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        l2tp_log (LOG_CRIT, "%s: Unable to allocate socket. Terminating.\n",
             __FUNCTION__);
        return -EINVAL;
    };

    if (bind (server_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        close (server_socket);
        l2tp_log (LOG_CRIT, "%s: Unable to bind socket: %s. Terminating.\n",
             __FUNCTION__, strerror(errno));
        return -EINVAL;
    };
    if (setsockopt(server_socket, SOL_SOCKET, SO_BINDTODEVICE, PPPOEIFNAME, strlen(PPPOEIFNAME)+1) < 0)
    {
        l2tp_log (LOG_CRIT, "%s: Unable to bind to ppp256.\n",
             __FUNCTION__);
    }
    if (getsockname (server_socket, (struct sockaddr *) &server, &length))
    {
        l2tp_log (LOG_CRIT, "%s: Unable to read socket name.Terminating.\n",
             __FUNCTION__);
        return -EINVAL;
    }
    /*
     * For L2TP/IPsec with KLIPSng, set the socket to receive IPsec REFINFO
     * values.
     */
    arg=1;
    if(setsockopt(server_socket, SOL_IP, IP_IPSEC_REFINFO,
		  &arg, sizeof(arg)) != 0) {
	    l2tp_log(LOG_CRIT, "setsockopt recvref: %s\n", strerror(errno));

	    gconfig.ipsecsaref=0;
    }
    
#ifdef USE_KERNEL
    if (gconfig.forceuserspace)
    {
        l2tp_log (LOG_INFO, "Not looking for kernel support.\n");
        kernel_support = 0;
    }
    else
    {
        int kernel_fd = socket(AF_PPPOX, SOCK_DGRAM, PX_PROTO_OL2TP);
        if (kernel_fd < 0)
        {
            l2tp_log (LOG_INFO, "L2TP kernel support not detected.\n");
            kernel_support = 0;
        }
        else
        {
            close(kernel_fd);
            l2tp_log (LOG_INFO, "Using l2tp kernel support.\n");
            kernel_support = -1;
        }
    }
#else
    l2tp_log (LOG_INFO, "This binary does not support kernel L2TP.\n");
#endif
    arg = fcntl (server_socket, F_GETFL);
    arg |= O_NONBLOCK;
    fcntl (server_socket, F_SETFL, arg);

    return 0;
}
/* end LNS错误导致无法建立隧道 DTS2012103105714 */

int init_network (void)
{
    long arg;
    unsigned int length = sizeof (server);
    gethostname (hostname, sizeof (hostname));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = gconfig.listenaddr; 
    server.sin_port = htons (gconfig.port);
    if ((server_socket = socket (PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        l2tp_log (LOG_CRIT, "%s: Unable to allocate socket. Terminating.\n",
             __FUNCTION__);
        return -EINVAL;
    };

    if (bind (server_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        close (server_socket);
        l2tp_log (LOG_CRIT, "%s: Unable to bind socket: %s. Terminating.\n",
             __FUNCTION__, strerror(errno));
        return -EINVAL;
    };
    if (setsockopt(server_socket, SOL_SOCKET, SO_BINDTODEVICE, PPPOEIFNAME, strlen(PPPOEIFNAME)+1) < 0)
    {
        l2tp_log (LOG_CRIT, "%s: Unable to bind to ppp256.\n",
             __FUNCTION__);
    }
    if (getsockname (server_socket, (struct sockaddr *) &server, &length))
    {
        l2tp_log (LOG_CRIT, "%s: Unable to read socket name.Terminating.\n",
             __FUNCTION__);
        return -EINVAL;
    }
    /*
     * For L2TP/IPsec with KLIPSng, set the socket to receive IPsec REFINFO
     * values.
     */
    arg=1;
    if(setsockopt(server_socket, SOL_IP, IP_IPSEC_REFINFO,
		  &arg, sizeof(arg)) != 0) {
	    l2tp_log(LOG_CRIT, "setsockopt recvref: %s\n", strerror(errno));

	    gconfig.ipsecsaref=0;
    }
    
#ifdef USE_KERNEL
    if (gconfig.forceuserspace)
    {
        l2tp_log (LOG_INFO, "Not looking for kernel support.\n");
        kernel_support = 0;
    }
    else
    {
        int kernel_fd = socket(AF_PPPOX, SOCK_DGRAM, PX_PROTO_OL2TP);
        if (kernel_fd < 0)
        {
            l2tp_log (LOG_INFO, "L2TP kernel support not detected.\n");
            kernel_support = 0;
        }
        else
        {
            close(kernel_fd);
            l2tp_log (LOG_INFO, "Using l2tp kernel support.\n");
            kernel_support = -1;
        }
    }
#else
    l2tp_log (LOG_INFO, "This binary does not support kernel L2TP.\n");
#endif
    arg = fcntl (server_socket, F_GETFL);
    arg |= O_NONBLOCK;
    fcntl (server_socket, F_SETFL, arg);
    gconfig.port = ntohs (server.sin_port);
    if ((chillout = chillout_init(gconfig.chilloutsocket, "xl2tpd", NULL)) == NULL) {
        l2tp_log(LOG_ERR, "Error calling chillout_init(%s, \"xl2tpd\", NULL)\n", 
            gconfig.chilloutsocket);
    	return 1;
    } else {
        char buffer[256];

        chillout_write(chillout, CHILLOUT_WRITE_SOCK, "show version");
    	chillout->callbacks = global_callbacks;
	    if (chillout_read(chillout, CHILLOUT_WRITE_SOCK, buffer)) {
		    l2tp_log(LOG_ERR, "error reading from chillout");
    	}
	    if (buffer[strlen(buffer) - 1] == '\n') {
		    buffer[strlen(buffer) - 1] = '\0';
    	}
    	l2tp_log(LOG_INFO, "Chillout subsystem loaded. Server version %s\n", buffer);
    	chillout_read(chillout, CHILLOUT_WRITE_SOCK, buffer);
    }
    chillout->callbacks = global_callbacks;
    return 0;
}

inline void extract (void *buf, int *tunnel, int *call)
{
    /*
     * Extract the tunnel and call #'s, and fix the order of the 
     * version
     */

    struct payload_hdr *p = (struct payload_hdr *) buf;
    if (PLBIT (p->ver))
    {
        *tunnel = p->tid;
        *call = p->cid;
    }
    else
    {
        *tunnel = p->length;
        *call = p->tid;
    }
}

inline void fix_hdr (void *buf)
{
    /*
     * Fix the byte order of the header
     */

    struct payload_hdr *p = (struct payload_hdr *) buf;
    _u16 ver = ntohs (p->ver);
    if (CTBIT (p->ver))
    {
        /*
         * Control headers are always
         * exactly 12 bytes big.
         */
        swaps (buf, 12);
    }
    else
    {
        int len = 6;
        if (PSBIT (ver))
            len += 4;
        if (PLBIT (ver))
            len += 2;
        if (PFBIT (ver))
            len += 4;
        swaps (buf, len);
    }
}

void dethrottle (void *call)
{
/*	struct call *c = (struct call *)call; */
/*	if (c->throttle) {
#ifdef DEBUG_FLOW
		log(LOG_DEBUG, "%s: dethrottling call %d, and setting R-bit\n",__FUNCTION__,c->ourcid); 
#endif 		c->rbit = RBIT;
		c->throttle = 0;
	} else {
		log(LOG_DEBUG, "%s:  call %d already dethrottled?\n",__FUNCTION__,c->ourcid); 	
	} */
}

void control_xmit (void *b)
{
    struct buffer *buf = (struct buffer *) b;
    struct tunnel *t;
    struct timeval tv;
    int ns;
    int type = 0;

    if (!buf)
    {
        l2tp_log (LOG_WARNING, "%s: called on NULL buffer!\n", __FUNCTION__);
        return;
    }

    t = buf->tunnel;
#ifdef DEBUG_CONTROL_XMIT
    if(t) {
	    l2tp_log (LOG_DEBUG,
		      "trying to send control packet to %d\n",
		      t->ourtid);
    }
#endif

    buf->retries++;
    ns = ntohs (((struct control_hdr *) (buf->start))->Ns);
    if (t)
    {
        if (ns < t->cLr)
        {
#ifdef DEBUG_CONTROL_XMIT
            l2tp_log (LOG_DEBUG, "%s: Tossing packet %d\n", __FUNCTION__, ns);
#endif
            /* Okay, it's been received.  Let's toss it now */
            toss (buf);
            return;
        }
    }
    if (buf->retries > gconfig.max_retries)
    {
        /*
           * Too many retries.  Either kill the tunnel, or
           * if there is no tunnel, just stop retransmitting.
         */
        if (t)
        {
            if (t->self->needclose)
            {
                l2tp_log (LOG_DEBUG,
                     "Unable to deliver closing message for tunnel %d. Destroying anyway.\n",
                     t->ourtid);
                t->self->needclose = 0;
                t->self->closing = -1;
                /* 
        		 * pm: notify awaiting chilloutd of the horrisonous failure of 
		         * the tunnel lift
        		 */
                if (waiting_first_cid) {
                    l2tp_log(LOG_NOTICE, "waiting first cid. Notifiying chilloutd.............\n");
                    chillout_write(chillout, 0, "error tunnel");
                    waiting_first_cid = 0;
                    g_lChilloutSend = CHILLOUT_MSG_SEND;
                }
            }
            else
            {
                l2tp_log (LOG_NOTICE,
                     "Maximum retries exceeded for tunnel %d.  Closing.\n",
                     t->ourtid);
                strcpy (t->self->errormsg, "Timeout");
                t->self->needclose = -1;
            }
        }
	free(buf->rstart);
	free(buf);
    }
    else
    {
        /*
           * FIXME:  How about adaptive timeouts?
         */
		/* BEGIN: add for UDK0000287744 FON: The home gateway does not send the L2TP and PPP messages to the LNS server according to the corresponding TR-069 parameters 2013.06.29 */ 
        type = ntohs(((struct half_words * ) (buf->start + sizeof(struct control_hdr) + sizeof(struct avp_hdr)))->s0);         
        if(Hello == type)
        {
            tv.tv_sec = gconfig.hello_retransmission_delay;
        }
        else
        {
            tv.tv_sec = gconfig.retransmission_delay;
        }
		/* END: add for UDK0000287744 FON: The home gateway does not send the L2TP and PPP messages to the LNS server according to the corresponding TR-069 parameters 2013.06.29 */
        tv.tv_usec = 0;
        /*DTS2014031502037: if ICCN already send, we alreay started ppp, so no need to resend it or wait for its ZLB*/
        if (type != ICCN)
        {
            schedule (tv, control_xmit, buf);
        }
#ifdef DEBUG_CONTROL_XMIT
        l2tp_log (LOG_DEBUG, "%s: Scheduling and transmitting packet %d\n",
             __FUNCTION__, ns);
#endif
        udp_xmit (buf, t);
    }
}

void udp_xmit (struct buffer *buf, struct tunnel *t)
{
    struct cmsghdr *cmsg;
    char cbuf[CMSG_SPACE(sizeof (unsigned int))];
    unsigned int *refp;
    struct msghdr msgh;
    int err;
    struct iovec iov;
    
    /*
     * OKAY, now send a packet with the right SAref values.
     */
    memset(&msgh, 0, sizeof(struct msghdr));

    msgh.msg_control = cbuf;
    msgh.msg_controllen = 0;

    if(gconfig.ipsecsaref && t->refhim != IPSEC_SAREF_NULL) {
	msgh.msg_controllen = sizeof(cbuf);

	cmsg = CMSG_FIRSTHDR(&msgh);
	cmsg->cmsg_level = SOL_IP;
	cmsg->cmsg_type  = IP_IPSEC_REFINFO;
	cmsg->cmsg_len   = CMSG_LEN(sizeof(unsigned int));

	if(gconfig.debug_network) {
		l2tp_log(LOG_DEBUG,"sending with saref=%d\n", t->refhim);
	}
	refp = (unsigned int *)CMSG_DATA(cmsg);
	*refp = t->refhim;

	msgh.msg_controllen = cmsg->cmsg_len;
    }
    
    iov.iov_base = buf->start;
    iov.iov_len  = buf->len;

    /* return packet from whence it came */
    msgh.msg_name    = &buf->peer;
    msgh.msg_namelen = sizeof(buf->peer);
    
    msgh.msg_iov  = &iov;
    msgh.msg_iovlen = 1;
    msgh.msg_flags = 0;


    /* Receive one packet. */
    if ((err = sendmsg(server_socket, &msgh, 0)) < 0) {
	l2tp_log(LOG_ERR, "udp_xmit failed with err=%d:%s\n",
		 err,strerror(errno));
    }
}

int build_fdset (fd_set *readfds)
{
	struct tunnel *tun;
	struct call *call;
	int max = 0;

	tun = tunnels.head;
	FD_ZERO (readfds);

	while (tun)
	{
		call = tun->call_head;
		while (call)
		{
			if (call->needclose ^ call->closing)
			{
				call_close (call);
				call = tun->call_head;
				if (!call)
					break;
				continue;
			}
			if (call->fd > -1)
			{
				if (!call->needclose && !call->closing)
				{
					if (call->fd > max)
						max = call->fd;
					FD_SET (call->fd, readfds);
				}
			}
			call = call->next;
		}
		/* Now that call fds have been collected, and checked for
		 * closing, check if the tunnel needs to be closed too
		 */
		if (tun->self->needclose ^ tun->self->closing)
		{
			if (gconfig.debug_tunnel)
				l2tp_log (LOG_DEBUG, "%s: closing down tunnel %d\n",
						__FUNCTION__, tun->ourtid);
			call_close (tun->self);
			/* Reset the while loop
			 * and check for NULL */
			tun = tunnels.head;
			if (!tun)
				break;
			continue;
		}
		tun = tun->next;
	}
	FD_SET (server_socket, readfds);
	if (server_socket > max)
		max = server_socket;
	FD_SET (control_fd, readfds);
	if (control_fd > max)
		max = control_fd;
    if (chillout != NULL) {
        FD_SET(chillout->readsock, readfds);
        if (chillout->readsock > max)
            max = chillout->readsock;
    }
	return max;
}

void network_thread ()
{
    /*
     * We loop forever waiting on either data from the ppp drivers or from
     * our network socket.  Control handling is no longer done here.
     */
    struct sockaddr_in from, to;
    unsigned int fromlen, tolen;
    int tunnel, call, selret;           /* Tunnel and call */
    int recvsize;               /* Length of data received */
    struct buffer *buf;         /* Payload buffer */
    struct call *c, *sc;        /* Call to send this off to */
    struct tunnel *st;          /* Tunnel */
    fd_set readfds;             /* Descriptors to watch for reading */
    int max;                    /* Highest fd */
    struct timeval tv;          /* Timeout for select */
    struct msghdr msgh;
    struct iovec iov;
    char cbuf[256];
    unsigned int refme, refhim;

    /* This one buffer can be recycled for everything except control packets */
    buf = new_buf (MAX_RECV_SIZE);

    tunnel = 0;
    call = 0;
    selret = 0;

    for (;;)
    {
        max = build_fdset (&readfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        schedule_unlock ();
        selret = select (max + 1, &readfds, NULL, NULL, NULL);
        if (selret < 0) {
            if (errno == EINTR) {
                continue;
            }
            l2tp_log(LOG_ERR, "%s: select returned %d: %s\n", __FUNCTION__, selret, strerror(errno));
            continue;
        }
        /* if we used that tv, in the select call, and select returned 0
         * that would mean timeout has passed without data
         */
        schedule_lock ();
        if (FD_ISSET (control_fd, &readfds))
        {
            do_control ();
        }
        if ((chillout != NULL) && FD_ISSET(chillout->readsock, &readfds)) {
            int ret;
            ret  = chillout_accept(chillout);
            if (ret != 0) {
                l2tp_log(LOG_ERR, "chillout_accept() failed\n");
                if (ret == CHILLOUT_DISCONNECTED) {
                    FD_CLR(chillout->readsock, &readfds);
                    chillout_finish(chillout);
                    chillout = NULL;
                }
            }
        }
        if (FD_ISSET (server_socket, &readfds))
        {
            /*
             * Okay, now we're ready for reading and processing new data.
             */
            recycle_buf (buf);

            /* Reserve space for expanding payload packet headers */
            buf->start += PAYLOAD_BUF;
            buf->len -= PAYLOAD_BUF;

	    memset(&from, 0, sizeof(from));
	    memset(&to,   0, sizeof(to));
	    
	    fromlen = sizeof(from);
	    tolen   = sizeof(to);
	    
	    memset(&msgh, 0, sizeof(struct msghdr));
	    iov.iov_base = buf->start;
	    iov.iov_len  = buf->len;
	    msgh.msg_control = cbuf;
	    msgh.msg_controllen = sizeof(cbuf);
	    msgh.msg_name = &from;
	    msgh.msg_namelen = fromlen;
	    msgh.msg_iov  = &iov;
	    msgh.msg_iovlen = 1;
	    msgh.msg_flags = 0;
	    
	    /* Receive one packet. */
	    recvsize = recvmsg(server_socket, &msgh, 0);

            if (recvsize < MIN_PAYLOAD_HDR_LEN)
            {
                if (recvsize < 0)
                {
                    if (errno != EAGAIN)
                        l2tp_log (LOG_WARNING,
                             "%s: recvfrom returned error %d (%s)\n",
                             __FUNCTION__, errno, strerror (errno));
                }
                else
                {
                    l2tp_log (LOG_WARNING, "%s: received too small a packet\n",
                         __FUNCTION__);
                }
		continue;
            }


	    refme=refhim=0;

	    /* extract IPsec info out */
	    if(gconfig.ipsecsaref) {
		    struct cmsghdr *cmsg;
		    /* Process auxiliary received data in msgh */
		    for (cmsg = CMSG_FIRSTHDR(&msgh);
			 cmsg != NULL;
			 cmsg = CMSG_NXTHDR(&msgh,cmsg)) {
			    if (cmsg->cmsg_level == IPPROTO_IP
				&& cmsg->cmsg_type == IP_IPSEC_REFINFO) {
				    unsigned int *refp;
				    
				    refp = (unsigned int *)CMSG_DATA(cmsg);
				    refme =refp[0];
				    refhim=refp[1];
			    }
		    }
	    }

	    /*
	     * some logic could be added here to verify that we only
	     * get L2TP packets inside of IPsec, or to provide different
	     * classes of service to packets not inside of IPsec.
	     */
	    buf->len = recvsize;
	    fix_hdr (buf->start);
	    extract (buf->start, &tunnel, &call);

	    if (gconfig.debug_network)
	    {
		l2tp_log(LOG_DEBUG, "%s: recv packet from %s, size = %d, "
			 "tunnel = %d, call = %d ref=%u refhim=%u\n",
			 __FUNCTION__, inet_ntoa (from.sin_addr),
			 recvsize, tunnel, call, refme, refhim);
	    }

	    if (gconfig.packet_dump)
	    {
		do_packet_dump (buf);
	    }
	    if (!
		(c = get_call (tunnel, call, from.sin_addr.s_addr,
			       from.sin_port, refme, refhim)))
	    {
		if ((c =
		     get_tunnel (tunnel, from.sin_addr.s_addr,
				 from.sin_port)))
		{
		    /*
		     * It is theoretically possible that we could be sent
		     * a control message (say a StopCCN) on a call that we
		     * have already closed or some such nonsense.  To
		     * prevent this from closing the tunnel, if we get a
		     * call on a valid tunnel, but not with a valid CID,
		     * we'll just send a ZLB to ack receiving the packet.
		     */
		    if (gconfig.debug_tunnel)
			l2tp_log (LOG_DEBUG,
				  "%s: no such call %d on tunnel %d.  Sending special ZLB\n",
				  __FUNCTION__);
		    handle_special (buf, c, call);

		    /* get a new buffer */
		    buf = new_buf (MAX_RECV_SIZE);
		}
		else
		    l2tp_log (LOG_DEBUG,
			      "%s: unable to find call or tunnel to handle packet.  call = %d, tunnel = %d Dumping.\n",
			      __FUNCTION__, call, tunnel);
		
	    }
	    else
	    {
		buf->peer = from;
		/* Handle the packet */
		c->container->chal_us.vector = NULL;
		if (handle_packet (buf, c->container, c))
		{
		    if (gconfig.debug_tunnel)
			l2tp_log (LOG_DEBUG, "%s: bad packet\n", __FUNCTION__);
		};
		if (c->cnu)
		{
		    /* Send Zero Byte Packet */
		    control_zlb (buf, c->container, c);
		    c->cnu = 0;
		}
	    };
	}

	/*
	 * finished obvious sources, look for data from PPP connections.
	 */
	st = tunnels.head;
        while (st)
        {
            sc = st->call_head;
            while (sc)
            {
                if ((sc->fd >= 0) && FD_ISSET (sc->fd, &readfds))
                {
                    /* Got some payload to send */
                    int result;
                    recycle_payload (buf, sc->container->peer);
/*
#ifdef DEBUG_FLOW_MORE
                    l2tp_log (LOG_DEBUG, "%s: rws = %d, pSs = %d, pLr = %d\n",
                         __FUNCTION__, sc->rws, sc->pSs, sc->pLr);
#endif
		    if ((sc->rws>0) && (sc->pSs > sc->pLr + sc->rws) && !sc->rbit) {
#ifdef DEBUG_FLOW
						log(LOG_DEBUG, "%s: throttling payload (call = %d, tunnel = %d, Lr = %d, Ss = %d, rws = %d)!\n",__FUNCTION__,
								 sc->cid, sc->container->tid, sc->pLr, sc->pSs, sc->rws); 
#endif
						sc->throttle = -1;
						We unthrottle in handle_packet if we get a payload packet, 
						valid or ZLB, but we also schedule a dethrottle in which
						case the R-bit will be set
						FIXME: Rate Adaptive timeout? 						
						tv.tv_sec = 2;
						tv.tv_usec = 0;
						sc->dethrottle = schedule(tv, dethrottle, sc); 					
					} else */
/*					while ((result=read_packet(buf,sc->fd,sc->frame & SYNC_FRAMING))>0) { */
                    while ((result =
                            read_packet (buf, sc->fd, SYNC_FRAMING)) > 0)
                    {
                        add_payload_hdr (sc->container, sc, buf);
                        if (gconfig.packet_dump)
                        {
                            do_packet_dump (buf);
                        }


                        sc->prx = sc->data_rec_seq_num;
                        if (sc->zlb_xmit)
                        {
                            deschedule (sc->zlb_xmit);
                            sc->zlb_xmit = NULL;
                        }
                        sc->tx_bytes += buf->len;
                        sc->tx_pkts++;
                        udp_xmit (buf, st);
                        recycle_payload (buf, sc->container->peer);
                    }
                    if (result != 0)
                    {
                        l2tp_log (LOG_WARNING,
                             "%s: tossing read packet, error = %s (%d).  Closing call.\n",
                             __FUNCTION__, strerror (-result), -result);
                        strcpy (sc->errormsg, strerror (-result));
                        sc->needclose = -1;
                    }
                }
                sc = sc->next;
            }
            st = st->next;
        }
    }

    if (chillout) {
        chillout_finish(chillout);
        l2tp_log(LOG_NOTICE, "Chillout subsystem finished");
    }
}

/*
 * callback_tunnel
 *
 * tunnel up <lac> 
 * <- tid <tunnel_id>
 * <- ok
 * tunnel down <lac>
 * <- ok
 * OR
 * <- error reason
 * TODO
 */
#define TUNNEL_NEW 0
#define TUNNEL_UP 1
#define TUNNEL_DOWN 2
extern int switch_io;     /* defined in xl2tpd.c */
static int callback_tunnel(chillout_t co, void *data, int argc, char **argv)
{
    int mode;
    struct tunnel *tunnel;

    if (argc != 3) {
        chillout_write(co, 0, "error argc %d", argc);
        return 1;
    }
    if (!strcmp(argv[1], "up")) {
        mode = TUNNEL_UP;
    } else if (!strcmp(argv[1], "down")) {
        mode = TUNNEL_DOWN;
    } else {
        chillout_write(co, 0, "error unknown argument %s", argv[1]);
        return 1;
    }
    if (mode == TUNNEL_UP) {
        tunnel = l2tp_call(argv[2], UDP_LISTEN_PORT, NULL, NULL);
        if (tunnel == NULL) {
            chillout_write(co, 0, "error creating tunnel for LAC %s", argv[2]);
            return 1;
        } else {
            chillout_write(co, 0, "tid %d", tunnel->tid);
            return 0;
        }
    } else {
        chillout_write(co, 0, "error todo"); 
    }
    return 1;
}

/*
 * callback_session
 *
 * session create <lac> <username> <password> <ip_address> <mac>
 * <- session <ppp device> <sessionid>
 * <- ok
 * session delete <sessionid>
 * <- ok 
 * OR
 * <- error <reason>
 * TODO
 */
#define SESSION_CREATE 1
#define SESSION_DELETE 2
static int callback_session(chillout_t co, void *data, int argc, char **argv)
{
    int mode, ret, cid, first;
    struct lac *lac;

    if (!strcmp(argv[1], "create")) {
        mode = SESSION_CREATE;
    } else if (!strcmp(argv[1], "delete")) {
        mode = SESSION_DELETE;
    }  else {
        chillout_write(co, 0, "error unknown argument %s", argv[1]);
        return CHILLOUT_ERROR;
    }

    if ((mode == SESSION_CREATE) && (argc != 6)) {
        chillout_write(co, 0, "error argc %d", argc);
        return CHILLOUT_ERROR;
    } else if ((mode == SESSION_DELETE) && (argc != 3)) {
        chillout_write(co, 0, "error argc %d", argc);
        return CHILLOUT_ERROR;
    }

    if (mode == SESSION_CREATE) {
        switch_io = 1;
        lac = laclist;
        while (lac && strcasecmp(lac->entname, argv[2]))
            lac = lac->next;
        if (!lac) {
            chillout_write(co, 0, "error lac %s not found", argv[2]);
            return CHILLOUT_ERROR;
        }
        snprintf (lac->authname, sizeof(lac->authname), "%s", argv[3]);
        snprintf (lac->password, sizeof(lac->password), "%s", argv[4]);
        strncpy (lac->chilloutmac, argv[5], STRLEN);

        lac->active = -1;
        lac->rtries = 0;
        if (!lac->c) {
            l2tp_log(LOG_DEBUG, "%s: This is the first call over this LAC (session %s)\n",
                __FUNCTION__, argv[1]);
            /* chillout_write(co, 0, "tunnel created"); */
            first = 1;
        } else {
            l2tp_log(LOG_DEBUG, "%s: This is not the first call over this LAC (session %s)\n", 
                __FUNCTION__, argv[1]);
            first = 0;
        }
        ret = magic_lac_dial(lac, &cid);
        if (ret != 0) {
            l2tp_log(LOG_DEBUG, "%s: error %d dialing LAC %s\n", __FUNCTION__,
                ret, argv[2]);
            chillout_write(co, 0, "error %d dialing LAC %s\n", ret, argv[2]);
            return CHILLOUT_ERROR;
        } else {
            if (first) {
                if (waiting_first_cid == 0) {
                    l2tp_log(LOG_DEBUG, "%s: very quick answer!\n", __FUNCTION__); 
                }
                return CHILLOUT_NOOK;
            }
            return CHILLOUT_NOOK;
        }
    } else if (mode == SESSION_DELETE) {
        cid = atoi(argv[2]);
        l2tp_log(LOG_DEBUG, "%s: Attempting to hangup session %d\n", __FUNCTION__, cid);
        /* TODO: lac_hangup seems to left sessions around until the last one... */
        if (lac_hangup(cid)) {
            chillout_write(chillout, 0, "error no such session %d", cid);
            return CHILLOUT_ERROR;
        }
        return 0;
    }
    return 0;
}

/*
 * callback_show
 *
 * show tunnel <tid>
 *  <print tunnel data>
 * show calls
 *  <list of sessions (calls)>
 *
 */
static int callback_show(chillout_t co, void *data, int argc, char **argv)
{
    struct tunnel *t = tunnels.head;
    struct call *c;

    if (!strcmp(argv[1], "calls")) {
        if ((tunnels.count == 0) || (t == NULL)) {
            chillout_write(co, 0, "no tunnel");
            return 0;
        }
        c = t->call_head;
        while(c != NULL) {
            if (c->lac != NULL) {
                chillout_write(co, 0, "call id %d", c->ourcid);
            }
            c = c->next;
        }
    }
    return 0;
}
/* callback_default */
static int callback_default(chillout_t co, void *data, int argc, char **argv)
{
    chillout_write(co, 0, "error unknown token %s", argv[0]); 
    return 1;
}
