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

#ifndef BCM_PLOAM_API_H
#define BCM_PLOAM_API_H

#include "bcm_gpon_api_common.h"

/**
 * PLOAM Driver user API
 **/

/*Device names*/
#define BCM_PLOAM_DEVICE_NAME     "bcm_ploam"
#define BCM_PLOAM_USR_DEVICE_NAME "bcm_user_ploam"
#define BCM_PLOAM_TOD_USR_DEVICE_NAME "bcm_user_tod"

#if defined(CONFIG_BCM_GPON_FPGA)
#define BCM_PLOAM_NUM_DATA_TCONTS 2
#define BCM_PLOAM_NUM_DATA_GEM_PORTS 4
#define BCM_PLOAM_NUM_GEM_MIB_IDXS            BCM_PLOAM_NUM_DATA_GEM_PORTS
#define BCM_PLOAM_MAX_GTCUS_QUEUES             BCM_PLOAM_NUM_DATA_GEM_PORTS  /* Same as GEM ports */
#else /*!FPGA:*/
#if defined(CONFIG_BCM96816)
#define BCM_PLOAM_NUM_DATA_TCONTS              32
#define BCM_PLOAM_NUM_DATA_GEM_PORTS           CONFIG_BCM_MAX_GEM_PORTS
#define BCM_PLOAM_NUM_GEM_MIB_IDXS             BCM_PLOAM_NUM_DATA_GEM_PORTS
#define BCM_PLOAM_DEF_GEM_MIB_IDX              (BCM_PLOAM_NUM_GEM_MIB_IDXS-1) /* Last one is catch all */
#define BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT   4 
#define BCM_PLOAM_MAX_GTCUS_QUEUES             BCM_PLOAM_NUM_DATA_GEM_PORTS  /* Same as GEM ports */
#else /* 6816 */

#define BCM_PLOAM_NUM_DATA_TCONTS              8
#define BCM_PLOAM_NUM_DATA_GEM_PORTS           CONFIG_BCM_MAX_GEM_PORTS
#define BCM_PLOAM_NUM_GEM_MIB_IDXS             32 /* Should not be greater than 32 */
#define BCM_PLOAM_DEF_GEM_MIB_IDX              (BCM_PLOAM_NUM_GEM_MIB_IDXS-1) /* Last one is catch all */
#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
#define BCM_PLOAM_MAX_GTCUS_QUEUES             (BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT * BCM_PLOAM_NUM_DATA_TCONTS)  
#ifdef SUPPORT_PONWIZ
#define BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT   128
#else
#define BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT   8
#endif
#else 
#define BCM_PLOAM_MAX_GTCUS_QUEUES             32  /* Should not be greater than 32 */
#define BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT   4 
#endif
#define BCM_PLOAM_NUM_DDRO_QUEUE               BCM_PLOAM_MAX_GTCUS_QUEUES
#define BCM_PLOAM_MAX_DDRO_QUEUE_SIZE          4096 /* max queue is 4096 KB */
#define BCM_PLOAM_MAX_DDRO_QUEUE_SIZE_IN_BYTES (BCM_PLOAM_MAX_DDRO_QUEUE_SIZE * 1024)
#define BCM_PLOAM_WRED_NUM_QUEUES              16
#define BCM_PLOAM_WRED_NUM_PROFILES            16

/* Making it a hard requirement to distribute equal number of queues to each TCONT */
#if (BCM_PLOAM_MAX_GTCUS_QUEUES != BCM_PLOAM_NUM_DATA_TCONTS*BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT)
#error "ERROR - (BCM_PLOAM_MAX_GTCUS_QUEUES != BCM_PLOAM_NUM_DATA_TCONTS*BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT)"
#endif

#endif /* 6818 */
#endif /*CONFIG_GPON_FPGA*/

/**
 * Ploam Driver ioctl command IDs
 **/
