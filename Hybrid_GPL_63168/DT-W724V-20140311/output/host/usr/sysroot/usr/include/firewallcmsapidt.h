/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : firewallapi.h
  ��    ��  : c00126165
  ��    ��  : ATPV100R002C03
  ��������  : 2008-7-1
  ��    ��  : 
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2008-7-1
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

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

/* SIP ����Э������ */
typedef enum tagFW_VOICE_PROTO_EN
{
    FW_VOICE_PROTO_NULL = 0,
    FW_VOICE_PROTO_BOTH = 1,
    FW_VOICE_PROTO_TCP = 6,
    FW_VOICE_PROTO_UDP = 17,
} FW_VOICE_PROTO_EN;

/* voice firewall�������� */
typedef struct tagFireWall_VOICE_ST
{
    VOS_UINT8   ucProto;                                    /* Э������ 6��17��tcp��udp ��ATP_VOICE_PROTO_EN */
    VOS_CHAR    acInterfaceName[ATP_SYS_SMALL_LEN];         /* ��ppp0 */
    VOS_INT32   lDstPortMin;                                /* ��ʼĿ�Ķ˿� */
    VOS_INT32   lDstPortMax;                                /* ����Ŀ�Ķ˿� */
}FIREWALL_VOICE_ST;
#endif


/******************************************************************************
  ��������  : ATP_FIREWALL_WanStart
  ��������  : 
              ��WAN UPʱ�����д�����
  �������  : 
              1.  pszWanPath:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-7-1
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

******************************************************************************/
//VOS_UINT32 ATP_FIREWALL_WanStart (VOS_CHAR* pszWanPath);
VOS_UINT32 ATP_FIREWALL_WanStart(VOS_VOID* pszWanPath, VOS_VOID *pvHook);
/******************************************************************************
  ��������  : ATP_FIREWALL_WanStop
  ��������  : 
              ��WAN Downʱ�����д�����
  �������  : 
              1.  pszWanPath:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-7-1
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_WanStop(VOS_VOID* pszWanPath, VOS_VOID *pvHook);

/******************************************************************************
  ��������  : ATP_FIREWALL_Init
  ��������  :   
              ����ǽ��ʼ������
  �������  : 
              1. VOS_VOID:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-7-1
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_Init(VOS_VOID);
#ifdef SUPPORT_ATP_VOICE
/******************************************************************************
  ��������  : ATP_FIREWALL_ServiceVoice
  ��������  : 
              ����ǽ���������ģ��Ľӿں���
  �������  : 
              1.  lLocalPortMin:voice���ض˿���ʼ�˿�
              2.  lLocalPortMax:voice���ض˿ڽ����˿�
              3.  ulVoiceCmd:   ATP_FW_CMD_TYPE_ADD  ����Ӧ�ӿ�
                                ATP_FW_CMD_TYPE_REMOVE  �ر���Ӧ�ӿ�
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-7-11
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_FIREWALL_ServiceVoice(VOS_INT32 lLocalPortMin, VOS_INT32 lLocalPortMax, VOS_UINT32 ulVoiceCmd);
#endif

/******************************************************************************
  ��������  : ATP_FIREWALL_ServiceSipAlg
  ��������  : 
              ����ǽ��SIP ALG�Ľӿں���
  �������  : 
              1.  pszWanRemote:
              2.  ulPort:
              3.  ulRtpPortLow:
              4.  ulRtpPortHigh:
              5.  ulSipAlgCmd:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-7-11
     ��    ��   : c00126165
     �޸�����   : ��ɳ���

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

