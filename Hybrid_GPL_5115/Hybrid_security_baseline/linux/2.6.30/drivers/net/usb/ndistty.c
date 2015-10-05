/* <DTS2013010905391 add ndistty driver z00185914 20130112 BEGIN */
/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2008, ��Ϊ�������޹�˾.
  �ļ���: NDISTTY.c
  ����: houxiaohe 00118013
  �汾: 001
  ��������: 2008-5-21
  �������: 2008-5-25
  ��������: NDISTTYģ����ʵ��BALONG Rm Net����Ҫ��ɲ��֣�ģ���ڳɹ����ز�����
            ���Ժ�ע����һ���豸�ļ���㡣�û�����ͨ�����豸�ļ������
            ���NDIS��Ϣ�Ķ�д����ɺ�����ģ���NDIS��Ϣ������ͬʱ��Ŀǰ��ʵ
            ����У�NDISTTYģ��Ϊhw_cdc_driverģ�鹹������ͺͽ���QOS��Ϣ�ĵ��ýӿ�
            ��NDISTTYģ���hw_cdc_driverģ�������ɶ������ݰ����͵�QOS���ء�Ϊ����
            Ч������Դ��ģ���ڲ����������һ����Ϣ��������

  ��Ҫ�����б�: trigger_tty, exit_tty, compose_internal_request, send_msg_to_user_space,
                interrupt_timer_proc, handle_reponse_available, get_node, insert_node_to_list,
                remove_list_node, return_node_to_free, ndis_tty_open,
                ndis_tty_close, ndis_tty_write,

                write_control_ndis_msg, write_ndis_completion, read_control_ndis_msg,
                read_ndis_completion, handler_resolve_available_msg,

  �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    zhangjiqiang   2013-1-8   001    ��QMITTYΪ�����޸�ΪAT��balong NDIS����

  ��ע:
 ************************************************************************/
/*===============================================================
�޸ļ�¼:
���ⵥ��             �޸�ʱ��    �޸���         �޸�����
DTS2013011408161 20130125    z00185914     ��hw_cdc_driver�е��жϴ����Ƶ�ndistty�У�����hw_cdc_driver.h�ļ�
DTS2013021703454 2013/02/17	z00185914     	�����š��Ż�ndis���Ŵ��� 
DTS2013020703621 20130207    q00186737     [B880]histudio͸��log
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

/********************* �ⲿ���� - ������usbnetģ���� *********************/

/* usbnet��probe������̽�鵽��RmNetָ����ж϶˵�Ķ˵�������*/
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

/********************* �ⲿ�ӿں��� - �ṩ��usbnetģ��ʹ�� *********************/



/********************* ��ģ��ʹ�õ�ȫ�ֱ��� *********************/
/*  USB�����Ƿ���������ʶ�� */
volatile unsigned int connect_stat = 0;

/*  �ж϶˵�urb */
static struct urb * p_int_urb = NULL;

static struct work_struct intr_work;

/*  ��¼��Ӧ����tty�ṹ��Ϣ */
static tty_memo external_access_name[NDIS_TTY_MINOR];

/* NDIS��Ϣ�ڵ� */
NDIS_NODE message_node[MSG_ROW];

/* NDIS��Ϣ������ */
unsigned char (*message_area)[MSG_ROW][MSG_COLUMN];

#define MEG_ROW(row) ((unsigned char *)message_area + (row * MSG_COLUMN))

/* NDIS��Ϣ��ȡ�ڵ� */
PNDIS_NODE pndis_read_node;
/* NDIS��Ϣд��ڵ� */
PNDIS_NODE pndis_write_node;


/*���ж�ͨ���Ļ������Ĵ�С*/
unsigned char interrupt_query_buf[INTERRUPT_PACKET_SIZE * MSG_ROW];




/* �ж���ѯ�Ƿ������� */
static unsigned char intr_setup_flag = 0;

/* ���ڹ����ڴ���Ϣ����������Ϣ�������� */
struct list_head free_list;

/* ��������������������������⾺̬ */
static spinlock_t lock_for_list = SPIN_LOCK_UNLOCKED;

