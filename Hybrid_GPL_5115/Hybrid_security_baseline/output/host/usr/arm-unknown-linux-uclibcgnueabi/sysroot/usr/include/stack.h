
/**\file stack.h
  *
  * Generic declarations for DHCP protocol stack
  *
  *\copyright 2007-2020, HW Tech
  *\author ATP Team
  *\date  2012-2-3
*/

#ifndef _STACK_H
#define _STACK_H


/* DHCP protocol -- see RFC 2131 */
#define ATP_DHCP_SERVER_PORT            67
#define ATP_DHCP_CLIENT_PORT            68

#define ATP_DHCP_BOOT_REQUEST            1
#define ATP_DHCP_BOOT_REPLY              2

#define ATP_DHCP_SEND_PACKET_ERR       -1
#define ATP_DHCP_GET_PACKET_ERR        -1

#define ATP_DHCP_DISCOVER           1
#define ATP_DHCP_OFFER              2
#define ATP_DHCP_REQUEST            3
#define ATP_DHCP_DECLINE            4
#define ATP_DHCP_ACK                 5
#define ATP_DHCP_NAK                  6
#define ATP_DHCP_RELEASE            7
#define ATP_DHCP_INFORM            8
/*start of ����֧��rfc3203�У�DHCP FORCERENEW ����
   ���·���IP��ַ��2008-3-31 by c00126165*/ 
#define ATP_DHCP_FORCERENEW        9
/*start of ����֧��rfc3203�У�DHCP FORCERENEW ����
   ���·���IP��ַ��2008-3-31 by c00126165*/ 

#define ATP_BROADCAST_FLAG            0x8000

#define ATP_DHCP_DEST_ARP_LEN 8
#define ATP_DHCP_CHADDR_LEN 16
#define ATP_DHCP_SNAME_LEN 64
#define ATP_DHCP_FILE_LEN 128
#define ATP_DHCP_OPTION_LEN 1024

#ifdef SO_MARK
#define SO_NFMARK       SO_MARK
#else
#define SO_NFMARK       66
#endif /* SO_MARK */

#if ATP_DEBUG
#define StackDebug(format, args...)\
{\
    printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);\
    printf(format, ##args);\
}
#else
#define StackDebug(x...)
#endif

typedef struct tagDhcpPacket 
{
	u_int8_t   cOp;
	u_int8_t   cHtype;
	u_int8_t   cHlen;
	u_int8_t   cHops;
	u_int32_t  ulXid;
	u_int16_t  usSecs;
	u_int16_t  usFlags;
	u_int32_t  ulCiaddr;
	u_int32_t  ulYiaddr;
	u_int32_t  ulSiaddr;
	u_int32_t  ulGiaddr;
	u_int8_t   acChaddr[ATP_DHCP_CHADDR_LEN];
	u_int8_t   acSname[ATP_DHCP_SNAME_LEN];
	u_int8_t   acFile[ATP_DHCP_FILE_LEN];
	u_int32_t  ulCookie;
	u_int8_t   acOptions[ATP_DHCP_OPTION_LEN];
}ATP_DHCP_PACKET_ST;


typedef struct DhcpFiveTeam
{
    u_int32_t ulSrcIp;                                 /*ԴIP*/
    int lSrcPort;                                      /*Դ�˿�*/
    u_int32_t ulDstIp;                                 /*Ŀ��IP*/
    int lDstPort;                                      /*Ŀ�Ķ˿�*/
    char acDstArp[ATP_DHCP_DEST_ARP_LEN];              /*Ŀ��MAC��ַ��ֻ��6�ֽڡ�ֻ��RAW��DHCP�����ô�ѡ��*/
    int lIfIndex;                                      /*�ӿڵ�Index�ţ�ֻ��RAW��DHCP�����ô�ѡ��*/
}ATP_DHCP_FIVE_TEAM_ST;

enum
{
    ATP_DHCP_RAW_PACKET=0,    /*RAW��DHCP��*/
    ATP_DHCP_KRNL_PACKET,     /*kernel��DHCP��*/
    ATP_DHCP_OTHER_PACKET
};

