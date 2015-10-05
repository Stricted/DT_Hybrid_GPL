/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : dhcpcoptions.h
  ��    ��  : c60023298
  ��    ��  : V100R002
  ��������  : 2007-10-18
  ��    ��  : 
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-10-18
     ��    ��   : c60023298
     �޸�����   : ��ɳ���

*********************************************************************************/


#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "atptypes.h"


#define ATP_OPTION_PADDING                         0x00
#define ATP_OPTION_SUBNET                          0x01
#define ATP_OPTION_TIME_OFFSET                     0x02
#define ATP_OPTION_ROUTER                          0x03
#define ATP_OPTION_TIME_SERVER                     0x04
#define ATP_OPTION_NAME_SERVER                     0x05
#define ATP_OPTION_DNS_SERVER                      0x06
#define ATP_OPTION_LOG_SERVER                      0x07
#define ATP_OPTION_COOKIE_SERVER                   0x08
#define ATP_OPTION_LPR_SERVER                      0x09
#define ATP_OPTION_HOST_NAME                       0x0c
#define ATP_OPTION_BOOT_SIZE                       0x0d
#define ATP_OPTION_DOMAIN_NAME                     0x0f
#define ATP_OPTION_SWAP_SERVER                     0x10
#define ATP_OPTION_ROOT_PATH                       0x11
#define ATP_OPTION_IP_TTL                          0x17
#define ATP_OPTION_MTU                             0x1a
#define ATP_OPTION_BROADCAST                       0x1c
#define ATP_OPTION_NTP_SERVER                      0x2a
#define ATP_OPTION_TR069_ACS                       0x2b 
#define ATP_OPTION_WINS_SERVER                     0x2c
#define ATP_OPTION_REQUESTED_IP                    0x32
#define ATP_OPTION_LEASE_TIME                      0x33
#define ATP_OPTION_OVER                            0x34
#define ATP_OPTION_MESSAGE_TYPE                    0x35
#define ATP_OPTION_SERVER_ID                       0x36
#define ATP_OPTION_PARAM_REQ                       0x37
#define ATP_OPTION_MESSAGE                         0x38
#define ATP_OPTION_MAX_SIZE                        0x39
#define ATP_OPTION_T1                              0x3a
#define ATP_OPTION_T2                              0x3b
#define ATP_OPTION_VENDOR                          0x3c
#define ATP_OPTION_CLIENT_ID                       0x3d
#define ATP_OPTION_TFTP_SERVER_NAME                0x42
#define ATP_OPTION_BOOTFILE_NAME                   0x43
#define ATP_OPTION_SMTP_SERVER                     0x45
#define ATP_OPTION_POP3_SERVER                     0x46
#define ATP_OPTION_UCLASS                          0x4d
#define ATP_OPTION_116                             0x74
#define ATP_OPTION_SIP_SERVER                      0x78
#define ATP_OPTION_STATIC_ROUTER                   0x79
#define ATP_OPTION_PRODUCTCLASS                    0x7d 
#ifdef SUPPORT_ATP_6RD_TUNNEL
#define ATP_OPTION_6RD				               0xd4		//zb: added by zKF40309, 2011.4.11
#endif
#define ATP_OPTION_SIP_ID                          0xfa
#define ATP_OPTION_END                             0xFF
/* end of DHCP option codes (partial list) */


#define ATP_OPTION_FIELD                           0
#define ATP_OPTION_FILE_FIELD                      1
#define ATP_OPTION_SNAME_FIELD                     2

#define ATP_OPTION_CODE                            0
#define ATP_OPTION_LEN                             1
#define ATP_OPTION_DATA                            2

#define ATP_OPTION_REQ	                           0x10 /* have the client request this option */

#define ATP_OPTION_TYPE_MASK	                   0x0F

/*������ĵ��Կ��غ�*/
#define DebugSwitch                                0

#if DebugSwitch
#define OptionDebug(str, args...) if (DebugSwitch)do { printf(str, ## args); \
				printf("\n"); } while(0)
