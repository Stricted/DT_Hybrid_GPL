#############################################################
#
# IPROUTE2
#
#############################################################

#
# Dependencies
#
IPROUTE2_DEPENDENCIES = atptypes

#############################################################

IPROUTE2_DEPENDENCIES = iproute2-extract
ifeq ($(BR2_LINUX_KERNEL_VERSION),"3.4.11")
IPROUTE2_TAR_SRC = $(TOPDIR)/dl/iproute2-hybrid.tar.gz
IPROUTE2_AFTER_EXTR_SRC = $(O)/build/iproute2-hybrid
else
IPROUTE2_TAR_SRC = $(TOPDIR)/dl/iproute2.tgz
IPROUTE2_AFTER_EXTR_SRC = $(O)/build/iproute2
endif
IPROUTE2_TAR_EXTRACT_OPT = tar zxvf

SERVICE_IPROUTE2_SRC_DIR = $(IPROUTE2_AFTER_EXTR_SRC)/src

### Version and change logs area
IPROUTE2_VERSION = V1.00
define IPROUTE2_CHANGELOGS
V1.00:
	1. First release by f00120964.

endef
#############################################################


# Target name
IPROUTE2_TARGET = all

# Target index
IPROUTE2_INDEX = 43

# Target name
IPROUTE2_PROGRAM_NAME =iproute2

# License
IPROUTE2_LICENSE = "GPLv2"
define IPROUTE2_INSTALL_TARGET_CMDS

	$(if $(BUILD_IPROUTE2_TC),
		install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/tc/tc $(TARGET_DIR)/bin;\
		$(TARGET_STRIP) $(TARGET_DIR)/bin/tc,)
	install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/ip/ip $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/ip;\
	install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/misc/ss $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/ss; \
	install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/misc/nstat $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/nstat;\
	install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/misc/ifstat $(TARGET_DIR)/bin;\
	$(TARGET_STRIP) $(TARGET_DIR)/bin/ifstat;\
	install -m 0755 $(SERVICE_IPROUTE2_SRC_DIR)/misc/rtacct $(TARGET_DIR)/bin;\
	$(TARGET_STRIP) $(TARGET_DIR)/bin/rtacct
endef

define IPROUTE2_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_IPROUTE2_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_IPROUTE2_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define IPROUTE2_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_IPROUTE2_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_IPROUTE2_SRC_DIR) clean; \
	fi
endef


$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/iproute2,iproute2))
