
# Also see "include/polarssl/config.h"

CFLAGS	= -D_FILE_OFFSET_BITS=64 -I$(INC_APP_DIR) -I$(OPENSSL_INC_DIR) -DHGW_SOFTVERSION=\"$(HGW_SOFTVERSION)\"
OFLAGS	= -O

ifeq ($(strip $(VERSION_TYPE)), ATP_DEBUG)
CFLAGS += -g
endif

# MicroBlaze specific options:
# CFLAGS += -mno-xl-soft-mul -mxl-barrel-shift

# To compile on Plan9:
# CFLAGS += -D_BSD_EXTENSION

# To compile as a shared library:
# CFLAGS += -fPIC

DLEXT=so
# OSX shared library extension:
# DLEXT=dylib

OBJS=	aes.o		arc4.o		base64.o	\
	bignum.o	certs.o		debug.o		\
	des.o		dhm.o		havege.o	\
	md2.o		md4.o		md5.o		\
	net.o		padlock.o	rsa.o		\
	sha1.o		sha2.o		sha4.o		\
	ssl_cli.o	ssl_srv.o	ssl_tls.o	\
	timing.o	x509parse.o	xtea.o		\
	camellia.o

.SILENT:

all: static

static: libcrypto.a

shared: libcrypto.$(DLEXT)

dynamic: shared

libcrypto.a: $(OBJS)
	echo "  AR    $@"
	ar r $@ $(OBJS)
	echo "  RL    $@"
	ranlib $@
	install -m 777 $@ $(LIB_ATP_DIR)
	rm -f $(LIB_ATP_DIR)/libssl.a
	cd $(LIB_ATP_DIR); \
	ln -s libcrypto.a libssl.a

libcrypto.so: libcrypto.a
	echo "  LD    $@"
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS)
	install -m 777 $@ $(LIB_ATP_DIR)
	rm -f $(LIB_ATP_DIR)/libssl.so
	cd $(LIB_ATP_DIR); \
	ln -s libcrypto.so libssl.so

libcrypto.dylib: libpolarcrypto.a
	echo "  LD    $@"
	$(CC) -dynamiclib -o $@ $(OBJS)

.c.o:
	echo "  CC    $<"
	$(CC) $(CFLAGS) $(OFLAGS) -c $<

clean:
	rm -f *.o libcrypto.*

