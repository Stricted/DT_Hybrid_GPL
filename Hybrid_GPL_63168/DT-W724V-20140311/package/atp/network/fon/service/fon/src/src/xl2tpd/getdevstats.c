/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file getdevstats.c
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author from dailinke
  *\date  2012-7-12
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if_link.h>
#include "atputil.h"

#ifdef SUPPORT_MSTATS_DEBUG
#define trace(fmt, argc...) printf(fmt, ##argc)
#else
#define trace(fmt, argc...)
#endif

#define MAX_NETLINK_STATS_BUFF      (1024)

typedef int (*content_match)(unsigned short key, unsigned short in);
typedef int (*copy_data)(void *dst, const void *src, unsigned long len);

struct netlink_stats_info
{
    int ioctlsocketfd;
    int netlinksocketfd;
    int initflag;
    struct sockaddr_nl myaddr;
    unsigned int seq;
    unsigned char acBuf[MAX_NETLINK_STATS_BUFF];
};

static struct netlink_stats_info g_stStatsInfo;
#ifdef NETLINK_STATS_SELF_TEST
static void netlink_stats_packetdump(unsigned char *pszContent, int packetlen);
#endif
static int netlink_stats_init(void);
#ifdef NETLINK_STATS_SELF_TEST
static void netlink_stats_uninit(void);
#endif
static int netlink_stats_getIfIdx(const char *pszDev);
static int netlink_stats_send(const char *pszDev);
static int netlink_stats_recv(void);
static int netlink_stats_parse_attr(struct rtattr *pastAttr[], struct nlmsghdr *hdr, int len);
static int netlink_stats_parse(content_match pfnMatch, unsigned short key, 
    copy_data pfnCopyData, void *pvOutput, unsigned int outputlen, unsigned int packetlen);

static int content_match_fn(unsigned short key, unsigned short in)
{
    return key == in;
}

static int copy_data_fn(void *dst, const void *src, unsigned long len)
{
    memcpy(dst, src, len);
    return 0;
}

// 返回0表示初始化成功或者已经初始化过了
static int netlink_stats_init(void)
{
    int ret = 0;
    int flag = 0;
    
    struct sockaddr_nl *pstMyAddr = NULL;
    if (g_stStatsInfo.initflag)
    {
        return 0;
    }
    g_stStatsInfo.netlinksocketfd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (-1 == g_stStatsInfo.netlinksocketfd)
    {
        printf("error: netlink create for statistic error: [%s]\r\n", strerror(errno));
        return (-1);
    }
    
    // 设置非阻塞
    flag = fcntl(g_stStatsInfo.netlinksocketfd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(g_stStatsInfo.netlinksocketfd, F_SETFL, flag);

    pstMyAddr = &g_stStatsInfo.myaddr;
    memset(pstMyAddr, 0, sizeof(struct sockaddr_nl));
    pstMyAddr->nl_family = AF_NETLINK;
    pstMyAddr->nl_pid = getpid() | 0x98765421;
    ret = bind(g_stStatsInfo.netlinksocketfd, (struct sockaddr *)pstMyAddr, sizeof(struct sockaddr_nl));
    if (ret)
    {
        printf("error: netlink bind for statistic error: [%s]\r\n", strerror(errno));
        close(g_stStatsInfo.netlinksocketfd);
        g_stStatsInfo.netlinksocketfd = -1;
        return (-1);
    }

    g_stStatsInfo.ioctlsocketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == g_stStatsInfo.ioctlsocketfd)
    {
        printf("create ioctl for index error: [%s]\r\n", strerror(errno));
        close(g_stStatsInfo.netlinksocketfd);
        g_stStatsInfo.netlinksocketfd = -1;
        return (-1);
    }
    
    g_stStatsInfo.initflag = 1;

    trace("init pid: %d success\r\n", pstMyAddr->nl_pid);

    return 0;
}
#ifdef NETLINK_STATS_SELF_TEST
//lint --e{528}
static void netlink_stats_uninit(void)
{
    if (0 <= g_stStatsInfo.ioctlsocketfd)
    {
        close(g_stStatsInfo.ioctlsocketfd);
    }
    if (0 <= g_stStatsInfo.netlinksocketfd)
    {
        close(g_stStatsInfo.netlinksocketfd);
    }
    g_stStatsInfo.ioctlsocketfd = -1;
    g_stStatsInfo.ioctlsocketfd = -1;
    g_stStatsInfo.initflag = 0;
    return ;
}
#endif
static int netlink_stats_getIfIdx(const char *pszDev)
{
    struct ifreq req;
    
    if (netlink_stats_init())
    {
        printf("error for netlink init\r\n");
        return (-1);
    }
    
    memset(&req, 0, sizeof(req));
    strcpy(req.ifr_name, pszDev);
    
    if (ioctl(g_stStatsInfo.ioctlsocketfd, SIOCGIFINDEX, &req))
    {
        printf("error: ioctl for get index error: [%s]\r\n", strerror(errno));
        return (-1);
    }
    
    trace("get index for dev: %s %d\r\n", pszDev, req.ifr_ifindex);
    
    return req.ifr_ifindex;
}

