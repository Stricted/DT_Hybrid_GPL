/***********************************************************************
//
//  Copyright (c) 2008  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          5300 California Avenue 
//          Irvine, California 92617 
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
************************************************************************/
#ifndef _BCMPWRMNGTDRV_H
#define _BCMPWRMNGTDRV_H

#include <linux/ioctl.h>
#include "bcmpwrmngtcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PWRMNGT_DRV_MAJOR 240

#define PWRMNGT_IOCTL_INITIALIZE \
    _IOWR(PWRMNGT_DRV_MAJOR, 0, PWRMNGT_DRV_INITIALIZE)
#define PWRMNGT_IOCTL_UNINITIALIZE \
    _IOR(PWRMNGT_DRV_MAJOR, 1, PWRMNGT_DRV_STATUS_ONLY)
#define PWRMNGT_IOCTL_SET_CONFIG \
    _IOWR(PWRMNGT_DRV_MAJOR, 2, PWRMNGT_DRV_CONFIG_PARAMS)
#define PWRMNGT_IOCTL_GET_CONFIG \
    _IOWR(PWRMNGT_DRV_MAJOR, 3, PWRMNGT_DRV_CONFIG_PARAMS)
#define PWRMNGT_IOCTL_GET_LOWPWR \
    _IOWR(PWRMNGT_DRV_MAJOR, 4, PWRMNGT_IF_STATUS)

#define MAX_PWRMNGT_DRV_IOCTL_COMMANDS 5

#define PWRMNGT_ALL_IF_BIT 0x01
#define PWRMNGT_ETH_IF_BIT 0x02
#define PWRMNGT_USB_IF_BIT 0x04


/* Typedefs. */
typedef struct
{
    PWRMNGT_STATUS status;
} PWRMNGT_DRV_STATUS_ONLY, *PPWRMNGT_DRV_STATUS_ONLY;

typedef struct
{
   PWRMNGT_CONFIG_PARAMS     init;
   PWRMNGT_STATUS            status;
} PWRMNGT_DRV_INITIALIZE, *PPWRMNGT_DRV_INITIALIZE;


typedef struct
{
   PWRMNGT_CONFIG_PARAMS        configParams;
   ui32                         configMask;
   PWRMNGT_STATUS               status;
} PWRMNGT_DRV_CONFIG_PARAMS, *PPWRMNGT_DRV_CONFIG_PARAMS ;

typedef struct
{
   char  iftype[16];
   int   ifIndex;
   int   bLinkDwn;
   int   bPopulated;
} PWRMNGT_IF_EVENT, *PPWRMNGT_IF_EVENT;


#ifdef __cplusplus
}
#endif

#endif /* _BCMPWRMNGTDRV_H */
