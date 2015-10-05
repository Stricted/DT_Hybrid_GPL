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
 * CircBuf -- Generic Circular Buffer
 *
 * Description:
 *	Implementation of generic circular buffer algorithms
 *
 *
 * Copyright (c) 1993-1998 AltoCom, Inc. All rights reserved.
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.14 $
 *
 * $Id: CircBuf.h,v 1.14 2004/06/24 03:10:37 ilyas Exp $
 *
 * $Log: CircBuf.h,v $
 * Revision 1.14  2004/06/24 03:10:37  ilyas
 * Added extra macro to be able to use un-cached variable (for status write)
 *
 * Revision 1.13  2004/02/09 23:47:02  ilyas
 * Fixed last change
 *
 * Revision 1.12  2004/02/06 22:52:58  ilyas
 * Improved stretch buffer write
 *
 * Revision 1.11  2002/12/30 23:27:55  ilyas
 * Added macro for HostDma optimizations
 *
 * Revision 1.10  2002/10/26 02:15:02  ilyas
 * Optimized and added new macros for HostDma
 *
 * Revision 1.9  2002/01/22 23:59:29  ilyas
 * Added paraenthesis around macro argument
 *
 * Revision 1.8  2002/01/15 22:28:38  ilyas
 * Extended macro to support readPtr from uncached address
 *
 * Revision 1.7  2001/09/21 19:47:05  ilyas
 * Fixed compiler warnings for VxWorks build
 *
 * Revision 1.6  2001/06/07 18:47:56  ilyas
 * Added more macros for circular buffer arithmetics
 *
 * Revision 1.5  2001/04/18 03:58:34  ilyas
 * Added LOG file write granularity
 *
 * Revision 1.4  2001/01/19 04:34:12  ilyas
 * Added more macros to circular buffer implementation
 *
 * Revision 1.3  2001/01/06 04:01:41  ilyas
 * Changed the way we write status messages
 *
 * Revision 1.2  2001/01/04 05:52:21  ilyas
 * Added implementation of stretchable circular buffer used in LOG and Status
 * handlers
 *
 * Revision 1.1  2000/05/03 03:45:55  ilyas
 * Original implementation
 *
 *
 *****************************************************************************/

#ifndef	CircBufHeader_H_
#define	CircBufHeader_H_


typedef struct {
	char	*pStart;
	char	*pEnd;
	char	*pRead;
	char	*pWrite;
} circBufferStruct;

/* Initialize circular buffer */

#define	CircBufferInit(pCB,buf,size)	do {			\
	(pCB)->pStart = (char *) (buf);						\
	(pCB)->pRead = (pCB)->pWrite = (pCB)->pStart;		\
	(pCB)->pEnd = (pCB)->pStart + size;					\
} while (0)

#define	CircBufferGetSize(pCB)			((pCB)->pEnd - (pCB)->pStart)
#define	CircBufferGetStartPtr(pCB)		((void *) (pCB)->pStart)
#define	CircBufferGetEndPtr(pCB)		((void *) (pCB)->pEnd)

#define	CircBufferReset(pCB)			(pCB)->pRead = (pCB)->pWrite = (pCB)->pStart


#define	CircBufferGetReadPtr(pCB)		((void *) (pCB)->pRead)
#define	CircBufferGetWritePtr(pCB)		((void *) (pCB)->pWrite)


#ifndef bcm47xx
#define	CircBufferDistance(pCB,p1,p2,d)	((char*)(p2) - (char*)(p1) - d >= 0 ?			\
											(char*)(p2) - (char*)(p1) - d :			\
											((char*)(p2)- (char*)(p1) - d + ((pCB)->pEnd - (pCB)->pStart)))

#define	CircBufferAddContig(pCB,p,n)	((char*)(p) + (n) == (pCB)->pEnd ? (pCB)->pStart : (char*)(p) + (n))
#else
static __inline int CircBufferDistance(circBufferStruct *pCB, char *p1, char *p2, int d)
{
	int tmp = p2 - p1 - d;

	return (tmp >= 0 ? tmp : tmp + (pCB->pEnd - pCB->pStart));
}

static __inline char * CircBufferAddContig(circBufferStruct *pCB, char *p, int n)
{
	p += n;
	return (p == pCB->pEnd ? pCB->pStart : p);
}
#endif

#define	CircBufferAdd(pCB,p,n)			((char*)(p) + (n) >= (pCB)->pEnd ?						\
											(pCB)->pStart + ((char*)(p) + (n) - (pCB)->pEnd) :	\
											(char*)(p) + (n))

#define	CircBufferReadUpdate(pCB,n)		(pCB)->pRead = CircBufferAdd(pCB,(pCB)->pRead,n)
#define	CircBufferWriteUpdate(pCB,n)	(pCB)->pWrite= CircBufferAdd(pCB,(pCB)->pWrite,n)

