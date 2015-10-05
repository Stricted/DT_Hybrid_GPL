# Makefile file for bftpd
#
# License:  GNU General Public License, Version 2.
#
CFLAGS= -Wall -s -Os -fomit-frame-pointer \
        $(ATPFLAGS) \
        -I.

LDFLAGS=-Wl,-allow-shlib-undefined
ifeq ($(strip $(BUILD_FTPD)), static)
CFLAGS += -DBUILD_STATIC
endif

ifneq ($(strip $(BUILD_ONE_USER)),)
CFLAGS += -DSUPPORT_ONE_USER
endif


# start of y42304 2008-12-20:优化编译选项，减少空间
CFLAGS += $(VENDOR_APP_CFLAGS) -I$(INC_APP_DIR)
# end of y42304 2008-4-19: 优化编译选项，减少空间

# start of y42304 2009-1-10:支持大文件
#ifneq ($(strip $(UCLIBC_HAS_LFS)),)
#CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
# end of y42304 2009-1-10

# start of 2009-3-15:支持大文件
CFLAGS += -D__USE_LARGEFILE64 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -D__USE_FILE_OFFSET64 
CFLAGS += -DVERSION=\"$(VERSION)\" 
ifdef BUILD_FTPDS
CFLAGS += -I$(OPENSSL_INC_DIR)
endif
# end of 2009-3-15:支持大文件

all: $(O)bftpd

# LIBS= -lcrypt
LIBS= -L$(LIB_ATP_DIR) -lcrypt -lgplutil -lpthread
#ifdef BUILD_FTPDS
LIBS += -lssl
#endif
HEADERS=bftpdutmp.h commands.h commands_admin.h cwd.h dirlist.h list.h login.h logging.h main.h mystring.h options.h targzip.h mypaths.h bftpd_message.h 
OBJS=$(O)bftpdutmp.o $(O)commands.o $(O)commands_admin.o $(O)cwd.o $(O)dirlist.o $(O)list.o $(O)login.o $(O)logging.o $(O)main.o $(O)mystring.o $(O)options.o bftpd_message.o
SRCS=bftpdutmp.c commands.c commands_admin.c cwd.c dirlist.c list.c login.c logging.c main.c mystring.c options.c bftpd_message.c 

install:
	install -m 755 $(O)bftpd $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/bftpd

dynamic: $(O)bftpd install

static: $(O)bftpd.a

$(O)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_$@) $(DBG_FLAGS) -c -o $@ $<

$(O)bftpd: $(HEADERS) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(O)bftpd.a: $(HEADERS) $(OBJS)
	$(AR) rcs bftpd.a $(OBJS) $(LIBS)

clean:
	-rm -f $(O)bftpd $(OBJS) $(O)bftpd.a


