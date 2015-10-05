#ifndef __BR_IGMP_PARSE_H__
#define __BR_IGMP_PARSE_H__

#include <linux/skbuff.h>
#include "br_private.h"

void br_igmp_process(struct net_bridge *br, struct sk_buff *skb);

#endif
