/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file wlancmsapi.h
  *wlancms api \n
  *WLAN参数，结构体和外部函数定义
  *\copyright 2007-2020，华为技术有限公司
  *\author l00193021 & z001758982
  *\date  2011-12-22
*/

#ifndef _WLAN_CMS_API_H
#define _WLAN_CMS_API_H

#ifdef __cplusplus
extern "C" {
#endif

/******** Include Files ***********/
#include <stdio.h>
#include <netinet/in.h>
#include "msgapi.h"
#include "cfmapi.h"
#include "atpconfig.h"
#include "atptypes.h"
#include "atplog.h"


#if ATP_DEBUG
#define DEBUGPC(str, args...)        \
do{                              \
    printf("[%s %s:%d]  ", __FILE__, __FUNCTION__, __LINE__); \
    printf("\033[1;33;40m");   \
    printf(str, ## args);      \
    printf("\033[0;37;40m\n"); \
}while(0) 
#else
#define DEBUGPC(str, args...)
#endif

#ifdef ATP_PCLINT 
#define ERR_DEBUG(str, args...)
#else
//错误信息打印
#define ERR_DEBUG(str, args...)        \
do{                              \
    printf("[%s %s:%d]  ", __FILE__, __FUNCTION__, __LINE__); \
    printf("\033[1;33;40m");   \
    printf(str, ## args);      \
    printf("\033[0;37;40m\n"); \
}while(0) 
#endif



/* 错误码定义 */
#define WLAN_MOD_CMS    0

/* WLAN公共配置参数掩码 */
#define WLAN_MASK_MODE              0x000000001     /*工作模式*/
#define WLAN_MASK_CHANNEL           0x000000002     /*信道*/
#define WLAN_MASK_COUNTRY           0x000000004     /*国家码*/
#define WLAN_MASK_RATE              0x000000008     /*速率*/
#define WLAN_MASK_POWER_VALUE       0x000000010     /*发射功率值或者等级*/
#define WLAN_MASK_MACFLT_CTL        0x000000020     /*MAC地址过滤开关*/
#define WLAN_MASK_MACFLT_POLICY     0x000000040     /*MAC地址过滤黑白名单*/
#define WLAN_MASK_AUTO_CHANNEL      0x000000080     /*Auto 信道是否开启*/
#define WLAN_MASK_WMM               0x000000100     /*WMM是否开启*/
#define WLAN_MASK_BW_VALUE          0x000000200     /*11NBW*/
#define WLAN_MASK_MCS_VALUE         0x000000400     /*11NMcs*/
#define WLAN_MASK_GI_VALUE          0x000000800     /*11NGI*/

#define WLAN_MASK_BAND              0x000001000     /*Band*/
#define WLAN_MASK_EXT_CHANNEL       0x000002000     /*扩展信道位置*/
#define WLAN_MASK_RTSTHRESHOLD      0x000004000     /*RTSThreshold*/
#define WLAN_MASK_FRAGTHRESHOLD     0x000008000     /*FragThreshold*/
#define WLAN_MASK_DTIMPERIOD        0x000010000     /*DtimPerod*/
#define WLAN_MASK_BEACONPERIOD      0x000020000     /*BeaconPeriod*/
#define WLAN_MASK_11H_EN            0x000040000     /*Band*/

#define WLAN_RTS_MAX_VALUE          2347     /*RTSThreshold 最大值*/
#define WLAN_RTS_MIN_VALUE          1        /*RTSThreshold 最小值*/
#define WLAN_FRAG_MAX_VALUE         2346     /*FragThreshold 最大值*/
#define WLAN_FRAG_MIN_VALUE         256      /*FragThreshold 最小值*/
#define WLAN_DTIMPERIOD_MAX_VALUE   255      /*DtimPerod 最大值*/
#define WLAN_DTIMPERIOD_MIN_VALUE   1        /*DtimPerod 最小值*/
#define WLAN_BEACONPERIOD_MAX_VALUE 1024     /*BeaconPeriod 最大值*/
#define WLAN_BEACONPERIOD_MIN_VALUE 20       /*BeaconPeriod 最小值*/

#define WLAN_OVERTURN_STATUS         0xff
#define WLAN_DISABLE        0
#define WLAN_ENABLE         1
#define WLAN_DISABLE_STR    "disable"
#define WLAN_ENABLE_STR     "enable"

#define WLAN_WPS_VERSION    1
#define WLAN_RES_FILE       "/var/wlan_res_lst"
#define WLAN_FON_ASSOCDEV_FILE  "/var/fon/wlan_assoc_dev"

#define WLAN_2_LEN                  2
#define WLAN_4_LEN                  4
#define WLAN_8_LEN                  8
#define WLAN_12_LEN                 12
#define WLAN_16_LEN                 16
#define WLAN_32_LEN                 32
#define WLAN_64_LEN                 64
#define WLAN_63_LEN                 63
#define WLAN_132_LEN                160
#define WLAN_256_LEN                256
#define WLAN_1024_LEN               1024

#define WLAN_STR_4_LEN              5
#define WLAN_STR_8_LEN              9
#define WLAN_STR_16_LEN             17
#define WLAN_STR_32_LEN             33
#define WLAN_STR_64_LEN             65
#define WLAN_STR_128_LEN            129

#define WLAN_SYS_LEN                256

#define  WLAN_TR181_RADIO_2G_IND_ID     1
#define  WLAN_TR181_RADIO_5G_IND_ID     2
#if defined(SUPPORT_ATP_WLAN_CONCURRENCE)
#define WLAN_SSID_TOTAL_NUM         (WLAN_2G_SSID_NUM + WLAN_5G_SSID_NUM) /*所有SSID的最大数*/
#else
#define WLAN_SSID_TOTAL_NUM         (WLAN_2G_SSID_NUM > WLAN_5G_SSID_NUM ? WLAN_2G_SSID_NUM : WLAN_5G_SSID_NUM)
#endif
#define WLAN_ASSOCIATE_MIN_NUM      1
#define WLAN_WEPKEY_NUM             4       /*WEP-KEY个数*/
#define WLAN_PSK_NUM                1       /*WPA-PSK个数*/
#define WLAN_WMM_NUM                4       /*WMM流个数*/
#define WLAN_RULE_NUM               7       /*一星期的天数*/
#define WPS_REGISTRAR_NUM           32      /*WPS注册者个数*/

#define WLAN_SSID_MAX_LEN           32      /*SSID最大长度*/
#define WLAN_WEPKEY_MAX_LEN         128     /*WEP-key最大长度*/
#define WLAN_PSK_MAX_LEN            64      /*PreSharedKey最大长度*/
#define WLAN_MAC_MAX_LEN            32      /*MAC地址最大长度*/
#define WLAN_MAC_FLT_LEN            17      /*MAC地址长度*/

#define WLAN_MACCTL_ENABLE          1       /*MAC地址过滤使能*/
#define WLAN_MACCTL_DISABLE         0       /*MAC地址过滤去使能*/
#define WLAN_POLICY_WHITE           1       /*MAC地址过滤白名单*/
#define WLAN_POLICY_BLACK           0       /*MAC地址过滤黑名单*/
#define WLAN_POLICY_WHITE_TR181     1       /*满足TR181要求 MAC地址过滤白名单*/
#define WLAN_POLICY_BLACK_TR181     2       /*满足TR181要求MAC地址过滤黑名单*/

/* menuconfig已经定义的变量 */
//#define WLAN_INFNAME_HEAD           "wl"
//#define WLAN_RADIO_NUM              2
//#define WLAN_2G_SSID_NUM            4
//#define WLAN_5G_SSID_NUM            4
//#define WLAN_WDS_NUM                4     /*WDS流个数*/
//#define WLAN_ASSOCIATE_NUM          32    /*关联设备个数*/
//#define WLAN_MACFILTER_NUM          32    /*支持MAC地址过滤最大个数*/
//#define WLAN_TXRXNUM_2G             0     //1/2/3/4/7/8/15
//#define WLAN_TXRXNUM_5G             0     //1/2/3/4/7/8/15


/* WLAN接口名定义是因BRCM驱动而固定死(请勿改)*/
#if defined(SUPPORT_ATP_WLAN_INF_REVERSE)
#define ATP_WL0                     (WLAN_INFNAME_HEAD"1") /*当2.4G或者5G或者2.4G/5G时2.4G的第一个SSID接口名*/
#define ATP_WL1                     (WLAN_INFNAME_HEAD"0") /*当2.4G&5G(同时打开)时5G的第一个SSID接口名*/
#else
#define ATP_WL0                     (WLAN_INFNAME_HEAD"0") /*当2.4G或者5G或者2.4G/5G时2.4G的第一个SSID接口名*/
#define ATP_WL1                     (WLAN_INFNAME_HEAD"1") /*当2.4G&5G(同时打开)时5G的第一个SSID接口名*/
#endif

/* Secure Type */
#define WLAN_BEACONTYPE_NONE            "None"
#define WLAN_BEACONTYPE_BASIC           "Basic"
#define WLAN_BEACONTYPE_WPA             "WPA"
#define WLAN_BEACONTYPE_11I             "11i"
#define WLAN_BEACONTYPE_MIXED           "WPAand11i"
#define WLAN_BEACONTYPE_WPAWPA2         "WPAWPA2"

/* Authentication Mode */
#define WLAN_AUTH_NONE                  "None"  /*open*/
#define WLAN_AUTH_SHARED                "SharedAuthentication"
#define WLAN_AUTH_BOTH                  "BothAuthentication"
#define WLAN_AUTH_PSK                   "PSKAuthentication"
#define WLAN_AUTH_EAP                   "EAPAuthentication" /*基于802.1x的EAP认证*/

/* Encryption Mode*/
#define WLAN_ENCRY_NONE                 "None"
#define WLAN_ENCRY_WEP                  "WEPEncryption"
#define WLAN_ENCRY_TKIP                 "TKIPEncryption"
#define WLAN_ENCRY_AES                  "AESEncryption"
#define WLAN_ENCRY_TKIP_AES             "TKIPandAESEncryption"

/*TR181 Security Mode*/
#define WLAN_SECURITY_NONE              "None"
#define WLAN_SECURITY_WEP64             "WEP-64"
#define WLAN_SECURITY_WEP128            "WEP-128"
#define WLAN_SECURITY_WPAPERSONAL       "WPA-Personal"
#define WLAN_SECURITY_WPA2PERSONAL      "WPA2-Personal"
#define WLAN_SECURITY_WPAWPA2PERSONAL   "WPA-WPA2-Personal"	
#define WLAN_SECURITY_WPAENTERPRISE     "WPA-Enterprise"
#define WLAN_SECURITY_WPA2ENTERPRISE    "WPA2-Enterprise"
#define WLAN_SECURITY_WPAWPA2ENTERPRISE "WPA-WPA2-Enterprise"

/* AIFSN */
#define WLAN_AIFSN_MIN                  1   /* 最小的AIFSN */
#define WLAN_AIFSN_MAX                  15  /* 最大的AIFSN */

/* ECW */
#define WLAN_ECW_MIN                    0   /* 最小的ECW */
#define WLAN_ECW_MAX                    15  /* 最大的ECW */

/* TXOP */
#define WLAN_TXOP_MIN                   0   /* 最小的TXOP */
#define WLAN_TXOP_MAX                   255 /* 最大的TXOP */

/* Weekday */
#define WLAN_WEEKDAY_MIN                0   /* 最小的WEEKDAY */
#define WLAN_WEEKDAY_MAX                6   /* 最大的WEEKDAY */
#define MAX_TIME_HOUR                   23
#define MAX_TIME_MINUTE                 59

/* 发射功率 */
#define WLAN_POWER_VALUE_MIN            0
#define WLAN_POWER_VALUE_MAX            100 /*单位dBm，依赖于硬件*/

#define WLAN_POWER_LEVEL_1              1   /*最大功率*/
#define WLAN_POWER_LEVEL_2              2
#define WLAN_POWER_LEVEL_3              3
#define WLAN_POWER_LEVEL_4              4
#define WLAN_POWER_LEVEL_5              5   /*最小功率*/

#define WLAN_MCS_VALUE_MIN              0
#define WLAN_MCS_VALUE_1X1              7
#define WLAN_MCS_VALUE_2X2              15
#define WLAN_MCS_VALUE_3X3              23
#define WLAN_MCS_VALUE_4X4              31
#define WLAN_MCS_VALUE_MAX              31
#define WLAN_MCS_VALUE_AUTO             33

/* 信道 */
#define WLAN_CHANNEL_AUTO               0   /* 自动选择信道 */
#define WLAN_CHANNEL_MIN                1   /* 最小的信道   */
#define WLAN_CHANNEL_MAX                14  /* 最大的信道   */

/*物理模式*/
#define WLAN_PHYTYPE_TYPE_A             "a" 
#define WLAN_PHYTYPE_TYPE_B             "b"
#define WLAN_PHYTYPE_TYPE_G             "g"     
#define WLAN_PHYTYPE_TYPE_N             "n"  
#define WLAN_PHYTYPE_TYPE_AC            "ac"  

/* 波段 */
#define WLAN_BAND_A                     5
#define WLAN_BAND_B                     2
#define WLAN_BAND_2G                    "2.4GHz"
#define WLAN_BAND_5G                    "5GHz"
#define WLAN_IDX_2G                     0
#define WLAN_IDX_5G                     1

/*扩展信道*/
#define WLAN_EXT_CHANNEL_ABOVE          "Above"
#define WLAN_EXT_CHANNEL_BELOW          "Below"

/* 工作模式 */
#define WLAN_STANDARD_TYPE_A            "a"
#define WLAN_STANDARD_TYPE_B            "b"
#define WLAN_STANDARD_TYPE_G            "g"                
#define WLAN_STANDARD_TYPE_B_G          "b/g"
#define WLAN_STANDARD_TYPE_G_N          "g/n"
#define WLAN_STANDARD_TYPE_B_G_N        "b/g/n"  
#define WLAN_STANDARD_TYPE_N            "n"
#define WLAN_STANDARD_TYPE_A_N          "a/n"
#define WLAN_STANDARD_TYPE_AC           "ac"
#define WLAN_STANDARD_TYPE_AC_N         "ac/n"

/* n模式的带宽*/
#define WLAN_BINDWIDTH_TYPE_20          20
#define WLAN_BINDWIDTH_TYPE_40          40
#define WLAN_BINDWIDTH_TYPE_80          80
#define WLAN_BINDWIDTH_TYPE_160         160

/* WEP加密等级 */
#define WLAN_WEP_ENCRYLEVEL_64          "40-bit"    /* 64位密钥，除去固定24位 */
#define WLAN_WEP_ENCRYLEVEL_128         "104-bit"   /* 128位密钥，除去固定24位 */

#define WLAN_WEPKEY_LEN_ASCII_64        5   /*64位密钥，密钥为字符串的长度*/
#define WLAN_WEPKEY_LEN_HEX_64          10  /*64位密钥，密钥为十六进制的长度*/
#define WLAN_WEPKEY_LEN_ASCII_128       13  /*128位密钥，密钥为字符串的长度*/    
#define WLAN_WEPKEY_LEN_HEX_128         26  /*128位密钥，密钥为十六进制的长度*/

/* WPS */
#define WLAN_WPSCTL_ENABLE              1
#define WLAN_WPSCTL_DISABLE             0
#define WLAN_WPSMODE_AP_PIN             "ap-pin"
#define WLAN_WPSMODE_CLIENT_PIN         "client-pin"
#define WLAN_WPSMODE_PBC                "pbc"
#define WLAN_WPSMODE_PIN_TR181          "PIN"
#define WLAN_WPSMODE_PBC_TR181          "PushButton"

/* ADD by y00181549 for DTS2014011101909 at 2014/1/24 */
#define BCM_UNSUPPORT_CLIENT_PIN        "00000000"

/* Schedule Rule */
#define WeekNoChangeMask                7
#define DayHourMaxMask                  24
#define ZERO_STRUCT(x)                  memset((char *)&(x), 0, sizeof(x))

#define WLAN_GI_LONG_TR098				"long"
#define WLAN_GI_SHORT_TR098				"short"
#define	WLAN_GI_LONG_TR181				"800nsec"
#define	WLAN_GI_SHORT_TR181				"400nsec"
#define	WLAN_GI_AUTO_TR181				"Auto"

#define	WLAN_STATUS_LOWERLAYER_DOWN	"LowerLayerDown"
#define WLAN_STATUS_UP                  "Up"
#define WLAN_STATUS_DOWN                "Down"

#define WLAN_IFC_STATUS_UP              "up"
#define WLAN_IFC_STATUS_DOWN            "down"

/* START MODIFY by y00181549 For DTS2013101607911 at 2013/10/16 */
#define WLAN_LOG_WPS_PUSHBOTTON       "PushButton"
#define WLAN_LOG_WPS_CLIENTPIN        "PIN Methode vom Client gesteuert"
/* END MODIFY by y00181549 For DTS2013101607911 at 2013/10/16 */

#if defined(SUPPORT_ATP_WLAN_DT)
#define WLAN_TRANSMITPOWER_30           30
#define WLAN_TRANSMITPOWER_60           60
#define WLAN_TRANSMITPOWER_100          100

/* Start of add by l00164468 2012/10/24 for DTS2012102101106 */
#define WLAN_INIT_TRUE          1
#define WLAN_INIT_FALSE         0
/* End of add by l00164468 2012/10/24 for DTS2012102101106 */

#define WLAN_BSS_ASSOC_MAX_NUM  32
#define WLAN_BSS_ASSOC_MIN_NUM  24
#endif

/* values for band specific 40MHz capabilities (deprecated) */
#define WLAN_BW_20ALL			0
#define WLAN_BW_40ALL			1
#define WLAN_BW_20IN2G_40IN5G   2

#define WLAN_BW_20MHZ_BIT		(1<<0)
#define WLAN_BW_40MHZ_BIT		(1<<1)
#define WLAN_BW_80MHZ_BIT		(1<<2)

/* Bandwidth capabilities */
#define WLAN_BW_CAP_20MHZ		(WLAN_BW_20MHZ_BIT)
#define WLAN_BW_CAP_40MHZ		(WLAN_BW_40MHZ_BIT|WLAN_BW_20MHZ_BIT)
#define WLAN_BW_CAP_80MHZ		(WLAN_BW_80MHZ_BIT|WLAN_BW_40MHZ_BIT|WLAN_BW_20MHZ_BIT)
#define WLAN_BW_CAP_UNRESTRICTED		0xFF

/* Wlan Mode 定义 */
typedef enum
{
	WLAN_MODE_INIT = -1, // INIT mode
    WLAN_AP_MODE = 0, // AP mode
    WLAN_CLIENT_MODE, // Client mode
    WLAN_APCLI_MODE,   // AP-Client mode
    WLAN_APCLI_MULTISTA_MODE  // AP-Client with multi-sta feature mode
}WLAN_MODE_TYPE_EN;

#ifdef SUPPORT_ATP_WLAN_EP
#define WLAN_EP_MAX_PROFILE_NUM         16
#define WLAPCLI_MAX_APLIST              48
#define WLAN_2G_STA_NUM                 1
#define WLAN_5G_STA_NUM                 1
#define WLAN_EP_INTF_NAME               "wl2"
#define WLAN_EP_WPA_SUPPLICANT_PROFILE  "/var/wpa_ep.conf"
#define WLAN_EP_WPA_AP_SCAN             "/var/ap_scan.conf"
#define WLAN_EP_UPLINK_PROFILE 			"/var/ep_uplink_profile"
#define WLAN_EP_PROFILE_HIGH_PRI        1
#define WLAN_EP_PROFILE_DEFAULT_PRI     0

/* AP Network type */
#define WLAPCLI_NT_IN                   "infrastructure"
#define WLAPCLI_NT_AD                   "adhoc"
#define WLAPCLI_AUTH_UNKNOW             "Unknow"
#define WLAPCLI_AUTH_OPEN               "Open"
#define WLAPCLI_AUTH_SHARED             "Shared"
#define WLAPCLI_AUTH_AUTO               "Auto"
#define WLAPCLI_AUTH_WPAPSK             "WPA-PSK"
#define WLAPCLI_AUTH_WPA2PSK            "WPA2-PSK"
#define WLAPCLI_AUTH_WPA1PSKWPA2PSK     "WPA/WPA2-PSK"

#define WLAPCLI_ENCY_UNKNOW             "UNKNOW"
#define WLAPCLI_ENCY_NONE               "NONE"
#define WLAPCLI_ENCY_WEP                "WEP"
#define WLAPCLI_ENCY_TKIP               "TKIP"
#define WLAPCLI_ENCY_AES                "AES"
#define WLAPCLI_ENCY_TKIPAES            "TKIPAES"
#define WLAPCLI_ERROR              "==ERROR=="

#define WLAN_EP_UPLINK_STAT_CONNECTED_STR       "Connected"
#define WLAN_EP_UPLINK_STAT_DISCONNECTED_STR    "Disconnected"

/* Wlan EP Uplink状态定义 */ 
typedef enum
{
    WLAN_EP_UPLINK_STAT_UNKNOW,         // 未知状态
    WLAN_EP_UPLINK_STAT_DISCONNECTED,
    WLAN_EP_UPLINK_STAT_CONNECTED
}WLAN_EP_UPLINK_STAT_EN;

#define WLAN_EP_CONECTTED_THRESHOLD 4
#define WLAN_EP_DISCONECTTED_THRESHOLD 2
#define WLAN_MONITER_ELOOP_SLEEP_INTERVAL 2

#define WLAN_DHCP_UPDATE_THRESHOLD 10

/* Wlan EP WPS状态定义 */ 
typedef enum
{
    WLAN_EP_WPS_STAT_UNKNOW,         // 未知状态
    WLAN_EP_WPS_STAT_NONE,           // 未进行WPS
    WLAN_EP_WPS_STAT_CLI_ACT,       // Client侧在进行WPS
    WLAN_EP_WPS_STAT_SUCCESS        // WPS成功
}WLAN_EP_WPS_STAT_EN;

/*WLAN 状态定义，band和模式*/
typedef struct
{
	VOS_INT32 ulWlanBand;
	VOS_INT32 ulWlanMode; 
}WLAN_STATUS_ST;
#endif
/* WEPKEY INDEX*/
typedef enum
{
    WLAN_WEPKEY_INDEX_1 = 1,
    WLAN_WEPKEY_INDEX_2,
    WLAN_WEPKEY_INDEX_3,
    WLAN_WEPKEY_INDEX_4
}WLAN_WEPKEY_INDEX_EN;

/* 生效 */
typedef enum 
{
    WLAN2G_SETUP = 1,
    WLAN2G_SETUP_ALL = WLAN2G_SETUP,
    WLAN2G_SETUP_BASIC,
    WLAN2G_SETUP_SECURITY,
    WLAN2G_SETUP_MAC_FILTER,
    WLAN2G_SETUP_WDS,
    WLAN2G_SETUP_ADVANCED,
    WLAN2G_SETUP_SES, /*SUPPORT_SES*/
    WLAN5G_SETUP = 100,
    WLAN5G_SETUP_ALL = WLAN5G_SETUP,
    WLAN5G_SETUP_BASIC,
    WLAN5G_SETUP_SECURITY,
    WLAN5G_SETUP_MAC_FILTER,
    WLAN5G_SETUP_WDS,
    WLAN5G_SETUP_ADVANCED,
    WLAN5G_SETUP_SES, /*SUPPORT_SES*/
    END_SETUP
}WLAN_SETUP_TYPE_EN;

/* 只读属性配置项 */
typedef enum
{
    WLAN_READ_ONLY_VERSION,
    WLAN_READ_ONLY_STATUS,
    WLAN_READ_ONLY_BSSID,
    WLAN_READ_ONLY_CHANNELUSED,
    WLAN_READ_ONLY_TOTALASSOC,
    WLAN_READ_ONLY_ASSOCMAC,
    WLAN_READ_ONLY_FLTNUM,    //add for V100R002
    WLAN_READ_ONLY_HTMCS
}WLAN_READ_ONLY_VAR_EN;

/*与资源相关的通用配置项*/
typedef enum 
{
    WLAN_RES_RADIO,
    WLAN_RES_RADIO_STATS,
    WLAN_RES_SSID,
    WLAN_RES_SSID_STATS,
    WLAN_RES_AP,
    WLAN_RES_AP_SECURITY,
    WLAN_RES_AP_WPS,
    WLAN_RES_AP_ASSOC,
    WLAN_RES_AP_WMM,
    WLAN_RES_EP,
    WLAN_RES_EP_STATS,
    WLAN_RES_EP_SEC,
    WLAN_RES_EP_PROFILE,
    WLAN_RES_EP_PROFILE_SEC,
    WLAN_RES_EP_WPS,
    WLAN_RES_PSK,
    WLAN_RES_WEP,
    WLAN_RES_MACFLT
}WLAN_RES_EN;

/*WLAN接口所属区域*/
typedef enum 
{
    WLAN_ID1_ID4 = 0,
    WLAN_ID5_ID8,
    WLAN_ID1_ID8
}WLAN_ID_AREA_EN;

/*WLAN Client associate priority*/
typedef enum
{
    ATP_WLANCLIENT_ASSOC_NOTIFY_PRIORITY_PWR = 0,
    ATP_WLANCLIENT_ASSOC_NOTIFY_PRIORITY_BRIDGE,
    ATP_WLANCLIENT_ASSOC_NOTIFY_PRIORITY_HOST
}ATP_WLANCLIENT_ASSOC_NOTIFY_PRIORITY;

/*WLAN Client disassociate priority*/
typedef enum
{
    ATP_WLANCLIENT_DISASSOC_NOTIFY_PRIORITY_PWR = 0,
    ATP_WLANCLIENT_DISASSOC_NOTIFY_PRIORITY_BRIDGE,
    ATP_WLANCLIENT_DISASSOC_NOTIFY_PRIORITY_HOST
}ATP_WLANCLIENT_DISASSOC_NOTIFY_PRIORITY;

/*WLAN Client disassociate priority*/
typedef enum
{
    ATP_WLANSTATUS_CHG_NOTIFY_PRIORITY_WAN = 0,
    ATP_WLANSTATUS_CHG_NOTIFY_PRIORITY_LAN,		
	ATP_WLANSTATUS_CHG_NOTIFY_PRIORITY_PROTOCOL,
    ATP_WLANSTATUS_CHG_NOTIFY_PRIORITY_PWR
}ATP_WLANSTATUS_CHG_NOTIFY_PRIORITY;
typedef enum
{
	ATP_WLAN_SECURITY_NONE = 0,
	ATP_WLAN_SECURITY_WEP64,
	ATP_WLAN_SECURITY_WEP128,
	ATP_WLAN_SECURITY_WPAPERSONAL,
	ATP_WLAN_SECURITY_WPA2PERSONAL,
	ATP_WLAN_SECURITY_WPAWPA2PERSONAL,
	ATP_WLAN_SECURITY_WPAENTERPRISE,
	ATP_WLAN_SECURITY_WPA2ENTERPRISE,
	ATP_WLAN_SECURITY_WPAWPA2ENTERPRISE
}ATP_WLAN_SECURITY;


/*WLAN局部参数记录*/
typedef struct 
{
    VOS_UINT32 ulLDevInstId;
    VOS_UINT32 ulWlInstId;
    VOS_INT32  iWlDouBand;  /*波段，0表示2.4GHz，1表示5GHz*/
}WLAN_LOCAL_REC_ST;

/*WLAN参数索引与接口名映射表*/
typedef struct
{
    VOS_INT32  iWlVarIdx;
    VOS_CHAR   acWlIfcName[WLAN_16_LEN];
}WLAN_PARA_MAP_ST;

/*WLAN绑定桥信息*/
#define ATP_BRIDGE_DEVICE_MAX 20
typedef struct
{
    VOS_CHAR   acBrName[WLAN_8_LEN];       
    VOS_UINT32 ulBindDevNum;
    VOS_CHAR   acBindDevName[ATP_BRIDGE_DEVICE_MAX][WLAN_8_LEN];    
}WLAN_BRIDGE_INFO_ST;

/* configuration 对象的参数 */
typedef struct
{
    VOS_INT32   iWlEnable;
    VOS_INT32   iWlHide;
    VOS_CHAR    acWlName[WLAN_16_LEN]; /*接口名wl0,wl0.1,...wl0.7*/
    VOS_CHAR    acWlSsid[WLAN_STR_32_LEN];
    VOS_INT32   iWlPowerValue;
    VOS_INT32   iWlChannel;/*auto=0*/
    VOS_INT32   iWlAutoChannelEnable;     /*auto*/
    VOS_CHAR    acWlRate[WLAN_STR_4_LEN];/*速率."auto","1"...*/
    VOS_CHAR    acWlBeaconType[WLAN_STR_32_LEN];
    VOS_INT32   iWlMacCtlEnbl;
    VOS_CHAR    acWlStandard[WLAN_STR_8_LEN];/*显示, "a", "b", "g", "n", "b/g", "b/g/n", "a/n" */
    VOS_INT32   iWlWepKeyIndex;
    VOS_CHAR    acWlBasicEncryption[WLAN_32_LEN];
    VOS_CHAR    acWlBasicAuth[WLAN_32_LEN];
    VOS_CHAR    acWlWpaEncryption[WLAN_32_LEN];
    VOS_CHAR    acWlWpaAuth[WLAN_32_LEN];
    VOS_CHAR    acWl11iEncryption[WLAN_32_LEN];
    VOS_CHAR    acWl11iAuth[WLAN_32_LEN];
    VOS_CHAR    acWlMixedEncryption[WLAN_32_LEN];
    VOS_CHAR    acWlMixedAuth[WLAN_32_LEN];
    VOS_INT32   iWlIsolate;
    VOS_UINT32  ulWlAssocNum;
    VOS_UINT32  ulWlAssocCtl;
    VOS_UINT32  ulWlMacFltnum;
    VOS_INT32   iWlMacFltPolicy;
    VOS_CHAR    acWlcountry[WLAN_16_LEN];
    VOS_INT32   iWlQosctl;
    VOS_CHAR    acWlExtChannelPos[WLAN_16_LEN]; /*扩展信道位置，0表示"Below"，1表示"Above"*/
    VOS_CHAR    acWlBWControl[WLAN_16_LEN];
    VOS_CHAR    acWlGIControl[WLAN_16_LEN];
    VOS_INT32   iWlHtMcs;
    VOS_INT32   iWlTxRxStream;   
    VOS_INT32   iWlDouBand;  /*波段，0表示2.4GHz，1表示5GHz*/
    VOS_INT32   iRTSThreshold;
    VOS_INT32   iFragThreshold;
    VOS_INT32   iDtimPeriod;
    VOS_INT32   iBeaconPeriod;
    VOS_INT32   iIEEE80211hEnable;
#ifdef SUPPORT_ATP_WDS
    VOS_INT32   iWlWDSEnable;    
    VOS_CHAR    acWlWDSEncryption[WLAN_STR_32_LEN];   
    VOS_CHAR    acWlWDSKey[WLAN_STR_32_LEN];
    VOS_CHAR    acWlWDSMode[WLAN_STR_32_LEN];
    VOS_CHAR    acWlLAZYWds[WLAN_STR_32_LEN];
#endif
#if defined(SUPPORT_ATP_WLAN_DT)
    VOS_CHAR    acWlKeyPassphrase[WLAN_STR_64_LEN];/*keypassphrase*/
    VOS_CHAR    acWlBasicDataRates[WLAN_STR_64_LEN];
    VOS_CHAR    acWlOptDataRates[WLAN_STR_64_LEN];
    VOS_INT32   iWlAutoRateFallBackEnable;
    VOS_INT32   iWlUAPSDEnable;
    VOS_INT32   iWlDependOnScheduleRule;
    VOS_INT32   iWlScheduleRuleMode;
    VOS_INT32   iWlSameStartHour;
    VOS_INT32   iWlSameEndHour;
    VOS_INT32   iWlSameStartMinute;
    VOS_INT32   iWlSameEndMinute;
#endif
	/* START MODIFY by y00181549 for DTS2013102504010 at 2013/10/26*/
    VOS_INT32   ulBeamFormingEnable;
	/* END MODIFY by y00181549 for DTS2013102504010 at 2013/10/26*/
}WLAN_TR069_CONF_PARA_ST;

/*WPS对象的参数*/
typedef struct
{
    VOS_INT32   iWlWpsCtl;
    VOS_CHAR    acWlWpsMode[WLAN_16_LEN];
    VOS_CHAR    acWlWpsPinVal[WLAN_16_LEN];
    VOS_CHAR    acWlAPPinVal[WLAN_16_LEN];
#if defined(SUPPORT_ATP_WLAN_DT)
    VOS_CHAR    acWlConfigMethod[WLAN_32_LEN];
    VOS_INT32   iWlSetupLock;
#endif
}WLAN_TR069_WPS_ST;

/*WEPKey对象的参数*/
typedef struct
{
    VOS_UINT32  ulWlInstId;
    VOS_CHAR    acWlWepKey[WLAN_WEPKEY_MAX_LEN];    
}WLAN_TR069_WEPKEY_ST;

/*MacFilter对象的参数*/
typedef struct
{
    VOS_UINT32 ulWlInstId;//w724v v no use
    VOS_CHAR   acWlSrcMacAddress[WLAN_MAC_FLT_LEN + 3];
    VOS_BOOL   bWlMacFltEnable;   //added by ckf40298 w724v v no use
    VOS_CHAR   acWlDeviceName[WLAN_32_LEN];//w724v v no use
}WLAN_ATP_MACFLT_ST;

/*PSK对象的参数*/
typedef struct
{   
    VOS_UINT32  ulWlInstId;
    VOS_CHAR    acWlPsk[WLAN_PSK_MAX_LEN + 4];
    VOS_CHAR    acWlKeyPassphrase[WLAN_STR_64_LEN];/*keypassphrase*/  
}WLAN_TR069_PSK_ST;

/*Assoc对象*/
typedef struct
{
    VOS_UINT32  ulWlInstId;/*Not used in wifi-client assoc and disassoc notify*/
    VOS_UINT32  iLeaseTime;/*Not used in wifi-client assoc and disassoc notify*/
    VOS_INT32   iWlDeviceAuth;/*Not used in wifi-client assoc and disassoc notify*/
    VOS_CHAR    InfcName[WLAN_8_LEN];/*wlan interface name:wl0 or wl0.x, not in the same level of tr069 with other members*/
    VOS_CHAR    acWlDeviceName[WLAN_32_LEN];/*client name*/
    VOS_CHAR    acAssocList[WLAN_STR_32_LEN];/*client mac*/
    VOS_CHAR    acAssocListIP[WLAN_STR_64_LEN];/*client IP*/
#ifdef SUPPORT_ATP_TR181    
    VOS_UINT32  ulLastDataDownlinkRate;
    VOS_UINT32  ulLastDataUplinkRate;
#endif    
}WLAN_TR069_ASSOC_ST;

#ifdef SUPPORT_ATP_WDS
/*WDS对象*/
typedef struct
{
    VOS_UINT32 ulWlInstId;
    VOS_CHAR   acWlSrcMacAddress[WLAN_MAC_FLT_LEN + 3];
}WLAN_ATP_WDS_ST;
#endif

#if defined(SUPPORT_ATP_WLAN_DT)
/*WPS Registrar对象的参数*/
typedef struct
{    
    VOS_UINT32 ulWlInstId;
    VOS_INT32  iWlEnable;  
    VOS_CHAR   acWlUUIDVal[WLAN_32_LEN];  
    VOS_CHAR   acWlDevName[WLAN_32_LEN];  
}WLAN_TR069_WPS_REGST_ST;

/*WMM对象*/
typedef struct
{
    VOS_UINT32  ulWlInstId;
    VOS_INT32   iWlAIFSN;
    VOS_INT32   iWlECWMin;
    VOS_INT32   iWlECWMax; 
    VOS_INT32   iWlTXOP;  
    VOS_INT32   iWlAckPolicy;
}WLAN_TR069_WMM_ST;

/*SCHDLRULE对象*/
typedef struct
{
    VOS_UINT32 ulWlInstId;
    VOS_INT32  iWlWeekday;
    VOS_INT32  iWlStartHour;
    VOS_INT32  iWlEndHour;
    VOS_INT32  iWlStartMinute;
    VOS_INT32  iWlEndMinute;
}WLAN_ATP_SCHDLRULE_ST;

typedef enum tagATP_WLAN_TMP_STATUS_EN
{
    ATP_WLAN_STATUS_DEF = -1,
	ATP_WLAN_STATUS_DISABLE,
    ATP_WLAN_STATUS_ENABLE    
} ATP_WLAN_TMP_STATUS_EN;
typedef enum tagATP_WLAN_WPS_MODE_EN
{
    ATP_WLAN_WPS_DEF = -1,
    ATP_WLAN_WPS_PBCFLAG,
    ATP_WLAN_WPS_PINFLAG
} ATP_WLAN_WPS_MODE_EN;
#endif

/*WLAN所有参数*/
typedef struct
{
    VOS_UINT32 ulLDevInstId;
    VOS_UINT32 ulWlInstId;
	VOS_INT32  iWlanPskMode;
    WLAN_TR069_CONF_PARA_ST stWlConfVar;
    WLAN_TR069_WPS_ST       stWlWpsVar;
    WLAN_TR069_WEPKEY_ST    astWlWepKey[WLAN_WEPKEY_NUM];
    WLAN_TR069_PSK_ST       astWlPsk[WLAN_PSK_NUM];
    WLAN_TR069_ASSOC_ST     astWlAssoc[WLAN_ASSOCIATE_NUM];
    WLAN_ATP_MACFLT_ST      astWlSrcMacAddress[WLAN_MACFILTER_NUM];
#ifdef SUPPORT_ATP_WDS    
    WLAN_ATP_WDS_ST         astWlWDSMAC[WLAN_WDS_NUM];
#endif
#if defined(SUPPORT_ATP_WLAN_DT)
    WLAN_TR069_WPS_REGST_ST astWlWpsRegstVar[WPS_REGISTRAR_NUM];
    WLAN_TR069_WMM_ST       astWlWMMAP[WLAN_WMM_NUM];
    WLAN_ATP_SCHDLRULE_ST   astWlScheduleRule[WLAN_RULE_NUM];
#endif
}WLAN_TR069_PARA_ST;

/*****************************
WLAN TR181 Radio结构体
*****************************/
typedef struct
{
    VOS_UINT32	ulInstId;
    VOS_INT32   iRadioEnable;
    VOS_INT32   iWlDouBand;//0:2.4G   1:5G
    VOS_INT32   iWlAutoChannelEnable;
    VOS_INT32   iWlChannel;
    VOS_INT32   iWlHtMcs;
    VOS_INT32   iWlPowerValue;
    VOS_INT32   iIEEE80211hEnable;
    VOS_INT32   iRTSThreshold;
    VOS_INT32   iFragThreshold;
    VOS_INT32   iDtimPeriod;
    VOS_INT32   iBeaconPeriod;
    VOS_CHAR    acWlStandard[WLAN_STR_8_LEN]; /*tr181 format, "a", "b", "g", "n", "b,g", "b,g,n", "a,n" */
    VOS_CHAR	acWlBWControl[WLAN_16_LEN];
    VOS_CHAR	acWlGIControl[WLAN_16_LEN];
    VOS_CHAR	acWlcountry[WLAN_16_LEN];
    VOS_CHAR    acWlExtChannelPos[WLAN_32_LEN]; /*扩展信道位置，0表示"Below"，1表示"Above"*/
	/* START MODIFY by y00181549 for DTS2013102504010 at 2013/10/26*/
    VOS_UINT32   ulBeamFormingEnable;
	/* END MODIFY by y00181549 for DTS2013102504010 at 2013/10/26*/
}WLAN_TR181_RADIO_ST;

/*****************************
WLAN TR181 SSID结构体
*****************************/
typedef struct
{
    VOS_UINT32           ulInstId;
    VOS_INT32            iSsidEnable;
    VOS_CHAR             acWlName[WLAN_16_LEN];
    VOS_CHAR             acWlSsid[WLAN_STR_64_LEN];
    WLAN_TR181_RADIO_ST *pstRadio;
}WLAN_TR181_SSID_ST;

/*****************************
WLAN TR181 AP Security结构体
*****************************/
typedef struct
{
    VOS_UINT32	ulModeEnabled;//安全类型ATP_WLAN_SECURITY
    VOS_INT32   iWlanPskMode;
    VOS_CHAR    acWlWepKey[WLAN_WEPKEY_MAX_LEN];   
    VOS_CHAR    acWlPsk[WLAN_PSK_MAX_LEN + 4];/*和DB 中Psk是一至的,存的客户设置的PSK*/
    VOS_CHAR    acWlKeyPassphrase[WLAN_STR_64_LEN];
}WLAN_TR181_AP_SECU_ST;

/*****************************
WLAN TR181 AP WPS结构体
*****************************/
typedef struct 
{
    VOS_INT32   iWlWpsCtl;
    VOS_CHAR    acWlWpsMode[WLAN_16_LEN];
    VOS_CHAR    acWlWpsPinVal[WLAN_16_LEN];
    VOS_CHAR    acWlAPPinVal[WLAN_16_LEN];
#if defined(SUPPORT_ATP_WLAN_DT)
    VOS_CHAR    acWlConfigMethod[WLAN_32_LEN];
    VOS_INT32   iWlSetupLock;
#endif
}WLAN_TR181_AP_WPS_ST;

/*****************************
WLAN TR181 AP结构体
*****************************/
typedef struct
{
    VOS_UINT32  ulInstId;
    VOS_INT32   iApEnable;
    VOS_INT32   iWlHide;
    VOS_INT32   iWlQosctl;
    VOS_INT32   iWlIsolate;
    VOS_INT32   iWlMacFltPolicy;	
    VOS_UINT32  ulWlAssocCtl;
    VOS_UINT32  ulWlAssocNum;
    VOS_UINT32  ulWlMacFltnum;//w724v 没用
#if defined(SUPPORT_ATP_WLAN_DT)
    VOS_INT32   iWlDependOnScheduleRule;
    VOS_INT32   iWlScheduleRuleMode;
    VOS_INT32   iWlUAPSDEnable;
    VOS_INT32   iWlSameStartHour;
    VOS_INT32   iWlSameEndHour;
    VOS_INT32   iWlSameStartMinute;
    VOS_INT32   iWlSameEndMinute;
    VOS_BOOL   bWlStopMode;
    WLAN_ATP_SCHDLRULE_ST astWlScheduleRule[WLAN_RULE_NUM];
    ATP_UTIL_TIMER_ST  *    pstSchTimer;
#endif
    WLAN_TR181_AP_SECU_ST stwlSecurity;
    WLAN_TR181_AP_WPS_ST  stWps;
    WLAN_ATP_MACFLT_ST    astWlSrcMacAddress[WLAN_MACFILTER_NUM];//w724v 没用
    WLAN_TR069_ASSOC_ST   astWlAssoc[WLAN_ASSOCIATE_NUM];
    WLAN_TR181_SSID_ST   *pstSsid;
}WLAN_TR181_AP_ST;

/*统计信息*/
typedef struct
{
    unsigned long ulUnitSent;             
    unsigned long ulUnitReceived;      
    unsigned long ulMultSent;   
    unsigned long ulMultReceived; 
    unsigned long ulBroadSent;            
    unsigned long ulBroadReceived;          
    unsigned long ulUnknowReceived;    
}WLAN_INTERFACE_STAT_ST;

typedef struct
{
    VOS_CHAR  *abbrev;	        //Abbreviation
    VOS_UINT32 ul2GRegion;      //Country Region: 2.4G
    VOS_UINT32 ul5GRegion;      //Country Region: 5G
}country_abbrev_t;

typedef	struct _SHA_CTX
{
    VOS_UINT32 Buf[5];             // buffers of five states
    VOS_UINT8  Input[80];          // input message
    VOS_UINT32 LenInBitCount[2];   // length counter for input message, 0 up to 64 bits
}SHA_CTX;

#ifdef SUPPORT_ATP_WLAN_EP
/* configuration 对象的参数 */
typedef struct
{
    VOS_UINT32 	ulWlEPConfInstId;
	VOS_CHAR    acWlEPSSID[WLAN_STR_32_LEN];             // Profile的SSID
	VOS_CHAR    acWlBSSID[WLAN_STR_32_LEN];              // BSSID
	VOS_UINT32  ulWlEPChannel;                           // Channel
    VOS_CHAR    acWlEPSecMode[WLAN_16_LEN];              // Profile的安全方式
    VOS_CHAR    acWlEPEncrypType[WLAN_16_LEN];           // Profile的加密模式
    VOS_UINT32  ulWlEPWEPKeyIndex;                       // Profile记录的wEP key Index
    VOS_CHAR    acWlEPWepKey[WLAN_STR_32_LEN];          // Profile记录的wep key
    VOS_CHAR    acWlEpPresharedKey[WLAN_PSK_MAX_LEN + 4];// Profile记录的Psk key
    VOS_UINT32 	ulWlEPConfPri;                           //Profile 连接优先级级别
}WLAN_EP_CONF_PARA_ST;


typedef struct
{
    VOS_UINT32 	ulLDevInstId;
    VOS_UINT32 	ulWlEPInstId;
    VOS_CHAR    acWlEPName[WLAN_16_LEN]; /*接口名wl0,wl0.1,...wl0.7*/  
    VOS_INT32   iWlEPDouBand;            /*波段，0表示2.4GHz，1表示5GHz*/	
    VOS_UINT32 	ulWlEPDefaultId;	
    WLAN_EP_CONF_PARA_ST stWlEPConfVar[WLAN_EP_MAX_PROFILE_NUM];
}WLAN_EP_PARA_ST;

/* configuration 对象的参数 */
/* WLSTA 搜索的AP列表 */
typedef struct
{
    VOS_UINT32  ulWlInstId;  
	VOS_CHAR    acWlSSID[WLAN_STR_32_LEN + 4];                 // 记录AP的SSID
    VOS_CHAR    acWlBSSID[WLAN_STR_32_LEN];                // 记录AP的BSSID
    VOS_CHAR    acWlPhyType[WLAN_16_LEN];              // 记录AP的物理类型
    VOS_CHAR    acWlSecMode[WLAN_16_LEN];              // 记录AP的安全方式
    VOS_CHAR    acWlEncrypType[WLAN_16_LEN];           // 记录AP的加密模式
    VOS_UINT32  ulWlChannel;                            // 记录AP使用的信道
    VOS_UINT32  ulWlSignal;                             // 记录AP的信号强度
    VOS_UINT32  ulHtExtcha;
    VOS_UINT32  ulWlSupportWPS;                         // 记录AP是否支持WPS 0:NO, 1:YES
    VOS_CHAR    acWlNetWorkType[WLAN_16_LEN];          // 记录AP的网络类型
}WLAN_EP_APLIST_ST;

/* WLSTA 的连接状态 */
typedef struct
{
    VOS_CHAR    acWlSSID[WLAN_STR_32_LEN];
    VOS_CHAR    acWlBSSID[WLAN_STR_32_LEN];
    VOS_CHAR    acWlConnStatus[WLAN_16_LEN];
    VOS_UINT32  ulWlChannel;
    VOS_UINT32  ulWlSignal;
    VOS_UINT32  ulWlConnSpeed;
    VOS_UINT32  ulWlTxSuccPkt;
    VOS_UINT32  ulWlReTxSuccPkt;
    VOS_UINT32  ulWlTxFailPkt;
    VOS_UINT32  ulWlRxSuccPkt;
    VOS_UINT32  ulWlRxFailPkt;    
}WLAN_EP_LINKINFO_ST;
#endif

/*****************************************************************************
*                               API Module
******************************************************************************/
/*!
  \brief WLANCMS初始化
  \param[in] 无
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_Init(VOS_VOID);

/*!
  \brief WLANCMS去初始化
  \param[in] 无
  \return VOS_NOK 或 VOS_OK
  \retval 此函数是预留函数
*/
VOS_UINT32 ATP_WLAN_UnInit(VOS_VOID);

/*!
  \brief    根据sta mac地址来获取sta的域名
  \param[in] 1.  pszMacAddr:
  \param[in] 2.  pszDomain:
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetAssocDomainByMac(VOS_CHAR *pszMacAddr,VOS_CHAR *pszDomain);

/*!
  \brief    获取SSID在g_astWlMapTbl中的位置
  \param[in] 1.  idx:
  \param[in] 2.  pulWlStartIdx:
  \param[in] 3.  pulWlEndIdx:
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetSSIDNum(VOS_UINT32 idx, VOS_UINT32 *pulWlStartIdx, VOS_UINT32 *pulWlEndIdx);

/*!
  \brief    根据接口名获取路径. 例如"wl0"得InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.与{1,1,0,0,0,0}
  \param[in] 1.  pszPath:
  \param[in] 2.  ulPathLen:
  \param[in] 3.  pulInstId:必须深度为ATP_CFM_MAX_TREE_DEPTH的ulKeyList
  \param[in] 4.  pszInterface:
  \return VOS_NOK 或 VOS_OK
  \retval 接口路径(字符串形式与ulKeyList形式)
*/
VOS_UINT32 ATP_WLAN_GetPathByInterface(VOS_CHAR* pszPath, VOS_UINT32 ulPathLen,
                                             VOS_UINT32* pulInstId, VOS_CHAR* pszInterface);

/*!
  \brief    根据路径获取接口名.例如InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.与{1,1,0,0,0,0}得"wl0"
  \param[in] 1.  pszPath:
  \param[in] 2.  pulInstId:必须深度为ATP_CFM_MAX_TREE_DEPTH的ulKeyList
  \param[in] 3.  pszInterface:
  \param[in] 4.  ulIfcLen:
  \return VOS_NOK 或 VOS_OK
  \retval 接口路径(字符串形式与ulKeyList形式)
*/
VOS_UINT32 ATP_WLAN_GetInterfaceByPath(VOS_CHAR* pszPath, VOS_UINT32* pulInstId,
                                                    VOS_CHAR* pszInterface, VOS_UINT32 ulIfcLen);

/*!
  \brief    根据接口名获取别名.wl0得SSID1
  \param[in] 1.  pszIntAlias:
  \param[in] 2.  pszInterface:
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetAliasByInterface(VOS_CHAR* pszIntAlias, VOS_UINT32 ulLen, VOS_CHAR* pszInterface);

/*!
  \brief    根据别名获取接口名.SSID1得wl0
  \param[in] 1.  pszInterface:
  \param[in] 2.  pszIntAlias:
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetInterfaceByAlias(VOS_CHAR* pszInterface, VOS_UINT32 ulLen, VOS_CHAR* pszIntAlias);

/*!
  \brief    根据接口名获取Setup类型. 例如"wl0"得InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.与{1,1,0,0,0,0}
  \param[in] 1.  pszPath:
  \param[in] 2.  ulIfcLen:
  \param[in] 3.  pulInstId:必须深度为ATP_CFM_MAX_TREE_DEPTH的ulKeyList
  \param[in] 4.  pszInterface:
  \return VOS_NOK 或 VOS_OK
  \retval 接口路径(字符串形式与ulKeyList形式)
*/
VOS_UINT32 ATP_WLAN_GetSetupTypeByInterface(WLAN_SETUP_TYPE_EN *penType, VOS_CHAR* pszInterface);

/*!
  \brief    WLAN绑定不同桥的处理
  \param[in] 1.  ulNewInstId:
  \param[in] 2.  aucInstId:
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_BindProc(VOS_UINT32 ulNewInstId, const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH]);

/*!
  \brief 获取wlan的状态
  \param[in] 1. idx
  \return WLAN的状态，1:开启. 0:关闭. -1:无效
  \retval 无
*/
VOS_INT32 ATP_WLAN_GetStatus(VOS_UINT32 idx);

/*!
  \brief 使能或去使能wlan模块
  \param[in] 1. idx
  \param[in] 2. iWlanEnable
  \return 无
  \retval 无
*/
VOS_VOID ATP_WLAN_SetStatus(VOS_UINT32 idx, VOS_INT32 iWlanEnable);

/*!
  \brief WLAN获取SSidIndex相应的连接数
  \param[in] 1.  pszInterface: wlan接口名
  \param[in] 2. *plAssocNum: 连接数  
  \param[in] 3. *pstAssocList: 连接地址列表
  \return VOS_NOK 或 VOS_OK
  \retval 连接数与地址列表
*/
VOS_UINT32 ATP_WLAN_GetAssocList(VOS_CHAR* pszInterface, 
                VOS_UINT32 *pulAssocNum, WLAN_TR069_ASSOC_ST *pstAssocList);

/*!
  \brief WLAN获取当前使用信道
*/
VOS_UINT32 ATP_WLAN_GetChannel(VOS_UINT32 idx,VOS_CHAR* pszChannel,VOS_UINT32 ulChannelLen);            

/*!
  \brief WLAN获取当前使用速率
*/
VOS_UINT32 ATP_WLAN_GetRate(VOS_UINT32 idx,VOS_CHAR* pszRate, VOS_UINT32 ulRateLen);


/*!
  \brief 提供WLAN配置参数全局结构给其他文件使用
  \param[in] *pstWlConfVar
  \return 无
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetAllVar(WLAN_TR069_PARA_ST *pstWlConfVar);

/*!
  \brief  WLAN配置生效函数。
  \param[in] 1.  pvData:WLAN配置生效的参数(需要转换成WLAN_SETUP_TYPE_EN *)
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_SetUp(WLAN_SETUP_TYPE_EN enData);

/*!
  \brief  WLAN 2.4G配置生效函数。
  \param[in] 1.  pvData: 不校验参数
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_2G_SetUp(VOS_VOID *pvData);

/*!
  \brief  WLAN 5G配置生效函数。
  \param[in] 1.  pvData: 不校验参数
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_5G_SetUp(VOS_VOID *pvData);

/*!
  \brief get value by variable name.通常用作获取实时参数的信息.或者只读属性
  \param[in] 1. enVar 只读属性 
  \param[in] 2. pszInterface 接口名
  \param[in] 3. pcVarValue ：variable name. 
  \param[in] 4. ulValueLen: variable value. 
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_GetVar(WLAN_READ_ONLY_VAR_EN enVar, VOS_CHAR* pszInterface,
                                  VOS_CHAR *pcVarValue, VOS_UINT32 ulValueLen);

/*!
  \brief set value by variable name.
  \param[in] 1. pszInterface 接口名
  \param[in] 2. pszVarName variable name
  \param[in] 3. pcVarValue    variable value. 
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_SetVar(VOS_CHAR* pszInterface, char *pszVarName, char *pszVarValue);

/*!
  \brief register notify event by other module which client is access/unaccess.
  \param[in] 1. pfProc
  \param[in] 2. ulPriority
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WlanClientAssoc_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority);
VOS_UINT32 ATP_WlanClientDisassoc_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority);


#ifdef WS323_MULTIWAN_LED_STATUS
VOS_UINT32 ATP_WlanStatusChg_NotifyRegister(AtpNotifierProc pfProc, VOS_UINT32 ulPriority);
VOS_UINT32 WlanStatusChgDoNotify(WLAN_STATUS_ST *pstWlanStatus);
#endif

#ifdef SUPPORT_ATP_FON
/*!
  \brief 提供接口给FON 设置 SSID
  \return VOS_NOK 或 VOS_OK
  \retval 无
*/
VOS_UINT32 ATP_WLAN_FonSsidConfig(VOS_CHAR * pszSsid, VOS_BOOL iEffect);
#endif
/*!
  \brief 响应wlan or WPS的按键事件，发转无线状态
  \param[in] 1.  pstMsg:
  \return 无
  \retval 无
*/
#if defined(SUPPORT_ATP_WLAN_MAIN_2G) || defined(SUPPORT_ATP_WLAN_COM_2G)
VOS_VOID ATP_WLAN_MonitorButtonStatus(const ATP_MSG_HEADER_ST *pstMsg);
#ifdef SUPPORT_ATP_WPS
VOS_VOID ATP_WPS_MonitorButtonStatus(const ATP_MSG_HEADER_ST *pstMsg);
#endif
#endif

#ifdef SUPPORT_ATP_WLAN_EP
VOS_VOID ATP_Band_MonitorButtonStatus(const ATP_MSG_HEADER_ST *pstMsg);
#endif


#if defined(SUPPORT_ATP_WLAN_MAIN_5G) || defined(SUPPORT_ATP_WLAN_COM_5G)
VOS_VOID ATP_WLAN_5G_MonitorButtonStatus(const ATP_MSG_HEADER_ST *pstMsg);
#ifdef SUPPORT_ATP_WPS
VOS_VOID ATP_WPS_5G_MonitorButtonStatus(const ATP_MSG_HEADER_ST *pstMsg);
#endif
#endif

#ifdef SUPPORT_ATP_WLAN_DT
VOS_UINT32 ATP_WLAN_DEL_MACFILTER(VOS_CHAR * pstMacAddress);
/* Start of add by l00164468 2012/10/24 for DTS2012102101106 */
VOS_INT32 ATP_WLAN_GetInitSetupStatus(VOS_VOID);
/* End of add by l00164468 2012/10/24 for DTS2012102101106 */
VOS_VOID ATP_WLAN_SetStatusNoDBSave(VOS_UINT32 idx, VOS_INT32 iWlanEnable);
#endif

/*****************************************************************************
*                       供内部wlancms调用的函数原型
******************************************************************************/
#ifdef SUPPORT_ATP_WLAN_DT
VOS_UINT32 Wlan_DelMacfilter(VOS_CHAR * pstMacAddress);
#endif
VOS_UINT32 WlanFindChangeInFile(VOS_CHAR *pFile, VOS_CHAR *pcKey,
                                        VOS_VOID *pvSrc, VOS_VOID *pvDst, VOS_UINT32 ulDepth);
VOS_VOID Wlan_GetPathFromDB(VOS_CHAR* pszPath, VOS_UINT32 ulPathLen,
                                      WLAN_RES_EN enWlanRes, const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH]);
VOS_UINT32 Wlan_GetStatusFromDB(VOS_INT32 *piWlanStatus, VOS_CONST_CHAR *pszBand);
VOS_UINT32 Wlan_GetFromDB(WLAN_TR069_PARA_ST *pstWlVar, VOS_INT32 iBand);
VOS_UINT32 WlanGetAssocDomainFromMac(VOS_CHAR *pszMacAddr,VOS_CHAR *pszDomain);
VOS_UINT32 WlanSetWepKeyToDB(VOS_UINT32 ulWlVarIdx, VOS_UINT32 ulWepKeyIdx);
VOS_UINT32 WlanSetStatusToDB(VOS_UINT32 ulInstId, VOS_UINT32 ulSetValues);
VOS_UINT32 WlanMoveObj(VOS_UINT32 ulNewInstId, const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH]);
VOS_UINT32 WlanGetIpAndHostnameFromDB(WLAN_TR069_ASSOC_ST *pstAssocList);
VOS_VOID WlanGetBridgeInfo(VOS_UINT32 *pulBrNum, WLAN_BRIDGE_INFO_ST *pstWlBrInfo);
VOS_UINT32 WlanUpdateBridge(VOS_BOOL isEnable, VOS_CHAR* pszPath);
#ifdef SUPPORT_ATP_WLANTEST
VOS_UINT32 WlanSetDBFromWPS(VOS_UINT32 ulWlVarIdx);
#endif
VOS_UINT32 WlanClientAssocDoNotify(WLAN_TR069_ASSOC_ST * pstAssocList);
VOS_UINT32 WlanClientDisassocDoNotify(WLAN_TR069_ASSOC_ST * pstAssocList);
VOS_VOID ATP_WLAN_RESOURCE_INIT(VOS_VOID);

#ifdef SUPPORT_ATP_WLAN_EP
VOS_UINT32 ATP_WLAN_EP_GET_AP_LIST(VOS_VOID);
VOS_UINT32 ATP_WLAN_EP_GET_LinkInfor(WLAN_EP_LINKINFO_ST *pstWlApCliLinkInfo);
VOS_UINT32 Wlan_GetModeFromDB(WLAN_MODE_TYPE_EN *piWlanMode, VOS_CONST_CHAR *pszBand);
#ifdef SUPPORT_ATP_WPS
#if defined(SUPPORT_ATP_WLAN_MAIN_2G)
VOS_VOID WlanEPWpsPress(VOS_VOID);
#endif
#if defined(SUPPORT_ATP_WLAN_MAIN_5G)
VOS_VOID WlanEP5GWpsPress(VOS_VOID);
#endif
#endif
VOS_UINT32 WlanSetModeToDB(VOS_UINT32 ulInstId, VOS_UINT32 ulSetValues);
VOS_UINT32 WlanEP_GetFromDB(WLAN_EP_PARA_ST *pstWlEPVar, VOS_INT32 iBand);
#endif

VOS_UINT32 WlanModeInit(VOS_VOID);
VOS_UINT32 WlanModeUnInit(VOS_VOID);
VOS_UINT32 WlanModeSetup(WLAN_SETUP_TYPE_EN enType);
VOS_VOID WlanResourceInit(VOS_VOID);
VOS_VOID WlanSetSetupAllFlag(WLAN_SETUP_TYPE_EN enType);
VOS_VOID WlanSetParSetupAll(VOS_VOID);
VOS_VOID WlanSetAction(VOS_VOID *pvArg);
VOS_VOID WlanChangWlanState(VOS_INT32    iWlStatue , VOS_BOOL bForPassStaOrEndTime);
/*START MODIFY by y00181549 for  DTS2013092900632 at 2013/9/29*/
VOS_UINT32 WlanStandardFormat(VOS_CHAR * pszStandard);
/*END MODIFY by y00181549 for  DTS2013092900632 at 2013/9/29*/
/* START ADD by y00181549 for DTS2013091006940,DTS2013091001799,DTS2013091001650 at 2013/9/30*/
VOS_INT32 WlanStaConnectSuccProc(VOS_UINT32 ulSsidObjIdx, VOS_CHAR * pacHostName,  VOS_CHAR * pacMacaddr);
/* END ADD by y00181549 for DTS2013091006940,DTS2013091001799,DTS2013091001650 at 2013/9/30*/
/*!
  \brief  hostcms需要用此接口,当mac地址过滤使能,hostcms需要考虑被加入wlan mac地址白名单的host不能自动老化
            反之,wlanmac地址去使能,可以自动老化.
            
  \param[in] 1.  pvData: 不校验参数
  \return VOS_TRUE  WLAN mac地址白名单使能
            VOS_FALSE  不使能
  \retval 无
*/
VOS_BOOL ATP_WLAN_GetWlanMacFilterEnable(VOS_VOID);
#ifdef __cplusplus
}
#endif

#endif /*_WLAN_CMS_API_H*/

