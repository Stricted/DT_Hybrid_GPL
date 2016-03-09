
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
/*start of 新增支持rfc3203中，DHCP FORCERENEW 包，
   重新分配IP地址。2008-3-31 by c00126165*/ 
#define ATP_DHCP_FORCERENEW        9
/*start of 新增支持rfc3203中，DHCP FORCERENEW 包，
   重新分配IP地址。2008-3-31 by c00126165*/ 

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
    u_int32_t ulSrcIp;                                 /*源IP*/
    int lSrcPort;                                      /*源端口*/
    u_int32_t ulDstIp;                                 /*目的IP*/
    int lDstPort;                                      /*目的端口*/
    char acDstArp[ATP_DHCP_DEST_ARP_LEN];              /*目的MAC地址，只有6字节。只有RAW的DHCP包才用此选项*/
    int lIfIndex;                                      /*接口的Index号，只有RAW的DHCP包才用此选项*/
}ATP_DHCP_FIVE_TEAM_ST;

enum
{
    ATP_DHCP_RAW_PACKET=0,    /*RAW的DHCP包*/
    ATP_DHCP_KRNL_PACKET,     /*kernel的DHCP包*/
    ATP_DHCP_OTHER_PACKET
};

/******************************************************************************
  函数名称：ATP_DHCP_GetPacket
  功能描述：根据lSockFd,选择是接受DHCP包的类型，来获取DHCP包
  调用函数：
  被调函数：
  输入参数：
            lSockFd :Socket的描述符
            ulType : 获取包的类型 
  输出参数：
            DHCP数据包
  返 回 值：
            ATP_DHCP_GET_PACKET_ERR:失败
            其它:成功
  其它说明：
            使用如下
            int lSockFd;
            ATP_DHCP_PACKET_ST packet;
            ...
            select(lSockFd+1,NULL,NULL,NULL,NULL);
            
            ATP_DHCP_GetPacket(&packet,lSockFd,ATP_DHCP_RAW_PACKET);
            或者
            ATP_DHCP_GetPacket(&packet,lSockFd,ATP_DHCP_KRNL_PACKET);
            ...
******************************************************************************/
int ATP_DHCP_GetPacket(ATP_DHCP_PACKET_ST *pstPacket,int lSockFd,u_int32_t ulType);

/******************************************************************************
  函数名称：ATP_DHCP_SendPacket
  功能描述：发送DHCP包
  调用函数：
  被调函数：
  输入参数：
            pstPacket :DHCP包的信息
            pstPacket :五元组，ATP_DHCP_FIVE_TEAM_ST。
            ulType:发送DHCP包的套节字方式。
  输出参数：
  返 回 值：
            ATP_DHCP_SEND_PACKET_ERR:失败
            其它:成功
  其它说明：
            使用如下
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
  函数名称  : ATP_DHCP_InitPacketHeader
  功能描述  : 建立Packet头
  输入参数  : 
              1.  *pstPacket:
              2.  cType:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-11-15
     作    者   : c60023298
     修改内容   : 完成初稿

******************************************************************************/
void ATP_DHCP_InitPacketHeader(ATP_DHCP_PACKET_ST *pstPacket, unsigned char cType);

/******************************************************************************
  函数名称  : ATP_DHCP_SetQosMark
  功能描述  : 给DHCP 发送报文打mark的API
  输入参数  : 
              1.  *pstPacket:
              2.  cType:
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2009-1-23
     作    者   : g00121640
     修改内容   : 完成初稿

******************************************************************************/
//void ATP_DHCP_SetQosMark(u_int32_t ulQosMark);
extern unsigned long g_ulNfMarkValue;

void ATP_DHCP_InitPacketHeader(ATP_DHCP_PACKET_ST *pstPacket, unsigned char cType);


#endif

