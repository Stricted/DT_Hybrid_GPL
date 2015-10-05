/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z81004143    2013/05/03    DTS2013050301791    [TR069] B890-66 V100R001C994B122(ROUTER): 新增TR069功能
*******************************************************************************/

#ifndef __WEB_MAIN_H__
#define __WEB_MAIN_H__

/* <DTS2012060604837 j81004134 2012/6/4 begin */
#include "atplog.h"
/* DTS2012060604837 j81004134 2012/6/4 end> */
#define ATP_WEB_TRY_LOGIN_TIMES     (3)
#define ATP_WEB_LOGIN_FAIL_TIMES    (3)
#define ATP_WEB_LOGIN_INTERVAL      (1 * 60)    // 1 minute
#define ATP_WEB_SHORT_DELAY     (500)       // 500 ms, half of a second

/*
* For CGI PROC
*/
#define WEB_REPLACE_PAGE_PATH   "/html/relocation.asp"

#ifdef SUPPORT_ATP_BT_HTML
#define WEB_LOGIN_PAGE_PATH         "/html/common/advanced_login.html"
#define WEB_ERR_PAGE_PATH           "/html/msgerrcode.asp"
#define WEB_HOME_PAGE_PATH          "/html/home/home.html"
#define WEB_NULL_PAGE_PATH          "/html/null.html"
#define WEB_SETTING_PAGE_PATH       "/html/settings/b_basic_settings.html"
#define WEB_PUBINFO_PAGE_PATH       "/html/settings/a_pubinfo.html"
#define WEB_STATUS_API_PATH         "/var/status.api"
#define WEB_FIRSTLOGIN_PAGE_PATH    "/html/home/a_fistlogin_configuration.html"
#elif SUPPORT_ATP_W723V_HTML
#define WEB_REPLACE_PAGE_PATH   "/auth/relocation.php"   /*重定向页面*/
#define WEB_LOGIN_PAGE_PATH     "/"
#define WEB_BEENDEN_PAGE_PATH   "/pub/top_beenden.php"   /*Speedport欢送页面 kf19971 2009-11-19*/
#define WEB_ERR_PAGE_PATH       "/pub/msgerrcode.php"
#define WEB_PUBINFO_PAGE_PATH   "/pub/hcti_pubinfo.php"
#define WEB_UPDIMG_PAGE_PATH    "/pub/hcti_laden_firmupd.php" 
#define WEB_UPDERR_PAGE_PATH    "/auth/hcti_laden_firm.php"
#else
#define WEB_LOGIN_PAGE_PATH         "/"
#define WEB_ERR_PAGE_PATH           "/html/msgerrcode.asp"
#define WEB_PUBINFO_PAGE_PATH       "/html/pubinfo.asp"
#define WEB_DOWNPARTCFG_PAGE_PATH   "/var/partcfg.bin"
#endif

#define WEB_DOWNCFG_PAGE_PATH       "/var/curcfg.bin"

#define WEB_TEMPSTRING_LENGTH       (128)
#define WEB_PATH_LENGTH             (256)
#define WEB_INSTANCEID_RESPOND      (64)
#define WEB_PARA_VALUE_LEN          (512)
#define WEB_SCANN_USBDIR_MAXNUM     (5000) // 00163811 add for dlna media server

extern VOS_CHAR    g_szReplacePage[WEB_PATH_LENGTH];
extern VOS_UINT32  g_enMsgRetCode;                                  /*配置消息的返回码*/
extern VOS_UINT32  g_enPubInfoIdx;
extern VOS_BOOL    g_bUploadingImg;
extern VOS_BOOL    g_bRebooting;

extern ATP_WEB_SERVER_ST *g_pstWebServer;

#ifdef SUPPORT_DESKTOP
#define ATP_WEB_CFGFILE_PATH        "../etc/cfgtmp.cfg"
#define ATP_WEB_PARTCFGFILE_PATH        "../etc/partcfgtmp.pcfg"
#else
#define ATP_WEB_CFGFILE_PATH        "/var/cfgtmp.cfg"
#define ATP_WEB_PARTCFGFILE_PATH        "/var/partcfgtmp.pcfg"
#endif

typedef enum tagATP_WEB_ACCOUNT_LEVEL_EN
{
    ATP_WEB_ACCOUNT_USER        = (1<<0),    // Lowest authority
    ATP_WEB_ACCOUNT_ADMIN       = (1<<1),
} ATP_WEB_ACCOUNT_LEVEL_EN;

#define ATP_WEB_ACCOUNT_NONE    0   // No authorization needed
#define ATP_WEB_ACCOUNT_ALL     (ATP_WEB_ACCOUNT_USER|ATP_WEB_ACCOUNT_ADMIN)

typedef enum tagATP_WEB_EXT_COOKIE_EN
{
    ATP_WEB_EXT_COOKIE_FIRST_MENU = ATP_WEB_COOKIE_ITEM_EXTEND,
    ATP_WEB_EXT_COOKIE_SECOND_MENU,
    ATP_WEB_EXT_COOKIE_THIRD_MENU
} ATP_WEB_EXT_COOKIE_EN;

typedef enum tagATP_WEB_PUB_INFO_IDX_EN
{
    ATP_WEB_PUB_INFO_REBOOTING,
    ATP_WEB_PUB_INFO_UPGRADING
} ATP_WEB_PUB_INFO_IDX_EN;

