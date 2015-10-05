#ifndef __FCACHE_H_INCLUDED__
#define __FCACHE_H_INCLUDED__

/*
*
*  Patent pending Flow Cache algorithm and implementation.
*
*
*  Copyright 2011, Broadcom Corporation
*
* <:label-BRCM:2007:proprietary:standard
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
 * File Name : fcache.h
 * Description of Flow Cache is CONFIDENTIAL and available ONLY in fcache.c .
 *
 *  Version 0.1: Prototype
 *  Version 1.0: BCM963xx
 *  Version 1.1: Multicast
 *  Version 1.2: L4 protocol, L1
 *  Version 2.0: FKB based
 *  Version 2.1: IPv6 Support
 *  Version 2.2: Fkb based Multicast Support (IPv4)
 *
 *******************************************************************************
 */
#define PKTFLOW_VERSION             "v2.2"

#define PKTFLOW_VER_STR             PKTFLOW_VERSION " " __DATE__ " " __TIME__
#define PKTFLOW_MODNAME             "Broadcom Packet Flow Cache "
#define PKTFLOW_NAME                "fcache"
#define FCACHE_PROCFS_DIR_PATH      PKTFLOW_NAME    /* dir: /procfs/fcache    */

/* Flow Cache Character Device */
#define FCACHE_DRV_MAJOR             242
#define FCACHE_DRV_NAME              PKTFLOW_NAME
#define FCACHE_DRV_DEVICE_NAME       "/dev/" FCACHE_DRV_NAME

/*
 * Conditional compilation of cache aligned declaration of flow members
 */
#define CC_FCACHE_ALIGNED_DECLARE
#if defined(CC_FCACHE_ALIGNED_DECLARE)
// #include <linux/cache.h>
#define _FCALIGN_     ____cacheline_aligned
#else
#define _FCALIGN_
#endif

/*
 * Conditional Compilation for Debug Support: global and per layer override
 * - Commenting out CC_CONFIG_FCACHE_DEBUG will disable debug for all layers.
 * - Selectively disable per subsystem by commenting out its define.
 * - Debug levels listed in pktDbg.h
 */
#ifdef PKTDBG
#define CC_CONFIG_FCACHE_DEBUG

/* LAB ONLY: Design development */
#define CC_CONFIG_FCACHE_COLOR          /* Color highlighted debugging     */
#define CC_CONFIG_FCACHE_DBGLVL     0   /* DBG_BASIC Level                 */
#define CC_CONFIG_FCACHE_DRV_DBGLVL 0   /* DBG_BASIC Level Basic           */
#define CC_CONFIG_FCACHE_STATS          /* Statistics design engineering   */
#endif

/* Functional interface return status */
#define FCACHE_ERROR                (-1)    /* Functional interface error     */
#define FCACHE_SUCCESS              0       /* Functional interface success   */

#define FCACHE_CHECK                1       /* Boolean enforcing key audits   */