// 给定一个接口，获取该接口的统计信息
static int netlink_stats_send(const char *pszDev)
{
    int ifindex = -1;
    unsigned char aucBuf[NLMSG_ALIGN(sizeof(struct nlmsghdr)) + NLMSG_ALIGN(sizeof(struct ifinfomsg))];
    struct nlmsghdr *pstNLHdr;    
    struct ifinfomsg *pstIfCfg;
    struct sockaddr_nl dest;
    int ret = 0;

    ifindex = netlink_stats_getIfIdx(pszDev);
    if (0 >= ifindex)
    {
        printf("error to get index for dev: %s\r\n", pszDev);
        return (-1);
    }

    trace("begin to init message\r\n");

    memset(aucBuf, 0, sizeof(aucBuf));
    pstNLHdr = (struct nlmsghdr *)aucBuf;
    pstNLHdr->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    pstNLHdr->nlmsg_seq = ++g_stStatsInfo.seq;
    pstNLHdr->nlmsg_flags = NLM_F_REQUEST;
    pstNLHdr->nlmsg_type = RTM_GETLINK;
    pstNLHdr->nlmsg_pid = g_stStatsInfo.myaddr.nl_pid;
    
    pstIfCfg = NLMSG_DATA(pstNLHdr);
    pstIfCfg->ifi_family = AF_UNSPEC;
    pstIfCfg->ifi_index = ifindex;

    memset(&dest, 0, sizeof(dest));
    dest.nl_family = AF_NETLINK;

    trace("begin to send message\r\n");

    ret = sendto(g_stStatsInfo.netlinksocketfd, (void *)aucBuf, sizeof(aucBuf), 
        0, (struct sockaddr *)&dest, sizeof(dest));

#ifdef NETLINK_STATS_SELF_TEST
    netlink_stats_packetdump(aucBuf, sizeof(aucBuf));
#endif
    trace("sendto return %d %s\r\n", ret, ret <= 0? strerror(errno):"no error");

    return ret;
}

static int netlink_stats_recv(void)
{
    int len = 0;
    struct msghdr msg;
    struct sockaddr_nl from;
    struct iovec iov;
    
    memset(g_stStatsInfo.acBuf, 0, sizeof(g_stStatsInfo.acBuf));
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&from;
    msg.msg_namelen = sizeof(from);
    iov.iov_base = g_stStatsInfo.acBuf;
    iov.iov_len = MAX_NETLINK_STATS_BUFF;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    trace("begin to receive message\r\n");
    
    while (1)
    {
        len = recvmsg(g_stStatsInfo.netlinksocketfd, &msg, MSG_DONTWAIT);
        if ((len < 0) && ((EINTR == errno) || (EAGAIN == errno)))
        {
            continue;
        }
        break;
    }
    // 按照正常处理逻辑，此处应该是如果发现被截断了，那么就应该重新申请内存并重新接收报文，但是这里考虑到本函数一次只是
    // 获取一个，因此空间足够了
    if ((len <= 0) || (msg.msg_flags & (MSG_ERRQUEUE | MSG_TRUNC)))
    {
        printf("error: message receive error or wrong from kernel[%s], please check\r\n", strerror(errno));
        return (-1);
    }

    trace("receive packet len: %d\r\n", len);

    return len;    
}

//debug use
#ifdef NETLINK_STATS_SELF_TEST
static void netlink_stats_packetdump(unsigned char *pszContent, int packetlen)
{    
    int i;
    for (i = 0; i < packetlen; i++)
    {
        if (i && !(i % 15))
        {
            printf("\r\n");
        }
        printf("%02x ", pszContent[i]);
    }
    printf("\r\n");
}
#endif

