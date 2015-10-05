#############################################################
#
# SIPROXD
#
#############################################################

# Dependencies
SIPROXD_DEPENDENCIES=osip siproxd-extract siproxd_cfg
#############################################################

ifeq ("$(VENDOR)", "W724v")
SIPROXD_TAR_SRC = $(TOPDIR)/dl/siproxd-dt.tgz
SIPROXD_AFTER_EXTR_SRC = $(O)/build/siproxd-dt
else
ifeq ("$(VENDOR)", "Hybrid")
SIPROXD_TAR_SRC = $(TOPDIR)/dl/siproxd-dt.tar.gz
SIPROXD_AFTER_EXTR_SRC = $(O)/build/siproxd-dt
else
SIPROXD_TAR_SRC = $(TOPDIR)/dl/siproxd-0.5.10.tgz
SIPROXD_AFTER_EXTR_SRC = $(O)/build/siproxd-0.5.10
endif
endif

SIPROXD_TAR_EXTRACT_OPT = tar zxvf

### Version and change logs area
SIPROXD_VERSION=
define SIPROXD_CHANGELOGS

endef
#############################################################


$(O)/package/atp/network/alg/service/siproxd/.stamp_configured:
	$(Q)if [ -f $(SIPROXD_AFTER_EXTR_SRC)/siproxd/configure ]; then \
		cd $(SIPROXD_AFTER_EXTR_SRC)/siproxd; \
		chmod +x configure; \
		$(TARGET_ATP_ENVS) ./configure --host=$(ARCH)-linux \
		--with-libosip-prefix="$(STAGING_DIR)/usr" LDFLAGS="-lgplutil" CFLAGS="-fno-strict-aliasing" --includedir="$(STAGING_DIR)/usr/include"; \
		fi
	@touch $(O)/package/atp/network/alg/service/siproxd/.stamp_configured

siproxd_cfg: $(O)/package/atp/network/alg/service/siproxd/.stamp_configured

# Install lib
define SIPROXD_INSTALL_TARGET_CMDS
	@install -m 777 $(SIPROXD_AFTER_EXTR_SRC)/siproxd/src/siproxd $(STAGING_DIR)/bin
	@install -m 777 $(SIPROXD_AFTER_EXTR_SRC)/siproxd/src/siproxd $(TARGET_DIR)/bin
endef

define SIPROXD_BUILD_CMDS
	$(Q)if [ -f $(SIPROXD_AFTER_EXTR_SRC)/siproxd/Makefile ]; then \
	$(MAKE) -C $(SIPROXD_AFTER_EXTR_SRC)/siproxd; \
	fi
endef

define SIPROXD_CLEAN_CMDS
	$(Q)if [ -f $(SIPROXD_AFTER_EXTR_SRC)/siproxd/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SIPROXD_AFTER_EXTR_SRC)/siproxd clean; \
	fi
endef

# Program name
SIPROXD_PROGRAM_NAME=siproxd

# Target name
SIPROXD_TARGET=all

SIPROXD_TYPE=opensrcbin

SIPROXD_MAKEFILE_DIR=$(SIPROXD_AFTER_EXTR_SRC)/siproxd

SIPROXD_INDEX=211

SIPROXD_LICENSE="GPLv2"

$(eval $(call ATPTARGETS,package/atp/network/alg/service/siproxd,siproxd,BUILD_SIPROXYD))
