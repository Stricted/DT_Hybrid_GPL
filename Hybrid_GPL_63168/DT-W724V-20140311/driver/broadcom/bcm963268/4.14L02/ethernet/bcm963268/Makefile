# File: bcmdrivers/broadcom/net/enet/bcm963xx/Makefile
#
# Makefile for the BCM63xx Enet/switch driver
#

#bcm_enet-objs +=  ../../../../../../package/atp/configuration/cmsmain/kernel/hw_enet.o
bcm_enet-objs += bcmenet.o
bcm_enet-objs += ethsw.o
bcm_enet-objs += bcmsw.o
bcm_enet-objs += ../shared/bcmswaccess.o

# Runner based platform specific object files
ifeq ($(strip $(BRCM_CHIP)),68500)
bcm_enet-objs += ethsw_phy_runner.o
bcm_enet-objs += bcmenet_runner.o
bcm_enet-objs += ethsw_runner.o
bcm_enet-objs += bcmsw_runner.o
# DSL/6818/6828 specific files
else ifeq ($(strip $(BRCM_CHIP)),6838)
bcm_enet-objs += ethsw_phy_runner.o
bcm_enet-objs += bcmenet_runner.o
bcm_enet-objs += ethsw_runner.o
bcm_enet-objs += bcmsw_runner.o
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/bcm968500/os
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/bcm968500/base
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/rdp
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/drv/egphy
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/drv
else
bcm_enet-objs += ../shared/bcmswaccess_dma.o
bcm_enet-objs += ../shared/bcmproc.o
bcm_enet-objs += ethsw_phy.o
bcm_enet-objs += bcmenet_dma.o
bcm_enet-objs += ethsw_dma.o
bcm_enet-objs += bcmsw_dma.o
endif

ifneq ($(strip $(CONFIG_BCM_GMAC)),)
bcm_enet-objs += bcmgmac.o
endif

ifneq ($(strip $(BUILD_ETHTOOL)),)
bcm_enet-objs += ../shared/bcmenet_ethtool.o
endif

ifneq ($(strip $(CONFIG_BCM_ENET)),)
obj-$(CONFIG_BCM_ENET) = bcm_enet.o
endif

#-DDUMP_DATA  -- dump data
#-DUSE_BH     -- use bottom half
#-DUSE_PROC   -- display ethernet driver internal info
#-DUSE_EMAC   -- need MDC/MDIO functions
#-DBCM_ENET_RX_LOG   -- Enable rx debug log
#-DBCM_ENET_LOG      -- Enable debug log
#-DBCM_ENET_TX_LOG   -- Enable tx debug log
#-DBCM_ENET_LINK_LOG   -- Enable link debug log
#-DBCM_ENET_DEBUG_BUILD   -- Enable Debug Build
#-DRXCHANNEL_BYTE_RATE_LIMIT -- Enable rx byte rate limiting of a channel
#-DRXCHANNEL_PKT_RATE_LIMIT -- Enable rx pkt rate limiting of a channel
#-DVLAN_TAG_FFF_STRIP -- Enable Stripping of 0xFFF tagged packets
#-DUSE_DEFAULT_EGRESS_QUEUE -- Enable use of default egress queue
#-DSEPARATE_MAC_FOR_WAN_INTERFACES -- Enable separate MAC addresses for WAN ifs

ifneq ($(strip $(BUILD_HELLO)),)
# Enable support for HELLO Packet CMF
EXTRA_CFLAGS += -DSUPPORT_HELLO
endif

ifneq ($(strip $(BUILD_SWMDK)),)
EXTRA_CFLAGS += -DSUPPORT_SWMDK
endif

ifneq ($(strip $(BUILD_ETHTOOL)),)
EXTRA_CFLAGS += -DSUPPORT_ETHTOOL
endif


