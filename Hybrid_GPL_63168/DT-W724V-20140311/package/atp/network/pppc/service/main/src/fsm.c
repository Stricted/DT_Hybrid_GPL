/*
 * fsm.c - {Link, IP} Control Protocol Finite State Machine.
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


/*
 * TODO:
 * Randomize fsm id on link/init.
 * Deal with variable outgoing MTU.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "pppd.h"
#include "fsm.h"
#include "atplog.h"

static const char rcsid[] = RCSID;

static void fsm_timeout __P((void *));
static void fsm_rconfreq __P((fsm *, int, u_char *, int));
static void fsm_rconfack __P((fsm *, int, u_char *, int));
static void fsm_rconfnakrej __P((fsm *, int, int, u_char *, int));
static void fsm_rtermreq __P((fsm *, int, u_char *, int));
static void fsm_rtermack __P((fsm *));
static void fsm_rcoderej __P((fsm *, u_char *, int));
static void fsm_sconfreq __P((fsm *, int));

#define PROTO_NAME(f)	((f)->callbacks->proto_name)

int peer_mru[NUM_PPP];

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
//for W724v
static void dt_penalty_end __P((void *arg));
static void dt_wait_last_penalty __P((void *arg));
/*TOCPE0056  6.4 & 6.5节*/
/*burst retry 阶段，发包时间点秒数, 0 1 2 4 8 16, */
/*burst失败，进入penalty阶段，发包时间点秒数, 0 1  2 4 8 16 32 64 128 256*/
/*报文间的间隔秒数1,1,2,4,8*/
static int burst_intervals[] = {1,1,2,4,8,16};

/*tocpe0056,LCP的重发间隔，根据剩下的重发个数做下标倒排数组,
第一次取重发数为6，重发间隔1s；第2个重试包再延时1s，第3个包在第2个基础上延时2秒，
类推，最后一个包为8秒，数组第2个元素表示在最后一个重试包发完后的16秒内未收到回应，则断掉LCP,数组第一个元素未用到*/
static int lcp_intervals[] = {3,16,8,4,2,1,1};
/*在burst与penalty之间等16s(penalty_intervals数组的第一个值)收回应, 若有回应，则penalty的包不用发*/
//static int penalty_intervals[] = {16,1,2,4,8,16,32,64,128,256};
static int penalty_intervals[9][6] = {{1,1,1,2,4,8}, 
                                       {16+2,1,1,2,4,8},
                                       {16+4,1,1,2,4,8}, 
                                       {16+8,1,1,2,4,8},
                                       {16+16,1,1,2,4,8}, 
                                       {16+32,1,1,2,4,8},    
                                       {16+64,1,1,2,4,8},
                                       {16+128,1,1,2,4,8}, 
                                       {16+256,1,1,2,4,8},                                                  
                                        };
static int last_intervals[] = {16+600,1,1,2,4,8};

extern fsm ipv6cp_fsm[NUM_PPP];
extern fsm ipcp_fsm[NUM_PPP];
/*Start of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036: IPCP/IPV6CP are both failed in burst stage, HG must terminate PPP*/
enum
{
    IPCP_BURST_OVER = 1,
    IPV6CP_BURST_OVER,
};
int burst_over_flag = 0;
/*End of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036*/
#endif

/*
 * fsm_init - Initialize fsm.
 *
 * Initialize fsm state.
 */
