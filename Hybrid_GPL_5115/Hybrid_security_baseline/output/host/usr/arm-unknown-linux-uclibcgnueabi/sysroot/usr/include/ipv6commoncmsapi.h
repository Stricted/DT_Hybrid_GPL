/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ipv6commoncmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x00192803
  *\date  2012-2-2
*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#ifndef __IPV6COMMONAPI_H__
#define __IPV6COMMONAPI_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef ATP_DEBUG
#define IPV6_LAN_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define IPV6_LAN_DEBUG(format,arg...)
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif


/* pd前缀preferred / valid时间默认值 */
#ifdef SUPPORT_ATP_TR181
#define IPV6_PREFIX_PREFERRED_TIME (86400)
#define IPV6_PREFIX_VALIDLIFE_TIME (604800)
#else
#define IPV6_PREFIX_PREFERRED_TIME (3600)
#define IPV6_PREFIX_VALIDLIFE_TIME (7200)
#endif

//!RA中M标志位
extern VOS_INT32 g_lManageFlagFromDhcp6s[ATP_BRIDGE_MAX];

//!RA中O标志位
extern VOS_INT32 g_lOtherFlagFromDhcp6s[ATP_BRIDGE_MAX];

/* IPv6地址宏 */
#define IPV6_MIN_ADDRESS_LENGTH     2
#define IPV6_MAX_ADDRESS_LENGTH     39
#define IPV6_MAX_ADDRESS_BUFFER     40

//!保存前缀数据的临时缓冲区大小
#define IPV6_PREFIX_BUFFER        24

/* IPv6 first area length */
#define IPV6_ADDR_LENGTH     4

/* 默认前缀 */
#define IPV6_DEFAULT_PREFIX       ("2001:0000:0000:0000::")

//!合法ipv6地址长度
#define ATP_DHCP6S_BUFFER_LENGTH     (128)

//!配置文件缓存大小
#define DHCP6S_CFG_FILE_BUFF        (128)
//!保存前缀数据的临时缓冲区大小
#define DHCP6S_PREFIX_BUFFER        (24)

//!\brief IPV6地址类型结构体
/*!
 *
 */
typedef enum tag_IPv6_ADDR_USE_TYPE_E
{
    IPv6_ADDR_USE_GLOBAL = 0,
    IPv6_ADDR_USE_DNS,
    IPv6_ADDR_USE_BUFF
}IPv6_ADDR_USE_TYPE_E;

/*标志位 */
typedef enum tag_ENABLE_FLAG_E
{
    FLAG_FALSE = 0,/*!< FLAG_FALSE is disable */
    FLAG_TRUE  = 1,/*!<  FLAG_TRUE is enable*/
    FLAG_BUFF
}ENABLE_FLAG_E;

typedef enum tag_IPv6_LOCAL_CFG_USE_TYPE_E
{
    IPv6_LOCAL_CFG_USE_RADVD = 0,
    IPv6_LOCAL_CFG_USE_ULA,
    IPv6_LOCAL_CFG_USE_DHCP6S,
    IPv6_LOCAL_CFG_USE_BUFF
}IPv6_LOCAL_CFG_USE_TYPE_E;


/*Radvd 本地配置结构体*/
typedef struct tag_IPV6_LOCAL_CFG_ST
{
    VOS_INT8   czInterfactAddr[ATP_BUFFER_128];        /* DHCP Server 端口全局地址 */
#ifdef SUPPORT_ATP_IPV6_ULA   
    VOS_CHAR  czNewLine;
    VOS_INT8   czInterfactULAAddr[ATP_BUFFER_128]; //added by zKF40309,2011,3,22
#endif  
    VOS_INT32 lBridgeKey;
}IPV6_LOCAL_CFG_ST,*IPV6_LOCAL_CFG_PST;

//!\brief  前缀选项结构体
/*!
 *
 */
typedef struct tag_IPV6_PREFIX_ST
{
    VOS_INT32 lPrefixLen;                        /*!< 保存生成前的前缀长度。  */
    VOS_INT8  czPrefixBeforGenerate[ATP_BUFFER_128];   /*!< 保存生成前的前缀,前缀长度小于等于64.当lPrefixLen = 64时，说明该前缀可以直接用于生成地址池，因此pcPrefixAfterGenerate = NULL  */
    VOS_INT8  czPrefixAfterGenerate[IPV6_PREFIX_BUFFER];   /*!< 保存生成后的前缀，该前缀长度固定为64.用于生成IP池和RA的无状态自动前缀配置使用。 */
}IPV6_PREFIX_ST,*IPV6_PREFIX_PST;

//!\brief 前缀参数结构体
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_PREFIX_ST
{
    VOS_INT32  lPrefixEnable;                    /*!< 表示前缀是否配置，ATP_DHCP6S_FLAG_ENABLE : 配置、 ATP_DHCP6S_FLAG_DISABLE: 非配置 */
    VOS_CHAR   czPrefix[ATP_DHCP6S_BUFFER_LENGTH];       /*!< 合法IPv6地址  */
    VOS_INT32  lPrefixLength;            /*!< 前缀长度 */
    VOS_UINT32 ulT1;
    VOS_UINT32 ulT2;
    
}ATP_DHCP6S_PREFIX_ST,*ATP_DHCP6S_PREFIX_PST;

//!\brief DNS 服务器参数结构体
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_DNS_SERVER_ST
{
    VOS_INT32 lServerEnable;        /*!< 表示DNS_SERVER是否配置*/
    VOS_CHAR  czServer[ATP_DHCP6S_BUFFER_LENGTH];      /*!< 如果有多个 DNS 服务器，需要在服务器地址中间添加','用以隔离。 */
}ATP_DHCP6S_DNS_SERVER_ST,*ATP_DHCP6S_DNS_SERVER_PST;

