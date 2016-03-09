/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : firewallapi.h
  作    者  : c00126165
  版    本  : ATPV100R002C03
  创建日期  : 2008-7-1
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-11-24
     作    者   : g00121640
     修改内容   : 完成初稿

*********************************************************************************/


#ifndef __ABUSE_API_H__
#define __ABUSE_API_H__



#include "atputil.h"


#define ATP_FWFILTER_DEV_NUM_MAX    256
#define ATP_FWFILTER_TIME_NUM_MAX    16
/* BEGIN: Modified by y67514, 2013/1/17   问题单号:HOMEGW-15146 [TOI] Data Destination Ports blocked by HG for abuse detection*/
//#define EMAIL_PORT  "25,110,143,465,587,993,995"
#define EMAIL_PORT  "25,465,587"
/* END:   Modified by y67514, 2013/1/17 */

enum
{
    ABUSE_UPDATE_USER_LIST = 0,
    ABUSE_UPDATE_ALL,
    ABUSE_LTE_UPDATE_ALL,    
};

#if defined(__cplusplus)
extern "C" {
#endif
VOS_UINT32 ATP_Abuse_Init(VOS_VOID);
VOS_UINT32 AbuseIpv6start(VOS_VOID *pcWanPath, VOS_VOID *pvHook);
VOS_UINT32 Abuse_EnableSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
    VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
    VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId) ;
VOS_UINT32 ABUSE_Usr_WhiteListCheck(
    const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
    VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
    VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ABUSE_Usr_WhiteListSetMsgProc(
    const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues, VOS_UINT32 *pulSetValues, 
    const VOS_UINT32 *pulChgParas, VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, 
    VOS_BOOL bSetObj) ;
VOS_UINT32 ABUSE_Usr_WhiteListDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif

