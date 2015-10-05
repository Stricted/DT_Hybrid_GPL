#############################################################
#
# KNAT
#
#############################################################

#
# Dependencies
#
KNAT_DEPENDENCIES = 

#############################################################
### Version and change logs area
KNAT_VERSION = V1.00

define KNAT_CHANGELOGS
V1.00:
	1. First release by FENG.

endef
#############################################################

KNAT_TYPE=atpkconfig

define KNAT_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_NF_CONNTRACK_MARK,$$(KERNEL_CONFIG_FILE))

ifeq ($(BUILD_CONENAT),y)
	$(call ATP_KCONFIG_DISABLE_OPT,CONFIG_IP_NF_TARGET_ALLCONENAT,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IP_NF_TARGET_CONENAT,$$(KERNEL_CONFIG_FILE))
endif

endef


$(eval $(call ATPTARGETS,package/atp/network/nat/kernel,knat,BUILD_NAT))
