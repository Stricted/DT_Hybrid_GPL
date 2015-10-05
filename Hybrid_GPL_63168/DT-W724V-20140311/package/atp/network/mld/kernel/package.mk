#############################################################
#
# KMLD
#
#############################################################

#
# Dependencies
#
KMLD_DEPENDENCIES = 

#############################################################
### Version and change logs area
KMLD_VERSION = V1.00

define KMLD_CHANGELOGS
V1.00:
	1. First release by PJ.

endef
#############################################################

KMLD_TYPE=atpkconfig

define KMLD_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IPV6_MROUTE,$$(KERNEL_CONFIG_FILE))
	
ifeq ($(BUILD_MLD_SNOOPING),y)
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_MLD_SNOOPING,$$(KERNEL_CONFIG_FILE))
endif

ifeq ($(BUILD_BR_MLD_SNOOP),y)
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_BR_MLD_SNOOP,$$(KERNEL_CONFIG_FILE))
endif

endef



$(eval $(call ATPTARGETS,package/atp/network/mld/kernel,kmld,BUILD_MLD))
