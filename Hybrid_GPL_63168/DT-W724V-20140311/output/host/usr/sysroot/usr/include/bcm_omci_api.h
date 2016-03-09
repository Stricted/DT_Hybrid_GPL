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

#ifndef BCM_OMCI_API_COMMON_H
#define BCM_OMCI_API_COMMON_H

#include "bcm_gpon_api_common.h"

/**
 * OMCI Driver user API
 **/

#define BCM_OMCI_DEVICE_NAME "bcm_omci"

/*Parameter Tags indicating whether the parameter is an input, output, or input/output argument*/
#ifndef IN
#define IN
#endif /*IN*/

#ifndef OUT
#define OUT
#endif /*OUT*/

#ifndef INOUT
#define INOUT
#endif /*INOUT*/

#define BCM_OMCI_RX_MSG_MAX_SIZE_BYTES 2040
#define BCM_OMCI_TX_MSG_MAX_SIZE_BYTES 2040

/**
 * OMCI Driver ioctl command IDs
 **/

#define BCM_OMCI_IOC_GET_COUNTERS         0  /*Return codes: 0*/
#define BCM_OMCI_IOC_GET_DRIVER_VERSION   1  /*Return codes: 0*/

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_COUNTERS*/
typedef struct {
  IN UINT32 reset;
  OUT UINT32 rxBytes;
  OUT UINT32 rxFragments;
  OUT UINT32 rxFragmentsDropped;
  OUT UINT32 rxMessagesTotal;
  OUT UINT32 rxMessagesDiscarded;
  OUT UINT32 txBytes;
  OUT UINT32 txFragments;
  OUT UINT32 txMessages;

} BCM_Omci_Counters;

#endif /*BCM_OMCI_API_COMMON_H*/
