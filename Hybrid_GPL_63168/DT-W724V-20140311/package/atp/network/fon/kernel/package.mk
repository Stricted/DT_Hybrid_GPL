#############################################################
#
# KFON
#
#############################################################

#
# Dependencies
#
KFON_DEPENDENCIES = 

#############################################################
### Version and change logs area
KFON_VERSION = V1.00

define KFON_CHANGELOGS
V1.00:
	1. First release by l00214441.

endef
#############################################################

KFON_TYPE=atpkconfig

define KFON_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_TUN,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_L2TP,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_L2TP_DEBUGFS,$$(KERNEL_CONFIG_FILE))	
	$(call ATP_KCONFIG_DISABLE_OPT,CONFIG_L2TP_V3,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_PPPOL2TP,$$(KERNEL_CONFIG_FILE))

endef


$(eval $(call ATPTARGETS,package/atp/network/fon/kernel,kfon,BUILD_FON))
