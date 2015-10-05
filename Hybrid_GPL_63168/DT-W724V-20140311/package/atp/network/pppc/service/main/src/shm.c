
/*
本文件实现对共享内存访问的封装.被使用的共享内存中存储了ppp 代理的lan和wan共享的
ppp帐号信息
*/
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "shm.h"


/***************************************************************************
// Function Name: PPPX_CreateAcntSet().
// Description  : 创建ppp proxy的共享内存，仅创建一次.
// Parameters   : 无.
// Returns      : 非0表示与共享内存关联的序号, -1 - ERROR.
****************************************************************************/
int PPPX_CreateAcntSet(int nKey)
{
    static int id = -1;

    if(id < 0)
        id = shmget(nKey, sizeof(struct tagPPPXAcntSet), S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL);

    return id;
}

/***************************************************************************
// Function Name: PPPX_GetAcntSet().
// Description  : 获得ppp proxy共享内存的指针.
// Parameters   : nShmid 共享内存的关联id.
// Returns      : 成功返回共享内存的指针， 失败返回 NULL.
****************************************************************************/
struct tagPPPXAcntSet* PPPX_GetAcntSet(int nShmid)
{
    void *p = NULL;

    p = shmat(nShmid, NULL, 0);
    if((void *)-1 == p)
        return NULL;
    else
        return (struct tagPPPXAcntSet*)p;
}

