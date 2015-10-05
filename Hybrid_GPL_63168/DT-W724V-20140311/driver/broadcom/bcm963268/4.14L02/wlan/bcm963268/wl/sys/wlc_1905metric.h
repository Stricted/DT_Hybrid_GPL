/* 
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

#ifndef _WLC_1905METRIC_H_
#define   _WLC_1905METRIC_H_
#define STA_BW_UPDATE_INTERVAL (5000)
#include <linux/spinlock.h>
#define WLM_LOCK(lock)          spin_lock_bh((spinlock_t *)(lock))
#define WLM_UNLOCK(lock)        spin_unlock_bh((spinlock_t *)(lock))

/*  For ioctls that take a list of MAC addresses */
typedef struct {
	uint interval;                 /*   interval of update */
	uint count;			/*  number of MAC addresses */
	unsigned char ea[1];	/*  variable length array of MAC addresses */
} WLM_1905_METRIC_MACLIST;

typedef struct {
	unsigned char ea[6];		/*  Station address */
	uint32			tx_pkts;	/*  # of packets transmitted */
	uint32			tx_failures;	/*  # of packets failed */
	uint32			rx_ucast_pkts;	/*  # of unicast packets received */
	uint32			rx_mcast_pkts;	/*  # of multicast packets received */
	uint32			tx_rate;	/*  Rate of last successful tx frame */
	uint32			rx_rate;	/*  Rate of last successful rx frame */
	uint32			rx_decrypt_succeeds;	/*  # of packet decrypted successfully */
	uint32			rx_decrypt_failures;	/*  # of packet decrypted unsuccessfully */
	uint32			linkavailability;
	uint32			macthroughput;
	uint32 			tx_used_rates; /*  current traffic rate for this scb */
	uint32 			tx_used_total; /*  current total traffic for this interface */
	uint32 			tx_bytes; /*  tx bytes*/
	uint32 			rx_bytes; /*  rx bytes*/
	int32                   tx_available_bandwidth; /*tx availabel bandwidth */
	int8 			knoise;
} WLM_1905_LINK_INFO;

typedef struct {
	uint32 count;
	WLM_1905_LINK_INFO linkinfo[1];
} WLM_1905_LINK_INFOS;

typedef enum {
	WLMETRIC_REPORT_DATA=200,
} WLM_1905__REPORT_CMD;  

/*  this data structure should be the same as wps_1905.h's WPS_1905_MESSAGE */
typedef struct {
	long type;
	WLM_1905__REPORT_CMD cmd;
	int index;
	int len;
	int status;
} WLM_1905_MESSAGE;

typedef struct
{
    unsigned char mac[6];
    struct list_head list;
} WLM_1905_MACLIST;


typedef enum wlm_1905_wllink_type
{
    WLM_1905_WLLINK_STA,
    WLM_1905_WLLINK_WDS,
    WLM_1905_WLLINK_PSTA
} WLM_1905_WLLINK_TYPE;

typedef struct wlm_1905_wllink {
 	unsigned char mac[6];
	unsigned char type;
} WLM_1905_WLLINK;

typedef struct wlm_1905_wllinks {
 	unsigned int count;
	WLM_1905_WLLINK links[1];
} WLM_1905_WLLINKS;

void *wlm_1905_initmetrictimer(void *wlc,unsigned int interval) ;
void wlm_1905_freetimers(void *wlcc);
void wlm_1905_start_timer(void *wl);
void wlm_1905_stop_timer(void *wl);
int wlm_1905_regmac(void *wlcc, uint8 *arg);
int wlm_1905_removemacs(void *wlcc,uint8 *arg) ;
int wlm_1905_getmetrics(void *wl,void *bscfg,uint8 * arg);
int wlm_1905_getlinklist(void *wl, void *bscfg, uint8 *buf, int len);
int wlm_1905_regsocket(void *wl,uint8 *arg,int len);
#endif
