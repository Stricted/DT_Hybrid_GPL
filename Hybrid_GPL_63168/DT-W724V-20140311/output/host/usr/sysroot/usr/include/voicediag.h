/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : voicediag.h
  ��    ��  : l00126521
  ��    ��  : 1.0
  ��������  : 2009-4-27
  ��    ��  : ����װ������ͷ�ļ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2009-4-27
     ��    ��   : l00126521
     �޸�����   : ��ɳ���

*********************************************************************************/

#define VOICE_DIAG_ON             1        /*װ����������*/
#define VOICE_DIAG_OFF            0        /*װ�����Թر�*/
#define VOICE_DIAG_DFT_ID         1        /*Ĭ��װ�������˺�*/
#define VOICE_DIAG_DFT_LINEID     0        /*Ĭ��װ�������˺�Line����*/   
#define VOICE_DIAG_DFT_LOCALPORT  5061     /*Ĭ��װ�������˺Ŷ˿�*/
#define VOICE_DFT_SINGLEPORT_SERVER   "192.168.1.2"   /*���˿�װ�����Է�������ַ*/
#define VOICE_DFT_MULTIPORT_SERVER    "192.168.1.1"   /*��˿�װ�����Է�������ַ*/
#define VOICE_DIAG_SINGLEPORT     1        /*���˿�*/
#define VOICE_DIAG_SINGLESERVERPORT   6052 /*���˿�װ�����Է������˿�*/  
#define VOICE_DIAG_DIGITMAP           "[X*#].T"       /*Ĭ�Ͻ���������ͼ*/
#define VOICE_DIAG_URGENT_INDEX       2               /*������ͼ����*/

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
