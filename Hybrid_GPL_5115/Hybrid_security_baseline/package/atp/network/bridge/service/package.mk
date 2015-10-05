#############################################################
#
# bridge-utils
#
#############################################################

#
# Dependencies
#
BRCTL_DEPENDENCIES = atptypes

#############################################################
### Version and change logs area
BRCTL_VERSION = V1.00
define BRCTL_CHANGELOGS
V1.00:
	1. First release by pail.

endef
#############################################################

# Target name
BRCTL_MAKEFILE_DIR = brctl

# Target name
BRCTL_TARGET = all

# Target index
BRCTL_INDEX = 36

# Target type
BRCTL_TYPE = service

# Target license
BRCTL_LICENSE =GPL

# Target name
BRCTL_PROGRAM_NAME = brctl

define BRCTL_BUILD_CMDS
	$(Q)if [ -f $$($(2)_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $$($(2)_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define BRCTL_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $$($(2)_BDIR)/brctl/$$(BRCTL_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin
	$(Q)install -m 777 $$($(2)_BDIR)/brctl/$$(BRCTL_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin
	$(Q)$(TARGET_STRIP) $(STAGING_DIR)/bin/$$(BRCTL_FULL_PROGRAM_NAME)
	$(Q)$(TARGET_STRIP) $(TARGET_DIR)/bin/$$(BRCTL_FULL_PROGRAM_NAME)
endef

define BRCTL_CLEAN_CMDS
	$(Q)if [ -f $$($(2)_SRC_DIR)/Makefile ]; then \
	rm -frv $$($(2)_BDIR)/*;\
	fi
endef

$(eval $(call ATPTARGETS,package/atp/network/bridge/service/bridge-utils,brctl,BUILD_BRIDGE))