/* mayang 2011-9-22 ���sem��֤ndis�״�openʱ��ʼ��������� */
static DECLARE_MUTEX(sema);

/********************* ��ģ��ʹ�õĺ������� *********************/

/* usb�ײ�NDIS��Ϣ�Ķ�д�������������*/
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

/* NDISTTY����ģ����ں˽ӿں��� */
static int ndis_tty_open(struct tty_struct * tty, struct file * filp);
static void ndis_tty_close(struct tty_struct * tty, struct file * filp);
static int ndis_tty_write(struct tty_struct * tty, const unsigned char * buf, int count);
static int ndis_tty_write_room(struct tty_struct * tty);
static int ndis_tty_ioctl(struct tty_struct * tty, struct file * filp, unsigned int cmd, unsigned long arg);
static void	ndis_tty_throttle(struct tty_struct * tty);
static void ndis_tty_unthrottle(struct tty_struct * tty);

/*��ӡǰ100���ж϶˵���ѯ����Ĵ�ӡ��*/
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

/* ��������unsigned long ֵ�Ĳ�*/
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
  ����ԭ��: void trigger_tty()
  ����: ����NDISTTY״̬���Ĵ���������,��usbnet��probe̽�⺯���ڲ�ѯ
        ��Rm Net�ӿں󣬲��ɹ��������ж�ͨ������������ͨ���Ժ���ã�
        trigger_tty�����������ڲ���ʼ��ʼ������Ҫ�õ��ڲ��������ṹ��
        �ڲ�ƥ�����ݽṹ�����ý���NDIS��Ϣ�ӿڵ��ڲ����̣����ú�����
        ѯ�����Ƿ�֧��QOS�����ڽ����ɹ��Ժ�ע���豸�ļ���㡣
  ����: ��
  ���: �ɹ�������Ϣ�ӿں󣬻�ע���豸�ļ���㣻����ע�ᣬ����ʧ��
  ����ֵ: ��
-------------------------------------------------------------*/
int trigger_tty(void)
{
	int i = 0;
    p_int_urb = NULL;
    connect_stat = 1;

    NDIS_DEBUG_MSG("trigger_tty is running!\n");
    NDIS_DEBUG_MSG("bEpAddress=0x%0x, bInterval=0x%0x\n", ep_int_rmnet->bEndpointAddress, ep_int_rmnet->bInterval);

	/* ������Ϣ������ */
    message_area = kmalloc(sizeof(*message_area), GFP_KERNEL);
    if (NULL == message_area)
    {
    	printk("%s: Kmalloc the buffer failed.\n", __FUNCTION__);
    	return -ENOMEM;
    }
    NDIS_DEBUG_MSG("alloc area ok [%d]\n", sizeof(*message_area));

    /* ��ʼ���ڲ���������Ϣ�ڵ� */
    init_dev_message_node_and_area();

    /*��ʼ�����õ�����ͷ*/
    INIT_LIST_HEAD(&free_list);

    /*ÿһ����Ϣ���Ķ�д������ָ��ָ����Ӧ��������ָ��������*
     *SDU����ʼλ�ã���Ϣ���������Ա��Ϊ��Ӧ�������ꡣ�ڳ�ʼ��*
    �����е���Ϣ��㶼���������Ϣ�����С�*/
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

    /* ��ʼ����д�ڵ� */
    pndis_write_node = (PNDIS_NODE)&(message_node[0]);
    pndis_read_node = (PNDIS_NODE)&(message_node[1]);
    
    NDIS_DEBUG_MSG("trigger_tty ok\n");
    return 0;
}
EXPORT_SYMBOL_GPL(trigger_tty);

