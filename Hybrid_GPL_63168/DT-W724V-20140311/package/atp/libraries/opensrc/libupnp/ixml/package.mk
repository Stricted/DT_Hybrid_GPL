#############################################################
#
# IXML
#
#############################################################


# Dependencies
IXML_DEPENDENCIES=
#############################################################
### Version and change logs area
IXML_VERSION=
define IXML_CHANGELOGS

endef
#############################################################

# Program name
IXML_PROGRAM_NAME=ixml

# Target name
IXML_TARGET=shared

IXML_TYPE=opensrclib

IXML_MAKEFILE_DIR=src

IXML_INDEX=21

IXML_LICENSE="BSD"

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libupnp/ixml,ixml,BUILD_LIBUPNP))
