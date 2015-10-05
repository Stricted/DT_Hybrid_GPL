#############################################################
#
# ssl
#
#############################################################

#
# Dependencies
#
SSL_DEPENDENCIES = atptypes

#############################################################
### Version and change logs area
SSL_VERSION = V1.01
define SSL_CHANGELOGS
V1.01:
	1. First release by Handy.
endef
#############################################################

# Makefile directory
SSL_MAKEFILE_DIR = src/polarssl/library
HOST_SSL_MAKEFILE_DIR = src/polarssl/library

# License
SSL_LICENSE = BSD

# PROGRAME_NAME
SSL_PROGRAM_NAME =crypto
HOST_SSL_PROGRAM_NAME =crypto

# Target name
SSL_TARGET = shared
HOST_SSL_TARGET = shared

define SSL_INSTALL_HEADER_CMDS
	$(Q)rm -fr $(STAGING_DIR)/usr/include/openssl
	$(Q)mkdir -p $(STAGING_DIR)/usr/include/openssl
	$(Q)install -m 777 $$(SSL_DIR)/inc/polarssl/openssl/*.h $(STAGING_DIR)/usr/include/openssl
	$(Q)mkdir -p $(HOST_DIR)/usr/include/openssl
	$(Q)install -m 777 $$(SSL_DIR)/inc/polarssl/openssl/*.h $(HOST_DIR)/usr/include/openssl
endef

define SSL_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $$(SSL_BDIR)/lib$(SSL_PROGRAM_NAME).so $(STAGING_DIR)/lib
	$(Q)install -m 777 $$(SSL_BDIR)/lib$(SSL_PROGRAM_NAME).so $(TARGET_DIR)/lib
	$(Q)cd $(TARGET_DIR)/lib; \
	rm -f libssl.so; \
	ln lib$(SSL_PROGRAM_NAME).so -s libssl.so
	$(Q)cd $(STAGING_DIR)/lib; \
	rm -f libssl.so; \
	ln lib$(SSL_PROGRAM_NAME).so -s libssl.so
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/ssl,ssl))
$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/ssl,ssl,BUILD_SSL,host))