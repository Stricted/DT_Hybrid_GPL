/******************************************************************************
  版权所有  : 2011-2020，华为技术有限公司
  文 件 名  : oamapi.h
  作    者  : l00135113
  版    本  : ATPV100R003C06
  创建日期  : 2011-8-15
  描    述  : 802.3ah(OAM)头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2011-8-15
     作    者   : l00135113
     修改内容   : 完成初稿

*********************************************************************************/
#ifndef __ATP_OAM_API_H__
#define __ATP_OAM_API_H__


#include "atputil.h"
#include "httpapi.h"
#include "msg/oammsgtypes.h"
//#define ATP_DEBUG 1

#if 0//def ATP_DEBUG
#define ATP_OAM_DEBUG(format, args...)       {printf("\nFILE:%s,LINE:%d,FUNC:%s: ", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("\n");}
#define ATP_OAM_ERR_DEBUG(format, args...)       {printf("\nFILE:%s,LINE:%d,FUNC:%s: ", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("\n");}
#else
#define ATP_OAM_DEBUG(x...)
#define ATP_OAM_ERR_DEBUG(format, args...)       
#endif

#define OAM_LOOPBACK_ENABLE	0x01
#define OAM_LOOPBACK_DISABLE 0x02
#define OAM_LOOPBACK_STATE 0x05
#define OAM_NORMAL_STATE 0x00

#define OAM_PDU_PROTO   ETH_P_SLOW
#define OAM_PDU_SUB_PROTO   0x03

/*local information tlv:Vendor Specific Information*/
#define OAM_VENDOR_INFO     "HWTC"

#define OAM_PRO_MAC_ADDR            "\x01\x80\xc2\x00\x00\x02"
//#define OAM_XP_MAC_ADDR            "\x00\xe0\x4c\x88\x7b\x91"

/*最大监听端口个数*/
#define ATP_MAX_LISTEN_SOCKET		(4)

/*默认接收缓冲区索引*/
#define OAM_DEFAULT_RECV_INDX		(0)

#define OAM_INTFACENAME_LEN         32

#define OAM_MAC_ADDR_LEN         6
#define OAM_INFO_TLV_OUI_LEN     3
#define OAM_INFO_TLV_VENDOR_LEN  4

#define OAM_FOUR_OCTET_LEN  4
#define OAM_EIGHT_OCTET_LEN  8

//#define OAM_SYS_CMD_LEN                 512

/*OAM最大报文缓冲区长度*/
#define OAM_MAX_BUF_LEN  1600

/*OAM报文最大长度*/
#define OAMPDU_MAX_LEN   1514 /*18+1496*/

/*OAM报文最小长度*/
#define OAMPDU_MIN_LEN   60  /*18+42*/


/*链路监控时间间隔，单位:秒*/
#define OAM_INTERVAL_TIME  60

/*发送同一链路监控事件的最多尝试次数*/
#define OAM_EVENT_SEND_MAX_TRY  5

/**/

/**/
extern VOS_CHAR g_acOamPduRecvBuff[];

/******************结构体***********************************************/

/*union*/


/* OAM 连接状态枚举*/
typedef enum tagATP_OAM_MODE_EN
{
    ATP_OAM_MODE_PASSSIVE = 0,  /*passive*/ 
    ATP_OAM_MODE_ACTIVE,        /*active*/
    ATP_OAM_MODE_END
}ATP_OAM_MODE_EN;

/*OAMPDU类型枚举*/
typedef enum tagOAM_PDU_CODE_EN
{
    OAM_PDU_CODE_INFO = 0x0,  /*information*/ 
    OAM_PDU_CODE_EVENT = 0x1,  /*Event Notification*/ 
    OAM_PDU_CODE_LOOPBACK = 0x4, /*LOOPBACK CONTROL*/
    
    OAM_PDU_CODE_END 
}ATP_OAM_PDU_CODE_EN;


/*Struct*/

/*OAMPDU
------------------------------------------------------------
| 6       | 6         |2   | 1     | 2   | 1  |42~1496 | 4 |   
------------------------------------------------------------
|Dest addr|Source addr|Type|SubType|Flags|Code|Data/Pad|CRC|
------------------------------------------------------------
*/
#pragma pack(1)
typedef struct OAM_PDU_ST{
    VOS_UINT8  ucTHaddr[OAM_MAC_ADDR_LEN];     /* 慢协议组播地址 0x0180-c200-0002*/
    VOS_UINT8  ucSHaddr[OAM_MAC_ADDR_LEN];     /* 源MAC地址*/
    VOS_UINT16 usType;                         /* 协议类型0x8809*/
    VOS_UINT8  ucSubType;                      /* 协议子类型0x03*/
    VOS_UINT16 usFlags;                        /* flag域*/
    VOS_UINT8  ucCode;                         /* 报文类型 0x00:information OAMPDU
                                                        0x01:Event notification OAMPDU              
                                                        0x04:Loopback control OAMPDU*/      
}ATP_OAM_PDU_ST;
#pragma pack()

/*Information TLV*/
#pragma pack(1)
typedef struct OAM_INFO_TLV_ST{
    VOS_UINT8  ucType;                      /* Type:0x00/0x01/0x02/0xfe/0xff*/
    VOS_UINT8  ucLength;                      /* Length:0x10*/    
    VOS_UINT8  ucOamVersion;                      /*0x01*/    
    VOS_UINT16 usRevision;
    VOS_UINT8  ucState;
    VOS_UINT8  ucOamConfig;
    VOS_UINT16 usOamPduConfig;
    VOS_UINT8  ucOui[OAM_INFO_TLV_OUI_LEN];
    VOS_UINT8  ucVendor[OAM_INFO_TLV_VENDOR_LEN];
 }ATP_OAM_INFO_TLV_ST;
#pragma pack()



//函数声明//
extern VOS_INT32 ATP_OAM_ServerInfoInit(VOS_VOID);
extern VOS_INT32 ATP_OAM_ListenSock(const VOS_CHAR *acInterfaceName,const VOS_INT32 lCurSock);
extern VOS_VOID OamMsgProc(const ATP_MSG_HEADER_ST *pstMsg);
extern VOS_INT32   OamResInit(VOS_VOID);
extern VOS_INT32  OamResUnInit(VOS_VOID);
extern VOS_INT32 ATP_OAM_RecvEventOamPduProc(const VOS_INT32 lSock,const ATP_OAM_PDU_ST *pstOampdu,const VOS_INT32 ulDataLen);
extern VOS_VOID ATP_OAM_SetLoopbackControlFlag(VOS_BOOL bEnable);
extern VOS_BOOL ATP_OAM_GetLoopbackControlFlag();
extern VOS_INT32 ATP_OAM_RecvLoopbackControlProc(const VOS_INT32 lSock,const ATP_OAM_PDU_ST *pstOampdu,const VOS_UINT32 ulDataLen);
/*****************************************************************
内部实现的命令
atpoamsdk.c调用到的命令
******************************************************************/



#endif

