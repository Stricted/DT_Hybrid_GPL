#############################################################
#
# iptables
#
#############################################################
#
# Dependencies
#
IPTABLES_DEPENDENCIES = atptypes iptables-extract iptables_cfg

#############################################################

IPTABLES_VERSION_STR_TMP=$(call qstrip,$(ATP_IPTABLES_VERSION))
IPTABLES_TAR_SRC = $(TOPDIR)/dl/iptables-$(IPTABLES_VERSION_STR_TMP).tar.gz
IPTABLES_AFTER_EXTR_SRC = $(O)/build/iptables-$(IPTABLES_VERSION_STR_TMP)
IPTABLES_TAR_EXTRACT_OPT = tar zxvf

SERVICE_IPTABLES_SRC_DIR = $(IPTABLES_AFTER_EXTR_SRC)/src


### Version and change logs area
IPTABLES_VERSION = V1.00
define IPTABLES_CHANGELOGS
V1.00:
	1. First release by h00163136.

endef
#############################################################

define IPTABLES_INSTALL_HEADER_CMDS
	$(Q)if [ -d $(SERVICE_IPTABLES_SRC_DIR)/extensions/ ];then \
	chmod +x $(SERVICE_IPTABLES_SRC_DIR)/extensions/.*;\
	fi
endef

# uClibc wrapper
ifeq ($(BR2_TOOLCHAIN_EXTERNAL_UCLIBC),y)
PLATFORM := $(ARCH)-uclibc
else
PLATFORM := $(ARCH)
endif

ifeq ($(ATP_IPTABLES_VERSION),"1.4.16.3")
ifeq ($(BUILD_IPV6),y)
$(O)/package/atp/libraries/opensrc/iptables/.stamp_configured:
	[ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ] || \
	(cd $(SERVICE_IPTABLES_SRC_DIR); \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --disable-devel \
	    --disable-shared \
	    --with-kernel=$(KERNEL_DIR) \
	    --prefix=$(SERVICE_IPTABLES_SRC_DIR)/install \
	)
else # disable ipv6
$(O)/package/atp/libraries/opensrc/iptables/.stamp_configured:
	[ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ] || \
	(cd $(SERVICE_IPTABLES_SRC_DIR); \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --disable-devel \
	    --disable-shared \
	    --with-kernel=$(KERNEL_DIR) \
	    --prefix=$(SERVICE_IPTABLES_SRC_DIR)/install \
	    --disable-ipv6 \
	)
endif
else
ifeq ($(BUILD_IPV6),y)
$(O)/package/atp/libraries/opensrc/iptables/.stamp_configured:
	[ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ] || \
	(cd $(SERVICE_IPTABLES_SRC_DIR); \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(KERNEL_DIR) \
	    --prefix=$(SERVICE_IPTABLES_SRC_DIR)/install \
	)
else # disable ipv6
$(O)/package/atp/libraries/opensrc/iptables/.stamp_configured:
	[ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ] || \
	(cd $(SERVICE_IPTABLES_SRC_DIR); \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(KERNEL_DIR) \
	    --prefix=$(SERVICE_IPTABLES_SRC_DIR)/install \
	    --disable-ipv6 \
	)
endif
endif
	@touch $(O)/package/atp/libraries/opensrc/iptables/.stamp_configured;

iptables_cfg: $(O)/package/atp/libraries/opensrc/iptables/.stamp_configured

define IPTABLES_BUILD_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) IS_DYNAMIC="$$($(2)_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" -D$(2)_MODULE_VERSION=\\\"$$($(2)_VERSION_STR)\\\" $$($(2)_FLAGS)" $(MAKE1) -C $(SERVICE_IPTABLES_SRC_DIR) $$($(2)_TARGET); \
	fi
endef

define IPTABLES_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_IPTABLES_SRC_DIR) clean; \
	fi
	rm -f $(O)/package/atp/libraries/opensrc/iptables/.stamp_configured;
endef

ifeq ($(ATP_IPTABLES_VERSION),"1.4.12")
define IPTABLES_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	install -d $(SERVICE_IPTABLES_SRC_DIR)/install;\
	install -d $(SERVICE_IPTABLES_SRC_DIR)/install/lib/iptables;\
	$(MAKE1) -C $(SERVICE_IPTABLES_SRC_DIR) install;\
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/install/sbin/$$($(2)_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin; \
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/install/sbin/$$($(2)_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(STAGING_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	fi
endef
else
ifeq ($(ATP_IPTABLES_VERSION),"1.4.16.3")
define IPTABLES_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
		install -m 755 $(SERVICE_IPTABLES_SRC_DIR)/iptables/xtables-multi $(STAGING_DIR)/bin; \
		ln -sf xtables-multi $(STAGING_DIR)/bin/iptables; \
		ln -sf xtables-multi $(STAGING_DIR)/bin/ip6tables; \
		install -m 755 $(SERVICE_IPTABLES_SRC_DIR)/iptables/xtables-multi $(TARGET_DIR)/bin; \
		ln -sf xtables-multi $(TARGET_DIR)/bin/iptables; \
		ln -sf xtables-multi $(TARGET_DIR)/bin/ip6tables; \
		$(TARGET_STRIP) $(STAGING_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
		$(TARGET_STRIP) $(TARGET_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME);	\
	fi
endef
else
define IPTABLES_INSTALL_TARGET_CMDS
	$(Q)if [ -f $(SERVICE_IPTABLES_SRC_DIR)/Makefile ]; then \
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(STAGING_DIR)/bin; \
	install -m 777 $(SERVICE_IPTABLES_SRC_DIR)/$$($(2)_FULL_PROGRAM_NAME) $(TARGET_DIR)/bin; \
	$(TARGET_STRIP) $(STAGING_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	$(TARGET_STRIP) $(TARGET_DIR)/bin/$$($(2)_FULL_PROGRAM_NAME); \
	fi
endef
endif
endif

ifeq ($(ATP_IPTABLES_VERSION),"1.4.12")
IPTABLES_TARGET = 
else
ifeq ($(ATP_IPTABLES_VERSION),"1.4.16.3")
IPTABLES_TARGET = 
else
IPTABLES_TARGET = dynamic
endif
endif

# License
IPTABLES_LICENSE = "GPLv2"
# Target index
IPTABLES_INDEX = 43

# Target name
IPTABLES_PROGRAM_NAME =iptables

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/iptables,iptables))