void
fsm_init(f)
    fsm *f;
{
    f->state = INITIAL;
    f->flags = 0;
    f->id = 0;				/* XXX Start with random id? */
    f->timeouttime = DEFTIMEOUT;

#ifdef SUPPORT_TD_CARD
    /* BEGIN: Modified by y67514, 2008/12/23   PN:ET128*/
    if ( 3 == dial_protocol )
    {
        /*HSPA??*/
        f->maxconfreqtransmits = USBMAXCONFREQS;
    }
    else
#endif
    {
        f->maxconfreqtransmits = DEFMAXCONFREQS;
    }
    /* END:   Modified by y67514, 2008/12/23 */
    f->maxtermtransmits = DEFMAXTERMREQS;
    f->maxnakloops = DEFMAXNAKLOOPS;
    f->term_reason_len = 0;

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    //for W724v
    f->ncp_enable = 1; /*W724v的penalty重试失败后，置0，保持600s后再置1,允许拨号*/
    f->dt_ncp_stage = STAGE_WAITING_TRIGGER;
    f->burst_counter = 0;
    f->penalty_counter = 0;
    f->penalty_burst_counter = 0;   
    f->last_burst_counter = 0;
    f->retry_interval = 1;
    /*start:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
	f->ipv6_P004_cnt = 0;
	f->ipv6_P004_flag = 0;
	/*End:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
    f->ipxcp_R020_flag = 0;
#endif
}


/*
 * fsm_lowerup - The lower layer is up.
 */
void
fsm_lowerup(f)
    fsm *f;
{
    switch( f->state ){
    case INITIAL:
	f->state = CLOSED;
	break;

    case STARTING:
	if( f->flags & OPT_SILENT )
	    f->state = STOPPED;
	else {
	    /* Send an initial configure-request */
	    fsm_sconfreq(f, 0);
	    f->state = REQSENT;
	}
	break;

    default:
	FSMDEBUG(("%s: Up event in state %d!", PROTO_NAME(f), f->state));
    }
}


/*
 * fsm_lowerdown - The lower layer is down.
 *
 * Cancel all timeouts and inform upper layers.
 */
void
fsm_lowerdown(f)
    fsm *f;
{
    switch( f->state ){
    case CLOSED:
	f->state = INITIAL;
	break;

    case STOPPED:
	f->state = STARTING;
	if( f->callbacks->starting )
	    (*f->callbacks->starting)(f);
	break;

    case CLOSING:
	f->state = INITIAL;
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	break;

    case STOPPING:
    case REQSENT:
    case ACKRCVD:
    case ACKSENT:
	f->state = STARTING;
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	break;

    case OPENED:
	if( f->callbacks->down )
	    (*f->callbacks->down)(f);
	f->state = STARTING;
	break;

    default:
	FSMDEBUG(("%s: Down event in state %d!", PROTO_NAME(f), f->state));
    }
}


/*
 * fsm_open - Link is allowed to come up.
 */
void
fsm_open(f)
    fsm *f;
{
    switch( f->state ){
    case INITIAL:
	f->state = STARTING;
	if( f->callbacks->starting )
	    (*f->callbacks->starting)(f);
	break;

    case CLOSED:
	if( f->flags & OPT_SILENT )
	    f->state = STOPPED;
	else {
	    /* Send an initial configure-request */
	    fsm_sconfreq(f, 0);
	    f->state = REQSENT;
	}
	break;

    case CLOSING:
	f->state = STOPPING;
	/* fall through */
    case STOPPED:
    case OPENED:
	if( f->flags & OPT_RESTART ){
	    fsm_lowerdown(f);
	    fsm_lowerup(f);
	}
	break;
    }
}

/*
 * terminate_layer - Start process of shutting down the FSM
 *
 * Cancel any timeout running, notify upper layers we're done, and
 * send a terminate-request message as configured.
 */
static void
terminate_layer(f, nextstate)
    fsm *f;
    int nextstate;
{
    if( f->state != OPENED )
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
    else if( f->callbacks->down )
	(*f->callbacks->down)(f);	/* Inform upper layers we're down */

    /* Init restart counter and send Terminate-Request */
    f->retransmits = f->maxtermtransmits;
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
    if(PPP_IPCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination Request");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_REQ);
    }
    else if (PPP_LCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination Request");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_REQ);

    }
#endif
    fsm_sdata(f, TERMREQ, f->reqid = ++f->id,
	      (u_char *) f->term_reason, f->term_reason_len);

    if (f->retransmits == 0) {
	/*
	 * User asked for no terminate requests at all; just close it.
	 * We've already fired off one Terminate-Request just to be nice
	 * to the peer, but we're not going to wait for a reply.
	 */
	f->state = nextstate == CLOSING ? CLOSED : STOPPED;
	if( f->callbacks->finished )
	    (*f->callbacks->finished)(f);
	return;
    }

    TIMEOUT(fsm_timeout, f, f->timeouttime);
    --f->retransmits;

    f->state = nextstate;
}

/*
 * fsm_close - Start closing connection.
 *
 * Cancel timeouts and either initiate close or possibly go directly to
 * the CLOSED state.
 */
void
fsm_close(f, reason)
    fsm *f;
    char *reason;
{
    f->term_reason = reason;
    f->term_reason_len = (reason == NULL? 0: strlen(reason));
    switch( f->state ){
    case STARTING:
	f->state = INITIAL;
	break;
    case STOPPED:
	f->state = CLOSED;
	break;
    case STOPPING:
	f->state = CLOSING;
	break;

    case REQSENT:
    case ACKRCVD:
    case ACKSENT:
    case OPENED:
	terminate_layer(f, CLOSING);
	break;
    }
}


/*
 * fsm_timeout - Timeout expired.
 */
static void
fsm_timeout(arg)
    void *arg;
{
    fsm *f = (fsm *) arg;

    if (PPP_IPV6CP==f->protocol)
	{
	    //单栈时只记录一次P004
        if (0 == f->ipv6_P004_flag)
    	{
			ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LOG_FAIL_WITHOUT_INFO);
			f->ipv6_P004_flag = 1;
    	}
		
		//双栈时一共记录5次，每重试一轮记录一次
	    if ((f->ipv6_P004_cnt < 4)
            &&(STARTING != ipcp_fsm[0].state)&&(INITIAL != ipcp_fsm[0].state)
			&& (((DT_BURST_NUM + 1 == f->burst_counter) && (STAGE_BURST == f->dt_ncp_stage))
			|| ((0 == f->penalty_burst_counter) && (f->penalty_counter > 0)&& (STAGE_PENALTY == f->dt_ncp_stage))))
    	{
		    ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LOG_FAIL_WITHOUT_INFO);
			f->ipv6_P004_cnt++;
    	}
	}

    switch (f->state) {
    case CLOSING:
    case STOPPING:
	if( f->retransmits <= 0 ){
	    /*
	     * We've waited for an ack long enough.  Peer probably heard us.
	     */
	    pppoe_setlasterror("ERROR_ISP_TIME_OUT");
	    f->state = (f->state == CLOSING)? CLOSED: STOPPED;
	    if( f->callbacks->finished )
		(*f->callbacks->finished)(f);
	} else {
	    /* Send Terminate-Request */
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                    //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination Request");
                    ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_REQ);
        }
        else if (PPP_LCP == f->protocol)
        {
                    //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination Request");
                    ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_REQ);
        }
