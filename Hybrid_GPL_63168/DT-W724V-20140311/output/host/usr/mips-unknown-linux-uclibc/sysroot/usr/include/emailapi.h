/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : emailapi.h
  ��    ��  : l00126521
  ��    ��  : 1.0
  ��������  : 2011-09-17
  ��    ��  : Email���ýӿ�ʵ��ͷ�ļ�
  �����б�  :
*********************************************************************************/
#define EMAIL_STR_2_LEN                     3
#define EMAIL_STR_16_LEN                    16
#define EMAIL_STR_32_LEN                    32
#define EMAIL_STR_64_LEN                    64
#define EMAIL_STR_256_LEN                   256
#define EMAIL_STR_512_LEN                   512


VOS_UINT32 ATP_EMAIL_Init( VOS_VOID );
VOS_UINT32 ATP_EMAIL_UnInit( VOS_VOID );
VOS_VOID   ATP_EMAIL_UpdateCfg(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID   ATP_EMAIL_GetEmailNoticeMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID   ATP_EMAIL_GetEmailResMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID   ATP_EMAIL_GetEmailErrMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID   ATP_EMAIL_DelMail(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 Email_RecordWanStatus(VOS_BOOL bIpV4, VOS_BOOL bStatus);


