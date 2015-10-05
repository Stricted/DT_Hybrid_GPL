#############################################################
#
#SAMBA
#
#############################################################
SAMBA_DEPENDENCIES = gplutil samba-extract
SAMBA_DEPENDENCIES += $(if $(BUILD_CUPS),cups,)
SAMBA_TAR_SRC = $(TOPDIR)/dl/samba-3.0.37.tar.gz
SAMBA_AFTER_EXTR_SRC = $(TOPDIR)/output/build/samba-3.0.37
SAMBA_TAR_EXTRACT_OPT = tar zxf

#############################################################
### Version and change logs area
define SAMBA_CHANGELOGS
V1.00:
	1. First release by l00193021.

endef
#############################################################
define SAMBA_BUILD_CMDS
	cp $(SAMBA_AFTER_EXTR_SRC)/src/$(ARCH)_config.h $(SAMBA_AFTER_EXTR_SRC)/src/include/config.h
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $(SAMBA_AFTER_EXTR_SRC)/src all
endef
define SAMBA_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(SAMBA_AFTER_EXTR_SRC)/src/bin/smbd $(TOPDIR)/output/target/bin
	$(Q)install -m 777 $(SAMBA_AFTER_EXTR_SRC)/src/bin/nmbd $(TOPDIR)/output/target/bin
	$(Q)install -m 777 $(SAMBA_AFTER_EXTR_SRC)/src/bin/smbpasswd $(TOPDIR)/output/target/bin
endef
define SAMBA_CLEAN_CMDS
	$(Q)if [ -d $(SAMBA_AFTER_EXTR_SRC)/src ]; then\
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $(SAMBA_AFTER_EXTR_SRC)/src clean; \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/applications/usbstorage/service/samba,samba,targets))