/*------------------------------------------------------------
  ����ԭ��: void exit_tty()
  ����: �˳�NDISTTY״̬���ĺ�����Ŀǰ�����˳�ʱ���Ŀ�����Ҫ������
        ��ģ�鸴λ��USB���ӱ���λ��Usbnet����disconnect��������
        usbnet��disconnect�����е��� exit_tty ��ʹNDISTTY״̬����
        ����
  ����: ��
  ���: �豸�ļ���㱻ע��
  ����ֵ: ��
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
  ����ԭ��: int ndis_tty_open(struct tty_struct * tty, struct file * filp)
  ����: ndis_tty_open������tty�����Ľӿں��������û�����ʹ��open������
        ��Ӧ���豸�ļ����ʱ��tty���Ľ������ndis_tty_open��������ɴ�
        �Ĳ�����Ϊ��֧�ֶ���û����̣��ڴ򿪹�����Ҫ���û�����ʹ�õĸ�
        �豸�Ž����б𣬶��ڲ���WDS���̵��û����̣�NDISTTYģ���ڲ������
        1����ȡWDS Client ID��NDIS��Ϣ��������ģ�顣�����ڵõ���ӦResponse
        �Ժ󣬼�¼���豸ƥ������֮�С�
        WDSӦ�ý����ڴ�NDISTTYģ�������ڲ�״̬����־ֵû����λ������
        ״̬��������
  ����: tty_struct * tty ָ���ں�TTY�ṹ��ָ��
        file * filpָ���ں��ļ��ṹ��ָ��
  ���: ��
  ����ֵ: int ret ͨ��������0��ʾ�򿪲����Ѿ���ɡ�
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

        /* ������֤��һ�γ�ʼ��������� */
        down_interruptible(&sema);
		if (1 == intr_setup_flag)
		{
			if((NULL != p_int_urb) && (-EINPROGRESS == p_int_urb->status))
            {
                usb_kill_urb(p_int_urb);
            }
		}
			/* �����ж϶˵���ѯ */
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
        /* ��ʼ��ndis��Ϣ����ȡndis version����Ӧ�ϲ�tty��client id
           ��ǰֻ֧��/dev/ndistty0, �����Ȳ�֧��
         */
        up(&sema);
        /* ��¼�ϲ����client��Ӧ��tty�ṹ */
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
  ����ԭ��: void ndis_tty_close(struct tty_struct * tty, struct file * filp)
  ����: ndis_tty_close������tty�����Ľӿں��������û�����ʹ��close�����ر�
        ��Ӧ���豸�ļ����ʱ��tty���Ľ������ndis_tty_close��������ɹر�
        �Ĳ�����Ϊ��֧�ֶ���û����̣��ڹرչ�����Ҫ���û�����ʹ�õĸ�
        �豸�Ž����б𣬶��ڲ���WDS���̵��û����̣�NDISTTYģ���ڲ������
        1���ͷ�WDS Client ID��NDIS��Ϣ��������ģ�顣�����ڵõ���ӦResponse
        �Ժ�����豸ƥ������֮����Ӧ�ļ�¼�Ĭ��WDS���̲����˳������
        WDS�����˳���NDISTTY����ģ���˳���
  ����: tty_struct * tty ָ���ں�TTY�ṹ��ָ��
        file * filpָ���ں��ļ��ṹ��ָ��
  ���: ��
  ����ֵ: ��
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
  ����ԭ��: int ndis_tty_write(struct tty_struct * tty, int from_user, const unsigned char * buf, int count)
  ����: ndis_tty_write������ndistty�����������tty����������Ҫ�Ĳ���
        ����֮һ�����û�������open�������豸�ļ����ʱ��tty����
        ���������ע���tty�����ķ������е� ndis_tty_write ��������
        ��ɰ��û��ռ�����ݴ����ںˡ���ndis_tty_write�л���û�����
        ����Ϣ��װ��Ϊ��һ��NDIS��Ϣ��Ȼ����USB�ײ㷢�͡�
  ����: struct tty_struct * tty  ϵͳ�����ָ��tty_struct��ָ��
        int  from_user  ָʾ���������û��ռ仹�����ں˿ռ䡣
        const unsigned char * buf ָ���û��ռ���������ݻ�������
        int  count  Ҫ����������ֽ�����
  ���: ��
  ����ֵ: ��USB�ײ�ɹ�������ֽ�����ȥNDIS��Ϣͷ�ĳ��ȡ�
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
  ����ԭ��: int ndis_tty_write_room(struct tty_struct * tty)
  ����: ndis_tty_write_room��tty�������ĵ��õģ���tty����ִ��write
       ����ʱ��tty�������Ļ����ndis_tty_write_room��������ѯ����
       �������Ĵ�С
  ����: ϵͳ�����ָ��tty_struct��ָ��
  ���: �ޡ�
  ����ֵ: �ڱ������з�����һ���̶��Ĵ�С��
