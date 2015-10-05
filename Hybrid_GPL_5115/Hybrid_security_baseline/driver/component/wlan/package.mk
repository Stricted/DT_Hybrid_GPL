#############################################################
#
# wlan component
#
#############################################################

#
# Dependencies
#
WLANCOM_DEPENDENCIES = 

#############################################################
### Version and change logs area
WLANCOM_VERSION = V1.01
define WLANCOM_CHANGELOGS
V1.00:
	1. First release by sunbo.

endef
#############################################################

WLANCOM_LICENSE="Huawei private"

define WLANCOM_INSTALL_TARGET_CMDS
	$(Q)mkdir -p $(TARGET_DIR)/lib/extra;\
	install -m 0777 $$(WLANCOM_DIR)/$(BUILD_COMPONENT_WLAN_CHIP)/$(CHIP_PRODUCT_NAME)/lib/* $(TARGET_DIR)/lib/extra;\
	install -m 0777 $$(WLANCOM_DIR)/$(BUILD_COMPONENT_WLAN_CHIP)/$(CHIP_PRODUCT_NAME)/bin/* $(TARGET_DIR)/bin;\
	install -m 0777 $$(WLANCOM_DIR)/$(BUILD_COMPONENT_WLAN_CHIP)/$(CHIP_PRODUCT_NAME)/etc/* $(TARGET_DIR)/etc;\
	$(if $(FACTORY_BUILD), mv $(TARGET_DIR)/lib/extra/wl_debug.ko $(TARGET_DIR)/lib/extra/wl.ko,mv $(TARGET_DIR)/lib/extra/wl_rel.ko $(TARGET_DIR)/lib/extra/wl.ko);\
	$(if $(FACTORY_BUILD), rm -f $(TARGET_DIR)/lib/extra/wl_rel.ko,rm -f $(TARGET_DIR)/lib/extra/wl_debug.ko)
endef

$(eval $(call ATPTARGETS,driver/component/wlan,wlancom,BUILD_COMPONENT_WLAN))
