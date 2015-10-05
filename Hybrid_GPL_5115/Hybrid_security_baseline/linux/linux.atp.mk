#############################################################
#
# linux kernel
#
#############################################################
#include dir.atp.mak
LINUX_DEPENDENCIES = install-all-atpkconfigs 
ifeq ($(BROADCOM_CHIP),y)
LINUX_DEPENDENCIES += bcmpkg
endif

ifeq ($(BUILD_LINUX_FAKEROOT),y)
LINUX_DEPENDENCIES += host-fakeroot
FAKEROOT = fakeroot
else
FAKEROOT =
endif

LINUX_VERSION=$(call qstrip,$(BR2_LINUX_KERNEL_VERSION))

# Compute the arch path, since i386 and x86_64 are in arch/x86 and not
# in arch/$(KERNEL_ARCH). Even if the kernel creates symbolic links
# for bzImage, arch/i386 and arch/x86_64 do not exist when copying the
# defconfig file.
ifeq ($(KERNEL_ARCH),i386)
KERNEL_ARCH_PATH=$(LINUX_DIR)/$(LINUX_VERSION)/arch/x86
else ifeq ($(KERNEL_ARCH),x86_64)
KERNEL_ARCH_PATH=$(LINUX_DIR)/$(LINUX_VERSION)/arch/x86
else
KERNEL_ARCH_PATH=$(LINUX_DIR)/$(LINUX_VERSION)/arch/$(BR2_ARCH)
endif

ifeq ($(BR2_LINUX_KERNEL_USE_DEFCONFIG),y)
KERNEL_DEFAULT_FILE = $(KERNEL_ARCH_PATH)/configs/$(call qstrip,$(BR2_LINUX_KERNEL_DEFCONFIG))_defconfig
else ifeq ($(BR2_LINUX_KERNEL_USE_CUSTOM_CONFIG),y)
KERNEL_DEFAULT_FILE = $(LINUX_DIR)/$(BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE)
endif

KERNEL_CONFIG_FILE=$(KERNEL_DIR)/.config

ifeq ($(BR2_LINUX_KERNEL_IMAGE_TARGET_CUSTOM),y)
LINUX_IMAGE_NAME=$(call qstrip,$(BR2_LINUX_KERNEL_IMAGE_TARGET_NAME))
else
ifeq ($(BR2_LINUX_KERNEL_UIMAGE),y)
ifeq ($(KERNEL_ARCH),blackfin)
# a uImage, but with a different file name
LINUX_IMAGE_NAME=vmImage
else
LINUX_IMAGE_NAME=uImage
endif
LINUX_DEPENDENCIES+=host-uboot-tools
else ifeq ($(BR2_LINUX_KERNEL_BZIMAGE),y)
LINUX_IMAGE_NAME=bzImage
else ifeq ($(BR2_LINUX_KERNEL_ZIMAGE),y)
LINUX_IMAGE_NAME=zImage
else ifeq ($(BR2_LINUX_KERNEL_VMLINUX_BIN),y)
LINUX_IMAGE_NAME=vmlinux.bin
else ifeq ($(BR2_LINUX_KERNEL_VMLINUX),y)
LINUX_IMAGE_NAME=vmlinux
endif
endif

define LINUX_CHANGELOGS
V1.00:
	1. First release by l00135113.

endef

define LINUX_INSTALL_HEADER_CMDS
	$(Q)cd $$(KERNEL_DIR)/include/;	rm -f asm; ln -sf asm-$(ARCH) asm;\
	cd $$(KERNEL_DIR); $(TARGET_ATP_ENVS) HUAWEI_DRIVERS_DIR="$(HUAWEI_DRIVERS_DIR)" $(MAKE1) ARCH=$(ARCH) silentoldconfig;\
	cp -f $(KERNEL_DIR)/include/linux/autoconf.h $(STAGING_DIR)/usr/include/linux/autoconf.h; \
	cp -Rf $(KERNEL_DIR)/include/mtd $(STAGING_DIR)/usr/include
endef 

# /* <DTS2012122500104   x81004082 2012/12/25 begin*/
define LINUX_BUILD_CMDS
	echo "===aaaa=BUILD_ENDIAN=$(BUILD_ENDIAN)=="
	rm -f $$(LINUX_BDIR)/.stamp_installtarget
	cd $$(KERNEL_DIR)/include/;	rm -f asm; ln -sf asm-$(ARCH) asm
	$(TARGET_ATP_ENVS) HUAWEI_DRIVERS_DIR="$(HUAWEI_DRIVERS_DIR)" $(FAKEROOT) $(MAKE1) -C $(KERNEL_DIR)  ARCH=$(ARCH) modules	
	$(TARGET_ATP_ENVS) HUAWEI_DRIVERS_DIR="$(HUAWEI_DRIVERS_DIR)" $(FAKEROOT) $(MAKE1) -C $(KERNEL_DIR)  ARCH=$(ARCH) modules_install
ifeq ($(PRODUCT),b880)    
	$(HYBRID_CHIP_BUILD_CMDS_EXT)
