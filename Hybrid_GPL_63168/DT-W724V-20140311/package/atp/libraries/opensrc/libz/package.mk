#############################################################
#
# ZLIB
#
#############################################################


#############################################################
### Version and change logs area
ZLIB_VERSION = V1.01
define ZLIB_CHANGELOGS
V1.01:
	1. First release by Handy.
endef
#############################################################

ZLIB_DEPENDENCIES = zlib-extract libz-cfg

ZLIB_TAR_SRC = $(TOPDIR)/dl/libz-1.2.3.tar.gz
ZLIB_AFTER_EXTR_SRC = $(O)/build/zlib-1.2.3
ZLIB_TAR_EXTRACT_OPT = tar zxvf

SERVICE_ZLIB_SRC_DIR = $(ZLIB_AFTER_EXTR_SRC)/src

# Makefile directory
ZLIB_MAKEFILE_DIR = src

ZLIB_PROGRAM_NAME=libz.so

# License
ZLIB_LICENSE=GPL

# Target name
ZLIB_TARGET=dynamic


$(O)/package/atp/libraries/opensrc/libz/.stamp_configured:
	$(Q)if [ -f $(SERVICE_ZLIB_SRC_DIR)/zlib.h ]; then \
	cp $(SERVICE_ZLIB_SRC_DIR)/zlib.h $(SERVICE_ZLIB_SRC_DIR)/zconf.h $(STAGING_DIR)/usr/include;\
	cp $(SERVICE_ZLIB_SRC_DIR)/zlib.h $(SERVICE_ZLIB_SRC_DIR)/zconf.h $(HOST_DIR)/usr/include;\
	fi
	@touch $(O)/package/atp/libraries/opensrc/libz/.stamp_configured

libz-cfg: $(O)/package/atp/libraries/opensrc/libz/.stamp_configured

define ZLIB_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_ZLIB_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_ZLIB_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define ZLIB_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_ZLIB_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_ZLIB_SRC_DIR) clean; \
	fi
endef

# Install libz.so
define ZLIB_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(SERVICE_ZLIB_SRC_DIR)/$(ZLIB_PROGRAM_NAME) $(TARGET_DIR)/lib;\
	install -m 777 $(SERVICE_ZLIB_SRC_DIR)/$(ZLIB_PROGRAM_NAME) $(STAGING_DIR)/lib;\
	$(TARGET_STRIP) $(TARGET_DIR)/lib/$(ZLIB_PROGRAM_NAME);\
	cd $(TARGET_DIR)/lib; \
	rm -f libz.so.1; \
	ln libz.so -s libz.so.1
	cd $(STAGING_DIR)/lib; \
	rm -f libz.so.1; \
	ln libz.so -s libz.so.1
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libz,zlib,BUILD_LIBZ))
