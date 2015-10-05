#ifndef _IPT_TRIGGER_H_target
#define _IPT_TRIGGER_H_target

#ifndef ATP_SUPPORT_PRTT_RPROTOCOL
#define ATP_SUPPORT_PRTT_RPROTOCOL
#endif

#define TRIGGER_TIMEOUT 600	/* 600 secs */

enum ipt_trigger_type
{
	IPT_TRIGGER_DNAT = 1,
	IPT_TRIGGER_IN = 2,
	IPT_TRIGGER_OUT = 3
};

struct ipt_trigger_ports {
	u_int16_t mport[2];	/* Related destination port range */
	u_int16_t rport[2];	/* Port range to map related destination port range to */
};

struct ipt_trigger_info {
	enum ipt_trigger_type type;
	u_int16_t proto;	/* Related protocol */
    
    /* 增加了对related协议的分类限制 */
    #ifdef ATP_SUPPORT_PRTT_RPROTOCOL
    u_int16_t rproto;
    #endif
    
	struct ipt_trigger_ports ports;
};

#endif /*_IPT_TRIGGER_H_target*/
