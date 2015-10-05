#############################################################
#
# Build the jffs2 root filesystem image
#
#############################################################

JFFS2_OPTS := -e $(BR2_TARGET_ROOTFS_JFFS2_EBSIZE)
SUMTOOL_OPTS := $(JFFS2_OPTS)

ifeq ($(BR2_TARGET_ROOTFS_JFFS2_PAD),y)
ifneq ($(strip $(BR2_TARGET_ROOTFS_JFFS2_PADSIZE)),0x0)
JFFS2_OPTS += --pad=$(strip $(BR2_TARGET_ROOTFS_JFFS2_PADSIZE))
else
JFFS2_OPTS += -p
endif
SUMTOOL_OPTS += -p
endif

ifeq ($(BR2_TARGET_ROOTFS_JFFS2_LE),y)
JFFS2_OPTS += -l
SUMTOOL_OPTS += -l
endif

ifeq ($(BR2_TARGET_ROOTFS_JFFS2_BE),y)
JFFS2_OPTS += -b
SUMTOOL_OPTS += -b
endif

#JFFS2_OPTS += -s $(BR2_TARGET_ROOTFS_JFFS2_PAGESIZE)
ifeq ($(BR2_TARGET_ROOTFS_JFFS2_NOCLEANMARKER),y)
JFFS2_OPTS += -n
SUMTOOL_OPTS += -n
endif

#/*添加用户自己定制mkfs.jffs2命令*/
ifneq ($(BR2_TARGET_ROOTFS_JFFS2_CUSTOM_CMD),"")
JFFS2_OPTS  += -N '$(subst $(space),\ ,$(BR2_TARGET_ROOTFS_JFFS2_CUSTOM_CMD))'
endif


ROOTFS_JFFS2_DEPENDENCIES = host-mtd

ifneq ($(BUILD_HYBRID),)
define EXT_JFFS2_CMD
	$(MKFS_JFFS2) $(JFFS2_OPTS) -d $(TARGET_DIR)/../lte -o $(BINARIES_DIR)/lte.jffs2
endef

define EXT_JFFS2_W_CMD
	$(MKFS_JFFS2) $(JFFS2_OPTS) -d $(TARGET_DIR)/../lte_w -o $(BINARIES_DIR)/lte_w.jffs2
endef

define EXT_JFFS2_FACTORY_CMD
	$(MKFS_JFFS2) $(JFFS2_OPTS) -d $(TARGET_DIR)/../lte_fact -o $(BINARIES_DIR)/lte_fact.jffs2
endef
else
define EXT_JFFS2_CMD
	echo ""
endef

define EXT_JFFS2_W_CMD
	echo ""
endef

define EXT_JFFS2_FACTORY_CMD
	echo ""
endef
endif

ifneq ($(BUILD_ATP_DEBUG_IAMGE),)
	SD5115_IMG_NAME=uImage_debug
else
	SD5115_IMG_NAME=uImage_release
endif

ifneq ($(BUILD_HYBRID),)
define ROOTFS_JFFS2_CMD_PRE
	mkdir -p $(TARGET_DIR)/etc/tftpd;\
	install -m 777 $(CONFIG_DIR)/extbin/$(SD5115_IMG_NAME) $(TARGET_DIR)/uImage;\
	rm -f $(TARGET_DIR)/etc/tftpd/uImage;\
	ln -s ../../uImage $(TARGET_DIR)/etc/tftpd/uImage;\
	cd $(TARGET_DIR)/etc/tftpd;\
	rm -f $(TARGET_DIR)/etc/tftpd/image.bin;\
	ln -s ../../var/lte_fota/lte_fota.bin $(TARGET_DIR)/etc/tftpd/image.bin;\
	mkdir -p $(TARGET_DIR)/etc/i2c;\
	install -m 777 $(CONFIG_DIR)/extbin/i2c.bin $(TARGET_DIR)/etc/i2c/;\
	echo $(HGW_I2C_VERSION) > $(TARGET_DIR)/etc/i2c/i2c.ini
endef
else
define ROOTFS_JFFS2_CMD_PRE
	echo ""
endef
endif

ifneq ($(BR2_TARGET_ROOTFS_JFFS2_SUMMARY),)
define ROOTFS_JFFS2_CMD
	$(MKFS_JFFS2) $(JFFS2_OPTS) -d $(TARGET_DIR) -o $$@.nosummary && \
	$(SUMTOOL) $(SUMTOOL_OPTS) -i $$@.nosummary -o $$@ && \
	rm $$@.nosummary
endef
else
define ROOTFS_JFFS2_CMD
	$(ROOTFS_JFFS2_CMD_PRE) ;\
	$(MKFS_JFFS2) $(JFFS2_OPTS) -d $(TARGET_DIR) -o $$@
endef
endif

define ROOTFS_JFFS2_CMD_EX
	$(MKFS_JFFS2) $(JFFS2_OPTS) 
endef


$(eval $(call ROOTFS_TARGET,jffs2))
