/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : bhal.h
  ��    ��  : atp v1.0
  ��������  : 2009-3-3
  ��    ��  :      ����bhal����ͨ�õĺ�ͽӿ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2009-3-3
     ��    ��   : l63336
     �޸�����   : ��ɳ���
     
��ע:
     bhalplat.h ֻ���ں�̬������á�
     
     �û�̬����ͳһ����atpflashapi.h,���������bhal.h
     bhal.h�����ں�̬���û�̬�����Ľṹ�塣

     ����: Flash/gpio/led/voip/hardware/key
     
*********************************************************************************/

#ifndef _BHAL_H
#define _BHAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "atpconfig.h"

typedef struct button
{
    int vaild;//�Ƿ���Ч
    unsigned short buttonPin;//��Ӧ��irq����pin��
    unsigned short buttonwork;//������Ч���ɿ����޷���ʼ�̰�����
    int keystatus;//��ǰ״̬
    int keymode;//irq����gpio
    int count1;//�̰��ɿ���Ч
    int count2;//������ס��Ч
    int (*fun1)();//�̰�����
    int (*fun2)();//��������
    int keycount;//������
}ATP_KEY_ST;

/* Flash definitions. */
#ifndef FLASH_API_OK
#define FLASH_API_OK                       0
#endif

#ifndef FLASH_API_ERROR
#define FLASH_API_ERROR                  (-1)
#endif

#define BHAL_OK                      0
#define BHAL_ERROR                   (-1)


#define BHAL_POLLCNT_1SEC           HZ
#define BHAL_FLASH_IDLE             0
#define BHAL_FLASH_WRITING          1

#ifdef DT_MAC_SUPPORT
#if defined(DT_W724V_MAC_SUPPORT)
/***************************************************
DT ��������:
��̫���ڹ̶���õ�1��MAC
PPPoE û��VLAN�Ľӿڻ�ȡ��2��MAC
PPPoE VLAN 7�Ľӿڻ�ȡ��3��MAC
DHCP VLAN 8�Ľӿڻ�ȡ��4��MAC
WLAN SSID1��ȡ��5��MAC
WLAN SSID1��ȡ��6��MAC
WLAN SSID1��ȡ��7��MAC
WLAN SSID1��ȡ��8��MAC
****************************************************/
typedef enum
{
    ATP_BHAL_ETH_MAC = 0,                    /*��̫����MAC*/        
    ATP_BHAL_PPPOE_WITHOUT_VLAN_MAC,         /*PPPoE û��VLAN�ӿ�MAC*/
    ATP_BHAL_PPPOE_VLAN_7,                   /*PPPoE VLAN 7�ӿ�MAC*/
    ATP_BHAL_DHCP_VLAN_8,                    /*DHCP VLAN 8�ӿ�MAC*/
    ATP_BHAL_WLAN_SSID1,                     /*WLAN SSID1 MAC*/
    ATP_BHAL_WLAN_SSID2,                     /*WLAN SSID2 MAC*/
    ATP_BHAL_WLAN_SSID3,                     /*WLAN SSID3 MAC*/
    ATP_BHAL_WLAN_SSID4,                     /*WLAN SSID4 MAC*/
	/* <DTS2013082102506 xiazhongyue 20130821 begin */
    ATP_BHAL_LTE_MAC,                      /*LTE wan �ӿ�mac��ַ */
	/* DTS2013082102506 xiazhongyue 20130821 end>   */
    ATP_BHAL_END_MAC,
}ATP_BHAL_MAC_TYPE_EN;

#elif defined(DT_W723V_MAC_SUPPORT)
/***************************************************
DT ��������:
��̫���ڹ̶���õ�1��MAC��
PPPoE û��VLAN�Ľӿڻ�ȡ��2��MAC
PPPoE VLAN 7�Ľӿڻ�ȡ��3��MAC
DHCP VLAN 8�Ľӿڻ�ȡ��4��MAC
WLAN�ӿڹ̶���õ�5��MAC��
****************************************************/
typedef enum
{
        ATP_BHAL_ETH_MAC = 0,                    /*��̫����MAC*/        
        ATP_BHAL_PPPOE_WITHOUT_VLAN_MAC,      /*PPPoE û��VLAN�ӿ�MAC*/
        ATP_BHAL_PPPOE_VLAN_7,                 /*PPPoE VLAN 7�ӿ�MAC*/
        ATP_BHAL_DHCP_VLAN_8,                   /*DHCP VLAN 8�ӿ�MAC*/
        ATP_BHAL_WLAN_MAC,                       /*WLAN�ӿ�MAC*/
        ATP_BHAL_END_MAC,
}ATP_BHAL_MAC_TYPE_EN;
#endif

#else

/***************************************************
ͨ�ù��:
��̫���ڷ����1��MAC(��MAC)
WAN�ӿڷ����2-9��MAC����˳�����
WLAN�ӿڵ�10-13��MAC����˳�����

****************************************************/
typedef enum
{
        ATP_BHAL_ETH_MAC = 0,                    /*��̫����MAC*/        
        ATP_BHAL_WAN_MAC,                       /*WAN�ӿ�MAC*/
        ATP_BHAL_WLAN_MAC,                       /*WLAN�ӿ�MAC*/        
        ATP_BHAL_END_MAC,
}ATP_BHAL_MAC_TYPE_EN;
typedef enum
{
        BHAL_WAN_MAC_MIN_INDX = 1,                    /*WAN�ӿ�MAC*/        
        BHAL_WAN_MAC_MAX_INDX = 8,                    /*WAN�ӿ�MAC*/        
        BHAL_WLAN_MAC_MIN_INDX = 9,                    /*WLAN�ӿ�MAC*/        
        BHAL_WLAN_MAC_MAX_INDX = 12,                    /*WLAN�ӿ�MAC*/        
}BHAL_MAC_TYPE_MIN_MAX_EN;
#endif

