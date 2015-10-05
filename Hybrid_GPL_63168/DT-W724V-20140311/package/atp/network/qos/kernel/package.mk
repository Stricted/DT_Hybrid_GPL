#############################################################
#
# KQOS
#
#############################################################

#
# Dependencies
#
KQOS_DEPENDENCIES = 

#############################################################
### Version and change logs area
KQOS_VERSION = V1.00

define KQOS_CHANGELOGS
V1.00:
	1. First release by FENG.

endef
#############################################################

KQOS_TYPE=atpkconfig

define KQOS_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IMQ,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IMQ_BEHAVIOR_AB,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_SET_OPT,CONFIG_IMQ_NUM_DEVS,16,$$(KERNEL_CONFIG_FILE))

endef


$(eval $(call ATPTARGETS,package/atp/network/qos/kernel,kqos,BUILD_QUEUE))
