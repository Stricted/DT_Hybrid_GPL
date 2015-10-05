
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


/* WAN连接类型长度 */
#define WAN_CONNTYPE_LEN 16

/* WAN服务列表长度 */
//#define WAN_SERVICE_LEN 256

/* WAN名字长度 */
#define WAN_NAME_LEN  32

/* LAN名字长度 */
#define LAN_NAME_LEN  32

/* LAN数目 */ //Modify for wifi wan binding
//#ifdef CONFIG_NET_RADIO
#define WAN_LAN_NUM 8
//#else
//#define WAN_LAN_NUM 4
//#endif
/*WAN端口绑定记录结构,该结构必须与WANCMS定义的一致 */
struct WanBindMap
{
	char   acLanName[LAN_NAME_LEN];  //LAN口名字做索引，一个LAN口只能绑定到一个PVC
	char   acPvcName[WAN_NAME_LEN];  //LAN口绑定的PVC名字，如果没有绑定则为空，NAS接口
	unsigned int ulPppxFlag;  //绑定的PVC是否启用了代理 
	unsigned int ulBindFlag;  //绑定标识。绑定了为BIND_TRUE；没有绑定则为BIND_FALSE
	char   acConnType[WAN_CONNTYPE_LEN];
	//char   acServiceList[WAN_SERVICE_LEN];
};

/*所有的wan信息*/
struct WanDev
{
	struct list_head list;
	char acName[WAN_NAME_LEN];	//WAN口名字做索引
	unsigned int ulBind_lans;   //绑定的LAN
	int lBinded;                //是否启用绑定
	int lPppx;                  //是否启用pppoe代理
    int lWan_type;              //wan类型
};

struct bindinfo
{
	char acName[WAN_NAME_LEN];	//WAN口名字做索引
	unsigned int ulBind_lans;   //绑定的LAN
	int lBinded;                //是否启用绑定
	int lPppx;                  //是否启用pppoe代理
    int lWan_type;              //wan类型
};



#endif
