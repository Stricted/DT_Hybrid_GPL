#############################################################
#
# KCOREDUMP
#
#############################################################

#
# Dependencies
#
KCOREDUMP_DEPENDENCIES = 

#############################################################
### Version and change logs area
KCOREDUMP_VERSION = V1.00

define KCOREDUMP_CHANGELOGS
V1.00:
	1. First release by sunbo.

endef
#############################################################

KCOREDUMP_TYPE=atpkconfig

define KCOREDUMP_LINUX_CONFIGURE_CMDS
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_ELF_CORE,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_DISABLE_OPT,CONFIG_CORE_DUMP_DEFAULT_ELF_HEADERS,$$(KERNEL_CONFIG_FILE))
endef


$(eval $(call ATPTARGETS,package/atp/configuration/debug/kernel,kcoredump,BUILD_COREDUMP))
