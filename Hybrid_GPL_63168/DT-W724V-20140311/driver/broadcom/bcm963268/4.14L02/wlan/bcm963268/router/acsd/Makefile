# acsd makefile

BUILD_DIR=$(word 1, $(subst /bcmdrivers, /bcmdrivers,$(shell pwd)))
#include $(BUILD_DIR)/make.common

CFLAGS += -Iinclude
CFLAGS += -I../../include
CFLAGS += -I../../common/include
CFLAGS += -I../../include/bcmcrypto
CFLAGS += -I../../shared
CFLAGS += -I../../router/shared
CFLAGS += -I../../shared/bcmwifi/include
CFLAGS += -DDSLCPE
CFLAGS += -Wall
CFLAGS += -DPKTC
CFLAGS += -DBCMDBG
#CFLAGS += -DDEBUG
CFLAGS += -fomit-frame-pointer -Wl,-allow-shlib-undefined -DCONFIG_32BIT 


LDFLAGS = -Os
LDFLAGS += -L$(EXTRALIBDIR)
LDFLAGS += -lc
LDFLAGS += -lgcc_s
LDFLAGS += -L$(LIB_ATP_DIR)
LDFLAGS += -lnvram
ifeq ($(DSLCPE_WLCSM_EXT),1)
LDFLAGS += -lwlcsm
endif
LDFLAGS += -lwlbcmcrypto
LDFLAGS += -lwlbcmshared
LDFLAGS += -lcfmapi -lhttpapi -llgplutil -lmsgapi -lxmlapi -lbhalapi -lm -lssl -lcrypto -lz

WIRELESS_DRIVER_PATH=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)
BCMWIFI_WIRELESS_ROUTER_PATH=$(WIRELESS_DRIVER_PATH)/shared/bcmwifi/src
ACSD_SRCFILES = dcs.c acs.c acsd_utils.c acsd_main.c acsd_cmd.c acsd_chanim.c bcmutils.c acs_dfsr.c bcmwifi_channels.c
#ACSD_OBJS_TMP = $(foreach x, $(ACSD_SRCFILES), $(shell find . -name "$x" -exec echo $(x:.c=.o) ";"))
ACSD_OBJS:=$(patsubst %.c,$(O)%.o,$(ACSD_SRCFILES))

ACSCLI_SRCFILES = acsd_cli.c acsd_utils.c
ACSCLI_OBJS_TMP = $(foreach x, $(ACSCLI_SRCFILES), $(shell find . -name "$x" -exec echo $(x:.c=.o) ";"))
ACSCLI_OBJS:=$(patsubst %.c,$(O)%.o,$(ACSCLI_SRCFILES))

$(warning ACSD_OBJ=$(ACSD_OBJS))
$(warning ACSCLI_OBJ=$(ACSCLI_OBJS))

vpath %.c ../../shared/ $(BCMWIFI_WIRELESS_ROUTER_PATH)

$(O)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

acsd: $(ACSD_OBJS)
ifneq ($(strip $(ACSD_OBJS)),)
	$(CC) $(CFLAGS) $^ -o $(O)$@  $(LDFLAGS)
endif
	install -m 755 $(O)$@ $(INSTALL_DIR)/bin/$@

acs_cli: $(ACSCLI_OBJS)
ifneq ($(strip $(ACSCLI_OBJS)),)
	$(CC) $(CFLAGS) $^ -o $(O)$@  $(LDFLAGS)
endif
	install -m 755 $(O)$@ $(INSTALL_DIR)/bin/$@

clean:
	rm -f $(ACSD_OBJS)
	rm -f $(ACSCLI_OBJS)
ifneq ($(strip $(ACSD_OBJS)),)
	rm -f $(O)acsd
endif
ifneq ($(strip $(ACSCLI_OBJS)),)
	rm -f $(O)acs_cli
endif

dynamic static: acsd acs_cli
