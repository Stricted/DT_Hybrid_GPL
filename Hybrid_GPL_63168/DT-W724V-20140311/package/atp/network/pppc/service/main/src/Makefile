#
# pppd makefile for Linux
#
include $(CONFIG_DIR)/.config
# Default installation locations
BINDIR = /usr/sbin
MANDIR = /usr/man

#PPPXLIBDIR = ../pppd

PPPDSRCS = main.c magic.c fsm.c lcp.c ipcp.c upap.c chap.c md5.c \
	   auth.c options.c sys-linux.c md4.c chap_ms.c cbcp.c \
	   demand.c utils.c tdb.c shm.c pppoatm.c tty.c pppol2tp.c
HEADERS =  callout.h pathnames.h patchlevel.h chap.h md5.h chap_ms.h md4.h \
	   ipxcp.h cbcp.h tdb.h  shm.h
MANPAGES = pppd.8

PPPDOBJS = $(O)main.o $(O)magic.o $(O)fsm.o $(O)lcp.o $(O)ipcp.o $(O)upap.o $(O)chap.o $(O)md5.o \
	   $(O)auth.o $(O)options.o $(O)demand.o $(O)utils.o $(O)sys-linux.o \
	   $(O)tdb.o \
	    $(O)pppoe.o $(O)pppoehash.o $(O)pppoe_client.o $(O)libpppoe.o \
	    $(O)atp_utils.o $(O)shm.o


ifneq ("$(BUILD_PPPoA)", "")
ifeq ("$(BROADCOM_CHIP)", "y")
COMPILE_FLAGS += -DATP_SUPPORT_BCM 
PPPDOBJS += $(O)pppoatm_bcm.o
else
PPPDOBJS += $(O)pppoatm.o
endif
endif

ifneq ("$(BUILD_L2TP_KRNL_MODE)", "")
PPPDOBJS += $(O)pppol2tp.o
endif

ifneq ("$(BUILD_PPPoS)", "")
PPPDOBJS += $(O)tty.o
endif

vpath	%.c .
vpath	%.c ./plugins/pppoe
vpath	%.h .
vpath	%.h ./include

#all: pppd
all: $(O)pppd.a
#all: pppd

#
# include dependancies if present and backup if as a header file
ifeq (.depend,$(wildcard .depend))
include .depend
endif

#
# COPTS = -O2 -pipe -Wall -g
#COPTS = -O2 -pipe -Wall -DDEBUG
COPTS = -s -Os -pipe -Wall
LIBS = $(O)lib/libatm.a

#ifneq ($(wildcard $(LIBCDIR)/lib/libcrypt.*),)
LIBS += -lcrypt
#LIBS += $(LIBCDIR)/lib/libcrypt.a
#endif

# Uncomment the next 2 lines to include support for Microsoft's
# MS-CHAP authentication protocol.

ifeq ("$(BUILD_SMALL_SYSTEM)", "")
CHAPMS=y
USE_CRYPT=y
endif
#ifneq ($(wildcard /usr/lib/libcrypt.*),)
#HAVE_CRYPT_H=y
#endif

# Uncomment the next line to include support for PPP packet filtering.
# This requires that the libpcap library and headers be installed
# and that the kernel driver support PPP packet filtering, which it
# doesn't yet.
#FILTER=y

#HAS_SHADOW=y
#USE_PAM=y

#ifneq ($(strip $(BUILD_PPPC6)),)
HAVE_INET6= $(if $(BUILD_PPPC6),"y")
#endif

#PLUGIN=y

INCLUDE_DIRS= -I. -I../include -I$(INC_HUAWEICFM_PATH) -I$(ATMLIBDIR)
#-I/opt/sgi17/linux/include

COMPILE_FLAGS= -D_linux_=1 -DHAVE_PATHS_H -DHAVE_MMAP
ifeq ($(strip $(BUILD_PPPD)), static)
COMPILE_FLAGS += -DBUILD_STATIC
endif

CFLAGS= $(COPTS) $(COMPILE_FLAGS) $(INCLUDE_DIRS)\ -I$(INC_MANAGE_DIR)\ -I$(INC_DRIVER_HUAWEI_DIR) -I$(DRIVER_HUAWEI_BHAL_ADAPTER_DIR) -I$(INC_FLASHCONF_DIR)
# For complile warning
CFLAGS += -fno-strict-aliasing

