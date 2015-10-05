/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2008, ��Ϊ�������޹�˾.
  �ļ���: qmitty.h
  ����: houxiaohe 00118013
  �汾: 001
  ��������: 2008-5-21
  �������: 2008-5-25
  ��������: qmitty.hͷ�ļ��У���Ҫ������ʵ��TTY����ģ����Ҫ�õ��ĺ궨�壬
            �磺ioctlҪʹ�õ������룻���豸�ļ��ţ���Ϣ���������������
            С������Ĵ�С�����Դ�������ʱ��������������־������Ϣ��
            ��ṹ���壻�ڲ�ƥ��ṹ����

  ��Ҫ�����б�:

  �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    houxiaohe   2008-06-28   001    �޸���ѯ�ж϶˵㶨ʱ�����Ϊ4����
    houxiaohe   2008-07-01   001    �޸���Ϣ���ṹ���purb��trans_start��Ա
                                    ������Ҷ�ʱ������궨��
    houxiaohe   2008-07-18  B000    �������ж������������ tasklet_scheule������
                                    ���п��ƶ˵��QMI��Ϣ��д����˵���ķ�������
                                    ���Ըı���Ϣ���Ľṹ��purb��dr����Ϊ��Ϣ���
                                    �ڲ��Ľṹ��Ա���ֱ�Ϊ struct urb��struct
                                    usb_ctrlrequest����ʵ����
    houxiaohe   2008-07-25  B000    �޸� tty_memo �ṹ�е� transaction_id Ϊ unsigned char

  ��ע:
 ************************************************************************/
#ifndef _QMITTY_H
#define _QMITTY_H

#define QMI_TTY_MAJOR 234   /*QMITTY����ģ��ʹ�õ����豸��*/
#define QMI_TTY_MINOR 4     /*QMITTY����ģ��֧�ֵ��û������������ڹ��ɸ��豸��*/

#define QMI_TTY_ROOM_SIZE 1024  /*�̶�����һ����Ϣ������һά����ĳ���*/

/*QMITTYģ��ѡ�õ�Magic����*/
#define QMI_TTY_IOC_MAGIC 'x'

/*����WDS���̲�ѯSTART_NETWORK_INTERFACEҵ��ID*/
#define QMI_TTY_IOCQNUM _IO(QMI_TTY_IOC_MAGIC, 3)

/*���������û����̲�ѯ��õ��ļ��Ŷ�Ӧ���豸ƥ���������Ƿ��Ѿ����Client ID*/
#define QMI_TTY_IOCQNUM_CID _IOWR(QMI_TTY_IOC_MAGIC, 4, QMI_IOCTL_PARAMS_ST)  /* <DTS2010121503955.QMITTY: gKF34687 2011-01-19 MOD>*/

#define QMI_TTY_IOCQNUM_TEST02 _IO(QMI_TTY_IOC_MAGIC, 5)
#define QMI_TTY_IOCQNUM_TEST03 _IO(QMI_TTY_IOC_MAGIC, 6)
#define QMI_TTY_IOCQNUM_TEST04 _IO(QMI_TTY_IOC_MAGIC, 7)
#define QMI_TTY_IOCQNUM_TEST05 _IO(QMI_TTY_IOC_MAGIC, 8)
#define QMI_TTY_IOCQNUM_TEST06 _IO(QMI_TTY_IOC_MAGIC, 9)
#define QMI_TTY_IOCQNUM_TEST07 _IO(QMI_TTY_IOC_MAGIC, 10)
#define QMI_TTY_IOCQNUM_TEST08 _IO(QMI_TTY_IOC_MAGIC, 11)

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

/* <DTS2010121503955.QMITTY: gKF34687 2011-01-19 ADD BEGIN */
/* mayang 2011-1-13 QMI ioctl params */
typedef struct QmiIoctlParams
{
    unsigned char client_id;
}QMI_IOCTL_PARAMS_ST;
/* DTS2010121503955.QMITTY: gKF34687 2011-01-19 ADD END> */

/* tty_memo�ṹ������QMITTY�ڲ�����¼�ϲ��û����̴������Ĺ�����Ϣ��
 *  ͨ����¼�µ�tty_structָ�룬����ȷ��������QMI��Ϣ������������һ��
 *  �û����̵Ŀռ䡣
 */