#else
#define OptionDebug(str, args...)
#endif
enum 
{
	ATP_OPTION_IP=1,
	ATP_OPTION_IP_PAIR,
	ATP_OPTION_STRING,
	ATP_OPTION_BOOLEAN,
	ATP_OPTION_U8,
	ATP_OPTION_U16,
	ATP_OPTION_S16,
	ATP_OPTION_U32,
	ATP_OPTION_S32,
	ATP_OPTION_IP_STRING
};

#define ATP_OPTION_LIST	                          0x70
#define DHCP_OPTION_NAME_LEN                      14

typedef struct DhcpOption 
{
	//char          acName[DHCP_OPTION_NAME_LEN]; /*OPTION������*/
	const char    *acName;
	unsigned char cFlags;                       /*OPTION���������ͱ�־*/
	unsigned char cCode;                        /*OPTION�Ĵ��� uc����,����c��*/
}ATP_DHCP_OPTION_ST;

/***********************************************************
  �������ƣ�g_stOptions

  �������á����壺�����֧�ֵ�option��

  ����ȡֵ��Χ��

  ���ñ������ĺ����嵥��
                      
  ����˵����
          ���������֧�ֵ�����option����: 
    (����Ը��������ڴ˽ṹ�������µ�option.)
      Name                      Code
    {"subnet"     <------>   ATP_OPTION_SUBNET},
	{"timezone"   <------>   ATP_OPTION_TIME_OFFSET},
	{"router"     <------>   ATP_OPTION_ROUTER},
	{"timesvr"    <------>   ATP_OPTION_TIME_SERVER},
	{"namesvr"    <------>   ATP_OPTION_NAME_SERVER},
	{"dns"        <------>   ATP_OPTION_DNS_SERVER},
	{"logsvr"     <------>   ATP_OPTION_LOG_SERVER},
	{"cookiesvr"  <------>   ATP_OPTION_COOKIE_SERVER},
	{"lprsvr"     <------>   ATP_OPTION_LPR_SERVER},
	{"hostname"   <------>   ATP_OPTION_HOST_NAME},
	{"bootsize"   <------>	 ATP_OPTION_BOOT_SIZE},
	{"domain"     <------>	 ATP_OPTION_DOMAIN_NAME},
	{"swapsvr"    <------>	 ATP_OPTION_SWAP_SERVER},
	{"rootpath"   <------>	 ATP_OPTION_ROOT_PATH},
	{"ipttl"      <------>	 ATP_OPTION_IP_TTL},
	{"mtu"        <------>	 ATP_OPTION_MTU},
	{"broadcast"  <------>	 ATP_OPTION_BROADCAST},
	{"ntpsvr"     <------>	 ATP_OPTION_NTP_SERVER},
	{"tr069acs"   <------>   ATP_OPTION_TR069_ACS},
	{"wins"       <------>	 ATP_OPTION_WINS_SERVER},
	{"requestip"  <------>	 ATP_OPTION_REQUESTED_IP},
	{"lease"      <------>	 ATP_OPTION_LEASE_TIME},
	{"dhcptype"   <------>	 ATP_OPTION_MESSAGE_TYPE},
	{"serverid"   <------>	 ATP_OPTION_SERVER_ID},
	{"tftp"       <------>	 ATP_OPTION_TFTP_SERVER_NAME},
	{"bootfile"   <------>	 ATP_OPTION_BOOTFILE_NAME},
	{"smtpsrv",	  <------>   ATP_OPTION_SMTP_SERVER},
    {"pop3srv",	  <------>   ATP_OPTION_POP3_SERVER},
    {"sipsrv",	  <------>   ATP_OPTION_SIP_SERVER},
         
 ***********************************************************/
extern const ATP_DHCP_OPTION_ST g_astOptions[];

/***********************************************************
  �������ƣ�g_lOptionLengths

  �������á����壺ÿһ��option�����ݳ���

  ����ȡֵ��Χ��

  ���ñ������ĺ����嵥��
                                           

  ʹ��ע�����
 ***********************************************************/
extern const int g_alOptionLengths[];

