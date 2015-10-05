#############################################################
#
# KATPCONSOLE
#
#############################################################
 
 
# Dependencies
KATPCONSOLE_DEPENDENCIES=
 
#############################################################
### Version and change logs area
KATPCONSOLE_VERSION=
define KATPCONSOLE_CHANGELOGS
 
endef
#############################################################
 
# Program name
KATPCONSOLE_PROGRAM_NAME=katpconsole
 
# Target name
KATPCONSOLE_TARGET=shared
 
KATPCONSOLE_TYPE=kernel
 
KATPCONSOLE_MAKEFILE_DIR=src
 
KATPCONSOLE_INDEX=345
 
KATPCONSOLE_LICENSE="atp"

define KATPCONSOLE_BUILD_CMDS
	@echo ""
endef

define KATPCONSOLE_INSTALL_TARGET_CMDS
	@echo ""
endef

define KATPCONSOLE_CLEAN_CMDS
	@echo ""
endef
 
$(eval $(call ATPTARGETS,package/atp/configuration/monitor/kernel,katpconsole,required))