#ifndef __ATP_WANCORE_H__
#define __ATP_WANCORE_H__


typedef enum tagATP_WANCORE_MSGTYPES_EN
{
    ATP_MSG_WANCORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_WANCORE),
    ATP_MSG_WANCORE_WANCONN_CHANGE,
} ATP_WANCORE_MSGTYPES_EN;

/* ���Ӵ�������Ҫ��DHCP/PPPOE�ͻ�����һ�� */
typedef enum
{
    /* �ȴ� */
    WAN_TRIGGER_IDLE = 0,
    /* ���� */
    WAN_TRIGGER_DISCONNECTING = 1,
    /* �ֶ� */
    WAN_TRIGGER_MANUAL = 2,
    /* �Զ� */
    WAN_TRIGGER_ALWAYSON = 3,
    /* ���貦�� */
    WAN_TRIGGER_ONDEMAND = 4, 
}WAN_TRIGGER_EN;


/* WAN����״̬ */
typedef enum
{
    /* δ��ʼ�� */
    ATP_WAN_STATUS_UNCONFIGURED,
    /* �������� */
    ATP_WAN_STATUS_CONNECTING,
    /* ������֤ */
    ATP_WAN_STATUS_AUTHENTICATING,
    /* ������ */
    ATP_WAN_STATUS_CONNECTED,
    /* δ���� */
    ATP_WAN_STATUS_PENDING,
    /* ���ڶϿ� */
    ATP_WAN_STATUS_DISCONNECTING,
    /* �ѶϿ� */
    ATP_WAN_STATUS_DISCONNECTED, 
}ATP_WAN_STATUS_EN;


#endif
