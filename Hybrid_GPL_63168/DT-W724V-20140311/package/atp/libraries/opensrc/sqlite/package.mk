#############################################################
#
# sqlite
#
#############################################################

#
# Dependencies
#
SQLITE_DEPENDENCIES = atptypes

#############################################################
### Version and change logs area
SQLITE_VERSION = V1.01
define SQLITE_CHANGELOGS
V1.01:
	1. First release by Handy.
endef
#############################################################

# Makefile directory
SQLITE_MAKEFILE_DIR = src

# License
SQLITE_LICENSE = BSD

# PROGRAME_NAME
SQLITE_PROGRAM_NAME =sqlite

# Target name
SQLITE_TARGET = shared

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/sqlite,sqlite))

include package/atp/libraries/opensrc/sqlite/lua-sqlite/package.mk
