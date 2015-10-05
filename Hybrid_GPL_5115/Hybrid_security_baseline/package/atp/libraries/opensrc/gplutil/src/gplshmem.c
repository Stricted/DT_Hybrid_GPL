#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmemapi.h"
#include "bget.h"

#define ATP_UTIL_GVAR_SHM_PATH  ("/var/")
#define ATP_UTIL_GVAR_SHM_ID    ('g')

#if 0
#define ATP_UTIL_GVAR_MAX_BUFF  (524288) //512k
#define ATP_UTIL_GVAR_MIN_BUFF  (1024) // 1k
#define ATP_UTIL_GVAR_MAGIC_NUM (0x00042047)
#endif

#define ATP_UTIL_GVAR_MAX_MODULENUM   (16)


#define ATP_UTIL_SEM_INIT_VALUE  (10)
#define ATP_UTIL_SEMVALUE_FOR_WRITE  (ATP_UTIL_SEM_INIT_VALUE)
#define ATP_UTIL_SEMVALUE_FOR_READ   (1)

#define ATP_UTIL_GVAR_SHM_BASEADDR  (0x2EF00000) // DO NOT MODIFY
#define ATP_UTIL_GVAR_SHM_INCREASE  (0x00100000)

typedef struct tagATP_UTIL_GVAR_PAIR
{
    VOS_UINT32                  ulGVarIdx;
    VOS_VOID                    *pvValue;
    VOS_UINT32                  ulValLen;
    struct tagATP_UTIL_GVAR_PAIR     *pstNext;
} ATP_UTIL_GVAR_PAIR;

typedef struct tagATP_UTIL_GVAR_INFO
{
    VOS_UINT32              ulMagicNum;
    VOS_UINT32              ulMemSize;
    VOS_INT32               lAccessSemID;
    VOS_INT32               lShmID;
    VOS_UINT16              usBgetOffset;
    VOS_UINT16              usMemCheck;     // 0: not check; 1: check
    ATP_UTIL_GVAR_PAIR*     pstGVars;
    ATP_UTIL_BGET_GLOBAL_ST stBgetGlobal;
}ATP_UTIL_GVAR_INFO;

#if 0
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux specific) */
};
#endif

static VOS_UINT32 gvarLoadShareMem(VOS_INT32 lShmID, VOS_VOID* pvBaseAddress)
{
    VOS_CHAR* pucShmAddr = NULL;
#ifdef ATP_PCLINT
    pvBaseAddress = pvBaseAddress;
#endif

    pucShmAddr = (VOS_CHAR*)shmat(lShmID, pvBaseAddress, 0);
    if (pucShmAddr != (VOS_CHAR*)pvBaseAddress)
    {
        return VOS_NOK;
    }
    return VOS_OK;
}