#endif
	    fsm_sdata(f, TERMREQ, f->reqid = ++f->id,
		      (u_char *) f->term_reason, f->term_reason_len);
	    TIMEOUT(fsm_timeout, f, f->timeouttime);
	    --f->retransmits;
	}
	break;

    case REQSENT:
    case ACKRCVD:
    case ACKSENT:
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
         //DTS2013110405440: IPCP或者IPV6CP alg_01算法超时需要记录R020日志
         if ((PPP_IPCP == f->protocol) || (PPP_IPV6CP == f->protocol))
         {
             if ((STAGE_BURST == f->dt_ncp_stage) && (DT_BURST_NUM + 1 == f->burst_counter)) 
             {
                 ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R020, PROTO_NAME(f)); //burst结束后记录R020
             }
             else if ((STAGE_PENALTY == f->dt_ncp_stage) && (f->penalty_counter > 0) && (0 == f->penalty_burst_counter)) 
             {
                 ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R020, PROTO_NAME(f)); //penalty阶段每一小组结束后记录R020
             }
             else if ((STAGE_WAIT600S == f->dt_ncp_stage) && (1 == f->ipxcp_R020_flag))
             {
                 f->ipxcp_R020_flag = 0;
                 ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R020, PROTO_NAME(f)); //wait600s阶段每一轮结束后记录R020
             }
         }
    
         if (f->retransmits <= 0 && (PPP_IPCP != f->protocol) && (PPP_IPV6CP != f->protocol)) {
#else
	if (f->retransmits <= 0) {
#endif
	    warn("%s: timeout sending Config-Requests\n", PROTO_NAME(f));
	    f->state = STOPPED;
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
        ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R023, PROTO_NAME(f));
        
        ATP_LOG_LogItemPrint(ATP_PPPC_LOG_R020, PROTO_NAME(f)); //DTS2013110405440: lcp alg_01 算法超时需要记录R020日志
#endif
	    if( (f->flags & OPT_PASSIVE) == 0 && f->callbacks->finished )
		(*f->callbacks->finished)(f);

	} else {
	    /* Retransmit the configure-request */
	    if (f->callbacks->retransmit)
		(*f->callbacks->retransmit)(f);
	    fsm_sconfreq(f, 1);		/* Re-send Configure-Request */
	    if( f->state == ACKRCVD )
		f->state = REQSENT;
	}
	break;

    default:
	FSMDEBUG(("%s: Timeout event in state %d!", PROTO_NAME(f), f->state));
    }
}

static void fsm_record_recv_termreq(void)
{
    FILE *pFile = NULL;
        
    pFile = fopen("/var/rcvtermreq", "w");
    if (NULL == pFile)
    {
        return;
    }

    fclose(pFile);
}

/*
 * fsm_input - Input packet.
 */
void
fsm_input(f, inpacket, l)
    fsm *f;
    u_char *inpacket;
    int l;
{
    u_char *inp;
    u_char code, id;
    int len;

    /*
     * Parse header (code, id and length).
     * If packet too short, drop it.
     */
    inp = inpacket;
    if (l < HEADERLEN) {
	FSMDEBUG(("fsm_input(%x): Rcvd short header.", f->protocol));
	return;
    }
    GETCHAR(code, inp);
    GETCHAR(id, inp);
    GETSHORT(len, inp);
    if (len < HEADERLEN) {
	FSMDEBUG(("fsm_input(%x): Rcvd illegal length.", f->protocol));
	return;
    }
    if (len > l) {
	FSMDEBUG(("fsm_input(%x): Rcvd short packet.", f->protocol));
	return;
    }
    len -= HEADERLEN;		/* subtract header length */

    if( f->state == INITIAL || f->state == STARTING ){
	FSMDEBUG(("fsm_input(%x): Rcvd packet in state %d.",
		  f->protocol, f->state));
	return;
    }
/*Start of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036: IPCP/IPV6CP are both failed in burst stage, HG must terminate PPP*/
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    if (PPP_IPCP == f->protocol)
    {
        burst_over_flag &= (~IPCP_BURST_OVER);
    }
    else if (PPP_IPV6CP == f->protocol)
    {
        burst_over_flag &= (~IPV6CP_BURST_OVER);
    }
#endif
/*End of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036*/

    /*
     * Action depends on code.
     */
    switch (code) {
    case CONFREQ:
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Configuration Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_CFG_REQ);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Configuration Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_CFG_REQ);
        }
