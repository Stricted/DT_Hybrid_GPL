#############################################################
#
# KETH
#
#############################################################

#
# Dependencies
#
KETH_DEPENDENCIES = 

#############################################################
### Version and change logs area
KETH_VERSION = V1.00

define KETH_CHANGELOGS
V1.00:
	1. First release by Pail.

endef
#############################################################

KETH_TYPE=atpkconfig

define KETH_LINUX_CONFIGURE_CMDS

ifneq ($(BRCM_DRIVER_FAP),y)
	$(call ATP_KCONFIG_DISABLE_OPT,CONFIG_BCM_FAP,$$(KERNEL_CONFIG_FILE))
endif

ifneq ($(BUILD_ETH_BCM_EXT_SWITCH_53125),y)
	$(call ATP_KCONFIG_SET_OPT,CONFIG_BCM_EXT_SWITCH,0,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_DISABLE_OPT,BRCM_EXT_SWITCH_REV,$$(KERNEL_CONFIG_FILE))
endif

endef


$(eval $(call ATPTARGETS,package/atp/network/eth/kernel,keth,BUILD_ETH))
