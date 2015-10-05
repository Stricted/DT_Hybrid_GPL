/**\file atputil.h
  * ATP���ƽ̨��������ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author z37589
  *\date  2007-8-23
*/

#ifndef __ATP_UTIL_H__
#define __ATP_UTIL_H__

/********************** Include Files ***************************************/

// Includes.
/*atp inc*/

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <linux/version.h>
#include "atptypes.h"
#include "msgapi.h"
#include "atpconfig.h"
#include "msgapi.h"
#include "caltime.h"

/********************** macro ***********************************************/
/*String Safe Length*/
#define ATP_STRING_LEN_512                  516
#define ATP_STRING_LEN_128                  132
#define ATP_STRING_LEN_256                  260   
#define ATP_STRING_LEN_64                   68    
#define ATP_STRING_LEN_32                   36  

/*String Actual Length*/
#define ATP_ACTUAL_LEN_512                  512 
#define ATP_ACTUAL_LEN_256                  256  
#define ATP_ACTUAL_LEN_128                  128  
#define ATP_ACTUAL_LEN_64                   64
#define ATP_ACTUAL_LEN_32                   32
#define ATP_ACTUAL_LEN_8                    8


/*Cli User Instande Id*/
#define ATP_CLI_ADMIN_INSTID                1
#define ATP_CLI_USER_INSTID                 2

/*Web User Instance Id*/
#define ATP_WEB_ADMIN_INSTID                1
#define ATP_WEB_USER_INSTID                 2

/*System Length*/
#define ATP_SYS_MEDIUM_LEN                  240
#define ATP_SYS_DOMAIN_LEN                  64
#define ATP_SYS_CMD_LEN                     512

/*Read Buf Length*/
#define ATP_SYS_READ_BUF_SIZE               128

#define ATP_STRING_LEN_268                  268
#define ATP_STRING_LEN_320                  320

/*Start of Protocol 2014-5-24 for UDK0000351459 Filter list TTL�޸�  by y00250667*/
#define ATP_ADDR_MASK4_MAX_LEN 32
#define ATP_ADDR_MASK6_MAX_LEN 128
/*End of Protocol 2014-5-24 for UDK0000351459 Filter list TTL�޸�  by y00250667*/


/* mic create msg server socket port for multi board communication */
#define ATP_MIC_SVR_IP_PORT 55555
/* mic create socket port for 5115 image transport */
#define ATP_MIC_TFTP_PORT 69

/*NFMARK*/
/* start of QoS by d00107688 2008-9-23 ��չ�� setsockopt ������������TR069�ȴ��ϱ����QoS */
/* ���ֵ��������չ�ں˵�setsockopt�ı�ŵģ����Ҫ�����ں��е� asm/socket.h �еı仯 */
/*SO_NFMARK��21�ں��õģ�30�ں�ֻ��SO_MARK*/
#ifdef SO_MARK
#define SO_NFMARK       SO_MARK
#else
#define SO_NFMARK       66
#endif /* SO_MARK */

/*Start of Protocol 2014-3-25 16:20 for ·�ɻ����޸� by t00189036*/
#ifdef SUPPORT_ATP_HYBRID_BONDING
#define NON_BLACKLIST_MARK 0x1000
#endif
/*End of Protocol 2014-3-25 16:20 for by t00189036*/

/* Ŀǰ������ʱȫ������ͬһ����ǣ������������Ҫϸ�ֳ�SIP,RTP�ȣ���ô�������޸ĸú�Ϳ����� */
#ifndef SUPPORT_ATP_DT_QOS
#define NFMARK_SIP      0xa
#define NFMARK_RTP      0xb
#define NFMARK_RTCP     0xc
/* TR069 */
#define NFMARK_TR069_CONTRACK   0xd
/* DNS */
#define NFMARK_DNS      0xB7
#define NFMARK_DNS_CONTRACK 0xBA

/* Start of modified by f00120964 for qos function 2012-4-2*/
#else
/*DT ���Ƶ�mark��־��tos��־*/
#define NFMARK_VOIP     0xA0
#define NFMARK_SIP      0xA1
#define NFMARK_SIPALG   0xA2
#define NFMARK_RTP      0xA3
#define NFMARK_RTCP     0xA4
#define NFMARK_H323     0xA5
#define NFMARK_NON_REG  0xA6
#define NFMARK_NON_RTP  0xA7
#define NFMARK_GRE_CTL  0xA8


