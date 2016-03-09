/******************************************************************************
  版权所有  : 2011-2020，华为技术有限公司
  文 件 名  : oamsdkproc.h
  作    者  : l00135113
  版    本  : ATPV100R003C06
  创建日期  : 2011-8-15
  描    述  : 802.3ah(OAM)头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2011-8-25
     作    者   : l00135113
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_OAM_SDK_PROC_H__
#define __ATP_OAM_SDK_PROC_H__


#define OAM_STATS_INFO_FILE             "/var/oamptmstats"
#define SIOCOAMLOOPBACK 0x89E3 /*Set OAM loopback enable*/

/*OAMPDU
------------------------------------------------------------
| 6       | 6         |2   | 1     | 2   | 1  |42~1496 | 4 |   
------------------------------------------------------------
|Dest addr|Source addr|Type|SubType|Flags|Code|Data/Pad|CRC|
------------------------------------------------------------


-------------------------------------------------------
|  1  |  1   |  1   |   1     |   1  |  1   |    1    |
-------------------------------------------------------
|local|local |local |local    |remote|remote|remote   |
|lost |link  |stable|satisfied|state |stable|satisfied|
|link |status|      |         |valid |      |         |
|timer|      |      |         |      |      |         |
|done |      |      |         |      |      |         |
-------------------------------------------------------
*/
/* OAM Control状态索引枚举*/
typedef enum tagOAM_CONTR_STATE_EN
{
    OAM_CONTR_STATE_LOCALLOSTLINKTIMERDONE = 0x80,  /*local_lost_link_timer_done*/ 
    OAM_CONTR_STATE_LOCALLINKSTATUS = 0x40,  /*local_link_status*/ 
    OAM_CONTR_STATE_LOCALSTABLE = 0x20,  /*local_stable*/ 
    OAM_CONTR_STATE_LOCALSATISFIED = 0x10,  /*local_satisfied*/ 
    OAM_CONTR_STATE_REMOTESTATEVALID = 0x08,  /*remote_state_valid*/ 
    OAM_CONTR_STATE_REMOTESTABLE = 0x04,  /*remote_stable*/ 
    OAM_CONTR_STATE_REMOTESATISFIED= 0x02,  /*remote_satisfied*/    
    OAM_CONTR_STATE_RESERVED = 0x01,        /*reserver bit*/
    OAM_CONTR_STATE__END
}ATP_OAM_CONTR_STATE_EN;

/* OAMPDU Flags状态索引枚举*/
typedef enum tagOAM_FLAGS_STATE_EN
{
    OAM_FLAGS_STATE_LINKFAULT = 0x0001,  /*LINK FAULT*/ 
    OAM_FLAGS_STATE_DYINGASP = 0x0002,  /*DYING GASP*/ 
    OAM_FLAGS_STATE_CRITICALEVENT = 0x0004, /*CRITICAL EVENT*/
    OAM_FLAGS_STATE_LOCALEVALUATING = 0x0008,  /*LOCAL EVALUATING*/ 
    OAM_FLAGS_STATE_LOCALSTABLE = 0x0010,  /*LOCALSTABLE*/ 
    OAM_FLAGS_STATE_REMOTEEVALUATING = 0x0020, /*REMOTE EVALUATING*/
    OAM_FLAGS_STATE_REMOTESTABLE = 0x0040,  /*REMOTE STABLE*/     
    OAM_FLAGS_STATE_RESERVED = 0X1FF,
    
    OAM_FLAGS_STATE_END
}ATP_OAM_FLAGS_STATE_EN;

/*local_pdu状态枚举*/
typedef enum tagOAM_CONTR_LOCALPDU_STATE_EN
{
    OAM_LOCALPDU_STATE_RX_INFO = 1,  /*RX_INFO*/ 
    OAM_LOCALPDU_STATE_INFO,  /*INFO*/     
    OAM_LOCALPDU_STATE_LF_INFO,  /*LF_INFO*/     
    OAM_LOCALPDU_STATE_ANY,  /*ANY*/    
    OAM_LOCALPDU_STATE_END
}ATP_OAM_CONTR_LOCALPDU_STATE_EN;

/*control oam discovery状态枚举*/
typedef enum tagOAM_CONTR_DISC_STATE_EN
{
    OAM_CONTR_DISC_STATE_FAULT = 1,  /*FAULT*/ 
    OAM_CONTR_DISC_STATE_PASSIVEWAIT,  /*PASSIVEWAIT*/ 
    OAM_CONTR_DISC_STATE_ACTIVESENDLOCAL, /*ACTIVE SEND LOCAL*/
    OAM_CONTR_DISC_STATE_SENDLOCALREMOTE, /*SEDN LOCAL REMOTE*/
    OAM_CONTR_DISC_STATE_SENDLOCALREMOTEOK, /*SEND LOCAL REMOTE OK*/
    OAM_CONTR_DISC_STATE_SENDANY,  /*SEND ANY*/

    OAM_CONTR_DISC_STATE_END 
}ATP_OAM_CONTR_DISC_STATE_EN;

