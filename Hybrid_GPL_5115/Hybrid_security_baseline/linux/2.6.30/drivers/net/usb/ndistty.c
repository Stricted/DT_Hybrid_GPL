/* <DTS2013010905391 add ndistty driver z00185914 20130112 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2008, 华为技术有限公司.
  文件名: NDISTTY.c
  作者: houxiaohe 00118013
  版本: 001
  创建日期: 2008-5-21
  完成日期: 2008-5-25
  功能描述: NDISTTY模块是实现BALONG Rm Net的重要组成部分，模块在成功加载并被触
            发以后，注册了一组设备文件结点。用户进程通过打开设备文件结点来
            完成NDIS消息的读写，完成和无线模块的NDIS消息交互。同时在目前的实
            框架中，NDISTTY模块为hw_cdc_driver模块构造出发送和接收QOS消息的调用接口
            ，NDISTTY模块和hw_cdc_driver模块合作完成对于数据包发送的QOS流控。为了有
            效利用资源，模块内部用链表管理一组消息缓冲区。

  主要函数列表: trigger_tty, exit_tty, compose_internal_request, send_msg_to_user_space,
                interrupt_timer_proc, handle_reponse_available, get_node, insert_node_to_list,
                remove_list_node, return_node_to_free, ndis_tty_open,
                ndis_tty_close, ndis_tty_write,

                write_control_ndis_msg, write_ndis_completion, read_control_ndis_msg,
                read_ndis_completion, handler_resolve_available_msg,

  修改历史记录列表:
    <作  者>    <修改时间>  <版本>  <修改描述>
    zhangjiqiang   2013-1-8   001    以QMITTY为基础修改为AT的balong NDIS驱动

  备注:
 ************************************************************************/
/*===============================================================
修改记录:
问题单号             修改时间    修改人         修改描述
DTS2013011408161 20130125    z00185914     将hw_cdc_driver中的中断处理移到ndistty中，新增hw_cdc_driver.h文件
DTS2013021703454 2013/02/17	z00185914     	【拨号】优化ndis拨号代码 
DTS2013020703621 20130207    q00186737     [B880]histudio透传log
================================================================*/
/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/smp_lock.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include "hw_cdc_driver.h"
#include "ndistty.h"

//#include <linux/netdevice.h>
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */

#ifdef  _lint
#undef  copy_from_user
#define copy_from_user strncpy
#endif
static int debug;
module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "debug=1 is turn on debug messages");
//#define NDIS_DEBUG_MSG(args...) if (debug) printk (KERN_DEBUG "ndistty: " args)
#define NDIS_DEBUG_MSG(fmt, arg...)  if (debug) printk (KERN_DEBUG "ndistty[%u]: " fmt , __LINE__ , ## arg)

/********************* 外部变量 - 定义在usbnet模块中 *********************/

/* usbnet的probe函数中探查到的RmNet指针和中断端点的端点描述符*/
static struct usb_device * rmnet;
static struct usb_endpoint_descriptor * ep_int_rmnet;
/* < DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 delete 1 row */
/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
static struct hw_cdc_net	 *hwcdc_net;
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */

/* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */
#ifdef CONFIG_B880_PRODUCT		
/* <DTS2013020703621 q00186737 2013/02/07 begin*/		
#define NDIS_IF_IDX 0x05	
/* DTS2013020703621 q00186737 2013/02/07 end> */
#else
#define NDIS_IF_IDX 0x04
#endif
/* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */

/********************* 外部接口函数 - 提供给usbnet模块使用 *********************/



/********************* 本模块使用的全局变量 *********************/
/*  USB网卡是否正常连接识别 */
volatile unsigned int connect_stat = 0;

/*  中断端点urb */
static struct urb * p_int_urb = NULL;

static struct work_struct intr_work;

/*  记录对应接入tty结构信息 */
static tty_memo external_access_name[NDIS_TTY_MINOR];

/* NDIS消息节点 */
NDIS_NODE message_node[MSG_ROW];

/* NDIS消息缓存区 */
unsigned char (*message_area)[MSG_ROW][MSG_COLUMN];

#define MEG_ROW(row) ((unsigned char *)message_area + (row * MSG_COLUMN))

/* NDIS消息读取节点 */
PNDIS_NODE pndis_read_node;
/* NDIS消息写入节点 */
PNDIS_NODE pndis_write_node;


/*读中断通道的缓冲区的大小*/
unsigned char interrupt_query_buf[INTERRUPT_PACKET_SIZE * MSG_ROW];




/* 中断轮询是否建立起来 */
static unsigned char intr_setup_flag = 0;

/* 用于管理内存消息缓冲区和消息结点的链表 */
struct list_head free_list;

/* 链表操作函数，加入自旋锁避免竞态 */
static spinlock_t lock_for_list = SPIN_LOCK_UNLOCKED;

/* mayang 2011-9-22 添加sem保证ndis首次open时初始化正常完成 */
static DECLARE_MUTEX(sema);

/********************* 本模块使用的函数声明 *********************/

/* usb底层NDIS消息的读写函数和完成例程*/
static int start_intr_ep_query(void);
static void intr_completion(struct urb * purb);
static int write_control_ndis_msg(PNDIS_NODE pnode, int count);
static int read_control_ndis_msg(PNDIS_NODE pnode, int count);
static void write_ndis_completion(struct urb * purb);
static void read_ndis_completion(struct urb * purb);

static void free_urbs(void);
static void kill_urbs(void); 

static void compose_internal_request(PNDIS_NODE ptr, unsigned int flag);
static void broadcast_msg(PNDIS_NODE pnode, int count);
static void send_msg_to_user_space(PNDIS_NODE pnode, struct tty_struct * tty, int count);
static void handle_get_version_response(PNDIS_NODE pnode, int ret);
static void handle_get_clientid_response(PNDIS_NODE pnode, int ret);
static void handle_release_clientid_response(PNDIS_NODE pnode, int ret);

static void handler_resolve_available_msg(PNDIS_NODE pnode, int ret);
static void handle_response_available(void);
static void clean_dummy_msg(void);

static void init_dev_message_node_and_area(void);
static void clean_node_and_area(PNDIS_NODE pnode);
static void return_node_to_free(PNDIS_NODE pnode);
static PNDIS_NODE get_free_node();

