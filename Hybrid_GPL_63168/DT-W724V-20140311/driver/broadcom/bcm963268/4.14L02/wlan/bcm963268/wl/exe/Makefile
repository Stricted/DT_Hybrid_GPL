#
# Makefile for wlctl utility.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /bcmdriver, /bcmdriver,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#include $(BUILD_DIR)/make.common
BUILD_WLCTL_SHLIB = 1

PROGRAM_NAME:= wlctl

RWL=1
ifeq ($(RWL),1)
SERVER_SOCKET:= wl_server_socket
endif

#CFLAGS +=-s -Os -fomit-frame-pointer -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
#CFLAGS += -I$(TOOLCHAIN)/include

CFLAGS += -DLINUX
ifeq ($(RWL),1)
CFLAGS += -DRWL_SOCKET  #-DRWL_DONGLE -DRWL_WIFI
endif
CFLAGS += -fno-strict-aliasing


LDFLAGS=-Os -Wl,-allow-shlib-undefined
CFLAGS_EXTRA =

WIRELESS_DRIVER_PATH=$(DRIVER_VENDOR_DIR)/wlan/$(CHIP_NAME)
INC_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/include
SHARED_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/shared
SHARED_WIRELESS_ROUTER_PATH=$(WIRELESS_DRIVER_PATH)/router/shared
BCMWIFI_WIRELESS_ROUTER_PATH=$(WIRELESS_DRIVER_PATH)/shared/bcmwifi/src
WLPPR_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/wl/ppr/src
COMM_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/common/include

INC_KERNEL_INC_PATH=$(KERNEL_DIR)/include
CFLAGS += -Wall -I$(INC_WIRELESS_DRIVER_PATH) -I$(COMM_WIRELESS_DRIVER_PATH) -I$(COMM_WIRELESS_DRIVER_PATH)/proto -I$(INC_KERNEL_INC_PATH) -DDSLCPE -DD11AC_IOTYPES -I$(WIRELESS_DRIVER_PATH)/shared/bcmwifi/include -I$(WIRELESS_DRIVER_PATH)/wl/ppr/include -DPPR_API 
CFLAGS += -I$(SHARED_WIRELESS_ROUTER_PATH)
#/* BEGIN: add by l00203771 2013/1/8 for DTS2013010808309 */
CFLAGS += -I$(DRIVER_VENDOR_DIR)/include
#/* END: add by l00203771 2013/1/8 for DTS2013010808309 */

# Append DSLCPE_WLAN_VERSION to EPI_VERSION_STR
include ../../dslcpe_wlan_minor_version
DSLCPE_WLAN_VERSION=cpe$(BRCM_VERSION).$(BRCM_VERSION)$(BRCM_RELEASE).$(DSLCPE_WLAN_MINOR_VERSION)
CFLAGS += -DDSLCPE_WLAN_VERSION=\"$(DSLCPE_WLAN_VERSION)\"

ifeq ($(strip $(BUILD_WLCTL)), static)
CFLAGS += -DBUILD_STATIC 
endif
CFLAGS += -DBCMWPA2 -DIL_BIGENDIAN

ifeq ($(DSLCPE_DT_BUILD),1)
CFLAGS	+= -DRADIUS_RESTRICTION
endif

ifeq ($(strip $(BRCM_CHIP)),6338)
CFLAGS += -DDSLCPE_SDIO
endif

ifeq ($(strip $(BUILD_WLCTL_SHLIB)),1) 
CFLAGS += -DDSLCPE_SHLIB
CFLAGS_EXTRA = -fpic
endif

CFLAGS += -DDSLCPE -DDSLCPE_SCBLIST
CFLAGS += -DDSLCPE_HUAWEI

#wlctl cmd verbose to show cmd in console
#CFLAGS += -DDSLCPE_VERBOSE

### Packet Chaining ####
CFLAGS += -DPKTC

WLCTLOBJS = wlu.o wlu_cmd.o wlu_iov.o wlu_client_shared.o wlu_pipe_linux.o wlu_pipe.o wlu_common.o wlu_rates_matrix.o
ifeq ($(RWL),1)
SERVER_OBJS = wlu_server_linux.o wlu_pipe_linux.o wlu_pipe.o wlu_server_shared.o shellproc_linux.o
endif
SHAREDOBJS = bcmutils.o bcmwifi_channels.o miniopt.o bcm_app_utils.o
WLPPROBJS = wlc_ppr.o
ROUTERSHAREDOBJS = 

ifeq ($(strip $(BUILD_WLCTL_SHLIB)),1) 
MAIN = wlu_linux_dslcpe
WLCTLOBJS += wlu_linux.o
else
MAIN = wlu_linux
endif
ifeq ($(RWL),1)
MAIN_SERVER = wlu_server_linux
MAINSERVEROBJ = $(O)$(MAIN_SERVER).obj
endif

MAINOBJ = $(O)$(MAIN).obj
OBJS_TMP = $(WLCTLOBJS) $(SHAREDOBJS) $(ROUTERSHAREDOBJS) $(WLPPROBJS)
ifeq ($(RWL),1)
SOCKET_OBJS:=$(patsubst %,$(O)%,$(SERVER_OBJS))
endif

