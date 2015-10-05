#
# nas: 802.1x EAPOL to RADIUS proxy (Network Access Server)
#

BUILD_DIR=$(word 1, $(subst /bcmdrivers, /bcmdrivers,$(shell pwd)))
#include $(BUILD_DIR)/make.common

CFLAGS += -I.
CFLAGS += -I../../include
CFLAGS += -I../../common/include
CFLAGS += -I../../bcmcrypto
CFLAGS += -I../../shared
CFLAGS += -I../../router/eapd
CFLAGS += -I../../router/shared
CFLAGS += -I../../shared/bcmwifi/include
CFLAGS += -I$(INC_APP_DIR)

ifeq ($(DSLCPE_DT_BUILD),1)
	CFLAGS	+= -DRADIUS_RESTRICTION
endif
CFLAGS  += -DNAS_WKSP_BUILD_NAS_AUTH=1
CFLAGS	+= -DNAS_WKSP_BUILD_NAS_SUPPL=1
CFLAGS	+= -DNAS_WKSP_BUILD_CMD_LINE=0
CFLAGS	+= -DNAS_WKSP_USE_FILE_SYSTEM=1
CFLAGS	+= -DNAS_RADIUS
CFLAGS	+= -DNAS_WKSP_SELECT_SUPPORT
CFLAGS	+= -DNAS_WDS_SUPPORT
CFLAGS	+= -DNAS_WKSP_ON_DEMAND
CFLAGS	+= -DBCMSUPPL
CFLAGS	+= -DBCMWPS
CFLAGS	+= -DBCMWPA2
CFLAGS  += -DIL_BIGENDIAN
CFLAGS	+= -DDSLCPE
CFLAGS  += -DDSLCPE_EVT
#CFLAGS  += -DDSLCPE_MULTI_RADIUS
#CFLAGS += -DBCMDBG
ifeq ($(BUILD_WLHSPOT),y)
CFLAGS += -DNAS_GTK_PER_STA
endif

CFLAGS += -Wall
CFLAGS += -s
CFLAGS += -Os
CFLAGS += -fomit-frame-pointer              

LDFLAGS = -Os
LDFALGS += -L$(EXTRALIBDIR)
LDFLAGS += -lc
LDFLAGS += -lgcc_s
LDFLAGS += -L$(LIB_ATP_DIR)
LDFLAGS += -lnvram
ifeq ($(DSLCPE_WLCSM_EXT),1)
LDFLAGS += -lwlcsm
endif
LDFLAGS += -lwlbcmcrypto
LDFLAGS += -lwlbcmshared
LDFLAGS += -latputil -lmsgapi -lbhalapi -lm

-include files

OBJS = $(foreach x, $(FILES), $(shell find . -name "$x" -exec echo $(x:.c=.o) ";"))

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

nas: $(OBJS)
ifneq ($(strip $(OBJS)),)
	$(CC) -o $@ $^ $(LDFLAGS) 
	cp -f $@ $@-$(EXT_CPU_ARCH_NAME)
else
	cp -f $@-$(EXT_CPU_ARCH_NAME) $@
endif

nas.a: $(OBJS)
ifneq ($(strip $(OBJS)),)
	$(AR) rcs $@ $^
endif

install: nas
	install -m 755 $< $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/$<
	ln -sf ../bin/$< $(INSTALL_DIR)/bin/nas4not

clean:
	rm -f $(OBJS)
ifneq ($(strip $(OBJS)),)
	rm -f nas.a
	rm -f nas
endif

dynamic: nas install
static: nas.a


