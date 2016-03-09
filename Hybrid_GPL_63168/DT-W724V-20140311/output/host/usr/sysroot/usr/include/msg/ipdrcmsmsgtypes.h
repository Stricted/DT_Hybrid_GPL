#ifndef __ATP_IPDRCMS_MSGTYPES_H__
#define __ATP_IPDRCMS_MSGTYPES_H__

#define ATP_IPDR_HOST_LEN           256

typedef struct tagATP_IPDR_MSG_ST
{
    /*BulkData Profile ʵ����*/
    VOS_UINT32 InstanceId;
    VOS_UINT32 ulInterval;
    /*��������*/
    VOS_CHAR   acStreamingHost[ATP_IPDR_HOST_LEN];
    /*���˿ں�*/
    VOS_UINT32 ulStreamingPort;
}ATP_IPDR_MSG_ST;

enum ATP_IPDRCMS_MSGTYPES_EN
{
    ATP_MSG_IPDR_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_TR232CMS),
    ATP_MSG_IPDR_BULK_PROFILE_ADD,
    ATP_MSG_IPDR_BULK_PROFILE_DEL,
    ATP_MSG_IPDR_BULK_PROFILE_ENABLE_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_ALIAS_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_REPORTINGINTERVAL_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_TIMEREFERENCE_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_STREAMINGHOST_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_STREAMINGPORT_CHANGE,
    ATP_MSG_IPDR_BULK_PROFILE_STREAMINGSESSIONID_CHANGE, 
    ATP_MSG_IPDR_BULK_PROFILE_REFERENCE_ADD,
    ATP_MSG_IPDR_BULK_PROFILE_REFERENCE_DEL,
    ATP_MSG_IPDR_BULK_PROFILE_REFERENCE_CHANGE,
    ATP_MSG_IPDR_IT_NODE_INFO,
} ;

#define ATP_CBBID_IPDR_NAME "ipdr"

#endif 
/*End of __ATP_IPDRCMS_MSGTYPES_H__*/
