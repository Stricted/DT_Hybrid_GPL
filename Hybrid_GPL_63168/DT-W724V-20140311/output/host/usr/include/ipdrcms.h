
/****************************************************************************
 * File Name:     ipdrcms.h
 * Author:        ATP team
 * Date:           2013/9/10
 * Descripton:   IPDR cms inner interface declarations
 *****************************************************************************/

#ifndef _IPDR_CMS_H
#define _IPDR_CMS_H

#include "atptypes.h"
#include "ipdrcmsapi.h"


#if defined(__cplusplus)
extern "C" {
#endif

typedef enum tagATP_E_ERR_IPDRCMS
{
    /*CFM����Ĵ�����*/
    ATP_E_ERR_IPDRCMS_PARA_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_TR232CMS),
    ATP_E_ERR_IPDRCMS_NOT_INIT,/*!< û�г�ʼ�� */
    ATP_E_ERR_IPDRCMS_INTER,
    ATP_E_ERR_IPDRCMS_VALUE_INVALID,
    ATP_E_ERR_IPDRCMS_VALUE_EXIST,
    ATP_E_ERR_IPDRCMS_MAX
}ATP_E_ERR_IPDRCMS;

/******************* define erea ************************/
#define ATP_STRING_ENABLED      "Enabled"
#define ATP_STRING_DISABLED     "Disabled"
#define ATP_IPDR_ALIAS_PREFIX   "cpe-"
#define ATP_IPDR_BDENABLE_PATH  "InternetGatewayDevice.Services.X_ATP_BulkData.Enable"

/******************* define protocol type ************************/
#define IPDR_STEAMING       "IPDR_STREAMING"
#define IPDR_FILE           "IPDR_FILE"

/******************* define encoding type ************************/
#define IPDR_XML            "IPDR_XML"
#define IPDR_XDR            "IPDR_XDR"

/******************* define ipdr status ************************/
#define IPDR_ENABLED        "IPDR_ENABLED"
#define IPDR_DISABLED       "IPDR_DISABLED"

/******************* define ipdr sessionid scope '0'-'9' or 'A'-'Z'****/
#define IPDR_SESSIONID_0               48
#define IPDR_SESSIONID_9               57
#define IPDR_SESSIONID_A               65
#define IPDR_SESSIONID_Z               90

/******************* define ipdr parameter length*****************/
#define ATP_IPDR_STATUS_LEN            16
#define ATP_IPDR_ALIAS_LEN             64
#define ATP_IPDR_PROTOCOL_LEN          20
#define ATP_IPDR_ENCODINGTYPE_LEN      5
#define ATP_IPDR_STRHOST_LEN           256
#define ATP_IPDR_REFERENCE_LEN         256

#define ATP_IPDR_BULKDATA_PROTOCOLS    "Streaming"
#define ATP_IPDR_BULKDATA_ENCODETYPE   "XDR"

#define ATP_IPDR_ZERO_RPTINTVAL        0                      /*�ϱ�ʱ��Ϊ0*/
#define ATP_IPDR_MAX_STREAMPORT        65535                  /*���streaming�˿ں�*/
#define ATP_IPDR_PATH_LEN              256

/******************* define ipdr common length*****************/
#define IPDR_MAX_WAN_PATH              129
#define IPDR_CMD_LEN                   128
#define IPDR_PROCESS_NAME              "ipdr"



/*ipdr �ṹ*/
typedef struct tagATP_IPDR_INFO_ST
{
    /*BulkData Profile ʵ����*/
    VOS_UINT32 ulBlkDtProfInstId;

    /*BulkData ʹ�ܱ�־*/
    VOS_UINT32 ulEnable;
    
    /*BulkData Profile ʵ��ʹ�ܱ�־*/
    VOS_UINT32 ulProfEnable;

    /*״̬*/
    VOS_CHAR   acStatus[ATP_IPDR_STATUS_LEN];

    /*��Profile�ı���*/
    VOS_CHAR   acAlias[ATP_IPDR_ALIAS_LEN];

    /*Э������*/
    VOS_CHAR   acProtocol[ATP_IPDR_PROTOCOL_LEN];

    /*��������*/
    VOS_CHAR   acEncodingType[ATP_IPDR_ENCODINGTYPE_LEN];

    /*�ϱ�ʱ����*/
    VOS_UINT32 ulReportingIntval ;

    /*��������*/
    VOS_CHAR   acStreamingHost[ATP_IPDR_STRHOST_LEN];

    /*���˿ں�*/
    VOS_UINT32 ulStreamingPort;

    /*�ỰSessionid*/
    VOS_UINT32 ulStreamingSessionId;

    /*Profileʵ���ĸ���*/
    VOS_UINT32 ulParaNumOfEntries ;

    /**/
    VOS_CHAR   acReference[ATP_IPDR_REFERENCE_LEN];

    struct tagATP_IPDR_INFO_ST *pstNext;
}ATP_IPDR_INFO_ST;

/******************* function erea ************************/
VOS_UINT32 IPDR_FindInList(const VOS_CHAR *pcInput, const VOS_CHAR *pcList);
VOS_UINT32 IpdrCheckAlias(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],const VOS_CHAR* pcInput); 
VOS_UINT32 IpdrCheckProtocol(const VOS_CHAR* pcInput);
VOS_UINT32 IpdrCheckEncodingType(const VOS_CHAR* pcInput);
VOS_UINT32 IpdrCheckReportingInterval(VOS_UINT32 ulInput);
VOS_UINT32 IpdrCheckTimeReference(const VOS_CHAR *pcInput);
VOS_BOOL   IpdrCheckValidateUrl(const VOS_CHAR *pcUrl);
VOS_UINT32 IpdrCheckStreamingHost(const VOS_CHAR *pcInput);
VOS_UINT32 IpdrCheckStreamingSessionID(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                                        const VOS_UINT32 ulInput);
VOS_UINT32 IpdrCheckStreamingPort(VOS_UINT32 ulInput);
VOS_UINT32 IpdrCheckAlias(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                const VOS_CHAR *pcInput);
VOS_UINT32 IpdrCheckBDProfileParaRef(const VOS_CHAR *pcInput);
VOS_UINT32 IpdrWanUpProc(VOS_VOID *pcWanPath, VOS_VOID *pvHook);
VOS_UINT32 IpdrWanDownProc(VOS_VOID *pcWanPath, VOS_VOID *pvHook);
VOS_UINT32 IsValidateAlias(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH], const VOS_CHAR *pcInput);
VOS_UINT32 IpdrCheckWanPath(const VOS_CHAR *pcWanPath);
VOS_UINT32 IpdrCheckBulkDataEnable();
VOS_VOID   IpdrProfilechanged(VOS_VOID *pstIpdrMsg);














//#define ATP_DEBUG 1

#ifdef ATP_DEBUG
#define ATP_IPDRCMS_DEBUG(format, args...)       {printf("FILE: %s FUNCTION %s LINE: %d: ", __FILE__, __FUNCTION__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_IPDRCMS_DEBUG(x...)
#endif

#if defined(__cplusplus)
}
#endif


#endif
