/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2008, ��Ϊ�������޹�˾.
  �ļ���: qmihdr.h
  ����: houxiaohe 00118013
  �汾: 001
  ��������: 2008-5-21
  �������: 2008-5-25
  ��������: qmihdr.hͷ�ļ��ж����ˣ�QMITTYģ���ڽ���Rm Net����������ʹ�õ�
            �ṹ�ͺꡣ�����漰��QMI�Ľṹ������Ӹ�ͨ�Ĵ�����ĵ�����ֲ����
            ͬʱΪ����Ӧ��Linuxƽ̨�Ͽ���ʱ���ֽڶ������⣬ʹ�����ֽڶ���
            ��ǿ�����ԡ�

  ��Ҫ�����б�:

  �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    houxiaohe   2008-07-18  B000    �����Set_Instance_ID������ͷ�������Ϣ�ṹ
                                    ����� SET_QMI_INSTANCE_ID �ĺ궨��
    houxiaohe   2008-07-23  B000    ɾ����148��149�е��ظ��궨��

  ��ע:
 ************************************************************************/
#ifndef _QMIHDR_H
#define _QMIHDR_H


// ========= USB Control Message ==========

#define USB_CTL_MSG_TYPE_QMI 0x01

#define QMICTL_SUPPORTED_MAJOR_VERSION 1
#define QMICTL_SUPPORTED_MINOR_VERSION 0

typedef unsigned char  UCHAR;
typedef unsigned short USHORT;

typedef enum _QMI_SERVICE_TYPE
{
    QMUX_TYPE_CTL = 0x00,
    QMUX_TYPE_WDS = 0x01,
    QMUX_TYPE_DMS = 0x02,
    QMUX_TYPE_NAS = 0x03,
    QMUX_TYPE_QOS = 0x04,
    QMUX_TYPE_MAX,
    QMUX_TYPE_ALL = 0xFF
} QMI_SERVICE_TYPE;

typedef enum _QMI_RESULT_CODE_TYPE
{
    QMI_RESULT_SUCCESS = 0x0000,
    QMI_RESULT_FAILURE = 0x0001
} QMI_RESULT_CODE_TYPE;

typedef enum _QMI_ERROR_CODE_TYPE
{
    QMI_ERR_NONE = 0x0000,
    QMI_ERR_CLIENT_IDS_EXHAUSTED = 0x0005,
    QMI_ERR_DENIED = 0x0006,
    QMI_ERR_INVALID_CLIENT_IDS = 0x0007,
    QMI_ERR_NO_BATTERY = 0x0008,
    QMI_ERR_INVALID_HANDLE   = 0x0009,
    QMI_ERR_INVALID_PROFILE  = 0x000A,
    QMI_ERR_STORAGE_EXCEEDED = 0x000B,
    QMI_ERR_VOICE_NOT_SUPPORTED = 0x000C,
    QMI_ERR_NO_NETWORK  = 0x000D,
    QMI_ERR_PIN_LOCKED  = 0x000E,
    QMI_ERR_OUT_OF_CALL = 0x000F,
    QMI_ERR_NOT_PROVISIONED = 0x0010,
    QMI_ERR_INVALID_ARG = 0x0020,
    QMI_ERR_NO_MEMORY = 0x0021,
    QMI_ERR_INTERNAL = 0x0022,
    QMI_ERR_FAULT = 0x0023
} QMI_ERROR_CODE_TYPE;