#endif
	fsm_rconfreq(f, id, inp, len);
	break;
    
    case CONFACK:
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Configuration ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_CFG_ACK);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Configuration ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_CFG_ACK);
        }
		#endif
	fsm_rconfack(f, id, inp, len);
	break;
    
    case CONFNAK:
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Configuration NAK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_CFG_NAK);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Configuration NAK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_CFG_NAK);
        }
		#endif
    case CONFREJ:
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Configuration Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_CFG_REJECT);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Configuration Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_CFG_REJECT);
        }
#endif
	fsm_rconfnakrej(f, code, id, inp, len);
	break;
    
    case TERMREQ:
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Termination Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_TERM_REQ);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Termination Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_TERM_REQ);
        }
#endif
    pppoe_setlasterror("ERROR_ISP_DISCONNECT");

    fsm_record_recv_termreq();
    
	fsm_rtermreq(f, id, inp, len);
	break;
    
    case TERMACK:
	#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Termination ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_TERM_ACK);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Termination ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_TERM_ACK);
        }
		#endif
    	fsm_rtermack(f);
    	break;
    
    case CODEREJ:
	#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Receive Code Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_RCV_CODE_REJECT);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Receive Code Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_RCV_CODE_REJECT);
        }
		#endif
    	fsm_rcoderej(f, inp, len);
    	break;
    
    default:
	if( !f->callbacks->extcode
	   || !(*f->callbacks->extcode)(f, code, id, inp, len) )
	    fsm_sdata(f, CODEREJ, ++f->id, inpacket, len + HEADERLEN);
	break;
    }
}


/*
 * fsm_rconfreq - Receive Configure-Request.
 */
static void
fsm_rconfreq(f, id, inp, len)
    fsm *f;
    u_char id;
    u_char *inp;
    int len;
{
    int code, reject_if_disagree;

    switch( f->state ){
    case CLOSED:
    	/* Go away, we're closed */
		#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
        if (PPP_IPCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_ACK);
        }
        else if (PPP_LCP == f->protocol)
        {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_ACK);
        }
		#endif
    	fsm_sdata(f, TERMACK, id, NULL, 0);
    	return;
    case CLOSING:
    case STOPPING:
	return;

    case OPENED:
	/* Go down and restart negotiation */
	if( f->callbacks->down )
	    (*f->callbacks->down)(f);	/* Inform upper layers */
	fsm_sconfreq(f, 0);		/* Send initial Configure-Request */
	f->state = REQSENT;
	break;

    case STOPPED:
	/* Negotiation started by our peer */
	fsm_sconfreq(f, 0);		/* Send initial Configure-Request */
	f->state = REQSENT;
	break;
    }

    /*
     * Pass the requested configuration options
     * to protocol-specific code for checking.
     */
    if (f->callbacks->reqci){		/* Check CI */
	reject_if_disagree = (f->nakloops >= f->maxnakloops);
	code = (*f->callbacks->reqci)(f, inp, &len, reject_if_disagree);
    } else if (len)
	code = CONFREJ;			/* Reject all CI */
    else
	code = CONFACK;
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
    switch (code)
    {
        case CONFREQ:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Configuration Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CFG_REQ);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Configuration Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CFG_REQ);
            }
            break;
            
        case CONFACK:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Configuration ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CFG_ACK);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Configuration ACK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CFG_ACK);
            }
            break;

       case CONFNAK:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Configuration NAK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CFG_NAK);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Configuration NAK");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CFG_NAK);
            }
            break;

       case CONFREJ:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Configuration Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CFG_REJECT);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Configuration Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CFG_REJECT);
            }
            break;

        case TERMREQ:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_REQ);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination Request");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_REQ);
            }
            break;

        case TERMACK:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination Ack");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_ACK);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination Ack");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_ACK);
            }
            break;

        case CODEREJ:
            if (PPP_IPCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Code Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CODE_REJECT);
            }
            else if (PPP_LCP == f->protocol)
            {
                //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Code Reject");
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CODE_REJECT);
            }
            break;

        default:
            break;
    }
