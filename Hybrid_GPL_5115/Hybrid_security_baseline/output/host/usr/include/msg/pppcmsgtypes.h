#ifndef __ATP__PPPCMSGTYPES_H__
#define __ATP__PPPCMSGTYPES_H__

#define ATP_CBBID_PPPC_NAME "pppc"


/* �ӿ������� */
#define ATP_PPPC_IFC_LEN  32
/* �ӿ������� */
#define ATP_PPPC_DEV_LEN  32
/* �û������� */
#define ATP_PPPC_USER_LEN  68
/* ���볤�� */
#define ATP_PPPC_PASSWD_LEN  68
/* ���б��� */
#define ATP_PPPC_BIND_LEN  64
/*AT����ָ���*/
#define ATP_PPPC_DIALNUMBER_LEN  32
/*APN���ó���*/
#define ATP_PPPC_SETAPN_LEN  32
/* DNS�б��� */
#define ATP_PPPC_DNS_LEN  64
/*start of ATP 2008.01.17 c60023298 �޸� for AU4D00330 �ļ���Ϣ�������ȡ���Ȳ�һ�� */
/* ���һ������ʧ����Ϣ���� */
#define ATP_PPPC_ERROR_LEN  64
/*end of ATP 2008.01.17 c60023298 �޸� for AU4D00330 �ļ���Ϣ�������ȡ���Ȳ�һ�� */
/* IP��ַ���� */
#define ATP_PPPC_IPADDR_LEN  16

#define WAN_PPP_SESSION_LEN 32

#define ATP_PPPOE_SERVICENAME_LEN  256
#define ATP_PPPOE_ACNAME_LEN  256

/* MAC��ַ���� */
#define ATP_PPPC_MAC_LEN  32
/* �������� */
#define ATP_PPPC_SERVICELIST_LEN  32



/* ������ */
typedef enum
{
    ATP_PPPC_OK = 0,
    /* ����PPPC��������ʼֵ */
    ATP_ERR_PPPC_START = 0x0820,
    /* ����У����� */
    ATP_ERR_PPPC_CHECK_FAILED,
    /* �����ļ����� */
    ATP_ERR_PPPC_CONFIG_FAILED,
    /* ���������� */
    ATP_ERR_PPPC_ALREADY_EXIST,
    /* ���̲����� */
    ATP_ERR_PPPC_NO_EXIST,
}ATP_ERR_PPPC_EN;

/* ���Ӵ�������Ҫ��WANģ�鱣��һ�� */
typedef enum
{
    /* �ȴ� */
    ATP_PPPC_TRIGGER_IDLE = 0,
    /* ���� */
    ATP_PPPC_TRIGGER_DISCONNECTING = 1,
    /* �ֶ� */
    ATP_PPPC_TRIGGER_MANUAL = 2,
    /* �Զ� */
    ATP_PPPC_TRIGGER_ALWAYSON = 3,
    /* ���貦�� */
    ATP_PPPC_TRIGGER_ONDEMAND = 4,
    /* ���ֵ */
    ATP_PPPC_TRIGGER_MAX
}ATP_PPPC_TRIGGER_EN;

/* PPPC����״̬ */
typedef enum
{
    /* δ��ʼ�� */
    ATP_PPPC_STATUS_UNCONFIGURED,
    /* �������� */
    ATP_PPPC_STATUS_CONNECTING,
    /* ������֤ */
    ATP_PPPC_STATUS_AUTHENTICATING,
    /* ������ */
    ATP_PPPC_STATUS_CONNECTED,
    /* δ���� */
    ATP_PPPC_STATUS_PENDING,
    /* ���ڶϿ� */
    ATP_PPPC_STATUS_DISCONNECTING,
    /* �ѶϿ� */
    ATP_PPPC_STATUS_DISCONNECTED 
}ATP_PPPC_STATUS_EN;

/* Э������ */
typedef enum
{
    /* PPPOE���� */
    ATP_PPPC_PROTOCOL_PPPOE,
    /* PPPOA��LLCģʽ */
    ATP_PPPC_PROTOCOL_PPPOA_LLC,
    /* PPPOA��VCģʽ */
    ATP_PPPC_PROTOCOL_PPPOA_VC,
    /* PPP���� */
    ATP_PPPC_PROTOCOL_PPP_DIALUP,
    /* PPPOE2A */
    ATP_PPPC_PROTOCOL_PPPOE2A,
    /*L2TP�����PPP����*/
    ATP_PPPC_PROTOCOL_L2TP,
}ATP_PPPC_PROTOCOL_EN;

/* ��֤��ʽ */
typedef enum
{
    /* AUTO */
    ATP_PPPC_AUTH_AUTO,
    /* PAP */
    ATP_PPPC_AUTH_PAP,
    /*CHAP */
    ATP_PPPC_AUTH_CHAP,
}ATP_PPPC_AUTH_EN;

#ifdef SUPPORT_ATP_IPV6
/* IPV6��ַ���÷�ʽ */
typedef enum
{
    /* Disable */
    ATP_PPPC_IPV6_ADDR_TYPE_DISABLE,
    /* SLAAC */
    ATP_PPPC_IPV6_ADDR_TYPE_SLAAC,
    /* DHCP */
    ATP_PPPC_IPV6_ADDR_TYPE_DHCP,
    /* Static */
    ATP_PPPC_IPV6_ADDR_TYPE_STATIC,
}ATP_PPPC_IPV6_ADDR_TYPE_EN;

