/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file ipv6commoncmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author x00192803
  *\date  2012-2-2
*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
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


/* pdǰ׺preferred / validʱ��Ĭ��ֵ */
#ifdef SUPPORT_ATP_TR181
#define IPV6_PREFIX_PREFERRED_TIME (86400)
#define IPV6_PREFIX_VALIDLIFE_TIME (604800)
#else
#define IPV6_PREFIX_PREFERRED_TIME (3600)
#define IPV6_PREFIX_VALIDLIFE_TIME (7200)
#endif

//!RA��M��־λ
extern VOS_INT32 g_lManageFlagFromDhcp6s[ATP_BRIDGE_MAX];

//!RA��O��־λ
extern VOS_INT32 g_lOtherFlagFromDhcp6s[ATP_BRIDGE_MAX];

/* IPv6��ַ�� */
#define IPV6_MIN_ADDRESS_LENGTH     2
#define IPV6_MAX_ADDRESS_LENGTH     39
#define IPV6_MAX_ADDRESS_BUFFER     40

//!����ǰ׺���ݵ���ʱ��������С
#define IPV6_PREFIX_BUFFER        24

/* IPv6 first area length */
#define IPV6_ADDR_LENGTH     4

/* Ĭ��ǰ׺ */
#define IPV6_DEFAULT_PREFIX       ("2001:0000:0000:0000::")

//!�Ϸ�ipv6��ַ����
#define ATP_DHCP6S_BUFFER_LENGTH     (128)

//!�����ļ������С
#define DHCP6S_CFG_FILE_BUFF        (128)
//!����ǰ׺���ݵ���ʱ��������С
#define DHCP6S_PREFIX_BUFFER        (24)

//!\brief IPV6��ַ���ͽṹ��
/*!
 *
 */
typedef enum tag_IPv6_ADDR_USE_TYPE_E
{
    IPv6_ADDR_USE_GLOBAL = 0,
    IPv6_ADDR_USE_DNS,
    IPv6_ADDR_USE_BUFF
}IPv6_ADDR_USE_TYPE_E;

/*��־λ */
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


/*Radvd �������ýṹ��*/
typedef struct tag_IPV6_LOCAL_CFG_ST
{
    VOS_INT8   czInterfactAddr[ATP_BUFFER_128];        /* DHCP Server �˿�ȫ�ֵ�ַ */
#ifdef SUPPORT_ATP_IPV6_ULA   
    VOS_CHAR  czNewLine;
    VOS_INT8   czInterfactULAAddr[ATP_BUFFER_128]; //added by zKF40309,2011,3,22
#endif  
    VOS_INT32 lBridgeKey;
}IPV6_LOCAL_CFG_ST,*IPV6_LOCAL_CFG_PST;

//!\brief  ǰ׺ѡ��ṹ��
/*!
 *
 */
typedef struct tag_IPV6_PREFIX_ST
{
    VOS_INT32 lPrefixLen;                        /*!< ��������ǰ��ǰ׺���ȡ�  */
    VOS_INT8  czPrefixBeforGenerate[ATP_BUFFER_128];   /*!< ��������ǰ��ǰ׺,ǰ׺����С�ڵ���64.��lPrefixLen = 64ʱ��˵����ǰ׺����ֱ���������ɵ�ַ�أ����pcPrefixAfterGenerate = NULL  */
    VOS_INT8  czPrefixAfterGenerate[IPV6_PREFIX_BUFFER];   /*!< �������ɺ��ǰ׺����ǰ׺���ȹ̶�Ϊ64.��������IP�غ�RA����״̬�Զ�ǰ׺����ʹ�á� */
}IPV6_PREFIX_ST,*IPV6_PREFIX_PST;

//!\brief ǰ׺�����ṹ��
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_PREFIX_ST
{
    VOS_INT32  lPrefixEnable;                    /*!< ��ʾǰ׺�Ƿ����ã�ATP_DHCP6S_FLAG_ENABLE : ���á� ATP_DHCP6S_FLAG_DISABLE: ������ */
    VOS_CHAR   czPrefix[ATP_DHCP6S_BUFFER_LENGTH];       /*!< �Ϸ�IPv6��ַ  */
    VOS_INT32  lPrefixLength;            /*!< ǰ׺���� */
    VOS_UINT32 ulT1;
    VOS_UINT32 ulT2;
    
}ATP_DHCP6S_PREFIX_ST,*ATP_DHCP6S_PREFIX_PST;

//!\brief DNS �����������ṹ��
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_DNS_SERVER_ST
{
    VOS_INT32 lServerEnable;        /*!< ��ʾDNS_SERVER�Ƿ�����*/
    VOS_CHAR  czServer[ATP_DHCP6S_BUFFER_LENGTH];      /*!< ����ж�� DNS ����������Ҫ�ڷ�������ַ�м����','���Ը��롣 */
}ATP_DHCP6S_DNS_SERVER_ST,*ATP_DHCP6S_DNS_SERVER_PST;

