#############################################################
#
# bcmpkg
#
#############################################################

BCMPKG_DEPENDENCIES = 

### Version and change logs area
BCMPKG_VERSION = V1.01

define BCMPKG_CHANGELOGS
V1.01:
	1. First release by sunbo.	
endef
#############################################################
LZMA=$(HOST_DIR)/usr/bin/cmplzma
BCM_FILLNVRAM=$(HOST_DIR)/usr/bin/createimg

LZMA_CMD=$(LZMA)

BCMPKG_LICENSE="Huawei private"

#LZMA=$HW_HOST_DIR/cmplzma -k -2 $TARGET_DIR/vmlinux $TARGET_DIR/vmlinux.bin $TARGET_DIR/vmlinux.lz

ifdef GPLREL
define BCMPKG_BUILD_CMDS
if [ -f $(TOPDIR)/$$(BCMPKG_DIR)/src/Makefile ]; then \
	$(Q)$(HOST_ATP_ENVS) O=$$($(2)_BDIR)/ HOST_DIR="$(HOST_DIR)" BCM_DIR="$(TOPDIR)/$$(BCMPKG_DIR)/src" $(MAKE) -C $$(BCMPKG_DIR)/src all;\
fi; 
endef
endif
ifndef GPLREL
define BCMPKG_BUILD_CMDS
$(Q)$(HOST_ATP_ENVS) O=$$($(2)_BDIR)/ HOST_DIR="$(HOST_DIR)" BCM_DIR="$(TOPDIR)/$$(BCMPKG_DIR)/src" $(MAKE) -C $$(BCMPKG_DIR)/src all
endef
endif
define BCMPKG_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $$($(2)_BDIR)/lzmacmd $(HOST_DIR)/usr/bin
	$(Q)install -m 777 $$($(2)_BDIR)/cmplzma $(LZMA)
	$(Q)install -m 777 $$($(2)_BDIR)/createimg $(HOST_DIR)/usr/bin
endef

define BCMPKG_CLEAN_CMDS
	$(Q)$(HOST_ATP_ENVS) O=$$($(2)_BDIR)/ $(MAKE) -C $$(BCMPKG_DIR)/src clean
endef

$(eval $(call ATPTARGETS,package/atp/hosttool/package/bcm,bcmpkg))
