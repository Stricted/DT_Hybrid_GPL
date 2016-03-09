#ifndef __WEB_MAIN_H__
#define __WEB_MAIN_H__

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
#endif

#ifdef SUPPORT_ATP_W723V_HTML
#define WEB_REPLACE_PAGE_PATH   "/auth/relocation.php"   /*重定向页面*/
#define WEB_LOGIN_PAGE_PATH     "/"
#define WEB_BEENDEN_PAGE_PATH   "/pub/top_beenden.php"   /*Speedport欢送页面 kf19971 2009-11-19*/
#define WEB_ERR_PAGE_PATH       "/pub/msgerrcode.php"
#define WEB_PUBINFO_PAGE_PATH   "/pub/hcti_pubinfo.php"
#define WEB_UPDIMG_PAGE_PATH    "/pub/hcti_laden_firmupd.php" 
#define WEB_UPDERR_PAGE_PATH    "/auth/hcti_laden_firm.php"
#endif

#ifdef SUPPORT_ATP_HG532T_DISABLE_HTML
#define WEB_LOGIN_PAGE_PATH         "/html/main.html"
#define WEB_ERR_PAGE_PATH           "/html/msgerrcode.asp"
#define WEB_PUBINFO_PAGE_PATH       "/html/pubinfo.asp"
#define WEB_DOWNPARTCFG_PAGE_PATH   "/var/partcfg.bin"
#else
#define WEB_LOGIN_PAGE_PATH         "/"
#define WEB_ERR_PAGE_PATH           "/html/msgerrcode.asp"
#define WEB_PUBINFO_PAGE_PATH       "/html/pubinfo.asp"
#define WEB_DOWNPARTCFG_PAGE_PATH   "/var/partcfg.bin"
#endif

#define WEB_IPCAPTURE_ATM           "nas_1_32"
#define WEB_IPCAPTURE_PTM           "nas_p1"
#ifdef SUPPORT_ATP_HYBRID
#define WEB_IPCAPTURE_ETHWAN        "nas1"
#else
#define WEB_IPCAPTURE_ETHWAN        "nas0"
#endif

#define WEB_DOWNCFG_PAGE_PATH       "/var/curcfg.bin"

#define WEB_TEMPSTRING_LENGTH       (128)
#define WEB_PATH_LENGTH             (256)
#define WEB_INSTANCEID_RESPOND      (64)
#define WEB_PARA_VALUE_LEN          (512)
#define WEB_SCANN_USBDIR_MAXNUM     (5000) // 00163811 add for dlna media server

#ifdef SUPPORT_ATP_MANUALUPG
typedef enum tagATP_WEB_STATECODE_EN
{
    WEB_REMOTEUPG_DOWNLOAD_VERSION= 0,                 //开始升级，检查网络设置。
    WEB_REMOTEUPG_DOWNLOAD_VERSION_FILE_FAILED,        //获取版本信息失败,网络异常，没有下载成功。
    WEB_REMOTEUPG_DOWNLOAD_VERSION_FILE_SUCCESS,       //下载版本信息文件成功，开始校验
    WEB_REMOTEUPG_VALIDATE_FAILED,                     //版本信息文件校验失败
    WEB_REMOTEUPG_SAME_VERSION_FAILED,                 //版本信息相同
    WEB_REMOTEUPG_DOWNLOAD_IMGAE_START,                //开始下载镜像文件
    WEB_REMOTEUPG_DOWNLOAD_IMGAE_FAILED,               //网络异常，镜像文件下载失败
    WEB_REMOTEUPG_DOWNLOAD_IMGAE_INVALID,              //镜像文件不合法，校验失败
    WEB_REMOTEUPG_IMGAE_SAVE,                          //镜像文件下载成功，正在保存到錐lash
    WEB_REMOTEUPG_IMGAE_REBOOT                         //正在重新启动
    
}ATP_WEB_STATECODE;
#endif

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

typedef enum{
    IPCAP_NONE,
    IPCAP_DSL = 1,
    IPCAP_ETHUP,
    IPCAP_WLAN5G,
    IPCAP_WLAN24G,
    IPCAP_LTE,
    IPCAP_LAN1,
    IPCAP_LAN2,
    IPCAP_LAN3,
    IPCAP_LAN4
}IPCAP_Type_EN;

typedef struct tagATP_WEB_CAPTURE_ST
{
    VOS_BOOL                    bFakeHeadFlagPath;
    ATP_SOCK_BASE_ST            *pstSockBasePath;
    ATP_HTTP_CLIENT_ST          *pstHttpClient;
} ATP_WEB_CAPTURE_ST;

typedef struct tagATP_CAPTURE_DELAY_ST
{
    ATP_UTIL_TIMER_ST * pstDelayTimer;
    IPCAP_Type_EN           ulIpCapType;
    ATP_WEB_CAPTURE_ST          *pstCapture;
} ATP_CAPTURE_DELAY_ST;

typedef enum tagATP_WEB_ACCOUNT_LEVEL_EN
{
    ATP_WEB_ACCOUNT_USER        = (1<<0),    // Lowest authority
    ATP_WEB_ACCOUNT_ADMIN       = (1<<1),
} ATP_WEB_ACCOUNT_LEVEL_EN;

#define ATP_WEB_ACCOUNT_NONE    0   // No authorization needed
#define ATP_WEB_ACCOUNT_ALL     ((VOS_UINT32)ATP_WEB_ACCOUNT_USER|(VOS_UINT32)ATP_WEB_ACCOUNT_ADMIN)

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

#endif

