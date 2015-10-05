/**\file atputil.h
  * ATP���ƽ̨��������ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author z37589
  *\date  2007-8-23
*/
/*******************************************************************************
Edit History of Huawei: 
<�޸���>     <�޸�ʱ��>    <���ⵥ��>          <�޸�����>
z81004143    2013/05/03    DTS2013050301791    [TR069] B890-66 V100R001C994B122(ROUTER): ����TR069����
z00159386    2013/07/06    DTS2013070601078    70V voip���Ժ���B890����
*******************************************************************************/

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

/* <DTS2013050301791 z81004143 2013/5/6 begin */
#ifdef SUPPORT_ATP_FIREWALL_B890
/* ·������汾�� */
#define VERSION_ROUTE_PATH          "/www/web.version"
#define VERSION_ROUTER_FIELD        "HUAWEI_EXTRAVERSION"
#define VERSION_ROUTER_PDM_FIELD    "HUAWEI_APPPDMVERSION"
#define VERSION_WEB_HTML_FIELD      "HUAWEI_HTMLCODEVERSION"
#define VERSION_WEB_HTML_PDM_FIELD  "HUAWEI_HTMLCODEPDMVERSION"
#define VERSION_WEB_RES_FIELD       "HUAWEI_RESVERSION"
#define VERSION_WEB_RES_PDM_FIELD   "HUAWEI_RESPDMVERSION"
#define VERSION_MAX_LEN             256
#define VERSION_ROUTER_HW_FIELD     "HUAWEI_HARDVERSION"  /* DTS2013042007588 l00162999 20130514 */

/* ·��Ӳ���汾�� */
#define VERSION_ROUTER_HW       "WL1B890I Ver.A"

/* Modem��Ӳ���汾�� */
#define VERSION_MODEM_SW_PATH   "/var/log/modem_software_name"
#define VERSION_MODEM_HW_PATH   "/var/log/modem_hardware_name"

/* DeviceInfo other */
#define INFO_STR_MAX_LEN        256
#define MODEM_IMEI_PATH         "/var/log/imei_name"
#define MODEM_IMSI_PATH         "/var/log/imsi_name"
#define MODEM_ICCID_PATH        "/var/log/iccid_name"

#define D(x)  x
#endif
/* DTS2013050301791 z81004143 2013/5/6 end> */

/*NFMARK*/
/* start of QoS by d00107688 2008-9-23 ��չ�� setsockopt ������������TR069�ȴ��ϱ����QoS */
/* ���ֵ��������չ�ں˵�setsockopt�ı�ŵģ����Ҫ�����ں��е� asm/socket.h �еı仯 */
/*SO_NFMARK��21�ں��õģ�30�ں�ֻ��SO_MARK*/
#ifdef SO_MARK
#define SO_NFMARK       SO_MARK
#else
#define SO_NFMARK       66
#endif /* SO_MARK */
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
/* TR069 */
#define NFMARK_TR069    0xAA
#define NFMARK_TR069_CONTRACK   0xAB00
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
    QOSDT_MARK_UCAST        = 0x01,  /* unicast */
    QOSDT_MARK_MCAST        = 0x02,  /* multicast */
    QOSDT_MARK_VOIP_BEARER  = 0x03,  /* voice RTP/RTCP */
    QOSDT_MARK_VOIP_CTL     = 0x04,  /* voice SIP */
    QOSDT_MARK_FTPACK        = 0x06,  /*FTP download ACK*/
    QOSDT_MARK_TOP          = 0x07   /* PPP, PPPoE, DHCP, Gararntee bandwidth */
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


/*Mount*/
#define USBDEVICE_MNT_DIR               "/mnt"
#define ATP_PHYMEDIUM_FILE   "/proc/proc_user_usbdevs"   
#define ATP_PROCPRINTER_FILE   "/proc/proc_user_printer"  
#define ATP_HARDDISK_MAXNUM     6
#define USB_LOGICVOLFSTYPE_LEN  12

