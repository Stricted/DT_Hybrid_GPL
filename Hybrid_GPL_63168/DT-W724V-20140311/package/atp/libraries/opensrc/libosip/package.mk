#############################################################
#
# OSIP
#
#############################################################
 
 
# Dependencies
OSIP_DEPENDENCIES= osip-extract osip_cfg

OSIP_TAR_SRC = $(TOPDIR)/dl/libosip2.tar.gz
OSIP_AFTER_EXTR_SRC = $(TOPDIR)/output/build/libosip2
OSIP_TAR_EXTRACT_OPT = tar zxf

#############################################################
### Version and change logs area
OSIP_VERSION=
define OSIP_CHANGELOGS
 
endef
#############################################################

$(OSIP_AFTER_EXTR_SRC)/.stamp_configured:
	$(Q)cd $(OSIP_AFTER_EXTR_SRC)/libosip2; \
	chmod +x configure; \
	$(TARGET_ATP_ENVS) ./configure --host=$(ARCH)-linux
	$(Q)touch $(OSIP_AFTER_EXTR_SRC)/.stamp_configured

osip_cfg: $(OSIP_AFTER_EXTR_SRC)/.stamp_configured

# Install lib
define OSIP_INSTALL_TARGET_CMDS
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/src/osip2/.libs/libosip2.a $(TARGET_DIR)/lib
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/src/osip2/.libs/libosip2.a $(STAGING_DIR)/lib
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/src/osipparser2/.libs/libosipparser2.a $(TARGET_DIR)/lib
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/src/osipparser2/.libs/libosipparser2.a $(STAGING_DIR)/lib
	@rm -fr $(STAGING_DIR)/usr/include/osip2
	@rm -fr $(STAGING_DIR)/usr/include/osipparser2
	@mkdir $(STAGING_DIR)/usr/include/osip2
	@mkdir $(STAGING_DIR)/usr/include/osipparser2
	@mkdir $(STAGING_DIR)/usr/include/osipparser2/headers
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/include/osip2/*.h $(STAGING_DIR)/usr/include/osip2
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/include/osipparser2/*.h $(STAGING_DIR)/usr/include/osipparser2
	@install -m 777 $$(OSIP_AFTER_EXTR_SRC)/libosip2/include/osipparser2/headers/*.h $(STAGING_DIR)/usr/include/osipparser2/headers
endef

define OSIP_INSTALL_HEADER_CMDS
	
endef

define OSIP_BUILD_CMDS
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $$(OSIP_AFTER_EXTR_SRC)/libosip2/src
endef

define OSIP_CLEAN_CMDS
	rm -f $$(OSIP_AFTER_EXTR_SRC)/.stamp_*
	if [ -f $$(OSIP_AFTER_EXTR_SRC)/libosip2/src/Makefile ]; then \
	$(TARGET_ATP_ENVS) $(MAKE1)	-C $$(OSIP_AFTER_EXTR_SRC)/libosip2/src clean; \
	fi
endef

# Program name
OSIP_PROGRAM_NAME=osip
 
# Target name
OSIP_TARGET=all
 
OSIP_TYPE=opensrclib
 
OSIP_MAKEFILE_DIR=libosip2
 
OSIP_INDEX=210
 
OSIP_LICENSE="LGPL"
 
$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libosip,osip,BUILD_LIBOSIP))