ifneq ($(wildcard $(SHARED_WIRELESS_DRIVER_PATH) $(BCMWIFI_WIRELESS_DRIVER_PATH)),)
vpath %.c $(SHARED_WIRELESS_DRIVER_PATH) $(CRYPTO_WIRELESS_DRIVER_PATH) $(SHARED_WIRELESS_ROUTER_PATH) $(BCMWIFI_WIRELESS_ROUTER_PATH) $(WLPPR_WIRELESS_DRIVER_PATH)
else
OBJS_TMP := $(foreach obj,$(OBJS_TMP),$(SHARED_WIRELESS_DRIVER_PATH)/$(OBJS_TMP)),$(CRYPTO_WIRELESS_DRIVER_PATH)/$(OBJS_TMP),$(SHARED_WIRELESS_ROUTER_PATH)/$(OBJS_TMP),$(BCMWIFI_WIRELESS_ROUTER_PATH)/$(OBJS_TMP),$(WLPPR_WIRELESS_DRIVER_PATH)/$(OBJS_TMP)
endif

OBJS:=$(patsubst %,$(O)%,$(OBJS_TMP))

$(warning MAINOBJ=$(MAINOBJ))
$(warning OBJS=$(OBJS))

ifeq ($(RWL),1)
all: $(O)lib$(PROGRAM_NAME) $(O)$(PROGRAM_NAME)	$(O)lib$(SERVER_SOCKET) $(O)$(SERVER_SOCKET)
else
all: $(O)lib$(PROGRAM_NAME) $(O)$(PROGRAM_NAME)
endif

ifeq ($(RWL),1)
install: $(O)$(PROGRAM_NAME) $(O)$(SERVER_SOCKET)
else
install: $(O)$(PROGRAM_NAME)
endif
	install -m 755 $(O)$(PROGRAM_NAME) $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/$(PROGRAM_NAME)
	if [ -e lib$(PROGRAM_NAME).so ]; then cp -av $(O)lib$(PROGRAM_NAME).so $(LIB_ATP_DIR); cp -av $(O)lib$(PROGRAM_NAME).so $(TARGET_DIR)/lib; fi
	mkdir -p $(INSTALL_DIR)/sbin
	ln -sf $(PROGRAM_NAME) $(INSTALL_DIR)/bin/wl
ifeq ($(RWL),1)
	install -m 755 $(O)$(SERVER_SOCKET) $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/$(SERVER_SOCKET)
	if [ -e $(O)lib$(SERVER_SOCKET).so ]; then cp -av $(O)lib$(SERVER_SOCKET).so $(LIB_ATP_DIR); cp -av $(O)lib$(SERVER_SOCKET).so $(TARGET_DIR)/lib; fi
	mkdir -p $(INSTALL_DIR)/sbin
	ln -sf $(SERVER_SOCKET) $(INSTALL_DIR)/bin/wl_server
endif

dynamic: all install

$(O)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -DPROTOTYPES $(CFLAGS) $(CFLAGS_EXTRA) -c $< -o $@

$(MAINOBJ): $(MAIN).c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

ifeq ($(RWL),1)
$(MAINSERVEROBJ): $(MAIN_SERVER).c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<
endif

ifeq ($(strip $(BUILD_WLCTL_SHLIB)),1) 

$(O)lib$(PROGRAM_NAME): $(OBJS) 
	$(LD) -shared -soname $(O)lib$(PROGRAM_NAME).so -o $(O)lib$(PROGRAM_NAME).so $(OBJS) -L$(LIBDIR) -lc
	@install -m 777 $(O)lib$(PROGRAM_NAME).so $(TARGET_DIR)/lib
	@install -m 777 $(O)lib$(PROGRAM_NAME).so $(STAGING_DIR)/lib

$(O)$(PROGRAM_NAME): $(O)lib$(PROGRAM_NAME) $(MAINOBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -L. -o $(O)$(PROGRAM_NAME) $(MAINOBJ) -l$(PROGRAM_NAME)

ifeq ($(RWL),1)
$(O)lib$(SERVER_SOCKET): $(SOCKET_OBJS)
	$(LD) -shared -soname $(O)lib$(SERVER_SOCKET).so -o $(O)lib$(SERVER_SOCKET).so $(SOCKET_OBJS) -L$(LIBDIR) -lc
	@install -m 777 $(O)lib$(SERVER_SOCKET).so $(TARGET_DIR)/lib
	@install -m 777 $(O)lib$(SERVER_SOCKET).so $(STAGING_DIR)/lib

$(O)$(SERVER_SOCKET): $(O)lib$(SERVER_SOCKET) $(MAINSERVEROBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -L. -o $(O)$(SERVER_SOCKET) $(MAINSERVEROBJ) -l$(SERVER_SOCKET)
endif

else
$(O)$(PROGRAM_NAME): $(MAINOBJ) $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(O)$(PROGRAM_NAME) $(MAINOBJ) $(OBJS)

$(O)lib$(PROGRAM_NAME):

ifeq ($(RWL),1)
$(O)$(SERVER_SOCKET): $(MAINSERVEROBJ) $(SOCKET_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(O)$(SERVER_SOCKET) $(MAINSERVEROBJ) $(SOCKET_OBJS)
$(O)lib$(SERVER_SOCKET):
endif
endif

clean:
	-rm -f $(O)$(PROGRAM_NAME) $(O)*.o  $(O)$(PROGRAM_NAME).a $(O)lib$(PROGRAM_NAME).so $(MAINOBJ)
ifeq ($(RWL),1)
	-rm -f $(O)$(SERVER_SOCKET) *.o  $(O)$(SERVER_SOCKET).a $(O)lib$(SERVER_SOCKET).so $(MAINSERVEROBJ)
endif