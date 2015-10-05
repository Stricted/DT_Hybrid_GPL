#############################################################
#
# KBRIDGE
#
#############################################################

#
# Dependencies
#
KBRIDGE_DEPENDENCIES = 

#############################################################
### Version and change logs area
KBRIDGE_VERSION = V1.00

define KBRIDGE_CHANGELOGS
V1.00:
	1. First release by Pail.

endef
#############################################################

KBRIDGE_TYPE=atpkconfig

define KBRIDGE_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_BRIDGE,$$(KERNEL_CONFIG_FILE))
endef


$(eval $(call ATPTARGETS,package/atp/network/bridge/kernel,kbridge,BUILD_BRIDGE))
