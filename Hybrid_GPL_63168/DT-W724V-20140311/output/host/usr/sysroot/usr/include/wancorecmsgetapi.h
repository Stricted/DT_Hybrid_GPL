#ifndef __WANCORE_CMS_GET_API_H__
#define __WANCORE_CMS_GET_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief 获取WAN协议类型API
  \param[in]  pszPath: WAN连接路径   
  \param[in]  pulProtocol: 协议类型ATP_WAN_PROTOCOL_EN
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetProtocol(const VOS_CHAR* pszPath, VOS_UINT32* pulProtocol);

#ifdef SUPPORT_ATP_MTU
/*!
  \brief 获取WAN连接MTUAPI
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulWanMtu: Mtu值
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetIpMtu(const VOS_CHAR* pszPath, VOS_UINT32* pulWanMtu);
#endif

#ifdef SUPPORT_ATP_TCP_MSS
/*!
  \brief 获取WAN连接对应的MSS 的API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulMss: MSS值
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetMSS(const VOS_CHAR* pszPath, VOS_UINT32* pulMss);
#endif

/*!
  \brief 获取WAN连接名称API
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulIntAliasLen: 长度
  \param[out]  pszIntAlias: WAN名称
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetConnInterfaceAlias(const VOS_CHAR* pszPath, VOS_UINT32 ulIntAliasLen, VOS_CHAR* pszIntAlias);

/*!
  \brief 获取WAN连接类型API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulConnType: 连接类型ATP_WAN_CONNTYPE_EN
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetConnType(const VOS_CHAR* pszPath, VOS_UINT32* pulConnType);

/*!
  \brief 获取WAN连接承载的服务类型
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulServiceList:  服务类型ATP_WAN_SERVICE_EN
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetServiceList(const VOS_CHAR* pszPath, VOS_UINT32* pulServiceList);

/*!
  \brief 获取WAN使能开关API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pucEnable: 是否使能
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetServiceEnable(const VOS_CHAR* pszPath, VOS_UINT8* pucEnable);

/*!
  \brief 获取WAN的NAT使能开关API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pucNatEnable: NAT是否使能
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetNatEnable(const VOS_CHAR* pszPath, VOS_UINT8* pucNatEnable);

/*!
  \brief 获取WAN的状态API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulStatus: WAN连接状态ATP_WAN_STATUS_EN
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatus(const VOS_CHAR* pszPath, VOS_UINT32* pulStatus);

#ifdef SUPPORT_ATP_IPV6
/*!
  \brief 获取WAN的状态API
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulStatus: WAN连接状态ATP_WAN_STATUS_EN
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatus6(const VOS_CHAR* pszPath, VOS_UINT32* pulStatus);
#endif

/*!
  \brief 判断wan是不是gre的
  \param[in]  pszPath: WAN连接路径
  \return VOS_BOOL
*/
VOS_BOOL ATP_WAN_IsGreWan(const VOS_CHAR* pszPath);

/*!
  \brief 获取WAN的接口名API
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulIfcLen:  接口名长度ATP_WAN_IFC_LEN
  \param[in]  pszInterface: WAN接口名
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInterface(const VOS_CHAR* pszPath, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);

VOS_UINT32 ATP_WAN_GetL3NasInterface(const VOS_CHAR* pszPath, VOS_UINT32 ulIfcLen, VOS_CHAR* pszL3InfName);

/*!
  \brief 获取WAN的接口IP地址
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulAddrLen: 接口IP长度ATP_WAN_ADDR_LEN
  \param[in]  pszIpAddr: WAN接口IP地址
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetIpAddr(const VOS_CHAR* pszPath, VOS_UINT32 ulAddrLen, VOS_CHAR* pszIpAddr);

/* start of AU4D00713 by d00107688 VPN无法ping通VPN对端地址 */

/*!
  \brief 获取WAN的接口默认网关和子网掩码
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulSubMaskLen: 子网掩码长度ATP_WAN_ADDR_LEN
  \param[in]  *pszSubMask: WAN接口子网掩码
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetMask(const VOS_CHAR* pszPath, VOS_UINT32 ulSubMaskLen, VOS_CHAR *pszSubMask);
/* end of AU4D00713 by d00107688 VPN无法ping通VPN对端地址 */

/*!
  \brief 获取WAN的接口默认网关
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulGateWayLen: 默认网关长度ATP_WAN_ADDR_LEN
  \param[in]  pszGateWay: WAN接口默认网关
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetGateWay(const VOS_CHAR* pszPath, VOS_UINT32 ulGateWayLen, VOS_CHAR* pszGateWay);

/*!
  \brief 获取WAN的接口DNS地址
  \param[in]  pszPath: WAN连接路径
  \param[in]  ulDnsLen: DNS地址长度ATP_WAN_ADDR_LEN
  \param[in]  pszPrimaryDns: 第一个DNS地址
  \param[in]  pszSecondDns:  第二个DNS地址
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetDns(const VOS_CHAR* pszPath, VOS_UINT32 ulDnsLen, VOS_CHAR* pszPrimaryDns, VOS_CHAR* pszSecondDns);

/*!
  \brief 获取WAN所有接口报文统计信息
  \param[in]  ulWdInstId: 第一层实例号 
  \param[in]  pstStats:  WAN所有接口收发包数
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetStatistics(const VOS_UINT32 ulWdInstId, ATP_WAN_STATS_ST* pstStats);

/*!
  \brief 校验WAN路径是否合法
  \param[in]  pszPath: WAN连接路径
  \return VOS_BOOL
  \retval 成功VOS_FALSE，失败VOS_FALSE
*/
VOS_BOOL ATP_WAN_ValidatePath(const VOS_CHAR* pszPath);