/* NDISTTY驱动模块的内核接口函数 */
static int ndis_tty_open(struct tty_struct * tty, struct file * filp);
static void ndis_tty_close(struct tty_struct * tty, struct file * filp);
static int ndis_tty_write(struct tty_struct * tty, const unsigned char * buf, int count);
static int ndis_tty_write_room(struct tty_struct * tty);
static int ndis_tty_ioctl(struct tty_struct * tty, struct file * filp, unsigned int cmd, unsigned long arg);
static void	ndis_tty_throttle(struct tty_struct * tty);
static void ndis_tty_unthrottle(struct tty_struct * tty);

/*打印前100次中断端点轮询结果的打印宏*/
#ifdef PRT_TIMER_INFO
static int count_prn = 0;
 #define PRT_INFO_TIMES 10
 #define VIEW_TIMER(str) \
    do \
    {\
        if (count_prn++ < PRT_INFO_TIMES)\
        {printk(KERN_ALERT "The response message is %s\n", str);}\
    } while (0)
#else
 #define VIEW_TIMER(s)
#endif

/* 计算两个unsigned long 值的差*/
#define differ_ret(i, j) (i > j ? i - j : (j ^ (~0)) + i)

/*lint -save -e* */
static struct tty_driver* ndis_tty_driver;
/*lint -restore*/

int set_rmnet_device(struct usb_device * rmnet_device)
{
    rmnet = rmnet_device;
    return 0;
}
EXPORT_SYMBOL_GPL(set_rmnet_device);

/* < DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 delete 6 row */
/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
int set_hwcdc_device(struct hw_cdc_net *dev)
{
    hwcdc_net = dev;
    return 0;
}
EXPORT_SYMBOL_GPL(set_hwcdc_device);
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */

int set_rmnet_intr_ep(struct usb_endpoint_descriptor * ep_int)
{
    ep_int_rmnet = ep_int;
    return 0;
}
EXPORT_SYMBOL_GPL(set_rmnet_intr_ep);

/*------------------------------------------------------------
  函数原型: void trigger_tty()
  描述: 建立NDISTTY状态机的触发器函数,由usbnet的probe探测函数在查询
        到Rm Net接口后，并成功配置完中断通道、批量数据通道以后调用，
        trigger_tty触发器函数内部开始初始化所有要用的内部缓冲区结构、
        内部匹配数据结构、调用建立NDIS消息接口的内部过程，调用函数查
        询网络是否支持QOS，并在建立成功以后注册设备文件结点。
  输入: 无
  输出: 成功建立消息接口后，会注册设备文件结点；否则不注册，建立失败
  返回值: 无
-------------------------------------------------------------*/
int trigger_tty(void)
{
	int i = 0;
    p_int_urb = NULL;
    connect_stat = 1;

    NDIS_DEBUG_MSG("trigger_tty is running!\n");
    NDIS_DEBUG_MSG("bEpAddress=0x%0x, bInterval=0x%0x\n", ep_int_rmnet->bEndpointAddress, ep_int_rmnet->bInterval);

	/* 分配消息缓冲区 */
    message_area = kmalloc(sizeof(*message_area), GFP_KERNEL);
    if (NULL == message_area)
    {
    	printk("%s: Kmalloc the buffer failed.\n", __FUNCTION__);
    	return -ENOMEM;
    }
    NDIS_DEBUG_MSG("alloc area ok [%d]\n", sizeof(*message_area));

    /* 初始化内部变量和消息节点 */
    init_dev_message_node_and_area();

    /*初始化所用到链表头*/
    INIT_LIST_HEAD(&free_list);

    /*每一个消息结点的读写缓冲区指针指向相应行坐标所指向的数组的*
     *SDU的起始位置，消息结点的坐标成员赋为对应的行坐标。在初始阶*
    段所有的消息结点都链入空闲消息链表中。*/
    for (i = 0; i < MSG_ROW; ++i)
    {
        message_node[i].p_msg_array = MEG_ROW(i);
        message_node[i].row_num = (unsigned int)i;
        list_add_tail((plisthead) & message_node[i].ptr_ndis_node, &free_list);
    }

    /* alloc urbs */
    for (i = 0; i < MSG_ROW; ++i)
    {
        message_node[i].purb = usb_alloc_urb (0, GFP_KERNEL);
        if (!message_node[i].purb)
        {
        	printk("%s - No free urbs available, i = %d\n", __FUNCTION__, i);
        	/* free all non-null urb */
        	free_urbs();
            //return -ENOMEM;
        }
    }

    /* 初始化读写节点 */
    pndis_write_node = (PNDIS_NODE)&(message_node[0]);
    pndis_read_node = (PNDIS_NODE)&(message_node[1]);
    
    NDIS_DEBUG_MSG("trigger_tty ok\n");
    return 0;
}
EXPORT_SYMBOL_GPL(trigger_tty);

/*------------------------------------------------------------
  函数原型: void exit_tty()
  描述: 退出NDISTTY状态机的函数。目前对于退出时机的考虑主要是在无
        线模块复位后，USB连接被复位，Usbnet调用disconnect方法，在
        usbnet的disconnect方法中调用 exit_tty ，使NDISTTY状态机撤
        销。
  输入: 无
  输出: 设备文件结点被注销
  返回值: 无
-------------------------------------------------------------*/
int exit_tty(void)
{
    free_urbs();
    connect_stat = 0;

    intr_setup_flag = 0; 

	if (NULL != message_area)
	{
		kfree(message_area);
	}	
	
    NDIS_DEBUG_MSG("exit_tty\n");
    return 0;
}
EXPORT_SYMBOL_GPL(exit_tty);

static void kill_urbs()
{
    int i;
    /* free urbs */
    for (i = 0; i < MSG_ROW; ++i)
    {
		if (message_node[i].purb)
        {
			/* kill in_progress urb */
        	if (message_node[i].purb->status == -EINPROGRESS)
            {   
    			printk("%s - kill urbs, i = %d\n", __FUNCTION__, i);
            	usb_kill_urb(message_node[i].purb);
            }
		}
    }

    NDIS_DEBUG_MSG("kill_urbs\n");
}