/******************************************************************************
  �������ƣ�ATP_OPTION_Get
  ��������������lCode����ȡDHCP��pstPacket�ж�Ӧ��Optionֵ
  ���ú�����
  ����������
  ���������
            lCode :Option�Ĵ���
            pPacket :DHCP��
  ���������
  �� �� ֵ��
            lCode ��Ӧ�����ݣ�
            NULL ��ʾû�д�ѡ��lCode������
  ����˵����
            ʹ������
            int lCode = 60;
            int lCodeLen;
            ATP_DHCP_PACKET_ST packet;
            unsigned char *veriod;
            veriod = ATP_OPTION_Get(&packet,lCode);

            //��ʾ�������ݳ���
            lCodeLen = *(veriod-1) ;

            ...
******************************************************************************/
unsigned char *ATP_OPTION_Get(void *pPacket, unsigned char ucCode);

/******************************************************************************
  �������ƣ�ATP_OPTION_AddString
  ������������DHCP��pstPacket��Option���һѡ��(��������Ϊ�ַ���)
  ���ú�����
  ����������
  ���������
            pucOptionStr :
                          DHCP����Option��ָ��
            pucStringData :
                          Ԥ��ӵ�optionѡ������ݡ�
                          ��ʽΪ: | code | length | data|
  ���������
  �� �� ֵ��
            pucStringData �ĳ��ȡ�
  ����˵����ʹ������
            int lCode = 56;
            ATP_DHCP_PACKET_ST packet;
            int length = 0;
            unsigned char *veriod = "test option";
            unsigned char *pucStringData = NULL;
            
            length = strlen(veriod);
            pucStringData = malloc(length + 2);
            if(NULL == pucStringData)
            {
                ...
            }
            pucStringData[ATP_OPTION_CODE] = lCode;
            pucStringData[ATP_OPTION_LEN]; = length;
            strncpy(pucStringData + 2, veriod, length);

            ATP_OPTION_AddString(packet.options,pucStringData);
            ...
******************************************************************************/
int ATP_OPTION_AddString(unsigned char *pucOptionStr, unsigned char *pucStringData);

/******************************************************************************
  �������ƣ�ATP_OPTION_AddInteger
  ������������DHCP��pstPacket��Option���һѡ��(��������Ϊ����)
  ���ú�����
  ����������
  ���������
            pucOptionStr :
                          DHCP����Option��ָ��
            ucCode :
                          option��CODE.
            ulData:
                          ucCode��Ӧ����������
  ���������
  �� �� ֵ��
            ulData �����ݳ���
  ����˵����ʹ������
            int lCode = 8;
            ATP_DHCP_PACKET_ST packet;
            int data = 80;

            ATP_OPTION_AddInteger(packet.options,lCode,data);
            ...
******************************************************************************/
int ATP_OPTION_AddInteger(unsigned char *pucOptionStr, unsigned char ucCode, u_int32_t ulData);

/******************************************************************************
  �������ƣ�ATP_OPTION_Parse
  ����������DHCPC�е��ô˺���������ACK��������option.
  ���ú�����
  ����������
  ���������
  ���������
  �� �� ֵ��
            
  ����˵����ʹ������
            ATP_DHCP_PACKET_ST packet;
            ...
            ATP_OPTION_Parse(packet,fill_options);
            ...
static void fill_options(unsigned char *option, ATP_DHCP_OPTION_ST *pstType)
{
      code = pstType->cCode;
      type = pstType->cFlags & ATP_OPTION_TYPE_MASK;
      optlen = g_alOptionLengths[type];
      for(;;) 
      {
        switch (code) 
        {
         case ATP_OPTION_ROUTER:
              
              break;
        }
        option += optlen;
        len -= optlen;
        if (len <= 0) break;
        if(ATP_OPTION_STRING == type)break;
     }
}
            ...
******************************************************************************/
void ATP_OPTION_Parse(void *pPacket,void (*pFunc)(unsigned char *pszOption, const ATP_DHCP_OPTION_ST *pstType));


#endif
