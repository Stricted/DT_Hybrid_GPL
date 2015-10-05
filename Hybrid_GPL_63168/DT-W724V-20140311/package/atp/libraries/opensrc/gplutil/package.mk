#############################################################
#
# gplutil
#
#############################################################
#

# Dependencies
#
GPLUTIL_DEPENDENCIES = atptypes bhal
#############################################################

GPLUTIL_VERSION = 1.01
define GPLUTIL_CHANGELOGS
V1.00:
	1. First release by Handy.
V1.02:
	2. x00192803.	
endef

#############################################################

# Makefile directory
GPLUTIL_MAKEFILE_DIR = src

# License
GPLUTIL_LICENSE = GPL

# PROGRAME_NAME
GPLUTIL_PROGRAM_NAME = gplutil

GPLUTIL_INDEX=18

# Target name
GPLUTIL_TARGET=shared


$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/gplutil,gplutil))
