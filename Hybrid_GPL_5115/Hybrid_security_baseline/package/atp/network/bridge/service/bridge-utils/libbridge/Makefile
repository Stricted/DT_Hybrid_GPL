
# brcm begin
KERNEL_HEADERS=-I$(KERNEL_DIR)/include
OUTPUT=$(O)libbridge/

#AR=ar
#RANLIB=ranlib

#CC=gcc
#CFLAGS = -Wall -g $(KERNEL_HEADERS)
CFLAGS = -Wall -Os $(KERNEL_HEADERS)
CFLAGS += -I$(TOOLCHAIN)/include
CFLAGS += $(BRCM_WERROR_CFLAGS)
# brcm end

prefix=/usr/local
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

libbridge_SOURCES= \
	libbridge_devif.c \
	libbridge_if.c \
	libbridge_init.c \
	libbridge_misc.c 

libbridge_OBJECTS=$(OUTPUT)libbridge_devif.o $(OUTPUT)libbridge_if.o $(OUTPUT)libbridge_init.o $(OUTPUT)libbridge_misc.o

all:$(OUTPUT)libbridge.a

install: $(OUTPUT)libbridge.a
	mkdir -p $(DESTDIR)$(includedir)
	install -m 644 libbridge.h $(DESTDIR)$(includedir)
	mkdir -p $(DESTDIR)$(libdir)
	install -m 644 $(OUTPUT)libbridge.a $(DESTDIR)$(libdir)


clean:
	rm -f $(OUTPUT)*.o $(OUTPUT)libbridge.a

$(OUTPUT)libbridge.a:$(libbridge_OBJECTS)
	$(AR) rcs $@ $(libbridge_OBJECTS)
	$(RANLIB) $@

$(OUTPUT)%.o: %.c libbridge.h libbridge_private.h
	mkdir -p $(OUTPUT)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OUTPUT)libbridge_compat.o:	libbridge_compat.c if_index.c
	$(CC) $(CFLAGS) -c libbridge_compat.c

