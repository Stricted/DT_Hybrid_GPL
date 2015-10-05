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
#ifndef __ATP_PROFILE_API_H__
#define __ATP_PROFILE_API_H__

#include "atputil.h"
/* <DTS2012092807579 z81004485 2012/10/8 begin  */
#define PROFILE_NAME_LEN           (64)
#define PROFILE_USERNAME_LEN       (64)         /* ATP_WAN_PPP_USER_LEN */
#define PROFILE_USERPASS_LEN       (64)         /* ATP_WAN_PPP_PASSWD_LEN */
/* DTS2012092807579 z81004485 2012/10/8 end> */
/*<DTS2010072102408  l129990 20100724  modify begin*/
#define PROFILE_APN_LEN            64
/*DTS2010072102408  l129990 20100724  modify end>*/
#define PROFILE_DIALNUM_LEN        32
#define PROFILE_ZERO_LEN           0
#define PROFILE_VISIBLE_KEY_MIN    32          /*old num is 33*/
#define PROFILE_VISIBLE_KEY_MAX    127 
#define PROFILE_EQUAL_MARK  61                 /* = */
#define PROFILE_AND_MARK  38                   /* &  */
#define PROFILE_DOUBLE_QUOTATION_MARK 34       /* " */
#define PROFILE_BACKSLASH_MARK  92             /* \ */
/*<DTS2012021701386 ADDED by chichao ,2012.2.21  begin*/
#if defined SUPPORT_ATP_PLMN_AUTO_PROFILE
#define PROFILE_PLMNMATCHED 64
#define PLMNLEN 7
#define EN_PLMN_FILE "/tmp/plmnnum.txt"
#define PLMN_ADDED_BY_USER "00000"
#endif
/*DTS2012021701386 ADDED by chichao ,2012.2.21  end>*/


/* DTS2011051903049.CMS: mayang 2011-5-25 DEL 4 Lines, move to wanconfig.h */
/*<DTS2010072102934  l129990 20100724  modify begin*/
#define PROFILE_NOT_SUPPORT_CHAR_COMMA 44      /* , */
#define PROFILE_NOT_SUPPORT_CHAR_COLON 58      /* : */
#define PROFILE_NOT_SUPPORT_CHAR_SEMICOLON 59  /* ; */
/*DTS2010072102934  l129990 20100724  modify end>*/
#define MACRO_SA  97                  //a
#define MACRO_SZ  122                    // z
#define MACRO_BA  65                  //A
#define MACRO_BZ  90                    // Z
#define MACRO_0  48                 //0
#define MACRO_9  57                    // 9
#define MACRO_SINGLE_QUOTATION_MARK  45       // -
#define MACRO_STOP_MARK  46               // .

typedef struct tagWU_PROFILE_INFO_ST    
{
    VOS_UINT32    ulDbIndex;
    /* <DTS2010112205010 : zhangqifeng 2010-11-30 ADDED BEGIN */
    VOS_CHAR   acProfileName[PROFILE_NAME_LEN+1];
    VOS_CHAR   acAPN[PROFILE_APN_LEN+1];
    VOS_CHAR   acDialNum[PROFILE_DIALNUM_LEN+1];
    VOS_CHAR   acUserName[PROFILE_USERNAME_LEN+1];
    VOS_CHAR   acPassWord[PROFILE_USERPASS_LEN+1];
    /* <DTS2010112205010 : zhangqifeng 2010-11-30 ADDED END */
    VOS_CHAR   acIPAddress[16];
    VOS_CHAR   acAUTH[8];
    VOS_BOOL   bReadOnly;
/* <DTS2012072101946 w00211169 2012-7-27 begin */
#if 1
//dial mode
    VOS_CHAR   ConnectionTrigger[16];
    VOS_UINT32 IdleDisconnectTime;
#endif
/* DTS2012072101946 w00211169 2012-7-27 end> */

/* <DTS2012021701386 ADDED by chichao ,2012.2.21 begin */ 
#if defined(SUPPORT_ATP_SAME_PROFILE_NAME)
    VOS_CHAR PlmnMatched[16];
#endif
/* DTS2012021701386 ADDED by chichao ,2012.2.21  end> */

/* <DTS2010120401310.QMI: gKF34687 2010-11-26 ADD BEGIN */
#if defined(SUPPORT_ATP_MTU)
    VOS_UINT32 pppmtu;
#endif
/* DTS2010120401310.QMI: gKF34687 2010-11-26 ADD END> */

/* <DTS2012072301465 w00211169 2012-8-22 begin */
#if defined(SUPPORT_ATP_IPV6)
    VOS_UINT32 ulXIpv4Enable;
    VOS_UINT32 ulXIpv6Enable;
#endif
/* DTS2012072301465 w00211169 2012-8-22 end> */
}WU_PROFILE_INFO_ST;

