/**\file wecmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author w00190448
  *\date  2011-12-20
*/

#ifndef __WE_CMS_API_H__
#define __WE_CMS_API_H__

#include "cmsapi.h"
#include "cmo_def.h"

//!��¼WAN ETHERNET״̬
#define   ATP_WANETH_STATUS_FILE_PATH      "/var/wanethstatus"
//!��·״̬
enum
{
    ATP_WANETHLINKSTATE_DOWN = 0,
    ATP_WANETHLINKSTATE_UP
};

#if defined(__cplusplus)
extern "C" {
#endif

VOS_INT32 ATP_WE_Status(VOS_VOID);

/*!
  \brief QOS����WANETH��·״̬�仯
  \param[in]  pfProc: ������
  \param[in]  ulPriority: ���ȼ�
  \param[in]  *pvNotifyHook: һ��ΪNULL
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_QosStatusChangeProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief QosLink������
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  *pvNotifyHook: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_QosLinkClearProc_NotifyRegister(AtpNotifierProc                 pfProc,
                                            						VOS_UINT32                      ulPriority,
                                            						VOS_VOID                        *pvNotifyHook);

/*!
  \brief WAN ETHERNET ��ʼ��API
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_Init (VOS_VOID);

/*!
  \brief WAN ETHERNET ȥ��ʼ��API
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_UnInit (VOS_VOID);

/*!
  \brief WAN ETHERNET״̬������������ppp����
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_VOID ATP_WE_Monitor(const ATP_MSG_HEADER_ST *pstMsg);

/*!
  \brief ���WAN�����豸������
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_WanConnDevAddMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues, VOS_UINT32 *pulSetValues, 
    const VOS_UINT32 *pulChgParas, VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId,
    VOS_BOOL bSetObj);

/*!
  \brief ɾ��WAN�����豸������
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_WanConnDevDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
    VOS_UINT32 ulObjId);

/*!
  \brief ����ʵ���Ż�ȡ��ʵ����WANETH����
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pszName: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_GetName(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, VOS_CHAR* pszName, VOS_UINT32 ulLen);

/*!
  \brief ��ȡWANETH����״̬
  \param[in]  ulWdInstId: 
  \param[in]  pszStatus: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_GetPhysicalStatus(VOS_UINT32 ulWdInstId, VOS_CHAR* pszStatus, VOS_UINT32 ulLen);

/*!
  \brief ��ȡ��·״̬
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pulStatus: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_GetLinkStatus(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId,  VOS_UINT32* pulStatus);

/*!
  \brief ��ȡWAN ETH״̬
  \param[in]  ulWdInstId: 
  \param[in]  ulWcdInstId: 
  \param[in]  pszIfc: 
  \param[in]  ulIfcLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_GetInterface(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId,  VOS_UINT32 ulWanInstId,
    VOS_CHAR* pszIfc, VOS_UINT32 ulIfcLen);

/*!
  \brief ��ȡWAN ETH�������Լ����������
  \param[in]  ulWdInstId: 
  \param[in]  pulMaxNum: 
  \param[in]  pulActionNum: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WE_GetConnNum(VOS_UINT32 ulWdInstId, VOS_UINT32* pulMaxNum, VOS_UINT32* pulActionNum);

#if 0
/*!
  \brief �ṩ����ע��ϢATP_MSG_MONITOR_EVT_LAN_ETH�Ľ���ģ��ע���ú���
  \param[in]  ulMsgType: 
  \param[in]  pfProc: 
  \param[in]  ulPriority: 
  \param[in]  **ppstMsgHdl: 
  \return 
  \retval
*/
VOS_UINT32 ATP_WE_WanEthStatusChangeProc_NotifyRegister(VOS_UINT32 ulMsgType, AtpMsgProc pfProc, VOS_UINT32 ulPriority, ATP_CMS_MSG_HDL_LIST_ST **ppstMsgHdl);
#endif

#if defined(__cplusplus)
}
#endif 

#endif