ifdef CHAPMS
CFLAGS   += -DCHAPMS=1
ifndef USE_CRYPT
LIBS     := -ldes $(LIBS)
else
CFLAGS   += -DUSE_CRYPT=1
ifneq ($(wildcard /usr/include/crypt.h),)
CFLAGS   += -DHAVE_CRYPT_H=1
endif
endif
PPPDOBJS += $(O)md4.o $(O)chap_ms.o
ifdef MSLANMAN
CFLAGS   += -DMSLANMAN=1
endif
endif

ifdef HAS_SHADOW
CFLAGS   += -DHAS_SHADOW
#LIBS     := -lshadow $(LIBS)
endif

# For "Pluggable Authentication Modules", see ftp.redhat.com:/pub/pam/.
ifdef USE_PAM
CFLAGS   += -DUSE_PAM
LIBS     := -lpam -ldl $(LIBS)
endif

# Lock library binary for Linux is included in 'linux' subdirectory.
ifdef LOCKLIB
LIBS     := -llock $(LIBS)
CFLAGS   += -DLOCKLIB=1
endif

ifdef PLUGIN
CFLAGS	+= -DPLUGIN
LDFLAGS	+= -Wl,-E
LIBS	+= -ldl
endif

ifdef FILTER
LIBS    += -lpcap
CFLAGS  += -DPPP_FILTER -I/usr/include/pcap
endif

ifneq ($(strip $(BUILD_PPPD)),)
CFLAGS += -DSUPPORT_PPPX
#PPPDOBJS += $(PPPXLIBDIR)/shm.o

endif

ifdef BUILD_PPPC6
     PPPDSRCS += ipv6cp.c eui64.c
     HEADERS  += ipv6cp.h eui64.h
     PPPDOBJS += $(O)ipv6cp.o $(O)eui64.o
     CFLAGS   += -DINET6=1
endif

ifeq ($(strip $(PPP_AUTODISCONN)),y)
CFLAGS += -DAUTODISCONN
endif

ifneq ($(strip $(BUILD_HGC)),)
CFLAGS += -DSUPPORT_HGC
endif

CFLAGS += $(ATPFLAGS) -I$(INC_DRIVER_VENDOR_DIR)/

# start of y42304 2008-12-20:优化编译选项，减少空间
# CFLAGS += $(VENDOR_APP_CFLAGS)

ifneq ($(strip $(BUILD_TD_CARD)),)
CFLAGS += -DSUPPORT_TD_CARD
endif
# end of y42304 2008-4-19: 优化编译选项，减少空间

CFLAGS += -I$(STAGING_DIR)/usr/include
LIBS += -L$(STAGING_DIR)/lib

INSTALL= install -o root

LIBS+=-L$(LIB_ATP_DIR) -lgplutil

static: $(O)pppd.a

ifneq ($(strip $(BR2_ARCH)),i386)
dynamic: pppd install
else
dynamic:
	rm -f $(O)pppc
	touch $(O)pppc
endif

$(O)libpppcapi.a: $(O)pppcapi.o
	@if [ -f pppcapi.c ];then \
	$(AR) rcs $@ $(O)pppcapi.o;\
	cp -f $(O)libpppcapi.a $(LIB_ATP_DIR)/libpppcapi.a; \
	fi

install:
	install -m 755 $(O)pppc $(TARGET_DIR)/bin
	$(STRIP) $(TARGET_DIR)/bin/pppc

$(O)pppd.a: $(PPPDOBJS)
	$(AR) rcs $(O)pppd.a  $(PPPDOBJS)

$(O)%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

pppd: $(PPPDOBJS)
	$(MAKE) -C lib
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,--allow-shlib-undefined -o $(O)pppc $(PPPDOBJS) $(LIBS)

clean:
	$(MAKE) -C lib clean
	rm -fr $(PPPDOBJS) $(O)pppc  $(O)*

depend:
	$(CPP) -M $(CFLAGS) $(PPPDSRCS) >.depend
