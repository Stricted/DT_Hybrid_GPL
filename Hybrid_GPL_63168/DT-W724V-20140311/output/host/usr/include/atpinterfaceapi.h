/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpinterfaceapi.h
  * TR181 Interface Object API
  *\copyright 2007-2020，华为技术有限公司
  *\author z65940
  *\date  2012-4-17
*/
#ifndef __ATP_ATPINTERFACEAPI_H__
#define __ATP_ATPINTERFACEAPI_H__

#include "atptypes.h"
#include "cfmapi.h"
#include "cmsapi.h"

#include "intfstatus.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define INTERFACE_LOWERLAYERS_MAX_LEN_D     (1024 + 4)

typedef enum tagATP_INTF_RET_CODE_EN
{
    ATP_INTF_RET_INTERNAL_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_INTERFACECORE),
    ATP_INTF_RET_PARA_ERR,
    ATP_INTF_RET_MEM_ERR,

    ATP_INTF_RET_INVALIE_LOWER_LAYER,
} ATP_INTF_RET_CODE_EN;


/*!<
  *     TR181 Common Parameters
  */

/*!<
 *  According to TR181 Architecture, each interface object should have the following parameters:
 *      Enable
 *      Status
 *      Alias
 *      Name
 *      LastChange
 *      LowerLayers
 *
 *      We expect that all the Interface Object start with these common paramters,
 *          so we can provide common functions for all interfaces
 *      But some of the Interface, such as Device.IP.Interface.{i}. does not start with these parameters,
 *          so we should provide this map to do the same action
 */

typedef enum tagATP_INTERFACE_COMMON_PARA_EN
{
    ATP_INTERFACE_ENABLE,
    ATP_INTERFACE_STATUS,
    ATP_INTERFACE_ALIAS,
    ATP_INTERFACE_NAME,
    ATP_INTERFACE_LASTCHANGE,
    ATP_INTERFACE_LOWERLAYERS
} ATP_INTERFACE_COMMON_PARA_EN;

//!\brief Interface type
/*!
 *
 */
typedef enum tagATP_INTF_TYPE_EN
{
    ATP_INTF_TYPE_UNKNOWN,
    ATP_INTF_TYPE_PPP,
    ATP_INTF_TYPE_ETHINTF,
    ATP_INTF_TYPE_ETHLINK,
    ATP_INTF_TYPE_ETHVLAN,
    ATP_INTF_TYPE_BRPORT,    
    ATP_INTF_TYPE_IP,
    ATP_INTF_TYPE_DSLCHANNEL,
    ATP_INTF_TYPE_DSLLINE,
    ATP_INTF_TYPE_DSLBOND,
    ATP_INTF_TYPE_ATM,
    ATP_INTF_TYPE_PTM,
    ATP_INTF_TYPE_WFSSID,
    ATP_INTF_TYPE_WFRADIO,
    ATP_INTF_TYPE_USB,
    ATP_INTF_TYPE_UPA,
    ATP_INTF_TYPE_HOMEPLUG,
    ATP_INTF_TYPE_MOCA,
    ATP_INTF_TYPE_HPNA,
	/* <DTS2013081901325 xiazhongyue 20130819 begin */
    ATP_INTF_TYPE_LTE,
	/* DTS2013081901325 xiazhongyue 20130819 end>   */
    //Add more after here
} ATP_INTF_TYPE_EN;

//!\brief Interface stream type
/*!This interface is wan intf or lan intf or unknown
 *
 */
typedef enum tagATP_INTF_STREAM_TYPE_EN
{
    ATP_INTF_STREAM_UNKNOWN,
    ATP_INTF_STREAM_WAN,
    ATP_INTF_STREAM_LAN,
} ATP_INTF_STREAM_TYPE_EN;

typedef struct tagATP_INTERFACE_TYPE_ST
{
    ATP_INTF_TYPE_EN        enIntfType;
    VOS_CHAR                *pcDomain;
} ATP_INTERFACE_TYPE_ST;

