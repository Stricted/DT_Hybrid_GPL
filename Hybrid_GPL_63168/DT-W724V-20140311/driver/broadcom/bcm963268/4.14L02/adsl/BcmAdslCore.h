/* 
* <:copyright-BRCM:2002:proprietary:standard
* 
*    Copyright (c) 2002 Broadcom Corporation
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

/***************************************************************************
****************************************************************************
** File Name  : BcmAdslCore.h
**
** Description: This file contains the definitions, structures and function
**              prototypes for Bcm Core ADSL PHY interface
**
***************************************************************************/
#if !defined(_BcmAdslCore_H)
#define _BcmAdslCore_H

#include "AdslMibDef.h"

/* 
**	Internal ADSL driver events handled by the ADSL driver
**  defined not to intersect with ADSL_LINK_STATE events (in vcmadsl.h)
*/

#define	ADSL_SWITCH_RJ11_PAIR	0x80000001

/***************************************************************************
** Function Prototypes
***************************************************************************/

void BcmAdslCoreInit(void);
void BcmAdslCoreUninit(void);
Bool BcmAdslCoreCheckBoard(void);
void BcmAdslCoreConnectionStart(unsigned char lineId);
void BcmAdslCoreConnectionStop(unsigned char lineId);
void BcmAdslCoreConnectionReset(unsigned char lineId);

void BcmAdslCoreGetConnectionInfo(unsigned char lineId, PADSL_CONNECTION_INFO pConnectionInfo);
void BcmAdslCoreDiagCmd(unsigned char lineId, PADSL_DIAG pAdslDiag);
void BcmAdslCoreDiagCmdAdsl(unsigned char lineId, int diagCmd, int len, void *pCmdData);
void BcmAdslCoreDiagCmdCommon(unsigned char lineId, int diagCmd, int len, void *pCmdData);
void BcmAdslCoreDiagCmdNotify(void);
void BcmAdslCoreDiagSetSyncTime(unsigned long syncTime);
void BcmAdslCoreDiagConnectionCheck(void);
unsigned long BcmAdslCoreDiagGetSyncTime(void);
char * BcmAdslCoreDiagScrambleString(char *s);
int  BcmAdslCoreSetObjectValue(unsigned char lineId, char *objId, int objIdLen, char *dataBuf, long *dataBufLen);
int  BcmAdslCoreGetObjectValue(unsigned char lineId, char *objId, int objIdLen, char *dataBuf, long *dataBufLen);
void BcmAdslCoreStartBERT(unsigned char lineId, unsigned long totalBits);
void BcmAdslCoreStopBERT(unsigned char lineId);
void BcmAdslCoreBertStartEx(unsigned char lineId, unsigned long bertSec);
void BcmAdslCoreBertStopEx(unsigned char lineId);
#ifndef DYING_GASP_API
Bool BcmAdslCoreCheckPowerLoss(void);
#endif
void BcmAdslCoreSendDyingGasp(int powerCtl);
void BcmAdslCoreConfigure(unsigned char lineId, adslCfgProfile *pAdslCfg);
void BcmAdslCoreGetVersion(adslVersionInfo *pAdslVer);
void BcmAdslCoreSetTestMode(unsigned char lineId, int testMode);
void BcmAdslCoreSetTestExecutionDelay(unsigned char lineId, int testMode, unsigned long value);
void BcmAdslCoreSelectTones(
	unsigned char lineId,
	int		xmtStartTone, 
	int		xmtNumTones, 
	int		rcvStartTone,
	int		rcvNumTones, 
	char	*xmtToneMap,
	char	*rcvToneMap);
void BcmAdslCoreDiagSelectTones(
	unsigned char lineId,
	int		xmtStartTone, 
	int		xmtNumTones, 
	int		rcvStartTone,
	int		rcvNumTones, 
	char	*xmtToneMap,
	char	*rcvToneMap);
