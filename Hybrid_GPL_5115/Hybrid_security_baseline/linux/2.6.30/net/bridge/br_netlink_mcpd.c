#include <linux/socket.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/in.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include "br_igmp.h"
#include "br_mld.h"
#include "br_netlink_mcpd.h"


//#define BR_DEV_DEBUG
#ifdef BR_DEV_DEBUG
#define br_netlink_debug(args...)   color_printk(__func__, __LINE__, _RET_IP_, ## args)
#else
#define br_netlink_debug(args...)
#endif



#define MAX_PAYLOAD             1024


#if 0
static DEFINE_RWLOCK(mcpd_queue_lock);
static LIST_HEAD(mcpd_queue_list);
static DEFINE_MUTEX(mcpd_mutex);
static unsigned int mcpd_queue_total;
static unsigned int mcpd_queue_dropped = 0;
static unsigned int mcpd_queue_user_dropped = 0;
static unsigned int mcpd_queue_maxlen = 64;
#endif

static struct sock *nl_sk = NULL;
static int mcpd_pid = 0;



struct mcpd_queue_entry 
{
    struct list_head list;
    struct sk_buff *skb;
    struct net_bridge *br;
};

typedef struct mcpd_msg_hdr 
{
    __u16 type;
    __u16 len;
} t_MCPD_MSG_HDR;

typedef enum mcpd_msgtype 
{
    MCDP_MSG_BASE = 0,
    MCPD_MSG_REGISTER, /* usr - > krnl -> usr */
    MCPD_MSG_VERDICT,
    MCPD_MSG_IGMP_PKT, /* krnl -> usr */
    MCPD_MSG_IGMP_SNOOP_ENTRY,
    MCPD_MSG_IGMP_SNOOP_ADD,
    MCPD_MSG_IGMP_SNOOP_DEL,
    MCPD_MSG_IGMP_LAN2LAN_SNOOP,
    MCPD_MSG_MLD_PKT, /* krnl -> usr */
    MCPD_MSG_MLD_SNOOP_ENTRY,
    MCPD_MSG_MLD_SNOOP_ADD,
    MCPD_MSG_MLD_SNOOP_DEL,
    MCPD_MSG_MLD_LAN2LAN_SNOOP,
    MCPD_MSG_IGMP_PURGE_ENTRY,
    MCPD_MSG_MLD_PURGE_ENTRY,
    MCPD_MSG_IF_CHANGE,
    MCPD_MSG_MC_FDB_CLEANUP, /* clean up for MIB RESET */
    MCPD_MSG_MAX
} t_MCPD_MSGTYPES;

typedef enum mcpd_ret_codes 
{
    MCPD_SUCCESS = 0,
    MCPD_GEN_ERR = 1,
    MCPD_RET_MEMERR = 2,
    MCPD_RET_ACCEPT = 3,
    MCPD_RET_DROP   = 4
} t_MCPD_RET_CODE;

typedef enum mcpd_proto_type
{
    MCPD_PROTO_IGMP = 0,
    MCPD_PROTO_MLD  = 1,
    MCPD_PROTO_MAX  = 2,
} t_MCPD_PROTO_TYPE;

typedef struct mcpd_register 
{
    int code;
} t_MCPD_REGISTER;

typedef struct mcpd_verdict
{
    __u32                     corr_tag;
    __u32                     code;
} t_MCPD_VERDICT;


// 注意，本结构体后面紧跟着的就是skb的数据
typedef struct mcpd_pkt_info
{
    char                      br_name[IFNAMSIZ];        // 该组播报文属于那个桥
    char                      port_name[IFNAMSIZ];      // 该组播报文从该桥的哪个具体接口收到的
    int                       port_no;                  // 该桥接口的编号
    int                       if_index;                 // 对应的该桥的接口index
    int                       data_len;                 // 数据长度，也就是传递上来的igmp报文的长度
    __u32                     corr_tag;                 // 对应的skb指针
    __u16                     tci;/* vlan id */             // 该IGMP报文的vlan id
} t_MCPD_PKT_INFO;

typedef struct mcpd_mld_snoop_entry 
{
    char                      br_name[IFNAMSIZ];
    char                      port_no;
    struct                    in6_addr grp;
    struct                    in6_addr src;
    struct                    in6_addr rep;
    int                       mode;
    int                       code;
    __u16                     tci;/* vlan id */
    t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];
} t_MCPD_MLD_SNOOP_ENTRY;