/* fc_error: unconditionally compiled */
#define fc_error(fmt, arg...)      \
        printk( CLRerr DBGsys "%-10s ERROR: " fmt CLRnl, __FUNCTION__, ##arg )

#undef FCACHE_DECL
#define FCACHE_DECL(x)      x,  /* for enum declaration in H file */

typedef enum
{
    FCACHE_DECL(FCACHE_DBG_DRV_LAYER)
    FCACHE_DECL(FCACHE_DBG_FC_LAYER)
    FCACHE_DECL(FCACHE_DBG_FHW_LAYER)
    FCACHE_DECL(FCACHE_DBG_LAYER_MAX)
} FcacheDbgLayer_t;


/*
 *------------------------------------------------------------------------------
 *              Flow Cache character device driver IOCTL enums
 * A character device and the associated userspace utility for design debug.
 *------------------------------------------------------------------------------
 */
typedef enum FcacheIoctl
{
    FCACHE_DECL(FCACHE_IOCTL_STATUS)
    FCACHE_DECL(FCACHE_IOCTL_ENABLE)
    FCACHE_DECL(FCACHE_IOCTL_DISABLE)
    FCACHE_DECL(FCACHE_IOCTL_FLUSH)
    FCACHE_DECL(FCACHE_IOCTL_DEFER)
    FCACHE_DECL(FCACHE_IOCTL_MCAST)
    FCACHE_DECL(FCACHE_IOCTL_IPV6)
    FCACHE_DECL(FCACHE_IOCTL_RESET_STATS)
    FCACHE_DECL(FCACHE_IOCTL_MONITOR)
    FCACHE_DECL(FCACHE_IOCTL_TIMER)
    FCACHE_DECL(FCACHE_IOCTL_CREATE_FLWSTATS)
    FCACHE_DECL(FCACHE_IOCTL_GET_FLWSTATS)
    FCACHE_DECL(FCACHE_IOCTL_DELETE_FLWSTATS)
    FCACHE_DECL(FCACHE_IOCTL_CLEAR_FLWSTATS)
    FCACHE_DECL(FCACHE_IOCTL_DUMP_FLWSTATS)
    FCACHE_DECL(FCACHE_IOCTL_GRE)
    FCACHE_DECL(FCACHE_IOCTL_DEBUG)
    FCACHE_DECL(FCACHE_IOCTL_INVALID)
} FcacheIoctl_t;


#include <pktHdr.h>

#if defined(CONFIG_BCM_PKTFLOW_MODULE) || defined(CONFIG_BCM_PKTFLOW)

#if !defined(CONFIG_BLOG)
#error "Attempting to build Flow cache without BLOG"
#endif

#include <linux/blog.h>

/*
 *------------------------------------------------------------------------------
 * Conditional Compile configuration for Packet Flow Cache
 *------------------------------------------------------------------------------
 */

#if defined(CONFIG_BLOG_IPV6)
#define CC_FCACHE_IPV6_SUPPORT
#endif

//#define CC_CONFIG_FCACHE_BLOG_MANUAL    /* LAB ONLY: Manual blog enabling  */

#define CC_CONFIG_FCACHE_PROCFS         /* Proc filesystem debug dumps     */

#define CC_CONFIG_FCACHE_STACK          /* Patent Pending: sw acceleration */

#define CC_CONFIG_FCACHE_DEFER          /* Defer HW activation on swhit   */
// #define CC_CONFIG_FCACHE_JENKINS_HASH   /* Jenkins 3word hash algorithm    */

typedef enum {
    FCACHE_HWACC_PRIO_0,            /* Highest Priority */
    FCACHE_HWACC_PRIO_1,            /* Lowest Priority  */
    FCACHE_HWACC_PRIO_MAX
} FcacheHwAccPrio_t;

/*
 *------------------------------------------------------------------------------
 * Implementation Constants 
 *------------------------------------------------------------------------------
 */

/* Flow cache static engineering: runtime poll board memory availability ...  */
#define FCACHE_MAX_ENTRIES        16384     /* Maximum number of entries      */
#define FCACHE_DEF_MAX_ENTRIES      256     /* Def Max number of entries      */
#if (FCACHE_MAX_ENTRIES > 16384)            /* CAUTION: Flow_t uint16_t self  */
#error "Invalid number of flow cache entries, DO NOT EXCEED 16384" 
#endif
#define FCACHE_HTABLE_SIZE         8192     /* Must not be greater than 16384 */
#if (FCACHE_HTABLE_SIZE > FCACHE_MAX_ENTRIES)
#error "Invalid number of flow cache hash table entries, DO NOT EXCEED 16384" 
#endif

#define FCACHE_STACK_SIZE           8               /* goto stack size        */
#define FCACHE_JHASH_RAND           0xBABEEBAB      /* Sufficiently random    */

/* Flow cache system periodic timer */
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#define FCACHE_REFRESH              ( 10 )          /* Poll timer interval   */
#else
#define FCACHE_REFRESH              ( 1 )          /* Poll timer interval   */
#endif
#define FCACHE_SLICE_MAX_ENT_COUNT  128
#define FCACHE_REFRESH_INTERVAL     ( FCACHE_REFRESH SECONDS )


/* Refresh based on layer-4 protocol */
#define FCACHE_REFRESH_IDLE         120
#define FCACHE_REFRESH_IDLE_INTERVAL ( FCACHE_REFRESH_IDLE SECONDS )
#define FCACHE_TUNNEL_4             0xFFFFFFFE
#define FCACHE_TUNNEL_6             0xFFFFFFFF

/* Flow cache entry experiencing hw|sw hits has idle quota of FLOW_MAX_QUOTA  */
#define FLOW_MAX_QUOTA              0xFF

/* Reconfigure Hardware HW if software hits larger than threshold */
#define FCACHE_REACTIVATE           (50)      /* Lookup threshold to reactivate */
#define FCACHE_MAX_PENALTY          8

/* Flow cache hash table IP-Tuple lookup result */
#define FCACHE_MISS                 0       /* Lookup IPTuple hash table miss */
#define FCACHE_HIT                  1       /* Lookup IPTuple hash table hit  */

/* Special tuple to signify an invalid tuple. */
#define FLOW_HW_INVALID             ((__force uint16_t)(0xFFFF))

#define FLOW_NF_INVALID             0x0

#define FLOW_IN_INVALID             0x03    /* Incarnation 0x03 is invalid    */
#define FLOW_IX_INVALID             0       /* Element at index 0 reserved    */
#define FLOW_NULL                   ((Flow_t*)NULL)

#define FDB_IN_INVALID             0x0F    /* Incarnation 0x0F is invalid    */
#define FDB_IX_INVALID             0       /* Element at index 0 reserved    */
#define FDB_NULL                   ((FdbEnt_t*)NULL)
#define FDB_KEY_INVALID            0
/*
 *------------------------------------------------------------------------------
 * All the low prio packets are dropped when the CPU congestion is experienced
 * except when the ANDing of low prio packet counts under CPU congestion in
 * fcache and the mask given below is 0. e.g. if the mask is 0x7F, then 1 out
 * of every 128 low prio packets will be accepted under congestion.  
 * This will relieve CPU congestion because of low prio packets.   
 *------------------------------------------------------------------------------
 */
#define FCACHE_IQOS_LOWPRIO_PKTCNT_MASK 0x7F

typedef enum {
    FCACHE_DECL(HW_CAP_NONE)
    FCACHE_DECL(HW_CAP_IPV4_UCAST)
    FCACHE_DECL(HW_CAP_IPV4_MCAST)
    FCACHE_DECL(HW_CAP_IPV6_UCAST)
    FCACHE_DECL(HW_CAP_IPV6_MCAST)
    FCACHE_DECL(HW_CAP_IPV6_TUNNEL)
    FCACHE_DECL(HW_CAP_MCAST_DFLT_MIPS)
    FCACHE_DECL(HW_CAP_MAX)
} HwCap_t;


/*
 *------------------------------------------------------------------------------
 *  Invoked by Packet HW Protocol layer to clear HW association.
 *  Based on the scope of the request:
 *      System_e scope: Clear hw association for all active flows.
 *      Engine_e scope: Clear hw associations of flows on an engine.
 *      Match_e  scope: Clear a uniquely identified flow.
 *------------------------------------------------------------------------------
 */
typedef enum {
    System_e,       /* System wide active flows */
    Match_e         /* Unique active flow of a specified match id */
} FlowScope_t;

typedef int ( *FC_CLEAR_HOOK)(uint32_t key, FlowScope_t scope);

/*
 * hooks initialized by HW Protocol Layers.
 * Fcache makes upcalls into packet HW Protocol layer via theses hooks
 */
typedef struct FcBindFhwHooks {
    HOOK3PARM       activate_fn; 
    HOOK3PARM       deactivate_fn;
    HOOK3PARM       refresh_fn; 
    HOOK2PARM       reset_stats_fn; 
    HOOK16          clear_fn; 
    FC_CLEAR_HOOK  *fc_clear_fn;
    HOOK4PARM       stats_fn; 
    HOOKP           hwsupport_fn; 
    HOOKV           mcast_dflt_fn;
} FcBindFhwHooks_t; 

/*
 *------------------------------------------------------------------------------
 * Flow cache binding to HW to register HW upcalls and downcalls
 * Upcalls from Flow cache to HW: activate, deactivate and refresh functions.
 * Downcalls from HW to Flow cache: clear hardware associations function.
 *------------------------------------------------------------------------------
 */
extern void fc_bind_fhw( FcBindFhwHooks_t *fhwHooks_p );

extern void fc_flwstats_bind(HOOK3PARM *getFlwStatsFn, 
                             HOOK3PARM flwEvictCallBack);

/*
 *------------------------------------------------------------------------------
 * Defer activation of HW. On every fcache defer number of packets per
 * interval fcache will attempt to activate HW. The interval is specified by
 * FCACHE_REFRESH_INTERVAL. To avoid a performance impact of repeated activation
 * attempts when HW tables are depleted, a penalty is applied (factored into
 * fcache deferral. Bursty traffic will have the penalty reduced.
 *
 * An argument of -1, implies a get of corresponding value.
 *------------------------------------------------------------------------------
 */
extern int fcacheDebug(int debug_level);

extern int  fcacheStatus(void);
extern int  fcacheDefer(int deferral);
extern int  fcacheMonitor(int monitor);
extern int  fcacheInterval(int interval);
extern int  fcacheChkHwSupport(Blog_t * blog_p);
extern void fcacheBindHwSupportHook(HOOKP hw_support_fn);
extern unsigned int  fcacheChkHwFeature(void);

/*
 *------------------------------------------------------------------------------
 * Manual enabling and disabling of Flow cache to Blog binding
 *  flag = 0 : disables binding to blog. No more logging.
 *  flag != 0: enables binding to blog via Flow cache receive/transmit.
 *------------------------------------------------------------------------------
 */
extern void fc_bind_blog(int flag);         /* disable[flag=0] enable[flag=1] */

/*
 *------------------------------------------------------------------------------
 * IP Flow learning status [defined by binding with blog]
 *------------------------------------------------------------------------------
 */
extern void fc_status(void);

/*
 *------------------------------------------------------------------------------
 * Flush all learnt entries in flow cache
 *------------------------------------------------------------------------------
 */
extern int  fc_flush(void);


/*
 *------------------------------------------------------------------------------
 * Flush all learned entries in flow cache for device dev_p
 *------------------------------------------------------------------------------
 */
extern void fc_flush_dev(void * dev_p);

/*
 *------------------------------------------------------------------------------
 * Flow Cache Entry Key:
 * A 32bit key that contains:
 *  - 16bit hardware connection id (encoding of HW engine and matchIx)
 *  -  8bit incarnation id (avoid latent access)
 *  -  8bit entry id (index of entry in flow cache table)
 *------------------------------------------------------------------------------
 */
typedef struct {
    union {
        struct {
            uint32_t    hw      : 16;   /* uint16_t hardware connection id    */
            uint32_t    incarn  :  2;   /* Allocation instance identification */
            uint32_t    self    : 14;   /* Index into static allocation table */
        } id;
        uint32_t word;
    };
} FlowKey_t;

/*
 *------------------------------------------------------------------------------
 * Flow Cache Table Entry:
 *------------------------------------------------------------------------------
 */
struct flow_t {
    struct dll_t    node;       /* First element implements dll               */
    FlowKey_t       key;        /* Second element implements incarnation      */
    struct flow_t   * chain_p;  /* Single linked list hash table chaining     */

    Blog_t          * blog_p;   /* Buffer log carrying flow context data      */
    uint8_t         idle;       /* Idle quota in seconds before cache flush   */
    uint8_t         penalty;    /* Number of activates attempted              */
    uint8_t         list_depth; /* Depth of jump_list (for multicast only)    */
    union {
       uint8_t      flags;
       struct {
          uint8_t   unused       :2;
          uint8_t   l2l_mcast    :1;
          uint8_t   mcast_dflt   :1;   /* mcast default rule */
          uint8_t   iq_prio      :1;   /* Ingress Qos Priority */
          uint8_t   hw_support   :1;   /* e.g. hw acceleration */
          uint8_t   is_ssm       :1;   /* SSM/ASM mcast acceleration          */
          uint8_t   incomplete   :1;   /* Indication of static configuration  */
       };
    };
    uint32_t        swhits;     /* Software lookup hits in last interval      */
    uint16_t        mtuAdj;
    uint8_t         hwAccIx;    /* HW Accelerator Index                       */
    uint8_t         hwPolledCnt;

    uint32_t        cumm_hits;  /* Cummulative sw hit count since creation    */
    uint32_t        cumm_bytes; /* Cummulative byte count since creation      */
    uint32_t        expires;

#if defined(CC_CONFIG_FCACHE_STACK)
                                /* Command sequence for packet mangling   */
    void            * jump_list[FCACHE_STACK_SIZE] _FCALIGN_;
#endif

    struct dll_t    src_fdb_node;   /* linked into FDB src list */
    struct dll_t    dst_fdb_node;   /* linked into FDB dst list */
} _FCALIGN_;                    /* 5 cache lines wide */

typedef struct flow_t Flow_t;

/*
 *------------------------------------------------------------------------------
 * Flow Cache Slice Timer Entry:
 *------------------------------------------------------------------------------
 */
struct sliceEnt_t {
    struct dll_t    node;       /* First element implements dll               */
    uint32_t        id;         /* slice timer entry id                       */
    Flow_t        * flow_p;     /* points to owned by flow                    */
} _FCALIGN_;                    
typedef struct sliceEnt_t SliceEnt_t;

/*
 *------------------------------------------------------------------------------
 * Flow Cache Entry Key:
 * A 32bit key that contains:
 *  - 16bit hardware connection id (encoding of HW engine and matchIx)
 *  -  8bit incarnation id (avoid latent access)
 *  -  8bit entry id (index of entry in flow cache table)
 *------------------------------------------------------------------------------
 */
typedef struct {
    union {
        struct {
            uint32_t    unused  : 16;   /* uint16_t unused */
            uint32_t    incarn  :  4;   /* Allocation instance identification */
            uint32_t    self    : 12;   /* Index into static allocation table */
        } id;
        uint32_t word;
    };
} FdbKey_t;

typedef union {
    uint8_t         u8[6];  /* MAC */
    uint16_t        u16[3]; /* MAC */
} FcMac_t;

/*
 *------------------------------------------------------------------------------
 * Flow Cache FDB Entry:
 *------------------------------------------------------------------------------
 */
struct fdbEnt_t {
    struct dll_t    node;           /* FDB list */
    FdbKey_t        key;            /* linking Linux and fcache FDB entries   */
    struct fdbEnt_t *chain_p;       /* FDB Hash list node                     */

    struct dll_t    src_act_list;   /* active flows                           */
    struct dll_t    src_idle_list;  /* idle flows                             */
    struct dll_t    dst_act_list;   /* active flows                           */
    struct dll_t    dst_idle_list;  /* idle flows                             */

    uint16_t        hashix;         /* FDB hash index                         */
    FcMac_t         mac;
    unsigned long   upd_time;       /* last update time in jiffies            */
    uint32_t        unused;         
#if defined(CC_CONFIG_FCACHE_STATS)
    uint32_t        src_flow_count; /* # of flows linked to src FDB list      */
    uint32_t        dst_flow_count; /* # of flows linked to dst FDB list      */
#endif
} _FCALIGN_;                    
typedef struct fdbEnt_t FdbEnt_t;

/* Flow cache static engineering: runtime poll board memory availability ...  */
#define FCACHE_FDB_HTABLE_SIZE      128     /* Must not be greater than 512   */
#define FCACHE_FDB_MAX_ENTRIES      512     /* Maximum number of FDB entries  */
#define FCACHE_FDB_DEF_MAX_ENTRIES  256     /* Def Max number of entries      */
#if (FCACHE_FDB_MAX_ENTRIES > 512)          
#error "Invalid number of flow cache FDB entries, DO NOT EXCEED 512" 
#endif

#else
#define     fc_bind_blog(enable)        NULL_STMT
#endif  /* defined(CONFIG_BCM_PKTFLOW_MODULE) || defined(CONFIG_BCM_PKTFLOW) */

#endif  /* defined(__FCACHE_H_INCLUDED__) */