/* TR069 */
#define NFMARK_TR069    0xAA
/*Start of Protocol 2014-3-25 16:21 for ·�ɻ����޸� by t00189036*/
#define NFMARK_TR069_CONTRACK   0x800
/*End of Protocol 2014-3-25 16:21 for by t00189036*/
/* IPTV */
#define NFMARK_IPTV     0xB0
#define NFMARK_RTSP     0xB1
/* PPP */
#define NFMARK_PPP      0xB5
/* IGMP */
#define NFMARK_IGMP     0xB6
/* DNS */
#define NFMARK_DNS      0xB7
/* DHCP */
#define NFMARK_DHCP     0xB8
/* FTP */
#define NFMARK_FTP      0xB9
enum/*mark*/
{
    QOSDT_MARK_BE           = 0x00,  /* data */
    QOSDT_MARK_FON          = 0x01,  /* data */
    QOSDT_MARK_UCAST        = 0x02,  /* unicast */
    QOSDT_MARK_MCAST        = 0x03,  /* multicast */
    QOSDT_MARK_VOIP_BEARER  = 0x04,  /* voice RTP/RTCP */
    QOSDT_MARK_VOIP_CTL     = 0x05,  /* voice SIP */
    QOSDT_MARK_FTPACK       = 0x06,  /*FTP download ACK*/
    QOSDT_MARK_TOP          = 0x07,   /* PPP, PPPoE, DHCP, Gararntee bandwidth */
    
    QOSDT_MARK_NON_VOIP_CTL = 0x09, /*non t-online sip*/
    QOSDT_MARK_NON_VOIP_BEARER = 0x0e,  /*non t-online rtp/rtcp*/
    QOSDT_MARK_MASK            = 0x0f
};

enum/*TOS*/
{
    QOSDT_TOS_BE            = 0x00,
    QOSDT_TOS_UCAST         = 0x60,
    QOSDT_TOS_MCAST         = 0x80,
    QOSDT_TOS_VOIP_BEARER   = 0xA0,
    QOSDT_TOS_VOIP_CTL      = 0xC0,
    QOSDT_TOS_BUTT          = 0xFF
};
#endif
/* End of modified by f00120964 for qos function 2012-4-2*/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
#define EBT_IPT_IFC_INVERSE           " ! -i "
#define EBT_IPT_IFC_O_INVERSE         " ! -o "
#define IPT_SIP_INVERSE               " ! -s "
#define IPT_DIP_INVERSE               " ! -d "
#define IPT_ICMPV6_TYPE_INVERSE       " ! --icmpv6-type "

#else
#define EBT_IPT_IFC_INVERSE           " -i ! "
#define EBT_IPT_IFC_O_INVERSE         " -o ! "
#define IPT_SIP_INVERSE               " -s ! "
#define IPT_DIP_INVERSE               " -d ! "
#define IPT_ICMPV6_TYPE_INVERSE       " --icmpv6-type ! "
#endif

#ifdef SUPPORT_ATP_MANUALUPG
#define WEB_DOWNLOAD_VERSION_FILE         "/var/version"
#define WEB_DOWNLOAD_STATE_FILE           "/var/downloadstate"
#define WEB_DOWNLOAD_FILE_FAILED          "0"
#define WEB_DOWNLOAD_IMAGE_FAILED         "1"
#define WEB_DOWNLOAD_IMAGE_SUCCESS        "2"
#define WEB_DOWNLOAD_WRITE_SUCCESS        "3"
#define WEB_DOWNLOAD_IMAGE_INVALID        "4"
#endif

/*Mount*/
#define USBDEVICE_MNT_DIR               "/mnt"
#define ATP_PHYMEDIUM_FILE   "/proc/proc_user_usbdevs"   
#define ATP_PROCPRINTER_FILE   "/proc/proc_user_printer"  
#define ATP_HARDDISK_MAXNUM     6
#define USB_LOGICVOLFSTYPE_LEN  12

#define ATP_PATH_DECT_STATUS  "/var/voice/dectstatus"
#define ATP_PATH_WLAN_STATUS  "/var/WpsSuccess"

/*Start:z00182249 for UDK0000294273 20130814*/
#define CWMP_SETPPP_FLAG_FILE  "var/cwmp_setppp_flag"
/*End:z00182249 for UDK0000294273 20130814*/
#define ROUTE6_STATUS_FILE    "/var/route6_status_file"
#define ROUTE_STATUS_FILE    "/var/route_status_file"

#define DSL_INTF_NAME       "ppp256"
#define DSL_TUNNEL_NAME     "gre1"
#define LTE_TUNNEL_NAME     "gre2"
#define BONDING_TUNNEL_NAME "gre"

#define DHCPS_FILE_NEIGHBOR    "/var/dhcp/dhcps/neighbors_new"
#define IPV6_MASK_128          128


/* BEGIN: Added by c65985, 2013/12/2   PN:DTS2013113008760*/
#define APP_PPP_TRIGER_MARK     0x100000    //���ڱ�ʾӦ�ò㴥�����貦��
#define PPP_TRIGER_MARK         0x200000    //���ڱ�ʾlan �ഥ�����貦��

#define LAN_PKT_DEFAULT_MARK    PPP_TRIGER_MARK

/*�뱣��INTERFACE_POLICE_ROUTING_6_MARK��LAN_PKT_DEFAULT_MARKֵ��ͬ*/
#define TUNNEL_POLICE_ROUTING_6_MARK    PPP_TRIGER_MARK
#define TUNNEL_POLICE_ROUTING_6_MASK    PPP_TRIGER_MARK