/******************************************************************************
  �������ƣ�ATP_DHCP_GetPacket
  ��������������lSockFd,ѡ���ǽ���DHCP�������ͣ�����ȡDHCP��
  ���ú�����
  ����������
  ���������
            lSockFd :Socket��������
            ulType : ��ȡ�������� 
  ���������
            DHCP���ݰ�
  �� �� ֵ��
            ATP_DHCP_GET_PACKET_ERR:ʧ��
            ����:�ɹ�
  ����˵����
            ʹ������
            int lSockFd;
            ATP_DHCP_PACKET_ST packet;
            ...
            select(lSockFd+1,NULL,NULL,NULL,NULL);
            
            ATP_DHCP_GetPacket(&packet,lSockFd,ATP_DHCP_RAW_PACKET);
            ����
            ATP_DHCP_GetPacket(&packet,lSockFd,ATP_DHCP_KRNL_PACKET);
            ...
******************************************************************************/
int ATP_DHCP_GetPacket(ATP_DHCP_PACKET_ST *pstPacket,int lSockFd,u_int32_t ulType);

/******************************************************************************
  �������ƣ�ATP_DHCP_SendPacket
  ��������������DHCP��
  ���ú�����
  ����������
  ���������
            pstPacket :DHCP������Ϣ
            pstPacket :��Ԫ�飬ATP_DHCP_FIVE_TEAM_ST��
            ulType:����DHCP�����׽��ַ�ʽ��
  ���������
  �� �� ֵ��
            ATP_DHCP_SEND_PACKET_ERR:ʧ��
            ����:�ɹ�
  ����˵����
            ʹ������
            ATP_DHCP_PACKET_ST packet;
	        int ret = 0;
	        ATP_DHCP_FIVE_TEAM_ST stDhcpFiveTeam;

            ...
	        memset(&stDhcpFiveTeam,0,sizeof(ATP_DHCP_FIVE_TEAM_ST));
	        if (...) 
        	{
        	    stDhcpFiveTeam.ulSrcIp = ciaddr;
        	    stDhcpFiveTeam.lSrcPort = ATP_DHCP_CLIENT_PORT;
        	    stDhcpFiveTeam.ulDstIp = server;
        	    stDhcpFiveTeam.lDstPort = ATP_DHCP_SERVER_PORT;
        	    
        	    ret = ATP_DHCP_SendPacket(&packet,&stDhcpFiveTeam,ATP_DHCP_KRNL_PACKET);

        	}
	       else 
        	{
        	    stDhcpFiveTeam.ulSrcIp = INADDR_ANY;
        	    stDhcpFiveTeam.lSrcPort = ATP_DHCP_CLIENT_PORT;
        	    stDhcpFiveTeam.ulDstIp = INADDR_BROADCAST;
        	    stDhcpFiveTeam.lDstPort = ATP_DHCP_SERVER_PORT;
        	    memcpy(stDhcpFiveTeam.acDstArp, MAC_BCAST_ADDR, 6);
        	    stDhcpFiveTeam.lIfIndex = client_config.ifindex;
        	    
        	    ret = ATP_DHCP_SendPacket(&packet,&stDhcpFiveTeam,ATP_DHCP_RAW_PACKET);
            }
            ...
******************************************************************************/
int ATP_DHCP_SendPacket(ATP_DHCP_PACKET_ST *pstPacket, const ATP_DHCP_FIVE_TEAM_ST *pstFiveTeam,u_int32_t ulType);

/******************************************************************************
  ��������  : ATP_DHCP_InitPacketHeader
  ��������  : ����Packetͷ
  �������  : 
              1.  *pstPacket:
              2.  cType:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-11-15
     ��    ��   : c60023298
     �޸�����   : ��ɳ���

******************************************************************************/
void ATP_DHCP_InitPacketHeader(ATP_DHCP_PACKET_ST *pstPacket, unsigned char cType);

/******************************************************************************
  ��������  : ATP_DHCP_SetQosMark
  ��������  : ��DHCP ���ͱ��Ĵ�mark��API
  �������  : 
              1.  *pstPacket:
              2.  cType:
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2009-1-23
     ��    ��   : g00121640
     �޸�����   : ��ɳ���

******************************************************************************/
//void ATP_DHCP_SetQosMark(u_int32_t ulQosMark);
extern unsigned long g_ulNfMarkValue;

void ATP_DHCP_InitPacketHeader(ATP_DHCP_PACKET_ST *pstPacket, unsigned char cType);


#endif