/*QMI��Ϣ�ṹ��ͷ����*/
struct _QCQMI_HDR
{
    UCHAR  IFType;  /*QMUX�ӿڵ����ͱ�־���̶��� 1 */
    USHORT Length;  /*������Ϣ�ĳ��ȼ�1*/
    UCHAR  CtlFlags;/*��־��Ϣ���ͷ��ǿ��Ƶ㻹������ģ��*/
    UCHAR  QMIType; /*QMI���������*/
    UCHAR  ClientId;/*��Ӧ��QMI�����Client ID*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMI_HDR QCQMI_HDR, * PQCQMI_HDR;

#define SDU_START_POSITION 6

struct _QCQMI
{
    UCHAR  IFType;  /*QMUX�ӿڵ����ͱ�־���̶��� 1 */
    USHORT Length;  /*������Ϣ�ĳ��ȼ�1*/
    UCHAR  CtlFlags;/*��־��Ϣ���ͷ��ǿ��Ƶ㻹������ģ��*/
    UCHAR  QMIType; /*QMI���������*/
    UCHAR  ClientId;/*��Ӧ��QMI�����Client ID*/
    UCHAR  SDU;     /*SDU��Ϣ�ṹ�����ַ�*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMI QCQMI, * PQCQMI;

struct _QMI_SERVICE_VERSION
{
    USHORT Major;       /*Э��ջ�汾��Ϣ�����汾��*/
    USHORT Minor;       /*Э��ջ�汾��Ϣ�ĸ��汾��*/
    USHORT AddendumMajor;   /*����Э��汾��Ϣ�����汾��*/
    USHORT AddendumMinor;   /*����Э��汾��Ϣ�ĸ��汾��*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_SERVICE_VERSION QMI_SERVICE_VERSION, * PQMI_SERVICE_VERSION;

// QMICTL Control Flags
#define QMICTL_CTL_FLAG_CMD 0x00    /*ָʾCTL��Ϣ��SDU����Ϣ����������*/
#define QMICTL_CTL_FLAG_RSP 0x01    /*ָʾCTL��Ϣ��SDU����Ϣ�����Ƿ���*/
#define QMICTL_CTL_FLAG_IND 0x02    /*ָʾCTL��Ϣ��SDU����Ϣ������ָʾ*/

#define QMICTL_FLAG_REQUEST 0x00
#define QMICTL_FLAG_RESPONSE 0x01
#define QMICTL_FLAG_INDICATION 0x02

#define QCQMI_CTL_FLAG_SERVICE 0x80   /*���Ƶ㷢��QMI��Ϣ, ����QMI��Ϣͷ�е�CtlFlag*/
#define QCQMI_CTL_FLAG_CTL_POINT 0x00   /*����ģ�鷢��QMI��Ϣ������QMI��Ϣͷ�е�CtlFlag*/

// QMICTL Type
#define QMICTL_SET_INSTANCE_ID_REQ 0x0020    /*����������SDU��QMUX��Ϣ��������������ʵ��������*/
#define QMICTL_SET_INSTANCE_ID_RESP 0x0020    /*����������SDU��QMUX��Ϣ��������������ʵ���ķ���*/
#define QMICTL_GET_VERSION_REQ 0x0021    /*����������SDU��QMUX��Ϣ�����ǻ�ȡ�汾��Ϣ������*/
#define QMICTL_GET_VERSION_RESP 0x0021    /*����������SDU��QMUX��Ϣ�����ǻ�ȡ�汾��Ϣ�ķ���*/
#define QMICTL_GET_CLIENT_ID_REQ 0x0022    /*����������SDU��QMUX��Ϣ�����ǻ�ȡClient ID������*/
#define QMICTL_GET_CLIENT_ID_RESP 0x0022    /*����������SDU��QMUX��Ϣ�����ǻ�ȡClient ID�ķ���*/
#define QMICTL_RELEASE_CLIENT_ID_REQ 0x0023    /*����������SDU��QMUX��Ϣ�������ͷ�Client ID������*/
#define QMICTL_RELEASE_CLIENT_ID_RESP 0x0023    /*����������SDU��QMUX��Ϣ�������ͷ�Client ID������*/
#define QMICTL_REVOKE_CLIENT_ID_IND 0x0024    /*����������SDU��QMUX��Ϣ�������ؼ���Client ID��ָʾ*/
#define QMICTL_INVALID_CLIENT_ID_IND 0x0025    /*����������SDU��QMUX��Ϣ������ʧЧ��Client ID��ָʾ*/
#define QMICTL_SET_DATA_FORMAT_REQ 0x0026    /*����������SDU��QMUX��Ϣ�������������ݸ�ʽ������*/
#define QMICTL_SET_DATA_FORMAT_RESP 0x0026    /*����������SDU��QMUX��Ϣ�������������ݸ�ʽ�ķ���*/
#define QMICTL_GET_QOS_NETWORK_STATUS_REQ 0x0027   /*QOS������SDU��QMUX����Ϣ���ͻ�ȡ�����Ƿ�֧��QOS*/
#define QMICTL_GET_QOS_NETWORK_STATUS_RESP 0x0027  /*QOS������SDU��QMUX����Ϣ���ͻ�ȡ�����Ƿ�֧��QOS��Ϣ����*/

// QMUX Message Definitions -- QMI SDU
#define QMUX_CTL_FLAG_SINGLE_MSG 0x00    /*��������ģ�����Ϣ�ǵ�һ����Ϣ*/
#define QMUX_CTL_FLAG_COMPOUND_MSG 0x01    /*��������ģ�����Ϣ����ϵ���Ϣ*/
#define QMUX_CTL_FLAG_TYPE_CMD 0x00    /*��������ģ�����Ϣ������*/
#define QMUX_CTL_FLAG_TYPE_RSP 0x02    /*��CTL��QMUX��SDU��Ϣ�Ƿ���*/
#define QMUX_CTL_FLAG_TYPE_IND 0x04    /*��CTL��QMUX��SDU��Ϣ��ָʾ*/
#define QMUX_CTL_FLAG_MASK_COMPOUND 0x01    /*QMUX�Ŀ�����Ϣ����*/
#define QMUX_CTL_FLAG_MASK_TYPE 0x06 // 00-cmd, 01-rsp, 10-ind

#define QCTLV_TYPE_REQUIRED_PARAMETER 0x01

#define QMUX_BROADCAST_CID 0xFF

#define QMI_WDS_START_NETWORK_INTERFACE 0x0020

struct _QMICTL_GET_VERSION_REQ_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_REQUEST
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_GET_VERSION_REQ
    USHORT Length;         // 0
//    UCHAR  TLVType;        // QCTLV_TYPE_REQUIRED_PARAMETER
//    USHORT TLVLength;      // var
//    UCHAR  QMUXTypes;      // List of one byte QMUX_TYPE values
                           // 0xFF returns a list of versions for all
                           // QMUX_TYPEs implemented on the device
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_GET_VERSION_REQ_MSG QMICTL_GET_VERSION_REQ_MSG, * PQMICTL_GET_VERSION_REQ_MSG;

struct _QMUX_TYPE_VERSION_STRUCT
{
    UCHAR  QMUXType;    /*Э��ջ����*/
    USHORT MajorVersion;
    USHORT MinorVersion;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMUX_TYPE_VERSION_STRUCT QMUX_TYPE_VERSION_STRUCT, * PQMUX_TYPE_VERSION_STRUCT;

struct _ADDENDUM_VERSION_PREAMBLE
{
    UCHAR LabelLength;      /*������Ϣ����*/
    UCHAR Label;        /*������Ϣ�����ֽ�*/
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _ADDENDUM_VERSION_PREAMBLE ADDENDUM_VERSION_PREAMBLE, * PADDENDUM_VERSION_PREAMBLE;

struct _QMI_TLV_HDR
{
    UCHAR  TLVType;
    USHORT TLVLength;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_TLV_HDR QMI_TLV_HDR, * PQMI_TLV_HDR;

#define QMICTL_GET_VERSION_RSP_TLV_TYPE_VERSION 0x01
#define QMICTL_GET_VERSION_RSP_TLV_TYPE_ADD_VERSION 0x10

struct _QCQMICTL_MSG_HDR
{
    UCHAR  CtlFlags; // 00-cmd, 01-rsp, 10-ind
    UCHAR  TransactionId;
    USHORT QMICTLType;
    USHORT Length;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMICTL_MSG_HDR QCQMICTL_MSG_HDR, * PQCQMICTL_MSG_HDR;

struct _QCQMICTL_MSG
{
    UCHAR  CtlFlags; // 00-cmd, 01-rsp, 10-ind
    UCHAR  TransactionId;   /*ҵ����*/
    USHORT QMICTLType;
    USHORT Length;
    UCHAR  Payload;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMICTL_MSG QCQMICTL_MSG, * PQCQMICTL_MSG;

struct _QMICTL_GET_VERSION_RESP_MSG
{
    UCHAR                    CtlFlags; // QMICTL_FLAG_RESPONSE
    UCHAR                    TransactionId;
    USHORT                   QMICTLType; // QMICTL_GET_VERSION_RESP
    USHORT                   Length;
    UCHAR                    TLVType; // QCTLV_TYPE_RESULT_CODE
    USHORT                   TLVLength; // 0x0004
    USHORT                   QMIResult;
    USHORT                   QMIError;
    UCHAR                    TLV2Type; // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT                   TLV2Length; // var
    UCHAR                    NumElements; // Num of QMUX_TYPE_VERSION_STRUCT
    QMUX_TYPE_VERSION_STRUCT TypeVersion;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_GET_VERSION_RESP_MSG QMICTL_GET_VERSION_RESP_MSG, * PQMICTL_GET_VERSION_RESP_MSG;

struct _QMICTL_SET_INSTANCE_ID_REQ_MSG
{
   UCHAR  CtlFlags;        // QMICTL_FLAG_REQUEST
   UCHAR  TransactionId;
   USHORT QMICTLType;      // QMICTL_SET_INSTANCE_ID_REQ
   USHORT Length;          // 4
   UCHAR  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
   USHORT TLVLength;       // 1
   UCHAR  Value;           // Host-unique QMI instance for this device driver
}__attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_SET_INSTANCE_ID_REQ_MSG QMICTL_SET_INSTANCE_ID_REQ_MSG, *PQMICTL_SET_INSTANCE_ID_REQ_MSG;

struct _QMICTL_SET_INSTANCE_ID_RESP_MSG
{
   UCHAR  CtlFlags;        // QMICTL_FLAG_RESPONSE
   UCHAR  TransactionId;
   USHORT QMICTLType;      // QMICTL_SET_INSTANCE_ID_RESP
   USHORT Length;
   UCHAR  TLVType;         // QCTLV_TYPE_RESULT_CODE
   USHORT TLVLength;       // 0x0004
   USHORT QMIResult;
   USHORT QMIError;
   UCHAR  TLV2Type;        // QCTLV_TYPE_REQUIRED_PARAMETER
   USHORT TLV2Length;      // 0x0002
   USHORT QMI_ID;          // Upper byte is assigned by MSM,
                           // lower assigned by host 
}__attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_SET_INSTANCE_ID_RESP_MSG QMICTL_SET_INSTANCE_ID_RESP_MSG, *PQMICTL_SET_INSTANCE_ID_RESP_MSG;

struct _QMICTL_GET_CLIENT_ID_REQ_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_REQUEST
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_GET_CLIENT_ID_REQ
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT TLVLength;      // 1
    UCHAR  QMIType;        // QMUX type
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_GET_CLIENT_ID_REQ_MSG QMICTL_GET_CLIENT_ID_REQ_MSG, * PQMICTL_GET_CLIENT_ID_REQ_MSG;

struct _QMICTL_GET_CLIENT_ID_RESP_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_RESPONSE
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_GET_CLIENT_ID_RESP
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_RESULT_CODE
    USHORT TLVLength;      // 0x0004
    USHORT QMIResult;      // result code
    USHORT QMIError;       // error code
    UCHAR  TLV2Type;       // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT TLV2Length;     // 2
    UCHAR  QMIType;
    UCHAR  ClientId;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_GET_CLIENT_ID_RESP_MSG QMICTL_GET_CLIENT_ID_RESP_MSG, * PQMICTL_GET_CLIENT_ID_RESP_MSG;

struct _QMICTL_RELEASE_CLIENT_ID_REQ_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_REQUEST
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_RELEASE_CLIENT_ID_REQ
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT TLVLength;      // 0x0002
    UCHAR  QMIType;
    UCHAR  ClientId;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_RELEASE_CLIENT_ID_REQ_MSG QMICTL_RELEASE_CLIENT_ID_REQ_MSG, * PQMICTL_RELEASE_CLIENT_ID_REQ_MSG;

struct _QMICTL_RELEASE_CLIENT_ID_RESP_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_RESPONSE
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_RELEASE_CLIENT_ID_RESP
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_RESULT_CODE
    USHORT TLVLength;      // 0x0004
    USHORT QMIResult;      // result code
    USHORT QMIError;       // error code
    UCHAR  TLV2Type;       // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT TLV2Length;     // 2
    UCHAR  QMIType;
    UCHAR  ClientId;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_RELEASE_CLIENT_ID_RESP_MSG QMICTL_RELEASE_CLIENT_ID_RESP_MSG,
* PQMICTL_RELEASE_CLIENT_ID_RESP_MSG;

struct _QMICTL_SET_DATA_FORMAT_REQ_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_REQUEST
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_SET_DATA_FORMAT_REQ
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_REQUIRED_PARAMETER
    USHORT TLVLength;      // 1
    UCHAR  DataFormat;     // 0-default; 1-QoS hdr present
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_SET_DATA_FORMAT_REQ_MSG QMICTL_SET_DATA_FORMAT_REQ_MSG, * PQMICTL_SET_DATA_FORMAT_REQ_MSG;

struct _QMICTL_SET_DATA_FORMAT_RESP_MSG
{
    UCHAR  CtlFlags;       // QMICTL_FLAG_RESPONSE
    UCHAR  TransactionId;
    USHORT QMICTLType;     // QMICTL_SET_DATA_FORMAT_RESP
    USHORT Length;
    UCHAR  TLVType;        // QCTLV_TYPE_RESULT_CODE
    USHORT TLVLength;      // 0x0004
    USHORT QMIResult;      // result code
    USHORT QMIError;       // error code
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_SET_DATA_FORMAT_RESP_MSG QMICTL_SET_DATA_FORMAT_RESP_MSG, * PQMICTL_SET_DATA_FORMAT_RESP_MSG;

struct _QMICTL_MSG
{
    union
    {
        // Message Header
        QCQMICTL_MSG_HDR QMICTLMsgHdr;