typedef struct mcpd_igmp_purge_entry
{
    struct in_addr            grp;
    struct in_addr            src;
    struct in_addr            rep;
    t_MCPD_PKT_INFO           pkt_info;
} t_MCPD_IGMP_PURGE_ENTRY;

static void mcpd_dump_buf(char *buf, int len)
{
#if 0
    int i;
    printk("========================KRNEL BPEELA START===================================\n");
    for(i =0; i < len; i++) 
    {
     printk("%02x", (unsigned char)buf[i]);
     if(!((i+1)%2))
         printk(" ");
     if(!((i+1)%16))
       printk("\n");
    }
    printk("\n");
    printk("=======================KRNL BPEELA END====================================\n");
#endif
}

#ifdef dead_debug_code
/* called under bridge lock */
/* this function is not used anywhere.  debug code only? */
static struct net_bridge_port *br_get_port_by_name(struct net_bridge *br, 
                                                   char* port_name)
{
    struct net_bridge_port *p;

    rcu_read_lock();
    list_for_each_entry_rcu(p, &br->port_list, list) {
        if (!memcmp(p->dev->name, port_name, IFNAMSIZ))
            return p;
    }
    rcu_read_unlock();

    return NULL;
}
#endif /* dead_debug_code */

#if 0
static int mcpd_enqueue_packet(struct net_bridge *br, struct sk_buff *skb)
{
    int status = 0;
    struct mcpd_queue_entry *entry;

    /* TODO: this code should be above */
    if (mcpd_queue_total >= mcpd_queue_maxlen) 
    {
        mcpd_queue_dropped++;
        mcpd_queue_user_dropped++;
        status = -ENOSPC;
        if (net_ratelimit())
        printk (KERN_WARNING "full at %d entries, " 
                              "dropping packets(s). Dropped: %d\n", 
                              mcpd_queue_total, mcpd_queue_dropped);
        goto err_code1;
    }

    entry = kmalloc(sizeof(struct mcpd_queue_entry), GFP_ATOMIC);

    if (entry == NULL) 
    {
        printk(KERN_ERR "mcpd_enqueue_packet()\n");
        return -ENOMEM;
    }

    entry->skb = skb;
    entry->br = br;

    write_lock_bh(&mcpd_queue_lock);
    list_add(&entry->list, &mcpd_queue_list);
    write_unlock_bh(&mcpd_queue_lock);

    mcpd_queue_total++;

    err_code1:

    return status;

} /* mcpd_enqueue_packet */
#endif


/*mcpd是bcm自己的igmp proxy进程，我们没用到*/
// 收到一个报文skb，对齐进行处理，也就是说将这个报文发送给上面的MCPD处理
int mcpd_process_skb(struct net_bridge *br, struct sk_buff *skb, int protocol)
{
    struct nlmsghdr *nlh;
    char *ptr = NULL;
    struct sk_buff *new_skb;
    t_MCPD_PKT_INFO *pkt_info;
    int buf_size;
    char *br_name = br->dev->name;
    int if_index = br->dev->ifindex;
    int port_no = skb->dev->br_port->port_no;
    int len;
    u8 *pData;

    if(!mcpd_pid)
        return 0;

    len   = skb->len;
    pData = skb->data;
    if (vlan_eth_hdr(skb)->h_vlan_proto != protocol)
    {
        if ( vlan_eth_hdr(skb)->h_vlan_proto == ETH_P_8021Q )
        {
            if ( vlan_eth_hdr(skb)->h_vlan_encapsulated_proto == protocol )
            {
                len   -= sizeof(struct vlan_hdr);
                pData += sizeof(struct vlan_hdr);
            }
            else
            {
                return 0;
            }
        }
    }

    buf_size = len + sizeof(t_MCPD_MSG_HDR) + sizeof(t_MCPD_PKT_INFO);
    new_skb  = alloc_skb(NLMSG_SPACE(buf_size), GFP_ATOMIC);

    if(!new_skb) {
        printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return 0;
    }

    nlh = (struct nlmsghdr *)new_skb->data;
    ptr = NLMSG_DATA(nlh);
    nlh->nlmsg_len = NLMSG_SPACE(buf_size);
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_flags = 0;
    skb_put(new_skb, NLMSG_SPACE(buf_size));
    if(protocol == ETH_P_IPV6)
        ((t_MCPD_MSG_HDR *)ptr)->type = MCPD_MSG_MLD_PKT;
    else
        ((t_MCPD_MSG_HDR *)ptr)->type = MCPD_MSG_IGMP_PKT;
    ((t_MCPD_MSG_HDR *)ptr)->len = sizeof(t_MCPD_PKT_INFO);
    ptr += sizeof(t_MCPD_MSG_HDR);

    pkt_info = (t_MCPD_PKT_INFO *)ptr;

    memcpy(pkt_info->br_name, br_name, IFNAMSIZ);
    memcpy(pkt_info->port_name, skb->dev->name, IFNAMSIZ);
    pkt_info->port_no = port_no;
    pkt_info->if_index = if_index;
    pkt_info->data_len = len;
    pkt_info->corr_tag = (__u32)skb;
    pkt_info->tci = 0;

#if defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE)
    if(skb->vlan_count)
        pkt_info->tci = (skb->vlan_header[0] >> 16);
#endif /*  defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE) */
    ptr += sizeof(t_MCPD_PKT_INFO);

    memcpy(ptr, pData, len);

    NETLINK_CB(new_skb).dst_group = 0;
    NETLINK_CB(new_skb).pid = mcpd_pid;
    mcpd_dump_buf((char *)nlh, 128);

    netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);

