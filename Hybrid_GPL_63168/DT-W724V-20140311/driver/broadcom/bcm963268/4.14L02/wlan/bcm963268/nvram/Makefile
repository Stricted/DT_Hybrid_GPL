#
# build file for nvram
#

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#include $(BUILD_DIR)/make.common

#include ../../../../make.deprules


INC_BRCMCFM_PATH=

PROGRAM_NAME:= nvram

CFLAGS =-s -Os -fomit-frame-pointer -I$(INC_DRIVER_VENDOR_DIR) -I$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)/include



LDFLAGS=-Os 
CFLAGS_EXTRA =

WIRELESS_DRIVER_PATH=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)
INC_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/include
WIRELESS_SHARED_PATH=$(WIRELESS_DRIVER_PATH)/shared
WIRELESS_CRYPTO_PATH=$(WIRELESS_DRIVER_PATH)/crypto

INC_KERNEL_INC_PATH=$(HOST_INCLUDE_DIR)

CFLAGS += -Wall -I$(INC_DRIVER_VENDOR_DIR) -I../include -I$(INC_WIRELESS_DRIVER_PATH)/proto -I$(INC_WIRELESS_DRIVER_PATH) -I$(INC_KERNEL_INC_PATH) -DDSLCPE
CFLAGS += -fPIC
ifeq ($(strip $(BUILD_WLCTL)), static)
CFLAGS += -DBUILD_STATIC 
endif

CFLAGS += -I$(TOOLCHAIN)/include

$(O)nvram_shared.o: nvram_shared.c
	@mkdir -p $(dir $@)
	$(CC) -g -fpic -c -DPROTOTYPES $(CFLAGS) -c -o $@ $<
	
$(O)brcm_generic_lock.o: brcm_generic_lock.c
	@mkdir -p $(dir $@)
	$(CC) -c -DPROTOTYPES $(CFLAGS) -o $@ $<
	
$(O)main.o: main.c
	@mkdir -p $(dir $@)
	$(CC) -g -c -DPROTOTYPES $(CFLAGS) -c -o $@ $<
	
OBJS = $(O)nvram_shared.o $(O)main.o $(O)brcm_generic_lock.o

all:  libnvram nvram 

install: $(PROGRAM_NAME)
	install -m 755 $(O)$(PROGRAM_NAME) $(INSTALL_DIR)/bin	
	$(STRIP) $(INSTALL_DIR)/bin/$(PROGRAM_NAME)
	cp -av $(O)lib$(PROGRAM_NAME).so $(INSTALL_DIR)/lib 

dynamic: all install
libnvram: $(O)nvram_shared.o $(O)brcm_generic_lock.o
	$(LD) -shared -soname $(O)libnvram.so -o $(O)libnvram.so $(O)nvram_shared.o $(O)brcm_generic_lock.o -L$(LIBDIR)	-lc
	install -m 755 $(O)libnvram.so $(LIB_ATP_DIR)
	install -m 755 $(O)libnvram.so $(TARGET_DIR)/lib

nvram: libnvram $(O)main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -L. -o $(O)nvram $(O)main.o -lnvram

clean:
	-rm -f $(O)libnvram.so $(O)nvram $(OBJS)


	