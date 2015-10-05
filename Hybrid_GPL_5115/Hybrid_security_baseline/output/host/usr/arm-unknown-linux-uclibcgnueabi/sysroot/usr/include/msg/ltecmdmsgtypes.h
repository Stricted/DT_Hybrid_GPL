#ifndef __ATP_LTECMD_MSGTYPES_H__
#define __ATP_LTECMD_MSGTYPES_H__

#define ATP_CBBID_LTECMD_NAME                  "ltecmd"

enum ATP_LTECMD_MSGTYPES_EN
{
    ATP_MSG_LTECMD_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_LTECMD),
    ATP_MSG_NDISCMS_WAN_CHG = ATP_MSG_LTECMD_START,   /*!< NDISCMS消息开始 */
    ATP_MSG_NDISCMS_STATUS_CHG,
    ATP_MSG_NDISCMS_DIAL_UP,
    ATP_MSG_NDISCMS_DIAL_DOWN,

} ;
#define ATP_NDIS_MAX_LEN 256
#define ATP_MAC_LEN 6
typedef struct 
{
    VOS_CHAR acApnName[ATP_NDIS_MAX_LEN];
    VOS_CHAR acDialNumber[ATP_NDIS_MAX_LEN];
    VOS_CHAR acUsername[ATP_NDIS_MAX_LEN];
    VOS_CHAR  acPassword[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv4Enable[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv6Enable[ATP_NDIS_MAX_LEN];
    VOS_CHAR acMac[ATP_NDIS_MAX_LEN];         /* 将rmnet0 MAC 地址 传递给 sd5115*/
}ATP_NDIS_SND_MSG_ST;
/*<DTS2013083001384 xiazhongyue 20130909 begin */
typedef struct
{
    VOS_UINT32 ulConnected; /* 拨号是否成功 */
    VOS_UINT32 ulIPv4Status; /* ipv4 拨号状态*/
    VOS_UINT32 ulIPv6Status; /* ipv6 拨号状态*/
    VOS_CHAR acIPv4address[ATP_NDIS_MAX_LEN]; /*ipv4 地址*/
    VOS_CHAR acMask[ATP_NDIS_MAX_LEN];/* ipv4 掩码 */
    VOS_CHAR acIPv4Gateway[ATP_NDIS_MAX_LEN];/* ipv4 网观 */
    VOS_CHAR acIPv4DNS[ATP_NDIS_MAX_LEN];/* ipv4 dns*/
    VOS_CHAR acIPv6address[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv6Raprefix[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv6Iapdprefix[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv6Gateway[ATP_NDIS_MAX_LEN];
    VOS_CHAR acIPv6DNS[ATP_NDIS_MAX_LEN];
    VOS_UINT32 ulTcpMss;
}ATP_NDIS_RCV_MSG_ST;
/*DTS2013083001384 xiazhongyue 20130909 end >*/

typedef struct
{
    VOS_UINT32 ulMsgID; /* 拨号是否成功 */ 
	VOS_UINT32 ulErrCode;
	VOS_CHAR acIPv4address[ATP_NDIS_MAX_LEN]; /*ipv4 地址*/
	VOS_CHAR acIPv6address[ATP_NDIS_MAX_LEN]; /*ipv6 地址*/
}ATP_NDIS_Log_MSG_ST;

#endif // End of __ATP_LTECMD_MSGTYPES_H__

