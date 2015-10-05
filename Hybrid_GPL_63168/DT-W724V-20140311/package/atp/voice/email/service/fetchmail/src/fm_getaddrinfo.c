#include "config.h"
#include "fetchmail.h"
#include "i18n.h"

#include <signal.h>
#include <errno.h>
#include <string.h>

/** This is a getaddrinfo() replacement that blocks SIGALRM,
 * to avoid issues with non-reentrant getaddrinfo() implementations
 * after SIGALRM timeouts, for instance on MacOS X or NetBSD. */
int fm_getaddrinfo(const char *node, const char *serv, const struct addrinfo *hints, struct addrinfo **res)
{
    int rc;
	struct sockaddr_in *tmp;

#ifndef GETADDRINFO_ASYNCSAFE
    sigset_t ss, os;

    sigemptyset(&ss);
    sigaddset(&ss, SIGALRM);

    if (sigprocmask(SIG_BLOCK, &ss, &os))
	report(stderr, GT_("Cannot modify signal mask: %s"), strerror(errno));
#endif

    rc = getaddrinfo(node, NULL, hints, res);//NULL
    if ((0 == rc) && (NULL != serv))
	{
		tmp = ((struct sockaddr_in *)((*res)->ai_addr));
	    if (0 == strcasecmp(serv, "POP3s"))
		{
			tmp->sin_port = ntohs(995);
		}
		if (0 == strcasecmp(serv, "POP3"))
		{
		    tmp->sin_port = ntohs(110);
		}
		if (0 == strcasecmp(serv, "IMAP"))
		{
			tmp->sin_port = ntohs(143);
		}
		if (0 == strcasecmp(serv, "IMAPs"))
		{
		   tmp->sin_port = ntohs(993);
		}
	}
#ifndef GETADDRINFO_ASYNCSAFE
    if (sigprocmask(SIG_SETMASK, &os, NULL))
	report(stderr, GT_("Cannot modify signal mask: %s"), strerror(errno));
#endif

    return rc;
}

/** this is a debugging freeaddrinfo() wrapper. */
void fm_freeaddrinfo(struct addrinfo *ai)
{
    freeaddrinfo(ai);
}