        // QMICTL Message
       QMICTL_SET_INSTANCE_ID_REQ_MSG               SetInstanceIdReq;
        QMICTL_SET_INSTANCE_ID_RESP_MSG              SetInstanceIdRsp;
        QMICTL_GET_VERSION_REQ_MSG        GetVersionReq;
        QMICTL_GET_VERSION_RESP_MSG       GetVersionRsp;
        QMICTL_GET_CLIENT_ID_REQ_MSG      GetClientIdReq;
        QMICTL_GET_CLIENT_ID_RESP_MSG     GetClientIdRsp;
        QMICTL_RELEASE_CLIENT_ID_REQ_MSG  ReleaseClientIdReq;
        QMICTL_RELEASE_CLIENT_ID_RESP_MSG ReleaseClientIdRsp;

        //QMICTL_REVOKE_CLIENT_ID_IND_MSG              RevokeClientIdInd;
        //QMICTL_INVALID_CLIENT_ID_IND_MSG             InvalidClientIdInd;
        QMICTL_SET_DATA_FORMAT_REQ_MSG  SetDataFormatReq;
        QMICTL_SET_DATA_FORMAT_RESP_MSG SetDataFormatRsp;
    }un_msg;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMICTL_MSG QMICTL_MSG, * PQMICTL_MSG;

#define uGetVersionRsp  un_msg.GetVersionRsp
#define uGetClientIdRsp un_msg.GetClientIdRsp
#define uReleaseClientIdRsp un_msg.ReleaseClientIdRsp

// ======================= QOS ==============================

#define QMI_QOS_SET_EVENT_REPORT_REQ 0x0001
#define QMI_QOS_SET_EVENT_REPORT_RESP 0x0001
#define QMI_QOS_EVENT_REPORT_IND 0x0001

struct _QMI_QOS_SET_EVENT_REPORT_REQ_MSG
{
    USHORT Type;            // QMUX type 0x0001
    USHORT Length;