/***************************************************************************
// Function Name: PPPX_GetAcntSet().
// Description  : 获得ppp proxy共享内存的指针.
// Parameters   : nShmid 共享内存的关联id.
// Returns      : 成功返回共享内存的指针， 失败返回 NULL.
****************************************************************************/
int PPPX_PutAcntSet(struct tagPPPXAcntSet* pAcntSet)
{
    int nRet;

    nRet = shmdt((void *)pAcntSet);
    if(-1 == nRet)
    {/*print err*/}

    return nRet;
}
/***************************************************************************
// Function Name: PPPX_FindUnusedAcnt().
// Description  : 查找一个空闲的帐号资源.
// Parameters   : pAcntSet 所有的帐号资源.
                  ppAcnt   如果有空闲资源可用，通过该参数传出
// Returns      : PPPX_TRUE 成功， PPPX_FALSE 失败.
****************************************************************************/
int PPPX_FindUnusedAcnt( struct tagPPPXAcntSet *pAcntSet, struct tagPPPXAcnt **ppAcnt)
{
    int i;
    int nAcntMax = PPPX_ACCOUNT_MAX;

    if(NULL == pAcntSet || NULL == ppAcnt)
        return PPPX_FALSE;

    if((pAcntSet->acnt_max > 0) && (pAcntSet->acnt_max < PPPX_ACCOUNT_MAX))
        nAcntMax = pAcntSet->acnt_max;
    
    for(i = 0; i < nAcntMax; i++)
    {
        if(PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
        {
            *ppAcnt = &(pAcntSet->item[i]);
            (*ppAcnt)->state = PPPX_ACNT_BUSY;
            (*ppAcnt)->wan.acnt = *ppAcnt;
            (*ppAcnt)->acnt_set = pAcntSet;
            (*ppAcnt)->wan.rtb_id = PPPX_RT_RULETB_ID_BASE + i;
            return PPPX_TRUE;
        }
    }

    return PPPX_FALSE;
        
}

/***************************************************************************
// Function Name: PPPX_FindUnusedLan().
// Description  : 查找一个帐号的lan侧接口可用资源.
// Parameters   : pAcnt 一个正在使用的帐号.
                  ppAcnt   如果有lan侧接口可用资源，通过该参数传出
// Returns      : PPPX_TRUE 成功， PPPX_FALSE 失败.
****************************************************************************/
int PPPX_FindUnusedLan(struct tagPPPXAcnt *pAcnt, struct tagPPPXLan **ppLan)
{
    int i;
    int nLanMax = PPPX_ACCOUNT_REPEAT_MAX;

    if(NULL == pAcnt || NULL == ppLan)
        return PPPX_FALSE;

    if((pAcnt->lan_max) >= 0 && (pAcnt->lan_max < PPPX_ACCOUNT_REPEAT_MAX))
        nLanMax = pAcnt->lan_max;

    for(i = 0; i < nLanMax; i++)
    {
        if(PPPX_FALSE == pAcnt->lan[i].used)
        {
            *ppLan = &(pAcnt->lan[i]);
            (*ppLan)->used = PPPX_TRUE;
            (*ppLan)->acnt = pAcnt;
            (*ppLan)->acnt->lan_num++;
            (*ppLan)->ip_idx = PPPX_LAN_INVALID_IPIDX;
            return PPPX_TRUE;
        }
    }

    return PPPX_FALSE;
}

/***************************************************************************
// Function Name: PPPX_FindUnusedIp().
// Description  : 获得一个未使用的ip的索引
// Parameters   : pAcntSet 地址所在的帐号集
                  pIpidx   可用的ip地址索引，通过该参数传出
// Returns      : PPPX_TRUE 成功， PPPX_FALSE 失败.
****************************************************************************/
int PPPX_FindUnusedIp(struct tagPPPXAcntSet *pAcntSet, int *pIpidx)
{
    int i, j;

    if(NULL == pAcntSet || NULL == pIpidx)
        return PPPX_FALSE;

    for(i = 0; i < PPPX_IP_POOL_MAX; i++)
    {
        if(0xFFFFFFFF == pAcntSet->ippool[i])
            continue;
        
        for(j = 0; j < 8 * sizeof(pAcntSet->ippool[0]); j++)
        {
            if(pAcntSet->ippool[i] & (1 << j))
                continue;

            pAcntSet->ippool[i] |= (1 << j);
            *pIpidx = i * (8 * sizeof(pAcntSet->ippool[0])) + j;
            return PPPX_TRUE;
        }
    }

    return PPPX_FALSE;
}
/***************************************************************************
// Function Name: PPPX_FindUnusedAcnt().
// Description  : 查找一个空闲的帐号资源.
// Parameters   : pAcntSet 所有的帐号资源.
                  ppAcnt   如果有空闲资源可用，通过该参数传出
// Returns      : PPPX_TRUE 成功， PPPX_FALSE 失败.
****************************************************************************/
/*start of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
int PPPX_FindIdleAcnt(struct tagPPPXAcntSet *pAcntSet, struct tagPPPXAcnt **ppAcnt, char *pName,char *lanname)
{
    int i,j;
    int nAcntMax = PPPX_ACCOUNT_MAX;
    int findbind = 0;
    int lanbind = 0;

    if(NULL == pAcntSet || NULL == ppAcnt || NULL == lanname)
        return PPPX_FALSE;

    //先检查该LAN口是否有绑定
    if ('\0' != lanname[0])
    {
        for (i = 0; i < PPPX_ACCOUNT_MAX; i++)
        {
            if (PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
            {
                continue;
            }
            for (j = 0 ; j < pAcntSet->item[i].wan.lannum; j++)
            {
                if (0 == strcasecmp(lanname, pAcntSet->item[i].wan.lanarray[j]))
                {
                    lanbind = 1;
                    break;
                }
            }
            if (lanbind == 1)
            {
                break;
            }
        }
    }

    if((pAcntSet->acnt_max > 0) && (pAcntSet->acnt_max < PPPX_ACCOUNT_MAX))
        nAcntMax = pAcntSet->acnt_max;
    
    for(i = 0; i < nAcntMax; i++)
    {
        if(PPPX_ACNT_IDLE != pAcntSet->item[i].state)
            continue;
        
        if((NULL != pName) && strcmp(pAcntSet->item[i].reg_name, pName))
            continue;
        /*如果lanname不为空，且绑定了，需要校验是否绑定到该wan*/
        //if(('\0' != lanname[0]) && pAcntSet->item[i].wan.bindenable)
        //LAN有绑定
        if(('\0' != lanname[0]) && (0 != lanbind))
        {
            //if(debug)
            //    printf("in idleanct lan name is %s",lanname);
            for(j = 0 ;j < pAcntSet->item[i].wan.lannum;j++)
            {
               // if(debug)
                //    printf("%d lanname is %s",j,pAcntSet->item[i].wan.lanarray[j]);
                if(!strcasecmp(lanname,pAcntSet->item[i].wan.lanarray[j]))
                    findbind = 1;
            }
            if(!findbind)
                continue;
        }
    /*end of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
        *ppAcnt = &(pAcntSet->item[i]);
        (*ppAcnt)->wan.acnt = *ppAcnt;
        (*ppAcnt)->acnt_set = pAcntSet;
        return PPPX_TRUE;
    }

    return PPPX_FALSE;
        
}

struct tagPPPXAcnt* PPPX_FindUsedAcnt(struct tagPPPXAcntSet *pAcntSet, const char *pUser)
{
    int i;
    
    if(NULL == pAcntSet)
        return NULL;
    for(i = 0; i < PPPX_ACCOUNT_MAX; i++)
    {
        if(PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
        {
            continue;
        }
        
        /*user name only*/
        if(NULL != pUser)
        {
           if( strcmp(pAcntSet->item[i].reg_name, pUser) != 0)
            {
                continue;
            }

           pAcntSet->item[i].acnt_set = pAcntSet;
           pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
           return &(pAcntSet->item[i]);
        }
    }
    return NULL;

}
/***************************************************************************
// Function Name: PPPX_FindUnusedLan().
// Description  : 按帐号用户名和密码查找一个指定帐号.
// Parameters   : pAcntSet 所有的帐号.
                  pUser   要查找的帐号用户名    NULL 表示忽略该参数
                  pPw     要查找的帐号用户密码, NULL 表示忽略该参数
// Returns      : 成功帐号的指针， 失败返回NULL.
****************************************************************************/
struct tagPPPXAcnt* PPPX_FindAcnt(struct tagPPPXAcntSet *pAcntSet, const char *pUser, const char *pPw,const char *lanname)
{
    /*start of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
    int i,j;
    int findbind = 0;
    int cont = 0;
    int lanbind = 0;
    
    if(NULL == pAcntSet ||NULL == lanname)
    {
        return NULL;
    }
    /*end of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */

