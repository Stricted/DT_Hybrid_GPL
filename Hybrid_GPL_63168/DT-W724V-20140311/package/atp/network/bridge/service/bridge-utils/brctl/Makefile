
# brcm begin
KERNEL_HEADERS=-I$(KERNEL_DIR)/include
OUTPUT=$(O)brctl/

#CC=gcc
#CFLAGS= -Wall -g -O2
CFLAGS= -Wall -s
ifeq ($(strip $(BUILD_BRCTL)),static)
CFLAGS += -DBUILD_STATIC
endif

# brcm end
LDFLAGS=
INCLUDE=-I../libbridge $(KERNEL_HEADERS)
LIBS= -L $(OUTPUT)../libbridge -lbridge

prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/share/man

INSTALL=/usr/bin/install -c


common_SOURCES= brctl_cmd.c brctl_disp.c
brctl_SOURCES=  brctl.c $(common_SOURCES)

common_OBJECTS= $(OUTPUT)brctl_cmd.o $(OUTPUT)brctl_disp.o
brctl_OBJECTS= $(OUTPUT)brctl.o $(OUTPUT)brctl_cmd.o $(OUTPUT)brctl_disp.o

OBJECTS= $(common_OBJECTS) $(brctl_OBJECTS)

PROGRAMS= $(OUTPUT)brctl


all:	$(PROGRAMS)
# brcm befing
dynamic: $(PROGRAMS)

static: $(brctl_OBJECTS) $(OUTPUT)../libbridge/libbridge.a
	$(AR) rcs $(OUTPUT)brctl.a $(brctl_OBJECTS) $(LIBS)
# brcm end

install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(sbindir)

$(OUTPUT)brctl:	$(brctl_OBJECTS) $(OUTPUT)../libbridge/libbridge.a
	$(CC) $(LDFLAGS) $(brctl_OBJECTS) $(LIBS) -o $(OUTPUT)brctl

$(OUTPUT)%.o: %.c brctl.h
	mkdir -p $(OUTPUT)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f *.o brctl core
	$(MAKE) $(MFLAGS) -C ../libbridge clean

$(OUTPUT)../libbridge/libbridge.a:
	mkdir -p $(OUTPUT)../libbridge
	$(MAKE) $(MFLAGS) -C ../libbridge
