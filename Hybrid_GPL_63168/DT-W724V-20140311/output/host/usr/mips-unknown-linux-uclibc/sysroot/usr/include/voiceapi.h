/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : voiceapi.h
  作    者  : l00126521
  版    本  : 1.0
  创建日期  : 2009-11-26
  描    述  : 语音配置接口实现头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2009-11-26
     作    者   : l00126521
     修改内容   : 完成初稿

*********************************************************************************/
typedef enum tagVOICE_DECT_MSG_TYPE_EN
{
    CMS_VOICE_DECT_REG = 1,          /*子机注册操作*/
    CMS_VOICE_DECT_RING,             /*子机呼叫操作*/
    CMS_VOICE_DECT_NAME,             /*子机名称下发*/
    CMS_VOICE_REG_STATUS,            /*SIP注册状态获取*/
    CMS_VOICE_DECT_INCOM,            /*子机呼入关联配置下发*/
    CMS_VOICE_DECT_OUTGO = 6,        /*子机呼出关联配置(包括CLIR使能)下发*/
    CMS_VOICE_DECT_CW,               /*子机呼叫等待*/
    CMS_VOICE_MSN_AUTODETECT,        /*自动检测MSN号码*/
    CMS_VOICE_MSN_AUTOGET,           /*获取自动获取的MSN号码*/
    CMS_VOICE_DECT_GETREG,           /*获取是否有子机注册信息*/
    CMS_VOICE_DECT_STOPREG = 11,     /*页面终止对码时，终止对码流程*/
    CMS_VOICE_FXO_STATUS,            /*FXO口装态获取*/
    CMS_VOICE_DECT_ONLBOOK,
    CMS_VOICE_DECT_EMAIL,
    CMS_VOICE_DECT_RSS,
    CMS_VOICE_DECT_RESET = 16,
    CMS_VOICE_DECT_REFRESH,
    
    CMS_VOICE_DECT_BUTT,
}VOICE_DECT_MSG_TYPE_EN;

// line settings list fields
#define ATP_LSL_FLD_LINE_NAME               1
#define ATP_LSL_FLD_LINE_ID                 2
#define ATP_LSL_FLD_ATTACHED_HANDSETS       3
#define ATP_LSL_FLD_DIALING_PREFIX          4
#define ATP_LSL_FLD_FP_MELODY               5
#define ATP_LSL_FLD_FP_VOLUME               6
#define ATP_LSL_FLD_BLOCKED_NUMBER          7
#define ATP_LSL_FLD_MULTI_CALL_MODE         8
#define ATP_LSL_FLD_INTRUSION_CALL          9
#define ATP_LSL_FLD_PERMANENT_CLIR          10
#define ATP_LSL_FLD_CALL_FORWARDING         11
#define ATP_LSL_FLD_CALL_FNR                12
#define ATP_LSL_FLD_CALL_FB                 13
VOS_UINT32 ATP_VOICE_Init( VOS_VOID );
VOS_UINT32 ATP_VOICE_UnInit( VOS_VOID );
VOS_VOID ATP_VOICE_GMsgProc(VOS_VOID *pvGMsg, VOS_UINT32 ulSubType);
VOS_UINT32 ATP_VOICE_ServiceGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_ServiceCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ServiceAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_CommonCfgCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CommonCfgSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId) ;
VOS_UINT32 ATP_VOICE_RTPCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_RTPSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_PhyInterfaceGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_PhyInterfaceFXSGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_PhyInterfaceCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_PhyInterfaceAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_PhyInterfaceDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_ProviderGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_ProviderCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ProviderAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ProviderDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_ProfileGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_ProfileCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ProfileAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ProfileDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_ProfileSIPCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_ProfileSIPSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_LineGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_LineAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_LineDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_LineSipGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineSIPCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineSIPSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_LineCodecListCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_LineCodecListAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_LineCodecListDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_FixedSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_FixedLineAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_DectMobileGetInstMsgProc(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, VOS_UINT32 *pulInstValues,
                            VOS_UINT32 ulArrayLen, VOS_UINT32 *pulNumOfInst);
VOS_UINT32 ATP_VOICE_DectMobileGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_DectCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_DectSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_DectMobileCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);							
VOS_UINT32 ATP_VOICE_DectMobileAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_DectMobileDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_TelBookAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_TelBookDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_VOID ATP_VOICE_DectMobileMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectRebootMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_SaveDectRfpicodeProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 ATP_VOICE_SIPLineGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_SIPLineCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_SIPLineAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_CodecListCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_CodecListAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_CodecListDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_ExtenProfileCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ExtenProfileAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ExtenProfileDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_CalControlCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CalControlSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_IncomingMapCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_IncomingMapAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_IncomingMapDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_OutgoingMapCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_OutgoingMapAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_OutgoingMapDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_NumberingPlanCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_NumberingPlanAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_NumberingPlanDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
VOS_UINT32 ATP_VOICE_ExtensionCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ExtensionAddSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
VOS_UINT32 ATP_VOICE_ExtensionDelProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);

