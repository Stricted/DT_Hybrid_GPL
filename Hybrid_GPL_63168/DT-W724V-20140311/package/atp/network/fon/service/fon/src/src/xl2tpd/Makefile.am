
sbin_PROGRAMS = xl2tpd

DFLAGS= -DDEBUG_PPPD -DTRUST_PPPD_TO_DIE
OSFLAGS= -DLINUX
FFLAGS= -DIP_ALLOCATION

AM_CFLAGS = $(DFLAGS) -fno-builtin -Wall -DSANITY $(OSFLAGS) $(FFLAGS) \
-I../chillout/ -L$(LIB_ATP_DIR) -lgplutil

xl2tpd_SOURCES= aaa.c aaa.h avp.c avp.h avpsend.c call.c call.h \
common.h control.c control.h file.c file.h ipsecmast.h l2tp.h \
md5.c md5.h misc.c misc.h network.c osport.h pty.c scheduler.c \
scheduler.h getdevstats.c xl2tpd.c

LDADD = ../chillout/libchillout.la

include $(TOPDIR)/package/atp/rule.mak