static void free_urbs()
{
    int i;
    /* free urbs */
    if(p_int_urb)
    {
    	if (p_int_urb->status == -EINPROGRESS)
        {   
    		usb_kill_urb(p_int_urb);
        }
    	usb_free_urb(p_int_urb);
    }
    
    for (i = 0; i < MSG_ROW; ++i)
    {
        if (message_node[i].purb)
        {
            NDIS_DEBUG_MSG("%s - free urbs, i = %d\n", __FUNCTION__, i);
            /* free all non-null urb */
            if (message_node[i].purb->status == -EINPROGRESS)
            {   
                usb_kill_urb(message_node[i].purb);
            }
            usb_free_urb(message_node[i].purb);
        }
    }

    NDIS_DEBUG_MSG("free_urbs\n");
}
/*------------------------------------------------------------
  函数原型: int ndis_tty_open(struct tty_struct * tty, struct file * filp)
  描述: ndis_tty_open函数是tty驱动的接口函数，当用户进程使用open函数打开
        相应的设备文件结点时，tty核心将会调用ndis_tty_open方法，完成打开
        的操作，为了支持多个用户进程，在打开过程中要对用户进程使用的副
        设备号进行判别，对于不是WDS进程的用户进程，NDISTTY模块内部构造出
        1条获取WDS Client ID的NDIS消息发往无线模块。并会在得到相应Response
        以后，记录在设备匹配数组之中。
        WDS应用进程在打开NDISTTY模块后，如果内部状态机标志值没有置位，则开启
        状态机创建。
  输入: tty_struct * tty 指向内核TTY结构的指针
        file * filp指向内核文件结构的指针
  输出: 无
  返回值: int ret 通常都返回0表示打开操作已经完成。
-------------------------------------------------------------*/
static int ndis_tty_open(struct tty_struct * tty, struct file * filp)
{
/* <DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 BEGIN */
    int ret = 0;
    unsigned short num = 0;
/* < DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 delete 2 row */

    num = tty->index;	
    if (0 != external_access_name[num].ptr_tty)
    {
        printk("%s - ndistty%d is already opened by someone!\n", __FUNCTION__, num);
        return -EBUSY;
    }
    NDIS_DEBUG_MSG("%s - open ndistty%d[%s]...\n", __FUNCTION__, num, dev_name(&rmnet->dev));

    tty->low_latency = 1;		
    tty->raw = 1;
    tty->real_raw = 1;
/* DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 END> */

    u16 status;
    ret = usb_get_status(rmnet, USB_RECIP_ENDPOINT, 0, &status);
    NDIS_DEBUG_MSG("usb_get_ep_status, ret=%d [0x%04x]\n", ret, status);

	ret = usb_get_status(rmnet, USB_RECIP_INTERFACE, NDIS_IF_IDX, &status);
    NDIS_DEBUG_MSG("usb_get_intf_status, ret=%d[0x%04x]\n", ret, status);

    if (connect_stat)
    {
/* < DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 delete 1 row */

        /* 加锁保证第一次初始化正常完成 */
        down_interruptible(&sema);
		if (1 == intr_setup_flag)
		{
			if((NULL != p_int_urb) && (-EINPROGRESS == p_int_urb->status))
            {
                usb_kill_urb(p_int_urb);
            }
		}
			/* 启动中断端点轮询 */
            ret = start_intr_ep_query();
            if (ret < 0)
            {
                printk(KERN_ALERT "%s- start intr query failed.\n", __FUNCTION__);
                if((NULL != p_int_urb) && (-EINPROGRESS == p_int_urb->status))
                {
                    usb_kill_urb(p_int_urb);
                }
                up(&sema);
                return ret;
            }
            intr_setup_flag = 1;
        /* 初始化ndis消息，获取ndis version及对应上层tty的client id
           当前只支持/dev/ndistty0, 其他先不支持
         */
        up(&sema);
        /* 记录上层接入client对应的tty结构 */
        external_access_name[num].ptr_tty = tty;
    }
    else
    {
        printk("NDISTTY not actived.\n");
        return -ENODEV;
    }

    NDIS_DEBUG_MSG("NDISTTY is opened by %d th devfs node\n", num);
    return 0;
}

/*------------------------------------------------------------
  函数原型: void ndis_tty_close(struct tty_struct * tty, struct file * filp)
  描述: ndis_tty_close函数是tty驱动的接口函数，当用户进程使用close函数关闭
        相应的设备文件结点时，tty核心将会调用ndis_tty_close方法，完成关闭
        的操作，为了支持多个用户进程，在关闭过程中要对用户进程使用的副
        设备号进行判别，对于不是WDS进程的用户进程，NDISTTY模块内部构造出
        1条释放WDS Client ID的NDIS消息发往无线模块。并会在得到相应Response
        以后，清除设备匹配数组之中相应的记录项。默认WDS进程不能退出，如果
        WDS进程退出则NDISTTY驱动模块退出。
  输入: tty_struct * tty 指向内核TTY结构的指针
        file * filp指向内核文件结构的指针
  输出: 无
  返回值: 无
-------------------------------------------------------------*/
static void ndis_tty_close(struct tty_struct * tty, struct file * filp)
{
    unsigned short num = 0;
    num = tty->index;
    if (NULL == external_access_name[num].ptr_tty)
    {
        printk("%s - ndistty%d is already closed!\n", __FUNCTION__, num);
        return;
    }

	NDIS_DEBUG_MSG("%s - close ndistty%d successfully! \n", __FUNCTION__, num);            
    memset((void *)&external_access_name[num], 0, sizeof(tty_memo));


    NDIS_DEBUG_MSG("NDISTTY is closed by %d th devfs node\n", num);
}

/*------------------------------------------------------------
  函数原型: int ndis_tty_write(struct tty_struct * tty, int from_user, const unsigned char * buf, int count)
  描述: ndis_tty_write函数是ndistty驱动所定义的tty驱动的最重要的操作
        方法之一。当用户进程用open方法打开设备文件结点时，tty核心
        会调用我们注册的tty驱动的方法集中的 ndis_tty_write 函数，来
        完成把用户空间的数据传入内核。在ndis_tty_write中会把用户输入
        的消息包装成为了一个NDIS消息，然后向USB底层发送。
  输入: struct tty_struct * tty  系统传入的指向tty_struct的指针
        int  from_user  指示操作是在用户空间还是在内核空间。
        const unsigned char * buf 指向用户空间的输入数据缓冲区。
        int  count  要输入的数据字节数。
  输出: 无
  返回值: 在USB底层成功传输的字节数减去NDIS消息头的长度。
-------------------------------------------------------------*/
static int ndis_tty_write(struct tty_struct * tty, const unsigned char * buf, int count)
{
    int ret = 0;
    unsigned short num = 0;
    int nMsgLen = 0;
    PNDIS_NODE pNode = NULL;

    /*printk(KERN_ALERT "ndis_tty_write is running\n");*/    /* < DTS2011011305021: gkf34687 2011-03-02 DEL>*/
    num = tty->index;
    pNode = get_free_node();
    if (NULL == pNode)
    {
        printk("%s-can not get free node, no mem here !!\n", __FUNCTION__);
        return -ENOMEM;
    }

	if (0 == connect_stat)
	{
    	return_node_to_free(pNode);
        return -ENODEV;
	}
	memcpy((MEG_ROW(pNode->row_num)), buf, count);
    nMsgLen = ((unsigned short)((unsigned int)count - 1));
	//printk("%s - count = %d\n", __FUNCTION__, nMsgLen + 1);
    ret = write_control_ndis_msg(pNode, nMsgLen + 1);

	return ret;
}