typedef struct tagATP_INTERFACE_CMO_MAP_ST
{
    VOS_UINT32          aulCmoMaps[ATP_INTERFACE_LOWERLAYERS + 1];
} ATP_INTERFACE_CMO_MAP_ST;

/*!<
 *  According to TR181 Architecture, each interface object should provide the following statistics parameters:
 *      BytesSent
 *      BytesReceived
 *      PacketsSent
 *      PacketsReceived
 *      ErrorsSent
 *      ErrorsReceived
 *      UnicastPacketsSent
 *      UnicastPacketsReceived
 *      DiscardPacketsSent
 *      DiscardPacketsReceived
 *      MulticastPacketsSent
 *      MulticastPacketsReceived
 *      BroadcastPacketsSent
 *      BroadcastPacketsReceived
 *      UnknownProtoPacketsReceived
 */
typedef struct tagATP_INTERFACE_STATS_ST
{
    
} ATP_INTERFACE_STATS_ST;

//! Forward declaration for Interface Object Instance
typedef struct tagATP_INTERFACE_ST ATP_INTERFACE_ST;

typedef struct tagATP_INTERFACE_DESCRIPTOR_ST ATP_INTERFACE_DESCRIPTOR_ST;

//! Interface Object Instance Destructor function
typedef VOS_VOID (*AtpInterfaceDestructor)(ATP_INTERFACE_ST *pstInterface);

//! Interface Object Descriptor Destructor function
typedef VOS_UINT32 (*AtpInterfaceDescriptorIterateProc)(const ATP_INTERFACE_DESCRIPTOR_ST *pstDescriptor, VOS_UINT32 ulOID);

//! Stats
typedef VOS_UINT32 (*AtpInterfaceStatsGetProc)(const ATP_INTERFACE_ST *pstInterface, VOS_UINT32 *pulGetObj);
typedef VOS_UINT32 (*AtpInterfaceStatsClearProc)(ATP_INTERFACE_ST *pstInterface);
typedef VOS_UINT32 (*AtpInterfaceInitProc)(ATP_INTERFACE_ST *pstInterface);
typedef VOS_UINT32 (*AtpInterfaceSetupProc)(ATP_INTERFACE_ST *pstInterface, ATP_INTERFACE_ST *pstLowerLayer);
typedef VOS_BOOL   (*AtpInterfaceLowerLayerCheckProc)(const ATP_INTERFACE_DESCRIPTOR_ST *pstDescriptor, const ATP_INTERFACE_DESCRIPTOR_ST *pstUpLayerDescriptor);

typedef VOS_UINT32 (*AtpInterfaceStatusUpdateProc)(ATP_INTERFACE_ST *pstInterface, ATP_INTERFACE_ST *pstLowerLayer);


struct tagATP_INTERFACE_DESCRIPTOR_ST
{
    const ATP_INTERFACE_CMO_MAP_ST          *pstCmoMap;     // Some CMO are not in the same order

    //! Interface name prefix shown by ifconfig
    const VOS_CHAR                          *pcIfNamePrefix;

    //! Init proc, invoked when system up or created a new interface
    AtpInterfaceInitProc                    pfInit;

    //! Setup proc, invoked from low layer to up layer
    AtpInterfaceSetupProc                   pfSetup;

    //! Status change proc
    AtpInterfaceStatusUpdateProc            pfStatusUpdate;

    //! Check if lower layer is OK
    AtpInterfaceLowerLayerCheckProc         pfLowerLayerCheck;

    //! Configuration proc
    CmsDelObjProc                           pfDelProc;
    CmsAddSetObjProc                        pfAddSetProc;
    CmsCheckObjProc                         pfCheckProc;

	CmsCheckSingleObjProc                   pfCheckProcSingleObj;
    CmsSetSingleObjProc                     pfSetProcSingleObj;

    /*!<
         *      Enable
         *      Status
         *      Alias
         *      Name
         *      LastChange
         *      LowerLayers
         */
    // Enable/Disable

