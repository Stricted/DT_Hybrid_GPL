/**\file wucmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author W00190448
  *\date  2011-12-23
*/

#ifndef __WU_CMS_API_H__
#define __WU_CMS_API_H__

//!��·״̬
enum
{
    ATP_WANUMTSLINKSTATE_DOWN = 0,
    ATP_WANUMTSLINKSTATE_UP
};


/********************** Global APIs Definitions *****************************/

#if defined(__cplusplus)
extern "C" {
#endif

//!WANUMTS��ʼ��API
VOS_UINT32 ATP_WU_Init (VOS_VOID);

//!WANUMTSȥ��ʼ��API
VOS_UINT32 ATP_WU_UnInit (VOS_VOID);

//!WANUMTS���ӳ���API
VOS_VOID ATP_WU_Monitor(const ATP_MSG_HEADER_ST *pstMsg);

//!���WCD������
VOS_UINT32 ATP_WU_WanConnDevAddMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues, VOS_UINT32 *pulSetValues, 
    const VOS_UINT32 *pulChgParas, VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId,
    VOS_BOOL bSetObj);

//!ɾ��WCD������
VOS_UINT32 ATP_WU_WanConnDevDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId);    

//!��ȡWAN UMTS��·��
VOS_UINT32 ATP_WU_GetName(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, VOS_CHAR* pszName, VOS_UINT32 ulLen);

//!��ȡWAN UMTS�����״̬
VOS_UINT32 ATP_WU_GetPhysicalStatus(VOS_UINT32 ulWdInstId, VOS_CHAR* pszStatus, VOS_UINT32 ulLen);

//!��ȡWAN UMTS��·״̬
VOS_UINT32 ATP_WU_GetLinkStatus(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId,  VOS_UINT32* pulStatus);

//!��ȡWAN UMTS��·�ӿ�
VOS_UINT32 ATP_WU_GetInterface(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, 
    VOS_CHAR* pszIfc, VOS_UINT32 ulIfcLen);

//!��ȡWAN UMTS��·������������͵�ǰ������
VOS_UINT32 ATP_WU_GetConnNum(VOS_UINT32 ulWdInstId, VOS_UINT32* pulMaxNum, VOS_UINT32* pulActionNum);


#if defined(__cplusplus)
}
#endif



#endif
