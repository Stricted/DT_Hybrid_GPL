#ifndef __IQCTL_COMMON_H_INCLUDED__
#define __IQCTL_COMMON_H_INCLUDED__

/*
 *
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
 * File Name : ingqosctl.h
 *
 *******************************************************************************
 */

#define IQ_NAME                 "ingqos"
#define IQ_DRV_NAME             IQ_NAME
#define IQ_DRV_DEVICE_NAME      "/dev/" IQ_DRV_NAME

#define IQCTL_ERROR             (-1)
#define IQCTL_SUCCESS           0

/*
 * Ioctl definitions.
 */
typedef enum {
    IQCTL_IOCTL_SYS,
    IQCTL_IOCTL_MAX
} iqctl_ioctl_t;

typedef enum {
    IQCTL_SUBSYS_STATUS,
    IQCTL_SUBSYS_DPORT,
    IQCTL_SUBSYS_DPORTTBL,
    IQCTL_SUBSYS_QUE,
    IQCTL_SUBSYS_MAX
} iqctl_subsys_t;

typedef enum {
    IQCTL_OP_GET,
    IQCTL_OP_SET,
    IQCTL_OP_ADD,
    IQCTL_OP_REM,
    IQCTL_OP_DUMP,
    IQCTL_OP_FLUSH,
    IQCTL_OP_MAX
} iqctl_op_t;

typedef enum {
    IQCTL_PROTO_TCP,
    IQCTL_PROTO_UDP,
    IQCTL_PROTO_MAX
} iqctl_proto_t;

typedef enum {
    IQCTL_ENT_DYN,
    IQCTL_ENT_STAT,
    IQCTL_ENT_MAX
} iqctl_ent_t;

typedef enum {
    IQCTL_PRIO_LOW,
    IQCTL_PRIO_HIGH,
    IQCTL_PRIO_MAX
} iqctl_prio_t;

typedef enum {
    IQCTL_CONG_STATUS_LO,
    IQCTL_CONG_STATUS_HI,
    IQCTL_CONG_STATUS_MAX
} iqctl_cong_status_t;

typedef enum {
    IQCTL_STATUS_DISABLE,
    IQCTL_STATUS_ENABLE,
    IQCTL_STATUS_MAX
} iqctl_status_t;

typedef struct {
    iqctl_subsys_t  subsys;
    iqctl_op_t      op;
    iqctl_status_t  status;
    iqctl_proto_t   proto;
    int             dport;
    iqctl_ent_t     ent;
    iqctl_prio_t    prio;
    int             nextIx;
} iqctl_data_t;


#endif  /* defined(__IQCTL_COMMON_H_INCLUDED__) */
