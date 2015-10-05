/******************************************************************************
  版权所有  : 2011-2020，华为技术有限公司
  文 件 名  : oamlinkmonitorapi.h
  作    者  : l00135113
  版    本  : ATPV100R003C06
  创建日期  : 2011-10-15
  描    述  : 802.3ah(OAM)头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2011-8-25
     作    者   : l00135113
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_OAM_LINKMONITORAPI_H__
#define __ATP_OAM_LINKMONITORAPI_H__


/*Link Event TLVs*/
/*Errored Symbol Period Event TLV*/
#pragma pack(1)
typedef struct OAM_EVENT_ERRORDSYMBOLPERIOD_TLV_ST{
    VOS_UINT8   ucType;                      /* Event Type:0x01*/
    VOS_UINT8   ucLength;                      /* Length:0x28*/    
    VOS_UINT16  usTimeStamp;
    VOS_UINT64  udlErrordSymbolWindow;
    VOS_UINT64  udlErrordSymbolThreshold;
    VOS_UINT64  udlErrordSymbols;    
    VOS_UINT64  udlErrorTotal;
    VOS_UINT32  ulEventTotal; 
 }ATP_OAM_EVENT_ERRORDSYMBOLPERIOD_TLV_ST;
#pragma pack()

/*Errored Frame Event TLV*/
#pragma pack(1)
typedef struct OAM_EVENT_ERRORDFE_TLV_ST{
    VOS_UINT8   ucType;                      /* Event Type:0x01*/
    VOS_UINT8   ucLength;                      /* Length:0x1A*/    
    VOS_UINT16  usTimeStamp;
    VOS_UINT16  usErrordFrameWindow;
    VOS_UINT32  ulErrordFrameThreshold;
    VOS_UINT32  ulErrordFrames;    
    VOS_UINT64  udlErrorTotal;
    VOS_UINT32  ulEventTotal; 
 }ATP_OAM_EVENT_ERRORDFE_TLV_ST;
#pragma pack()


/*Errored Frame Period Event TLV*/
#pragma pack(1)
typedef struct OAM_EVENT_ERRORDFPE_TLV_ST{
    VOS_UINT8   ucType;                      /* Event Type:0x03*/
    VOS_UINT8   ucLength;                      /* Length:0x1C*/    
    VOS_UINT16  usTimeStamp;
    VOS_UINT32  ulErrordFrameWindow;
    VOS_UINT32  ulErrordFrameThreshold;
    VOS_UINT32  ulErrordFrames;    
    VOS_UINT64  udlErrorTotal;
    VOS_UINT32  ulEventTotal; 
 }ATP_OAM_EVENT_ERRORDFPE_TLV_ST;
#pragma pack()

/*Errored Frame Seconds Summary Event TLV*/ 
#pragma pack(1)
typedef struct OAM_EVENT_ERRORDFSSE_TLV_ST{
    VOS_UINT8   ucType;                      /* Event Type:0x04*/
    VOS_UINT8   ucLength;                      /* Length:0x12*/    
    VOS_UINT16  usTimeStamp;
    VOS_UINT16  usErrordFSSWindow;
    VOS_UINT16  usErrordFSSThreshold;
    VOS_UINT16  usErrordFSS;    
    VOS_UINT32  ulErrorTotal;
    VOS_UINT32  ulEventTotal; 
 }ATP_OAM_EVENT_ERRORDFSSE_TLV_ST;
#pragma pack()

/*记录上一次链路监控统计数据*/
typedef struct OAM_EVENT_LAST_INFO_ST{
    VOS_BOOL    bLinkMonitorActive;
    VOS_UINT16  usCurEventTimeStamp;     
    VOS_INT32   lFirstTimeSecs; 
    VOS_UINT32  ulSymbolEventTotal; 
    VOS_UINT32  ulErrorFSSTotal;
    VOS_UINT32  ulFSSEventTotal; 
    VOS_UINT64  udlErrorSymbolTotal;
 }ATP_OAM_EVENT_LAST_INFO_ST;

/* OAM 链路监控事件状态索引枚举*/
typedef enum tagOAM_EVENT_STATUS_EN
{
    OAM_EVENT_ERRORED_SPE = 0x01,  /*Errored Symbol Period Event*/ 
    OAM_EVENT_ERRORED_FE = 0x02,  /*Errored Frame Event*/ 
    OAM_EVENT_ERRORED_FPE = 0x04,  /*Errored Frame Period Event*/ 
    OAM_EVENT_ERRORED_FSSE = 0x08,  /*Errored Frame Seconds Summary Event*/ 

    OAM_EVENT_ERRORED_END 
}ATP_OAM_EVENT_STATUS_EN;

/* OAM 链路监控事件Type枚举*/
typedef enum tagOAM_EVENT_TYPE_EN
{
    OAM_EVENT_ERRORED_OVER_FLAG = 0x0,  /*Tlv 结束标识*/ 
    OAM_EVENT_ERRORED_TYPE_SPE = 0x01,  /*Errored Symbol Period Event*/ 
    OAM_EVENT_ERRORED_TYPE_FE = 0x02,  /*Errored Frame Event*/ 
    OAM_EVENT_ERRORED_TYPE_FPE = 0x03,  /*Errored Frame Period Event*/ 
    OAM_EVENT_ERRORED_TYPE_FSSE = 0x04,  /*Errored Frame Seconds Summary Event*/ 

    OAM_EVENT_ERRORED_TYPE_END 
}ATP_OAM_EVENT_TYPE_EN;

/*本端记录Event OAMPDU信息结构体*/
typedef struct OAM_EVENT_PDU_ST
{
    VOS_UINT16 usSendSeqNum;                    /*发送的Event OAMPDU的Sequence number*/
    VOS_UINT16 usRcvSeqNum;                    /*接收的Event OAMPDU的Sequence number*/
    VOS_UINT32 ulSendBufLen;                   /*发送的OAMPDU的报文长度*/
    VOS_CHAR   *pszSendBuff;                    /*需要发送的OAMPDU的起始地址*/ 
}ATP_OAM_EVENT_PDU_ST;

extern VOS_INT32 OAMLinkMonitorInit(VOS_VOID);
extern VOS_INT32 OAMLinkMonitorUnInit(VOS_VOID);
extern VOS_VOID OamLinkMonitorTimerProc(VOS_VOID *pvHook);

#endif

