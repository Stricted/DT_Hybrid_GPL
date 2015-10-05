dnl
dnl   "$Id: cups-gssapi.m4 8659 2009-05-18 22:03:04Z mike $"
dnl
dnl   GSSAPI/Kerberos library detection.
dnl
dnl   Copyright 2007-2009 by Apple Inc.
dnl   Copyright 2006-2007 by Easy Software Products.
dnl
dnl   This file contains Kerberos support code, copyright 2006 by
dnl   Jelmer Vernooij.
dnl
dnl   These coded instructions, statements, and computer programs are the
dnl   property of Apple Inc. and are protected by Federal copyright
dnl   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
dnl   which should have been included with this file.  If this file is
dnl   file is missing or damaged, see the license at "http://www.cups.org/".
dnl

AC_ARG_ENABLE(gssapi, [  --disable-gssapi        disable GSSAPI support])

LIBGSSAPI=""
AC_SUBST(LIBGSSAPI)

if test x$enable_gssapi != xno; then
	AC_PATH_PROG(KRB5CONFIG, krb5-config)
	if test "x$KRB5CONFIG" != x; then
		case "$uname" in
			Darwin)
				# Mac OS X weak-links to the Kerberos framework...
				LIBGSSAPI="-weak_framework Kerberos"
				;;
			SunOS*)
				# Solaris has a non-standard krb5-config, don't use it!
				AC_CHECK_LIB(gss, gss_display_status,
					AC_DEFINE(HAVE_GSSAPI, 1, [Whether GSSAPI is available])
					CFLAGS="`$KRB5CONFIG --cflags` $CFLAGS"		
					CPPFLAGS="`$KRB5CONFIG --cflags` $CPPFLAGS"		
					LIBGSSAPI="-lgss `$KRB5CONFIG --libs`")
				;;
			*)
				# Other platforms just ask for GSSAPI
				CFLAGS="`$KRB5CONFIG --cflags gssapi` $CFLAGS"		
				CPPFLAGS="`$KRB5CONFIG --cflags gssapi` $CPPFLAGS"		
				LIBGSSAPI="`$KRB5CONFIG --libs gssapi`"
				;;
		esac
		AC_DEFINE(HAVE_GSSAPI, 1, [Whether GSSAPI is available])
	else
		# Check for vendor-specific implementations...
		case "$uname" in
			HP-UX*)
				AC_CHECK_LIB(gss, gss_display_status,
					AC_DEFINE(HAVE_GSSAPI, 1, [Whether GSSAPI is available])
					LIBGSSAPI="-lgss -lgssapi_krb5")
				;;
			SunOS*)
				AC_CHECK_LIB(gss, gss_display_status,
					AC_DEFINE(HAVE_GSSAPI, 1, [Whether GSSAPI is available])
					LIBGSSAPI="-lgss")
				;;
		esac
	fi

	if test "x$LIBGSSAPI" != x; then
		AC_CHECK_HEADER(krb5.h, AC_DEFINE(HAVE_KRB5_H))
		AC_CHECK_HEADER(gssapi.h, AC_DEFINE(HAVE_GSSAPI_H))
		AC_CHECK_HEADER(gssapi/gssapi.h, AC_DEFINE(HAVE_GSSAPI_GSSAPI_H))
		AC_CHECK_HEADER(gssapi/gssapi_generic.h, AC_DEFINE(HAVE_GSSAPI_GSSAPI_GENERIC_H))
		AC_CHECK_HEADER(gssapi/gssapi_krb5.h, AC_DEFINE(HAVE_GSSAPI_GSSAPI_KRB5_H))

		SAVELIBS="$LIBS"
		LIBS="$LIBS $LIBGSSAPI"

		AC_CHECK_FUNC(gsskrb5_register_acceptor_identity, 
			      AC_DEFINE(HAVE_GSSKRB5_REGISTER_ACCEPTOR_IDENTITY))
		AC_CHECK_FUNC(krb5_cc_new_unique, AC_DEFINE(HAVE_KRB5_CC_NEW_UNIQUE))
		AC_CHECK_FUNC(krb5_ipc_client_set_target_uid, AC_DEFINE(HAVE_KRB5_IPC_CLIENT_SET_TARGET_UID))

		AC_MSG_CHECKING(for GSS_C_NT_HOSTBASED_SERVICE)
		if test $ac_cv_header_gssapi_gssapi_h = yes; then
			AC_TRY_COMPILE([ #include <gssapi/gssapi.h> ],
				       [ gss_OID foo = GSS_C_NT_HOSTBASED_SERVICE; ],
				       AC_DEFINE(HAVE_GSS_C_NT_HOSTBASED_SERVICE)
				       AC_MSG_RESULT(yes),
				       AC_MSG_RESULT(no))
		else
			AC_TRY_COMPILE([ #include <gssapi.h> ],
				       [ gss_OID foo = GSS_C_NT_HOSTBASED_SERVICE; ],
				       AC_DEFINE(HAVE_GSS_C_NT_HOSTBASED_SERVICE)
				       AC_MSG_RESULT(yes),
				       AC_MSG_RESULT(no))
		fi

		LIBS="$SAVELIBS"

		AC_MSG_CHECKING(for Heimdal Kerberos)
		AC_TRY_COMPILE([ #include <krb5.h> ],
			       [ char *tmp = heimdal_version; ],
			       AC_DEFINE(HAVE_HEIMDAL)
			       AC_MSG_RESULT(yes),
			       AC_MSG_RESULT(no))
	fi
fi

dnl Default GSS service name...
AC_ARG_WITH(gssservicename, [  --with-gssservicename   set default gss service name],
	default_gssservicename="$withval",
	default_gssservicename="default")

if test x$default_gssservicename != xno; then
	if test "x$default_gssservicename" = "xdefault"; then
		CUPS_DEFAULT_GSSSERVICENAME="host"
	else
		CUPS_DEFAULT_GSSSERVICENAME="$default_gssservicename"
	fi
else
	CUPS_DEFAULT_GSSSERVICENAME=""
fi

AC_DEFINE_UNQUOTED(CUPS_DEFAULT_GSSSERVICENAME, "$CUPS_DEFAULT_GSSSERVICENAME")

dnl
dnl End of "$Id: cups-gssapi.m4 8659 2009-05-18 22:03:04Z mike $".
dnl
