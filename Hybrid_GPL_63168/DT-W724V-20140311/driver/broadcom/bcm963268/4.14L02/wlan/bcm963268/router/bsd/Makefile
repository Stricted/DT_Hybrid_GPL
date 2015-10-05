# BSD makefile

BUILD_DIR=$(word 1, $(subst /bcmdrivers, /bcmdrivers,$(shell pwd)))
include $(BUILD_DIR)/make.common

CFLAGS += -I.
CFLAGS += -I../../include
CFLAGS += -I../../common/include
CFLAGS += -I../../bcmcrypto
CFLAGS += -I../../shared
CFLAGS += -I../../router/shared
CFLAGS += -I../../shared/bcmwifi/include
CFLAGS += -DIL_BIGENDIAN
CFLAGS += -DDSLCPE
CFLAGS += -Wall
#ifdef BCMINTERNAL
ifneq ($(NO_BCMINTERNAL), 1)
CFLAGS += -DBCMINTERNAL
endif

LDFLAGS = -O2
LDFLAGS += -L$(EXTRALIBDIR)
LDFLAGS += -lc
LDFLAGS += -lgcc_s
LDFLAGS += -L$(INSTALL_DIR)/lib
ifeq ($(DSLCPE_WLCSM_EXT),1)
LDFLAGS += -lwlcsm
endif

LDFLAGS += -lnvram
LDFLAGS += -lwlbcmcrypto
LDFLAGS += -lwlbcmshared

BSD_SRCFILES = bsd_main.c bsd.c bsd_util.c bsd_comm.c 
BSD_OBJS = $(foreach x, $(BSD_SRCFILES), $(shell find . -name "$x" -exec echo $(x:.c=.o) ";"))
BSD_OBJS += bcmwifi_channels.o
$(warning BSD_OBJS=$(BSD_OBJS))

#endif
#vpath %.c ./
vpath %.c ../../shared/ ../../shared/bcmwifi/src/
#$(warning $(BSD_OBJS))

$(BSD_OBJS) : bsd.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

bsd: $(BSD_OBJS)
ifneq ($(strip $(BSD_OBJS)),)
	$(CC) $(CFLAGS) $^ -o $@  $(LDFLAGS)
	cp -f $@ $@-$(EXT_CPU_ARCH_NAME)
else
	cp -f $@-$(EXT_CPU_ARCH_NAME) $@
endif

install: bsd
	install -m 755 $< $(INSTALL_DIR)/bin/$<

clean:
	rm -f $(BSD_OBJS)
	rm -f *.o *.d
ifneq ($(strip $(BSD_OBJS)),)
	rm -f bsd 
endif

dynamic static: bsd


OBJS = $(BSD_OBJS)


#
# Include the rule for making dependency files.
# The '-' in front of the second include suppresses
# error messages when make cannot find the .d files.
# It will just regenerate them.
# See Section 4.14 of Gnu Make.
#

include $(BUILD_DIR)/make.deprules

-include $(OBJS:.o=.d)
