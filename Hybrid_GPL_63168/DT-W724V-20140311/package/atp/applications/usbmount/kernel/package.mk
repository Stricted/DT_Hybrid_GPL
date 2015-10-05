#############################################################
#
# KUSBMOUNT
#
#############################################################

#
# Dependencies
#
KUSBMOUNT_DEPENDENCIES = 

#############################################################
### Version and change logs area
KUSBMOUNT_VERSION = V1.00

define KUSBMOUNT_CHANGELOGS
V1.00:
	1. First release by w00190448

endef
#############################################################

KUSBMOUNT_TYPE=atpkconfig

define KUSBMOUNT_LINUX_CONFIGURE_CMDS

	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_NLS_UTF8,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_ENABLE_OPT,CONFIG_NLS_CODEPAGE_437,$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_SET_OPT,CONFIG_FAT_DEFAULT_IOCHARSET,"\"utf8"\",$$(KERNEL_CONFIG_FILE))
	$(call ATP_KCONFIG_SET_OPT,CONFIG_FAT_DEFAULT_CODEPAGE,437,$$(KERNEL_CONFIG_FILE))
	
#W724v将usb-storage模块运行，通过wanumts的宏隔开#
ifneq ($(BUILD_WANUMTS),y)
	$(call ATP_KCONFIG_SET_OPT,CONFIG_USB_STORAGE,m,$$(KERNEL_CONFIG_FILE))
endif
	
endef


$(eval $(call ATPTARGETS,package/atp/applications/usbmount/kernel,kusbmount,BUILD_USBMOUNT))
