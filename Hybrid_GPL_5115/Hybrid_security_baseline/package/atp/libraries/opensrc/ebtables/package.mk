#############################################################
#
# ebtables
#
#############################################################

#
# Dependencies
#
EBTABLES_DEPENDENCIES = ebtables-extract

#############################################################

EBTABLES_VERSION_STR=$(call qstrip,$(ATP_EBTABLES_VERSION))
EBTABLES_TAR_SRC = $(TOPDIR)/dl/ebtables-$(EBTABLES_VERSION_STR).tar.gz
EBTABLES_AFTER_EXTR_SRC = $(O)/build/ebtables-$(EBTABLES_VERSION_STR)
EBTABLES_TAR_EXTRACT_OPT = tar zxvf

SERVICE_EBTABLES_SRC_DIR = $(EBTABLES_AFTER_EXTR_SRC)/src

### Version and change logs area
EBTABLES_VERSION = V1.00
define IPTABLES_CHANGELOGS
V1.00:
	1. First release by h00163136.

endef
#############################################################

# Target name
EBTABLES_TARGET = dynamic

# Target index
EBTABLES_INDEX = 44

# Target name
EBTABLES_PROGRAM_NAME =ebtables

define EBTABLES_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_EBTABLES_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_EBTABLES_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define EBTABLES_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_EBTABLES_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_EBTABLES_SRC_DIR) clean; \
	fi
endef

define EBTABLES_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_EBTABLES_SRC_DIR)/Makefile ]; then \
	install -m 777 $(SERVICE_EBTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin; \
	install -m 777 $(SERVICE_EBTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(STAGING_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/ebtables,ebtables))
