# File: bcmdrivers/opensource/char/pktdma
#
# Makefile for the BCM63xx pktdma driver

obj-$(CONFIG_BCM_PKTDMA) += bcmPktDmaEthIudma.o
obj-$(CONFIG_BCM_PKTDMA) += bcmPktDmaBds.o

#ifneq ($(strip $(BRCM_DRIVER_XTM)),)
#ifneq ($(strip $(BCM_XTMCFG))$(strip $(BCM_XTMCFG_MODULE)),)
obj-$(CONFIG_BCM_PKTDMA) += bcmPktDmaXtmIudma.o
#endif
#endif

ifneq ($(strip $(CONFIG_BCM_FAP)),)
obj-$(CONFIG_BCM_PKTDMA) += bcmPktDmaEthDqm.o
obj-$(CONFIG_BCM_PKTDMA) += bcmPktDma.o
#ifneq ($(strip $(BRCM_DRIVER_XTM)),)
obj-$(CONFIG_BCM_PKTDMA) += bcmPktDmaXtmDqm.o
#endif
endif
#EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) 
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/enet/shared
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)/4ke
#EXTRA_CFLAGS  += -I$(INC_DRIVER_VENDOR_DIR) -I$(INC_DRIVER_HUAWEI_PRD_DIR) -I$(INC_APP_DIR) -I$(DRIVER_VENDOR_DIR)/fap/$(SUB_CHIP) \ 
EXTRA_CFLAGS  += -I$(DRIVER_VENDOR_DIR)/fap/$(SUB_CHIP)  \
                 -I$(DRIVER_VENDOR_DIR)/fap/$(SUB_CHIP)/4ke -I$(DRIVER_VENDOR_DIR)/xtmrt/$(SUB_CHIP) -I$(DRIVER_VENDOR_DIR)/ethernet/shared

#ifeq ($(CONFIG_BCM_XTMRT_IMPL),1)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/xtmrt/impl3
#else
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/xtmrt/impl$(CONFIG_BCM_XTMRT_IMPL)
#endif
#EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

EXTRA_CFLAGS += -Werror -Wall -Wfatal-errors


-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s

