#ifndef __PKT_CMF_6816_H_INCLUDED__
#define __PKT_CMF_6816_H_INCLUDED__
/* 
* <:copyright-BRCM:2007:proprietary:standard
* 
*    Copyright (c) 2007 Broadcom Corporation
*    All Rights Reserved
* 
*  This program is the proprietary software of Broadcom Corporation and/or its
*  licensors, and may only be used, duplicated, modified or distributed pursuant
*  to the terms and conditions of a separate, written license agreement executed
*  between you and Broadcom (an "Authorized License").  Except as set forth in
*  an Authorized License, Broadcom grants no license (express or implied), right
*  to use, or waiver of any kind with respect to the Software, and Broadcom
*  expressly reserves all rights in and to the Software and all intellectual
*  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
*  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
*  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
* 
*  Except as expressly set forth in the Authorized License,
* 
*  1. This program, including its structure, sequence and organization,
*     constitutes the valuable trade secrets of Broadcom, and you shall use
*     all reasonable efforts to protect the confidentiality thereof, and to
*     use this information only in connection with your use of Broadcom
*     integrated circuit products.
* 
*  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
*     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
*     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
*     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
*     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
*     PERFORMANCE OF THE SOFTWARE.
* 
*  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
*     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
*     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
*     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
*     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
*     LIMITED REMEDY.
* :>
*/


/*
 *******************************************************************************
 * File Name  : pktCmf.h
 *
 * Description: This file contains the specification of some common definitions
 *      and interfaces to other modules. This file may be included by both
 *      Kernel and userapp (C only).
 *
 *******************************************************************************
 */

#include "pktHdr.h"     /* Common definitions and utilities */
#include "pktCmf_6816_public.h"

/*----- Defines -----*/

#define PKTCMF_VERSION              "0.1"
#define PKTCMF_VER_STR              "v" PKTCMF_VERSION " " __DATE__ " " __TIME__
#define PKTCMF_MODNAME              "Broadcom Packet "

/* include bcm_map.h to use CMF_IS_6816Bx() */
#if defined(CONFIG_BCM96816)
#define CMF_IS_6816B0()             ((PERF->RevID  & REV_ID_MASK) == 0xB0)
#define CMF_IS_6816B1()             ((PERF->RevID  & REV_ID_MASK) == 0xB1)
#define CMF_IS_6816BX()             ( CMF_IS_6816B0() || CMF_IS_6816B1() )
#endif

#define PKTCMF_NAME                 "pktcmf"

#define PKTCMF_ERROR                (-1)
#define PKTCMF_SUCCESS              0

/* Packet CMF Character Device */
#define PKTCMFDRV_MAJOR             232
#define PKTCMFDRV_NAME              PKTCMF_NAME
#define PKTCMFDRV_DEVICE_NAME       "/dev/" PKTCMFDRV_NAME

/* Packet CMF Control Utility Executable */
#define PKTCMF_CTL_UTILITY_PATH     "/bin/cmfctl"

/* Packet CMF Proc FS Directory Path */
#define PKTCMF_PROC_FS_DIR_PATH     PKTCMF_NAME

/* Conditional Compilation */
// #define CC_LAB_CMF_FFE_BYPASS_MODE

/* Configuration of default action on MISS: "Drop" or "Send to MIPS" */
#define CMF_MISS_CFG_DROP   0
#define CMF_MISS_CFG_MIPS   1
#define CMF_MISS_CFG_PASS   2
#define CMF_MISS_DEFAULT    CMF_MISS_CFG_DROP

/* Menuconfig: BRCM_DRIVER_PKTFLOW_DEBUG selection will cause -DPKTDBG C Flags*/
#ifdef PKTDBG
#define CC_PKTCMF_ASSERT
#endif

#define CC_PKTCMF_DEBUG

#if defined( __KERNEL__ )
#include "bcm_OS_Deps.h"
#define KERNEL_LOCK(level)          local_irq_save(level)
#define KERNEL_UNLOCK(level)        local_irq_restore(level)
#endif

typedef enum {
    CMF_DECL(PKTCMF_ENGINE_SWC)
    CMF_DECL(PKTCMF_ENGINE_MCAST)
    CMF_DECL(PKTCMF_ENGINE_ALL) /* max number of CMF enum */
} PktCmfEngine_t;

