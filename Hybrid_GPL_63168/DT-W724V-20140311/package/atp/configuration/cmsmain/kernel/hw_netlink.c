#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <net/sock.h>
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/string.h>


#include "msg/kcmsmonitormsgtypes.h"
//MODULE_LICENSE("GPL");
//MODULE_DESCRIPTION("Netlink module for kernel<->userspace messge.");
void (*hw_ctclean_input)(struct sk_buff *skb) = NULL;


void eth_link_change_hooker(const char *pszChangeEthName,int portNum,int result)
{
    char *ptrEthName = NULL;
    netlink_common_msg *pstMsg = NULL;
    
    if(NULL == pszChangeEthName || 0 == strlen(pszChangeEthName))
    {
        printk("\nInvalid para,null pointer.\n");

        return;
    }

    ptrEthName = (char *)kmalloc((sizeof(netlink_common_msg)), GFP_KERNEL);
    if (NULL == ptrEthName)
    {        
        printk("\nKmalloc mem error.\n");
        
        return;
    }

    pstMsg = (netlink_common_msg *)ptrEthName;
    pstMsg->eventType = ATP_MSG_MONITOR_EVT_LAN_ETH;
    pstMsg->eventResult = result;
    pstMsg->eventPortNum = portNum;    
    snprintf(pstMsg->acPortName,NETLINK_MSG_CMD_LEN,"%s",pszChangeEthName);

    syswatch_nl_send(ATP_MSG_MONITOR_EVT_LAN_ETH, (unsigned char *)pstMsg,sizeof(netlink_common_msg));

    kfree((void*)ptrEthName);

    return;
}

EXPORT_SYMBOL(eth_link_change_hooker);

static u32 us_pid = -1;

static int syswatch_nl_inited = 0;
struct sock *syswatch_nl_sock = NULL;
static DEFINE_MUTEX(hw_netlink_mutex); /* serialization of message processing */

/*-------------------------------------------------
函数原型： static int syswatch_nl_send(char *name, char comm);
描    述：		通过netlink发送消息到userspace 
输    入： 		name  发送的消息内容
                       		comm 发送的消息类型参考syswatch_msg_comm
输    出：  	无
返 回 值:		 正确时返回0
                       		错误时返回<0
--------------------------------------------------*/
int syswatch_nl_send(unsigned int type, unsigned char *buf, unsigned int len)
{
    struct sk_buff *skb;
    struct generic_nl_msg *body_msg;
    struct nlmsghdr *nlh;
    static int seq = 0;
    int size;

    if (-1 == us_pid)
    {
        printk(KERN_ALERT "No userspace pid joined yet\n");
        return -EBADF;
    }

    if (len > 1024)
    {
        len = 1024;
    }
    if ((NULL == buf) || (0 == len))
    {
        size = NLMSG_SPACE(sizeof(*body_msg));
    }
    else 
    {
        size = NLMSG_SPACE(sizeof(*body_msg) + len);
    }

    skb = alloc_skb(size, GFP_ATOMIC);
    if (!skb)
    {
        //printk(KERN_ALERT "OOM when syswatch_nl_send");
        return -ENOMEM;
    }

    nlh = NLMSG_PUT(skb, KERNEL_PID, seq++, NLMSG_DONE, size - sizeof(*nlh));
    body_msg = (struct generic_nl_msg *)NLMSG_DATA(nlh);

    memset(body_msg, 0, size - sizeof(*nlh)); 
    body_msg->comm = type;

    if ((NULL != buf) && (len > 0))
    {
        memcpy(body_msg->data, buf, len);
        body_msg->len = len;
    }
    else 
    {
        body_msg->len = 0;
    }

    NETLINK_CB(skb).dst_group = SYSWATCH_USERSPACE_GROUP;
nlmsg_failure:
    //return netlink_broadcast(syswatch_nl_sock, skb, 0, SYSWATCH_USERSPACE_GROUP, GFP_ATOMIC);
    return netlink_unicast(syswatch_nl_sock, skb, us_pid, 1);
    
}


