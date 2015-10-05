/* vi: set sw=4 ts=4: */
/*
 * Poweroff reboot and halt, oh my.
 *
 * Copyright 2006 by Rob Landley <rob@landley.net>
 *
 * Licensed under GPL version 2, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include <sys/reboot.h>

#if defined(SUPPORT_BCM96358) || defined(SUPPORT_BCM96368)
typedef struct BhalIoctParms
{
    char *pcString;
    char *pcBuf;
    unsigned int   ulType;
    unsigned int   ulStrLen;
    unsigned int   ulOffset;  
    int   lResult; 
} BHAL_IOCTL_PARMS_ST;

#define BHAL_IOCTL_SOFT_REBOOT  _IOWR('B', 19, BHAL_IOCTL_PARMS_ST)
    
int bhalIoctl(unsigned int ulBhalioctl, unsigned int ulType, unsigned char *pcString, unsigned int ulStrLen, unsigned int ulOffset, unsigned char *pcBuf);
int bhalIoctl(unsigned int ulBhalioctl, unsigned int ulType, unsigned char *pcString, unsigned int ulStrLen, unsigned int ulOffset, unsigned char *pcBuf)
{
    int lRet = -1;
    int lBoardFd = 0;
    BHAL_IOCTL_PARMS_ST stIoctlParms;
    
    lBoardFd = open("/dev/bhal", O_RDWR);
    if (lBoardFd != -1) 
    {
        stIoctlParms.pcString = pcString;
        stIoctlParms.ulStrLen = ulStrLen;
        stIoctlParms.ulOffset = ulOffset;
        stIoctlParms.ulType   = ulType;
        stIoctlParms.pcBuf    = pcBuf;
        
        ioctl(lBoardFd, ulBhalioctl, &stIoctlParms);
        close(lBoardFd);

        lRet = stIoctlParms.lResult;
    } 
    else
    {
        printf("Unable to open device /dev/bhal.\n");
    }

    return lRet;
}   

void BSP_SYS_SoftReboot(void)
{
	/* w69233 2009-06-06 add: avoid bcm6368 chip soft reset failed begin */
	int i;
	char cmd[32];

	for (i = 0; i < 4; i++)  {
		if (i == 0) {
			system("ifconfig wl0 down");
		}
		else {
			sprintf(cmd, "ifconfig %s.%d down", "wl0", i);     
			system(cmd);
		}
	}
	/* w69233 2009-06-06 add: avoid bcm6368 chip soft reset failed end */

/*start by f00184228 2011-8-22 DTS2011082201696 */
#ifdef SUPPORT_HG622U
    system("killall pppc");
/*sleep 一段时间，防止同时使能IPv6、IPv4时不发PADT报文*/
	sleep(2);
#endif
/*end by f00184228 2011-8-22 DTS2011082201696 */
    
	bhalIoctl(BHAL_IOCTL_SOFT_REBOOT, 0, "", 0, 0, "");
}


#endif

int halt_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int halt_main(int argc, char **argv)
{
	static const int magic[] = {
#ifdef RB_HALT_SYSTEM
RB_HALT_SYSTEM,
#elif defined RB_HALT
RB_HALT,
#endif
#ifdef RB_POWER_OFF
RB_POWER_OFF,
#elif defined RB_POWERDOWN
RB_POWERDOWN,
#endif
RB_AUTOBOOT
	};
	static const int signals[] = { SIGUSR1, SIGUSR2, SIGTERM };

	char *delay;
	int which, flags, rc = 1;

	/* Figure out which applet we're running */
	for (which = 0; "hpr"[which] != *applet_name; which++);

	/* Parse and handle arguments */
	flags = getopt32(argv, "d:nf", &delay);
	if (flags & 1) sleep(xatou(delay));
	if (!(flags & 2)) sync();

	/* start of y42304 20080419: 解决96358芯片软复位问题：*/
#if defined(SUPPORT_BCM96358) || defined(SUPPORT_BCM96368)
	BSP_SYS_SoftReboot();
	return 0;
#endif
	/* end of y42304 20080419: 解决96358芯片软复位问题：*/

	/* Perform action. */
	if (ENABLE_INIT && !(flags & 4)) {
		if (ENABLE_FEATURE_INITRD) {
			pid_t *pidlist = find_pid_by_name("linuxrc");
			if (pidlist[0] > 0)
				rc = kill(pidlist[0], signals[which]);
			if (ENABLE_FEATURE_CLEAN_UP)
				free(pidlist);
		}
		if (rc)
			rc = kill(1, signals[which]);
	} 
	else
	{   
		rc = reboot(magic[which]);
	}

	if (rc)
		bb_error_msg("no");
	return rc;
}