void BcmAdslCoreSetAdslDiagMode(unsigned char lineId, int diagMode);
int BcmAdslCoreGetConstellationPoints (int toneId, ADSL_CONSTELLATION_POINT *pointBuf, int numPoints);
int BcmAdslCoreGetOemParameter (int paramId, void *buf, int len);
int BcmAdslCoreSetOemParameter (int paramId, void *buf, int len);
int BcmAdslCoreSetXmtGain(unsigned char lineId, int gain);
int  BcmAdslCoreGetSelfTestMode(void);
void BcmAdslCoreSetSelfTestMode(int stMode);
int  BcmAdslCoreGetSelfTestResults(void);
void BcmAdslCoreAfeTestMsg(void *pMsg);
void BcmAdslCoreDebugCmd(unsigned char lineId, void *pMsg);
#ifdef SUPPORT_XDSLDRV_GDB
void GdbStubInit(void);
void setGdbOn(void);
char isGdbOn(void);
void BcmAdslCoreGdbCmd(void *pCmd, int cmdLen);
void BcmAdslCoreGdbTask(void);
#endif
#ifdef __KERNEL__
int BcmXdslCoreDiagProcFileCreate(void);
void BcmXdslCoreDiagProcFileRemove(void);
#if !defined(CONFIG_BCM96358) && !defined(CONFIG_BCM96348) && !defined(CONFIG_BCM96338)
void BcmAdslCoreResetPhy(int copyImage);
#endif
#endif /* __KERNEL__ */
ADSL_LINK_STATE BcmAdslCoreGetEvent (unsigned char lineId);
Bool BcmAdslCoreSetSDRAMBaseAddr(void *pAddr);
Bool BcmAdslCoreSetVcEntry (int gfc, int port, int vpi, int vci, int pti_clp);
Bool BcmAdslCoreSetGfc2VcMapping(Bool bOn);
Bool BcmAdslCoreSetAtmLoopbackMode(void);
void BcmAdslCoreResetStatCounters(unsigned char lineId);

Bool BcmAdslCoreG997SendData(unsigned char lineId, void *buf, int len);

void *BcmAdslCoreG997FrameGet(unsigned char lineId, int *pLen);
void *BcmAdslCoreG997FrameGetNext(unsigned char lineId, int *pLen);
void BcmAdslCoreG997FrameFinished(unsigned char lineId);

void BcmAdslCoreAtmSetPortId(int path, int portId);
void BcmAdslCoreAtmClearVcTable(void);
void BcmAdslCoreAtmAddVc(int vpi, int vci);
void BcmAdslCoreAtmDeleteVc(int vpi, int vci);
void BcmAdslCoreAtmSetMaxSdu(unsigned short maxsdu);

void BcmAdsl_Notify(unsigned char lineId);
void BcmAdsl_ConfigureRj11Pair(long pair);
void BcmAdslCoreDelay(unsigned long timeMs);
#if !defined(CONFIG_BCM96338) && !defined(CONFIG_BCM96348) && !defined(CONFIG_BCM96358)
void BcmXdslCoreSendAfeInfo(int toPhy);
#endif
void BcmXdslNotifyRateChange(unsigned char lineId);
void BcmXdslCoreMiscIoCtlFunc(unsigned char lineId);
void BcmXdslCoreMaintenanceTask(void);

#if !defined(CONFIG_BCM96338) && !defined(CONFIG_BCM96348) && !defined(CONFIG_BCM96358)
int BcmXdslCoreGetAfeBoardId(unsigned long *pAfeIds);
#ifdef CONFIG_VDSL_SUPPORTED
void SetupReferenceClockTo6306(void);
int IsAfe6306ChipUsed(void);
#ifdef CONFIG_BCM96368
void PLLPowerUpSequence6306(void);
#endif
#endif	/* CONFIG_VDSL_SUPPORTED */
#endif	/* !defined(CONFIG_BCM96338) && !defined(CONFIG_BCM96348) && !defined(CONFIG_BCM96358) */
unsigned long BcmAdslCoreGetCycleCount(void);
unsigned long BcmAdslCoreCycleTimeElapsedUs(unsigned long cnt1, unsigned long cnt0);

#define	BcmCoreCommandHandlerSync(cmd)	do {	\
	BcmCoreDpcSyncEnter();						\
	AdslCoreCommandHandler(cmd);				\
	BcmCoreDpcSyncExit();						\
} while (0)

#endif /* _BcmAdslCore_H */

