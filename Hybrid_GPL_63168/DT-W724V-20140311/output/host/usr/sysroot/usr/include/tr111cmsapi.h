#ifndef __TR111CMS_API_H__
#define __TR111CMS_API_H__

#if defined(SUPPORT_ATP_TR111_PART2)
typedef struct tagTR111_STATE_ST
{ 
    VOS_UINT32          ulLastNatIp;
    VOS_UINT16          usLastNatPort;
} TR111_STATE_ST;
#endif

#if defined(SUPPORT_ATP_TR111_PART1)
typedef struct tagCWMP_ASSOCDEVICE_LIST_ST
{
    VOS_CHAR    *pcOUI;                 /* OUI  */
    VOS_CHAR    *pcSN;                  /* SN   */
    VOS_CHAR    *pcProductClass;        /* Product Class */
    VOS_CHAR    *pcMacaddr;        /* Mac address */
    struct tagCWMP_ASSOCDEVICE_LIST_ST    *pstNext;
} CWMP_ASSOCDEVICE_LIST_ST;
#endif
typedef struct tagNOTIFY_ARG_ST
{
    VOS_UINT32          ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    VOS_UINT32          ulCMO;
    VOS_UINT32          ulGenTime;
    VOS_UINT32          ulPeriod;
    VOS_BOOL            bNotify;
    ATP_UTIL_TIMER_ST   **ppstTimer;
} NOTIFY_ARG_ST;
#define ATP_MAX_LANDEVICE_D             (8)




/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C"
{
#endif


#ifdef ATP_DEBUG
#define ATP_TR111_DEBUG(format, args...)           {printf("\nFILE:%s,LINE:%d,FUNC:%s: ", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("\n");}
#else
#define ATP_TR111_DEBUG(format, args...) 
#endif


#if defined(__cplusplus)
}
#endif


#ifdef SUPPORT_ATP_TR111_PART1
VOS_UINT32 ATP_TR111Part1_ParaCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                       VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                       VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                       VOS_UINT32 ulNumOfPara);

VOS_UINT32 ATP_TR111Part1_SetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                        VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                        VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);

#endif

#ifdef SUPPORT_ATP_TR111_PART2
VOS_UINT32 ATP_TR111Part2_ParaCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                       VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                       VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                       VOS_UINT32 ulNumOfPara);

VOS_UINT32 ATP_TR111Part2_SetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                        VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                        VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
#endif


VOS_VOID ATP_CFM_NotifyChangeTimer(ATP_UTIL_TIMER_ST *pstTimer, VOS_UINT32 ulNotifyLimit);

VOS_UINT32 ATP_CFM_NotifyWithLimit(
                        const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32          ulCMO,
                        VOS_UINT32          ulNotifyLimit,
                        ATP_UTIL_TIMER_ST   **ppstTimer);
						
VOS_VOID NotifyDoRealMsg(const VOS_UINT32    ulKeyList[ATP_CFM_MAX_TREE_DEPTH],
                        VOS_UINT32          ulCMO);				
			


#endif

