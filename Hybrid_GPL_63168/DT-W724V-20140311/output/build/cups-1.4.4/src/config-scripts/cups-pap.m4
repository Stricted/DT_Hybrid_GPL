dnl
dnl "$Id: cups-pap.m4 8524 2009-04-19 21:18:45Z mike $"
dnl
dnl   PAP (AppleTalk) stuff for the Common UNIX Printing System (CUPS).
dnl
dnl   Copyright 2007-2009 by Apple Inc.
dnl   Copyright 2006 by Easy Software Products, all rights reserved.
dnl
dnl   These coded instructions, statements, and computer programs are the
dnl   property of Apple Inc. and are protected by Federal copyright
dnl   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
dnl   which should have been included with this file.  If this file is
dnl   file is missing or damaged, see the license at "http://www.cups.org/".
dnl

# Currently the PAP backend is only supported on MacOS X with the AppleTalk
# SDK installed...
AC_ARG_ENABLE(pap, [  --enable-pap            build with AppleTalk support])

PAP=""
AC_SUBST(PAP)

if test x$enable_pap = xyes -a $uname = Darwin; then
	AC_CHECK_HEADER(netat/appletalk.h,[
		PAP="pap"
		AC_CHECK_HEADER(AppleTalk/at_proto.h)])
fi

dnl
dnl End of "$Id: cups-pap.m4 8524 2009-04-19 21:18:45Z mike $".
dnl