/*------------------------------------------------------------
  函数原型: int ndis_tty_write_room(struct tty_struct * tty)
  描述: ndis_tty_write_room是tty驱动核心调用的，当tty驱动执行write
       方法时，tty驱动核心会调用ndis_tty_write_room方法来查询输入
       缓冲区的大小
  输入: 系统传入的指向tty_struct的指针
  输出: 无。
  返回值: 在本工程中返回了一个固定的大小。
-------------------------------------------------------------*/
static int ndis_tty_write_room(struct tty_struct * tty)
{
    return NDIS_TTY_ROOM_SIZE;
}

/*------------------------------------------------------------
  函数原型: void ndis_tty_throttle(struct tty_struct * tty)
  描述: ndis_tty_throttle用于当缓冲区满时，通知设备不要输入数据
  输入: 系统传入的指向tty_struct的指针。
  输出: 无。
  返回值: 无。
-------------------------------------------------------------*/
static void ndis_tty_throttle(struct tty_struct * tty)
{
    return;
}

/*------------------------------------------------------------
  函数原型: void ndis_tty_unthrottle(struct tty_struct * tty)
  描述: ndis_tty_unthrottle用于当缓冲区被清除进，通知设备输入数据
  输入: 系统传入的指向tty_struct的指针。
  输出: 无。
  返回值: 无
-------------------------------------------------------------*/
static void ndis_tty_unthrottle(struct tty_struct * tty)
{
    return;
}

/*------------------------------------------------------------
  函数原型: int ndis_tty_ioctl(struct tty_struct * tty, struct file * filp, unsigned int cmd, unsigned long arg)
  描述: ndis_tty_ioctl是重要的用户进程的控制方法接口，当用户进程
        打开了相应的tty设备文件结点时，可以使用ioctl方法来让tty
        驱动执行一些特定的操作。具体执行什么操作由功能码区分。
  输入: struct tty_struct * tty  系统传入的指向tty_struct的指针
        struct file * filp   系统维护的内部的文件指针
        unsigned int cmd  定义的功能码
        unsigned long arg  一般是一个传入的指针
  输出: 由功能码区分
  返回值: 执行特定操作的结果值。
-------------------------------------------------------------*/
static int ndis_tty_ioctl(struct tty_struct * tty, struct file * filp, unsigned int cmd, unsigned long arg)
{
    short num;

    num = tty->index;
    switch (cmd)
    {
    case NDIS_TTY_IOCQNUM:
    {
        return 0;
    }

    case NDIS_TTY_IOCQNUM_CID:
    {
        /*用户进程在打开NDISTTY设备文件结点以后，都要先用 ioctl(,NDIS_TTY_IOCQNUM_CID)来查询是否已获得Client ID*/
        NDIS_DEBUG_MSG("Query tty%d's CID=%d.\n", num, external_access_name[num].client_id);
        if (external_access_name[num].client_id)
        {
            NDIS_IOCTL_PARAMS_ST stParams;
            if (0 == copy_from_user((void*)&stParams, (void*)arg, sizeof(stParams)))
            {
                stParams.client_id = external_access_name[num].client_id;
                __copy_to_user((NDIS_IOCTL_PARAMS_ST*)arg, &stParams, sizeof(NDIS_IOCTL_PARAMS_ST));
            }
        }
        return 0;
    }
    case NDIS_TTY_IOCQNUM_TEST02:
    {
        printk(KERN_ALERT "Just do it in the third NDIS_Tty_IOCQNUM_TEST02## ");
        return (2);
    }
    case NDIS_TTY_IOCQNUM_TEST03:
    {
        printk(KERN_ALERT "Just do it in the fourth NDIS_Tty_IOCQNUM_TEST03## ");

        return (3);
    }
    case NDIS_TTY_IOCQNUM_TEST04:
    {
        printk(KERN_ALERT "Just do it in the fifth NDIS_Tty_IOCQNUM_TEST04## ");

        return (1);
    }
    case NDIS_TTY_IOCQNUM_TEST05:
    {
        printk(KERN_ALERT "Just do it in the sixth NDIS_Tty_IOCQNUM_TEST05## \n");
        return (ERR_COMMAND_NOT_IMPLEMENT);
    }
    case NDIS_TTY_IOCQNUM_TEST06:
    {
        return (ERR_COMMAND_NOT_IMPLEMENT);
    }
    case NDIS_TTY_IOCQNUM_TEST07:
    {
        printk(KERN_ALERT "Just do it in the eighth NDIS_Tty_IOCQNUM_TEST07## ");
        return (ERR_COMMAND_NOT_IMPLEMENT);
    }
    case NDIS_TTY_IOCQNUM_TEST08:
    {
        printk(KERN_ALERT "Just do it in the ninth NDIS_Tty_IOCQNUM_TEST08## ");
        return (ERR_COMMAND_NOT_IMPLEMENT);
    }

    default:
        return (ERR_COMMAND_NOT_EXIST);
    }
}

