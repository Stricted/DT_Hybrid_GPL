/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6scmsmsgtypes.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author x00192803
  *\date  2012-2-11
*/


#ifndef __ATP_DHCP6SCMS_MSGTYPES_H__
#define __ATP_DHCP6SCMS_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_CBBID_DHCP6S_NAME                  "dhcp6s"

enum ATP_DHCP6S_MSGTYPES_EN
{
    ATP_MSG_DHCP6S_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DHCP6SCMS),    //����DHCP6s����      
    ATP_MSG_DHCP6S_STOP,         //ֹͣDHCP6s����
    ATP_MSG_DHCP6S_INITOVER,  //DHCP6s�����������
    ATP_MSG_DHCP6S_DELETE_NODE, //ɾ����ǰ������Ӧ���
#ifdef SUPPORT_ATP_TR181    
    ATP_MSG_DHCP6S_CFG_CHANGE,
    ATP_MSG_DHCP6S_CLIENT_GET_ADDR, 
    ATP_MSG_DHCP6S_CLIENT_GET_PREFIX,    
    ATP_MSG_DHCP6S_CLIENT_SENT_OPTION,  
    ATP_MSG_DHCP6S_SERVER_SENT_OPTION,      
    ATP_MSG_DHCP6S_LAN_STATUS_DOWN,      
#endif    
} ;

#define DCHP6SMSG_OPT_LEN 32
#define DCHP6SMSG_CONFIL_LEN 128
#define DCHP6SMSG_IFC_LEN 32

#ifdef SUPPORT_ATP_TR181    
#define DCHP6SMSG_ADDR_LEN 128

#define INTERFACE_LEN_MAX             256
#define DHCPV6_COMM_ID_LEN          32
#define DHCPV6_IP_PREFIX_LEN          32	
#define POOL_STATUS_LEN                 32
#define DUID_LEN                                256	
#define LINK_ADDR_LEN                      17   //hex string format mac address	

//!option ����
#define DHCP6S_OPTION_LEN  256

typedef struct tagDHCPv6_SERVER_POOL_NODE {
    struct tagDHCPv6_SERVER_POOL_NODE  * next;   
    
    VOS_BOOL              bEnable;                                           // Enables or disables this Pool entry.
    VOS_UINT32           ulPoolOrder;	                              // pool precedence, 1 is the highest priority
    VOS_CHAR              acStatus[POOL_STATUS_LEN];            //The status of this entry: enable, disable, error_misconfigured
    VOS_CHAR              acInterface[INTERFACE_LEN_MAX]; 
    VOS_BOOL              bIanaEnable;   
    VOS_CHAR              acIanaPrefix[DHCPV6_IP_PREFIX_LEN];
    VOS_BOOL              bIapdEnable;   
    VOS_UINT32           ulIapdAddrLen;
    VOS_CHAR              acDuid[DUID_LEN];   	                             // mapping to client id, dhcpv6 option 1   
    VOS_CHAR              acVendorId[DHCPV6_COMM_ID_LEN];   	// mapping to dhcpv6 option 16
    VOS_CHAR              acUsrClassId[DHCPV6_COMM_ID_LEN];     // mapping to dhcpv6 option 15
    VOS_CHAR              acSrcAddr[LINK_ADDR_LEN];   	              // link-layer address   
    VOS_CHAR              acSrcAddrMask[LINK_ADDR_LEN];  
    
} DHCPv6_SERVER_POOL_NODE;


struct stDhcp6sClientInfo
{
    VOS_CHAR     acSourceAddr[DCHP6SMSG_ADDR_LEN];        /*!< client�ӿڵ�ַ  */
    VOS_UINT32  ulPreferredtime;        /*!< �Ƽ�ʱ��*/
    VOS_UINT32  ulValidtime;                /*!< ��Чʱ�� */    
    VOS_CHAR     acAddr[DCHP6SMSG_ADDR_LEN];        /*!< IANA ��ַ��iapdǰ׺ */
};

struct stDhcp6sRevOpt
{
    VOS_CHAR          acSourceAddr[DCHP6SMSG_IFC_LEN];        /*!< �ӿ���  */
    VOS_UINT32       ulOptTag;        /*!< option  code */
    VOS_UINT8         acOptValue[DHCP6S_OPTION_LEN];        /*!< option  value  */
};
#endif  

//!\brief dhcp6s��Ϣ�ṹ��
/*!
 * Я���ӿ����Լ������в�������Ϣ��dhcp6s����
 */
struct stDhcp6sData
{
    VOS_CHAR    acInputOptions[DCHP6SMSG_OPT_LEN];    /*!< ���ò�����Ϣ */
    VOS_CHAR    acConfigFile[DCHP6SMSG_CONFIL_LEN];    /*!< ���ò�����Ϣ */    
    VOS_CHAR    acInterface[DCHP6SMSG_IFC_LEN];        /*!< �ӿ���  */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