/* Start of adding support for WS322/WS323 MAC scheme, d00176003@2012/08/28 */
typedef enum 
{
	ATP_BHAL_WS_MODE_AP = 0, 			/* APģʽ */
	ATP_BHAL_WS_MODE_CLIENT,			/* Clientģʽ */
	ATP_BHAL_WS_MODE_REPEATER, 			/* Repeaterģʽ */
	ATP_BHAL_WS_MODE_END,
} ATP_BHAL_WS_MODE;
/* Start of adding support for WS322/WS323 MAC scheme, d00176003@2012/08/28 */

typedef enum 
{
    BSP_E_FLASH_AREA_BOOTLOADER = 0,         /* bootloader���� */
    BSP_E_FLASH_AREA_MAIN_KERNEL,            /* �ں˷��� */
    BSP_E_FLASH_AREA_MAIN_FS,                /* �ļ�ϵͳ���� */
    BSP_E_FLASH_AREA_CONFIGFILE_CURRENT,     /* ��ǰ�����ļ����� */
    BSP_E_FLASH_AREA_CONFIGFILE_CUSTOMIZE,   /* ��Ӫ�������ļ����� */    
    BSP_E_FLASH_AREA_CONFIGFILE_FACTORY,     /* ���������ļ����� */
    BSP_E_FLASH_AREA_TEMP_PARAM,			    /* �����ɱ��������: �������������ʶ�Լ�TR069�������� */
    BSP_E_FLASH_AREA_FIXED_PARAM,            /* MAC,SN�������� */    
    BSP_E_FLASH_AREA_LOG,			        /* ��־�������� */    
    BSP_E_FLASH_AREA_SLAVE_KERNEL,            /* �ڶ��ں˷��� */
    BSP_E_FLASH_AREA_SLAVE_FS,                /* �ڶ��ļ�ϵͳ���� */
    BSP_E_FLASH_AREA_ALLFLASH,                /* ����Flash���� */
    BSP_E_FLASH_AREA_END                      /* ������ */
} BSP_E_FLASH_AREA;

#define BHAL_WAN_INTERFACE_LEN_MAX	16

/* ϵͳ������־��Ϣ: */
#define UGPINFO_SIZE                        16
#define BHAL_UPGINFO_START_OFFSET           BHAL_TEMP_PARAM_START_OFFSET
                      
#define BHAL_TR069Info_OFFSET               (BHAL_TEMP_PARAM_START_OFFSET+UGPINFO_SIZE)
#define BHAL_TR069_INFO_SIZE                (2*1024)

#define BHAL_PIN_INFO_OFFSET                (BHAL_TR069Info_OFFSET+BHAL_TR069_INFO_SIZE)
#define BHAL_PIN_INFO_SIZE                  (256)

#define  BHAL_BOOTFROM_INFO_OFFSET          (BHAL_PIN_INFO_OFFSET+BHAL_PIN_INFO_SIZE)
#define  BHAL_BOOTFROM_INFO_SIZE            16

#define   BHAL_SLAVE_DIFFADDR_OFFSET        (BHAL_BOOTFROM_INFO_OFFSET+BHAL_BOOTFROM_INFO_SIZE)
#define   BHAL_SLAVE_DIFFADDR_SIZE          (12)

/*begin of modification for double config for SRB by y00181549 for DTS2011090202124, 2011.9.5*/
#define BHAL_CFGFILE_STAT_SIZE              (8)

#define BHAL_PPP_SESSION_OFFSET              0

#define ATP_PPPC_IFC_LEN 32
#define WAN_PPP_SESSION_LEN 32
#define WAN_MAX_NUM 16

typedef struct{    
    /* PPP�ӿ��� */    
    char acInterface[ATP_PPPC_IFC_LEN];    
    /* ppp�ỰID */   
    char acPppSessid[WAN_PPP_SESSION_LEN];
}BHAL_PPP_SESSION_ST;


#ifndef _ATP_BHAL_PARAM_EN_
#define _ATP_BHAL_PARAM_EN_
/* ��оƬ */
typedef enum 
{  
    BHAL_CHIP_ID = 0,               /* ��оƬID */
    BHAL_CHIP_REVISION_ID,          /* ��оƬ�汾�� */
    BHAL_CHIP_CLK_SEL,              /*��оƬ����*/
    BHAL_CHIP_PHY_ID,               /* PHYоƬIDֵ */
    BHAL_CHIP_PARAM_END,            /* ������ */
} ATP_BHAL_CHIP_PARAM_EN;

#endif

