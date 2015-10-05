#############################################################
#
#CUPS
#
#############################################################
CUPS_DEPENDENCIES =gplutil cups-extract
CUPS_TAR_SRC = $(TOPDIR)/dl/cups-1.4.4.tar.gz
CUPS_AFTER_EXTR_SRC = $(TOPDIR)/output/build/cups-1.4.4
CUPS_TAR_EXTRACT_OPT = tar zxf

#############################################################
### Version and change logs area
define CUPS_CHANGELOGS
V1.00:
	1. First release by w00190448.

endef
#############################################################

define CUPS_BUILD_CMDS
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $(CUPS_AFTER_EXTR_SRC)/src  all
endef
define CUPS_INSTALL_TARGET_CMDS
	$(Q)install -m 777 $(CUPS_AFTER_EXTR_SRC)/src/scheduler/cupsd $(TOPDIR)/output/target/bin
endef

define CUPS_CLEAN_CMDS
	if [ -d $(CUPS_AFTER_EXTR_SRC)/src ]; then\
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $(CUPS_AFTER_EXTR_SRC)/src clean;\
	fi
endef

$(eval $(call ATPTARGETS,package/atp/applications/usbprinter/service/cups,cups,BUILD_CUPS))