EXTRA_CFLAGS += -DUSE_BH
EXTRA_CFLAGS += -DUSE_VPORTS
EXTRA_CFLAGS += -DEXPORT_SYMTAB
#EXTRA_CFLAGS += -Werror
EXTRA_CFLAGS += -g
#EXTRA_CFLAGS +=  -DBCM_ENET_DEBUG_BUILD
#EXTRA_CFLAGS +=  -DBCM_ENET_RX_LOG
#EXTRA_CFLAGS +=  -DBCM_ENET_TX_LOG
EXTRA_CFLAGS +=  -DBCM_ENET_LOG
#EXTRA_CFLAGS +=  -DRXCHANNEL_BYTE_RATE_LIMIT
EXTRA_CFLAGS +=  -DRXCHANNEL_PKT_RATE_LIMIT
EXTRA_CFLAGS +=  -DVLAN_TAG_FFF_STRIP
EXTRA_CFLAGS +=  -DUSE_DEFAULT_EGRESS_QUEUE 
EXTRA_CFLAGS += -DSEPARATE_MAC_FOR_WAN_INTERFACES
# Stats from HW is supported only from Switch at this time; not from Runner
# This needs to be looked into again in 4.14L.02 when flows will be acceleated/switched by Runner
ifeq ($(strip $(BRCM_CHIP)),68500)
ifneq ($(strip $(BRCM_EXT_SWITCH_TYPE)),)
EXTRA_CFLAGS += -DREPORT_HARDWARE_STATS
endif
else
EXTRA_CFLAGS += -DREPORT_HARDWARE_STATS
endif
#EXTRA_CFLAGS += -DDUMP_DATA
#EXTRA_CFLAGS += -DNON_LINUX_BUILD
#EXTRA_CFLAGS += -DNO_CFE

ifneq ($(strip $(BRCM_CONFIG_HIGH_RES_TIMERS)),)
EXTRA_CFLAGS += -DMOCA_HIGH_RES_TX
endif

ifneq ($(strip $(BRCM_SWITCH_SCHED_SP)),)
EXTRA_CFLAGS += -DBCM_SWITCH_SCHED_SP
endif
ifneq ($(strip $(BRCM_SWITCH_SCHED_WRR)),)
EXTRA_CFLAGS += -DBCM_SWITCH_SCHED_WRR
endif


EXTRA_CFLAGS += -I$(obj) -I$(obj)/../shared
EXTRA_CFLAGS += -I$(STAGING_INC_DIR)
#EXTRA_CFLAGS += -I$(INC_DRIVER_VENDOR_DIR) -I$(INC_DRIVER_HUAWEI_DIR) -I$(INC_DRIVER_HUAWEI_PRD_DIR) -I$(INC_APP_DIR)
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/xtmrt/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268
EXTRA_CFLAGS += -I$(DRIVER_VENDOR_DIR)/fap/bcm963268/4ke       
#EXTRA_CFLAGS += -I$(DRIVER_HUAWEI_BHAL_ADAPTER_DIR)

#EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/include/$(BRCM_BOARD)
#ifeq ($(CONFIG_BCM_XTMRT_IMPL),1)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/xtmrt/impl3
#else
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/xtmrt/impl$(CONFIG_BCM_XTMRT_IMPL)
#endif
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)
#EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/char/fap/impl$(CONFIG_BCM_FAP_IMPL)/4ke
EXTRA_CFLAGS += -I$(KERNEL_DIR)/net/core
ifneq ($(strip $(CONFIG_BCM_RDPA)),)
EXTRA_CFLAGS += $(EXTRA_CFLAGS_RDPA)
endif

INC_SHARED_968500_PATH=$(INC_BRCMSHARED_PUB_PATH)/bcm968500

ifeq ($(strip $(BRCM_CHIP)),68500)
EXTRA_CFLAGS += -I$(INC_SHARED_968500_PATH)/brd
EXTRA_CFLAGS += -I$(INC_SHARED_968500_PATH)/drv
EXTRA_CFLAGS += -I$(INC_SHARED_968500_PATH)/dpd
endif

ifneq ($(strip $(CONFIG_BCM_WLAN)),)
EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/net/wl/impl$(CONFIG_BCM_WLAN_IMPL)/include
EXTRA_CFLAGS += -DDSLCPE -DBCMDRIVER -DPKTC
endif

-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s