ATP_STATIC VOS_BOOL gvarShmExist(VOS_UINT32 ulModIdx)
{
    VOS_INT32 lKey = -1;
    VOS_INT32 lShmNum = 0;
    VOS_INT32 i;
    struct shm_info stShmInfo;
    struct shmid_ds stShmSeg;
    struct ipc_perm *ipcp = &stShmSeg.shm_perm;

    if (ulModIdx > ATP_UTIL_GVAR_MAX_MODULENUM)
    {
        return VOS_FALSE;
    }
    //get the key
    lKey = ftok(ATP_UTIL_GVAR_SHM_PATH, (VOS_INT32)(ATP_UTIL_GVAR_SHM_ID + ulModIdx));
    //check segment exist
    lShmNum = shmctl(0, SHM_INFO, (struct shmid_ds *) (void *) &stShmInfo);
    if (lShmNum < 0)
    {
        return VOS_FALSE;
    }
    for (i = 0; i < lShmNum + 1; i++)   // !! lShmNum + 1
    {
        shmctl(i, SHM_STAT, &stShmSeg);
        if (lKey == (VOS_INT32)ipcp->__key)    // __key??
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}

static VOS_INT32 gvarSemWait(VOS_INT32 lSemID, VOS_INT32 lSemValue)
{
    struct sembuf stSemBuf;
    VOS_BOOL  bAgainFlag = VOS_TRUE;
    VOS_INT lRet     = 0;

    if (0 == lSemID)
    {
        return 0;
    }

    while(bAgainFlag)
    {
        stSemBuf.sem_num = 0;
        stSemBuf.sem_op  = (VOS_INT16)(0 - lSemValue);
        stSemBuf.sem_flg = SEM_UNDO;

        if (-1 == semop(lSemID, &stSemBuf, 1))
        {
            if(EINTR != errno)
            {
                bAgainFlag = VOS_FALSE;
                lRet      = -1;
            }
        }
        else
        {
            bAgainFlag = VOS_FALSE;;
        }
    }
    return lRet;
}

static VOS_INT32 gvarSemPost(VOS_INT32 lSemID, VOS_INT32 lSemValue)
{
    struct sembuf stSemBuf;
    VOS_BOOL  bAgainFlag = VOS_TRUE;
    VOS_INT lRet     = 0;

    if (0 == lSemID)
    {
        return 0;
    }

    while(bAgainFlag)
    {
        stSemBuf.sem_num = 0;
        stSemBuf.sem_op  = (VOS_INT16)lSemValue;
        stSemBuf.sem_flg = SEM_UNDO;
        
        if (-1 == semop(lSemID, &stSemBuf, 1))
        {
            if(EINTR != errno)
            {
                bAgainFlag  = VOS_FALSE;
                lRet        = -1;
            }
        }
        else
        {
            bAgainFlag = VOS_FALSE;;
        }
    }
    return lRet;
}

ATP_STATIC ATP_UTIL_GVAR_PAIR *gvarFindByIdx(ATP_UTIL_GVAR_INFO* pstGvarInfo, VOS_UINT32 ulGVarIdx)
{
    ATP_UTIL_GVAR_PAIR       *pstTmp;
#ifdef ATP_PCLINT
    pstGvarInfo = pstGvarInfo;
    pstGvarInfo->lShmID = pstGvarInfo->lShmID;
    pstGvarInfo->ulMagicNum = pstGvarInfo->ulMagicNum;
    pstGvarInfo->ulMemSize = pstGvarInfo->ulMemSize;
    pstGvarInfo->usBgetOffset = pstGvarInfo->usBgetOffset;
    pstGvarInfo->usMemCheck = pstGvarInfo->usMemCheck;
#endif
    pstTmp = pstGvarInfo->pstGVars;
    while (NULL != pstTmp)
    {
        if (ulGVarIdx == pstTmp->ulGVarIdx)
        {
            break;
        }
        pstTmp = pstTmp->pstNext;
    }
    return pstTmp;
}

ATP_STATIC VOS_VOID gvarDelByIdx(VOS_VOID* pvBgetHdl, ATP_UTIL_GVAR_INFO* pstGvarInfo, VOS_UINT32 ulGVarIdx)
{
    ATP_UTIL_GVAR_PAIR       *pstPrev;
    ATP_UTIL_GVAR_PAIR       *pstTmp;

    pstPrev = NULL;
    pstTmp = pstGvarInfo->pstGVars;
    while (NULL != pstTmp)
    {
        if (ulGVarIdx == pstTmp->ulGVarIdx)
        {
            if (NULL != pstPrev)
            {
                pstPrev->pstNext = pstTmp->pstNext;
            }
            else
            {
                pstGvarInfo->pstGVars = pstTmp->pstNext;
            }
            ATP_UTIL_BgetFree(pvBgetHdl, (VOS_VOID *)(pstTmp->pvValue));
            ATP_UTIL_BgetFree(pvBgetHdl, (VOS_VOID *)pstTmp);
            return;
        }
        pstPrev = pstTmp;
        pstTmp = pstTmp->pstNext;
    }
}

VOS_VOID ATP_UTIL_BgetFree(VOS_VOID* pvBgetHandle, VOS_VOID* pvBuf)
{
    if (pvBgetHandle)
    {
        brel(pvBgetHandle, pvBuf);
    }
}

VOS_VOID* ATP_UTIL_BgetMalloc(VOS_VOID* pvBgetHandle, VOS_UINT32 ulSize)
{
    if (pvBgetHandle)
    {
        return bget(pvBgetHandle, (bufsize)ulSize);
    }
    else
    {
        return VOS_NULL_PTR;
    }
}

VOS_VOID* ATP_UTIL_BgetStrdup(VOS_VOID* pvBgetHandle, VOS_VOID* pvString)
{

    VOS_CHAR* pszTmp;
#ifdef ATP_PCLINT
    pvString = pvString;
#endif

    if (!pvString)
    {
        return VOS_NULL_PTR;
    }

    pszTmp = ATP_UTIL_BgetMalloc(pvBgetHandle, strlen(pvString) + 1);
    if (pszTmp)
    {
        strcpy(pszTmp, pvString);
        return pszTmp;
    }
    else
    {
        return VOS_NULL_PTR;
    }
}

VOS_VOID *ATP_UTIL_GVarGetMemHandle(VOS_UINT32 ulModIdx)
{
    ATP_UTIL_GVAR_INFO          *pstGvarInfo;
    VOS_VOID                    *pvUserBaseAddr;

    //check if the gvar module exist
    if (VOS_FALSE == gvarShmExist(ulModIdx))
    {
        return NULL;
    }

    pvUserBaseAddr = (VOS_VOID *)(ATP_UTIL_GVAR_SHM_BASEADDR + ulModIdx * ATP_UTIL_GVAR_SHM_INCREASE);
    pstGvarInfo = (ATP_UTIL_GVAR_INFO *)pvUserBaseAddr;
    return (VOS_VOID *)(&(pstGvarInfo->stBgetGlobal));
}

VOS_UINT32 ATP_UTIL_GVarAttach(VOS_UINT32 ulGvarModule)
{
    VOS_INT32 lKey;
    VOS_INT32 lShmNum = 0;
    VOS_INT32 lShmID;
    VOS_INT32 i; 
    struct shm_info stShmInfo;
    struct shmid_ds stShmSeg;
    struct ipc_perm *ipcp = &stShmSeg.shm_perm;
    VOS_VOID* pvUserBaseAddr = VOS_NULL_PTR;

    if (ulGvarModule >= ATP_UTIL_GVAR_MOD_END)
    {
        return VOS_NOK;
    }    

    lKey = ftok(ATP_UTIL_GVAR_SHM_PATH, (VOS_INT32)(ATP_UTIL_GVAR_SHM_ID + ulGvarModule));
    lShmNum = shmctl(0, SHM_INFO, (struct shmid_ds *) (void *) &stShmInfo);
    if (lShmNum < 0)
    {
        return VOS_NOK;
    }
    for (i = 0; i < lShmNum + 1; i++)   // !! lShmNum + 1
    {
        lShmID = shmctl(i, SHM_STAT, &stShmSeg);
        if (lKey == (VOS_INT32)ipcp->__key)    // __key??
        {
            // do shmat 
            pvUserBaseAddr = (VOS_VOID *)(ATP_UTIL_GVAR_SHM_BASEADDR + ulGvarModule * ATP_UTIL_GVAR_SHM_INCREASE);
            return gvarLoadShareMem(lShmID, pvUserBaseAddr);
        }
    }
    return VOS_NOK;
}

VOS_UINT32 ATP_UTIL_GVarGetValueEx(VOS_UINT32 ulVarIdx, VOS_VOID *pvValue,
                                    VOS_UINT32 ulBufLen, VOS_UINT32 *pulValueLength)
{
    ATP_UTIL_GVAR_PAIR       *pstTmp;
    ATP_UTIL_GVAR_INFO* pstGvarInfo = VOS_NULL_PTR;
    VOS_VOID* pvUserBaseAddr = VOS_NULL_PTR;

    if (NULL != pulValueLength)
    {
        (*pulValueLength) = 0;
    }

    if (NULL == pvValue)
    {
        return VOS_NOK;
    }

    //check if the gvar module exist
    if (VOS_FALSE == gvarShmExist(ATP_UTIL_GVAR_MOD_MASK(ulVarIdx)))
    {
        return VOS_OK;
    }

    pvUserBaseAddr = (VOS_VOID *)(ATP_UTIL_GVAR_SHM_BASEADDR + ATP_UTIL_GVAR_MOD_MASK(ulVarIdx) * ATP_UTIL_GVAR_SHM_INCREASE);
    pstGvarInfo = (ATP_UTIL_GVAR_INFO*)pvUserBaseAddr;
    if (NULL == pstGvarInfo->pstGVars)
    {
        return VOS_OK;
    }

    gvarSemWait(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_READ);
    pstTmp = gvarFindByIdx(pstGvarInfo, ulVarIdx);
    if (NULL != pstTmp)
    {
        if (pstTmp->ulValLen < ulBufLen)
        {
            ulBufLen = pstTmp->ulValLen;
        }
        memcpy((VOS_VOID *)(pvValue), pstTmp->pvValue, ulBufLen);
        if (NULL != pulValueLength)
        {
            (*pulValueLength) = ulBufLen;
        }
    }

    gvarSemPost(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_READ);
    return VOS_OK;
}

VOS_UINT32 ATP_UTIL_GVarGetValueWithCopy(VOS_UINT32 ulVarIdx, VOS_VOID **ppvValue, VOS_UINT32 *pulValueLength)
{
    ATP_UTIL_GVAR_PAIR       *pstTmp;
    ATP_UTIL_GVAR_INFO* pstGvarInfo = VOS_NULL_PTR;
    VOS_VOID* pvUserBaseAddr = VOS_NULL_PTR;

    if (NULL == ppvValue)
    {
        return VOS_NOK;
    }

    (*ppvValue) = NULL;
    if (NULL != pulValueLength)
    {
        (*pulValueLength) = 0;
    }

    //check if the gvar module exist
    if (VOS_FALSE == gvarShmExist(ATP_UTIL_GVAR_MOD_MASK(ulVarIdx)))
    {
        return VOS_OK;
    }

    pvUserBaseAddr = (VOS_VOID *)(ATP_UTIL_GVAR_SHM_BASEADDR + ATP_UTIL_GVAR_MOD_MASK(ulVarIdx) * ATP_UTIL_GVAR_SHM_INCREASE);
    pstGvarInfo = (ATP_UTIL_GVAR_INFO*)pvUserBaseAddr;
    if (NULL == pstGvarInfo->pstGVars)
    {
        return VOS_OK;
    }

    gvarSemWait(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_READ);
    pstTmp = gvarFindByIdx(pstGvarInfo, ulVarIdx);
    if (NULL != pstTmp)
    {
        (*ppvValue) = malloc(pstTmp->ulValLen);
        if (NULL == (*ppvValue))
        {
            gvarSemPost(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_READ);
            return VOS_NOK;
        }

        memcpy((VOS_VOID *)(*ppvValue), pstTmp->pvValue, pstTmp->ulValLen);
        if (NULL != pulValueLength)
        {
            (*pulValueLength) = pstTmp->ulValLen;
        }
    }

    gvarSemPost(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_READ);
    return VOS_OK;
}

VOS_UINT32 ATP_UTIL_GVarSetValue(VOS_UINT32 ulVarIdx, const VOS_VOID *pvValue, VOS_UINT32 ulValLen)
{
    VOS_UINT32          ulRetVal;
    VOS_VOID            *pvNew;
    ATP_UTIL_GVAR_PAIR       *pstTmp;
    ATP_UTIL_GVAR_INFO* pstGvarInfo = VOS_NULL_PTR;
    VOS_VOID* pvUserBaseAddr = VOS_NULL_PTR;

    ulRetVal = VOS_OK;

    //check if the gvar module exist
    if (VOS_FALSE == gvarShmExist(ATP_UTIL_GVAR_MOD_MASK(ulVarIdx)))
    {
        return VOS_NOK;
    }

    pvUserBaseAddr = (VOS_VOID *)(ATP_UTIL_GVAR_SHM_BASEADDR + ATP_UTIL_GVAR_MOD_MASK(ulVarIdx) * ATP_UTIL_GVAR_SHM_INCREASE);
    pstGvarInfo = (ATP_UTIL_GVAR_INFO*)pvUserBaseAddr;

    gvarSemWait(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_WRITE);
    if (0 == ulValLen) // Delete the pair item
    {
        gvarDelByIdx(pvUserBaseAddr, pstGvarInfo, ulVarIdx);
        gvarSemPost(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_WRITE);
        return VOS_OK;
    }

    pstTmp = gvarFindByIdx(pstGvarInfo, ulVarIdx);
    pvNew  = (VOS_VOID *)ATP_UTIL_BgetMalloc(&pstGvarInfo->stBgetGlobal, ulValLen);
    if (NULL != pvNew)
    {
        memcpy((VOS_VOID *)pvNew, pvValue, ulValLen);
        if (NULL != pstTmp) // Overwrite the old value
        {
            ATP_UTIL_BgetFree(&pstGvarInfo->stBgetGlobal, (VOS_VOID *)(pstTmp->pvValue));
            pstTmp->pvValue = pvNew;
            pstTmp->ulValLen = ulValLen;
        }
        else // Append a new item to the pair list
        {
            pstTmp = ATP_UTIL_BgetMalloc(&pstGvarInfo->stBgetGlobal, sizeof(ATP_UTIL_GVAR_PAIR));
            if (NULL != pstTmp)
            {
                memset((VOS_VOID *)pstTmp, 0, sizeof(ATP_UTIL_GVAR_PAIR));
                pstTmp->ulGVarIdx = ulVarIdx;
                pstTmp->ulValLen = ulValLen;
                pstTmp->pvValue = pvNew;
                pstTmp->pstNext = pstGvarInfo->pstGVars;
                pstGvarInfo->pstGVars = pstTmp;
            }
            else
            {
                ATP_UTIL_BgetFree(&pstGvarInfo->stBgetGlobal, (VOS_VOID *)pvNew);
                ulRetVal = VOS_NOK;
            }
        }
    }
    else
    {
        ulRetVal = VOS_NOK;
    }

    gvarSemPost(pstGvarInfo->lAccessSemID, ATP_UTIL_SEMVALUE_FOR_WRITE);
    return ulRetVal;
}

