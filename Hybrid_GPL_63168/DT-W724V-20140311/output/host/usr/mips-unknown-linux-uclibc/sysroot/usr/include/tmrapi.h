/*****************************************************************************
 *               Copyright (C) 2008, Huawei Tech. Co., Ltd.               
 *                           ALL RIGHTS RESERVED                               
 * FileName: tmrapi.h                                                          
 * Version: 1.0                                                                
 * Description:  the apis of timer.    
 *                                                                             
 * History:                                                                    
 * 1. 2009-02-06, p42047 Create this file.                                  
******************************************************************************/

#ifndef __TMR_API_H__
#define __TMR_API_H__

/*****************************************************************************/
/*  本文件需要包含的其他头文件依赖                                           */
/*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/******************************************************************************/
/*************/

#ifdef ATP_DEBUG
#define TMR_DEBUG(format, args...)  {printf("TMR DEBUG============file:%s, line:%d ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define TMR_DEBUG(format, args...)
#endif

typedef enum tagATP_TMR_TIMER_MODE
{
    ATP_TMR_TIMER_LOOP,         // the timer start and timeout loop
    ATP_TMR_TIMER_ONE_SHOOT     // the timer start ONE timer and stop
}ATP_TMR_TIMER_MODE;

typedef enum tagATP_TMR_E_ERR
{
    ATP_TMR_E_ERR_INVALID_PARA = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_TIMER),
    ATP_TMR_E_ERR_MEM,
    ATP_TMR_E_ERR_SEM,
    ATP_TMR_E_ERR_TMR_CREATE,
        
}ATP_TMR_E_ERR;

typedef enum tagATP_TMR_E_LINK_TYPE
{
    ATP_TMR_E_ZEROLINK,
    ATP_TMR_E_SORTLINK,
}ATP_TMR_E_LINK_TYPE;

typedef VOS_VOID* ATP_TIMER_ID;

#define TICKS_PER_SECOND    (100)
#define MSEC_PER_TICK       (1000 / TICKS_PER_SECOND)
#define USEC_PER_TICK       (1000 * MSEC_PER_TICK)
#define USEC_PER_SECOND     (1000000)

/* size of relative timer hash table, set it from 0 to 31 */
#define PERELTM_HASH_BITS 10
#define TM_HASH_LEN (1 << PERELTM_HASH_BITS)
#define TM_HASH_MASK (TM_HASH_LEN - 1)
#define TM_TMLEN_MASK (~TM_HASH_MASK)
#define TM_HASH_FUNC(value) ((value) & TM_HASH_MASK)

#define TM_NAME_LEN (12)

#define TM_MAX_TM_NUM   (1024)

#define TIME_ELAPSED_US(after, before) \
    (((after).tv_sec - (before).tv_sec) * USEC_PER_SECOND \
            + (after).tv_usec - (before).tv_usec)

/*=================list operation set================*/
/** The struct of a list item*/
struct list_head {
 	struct list_head * next, *prev;
};

/** init the list item of list head */
#define list_init(head) /*lint -save -e717*/do { \
    (head)->next = (head)->prev = (head); \
}while(0)/*lint -restore*/

/** Add an item to a list after a special location */
#define list_add(item, where) /*lint -save -e717*/do { \
       (item)->next = (where)->next; \
       (item)->prev = (where); \
       (where)->next = (item); \
       (item)->next->prev = (item); \
}while(0)/*lint -restore*/

/** Add an item to a list before a special location */
#define list_add_before(item, where) list_add(item, (where)->prev)

/** remove an item from a list */
#define list_remove(item) /*lint -save -e717*/do { \
        (item)->prev->next = (item)->next; \
        (item)->next->prev = (item)->prev; \
}while(0)/*lint -restore*/

/** true if the list is empty */
#define list_isempty(head) ((head)->next == (head))

/** true if an item is just init and wasn't added to the queue */
#define list_isinqueue(item) ((item)->next != (item))

/** traval a list, but you cannot remove item while travaling */
#define for_each_list_item(item, head) \
        for(item=(head)->next; item!=head; item = (item)->next)

/** traval a list, you can remove item while travaling */
#define for_each_list_item_safe(item, temp, head)\
        for( (item) = (head)->next, (temp) = (item)->next; \
             (item) != (head); \
             (item) = (temp), (temp) = (item)->next )

/** get the entry by list field */
#define list_entry(item, type, member)\
        ((type *)((char *)item - (char *)(&((type *)0)->member)))
/*=================list operation set END================*/

typedef VOS_VOID(*ATP_TimerCallback)(VOS_VOID* pvArg, ATP_TIMER_ID hTimerID, VOS_CHAR* pszTimerName);

