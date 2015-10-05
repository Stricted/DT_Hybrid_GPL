# wlbcmshared shared library makefile

BUILD_DIR=$(word 1, $(subst /bcmdrivers, /bcmdrivers,$(shell pwd)))
#include $(BUILD_DIR)/make.common

CFLAGS += -I.
CFLAGS += -I../../include
CFLAGS += -I../../common/include
CFLAGS += -I../../shared
CFLAGS += -I../../shared/bcmwifi/include
CFLAGS += -DBCMSUP_PSK
CFLAGS += -DBCMSUPPL
CFLAGS += -DBCMWPS
CFLAGS += -DBCMWPA2
CFLAGS += -DIL_BIGENDIAN
CFLAGS += -DDSLCPE
CFLAGS += -s
CFLAGS += -Os
CFLAGS += -Wall
CFLAGS += -fomit-frame-pointer

LDFLAGS = -shared
LDFLAGS += -Os

-include files

OBJS = $(foreach x, $(FILES), $(shell find . ../../shared -name "$x" -exec echo $(x:.c=.o) ";"))

vpath %.c ./
vpath %.c ../../shared

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

bcmshared: $(OBJS)
ifneq ($(strip $(OBJS)),)
ifeq ($(BRCM_WAPI),y)
	$(CC) $(LDFLAGS) -o libwlbcmshared_wapi.so $^
	cp -f libwlbcmshared_wapi.so libwlbcmshared_wapi-$(EXT_CPU_ARCH_NAME).so
else
	$(CC) $(LDFLAGS) -o libwlbcmshared.so $^
	cp -f libwlbcmshared.so libwlbcmshared-$(EXT_CPU_ARCH_NAME).so
endif
else
ifeq ($(BRCM_WAPI),y)
	cp -f libwlbcmshared_wapi-$(EXT_CPU_ARCH_NAME).so libwlbcmshared_wapi.so
else
	cp -f libwlbcmshared-$(EXT_CPU_ARCH_NAME).so libwlbcmshared.so
endif
endif

static: $(OBJS)
ifneq ($(strip $(OBJS)),)
ifeq ($(BRCM_WAPI),y)
	$(AR) -rcs libwlbcmshared_wapi.a $^
else
	$(AR) -rcs libwlbcmshared.a $^
endif
endif

install: bcmshared
ifeq ($(BRCM_WAPI),y)
	install -m 755 libwlbcmshared_wapi.so $(LIB_ATP_DIR)/libwlbcmshared.so
	install -m 755 libwlbcmshared_wapi.so $(TARGET_DIR)/lib/libwlbcmshared.so
else
	install -m 755 libwlbcmshared.so $(LIB_ATP_DIR)/libwlbcmshared.so
	install -m 755 libwlbcmshared.so $(TARGET_DIR)/lib/libwlbcmshared.so
endif

clean:
	rm -f $(OBJS)
ifneq ($(strip $(OBJS)),)
	rm -f libwlbcmshared.a libwlbcmshared.so
	rm -f libwlbcmshared_wapi.a libwlbcmshared_wapi.so
endif
dynamic: install
