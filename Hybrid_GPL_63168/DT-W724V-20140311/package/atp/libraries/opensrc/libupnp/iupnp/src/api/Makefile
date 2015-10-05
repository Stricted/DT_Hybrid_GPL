################### depend objects
OBJS = upnpapi.o upnptools.o UpnpString.o

ifeq ($(strip $(ver)),debug)
OBJS += upnpdebug.o
endif

######################### compile options
CFLAGS = -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64

include $(TOPDIR)/package/atp/rule.mak


