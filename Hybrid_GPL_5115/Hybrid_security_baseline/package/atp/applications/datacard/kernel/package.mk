#############################################################
#
# KNDIS
#
#############################################################

#
# Dependencies
#
KNDIS_DEPENDENCIES = 

#############################################################
### Version and change logs area
KNDIS_VERSION = V1.00

define KNDIS_CHANGELOGS
V1.00:
	1. First release by w00181549

endef
#############################################################

KNDIS_TYPE=atpkconfig

define KNDIS_LINUX_CONFIGURE_CMDS

	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_USB_QMITTY,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_USB_USBNET,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_USB_NET_RMNET_ETHERNET,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_ATP_NDIS_QMI,$$(KERNEL_CONFIG_FILE))	
endef

$(eval $(call ATPTARGETS,package/atp/applications/datacard/kernel,kndis,BUILD_NDIS))
