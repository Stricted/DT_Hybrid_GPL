#############################################################
#
# KCMSMONITOR
#
#############################################################

#
# Dependencies
#
KCMSMONITOR_DEPENDENCIES = 

#############################################################
### Version and change logs area
KCMSMONITOR_VERSION = V1.00

define KCMSMONITOR_CHANGELOGS
V1.00:
	1. First release by l00135113.

endef
#############################################################

KCMSMONITOR_TYPE=

define KCMSMONITOR_BUILD_CMDS
	@echo ""
endef

define KCMSMONITOR_INSTALL_TARGET_CMDS
	@echo ""
endef
KCMSMAIN_INDEX =165
define KCMSMONITOR_CLEAN_CMDS
	rm -f $(TOPDIR)/$$(KCMSMONITOR_DIR)/*.o $(TOPDIR)/$$(KCMSMONITOR_DIR)/.*.cmd $(TOPDIR)/$$(KCMSMONITOR_DIR)/*.order
endef

$(eval $(call ATPTARGETS,package/atp/configuration/cmsmain/kernel,kcmsmonitor,required))