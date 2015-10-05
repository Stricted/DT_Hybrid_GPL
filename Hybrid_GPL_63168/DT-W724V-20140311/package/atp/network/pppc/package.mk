#############################################################
#
# pppccms
#
ifndef GPLREL
include package/atp/network/pppc/cms/package.mk
endif
include package/atp/network/pppc/service/main/package.mk
ifndef GPLREL
include package/atp/network/pppc/pppintfcms/package.mk
include package/atp/network/pppc/web/package.mk
endif
