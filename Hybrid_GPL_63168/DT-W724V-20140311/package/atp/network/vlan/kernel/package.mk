
#############################################################
#
# KVLAN
#
#############################################################
 
 
# Dependencies
KVLAN_DEPENDENCIES=
#############################################################
### Version and change logs area
KVLAN_VERSION=1.0
define KVLAN_CHANGELOGS
 created by pail
endef
#############################################################
KVLAN_TYPE=atpkconfig

KVLAN_INDEX=243
 
define KVLAN_LINUX_CONFIGURE_CMDS
#	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_BCM_VLAN,$(KERNEL_CONFIG_FILE))
endef

$(eval $(call ATPTARGETS,package/atp/network/vlan/kernel,kvlan,BUILD_BCM_VLAN))