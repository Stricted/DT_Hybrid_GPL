#############################################################
#
# reorder
#
#############################################################

#
# Dependencies
#
REORDER_DEPENDENCIES = 

#############################################################
### Version and change logs area
REORDER_VERSION = V1.01
define REORDER_CHANGELOGS
V1.00:
	1. First release by fangjing.

endef
#############################################################

REORDER_MAKEFILE_DIR=reorder

REORDER_LICENSE="Huawei private"

REORDER_TYPE=kernel

REORDERBAK_TYPE=atpkconfig

define REORDER_INSTALL_HEADER_CMDS
	install -m 770 $(TOPDIR)/package/atp/network/hybrid/kernel/inc/*.h $(STAGING_DIR)/usr/include/
endef

define REORDERBAK_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_NET_IPGRE_DEMUX,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_NET_IPGRE,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_IPV6_GRE,$$(KERNEL_CONFIG_FILE))
endef

define REORDER_BUILD_CMDS
	@echo ""
endef

define REORDER_INSTALL_TARGET_CMDS
	@echo ""
endef

define REORDER_CLEAN_CMDS
	@echo ""
endef

$(eval $(call ATPTARGETS,package/atp/network/hybrid/kernel,reorder,ATP_HYBRID_REORDER))