-------------------------------------------------------------*/
static int ndis_tty_write_room(struct tty_struct * tty)
{
    return NDIS_TTY_ROOM_SIZE;
}

/*------------------------------------------------------------
  ����ԭ��: void ndis_tty_throttle(struct tty_struct * tty)
  ����: ndis_tty_throttle���ڵ���������ʱ��֪ͨ�豸��Ҫ��������
  ����: ϵͳ�����ָ��tty_struct��ָ�롣
  ���: �ޡ�
  ����ֵ: �ޡ�
-------------------------------------------------------------*/
static void ndis_tty_throttle(struct tty_struct * tty)
{
    return;
}

/*------------------------------------------------------------
  ����ԭ��: void ndis_tty_unthrottle(struct tty_struct * tty)
  ����: ndis_tty_unthrottle���ڵ����������������֪ͨ�豸��������
  ����: ϵͳ�����ָ��tty_struct��ָ�롣
  ���: �ޡ�
  ����ֵ: ��
-------------------------------------------------------------*/
static void ndis_tty_unthrottle(struct tty_struct * tty)
{
    return;
}

/*------------------------------------------------------------
  ����ԭ��: int ndis_tty_ioctl(struct tty_struct * tty, struct file * filp, unsigned int cmd, unsigned long arg)
  ����: ndis_tty_ioctl����Ҫ���û����̵Ŀ��Ʒ����ӿڣ����û�����
        ������Ӧ��tty�豸�ļ����ʱ������ʹ��ioctl��������tty
        ����ִ��һЩ�ض��Ĳ���������ִ��ʲô�����ɹ��������֡�
  ����: struct tty_struct * tty  ϵͳ�����ָ��tty_struct��ָ��
        struct file * filp   ϵͳά�����ڲ����ļ�ָ��
        unsigned int cmd  ����Ĺ�����
        unsigned long arg  һ����һ�������ָ��
  ���: �ɹ���������
  ����ֵ: ִ���ض������Ľ��ֵ��
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
        /*�û������ڴ�NDISTTY�豸�ļ�����Ժ󣬶�Ҫ���� ioctl(,NDIS_TTY_IOCQNUM_CID)����ѯ�Ƿ��ѻ��Client ID*/
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
  ����ԭ��: int write_control_ndis_msg(PNDIS_NODE pnode, int count)
  ����: write_control_ndis_msg��ͨ���ײ��usb��Զ˵�����ģ�鷢��NDIS��Ϣ�ĺ�����
        NDIS��Ϣͨ��0�˵㷢�͡�
  ����: PNDIS_NODE pnode �ǵ����߻�õĿ��е���Ϣ��㣬
        int count ��Ҫ���͵�SDU����
  ���: urb�ύ�ɹ����������������ڵײ�����Ŷ�
  ����ֵ: ���ͳɹ����ط�����Ϣ�ĳ��ȣ�ʧ����ѽ�������ͬʱ����usb���صĴ�����
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
  ����ԭ��: void write_ndis_completion(struct urb * purb)
  ����: write_ndis_completion��write_control_ndis_msg��������̣����д���
        ���ύ��urb�ɹ�ִ�����Ժ�ĺ���������
  ����: struct urb * purb�ύ�˵�urb
  ���: ��ʹ�õ���Ϣ�������ӣ��ͷ�urbʹ�õ��ڴ�
  ����ֵ: ��
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
  ����ԭ��: int read_control_ndis_msg(PNDIS_NODE pnode, int count)
  ����: read_control_ndis_msg�Ǵ�usb�ײ��ȡ����ģ�鷢�͵�NDIS��Ϣ�ĺ�����
  ����: PNDIS_NODE pnode ���÷���õ���Ϣ���
        int count Ҫ���͵���Ϣ����
  ���: �ɹ��ύurb,�����������ڵײ����urb�Ŷ�
  ����ֵ: �ɹ�ִ�з��ط����˵���Ϣ����ֵ��ʧ�ܰ�ʹ�õĽ�������ͬʱ����usb���صĴ�����
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
  ����ԭ��: void read_ndis_completion(struct urb * purb)
  ����: read_ndis_completion��read_control_ndis_msg��������̣����
        ���ύ��urb�����Ժ�ĺ�������
  ����: struct urb * purb �ύ��urb��ָ��
  ���: ����Ϣ������Ҫ������Ӧ�Ĵ���ͬʱҪ�ͷ�urbʹ���ڴ�
  ����ֵ: ��
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
  ����ԭ��: static int start_intr_ep_query()
  ����: �����ж϶˵���ѯ
  ����: ��
  ���: ��
  ����ֵ: �ɹ�����0�����򷵻�errno
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
  ����ԭ��: void intr_completion(struct urb * purb)
  ����: intr_completion��read_interrupt_ndis_msg��������̣�
  ����: �ύ��urbָ��
  ���: ��Ч������ͨ��
  ����ֵ: ��
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
		/* <DTS2013021703454 �Ż�ndis���Ŵ��� z00185914 20130217 DELETE 1 ROW */
        if (status != 0)
        {
            printk("%s - resubmit intr urb failed, status=%d\n", __FUNCTION__, status);
        }
    }
    