typedef enum
{
   BCM_PLOAM_IOC_INIT                   =     10, /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_EVENT_STATUS             ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_MASK_EVENT                   ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_ALARM_STATUS             ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_MASK_ALARM                   ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_SET_SF_SD_THRESHOLD          ,   /*Return codes: 0, EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_GET_SF_SD_THRESHOLD          ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_REGISTER_MSG                 ,   /*Return codes: 0, EINVAL_PLOAM_DUPLICATE*/
   BCM_PLOAM_IOC_UNREGISTER_MSG               ,   /*Return codes: 0, EINVAL*/
   BCM_PLOAM_IOC_START                        ,   /*Return codes: 0, EINVAL_PLOAM_STATE, EINVAL_INIT_OPER_STATE*/
   BCM_PLOAM_IOC_STOP                         ,   /*Return codes: 0, EINVAL*/
   BCM_PLOAM_IOC_GET_STATE                    ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_MESSAGE_COUNTERS         ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_GTC_COUNTERS             ,   /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_GEM_PORT_COUNTERS        ,   /*Return codes: 0, EINVAL_PLOAM_GEM_PORT*/
/*Return codes: 0, EINVAL_PLOAM_ARG, EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_DUPLICATE, EINVAL_PLOAM_NOENT*/
   BCM_PLOAM_IOC_CFG_GEM_PORT                 ,
#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
   BCM_PLOAM_IOC_CFG_DS_GEM_PORT_IX_ENCRYPTION                 ,
   BCM_PLOAM_IOC_CFG_DS_GEM_PORT_ID_ENCRYPTION                 ,
#endif
/*Return codes: 0, EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_STATE, EINVAL_PLOAM_ARG*/
#if defined(CONFIG_BCM96816)  || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
   BCM_PLOAM_IOC_CFG_GEM_PORT_QOS             , 
#endif
   BCM_PLOAM_IOC_DECFG_GEM_PORT               ,  /*Return codes: 0, EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_ENABLE_GEM_PORT              ,  /*Return codes: 0, EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_GET_GEM_PORT_CFG             ,  /*Return codes: 0, EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_GET_ALLOC_IDS                ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_OMCI_PORT_INFO           ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_SET_1ST_SN_PREAMBLE          ,  /*Return codes: EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_GET_GTC_PARAMS               ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_ONU_ID                   ,  /*Return codes: 0, ENODATA*/
   BCM_PLOAM_IOC_GET_FEC_MODE                 ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_ENCRYPTION_KEY           ,  /*Return codes: 0, ENODATA, EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_SET_SERIAL_PASSWD            ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_SERIAL_PASSWD            ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GET_DRIVER_VERSION           ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_CFG_GEM_PORT_GTCDS           ,  /*Return codes: EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_ARG, EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_GTCDS_CFG                    ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_MCAST_FILTER_ENTRY           ,  /*Return codes: EINVAL_PLOAM_ARG, EINVAL_PLOAM_NOENT*/
   BCM_PLOAM_IOC_GTCUS_CFG                    ,  /*Return codes: EINVAL_PLOAM_ARG, EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_GET_REGISTERED_MSGS          ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_GEN_PRBS                     ,  /*Return codes: EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_GET_PRBS_STATE               ,  /*Return codes: 0*/
   BCM_OPTICS_IOC_IF_CFG                      ,  /*Return codes: 0*/
   BCM_OPTICS_IOC_GET_IF_CFG                  ,  /*Return codes: 0*/
   BCM_AE_IOC_MODE                            ,  /*Return codes: EINVAL_PLOAM_STATE*/
   BCM_AE_IOC_STATUS                          ,  /*Return codes: EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_SET_TO1_TO2                  ,  /*Return codes: EINVAL_PLOAM_ARG*/
   BCM_PLOAM_IOC_GET_TO1_TO2                  ,  /*Return codes: 0*/
   BCM_PLOAM_IOC_SET_ALARM_SOAK_TIME          ,  /*Return codes: EINVAL_PLOAM_NOENT*/
   BCM_PLOAM_IOC_GET_ALARM_SOAK_TIME          ,  /*Return codes: EINVAL_PLOAM_NOENT*/
   BCM_PLOAM_IOC_SET_CFG_GEM_PORT_US_SHPR     ,  /*Return codes: 0, EINVAL_PLOAM_NOENT, EINVAL_PLOAM_ARG, EINVAL_PLOAM_GEM_PORT,
                                                       *              EINVAL_PLOAM_BT_OUT_OF_RANGE, EINVAL_PLOAM_INTERNAL_ERR */
   BCM_PLOAM_IOC_GET_CFG_GEM_PORT_US_SHPR     ,  /*Return codes: 0, EINVAL_PLOAM_NOENT, EINVAL_PLOAM_GEM_PORT*/
   BCM_PLOAM_IOC_SET_ACK_OFFSET               ,  /*Return codes: 0, EINVAL*/
   BCM_PLOAM_IOC_GET_ACK_OFFSET               ,  /*Return codes: 0 */
   BCM_AE_IOC_SET_MODE                        ,  /*Return codes: 0 */
   BCM_AE_IOC_GET_MODE                        ,  /*Return codes: 0 */
   BCM_AE_IOC_START                           ,  /*Return codes: 0 */
   BCM_AE_IOC_STOP                            ,  /*Return codes: 0 */
#if defined(CONFIG_BCM_GPON_AE_AUTO_SWITCH)
   BCM_PLOAM_IOC_DISABLE_GPON_AE_AUTO_SWITCH  ,  /*Return codes: 0 */
   BCM_PLOAM_IOC_GET_GPON_AE_AUTO_SWITCH      ,  /*Return codes: 0 */
#endif /* CONFIG_BCM_GPON_AE_AUTO_SWITCH */

   BCM_PLOAM_IOC_GET_TCONT_CFG                , 
#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
   BCM_PLOAM_IOC_CFG_GEM_PORT_GTCDS_X         ,  /*Return codes: EINVAL_PLOAM_GEM_PORT, EINVAL_PLOAM_ARG, EINVAL_PLOAM_STATE*/
   BCM_PLOAM_IOC_GET_GEM_MIB_IDX_COUNTERS     ,  /*Return codes: 0 */
   BCM_PLOAM_IOC_GET_US_QUEUE_IDX_COUNTERS    ,  /*Return codes: 0 */
   BCM_PLOAM_IOC_CFG_TCONT_QUEUE_MAP          , 
   BCM_PLOAM_IOC_DECFG_TCONT_QUEUE_MAP          ,
   BCM_PLOAM_IOC_CFG_TCONT_SCHD_POLICY        , 
   BCM_PLOAM_IOC_CFG_TCONT_ALLOCID            , 
   BCM_PLOAM_IOC_DECFG_TCONT_ALLOCID          ,
   BCM_PLOAM_IOC_CFG_US_QOS                   , 
   BCM_PLOAM_IOC_CFG_US_SHAPER                , 
   BCM_PLOAM_IOC_GET_US_QUEUE_CFG             , 
   BCM_PLOAM_IOC_SET_GEM_BLOCK_LENGTH         , 
   BCM_PLOAM_IOC_GET_GEM_BLOCK_LENGTH         , 
   BCM_AE_IOC_SET_LOOPBACK_MODE               ,  
   BCM_AE_IOC_GET_LOOPBACK_MODE               ,  
   BCM_PLOAM_IOC_CONFIG_GEM_TO_DSMIBIDX_MAP   ,
   BCM_PLOAM_IOC_SET_TIME_OF_DAY              ,
   BCM_PLOAM_IOC_GET_TIME_OF_DAY              ,
   BCM_PLOAM_IOC_CFG_GTC_DS_SCHD              ,
   BCM_PLOAM_IOC_GET_GTC_DS_SCHD_CFG          ,
#endif /* 6818 || 68500 || 6838 */
#if defined(CONFIG_BCM_GPON_DDRO)
   BCM_PLOAM_IOC_GET_DDR_OFFLOAD_CFG          , 
   BCM_PLOAM_IOC_SET_DDR_OFFLOAD_CFG          ,  
   BCM_PLOAM_IOC_CFG_WRED_PROFILE             ,
   BCM_PLOAM_IOC_CFG_WRED_Q                   ,
   BCM_PLOAM_IOC_GET_WRED_PROFILE             ,
   BCM_PLOAM_IOC_GET_WRED_Q                   ,
   BCM_PLOAM_IOC_GET_MATCHING_WRED_PROFILES   ,
#endif /* CONFIG_BCM_GPON_DDRO */
#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
   BCM_PLOAM_IOC_DECFG_TCONT_SCHD_POLICY      ,
   BCM_PLOAM_IOC_DECFG_US_SHAPER              ,
   BCM_PLOAM_IOC_DECFG_US_QOS                 ,
   BCM_PLOAM_IOC_CFG_LAN_PORT,
#endif
} BCM_Ploam_CommandId;

/*Ploam Driver Event IDs*/
#define BCM_PLOAM_EVENT_ONU_STATE_CHANGE       (1<<0)
#define BCM_PLOAM_EVENT_ALARM                  (1<<1)
#define BCM_PLOAM_EVENT_OMCI_PORT_ID           (1<<2)
#define BCM_PLOAM_EVENT_USER_PLOAM_RX_OVERFLOW (1<<3)
#define BCM_PLOAM_EVENT_USER_PLOAM_TX_OVERFLOW (1<<4)
#define BCM_PLOAM_EVENT_GEM_STATE_CHANGE       (1<<5)
#define BCM_PLOAM_EVENT_USER_TOD_RX_OVERFLOW   (1<<6)

/*Type of ioctl argument pointer for commands BCM_PLOAM_IOC_GET_EVENT_STATUS*/
typedef struct {
  UINT32 eventBitmap; /*Bitwise OR of BCM_PLOAM_EVENT_* flags*/
  UINT32 eventMask; /*Bitwise OR of BCM_PLOAM_EVENT_* flags*/

} BCM_Ploam_EventStatusInfo ;

/*Type of ioctl argument pointer for commands BCM_PLOAM_IOC_MASK_EVENT*/
typedef struct {
  UINT32 eventMask; /*Bitwise OR of BCM_PLOAM_EVENT_* flags*/

} BCM_Ploam_EventMaskInfo ;

/*Ploam Driver Alarms*/
#define BCM_PLOAM_ALARM_ID_APC_FAIL  (1<<0)
#define BCM_PLOAM_ALARM_ID_LOS       (1<<1)
#define BCM_PLOAM_ALARM_ID_LOL       (1<<2)
#define BCM_PLOAM_ALARM_ID_LOF       (1<<3)
#define BCM_PLOAM_ALARM_ID_LCDG      (1<<4)
#define BCM_PLOAM_ALARM_ID_SF        (1<<5)
#define BCM_PLOAM_ALARM_ID_SD        (1<<6)
#define BCM_PLOAM_ALARM_ID_SUF       (1<<7)
#define BCM_PLOAM_ALARM_ID_MEM       (1<<8)
#define BCM_PLOAM_ALARM_ID_DACT      (1<<9)
#define BCM_PLOAM_ALARM_ID_DIS      (1<<10)
#define BCM_PLOAM_ALARM_ID_PEE      (1<<11)
#define BCM_PLOAM_ALARM_ID_SF_OFF    (1<<12)
#define BCM_PLOAM_ALARM_ID_SD_OFF    (1<<13)

#define BCM_PLOAM_NUM_ALARMS 14

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_ALARM_STATUS*/
typedef struct {
  OUT UINT32 alarmStatusBitmap; /* Bitwise OR of BCM_PLOAM_ALARM_ID_* flags*/
  OUT UINT32 alarmEventBitmap;  /* Bitwise OR of BCM_PLOAM_ALARM_ID_* flags*/
  OUT UINT32 alarmMaskBitmap;   /* Bitwise OR of BCM_PLOAM_ALARM_ID_* flags*/

} BCM_Ploam_AlarmStatusInfo ;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_MASK_ALARM*/
typedef struct {
  IN UINT32 alarmMaskBitmap; /* Bitwise OR of BCM_PLOAM_ALARM_ID_* flags */

} BCM_Ploam_MaskAlarmInfo ;