    // UCHAR  TLVType;          // 0x01 - physical link state
    // USHORT TLVLength;        // 1
    // UCHAR  PhyLinkStatusRpt; // 0-enable; 1-disable
    UCHAR  TLVType2;        // 0x02 = global flow reporting
    USHORT TLVLength2;      // 1
    UCHAR  GlobalFlowRpt;   // 1-enable; 0-disable
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_QOS_SET_EVENT_REPORT_REQ_MSG QMI_QOS_SET_EVENT_REPORT_REQ_MSG, * PQMI_QOS_SET_EVENT_REPORT_REQ_MSG;

struct _QMI_QOS_SET_EVENT_REPORT_RESP_MSG
{
    USHORT Type;            // QMUX type 0x0010
    USHORT Length;
    UCHAR  TLVType;         // 0x02 - result code
    USHORT TLVLength;       // 4
    USHORT ResultCode;      // QMUX_RESULT_SUCCESS
                            // QMUX_RESULT_FAILURE
    USHORT ErrorCode;       // QMUX_ERR_INVALID_ARG
                            // QMUX_ERR_NO_MEMORY
                            // QMUX_ERR_INTERNAL
                            // QMUX_ERR_FAULT
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_QOS_SET_EVENT_REPORT_RESP_MSG QMI_QOS_SET_EVENT_REPORT_RESP_MSG,
* PQMI_QOS_SET_EVENT_REPORT_RESP_MSG;

struct _QMI_QOS_GET_QOS_NETWORK_STATUS_RESP_MSG
{
    USHORT Type;               //  QMUX type 0x0027
    USHORT Length;
    UCHAR  TLVType;         // 0x02 - result code
    USHORT TLVLength;      // 4
    USHORT ResultCode;
    USHORT ErrorCode;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMI_QOS_GET_QOS_NETWORK_STATUS_RESP_MSG QMI_QOS_GET_QOS_NETWORK_STATUS_RESP_MSG,
* PQMI_QOS_GET_QOS_NETWORK_STATUS_RESP_MSG;

struct _QCQMUX
{
    UCHAR  CtlFlags;     // 0: single QMUX Msg; 1:
    USHORT TransactionId;
    UCHAR  Message; // Type(2), Length(2), Value
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMUX QCQMUX, * PQCQMUX;

struct _QCQMUX_MSG_HDR
{
    USHORT Type;
    USHORT Length;
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QCQMUX_MSG_HDR QCQMUX_MSG_HDR, * PQCQMUX_MSG_HDR;

struct _QMUX_MSG
{
    union
    {
        // Message Header
        QCQMUX_MSG_HDR QMUXMsgHdr;