    //先检查该LAN口是否有绑定
    if ('\0' != lanname[0])
    {
        for (i = 0; i < PPPX_ACCOUNT_MAX; i++)
        {
            if (PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
            {
                continue;
            }
            for (j = 0 ; j < pAcntSet->item[i].wan.lannum; j++)
            {
                if (0 == strcasecmp(lanname, pAcntSet->item[i].wan.lanarray[j]))
                {
                    lanbind = 1;
                    break;
                }
            }
            if (lanbind == 1)
            {
                break;
            }
        }
    }

    for(i = 0; i < PPPX_ACCOUNT_MAX; i++)
    {
        if(PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
            continue;
        cont = 0;
        /*user name only*/
        if((NULL != pUser) && (NULL == pPw))
        {
           /*start of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
           //if(('\0' == lanname[0]) ||(0 == pAcntSet->item[i].wan.bindenable))
           //未知LAN或LAN未绑定
           if(('\0' == lanname[0]) || (0 == lanbind))
           {
               if( strcmp(pAcntSet->item[i].user, pUser) != 0)
                   cont = 1;
           }
           else
           {   /*不仅要比较用户名，还要比较是否绑定lan*/
              // if(debug)
              //     printf("in findacnt lanname is %s\n",lanname);
               if( strcmp(pAcntSet->item[i].user, pUser) != 0)
               {
                   cont = 1;
               }
               else
               {
                   for(j = 0 ;j < pAcntSet->item[i].wan.lannum;j++)
                   {
                       //if(debug)
                       //    printf("%d lanname is %s\n",j,pAcntSet->item[i].wan.lanarray[j]);
                       if(!strcasecmp(lanname,pAcntSet->item[i].wan.lanarray[j]))
                       {
                           findbind = 1;
                           break;
                       }
                   }
                   if(!findbind)
                       cont = 1;
               }
           }
           if(cont)
               continue;
           else
           {
               //if(debug)
               //    printf("find acnt !!!\n");
           }
           /*end of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */

           pAcntSet->item[i].acnt_set = pAcntSet;
           pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
           return &(pAcntSet->item[i]);
        }

        /*password only*/
        if((NULL == pUser) && (NULL != pPw) )
        {
            /*start of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
            //if(('\0' == lanname[0]) ||(0 == pAcntSet->item[i].wan.bindenable))
            //未知LAN或LAN未绑定
            if(('\0' == lanname[0]) || (0 == lanbind))
            {
                if(strcmp(pAcntSet->item[i].pw, pPw) != 0)
                    cont = 1;
            }
            else
            {  /*不仅要比较用户名，还要比较是否绑定lan*/
               if( strcmp(pAcntSet->item[i].pw, pPw) != 0)
               {
                   cont = 1;
               }
               else
               {
                   for(j = 0 ;j < pAcntSet->item[i].wan.lannum;j++)
                   {
                       if(!strcmp(lanname,pAcntSet->item[i].wan.lanarray[j]))
                           findbind = 1;
                   }
                   if(!findbind)
                       cont = 1;
               }
            }
            if(cont)
                continue;
            /*end of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */

            pAcntSet->item[i].acnt_set = pAcntSet;
            pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
            return &(pAcntSet->item[i]);
        }

        if((NULL != pUser) && (NULL != pPw)
            && strcmp(pAcntSet->item[i].user, pUser) == 0 
            && strcmp(pAcntSet->item[i].pw, pPw) == 0)
        {
            /*start of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
            //if(('\0' == lanname[0]) ||(0 == pAcntSet->item[i].wan.bindenable))
            //未知LAN或LAN未绑定
            if(('\0' == lanname[0]) || (0 == lanbind))
            {
                pAcntSet->item[i].acnt_set = pAcntSet;
                pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
                return &(pAcntSet->item[i]); 
            }
            else
            {
                for(j = 0 ;j < pAcntSet->item[i].wan.lannum;j++)
                {
                    if(!strcmp(lanname,pAcntSet->item[i].wan.lanarray[j]))
                        findbind = 1;
                }
                if(findbind)
                {        
                    pAcntSet->item[i].acnt_set = pAcntSet;
                    pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
                    return &(pAcntSet->item[i]); 
                }
            }
            /*end of HG_Maintenance by z67625 2007.10.16 ATPV100R001 AU4D00122 */
        }
    }