typedef struct tagATP_TMR_HOOK {
    struct tagATP_TMR_HOOK* pstNext;

    VOS_VOID (*pfHook)(VOS_INT32);
    VOS_INT32 lInterval;
    VOS_INT32 lEscape;
}ATP_TMR_HOOK;

typedef struct tagATP_TMR_TASKLET {
    struct list_head stHead;
#ifdef TIMER_LOCK_USE_SEM
    VOS_INT32 lLock;
#else
    pthread_mutex_t lLock;
#endif
}ATP_TMR_TASKLET;

typedef struct tagATP_TMR_TASKLET_ITEM {
    struct list_head stList;
    VOS_VOID (*pfTask)(VOS_VOID*);
    VOS_VOID* pvArg;
}ATP_TMR_TASKLET_ITEM;

typedef struct tagATP_TMR_HASH
{
    struct list_head stZeroLink;
    struct list_head stSortLink;
}ATP_TMR_HASH;

typedef struct tagATP_TMR_TIMER_CB
{
    struct list_head list;
    VOS_UINT32 ulExpireMs;      /* expire time in ms */
    VOS_UINT32 ulTickLeft;      /* time passed, used in hash link */
    ATP_TimerCallback pfTimeout;
    VOS_VOID* pvArg;
    VOS_UINT32 ulMode;          /* ATP_TMR_TIMER_LOOP, ATP_TMR_TIMER_ONE_SHOOT */
    VOS_UINT32 ulLink;          /* RELTM_LINK_ZERO or RELTM_LINK_SORTED */
	VOS_UINT32 ulIndex;         /* index in timer array */
    VOS_CHAR acTimerName[TM_NAME_LEN];     /* timer name */
}ATP_TMR_TIMER_CB;

typedef struct tagATP_TMR_TIMER_INFO
{
    ATP_TMR_TIMER_CB* pstTimer;
    VOS_UINT32 ulTimeout;
    VOS_VOID* pvArg;
}ATP_TMR_TIMER_INFO;


/*****************************************************************************
 函 数 名  : ATP_XML_GetVersion
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 返回组件版本号

 修改历史      :
  1.日    期   : 2009年4月20日
    作    者   : 俞佑清42304
    修改内容   : 新生成函数

*****************************************************************************/
const VOS_CHAR * ATP_TIMER_GetVersion(void);

/******************************************************************************
  函数名称  : ATP_TMR_InitTimer
  功能描述  : Timer initial API, invoke just ONE time when process initial.
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern VOS_UINT32 ATP_TMR_InitTimer();

/******************************************************************************
  函数名称  : ATP_TMR_DeInitTimer
  功能描述  : Timer deinitial API.
  输入参数  : 
              无
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern VOS_UINT32 ATP_TMR_DeInitTimer();

/******************************************************************************
  函数名称  : ATP_TMR_StartTimer
  功能描述  : Get and start a timer.
  输入参数  : 
              1.  *pulTimerID:
              2.  ulSleepMs:
              3.  pCallback:
              4.  pPara:
              5.  enTimeMode:
              6.  pszTimeName:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern VOS_UINT32 ATP_TMR_StartTimer(ATP_TIMER_ID          *pulTimerID,
                                 VOS_UINT32            ulTimeout,
                                 ATP_TimerCallback     pfCallback,
                                 const VOS_VOID*       pvArg,
                                 ATP_TMR_TIMER_MODE    enTimeMode,
                                 const VOS_CHAR*       pszTimerName);

/******************************************************************************
  函数名称  : ATP_TMR_GetTimer
  功能描述  : Get a timer but not start it.
  输入参数  : 
              1.  pszTimerName:
              2.  ulTimeout:
              3.  pfCallback:
              4.  pvArg:
              5.  enTimeMode:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern ATP_TIMER_ID ATP_TMR_GetTimer(const VOS_CHAR*   pszTimerName, 
                              ATP_TimerCallback pfCallback,
                              const VOS_VOID*   pvArg,
                              ATP_TMR_TIMER_MODE    enTimeMode);

/******************************************************************************
  函数名称  : ATP_TMR_StartTimerByID
  功能描述  : Start or restart a timer.
  输入参数  : 
              1.  ulTimerID:
              2.  ulTimeout:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern VOS_UINT32 ATP_TMR_StartTimerByID(ATP_TIMER_ID ulTimerID, VOS_UINT32 ulTimeoutMs);

/******************************************************************************
  函数名称  : ATP_TMR_StopTimer
  功能描述  : Stop a timer.
  输入参数  : 
              1.  ulTimerID:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-2-6
     作    者   : p42047
     修改内容   : 完成初稿

******************************************************************************/
extern VOS_UINT32 ATP_TMR_StopTimer(ATP_TIMER_ID ulTimerID, VOS_BOOL bFreeTimer);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif  /* __TMR_API_H__ */


