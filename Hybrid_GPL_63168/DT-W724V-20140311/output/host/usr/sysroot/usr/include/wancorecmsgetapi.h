#ifndef __WANCORE_CMS_GET_API_H__
#define __WANCORE_CMS_GET_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief ��ȡWANЭ������API
  \param[in]  pszPath: WAN����·��   
  \param[in]  pulProtocol: Э������ATP_WAN_PROTOCOL_EN
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetProtocol(const VOS_CHAR* pszPath, VOS_UINT32* pulProtocol);

#ifdef SUPPORT_ATP_MTU
/*!
  \brief ��ȡWAN����MTUAPI
  \param[in]  pszPath: WAN����·��
  \param[in]  pulWanMtu: Mtuֵ
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetIpMtu(const VOS_CHAR* pszPath, VOS_UINT32* pulWanMtu);
#endif

#ifdef SUPPORT_ATP_TCP_MSS
/*!
  \brief ��ȡWAN���Ӷ�Ӧ��MSS ��API
  \param[in]  pszPath: WAN����·��
  \param[in]  pulMss: MSSֵ
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetMSS(const VOS_CHAR* pszPath, VOS_UINT32* pulMss);
#endif

/*!
  \brief ��ȡWAN��������API
  \param[in]  pszPath: WAN����·��
  \param[in]  ulIntAliasLen: ����
  \param[out]  pszIntAlias: WAN����
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetConnInterfaceAlias(const VOS_CHAR* pszPath, VOS_UINT32 ulIntAliasLen, VOS_CHAR* pszIntAlias);

/*!
  \brief ��ȡWAN��������API
  \param[in]  pszPath: WAN����·��
  \param[in]  pulConnType: ��������ATP_WAN_CONNTYPE_EN
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetConnType(const VOS_CHAR* pszPath, VOS_UINT32* pulConnType);

/*!
  \brief ��ȡWAN���ӳ��صķ�������
  \param[in]  pszPath: WAN����·��
  \param[in]  pulServiceList:  ��������ATP_WAN_SERVICE_EN
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetServiceList(const VOS_CHAR* pszPath, VOS_UINT32* pulServiceList);

/*!
  \brief ��ȡWANʹ�ܿ���API
  \param[in]  pszPath: WAN����·��
  \param[in]  pucEnable: �Ƿ�ʹ��
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetServiceEnable(const VOS_CHAR* pszPath, VOS_UINT8* pucEnable);

/*!
  \brief ��ȡWAN��NATʹ�ܿ���API
  \param[in]  pszPath: WAN����·��
  \param[in]  pucNatEnable: NAT�Ƿ�ʹ��
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetNatEnable(const VOS_CHAR* pszPath, VOS_UINT8* pucNatEnable);

/*!
  \brief ��ȡWAN��״̬API
  \param[in]  pszPath: WAN����·��
  \param[in]  pulStatus: WAN����״̬ATP_WAN_STATUS_EN
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatus(const VOS_CHAR* pszPath, VOS_UINT32* pulStatus);

#ifdef SUPPORT_ATP_IPV6
/*!
  \brief ��ȡWAN��״̬API
  \param[in]  pszPath: WAN����·��
  \param[in]  pulStatus: WAN����״̬ATP_WAN_STATUS_EN
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatus6(const VOS_CHAR* pszPath, VOS_UINT32* pulStatus);
#endif

/*!
  \brief �ж�wan�ǲ���gre��
  \param[in]  pszPath: WAN����·��
  \return VOS_BOOL
*/
VOS_BOOL ATP_WAN_IsGreWan(const VOS_CHAR* pszPath);

/*!
  \brief ��ȡWAN�Ľӿ���API
  \param[in]  pszPath: WAN����·��
  \param[in]  ulIfcLen:  �ӿ�������ATP_WAN_IFC_LEN
  \param[in]  pszInterface: WAN�ӿ���
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInterface(const VOS_CHAR* pszPath, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);

VOS_UINT32 ATP_WAN_GetL3NasInterface(const VOS_CHAR* pszPath, VOS_UINT32 ulIfcLen, VOS_CHAR* pszL3InfName);

/*!
  \brief ��ȡWAN�Ľӿ�IP��ַ
  \param[in]  pszPath: WAN����·��
  \param[in]  ulAddrLen: �ӿ�IP����ATP_WAN_ADDR_LEN
  \param[in]  pszIpAddr: WAN�ӿ�IP��ַ
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetIpAddr(const VOS_CHAR* pszPath, VOS_UINT32 ulAddrLen, VOS_CHAR* pszIpAddr);

/* start of AU4D00713 by d00107688 VPN�޷�pingͨVPN�Զ˵�ַ */

