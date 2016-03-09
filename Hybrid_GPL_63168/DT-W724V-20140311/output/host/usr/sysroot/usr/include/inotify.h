/* libinotifytools/src/inotifytools/inotify.h.  Generated from inotify.h.in by configure.  */
#ifndef _INOTIFY_H
#define _INOTIFY_H 1

/* this is defined to 1 if <sys/inotify.h> works. */
/* #undef SYS_INOTIFY_H_EXISTS_AND_WORKS */

#if defined(SUPPORT_ATP_WEB) && defined(SUPPORT_ATP_WEB_INETD)
#else
#define SYS_INOTIFY_H_EXISTS_AND_WORKS
#endif

#ifdef SYS_INOTIFY_H_EXISTS_AND_WORKS
#include <sys/inotify.h>
#else
#include "inotify-nosys.h"
#endif // SYS_INOTIFY_H_EXISTS_AND_WORKS


#endif

