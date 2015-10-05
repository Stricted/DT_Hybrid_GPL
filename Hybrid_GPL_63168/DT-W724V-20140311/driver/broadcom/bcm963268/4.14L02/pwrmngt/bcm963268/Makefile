# File: bcmdrivers/char/pwrmngt/bcm963xx/Makefile
#
# Makefile for the Power Management Module
#

pwrmngtd-objs   := pwrmngtdrv.o
pwrmngtd-objs   += pwrmngt.o

ifneq ($(CONFIG_BCM_AVS_PWRSAVE),)
pwrmngtd-objs   += pwrmngtavs.o

COMPILE_40NM_AVS=n
COMPILE_65NM_AVS=n

# Checks for 65NM compilation
ifeq ($(strip $(BRCM_CHIP)),6368)
COMPILE_65NM_AVS=y
endif

ifeq ($(strip $(BRCM_CHIP)),6362)
COMPILE_65NM_AVS=y
endif

ifeq ($(strip $(BRCM_CHIP)),6328)
COMPILE_65NM_AVS=y
endif

ifeq ($(strip $(BRCM_CHIP)),6816)
COMPILE_65NM_AVS=y
endif

ifeq ($(strip $(BRCM_CHIP)),6818)
COMPILE_65NM_AVS=y
endif

ifeq ($(strip $(BRCM_CHIP)),63268)
COMPILE_65NM_AVS=y
endif

# Checks for 40NM compilation
ifneq ($(strip $(BUILD_6802_MOCA)),)
COMPILE_40NM_AVS=y
EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/char/board/bcm963xx/impl1/bbsi
EXTRA_CFLAGS += -DBCHP_CHIP=6802 -DBCHP_FAMILY=9999
else
EXTRA_CFLAGS += -DBCHP_CHIP=$(BRCM_CHIP) -DBCHP_FAMILY=9999
endif


ifeq ($(strip $(BRCM_CHIP)),6318)
COMPILE_40NM_AVS=y
endif

ifeq ($(strip $(COMPILE_65NM_AVS)),y)
pwrmngtd-objs += avs_65/avs_65.o
EXTRA_CFLAGS += -DRUN_AVS_65
endif

ifeq ($(strip $(COMPILE_40NM_AVS)),y)
pwrmngtd-objs += avs_40/avs_start.o
EXTRA_CFLAGS += -DRUN_AVS_40
endif

endif

obj-$(CONFIG_BCM_PWRMNGT) += pwrmngtd.o

EXTRA_CFLAGS += -I.
EXTRA_CFLAGS += -I./avs_40
EXTRA_CFLAGS += -I$(HOST_INCLUDE_DIR)
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/ethernet/shared 

# EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/net/enet/shared 
# EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD)
# EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
# EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) 
# EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -fno-exceptions
EXTRA_CFLAGS += -Werror -Wfatal-errors

 #-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s avs*/*.o *.ko

