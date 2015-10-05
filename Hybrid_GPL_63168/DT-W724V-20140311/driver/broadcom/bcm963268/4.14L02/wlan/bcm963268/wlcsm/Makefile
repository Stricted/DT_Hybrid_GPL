#Linux Makefile
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#WIRELESS_DRIVER_PATH=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)
SRCBASE=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)
#include $(BUILD_DIR)/make.common
#include $(SRCBASE)/dslcpe_wlan_minor_version
#DSLCPE_WLAN_VERSION=cpe$(BRCM_VERSION).$(BRCM_RELEASE)'L'$(BRCM_EXTRAVERSION).$(DSLCPE_WLAN_MINOR_VERSION)

INCLUDE += -I$(SRCBASE)/wl/wlcsm_ext/include -I./include
INCLUDE += -I. -I.. -I$(SRCBASE)/include -I$(SRCBASE)/common/include -I$(SRCBASE)/shared -I$(SRCBASE)/router/shared -I$(SRCBASE)/wl/exe
INCLUDE += -I$(SRCBASE)/security/secfrw/include -I$(SRCBASE)/supp/include -I$(SRCBASE)/shared/bcmwifi/include

CFLAGS += $(INCLUDE)
CFLAGS += -Wall

CFLAGS  += -s
#CFLAGS += -DDSLCPE_WLAN_VERSION=\"$(DSLCPE_WLAN_VERSION)\"
CFLAGS += -DDSLCPE
CFLAGS += -fPIC


#LDFLAGS += -L. -L$(TOP)/nvram -L$(INSTALL_DIR)/lib -lnvram
#LDFLAGS += -L. 
#LDFLAGS += -lwlbcmcrypto
#LDFLAGS += -lwlbcmshared
#LDFLAGS += -L$(TOP)/shared -L$(INSTALL_DIR)/shared/usr/lib -lshared


vpath %.c $(CURR_DIR)/src


WLCSM_FILES = $(foreach x, $(shell find ./src -name \*.c) ,$(notdir $(x)))
WLCSM_OBJS = $(foreach x, $(WLCSM_FILES),$(x:.c=.o))

OBJS += $(WLCSM_OBJS)

#OBJS = trace.o
#OBJS += bcm_encode.o bcm_decode.o
#OBJS += bcm_encode_ie.o bcm_decode_ie.o
#OBJS += bcm_encode_gas.o bcm_decode_gas.o
#OBJS += bcm_encode_anqp.o bcm_decode_anqp.o
#OBJS += bcm_encode_hspot_anqp.o bcm_decode_hspot_anqp.o 
#OBJS += bcm_decode_wnm.o bcm_encode_wnm.o
#OBJS += bcm_decode_p2p.o
#OBJS += dsp_ap.o tmr.o
#OBJS += wlu_api.o wlu_linux_api.o
#OBJS += bcm_app_utils.o bcmseclib_linux_timer.o bcmseclib_timer.o bcm_llist.o
#OBJS += bcm_gas.o
#OBJS += bcmevent.o
#OBJS += tcp_srv.o

all: libwlcsm.so 
ifneq ($(strip $(WLCSM_FILES)),)
libwlcsm.so: $(OBJS)
	$(CC) -s -Os -fPIC -shared -o libwlcsm.so -lpthread $(OBJS)	
	cp -f libwlcsm.so libwlcsm-$(EXT_CPU_ARCH_NAME).so
else
libwlcsm.so:
	cp -f libwlcsm-$(EXT_CPU_ARCH_NAME).so libwlcsm.so
endif
	install -m 755 libwlcsm.so $(LIB_ATP_DIR)/libwlcsm.so

install: all
	install -m 755 libwlcsm.so $(LIB_ATP_DIR)/libwlcsm.so
	install -m 755 libwlcsm.so $(TARGET_DIR)/lib/libwlcsm.so
	
dynamic: all install
#libwlcsm.a:
#	$(AR) -cvq $@ $(OBJS)
#	cp -f libwlcsm.a libwlcsm-$(EXT_CPU_ARCH_NAME).so
clean:
	rm -f *.o *.map *.d  *.d.*
	rm -f libwlcsm.so

