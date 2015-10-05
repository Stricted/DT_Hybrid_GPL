/************************************************************************* 
 *  ��Ȩ����:   2007-2020, ��Ϊ�������޹�˾
 *  �ļ���:     ethapi.h
 *  ����:       ������42304
 *  �汾:       1.0
 *  �������:   03/21/2009
 *  ����:       �����ն����ƽ̨ATP ETH��֯ģ��
 *
 *  ��Ҫ�����б�:
 *
 *  �޸���ʷ��¼�б�:
 *  1.����: 03/11/2009
 *    ����: ������42304
 *    �޸�: ��ɳ���
***************************************************************************/
#ifndef __ETH_API_H__
#define __ETH_API_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define     ETH_INTERFACE_NAME                              "eth0"
#define     ETH_STATUS_LEN                                  16
#define     ETH_MAC_ADDR_LEN                                32
#define     ETH_CMD_LEN                                     256
#define     ETH_IFC_NAME_LEN                                16

#define     ETH_PORT_MAX_NUM                                4


#define     ETH_PORT_1_UP                                   8
#define     ETH_PORT_2_UP                                   4 
#define     ETH_PORT_3_UP                                   2
#define     ETH_PORT_4_UP                                   1

#define     LAN_DEVICE_INSTID                               1

#define     ATP_ETH_LINK_STATE                              "/var/ethlinkstate"
#define     ATP_ETH_PATH_LEN                                128


typedef enum tagETH_LINK_STATE_EN
{
    ETH_LINKSTATE_DOWN = 0,
    ETH_LINKSTATE_UP,
    ETH_LINKSTATE_ERROR    
}ETH_LINK_STATE_EN;

typedef struct 
{    
    VOS_UINT32 ulEthDevInstId;
    VOS_UINT32 ulEthItfPortInstId;
    VOS_BOOL   bEnable;                          /* �˿ڽ��û�����       */
    VOS_UINT32 ulStatus;                         /* ����״̬ UP/DOWN     */            
    VOS_CHAR   acMacAddress[ETH_MAC_ADDR_LEN];   /* �ӿ������ַ         */ 
   VOS_CHAR acInfName[ETH_IFC_NAME_LEN];//eth0.x
}ETH_INTERFACE_CFG_ST;

extern ETH_INTERFACE_CFG_ST g_astEthItfCfg[ETH_PORT_MAX_NUM];


VOS_UINT32 ATP_ETH_Init(VOS_VOID);
VOS_UINT32 ATP_ETH_UnInit(VOS_VOID);
VOS_UINT32 ATP_ETH_GetInterface(VOS_CHAR* pszPath, VOS_UINT32 ulIfcLen, VOS_CHAR* pszInterface);
VOS_UINT32 ATP_ETH_GetInterfaceAlias(VOS_CHAR* pszIntAlias, VOS_CHAR* pszInterface);
VOS_UINT32 ATP_ETH_GetInterfacePortNum(VOS_CHAR* pszInterface);

VOS_BOOL ATP_ETH_ValidateSpeed(const VOS_CHAR *pszSpeed);
VOS_UINT32 ATP_ETH_ValidateDuplex(const VOS_CHAR *pszDuplex);
VOS_UINT32 ATP_ETH_GetNextInfo(VOS_UINT32 *pulLanDeviceId,VOS_UINT *pszLanItfCfgId,VOS_CHAR *pszLanIfcType,VOS_CHAR *pszLanIfcRef);

VOS_UINT32 ATP_ETH_GetCfg(ETH_INTERFACE_CFG_ST* pstInfCfg, const VOS_CHAR *pszLanIfcName);
VOS_UINT32 ATP_ETH_UpdateCfg(ETH_INTERFACE_CFG_ST pstInfCfg, const VOS_CHAR *pszLanIfcName);
#if defined(__cplusplus)
}
#endif

#endif