/*Type of ioctl argument pointer for commands BCM_PLOAM_IOC_SET_SF_SD_THRESHOLD
 *and BCM_PLOAM_IOC_GET_SF_SD_THRESHOLD*/
typedef struct {
  INOUT UINT8 sf_exp; /*Range: [3..8]*/
  INOUT UINT8 sd_exp; /*Range: [sf_exp+1..9]*/

} BCM_Ploam_SFSDthresholdInfo ;

#define BCM_PLOAM_DIRECTION_US 0
#define BCM_PLOAM_DIRECTION_DS 1

#define BCM_PLOAM_NUM_PLOAM_PAYLOAD_BYTES 10

/*Type of object returned when reading from "bcm_user_ploam" device file */
typedef struct {
  UINT16 seqNum;
  UINT8 direction; /*BCM_PLOAM_DIRECTION_US or BCM_PLOAM_DIRECTION_DS*/
  UINT8 onu_id;
  UINT8 msg_id;
  UINT8 payload[BCM_PLOAM_NUM_PLOAM_PAYLOAD_BYTES];
  UINT8 crc;

} BCM_Ploam_Msg;

/*Legacy:*/
typedef BCM_Ploam_Msg BCM_Ploam_msg;


/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_REGISTER_MSG*/
typedef struct {
  IN UBOOL8 registerAll;
  IN UINT8 direction; /*BCM_PLOAM_DIRECTION_US or BCM_PLOAM_DIRECTION_DS*/
  IN UINT8 msgId;

} BCM_Ploam_RegisterMsgInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_UNREGISTER_MSG*/
typedef struct {
  IN UBOOL8 unregisterAll;
  IN UINT8 direction; /*BCM_PLOAM_DIRECTION_US or BCM_PLOAM_DIRECTION_DS*/
  IN UINT8 msgId;

} BCM_Ploam_UnregisterMsgInfo ;

/*Type of ioctl argument pointer for BCM_PLOAM_IOC_GET_REGISTERED_MSGS*/
typedef struct {
  OUT UINT32 usMsgMap[8];
  OUT UINT32 dsMsgMap[8];

} BCM_Ploam_RegisteredMsgsInfo;

/*ONU Administrative states*/
typedef enum {
  BCM_PLOAM_ASTATE_OFF=0,
  BCM_PLOAM_ASTATE_ON

} BCM_Ploam_AdminState;

/*ONU Operational states*/
typedef enum {
  /* GPON States */
  BCM_PLOAM_OSTATE_INITIAL_O1=1,
  BCM_PLOAM_OSTATE_STANDBY_O2,
  BCM_PLOAM_OSTATE_SERIAL_NUMBER_O3,
  BCM_PLOAM_OSTATE_RANGING_O4,
  BCM_PLOAM_OSTATE_OPERATION_O5,
  BCM_PLOAM_OSTATE_POPUP_O6,
  BCM_PLOAM_OSTATE_EMERGENCY_STOP_O7,
  BCM_PLOAM_OSTATE_DEACTIVATED_O8,
#if defined(CONFIG_BCM_GPON_AE_AUTO_SWITCH)
  /* AE States */
  BCM_PLOAM_OSTATE_AE_INITIAL_O1,
  BCM_PLOAM_OSTATE_AE_OPERATION_O5,
#endif /* CONFIG_BCM_GPON_AE_AUTO_SWITCH */
  BCM_PLOAM_OSTATE_MAX,
} BCM_Ploam_OperState;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_START*/
typedef struct {
  IN UINT8 initOperState; /*BCM_PLOAM_OSTATE_INITIAL_O1 or BCM_PLOAM_OSTATE_EMERGENCY_STOP_O7*/

} BCM_Ploam_StartInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_STOP*/
typedef struct {
  IN UBOOL8 sendDyingGasp;

} BCM_Ploam_StopInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_STATE*/
typedef struct {
  OUT UINT8 adminState; /*BCM_PLOAM_ASTATE_ON or BCM_PLOAM_ASTATE_OFF*/
  OUT UINT8 operState;  /*BCM_PLOAM_OSTATE_* */

} BCM_Ploam_StateInfo;

/*Legacy:*/
typedef BCM_Ploam_StateInfo BCM_Ploam_stateInfo;


/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_MESSAGE_COUNTERS*/
typedef struct {
  IN UINT32 reset;
  OUT UINT32 crcErrors;
  OUT UINT32 rxPloamsTotal;
  OUT UINT32 rxPloamsUcast;
  OUT UINT32 rxPloamsBcast;
  OUT UINT32 rxPloamsDiscarded;
  OUT UINT32 rxPloamsNonStd;
  OUT UINT32 txPloams;
  OUT UINT32 txPloamsNonStd;

} BCM_Ploam_MessageCounters;

/*Legacy:*/
typedef BCM_Ploam_MessageCounters BCM_Ploam_messageCounters;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_GTC_COUNTERS*/
typedef struct {
  IN UINT32 reset;
  OUT UINT32 bipErr;
  OUT UINT32 fecCerr;
  OUT UINT32 fecUerr;
  OUT UINT32 fecCWs;
  OUT UINT32 fecSecs;
  OUT UINT32 hecCerr;
  OUT UINT32 hecUerr;

} BCM_Ploam_GtcCounters;

/*Legacy:*/
typedef BCM_Ploam_GtcCounters BCM_Ploam_gtcCounters;

#define BCM_PLOAM_GEM_PORT_IDX_ALL 0xfffe
#define BCM_PLOAM_GEM_MIB_IDX_ALL  0xfe
#define BCM_PLOAM_US_QUEUE_IDX_ALL  0xfe
#define BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED 0xffff
#define BCM_PLOAM_GEM_PORT_ID_UNASSIGNED 0xffff
#define BCM_PLOAM_US_QUEUE_IDX_UNASSIGNED 0xff
#define BCM_PLOAM_DS_MIB_IDX_UNASSIGNED 0xff

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_GEM_PORT_COUNTERS*/
typedef struct {
  IN UINT32 reset;
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *BCM_PLOAM_GEM_PORT_IDX_ALL and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
  OUT UINT32 rxBytes;
  OUT UINT32 txBytes;
  OUT UINT32 rxFragments;
  OUT UINT32 txFragments;
  OUT UINT32 txFrames;
  OUT UINT32 rxFrames;
  OUT UINT32 rxDroppedFrames;
  OUT UINT32 txDroppedFrames;
  OUT UINT32 rxMcastAcceptedFrames;
  OUT UINT32 rxMcastDroppedFrames;
#if defined(CONFIG_BCM96818)
  OUT UINT32 txWredDropsInBytes;
#endif

} BCM_Ploam_GemPortCounters;

/*Legacy:*/
typedef BCM_Ploam_GemPortCounters BCM_Ploam_gemPortCounters;

