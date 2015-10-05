#############################################################
#
# pppc
#
#############################################################

#
# Dependencies
#
PPPC_DEPENDENCIES = atptypes gplutil

#############################################################
### Version and change logs area
PPPC_VERSION = V1.01
define PPPC_CHANGELOGS
V1.00:
	1. First release by d00191326.

endef
#############################################################

# Target name
PPPC_TARGET = dynamic

# Target index
PPPC_INDEX = 61

# Target type
PPPC_TYPE = main
PPPC_LICENSE = GPLv2

# Target name
PPPC_PROGRAM_NAME = pppc

$(eval $(call ATPTARGETS,package/atp/network/pppc/service/main,pppc))
