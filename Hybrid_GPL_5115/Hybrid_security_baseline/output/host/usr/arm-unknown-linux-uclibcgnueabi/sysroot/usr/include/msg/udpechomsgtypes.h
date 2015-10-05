/*!
  \brief 需要发送的消息
  \param[in] ATP_MODULE_UDPECHO: 
  \return 
  \retval
*/
#ifndef __ATP_UDPECHO_MSGTYPES_H__
#define __ATP_UDPECHO_MSGTYPES_H__

#ifdef SUPPORT_ATP_TR143_UDPECHO

#ifndef ATP_CBBID_UDPECHO_NAME
#define ATP_CBBID_UDPECHO_NAME "udpecho"
#endif //ATP_CBBID_UDPECHO_NAME

enum ATP_UDPECHO_MSGTYPES_EN
{
     ATP_MSG_UDPE_SAVE_DIAG_RECORD = ATP_MSG_CATEGORY_DEF(ATP_MODULE_UDPECHO),
} ;
#endif

typedef struct tagATP_UDPE_DIAG_RECORD_ST
{   
    VOS_UINT32          ulPacketsReceived;
    VOS_UINT32          ulPacketsResponded;
    VOS_UINT32          ulBytesReceived;
    VOS_UINT32          ulBytesResponded;
    VOS_CHAR            scTimeFirstPacketReceived[32];/*2008-04-09T15:01:05.123456     26 BYTES*/
    VOS_CHAR            scTimeLastPacketReceived[32];
}ATP_UDPE_DIAG_RECORD_ST;

#endif

