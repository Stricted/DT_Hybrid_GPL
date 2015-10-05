/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: mtacfg.h
*    Creation Date: 4 July 2000 (v0.00)
*    VSS Info:
*        $Revision: 23 $
*        $Date: 9/14/01 4:54p $
*
****************************************************************************
*    Description:
*
*     This header file contains all the different build options that
*     are required by CablexChange components.
*
****************************************************************************/

#ifndef _MTACFG_H
#define _MTACFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cfg/endptCfg.h>
#include <cfg/vocmCfg.h>
#include <cfg/casCfg.h>

#define MTA_VERSION_INFO   "1.0"

#if ( defined(IP_MGCP) || defined(IP_H323) || defined(IP_SIP) || defined(IP_MEGACO) )
#define VOIP
#endif

#ifndef VOIP
#define ENDPOINT_NULL_SHIM
#endif


/***************************************************************
** Task Priority options
****************************************************************/

#define RTP_TASK_PRIORITY        BCMOS_TASK_PRTY_8  /* task priority for rtp */
#define HGCP_TASK_PRIORITY       BCMOS_TASK_PRTY_8  /* task priority for HGCP */
#define RTCP_TASK_PRIORITY       BCMOS_TASK_PRTY_13 /* task priority for rtcp */


/* Additional DSL specific definitions not provided by Cable Cfg files */

/* For backward compatibility with older code */
#define MAX_ENDPT   VOCM_CFG_MAX_ENDPT
#define MAX_CNX   VOCM_CFG_MAX_CNX

#ifdef __cplusplus
    }
#endif

#endif /* MTACFG_H */



