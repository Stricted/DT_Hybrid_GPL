/* <DTS2013010905391 add ndistty driver z00185914 20130112 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2013, 华为技术有限公司.
  文件名: ndistty.h
  作者: zhangjiqiang 00185914
  版本: 001
  创建日期: 2013-1-8
  完成日期: 2013-1-8
  功能描述: ndistty.h头文件中，主要定义了实现TTY驱动模块所要用到的宏定义，
            如：ioctl要使用的命令码；主设备文件号；消息缓冲区的行坐标大
            小列坐标的大小；重试次数；定时间隔；结点和链表标志符；消息结
            点结构定义；内部匹配结构数组

  主要函数列表:

  修改历史记录列表:
    <作  者>    <修改时间>  <版本>  <修改描述>
    zhangjiqiang   2013-1-8   001    以NDISTTY为基础修改为AT的balong NDIS驱动

  备注:
 ************************************************************************/
/*===============================================================
修改记录:
问题单号             修改时间    修改人         修改描述
DTS2013011408161 20130125    z00185914     将hw_cdc_driver中的中断处理移到ndistty中，新增hw_cdc_driver.h文件
================================================================*/
#ifndef _NDISTTY_H
#define _NDISTTY_H

#define NDIS_TTY_MAJOR 234   /*NDISTTY驱动模块使用的主设备号*/
#define NDIS_TTY_MINOR 4     /*NDISTTY驱动模块支持的用户进程数，用于构成副设备号*/

#define NDIS_TTY_ROOM_SIZE 1024  /*固定返回一个消息缓冲区一维数组的长度*/

typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
/*NDISTTY模块选用的Magic数字*/
#define NDIS_TTY_IOC_MAGIC 'x'

/*用于WDS进程查询START_NETWORK_INTERFACE业务ID*/
#define NDIS_TTY_IOCQNUM _IO(NDIS_TTY_IOC_MAGIC, 3)

/*用于其他用户进程查询获得的文件号对应的设备匹配数组中是否已经获得Client ID*/
#define NDIS_TTY_IOCQNUM_CID _IOWR(NDIS_TTY_IOC_MAGIC, 4, NDIS_IOCTL_PARAMS_ST)  /* <DTS2010121503955.NDISTTY: gKF34687 2011-01-19 MOD>*/

#define NDIS_TTY_IOCQNUM_TEST02 _IO(NDIS_TTY_IOC_MAGIC, 5)
#define NDIS_TTY_IOCQNUM_TEST03 _IO(NDIS_TTY_IOC_MAGIC, 6)
#define NDIS_TTY_IOCQNUM_TEST04 _IO(NDIS_TTY_IOC_MAGIC, 7)
#define NDIS_TTY_IOCQNUM_TEST05 _IO(NDIS_TTY_IOC_MAGIC, 8)
#define NDIS_TTY_IOCQNUM_TEST06 _IO(NDIS_TTY_IOC_MAGIC, 9)
#define NDIS_TTY_IOCQNUM_TEST07 _IO(NDIS_TTY_IOC_MAGIC, 10)
#define NDIS_TTY_IOCQNUM_TEST08 _IO(NDIS_TTY_IOC_MAGIC, 11)

#define MSG_ROW 6   /*消息缓冲区二维数组的行的大小*/
#define MSG_COLUMN 1024    /*消息缓冲区二维数组的列的大小*/
#define INTERRUPT_PACKET_SIZE 16  /*读中断通道的缓冲区的大小*/

#define TIMER_INTERVAL 1 /*(1 / 10)*/   /*定时器的启动的频率*/
#define WAIT_INTERVALTIME (50)    /*等待时长，50毫秒*/
#define RETRY_TIMES 3  /*初始阶段收发消息的重试次数*/
#define CHECK_INTERVAL 2      /*悬挂状态检查定时器间隔2秒*/
#define PENDING_TIME 200 /* 悬挂等待时长*/
#define EXIT_PENDING_TIME 500   /*  退出等待时长*/

#define  USER_NODE_TYPE (0x0800)        /*自定义结点类型宏定义*/
#define  FREE_NODE (USER_NODE_TYPE + 1)
#define  QOS_NODE (USER_NODE_TYPE + 2)

#define  USER_NODE_LIST_TYPE (0x08F0)            /*自定义的链表类型宏定义*/
#define  READ_LIST (USER_NODE_LIST_TYPE + 1)
#define  SENT_LIST (USER_NODE_LIST_TYPE + 2)
#define  QOS_LIST (USER_NODE_LIST_TYPE + 3)

typedef  unsigned int       BOOL;
typedef  struct list_head * plisthead;
#define   TRUE 1
#define   FALSE 0

#define CDC_NOTIFICATION_NETWORK_CONNECTION 0x00    /*高通定义的网络连接指示的定义*/
#define CDC_NOTIFICATION_RESPONSE_AVAILABLE 0x01    /*高通定义的有命令通告指示的定义*/
#define CDC_NOTIFICATION_SERIAL_STATE 0x20    /*高通定义的串行链路状态的定义*/
#define CDC_NOTIFICATION_CONNECTION_SPD_CHG 0x2A    /*高通定义的连接速率改变的定义*/