    //! Lower Layer Status Change Event Proc

    //! Stats proc
    AtpInterfaceStatsGetProc                pfStatsGetProc;
    AtpInterfaceStatsClearProc              pfStatsClearProc;

    //! Destroy interface object instance
    AtpInterfaceDestructor                  pfDestructor;

    VOS_UINT32                              ulMaxInstNum;
} ;

// General Stats object interface for 
VOS_UINT32 ATP_INTF_InterfaceDescriptorRegister(const VOS_CHAR *pcDomain, const ATP_INTERFACE_DESCRIPTOR_ST *pstIntfDescriptor);

const ATP_INTERFACE_DESCRIPTOR_ST *ATP_INTF_InterfaceDescriptorFindByOID(VOS_UINT32 ulOID);

VOS_VOID ATP_INTF_InterfaceDescriptorIterate(AtpInterfaceDescriptorIterateProc pfIterateProc);

/*!<
 *  Interface Object Instances
 */
typedef struct tagATP_LOWERLAYER_LIST_ST
{
    struct tagATP_LOWERLAYER_LIST_ST    *pstNext;
    ATP_INTERFACE_ST                    *pstInterface;
    //TODO: 增加标记变量，保证迭代ATP_INTF_LowestInterfaceIterate调用时每条链路只执行一次
} ATP_LOWERLAYER_LIST_ST;

struct tagATP_INTERFACE_ST
{
    ATP_INTERFACE_ST            *pstNext;

    // Internal Params
    VOS_UINT32                  ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    VOS_UINT32                  ulObjID;
    VOS_UINT32                  *pulObj;
    VOS_UINT32                  ulIfIdx;
    const ATP_INTERFACE_DESCRIPTOR_ST *pstDescriptor;
    ATP_INTF_STREAM_TYPE_EN     enStreamType;//this interface is belong to lan or wan
    VOS_UINT32                  ulStatusChgtime;//the time when last status changed

    VOS_VOID                    *pvHook;

    ATP_LOWERLAYER_LIST_ST      *pstLowerLayers;
};

typedef struct tagATP_ADDITIONAL_STATS_ST
{
	VOS_UINT32	ulMulticastPktsRx;		 /* Multicast packets received	*/
	VOS_UINT32	ulMulticastPktsTx;		 /* Multicast packets transmitted	*/
	VOS_UINT32	ulBroadcastPktsRx;		 /* Broadcast packets received 	*/
	VOS_UINT32	ulBroadcastPktsTx;		 /* Broadcast packets transmitted	*/
	VOS_UINT32	ulUnicastPktsRx;		        /* Unicast packets received		*/
	VOS_UINT32	ulUnicastPktsTx;		        /* Unicast transmit problems	*/
	VOS_UINT32	ulUnknownProtoPktsRx;		/* UnknownProto packets received	*/
} ATP_ADDITIONAL_STATS_ST;

typedef struct tagATP_REGISTER_ST
{
	VOS_UINT32	ulFirstAddr;		 /* Register first address	*/
	VOS_CHAR *	pszName;		 /* Register name	*/
	VOS_UINT32	ulLen;		        /* Register length	*/
} ATP_REGISTER_ST;

VOS_UINT32 ATP_INTF_InterfaceInit();

// Iterate and Location

typedef VOS_BOOL (*AtpInterfaceCompareProc)(const ATP_INTERFACE_ST *pstInterface, const VOS_VOID *pvData);
typedef VOS_UINT32 (*AtpInterfaceIterateProc)(ATP_INTERFACE_ST *pstInterface, VOS_VOID *pvData);

VOS_BOOL ATP_INTF_InterfaceIsLowerLayer(const ATP_INTERFACE_ST *pstCurIntf, const ATP_INTERFACE_ST *pstLowerLayer);