#define CMF_TUPLE16_MCAST_MASK    (1<<12)  /* must be an integer power of 2 */

/* Construct a 16bit CMF tuple from the Engine and matched FlowInfo Element. */
#define CMF_TUPLE16(eng,matchIx)                                        \
    ( (eng == PKTCMF_ENGINE_MCAST) ?                                    \
      (__force uint16_t)(matchIx | CMF_TUPLE16_MCAST_MASK) :            \
          (__force uint16_t)(matchIx) )

/* Special tuple to signify an invalid tuple. */
#define CMF_TUPLE16_INVALID     FHW_TUPLE_INVALID
#define PKTCMF_MAX_FLOWS        512

extern uint8_t cmfGetHwEngine(uint32_t hwTuple);
extern uint16_t cmfGetHwEntIx(uint32_t hwTuple);

/*
 *------------------------------------------------------------------------------
 *              Packet CFM character device driver IOCTL enums
 * A character device and the associated userspace utility for design debug.
 * Include pktCmfParser.h for ACTIVATE/DEACTIVATE IOCTLs
 *------------------------------------------------------------------------------
 */
typedef enum {
    CMF_DECL(PKTCMF_IOC_STATUS)
    CMF_DECL(PKTCMF_IOC_RESET)
    CMF_DECL(PKTCMF_IOC_INIT)
    CMF_DECL(PKTCMF_IOC_ENABLE)
    CMF_DECL(PKTCMF_IOC_DISABLE)
    CMF_DECL(PKTCMF_IOC_LEARN)
    CMF_DECL(PKTCMF_IOC_DEFER)
    CMF_DECL(PKTCMF_IOC_MCAST)
    CMF_DECL(PKTCMF_IOC_NAT)
    CMF_DECL(PKTCMF_IOC_KCALL)
    CMF_DECL(PKTCMF_IOC_UNITTEST)
    CMF_DECL(PKTCMF_IOC_DEBUG)
    CMF_DECL(PKTCMF_IOC_PRINT)
    CMF_DECL(PKTCMF_IOC_HELLOCONFIG)
    CMF_DECL(PKTCMF_IOC_HELLOTRAFFIC)
    CMF_DECL(PKTCMF_IOC_ASPF)
    CMF_DECL(PKTCMF_IOC_PADLEN)
    CMF_DECL(PKTCMF_IOC_SET_GBL)
    CMF_DECL(PKTCMF_IOC_GET_GBL)
    CMF_DECL(PKTCMF_IOC_SET_PORT_DEFAULTS)
    CMF_DECL(PKTCMF_IOC_GET_PORT_DEFAULTS)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_AS_TBL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_AS_TBL)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_DP_TBL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_DP_TBL)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_EP_TBL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_EP_TBL)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_PAC_TBL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_PAC_TBL)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_FCB)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_FCB)
    CMF_DECL(PKTCMF_IOC_FFE_MEM_ACCESS)
    CMF_DECL(PKTCMF_IOC_FFE_CFG_LABEL)
    CMF_DECL(PKTCMF_IOC_FFE_CFG_SNOOP)
    CMF_DECL(PKTCMF_IOC_FFE_CFG_PATCH)
    CMF_DECL(PKTCMF_IOC_CMF_CFG_MISS)
    CMF_DECL(PKTCMF_IOC_LAB_FLOW)
    CMF_DECL(PKTCMF_IOC_FCB_CTRL)
    CMF_DECL(PKTCMF_IOC_GET_FCBHITS)
    CMF_DECL(PKTCMF_IOC_SET_GBL_STATUS_RL)
    CMF_DECL(PKTCMF_IOC_SET_TCICFG_RL)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_RL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_RL)
    CMF_DECL(PKTCMF_IOC_UPDATE_FWDINFO)
    CMF_DECL(PKTCMF_IOC_ACTIVATE_PBITS2DESTQ_TBL)
    CMF_DECL(PKTCMF_IOC_DEACTIVATE_PBITS2DESTQ_TBL)
    CMF_DECL(PKTCMF_IOC_MAX)
} pktCmf_ioctl_t;