/*!
  \brief 获取ATM层和WAN层实例号
  \param[in]  pszPath: WAN连接路径
  \param[in]  pulWdInstId: 第一层DEV实例号固定为1
  \param[in]  pulWcdInstId: 第二层ATM实例号
  \param[in]  pulWanInstId: 第三层WAN实例号
  \param[in]  pulWanCmoId: 第三层WAN实例CMO
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInstIdByPath(const VOS_CHAR* pszPath, VOS_UINT32* pulWdInstId, VOS_UINT32* pulWcdInstId, VOS_UINT32* pulWanInstId, VOS_UINT32* pulWanCmoId);

/*!
  \brief 根据接口名获取WAN接口Path
  \param[in]  pacWanInterface: WAN 接口名
  \param[in]  pszPath: WAN接口 Path
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
#ifndef SUPPORT_ATP_TR181
VOS_UINT32 ATP_WAN_GetPathByInterface(const VOS_CHAR* pacWanInterface, VOS_CHAR* pszPath);
#else
//TR181相关的cms中上没有保存pszPath的全局变量，无法直接返回指向全局指针，因此需要更换API的入参形式
VOS_UINT32 ATP_WAN_GetPathByInterface(const VOS_CHAR* pacWanInterface, VOS_CHAR* pszPath, VOS_UINT32 ulLen);
#endif

/*!
  \brief 根据WAN路径查找WAN实例
  \param[in]  pszPath: WAN路径
  \return ATP_WAN_INFO_ST*
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceByPath(const VOS_CHAR* pszPath);

/*!
  \brief 此函数根据构造的WAN路径查找WAN实例，WAN路径最后是不带点的
        如无特殊需求，请优先使用上面的原始函数ATP_WAN_GetInstanceByPath
  \param[in]  pszPath: WAN路径
  \return ATP_WAN_INFO_ST*
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceByRefPath(const VOS_CHAR* pszPath);


/*!
  \brief 获得WAN链表的头指针
  \param[in] VOS_VOID: 
  \return ATP_WAN_INFO_ST*
  \retval WAN结构指针
*/
ATP_WAN_INFO_ST* ATP_WAN_GetInstanceHeader(VOS_VOID);

/*!
  \brief 根据Wanpath 获取对应的WAN的节点和实例号以及CMO
  \param[in]  pszPath: WAN连接路径
  \param[in]  *pulKeyList: id列表
  \param[in]  *pulWanCmo: WAN CMO
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetInstList(const VOS_CHAR* pszPath,  VOS_UINT32 *pulKeyList, VOS_UINT32 *pulWanCmo);

/*!
  \brief 获取ACS信息
  \param[in]  pstCurrentWanForACS: 信息结构体
  \return 
*/
void ATP_WAN_GetACSInfo(CurrentWanForACS * pstCurrentWanForACS);

VOS_UINT32 ATP_WAN_GetLinkName(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId, 
    VOS_CHAR* pszName, VOS_UINT32 ulLen);

/*!
  \brief 获取设备类型
  \param[in]  ulWdInstId: 设备实例ID
  \return VOS_UINT32设备类型
  \retval 
*/
VOS_UINT32 ATP_WAN_GetDeviceType(VOS_UINT32 ulWdInstId);

/*!
  \brief 根据WAN上行类型获取WANDevice.{i}的实例号
  \param[in]  *pcWanTypeString: 
  \param[in]  *pulWandevInstId: 
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
*/
VOS_UINT32 ATP_WAN_GetWanDevInstIdByWanType(const VOS_CHAR *pcWanTypeString, VOS_UINT32 *pulWandevInstId);

ATP_WAN_INFO_ST* ATP_WAN_GetWanNext(const ATP_WAN_INFO_ST* pstWan);
/*!
  \brief 遍历WAN连接
  \param[in]  pulIndex: WAN索引号，初始为0
  \param[in]  ulPathLen: WAN路径长度ATP_WAN_PATH_LEN
  \param[in]  pszPath: WAN路径
  \return VOS_UINT32
  \retval 成功VOS_OK，失败ATP_ERR_WAN_EN
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
  \brief 判断用户类型
  \param[in]  *pulIsFlag: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_WAN_IfSetupUser(VOS_BOOL *pulIsFlag);
VOS_UINT32 ATP_WAN_IfFactoryForFireWall(VOS_UINT32 *pulIsFlag);
#endif

#if defined(__cplusplus)
}
#endif

#endif

