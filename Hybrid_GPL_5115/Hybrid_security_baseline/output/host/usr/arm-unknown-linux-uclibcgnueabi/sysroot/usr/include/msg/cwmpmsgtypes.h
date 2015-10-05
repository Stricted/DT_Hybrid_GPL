#ifndef __ATP_CWMP_MSGTYPES_H__
#define __ATP_CWMP_MSGTYPES_H__

#define ATP_CBBID_CWMP_NAME                  "cwmp"

enum ATP_CWMP_MSGTYPES_EN
{
    ATP_MSG_CWMP_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CWMP),
    ATP_MSG_CWMP_WANIP,
    ATP_MSG_CWMP_DIAGNOSTIC,
    ATP_MSG_CWMP_AUTHTYPE,
    ATP_MSG_CWMP_UDP_CON_REQ,
    ATP_MSG_CWMP_CERT,
    ATP_MSG_CWMP_IS_BUSY,
    ATP_MSG_CWMP_RETRY_URL,
#ifdef SUPPORT_ATP_CWMP_DOWNLOAD_QUEUE
    ATP_MSG_CWMP_SCHEDULEDWL_END,
#endif
    /*START OF ADD BY Z00163152:20110516 FOR DTS2011051303726 */
    ATP_MSG_CWMP_PROVISIONCODE_INFORM,
    /*END OF ADD BY Z00163152:20110516 FOR DTS2011051303726 */
    /*start 根据BT要求添加Operating mode参数by l38280*/
#ifdef SUPPORT_ATP_WAN_AUTO_SENSING
    ATP_MSG_CWMP_MODE_INFORM,
#endif
    /*end 根据BT要求添加Operating mode参数by l38280*/
#ifdef SUPPORT_ATP_TR181
    // check DefaultActiveNotificationThrottle
        ATP_MSG_CWMP_ACTNOTIFY_THROTTLE,
#endif

} ;


enum ATP_CWMP_KEYID_EN
{
    ATP_CWMP_KEY_BOTH_CERTIFACATE = ATP_SEC_KEYID_CODE_DEF(ATP_MODULE_CWMP), /*!< cfg file key */
};


/*
 *  CWMP消息定义
 */

/* WAN接口更改消息体结构 */
#define ATP_CWMP_WANIP_STATUS_UP        (1<<0)
#define ATP_CWMP_WANIP_BIND_CWMP        (1<<1)
#ifdef SUPPORT_ATP_CWMP_IPV6
#define ATP_CWMP_IP_MAX   4
#endif

typedef struct tagATP_CWMP_WANIP_ST
{
    VOS_UINT32	ulWANState;
    VOS_CHAR	acWANInterface[256];
#ifdef SUPPORT_ATP_CWMP_IPV6
    VOS_CHAR	acWANIP[ATP_CWMP_IP_MAX][64];
#else
    VOS_CHAR	acWANIP[16];
#endif
    VOS_CHAR  acNewUrl[256 + 4];        
    VOS_CHAR  bWithOpt43;
} ATP_CWMP_WANIP_ST;

typedef struct tagATP_CWMP_STUN_RES_ST
{
    VOS_UINT32      ulLocalIP;
    VOS_UINT32      ulIP;
    VOS_UINT16      usPort;
} ATP_CWMP_STUN_RES_ST;

typedef struct tagATP_CWMP_PPPUSER_ST
{
	VOS_CHAR	acWanPath[128];
	VOS_CHAR	acPppUser[64];
} ATP_CWMP_PPPUSER_ST;

#define ATP_CWMP_DEFAULT_CON_REQ_PORT_D     (7547)

#ifdef SUPPORT_ATP_CWMP_DOWNLOAD_QUEUE
//#define ATP_CBBID_WINDOWTIME_NAME           "cwmpwindowtime"

typedef struct tagATP_UPG_CWMP_EVT_ST
{
    const VOS_CHAR                  acCmdKey[32];
    VOS_UINT32                      ulErrorCode;
    VOS_UINT32                      ulTaskType;
    VOS_BOOL                        bReboot;
} ATP_UPG_CWMP_EVT_ST;
#define CWMP_DOWNLOAD_STARTTIME      "/var/state/DownloadStartTime"
#endif

#ifdef SUPPORT_ATP_CWMP_CANCELTRANS
#define CWMP_NORMMAL_COMMANDKEY      "/var/state/NormalCommdKey"
#define CWMP_CANCELTRANS_COMMANDKEY  "/var/state/CancelTransCommdKey"
#endif

#endif // End of __ATP_CWMP_MSGTYPES_H__

