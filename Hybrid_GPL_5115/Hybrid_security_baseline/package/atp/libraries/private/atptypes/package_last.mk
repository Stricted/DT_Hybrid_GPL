#############################################################
#
# atptypes
#
#############################################################

#
# Dependencies
#
ATPTYPES_DEPENDENCIES = prepare dirs install-all-atpkconfigs install-all-headers install_cbbids

ATPTYPES_CLEAN_DEPENDENCIES = remove-all-headers

#############################################################
### Version and change logs area
ATPTYPES_VERSION = V1.02

define ATPTYPES_CHANGELOGS
V1.01:
	1. First release by Handy.

V1.02:
	1. Changed for testing.
endef
#############################################################

ATPTYPES_HAS_DOC=y

ATPTYPES_LICENSE="Huawei private"

define GEN_MODULE_MACRO
	@echo "define " $1 "=" $($1)
endef

$(O)/package/atp/libraries/private/atptypes/.stamp_install_cbbid:
	$(Q)rm -f $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)mkdir -p $(ATPTYPES_BDIR)/inc
	$(Q)echo "#ifndef __ATP_CBBID_H__" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "#define __ATP_CBBID_H__" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "// Auto generated module ids" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "enum ATP_MODULE_CATEGORY_EN" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "{" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)$(foreach m,$(CBBIDTARGETS),echo "    ATP_MODULE_$m=$($(m)_INDEX)," >> $(ATPTYPES_BDIR)/inc/atpcbbid.h;)
	$(Q)echo "};" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "#endif" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)echo "" >> $(ATPTYPES_BDIR)/inc/atpcbbid.h
	$(Q)touch $(O)/package/atp/libraries/private/atptypes/.stamp_install_cbbid
	$(Q)install -m 777 $(ATPTYPES_BDIR)/inc/atpcbbid.h $(STAGING_DIR)/usr/include
	$(Q)install -m 777 $(ATPTYPES_BDIR)/inc/atpcbbid.h $(HOST_DIR)/usr/include

ifndef GPLREL
install_cbbids: $(O)/package/atp/libraries/private/atptypes/.stamp_install_cbbid

define ATPTYPES_INSTALL_HEADER_CMDS
	$(Q)if [ -f $(BR2_DEPENDS_DIR)/autoconf.h ]; then \
	mkdir -p $$(ATPTYPES_BDIR)/inc; \
	cp $(BR2_DEPENDS_DIR)/autoconf.h $$(ATPTYPES_BDIR)/inc/atpconfig.h; \
	$(SED) "s/define BUILD_/define SUPPORT_ATP_/" $$(ATPTYPES_BDIR)/inc/atpconfig.h; \
	install -m 777 $$(ATPTYPES_DIR)/inc/*.h $(STAGING_DIR)/usr/include; \
	install -m 777 $$(ATPTYPES_BDIR)/inc/*.h $(STAGING_DIR)/usr/include; \
	install -m 777 $$(ATPTYPES_DIR)/inc/*.h $(HOST_DIR)/usr/include; \
	install -m 777 $$(ATPTYPES_BDIR)/inc/*.h $(HOST_DIR)/usr/include; \
	fi
endef

define ATPTYPES_INSTALL_TARGET_CMDS
	$(Q)mkdir -p $(HOST_DIR)/usr/include
	$(Q)install -m 777 $$(ATPTYPES_BDIR)/inc/*.h $(HOST_DIR)/usr/include
endef

define ATPTYPES_CLEAN_CMDS
	$(Q)rm -f $(TOPDIR)/output/build/buildroot-config/auto.conf
endef

endif
ifdef GPLREL
install_cbbids: 
endif


$(eval $(call ATPTARGETS,package/atp/libraries/private/atptypes,atptypes,required))
