#############################################################
#
# libiconv
#
#############################################################
#
# Dependencies
#
ICONV_DEPENDENCIES = atptypes iconv-extract libiconv_cfg

ICONV_TAR_SRC = $(TOPDIR)/dl/libiconv-1.13.1.tar.gz
ICONV_AFTER_EXTR_SRC = $(O)/build/libiconv-1.13.1
ICONV_TAR_EXTRACT_OPT = tar zxvf

SERVICE_ICONV_SRC_DIR = $(ICONV_AFTER_EXTR_SRC)/src

ICONV_VERSION = 1.01
define ICONV_CHANGELOGS
V1.00:
	1. First release by <<ccmm>>.
endef

# Program name
ICONV_PROGRAM_NAME=iconv

# Target name
ICONV_TARGET=shared

ICONV_TYPE=opensrclib

ICONV_MAKEFILE_DIR=src

ICONV_INDEX=19

ICONV_LICENSE="LGPL"

$(O)/package/atp/libraries/opensrc/libiconv/.stamp_configured:
	$(Q)if [ -f $(SERVICE_ICONV_SRC_DIR)/configure ]; then \
		cd $(SERVICE_ICONV_SRC_DIR); \
		make distclean;\
		chmod 777 configure;\
		$(TARGET_ATP_ENVS) ./configure --host=$(ARCH)-linux --disable-debug; \
		fi
	@touch $(O)/package/atp/libraries/opensrc/libiconv/.stamp_configured

libiconv_cfg: $(O)/package/atp/libraries/opensrc/libiconv/.stamp_configured

# Install lib
define ICONV_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(SERVICE_ICONV_SRC_DIR)/lib/.libs/libiconv.so.2.5.0 $(STAGING_DIR)/lib;\
	install -m 777 $(STAGING_DIR)/lib/libiconv.so.2.5.0 $(TARGET_DIR)/lib; \
	$(TARGET_STRIP) $(TARGET_DIR)/lib/libiconv.so.2.5.0;\
	cd $(TARGET_DIR)/lib; rm -f libiconv.so; \
	rm -f libiconv.so.2; \
	ln -s libiconv.so.2.5.0 libiconv.so.2; \
	ln -s libiconv.so.2.5.0 libiconv.so;
	$(Q)cd $(STAGING_DIR)/lib; rm -f libiconv.so; \
	rm -f libiconv.so.2; \
	ln -s libiconv.so.2.5.0 libiconv.so.2; \
	ln -s libiconv.so.2.5.0 libiconv.so;
endef

define ICONV_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_ICONV_SRC_DIR)/Makefile ]; then \
		$(MAKE) -C $(SERVICE_ICONV_SRC_DIR); \
	fi
endef

define ICONV_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_ICONV_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_ICONV_SRC_DIR) clean; \
	fi
endef

define ICONV_BRANCH_BINARY_CMDS
	$(Q)if [ -f $(SERVICE_ICONV_SRC_DIR)/Makefile ]; then \
	cp $(SERVICE_ICONV_SRC_DIR)/lib/.libs/libiconv.so.2.5.0 $(ICONV_AFTER_EXTR_SRC); \
	cd $(SERVICE_ICONV_SRC_DIR); \
	rm -fr *; \
	fi
	$(Q)if [ -f $(ICONV_AFTER_EXTR_SRC)/libiconv.so.2.5.0 ]; then \
	mkdir -p $(SERVICE_ICONV_SRC_DIR)/lib/.libs;\
	mv $(ICONV_AFTER_EXTR_SRC)/libiconv.so.2.5.0 $(SERVICE_ICONV_SRC_DIR)/lib/.libs/libiconv.so.2.5.0; \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libiconv,iconv,BUILD_LIBICONV))
