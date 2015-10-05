//
// "$Id: ppdc-private.h 8698 2009-06-05 20:49:06Z mike $"
//
//   Private definitions for the CUPS PPD Compiler.
//
//   Copyright 2009 by Apple Inc.
//
//   These coded instructions, statements, and computer programs are the
//   property of Apple Inc. and are protected by Federal copyright
//   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
//   which should have been included with this file.  If this file is
//   file is missing or damaged, see the license at "http://www.cups.org/".
//

#ifndef _PPDC_PRIVATE_H_
#  define _PPDC_PRIVATE_H_

//
// Include necessary headers...
//

#  include "ppdc.h"
#  include <cups/debug.h>
#  include <cups/globals.h>
#  include <errno.h>


//
// Macros...
//

#  define PPDC_NEW		DEBUG_printf(("%s: %p new", class_name(), this))
#  define PPDC_NEWVAL(s)	DEBUG_printf(("%s(\"%s\"): %p new", class_name(), s, this))
#  define PPDC_DELETE		DEBUG_printf(("%s: %p delete", class_name(), this))
#  define PPDC_DELETEVAL(s)	DEBUG_printf(("%s(\"%s\"): %p delete", class_name(), s, this))


#endif // !_PPDC_PRIVATE_H_

//
// End of "$Id: ppdc-private.h 8698 2009-06-05 20:49:06Z mike $".
//