    return NULL;
}

/***************************************************************************
// Function Name: PPPX_FindLan().
// Description  : 在一个帐号中按接口名查找指定lan接口.
// Parameters   : pAcnt 一个正在使用的帐号.
                  pName   lan侧接口的名字
// Returns      : 成功返回lan接口的指针， 失败空.
****************************************************************************/
struct tagPPPXLan* PPPX_FindLan(struct tagPPPXAcnt *pAcnt, const char* pName)
{
    int i;
    
    if(NULL == pAcnt || NULL == pName)
        return NULL;

    for(i = 0; i < PPPX_ACCOUNT_REPEAT_MAX; i++)
    {
        if(PPPX_FALSE == pAcnt->lan[i].used)
            continue;
        
        if(strcmp(pAcnt->lan[i].name, pName) == 0)
        {
            pAcnt->lan[i].acnt = pAcnt;
            return &(pAcnt->lan[i]);
        }
    }

    return NULL;
}

/***************************************************************************
// Function Name: PPPX_FindWan().
// Description  : 在一个帐号集中查询指定的wan接口.
// Parameters   : pAcntSet 一个正在使用的帐号集.
                  pName   wan侧接口的名字
// Returns      : 成功返回wan接口的指针， 失败空.
****************************************************************************/
struct tagPPPXWan* PPPX_FindWan(struct tagPPPXAcntSet *pAcntSet, const char* pName)
{
    int i;
    
