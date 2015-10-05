/***********************************************************************
  版权信息: 版权所有(C) 1988-2008, 华为技术有限公司.
  文件名: qmitty.h
  作者: houxiaohe 00118013
  版本: 001
  创建日期: 2008-5-21
  完成日期: 2008-5-25
  功能描述: qmitty.h头文件中，主要定义了实现TTY驱动模块所要用到的宏定义，
            如：ioctl要使用的命令码；主设备文件号；消息缓冲区的行坐标大
            小列坐标的大小；重试次数；定时间隔；结点和链表标志符；消息结
            点结构定义；内部匹配结构数组

  主要函数列表:

  修改历史记录列表:
    <作  者>    <修改时间>  <版本>  <修改描述>
    houxiaohe   2008-06-28   001    修改轮询中断端点定时器间隔为4毫秒
    houxiaohe   2008-07-01   001    修改消息结点结构添加purb和trans_start成员
                                    添加悬挂定时器间隔宏定义
    houxiaohe   2008-07-18  B000    由于在中断完成例程中用 tasklet_scheule调用来
                                    进行控制端点的QMI消息读写造成了单板的反复重启
                                    所以改变消息结点的结构，purb和dr定义为消息结点
                                    内部的结构成员，分别为 struct urb和struct
                                    usb_ctrlrequest包的实例。
    houxiaohe   2008-07-25  B000    修改 tty_memo 结构中的 transaction_id 为 unsigned char

  备注:
 ************************************************************************/
#ifndef _QMITTY_H
#define _QMITTY_H

#define QMI_TTY_MAJOR 234   /*QMITTY驱动模块使用的主设备号*/
#define QMI_TTY_MINOR 4     /*QMITTY驱动模块支持的用户进程数，用于构成副设备号*/

#define QMI_TTY_ROOM_SIZE 1024  /*固定返回一个消息缓冲区一维数组的长度*/

/*QMITTY模块选用的Magic数字*/
#define QMI_TTY_IOC_MAGIC 'x'

/*用于WDS进程查询START_NETWORK_INTERFACE业务ID*/
#define QMI_TTY_IOCQNUM _IO(QMI_TTY_IOC_MAGIC, 3)

/*用于其他用户进程查询获得的文件号对应的设备匹配数组中是否已经获得Client ID*/
#define QMI_TTY_IOCQNUM_CID _IOWR(QMI_TTY_IOC_MAGIC, 4, QMI_IOCTL_PARAMS_ST)  /* <DTS2010121503955.QMITTY: gKF34687 2011-01-19 MOD>*/

#define QMI_TTY_IOCQNUM_TEST02 _IO(QMI_TTY_IOC_MAGIC, 5)
#define QMI_TTY_IOCQNUM_TEST03 _IO(QMI_TTY_IOC_MAGIC, 6)
#define QMI_TTY_IOCQNUM_TEST04 _IO(QMI_TTY_IOC_MAGIC, 7)
#define QMI_TTY_IOCQNUM_TEST05 _IO(QMI_TTY_IOC_MAGIC, 8)
#define QMI_TTY_IOCQNUM_TEST06 _IO(QMI_TTY_IOC_MAGIC, 9)
#define QMI_TTY_IOCQNUM_TEST07 _IO(QMI_TTY_IOC_MAGIC, 10)
#define QMI_TTY_IOCQNUM_TEST08 _IO(QMI_TTY_IOC_MAGIC, 11)

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

/* <DTS2010121503955.QMITTY: gKF34687 2011-01-19 ADD BEGIN */
/* mayang 2011-1-13 QMI ioctl params */
typedef struct QmiIoctlParams
{
    unsigned char client_id;
}QMI_IOCTL_PARAMS_ST;
/* DTS2010121503955.QMITTY: gKF34687 2011-01-19 ADD END> */

/* tty_memo结构用于在QMITTY内部，记录上层用户进程打开驱动的关联信息，
 *  通过记录下的tty_struct指针，可以确定读出的QMI消息反馈，送入哪一个
 *  用户进程的空间。
 */
