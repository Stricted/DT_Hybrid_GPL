################### depend objects
OBJS = inet_pton.o

SUB_DIRS = api gena genlib soap ssdp urlconfig uuid \
		   genlib/client_table \
		   genlib/miniserver \
		   genlib/service_table \
		   genlib/util genlib/net \
		   genlib/net/uri \
		   genlib/net/http

OBJS += $(patsubst %.c, %.o, $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)/*.c)))

######################### compile options
CFLAGS = -I$(TOPDIR)/package/atp/applications/dlna/kernel/upnp/src/inc -Wl,-allow-shlib-undefined -Wno-strict-aliasing -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64

################### Yourself lib(if no, fill nothing)
EXTRA_LIB = ixmlapi

include $(TOPDIR)/package/atp/rule.mak
