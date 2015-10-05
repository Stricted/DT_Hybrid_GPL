#############################################################
#
# wine
#
#############################################################

#
# Dependencies
#
WINE_DEPENDENCIES = wine-extract

#############################################################

WINE_TAR_VERSION_STR=1.1.27
WINE_TAR_SRC = $(TOPDIR)/dl/wine-$(WINE_TAR_VERSION_STR).tar.bz2
WINE_AFTER_EXTR_SRC = $(O)/build/wine
WINE_TAR_EXTRACT_OPT = tar xjf

SERVICE_WINE_SRC_DIR = $(WINE_AFTER_EXTR_SRC)/wine

### Version and change logs area
WINE_VERSION = V1.00
define IPTABLES_CHANGELOGS
V1.00:
	1. First release by z65940.

endef
#############################################################

# Target name
WINE_TARGET=dynamic

# Target index
WINE_INDEX=4444

WINE_FEATURE=buildroot

WINE_LICENSE="GPL"

# Target name
WINE_PROGRAM_NAME=wine

define WINE_BUILD_CMDS
	@echo "Build ..."
	$(MAKE) -C $(SERVICE_WINE_SRC_DIR)
endef

define WINE_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_WINE_SRC_DIR)/Makefile ]; then \
	$(MAKE) -C $(SERVICE_WINE_SRC_DIR) clean; \
	fi
endef

define WINE_INSTALL_TARGET_CMDS
	@echo "Install ..."
endef

#$(eval $(call ATPTARGETS,package/atp/hosttool/wine,wine,BUILD_NONE))
