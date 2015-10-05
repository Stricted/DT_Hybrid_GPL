#############################################################
#
# addecc
#
#############################################################
HOST_SIGN_DEPENDENCIES =atptypes

### Version and change logs area
HOST_SIGN_VERSION = V1.01

define ADDECC_CHANGELOGS
V1.01:
	1. First release by lijianjun.
	
endef
#############################################################

ADDECC_UTIL=$(HOST_DIR)/usr/bin/createEccFullImage

ADDECC_PROGRAM_NAME=createEccFullImage

define ADDECC_INSTALL_TARGET_CMDS
	install -m 0755 $(O)/package/atp/hosttool/package/addecc/createEccFullImage $(ADDECC_UTIL)
endef

define ADDECC_BUILD_CMDS
	$(Q)if [ -f $$(ADDECC_DIR)/src/Makefile ]; then \
		$(HOST_ATP_ENVS) O=$$($(2)_BDIR)/ HOST_DIR="$(HOST_DIR)" ADDECC_DIR="$(ADDECC_DIR)" $(MAKE) -C $$(ADDECC_DIR)/src all;\
	fi
endef

$(eval $(call ATPTARGETS,package/atp/hosttool/package/addecc,addecc))

######################################################################
#  addecc tool usage                                                   #
######################################################################
ifeq ($(BUILD_ADDECC),y)
define ADDECC_CMD
	$(ADDECC_UTIL)
endef
else
define ADDECC_CMD
	echo
endef
endif