/*------------------------------------------------------------
  函数原型: int write_control_ndis_msg(PNDIS_NODE pnode, int count)
  描述: write_control_ndis_msg是通过底层的usb向对端的无线模块发送NDIS消息的函数，
        NDIS消息通过0端点发送。
  输入: PNDIS_NODE pnode 是调用者获得的空闲的消息结点，
        int count 是要发送的SDU长度
  输出: urb提交成功，主机控制器会在底层进行排队
  返回值: 发送成功返回发送消息的长度，失败则把结点重入队同时返回usb传回的错误码
-------------------------------------------------------------*/
static int write_control_ndis_msg(PNDIS_NODE pnode, int count)
{
    int ret;
    if(connect_stat)
	{
		char *pbuf = NULL;
		pbuf = MEG_ROW(pnode->row_num);
		/*
		if (0 < count)
	    {
			pbuf = (char *)kmalloc(count + 1, GFP_KERNEL);
			if (NULL == pbuf)
	        {
				printk("%s: Kmalloc the buffer failed.\n", __FUNCTION__);
				return -1;
			}
			memset(pbuf, 0, count+1);
		}
	    //memcpy(pbuf, message_area[pnode->row_num], count);
		memcpy(pbuf, MEG_ROW(pnode->row_num), count);
		*/

	    ret = usb_control_msg(rmnet,
	                          usb_sndctrlpipe(rmnet, 0),
	                          CDC_SEND_ENCAPSULATED_CMD, 
	                          CLASS_HOST_TO_INTERFACE,
	                          0x00,
	                          NDIS_IF_IDX, /* <DTS2010112503937.NDISTTY: mayang 2010-11-25 MOD> */
	                          pbuf,
	                          count,
	                          5000);
		if (ret < 0)
		{
			printk("%s - error, write < %s > ret = %d\n", __FUNCTION__, pbuf, ret);
		}

		/*
	    if (NULL != pbuf)
	    {
	        kfree(pbuf);
	        pbuf = NULL;
	    }
	    */
	    return_node_to_free(pnode);
		return ret;
	}
    else
    {
        return_node_to_free(pnode);
        return 0;
    }
}

/*------------------------------------------------------------
  函数原型: void write_ndis_completion(struct urb * purb)
  描述: write_ndis_completion是write_control_ndis_msg的完成例程，进行处理
        当提交的urb成功执行了以后的后续工作。
  输入: struct urb * purb提交了的urb
  输出: 把使用的消息结点重入队，释放urb使用的内存
  返回值: 无
-------------------------------------------------------------*/
static void write_ndis_completion(struct urb * purb)
{
    /*printk("%s - urb->status=%d\n", __FUNCTION__, purb->status);*/    
    if (purb->status)
    {
        printk("%s - error, urb->status=%d\n", __FUNCTION__, purb->status);
    }
    return_node_to_free((PNDIS_NODE )(purb->context));
}

/*------------------------------------------------------------
  函数原型: int read_control_ndis_msg(PNDIS_NODE pnode, int count)
  描述: read_control_ndis_msg是从usb底层读取无线模块发送的NDIS消息的函数，
  输入: PNDIS_NODE pnode 调用方获得的消息结点
        int count 要发送的消息长度
  输出: 成功提交urb,主机控制器在底层对于urb排队
  返回值: 成功执行返回发送了的消息长度值，失败把使用的结点重入队同时返回usb传回的错误码
-------------------------------------------------------------*/
static int read_control_ndis_msg(PNDIS_NODE pnode, int count)
{
    int ret = 0;
    if(connect_stat)
    {
#ifdef NDIS_MSG_PRINT_DBG
	    int i =0;
#endif			

	    unsigned char* pbuf = NULL;
	    pbuf = MEG_ROW(pnode->row_num);
	    /*
	    unsigned char *pbuf = kmalloc(sizeof(message_area[pnode->row_num]), GFP_KERNEL);
		if (NULL == pbuf)
	    {
			printk("%s: Kmalloc the buffer failed.\n", __FUNCTION__);
			return -1;
		}	
		memset(pbuf, 0, sizeof(message_area[pnode->row_num]));
		*/

		ret = usb_control_msg(rmnet,
		                        usb_rcvctrlpipe(rmnet, 0),
		                        CDC_GET_ENCAPSULATED_RSP,
		                        CLASS_INTERFACE_TO_HOST,
		                        0x00,
		                        NDIS_IF_IDX,/* <DTS2010112503937.NDISTTY: mayang 2010-11-25 MOD> */
		                        pbuf,
		                        count,
		                        5000);
	    if(0 < ret)
	    {
	        //printk(KERN_INFO "[%s]read NDIS MSG [%d]\n", __FUNCTION__, ((PQCNDIS)pbuf)->Length);
		
	        //for(i = 0; i < ((((PQCNDIS)pbuf)->Length)>>8) + 1;++i)
#ifdef NDIS_MSG_PRINT_DBG
	        for (i = 0; i < ret; ++i)
	        {
	            printk("0x%02x ", pbuf[i]);
				if ((0 != i) && (0 == (i+1)%16))
	            {
	                printk("\n");
	            }
	        }
	        printk("\n");
#endif			
	        //printk("\nread msgid (0x%04x) end\n", *((USHORT*)(pbuf + SDU_START_POSITION + 3)));
			
			//memcpy(message_area[pnode->row_num], pbuf, ret);
			
			handler_resolve_available_msg(pnode, ret);
	    }
		else if(0 > ret)
		{
			NDIS_DEBUG_MSG("%s - error, ret = %d\n", __FUNCTION__, ret);
			return_node_to_free(pnode);
		}
		else
		{
			return_node_to_free(pnode);
		}

		return ret;
		/*
	    if (NULL != pbuf)
	    {
	        kfree(pbuf);
	        pbuf = NULL;
	    }
		*/
    }
    else
    {
        return_node_to_free(pnode);
		printk("[%s][%s][%d]\r\n",__FILE__,__FUNCTION__,__LINE__);
        return 0;
    }	
}

/*------------------------------------------------------------
  函数原型: void read_ndis_completion(struct urb * purb)
  描述: read_ndis_completion是read_control_ndis_msg的完成例程，完成
        当提交的urb返回以后的后续工作
  输入: struct urb * purb 提交的urb的指针
  输出: 有消息读出则要进行相应的处理，同时要释放urb使用内存
  返回值: 无
-------------------------------------------------------------*/
static void read_ndis_completion(struct urb * purb)
{
    /*printk("%s - urb->status=%d, actual_length=0x%0x\n", __FUNCTION__, purb->status, purb->actual_length);*/    /* < DTS2011011305021: gkf34687 2011-03-02 DEL>*/
    if (purb->status)
    {
        printk("read error in read completion, status=%d, len=%d\n", purb->status, purb->actual_length);
        /* DTS2010112205139.NDISTTY: mayang 2010-11-20 Del 1 line */
    }

    if (purb->actual_length && purb->status == 0)
    {
        int i = 0;
        PNDIS_NODE tmpnode = purb->context;
        NDIS_DEBUG_MSG("[%s]read NDIS MSG\n", __FUNCTION__);
        for(i = 0; i < purb->actual_length;++i)
        {
            //printk("%02x ",message_area[tmpnode->row_num][i]);
			printk("%02x ", *(unsigned char *)(MEG_ROW(tmpnode->row_num)+i));
            if ((0 != i) && (0 == (i+1)%16))
            {
                printk("\n");
            }
        }
        NDIS_DEBUG_MSG("\nRead Rsp msg(0x%0x) end\n", le16_to_cpu(*(USHORT*)(MEG_ROW(tmpnode->row_num)/*+SDU_START_POSITION*/ + 3)));
        handler_resolve_available_msg((PNDIS_NODE)(purb->context), purb->actual_length);
    }
    else
    {
        return_node_to_free((PNDIS_NODE)(purb->context));
    }
}

