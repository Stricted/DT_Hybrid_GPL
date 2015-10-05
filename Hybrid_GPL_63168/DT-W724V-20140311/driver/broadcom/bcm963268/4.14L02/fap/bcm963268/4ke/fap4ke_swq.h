#ifndef __FAP4KE_SWQ_H_INCLUDED__
#define __FAP4KE_SWQ_H_INCLUDED__

/*
<:copyright-BRCM:2007:DUAL/GPL:standard

   Copyright (c) 2007 Broadcom Corporation
   All Rights Reserved

Unless you and Broadcom execute a separate written software license 
agreement governing use of this software, this software is licensed 
to you under the terms of the GNU General Public License version 2 
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php, 
with the following added to such license:

   As a special exception, the copyright holders of this software give 
   you permission to link this software with independent modules, and 
   to copy and distribute the resulting executable under terms of your 
   choice, provided that you also meet, for each linked independent 
   module, the terms and conditions of the license of that module. 
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications 
   of the software.  

Not withstanding the above, under no circumstances may you combine 
this software in any way with any other Broadcom software provided 
under a license other than the GPL, without Broadcom's express prior 
written consent. 

:>
*/
/* SW Queue Data */


#define SWQ_HOST2FAP_GSO_LOOPBACK_BUDGET 32

#define SWQ_HOST2FAP_GSO_LOOPBACK_Q  0
#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_MSG_SIZE 4   /*words*/
#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_DEPTH 1024

#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_MEM_SIZE  \
        (SWQ_HOST2FAP_GSO_LOOPBACK_Q_MSG_SIZE * SWQ_HOST2FAP_GSO_LOOPBACK_Q_DEPTH)
        

#define SWQ_FAP2HOST_GSO_LOOPBACK_Q  1
#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_MSG_SIZE 2   /*words*/
#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_DEPTH 2048

#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_MEM_SIZE   \
        (SWQ_FAP2HOST_GSO_LOOPBACK_Q_MSG_SIZE * SWQ_FAP2HOST_GSO_LOOPBACK_Q_DEPTH)

#define SWQ_MAX  2


#if 0
typedef struct {
  uint32 gsoLoopBackH2FSwqMem[SWQ_HOST2FAP_GSO_LOOPBACK_Q_MEM_SIZE];
  uint32 gsoLoopBackF2HSwqMem[SWQ_FAP2HOST_GSO_LOOPBACK_Q_MEM_SIZE];
} fap4ke_Sdram_Swq_t;
#endif

typedef struct SWQDataMsg_t
{
    uint32    data[4];   
} SWQDataMsg_t;

typedef struct fap4ke_SWQueue_t{
    struct{
        uint8  dqm:6;         /* assoicated DQM for interrupt mgmt */
        uint8  dqmDir:1;
        uint8  fapId:1;
    };
    uint8  msgSize;    /* size of each message */
    uint16 maxDepth;

    uint16 rsvd;
    uint16 count;    /* number of messages in Queue */

    uint32 *qStart;   /* pointer to the start of the Queue in DDR */
    uint32 *qEnd;     /* pointer to the start of the Queue in DDR */

    uint32 *rdPtr;   /* current read pointer */
    uint32 *wrPtr;   /* current read pointer */

    uint32 dropped; /* number of message dropped due to queue full */
    uint32 processed; /* total number of messages passed */
} fap4ke_SWQueue_t;


#ifdef FAP_4KE
extern inline int swqXmitAvailable4ke(fap4ke_SWQueue_t *swq);
extern inline void swqXmitMsg4ke(fap4ke_SWQueue_t *swq, SWQDataMsg_t *msg);
extern inline int swqRecvAvailable4ke(fap4ke_SWQueue_t *swq);
extern void swqRecvMsg4ke(fap4ke_SWQueue_t *swq, SWQDataMsg_t *msg);
extern void inline swqInit4ke(fap4ke_SWQueue_t *swq, uint32 *qmem, uint32 msgSize,
                                uint32 maxDepth, uint32 dqm, uint8 dqmDir);
extern void swqDump4ke(fap4ke_SWQueue_t *swq);
#endif  

#endif  /* defined(__FAP4KE_SWQ_H_INCLUDED__) */
