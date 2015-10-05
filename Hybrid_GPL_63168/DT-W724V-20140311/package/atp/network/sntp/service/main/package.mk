#############################################################
#
# sntp
#
#############################################################

#
# Dependencies
#
SNTP_DEPENDENCIES = atptypes gplutil
SNTP_CLEAN_DEPENDENCIES = atptypes-clean gplutil-clean

#############################################################
### Version and change logs area
SNTP_VERSION = V1.01
define SNTP_CHANGELOGS
V1.00:
	1. First release by PJ.

endef
#############################################################
# License
GPLUTIL_LICENSE = GPL

# Target name
SNTP_TARGET = dynamic

# Target index
SNTP_INDEX = 59

# Target type
SNTP_TYPE = main

# Target name
SNTP_PROGRAM_NAME = sntp

$(eval $(call ATPTARGETS,package/atp/network/sntp/service/main,sntp,BUILD_SNTP))
