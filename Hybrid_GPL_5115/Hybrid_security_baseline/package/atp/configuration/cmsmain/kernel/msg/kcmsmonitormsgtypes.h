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
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
    ATP_MONITOR_EVT_SERVLED_STOP,    /*Service LED stop*/
#endif
};

/*内核和用户态通信的netlink协议类型*/
#define NETLINK_SYSWATCH                20
#define SYSWATCH_USERSPACE_GROUP    31
#define KERNEL_PID      0
#define NETLINK_MSG_CMD_LEN         64

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

#endif // End of __ATP_KCMSMAIN_MSGTYPES_H__

