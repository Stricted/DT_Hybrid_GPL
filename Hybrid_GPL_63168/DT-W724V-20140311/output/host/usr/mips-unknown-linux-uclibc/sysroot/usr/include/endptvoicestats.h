/***************************************************************************
 * Broadcom Corp. Confidential
 * Copyright 2001 Broadcom Corp. All Rights Reserved.
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED
 * SOFTWARE LICENSE AGREEMENT BETWEEN THE USER AND BROADCOM.
 * YOU HAVE NO RIGHT TO USE OR EXPLOIT THIS MATERIAL EXCEPT
 * SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 ***************************************************************************
 * File Name  : endptvoicestats.h
 *
 * Description: This file contains the definitions of the voice statistics
 *              structures. This file should be used in user space when
 *              working with the voice statistics structure.
 *
 *              The files hpnet.h and hppve.h
 *              should not be included in user space because of a conflict
 *              in the definitions supplied by hausenv.h (in the endpoint/vocm/
 *              ldxIntf/dspinc directory) and the definitions supplied by bcmtypes.h
 *              (in the /sgibcm_2_4_17/linux/include/asm-mips/bcm96345 directory).
 *
 * Updates    :
 ***************************************************************************/

#if !defined(ENDPTSTATS_H)
#define ENDPTSTATS_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
#ifdef ENDPT_6345
   UINT16    peakHoldingTime;    /* Peak Holding Time since last statistic */

   /* Total number of superpackets arriving on the egress path */
   UINT16    packetCount;        /* # of packets received */

   /* addTail and reorder are successful additions to the JB */
   /* addTail is typically 1..N times the packetCount if superpackets are enabled */
   UINT16    addTailCount;       /* # of frames added to tail of JB - normal case */
   UINT16    reorderCount;       /* # of frames reordered */

   /* overrun, duplicate, outOfRange, tooLate, jitterMax excess are packets that have been discarded */
   UINT16    overrunCount;       /* Decoder overrun count */
   UINT16    duplicateCount;     /* # of duplicate frames deleted */
   UINT16    outOfRangeCount;    /* # of frames with timestamps too far from current TS to be handled */
   UINT16    tooLateCount;       /* Packet arrived too late (it's redundant mate already played) */

   /* cantDecode are packets that can't be played out due to algorithm not available */
   UINT16    cantDecodeCount;    /* Can't decode packet - decoder not in load or pkt hdr bad */

   /* The following are internal to the AJC module - they do not represent physical packets */
   UINT16    ajcUnderrunCount;   /* Adaptive Jitter Control: jitter buffer underruns */
   UINT16    ajcDeleteCount;     /* Adaptive Jitter Control: # of packet deletes done to reduce holding time */
   UINT16    ajcRepeatCount;     /* Adaptive Jitter Control: # of packet repeats done to either increase holding time
                                    or due to late frame or lost frames. */
   UINT16    ajcResyncCount;     /* Number of times ajb resynced (went through buildout state) */
   UINT16    ajcPhaseJitterCount;     /* Number of times ajb inserted a phase discontinuity
                                    (possibly in silence or during CNG or due to a repeat/delete). */
   /* 14 entries.  Make sure this is even (!) */
#else
  UINT16    peakHoldingTime;         /* Peak Holding Time since last statistic */
  UINT16    minimumHoldingTime;      /* Long-term tracker of min. Hold Time */
  UINT16    targetHoldingTime;       /* Target holding Time */
  UINT16    inactiveFrameCount;      /* Inactive Frame Count */
  UINT16    activeFrameCount;        /* Active Frame Count */
  UINT16    hsxDecoderOverrunCount;  /* Decoder overrun count */
  UINT16    hsxDecoderUnderrunCount; /* Decoder underrun count */
  UINT16    lostFrameCount;          /* Lost frames resulting in frame repeat */
#endif
} ENDPT_VOICE_DECODERSTATS;

/* Encoder Statistics */
typedef struct
{
#ifdef ENDPT_6345
   UINT16   inactiveFrameCount;     /* Inactive Frame Count of Suppressed Frames*/
   UINT16   activeFrameCount;       /* Active Frame Count - actually sent ingress */
   UINT16   sidFrameCount;          /* SID Frame Count */
   UINT16   toneRelayCount;         /* # of tone packets from PTE relayed ingress */
#else
  UINT16    hsxEncoderOverrunCount;  /* Encoder overrun count */
#endif
} ENDPT_VOICE_ENCODERSTATS;


typedef struct
{
   ENDPT_VOICE_DECODERSTATS   decoder;
   ENDPT_VOICE_ENCODERSTATS   encoder;
} ENDPT_VOICE_PVE_STATS;

typedef struct
{
   ENDPT_VOICE_PVE_STATS hsxPVEstats;         /* statistics for PVE encoder and decoder */

} ENDPT_VOICE_STATS;

#if defined(__cplusplus)
}
#endif

#endif /* ENDPTSTATS_H */