/* ���� */
typedef enum 
{  
    /* �����������ID. ��ID��Ӧһ�׵������ò�����һ�׵�����Ը��ݲ�ͬ����Ҫ��
       ���ò�ͬ�ĵ������ID,�Ӷ����в�ͬ�ĵ������ò��� */
    BHAL_BOARD_ID = 0,
    /* �г�ÿ�׵�������Ӧ�ĵ����������ID */
    BHAL_BOARD_ALL_ID,
    /* ����Ӳ���汾�� */            
    BHAL_BOARD_VERSION,    
    /* A�壬B�壬C�� */
    BHAL_BOARD_PCBVERSION,        
    BHAL_BASE_MAC_ADDR,             /* ��mac��ַ */
    BHAL_MAC_NUMBER,                /* mac��ַ��Ŀ */
    BHAL_BOARD_MANUFACTURER,        /* ����������   */
    BHAL_BOARD_SERIALNUMBER,        /* ����ϵ�к�   */
    BHAL_BOARD_PRODUCTSN,           /* �������к�sn   */
    BHAL_CUSTOMER_SERIALNUM,        /* ʵ�����豸��־��ouinum*/
    BHAL_CWMP_USERNAME,
    BHAL_CWMP_PWD,
    BHAL_USER_PWD,                  /* admin����ȱʡ���� */ 
    BHAL_USER_NAME,
    BHAL_SIP_PWD1,
    BHAL_WLAN_PWD,                  /* WLAN ����PSK ���� */
    BHAL_SSID,                      /* WLAN����ȱʡSSID */
    BHAL_APPINCODE,                 /* WLAN����ȱʡappin�� */
    BHAL_WEP_PWD,                   /* WLAN ����WEPKey ���� */
    BHAL_WLAN_PWD_5G,
    BHAL_SSID_5G,
    BHAL_APPINCODE_5G,
    BHAL_WEP_PWD_5G,
    BHAL_OUI,
    BHAL_CUSTOMER_SN1,
    BHAL_PPPOE_USER,
    BHAL_PPPOE_PWD,
    BHAL_FON_KEY,
    BHAL_BOARD_MAKE_DATE,           /*������������*/
    BHAL_TELNET_STATUS,             /* Telnet ����/�رձ�ʶ */
    BHAL_SEC_KEY,                   /* sec ģ��device keys */
    BHAL_TR064_RESETPWD,            /* TR064������������ */
    BHAL_EQUIP_ELABLE,               /* ���ӱ�ǩ*/
#ifdef SUPPORT_ATP_EQUIP_WLAN_FLAG
    BHAL_EQUIP_DISABLE_WLAN_FLAG,
#endif
#ifdef SUPPORT_ATP_EQUIP_PORT_CHK_FLAG
    BHAL_EQUIP_PORT_PROBED_BITMAP,
#endif
    BHAL_IDENTIFIER1,
    BHAL_DECT_CALIB_PARA,
    BHAL_BASE_MAC_ADDR_HALF,
    BHAL_BOARD_PARAM_END,           /* ������ */
} BSP_E_PARAM_TYPE;


typedef enum
{   
    BHAL_LED_Power,
    BHAL_LED_Adsl,
    BHAL_LED_Adsl2,
    BHAL_LED_Internet,
    BHAL_LED_Lan1,
    BHAL_LED_Lan2, //5
    BHAL_LED_Lan3,
    BHAL_LED_Lan4,
    BHAL_LED_Lan5,
    BHAL_LED_Wan,
    BHAL_LED_Hpna, //10
    BHAL_LED_Hspa,
    BHAL_LED_Ppp,
    BHAL_LED_Wlan,
    BHAL_LED_WlanEable,
    BHAL_LED_Wlan5G, //15
    BHAL_LED_Wlan5GEable,
    BHAL_LED_Wps,
    BHAL_LED_Usb,
    BHAL_LED_BlPower,
    BHAL_LED_BlResetCfg, //20    
    BHAL_LED_BlReStop,
    BHAL_LED_Voip1,
    BHAL_LED_Voip2,
    BHAL_LED_Service1,
    BHAL_LED_Service2, //25
    BHAL_LED_Service3,
    BHAL_LED_Service4,
    BHAL_LED_DectSearch,
    BHAL_LED_Pots,
    BHAL_LED_SesWlan, //30
    BHAL_LED_LinkLte,// 31   // START ADD: l00252599 FOR LTE LED
    BHAL_LED_LineLte1,//32
    BHAL_LED_LineLte2,//33
    BHAL_LED_LineLte3,//34
    BHAL_LED_LineLte4,//35
    BHAL_LED_LineLte5,//36
    BHAL_LED_LineLteAll,//37  // START ADD: l00252599 FOR SETUP LED
    BHAL_LED_ServiceAll,//38    
    BHAL_LED_Failstart, //NOTE: Insert the new led name before this one.
    BHAL_LED_Powerfail,
    BHAL_LED_Adslfail,
    BHAL_LED_Adsl2fail,
    BHAL_LED_Internetfail,
    BHAL_LED_Lan1fail,
    BHAL_LED_Lan2fail,
    BHAL_LED_Lan3fail,
    BHAL_LED_Lan4fail,
    BHAL_LED_Lan5fail,
    BHAL_LED_Wanfail,
    BHAL_LED_Hpnafail,
    BHAL_LED_Hspafail,
    BHAL_LED_Pppfail,
    BHAL_LED_Wlanfail,
    BHAL_LED_WlanEablefail,
    BHAL_LED_Wlan5Gfail,
    BHAL_LED_Wlan5GEablefail,
    BHAL_LED_Wpsfail,
    BHAL_LED_Usbfail,
    BHAL_LED_BlPowerfail,
    BHAL_LED_BlResetCfgfail,    
    BHAL_LED_BlReStopfail,
    BHAL_LED_Voip1fail,
    BHAL_LED_Voip2fail,
    BHAL_LED_Service1fail,
    BHAL_LED_Service2fail,
    BHAL_LED_End,       // NOTE: Insert the new led name before this one.
} BSP_E_LED_NAME;

/* LED State */
typedef enum
{
    BHAL_LED_STATE_OFF = 0,                  /* turn led off */
    BHAL_LED_STATE_ON,                     /* turn led on */
    BHAL_LED_STATE_RED,                 /* turn led on red */
    BHAL_LED_STATE_BLINKONCE,           /* blink once, ~xxxms and ignore the same call during the xxxms period */
    BHAL_LED_STATE_SLOW_FLICKER_ONCE,   /* Slow Flicker once, VDF customized and ignore the same call during blink period */
    BHAL_LED_STATE_FAST_FLICKER_ONCE,   /* Fast Flicker once, VDF customized and ignore the same call during blink period */
    BHAL_LED_STATE_VERY_SLOW_BLINK_CONTINUES,     /* Jiajun 12/18/2007: slow blink contines at ~500ms interval */    
    BHAL_LED_STATE_SLOWBLINK_CONTINUES, /* slow blink continues at ~xxxms interval */
    BHAL_LED_STATE_FASTBLINK_CONTINUES, /* fast blink continues at ~xxxms interval */    
    BHAL_LED_STATE_UPG_FASTBLINK_CONTINUES, /* fast blink continues at ~xxxms interval */    
    BHAL_LED_STATE_UPG_FASTBLINK_STOP, /* fast blink continues at ~xxxms interval */
	BHAL_LED_STATE_SERVICE_SLOWBLINK_CONTINUES = 30,
	BHAL_LED_STATE_BITTEWARTEN_SLOWBLINK_CONTINUES,
	BHAL_LED_WPS_WLAN_STATE_SLOWBLINK_CONTINUES,
	BHAL_LED_WPS_VOIP_STATE_SLOWBLINK_CONTINUES,
	BHAL_LED_WPS_WLAN_VOIP_STATE_SLOWBLINK_CONTINUES,
    BHAL_LED_STATE_END                 /* ������ */
} BSP_E_LED_STATE;

