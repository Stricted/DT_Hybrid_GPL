#ifndef __BR_NETLINK_MCPD_H__
#define __BR_NETLINK_MCPD_H__

#include <linux/if.h>
#include <linux/in.h>


#define MCPD_SNOOP_IN_ADD    1
#define MCPD_SNOOP_IN_CLEAR  2
#define MCPD_SNOOP_EX_ADD    3
#define MCPD_SNOOP_EX_CLEAR  4


#define MCPD_MAX_IFS            10


typedef struct mcpd_wan_info
{
    char                      if_name[IFNAMSIZ];    // WAN接口名字
    int                       if_ops;       // 该WAN接口的类型，桥接还是路由，比如MCPD_IF_TYPE_BRIDGED
} t_MCPD_WAN_INFO;


typedef struct mcpd_igmp_snoop_entry 
{
    char                      br_name[IFNAMSIZ];
    //char                      port_name[IFNAMSIZ];
    int                       port_no;      // 对应的桥接口也就是哪个口收到该IGMP报文的
    struct                    in_addr grp;  // 对应的组播地址
    struct                    in_addr src;  // 对应的源，比如INCLUDE哪个源地址
    struct                    in_addr rep;  // report报告者的地址
    int                       mode;         // 需要做的操作
    int                       code;
    __u16                     tci;          /* vlan id 该报文的VLAN */
    t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];  // 所有的只要是开启了IGMP功能的WAN接口
} t_MCPD_IGMP_SNOOP_ENTRY;

void mcpd_nl_process_igmp_snoop_entry_api(t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry);

#endif
