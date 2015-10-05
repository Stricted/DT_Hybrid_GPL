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
 * G997.h 
 *
 * Description:
 *	This file contains the exported functions and definitions for G97Framer
 *
 *
 * Copyright (c) 1993-1998 AltoCom, Inc. All rights reserved.
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.3 $
 *
 * $Id: G997.h,v 1.3 2003/07/18 18:56:59 ilyas Exp $
 *
 * $Log: G997.h,v $
 * Revision 1.3  2003/07/18 18:56:59  ilyas
 * Added support for shared TX buffer (for ADSL driver)
 *
 * Revision 1.2  2002/07/20 00:51:41  ilyas
 * Merged witchanges made for VxWorks/Linux driver.
 *
 * Revision 1.1  2001/12/13 02:28:27  ilyas
 * Added common framer (DslPacket and G997) and G997 module
 *
 *
 *
 *****************************************************************************/

#ifndef	G997FramerHeader
#define	G997FramerHeader

extern Boolean  G997Init(
		void					*gDslVars, 
		bitMap					setup, 
		ulong					rxBufNum,
		ulong					rxBufSize,
		ulong					rxPacketNum,
		upperLayerFunctions		*pUpperLayerFunctions,
		dslCommandHandlerType	g997PhyCommandHandler);

extern void		G997Close(void *gDslVars);
extern void		G997Timer(void *gDslVars, long timeQ24ms);
extern Boolean	G997CommandHandler (void *gDslVars, dslCommandStruct *cmd);
extern void		G997StatusSnooper (void *gDslVars, dslStatusStruct *status);

extern	int		G997SendFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);
extern  int		G997ReturnFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);

extern  Boolean G997SetTxBuffer(void *gDslVars, ulong len, void *bufPtr);
extern	void *	G997GetFramePoolHandler(void *gDslVars);

#endif	/* G997FramerHeader */