#if 0
    mcpd_enqueue_packet(br, skb);
#endif

    return 1;
} /* mcpd_process_skb */
EXPORT_SYMBOL(mcpd_process_skb);


// 处理收到的register报文，这个消息就是用户态的mcpd进程发送消息给内核告诉内核对应的pid
static void mcpd_nl_process_registration(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    struct sk_buff *new_skb = NULL;
    char *new_ptr = NULL;
    struct nlmsghdr *new_nlh = NULL;
    int buf_size;

    mcpd_pid = nlh->nlmsg_pid;

    buf_size = NLMSG_SPACE((sizeof(t_MCPD_MSG_HDR) + sizeof(t_MCPD_REGISTER)));

    new_skb = alloc_skb(buf_size, GFP_ATOMIC);

    if(!new_skb) {
        printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return;
    }

    ptr = NLMSG_DATA(nlh);

    if(new_skb)
    {
        new_nlh = (struct nlmsghdr *)new_skb->data;
        new_ptr = NLMSG_DATA(new_nlh);
        new_nlh->nlmsg_len = buf_size;
        new_nlh->nlmsg_pid = 0;
        new_nlh->nlmsg_flags = 0;
        skb_put(new_skb, buf_size);
        ((t_MCPD_MSG_HDR *)new_ptr)->type = MCPD_MSG_REGISTER;
        ((t_MCPD_MSG_HDR *)new_ptr)->len = sizeof(t_MCPD_REGISTER);
        new_ptr += sizeof(t_MCPD_MSG_HDR);
        ((t_MCPD_REGISTER *)new_ptr)->code = MCPD_SUCCESS;

        NETLINK_CB(new_skb).dst_group = 0;
        NETLINK_CB(new_skb).pid = mcpd_pid;


        netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);
    }

    return;
} /* mcpd_nl_process_registration */

// 处理收到LAN到LAN的snooping功能
static void mcpd_nl_process_lan2lan_snooping(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    unsigned char *ptr;
    int val;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    val = (int)*(int *)ptr;

    br_igmp_lan2lan_snooping_update(val);

    return;
}

static int mcpd_is_br_port(struct net_bridge *br,struct net_device *from_dev)
{
    struct net_bridge_port *p = NULL;
    int ret = 0;

    spin_lock_bh(&br->lock);
    list_for_each_entry_rcu(p, &br->port_list, list) {
        if ((p->dev) && (!memcmp(p->dev->name, from_dev->name, IFNAMSIZ)))
            ret = 1;
    }
    spin_unlock_bh(&br->lock);

    return ret;
} /* br_igmp_is_br_port */