/* IPV6��ַ����ѡ�� */
typedef enum
{
    /* IANA + IAPD */
    ATP_PPPC_IPV6_ADDR_OPTION_BOTH,
    /* IANA */
    ATP_PPPC_IPV6_ADDR_OPTION_IANA,
    /* IAPD */
    ATP_PPPC_IPV6_ADDR_OPTION_IAPD,
    /* ����Ӧ���ݲ�֧�� */
    ATP_PPPC_IPV6_ADDR_OPTION_AUTO,
}ATP_PPPC_IPV6_ADDR_OPTION_EN;
#endif


/* PPPOA QOS */
typedef struct
{
    unsigned int ulATMQoS;
    unsigned int ulATMPeakCellRate;
    unsigned int ulATMMaximumBurstSize;
    unsigned int ulATMSustainableCellRate;
}ATP_PPPC_ATMQOS_ST;

/* ���ò����ṹ */
typedef struct
{
    /* PPP�ӿ��� */
    char acInterface[ATP_PPPC_IFC_LEN];
    /* PPP�ӿ��� */
    char acDevice[ATP_PPPC_DEV_LEN];
#ifdef SUPPORT_ATP_PPP_TOGGLE_DT_W723V
    char acDevice_bak[ATP_PPPC_DEV_LEN];
#endif
    /* �����û��� */
    char acUserName[ATP_PPPC_USER_LEN];
    /* ���� */
    char acPassWord[ATP_PPPC_PASSWD_LEN];
    /* ���б� */
    char acBindList[ATP_PPPC_BIND_LEN];
    /* �󶨹��ܱ�ʶ */
    unsigned char ucBindEnable;
    /* �����ܱ�ʶ */
    unsigned char ucProxyEnable;
    /* ��������û��� */
    unsigned char ucProxyNum;
    /* Э������ */
    unsigned char ucProtocol;
    /* ���Կ��� */
    unsigned char ucDebugEnable;
    /* ���� */
    unsigned char aucReserved[3];
    /* PPPOA QOS */
    ATP_PPPC_ATMQOS_ST stAtmQos;
    char acPppSessid[WAN_PPP_SESSION_LEN];
	/************ATPV1R2C03 �·���������д������****************/
    /*��֤��ʽ*/
    unsigned int ulAuth;
    /*���貦�ſ���ʱ��*/
    unsigned int ulIdleTime;
         /*B300 ����*/
    unsigned int ulRandomDialInterval;  //PPPOE��ʼ����ǰ����ȴ�ʱ������ֵ��
    unsigned int ulRetryDialInterval;  //PPPOE����ʧ�ܺ�ȴ���ʱ������
    unsigned int ulRetrySendInterval;  //PPPOE�ط����ĵ�ʱ����
    unsigned int ulKeepAliveInterval;  //LCP����ʱ����
    unsigned int ulRetryTimes; //LCP��������ʧ�ܴ����޸�
#ifdef SUPPORT_ATP_WANUMTS
	/*AT����ָ��*/
	char acDialNumber[ATP_PPPC_DIALNUMBER_LEN];
	/*����APN*/
	char acSetApn[ATP_PPPC_SETAPN_LEN];
#endif
    unsigned int ulMaxMru;
#ifdef SUPPORT_ATP_MTU
    /* ���MTU */
    unsigned int ulMaxMtu;
#endif
    int iReqTime;
     /************ATPV1R2C03 �·���������д������****************/
     /*VHG���� pppoe service name*/
    char acPppoeServiceName[ATP_PPPOE_SERVICENAME_LEN];
    char acPppoeAcName[ATP_PPPOE_ACNAME_LEN];
    /* ��̫������PPPOA��MAC��ַ */
    char acPppoe2aMacAddr[ATP_PPPC_MAC_LEN];
    /* ��̫������PPPOA�ĻỰID */
    unsigned int ulPppoe2aSid;
#ifdef SUPPORT_ATP_IPV6
    /* �Ƿ�����IPv4���� */
    unsigned int ulIpv4;
    /* �Ƿ�����IPv6���� */
    unsigned int ulIpv6;
    /* IPv6�����ַ���ͣ���IANA��IAPD */
    unsigned int ulManagedAddress;
#endif
    unsigned int ulPvcSearchTime;
    char acServiceList[ATP_PPPC_SERVICELIST_LEN];
#ifdef SUPPORT_ATP_PPP_STATICIP
    char acRequestIP[ATP_PPPC_IPADDR_LEN];
#endif
#ifdef SUPPORT_ATP_DT_WAN_PAGE     
    char acRequestPriDns[ATP_PPPC_IPADDR_LEN]; 
    char acRequestSekDns[ATP_PPPC_IPADDR_LEN]; 
    unsigned int ulEffDemandFlag;
    unsigned int ulIspFlag;
    unsigned int ulDefaultUPFlag;
#endif
}ATP_PPPC_CONFIG_ST;

#define PPPC_IFC_LEN (64)
typedef struct tagATP_PPPC_STATUSCHG_MSG_ST
{
    VOS_UINT32 ulStatus;
    VOS_CHAR   acIntfName[PPPC_IFC_LEN];
}ATP_PPPC_STATUSCHG_MSG_ST;

enum ATP_MSGTYPES_EN
{
    ATP_MSG_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_PPPC),
    ATP_MSG_START_DHCP6C_PRO,
    ATP_MSG_ONDEMAND_READY_ROUTE,
    ATP_MSG_PPPC_STATUS_CHANGE,
    ATP_MSG_CMS_CONFIRM_VLAN_INF,
    ATP_MSG_PPPC_VLAN_FORCE_CHANGE,
/* <DTS2012082701764 w00211169 2012-9-4 begin */
    ATP_MSG_ONDEMAND_IPV6_DNS_TRIGGER,
/* DTS2012082701764 w00211169 2012-9-4 end> */
} ;

#endif // End of __ATP__MSGTYPES_H__