readtimer:
    VIEW_TIMER("CDC message end");
}


/*------------------------------------------------------------
  ����ԭ��: void    send_msg_to_user_space(PNDIS_NODE pnode, struct tty_struct * tty, int count)
  ����: send_msg_to_user_space�������ڣ�����USB�ײ�ɹ�����NDIS��Ϣ
        ʱ�����Ҿ����б���WDS���͵���Ϣ����Ҫ����WDS���̻������û�
        ���̽��д�������������tty���ĵĻ��������������ˢ�º���
        ��WDS��Ϣ�����û����̿ռ䡣
  ����: PNDIS_NODE pnode ����WDS��Ϣ����Ϣ���.
        struct tty_struct * tty ��¼�µĶ�Ӧ���û����̴򿪵�ָ���
        tty_struct��ָ��
        int count Ҫˢ�µ��û����̿ռ���ֽ�����
  ���: ������û��ռ��WDS��Ϣ��
  ����ֵ: �ޡ�
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
  ����ԭ��: void    broadcast_msg(PNDIS_NODE pnode, int count)
  ����: broadcast_msg �Ƕ����Ǵ�USB�ײ�������Ҫ�㲥�������Ϣ��
       ������㷨���£��������ÿһ�����ڵ��ϲ�WDS�û����̣�ִ��
       send_msg_to_user_space������Ϣ�����û��ռ䡣��󣬰���Ϣ
       �����뵽QoS_used_list�����У�����usbnet���д���
  ����: PNDIS_NODE pnode ��Ҫ���д������Ϣ��㡣
        int count ��Ҫˢ���û��ռ����Ϣ�ĳ��ȡ�
  ���: ���д��ڵ��û����̺�usbnet������յ��㲥��Ϣ��
  ����ֵ: �ޡ�
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
  ����ԭ��: void  handler_resolve_available_msg(PNDIS_NODE tmpnode, int ret)
  ����: handler_resolve_available_msg�ڶ�NDIS��Ϣ����������б����ã����ڶԶ���
        ��NDIS��Ϣ���д���
  ����: PNDIS_NODE tmpnode ���������Ҫ�������Ϣ���
        int ret ʵ�ʶ�������Ϣ����ֵ
  ���: �Զ����NDIS��Ϣ���н�������
  ����ֵ: ��
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
  ����ԭ��: void    handle_response_available(void)
  ����: handle_response_available�ǽ���NDIS��Ϣ��������Ĺؼ�������
        ���ж�ͨ������������ͨ���handle_response_available��
        ���USB����ͨ������NDIS��Ϣ�������ڳɹ���ȡ����Ϣ������
        �󣬸�����Ϣ�����е�Client Id�������б���Ϣ�ý������ִ�
        ��ʽ��Ŀǰ���������в��õ��㷨ʱ���㲥��Ϣת��㲥����
        Client IdΪ0��ת��CTL����Ϣ������WDS Client Id�İ���Ϣ
        ������WDS�û����̿ռ䣻�������û����̵�Client Id�����Ϣ
        ���������û����̿ռ䡣
  ����: �ޡ�
  ���: ���ݲ�ͬ��Client Idת�벻ͬ�Ĵ����С�
  ����ֵ: �ޡ�
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
 �� �� ��  : clean_dummy_msg
 ��������  : ������ƶ˵�����Ч����Ϣ
 �������  : void  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��6��7��
    ��    ��   : w00211169
    �޸�����   : �����ɺ���

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
  ����ԭ��: void    init_dev_message_node_and_area(void)
  ����: init_dev_message_node_and_area������ɣ�����NDISTTYģ����
        ��Ҫ�õ��ģ��ڲ��ź����飬��Ϣ����������Ϣ��㣬�û�����
        ƥ������ĳ�ʼ�������ҳ�ʼ����ɺ�ÿ����Ϣ����ָ���
        Աָ��SDU��ʼλ�ã���Ϣ�����г�Ա����Ϣ��������������
        һһ��Ӧ�����е���Ϣ����ʼ��󶼱����뵽���н�������С�
  ����: ��
  ���: ��ɳ�ʼ�׶εĳ�ʼ�������������칤��
  ����ֵ: ��
-------------------------------------------------------------*/
static void init_dev_message_node_and_area(void)
{
    int i;
    NDIS_DEBUG_MSG("init_dev_message_node_and_area is running!\n");

    /*��ʼ���ж���ѯ������*/
    memset((void *)interrupt_query_buf, 0, INTERRUPT_PACKET_SIZE * MSG_ROW);

    /*��ʼ���û�����ƥ������*/
    for (i = 0; i < NDIS_TTY_MINOR; ++i)
    {
        memset((void *)&external_access_name[i], 0, sizeof(tty_memo));
    }

    /*��ʼ����Ϣ������*/
	memset(message_area, 0, sizeof(*message_area));	

    /*��ʼ����Ϣ���*/
    for (i = 0; i < MSG_ROW; ++i)
    {
        memset((void *) &message_node[i], 0, sizeof(NDIS_NODE));
    }

}