#define	CircBufferReadUpdateContig(pCB,n)	(pCB)->pRead = CircBufferAddContig(pCB,(pCB)->pRead,n)
#define	CircBufferWriteUpdateContig(pCB,n)	(pCB)->pWrite= CircBufferAddContig(pCB,(pCB)->pWrite,n)

#define	CircBufferGetReadAvail(pCB)		CircBufferDistance(pCB,(pCB)->pRead,(pCB)->pWrite,0)
#define	CircBufferIsReadEmpty(pCB)		((pCB)->pRead == (pCB)->pWrite)
#define	CircBufferGetWriteAvail(pCB)	CircBufferDistance(pCB,(pCB)->pWrite,(pCB)->pRead,1)
#define	CircBufferGetWriteAvailN(pCB,n)	CircBufferDistance(pCB,(pCB)->pWrite,(pCB)->pRead,n)

#define	CircBufferGetReadContig(pCB)	((unsigned long)(pCB)->pWrite >= (unsigned long) (pCB)->pRead ?	\
											(pCB)->pWrite - (pCB)->pRead :		\
											(pCB)->pEnd	  - (pCB)->pRead)

#define	CircBufferGetWriteContig(pCB)	((pCB)->pEnd - (pCB)->pWrite > CircBufferGetWriteAvail(pCB) ?	\
											CircBufferGetWriteAvail(pCB) :		\
											(pCB)->pEnd - (pCB)->pWrite)

/*
**
**		structure and macros for "strectch" buffer
**
*/

typedef struct {
	char	*pStart;
	char	*pEnd;
	char	*pExtraEnd;
	char	*pStretchEnd;
	char	*pRead;
	char	*pWrite;
} stretchBufferStruct;

#define	StretchBufferInit(pSB,buf,size,extra)	do {	\
	(pSB)->pStart = (char *) (buf);						\
	(pSB)->pRead = (pSB)->pWrite = (pSB)->pStart;		\
	(pSB)->pEnd = (pSB)->pStart + (size);				\
	(pSB)->pStretchEnd = (pSB)->pEnd;					\
	(pSB)->pExtraEnd = (pSB)->pEnd+(extra);				\
} while (0)

#define	StretchBufferGetSize(pSB)		((pSB)->pEnd - (pSB)->pStart)
#define	StretchBufferGetStartPtr(pSB)	((void *) (pSB)->pStart)
#define	StretchBufferGetReadPtr(pSB)	((void *) (pSB)->pRead)
#define	StretchBufferGetWritePtr(pSB)	((void *) (pSB)->pWrite)
#define	StretchBufferReset(pSB)			((pSB)->pRead = (pSB)->pWrite = (pSB)->pStart)

#define	StretchBufferGetReadToEnd(pSB)	((pSB)->pStretchEnd - (pSB)->pRead)

static __inline int _StretchBufferGetReadAvailTotal(stretchBufferStruct *fBuf, char *rdPtr)
{
	char *wrPtr = fBuf->pWrite;
	return( ((wrPtr - rdPtr) >= 0) ? wrPtr - rdPtr: ((fBuf->pEnd - rdPtr) + (wrPtr -fBuf->pStart)));
}
#define	StretchBufferGetReadAvailTotal(pSB)		_StretchBufferGetReadAvailTotal(pSB, (pSB)->pRead)

static __inline int StretchBufferGetReadAvail(stretchBufferStruct *fBuf)
{
	char *wrPtr = fBuf->pWrite;
	char *rdPtr = fBuf->pRead;
	return( ((wrPtr - rdPtr) >= 0) ? wrPtr - rdPtr: fBuf->pStretchEnd - rdPtr );
}
static __inline int _StretchBufferGetWriteAvail(stretchBufferStruct *fBuf, char *rdPtr)
{
	char *wrPtr = fBuf->pWrite;
	return( ((rdPtr - wrPtr) > 0) ?  rdPtr - wrPtr - 1: fBuf->pExtraEnd - wrPtr );
}
#define	StretchBufferGetWriteAvail(pSB)		_StretchBufferGetWriteAvail(pSB, (pSB)->pRead)

#define	StretchBufferReadUpdate(pSB,n)		do {								\
	char	*p;																	\
																				\
	p = (pSB)->pRead + (n);														\
	(pSB)->pRead = (p >= (pSB)->pEnd ? (pSB)->pStart : p);						\
} while (0)

#define	_StretchBufferWriteUpdate(pSB,rd,n)	do {								\
	char	*p;																	\
																				\
	p = (pSB)->pWrite + (n);													\
	if (p >= (pSB)->pEnd) {														\
	  if ((rd) != (pSB)->pStart) {												\
		(pSB)->pStretchEnd = p;													\
		(pSB)->pWrite = (pSB)->pStart;											\
	  }																			\
	}																			\
	else																		\
		(pSB)->pWrite = p;														\
} while (0)

#define	StretchBufferWriteUpdate(pSB,n)		_StretchBufferWriteUpdate(pSB,(pSB)->pRead,n)	

#endif	/* CircBufHeader_H_ */