ATP_INTERFACE_ST *ATP_INTF_InterfaceGetUpLayer(const ATP_INTERFACE_ST *pstLowIntf, const ATP_INTERFACE_ST *pstPrevUplayer);

VOS_UINT32 ATP_INTF_InterfaceIterate(AtpInterfaceIterateProc pfIterateProc, VOS_VOID *pvData);

ATP_INTERFACE_ST *ATP_INTF_InterfaceFind(AtpInterfaceCompareProc pfCompareProc, const VOS_VOID *pvData);

ATP_INTERFACE_ST *ATP_INTF_InterfaceFindByAlias(const VOS_CHAR *pcAlias);

ATP_INTERFACE_ST *ATP_INTF_InterfaceFindByKeyList(const VOS_UINT32 aucInstId[ATP_CFM_MAX_TREE_DEPTH], VOS_UINT32 ulObjId);

ATP_INTERFACE_ST *ATP_INTF_InterfaceFindByDomainName(const VOS_CHAR *pcDomain);

ATP_INTERFACE_ST *ATP_INTF_InterfaceFindByIfName(const VOS_CHAR *pcIfName);

// Status update
VOS_UINT32 ATP_INTF_InterfaceStatusUpdate(ATP_INTERFACE_ST *pstInterface);

VOS_UINT32 ATP_INTF_InterfaceSetLowerLayers(ATP_INTERFACE_ST *pstInterface);

VOS_UINT32 ATP_INTF_InterfaceStatusUpdateSelf(ATP_INTERFACE_ST *pstInterface);

VOS_BOOL ATP_INTF_InterfaceIsLowerLayerStatusSame(const ATP_INTERFACE_ST *pstInterface, VOS_UINT32 ulStatus);

VOS_UINT32 ATP_INTF_InterfaceCheckLowerLayersStatus(const ATP_INTERFACE_ST *pstInterface, VOS_UINT32 ulStatus);

VOS_UINT32 ATP_INTF_InterfaceGetParaIdx(const ATP_INTERFACE_DESCRIPTOR_ST *pstDescriptor, ATP_INTERFACE_COMMON_PARA_EN enPara);

VOS_UINT32 *ATP_INTF_InterfaceGetCommonParaValue(const ATP_INTERFACE_ST *pstInterface, ATP_INTERFACE_COMMON_PARA_EN enPara);

VOS_UINT32 ATP_INTF_InterfaceSetCommonParaValue(ATP_INTERFACE_ST *pstInterface, ATP_INTERFACE_COMMON_PARA_EN enPara,
                                                              VOS_UINT32 *pulValue);

VOS_UINT32 ATP_INTF_InterfaceGetStatus(const ATP_INTERFACE_ST *pstInterface);
VOS_UINT32 ATP_INTF_InterfaceGetStatusArrayLen();
ATP_INTF_TYPE_EN ATP_INTF_InterfaceGetType(const ATP_INTERFACE_ST *pstIntf);
VOS_BOOL ATP_INTF_IsInterfaceExist(const VOS_CHAR *pszIfc);

VOS_VOID ATP_INTF_InterfaceListAppend(ATP_INTERFACE_ST *pstIntf);

VOS_UINT32 ATP_INTF_InterfaceGetDomain(const ATP_INTERFACE_ST *pstInterface, VOS_CHAR* pcDomain, VOS_UINT32 ulLen);

VOS_UINT32 ATP_INTF_InterfaceStackAdd(const ATP_INTERFACE_ST *pstHigherIntf, const ATP_INTERFACE_ST *pstLowerIntf);

VOS_BOOL ATP_INTF_CheckFakeObjByName(const ATP_INTERFACE_ST *pstIntf);
VOS_UINT32 ATP_INTF_AdditionalStatsGet(const VOS_CHAR *pszIfname, ATP_ADDITIONAL_STATS_ST *pstAdditionalStats);/* <DTS2013093003078 xiazhongyue 20131008 modify  > */


#ifdef ATP_DEBUG
#define ATP_INTF_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_INTF_DEBUG(x...)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

