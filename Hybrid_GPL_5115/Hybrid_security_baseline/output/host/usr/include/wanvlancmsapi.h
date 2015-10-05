#include "atptypes.h"

#ifndef __WANVLAN_CMS_API__
#define __WANVLAN_CMS_API__

//#define ATP_DEBUG 1
#ifdef ATP_DEBUG
#define ATP_VLAN_DEBUG(format, args...) printf("VLAN: file[%s] line[%d] func[%s]: " format "\r\n", __FILE__, __LINE__,__FUNCTION__, ## args)
#define ATP_VLAN_CMD(acCmd)  {printf("VLAN: file[%s] line[%d] func[%s]: [%s] \r\n", __FILE__, __LINE__,__FUNCTION__, acCmd); ATP_Exec(acCmd);}
#else
#define ATP_VLAN_DEBUG(format, args...)
#define ATP_VLAN_CMD ATP_Exec
#endif

typedef struct tagATP_VLAN_CONFIG_ST
{
    /*下一节点的指针*/
    struct tagATP_VLAN_CONFIG_ST* pstNext;
    VOS_UINT32 ulConfId;
    VOS_CHAR acPath[ATP_BUFFER_1024];
    VOS_CHAR acName[ATP_BUFFER_64];
    VOS_UINT32 ulVlanId;
    VOS_UINT32 ulPriority;
    VOS_CHAR acLowerLayers[ATP_BUFFER_1024];
} ATP_VLAN_CONFIG_ST;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

VOS_UINT32 ATP_VLANCONF_Init(VOS_VOID);

VOS_UINT32 ATP_VLANCONF_GetLinkName(VOS_CHAR* pcPath, VOS_CHAR* pszName, VOS_UINT32 ulLen);

ATP_VLAN_CONFIG_ST* ATP_VLANCONF_GetInstance(VOS_CHAR* pcPath);

VOS_UINT32 ATP_VLANCONF_Clear(VOS_UINT32 ulWdInstId, VOS_UINT32 ulWcdInstId);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