#endif
    /* send the Ack, Nak or Rej to the peer */
    fsm_sdata(f, code, id, inp, len);

    if (code == CONFACK) {
	if (f->state == ACKRCVD) {
	    UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	    f->state = OPENED;
	    if (f->callbacks->up)
            {   
                if (PPP_LCP == f->protocol)
                {
                    /*HOMEGW-15416：为规避DT特定环境中，当网关连续发送ACK和
                认证报文的情况下，可能出现认证报文先到达服务器，导致服
                务器认为报文异常从而断开连接的情况，所以连续发送的情况
                下，中间间隔3ms*/
                    usleep(3000);
                }
                (*f->callbacks->up)(f);	/* Inform upper layers */
            }
	} else
	    f->state = ACKSENT;
	f->nakloops = 0;

    } else {
	/* we sent CONFACK or CONFREJ */
	if (f->state != ACKRCVD)
	    f->state = REQSENT;
	if( code == CONFNAK )
	    ++f->nakloops;
    }
}


/*
 * fsm_rconfack - Receive Configure-Ack.
 */
static void
fsm_rconfack(f, id, inp, len)
    fsm *f;
    int id;
    u_char *inp;
    int len;
{
    if (id != f->reqid || f->seen_ack)		/* Expected id? */
	return;					/* Nope, toss... */
    if( !(f->callbacks->ackci? (*f->callbacks->ackci)(f, inp, len):
	  (len == 0)) ){
	/* Ack is bad - ignore it */
	error("Received bad configure-ack: %P", inp, len);
	return;
    }
    f->seen_ack = 1;

    switch (f->state) {
    case CLOSED:
    case STOPPED:
	fsm_sdata(f, TERMACK, id, NULL, 0);
	break;

    case REQSENT:
	f->state = ACKRCVD;
	f->retransmits = f->maxconfreqtransmits;
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
	if ((STAGE_WAIT600S == f->dt_ncp_stage) && (PPP_IPCP==f->protocol || PPP_IPV6CP == f->protocol))
	{
	    UNTIMEOUT(dt_wait_last_penalty, f);
	}
#endif
	break;

    case ACKRCVD:
	/* Huh? an extra valid Ack? oh well... */
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	fsm_sconfreq(f, 0);
	f->state = REQSENT;
	break;

    case ACKSENT:
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	f->state = OPENED;
	f->retransmits = f->maxconfreqtransmits;
        if (f->callbacks->up)
	        (*f->callbacks->up)(f);	/* Inform upper layers */
	break;

    case OPENED:
	/* Go down and restart negotiation */
	if (f->callbacks->down)
	    (*f->callbacks->down)(f);	/* Inform upper layers */
	fsm_sconfreq(f, 0);		/* Send initial Configure-Request */
	f->state = REQSENT;
	break;
    }
}


/*
 * fsm_rconfnakrej - Receive Configure-Nak or Configure-Reject.
 */
static void
fsm_rconfnakrej(f, code, id, inp, len)
    fsm *f;
    int code, id;
    u_char *inp;
    int len;
{
    int (*proc) __P((fsm *, u_char *, int));
    int ret;

    if (id != f->reqid || f->seen_ack)	/* Expected id? */
	return;				/* Nope, toss... */
    proc = (code == CONFNAK)? f->callbacks->nakci: f->callbacks->rejci;
    if (!proc || !(ret = proc(f, inp, len))) {
	/* Nak/reject is bad - ignore it */
	error("Received bad configure-nak/rej: %P", inp, len);
	return;
    }
    f->seen_ack = 1;

    switch (f->state) {
    case CLOSED:
    case STOPPED:
	fsm_sdata(f, TERMACK, id, NULL, 0);
	break;

    case REQSENT:
    case ACKSENT:
	/* They didn't agree to what we wanted - try another request */
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
	if ((STAGE_WAIT600S == f->dt_ncp_stage) && (PPP_IPCP==f->protocol || PPP_IPV6CP == f->protocol))
	{
	    UNTIMEOUT(dt_wait_last_penalty, f);
	}
#endif
	if (ret < 0)
	    f->state = STOPPED;		/* kludge for stopping CCP */
	else
	    fsm_sconfreq(f, 0);		/* Send Configure-Request */
	break;

    case ACKRCVD:
	/* Got a Nak/reject when we had already had an Ack?? oh well... */
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	fsm_sconfreq(f, 0);
	f->state = REQSENT;
	break;

    case OPENED:
	/* Go down and restart negotiation */
	if (f->callbacks->down)
	    (*f->callbacks->down)(f);	/* Inform upper layers */
	fsm_sconfreq(f, 0);		/* Send initial Configure-Request */
	f->state = REQSENT;
	break;
    }
}


/*
 * fsm_rtermreq - Receive Terminate-Req.
 */
