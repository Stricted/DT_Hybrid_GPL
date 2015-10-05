/*
 * "$Id: phpcups.h 6649 2007-07-11 21:46:42Z mike $"
 *
 *   PHP module include file for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 2007 by Apple Inc.
 *   Copyright 1997-2006 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 */

#ifndef PHPCUPS_H
#  define PHPCUPS_H

/*
 * Include necessary headers...
 */

#  include <cups/cups.h>
#  include <cups/language.h>
#  include <cups/debug.h>
#  include <stdlib.h>
#  include <ctype.h>
#  include <errno.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#  if defined(WIN32) || defined(__EMX__)
#    include <io.h>
#  else
#    include <unistd.h>
#  endif /* WIN32 || __EMX__ */


/*
 * Zend definitions...
 */

extern zend_module_entry	phpcups_module_entry;
#  define phpext_phpcups_ptr	&phpcups_module_entry

#  ifdef PHP_WIN32
#    define PHP_PHPCUPS_API	__declspec(dllexport)
#  else
#    define PHP_PHPCUPS_API
#  endif

#  ifdef ZTS
#    include "TSRM.h"
#  endif

PHP_MINIT_FUNCTION(phpcups);

PHP_FUNCTION(cups_cancel_job);
PHP_FUNCTION(cups_get_dests);
PHP_FUNCTION(cups_get_jobs);
PHP_FUNCTION(cups_last_error);
PHP_FUNCTION(cups_last_error_string);
PHP_FUNCTION(cups_print_file);
PHP_FUNCTION(cups_print_files);

#endif	/* !PHPCUPS_H */


/*
 * End of "$Id: phpcups.h 6649 2007-07-11 21:46:42Z mike $".
 */
