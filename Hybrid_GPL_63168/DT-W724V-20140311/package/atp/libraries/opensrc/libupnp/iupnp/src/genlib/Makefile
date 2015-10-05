################### depend objects
OBJS = client_table/client_table.o miniserver/miniserver.o \
	   net/http/httpparser.o net/http/httpreadwrite.o net/http/parsetools.o net/http/statcodes.o \
	   net/http/webserver.o net/uri/uri.o net/sock.o \
	   service_table/service_table.o util/membuffer.o util/strintmap.o util/upnp_timeout.o util/util.o

######################### compile options
CFLAGS = -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64

include $(TOPDIR)/package/atp/rule.mak