// 第三个参数表示数据长度(不包括属性长度)
static int netlink_stats_parse_attr(struct rtattr *pastAttr[], struct nlmsghdr *hdr, int len)
{
    int rtattr_len = 0;
    struct rtattr *pstRtattr = NULL;

    rtattr_len = hdr->nlmsg_len - len;
    pstRtattr = (struct rtattr *)((char *)hdr + len);
    for (; RTA_OK(pstRtattr, rtattr_len); pstRtattr = RTA_NEXT(pstRtattr, rtattr_len))
    {
        if (NULL == pastAttr[pstRtattr->rta_type])
        {
            trace("type %d match\r\n", pstRtattr->rta_type);
            pastAttr[pstRtattr->rta_type] = pstRtattr;
        }
    }
    return 0;
}

static int netlink_stats_parse(content_match pfnMatch, unsigned short key, 
    copy_data pfnCopyData, void *pvOutput, unsigned int outputlen, unsigned int packetlen)
{
    int i = 0;
    int flag = 0;
    pid_t pid;
    struct nlmsghdr *pstNLHdr;
    struct rtattr *pstRTA[IFLA_MAX] = {NULL};
    struct nlmsgerr *err;
    
    pid = g_stStatsInfo.myaddr.nl_pid;
    
    pstNLHdr = (struct nlmsghdr *)g_stStatsInfo.acBuf;
//lint --e{574}
    for (; NLMSG_OK(pstNLHdr, packetlen); pstNLHdr = NLMSG_NEXT(pstNLHdr, packetlen))
    {
        if (pstNLHdr->nlmsg_pid != pid)
        {
            trace("pid not match [%d != %d]\r\n", pstNLHdr->nlmsg_pid, pid);
            continue;
        }
        if (NLMSG_ERROR == pstNLHdr->nlmsg_type)
        {
            err = (struct nlmsgerr *)NLMSG_DATA(pstNLHdr);
            if (NLMSG_LENGTH(sizeof(struct nlmsgerr)) > pstNLHdr->nlmsg_len)
            {
                printf("error message was be truncted\r\n");
            }
            else
            {
                errno = -err->error;
                printf("error happend: [%s]\r\n", strerror(errno));
            }
            return (-1);
        }
        
        if (RTM_NEWLINK != pstNLHdr->nlmsg_type)
        {
            trace("message type no match[%02x != %02x]\r\n", pstNLHdr->nlmsg_type, RTM_NEWLINK);
            continue;
        }

        if (pstNLHdr->nlmsg_flags & MSG_TRUNC)
        {
            printf("kernel return too long message to save\r\n");
            return (-1);
        }
        
        memset(pstRTA, 0, sizeof(pstRTA));
        netlink_stats_parse_attr(pstRTA, pstNLHdr, NLMSG_LENGTH(sizeof(struct ifinfomsg)));
        for (i = 0; i < IFLA_MAX; i++)
        {
            if (!pstRTA[i])
            {
                continue;
            }
            if (pfnMatch && pfnMatch(key, pstRTA[i]->rta_type))
            {
//lint -e{522}
                pfnCopyData(pvOutput, RTA_DATA(pstRTA[i]), outputlen);
                flag = 1;
                break;
            }
        }
        
        if (flag)
        {
            break;
        }
    }
#ifdef NETLINK_STATS_SELF_TEST
    netlink_stats_packetdump(g_stStatsInfo.acBuf, packetlen);
#endif
    return 0;
}

void ATP_UTIL_get_dev_stats(const char *pszDev, struct rtnl_link_stats *pstStats)
{
    int len = 0;
    struct rtnl_link_stats stats = {0};
    
    if (!pszDev || ('\0' == pszDev[0]) || (!pstStats))
    {
        return;
    }
    netlink_stats_init();
    if (0 > netlink_stats_send(pszDev))
    {
        trace("get %s multicast error: send packet error\r\n", pszDev);
        return;
    }
    len = netlink_stats_recv();
    if (len <= 0)
    {
        trace("can't receive packet form kernel\r\n");
        return;
    }
    netlink_stats_parse(content_match_fn, IFLA_STATS, copy_data_fn, &stats, sizeof(stats), len);
    memcpy(pstStats, &stats, sizeof(stats));
}

#ifdef NETLINK_STATS_SELF_TEST
void main(int argc, char **argv)
{
    int count = 0;
    int i;
    struct rtnl_link_stats stats;
    
    for (i = 1; i < argc; i++)
    {
        count = netlink_getmulticastPakcetStats(argv[i]);
        printf("multicast: %lu\r\n", count);
    }

    return ;
}
#endif

