/* <DTS2013010905391 add ndistty driver z00185914 20130112 BEGIN */
/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2013, ��Ϊ�������޹�˾.
  �ļ���: ndistty.h
  ����: zhangjiqiang 00185914
  �汾: 001
  ��������: 2013-1-8
  �������: 2013-1-8
  ��������: ndistty.hͷ�ļ��У���Ҫ������ʵ��TTY����ģ����Ҫ�õ��ĺ궨�壬
            �磺ioctlҪʹ�õ������룻���豸�ļ��ţ���Ϣ���������������
            С������Ĵ�С�����Դ�������ʱ��������������־������Ϣ��
            ��ṹ���壻�ڲ�ƥ��ṹ����

  ��Ҫ�����б�:

  �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    zhangjiqiang   2013-1-8   001    ��NDISTTYΪ�����޸�ΪAT��balong NDIS����

  ��ע:
 ************************************************************************/
/*===============================================================
�޸ļ�¼:
���ⵥ��             �޸�ʱ��    �޸���         �޸�����
DTS2013011408161 20130125    z00185914     ��hw_cdc_driver�е��жϴ����Ƶ�ndistty�У�����hw_cdc_driver.h�ļ�
================================================================*/
#ifndef _NDISTTY_H
#define _NDISTTY_H

#define NDIS_TTY_MAJOR 234   /*NDISTTY����ģ��ʹ�õ����豸��*/
#define NDIS_TTY_MINOR 4     /*NDISTTY����ģ��֧�ֵ��û������������ڹ��ɸ��豸��*/

#define NDIS_TTY_ROOM_SIZE 1024  /*�̶�����һ����Ϣ������һά����ĳ���*/

typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
/*NDISTTYģ��ѡ�õ�Magic����*/
#define NDIS_TTY_IOC_MAGIC 'x'

/*����WDS���̲�ѯSTART_NETWORK_INTERFACEҵ��ID*/
#define NDIS_TTY_IOCQNUM _IO(NDIS_TTY_IOC_MAGIC, 3)

/*���������û����̲�ѯ��õ��ļ��Ŷ�Ӧ���豸ƥ���������Ƿ��Ѿ����Client ID*/
#define NDIS_TTY_IOCQNUM_CID _IOWR(NDIS_TTY_IOC_MAGIC, 4, NDIS_IOCTL_PARAMS_ST)  /* <DTS2010121503955.NDISTTY: gKF34687 2011-01-19 MOD>*/

#define NDIS_TTY_IOCQNUM_TEST02 _IO(NDIS_TTY_IOC_MAGIC, 5)
#define NDIS_TTY_IOCQNUM_TEST03 _IO(NDIS_TTY_IOC_MAGIC, 6)
#define NDIS_TTY_IOCQNUM_TEST04 _IO(NDIS_TTY_IOC_MAGIC, 7)
#define NDIS_TTY_IOCQNUM_TEST05 _IO(NDIS_TTY_IOC_MAGIC, 8)
#define NDIS_TTY_IOCQNUM_TEST06 _IO(NDIS_TTY_IOC_MAGIC, 9)
#define NDIS_TTY_IOCQNUM_TEST07 _IO(NDIS_TTY_IOC_MAGIC, 10)
#define NDIS_TTY_IOCQNUM_TEST08 _IO(NDIS_TTY_IOC_MAGIC, 11)

#define MSG_ROW 6   /*��Ϣ��������ά������еĴ�С*/
#define MSG_COLUMN 1024    /*��Ϣ��������ά������еĴ�С*/
#define INTERRUPT_PACKET_SIZE 16  /*���ж�ͨ���Ļ������Ĵ�С*/

#define TIMER_INTERVAL 1 /*(1 / 10)*/   /*��ʱ����������Ƶ��*/
#define WAIT_INTERVALTIME (50)    /*�ȴ�ʱ����50����*/
#define RETRY_TIMES 3  /*��ʼ�׶��շ���Ϣ�����Դ���*/
#define CHECK_INTERVAL 2      /*����״̬��鶨ʱ�����2��*/
#define PENDING_TIME 200 /* ���ҵȴ�ʱ��*/
#define EXIT_PENDING_TIME 500   /*  �˳��ȴ�ʱ��*/

#define  USER_NODE_TYPE (0x0800)        /*�Զ��������ͺ궨��*/
#define  FREE_NODE (USER_NODE_TYPE + 1)
#define  QOS_NODE (USER_NODE_TYPE + 2)

#define  USER_NODE_LIST_TYPE (0x08F0)            /*�Զ�����������ͺ궨��*/
#define  READ_LIST (USER_NODE_LIST_TYPE + 1)
#define  SENT_LIST (USER_NODE_LIST_TYPE + 2)
#define  QOS_LIST (USER_NODE_LIST_TYPE + 3)

typedef  unsigned int       BOOL;
typedef  struct list_head * plisthead;
#define   TRUE 1
#define   FALSE 0

