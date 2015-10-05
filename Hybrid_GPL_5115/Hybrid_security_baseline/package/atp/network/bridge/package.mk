ifndef GPLREL
include package/atp/network/bridge/cms/package.mk
endif
include package/atp/network/bridge/kernel/package.mk
include package/atp/network/bridge/service/package.mk
ifndef GPLREL
include package/atp/network/bridge/bridgeportcms/package.mk
include package/atp/network/bridge/web/package.mk
endif
