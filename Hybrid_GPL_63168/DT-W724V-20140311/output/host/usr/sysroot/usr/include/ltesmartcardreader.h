#ifndef __ATP_SMARTCARD_API_H__
#define __ATP_SMARTCARD_API_H__

#include "atputil.h"
#define LTE_SMARTCARDREADER_LEN_64 64
typedef struct  tagLTE_SMARTCARD_INFO_ST
{
    VOS_CHAR acStatus[LTE_SMARTCARDREADER_LEN_64];
    VOS_CHAR acApplication[LTE_SMARTCARDREADER_LEN_64];
}LTE_SMARTCARD_INFO_ST;

typedef struct tagLTE_SMARTCARDREADER_INFO_ST    
{
    VOS_BOOL bEnable;
    VOS_UINT32 ulDbIndex;
    VOS_CHAR acStatus[LTE_SMARTCARDREADER_LEN_64];
    VOS_CHAR acResetTime[LTE_SMARTCARDREADER_LEN_64];
    LTE_SMARTCARD_INFO_ST stSmartcard;
}LTE_SMARTCARDREADER_INFO_ST;
typedef struct tagLTE_SMARTCARDREADER_NODE_ST    
{
    LTE_SMARTCARDREADER_INFO_ST stSmartcardReader;
    struct tagLTE_SMARTCARDREADER_NODE_ST *next;
}LTE_SMARTCARDREADER_NODE_ST;

/* smart card reader status*/
#define LTE_SMARTCARDREADER_STAT_ENABLE                     "Enable"
#define LTE_SMARTCARDREADER_STAT_DISABLE                  "Disable"
#define LTE_SMARTCARDREADER_STAT_ERROR                     "Error"

/* smart card status*/
#define LTE_SMARTCARD_STAT_NONE                     "None"
#define LTE_SMARTCARD_STAT_RUNNING             "Running"
#define LTE_SMARTCARD_STAT_Error                        "Error"

#if defined(__cplusplus)
extern "C" {
#endif
VOS_UINT32 ATP_LTE_SmartcardReader_Init( );
#if defined(__cplusplus)
}
#endif

#endif
