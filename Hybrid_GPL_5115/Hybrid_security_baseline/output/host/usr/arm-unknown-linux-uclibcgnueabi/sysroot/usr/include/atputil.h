/**\file atputil.h
  * ATP软件平台公共函数头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author z37589
  *\date  2007-8-23
*/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z81004143    2013/05/03    DTS2013050301791    [TR069] B890-66 V100R001C994B122(ROUTER): 新增TR069功能
z00159386    2013/07/06    DTS2013070601078    70V voip特性合入B890主干
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
/* 路由软件版本号 */
#define VERSION_ROUTE_PATH          "/www/web.version"
#define VERSION_ROUTER_FIELD        "HUAWEI_EXTRAVERSION"
#define VERSION_ROUTER_PDM_FIELD    "HUAWEI_APPPDMVERSION"
#define VERSION_WEB_HTML_FIELD      "HUAWEI_HTMLCODEVERSION"
#define VERSION_WEB_HTML_PDM_FIELD  "HUAWEI_HTMLCODEPDMVERSION"
#define VERSION_WEB_RES_FIELD       "HUAWEI_RESVERSION"
#define VERSION_WEB_RES_PDM_FIELD   "HUAWEI_RESPDMVERSION"
#define VERSION_MAX_LEN             256
#define VERSION_ROUTER_HW_FIELD     "HUAWEI_HARDVERSION"  /* DTS2013042007588 l00162999 20130514 */

/* 路由硬件版本号 */
#define VERSION_ROUTER_HW       "WL1B890I Ver.A"

/* Modem软硬件版本号 */
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
/* start of QoS by d00107688 2008-9-23 扩展了 setsockopt 用来给语音，TR069等打上标记做QoS */
/* 这个值是用来扩展内核的setsockopt的编号的，因此要跟着内核中的 asm/socket.h 中的变化 */
/*SO_NFMARK是21内核用的，30内核只有SO_MARK*/
#ifdef SO_MARK
#define SO_NFMARK       SO_MARK
#else
#define SO_NFMARK       66
#endif /* SO_MARK */
/* 目前语音暂时全部打上同一个标记，后续如果还需要细分出SIP,RTP等，那么在这里修改该宏就可以了 */
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
/*DT 定制的mark标志和tos标志*/
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

/* BEGIN: Added by z67728, 2011/3/2 BT安全性问题,修改代码中的strcyp和sprintf系统调用
   为strncpy和snprintf.*/
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
//!\brief 接口信息结构
/*!
 *  记录接口距上次复位后接收、发送的分组数，字节数及错误情况等
 */
