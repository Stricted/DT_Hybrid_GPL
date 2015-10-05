/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file upnpmsgtypes.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author W00200480
  *\date  2012-1-16
*/
#ifndef __ATP_UPNP_MSGTYPES_H__
#define __ATP_UPNP_MSGTYPES_H__

#define ATP_CBBID_UPNP_NAME                  "upnp"
#define ATP_CBBID_UPNPACT_NAME               "upnpaction"
#define ATP_UPNP_SSDP_ADV_IPV6          "FF02::C"
#define ATP_UPNP_SSDP_ADV_IP              "239.255.255.250"
#define ATP_UPNP_SSDP_ADV_PORT          (1900)
#define ATP_UPNP_SSDP_HOST                  ATP_UPNP_SSDP_ADV_IP":1900"
#define ATP_UPNP_HTTP_SERVER_PORT    (37215)
#define ATP_UPNP_HTTPS_SERVER_PORT  (37443)
#define ATP_UPNP_SSDP_TTL_VALUE        (4)
#define ATP_UPNP_SSDP_HOSTIPV6          "[FF02::C]:1900"
#define ATP_UPNP_HTTP_SERVER_DEF_IP     "169.254.100.156"

enum ATP_UPNP_MSGTYPES_EN
{
    ATP_MSG_UPNP_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_UPNP),
    ATP_MSG_UPNP_ENABLE_UPNP,
    ATP_MSG_UPNP_DISABLE_UPNP,
    ATP_MSG_UPNP_SET_CFGPWD,
    ATP_MSG_UPNP_RENEW_UPNP_TREE,
    ATP_MSG_UPNP_REBOOT,
    ATP_MSG_UPNP_MIC_ADD_SSDP,
    ATP_MSG_UPNP_LAN_IP_CHANGE,
    ATP_MSG_UPNP_TO_PORTMAPPING,
    ATP_MSG_UPNP_REFRESH_MAPPING,
    ATP_MSG_UPNP_REMOVE_PORTMAPPING/*host ������,����portmap*/
};

#define UPNP_PORTMAPPING_STRING_LEN     (64 + 4)

typedef enum tagATP_UPNP_TO_PORTMAPPING_MSG_TYPE_EN
{
    ATP_UPNP_PORTMAPPING_ADD = 1,/*!< ����Ĳ�������: ��� */
    ATP_UPNP_PORTMAPPING_DEL = 2,/*!< ����Ĳ�������: ɾ�� */
}ATP_UPNP_TO_PORTMAPPING_TYPE_EN;

typedef enum tagATP_UPNP_PORTMAPPING_RULE_OPS_EN
{
    ATP_UPNP_PORTMAPPING_RULE_DISABLE = 0,/*!< ����Ĳ���:ȥʹ�� */
    ATP_UPNP_PORTMAPPING_RULE_ENABLE = 1,/*!< ����Ĳ���:ʹ�� */
}ATP_UPNP_PORTMAPPING_RULE_OP;

typedef enum tagATP_PORTMAPPING_TO_UPNP_RET_VALUE_EN
{
    ATP_PORTMAPPING_TO_UPNP_RET_OK = 0,/*!< UPNP���ֵ:�ɹ� */
    ATP_PORTMAPPING_TO_UPNP_RET_NOK = 1,/*!< UPNP���ֵ:ʧ�� */
}ATP_PORTMAPPING_TO_UPNP_RET_VALUE_EN;

//!�� UPNP ͨѶ�����ݽṹ������ģ�鹲��
typedef struct tagATP_UPnP_PORTMAPPING_ST
{
    VOS_UINT32  ulOpType;                                   /*!< ��Ϣ���� */
    VOS_BOOL    bEnable;                                    /*!< �����Ƿ�ʹ�� */
    VOS_UINT16  usExternalPort;                             /*!< �ⲿ�˿� */
    VOS_UINT16  usInternalPort;                             /*!< �ض�����ڲ��˿� */
    VOS_UINT32  ulLeaseDuration;                            /*!< ������Ч�ڣ���ʱ��ʵ�� */
    VOS_CHAR    acProtocol[16];               /*!< Э�� */
    VOS_CHAR    acRemoteHost[16];            /*!< �ⲿ���� */
    VOS_CHAR    acInternalClient[16];        /*!< ���ض��򵽵��ڲ����� */
    VOS_CHAR    acPortMapDesc[68];           /*!< ������������ */
}ATP_UPnP_PORTMAPPING_ST;

#define ATP_UPNP_TO_PORTMAPPING_MSG_DATE_LEN        sizeof(ATP_UPnP_PORTMAPPING_ST)

typedef struct tagATP_UPnP_RenewTreeMsg_ST
{
    VOS_UINT32  ulNum;
    VOS_CHAR    acDomain[256]; /* ulNum * 256 */
}ATP_UPnP_RenewTreeMsg;

#ifdef SUPPORT_DESKTOP
#define ATP_UPNP_CONF_PATH_D                  "upnpIGD.conf"
#else
#define ATP_UPNP_CONF_PATH_D                  "/var/upnpIGD.conf"
#endif

typedef struct tagATP_UPnP_WanInfo_ST
{
    struct tagATP_UPnP_WanInfo_ST *pstNext;
    VOS_CHAR    *pcWanDevDomain;
    VOS_CHAR    *pcWanConnDomain;
    VOS_CHAR    *pcWanIPIntfDomain;
    VOS_BOOL    bIpIntf;              /* true ip false ppp*/
}ATP_UPnP_WanInfo_ST;

typedef struct tagATP_UPnP_LanInfo_ST
{
    struct tagATP_UPnP_LanInfo_ST *pstNext;
    VOS_CHAR        *pcLanDevDomain;
    VOS_CHAR        *pcWlanServiceDomain;
    //VOS_CHAR        *pcEthServiceDomain;
}ATP_UPnP_LanInfo_ST;

typedef struct tagATP_UPnP_ConfInfo_ST
{
    ATP_UPnP_WanInfo_ST         *pstWanInfoList;
    ATP_UPnP_LanInfo_ST         *pstLanInfoList;
}ATP_UPnP_ConfInfo_ST;



/*!
  \brief UPNP PortMapping��Ϣ������
  \param[in]  pstMsg: ���ܵ�����Ϣ
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
#endif // End of __ATP_UPNP_MSGTYPES_H__