#define BCM_PLOAM_ALLOC_ID_UNASSIGNED 0xffff
#define BCM_PLOAM_MAX_GEM_ID          0x0fff
#define BCM_PLOAM_MAX_ALLOC_ID        0x0fff

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_TCONT_CFG*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_TCONTS-1]*/
  IN  UINT8  tcontIdx;
  OUT UINT8  enabled;
  OUT UINT16 allocID;
  OUT UINT32 queueMap; 
  OUT UINT8  schdPolicy; 
  OUT UINT16 refCount;
  OUT UINT8  ploamCreated; 
  OUT UINT8  omciCreated; 

} BCM_Ploam_TcontInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_CFG_TCONT*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_TCONTS-1]*/
  IN UINT8  tcontIdx;
  IN UINT16 allocID; /*Range: [0..BCM_PLOAM_MAX_ALLOC_ID or BCM_PLOAM_ALLOC_ID_UNASSIGNED]*/

} BCM_Ploam_TcontAllocIdInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_CFG_GEM_PORT*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1], and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED
  *to have the driver allocate the port*/
  INOUT UINT16 gemPortIndex;
  IN UINT16 gemPortID; /*Range: [0..BCM_PLOAM_MAX_GEM_ID]*/
 /*Accepted values: [0..BCM_PLOAM_MAX_ALLOC_ID], and BCM_PLOAM_ALLOC_ID_UNASSIGNED
  *for a DS-only GEM Port*/
  IN UINT16 allocID;
  /* Accepted values: [0..BCM_PLOAM_MAX_GTCUS_QUEUES-1], and BCM_PLOAM_US_QUEUE_IDX_UNASSIGNED */
  IN UINT8  usQueueIndex; /* Required only for 6818G chip */
  /* Accepted values: [0..BCM_PLOAM_NUM_GEM_MIB_IDXS-1], and BCM_PLOAM_DS_MIB_IDX_UNASSIGNED */
  INOUT UINT8 dsMibIndex; /* Required only for 6818G chip */

} BCM_Ploam_CfgGemPortInfo;

typedef BCM_Ploam_CfgGemPortInfo BCM_Ploam_cfgGemPortInfo;
#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
#define BCM_PLOAM_MAX_PRIORITY 7
#define BCM_PLOAM_MAX_WEIGHT 63
#define BCM_PLOAM_MIN_WEIGHT 1
#else
#define BCM_PLOAM_MAX_PRIORITY 31
#define BCM_PLOAM_MAX_WEIGHT 127
#endif


/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_CFG_GEM_PORT_QOS*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
  IN UINT8 priority; /*Range: [0..BCM_PLOAM_MAX_PRIORITY]*/
  IN UINT8 weight; /*Range: [0..BCM_PLOAM_MAX_WEIGHT]*/

} BCM_Ploam_CfgGemPortQosInfo;

/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_CFG_US_QOS */
typedef struct {
    INOUT UINT8  spEnabled; /* Accepted values 0..1 */
    INOUT UINT8  priority;  /*Only used if spEnabled=1; Range: [0..BCM_PLOAM_MAX_PRIORITY]*/
    INOUT UINT8  weight;    /*Range: [0..BCM_PLOAM_MAX_WEIGHT]*/
}BCM_Ploam_UsQosInfo;
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_MAX_GTCUS_QUEUES-1]*/
  INOUT UINT16 usQueueIdx;
  INOUT BCM_Ploam_UsQosInfo qosInfo;

} BCM_Ploam_UsQosCfgInfo;

/*Multicast Filter Mode*/
typedef enum {
  BCM_PLOAM_GTCDS_MCAST_FILTER_OFF=0,
  BCM_PLOAM_GTCDS_MCAST_FILTER_DMAC=1,
  BCM_PLOAM_GTCDS_MCAST_FILTER_DMAC_OVID=2,
  BCM_PLOAM_GTCDS_MCAST_FILTER_DMAC_OVID_IVID=3

} BCM_Ploam_McastFilterMode;

#define BCM_PLOAM_GTCDS_NUM_QUEUES 8

/*Type of ioctl arguement pointer for command BCM_PLOAM_IOC_CFG_GEM_PORT_GTCDS*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
  IN UINT8 queueIdx; /*Range: [0..BCM_PLOAM_GTCDS_NUM_QUEUES-1]*/
  IN UINT8 mcastFilterMode; /*BCM_Ploam_McastFilterMode*/

} BCM_Ploam_CfgGemPortGtcDsInfo;

typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
  IN UINT8 queueIdx; /*Range: [0..BCM_PLOAM_GTCDS_NUM_QUEUES-1]*/
  IN UINT8 mcastFilterMode; /*BCM_Ploam_McastFilterMode*/
  IN UINT8 destQPbitBased; 
  IN UINT8 useTag1Pbits;
} BCM_Ploam_CfgGemPortGtcDsInfo_X;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_DECFG_GEM_PORT*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *BCM_PLOAM_GEM_PORT_IDX_ALL and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;

} BCM_Ploam_DecfgGemPortInfo;


#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_UPDATE_GEM_PORT_TO_IDX_MAP*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1] and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  IN UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  IN UINT16 gemPortID;
  IN UINT8  dsMibIdx;
} BCM_Ploam_ConfigGemToDsMibIdxMapInfo;

/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_CFG_GTC_DS_SCHD and BCM_PLOAM_IOC_GET_GTC_DS_SCHD_CFG*/
#define BCM_PLOAM_GTC_DS_MAX_QUEUES 8
#define BCM_PLOAM_GTC_DS_MAX_QUEUE_THRESHOLD   (0x480)
#define BCM_PLOAM_GTC_DS_MIN_QUEUE_THRESHOLD   (0x020)

typedef struct {
 /*Accepted values: [BCM_PLOAM_GTC_DS_MIN_QUEUE_THRESHOLD..BCM_PLOAM_GTC_DS_MAX_QUEUE_THRESHOLD]*/
  INOUT UINT16 qThreshold[BCM_PLOAM_GTC_DS_MAX_QUEUES];

} BCM_Ploam_GtcDsSchdCfgInfo;

#endif

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_ENABLE_GEM_PORT*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
  IN UBOOL8 enable;

} BCM_Ploam_EnableGemPortInfo;

/*Legacy:*/
typedef BCM_Ploam_EnableGemPortInfo BCM_Ploam_enableGemPortInfo;

/*GEM Port Flags*/
#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENABLED_BIT_POS        (0)
#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENCRYPTED_BIT_POS      (1)
#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_OPERATIONAL_BIT_POS    (2)
#define BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_PBIT_BASED_BIT_POS     (3) 
#define BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_USE_TAG1_PBIT_BIT_POS  (4) 

#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENABLED (1<<BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENABLED_BIT_POS)
#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENCRYPTED (1<<BCM_PLOAM_GEM_PORT_FLAGS_PORT_ENCRYPTED_BIT_POS)
#define BCM_PLOAM_GEM_PORT_FLAGS_PORT_OPERATIONAL (1<<BCM_PLOAM_GEM_PORT_FLAGS_PORT_OPERATIONAL_BIT_POS)
/* DS DestQ selection based on P-bits */
#define BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_PBIT_BASED (1<<BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_PBIT_BASED_BIT_POS) 
/* DS DestQ selection based on inner tag P-bits */
#define BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_USE_TAG1_PBIT (1<<BCM_PLOAM_GEM_PORT_FLAGS_DS_Q_USE_TAG1_PBIT_BIT_POS) 

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_GEM_PORT_CFG*/
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
  *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
  INOUT UINT16 gemPortIndex;
 /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
  *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
  INOUT UINT16 gemPortID;
 /*Accepted values: [0..BCM_PLOAM_MAX_ALLOC_ID], or BCM_PLOAM_ALLOC_ID_UNASSIGNED
  *for a DS-only GEM Port*/
  OUT UINT16 allocID;
  OUT UINT16 flags;           /*Bitwise OR of BCM_PLOAM_GEM_PORT_FLAGS_* */
  OUT UINT16 priority;        /* deprecated in 6818G - moved to UsQos */
  OUT UINT16 weight;          /* deprecated in 6818G - moved to UsQos */
  OUT UINT8  mibCounterIdx;   /* Range: 0..BCM_PLOAM_NUM_GEM_MIB_IDXS-1*/
  OUT UINT8  usQueueIdx;      /* GTCUS destQ. Range: 0..BCM_PLOAM_NUM_GTCUS_QUEUES_PER_TCONT */
  OUT UINT8  dsQueueIdx;      /* GTCDS output queue. Range: 0..7*/
  OUT UINT8  mcastFilterMode; /* BCM_Ploam_McastFilterMode*/
} BCM_Ploam_GemPortInfo;