typedef enum
{
    BHAL_LED_Green = 0,
    BHAL_LED_Red,
    BHAL_LED_Yellow,
    BHAL_LED_All,
}BSP_E_LED_EQUIPCOLOR;


typedef enum
{
    BHAL_LED_SET = 0,
    BHAL_LED_ALL_ON,
    BHAL_LED_ALL_OFF,
    BHAL_LED_TIME_SET,
    BHAL_LED_SEQUENCE_BLINK,
    BHAL_LED_WPS_SET,
    BHAL_LED_TEST_ON,
    BHAL_LED_TEST_OFF,
    BHAL_LED_TEST_STATE_SET,
    BHAL_LED_ALL_BLINK,
    BHAL_LED_ACTION,
}BSP_E_LED_OPERATION;

typedef enum
{
    BHAL_GPIO_LOW32 = 0,   /* GPIO��ΪGPIO0-GPIO31 */
    BHAL_HIGH_BIT,         /* GPIO��ΪGPI32������ */
}BHAL_GPIO_NUM;


typedef enum 
{  
    BHAL_GPIO_LOW = 0,              /* �͵�ƽ */
    BHAL_GPIO_HIGH,                 /* �ߵ�ƽ */
    BHAL_GPIO_END,                  /* ������ */
} BSP_E_GPIO_STATE;

/* Key Status */
typedef enum 
{
    BHAL_KEY_STATE_NOPRESS = 0,         /* ��ť�޶��� */
    BHAL_KEY_STATE_PRESS,               /* ��ť������ */
    BHAL_KEY_STATE_NOKEY,               /* ��ť������ */
    BHAL_KEY_STATE_END,                 /* ������ */
} BSP_E_KEY_STATE;


typedef enum
{
    BHAL_KEY_RESET = 0,             /* �ָ��������ð�ť */    
    BHAL_KEY_SWITCH_WLAN,           /* WLAN���ذ�ť */    
    BHAL_KEY_SWITCH_WPS,            /* WPS���ذ�ť */    
    BHAL_KEY_SWITCH_LCD,		/* LCD�л���ť*/
    BHAL_KEY_ENSURE_LCD,		/* LCDȷ�ϰ�ť*/
    BHAL_SCREEN_TIMER_LCD,
    BHAL_KEY_SWITCH_DECT_PP,        /* DECT ���밴ť */
    BHAL_KEY_SWITCH_DECT_PAGE,      /* DECT PAGE��ť */
    BHAL_KEY_SWITCH_WLAN5G,
    BHAL_KEY_OTHER1,
    BHAL_KEY_OTHER2,
    BHAL_KEY_OTHER3,
    BHAL_KEY_OTHER4,
    BHAL_KEY_OTHER5,
    BHAL_KEY_OTHER6,
    BHAL_KEY_END,                   /* ������ */
} ATP_BHAL_KEY_EN;

typedef enum
{
    BHAL_KEYSCAN_MODE_NORMAL = 0,
    BHAL_KEYSCAN_MODE_EQUIP,
    BHAL_KEYSCAN_MODE_END,
}ATP_BHAL_KEYSCAN_MODE_EN;

typedef enum
{
    BHAL_LED_MODE_NORMAL = 0,
    BHAL_LED_MODE_EQUIP,
    BHAL_LED_MODE_END,
}ATP_BHAL_LED_MODE_EN;


typedef enum {
	RED_LED = 0,
	GREEN_LED,
	BLUE_LED,
	NET_LINK_LED,
	WLAN_SIGNAL_HIGH_LED,
	WLAN_SIGNAL_MID_LED,
	WLAN_SIGNAL_LOW_LED,
	LED_NAME_MAX_NUMBER,
} BOARD_SPEC_LED_NAME;

typedef enum {
	LED_ACTION_OFF,
	LED_ACTION_ON,
	LED_ACTION_BLINK,
	LED_ACTION_MAX,
} BOARD_LED_ACTION;

typedef struct {
	BOARD_SPEC_LED_NAME name;
	BOARD_LED_ACTION action;
	unsigned long last; // second
	unsigned long on; // ms
	unsigned long off; // ms
} BOARD_LED_ACTION_INFO;



typedef enum 
{  
    BHAL_VOIP_OFF = 0,              /* û�н���VOIPҵ�� */
    BHAL_VOIP_ON,                   /* ���ڽ���VOIPҵ�� */
    BHAL_VOIP_END,                  /* ������ */
} ATP_BHAL_VOIP_STATE_EN;

/*****************************************************************************/
/*          board ioctl calls for flash, led and some other utilities        */
/*****************************************************************************/

/* Defines. for board driver */
#define BHAL_IOCTL_MAGIC             'B'
#define BHAL_DRV_MAJOR               255
#define NOT_SUPPROT_GET_VERSION      0xFF

#define MAC_ADDRESS_ANY         (unsigned long) -1

#define BHAL_IOCTL_INIT_FLASH \
    _IOWR(BHAL_IOCTL_MAGIC, 0, BHAL_IOCTL_PARMS_ST)   

#define BHAL_IOCTL_GET_FLASH_SIZE \
    _IOWR(BHAL_IOCTL_MAGIC, 1, BHAL_IOCTL_PARMS_ST)
    