#define CDC_NOTIFICATION_NETWORK_CONNECTION 0x00    /*��ͨ�������������ָʾ�Ķ���*/
#define CDC_NOTIFICATION_RESPONSE_AVAILABLE 0x01    /*��ͨ�����������ͨ��ָʾ�Ķ���*/
#define CDC_NOTIFICATION_SERIAL_STATE 0x20    /*��ͨ����Ĵ�����·״̬�Ķ���*/
#define CDC_NOTIFICATION_CONNECTION_SPD_CHG 0x2A    /*��ͨ������������ʸı�Ķ���*/

/* <DTS2013011408161 move urb request to ndistty z00185914 20130125 BEGIN */
typedef struct NdisIoctlParams
{
    unsigned char client_id;
}NDIS_IOCTL_PARAMS_ST;
/* DTS2013011408161 move urb request to ndistty z00185914 20130125 END> */


/* tty_memo�ṹ������NDISTTY�ڲ�����¼�ϲ��û����̴������Ĺ�����Ϣ��
 *  ͨ����¼�µ�tty_structָ�룬����ȷ��������NDIS��Ϣ������������һ��
 *  �û����̵Ŀռ䡣
 */
struct _tty_memo
{
    unsigned char client_id;    /*���ڼ�¼�û����̻�ȡ��Client ID*/
    unsigned char type;     /*���ڼ�¼�û���������ķ�������*/
    /* <AU3D00183.NDISTTY: houxiaohe 2008-7-25 MODIFY BEGIN */
    unsigned char transaction_id;   /*���ڼ�¼�ض���ҵ��ID*/
    /* AU3D00183.NDISTTY: houxiaohe 2008-7-25 MODIFY END> */
    struct tty_struct *   ptr_tty;      /*���ڼ�¼�û����̴�tty����ʱʹ�õ�tty_structָ��*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _tty_memo tty_memo;

/* NDIS_NODE�ṹ��Ϊ�˱��ڹ���NDISTTYģ���շ�NDIS��Ϣ�������,��Ϣ��������������
 *  �ǿ��ǵ��շ���Ϣ���Ƶ����ÿ�ζ����ڴ��������ͷ�ͬ�����п�����
 */

struct _NDIS_NODE
{
    struct list_head    ptr_ndis_node;   /*˫��ָ��������Ϣ�����������*/
    unsigned char*       p_msg_array;    /*ָ��SDU�ĵ�ַ*/
    unsigned int         row_num;        /*���Ӧ��Ϣ��������������*/
    unsigned long        trans_start;    /*��¼���ύ��urbͣ����ʱ��*/
    struct urb*         purb;           /*��ײ����ͨ��ò�ύ��urb*/
    struct usb_ctrlrequest dr;          /*��ײ����ͨ���ύ��Setup��*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _NDIS_NODE NDIS_NODE, * PNDIS_NODE;


/*�궨�����ڴ�ӡ��ѯ�ж϶�ʱ��ǰ100�εĲ�ѯ���ָʾ*/
#define PRT_TIMER_INFO

/*�궨�������ڳ�ʼ�������в��Լ�飬�ź����顢��Ϣ��㡢��Ϣ����������Ϣ����ʼ������Ƿ�ɹ�����*/
#define TEST_INIT_NODE_AND_AREA

/* ndis_tty_open�в���0�Ŷ˵�Ļ����շ����� */
//#define NDIS_TTY_EP0_TEST

/* �����ж϶˵㹦��ʱ������Ҫ�ѿ������� */
#define NDIS_TTY_INTR_SUPPORT

/* �����ж϶˵㹦��ʱ������Ҫ�ѿ������� */
#define NDIS_TTY_INTR_TEST

/* ʹ��usb_control_msg��������NDIS��Ϣ */
#define NDIS_TTY_USB_CNTL_MSG

/* ��ӡndis��Ϣ */
//#define NDIS_MSG_PRINT_DBG

#define CLASS_HOST_TO_INTERFACE     0x21    /*USB�豸��������*/
#define CLASS_INTERFACE_TO_HOST     0XA1    /*USB�豸��������*/
#define CDC_SEND_ENCAPSULATED_CMD   0x00    /*USB����������*/
#define CDC_GET_ENCAPSULATED_RSP    0x01    /*USB����������*/
#define CDC_SET_LINE_CODING          0x20    /*USB��·��������*/
#define CDC_GET_LINE_CODING          0x21    /*USB��·��������*/
#define CDC_SET_CONTROL_LINE_STATE  0x22    /*USB��·��������*/
#define CDC_SEND_BREAK                0x23    /*USB��·��������*/
#define CDC_SET_COMM_FEATURE         0x02    /*USB��·��������*/

#define ERR_COMMAND_NOT_EXIST        0x3F0   /*NDISTTYģ����ΪIOCTL���ö���Ĵ����룬�����벻����*/
#define ERR_COMMAND_NOT_IMPLEMENT    0x3F1  /*NDISTTYģ����ΪIOCTL���ö���Ĵ����룬������û�б�ʵ��*/
struct _USB_NOTIFICATION_STATUS
{
    UCHAR  bmRequestType;
    UCHAR  bNotification;   /*�Զ�ȡ�ж�ͨ���������Ҫ���ж�ֵ*/
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