/*Legacy:*/
typedef BCM_Ploam_GemPortInfo BCM_Ploam_gemPortInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_ALLOC_IDS*/
typedef struct {
  OUT UINT16 numAllocIDs;
  OUT UINT16 allocIDs[BCM_PLOAM_NUM_DATA_TCONTS];

} BCM_Ploam_AllocIDs;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_OMCI_PORT_INFO*/
typedef struct {
  OUT UBOOL8 omciGemPortActivated;
  OUT UBOOL8 encrypted;
  OUT UINT16 omciGemPortID;

} BCM_Ploam_OmciPortInfo;

typedef BCM_Ploam_OmciPortInfo BCM_Ploam_omciPortInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_SET_1ST_SN_PREAMBLE*/
typedef struct {
  IN UBOOL8 enable;
  IN UINT8 preambleLength1;
  IN UINT8 preambleLength2;

} BCM_Ploam_1stSnPreambleInfo;

/*Power Level Modes*/
#define BCM_PLOAM_POWER_LEVEL_NORMAL 0
#define BCM_PLOAM_POWER_LEVEL_MIN_3DB 1
#define BCM_PLOAM_POWER_LEVEL_MIN_6DB 2

#define BCM_PLOAM_NUM_DELIMITER_BYTES 3

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_GTC_PARAMS*/
typedef struct {
  OUT UINT8 numGuardBits;
  OUT UINT8 numType1PreambleBits;
  OUT UINT8 numType2PreambleBits;
  OUT UINT8 numType3PreambleBits;
  OUT UINT8 type3PreamblePattern;
  OUT UINT8 delimiterData[BCM_PLOAM_NUM_DELIMITER_BYTES];
  OUT UINT8 powerLevel; /*One of BCM_PLOAM_POWER_LEVEL_* */
  OUT UINT32 eqd;

} BCM_Ploam_GtcParamInfo;

/*Legacy:*/
typedef BCM_Ploam_GtcParamInfo BCM_Ploam_gtcParamInfo;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_ONU_ID*/
typedef struct {
  OUT UINT8 onuId;

} BCM_Ploam_GetOnuIdInfo;

#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_SET_GEM_BLOCK_LENGTH/BCM_PLOAM_IOC_GET_GEM_BLOCK_LENGTH*/
typedef struct {
  INOUT UINT32 gemBlkLen;

} BCM_Ploam_GemBlkLenInfo;

/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_SET_TIME_OF_DAY*/
typedef struct {
  UINT16 secondsMSB; /* 16-bit MSB of seconds */
  UINT32 secondsLSB; /* 32-bit LSB of seconds */
  UINT32 nanoSeconds;/* 32-bit nano seconds */

} BCM_Ploam_TimeStamp; /* IEEE 1588-2008 time representation */

typedef struct {
  IN BCM_Ploam_TimeStamp tStampN;  /* Time stamp at the mentioned superframe */
  IN UINT32 superframe;            /* Super frame to match */
  IN UINT8 pulseWidth;             /* Width of generated pulse in 155MHz clock cycles */
  IN UINT8 enable;                 /* enable or disable ToD */
  IN UINT8 enableUsrEvent;         /* enable or disable User Event generation */

} BCM_Ploam_TimeOfDayInfo;

typedef struct {
  OUT BCM_Ploam_TimeStamp currentTime; /* Time at which next pulse/UsrEvent is generated */
  OUT UINT32 eventSeqNum;              /* Running Event sequence number count */

} BCM_Ploam_TodEventMsg;
#endif /* 6818 */
/*FEC mode bitmap flags:*/
#define BCM_PLOAM_FEC_MODE_FLAG_US_FEC_ON (1<<0)
#define BCM_PLOAM_FEC_MODE_FLAG_DS_FEC_ON (1<<1)

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_FEC_MODE*/
typedef struct {
  OUT UINT32 fecMode; /*Bitwise OR of BCM_PLOAM_FEC_MODE_FLAG_* */

} BCM_Ploam_GetFecModeInfo;

#define BCM_PLOAM_ENCRYPTION_KEY_SIZE_BYTES 16

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_ENCRYPTION_KEY*/
typedef struct {
  OUT UINT8 key[BCM_PLOAM_ENCRYPTION_KEY_SIZE_BYTES];

} BCM_Ploam_GetEncryptionKeyInfo;

typedef BCM_Ploam_GetEncryptionKeyInfo BCM_PloamGetEncryptionKeyInfo;

#if defined(CONFIG_BCM96816)

#define BCM_PLOAM_MIN_FRAG_SIZE_MIN 2
#define BCM_PLOAM_MIN_FRAG_SIZE_MAX 255
#define BCM_PLOAM_CELL_GRANT_SIZE_MIN 1
#define BCM_PLOAM_CELL_GRANT_SIZE_MAX 255

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GTCUS_CFG*/
typedef struct {
  IN UINT16 minFragSize; /*Range [BCM_PLOAM_MIN_FRAG_SIZE_MIN..BCM_PLOAM_MIN_FRAG_SIZE_MAX]*/
  IN UINT16 cellGrantSize; /*Range [BCM_PLOAM_CELL_GRANT_SIZE_MIN..BCM_PLOAM_CELL_GRANT_SIZE_MAX]*/
  IN UINT16 flags; /*Reserved*/

} BCM_Ploam_GtcUsCfgInfo;
#endif /* 6816 */

/*GTCDS Flags*/
#define BCM_PLOAM_GTCDS_FLAG_EN_PBIT_QID_MAPPING 1
#define BCM_PLOAM_GTCDS_FLAG_INNER_PBIT_QID_MAPPING 2

/*Enter this TPID to keep current value*/
#define BCM_PLOAM_TPID_KEEP_CURRENT 0xffff
#define GTCDS_VLAN_TPID_MAX          4

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GTCDS_CFG*/
typedef struct {
  /* NOTE : flags are only used in 6816 , not in 6818G */
  IN UINT16 flags; /*Bitwise OR of BCM_PLOAM_GTCDS_FLAG_* */
  IN UINT16 innerVlanTpids[GTCDS_VLAN_TPID_MAX];
  IN UINT16 outerVlanTpids[GTCDS_VLAN_TPID_MAX];

} BCM_Ploam_GtcDsCfgInfo;

/*Multicast filter operations*/
typedef enum {
  BCM_PLOAM_MCAST_FILTER_OP_CLR=0,
  BCM_PLOAM_MCAST_FILTER_OP_ADD=1,
  BCM_PLOAM_MCAST_FILTER_OP_CLR_ALL=2,
  BCM_PLOAM_MCAST_FILTER_OP_READ_BEGIN=3,
  BCM_PLOAM_MCAST_FILTER_OP_READ_NEXT=4,

} BCM_Ploam_McastFilterOps;

#define BCM_PLOAM_VID_NOT_USED 0xffff
#define BCM_PLOAM_MAX_NUM_MCAST_FILTER_ENTRIES 512

/*Type of ioctl argument pointer for command BCM_IOC_MCAST_FILTER_ENTRY*/
typedef struct {
  IN UINT16 op; /*BCM_Ploam_McastFilterOps*/
  IN UINT16 innerVid; /*Range: [0..4095] or BCM_PLOAM_VID_NOT_USED*/
  IN UINT16 outerVid; /*Range: [0..4095] or BCM_PLOAM_VID_NOT_USED*/
  /*48 Bit Multicast MAC address.
   *The following must be true (DA is the MAC address, bit indexed):
   *(DA[47:23] == 01-00-5E-'0') || (DA[47:32] == 33-33)*/
  IN UINT8 mcastMacAddr[6];

} BCM_Ploam_McastFilterEntry;

