/*************************************************
* <:copyright-BRCM:2013:proprietary:standard
*
*    Copyright (c) 2013 Broadcom Corporation
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
:>
*/
/**  @mainpage Broadcom Configureation Statics Module
 *
 *   @section Introduction
 *
 *   This document covers Broadcom WLAN Configuration and Statics
 *   Module(WCSM).
 *
 *  @file wlcsm_linux.h
 *  @brief shared wlcsm header file for userspace and kernel space
 *
 *  this header file includes all general functions and structure definations
 *  used for this wlcsm module.
 *
 * */

#ifndef __WLCSM_LINUX_H_
#define  __WLCSM_LINUX_H_

//#define WLCSM_DEBUG

#define WLCSM_SET_TYPE(x, v)  (((struct wlcsm_msg_hdr *)x)->type = v)
#define WLCSM_SET_LEN(x, v)  (((struct wlcsm_msg_hdr *)x)->len = v)

#define TRUE 1
#define FALSE 0
#define m_MAX_BUF_SIZE    1024
#define WLCSM_NAMEVALUEPAIR_MAX 512

#define WLCSM_TRACE_DBG 1
#define WLCSM_TRACE_ERR 2
#define WLCSM_TRACE_LOG 4

typedef struct t_wlcsm_name_valuepair {
    int len;                                /**< total length of this structure name:value */
    char value[1];                          /**< acting as pointer to the name:value pair string */
} t_WLCSM_NAME_VALUEPAIR;

#define VALUEPAIR_NAME(v) (((t_WLCSM_NAME_VALUEPAIR *)(v))->value)
#define VALUEPAIR_VALUE(v) (((t_WLCSM_NAME_VALUEPAIR*)((char *)(v)+((t_WLCSM_NAME_VALUEPAIR*)(v))->len))->value)
#define VALUEPAIR_NAME_LEN(v) (((t_WLCSM_NAME_VALUEPAIR *)(v))->len-sizeof(int))
#define VALUEPAIR_VALUE_LEN(v) (((t_WLCSM_NAME_VALUEPAIR*)((char *)(v)+((t_WLCSM_NAME_VALUEPAIR*)(v))->len))->len)
#define VALUEPAIR_LEN(v) (VALUEPAIR_NAME_LEN(v)+VALUEPAIR_VALUE_LEN(v)+2*sizeof(int))
#define VALUEPAIR_OLD_VALUE(v)   (((t_WLCSM_NAME_VALUEPAIR *)(((char*)(v))+ VALUEPAIR_LEN))->value)

#define VALUEPAIR_SET(v,name,value) do { \
    int buflen=2*sizeof(int)+2+strlen((name))+(value)!=NULL?strlen((value)):0;\
    memset((void *)v,0,buflen); \
    t_WLCSM_NAME_VALUEPAIR *vp=(t_WLCSM_NAME_VALUEPAIR *)(v); \
    vp->len=strlen(name)+1+sizeof(int); \
    strcpy(vp->value,name); \
    vp = (t_WLCSM_NAME_VALUEPAIR *)(v+vp->len); \
    if((value)!=NULL) { \
	    vp->len=strlen((value))+1; \
	    strcpy(vp->value,(value)); \
    } else vp->len=0; \
} while(0)


typedef struct wlcsm_msg_hdr {
    unsigned short type;
    unsigned short len;
} t_WLCSM_MSG_HDR;

typedef enum wlcsm_msgtype {
    WLCSM_MSG_BASE = 0,
    WLCSM_MSG_REGISTER,
    WLCSM_MSG_NVRAM_SET,
    WLCSM_MSG_NVRAM_GET,
    WLCSM_MSG_NVRAM_UNSET,
    WLCSM_MSG_NVRAM_GETALL,
    WLCSM_MSG_NVRAM_GETALL_DONE, /* for getall operation, if nvarm set is too big,more transaction may needed, so need this msg to indeciate done */
    WLCSM_MSG_NVRAM_COMMIT,
    WLCSM_MSG_NVRAM_SETTRACE,

} t_WLCSM_MSGTYPES;

typedef enum wlcsm_ret_codes {
    WLCSM_SUCCESS = 0,
    WLCSM_GEN_ERR = 1,
} t_WLCSM_RET_CODE;


typedef struct wlcsm_register {
    int code;
} t_WLCSM_REGISTER;



#ifdef WLCSM_DEBUG
extern unsigned int  g_WLCSM_TRACE_LEVEL;
#endif


#ifndef __KERNEL__
#include <stdlib.h>

#ifdef WLCSM_DEBUG
#define WLCSM_TRACE(loglevel,fmt,arg...)   do { if(g_WLCSM_TRACE_LEVEL & loglevel) fprintf(stdout,"---:%s:%d  "fmt,__FUNCTION__,__LINE__,##arg); } while(0)
#else
#define WLCSM_TRACE(loglevel,fmt,arg...)
#endif

#ifndef  _MALLOC_
#define _MALLOC_(x)             calloc(x, sizeof(char))
#endif

#ifndef  _MFREE_
#define _MFREE_(buf)      free(buf)
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)            (sizeof(a) / sizeof(a[0]))
#endif

#else                                           /**< kernel definitions */

#ifdef WLCSM_DEBUG
#define WLCSM_TRACE(loglevel,fmt,arg...)   do { if(g_WLCSM_TRACE_LEVEL & loglevel) printk("---:%s:%d  "fmt,__FUNCTION__,__LINE__,##arg); } while(0)
#else
#define WLCSM_TRACE(loglevel,fmt,arg...)
#endif

#endif


#endif
