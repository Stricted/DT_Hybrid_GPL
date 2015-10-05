ifndef GPLREL

include package/atp/hosttool/package/imagecmd/*.mk
endif
include package/atp/hosttool/package/pkg/*.mk
ifndef GPLREL
include package/atp/hosttool/package/sign/*.mk
endif

ifeq ($(BUILD_BCMPKG),y)
include package/atp/hosttool/package/bcm/*.mk
endif

ifeq ($(BUILD_MLZMA),y)
include package/atp/hosttool/package/mlzma/*.mk
endif

ifeq ($(BUILD_QLZMA),y)
include package/atp/hosttool/package/qlzma/*.mk
endif