/*------------------------------------------------------------
  函数原型: static int start_intr_ep_query()
  描述: 启动中断端点轮询
  输入: 无
  输出: 无
  返回值: 成功返回0，否则返回errno
-------------------------------------------------------------*/
static int start_intr_ep_query()
{
    int ret;
    unsigned int pipe = 0;
    unsigned int maxp = 0;
    pipe = usb_rcvintpipe(rmnet, ep_int_rmnet->bEndpointAddress);
    maxp = le16_to_cpu(ep_int_rmnet->wMaxPacketSize);
    NDIS_DEBUG_MSG("%s - pipe=0x%0x, maxp=0x%0x buf= %d\n", __FUNCTION__, pipe, maxp, sizeof(interrupt_query_buf));
    p_int_urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!p_int_urb)
    {
        printk("%s - alloc int urb failed\n", __FUNCTION__);
        return -ENOMEM;
    }

    unsigned char *pbuf = kmalloc(sizeof(interrupt_query_buf), GFP_KERNEL);
	if (NULL == pbuf)
    {
		printk("%s: Kmalloc the buffer failed.\n", __FUNCTION__);
		return -ENOMEM;
	}	
	memset(pbuf, 0, sizeof(interrupt_query_buf));
	
	memset(&intr_work, 0, sizeof(struct work_struct));
	INIT_WORK(&intr_work, handle_response_available);

    /* fill interrupt urb */
    usb_fill_int_urb(p_int_urb,
                     rmnet,
                     pipe,
                     pbuf,/*interrupt_query_buf,*/
                     maxp,
                     intr_completion,
                     &intr_work,
                     10);
    /* usb_ndis_clear_halt(rmnet, p_int_urb->pipe); */
    ret = usb_submit_urb(p_int_urb, GFP_KERNEL);
    if (ret < 0)
    {
        printk("%s - submit_urb failed, ret=%d\n", __FUNCTION__, ret);
    }
    else
    {
        NDIS_DEBUG_MSG("%s - submit_urb successful, ret=%d, len=%d\n", __FUNCTION__, ret, p_int_urb->actual_length);
        ret = p_int_urb->actual_length;
    }
    return ret;
}


/*------------------------------------------------------------
  函数原型: void intr_completion(struct urb * purb)
  描述: intr_completion是read_interrupt_ndis_msg的完成例程，
  输入: 提交的urb指针
  输出: 有效的命令通告
  返回值: 无
-------------------------------------------------------------*/
static void intr_completion(struct urb * purb)
{
    int status = 0;
/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
	int urbstatus = purb->status;
    struct usb_ctrlrequest * ctlreq;
    static int i = 0;
	
	if (-ENOENT == urbstatus || -ESHUTDOWN == urbstatus)
	{
        if (netif_msg_ifdown (hwcdc_net)){
			printk("[%s][%d] intr shutdown, code %d\r\n",__FILE__,__LINE__,urbstatus);
        }
        return;
	}
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */
	if (purb->actual_length == 0)
    {
        if (i++ < 10)
        {
            printk("%s -- i = %d\n", __FUNCTION__, i);
        }
        goto readtimer;/* lint !e801 */
    }
    else
    {
        ctlreq = (struct usb_ctrlrequest *)purb->transfer_buffer;

		//printk("Request = 0x%02x, Request type = 0x%02x\n", ctlreq->bRequest, ctlreq->bRequestType);
		if (0xa1 == ctlreq->bRequestType)
		{
	        switch (ctlreq->bRequest)
	        {
	            case CDC_NOTIFICATION_SERIAL_STATE:
	            {
	                VIEW_TIMER("CDC_NOTIFICATION_SERIAL_STATE");
	                break;
	            }
	            case CDC_NOTIFICATION_NETWORK_CONNECTION:
	            {
	                VIEW_TIMER("CDC_NOTIFICATION_NETWORK_CONNECTION");
	                break;
	            }	            
	            case CDC_NOTIFICATION_CONNECTION_SPD_CHG:
	            {
	                VIEW_TIMER("CDC_NOTIFICATION_CONNECTION_SPD_CHG");
	                break;
	            }
				case CDC_NOTIFICATION_RESPONSE_AVAILABLE:
	            {
	                VIEW_TIMER("CDC available message");
	                if(connect_stat)
	                {
						VIEW_TIMER("CDC_NOTIFICATION_RESPONSE_AVAILABLE: handle_response_available\n");
	                   // handle_response_available();/* DTS2010112205139.NDISTTY: mayang 2010-11-20 Mod */
	                    schedule_work((struct work_struct*)purb->context);
	                }
	                break;
	            }
	            default:
	            {
	                VIEW_TIMER("CDC, default case.");
	                break;
	            }
        	}
        }
    }

    /* re-submit intr urb */
    if(connect_stat)
    {
        memset(purb->transfer_buffer, 0, purb->transfer_buffer_length);
        status = usb_submit_urb (purb, GFP_ATOMIC);
		/* <DTS2013021703454 优化ndis拨号代码 z00185914 20130217 DELETE 1 ROW */
        if (status != 0)
        {
            printk("%s - resubmit intr urb failed, status=%d\n", __FUNCTION__, status);
        }
    }
    
readtimer:
    VIEW_TIMER("CDC message end");
}