/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
typedef struct NdisIoctlParams
{
    unsigned char client_id;
}NDIS_IOCTL_PARAMS_ST;
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */


/* tty_memo结构用于在NDISTTY内部，记录上层用户进程打开驱动的关联信息，
 *  通过记录下的tty_struct指针，可以确定读出的NDIS消息反馈，送入哪一个
 *  用户进程的空间。
 */
struct _tty_memo
{
    unsigned char client_id;    /*用于记录用户进程获取的Client ID*/
    unsigned char type;     /*用于记录用户进程申请的服务类型*/
    /* <AU3D00183.NDISTTY: houxiaohe 2008-7-25 MODIFY BEGIN */
    unsigned char transaction_id;   /*用于记录特定的业务ID*/
    /* AU3D00183.NDISTTY: houxiaohe 2008-7-25 MODIFY END> */
    struct tty_struct *   ptr_tty;      /*用于记录用户进程打开tty驱动时使用的tty_struct指针*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _tty_memo tty_memo;

/* NDIS_NODE结构是为了便于管理NDISTTY模块收发NDIS消息而定义的,消息缓冲区采用数组
 *  是考虑到收发消息相对频繁，每次都有内存的请求和释放同样是有开销的
 */

struct _NDIS_NODE
{
    struct list_head    ptr_ndis_node;   /*双端指针用于消息结点的链表操作*/
    unsigned char*       p_msg_array;    /*指向SDU的地址*/
    unsigned int         row_num;        /*相对应消息缓冲区的行坐标*/
    unsigned long        trans_start;    /*记录被提交的urb停留的时间*/
    struct urb*         purb;           /*向底层控制通道貌提交的urb*/
    struct usb_ctrlrequest dr;          /*向底层控制通道提交的Setup包*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _NDIS_NODE NDIS_NODE, * PNDIS_NODE;


/*宏定义用于打印轮询中断定时器前100次的查询结果指示*/
#define PRT_TIMER_INFO

/*宏定义用于在初始化过程中测试检查，信号数组、消息结点、消息缓冲区、消息结点初始化入队是否成功进行*/
#define TEST_INIT_NODE_AND_AREA

/* ndis_tty_open中测试0号端点的基本收发功能 */
//#define NDIS_TTY_EP0_TEST

/* 测试中断端点功能时，依需要把开启功能 */
#define NDIS_TTY_INTR_SUPPORT

/* 测试中断端点功能时，依需要把开启功能 */
#define NDIS_TTY_INTR_TEST

/* 使用usb_control_msg函数发送NDIS消息 */
#define NDIS_TTY_USB_CNTL_MSG

/* 打印ndis消息 */
//#define NDIS_MSG_PRINT_DBG

#define CLASS_HOST_TO_INTERFACE     0x21    /*USB设备请求类型*/
#define CLASS_INTERFACE_TO_HOST     0XA1    /*USB设备请求类型*/
#define CDC_SEND_ENCAPSULATED_CMD   0x00    /*USB请求命令码*/
#define CDC_GET_ENCAPSULATED_RSP    0x01    /*USB请求命令码*/
#define CDC_SET_LINE_CODING          0x20    /*USB线路设置命令*/
#define CDC_GET_LINE_CODING          0x21    /*USB线路设置命令*/
#define CDC_SET_CONTROL_LINE_STATE  0x22    /*USB线路设置命令*/
#define CDC_SEND_BREAK                0x23    /*USB线路设置命令*/
#define CDC_SET_COMM_FEATURE         0x02    /*USB线路设置命令*/

#define ERR_COMMAND_NOT_EXIST        0x3F0   /*NDISTTY模块中为IOCTL调用定义的错误码，命令码不存在*/
#define ERR_COMMAND_NOT_IMPLEMENT    0x3F1  /*NDISTTY模块中为IOCTL调用定义的错误码，命令码没有被实现*/
struct _USB_NOTIFICATION_STATUS
{
    UCHAR  bmRequestType;
    UCHAR  bNotification;   /*对读取中断通道结果的主要的判定值*/
    USHORT wValue;
    USHORT wIndex;  // interface #
    USHORT wLength; // number of data bytes
    USHORT usValue; // serial status, etc.
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _USB_NOTIFICATION_STATUS USB_NOTIFICATION_STATUS, * PUSB_NOTIFICATION_STATUS;

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h> /* for in_interrupt() */
#include <linux/netdevice.h>
static __inline__ void wait_ms(unsigned int ms)
{
    unsigned long expire;
    expire = ms ? msecs_to_jiffies(ms) : MAX_SCHEDULE_TIMEOUT;
    if (!in_interrupt())
    {
        current->state = TASK_UNINTERRUPTIBLE;
        schedule_timeout(expire);
    }
    else
    {
        mdelay(ms);
    }
}

/* DTS2013011408161 move urb request to ndistty z00185914 20130125 DELETE 4 lines */
#endif  //_NDISTTY_H

/* DTS2013010905391 add ndistty driver z00185914 20130112 END> */