endif
#/* <DTS2013092310511 z00182709 2013/09/24 begin */
ifeq ($(PRODUCT),b881)    
	cd $(WIFI_DRIVER_PATH) && make;\
	mkdir -p $(TARGET_DIR)/etc/Wireless/RT2860AP;\
	mkdir -p $(TARGET_DIR)/lib/modules/wap;\
	mkdir -p $(TARGET_DIR)/bin;\
	cp -f $(WIFI_DRIVER_PATH)/rootfs/ated $(TARGET_DIR)/bin;\
	cp -f $(WIFI_DRIVER_PATH)/rootfs/RT2860AP.dat $(TARGET_DIR)/etc/Wireless/RT2860AP/RT2860AP.dat;\
	cp -f $(WIFI_DRIVER_PATH)/rootfs/RT30xxEEPROM.bin $(TARGET_DIR)/etc/RT30xxEEPROM.bin;\
	cp -f $(WIFI_DRIVER_PATH)/rootfs/rt5390ap.ko $(TARGET_DIR)/lib/extra/rt5390ap.ko;
endif
#/* DTS2013092310511 z00182709 2013/09/24 end> */
	$(TARGET_ATP_ENVS) HUAWEI_DRIVERS_DIR="$(HUAWEI_DRIVERS_DIR)" $(FAKEROOT) $(MAKE1) -C $(KERNEL_DIR) ARCH=$(ARCH) $(LINUX_IMAGE_NAME)
endef

define LINUX_CLEAN_CMDS
	@echo "=$(LINUX_DIR)==";\
	$(TARGET_ATP_ENVS) HUAWEI_DRIVERS_DIR="$(HUAWEI_DRIVERS_DIR)" $(MAKE1) -C $(KERNEL_DIR) ARCH=mips clean
ifeq ($(PRODUCT),b880)    
	$(HYBRID_CHIP_CLEAN_CMDS_EXT)
endif
	@echo "==clean atpkconfig===";\
	make remove-all-atpkconfigs;
#/* <DTS2013092310511 z00182709 2013/09/24 begin */
ifeq ($(PRODUCT),b881)
	cd $(WIFI_DRIVER_PATH) && (make clean);
endif
#/* DTS2013092310511 z00182709 2013/09/24 end> */
endef
# /* DTS2012122500104   x81004082 2012/12/25 end> */

ifeq ($(BR2_LINUX_KERNEL_VMLINUX),y)
ifeq ($(BUILD_QLZMA),y)
define LINUX_INSTALL_TARGET_CMDS
	cp $(KERNEL_DIR)/vmlinux $(BINARIES_DIR)/.
	cp $(KERNEL_DIR)/System.map $(BINARIES_DIR)/sysmap/
	$(STRIP) --remove-section=.note --remove-section=.comment $(BINARIES_DIR)/vmlinux
	$(OBJCOPY) -O binary $(BINARIES_DIR)/vmlinux $(BINARIES_DIR)/vmlinux.bin
	$(LZMA_CMD) $(call qstrip,$(ATP_LZMA_OPT))

	cp $(BINARIES_DIR)/$(ATP_LINUX_FILENAME) $(BINARIES_DIR)/$(ATP_LINUX_FILENAME).qlzma
	${MKIMAGE_CMD} -A mips -O linux -T kernel -C lzma \
		-a 0x${LDADDR} -e ${ENTRY} -n "Linux Kernel Image"    \
		-d $(BINARIES_DIR)/$(ATP_LINUX_FILENAME).qlzma $(BINARIES_DIR)/$(ATP_LINUX_FILENAME)
	rm -f $(BINARIES_DIR)/$(ATP_LINUX_FILENAME).qlzma 
	rm -f $(BINARIES_DIR)/vmlinux 
	rm -f $(BINARIES_DIR)/vmlinux.bin
endef
else
define LINUX_INSTALL_TARGET_CMDS
	cp $(KERNEL_DIR)/vmlinux $(BINARIES_DIR)/.
	cp $(KERNEL_DIR)/System.map $(BINARIES_DIR)/sysmap/
	$(STRIP) --remove-section=.note --remove-section=.comment $(BINARIES_DIR)/vmlinux
	$(OBJCOPY) -O binary $(BINARIES_DIR)/vmlinux $(BINARIES_DIR)/vmlinux.bin
	$(LZMA_CMD) $(call qstrip,$(ATP_LZMA_OPT))
	rm -f $(BINARIES_DIR)/vmlinux 
	rm -f $(BINARIES_DIR)/vmlinux.bin
endef
endif
else
ifneq ($(BR2_TARGET_ROOTFS_JFFS2),)
define LINUX_INSTALL_TARGET_CMDS
	$(Q)cp $$(KERNEL_ARCH_PATH)/boot/uImage $(BINARIES_DIR)/uImage;\
	cp $(BINARIES_DIR)/uImage $(TARGET_DIR)/uImage;\
	cp $(KERNEL_DIR)/System.map $(BINARIES_DIR)/sysmap/
endef
else
define LINUX_INSTALL_TARGET_CMDS
	$(Q)cp $$(KERNEL_ARCH_PATH)/boot/uImage $(BINARIES_DIR)/uImage;\
	cp $(KERNEL_DIR)/System.map $(BINARIES_DIR)/sysmap/
endef
endif
endif

# Target type
LINUX_TYPE =kernel

$(eval $(call ATPTARGETS,linux,linux))
