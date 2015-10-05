/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file dhcp6ccmsmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x00192803
  *\date  2012-1-31
*/

#ifndef __NEIGHDISCAPI_MSGTYPES_H__
#define __NEIGHDISCAPI_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define NEIGH_DISC_RS_ENABLE_FILE                    "/var/wan/%s/rs_enable"

#define NEIGH_DISC_RS_RTYMAXNUM_FILE                  "/var/wan/%s/rs_rtymaxnum"

#define NEIGH_DISC_RS_RTYINTV_FILE                   "/var/wan/%s/rs_rtyinterval"

typedef struct tagNEIGH_DISC_NS_INFO_ST
{
    VOS_BOOL     bNUDEnable;        /*!< Enables or disables Neighbor Unreachability Detection (NUD)*/
    VOS_UINT32  ulRetransTimer;      /*!< RetransTimer specifies the delay between consecutive Neighbor Solicitation transmissions performed during Duplicate Address Detection (DAD) */
}NEIGH_DISC_NS_INFO_ST;

typedef struct tagNEIGH_DISC_RS_INFO_ST
{
    VOS_BOOL     bRSEnable;        /*!< Enables or disables Router Solicitation (RS) on Interface */
    VOS_UINT32  ulRtrSolicitationInterval;      /*!< RtrSolicitationInterval specifies the delay between consecutive Router Solicitation transmissions. */
    VOS_UINT32  ulMaxRtrSolicitations;      /*!< Maximum Number of Router Solicitation Transmission messages*/
}NEIGH_DISC_RS_INFO_ST;

typedef struct tagNEIGH_DISC_INFO_ST
{
    struct tagNEIGH_DISC_INFO_ST   *pstNext;

    VOS_UINT32  ulInstId;				/*!<结点实例号. */

    VOS_BOOL     bEnable;        /*!< Enables or disables this InterfaceSetting entry.*/

    VOS_CHAR     acStatus[ATP_BUFFER_64];        /*!< The status of this entry*/

    VOS_CHAR     acInterface[ATP_BUFFER_128];        /*!< The value MUST be the path name of a row in the IP.Interface table*/

    VOS_CHAR     acIpintfName[ATP_BUFFER_128];        /*!< */

    NEIGH_DISC_NS_INFO_ST   stNeighDiscNsInfo;

    NEIGH_DISC_RS_INFO_ST   stNeighDiscRsInfo;

} NEIGH_DISC_INFO_ST;

enum ATP_NEIGH_DISC_MSGTYPES_EN
{
    ATP_MSG_NEIGH_DISC_RS_INFO = ATP_MSG_CATEGORY_DEF(ATP_MODULE_NEIGHDISCCMS),    //rs消息    
} ;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