static void
fsm_rtermreq(f, id, p, len)
    fsm *f;
    int id;
    u_char *p;
    int len;
{
    switch (f->state) {
    case ACKRCVD:
    case ACKSENT:
	f->state = REQSENT;		/* Start over but keep trying */
	break;

    case OPENED:
	if (len > 0) {
	    info("%s terminated by peer (%0.*v)", PROTO_NAME(f), len, p);
	} else
	    info("%s terminated by peer", PROTO_NAME(f));
	f->retransmits = 0;
	f->state = STOPPING;
	if (f->callbacks->down)
	    (*f->callbacks->down)(f);	/* Inform upper layers */
	TIMEOUT(fsm_timeout, f, f->timeouttime);
	break;
    }
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
    if (PPP_IPCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Termination ACK");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_TERM_ACK);
    }
    else if (PPP_LCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Termination ACK");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_TERM_ACK);
    }
	#endif
    fsm_sdata(f, TERMACK, id, NULL, 0);
}


/*
 * fsm_rtermack - Receive Terminate-Ack.
 */
static void
fsm_rtermack(f)
    fsm *f;
{
    switch (f->state) {
    case CLOSING:
	UNTIMEOUT(fsm_timeout, f);
	f->state = CLOSED;
	if( f->callbacks->finished )
	    (*f->callbacks->finished)(f);
	break;
    case STOPPING:
	UNTIMEOUT(fsm_timeout, f);
	f->state = STOPPED;
	if( f->callbacks->finished )
	    (*f->callbacks->finished)(f);
	break;

    case ACKRCVD:
	f->state = REQSENT;
	break;

    case OPENED:
	if (f->callbacks->down)
	    (*f->callbacks->down)(f);	/* Inform upper layers */
	fsm_sconfreq(f, 0);
	f->state = REQSENT;
	break;
    }
}


/*
 * fsm_rcoderej - Receive an Code-Reject.
 */
static void
fsm_rcoderej(f, inp, len)
    fsm *f;
    u_char *inp;
    int len;
{
    u_char code, id;

    if (len < HEADERLEN) {
	FSMDEBUG(("fsm_rcoderej: Rcvd short Code-Reject packet!"));
	return;
    }
    GETCHAR(code, inp);
    GETCHAR(id, inp);
    warn("%s: Rcvd Code-Reject for code %d, id %d", PROTO_NAME(f), code, id);

    if( f->state == ACKRCVD )
	f->state = REQSENT;
}


/*
 * fsm_protreject - Peer doesn't speak this protocol.
 *
 * Treat this as a catastrophic error (RXJ-).
 */
void
fsm_protreject(f)
    fsm *f;
{
    switch( f->state ){
    case CLOSING:
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	/* fall through */
    case CLOSED:
	f->state = CLOSED;
	if( f->callbacks->finished )
	    (*f->callbacks->finished)(f);
	break;

    case STOPPING:
    case REQSENT:
    case ACKRCVD:
    case ACKSENT:
	UNTIMEOUT(fsm_timeout, f);	/* Cancel timeout */
	/* fall through */
    case STOPPED:
	f->state = STOPPED;
	if( f->callbacks->finished )
	    (*f->callbacks->finished)(f);
	break;

    case OPENED:
	terminate_layer(f, STOPPING);
	break;

    default:
	FSMDEBUG(("%s: Protocol-reject event in state %d!",
		  PROTO_NAME(f), f->state));
    }
}


/*
 * fsm_sconfreq - Send a Configure-Request.
 */