typedef struct
{
    unsigned long ulBytesReceived;        /* 上次复位后，此接口接收的总字节数 */
    unsigned long ulPacketsReceived;      /* 上次复位后，此接口接收的分组总数 */
    unsigned long ulPacketsReceivedErr;   /* 上次复位后，此接口接收的错误的分组总数*/
    unsigned long ulPacketsReceivedDrops; /* 上次复位后，此接口接收的丢弃的分组总数*/
    unsigned long ulBytesSent;            /* 上次复位后，此接口发送的总字节数 */
    unsigned long ulPacketsSent;          /* 上次复位后，此接口发送的分组总数 */
    unsigned long ulPacketsSentErr;       /* 上次复位后，此接口发送的错误分组总数 */
    unsigned long ulPacketsSentDrops;     /* 上次复位后，此接口发送的丢弃分组总数 */
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
/*一.Valid Function*/
/*1.IPv6 Valid*/
/*!
  \brief 获取Ipv6 本地链路地址
  \param[in]  pszIfc: 接口名
  \param[out]  pszAddr: 获取到的本地链路地址
  \return 是否获取成功
  \retval VOS_TRUE  获取成功
  \retval VOS_FALSE 获取失败
*/
VOS_BOOL ATP_UTIL_GetIpv6LinkAddress(const VOS_CHAR* pszIfc, VOS_CHAR* pszAddr);

/*二.common call functions*/
/*1.convert & get & set Function*/
/*!
  \brief  把字符串转换成能被Shell 识别的字符串
  \param[in]  *pszStr: 待转换字符串
  \param[out]  *pszConvertedStr: 转换后的字符串
  \return 是否转换成功
  \retval VOS_TRUE  转换成功
  \retval VOS_FALSE 转换失败
*/
VOS_BOOL ATP_UTIL_ConvertStrToShellStr(VOS_CHAR *pszStr, VOS_CHAR *pszConvertedStr);

/*2.ExecCmd Function*/
/*!
  \brief 执行SHELL命令，最多等待20s
  \param[in]  *pcCommand: SHELL命令
  \param[in]  ucSecond: 等待的时间，单位为秒
  \return  0  表示命令执行成功
  \return -1 表示命令执行失败
*/
VOS_INT ATP_UTIL_ExecCmd_ByTime(VOS_CHAR *pcCommand, VOS_UINT32 ucSecond);

/*!
  \brief  执行命令，不会启动sh进程, 可以用于替换system函数, 最多等待40s,超时加将kill执行进程,返回-1
           如果想在后台执行,命令末尾加&
  \param[in]  *pcCommand: 待执行命令
  \return 命令是否执行成功
  \retval lStatus 命令执行状态
  \retval 0  创建的进程pid号为0
  \retval -1 表示命令执行失败(e.g.  fork() failed , pcCommand is null, timout)
*/
VOS_INT  ATP_UTIL_ExecCmdNoHang(VOS_CHAR *pcCommand);
/*!
  \brief 根据命令获取到进程号
  \param[in]  pcCommand: 待获取进程号的命令
  \return是否查找成功
  \retval pPid  查找到的进程号 ，大于0
  \retval 0  表示查找失败
*/
VOS_INT ATP_UTIL_GetPid(VOS_CHAR * pcCommand);
/*!
  \brief  根据进程名得到进程号，如果有多个匹配则会列出所有匹配的进程ID
  \param[in]  pszPidName: 进程名
  \return 指向进程号列表的指针
  \retval pPidList  获取到的进程号列表指针
  \retval VOS_NULL 获取进程号列表为NULL
*/
VOS_INT32 *ATP_UTIL_FindPidByName( VOS_CHAR* pszPidName);

#define ATP_Exec(cmd)		                ATP_UTIL_ExecCmdNoHang (cmd) //不打印命令行信息
#define ATP_ExecDebug(cmd)	                ATP_UTIL_ExecCmdNoHang (cmd) //打印命令行信息1->0

/*3.InterfaceName Function*/
/*!
  \brief 根据接口名获取Ip地址，只针对Ipv4
  \param[in]  pszIfName: 接口名
  \param[out]  pstAddr: 获取到的sockaddr_in Ip地址
  \return 是否获取地址成功
  \retval VOS_OK   获取地址成功
  \retval VOS_NOK 获取地址失败
*/
VOS_UINT32 ATP_UTIL_GetIpByName(const VOS_CHAR* pszIfName, struct sockaddr_in* pstAddr);

/*4. 3GUsbStickNode*/
#ifdef SUPPORT_ATP_AT
//!\brief 要求获得数据卡USB 串口接点号类型
/*!
 *at 命令接点号，上网数据接点号，语音接点号
 */
typedef enum tagATP_3G_TYPE_REQ_EN
{
    ATP_3G_BEGIN_REQ,
    ATP_3G_PCUIND_REQ,         // 获取at 命令接点号
    ATP_3G_MODEMND_REQ,        // 获得上网数据接点号
    ATP_3G_VOICEND_REQ,      // 获得语音接点号
    ATP_3G_END_REQ,
}ATP_3G_TYPE_REQ_EN;
/*!
  \brief 获得数据卡对应功能的USB 串口接点号
  \param[in]  enReqType: 要求的节点号类型
  \param[in]  pszRetNode:用于存放对应的接点名字
  \return 是否获取成功
  \retval VOS_TRUE  获取成功
  \retval VOS_FALSE 获取失败
*/
VOS_INT32 ATP_Get3GUsbStickNode(ATP_3G_TYPE_REQ_EN enReqType, VOS_CHAR * pszRetNode, VOS_UINT32 ulLen);/* <DTS2013083001384 xiazhongyue 20130830 modify> */
#endif

/*5. Common*/
/*!
  \brief 把字符串中的'\0'去掉
  \param[in]  *pcBuf: 待去除'\0'的字符串
  \return 是否去除成功
  \retval VOS_TRUE  去除成功
  \retval VOS_FALSE 去除失败
*/
VOS_BOOL ATP_UTIL_RmDelimitor( VOS_CHAR *pcBuf);

/*6. Interface Type*/
/*!
  \brief 获取接口类型
  \param[in]  *pcIntf: 待获取接口
  \return 获取到的接口类型
  \retval ulType 获取到的接口类型
  \retval 0 获取接口类型失败
*/
VOS_UINT32 ATP_UTIL_GetIntfType(const VOS_CHAR *pcIntf);

/*7. Qos*/

/* Start of modified by f00120964 for qos function 2012-03-23*/
/* <DTS2013070601078  z00159386  20130706  begin */
#if defined SUPPORT_ATP_APP_CLASS_QOS || defined SUPPORT_ATP_VOICE
/* DTS2013070601078  z00159386  20130706  end> */
/*!
  \brief 给定一个socketfd，设置该sockfd的mark值，根据该MARK值用来做QoS
  \param[in]  ulSockfd: 给定的socketfd
  \param[in]  ulMarkValue: 设置的Mark值
  \return 是否设置成功
  \retval  0  设置成功
  \retval -1 设置失败
*/
VOS_INT32  ATP_UTIL_QosSetMark(const VOS_UINT32 ulSockfd, const VOS_UINT32 ulMarkValue);
#endif
/* End of modified by f00120964 for qos function 2012-03-23*/

/*9. Wan Backup*/
#ifdef SUPPORT_ATP_WAN_BACKUP
/*!
  \brief ATP_UTIL_WanBackUpGetBannerFile未实现
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
  \brief 将slave中的事务一次性提交
  \return 返回提交是否成功
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
  \brief 根据字符串获取对应的ATP_UTIL_CSTRMATCH_PATTERN类型
  \param[in]  str: 待查询字符串
  \return 返回匹配到的类型
  \retva type  找到的可匹配类型
  \retval ATP_UTIL_STRMATCH_EXACT str为空时
  \retva ATP_UTIL_STRMATCH_ERROR 未找到可匹配的类型
*/
ATP_UTIL_CSTRMATCH_PATTERN ATP_UTIL_GetMatchType(const VOS_CHAR * str);

/*14. IPP*/
#ifdef SUPPORT_ATP_IPP
/*!
  \brief 判断打印机是否存在
  \param[in] VOS_VOID: 无
  \return 返回打印机是否存在标志
  \retval VOS_TRUE 打印机存在
  \retval VOS_FALSE 打印机不存在
*/
VOS_BOOL ATP_UTIL_IsPrinterExist(VOS_VOID);
#endif

/*15.Dynamic Launched Application Update*/
/*!
  \brief 更新动态应用程序列表
  \param[in]  *pcAppName: 需更新的应用程序名
  \param[in]  bStop: 是否停止更新
  \return 返回发送消息错误码
  \retval VOS_OK 更新消息发送成功
  \retval VOS_NOK pcAppName为NULL
  \retval ATP_MSG_RET_PARA_ERR  消息参数错误
  \retval ATP_MSG_RET_NOT_INITED 未初始化
  \retval ATP_MSG_RET_OVERFLOW  消息溢出
  \retval ATP_MSG_RET_PEER_DEAD  socket被peer关闭
  \retval ATP_MSG_RET_INTERNAL_ERR  内部错误
*/
VOS_UINT32 ATP_UTIL_UpdateDynAppListenFd(const VOS_CHAR *pcAppName, VOS_BOOL bStop);

/*16.InterfaceInfo Function*/
/*!
  \brief  获取接口pszIfcName的统计信息
  \param[in]  *pszIfcName: 待获取统计信息的接口名
  \param[out]  *pstState: 接口统计信息
  \return 无
  \retval 无
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
  \brief 在字符串str中根据待匹配类型type匹配是否存在*pattern
  \param[in]  str: 待验证字符串
  \param[in]  pattern: 用来匹配的字符串
  \param[in]  type: 待匹配类型
  \return 返回是否匹配成功标志
  \retval VOS_TRUE 匹配成功
  \retval VOS_FALSE 匹配失败
*/
VOS_BOOL ATP_UTIL_CstrMatch(const VOS_CHAR * str, const VOS_CHAR * pattern, ATP_UTIL_CSTRMATCH_PATTERN type);


/*!
  \brief 根据LAN接口名(eth0.4)获取br
  \param[in]  pszLan: 
  \param[in]  pszBrName: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UTIL_GetBrByLanInterface(const VOS_CHAR* pszLan, VOS_CHAR* pszBrName, VOS_UINT32 ulLen);

/*!
  \brief 通过mac 地址获取接口名
  \param[in]  psMacaddr: 待获取接口的MAC 地址
  \param[out]  psInterfaceName: 获取到得接口名
  \param[in]  ulInfNameLen: 接口名长度
  \return 获取接口名是否成功
  \retval VOS_OK  获取接口名成功
  \retval VOS_NOK  获取接口名失败
*/
VOS_UINT32 ATP_UTIL_GetInterfaceNameByMacAddr(const VOS_CHAR * psMacaddr, VOS_CHAR* psInterfaceName, VOS_UINT32 ulInfNameLen);

/*!
  \brief 获取所有bridge的接口名称
  \param[in]  pcNameBuf: 存放接口名的缓冲区
  \param[in]  ulBufLen: 存放接口名的缓冲区长度
  \return 接口个数
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
/* DTS2013070108444 p00134535 2013-7-12 静态IPoE拨号var/wan/nas1下信息不全 begin */
VOS_INT32 ATP_UTIL_Write_To_File(const char* path, const char* content);
/* DTS2013070108444 p00134535 2013-7-12 end */
#if defined(__cplusplus)
}
#endif

#endif

