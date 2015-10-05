ifdef BCM_KF # defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
#
# Makefile for the Godenrod board common
#

# remove optimization for debug 
#EXTRA_CFLAGS += -O0 -g
EXTRA_CFLAGS    += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)

obj-y	 := lilac_int.o   \
            lilac_spi.o   \
            lilac_setup.o \
            lilac_prom.o  \
			tulip.o	\
			
obj-$(CONFIG_PCI)	 += lilac_pci.o
endif # BCM_KF