struct _tty_memo
{
    unsigned char client_id;    /*���ڼ�¼�û����̻�ȡ��Client ID*/
    unsigned char type;     /*���ڼ�¼�û���������ķ�������*/
    /* <AU3D00183.QMITTY: houxiaohe 2008-7-25 MODIFY BEGIN */
    unsigned char transaction_id;   /*���ڼ�¼�ض���ҵ��ID*/
    /* AU3D00183.QMITTY: houxiaohe 2008-7-25 MODIFY END> */
    struct tty_struct *   ptr_tty;      /*���ڼ�¼�û����̴�tty����ʱʹ�õ�tty_structָ��*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _tty_memo tty_memo;

/* QMI_NODE�ṹ��Ϊ�˱��ڹ���QMITTYģ���շ�QMI��Ϣ�������,��Ϣ��������������
 *  �ǿ��ǵ��շ���Ϣ���Ƶ����ÿ�ζ����ڴ��������ͷ�ͬ�����п�����
 */

/* <AU3D00181.QMITTY: houxiaohe 2008-7-18 MODIFY BEGIN */
struct _QMI_NODE
{
    struct list_head    ptr_qmi_node;   /*˫��ָ��������Ϣ�����������*/
    unsigned char*       p_msg_array;    /*ָ��SDU�ĵ�ַ*/
    unsigned int         row_num;        /*���Ӧ��Ϣ��������������*/
    unsigned long        trans_start;    /*��¼���ύ��urbͣ����ʱ��*/
    struct urb*         purb;           /*��ײ����ͨ��ò�ύ��urb*/
    struct usb_ctrlrequest dr;          /*��ײ����ͨ���ύ��Setup��*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_NODE QMI_NODE, * PQMI_NODE;

/* AU3D00181.QMITTY: houxiaohe 2008-7-18 MODIFY END> */

/*�궨�����ڴ�ӡ��ѯ�ж϶�ʱ��ǰ100�εĲ�ѯ���ָʾ*/
#define PRT_TIMER_INFO

/*�궨�������ڳ�ʼ�������в��Լ�飬�ź����顢��Ϣ��㡢��Ϣ����������Ϣ����ʼ������Ƿ�ɹ�����*/
#define TEST_INIT_NODE_AND_AREA

/* qmi_tty_open�в���0�Ŷ˵�Ļ����շ����� */
//#define QMI_TTY_EP0_TEST

/* �����ж϶˵㹦��ʱ������Ҫ�ѿ������� */
#define QMI_TTY_INTR_SUPPORT

/* �����ж϶˵㹦��ʱ������Ҫ�ѿ������� */
#define QMI_TTY_INTR_TEST

/* ʹ��usb_control_msg��������QMI��Ϣ */
#define QMI_TTY_USB_CNTL_MSG

/* ��ӡqmi��Ϣ */
//#define QMI_MSG_PRINT_DBG

#define CLASS_HOST_TO_INTERFACE     0x21    /*USB�豸��������*/
#define CLASS_INTERFACE_TO_HOST     0XA1    /*USB�豸��������*/
#define CDC_SEND_ENCAPSULATED_CMD   0x00    /*USB����������*/
#define CDC_GET_ENCAPSULATED_RSP    0x01    /*USB����������*/
#define CDC_SET_LINE_CODING          0x20    /*USB��·��������*/
#define CDC_GET_LINE_CODING          0x21    /*USB��·��������*/
#define CDC_SET_CONTROL_LINE_STATE  0x22    /*USB��·��������*/
#define CDC_SEND_BREAK                0x23    /*USB��·��������*/
#define CDC_SET_COMM_FEATURE         0x02    /*USB��·��������*/

#define ERR_COMMAND_NOT_EXIST        0x3F0   /*QMITTYģ����ΪIOCTL���ö���Ĵ����룬�����벻����*/
#define ERR_COMMAND_NOT_IMPLEMENT    0x3F1  /*QMITTYģ����ΪIOCTL���ö���Ĵ����룬������û�б�ʵ��*/

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
