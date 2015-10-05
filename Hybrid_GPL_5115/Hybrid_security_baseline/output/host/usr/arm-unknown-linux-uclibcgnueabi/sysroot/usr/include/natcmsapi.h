/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file natcmsapi.h
  *NAT ģ�鴦����
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author h00170023
  *\date  2012-2-6
*/


#ifndef __ATP_NAT_API_H__
#define __ATP_NAT_API_H__

#include "atptypes.h"
#include "atputil.h"
//#include "atperrdef.h"
#include "msgapi.h"
#include "cfmapi.h"

//#define ATP_DEBUG 1

#ifdef ATP_DEBUG
#define ATP_CMD(x)  {printf("%s \r\n", (x));\
    ATP_Exec(x);}
#define NATCMS_DEBUG(format, args...)          printf("<DEBUG> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#define NATCMS_ERROR(format, args...)          printf("<ERROR> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#else
#define ATP_CMD(x)  ATP_Exec(x)
#define NATCMS_DEBUG(format, args...)
#define NATCMS_ERROR(format, args...)          printf("<ERROR> ---> FILE: %s LINE: %d: ", __FILE__, __LINE__); printf(format, ## args); printf("\n")
#endif


#define ATP_FIREWALL_CONNTRACK_NUM    1000
#define ATP_NAT_CMD_LEN       256

#define ATP_SYS_PORT_MAX                    65535
#define ATP_SYS_PORT_MIN                    1

#define KRNL_STR_2_6_8        "2.6.8.1"
#define KRNL_STR_2_6_20       "2.6.20"
#define KRNL_STR_2_6_21_5     "2.6.21.5"
#define KRNL_STR_2_6_30       "2.6.30"


#define KRNL_CT_PATH_2_6_20       "/lib/kernel/net/netfilter"
#define KRNL_NAT_PATH_2_6_20      "/lib/kernel/net/ipv4/netfilter"
#define KRNL_CT_NAT_PATH_2_6_8    "/lib/kernel/net/ipv4/netfilter"

#define CT_CLEAN_PATH_2_6_8       "/proc/sys/net/ipv4/netfilter/ip_conntrack_dns"
#define CT_CLEAN_PATH_2_6_20      "/proc/sys/net/ipv4/netfilter/ip_ct_clean"

//DTҪ����port forwarding��AlwaysOnʱ��V4���ܵ��ߣ������ļ�������־
//����ȫ��ɾ��ʱ��ɾ�����ļ�
#ifdef SUPPORT_ATP_PPP_DYN_DIAL
#define V4_PORT_FORWARDING_FILE    "/var/port_forwarding"
#endif

typedef enum
{
    KRNL_2_6_8 = 0,
    KRNL_2_6_20 = 1,
    KRNL_2_6_21_5 = 2,
    KRNL_2_6_30 = 3,
    KRNL_MAX = 9,   
} ATP_KRNL_VERSION;

extern ATP_KRNL_VERSION g_enKrnlVersion;
extern VOS_CHAR g_acKrnlCtPath[ATP_SYS_MEDIUM_LEN];
extern VOS_CHAR g_acKrnlNatPath[ATP_SYS_MEDIUM_LEN];
extern VOS_CHAR  g_acCtCleanPath[ATP_SYS_MEDIUM_LEN];

/*!
  \brief Natģ���ʼ������
  \param[in]  *pcWanPath: ���õ�WAN Path
  \param[in]  *pvHook: ���õĹҹ�����
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_Init(VOS_VOID *pcWanPath, VOS_VOID *pvHook);

/*!
  \brief NAT�������Գ�ʼ������DMZ,PORTMAP,PORTTRIGER
  \param[in]  *pcWanPath: 
  \param[in]  *pvHook: 
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_Other_Init(VOS_VOID *pcWanPath, VOS_VOID *pvHook);

/*!
  \brief ����WAN��NAT����
  \param[in]  pszPath: ��Ҫ����NAT���ܵ�WAN��·��
  \param[in]  *pvHook: ���ҵı��ò���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_WanStart(VOS_VOID* pszPath, VOS_VOID *pvHook);

/*!
  \brief ֹͣWAN��NAT����
  \param[in]  pszPath: ��ҪֹͣNAT���ܵ�WAN��·��
  \param[in]  *pvHook: ���ҵı��ò���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_WanStop(VOS_VOID* pszPath, VOS_VOID *pvHook);

/*!
  \brief ����ڴ��б����nat����
  \param[in]  pszPath: ��Ҫɾ��NAT���ܵ�WAN��·��
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_WanClear(VOS_VOID* pszPath,VOS_VOID *pvHook);

/*START ADD: by h00170023 at 20110107 for ct clean*/
/*!
  \brief ������Ӹ���
  \param[in]  pszPath: ��Ҫ����NAT���ܵ�WAN��·��
  \param[in]  *pvHook: ���ҵı��ò���
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_CTclean(VOS_VOID* pszPath, VOS_VOID *pvHook);
/*END ADD: by h00170023 at 20110107 for ct clean*/

