#ifndef __PPPINTF_CMS_API_DT_H__
#define __PPPINTF_CMS_API_DT_H__

#include "atputil.h"

#ifdef SUPPORT_ATP_DT_WAN_PAGE
#define DT_WAN_DEFAULT_USERNAME    "5200550445890001@setup.t-online.de"
#define DT_WAN_DEFAULT_PASSWORD    "setup"
/* BEGIN: add for DTS2013101108878 ��Hybrid��Scrat BNG Э�鲿��ʵ�� by l00214441 2013.10.11 */ 
#define DT_WAN_BNG_REALM            "@setup.t-online.de"
/* END: add for DTS2013101108878 ��Hybrid��Scrat BNG Э�鲿��ʵ�� by l00214441 2013.10.11 */ 
#endif

VOS_UINT32 ATP_WAN_IfSetupUser(VOS_BOOL *pulIsFlag);
/*���ppp wan�ӿڵ�ʵ��״̬(��var�µ�״̬�ļ�)*/
VOS_UINT32 PPPIntfGetStatus(const VOS_CHAR* acIntfName);

#endif