/*�뱣��INTERFACE_POLICE_ROUTING_MARK��LAN_PKT_DEFAULT_MARKֵ��ͬ*/
#define TUNNEL_POLICE_ROUTING_MARK		PPP_TRIGER_MARK
#define TUNNEL_POLICE_ROUTING_MASK		PPP_TRIGER_MARK
/* END:   Added by c65985, 2013/12/2 PN:DTS2013113008760*/

#define DSL_TUNNEL_WANPATH  "Device.IP.Interface.7."
#define LTE_TUNNEL_WANPATH  "Device.IP.Interface.8."
#define DSL_PPP_WANPATH     "Device.IP.Interface.2."
#define BONDING_WANPATH     "Device.IP.Interface.5."
/*Start of ATP 2014-3-31 for DTS2014032408036 by y00250667*/
#define ATP_VLAN8_WANPATH     "Device.IP.Interface.3"
/*End of ATP 2014-3-31 for DTS2014032408036 by y00250667*/


#define ATP_FILE_BNG_MODE        "/var/sysinfo/sys_bnginfo"
#define ATP_GRE2_STATUS6_FILE    "/var/wan/gre2/status6"
#define ATP_GRE2_STATUS_FILE     "/var/wan/gre2/status"

/* Host Info Macro */
#define ATP_UTIL_BR_FDB_MAX   128
#define ATP_UTIL_MAC_LEN   18
#define ATP_UTIL_INTERFACE_LEN  16
#define ATP_UTIL_ISLOCAL_LEN    8

#ifdef SUPPORT_ATP_HYBRID_BONDING
#define ATP_CONNTRACK_GET_OUTPUT_TEST_MARK 0x2000
#define ATP_SENDTO_RET_INDEX_SHIFT_MIN (-10000)
#define ATP_SENDTO_RET_INDEX_SHIFT_MAX (-20000)
#endif

typedef struct 
{
    VOS_UINT8 ucProtocol; /*IPPROTO_UDP 17 or IPPROTO_TCP 6*/
    VOS_UINT8 ucTos;
    VOS_UINT16 usDstPort;
    VOS_UINT16 usSrcPort;
    VOS_UINT32 ulMark;
    VOS_CHAR acDstIP[ATP_STRING_LEN_64];
} ATP_UTIL_PKT_INFO_ST;


/* BEGIN: Added by z67728, 2011/3/2 BT��ȫ������,�޸Ĵ����е�strcyp��sprintfϵͳ����
   Ϊstrncpy��snprintf.*/
#define STRNCPY(DstStr,SrcStr) \  
    do \
    {\    
        strncpy(DstStr,SrcStr,sizeof(DstStr) - 1);\
        DstStr[sizeof(DstStr) - 1] = 0; \
    }while(0)

