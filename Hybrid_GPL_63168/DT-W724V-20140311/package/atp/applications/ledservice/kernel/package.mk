#############################################################
#
# ledservicekernel
#
#############################################################

#
# Dependencies
#
LEDSERVICEKERNEL_DEPENDENCIES = 

#############################################################
### Version and change logs area
LEDSERVICEKERNEL_VERSION = V1.01
define LEDSERVICEKERNEL_CHANGELOGS
V1.00:
	1. First release by jiangchuan.

endef
#############################################################
#LEDSERVICEKERNEL_MAKEFILE_DIR=$(call qstrip,$(ATP_LEDSERVICE_PATH))
LEDSERVICEKERNEL_MAKEFILE_DIR=$(ATP_LEDSERVICE_PATH)

LEDSERVICEKERNEL_LICENSE="Huawei private"

LEDSERVICEKERNEL_TYPE=kernel

define LEDSERVICEKERNEL_BUILD_CMDS
	$(Q)echo ""
endef

define LEDSERVICEKERNEL_INSTALL_TARGET_CMDS
	$(Q)echo ""
endef

$(eval $(call ATPTARGETS,package/atp/applications/ledservice/kernel,ledservicekernel,BUILD_LEDSERVICE))
