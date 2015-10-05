/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BEGIN: Added by weiyue w00182511, 2011/7/23   PN:TR143功能需求开发*/
#include <assert.h>    
/* END:   Added by weiyue w00182511, 2011/7/23 */
#include "libbb.h"

/* Like strncpy but make sure the resulting string is always 0 terminated. */
char *safe_strncpy(char *dst, const char *src, size_t size)
{
	if (!size) return dst;
	dst[--size] = '\0';
	return strncpy(dst, src, size);
}

/* BEGIN: Added by weiyue w00182511, 2011/7/23   PN:TR143功能需求开发*/
int safe_strtoul(char *arg, unsigned long* value)
{
	char *endptr;
	int errno_save = errno;

	assert(arg!=NULL);
	errno = 0;
	*value = strtoul(arg, &endptr, 0);
	if (errno != 0 || *endptr!='\0' || endptr==arg)
       {
		return 1;
	}
	errno = errno_save;
	return 0;
}
/* END:   Added by weiyue w00182511, 2011/7/23 */
