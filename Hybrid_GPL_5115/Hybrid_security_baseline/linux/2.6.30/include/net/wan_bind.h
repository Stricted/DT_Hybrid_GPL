
#ifndef _WAN_BIND_H_
#define _WAN_BIND_H_
#include <linux/skbuff.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm/atomic.h>
#include <linux/if.h>
#include <linux/netdevice.h>

#define WAN_UNSETINFO 61
#define WAN_SETINFO 62


/* WAN�������ͳ��� */
#define WAN_CONNTYPE_LEN 16

/* WAN�����б��� */
//#define WAN_SERVICE_LEN 256

/* WAN���ֳ��� */
#define WAN_NAME_LEN  32

/* LAN���ֳ��� */
#define LAN_NAME_LEN  32

/* LAN��Ŀ */ //Modify for wifi wan binding
//#ifdef CONFIG_NET_RADIO
#define WAN_LAN_NUM 8
//#else
//#define WAN_LAN_NUM 4
//#endif
/*WAN�˿ڰ󶨼�¼�ṹ,�ýṹ������WANCMS�����һ�� */
struct WanBindMap
{
	char   acLanName[LAN_NAME_LEN];  //LAN��������������һ��LAN��ֻ�ܰ󶨵�һ��PVC
	char   acPvcName[WAN_NAME_LEN];  //LAN�ڰ󶨵�PVC���֣����û�а���Ϊ�գ�NAS�ӿ�
	unsigned int ulPppxFlag;  //�󶨵�PVC�Ƿ������˴��� 
	unsigned int ulBindFlag;  //�󶨱�ʶ������ΪBIND_TRUE��û�а���ΪBIND_FALSE
	char   acConnType[WAN_CONNTYPE_LEN];
	//char   acServiceList[WAN_SERVICE_LEN];
};

/*���е�wan��Ϣ*/
struct WanDev
{
	struct list_head list;
	char acName[WAN_NAME_LEN];	//WAN������������
	unsigned int ulBind_lans;   //�󶨵�LAN
	int lBinded;                //�Ƿ����ð�
	int lPppx;                  //�Ƿ�����pppoe����
    int lWan_type;              //wan����
};

struct bindinfo
{
	char acName[WAN_NAME_LEN];	//WAN������������
	unsigned int ulBind_lans;   //�󶨵�LAN
	int lBinded;                //�Ƿ����ð�
	int lPppx;                  //�Ƿ�����pppoe����
    int lWan_type;              //wan����
};



#endif