enum tagWEB_EXT_HEADER_EN
{
    WEB_EXT_HEADER_CONTENT_DISPOSITION = ATP_HTTP_HEADER_EXTEND,
    WEB_EXT_HEADER_PRAGMA,
#if defined(SUPPORT_ATP_WEB_COMPRESS) && defined(SUPPORT_ATP_WEB_SEPARATE)
    WEB_EXT_HEADER_CONTENT_ENCODING,
#endif
};

#ifdef SUPPORT_ATP_NAT_HOST_PORT_MAP
typedef struct tagATP_WEB_PORT_ST
{
    VOS_UINT32  ulStartPort;
    VOS_UINT32  ulEndPort;
    VOS_UINT32  ulEnabled;
} ATP_WEB_PORT_ST;

typedef struct tagWEB_WAN_PORTMAP_ST
{
    const VOS_CHAR   *pcDescription;
    const VOS_CHAR   *pcClientMac;
    const VOS_CHAR   *pcProtocol;
    VOS_BOOL         bEnable;
    VOS_UINT32       ulExternalPort;
    VOS_UINT32       ulExternalPortEngRange;
    VOS_UINT32       ulInternalPort;
    VOS_UINT32       ulLocatiobVal;
} WEB_WAN_PORTMAP_ST;

#define ENABLE_TO_ENABLE                    (0)
#define DISABLE_TO_DISABLE                  (1)
#define ENABLE_TO_DISABLE                   (2)
#define DISABLE_TO_ENABLE                   (3)

#define ATP_ACTUAL_LEN_16                   16
#endif
VOS_VOID ATP_WEB_AspInit(VOS_VOID);

VOS_VOID ATP_WEB_CgiInit(VOS_VOID);

VOS_VOID webReboot(VOS_VOID *pvHook);

#ifdef SUPPORT_ATP_ATP_FMCWEB
//b890,modified by wuxiulin
/* <DTS2013022508190 j81004134 2013/2/27 begin */
#ifndef ATP_PCLINT
#ifdef ATP_DEBUG
#define ATP_WEB_SERVER_DEBUG(format, args...)  {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_WEB_SERVER_DEBUG(format, args...)  logprintf_info(format, ##args)
#endif
#else
#define ATP_WEB_SERVER_DEBUG(format, args...)
#endif
/* DTS2013022508190 j81004134 2013/2/27 end> */
/* <DTS2012072501430 w00189185 2012/8/3 begin  */
typedef enum tagWEB_XML_HANRESULT_EN
{
    
    WEB_XML_OK =                    0,
    WEB_XML_ERROR =                 1,
    WEB_XML_LOGIN_USER_NOTEXIT =    108001,
    WEB_XML_LOGIN_PASSWORD_ERR =    108002,
    WEB_XML_LOGIN_MUTIL_USER =      108003,    
    WEB_XML_PASSWORD_MODIFY_ERR =   108004,
    
    ATP_WEB_XML_NOT_SENDBUF =       9000
    
} WEB_XML_HANRESULT_EN;
/* DTS2012072501430 w00189185 2012/8/3 end> */

VOS_VOID webRestore(VOS_VOID *pvHook);
VOS_UINT32 WebGetMethodProc(ATP_HTTP_CLIENT_ST *pstHttpClient,const VOS_CHAR *pcUri);

//login logout相关函数，定义在weblogin.c中
/* <DTS2013040206195 z81004485 2013/4/2 PC_LINT begin */
/* <DTS2012072501430 w00189185 2012/8/3 begin  */

/* DTS2012072501430 w00189185 2012/8/3 end> */


//weblog 相关函数，定义在Weblog.c中
VOS_UINT32 webxmlLogInfo(const ATP_HTTP_CLIENT_ST *pstHttpClient, const char *pcUri,VOS_CHAR *buf);
/* <DTS2012080603261 z00189398 2012-08-07 begin */
//usbscan.c 获取U盘指定目录下的文件及类型
VOS_UINT32 ATP_WEB_ScanDir_EX(ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcDir, VOS_UINT32 ulFindType);
/* DTS2012080603261 z00189398 2012-08-07 end> */
//定义在webupdownloadcgi.c中，上传配置文件
VOS_UINT32 webxmldownloadcfg(ATP_HTTP_CLIENT_ST *pstHttpClient);
/* <DTS2013050301791 z81004143 2013/3/8 begin */
VOS_UINT32 webxmlUploadCwmpCert(const ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
VOS_UINT32 webxmlgetUploadCwmpCertRst(const ATP_HTTP_CLIENT_ST *pstHttpClient, const VOS_CHAR *pcUri, VOS_CHAR *buf);     /* DTS2013061803625 z81004485 Coverity 2013/6/18 */
/* DTS2013050301791 z81004143 2013/3/8 end> */
//定义在upgweb.c中
/* DTS2013040206195 z81004485 2013/4/2 PC_LINT end> */
//u盘自动扫描标志，定义在dmswebproc.c文件中
VOS_UINT32 Web_XML_getUTF8AutoDetectValue();

VOS_UINT32 Web_XML_setUTF8AutoDetectValue(VOS_CHAR *cWriteValue);
/* <DTS2013032808246 z81004485 2013/3/28 begin  */
VOS_UINT32 webIsLogin();
/* DTS2013032808246 z81004485 2013/3/28 end> */
#endif

#endif