static void
fsm_sconfreq(f, retransmit)
    fsm *f;
    int retransmit;
{
    u_char *outp;
    int cilen;

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    int dt_ncp_trying = 0;
    int proto = 0;

    if (PPP_IPCP==f->protocol || PPP_IPV6CP==f->protocol)
    {
        dt_ncp_trying = 1;//在进行ipcp 或ipv6cp的发送
        proto = (PPP_IPCP==f->protocol)? PPP_IP : PPP_IPV6;
        f->retransmits = 0;/*ipcp和ipv6cp重试都不用原来状态机里的计数器*/
    }
#endif

    if( f->state != REQSENT && f->state != ACKRCVD && f->state != ACKSENT ){
	/* Not currently negotiating - reset options */
	if( f->callbacks->resetci )
	    (*f->callbacks->resetci)(f);
	f->nakloops = 0;
    }

    if( !retransmit ){
	/* New request - reset retransmission counter, use new ID */
	f->retransmits = f->maxconfreqtransmits;
	f->reqid = ++f->id;
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
	/*proto非0，则为ipcp或ipv6cp, retransmit=0, 说明是首次, 置初始值*/
	if (proto)
	{
	    f->burst_counter = 0;
	    f->penalty_counter = 0;
        f->penalty_burst_counter = 0;
        f->last_burst_counter = 0;
		/*start:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
		f->ipv6_P004_cnt = 0;
		f->ipv6_P004_flag = 0;
		/*End:z00182249 for UDK0000287513：P004记录条件为当网关发的ipv6cp报文服务器没有回复时20130909*/
	}
#endif
    }
	/*start of protocol WAN <3.1.3IPCP阶段ACK报文丢失> porting by s60000658 20060505*/
    else if((unsigned char)1 == f->seen_ack)
    {
        f->retransmits = 1;
    }
	/*end of protocol WAN <3.1.3IPCP阶段ACK报文丢失> porting by s60000658 20060505*/
    
    f->seen_ack = 0;

    /*
     * Make up the request packet
     */
    outp = outpacket_buf + PPP_HDRLEN + HEADERLEN;
    if( f->callbacks->cilen && f->callbacks->addci ){
	cilen = (*f->callbacks->cilen)(f);
	if( cilen > peer_mru[f->unit] - HEADERLEN )
	    cilen = peer_mru[f->unit] - HEADERLEN;
	if (f->callbacks->addci)
	    (*f->callbacks->addci)(f, outp, &cilen);
    } else
	cilen = 0;

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    if (retransmit && dt_ncp_trying)
    {
        //5个retry burst报文发完了，又走到此处,准备发第一个penalty报文前
        //若无其它NCP协议在运行，则断掉pppoe重来
        if ((DT_BURST_NUM + 1 == f->burst_counter) && (STAGE_BURST == f->dt_ncp_stage))
        {        
/*Start of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036: IPCP/IPV6CP are both failed in burst stage, HG must terminate PPP*/
            if (PPP_IPCP == f->protocol)
            {
                burst_over_flag |= IPCP_BURST_OVER;
            }
            else if(PPP_IPV6CP == f->protocol)
            {
                burst_over_flag |= IPV6CP_BURST_OVER;
            }
            if (burst_over_flag == (IPCP_BURST_OVER | IPV6CP_BURST_OVER))
            {
                num_np_open = 1;//赋值为1，让np_finished断掉lcp
            }
/*End of ATP 2012-11-14 for CR-DynIPv4_W724v by t00189036*/            
            
            np_finished(f->unit, proto);/*此函数里面num_np_open会减一*/
            if (num_np_open <= 0)
            {
                //printf("%s %d, no NCP running after burst retry fail, return ! phase=%d\n", __func__, __LINE__, phase);
                return;
            }
            //接下来用fsm_sdata发penalty协商报文
            num_np_open++;
            //printf("%s %d, num_np_open+1,  now=%d\n", __func__, __LINE__, num_np_open);
        }
    }
#endif

    /* send the request to our peer */
#ifdef SUPPORT_ATP_PPP_LOG_BT_HHUB3
    if (PPP_IPCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP IPCP Send Configuration Request");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_IPCP_SEND_CFG_REQ);
    }
    else if (PPP_LCP == f->protocol)
    {
        //ATP_LOG_Printf(ATP_LOG_TYPE_WAN, ATP_LOG_LEVEL_NOTICE, 1, "PPP LCP Send Configuration Request");
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_PPP_LCP_SEND_CFG_REQ);
    }
#endif

    /*burst阶段的最后一次超时的时候不发包*/
    if (!((DT_BURST_NUM + 1 == f->burst_counter) && (STAGE_BURST == f->dt_ncp_stage)))
    {
        fsm_sdata(f, CONFREQ, f->reqid, outp, cilen);
    }

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    /*for W724v*/
    /*burst retry 阶段，发包时间点秒数, 0 1 2 4 8 16, */
    /*burst失败，进入penalty阶段，发包时间点秒数, 0 1  2 4 8 16 32 64 128 256*/
    if (dt_ncp_trying)
    {   
        if (f->burst_counter < DT_BURST_NUM + 1) /*retry burst阶段，除初始包外，另发5个重试包*/
        {
            f->dt_ncp_stage = STAGE_BURST;
            f->retry_interval = burst_intervals[f->burst_counter++];
            //printf("[%s burst try] retry_interval=%d\n", PROTO_NAME(f), f->retry_interval);
        }
        else if (f->penalty_counter < DT_PENALTY_NUM) /*penalty 阶段，先发1个，再发9个重试报文，再等待600s后，等待下次触发*/
        {
            f->dt_ncp_stage = STAGE_PENALTY;
            //f->retry_interval = penalty_intervals[f->penalty_counter++];
            f->retry_interval = penalty_intervals[f->penalty_counter][f->penalty_burst_counter++];  
            //printf("[%s penalty try] retry_interval=%d\n", PROTO_NAME(f), f->retry_interval);   
            if (DT_BURST_PACKET_NUM == f->penalty_burst_counter)
            {
                f->penalty_counter++;
                f->penalty_burst_counter = 0;
            }
        }
        else
        {
            /*retry 和penalty都发完了,等3s收最后一个penalty包的回应，若无回应，则600s-3s时间内都保持不拨号状态*/
             //3s后执行dt_wait_last_penalty,若之间收到了回应，则该定时函数会被删除，不需执行，见rconfack, rconfnakrej
            f->dt_ncp_stage = STAGE_WAIT600S;
            //printf("sent last penalty try, wait 3s for  reply\n");
            //TIMEOUT(dt_wait_last_penalty, f, DEFTIMEOUT);
            f->retry_interval = last_intervals[f->last_burst_counter++];    
            //printf("[%s last try] retry_interval=%d\n", PROTO_NAME(f), f->retry_interval);               
            if (DT_BURST_PACKET_NUM == f->last_burst_counter)
            {
                f->last_burst_counter = 0;
                f->ipxcp_R020_flag = 1;
            }
            //return;
        }
    }