/*!
  \brief PortMapping ��Ϣ������
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_PortMapMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);

/*!
  \brief DMZ��Ϣ�ַ�������
  \param[in]  *pstMsg: 
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_DmzMsgProc(VOS_VOID *pstMsg);

/*!
  \brief �ض���telnet�˿ڴ�����
  \param[in]  usChangePort: 
  \param[in]  usChangedPort: 
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_ChgTelnetPort(VOS_UINT16 usChangePort, VOS_UINT16 usChangedPort);

/*!
  \brief �ض���ftp�˿�
  \param[in]  usChangePort: 
  \param[in]  usChangedPort: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_NAT_ChgFtpPort(VOS_UINT16 usChangePort, VOS_UINT16 usChangedPort);

/*!
  \brief LAN��br+�ӿ���Ϣ�ı����Ҫͬ���޸�mac�����б���ͨ���Ĺ���nat��masqurede����url������ȱʡ�������ʣ�ftp�е���Ч����
  \param[in]  pszNewLanIp: ���ĺ������ip
  \param[in]  pszNewSubnetMask: ���ĺ��������������
  \param[in]  pszOldLanIp: ����ǰ������ip 
  \param[in]  pszOldSunnetMask: ����ǰ��������������
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_LanChange(VOS_CHAR* pszNewLanIp, VOS_CHAR* pszNewSubnetMask, 
                                   VOS_CHAR* pszOldLanIp, VOS_CHAR* pszOldSunnetMask);
/*!
  \brief ��ȡPortMa�Ľ�����
  \param[in]  pszPath: ���WAN��·��
  \param[in]  pulNum: Ҫ��ȡ�Ľڵ����
  \return 
  \retval VOS_OK ִ�гɹ�
*/
VOS_UINT32 ATP_NAT_PMGetNodeNums(VOS_CHAR* pszPath, VOS_UINT32* pulNum);

/*!
  \brief ��Nat�µ�IGMP Proxy����
  \param[in]  bEnable: IGMP Proxyʹ��
  \param[in]  pszInterface: Ҫ���õ�WAN�ӿ�
  \return 
*/
VOS_VOID ATP_NAT_ServiceIgmp(VOS_BOOL bEnable, VOS_CHAR* pszInterface);

/*!
  \brief Nat�����ں�ģ��
  \param[in] VOS_VOID: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_LoadModule(VOS_VOID);

/*!
  \brief PortMapping У��
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_PortMapCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);

/*!
  \brief PortMapping ɾ��һ��ʵ��
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_PortMapDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
/*!
  \brief DMZ����޸�һ��ʵ��
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_DmzAddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                         VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                         VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                         VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjIdj);

/*!
  \brief DMZ У��
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_DMZCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara);

#ifdef SUPPORT_ATP_MULTINAT
/*!
  \brief ��NATģ��У�麯��
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_MNatCheck(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                            VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                            VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                            VOS_UINT32 ulNumOfPara, VOS_BOOL bSetObj);
/*!
  \brief ��NATģ������޸���Ϣ������
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \param[in]  *pulOldValues: 
  \param[in]  *pulSetValues: 
  \param[in]  *pulChgParas: 
  \param[in]  ulNumOfPara: 
  \param[in]  ulNextObjId: 
  \param[in]  bSetObj: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_MNatAddSetMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                VOS_UINT32 ulObjId, const VOS_UINT32 *pulOldValues,
                                VOS_UINT32 *pulSetValues, const VOS_UINT32 *pulChgParas,
                                VOS_UINT32 ulNumOfPara, VOS_UINT32 ulNextObjId, VOS_BOOL bSetObj);
/*!
  \brief ��NATģ��ɾ����Ϣ������
  \param[in]  aucInstId[ATP_CFM_MAX_TREE_DEPTH]: 
  \param[in]  ulObjId: 
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_UINT32 ATP_NAT_MNatDelMsgProc(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH],
                                     VOS_UINT32 ulObjId);
#endif

/* start of by d00107688 for UPnP PortMapping  2009-06-11 */
#ifdef SUPPORT_ATP_UPNP

/*!
  \brief UPNP PortMapping��Ϣ������
  \param[in]  pstMsg: ���ܵ�����Ϣ
  \return 
  \retval VOS_OK ִ�гɹ�
  \retval VOS_NOK ִ��ʧ��
*/
VOS_VOID  ATP_NAT_PORTMAPPING_UPNP_MSG_Process(const ATP_MSG_HEADER_ST* pstMsg);

#endif
/* end of by d00107688 for UPnP PortMapping  2009-06-11 */

//w724v ʹ��tr181, natֻ��enable,disable����ֵ��dtֻҪ��CONE_NAT
#ifdef SUPPORT_ATP_TR181
VOS_UINT8 ATP_NAT_GetEnableByPath(const VOS_CHAR *pszPath);
#endif

#endif

