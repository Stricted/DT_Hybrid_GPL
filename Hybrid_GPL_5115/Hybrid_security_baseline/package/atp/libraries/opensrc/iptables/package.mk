#############################################################
#
# iptables
#
#############################################################

#
# Dependencies
#
IPTABLES_DEPENDENCIES = atptypes iptables-extract

#############################################################

IPTABLES_VERSION_STR=$(call qstrip,$(ATP_IPTABLES_VERSION))
IPTABLES_TAR_SRC = $(TOPDIR)/dl/iptables-$(IPTABLES_VERSION_STR).tar.gz
IPTABLES_AFTER_EXTR_SRC = $(O)/build/iptables-$(IPTABLES_VERSION_STR)
IPTABLES_TAR_EXTRACT_OPT = tar zxvf

SERVICE_IPTABLES_SRC_DIR = $(IPTABLES_AFTER_EXTR_SRC)/src

### Version and change logs area
IPTABLES_VERSION = V1.00
define IPTABLES_CHANGELOGS
V1.00:
	1. First release by h00163136.

endef
#############################################################

define IPTABLES_INSTALL_HEADER_CMDS
	$(Q)if [ -d $(SERVICE_IPTABLES_SRC_DIR)/extensions/ ];then \
	chmod +x $(SERVICE_IPTABLES_SRC_DIR)/extensions/.*;\
	fi
endef

define IPTABLES_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_IPTABLES_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define IPTABLES_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_IPTABLES_SRC_DIR) clean; \
	fi
endef

define IPTABLES_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin; \
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(STAGING_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	fi
endef

# Target name
IPTABLES_TARGET = dynamic


# Target index
IPTABLES_INDEX = 43

# Target name
IPTABLES_PROGRAM_NAME =iptables

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/iptables,iptables))