#endif

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    /*DT lcp的报文重试间隔为tocpe0056算法alg_01, 根据剩下的重发个数得到重发间隔, chap, pap一样*/
    if (PPP_LCP == f->protocol || PPP_CHAP == f->protocol || PPP_PAP == f->protocol)
    {
        f->timeouttime = lcp_intervals[f->retransmits];
    }
    /*ipcp 和ipv6cp之外的报文按原来的重试逻辑*/
    if ((f->protocol != PPP_IPCP) && (f->protocol != PPP_IPV6CP))
    {
#endif
    /* start the retransmit timer */
    --f->retransmits;
    TIMEOUT(fsm_timeout, f, f->timeouttime);
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    }
    else if (dt_ncp_trying)
    {
        if (STAGE_BURST== f->dt_ncp_stage || STAGE_PENALTY == f->dt_ncp_stage || STAGE_WAIT600S == f->dt_ncp_stage)
        {
        #if 0
            if (STAGE_BURST== f->dt_ncp_stage)
                printf("schedule BURST %dth, send after %ds, num_np_open=%d\n", f->burst_counter, f->retry_interval, num_np_open);
            else
                printf("schedule PENALTY %dth, send after %ds, num_np_open=%d\n", f->penalty_counter, f->retry_interval, num_np_open);
        #endif        
            TIMEOUT(fsm_timeout, f, f->retry_interval);
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
#endif
}


/*
 * fsm_sdata - Send some data.
 *
 * Used for all packets sent to our peer by this module.
 */
void
fsm_sdata(f, code, id, data, datalen)
    fsm *f;
    u_char code, id;
    u_char *data;
    int datalen;
{
    u_char *outp;
    int outlen;

    /* Adjust length to be smaller than MTU */
    outp = outpacket_buf;
    if (datalen > peer_mru[f->unit] - HEADERLEN)
	datalen = peer_mru[f->unit] - HEADERLEN;
    if (datalen && data != outp + PPP_HDRLEN + HEADERLEN)
	BCOPY(data, outp + PPP_HDRLEN + HEADERLEN, datalen);
    outlen = datalen + HEADERLEN;
    MAKEHEADER(outp, f->protocol);
    PUTCHAR(code, outp);
    PUTCHAR(id, outp);
    PUTSHORT(outlen, outp);
    output(f->unit, outpacket_buf, outlen + PPP_HDRLEN);
}

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
//for W724v
/*penalty发完了且没收到任何回应，等待600s-3s后将对应的协议置为可发送拨号包状态*/
static void
dt_penalty_end(arg)
    void *arg;
{
    fsm *f = (fsm *) arg;

   // printf("600s penalty timeout, allow %s again!\n", PROTO_NAME(f));
    f->dt_ncp_stage = STAGE_WAITING_TRIGGER;
    f->ncp_enable = 1;
    /*如果是一直在线模式并且是v6的拨号重试，则不需要等ipv6报文触发，直接自动再拨号*/
    if (!demand && PPP_IPV6CP == f->protocol)
    {
        f->state = CLOSED;
        fsm_open(f);
        num_np_open++;
        //printf("%s %d, num_np_open+1,  now=%d\n", __func__, __LINE__, num_np_open);
        //printf("ipv6 penaly end, auto do ipv6 dialing\n");
    }
}

//执行该函数，说明最后的penalty报文都没有在3s内收到回应，则按dt要求等600s
static void
dt_wait_last_penalty(arg)
    void *arg;
{
    fsm *f = (fsm *) arg;

    //printf("%s %d  called for not recv any %s reply!\n", __func__, __LINE__, PROTO_NAME(f));
    //UNTIMEOUT(fsm_timeout, f);
    //置0，600s(DT_PENALTY_TIMEOUT)内不允许ipcp或ipv6cp协商
    //printf("let f->ncp_enable=0, keep it for 600s\n");
    f->ncp_enable = 0;
    //等待600s
    TIMEOUT(dt_penalty_end, f, DT_PENALTY_TIMEOUT - DEFTIMEOUT);
    --num_np_open;
    //printf("%s %d, num_np_open-1,  now=%d\n", __func__, __LINE__, num_np_open);
}
#endif