/*------------------------------------------------------------
  函数原型: void    send_msg_to_user_space(PNDIS_NODE pnode, struct tty_struct * tty, int count)
  描述: send_msg_to_user_space函数是在，当从USB底层成功读入NDIS消息
        时，并且经过判别是WDS类型的消息，需要交给WDS进程或其他用户
        进程进行处理的情况，调用tty核心的缓冲区流输入和流刷新函数
        把WDS消息送入用户进程空间。
  输入: PNDIS_NODE pnode 读入WDS消息的消息结点.
        struct tty_struct * tty 记录下的对应的用户进程打开的指向的
        tty_struct的指针
        int count 要刷新到用户进程空间的字节数。
  输出: 输出到用户空间的WDS消息。
  返回值: 无。
-------------------------------------------------------------*/
static void send_msg_to_user_space(PNDIS_NODE pnode, struct tty_struct * tty, int count)
{
    int i = 0;
    if (NULL == tty)
    {
        printk("%s - tty maybe null.\n", __FUNCTION__);
        return;
    }
    /*printk("%s - send msg_len(0x%0x) to user space.\n", __FUNCTION__, count-SDU_START_POSITION);*/    /* < DTS2011011305021: gkf34687 2011-03-02 DEL>*/
	tty_buffer_request_room(tty, count);
    for (i = 0; i < count; ++i)
    {
#ifdef NDIS_MSG_PRINT_DBG
        printk("%02x ", *(MEG_ROW(pnode->row_num)+i));
#endif
        tty_insert_flip_char(tty, *(unsigned char *)(MEG_ROW(pnode->row_num)+i),
         TTY_NORMAL);
    }
#ifdef NDIS_MSG_PRINT_DBG
	printk("\n ");
#endif
    tty_flip_buffer_push(tty);
#ifdef NDIS_MSG_PRINT_DBG
    printk("\n");
#endif
}

/*------------------------------------------------------------
  函数原型: void    broadcast_msg(PNDIS_NODE pnode, int count)
  描述: broadcast_msg 是对于是从USB底层读入的需要广播处理的消息，
       处理的算法如下：首先针对每一个存在的上层WDS用户进程，执行
       send_msg_to_user_space，把消息送入用户空间。最后，把消息
       结点插入到QoS_used_list链表中，唤醒usbnet进行处理。
  输入: PNDIS_NODE pnode 需要进行处理的消息结点。
        int count 需要刷入用户空间的消息的长度。
  输出: 所有存在的用户进程和usbnet都会接收到广播消息。
  返回值: 无。
-------------------------------------------------------------*/
static void broadcast_msg(PNDIS_NODE pnode, int count)
{
    int i = 0;

    for (i = 0; i < NDIS_TTY_MINOR; ++i)
    {
        if (external_access_name[i].ptr_tty != NULL)
        {
            send_msg_to_user_space(pnode, external_access_name[i].ptr_tty, count);
        }
    }
}

/*------------------------------------------------------------
  函数原型: void  handler_resolve_available_msg(PNDIS_NODE tmpnode, int ret)
  描述: handler_resolve_available_msg在读NDIS消息的完成例程中被调用，用于对读入
        的NDIS消息进行处理
  输入: PNDIS_NODE tmpnode 完成例程中要处理的消息结点
        int ret 实际读出的消息长度值
  输出: 对读入的NDIS消息进行解析操作
  返回值: 无
-------------------------------------------------------------*/
static void handler_resolve_available_msg(PNDIS_NODE tmpnode, int ret)
{
    int i;
    if (ret)
    {
		for(i = 0; i < NDIS_TTY_MINOR; i++)
		{
	        if (external_access_name[i].ptr_tty)
	        {
				 //printk("%s: send_msg_to_user_space [%d]%s\r\n",__FUNCTION__,i,external_access_name[i].ptr_tty->name); 
	            send_msg_to_user_space(tmpnode, external_access_name[i].ptr_tty, ret);
	        }
		}
        return_node_to_free(tmpnode);
    }
    else
    {
        return_node_to_free(tmpnode);
    }
}

/*------------------------------------------------------------
  函数原型: void    handle_response_available(void)
  描述: handle_response_available是进行NDIS消息解析处理的关键函数，
        在中断通道返馈有命令通告后，handle_response_available发
        起从USB控制通道读入NDIS消息返馈。在成功读取了消息返馈以
        后，根据消息返馈中的Client Id，进行判别消息该进行那种处
        理方式。目前，本工程中采用的算法时：广播消息转入广播处理；
        Client Id为0则转入CTL类消息处理；是WDS Client Id的把消息
        体送入WDS用户进程空间；是其他用户进程的Client Id则把消息
        送入其他用户进程空间。
  输入: 无。
  输出: 根据不同的Client Id转入不同的处理中。
  返回值: 无。
-------------------------------------------------------------*/
static void handle_response_available(void)
{
    PNDIS_NODE pNode = NULL;

/* <DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 BEGIN */
    int i = 0;
	//printk("%s-handle %d response msg !!\n", __FUNCTION__, i);
	do
    {
	    pNode = get_free_node();
	    if (NULL == pNode)
	    {
	        printk("%s-can not get free node, no mem here !!\n", __FUNCTION__);
	        return;
	    }

/* < DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 BEGIN */
	    if (read_control_ndis_msg(pNode, MSG_COLUMN) < MSG_COLUMN)// all read out
/* DTS2013011706280 improve NDIS dial up slowly z00185914 20130119 END> */
	    {
			break;
	    }

        i++;
	} while(1);

    if (i > 1)printk("%s-handle %d response msg !!\n", __FUNCTION__, i);
	NDIS_DEBUG_MSG("%s-handle %d response msg !!\n", __FUNCTION__, i);
/* DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 END> */
}

/* DTS2010112205139.NDISTTY: mayang 2010-11-20 ADD END> */

/*****************************************************************************
 函 数 名  : clean_dummy_msg
 功能描述  : 清理控制端点中无效的消息
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年6月7日
    作    者   : w00211169
    修改内容   : 新生成函数

*****************************************************************************/
static void clean_dummy_msg(void)
{
    PNDIS_NODE pNode = NULL;
	int i = 0;
	
	for (i = 0; i <20; i++)
	{
	    pNode = get_free_node();
	    if (NULL == pNode)
	    {
	        printk("%s-can not get free node, no mem here !!\n", __FUNCTION__);
	        return;
	    }

	    if (read_control_ndis_msg(pNode, MSG_COLUMN) <= 0)
	    {
			break;
	    }	
	}
	
	NDIS_DEBUG_MSG("%s-clean %d dummy msg !!\n", __FUNCTION__, i);
}

/*------------------------------------------------------------
  函数原型: void    init_dev_message_node_and_area(void)
  描述: init_dev_message_node_and_area函数完成，对于NDISTTY模块中
        所要用到的，内部信号数组，消息缓冲区，消息结点，用户进程
        匹配数组的初始化。并且初始化完成后，每个消息结点的指针成
        员指向SDU起始位置，消息结点的行成员与消息缓冲区的行坐标
        一一对应，所有的消息结点初始完后都被链入到空闲结点链表中。
  输入: 无
  输出: 完成初始阶段的初始化工作和链表构造工作
  返回值: 无
-------------------------------------------------------------*/
static void init_dev_message_node_and_area(void)
{
    int i;
    NDIS_DEBUG_MSG("init_dev_message_node_and_area is running!\n");

    /*初始化中断轮询缓冲区*/
    memset((void *)interrupt_query_buf, 0, INTERRUPT_PACKET_SIZE * MSG_ROW);

    /*初始化用户进程匹配数组*/
    for (i = 0; i < NDIS_TTY_MINOR; ++i)
    {
        memset((void *)&external_access_name[i], 0, sizeof(tty_memo));
    }

    /*初始化消息缓冲区*/
	memset(message_area, 0, sizeof(*message_area));	

    /*初始化消息结点*/
    for (i = 0; i < MSG_ROW; ++i)
    {
        memset((void *) &message_node[i], 0, sizeof(NDIS_NODE));
    }

}