/*------------------------------------------------------------
  ����ԭ��: PNDIS_NODE   get_free_node()
  ����: �κ���Ҫ��ȡ�ͷ���NDIS��Ϣ�Ĳ������������Ȼ�ȡһ����Ϣ��
        �㣬ͨ��������Ϣ�����ָ�����Ϣ������������NDIS��Ϣ��װ
        �䡢�����Ƕ���Ϣ���н�����
  ����: ��
  ���: ��
  ����ֵ: �ɹ���ȡ���򷵻سɹ���ȡ����Ϣ����ָ��
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
  ����ԭ��: void return_node_to_free(PNDIS_NODE pnode)
  ����: return_read_node_to_free���������Ѿ���ɶ���Ϣ�������Ϣ
        ��㣬���ȴӶ�������ɾ����Ȼ�����Ӧ��Ϣ���������㣬���
        ����Ϣ���黹�����������С�
  ����: PNDIS_NODE pnode�ȴ��黹����Ϣ��㡣
  ���: �ɹ�ִ�У����黹������������
  ����ֵ: ��
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
  ����ԭ��: void    clean_node_and_area(PNDIS_NODE pnode)
  ����: clean_node_and_area�������ڣ�������������д����Ϣ���
        �������Ӧ����Ϣ���������������������������Ҫ����Ϊ��1��
        ��ʼ��ÿһ��NDIS_NODE�е�list_head��2������Ϣ������������
        �����㡣
  ����: PNDIS_NODE pnode�ȴ�ִ�����������Ϣ��㡣
  ���: �������Ӧ����Ϣ�������������㡣
  ����ֵ: ��
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
  ����ԭ��: static int tty_module_init(void)
  ����: tty_module_init��NDISTTYģ��ģ�ģ���ʼ����������ģ��װ
        �����ڵ㡣
  ����: ��
  ���: ��
  ����ֵ: ��
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
  ����ԭ��: static void tty_module_exit(void)
  ����: tty_module_exit��NDISTTYģ��ģ�ģ����˳�����
  ����: ��
  ���: ��
  ����ֵ: ��
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


