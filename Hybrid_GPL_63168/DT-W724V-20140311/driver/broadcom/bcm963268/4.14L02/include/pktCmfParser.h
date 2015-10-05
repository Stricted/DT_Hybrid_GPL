/*
<:copyright-broadcom

 Copyright (c) 2007 Broadcom Corporation
 All Rights Reserved
 No portions of this material may be reproduced in any form without the
 written permission of:
          Broadcom Corporation
          5300 California Avenue
          Irvine, California 92617
 All information contained in this document is Broadcom Corporation
 company private, proprietary, and trade secret.

:>
*/

/*
 *******************************************************************************
 * File Name  : pktCmfParser.h
 *
 * Description: This file contains the specification of some common definitions
 *      and interfaces to other modules. This file may be included by both
 *      Kernel and userapp (C only).
 *
 *******************************************************************************
 */

#ifndef __PKT_CMF_PARSER_H_INCLUDED__
#define __PKT_CMF_PARSER_H_INCLUDED__

/************************************************************
 * User must include bpaapi_access.h before this header file
 ************************************************************/

#define PKTCMF_PARSER_DONT_CARE ~0

#define PKTCMF_PARSER_IS_DONT_CARE(x) ( (x) == (typeof(x))(PKTCMF_PARSER_DONT_CARE) )

/**** Parser IOCTL Structures ****/
typedef enum {
    PKTCMF_IOC_SET_GBL_NONE = 0,
    PKTCMF_IOC_SET_GBL_FLOW_MISS_ACTION = (1 << 0),
} pktCmfParser_iocGblFlags_t;

typedef struct {
    uint32_t setFlag;
    BpaFlowMissAction_t    flowMissAction;
} pktCmfParser_iocGbl_t;

typedef enum {
    PKTCMF_IOC_SET_PORT_DEF_NONE = 0,
    PKTCMF_IOC_SET_PORT_DEF_ACL = (1 << 0),
    PKTCMF_IOC_SET_PORT_DEF_TPID = (1 << 1),
    PKTCMF_IOC_SET_PORT_DEF_PADLEN = (1 << 2),
} pktCmfParser_iocPortDefFlags_t;

typedef struct {
    uint32_t setFlag;
    struct {
        BpaPort_t    srcPort;
        BpaAclType_t aclType;
    } portAclDefaults;
    BpaTpidCfg_t tpidCfg;
    struct {
        BpaPort_t    srcPort;
        uint32_t     padLen;
    } portPadLenDefaults;
} pktCmfParser_iocPortDefaults_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaAsTblInfo_t asTblInfo;
} pktCmfParser_iocAsTable_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaDscpTblInfo_t dpTblInfo;
} pktCmfParser_iocDpTable_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaPbits2DestQTblInfo_t pbits2DestQTblInfo;
} pktCmfParser_iocPbits2DestQTable_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaPacTblInfo_t pacTblInfo;
} pktCmfParser_iocPacTable_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaEpTblInfo_t epTblInfo;
} pktCmfParser_iocEpTable_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    uint32_t fcbLength;
    BpaFCB_t *fcb;
    pktCmfParser_EntityId_t eidPacTable;
    pktCmfParser_EntityId_t eidEpTable;
    pktCmfParser_EntityId_t eidPbits2DestQTable;
} pktCmfParser_iocFcb_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaFCBHits_t fcbHits;
} pktCmfParser_iocFcbHits_t;

typedef struct {
    uint32_t    rlIndex;
    BpaRLEnt_t  cfg;
} pktCmfParser_iocRL_info_t;

typedef struct {
    BpaRLStatus_t   rlStatus;
    BpaRLDir_t      rlDir;
    BpaRLTciCfg_t   rlTciCfg;
    pktCmfParser_iocRL_info_t rlInfo;
} pktCmfParser_iocRL_t;

typedef struct {
    pktCmfParser_EntityId_t eid;
    BpaFwdInfo_t fwdInfo;
} pktCmfParser_iocFwdInfo_t;


#endif /* __PKT_CMF_PARSER_H_INCLUDED__ */
