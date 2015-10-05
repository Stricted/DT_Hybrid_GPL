/*
*  Copyright 2011, Broadcom Corporation
*
* <:copyright-BRCM:2011:proprietary:standard
* 
*    Copyright (c) 2011 Broadcom Corporation
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


#ifndef _BCM_EPON_H_
#define _BCM_EPON_H_


/*************************************
 ******       User API           *****
 *************************************/

// Device name
#define BCM_EPON_DEVICE_NAME  "bcmepon"
#define BCM_EPON_DEVICE_PATH  "/dev/" BCM_EPON_DEVICE_NAME

 
#ifndef PACK
#define PACK __attribute__((__packed__))
#endif

/* IOCTL commands */
typedef enum {
    BCM_EPON_BOOT_EPON_MAC=0,
    BCM_EPON_DUMP_DLU_RULES,
    BCM_EPON_DUMP_GLU_RULES,
    BCM_EPON_DUMP_DLU_RULE,
    BCM_EPON_DUMP_GLU_RULE,
    BCM_EPON_ADD_DLU_RULE,
    BCM_EPON_ADD_GLU_RULE,
    BCM_EPON_ADD_VLAN_TRANSLATION_RULE,
    BCM_EPON_ADD_VLAN_AGGREGATION_RULE,
    BCM_EPON_SET_VLAN_MODE,
    BCM_EPON_DUMP_PORT_RULES,
    BCM_EPON_DELETE_PORT_RULES,
    BCM_EPON_MAC_BOOT_DONE,
    BCM_EPON_HORNET_CLI_OUTPUT_ECHO,
    BCM_EPON_CLI_SEND_HORNET_CMD,
    BCM_EPON_IOC_DEBUG,
    BCM_EPON_DS_DLF_FORWARD,
    BCM_EPON_INFO_CMD,
    BCM_EPON_SET_PID,
    BCM_EPON_IOC_MAX
} bcmEpon_ioctlCmd_t;

typedef struct
{
    void *pvImageSrc;
	unsigned int uiImageSize;
} PACK bcmEpon_BootImage_t;

typedef enum
{
    EponDlu =0,
    EponGlu
} TkOnuLueSel;

typedef struct
{
    unsigned int u32[3];
} PACK Tk3701LueRule;

typedef struct {
    unsigned char id;
    unsigned char lue_sel;
    unsigned short filterVid;
    unsigned short targetVid;
} PACK bcmEpon_Translate_t;

typedef struct {
    unsigned char id;
    unsigned char lue_sel;
    unsigned short filterVid;
    unsigned short targetVid;
    unsigned char  mac[6];
} PACK bcmEpon_Aggregate_t;

typedef struct {
    unsigned char id;
    Tk3701LueRule data;
} PACK bcmEpon_Rule_t;

typedef enum
{
	CtcVlanTransparent		= 0x00,
	CtcVlanTag				= 0x01,
	CtcVlanTranslation		= 0x02,
	CtcVlanN21Translation    = 0x03,
	CtcVlanTrunk             = 0x04,
	CtcNumVlans
} PACK TkOnuVlanMode;

typedef struct {
    unsigned char port;
    TkOnuVlanMode mode;
    unsigned short default_vid;
} PACK bcmEpon_VlanMode_t;


// Structure for a Hornet CLI command (ASCII text)
typedef struct
{
    char *pcCmdText;
	unsigned int uiCmdTextSz;
} PACK bcmEpon_HornetCliCmd_t;

typedef struct {
    union {
		bcmEpon_BootImage_t bootImage;
        bcmEpon_Rule_t rule;
        bcmEpon_Translate_t translate;
        bcmEpon_Aggregate_t aggregate;
        bcmEpon_VlanMode_t vlan;
        bcmEpon_HornetCliCmd_t hornetCliCmd;
        int logLevel; 
        unsigned int infoType;
    }parm;
} PACK bcmEpon_Param_t;


#endif /* _BCM_EPON_H_ */

