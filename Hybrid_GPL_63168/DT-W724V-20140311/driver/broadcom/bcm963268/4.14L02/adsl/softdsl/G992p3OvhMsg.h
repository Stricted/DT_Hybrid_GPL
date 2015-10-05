/*
<:copyright-broadcom 
 
 Copyright (c) 2002 Broadcom Corporation 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom Corporation 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom Corporation 
 company private, proprietary, and trade secret. 
 
:>
*/
/****************************************************************************
 *
 * G992p3OvhMsg.h 
 *
 * Description:
 *	This file contains the exported functions and definitions for G992p3 
 *  overhead channel messages
 *
 *
 * Copyright (c) 1999-2003 BroadCom, Inc. All rights reserved.
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.1 $
 *
 * $Id: G992p3OvhMsg.h,v 1.1 2003/07/18 19:39:18 ilyas Exp $
 *
 * $Log: G992p3OvhMsg.h,v $
 * Revision 1.1  2003/07/18 19:39:18  ilyas
 * Initial G.992.3 overhead channel message implementation (from ADSL driver)
 *
 *
 *****************************************************************************/

#ifndef	G992p3OvhMsgFramerHeader
#define	G992p3OvhMsgFramerHeader

#define	kG992p3OvhMsgFrameBufCnt	-1

extern Boolean  G992p3OvhMsgInit(
		void					*gDslVars, 
		bitMap					setup,
		dslFrameHandlerType		rxReturnFramePtr,
		dslFrameHandlerType		txSendFramePtr,
		dslFrameHandlerType		txSendCompletePtr,
		dslCommandHandlerType	commandHandler,
		dslStatusHandlerType	statusHandler);

extern void		G992p3OvhMsgReset(void *gDslVars);
extern void		G992p3OvhMsgClose(void *gDslVars);
extern void		G992p3OvhMsgTimer(void *gDslVars, long timeQ24ms);
extern Boolean	G992p3OvhMsgCommandHandler (void *gDslVars, dslCommandStruct *cmd);
extern void		G992p3OvhMsgStatusSnooper (void *gDslVars, dslStatusStruct *status);

extern	int		G992p3OvhMsgSendCompleteFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);
extern  int		G992p3OvhMsgIndicateRcvFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);
extern  void	G992p3OvhMsgReturnFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);

extern void		G992p3OvhMsgSetL3(void *gDslVars);
extern void		G992p3OvhMsgSetL0(void *gDslVars);

extern Boolean	G992p3OvhMsgSendClearEocFrame(void *gDslVars, dslFrame *pFrame);
extern void G992p3OvhMsgSetRateChangeFlag(void *gDslVars);
#endif	/* G992p3OvhMsgFramerHeader */