        // QMIQOS Messages
        QMI_QOS_SET_EVENT_REPORT_REQ_MSG  QosSetEventReportReq;
        QMI_QOS_SET_EVENT_REPORT_RESP_MSG QosSetEventReportRsp;

        //QMI_QOS_EVENT_REPORT_IND_MSG              QosEventReportInd;
    };
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _QMUX_MSG QMUX_MSG, * PQMUX_MSG;

struct _USB_NOTIFICATION_STATUS
{
    UCHAR  bmRequestType;
    UCHAR  bNotification;   /*�Զ�ȡ�ж�ͨ���������Ҫ���ж�ֵ*/
    USHORT wValue;
    USHORT wIndex;  // interface #
    USHORT wLength; // number of data bytes
    USHORT usValue; // serial status, etc.
} __attribute__((packed, aligned(sizeof(char))));
typedef struct _USB_NOTIFICATION_STATUS USB_NOTIFICATION_STATUS, * PUSB_NOTIFICATION_STATUS;

typedef enum    _START_TRANSACTION_TYPE
{
    GET_QMI_VERSION_INFO = 0x00,
    GET_INTERNAL_WDS_CLIENTID = 0x01,
    SET_QOS_DATAFORMAT = 0x02,
    GET_INTERNAL_QOS_CLIENTID = 0x03,
    SET_QOS_REPORT_EVENT = 0x04,
    QMI_QOS_GET_NETWORK_STATUS = 0x05,
    TRANSACTION_TYPE_MAX = 0x06
}START_TRANSACTION_TYPE;

#define RELEASE_CLIENT_ID_TRANSACTION 0x07

/* <AU3D00181.QMITTY: houxiaohe 2008-7-18 ADD BEGIN */
#define SET_QMI_INSTANCE_ID 0x08
/* AU3D00181.QMITTY: houxiaohe 2008-7-18 ADD END> */

#endif //end of the _QMIHDR_H
