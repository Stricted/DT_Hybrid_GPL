#############################################################
#
# host atppkg
#
#############################################################

HOST_ATPPKG_DEPENDENCIES = atptypes host-atppkg-clean host-upgapi

### Version and change logs area
HOST_ATPPKG_VERSION = V1.01

define HOST_ATPPKG_CHANGELOGS
V1.01:
	1. First release by sunbo.
	
endef
#############################################################
CREATE_MULTI_UPGRADE_INI=$(HOST_DIR)/usr/bin/createMultiUpgradeIni
CALCCRC=$(HOST_DIR)/usr/bin/calccrc
PKGER=$(HOST_DIR)/usr/bin/pkger
GENHEAD=$(HOST_DIR)/usr/bin/genhead
GENINI=$(HOST_DIR)/usr/bin/genini
GENFACTORYCFG=$(HOST_DIR)/usr/bin/genfactorycfg
GENFACTORYPARTITION=$(HOST_DIR)/usr/bin/genFactoryPartition

ifeq ($(BUILD_ATP_RELEASE_IAMGE),y)
PKG_CMD_OPTS:= -p $(HGW_PRODUCTCLASS) -v $(HGW_SOFTVERSION_WEB)
endif
ifeq ($(BUILD_ATP_DEBUG_IAMGE),y)
PKG_CMD_OPTS:= -p $(HGW_PRODUCTCLASS) -v $(HGW_RELEASE_ENGINEERING)
endif
ifeq ($(BUILD_ATP_ACS_IAMGE),y)
PKG_CMD_OPTS:= -p $(HGW_PRODUCTCLASS) -v $(HGW_RELEASE_ENGINEERING)
endif

ifeq ($(BUILD_SIGN),y)
	PKG_CMD_OPTS += -s -r $(BUILD_UPG_KEY1_KEYSIZE) -h $(BUILD_UPG_KEY1_ALGID)
endif

HOST_ATPPKG_TARGET=all

ATPPKG_LICENSE="Huawei private"

define HOST_ATPPKG_INSTALL_TARGET_CMDS
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/createMultiUpgradeIni $(CREATE_MULTI_UPGRADE_INI)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/pkger $(PKGER)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/calccrc $(CALCCRC)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/genhead $(GENHEAD)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/genini $(GENINI)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/genfactorycfg $(GENFACTORYCFG)
	$(Q)install -m 0755 $$(HOST_ATPPKG_BDIR)/genFactoryPartition $(GENFACTORYPARTITION)
endef

define PACKAGE_GENFACTORYCFG_CMD
	$(GENFACTORYCFG)
endef

define PACKAGE_GENFACTORYPARTITION_CMD
	$(GENFACTORYPARTITION)
endef

define PACKAGE_PKG_CMD
	$(PKGER) $(PKG_CMD_OPTS) 
endef

define PACKAGE_CRC_CMD
	$(CALCCRC)
endef

define PACKAGE_MUPG_CMD
	$(CREATE_MULTI_UPGRADE_INI)
endef

ifeq ($(strip $(BR2_ENDIAN)),"LITTLE")
define PACKAGE_GENHEAD_CMD
	$(GENHEAD) -L l
endef
else
CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
define PACKAGE_GENHEAD_CMD
	$(GENHEAD) -L b
endef
endif

INIFILE=$(BINARIES_DIR)/config.ini
ifeq ($(BUILD_AUTO_GEN_INI),y)
define GEN_INI_CMD
	$(GENINI) -o $(INIFILE)
endef
else
define GEN_INI_CMD
	$(Q)cp $(TOPDIR)/$(BUILD_CUSTOMER_INI_PATH) $(INIFILE)
endef
endif


$(eval $(call ATPTARGETS,package/atp/hosttool/package/pkg,atppkg,BUILD_ATPPKG,host))
