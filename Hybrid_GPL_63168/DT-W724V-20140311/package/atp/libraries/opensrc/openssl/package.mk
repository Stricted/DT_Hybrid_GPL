#############################################################
#
# openssl
#
#############################################################
#
# Dependencies
#
OPENSSL_DEPENDENCIES = openssl-extract

OPENSSL_TAR_SRC = $(TOPDIR)/dl/openssl-1.0.1j.tar.gz
OPENSSL_AFTER_EXTR_SRC = $(TOPDIR)/output/build/openssl-1.0.1
OPENSSL_TAR_EXTRACT_OPT = tar zxvf

OPENSSL_VERSION = 1.00

define OPENSSL_CHANGELOGS
V1.00:
	1. port from BT code base.
endef

# Program name
OPENSSL_PROGRAM_NAME=openssl

# Target name
OPENSSL_TARGET=shared

OPENSSL_TYPE=opensrclib

OPENSSL_SRC_DIR=src

OPENSSL_INDEX=195

OPENSSL_LICENSE="OpenSSL"

# Install header
define OPENSSL_INSTALL_HEADER_CMDS
	mkdir -p $($(2)_AFTER_EXTR_SRC); \
	cd $($(2)_AFTER_EXTR_SRC); \
	$($(2)_TAR_EXTRACT_OPT) $($(2)_TAR_SRC); \
	$(Q)touch $$($(2)_BDIR)/.stamp_extracted; \
	rm -fr $(STAGING_DIR)/usr/include/openssl-1.0.0; mkdir $(STAGING_DIR)/usr/include/openssl-1.0.0; \
	cp $(OPENSSL_AFTER_EXTR_SRC)/src/include/openssl/*.h $(STAGING_DIR)/usr/include/openssl-1.0.0; \
	rm -fr $(HOST_DIR)/usr/include/openssl-1.0.0; mkdir $(HOST_DIR)/usr/include/openssl-1.0.0; \
	cp $(OPENSSL_AFTER_EXTR_SRC)/src/include/openssl/*.h $(HOST_DIR)/usr/include/openssl-1.0.0; \
	sed -i 's/#include\ <openssl/#include\ <openssl-1.0.0/' `find $(STAGING_DIR)/usr/include/openssl-1.0.0 -name *.h`;\
	sed -i 's/#\ include\ <openssl/#include\ <openssl-1.0.0/' `find $(STAGING_DIR)/usr/include/openssl-1.0.0 -name *.h`;
endef

# Install lib
define OPENSSL_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(OPENSSL_AFTER_EXTR_SRC)/src/libssl.so.1.0.0 $(STAGING_DIR)/lib;\
	install -m 777 $(OPENSSL_AFTER_EXTR_SRC)/src/libcrypto.so.1.0.0 $(STAGING_DIR)/lib;\
	install -m 777 $(STAGING_DIR)/lib/libcrypto.so.1.0.0 $(TARGET_DIR)/lib; \
	install -m 777 $(STAGING_DIR)/lib/libssl.so.1.0.0 $(TARGET_DIR)/lib; \
	$(TARGET_STRIP) $(TARGET_DIR)/lib/libcrypto.so.1.0.0;\
	$(TARGET_STRIP) $(TARGET_DIR)/lib/libssl.so.1.0.0;\
	mkdir $(OPENSSL_AFTER_EXTR_SRC)/src/lib; \
	install -m 777 $(OPENSSL_AFTER_EXTR_SRC)/src/libssl.so.1.0.0 $(OPENSSL_AFTER_EXTR_SRC)/src/lib/libssl_openssl.so; \
	install -m 777 $(OPENSSL_AFTER_EXTR_SRC)/src/libcrypto.so.1.0.0 $(OPENSSL_AFTER_EXTR_SRC)/src/lib/libcrypto_openssl.so; \
	cd $(TARGET_DIR)/lib; rm -f libcrypto_openssl.so; \
	rm -f libssl_openssl.so; \
	ln -s libcrypto.so.1.0.0 libcrypto_openssl.so; \
	ln -s libssl.so.1.0.0 libssl_openssl.so;
	$(Q)cd $(STAGING_DIR)/lib; rm -f libcrypto_openssl.so; \
	rm -f libssl_openssl.so; \
	ln -s libcrypto.so.1.0.0 libcrypto_openssl.so; \
	ln -s libssl.so.1.0.0 libssl_openssl.so;
endef

define OPENSSL_BUILD_CMDS
	$(Q)if [ -f $(OPENSSL_AFTER_EXTR_SRC)/src/Makefile ]; then \
		$(TARGET_ATP_ENVS) $(MAKE1) -C $(OPENSSL_AFTER_EXTR_SRC)/src build_libs BUILD_ENDIAN=$(BR2_ENDIAN); \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/openssl,openssl,BUILD_OPENSSL_O))
