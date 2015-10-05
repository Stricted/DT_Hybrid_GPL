ifndef GPLREL
include package/atp/network/igmp/cms/package.mk
include package/atp/network/igmp/service/package.mk
endif
include package/atp/network/igmp/kernel/package.mk
ifndef GPLREL
include package/atp/network/igmp/cli/package.mk
include package/atp/network/igmp/web/package.mk
endif