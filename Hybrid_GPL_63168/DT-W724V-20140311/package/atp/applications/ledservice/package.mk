ifneq ($(BUILD_LEDSERVICE),)
ifndef GPLREL
include package/atp/applications/ledservice/cms/package.mk
endif
include package/atp/applications/ledservice/kernel/package.mk
endif

