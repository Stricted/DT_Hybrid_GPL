
#ifndef _LINUX_IGMP_STATISTIC_H
#define _LINUX_IGMP_STATISTIC_H

/* BEGIN: Added by z67728, 2010/7/7 For BT statistic */

#define IGMP_SESSION_MAX_NUM 16

enum tag_MULTI_SEND_TYPE_E
{
    MULTI_SEND_TYPE_LOCAL      = 0,
    MULTI_SEND_TYPE_FORWARD    = 1,
    MULTI_SEND_TYPE_BUFF
};

enum tag_MULTI_TYPE_E
{
    IGMP_JOIN_GROUP_PASKET      = 0, /* ��������Layer3�����IGMP���뱨�� */
    IGMP_LEAVE_GROUP_PASKET     = 1, /* ��������Layer3�����IGMP�뿪���� */
    IGMP_UP_REQUEST_PACKET      = 2, /* ͳ�ƿ�����������յ����鲥������ */
    IGMP_DOWN_RETRANSMIT_PACKET = 3, /* ͳ�ƿ�����������յ����鲥������ */
    IGMP_MULTI_GROUP            = 4, /* ��ǰ�鲥�Ự��Ŀ                 */
    IGMP_MULTI_DATA_PACKET      = 5, /* ������Ľ��յ����鲥��           */
    IGMP_BUFF
};

struct tag_igmp_port_statistic
{
    unsigned long ulIndex;          /* ʵ��ID */
    unsigned long ulPortNum;
    unsigned long ulStatistic;
    struct tag_igmp_port_statistic *next;
};

struct tag_igmp_multicast_union
{
    unsigned char ucMultiMacAddr[IGMP_SESSION_MAX_NUM][3];
    struct tag_igmp_multicast_union *next;
};

struct tag_igmp_statistics_st
{
    unsigned long ulOwnJoinPacketsNum;
    unsigned long ulOwnLeavePacketsNum;
    unsigned long ulUpRequestPacketsNum;
    unsigned long ulDownRetransmitPacketsNum;
    unsigned long ulMultiGroupsNum;
    unsigned long ulMultiDataPacketsNum;
    struct tag_igmp_port_statistic  *pstDownstream;
    struct tag_igmp_port_statistic  *pstUpstream;
    struct tag_igmp_multicast_union *pstMultiSession;
    struct tag_igmp_statistics_st *next;
};

struct tag_igmp_statistics_interface_group_record_st
{
    unsigned int address;
    unsigned int stats;
};

struct tag_igmp_statistics_interface_st
{
    char                                                 czInterfaceName[IFNAMSIZ];            /* �ӿ���           */
    struct tag_igmp_statistics_st                        stIgmpStatistics;                     /* �ӿ�IGMPͳ����Ϣ */
    struct tag_igmp_statistics_interface_group_record_st stIgmpGroup[IGMP_SESSION_MAX_NUM];    /* �鲥���¼����   */
    struct tag_igmp_statistics_interface_st             *next;
};

extern void br_igmp_statistic_clearallchain(void);
extern int  br_igmp_statistic_inc(struct sk_buff *skb,int type);
extern void br_igmp_statistic_print();
extern void br_igmp_statistic_setenable(int enable);
extern int  ppp_igmp_statistic_inc(struct sk_buff *skb);
extern int  ppp_ip_statistic_inc(struct sk_buff *skb,int type);
extern struct tag_igmp_statistics_interface_st * br_igmp_statistic_findnode(const char *pcName);
extern struct tag_igmp_statistics_interface_st * br_igmp_statistic_getchainheader(void);
extern void br_igmp_statistic_setinterfaceport(char *pcinterface, unsigned long ulacction, unsigned long ulindex, unsigned long ulportattr, unsigned long ulport);
/* END:   Added by z67728, 2010/7/7 */
#endif