struct _tty_memo
{
    unsigned char client_id;    /*用于记录用户进程获取的Client ID*/
    unsigned char type;     /*用于记录用户进程申请的服务类型*/
    /* <AU3D00183.QMITTY: houxiaohe 2008-7-25 MODIFY BEGIN */
    unsigned char transaction_id;   /*用于记录特定的业务ID*/
    /* AU3D00183.QMITTY: houxiaohe 2008-7-25 MODIFY END> */
    struct tty_struct *   ptr_tty;      /*用于记录用户进程打开tty驱动时使用的tty_struct指针*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _tty_memo tty_memo;

/* QMI_NODE结构是为了便于管理QMITTY模块收发QMI消息而定义的,消息缓冲区采用数组
 *  是考虑到收发消息相对频繁，每次都有内存的请求和释放同样是有开销的
 */

/* <AU3D00181.QMITTY: houxiaohe 2008-7-18 MODIFY BEGIN */
struct _QMI_NODE
{
    struct list_head    ptr_qmi_node;   /*双端指针用于消息结点的链表操作*/
    unsigned char*       p_msg_array;    /*指向SDU的地址*/
    unsigned int         row_num;        /*相对应消息缓冲区的行坐标*/
    unsigned long        trans_start;    /*记录被提交的urb停留的时间*/
    struct urb*         purb;           /*向底层控制通道貌提交的urb*/
    struct usb_ctrlrequest dr;          /*向底层控制通道提交的Setup包*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_NODE QMI_NODE, * PQMI_NODE;

/* AU3D00181.QMITTY: houxiaohe 2008-7-18 MODIFY END> */

/*宏定义用于打印轮询中断定时器前100次的查询结果指示*/
#define PRT_TIMER_INFO

/*宏定义用于在初始化过程中测试检查，信号数组、消息结点、消息缓冲区、消息结点初始化入队是否成功进行*/
#define TEST_INIT_NODE_AND_AREA

/* qmi_tty_open中测试0号端点的基本收发功能 */
//#define QMI_TTY_EP0_TEST

/* 测试中断端点功能时，依需要把开启功能 */
#define QMI_TTY_INTR_SUPPORT

/* 测试中断端点功能时，依需要把开启功能 */
#define QMI_TTY_INTR_TEST

/* 使用usb_control_msg函数发送QMI消息 */
#define QMI_TTY_USB_CNTL_MSG

/* 打印qmi消息 */
//#define QMI_MSG_PRINT_DBG

#define CLASS_HOST_TO_INTERFACE     0x21    /*USB设备请求类型*/
#define CLASS_INTERFACE_TO_HOST     0XA1    /*USB设备请求类型*/
#define CDC_SEND_ENCAPSULATED_CMD   0x00    /*USB请求命令码*/
#define CDC_GET_ENCAPSULATED_RSP    0x01    /*USB请求命令码*/
#define CDC_SET_LINE_CODING          0x20    /*USB线路设置命令*/
#define CDC_GET_LINE_CODING          0x21    /*USB线路设置命令*/
#define CDC_SET_CONTROL_LINE_STATE  0x22    /*USB线路设置命令*/
#define CDC_SEND_BREAK                0x23    /*USB线路设置命令*/
#define CDC_SET_COMM_FEATURE         0x02    /*USB线路设置命令*/

#define ERR_COMMAND_NOT_EXIST        0x3F0   /*QMITTY模块中为IOCTL调用定义的错误码，命令码不存在*/
#define ERR_COMMAND_NOT_IMPLEMENT    0x3F1  /*QMITTY模块中为IOCTL调用定义的错误码，命令码没有被实现*/

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h> /* for in_interrupt() */
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

extern int set_rmnet_device(struct usb_device * rmnet_device);
extern int set_rmnet_intr_ep(struct usb_endpoint_descriptor * ep_int);
extern int trigger_tty(void);
extern int exit_tty(void);

#endif  //_QMITTY_H
