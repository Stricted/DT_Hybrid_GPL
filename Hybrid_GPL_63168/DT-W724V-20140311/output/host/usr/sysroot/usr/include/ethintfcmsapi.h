#ifndef __ETHINTFCMS_API_H__
#define __ETHINTFCMS_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

//#define ATP_DEBUG
#ifdef ATP_DEBUG
#define ATP_ETHINTF_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_ETHINTF_DEBUG(x...)
#endif

#define     ETH_MAC_ADDR_LEN                                32
#define     ETH_IFC_NAME_LEN                                16
#define     ETH_PORT_MAX_NUM                                5
#define     ETH_PORT_MAX_LAN_NUM                       4
#define     ETH_CMD_LEN                                     256

typedef struct 
{    
    VOS_UINT32 ulEthIntfInstId;
    VOS_BOOL   bEnable;                          /* 端口禁用或启用       */
    VOS_UINT32 ulStatus;                         /* 连接状态 UP/DOWN     */            
    VOS_CHAR   acMacAddress[ETH_MAC_ADDR_LEN];   /* 接口物理地址         */ 
    VOS_CHAR acInfName[ETH_IFC_NAME_LEN];//eth0.x
    VOS_UINT32 ulLastChange;                     /*上一次状态改变*/
    VOS_BOOL   bMACAddressControlEnable;         //tr064 macaddress control enable
}ETH_INTERFACE_CFG_ST;


extern ETH_INTERFACE_CFG_ST g_astEthItfCfg[ETH_PORT_MAX_NUM];

//VOS_UINT32 ATP_ETH_Init(VOS_VOID);
#if defined(__cplusplus)
}
#endif

#endif

