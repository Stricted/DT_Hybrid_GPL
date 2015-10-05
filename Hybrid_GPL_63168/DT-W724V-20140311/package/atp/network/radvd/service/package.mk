#############################################################
#
# radvd
#
#############################################################

#
# Dependencies
#
RADVD_DEPENDENCIES = atptypes gplutil radvd-extract

#############################################################

RADVD_TAR_SRC = $(TOPDIR)/dl/radvd-1.4.tar.gz
RADVD_AFTER_EXTR_SRC = $(O)/build/radvd-1.4
RADVD_TAR_EXTRACT_OPT = tar zxvf

SERVICE_RADVD_SRC_DIR = $(RADVD_AFTER_EXTR_SRC)/radvd

### Version and change logs area
RADVD_VERSION = V1.00
define RADVD_CHANGELOGS
V1.00:
	1. First release by x00192803.

endef
#############################################################

# Target name
RADVD_MAKEFILE_DIR = radvd

# Target name
RADVD_TARGET = all

# Target index
RADVD_INDEX = 62

# Target type
RADVD_TYPE = service

# Target license
RADVD_LICENSE =gpl

# Target name
RADVD_PROGRAM_NAME = radvd

define RADVD_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_RADVD_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_RADVD_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define RADVD_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_RADVD_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_RADVD_SRC_DIR) clean; \
	fi
endef

define RADVD_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_RADVD_SRC_DIR)/Makefile ]; then \
	install -m 777 $(SERVICE_RADVD_SRC_DIR)/$(RADVD_PROGRAM_NAME) $(STAGING_DIR)/bin;\
	install -m 777 $(SERVICE_RADVD_SRC_DIR)/$(RADVD_PROGRAM_NAME) $(TARGET_DIR)/bin;\
	$(TARGET_STRIP) $(TARGET_DIR)/bin/$(RADVD_PROGRAM_NAME);\
	$(TARGET_STRIP) $(STAGING_DIR)/bin/$(RADVD_PROGRAM_NAME);\
	fi
endef

$(eval $(call ATPTARGETS,package/atp/network/radvd/service,radvd,BUILD_RADVD))