// 用户态的mcpd处理完内核的igmp之后，从而下发给snooping告诉snooping如何进行报文转发处理
static void mcpd_nl_process_igmp_snoop_entry(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    struct net_bridge_port *prt;
    t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry;
    unsigned char *ptr;
    struct net_device *from_dev= NULL;
    int idx = 0;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    snoop_entry = (t_MCPD_IGMP_SNOOP_ENTRY *)ptr;

    dev = dev_get_by_name(&init_net, snoop_entry->br_name);
        
    if(!dev)
        return;

    br = netdev_priv(dev);

    if(!br)
    {
        dev_put(dev);
        return;
    }

    rcu_read_lock();
    prt = br_get_port(br, snoop_entry->port_no);
    rcu_read_unlock();

    for(idx = 0; idx < MCPD_MAX_IFS; idx++)
    {
        if(snoop_entry->wan_info[idx].if_ops)
        {
            // 获取WAN接口
            from_dev = dev_get_by_name(&init_net, 
                                       snoop_entry->wan_info[idx].if_name);
            if (!from_dev)
               continue;

            if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
                (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
            {
                // 这个函数的各个参数意义，from_dev表示对应的WAN接口，
                // if_ops表示该WAN接口对应的标记，桥接还是路由
                // br表示对应的桥
                // prt表示对应的IGMP报文的口
                // grp表示对应的组播地址
                // rep表示对应的报告者的地址
                // mode表示要做的操作，是添加还是删除等
                // src表示对应的源
                br_igmp_mc_fdb_remove(from_dev,
                                  br, 
                                  prt, 
                                  &snoop_entry->grp, 
                                  &snoop_entry->rep, 
                                  snoop_entry->mode, 
                                  &snoop_entry->src);
            }
            else
            {
                if((snoop_entry->wan_info[idx].if_ops == MCPD_IF_TYPE_BRIDGED) && 
                   (!mcpd_is_br_port(br, from_dev)))
                {
                   dev_put(from_dev);
                   continue;
                }

                br_igmp_mc_fdb_add(from_dev,
                               snoop_entry->wan_info[idx].if_ops,
                               br, 
                               prt, 
                               &snoop_entry->grp, 
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci, 
                               &snoop_entry->src);
            }
            dev_put(from_dev);
        }
        else
        {
            break;
        }
    }

    /* if LAN-2-LAN snooping enabled make an entry */
    if(br_igmp_get_lan2lan_snooping_info())
    {
        if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
            (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
        {
            br_igmp_mc_fdb_remove(dev,
                                    br, 
                                    prt, 
                                    &snoop_entry->grp, 
                                    &snoop_entry->rep, 
                                    snoop_entry->mode, 
                                    &snoop_entry->src);
        }
        else
        {

            br_igmp_mc_fdb_add(dev,
                               MCPD_IF_TYPE_BRIDGED,
                               br, 
                               prt, 
                               &snoop_entry->grp, 
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci, 
                               &snoop_entry->src);
        }
    }

    if(dev)
        dev_put(dev);

    return;
} /* mcpd_nl_process_igmp_snoop_entry*/


void mcpd_nl_process_igmp_snoop_entry_api(t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry)
{
    int idx = 0;
    struct net_device *from_dev = NULL;
    struct net_bridge *br = NULL;
    struct net_device *dev = NULL;
    struct net_bridge_port *prt = NULL;

    dev = dev_get_by_name(&init_net, snoop_entry->br_name);    
    if(!dev)
    {
        br_netlink_debug("error cann't get dev %s\r\n", snoop_entry->br_name);
        return;
    }
    
    br = netdev_priv(dev);
    if(!br)
    {
        br_netlink_debug("netdev_priv error cann't get dev %s\r\n", snoop_entry->br_name);
        dev_put(dev);
        return;
    }
    
    rcu_read_lock();
    prt = br_get_port(br, snoop_entry->port_no);
    rcu_read_unlock();

    for(idx = 0; idx < MCPD_MAX_IFS; idx++)
    {
        br_netlink_debug("wan type %d\r\n", snoop_entry->wan_info[idx].if_ops);
        if(snoop_entry->wan_info[idx].if_ops)
        {
            // 获取WAN接口
            from_dev = dev_get_by_name(&init_net, 
                                       snoop_entry->wan_info[idx].if_name);

            if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
                (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
            {
                br_netlink_debug("now, begin to delete snooping entry\r\n");
                // 这个函数的各个参数意义，from_dev表示对应的WAN接口，
                // if_ops表示该WAN接口对应的标记，桥接还是路由
                // br表示对应的桥
                // prt表示对应的IGMP报文的口
                // grp表示对应的组播地址
                // rep表示对应的报告者的地址
                // mode表示要做的操作，是添加还是删除等
                // src表示对应的源
                br_igmp_mc_fdb_remove(from_dev,
                                  br, 
                                  prt, 
                                  &snoop_entry->grp, 
                                  &snoop_entry->rep, 
                                  snoop_entry->mode, 
                                  &snoop_entry->src);
            }
            else
            {
                br_netlink_debug("now, begin to add snooping entry\r\n");
                br_netlink_debug("from_dev:%s if_ops:%d br:%s\r\n",from_dev->name,snoop_entry->wan_info[idx].if_ops,br->dev->name);
                br_igmp_mc_fdb_add(from_dev,
                               snoop_entry->wan_info[idx].if_ops,
                               br, 
                               prt, 
                               &snoop_entry->grp, 
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci, 
                               &snoop_entry->src);
            }

            if(from_dev)
                dev_put(from_dev);
        }
        else
        {            
            br_netlink_debug("error not ops for wan ops\r\n");
            break;
        }
    }

    /* if LAN-2-LAN snooping enabled make an entry */
    if(br_igmp_get_lan2lan_snooping_info())
    {
        if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
            (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
        {
            br_igmp_mc_fdb_remove(dev,
                                    br, 
                                    prt, 
                                    &snoop_entry->grp, 
                                    &snoop_entry->rep, 
                                    snoop_entry->mode, 
                                    &snoop_entry->src);
        }
        else
        {

            br_igmp_mc_fdb_add(dev,
                               snoop_entry->wan_info[idx].if_ops,
                               br, 
                               prt, 
                               &snoop_entry->grp, 
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci, 
                               &snoop_entry->src);
        }
    }

    if(dev)
        dev_put(dev);

    return;
} /* mcpd_nl_process_igmp_snoop_entry*/

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
static void mcpd_nl_process_mld_lan2lan_snooping(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    unsigned char *ptr;
    int val;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    val = (int)*(int *)ptr;

    br_mld_lan2lan_snooping_update(val);

    return;
}

static void mcpd_nl_process_mld_snoop_entry(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    struct net_bridge_port *prt;
    t_MCPD_MLD_SNOOP_ENTRY *snoop_entry;
    unsigned char *ptr;
    struct net_device *from_dev= NULL;
    int idx = 0;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    snoop_entry = (t_MCPD_MLD_SNOOP_ENTRY *)ptr;

    dev = dev_get_by_name(&init_net, snoop_entry->br_name);

    if(!dev)
        return;

    br = netdev_priv(dev);

    if(!br)
    {
        dev_put(dev);
        return;
    }

    rcu_read_lock();
    prt = br_get_port(br, snoop_entry->port_no);
    rcu_read_unlock();

    for(idx = 0; idx < MCPD_MAX_IFS; idx++)
    {
        if(snoop_entry->wan_info[idx].if_ops)
        {
            from_dev = dev_get_by_name(&init_net, 
                                       snoop_entry->wan_info[idx].if_name);
            if(!from_dev)
               continue;

            if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
                (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
            {
                br_mld_mc_fdb_remove(from_dev,
                                    br, 
                                    prt, 
                                    &snoop_entry->grp, 
                                    &snoop_entry->rep, 
                                    snoop_entry->mode, 
                                    &snoop_entry->src);
            }
            else
            {
                if((snoop_entry->wan_info[idx].if_ops == MCPD_IF_TYPE_BRIDGED) && 
                   (!mcpd_is_br_port(br, from_dev)))
                {
                   dev_put(from_dev);
                   continue;
                }

                br_mld_mc_fdb_add(from_dev,
                                snoop_entry->wan_info[idx].if_ops,
                                br, 
                                prt, 
                                &snoop_entry->grp, 
                                &snoop_entry->rep, 
                                snoop_entry->mode, 
                                snoop_entry->tci, 
                                &snoop_entry->src);
            }
            dev_put(from_dev);
        }
        else
        {
            break;
        }
    }

    /* if LAN-2-LAN snooping enabled make an entry */
    if(br_mld_get_lan2lan_snooping_info())
    {
        if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
            (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
        {
            br_mld_mc_fdb_remove(dev,
                                 br, 
                                 prt, 
                                 &snoop_entry->grp, 
                                 &snoop_entry->rep, 
                                 snoop_entry->mode, 
                                 &snoop_entry->src);
        }
        else
        {
            br_mld_mc_fdb_add(dev,
                              MCPD_IF_TYPE_BRIDGED,
                              br, 
                              prt, 
                              &snoop_entry->grp, 
                              &snoop_entry->rep, 
                              snoop_entry->mode, 
                              snoop_entry->tci, 
                              &snoop_entry->src);
        }
    }

    if(dev)
        dev_put(dev);

    return;
} /* mcpd_nl_process_mld_snoop_entry*/
#endif


// 接口改变函数
static void mcpd_nl_process_if_change(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *ndev = NULL;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    unsigned char *ptr;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);
    ndev = dev_get_by_name(&init_net, ptr);
    if(!ndev)
        return;


    if (ndev->priv_flags & IFF_EBRIDGE)
    {
        br = netdev_priv(ndev);
        if ( NULL == br )
        {
            dev_put(ndev);
            return;
        }

        /* update is for a bridge interface so flsuh all entries */
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
        br_igmp_process_if_change(br, NULL);
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
        br_mld_process_if_change(br, NULL);
#endif
    }
    else
    {
        read_lock(&dev_base_lock);
        for_each_netdev(&init_net, dev)
        {
            if(dev->priv_flags & IFF_EBRIDGE)
            {
                br = netdev_priv(dev);
                if ( NULL == br )
                {
                    continue;
                }
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
                br_igmp_process_if_change(br, ndev);
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
                br_mld_process_if_change(br, ndev);
#endif
            }
        }
        read_unlock(&dev_base_lock);
    }

    dev_put(ndev);

    return;
} /* mcpd_nl_process_igmp_snoop_entry*/

static void mcpd_nl_process_mc_fdb_cleanup(void)
{
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;

    read_lock(&dev_base_lock);
    for(dev = first_net_device(&init_net); 
        dev; 
        dev = next_net_device(dev)) 
    {
        br = netdev_priv(dev);
        if((dev->priv_flags & IFF_EBRIDGE) && (br))
        {
            if(br->igmp_snooping)
                br_igmp_mc_fdb_cleanup(br);

#if (defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP))
            if(br->mld_snooping)
                br_mld_mc_fdb_cleanup(br);
#endif
        }
    }
    read_unlock(&dev_base_lock);
    return;
}

static int mcpd_nl_process_verdict(t_MCPD_VERDICT *verdict)
{
#if 0
    struct mcpd_queue_entry *entry = NULL;
    struct list_head *p;

    list_for_each_prev(p, &mcpd_queue_list) 
    {
        if((__u32)(((struct mcpd_queue_entry *)p)->skb) == verdict->corr_tag)
        {
            entry = (struct mcpd_queue_entry *)p;
            break;
        }
    }

    if(entry)
    {
        write_lock_bh(&mcpd_queue_lock);
        list_del(&entry->list);
        write_unlock_bh(&mcpd_queue_lock);

        if(verdict->code != MCPD_RET_DROP)
        {
            br_flood_forward(entry->br, entry->skb);
        }
        else
        {
            kfree_skb(entry->skb);
        }
        kfree(entry);

        mcpd_queue_total--;
    }
#endif

    return 0;
} /* mcpd_nl_process_verdict */

void mcpd_nl_send_igmp_purge_entry(struct net_bridge_mc_fdb_entry *igmp_entry)
{
    t_MCPD_IGMP_PURGE_ENTRY *purge_entry;
    int buf_size = 0;
    struct sk_buff *new_skb;
    struct nlmsghdr *nlh;
    char *ptr = NULL;
    struct net_bridge_mc_rep_entry *rep = NULL;

    if(!igmp_entry)
        return;

    rep = list_first_entry(&igmp_entry->rep_list, struct net_bridge_mc_rep_entry, list);

    if(!rep)
        return;

    if(!mcpd_pid)
        return;

    buf_size = sizeof(t_MCPD_IGMP_PURGE_ENTRY) + sizeof(t_MCPD_MSG_HDR);
    new_skb = alloc_skb(NLMSG_SPACE(buf_size), GFP_ATOMIC);

    if(!new_skb) 
    {
        return;
    }

    nlh = (struct nlmsghdr *)new_skb->data;
    ptr = NLMSG_DATA(nlh);
    nlh->nlmsg_len = NLMSG_SPACE(buf_size);
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_flags = 0;
    skb_put(new_skb, NLMSG_SPACE(buf_size));
    ((t_MCPD_MSG_HDR *)ptr)->type = MCPD_MSG_IGMP_PURGE_ENTRY;
    ((t_MCPD_MSG_HDR *)ptr)->len = sizeof(t_MCPD_IGMP_PURGE_ENTRY);
    ptr += sizeof(t_MCPD_MSG_HDR);

    purge_entry = (t_MCPD_IGMP_PURGE_ENTRY *)ptr;

    purge_entry->grp.s_addr = igmp_entry->grp.s_addr;
    purge_entry->src.s_addr = igmp_entry->src_entry.src.s_addr;
    purge_entry->rep.s_addr = rep->rep.s_addr;

    purge_entry->pkt_info.br_name[0] = '\0';
    memcpy(purge_entry->pkt_info.port_name, igmp_entry->dst->dev->name, IFNAMSIZ);
    purge_entry->pkt_info.port_no = igmp_entry->dst->port_no;
    purge_entry->pkt_info.if_index = 0;
    purge_entry->pkt_info.data_len = 0;
    purge_entry->pkt_info.corr_tag = 0;

#if defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE)
    purge_entry->pkt_info.tci = igmp_entry->lan_tci;
#endif /*  defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE) */

    NETLINK_CB(new_skb).dst_group = 0;
    NETLINK_CB(new_skb).pid = mcpd_pid;
    mcpd_dump_buf((char *)nlh, 128);

    netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);

    return;
} /* mcpd_nl_send_purge_entry */

static inline void mcpd_nl_rcv_skb(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    unsigned short msg_type;

    if (skb->len >= NLMSG_SPACE(0)) 
    {
        if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
            return;

        ptr = NLMSG_DATA(nlh);

        msg_type = *(unsigned short *)ptr;
        switch(msg_type)
        {
            // 这个消息用来注册
            case MCPD_MSG_REGISTER:
                mcpd_nl_process_registration(skb);
                break;

            // 比如当收到一个对应的IGMP的report报文的时候调用本函数
            // 这个是最关键的函数
            case MCPD_MSG_IGMP_SNOOP_ENTRY:
                mcpd_nl_process_igmp_snoop_entry(skb);
                break;

            // 这个消息只是用来控制是否使能LAN2LAN的snooping功能    
            case MCPD_MSG_IGMP_LAN2LAN_SNOOP:
                mcpd_nl_process_lan2lan_snooping(skb);
                break;

            // 暂时无用
            case MCPD_MSG_VERDICT:
                mcpd_nl_process_verdict((t_MCPD_VERDICT *)
                                            (ptr + sizeof(t_MCPD_MSG_HDR)));
                break;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
            case MCPD_MSG_MLD_SNOOP_ENTRY:
                mcpd_nl_process_mld_snoop_entry(skb);
                break;

            case MCPD_MSG_MLD_LAN2LAN_SNOOP:
                mcpd_nl_process_mld_lan2lan_snooping(skb);
                break;
#endif

            // 这个也不需要太关系，当br改变时才使用
            case MCPD_MSG_IF_CHANGE:
                mcpd_nl_process_if_change(skb);
                break;

            case MCPD_MSG_MC_FDB_CLEANUP:
                mcpd_nl_process_mc_fdb_cleanup();
                break;

            default:
                printk("MCPD Unknown usr->krnl msg type \n");
        }
    }

    return;
} /* mcpd_nl_rcv_skb */

#if 0
static void mcpd_nl_data_ready(struct sock *sk, int len)
{
    struct sk_buff *skb = NULL;
    unsigned int qlen = skb_queue_len(&sk->sk_receive_queue);

    while (qlen-- && (skb = skb_dequeue(&sk->sk_receive_queue))) 
    {
        mcpd_nl_rcv_skb(skb);
        kfree_skb(skb);
    }
} /* mcpd_nl_data_ready */
#endif

// TODO: 需要移到netlink.h中

#define NETLINK_MCPD            30       /* for multicast */

static int __init mcpd_module_init(void)
{
    printk(KERN_INFO "Initializing MCPD Module\n");

    nl_sk = netlink_kernel_create(&init_net, NETLINK_MCPD, 0, 
                                mcpd_nl_rcv_skb, NULL, THIS_MODULE);

    if(nl_sk == NULL) 
    {
        printk("MCPD: failure to create kernel netlink socket\n");
        return -ENOMEM;
    }

    return 0;
} /* mcpd_module_init */

static void __exit mcpd_module_exit(void)
{
    sock_release(nl_sk->sk_socket); 
    printk(KERN_INFO "Removed MCPD\n");
} /* mcpd_module_exit */

module_init(mcpd_module_init);
module_exit(mcpd_module_exit);
