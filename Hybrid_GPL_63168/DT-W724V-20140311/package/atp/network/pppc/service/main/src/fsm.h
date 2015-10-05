/*
 * fsm.h - {Link, IP} Control Protocol Finite State Machine definitions.
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
 *
 */

/*
 * Packet header = Code, id, length.
 */
#define HEADERLEN	4


/*
 *  CP (LCP, IPCP, etc.) codes.
 */
#define CONFREQ		1	/* Configuration Request */
#define CONFACK		2	/* Configuration Ack */
#define CONFNAK		3	/* Configuration Nak */
#define CONFREJ		4	/* Configuration Reject */
#define TERMREQ		5	/* Termination Request */
#define TERMACK		6	/* Termination Ack */
#define CODEREJ		7	/* Code Reject */


/*
 * Each FSM is described by an fsm structure and fsm callbacks.
 */
typedef struct fsm {
    int unit;			/* Interface unit number */
    int protocol;		/* Data Link Layer Protocol field value */
    int state;			/* State */
    int flags;			/* Contains option bits */
    u_char id;			/* Current id */
    u_char reqid;		/* Current request id */
    u_char seen_ack;		/* Have received valid Ack/Nak/Rej to Req */
    int timeouttime;		/* Timeout time in milliseconds */
    int maxconfreqtransmits;	/* Maximum Configure-Request transmissions */
    int retransmits;		/* Number of retransmissions left */
    int maxtermtransmits;	/* Maximum Terminate-Request transmissions */
    int nakloops;		/* Number of nak loops since last ack */
    int maxnakloops;		/* Maximum number of nak loops tolerated */
    struct fsm_callbacks *callbacks;	/* Callback routines */
    char *term_reason;		/* Reason for closing protocol */
    int term_reason_len;	/* Length of term_reason */

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
    //for W724v
    int ncp_enable; /*仅用于dt ipcp 和ipv6cp, 控制是否发拨号协商报文*/
    int dt_ncp_stage; /*dt ncp协商时处于的阶段*/
    int burst_counter; /*burst try时的计数器*/
    int penalty_counter; /*penalty try时的计数器*/
    int penalty_burst_counter; /*penalty burst try时的计数器*/   
    int last_burst_counter; /*last burst try时的计数器*/       
    int retry_interval; /*发retry 包间隔*/
	int ipv6_P004_cnt; /*当ppp一直重试发ipv6cp报文时，P004日志记录次数*/
	int ipv6_P004_flag;/*当ppp不重试发ipv6cp报文时，P004日志记录一次的标记*/
    int ipxcp_R020_flag;/*处于wait600s阶段一轮alg_01超时后记录R020标志*/
    //for W724v end
#endif

} fsm;


typedef struct fsm_callbacks {
    void (*resetci)		/* Reset our Configuration Information */
		__P((fsm *));
    int  (*cilen)		/* Length of our Configuration Information */
		__P((fsm *));
    void (*addci) 		/* Add our Configuration Information */
		__P((fsm *, u_char *, int *));
    int  (*ackci)		/* ACK our Configuration Information */
		__P((fsm *, u_char *, int));
    int  (*nakci)		/* NAK our Configuration Information */
		__P((fsm *, u_char *, int));
    int  (*rejci)		/* Reject our Configuration Information */
		__P((fsm *, u_char *, int));
    int  (*reqci)		/* Request peer's Configuration Information */
		__P((fsm *, u_char *, int *, int));
    void (*up)			/* Called when fsm reaches OPENED state */
		__P((fsm *));
    void (*down)		/* Called when fsm leaves OPENED state */
		__P((fsm *));
    void (*starting)		/* Called when we want the lower layer */
		__P((fsm *));
    void (*finished)		/* Called when we don't want the lower layer */
		__P((fsm *));
    void (*protreject)		/* Called when Protocol-Reject received */
		__P((int));
    void (*retransmit)		/* Retransmission is necessary */
		__P((fsm *));
    int  (*extcode)		/* Called when unknown code received */
		__P((fsm *, int, int, u_char *, int));
    char *proto_name;		/* String name for protocol (for messages) */
} fsm_callbacks;


/*
 * Link states.
 */
#define INITIAL		0	/* Down, hasn't been opened */
#define STARTING	1	/* Down, been opened */
#define CLOSED		2	/* Up, hasn't been opened */
#define STOPPED		3	/* Open, waiting for down event */
#define CLOSING		4	/* Terminating the connection, not open */
#define STOPPING	5	/* Terminating, but open */
#define REQSENT		6	/* We've sent a Config Request */
#define ACKRCVD		7	/* We've received a Config Ack */
#define ACKSENT		8	/* We've sent a Config Ack */
#define OPENED		9	/* Connection available */


/*
 * Flags - indicate options controlling FSM operation
 */
#define OPT_PASSIVE	1	/* Don't die if we don't get a response */
#define OPT_RESTART	2	/* Treat 2nd OPEN as DOWN, UP */
#define OPT_SILENT	4	/* Wait for peer to speak first */

#ifdef SUPPORT_ATP_PPP_DYN_DIAL
/*for W724v  NCP重试算法*/
#define DT_BURST_NUM  5
#define DT_BURST_PACKET_NUM  6
#define DT_PENALTY_NUM 9
#define DT_PENALTY_TIMEOUT  600
//#define DT_PENALTY_NUM 5  /*改为5，缩短调试等待时间*/
//#define DT_PENALTY_TIMEOUT  10   /*测试，缩短等待时间为10s*/

#define STAGE_BURST        0
#define STAGE_PENALTY    1
#define STAGE_WAIT600S   2
#define STAGE_WAITING_TRIGGER 3
/*for W724 end*/
#endif
/*
 * Timeouts.
 */
#define DEFTIMEOUT	3	/* Timeout time in seconds */
#define DEFMAXTERMREQS	2	/* Maximum Terminate-Request transmissions */
#define DEFMAXCONFREQS	20	/* Maximum Configure-Request transmissions */
#define DEFMAXNAKLOOPS	5	/* Maximum number of nak loops */
/* BEGIN: Added by y67514, 2008/12/23   PN:ET128*/
#define USBMAXCONFREQS      5       /* Maximum Configure-Request transmissions when use PPOU*/
/* END:   Added by y67514, 2008/12/23 */

/*
 * Prototypes
 */
void fsm_init __P((fsm *));
void fsm_lowerup __P((fsm *));
void fsm_lowerdown __P((fsm *));
void fsm_open __P((fsm *));
void fsm_close __P((fsm *, char *));
void fsm_input __P((fsm *, u_char *, int));
void fsm_protreject __P((fsm *));
void fsm_sdata __P((fsm *, int, int, u_char *, int));


/*
 * Variables
 */
extern int peer_mru[];		/* currently negotiated peer MRU (per unit) */
