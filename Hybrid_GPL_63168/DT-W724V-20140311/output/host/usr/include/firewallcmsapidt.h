/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : firewallapi.h
  作    者  : c00126165
  版    本  : ATPV100R002C03
  创建日期  : 2008-7-1
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2008-7-1
     作    者   : c00126165
     修改内容   : 完成初稿

*********************************************************************************/


#ifndef __FIREWALL_API_H__
#define __FIREWALL_API_H__

#include "atputil.h"


#ifdef SUPPORT_ATP_VOICE
#if 0 
#define ATP_VOICE_RTP_RANGE (20)
#endif
#endif

#ifdef ATP_DEBUG

#define ATP_CMD(x)  {printf("DEBUG:[%s:%d] <%s> \r\n", __FILE__, __LINE__, x); ATP_Exec(x);}

#else

#define ATP_CMD(x)  ATP_Exec(x)

#endif

#define ATP_HYBRID_LTE_INTF_NAME  "rmnet0"
#define ATP_HYBRID_GRE_INTF_NAME  "gre+"


typedef enum tagFW_COMMOND_TYPE
{
    FW_CMD_TYPE_ADD = 0,       //ATP_FW_CMD_TYPE_ADD
    FW_CMD_TYPE_INSERT = 1,    //ATP_FW_CMD_TYPE_INSERT
    FW_CMD_TYPE_REMOVE = 2     //ATP_FW_CMD_TYPE_REMOVE
}FW_COMMAND_TYPE_EN;         //ATP_FW_COMMAND_TYPE_EN


typedef enum tagAtpFwCommandType
{
    ATP_FW_CMD_TYPE_ADD = 0,       //FW_CMD_TYPE_ADD
    ATP_FW_CMD_TYPE_INSERT = 1,    //FW_CMD_TYPE_INSERT
    ATP_FW_CMD_TYPE_REMOVE = 2     //FW_CMD_TYPE_REMOVE
}ATP_FW_COMMAND_TYPE_EN;         //FW_COMMAND_TYPE_EN


#ifdef SUPPORT_ATP_VOICE
#define FW_BRIDGE_NAME "br0"

#define ATP_VOICE_RTP_RANGE (20)

/* SIP 传输协议类型 */
typedef enum tagFW_VOICE_PROTO_EN
{
    FW_VOICE_PROTO_NULL = 0,
    FW_VOICE_PROTO_BOTH = 1,
    FW_VOICE_PROTO_TCP = 6,
    FW_VOICE_PROTO_UDP = 17,
} FW_VOICE_PROTO_EN;

/* voice firewall输入条件 */
typedef struct tagFireWall_VOICE_ST
{
    VOS_UINT8   ucProto;                                    /* 协议类型 6或17即tcp或udp 见ATP_VOICE_PROTO_EN */
    VOS_CHAR    acInterfaceName[ATP_SYS_SMALL_LEN];         /* 如ppp0 */
    VOS_INT32   lDstPortMin;                                /* 起始目的端口 */
    VOS_INT32   lDstPortMax;                                /* 结束目的端口 */
}FIREWALL_VOICE_ST;
#endif


/******************************************************************************
  函数名称  : ATP_FIREWALL_WanStart
  功能描述  : 
              对WAN UP时，进行处理函数
  输入参数  : 
              1.  pszWanPath:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-7-1
     作    者   : c00126165
     修改内容   : 完成初稿

******************************************************************************/
//VOS_UINT32 ATP_FIREWALL_WanStart (VOS_CHAR* pszWanPath);
VOS_UINT32 ATP_FIREWALL_WanStart(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
/******************************************************************************
  函数名称  : ATP_FIREWALL_WanStop
  功能描述  : 
              对WAN Down时，进行处理函数
  输入参数  : 
              1.  pszWanPath:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-7-1
     作    者   : c00126165
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_WanStop(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/******************************************************************************
  函数名称  : ATP_FIREWALL_Init
  功能描述  :   
              防火墙初始化函数
  输入参数  : 
              1. VOS_VOID:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-7-1
     作    者   : c00126165
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_Init(VOS_VOID);
#ifdef SUPPORT_ATP_VOICE
/******************************************************************************
  函数名称  : ATP_FIREWALL_ServiceVoice
  功能描述  : 
              防火墙服务对语音模块的接口函数
  输入参数  : 
              1.  lLocalPortMin:voice本地端口起始端口
              2.  lLocalPortMax:voice本地端口结束端口
              3.  ulVoiceCmd:   ATP_FW_CMD_TYPE_ADD  打开相应接口
                                ATP_FW_CMD_TYPE_REMOVE  关闭相应接口
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-7-11
     作    者   : c00126165
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_ServiceVoice(VOS_INT32 lLocalPortMin, VOS_INT32 lLocalPortMax, VOS_UINT32 ulVoiceCmd);
#endif

/******************************************************************************
  函数名称  : ATP_FIREWALL_ServiceSipAlg
  功能描述  : 
              防火墙对SIP ALG的接口函数
  输入参数  : 
              1.  pszWanRemote:
              2.  ulPort:
              3.  ulRtpPortLow:
              4.  ulRtpPortHigh:
              5.  ulSipAlgCmd:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-7-11
     作    者   : c00126165
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_ServiceSipAlg(VOS_CHAR* pszWanRemote, VOS_UINT32 ulPort, 
                                      VOS_UINT32 ulRtpPortLow, VOS_UINT32 ulRtpPortHigh,
                                      VOS_UINT32 ulSipAlgCmd);


//VOS_UINT32 ATP_FIREWALL_LanChange(VOS_CHAR* pszNewLanIp, VOS_CHAR* pszNewSubnetMask);
VOS_UINT32 ATP_FIREWALL_LanChange(VOS_VOID *pvNotifier, VOS_VOID *pvHook);

VOS_VOID ATP_FIREWALL_AddNasFactoryRule(VOS_VOID);

VOS_VOID ATP_FIREWALL_DelNasFactoryRule(VOS_VOID);

#ifdef SUPPORT_ATP_IPV6
//VOS_UINT32 ATP_FIREWALL6_WanStart (VOS_CHAR* pszWanPath);
//VOS_UINT32 ATP_FIREWALL6_WanStop(VOS_CHAR* pszWanPath);
VOS_UINT32 ATP_FIREWALL6_WanStart(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
VOS_UINT32 ATP_FIREWALL6_WanStop(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
#endif

VOS_VOID ATP_FIREWALL_DelLanMacFilter(VOS_UINT32 ulPortIndex,VOS_BOOL bMacEnable,const VOS_CHAR *pszOldMacList);
VOS_VOID ATP_FIREWALL_AddAllLanMacFilter(VOS_UINT32 ulPortIndex,VOS_BOOL bMacEnable,const VOS_CHAR *pszNewMacList);




#endif

