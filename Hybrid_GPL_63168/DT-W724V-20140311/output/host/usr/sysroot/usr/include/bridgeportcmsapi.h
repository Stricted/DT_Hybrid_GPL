#ifndef __BRIDGEPORTCMS_API_H__
#define __BRIDGEPORTCMS_API_H__

#ifdef ATP_DEBUG
#define ATP_BRPORT_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_BRPORT_DEBUG(x...)
#endif

#ifdef SUPPORT_ATP_BR_IGMP_SNOOP
#define ATP_BRIDGE_IGMPSNOOPING_BLOCK_FLAG      2
#endif
#define ATP_BRIDGE_IGMPSNOOPING_ENABLE_FLAG     1
#define ATP_BRIDGE_IGMPSNOOPING_DISABLE_FLAG    0

/*!
  \brief ����IGMP Snooping����
  \param[in]  bEnable: IGMP Snooping����ѡ�"enable"-->ʹ�ܣ�"disable"-->����
  \return 
  \retval ����IGMP Snooping���ܳɹ��򷵻�VOS_OK�� 
              ����IGMP Snooping����ʧ���򷵻ش�����ATP_ERR_BRIDGE_ENABLEIGMPSNOOPING_EN
*/

VOS_UINT32 ATP_BRIDGE_EnableIgmpSnooping(VOS_BOOL bEnable);

/*!
  \brief ����Eth�ڵ����ֵõ��������ŵ�����
  \param[in]  *pszInfName: Eth�ӿ���
  \param[out]  plRetBridgeIntfName: �Žӿ���
  \param[in]  iBrLen: �Žӿ�������
  \return �ɹ�����VOS_OK, ʧ�ܷ���VOS_NOK
*/


     /*!
      \brief �жϴ����psBrDomain �������Ƿ���һ���Ϸ�����ʵ��
      \param[in] psBrDomain: Ҫ�������domain
      \param[out]  
      \return   ���������domain ���ŷ���vos_true   
    */
VOS_BOOL BridgePortIsValidBridgeDomain(const VOS_CHAR *psBrDomain);

    /*!
      \brief �жϴ����psPortDomain ����Port�Ƿ���һ���Ϸ���Portʵ��
      \param[in] psPortDomain: Ҫ�����Port domain
      \param[out]  
      \return   ���������domain ��Port����vos_true   
    */
                            
VOS_BOOL BridgePortIsValidPortDomain(const VOS_CHAR *psPortDomain);
          
    /*!
      \brief �жϴ����psPortDomain ��,��ȡ����ŵĵ�ǰ״̬
      \param[in] psPortDomain: Ҫ�����Port domain
      \param[out]  
      \return  ����Port enable״̬
    */
VOS_BOOL BridgePortGetBrStatus(const VOS_CHAR *psBrDomain);

    /*!
      \brief �жϴ����psPortDomain ��,��ȡ���Port ��Ӧ��Interface, ����InterfaceName(VOS_CHAR *)  �������interface
      \param[in] psPortDomain: Ҫ�����Port domain
      \param[out]  
      \return  ����bridge enable״̬
    */
VOS_UINT32 BridgePortGetInterfaceByPortDomain(const VOS_CHAR *psPortDomain,VOS_CHAR *psInterface,VOS_UINT32 ulInterfaceLen);

VOS_UINT32 ATP_BRIDGE_FilterInit(VOS_VOID);

VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfaceName(const VOS_CHAR *pszInfName, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);

#endif

