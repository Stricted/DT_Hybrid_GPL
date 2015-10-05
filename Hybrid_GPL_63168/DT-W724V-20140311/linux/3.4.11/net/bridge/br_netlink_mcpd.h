#ifndef __BR_NETLINK_MCPD_H__
#define __BR_NETLINK_MCPD_H__

#include <linux/if.h>
#include <linux/in.h>


#define MCPD_SNOOP_IN_ADD    1
#define MCPD_SNOOP_IN_CLEAR  2
#define MCPD_SNOOP_EX_ADD    3
#define MCPD_SNOOP_EX_CLEAR  4


#define MCPD_MAX_IFS            10



void mcpd_nl_process_igmp_snoop_entry_api(t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry);

#endif
