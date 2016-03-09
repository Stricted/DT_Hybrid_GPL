#ifndef __ATPNET_H_
#define __ATPNET_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LINKSTATE_DOWN      0
#define LINKSTATE_UP        1

/*
 * Ioctl definitions.
 */
/* reserved SIOCDEVPRIVATE */

#if defined(BCM96362) || defined(BCM963268) || defined(BCM96318)
enum {
    SIOCGLINKSTATE = SIOCDEVPRIVATE + 1,
    SIOCSCLEARMIBCNTR,
    SIOCMIBINFO,
    SIOCGENABLEVLAN,
    SIOCGDISABLEVLAN,
    SIOCGQUERYNUMVLANPORTS,
    SIOCGQUERYNUMPORTS,
    SIOCPORTMIRROR,
    SIOCSWANPORT,
    SIOCGWANPORT,
    SIOCETHCTLOPS,
    SIOCGPONIF,
    SIOCETHSWCTLOPS,
    SIOCGSWITCHPORT,
    SIOCLAST,
};
#else
enum {
    SIOCGLINKSTATE = SIOCDEVPRIVATE + 1,
    SIOCREADWRITEREG,
    SIOCSCLEARMIBCNTR,
    SIOCGIFTRANSSTART,
    SIOCMIBINFO,
    SIOCSDUPLEX,	/* 0: auto 1: full 2: half */
    #if 0
    SIOCSSPEED,		/* 0: auto 1: 100mbps 2: 10mbps */
    #endif
    SIOCSETMODE,
    SIOCCIFSTATS,
    SIOCGENABLEVLAN,
    SIOCGDISABLEVLAN,
    SIOCGQUERYNUMVLANPORTS,
    SIOCGSWITCHTYPE,
    SIOCGQUERYNUMPORTS,
    #if 0
    SIOCGMACTOPORT,
    #endif 
    SIOCREATWAN,
    SIOCGLINKSTATUS,
    SIOCGLSWLINKSTATE,
    SIOCLAST
};
#endif

#define SPEED_10MBIT        10000000
#define SPEED_100MBIT       100000000

typedef struct IoctlMibInfo
{
    unsigned long ulIfLastChange;
    unsigned long ulIfSpeed;
} IOCTL_MIB_INFO, *PIOCTL_MIB_INFO;


#ifdef __cplusplus
}
#endif

#endif 
