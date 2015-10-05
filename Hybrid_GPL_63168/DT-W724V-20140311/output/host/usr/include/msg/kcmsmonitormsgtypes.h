#ifndef __ATP_KCMSMAIN_MSGTYPES_H__
#define __ATP_KCMSMAIN_MSGTYPES_H__

enum ATP_KCMSMAIN_MSGTYPES_EN
{
    ATP_MSG_KCMSMAIN_START = (165 << 20),
    ATP_MSG_MONITOR_EVT_RESET,      /*恢复出厂设置按键*/
    ATP_MSG_MONITOR_EVT_REBOOT,      /*重启网关按键*/    
    ATP_MSG_MONITOR_EVT_DSL_UP,
    ATP_MSG_MONITOR_EVT_DSL_DOWN,
    ATP_MSG_MONITOR_EVT_DSL_TRAINING,
    ATP_MSG_MONITOR_EVT_DSL_INIT,
    ATP_MSG_MONITOR_EVT_LAN_ETH,
    ATP_MSG_MONITOR_EVT_DATACARD_START,       
    ATP_MSG_MONITOR_EVT_DATACARD_STOP, 
    ATP_MSG_MONITOR_EVT_WLAN,          /*WLAN开关*/
    ATP_MSG_MONITOR_EVT_WPS,           /*WPS按键*/
    ATP_MSG_MONITOR_EVT_WLAN_5G,       /*WLAN 5G开关*/
    ATP_MSG_MONITOR_EVT_WPS_5G,        /*WPS 5G按键*/
    ATP_MSG_MONITOR_EVT_DECT_PP,    /*DECT对码按键*/
    ATP_MSG_MONITOR_EVT_DECT_SEARCH,    /*DECT查找按键*/
    ATP_MSG_MONITOR_EVT_LOG,
    ATP_MSG_MONITOR_EVT_IPTV_START,    
    ATP_MSG_MONITOR_EVT_IPTV_STOP,  
    ATP_MSG_MONITOR_EVT_RTSP_START,    
    ATP_MSG_MONITOR_EVT_RTSP_STOP,         
    ATP_MSG_MONITOR_EVT_IPP_START,       
    ATP_MSG_MONITOR_EVT_IPP_STOP,   
    ATP_MSG_MONITOR_EVT_DYING_GASP,  /*Dying Gasp紧急事件*/ 	
    ATP_MSG_MONITOR_EVT_USBSTORAGE_PLUGIN,
    ATP_MSG_MONITOR_EVT_USBSTORAGE_PLUGOUT,
    ATP_MSG_MONITOR_EVT_WLANRF,      /*无线参数读写事件*/
    ATP_MONITOR_EVT_SERVLED_STOP,    /*Service LED stop*/
	ATP_MSG_MONITOR_EVT_WLAN_BAND_CHG,	   /*WLAN 2.4 G/5G 切换开关*/
	ATP_MSG_MONITOR_EVT_RCV_PADT,
};

/*内核和用户态通信的netlink协议类型*/
#define NETLINK_SYSWATCH                20
#define NETLINK_ATP_CONSOLE             21 /* send console printk to userspace */

#define SYSWATCH_USERSPACE_GROUP    31
#define KERNEL_PID      0
#define NETLINK_MSG_CMD_LEN         64

#define MAX_MSGSIZE 1024
#define MAX_RULE_NUM 15800

#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
#define ESTABLISHED 3

#define ATP_KCMSMAIN_NETLINK_NAT 0xFF01

#define NAT_NETLINK_PID (getpid() + ATP_KCMSMAIN_NETLINK_NAT)

struct in6_addr_ex
{
	unsigned int u6_addr32[4];
};

/*Start of Protocol 2014-4-17 16:30 for Filterilst的及时生效 by t00189036*/
typedef enum 
{
    CT_ALL,/*所有的连接跟踪，为兼容原来的清连接跟踪，将CT_ALL记为0*/        
    CT_NONE,/*不清连接跟踪*/
    CT_BYPASS,/*走dsl bypass的连接跟踪*/
    CT_BONDING,/*走bonding的连接跟踪*/     
} CT_TYPE;
/*End of Protocol 2014-4-17 16:30 for by t00189036*/


typedef struct tagATP_CT_RULE_INFO
{
    unsigned int    ulSrcIp;
    unsigned int    ulSrcMask;    
    unsigned int    ulDstIp;
    unsigned int    ulDstMask;
    unsigned short  usSrcPort;
    unsigned short  usDstPort;
    unsigned char   ucProtocol;
    unsigned char   ucDscp;
    unsigned short  usRes;/*保留项，使结构体四字节对其*/
    struct in6_addr_ex	stSrcIp6;
    struct in6_addr_ex	stSrcMask6;
    struct in6_addr_ex	stDstIp6;
    struct in6_addr_ex	stDstMask6;
    /*Start of Protocol 2014-4-17 16:30 for Filterilst的及时生效 by t00189036*/
    CT_TYPE enCtType;/*清除连接跟踪的类型*/
    /*End of Protocol 2014-4-17 16:30 for by t00189036*/
} ATP_CT_RULE_INFO;

// this is the msg struct that being sent between the kernel and userspace
struct  generic_nl_msg
{
    unsigned int len; 		// the length of the "data" field
    unsigned int comm;  	// which kind of msg, referring to 'syswatch_msg_comm'
    unsigned char data[0]; 	// if the 'comm' needs to add more payload, put it here
};

typedef struct 
{
    unsigned int logType;
    unsigned int logLevel;
    unsigned int logNum;
}netlink_log_header;

/*内核通知用户态具体事件信息结构体*/
typedef struct netlink_common_msg_st
{
    unsigned int eventType;
    unsigned int eventResult;
    unsigned int eventPortNum;
    char         acPortName[NETLINK_MSG_CMD_LEN];
}netlink_common_msg;

// Global variable to indicate whether the userspace netlink socket has created,
// and send the monitored processes' names to the kernel
extern struct sock *syswatch_nl_sock;

//extern int syswatch_nl_init(void);
extern int syswatch_nl_send(unsigned int type, unsigned char *buf, unsigned int len);

extern int syswatch_sendLog(unsigned int logType, unsigned int logLevel, unsigned int logNum, unsigned char *str);

#ifdef CONFIG_ATP_CT_CLEAN
extern void hw_ctclean_process(struct sk_buff *skb);
#endif
#endif // End of __ATP_KCMSMAIN_MSGTYPES_H__