//!\brief Domain name 参数结构体 
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_DOMAIN_SERVER_ST
{
    VOS_INT32 lServerEnable;      /*!< 表示DOMAIN_SERVER是否配置*/
    VOS_CHAR  czServer[ATP_DHCP6S_BUFFER_LENGTH];      /*!< DOMAIN_SERVER地址*/
}ATP_DHCP6S_DOMAIN_SERVER_ST,*ATP_DHCP6S_DOMAIN_SERVER_PST;

//!\brief DHCPv6 Server 服务器参数结构体
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_SERVER_ST
{
    ATP_DHCP6S_DNS_SERVER_ST    stDNSServer;   /*!< DNS 服务器*/
    ATP_DHCP6S_DOMAIN_SERVER_ST stDomainNameServer; /*!< 域名服务器*/
}ATP_DHCP6S_SERVER_ST,*ATP_DHCP6S_SERVER_PST;

//!\brief DHCPv6 Server 需要WAN侧提供参数结构体 
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_PARAMETER_ST
{
    ATP_DHCP6S_PREFIX_ST stPrefixPara;    /*!< 前缀参数*/
    ATP_DHCP6S_SERVER_ST stServerPara;  /*!< 服务器参数*/  
    VOS_CHAR acbridgename[ATP_BUFFER_16] ;    /*!< 桥名*/
    VOS_INT32 lBridgeKey ;    
}ATP_DHCP6S_PARAMETER_ST,*ATP_DHCP6S_PARAMETER_PST;

//! 用于保存 WAN 侧数据参数 
extern ATP_DHCP6S_PARAMETER_ST g_stDHCPv6SaveWanPara[ATP_BRIDGE_MAX];

/*!检查十六进制数的合法性
  \brief 
  \param[in]  lHex: 
  \return 
  \retval static VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_CheckHex(const VOS_INT32 lHex);

/*!
  \brief 验证IPv6地址的合法性
  \param[in]  pcString: 
  \param[in]  lUseType: 
  \return 
  \retval VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_CheckIPv6Address(const VOS_CHAR * pcString,VOS_INT32 lUseType);

/*!
  \brief 将字符串转换为完整地址
  \param[in]  pucSrcString: 
  \param[in]  pucBuffer: 
  \return 
  \retval VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_TransformStrToFullIPv6Address(const VOS_CHAR * pucSrcString, VOS_CHAR * pucBuffer);

/*!
  \brief 获取指定端口的IPv6地址,获取到的地址将以','分隔保存在pucBuffer中
  \param[in]  pucInterface: 
  \param[in]  pucBuffer: 
  \param[in]  ulBufSize: 
  \param[in]  ulDisplayAddrNum: 
  \return VOS_UINT32
  \retval 
*/
extern VOS_UINT32 ATP_IPV6COMMON_GetInterfaceIPv6Address(const VOS_CHAR * pucInterface, VOS_CHAR * pucBuffer,const VOS_UINT32 ulBufSize,VOS_UINT32 ulDisplayAddrNum);

/*!
  \brief 获取指定端口的IPv6地址,获取到的地址将以','分隔保存在pucBuffer中
  \param[in]  pucInterface: 
  \param[in]  pucTunnelIfcName: 
  \param[in]  pucBuffer: 
  \param[in]  ulBufSize: 
  \param[in]  ulDisplayAddrNum: 
  \return 
  \retval VOS_UINT32
*/
VOS_UINT32 ATP_IPV6COMMON_GetInterfaceIPv6AddressDetail(const VOS_CHAR * pucInterface, const VOS_CHAR * pucTunnelIfcName,VOS_CHAR * pucBuffer,const VOS_UINT32 ulBufSize,VOS_UINT32 ulDisplayAddrNum);

/*!
  \brief 清除radvd或dhcp6s写入文件的旧配置前缀
  \param[in]  lBridgeKey: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/
VOS_INT32 ATP_IPV6COMMON_CtlDeleteLocalCfg(const VOS_INT32 lBridgeKey,const VOS_CHAR * pcFileName,const VOS_INT32 lUseType);

/*!
  \brief 将radvd或dhcp6s的前缀配置添加到br0，并写入文件保存
  \param[in]  pstLocalCfg: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/    
VOS_INT32 ATP_IPV6COMMON_CtlSetLocalCfg( IPV6_LOCAL_CFG_PST pstLocalCfg,const VOS_CHAR * pcFileName, const VOS_INT32 lUseType);

/*!
  \brief 设置radvd或dhcp6s的前缀配置，将前缀写入或删出br0，从而会影响该前缀在主路由表中的添加或删除
           如没有文件则建立文件保存，若存在则修改文件保存的前缀内容
  \param[in]  pstLocalCfg: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/
VOS_INT32 ATP_IPV6COMMON_CtlCheckLocalCfg( IPV6_LOCAL_CFG_PST pstLocalCfg,const VOS_CHAR * pcFileName, const VOS_INT32 lUseType);

/*!
  \brief 根据前缀信息生成前缀
  \param[in]  pstPrefix: 
  \return VOS_INT32
  \retval 前缀是否生成成功
*/
VOS_INT32 ATP_IPV6COMMON_CtlGeneratePrefix( IPV6_PREFIX_PST pstPrefix);

VOS_INT32 ATP_UTIL_LockFile(VOS_CHAR *pszFileName, FILE ** fFile);

 VOS_VOID ATP_UTIL_UnlockFile(VOS_INT32 iLockFd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __IPV6COMMONAPI_H__ */
