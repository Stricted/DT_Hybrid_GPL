include package/atp/network/firewall/*/package.mk
ifndef GPLREL
ifneq ($(BUILD_DT_ABUSE),)
include package/atp/network/firewall/service/abusecmd/package.mk
endif
endif
