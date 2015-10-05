#############################################################
#
# multinas
#
#############################################################

#
# Dependencies
#
MULTINAS_DEPENDENCIES = 

#############################################################
### Version and change logs area
MULTINAS_VERSION = V1.01
define MULTINAS_CHANGELOGS
V1.00:
	1. First release by sunbo.

endef
#############################################################
ifneq ($(strip $(BROADCOM_CHIP)),)
MULTINAS_MAKEFILE_DIR=multinas
else
ifneq ($(strip $(QUALCOMM_CHIP)),)
MULTINAS_MAKEFILE_DIR=smux
endif
endif

MULTINAS_LICENSE="Huawei private"

MULTINAS_TYPE=kernel

define MULTINAS_BUILD_CMDS
	$(Q)echo ""
endef

define MULTINAS_INSTALL_TARGET_CMDS
	$(Q)echo ""
endef

ifneq ($(strip $(QUALCOMM_CHIP)),)
define MULTINAS_INSTALL_HEADER_CMDS
	install -m 770 $(TOPDIR)/package/atp/network/wan/kernel/inc/*.h $(STAGING_DIR)/usr/include/
endef
endif

ifneq ($(strip $(BROADCOM_CHIP)),)
$(eval $(call ATPTARGETS,package/atp/network/wan/kernel,multinas,BROADCOM_CHIP))
else
ifneq ($(strip $(QUALCOMM_CHIP)),)
$(eval $(call ATPTARGETS,package/atp/network/wan/kernel,multinas,QUALCOMM_CHIP))
endif
endif