int syswatch_sendLog(unsigned int logType, unsigned int logLevel, unsigned int logNum, unsigned char *str)
{
    netlink_log_header  stHeader;
    netlink_log_header *pstHeader;
    unsigned char *pucBuf;
    unsigned int len;
    int ret;

    if (NULL == str)
    {
        stHeader.logLevel = logType;
        stHeader.logLevel = logLevel;
        stHeader.logLevel = logNum;
        return syswatch_nl_send(ATP_MSG_MONITOR_EVT_LOG, (unsigned char *)(&stHeader), sizeof(netlink_log_header));
    }

    len = (unsigned int)(strlen(str) + 1);

    pucBuf = (unsigned char *)kmalloc((len + sizeof(netlink_log_header)), GFP_KERNEL);
    if (NULL == pucBuf)
    {
        return -1;
    }

    pstHeader = (netlink_log_header *)pucBuf;
    pstHeader->logType = logType;
    pstHeader->logLevel = logLevel;
    pstHeader->logNum = logNum;
    memcpy((void *)(pucBuf + sizeof(netlink_log_header)), str, len);
    len += sizeof(netlink_log_header);

    ret = syswatch_nl_send(ATP_MSG_MONITOR_EVT_LOG, pucBuf, len);
    kfree((void *)pucBuf);
    return ret;
}

EXPORT_SYMBOL(syswatch_sendLog);
EXPORT_SYMBOL(syswatch_nl_send);


/*-------------------------------------------------
函数原型： static void syswatch_nl_input(struct sk_buff *__skb)；
描    述： 		此函数处理userspace下发的消息
输    入： 		封装了userspace消息体的sk_buff
输    出： 		无
返 回 值： 	无
--------------------------------------------------*/
static void syswatch_nl_input(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    int    err;

    if(NULL == skb)
    {
        printk("\n Recv empty netlink data.\n");
        return;
    }
    
    nlh = (struct nlmsghdr *)skb->data;
    printk("received netlink message payload from pid %d:%d:%d:%d\n", nlh->nlmsg_pid, skb->data_len, nlh->nlmsg_len, nlh->nlmsg_type);

    if (ATP_KCMSMAIN_NETLINK_NAT == nlh->nlmsg_type)
    {
        if (hw_ctclean_input)
        {
            hw_ctclean_input(skb);
        }
        return;
    }
    us_pid = nlh->nlmsg_pid;
}

static void syswatch_nl_input_sk(struct sock *sk, int len)
{
    struct sk_buff *skb;
    unsigned int qlen = skb_queue_len(&sk->sk_receive_queue);

    while (qlen-- && (skb = skb_dequeue(&sk->sk_receive_queue))) {
        syswatch_nl_input(skb);
        kfree_skb(skb);
    }
}

/*-------------------------------------------------
函数原型： static int syswatch_nl_init(void);
描    述： 		初始化内核syswatch netlink socket
输    入： 		无
输    出： 		无
返 回 值： 	创建成功返回0
				否则返回< 0
--------------------------------------------------*/
static int __devinit syswatch_nl_init(void)
{
#ifdef CONFIG_ATP_CT_CLEAN
    /*register ct clean hook*/
    hw_ctclean_input = hw_ctclean_process;
    printk("set hook ct_clean_input to hw_ctclean_process\n");
#endif
#ifdef CONFIG_MTK_CHIP
	syswatch_nl_sock = netlink_kernel_create(NETLINK_SYSWATCH, 0,
					  syswatch_nl_input_sk, THIS_MODULE);
#else
    syswatch_nl_sock = netlink_kernel_create(&init_net, NETLINK_SYSWATCH, 0,
					  syswatch_nl_input, NULL, THIS_MODULE);
#endif
    if (!syswatch_nl_sock)
    {
        printk(KERN_ALERT "kernel create netlink failed.\n");
        return -EIO;
    }
    syswatch_nl_inited = 1;

    return 0;
}

static void __devexit syswatch_nl_fini(void)
{
    syswatch_nl_inited = 0;
    us_pid = -1;
    //netlink_kernel_release(syswatch_nl_sock);
}

subsys_initcall(syswatch_nl_init);
module_exit(syswatch_nl_fini);

