#############################################################
#
# BCM driver
#
#############################################################

#
# Dependencies
#
BCMCHIP_DEPENDENCIES = 

#############################################################
### Version and change logs area
BCMCHIP_VERSION = V1.01
define BCMCHIP_CHANGELOGS
V1.00:
	1. First release by sunbo.

endef
#############################################################

ifneq ($(BUILD_HYBRID),)
define HYBRID_TFTPD_CMD
	install -m 770	$$(BCMCHIP_DIR)/profile_hybrid $(TARGET_DIR)/etc/profile
endef
else
define HYBRID_TFTPD_CMD
	echo ""
endef
endif

BCMCHIP_LICENSE="Huawei private"

BCMCHIP_TYPE=kernel

BCMCHIP_MAKEFILE_DIR=$(CHIP_NAME)/$(DRIVER_RELEASE_VERSION)

define BCMCHIP_BUILD_CMDS
	$(Q)echo ""
endef

define BCMCHIP_INSTALL_TARGET_CMDS
	install -m 770 	$$(BCMCHIP_DIR)/device_table_bcm.dynamic $(DEVICETABLE_DIR)
	install -m 770	$$(BCMCHIP_DIR)/startbsp $(TARGET_DIR)/bin/startbsp
	$(HYBRID_TFTPD_CMD)
endef

define BCMCHIP_INSTALL_HEADER_CMDS
	install -m 770 $(TOPDIR)/$$(BCMCHIP_DIR)/$(BCMCHIP_MAKEFILE_DIR)/include/*.h $(STAGING_DIR)/usr/include
	mkdir -p $(STAGING_DIR)/usr/include/bcm
	install -m 770 $(TOPDIR)/$$(BCMCHIP_DIR)/$(BCMCHIP_MAKEFILE_DIR)/include/bcm/*.h $(STAGING_DIR)/usr/include/bcm
endef


$(eval $(call ATPTARGETS,driver/broadcom,bcmchip,BROADCOM_CHIP))
