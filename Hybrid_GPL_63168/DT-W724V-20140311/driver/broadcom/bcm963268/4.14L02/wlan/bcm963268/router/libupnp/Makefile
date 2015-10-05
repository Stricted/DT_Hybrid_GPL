# libnupnp makefile

BUILD_DIR=$(word 1, $(subst /bcmdrivers, /bcmdrivers,$(shell pwd)))
#include $(BUILD_DIR)/make.common

CFLAGS += -Iinclude
CFLAGS += -I../../include
CFLAGS += -I../../include/bcmcrypto
CFLAGS += -I../../shared
CFLAGS += -I../../router/shared
CFLAGS += -DDSLCPE
CFLAGS += -Wall
CFLAGS += -DNDEBUG

#CFLAGS += -g -DDEBUG -UNDEBUG -DBCMDBG

SRCFILES = upnp.c upnp_ssdp.c upnp_http.c upnp_gena.c upnp_soap.c \
	upnp_description.c upnp_device.c upnp_util.c upnp_msg.c \
	upnp_linux_osl.c

OBJS = $(foreach x, $(SRCFILES), $(shell find . -name "$x" -exec echo $(x:.c=.o) ";"))

$(warning $(OBJS))

vpath %.c ./upnp
vpath %.c ./linux

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

wlupnp: $(OBJS)
ifneq ($(strip $(OBJS)),)
	$(CC)  -shared -Os -o libwlupnp.so $^
	cp -f libwlupnp.so libwlupnp-$(EXT_CPU_ARCH_NAME).so
else
	cp -f libwlupnp-$(EXT_CPU_ARCH_NAME).so libwlupnp.so
endif

install: wlupnp
	install -m 755 libwlupnp.so $(LIB_ATP_DIR)/libwlupnp.so
	install -m 755 libwlupnp.so $(TARGET_DIR)/lib/libwlupnp.so

clean:
	rm -f $(OBJS)
ifneq ($(strip $(OBJS)),)
	rm -f libwlupnp.so
endif

dynamic: install