/**************h00190880*****************/
//#define VDB_DB_STATUS           "/var/vdb.status"
//#define VDB_VDB_WRITE_STATUS    "/var/vdb.write"

VOS_VOID ATP_VOICE_LEDStatusInfoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectFwUpdateMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
/*BEGIN:added by l00192646,2012/07/04  for 时间同步*/
VOS_VOID ATP_VOICE_DectTimeUpdate(const ATP_MSG_HEADER_ST *pstMsg);
/*END:added by l00192646,2012/07/04  for 时间同步*/
VOS_VOID ATP_VOICE_CWInfoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectNemoModeUpdateMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
//VOS_VOID ATP_VOICE_FXOStatusMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_CFInfoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_WanWillDownMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_LEDInfoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
/**************h00190880*****************/
VOS_VOID ATP_VOICE_SaveVDBMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_AutoMsnMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
#if 0
VOS_UINT32 ATP_VOICE_LineSessionGetInstMsgProc(
                            const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, VOS_UINT32 *pulInstValues,
                            VOS_UINT32 ulArrayLen, VOS_UINT32 *pulNumOfInst);
VOS_UINT32 ATP_VOICE_LineSessionGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
#endif
VOS_UINT32 ATP_VOICE_LineStatusGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineStatusCheckProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                              VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineStatusSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_LineCallingFeaturesSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);							
VOS_UINT32 ATP_VOICE_LineCallingFeaturesGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_LineCallingFeaturesCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_ExtensionCallingFeaturesSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_ExtensionCallingFeaturesCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
/*BEGIN: Added by l00180792 @20120627 for wt-104 calllog*/
VOS_VOID ATP_VOICE_SaveCallLogToDB(const ATP_MSG_HEADER_ST *pstMsg);
/*END: Added by l00180792 @20120627 for wt-104 calllog*/
VOS_UINT32 ATP_VOICE_CallLogGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CallLogSignallingPerformanceGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CallLogSessionGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CallLogSessionCodecGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CallLogSessionRTPGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_CallLogSessionVoiceQualityGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_EmailTreeCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_EmailTreeSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_RssIntervalCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                          VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                          VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                          VOS_UINT32 ulNumOfPara);
VOS_UINT32 ATP_VOICE_RssIntervalSetProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId);
VOS_UINT32 ATP_VOICE_DectEmailGetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                              VOS_UINT32 ulObjId, VOS_UINT32 *pulGetdValues,
                              const VOS_UINT32 *pbFocusParas,
                              VOS_UINT32 ulNumOfPara);

/*BEGIN:added by l00192646,2012/05/23,for pc-lint*/
VOS_VOID ATP_VOICE_DectRegOpen(const ATP_MSG_HEADER_ST *pstMsg);
/*END:added by l00192646,2012/05/23,for pc-lint*/
VOS_VOID ATP_VOICE_StopDectReg(const ATP_MSG_HEADER_ST *pstMsg);//Start by z00182276 for UDK0000340477,在结束wps时同时终止dect对码流程 at 2014.2.14
VOS_VOID ATP_VOICE_DectRegStop(VOS_VOID);
VOS_VOID VoiceDectRegTimeOut(VOS_VOID *pvArg);
/*BEGIN:added by l00192646,2012/05/19  for dect search(paging)硬件按键适配*/
VOS_VOID ATP_VOICE_DectSearch(const ATP_MSG_HEADER_ST *pstMsg);
/*END:added by l00192646,2012/05/19  for dect search(paging)硬件按键适配*/
VOS_VOID ATP_VOICE_VDBRestore(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_InitSaveMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_SetVadMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_UINT32 ATP_VOICE_GetIfReg(VOS_VOID);
VOS_UINT32 ATP_VOICE_PreGlobleCheck();
VOS_UINT32 ATP_VOICE_GlobleCheck();
VOS_BOOL ATP_VOICE_IfNeedSaveVdb(VOS_VOID);
VOS_VOID ATP_VOICE_DectLineSetMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectNameUpdateMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectPINUpdateMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectSubsDataSaveMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_WebDectCfgUpdateMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
/* BEGIN: Added by zengjun 182276, 2011/6/2   DT快速拨号特性*/
VOS_VOID ATP_VOICE_ErasureCalllistMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
/* END:   Added by zengjun 182276, 2011/6/2 */
/*BEGIN:added by l00192646,2012/06/02,for wt-104 DECT*/
VOS_VOID ATP_Voice_SaveHandsetVersionProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_Voice_SuotaSuccessProc(const ATP_MSG_HEADER_ST *pstMsg);
/*END:added by l00192646,2012/06/02,for wt-104 DECT*/
VOS_VOID ATP_VOICE_DectCFInfoMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_DectBaseResetMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID ATP_VOICE_WebDectUnregMsgProc(const ATP_MSG_HEADER_ST *pstMsg);