/*------------------------------------------------------------
  函数原型: PNDIS_NODE   get_free_node()
  描述: 任何需要读取和发送NDIS消息的操作，都必须先获取一个消息结
        点，通过对于消息结点所指向的消息缓冲区，进行NDIS消息的装
        配、或者是对消息进行解析。
  输入: 无
  输出: 无
  返回值: 成功获取，则返回成功获取的消息结点的指针
-------------------------------------------------------------*/
static PNDIS_NODE get_free_node()
{
    PNDIS_NODE tmpnode = NULL;
    unsigned long flags;
    struct list_head * tmplist = &free_list;

    spin_lock_irqsave(&lock_for_list, flags);

    if (!list_empty(tmplist))
    {
        tmpnode = (PNDIS_NODE)tmplist->next;//lint !e40 !e115
        list_del_init((plisthead) & tmpnode->ptr_ndis_node);
    }

    spin_unlock_irqrestore(&lock_for_list, flags);

    return tmpnode;
}

/*------------------------------------------------------------
  函数原型: void return_node_to_free(PNDIS_NODE pnode)
  描述: return_read_node_to_free函数，把已经完成读消息处理的消息
        结点，首先从读链表中删除，然后对相应消息缓冲区清零，最后
        把消息结点归还到空闲链表中。
  输入: PNDIS_NODE pnode等待归还的消息结点。
  输出: 成功执行，结点归还到空闲链表中
  返回值: 无
-------------------------------------------------------------*/
static void return_node_to_free(PNDIS_NODE pnode)
{
    unsigned long flags;
    clean_node_and_area(pnode);
    spin_lock_irqsave(&lock_for_list, flags);
    list_add_tail(((plisthead) & pnode->ptr_ndis_node), &free_list);
    spin_unlock_irqrestore(&lock_for_list, flags);
}


/*------------------------------------------------------------
  函数原型: void    clean_node_and_area(PNDIS_NODE pnode)
  描述: clean_node_and_area函数对于，完成了申请读、写的消息结点
        对于相对应的消息缓冲区进行清零操作。操作的主要步骤为：1、
        初始化每一个NDIS_NODE中的list_head；2、对消息缓冲区按行坐
        标清零。
  输入: PNDIS_NODE pnode等待执行清零操作消息结点。
  输出: 对于相对应的消息缓冲区进行清零。
  返回值: 无
-------------------------------------------------------------*/
static void    clean_node_and_area(PNDIS_NODE pnode)
{
    INIT_LIST_HEAD(&pnode->ptr_ndis_node);
    memset((void * )MEG_ROW(pnode->row_num), 0, MSG_COLUMN);
}

static const struct tty_operations ndis_tty_ops = {
	.open       = ndis_tty_open,
	.close      = ndis_tty_close,
	.write      = ndis_tty_write,
	.write_room = ndis_tty_write_room,
	.ioctl      = ndis_tty_ioctl,
	.throttle   = ndis_tty_throttle,
	.unthrottle = ndis_tty_unthrottle,
};

/*------------------------------------------------------------
  函数原型: static int tty_module_init(void)
  描述: tty_module_init是NDISTTY模块的，模块初始化函数，是模块装
        入的入口点。
  输入: 无
  输出: 无
  返回值: 无
-------------------------------------------------------------*/
static int tty_module_init(void)
{
    int result = 0;
    //printk("Enter - %s\n", __FUNCTION__);
    ndis_tty_driver = alloc_tty_driver(NDIS_TTY_MINOR);
	if (!ndis_tty_driver)
    {
        printk("%s - alloc_tty_driver failed!\n", __FUNCTION__);
	    return -ENOMEM;
    }

    ndis_tty_driver->owner = THIS_MODULE;
	ndis_tty_driver->driver_name = "QMITTY";
	ndis_tty_driver->name = 	"qmitty";
	ndis_tty_driver->major = NDIS_TTY_MAJOR;
	ndis_tty_driver->minor_start = 0;
	ndis_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	ndis_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	ndis_tty_driver->flags = TTY_DRIVER_REAL_RAW /*| TTY_DRIVER_DYNAMIC_DEV*/;
	ndis_tty_driver->init_termios = tty_std_termios;
    ndis_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    ndis_tty_driver->init_termios.c_lflag = ISIG;
    ndis_tty_driver->init_termios.c_oflag = 0;
    tty_set_operations(ndis_tty_driver, &ndis_tty_ops);
    //printk("%s - tty struct init successful !\n", __FUNCTION__);
    
    result = tty_register_driver(ndis_tty_driver);
    if (result)
    {
        printk(KERN_ALERT "%s - tty_register_driver failed!\n", __FUNCTION__);
        return (-1);
    }

    printk(KERN_ALERT "Registered the ndis_tty_driver !!\n");
    return 0;
}

/*------------------------------------------------------------
  函数原型: static void tty_module_exit(void)
  描述: tty_module_exit是NDISTTY模块的，模块的退出函数
  输入: 无
  输出: 无
  返回值: 无
-------------------------------------------------------------*/
static void tty_module_exit(void)
{
    //printk(KERN_ALERT "NDIS_TTY module is exiting ");
    /*  exit_tty(); */
    printk(KERN_ALERT "NDIS_TTY module exit completely\n");
    tty_unregister_driver(ndis_tty_driver);
    /* <DTS2010121503955.NDISTTY: gKF34687 2011-01-19 ADD BEGIN */
    /* free tty driver */
    put_tty_driver(ndis_tty_driver);

	if (NULL != message_area)
	{
		kfree(message_area);
	}	
    /* DTS2010121503955.NDISTTY: gKF34687 2011-01-19 ADD END> */
}

MODULE_LICENSE("GPL");
module_init(tty_module_init);
module_exit(tty_module_exit);
/* DTS2013010905391 add ndistty driver z00185914 20130112 END> */