#define BCM_PLOAM_SERIAL_NUMBER_SIZE_BYTES 8
#define BCM_PLOAM_PASSWORD_SIZE_BYTES 10

/*Type of ioctl argument pointer for commands BCM_PLOAM_IOC_SET_SERIAL_PASSWD and
 *BCM_PLOAM_IOC_GET_SERIAL_PASSWD*/
typedef struct {
  INOUT UINT8 serialNumber[BCM_PLOAM_SERIAL_NUMBER_SIZE_BYTES];
  INOUT UINT8 password[BCM_PLOAM_PASSWORD_SIZE_BYTES];

} BCM_Ploam_SerialPasswdInfo;

typedef BCM_Ploam_SerialPasswdInfo BCM_Ploam_serialPasswdInfo;

typedef enum {
  BCM_GPON_SERDES_SATA_PRBS7=0,
  BCM_GPON_SERDES_SATA_PRBS15=1,
  BCM_GPON_SERDES_SATA_PRBS23=2,
  BCM_GPON_SERDES_SATA_PRBS31=3,
  BCM_GPON_SERDES_WRAPPER_PSEUDO_RANDOM=4

} BCM_Ploam_PseudoRandomMode;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GEN_PRBS*/
typedef struct {
  IN UINT8 enable;
  IN UINT8 mode; /* BCM_PLOAM_PRBS* */

} BCM_Ploam_GenPrbsInfo;

#define BCM_OPTICS_FLAG_AH_LOS   1
#define BCM_OPTICS_FLAG_AH_BEN   2
#define BCM_OPTICS_FLAG_LVDS_BEN_OE  4
#define BCM_OPTICS_FLAG_LVTTL_BEN_OE 8

/*Type of ioctl argument pointer for command BCM_OPTICS_IOC_IF_CFG and BCM_OPTICS_IOC_GET_IF_CFG*/
typedef struct {
  INOUT UINT8 flags; /*Bitwise OR of BCM_OPTICS_FLAG_* flags*/
  INOUT UINT8 laserTon;
  INOUT UINT8 laserToff;
  INOUT UINT32 txIdlePattern;

} BCM_Optics_IfCfgInfo;

#define BCM_AE_MODE_FLAG_SGMII    1

/*Type of ioctl argument pointer for command BCM_AE_IOC_MODE*/
typedef struct {
  IN UINT8 flags; /*Bitwise OR of BCM_AE_MODE_FLAG_* flags*/

} BCM_AE_ModeInfo;

/*The following BCM_AE_STATUS_* defines are deprecated.
 *Use the BCM_AE_STATUS0_* defines instead.
 */
#define BCM_AE_STATUS_TX_FIFO_ERR_DETECTED (1<<15)
#define BCM_AE_STATUS_RX_FIFO_ERR_DETECTED (1<<14)
#define BCM_AE_STATUS_FALSE_CARRIER_DETECTED (1<<13)
#define BCM_AE_STATUS_CRC_ERR_DETECTED (1<<12)
#define BCM_AE_STATUS_TX_ERR_DETECTED (1<<11)
#define BCM_AE_STATUS_RX_ERR_DETECTED (1<<10)
#define BCM_AE_STATUS_CARRIER_EXTENDED_ERR_DETECTED (1<<9)
#define BCM_AE_STATUS_EARLY_END_EXTENSION_DETECTED (1<<8)
#define BCM_AE_STATUS_LINK_STATUS_CHG (1<<7)
#define BCM_AE_STATUS_PAUSE_RESOLUTION_RXSIDE (1<<6)
#define BCM_AE_STATUS_PAUSE_RESOLUTION_TXSIDE (1<<5)
#define BCM_AE_STATUS_SPEED_STATUS_HI (1<<4)
#define BCM_AE_STATUS_SPEED_STATUS_LO (1<<3)
#define BCM_AE_STATUS_DUPLEX_STATUS (1<<2)
#define BCM_AE_STATUS_LINK_STATUS (1<<1)
#define BCM_AE_STATUS_SGMII_MODE (1<<0)

#define BCM_AE_STATUS_SPEED(flags) (((flags)>>3)&3)
#define BCM_AE_STATUS_SPEED_10MBPS 0
#define BCM_AE_STATUS_SPEED_100MBPS 1
#define BCM_AE_STATUS_SPEED_1000MBPS 2

#define BCM_AE_STATUS0_TX_FIFO_ERR_DETECTED (1<<15)
#define BCM_AE_STATUS0_RX_FIFO_ERR_DETECTED (1<<14)
#define BCM_AE_STATUS0_FALSE_CARRIER_DETECTED (1<<13)
#define BCM_AE_STATUS0_CRC_ERR_DETECTED (1<<12)
#define BCM_AE_STATUS0_TX_ERR_DETECTED (1<<11)
#define BCM_AE_STATUS0_RX_ERR_DETECTED (1<<10)
#define BCM_AE_STATUS0_CARRIER_EXTENDED_ERR_DETECTED (1<<9)
#define BCM_AE_STATUS0_EARLY_END_EXTENSION_DETECTED (1<<8)
#define BCM_AE_STATUS0_LINK_STATUS_CHG (1<<7)
#define BCM_AE_STATUS0_PAUSE_RESOLUTION_RXSIDE (1<<6)
#define BCM_AE_STATUS0_PAUSE_RESOLUTION_TXSIDE (1<<5)
#define BCM_AE_STATUS0_SPEED_STATUS_HI (1<<4)
#define BCM_AE_STATUS0_SPEED_STATUS_LO (1<<3)
#define BCM_AE_STATUS0_DUPLEX_STATUS (1<<2)
#define BCM_AE_STATUS0_LINK_STATUS (1<<1)
#define BCM_AE_STATUS0_SGMII_MODE (1<<0)

#define BCM_AE_STATUS0_SPEED(flags) (((flags)>>3)&3)
#define BCM_AE_STATUS0_SPEED_10MBPS 0
#define BCM_AE_STATUS0_SPEED_100MBPS 1
#define BCM_AE_STATUS0_SPEED_1000MBPS 2

#define BCM_AE_STATUS1_SGMII_MODE_CHANGE (1<<15)
#define BCM_AE_STATUS1_CONSISTENCY_MISMATCH (1<<14)
#define BCM_AE_STATUS1_AUTONEG_RESOLUTION_ERR (1<<13)
#define BCM_AE_STATUS1_SGMII_SELECTOR_MISMATCH (1<<12)
#define BCM_AE_STATUS1_SYNC_STATUS_FAIL (1<<11)
#define BCM_AE_STATUS1_SYNC_STATUS_OK (1<<10)
#define BCM_AE_STATUS1_RUDI_C (1<<9)
#define BCM_AE_STATUS1_RUDI_L (1<<8)
#define BCM_AE_STATUS1_RUDI_INVALID (1<<7)
#define BCM_AE_STATUS1_LINK_DOWN_SYNC_LOSS (1<<6)
#define BCM_AE_STATUS1_IDLE_DETECT_STATE (1<<5)
#define BCM_AE_STATUS1_COMPLETE_ACK_STATE (1<<4)
#define BCM_AE_STATUS1_ACK_DETECT_STATE (1<<3)
#define BCM_AE_STATUS1_ABILITY_DETECT_STATE (1<<2)
#define BCM_AE_STATUS1_AN_DISABLE_LINK_OK_STATE (1<<1)
#define BCM_AE_STATUS1_AN_ENABLE_STATE (1<<0)

/*Type of ioctl argument pointer for command BCM_AE_IOC_STATUS*/
typedef struct {
#define BCM_AE_STATUS_NUM_REGS 2
  OUT UINT16 flags[2]; /*Bitwise OR of BCM_AE_STATUS0|1_* flags*/

} BCM_AE_StatusInfo;

