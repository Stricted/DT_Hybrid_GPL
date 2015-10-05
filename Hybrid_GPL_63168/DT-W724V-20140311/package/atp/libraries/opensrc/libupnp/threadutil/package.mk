#############################################################
#
# THREADUTIL
#
#############################################################


# Dependencies
THREADUTIL_DEPENDENCIES=
#############################################################
### Version and change logs area
THREADUTIL_VERSION=
define THREADUTIL_CHANGELOGS

endef
#############################################################

# Program name
THREADUTIL_PROGRAM_NAME=threadutil

# Target name
THREADUTIL_TARGET=shared

THREADUTIL_TYPE=opensrclib

THREADUTIL_MAKEFILE_DIR=src

THREADUTIL_INDEX=22

THREADUTIL_LICENSE="BSD"

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libupnp/threadutil,threadutil,BUILD_LIBUPNP))
