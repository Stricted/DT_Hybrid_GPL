
#ifndef __BR_IGMP_SNOOPING_H
#define __BR_IGMP_SNOOPING_H

#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/if_bridge.h>

#define IGMP_TIMER_CHECK_TIMEOUT 10
#define IGMP_QUERY_TIMEOUT 150

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif

#define BR_IGMP_SNOOPING_DISABLE 0
#define BR_IGMP_SNOOPING_ENABLE 1

typedef union ipv4
{
    unsigned int ulIpAddr;
    unsigned char acIpAddr[4];
}igmp_ipaddr;

struct net_bridge_igmp_snooping_entry
{
    struct list_head 		  list;
	struct net_bridge_port	  *dev_dst;             /*Ŀ���Ŷ˿ڶ�Ӧ�ĵ�ַ����eth0.**/
	unsigned char			  grp_mac[ETH_ALEN];  /*�鲥��ַ��Ӧ��MAC��ַ����01:00:5e:xx:xx:xx*/
	unsigned char			  host_mac[ETH_ALEN];   /*PC����MAC��ַ*/
	unsigned long			  time;                  /*��ʱ��*/
};

struct net_bridge_igmp_snooping
{
	struct list_head		igmp_list;         /*����ͷ*/
	struct timer_list 		igmp_timer;        /*��ʱ��*/
	int		                igmp_snooping_enable;         /*0:��ʾIGMP Snooping����*/
	spinlock_t		        igmp_lock;         /*������*/
	int		 	            igmp_start_timer; /*��ʱ���Ƿ����ı�־*/
};

struct net_bridge_igmp_robust_timerlist
{
    struct timer_list timer;      /*��ʱ��          */
    unsigned int      interval;          
    unsigned int      num;               
    unsigned int      use;        /*��ʱ���Ƿ�ʹ��*/
    void             *skb;        /*����            */
    void             *br;         /*���豸          */
};

/* BEGIN: Added by z67728, 2010/7/12 FOR BT IGMP : ��snooping��¼��������Ϣ��,
   �������͵�IGMPv2���汨�Ľ�����ͨ���Ŷ˿�,�������ᵼ���鲥����ʱ�ж�.���յ�
   �����Ž�wan�Ĳ�ѯ���ĺ�,snooping�����Ӧ���IGMPv2���汨��ͨ��.*/
struct net_bridge_igmp_query_group
{
    struct net_bridge_igmp_query_group *next;
    unsigned char			            grp_mac[ETH_ALEN];    /*�鲥��ַ��Ӧ��MAC��ַ����01:00:5e:xx:xx:xx*/
    unsigned char			            interface[16];        /*�յ�query�Ľӿ�                           */
};

struct net_bridge_igmp_query_timer
{
    struct timer_list 		igmp_timer;        /* ��ʱ��                 */
    int		 	            igmp_start_timer;  /* ��ʱ���Ƿ����ı�־   */
    int                     igmp_query_enable; /* �յ���ѯ�������ñ�־λ */
};

struct net_bridge_igmp_query_chain
{
    struct net_bridge_igmp_query_group *queruchain;
    struct net_bridge_igmp_query_timer  querytimer;
    spinlock_t		                    igmp_lock;         /*������*/
};
void br_igmp_snooping_query_report_timeout(unsigned long ptr);
void br_igmp_snooping_free_query_chain(void);
int br_igmp_snooping_del_query_node(unsigned char * dest);
struct net_bridge_igmp_query_group *br_igmp_snooping_find_query_node(unsigned char * dest,unsigned char *interface);
int br_igmp_snooping_add_query_node(unsigned char * dest,unsigned char *interface);
/* END:   Added by z67728, 2010/7/12 */
extern  int br_igmp_snooping_del_by_nbp(struct net_bridge_port *dev_port);
extern  void  br_igmp_snooping_clear(void);
extern  int   br_igmp_snooping_forward(struct sk_buff *skb, struct net_bridge *br,unsigned char *dest,int forward);
extern  void  br_igmp_snooping_set_enable(int enable);
extern  void  br_igmp_snooping_show(void);
extern  void  br_igmp_snooping_init(void);
#if defined(CONFIG_BR_IGMP_STATISTIC)
int br_igmp_snooping_find_intrface_group_num(const char*pcInterface);
#endif
#endif