#define BHAL_IOCTL_WRITE_FLASH \
    _IOWR(BHAL_IOCTL_MAGIC, 2, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_READ_FLASH \
    _IOWR(BHAL_IOCTL_MAGIC, 3, BHAL_IOCTL_PARMS_ST)
    
#define BHAL_IOCTL_ERASE_FLASH \
    _IOWR(BHAL_IOCTL_MAGIC, 4, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_UPDATE_FLASH \
    _IOWR(BHAL_IOCTL_MAGIC, 5, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_CHIP_PARAM \
    _IOWR(BHAL_IOCTL_MAGIC, 6, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_BOARD_PARAM \
    _IOWR(BHAL_IOCTL_MAGIC, 7, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SET_BOARD_PARAM \
    _IOWR(BHAL_IOCTL_MAGIC, 8, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_BASE_MAC_ADDRESS \
    _IOWR(BHAL_IOCTL_MAGIC, 9, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SET_BASE_MAC_ADDRESS \
    _IOWR(BHAL_IOCTL_MAGIC, 10, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_MAC_ADDRESS \
    _IOWR(BHAL_IOCTL_MAGIC, 11, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_RELEASE_MAC_ADDRESS \
    _IOWR(BHAL_IOCTL_MAGIC, 12, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_MAC_NUMBERS \
    _IOWR(BHAL_IOCTL_MAGIC, 13, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SET_MAC_NUMBERS \
    _IOWR(BHAL_IOCTL_MAGIC, 14, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_CTRL_LED \
    _IOWR(BHAL_IOCTL_MAGIC, 15, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SET_GPIO_BIT \
    _IOWR(BHAL_IOCTL_MAGIC, 16, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_VOIP_STATE \
    _IOWR(BHAL_IOCTL_MAGIC, 17, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_CHECK_KEY_STATE \
    _IOWR(BHAL_IOCTL_MAGIC, 18, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SOFT_REBOOT \
    _IOWR(BHAL_IOCTL_MAGIC, 19, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_HARD_REBOOT \
    _IOWR(BHAL_IOCTL_MAGIC, 20, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_BOOTLOADER_VERSION \
    _IOWR(BHAL_IOCTL_MAGIC, 21, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_SDRAM_SIZE \
    _IOWR(BHAL_IOCTL_MAGIC, 22, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_DYINGGASP \
    _IOWR(BHAL_IOCTL_MAGIC, 23, BHAL_IOCTL_PARMS_ST)
    
#define BHAL_IOCTL_GET_FLASH_OFFSET \
    _IOWR(BHAL_IOCTL_MAGIC, 24, BHAL_IOCTL_PARMS_ST)
    
#define BHAL_KEY_SWITCH_WLAN_WPS \
    _IOWR(BHAL_IOCTL_MAGIC, 25, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_SYS_REAL_TIME \
    _IOWR(BHAL_IOCTL_MAGIC, 26, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_PPP_SESSION \
    _IOWR(BHAL_IOCTL_MAGIC, 27, BHAL_IOCTL_PARMS_ST) 

#define BHAL_IOCTL_SAVE_PPP_SESSION \
    _IOWR(BHAL_IOCTL_MAGIC, 28, BHAL_IOCTL_PARMS_ST) 

#define BHAL_GET_DSLACTIVETIME \
    _IOWR(BHAL_IOCTL_MAGIC, 29, BHAL_IOCTL_PARMS_ST)
    
#define BHAL_GET_DSLUPTIME \
    _IOWR(BHAL_IOCTL_MAGIC, 30, BHAL_IOCTL_PARMS_ST)    

#define BHAL_IOCTL_GET_CURRENT_TIME \
    _IOWR(BHAL_IOCTL_MAGIC, 31, BHAL_IOCTL_PARMS_ST)

#ifdef SUPPORT_ATP_DECT_VOICE
#define BHAL_IOCTL_GET_DECT_CALIB_PARA \
    _IOWR(BHAL_IOCTL_MAGIC, 32, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SET_DECT_CALIB_PARA \
    _IOWR(BHAL_IOCTL_MAGIC, 33, BHAL_IOCTL_PARMS_ST)
#endif
#define BHAL_IOCTL_SET_KRNL_DBG_FLAGS \
    _IOWR(BHAL_IOCTL_MAGIC, 34, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_SETUP_KEYTEST_MODE \
    _IOWR(BHAL_IOCTL_MAGIC, 35, BHAL_IOCTL_PARMS_ST)   

#ifdef SUPPORT_ATP_TCCONSOLE
#define BHAL_IOCTL_SOFT_RESET \
    _IOWR(BHAL_IOCTL_MAGIC, 36, BHAL_IOCTL_PARMS_ST)
#endif

#ifdef BRCM_DAYING_GASP
#define BHAL_IOCTL_CTRL_DYING_GASP \
    _IOWR(BHAL_IOCTL_MAGIC, 37, BHAL_IOCTL_PARMS_ST)
#endif

#ifdef SUPPORT_ATP_BOARD_TYPE_DETECT
#define BHAL_IOCTL_GET_PRODUCT_TYPE \
    _IOWR(BHAL_IOCTL_MAGIC, 38, BHAL_IOCTL_PARMS_ST)
#endif

/*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
#define BHAL_IOCTL_GPIO_I2C_READ \
    _IOWR(BHAL_IOCTL_MAGIC, 39, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GPIO_I2C_WRITE \
    _IOWR(BHAL_IOCTL_MAGIC, 40, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GPIO_I2C_UPG \
    _IOWR(BHAL_IOCTL_MAGIC, 41, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GPIO_ATMEL_I2C_READ \
    _IOWR(BHAL_IOCTL_MAGIC, 42, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GPIO_ATMEL_I2C_WRITE \
    _IOWR(BHAL_IOCTL_MAGIC, 43, BHAL_IOCTL_PARMS_ST)
#endif

/*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/

#define BHAL_IOCTL_SET_LED \
    _IOWR(BHAL_IOCTL_MAGIC, 71, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_DEL_LED \
    _IOWR(BHAL_IOCTL_MAGIC, 72, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_DEL_ALL_LED \
    _IOWR(BHAL_IOCTL_MAGIC, 73, BHAL_IOCTL_PARMS_ST)

/*start: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
#define BHAL_IOCTL_SET_CONSOLE_TYPE \
    _IOWR(BHAL_IOCTL_MAGIC, 74, BHAL_IOCTL_PARMS_ST)
#endif
/*end: modify by j00127542 for Security red line, DTS2012050404171, 2012-05-04*/

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
/* ���ö�Ӧ�Ƶ���˸����*/
#define BHAL_IOCTL_SET_LED_TOTAL_TIMES \
    _IOWR(BHAL_IOCTL_MAGIC, 75, BHAL_IOCTL_PARMS_ST)
/* ��ȡ��Ӧ�Ƶ���˸����*/
#define BHAL_IOCTL_GET_LED_REMAIN_TIMES \
    _IOWR(BHAL_IOCTL_MAGIC, 76, BHAL_IOCTL_PARMS_ST)
/* ��ȡ��Ӧ�Ƶ�״̬*/
#define BHAL_IOCTL_GET_LED_STATE \
    _IOWR(BHAL_IOCTL_MAGIC, 77, BHAL_IOCTL_PARMS_ST)
#endif
#ifdef SUPPORT_ATP_VOICE_DT
#define BHAL_IOCTL_SET_URGENTCALL_STATUS \
    _IOWR(BHAL_IOCTL_MAGIC, 78, BHAL_IOCTL_PARMS_ST)
#endif
#define BHAL_IOCTL_SET_KERNELMAC \
    _IOWR(BHAL_IOCTL_MAGIC, 79, BHAL_IOCTL_PARMS_ST)
	
#ifdef SUPPORT_ATP_WLAN_DT
#define BHAL_IOCTL_SET_WPS_BUTTON_STATUS \
    _IOWR(BHAL_IOCTL_MAGIC, 80, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_GET_WPS_BUTTON_STATUS \
    _IOWR(BHAL_IOCTL_MAGIC, 81, BHAL_IOCTL_PARMS_ST)

#define BHAL_IOCTL_CLR_WPS_BUTTON_STATUS \
    _IOWR(BHAL_IOCTL_MAGIC, 82, BHAL_IOCTL_PARMS_ST)    
#endif

#ifdef SUPPORT_ATP_VOICE_DT
#define BHAL_IOCTL_DECT_RESET \
    _IOWR(BHAL_IOCTL_MAGIC, 83, BHAL_IOCTL_PARMS_ST)
#endif
#define BHAL_IOCTL_CMS_INITED \
    _IOWR(BHAL_IOCTL_MAGIC, 84, BHAL_IOCTL_PARMS_ST)

#ifdef SUPPORT_ATP_HYBRID
#define BHAL_IOCTL_LTE_RESET \
    _IOWR(BHAL_IOCTL_MAGIC, 85, BHAL_IOCTL_PARMS_ST)
#endif
/* START ADD by y00181549 for DTS2013101803442 at 20131129*/
#define BHAL_IOCTL_SET_WLAN_SETTING_STATUS \
    _IOWR(BHAL_IOCTL_MAGIC, 86, BHAL_IOCTL_PARMS_ST)  
/* END ADD by y00181549 for DTS2013101803442 at 20131129*/
/* START MODIFY by y00181549 for DTS2013101803442 at 20140108*/
#ifdef SUPPORT_ATP_GPIO_I2C
#define BHAL_IOCTL_I2C_INIT \
    _IOWR(BHAL_IOCTL_MAGIC, 87, BHAL_IOCTL_PARMS_ST)  
#endif
/* END MODIFY by y00181549 for DTS2013101803442 at 20140108*/

#define BHAL_IOCTL_UPG_FLAG \
    _IOWR(BHAL_IOCTL_MAGIC, 88, BHAL_IOCTL_PARMS_ST)  

/* Flash��Variable��������ݽṹ */
#define BHAL_BOOTLINE_LEN                256
#define BHAL_BOARD_ID_LEN                16
#define BHAL_BOARD_TYPE_STRING_LEN       16
#define BHAL_BOARD_NUMBER_LEN            32
#define BHAL_BOARD_PRODUCTCLASS_LEN		 32
#define BHAL_BOARD_PRODUCTCLASS_MAGIC_NUM "#PC#"
#define BHAL_BOARD_SERIAL_NUMBER_LEN     32
#define BHAL_CUSTOMER_SERIAL_NUMBER_LEN  64

#define BHAL_ASSEMBLY_STATUS_LEN         4
#define BHAL_WLAN_PWD_LEN                64
#define MIN_WLANPWD_LEN                  8
#define MAX_WLANPWD_LEN                  BHAL_WLAN_PWD_LEN-1
#define BHAL_APPINCODE_LEN               16
#define BHAL_WEP_PWD_LEN                 128

#define BHAL_USER_PWD_LEN                64

#define BHAL_USER_NAME_LEN               32
#define MIN_USERPWD_LEN                  1
#define MAX_USERPWD_LEN                  BHAL_USER_PWD_LEN-1
#define MIN_USERNAME_LEN                 1
#define MAX_USERNAME_LEN                 BHAL_USER_NAME_LEN-1

#define BHAL_OUI_LEN                     128
#define BHAL_CUSTOMER_SN1_LEN            128
#define BHAL_CWMP_USERNAME_LEN           256
#define BHAL_CWMP_PWD_LEN                256

#define BHAL_PPPOE_USER_LEN              64
#define BHAL_PPPOE_PWD_LEN               64
#define BHAL_SIP_PWD1_LEN                128
#define BHAL_FON_KEY_LEN                 128

#define BHAL_EALBLE_MAX_LEN              1536
#define BHAL_ELABEL_OFFSET               0
#define BHAL_TR064_RESETPWD_LEN          64
#define BHAL_IDENTIFIER1_LEN             128

#define BHAL_BOARD_MAKE_DATE_LEN         10
#define BCM96388_USBLEN                  22
#define RALINK_USBLEN                    11
#define TMP_BUFLEN_512                   512

#define BHAL_MAC_ADDRESS_LEN             6
#define BHAL_OBLI_MAC_ADDRESS_LEN        8
#ifdef DT_MAC_SUPPORT
#define BHAL_MAC_NUMBERS_MAX             10
#else
#define BHAL_MAC_NUMBERS_MAX             13
#endif
#define BHAL_SSID_NUM_MAX                4
#define BHAL_MAC_NUMBERS_DEFAULT         0 
#define BHAL_SYS_PARAM_OFFSET            0  /* ϵͳ������VARIABLE�����е�ƫ�Ƶ�ַ */
#define BHAL_SYS_PARAM_MAGIC             "SYSPARAM"
#define BHAL_SSID_LEN                    32
#define BHAL_EQUIP_DISABLE_WLAN_STR      "Disable"
#define BHAL_EQUIP_ENABLE_WLAN_STR       "Enable"
#define BHAL_EQUIP_DISABLE_WLAN_STR_LEN  8
#define BHAL_SEC_KEY_LEN                1024
#define BHAL_RESERVE_AREA_LEN        32*1024
#define BHAL_DECT_CALIB_PARA_LEN                148

#define MIN_SSID_LEN                     1
#define MAX_SSID_LEN           BHAL_SSID_LEN-1

#define BHAL_BOOTLOADVER_SIZE           32
#define BHAL_BOOTLOAD_VERSION           "1.0.0"
#define BHAL_SDMEMSIZE                  0x2000000
#define WLAN_WPS_NONE                0
#define APCLI_WPS_IN_AP              1  /* WPS PCB work on AP mode*/
#define APCLI_WPS_IN_CLI             2  /* WPS PCB work on CLIENT mode*/
#define APCLI_WPS_IN_APCLI           3  /* WPS PCB work on AP + CLIENT mode*/
#define APCLI_WPS_IN_AP_ACT          4
#define APCLI_WPS_IN_CLI_ACT         5 
#define APCLI_WPS_IN_APCLI_ACT       6
#define WLAN_WPS_CLI_SUCCESS         7
#define WLAN_WPS_CLI_FAILED          8
#define WLAN_WPS_AP_SUCCESS          9
#define WLAN_WPS_AP_FAILED          10

#define PIN_CODE_VALID_FLAG 0x66
/* Ŀ���CPU���ֽ��� */
typedef enum 
{     
    BHAL_BIG_ENDIAN = 0,               /* ��� */
    BHAL_LITTLE_ENDIAN,                /* С�� */   
    BHAL_CUP_BYTE_END,              /* ������ */
}ATP_BHAL_CPU_BYTE_ORDER_EN;

typedef struct BhalIoctParms
{
    char *pcString;
    char *pcBuf;
    unsigned int   ulType;
    unsigned int   ulStrLen;
    unsigned int   ulOffset;  
    int   lResult; 
} BHAL_IOCTL_PARMS_ST;

typedef struct
{
    unsigned long ulId;
    char chInUse;
    char chReserved[3];
}BHAL_MAC_ADDR_INFO_ST;

typedef enum
{
    LED_CONTROL_ENABLE = 0,			    /* enumerating */
    LED_CONTROL_DISABLE,
}BSP_LED_ENDISABLE;



#define DECT_MAGIC_CODE0            0x44
#define DECT_MAGIC_CODE1            0x6A
#define DECT_MAGIC_CODE2            0xA5
#define DECT_MAGIC_CODE3            0x5A

typedef enum tagATP_DECT_CALIB_TYPE_EN
{
    ATP_DECT_CALIB_ALL = 0,
    ATP_DECT_CALIB_RVBG,
    ATP_DECT_CALIB_RVREF,
    ATP_DECT_CALIB_RXTUN,
    ATP_DECT_CALIB_RFPI,
    ATP_DECT_CALIB_IPUI,
    ATP_DECT_CALIB_MAGIC,
    ATP_DECT_CALIB_CLIMASK = 0x80,
    ATP_DECT_CALIB_END,
}ATP_DECT_CALIB_TYPE_EN;

typedef struct
{
    unsigned char   ucDect_RVBG;                //У׼ֵ a) RVBG
    unsigned char   ucDect_RVREF;               //У׼ֵ b) RVREF
    unsigned char   ucDect_RXTUN;               //У׼ֵ c) RXTUN
    unsigned char   ucDect_ParaUnUsed;          //δʹ��
    unsigned char   aucDectRfpiCode[12];        //У׼ֵ d) RFPI
    unsigned char   aucMagicCode[4];            //Magic Code
    unsigned char   aucDect_Resrv[128];
}ATP_BHAL_DECT_CALIB_PARA_ST;

#define BHAL_LOCK_TELNETD_MASK	0x01

typedef struct
{
    char szReversebeg[32];                     //32    
    char szMagic[8];                           //8
    char szProductSN[BHAL_BOARD_NUMBER_LEN];   //32  sn
    char szSerialNumber[BHAL_BOARD_SERIAL_NUMBER_LEN]; //32 ʵ�ʴ���� barcode
    char szBaseMacAddr[BHAL_OBLI_MAC_ADDRESS_LEN];  //8           /* ��MAC��ַ */
    unsigned short usNumMacAddrs;                   //2           /* MAC��ַ�� */
    char szPad[2];                                 //2
    BHAL_MAC_ADDR_INFO_ST MacAddrsNoUsed[10];      //80
    char szCustomerSerialNumber[BHAL_CUSTOMER_SERIAL_NUMBER_LEN]; //64   /* ʵ�����豸��־��ouinum*/
    char szAssembEnd[BHAL_ASSEMBLY_STATUS_LEN]; //4
    char szSsid[BHAL_SSID_LEN];                 //32
    char szWlpwd[BHAL_WLAN_PWD_LEN];            //64
    char szAppcode[BHAL_APPINCODE_LEN];         //16
    char szWeppwd[BHAL_WEP_PWD_LEN];            //128
    char szSsid5G[BHAL_SSID_LEN];               //32
    char szWlpwd5G[BHAL_WLAN_PWD_LEN];          //64
    char szAppcode5G[BHAL_APPINCODE_LEN];       //16
    char szWeppwd5G[BHAL_WEP_PWD_LEN];          //128
    char szUsrpwd[BHAL_USER_PWD_LEN];           //64
    char szUsrName[BHAL_USER_NAME_LEN];           //32
    /* PN:  WUMING TELMEX  ���Կ���
    ��������OUI/CUSTOMER/PPPOEUSER/PPPOE PASSWORD���
    �Ͷ�Ӧ����ʾ����*/
    char szMakeDate[BHAL_BOARD_MAKE_DATE_LEN];  //10  manufacturingdate
    char szOui[BHAL_OUI_LEN];           
    char szCustomerSn1[BHAL_CUSTOMER_SN1_LEN];        
    char szCwmpUsrName[BHAL_CWMP_USERNAME_LEN];      
    char szCwmpUsrPwd[BHAL_CWMP_PWD_LEN]; 
    char szSipPwd1[BHAL_SIP_PWD1_LEN];
    char szFonKey[BHAL_FON_KEY_LEN];
    char szPppoeUser[BHAL_PPPOE_USER_LEN];  
    char szPppoePwd[BHAL_PPPOE_PWD_LEN];
    BHAL_MAC_ADDR_INFO_ST MacAddrs[BHAL_MAC_NUMBERS_MAX]; //256
    char szBoardId[32];                           //32
    char szProductClass[BHAL_BOARD_PRODUCTCLASS_LEN];	//32 ProductCLass
    char szIdentifier1[BHAL_IDENTIFIER1_LEN];

    unsigned int EquipPortProbedBitmap; // WAN: bit0 ; LAN: bit1 ~ bit4; Reserved: bit5 ~bit31;
    unsigned char szEquipDisableWlan[BHAL_EQUIP_DISABLE_WLAN_STR_LEN]; // BHAL_EQUIP_DISABLE_WLAN_FLAG ==>  "EquipDisableWlan"
    char szSecKeys[BHAL_SEC_KEY_LEN]; // sec keys
#ifdef SUPPORT_ATP_VOICE_DT
    char szReserved[BHAL_RESERVE_AREA_LEN]; //reserved��������ͨװ��������װ��������������ͨװ���Ӵ����з���
    ATP_BHAL_DECT_CALIB_PARA_ST stDectCalibPara; //����dectװ����Ҫ���κ�����¶����ᱻ����
#endif
}BHAL_VARIABLE_SYS_PARAM_ST;

typedef struct VARIABLE_EXTRA
{
    char szTr064Pwd[BHAL_TR064_RESETPWD_LEN];
    char aucELabel[BHAL_EALBLE_MAX_LEN];
}BHAL_VARIABLE_EX_PARAM_ST;


typedef struct tagBHAL_INTERFACE
{
}BHAL_INTERFACE_ST;

int bhalGetMacAddress( unsigned char *pucaMacAddr, unsigned long ulId );
int bhalReleaseMacAddress( unsigned char *pucaMacAddr );
//int bhalReadFlash(void *pszDataBuffer, unsigned long ulFlashAddr, unsigned long ulLen);
/*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
unsigned int gpio_i2c_init(void);
unsigned int gpio_i2c_read(unsigned char *buffer, unsigned int len);
unsigned int gpio_i2c_write(unsigned char *data, unsigned int len);
unsigned int gpio_i2c_read_atmel(unsigned char cmd, unsigned char *buffer, unsigned int len);
unsigned int gpio_i2c_write_atmel(unsigned char cmd, unsigned char *data, unsigned int len);
unsigned int gpio_i2c_upg(unsigned char *i2cUpgBuf, unsigned int i2cUpgLen);
int Touch_Buzzer_Sound();
int Touch_Read_Touch_Key(unsigned char *key_value);
#endif
/*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#if 1
#define BHAL_LAN_MAX_NUM  4
typedef struct tagETHERNETLED_DEV_INFO
{
     char acDevName[BHAL_WAN_INTERFACE_LEN_MAX];
     unsigned long last_rx;
     unsigned long trans_start;
     struct tagETHERNETLED_DEV_INFO *next;     
} ETHERNETLED_DEV_INFO;

typedef struct tagETHERNETLED_STAT_INFO
{
     char acLanName[BHAL_WAN_INTERFACE_LEN_MAX];
	 int  iPortNum;
     unsigned long last_rx;
	 unsigned long trans_start;
     struct tagETHERNETLED_STAT_INFO *next;     
} ETHERNETLED_STAT_INFO;

typedef struct tagINTERNETLED_DEV_INFO
{
     char acDevName[BHAL_WAN_INTERFACE_LEN_MAX];
     unsigned long last_rx;
     unsigned long trans_start;
     struct tagINTERNETLED_DEV_INFO *next;     
} INTERNETLED_DEV_INFO;

typedef struct tagWLANLED_DEV_INFO
{
     char acDevName[BHAL_WAN_INTERFACE_LEN_MAX];
     unsigned int ledDeactive;
	 unsigned int connectNum;
     unsigned long rx_packets;
     unsigned long tx_packets;
     struct tagWLANLED_DEV_INFO *next;     
} WLAN_DEV_INFO;
#endif

#define WLAN_OK 0
#define WLAN_NOK 1
#define WLAN_DISABLE 2

#define LED_TIME_INFINITE 		0xFFFFFFFF

#ifdef __cplusplus
}
#endif

#endif /* _BHAL_H */

