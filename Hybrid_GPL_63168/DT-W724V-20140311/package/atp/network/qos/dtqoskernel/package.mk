#############################################################
#
# KDTQOS
#
#############################################################

#
# Dependencies
#
KDTQOS_DEPENDENCIES = 

#############################################################
### Version and change logs area
KDTQOS_VERSION = V1.00

define KDTQOS_CHANGELOGS
V1.00:
	1. First release by FENG.

endef
#############################################################

KDTQOS_TYPE=kernel

define KDTQOS_INSTALL_HEADER_CMDS
	install -m 770 $(TOPDIR)/package/atp/network/qos/dtqoskernel/inc/*.h $(STAGING_DIR)/usr/include/
endef

define KDTQOS_BUILD_CMDS
	@echo ""
endef

define KDTQOS_INSTALL_TARGET_CMDS
	@echo ""
endef

define KDTQOS_CLEAN_CMDS
	@echo ""
endef

$(eval $(call ATPTARGETS,package/atp/network/qos/dtqoskernel,kdtqos,DT_QOS))
