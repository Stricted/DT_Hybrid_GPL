#############################################################
#
# strace
#
#############################################################

#
# Dependencies
#
STRACE_DEPENDENCIES = strace_cfg

#############################################################
### Version and change logs area
STRACE_VERSION = V1.01
define STRACE_CHANGELOGS
V1.01:
	1. First release by Handy.
endef
#############################################################

strace_cfg: $(O)/package/atp/libraries/opensrc/strace/.stamp_configured

$(O)/package/atp/libraries/opensrc/strace/.stamp_configured:
	$(Q)cd $(STRACE_DIR)/src; \
	chmod +x configure; \
	$(TARGET_ATP_ENVS) ./configure --host=$(ARCH)-linux
	$(Q)touch $(O)/package/atp/libraries/opensrc/strace/.stamp_configured

# Makefile directory
STRACE_MAKEFILE_DIR = src

# License
STRACE_LICENSE = BSD

# PROGRAME_NAME
STRACE_PROGRAM_NAME =strace



# Target name
STRACE_TARGET = all


define STRACE_BUILD_CMDS
	$(Q)if [ -f package/atp/libraries/opensrc/strace/strace ]; then \
		echo "nothing need to build for strace...."; \
	else \
		$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $$($(2)_SRC_DIR) $$($(2)_TARGET); \
	fi;
endef


define STRACE_INSTALL_TARGET_CMDS
	$(Q)if [ -f package/atp/libraries/opensrc/strace/strace ]; then \
		echo "instal exist strace...."; \
		install -m 777 $$(STRACE_DIR)/strace $(TARGET_DIR)/bin; \
	else \
		install -m 777 $$(STRACE_DIR)/src/strace $(STAGING_DIR)/bin; \
	    install -m 777 $$(STRACE_DIR)/src/strace $(TARGET_DIR)/bin; \
	fi;

endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/strace,strace,BUILD_STRACE))