    if(NULL == pAcntSet || NULL == pName)
        return NULL;

    for(i = 0; i < PPPX_ACCOUNT_MAX; i++)
    {
        if(PPPX_ACNT_UNUSED == pAcntSet->item[i].state)
            continue;
        
        if(strcmp(pAcntSet->item[i].wan.name, pName) == 0)
        {
            pAcntSet->item[i].wan.acnt = &(pAcntSet->item[i]);
            pAcntSet->item[i].wan.acnt->acnt_set = pAcntSet;
            return &(pAcntSet->item[i].wan);
        }
    }

    return NULL;
}

/***************************************************************************
// Function Name: PPPX_RemoveLan().
// Description  : 删除lan接口.
// Parameters   : pLan 需要删除的lan接口.

****************************************************************************/
void PPPX_RemoveLan(struct tagPPPXLan *pLan)
{
    if(NULL == pLan)
        return;

    pLan->used = PPPX_FALSE;
    pLan->acnt->lan_num--; 
    pLan->pid = 0;

    if(0 == pLan->acnt->lan_num)
    {
        pLan->acnt->user[0] = 0;
        pLan->acnt->pw[0] = 0;
    }
}

/***************************************************************************
// Function Name: PPPX_RemoveAcnt().
// Description  : 删除一个帐号.
// Parameters   : pAcnt 需要删除的帐号
****************************************************************************/
void PPPX_RemoveAcnt(struct tagPPPXAcnt *pAcnt)
{
    if(NULL == pAcnt)
        return;

    pAcnt->state = PPPX_ACNT_UNUSED;
    pAcnt->user[0] = 0; 
    pAcnt->pw[0] = 0;
}

/***************************************************************************
// Function Name: PPPX_RemoveAcnt().
// Description  : 删除一个帐号.
// Parameters   : pAcnt 需要删除的帐号
****************************************************************************/
void PPPX_RemoveIp(struct tagPPPXLan *pLan)
{
    int nSeg;
    int nBit;
    
    if((NULL == pLan) || (PPPX_LAN_INVALID_IPIDX == pLan->ip_idx))
        return;

    nSeg = (int)(pLan->ip_idx/(8 * sizeof(pLan->acnt->acnt_set->ippool[0])));
    nBit = pLan->ip_idx % (8 * sizeof(pLan->acnt->acnt_set->ippool[0]));
    
    if(nSeg < PPPX_IP_POOL_MAX)
    {
        pLan->acnt->acnt_set->ippool[nSeg] &= (~((unsigned int)1 << nBit));
        pLan->ip_idx = PPPX_LAN_INVALID_IPIDX;
    }
}
/***************************************************************************
// Function Name: PPPX_AcntIsRegitered().
// Description  : 查询一个帐号名是否已经在系统中注册.
// Parameters   : pName 需要查询的帐号名称
// return       : PPPX_TRUE  帐号已经注册
//                PPPX_FALSE 帐号未注册
****************************************************************************/
int PPPX_AcntIsRegitered(const struct tagPPPXAcntSet *pAcntSet, char *pName)
{
    int i;
    int nAcntMax = PPPX_ACCOUNT_MAX;
    
    if(NULL == pAcntSet || NULL == pName)
        return PPPX_FALSE;

    if((pAcntSet->acnt_max > 0) && (pAcntSet->acnt_max < PPPX_ACCOUNT_MAX))
        nAcntMax = pAcntSet->acnt_max;
    
    for(i = 0; i < nAcntMax; i++)
    {

        if(strcmp(pAcntSet->item[i].reg_name, pName))
            continue;

        return PPPX_TRUE;
    }
    
    return PPPX_FALSE;
}
