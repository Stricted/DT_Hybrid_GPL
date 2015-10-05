ifndef GPLREL
include package/atp/network/eth/service/ethcmd/package.mk
include package/atp/network/eth/service/ethswctl/package.mk
include package/atp/network/eth/lanethcms/package.mk
include package/atp/network/eth/wanethcms/package.mk
include package/atp/network/eth/ethintfcms/package.mk
include package/atp/network/eth/ethlinkcms/package.mk
endif
include package/atp/network/eth/kernel/package.mk
ifndef GPLREL
include package/atp/network/eth/cli/package.mk
endif
