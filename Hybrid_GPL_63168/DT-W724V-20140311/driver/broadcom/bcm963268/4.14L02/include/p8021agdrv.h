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
/***************************************************************************
 * File Name  : p8021agdrv.h
 *
 * Description: This file contains the definitions and structures for the
 *              Linux IOCTL interface to access the IEEE P802.1ag driver.
 ***************************************************************************/

#if !defined(_P8021AGDRV_H_)
#define _P8021AGDRV_H_

#define ETH_ALEN 6
#define IFNAMSIZ 16

/* Defines. */
#define MAX_LINKTRACE_REPLIES           16
#define P8021AG_MAJOR                   227

#define P8021AG_IOCTL_START          _IOWR(P8021AG_MAJOR, 0, P8021AG_START_STOP)
#define P8021AG_IOCTL_STOP           _IOWR(P8021AG_MAJOR, 1, P8021AG_START_STOP)
#define P8021AG_IOCTL_SET_MDLEVEL    _IOWR(P8021AG_MAJOR, 2, int)
#define P8021AG_IOCTL_SEND_LOOPBACK  _IOWR(P8021AG_MAJOR, 3, P8021AG_LOOPBACK)
#define P8021AG_IOCTL_SEND_LINKTRACE _IOWR(P8021AG_MAJOR, 4, P8021AG_LINKTRACE)

/* Typedefs. */
typedef struct
{
    char szDeviceName[IFNAMSIZ];
    int nResult;
} P8021AG_START_STOP, *PP8021AG_START_STOP;

typedef struct
{
    unsigned char ucMacAddr[ETH_ALEN];
    unsigned short usVlanId;
    int nResult;
} P8021AG_LOOPBACK, *PP8021AG_LOOPBACK;

typedef struct
{
    unsigned char ucMacAddr[ETH_ALEN];
    unsigned short usVlanId;
    int nMacAddrListLen;
    unsigned char ucMacAddrList[MAX_LINKTRACE_REPLIES][ETH_ALEN];
    int nResult;
} P8021AG_LINKTRACE, *PP8021AG_LINKTRACE;

#endif // _P8021AGDRV_H_

