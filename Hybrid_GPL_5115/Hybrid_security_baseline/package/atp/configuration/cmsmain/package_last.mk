include package/atp/configuration/cmsmain/kernel/package.mk

#############################################################
#
# cms
#
#############################################################

#
# Dependencies
#
CMS_DEPENDENCIES = atptypes cfm http $(CMSTARGETS)

#############################################################
### Version and change logs area
CMS_VERSION = V1.01
define CMS_CHANGELOGS
V1.00:
	1. First release by Handy.

endef
#############################################################

# Target name
CMS_INDEX =152

# Target name
CMS_TARGET = all

# Target name
CMS_PROGRAM_NAME =cms

define CMS_BUILD_CMDS
	if [ -f $$(CMS_SRC_DIR)/Makefile ]; then \
	$(TARGET_ATP_ENVS) O=$$(CMS_BDIR)/ PROGRAM_NAME=$$(CMS_REAL_PROGRAM_NAME) IS_DYNAMIC="$$(CMS_IS_DYNAMIC)" $(MAKE1) -C $$(CMS_SRC_DIR) clean;\
	rm -f $$(CMS_BDIR)/.stamp_installtarget; \
	$(TARGET_ATP_ENVS) O=$$(CMS_BDIR)/ PROGRAM_NAME=$$(CMS_REAL_PROGRAM_NAME) IS_DYNAMIC="$$(CMS_IS_DYNAMIC)" CMSLIBS="$$(CMSTARGETS_LIBS)" WEBLIBS="$$(WEBTARGETS_LIBS)" CLILIBS="$$(CLITARGETS_LIBS)" ATPFLAGS="-DMODULE_VERSION=\\\"$$(CMS_VERSION_STR)\\\" -DCMS_MODULE_VERSION=\\\"$$(CMS_VERSION_STR)\\\" $$(CMS_FLAGS)" $(MAKE1) -C $$(CMS_SRC_DIR) $$(CMS_TARGET);\
	fi
endef

cms-clean-all: cms-clean $(CMSTARGETS_CLEAN)

$(eval $(call ATPTARGETS,package/atp/configuration/cmsmain,cms,required))
