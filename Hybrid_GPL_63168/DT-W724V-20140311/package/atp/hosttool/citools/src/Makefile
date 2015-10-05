	
# include the rule.mak
include $(CONFIG_DIR)/.config

all: sizesync

dynamic: sizesync

sizesync:
ifeq ($(strip $(BUILD_ATP_DEBUG_IAMGE)),y)
	@python $(TOPDIR)/package/atp/hosttool/citools/src/sizesync.py $(TARGET_DIR) $(STAGING_DIR) $(HGW_IMAGE_DEBUG_WITHOUT_BOOTLOADER_NAME).bin $(ATP_LINUX_FILENAME) $(CUSTOMER) $(VENDOR) $(SVNVERSION)
else
	@python $(TOPDIR)/package/atp/hosttool/citools/src/sizesync.py $(TARGET_DIR) $(STAGING_DIR) $(HGW_IMAGE_RELEASE_WITHOUT_BOOTLOADER_NAME).bin $(ATP_LINUX_FILENAME) $(CUSTOMER) $(VENDOR) $(SVNVERSION)
endif

clean:

