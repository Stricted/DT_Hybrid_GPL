#############################################################
#
# hisi driver
#
#############################################################

#
# Dependencies
#
HISICHIP_DEPENDENCIES = host-fakeroot

#############################################################
### Version and change logs area
HISICHIP_VERSION = V1.01
define HISICHIP_CHANGELOGS
V1.00:
	1. First release by sunbo.

endef
#############################################################

# /* <DTS2012122500227   x81004082 2012/12/25 begin*/

HISICHIP_LICENSE="Huawei private"

ifneq ($(BR2_TARGET_ROOTFS_JFFS2),)
STARTBSP_FILE_NAME=startbsp_custom
else
STARTBSP_FILE_NAME=startbsp_custom_ramdisk
endif

ifeq ($(PRODUCT),b880)
define HYBRID_CHIP_BUILD_CMDS_EXT
	$(Q)$(TARGET_ATP_ENVS) ARCH=$(ARCH) $(MAKE1) -C $(KERNEL_DIR) M=$(HISI_DRIVER_PATH)/ext_int modules
	$(Q)install -m 0777 $(HISI_DRIVER_PATH)/ext_int/hi_ext_int_custom.ko $(TARGET_DIR)/lib/extra
endef

define HYBRID_CHIP_CLEAN_CMDS_EXT
	$(Q)$(TARGET_ATP_ENVS) ARCH=$(ARCH) $(MAKE1) -C $(KERNEL_DIR) M=$(HISI_DRIVER_PATH)/ext_int clean
endef
endif

ifeq ($(PRODUCT),b880)
ifeq ($(MODEL), b880_cube)
WAN_PORT=9
else
WAN_PORT=8
endif
#/* <DTS2013092310511 z00182709 2013/09/24 begin */
else ifeq ($(PRODUCT),b881)
WAN_PORT=8
#/* DTS2013092310511 z00182709 2013/09/24 end> */
else
WAN_PORT=4
endif
ifdef BUILD_VOICE_ATP
HISICHIP_TYPE=kernel
HISICHIP_MAKEFILE_DIR=$(PRODUCT)/$(ATP_CHIP_NAME)/slic
define HISICHIP_INSTALL_HEADER_CMDS
	$(Q)install -m 0777 $(HISI_DRIVER_PATH)/so/* $(STAGING_DIR)/lib/
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/*.h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/common/*.h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/k_space/hi_kernel_slic.h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/u_space/hi_slic.h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/u_space/hi_voip.h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/u_space/basic/os/*h $(STAGING_DIR)/usr/include
	$(Q)install -m 770  $(HISI_DRIVER_PATH)/include/u_space/basic/*h $(STAGING_DIR)/usr/include
endef
endif

define HISICHIP_BUILD_CMDS
	$(Q)install -m 0777 $(HISI_DRIVER_PATH)/so/* $(STAGING_DIR)/lib/
endef

# /* 根据产品名称进行编译控制，兼容b890 */
#DTS2013070305250 j81004134 begin 2013/07/04
define HISICHIP_INSTALL_TARGET_CMDS
	$(Q)install -m 0770  $(HISI_DRIVER_PATH)/include/*.h $(STAGING_DIR)/usr/include
	$(Q)install -m 0770  $(HISI_DRIVER_PATH)/include/common/*.h $(STAGING_DIR)/usr/include
	$(Q)install -m 0770  $(HISI_DRIVER_PATH)/include/k_space/sdk/sd5115/*.h $(STAGING_DIR)/usr/include
	$(Q)install -m 0777 $(HISICHIP_DIR)/$(PRODUCT)/startnat* $(TARGET_DIR)/bin;\
	install -m 770 $(HISICHIP_DIR)/$(PRODUCT)/$(STARTBSP_FILE_NAME) $(TARGET_DIR)/bin;\
	mv $(TARGET_DIR)/bin/$(STARTBSP_FILE_NAME) $(TARGET_DIR)/bin/startbsp
	$(Q)install -m 0777 $(HISI_DRIVER_PATH)/bin/* $(TARGET_DIR)/bin;\
	install -m 0777 $$(HISICHIP_DIR)/$(PRODUCT)/profile $(TARGET_DIR)/etc/profile;\
	mkdir -p $(TARGET_DIR)/lib/extra;\
	install -m 0777 $(HISI_DRIVER_PATH)/ko/* $(TARGET_DIR)/lib/extra;\
	install -m 0777 $(HISI_DRIVER_PATH)/so/* $(TARGET_DIR)/lib/;\
	install -m 0777 $(HISI_DRIVER_PATH)/slic/*.ko $(TARGET_DIR)/lib/extra;\
	mkdir -p $(TARGET_DIR)/home;\
	cp -Rf $(HISI_DRIVER_PATH)/script/cli $(TARGET_DIR)/home;\
	install -m 770 	$$(HISICHIP_DIR)/device_table_hisi.dynamic $(DEVICETABLE_DIR)
	$(Q)cd $(TARGET_DIR);\
	rm -f init;\
	ln -s ./bin/busybox init;
ifndef BUILD_VOICE_ATP
	rm -f $(TARGET_DIR)/bin/zsp_min.out;\
	rm -f $(TARGET_DIR)/bin/hi_voip_socket;\
	rm -f $(TARGET_DIR)/bin/hi_voip;\
	rm -f $(TARGET_DIR)/lib/extra/hi_kvoip.ko;\
	rm -f $(TARGET_DIR)/lib/extra/hi_kslic.ko;\
	rm -f $(TARGET_DIR)/lib/libhi_voip.so;\
	rm -f $(TARGET_DIR)/lib/libhi_slic.so;
endif
	cd $(TARGET_DIR)/dev;\
	rm -f console;\
	fakeroot mknod console c 5 1
endef
#DTS2013070305250 j81004134 end 2013/07/04
# /* DTS2012122500227   x81004082 2012/12/25 end> */

$(eval $(call ATPTARGETS,driver/hisi,hisichip,HISI_CHIP))
