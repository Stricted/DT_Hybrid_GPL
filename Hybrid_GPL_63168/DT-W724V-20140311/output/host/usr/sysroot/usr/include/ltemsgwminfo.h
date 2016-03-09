/******************************************************************************
  版权所有  : 2007-2008，华为技术有限公司
  文 件 名  : wuprofile.h
  作    者  : l00101002
  版    本  : v1.0
  创建日期  : 2009-1-15
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2009-1-15
     作    者   : l00101002
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_LTEMSG_WMINFO_API_H__
#define __ATP_LTEMSG_WMINFO_API_H__

#ifdef ATP_DEBUG
#define ATP_LTEMSG_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_LTEMSG_DEBUG(x...)
#endif

VOS_UINT32 ATP_LTEMsgGetSysinfo(StSysinfo *pstSysinfo);
VOS_UINT32 ATP_LTEMsgGetServiceStatus(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEMsgGetServiceDomain(const StSysinfo *pstSysinfo,VOS_CHAR *pcDst,VOS_UINT32 ulLen);
VOS_UINT32 ATP_LTEMsgGetSignal(VOS_INT *iDst, VOS_UINT32 ulFlag);
VOS_UINT32 ATP_MsgGetSimcardState(VOS_CHAR  *pcSimState, VOS_UINT32 ulLen);

#endif