#define SNPRINTF(DstStr,format,arg...) \
    do \
    { \
        snprintf(DstStr,sizeof(DstStr),format,## arg); \
    }while(0)
/* END:   Added by z67728, 2011/3/2 */

/*Interface Info*/
//!\brief �ӿ���Ϣ�ṹ
/*!
 *  ��¼�ӿھ��ϴθ�λ����ա����͵ķ��������ֽ��������������
 */
typedef struct
{
    unsigned long ulBytesReceived;        /* �ϴθ�λ�󣬴˽ӿڽ��յ����ֽ��� */
    unsigned long ulPacketsReceived;      /* �ϴθ�λ�󣬴˽ӿڽ��յķ������� */
    unsigned long ulPacketsReceivedErr;   /* �ϴθ�λ�󣬴˽ӿڽ��յĴ���ķ�������*/
    unsigned long ulPacketsReceivedDrops; /* �ϴθ�λ�󣬴˽ӿڽ��յĶ����ķ�������*/
    unsigned long ulBytesSent;            /* �ϴθ�λ�󣬴˽ӿڷ��͵����ֽ��� */
    unsigned long ulPacketsSent;          /* �ϴθ�λ�󣬴˽ӿڷ��͵ķ������� */
    unsigned long ulPacketsSentErr;       /* �ϴθ�λ�󣬴˽ӿڷ��͵Ĵ���������� */
    unsigned long ulPacketsSentDrops;     /* �ϴθ�λ�󣬴˽ӿڷ��͵Ķ����������� */
}ATP_UTIL_INTERFACE_STAT_ST;

/* Host Info which got by brctl showmacs */
typedef struct
{
    VOS_UINT8  portNum;
    VOS_CHAR    macAddr[ATP_UTIL_MAC_LEN];
    VOS_CHAR    intfaceName[ATP_UTIL_INTERFACE_LEN];
    VOS_CHAR    isLocal[ATP_UTIL_ISLOCAL_LEN];
} ATP_UTIL_BR_FDB_INFO;

#define WAN_TO_INTERFACE "/var/wan/wan2interface"
#define WAN_CMDWORD_LEN 256
#define WAN_STATUS_OK 1
#define WAN_STATUS_NOK 0

typedef enum tagWAN_KEYWORD_EN
{
    ATP_WAN_NONE=0,
    ATP_WAN_PPPOE,
    ATP_WAN_DHCP,
    ATP_WAN_UMTS,
}WAN_KEYWORD_EN;

typedef enum tagWAN_LEVEL_EN
{
    ATP_WAN_LEVEL_PHY=1,
    ATP_WAN_LEVEL_LINK,
    ATP_WAN_LEVEL_IP,
}WAN_LEVEL_EN;

typedef enum 
{ 
	ATP_XDSL_MODE_ATM,
	ATP_XDSL_MODE_PTM,
	ATP_XDSL_MODE_UNKNOWN,
}ATP_XTM_MODE_EN;


/*function macro*/
#if defined(__cplusplus)
extern "C" {
#endif
/*һ.Valid Function*/
/*1.IPv6 Valid*/
/*!
  \brief ��ȡIpv6 ������·��ַ
  \param[in]  pszIfc: �ӿ���
  \param[out]  pszAddr: ��ȡ���ı�����·��ַ
  \return �Ƿ��ȡ�ɹ�
  \retval VOS_TRUE  ��ȡ�ɹ�
  \retval VOS_FALSE ��ȡʧ��
*/
VOS_BOOL ATP_UTIL_GetIpv6LinkAddress(const VOS_CHAR* pszIfc, VOS_CHAR* pszAddr);

/*��.common call functions*/
/*1.convert & get & set Function*/
/*!
  \brief  ���ַ���ת�����ܱ�Shell ʶ����ַ���
  \param[in]  *pszStr: ��ת���ַ���
  \param[out]  *pszConvertedStr: ת������ַ���
  \return �Ƿ�ת���ɹ�
  \retval VOS_TRUE  ת���ɹ�
  \retval VOS_FALSE ת��ʧ��
*/
VOS_BOOL ATP_UTIL_ConvertStrToShellStr(VOS_CHAR *pszStr, VOS_CHAR *pszConvertedStr);

/*2.ExecCmd Function*/
/*!
  \brief ִ��SHELL������ȴ�20s
  \param[in]  *pcCommand: SHELL����
  \param[in]  ucSecond: �ȴ���ʱ�䣬��λΪ��
  \return  0  ��ʾ����ִ�гɹ�
  \return -1 ��ʾ����ִ��ʧ��
*/
VOS_INT ATP_UTIL_ExecCmd_ByTime(VOS_CHAR *pcCommand, VOS_UINT32 ucSecond);

/*!
  \brief  ִ�������������sh����, ���������滻system����, ���ȴ�40s,��ʱ�ӽ�killִ�н���,����-1
           ������ں�ִ̨��,����ĩβ��&
  \param[in]  *pcCommand: ��ִ������
  \return �����Ƿ�ִ�гɹ�
  \retval lStatus ����ִ��״̬
  \retval 0  �����Ľ���pid��Ϊ0
  \retval -1 ��ʾ����ִ��ʧ��(e.g.  fork() failed , pcCommand is null, timout)
*/
VOS_INT  ATP_UTIL_ExecCmdNoHang(VOS_CHAR *pcCommand);
/*!
  \brief ���������ȡ�����̺�
  \param[in]  pcCommand: ����ȡ���̺ŵ�����
  \return�Ƿ���ҳɹ�
  \retval pPid  ���ҵ��Ľ��̺� ������0
  \retval 0  ��ʾ����ʧ��
*/
VOS_INT ATP_UTIL_GetPid(VOS_CHAR * pcCommand);
/*!
  \brief  ���ݽ������õ����̺ţ�����ж��ƥ������г�����ƥ��Ľ���ID
  \param[in]  pszPidName: ������
  \return ָ����̺��б��ָ��
  \retval pPidList  ��ȡ���Ľ��̺��б�ָ��
  \retval VOS_NULL ��ȡ���̺��б�ΪNULL
*/
VOS_INT32 *ATP_UTIL_FindPidByName( VOS_CHAR* pszPidName);

VOS_INT32 ATP_UTIL_FindKernelPidByName( VOS_CHAR* pszPidName);

VOS_BOOL ATP_UTIL_IsExistDefaultRoute(VOS_BOOL isIpv6);

#define ATP_Exec(cmd)		                ATP_UTIL_ExecCmdNoHang (cmd) //����ӡ��������Ϣ
#define ATP_ExecDebug(cmd)	                ATP_UTIL_ExecCmdNoHang (cmd) //��ӡ��������Ϣ1->0

/*3.InterfaceName Function*/
/*!
  \brief ���ݽӿ�����ȡIp��ַ��ֻ���Ipv4
  \param[in]  pszIfName: �ӿ���
  \param[out]  pstAddr: ��ȡ����sockaddr_in Ip��ַ
  \return �Ƿ��ȡ��ַ�ɹ�
  \retval VOS_OK   ��ȡ��ַ�ɹ�
  \retval VOS_NOK ��ȡ��ַʧ��
*/
VOS_UINT32 ATP_UTIL_GetIpByName(const VOS_CHAR* pszIfName, struct sockaddr_in* pstAddr);

/*4. 3GUsbStickNode*/
#ifdef SUPPORT_ATP_AT
//!\brief Ҫ�������ݿ�USB ���ڽӵ������
/*!
 *at ����ӵ�ţ��������ݽӵ�ţ������ӵ��
 */
typedef enum tagATP_3G_TYPE_REQ_EN
{
    ATP_3G_BEGIN_REQ,
    ATP_3G_PCUIND_REQ,         // ��ȡat ����ӵ��
    ATP_3G_MODEMND_REQ,        // ����������ݽӵ��
    ATP_3G_VOICEND_REQ,      // ��������ӵ��
    ATP_3G_END_REQ,
}ATP_3G_TYPE_REQ_EN;
/*!
  \brief ������ݿ���Ӧ���ܵ�USB ���ڽӵ��
  \param[in]  enReqType: Ҫ��Ľڵ������
  \param[in]  pszRetNode:���ڴ�Ŷ�Ӧ�Ľӵ�����
  \return �Ƿ��ȡ�ɹ�
  \retval VOS_TRUE  ��ȡ�ɹ�
  \retval VOS_FALSE ��ȡʧ��
*/
VOS_INT32 ATP_Get3GUsbStickNode(ATP_3G_TYPE_REQ_EN enReqType, VOS_CHAR * pszRetNode, VOS_UINT32 ulLen);
#endif

/*5. Common*/
/*!
  \brief ���ַ����е�'\0'ȥ��
  \param[in]  *pcBuf: ��ȥ��'\0'���ַ���
  \return �Ƿ�ȥ���ɹ�
  \retval VOS_TRUE  ȥ���ɹ�
  \retval VOS_FALSE ȥ��ʧ��
*/
VOS_BOOL ATP_UTIL_RmDelimitor( VOS_CHAR *pcBuf);

/*6. Interface Type*/
/*!
  \brief ��ȡ�ӿ�����
  \param[in]  *pcIntf: ����ȡ�ӿ�
  \return ��ȡ���Ľӿ�����
  \retval ulType ��ȡ���Ľӿ�����
  \retval 0 ��ȡ�ӿ�����ʧ��
*/
VOS_UINT32 ATP_UTIL_GetIntfType(const VOS_CHAR *pcIntf);

/*7. Qos*/

/* Start of modified by f00120964 for qos function 2012-03-23*/
/*!
  \brief ����һ��socketfd�����ø�sockfd��markֵ�����ݸ�MARKֵ������QoS
  \param[in]  ulSockfd: ������socketfd
  \param[in]  ulMarkValue: ���õ�Markֵ
  \return �Ƿ����óɹ�
  \retval  0  ���óɹ�
  \retval -1 ����ʧ��
*/
VOS_INT32  ATP_UTIL_QosSetMark(const VOS_UINT32 ulSockfd, const VOS_UINT32 ulMarkValue);
/* End of modified by f00120964 for qos function 2012-03-23*/

/*9. Wan Backup*/
#ifdef SUPPORT_ATP_WAN_BACKUP
/*!
  \brief ATP_UTIL_WanBackUpGetBannerFileδʵ��
  \param[in] VOS_VOID: 
  \return 
  \retval 
*/
VOS_VOID ATP_UTIL_WanBackUpGetBannerFile(const VOS_CHAR* pszBannerUrl);
#endif

/*11. Upg*/
typedef enum tagATP_UPG_FILE_EN
{
    ATP_UPG_FILE_TYPE_UNKNOWN = 0,
    ATP_UPG_MUPG_GZIP_TYPE,
    ATP_UPG_MUPG_USTAR_TYPE,
    ATP_UPG_UPG_FULL_TYPE,

    ATP_UPG_FILE_TYPE_EXTEND
} ATP_UPG_FILE_EN;

#ifdef SUPPORT_ATP_MUPG
#define MUPG_PATH_LEN         256
/*!
  \brief ��slave�е�����һ�����ύ
  \return �����ύ�Ƿ�ɹ�
  \retval  0 MakeCommit Success
  \retval -1 MakeCommit Failed
*/
VOS_UINT32 ATP_UTIL_MupgMakeCommit();
#endif

/*12. TTL*/
#ifdef SUPPORT_ATP_VOICE
#define ATP_DNS_NAME_TOTAL_LEN_D (255)
typedef struct tagATP_DNS_TTL_REG_ST
{
    VOS_CHAR        acName[ATP_DNS_NAME_TOTAL_LEN_D + 1];
    VOS_UINT32      ulType;
    VOS_UINT32      ulClass;
} ATP_DNS_TTL_REG_ST;
VOS_UINT32 ATP_UTIL_DnsTtlListen_Reg(VOS_UINT32 ulType, const VOS_CHAR *pcDomainName);
VOS_UINT32 ATP_UTIL_DnsTtlListen_UnReg(VOS_UINT32 ulType, const VOS_CHAR *pcDomainName);
#endif

/*13. Match Type*/
typedef enum tagATP_UTIL_CSTRMATCH_PATTERN { 
    ATP_UTIL_STRMATCH_EXACT = 0, 
    ATP_UTIL_STRMATCH_PREFIX, 
    ATP_UTIL_STRMATCH_SUFFIX, 
    ATP_UTIL_STRMATCH_SUBSTRING,
    ATP_UTIL_STRMATCH_ERROR
} ATP_UTIL_CSTRMATCH_PATTERN;
/*!
  \brief �����ַ�����ȡ��Ӧ��ATP_UTIL_CSTRMATCH_PATTERN����
  \param[in]  str: ����ѯ�ַ���
  \return ����ƥ�䵽������
  \retva type  �ҵ��Ŀ�ƥ������
  \retval ATP_UTIL_STRMATCH_EXACT strΪ��ʱ
  \retva ATP_UTIL_STRMATCH_ERROR δ�ҵ���ƥ�������
*/
ATP_UTIL_CSTRMATCH_PATTERN ATP_UTIL_GetMatchType(const VOS_CHAR * str);

/*14. IPP*/
#ifdef SUPPORT_ATP_IPP
/*!
  \brief �жϴ�ӡ���Ƿ����
  \param[in] VOS_VOID: ��
  \return ���ش�ӡ���Ƿ���ڱ�־
  \retval VOS_TRUE ��ӡ������
  \retval VOS_FALSE ��ӡ��������
*/
VOS_BOOL ATP_UTIL_IsPrinterExist(VOS_VOID);
#endif

/*15.Dynamic Launched Application Update*/
/*!
  \brief ���¶�̬Ӧ�ó����б�
  \param[in]  *pcAppName: ����µ�Ӧ�ó�����
  \param[in]  bStop: �Ƿ�ֹͣ����
  \return ���ط�����Ϣ������
  \retval VOS_OK ������Ϣ���ͳɹ�
  \retval VOS_NOK pcAppNameΪNULL
  \retval ATP_MSG_RET_PARA_ERR  ��Ϣ��������
  \retval ATP_MSG_RET_NOT_INITED δ��ʼ��
  \retval ATP_MSG_RET_OVERFLOW  ��Ϣ���
  \retval ATP_MSG_RET_PEER_DEAD  socket��peer�ر�
  \retval ATP_MSG_RET_INTERNAL_ERR  �ڲ�����
*/
VOS_UINT32 ATP_UTIL_UpdateDynAppListenFd(const VOS_CHAR *pcAppName, VOS_BOOL bStop);

/*16.InterfaceInfo Function*/
/*!
  \brief  ��ȡ�ӿ�pszIfcName��ͳ����Ϣ
  \param[in]  *pszIfcName: ����ȡͳ����Ϣ�Ľӿ���
  \param[out]  *pstState: �ӿ�ͳ����Ϣ
  \return ��
  \retval ��
*/
VOS_VOID ATP_UTIL_GetIfcStatInfo(VOS_CHAR *pszIfcName, ATP_UTIL_INTERFACE_STAT_ST *pstState);

#ifdef ATP_DEBUG
#define ATP_UTIL_DEBUG(format, args...)\
{\
    printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__func__, __LINE__);\
    printf(format, ##args);\
}
#else
#define ATP_UTIL_DEBUG(x...)
#endif
/*!
  \brief ���ַ���str�и��ݴ�ƥ������typeƥ���Ƿ����*pattern
  \param[in]  str: ����֤�ַ���
  \param[in]  pattern: ����ƥ����ַ���
  \param[in]  type: ��ƥ������
  \return �����Ƿ�ƥ��ɹ���־
  \retval VOS_TRUE ƥ��ɹ�
  \retval VOS_FALSE ƥ��ʧ��
*/
VOS_BOOL ATP_UTIL_CstrMatch(const VOS_CHAR * str, const VOS_CHAR * pattern, ATP_UTIL_CSTRMATCH_PATTERN type);


/*!
  \brief ����LAN�ӿ���(eth0.4)��ȡbr
  \param[in]  pszLan: 
  \param[in]  pszBrName: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_UTIL_GetBrByLanInterface(const VOS_CHAR* pszLan, VOS_CHAR* pszBrName, VOS_UINT32 ulLen);

/*!
  \brief ͨ��mac ��ַ��ȡ�ӿ���
  \param[in]  psMacaddr: ����ȡ�ӿڵ�MAC ��ַ
  \param[out]  psInterfaceName: ��ȡ���ýӿ���
  \param[in]  ulInfNameLen: �ӿ�������
  \return ��ȡ�ӿ����Ƿ�ɹ�
  \retval VOS_OK  ��ȡ�ӿ����ɹ�
  \retval VOS_NOK  ��ȡ�ӿ���ʧ��
*/
VOS_UINT32 ATP_UTIL_GetInterfaceNameByMacAddr(const VOS_CHAR * psMacaddr, VOS_CHAR* psInterfaceName, VOS_UINT32 ulInfNameLen);

/*!
  \brief ��ȡ����bridge�Ľӿ�����
  \param[in]  pcNameBuf: ��Žӿ����Ļ�����
  \param[in]  ulBufLen: ��Žӿ����Ļ���������
  \return �ӿڸ���
*/
VOS_UINT32 ATP_UTIL_GetBridgeAllNames(VOS_CHAR *pcNameBuf, VOS_UINT32 ulBufLen);

VOS_UINT32 ATP_UTIL_GetBrFdbInfoByMac(const VOS_CHAR* psMacaddr, ATP_UTIL_BR_FDB_INFO * pstFdbInfo);

VOS_UINT32 ATP_UTIL_GetBrFdbInfoByInterface(const VOS_CHAR* psInterfaceName, ATP_UTIL_BR_FDB_INFO * pstHostInfo, VOS_UINT32 ulNum);

VOS_BOOL ATP_UTIL_ValidatePort(VOS_UINT32 ulPort);

VOS_UINT32 ATP_GetWan2InterfaceName(WAN_KEYWORD_EN ulWanKeyWord,VOS_CHAR *pszInterface, VOS_UINT32 ulIntfLen);

VOS_UINT32 ATP_GetWan2InterfaceStatus(WAN_LEVEL_EN ulConnLevel,WAN_KEYWORD_EN ulWanKeyWord,VOS_UINT32 *puzStatus);
VOS_VOID ATP_UTIL_TriggerDemand(VOS_VOID);

VOS_UINT32 ATP_UTIL_CheckTimeValid(ATP_UTIL_PERIODICTIMEINT_ST astPerTimeInfo);
VOS_UINT32 ATP_UTIL_GetDifTime(const VOS_CHAR *pstPrdTime, const VOS_CHAR *pstCurTime, 
                                                VOS_UINT32 *pstDifVal, VOS_UINT32 *pstPrdicFlag);
VOS_VOID ATP_UTIL_GetDelayTime(VOS_UINT32 ulDetaTime, VOS_UINT32 ulInformInterval, VOS_UINT32 ulPrdFlag, VOS_UINT32 *pulDelayTime);
VOS_UINT32 ATP_UTIL_UCTTimeFormatCheck(const VOS_CHAR *pstTime, ATP_UTIL_PERIODICTIMEINT_ST *pstTimeInt);

VOS_UINT32 ATP_UTIL_GetUploadBandwidth();
VOS_UINT32 ATP_UTIL_GetDownloadBandwidth();
VOS_UINT32 ATP_UTIL_GetXdslActiveMode(VOS_UINT32 *pulActiveMode);

VOS_UINT32 ATP_IS_ValidDomain(VOS_CHAR *acDomainName, VOS_UINT32 ulMaxLength);
    
#ifdef SUPPORT_ATP_VOICE
VOS_BOOL ATP_UTIL_VoiceGetVoipStatus(VOS_VOID);
#endif

VOS_BOOL    ATP_UTIL_JudgeIsLittleEnd(VOS_VOID);
VOS_UINT64 ATP_UTIL_Htonll(const VOS_UINT64 ullHostLonglong);
VOS_UINT64 ATP_UTIL_Ntohll(const VOS_UINT64 ullNetLonglong);
VOS_BOOL ATP_UTIL_ValidateLength(VOS_CHAR *pszStr, VOS_UINT ulMax);
VOS_BOOL ATP_UTIL_ValidateNumber(VOS_CHAR* pcNum);
VOS_BOOL ATP_UTIL_DECTRegister_Status(VOS_VOID);
VOS_INT32 ATP_UTIL_CheckWlanStatus(void);
VOS_BOOL ATP_UTIL_WPSRegister_Status(VOS_VOID);

VOS_CHAR *ATP_UTIL_IconvConvert (const VOS_CHAR *pszInput, const char* tocode, const char* fromcode);

void ATP_UTIL_get_dev_stats(const char * pszDev, struct rtnl_link_stats * pstStats);

VOS_BOOL ATP_UTIL_IsLTEWanV4Up(void);
VOS_BOOL ATP_UTIL_IsLTEWanV6Up(void);
VOS_BOOL ATP_UTIL_IsWanV6Up(void);
VOS_BOOL ATP_UTIL_IsWanV4Up(void);
VOS_BOOL ATP_UTIL_IsTunnelUp(void);
VOS_UINT32 ATP_UTIL_GetV6Addr(VOS_CHAR* pcAddr, VOS_UINT32 ulLen);
VOS_UINT32 ATP_UTIL_GetV4Addr(VOS_CHAR* pcAddr, VOS_UINT32 ulLen);

VOS_BOOL ATP_UTIL_IsIPv4LanAddr( VOS_CHAR * pszIp);
VOS_BOOL ATP_UTIL_IsIPv6GlobalAddr( VOS_CHAR * pszIp);
/*!
  \brief  get system current time
  \return 
  \retval str format hh:mm:ss
*/
const VOS_CHAR* ATP_UTIL_GetCurrTime();

/*!
  \brief  get system current time
  \return 
  \retval str format Y-m-dThh:mm:ss
*/
const VOS_CHAR* ATP_UTIL_GetCurrUTCTime();
VOS_BOOL ATP_UTIL_IsLinklocalOrULAAddr( VOS_CHAR * pszIp);

VOS_BOOL ATP_UTIL_ValidateIpAddress(VOS_CHAR *pcIpAddr) ;
/*Start:z00182249 for UDK0000294273 20130814*/
VOS_UINT32 ATP_GetPPPInfo_FromCfgFile(VOS_CHAR* pszPPPUserName, VOS_UINT32 lPPPUserNameLen, VOS_CHAR* pszPPPassWord, VOS_UINT32 lPPPassWordLen, VOS_UINT32 * pszISPFlag);
/*End:z00182249 for UDK0000294273 20130814*/

/*!
  \brief gen random buffer
  \param[in]  pcBuffer: 
  \param[in]  ulLen: 
  \return 
  \retval 
*/
VOS_UINT32 ATP_UTIL_GenRandomBuffer(VOS_UINT8 * pucBuffer, VOS_UINT32 ulLen);

/*!
  \brief  gen Random str
  \param[in]  *pcStrBuffer: 
  \param[in]  ulLen: 
  \return 
  \retval 
*/
VOS_UINT32 ATP_UTIL_GenRandomStr(VOS_CHAR *pcStrBuffer, VOS_UINT32 ulLen);

/*!
  \brief gen random integer
  \param[in]  *pulRandomNum: 
  \return 
  \retval 
*/
VOS_UINT32 ATP_UTIL_GenRandomInteger();

VOS_UINT32 ATP_UTIL_GetMaskByName(const VOS_CHAR* pszIfName, struct sockaddr_in* pstAddr);

VOS_VOID ATP_UTIL_GetIpv6AddrEffectByMask(struct in6_addr *pstAddr, struct in6_addr *pstAddrMask);

VOS_VOID ATP_UTIL_GetIpv4AddrEffectByMask(VOS_INT *pIpAddr, VOS_INT *pIpMask);

VOS_BOOL ATP_UTIL_IpAddrSplit(VOS_CHAR *pszIpAddrValue, VOS_INT *pSplitValue) ;

VOS_UINT32 ATP_UTIL_TurnNumToMask6(struct in6_addr *pstAddr, VOS_INT32 lMask);

VOS_BOOL ATP_UTIL_GetBr0Ipv6Address(VOS_CHAR* pszAddr, VOS_UINT32 ulBufLen);

VOS_UINT32 ATP_UTIL_GetInterfaceMtu(const VOS_CHAR* pcIntf, VOS_UINT32 *pulMtu);

/*Start of Protocol 2014-5-24 for UDK0000351459 Filter list TTL�޸�  by y00250667*/
VOS_UINT32 ATP_UTIL_CleanConntrackByIpPort(const VOS_CHAR* pcAddr, VOS_UINT16 usMask, VOS_UINT16 usPort, VOS_BOOL bIsDst, CT_TYPE enCtType);

VOS_UINT32 ATP_UTIL_CleanConntrackByIp6Port(const VOS_CHAR* pcAddr, VOS_UINT16 usMask, VOS_UINT16 usPort, VOS_BOOL bIsDst, CT_TYPE enCtType);
/*End of Protocol 2014-5-24 for UDK0000351459 Filter list TTL�޸�  by y00250667*/

/*!
  \brief is in bng mode
  \param[in]   
  \return 
  \retval 
*/
VOS_BOOL ATP_UTIL_IsBngMode();
/*Start of Protocol 2014-3-25 16:21 for ·�ɻ����޸� by t00189036*/
#ifdef SUPPORT_ATP_HYBRID_BONDING
VOS_INT32  ATP_UTIL_SetFilterlistControlMark(VOS_INT32 ulSockfd);
VOS_CHAR* ATP_UTIL_GetOutputIntf(const ATP_UTIL_PKT_INFO_ST *pstPkt);
#endif
/*End of Protocol 2014-3-25 16:21 for by t00189036*/

/*Start of Protocol 2014-5-23 16:25 for HOMEGW-17282:���blacklist·�� by t00189036*/
VOS_VOID ATP_UTIL_GetGatewayByIntf(VOS_BOOL bIsIPv4, const VOS_CHAR* pcIntf, VOS_CHAR* pcGateway, VOS_UINT32 ulLen);
/*End of Protocol 2014-5-23 16:25 for by t00189036*/

#if defined(__cplusplus)
}
#endif

#endif

