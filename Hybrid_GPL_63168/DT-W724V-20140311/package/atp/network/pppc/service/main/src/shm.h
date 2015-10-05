#ifndef _PPPX_SHM_H_
#define  _PPPX_SHM_H_

#define PPPX_SHM_KEY                63873
#define PPPX_ACCOUNT_MAX           8       
#define PPPX_ACCOUNT_REPEAT_MAX   16
#define PPPX_IFCNAME_MAX           16
#define PPPX_USER_MAX               256
#define PPPX_PW_MAX                 256
#define PPPX_IP_POOL_MAX           32

/*Start of ATP 2008-12-8 for AU4D01133 by g00121640: 修改pppoe代理拨号等待认证超时时间*/
#if 0
#define PPPX_WAN_AUTH_TIMEOUT      60  //s
#else
#define PPPX_WAN_AUTH_TIMEOUT      30  //s
#endif
/*End of ATP 2008-12-8 for AU4D01133 by g00121640 */

#define PPPX_LAN_INVALID_IPIDX    (-1)
#define PPPX_RT_RULETB_ID_BASE    60
#define PPPX_NEGO_SHMID_PATH       "/var/pppx_shmid"
/*add by z67625 20070915 start*/
#define LANN                      9
#define LANLEN                    16

#define PPPX_TRUE                   (1)
#define PPPX_FALSE                  (0)

#ifndef NULL
#define  NULL                         0
#endif

enum
{
    PPPX_ACNT_UNUSED,
    PPPX_ACNT_IDLE,
    PPPX_ACNT_BUSY
};

enum
{
    PPPX_WAN_INIT,
    PPPX_WAN_DOWN,
    PPPX_WAN_SLEEP,
    PPPX_WAN_WAKEUP,
    PPPX_WAN_AUTHING,
    PPPX_WAN_AUTH_FAIL,
    PPPX_WAN_AUTH_SUCC,
    PPPX_WAN_UP
};

struct tagPPPXLan
{
   char name[PPPX_IFCNAME_MAX];
   int  used;
   int  pid;
   int  ip_idx;
   struct tagPPPXAcnt *acnt;
};

struct tagPPPXWan
{
   char name[PPPX_IFCNAME_MAX];
   char lanarray[LANN][LANLEN];
   int  lannum;
   int  bindenable;
   int  pid;
   int  state;
   int  rtb_id;
   struct tagPPPXAcnt *acnt;
};

struct tagPPPXAcnt
{
    int  state;
    int  lan_num;
    int  lan_max;
    char reg_name[PPPX_USER_MAX];
    char user[PPPX_USER_MAX];
    char pw[PPPX_PW_MAX];
    struct tagPPPXWan wan;
    struct tagPPPXLan lan[PPPX_ACCOUNT_REPEAT_MAX];
    struct tagPPPXAcntSet *acnt_set;
};

struct tagPPPXAcntSet
{
    unsigned long srv_ip;
    unsigned int ippool[PPPX_IP_POOL_MAX];    /*共支持256个地址，每个元素的每一个位表示地址是否空闲。0空闲，1占用*/
    int acnt_max;
    struct tagPPPXAcnt item[PPPX_ACCOUNT_MAX];
};

extern int PPPX_CreateAcntSet(int nKey);
extern struct tagPPPXAcntSet* PPPX_GetAcntSet(int nShmid);
extern int PPPX_PutAcntSet(struct tagPPPXAcntSet* pAcntSet);
extern int PPPX_FindUnusedAcnt(struct tagPPPXAcntSet *pAcntSet, struct tagPPPXAcnt **ppAcnt);
extern int PPPX_FindUnusedLan(struct tagPPPXAcnt *pAcnt, struct tagPPPXLan **ppLan);
extern int PPPX_FindUnusedIp(struct tagPPPXAcntSet *pAcntSet, int *pIpidx);
extern int PPPX_FindIdleAcnt(struct tagPPPXAcntSet *pAcntSet, struct tagPPPXAcnt **ppAcnt, char *pName,char *lanname);
extern struct tagPPPXAcnt* PPPX_FindAcnt(struct tagPPPXAcntSet *pAcntSet, const char *pUser, const char *pPw,const char *lanname);
extern struct tagPPPXLan* PPPX_FindLan(struct tagPPPXAcnt *pAcnt, const char* pName);
extern struct tagPPPXWan* PPPX_FindWan(struct tagPPPXAcntSet *pAcntSet, const char* pName);
extern void PPPX_RemoveLan(struct tagPPPXLan *pLan);
extern void PPPX_RemoveAcnt(struct tagPPPXAcnt *pAcnt);
extern void PPPX_RemoveIp(struct tagPPPXLan *pLan);
extern struct tagPPPXAcnt* PPPX_FindUsedAcnt(struct tagPPPXAcntSet *pAcntSet, const char *pUser);

extern int  PPPX_AcntIsRegitered(const struct tagPPPXAcntSet *pAcntSet, char *pName);










#endif
