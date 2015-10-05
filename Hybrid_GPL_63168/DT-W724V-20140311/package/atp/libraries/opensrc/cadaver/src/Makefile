# cadaver Makefile: generated from Makefile.in

PACKAGE = cadaver
VERSION = 0.23.3

SHELL = /bin/sh


# Installation paths.
prefix = /home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/bin
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
mandir = ${datarootdir}/man
man1dir = $(mandir)/man1
datarootdir = ${prefix}/share
datadir = $(prefix)/share
docdir = $(prefix)/doc/cadaver

# intl stuff
localedir = $(datadir)/locale
gnulocaledir = $(prefix)/share/locale
gettextsrcdir = $(prefix)/share/gettext
aliaspath = $(localedir):.

# Build paths
top_srcdir = .
top_builddir = .


# Toolchain settings.
CC = /opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/mips-unknown-linux-uclibc-gcc
INCLUDES = -I$(top_srcdir)/src
CPPFLAGS = -DHAVE_CONFIG_H -I$(top_builddir) -I$(top_srcdir)/lib  -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/include  -I/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/usr/include -D_LARGEFILE64_SOURCE -DNE_LFS -DXML_BYTE_ORDER=21 -DXML_DTD -I$(top_srcdir)/lib/expat/xmlparse -I$(top_srcdir)/lib/expat/xmltok -DLOCALEDIR=\"$(localedir)\"
CFLAGS = -g -O2 -I$(top_srcdir)/lib/neon -Wall -Wmissing-declarations -Wshadow -Wreturn-type -Wsign-compare -Wundef -Wpointer-arith -Wbad-function-cast -Wformat-security
ALL_CFLAGS = $(CPPFLAGS) $(INCLUDES) $(CFLAGS)
LDFLAGS =  -L/home/zengyao/DT_Hybrid_GPL_1.00.052/DT-W724V-20140311/output/host/usr/mips-unknown-linux-uclibc/sysroot/lib 
LIBS =  -Llib/neon -lneon -lssl_openssl -lcrypto_openssl  -lgplutil

TARGET = $(PACKAGE)
SUBDIRS = lib/neon lib/intl
OBJECTS = src/cadaver.o src/common.o src/commands.o src/ls.o	\
	 src/cmdline.o src/options.o src/utils.o src/edit.o \
	src/version.o src/search.o src/webdav/webdavclient.o
LIBOBJS = lib/basename.o lib/dirname.o lib/rpmatch.o lib/yesno.o	\
	lib/glob.o lib/getpass.o lib/tempname.o lib/mkstemp.o \
	 ${LIBOBJDIR}$(top_builddir)/lib/expat/xmltok/xmltok$U.o ${LIBOBJDIR}$(top_builddir)/lib/expat/xmltok/xmlrole$U.o ${LIBOBJDIR}$(top_builddir)/lib/expat/xmlparse/xmlparse$U.o ${LIBOBJDIR}$(top_builddir)/lib/expat/xmlparse/hashtable$U.o ${LIBOBJDIR}lib/netrc$U.o
ALLOBJS = $(OBJECTS) $(LIBOBJS)

# Installation programs
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL = /usr/bin/install -c

all: $(TARGET)

.PHONY: subdirs install clean distclean

$(TARGET): $(ALLOBJS) subdirs
	$(CC) $(LDFLAGS) -o $(TARGET) $(ALLOBJS) $(LIBS)

.c.o:
	$(CC) $(ALL_CFLAGS) -o $@ -c $<

subdirs:
	for d in $(SUBDIRS); do (cd $$d; $(MAKE)); done

clean:
	rm -f ../.stamp_configured ../.stamp_opensslconfigured
	rm -f $(OBJECTS) $(LIBOBJS) $(TARGET) *~
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean); done

distclean: clean
	rm -f Makefile config.*
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) distclean); done

install: $(TARGET) install-nls
	@echo "Creating directories..."
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(man1dir)
	@echo "Installing $(TARGET) executable..."
	$(INSTALL_PROGRAM) $(TARGET) $(DESTDIR)$(bindir)/$(TARGET)
	@echo "Installing man page..."
	$(INSTALL_DATA) $(top_srcdir)/doc/cadaver.1 $(DESTDIR)$(man1dir)/cadaver.1

install-nls:
	@cd po && $(MAKE) install

Makefile: Makefile.in
	@./config.status Makefile

# Deps
src/cadaver.o: src/cadaver.c config.h src/common.h	\
	src/options.h src/cmdline.h src/commands.h src/cadaver.h src/webdav/webdavclient.h
src/options.o: src/options.c config.h src/options.h src/cadaver.h src/common.h 
src/cmdline.o: src/cmdline.c src/cmdline.h src/cadaver.h lib/basename.h \
	lib/glob.h src/commands.h src/common.h
src/commands.o: src/commands.c src/commands.h src/cadaver.h \
	lib/basename.h src/options.h src/common.h
src/edit.o: src/edit.c src/cadaver.h src/options.h src/common.h
src/common.o: src/common.c src/common.h config.h
src/ls.o: src/ls.c src/commands.h src/cadaver.h config.h
src/search.o: src/search.c src/commands.h src/cadaver.h config.h
src/utils.o: src/utils.c src/utils.h src/cadaver.h config.h
src/version.o: src/version.c src/utils.h src/cadaver.h config.h
lib/netrc.o: lib/netrc.c lib/netrc.h config.h
lib/getpass.o: lib/getpass.c lib/getpass.h config.h
lib/tempname.o: lib/tempname.c config.h
lib/mkstemp.o: lib/mkstemp.c src/common.h config.h
lib/yesno.o: lib/yesno.c src/common.h config.h
src/webdav/webdavclient.o: src/webdav/webdavclient.c src/webdav/webdavclient.h