#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/*Type of ioctl argument pointer for command 
  BCM_AE_IOC_SET_LOOPBACK_MODE and BCM_AE_IOC_GET_LOOPBACK_MODE*/
typedef struct {
  INOUT UINT8 enable; /* 0 - disable; 1 - enable loopback */

} BCM_AE_LoopbackModeInfo;
#endif /* 6818 */

#define BCM_PLOAM_MAX_TO1_MS 10000000
#define BCM_PLOAM_MAX_TO2_MS 10000000

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_SET_TO1_TO2 and
 *BCM_PLOAM_IOC_GET_TO1_TO2*/
typedef struct {
  INOUT UINT32 to1;
  INOUT UINT32 to2;

} BCM_Ploam_TO1TO2Info;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_SET_ALARM_SOAK_TIME and
 *BCM_PLOAM_IOC_GET_ALARM_SOAK_TIME*/
typedef struct {
  IN UINT16 alarmId; /*One of BCM_PLOAM_ALARM_ID_* */
  INOUT UINT16 declSoakTime10ms;
  INOUT UINT16 retireSoakTime10ms;

} BCM_Ploam_AlarmSoakTimeInfo;

/* Error Return codes */
#define EINVAL_PLOAM_DUPLICATE           0x102
#define EINVAL_PLOAM_INIT_OPER_STATE     0x103
#define EINVAL_PLOAM_GEM_PORT            0x104
#define EINVAL_PLOAM_GEM_PORT_ENABLED    0x105
#define EINVAL_PLOAM_STATE               0x109
#define EINVAL_PLOAM_ARG                 0x10a
#define EINVAL_PLOAM_NOENT               0x10b
#define EINVAL_PLOAM_BT_OUT_OF_RANGE     0x10c
#define EINVAL_PLOAM_INTERNAL_ERR        0x10d
#define EINVAL_PLOAM_GEM_MIB_IDX         0x10e
#define EINVAL_PLOAM_US_QUEUE_MAPPED     0x10f
#define EINVAL_PLOAM_US_QUEUE_IDX        0x110
#define EINVAL_PLOAM_US_QUEUE_PRIORITY   0x111
#define EINVAL_PLOAM_US_QUEUE_WEIGHT     0x112
#define EINVAL_PLOAM_RESOURCE_UNAVAIL    0x113

/* Max rate Kbps*/
#define BCM_PLOAM_MAX_US_RATE_KBPS (1244160)        /* 19440*8*8000/1000*/
#if defined(CONFIG_BCM96818)
#define BCM_PLOAM_MAX_BURST_SIZE_BYTES   (0xFFFFFF)  /* in Bytes*/
#define BCM_PLOAM_MIN_US_RATE_KBPS       (128)       /* Required to fit bucket size in 16 bits register
                                                      * This could be changed if other variables are changed. */
#endif

#if defined(CONFIG_BCM96816) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/* Gem Port Upstream Shaping/Rate limiting */

/* Type of service */
#define BCM_PLOAM_US_SHAPING_UBR  (0)               /* Unspecified Rate */
#define BCM_PLOAM_US_SHAPING_VBR  (1)               /* Variable Rate */
#define BCM_PLOAM_US_SHAPING_CBR  (2)               /* Constant Rate */
/* Min rate Kbps*/
#define BCM_PLOAM_MIN_US_RATE_KBPS (4)              /* Required to fit the PCR/SCR value in 24 bits register*/
#define BCM_PLOAM_MIN_BURST_SIZE_BYTES   (96)       /* in Bytes*/
#define BCM_PLOAM_MAX_BURST_SIZE_BYTES   (3145680)  /* in Bytes*/

typedef struct
{
  INOUT UINT16 algo;    /* 0-UBR, 1-VBR, 2-CBR */
  union
  {
     struct
     {
        INOUT UINT32 rateKbps;   /* Max Rate */   
     }cbr;
     struct
     {
        INOUT UINT32 minRateKbps;  /* Minimun gauranteed rate - Only valid if MCR is enabled */     
        INOUT UINT16 mcrEnable; /* 1 - MCR Enable for UBR, 0- Normal UBR */
     }ubr;
     struct
     {
        INOUT UINT32 peakRateKbps;     /* Peak Rate */ 
        INOUT UINT32 sustainedRateKbps;/* Sustained Rate */
        INOUT UINT32 maxBurstSizeBytes;     /* Max burst size in bytes (multiple of 48 Bytes) to burst at Peak Rate */
     }vbr;
  }algoSpecific;
}BCM_Ploam_GemPortShaperInfo;

typedef struct
{
   /*Accepted values: [0..BCM_PLOAM_NUM_DATA_GEM_PORTS-1],
    *and BCM_PLOAM_GEM_PORT_IDX_UNASSIGNED*/
   INOUT UINT16 gemPortIndex; 
   /*Accepted values: [0..BCM_PLOAM_MAX_GEM_ID] and
    *BCM_PLOAM_GEM_PORT_ID_UNASSIGNED.*/
   IN UINT16 gemPortID;
   INOUT BCM_Ploam_GemPortShaperInfo shaperInfo;
}BCM_Ploam_ConfigGemPortShaper;
#endif /* 6818 */
#if !defined(CONFIG_BCM96816) || defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_CFG_US_SHAPER */
typedef struct {
    INOUT UINT8  cirEnabled;/* Accepted values 0..1 */
    INOUT UINT8  eirEnabled;/* Accepted values 0..1 */
    INOUT UINT8  beDisabled;/* Accepted values 0..1 */
    INOUT UINT32 cirRateKbps; /* Accepted values BCM_PLOAM_MIN_US_RATE_KBPS..BCM_PLOAM_MAX_US_RATE_KBPS */
    INOUT UINT32 cbs;       /* Committed Burst Size (Bytes) - default=0 */
                            /* Burst Size should always be greater than the max packet size supported */
    INOUT UINT32 eirRateKbps; /* PIR - CIR */ /* Accepted values BCM_PLOAM_MIN_US_RATE_KBPS..BCM_PLOAM_MAX_US_RATE_KBPS */
    INOUT UINT32 ebs;       /* Extended Burst Size (Bytes) - default=0 */
                            /* Burst Size should always be greater than the max packet size supported */
}BCM_Ploam_UsShaperInfo;
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_MAX_GTCUS_QUEUES-1]*/
  INOUT UINT16  usQueueIdx;
  INOUT BCM_Ploam_UsShaperInfo shpInfo;

} BCM_Ploam_UsShaperCfgInfo;

typedef struct {
  INOUT UINT16  portIdx;
  INOUT UBOOL8  enable;
} BCM_ConfigLanPort ;

/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_GET_US_QUEUE_CFG */
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_MAX_GTCUS_QUEUES-1]*/
  IN  UINT16  usQueueIdx;
  OUT UINT8  mapped;
  OUT UINT8  tcontIdx;
  OUT BCM_Ploam_UsQosInfo qosInfo;
  OUT BCM_Ploam_UsShaperInfo shpInfo;

} BCM_Ploam_UsQueueCfgInfo;
#endif /* 6818 */
typedef enum
{
    TCONT_SCHD_POLICY_NONE = 0,
    TCONT_SCHD_POLICY_SP,     /* Only applies to 6838 and 68500 */
    TCONT_SCHD_POLICY_WRR,    /* Only applies to 6838 and 68500 */
    TCONT_SCHD_POLICY_SP_WRR, /* Only applies to 6818 */
    TCONT_SCHD_POLICY_SP_WFQ, /* Only applies to 6818 */
    TCONT_SCHD_POLICY_MAX
} TcontSchdPolicy;

