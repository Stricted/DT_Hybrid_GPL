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

#define ATP_WAN_STR_PAP "PAP"
#define ATP_WAN_STR_CHAP "CHAP"
#define ATP_WAN_STR_CHAP_PAP "AUTO"
#define ATP_WAN_STR_NONE  "NONE"

typedef struct tagLTE_PROFILE_INFO_ST    
{
    VOS_UINT32    ulDbIndex;
    VOS_BOOL  bEnable;
    VOS_CHAR   acDescription[PROFILE_NAME_LEN+1];
    VOS_CHAR   acAPN[PROFILE_APN_LEN+1];
    VOS_CHAR   acDialNum[PROFILE_DIALNUM_LEN+1];
    VOS_CHAR   acUserName[PROFILE_USERNAME_LEN+1];
    VOS_CHAR   acPassWord[PROFILE_USERPASS_LEN+1];   
    VOS_CHAR   acAUTH[8];
    VOS_BOOL   bReadOnly;
#if defined(SUPPORT_ATP_IPV6)
    VOS_BOOL   bIpv4Enable;
    VOS_BOOL  bIpv6Enable;
#endif
/* <DTS2012072101946 w00211169 2012-7-27 begin */
#if 0
//dial mode
   VOS_CHAR   acIPAddress[16];
    VOS_CHAR   ConnectionTrigger[16];
    VOS_UINT32 IdleDisconnectTime;

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
#endif
}LTE_PROFILE_INFO_ST;

typedef struct tagLTE_PROFILE_NODE_ST
{
    LTE_PROFILE_INFO_ST stProfileEntry;
    struct tagLTE_PROFILE_NODE_ST *next;
}LTE_PROFILE_NODE_ST;

#define WU_MAX_PROFILE_NUM 128 /* DTS2013070300449 z81004143 2013/7/3 */
#define WU_WAN_DEVICE_ID_FOR_UMTS 4

#if defined(__cplusplus)
extern "C" {
#endif
LTE_PROFILE_NODE_ST *LTEGetProfileList();
LTE_PROFILE_INFO_ST * LTEGetCurrentProfile(void);/*<DTS2013091206163 xiazhongyue 20130912 modify>*/
VOS_VOID   LTESetCurrentProfile( LTE_PROFILE_INFO_ST *pstProfileNode);
VOS_UINT32  LTEProfileSet(LTE_PROFILE_INFO_ST *profileinfo);
VOS_VOID  LTEProfileGet(VOS_UINT32 ulDbProfileIndex, LTE_PROFILE_INFO_ST ** pstProfileEntry);
VOS_BOOL LTEValidateSpecialChar(const VOS_CHAR* pszValue, VOS_CHAR* pszSpecial);
VOS_BOOL LTEProfileCheckValue(const VOS_CHAR * pValue);
VOS_BOOL LTEProfileCheckDescription(const VOS_CHAR * pValue);
VOS_BOOL LTEProfileCheckAPNValue(const VOS_CHAR * pValue);
VOS_BOOL  LTEProfileCheckUserNameValue(const VOS_CHAR * pValue);
VOS_BOOL LTEProfileCheckPassWordValue(const VOS_CHAR * pValue);
VOS_BOOL LTEProfileCheckDialNumValue(const  VOS_CHAR * pValue);
VOS_BOOL LTEProfileCheckAuthProtocol(const VOS_CHAR* pszValue);
VOS_BOOL LTEProfileParaCheck(LTE_PROFILE_INFO_ST *pstPfConfVar, const VOS_UINT32 *pulProfileObj, VOS_BOOL *bInitFailFlg);
VOS_UINT32 LTEProfileInit( );
VOS_UINT32 LTEProfileRemove(VOS_UINT32 ulDbProfileIndex);
VOS_VOID LTEUpSendUpdateMsg(VOS_VOID *pVoid);
VOS_VOID LTEDownSendUpdateMsg(VOS_VOID *pVoid);
VOS_UINT32 LTEProfileInitNotify(VOS_VOID *pvVoid, VOS_VOID *pvHook);/* <DTS2013091800434 xiazhongyue 20130918 modify  > */
VOS_VOID LTEDownDelaySendUpdateMsg(VOS_VOID *pVoid);
VOS_VOID LTEUpDelaySendUpdateMsg(VOS_VOID *pVoid);
#if defined(__cplusplus)
}
#endif

#endif