typedef enum
{
    CMF_DECL(PKTCMF_HELLO_CMF)  /* Packet CMF Engine                        */
    CMF_DECL(PKTCMF_HELLO_DST)  /* LANPort EMACS, GPON GEM default destQ    */
    CMF_DECL(PKTCMF_HELLO_FLT)  /* Rule Filter Inspection Element Table     */
    CMF_DECL(PKTCMF_HELLO_SIG)  /* Rule Signature Vector TCAM               */
    CMF_DECL(PKTCMF_HELLO_CTX)  /* Context RAM: Sig H|M, Flow H|M, Aspf Hit */
    CMF_DECL(PKTCMF_HELLO_FFE)  /* Flexible Forwarding Engine               */
    CMF_DECL(PKTCMF_HELLO_ASP)  /* Antispoof Vector, TCAM and Logic         */
    CMF_DECL(PKTCMF_HELLO_FLW)  /* Flow Parse Vector TCAM                   */
    CMF_DECL(PKTCMF_HELLO_FIR)  /* Flow Info RAM                            */
    CMF_DECL(PKTCMF_HELLO_NAT)  /* NAT RAM                                  */
    CMF_DECL(PKTCMF_HELLO_HDR)  /* Header info RAM                          */
    CMF_DECL(PKTCMF_HELLO_CMD)  /* Rule Command Set                         */
    CMF_DECL(PKTCMF_HELLO_MOD)  /* Packet Modification Block                */

    CMF_DECL(PKTCMF_HELLO_SWC)  /* Switch VLAN 802.1Q strip/replace TODO    */
    CMF_DECL(PKTCMF_HELLO_GCF)  /* GPON Mcast Coarse Filter TODO            */

    CMF_DECL(PKTCMF_HW_LAYER)
    CMF_DECL(PKTCMF_HW_FLT)
    CMF_DECL(PKTCMF_HW_CAM)
    CMF_DECL(PKTCMF_HW_CTX)
    CMF_DECL(PKTCMF_HW_FIR)
    CMF_DECL(PKTCMF_HW_NAT)
    CMF_DECL(PKTCMF_HW_HDR)
    CMF_DECL(PKTCMF_HW_CMD)
    CMF_DECL(PKTCMF_HW_CAP)
    CMF_DECL(PKTCMF_HW_HITS)
    CMF_DECL(PKTCMF_HW_MISS)
    CMF_DECL(PKTCMF_HW_DROP)

    CMF_DECL(PKTCMF_HWIF_LAYER)
    CMF_DECL(PKTCMF_HWIF_FLT)
    CMF_DECL(PKTCMF_HWIF_SIG)
    CMF_DECL(PKTCMF_HWIF_ASP)
    CMF_DECL(PKTCMF_HWIF_DSC)
    CMF_DECL(PKTCMF_HWIF_CTX)
    CMF_DECL(PKTCMF_HWIF_CMD)
    CMF_DECL(PKTCMF_HWIF_FPV)
    CMF_DECL(PKTCMF_HWIF_FIR)
    CMF_DECL(PKTCMF_HWIF_HDR)
    CMF_DECL(PKTCMF_HWIF_NAT)
    CMF_DECL(PKTCMF_HWIF_FLW)
    CMF_DECL(PKTCMF_HWIF_TPID)
    CMF_DECL(PKTCMF_HWIF_NEQ)
    CMF_DECL(PKTCMF_HWIF_PADLEN)
    CMF_DECL(PKTCMF_HWIF_RL)

    CMF_DECL(PKTCMF_HAL_LAYER)
    CMF_DECL(PKTCMF_HAL_FLT)
    CMF_DECL(PKTCMF_HAL_SIG)
    CMF_DECL(PKTCMF_HAL_ASP)
    CMF_DECL(PKTCMF_HAL_DSC)
    CMF_DECL(PKTCMF_HAL_CTX)
    CMF_DECL(PKTCMF_HAL_CMD)
    CMF_DECL(PKTCMF_HAL_FPV)
    CMF_DECL(PKTCMF_HAL_FIR)
    CMF_DECL(PKTCMF_HAL_HDR)
    CMF_DECL(PKTCMF_HAL_NAT)
    CMF_DECL(PKTCMF_HAL_FLW)
    CMF_DECL(PKTCMF_HAL_NEQ)

    CMF_DECL(PKTCMF_PARSER_LAYER)
    CMF_DECL(PKTCMF_PARSER_FCB)
    CMF_DECL(PKTCMF_PARSER_FLW)
    CMF_DECL(PKTCMF_PARSER_ASTBL)
    CMF_DECL(PKTCMF_PARSER_DPTBL)
    CMF_DECL(PKTCMF_PARSER_PACTBL)
    CMF_DECL(PKTCMF_PARSER_EPTBL)
    CMF_DECL(PKTCMF_PARSER_PBITS2DESTQTBL)

    CMF_DECL(PKTCMF_FFE_LAYER)

    CMF_DECL(PKTCMF_NAT_LAYER)
    CMF_DECL(PKTCMF_LEARN_LAYER)
    CMF_DECL(PKTCMF_CFG_LAYER)
} pktCmf_layer_t;

