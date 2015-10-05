/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file upnpmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
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
    ATP_MSG_UPNP_REMOVE_PORTMAPPING/*host 有增减,更新portmap*/
};

#define UPNP_PORTMAPPING_STRING_LEN     (64 + 4)

typedef enum tagATP_UPNP_TO_PORTMAPPING_MSG_TYPE_EN
{
    ATP_UPNP_PORTMAPPING_ADD = 1,/*!< 规则的操作类型: 添加 */
    ATP_UPNP_PORTMAPPING_DEL = 2,/*!< 规则的操作类型: 删除 */
}ATP_UPNP_TO_PORTMAPPING_TYPE_EN;

typedef enum tagATP_UPNP_PORTMAPPING_RULE_OPS_EN
{
    ATP_UPNP_PORTMAPPING_RULE_DISABLE = 0,/*!< 规则的操作:去使能 */
    ATP_UPNP_PORTMAPPING_RULE_ENABLE = 1,/*!< 规则的操作:使能 */
}ATP_UPNP_PORTMAPPING_RULE_OP;

typedef enum tagATP_PORTMAPPING_TO_UPNP_RET_VALUE_EN
{
    ATP_PORTMAPPING_TO_UPNP_RET_OK = 0,/*!< UPNP结果值:成功 */
    ATP_PORTMAPPING_TO_UPNP_RET_NOK = 1,/*!< UPNP结果值:失败 */
}ATP_PORTMAPPING_TO_UPNP_RET_VALUE_EN;

//!与 UPNP 通讯的数据结构，两个模块共用
typedef struct tagATP_UPnP_PORTMAPPING_ST
{
    VOS_UINT32  ulOpType;                                   /*!< 消息类型 */
    VOS_BOOL    bEnable;                                    /*!< 规则是否使能 */
    VOS_UINT16  usExternalPort;                             /*!< 外部端口 */
    VOS_UINT16  usInternalPort;                             /*!< 重定向的内部端口 */
    VOS_UINT32  ulLeaseDuration;                            /*!< 规则有效期，暂时不实现 */
    VOS_CHAR    acProtocol[16];               /*!< 协议 */
    VOS_CHAR    acRemoteHost[16];            /*!< 外部主机 */
    VOS_CHAR    acInternalClient[16];        /*!< 被重定向到的内部主机 */
    VOS_CHAR    acPortMapDesc[68];           /*!< 规则描述名称 */
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
  \brief UPNP PortMapping消息处理函数
  \param[in]  pstMsg: 接受到的消息
  \return 
  \retval VOS_OK 执行成功
  \retval VOS_NOK 执行失败
*/
#endif // End of __ATP_UPNP_MSGTYPES_H__

