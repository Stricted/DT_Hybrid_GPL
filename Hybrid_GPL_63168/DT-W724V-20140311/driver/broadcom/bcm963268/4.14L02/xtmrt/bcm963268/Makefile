# File: bcmdrivers/opensource/net/xtmrt/Makefile
#
# Makefile for the BCM63xx bcmxtmrt driver.
#

obj-y := bcmxtmrt.o bcmxtmrtbond.o

EXTRA_CFLAGS += -DDPERF_MON_BONDING_US -DDPTMBOND_US_PRIO_TRAFFIC_SPLIT
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/enet/shared
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/xtmrt/impl$(CONFIG_BCM_XTMRT_IMPL)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)/4ke
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/xtmrt/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268/4ke 
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/ethernet/shared 

EXTRA_CFLAGS += -Werror -Wfatal-errors
ifeq "$(BRCM_PHY_LOOPBACK)" "y"
EXTRA_CFLAGS += -DPHY_LOOPBACK
endif

clean:
	rm -f core *.o *.a *.s