/*parser状态枚举*/
typedef enum tagOAM_PARSER_ACTION_EN
{
    OAM_PARSER_ACTION_FWD = 1,  /*FWD*/ 
    OAM_PARSER_ACTION_LB,  /*LB*/ 
    OAM_PARSER_ACTION_DISCARD,  /*DISCARD*/ 

    OAM_PARSER_ACTION_END 
}ATP_OAM_PARSER_ACTION_EN;

typedef struct OAM_INFO_ST
{
    VOS_BOOL   bActive;
    VOS_INT32  lSock;                          /* 全局socket句柄*/ 
    VOS_UINT32 ulSendBufLen;                   /*发送的OAMPDU的报文长度*/
    VOS_CHAR   *pszSendBuff;                    /*需要发送的OAMPDU的起始地址*/
    struct sockaddr_ll addr;                   /* 本地地址信息*/ 

    ATP_OAM_PDU_ST *pstLocalOamPduComm;      /*OAMPDU报文公共信息(DA/SA/Type/Subtype)*/
    ATP_OAM_INFO_TLV_ST *pstLocalTlvInfo;    /*Local information tlv*/
    ATP_OAM_INFO_TLV_ST *pstLastRemoteTlvInfo; /*最近一次收到的correct information tlv*/
    
}ATP_OAM_INFO_ST;

/*记录xDsl统计信息*/
typedef struct
{
    VOS_UINT ulReceiveBlocks;              /* 自最近 DSL Showtime 起已成功接收的超级帧数   */
    VOS_UINT ulTransmitBlocks;             /* 自最近DSL显示时间起已发送成功的超级帧数      */
    VOS_UINT ulCellDelin;                  /* 自最近DSL显示时间起发生的信元头定界错误数    */
    VOS_UINT ulLinkRetrain;
    VOS_UINT ulInitErrors;
    VOS_UINT ulInitTimeouts;
    VOS_UINT ulLossOfFraming;
    VOS_UINT ulFECErrors;                   /* 自最近DSL显示时间起检测到的FEC错误数 */
    VOS_UINT ulATUCFECErrors;           /* 自最近DSL显示时间起由ATU－C检测到的FEC错误数 */
    VOS_UINT ulHECErrors;                  /* 自最近DSL显示时间起检测到的HEC错误数         */
    VOS_UINT ulATUCHECErrors;              /* 自最近DSL显示时间起由ATU－C检测到的HEC错误数 */
    VOS_UINT64 udlCRCErrors;                   /* 自最近DSL显示时间起检测到的CRC错误数 */
    VOS_UINT ulATUCCRCErrors;               /* 自最近DSL显示时间起由ATU－C检测到的CRC错误数 */
    VOS_UINT ulReceiveErroredSecs;         /* 自最近DSL显示时间起下行发生的错误秒数        */
    VOS_UINT ulTransmitErroredSecs;        /* 自最近DSL显示时间起上行发生的错误秒数        */
    VOS_UINT ulReceiveSeverelyErroredSecs; /* 自最近DSL显示时间起下行发生的严重错误秒数    */
    VOS_UINT ulTransmitSeverelyErroredSecs;/* 自最近DSL显示时间起上行发生的严重错误秒数    */
    
    /*以下为冗余字段,DT不要求*/
    /* 如下参数保留为将来用 */    
    VOS_UINT  ulReceiveCntCellTotal;       /* 自最近DSL显示时间起所接收的cell数目        */
    VOS_UINT  ulTransmitCntCellTotal;      /* 自最近DSL显示时间起所发送的cell数目        */
    VOS_UINT ulReceiveCntBitErrors;        /* 接收位发生错误数目 */
    VOS_UINT ulTransmitCntBitErrors;       /* 发送位发生错误数目 */

    // new spec for hg612
    VOS_UINT ulHECErrorsOfPath1;            /* 自最近DSL显示时间起检测到的HEC错误数 , PATH1 */
    VOS_UINT ulATUCHECErrorsOfPath1;        /* 自最近DSL显示时间起由ATU－C检测到的HEC错误数, PATH1 */
    VOS_UINT ulCRCErrorsOfPath1;            /* 自最近DSL显示时间起检测到的path1 CRC错误数 */
    VOS_UINT ulATUCCRCErrorsOfPath1;        /* 自最近DSL显示时间起由ATU－C检测到的path1 CRC错误数 */
    VOS_UINT ulFECErrorsOfPath1;            /* 自最近DSL显示时间起检测到的FEC错误数 PATH1 */
    VOS_UINT ulATUCFecErrorsOfPath1;        /* 自最近DSL显示时间起由ATU－C检测到的FEC错误数 PATH1 */
}XDSL_STATS_SHOWTIME_ST;

extern VOS_VOID OamLostLinkTimerReset(VOS_VOID);
extern VOS_INT32   OAMSigProcSetup(VOS_VOID);
extern VOS_VOID OamPduTimerProc(VOS_VOID *pvHook);
extern VOS_INT32 OamGetIntfaceIndx(VOS_INT32 fd, const VOS_CHAR* pszIntfName);
extern VOS_INT32 OamGetXdslShowtimeStatistic(XDSL_STATS_SHOWTIME_ST *pstStatsTotalInfo);
extern VOS_INT32 OamSendPduProc(const VOS_CHAR *pstSrcPtr, const VOS_UINT32 ulLen,const VOS_INT32 lCurSock);
#endif

