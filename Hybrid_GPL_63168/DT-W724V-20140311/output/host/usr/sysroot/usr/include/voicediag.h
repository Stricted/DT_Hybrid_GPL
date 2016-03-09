/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : voicediag.h
  作    者  : l00126521
  版    本  : 1.0
  创建日期  : 2009-4-27
  描    述  : 语音装备测试头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2009-4-27
     作    者   : l00126521
     修改内容   : 完成初稿

*********************************************************************************/

#define VOICE_DIAG_ON             1        /*装备测试启动*/
#define VOICE_DIAG_OFF            0        /*装备测试关闭*/
#define VOICE_DIAG_DFT_ID         1        /*默认装备测试账号*/
#define VOICE_DIAG_DFT_LINEID     0        /*默认装备测试账号Line索引*/   
#define VOICE_DIAG_DFT_LOCALPORT  5061     /*默认装备测试账号端口*/
#define VOICE_DFT_SINGLEPORT_SERVER   "192.168.1.2"   /*单端口装备测试服务器地址*/
#define VOICE_DFT_MULTIPORT_SERVER    "192.168.1.1"   /*多端口装备测试服务器地址*/
#define VOICE_DIAG_SINGLEPORT     1        /*单端口*/
#define VOICE_DIAG_SINGLESERVERPORT   6052 /*单端口装备测试服务器端口*/  
#define VOICE_DIAG_DIGITMAP           "[X*#].T"       /*默认紧急号码数图*/
#define VOICE_DIAG_URGENT_INDEX       2               /*紧急数图索引*/

typedef enum tagVOICE_DIAG_TYPE_EN
{
    VOICE_SIPDIAG_ON = 0,  
    VOICE_SIPDIAG_OFF,//added by fKF39698 @20110616
    VOICE_SIPDIAG_TEST,
    VOICE_SIPDIAG_STATUS,
}VOICE_DIAG_TYPE_EN;


VOS_VOID ATP_VOICE_DiagMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 VoiceDiagSipOn( VOS_VOID);
VOS_UINT32 VoiceDiagSipOff( VOS_VOID);
VOS_UINT32 VoiceDiagSipTesterOn( VOS_VOID);
#if 0
VOS_INT32 VoiceGetDiagStatus( VOS_UINT32 *pulOutput);
#endif
