# Makefile for the huawei drivers
#
unexport bhal_dir
bhal_dir = $(CHIP_NAME)

subdir-y = $(CHIP_NAME)

obj-y += atpflash.o atpnandparts.o

######### suport brcm special

ifeq ("$(strip $(CHIP_NAME))","bcm963268")  
obj-y += $(bhal_dir)/boardparms.o $(bhal_dir)/bcm63xx_led.o bhal.o $(bhal_dir)/bhalplat.o $(bhal_dir)/boardparms_voice.o
endif

ifeq ("$(strip $(CHIP_NAME))","bcm96362")
obj-y += $(bhal_dir)/boardparms.o $(bhal_dir)/bcm63xx_led.o bhal.o $(bhal_dir)/bhalplat.o $(bhal_dir)/boardparms_voice.o
endif

ifeq ("$(strip $(CHIP_NAME))","bcm96318") 
obj-y += $(bhal_dir)/boardparms.o $(bhal_dir)/bcm63xx_led.o bhal.o $(bhal_dir)/bhalplat.o
endif

ifeq ("$(strip $(CHIP_NAME))","rt63365")
obj-y += $(bhal_dir)/rt63365_led.o bhal.o $(bhal_dir)/bhalplat.o
endif

ifeq ("$(strip $(CHIP_NAME))","ar9342")
obj-y += $(bhal_dir)/ar9342_led.o bhal.o $(bhal_dir)/bhalplat.o
endif

ifneq ($(strip $(CONFIG_ATP_BOARDLED)),)
#obj-y += boardled.o
endif

EXTRA_CFLAGS += -I. -I$(DRIVER_HUAWEI_DIR)/include -I$(INC_DRIVER_VENDOR_DIR)  -I$(KERNEL_DIR)/include -I$(DRIVER_HUAWEI_DIR)/flash \
                -I$(DRIVER_HUAWEI_BHAL_ADAPTER_DIR) -I$(INC_FLASHCONF_DIR) -I$(STAGING_INC_DIR)


ifneq ($(BUILD_WLAN_APCLI),)
CFLAGS += -DSUPPORT_ATP_WLAN_APCLI
endif

ifneq ($(BUILD_WLAN_AP),)
	EXTRA_CFLAGS += -DSUPPORT_ATP_WLAN_AP
endif

ifneq ($(BUILD_EQUIP_WLAN_FLAG),)
CFLAGS += -DSUPPORT_ATP_EQUIP_WLAN_FLAG
endif

ifneq ($(BUILD_EQUIP_PORT_CHK_FLAG),)
CFLAGS += -DSUPPORT_ATP_EQUIP_PORT_CHK_FLAG
endif

clean:
	rm -f *.o *.cmd
