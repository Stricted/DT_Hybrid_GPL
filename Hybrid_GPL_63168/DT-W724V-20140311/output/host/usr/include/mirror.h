/**\file mirror.h
  *镜像抓包代码头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author 刘烨(l81004871)
  *\date  2012-3-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>

#include "atptypes.h"
#include "atp_interface.h"

#if ATP_DEBUG
#define DEBMIRROR(str, args...)        \
do{                              \
    printf("[%s %s:%d]  ", __FILE__, __FUNCTION__, __LINE__); \
    printf("\033[1;33;40m");   \
    printf(str, ## args);      \
    printf("\033[0;37;40m\n"); \
}while(0) 
#else
#define DEBMIRROR(str, args...)
#endif

#define LINE_END            "\r\n"
#define CMD_PREFIX          "--"
#define OPTIONS_MAX_NUM     8

#define MIRROR_INTF_SIZE    32
#define MIRROR_DIR_IN       0
#define MIRROR_DIR_OUT      1
#define MIRROR_DISABLED     0
#define MIRROR_ENABLED      1

#define MIRROR_LAN1       "lan1"
#define MIRROR_LAN2       "lan2"
#define MIRROR_LAN3       "lan3"
#define MIRROR_LAN4       "lan4"
#define MIRROR_ETHWAN     "ethwan"
#define MIRROR_WLAN       "wlan"
#define MIRROR_DSL        "dsl"
#ifdef SUPPORT_ATP_LTE
#define MIRROR_LTE        "lte"
#define MIRROR_LTE_NUM        6
#endif
#ifdef SUPPORT_ATP_HYBRID
#define MIRROR_BONDING        "bonding"
#endif
#define MIRROR_LAN1_NUM       0
#define MIRROR_LAN2_NUM       1 
#define MIRROR_LAN3_NUM       2
#define MIRROR_LAN4_NUM       3
#define MIRROR_GE_NUM         4
#define MIRROR_WLAN_NUM       5
#define MIRROR_DSL_NUM        6
#define MIRROR_ETHWAN_NUM     7


#define READ_BUFF_LEN         512
enum {
    SIOCGLINKSTATE = SIOCDEVPRIVATE + 1,
    SIOCSCLEARMIBCNTR,
    SIOCMIBINFO,
    SIOCGENABLEVLAN,
    SIOCGDISABLEVLAN,
    SIOCGQUERYNUMVLANPORTS,
    SIOCGQUERYNUMPORTS,
    SIOCPORTMIRROR,
    SIOCSWANPORT,
    SIOCGWANPORT,
    SIOCETHCTLOPS,
    SIOCGPONIF,
    SIOCETHSWCTLOPS,
    SIOCGSWITCHPORT,
    SIOCLAST,
};

typedef struct _MirrorCfg
{
    char szMonitorInterface[MIRROR_INTF_SIZE];
    char szMirrorInterface[MIRROR_INTF_SIZE];
    int nDirection;
    int nStatus;
} MirrorCfg ;

typedef int (*fpProcessFun)(void *pvData);

typedef struct mirror_command	
{
    char *pstCmd;
    int iOptNumber;
    char *pacOptions[OPTIONS_MAX_NUM];
    fpProcessFun fpProcess;
}MIRROR_CMD_ST;

typedef struct mirror_type	
{
    char *pacType;
    unsigned int ulPortNum;
}MIRROR_TYPE_ST;

#define SIOCETHWLMIRROR   0x894C
#define MAX_MIRROR_PORT_MUM 10
typedef struct st_port_mirror_ioctl_data{
    int  port_mirror_ctl;     /*enable:1 or disable:0*/
    int  mirror_from;
    int  mirror_to;
    int  mirror_opt;          /*get:0 or set:1*/
    int  mirror_status[MAX_MIRROR_PORT_MUM];    
}port_mirror_ioctl_data;


static void usage();
static void CreateSocket(void);
static void CloseSocket(void);
static int SetMirror(int iDir, int iStatus);
static int MirrorStart(void *pvData);
static int MirrorStartAll(void *pvData);
static int MirrorStop(void *pvData);
static int MirrorStopAll(void *pvData);
static int SetArgs(MIRROR_CMD_ST *pstCfg,char **ppszArgv);
static MIRROR_CMD_ST *FindCmd(const char *pszCmdName);
static int ProcessCmd(int argc, char **argv);
static int MirrorStartFunc(void *pvData);
static int MirrorXdslStartFunc(const MIRROR_CMD_ST *pstCmd);
static int MirrorXdslStopFunc(void);
static int MirrorStopFunc(void *pvData);
static int MirrorKernelStartFunc(const MIRROR_CMD_ST *pstCmd,const unsigned int ulMirrorType);
static int ethSetMirror(int from_phy_id, int to_phy_id);
