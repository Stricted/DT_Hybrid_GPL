
/*
���ļ�ʵ�ֶԹ����ڴ���ʵķ�װ.��ʹ�õĹ����ڴ��д洢��ppp �����lan��wan�����
ppp�ʺ���Ϣ
*/
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "shm.h"


/***************************************************************************
// Function Name: PPPX_CreateAcntSet().
// Description  : ����ppp proxy�Ĺ����ڴ棬������һ��.
// Parameters   : ��.
// Returns      : ��0��ʾ�빲���ڴ���������, -1 - ERROR.
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
// Description  : ���ppp proxy�����ڴ��ָ��.
// Parameters   : nShmid �����ڴ�Ĺ���id.
// Returns      : �ɹ����ع����ڴ��ָ�룬 ʧ�ܷ��� NULL.
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
// Description  : ���ppp proxy�����ڴ��ָ��.
// Parameters   : nShmid �����ڴ�Ĺ���id.
// Returns      : �ɹ����ع����ڴ��ָ�룬 ʧ�ܷ��� NULL.
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
// Description  : ����һ�����е��ʺ���Դ.
// Parameters   : pAcntSet ���е��ʺ���Դ.
                  ppAcnt   ����п�����Դ���ã�ͨ���ò�������
// Returns      : PPPX_TRUE �ɹ��� PPPX_FALSE ʧ��.
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
// Description  : ����һ���ʺŵ�lan��ӿڿ�����Դ.
// Parameters   : pAcnt һ������ʹ�õ��ʺ�.
                  ppAcnt   �����lan��ӿڿ�����Դ��ͨ���ò�������
// Returns      : PPPX_TRUE �ɹ��� PPPX_FALSE ʧ��.
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
// Description  : ���һ��δʹ�õ�ip������
// Parameters   : pAcntSet ��ַ���ڵ��ʺż�
                  pIpidx   ���õ�ip��ַ������ͨ���ò�������
// Returns      : PPPX_TRUE �ɹ��� PPPX_FALSE ʧ��.
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
// Description  : ����һ�����е��ʺ���Դ.
// Parameters   : pAcntSet ���е��ʺ���Դ.
                  ppAcnt   ����п�����Դ���ã�ͨ���ò�������
// Returns      : PPPX_TRUE �ɹ��� PPPX_FALSE ʧ��.
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

    //�ȼ���LAN���Ƿ��а�
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
        /*���lanname��Ϊ�գ��Ұ��ˣ���ҪУ���Ƿ�󶨵���wan*/
        //if(('\0' != lanname[0]) && pAcntSet->item[i].wan.bindenable)
        //LAN�а�
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
// Description  : ���ʺ��û������������һ��ָ���ʺ�.
// Parameters   : pAcntSet ���е��ʺ�.
                  pUser   Ҫ���ҵ��ʺ��û���    NULL ��ʾ���Ըò���
                  pPw     Ҫ���ҵ��ʺ��û�����, NULL ��ʾ���Ըò���
// Returns      : �ɹ��ʺŵ�ָ�룬 ʧ�ܷ���NULL.
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

    //�ȼ���LAN���Ƿ��а�
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
           //δ֪LAN��LANδ��
           if(('\0' == lanname[0]) || (0 == lanbind))
           {
               if( strcmp(pAcntSet->item[i].user, pUser) != 0)
                   cont = 1;
           }
           else
           {   /*����Ҫ�Ƚ��û�������Ҫ�Ƚ��Ƿ��lan*/
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
            //δ֪LAN��LANδ��
            if(('\0' == lanname[0]) || (0 == lanbind))
            {
                if(strcmp(pAcntSet->item[i].pw, pPw) != 0)
                    cont = 1;
            }
            else
            {  /*����Ҫ�Ƚ��û�������Ҫ�Ƚ��Ƿ��lan*/
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
            //δ֪LAN��LANδ��
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
// Description  : ��һ���ʺ��а��ӿ�������ָ��lan�ӿ�.
// Parameters   : pAcnt һ������ʹ�õ��ʺ�.
                  pName   lan��ӿڵ�����
// Returns      : �ɹ�����lan�ӿڵ�ָ�룬 ʧ�ܿ�.
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
// Description  : ��һ���ʺż��в�ѯָ����wan�ӿ�.
// Parameters   : pAcntSet һ������ʹ�õ��ʺż�.
                  pName   wan��ӿڵ�����
// Returns      : �ɹ�����wan�ӿڵ�ָ�룬 ʧ�ܿ�.
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
// Description  : ɾ��lan�ӿ�.
// Parameters   : pLan ��Ҫɾ����lan�ӿ�.

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
// Description  : ɾ��һ���ʺ�.
// Parameters   : pAcnt ��Ҫɾ�����ʺ�
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
// Description  : ɾ��һ���ʺ�.
// Parameters   : pAcnt ��Ҫɾ�����ʺ�
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
// Description  : ��ѯһ���ʺ����Ƿ��Ѿ���ϵͳ��ע��.
// Parameters   : pName ��Ҫ��ѯ���ʺ�����
// return       : PPPX_TRUE  �ʺ��Ѿ�ע��
//                PPPX_FALSE �ʺ�δע��
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
