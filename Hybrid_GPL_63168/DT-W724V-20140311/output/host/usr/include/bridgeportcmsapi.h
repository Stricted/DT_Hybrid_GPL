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
  \brief 开关IGMP Snooping功能
  \param[in]  bEnable: IGMP Snooping开关选项。"enable"-->使能；"disable"-->禁用
  \return 
  \retval 开关IGMP Snooping功能成功则返回VOS_OK， 
              开关IGMP Snooping功能失败则返回错误码ATP_ERR_BRIDGE_ENABLEIGMPSNOOPING_EN
*/

VOS_UINT32 ATP_BRIDGE_EnableIgmpSnooping(VOS_BOOL bEnable);

/*!
  \brief 根据Eth口的名字得到它所在桥的名字
  \param[in]  *pszInfName: Eth接口名
  \param[out]  plRetBridgeIntfName: 桥接口名
  \param[in]  iBrLen: 桥接口名长度
  \return 成功返回VOS_OK, 失败返回VOS_NOK
*/


     /*!
      \brief 判断传入的psBrDomain 名的桥是否是一个合法的桥实例
      \param[in] psBrDomain: 要传入的桥domain
      \param[out]  
      \return   存在以这个domain 的桥返回vos_true   
    */
VOS_BOOL BridgePortIsValidBridgeDomain(const VOS_CHAR *psBrDomain);

    /*!
      \brief 判断传入的psPortDomain 名的Port是否是一个合法的Port实例
      \param[in] psPortDomain: 要传入的Port domain
      \param[out]  
      \return   存在以这个domain 的Port返回vos_true   
    */
                            
VOS_BOOL BridgePortIsValidPortDomain(const VOS_CHAR *psPortDomain);
          
    /*!
      \brief 判断传入的psPortDomain 名,获取这个桥的当前状态
      \param[in] psPortDomain: 要传入的Port domain
      \param[out]  
      \return  返回Port enable状态
    */
VOS_BOOL BridgePortGetBrStatus(const VOS_CHAR *psBrDomain);

    /*!
      \brief 判断传入的psPortDomain 名,获取这个Port 对应的Interface, 并以InterfaceName(VOS_CHAR *)  代表这个interface
      \param[in] psPortDomain: 要传入的Port domain
      \param[out]  
      \return  返回bridge enable状态
    */
VOS_UINT32 BridgePortGetInterfaceByPortDomain(const VOS_CHAR *psPortDomain,VOS_CHAR *psInterface,VOS_UINT32 ulInterfaceLen);

VOS_UINT32 ATP_BRIDGE_FilterInit(VOS_VOID);

VOS_UINT32 ATP_BRIDGE_GetBridgeIntfNameByInterfaceName(const VOS_CHAR *pszInfName, VOS_CHAR * plRetBridgeIntfName, VOS_UINT32 iBrLen);

#endif

