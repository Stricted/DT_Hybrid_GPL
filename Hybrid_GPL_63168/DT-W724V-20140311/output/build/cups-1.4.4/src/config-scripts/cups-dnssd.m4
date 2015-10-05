dnl
dnl "$Id: cups-dnssd.m4 8789 2009-08-28 22:54:34Z mike $"
dnl
dnl   DNS Service Discovery (aka Bonjour) stuff for the Common UNIX Printing System (CUPS).
dnl
dnl   http://www.dns-sd.org
dnl   http://www.multicastdns.org/
dnl   http://developer.apple.com/networking/bonjour/
dnl
dnl   Copyright 2007-2009 by Apple Inc.
dnl
dnl   These coded instructions, statements, and computer programs are the
dnl   property of Apple Inc. and are protected by Federal copyright
dnl   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
dnl   which should have been included with this file.  If this file is
dnl   file is missing or damaged, see the license at "http://www.cups.org/".
dnl

AC_ARG_ENABLE(dnssd, [  --disable-dnssd         disable DNS Service Discovery support])
AC_ARG_WITH(dnssd-libs, [  --with-dnssd-libs       set directory for DNS Service Discovery library],
	LDFLAGS="-L$withval $LDFLAGS"
	DSOFLAGS="-L$withval $DSOFLAGS",)
AC_ARG_WITH(dnssd-includes, [  --with-dnssd-includes   set directory for DNS Service Discovery includes],
	CFLAGS="-I$withval $CFLAGS"
	CPPFLAGS="-I$withval $CPPFLAGS",)

DNSSDLIBS=""
DNSSD_BACKEND=""

if test x$enable_dnssd != xno; then
	AC_CHECK_HEADER(dns_sd.h, [
		case "$uname" in
			Darwin*)
				# Darwin and MacOS X...
				AC_DEFINE(HAVE_DNSSD)
				AC_DEFINE(HAVE_COREFOUNDATION)
				AC_DEFINE(HAVE_SYSTEMCONFIGURATION)
				DNSSDLIBS="-framework CoreFoundation -framework SystemConfiguration"
				DNSSD_BACKEND="dnssd"
				;;
			*)
				# All others...
				AC_MSG_CHECKING(for current version of dns_sd library)
				SAVELIBS="$LIBS"
				LIBS="$LIBS -ldns_sd"
				AC_TRY_COMPILE([#include <dns_sd.h>],
					[int constant = kDNSServiceFlagsShareConnection;
					unsigned char txtRecord[100];
					uint8_t valueLen;
					TXTRecordGetValuePtr(sizeof(txtRecord),
					    txtRecord, "value", &valueLen);],
					AC_MSG_RESULT(yes)
					AC_DEFINE(HAVE_DNSSD)
					DNSSDLIBS="-ldns_sd"
					DNSSD_BACKEND="dnssd",
					AC_MSG_RESULT(no))
				LIBS="$SAVELIBS"
				;;
		esac
	])
fi

AC_SUBST(DNSSDLIBS)
AC_SUBST(DNSSD_BACKEND)

dnl
dnl End of "$Id: cups-dnssd.m4 8789 2009-08-28 22:54:34Z mike $".
dnl