/*Type of ioctl argument pointer for command 
  BCM_PLOAM_IOC_CFG_TCONT_ARBITRATION/BCM_PLOAM_IOC_GET_TCONT_ARBITRATION */
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_TCONTS-1]*/
  IN UINT8 tcontIdx;
  INOUT TcontSchdPolicy schdPolicy; /* SP_WRR or SP_WFQ */
} BCM_Ploam_TcontSchdPolicy;

/*Type of ioctl argument pointer for command 
BCM_PLOAM_IOC_CFG_TCONT_QUEUE_MAP/BCM_PLOAM_IOC_DECFG_TCONT_QUEUE_MAP/BCM_PLOAM_IOC_GET_TCONT_QUEUE_MAP */
typedef struct {
 /*Accepted values: [0..BCM_PLOAM_NUM_DATA_TCONTS-1]*/
  IN UINT8 tcontIdx;
 /*Accepted values: Set corresponding bit position to map the queue */
  INOUT UINT32 queueMap;

} BCM_Ploam_TcontUsQueueMap;


/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_SET_ACK_OFFSET and 
  BCM_PLOAM_IOC_GET_ACK_OFFSET */
typedef struct {
  INOUT int offset; /* Offset in DS-PLOAM from where to start copy in US-PLOAM-ACK */
} BCM_Ploam_AckOffsetInfo;

#if defined(CONFIG_BCM_GPON_AE_AUTO_SWITCH)
/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_DISABLE_GPON_AE_AUTO_SWITCH and 
  BCM_PLOAM_IOC_GET_GPON_AE_AUTO_SWITCH_MODE */
typedef struct {
  OUT UBOOL8 gponAeAutoSwitchEnabled; 
} BCM_Ploam_GponAeAutoSwitchInfo;
#endif /* CONFIG_BCM_GPON_AE_AUTO_SWITCH */

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_US_QUEUE_IDX_COUNTERS*/
typedef struct {
  IN UINT32 reset;
 /*Accepted values: [0..BCM_PLOAM_MAX_GTCUS_QUEUES-1],
  *BCM_PLOAM_US_QUEUE_IDX_ALL */
  INOUT UINT8 usQueueIdx;
  OUT UINT32 txBytes;
  OUT UINT32 txFragments;
  OUT UINT32 txFrames;
  OUT UINT32 txDroppedFrames;
  OUT UINT32 wredDropsInBytes;
} BCM_Ploam_UsQueueIdxCounters;

/*Type of ioctl argument pointer for command BCM_PLOAM_IOC_GET_GEM_MIB_IDX_COUNTERS*/
typedef struct {
  IN UINT32 reset;
 /*Accepted values: [0..BCM_PLOAM_NUM_GEM_MIB_IDXS-1],
  *BCM_PLOAM_GEM_MIB_IDX_ALL */
  INOUT UINT16 gemMibIndex;
  OUT UINT32 rxBytes;
  OUT UINT32 rxFragments;
  OUT UINT32 rxFrames;
  OUT UINT32 rxDroppedFrames;
  OUT UINT32 rxMcastAcceptedFrames;
  OUT UINT32 rxMcastDroppedFrames;
  OUT UINT32 rxFcsErr;

} BCM_Ploam_DsGemMibIdxCounters;

#if defined(CONFIG_BCM968500) || defined(CONFIG_BCM96838)
/*Type of ioctl argument pointer for command ploamIOCsetGemPortEncryptionByIX/ID*/
typedef struct {
  IN UINT16 gemPortId;
  IN UINT16 gemIndex;
  IN UBOOL8 encryption;
} BCM_Ploam_GemPortEncryption;
#endif

#if defined(CONFIG_BCM_GPON_DDRO)
/*Type of ioctl arguement pointer for command BCM_PLOAM_IOC_GET_DDR_OFFLOAD_CFG, BCM_PLOAM_IOC_SET_DDR_OFFLOAD_CFG */
typedef struct {
  INOUT UBOOL8 enable;
  INOUT UINT16 queueSize[BCM_PLOAM_NUM_DDRO_QUEUE];
} BCM_Ploam_GponDdrOffloadInfo;

/* WRED Feature Related Defines */
#define BCM_PLOAM_WRED_DEFAULT_DROP_PROBABILITY       10 /* 10%. As per Jacobson's RED algorithm. Page 11 Section 7 */
#define BCM_PLOAM_WRED_DEFAULT_QWAIT_EXP               9 /* Setting default to 2^(-9) = 0.002 approx. as per Jacobson's RED algorithm. 
                                                            Page 10 Section 6.2*/
#define BCM_PLOAM_DDR_Q_BLOCK_SIZE                    64 /* 64 Byte blocks*/
#define BCM_PLOAM_WRED_DISABLE_DROP_THRESHOLD_VALUE   9999999

/* Profile Oper Flag */
typedef enum {
  BCM_PLOAM_WRED_PROFILE_CREATE=0,
  BCM_PLOAM_WRED_PROFILE_MODIFY,
  BCM_PLOAM_WRED_PROFILE_DELETE,
  BCM_PLOAM_WRED_PROFILE_MAX_OPER_FLAG = BCM_PLOAM_WRED_PROFILE_DELETE
} BCM_Ploam_WRED_OperFlag;

typedef struct {
  IN    BCM_Ploam_WRED_OperFlag  operFlag;
  INOUT UINT8                    profileIdx;
  /* Accepted values: [0..BCM_PLOAM_WRED_NUM_PROFILES-1] */
  IN    UINT8                    maxDropProbInPercent;
  /* Accepted values: [0..100] */
  IN    UINT32                   minDropThresholdInBytes;
  /* Accepted values: [0..BCM_PLOAM_MAX_DDRO_QUEUE_SIZE], BCM_PLOAM_WRED_DISABLE_DROP_THRESHOLD_VALUE */
  /* The rule of thumb for optimal performance as per WRED Algorithm is to have minDropThresholdInKB <= (maxDropThresholdInKB / 2)*/
  IN    UINT32                   maxDropThresholdInBytes;
  /* Accepted values: [0..BCM_PLOAM_MAX_DDRO_QUEUE_SIZE], BCM_PLOAM_WRED_DISABLE_DROP_THRESHOLD_VALUE */
} BCM_Ploam_WREDProfileInfo;

typedef struct {
  OUT   UINT8   isCreated;
  IN    UINT8   profileIdx;
  /* Accepted values: [0..BCM_PLOAM_WRED_NUM_PROFILES-1] */
  OUT   UINT8   maxDropProbInPercent;
  OUT   UINT32  minDropThresholdInBytes;
  OUT   UINT32  maxDropThresholdInBytes;
} BCM_Ploam_WREDGetProfileInfo;

typedef struct {
  OUT UINT16 profileIdxMask;
  IN  UINT8  maxDropProbInPercent;
  IN  UINT32 minDropThresholdInBytes;
  IN  UINT32 maxDropThresholdInBytes;
} BCM_Ploam_WREDMatchingProfiles;

typedef struct {
  IN    UINT8  qIdx;
  /* Accepted values: [0..BCM_PLOAM_WRED_NUM_QUEUES-1] */
  INOUT UINT8  enable;
  /* Accepted values: [0 - disable, 1-enable] */
  INOUT UINT8  qWeightExp;
  /* Accepted values: [0..31] */
  INOUT UINT8  ylwProfileIdx;
  /* Accepted values: [0..BCM_PLOAM_WRED_NUM_PROFILES-1] */
  INOUT UINT8  grnProfileIdx;
  /* Accepted values: [0..BCM_PLOAM_WRED_NUM_PROFILES-1] */
} BCM_Ploam_WREDQInfo;
#endif /* CONFIG_BCM_GPON_DDRO */

/* O1-O7 buffer size in scratch pad */
#define SP_LINK_STATE_BUFFER_SIZE ( 7 )

#endif /*BCM_PLOAM_IOC_API_H*/

