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
    char                      if_name[IFNAMSIZ];    // WAN�ӿ�����
    int                       if_ops;       // ��WAN�ӿڵ����ͣ��Žӻ���·�ɣ�����MCPD_IF_TYPE_BRIDGED
} t_MCPD_WAN_INFO;


typedef struct mcpd_igmp_snoop_entry 
{
    char                      br_name[IFNAMSIZ];
    //char                      port_name[IFNAMSIZ];
    int                       port_no;      // ��Ӧ���Žӿ�Ҳ�����ĸ����յ���IGMP���ĵ�
    struct                    in_addr grp;  // ��Ӧ���鲥��ַ
    struct                    in_addr src;  // ��Ӧ��Դ������INCLUDE�ĸ�Դ��ַ
    struct                    in_addr rep;  // report�����ߵĵ�ַ
    int                       mode;         // ��Ҫ���Ĳ���
    int                       code;
    __u16                     tci;          /* vlan id �ñ��ĵ�VLAN */
    t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];  // ���е�ֻҪ�ǿ�����IGMP���ܵ�WAN�ӿ�
} t_MCPD_IGMP_SNOOP_ENTRY;

void mcpd_nl_process_igmp_snoop_entry_api(t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry);

#endif