typedef uint32_t pktCmfParser_EntityId_t;

#define HELLO_PKT_ERRORED   2
#define HELLO_PKT_FWD2MIPS  0
#define HELLO_PKT_CONSUMED  1
#define HELLO_PKT_DROPPED   2

struct sk_buff;
typedef int (*pktCmfHelloTx)(struct sk_buff * skb_p, int xPort);

#ifdef SUPPORT_HELLO
/*
 * SUPPORT_HELLO conditional compilation: -DSUPPORT_HELLO
 * See: impl2/Makefile  defines CFLAGS += -DSUPPORT_HELLO
 *      cmfctl/Makefile defines CFLAGS += -DSUPPORT_HELLO
 *      bcmenet/impl3/Makefile defines EXTRA_CFLAGS += -DSUPPORT_HELLO
 */
extern int pktCmfHelloRx(struct sk_buff * skb_p, int xPort,
                         pktCmfHelloTx xmitFn);

/* HELLO Callback : Single entry point into HELLO */
extern int pktCmfHelloIf( pktCmf_if_t callback, int *parg1, int *parg2 );
#else
#define pktCmfHelloRx(skb, port, xmitFn) 0
#define pktCmfHelloIf(cb,p1,p2) PKTCMF_SUCCESS
#endif

/* FiltCam Port Capture Configuration: CmfCtl <-> HW */
typedef union {
    struct {
        uint16_t captureFIR     : 1;
        uint16_t captureSIG     : 1;
        uint16_t captureFPV     : 1;
        uint16_t captureASP     : 1;

        uint16_t captureMiss    : 1;
        uint16_t captureHit     : 1;

        uint16_t captureOff     : 1;        /* Disable capture feature */
        uint16_t captureDump    : 1;        /* Dump anything captured */

        uint16_t capturePort    : 8;
    };
    uint16_t u16;
} PortCaptureRequest_t;

/* Lab Flows Configuration: CmfCtl <-> Config : VLAN filters */
typedef enum {
    CMF_DECL(FILT_INV) 
    CMF_DECL(FILT_PBITS)        /* VLAN Outer Tag  3bit pBits */
    CMF_DECL(FILT_VID)          /* VLAN Outer Tag 12bit VLAN ID */
    CMF_DECL(FILT_TCID)         /* VLAN Outer Tag 16bit VLAN TCID */
    CMF_DECL(FILT_ETYPE)        /* Untagged EtherType or Outer VLAN TPID */
    CMF_DECL(FILT_PORT)         /* Ingress Port */
    CMF_DECL(FILT_MAX)
} CmfLabFiltType_t;

/* Description of a single 16bit filter static flow, with forwarding info */
typedef union {
    struct {

        struct {
            uint16_t         filt   :  5;    /* bits 31..27 */
            uint16_t         destQ  :  3;    /* bits 26..24 */
            uint16_t         dest   :  8;    /* bits 23..16 */
        } fwd;

        union {
            struct {
                uint16_t    pbits  :  3;    /* bits 15..13 */
                uint16_t    dei    :  1;    /* bit  12     */
                uint16_t    id     : 12;    /* bits 11..00 */
            } vlan;
            uint16_t        tcid;           /* bits 15..00 */
            uint16_t        etype;          /* bits 15..00 */
            uint16_t        u16;            /* bits 15..00 */
        } value;
    };

    uint32_t u32;

} CmfLabFlow_t;

#endif  /* defined(__PKT_CMF_6816_H_INCLUDED__) */