/* Host Info Macro */
#define ATP_UTIL_BR_FDB_MAX   128
#define ATP_UTIL_MAC_LEN   18
#define ATP_UTIL_INTERFACE_LEN  16
#define ATP_UTIL_ISLOCAL_LEN    8

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
VOS_INT32 ATP_Get3GUsbStickNode(ATP_3G_TYPE_REQ_EN enReqType, VOS_CHAR * pszRetNode, VOS_UINT32 ulLen);/* <DTS2013083001384 xiazhongyue 20130830 modify> */
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
/* <DTS2013070601078  z00159386  20130706  begin */
#if defined SUPPORT_ATP_APP_CLASS_QOS || defined SUPPORT_ATP_VOICE
/* DTS2013070601078  z00159386  20130706  end> */
/*!
  \brief ����һ��socketfd�����ø�sockfd��markֵ�����ݸ�MARKֵ������QoS
  \param[in]  ulSockfd: ������socketfd
  \param[in]  ulMarkValue: ���õ�Markֵ
  \return �Ƿ����óɹ�
  \retval  0  ���óɹ�
  \retval -1 ����ʧ��
*/
VOS_INT32  ATP_UTIL_QosSetMark(const VOS_UINT32 ulSockfd, const VOS_UINT32 ulMarkValue);
#endif
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

VOS_UINT32 ATP_GetWan2InterfaceName(WAN_KEYWORD_EN ulWanKeyWord,VOS_CHAR *pszInterface);

VOS_UINT32 ATP_GetWan2InterfaceStatus(WAN_LEVEL_EN ulConnLevel,WAN_KEYWORD_EN ulWanKeyWord,VOS_UINT32 *puzStatus);
VOS_VOID ATP_UTIL_TriggerDemand(VOS_VOID);

VOS_UINT32 ATP_UTIL_CheckTimeValid(ATP_UTIL_PERIODICTIMEINT_ST astPerTimeInfo);
VOS_UINT32 ATP_UTIL_GetDifTime(const VOS_CHAR *pstPrdTime, const VOS_CHAR *pstCurTime, 
                                                VOS_UINT32 *pstDifVal, VOS_UINT32 *pstPrdicFlag);
VOS_VOID ATP_UTIL_GetDelayTime(VOS_UINT32 ulDetaTime, VOS_UINT32 ulInformInterval, VOS_UINT32 ulPrdFlag, VOS_UINT32 *pulDelayTime);
VOS_UINT32 ATP_UTIL_UCTTimeFormatCheck(const VOS_CHAR *pstTime, ATP_UTIL_PERIODICTIMEINT_ST *pstTimeInt);

VOS_UINT32 ATP_UTIL_GetUploadBandwidth();
VOS_UINT32 ATP_UTIL_GetDownloadBandwidth();

#ifdef SUPPORT_ATP_VOICE
VOS_BOOL ATP_UTIL_VoiceGetVoipStatus(VOS_VOID);
#endif
VOS_BOOL    ATP_UTIL_JudgeIsLittleEnd(VOS_VOID);
VOS_UINT64 ATP_UTIL_Htonll(const VOS_UINT64 ullHostLonglong);
VOS_UINT64 ATP_UTIL_Ntohll(const VOS_UINT64 ullNetLonglong);
VOS_BOOL ATP_UTIL_ValidateLength(VOS_CHAR *pszStr, VOS_UINT ulMax);
VOS_BOOL ATP_UTIL_ValidateNumber(VOS_CHAR* pcNum);

void ATP_UTIL_get_dev_stats(const char * pszDev, struct rtnl_link_stats * pstStats);

/* <DTS2013050301791 z81004143 2013/5/6 begin */
#ifdef SUPPORT_ATP_FIREWALL_B890
VOS_UINT32 ATP_UTIL_EncryptWEBdata(const char *in, char *out);
VOS_UINT32 ATP_UTIL_DecryptWebData(const char *in, char *out, unsigned long len);
VOS_INT32 ATP_UTIL_GetVerFromFile(char *field, char *buf);
#endif
/* DTS2013050301791 z81004143 2013/5/6 end> */
/* DTS2013070108444 p00134535 2013-7-12 ��̬IPoE����var/wan/nas1����Ϣ��ȫ begin */
VOS_INT32 ATP_UTIL_Write_To_File(const char* path, const char* content);
/* DTS2013070108444 p00134535 2013-7-12 end */
#if defined(__cplusplus)
}
#endif

#endif