typedef struct tagWU_PROFILE_NODE_ST
{
    WU_PROFILE_INFO_ST stProfileEntry;
    struct tagWU_PROFILE_NODE_ST *next;
}WU_PROFILE_NODE_ST;

#define WU_MAX_PROFILE_NUM 128 /* DTS2013070300449 z81004143 2013/7/3 */
#define WU_WAN_DEVICE_ID_FOR_UMTS 4

#if defined(__cplusplus)
extern "C" {
#endif

extern WU_PROFILE_NODE_ST* g_pstProfileList;

VOS_UINT32 WuGetProfileByName(const VOS_CHAR * name , WU_PROFILE_INFO_ST*  pstProfileEntry); /*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileSet(const WU_PROFILE_INFO_ST *profileinfo);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileRemove(VOS_UINT32 ulDbProfileIndex);
VOS_UINT32 WuProfileGet(VOS_UINT32 ulWdInstId,VOS_UINT32 ulDbProfileIndex, WU_PROFILE_INFO_ST ** pstProfileEntry);
VOS_UINT32 WuGetProfileEntryNums();
/* <DTS2012021701386 ADDED by chichao ,2012.2.21 begin */
#if defined(SUPPORT_ATP_SAME_PROFILE_NAME)
VOS_UINT32 WuProfileCheckProfilePlmnValue(const VOS_CHAR *pValue);
VOS_UINT32 GetProfileByNameAndPlmn(const VOS_CHAR * name ,WU_PROFILE_INFO_ST*  pstProfileEntry); 
#endif
/* DTS2012021701386 ADDED by chichao ,2012.2.21  end> */
VOS_UINT32 WuProfileCheckProfileNameValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckAPNALLValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckUserNameValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckPassWordValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckDialNumValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckIpAddressValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckStrValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_UINT32 WuProfileCheckAPNValue(const VOS_CHAR * pValue);/*LINT.WEB l00101002 20100324*/
VOS_BOOL WuProfileCheckAuthProtocol(const VOS_CHAR* pszValue); /* DTS2013052706724 z81004143 2013/5/28 */

/* <DTS2012072101946 w00211169 2012-7-28 begin */
VOS_BOOL WanValidateIdleTime(const VOS_UINT32 ulValue);
VOS_UINT32  syncCurrProfileList(const WU_PROFILE_INFO_ST* curProFileInfo,VOS_BOOL bDMatch, VOS_BOOL bWanInit);
VOS_UINT32  syncCurrProfileListDel(const WU_PROFILE_INFO_ST* curProFileInfo,const WU_PROFILE_INFO_ST *nextProFileInfo);
/* DTS2012072101946 w00211169 2012-7-28 end> */

/* <DTS2012072301465 w00211169 2012-8-23 begin */
VOS_BOOL WanValidateIpType(const VOS_UINT32 ulValue);
/* DTS2012072301465 w00211169 2012-8-23 end> */

/* <DTS2010120401310.QMI: gKF34687 2010-11-26 ADD BEGIN */
#if defined(SUPPORT_ATP_MTU)
VOS_BOOL WanValidateIdleMtu(const VOS_UINT32 ulValue);
#endif
/* DTS2010120401310.QMI: gKF34687 2010-11-26 ADD END> */
/*<DTS2012022903956 c00121606  2012-2-25 begin*/
#if defined(SUPPORT_ATP_RETRY_APN)
VOS_UINT32 WuGetNextProfileByName(const VOS_CHAR * name , const VOS_CHAR *plmnnub , WU_PROFILE_INFO_ST* pstProfileEntry); /*LINT.WEB l00101002 20100324*/
#endif 
/*<DTS2012022903956 c00121606  2012-2-25 end*/
/* <DTS2013051001477DTS2013050909500  20130517 xiazhongyue begin */
VOS_BOOL WuProfileCheckDailmodeValue(const VOS_CHAR * pValue);
/* <DTS2013051001477DTS2013050909500  20130517 xiazhongyue end> */
#if defined(__cplusplus)
}
#endif

#endif
