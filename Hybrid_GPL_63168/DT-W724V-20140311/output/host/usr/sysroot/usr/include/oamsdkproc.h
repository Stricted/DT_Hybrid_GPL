/******************************************************************************
  ��Ȩ����  : 2011-2020����Ϊ�������޹�˾
  �� �� ��  : oamsdkproc.h
  ��    ��  : l00135113
  ��    ��  : ATPV100R003C06
  ��������  : 2011-8-15
  ��    ��  : 802.3ah(OAM)ͷ�ļ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2011-8-25
     ��    ��   : l00135113
     �޸�����   : ��ɳ���

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
/* OAM Control״̬����ö��*/
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

/* OAMPDU Flags״̬����ö��*/
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

/*local_pdu״̬ö��*/
typedef enum tagOAM_CONTR_LOCALPDU_STATE_EN
{
    OAM_LOCALPDU_STATE_RX_INFO = 1,  /*RX_INFO*/ 
    OAM_LOCALPDU_STATE_INFO,  /*INFO*/     
    OAM_LOCALPDU_STATE_LF_INFO,  /*LF_INFO*/     
    OAM_LOCALPDU_STATE_ANY,  /*ANY*/    
    OAM_LOCALPDU_STATE_END
}ATP_OAM_CONTR_LOCALPDU_STATE_EN;

/*control oam discovery״̬ö��*/
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

/*parser״̬ö��*/
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
    VOS_INT32  lSock;                          /* ȫ��socket���*/ 
    VOS_UINT32 ulSendBufLen;                   /*���͵�OAMPDU�ı��ĳ���*/
    VOS_CHAR   *pszSendBuff;                    /*��Ҫ���͵�OAMPDU����ʼ��ַ*/
    struct sockaddr_ll addr;                   /* ���ص�ַ��Ϣ*/ 

    ATP_OAM_PDU_ST *pstLocalOamPduComm;      /*OAMPDU���Ĺ�����Ϣ(DA/SA/Type/Subtype)*/
    ATP_OAM_INFO_TLV_ST *pstLocalTlvInfo;    /*Local information tlv*/
    ATP_OAM_INFO_TLV_ST *pstLastRemoteTlvInfo; /*���һ���յ���correct information tlv*/
    
}ATP_OAM_INFO_ST;

/*��¼xDslͳ����Ϣ*/
typedef struct
{
    VOS_UINT ulReceiveBlocks;              /* ����� DSL Showtime ���ѳɹ����յĳ���֡��   */
    VOS_UINT ulTransmitBlocks;             /* �����DSL��ʾʱ�����ѷ��ͳɹ��ĳ���֡��      */
    VOS_UINT ulCellDelin;                  /* �����DSL��ʾʱ����������Ԫͷ���������    */
    VOS_UINT ulLinkRetrain;
    VOS_UINT ulInitErrors;
    VOS_UINT ulInitTimeouts;
    VOS_UINT ulLossOfFraming;
    VOS_UINT ulFECErrors;                   /* �����DSL��ʾʱ�����⵽��FEC������ */
    VOS_UINT ulATUCFECErrors;           /* �����DSL��ʾʱ������ATU��C��⵽��FEC������ */
    VOS_UINT ulHECErrors;                  /* �����DSL��ʾʱ�����⵽��HEC������         */
    VOS_UINT ulATUCHECErrors;              /* �����DSL��ʾʱ������ATU��C��⵽��HEC������ */
    VOS_UINT64 udlCRCErrors;                   /* �����DSL��ʾʱ�����⵽��CRC������ */
    VOS_UINT ulATUCCRCErrors;               /* �����DSL��ʾʱ������ATU��C��⵽��CRC������ */
    VOS_UINT ulReceiveErroredSecs;         /* �����DSL��ʾʱ�������з����Ĵ�������        */
    VOS_UINT ulTransmitErroredSecs;        /* �����DSL��ʾʱ�������з����Ĵ�������        */
    VOS_UINT ulReceiveSeverelyErroredSecs; /* �����DSL��ʾʱ�������з��������ش�������    */
    VOS_UINT ulTransmitSeverelyErroredSecs;/* �����DSL��ʾʱ�������з��������ش�������    */
    
    /*����Ϊ�����ֶ�,DT��Ҫ��*/
    /* ���²�������Ϊ������ */    
    VOS_UINT  ulReceiveCntCellTotal;       /* �����DSL��ʾʱ���������յ�cell��Ŀ        */
    VOS_UINT  ulTransmitCntCellTotal;      /* �����DSL��ʾʱ���������͵�cell��Ŀ        */
    VOS_UINT ulReceiveCntBitErrors;        /* ����λ����������Ŀ */
    VOS_UINT ulTransmitCntBitErrors;       /* ����λ����������Ŀ */

    // new spec for hg612
    VOS_UINT ulHECErrorsOfPath1;            /* �����DSL��ʾʱ�����⵽��HEC������ , PATH1 */
    VOS_UINT ulATUCHECErrorsOfPath1;        /* �����DSL��ʾʱ������ATU��C��⵽��HEC������, PATH1 */
    VOS_UINT ulCRCErrorsOfPath1;            /* �����DSL��ʾʱ�����⵽��path1 CRC������ */
    VOS_UINT ulATUCCRCErrorsOfPath1;        /* �����DSL��ʾʱ������ATU��C��⵽��path1 CRC������ */
    VOS_UINT ulFECErrorsOfPath1;            /* �����DSL��ʾʱ�����⵽��FEC������ PATH1 */
    VOS_UINT ulATUCFecErrorsOfPath1;        /* �����DSL��ʾʱ������ATU��C��⵽��FEC������ PATH1 */
}XDSL_STATS_SHOWTIME_ST;

extern VOS_VOID OamLostLinkTimerReset(VOS_VOID);
extern VOS_INT32   OAMSigProcSetup(VOS_VOID);
extern VOS_VOID OamPduTimerProc(VOS_VOID *pvHook);
extern VOS_INT32 OamGetIntfaceIndx(VOS_INT32 fd, const VOS_CHAR* pszIntfName);
extern VOS_INT32 OamGetXdslShowtimeStatistic(XDSL_STATS_SHOWTIME_ST *pstStatsTotalInfo);
extern VOS_INT32 OamSendPduProc(const VOS_CHAR *pstSrcPtr, const VOS_UINT32 ulLen,const VOS_INT32 lCurSock);
#endif

