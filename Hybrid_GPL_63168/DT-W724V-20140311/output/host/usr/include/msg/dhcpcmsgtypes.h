#ifndef __ATP_DHCPC_MSGTYPES_H__
#define __ATP_DHCPC_MSGTYPES_H__

#define ATP_CBBID_DHCPC_NAME "dhcpc"

/*Start of ATP 2012-5-9 for TR181 by t00189036: 上报dhcpc的状态*/
//Don't change these macros unthinkingly
#define DHCPC_STATUS_INIT_SELECTING 0
#define DHCPC_STATUS_REQUESTING 1
#define DHCPC_STATUS_BOUND      2
#define DHCPC_STATUS_RENEWING   3
#define DHCPC_STATUS_REBINDING  4
#define DHCPC_STATUS_INIT_REBOOT    5
#define DHCPC_STATUS_RENEW_REQUESTED 6
#define DHCPC_STATUS_RELEASED   7

#define ATP_DHCPC_MAX_IFC_LEN 32
#define DHCPC_IFC_LEN (64)
#define DHCPC_OPTION_43_LEN 256
#define DHCPC_OPTION_LEN  256
#define DHCPC_SENT_OPTION_NUM_MAX  16
#define DHCPC_REQ_OPTION_NUM_MAX  32



typedef struct tagATP_DHCPC_STATUSCHG_MSG_ST
{
    VOS_UINT32 ulStatus;
    VOS_CHAR   acIntfName[DHCPC_IFC_LEN];
}ATP_DHCPC_STATUSCHG_MSG_ST;
/*End of ATP 2012-5-9 for TR181 by t00189036*/

typedef struct tagATP_DHCPC_OPTION_ST
{
    VOS_UINT32        tag;
    VOS_UINT32        len;
    VOS_UINT8         value[DHCPC_OPTION_LEN];  
}ATP_DHCPC_OPTION_ST;

enum ATP_DHCPC_MSGTYPES_EN
{
    ATP_MSG_DHCPC_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DHCPC),
    ATP_MSG_DHCPC_CWMPRENEW,
    ATP_MSG_DHCPC_OPTIONS_COME,
    ATP_MSG_DHCPC_STATUS_CHANGE,
    ATP_MSG_DHCPC_VLAN_FORCE_CHANGE,
    ATP_MSG_DHCPC_SENTOPTION_UPDATE,
    ATP_MSG_DHCPC_REQOPTION_UPDATE,
    ATP_MSG_DHCPC_RENEW_UPDATE,
    ATP_MSG_DHCPC_DNS_CHANGE,    
} ;
typedef struct tagATP_DHCPC_OPTION43_MSG_ST
{
    VOS_CHAR acIfc[ATP_DHCPC_MAX_IFC_LEN];
    VOS_CHAR acUrl[DHCPC_OPTION_43_LEN];
} ATP_DHCPC_OPTION43_MSG_ST;

/*Start:y00250610 for DTS2013111602045 ：Hybrid工程页面上leaseTimeRemaining,T1Remaining, T2Remaining时间不对*/
typedef struct _TagRenewMsg 
{
	u_int32_t  ulYiaddr;            /* your ip address from the ACK of renew packet*/
    unsigned long ulLease;          /* the lease from the ACK renew packet*/
}DHCP_CLIENT_RENEW_MSG_ST;
/*End:y00250610 for DTS2013111602045 ：Hybrid工程页面上leaseTimeRemaining,T1Remaining, T2Remaining时间不对*/

#endif // End of __ATP_DHCPC_MSGTYPES_H__