/*!
  \brief ��ȡWAN�Ľӿ�Ĭ�����غ���������
  \param[in]  pszPath: WAN����·��
  \param[in]  ulSubMaskLen: �������볤��ATP_WAN_ADDR_LEN
  \param[in]  *pszSubMask: WAN�ӿ���������
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetMask(const VOS_CHAR* pszPath, VOS_UINT32 ulSubMaskLen, VOS_CHAR *pszSubMask);
/* end of AU4D00713 by d00107688 VPN�޷�pingͨVPN�Զ˵�ַ */

/*!
  \brief ��ȡWAN�Ľӿ�Ĭ������
  \param[in]  pszPath: WAN����·��
  \param[in]  ulGateWayLen: Ĭ�����س���ATP_WAN_ADDR_LEN
  \param[in]  pszGateWay: WAN�ӿ�Ĭ������
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetGateWay(const VOS_CHAR* pszPath, VOS_UINT32 ulGateWayLen, VOS_CHAR* pszGateWay);

/*!
  \brief ��ȡWAN�Ľӿ�DNS��ַ
  \param[in]  pszPath: WAN����·��
  \param[in]  ulDnsLen: DNS��ַ����ATP_WAN_ADDR_LEN
  \param[in]  pszPrimaryDns: ��һ��DNS��ַ
  \param[in]  pszSecondDns:  �ڶ���DNS��ַ
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetDns(const VOS_CHAR* pszPath, VOS_UINT32 ulDnsLen, VOS_CHAR* pszPrimaryDns, VOS_CHAR* pszSecondDns);

/*!
  \brief ��ȡWAN���нӿڱ���ͳ����Ϣ
  \param[in]  ulWdInstId: ��һ��ʵ���� 
  \param[in]  pstStats:  WAN���нӿ��շ�����
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatistics(const VOS_UINT32 ulWdInstId, ATP_WAN_STATS_ST* pstStats);

/*!
  \brief У��WAN·���Ƿ�Ϸ�
  \param[in]  pszPath: WAN����·��
  \return VOS_BOOL
  \retval �ɹ�VOS_FALSE��ʧ��VOS_FALSE
*/
VOS_BOOL ATP_WAN_ValidatePath(const VOS_CHAR* pszPath);

/*!
  \brief ��ȡATM���WAN��ʵ����
  \param[in]  pszPath: WAN����·��
  \param[in]  pulWdInstId: ��һ��DEVʵ���Ź̶�Ϊ1
  \param[in]  pulWcdInstId: �ڶ���ATMʵ����
  \param[in]  pulWanInstId: ������WANʵ����
  \param[in]  pulWanCmoId: ������WANʵ��CMO
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInstIdByPath(const VOS_CHAR* pszPath, VOS_UINT32* pulWdInstId, VOS_UINT32* pulWcdInstId, VOS_UINT32* pulWanInstId, VOS_UINT32* pulWanCmoId);

/*!
  \brief ���ݽӿ�����ȡWAN�ӿ�Path
  \param[in]  pacWanInterface: WAN �ӿ���
  \param[in]  pszPath: WAN�ӿ� Path
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
#ifndef SUPPORT_ATP_TR181
VOS_UINT32 ATP_WAN_GetPathByInterface(const VOS_CHAR* pacWanInterface, VOS_CHAR* pszPath);
#else
//TR181��ص�cms����û�б���pszPath��ȫ�ֱ������޷�ֱ�ӷ���ָ��ȫ��ָ�룬�����Ҫ����API�������ʽ
VOS_UINT32 ATP_WAN_GetPathByInterface(const VOS_CHAR* pacWanInterface, VOS_CHAR* pszPath, VOS_UINT32 ulLen);
#endif

/*!
  \brief ����WAN·������WANʵ��
  \param[in]  pszPath: WAN·��
  \return ATP_WAN_INFO_ST*
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceByPath(const VOS_CHAR* pszPath);

/*!
  \brief �˺������ݹ����WAN·������WANʵ����WAN·������ǲ������
        ������������������ʹ�������ԭʼ����ATP_WAN_GetInstanceByPath
  \param[in]  pszPath: WAN·��
  \return ATP_WAN_INFO_ST*
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceByRefPath(const VOS_CHAR* pszPath);


/*!
  \brief ���WAN�����ͷָ��
  \param[in] VOS_VOID: 
  \return ATP_WAN_INFO_ST*
  \retval WAN�ṹָ��
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceHeader(VOS_VOID);

/*!
  \brief ����Wanpath ��ȡ��Ӧ��WAN�Ľڵ��ʵ�����Լ�CMO
  \param[in]  pszPath: WAN����·��
  \param[in]  *pulKeyList: id�б�
  \param[in]  *pulWanCmo: WAN CMO
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInstList(const VOS_CHAR* pszPath,  VOS_UINT32 *pulKeyList, VOS_UINT32 *pulWanCmo);

/*!
  \brief ��ȡACS��Ϣ
  \param[in]  pstCurrentWanForACS: ��Ϣ�ṹ��
  \return 
*/
void ATP_WAN_GetACSInfo(CurrentWanForACS * pstCurrentWanForACS);

