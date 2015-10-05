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
*    Filename: bcmos.h
*    Creation Date: 8 June 2000 (v0.00)
*    VSS Info:
*        $Revision: 16 $
*        $Date: 5/03/01 5:03p $
*
****************************************************************************
*    Description:
*
*      Broadcom Generic Operating System Functions.
*
****************************************************************************/

#ifndef _BCMOS_H
#define _BCMOS_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KERNEL__
#include <cxcEnv.h>
#include <bosSleep.h>
#include <bosTask.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <bcmtypes.h>

#define malloc(arg) kmalloc(arg, GFP_KERNEL)
#define free(arg) kfree(arg)

#define BCMOS_ASSERT(expr) \
if ((expr)? 0:1)                                                     \
{                                                                    \
   printk(" ASSERT !!! File %s, line %u \n", __FILE__, __LINE__);    \
   bosSleep( 10 );                                                 \
   cli();   /* Disable interrupts */                                 \
   while(1){ ; }                                                     \
}

#define CXC_ASSERT BCMOS_ASSERT		// For compatibility with CX's endpoint

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*  Special BOS definitions   */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#define DSL_NO_RESETSUPPORT


#define  bosSocketHtoNL( num )   ((UINT32)( num ))
#define  bosSocketHtoNS( num )   ((UINT16)( num ))
#define  bosSocketNtoHL( num )   ((UINT32)( num ))
#define  bosSocketNtoHS( num )   ((UINT16)( num ))
#define  bosSocketNtoHS( num )   ((UINT16)( num ))
#define  bosSocketNtoHU16  bosSocketNtoHS


#else /* LINUX */

#include <cxcEnv.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#define BCMOS_TASKNAME( nameVar, nameStr ) char nameVar[] = nameStr

#define TEN_MSEC           10
#define TWENTY_MSEC        20
#define ONE_HUNDRED_MSEC   100
#define FIVE_HUNDRED_MSEC  500
#define ONE_SECOND         1000
#define TWO_SECONDS        2000

#define BCMOS_ASSERT( expr) assert( expr )

#endif

#ifdef __cplusplus
    }
#endif


#endif  /* _BCMOS_H  */


