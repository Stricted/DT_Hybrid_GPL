################### program name 
PROGRAM_NAME = crypto

CFLAGS	+= -D_FILE_OFFSET_BITS=64

################### depend objects
OBJS = aes.o		arc4.o		base64.o	\
	bignum.o	certs.o		debug.o		\
	des.o		dhm.o		havege.o	\
	md2.o		md4.o		md5.o		\
	net.o		padlock.o	rsa.o		\
	sha1.o		sha2.o		sha4.o		\
	ssl_cli.o	ssl_srv.o	ssl_tls.o	\
	timing.o	x509parse.o	xtea.o		\
	camellia.o

all: install_headers static shared
	@cd $(STAGING_DIR)/lib; \
	ln -sf libcrypto.a libssl.a; \
	ln -sf libcrypto.so libssl.so

install_headers:
	@rm -fr $(STAGING_DIR)/usr/include/openssl
	@mkdir -p $(STAGING_DIR)/usr/include/openssl
	@install -m 777 ../../../inc/polarssl/openssl/*.h $(STAGING_DIR)/usr/include/openssl

include $(TOPDIR)/package/atp/rule.mak
