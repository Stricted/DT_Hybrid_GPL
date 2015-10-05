include package/atp/libraries/libraries.mk
include package/atp/applications/atp.mk
include package/atp/configuration/configuration.mk
include package/atp/network/network.mk
include package/atp/voice/voice.mk
include package/atp/hosttool/package/package.mk
ifndef GPLREL
include package/atp/hosttool/cfgcodec/package.mk
include package/atp/hosttool/symboldb/package.mk
endif
include package/atp/hosttool/citools/package.mk
include package/atp/hosttool/wine/package.mk
# Must be the last, please add above here
include package/atp/configuration/cmsmain/package_last.mk
ifndef GPLREL
include package/atp/hosttool/documentation/atpdoc.mk
include package/atp/configuration/web/service/package_last.mk
include package/atp/configuration/cli/service/package_last.mk
endif
include package/atp/libraries/private/atptypes/package_last.mk