//!\brief Domain name �����ṹ�� 
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_DOMAIN_SERVER_ST
{
    VOS_INT32 lServerEnable;      /*!< ��ʾDOMAIN_SERVER�Ƿ�����*/
    VOS_CHAR  czServer[ATP_DHCP6S_BUFFER_LENGTH];      /*!< DOMAIN_SERVER��ַ*/
}ATP_DHCP6S_DOMAIN_SERVER_ST,*ATP_DHCP6S_DOMAIN_SERVER_PST;

//!\brief DHCPv6 Server �����������ṹ��
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_SERVER_ST
{
    ATP_DHCP6S_DNS_SERVER_ST    stDNSServer;   /*!< DNS ������*/
    ATP_DHCP6S_DOMAIN_SERVER_ST stDomainNameServer; /*!< ����������*/
}ATP_DHCP6S_SERVER_ST,*ATP_DHCP6S_SERVER_PST;

//!\brief DHCPv6 Server ��ҪWAN���ṩ�����ṹ�� 
/*!
 *
 */
typedef struct tag_ATP_DHCP6S_PARAMETER_ST
{
    ATP_DHCP6S_PREFIX_ST stPrefixPara;    /*!< ǰ׺����*/
    ATP_DHCP6S_SERVER_ST stServerPara;  /*!< ����������*/  
    VOS_CHAR acbridgename[ATP_BUFFER_16] ;    /*!< ����*/
    VOS_INT32 lBridgeKey ;    
}ATP_DHCP6S_PARAMETER_ST,*ATP_DHCP6S_PARAMETER_PST;

//! ���ڱ��� WAN �����ݲ��� 
extern ATP_DHCP6S_PARAMETER_ST g_stDHCPv6SaveWanPara[ATP_BRIDGE_MAX];

/*!���ʮ���������ĺϷ���
  \brief 
  \param[in]  lHex: 
  \return 
  \retval static VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_CheckHex(const VOS_INT32 lHex);

/*!
  \brief ��֤IPv6��ַ�ĺϷ���
  \param[in]  pcString: 
  \param[in]  lUseType: 
  \return 
  \retval VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_CheckIPv6Address(const VOS_CHAR * pcString,VOS_INT32 lUseType);

/*!
  \brief ���ַ���ת��Ϊ������ַ
  \param[in]  pucSrcString: 
  \param[in]  pucBuffer: 
  \return 
  \retval VOS_INT32
*/
extern VOS_INT32 ATP_IPV6COMMON_TransformStrToFullIPv6Address(const VOS_CHAR * pucSrcString, VOS_CHAR * pucBuffer);

/*!
  \brief ��ȡָ���˿ڵ�IPv6��ַ,��ȡ���ĵ�ַ����','�ָ�������pucBuffer��
  \param[in]  pucInterface: 
  \param[in]  pucBuffer: 
  \param[in]  ulBufSize: 
  \param[in]  ulDisplayAddrNum: 
  \return VOS_UINT32
  \retval 
*/
extern VOS_UINT32 ATP_IPV6COMMON_GetInterfaceIPv6Address(const VOS_CHAR * pucInterface, VOS_CHAR * pucBuffer,const VOS_UINT32 ulBufSize,VOS_UINT32 ulDisplayAddrNum);

/*!
  \brief ��ȡָ���˿ڵ�IPv6��ַ,��ȡ���ĵ�ַ����','�ָ�������pucBuffer��
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
  \brief ���radvd��dhcp6sд���ļ��ľ�����ǰ׺
  \param[in]  lBridgeKey: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/
VOS_INT32 ATP_IPV6COMMON_CtlDeleteLocalCfg(const VOS_INT32 lBridgeKey,const VOS_CHAR * pcFileName,const VOS_INT32 lUseType);

/*!
  \brief ��radvd��dhcp6s��ǰ׺������ӵ�br0����д���ļ�����
  \param[in]  pstLocalCfg: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/    
VOS_INT32 ATP_IPV6COMMON_CtlSetLocalCfg( IPV6_LOCAL_CFG_PST pstLocalCfg,const VOS_CHAR * pcFileName, const VOS_INT32 lUseType);

/*!
  \brief ����radvd��dhcp6s��ǰ׺���ã���ǰ׺д���ɾ��br0���Ӷ���Ӱ���ǰ׺����·�ɱ��е���ӻ�ɾ��
           ��û���ļ������ļ����棬���������޸��ļ������ǰ׺����
  \param[in]  pstLocalCfg: 
  \param[in]  pcFileName: 
  \param[in]  lUseType: 
  \return 
  \retval 
*/
VOS_INT32 ATP_IPV6COMMON_CtlCheckLocalCfg( IPV6_LOCAL_CFG_PST pstLocalCfg,const VOS_CHAR * pcFileName, const VOS_INT32 lUseType);

/*!
  \brief ����ǰ׺��Ϣ����ǰ׺
  \param[in]  pstPrefix: 
  \return VOS_INT32
  \retval ǰ׺�Ƿ����ɳɹ�
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
