#############################################################
#
# IUPNP
#
#############################################################


# Dependencies
IUPNP_DEPENDENCIES=
#############################################################
### Version and change logs area
IUPNP_VERSION=
define IUPNP_CHANGELOGS

endef
#############################################################

# Program name
IUPNP_PROGRAM_NAME=iupnp

# Target name
IUPNP_TARGET=shared

IUPNP_TYPE=opensrclib

IUPNP_MAKEFILE_DIR=src

IUPNP_INDEX=20

IUPNP_LICENSE="BSD"

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/libupnp/iupnp,iupnp,BUILD_LIBUPNP))
