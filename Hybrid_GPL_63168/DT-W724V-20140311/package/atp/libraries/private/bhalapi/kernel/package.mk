#############################################################
#
# BHAL
#
#############################################################

#
# Dependencies
#
BHAL_DEPENDENCIES = 

#############################################################
### Version and change logs area
BHAL_VERSION = V1.00

define BHAL_CHANGELOGS
V1.00:
	1. First release by l00135113.

endef
#############################################################

BHAL_TYPE=kernel

define BHAL_BUILD_CMDS
	cp $$(BHAL_DIR)/src/$(strip $(CHIP_NAME))/bhalplat.h $(STAGING_DIR)/usr/include
	cp $$(BHAL_DIR)/inc/bhal.h $(STAGING_DIR)/usr/include
	cp $$(BHAL_DIR)/inc/bhal.h $(HOST_DIR)/usr/include
	
	$(Q)if [[ "$(strip $(CHIP_NAME))" = "rt63365" ]]; then \
	cp -Rf $(KERNEL_DIR)/include/mtd $(STAGING_DIR)/usr/include; \
	cp -f $(KERNEL_DIR)/include/asm-mips/socket.h $(STAGING_DIR)/usr/include/asm; \
	fi
endef

define BHAL_INSTALL_TARGET_CMDS
	echo ""
endef

$(eval $(call ATPTARGETS,package/atp/libraries/private/bhalapi/kernel,bhal,required))