#############################################################
#
# KIGMP
#
#############################################################

#
# Dependencies
#
KIGMP_DEPENDENCIES = 

#############################################################
### Version and change logs area
KIGMP_VERSION = V2.00

define KIGMP_CHANGELOGS
V1.00:
	1. First release by PJ.
V2.00:
	2.support broadcom IGMP snooping
endef
#############################################################

KIGMP_TYPE=atpkconfig

define KIGMP_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IP_MROUTE,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IP_MULTICAST,$$(KERNEL_CONFIG_FILE))
	
ifeq ($(BUILD_IGMP_SNOOPING),y)
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IGMP_SNOOPING,$$(KERNEL_CONFIG_FILE))
endif

ifeq ($(BUILD_BR_IGMP_SNOOP),y)
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_BR_IGMP_SNOOP,$$(KERNEL_CONFIG_FILE))
endif

endef



$(eval $(call ATPTARGETS,package/atp/network/igmp/kernel,kigmp,BUILD_IGMP))