VOS_UINT32 ATP_WAN_GetLinkName(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, 
    VOS_CHAR* pszName, VOS_UINT32 ulLen);

/*!
  \brief ��ȡ�豸����
  \param[in]  ulWdInstId: �豸ʵ��ID
  \return VOS_UINT32�豸����
  \retval 
*/
VOS_UINT32 ATP_WAN_GetDeviceType(VOS_UINT32 ulWdInstId);

/*!
  \brief ����WAN�������ͻ�ȡWANDevice.{i}��ʵ����
  \param[in]  *pcWanTypeString: 
  \param[in]  *pulWandevInstId: 
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetWanDevInstIdByWanType(const VOS_CHAR *pcWanTypeString, VOS_UINT32 *pulWandevInstId);

ATP_WAN_INFO_ST* ATP_WAN_GetWanNext(const ATP_WAN_INFO_ST* pstWan);
/*!
  \brief ����WAN����
  \param[in]  pulIndex: WAN�����ţ���ʼΪ0
  \param[in]  ulPathLen: WAN·������ATP_WAN_PATH_LEN
  \param[in]  pszPath: WAN·��
  \return VOS_UINT32
  \retval �ɹ�VOS_OK��ʧ��ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetNext(VOS_UINT32* pulIndex, VOS_UINT32 ulPathLen, VOS_CHAR* pszPath);
VOS_UINT32 ATP_WAN_GetWanConnTrigger(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanServiceList(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanCmo(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32* pulOut);
VOS_UINT32 ATP_WAN_GetWanAddrType(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpAddr(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanInterface(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanTunnelIfcName(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6Connstatus(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanConnstatus(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIndex(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanEnable(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanPath(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanGateWay(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanDynamicDns(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanDnsServers(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanDnsOverride(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanDnsEnabled(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanMaxMtuSize(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanConnType(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_BOOL ATP_WAN_Judge_Connected(const ATP_WAN_INFO_ST* pstWan);
VOS_BOOL ATP_WAN_Contain_Servicelist(const ATP_WAN_INFO_ST* pstWan, const VOS_CHAR* pstServicelist);
VOS_BOOL ATP_WAN_Is_Same_Path(const ATP_WAN_INFO_ST* pstOldWan, const ATP_WAN_INFO_ST* pstNewWan);
VOS_CHAR* ATP_WAN_Get_Interface(ATP_WAN_INFO_ST* pstWan);
VOS_UINT32 ATP_WAN_GetWanProtocol(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
#ifdef SUPPORT_ATP_IPV6
VOS_UINT32 ATP_WAN_GetWanIpv4Enable(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6Enable(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6PrefixList(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6DynamicGateWay(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6MTUFlag(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6AddrType(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6DnsOverride(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6DynamicDns(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6DnsServers(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR *pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6Addr(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6DnsEnabled(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6GateWay(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6RaPrefix(const ATP_WAN_INFO_ST* pstWan, VOS_CHAR* pcOut, VOS_UINT32 ulLen);
VOS_UINT32 ATP_WAN_GetWanIpv6RaPrefixLen(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_UINT32 ATP_WAN_GetWanIpv6PrefixLength(const ATP_WAN_INFO_ST* pstWan, VOS_UINT32 *pulOut);
VOS_BOOL ATP_WAN_Judge_Connected_Ipv6(const ATP_WAN_INFO_ST* pstWan);
#endif

#ifdef SUPPORT_ATP_DT_WAN_PAGE
/*!
  \brief �ж��û�����
  \param[in]  *pulIsFlag: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_WAN_IfSetupUser(VOS_BOOL *pulIsFlag);
VOS_UINT32 ATP_WAN_IfFactoryForFireWall(VOS_UINT32 *pulIsFlag);
#endif

#if defined(__cplusplus)
}
#endif

#endif

